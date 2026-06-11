/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_xorchk.h 4314 2014-11-10 12:45:37Z golczewskim $
============================================================================ 
DESCRIPTION
XORCHK configuration for r7f701412
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

********************************************************************************
*/

#ifndef CONFIG_XORCHK_H_
#define CONFIG_XORCHK_H_

/* For Debug output */
#if defined (R_DBG_PRINT_XORCHK_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: XORCHK device configuration
  
  Configuration of the device dependanant part of the XORCHK macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_XORCHK_MACRO_NUM

  Number of macros in the device.
*/

#define R_XORCHK_MACRO_NUM         2u


typedef enum
{
    R_XORCHK_FUNC_TAUB1O9_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB1O7_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB1O5_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB1O3_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB1O1_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB0O13_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB0O11_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB0O9_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB0O7_XORUNIT = 0,
    R_XORCHK_FUNC_TAUB0O5_XORUNIT = 0,	
	R_XORCHK_FUNC_TAUB0O3_XORUNIT = 0,
	R_XORCHK_FUNC_TAUB0O1_XORUNIT = 0,
	
	R_XORCHK_FUNC_PCMP0BN1_XORUNIT = 1,
	R_XORCHK_FUNC_PCMP0BN0_XORUNIT = 1,
	R_XORCHK_FUNC_PCMP0AP0_XORUNIT = 1,
	R_XORCHK_FUNC_PCMP0AN0_XORUNIT = 1,
	R_XORCHK_FUNC_PCMP0AP1_XORUNIT = 1,
	R_XORCHK_FUNC_PCMP0BP1_XORUNIT = 1,
	R_XORCHK_FUNC_PCMP0BP0_XORUNIT = 1,
	R_XORCHK_FUNC_PCMP0AN1_XORUNIT = 1,

	R_XORCHK_FUNC_SG0AO_XORUNIT = 2,
	R_XORCHK_FUNC_SG0FAOL_XORUNIT = 2,
	R_XORCHK_FUNC_SG0FAO_XORUNIT = 2
} r_xorchk_unitno_of_function_t;

typedef enum
{
    R_XORCHK_FUNC_TAUB1O9_XORBIT = 12,
    R_XORCHK_FUNC_TAUB1O7_XORBIT = 11,
    R_XORCHK_FUNC_TAUB1O5_XORBIT = 10,
    R_XORCHK_FUNC_TAUB1O3_XORBIT = 9,
    R_XORCHK_FUNC_TAUB1O1_XORBIT = 8,
    R_XORCHK_FUNC_TAUB0O13_XORBIT = 6,
    R_XORCHK_FUNC_TAUB0O11_XORBIT = 5,
    R_XORCHK_FUNC_TAUB0O9_XORBIT = 4,
    R_XORCHK_FUNC_TAUB0O7_XORBIT = 3,
    R_XORCHK_FUNC_TAUB0O5_XORBIT = 2,	
	R_XORCHK_FUNC_TAUB0O3_XORBIT = 1,
	R_XORCHK_FUNC_TAUB0O1_XORBIT = 0,
	
	R_XORCHK_FUNC_PCMP0BN1_XORBIT = 7,
	R_XORCHK_FUNC_PCMP0BN0_XORBIT = 6,
	R_XORCHK_FUNC_PCMP0AP0_XORBIT = 5,
	R_XORCHK_FUNC_PCMP0AN0_XORBIT = 4,
	R_XORCHK_FUNC_PCMP0AP1_XORBIT = 3,
	R_XORCHK_FUNC_PCMP0BP1_XORBIT = 2,
	R_XORCHK_FUNC_PCMP0BP0_XORBIT = 1,
	R_XORCHK_FUNC_PCMP0AN1_XORBIT = 0,

	R_XORCHK_FUNC_SG0AO_XORBIT = 2,
	R_XORCHK_FUNC_SG0FAOL_XORBIT = 1,
	R_XORCHK_FUNC_SG0FAO_XORBIT = 0
} r_xorchk_bitpos_of_function_t;

typedef enum
{
	R_XORCHK_UNIT0_DNF = 9,
	R_XORCHK_UNIT1_DNF = 10,
	R_XORCHK_UNIT2_DNF = 10
} r_xorchk_unit_use_dnf_t;

//typedef enum
//{
//	R_XORCHK_UNIT0_DNF_CLK = R_DEV_CKS_TAUB0,
//	R_XORCHK_UNIT1_DNF_CLK = R_DEV_CKS_CLKFIX,
//	R_XORCHK_UNIT2_DNF_CLK = R_DEV_CKS_CLKFIX
//} r_xorchk_unit_use_dnf_clk_t;

//2.6.2.6 DNFAnEN — Digital Noise Elimination Enable Register
//This register can be read or written in 16-bit units.
//
//2.6.2.5 DNFAnCTL — Digital Noise Elimination Control Register
//This register can be read or written in 8-bit units.
//bit 2-0: prescaler of digital filter sampling clock
//bit 6-5: number of samples for valid pulse
//         (00b: 2 samples, 11b: 5 samples)


/*******************************************************************************
  Constant: R_XORCHK_BASE

   Base address for the macro registers 
*/

#define R_XORCHK0_BASE        0xFFF13000u
#define R_XORCHK1_BASE        0xFFF14000u

/* common base address of XORCHK0 and XORCHK1 clock register XORCHKCLKCFG */
#define R_XORCHK_COMMON_BASE  0xFFC06000u


/***********************************************************
  Macro: R_XORCHK_WRITE_REG

  Write XORCHKC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_XORCHK_REGS
    #define R_XORCHK_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_XORCHK_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_XORCHK_READ_REG

  Read XORCHKC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_XORCHK_REGS
    #define R_XORCHK_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_XORCHK_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* CONFIG_XORCHK_H_ */

