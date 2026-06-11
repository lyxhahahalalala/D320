/*
****************************************************************************
PROJECT : VLIB macro device level config
FILE    : $Id: r_config_discom.h 3010 2014-06-17 14:48:17Z florian.zimmermann $
============================================================================ 
DESCRIPTION
DISCOM configuration for r7f701412
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

#ifndef CONFIG_DISCOM_H_
#define CONFIG_DISCOM_H_

/*******************************************************************************
  Title: DISCOM device configuration
  
  Configuration of the device dependanant part of the DISCOM macro 
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_DISCOM_MACRO_NUM

  Number of macros in the device.
*/

#define R_DISCOM_MACRO_NUM         4u



/*******************************************************************************
  Constant: R_DISCOM_BASE

   Base address for the macro registers 
*/

#define R_DISCOM_BASE_0         0xF2009000u
#define R_DISCOM_BASE_1         0xF2009400u
#define R_DISCOM_BASE_2         0xF2009800u
#define R_DISCOM_BASE_3         0xF2009C00u


/***********************************************************
  Macro: R_DISCOM_WRITE_REG

  Write DISCOMC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DISCOM_REGS
    #define R_DISCOM_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_DISCOM_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_DISCOM_READ_REG

  Read DISCOMC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DISCOM_REGS
    #define R_DISCOM_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_DISCOM_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


/*******************************************************************************
  Macro: R_DISCOM_WRITE32

  Write DISCOM register 32 bit.
*/

#ifdef R_DBG_PRINT_DISCOM_REGS
    #define R_DISCOM_WRITE32(ADDR, VALUE)  {R_DBG_PrintRegCmd(32, (ADDR), (VALUE));  *((volatile uint32_t*)(ADDR)) = (VALUE);}
#else
    #define R_DISCOM_WRITE32(ADDR, VALUE)  {(*((volatile uint32_t*)(ADDR)) = (VALUE));}
#endif


/*******************************************************************************
  Macro: R_DISCOM_READ32

  Read DISCOM register 32 bit.
*/

#ifdef R_DBG_PRINT_DISCOM_REGS
    #define R_DISCOM_READ32(ADDR)   (R_DBG_PrintRegRead32((ADDR)))
#else
    #define R_DISCOM_READ32(ADDR)   ((*((volatile uint32_t*)((ADDR)))))
#endif


#endif /* CONFIG_DISCOM_H_ */

