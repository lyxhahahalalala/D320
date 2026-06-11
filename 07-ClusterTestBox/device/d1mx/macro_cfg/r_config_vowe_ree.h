/*
****************************************************************************
PROJECT : VOWE
FILE    : $Id: r_config_vowe_ree.h 3813 2014-09-11 13:35:49Z tobyas.hennig $
============================================================================ 
DESCRIPTION
VOWE configuration for r7f701412
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

#ifndef CONFIG_VOWE_REE_H_
#define CONFIG_VOWE_REE_H_

#if defined (R_DBG_PRINT_VOWE_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: VOWE Configuration
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Define: R_VOWE Ringbuffer variables

  The position of these two variables is in order and offset for several devices, 
  so both addresses are defined as absolute addresses per device.
*/
#define R_VOWE_EXT_RBUF_SIZE 0xffc06044u
#define R_VOWE_EXT_RBUF_ADDR 0xffc06040u


/*******************************************************************************
  Constant: R_VOWE_MACRO_NUM

  Number of VOWE macros in the device.
*/

#define R_VOWE_MACRO_NUM         1u

/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_VOWE_WRITE32

  Write VOWE register 32 bit.
*/

#ifdef R_DBG_PRINT_VOWE_REGS
    #define R_VOWE_WRITE32(ADDR, VALUE)  {R_DBG_PrintRegCmd(32, (ADDR), (VALUE));  *((volatile uint32_t*)(ADDR)) = (VALUE);}
#else
    #define R_VOWE_WRITE32(ADDR, VALUE)  {(*((volatile uint32_t*)(ADDR)) = (VALUE));}
#endif


/*******************************************************************************
  Macro: R_VOWE_READ32

  Read VOWE register 32 bit.
*/

#ifdef R_DBG_PRINT_VOWE_REGS
    #define R_VOWE_READ32(ADDR)   (R_DBG_PrintRegRead32((ADDR)))
#else
    #define R_VOWE_READ32(ADDR)   ((*((volatile uint32_t*)((ADDR)))))
#endif



#endif /* CONFIG_VOWE_REE_H_ */
