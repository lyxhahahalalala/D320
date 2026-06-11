/*
****************************************************************************
PROJECT : VLIB Macro Driver 
FILE    : $Id: r_ecm_main.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic ECM driver
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


#ifndef R_ECM_MAIN_H_
#define R_ECM_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Main Module Internal Interface

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

#undef  DEF_EXTERN_
#ifndef R_ECM_MAIN_GLOBALS_
#define DEF_EXTERN_ extern 
#else
#define DEF_EXTERN_
#endif

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: r_ecm_Dev_t

  Device instance specific data.
  
  All instance specific data is kept within that structure.

  Members:
  BasAddr       - Macro register base address
  Locked        - if set, the device is already initialised 
  Settings      - Some settings  -  Just a placeholder/example -  
  Config        - Some config    - don't need to use both 
  IsrCallback   - Pointer table to Isr callback functions
*/

typedef struct {
    
    uint32_t   BaseAddr;
    uint32_t   Settings;
    uint32_t   Config;
    void       (*FEINTCallback[R_DEV_ECMERR_LAST])(void);
    void       (*FENMICallback[R_DEV_ECMERR_LAST])(void);

} r_ecm_Dev_t;


/*******************************************************************************
  Variable: r_ecm_Dev

  Array for device instance specific data.
*/

DEF_EXTERN_ r_ecm_Dev_t r_ecm_Dev[R_ECM_MACRO_NUM];


/*******************************************************************************
  Function: R_ECM_CheckMacroParams
  
  Performs a range check for standard macro parameters. 
  
  For unused parameters fill in a 0 there  

 Parameters:
  Unit          - Ecm Macro instance number

  Returns:
  see: <r_ecm_Error_t>

*/
r_ecm_Error_t R_ECM_CheckMacroParams(uint32_t Unit); 


/*******************************************************************************
  Function: R_ECM_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_ecm_Error_t>
  
  Returns:
  void
*/

void R_ECM_ErrorHandler(uint32_t Unit, r_ecm_Error_t Error);


/*******************************************************************************
  Function: R_ECM_CheckProtectionCommandStatus

  Check if the last protected command sequence executed successfully.
  Returns zero (0) on success and one (1) on failure.

  Parameters:
  Unit       - Instance number
  
  Returns:
  uint32_t
*/

uint32_t R_ECM_CheckProtectionCommandStatus(uint32_t Unit);


/*******************************************************************************
  Function: R_ECM_GetRegisterNoFromErrSource

  Translates an error source from its identification-number to 
  its register number where it's settings- or status-bit will be found.

  Parameters:
  Unit       - Instance number
  
  Returns:
  uint32_t
*/

uint32_t R_ECM_GetRegisterNoFromErrSource(uint32_t Unit, r_dev_EcmErrSel_t ErrSource);


/*******************************************************************************
  Function: R_ECM_GetRegisterMaskFromErrSource

  Translates an error source from its identification-number to 
  its bit mask where it's settings- or status-bit will be found.

  Parameters:
  Unit       - Instance number
  
  Returns:
  uint32_t
*/

uint32_t R_ECM_GetRegisterMaskFromErrSource(uint32_t Unit, r_dev_EcmErrSel_t ErrSource);


#ifdef __cplusplus
}
#endif

#endif /* R_ECM_MAIN_H_  */
