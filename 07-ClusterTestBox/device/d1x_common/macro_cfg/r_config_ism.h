/*
****************************************************************************
PROJECT : ISM
FILE    : $Id: r_config_ism.h 4975 2015-03-27 16:22:34Z tobyas.hennig $
============================================================================ 
DESCRIPTION
ISM configuration for r7f701412
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

****************************************************************************
*/

#ifndef CONFIG_ISM_H_
#define CONFIG_ISM_H_

#if defined (R_DBG_PRINT_ISM_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: ISM Configuration
*/


/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_ISM_MACRO_NUM

  Number of ISM macros in the device.
*/

#define R_ISM_MACRO_NUM         1


/*******************************************************************************
  Constant: R_ISM_MACRO_BASEn

  Register Base Address of ISM macros in the device.
*/

#define R_ISM_MACRO_BASE0      ( 0xFFF00000u )

/* ISM OPERATION PROPERTIES */


#define R_ISM_PWMTABLEOFFSET  ( 0x00004000 )
#define R_ISM_ZPDTABLEOFFSET  ( 0x00007000 )

#define EIC165           (CAST_US(0xffff614a))
#define EIC166           (CAST_US(0xffff614c))
#define EIC167           (CAST_US(0xffff614e))
#define EIC168           (CAST_US(0xffff6150))

#define R_ISM_CLOCK_HZ      ( ( uint32_t )R_SYS_ISM_SysGetClockHz(R_ISM_UNIT) )


/* Soft reset settings */

#define R_ISM_RECOVERYDELAY     ( 10000L )   /* must be at least one PWM period */
#define R_ISM_SWRESET_PDISABLE  0            /* ports are not disabled on s-reset */

/***********************************************************
  Constants: Interrupt and Clocking Registers
*/

#define CAST_UC(x) (*((volatile unsigned char  *)(x)))
#define CAST_US(x) (*((volatile unsigned short *)(x)))
#define CAST_UL(x) (*((volatile unsigned long  *)(x)))

/* INTERRUPT REGISTER MAPPING */

#define R_ISM_INTM0DONE            EIC166
#define R_ISM_INTM0REACHED         EIC165
#define R_ISM_INTM0ZPDAD           EIC167
#define R_ISM_INTM0ZPD             EIC168
#define R_ISM_INTCLEAR             0x008F

/* port setup of ISM function */
#define R_ISM_PORT_SINPFB0       16u
#define R_ISM_PORT_SINMFB0       16u
#define R_ISM_PORT_COSPFB0       16u
#define R_ISM_PORT_COSMFB0       16u
#define R_ISM_PORT_SINPFB1       16u
#define R_ISM_PORT_SINMFB1       16u
#define R_ISM_PORT_COSPFB1       16u
#define R_ISM_PORT_COSMFB1       16u
#define R_ISM_PORT_SINPFB2       17u
#define R_ISM_PORT_SINMFB2       17u
#define R_ISM_PORT_COSPFB2       17u
#define R_ISM_PORT_COSMFB2       17u
#define R_ISM_PORT_SINPFB3       17u
#define R_ISM_PORT_SINMFB3       17u
#define R_ISM_PORT_COSPFB3       17u
#define R_ISM_PORT_COSMFB3       17u
#define R_ISM_PORT_SINPFB4       16u
#define R_ISM_PORT_SINMFB4       16u
#define R_ISM_PORT_COSPFB4       16u
#define R_ISM_PORT_COSMFB4       16u
#define R_ISM_PORT_SINPFB5       17u
#define R_ISM_PORT_SINMFB5       17u
#define R_ISM_PORT_COSPFB5       17u
#define R_ISM_PORT_COSMFB5       17u

