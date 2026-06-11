/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_ecm_main.c 4537 2014-12-12 10:23:23Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for the ECM macro
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
  Title: ECM Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_dev_err_types.h"

#include "r_config_ecm.h"
#include "r_ecm_api.h"
#define R_ECM_MAIN_GLOBALS_
#include "r_ecm_main.h"
#include "r_ecm_sys.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_EcmVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_EcmDrvVer[] = R_VERSION(ECM, R_ECM_VERSION_HI, R_ECM_VERSION_LO);



/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;




/*******************************************************************************
  Section: Local Functions
*/


/*******************************************************************************
  Function: loc_ConfigureErrorSourceBitMaskRegister

  Read-Modify-Write operation for protected registers.
  Will set all bits that are defined by mask to <0> or <1> as defined by <SetClear>.

  Parameters:
  ProtAddr      - Protection Command Register
  Addr          - Register to be altered
  Mask          - Bits to be altered
  SetClear      - Select if the bits defined by <Mask> are all set to <1> or to <0>
  
  Returns:
  void
*/

void loc_ChangeProtectedRegisterBit(uint32_t Unit, uint32_t ProtAddr, uint32_t Addr, uint32_t Mask, uint32_t SetClear);
void loc_ChangeProtectedRegisterBit(uint32_t Unit, uint32_t ProtAddr, uint32_t Addr, uint32_t Mask, uint32_t SetClear)
{
    uint32_t regVal = 0;
    uint32_t setVal = 0;
    uint32_t retry  = 10;
    uint32_t leave  = 0;
    
    if (0 != SetClear)
    {
        setVal = 0xffffffffu;
    }
    
    regVal = R_ECM_READ_REG(32, Addr);
    
    regVal &= (~Mask); /* Clear Masked Bit */
    regVal |= (Mask & setVal); /* Write Value To Masked Bit */
    
    /* The write sequence for protected registers might be disrupted by interrupts, 
       that also write to the ECM. 
       Try several times to write the new value to the register, then fail. */
    for (; 0 == leave;) 
    {
        /* Write value */
        R_ECM_WRITE_PROT_REG(ProtAddr, Addr, regVal);
        
        /* Check for success and leave if result is positive */
        if (0 == R_ECM_CheckProtectionCommandStatus(Unit))
        {
            leave = 1u;
        }
        
        /* Stop and report error if still unsuccessful after a certain number of retries. */
        if (0 == retry)
        {
            R_ECM_ErrorHandler(Unit, R_ECM_ERR_NG);
            leave = 1u;
        }
        
        retry--;
    }
}


/*******************************************************************************
  Function: loc_CheckProtectionCommandStatus

  Check the values of the supplied protection command status register

  Parameters:
  Addr      - Protection Command Status Register
  
  Returns:
  uint32_t
*/

uint32_t loc_CheckProtectionCommandStatus(uint32_t Addr);
uint32_t loc_CheckProtectionCommandStatus(uint32_t Addr)
{
    uint8_t regVal = R_ECM_READ_REG(8, Addr);
    
    if (1 == regVal)
    {
        return 1;
    }
    
    return 0;
}


/*******************************************************************************
  Function: loc_ConfigureErrorSourceBitMaskRegister

  There are several registers that all contain the same bitmasks.
  This is a univeral function to set the bit to <Enable> that corresponds to a certain error source.
  
  There are always two successive registers for the bitmasks.
  If the supplied <ErrSource> belongs to the second register, 
  the supplied <RegOffset> will be adjusted automatically.
  <ProtOffset> provides the offset address to the Protection Command 
  Register that is associated with the Configuration Register.

  Parameters:
  Unit          - Macro Instance number
  ErrSource     - Error source to be configured to the value of <enable>
  Enable        - Select if the Bit is set (1) or cleared (0)
  RegOffset     - Configuration Register Offset
  ProtOffset    - Protection Command Register Offset
  
  Returns:
  uint32_t
*/

