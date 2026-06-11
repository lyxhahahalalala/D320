/*
****************************************************************************
PROJECT : JCUA driver
FILE    : $Id: r_sys_jcua.c 3677 2014-08-29 08:57:11Z kouji.hatada.hx $
============================================================================

DESCRIPTION
JCUA support functions for d1x
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
demonstration purposes at customers premises listed in the signed license
agreement.

****************************************************************************
*/

/***************************************************************************
  Title: JCUA Driver support functions

  Implementation of the JCUA driver support functions for the D1x device.
*/


/***************************************************************************
  Section: Includes
*/
#include    "r_typedefs.h"
#include    "r_jcua_api.h"
#include    "r_jcua_sys.h"
#include    "r_config_jcua.h"
#include    "r_dev_api.h"
#include    "r_tick_api.h"


/***************************************************************************
  Section: Local Defines
*/
/***************************************************************************
  Constant: LOC_DEV_INT_ENABLE

  The offset value of the interrupt enable value.
*/
#define LOC_DEV_INT_ENABLE       (0x01u)

/***************************************************************************
  Constant: LOC_DEV_INT_DISABLE

  The offset value of the interrupt disable value.
*/
#define LOC_DEV_INT_DISABLE      (0x00u)


/***************************************************************************
  Constant: LOC_JCSWRST

  The address of the JCSWRST register (JCUA Software reset register)
*/
#define LOC_JCSWRST     (0xFFC06010u)

/***************************************************************************
  Constant: LOC_JCUA_JCUA0RES

  A constant for bit mask value and offset value of JCUA0RES(JCSWRST.JCUA0RES)

  LOC_JCUA_JCUA0RES_MASK  - Mask value.
  LOC_JCUA_JCUA0RES_SHIFT - Offset value.
*/
#define LOC_JCUA_JCUA0RES_MASK           (0x00000100u)
#define LOC_JCUA_JCUA0RES_SHIFT          (8)

/***************************************************************************
  Constant: LOC_JCUA_RESET_WAIT_TIME_US

  Wait time before JCSWRST.
*/
#define LOC_JCUA_RESET_WAIT_TIME_US       30u


/***************************************************************************
  Constant: LOC_JCUA_BUS_RESET_TIMEOUT_MS

  The timeout [in milliseconds] for JCUA bus reset.
  (JCUA Software reset via JCSWRST register)
*/
#define LOC_JCUA_BUS_RESET_TIMEOUT_MS     2u


/***************************************************************************
  Section: JCUA Driver Support Functions
*/


/***************************************************************************
  Group: JCUA Basic interface functions
*/

/***************************************************************************
  Function: R_JCUA_Sys_Init

  see: <r_jcua_sys.h>
*/

r_jcua_Error_t R_JCUA_Sys_Init(const uint32_t Unit)
{
    r_jcua_Error_t error = R_JCUA_ERR_OK;

    if (Unit == 0)
    {
        /* Initialize Unit 0 of JCUA macro. */

        /* Bus Reset for JCUA (with timeout) */
        /* D1x OPC Rev5.01 item #12 "JCUA reset operation"
           -> JCCMD.BRST must not be used. (JCU macro internal reset not sufficent)
           -> JCSWRST.JCUA0RES must be used instead. (JCU macro external reset)
              Refer to D1x UM section 44.6 "Software Reset Processing".
                Use the following software reset procedure to reset the JCUA unit:
                0. Wait for 30us.
                1. Set JCSWRST.JCUA0RES bit to 1, to generate the software reset condition.
                2. Read JCSWRST.JCUA0RES bit until bit is set to 1, to wait for software reset is active.
                3. Set JCSWRST.JCUA0RES bit to 0, to release software reset condition.
                By this sequennce all JCUA registers with exception
                of the JCSWRST register are initialized by a software reset.
        */
        uint32_t Timeout;
        uint32_t val;
        Timeout = (R_TICK_GetTimeUS(0) + LOC_JCUA_RESET_WAIT_TIME_US);
        do {
            ;
        } while (Timeout > R_TICK_GetTimeUS(0));

        R_JCUA_WRITE32(LOC_JCSWRST, LOC_JCUA_JCUA0RES_MASK);
        val = 0;
        Timeout = (R_TICK_GetTimeMS(0) + LOC_JCUA_BUS_RESET_TIMEOUT_MS);
        while (0 == (val & LOC_JCUA_JCUA0RES_MASK)) {
            if (Timeout <= R_TICK_GetTimeMS(0))
            {
                /* timeout occured */
                return R_JCUA_ERR_BUS_TIMEOUT;
            }

            val = R_JCUA_READ32(LOC_JCSWRST);
        }
        R_JCUA_WRITE32(LOC_JCSWRST, 0u);
        
        /* Enable interrupt. */
        R_JCUA_Sys_InterruptEnable(0);
    }
    else
    {
        error = R_JCUA_ERR_RANGE_UNIT;
    }
    return error;
}


