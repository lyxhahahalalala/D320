/*
****************************************************************************
PROJECT : Drw2D driver
FILE    : $Id: r_sys_drw2d_dhd_internal.h 5614 2015-07-31 08:50:52Z florian.zimmermann $
============================================================================
DESCRIPTION
internal driver for DRW2D
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

#ifndef R_SYS_DRW2D_DHD_INTERNAL_H_
#define R_SYS_DRW2D_DHD_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs__ */

#include <davehd_driver.h>

#if defined (__ghs__)
#pragma ghs endnomisra
#endif /* __ghs__ */


/* Define R_DRW2D_USE_SETUP_EDGE_AA if dhd_setup_edge_aa is to be used to setup the aa edges */
#define R_DRW2D_USE_SETUP_EDGE_AA (defined)
/* Define R_DRW2D_USE_SETUP_EDGE_AA2 if dhd_setup_edge_aa2 is to be used to setup the aa edges */
/* #define R_DRW2D_USE_SETUP_EDGE_AA2 defined */
/* Define R_DRW2D_USE_SETUP_EDGE_AA3 if R_DRW2D_Sys_DHD_Setup_Edge_AA3 is to be used to setup the aa edges */
/* #define R_DRW2D_USE_SETUP_EDGE_AA3 defined */

#ifdef R_DRW2D_USE_SETUP_EDGE_AA
#define R_DRW2D_SYS_DHD_SETUP_EDGE_AA  (dhd_setup_edge_aa)
#elif defined(R_DRW2D_USE_SETUP_EDGE_AA2)
#define R_DRW2D_SYS_DHD_SETUP_EDGE_AA  dhd_setup_edge_aa2
#elif defined(R_DRW2D_USE_SETUP_EDGE_AA3)
#define R_DRW2D_SYS_DHD_SETUP_EDGE_AA  R_DRW2D_Sys_DHD_Setup_Edge_AA3
#endif

/* (note) blits/lines need noaa_notie() setup fxn or thin primitives will disappear at sub-pixel coords */
#define R_DRW2D_SYS_DHD_SETUP_EDGE_NOAA (dhd_setup_edge_noaa) 
/* #define R_DRW2D_SYS_DHD_SETUP_EDGE_NOAA (dhd_setup_edge_noaa_notie) */

/* (note) ellipse need noaa() setup fxn or primitive will have overlapping geometry (blending!) */
#define R_DRW2D_SYS_DHD_SETUP_EDGE_NOAA_ELLIPSE (dhd_setup_edge_noaa) 
/* #define R_DRW2D_SYS_DHD_SETUP_EDGE_NOAA_ELLIPSE (dhd_setup_edge_noaa_notie) */



/***********************************************************
  Type: r_drw2d_BufferDHD_t

  Description:
    DaveHD specific buffer type


  Fields:
    IsAllocated - R_TRUE if buffer is currently allocated
     DHD_Buffer - DaveHD specific buffer structure

*/
typedef struct
{
    r_drw2d_Boolean_t IsAllocated;
    dhd_buffer_t      DHD_Buffer;
} r_drw2d_BufferDHD_t;



/***********************************************************
  Type: r_drw2d_Vect_DHD_t

  Description:
  A 2D point (integer with 4 bit presision).

  Fields:
    X - Horizontal position
    Y - Vertical position

*/
typedef struct
{
    dhd_int32_t X;
    dhd_int32_t Y;
} r_drw2d_Vect_DHD_t;



/***********************************************************
  Section: Macros
*/



/*******************************************************************************
  Macro: R_DRW2D_X_2DHD_4BIT_PREC

  Convert DRW2D (1.15.16) fixed point to DHD 4 bit precision.

  Arguments:
    x - 1.15.16 fixed point value

  Returns:
    int32_t
*/
#define R_DRW2D_X_2DHD_4BIT_PREC(x) ((int32_t)((x) / 4096))



/*******************************************************************************
  Macro: R_DRW2D_I_2DHD_4BIT_PREC

  Convert integer to DHD 4 bit precision.

  Arguments:
    i - integer value

  Returns:
    int32_t
*/
#define R_DRW2D_I_2DHD_4BIT_PREC(i) ((int32_t)((i) * 16))

/***********************************************************
  Section: Functions
*/



