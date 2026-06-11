/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_ecm_isr.c 4537 2014-12-12 10:23:23Z golczewskim $
============================================================================ 
DESCRIPTION
Generic ECM driver
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2012
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
  Title: ECM Interrupt Functions

  Handle the ECM interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_dev_err_types.h"

#include "r_config_ecm.h"
#include "r_ecm_api.h"
#include "r_ecm_sys.h"
#include "r_ecm_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <r_ecm_api.h>
*/


/*******************************************************************************
  Function: R_ECM_EnableInt
*/
#if 0
void R_ECM_EnableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt) 
{
    r_ecm_Error_t err; 
    err = R_ECM_CheckMacroParams(Unit);
    if ((R_ECM_ERR_OK != err) || (Interrupt >= R_ECM_INT_NUM)) 
    {
        R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE); 
        return; 
    } 
    R_ECM_SYS_EnableInt(Unit, Interrupt); 
}
#endif

/*******************************************************************************
  Function: R_ECM_DisableInt
*/
#if 0
void R_ECM_DisableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt) 
{
    r_ecm_Error_t err; 
    err = R_ECM_CheckMacroParams(Unit);
    if ((R_ECM_ERR_OK != err) || (Interrupt >= R_ECM_INT_NUM)) 
    {
        R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE); 
        return; 
    } 
    R_ECM_SYS_DisableInt(Unit, Interrupt);    
}
#endif

/*******************************************************************************
  Function: R_ECM_SetIsrCallback

  See: <r_ecm_api.h> 
*/

 r_ecm_Error_t R_ECM_SetIsrCallback(uint32_t Unit, 
                                              r_ecm_Interrupt_t Interrupt,     
                                              r_dev_EcmErrSel_t ErrorSource,
                                              void (*Isr)(void)) 
{
    r_ecm_Error_t err; 
    err = R_ECM_CheckMacroParams(Unit);
    if ((R_ECM_ERR_OK != err) || (Interrupt >= R_ECM_INT_NUM) || (ErrorSource >= R_DEV_ECMERR_LAST)) 
    {
        R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE); 
        return R_ECM_ERR_RANGE; 
    } 
    
    if ( R_ECM_INT_FEINT == Interrupt )
    {
        r_ecm_Dev[Unit].FEINTCallback[ErrorSource] = Isr;
    }
    else
    {
        r_ecm_Dev[Unit].FENMICallback[ErrorSource] = Isr;
    }

    return R_ECM_ERR_OK;
}


/*******************************************************************************
  Function: R_ECM_FeNmiIsr

  See: <r_ecm_api.h> 
*/

void R_ECM_FeNmiIsr(uint32_t Unit)
{
    r_dev_EcmErrSel_t ErrSource;
    uint32_t regNo, regMask;
    uint32_t regErrSrcStatus, regErrIntActive0, regErrIntActive1;
    uint32_t base;
    uint32_t errorSourceSet = 0;

    base =    r_ecm_Dev[Unit].BaseAddr;
    
    /* Check which interrupts are unmasked at all. Only process thos interrupts */
    regErrIntActive0 = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_NMICFG0);
    regErrIntActive1 = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_NMICFG1);
    
    /* Go through each error source and execute the interrupt handler if required */
    for (ErrSource = R_DEV_ECMERR_FIRST; R_DEV_ECMERR_LAST > ErrSource; ErrSource++)
    {
        regNo   = R_ECM_GetRegisterNoFromErrSource(Unit, ErrSource);
        regMask = R_ECM_GetRegisterMaskFromErrSource(Unit, ErrSource);
        
        errorSourceSet = 0;

        /* Check the error source status register for the currently checked error source */
        if (0 == regNo)
        {
            /* Read status bit of currently checked error source */
            regErrSrcStatus = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_MASTER + R_ECM_REG_MESSTR0);
            regErrSrcStatus &= regErrIntActive0;    /* Delete status bit if FENMI is masked 
                                                       be aware that regErrIntActive0 is inversed itself
                                                       0: unmasked (active) 1: masked (inactive), 
                                                       thus no '~' required here  */
            
            /* Check status bit of currently checked error source */
            if (0 != (regErrSrcStatus & regMask))
            {
                errorSourceSet = 1;
            }
        }
        else
        {
            /* Read status bit of currently checked error source */
            regErrSrcStatus = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_MASTER + R_ECM_REG_MESSTR1);
            regErrSrcStatus &= regErrIntActive1;    /* Delete status bit if FENMI is masked */

            /* Check status bit of currently checked error source */
            if (0 != (regErrSrcStatus & regMask))
            {
                errorSourceSet = 1;
            }
        }
        
        /* If there is an active error for the currently checked error source, execeute the interrupt handler */
        if (1 == errorSourceSet)
        {
            if (0 == r_ecm_Dev[Unit].FENMICallback[ErrSource])
            {
                /* Interrupt unmasked, but no Interrupt Vector defined! */
                R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE); 
            }
            else
            {
                r_ecm_Dev[Unit].FENMICallback[ErrSource]();
                R_ECM_ClearErrorSourceStatus(Unit, ErrSource);
            }
        }
    }
    
    
    /* An NMI will not return to normal code execution. RESET THE DEVICE. */
    R_ECM_WRITE_REG(32, /*No "base + ", this is a direct address!*/ R_RESET_SWRESA, 0x00000001);
}


