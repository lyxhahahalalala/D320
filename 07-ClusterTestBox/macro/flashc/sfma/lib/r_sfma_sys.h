/*
****************************************************************************
PROJECT : SFMA driver
FILE    : $Id: r_sfma_sys.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
SFMA support functions
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
  Title: SFMA Driver support functions

  SFMA (Serial Flash Memory Interface A) driver support functions

  The generic SFMA driver uses these functions. They have to be implemented 
  within the driver library for a concrete device. (e.g. D1L, D1M, D1H)

  Support functions are functions that are not part of the driver itself 
  but they must be provided to integrate the driver on a particular board.
*/

#ifndef R_SFMA_SYS_H
#define R_SFMA_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
  Section: Global Functions
*/
/***************************************************************************
  Group: SFMA driver Basic interface functions
*/

/***************************************************************************
  Function: R_SFMA_Sys_Init

  Description:
  This function is called from <R_SFMA_Init> function.

  The initialization code of environment-depend (e.g. interrupt priority, power control or
  clock control) is implemented by this function.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_Init(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_Sys_DeInit

  Description:
  This function is called from <R_SFMA_DeInit> function.

  The de-initialization code of environment-depend is implemented by this function.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_DeInit(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_Sys_BaseAddrGet

  Description:
  This function gives back base address of dedicated macro.

  Parameter:
  Unit    - The parameter specifies the instance number.

  Return value:
  Macro base address.
*/
uint32_t R_SFMA_Sys_BaseAddrGet(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_Sys_ClockGet

  Description:
  This function gives back clock frequency of dedicated clock input.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  The internal bus clock frequency(Hz).
*/
uint32_t R_SFMA_Sys_ClockGet(const uint32_t Unit);

/***************************************************************************
  Function: R_SFMA_Sys_CalibSetting

  Description:
  This function sets the calibration of the SFMA macro.

  Parameter:
  Unit   - The parameter specifies the instance number.
  Config - This is a pointer to the <r_sfma_Config_t> structure to the
           configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_CalibSetting(const uint32_t Unit, const r_sfma_Config_t * const Config);

/***************************************************************************
  Function:  R_SFMA_Sys_Close(const uint32_t Unit)

  Description:
  This function closes the SFMA in external address space.

  Parameter:
  Unit   - The parameter specifies the instance number.


  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_Close(const uint32_t Unit);

/***************************************************************************
  Function: R_SFMA_Sys_DDREnable

  Description:
  This function returns valid / invalid of the memory access mode (DDR or SDR).
  If you use LSI which is supported DDR mode, this function should return R_TRUE.
  (e.g. in case of D1Mx and D1L2x, this function return R_TRUE.)

  Parameter:
  None.

  Return value:
  R_TRUE or R_FALSE.
*/
uint8_t R_SFMA_Sys_DDREnable(void);

/***************************************************************************
  Function: R_SFMA_Sys_BusWidth8BitEnable

  Description:
  This function returns valid / invalid of the bus width 8bits mode (4bits or 8bits).
  If you use LSI which is supported the bus width 8bits mode, this function should return R_TRUE.
  (e.g. in case of D1Mx and D1L2H, this function return R_TRUE.)

  Parameter:
  None.

  Return value:
  R_TRUE or R_FALSE.
*/
uint8_t R_SFMA_Sys_BusWidth8BitEnable(void);

/***************************************************************************
  Function: R_SFMA_Sys_SyncPeripheral

  Description:
  This function performs synchronization processing to reflect updated
  contents of the control register.

  Parameter:
  None.

  Return value:
  void.
*/
void R_SFMA_Sys_SyncPeripheral(void);

/***************************************************************************
  Group: SFMA driver port interface functions
*/

/***************************************************************************
  Function: R_SFMA_Sys_SpibscPortSet

  Description:
  This function sets the I/O signals of the SFMA.

  Parameter:
  DataBusWidth - The parameter specifies the data bus width.
  Bsz          - The parameter specifies the number of serial flash memories to be connected.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_SpibscPortSet(const uint8_t  DataBusWidth,
                                        const uint32_t Bsz);


/***************************************************************************
  Group: SFMA driver OS interface functions
*/

/***************************************************************************
  Function: R_SFMA_Sys_Lock

  Description:
  This function locks the SFMA driver access to the specified unit for other threads.

  In the environment which is using OS, all drivers should implement the mutex
  or semaphore to this function to prevent re-entry to the driver function at
  the multi-threading.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_Lock(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_Sys_Unlock

  Description:
  This function unlocks the SFMA driver access to the specified unit for other threads.

  In the environment which is using OS, all drivers should implement the mutex
  or semaphore to this function to prevent re-entry to the driver function at
  the multi-threading.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_Unlock(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_Sys_Relax

  Description:
  This function enables to avoid the block of the CPU.
  This function is regularly called during execution of <R_SFMA_DataErase>, 
  <R_SFMA_DataWrite> and <R_SFMA_DataRead>.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Sys_Relax(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_Sys_GetCal

  Description:
  This function retrieves the calibration value from the CKDLY register

  Return value:
  uint32_t - Calibration value
*/
uint32_t R_SFMA_Sys_GetCal(const uint32_t Unit);

#ifdef __cplusplus
}
#endif

#endif /* R_SFMA_SYS_H */