/***********************************************************
  Function: R_DRW2D_Sys_DHD_TriangleClipBbox

  Description:
    Clips a triangle to the effective clip rect.
    Shifts out the bounding box of the three points by offset (in integer pixels).

  Returns:
    R_TRUE if successful. R_FALSE if triangle is outside the effective clip rect.

*/
r_drw2d_Boolean_t R_DRW2D_Sys_DHD_TriangleClipBbox (r_drw2d_DeviceBase_t *DeviceBase,
                                                    r_drw2d_Vect_DHD_t   *SrcTriangle,
                                                    r_drw2d_IntRect_t    *DestRect,
                                                    int32_t              *Ystart,
                                                    uint32_t             Offset
                                                    );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_RectClipBbox

  Description:
    Clips a rectangle to the effective clip rect.

  Returns:
    R_TRUE if successful. R_FALSE if rectangle is outside the effective clip rect.

*/
r_drw2d_Boolean_t R_DRW2D_Sys_DHD_RectClipBbox (r_drw2d_DeviceBase_t *DeviceBase,
                                                r_drw2d_Vect_DHD_t   *SrcRect,
                                                r_drw2d_IntRect_t    *DestRect,
                                                int32_t              *Ystart
                                                );





/***********************************************************
  Function: R_DRW2D_Sys_DHD_Cross2dZ

  Description:
    Calc. vector cross product. Used to determine triangle winding order.

  Returns:
    Sign of cross product Z component.

*/
int32_t R_DRW2D_Sys_DHD_Cross2dZ (r_drw2d_Point_t BA, r_drw2d_Point_t CB);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_FlipEdgeFlags

  Description:
    Flip edge flags

  Returns:
    Flipped edge flags

*/
uint32_t R_DRW2D_Sys_DHD_FlipEdgeFlags (uint32_t EdgeFlags);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Framebuffer

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Framebuffer (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                         );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_FillMode

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_FillMode (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                      );


/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Effects

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Effects (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                     );





/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_BlendMode

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_BlendMode (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                       );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Texture

  Description:
    Update texture format/address/RLE/VT/swizzle

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Texture (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx,
    uint32_t TexID
                                                                     );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_StaticMap_CacheXForm

  Description:
    Calculate transformed texture coordinates. Not used for UV based texture setups.

    The transformed coordinates will be stored in the *TexCoordsTrans* field of the device context.

    This is called only when the texture matrix has changed, _NOT_ per primitive.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_CacheXForm (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                                );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_StaticMap

  Description:
    Update u/v interpolators depending on texture matrix. Not used for UV based texture setups.

    This is called only when the texture matrix has changed, _NOT_ per primitive.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_StaticMap (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                               );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_StaticMap_Perspective

  Description:
    Update u/v and rhw interpolators when perspective texture mapping is enabled.

    Since the z component depends on the geometry being drawn, this fxn is called
    each time a primitive is rendered.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_StaticMap_Perspective (
          r_drw2d_DeviceDHD_t     *DevDHD,
          r_drw2d_RenderContext_t *RenderCtx,
    const r_drw2d_Vec4_t          *V0,
    const r_drw2d_Vec4_t          *V1,
    const r_drw2d_Vec4_t          *V2,
          r_drw2d_Boolean_t       *RetIsVisible
                                                                                           );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_UV

  Description:
    Update u/v interpolators when UV-based texture mapping is enabled.
    Since the interpolator setup depends on the geometry being drawn, this fxn is called
    each time a primitive is rendered.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_UV(
          r_drw2d_DeviceDHD_t     *DevDHD,
          r_drw2d_RenderContext_t *RenderCtx,
    const r_drw2d_Vec4_t          *V0,
    const r_drw2d_Vec4_t          *V1,
    const r_drw2d_Vec4_t          *V2,
    const r_drw2d_UVCoord_t       *UVCoords
                                                                       );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_UV_Perspective

  Description:
    Update u/v and rhw interpolators when perspective texture mapping is enabled.

    Since the z component depends on the geometry being drawn, this fxn is called
    each time a primitive is rendered.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Texture_UV_Perspective (
          r_drw2d_DeviceDHD_t     *DevDHD,
          r_drw2d_RenderContext_t *RenderCtx,
    const r_drw2d_Vec4_t          *V0,
    const r_drw2d_Vec4_t          *V1,
    const r_drw2d_Vec4_t          *V2,
    const r_drw2d_UVCoord_t       *UVCoords,
          r_drw2d_Boolean_t       *RetIsVisible
                                                                                    );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Convolve1d

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Convolve1d (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                        );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Convolve2d

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Convolve2d (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                        );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_UpdateRenderstates_Convolve

  Description:
    Update render states according to dirty flags (see <r_drw2d_DeviceBase_t>:CtxDirtyFlags)
     and required flags.

  Return value:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_UpdateRenderstates_Convolve (
    r_drw2d_DeviceDHD_t     *DevDHD,
    r_drw2d_RenderContext_t *RenderCtx
                                                                        );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_Setup_Edge_AA3

  Set up antialiased edge.

  Compared to dhd_setup_edge_aa(), the antialiased pixels are shifted
   towards the polygon center.

  Arguments:
    Device - Address of device context base class

  Returns:
    Error code, see <r_drw2d_Error_t>.

