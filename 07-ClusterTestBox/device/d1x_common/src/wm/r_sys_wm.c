/*
****************************************************************************
PROJECT : WM
FILE    : $Id: r_sys_wm.c 7909 2016-03-09 10:08:44Z matthias.nippert $
============================================================================ 
DESCRIPTION
WM system functions for D1x
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
****************************************************************************
*/

/***********************************************************
  Title: D1Mx and D1Lx WM SYS API implementation  
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_cdi_api.h"

#include "r_ddb_api.h"      /* display database with display timings */
#include "r_config_wm.h"
#include "r_wm_api.h"
#include "r_wm_sys.h"
#include "r_sys_wm.h"

#ifdef USE_ROS
#include "r_os_api.h"
#endif

#include "r_vdce_api.h"
#include "r_spea_api.h"
#include "r_dev_pin_types.h"
#include "r_dev_api.h"

#if defined R_WM_MSQ_DEQUEUE_PROFILING || defined R_WM_MSQ_ENQUEUE_PROFILING
#include "r_profile.h"
#endif


/***********************************************************
  Section: Local Defines
*/

/* #define R_WM_SYS_ERROR_TRACKING */

#ifdef R_WM_SYS_ERROR_TRACKING
    #include "r_bsp_stdio_api.h"
#endif

/***********************************************************
  Macro: LOC_INVALID_LAYER
  
  Defines an invalid layer constant.
*/
#define LOC_INVALID_LAYER 0xffffffffu

/***********************************************************
  Macro: LOC_INVALID_SPRITE 
  
  Defines an invalid sprite constant.
*/
#define LOC_INVALID_SPRITE 16

/***********************************************************
  Macro: LOC_USE_OS

  Flag signaling if the OS is used
*/

#ifdef USE_ROS
  #define LOC_USE_OS
#endif

   
/***********************************************************
  Macro: VDCECTL
  
  ITU 656 register.
*/
#define VDCECTL         0xFFC0601Cu

/***********************************************************
  Macro: VDCECTL_VISL1
  
  ITU 656 register.
*/

#define VDCECTL_VISL0   (1ul<<0)
#define VDCECTL_VISL1   (1ul<<1)
#define VDCECTL_PXSL    (1ul<<3)
#define VDCECTL_UPDT0   (1ul<<4)
#define VDCECTL_UPDT1   (1ul<<5)


/***********************************************************
  Section: Global Variables
*/

/***********************************************************
  Variable : r_wm_DevSys
  
  Sys layer device object.
*/

r_wm_DevSys_t r_wm_DevSys[R_WM_DEV_NUM];

/***********************************************************
  Section: Local Function Declarations
*/

static float32_t loc_floorf( float32_t x );
static int32_t   loc_iroundf( float32_t f );
static float32_t loc_minf( float32_t a, float32_t b );
static float32_t loc_maxf( float32_t a, float32_t b );
static float32_t loc_log2( float32_t i );
static float32_t loc_pow2( float32_t i );
static float32_t loc_powf( float32_t a, float32_t b );


static void loc_VblankIsr(uint32_t Unit);
static void loc_ScanlineIsr(uint32_t Unit);
static void loc_VI_VblankIsr(uint32_t Unit);
static void locIsr_VO_0_Vblank(void);
static void locIsr_VO_0_Scanline(void);
static void locIsr_VI_0_Vblank(void);
static void locIsr_VO_1_Vblank(void);
static void locIsr_VO_1_Scanline(void);
static void locIsr_VI_1_Vblank(void);
static r_vdce_Format_t loc_GetVdceFormat(r_wm_WinColorFmt_t Format);
static void loc_Layer2Spea (    uint32_t        Unit,
                                uint32_t        Layer,
                                r_spea_Unit_t  *SpeaUnit,
                                uint8_t        *SpeaUnitOrdinal
                           );
static void loc_SpeaOrdinal2Layer ( uint32_t    Unit,
                                    uint8_t     SpeaUnitOrdinal,
                                    uint32_t   *Layer
                                  );
static uint8_t loc_FindSpriteOrdinal   (   uint32_t            Unit,
                                    const r_wm_Sprite_t *Sprite
                                );
static uint32_t loc_ReassignAllSprites(uint32_t Unit, r_wm_Sprite_t* RootSprite, uint32_t Layer);
static uint32_t loc_WinGetCount(uint32_t Unit);
static r_wm_sys_vdce_WinLayerPair_t* loc_WinLayerPairGet(uint32_t Unit, const r_wm_Window_t* Win);
static r_wm_sys_vdce_WinLayerPair_t* loc_WinLayerPairGetByLayer(uint32_t Unit, const uint32_t Layer);
static uint32_t loc_SetRle(uint32_t Unit, r_wm_WinColorFmt_t Fmt, uint32_t Buf);
static uint32_t loc_CheckHWLayerCapabilities(uint32_t Layer, const r_wm_Window_t* Win);
static uint32_t loc_WinReconfAll(uint32_t Unit);
static uint32_t loc_WinSetViewportAndGeom  (   uint32_t            Unit,
                                        const r_wm_Window_t *Win,
                                        uint32_t            Layer,
                                        int32_t             PosX,
                                        int32_t             PosY,
                                        int32_t             Pitch,
                                        int32_t             Width,
                                        int32_t             Height,
                                        int32_t             Bpp
                                    );

static uint32_t loc_WinSetFb   (   uint32_t            Unit,
                            const r_wm_Window_t *Win,
                            int32_t             PosX,
                            int32_t             PosY,
                            const void          *Fb
                        );
static const void* loc_WinGetOnScreenFb(const r_wm_Window_t *Win);
static const r_vdce_Error_t loc_VDCE_LayerEnable(uint32_t Unit, uint32_t Layer);
static const r_vdce_Error_t loc_VDCE_LayerDisable(uint32_t Unit, uint32_t Layer);
static void loc_VDCE_ErrorCallback(const uint32_t Unit, const r_vdce_Error_t Error);
static void loc_SPEA_ErrorCallback(const uint32_t Unit, uint32_t Error);
static uint32_t loc_WindowIsYUVLayer(const r_wm_Window_t *Window);

static uint32_t loc_fitCurveForVdce( uint16_t *InValue, float32_t  *OutValue, uint32_t NumEntries,
                                     r_vdce_GammaCorrect_t *FittedCurve );
static uint32_t loc_getGammaCurve( uint32_t Unit, uint8_t Gamma, r_vdce_GammaCorrect_t *Curve );



/***********************************************************
  Section: Local Variables
*/

#ifdef LOC_USE_OS
    static R_OS_ThreadMutex_t     locMutexMsgQueue[R_WM_DEV_NUM];
    static R_OS_ThreadMutex_t     locMutexBufferMgmnt[R_WM_DEV_NUM];
    static R_OS_ThreadMutex_t     locMutexWindows[R_WM_DEV_NUM];
    static R_OS_ThreadMutex_t     locMutexDevice[R_WM_DEV_NUM];
    static uint32_t               locMutexInitCreated[R_WM_DEV_NUM] = {0};
    static R_OS_Sem_t             locSemaMsgTrigger[R_WM_DEV_NUM];
    
    static R_OS_Sem_t             locSemaISRTrigger[R_WM_DEV_NUM][R_WM_EVENT_LAST];
#endif

static volatile uint32_t loc_WaitVblankInt[R_WM_DEV_NUM];
static volatile uint32_t loc_WaitScanlineInt[R_WM_DEV_NUM];

#ifdef R_WM_SYS_ERROR_TRACKING
    static volatile r_wm_Error_t loc_LastError_Code = R_WM_ERR_OK;
    static volatile uint32_t     loc_LastError_Info = 0;
    static volatile uint32_t     loc_LastError_Unit = 0;
    static volatile uint32_t     loc_LastError_Line = 0;
    static volatile uint32_t     loc_LastError_Overflow = 0;
    static volatile uint32_t     loc_LastError_NewError = 0;
#endif /* R_WM_SYS_ERROR_TRACKING */

static void (*loc_Isr[R_WM_DEV_NUM][R_WM_EVENT_LAST])(void) =
{
    /* The member with value zero of r_wm_EventId_t is not used */
    {0, &locIsr_VO_0_Vblank, &locIsr_VO_0_Scanline, &locIsr_VI_0_Vblank}
#if (R_WM_DEV_NUM > 1)
    ,
    {0, &locIsr_VO_1_Vblank, &locIsr_VO_1_Scanline, &locIsr_VI_1_Vblank}
#endif
};

static r_cdi_Heap_t *loc_CpuHeap_WM = 0;
static r_cdi_Heap_t *loc_VidHeap_WM = 0;

/***********************************************************
  Section: Local Functions
*/

/*
The following function are a replacement for math.h to ensure, the drivers are stdlib-free.
You may replace these functions by a library implementation if available.

MISRA disabled due to binary fiddling with floating point variables in <loc_log2> and <loc_pow2>.
*/
#pragma ghs startnomisra

/***********************************************************
  Function: loc_floorf
  
  You may replace this by a library implementation if available.
*/
static float32_t loc_floorf( float32_t x )
{
    int64_t y = (int64_t)x;
    if (x < 0) {
        y--;
    }
    return y;
}

/***********************************************************
  Function: loc_iroundf
  
  You may replace this by a library implementation if available.
*/
static int32_t loc_iroundf( float32_t f )
{
    f += (f>0) ? 0.5f : -0.5f;
    return (int32_t)f;
}


/*******************************************************************************
  Function: minf

  Return the minimum of the two supplied integer values

  You may replace this by a library implementation if available.

  Parameters:
  a,b  - Values to be compared
  
  Returns:
  int32_t
*/
static float32_t loc_minf( float32_t a, float32_t b )
{
    return ( a < b ) ? a : b;
}

/***********************************************************
  Function: loc_maxf
  
  You may replace this by a library implementation if available.
*/
static float32_t loc_maxf( float32_t a, float32_t b )
{
    return ( a > b ) ? a : b;
}

typedef union intfloat32_tag
{
	float32_t fVal;
	uint32_t  i32Val;

} intfloat32_t;

/***********************************************************
  Function: loc_log2
  
  Needed by <loc_powf>.
*/
static float32_t loc_log2( float32_t i )
{
    const float32_t OOshift23 = 1.0f / (1uL<<23);
    const float32_t LogBodge  = 0.346607f;

    intfloat32_t _i;
    float32_t x;
    float32_t y;
    
    /* Cast types via union as we want to keep the bits as they are */
    _i.fVal = i;
    
    x  = (float32_t)_i.i32Val;
    x *=  OOshift23; /* 1/(2^23) */
    x  =  x - 127;
    
    y = x - loc_floorf(x);
    y = (y-(y*y) ) * LogBodge;
    
    return x+y;
}

/***********************************************************
  Function: loc_pow2
  
  Needed by <loc_powf>.
*/
static float32_t loc_pow2( float32_t i )
{
    const float32_t shift23  = (1uL<<23);
    const float32_t PowBodge = 0.33971f;

    intfloat32_t _x;
    float32_t x;
    float32_t y;
    
    y = i - loc_floorf(i);
    y = (y-(y*y)) * PowBodge;
    
    x  = (i + 127) - y;
    x *= shift23; /* 2^23 */

    /* Cast types via union as we want to keep the bits as they are */
    _x.i32Val = (int32_t)x;
    
	return _x.fVal;
}

/***********************************************************
  Function: loc_powf
  
  The maximum error for this implementation of powf for the range of [0.5 .. 4] is 0.7%
  This is sufficient for its intended purpose.
  You may replace this by a library implementation if available.
*/
static float32_t loc_powf( float32_t a, float32_t b )
{
    return loc_pow2( b * loc_log2(a) );
}

#pragma ghs endnomisra



/***********************************************************
  Function: loc_IsrVblank
  
  Generic handler for the VBLANK interrupt.
*/
static void loc_VblankIsr(uint32_t Unit)
{
    r_wm_Event_t event = { R_WM_EVENT_VBLANK };

#if defined R_WM_SKIP_FRAME_ON_SPEA_BUSY
    uint32_t spea_update_pending;
    
    /* get update flag and filter for unit - if flag is set, do not process this interrupt */
    spea_update_pending = R_SPEA_UnitUpdateActive(0, R_SPEA_SU0);
    spea_update_pending &= (1uL<<Unit);
    if (0 != spea_update_pending)
    {
#if defined R_WM_MSQ_DEQUEUE_PROFILING || defined R_WM_MSQ_ENQUEUE_PROFILING
        R_WM_Profile_Log_VBLANK_Skipped();
#endif
        return;
    }
#endif
#ifdef LOC_USE_OS
    while (loc_WaitVblankInt[Unit] != 0)
    {
        R_OS_SemPost(&locSemaISRTrigger[Unit][R_WM_EVENT_VBLANK]);
        loc_WaitVblankInt[Unit]--;
    }
#else
    if (loc_WaitVblankInt[Unit] == 1)
    {
        loc_WaitVblankInt[Unit] = 0;
    }
#endif

    if (0 != (r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_CAPTURE_ACTIVE) )
    {
        /* Can we do triple buffering? */
        if (r_wm_DevSys[Unit].RootCapt->Window->Surface.Fb.BufNum > 2)
        {
            r_wm_WinBuffer_t *buf;
            
            /* Get a free buffer */
            buf = R_WM_Cap_CapBufGet (Unit, r_wm_DevSys[Unit].RootCapt->Window);

            if (buf != 0)
            {
                /* And capture into it starting with the next frame */
                R_VDCE_CapBufSet(r_wm_DevSys[Unit].RootCapt->CapUnit,
                                 (uint32_t)buf->Data,
                                 (uint32_t)buf->Data);
            }
        }
    }

    if ((r_wm_DevSys[Unit].RegEvents & (uint32_t)(1uL << R_WM_EVENT_VBLANK)) != 0)
    {
        if (r_wm_DevSys[Unit].EventCb != 0)
        {
            r_wm_DevSys[Unit].EventCb(Unit, &event);
        }
    }
}

/***********************************************************
  Function: loc_IsrScanline
  
  Generic handler for the SCANLINE interrupt.
*/

static void loc_ScanlineIsr(uint32_t Unit)
{
    r_wm_Event_t event = { R_WM_EVENT_SCANLINE };
    if ((r_wm_DevSys[Unit].RegEvents & (uint32_t)(1uL << R_WM_EVENT_SCANLINE)) != 0)
    {
        if (r_wm_DevSys[Unit].EventCb != 0)
        {
            r_wm_DevSys[Unit].EventCb(Unit, &event);
        }
    }

#ifdef LOC_USE_OS
    while (loc_WaitScanlineInt[Unit] != 0)
    {
        R_OS_SemPost(&locSemaISRTrigger[Unit][R_WM_EVENT_SCANLINE]);
        loc_WaitScanlineInt[Unit]--;
    }
#else
    if (loc_WaitScanlineInt[Unit] == 1)
    {
        loc_WaitScanlineInt[Unit] = 0;
    }
#endif
}

