/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_xbus.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
XBUS configuration for d1x
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

#ifndef CONFIG_XBUS_H_
#define CONFIG_XBUS_H_

/*******************************************************************************
  Title: XBUS device configuration
  
  Configuration of the device dependanant part of the XBUS macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_XBUS_MACRO_NUM

  Number of macros in the device.
*/

#define R_XBUS_MACRO_NUM         1u



/*******************************************************************************
  Constant: R_XBUS_BASE

   Base address for the macro registers 
*/

#define R_XBUS_BASE_ADDR        0xFFFEEE00u

/*******************************************************************************
  Constant: R_XBUS_CACHE_...

  Cache Geometry definitions 
  
*/

#define R_XBUS_CACHE_BLK_NB     4u
#define R_XBUS_CACHE_WAY_NB     4u
#define R_XBUS_CACHE_IDX_NB     0x80u 
/***********************************************************
  Macro: R_XBUS_WRITE_REG

  Write XBUSC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_XBUS_REGS
    #define R_XBUS_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_XBUS_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_XBUS_READ_REG

  Read XBUSC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_XBUS_REGS
    #define R_XBUS_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_XBUS_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* CONFIG_XBUS_H_ */

