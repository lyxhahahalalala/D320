/*
****************************************************************************
PROJECT : SFMA
FILE    : $Id: r_config_sfma_ree.h 3813 2014-09-11 13:35:49Z tobyas.hennig $
============================================================================ 
DESCRIPTION
SFMA configuration for r7f701412
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

#ifndef CONFIG_SFMA_REE_H_
#define CONFIG_SFMA_REE_H_

#if defined (R_DBG_PRINT_SFMA_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: SFMA Configuration
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_SFMA_MACRO_NUM

  Number of SFMA macros in the device.
*/

#define R_SFMA_MACRO_NUM         1u

/***************************************************************************
 Constant: R_SFMA_BASE_ADD

 Define a base address value.
*/
#define R_SFMA_BASE_ADD             (0xF2FF0000u)

/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_SFMA_WRITE_REG

  Write SFMA register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_SFMA_REGS
    #define R_SFMA_WRITE_REG(SIZE, ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)
#else
    #define R_SFMA_WRITE_REG(SIZE, ADDR, VALUE)  (*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE))
#endif



/*******************************************************************************
  Macro: R_SFMA_READ_REG

  Read SFMA register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_SFMA_REGS
    #define R_SFMA_READ_REG(SIZE, ADDR) R_DBG_PrintRegRead##SIZE##(ADDR)
#else
    #define R_SFMA_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* CONFIG_SFMA_REE_H_ */
