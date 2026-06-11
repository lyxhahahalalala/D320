/*
****************************************************************************
PROJECT : Vlib device driver
FILE    : $Id: r_dev_pin_types.h 3010 2014-06-17 14:48:17Z florian.zimmermann $
============================================================================ 
DESCRIPTION
All instances of the guard macros used on the device.
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

#ifndef DEV_GUARD_TYPES_H_
#define DEV_GUARD_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Guard Instances

  A list of the instances of each of the guards build into this devices.
*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_dev_PbgInstance_t

  All available PBus Guard Instances of this device

  Values:
    R_DEV_PBG_PBG0A        - PBus Guard Instance 0 A
    R_DEV_PBG_PBG0B        - PBus Guard Instance 0 B
    R_DEV_PBG_PBG1A        - PBus Guard Instance 1 A
    [...]
    R_DEV_PBG_NUM          - Delimiter

*/
typedef enum {
    R_DEV_PBG_PBG0A,
    R_DEV_PBG_PBG0B,
    R_DEV_PBG_PBG1A,
    R_DEV_PBG_PBG10A,
    R_DEV_PBG_PBG2A,
    R_DEV_PBG_PBG2B,
    R_DEV_PBG_PBG3A,
    R_DEV_PBG_PBG3B,
    R_DEV_PBG_PBG30A,
    R_DEV_PBG_PBG30B,
    R_DEV_PBG_PBG32A,
    R_DEV_PBG_PBG32B,
    R_DEV_PBG_PBG5A,
    R_DEV_PBG_PBG5B,
    R_DEV_PBG_PBG5C,
    
    /* DELIMITER */
    R_DEV_PBG_NUM
    
} r_dev_PbgInstance_t;



/*******************************************************************************
  Enum: r_dev_XcgInstance_t

  All available XC Guard Instances of this device
  
  Values:
  R_DEV_XCG_VRAM0   - VRAM 0
  R_DEV_XCG_VRAM1   - VRAM 1
  R_DEV_XCG_SDRAM0  - SDRAM Port 0 (XC0_0 -> RLE, SP0 -> VI0, L00, L01, OIR, L10, L13)
  R_DEV_XCG_SDRAM1  - SDRAM Port 1 (XC0_1 -> SP1, SP2 -> L02, VOWE, L03, L12, VI1, L11)
  R_DEV_XCG_SDRAM2  - SDRAM Port 2 (XC1   -> DGB, CPU, DMA, JCU, ETH, MLB)
  R_DEV_XCG_SDRAM3  - SDRAM Port 3 (2DGPU)
  R_DEV_XCG_RRAM    - Retention RAM
  R_DEV_XCG_SFMA    - Serial Flash
  R_DEV_XCG_XC1REGS - XC1 Register Bus
  R_DEV_XCG_NUM     - Delimiter
*/

typedef enum
{
    R_DEV_XCG_VRAM0   = 0x00,
    R_DEV_XCG_VRAM1   = 0x01,
    R_DEV_XCG_SDRAM0  = 0x02,
    R_DEV_XCG_SDRAM1  = 0x03,
    R_DEV_XCG_SDRAM2  = 0x04,
    R_DEV_XCG_SDRAM3  = 0x05,
    R_DEV_XCG_RRAM    = 0x06,
    R_DEV_XCG_SFMA    = 0x07,
    R_DEV_XCG_XC1REGS = 0x08,

    /* DELIMITER */
    R_DEV_XCG_NUM     = 0x09,
} r_dev_XcgInstance_t;




#ifdef __cplusplus
}
#endif

#endif /* DEV_GUARD_TYPES_H_  */
