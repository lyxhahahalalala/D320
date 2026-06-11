/*
****************************************************************************
PROJECT : IP Testing
FILE    : $Id: r_bsp_hmi_sys.h 778 2013-04-10 15:26:40Z michael.golczewski $
============================================================================ 
Description
Main Module Header for IP test frame
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

#ifndef BSP_HMI_SYS_H_
#define BSP_HMI_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Section: board specific HMI service functions
 
  These functions must be implemented specific in
  'vlib/bsb/board/<any board>/src/hmi/r_bsp_sys_hmi.c'
*/

/*******************************************************************************
  Function: R_BSP_SYS_HMI_Init
*/

void R_BSP_SYS_HMI_Init(void); 

/*******************************************************************************
  Function: R_BSP_SYS_HMI_DeInit
*/

void R_BSP_SYS_HMI_DeInit(void); 
    
/*******************************************************************************
  Function: R_BSP_SYS_HMI_GetLed

  Get the state (on or off) of a LED. 

  Parameters:
  LedNumber    - LED Id (numeric)

  Returns:
  == 0     - LED is off 
  >  0     - LED is on, return value equals the brightness setting
  <  0     - wrong LED Id
  
*/

 int8_t R_BSP_SYS_HMI_GetLed(uint8_t LedNumber); 


/*******************************************************************************
  Function: R_BSP_SYS_HMI_SetLed

  Set the brightness (from 0: off to 100: on) of a LED. 
  
  The diming itself might not be present on the given board, 
  in this casse any value for Brightness other than 0 turns the LED on 

  Parameters:
  LedNumber    - LED Id (numeric)
  Brightness   - From 0 (off) to 100 (on)

  Returns:
  == E_OK    - success
  != E_OK    - wrong LED number 
  
*/

r_Error_t R_BSP_SYS_HMI_SetLed(uint8_t LedNumber, uint32_t Brightness); 
    


#ifdef __cplusplus
}
#endif

#endif /* BSP_HMI_SYS_H_ */

