/*
****************************************************************************
PROJECT : VDCO driver
FILE    : $Id: r_config_vdco.h 3879 2014-09-17 08:18:24Z matthias.nippert $
============================================================================ 
DESCRIPTION
VDCO Configuration for D1M2(H)
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

****************************************************************************
*/

#ifndef CONFIG_VDCO_H_
#define CONFIG_VDCO_H_

#if defined (R_DBG_PRINT_VDCO_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: VDCO Configuration for D1M2(H)
  
  VDCO Configuration
*/

/*******************************************************************************
  Section: Global Defines
************************************************************/


/*******************************************************************************
    Constant: R_VDCO_MACRO_NUM

    Number of VDCO macros in the device.

*/
#define R_VDCO_MACRO_NUM         2u


/*******************************************************************************
    Constant: R_VDCO_0_BASE, R_VDCO_1_BASE

    Base address of the VDCO macro Units in the device.
*/
#define R_VDCO_0_BASE       0xF2004000u
#define R_VDCO_1_BASE       0xF2006000u


/*******************************************************************************
  Constant: R_VDCO_RESOLUTION_*

  min and max resolution setting for both axis

*/
#define R_VDCO_RESOLUTION_HMIN  120u
#define R_VDCO_RESOLUTION_HMAX  1280u
#define R_VDCO_RESOLUTION_VMIN  120u
/* MNI: Changed Maximum resolution for the compatibility of the VOWE timing fix.
        As the timing fix expands the vertical resolution, setting 1024 would be 
        expanded to >1024. As the VDCO internally supports 2048, VMAX can be increased
        to include the margin for the VOWE timing fix. Mantis #2325. */
#define R_VDCO_RESOLUTION_VMAX  (1024u+16u)

#define R_VDCO_RESOLUTION_VTOTAL_MIN 1u
#define R_VDCO_RESOLUTION_VTOTAL_MAX 2048u
#define R_VDCO_RESOLUTION_HTOTAL_MIN 1u
#define R_VDCO_RESOLUTION_HTOTAL_MAX 2048u


/*******************************************************************************
  Constant: R_VDCO_LAYER_DISPLAY_MAX_*
  
  R_VDCO_LAYER_DISPLAY_MAX_WIDTH    - max V resolution
  R_VDCO_LAYER_DISPLAY_MAX_HEIGHT   - max H resolution
*/
#define R_VDCO_LAYER_DISPLAY_MAX_WIDTH         1280u
#define R_VDCO_LAYER_DISPLAY_MAX_HEIGHT        1024u

/*******************************************************************************
    Constant: R_VDCO_BURST_MODE

    defines interpolation mode for the lower bits of a color component 

    0 - VDCO uses 4 beat burst
    1 - VDCO uses 16 beat burst
*/
#define R_VDCO_BURST_MODE         1u


/*******************************************************************************
    Constant: R_VDCO_LOWERBIT_COLOR_INTERPOLATION_MODE

    defines interpolation mode for the lower bits of a color component 

    0 - Lower bit interpolation mode: 
         Interpolates deficient bits from the higher bits of RGB data.
    1 - Lower bit-fixed mode: Fixes deficient bits to 0.
*/
#define R_VDCO_LOWERBIT_COLOR_INTERPOLATION_MODE         0u

/*******************************************************************************
    Constant: R_VDCO_YC_CONVERSION_MODE

    defines YC to RGB conversion mode

    0 - Y and C range 0 .. 255
    1 - Y range 16 .. 235, C range 16 .. 240
*/
#define R_VDCO_YC_CONVERSION_MODE         0u


/*******************************************************************************
    Constant: R_VDCO_n_EHSYNC_POLARITY, R_VDCO_n_EVSYNC_POLARITY

    Polarity of hardwired external sync signals 
*/
#define R_VDCO_0_EHSYNC_POLARITY         (R_VDCO_POLARITY_NEGATIVE)
#define R_VDCO_0_EVSYNC_POLARITY         (R_VDCO_POLARITY_NEGATIVE)
#define R_VDCO_1_EHSYNC_POLARITY         (R_VDCO_POLARITY_NEGATIVE)
#define R_VDCO_1_EVSYNC_POLARITY         (R_VDCO_POLARITY_NEGATIVE)