*/
void R_DRW2D_Sys_DHD_Setup_Edge_AA3 (dhd_edge_data_t *Dst,
                                              dhd_int32_t X1, dhd_int32_t Y1,
                                              dhd_int32_t X2, dhd_int32_t Y2,
                                              dhd_int32_t XO, dhd_int32_t YO,
                                              dhd_float32_t Blur
                                              );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_Setup_Edge_AA3

  Set up antialiased edge.

  Compared to dhd_setup_edge_aa(), the antialiased pixels are shifted
   towards the polygon center.

  Arguments:
    Device - Address of device context base class

  Returns:
    Error code, see <r_drw2d_Error_t>.

*/
void R_DRW2D_Sys_DHD_Setup_Striping (r_drw2d_DeviceDHD_t *DevDHD, uint16_t PosX);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_AllocCLUT

  Allocate the given number of color lookup table entries.

  Returns:
    Error code, see <r_drw2d_Error_t>.
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_AllocCLUT (r_drw2d_DeviceDHD_t *DevDHD, uint32_t NumEntries);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_FreeCLUT
*/
void R_DRW2D_Sys_DHD_FreeCLUT (r_drw2d_DeviceDHD_t *DevDHD);

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge0

  Calculates edge control code given the parameters for edge 0.

  Returns:
    Edge control code.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge0(dhd_uint32_t X);

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge1

  Calculates edge control code given the parameters for edge 1.

  Returns:
    Edge control code.
*/

R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge1(dhd_uint32_t X);

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge2

  Calculates edge control code given the parameters for edge 2.

  Returns:
    Edge control code.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge2(dhd_uint32_t X);


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge3

  Calculates edge control code given the parameters for edge 3.

  Returns:
    Edge control code.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge3(dhd_uint32_t X);


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge4

  Calculates edge control code given the parameters for edge 4.

  Returns:
    Edge control code.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge4(dhd_uint32_t X);

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Draw_uv

  Utility function to build draw attribute flags (UV Part).

  Returns:
    Updated attribute flags.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Draw_uv(dhd_uint32_t X);

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Draw_rhw

  Utility function to build draw attribute flags (RHW Part).

  Returns:
    Updated attribute flags.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Draw_rhw(dhd_uint32_t X);

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Draw_col

  Utility function to build draw attribute flags (COL Part).

  Returns:
    Updated attribute flags.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Draw_col(dhd_uint32_t X);




/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge0

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge0(dhd_uint32_t X)
{
    return DHD_EDGE0(X);
}


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge1

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge1(dhd_uint32_t X)
{
    return DHD_EDGE1(X);
}


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge2

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge2(dhd_uint32_t X)
{
    return DHD_EDGE2(X);
}


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge3

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge3(dhd_uint32_t X)
{
    return DHD_EDGE3(X);
}

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Edge4

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Edge4(dhd_uint32_t X)
{
    return DHD_EDGE4(X);
}

/***********************************************************
  Function: R_DRW2D_Sys_DHD_Draw_uv

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Draw_uv(dhd_uint32_t X)
{
    return DHD_DRAW_UV(X);
}


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Draw_rhw

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Draw_rhw(dhd_uint32_t X)
{
    return DHD_DRAW_RHW(X);
}


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Draw_col

  For details see function declaration.
*/
R_INLINE dhd_uint32_t R_DRW2D_Sys_DHD_Draw_col(dhd_uint32_t X)
{
    return DHD_DRAW_COL(X);
}


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Texture_enabled

  For details see function declaration.
*/
void R_DRW2D_Sys_DHD_Texture_enabled(dhd_uint32_t *Status, r_drw2d_RenderContext_t *RenderCtx);


/***********************************************************
  Function: R_DRW2D_Sys_DHD_Texture_count

  For details see function declaration.
*/
dhd_uint32_t R_DRW2D_Sys_DHD_Texture_count(r_drw2d_RenderContext_t *RenderCtx);


/***********************************************************
  Function: R_DRW2D_Sys_DHD_GradientAndAlpha

  Description:
     Performs a parameter check of effect stage and calculates the dhd limiter values
     needed for the gradient.

  Parameters:
     DevDHD - Device handle (see <r_drw2d_DeviceDHD_t>)
     x0 - x-value of dhd primitive bounding box.
     y0 - y-value of dhd primitive bounding box.
     effect_Stage - Effect Stage. Should be a gradient.
     alpha        - constant alpha
  Returns:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)
*/

