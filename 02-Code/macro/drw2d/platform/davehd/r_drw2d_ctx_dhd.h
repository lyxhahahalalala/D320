/*
****************************************************************************
PROJECT : Drw2D driver
FILE    : $Id: r_drw2d_ctx_dhd.h 5614 2015-07-31 08:50:52Z florian.zimmermann $
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

#ifndef R_DRW2D_CTX_DHD_H_
#define R_DRW2D_CTX_DHD_H_

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


/*******************************************************************************
  Title: DRW2D device context initialization

  Defines <r_drw2d_DeviceDHD_s> struct and <R_DRW2D_Open> API function.

  An application using DRW2D must include this header file.
*/




/*******************************************************************************
  Section: Types and enumerations
*/



/***********************************************************
  Type: r_drw2d_DeviceDHD_t
  
  Description:
    DaveHD specific device context.


  Fields:
                      Base - Device base structure (see <r_drw2d_DeviceBase_t>)
         DHD_DeviceContext - DaveHD specific device context (see <dhd_init> and <dhd_device_t>) 
                DHD_Handle - Not NULL (points to DHD_DeviceContext) while device is in use.
              PerfCounters - Internal DHD performance counter
                 SaveState - DHD driver state snapshot (see <R_DRW2D_NativeDriverBegin>)
            SaveStateValid - True if driver state is valid
      Clut_NumAllocEntries - Internal entry count for CLUT API usage
      Clut_FirstAllocEntry - Internal alloc status for CLUT API usage
       ConvolveKernelWidth - Current convolution kernel width
      ConvolveKernelHeight - Current convolution kernel height
      ConvolveKernelSigned - Signedness of values in the current convolution kernel.
              MarkedJobGen - Current Job generation used internally in <R_DRW2D_GpuFinished>
               MarkedJobId - Current Job ID used internally in <R_DRW2D_GpuFinished>
                    UVData - Last used UV setup
                   RHWData - Last used RHW setup
                  TexelOrg - Last used texture format
                  AddrMode - Last used address mode
   FramebufferBitsPerPixel - Last used framebuffer bits per pixel
       TextureBitsPerPixel - Last used texture bits per pixel

*/
struct r_drw2d_DeviceDHD_s
{
    r_drw2d_DeviceBase_t      Base;

    dhd_device_t              DHD_DeviceContext;
    dhd_handle_t              DHD_Handle;

    dhd_performance_counter_t PerfCounters[R_DRW2D_PERFCOUNT_DHD_NUM_HWCOUNTERS];

    dhd_state_t              *SaveState;
    r_drw2d_Boolean_t         SaveStateValid;
    dhd_uint32_t              CLUT_NumAllocEntries;
    dhd_uint32_t              CLUT_FirstAllocEntry;
    uint16_t                  ConvolveKernelWidth;
    uint16_t                  ConvolveKernelHeight;
    r_drw2d_Boolean_t         ConvolveKernelSigned;
    dhd_mark_t                JobMarker;
    r_drw2d_Boolean_t         JobMarkerValid;


    struct
    {
        dhd_uv_data_t             UVData[2];
        dhd_rhw_data_t            RHWData[2];
        dhd_texel_org_t           TexelOrg;
        dhd_address_mode_t        AddrMode;
        uint16_t                  FramebufferBitsPerPixel;
        uint16_t                  TextureBitsPerPixel;
    } TempState;
};

typedef struct r_drw2d_DeviceDHD_s r_drw2d_DeviceDHD_t;

#ifdef __cplusplus
}
#endif

#endif /* R_DRW2D_CTX_DHD_H_ */
