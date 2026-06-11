/*
****************************************************************************
PROJECT : JCUA driver
FILE    : $Id: r_config_jcua.h 3725 2014-09-03 04:09:02Z kouji.hatada.hx $
============================================================================

DESCRIPTION
JCUA Configuration for d1x
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2013 - 2015
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

#ifndef R_CONFIG_JCUA_H
#define R_CONFIG_JCUA_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
  Title: JCUA Driver configuraion
  
  Configuration of the device dependent part of the JCUA macro
*/

/***************************************************************************
  Section: Global defines
*/

/***************************************************************************
  Constant: R_JCUA_USE_OS 

  Define a OS is used or is not used.
*/

#define    R_JCUA_USE_OS     (0)    /* OS is not used */

/***************************************************************************
  Constant: R_JCUA_MACRO_NUM 

  Number of macros in the device.
*/

#define    R_JCUA_MACRO_NUM  (1)    /* JCUA macro number */

/***************************************************************************
  Constant: R_JCUA_STRIDE_UNIT_SIZE

  Define a JCUA stride unit size. The value which can be set is 128 or 32.

  Note: The set value is based on the setting of VDCE driver. In D1x, setting is 128.
*/

#define    R_JCUA_STRIDE_UNIT_SIZE  (128u)    /* JCUA stride unit size */

#if R_JCUA_STRIDE_UNIT_SIZE == 128u
    #define R_JCUA_STRIDE_MASK      (0x00007F80uL)
    #define R_JCUA_STRIDE_ANDMASK   (0x0000007FuL)
#else
    #define R_JCUA_STRIDE_MASK      (0x00007FE0uL)
    #define R_JCUA_STRIDE_ANDMASK   (0x0000001FuL)
#endif

/****************************************************************************
  Constant: R_JCUA_BASE_ADD

  Base address for the macro registers.
*/
#define R_JCUA_BASE_ADD             (0xF200C000uL)

/****************************************************************************
  Constant: R_JCUA_WRITE32

  Write JCUA register 32bit.
*/
#define R_JCUA_WRITE32(ADDR, VALUE)  (*((volatile uint32_t*)(ADDR)) = (VALUE))

/****************************************************************************
  Constant: R_JCUA_WRITE8

  Write JCUA register 8bit.
*/
#define R_JCUA_WRITE8(ADDR, VALUE)   (*((volatile uint8_t*)(ADDR)) = ((uint8_t)(VALUE)))

/****************************************************************************
  Constant: R_JCUA_READ32

  Read JCUA register 32bit.
*/
#define R_JCUA_READ32(ADDR)          (*((volatile uint32_t*)((ADDR))))

/****************************************************************************
  Constant: R_JCUA_READ8

  Read JCUA register 8bit.
*/
#define R_JCUA_READ8(ADDR)           (*((volatile uint8_t*)((ADDR))))

#ifdef __cplusplus
}
#endif

#endif /* R_CONFIG_JCUA_H */

