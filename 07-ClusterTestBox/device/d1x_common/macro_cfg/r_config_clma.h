/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_clma.h 7180 2016-01-07 17:45:54Z tobyas.hennig $
============================================================================ 
DESCRIPTION
CLMA configuration for D1x
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2015
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

********************************************************************************
*/

#ifndef CONFIG_CLMA_H_
#define CONFIG_CLMA_H_

/*******************************************************************************
  Title: CLMA device configuration
  
  Configuration of the device dependanant part of the CLMA macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_CLMA_MACRO_NUM

  Number of macros in the device.
*/

#define R_CLMA_MACRO_NUM         7u


/*******************************************************************************
  Constant: R_CLMA_BASE

   Base addresses for each of the CLMAn registers 
*/

#define R_CLMA0_BASE        0xFFF8C000u
#define R_CLMA1_BASE        0xFFF8F000u
#define R_CLMA2_BASE        0xFFF8F080u
#define R_CLMA3_BASE        0xFFF8F100u
#define R_CLMA4_BASE        0xFFF8F180u
#define R_CLMA5_BASE        0xFFF8F200u
#define R_CLMA6_BASE        0xFFF8F280u


/***********************************************************
  Macro: R_CLMA_WRITE_REG

  Write CLMAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_CLMA_REGS
    #define R_CLMA_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_CLMA_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_CLMA_READ_REG

  Read CLMAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_CLMA_REGS
    #define R_CLMA_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_CLMA_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


/*******************************************************************************
  Macro: R_DEV_WRITE_PROT_REG

  Write value to a protected register.
  
  Protected register are always 32bit wide 
*/
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

#ifdef R_DBG_PRINT_CLMA_REGS
#define R_CLMA_WRITE_PROT_REG(PROT_ADDR, ADDR, VALUE) {\
    uint8_t prot_stat = 0; \
    uint8_t new_val   = 0; \
  do { \
  *((volatile uint8_t*)(PROT_ADDR)) = (0xa5);\
  *((volatile uint8_t*)(ADDR))      = (VALUE);\
  *((volatile uint8_t*)(ADDR))      = (uint8_t)(~(VALUE));\
  *((volatile uint8_t*)(ADDR))      = (VALUE);\
  prot_stat = *((volatile uint8_t*)((PROT_ADDR) + 4u)); \
  new_val   = *((volatile uint82_t*)(ADDR)); \
} while ((new_val != (VALUE)) &&  (prot_stat != 0));\
R_DBG_PrintRegCmd(8, (ADDR), (VALUE));}
#else
#define R_CLMA_WRITE_PROT_REG(PROT_ADDR, ADDR, VALUE) {\
    uint8_t prot_stat = 0; \
    uint8_t new_val   = 0; \
    do { \
  *((volatile uint8_t*)(PROT_ADDR)) = (0xa5);\
  *((volatile uint8_t*)(ADDR))      = (VALUE);\
  *((volatile uint8_t*)(ADDR))      = (uint8_t)(~(VALUE));\
  *((volatile uint8_t*)(ADDR))      = (VALUE);\
  prot_stat = *((volatile uint8_t*)((PROT_ADDR) + 4u)); \
  new_val   = *((volatile uint8_t*)(ADDR)); \
} while ((new_val != (VALUE)) &&  (prot_stat != 0));}
#endif
#if defined (__ghs__)
#pragma ghs endnomisra
#endif

#endif /* CONFIG_CLMA_H_ */

