/*
****************************************************************************
PROJECT : XBUS driver
FILE    : $Id: r_xbus_regs.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for XBUS macro
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

#ifndef R_XBUS_REGS_H_
#define R_XBUS_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: XBUS Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: Register Offsets

  R_XBUS_CONFIG_0    - Configuration register 0 
  R_XBUS_CONFIG_1    - Configuration register 1
  R_XBUS_CONTROL     - Control register  

*/

#define R_XBUS_CFG      0x00
#define R_XBUS_SYSERR   0x0C
#define R_XBUS_ERRCNT   0x10
#define R_XBUS_ERRADR   0x14
#define R_XBUS_REQSYNC  0x18
#define R_XBUS_BUFFLUSH 0x1C
#define R_XBUS_CACHECMD 0x20
#define R_XBUS_TAGLO    0x24
#define R_XBUS_LRU      0x28
#define R_XBUS_DATALO   0x2C
#define R_XBUS_DATAHI   0x30
#define R_XBUS_TAGECC   0x34
#define R_XBUS_DATAECC  0x38

#define R_XBUS_CABASE0  0x40
#define R_XBUS_CAMASK0  0x44
#define R_XBUS_CABASE1  0x48
#define R_XBUS_CAMASK1  0x4C

#define R_XBUS_BFBASE0  0x60
#define R_XBUS_BFMASK0  0x64
#define R_XBUS_BFBASE1  0x68
#define R_XBUS_BFMASK1  0x6C
                                    
#ifdef __cplusplus
}
#endif

#endif /* R_XBUS_REGS_H_  */