/***********************************************************
  Function: loc_VI_VblankIsr
  
  Generic handler for the Video Input VBLANK interrupt.
*/
static void loc_VI_VblankIsr(uint32_t Unit)
{
    r_wm_Event_t event = { R_WM_EVENT_VI_VBLANK };
    
    if (0 != (r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_CAPTURE_ACTIVE) )
    {
        /* Can we do triple buffering? */
        if (r_wm_DevSys[Unit].RootCapt->Window->Surface.Fb.BufNum > 2)
        {
            r_wm_WinBuffer_t *buf;
            
            /* Get freshly captured buffer */
            buf = R_WM_Cap_DispBufGet(Unit,r_wm_DevSys[Unit].RootCapt->Window);
            
            if (buf != 0)
            {
                /* And display it with the next frame */
                if (R_FALSE == R_WM_Sys_WindowSetFb(Unit, r_wm_DevSys[Unit].RootCapt->Window, buf->Data))
                {
                    /*err = R_WM_ERR_SYS_WIN_SWAP_FAILED;*/
                }
            }
        }
    }

    
    if ((r_wm_DevSys[Unit].RegEvents & (uint32_t)(1uL << R_WM_EVENT_VI_VBLANK)) != 0)
    {
        if (r_wm_DevSys[Unit].EventCb != 0)
        {
            r_wm_DevSys[Unit].EventCb(Unit, &event);
        }
    }
}

/***********************************************************
  Function: locIsr_VO_0_Vblank
  
  VDCE VBLANK ISR for VOUT unit 0.
*/
static void locIsr_VO_0_Vblank(void)
{
    loc_VblankIsr(0);
}

/***********************************************************
  Function: locIsr_VO_0_Scanline
  
  VDCE SCANLINE ISR for VOUT unit 0.
*/

static void locIsr_VO_0_Scanline(void)
{
    loc_ScanlineIsr(0);
}

/***********************************************************
  Function: locIsr_VI_0_Vblank
  
  VDCE VBLANK ISR for VIN unit 0.
*/
static void locIsr_VI_0_Vblank(void)
{
    loc_VI_VblankIsr(0);
}


#if (R_WM_DEV_NUM > 1)

/***********************************************************
  Function: locIsr_VO_1_Vblank
  
  VDCE VBLANK ISR for VOUT unit 1.
*/

static void locIsr_VO_1_Vblank(void)
{
    loc_VblankIsr(1);
}

/***********************************************************
  Function: locIsr_VO_0_Scanline
  
  VDCE SCANLINE ISR for VOUT unit 1.
*/

static void locIsr_VO_1_Scanline(void)
{
    loc_ScanlineIsr(1);
}

/***********************************************************
  Function: locIsr_VI_1_Vblank
  
  VDCE VBLANK ISR for VIN unit 1.
*/
static void locIsr_VI_1_Vblank(void)
{
    loc_VI_VblankIsr(1);
}


#endif /* (R_WM_DEV_NUM > 1) */
 


#ifdef R_WM_SYS_ERROR_TRACKING
    
    #define R_WM_SYS_REPORT_ERROR(Unit,Error,AdditionalInfo)    \
                (loc_SetLastError((Unit),(Error),(AdditionalInfo), __LINE__))
    
    /***********************************************************
      Function: R_WM_Sys_GetLastError
    */
    r_wm_Error_t R_WM_Sys_GetLastError(uint32_t *Unit, uint32_t *AdditionalInfo, uint32_t *Overflow, uint32_t *New)
    {
        if (Unit != 0)
        {
            *Unit = loc_LastError_Unit;
        }
        if (AdditionalInfo != 0)
        {
            *AdditionalInfo = loc_LastError_Info;
        }
        if (Overflow != 0)
        {
            *Overflow = loc_LastError_Overflow;
        }
        if (New != 0)
        {
            *New = loc_LastError_NewError;
        }
        
        loc_LastError_NewError = 0;
        loc_LastError_Overflow = 0;
        
        return loc_LastError_Code;
    }
    
    /***********************************************************
      Function: loc_SetLastError
    */
    static void loc_SetLastError(uint32_t Unit, r_wm_Error_t Error, uint32_t AdditionalInfo, uint32_t Line);
    
    static void loc_SetLastError(uint32_t Unit, r_wm_Error_t Error, uint32_t AdditionalInfo, uint32_t Line)
    {
        if (AdditionalInfo != 0)
        {
            R_BSP_STDIO_Printf("ERROR in R_SYS_WM, Line %u, Unit %u, Code %u/%u\n", Line, Unit, Error, AdditionalInfo);
        }
        else
        {
            R_BSP_STDIO_Printf("ERROR in R_SYS_WM, Line %u, Unit %u, Code %u\n", Line, Unit, Error);
        }
        
        if (0 == loc_LastError_NewError)
        {
            loc_LastError_Code = Error;
            loc_LastError_Info = AdditionalInfo;
            loc_LastError_Unit = Unit;
            loc_LastError_Line = Line;
            loc_LastError_NewError = 1;
        }
        else
        {
            loc_LastError_Overflow = 1;
        }
    }

#else /* R_WM_SYS_ERROR_TRACKING */
    #define R_WM_SYS_REPORT_ERROR(x,y,z)
  
#endif /* R_WM_SYS_ERROR_TRACKING */

/***********************************************************
  Function: loc_GetVdceFormat
*/
static r_vdce_Format_t loc_GetVdceFormat(r_wm_WinColorFmt_t Format)
{
    r_vdce_Format_t  vdcosim_format;

    switch (Format) {
    case R_WM_COLORFMT_RGB565:
        vdcosim_format = R_VDCE_RGB565;
        break;
    case R_WM_COLORFMT_ARGB1555:
        vdcosim_format = R_VDCE_ARGB1555;
        break;
    case R_WM_COLORFMT_RGBA5551:
        vdcosim_format = R_VDCE_RGBA5551;
        break;
    case R_WM_COLORFMT_RGB0444:
    case R_WM_COLORFMT_ARGB4444:
        vdcosim_format = R_VDCE_ARGB4444;
        break;
    case R_WM_COLORFMT_RGB0888:
    case R_WM_COLORFMT_RLE24RGB0888:
        vdcosim_format = R_VDCE_RGB0888;
        break;
    case R_WM_COLORFMT_ARGB8888:
    case R_WM_COLORFMT_RLE24ARGB8888:
        vdcosim_format = R_VDCE_ARGB8888; 
        break;
    case R_WM_COLORFMT_RGBA8888:
       vdcosim_format = R_VDCE_RGBA8888;
       break;
    case R_WM_COLORFMT_CLUT8:
        vdcosim_format = R_VDCE_CLUT8;
        break;
    case R_WM_COLORFMT_CLUT4:
        vdcosim_format = R_VDCE_CLUT4;
        break;
    case R_WM_COLORFMT_CLUT1:
        vdcosim_format = R_VDCE_CLUT1;
        break;
    case R_WM_COLORFMT_YCBCR_422:
        vdcosim_format = R_VDCE_YCBCR_422;
        break;
    case R_WM_COLORFMT_YCBCR_444:
        vdcosim_format = R_VDCE_YCBCR_444;
        break;
    case R_WM_COLORFMT_YUV_YUYV:
        vdcosim_format = R_VDCE_YUV_YUYV;
        break;
    case R_WM_COLORFMT_YUV_UYVY:
        vdcosim_format = R_VDCE_YUV_UYVY;
        break;
    case R_WM_COLORFMT_YUV_YVYU:
        vdcosim_format = R_VDCE_YUV_YVYU;
        break;
    case R_WM_COLORFMT_YUV_VYUY:
        vdcosim_format = R_VDCE_YUV_VYUY;
        break;
    default:
        vdcosim_format = R_VDCE_FORMAT_LAST;
        break;
    }
    return vdcosim_format;
}

uint32_t R_WM_Priv_WindowIsRleLayer(const r_wm_Window_t  *Window);



/***********************************************************
  Function: loc_WindowIsYUVLayer
*/
static uint32_t loc_WindowIsYUVLayer(const r_wm_Window_t *Window)
{
    uint32_t ret_val;
    switch (Window->ColorFmt)
    {
        case R_WM_COLORFMT_YUV_YUYV:
        case R_WM_COLORFMT_YUV_UYVY:
        case R_WM_COLORFMT_YUV_YVYU:
        case R_WM_COLORFMT_YUV_VYUY:
        case R_WM_COLORFMT_YCBCR_422:
        case R_WM_COLORFMT_YCBCR_444:
            ret_val = 1;
            break;
        default:
            ret_val = 0;
            break;
    }
    return ret_val;
}


/***********************************************************
  Function: loc_GetVdceCapFormat
*/

static void loc_Layer2Spea (    uint32_t        Unit,
                                uint32_t        Layer,
                                r_spea_Unit_t  *SpeaUnit,
                                uint8_t        *SpeaUnitOrdinal
                           )
{
    if (Unit == 0)
    {
        *SpeaUnitOrdinal = Layer - 1;
    }
    else
    {
        *SpeaUnitOrdinal = 3 - Layer;
    }
    *SpeaUnit = (r_spea_Unit_t)(R_SPEA_SU0 + *SpeaUnitOrdinal);
}

static void loc_SpeaOrdinal2Layer ( uint32_t    Unit,
                                    uint8_t     SpeaUnitOrdinal,
                                    uint32_t   *Layer
                                  )
{
    if (Unit == 0)
    {
        *Layer = SpeaUnitOrdinal + 1;
    }
    else
    {
        *Layer = 3 - SpeaUnitOrdinal;
    }
}

static uint8_t loc_FindSpriteOrdinal   (   uint32_t            Unit,
                                    const r_wm_Sprite_t *Sprite
                                )
{
    const r_wm_Sprite_t* tmp_sprite = Sprite->Window->Surface.SpritesRoot;
    uint8_t sprite_no = 0;
    while   (   (tmp_sprite != R_NULL)
                &&
                (sprite_no < R_VDCE_MAX_SPRITES_PER_LAYER)
            )
    {
        if (tmp_sprite == Sprite)
        {
            break;
        }
        ++sprite_no;
        tmp_sprite = tmp_sprite->Next;
    }
    return sprite_no;
}

static uint32_t loc_ReassignAllSprites(uint32_t Unit, r_wm_Sprite_t* RootSprite, uint32_t Layer)
{
    /* reassign all the sprites  */
    r_spea_SpriteConfig_t sprite_conf;
    uint32_t sprite_no = 0;
    r_wm_Sprite_t* sprite = RootSprite;
    
    r_spea_Unit_t spea_unit;
    uint8_t spea_unit_ordinal = 0;
    
    loc_Layer2Spea(Unit, Layer, &spea_unit, &spea_unit_ordinal);
    
    while   (   (sprite != R_NULL)
                &&
                (sprite_no < R_VDCE_MAX_SPRITES_PER_LAYER)
            )
            
    {
        sprite_conf.PosX = sprite->PosX;
        sprite_conf.PosY = sprite->PosY;
        sprite_conf.Width = sprite->Width;
        sprite_conf.Height = sprite->Height;
        sprite_conf.SrcAddr = (uint32_t)sprite->Data; 
        sprite_conf.VSync = Unit;
        if (sprite->Status == R_WM_SPRITESTATUS_ENABLED)
        {
            sprite_conf.Enable = 1;
        }
        else
        {
            sprite_conf.Enable = 0;
        }
        if (R_SPEA_ERR_OK != R_SPEA_SetSprite(0, spea_unit_ordinal, sprite_no, &sprite_conf, R_WM_ColorFmtBitsPerPixGet(sprite->Window->ColorFmt)))
        {
            return 0;
        }
        ++sprite_no;
        sprite = sprite->Next;
    }
    
    for (; sprite_no != R_VDCE_MAX_SPRITES_PER_LAYER; ++sprite_no)
    {
        if (R_SPEA_ERR_OK != R_SPEA_SpriteEnable(0, spea_unit_ordinal, sprite_no, 0))
        {
            return 0;
        }
    }
    /* Set respective R_WM_SYS_FRAME_STATUS_SPEA_SUn_UPDATE_BIT */
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_UPDATE_BIT+spea_unit_ordinal));
    
    return 1;
}

static uint32_t loc_WinGetCount(uint32_t Unit)
{
    uint32_t n = 0;
    r_wm_Window_t* cur_win = r_wm_DevSys[Unit].RootWin;

    while (cur_win != R_NULL)
    {
        cur_win = cur_win->Next;
        ++n;
    }
    return n;
}

static r_wm_sys_vdce_WinLayerPair_t* loc_WinLayerPairGet(uint32_t Unit, const r_wm_Window_t* Win)
{
    uint32_t i;
    for (i = 0; i < R_VDCE_MAX_LAYERS; ++i)
    {
        if (r_wm_DevSys[Unit].WinToLayerMap[i].Win == Win)
        {
            return &r_wm_DevSys[Unit].WinToLayerMap[i];
        }
    }
    return R_NULL;
}

static r_wm_sys_vdce_WinLayerPair_t* loc_WinLayerPairGetByLayer(uint32_t Unit, const uint32_t Layer)
{
    uint32_t i;
    for (i = 0; i < R_VDCE_MAX_LAYERS; ++i)
    {
        if (r_wm_DevSys[Unit].WinToLayerMap[i].Layer == Layer)
        {
            return &r_wm_DevSys[Unit].WinToLayerMap[i];
        }
    }
    return R_NULL;
}

