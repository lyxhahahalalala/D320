/*
****************************************************************************
PROJECT : RIIC driver
FILE    : $Id: r_riic_regs.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Register declaration for RIIC macro driver 
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

#ifndef R_RIIC_REGS_H_
#define R_RIIC_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: RIIC Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: Register Offsets
    RIIC_REGNAME           - RIIC register

*/

#define R_RIIC_CR1	0x0000u
#define R_RIIC_CR2	0x0004u
#define R_RIIC_MR1	0x0008u
#define R_RIIC_MR2	0x000Cu
#define R_RIIC_MR3	0x0010u
#define R_RIIC_FER	0x0014u
#define R_RIIC_SER	0x0018u
#define R_RIIC_IER	0x001Cu
#define R_RIIC_SR1	0x0020u
#define R_RIIC_SR2	0x0024u
#define R_RIIC_SAR0	0x0028u
#define R_RIIC_SAR1	0x002Cu
#define R_RIIC_SAR2	0x0030u
#define R_RIIC_BRL	0x0034u
#define R_RIIC_BRH	0x0038u
#define R_RIIC_DRT	0x003Cu
#define R_RIIC_DRR	0x0040u

#define R_RIIC_SR2_START    (1ul<<2u)
#define R_RIIC_SR2_STOP     (1ul<<3u)
#define R_RIIC_SR2_NACKF    (1ul<<4u)
#define R_RIIC_SR2_RDRF     (1ul<<5u)
#define R_RIIC_SR2_TEND     (1ul<<6u)
#define R_RIIC_SR2_TDRE     (1ul<<7u)


/*******************************************************************************
  Section: Global Types
  
  RIIC Register Definitions
*/


#ifdef __cplusplus
}
#endif

#endif /* R_RIIC_REGS_H_  */
