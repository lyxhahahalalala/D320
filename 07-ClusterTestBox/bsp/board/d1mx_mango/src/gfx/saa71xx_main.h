/*
****************************************************************************
PROJECT : BSP - GFX - VI SAA71XX driver
FILE    : $Id: saa71xx_main.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================
DESCRIPTION
BSP Implementation D1M2(H) MANGO Board
SAA71XX driver (main header)
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

#ifndef R_SAA71XX_MAIN_H_
#define R_SAA71XX_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Main Module Internal Interface

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

#undef _EXTERN_
#ifndef _SAA71XX_MAIN_GLOBALS_
#define _EXTERN_ extern 
#else
#define _EXTERN_
#endif

/***********************************************************
  Type: R_SAA71xx_REG_CHROMGAINCTL

  Register (sub)address of Chrominance gain control register
*/
#define R_SAA71xx_REG_CHROMGAINCTL  0xfu


/***********************************************************
  Section: Global Types
*/

typedef struct {
    uint8_t  *DataTable;
    int       Size;
    uint8_t  *Name;
    int       Address[2];
} R_SAA71xx_Probe_t;


/***********************************************************
  Type: R_SAA71xx_Dev_t

  Device instance specific data.
  All instance specific data is kept within that structure.

  Members:
  Type     - tbd
  Address  - tbd
  Current  - tbd
*/

typedef struct {
    R_SAA71xx_Type_t         Type;
    int                      Address;
    const R_SAA71xx_Probe_t *Current;
} R_SAA71xx_Dev_t;


/***********************************************************
  Section: Global Variables
*/

/***********************************************************
  Constant: R_SAA71XX_MACRO_NUM

  Number of SAA71XX macros in the device.
*/

#define R_SAA71XX_MACRO_NUM         1

/***********************************************************
  Variable: R_SAA71xx_Dev

  Array for device instance specific data.
*/

_EXTERN_ R_SAA71xx_Dev_t R_SAA71xx_Dev[R_SAA71XX_MACRO_NUM];


/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: R_SAA71xx_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  R_SAA71xx_ErrorHandler will call this user function. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <R_SAA71xx_Error_t>
  
  Returns:
  void
*/

void R_SAA71xx_ErrorHandler(uint32_t Unit, R_SAA71xx_Error_t Error);




#ifdef __cplusplus
}
#endif

#endif /* R_SAA71XX_MAIN_H_  */