static uint32_t loc_SetRle(uint32_t Unit, r_wm_WinColorFmt_t Fmt, uint32_t Buf)
{
    r_spea_RleConfig_t rle_conf;
    uint32_t err_exit = 0;
    switch (Fmt)
    {
        case R_WM_COLORFMT_RLE24ARGB8888:
        case R_WM_COLORFMT_RLE24RGB0888:
            rle_conf.ColorMode = R_SPEA_CM_24BPP;
            break;
        case R_WM_COLORFMT_RLE18ARGB8888:
        case R_WM_COLORFMT_RLE18RGB0888:
            rle_conf.ColorMode = R_SPEA_CM_18BPP;
            break;
        case R_WM_COLORFMT_RLE8CLUT8:
        case R_WM_COLORFMT_RLE8CLUT4:
        case R_WM_COLORFMT_RLE8CLUT1:
            rle_conf.ColorMode = R_SPEA_CM_8BPP;
            break;
        default:
            err_exit = 1;
            rle_conf.ColorMode = R_SPEA_CM_24BPP;
            break;
    }
    if (err_exit == 1)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_COLORFMT, 0);
        return 0;   /* R_WM_ERR_COLORFMT */
    }

    /* Both RLE units have VideoOutID 0 set as default in HW.
       R_SPEA_SetRle is not allowed to set the same VideoOutID
       as the other unit already has. Furthermore, R_SPEA_UnitEnable
       starts both units, so if the other unit operation is not desired,
       its VideoOutID needs to be set to some invalid value.
    */
    
    /* check if the other unit is already in use,
       if not, it needs to be setup with an invalid VideoOutID,
       if yes, than it already has a (valid) different VideoOutID
    */
    if  ((r_wm_DevSys[Unit ^ 1u].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_RLE) == 0uL)
    {
        /* some foo defaults */
        rle_conf.ColorMode = R_SPEA_CM_24BPP;
        rle_conf.VirtStart = 0x30000000u;
        rle_conf.MemStart = 0xffffffffu;
        rle_conf.VideoOutID = 0xff; /* an invalid Id */
        if (R_SPEA_ERR_OK != R_SPEA_SetRle(0, Unit ^ 1u, &rle_conf))
        {
            return 0;
        }
    }
   
    
    rle_conf.VirtStart = 0x30000000u;
    rle_conf.MemStart = Buf;
    rle_conf.VideoOutID = Unit;
    if (R_SPEA_ERR_OK != R_SPEA_SetRle(0, Unit, &rle_conf))
    {
        return 0;
    }
    
    /*  R_SPEA_UnitEnable actually turns both units on, but by supplying the
        invalid VideoOutID for the one we don't need, it is effectively 'turned off'
    */
    if (R_SPEA_ERR_OK != R_SPEA_UnitEnable(0, (r_spea_Unit_t)(R_SPEA_RLE0 + Unit), 1))
    {
        return 0;
    }

    /* Set respective R_WM_SYS_FRAME_STATUS_SPEA_RLEn_UPDATE_BIT */
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_RLE_UPDATE_BIT));

    /* set the flag that the unit is in use */
    r_wm_DevSys[Unit].GlobalStatusFlags |= R_WM_SYS_GLOBAL_STATUS_FLAG_RLE;
    
    return 1;
}

static uint32_t loc_CheckHWLayerCapabilities(uint32_t Layer, const r_wm_Window_t* Win)
{
    if ( ( 0 != R_WM_Priv_WindowIsRleLayer(Win) ) && ( Layer > 0 ) )
    {
        /* Only layer 0 may be an RLE layer */
        return 0;
    }
    
    if ( ( 0 != loc_WindowIsYUVLayer(Win) ) && ( Layer > 1 ) )
    {
        /* Only layer 0 and 1 may be YUV layers */
        return 0;
    }
    
    if ( ( Win->Mode == R_WM_WINMODE_SPRITES ) && ( Layer < 1 ) )
    {
        /* Only layer 1 to 3 may be SPRITE layers */
        return 0;
    }
    
    return 1;
}

static uint32_t loc_WinReconfAll(uint32_t Unit)
{
    uint32_t slot;
    uint32_t layer;
    const r_wm_Window_t* cur_win;
    r_wm_Window_t* RootWin = r_wm_DevSys[Unit].RootWin;
    r_vdce_CkARGB32_t ck_in;
    r_vdce_CkARGB32_t ck_out;
    r_vdce_Error_t vdce_err;
    const void* fb;
    
    uint32_t win_count_cond = (loc_WinGetCount(Unit) == 4);
    uint32_t is_rle_layer_cond = (R_WM_Priv_WindowIsRleLayer(RootWin) == 1);
    uint32_t is_yuv_layer_cond = 0;
    
    /* If second window has a YUV format, we need to start at layer zero, as YUV is only supported on 0 and 1, otherwise 2nd window might be mapped onto Layer 2 */
    if (loc_WinGetCount(Unit) >= 2)
    {
        if (loc_WindowIsYUVLayer(RootWin->Next) == 1)
        {
            is_yuv_layer_cond = 1;
        }
    }
    /* If layers 2 and 3 have YUV format, this is unsupported! */
    
    /* determine the start layer */
    if  (   (win_count_cond != 0)
            ||
            (is_rle_layer_cond != 0)
            ||
            (is_yuv_layer_cond != 0)
            ||
            (r_wm_DevSys[Unit].RootCapt != 0)
            
        )
    {
        /* all layers need to be used */
        layer = 0;
    }

    else
    {
        /*  refrain from using the layer 0 until really necessary,
            because of blending
        */
        loc_VDCE_LayerDisable(Unit, 0);
        layer = 1;
    }

    /* todo: don't need to reconfigure all layers always */

    slot = 0;
    cur_win = RootWin;
    while (cur_win != R_NULL)
    {
        r_wm_DevSys[Unit].WinToLayerMap[slot].Win = cur_win; 
        r_wm_DevSys[Unit].WinToLayerMap[slot].Layer = layer;
        loc_VDCE_LayerDisable(Unit, layer);
        
        /* Check if window's hardware requirements can be fulfilled by selected layer no */
        if (0 == loc_CheckHWLayerCapabilities(layer, cur_win) )
        {
            R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NO_PHYS_WINDOW, 0);
            return 0;
        }
        
        /* chroma key */
        if (cur_win->ColorKey.Enabled != 0)
        {
            ck_in.R = cur_win->ColorKey.In.RgbKey.Red;
            ck_in.G = cur_win->ColorKey.In.RgbKey.Green;
            ck_in.B = cur_win->ColorKey.In.RgbKey.Blue;
            ck_in.A = 0x00;
            
            ck_out.R = cur_win->ColorKey.Out.Red;
            ck_out.G = cur_win->ColorKey.Out.Green;
            ck_out.B = cur_win->ColorKey.Out.Blue;
            ck_out.A = cur_win->ColorKey.Out.Alpha;
            
            /* Layer Alpha and Chroma Keying are exclusive. 
               Chroma Keying overrides Layer Alpha. */
            if((0 < layer) && (layer < 4))
            {
                if (R_VDCE_ERR_OK != R_VDCE_LayerAlphaConstDisable(Unit,layer))
                {
                    return 0;
                }
            }
            if (R_VDCE_ERR_OK != R_VDCE_LayerChromaKeyEnable(Unit, layer, &ck_in, &ck_out))
            {
                return 0;
            }
        }
        
        if (R_VDCE_ERR_OK != R_VDCE_LayerFormatSet(Unit, layer, loc_GetVdceFormat(cur_win->ColorFmt)))
        {
            return 0;
        }
        if (0 < cur_win->ClutNumEntries)
        {
            r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_LAYER_0_CLUT_UPDATE_BIT+layer));
        }
		
		/* update the scaling */
        if  (   (layer == R_VDCE_LAYER_SCALER0)
            ||
            (layer == R_VDCE_LAYER_SCALER1)
        )
        {
            if (r_wm_DevSys[Unit].RootCapt->Window == cur_win)
            {
                if (r_wm_DevSys[Unit].RootCapt->ScaledWidth != 0)
                {
                    if (0 == R_VDCE_LayerImgScaleX(
                                    Unit /*r_wm_DevSys[Unit].RootCapt->CapUnit */, 
                                    Unit ^ r_wm_DevSys[Unit].RootCapt->CapUnit, 
                                    r_wm_DevSys[Unit].RootCapt->ScaledWidth, 
                                    R_VDCE_SCALING_SMALLER))
                    {
                        return 0;
                    }
                }
                if (r_wm_DevSys[Unit].RootCapt->ScaledHeight != 0)
                {
                    if (0 == R_VDCE_LayerImgScaleY(
                                    Unit /*r_wm_DevSys[Unit].RootCapt->CapUnit*/, 
                                    Unit ^ r_wm_DevSys[Unit].RootCapt->CapUnit, 
                                    r_wm_DevSys[Unit].RootCapt->ScaledHeight, 
                                    R_VDCE_SCALING_SMALLER))
                    {
                        return 0;
                    }
                }
            }
            else
            {
                if (0 == R_VDCE_LayerImgScaleX(Unit, layer, cur_win->Width, R_VDCE_SCALING_DISABLE))
                {
                    return 0;
                }
                if (0 == R_VDCE_LayerImgScaleY(Unit, layer, cur_win->Height, R_VDCE_SCALING_DISABLE))
                {
                    return 0;
                }
            }
        }

        /* Layer 0 and Layer 4 cannot be assigned constant Alpha */
        if((0 < layer) && (layer < 4) && (cur_win->ColorKey.Enabled == 0))
        {
            if (R_VDCE_ERR_OK != R_VDCE_LayerAlphaConstEnable(Unit, layer, cur_win->Alpha))
            {
                return 0;
            }
        }
        if((0 < layer) && (layer < 4))
        {
            if (R_VDCE_ERR_OK != R_VDCE_LayerAlphaChannelEnable(Unit, layer))
            {
                return 0;
            }
            
            if (R_FALSE != cur_win->UsePremultipliedAlpha)
            {
                vdce_err = R_VDCE_LayerPremultipliedAlphaEnable(Unit, layer);
            }
            else
            {
                vdce_err = R_VDCE_LayerPremultipliedAlphaDisable(Unit, layer);
            }
            if (vdce_err != R_VDCE_ERR_OK)
            {
                return 0;
            }
        }
        
        /* Set vertical mirroring */
        /* Enable vertical mirroring */
        if ( 0 != ( ((uint32_t)cur_win->Flags) & ((uint32_t)R_WM_WINFLAG_V_MIRROR) ) )
        {
            if (R_VDCE_ERR_OK != R_VDCE_LayerModeSet(Unit, layer, R_VDCE_LAYER_MODE_V_MIRORING) )
            {
                return 0;
            }
        }
    
        /* Disable vertical mirroring */
        if ( 0 != ( ((uint32_t)cur_win->Flags) & ((uint32_t)R_WM_WINFLAG_V_MIRROR) ) )
        {
            if (R_VDCE_ERR_OK != R_VDCE_LayerModeSet(Unit, layer, R_VDCE_LAYER_MODE_NONE) )
            {
                return 0;
            }
        }

        
        /* set the layer base address from the ON_SCREEN fb */
        if (cur_win->Mode != R_WM_WINMODE_SPRITES)
        {
            fb = loc_WinGetOnScreenFb(cur_win);
            if (fb != 0)
            {
                if (0 == loc_WinSetFb(Unit, cur_win, cur_win->PosX, cur_win->PosY, fb))
                {
                    return 0;
                }
            }
        }
        else
        {
            if (0 == loc_WinSetFb(Unit, cur_win, cur_win->PosX, cur_win->PosY, 0 /* will be set in the function */))
            {
                return 0;
            }
            if (cur_win->Surface.SpritesRoot != 0)
            {
                uint8_t spea_unit_ordinal;
                r_spea_Unit_t spea_unit;
                
                loc_Layer2Spea(Unit, layer, &spea_unit, &spea_unit_ordinal);
                r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_REASSIGN_BIT+spea_unit_ordinal));
            }
        }
        /* This will also enable the layer */
        if (0 == loc_WinSetViewportAndGeom( Unit,
                                            cur_win,
                                            layer,
                                            cur_win->PosX,
                                            cur_win->PosY,
                                            cur_win->Pitch,
                                            cur_win->Width,
                                            cur_win->Height,
                                            R_WM_ColorFmtBitsPerPixGet(cur_win->ColorFmt) ))
        {
            return 0;
        }

        cur_win = cur_win->Next;
        ++layer;
        ++slot;
    }
    for (; slot < R_VDCE_MAX_LAYERS; ++slot)
    {
        if (layer < R_VDCE_MAX_LAYERS)
        {
            loc_VDCE_LayerDisable(Unit, layer);
            ++layer;
        }
        r_wm_DevSys[Unit].WinToLayerMap[slot].Win = R_NULL; 
        r_wm_DevSys[Unit].WinToLayerMap[slot].Layer = LOC_INVALID_LAYER;
    }
    return 1;
}

static uint32_t loc_WinSetViewportAndGeom ( uint32_t     Unit,
                                            const r_wm_Window_t *Win,
                                            uint32_t            Layer,
                                            int32_t             PosX,
                                            int32_t             PosY,
                                            int32_t             Pitch,
                                            int32_t             Width,
                                            int32_t             Height,
                                            int32_t             Bpp
                                          )
{
    int32_t pix_per_align_step = (128 / Bpp) * 8;
    if (PosY < 0)
    {
        /* todo neg. height */
        Height += PosY; /* shorten the height */
        PosY = 0;
    }
    if (PosX < 0)
    {
        /* Instead of directly adjusting the layer's position on the screen, 
           it is also possible to change the base address of the layer.
           By increasing the base address you effectively move the layer to the left.
           
           A crucial point of this method is, the VDCE's Layer Base Adress must 
           be 128Byte aligned. This is 32px for 4BPP, 64px for 2BPP, etc...
           So this method works best, if there is a reserve of 32px, 64px, etc 
           in the blanking width to be used with conventional moving.
           
           As a consequence, moving by changing the base address can only be used 
           when the reserve is equal or bigger than 32px, 64px, etc..
        */
        if ( (0 > (r_wm_DevSys[Unit].HorizontalMoveReserve + PosX)) && 
             (pix_per_align_step>r_wm_DevSys[Unit].HorizontalMoveReserve) )
        {
            R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_SYS_WIN_MOVE_FAILED, 0);
            return 0;
        }
        /* Shorten the width aligned to 128Byte steps */
        Width -= (int32_t)((uint32_t)-PosX & ~(uint32_t)(pix_per_align_step - 1u));
        PosX = PosX % pix_per_align_step;
    }

    if((Width > 0)&&(Height > 0))
    {
        if (R_VDCE_ERR_OK != R_VDCE_LayerMemGeometrySet(Unit, Layer, Pitch, Height))
        {
            return 0;
        }
        if (R_VDCE_ERR_OK != R_VDCE_LayerViewPortSet(Unit, Layer, 0, 0, PosX, PosY, Width, Height))
        {
            return 0;
        }
    }

    if (Win->Status == R_WM_WINSTATUS_ENABLED)
    {
        if((Width <= 0)||(Height <= 0))
        {
            loc_VDCE_LayerDisable(Unit, Layer);
        }
        else
        {
            loc_VDCE_LayerEnable(Unit, Layer);
        }
    }
    return 1;
}

