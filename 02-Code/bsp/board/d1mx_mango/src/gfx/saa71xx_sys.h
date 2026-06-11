/*
****************************************************************************
PROJECT : BSP - GFX - VI SAA71XX driver
FILE    : $Id: saa71xx_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================
DESCRIPTION
BSP Implementation D1M2(H) MANGO Board
SAA71XX driver (sys functions header)
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

#ifndef R_SAA71XX_SYS_H_
#define R_SAA71XX_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: SAA71XX Support Functions

  The generic SAA71XX driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: R_SAA71xx_Sys_PortInit

  Configurates Ports for UART communication. 
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_PortInit(uint32_t Unit);


/***********************************************************
  Function: R_SAA71xx_Sys_PortDeInit

  Deinit of uart pins.
  
  Sets port pins of dedicated channel back to default value.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_PortDeInit(uint32_t Unit);


/***********************************************************
  Function: R_SAA71xx_Sys_CvbsSel

  Returns the SAA video input mode selection.
  
  This function is called to determine the selected
  video input mode for the SAA; Svideo or CVBS.

  Parameters:
  Unit       - Instance number

  Returns:
  R_SAA71XX_SVIDEO - if selection is SAA in Svideo mode
  R_SAA71XX_CVBS   - if selection is SAA in CVBS mode
*/

R_SAA71xx_VideoMode_t R_SAA71xx_Sys_CvbsSel(uint32_t Unit);


/***********************************************************
  Function: R_SAA71xx_Sys_I2CInit

  Configurates SAA I2C communication. 
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_I2CInit(uint32_t Unit);

R_SAA71xx_Error_t R_SAA71xx_Sys_I2CProbe(uint32_t Unit, int Address);

R_SAA71xx_Error_t R_SAA71xx_Sys_I2CWrite(uint32_t Unit, int Address, 
                                         int Offset, int Data);

int R_SAA71xx_Sys_I2CRead(uint32_t Unit, int Address, 
                                         int Offset);

#ifdef __cplusplus
}
#endif


#endif /* R_SAA71XX_SYS_H_ */
