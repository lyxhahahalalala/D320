/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_spea.h 7658 2016-02-15 14:46:21Z matthias.nippert $
============================================================================ 
DESCRIPTION
SPEA configuration for r7f701412
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

****************************************************************************
*/

#ifndef R_CONFIG_SPEA_H
#define R_CONFIG_SPEA_H

#if defined (R_DBG_PRINT_SPEA_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/***********************************************************
  Title: SPEA device configuration
  
  Configuration of the device dependent part of the SPEA macro
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_SPEA_MACRO_NUM

  Number of macros in the device.
*/

#define R_SPEA_MACRO_NUM         1u

/*******************************************************************************
  Constant:  R_SPEA_MAX_SPRITE

  Number of sprites per unit
*/
#define R_SPEA_MAX_SPRITE        16u

/*******************************************************************************
  Constant:  R_SPEA_MAX_SPRITE_UNIT

  Number of sprite units per macro
*/
#define R_SPEA_MAX_SPRITE_UNIT   3u

/*******************************************************************************
  Constant:  R_SPEA_MAX_RLE_UNIT

  Number of RLE units per macro

*/
#define R_SPEA_MAX_RLE_UNIT      2u


/*******************************************************************************
  Constant:  R_SPEA_MAX_WIDTH

  Width of a virtual layer in byte

*/
#define R_SPEA_MAX_WIDTH       8192u

/*******************************************************************************
  Constant:  R_SPEA_BUS_WIDTH_BITS

  Width of the connected bus in bits

*/
#define R_SPEA_BUS_WIDTH_BITS   64u

/*******************************************************************************
  Constant: R_SPEA_BASE

   Base address for the macro registers 
*/

#define R_SPEA_BASE             0xfffd4000u


/***********************************************************
  Macro: R_SPEA_WRITE_REG

  Write SPEAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_SPEA_REGS
    #define R_SPEA_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_SPEA_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_SPEA_READ_REG

  Read SPEAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_SPEA_REGS
    #define R_SPEA_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_SPEA_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* R_CONFIG_SPEA_H */
