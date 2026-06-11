/*
****************************************************************************
PROJECT : OSTM driver
FILE    : $Id: r_sys_ostm.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for OSTM macro
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
  Title: OSTM Support Functions

  Implementation of the OSTM support functions for the
  D1x device.
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_ostm_api.h"
#include "r_ostm_sys.h"
#include "r_config_ostm.h"
#include "r_dev_api.h"


/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Constant: loc_BaseAddress

  Base address table of OSTM macros
*/
static const uint32_t loc_BaseAddress[R_OSTM_MACRO_NUM] = 
{
/*ostm0 */     R_OSTM0_BASE_ADDR,
/*ostm1 */     R_OSTM1_BASE_ADDR
};

/*******************************************************************************
  Constant: loc_ClkSelId

  Clock selector ID for each OSTM unit  
*/
static const r_dev_ClkSel_t loc_ClkSelId[R_OSTM_MACRO_NUM] = 
{
/*ostm0 */    R_DEV_CKS_OSTM,
/*ostm1 */    R_DEV_CKS_OSTM,
};

/*******************************************************************************
  Section: Local Types
*/


/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Section: OSTM Macro Driver Support Functions

  Comments see: <ostm_sys.h>
*/


/*******************************************************************************
  Function: R_SYS_OSTM_GetBaseAddr
*/

uint32_t R_SYS_OSTM_GetBaseAddr(uint32_t Unit)
{
    uint32_t addr;
    addr = loc_BaseAddress[Unit];
    return addr;
}


/*******************************************************************************
  Function: R_SYS_OSTM_GetClock
*/

uint32_t R_SYS_OSTM_GetClock(uint32_t Unit) 
{
    uint32_t val;
    val = R_DEV_ClkFrequencyHz(loc_ClkSelId[Unit]);
    return val;
}



/*******************************************************************************
  Function: R_SYS_OSTM_EnableInt
*/

void R_SYS_OSTM_EnableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt) 
{
        R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_OSTM0 + Unit), 1);
}

/*******************************************************************************
  Function: R_SYS_OSTM_DisableInt
*/

void R_SYS_OSTM_DisableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt)
{
    R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_OSTM0 + Unit), 0);
}