/*******************************************************************************
  Function: R_ECM_FeIntIsr

  See: <r_ecm_api.h> 
*/

void R_ECM_FeIntIsr(uint32_t Unit)
{
    r_dev_EcmErrSel_t ErrSource;
    uint32_t regNo, regMask;
    uint32_t regErrSrcStatus, regErrIntActive0, regErrIntActive1;
    uint32_t base;
    uint32_t errorSourceSet = 0;

    base =    r_ecm_Dev[Unit].BaseAddr;
    
    /* Check which interrupts are unmasked at all. Only process thos interrupts */
    regErrIntActive0 = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_MICFG0);
    regErrIntActive1 = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_MICFG1);
    
    /* Go through each error source and execute the interrupt handler if required */
    for (ErrSource = R_DEV_ECMERR_FIRST; R_DEV_ECMERR_LAST > ErrSource; ErrSource++)
    {
        regNo   = R_ECM_GetRegisterNoFromErrSource(Unit, ErrSource);
        regMask = R_ECM_GetRegisterMaskFromErrSource(Unit, ErrSource);
        
        errorSourceSet = 0;

        /* Check the error source status register for the currently checked error source */
        if (0 == regNo)
        {
            /* Read status bit of currently checked error source */
            regErrSrcStatus = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_MASTER + R_ECM_REG_MESSTR0);
            regErrSrcStatus &= regErrIntActive0;    /* Delete status bit if FENMI is masked 
                                                       be aware that regErrIntActive0 is inversed itself
                                                       0: unmasked (active) 1: masked (inactive), 
                                                       thus no '~' required here  */
            

            /* Check status bit of currently checked error source */
            if (0 != (regErrSrcStatus & regMask))
            {
                errorSourceSet = 1;
            }
        }
        else
        {
            /* Read status bit of currently checked error source */
            regErrSrcStatus = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_MASTER + R_ECM_REG_MESSTR1);
            regErrSrcStatus &= regErrIntActive1;    /* Delete status bit if FEINT is masked */

            /* Check status bit of currently checked error source */
            if (0 != (regErrSrcStatus & regMask))
            {
                errorSourceSet = 1;
            }
        }
        
        /* If there is an active error for the currently checked error source, execeute the interrupt handler */
        if (1 == errorSourceSet)
        {
            if (0 == r_ecm_Dev[Unit].FEINTCallback[ErrSource])
            {
                /* Interrupt unmasked, but no Interrupt Vector defined! */
                R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE); 
            }
            else
            {
                r_ecm_Dev[Unit].FEINTCallback[ErrSource]();
                R_ECM_ClearErrorSourceStatus(Unit, ErrSource);
            }
        }
    }
}