r_drw2d_Error_t R_DRW2D_Sys_DHD_GradientAndAlpha(r_drw2d_DeviceDHD_t*   DevDHD,
                                                  int32_t                X0,
                                                  int32_t                Y0,
                                                  r_drw2d_EffectStage_t* Effect_Stage,
                                                  r_drw2d_FixedP_t       Alpha
                                                  );



/***********************************************************
  Function: R_DRW2D_Sys_DHD_SetupGradientAndAlpha

  Description:
     Scans the Effect stages of the render context and sets up the
     gradient and constant alpha.


  Parameters:
     DevDHD - Device handle (see <r_drw2d_DeviceDHD_t>)
     renderCtx - Current Render Context.
     x0 - x-value of dhd primitive bounding box.
     y0 - y-value of dhd primitive bounding box.
     dhdEdgeControl - Buffer to receives dhd edge control information.

  Returns:
    <R_DRW2D_ERR_OK> or error code (see <r_drw2d_Error_t>)

*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_SetupGradientAndAlpha(r_drw2d_DeviceDHD_t     *DevDHD,
                                                      r_drw2d_RenderContext_t *RenderCtx,
                                                      int32_t                  X0,
                                                      int32_t                  Y0,
                                                      dhd_uint32_t            *DhdEdgeControl
                                                      );


/***********************************************************
  Function: R_DRW2D_Sys_DHD_GetFramebufferBitsPerPixel

  Description:
    Map framebuffer pixel format to bits per pixel.

  Return value:
    Bits per pixel. 0 if format is not supported.
*/

uint16_t R_DRW2D_Sys_DHD_GetFramebufferBitsPerPixel (r_drw2d_PixelFormat_t PixelFormat);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_GetTextureBitsPerPixel

  Description:
    Map texture pixel format to bits per pixel.

  Return value:
    Bits per pixel. 0 if format is not supported.
*/

uint16_t R_DRW2D_Sys_DHD_GetTextureBitsPerPixel (r_drw2d_PixelFormat_t PixelFormat);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_PixelFormatToDHDPixelOrg

  Description:
    Map DRW2D pixel format to DHD pixel org (framebuffer format).
*/
r_drw2d_Error_t R_DRW2D_Sys_DHD_PixelFormatToDHDPixelOrg (r_drw2d_PixelFormat_t PixelFormat, dhd_pixel_org_t *DstDhdPixelOrg);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_PixelFormatToDHDTexelOrg

  Description:
    Map DRW2D pixel format to DHD texel org (texture format).
*/

r_drw2d_Error_t R_DRW2D_Sys_DHD_PixelFormatToDHDTexelOrg (r_drw2d_PixelFormat_t PixelFormat, dhd_texel_org_t *DstDhdTexelOrg);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_PixelFormatHasAlpha

  Description:
    Checks if a DRW2D pixel format has an alpha value.
*/

r_drw2d_Boolean_t R_DRW2D_Sys_DHD_PixelFormatHasAlpha (r_drw2d_PixelFormat_t PixelFormat);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_ColorToFillPattern

  Description:
    Convert ARGB32 color to fill pattern (for <dhd_fill> call).

  Arguments:
    PixelFormat - Framebuffer/target pixel format
          Color - Fill color (packed ARGB32 format)

  Return value:
    32bit fill pattern.
*/

uint32_t R_DRW2D_Sys_DHD_ColorToFillPattern (r_drw2d_PixelFormat_t PixelFormat,
                                             r_drw2d_Color_t       Color);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_BlendFactorToDHDBlendFactor

  Description:
    Map DRW2D blend factor to DHD blend factor.

  Return value:
    R_FALSE if DstBlendFact is NULL or SrcBlendFactor is not supported, else R_TRUE.
*/

r_drw2d_Boolean_t R_DRW2D_Sys_DHD_BlendFactorToDHDBlendFactor (r_drw2d_BlendFactor_t SrcBlendFactor, dhd_blend_factor_t *DstBlendFact);



/***********************************************************
  Function: R_DRW2D_Sys_DHD_AlphaPremultiplyRGB

  Description:
    Checks if the source and destination have to be premultiplied with alpha before blending.
*/

r_drw2d_Boolean_t R_DRW2D_Sys_DHD_AlphaPremultiplyRGB (r_drw2d_BlendMode_t BlendMode);








#endif /* R_SYS_DRW2D_DHD_INTERNAL_H_ */
