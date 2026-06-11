/*
****************************************************************************
PROJECT : SFMA driver
FILE    : $Id: r_sys_sfma_gpio.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
SFMA support functions for d1x
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
  Title: SFMA Driver support functions

  Implementation of the SFMA support functions for the D1x device.

*/

/***************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"
#include "r_sfma_api.h"
#include "r_sfma_sys.h"


/***************************************************************************
  Section: SMFA driver support functions (GPIO setting)

  Comments see: <r_sfma_sys.h>
*/

/***************************************************************************
  Function: R_SFMA_Sys_SpibscPortSet

  see: <r_sfma_sys.h>
*/
r_sfma_Error_t R_SFMA_Sys_SpibscPortSet(const uint8_t  DataBusWidth,
                                        const uint32_t Bsz)
{
    return R_SFMA_ERR_OK;

} /* End of function R_SFMA_Sys_SpibscPortSet() */


