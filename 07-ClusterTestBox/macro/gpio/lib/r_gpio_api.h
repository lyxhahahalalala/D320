/*
****************************************************************************
PROJECT : GPIO driver
FILE    : $Id: r_gpio_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
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

 Version Author Description
 $Log: $


*/

#ifndef R_GPIO_API_H_
#define R_GPIO_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: GPIO API

  An application using general purpose ports  
  should include this header file.
  It provides interface to write and read output pins
  and external pin interrups 
*/

/*******************************************************************************
    Section: Global Types
*/

/*******************************************************************************
  Enum: r_gpio_IntTrigger_t

    Possible settings for external inerrupt detection 

    R_GPIO_INT_TRIGGER_OFF     - detection disabled 
    R_GPIO_INT_RISING_EDGE     - interrupt on rising edge
    R_GPIO_INT_FALLING_EDGE    - interrupt on falling edge
    R_GPIO_INT_BOTH_EDGES      - interrupt on both edges
    R_GPIO_INT_LEVEL_HIGH      - level generated interrupt (high level)
    R_GPIO_INT_LEVEL_LOW       - level generated interrupt (low level)
    R_GPIO_INT_TRIGGER_NUM     - delimiter     

*/
typedef enum
{
    R_GPIO_INT_TRIGGER_OFF  = 0u, 
    R_GPIO_INT_RISING_EDGE     ,
    R_GPIO_INT_FALLING_EDGE    ,
    R_GPIO_INT_BOTH_EDGES      ,
    R_GPIO_INT_LEVEL_HIGH      ,
    R_GPIO_INT_LEVEL_LOW       ,
    R_GPIO_INT_TRIGGER_NUM       
}r_gpio_IntTrigger_t; 

/*******************************************************************************
  Structure: r_gpio_IntConfig_t
   
   Use this structure to configure external interrups  
   Pin assignment is done on device (global) level
  
  Values: 
  Port      -   Port number
  Pin       -   Pin number
  Trigger   -   Trigger settings, see <r_gpio_IntTrigger_t>
  Callback  -   Callback function 
*/

typedef struct  {
    int32_t                     Port;
    int32_t                     Pin;
    r_gpio_IntTrigger_t     Trigger; 
    void                    (*Callback)(void);
}r_gpio_IntConfig_t;  


/*******************************************************************************
  Section: Global API Functions
*/



/*******************************************************************************
  Function: R_GPIO_PinRead

  The function returns the physical level of the pin 

  Parameters:

    Port       - Port number (0,R_SYS_PORT_NUM)
    Pin        - Pin number  (015)

  Returns:
    = 1      - Pin is HIGH
    = 0      - Pin is LOW
*/
 uint8_t R_GPIO_PinRead(int32_t Port, uint8_t Pin);

/*******************************************************************************
  Function: R_GPIO_PortRead

  The function returns the physical level of all pins 
  of the given Port.

  Parameters:

    Port       - Port number (0,R_SYS_PORT_NUM)

  Returns:
  Port input register value 
*/
 uint16_t R_GPIO_PortRead(int32_t Port);


/*******************************************************************************
  Function: R_GPIO_TogglePin

  The function inverts the output level of the given pin

  Parameters:

  Port       - Port number (0,R_SYS_PORT_NUM)
  Pin        - Pin  number  (0,15)

  Returns:
  void 

*/

void R_GPIO_TogglePin(int32_t Port, uint8_t Pin);

/*******************************************************************************
  Function: R_GPIO_TogglePort

  The function inverts the output level of the all pins 
  (with mask set) of the given port

  Parameters:

  Port       - Port number (0u,R_SYS_PORT_NUM)
  Mask       - Pin mask, only when a bit is set here 
               the corresponding pin will be changed 

  Returns:
  void 

*/

void R_GPIO_TogglePort(int32_t Port, uint16_t Mask);

/*******************************************************************************
  Function: R_GPIO_PinDirection

  The function sets the direction output or input) of the given pin  

  Parameters:

    Port       - Port number (0,R_SYS_PORT_NUM)
    Pin        - Pin number  (0,15)
    Dir        - 0: Output, !=0 Input 

  Returns:
  void 
*/
void R_GPIO_PinDirection(int32_t Port, uint8_t Pin, uint8_t Dir);

