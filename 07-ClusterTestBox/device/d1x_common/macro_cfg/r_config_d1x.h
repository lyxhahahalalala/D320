/*
****************************************************************************
PROJECT : Generic memory configuration for multiple devices
FILE    : $Id: r_config_d1x.h 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================
DESCRIPTION
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
#ifndef R_CONFIG_D1X_H_
#define R_CONFIG_D1X_H_
/*******************************************************************************
  Title: D1x memory configurations

  Generic memory configuration for multiple devices
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
 * */

/* D1L */
#define D1L_VRAM0                (0x3FFDC000)
#define D1L_VRAM0_SIZE           (0x24000u)
#define D1L_VRAM0_WRAP_ARGB6666  (0x97FD0000)
#define D1L_VRAM0_WRAP_ARGB666   (0x8FFC0000)
#define D1L_VRAM0_WRAP_ARGB888   (0x87FD0000)

/* D1M */
#define D1M1_VRAM0               (0x3FE74000)
#define D1M2_VRAM0               (0x3FCE8000)
#define D1M2_VRAM1               (0x3FE74000)
#define D1M_SDRAM_BASE           (0x40000000)
#define D1M_VRAM0_SIZE           (0x18C000)
#define D1M2_VRAM1_SIZE          (0x18C000)
#define D1M1_SDRAM_SIZE          (16u  * 1024u * 1024u) /*  16 MB */
#define D1M2_SDRAM_SIZE          (512u * 1024u * 1024u) /* 512 MB */

/* D1M1 */
#define D1M1_VRAM0_WRAP_ARGB6666 (0x97DF0000)
#define D1M1_VRAM0_WRAP_ARGB666  (0x8FD40000)
#define D1M1_VRAM0_WRAP_ARGB888  (0x87DF0000)

/* D1M2 */
#define D1M2_VRAM0_WRAP_ARGB6666 (0x97BE0000)
#define D1M2_VRAM0_WRAP_ARGB666  (0x8FA80000)
#define D1M2_VRAM0_WRAP_ARGB888  (0x87BE0000)

#define D1M2_VRAM1_WRAP_ARGB6666 (0x97DF0000)
#define D1M2_VRAM1_WRAP_ARGB666  (0x8FD40000)
#define D1M2_VRAM1_WRAP_ARGB888  (0x87DF0000)

#endif /* R_CONFIG_D1X_H_ */
