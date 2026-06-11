/*
****************************************************************************
PROJECT : Drw2D driver
FILE    : $Id: r_drw2d_sys.h 7049 2015-12-14 10:21:19Z michael.golczewski $
============================================================================
DESCRIPTION
Driver for DRW2D
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

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

****************************************************************************
*/

#ifndef R_DRW2D_SYS_H_
#define R_DRW2D_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: DRW2D Support Functions for DaveHD (internal)

  It is not recommended to use this API directly.
  Use R_DRW2D_API_* instead.
*/


/***********************************************************
  Section: Global Defines
*/


/***********************************************************
  Constant: R_DRW2D_UNIT_GLOBAL

  Magic unit number used when handling a global error that
   occured in <R_DRW2D_Init> or <R_DRW2D_Exit>.

  See also:
   <R_DRW2D_ErrorHandler>, <R_DRW2D_UNIT_UNKNOWN>
*/
#define R_DRW2D_UNIT_GLOBAL  (0x80008001u)



/***********************************************************
  Constant: R_DRW2D_UNIT_UNKNOWN

  Magic unit number used when handling an error that
   occured when no valid device context was provided 
   by the application and the function called did not
   specify a Unit parameter.

  See also:
   <R_DRW2D_ErrorHandler>, <R_DRW2D_UNIT_GLOBAL>
*/
#define R_DRW2D_UNIT_UNKNOWN  (0x80008002u)



/***********************************************************
  Section: API-side dirty flag defines/constants

  The API-side dirty flags are set and evaluated on the API-side.
*/



/***********************************************************
  Constant: R_DRW2D_API_DIRTY_EFFCLIPRECT

  Indicates that the effective cliprect has to be recalculated.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_API_DIRTY_EFFCLIPRECT           ((uint16_t)(((uint16_t)1u) << ((uint16_t)0)))



/***********************************************************
  Constant: R_DRW2D_API_DIRTY_ALL

  Bitmask that combines all API-side dirty flags.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_API_DIRTY_ALL  ( \
   (R_DRW2D_API_DIRTY_EFFCLIPRECT) \
                                 )


#define R_DRW2D_API_DIRTY_ALL_INIT  (R_DRW2D_API_DIRTY_ALL)




/***********************************************************
  Section: Sys-side dirty flag defines/constants

  The Sys-side dirty flags are usually set on the API-side and evaluated/reset on the Sys-side.
*/



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_CLIPRECT

  Indicates that the cliprect has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_CLIPRECT              1u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_FGCOLOR

  Indicates that the foreground color has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_FGCOLOR               2u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_BGCOLOR

  Indicates that the background resp. fill color has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_BGCOLOR               4u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_FILLMODE

  Indicates that the fill mode has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_FILLMODE              8u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_BLENDMODE

  Indicates that the blending functions have changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_BLENDMODE             16u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_FRAMEBUFFER

  Indicates that the framebuffer (render target) has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_FRAMEBUFFER           32u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_TEXTURE_0

  Indicates that the source texture has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_TEXTURE_0              128u

/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_TEXTURE_0

  Indicates that the source texture has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_TEXTURE_1              256u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_TEXTURE_CACHEXFORM

  Indicates that the static texture mapping (via TextureMatrix) has changed and
   the texture transformation cache needs to be updated.

   (note) UV based texture coordinate transformation is done per primitive
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_TEXTURE_CACHEXFORM    512u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_TEXTURE_STATICMAP

  Indicates that the static texture mapping (via TextureMatrix) has changed.

   (note) UV based texture setup is done per primitive
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_TEXTURE_STATICMAP     1024u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_IMAGEQUALITY

  Indicates that the image quality has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_IMAGEQUALITY          2048u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_CONVKERNEL1D

  Indicates that the 1D convolution kernel (preset) has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_CONVKERNEL1D          4096u



/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_CONVKERNEL2D

  Indicates that the 2D convolution kernel (preset) has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_CONVKERNEL2D          8192u


/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_EFFECTS

  Indicates that the effect stages have changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_EFFECTS               16384u

/***********************************************************
  Constant: R_DRW2D_SYS_DIRTY_CONVKERNEL

  Indicates that the convolution kernel has changed.
  
  See also:
    <r_drw2d_DeviceBase_t>
*/
#define R_DRW2D_SYS_DIRTY_CONVKERNEL            32768u


