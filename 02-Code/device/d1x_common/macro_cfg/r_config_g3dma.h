/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_g3dma.h 7180 2016-01-07 17:45:54Z tobyas.hennig $
============================================================================ 
DESCRIPTION
G3DMA configuration for D1x
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

#ifndef CONFIG_G3DMA_H_
#define CONFIG_G3DMA_H_

/*******************************************************************************
  Title: G3DMA device configuration
  
  Configuration of the device dependanant part of the G3DMA macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_G3DMA_MACRO_NUM

  Number of macros in the device (this is the value relevant for SW, not necessary 
  the real HW implementation.
*/

#define R_G3DMA_MACRO_NUM         1u

/*******************************************************************************
  Constant: R_DMA_CHANNEL_NUM

  Number of available DMA channels
*/
#define R_DMA_CHANNEL_NUM             16u 


/*******************************************************************************
  Constant: R_G3DMA_BASE

   Base address for the macro registers 
*/

#define R_G3DMA_BASE        0xFFFF8000u


#define R_DMA_CHANNEL_OFFSET     0x40u 
#define R_DTFR_CHANNEL_OFFSET    0x02u

/***********************************************************
  Macro: R_G3DMA_WRITE_REG

  Write G3DMAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_G3DMA_REGS
    #define R_G3DMA_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_G3DMA_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_G3DMA_READ_REG

  Read G3DMAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_G3DMA_REGS
    #define R_G3DMA_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_G3DMA_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* CONFIG_G3DMA_H_ */

