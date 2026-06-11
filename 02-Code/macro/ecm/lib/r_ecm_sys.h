/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_ecm_sys.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic ECM driver
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

#ifndef R_ECM_SYS_H_
#define R_ECM_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic ECM Support Functions

  The generic ECM driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/


/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_ECM_BASE_OFFSET

   Base address offsets for the ECM register groups
*/

#define R_ECM_BASE_OFFSET_MASTER    0x0000u /* Offset of ECM Master Registers Address List */
#define R_ECM_BASE_OFFSET_CHECKER   0x1000u /* Offset of ECM Checker Registers Address List */
#define R_ECM_BASE_OFFSET_COMMON    0x2000u /* Offset of ECM Common Registers Address List */


/*******************************************************************************
  Constant: R_ECM_REG_

   ECM Register Offsets 
*/

/* ECM Master Registers */
#define R_ECM_REG_MESET       0x00u     /* ECM master error set trigger register */
#define R_ECM_REG_MECLR       0x04u     /* ECM master error clear trigger register */
#define R_ECM_REG_MESSTR0     0x08u     /* ECM master error source status register 0 */
#define R_ECM_REG_MESSTR1     0x0Cu     /* ECM master error source status register 1 */
#define R_ECM_REG_MPCMD0      0x10u     /* ECM master protection command register */

/* ECM Checker Registers */
#define R_ECM_REG_CESET       0x00u     /* ECM checker error set trigger register */
#define R_ECM_REG_CECLR       0x04u     /* ECM checker error clear trigger register */
#define R_ECM_REG_CESSTR0     0x08u     /* ECM checker error source status register 0 */
#define R_ECM_REG_CESSTR1     0x0Cu     /* ECM checker error source status register 1 */
#define R_ECM_REG_CPCMD0      0x10u     /* ECM checker protection command register */

/* ECM Common Registers */
#define R_ECM_REG_EPCFG       0x00u     /* ECM error pulse configuration register */
#define R_ECM_REG_MICFG0      0x04u     /* ECM maskable interrupt configuration register 0 */
#define R_ECM_REG_MICFG1      0x08u     /* ECM maskable interrupt configuration register 1 */
#define R_ECM_REG_NMICFG0     0x0Cu     /* ECM FE level non-maskable interrupt configuration register 0 */
#define R_ECM_REG_NMICFG1     0x10u     /* ECM FE level non-maskable interrupt configuration register 1 */
#define R_ECM_REG_IRCFG0      0x14u     /* ECM internal reset configuration register 0 */
#define R_ECM_REG_IRCFG1      0x18u     /* ECM internal reset configuration register 1 */
#define R_ECM_REG_EMK0        0x1Cu     /* ECM error mask register 0 */
#define R_ECM_REG_EMK1        0x20u     /* ECM error mask register 1 */
#define R_ECM_REG_ESSTC0      0x24u     /* ECM error source status clear register 0 */
#define R_ECM_REG_ESSTC1      0x28u     /* ECM error source status clear register 1 */
#define R_ECM_REG_PCMD1       0x2Cu     /* ECM protection command register */
#define R_ECM_REG_PS          0x30u     /* ECM protection status register */
#define R_ECM_REG_PE0         0x34u     /* ECM pseudo error trigger register 0 */
#define R_ECM_REG_PE1         0x38u     /* ECM pseudo error trigger register 1 */
#define R_ECM_REG_DTMCTL      0x3Cu     /* ECM delay timer control register */
#define R_ECM_REG_DTMR        0x40u     /* ECM delay timer register */
#define R_ECM_REG_DTMCMP      0x44u     /* ECM delay timer compare register */
#define R_ECM_REG_DTMCFG0     0x48u     /* ECM delay timer configuration register 0 */
#define R_ECM_REG_DTMCFG1     0x4Cu     /* ECM delay timer configuration register 1 */
#define R_ECM_REG_DTMCFG2     0x50u     /* ECM delay timer configuration register 2 */
#define R_ECM_REG_DTMCFG3     0x54u     /* ECM delay timer configuration register 3 */

/* ECM error pulse control register */
#define R_ECM_REG_EPCTL       0xFFC0600Cu

/* This is the reset controller's software reset register */
#define R_RESET_SWRESA        0xfff80a04ul

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_ECM_SYS_HardwareInit

  Setup the HW part for the generic ECM.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_ECM_SYS_HardwareDeInit

  Deinit of the HW part for the generic ECM.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_ecm_Error_t>
*/

r_ecm_Error_t R_ECM_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_ECM_SYS_BaseAddr

  Returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_ECM_SYS_BaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_ECM_SYS_EnableInt

  Enable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eeecm_Interrupt_t>
  
  Returns:
  void
*/
#if 0
void R_ECM_SYS_EnableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt);
#endif

/*******************************************************************************
  Function: R_ECM_SYS_DisableInt

  Disable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eeecm_Interrupt_t>
  
  Returns:
  void
*/
#if 0
void R_ECM_SYS_DisableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt);
#endif

#ifdef __cplusplus
}
#endif


#endif /* ECM_SYS_H_ */
