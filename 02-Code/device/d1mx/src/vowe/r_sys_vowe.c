/*
****************************************************************************
PROJECT : VOWE driver
FILE    : $Id: r_sys_vowe.c 3800 2014-09-11 09:02:19Z hideki.negishi.wz $
============================================================================
DESCRIPTION
VOWE(Video Output Warping Engine) support functions for d1x_scit
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
  Title: VOWE Support functions

  Implementation of the VOWE support functions for the D1x device.
*/


/***************************************************************************
  Section: Includes
*/
#include "r_config_vowe.h"
#include "r_typedefs.h"
#include "r_vowe_api.h"
#include "r_vowe_sys.h"
#include "r_ddb_api.h"      /* display database with display timings */
#include "r_vdce_api.h"
#include "r_dev_api.h"

/***************************************************************************
  Section: Local Defines
*/
#define LOC_VOWE_UNIT_0           (0)
#define LOC_VDCE_UNIT_0           (0)

#define LOC_ADJUST_DISPLAY_TIMING (8)


/***************************************************************************
  Section: Local Variables
*/

/***************************************************************************
  Section: Local Functions
*/


/***************************************************************************
  Section: Global Functions
*/


/***************************************************************************
  Section: VOWE Driver Support Functions

  Comments see <VOWE Support Functions>
*/

/***************************************************************************
  Function: R_VOWE_Sys_Init
*/
r_vowe_Error_t R_VOWE_Sys_Init(const uint32_t Unit)
{
    r_vowe_Error_t error = R_VOWE_ERR_OK;
    r_vdce_Error_t vdce_result;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
            /* The initialization which is peculiar to the device to use */
    
            /* Adjust display timing. */
            vdce_result = R_VDCE_DisplayTimingAdjust(Unit, LOC_ADJUST_DISPLAY_TIMING);
            if (R_VDCE_ERR_OK != vdce_result)
            {
                error = R_VOWE_ERR_SYS_VDCE;
            }
        }
        else
        {
            error = R_VOWE_ERR_RANGE_UNIT;
        }
    }
    return error;
}


/***************************************************************************
  Function: R_VOWE_Sys_DeInit
*/
r_vowe_Error_t R_VOWE_Sys_DeInit(const uint32_t Unit)
{
    r_vowe_Error_t error = R_VOWE_ERR_OK;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
            /* The de-initialization which is peculiar to the device to use */
        }
        else
        {
            error = R_VOWE_ERR_RANGE_UNIT;
        }
    }
    return error;
}


/***************************************************************************
  Function: R_VOWE_Sys_InterruptEnable
*/
void R_VOWE_Sys_InterruptEnable(const uint32_t Unit)
{
    /* Check unit number.*/
    if (Unit == LOC_VOWE_UNIT_0)
    {
        /* Interrupt priority set */

        /* Interrupt enable */
        R_DEV_IntEnable(R_DEV_INT_VOWE, 1);
    }
    return;
}


/***************************************************************************
  Function: R_VOWE_Sys_InterruptDisable
*/
void R_VOWE_Sys_InterruptDisable(const uint32_t Unit)
{
    /* Check unit number.*/
    if (Unit == LOC_VOWE_UNIT_0)
    {
        /* Interrupt disable */
        R_DEV_IntEnable(R_DEV_INT_VOWE, 0); 
    }

    return;
}


/***************************************************************************
  Function: R_VOWE_Sys_BaseAddrGet
*/
uint32_t R_VOWE_Sys_BaseAddrGet(const uint32_t Unit)
{
    uint32_t       base_addr;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
            base_addr = R_VOWE_BASE_ADDR;
        }
        else
        {
            base_addr = 0x0uL;
        }
    }
    return base_addr;
}


/***************************************************************************
  Function: R_VOWE_Sys_ExtBaseAddrGet
*/
uint32_t R_VOWE_Sys_ExtBaseAddrGet(const uint32_t Unit)
{
    uint32_t       ext_base_addr;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
            ext_base_addr = R_VOWE_EXT_BASE_ADDR;
        }
        else
        {
            ext_base_addr = 0x0uL;
        }
    }
    return ext_base_addr;
}


