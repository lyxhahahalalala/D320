/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_pbg_sys.c 7186 2016-01-08 10:53:03Z tobyas.hennig $
============================================================================ 
DESCRIPTION
PBus GUARD system functions for D1x
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
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/


/*******************************************************************************
  Title: PBus GUARD device functions

  Implementation of the PBus GUARD support functions for the device.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"

#include "r_pbg_api.h"
#include "r_pbg_sys.h"
#include "r_config_pbg.h"



/*******************************************************************************
  Section: Local Constants
*/

const r_pbg_t r_pbus_guards[] = {
    /* bus guard,       channels, base0,      base1 */
        /* Data Flash, ICU-S, Data Flash ECC, P0A_ERRSLV*/

    { R_DEV_PBG_PBG0A,  0x00F1u, 0xFFF94000u, 0xFFF95000u }, /* PBG0A  - Ch 0,       4,5 6,7                         */
        /* INTC2, DMA */
    { R_DEV_PBG_PBG0B,  0x0003u, 0xFFC4C000u, 0xFFC4C800u }, /* PBG0B  - Ch 0,1                                      */
        /* SSIF0, SSIF1, LCBI0, P1A_ERRSLV, SDRB0, DRCA0 */
    { R_DEV_PBG_PBG1A,  0x0077u, 0xFFF9B000u, 0xFFF9C000u }, /* PBG1A  - Ch 0,1,2,   4,5,6                           */
    { R_DEV_PBG_PBG10A, 0x007Fu, 0xFFF9D000u, 0xFFF9E000u }, /* PBG10A - Ch 0,1,2,3, 4,5,6                           */
    { R_DEV_PBG_PBG2A,  0x02FFu, 0xFFFC7000u, 0xFFFC8000u }, /* PBG2A  - Ch 0,1,2,3, 4,5,6,7,   9                    */
    { R_DEV_PBG_PBG3A,  0x01FAu, 0xFFDC0000u, 0xFFDC1000u }, /* PBG3A  - Ch   1,  3, 4,5,6,7, 8                      */
    { R_DEV_PBG_PBG3B,  0x0DFFu, 0xFFDC2000u, 0xFFDC3000u }, /* PBG3B  - Ch 0,1,2,3, 4,5,6,7, 8,  10,11              */
    { R_DEV_PBG_PBG30A, 0xFFFFu, 0xFFDC8000u, 0xFFDC9000u }, /* PBG30A - Ch 0,1,2,3, 4,5,6,7, 8,9,10,11 ,12,13,14,15 */
    { R_DEV_PBG_PBG30B, 0x0003u, 0xFFDCA000u, 0xFFDCB000u }, /* PBG30B - Ch 0,1                                      */
    { R_DEV_PBG_PBG5A,  0xF78Cu, 0xFFF90000u, 0xFFF90400u }, /* PBG5A  - Ch     2,3,       7, 8,9,10,    12,13,14,15 */
    { R_DEV_PBG_PBG5B,  0x07FFu, 0xFFF90800u, 0xFFF90C00u }, /* PBG5B  - Ch 0,1,2,3, 4,5,6,7, 8,9,10                 */
    { R_DEV_PBG_PBG5C,  0x1FFFu, 0xFFF91000u, 0xFFF91400u }, /* PBG5C  - Ch 0,1,2,3, 4,5,6,7, 8,9,10,11 ,12          */

    { R_DEV_PBG_PBG32A, 0xFFFFu, 0xFFDC4000u, 0xFFDC5000u }, /* PBG32A - Ch 0,1,2,3, 4,5,6,7, 8,9,10,11 ,12,13,14,15 */
    { R_DEV_PBG_PBG32B, 0x000Fu, 0xFFDC6000u, 0xFFDC7000u }, /* PBG32B - Ch 0,1,2,3,                                 */

    /* DELIMITER - DO NOT CHANGE */
    { R_DEV_PBG_NUM, 0, 0, 0   }
};


/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_PBG_SYS_BaseAddr
  
  See: r_pbg_sys.h for details
