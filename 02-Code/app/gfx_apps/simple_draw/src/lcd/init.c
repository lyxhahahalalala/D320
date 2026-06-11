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


#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_bsp_api.h"         /* Board support package */
#include "r_bsp_hmi_api.h"     /* Board support package - HMI (Buttons + Rotary) */
#include "r_bsp_stdio_api.h"
#include "r_ddb_api.h"
#include "r_cdi_api.h"
#include "r_wm_api.h"
#include "r_wm_sys.h"
#include "davehd_driver.h"

#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"
#include "r_jcua_api.h"
#include "r_sfma_api.h"
#include "r_util_dhd.h"
#include "r_tick_api.h"

#include "main.h"
#include "wm.h"
#include "config.h"
#include "mem_init.h"
#include "img_format.h"
#include "write.h"
#include "init.h"
#include "zk.h"

#include "string.h"
#include "bsp.h"
#include "SYSTEM.h"
#include "motor_sm.h"
#include "app_tpmsmain.h"
#include "uds_support.h"
#include "app_can.h"
#include "pwm.h"
#include "ptmr.h"
#include "app_data_center.h"



unsigned int color_convert(unsigned char color_src);

/***********************************************************
  Section: Local Defines
  */


#if !defined USE_INTERNAL_ROM

#define TRUCKBASEADDR          0
#define TRUCKLENGTH            24640
#define SFMAUNIT               0
#define LEFTOFFROADADDR        26*1024
#define LEFTOFFROADADDRLENGTH  88480
#define RIGHTOFFROADADDR       (26+88*2)*1024
#define FRONTOFFROADADDR       (26+88)*1024

uint8_t SPI_Image[1024*100] __attribute__((aligned(8))) ={0};
static uint8_t * locImgPtr = (uint8_t*) SPI_Image;
Img_t Img_start = {
    "start_00009_03.bin",
    0,
    276,
    276,
    IMG_ARGB4444,
    IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY,
    0,
    0,
    0,
    &locImgPtr,
    0//152352
};
const Img_t Img_truck = {
    "truck.bin",
    0,
    96,
    100,
    IMG_RGB565,
    IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY,
    0,
    0,
    0,
    &locImgPtr,
    19200
};
const Img_t Img_leftOffset = {
    "leftOffset.bin",
    0,
    148,
    158,
    IMG_RGB565,
    IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY,
    0,
    0,
    0,
    &locImgPtr_lo,
    46768
};
const Img_t Img_rightOffset = {
    "rigthOffset.bin",
    0,
    148,
    158,
    IMG_RGB565,
    IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY,
    0,
    0,
    0,
    &locImgPtr_ro,
    46768
};
const Img_t Img_frontCollision = {
    "leftOffset02.bin",
    0,
    280,
    158,
    IMG_RGB565,
    IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY,
    0,
    0,
    0,
    &locImgPtr,
    88480
};
const Img_t Img_TPMS = {
    "cartire.bin",
    0,
    100,
    176,
    IMG_ARGB4444,
    IMG_ATTRIBUTE_USE_FROM_PERSISTENT_MEMORY,
    0,
    0,
    0,
    &locImgPtr,
    35200
};
#endif

static struct r_drw2d_DeviceDHD_s loc_DHDDev;
static r_drw2d_Framebuffer_t loc_Fb;

r_drw2d_Device_t    loc_Drw2dDev;

extern r_cdi_Heap_t loc_lRAM_heap;
extern r_cdi_Heap_t loc_VRAM_heap;
extern uint32_t     loc_VRAM0;

extern const Font_t Font_TellTable;
extern const Font_t Font_BigTellTable;
extern const Font_t Font_Tire;
extern const Font_t Font_TRUCK ;
extern const Font_t FontLogo;
extern const Font_t FontRound;
extern const Font_t Font_Text;
extern const Font_t Font_LowBatt;
extern const Font_t Font_IntelligentDriving;
extern const Font_t Font_AuxiliaryBraking;


r_wm_Window_t loc_WmWindow;
r_wm_Window_t loc_SpriteWindow, loc_SpriteWindow1, loc_SpriteWindow2;
r_wm_Sprite_t s_atSprite[3][16];


enum{
	MAIN_VIEW_SPRITE_ID_BG = 0,    //背景
	MAIN_VIEW_SPRITE_ID_SPD_H,     //速度高位
	MAIN_VIEW_SPRITE_ID_SPD_M,     //速度中位
	MAIN_VIEW_SPRITE_ID_SPD_L,     //速度低位
	MAIN_VIEW_SPRITE_ID_BattSoc,   //电池soc背景
	MAIN_VIEW_SPRITE_ID_H2Soc_MASK,//氢气SOC背景
	MAIN_VIEW_SPRITE_ID_Motor_MASK,//电机背景
	
	MAIN_VIEW_SPRITE_ID_LDWS_LEFT, //红色左车道线
	MAIN_VIEW_SPRITE_ID_LDWS_RIGHT,//红色右车道线
	
	MENU_VIEW_SPRITE_ID_BG_L1,     //菜单背景线
	MENU_VIEW_SPRITE_ID_BG_L2,     //菜单背景线
	
	MENU_VIEW_SPRITE_ID_BOX,       //选中框
	MENU_VIEW_SPRITE_ID_Choose,    //选中符号
	
	CHARGE_VIEW_SPRITE_ID_SOC_BG,  //充电界面SOC电池背景
	
