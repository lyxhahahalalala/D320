/*
****************************************************************************
PROJECT : JCU Driver
FILE    : $Id: r_config_jcu.h 3813 2014-09-11 13:35:49Z tobyas.hennig $
============================================================================ 
DESCRIPTION
JCU Configuration of r7f701412 (D1M2(H)) device
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


#ifndef CONFIG_JCU_H_
#define CONFIG_JCU_H_

#if defined (R_DBG_PRINT_JCU_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/***********************************************************
  Title: JCU device configuration
  
  Configuration of the device dependanant part of the JCU macro 
*/


/***********************************************************
  Section: Global Defines
*/
#define R_JCU_MACRO_NUM     1      

/***********************************************************
  Constant: R_JCU_REG

  Base address definition for the JCU registers 
*/

#define R_JCU_REG (*(volatile struct st_jcu *)0xF200C000)    


/****************************************************************************
  Constant: R_JCUA_WRITE32

  Write JCUA register 32bit.
*/
#define R_JCUA_WRITE32(ADDR, VALUE)  (*((volatile uint32_t*)(ADDR)) = (VALUE))

/****************************************************************************
  Constant: R_JCUA_READ32

  Read JCUA register 32bit.
*/
#define R_JCUA_READ32(ADDR)          (*((volatile uint32_t*)((ADDR))))


#endif /* CONFIG_JCU_H_ */

