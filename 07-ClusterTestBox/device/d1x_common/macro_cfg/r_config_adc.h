/*
****************************************************************************
PROJECT : ADC
FILE    : $Id: r_config_adc.h 4366 2014-11-19 09:43:51Z golczewskim $
============================================================================ 
DESCRIPTION
ADC driver configuration for r7f701412
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

#ifndef CONFIG_ADC_H_
#define CONFIG_ADC_H_

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif


/*******************************************************************************
  Title: ADC Configuration of r7f701412
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_ADC_MACRO_NUM

  Number of ADC macros in the device.
*/

#define R_ADC_MACRO_NUM         2


/*******************************************************************************
  Constant: R_ADC0_MAX_CHANNEL_NUM
  
  The ADCA0 macro supports the following channels
 
  ADCE Io 0 - 20
  ADCE Io 36
  ADCE Io 37

  Constant: R_ADC1_MAX_CHANNEL_NUM
  
*/

#define R_ADC0_MAX_CHANNEL_NUM    38
#define R_ADC1_MAX_CHANNEL_NUM    0

#define R_ADC_MAX_CHANNEL_NUM(n)  (R_ADC##n##_MAX_CHANNEL_NUM)


/*******************************************************************************
  Constant: R_ADC_CHANNEL_MAX_NUM
  
  The maximal supported channels of ADCA0 and ADCA1,
  the realy used number is determined by the device 
*/

#define R_ADC_CHANNEL_MAX_NUM     38


/*******************************************************************************
  Constant: R_ADC0_CG_HW_TRIG_NUM

  Number of ADC0 hardware triggers for each conversion group.


  Constant: R_ADC1_CG_HW_TRIG_NUM

  Number of ADC1 hardware triggers for each conversion group.
*/

#define R_ADC0_CG_HW_TRIG_NUM       16
#define R_ADC1_CG_HW_TRIG_NUM       0

#define R_ADC_CG_HW_TRIG_NUM(n)    (R_ADC##n##_CG_HW_TRIG_NUM)


/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_ADC_WRITE_REG

  Write ADC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_DEV_REGS
    #define R_ADC_WRITE_REG(SIZE, ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)
#else
    #define R_ADC_WRITE_REG(SIZE, ADDR, VALUE)  (*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE))
#endif



/*******************************************************************************
  Macro: R_ADC_READ_REG

  Read ADC register register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DEV_REGS
    #define R_ADC_READ_REG(SIZE, ADDR) R_DBG_PrintRegRead##SIZE##(ADDR)
#else
    #define R_ADC_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

/*******************************************************************************
  Type: r_adc_HWTrigger_t

  Hardware Trigger for ADCA 

SCAN group  Triger select 1
    T1SEL0 ADCE0TRGI1 External trigger pin
    T1SEL1 INTP0 External interrupt pin
    T1SEL2 INTP1
    T1SEL3 INTP2
    T1SEL4 INTTAUB0I2 TAUB0
    T1SEL5 INTTAUB0I4
    T1SEL6 INTTAUB0I6
    T1SEL7 INTTAUB0I8
    T1SEL8 INTTAUB1I2 TAUB1
    T1SEL9 INTTAUB1I4
    T1SEL10 INTTAUB1I6
    T1SEL11 INTTAUB1I8
    T1SEL12 INTTAUB2I0 TAUB2
    T1SEL13 INTTAUB2I1
    T1SEL14 INTTAUB2I2
    T1SEL15 INTTAUB2I3

SCAN group  Triger select 2
    T2SEL0 ADCE0TRGI2 External trigger pin
    T2SEL1 INTP0 External interrupt pin
    T2SEL2 INTP1
    T2SEL3 INTP2
    T2SEL4 INTTAUB0I2 TAUB0
    T2SEL5 INTTAUB0I4
    T2SEL6 INTTAUB0I6
    T2SEL7 INTTAUB0I8
    T2SEL8 INTTAUB0I10
    T2SEL9 INTTAUB0I12
    T2SEL10 INTTAUB0I14
    T2SEL11 INTTAUB1I2 TAUB1
    T2SEL12 INTTAUB1I4
    T2SEL13 INTTAUB1I6
    T2SEL14 INTTAUB1I8
    T2SEL15 INTTAUB1I10

SCAN group  Triger select 3
    T3SEL0 ADCE0TRGI3 External trigger pin
    T3SEL1 INTP0 External interrupt pin   
    T3SEL2 INTP1                          
    T3SEL3 INTP2                          
    T3SEL4 INTTAUB0I2 TAUB0               
    T3SEL5 INTTAUB0I4                     
    T3SEL6 INTTAUB0I6                     
    T3SEL7 INTTAUB0I8                     
    T3SEL8 INTTAUB1I2 TAUB1               
    T3SEL9 INTTAUB1I4                     
    T3SEL10 INTTAUB1I6                    
    T3SEL11 INTTAUB1I8                    
    T3SEL12 INTTAUB2I0 TAUB2              
    T3SEL13 INTTAUB2I1                    
    T3SEL14 INTTAUB2I2                    
    T3SEL15 INTTAUB2I3                    
               
TSN group (temperature sensor)
    TTSNSEL0 — not connected
    TTSNSEL1 —
    TTSNSEL2 —
    TTSNSEL3 —
    TTSNSEL4 INTTAUB0I2 TAUB0
    TTSNSEL5 INTTAUB0I4
    TTSNSEL6 INTTAUB0I6
    TTSNSEL7 INTTAUB0I8
    TTSNSEL8 INTTAUB1I2 TAUB1
    TTSNSEL9 INTTAUB1I4
    TTSNSEL10 INTTAUB1I6
    TTSNSEL11 INTTAUB1I8
    TTSNSEL12 INTTAUB2I0 TAUB2
    TTSNSEL13 INTTAUB2I1
    TTSNSEL14 INTTAUB2I2
    TTSNSEL15 INTTAUB2I3
*/
 
   
typedef enum
{
    R_ADC_TRG_SEL0 = 0,
    R_ADC_TRG_SEL1,
    R_ADC_TRG_SEL2,
    R_ADC_TRG_SEL3,
    R_ADC_TRG_SEL4,
    R_ADC_TRG_SEL5,
    R_ADC_TRG_SEL6,
    R_ADC_TRG_SEL7,
    R_ADC_TRG_SEL8,
    R_ADC_TRG_SEL9,
    R_ADC_TRG_SEL10,
    R_ADC_TRG_SEL11,
    R_ADC_TRG_SEL12,
    R_ADC_TRG_SEL13,
    R_ADC_TRG_SEL14,
    R_ADC_TRG_SEL15
} r_adc_HWTrigger_t;

#endif /* CONFIG_ADC_H_ */
