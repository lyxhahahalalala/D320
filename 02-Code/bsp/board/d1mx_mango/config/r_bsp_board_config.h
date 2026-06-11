/*
****************************************************************************
PROJECT : BSP config
FILE    : $Id: r_bsp_board_config.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
Description
BSP configuration
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
#ifndef R_BSP_BOARD_CONFIG_H_
#define R_BSP_BOARD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: BSP Board Configuration

  All board specific defines are located here   
  
*/
/*******************************************************************************
  Section: Global Constants
*/
/*******************************************************************************
  Constant: BSP_MAX_LED
  
  Number of availiable LEDs
   
*/

#define BSP_MAX_LED  16u

/*******************************************************************************
  Constant: BSP_DEFAULT_UART
    
  Set the default UART to use, or 0xFF for debuggser I/O pane 
  
*/
#ifndef BSP_DEFAULT_UART
#define BSP_DEFAULT_UART 0xFF
#endif






#ifdef __cplusplus                                   
}                                                    
#endif

#endif /* R_BSP_BOARD_CONFIG_H_*/