/***************************************************************************
  Function: R_VOWE_Sys_VDCEnable
*/
r_vowe_Error_t R_VOWE_Sys_VDCEnable(const uint32_t             Unit,
                                    const r_vowe_BufferMode_t  WorkBufferMode,
                                    const uint8_t      * const VdceBufferAddr,
                                    const uint32_t             VdceBufferSize,
                                    const uint32_t             Stride,
                                    const r_vowe_ColorFormat_t ColorFormat,
                                    const uint32_t             SourceWidth,
                                    const uint32_t             SourceHeight,
                                    const uint32_t             RingBufferDelay,
                                    const r_vowe_DestMode_t    DestMode)
{
    r_vowe_Error_t  error  = R_VOWE_ERR_OK;
    r_vdce_Error_t  vdce_result;
    r_vdce_Format_t vdce_format;
    r_vdce_OirMode_t vdce_OirMode;
    uint32_t        ring_buffer_height;
    int32_t         pixel_size;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
            vdce_result = R_VDCE_OirBaseSet(Unit, (uint32_t)VdceBufferAddr);
            if (R_VDCE_ERR_OK == vdce_result)
            {
                if (R_VOWE_FORMAT_RGB565 == ColorFormat)
                {
                    vdce_format = R_VDCE_RGB565;
                    pixel_size = R_VOWE_SIZE_OF_RGB565;
                }
                else
                {
                    vdce_format = R_VDCE_ARGB8888;
                    pixel_size = R_VOWE_SIZE_OF_ARGB8888;
                }

                vdce_OirMode = R_VDCE_OIR_MODE_NORMAL;
                if (R_VOWE_DEST_MODE_SCREEN_SHOTS == DestMode)
                {
                    vdce_OirMode = R_VDCE_OIR_MODE_SCREEN_SHOTS;
                }
    
                /* Select buffer mode */
                if (R_VOWE_FRAME_BUFFER_MODE == WorkBufferMode)
                {
                    /* Setup the Frame Buffer Mode. */
                    vdce_result = R_VDCE_OirRingBufferDisable(Unit);
                }
                else
                {
                    /* Setup the Ring Buffer Mode. */
                    ring_buffer_height = VdceBufferSize / Stride / (uint32_t)pixel_size;
                    vdce_result = R_VDCE_OirRingBufferEnable(Unit, ring_buffer_height);
                    if (R_VDCE_ERR_OK == vdce_result)
                    {
                        vdce_result = R_VDCE_OirVSyncDelaySet(Unit, RingBufferDelay);
                    }
                }
                if (R_VDCE_ERR_OK == vdce_result)
                {
                    vdce_result = R_VDCE_OirMemGeometrySet(Unit, Stride,
                                           64); /* In VDCE, it doesn't use this value. */
                    if (R_VDCE_ERR_OK == vdce_result)
                    {
                        vdce_result = R_VDCE_OirViewPortSet(Unit,
                                                            0, 0,
                                                            0, 0,
                                                            (int32_t)SourceWidth,
                                                            (int32_t)(SourceHeight + R_VOWE_SOURCE_HEIGHT_MARGIN));
                        if (R_VDCE_ERR_OK == vdce_result)
                        {
                            vdce_result = R_VDCE_OirFormatSet(Unit, vdce_format);
                            if (R_VDCE_ERR_OK == vdce_result)
                            {
                                vdce_result = R_VDCE_OirModeSet(Unit, vdce_OirMode);
                                if (R_VDCE_ERR_OK == vdce_result)
                                {
                                    vdce_result = R_VDCE_OirEnable(Unit);
                                } /* R_VDCE_OirModeSet */
                            } /* R_VDCE_OirFormatSet */
                        } /* R_VDCE_OirViewPortSet */
                    } /* R_VDCE_OirMemGeometrySet */
                } /* Select buffer mode */
            } /* R_VDCE_OirBaseSet */
    
            if (R_VDCE_ERR_OK != vdce_result)
            {
                error = R_VOWE_ERR_SYS_VDCE;
            }
        }
        else
        {
            error = R_VOWE_ERR_RANGE_UNIT;
        }


    }
    return error;
}


/***************************************************************************
  Function: R_VOWE_Sys_VDCDisable
*/
r_vowe_Error_t R_VOWE_Sys_VDCDisable(const uint32_t Unit)
{
    r_vowe_Error_t error = R_VOWE_ERR_OK;
    r_vdce_Error_t vdce_result;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
            vdce_result = R_VDCE_OirDisable(LOC_VDCE_UNIT_0);
            if (R_VDCE_ERR_OK != vdce_result)
            {
                error = R_VOWE_ERR_SYS_VDCE;
            }
        }
        else
        {
            error = R_VOWE_ERR_RANGE_UNIT;
        }
    }
    return error;
}


/***************************************************************************
  Group: VOWE OS interface
*/

/***************************************************************************
  Function: R_VOWE_Sys_Lock
*/
r_vowe_Error_t R_VOWE_Sys_Lock(const uint32_t Unit)
{
    r_vowe_Error_t error = R_VOWE_ERR_OK;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
        }
        else
        {
            error = R_VOWE_ERR_RANGE_UNIT;
        }
    }
    return error;
}


/***************************************************************************
  Function: R_VOWE_Sys_Unlock
*/
r_vowe_Error_t R_VOWE_Sys_Unlock(const uint32_t Unit)
{
    r_vowe_Error_t error = R_VOWE_ERR_OK;

    {
        /* Check unit number.*/
        if (Unit == LOC_VOWE_UNIT_0)
        {
        }
        else
        {
            error = R_VOWE_ERR_RANGE_UNIT;
        }
    }
    return error;
}


