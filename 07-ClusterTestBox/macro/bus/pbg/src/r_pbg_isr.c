/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_pbg_isr.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic XC GUARD driver
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
  Title: XC GUARD Interrupt Functions

  Handle the XC GUARD interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"

#include "r_config_pbg.h"
#include "r_pbg_api.h"
#include "r_pbg_sys.h"
#include "r_pbg_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <r_pbg_api.h>
*/

#if 0
/*******************************************************************************
  Function: R_PBG_EnableInt
*/

void R_PBG_EnableInt(uint32_t Unit, r_pbg_Interrupt_t Interrupt) 
{
    uint32_t base;
    r_pbg_RegINT1L_t int1l;
    r_pbg_Error_t err; 
    
    err = R_PBG_CheckMacroParams(Unit);
    if ((R_PBG_ERR_OK != err) || (Interrupt >= R_PBG_INT_NUM)) 
    {
        R_PBG_ErrorHandler(Unit, R_PBG_ERR_RANGE); 
        return; 
    } 
    
    base = r_pbg_Dev[Unit].BaseAddr;
    int1l.Word = R_PBG_READ32(base + R_PBGnINT1L);
    if (R_PBG_INT_WG == Interrupt)
    {
        int1l.Bit.WINTEN = 1;
    }
    if (R_PBG_INT_RG == Interrupt)
    {
        int1l.Bit.RINTEN = 1;
    }
    R_PBG_WRITE32(base + R_PBGnINT1L, int1l.Word);
    
    R_PBG_SYS_EnableInt(Unit, Interrupt); 
}


/*******************************************************************************
  Function: R_PBG_DisableInt
*/

void R_PBG_DisableInt(uint32_t Unit, r_pbg_Interrupt_t Interrupt) 
{
    uint32_t base;
    r_pbg_RegINT1L_t int1l;
    r_pbg_Error_t err; 
    
    err = R_PBG_CheckMacroParams(Unit);
    if ((R_PBG_ERR_OK != err) || (Interrupt >= R_PBG_INT_NUM)) 
    {
        R_PBG_ErrorHandler(Unit, R_PBG_ERR_RANGE); 
        return; 
    } 

    base = r_pbg_Dev[Unit].BaseAddr;
    int1l.Word = R_PBG_READ32(base + R_PBGnINT1L);
    if (R_PBG_INT_WG == Interrupt)
    {
        int1l.Bit.WINTEN = 0;
    }
    if (R_PBG_INT_RG == Interrupt)
    {
        int1l.Bit.RINTEN = 0;
    }
    R_PBG_WRITE32(base + R_PBGnINT1L, int1l.Word);
    
/*    R_PBG_SYS_DisableInt(Unit, Interrupt);     */
}

/*******************************************************************************
  Function: R_PBG_SetIsrCallback
*/

 r_pbg_Error_t R_PBG_SetIsrCallback(uint32_t Unit, r_pbg_Interrupt_t Interrupt, r_pbg_isrCallback_t Isr) 
{
    r_pbg_Error_t err; 
    err = R_PBG_CheckMacroParams(Unit);
    if ((R_PBG_ERR_OK != err) || (Interrupt >= R_PBG_INT_NUM)) 
    {
        R_PBG_ErrorHandler(Unit, R_PBG_ERR_RANGE); 
        return R_PBG_ERR_RANGE; 
    } 
        
    r_pbg_Dev[Unit].IsrCallback[Interrupt] = Isr;
    return R_PBG_ERR_OK;
}


/*******************************************************************************
  Function: R_PBG_IsrWriteErr
*/

void R_PBG_IsrErr(void) 
{
    /* r_pbg_Error_t err; */
    uint32_t instance = 0;

    uint32_t base;
    r_pbg_RegINT1L_t     int1l;
    r_pbg_RegOVF_t       ovf;
    uint32_t             errAddress;
    r_pbg_RegERRRTYPE_t  errType;

    /* Handle Interrupt - Step 1: Find instance that caused the interrupt */
    for (instance = 0; instance < R_PBG_INSTANCE_NUM; instance++)
    {
        base = r_pbg_Dev[instance].BaseAddr;
        int1l.Word = R_PBG_READ32(base + R_PBGnINT1L);
        
        /* Write Error Interrupt occured! */
        if(1 == int1l.Bit.WINTSTAT)
        {
            errAddress   = R_PBG_READ32(base + R_PBGnERRWADDR);
            errType.Word = R_PBG_READ32(base + R_PBGnERRWTYPE);
            ovf.Word     = R_PBG_READ32(base + R_PBGnOVF);
            
            int1l.Bit.WINTSTAT = 1; /* Clear Write Error Interrupt */
            int1l.Bit.RINTSTAT = 0; /* Do not touch read error Interrupt */
            R_PBG_WRITE32(base + R_PBGnINT1L, int1l.Word);
            
            r_pbg_Dev[instance].IsrCallback[R_PBG_INT_WG](errAddress, errType.Word, ovf.Bit.WOF, instance);
        }

        /* Read Error Interrupt occured! */
        if(1 == int1l.Bit.RINTSTAT)
        {
            errAddress   = R_PBG_READ32(base + R_PBGnERRRADDR);
            errType.Word = R_PBG_READ32(base + R_PBGnERRRTYPE);
            ovf.Word     = R_PBG_READ32(base + R_PBGnOVF);
            
            int1l.Bit.RINTSTAT = 1; /* Clear Read Error Interrupt */
            int1l.Bit.WINTSTAT = 0; /* Do not touch write error Interrupt */
            R_PBG_WRITE32(base + R_PBGnINT1L, int1l.Word);
            
            r_pbg_Dev[instance].IsrCallback[R_PBG_INT_RG](errAddress, errType.Word, ovf.Bit.ROF, instance);
        }

    }
}

