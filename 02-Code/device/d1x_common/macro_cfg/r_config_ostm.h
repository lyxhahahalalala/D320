/*
****************************************************************************
PROJECT : OSTM
FILE    : $Id: r_config_ostm.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
OSTM configuration for D1x
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2015
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

#ifndef CONFIG_OSTM_H_
#define CONFIG_OSTM_H_

#if defined (R_DBG_PRINT_OSTM_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Title: r7f701412 OSTM Configuration
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_OSTM_MACRO_NUM

  Number of OSTM macros in the device.
*/
#define R_OSTM_MACRO_NUM         2u

/*******************************************************************************
  Constant: R_OSTM_BASE_ADDR

  Number of OSTM macros in the device.
*/
#define R_OSTM0_BASE_ADDR         (0xFFEC0000u)
#define R_OSTM1_BASE_ADDR         (0xFFEC1000u)

/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_OSTM_WRITE_REG

  Write OSTM register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_OSTM_REGS
    #define R_OSTM_WRITE_REG(SIZE, ADDR, VALUE) {\
         R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  \
        *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_OSTM_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE));}
#endif



/*******************************************************************************
  Macro: R_OSTM_READ_REG

  Read OSTM register register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_OSTM_REGS
    #define R_OSTM_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_OSTM_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif


#endif /* CONFIG_OSTM_H_ */

