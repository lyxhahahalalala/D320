/*
**************************************************************************** 
PROJECT : JCUA driver
FILE    : $Id: r_jcua_sys.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
JCUA support functions
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
  Title: JCUA Driver support functions

  JCUA (JPEG Codec Unit A) driver support functions

  The generic JCUA driver uses these functions. They have to be implemented 
  within the driver library for a concrete device. (e.g. D1L, D1M, D1H)

  Support functions are functions that are not part of the driver itself 
  but they must be provided to integrate the driver on a particular board.
*/

#ifndef R_JCUA_SYS_H
#define R_JCUA_SYS_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
  Section: Global Functions
*/
/***************************************************************************
  Group: JCUA driver Basic interface functions
*/

/***************************************************************************
  Function: R_JCUA_Sys_Init

  Description:
  This function is called from <R_JCUA_Init> function.

  Parameter:
  Unit  - The parameter specifies the instance number.
  
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_Init(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_DeInit

  Description:
  This function is called from <R_JCUA_DeInit> function.

  Parameter:
  Unit  - The parameter specifies the instance number.
  
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_DeInit(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_InterruptEnable

  Description:
  This function is enable interrupt request.

  Parameter:
  Unit  - The parameter specifies the instance number.
  
  Return value:
  None. 
*/
void R_JCUA_Sys_InterruptEnable(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_InterruptDisable

  Description:
  This function is disable interrupt request.

  Parameter:
  Unit  - The parameter specifies the instance number.
  
  Return value:
  None.
*/
void R_JCUA_Sys_InterruptDisable(const uint32_t Unit);



/***************************************************************************
  Function: R_JCUA_Sys_BaseAddrGet

  Description:
  This function gives back base address of dedicated macro channel.

  Parameter:
  Unit     - The parameter specifies the instance number.

  Return value:
  uint32_t - base address of macro instance (Unit).
*/
uint32_t R_JCUA_Sys_BaseAddrGet(const uint32_t Unit);


/***************************************************************************
  Group: JCUA driver OS interface functions
*/

/***************************************************************************
  Function: R_JCUA_Sys_Lock

  Description:
  Lock the JCUA driver access for the other thread.

  In the environment which is using OS, all drivers should implement the mutex
  or semaphore to this function to prevent re-entry to the driver function at
  the multi-threading.

  Parameter:
  Unit  -  The parameter specifies the instance number.
  
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_Lock(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_Unlock

  Description:
  Unlock the JCUA driver access for the other thread.

  In the environment which is using OS, all drivers should implement the mutex
  or semaphore to this function to prevent re-entry to the driver function at
  the multi-threading.

  Parameter:
  Unit  -  The parameter specifies the instance number.
  
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_Unlock(const uint32_t Unit);

#ifdef __cplusplus
}
#endif

#endif /* R_JCUA_SYS_H */

