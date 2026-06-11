/*
****************************************************************************
PROJECT : SFMA driver
FILE    : $Id: r_sfcdb_macronix.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
Flash driver for a macronix device
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

/***************************************************************************
  Title: Serial flash command table

  Note: This file defines the macro define and the flash command table for
        Macronix MX25L51245G serial flash device.
        If you use other serial flash device of Macronix, change setting
        parameters according to the model number of the target.

*/

#ifndef R_SFCDB_MACRONIX_H
#define R_SFCDB_MACRONIX_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
  Section: Global Defines
*/

/***************************************************************************
  Constant: R_SFC_MACRONIX_MANUFACTURE_ID

  Manufacture ID.
*/
#define R_SFC_MACRONIX_MANUFACTURE_ID (0xC2)

/***************************************************************************
  Constant: R_SFC_MACRONIX_MEMORYTYPE_ID

  Memori type ID.
*/
#define R_SFC_MACRONIX_MEMORYTYPE_ID (0x20)

/***************************************************************************
  Constant: R_SFC_MACRONIX_CAPACITY_ID

  Capacity ID.
*/
#define R_SFC_MACRONIX_CAPACITY_ID (0x1A)

/***************************************************************************
  Constant: R_SFC_MACRONIX_PAGE_SIZE

  Page size(Byte).
*/
#define R_SFC_MACRONIX_PAGE_SIZE (256)

/***************************************************************************
  Constant: R_SFC_MACRONIX_SECTOR_SIZE

  Erase sector size(Byte).
*/
#define R_SFC_MACRONIX_SECTOR_SIZE (4 * 1024)

/***************************************************************************
  Constant: R_SFC_MACRONIX_FLASH_MEMORY_SIZE

  Serial flash memory size(Byte).
*/
#define R_SFC_MACRONIX_FLASH_MEMORY_SIZE (64 * 1024 * 1024)


/***************************************************************************
  Section: Global Variables
*/

extern r_sfma_FlashCommand_t r_sfc_Macronix_32bitAddrCmd_40MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_32bitAddrCmd_80MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_24bitAddrCmd_40MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_24bitAddrCmd_80MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_32bitAddrQuadCmd_40MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_32bitAddrQuadCmd_80MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_24bitAddrQuadCmd_40MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_24bitAddrQuadCmd_80MHz;

extern r_sfma_FlashCommand_t r_sfc_Macronix_32bitAddrCmdPerformanceEnhance_80MHz;
extern r_sfma_FlashCommand_t r_sfc_Macronix_32bitAddrCmdPerformanceEnhance_40MHz;
#ifdef __cplusplus
}
#endif

#endif /* R_SFCDB_MACRONIX_H */