r_ecm_Error_t loc_ConfigureErrorSourceBitMaskRegister(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t Enable, uint32_t RegOffset, uint32_t ProtOffset);
r_ecm_Error_t loc_ConfigureErrorSourceBitMaskRegister(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t Enable, uint32_t RegOffset, uint32_t ProtOffset)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;
    uint32_t regNo, regMask;
    uint32_t RegAddr, ProtAddr;
    
    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        regNo   = R_ECM_GetRegisterNoFromErrSource(Unit, ErrSource);
        regMask = R_ECM_GetRegisterMaskFromErrSource(Unit, ErrSource);
        
        RegAddr  = base + RegOffset;
        ProtAddr = base + ProtOffset;
        
        /* There are always two configuration registers as we have more than 32 
           but less than 64 error sources and need one bit for each source 
           
           This function always expects to get the Register0 of the configuration
           to be changed. For error sources greater than 32, the register address
           is increased by 0x04. */
        RegAddr += 4u*regNo;
        
        loc_ChangeProtectedRegisterBit(Unit, ProtAddr, RegAddr, regMask, Enable);
    }

    return err;
}


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_ECM_CheckMacroParams
  
  See: <r_ecm_main.h>
*/
r_ecm_Error_t R_ECM_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_ECM_MACRO_NUM <= Unit)
    {
        R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE);
        return  R_ECM_ERR_RANGE; 
    }
    return R_ECM_ERR_OK;  
}

/*******************************************************************************
  Function: R_ECM_ErrorHandler
  
  See: <r_ecm_main.h>
*/

void R_ECM_ErrorHandler(uint32_t Unit, r_ecm_Error_t Error) 
{
    if (0 != loc_ErrorHandler) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t) Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for(;;) 
        {        /* we don't have a handler */
        }        /* so, let's wait here */
    }
}

/*******************************************************************************
  Function: R_ECM_CheckProtectionCommandStatus

  See: <r_ecm_main.h> 
*/

uint32_t R_ECM_CheckProtectionCommandStatus(uint32_t Unit)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;

    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        return loc_CheckProtectionCommandStatus(base + R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PS);
    }
}


/*******************************************************************************
  Function: R_ECM_GetRegisterNoFromErrSource

  See: <r_ecm_main.h> 
*/

uint32_t R_ECM_GetRegisterNoFromErrSource(uint32_t Unit, r_dev_EcmErrSel_t ErrSource)
{
    uint32_t regNo = 0;
    
    if (R_DEV_ECMERR_LAST <= ErrSource)
    {
        R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE);
    }
    
    /* R_DEV_ERR_CLMATI4 should be: (int)ErrSource == 31 */
    if (31u >= (uint32_t)ErrSource)
    {
        regNo = 0;
    }
    else
    {
        regNo = 1;
    }
    
    return regNo;
}

/*******************************************************************************
  Function: R_ECM_GetRegisterMaskFromErrSource

  See: <r_ecm_main.h> 
*/

uint32_t R_ECM_GetRegisterMaskFromErrSource(uint32_t Unit, r_dev_EcmErrSel_t ErrSource)
{
    uint32_t regMask = 0;
    uint32_t intNo = (uint32_t)ErrSource;
    
    if (R_DEV_ECMERR_LAST <= ErrSource)
    {
        R_ECM_ErrorHandler(Unit, R_ECM_ERR_RANGE);
    }

    /* Every 32 interrupts are handled by a different register, so bit-numbering restarts with zero. */
    intNo %= 32;
    
    regMask = (uint32_t)((uint32_t)1u << intNo);
    
    return regMask;
}





/*******************************************************************************
  Section: Global API Functions
  
  See: <r_ecm_api.h>
*/



/*******************************************************************************
  Function: R_ECM_GetVersionStr

  See: <r_ecm_api.h> 
*/
const int8_t * R_ECM_GetVersionStr(void) 
{
    return (const int8_t *) loc_EcmDrvVer;
}


/*******************************************************************************
  Function: R_ECM_GetMajorVersion

  See: <r_ecm_api.h> 
*/
const uint16_t R_ECM_GetMajorVersion(void) 
{
    return R_ECM_VERSION_HI; 
}


/*******************************************************************************
  Function: R_ECM_GetMinorVersion

  See: <r_ecm_api.h> 
*/
const uint16_t R_ECM_GetMinorVersion(void) 
{
    return R_ECM_VERSION_LO; 
}


