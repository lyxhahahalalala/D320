/*
****************************************************************************
PROJECT : Sprite engine
FILE    : $Id: r_spea_sys.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
SPEA support functions
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2015
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

#ifndef R_SPEA_SYS_H_
#define R_SPEA_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: SPEA Support Functions

  The generic SPEA driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_SPEA_SYS_HardwareInit

  Setup the HW for the generic SPEA.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SPEA_SYS_HardwareDeInit

  Deinit of the HW for the generic SPEA.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SPEA_SYS_BaseAddr

  returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SPEA_SYS_BaseAddr(uint32_t Unit);

/*******************************************************************************
  Function: R_SPEA_SYS_ErrorHandler
  Low level error handler, called in case there is no user error handled was 
  assigned for this macro 
  
  This function will just call R_DEV_ErrrorHandler (avoid having to assing 
  a dedicated low level error handler for all the macros)    
    
  Parameters:
  Unit       - Instance number
  Error      - detected error    
  
  Returns:
  void  
*/
void R_SPEA_SYS_ErrorHandler(uint32_t Unit, r_spea_Error_t Error);  


#ifdef __cplusplus
}
#endif


#endif /* R_SPEA_SYS_H_ */
