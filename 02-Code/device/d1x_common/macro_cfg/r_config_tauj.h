/*
****************************************************************************
PROJECT : TAUJ
FILE    : $Id: r_config_tauj.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
TAUJ configuration for r7f701412
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

#ifndef R_CONFIG_TAUJ_H_
#define R_CONFIG_TAUJ_H_

#if defined (R_DBG_PRINT_TAUJ_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/***********************************************************
  Title: TAUJ Configuration
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constant: R_TAUJ_CHANNEL_NUM

  Number of TAUJ macros in the device.
*/

#define R_TAUJ_MACRO_NUM         1


/***********************************************************
  Constant: R_TAUJ_0_BASE_ADDR

  Number of TAUJ macros in the device.
*/

#define R_TAUJ_0_BASE_ADDR         (0xFFE50000u)


/***********************************************************
  Section: Global Macros
*/

/***********************************************************
  Macro: R_TAUJ_CLR_BIT

  Clear a single bit
*/

#define R_TAUJ_CLR_BIT(data, bit) ((data) &= (uint16_t)(~((uint16_t)((uint16_t)1 << (bit)))))


/***********************************************************
  Macro: R_TAUJ_SET_BIT

  Set a single bit
*/

#define R_TAUJ_SET_BIT(data, bit) ((data) |= (uint16_t)((uint16_t)1 << (bit)))


/*******************************************************************************
  Macro: R_TAUJ_WRITE_REG

  Write TAUJ register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_TAUJ_REGS
    #define R_TAUJ_WRITE_REG(SIZE, ADDR, VALUE) {\
                                  R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));\
                                *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_TAUJ_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE));}
#endif


/*******************************************************************************
  Macro: R_TAUJ_READ_REG

  Read TAUJ register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_TAUJ_REGS
    #define R_TAUJ_READ_REG(SIZE, ADDR)  (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_TAUJ_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


/***********************************************************
  Macro: R_TAUJ_WRITE8

  Write TAUJ register 8 bit.
*/
 #define R_TAUJ_WRITE8(ADDR, VALUE)  {R_TAUJ_WRITE_REG(8,(ADDR),(VALUE))}



/***********************************************************
  Macro: R_TAUJ_WRITE16

  Write TAUJ register 16 bit.
*/
 #define R_TAUJ_WRITE16(ADDR, VALUE)   {R_TAUJ_WRITE_REG(16,(ADDR),(VALUE))}


/***********************************************************
  Macro: R_TAUJ_WRITE32

  Write TAUJ register 32 bit.
*/

 #define R_TAUJ_WRITE32(ADDR, VALUE)   {R_TAUJ_WRITE_REG(32,(ADDR),(VALUE))}



/***********************************************************
  Macro: R_TAUJ_READ8

  Read TAUJ register 8 bit.
*/

#define R_TAUJ_READ8(ADDR)           (R_TAUJ_READ_REG(8, (ADDR)))


/***********************************************************
  Macro: R_TAUJ_READ16

  Read TAUJ register 16 bit.
*/

#define R_TAUJ_READ16(ADDR)          (R_TAUJ_READ_REG(16, (ADDR)))


/***********************************************************
  Macro: R_TAUJ_READ32

  Read TAUJ register 32 bit.
*/
#define R_TAUJ_READ32(ADDR)          (R_TAUJ_READ_REG(32, (ADDR)))

#endif /* CONFIG_TAUJ_H_ */