	VIEW_SPRITE_ID_COUNT
	
};
const spriteRes_t mainViewSprites[] = {
    								  //  x,   y, Win, spn, posZ
    [MAIN_VIEW_SPRITE_ID_BG] =         {  0,   0,  0,   0,   0},     // 第一层1
	
    [MAIN_VIEW_SPRITE_ID_SPD_H] =      {304, 121,  1,   0,   0},
    [MAIN_VIEW_SPRITE_ID_SPD_M] =      {368, 121,  1,   1,   0},
    [MAIN_VIEW_SPRITE_ID_SPD_L] =      {432, 121,  1,   2,   0},
	[MAIN_VIEW_SPRITE_ID_BattSoc] =    { 20, 275,  1,   3,   0},
	[MAIN_VIEW_SPRITE_ID_H2Soc_MASK] = {620, 275,  1,   4,   0},
   	[MAIN_VIEW_SPRITE_ID_Motor_MASK] = {620, 275,  1,   5,   0},
	[MAIN_VIEW_SPRITE_ID_LDWS_LEFT]  = {181, 235,  1,   6,   1},
	[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT] = {461, 235,  1,   7,   1},
	
	[MENU_VIEW_SPRITE_ID_BG_L1] =      {  0,  60,  1,   8,   0},
    [MENU_VIEW_SPRITE_ID_BG_L2] =      {  0, 425,  1,   9,   0},
	
	[MENU_VIEW_SPRITE_ID_BOX] =        { 22, 124,  1,  10,   0},
    [MENU_VIEW_SPRITE_ID_Choose] =     {270, 134,  1,  11,   0},
	
   	[CHARGE_VIEW_SPRITE_ID_SOC_BG] =   {296, 143,  1,  12,   0},


};

const Img_t * const frmMainSpdNumImgs[] = {
    &Img_spdn0,
    &Img_spdn1,
    &Img_spdn2,
    &Img_spdn3,
    &Img_spdn4,
    &Img_spdn5,
    &Img_spdn6,
    &Img_spdn7,
    &Img_spdn8,
    &Img_spdn9,
};


static r_drw2d_Boolean_t loc_DRW2DErrorHandler(r_drw2d_Error_t  Error, void *UserData)
{
    loc_Error(1);
    return R_TRUE;
}


void loc_Error(int32_t Err)
{
    while (Err)
    {
    }
}


void start_draw(void)
{
    R_WM_FrameWait(LOC_WM_UNIT, 0);
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


void loc_RenderClear(int32_t Frame)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;

    /* Set green-ish clear color and fill framebuffer */
    ret |= R_DRW2D_CtxBgColor(loc_Drw2dDev, Frame);
    ret |= R_DRW2D_FramebufferClear(loc_Drw2dDev);

    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}
void loc_ClearRect(int32_t x, int32_t y, int32_t w, int32_t h)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;

    /* rect */
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
    ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, 0x00000000); //0x33333333
    rect.Pos.X       = R_DRW2D_2X(x);
    rect.Pos.Y       = R_DRW2D_2X(y);
    rect.Size.Width  = R_DRW2D_2X(w);
    rect.Size.Height = R_DRW2D_2X(h);
    ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    // R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
    
    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}
void vw_clearFont31Value(int32_t x, int32_t y, uint8_t charNum)
{
    loc_ClearRect(x - 32 * charNum, y, 32 * charNum, 31);
}


void loc_RenderRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;

    /* rect */    
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
    ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, color);
    rect.Pos.X       = R_DRW2D_2X(x);
    rect.Pos.Y       = R_DRW2D_2X(y);
    rect.Size.Width  = R_DRW2D_2X(w);
    rect.Size.Height = R_DRW2D_2X(h);
    ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);

    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}
void loc_RenderTriangles(Coord_t *coord, uint32_t color)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_Point_t triangle[3];
	
	/* triangle */
	ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
	ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
	ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, color);
	triangle[0].X = R_DRW2D_2X(coord[0].x);
	triangle[0].Y = R_DRW2D_2X(coord[0].y);
	triangle[1].X = R_DRW2D_2X(coord[1].x);
	triangle[1].Y = R_DRW2D_2X(coord[1].y);
	triangle[2].X = R_DRW2D_2X(coord[2].x);
	triangle[2].Y = R_DRW2D_2X(coord[2].y);
	ret |= R_DRW2D_DrawTriangles(loc_Drw2dDev, triangle, 3, 0);
	R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
	
	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
	
}
void loc_RenderQuads(Coord_t *coord, uint32_t color)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_Point_t point[4];
	
	/* Quads */
	ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
	ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
	ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, color);
	point[0].X = R_DRW2D_2X(coord[0].x);
	point[0].Y = R_DRW2D_2X(coord[0].y);
	point[1].X = R_DRW2D_2X(coord[1].x);
	point[1].Y = R_DRW2D_2X(coord[1].y);
	point[2].X = R_DRW2D_2X(coord[2].x);
	point[2].Y = R_DRW2D_2X(coord[2].y);
	point[3].X = R_DRW2D_2X(coord[3].x);
	point[3].Y = R_DRW2D_2X(coord[3].y);
	ret |= R_DRW2D_DrawQuads(loc_Drw2dDev, point, 4, 0);
	R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
	
	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
	
}


void loc_RenderLines(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1,  unsigned int color)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_LineStyle_t LineStyle;
	r_drw2d_Point_t point[2];
	
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
    ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, color);
    LineStyle.LineCap    = R_DRW2D_LINECAP_ROUND; //R_DRW2D_LINECAP_BUTT; //R_DRW2D_LINECAP_SQUARE; //R_DRW2D_LINECAP_ROUND;
    LineStyle.LineJoin   = R_DRW2D_LINEJOIN_NONE;
    LineStyle.MiterLimit = R_DRW2D_2X(1);;
    LineStyle.Width      = R_DRW2D_2X(10);
	LineStyle.IsClosed   = 0;
	ret |= R_DRW2D_CtxLineStyle(loc_Drw2dDev, &LineStyle);
	point[0].X           = R_DRW2D_2X(x);
    point[0].Y           = R_DRW2D_2X(y);
    point[1].X           = R_DRW2D_2X(x1);
    point[1].Y           = R_DRW2D_2X(y1);
    ret |= R_DRW2D_DrawLines(loc_Drw2dDev, point, 2);

    R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);

    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}
