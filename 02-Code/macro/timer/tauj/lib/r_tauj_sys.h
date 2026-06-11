/*
****************************************************************************
PROJECT : TAUJ driver
FILE ID : $Id: r_tauj_sys.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for TAUJ macro
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

#ifndef R_TAUJ_SYS_H_
#define R_TAUJ_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Tauj Support Functions

  The generic Tauj driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: R_TAUJ_Sys_PortInit

  Configurates Ports for Tauj.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_tauj_Error_t>
*/
r_tauj_Error_t R_TAUJ_Sys_PortInit(uint32_t Unit);


/***********************************************************
  Function: R_TAUJ_Sys_PortDeInit

  Deinit of Ports for Tauj.

  Sets port pins of dedicated channel back to default value.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_Sys_PortDeInit(uint32_t Unit);


/***********************************************************
  Function: R_TAUJ_Sys_GetBaseAddr

  Gives back base address of dedicated macro channel.

  Parameters:
  Unit       - Instance number

  Returns:
  Macro base address
*/

uint32_t R_TAUJ_Sys_GetBaseAddr(uint32_t Unit);


/***********************************************************
  Function: R_TAUJ_Sys_GetClock

  Gives back clock frequency of dedicated clock input.

  Parameters:
  Unit       - Instance number

  Returns:
  Clock frequency
*/

uint32_t R_TAUJ_Sys_GetClock(uint32_t Unit);




/***********************************************************
  Function: R_TAUJ_Sys_IntCInit

  Init of Tauj Interrupt for dedicated channel

  Parameters:
  Unit       - Instance number

  Returns:
  void
*/

void R_TAUJ_Sys_IntCInit(uint32_t Unit);


/***********************************************************
  Function: R_TAUJ_Sys_SetFilterBypass

  Sets the filter bypass for the required unit

  Parameters:
  Unit         - TAUJ instance number

  Returns:
  0            - TAUJ unit in range
  1            - TAUJ unit out of range
*/

uint32_t R_TAUJ_Sys_SetFilterBypass(uint32_t Unit);


/***********************************************************
  Function: R_TAUJ_Sys_EnableInt

  Enable the interrupt controller for 'Interrupt'.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_tauj_Interrupt_t>

  Returns:
  void
*/

void R_TAUJ_Sys_EnableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt);


/***********************************************************
  Function: R_TAUJ_Sys_DisableInt

  Disable the interrupt controller for 'Interrupt'.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_tauj_Interrupt_t>

  Returns:
  void
*/

void R_TAUJ_Sys_DisableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt);



#ifdef __cplusplus
}
#endif



#endif /* R_TAUJ_SYS_H_ */
