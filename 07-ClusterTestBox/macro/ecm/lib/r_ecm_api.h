/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_ecm_api.h 4537 2014-12-12 10:23:23Z golczewskim $
============================================================================ 
DESCRIPTION
Generic part of the macro driver
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2012
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

#ifndef R_ECM_API_H_
#define R_ECM_API_H_

#include "r_dev_err_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: ECM Driver API 
  
  An application using ECM should only need to include this r_ecm_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_ECM_VERSION_HI and R_ECM_VERSION_LO

  Driver version information  
*/

#define R_ECM_VERSION_HI 0
#define R_ECM_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_ecm_Error_t

  ECM macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_ECM_ERR_OK           - No error
  R_ECM_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_ECM_ERR_OK         = 0x00,
    R_ECM_ERR_NG         ,
    R_ECM_ERR_RANGE      ,
    R_ECM_ERR_LOCKED     ,
    R_ECM_ERR_NOTLOCKED  
} r_ecm_Error_t;

/*******************************************************************************
  Enum: r_ecm_Interrupt_t

  Ecm macro interrupt type. 
  
  List of all available interrupts for this macro.

  Values:
  R_ECM_INT_FENMI      - Fatal Exception Non-Maskable-Interrupt
  R_ECM_INT_FEINT      - Fatal Exception Maskable-Interrupt
  R_ECM_INT_NUM        - Delimiter 
*/

typedef enum
{
    R_ECM_INT_FENMI,
    R_ECM_INT_FEINT,
    R_ECM_INT_NUM
} r_ecm_Interrupt_t;



/*******************************************************************************
  Enum: r_ecm_ErrorOutPinState_t

  Ecm macro error output pin state. 
  
  Values:
  R_ECM_EPS_NoError    - 
  R_ECM_EPS_Error      - 
  R_ECM_EPS_NUM        - Delimiter 
*/

typedef enum
{
    R_ECM_EPS_NoError,
    R_ECM_EPS_Error,
    R_ECM_EPS_NUM
} r_ecm_ErrorPinState_t;


/*******************************************************************************
  Enum: r_ecm_ErrorOutPinMode_t

  Ecm macro error output pin mode. 
  
  Values:
  R_ECM_EP_NonDynamic - 
  R_ECM_EP_Dynamic    - 
  R_ECM_EP_NUM        - Delimiter 
*/

typedef enum
{
    R_ECM_EP_NonDynamic,
    R_ECM_EP_Dynamic,
    R_ECM_EP_NUM
} r_ecm_ErrorPinMode_t;


/*******************************************************************************
  Enum: r_ecm_ErrorPulseMode_t

  Ecm macro timer source for dynamic error output pin pulse generation. 
  
  Values:
  R_ECM_EPM_TAUB2CH15  - 
  R_ECM_EPM_OSTM       - 
  R_ECM_EPM_NUM        - Delimiter 
*/

typedef enum
{
    R_ECM_EPM_TAUB2CH15,
    R_ECM_EPM_OSTM,
    R_ECM_EPM_NUM
} r_ecm_ErrorPulseMode_t;


/*******************************************************************************
  Type: r_ecm_ErrSrcConfig

  All possible configurations for a specific error source
  
  Members:
  ErrSrc        - Error Source to be configured
  FEINT         - Enables / Disables the maskable interrupt for this error source 
  FENMI         - Enables / Disables the non-maskable interrupt for this error source
  FEINTTimer    - Enables / Disables the delay timer for the maskable interrupt for this error source 
  FENMITimer    - Enables / Disables the delay timer for the non-maskable interrupt for this error source
  Reset         - Enables / Disables the reset geneartion for this error source
  Mask          - Enabled / Disables the error output pin erro signalling for this error source
*/

typedef struct {
    
    r_dev_EcmErrSel_t   ErrSource;
    uint32_t            FEINT:1;
    uint32_t            FENMI:1;
    uint32_t            FEINTTimer:1;
    uint32_t            FENMITimer:1;
    uint32_t            Reset:1;
    uint32_t            Mask:1;

} r_ecm_ErrSrcConfig_t;

#if 0
/*******************************************************************************
  Enum: r_ecm_ProtectionRegisterSelect_t

  Ecm macro has three different protection registers. 
  
  Values:
  R_ECM_PR_Master     - 
  R_ECM_PR_Checker    - 
  R_ECM_PR_Common     - 
  R_ECM_EP_NUM        - Delimiter 
*/

typedef enum
{
    R_ECM_PR_Master,
    R_ECM_PR_Checker,
    R_ECM_PR_Common,
    R_ECM_PR_NUM
} r_ecm_ProtectionRegisterSelect_t;
#endif