/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_TEXTURE_STATIC (             \
    R_DRW2D_SYS_DIRTY_TEXTURE_0          |             \
    R_DRW2D_SYS_DIRTY_TEXTURE_CACHEXFORM |             \
    R_DRW2D_SYS_DIRTY_TEXTURE_STATICMAP)               \

/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_INIT (                   \
    R_DRW2D_SYS_DIRTY_CLIPRECT           |             \
    R_DRW2D_SYS_DIRTY_FGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_BGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_FILLMODE           |             \
    R_DRW2D_SYS_DIRTY_BLENDMODE          |             \
    R_DRW2D_SYS_DIRTY_FRAMEBUFFER        |             \
    R_DRW2D_SYS_DIRTY_IMAGEQUALITY       |             \
    R_DRW2D_SYS_DIRTY_CONVKERNEL1D       |             \
    R_DRW2D_SYS_DIRTY_CONVKERNEL2D       |             \
    R_DRW2D_SYS_DIRTY_TEXTURE_STATIC)                  \


/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_NOCONV (                 \
    R_DRW2D_SYS_DIRTY_CLIPRECT           |             \
    R_DRW2D_SYS_DIRTY_FGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_BGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_FILLMODE           |             \
    R_DRW2D_SYS_DIRTY_BLENDMODE          |             \
    R_DRW2D_SYS_DIRTY_FRAMEBUFFER        |             \
    R_DRW2D_SYS_DIRTY_IMAGEQUALITY       |             \
    R_DRW2D_SYS_DIRTY_TEXTURE_STATIC)                  \


/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_NOTEX ( \
    R_DRW2D_SYS_DIRTY_CLIPRECT           |             \
    R_DRW2D_SYS_DIRTY_FGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_BGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_FILLMODE           |             \
    R_DRW2D_SYS_DIRTY_BLENDMODE          |             \
    R_DRW2D_SYS_DIRTY_FRAMEBUFFER        |             \
    R_DRW2D_SYS_DIRTY_IMAGEQUALITY)                     



/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_UV ( \
    R_DRW2D_SYS_DIRTY_CLIPRECT           |             \
    R_DRW2D_SYS_DIRTY_FGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_BGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_FILLMODE           |             \
    R_DRW2D_SYS_DIRTY_BLENDMODE          |             \
    R_DRW2D_SYS_DIRTY_FRAMEBUFFER        |             \
    R_DRW2D_SYS_DIRTY_TEXTURE_0          |             \
    R_DRW2D_SYS_DIRTY_IMAGEQUALITY)                     



/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_CONV (                                \
    R_DRW2D_SYS_DIRTY_CLIPRECT           |             \
    R_DRW2D_SYS_DIRTY_FGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_BGCOLOR            |             \
    R_DRW2D_SYS_DIRTY_FILLMODE           |             \
    R_DRW2D_SYS_DIRTY_BLENDMODE          |             \
    R_DRW2D_SYS_DIRTY_FRAMEBUFFER        |             \
    R_DRW2D_SYS_DIRTY_TEXTURE_0)                          



/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_CONV1D (                              \
    R_DRW2D_SYS_DIRTY_ALL_CONV        |                \
    R_DRW2D_SYS_DIRTY_CONVKERNEL1D)                     



/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_CONV2D (\
    R_DRW2D_SYS_DIRTY_ALL_CONV        |                \
    R_DRW2D_SYS_DIRTY_CONVKERNEL2D)



/* (note) do not add brackets () or GHS MISRA check will fail */
#define R_DRW2D_SYS_DIRTY_ALL_CONV_USER (\
    R_DRW2D_SYS_DIRTY_ALL_CONV        |                \
    R_DRW2D_SYS_DIRTY_CONVKERNEL)





/***********************************************************
  Section: Types 
*/





/***********************************************************
  Section: Variables
*/





/***********************************************************
  Section: DRW2D driver support functions

  The DRW2D API supports different devices with different graphics driver macros. 

  It is also possible to use different macros in the same device. 
*/



/***********************************************************
  Group: DRW2D Basic interface functions
*/

/***********************************************************
  Function: R_DRW2D_Platform_Init

  Description:
    Global initialization of DRW2D DaveHD API.

    Called by <R_DRW2D_Init>.

  Return value:
    See <r_drw2d_Error_t>.
*/