/*******************************************************************************
  Function: R_ECM_Init

  See: <r_ecm_api.h> 
*/

 r_ecm_Error_t R_ECM_Init(uint32_t Unit) 
 {
    r_dev_EcmErrSel_t    ErrSource;
    r_ecm_ErrSrcConfig_t ErrSrcConfig;
    r_ecm_Error_t        err;

    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }

    if (0 == r_ecm_Dev[Unit].BaseAddr)
    {
        r_ecm_Dev[Unit].BaseAddr = R_ECM_SYS_BaseAddr(Unit);
        
        ErrSrcConfig.FEINT      = 0;
        ErrSrcConfig.FENMI      = 0;
        ErrSrcConfig.FEINTTimer = 0;
        ErrSrcConfig.FENMITimer = 0;
        ErrSrcConfig.Reset      = 0;
        ErrSrcConfig.Mask       = 0;
        
        for (ErrSource = R_DEV_ECMERR_FIRST; R_DEV_ECMERR_LAST > ErrSource; ErrSource++)
        {
            if ( (R_DEV_ECMERR_WDTA0 == ErrSource) 
              || (R_DEV_ECMERR_WDTA1 == ErrSource) 
              || (R_DEV_ECMERR_NMI   == ErrSource) )
            {
                ErrSrcConfig.Reset = 1u;
            }
            else
            {
                ErrSrcConfig.Reset = 0;
            }
            ErrSrcConfig.ErrSource                   = ErrSource; 
            r_ecm_Dev[Unit].FEINTCallback[ErrSource] = 0;
            r_ecm_Dev[Unit].FENMICallback[ErrSource] = 0;
            R_ECM_SetErrorSourceConfig(Unit, &ErrSrcConfig);
            
            /* TODO: Rest of the registers */
        }
        
        err = R_ECM_SYS_HardwareInit(Unit);
    }
    else 
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_LOCKED; 
        R_ECM_ErrorHandler(Unit, err);
    }
    return err;
}


/*******************************************************************************
  Function: R_ECM_DeInit

  See: <r_ecm_api.h> 
*/

 r_ecm_Error_t R_ECM_DeInit(uint32_t Unit)
 {
    r_dev_EcmErrSel_t    ErrSource;
    r_ecm_ErrSrcConfig_t ErrSrcConfig;
    r_ecm_Error_t err;

    err = R_ECM_CheckMacroParams(Unit); 

    if (R_ECM_ERR_OK  != err) 
    {
        return err; 
    }
    if (0 != r_ecm_Dev[Unit].BaseAddr)
    {
        ErrSrcConfig.FEINT      = 0;
        ErrSrcConfig.FENMI      = 0;
        ErrSrcConfig.FEINTTimer = 0;
        ErrSrcConfig.FENMITimer = 0;
        ErrSrcConfig.Reset      = 0;
        ErrSrcConfig.Mask       = 0;

        for (ErrSource = R_DEV_ECMERR_FIRST; R_DEV_ECMERR_LAST > ErrSource; ErrSource++)
        {
            r_ecm_Dev[Unit].FEINTCallback[ErrSource] = 0;
            r_ecm_Dev[Unit].FENMICallback[ErrSource] = 0;
            ErrSrcConfig.ErrSource                   = ErrSource; 
            R_ECM_SetErrorSourceConfig(Unit, &ErrSrcConfig);
        }

        err = R_ECM_SYS_HardwareDeInit(Unit);
        
        r_ecm_Dev[Unit].BaseAddr = 0;
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_ECM_ERR_NOTLOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_ECM_SetErrorCallback

  See: <r_ecm_api.h> 
*/

void R_ECM_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}


/*******************************************************************************
  Function: R_ECM_SetErrorOutPinMode

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_SetErrorOutPinMode(uint32_t Unit, r_ecm_ErrorPinMode_t EPMode)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;
    uint32_t enable = 0;
    uint32_t RegAddr, ProtAddr;

    uint32_t RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_EPCFG;
    uint32_t ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        if (R_ECM_EP_Dynamic == EPMode)
        {
            enable = 1u;
        }

        RegAddr  = base + RegOffset;
        ProtAddr = base + ProtOffset;
        
        loc_ChangeProtectedRegisterBit(Unit, ProtAddr, RegAddr, 0x01, enable);
    }

    return err;
}


/*******************************************************************************
  Function: R_ECM_SetErrorOutPulseMode

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_SetErrorOutPulseMode(uint32_t Unit, r_ecm_ErrorPulseMode_t EPMode)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint8_t regVal = 0;

    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        if (R_ECM_EPM_OSTM == EPMode)
        {
            regVal = 1u;
        }

        /* This register has the base address included */
        R_ECM_WRITE_REG(32, /*No "base + ", this is a direct address!*/ R_ECM_REG_EPCTL, regVal);
    }

    return err;
}


