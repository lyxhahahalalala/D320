/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_tick_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
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

#ifndef R_TICK_API_H_
#define R_TICK_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: Generic TICK API
*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_tick_Error_t

  TICK macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_TICK_ERR_OK           - No error
  R_TICK_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_TICK_ERR_OK         = 0x00u,
    R_TICK_ERR_NG         = 0x01
} r_tick_Error_t;


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_TICK_Init

  Driver init function.

  Parameters:
  Unit          - Instance number
 
  Returns:
  see: <r_tick_Error_t>
*/

 r_tick_Error_t R_TICK_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_TICK_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_tick_Error_t>
*/

 r_tick_Error_t R_TICK_DeInit(uint32_t Unit);



/*******************************************************************************
  Function: R_TICK_GetTimeMS

  Return system tick time in ms

  Parameters:
  Unit       - Instance number

  Returns:
  Tick time in ms
*/

 uint32_t R_TICK_GetTimeMS(uint32_t Unit);


/*******************************************************************************
  Function: R_TICK_GetTimeUS

  Return system tick time in us

  Parameters:
  Unit       - Instance number

  Returns:
  Tick time in ms
*/

 uint32_t R_TICK_GetTimeUS(uint32_t Unit);


/*******************************************************************************
  Function: R_TICK_WaitMS

  Wait for at least 'TickMS' ms and return

  Parameters:
  Unit        - Instance number
  TickMS      - Number of ms to wait

  Returns:
  void
*/

void R_TICK_WaitMS(uint32_t Unit, uint32_t TickMS);


/*******************************************************************************
  Function: R_TICK_WaitUS

  Wait for at least 'TickUS' us and return

  Parameters:
  Unit        - Instance number
  TickUS      - Number of ms to wait

  Returns:
  void
*/

void R_TICK_WaitUS(uint32_t Unit, uint32_t TickUS);


/*******************************************************************************
  Function: R_TICK_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  Isr           - Isr function pointer
  
  Returns:
  void
*/
r_tick_Error_t R_TICK_SetIsrCallback(uint32_t Unit, void (*Isr)(void));
                                 

/*******************************************************************************
  Function: R_TICK_Isr

  Driver interrupt service routine. 
  
  This function shall be called by device interrupt handler 
  (dfxxxx_isr.c) and can be found in tick\src\tick_isr.c. 

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_TICK_Isr(uint32_t Unit);


/*******************************************************************************
  Function: R_TICK_EnableInt

  Enable the TICK interrupt.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_TICK_EnableInt(uint32_t Unit);


/*******************************************************************************
  Function: R_TICK_DisableInt

  Disable the TICK interrupt.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_TICK_DisableInt(uint32_t Unit);


#ifdef __cplusplus
}
#endif

#endif /* R_TICK_API_H_  */
