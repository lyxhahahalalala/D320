/*
****************************************************************************
PROJECT : r7f701412
FILE    : $Id: r_sys_davehd.h 2282 2014-04-07 13:04:56Z tobyas.hennig $
============================================================================
DESCRIPTION
r7f701412 interrupt service routines
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

#ifndef R_SYS_DAVEHD_H_
#define R_SYS_DAVEHD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define R_DHD_MACRO_NUM    1

typedef enum {
    R_DHD_INT_SYNC,
    R_DHD_INT_PAUSE,
    R_DHD_INT_SPECIAL,
    R_DHD_INT_NUM
} r_dhd_Interrupt_t;

uint32_t R_SYS_DHD_GetBaseAddr(uint32_t Unit);
void     R_SYS_DHD_IntCInit(uint32_t Unit);
void     R_SYS_DHD_IntCDeInit(uint32_t Unit);

#ifdef __cplusplus
}
#endif

#endif /* R_SYS_DAVEHD_H_*/