static uint32_t loc_WinSetFb   (   uint32_t            Unit,
                            const r_wm_Window_t *Win,
                            int32_t             PosX,
                            int32_t             PosY,
                            const void          *Fb
                        )
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    uint32_t Bpp = R_WM_ColorFmtBitsPerPixGet(Win->ColorFmt);
    int32_t add_h = 0;
    int32_t add_v = 0;
    if (Bpp == 0)
    {
        Bpp = 1;
    }
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NO_PHYS_WINDOW, 0);
        return 0;
    }

    if (R_WM_Priv_WindowIsRleLayer(Win) == 1)
    {
        /* Only the Root window may be an RLE window */
        if (Win != r_wm_DevSys[Unit].RootWin)
        {
            R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NO_PHYS_WINDOW, 0);
            return 0; /* R_WM_ERR_NO_PHYS_WINDOW */
        }

        /* RLE Buffers must be 128-byte aligned */
        if (0 != ((uint32_t)Fb&0x0000007Fu) )
        {
            R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_ADDRESS_ALIGNMENT, 0);
            return 0; /* R_WM_ERR_ADDRESS_ALIGNMENT */
        }

        if (0 == loc_SetRle(Unit, Win->ColorFmt, (uint32_t)Fb))
        {
            return 0;
        }

        /* set the address of the virtual layer */
        Fb = (void*)0x30000000u;
    }
    else if (Win->Mode == R_WM_WINMODE_SPRITES)
    {
        Fb = (void*)0x30000000u;
    }
    else
    {
    }
    
    /* check if the negative start position workaround is needed */
    if (R_WM_Priv_WindowIsRleLayer(Win) == 0)
    {
        if  (PosY < 0)
        {
            /* Move the Window up out of the screen by changing the start address stribe-by-stride */
            add_v = -PosY;
        }
        if  (PosX < 0)
        {
            /* Instead of directly adjusting the layer's position on the screen, 
               it is also possible to change the base address of the layer.
               By increasing the base address you effectively move the layer to the left.
               
               A crucial point of this method is, the VDCE's Layer Base Adress must 
               be 128Byte aligned. This is 32px for 4BPP, 64px for 2BPP, etc...
               So this method works best, if there is a reserve of 32px, 64px, etc 
               in the blanking width to be used with conventional moving.
               
               As a consequence, moving by changing the base address can only be used 
               when the reserve is equal or bigger than 32px, 64px, etc..
            */
            int32_t pix_per_align_step = (128 / Bpp) * 8;
            
            /* If PosX exceeds conventional moving range, check if requested PosX can be served by address shifting */
            if ( (0 > (r_wm_DevSys[Unit].HorizontalMoveReserve + PosX)) && 
                 (pix_per_align_step>r_wm_DevSys[Unit].HorizontalMoveReserve) )
            {
                R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_SYS_WIN_MOVE_FAILED, 0);
                return 0;
            }
            
            /* Increase address aligned to 128Byte steps */
            add_h = (int32_t)((uint32_t)(-PosX) & ~(uint32_t)(pix_per_align_step - 1u));
        }
        Fb = Fb +  (((add_h + (add_v * Win->Pitch)) * Bpp) / 8);
    }
    
    if (R_VDCE_ERR_OK != R_VDCE_LayerBaseSet(Unit, win_layer_pair->Layer, (uint32_t)Fb))
    {
        return 0;
    }
    return 1;
}

static const void* loc_WinGetOnScreenFb(const r_wm_Window_t *Win)
{
    uint32_t i;
    for (i = 0; i < Win->Surface.Fb.BufNum; ++i)
    {
        if (Win->Surface.Fb.Buffer[i].Status == R_WM_WINBUF_ON_SCREEN)
        {
            return Win->Surface.Fb.Buffer[i].Data;
        }
    }
    return 0;
}


static const r_vdce_Error_t loc_VDCE_LayerEnable(uint32_t Unit, uint32_t Layer)
{
    r_vdce_Error_t vdce_err = R_VDCE_ERR_OK;
    if (0 == r_wm_DevSys[Unit].HwLayerEnabled[Layer])
    {
        vdce_err = R_VDCE_LayerEnable(Unit, Layer);
        
        if (R_VDCE_ERR_OK == vdce_err)
        {
            r_wm_DevSys[Unit].HwLayerEnabled[Layer] = 1;
        }
    }
    
    return vdce_err;
}

static const r_vdce_Error_t loc_VDCE_LayerDisable(uint32_t Unit, uint32_t Layer)
{
    r_vdce_Error_t vdce_err = R_VDCE_ERR_OK;
    if (1 == r_wm_DevSys[Unit].HwLayerEnabled[Layer])
    {
        vdce_err = R_VDCE_LayerDisable(Unit, Layer);

        if (R_VDCE_ERR_OK == vdce_err)
        {
            r_wm_DevSys[Unit].HwLayerEnabled[Layer] = 0;
        }
    }
    
    return vdce_err;
}


static void loc_VDCE_ErrorCallback(const uint32_t Unit, const r_vdce_Error_t Error)
{
    if (R_VDCE_ERR_OK != Error)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_VOUT_INTERNAL, Error);
        R_WM_ErrorHandler(Unit, R_WM_ERR_VOUT_INTERNAL);
    }
}

static void loc_SPEA_ErrorCallback(const uint32_t Unit, uint32_t Error)
{
    if (R_SPEA_ERR_OK != Error)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_SPEA_INTERNAL, Error);
        R_WM_ErrorHandler(Unit, R_WM_ERR_SPEA_INTERNAL);
    }
}

/***************************************************************************
  Function: loc_fitCurveForVdce
  
  Takes two arrays <InValue>, <OutValue> of exactly 33 points, that represent 
  a colour correction curve. 
  
  <InValue> is expected to be in range of [0..256] including 256 for the last element!
  <OutValue is expected to be in range of [0..256] including 256 for the last element!
  
  Both series <InValue>, <OutValue> are required to start in the Origin (0,0), thus <InValue>[0] 
  and <OutValue>[0] need to be zero. 
  Non-matching <InValue> is an error! Non-matching <OutValue> will be fitted!
  
  Both series <InValue>, <OutValue> are required to be monotonically nondecreasing, thus 
  the next value is always bigger than the previous. Non-matching values will be fitted!

  Both series <InValue>, <OutValue> are required to have a slope not bigger than 2, thus 
  for a distance dIn: dOut < 2*dIn . Non-matching values will be fitted!
  
  Parameters:
  InValue       - 33 Reference points starting with ZERO
  OutValue      - 33 Reference points
  NumEntries    - Number of reference points
  FittedCurve   - Reference to the curve to be filled
*/
static uint32_t loc_fitCurveForVdce ( 
                                uint16_t   *InValue, 
                                float32_t  *OutValue, 
                                uint32_t    NumEntries,
                                r_vdce_GammaCorrect_t *FittedCurve )
{
    const float32_t max_gain = 2047.0f/1024.0f;
    
    float32_t fittedOutValue_last = 0.0f;
    float32_t fittedOutValue = 0.0f;

    uint32_t  i;
    uint8_t   areaSize;
    
    /* Expecting exactly 33 entries */
    if ( 33 != NumEntries )
    {
        return 0;
    }
    
    /* First entry is expected to start with zero */
    if ( InValue[0] != 0 )
    {
        return 0;
    }
    
    for (i=1; i< NumEntries; i++)
    {
        /* Restrict the area size to positive values bigger than zero */
        if ( InValue[i] <= InValue[i-1] )
        {
            areaSize = 1;
        }
        else
        {
            areaSize = InValue[i] - InValue[i-1];
        }
        
        /* Calculate a (new, if necessary) output value that lies within the HW restrictions */
        fittedOutValue = fittedOutValue_last
                       + loc_maxf( loc_minf( 
                                        OutValue[i] - fittedOutValue_last, 
                                        areaSize * max_gain ),
                                      0.0f );
        
        /* Calculate the corresponding gain and area */
        FittedCurve->Gain[i-1] = loc_iroundf( loc_minf( 2047.0f,
                                  ((fittedOutValue - fittedOutValue_last) * 1024.0f) / areaSize )  );
        FittedCurve->Area[i-1] = InValue[i-1];
        
        fittedOutValue_last = fittedOutValue;
    }
    
    return 1;
}

/***************************************************************************
  Function: loc_getGammaCurve
  
  Fill the supplied Struct with a color correction curve, that matches the given Gamma Coefficient.
  
  Parameters:
  Gamma     - Gamma coeficcient [32..256]. Internall scaled by 1/128. 
              Floating-point range is [0.25f .. 2.0f]
  Curve     - Color correction curve to be input into VDCE.
*/
static uint32_t loc_getGammaCurve( uint32_t Unit, uint8_t Gamma, r_vdce_GammaCorrect_t *Curve )
{
    /* For the gamma correction curve, we take a constant size of the segements so they are equally distributed */
    const uint8_t   seg_size = 8;
    
    float32_t f_gamma;
    uint8_t   segment;
    
    uint16_t  In[33] = {0};
    float32_t Out[33] = {0};
    
    if ( 0 == Curve )
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
    f_gamma = Gamma / 128.0f;
    f_gamma = (0.25f > f_gamma) ? 0.25f : f_gamma;

    /* Step 0 is special. The curve starts always in the origin (0,0) */
    In[0] =   0u;
    Out[0] =  0.0f;

    for (segment = 1; segment<33; segment++)
    {
        /* Input brightness value */
        In[segment] = (segment) * seg_size;
        
        /* Output value as we want it */
        Out[segment] = 255.0f * loc_powf( In[segment]/255.0f, 1.0f/f_gamma );
    }
    
    /* Create a curve over the input/output values, staying in the capabilities of the VDCE */
    if (0 == loc_fitCurveForVdce ( In, Out, 33, Curve))
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_PARAM_INCORRECT, 0);
        return 0;
    }
    
    return 1;
}
/***********************************************************
  Section: WM Driver Support Functions
  
  Comments see: <WM Support Functions>
*/


/***********************************************************
  Group: WM Basic interface functions
*/


/***********************************************************
  Function: R_WM_Sys_Init
*/

uint32_t R_WM_Sys_DevInit   (   uint32_t Unit,
                                void (*EventCb)(uint32_t Unit, const r_wm_Event_t *Event)
                            )
{
    uint32_t i;
    
#ifdef LOC_USE_OS

    /* create a mutex for access to the event queue*/
    R_OS_ThreadMutexInit(&locMutexMsgQueue[Unit], 0);

    /* create a mutex for access to the buffers of a window */
    R_OS_ThreadMutexInit(&locMutexBufferMgmnt[Unit], 0);

    /* create a mutex for global WM locking */
    R_OS_ThreadMutexInit(&locMutexWindows[Unit], 0);

    R_OS_SemInit(&locSemaMsgTrigger[Unit], 0, 0);
    
   for (i = 0; i < R_WM_EVENT_LAST; ++i)
   {		 
       R_OS_SemInit(&locSemaISRTrigger[Unit][i], 0, 0);
   }
#endif

    loc_WaitVblankInt[Unit] = 0;
    loc_WaitScanlineInt[Unit] = 0;

    for (i = 0; i < R_VDCE_MAX_LAYERS; ++i)
    {
        r_wm_DevSys[Unit].WinToLayerMap[i].Win = R_NULL;
        r_wm_DevSys[Unit].WinToLayerMap[i].Layer = LOC_INVALID_LAYER;
        r_wm_DevSys[Unit].HwLayerEnabled[i] = 0;
    }
    r_wm_DevSys[Unit].EventCb = EventCb;
    r_wm_DevSys[Unit].GlobalStatusFlags = 0;
    r_wm_DevSys[Unit].HorizontalMoveReserve = 0;
    
    if (R_VDCE_ERR_OK != R_VDCE_Init(Unit))
    {
        return 0;
    }
    if (R_VDCE_ERR_OK != R_VDCE_ErrorCallbackSet(&loc_VDCE_ErrorCallback))
    {
        return 0;
    }

    if (R_SPEA_ERR_OK != R_SPEA_Init(0))
    {
        return 0;
    }
    if (R_SPEA_ERR_OK != R_SPEA_UnitEnable(0, R_SPEA_SU0, 1))
    {
        return 0;
    }
    if (R_SPEA_ERR_OK != R_SPEA_UnitEnable(0, R_SPEA_SU1, 1))
    {
        return 0;
    }
    if (R_SPEA_ERR_OK != R_SPEA_UnitEnable(0, R_SPEA_SU2, 1))
    {
        return 0;
    }
    R_SPEA_SetErrorCallback(&loc_SPEA_ErrorCallback);
    
    if (R_VDCE_ERR_OK != R_VDCE_DisplayDisable(Unit))
    {
        return 0;
    }
    
    return 1;
}
   
/***********************************************************
  Function: R_WM_Sys_DevRootWinSet
*/
void R_WM_Sys_DevRootWindowSet(uint32_t Unit, r_wm_Window_t* RootWin)
{
    r_wm_DevSys[Unit].RootWin = RootWin;
}

/***********************************************************
  Function: R_WM_Sys_DevRootCaptureSet
*/
void R_WM_Sys_DevRootCaptureSet(uint32_t Unit, r_wm_Capture_t* RootCapt)
{
    r_wm_DevSys[Unit].RootCapt = RootCapt;
}

/***********************************************************
  Function: R_WM_Sys_DevWaitForHwWriteReady
*/
void R_WM_Sys_DevWaitForHwWriteReady(uint32_t Unit)
{
#ifdef LOC_USE_OS
    /* wait for the SCANLINE interrupt */
    loc_WaitScanlineInt[Unit]++;

    /* Don't wait if a shutdown request flag is set */
    if (0 != (r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_SHUTDOWN) )
    {
        return;
    }
    R_OS_SemWait(&locSemaISRTrigger[Unit][R_WM_EVENT_SCANLINE]);
#else
    /* wait for the SCANLINE interrupt */
    loc_WaitScanlineInt[Unit] = 1;

    while (loc_WaitScanlineInt[Unit] == 1)
    {
    }
#endif
}

/***********************************************************
  Function: R_WM_Sys_DevWaitForHwUpdated
*/
void R_WM_Sys_DevWaitForHwUpdated(uint32_t Unit)
{
#ifdef LOC_USE_OS
    /* wait for the VBLANK interrupt */
    loc_WaitVblankInt[Unit]++;

    /* Don't wait if a shutdown request flag is set */
    if (0 != (r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_SHUTDOWN) )
    {
        return;
    }
    R_OS_SemWait(&locSemaISRTrigger[Unit][R_WM_EVENT_VBLANK]);
#else
    /* wait for the VBLANK interrupt */
    loc_WaitVblankInt[Unit] = 1;

    while (loc_WaitVblankInt[Unit] == 1)
    {
    }
#endif
}

/***********************************************************
  Function: R_WM_Sys_DevFrameStarted
*/
void R_WM_Sys_DevFrameStarted(uint32_t Unit)
{
    /* reset the sprite unit update flags */
    r_wm_DevSys[Unit].FrameStatusFlags = 0;
}

/***********************************************************
  Function: R_WM_Sys_DevFrameFinished
*/
static r_wm_sys_vdce_WinLayerPair_t* loc_Spea2WinLayerPair(uint32_t Unit, uint8_t SpeaUnitOrdinal);
static r_wm_sys_vdce_WinLayerPair_t* loc_Spea2WinLayerPair(uint32_t Unit, uint8_t SpeaUnitOrdinal)
{
    r_wm_Window_t* cur_win = r_wm_DevSys[Unit].RootWin;
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair;
    uint8_t spea_unit_ordinal;
    r_spea_Unit_t spea_unit;
    
    /* Find Root Win */
    while (cur_win != R_NULL)
    {
        win_layer_pair = loc_WinLayerPairGet(Unit, cur_win);
        loc_Layer2Spea(Unit, win_layer_pair->Layer, &spea_unit, &spea_unit_ordinal);
        
        if (SpeaUnitOrdinal == spea_unit_ordinal)
        {
            return win_layer_pair;
        }
        
        cur_win = cur_win->Next;
    }
    
    return 0;
}


