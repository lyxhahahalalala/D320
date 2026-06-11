/*
****************************************************************************
PROJECT : CDI
FILE    : $Id: r_cdi_main.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Common Driver Interface
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

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_cdi_api.h"


/*******************************************************************************
  Section: Global API Base Functions
*/

/*******************************************************************************
 Function: R_CDI_Init

  Setup common driver interface.

  Parameters:
  void

  Returns:
  ==0        - Init OK
  !=0        - Init failed
*/

 int32_t R_CDI_Init(void) {
    return 0;
}

/*******************************************************************************
 Function: R_CDI_DeInit

  Commin driver interface deinit function.

  Parameters:
  void

  Returns:
  ==0        - Deinit OK
  !=0        - Deinit failed
*/

 int32_t R_CDI_DeInit(void) {
    return 0;
}