void loc_RenderBezierCurves(Coord_t *coord, uint8_t point_num, uint32_t color)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_LineStyle_t LineStyle;
	r_drw2d_Point_t *point = (r_drw2d_Point_t*)coord;
	
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
    ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, color);
	ret |= R_DRW2D_CtxImgQuality(loc_Drw2dDev, R_DRW2D_IMGQUALITY_HIGH); //R_DRW2D_IMGQUALITY_LOW //R_DRW2D_IMGQUALITY_MEDIUM  //R_DRW2D_IMGQUALITY_HIGH
	
    LineStyle.LineCap    = R_DRW2D_LINECAP_ROUND; //R_DRW2D_LINECAP_BUTT; //R_DRW2D_LINECAP_SQUARE; //R_DRW2D_LINECAP_ROUND;
    LineStyle.LineJoin   = R_DRW2D_LINEJOIN_NONE;
    LineStyle.MiterLimit = R_DRW2D_2X(1);;
    LineStyle.Width      = R_DRW2D_2X(5);
	LineStyle.IsClosed   = 0;
	ret |= R_DRW2D_CtxLineStyle(loc_Drw2dDev, &LineStyle);
	for(uint8_t i = 0; i < point_num; i++)
	{
		point[i].X = R_DRW2D_2X(coord[i].x);
		point[i].Y = R_DRW2D_2X(coord[i].y);
	}
	ret |= R_DRW2D_DrawBezierCurves(loc_Drw2dDev, point, point_num);
    R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);

    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}


void loc_RenderTexture(void* addr, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;
    r_drw2d_Texture_t tex_argb32;

    tex_argb32.Handle             = R_NULL;
    tex_argb32.Flags              = R_DRW2D_TEX_VT | R_DRW2D_TEX_BILINEAR;
    tex_argb32.Buffer.Pitch       = w;
    tex_argb32.Buffer.Size.Width  = w;
    tex_argb32.Buffer.Size.Height = h;
    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
    tex_argb32.Buffer.Data        = (void*)addr;

    rect.Pos.X       = R_DRW2D_2X(x);
    rect.Pos.Y       = R_DRW2D_2X(y);
    rect.Size.Width  = R_DRW2D_2X(w);
    rect.Size.Height = R_DRW2D_2X(h);

    /* Set texture */
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
#if 1
    /* rotate and translate */
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, rect.Pos.X, rect.Pos.Y);
#endif
    /* Draw texture into rectangle */
    ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);

    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}
void loc_RenderImg_BlendMode(unsigned int x, unsigned int y, const Img_t *img, r_drw2d_BlendMode_t BlendMode)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;
    r_drw2d_Texture_t tex_argb32;
    bool valid = true;

    if (NULL == img)
    {
        return ;
    }

    tex_argb32.Handle             = R_NULL;
    tex_argb32.Flags              = R_DRW2D_TEX_VT | R_DRW2D_TEX_BILINEAR;
    tex_argb32.Buffer.Pitch       = img->Width;
    tex_argb32.Buffer.Size.Width  = img->Width;
    tex_argb32.Buffer.Size.Height = img->Height;

    switch(img->Colorformat)
    {
        case IMG_ALPHA8:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_AL8;//R_DRW2D_PIXELFORMAT_ALPHA8;
	    break;
        case IMG_RGB565:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGB565;
	    break;
        case IMG_ARGB8888:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
	    break;
        case IMG_RGBA8888:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA8888;
	    break;
        case IMG_ARGB1555:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB1555;
	    break;
        case IMG_RGBA5551:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA5551;
	    break;
        case IMG_ARGB4444:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB4444;
	    break;
        case IMG_RGBA4444:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA4444;
	    break;
        case IMG_CLUT8:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_AL8;
	    break;
        default:
	    valid = false;
	    break;
    }

    if (valid == false)
    {
        return ;
    }

    if (img->Data == NULL)
    {
        return ;
    }

    tex_argb32.Buffer.Data        = (void*)*img->Data;
    rect.Pos.X       = R_DRW2D_2X(x);
    rect.Pos.Y       = R_DRW2D_2X(y);
    rect.Size.Width  = R_DRW2D_2X(img->Width);
    rect.Size.Height = R_DRW2D_2X(img->Height);

    /* Set texture */
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, BlendMode);
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, rect.Pos.X, rect.Pos.Y);
    /* Draw texture into rectangle */
    ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
    
    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}
void loc_RenderImg(unsigned int x, unsigned int y, const Img_t *img)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;
    r_drw2d_Texture_t tex_argb32;
    bool valid = true;

    if (NULL == img)
    {
        return ;
    }

    tex_argb32.Handle             = R_NULL;
    tex_argb32.Flags              = R_DRW2D_TEX_VT | R_DRW2D_TEX_BILINEAR;
    tex_argb32.Buffer.Pitch       = img->Width;
    tex_argb32.Buffer.Size.Width  = img->Width;
    tex_argb32.Buffer.Size.Height = img->Height;

    switch(img->Colorformat)
    {
        case IMG_ALPHA8:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_AL8;//R_DRW2D_PIXELFORMAT_ALPHA8;
	    break;
        case IMG_RGB565:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGB565;
	    break;
        case IMG_ARGB8888:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
	    break;
        case IMG_RGBA8888:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA8888;
	    break;
        case IMG_ARGB1555:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB1555;
	    break;
        case IMG_RGBA5551:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA5551;
	    break;
        case IMG_ARGB4444:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB4444;
	    break;
        case IMG_RGBA4444:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA4444;
	    break;
        case IMG_CLUT8:
	    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_AL8;
	    break;
        default:
	    valid = false;
	    break;
    }

    if (valid == false)
    {
        return ;
    }

    if (img->Data == NULL)
    {
        return ;
    }

    tex_argb32.Buffer.Data        = (void*)*img->Data;
    rect.Pos.X       = R_DRW2D_2X(x);
    rect.Pos.Y       = R_DRW2D_2X(y);
    rect.Size.Width  = R_DRW2D_2X(img->Width);
    rect.Size.Height = R_DRW2D_2X(img->Height);

    /* Set texture */
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, rect.Pos.X, rect.Pos.Y);
    /* Draw texture into rectangle */
    ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
    
    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}