r_drw2d_Error_t R_DRW2D_Platform_Init(void);



/***********************************************************
  Function: R_DRW2D_Platform_Exit

  Description:
    Global shutdown of DRW2D DaveHD API.

    Called by <R_DRW2D_Exit>.

  Return value:
    See <r_drw2d_Error_t>.
*/

r_drw2d_Error_t R_DRW2D_Platform_Exit(void);



/***********************************************************
  Function: R_DRW2D_Platform_Open

  Initialize DaveHD device context.

  Arguments:
            Unit - DRW2D unit number (0..(<R_DRW2D_NUM_UNITS> - 1))
    HwInstanceNr - DaveHD hardware instance nr
    DevCtxSlotNr - Device context slot nr (see "r_sys_drw2d_dhd.c":locDHDDeviceContexts and <r_drw2d_UnitApi_t>.DeviceContexts)
       RetDevice - Returns address of device context base class (allocated during call)

  Returns:
    Error code, see <r_drw2d_Error_t>.

*/
r_drw2d_Error_t R_DRW2D_Platform_Open(r_drw2d_Unit_t        Unit,
                                 uint32_t              HwInstanceNr,
                                 void                 *DeviceInternal,
                                 r_drw2d_DeviceBase_t**RetDevice
                                 );



/***********************************************************
  Function: R_DRW2D_Platform_Close

  Shutdown/clean up DaveHD device context.

  Arguments:
    Device - Address of device context base class

  Returns:
    Error code, see <r_drw2d_Error_t>.

*/
r_drw2d_Error_t R_DRW2D_Platform_Close(r_drw2d_DeviceBase_t *DeviceBase);


/***********************************************************
  Function: R_DRW2D_Platform_RenderContext_Init

  Description:
    Initialize a render context
*/

void R_DRW2D_Platform_RenderContext_Init(r_drw2d_RenderContext_t *RenderContext,
                                    r_drw2d_DeviceBase_t    *DeviceBase
                                    );



/***********************************************************
  Function: R_DRW2D_Platform_DrawRectConvolve

  Description:
    1d or 2d convolution kernel blit

    Called by <R_DRW2D_DrawRectConvolve1dx>, <R_DRW2D_DrawRectConvolve1dy>, <R_DRW2D_DrawRectConvolve2d>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_DrawRectConvolve (r_drw2d_DeviceBase_t *DeviceBase,
                                              const r_drw2d_IntRect_t    *Rect,
                                              uint16_t TextureOffX,
                                              uint16_t TextureOffY
                                                              );


/***********************************************************
  Function: R_DRW2D_Platform_DrawBezierSegment

  Description:
    Render a single rect. Vertices 4 and 5 (E and F) are located halfway between AB resp. CD.

    E and F are used to set up the AB and CD edge interpolators, which may slightly
     deviate from interpolators setup using AB and CD only (=> polygon holes).

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_DrawBezierSegment(r_drw2d_DeviceBase_t *DeviceBase,
                                              const r_drw2d_Vec4_t *Vertices
                                              );




/***********************************************************
  Function: R_DRW2D_Platform_DrawEllipse

  Description:
    Render an ellipse

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_DrawEllipse(r_drw2d_DeviceBase_t *DeviceBase,
                                        r_drw2d_Vec4_t        Center,
                                        r_drw2d_FixedP_t      RadiusX,
                                        r_drw2d_FixedP_t      RadiusY
                                        );



/***********************************************************
  Function: R_DRW2D_Platform_DrawQuad_SharedEdge

  Description:
    Render a single rect. Vertices 4 and 5 (E and F) are located halfway between AB resp. CD.

    E and F are used to set up the AB and CD edge interpolators, which may slightly
     deviate from interpolators setup using AB and CD only (=> polygon holes).

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_DrawQuad_SharedEdge(r_drw2d_DeviceBase_t *DeviceBase,
                                                const r_drw2d_Vec4_t *Vertices,
                                                uint32_t              QuadEdgeFlags
                                                );



/***********************************************************
  Function: R_DRW2D_Platform_DrawQuad

  Description:
    Render a single quad.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_DrawQuad(r_drw2d_DeviceBase_t *DeviceBase,
                                     const r_drw2d_Vec4_t *Vertices,
                                     uint32_t             QuadEdgeFlags
                                    );


/***********************************************************
  Function: R_DRW2D_Platform_DrawTriangle

  Description:
    Render a single triangle.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_DrawTriangle(r_drw2d_DeviceBase_t *DeviceBase,
                                         r_drw2d_Vec4_t        A,
                                         r_drw2d_Vec4_t        B,
                                         r_drw2d_Vec4_t        C,
                                         uint32_t              EdgeFlags /* see R_DRW2D_EDGE_xxx */
                                        );