/*******************************************************************************
  Function: R_ECM_StopDelayTimer

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_StopDelayTimer(uint32_t Unit)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;
    uint32_t RegAddr, ProtAddr;

    uint32_t RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_DTMCTL;
    uint32_t ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        RegAddr  = base + RegOffset;
        ProtAddr = base + ProtOffset;
        
        loc_ChangeProtectedRegisterBit(Unit, ProtAddr, RegAddr, 0x02, 1u);
    }

    return err;
}


/*******************************************************************************
  Function: R_ECM_GetDelayTimerStatus

  See: <r_ecm_api.h> 
*/

uint32_t R_ECM_GetDelayTimerStatus(uint32_t Unit, uint32_t* TimerValue)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;
    uint32_t RegVal;
    uint32_t Status;

    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return 0; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return 0;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        RegVal = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_DTMCTL);
        
        if (0 != TimerValue)
        {
            *TimerValue = R_ECM_READ_REG(32, base + R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_DTMR);
        }

        if (0 == (0x01u & RegVal))
        {
            /* Timer stopped */
            Status = 0;
        }
        else
        {
            /* Timer running */
            Status = 1;
        }
    }

    return Status;
}


/*******************************************************************************
  Function: R_ECM_SetErrorOutput
  
  Sets the state of the error output pin to ERROR
  
  To change the state of the error output pin, both master and checker area have to be set individually.
  As this cannot happen in parallel, master and checker are in different states temporarily.
  This will trigger the compare error. So this error mus tbe masked before starting the operation.
  
  Sequence:
    (1) Save ECM compare error settings.
    (2) Disable ECM compare error settings: Interrupts; Reset; Error Mask
    (3) Set Master and Checker error output pin state
    (4) Clear ECM compare error
    (5) Restore ECM compare error settings.

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_SetErrorOutput(uint32_t Unit, r_ecm_ErrorPinState_t State)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;
    uint32_t MasterRegAddr, MasterProtAddr;
    uint32_t CheckerRegAddr, CheckerProtAddr;

    uint32_t MasterRegOffset =   R_ECM_BASE_OFFSET_MASTER + R_ECM_REG_MESET;
    uint32_t MasterProtOffset =  R_ECM_BASE_OFFSET_MASTER + R_ECM_REG_MPCMD0;
    uint32_t CheckerRegOffset =  R_ECM_BASE_OFFSET_CHECKER + R_ECM_REG_CESET;
    uint32_t CheckerProtOffset = R_ECM_BASE_OFFSET_CHECKER + R_ECM_REG_CPCMD0;
    
    r_ecm_ErrSrcConfig_t INTECMConfig, INTECMConfigBackup;
    
    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        MasterRegAddr  = base + MasterRegOffset;
        MasterProtAddr = base + MasterProtOffset;
        CheckerRegAddr  = base + CheckerRegOffset;
        CheckerProtAddr = base + CheckerProtOffset;
        
        /* To clear the error, we need to write to CLR instear of ST register */
        if (R_ECM_EPS_NoError == State)
        {
            MasterRegAddr += 4u;
            CheckerRegAddr += 4u;
        }
        
        /* Backup current error source configuration */
        INTECMConfigBackup = R_ECM_GetErrorSourceConfig(Unit, R_DEV_ECMERR_ECM);
        
        /* Disable error generation for error source */
        INTECMConfigBackup.ErrSource = R_DEV_ECMERR_ECM; 
        INTECMConfig = INTECMConfigBackup;
        INTECMConfig.FEINT = 0u;
        INTECMConfig.FENMI = 0u;
        INTECMConfig.FEINTTimer = 0u;
        INTECMConfig.FENMITimer = 0u;
        INTECMConfig.Reset = 0u;
        INTECMConfig.Mask = 0u;
        R_ECM_SetErrorSourceConfig(Unit, &INTECMConfig);
        
        /* Set state of error putput pin */
        loc_ChangeProtectedRegisterBit(Unit, MasterProtAddr, MasterRegAddr, 0x01u, 1u);
        loc_ChangeProtectedRegisterBit(Unit, CheckerProtAddr, CheckerRegAddr, 0x01u, 1u);
        
        /* Restore error source configuration */
        R_ECM_SetErrorSourceConfig(Unit, &INTECMConfigBackup);
    }

    return err;
}


