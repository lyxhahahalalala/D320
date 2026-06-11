/*
****************************************************************************
PROJECT : VOWE driver
FILE    : $Id: r_vowe_sys.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
VOWE(Video Output Warping Engine) support functions
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

/***************************************************************************
  Title: VOWE Driver Support Functions

  VOWE (Video Output Warping Engine) driver driver support functions

  The generic VOWE driver uses these functions. They have to be implemented 
  within the driver library for a concrete device. (e.g. D1L, D1M, D1H)

  Support functions are functions that are not part of the driver itself 
  but they must be provided to integrate the driver on a particular board.
*/

#ifndef R_VOWE_SYS_H_
#define R_VOWE_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif



/***************************************************************************
  Section: Global defines
*/
/***************************************************************************
  Group: Constants
*/

/***************************************************************************
  Constants: Source height margin

  This definition specifies the margin of source height.

  Values:
  R_VOWE_SOURCE_HEIGHT_MARGIN  - This definition specifies the margin of source height.

*/
#define R_VOWE_SOURCE_HEIGHT_MARGIN (4)


/***************************************************************************
  Section: Global Functions
*/
/***************************************************************************
  Group: VOWE driver Basic interface functions
*/
/***************************************************************************
  Function: R_VOWE_Sys_Init

  Description:
  This function is called from <R_VOWE_Init> function.

  The initialization code of environment-depend (e.g. interrupt priority, power control and
  clock control) is implemented by this function

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_PARAM_INCORRECT - A parameter provided to a function is incorrect.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Sys_Init(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Sys_DeInit

  Description:
  This function is called from <R_VOWE_DeInit> function.

  The de-initialization code of environment-depend is implemented by this function.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_PARAM_INCORRECT - A parameter provided to a function is incorrect.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Sys_DeInit(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Sys_InterruptEnable

  Description:
  This function enables the interrupt.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  none
*/
void R_VOWE_Sys_InterruptEnable(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Sys_InterruptDisable

  Description:
  This function disables the interrupt.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  none
*/
void R_VOWE_Sys_InterruptDisable(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Sys_BaseAddrGet

  Description:
  This function gives back base address of dedicated unit.

  If the Unit is spcified invalid unit number, this function return zero.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  Macro base address.
*/
uint32_t R_VOWE_Sys_BaseAddrGet(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Sys_ExtBaseAddrGet

  Description:
  This function gives back extra base address of dedicated unit. 

  If the Unit is spcified invalid unit number, this function return zero.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  Macro extra base address.
*/
uint32_t R_VOWE_Sys_ExtBaseAddrGet(const uint32_t Unit);


/***************************************************************************
  Group: VOWE driver VDCE interface functions
*/
/***************************************************************************
  Function: R_VOWE_Sys_VDCEnable

  Description:
  This function make to enables the VDCE's feature for VOWE driver and allocates 
  the VDCE's resources for VOWE driver.

  Parameter:
  Unit            - The parameter specifies the instance number.
  WorkBufferMode  - The parameter specifies the work buffer mode. See <r_vowe_BufferMode_t>
  VdceBufferAddr  - The is a pointer to the top address of the work buffer to set to VDCE.
  VdceBufferSize  - The parameter specifies the size of work bufffer which specified by
                    VdceBufferAddr.
  Stride          - The parameter specifies the stride of output image.
  ColorFormat     - The parameter specifies the color format of output image.
                    See <r_vowe_ColorFormat_t>.
  SourceWidth     - The parameter specifies the source image width (pixel).
  SourceHeight    - The parameter specifies the source image height (pixel).
  RingBufferDelay - The parameter specifies the delay line of ring buffer. 
  DestMode        - The parameter specifies the destination mode. 

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_SYS_VDCE        - The error at the SYS function of VDC.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Sys_VDCEnable(const uint32_t               Unit,
                                    const r_vowe_BufferMode_t    WorkBufferMode,
                                    const uint8_t        * const VdceBufferAddr,
                                    const uint32_t               VdceBufferSize,
                                    const uint32_t               Stride,
                                    const r_vowe_ColorFormat_t   ColorFormat,
                                    const uint32_t               SourceWidth,
                                    const uint32_t               SourceHeight,
                                    const uint32_t               RingBufferDelay,
                                    const r_vowe_DestMode_t      DestMode);


/***************************************************************************
  Function: R_VOWE_Sys_VDCDisable

  Description:
  This function makes to disables the VDCE's feature for VOWE driver and releases
  the VDCE's resources for VOWE driver.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_SYS_VDC         - The error at the SYS function of VDC.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Sys_VDCDisable(const uint32_t Unit);


/***************************************************************************
  Group: VOWE driver OS interface functions
*/
/***************************************************************************
  Function: R_VOWE_Sys_Lock

  Description:
  This function locks the VOWE driver access to the specified unit for other threads.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Sys_Lock(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Sys_Unlock

  Description:
  This function unlocks the VOWE driver access to the specified unit for other threads.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Sys_Unlock(const uint32_t Unit);


#ifdef __cplusplus
}
#endif


#endif /* R_VOWE_SYS_H_ */