void R_WM_Sys_DevFrameFinished(uint32_t Unit)
{
    /* check the sprite unit update flags */
    uint32_t spea_unit_ordinal;
    r_spea_Unit_t spea_unit;
    uint32_t layer;
    
    /* Rewrite the Sprite configuration for those units that got changed during the last frame */
    for (spea_unit_ordinal = 0; spea_unit_ordinal < 3; ++spea_unit_ordinal)
    {
        /* Read respective R_WM_SYS_FRAME_STATUS_SPEA_SUn_REASSIGN_BIT */
        if ((r_wm_DevSys[Unit].FrameStatusFlags & (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_REASSIGN_BIT+spea_unit_ordinal))) != 0)
        {
            /* Update respective window's sprites */
            r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_Spea2WinLayerPair(Unit, spea_unit_ordinal);
            
            if ( (win_layer_pair != 0) && (win_layer_pair->Win->Mode != R_WM_WINMODE_SPRITES) )
            {
                /* Layer is no sprite layer (anymore), deinitialise all sprites */
                /* Breakpoint here to validate if sprites root is NULL in this case. */
                /* asm("NOP"); */
            }
            
            if (win_layer_pair != 0)
            {
                if (0 == loc_ReassignAllSprites(Unit, win_layer_pair->Win->Surface.SpritesRoot, win_layer_pair->Layer))
                {
    #if 0
                    /* This API function <R_WM_Sys_DevFrameFinished> cannot return a value, so just do nothing */
                    return 0;
    #endif
                }
            }
            else
            {
                /* No more sprites and no more window */
                loc_SpeaOrdinal2Layer(Unit, spea_unit_ordinal, &layer);
                if (0 == loc_ReassignAllSprites(Unit, R_NULL, layer))
                {
    #if 0
                    /* This API function <R_WM_Sys_DevFrameFinished> cannot return a value, so just do nothing */
                    return 0;
    #endif
                }
            }
        }
    }
    
    /* Set update request bit for those Sprite Units, that have been written to in the last frame */
    for (spea_unit_ordinal = 0; spea_unit_ordinal < 3; ++spea_unit_ordinal)
    {
        /* Read respective R_WM_SYS_FRAME_STATUS_SPEA_SUn_UPDATE_BIT */
        if ((r_wm_DevSys[Unit].FrameStatusFlags & (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_UPDATE_BIT+spea_unit_ordinal))) != 0)
        {
            spea_unit = (r_spea_Unit_t)(R_SPEA_SU0 + spea_unit_ordinal);
            if (R_SPEA_ERR_OK != R_SPEA_UpdateUnit(0, spea_unit, Unit))
            {
                /* This API function <R_WM_Sys_DevFrameFinished> cannot return a value, so just do nothing */
#if 0
                return 0;
#endif
            }
        }
    }
    
    /* Process update request for RLE Unit, if it has been written to in the last frame */
    if ( (r_wm_DevSys[Unit].FrameStatusFlags & (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_RLE_UPDATE_BIT))) != 0 )
    {
        if (R_SPEA_ERR_OK != R_SPEA_UpdateUnit(0, (r_spea_Unit_t)(R_SPEA_RLE0+Unit), Unit))
        {
    #if 0
            return 0;
    #endif
        }
    }
    
    /* If no window was visible in this frame, pending SPEA Update Requests cannot be completed immediately.
       => Manually create a SYNC signal. 
          Flickering or artifacts are no problem as nothing is visible on-screen! */
    if ( ((r_wm_DevSys[Unit].FrameStatusFlags & R_WM_SYS_FRAME_STATUS_SPEA_UPDATE_MASK) != 0)
        && ((r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_WINDOWS_VISIBLE) == 0) )
    {
        uint32_t vdcectl = R_DEV_READ_REG(32, VDCECTL);
        
    #if defined R_WM_MSQ_DEQUEUE_PROFILING || defined R_WM_MSQ_ENQUEUE_PROFILING
        R_WM_Profile_Log_CheckSPEAUpd();
    #endif

        /* Check UM 37.6 "Hardware and Software Update" about the use of the UPDTn Bit */
        /* Set SYNC(Unit) Signal to 1 */
        R_DEV_WRITE_REG(32, VDCECTL, vdcectl | ((0 == Unit) ? VDCECTL_UPDT0 : VDCECTL_UPDT1) );
        
        /* Set to 0 to create falling edge */
        R_DEV_WRITE_REG(32, VDCECTL, vdcectl);
        
        
    #if defined R_WM_MSQ_DEQUEUE_PROFILING || defined R_WM_MSQ_ENQUEUE_PROFILING
        R_WM_Profile_Log_SPEASWSync();
        for (vdcectl=0; vdcectl<10; vdcectl++) {
            R_WM_Profile_Log_CheckSPEAUpd();
        }
    #endif
    }
    
    /* Check if a CLUT update is pending for any layer */
    if ((r_wm_DevSys[Unit].FrameStatusFlags & R_WM_SYS_FRAME_STATUS_LAYER_CLUT_UPDATE_MASK) != 0)
    {
        for (layer = 0; layer < R_VDCE_MAX_LAYERS; layer++)
        {
            /* Read respective R_WM_SYS_FRAME_STATUS_LAYER_0_CLUT_UPDATE_BIT */
            if ((r_wm_DevSys[Unit].FrameStatusFlags & (1uL << (R_WM_SYS_FRAME_STATUS_LAYER_0_CLUT_UPDATE_BIT+layer))) != 0)
            {
                r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGetByLayer(Unit, layer);
                
                if (0 < win_layer_pair->Win->ClutNumEntries)
                {
                    if (1 == r_wm_DevSys[Unit].HwLayerEnabled[layer])
                    {
                        if (R_VDCE_ERR_OK != R_VDCE_LayerDisable(Unit, layer) )
                        {
                    #if 0
                            return 0;
                    #endif
                        }
                    }
                    if (R_VDCE_ERR_OK != R_VDCE_LayerClutSet(Unit, layer, 0, win_layer_pair->Win->ClutNumEntries, (r_vdce_ClutARGB32_t*)(win_layer_pair->Win->Clut)))
                    {
                #if 0
                        return 0;
                #endif
                    }
                    if (1 == r_wm_DevSys[Unit].HwLayerEnabled[layer])
                    {
                        if (R_VDCE_ERR_OK != R_VDCE_LayerEnable(Unit, layer) )
                        {
                    #if 0
                            return 0;
                    #endif
                        }
                    }
                }
            }
        }
    }
    
    /* Check if we will have visible windows in the following frame */
    {
        uint32_t status = 0;
        
        for(layer = 0; layer < R_VDCE_MAX_LAYERS; layer++)
        {
            if (r_wm_DevSys[Unit].HwLayerEnabled[layer] != 0)
            {
                status = 1;
            }
        }
        if (status != 0)
        {
            r_wm_DevSys[Unit].GlobalStatusFlags |= R_WM_SYS_GLOBAL_STATUS_FLAG_WINDOWS_VISIBLE;
        }
        else
        {
            r_wm_DevSys[Unit].GlobalStatusFlags &= ~R_WM_SYS_GLOBAL_STATUS_FLAG_WINDOWS_VISIBLE;
        }
    }
}
   
/***********************************************************
  Function: R_WM_Sys_DeInit
*/

uint32_t R_WM_Sys_DevDeinit(uint32_t  Unit)
{
#ifdef LOC_USE_OS
    uint32_t i;
#endif

	/* Preparing shutdown by trying to escape from any wait states */
    r_wm_DevSys[Unit].GlobalStatusFlags |= R_WM_SYS_GLOBAL_STATUS_FLAG_SHUTDOWN;

#ifdef LOC_USE_OS
    /* Make sure, wm isn't waiting for some events */
    while (loc_WaitVblankInt[Unit] != 0)
    {
        R_OS_SemPost(&locSemaISRTrigger[Unit][R_WM_EVENT_VBLANK]);
        loc_WaitVblankInt[Unit]--;
    }
    while (loc_WaitScanlineInt[Unit] != 0)
    {
        R_OS_SemPost(&locSemaISRTrigger[Unit][R_WM_EVENT_SCANLINE]);
        loc_WaitScanlineInt[Unit]--;
    }
    
    /* delete mutex */
    R_OS_ThreadMutexDestroy(&locMutexWindows[Unit]);
    R_OS_ThreadMutexDestroy(&locMutexMsgQueue[Unit]);
    R_OS_ThreadMutexDestroy(&locMutexBufferMgmnt[Unit]);
    R_OS_ThreadSleep(20);
    R_OS_SemPost(&locSemaMsgTrigger[Unit]);
    R_OS_ThreadSleep(20); /* (todo) thread sync. via sleep() might cause problems during shutdown */
    R_OS_SemDestroy(&locSemaMsgTrigger[Unit]);

    for (i = 0; i < R_WM_EVENT_LAST; ++i)
    {		 
        R_OS_SemDestroy(&locSemaISRTrigger[Unit][i]);
    }
#else
	loc_WaitVblankInt[Unit] = 0;
    loc_WaitScanlineInt[Unit] = 0;
#endif

    if (R_VDCE_ERR_OK != R_VDCE_DeInit(Unit))
    {
        return 0;
    }
    return 1;
}

/***********************************************************
  Group: WM Memory functions
*/
/***********************************************************
  Function: R_WM_Sys_Heap_Set
*/
void R_WM_Sys_Heap_Set(const void *Cpu, const void *Video)
{
    loc_CpuHeap_WM = (r_cdi_Heap_t*)Cpu;
    loc_VidHeap_WM = (r_cdi_Heap_t*)Video;
}

/***********************************************************
  Function: R_WM_Sys_Alloc
*/

void* R_WM_Sys_Alloc    (   uint32_t      Size,
                            r_wm_Memory_t MemType
                        )
{

    uint32_t addr;
    switch (MemType)
    {
    case R_WM_MEM_CPU:
        if (0 == loc_CpuHeap_WM)
        {
            addr = 0;
        }
        else
        {
            addr = (uint32_t)R_CDI_Alloc(loc_CpuHeap_WM, Size);
        }
    	break;
    case R_WM_MEM_VIDEO:
        if (0 == loc_VidHeap_WM)
        {
            addr = 0;
        }
        else
        {
            addr = (uint32_t)R_CDI_Alloc(loc_VidHeap_WM, Size);
        }
        break;
    default: 
        addr = 0;
        break;
    }
    return (void*)addr;

}


/***********************************************************
  Function: R_WM_Sys_Free
*/

uint32_t R_WM_Sys_Free  (   void*           Memory,
                            r_wm_Memory_t   MemType
                        )
{
    if (MemType == R_WM_MEM_CPU)
    {
        if (0 == loc_CpuHeap_WM)
        {
            R_WM_SYS_REPORT_ERROR(0xff, R_WM_ERR_MALLOC_FAILED, 0);
            return 0;
        }
        R_CDI_Free((uint32_t)Memory, loc_CpuHeap_WM);
    }
    else
    if (MemType == R_WM_MEM_VIDEO)
    {
        if (0 == loc_VidHeap_WM)
        {
            R_WM_SYS_REPORT_ERROR(0xff, R_WM_ERR_MALLOC_FAILED, 0);
            return 0;
        }
        R_CDI_Free((uint32_t)Memory, loc_VidHeap_WM);
    }
    else
    {
        R_WM_SYS_REPORT_ERROR(0xff, R_WM_ERR_MALLOC_FAILED, 0);
        return 0;
    }
    return 1;
}

/***********************************************************
  Group: WM Drawing engine interface
*/

/***********************************************************
  Group: WM Video input interface
*/


/***********************************************************
  Group: WM OS interface
*/

/***********************************************************
  Function: R_WM_Sys_LockWindows
*/

void R_WM_Sys_LockWindows(uint32_t Unit)
{
     if (Unit >= R_WM_DEV_NUM)
     {
        R_WM_ErrorHandler(Unit, R_WM_ERR_INVALID_WM_UNIT);
        return;
     }

#ifdef LOC_USE_OS
    R_OS_ThreadMutexLock(&locMutexWindows[Unit]);
#endif
}


/***********************************************************
  Function: R_WM_Sys_UnlockWindows
*/

void R_WM_Sys_UnlockWindows(uint32_t Unit)
{
     if (Unit >= R_WM_DEV_NUM)
     {
        R_WM_ErrorHandler(Unit, R_WM_ERR_INVALID_WM_UNIT);
        return;
     }

#ifdef LOC_USE_OS
    R_OS_ThreadMutexUnlock(&locMutexWindows[Unit]);
#endif
}


/***********************************************************
  Function: R_WM_Sys_LockMsgQueue
*/

void R_WM_Sys_LockMsgQueue(uint32_t Unit)
{
#ifdef LOC_USE_OS
    R_OS_ThreadMutexLock(&locMutexMsgQueue[Unit]);
#endif
}

/***********************************************************
  Function: R_WM_Sys_LockMsgQueue
*/

uint32_t R_WM_Sys_TryLockMsgQueue(uint32_t Unit)
{
#ifdef LOC_USE_OS
    R_OS_ThreadMutexLock(&locMutexMsgQueue[Unit]);
#endif
    return 1;
}


/***********************************************************
  Function: R_WM_Sys_UnlockMsgQueue
*/

void R_WM_Sys_UnlockMsgQueue(uint32_t Unit)
{
     if (Unit >= R_WM_DEV_NUM)
     {
        R_WM_ErrorHandler(Unit, R_WM_ERR_INVALID_WM_UNIT);
        return;
     }
#ifdef LOC_USE_OS
    R_OS_ThreadMutexUnlock(&locMutexMsgQueue[Unit]);
#endif
}

/***********************************************************
  Function: R_WM_Sys_LockBuffers
*/

void R_WM_Sys_LockBuffers(uint32_t Unit)
{
     if (Unit >= R_WM_DEV_NUM)
     {
        R_WM_ErrorHandler(Unit, R_WM_ERR_INVALID_WM_UNIT);
        return;
     }

#ifdef LOC_USE_OS
    R_OS_ThreadMutexLock(&locMutexBufferMgmnt[Unit]);
#endif
}


/***********************************************************
  Function: R_WM_Sys_UnlockBuffers
*/

void R_WM_Sys_UnlockBuffers(uint32_t Unit)
{
     if (Unit >= R_WM_DEV_NUM)
     {
        R_WM_ErrorHandler(Unit, R_WM_ERR_INVALID_WM_UNIT);
        return;
     }

#ifdef LOC_USE_OS
    R_OS_ThreadMutexUnlock(&locMutexBufferMgmnt[Unit]);
#endif
}

/***********************************************************
  Function: R_WM_Sys_LockDevice
*/