/***********************************************************
  Function: R_DRW2D_Platform_DrawTriangleUV

  Description:
    Render a single triangle.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_DrawTriangleUV(r_drw2d_DeviceBase_t    *DeviceBase,
                                           r_drw2d_Vec4_t           A,
                                           r_drw2d_Vec4_t           B,
                                           r_drw2d_Vec4_t           C,
                                           uint32_t                 EdgeFlags, /* see R_DRW2D_EDGE_xxx */
                                           const r_drw2d_UVCoord_t *UVCoords
                                          );




/***********************************************************
  Function: R_DRW2D_Platform_BlitTexture

  Description:
    Blit a texture.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_BlitTexture(   r_drw2d_DeviceBase_t    *DeviceBase,
                                                const r_drw2d_Vec4_t    *Vertices,
                                                const r_drw2d_Rect_t    *SrcRect);




/***********************************************************
  Function: R_DRW2D_Platform_FramebufferClear

  Description:
    Clear frame buffer

    Called by <R_DRW2D_FramebufferFree>.

  Return value:
    See <r_drw2d_Error_t>.
*/

r_drw2d_Error_t R_DRW2D_Platform_FramebufferClear(r_drw2d_DeviceBase_t *DeviceBase);




/***********************************************************
  Function: R_DRW2D_Platform_GpuFlush

  Description:
    Flush pending commands to renderer, do not wait.

    Called by <R_DRW2D_GpuFinish>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_GpuFlush(r_drw2d_DeviceBase_t *DeviceBase);



/***********************************************************
  Function: R_DRW2D_Platform_GpuFlushMark

  Description:
    Flush pending commands to renderer, do not wait but mark this render job.
    The "mark" if this render job finished can later be queried by R_DRW2D_GpuFinished.

    Called by <R_DRW2D_GpuFinish>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_GpuFlushMark(r_drw2d_DeviceBase_t *DeviceBase);



/***********************************************************
  Function: R_DRW2D_Platform_GpuFinish

  Description:
    Flush pending commands to renderer and wait until rendering has finished.

    Called by <R_DRW2D_GpuFlush>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_GpuFinish(r_drw2d_DeviceBase_t *DeviceBase);


/***********************************************************
  Function: R_DRW2D_Platform_GpuFinished

  Description:
    Indicate whether the GPU still has pending rendering jobs.
    Can be used for single-threaded non-blocking use cases.

    Called by <R_DRW2D_GpuFinished>.

  Return value:
    Yes or No, see <r_drw2d_Boolean_t>.
*/
r_drw2d_Boolean_t R_DRW2D_Platform_GpuFinished(r_drw2d_DeviceBase_t *DeviceBase);


/***********************************************************
  Function: R_DRW2D_Platform_GpuCmdListCreate

  Description:
    Allocate empty command list.

    Called by <R_DRW2D_GpuCmdListCreate>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_GpuCmdListCreate(r_drw2d_DeviceBase_t *DeviceBase,
                                             r_drw2d_GpuCmdList_t *RetCmdList
                                            );



/***********************************************************
  Function: R_DRW2D_Platform_GpuCmdListGenerate

  Description:
    Record command list by calling a function that invokes render commands.
    Any previously recorded command list data will be discarded.
    The command list must have been created using <R_DRW2D_Platform_GpuCmdListCreate>.

    Called by <GpuCmdListGenerate>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_GpuCmdListGenerate(r_drw2d_DeviceBase_t        *DeviceBase,
                                               r_drw2d_GpuCmdList_t         CmdList,
                                               r_drw2d_GpuCmdListCallback_t Cbk,
                                               void*                        UserData
                                              );



/***********************************************************
  Function: R_DRW2D_Platform_GpuCmdListExec

  Description:
    Execute previously recorded command list.

    Called by <R_DRW2D_GpuCmdListExec>.

  Return value:
    See <r_drw2d_Error_t>.
    */
