/*
****************************************************************************
PROJECT : SG driver
FILE    : $Id: r_config_sg.h 7180 2016-01-07 17:45:54Z tobyas.hennig $
============================================================================ 
DESCRIPTION
SG configuration for D1x
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

Warranty Disclaimer

Because the Product(s) is licensed free of charge, there is no warranty 
of any kind whatsoever and expressly disclaimed and excluded by Renesas, 
either expressed or implied, including but not limited to those for 
non-infringement of intellectual property, merchantability and/or 
fitness for the particular purpose. 
Renesas shall not have any obligation to maintain, service or provide bug 
fixes for the supplied Product(s) and/or the Application.

Each User is solely responsible for determining the appropriateness of 
using the Product(s) and assumes all risks associated with its exercise 
of rights under this Agreement, including, but not limited to the risks 
and costs of program errors, compliance with applicable laws, damage to 
or loss of data, programs or equipment, and unavailability or 
interruption of operations.

Limitation of Liability

In no event shall Renesas be liable to the User for any incidental, 
consequential, indirect, or punitive damage (including but not limited 
to lost profits) regardless of whether such liability is based on breach 
of contract, tort, strict liability, breach of warranties, failure of 
essential purpose or otherwise and even if advised of the possibility of 
such damages. Renesas shall not be liable for any services or products 
provided by third party vendors, developers or consultants identified or
referred to the User by Renesas in connection with the Product(s) and/or the 
Application.

****************************************************************************
*/

#ifndef CONFIG_SG_H_
#define CONFIG_SG_H_

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/***********************************************************
  Title: device specific SG Configuration
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constant: R_SG0_BASE_ADDRESS

  Macro instance base address of SG macro #0.
*/

#define R_SG0_BASE_ADDRESS (0xFFF11000u)
#define R_SG1_BASE_ADDRESS (0xFFF11100u)
#define R_SG2_BASE_ADDRESS (0xFFF11200u)
#define R_SG3_BASE_ADDRESS (0xFFF11300u)
#define R_SG4_BASE_ADDRESS (0xFFF11400u)


/***********************************************************
  Constant: R_SG_MACRO_NUM

  Number of SG macros in the device.
*/

#define R_SG_MACRO_NUM         5u

/*******************************************************************************
  Macro: R_SG_WRITE_REG

  Write SG register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_DEV_REGS
    #define R_SG_WRITE_REG(SIZE, ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)
#else
    #define R_SG_WRITE_REG(SIZE, ADDR, VALUE)  (*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE))
#endif


/*******************************************************************************
  Macro: R_SG_READ_REG

  Read SG register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DEV_REGS
#define R_SG_READ_REG(SIZE, ADDR) R_DBG_PrintRegRead##SIZE##(ADDR)
#else
#define R_SG_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
 #endif


#endif /* CONFIG_SG_H_ */