/*******************************************************************************
  Function: R_ECM_SetDelayTimerCompareValue

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_SetDelayTimerCompareValue(uint32_t Unit, uint32_t CompareValue)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;
    uint32_t RegAddr, ProtAddr;

    uint32_t RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_DTMCMP;
    uint32_t ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else if(0xffffu < CompareValue)
    {
        err = R_ECM_ERR_RANGE; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        RegAddr  = base + RegOffset;
        ProtAddr = base + ProtOffset;
        
        loc_ChangeProtectedRegisterBit(Unit, ProtAddr, RegAddr, 0xffff, CompareValue);
    }

    return err;
}


/*******************************************************************************
  Function: R_ECM_GetErrorSourceConfig

  See: <r_ecm_api.h> 
*/

r_ecm_ErrSrcConfig_t R_ECM_GetErrorSourceConfig(uint32_t Unit, r_dev_EcmErrSel_t ErrSource)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t base;
    uint32_t regNo, regMask;
    uint32_t regVal;

    r_ecm_ErrSrcConfig_t ErrSourceStatus;
    ErrSourceStatus.ErrSource = ErrSource;
    ErrSourceStatus.FEINT = 0;
    ErrSourceStatus.FENMI = 0;
    ErrSourceStatus.FEINTTimer = 0;
    ErrSourceStatus.FENMITimer = 0;
    ErrSourceStatus.Reset = 0;
    ErrSourceStatus.Mask = 0;

    
    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return ErrSourceStatus; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        /* please, no double-init*/
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return ErrSourceStatus;
    }
    else
    {
        base = r_ecm_Dev[Unit].BaseAddr;
        
        regNo   = R_ECM_GetRegisterNoFromErrSource(Unit, ErrSource);
        regMask = R_ECM_GetRegisterMaskFromErrSource(Unit, ErrSource);
        
        regVal =  R_ECM_READ_REG(32, (base + R_ECM_BASE_OFFSET_COMMON) + (R_ECM_REG_MICFG0  + (4u * regNo)));
        if (0 != (regVal & regMask))
        {
            ErrSourceStatus.FEINT = 1u;
        }
        
        regVal =  R_ECM_READ_REG(32, (base + R_ECM_BASE_OFFSET_COMMON) + (R_ECM_REG_NMICFG0 + (4u * regNo)));
        if (0 != (regVal & regMask))
        {
            ErrSourceStatus.FENMI = 1u;
        }
        
        regVal =  R_ECM_READ_REG(32, (base + R_ECM_BASE_OFFSET_COMMON) + (R_ECM_REG_IRCFG0  + (4u * regNo)));
        if (0 != (regVal & regMask))
        {
            ErrSourceStatus.Reset = 1u;
        }
        
        regVal =  R_ECM_READ_REG(32, (base + R_ECM_BASE_OFFSET_COMMON) + (R_ECM_REG_EMK0    + (4u * regNo)));
        if (0 != (regVal & regMask))
        {
            ErrSourceStatus.Mask = 1u;
        }
        
        regVal =  R_ECM_READ_REG(32, (base + R_ECM_BASE_OFFSET_COMMON) + (R_ECM_REG_DTMCFG0 + (4u * regNo)));
        if (0 != (regVal & regMask))
        {
            ErrSourceStatus.FEINTTimer = 1u;
        }
        
        regVal =  R_ECM_READ_REG(32, (base + R_ECM_BASE_OFFSET_COMMON) + (R_ECM_REG_DTMCFG2 + (4u * regNo)));
        if (0 != (regVal & regMask))
        {
            ErrSourceStatus.FENMITimer = 1u;
        }
    }

    return ErrSourceStatus;
}


