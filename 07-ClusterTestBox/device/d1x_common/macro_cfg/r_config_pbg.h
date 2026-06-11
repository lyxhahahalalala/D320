/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_pbg.h 7180 2016-01-07 17:45:54Z tobyas.hennig $
============================================================================ 
DESCRIPTION
XC GUARD configuration for D1x
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

#ifndef CONFIG_PBG_H_
#define CONFIG_PBG_H_

#if defined (R_DBG_PRINT_PBG_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: PBG device configuration
  
  Configuration of the device dependanant part of the PBG macro 
*/




/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_PBG_MACRO_NUM

  Number of macros in the device.
  This is handled via <vlib:/device/<device>/lib/r_dev_guard_types.h>
*/

#define R_PBG_MACRO_NUM          (R_DEV_PBG_NUM)

/*******************************************************************************
  Constant: R_PBG_DEFAULT_SETTING

  The default setting of a PBus Guard
*/

#define R_PBG_DEFAULT_SETTING    (0x07ffffffu)


/***********************************************************
  Macro: R_PBG_WRITE_REG

  Write PBGC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_PBG_REGS
    #define R_PBG_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_PBG_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_PBG_READ_REG

  Read PBG register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_PBG_REGS
    #define R_PBG_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_PBG_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


/*******************************************************************************
  Macro: R_PBG_WRITE32

  Write PBG register 32 bit.
*/

#ifdef R_DBG_PRINT_PBG_REGS
    #define R_PBG_WRITE32(ADDR, VALUE)  {R_DBG_PrintRegCmd(32, (ADDR), (VALUE));  *((volatile uint32_t*)(ADDR)) = (VALUE);}
#else
    #define R_PBG_WRITE32(ADDR, VALUE)  {(*((volatile uint32_t*)(ADDR)) = (VALUE));}
#endif


/*******************************************************************************
  Macro: R_PBG_READ32

  Read PBG register 32 bit.
*/

#ifdef R_DBG_PRINT_PBG_REGS
    #define R_PBG_READ32(ADDR)   (R_DBG_PrintRegRead32((ADDR)))
#else
    #define R_PBG_READ32(ADDR)   ((*((volatile uint32_t*)((ADDR)))))
#endif


#endif /* CONFIG_PBG_H_ */

