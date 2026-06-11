/*
****************************************************************************
PROJECT : OSTM driver
FILE ID : $Id: r_ostm_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for OSTM macro
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

#ifndef R_OSTM_SYS_H_
#define R_OSTM_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: OSTM Support Functions

  The generic OSTM driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_SYS_OSTM_PortInit

  Configurates Ports for UART communication. 
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_ostm_Error_t>
*/
r_ostm_Error_t R_SYS_OSTM_PortInit(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_OSTM_PortDeInit

  Deinit of uart pins.
  
  Sets port pins of dedicated channel back to default value.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_ostm_Error_t>
*/

/* r_ostm_Error_t R_SYS_OSTM_PortDeInit(uint32_t Unit); */

/*******************************************************************************
  Function: R_SYS_OSTM_GetBaseAddr

  Gives back base address of dedicated macro channel.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_OSTM_GetBaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_OSTM_GetClock

  Gives back clock frequency of dedicated clock input. 
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Clock frequency
*/

uint32_t R_SYS_OSTM_GetClock(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_OSTM_IntCInit

  Init of UART Interrupt for dedicated channel
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SYS_OSTM_IntCInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_OSTM_EnableInt

  Enable the interrupt controller for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ostm_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_OSTM_EnableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SYS_OSTM_DisableInt

  Disable the interrupt controller for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ostm_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_OSTM_DisableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SYS_OSTM_SetClock

  Sets the clock for the selected Ostm unit

  Parameters:
  Unit          Ostm unit
  uint8_t       ID
  
  Returns:
  void

*/
void R_SYS_OSTM_SetClock(uint32_t Unit, uint8_t ID);



#ifdef __cplusplus
}
#endif


#endif /* R_OSTM_SYS_H_ */