void R_WM_Sys_LockDevice(uint32_t Unit)
{
     if (Unit >= R_WM_DEV_NUM)
     {
        R_WM_ErrorHandler(Unit, R_WM_ERR_INVALID_WM_UNIT);
        return;
     }

#ifdef LOC_USE_OS
    if (0 == locMutexInitCreated[Unit])
    {
        R_OS_ThreadMutexInit(&locMutexDevice[Unit], 0);
        locMutexInitCreated[Unit] = 1;
    }
    R_OS_ThreadMutexLock(&locMutexDevice[Unit]);
#endif
}


/***********************************************************
  Function: R_WM_Sys_UnlockDevice
*/

void R_WM_Sys_UnlockDevice(uint32_t Unit)
{
#ifdef LOC_USE_OS
    R_OS_ThreadMutexUnlock(&locMutexDevice[Unit]);
#endif
}

/***********************************************************
  Function: R_WM_Sys_ScreenBgColorSet
*/

uint32_t R_WM_Sys_ScreenBgColorSet(uint32_t Unit, 
                                       uint8_t  Red, 
                                       uint8_t  Green, 
                                       uint8_t  Blue)
{
    if ( R_VDCE_ERR_OK != R_VDCE_DisplayColorSet(Unit, Red, Green, Blue) )
    {
        return 0;
    }
    return 1;
}

uint32_t R_WM_Sys_WindowSetFb(uint32_t Unit, const r_wm_Window_t* Win, const void* Fb)
{
    return loc_WinSetFb(Unit, Win, Win->PosX, Win->PosY, Fb);
}

/***********************************************************
  Function: R_WM_Sys_ScreenColorCurveSet
*/
uint32_t R_WM_Sys_ScreenColorCurveSet(  uint32_t Unit,
                                        const r_wm_ClutEntry_t *ColorCurve,
                                        uint32_t NumEntries
                                      )
{
    r_vdce_GammaCorrect_t R = {0}, G = {0}, B = {0};
    r_vdce_Gamma_t vdceGamma = {0};
    
    uint32_t i;
    
    uint16_t  InValue[33];
    float32_t OutValueR[33], OutValueG[33], OutValueB[33];

    if ( (0 == ColorCurve) || (33 != NumEntries) )
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_PARAM_INCORRECT, 0);
        return 0;
    }
    
    vdceGamma.R = &R;
    vdceGamma.G = &G;
    vdceGamma.B = &B;

    for (i=0; i<33; i++)
    {
        InValue[i] =  i*8;
        OutValueR[i] = ColorCurve[i].R;
        OutValueG[i] = ColorCurve[i].G;
        OutValueB[i] = ColorCurve[i].B;
    }
    
    if (0 == loc_fitCurveForVdce ( InValue, OutValueR, 33, &R ))
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_PARAM_INCORRECT, 0);
        return 0;
    }
    if (0 == loc_fitCurveForVdce ( InValue, OutValueG, 33, &G ))
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_PARAM_INCORRECT, 0);
        return 0;
    }
    if (0 == loc_fitCurveForVdce ( InValue, OutValueB, 33, &B ))
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_PARAM_INCORRECT, 0);
        return 0;
    }
    
    if ( R_VDCE_ERR_OK !=  R_VDCE_DisplayGammaCorrectSet( Unit, &vdceGamma ) )
    {
        return 0;
    }
    
    return 1;
}

/***************************************************************************
  Function: R_WM_Sys_ScreenGammaSet
*/
uint32_t R_WM_Sys_ScreenGammaSet(   uint32_t Unit, 
                                    const uint8_t GammaRed, 
                                    const uint8_t GammaGreen, 
                                    const uint8_t GammaBlue 
                                )
{
    r_vdce_GammaCorrect_t vdceGammaRed =   {0};
    r_vdce_GammaCorrect_t vdceGammaGreen = {0};
    r_vdce_GammaCorrect_t vdceGammaBlue =  {0};
    r_vdce_Gamma_t vdceGamma = {0};

    if ( (128 == GammaRed) && (128 == GammaGreen) && (128 == GammaBlue) )
    {
        /* All one? Then turn off Gamma correction */
        if ( R_VDCE_ERR_OK !=  R_VDCE_DisplayGammaCorrectSet( Unit, R_NULL ) )
        {
            return 0;
        }
    }
    else
    {
        if ( 0 == loc_getGammaCurve( Unit, GammaRed, &vdceGammaRed ))
        {
            return 0;
        }
        vdceGamma.R =    &vdceGammaRed;
        
        /* Same as red gamma? Reuse red calculation */
        if ( GammaGreen == GammaRed )
        {
            vdceGamma.G =    &vdceGammaRed;
        }
        else
        {
            if ( 0 == loc_getGammaCurve( Unit, GammaGreen, &vdceGammaGreen ))
            {
                return 0;
            }
            vdceGamma.G =    &vdceGammaGreen;
        }
        
        /* Same as red gamma? Reuse red calculation */
        if ( GammaBlue == GammaRed )
        {
            vdceGamma.B =    &vdceGammaRed;
        }
        /* Same as green gamma? Reuse green calculation */
        else if ( GammaBlue == GammaGreen )
        {
            vdceGamma.B =    &vdceGammaGreen;
        }
        else
        {
            if ( 0 == loc_getGammaCurve( Unit, GammaBlue, &vdceGammaBlue ))
            {
                return 0;
            }
            vdceGamma.B =    &vdceGammaBlue;
        }
        
        if ( R_VDCE_ERR_OK !=  R_VDCE_DisplayGammaCorrectSet( Unit, &vdceGamma ) )
        {
            return 0;
        }
    }
    
    return 1;
}


/***********************************************************
  Function: R_WM_Sys_DevInfoGet
*/

void  R_WM_Sys_DevInfoGet   (   uint32_t  Unit,
                                uint32_t* MaxLayers,
                                uint32_t* MaxPitch, 
                                uint32_t* MaxWidth, 
                                uint32_t* MaxHeight,
                                uint32_t* SpritesOrderAscending
                            )
{
    if (0 != MaxLayers)
    {
        *MaxLayers = R_VDCE_MAX_LAYERS;
    }
    if (0 != MaxPitch)
    {
       *MaxPitch = R_VDCE_MAX_LAYER_PITCH;
    }
    if (0 != MaxWidth)
    {
       *MaxWidth = R_VDCE_MAX_LAYER_WIDTH;
    }
    if (0 != MaxHeight)
    {
       *MaxHeight = R_VDCE_MAX_LAYER_HEIGHT;
    }
    if (0 != SpritesOrderAscending)
    {
        *SpritesOrderAscending = 0;
    }
}

/***********************************************************
  Function: R_WM_Sys_ScreenEnable
*/

uint32_t R_WM_Sys_ScreenEnable(uint32_t Unit, uint32_t Enabled)
{
    r_vdce_Error_t err;
    if (Enabled == 1)
    {
        if (R_VDCE_ERR_OK != R_VDCE_IntcEnable(Unit, R_VDCE_INTC_SCANLINE))
        {
            return 0;
        }
        if (R_VDCE_ERR_OK != R_VDCE_IntcEnable(Unit, R_VDCE_INTC_VBLANK))
        {
            return 0;
        }
        err = R_VDCE_DisplayEnable(Unit);
    }
    else
    {
        if (R_VDCE_ERR_OK != R_VDCE_IntcDisable(Unit, R_VDCE_INTC_SCANLINE))
        {
            return 0;
        }
        if (R_VDCE_ERR_OK != R_VDCE_IntcDisable(Unit, R_VDCE_INTC_VBLANK))
        {
            return 0;
        }
        err = R_VDCE_DisplayDisable(Unit);
    }
    
    if (err != R_VDCE_ERR_OK)
    {
        return 0;
    }
    return 1;
}

uint32_t R_WM_Sys_ScreenTimingSetByName     (   uint32_t        Unit,
                                                const int8_t*   Name
                                            )
{
    r_ddb_Timing_t *timing;

    if ((Name == 0) || (*Name == 0))
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_PARAM_INCORRECT, 0);
        return 0;
    }
    timing = R_DDB_GetDisplayTiming((const char*) Name);

    return R_WM_Sys_ScreenTimingSet(Unit, timing);
}


uint32_t R_WM_Sys_DevEventRegister  (   uint32_t        Unit,
                                        r_wm_EventId_t  EventId,
                                        uint32_t        Arg
                                    )
{
    r_wm_DevSys[Unit].RegEvents |= (uint32_t)(1uL << EventId);
    if (EventId == R_WM_EVENT_SCANLINE)
    {
        if (R_VDCE_ERR_OK != R_VDCE_IntcCallbackSet(Unit, R_VDCE_INTC_SCANLINE, loc_Isr[Unit][R_WM_EVENT_SCANLINE]))
        {
            return 0;
        }
        if (R_VDCE_ERR_OK != R_VDCE_IntcScanlineSet(Unit, Arg))
        {
            return 0;
        }
        if (R_VDCE_ERR_OK != R_VDCE_IntcEnable(Unit, R_VDCE_INTC_SCANLINE))
        {
            return 0;
        }
    }
    return 1;
}

/***********************************************************
  Function: R_WM_Sys_ScreenTimingSet
*/

uint32_t R_WM_Sys_ScreenTimingSet(uint32_t Unit, r_ddb_Timing_t *timing)
{
    if (timing == 0)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_PARAM_INCORRECT, 0);
        return 0;
    }

    if(R_VDCE_ERR_OK != R_VDCE_DisplayTimingSet(Unit, timing))
    {
	    return 0;
	}
    /* set the interrupt service routines */
    /* SCANLINE interrupt */
    if (R_VDCE_ERR_OK != R_VDCE_IntcCallbackSet(Unit, R_VDCE_INTC_SCANLINE, loc_Isr[Unit][R_WM_EVENT_SCANLINE]))
    {
        return 0;
    }

    if (timing->ScreenHeight > 240)
    {
        if (R_VDCE_ERR_OK != R_VDCE_IntcScanlineSet(Unit, timing->ScreenHeight-50))
        {
            return 0;
        }
    }
    else
    {
        if (R_VDCE_ERR_OK != R_VDCE_IntcScanlineSet(Unit, timing->ScreenHeight/10))
        {
            return 0;
        }
    }
    if (R_VDCE_ERR_OK != R_VDCE_IntcEnable(Unit, R_VDCE_INTC_SCANLINE))
    {
        return 0;
    }

    /* VBLANK interrupt */
    if (R_VDCE_ERR_OK != R_VDCE_IntcCallbackSet(Unit, R_VDCE_INTC_VBLANK, loc_Isr[Unit][R_WM_EVENT_VBLANK]))
    {
        return 0;
    }
    if (R_VDCE_ERR_OK != R_VDCE_IntcEnable(Unit, R_VDCE_INTC_VBLANK))
    {
        return 0;
    }
    
    r_wm_DevSys[Unit].HorizontalMoveReserve = timing->H.BlankWidth - 16;

    return 1;
}


/***********************************************************
  Function: R_WM_Sys_ScreenColorFormatSet
*/

uint32_t R_WM_Sys_ScreenColorFormatSet  (   uint32_t            Unit,
                                                r_wm_OutColorFmt_t  OutFmt
                                            )
{
    uint32_t  Format = ((uint32_t)OutFmt&(~(uint32_t)R_WM_OUTCOLORFMT_FLAG_MASK));
    uint32_t  Flags  = ((uint32_t)OutFmt&( (uint32_t)R_WM_OUTCOLORFMT_FLAG_MASK));

    r_vdce_OutEndian_t  VdceOutEndian = R_VDCE_OUT_ENDIAN_LAST;
    r_vdce_OutFormat_t  VdceOutFormat = R_VDCE_OUT_FORMAT_LAST;
    r_vdce_OutSwap_t    VdceOutSwap   = R_VDCE_OUT_SWAP_BR_ON;//

    /* Parse flags endian */
    if (0 == ( Flags & (uint32_t)R_WM_OUTCOLORFMT_FLAG_ENDIAN ) )
    {
        /* Default value when flag is not set. */
        VdceOutEndian = R_VDCE_OUT_ENDIAN_LITTLE;
    }
    else
    {
        VdceOutEndian = R_VDCE_OUT_ENDIAN_BIG;

        /* Remove parsed flag */
        Flags = Flags & ~(uint32_t)R_WM_OUTCOLORFMT_FLAG_ENDIAN;
    }

    /* Parse flags SWAP */
    if (0 == ( Flags & (uint32_t)R_WM_OUTCOLORFMT_FLAG_SWAP_BR ) )
    {
        /* Default value when flag is not set. */
        VdceOutSwap = R_VDCE_OUT_SWAP_BR_ON;//
    }
    else
    {
        VdceOutSwap = R_VDCE_OUT_SWAP_BR_ON;

        /* Remove parsed flag */
        Flags = Flags & ~(uint32_t)R_WM_OUTCOLORFMT_FLAG_SWAP_BR;
    }

    /* Parse color formats */
    switch(Format)
    {
        case R_WM_OUTCOLORFMT_RGB888:
            VdceOutFormat = R_VDCE_OUT_FORMAT_RGB888;
            break;
        case R_WM_OUTCOLORFMT_RGB666:
            VdceOutFormat = R_VDCE_OUT_FORMAT_RGB666;
            break;
        case R_WM_OUTCOLORFMT_RGB565:
            VdceOutFormat = R_VDCE_OUT_FORMAT_RGB565;
            break;
        default:
            VdceOutFormat = R_VDCE_OUT_FORMAT_LAST;
            break;
    }
    
    /* Check if we could assign valid values */
    if ( (0 != Flags) || (R_VDCE_OUT_ENDIAN_LAST == VdceOutEndian) ||
                         (R_VDCE_OUT_SWAP_BR_LAST == VdceOutSwap))
    {
        /* Unsupported flags */
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_DISPLAY_OUTPUT_FORMAT_SET, 0);
        return 0; /* R_WM_ERR_DISPLAY_OUTPUT_FORMAT_SET */
    }

    if (R_VDCE_OUT_FORMAT_LAST == VdceOutFormat)
    {
        /* Unsupported color format */
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_DISPLAY_OUTPUT_FORMAT_SET, 0);
        return 0; /* R_WM_ERR_DISPLAY_OUTPUT_FORMAT_SET */
    }
    
    
    
    /* Configure VDCE */
    if(R_VDCE_ERR_OK != R_VDCE_DisplayOutEndianSet(Unit, VdceOutEndian))
    {
        return 0;
    }

    if(R_VDCE_ERR_OK != R_VDCE_DisplayOutSwapBR(Unit, VdceOutSwap))
    {
        return 0;
    }

    if(R_VDCE_ERR_OK != R_VDCE_DisplayOutFormatSet(Unit, VdceOutFormat))
    {
        return 0;
    }

    return 1;
}


uint32_t R_WM_Sys_WindowCreate(uint32_t Unit, const r_wm_Window_t* Win)
{
    return loc_WinReconfAll(Unit);
}