/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_ECM_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_ECM_GetVersionStr(void); 

/*******************************************************************************
  Function: R_ECM_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_ECM_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_ECM_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_ECM_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_ECM_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_ecm_Error_t>
*/

 r_ecm_Error_t R_ECM_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_ECM_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_ecm_Error_t>
*/

 r_ecm_Error_t R_ECM_DeInit(uint32_t Unit);


/*******************************************************************************
  Group: ECM Error and ISR Functions
*/

/*******************************************************************************
  Function: R_ECM_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_ECM_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));

/*******************************************************************************
  Function: R_ECM_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  For each, the maskable-interrupt and the non-maskable-interrupt, 
  all available error sources can be assigned as callback.
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  Interrupt     - What interrupt,  see: <r_ecm_Interrupt_t>
  ErrorSource   - What error source,  see: <r_dev_EcmErrSel_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_ecm_Error_t>
*/

 r_ecm_Error_t R_ECM_SetIsrCallback(uint32_t Unit, r_ecm_Interrupt_t Interrupt, 
                                    r_dev_EcmErrSel_t ErrorSource, void (*Isr)(void));
                                 

/*******************************************************************************
  Function: R_ECM_FeNmiIsr

  Driver Fatal-Exception Level Non-Maskable-Interrupt interrupt service routine. 
  
  This function shall be called by device interrupt handler 
  (xxxxxx_isr.c) and can be found in ecm\src\ecm_isr.c. 

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_ECM_FeNmiIsr(uint32_t Unit);


/*******************************************************************************
  Function: R_ECM_FeIntIsr

  Driver Fatal-Exception Level Maskable-Interrupt interrupt service routine. 
  
  This function shall be called by device interrupt handler 
  (xxxxxx_isr.c) and can be found in ecm\src\ecm_isr.c. 

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_ECM_FeIntIsr(uint32_t Unit);



#if 0
/*******************************************************************************
  Function: R_ECM_EnableInt

  Enable the ECM interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ecm_Interrupt_t>
  
  Returns:
  void
*/

void R_ECM_EnableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt);
#endif

#if 0
/*******************************************************************************
  Function: R_ECM_DisableInt

  Disable the ECM interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ecm_Interrupt_t>
  
  Returns:
  void
*/

void R_ECM_DisableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt);
#endif




/*******************************************************************************
  Function: R_ECM_SetErrorOutPinMode

  Set the pinmode either to dynamic or non-dynamic.
  
  Parameters:
  Unit       - Instance number
  EPMode     - Pin mode, see: <r_ecm_ErrorPinMode_t>
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_SetErrorOutPinMode(uint32_t Unit, r_ecm_ErrorPinMode_t EPMode);

/*******************************************************************************
  Function: R_ECM_SetErrorOutPulseMode

  Select the timer that generates the pulses for the dynamic pin mode
  
  Parameters:
  Unit       - Instance number
  EPMode     - Pin mode, see: <r_ecm_ErrorPulseMode_t>
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_SetErrorOutPulseMode(uint32_t Unit, r_ecm_ErrorPulseMode_t EPMode);

/*******************************************************************************
  Function: R_ECM_StopDelayTimer

  Stop the delay timer that can be configured to start when a certain error source reports an error.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_StopDelayTimer(uint32_t Unit);

/*******************************************************************************
  Function: R_ECM_GetDelayTimerStatus

  Check if the delay timer that can be configured to start when a certain 
  error source reports an error is currently stopped or running.
  The status of the timer is either (0) stopped or (1) running.
  Additionally, the current counter values can be retireved by supplying a valid pointer.
  
  Parameters:
  Unit       - Instance number
  TimerValue - Counter value of the timer. Provide null-pointer if not required.
  
  Returns:
  uint32_t: (0) stopped or (1) running
*/

uint32_t R_ECM_GetDelayTimerStatus(uint32_t Unit, uint32_t* TimerValue);

