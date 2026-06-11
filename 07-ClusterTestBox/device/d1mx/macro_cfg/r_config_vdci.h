/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_vdci.h 2566 2014-05-06 11:49:17Z tobyas.hennig $
============================================================================ 
DESCRIPTION
VDCI configuration for r7f701412
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

#ifndef CONFIG_VDCI_H_
#define CONFIG_VDCI_H_

#if defined (__ghs__)
#pragma ghs startnomisra
#endif

/***********************************************************
  Title: VDCI device configuration
  
  Configuration of the device dependanant part of the VDCI macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_VDCI_MACRO_NUM

  Number of macros in the device.
*/

#define R_VDCI_MACRO_NUM         2u


/*******************************************************************************
    Constant: R_VDCI_0_BASE, R_VDCI_1_BASE

    Base address of the VDCO macro Units in the device.
*/
#define R_VDCI_0_BASE       0xF2004000u
#define R_VDCI_1_BASE       0xF2006000u


/*******************************************************************************
    Constant: R_VDCI_CHANNEL_COUNT

    Define the maimum number of write channels per unit.
*/
#define R_VDCI_WRITE_CHANNEL_COUNT 2

/*******************************************************************************
  Constant: R_VDCI_RESOLUTION_*

  min and max resolution setting for both axis

*/
#define R_VDCI_RESOLUTION_HMIN  16u
#define R_VDCI_RESOLUTION_HMAX  1024u
#define R_VDCI_RESOLUTION_VMIN  16u
#define R_VDCI_RESOLUTION_VMAX  1024u

#define R_VDCI_RESOLUTION_VTOTAL_MIN 1u
#define R_VDCI_RESOLUTION_VTOTAL_MAX 2048u
#define R_VDCI_RESOLUTION_HTOTAL_MIN 1u
#define R_VDCI_RESOLUTION_HTOTAL_MAX 2048u

/*******************************************************************************
    Constant: R_VDCI_BURST_MODE

    defines interpolation mode for the lower bits of a color component 

    0 - VDCO uses 4 beat burst
    1 - VDCO uses 16 beat burst
*/
#define R_VDCI_BURST_MODE         1u



/*******************************************************************************
    Constant: R_VDCI_LOWERBIT_COLOR_INTERPOLATION_MODE

    defines interpolation mode for the lower bits of a color component 

    0 - Lower bit interpolation mode: 
         Interpolates deficient bits from the higher bits of RGB data.
    1 - Lower bit-fixed mode: Fixes deficient bits to 0.
*/
#define R_VDCI_LOWERBIT_COLOR_INTERPOLATION_MODE         0u

/*******************************************************************************
    Constant: R_VDCI_YC_CONVERSION_MODE

    defines YC to RGB conversion mode

    0 - Y and C range 0 .. 255
    1 - Y range 16 .. 235, C range 16 .. 240
*/
#define R_VDCI_YC_CONVERSION_MODE         0u



/*******************************************************************************
    Constant: R_VDCI_ENDIAN_CONVERSION

    default endian conversion for reading VRAM 
    is little endian R_VDCI_ENDIAN_LITTLE.

    see also <r_vdci_Endian_t>.
*/
#if 0
#define R_VDCI_ENDIAN_CONVERSION         R_VDCI_ENDIAN_LITTLE
#endif



/*******************************************************************************
    Constant: R_VDCI_n_INTERRUPT_FACTOR_i

    define of interrupt factor for all interrupt events.
    
    0     - falling edge
    1     - rising edge
    2 & 3 - both edges

    interrupt event i:
    0 - V blank
    1 - VSync
    2 - H blank
    3 - ext sync error
    4 - display underrun
    5 - NA
    6 - data bus time out (For this event, only the rising edge setting makes sense)

    Note:
    keep VBlank interrupt always enabled with falling edge, the driver needs this interrupt.
    
*/
#define R_VDCI_0_INTERRUPT_FACTOR_0     0 /* keep 0 for falling edge */
#define R_VDCI_0_INTERRUPT_FACTOR_1     0
#define R_VDCI_0_INTERRUPT_FACTOR_2     0
#define R_VDCI_0_INTERRUPT_FACTOR_3     0
#define R_VDCI_0_INTERRUPT_FACTOR_4     0
#define R_VDCI_0_INTERRUPT_FACTOR_6     1 

#define R_VDCI_1_INTERRUPT_FACTOR_0     0 /* keep 0 for falling edge */
#define R_VDCI_1_INTERRUPT_FACTOR_1     0
#define R_VDCI_1_INTERRUPT_FACTOR_2     0
#define R_VDCI_1_INTERRUPT_FACTOR_3     0
#define R_VDCI_1_INTERRUPT_FACTOR_4     0
#define R_VDCI_1_INTERRUPT_FACTOR_6     1 


/*******************************************************************************
Section: Global Defines for register access
************************************************************/


/*******************************************************************************
  Macro: R_VDCI_WRITE8

  Write VDCO register 8 bit.
*/

#define R_VDCI_WRITE8(ADDR, VALUE)      R_DEV_WRITE_REG(8, (ADDR), (VALUE))


/*******************************************************************************
  Macro: R_VDCI_WRITE16

  Write VDCO register 16 bit.
*/

#define R_VDCI_WRITE16(ADDR, VALUE)      R_DEV_WRITE_REG(16, (ADDR), (VALUE))

/*******************************************************************************
  Macro: R_VDCI_WRITE32

  Write VDCO register 32 bit.
*/

#define R_VDCI_WRITE32(ADDR, VALUE)      R_DEV_WRITE_REG(32, (ADDR), (VALUE))


/*******************************************************************************
  Macro: R_VDCI_READ8

  Read VDCO register 8 bit.
*/

#define R_VDCI_READ8(ADDR)           R_DEV_READ_REG(8, (ADDR))

/*******************************************************************************
  Macro: R_VDCI_READ16

  Read VDCO register 16 bit.
*/

#define R_VDCI_READ16(ADDR)           R_DEV_READ_REG(16, (ADDR))

/*******************************************************************************
  Macro: R_VDCI_READ32

  Read VDCO register 32 bit.
*/

#define R_VDCI_READ32(ADDR)           R_DEV_READ_REG(32, (ADDR))

#if defined (__ghs__)
#pragma ghs endnomisra
#endif

#endif /* CONFIG_VDCI_H_ */
