/************************************************************************************************
PROJECT : simple_draw
FILE    : $Id: main.c 1164 2013-07-31 08:29:57Z florian.zimmermann $
============================================================================ 
DESCRIPTION
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.
****************************************************************************
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_bsp_api.h"         /* Board support package */
#include "r_bsp_hmi_api.h"     /* Board support package - HMI (Buttons + Rotary) */
#include "r_ddb_api.h"
#include "r_cdi_api.h"
#include "r_wm_api.h"
#include "davehd_driver.h"

#include "main.h"
#include "wm.h"
#include "mem_init.h"
#include "r_util_dhd.h"

#include "config.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"

#include "r_tick_api.h"

#include "r_jcua_api.h"
#include "r_bsp_stdio_api.h"
#include "r_wm_sys.h"

#include "bsp.h"
#include "system.h"
#include "write.h"
#include <math.h>

/***********************************************************
  Section: Local Defines
*/

/* bitmap widths */
#define LOC_KING_W    (510)
#define LOC_GEAR_W    (56)
/* bitmap heights */
#define LOC_KING_H    (141)
#define LOC_GEAR_H    (48)
/* bitmap sizes */
#define LOC_KING_SIZE (LOC_KING_W * LOC_KING_H * 4)
#define LOC_GEAR_SIZE (LOC_GEAR_W * LOC_GEAR_H * 4)

#define LOC_SF_BMP_W   (104)
#define LOC_SF_BMP_H   (104)
#define LOC_SF_ADDRESS_EX (0x10000000)


/*******************************************************************************
  Define: LOC_DL_CHANGE
  
  0 : not change display list in executing.
  1 : change display list in executing.
*/
#define LOC_DL_CHANGE         (1)

#define LOC_ON_SPRITE       0
#define LOC_OFF_SPRITE      1
#define LOC_RESUME_SPRITE   2
#define LOC_PAUSE_SPRITE    3
#define LOC_SEL_SPRITE      4

#define LOC_SEL_DIM         8

/***********************************************************
  Section: Local Variables
*/

/***********************************************************

  Variable: loc_WmWindow

  Pointer to the Window handles
*/
r_wm_Window_t loc_WmWindow;

/***********************************************************

  Variable: loc_Drw2dDev

  Pointer to the Drw2D device handle
*/

static r_drw2d_Device_t loc_Drw2dDev;

/***********************************************************
  Variable: loc_Fb

  Framebuffer information for Drw2D
*/
static r_drw2d_Framebuffer_t loc_Fb;

/***********************************************************
  Variable: g_BlackKingBmp

  the Bitmap (external)
*/

extern r_cdi_Heap_t       loc_lRAM_heap;
extern r_cdi_Heap_t       loc_VRAM_heap;
extern uint32_t             loc_VRAM0;

extern void System_Init(void);

/***********************************************************
  Variable: loc_DHDDev

  DHD Drawing handle
*/
static struct r_drw2d_DeviceDHD_s loc_DHDDev;

/***********************************************************
  Variable: loc_ExitFlag

  flag to test exit pathes
*/
//static int8_t loc_ExitFlag = R_FALSE;

/***********************************************************
  Section: Local Functions
*/

/***********************************************************
 Function: loc_DRW2DErrorHandler

 Error handler used for Drw2d errors
*/
static r_drw2d_Boolean_t loc_DRW2DErrorHandler(r_drw2d_Error_t  Error, void *UserData)
{
    loc_Error(1);
    return R_TRUE;
}

/*******************************************************************************
  Function: loc_RenderClear

  Render function called each frame
*/
void loc_RenderClear(int32_t Frame)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;

    /* Set green-ish clear color and fill framebuffer */
    ret |= R_DRW2D_CtxBgColor(loc_Drw2dDev, 0xff000000);
    ret |= R_DRW2D_FramebufferClear(loc_Drw2dDev);

    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}

void start_draw(void)
{
    loc_Fb.Buffer.Data = R_WM_WindowNewDrawBufGet(LOC_WM_UNIT, &loc_WmWindow);
    R_DRW2D_FramebufferSet(loc_Drw2dDev, &loc_Fb);
}

void end_draw(void)
{
    R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
    R_WM_WindowSwap(LOC_WM_UNIT, &loc_WmWindow);
    R_WM_FrameEndMark(LOC_WM_UNIT, 0);
    R_WM_FrameWait(LOC_WM_UNIT, 0);
}

static void loc_init(void)
{
    uint32_t frame;
    int32_t ret = R_DRW2D_ERR_OK;

    /* Init framebuffer struct */
    loc_Fb.Flags  = (r_drw2d_FramebufferFlags_t)0;
    loc_Fb.Handle = 0; /* not used currently */
    loc_Fb.Buffer.Pitch       = LOC_DISPLAY_STRIDE;
    loc_Fb.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
    loc_Fb.Buffer.Size.Width  = LOC_DISPLAY_WIDTH;
    loc_Fb.Buffer.Size.Height = LOC_DISPLAY_HEIGHT;
    loc_Fb.Buffer.Data        = 0;

    loc_InitWM(&loc_lRAM_heap, &loc_VRAM_heap);
    R_WM_Sys_ScreenColorFormatSet(0, R_WM_OUTCOLORFMT_FLAG_MASK);
    R_WM_FrameEndMark(LOC_WM_UNIT, 0);
    R_WM_FrameWait(LOC_WM_UNIT, 0);

    R_BSP_STDIO_Printf("Now using VOWE, JCUA and WM\n");

    loc_CreateWindow(R_WM_WINMODE_FB,
                     0,
                     0,
                     0,
                     LOC_DISPLAY_WIDTH,
                     LOC_DISPLAY_HEIGHT,
                     LOC_DISPLAY_STRIDE,
                     R_WM_COLORFMT_ARGB8888,
                     &loc_WmWindow);

    /* start up gfx engine */
    ret |= R_DRW2D_Init();
    ret |= R_DRW2D_GlobalErrCallbackSet(&loc_DRW2DErrorHandler, R_NULL);
    ret |= R_DRW2D_Open(LOC_DRW2D_UNIT, R_DRW2D_UNIT_DHD0, &loc_DHDDev, &loc_Drw2dDev);

    loc_RenderClear(1);
}

/*******************************************************************************
  Function: loc_Error

  This function is called when error occurs
  The application hangs in there.
*/
void loc_Error(int32_t Err)
{
    while (Err)
    {
    }
}

int32_t init(void)
{

    R_BSP_Init();

    loc_SetupMemManager();
    R_UTIL_DHD_Init(LOC_DHD_UNIT);
    R_UTIL_DHD_Config((dhd_gpu_ptr_t)loc_VRAM0, DHD_MEMORY_SIZE, &loc_lRAM_heap);

    loc_init();

	return 0;
}

int32_t de_init(void)
{
    R_UTIL_DHD_DeInit(LOC_DHD_UNIT);
	return 0;
}


