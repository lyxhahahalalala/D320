/*
****************************************************************************
PROJECT : r7f701412
FILE    : $Id: r_sys_davehd.c 2282 2014-04-07 13:04:56Z tobyas.hennig $
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


/*******************************************************************************
  Title: r7f701412 DHD Support Functions

  Implementation of the DHD support functions for the
  device r7f701412.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_config_davehd.h"
#include "r_sys_davehd.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif


            
/*******************************************************************************
  Variable: loc_IntAddress

  Stores the Addresses of the Interrupt control registers for each unit
  The order of the adresses for each interrupt type schould be the same
  as defined in dhd. See <r_dhd_Interrupt_t>

*/

static r_dev_IntSel_t loc_dhd_IntAddress[R_DHD_MACRO_NUM][R_DHD_INT_NUM] = 
{
/* unit 0 */    {R_DEV_INT_DAVEHD0SYNC, R_DEV_INT_DAVEHD0PAUSE, R_DEV_INT_DAVEHD0SP}
};

/*******************************************************************************
  Section: Global Functions
*/


/*******************************************************************************
  Function: R_SYS_DHD_GetBaseAddr

*/

uint32_t R_SYS_DHD_GetBaseAddr(uint32_t Unit) 
{
    uint32_t addr;

    switch(Unit) 
    {
    case 0:
        addr = R_DAVEHD_0_BASE;
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "DHD: We do not have this Unit");   
        break;
    }

    return addr;
}



/*******************************************************************************
  Function: R_SYS_DHD_IntCInit

*/
void R_SYS_DHD_IntCInit(uint32_t Unit) 
{
    uint8_t i;

    if (Unit >= R_DHD_MACRO_NUM)
    {
       R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "DHD: We do not have this Unit"); 
    }

    /* Enable all interrupts */
    for(i = 0u; i < R_DHD_INT_NUM; i++) {
        R_DEV_IntEnable(loc_dhd_IntAddress[Unit][i], 1);
    }
}


/*******************************************************************************
  Function: R_SYS_DHD_IntCDeInit

*/
void R_SYS_DHD_IntCDeInit(uint32_t Unit) 
{
    uint8_t i;

    if (Unit >= R_DHD_MACRO_NUM)
    {
       R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "DHD: We do not have this Unit"); 
    }

    /* Disable all interrupts */
    for(i = 0u; i < R_DHD_INT_NUM; i++) {
        R_DEV_IntEnable(loc_dhd_IntAddress[Unit][i], 0);
    }
}