void loc_RenderImgScale(uint16_t x, uint16_t y, float ScaleX, float ScaleY, const Img_t *img)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_Rect_t  rect;
	r_drw2d_Texture_t tex_argb32;
	bool valid = true;
	
	if (NULL == img)
	{
		return ;
	}
	
	tex_argb32.Handle             = R_NULL;
	tex_argb32.Flags              = R_DRW2D_TEX_VT | R_DRW2D_TEX_BILINEAR;
	tex_argb32.Buffer.Pitch       = img->Width;
	tex_argb32.Buffer.Size.Width  = img->Width;
	tex_argb32.Buffer.Size.Height = img->Height;
	switch(img->Colorformat)
	{
        case IMG_ALPHA8:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ALPHA8;
        break;
        case IMG_RGB565:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGB565;
        break;
        case IMG_ARGB8888:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
        break;
        case IMG_RGBA8888:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA8888;
        break;
        case IMG_ARGB1555:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB1555;
        break;
        case IMG_RGBA5551:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA5551;
        break;
        case IMG_ARGB4444:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB4444;
        break;
        case IMG_RGBA4444:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGBA4444;
        break;
        case IMG_CLUT8:
        tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_AL8;
        break;
        default:
        valid = false;
        break;
    }
	tex_argb32.Buffer.Data = (void*)*img->Data;
	
    if (valid == false)
    {
        return ;
    }
	
    if (img->Data == NULL)
    {
        return ;
    }
	
    rect.Pos.X       = R_DRW2D_2X(x);
    rect.Pos.Y       = R_DRW2D_2X(y);
    rect.Size.Width  = R_DRW2D_2X(img->Width * ScaleX);
    rect.Size.Height = R_DRW2D_2X(img->Height * ScaleY);
	
    /* Set texture */
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);
	ret |= R_DRW2D_CtxTextureScale(loc_Drw2dDev, R_DRW2D_2X(ScaleX), R_DRW2D_2X(ScaleY));
    ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, rect.Pos.X, rect.Pos.Y);
	
    /* Draw texture into rectangle */
    ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
	ret |= R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
	
    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}


void loc_spriteCreate(uint8_t wm, uint8_t spn, unsigned int x, unsigned int y, unsigned int z, const Img_t *img)
{
    volatile r_wm_Error_t err;
    if (wm > LOC_SPRITE_WIN3)
    {
        return ;
    }
    s_atSprite[wm][spn].Data = *img->Data;
    s_atSprite[wm][spn].PosX = x;
    s_atSprite[wm][spn].PosY = y;
    s_atSprite[wm][spn].PosZ = z;
    s_atSprite[wm][spn].Width = img->Width;
    s_atSprite[wm][spn].Height = img->Height;

    switch(wm)
    {
        case LOC_SPRITE_WIN1:
            s_atSprite[wm][spn].Window = &loc_SpriteWindow;
	    break;
        case LOC_SPRITE_WIN2:
            s_atSprite[wm][spn].Window = &loc_SpriteWindow1;
	    break;
        case LOC_SPRITE_WIN3:
            s_atSprite[wm][spn].Window = &loc_SpriteWindow2;
	    break;
        default:
	    break;
    }

    err = R_WM_SpriteCreate(LOC_WM_UNIT, & s_atSprite[wm][spn]);
    
}
void loc_spriteDisable(uint8_t wm, uint8_t spn)
{
    volatile r_wm_Error_t err;
    if (wm > LOC_SPRITE_WIN3)
    {
        return ;
    }
    err = R_WM_SpriteDisable(LOC_WM_UNIT, &s_atSprite[wm][spn]);
}
void loc_spriteEnable(uint8_t wm, uint8_t spn)
{
    volatile r_wm_Error_t err;
    if (wm > LOC_SPRITE_WIN3)
    {
        return ;
    }
    err = R_WM_SpriteEnable(LOC_WM_UNIT, &s_atSprite[wm][spn]);
    //R_WM_FrameEndMark(LOC_WM_UNIT, 0);
    //R_WM_FrameWait(LOC_WM_UNIT, 0);
}
void loc_spriteDelete(uint8_t wm, uint8_t spn)
{
    if (wm > LOC_SPRITE_WIN3)
    {
        return ;
    }
    R_WM_SpriteDelete(LOC_WM_UNIT, &s_atSprite[wm][spn]);
}
void loc_spriteSetBuf(uint8_t wm, uint8_t spn, const Img_t *img)
{
    if (wm > LOC_SPRITE_WIN3)
    {
        return ;
    }
    R_WM_SpriteBufSet(LOC_WM_UNIT, &s_atSprite[wm][spn], *img->Data);
}
void loc_spriteMove(uint8_t wm, uint8_t spn, uint32_t x, uint32_t y, uint32_t z)
{
    R_WM_SpriteMove(LOC_WM_UNIT, &s_atSprite[wm][spn], x, y, z);
}


void update_box_location(uint32_t x,uint32_t y)
{
    start_draw();
    loc_spriteSetBuf(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn, \
	    &Img_Box);
    loc_spriteMove(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn, \
	    x, \
	    y, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].posZ);

    loc_spriteEnable(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn);
    end_draw();
}
void disable_box(void)
{
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn);

}

