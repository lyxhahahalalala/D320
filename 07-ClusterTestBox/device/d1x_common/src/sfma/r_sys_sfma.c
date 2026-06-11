/*
****************************************************************************
PROJECT : SFMA driver
FILE    : $Id: r_sys_sfma.c 7539 2016-02-04 13:01:47Z florian.zimmermann $
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
#include "r_config_sfma.h"
#include "r_dev_api.h"


/***************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Constant: LOC_SYS_SFMA_INTERNAL_BUS_CLOCK

  This constant is the internal bus clock.
  D1L2 + D1Mx:
    PLL1       => 480MHz
    SFMA clock => 480MHz / 3 = 160MHz
  D1L1:
    PLL1       => 480MHz
    SFMA clock => 480MHz / 6 = 80MHz
*/
#define LOC_SYS_SFMA_INTERNAL_BUS_CLOCK         (160 * 1000 * 1000)
#define LOC_SYS_SFMA_INTERNAL_BUS_CLOCK_D1L1     (80 * 1000 * 1000)

/*******************************************************************************
  Constant: LOC_SYS_SFMA_CKDLY

  The offset value of the CKDLY register from the base address.
*/
#define LOC_SYS_SFMA_CKDLY              (0x00000050uL)

/*******************************************************************************
  Constant: LOC_SYS_SFMA_SPODLY

  The offset value of the SPODL register from the base address.
*/
#define LOC_SYS_SFMA_SPODLY             (0x00000068uL)

/*******************************************************************************
  Constant: LOC_SYS_SFMA_CLOCK_ADJUSTED_LIMIT

  The limit value which needs the adjustment of the phase relationship.
*/
#define LOC_SYS_SFMA_CLOCK_ADJUSTED_LIMIT       (60 * 1000 * 1000)

/***************************************************************************
  Constant: LOC_SYS_SFMA_DRCR

  The offset value of the DRCR register from the base address.
*/
#define LOC_SYS_SFMA_DRCR               (0x0c)


/***************************************************************************
  Constant: LOC_SYS_SFMA_CKDLY_MASK

  Mask for the CKDLY register.
*/
#define LOC_SYS_SFMA_CKDLY_MASK         (0xFFF8FFF8uL)


/***************************************************************************
  Constant: LOC_SFMA_DEFAULT_CAL

  Default calibration for Serial flash memory.

    SPBCLK clock phase shift        3 (Upper 16bits)
    Sampling point phase shift      1 (Lower 16 bits)
*/

#define SPBCLK_CLOCK_PHASE_SHIFT    (3u)
#define SAMPLING_POINT_PHASE_SHIFT  (1u)
#define LOC_SFMA_DEFAULT_CAL        ((SPBCLK_CLOCK_PHASE_SHIFT << 16) | SAMPLING_POINT_PHASE_SHIFT)


/***************************************************************************
  Section: SFMA driver support functions

  Comments see: <r_sfma_sys.h>
*/

/***************************************************************************
  Function: R_SFMA_Sys_Init

  see: <r_sfma_sys.h>
*/
r_sfma_Error_t R_SFMA_Sys_Init(const uint32_t  Unit)
{
    r_sfma_Error_t sys_err = R_SFMA_ERR_OK;

    if (Unit >= R_SFMA_MACRO_NUM)
    {
        sys_err = R_SFMA_ERR_RANGE_UNIT;
    }

    return sys_err;
}


/***************************************************************************
  Function: R_SFMA_Sys_DeInit

  see: <r_sfma_sys.h>
*/

r_sfma_Error_t R_SFMA_Sys_DeInit(const uint32_t  Unit)
{
    r_sfma_Error_t sys_err = R_SFMA_ERR_OK;

    if (Unit >= R_SFMA_MACRO_NUM)
    {
        sys_err = R_SFMA_ERR_RANGE_UNIT;
    }

    return sys_err;
}

/***************************************************************************
  Function: R_SFMA_Sys_BaseAddrGet

  see: <r_sfma_sys.h>
*/
uint32_t R_SFMA_Sys_BaseAddrGet(const uint32_t Unit)
{
    uint32_t addr;

    if (Unit == 0)
    {
        addr = R_SFMA_BASE_ADD;
    }
    else
    {
        addr = R_SFMA_BASE_ADD;
    }

    return addr;
}

/***************************************************************************
  Function: R_SFMA_Sys_ClockGet

  see: <r_sfma_sys.h>
*/
uint32_t R_SFMA_Sys_ClockGet(const uint32_t Unit)
{
    uint32_t val = 0;
    r_dev_Device_t device  =  R_DEV_GetDev();
    
    if (Unit == 0)
    {
        if (device < R_DEV_R7F701402)
        {
            val = LOC_SYS_SFMA_INTERNAL_BUS_CLOCK_D1L1;
        }
        else
        {
            val = LOC_SYS_SFMA_INTERNAL_BUS_CLOCK;
        }
    }
    else
    {
        /* nothing */
    }

    return val;
}


