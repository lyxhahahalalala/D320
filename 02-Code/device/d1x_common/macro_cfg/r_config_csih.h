/*
****************************************************************************
PROJECT : CSIH driver
FILE    : $Id: r_config_csih.h 7324 2016-01-22 10:05:32Z matthias.nippert $
============================================================================ 
DESCRIPTION
CSIH configuration for r7f701412
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

#ifndef CONFIG_CSIH_H_
#define CONFIG_CSIH_H_

#if defined (R_DBG_PRINT_CSIH_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif


/*******************************************************************************
  Title: CSIH Configuration of r7f701412
*/

/*******************************************************************************
  Section: Global Defines
*/


/* CSIH address = base address + OS/User offset + register offset */
#define R_CSIH_USER_OFFSET (0x00000000)
#define R_CSIH_OS_OFFSET   (0x00001000)


#define CSIH0_FCLACTL0 0
#define CSIH0_FCLACTL1 0
#define CSIH0_FCLACTL2 0
#define CSIH0_FCLACTL3 0

#define CSIH1_FCLACTL0 0
#define CSIH1_FCLACTL1 0
#define CSIH1_FCLACTL2 0
#define CSIH1_FCLACTL3 0

/*******************************************************************************
  Constant: R_CSIH_MACRO_NUM

  Number of macros in the device.
*/

#define R_CSIH_MACRO_NUM         2

/*******************************************************************************
  Constant: R_CSIH_VARIANT

  This device has two different versions of CSIH implemented.
  This enables version handling in the CSIH driver.
*/

#define R_CSIH_VARIANT

#define R_CSIH_VARIANT_ORIGINAL     (0u)   /* Same as F1x */
#define R_CSIH_VARIANT_ALTERNATIVE  (1u)   /* Alternative version */

/*******************************************************************************
  Constant: R_CSIH_FIFO_SIZE

  Size of FIFO buffer.
*/

#define R_CSIH_FIFO_SIZE            (16u)

/*******************************************************************************
  Constant: R_CSIH_TXBUF_SIZE

  Size of write buffer.
*/

#define R_CSIH_TXBUF_BYTE_SIZE        10

/*******************************************************************************
  Constant: R_CSIH_RXBUF_BYTE_SIZE

  Size of write buffer.
*/

#define R_CSIH_RXBUF_BYTE_SIZE        10



/*******************************************************************************
  Section: Global Macros
*/

/*******************************************************************************
  Macro: R_CSIH_WRITE_REG

  Write CSIH register with predefined size.
  SIZE allowed values are 8, 16, 32
*/
#ifdef R_DBG_PRINT_CSIH_REGS
    #define R_CSIH_WRITE_REG(SIZE, ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)
#else
    #define R_CSIH_WRITE_REG(SIZE, ADDR, VALUE)  (*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE))
#endif



/*******************************************************************************
  Macro: R_CSIH_READ_REG

  Read CSIH register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_CSIH_REGS
#define R_CSIH_READ_REG(SIZE, ADDR) R_DBG_PrintRegRead##SIZE##(ADDR)
#else
#define R_CSIH_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
 #endif

#endif /* CONFIG_CSIH_H_ */