*/

r_pbg_Error_t R_PBG_SYS_GetInstance(r_pbg_Id_t Id, r_pbg_t *Instance)
{
    const r_pbg_t * pbus_guard_ptr = r_pbus_guards;
    
    /* Find the right instance */
    /* Requested instance exceeds range */
    if ( Id.Instance >= R_DEV_PBG_NUM )
    {
        pbus_guard_ptr = 0;
    }

    while (0 != pbus_guard_ptr)
    {
        /* Found requested instance */
        if ( pbus_guard_ptr->Instance == Id.Instance )
        {
            /* Check if the instance has the requested channel */
            if ( 0 != ((uint32_t)pbus_guard_ptr->ChannelActive & (1ul << Id.Channel)) )
            {
                /* Found! */
                break;
            }
            /* if requested channel does not exist. Zero will be returned.*/
            else
            {
                pbus_guard_ptr = 0;
            }
        }
        /* Requested instance not found */
        if ( R_DEV_PBG_NUM == pbus_guard_ptr->Instance )
        {
            pbus_guard_ptr = 0;
        }
        
        /* Check next entry */
        if ( 0 != pbus_guard_ptr )
        {
            pbus_guard_ptr++;
        }
    }
    
    if (0 != pbus_guard_ptr)
    {
        *Instance = *pbus_guard_ptr;
        return R_PBG_ERR_OK;
    }
    else
    {
        return R_PBG_ERR_RANGE;
    }
}


/*******************************************************************************
  Function: R_PBG_ConfigureGuardSysCall_Internal

  For those devices that have no Supervisor/User mode implemented yet, 
  this call should redirect to <R_PBG_ConfigureGuard>
    
  See: <r_pbg_sys.h> 
*/
r_pbg_Error_t R_PBG_ConfigureGuardSysCall_Internal(r_pbg_Id_t Id, r_pbg_Config_t Config)
{
    return (r_pbg_Error_t)R_DEV_SysCall(
        R_DEV_SYSCALL_CONFIGURE_PBG, (void*)(&Id), (void*)(&Config), 0);
}


/*******************************************************************************
  Function: R_PBG_SYS_HardwareInit
  
  See: r_PBG_sys.h for details
*/

r_pbg_Error_t R_PBG_SYS_HardwareInit(uint32_t Unit) 
{
    if (Unit >= R_PBG_MACRO_NUM) 
    {
        return R_PBG_ERR_RANGE; 
    }
    else 
    {
        return R_PBG_ERR_OK;
    } 
}
/*******************************************************************************
  Function: R_PBG_SYS_HardwareDeInit
  
  See: r_PBG_sys.h for details
*/

r_pbg_Error_t R_PBG_SYS_HardwareDeInit(uint32_t Unit) 
{
    if (Unit >= R_PBG_MACRO_NUM) 
    {
        return R_PBG_ERR_RANGE; 
    }
    else 
    {
        return R_PBG_ERR_OK;
    } 
}


#if 0
/*******************************************************************************
  Function: R_PBG_SYS_EnableInt
*/

void R_PBG_SYS_EnableInt(uint32_t Unit, r_pbg_Interrupt_t Interrupt) 
{
    /* Currently there is only one vector for all instances, so this is more or less useless */
    Unit = 0;
    
    switch (Unit)
    {
    case 0:
        /* +1 -Int because list order is inverted */
        /*R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_PBGWRITEERR + 1 - Interrupt), 1); */
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "PBG: We do not have this Unit");      
        break; 
    }    
}


/*******************************************************************************
  Function: R_PBG_SYS_DisableInt
*/

void R_PBG_SYS_DisableInt(uint32_t Unit, r_pbg_Interrupt_t Interrupt) 
{
    switch (Unit)
    {
    case 0:
        /* +1 -Int because list order is inverted */
        /*R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_PBGWRITEERR + 1 - Interrupt), 0); */
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "PBG: We do not have this Unit");      
        break; 
   }    
}
#endif


