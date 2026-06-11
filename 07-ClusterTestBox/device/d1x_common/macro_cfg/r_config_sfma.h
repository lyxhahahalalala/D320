/*
****************************************************************************
PROJECT : SFMA driver
FILE    : $Id: r_config_sfma.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
SFMA configuration for d1x
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

#ifndef R_CONFIG_SFMA_H
#define R_CONFIG_SFMA_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined (R_DBG_PRINT_SFMA_REGS) 
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/***************************************************************************
  Title: SFMA Driver configuration

  Configuration of the device depnedanant part of the SFMA macro
*/

/***************************************************************************
  Section: Global Defines
*/

/***************************************************************************
  Constant: R_SFMA_BASE_ADD

  Base address for the macro registers.
*/
#define R_SFMA_BASE_ADD             (0xF2FF0000u)

/***************************************************************************
  Constant: R_SFMA_ADDRESS_ALIGNMENT

  Define a SFMA address alignment.
*/
#define R_SFMA_ADDRESS_ALIGNMENT    (0x01FFFFFFuL)

/***************************************************************************
  Constant: R_SFMA_MACRO_NUM

  Number of macros in the device.
*/
#define R_SFMA_MACRO_NUM            (0x00000001uL)

/***************************************************************************
  Constant: R_SFMA_RELAX_CALL_SIZE

  Define a data size to call R_SFMA_Sys_Relax function.
*/
#define R_SFMA_RELAX_CALL_SIZE      (0x400)

 /***************************************************************************
  Constant: R_SFMA_TRANSFER_END_WAIT

  Define the time-out count of the data transfer wait.
*/
#define R_SFMA_TRANSFER_END_WAIT    (1000000u)

/***************************************************************************
  Constant: R_SFMA_SPBSSL_NEGATE_WAIT

  Define the time-out count of the negate wait of the SPBSSL pin.
*/
#define R_SFMA_SPBSSL_NEGATE_WAIT   (1000000u)

/***************************************************************************
  Constant: R_SFMA_BUSY_WAIT

  Define the time-out count of the busy wait of the serial flash memory.
*/
#define R_SFMA_BUSY_WAIT            (1000000u)

/***************************************************************************
  Macro: R_SFMA_WRITE32

  Write SFMA register 32 bit.
*/

#define R_SFMA_WRITE32(ADDR, VALUE)  {R_SFMA_WRITE_REG(32, (ADDR), (VALUE) )}

/***************************************************************************
  Macro: R_SFMA_READ32

  Read SFMA register 32 bit.
*/

#define R_SFMA_READ32(ADDR)   (R_SFMA_READ_REG(32, (ADDR)))

/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_SFMA_WRITE_REG

  Write SFMA register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_SFMA_REGS
    #define R_SFMA_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_SFMA_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE));}
#endif



/*******************************************************************************
  Macro: R_SFMA_READ_REG

  Read SFMA register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_SFMA_REGS
    #define R_SFMA_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_SFMA_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


#ifdef __cplusplus
}
#endif

#endif /* R_CONFIG_SFMA_H */