void update_choose_location(uint32_t x,uint32_t y)
{
    start_draw();
    loc_spriteSetBuf(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn, \
	    &Img_Choose);
    loc_spriteMove(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn, \
	    x, \
	    y, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].posZ);

    loc_spriteEnable(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn);
    end_draw();

}
void disable_choose_image(void)
{
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn);
}
void disable_choose(void)
{
    forceupdate = 1;

}


uint8_t forceupdate = 1;
void loc_speedUpdate(void)
{
    const Img_t *ptImg[3];
    uint8_t u8Spd;
    static uint8_t speedold = 0;
	static uint8_t SpdScene = 0xFF;
    static float OldLowCruiseSpd = 0x00;
	float LowCruiseSpd = 0x00;
	
	uint8_t speed = get_display_speed(); //get_vehicle_speed();
	
	GeneralUse_t *Pcan_0x0CFF0931 = NULL; 
	Pcan_0x0CFF0931 = (GeneralUse_t*)can_getPCanBuffer(0x0CFF0931);
	
	
	/* 超低速巡航显示：
	当激活超低速巡航时，显示的巡航目标车速报文0x0CFF0931 （同时原先的巡航状态和巡航目标车速会同时发生，低速巡航优先级高与普通巡航）。
	此时屏幕中间的实际车速在小于2km/h时，显示精度为0.1，大于2km/h时，显示精度为1。
	（请注意小于2km/h时车速滤波，防止车速跳动过快）。	
	0x0CFF0931  
	Byte3 bit1-2，00：不激活，01：激活
	Byte3 bit3-8，低速巡航目标车速，factor：0.1km/h，偏移0
	 */
	
    start_draw();
	if(0x01 == Pcan_0x0CFF0931->byte3.bit12) //激活超低速巡航
	{
		SpdScene = 0x01;
		
		LowCruiseSpd = get_display_speed(); //get_vehicle_speed();
		
		if(2U > LowCruiseSpd)
		{
			//Speed debounce
			LowCruiseSpd = (LowCruiseSpd + OldLowCruiseSpd)/2;
		}
		
		if((OldLowCruiseSpd != LowCruiseSpd)||(0x01 != SpdScene)||(0x00 != forceupdate))
		{
			forceupdate = 0;
			if(LowCruiseSpd <= 2.0)
			{
				uint8_t temp = (uint8_t)(LowCruiseSpd * 10);
				ptImg[2] = frmMainSpdNumImgs[temp % 10];
				ptImg[1] = &Img_point;
				temp /= 10;
				ptImg[0] = frmMainSpdNumImgs[temp % 10];
				loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn, \
								ptImg[0]);
				loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
								ptImg[1]);
				loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
								ptImg[2]);
				loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn, \
								330,\
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].y, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].posZ);
				loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].x, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].y, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].posZ);
				loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
								422,\
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ);
				
				loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
				loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
				loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
								mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
			}
			else
			{
				uint8_t temp = (uint8_t)(LowCruiseSpd);
				ptImg[2] = frmMainSpdNumImgs[temp % 10];
				temp /= 10;
				ptImg[1] = frmMainSpdNumImgs[temp % 10];
				temp /= 10;
				ptImg[0] = frmMainSpdNumImgs[temp % 10];
				
				temp = (uint8_t)(LowCruiseSpd);
				if (temp < 10)
				{
					loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
									ptImg[2]);
					
					loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
									368, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ);
					
					loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
					loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
					loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
				}
				else if (temp < 100)
				{
					loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
									ptImg[1]);
					loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
									ptImg[2]);
					
					loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
									336, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].y, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].posZ);
					loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
									400, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ);
					
					loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
					loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
					loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
				}
				else
				{
					loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn, \
									ptImg[0]);
					loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
									ptImg[1]);
					loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
									ptImg[2]);
					
					loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].x, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].y, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].posZ);
					loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].x, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].y, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].posZ);
					loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].x, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ);
				
					loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
					loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
					loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
									mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
				}
			}			
		}
		else
		{
		
		}
		OldLowCruiseSpd = LowCruiseSpd;	
		
		end_draw();
		
    }
    else if((speedold == speed)&&(!forceupdate)&&(0x02 == SpdScene))
	{
		SpdScene = 0x02;
		
		/* if(speed < 10)
		{
			LCD_DrawRect(368 + 65, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y + 58, 64 + 46, 24,TOUMING);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh24, VM_COLOR_WHITE, 0);	
			zk_oem2uni((uint8_t*)"km/h", uniStr, sizeof(uniStr));
			zk_printZH(368 + 65, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y + 58, uniStr);
		}
		else if(speed < 100)
		{
			LCD_DrawRect(368 + 65, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y + 58, 64 + 46, 24,TOUMING);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh24, VM_COLOR_WHITE, 0);	
			zk_oem2uni((uint8_t*)"km/h", uniStr, sizeof(uniStr));
			zk_printZH(400 + 65, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y + 58, uniStr);
		}
		else
		{
			LCD_DrawRect(368 + 65, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y + 58, 64 + 46, 24,TOUMING);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh24, VM_COLOR_WHITE, 0);	
			zk_oem2uni((uint8_t*)"km/h", uniStr, sizeof(uniStr));
			zk_printZH(432 + 65, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y + 58, uniStr);
		} */
		
		end_draw();
		return;
    }
    else
	{
		u8Spd = speed;
		SpdScene = 0x02;
		forceupdate =0;
		ptImg[2] = frmMainSpdNumImgs[u8Spd % 10];
		u8Spd /= 10;
		ptImg[1] = frmMainSpdNumImgs[u8Spd % 10];
		u8Spd /= 10;
		ptImg[0] = frmMainSpdNumImgs[u8Spd % 10];

		if (speed < 10)
		{
			loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
							ptImg[2]);
			
			loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
							368, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ);
			
			loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
			
			loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
			
			loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
			
		}
		else if (speed < 100)
		{
			loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
							ptImg[1]);
			loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
						mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
						ptImg[2]);
			
			loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
							336, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].y, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].posZ);
			loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
							400, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ);
			
			loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
			loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
			loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
			
		}
		else
		{
			loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn, \
							ptImg[0]);
			loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
							ptImg[1]);
			loc_spriteSetBuf(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
							ptImg[2]);
			
			loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].x, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].y, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].posZ);
			loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].x, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].y, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].posZ);
			loc_spriteMove(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].x, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ);

			loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
			loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
			loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
							mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
			
		}
    }
    speedold = speed;
    
	end_draw();

}