uint32_t R_WM_Sys_WindowDelete(uint32_t Unit, const r_wm_Window_t* Win)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    /* if the layer is enabled, disable the layer */
    if (Win->Status == R_WM_WINSTATUS_ENABLED)
    {
        loc_VDCE_LayerDisable(Unit, win_layer_pair->Layer);
    }
    win_layer_pair->Win = R_NULL;
    win_layer_pair->Layer = LOC_INVALID_LAYER;
    return 1;
}

uint32_t R_WM_Sys_WindowEnable(uint32_t Unit, const r_wm_Window_t* Win, uint32_t Enabled)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }

    if (Enabled != 0)
    {
        /* do not allow enabling the layer out of screen limits */
        if (Win->PosY < 0)
        {
            if ((-Win->PosY) >= Win->Height)
            {
                return 1;
            }
        }
        if (Win->PosX < 0)
        {
            if ((-Win->PosX) >= Win->Width)
            {
                return 1;
            }
        }
        loc_VDCE_LayerEnable(Unit, win_layer_pair->Layer);
    }
    else
    {
        loc_VDCE_LayerDisable(Unit, win_layer_pair->Layer);
    }
    return 1;
}

uint32_t R_WM_Sys_WindowPosSet(uint32_t Unit, const r_wm_Window_t* Win, int32_t PosX, int32_t PosY, int32_t PosZ)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair;
    const void* fb;
    if (Win->PosZ != PosZ)
    {
        /* the Z has changed, the windows need to be reassigned to the layers */
        if (0 == loc_WinReconfAll(Unit))
        {
            return 0;
        }
    }
    win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    if (0 == loc_WinSetViewportAndGeom( Unit,
                                        Win,
                                        win_layer_pair->Layer,
                                        PosX,
                                        PosY,
                                        Win->Pitch,
                                        Win->Width,
                                        Win->Height,
                                        R_WM_ColorFmtBitsPerPixGet(Win->ColorFmt) ))
    {
        return 0;
    }
    /* due to negative offset workaround, the framebuffer start address might need correction */
    if (Win->Mode != R_WM_WINMODE_SPRITES)
    {
        fb = loc_WinGetOnScreenFb(Win);
        if (fb == 0)
        {
            R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
            return 0;
        }
    }
    else
    {
        fb = 0;
    }
    
    if (0 == loc_WinSetFb(Unit, Win, PosX, PosY, fb))
    {
        return 0;
    }
    
    return 1;
}

uint32_t R_WM_Sys_WindowGeomSet (   uint32_t Unit,
                                    const r_wm_Window_t* Win,
                                    uint32_t Pitch,
                                    uint32_t Width,
                                    uint32_t Height
                                )
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    if (win_layer_pair->Layer == LOC_INVALID_LAYER)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_RANGE_WM, 0);
        return 0;
    }
    
    /* The Layer must be disabled for geometry configuration */
    loc_VDCE_LayerDisable(Unit, win_layer_pair->Layer);
    
    if (0 == loc_WinSetViewportAndGeom( Unit,
                                        Win,
                                        win_layer_pair->Layer,
                                        Win->PosX,
                                        Win->PosY,
                                        Pitch,
                                        Width,
                                        Height,
                                        R_WM_ColorFmtBitsPerPixGet(Win->ColorFmt) ))
    {
        return 0;
    }

    /* If the layer was enabled before configuration, enable it again */
    if (Win->Status == R_WM_WINSTATUS_ENABLED)
    {
        loc_VDCE_LayerEnable(Unit, win_layer_pair->Layer);
    }

    return 1;
}

uint32_t R_WM_Sys_WindowColorFmtSet (   uint32_t                Unit,
                                        const r_wm_Window_t*    Win,
                                        r_wm_WinColorFmt_t      ColorFmt
                                    )
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    if (win_layer_pair->Layer == LOC_INVALID_LAYER)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_RANGE_WM, 0);
        return 0;
    }
    
    if ( R_WM_ColorFmtBitsPerPixGet(Win->ColorFmt) != R_WM_ColorFmtBitsPerPixGet(ColorFmt) )
    {
        if (Win->Mode == R_WM_WINMODE_SPRITES)
        {
            if (R_VDCE_ERR_OK != R_VDCE_LayerMemGeometrySet(Unit, win_layer_pair->Layer, Win->Pitch, Win->Height))
            {
                return 0;
            }
        }
    }
    
    /* The Layer must be disabled for color format configuration */
    loc_VDCE_LayerDisable(Unit, win_layer_pair->Layer);
    
    if (R_VDCE_ERR_OK != R_VDCE_LayerFormatSet(Unit, win_layer_pair->Layer, loc_GetVdceFormat(ColorFmt)))
    {
        return 0;
    }
    
    /* If the layer was enabled before configuration, enable it again */
    if (Win->Status == R_WM_WINSTATUS_ENABLED)
    {
        loc_VDCE_LayerEnable(Unit, win_layer_pair->Layer);
    }
    
    return 1;
}

uint32_t R_WM_Sys_SpriteCreate(uint32_t Unit, const r_wm_Sprite_t* Sprite)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Sprite->Window);
    uint8_t spea_unit_ordinal;
    r_spea_Unit_t spea_unit;

    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
    loc_Layer2Spea(Unit, win_layer_pair->Layer, &spea_unit, &spea_unit_ordinal);
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_REASSIGN_BIT+spea_unit_ordinal));

    /* make the sprite initially invisible */
    R_WM_Sys_SpriteEnable(Unit, Sprite, 0);
    return 1;
}

uint32_t R_WM_Sys_SpriteEnable(uint32_t Unit, const r_wm_Sprite_t* Sprite, uint32_t Enabled)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Sprite->Window);
    uint8_t sprite_no = LOC_INVALID_SPRITE;
    r_spea_Unit_t spea_unit;
    uint8_t spea_unit_ordinal;
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }

    loc_Layer2Spea(Unit, win_layer_pair->Layer, &spea_unit, &spea_unit_ordinal);
    sprite_no = loc_FindSpriteOrdinal(Unit, Sprite);
    if (sprite_no == LOC_INVALID_SPRITE)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_RANGE_WM, 0);
        return 0;
    }
    
    if (R_SPEA_ERR_OK != R_SPEA_SpriteEnable(0, spea_unit_ordinal, sprite_no, Enabled))
    {
        return 0;
    }
    
    /* Set respective R_WM_SYS_FRAME_STATUS_SPEA_SUn_UPDATE_BIT */
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_UPDATE_BIT+spea_unit_ordinal));
    return 1;
}

uint32_t R_WM_Sys_SpriteDelete(uint32_t Unit, const r_wm_Sprite_t* Sprite)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Sprite->Window);
    uint8_t spea_unit_ordinal;
    r_spea_Unit_t spea_unit;
    
    loc_Layer2Spea(Unit, win_layer_pair->Layer, &spea_unit, &spea_unit_ordinal);
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_REASSIGN_BIT+spea_unit_ordinal));

    return 1;
}

uint32_t R_WM_Sys_SpriteMove(uint32_t Unit, const r_wm_Sprite_t* Sprite, uint32_t PosX, uint32_t PosY, uint32_t PosZ)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Sprite->Window);
    uint32_t sprite_no = LOC_INVALID_SPRITE;
    
    r_spea_Unit_t spea_unit;
    uint8_t spea_unit_ordinal = 0;
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }

    loc_Layer2Spea(Unit, win_layer_pair->Layer, &spea_unit, &spea_unit_ordinal);
    sprite_no = loc_FindSpriteOrdinal(Unit, Sprite);
    if (sprite_no == LOC_INVALID_SPRITE)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_RANGE_WM, 0);
        return 0;
    }
    
    if (R_SPEA_ERR_OK != R_SPEA_SetSpritePos(0, spea_unit_ordinal, sprite_no, PosX, PosY, R_WM_ColorFmtBitsPerPixGet(Sprite->Window->ColorFmt)))
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_SPEA_INTERNAL, 0);
        return 0;
    }

    /* If z-Pos of the spite has changes, all sprite registers have to be reassigned. */
    if (Sprite->PosZ != PosZ)
    {
        r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_REASSIGN_BIT+spea_unit_ordinal));
    }
    
    /* Set respective R_WM_SYS_FRAME_STATUS_SPEA_SUn_UPDATE_BIT */
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_UPDATE_BIT+spea_unit_ordinal));
    return 1;
}

uint32_t R_WM_Sys_SpriteBufSet(uint32_t Unit, const r_wm_Sprite_t *Sprite, void* Buf)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Sprite->Window);
    r_spea_SpriteConfig_t sprite_conf;
    uint32_t sprite_no = LOC_INVALID_SPRITE;
    
    r_spea_Unit_t spea_unit;
    uint8_t spea_unit_ordinal = 0;
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }

    loc_Layer2Spea(Unit, win_layer_pair->Layer, &spea_unit, &spea_unit_ordinal);
    sprite_no = loc_FindSpriteOrdinal(Unit, Sprite);
    if (sprite_no == LOC_INVALID_SPRITE)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_RANGE_WM, 0);
        return 0;
    }
    
    sprite_conf.PosX = Sprite->PosX;
    sprite_conf.PosY = Sprite->PosY;
    sprite_conf.Width = Sprite->Width;
    sprite_conf.Height = Sprite->Height;
    sprite_conf.SrcAddr = (uint32_t)Buf;
    sprite_conf.VSync = Unit;
    if (Sprite->Status == R_WM_SPRITESTATUS_ENABLED)
    {
        sprite_conf.Enable = 1;
    }
    else
    {
        sprite_conf.Enable = 0;
    }
    sprite_conf.VSync = Unit;
    
    if (R_SPEA_ERR_OK != R_SPEA_SetSprite(0, spea_unit_ordinal, sprite_no, &sprite_conf, R_WM_ColorFmtBitsPerPixGet(Sprite->Window->ColorFmt)))
    {
        return 0;
    }
    
    /* Set respective R_WM_SYS_FRAME_STATUS_SPEA_SUn_UPDATE_BIT */
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_UPDATE_BIT+spea_unit_ordinal));
    return 1;
}

uint32_t R_WM_Sys_WindowAlphaSet(uint32_t Unit, const r_wm_Window_t* Win, uint8_t Alpha)
{
    r_vdce_Error_t err;
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
    if (win_layer_pair->Win->ColorKey.Enabled != 0)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NO_PHYS_WINDOW, 0);
        return 0;
    }
    
    if ((0 < win_layer_pair->Layer) && (win_layer_pair->Layer < 4))
    {
        err = R_VDCE_LayerAlphaConstEnable(Unit, win_layer_pair->Layer, Alpha);
        if (err != R_VDCE_ERR_OK)
        {
            return 0;
        }
    }
    return 1;
}

uint32_t R_WM_Sys_WindowPremultipliedAlphaEnable (uint32_t Unit, const r_wm_Window_t* Win, uint8_t Enabled)
{
    r_vdce_Error_t err;
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
    if ((0 < win_layer_pair->Layer) && (win_layer_pair->Layer < 4))
    {
        /* The Layer must be disabled for premultiplied alpha configuration */
        loc_VDCE_LayerDisable(Unit, win_layer_pair->Layer);
        
        if (R_FALSE != Enabled)
        {
            err = R_VDCE_LayerPremultipliedAlphaEnable(Unit, win_layer_pair->Layer);
        }
        else
        {
            err = R_VDCE_LayerPremultipliedAlphaDisable(Unit, win_layer_pair->Layer);
        }

        if (err != R_VDCE_ERR_OK)
        {
            return 0;
        }
        
        /* If the layer was enabled before configuration, enable it again */
        if (Win->Status == R_WM_WINSTATUS_ENABLED)
        {
            loc_VDCE_LayerEnable(Unit, win_layer_pair->Layer);
        }
    }
    return 1;
}

uint32_t R_WM_Sys_WindowFlagsUpdate (uint32_t Unit, const r_wm_Window_t* Win, r_wm_WinFlags_t SetFlags, r_wm_WinFlags_t ClearFlags)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
    if ( 0 != ( ((uint32_t)SetFlags) & ((uint32_t)ClearFlags) ) )
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_SYS_WIN_FLAG_UPDATE_FAILED, 0);
        return 0;
    }
    
    if (1 == r_wm_DevSys[Unit].HwLayerEnabled[win_layer_pair->Layer])
    {
        if (R_VDCE_ERR_OK != R_VDCE_LayerDisable(Unit, win_layer_pair->Layer))
        {
            return 0;
        }
    }
    
    /* Enable vertical mirroring */
    if ( 0 != ( ((uint32_t)SetFlags) & ((uint32_t)R_WM_WINFLAG_V_MIRROR) ) )
    {
        if (R_VDCE_ERR_OK != R_VDCE_LayerModeSet(Unit, win_layer_pair->Layer, R_VDCE_LAYER_MODE_V_MIRORING) )
        {
            return 0;
        }
    }

    /* Disable vertical mirroring */
    if ( 0 != ( ((uint32_t)ClearFlags) & ((uint32_t)R_WM_WINFLAG_V_MIRROR) ) )
    {
        if (R_VDCE_ERR_OK != R_VDCE_LayerModeSet(Unit, win_layer_pair->Layer, R_VDCE_LAYER_MODE_NONE) )
        {
            return 0;
        }
    }
    
    if (1 == r_wm_DevSys[Unit].HwLayerEnabled[win_layer_pair->Layer])
    {
        if (R_VDCE_ERR_OK != R_VDCE_LayerEnable(Unit, win_layer_pair->Layer))
        {
            return 0;
        }
    }

    return 1;
}

uint32_t R_WM_Sys_WindowClutSet(uint32_t Unit, const r_wm_Window_t* Win, uint32_t NumEntries, const r_wm_ClutEntry_t* Clut)
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
    if (0 == NumEntries)
    {
        return 0;
    }
    else
    {
        r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_LAYER_0_CLUT_UPDATE_BIT+win_layer_pair->Layer));
    }
    
    return 1;
}

uint32_t R_WM_Sys_WindowDeleteAllSprites    (   uint32_t Unit,
                                                const r_wm_Window_t* Win
                                            )
{
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    uint32_t sprite;
    r_spea_Unit_t spea_unit;
    uint8_t spea_unit_ordinal = 0;
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }

    loc_Layer2Spea(Unit, win_layer_pair->Layer, &spea_unit, &spea_unit_ordinal);
    
    for (sprite = 0; sprite < R_VDCE_MAX_SPRITES_PER_LAYER; ++sprite)
    {
        if (R_SPEA_ERR_OK != R_SPEA_SpriteEnable(0, spea_unit_ordinal, sprite, 0))
        {
            return 0;
        }
    }
    
    /* Set respective R_WM_SYS_FRAME_STATUS_SPEA_SUn_UPDATE_BIT */
    r_wm_DevSys[Unit].FrameStatusFlags |= (1uL << (R_WM_SYS_FRAME_STATUS_SPEA_SU0_UPDATE_BIT+spea_unit_ordinal));
    return 1;
}