/*******************************************************************************
    Constant: R_VDCO_n_DATA_OUTPUT_DELAY

    output delay settings for data signals. Range 0 .. 3 clock cycles.
*/
#define R_VDCO_0_DATA_OUTPUT_DELAY         0
#define R_VDCO_1_DATA_OUTPUT_DELAY         0

/*******************************************************************************
    Constant: R_VDCO_n_INTERRUPT_FACTOR_i

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
#define R_VDCO_0_INTERRUPT_FACTOR_0     0 /* keep 0 for falling edge */
#define R_VDCO_0_INTERRUPT_FACTOR_1     0
#define R_VDCO_0_INTERRUPT_FACTOR_2     0
#define R_VDCO_0_INTERRUPT_FACTOR_3     0
#define R_VDCO_0_INTERRUPT_FACTOR_4     0
#define R_VDCO_0_INTERRUPT_FACTOR_6     1 

#define R_VDCO_1_INTERRUPT_FACTOR_0     0 /* keep 0 for falling edge */
#define R_VDCO_1_INTERRUPT_FACTOR_1     0
#define R_VDCO_1_INTERRUPT_FACTOR_2     0
#define R_VDCO_1_INTERRUPT_FACTOR_3     0
#define R_VDCO_1_INTERRUPT_FACTOR_4     0
#define R_VDCO_1_INTERRUPT_FACTOR_6     1 





/*******************************************************************************
Section: Global Defines for register access
************************************************************/


/*******************************************************************************
  Macro: R_VDCO_WRITE8

  Write VDCO register 8 bit.
*/

#ifdef R_DBG_PRINT_VDCO_REGS
    #define R_VDCO_WRITE8(ADDR, VALUE)  {R_DBG_PrintRegCmd((8), (ADDR), (VALUE));  *((volatile uint8_t*)(ADDR)) = (VALUE);}
#else
    #define R_VDCO_WRITE8(ADDR, VALUE)  {(*((volatile uint8_t*)(ADDR)) = (VALUE));}
#endif


/*******************************************************************************
  Macro: R_VDCO_WRITE16

  Write VDCO register 16 bit.
*/

#ifdef R_DBG_PRINT_VDCO_REGS
    #define R_VDCO_WRITE16(ADDR, VALUE)  {R_DBG_PrintRegCmd((16), (ADDR), (VALUE));  *((volatile uint16_t*)(ADDR)) = (VALUE);}
#else
    #define R_VDCO_WRITE16(ADDR, VALUE)  {(*((volatile uint16_t*)(ADDR)) = (VALUE));}
#endif

/*******************************************************************************
  Macro: R_VDCO_WRITE32

  Write VDCO register 32 bit.
*/

#ifdef R_DBG_PRINT_VDCO_REGS
    #define R_VDCO_WRITE32(ADDR, VALUE)  {R_DBG_PrintRegCmd((32), (ADDR), (VALUE));  *((volatile uint32_t*)(ADDR)) = (VALUE);}
#else
    #define R_VDCO_WRITE32(ADDR, VALUE)  {(*((volatile uint32_t*)(ADDR)) = (VALUE));}
#endif


/*******************************************************************************
  Macro: R_VDCO_READ8

  Read VDCO register 8 bit.
*/

#ifdef R_DBG_PRINT_VDCO_REGS
    #define R_VDCO_READ8(ADDR)   (R_DBG_PrintRegRead8((ADDR)))
#else
    #define R_VDCO_READ8(ADDR)   ((*((volatile uint8_t*)((ADDR)))))
#endif


/*******************************************************************************
  Macro: R_VDCO_READ16

  Read VDCO register 16 bit.
*/

#ifdef R_DBG_PRINT_VDCO_REGS
    #define R_VDCO_READ16(ADDR)   (R_DBG_PrintRegRead16((ADDR)))
#else
    #define R_VDCO_READ16(ADDR)   ((*((volatile uint16_t*)((ADDR)))))
#endif

/*******************************************************************************
  Macro: R_VDCO_READ32

  Read VDCO register 32 bit.
*/

#ifdef R_DBG_PRINT_VDCO_REGS
    #define R_VDCO_READ32(ADDR)   (R_DBG_PrintRegRead32((ADDR)))
#else
    #define R_VDCO_READ32(ADDR)   ((*((volatile uint32_t*)((ADDR)))))
#endif

#endif /* CONFIG_VDCO_H_ */
