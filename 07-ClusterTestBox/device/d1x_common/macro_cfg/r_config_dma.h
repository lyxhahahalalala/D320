/*
****************************************************************************
PROJECT : D1x
FILE    : $Id: r_config_dma.h 7180 2016-01-07 17:45:54Z tobyas.hennig $
============================================================================ 
DESCRIPTION
DMA configuration for D1x
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

#ifndef CONFIG_DMA_H_
#define CONFIG_DMA_H_

#if defined (R_DBG_PRINT_DMA_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/***********************************************************
  Title: upd70xxxx dmac Configuration
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constant: EE_DMAC_MACRO_NUM

  Number of macros in the device.
*/

#define R_DMA_MACRO_NUM        1u

/***********************************************************
  Constant: EE_DMAC_CHANNEL_NUM

  Number of channels in the device.
*/

#define R_DMA_CHANNEL_NUM      8u

/***********************************************************
  Constant: DMAC_SYSTEM_CLOCK

  System Clock of the macro
*/

#define R_DMA_SYSTEM_CLOCK 20000000u


/***********************************************************
  Constant: DMAC_BASE_ADDRESS

  Base address of the macro
*/

#define R_DMA_BASE_ADDRESS      0xFFFF7300u

#define R_DMA_DTFR_BASE_ADDRESS 0xFFFF7B00u


#define R_DMA_CHANNEL_OFFSET     0x30u
#define R_DTFR_CHANNEL_OFFSET    0x02u
#define R_DMA_CHANNEL_OFFSET_GAP 0x80u


/***********************************************************
  Section: Global Macros
*/

/***********************************************************
  Macro: R_DMA_WRITE_REG

  Write DMAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DMA_REGS
    #define R_DMA_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_DMA_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_DMA_READ_REG

  Read DMAC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DMA_REGS
    #define R_DMA_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_DMA_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR)))) 
#endif

#endif /* CONFIG_DMA_H_ */

