/*
****************************************************************************
PROJECT : RIIC driver
FILE    : $Id: r_riic_main.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for RIIC macro
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


#ifndef R_RIIC_MAIN_H_
#define R_RIIC_MAIN_H_

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

#undef R_EXTERN
#ifndef RIIC_MAIN_GLOBALS
#define R_EXTERN extern 
#else
#define R_EXTERN
#endif

/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: r_riic_Dev_t

  Device instance specific data.
  
  All instance specific data is kept within that structure.

  Members:
  BaseAddr      - Basis address
  
*/

typedef struct 
{
    uint32_t  BaseAddr;
} r_riic_Dev_t;


/*******************************************************************************
  Variable: r_riic_Dev

  Array for device instance specific data.
*/

R_EXTERN r_riic_Dev_t r_riic_Dev[R_RIIC_MACRO_NUM];


/*******************************************************************************
  Section: Global Functions
*/

#ifdef __cplusplus
}
#endif

#endif /* R_RIIC_MAIN_H_  */
