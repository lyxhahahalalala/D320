/*
****************************************************************************
PROJECT : OSTM driver
FILE    : $Id: r_ostm_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
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

#ifndef R_OSTM_API_H_
#define R_OSTM_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: OSTM API

  An application using OSTM should include this header file.

*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_ostm_Error_t

  OSTM macro driver error code.

  Values:
  R_OSTM_ERR_OK          - No error
  R_OSTM_ERR_NG          - Unspecified error
  R_OSTM_ERR_PORTINIT    - OSTM port init failed
  R_OSTM_ERR_LAST        - Delimiter 
*/

typedef enum
{
    R_OSTM_ERR_OK         = 0x00u,
    R_OSTM_ERR_NG         = 0x01u,
    R_OSTM_ERR_RANGE      = 0x02u,
    R_OSTM_ERR_PORTINIT,
    R_OSTM_ERR_LAST
} r_ostm_Error_t;


/*******************************************************************************
  Enum: r_ostm_Interrupt_t

  OSTM macro interrupt type. List of all available interrupts for this macro

  Values:
    R_OSTM_INT_TINT       - Overflow Interrupt 
    R_OSTM_INT_NUM        - Delimiter 
*/

typedef enum
{
    R_OSTM_INT_TINT = 0 ,
    R_OSTM_INT_NUM
} r_ostm_Interrupt_t;


/*******************************************************************************
  Enum: r_ostm_Mode_t

  OSTM macro send direction.

  Configuration parameter for Uart init.

  Values:
    R_OSTM_INTERVAL        - Macro is in interval mode
    R_OSTM_FREE_RUN        - Macro is in free-run mode
*/

typedef enum
{
    R_OSTM_INTERVAL = 0u,
    R_OSTM_FREE_RUN
} r_ostm_Mode_t;


/*******************************************************************************
  Enum: r_ostm_IntCtl_t

  OSTM macro 

  Values:
    
    R_OSTM_START_INT_DISABLE   - Disable Interrupt at counter start
    R_OSTM_START_INT_ENABLE    - Enable Interrupt at counter start
*/

typedef enum
{    
    R_OSTM_START_INT_DISABLE = 0u,
    R_OSTM_START_INT_ENABLE
} r_ostm_IntCtl_t;


/*******************************************************************************
  Enum: r_ostm_OutputMode_t

  OSTM macro send direction.

  Configuration parameter for Uart init.

  Values:
    R_OSTM_OUTPUT_LEVEL    - Output level controlled by Software
    R_OSTM_TOGGLE_OUTPUT   - Output level toggle
*/

typedef enum
{
    R_OSTM_OUTPUT_CTL = 0u,
    R_OSTM_OUTPUT_TOGGLE
} r_ostm_OutputMode_t;


/*******************************************************************************
  Enum: r_ostm_OutputLevel_t

  OSTM macro send direction.

  Configuration parameter for Uart init.

  Values:
    R_OSTM_OUTPUT_LEVEL    - Output level controlled by Software
    R_OSTM_TOGGLE_OUTPUT   - Output level toggle
*/

typedef enum
{
    R_OSTM_LOW = 0u,
    R_OSTM_HIGH 
} r_ostm_OutputLevel_t;


/*******************************************************************************
  Enum: r_ostm_SvStop_t

  OSTM macro send direction.

  Configuration parameter for Uart init.

  Values:
    R_OSTM_OUTPUT_LEVEL    - Output level controlled by Software
    R_OSTM_TOGGLE_OUTPUT   - Output level toggle
*/

typedef enum
{
    R_OSTM_USE_SVSTOP = 0u,
    R_OSTM_NO_SVSTOP = 0x80
} r_ostm_SvStop_t;


/*******************************************************************************
  Enum: r_ostm_Tcke_t

  OSTM macro count source.


  Values:
    R_OSTM_COUNT_ENABLE    - 
    R_OSTM_COUNT_DISABLE   - 
    R_OSTM_COUNT_TCKEN14   - 
    R_OSTM_COUNT_TCKEN15   - 
*/

typedef enum
{
    R_OSTM_COUNT_ENABLE = 0u,
    R_OSTM_COUNT_DISABLE,
    R_OSTM_COUNT_TCKEN14,
    R_OSTM_COUNT_TCKEN15
} r_ostm_Tcke_t;


/*******************************************************************************
  Enum: r_ostm_OutputParams_t

  Ostm macro configuration data


*/     