#if !defined USE_INTERNAL_ROM
void loc_RenderLogo(unsigned int x, unsigned int y)
{
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;
    r_drw2d_Texture_t tex_argb32;
    // bool valid = true;
	
    tex_argb32.Handle             = R_NULL;
    tex_argb32.Flags              = R_DRW2D_TEX_VT | R_DRW2D_TEX_BILINEAR;
    tex_argb32.Buffer.Pitch       = 276;
    tex_argb32.Buffer.Size.Width  = 276;
    tex_argb32.Buffer.Size.Height = 276;
    tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_RGB565;
    tex_argb32.Buffer.Data        = (void*)SPI_Image;
    rect.Pos.X       = R_DRW2D_2X(x);
    rect.Pos.Y       = R_DRW2D_2X(y);
    rect.Size.Width  = R_DRW2D_2X(276);
    rect.Size.Height = R_DRW2D_2X(276);

    /* Set texture */
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
    ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
    ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, rect.Pos.X, rect.Pos.Y);
    /* Draw texture into rectangle */
    ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
    R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);
    

    if (R_DRW2D_ERR_OK != ret)
    {
        loc_Error(ret);
    }
}


void disable_line_image(void)
{
    loc_ClearRect(0, 60,800, 12);
    loc_ClearRect(0, 420,800, 12);
    RenderTruckArea(0);
}
void display_line_image(void)
{
	memset(SPI_Image,0,100*1024);
	start_draw();
    RenderTruckArea(0);
    R_SFMA_DataRead(SFMAUNIT,LineBASEADDR,SPI_Image,LineLENGTH);//1024*200
    loc_RenderImg(0, 60, &Img_Line);
    loc_RenderImg(0, 420, &Img_Line);
	end_draw();
	
}

#define TPMSBASEADDR (26+88*3+20+20)*1024
#define TPMSLENGTH 35200
#define LineBASEADDR (26+88*3+20)*1024
#define LineLENGTH 19200
void disable_tpms_image(void)
{
	start_draw();
    loc_ClearRect(350, 200,100, 176);
    RenderTruckArea(0);
	end_draw();
}
void display_tpms_image(void)
{
	start_draw();
	memset(SPI_Image,0,100*1024);
    RenderTruckArea(0);
    R_SFMA_DataRead(SFMAUNIT, TPMSBASEADDR, SPI_Image, TPMSLENGTH);//1024*200
    loc_RenderImg(350, 200, &Img_TPMS);
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn);
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn);
	end_draw();
}

uint8_t displaytrucktpms_old = 88;
#define TPMSIMAGEADADDR (26+88*3+20+20+43+11*10)*1024
#define TPMSIMAGELENGTH 139672
void RenderTruckArea(uint8_t display)
{
    static uint8_t display_old = 0;
    if(display_old!=display)
	{
		display_old = display;
		loc_ClearRect(180, 235, 280+165, 158);
		
		memset(SPI_Image,0,100*1024);
	
		if(display == 1)
			R_SFMA_DataRead(SFMAUNIT,TRUCKBASEADDR,SPI_Image,TRUCKLENGTH);//1024*200
		else	if(display == 2)
			R_SFMA_DataRead(SFMAUNIT,LEFTOFFROADADDR,SPI_Image,LEFTOFFROADADDRLENGTH);//1024*200
		else	if(display == 3)
			R_SFMA_DataRead(SFMAUNIT,RIGHTOFFROADADDR,SPI_Image,LEFTOFFROADADDRLENGTH);//1024*200
		else	if(display == 4)
			R_SFMA_DataRead(SFMAUNIT,FRONTOFFROADADDR,SPI_Image,LEFTOFFROADADDRLENGTH);//1024*200
		else if(display ==5)
			 R_SFMA_DataRead(SFMAUNIT,TPMSIMAGEADADDR,SPI_Image,TPMSIMAGELENGTH);//1024*200
			else;
		ClrWdt();
    }
    start_draw();
    if(display == 1)//display truck
	{
		loc_RenderImg(340, 274, &Img_truck);
		displaytrucktpms_old = display ;
		update_tpmsvalue = 1;

    }
    else if(display == 2)//display left off road
	{
		displaytrucktpms_old = display ;
		loc_RenderImg(180, 235, &Img_leftOffset);
		update_tpmsvalue = 1;
    }
    else if(display == 3)//display right off road
	{
		displaytrucktpms_old = display ;
		loc_RenderImg(345, 235, &Img_rightOffset);
		update_tpmsvalue = 1;
    }
    else if(display == 4)//display front collision
	{
		displaytrucktpms_old = display ;
		loc_RenderImg(263, 235, &Img_frontCollision);
		update_tpmsvalue = 1;
    }
	else if(display == 5)//display tpms
	{
		if(displaytrucktpms_old != display )
		{
			displaytrucktpms_old = display ;
			if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
			{
				loc_Render_TPMSMAIN(180+180, 235,WHITE,"1");
			}
			else
				loc_Render_TPMSMAIN(180+180, 235,WHITE,"2");
		}
		if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
			tpms_mainframe(1);
    }
    else
	{
		;
    }
    end_draw();

}
#endif


