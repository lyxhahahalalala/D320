/*
****************************************************************************
PROJECT : Pin Driver
FILE    : $Id: r_config_pin.h 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
Pin configuration for r7f701412
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

#ifndef CONFIG_PIN_H_
#define CONFIG_PIN_H_

/*******************************************************************************
  Title: Pin Register Configuration
*/

/*******************************************************************************
  Section: Includes
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_DEV_PIN_BASE

  Base address for pin registers 
*/

#define R_DEV_PIN_BASE          0xFF610000u  /* PORTUn_Base (for n>0) */

/* PORTOn_Base address is handled by register offset
    using the define R_DEV_PIN_BASEO_OFFSET */
#define R_DEV_PIN_BASEO_OFFSET  (0xFFC10000u - R_DEV_PIN_BASE)

/* PORTU/O_0_Base address (for n = 0) is handled by register offset
    using the define R_DEV_PIN_BASEOU_0_OFFSET */
#define R_DEV_PIN_BASEOU_0_OFFSET  (0xFF618000u - R_DEV_PIN_BASE)

/*******************************************************************************
  Constant: R_DEV_PORT_OFFSET

  Offset for each port
*/

#define R_DEV_PORT_OFFSET         0x0040u

/*******************************************************************************
  Constant: R_DEV_PIN_OFFSET

  Offset for each pin
  (used only for PCRn register that allows direct pin based control)
*/

#define R_DEV_PIN_OFFSET         0x0004u


#endif /* CONFIG_PIN_H_ */