uint32_t R_WM_Sys_WindowColorKeyEnable(uint32_t Unit, const r_wm_Window_t* Win, uint32_t Enabled)
{
    r_vdce_CkARGB32_t ck_in;
    r_vdce_CkARGB32_t ck_out;
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair = loc_WinLayerPairGet(Unit, Win);
    
    if (win_layer_pair == R_NULL)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
    if (0 == win_layer_pair->Layer)
    {
        /* R_WM_ERR_COLOR_KEYING_NOT_SUPPORTED_FOR_LAYER */
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_COLOR_KEYING_NOT_SUPPORTED_FOR_LAYER, 0);
        return 0;
    }
    
    /* The Layer must be disabled for Colorkey configuration */
    loc_VDCE_LayerDisable(Unit, win_layer_pair->Layer);
    
    if (Enabled != 0)
    {
        ck_in.R = Win->ColorKey.In.RgbKey.Red;
        ck_in.G = Win->ColorKey.In.RgbKey.Green;
        ck_in.B = Win->ColorKey.In.RgbKey.Blue;
        ck_in.A = 0x00;
        
        ck_out.R = Win->ColorKey.Out.Red;
        ck_out.G = Win->ColorKey.Out.Green;
        ck_out.B = Win->ColorKey.Out.Blue;
        ck_out.A = Win->ColorKey.Out.Alpha;
        
        /* Layer Alpha and Chroma Keying are exclusive. 
           Chroma Keying overrides Layer Alpha. */
        if((0 < win_layer_pair->Layer) && (win_layer_pair->Layer < 4))
        {
            if (R_VDCE_ERR_OK != R_VDCE_LayerAlphaConstDisable(Unit, win_layer_pair->Layer))
            {
                return 0;
            }
        }
        if (R_VDCE_ERR_OK != R_VDCE_LayerChromaKeyEnable(Unit, win_layer_pair->Layer, &ck_in, &ck_out))
        {
            return 0;
        }
    }
    else
    {
        if (R_VDCE_ERR_OK != R_VDCE_LayerChromaKeyDisable(Unit, win_layer_pair->Layer))
        {
            return 0;
        }

        /* Layer Alpha and Chroma Keying are exclusive. 
           Layer Alpha can be enabled again. */
        if((0 < win_layer_pair->Layer) && (win_layer_pair->Layer < 4))
        {
            if (R_VDCE_ERR_OK != R_VDCE_LayerAlphaConstEnable(Unit, win_layer_pair->Layer, win_layer_pair->Win->Alpha))
            {
                return 0;
            }
        }
    }
    
    
    /* If the layer was enabled before configuration, enable it again */
    if (Win->Status == R_WM_WINSTATUS_ENABLED)
    {
        loc_VDCE_LayerEnable(Unit, win_layer_pair->Layer);
    }
    
    return 1;
}

static const r_dev_PinConfig_t loc_capture1PinConfig[] =   /* based on loc_D1M2H_ViItuCaputePinConfig */
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    
    /* VDCE1 Input ITU656 (from analog SAA71xx video converter) */
    {43, 2,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In Data7 */
    {43, 3,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {43, 4,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {43, 5,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {43, 6,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, 
    {43, 7,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {43, 8,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {43, 9,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In Data0 */ 
    {43, 10, 2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In CLK */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

static const r_dev_PinConfig_t loc_capture0PinConfig[] =
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* VDCE0 Input ITU656 (from analog SAA71xx video converter) */
/* ITU from SAA71xx video converter */
    {42, 0,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In Data7 */
    {42, 1,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {42, 2,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {42, 3,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {42, 4,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},

    {42, 5,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {42, 6,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1},
    {42, 7,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In Data0 */
    {42, 8,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In CLK */
#if 0 /* not needed for ITU656 */
    {42, 9,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In HS - not needed for ITU656 */
    {42, 10, 2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_SCHMITT1}, /* VDCE1 In VS - not needed for ITU656 */
#endif
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1}
};


uint32_t R_WM_Sys_CaptureCreate(uint32_t Unit, const r_wm_Capture_t* Capt)
{
    uint32_t val;
    r_vdce_Fbformat_t fb_fmt;
    uint32_t fb_bpp;
    r_vdce_Error_t err;
    r_wm_sys_vdce_WinLayerPair_t* win_layer_pair;
    uint32_t should_return;
    
    /*  need to reconf all the windows because the layer assignment needs to start from the layer 0 where the VIN can be output to 
        in case of the same VDCE channel for the VIN and VOUT
        TODO VDCE channels for VIN and VOUT are different
    */
    if (0 == loc_WinReconfAll(Unit))
    {
        return 0;
    }
    
    win_layer_pair = loc_WinLayerPairGet(Unit, Capt->Window);

    if (win_layer_pair == 0)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NULL_PTR_ARGUMENT, 0);
        return 0;
    }
    
#if 0
    /* check if scaling applicable */
    if  (   (win_layer_pair->Layer == R_VDCE_LAYER_SCALER0)
            ||
            (win_layer_pair->Layer == R_VDCE_LAYER_SCALER1)
        )
    {
        /* todo: with "Capt->CapUnit" as parameter only downscaling is allowed */
        if (Capt->ScaledWidth != 0)
        {
            if (0 == R_VDCE_LayerImgScaleX(Capt->CapUnit, win_layer_pair->Layer, Capt->ScaledWidth, R_VDCE_SCALING_SMALLER))
            {
                return 0;
            }
        }
        if (Capt->ScaledHeight != 0)
        {
            if (0 == R_VDCE_LayerImgScaleY(Capt->CapUnit, win_layer_pair->Layer, Capt->ScaledHeight, R_VDCE_SCALING_SMALLER))
            {
                return 0;
            }
        }

    }
#endif
    
    if (((uint32_t)Capt->Mode & (uint32_t)R_WM_CAPMODE_YUV_ITU656) != 0)
    {
        /*  setup the HW for the ITU mode:
            select ITU656 (CLK + DATA7..0) for analog camera connected to SAA71xx
        */
        val = R_DEV_READ_REG(32, VDCECTL);
        if(Capt->CapUnit == 1)
        {
            val &= VDCECTL_VISL1;
        }
        else if(Capt->CapUnit == 0)
        {
            val &= VDCECTL_VISL0;
        }
        else
        {
            /* Not avialable */
        }
        val &= VDCECTL_PXSL;

        R_DEV_WRITE_REG(32, VDCECTL, ((val & 0x3Fu) | 0x10000u));
        if(Capt->CapUnit == 1)
        {
            R_DEV_PinInit(loc_capture1PinConfig);
        }
        else if(Capt->CapUnit == 0)
        {
            R_DEV_PinInit(loc_capture0PinConfig);
        }
        else
        {
            /* Not avialable */
        }
    }
    
    should_return = 0;
    switch (Capt->Window->ColorFmt)
    {
    case R_WM_COLORFMT_RGB0888:
    case R_WM_COLORFMT_ARGB8888:
        fb_fmt = R_VDCE_FB_FORMAT_RGB0888;
        fb_bpp = 4;
        break;
    case R_WM_COLORFMT_RGB565:
        fb_fmt = R_VDCE_FB_FORMAT_RGB565;
        fb_bpp = 2;
        break;
    default:
        should_return = 1;
        break;
    }
    if (should_return != 0)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NO_PHYS_WINDOW, 0);
        return 0;
    }

    err = R_VDCE_CapModeSet(Capt->CapUnit, (r_vdce_CapMode_t)((uint32_t)Capt->Mode & (~0xe0000000u)));    /* get rid of 3 WM internal MSBs that do not map to VDCE opts */
    if (err != R_VDCE_ERR_OK)
    {
        return 0;
    }

    #if 1
    (void)fb_bpp;
    /* BOB Deinterlacing */
    err = R_VDCE_CapBufGeometrySetup    (   Capt->CapUnit,
                                            (uint32_t)(Capt->Window->Surface.Fb.Buffer[0].Data),
                                            0,
                                            Capt->StrideX,
                                            Capt->Width,
                                            Capt->Height,
                                            Capt->StartX,
                                            Capt->StartY1,
                                            fb_fmt
                                        );

    #else
    /* WEAVE Deinterlacing */
    if (Capt->Window->Surface.Fb.BufNum < 2)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_SYS_CAPTURE_CREATE_FAILED, 0);
        return 0;
    }
    err = R_VDCE_CapBufGeometrySetup    (   Capt->CapUnit,
                                            (uint32_t)(Capt->Window->Surface.Fb.Buffer[0].Data),
                                            (uint32_t)(Capt->Window->Surface.Fb.Buffer[0].Data + (Capt->StrideX * fb_bpp) ),
                                            Capt->StrideX * 2,
                                            Capt->Width,
                                            Capt->Height,
                                            Capt->StartX,
                                            Capt->StartY1,
                                            fb_fmt
                                        );
    #endif
    if (err != R_VDCE_ERR_OK)
    {
        return 0;
    }
    
    if (R_VDCE_ERR_OK != R_VDCE_LayerVSyncDelaySet(Unit, win_layer_pair->Layer, Capt->Delay))
    {
        return 0;
    }

    if (((uint32_t)Capt->Mode & (uint32_t)R_WM_CAPMODE_YUV_ITU656) != 0)
    {
        if (R_VDCE_ERR_OK != R_VDCE_LayerMatrixBT601Set(Unit, R_VDCE_LAYER_INPUT))
        {
            return 0;
        }
    }
    
    /* set the interrupt service routines */
    if (R_VDCE_ERR_OK != R_VDCE_IntcCallbackSet(Capt->CapUnit, R_VDCE_INTC_CAP_VBLANK, loc_Isr[Unit][R_WM_EVENT_VI_VBLANK]))
    {
        return 0;
    }
#if 0   /* unused */
    if (R_VDCE_ERR_OK != R_VDCE_IntcCapScanlineSet(Capt->CapUnit, Capt->Height-50))
    {
        return 0;
    }
#endif
    
    return 1;
}

uint32_t R_WM_Sys_CaptureDelete(uint32_t Unit, const r_wm_Capture_t* Capt)
{
    return R_WM_Sys_CaptureEnable(Unit, Capt, 0);
}

uint32_t R_WM_Sys_CaptureEnable(uint32_t Unit, const r_wm_Capture_t* Cap, uint32_t Enabled)
{
    if (Enabled != 0)
    {
        if (0 == (r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_CAPTURE_ACTIVE) )
        {
            if (R_VDCE_ERR_OK != R_VDCE_CapEnable(Cap->CapUnit, Unit))
            {
                return 0;
            }
            if (R_VDCE_ERR_OK != R_VDCE_IntcEnable(Cap->CapUnit, R_VDCE_INTC_CAP_VBLANK))
            {
                return 0;
            }
            r_wm_DevSys[Unit].GlobalStatusFlags |= R_WM_SYS_GLOBAL_STATUS_FLAG_CAPTURE_ACTIVE;
        }
    }
    else
    {
        if (0 != (r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_CAPTURE_ACTIVE) )
        {
            if (R_VDCE_ERR_OK != R_VDCE_IntcDisable(Cap->CapUnit, R_VDCE_INTC_CAP_VBLANK))
            {
                return 0;
            }
            if (R_VDCE_ERR_OK != R_VDCE_CapDisable(Cap->CapUnit))
            {
                return 0;
            }
            r_wm_DevSys[Unit].GlobalStatusFlags &= ~R_WM_SYS_GLOBAL_STATUS_FLAG_CAPTURE_ACTIVE;
        }
    }
    return 1;
}

uint32_t R_WM_Sys_MsgQueueSetup(uint32_t Unit, void* MsgQueueStorage, uint32_t Size)
{
    return (R_CDI_RbSetup(&r_wm_DevSys[Unit].EvQueueRingBuf, MsgQueueStorage, Size, sizeof(r_wm_Msg_t)) == 0);
}

uint32_t R_WM_Sys_MsgQueueRead(uint32_t Unit, r_wm_Msg_t* Msg)
{
    if (R_CDI_RbRead(&r_wm_DevSys[Unit].EvQueueRingBuf, Msg, 1) != 1)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NG, 0);
        return 0;
    }
    return 1;
}

uint32_t R_WM_Sys_MsgQueueWrite(uint32_t Unit, r_wm_Msg_t* Msg)
{
    if (R_CDI_RbWrite(&r_wm_DevSys[Unit].EvQueueRingBuf, (void*)Msg, 1) != 1)
    {
        R_WM_SYS_REPORT_ERROR(Unit, R_WM_ERR_NG, 0);
        return 0;
    }
    return 1;
}

uint32_t R_WM_Sys_DevCountGet(void)
{
    return R_WM_DEV_NUM;
}

uint32_t R_WM_Sys_IsShutdownActive(uint32_t Unit)
{
    if (0 != (r_wm_DevSys[Unit].GlobalStatusFlags & R_WM_SYS_GLOBAL_STATUS_FLAG_SHUTDOWN) )
    {
        return 1;
    }
    return 0;
}

#if 0
r_vdce_CapMode_t loc_GetVdceCapFormat(uint32_t Unit, r_wm_CapMode_t WmCapMode)
{
    switch (WmCapMode)
    {
    case R_WM_CAPMODE_YUV_ITU656:
        return R_VDCE_CAP_MODE_YUV_ITU656
    case R_WM_CAPMODE_YUV_8BIT:
        return R_VDCE_CAP_MODE_YUV_8BIT;
    case R_WM_CAPMODE_YUV_16BIT:
        return R_VDCE_CAP_MODE_YUV_16BIT;
    case R_WM_CAPMODE_RGB_16BPP:
        return R_VDCE_CAP_MODE_RGB_16BPP;
    case R_WM_CAPMODE_RGB_18BPP:
        return R_VDCE_CAP_MODE_RGB_18BPP;
    case R_WM_CAPMODE_RGB_24BPP:
        return R_VDCE_CAP_MODE_RGB_24BPP;
    case R_WM_CAPMODE_DITHER:
        return R_VDCE_CAP_MODE_DITHER;
    case R_WM_CAPMODE_YUV_Y_UV_INVERT:
        return R_VDCE_CAP_MODE_YUV_Y_UV_INVERT;
    case R_WM_CAPMODE_RGB_SYNC_INVERT:
        return R_VDCE_CAP_MODE_RGB_SYNC_INVERT;
    case R_WM_CAPMODE_DATA_CLK_INVERT:
        return R_VDCE_CAP_MODE_DATA_CLK_INVERT;
    case R_WM_CAPMODE_VSYNC_CLK_INVERT:
        return R_VDCE_CAP_MODE_VSYNC_CLK_INVERT;
    case R_WM_CAPMODE_HSYNC_CLK_INVERT:
        return R_VDCE_CAP_MODE_HSYNC_CLK_INVERT;
    case R_WM_CAPMODE_H_MIRRORING:
        return R_VDCE_CAP_MODE_H_MIRRORING;
    case R_WM_CAPMODE_V_MIRRORING:
        return R_WM_CAPMODE_V_MIRRORING;
    default:
        return R_WM_CAPMODE_YUV_ITU656;
    }
}
#endif