/***************************************************************************
  Function: R_JCUA_Sys_DeInit

  see: <r_jcua_sys.h>
*/

r_jcua_Error_t R_JCUA_Sys_DeInit(const uint32_t Unit)
{
    r_jcua_Error_t error = R_JCUA_ERR_OK;

    if (Unit == 0)
    {
        /* De-Initialize Unit 0 of JCUA macro. */
        /* Disable interrupt. */
        R_JCUA_Sys_InterruptDisable(0);
    }
    else
    {
        error = R_JCUA_ERR_RANGE_UNIT;
    }
    return error;
}


/***************************************************************************
  Function: R_JCUA_Sys_InterruptEnable

  see: <r_jcua_sys.h>
*/

void R_JCUA_Sys_InterruptEnable(const uint32_t Unit)
{
    if (Unit == 0)
    {
        R_DEV_IntEnable(R_DEV_INT_JCU0EDI, LOC_DEV_INT_ENABLE);
        R_DEV_IntEnable(R_DEV_INT_JCU0DTI, LOC_DEV_INT_ENABLE);
    } /* end if */
}


/***************************************************************************
  Function: R_JCUA_Sys_InterruptDisable

  see: <r_jcua_sys.h>
*/

void R_JCUA_Sys_InterruptDisable(const uint32_t Unit)
{
    if (Unit == 0)
    {
        R_DEV_IntEnable(R_DEV_INT_JCU0DTI, LOC_DEV_INT_DISABLE);
        R_DEV_IntEnable(R_DEV_INT_JCU0EDI, LOC_DEV_INT_DISABLE);
    } /* end if */
}

/***************************************************************************
  Function: R_JCUA_Sys_BaseAddrGet

  see: <r_jcua_sys.h>
*/
uint32_t R_JCUA_Sys_BaseAddrGet(const uint32_t Unit)
{
    uint32_t base_addr = 0;

    if (Unit == 0)
    {
        base_addr = R_JCUA_BASE_ADD;
    }
    return base_addr;
}


/***************************************************************************
  Group: JCUA OS interface
*/


/***************************************************************************
  Function: R_JCUA_Sys_Lock

  see: <r_jcua_sys.h>
*/

r_jcua_Error_t R_JCUA_Sys_Lock(const uint32_t Unit)
{
    r_jcua_Error_t error = R_JCUA_ERR_OK;

    /* Check parameters. */
    if (Unit >= R_JCUA_MACRO_NUM)
    {
        error = R_JCUA_ERR_RANGE_UNIT;
    } /* end if */

    return error;
}


/***************************************************************************
  Function: R_JCUA_Sys_Unlock

  see: <r_jcua_sys.h>
*/

r_jcua_Error_t R_JCUA_Sys_Unlock(const uint32_t Unit)
{
    r_jcua_Error_t error = R_JCUA_ERR_OK;

    /* Check parameters. */
    if (Unit >= R_JCUA_MACRO_NUM)
    {
        error = R_JCUA_ERR_RANGE_UNIT;
    } /* end if */
    return error;
}