void display_Logo(void)
{
    uint32_t initValue = 0;

    start_draw();
#if 1
    for(uint8_t i = 0;i < 30;i++)
	{
		loc_Render_Logo(250,280, 0x00FFFFFF+initValue,"0");
		loc_Render_Logo(250,280, 0x00FFFFFF+initValue,"0");
		R_TICK_WaitMS(0, 30);
		initValue+=0x8000000;
		end_draw();
		
		if(i == 0) //自检
		{
			memset(&alarm_led,0xff,sizeof(alarm_led));
			LED_MirrorHeat = 0x01;
			LED_Out();
			
			if(HWVersion == VERSION_A)	//第一版，没有增加反相器电路
				HC595EN = 0;
			else						//后面版本增加反相器电路
				HC595EN = 1;
			
			GeneralUse_t *BCMI_18FF2732 = NULL;
			BCMI_18FF2732 =  (GeneralUse_t*)can_getBCanBuffer(0x18FF2732);
			if(BCMI_18FF2732->byte3.bit12 == 1)
			{
				PWM_Update_Duty_Cycle(LED_CHANNEL, e_backlight_led_night,    PWM_PERIOD);
				PWM_Update_Duty_Cycle(BG_CHANNEL,  e_backlight_pannel_night, PWM_PERIOD);
				PWM_Update_Duty_Cycle(LCD_CHANNEL, e_backlight_lcd_night,    PWM_PERIOD);
			}
			else
			{
				PWM_Update_Duty_Cycle(LED_CHANNEL, e_backlight_led,    PWM_PERIOD);
				PWM_Update_Duty_Cycle(BG_CHANNEL,  e_backlight_pannel, PWM_PERIOD);
				PWM_Update_Duty_Cycle(LCD_CHANNEL, e_backlight_lcd,    PWM_PERIOD);
			}
			
		}
		if(i == 6)
		{
			StepperMotorback();
		}
		
    }

    R_TICK_WaitMS(0, 200);
    StepperMotormiddle();
    for(uint8_t i = 32;i>2; i--)
	{
		loc_Render_Logo(250,280, 0x00FFFFFF+initValue,"0");
		loc_Render_Logo(250,280, 0x00FFFFFF+initValue,"0");
		R_TICK_WaitMS(0, 30);
		initValue-=0x8000000;
		end_draw();
		ClrWdt();
		// R_TICK_WaitMS(0, 100);
    }
	
#else
    memset(&alarm_led,0xff,sizeof(alarm_led));
    LED_Out();
    PWM_Update_Duty_Cycle(LED_CHANNEL, 90, PWM_PERIOD);
    PWM_Update_Duty_Cycle(BG_CHANNEL, 90, PWM_PERIOD);//980hz
	PWM_Update_Duty_Cycle(LCD_CHANNEL, 90, PWM_PERIOD);
    StepperMotorback();
    for(uint8_t i = 0;i < 5;i++)
	{
		start_draw();
		R_SFMA_DataRead(0,204800*i,SPI_Image,152352);//1024*200
		loc_RenderImg(200,120,&Img_start);
		end_draw();
		ClrWdt();
		
		if( i ==16)
			StepperMotormiddle();
    }
    loc_ClearRect(200, 120, 300, 300);
#endif
    
}


void show_ldws_left_lane(void)
{
    loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].wm, \
	                 mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].spn);
}
void hide_ldws_left_lane(void)
{
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].wm, \
	                  mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].spn);

}

void show_ldws_right_lane(void)
{
    loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].wm, \
	                 mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].spn);
}
void hide_ldws_right_lane(void)
{
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].wm, \
	                  mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].spn);

}



void show_BG(void)
{
    start_draw();
	
    loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].spn);
	// loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].spn);
	if(eol_fadongjiorkaji==0x2d)
	{
		loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].spn);
		loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].spn);
	}
	else
	{
		loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].spn);
		loc_spriteEnable(mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].spn);
	}
	
	loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn);
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn);
    
	loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].spn);
	loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].spn);
	
	loc_spriteDisable(mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].wm, mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].spn);
	
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].spn);
	
	end_draw();
	
}
void disable_BG(void)
{
    bg_shown = 0;
    start_draw();
	//LCD_Clear();
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].spn);
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
    
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn);
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn);
	loc_spriteEnable(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].spn);
    loc_spriteEnable(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].spn);
	
	loc_spriteDisable(mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].wm, mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].spn);
	
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].spn);
	
	end_draw();
	
}
void display_Charge_BG(void)
{
	start_draw();
	
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].spn);
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn);
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn);
    loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn);
    
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn);
    loc_spriteDisable(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn);
	loc_spriteEnable(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].spn);
    loc_spriteEnable(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].wm, mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].spn);
	
	loc_spriteEnable(mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].wm, mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].spn);
	
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].spn);
	loc_spriteDisable(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].wm, mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].spn);
	
    end_draw();
}