/*******************************************************************************
  Function: R_GPIO_WritePin

  The function sets or resets the output of the given pin

  Parameters:

  Port       - Port number (0,R_SYS_PORT_NUM)
  Pin        - Pin  number (0,15)
  Value      - 1: Set output HIGH, 0: Set output LOW 
  
  Returns:
  void   
 */

void R_GPIO_WritePin(int32_t Port, uint8_t Pin, uint8_t Value);

/*******************************************************************************
  Function: R_GPIO_WritePort

  The function sets or resets the output of the all pins
  of the given Port simultanously 
  
  Parameters:

  Port       - Port number (0-R_SYS_PORT_NUM)
  Mask       - Pin mask, only when a bit is set here 
               the corresponding pin will be changed 
  Value      - Bit_x is 1 -> pin_x is HIGH, 
               Bit_x is 0 -> pin_x is LOW,   
  
  Returns:
  void   
 */

void R_GPIO_WritePort(int32_t Port, uint16_t Mask, uint16_t Value);


/*******************************************************************************
  Function: R_GPIO_Init(void)

    GPIO initialisation function

  Parameters:
    void
  
  Returns:
    void

*/

void R_GPIO_Init(void);


/*******************************************************************************
  Function: R_GPIO_DeInit(void)

  Clears all regs and sets all ports to (disabled) GPIO input

  Parameters:
  void

  Returns:
  void

*/

void R_GPIO_DeInit(void);

/*******************************************************************************
  Function: R_GPIO_EnableInt
  
  Enable the given external (pin) interrupt  
  
  Parameters:
  Unit        -  the number of the external interrupt 
                     
  Returns:              
  void
*/

void R_GPIO_EnableInt(uint32_t Unit);  

/*******************************************************************************
  Function: R_GPIO_DisableInt
  
  Disable the given external (pin) interrupt  
  
  Parameters:
  Unit        -  the number of the external interrupt 
                     
  Returns:              
  void
*/

void R_GPIO_DisableInt(uint32_t Unit);  

/*******************************************************************************
  Function: R_GPIO_SetIntCallback
  
  Assign callback function to an external interrupt ISR
  
  Parameters:
  Unit        - the number of the external interrupt 
  Isr         - Callback ISR        
  
  Returns:              
  == R_ERR_OK     - success
  != R_ERR_OK     - fail
*/
r_Error_t R_GPIO_SetIntCallback(uint32_t Unit, void (*Isr)(void)) ; 


/*******************************************************************************
  Function: R_GPIO_InitInt

  Initilize the given external interrupt   
  
  Parameters:
  Unit        - the number of the external interrupt 
  Config      - config data, see <r_gpio_IntConfig_t>       
  
  Returns:              
  == R_ERR_OK     - success
  != R_ERR_OK     - fail
*/
r_Error_t R_GPIO_InitInt(uint32_t Unit, r_gpio_IntConfig_t *Config); 

/*******************************************************************************
  Function: R_GPIO_DeInitInt

  De-Initilize the given external interrupt   
  
  Parameters:
  Unit        - the number of the external interrupt 
  Config      - config data, see <r_gpio_IntConfig_t>       
  
  Returns:              
  == R_ERR_OK     - success
  != R_ERR_OK     - fail
*/
r_Error_t R_GPIO_DeInitInt(uint32_t Unit); 

/*******************************************************************************
  Function: R_GPIO_ReadIntPin

  Read the actual level of the external interrupt pin
  
  Parameters:
  Unit        - the number of the external interrupt 
  
  Returns:              
  == 1        - Pin is HIGH
  == 0     - Pin is LOW
*/
uint8_t R_GPIO_ReadIntPin(uint32_t Unit);


/*******************************************************************************
  Function: R_GPIO_IntIsr 
  
  ISR for the external ionterrupts
  
  Parameters:
  Unit        - the number of the external interrupt 
              
  Returns:              
  void
  
*/
void R_GPIO_IntIsr(uint32_t Unit);


/*******************************************************************************
  Function: R_GPIO_IOReset 
  
  Enable/disable Reset state of IO Buffer.
  
  Parameters:
  SetRelease       - 1 - activate reset state, 
                     0 - release reset state, 
              
  Returns:              
  void
  
*/
void R_GPIO_IOReset(uint8_t SetRelease);

/* Add here handling of external interrupts */



#ifdef __cplusplus
}
#endif

#endif /* R_GPIO_API_H_  */