/* pin setup of ISM function */
#define R_ISM_PORT_BIT_SINPFB0   (uint16_t)(1<<0)
#define R_ISM_PORT_BIT_SINMFB0   (uint16_t)(1<<1)
#define R_ISM_PORT_BIT_COSPFB0   (uint16_t)(1<<2)
#define R_ISM_PORT_BIT_COSMFB0   (uint16_t)(1<<3)
#define R_ISM_PORT_BIT_SINPFB1   (uint16_t)(1<<4)
#define R_ISM_PORT_BIT_SINMFB1   (uint16_t)(1<<5)
#define R_ISM_PORT_BIT_COSPFB1   (uint16_t)(1<<6)
#define R_ISM_PORT_BIT_COSMFB1   (uint16_t)(1<<7)
#define R_ISM_PORT_BIT_SINPFB2   (uint16_t)(1<<4)
#define R_ISM_PORT_BIT_SINMFB2   (uint16_t)(1<<5)
#define R_ISM_PORT_BIT_COSPFB2   (uint16_t)(1<<6)
#define R_ISM_PORT_BIT_COSMFB2   (uint16_t)(1<<7)
#define R_ISM_PORT_BIT_SINPFB3   (uint16_t)(1<<8)
#define R_ISM_PORT_BIT_SINMFB3   (uint16_t)(1<<9)
#define R_ISM_PORT_BIT_COSPFB3   (uint16_t)(1<<10)
#define R_ISM_PORT_BIT_COSMFB3   (uint16_t)(1<<11)
#define R_ISM_PORT_BIT_SINPFB4   (uint16_t)(1<<8)
#define R_ISM_PORT_BIT_SINMFB4   (uint16_t)(1<<9)
#define R_ISM_PORT_BIT_COSPFB4   (uint16_t)(1<<10)
#define R_ISM_PORT_BIT_COSMFB4   (uint16_t)(1<<11)
#define R_ISM_PORT_BIT_SINPFB5   (uint16_t)(1<<0)
#define R_ISM_PORT_BIT_SINMFB5   (uint16_t)(1<<1)
#define R_ISM_PORT_BIT_COSPFB5   (uint16_t)(1<<2)
#define R_ISM_PORT_BIT_COSMFB5   (uint16_t)(1<<3)

/* alternative function setup of ISM function */
#define R_ISM_PORT_FUNC_SINPFB0  1u
#define R_ISM_PORT_FUNC_SINMFB0  1u
#define R_ISM_PORT_FUNC_COSPFB0  1u
#define R_ISM_PORT_FUNC_COSMFB0  1u
#define R_ISM_PORT_FUNC_SINPFB1  1u
#define R_ISM_PORT_FUNC_SINMFB1  1u
#define R_ISM_PORT_FUNC_COSPFB1  1u
#define R_ISM_PORT_FUNC_COSMFB1  1u
#define R_ISM_PORT_FUNC_SINPFB2  1u
#define R_ISM_PORT_FUNC_SINMFB2  1u
#define R_ISM_PORT_FUNC_COSPFB2  1u
#define R_ISM_PORT_FUNC_COSMFB2  1u
#define R_ISM_PORT_FUNC_SINPFB3  1u
#define R_ISM_PORT_FUNC_SINMFB3  1u
#define R_ISM_PORT_FUNC_COSPFB3  1u
#define R_ISM_PORT_FUNC_COSMFB3  1u
#define R_ISM_PORT_FUNC_SINPFB4  1u
#define R_ISM_PORT_FUNC_SINMFB4  1u
#define R_ISM_PORT_FUNC_COSPFB4  1u
#define R_ISM_PORT_FUNC_COSMFB4  1u
#define R_ISM_PORT_FUNC_SINPFB5  1u
#define R_ISM_PORT_FUNC_SINMFB5  1u
#define R_ISM_PORT_FUNC_COSPFB5  1u
#define R_ISM_PORT_FUNC_COSMFB5  1u


/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_ISM_WRITE_REG

  Write ISM register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_ISM_REGS
    #define R_ISM_WRITE_REG(SIZE, ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)
#else
    #define R_ISM_WRITE_REG(SIZE, ADDR, VALUE)  (*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE))
#endif

#define R_ISM_WRITE_REG_SILENT(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE));}


/*******************************************************************************
  Macro: R_ISM_READ_REG

  Read ISM register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_ISM_REGS
    #define R_ISM_READ_REG(SIZE, ADDR) R_DBG_PrintRegRead##SIZE##(ADDR)
#else
    #define R_ISM_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


#endif /* CONFIG_ISM_H_ */
