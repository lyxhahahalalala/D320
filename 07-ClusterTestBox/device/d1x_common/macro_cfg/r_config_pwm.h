/*
****************************************************************************
PROJECT : PWM
FILE    : $Id: r_config_pwm.h 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
PWM configuration for r7f701412
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

#ifndef _CONFIG_PWM_H_
#define _CONFIG_PWM_H_

/*******************************************************************************
  Title: PWM Configuration of r7f701412
*/

/*******************************************************************************
  Section: Global Defines
*/

#define R_PWM_PWBA0                0xFFE45800
#define R_PWM_PWSA0                0xFFE45A00
#define R_PWM_PWGA0                0xFFE44000
#define R_PWM_SLPWGA0              0xFFE45900

/*******************************************************************************
  Section: Global Constants
*/

/*******************************************************************************
  Constant: R_PWM_PWBA_NUM

  Number of Unit PWBA in the device.
*/

#define R_PWM_PWBA_NUM             1

/*******************************************************************************
  Constant: R_PWM_PWGA_NUM

  Number of Unit PWGA in the device.
*/

#define R_PWM_PWGA_NUM             24

/*******************************************************************************
  Constant: R_PWM_PWSA_NUM

  Number of Unit PWSA in the device.
*/

#define R_PWM_PWSA_NUM             1

/*******************************************************************************
  Constant: R_PWM_MACRO_NUM

  Number of PWM Macro in the device.
*/

#define R_PWM_MACRO_NUM            1

/*******************************************************************************
  Constant: R_PWM_ADC0PhyCh_MAX_NUM

  Number of PWM related ADC0 Physical Channels in the device.
*/

#define R_PWM_ADC0PhyCh_MAX_NUM    36

/*******************************************************************************
  Constant: R_PWM_ADC1PhyCh_MAX_NUM

  Number of PWM related ADC1 Physical Channels in the device.
*/

#define R_PWM_ADC1PhyCh_MAX_NUM    0

/*******************************************************************************
  Constant: R_PWM_ADCMACRO_NUM

  Number of PWM related ADC macros in the device.
*/

#define R_PWM_ADCMACRO_NUM         1

/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_PWM_WRITE_REG

  Write PWM register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_PWM_REGS
    #define R_PWM_WRITE_REG(SIZE, ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)
#else
    #define R_PWM_WRITE_REG(SIZE, ADDR, VALUE)  (*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE))
#endif



/*******************************************************************************
  Macro: R_PWM_READ_REG

  Read PWM register register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_PWM_REGS
    #define R_PWM_READ_REG(SIZE, ADDR) R_DBG_PrintRegRead##SIZE##(ADDR)
#else
    #define R_PWM_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* _CONFIG_PWM_H_ */