/*******************************************************************************
  Function: R_ECM_SetErrorSourceConfig

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_SetErrorSourceConfig(uint32_t Unit, r_ecm_ErrSrcConfig_t *ErrSourceConfig)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;

    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        /* The <err> is for debugging, as errors usually end int he error handler */
        err = R_ECM_EnableFEINT(Unit, ErrSourceConfig->ErrSource, ErrSourceConfig->FEINT);
        err = R_ECM_EnableFENMI(Unit, ErrSourceConfig->ErrSource, ErrSourceConfig->FENMI);
        err = R_ECM_EnableReset(Unit, ErrSourceConfig->ErrSource, ErrSourceConfig->Reset);
        err = R_ECM_EnableMask (Unit, ErrSourceConfig->ErrSource, ErrSourceConfig->Mask );
        err = R_ECM_EnableFEINTTimer(Unit, ErrSourceConfig->ErrSource, ErrSourceConfig->FEINTTimer);
        err = R_ECM_EnableFENMITimer(Unit, ErrSourceConfig->ErrSource, ErrSourceConfig->FENMITimer);
    }

    return err;
}


/*******************************************************************************
  Function: R_ECM_ConfigureAllErrorSources

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_ConfigureAllErrorSources(uint32_t Unit, r_ecm_ErrSrcConfig_t *ErrSourceConfig, uint32_t Count)
{
    r_ecm_Error_t err = R_ECM_ERR_OK;
    uint32_t index = 0;

    err = R_ECM_CheckMacroParams(Unit); 
    if (R_ECM_ERR_OK != err) 
    {
        R_ECM_ErrorHandler(Unit, err);
        return err; 
    }
    else if (0 == r_ecm_Dev[Unit].BaseAddr)
    {   
        err = R_ECM_ERR_NOTLOCKED; 
        R_ECM_ErrorHandler(Unit, err);
        return err;
    }
    else
    {
        for (index = 0; index < Count; index++)
        {
            /* The <err> is for debugging, as errors usually end int he error handler */
            err = R_ECM_SetErrorSourceConfig(Unit, &(ErrSourceConfig[index]));
        }
    }

    return err;
}



/* TODO: Some of these functions have a bit more that is available for manipulation.
         Add these corner cases! */
         
/*******************************************************************************
  Function: R_ECM_EnableFENMITimer

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_EnableFENMITimer(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_DTMCFG2;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, enable, RegOffset, ProtOffset);

    return err;
}

/*******************************************************************************
  Function: R_ECM_EnableFEINTTimer

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_EnableFEINTTimer(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_DTMCFG0;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, enable, RegOffset, ProtOffset);

    return err;
}

/*******************************************************************************
  Function: R_ECM_TriggerPseudoError

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_TriggerPseudoError(uint32_t Unit, r_dev_EcmErrSel_t ErrSource)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PE0;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, 1u, RegOffset, ProtOffset);

    return err;
}


/*******************************************************************************
  Function: R_ECM_ClearErrorSourceStatus

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_ClearErrorSourceStatus(uint32_t Unit, r_dev_EcmErrSel_t ErrSource)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_ESSTC0;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, 1u, RegOffset, ProtOffset);

    return err;
}


/*******************************************************************************
  Function: R_ECM_EnableMask

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_EnableMask(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_EMK0;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, enable, RegOffset, ProtOffset);

    return err;
}


/*******************************************************************************
  Function: R_ECM_EnableReset

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_EnableReset(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_IRCFG0;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, enable, RegOffset, ProtOffset);

    return err;
}


/*******************************************************************************
  Function: R_ECM_EnableFENMI

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_EnableFENMI(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_NMICFG0;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, enable, RegOffset, ProtOffset);

    return err;
}


/*******************************************************************************
  Function: R_ECM_EnableFEINT

  See: <r_ecm_api.h> 
*/

r_ecm_Error_t R_ECM_EnableFEINT(uint32_t Unit, r_dev_EcmErrSel_t ErrSource, uint32_t enable)
{
    r_ecm_Error_t err;
    uint32_t RegOffset, ProtOffset;
    
    RegOffset =  R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_MICFG0;
    ProtOffset = R_ECM_BASE_OFFSET_COMMON + R_ECM_REG_PCMD1;
    
    err = loc_ConfigureErrorSourceBitMaskRegister(Unit, ErrSource, enable, RegOffset, ProtOffset);

    return err;
}


/* END */
