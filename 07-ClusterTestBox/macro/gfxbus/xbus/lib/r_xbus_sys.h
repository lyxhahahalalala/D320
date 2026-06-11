/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_xbus_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic XBUS driver
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

#ifndef R_XBUS_SYS_H_
#define R_XBUS_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic XBUS Support Functions

  The generic XBUS driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_XBUS_SYS_HardwareInit

  Setup the HW part for the generic XBUS.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_xbus_Error_t>
*/

r_xbus_Error_t R_XBUS_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_XBUS_SYS_HardwareDeInit

  Deinit of the HW part for the generic XBUS.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_xbus_Error_t>
*/

r_xbus_Error_t R_XBUS_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_XBUS_SYS_BaseAddr

  Returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_XBUS_SYS_BaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_XBUS_SYS_GetMaxIdx

  Returns the Max index for the cache.
  This value depends on the actual device and determines the amount of available cache
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Max Index 
*/
uint32_t R_XBUS_SYS_GetMaxIdx(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_XBUS_ErrorHandler
  Low level error handler, called in case there is no user error handled was 
  assigned for this macro 
  
  This function will just call R_DEV_ErrrorHandler (avoid having to assing a low level 
  error handler for all the macros)    
    
  Parameters:
  Unit       - Instance number
  Error      - detected error    
  
  Returns:
  void  
*/
void R_SYS_XBUS_ErrorHandler(uint32_t Unit, r_xbus_Error_t Error);  

/*******************************************************************************
  Function: R_XBUS_SYS_BusBusy
  Ckeck if there is a transfer ongoing on the given bus 
  
  Parameters:
  Unit       - XBUS Unit number
  
  Returns:
  == TRUE    - transfer ongoing 
  != TRUE    - no activity  

*/

r_Error_t R_XBUS_SYS_BusBusy(uint32_t Unit); 


#ifdef __cplusplus
}
#endif


#endif /* R_XBUS_SYS_H_ */