/***************************************************************************
  Function: R_SFMA_Sys_CalibSetting

  see: <r_sfma_sys.h>
*/
r_sfma_Error_t R_SFMA_Sys_CalibSetting(const uint32_t Unit,
                                       const r_sfma_Config_t * const Config)
{
    uint32_t base;
    r_sfma_Error_t sys_err = R_SFMA_ERR_OK;

    if (Config != R_NULL)
    {
        base = R_SFMA_Sys_BaseAddrGet(Unit);

        if (Config->SerialFlashMemoryMaxClock >= LOC_SYS_SFMA_CLOCK_ADJUSTED_LIMIT)
        {
            /* check for valid calibration values */
            if(0 == (LOC_SYS_SFMA_CKDLY_MASK & Config->Calibration))
            {
                R_SFMA_WRITE32((base+LOC_SYS_SFMA_CKDLY),  0xa5000000u + Config->Calibration); /* Clock delay */
                R_SFMA_WRITE32((base+LOC_SYS_SFMA_SPODLY), 0xa5000000u); /* No Data delay */
            }
            else
            {
                /* error use a default */
                R_SFMA_WRITE32((base+LOC_SYS_SFMA_CKDLY),  0xa5000000u + LOC_SFMA_DEFAULT_CAL); /* Clock delay */
                R_SFMA_WRITE32((base+LOC_SYS_SFMA_SPODLY), 0xa5000000u); /* No Data delay */

                /* error calibration value */
                sys_err = R_SFMA_ERR_PARAM_INCORRECT;
            }
        }
        else
        {
            /* low frequency setting */

            /* clock delay */
            R_SFMA_WRITE32((base+LOC_SYS_SFMA_CKDLY),  0xa5000000u);

            /* fixed 2ns delay  */
            R_SFMA_WRITE32((base+LOC_SYS_SFMA_SPODLY), 0xa5001111u);
        }
    }
    else
    {
        sys_err = R_SFMA_ERR_PARAM_INCORRECT;
    }

    return sys_err;
}


/***************************************************************************
  Function: R_SFMA_Sys_Close

  see: <r_sfma_sys.h>
*/
r_sfma_Error_t R_SFMA_Sys_Close(const uint32_t Unit)
{
    uint32_t base;
    r_sfma_Error_t sys_err = R_SFMA_ERR_OK;
    uint32_t regval;   
 
    if (Unit == 0)
    {
        /* terminate any transfers from external address space */
        base = R_SFMA_Sys_BaseAddrGet(Unit);

        regval = R_SFMA_READ32(base + LOC_SYS_SFMA_DRCR);
        R_SFMA_WRITE32((base + LOC_SYS_SFMA_DRCR),   (regval | 0x01000000uL)); /* set negate */
    }
    else
    {
        sys_err = R_SFMA_ERR_PARAM_INCORRECT;
    }

    return sys_err;
}


/***************************************************************************
  Function: R_SFMA_Sys_DDREnable

  see: <r_sfma_sys.h>
*/
uint8_t R_SFMA_Sys_DDREnable(void)
{
    uint32_t device;
    uint8_t  ret;

    device = R_DEV_GetDev();
    if (device == R_DEV_R7F701401)
    {
        ret = R_FALSE;
    }
    else
    {
        ret = R_TRUE;
    }
    return ret;
}

/***************************************************************************
  Function: R_SFMA_Sys_BusWidth8BitEnable

  see: <r_sfma_sys.h>
*/
uint8_t R_SFMA_Sys_BusWidth8BitEnable(void)
{
    uint32_t device;
    uint8_t  ret;

    device = R_DEV_GetDev();
    if ((device == R_DEV_R7F701401) ||
        (device == R_DEV_R7F701402))
    {
        ret = R_FALSE;
    }
    else
    {
        ret = R_TRUE;
    }
    return ret;
}

/***************************************************************************
  Function: R_SFMA_Sys_SyncPeripheral

  see: <r_sfma_sys.h>
*/
void R_SFMA_Sys_SyncPeripheral(void)
{
    R_DEV_SyncP();
}

/***************************************************************************
  Function: R_SFMA_Sys_Lock

  see: <r_sfma_sys.h>
*/
r_sfma_Error_t R_SFMA_Sys_Lock(const uint32_t Unit)
{
    r_sfma_Error_t sys_err = R_SFMA_ERR_OK;

    if (Unit >= R_SFMA_MACRO_NUM)
    {
        sys_err = R_SFMA_ERR_RANGE_UNIT;
    }

    return sys_err;
}

/***************************************************************************
  Function: R_SFMA_Sys_Unlock

  see: <r_sfma_sys.h>
*/
r_sfma_Error_t R_SFMA_Sys_Unlock(const uint32_t Unit)
{
    r_sfma_Error_t sys_err = R_SFMA_ERR_OK;

    if (Unit >= R_SFMA_MACRO_NUM)
    {
        sys_err = R_SFMA_ERR_RANGE_UNIT;
    }

    return sys_err;
}

/***************************************************************************
  Function: R_SFMA_Sys_Relax

  see: <r_sfma_sys.h>
*/
r_sfma_Error_t R_SFMA_Sys_Relax(const uint32_t Unit)
{
    r_sfma_Error_t sys_err = R_SFMA_ERR_OK;

    if (Unit >= R_SFMA_MACRO_NUM)
    {
        sys_err = R_SFMA_ERR_RANGE_UNIT;
    }

    return sys_err;
}


/***************************************************************************
  Function: R_SFMA_Sys_GetCal

  see: <r_sfma_sys.h>
*/
uint32_t R_SFMA_Sys_GetCal(const uint32_t Unit)
{
    uint32_t base;
    uint32_t regval;
 
    if (Unit == 0)
    {
        /* get the base address */
        base = R_SFMA_Sys_BaseAddrGet(Unit);

        regval = R_SFMA_READ32(base + LOC_SYS_SFMA_CKDLY);
    }
    else
    {
        /* error */
        regval = 0xFFFFFFFFuL;
    }

    return regval;
}
