/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_ecm.h 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
ECM configuration for r7f701412
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

********************************************************************************
*/

#ifndef CONFIG_ECM_H_
#define CONFIG_ECM_H_

/*******************************************************************************
  Title: ECM device configuration
  
  Configuration of the device dependanant part of the ECM macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_ECM_MACRO_NUM

  Number of macros in the device.
*/

#define R_ECM_MACRO_NUM         1u



/*******************************************************************************
  Constant: R_ECM_BASE

   Base address for the macro registers 
*/

#define R_ECM_BASE        0xffcb0000u



/***********************************************************
  Macro: R_ECM_WRITE_REG

  Write ECMC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_ECM_REGS
    #define R_ECM_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_ECM_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_ECM_READ_REG

  Read ECMC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_ECM_REGS
    #define R_ECM_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_ECM_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


/***********************************************************
  Macro: R_ECM_WRITE_PROT_REG

  Write protected ECMC register with 32 Bit size.
*/

#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

#ifdef R_DBG_PRINT_ECM_REGS
#define R_ECM_WRITE_PROT_REG(PROT_ADDR, ADDR, VALUE) { \
  *((volatile uint32_t*)(PROT_ADDR)) = (0x000000a5);\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  *((volatile uint32_t*)(ADDR))      = (uint32_t)(~(VALUE));\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
R_DBG_PrintRegCmd(32, (ADDR), (VALUE));}
#else
#define R_ECM_WRITE_PROT_REG(PROT_ADDR, ADDR, VALUE) { \
  *((volatile uint32_t*)(PROT_ADDR)) = (0x000000a5);\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  *((volatile uint32_t*)(ADDR))      = (uint32_t)(~(VALUE));\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
}
#endif

#if defined (__ghs__)
#pragma ghs endnomisra
#endif

#endif /* CONFIG_ECM_H_ */