typedef struct {
    
    r_ostm_OutputMode_t     Mode;
    r_ostm_OutputLevel_t    Level;
    
} r_ostm_OutputParams_t;


/*******************************************************************************
  Enum: r_ostm_Parameter_t

  Ostm macro configuration data

  CmpVal       - Comare value
  Mode         - Operating Mode, see: <r_ostm_Mode_t>
  IntCtl       - Configures Interrupt at counter start, see: <r_ostm_IntCtl_t>

*/     

typedef struct {
    r_ostm_Mode_t              Mode;  
    uint32_t                   CmpVal;
    r_ostm_IntCtl_t            IntCtl;
} r_ostm_Parameter_t;



/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_OSTM_Init

  Driver init function.

  Parameters:
  Unit       - Instance number
  Config     - see: <r_ostm_Parameter_t>
 
  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_Init(uint32_t Unit, r_ostm_Parameter_t *Config);


/*******************************************************************************
  Function: R_OSTM_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_GetStatus

  Gives information if counter is enabled or disabled

  Parameters:
  Unit       - Instance number

  Returns:
  Status of Counter 
*/

 uint8_t R_OSTM_GetStatus(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_GetCounterValue

  Returns the value of the counter

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_ostm_Error_t>
*/

 uint32_t R_OSTM_GetCounterValue(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_GetCompareRegValue

  Returns the value of the copmare register

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_ostm_Error_t>
*/

 uint32_t R_OSTM_GetCompareRegValue(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_GetOutputState

  Returns the value of the counter

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_ostm_Error_t>
*/

 uint8_t R_OSTM_GetOutputState(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_Start

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_Start(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_Stop

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_Stop(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_OutputCtl

  Sets the configuration of the output 

  Parameters:
  Unit       - Instance number
  OutputParams

  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_OutputCtl(uint32_t Unit, r_ostm_OutputParams_t *Params);


/*******************************************************************************
  Function: R_OSTM_SetOutputMode

 Sets only value of TOE register
 
  Parameters:
    Unit        - Instance number
    Mode        - Output config
  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_SetOutputMode(uint32_t Unit, r_ostm_OutputParams_t *Mode);


/*******************************************************************************
  Function: R_OSTM_SetSvstop

  Sets usage of the SVSTOP signal.

  The SVSTOP signal stops the counter in debug mode at 
  a breakpoint32_t .

  Parameters:
    Unit        - Instance number
    Svstop      - see: <r_ostm_SvStop_t>
  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_SetSvstop(uint32_t Unit, r_ostm_SvStop_t Svstop );



/*******************************************************************************
  Function: R_OSTM_SetCmp

  Sets new compare register value
 
  Parameters:
    Unit        - Instance number
    CmpVal      - New compare register value
    
  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_SetCmp(uint32_t Unit, uint32_t CmpVal);


/*******************************************************************************
  Function: R_OSTM_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback) 
                  with two paramerters (unit and error number) 
  
  Returns:
  void
 
*/

void R_OSTM_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error));


/*******************************************************************************
  Section: Ostm ISR Functions
*/

/*******************************************************************************
  Function: R_OSTM_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  IntType       - What kind of interrupt; see: <r_ostm_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_SetIsrCallback(uint32_t Unit, r_ostm_Interrupt_t IntType, 
                                 void (*Isr)(void));


/*******************************************************************************
  Function: R_OSTM_TintIsr

  Driver OV interrupt service routine. 
  This function shall be called by device interrupt handler 
  (<devicename>_isr.c) and can be found in ..\timer\ostm\src\ostm_isr.c.

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_OSTM_TintIsr(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_EnableInt

  Enable the Ostm interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ostm_Interrupt_t>
  
  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_EnableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_OSTM_DisableInt

  Disable the Ostm interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ostm_Interrupt_t>
  
  Returns:
  see: <r_ostm_Error_t>
*/

 r_ostm_Error_t R_OSTM_DisableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_OSTM_GetClock

  Gets the clock for the selected Ostm unit

  Parameters:
  Unit          Ostm unit

  Returns:
  uint32_t      clock frequency

*/
 uint32_t R_OSTM_GetClock(uint32_t Unit);


/*******************************************************************************
  Function: R_OSTM_SetClock

  Sets the clock for the selected Ostm unit

  Parameters:
  Unit          Ostm unit
  uint8_t       ID
  
  Returns:
  void

*/
void R_OSTM_SetClock(uint32_t Unit, uint8_t ID);


#ifdef __cplusplus
}
#endif

#endif /* R_OSTM_API_H_  */
