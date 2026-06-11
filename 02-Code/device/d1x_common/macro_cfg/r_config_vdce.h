/*
****************************************************************************
PROJECT : VDCE driver
FILE    : $Id: r_config_vdce.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
VDCE system functions
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

/******************************************************************************
  Title: VDCE system functions

  VDCE driver system functions

******************************************************************************/

#ifndef R_CONFIG_VDCE_H
#define R_CONFIG_VDCE_H


#if defined (R_DBG_PRINT_VDCE_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif

/***************************************************************************
 Constants: Unit Number

 The definition of the unit number.
*/

#define R_VDCE_UNIT0         (0u)
#define R_VDCE_UNIT1         (1u)

/******************************************************************************
 Constant: R_VDCE_VI_RGB666 

 Enable RGB666 on video input
*/

#define R_VDCE_VI_RGB666     (1)

/******************************************************************************
 Constant: R_VDCE_VI_RGB888 

 Enable RGB888 on video input
*/

#define R_VDCE_VI_RGB888     (1)

/*******************************************************************************
  Constant: R_VDCE_BASE_ADDR0

   Base address 0 for the macro registers 
*/
#define R_VDCE_BASE_ADDR0   (0xF2004000u)

/*******************************************************************************
  Constant: R_VDCE_BASE_ADDR1

   Base address 1 for the macro registers 
*/
#define R_VDCE_BASE_ADDR1   (0xF2006000u)

/***************************************************************************
  Macro: R_VDCE_WRITE32

  Write VDCE register 32 bit.
*/

#ifdef R_DBG_PRINT_VDCE_REGS
#define R_VDCE_WRITE32(ADDR, VALUE)  {R_DBG_PrintRegCmd(32, (ADDR), (VALUE));  *((volatile uint32_t*)(ADDR)) = (VALUE);}
#else
#define R_VDCE_WRITE32(ADDR, VALUE)  {(*((volatile uint32_t*)(ADDR)) = (VALUE)); }
#endif

/***************************************************************************
  Macro: R_VDCE_WRITE16

  Write VDCE register 16 bit.
*/

#ifdef R_DBG_PRINT_VDCE_REGS
#define R_VDCE_WRITE16(ADDR, VALUE)  {R_DBG_PrintRegCmd(16, (ADDR), (VALUE));  *((volatile uint16_t*)(ADDR)) = (VALUE);}
#else
#define R_VDCE_WRITE16(ADDR, VALUE)  {(*((volatile uint16_t*)(ADDR)) = (VALUE)); }
#endif

/***************************************************************************
  Macro: R_VDCE_READ32

  Read VDCE register 32 bit.
*/
#ifdef R_DBG_PRINT_VDCE_REGS
#define R_VDCE_READ32(ADDR)   (R_DBG_PrintRegRead32((ADDR)))
#else
#define R_VDCE_READ32(ADDR)   ((*((volatile uint32_t*)((ADDR)))))
#endif

/***************************************************************************
  Macro: R_VDCE_READ16

  Read VDCE register 16 bit.
*/
#ifdef R_DBG_PRINT_VDCE_REGS
#define R_VDCE_READ16(ADDR)   (R_DBG_PrintRegRead16((ADDR)))
#else
#define R_VDCE_READ16(ADDR)   ((*((volatile uint16_t*)((ADDR)))))
#endif

/***************************************************************************
  Constant: R_VDCE_CHECK_PARAMETERS

  The definition for valid int the parameter check with core function.
*/
/* #define     R_VDCE_CHECK_PARAMETERS */

#endif /* R_CONFIG_VDCE_H */