/*******************************************************************************
  Function: R_ECM_SetErrorOutput

  Sets the state of the error output pin to <State>.
  This function can eiterh set the error output pin or clear it.
  To clear this pin, make sure that there are no active errors, 
  that are configured to set the status of this pin to ERROR.
  
  Parameters:
  Unit       - Instance number
  State      - Error Pin State, see: <r_ecm_ErrorPinState_t>
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_SetErrorOutput(uint32_t Unit, r_ecm_ErrorPinState_t State);

/*******************************************************************************
  Function: R_ECM_SetDelayTimerCompareValue

  Set the compare values of the delay timer that can be configured to start 
  when a certain error source reports an error.
  When the delay timer reaches the compare value, either the output pin can be 
  set to error or a reset condition can be generated.
  
  Parameters:
  Unit          - Instance number
  CompareValue  - Compare value for the timer.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_SetDelayTimerCompareValue(uint32_t Unit, uint32_t CompareValue);

/*******************************************************************************
  Function: R_ECM_GetErrorSourceConfig

  Retrieve the configuration of a certain error source.
  This function return a struct, that contains all setting that are specific to 
  the requested error source.
  
  Parameters:
  Unit          - Instance number
  ErrSource     - Error Source whose configuration should be retrieved.
  
  Returns:
  see: <r_ecm_ErrSrcConfig_t>
*/

r_ecm_ErrSrcConfig_t R_ECM_GetErrorSourceConfig(uint32_t Unit, r_dev_EcmErrSel_t ErrSource);

/*******************************************************************************
  Function: R_ECM_SetErrorSourceConfig

  Set the configuration of a certain error source.
  This function expects, that contains all setting that are specific to 
  the error source to be configured.
  
  Parameters:
  Unit              - Instance number
  ErrSourceConfig   - Pointer to the error source configuration 
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_SetErrorSourceConfig(uint32_t Unit, r_ecm_ErrSrcConfig_t *ErrSourceConfig);

/*******************************************************************************
  Function: R_ECM_ConfigureAllErrorSources

  Batch operation of <R_ECM_SetErrorSourceConfig>.
  Provide an array with configuration data, and all source that are represented 
  in the array will be configured.
  The <r_ecm_ErrSrcConfig_t> also contains an <r_dev_EcmErrSel_t> parameter that 
  will be evaluated by this function.
  
  Parameters:
  Unit              - Instance number
  ErrSourceConfig   - Array of configuration data to be written
  Count             - Number of elements to be written
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_ConfigureAllErrorSources(uint32_t Unit, r_ecm_ErrSrcConfig_t *ErrSourceConfig, uint32_t Count);

/*******************************************************************************
  Function: R_ECM_EnableFENMITimer
  
  Enable or Disable the FENMI Timer for the requested error source.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  Enable            - Set (0) to enable or (1) to disable the configuration for this error source.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_EnableFENMITimer(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable);


/*******************************************************************************
  Function: R_ECM_EnableFEINTTimer
  
  Enable or Disable the FEINT Timer for the requested error source.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  Enable            - Set (0) to enable or (1) to disable the configuration for this error source.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_EnableFEINTTimer(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable);

/*******************************************************************************
  Function: R_ECM_TriggerPseudoError
  
  Simulate an error event for the requested error source.
  The event will then behave linke a real error that was input by a connected module.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_TriggerPseudoError(uint32_t Unit, r_dev_EcmErrSel_t ErrSource);

/*******************************************************************************
  Function: R_ECM_ClearErrorSourceStatus

  Clear the error source status for the requested error source.
  Resets the error source status to normal.  Make sure, the error source is cleared.
  If the error input of this signal still alters an error, 
  the status will immediately be active again.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_ClearErrorSourceStatus(uint32_t Unit, r_dev_EcmErrSel_t ErrSource);

/*******************************************************************************
  Function: R_ECM_EnableMask

  Enable or Disable the error output pin signalisation for the requested error source.
  If enabled, the requested error source will be able cause the error output pin to drive low level.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  Enable            - Set (0) to enable or (1) to disable the configuration for this error source.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_EnableMask(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable);

/*******************************************************************************
  Function: R_ECM_EnableReset

  Enable or Disable the reset signal generation for the requested error source.
  If enabled, the requested error source will be able cause a reset.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  Enable            - Set (0) to enable or (1) to disable the configuration for this error source.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_EnableReset(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable);

/*******************************************************************************
  Function: R_ECM_EnableFENMI

  Enable or Disable the FE-level non-maksable interrupt generation for the requested error source.
  If enabled, the requested error source will be able cause an FENMI.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  Enable            - Set (0) to enable or (1) to disable the configuration for this error source.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_EnableFENMI(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable);

/*******************************************************************************
  Function: R_ECM_EnableFEINT

  Enable or Disable the FE-level maksable interrupt generation for the requested error source.
  If enabled, the requested error source will be able cause an FEINT.
  
  Parameters:
  Unit              - Instance number
  ErrSource         - Error Source whose configuration should be written.
  Enable            - Set (0) to enable or (1) to disable the configuration for this error source.
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_EnableFEINT(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable);



#ifdef __cplusplus
}
#endif

#endif /* ECM_API_H  */
