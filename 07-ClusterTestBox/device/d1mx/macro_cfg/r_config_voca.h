/*
****************************************************************************
PROJECT : VOCA driver
FILE    : $Id: r_config_voca.h 3813 2014-09-11 13:35:49Z tobyas.hennig $
============================================================================ 
DESCRIPTION
VOCA Configuration for D1M2(H) device (r7f701412)
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

#ifndef CONFIG_VOCA_H_
#define CONFIG_VOCA_H_


#if defined (R_DBG_PRINT_VOCA_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: Device specific VOCA Configuration
*/

/*******************************************************************************
  Section: Global Defines
************************************************************/

/*******************************************************************************
    Constant: R_VOCA_MACRO_NUM

    Number of VOCA macros in the device.

    - For D1Mx one unit will be available
*/
#define R_VOCA_MACRO_NUM         1


/*******************************************************************************
  Constants: R_VOCA_MAX_MONITOR_AREA_NUM
  
  The maximum number of available monitor areas
*/
#define R_VOCA_MAX_MONITOR_AREA_NUM    16


/*******************************************************************************
    Constant: R_VOCA_0_BASE, R_VOCA_1_BASE

    Base address of the VOCA macro Units in the device.
*/
#define R_VOCA_0_BASE       0xFFFD8000u


/*******************************************************************************
  Constant: R_VOCA_HW_VO*_IS_ASSIGNED

  Define, if there is a VO assigned in the hardware to the VOC.
*/

/* In D1M2(H) two VOs are connected to VOCA. */
#define R_VOCA_HW_VO_CH0_IS_AVAILABLE  1
#define R_VOCA_HW_VO_CH1_IS_AVAILABLE  1

#if 0
/* In D1M1(H) and D1L2(H) only one VO is connected to VOCA. */
  #define R_VOCA_HW_VO_CH0_IS_AVAILABLE  1
  #define R_VOCA_HW_VO_CH1_IS_AVAILABLE  0
#endif


/*******************************************************************************
  Constant: R_VOCA_RESOLUTION_*

  min and max resolution setting for both axis

*/
#define R_VOCA_RESOLUTION_HMIN  128
#define R_VOCA_RESOLUTION_HMAX  2048
#define R_VOCA_RESOLUTION_VMIN  128
#define R_VOCA_RESOLUTION_VMAX  2048


/*******************************************************************************
  Constant: R_VOCA_MONITOR_AREA_MAX_*
  
  R_VOCA_MONITOR_AREA_MAX_WIDTH    - max V resolution
  R_VOCA_MONITOR_AREA_MAX_HEIGHT   - max H resolution
*/
#define R_VOCA_MONITOR_AREA_MAX_WIDTH         128
#define R_VOCA_MONITOR_AREA_MAX_HEIGHT        128


#define R_VOCA_MONITOR_AREA_MIN_THRESHOLD   1
#define R_VOCA_MONITOR_AREA_MAX_THRESHOLD   (262144-1)

#define R_VOCA_ACTIVITYMON_MAXTIME_US   136000UL
#define R_VOCA_ACTIVITYMON_MINTIME_US   136000UL

#define R_VOCA_ROSCL_CLK_HZ     240000      /* ROSCL clock 240kHz */

#define R_VOCA_EXPRAM_SIZE_BYTE     8192

/*******************************************************************************
  Constant: USE_VOCA_DISABLE_INTERRUPT_WHEN_CLEARING

  enable workaround of interrupt disable
  when performing clearing of VOCA errors

*/
#define USE_VOCA_DISABLE_INTERRUPT_WHEN_CLEARING     1

/*******************************************************************************
  Constant: USE_VOCA_WAIT_VSYNC_WORKAROUND

  enable workaround of waiting for VSYNC
   o) after sw reset (polling) finised

*/
#define USE_VOCA_WAIT_VSYNC_WORKAROUND     1


/*******************************************************************************
  Constants: R_VOCA_VSYNC_WAIT_TIMEOUT_MS
  
  The timeout for waiting of V-Sync from Video source (in Microseconds).
  Should be a value higher than one frametime. i.e. 50 fps = 20ms / frame.
*/
#define R_VOCA_VSYNC_WAIT_TIMEOUT_MS    200
/* 
    A setting of
     o) #define R_VOCA_VSYNC_WAIT_TIMEOUT_MS    20
    would be ideal. However due to test case [VOCA_TC_0005],
    that is running with ~10 times reduced pixel clock
    we need to set the timout for vsync a higher (20 -> 200ms).
*/


/*******************************************************************************
  Constant: R_VOCA_DEBUG_ENABLE

  enable debug functions of VOCA API (should be set to 0 for application usage)

*/
#define R_VOCA_DEBUG_ENABLE     1


/*******************************************************************************
Section: Global Defines for register access
************************************************************/


/*******************************************************************************
  Macro: R_VOCA_WRITE_REG

  Write VOCA register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_VOCA_REGS
    #define R_VOCA_WRITE_REG(SIZE, ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)
#else
    #define R_VOCA_WRITE_REG(SIZE, ADDR, VALUE)  (*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE))
#endif


/*******************************************************************************
  Macro: R_VOCA_READ_REG

  Read VOCA register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_VOCA_REGS
    #define R_VOCA_READ_REG(SIZE, ADDR) R_DBG_PrintRegRead##SIZE##(ADDR)
#else
    #define R_VOCA_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* CONFIG_VOCA_H_ */