r_drw2d_Error_t R_DRW2D_Platform_GpuCmdListExec(r_drw2d_DeviceBase_t *DeviceBase,
                                           r_drw2d_GpuCmdList_t CmdList
                                           );



/***********************************************************
  Function: R_DRW2D_Platform_GpuCmdListCopy

  Description:
    Copy command list data to memory area.
    If DestAddr is NULL, Size returns the required size (in bytes) and no command list data is copied.
    If RelocBaseAddr is != NULL, relocate jump commands so that the command list can later be
     executed from the given address (e.g. in flash memory).
    The copied command list may not be re-recorded or deleted using <R_DRW2D_Platform_GpuCmdListDelete>.
    This call is only available in emulation builds (i.e. on the development host).

    Called by <R_DRW2D_GpuCmdListCopy>.

  Return value:
    See <r_drw2d_Error_t>.
    */
r_drw2d_Error_t R_DRW2D_Platform_GpuCmdListCopy(r_drw2d_DeviceBase_t *DeviceBase,
                                           r_drw2d_GpuCmdList_t CmdList,
                                           void *               DestAddr,
                                           uint32_t *           Size,
                                           void *               RelocBaseAddr
                                          );



/***********************************************************
  Function: R_DRW2D_Platform_GpuCmdListDelete

  Description:
    Delete command list.
    The command list must have been created using <R_DRW2D_Platform_GpuCmdListCreate>.

    Called by <R_DRW2D_GpuCmdListDelete>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_GpuCmdListDelete(r_drw2d_DeviceBase_t *DeviceBase,
                                             r_drw2d_GpuCmdList_t  CmdList
                                            );




/***********************************************************
  Function: R_DRW2D_Platform_NativeDriverHandleGet

  Description:
    Get platform-specific low level driver handle

    Called by <R_DRW2D_NativeDriverHandleGet>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_NativeDriverHandleGet(r_drw2d_DeviceBase_t  *DeviceBase,
                                                  void                 **RetNativeDriverHandle
                                                 );



/***********************************************************
  Function: R_DRW2D_Platform_NativeDriverSaveState

  Description:
    Backup platform-specific low level driver state

    Called by <R_DRW2D_NativeDriverBegin>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_NativeDriverSaveState(r_drw2d_DeviceBase_t *DeviceBase);



/***********************************************************
  Function: R_DRW2D_Platform_NativeDriverTryRestoreState

  Description:
    Restore platform-specific low level driver state if it was saved in <R_DRW2D_NativeDriverBegin>.

    Called by <R_DRW2D_NativeDriverEnd>.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_NativeDriverTryRestoreState(r_drw2d_DeviceBase_t *DeviceBase);



/***********************************************************
  Function: R_DRW2D_Platform_PerfCountersAlloc

  Description:
    Allocate hardware performance counters.

    Can fail if another context is already using the counters.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_PerfCountersAlloc(r_drw2d_DeviceBase_t *DeviceBase);



/***********************************************************
  Function: R_DRW2D_Platform_PerfCountersFree

  Description:
    Free hardware performance counters.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_PerfCountersFree(r_drw2d_DeviceBase_t *DeviceBase);



/***********************************************************
  Function: R_DRW2D_Platform_PerfValueGet

  Description:
    Query the driver for HW cycles of type Type and return the value in Result.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_PerfValueGet(r_drw2d_DeviceBase_t  *DeviceBase,
                                         r_drw2d_Performance_t  Type,
                                         uint32_t *             RetValue
                                        );



/***********************************************************
  Function: R_DRW2D_Platform_PerfValueReset

  Description:
    Query the driver for HW cycles of type Type and return the value in Result.

  Return value:
    See <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Platform_PerfValueReset(r_drw2d_DeviceBase_t *DeviceBase,
                                           r_drw2d_Performance_t Type
                                          );







/***********************************************************
  Function: R_DRW2D_Platform_UpdateRenderstates

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/

r_drw2d_Error_t R_DRW2D_Platform_UpdateRenderstates(r_drw2d_DeviceBase_t  *DeviceBase,
                                               uint32_t               ReqFlags
                                              );





#ifdef __cplusplus
}
#endif

#endif /* R_DRW2D_SYS_H_ */
