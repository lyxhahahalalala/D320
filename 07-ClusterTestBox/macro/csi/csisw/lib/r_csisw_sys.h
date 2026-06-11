/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_csisw_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic CSISW driver
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2014
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

#ifndef R_CSISW_SYS_H_
#define R_CSISW_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic CSISW Support Functions

  The generic CSISW driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Enum: r_csisw_Pin_t

  Pin selection

  Values:
    R_CSISW_CLK_PIN   - clock pin        (output)
    R_CSISW_TXD_PIN   - transmit pin     (output)
    R_CSISW_RXD_PIN   - receive pin      (input)
    R_CSISW_SS0_PIN   - slave select pin (output)
*/

typedef enum {
    R_CSISW_CLK_PIN,
    R_CSISW_TXD_PIN,
    R_CSISW_RXD_PIN,
    R_CSISW_SS0_PIN
} r_csisw_Pin_t;

/*******************************************************************************
  Enum: r_csisw_Level_t

  Pin selection

  Values:
    R_CSISW_LEVEL_ACTIVE     - active level
    R_CSISW_LEVEL_INACTIVE   - inactive level
*/

typedef enum {
    R_CSISW_LEVEL_ACTIVE = 1,
    R_CSISW_LEVEL_INACTIVE = 0
} r_csisw_Level_t;

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_CSISW_SYS_HardwareInit

  Setup the HW part for the generic CSISW.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_CSISW_SYS_HardwareDeInit

  Deinit of the HW part for the generic CSISW.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_CSISW_SYS_SetPin

  Set decidated (output) pin to certain level.
  
  Parameters:
  Unit       - Instance number
  PinName    - Name of port pin to set. see: <r_csisw_Pin_t>
  Level      - Level to set the pin to. see: <r_csisw_Level_t>
  
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_SYS_SetPin(uint32_t Unit,
                                    r_csisw_Pin_t PinName,
                                    r_csisw_Level_t Level);


/*******************************************************************************
  Function: R_CSISW_SYS_GetPin

  Get level of decidated (input) pin.
  
  Parameters:
  Unit       - Instance number
  PinName    - Name of port pin to set. see: <r_csisw_Pin_t>
  
  
  Returns:
  Level      - Level to set the pin to. see: <r_csisw_Level_t>
*/

r_csisw_Level_t R_CSISW_SYS_GetPin(uint32_t Unit, r_csisw_Pin_t PinName);


/*******************************************************************************
  Function: R_CSISW_SYS_Delay

  wait a short time
  
  Parameters:
  Unit       - Instance number
  TimeUs     - time to wait in us
  
  
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_SYS_Delay(uint32_t Unit, uint32_t TimeUs);


#ifdef __cplusplus
}
#endif


#endif /* R_CSISW_SYS_H_ */
