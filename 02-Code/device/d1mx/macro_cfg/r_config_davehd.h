/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_config_davehd.h 3010 2014-06-17 14:48:17Z florian.zimmermann $
============================================================================ 
DESCRIPTION
DaveHD Configuration of D1M2(H) device (r7f701412)
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

#ifndef CONFIG_DAVEHD_H_
#define CONFIG_DAVEHD_H_

/*******************************************************************************
  Title: DaveHD Configuration of D1M2(H) device (r7f701412)
*/

/*******************************************************************************
  Section: Includes
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
 
  Constant: R_DAVEHD_0_BASE

  Base address of the dave2d macro Units in the device.
*/

#define R_DAVEHD_0_BASE   0xFFFD0000u     /* gfx - DRWE base address D1M2(H) (r7f701412 */



#endif /* CONFIG_DAVEHD_H_ */
