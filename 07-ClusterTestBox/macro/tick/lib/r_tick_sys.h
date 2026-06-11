/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_tick_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic TICK driver
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

#ifndef R_TICK_SYS_H_
#define R_TICK_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic TICK Support Functions

  The generic TICK driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_SYS_TICK_HardwareInit

  Setup the HW counter for the generic TICK.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_tick_Error_t>
*/

r_tick_Error_t R_SYS_TICK_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_TICK_HardwareDeInit

  Deinit of the HW counter for the generic TICK.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_tick_Error_t>
*/

r_tick_Error_t R_SYS_TICK_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_TICK_IntcInit

  Init of TICK Interrupt for dedicated channel
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SYS_TICK_IntcInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_TICK_EnableInt

  Enable the interrupt controller.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SYS_TICK_EnableInt(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_TICK_DisableInt

  Disable the interrupt controller.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SYS_TICK_DisableInt(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_TICK_GetTimeUS

  Return time in us.

  The function gets the current tick driver's time in ms.
  It has to add the already elapsed us ticks to that time
  and return the result in us.

  Parameters:
  Unit       - Instance number
  TimeMS     - Tick driver time in ms

  Returns:
  Total time in us
*/

uint32_t R_SYS_TICK_GetTimeUS(uint32_t Unit, uint32_t TimeMS);


#ifdef __cplusplus
}
#endif


#endif /* R_TICK_SYS_H_ */
