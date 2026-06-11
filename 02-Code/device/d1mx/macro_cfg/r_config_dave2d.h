/*
****************************************************************************
PROJECT : DAVE2D
FILE    : $Id: r_config_dave2d.h 3073 2014-06-23 16:05:31Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Dave2d Configuration of D1M2(H) device
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

#ifndef CONFIG_DAVE2D_H_
#define CONFIG_DAVE2D_H_

/*******************************************************************************
  Title: Dave2d Configuration of D1M2(H) device
*/

/*******************************************************************************
  Section: Includes
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: EE_DAVE2D_MACRO_NUM

  Number of dave2d macros in the device.
*/

#define EE_DAVE2D_MACRO_NUM     1


/*******************************************************************************
  Constant: EE_DAVE2D_0_BASE

  Base address of the dave2d macro Units in the device.
*/

#define EE_DAVE2D_0_BASE   0xFF835000      /* Dx4-H - gfx - DRWE base address */


/*******************************************************************************
  Macro: EE_WRITE_DAVE_REG

  Write dave2d register 32 bit via register offset.
*/

#define EE_WRITE_DAVE_REG( BASE, OFFSET, DATA ) \
  *((volatile uint32_t*)((BASE)+((OFFSET) << 2 ))) = (DATA)

/*******************************************************************************
  Macro: EE_READ_DAVE_REG

  Read dave2d register 32 bit via register offset.
*/

#define EE_READ_DAVE_REG( BASE, OFFSET ) \
  *((volatile uint32_t*)((BASE)+((OFFSET) << 2 )))


/*******************************************************************************
  Macro: EE_DAVE_WRITE32

  Write dave2d register 32 bit.
*/

#define EE_DAVE_WRITE32(Addr, Data) \
    *((volatile uint32_t*)(Addr)) = (Data)

/*******************************************************************************
  Macro: EE_DAVE_READ32

  Read dave2d register 32 bit.
*/

#define EE_DAVE_READ32(Addr) \
    *((volatile uint32_t*)(Addr))

#endif /* CONFIG_DAVE2D_H_ */

