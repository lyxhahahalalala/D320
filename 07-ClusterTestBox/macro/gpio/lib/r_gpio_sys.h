/*
****************************************************************************
PROJECT : GPIO driver
FILE ID : $Id: r_gpio_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for GPIO macro
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

#ifndef R_GPIO_SYS_H_
#define R_GPIO_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: GPIO Support Functions

  The generic GPIO driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_SYS_GPIO_GetPin

  This is the device specific implemetation of the R_GPIO_PinRead
  function 

  Parameters:

    Port       - Port number 
    Pin        - Pin number  

  Returns:
    = 1      - Pin is HIGH
    = 0      - Pin is LOW
*/

 uint8_t R_SYS_GPIO_GetPin(int32_t Port, uint8_t Pin);

/*******************************************************************************
  Function: R_SYS_GPIO_SetPin

  This is the device specific implemetation of the R_GPIO_WritePin
  function 

  Parameters:

  Port       - Port number 
  Pin        - Pin  number 
  Value      - 1: Set output HIGH, 0: Set output LOW 
  
  Returns:
  void   
*/

void R_SYS_GPIO_SetPin(int32_t Port, uint8_t Pin, uint8_t Value);

/*******************************************************************************
  Function: R_SYS_GPIO_InvPin

  Inverts the level output on the given Port and Pin
  Parameters:

  Port       - Port number 
  Pin        - Pin  number 
  
  Returns:
  void   
*/

void R_SYS_GPIO_InvPin(int32_t Port, uint8_t Pin);


/*******************************************************************************
  Function: R_SYS_GPIO_InvPort

  Inverts the level output on the given Port (all pins are inverted)
  
  Parameters:

  Port       - Port number 
  
  Returns:
  void   
*/
void R_SYS_GPIO_InvPort(int32_t Port); 

/*******************************************************************************
  Function: R_SYS_GPIO_GetPort
  
  This is the device specific implemetation of the R_GPIO_PortRead
  function 
  
  Parameters:

    Port       - Port number 

  Returns:
  Port input register value 

*/

 uint16_t R_SYS_GPIO_GetPort(int32_t Port);
 
 /*******************************************************************************
  Function: R_SYS_GPIO_PinDirection
  
  This is the device specific implemetation of the R_GPIO_PinDirection function

  Parameters:

    Port       - Port number (0-R_SYS_PORT_NUM)
    Pin        - Pin number  (0-15)
    Dir        - 0: Output, !=0 Input 

  Returns:
  void 

*/

void R_SYS_GPIO_PinDirection(int32_t Port, uint8_t Pin, uint8_t Dir);


/*******************************************************************************
  Function: R_SYS_GPIO_WritePort

  This is the device specific implemetation of the R_GPIO_WritePort
  function 

  Port       - Port number 
  Mask       - Pin mask, only when a bit is set here 
               the corresponding pin will be changed 
  Val        - Bit_x is 1 -> pin_x is HIGH, 
               Bit_x is 0 -> pin_x is LOW,   
  
  Returns:
  void   
*/

void R_SYS_GPIO_WritePort(int32_t Port, uint16_t Mask, uint16_t Value); 

/*******************************************************************************
  Function: R_SYS_GPIO_EnableInt
  
  Enable the given external (pin) interrupt  
  
  Parameters:
  Unit        -  the number of the external interrupt 
                     
  Returns:              
  void
*/

void R_SYS_GPIO_EnableInt(uint32_t Unit);  

/*******************************************************************************
  Function: R_SYS_GPIO_DisableInt
  
  Disable the given external (pin) interrupt  
  
  Parameters:
  Unit        -  the number of the external interrupt 
                     
  Returns:              
  void
*/

void R_SYS_GPIO_DisableInt(uint32_t Unit);  


/*******************************************************************************
  Function: R_SYS_GPIO_SetIntPrio

  Configure interrupt priority.  

  Parameters:
  Unit       - number (Id) of external interrupt 
  Prio       - Interrupt priority 

  Returns:
  Returns:
  void
*/

void R_SYS_GPIO_SetIntPrio(uint32_t Unit, uint8_t Prio) ;

/*******************************************************************************
  Function: R_SYS_GPIO_SetIntFilter

  Configure the trigger (EDGE,level,off) for the given external interrupt.  

  Parameters:
  Unit       - number (Id) of external interrupt 
  Trigger    - activation trigger, see <r_gpio_IntTrigger_t>

  Returns:
  void
*/
void R_SYS_GPIO_SetIntFilter(uint32_t Unit, r_gpio_IntTrigger_t Trigger);
          
/*******************************************************************************
  Function: R_SYS_GPIO_ClearIntPendingFlag

  Clear the interrupt pending flag for the given external interrupt.  

  Parameters:
  Unit        - number (Id) of external interrupt 

  Returns:
  void
*/

void R_SYS_GPIO_ClearIntPendingFlag(uint32_t Unit); 

/*******************************************************************************
  Function: R_SYS_GPIO_IOReset
  
  This is the device specific implemetation of the R_GPIO_IOReset
  function 
  
  Parameters:
  SetRelease  - 1 - activate reset state, 
                0 - release reset state, 
              
  Returns:              
  void

*/

void R_SYS_GPIO_IOReset(uint8_t SetRelease); 

#ifdef __cplusplus
}
#endif


#endif /* R_GPIO_SYS_H_ */