#endif

/* Not sure wether to keep it for better understanding */
#if 0
/*******************************************************************************
  Function: R_PBG_IsrWriteErr
*/

void R_PBG_IsrWriteErr(uint32_t Unit) 
{
    r_pbg_Error_t err; 
    uint32_t instance = 0;

    err = R_PBG_CheckMacroParams(Unit);
     
    /* Currently there is only one vector for all instances, so this is more or less useless */
    if ((R_PBG_ERR_OK != err)) 
    {
        R_PBG_ErrorHandler(Unit, R_PBG_ERR_RANGE); 
        return; 
    }
    else
    {
        uint32_t base;
        r_pbg_RegINT1L_t     int1l;
        r_pbg_RegOVF_t       ovf;
        uint32_t             errAddress;
        r_pbg_RegERRRTYPE_t  errType;

        /* Handle Interrupt - Step 1: Find instance that caused the interrupt */
        for (instance = 0; instance < R_PBG_INSTANCE_NUM; instance++)
        {
            base = r_pbg_Dev[instance].BaseAddr;
            int1l.Word = R_PBG_READ32(base + R_PBGnINT1L);
            
            /* Interrupt occured! */
            if(1 == int1l.Bit.WINTSTAT)
            {
                errAddress   = R_PBG_READ32(base + R_PBGnERRWADDR);
                errType.Word = R_PBG_READ32(base + R_PBGnERRWTYPE);
                ovf.Word     = R_PBG_READ32(base + R_PBGnOVF);
                
                int1l.Bit.WINTSTAT = 1; /* Clear Write Error Interrupt */
                int1l.Bit.RINTSTAT = 0; /* Do not touch read error Interrupt */
                R_PBG_WRITE32(base + R_PBGnINT1L, int1l.Word);
                
                r_pbg_Dev[instance].IsrCallback[R_PBG_INT_WG](errAddress, errType.Word, ovf.Bit.WOF);
            }
        }
    }
}


/*******************************************************************************
  Function: R_PBG_IsrReadErr
*/

void R_PBG_IsrReadErr(uint32_t Unit) 
{
    r_pbg_Error_t err; 
    uint32_t instance = 0;

    err = R_PBG_CheckMacroParams(Unit);
     
    /* Currently there is only one vector for all instances, so this is more or less useless */
    if ((R_PBG_ERR_OK != err)) 
    {
        R_PBG_ErrorHandler(Unit, R_PBG_ERR_RANGE); 
        return; 
    }
    else
    {
        uint32_t base;
        r_pbg_RegINT1L_t     int1l;
        r_pbg_RegOVF_t       ovf;
        uint32_t             errAddress;
        r_pbg_RegERRRTYPE_t  errType;

        /* Handle Interrupt - Step 1: Find instance that caused the interrupt */
        for (instance = 0; instance < R_PBG_INSTANCE_NUM; instance++)
        {
            base = R_PBG_SYS_BaseAddr(instance);
            int1l.Word = R_PBG_READ32(base + R_PBGnINT1L);

            /* Interrupt occured! */
            if(1 == int1l.Bit.RINTSTAT)
            {
                errAddress   = R_PBG_READ32(base + R_PBGnERRRADDR);
                errType.Word = R_PBG_READ32(base + R_PBGnERRRTYPE);
                ovf.Word     = R_PBG_READ32(base + R_PBGnOVF);
                
                int1l.Bit.RINTSTAT = 1; /* Clear Read Error Interrupt */
                int1l.Bit.WINTSTAT = 0; /* Do not touch Write Error Interrupt */
                R_PBG_WRITE32(base + R_PBGnINT1L, int1l.Word);
                R_PBG_WRITE32(base + R_PBGnOVF, 0x00000001);    /* Clear Read OVF flag */
                
                if(0 == r_pbg_Dev[instance].BaseAddr)
                {
                    /* Interrupt casued by uninitialised instance! Bad! */
                    R_PBG_ErrorHandler(Unit, R_PBG_ERR_UNINITIALISED); 
                }
                else
                {
                    r_pbg_Dev[instance].IsrCallback[R_PBG_INT_RG](errAddress, errType.Word, ovf.Bit.ROF);
                }
            }
        }
    }
}
#endif
