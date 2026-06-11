/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_vram.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
VRAM configuration for r7f701412
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.

********************************************************************************
*/

#ifndef CONFIG_VRAM_H_
#define CONFIG_VRAM_H_

/*******************************************************************************
  Title: VRAM device configuration
  
  Configuration of the device dependanant part of the VRAM macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_VRAM_MACRO_NUM

  Number of macros in the device.
*/

#define R_VRAM_MACRO_NUM         2u



/*******************************************************************************
  Constant: R_VRAM_BASE

   Base address for the macro registers 
*/

#define R_VRAM_0_BASE        0xFFFD1000u
#define R_VRAM_1_BASE        0xFFFD2000u


/***********************************************************
  Macro: R_VRAM_WRITE_REG

  Write VRAMC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_VRAM_REGS
    #define R_VRAM_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_VRAM_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_VRAM_READ_REG

  Read VRAMC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_VRAM_REGS
    #define R_VRAM_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_VRAM_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* CONFIG_VRAM_H_ */