void loc_RenderImgA8(uint16_t x,uint16_t y, char *src, Font_t Font, uint32_t fcolor)
{
	SelectFont(&Font);
	Write(loc_Drw2dDev, x, y, fcolor, src);
	
}
void loc_Render_Logo(uint16_t x,uint16_t y, uint32_t color, char value[])
{
    SelectFont(&FontLogo);
    Write(loc_Drw2dDev, x, y, color, value);
    SelectFont(&FontRound );
    Write(loc_Drw2dDev, x+74, y -124, color&0xffff0000, value);
}
void loc_Render_FHP(uint16_t x,uint16_t y, uint8_t color, char value[])
{
    unsigned int color_f = color_convert(color);
    SelectFont(&Font_TellTable);
    Write(loc_Drw2dDev, x, y, color_f, value);
}
void loc_Render_FHP_Text(uint16_t x,uint16_t y, uint8_t color, char value[])
{
    unsigned int color_f = color_convert(color);
    SelectFont(&Font_Text);
    Write(loc_Drw2dDev, x, y, color_f, value);
}
void loc_Render_FHP_LowBatt(uint16_t x,uint16_t y, uint8_t color, char value[])
{
    unsigned int color_f = color_convert(color);
    SelectFont(&Font_LowBatt);
    Write(loc_Drw2dDev, x, y, color_f, value);
}
void loc_Render_FHP_IntelligentDriving(uint16_t x,uint16_t y, uint8_t color, char value[])
{
    unsigned int color_f = color_convert(color);
    SelectFont(&Font_IntelligentDriving);
    Write(loc_Drw2dDev, x, y, color_f, value);
}
void loc_Render_FHP_AuxiliaryBraking(uint16_t x,uint16_t y, uint8_t color, char value[])
{
    unsigned int color_f = color_convert(color);
    SelectFont(&Font_AuxiliaryBraking);
    Write(loc_Drw2dDev, x, y, color_f, value);
}


void loc_Render_BigIcon(uint16_t x,uint16_t y, uint32_t color, char value[])
{
    SelectFont(&Font_BigTellTable);
    Write(loc_Drw2dDev, x, y, color, value);
}
void loc_Render_BigFHP(uint16_t x,uint16_t y, uint8_t color, char value[])
{
    unsigned int color_f   = color_convert(color);
    SelectFont(&Font_BigTellTable);
    Write(loc_Drw2dDev, x, y, color_f, value);
}
void loc_Render_TPMSMAIN(uint16_t x,uint16_t y, uint8_t color, char value[])
{
    unsigned int color_f   = color_convert(color);
    SelectFont(&Font_TRUCK);
    Write(loc_Drw2dDev, x, y, color_f, value);
}
void loc_Render_Tire(uint16_t x,uint16_t y, uint8_t color)
{
    unsigned int color_f   = color_convert(color);
    SelectFont(&Font_Tire);
    Write(loc_Drw2dDev, x, y, color_f, "1");
}


static void loc_init(void)
{
    // uint32_t frame;
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

    //R_BSP_STDIO_Printf("Now using VOWE, JCUA and WM\n");
	
    loc_CreateWindow(R_WM_WINMODE_FB,
	                0,
	                0,
	                2,
	                LOC_DISPLAY_WIDTH,
	                LOC_DISPLAY_HEIGHT,
	                LOC_DISPLAY_STRIDE,
	                R_WM_COLORFMT_ARGB8888,
	                &loc_WmWindow);
	
    loc_CreateWindow(R_WM_WINMODE_SPRITES,
	                0,
	                0,//200,
	                0,
	                800,
	                480,
	                2048,
	                R_WM_COLORFMT_RGB565,
	                &loc_SpriteWindow);
	
    loc_CreateWindow(R_WM_WINMODE_SPRITES,
	                0,
	                0,
	                1,
	                800,
	                480,
	                2048,
	                R_WM_COLORFMT_ARGB4444,
	                &loc_SpriteWindow1);

    R_WM_WindowColorKeyEnable(LOC_WM_UNIT, &loc_WmWindow);
    R_WM_WindowColorKeyEnable(LOC_WM_UNIT, &loc_SpriteWindow);
    R_WM_WindowColorKeyEnable(LOC_WM_UNIT, &loc_SpriteWindow1);
    /* start up gfx engine */
    ret |= R_DRW2D_Init();
    ret |= R_DRW2D_GlobalErrCallbackSet(&loc_DRW2DErrorHandler, R_NULL);
    ret |= R_DRW2D_Open(LOC_DRW2D_UNIT, R_DRW2D_UNIT_DHD0, &loc_DHDDev, &loc_Drw2dDev);
	
    start_draw();
    loc_RenderClear(1);
    end_draw();
	
    start_draw();
    loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BG].posZ, \
	    &Img_bg);

    loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_H].posZ, \
	    &Img_spdn0);
    loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_M].posZ, \
	    &Img_spdn0);
    loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_SPD_L].posZ, \
	    &Img_spdn0);
	
    loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_BattSoc].posZ, \
	    &Img_BattSoc);

    loc_spriteCreate(mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].spn, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].x, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].y, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BOX].posZ, \
	    &Img_Box);

    loc_spriteCreate(mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].spn, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].x, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].y, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_Choose].posZ, \
	    &Img_Choose);
#if defined USE_INTERNAL_ROM
    loc_spriteCreate(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].spn, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].x, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].y, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L1].posZ, \
	    &Img_Line);
    loc_spriteCreate(mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].wm, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].spn, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].x, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].y, \
	    mainViewSprites[MENU_VIEW_SPRITE_ID_BG_L2].posZ, \
	    &Img_Line);
	loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_H2Soc_MASK].posZ, \
	    &Img_H2_SOC);
	loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_Motor_MASK].posZ, \
	    &Img_motor);

#endif
	
	loc_spriteCreate(mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].wm, \
	    mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].spn, \
	    mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].x, \
	    mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].y, \
	    mainViewSprites[CHARGE_VIEW_SPRITE_ID_SOC_BG].posZ, \
	    &Img_ChgSOC_BG);
	
	loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_LEFT].posZ, \
	    &Img_leftOffset);
	loc_spriteCreate(mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].wm, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].spn, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].x, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].y, \
	    mainViewSprites[MAIN_VIEW_SPRITE_ID_LDWS_RIGHT].posZ, \
	    &Img_rightOffset);	
	
    end_draw();

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

/*===========================================================================
* File Revision History (bottom to top: first revision to last revision)
*============================================================================
* $Log:$
* Rev:     Userid:       Date:       (Description on following lines: CR #, etc.)
* -------  ------------  -----------
*
*  
*===========================================================================*/

