/*
****************************************************************************
PROJECT : Drw2D driver
FILE    : $Id: r_drw2d_priv.h 11853 2015-05-04 11:46:13Z C. Thaler (hh04074) $
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

/***********************************************************
  Title: DRW2D private functions (internal)
  
  DRW2D Main Module Internal Interface

  Only the driver modules shall include this header.
*/

#ifndef R_DRW2D_PRIV_H_
#define R_DRW2D_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Section: Global Types
*/



/***********************************************************
  Type: r_drw2d_UnitApi_t

  Private, per-unit data (API-side).

  Fields:
             Mutex - Used to synchronize access to this unit

  See also:
    <r_drw2d_Units> array variable.
*/
typedef struct r_drw2d_UnitApi_s
{
    r_drw2d_OS_Mutex_t       Mutex;
} r_drw2d_UnitApi_t;





/***********************************************************
  Section: Global Variables
*/


extern r_drw2d_UnitApi_t r_drw2d_UnitApis[R_DRW2D_NUM_UNITS];


/***********************************************************
  Section: Local Functions
*/

/***********************************************************
  Function: drw2d_ErrorHandler

  Handle DRW2D error by calling device context specific error
   handler (if set), or by spinning in an infinite loop.
*/

void drw2d_ErrorHandler (r_drw2d_Unit_t        Unit,
                            r_drw2d_Error_t       Error,
                            r_drw2d_DeviceBase_t *DeviceContextHint
                           );



/***********************************************************
  Function: drw2d_IsValidPixelFormat

  Check whether the given pixel format is valid.

  (note) The pixel format may still be unsupported by the actual graphics driver backend
*/

r_drw2d_Error_t drw2d_IsValidPixelFormat (r_drw2d_PixelFormat_t PixelFormat);



/***********************************************************
  Function: drw2d_IsValidBufferSize

  Check whether the given texture or framebuffer size is supported
*/

r_drw2d_Error_t drw2d_IsValidBufferSize (const r_drw2d_IntSize_t Size);



/***********************************************************
  Function: drw2d_UpdateEffectiveClipRect

  Clip user cliprect against framebuffer size and store in DeviceBase->State.EffectiveClipRect.
*/

void drw2d_UpdateEffectiveClipRect (r_drw2d_DeviceBase_t *DeviceBase);



/***********************************************************
  Function: drw2d_CheckUpdateEffectiveClipRect

  Check <R_DRW2D_API_DIRTY_EFFCLIPRECT> flag and update cliprect if necessary.
*/
R_INLINE void drw2d_CheckUpdateEffectiveClipRect (r_drw2d_DeviceBase_t *DeviceBase);

R_INLINE void drw2d_CheckUpdateEffectiveClipRect (r_drw2d_DeviceBase_t *DeviceBase) {
   if(0 != (DeviceBase->State.APIDirtyFlags & R_DRW2D_API_DIRTY_EFFCLIPRECT))
   {
      drw2d_UpdateEffectiveClipRect(DeviceBase);
      DeviceBase->State.APIDirtyFlags &= (uint16_t) ~R_DRW2D_API_DIRTY_EFFCLIPRECT;
   }
}



/***********************************************************
  Function: drw2d_VtxTransform

  Transform 4D vector(s) by 4x4 vertex matrix and project onto screen if 3D transformation is enabled.
*/
void drw2d_VtxTransform (const r_drw2d_DeviceBase_t *DeviceBase,
                                  r_drw2d_Vec4_t       *Vec,
                                  uint32_t                    NumVertices
                                  );



/***********************************************************
  Function: drw2d_TexTransformByTextureMatrix

  Transform 3D vector by 3x2 matrix (W component is ignored).
*/
r_drw2d_Point_t drw2d_TexTransformByTextureMatrix (const r_drw2d_FixedP_t *Mat,
                                                            const r_drw2d_Vec4_t    Vec
                                                            );


/***********************************************************
  Function: drw2d_FixLen

  Calculate length of given 2D vector
*/
r_drw2d_FixedP_t drw2d_FixLen (r_drw2d_Point_t P);


/***********************************************************
  Function: drw2d_GetDirtyFlagsAvail
*/
uint32_t drw2d_GetDirtyFlagsAvail (r_drw2d_DeviceBase_t *DevBase);

/***********************************************************
  Function: drw2d_GetDirtyFlagsAvailNoConv
*/
uint32_t drw2d_GetDirtyFlagsAvailNoConv (r_drw2d_DeviceBase_t *DevBase);

/***********************************************************
  Function: drw2d_GetTextureEnabled
*/
void drw2d_GetTextureEnabled(uint32_t *Status, r_drw2d_RenderContext_t *RenderCtx);

/***********************************************************
  Function: drw2d_GetTextureCount
*/
uint32_t drw2d_GetTextureCount(r_drw2d_RenderContext_t *RenderCtx);

/***********************************************************
  Function: drw2d_GetTextureCount
*/
uint32_t drw2d_GetTexturePrimary(r_drw2d_RenderContext_t *RenderCtx);


#ifdef __cplusplus
}
#endif

#endif /* R_DRW2D_PRIV_H_  */
