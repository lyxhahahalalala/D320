/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_XCG.h 1455 2013-09-26 15:10:49Z michael.golczewski $
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

#ifndef CONFIG_XCG_H_
#define CONFIG_XCG_H_

#if defined (R_DBG_PRINT_XCG_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: XCG device configuration
  
  Configuration of the device dependanant part of the XCG macro 
*/


/*
Slaves                                                        Masters

                CPU Subsystem
                CPU I/F Global I/F                                                                                                  Sprite Engine
                CPU     DMA     OCD     GPU2D   MLBB    ETNB    JCUA    RLE     SP0     SP1     SP2
                MSTID1  MSTID2  MSTID0  MSTID4  MSTID7  MSTID6  MSTID5  MSTID8  MSTID9  MSTID10 MSTID11

LRAM/CodeFlash  –       –       –       R       R/W     R       R       R       R       R       R
RRAM            R/W     R/W     R/W     –       –       –       –       –       –       –       –
SFMA            R       R       R       R       –       –       R       R       R       R       R
VRAM0           R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W
VRAM1           R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W     R/W
SDRB    Port 0  –       –       –       –       –       –       –       R/W     R/W     –       –
        Port 1  –       –       –       –       –       –       –       –       –       R/W     R/W
        Port 2  R/W     R/W     R/W     –       R/W     R/W     R/W     –       –       –       –
        Port 3  –       –       –       R/W     –       –       –       –       –       -       -
Mod. registers  R/W     –       –       –       –       –       –       –       –       –       –
*/



/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_XCG_MACRO_NUM

  Number of macros in the device.
*/

#define R_XCG_MACRO_NUM          9u


/*******************************************************************************
  Constant: R_XCG_WINDOW_NUM

  Number of guard windows in the macro.
*/

#define R_XCG_WINDOW_NUM         8u


/*******************************************************************************
  Constant: R_XCG_MASTERS_NUM

  Number of masters that can be configured per window.
*/

#define R_XCG_MASTERS_NUM        12u


/*******************************************************************************
  Constant: R_XCG_BASE

   Base address for the macro registers 
*/

/* XCG3   Retention RAM (RRAM) */
#define R_XCG_BASE_0      (0xFFFC0000u)
/* XCG4   Video RAM 0 (VRAM0) */
#define R_XCG_BASE_1      (0xFFFC1000u)
/* XCG5   Video RAM 1 (VRAM1) */
#define R_XCG_BASE_2      (0xFFFC2000u)
/* XCG6   Serial Flash Memory I/F (SFMA) */
#define R_XCG_BASE_3      (0xFFFC3000u)
/* XCG7   DDR2-SDRAM Memory Controller (SDRB) port 3 (2DGPU) */
#define R_XCG_BASE_4      (0xFFFC4000u)
/* XCG8   Cross-connect XC1 module registers bridge */
#define R_XCG_BASE_5      (0xFFFC6000u)
/* XCG9   DDR2-SDRAM Memory Controller (SDRB) port 2 (XC1) */
#define R_XCG_BASE_6      (0xFFFC4400u)
/* XCG10  DDR2-SDRAM Memory Controller (SDRB) port 0 (XC0_0) */
#define R_XCG_BASE_7      (0xFFFC4800u)
/* XCG11  DDR2-SDRAM Memory Controller (SDRB) port 1 (XC0_1) */
#define R_XCG_BASE_8      (0xFFFC4C00u)


/*******************************************************************************
  Constant: R_XCG_WINDOW_GRANULARITY

  The window size must be a multiple of this value.
  A 2^n value is required here.
  
  0x00001000: Granularity of 4k.
*/

#define R_XCG_WINDOW_GRANULARITY        (0x1000u)


/*******************************************************************************
  Constant: R_XCG_WINDOW_GRANULARITY_MASK

  This constant defines all bits that are valid to be used for the guard window configuration.
  A 2^n-1 value is required here.
  
  0xFFFFF000: Granularity of 4k.
*/
#define R_XCG_WINDOW_GRANULARITY_VALID_MASK      (~(R_XCG_WINDOW_GRANULARITY-1ul))
#define R_XCG_WINDOW_GRANULARITY_INVALID_MASK    ( (R_XCG_WINDOW_GRANULARITY-1ul))




/***********************************************************
  Macro: R_XCG_WRITE_REG

  Write XCGC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_XCG_REGS
    #define R_XCG_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_XCG_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_XCG_READ_REG

  Read XCG register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_XCG_REGS
    #define R_XCG_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_XCG_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

/*******************************************************************************
  Macro: R_XCG_WRITE32

  Write XCG register 32 bit.
*/

#ifdef R_DBG_PRINT_XCG_REGS
    #define R_XCG_WRITE32(ADDR, VALUE)  {R_DBG_PrintRegCmd(32, (ADDR), (VALUE));  *((volatile uint32_t*)(ADDR)) = (VALUE);}
#else
    #define R_XCG_WRITE32(ADDR, VALUE)  {(*((volatile uint32_t*)(ADDR)) = (VALUE));}
#endif


/*******************************************************************************
  Macro: R_XCG_READ32

  Read XCG register 32 bit.
*/

#ifdef R_DBG_PRINT_XCG_REGS
    #define R_XCG_READ32(ADDR)   (R_DBG_PrintRegRead32((ADDR)))
#else
    #define R_XCG_READ32(ADDR)   ((*((volatile uint32_t*)((ADDR)))))
#endif

#endif /* CONFIG_XCG_H_ */


