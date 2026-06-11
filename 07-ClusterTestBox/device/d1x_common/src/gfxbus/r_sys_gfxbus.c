/*
****************************************************************************
PROJECT : D1x
FILE    : $Id: r_sys_gfxbus.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
XBUS system functions for D1x
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2015
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
  Title: D1x GFXBUS Support Functions

  Implementation of the XBUS support functions for the 
  device D1x.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_gfxbus_api.h"
#include "r_gfxbus_sys.h"
#include "r_xbus_api.h"
#include "r_xbus_sys.h"
#include "r_config_xbus.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif



/*******************************************************************************
  Section: Global Functions

*/
/*******************************************************************************
  Function: R_GFXBUS_SYS_Config

  See: r_gfxbus_sys.h for details
*/


r_gfxbus_Error_t R_GFXBUS_SYS_Config(uint32_t Unit, r_gfxbus_Cfg_t * Cfg)
{
    return (r_gfxbus_Error_t) R_XBUS_Config(Unit, (r_xbus_Cfg_t* )Cfg);      
} 

/*******************************************************************************
  Function: R_GFXBUS_SYS_Flush

  See: r_gfxbus_sys.h for details
*/
void R_GFXBUS_SYS_Flush(uint32_t Unit, uint32_t Flush, uint32_t Size) 
{
    R_XBUS_Flush(Unit, Flush, Size);      
} 

/*******************************************************************************
  Function: R_GFXBUS_SYS_SetMode

  See: r_gfxbus_sys.h for details
*/
r_gfxbus_Error_t R_GFXBUS_SYS_SetMode(uint32_t Unit, uint8_t Area, r_gfxbus_AreaMode_t Mode)
{
    return (r_gfxbus_Error_t) R_XBUS_SetMode(Unit, Area, (r_xbus_AreaMode_t) Mode); 
} 

/*******************************************************************************
  Function: R_GFXBUS_SYS_Init

   See: r_gfxbus_sys.h for details
*/

int32_t R_GFXBUS_SYS_Init(uint32_t Unit)
{
    return (int32_t) R_XBUS_Init(Unit); 
} 


/*******************************************************************************
  Function: R_GFXBUS_SYS_DeInit

  See: r_gfxbus_sys.h for details
*/

int32_t R_GFXBUS_SYS_DeInit(uint32_t Unit)
{
    return R_XBUS_DeInit(Unit); 
} 


/*******************************************************************************
  Function: R_SYS_XBUS_BusBusy
  
  See: r_gfxbus_sys.h for details
*/

r_Error_t R_GFXBUS_SYS_BusBusy(uint32_t Unit) 
{
    return R_XBUS_SYS_BusBusy(Unit); 
}


