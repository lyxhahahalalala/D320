/*
****************************************************************************
PROJECT : DRW2D
FILE    : $Id: r_config_drw2d.h 3678 2014-08-29 10:52:42Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for DRW2D macro
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

#ifndef R_CONFIG_DRW2D_H_
#define R_CONFIG_DRW2D_H_



/***********************************************************
  Title: config_drw2d.h
  
  DRW2D configuration
*/



/***********************************************************
  Section: Includes
************************************************************/





/***********************************************************
  Section: Global Defines
************************************************************/



/***********************************************************
  Constant: R_DRW2D_UNIT_*

  Enumeration of DRW2D units.


  DRW2D units:
  R_DRW2D_UNIT_DHD0      - 0:  First DaveHD instance (#0)
*/

#define R_DRW2D_UNIT_DHD0     0u



/***********************************************************
  Constant: R_DRW2D_NUM_UNITS

  Number of DRW2D devices. see <R_DRW2D_DEV_*>.

*/

#define R_DRW2D_NUM_UNITS     1u



/***********************************************************
  Constant: R_DRW2D_MAX_BUFFER_WIDTH

  Maximum framebuffer/texture width
*/

#define R_DRW2D_MAX_BUFFER_WIDTH  (4095u)



/***********************************************************
  Constant: R_DRW2D_MAX_BUFFER_HEIGHT

  Maximum framebuffer/texture height
*/

#define R_DRW2D_MAX_BUFFER_HEIGHT  (4095u)



/***********************************************************
  Constant: R_DRW2D_PERFCOUNT_DHD_NUM_HWCOUNTERS

  Number of DaveHD hardware performance counters used by DRW2D
   (currently up to 2 on Renesas board)
*/
#ifdef R_DRW2D_OS_WIN32
#define R_DRW2D_PERFCOUNT_DHD_NUM_HWCOUNTERS  (3u)
#else
#define R_DRW2D_PERFCOUNT_DHD_NUM_HWCOUNTERS  (4u)
#endif /* R_DRW2D_OS_WIN32 */



/***********************************************************
  Constant: R_DRW2D_PERFCOUNT_TRACE_CLEAR

  If defined, trace number of pixels cleared with R_DRW2D_FramebufferClear().
*/
#define R_DRW2D_PERFCOUNT_TRACE_CLEAR (defined)



/***********************************************************
  Constant: R_DRW2D_DHD_SWIZZLE_MODE

  Selects DaveHD texture swizzle mode (4x4, 16x16, full swizzle).

   Options:
     E_DHD_ACCESS_SWIZZLE_4    - 4x4 block swizzle [default]
     E_DHD_ACCESS_SWIZZLE_16   - 16x16 block swizzle
     E_DHD_ACCESS_SWIZZLE_FULL - Full swizzle (requires quadratic, power of two texture)
*/
#define R_DRW2D_DHD_SWIZZLE_MODE  (E_DHD_ACCESS_SWIZZLE_4)



/***********************************************************
  Constant: R_DRW2D_DHD_VT_BPP_1

  Selects DaveHD virtual tiling split mode for one byte per pixel texture formats.

   (note) The default DaveHD cacheline size is 128 bytes

   Options:
     E_DHD_ACCESS_VIRTUAL_TILED_2 - 2 splits per cacheline (64x2 block size)
     E_DHD_ACCESS_VIRTUAL_TILED_4 - 4 splits per cacheline (32x4 block size)
     E_DHD_ACCESS_VIRTUAL_TILED_8 - 8 splits per cacheline (16x8 block size) [default]
     E_DHD_ACCESS_VIRTUAL_TILED_16 - 16 splits per cacheline (8x16 block size)

*/
#define R_DRW2D_DHD_VT_BPP_1  (E_DHD_ACCESS_VIRTUAL_TILED_8)



/***********************************************************
  Constant: R_DRW2D_DHD_VT_BPP_2

  Selects DaveHD virtual tiling split mode for two bytes per pixel texture formats.

   (note) The default DaveHD cacheline size is 128 bytes

   Options:
     E_DHD_ACCESS_VIRTUAL_TILED_2 - 2 splits per cacheline (32x2 block size)
     E_DHD_ACCESS_VIRTUAL_TILED_4 - 4 splits per cacheline (16x4 block size)
     E_DHD_ACCESS_VIRTUAL_TILED_8 - 8 splits per cacheline (8x8 block size) [default]
     E_DHD_ACCESS_VIRTUAL_TILED_16 - 16 splits per cacheline (4x16 block size)

*/
#define R_DRW2D_DHD_VT_BPP_2  (E_DHD_ACCESS_VIRTUAL_TILED_8)



/***********************************************************
  Constant: R_DRW2D_DHD_VT_BPP_4

  Selects DaveHD virtual tiling split mode for four bytes per pixel texture formats.

   (note) The default DaveHD cacheline size is 128 bytes

   Options:
     E_DHD_ACCESS_VIRTUAL_TILED_2 - 2 splits per cacheline (16x2 block size)
     E_DHD_ACCESS_VIRTUAL_TILED_4 - 4 splits per cacheline (8x4 block size) [default]
     E_DHD_ACCESS_VIRTUAL_TILED_8 - 8 splits per cacheline (4x8 block size)
     E_DHD_ACCESS_VIRTUAL_TILED_16 - 16 splits per cacheline (2x16 block size)

*/
#define R_DRW2D_DHD_VT_BPP_4  (E_DHD_ACCESS_VIRTUAL_TILED_4)



#endif /* R_CONFIG_DRW2D_H_ */

