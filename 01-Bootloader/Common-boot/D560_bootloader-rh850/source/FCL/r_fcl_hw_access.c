#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RH850 devices, based on the RV40 Flash technology
*
* File Name     : $Source: r_fcl_hw_access.c $
* Lib. Version  : $RH850_FCL_LIB_VERSION_T01: V2.00 $
* Mod. Revision : $Revision: 1.64 $
* Mod. Date     : $Date: 2014/09/01 13:40:55MESZ $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : FCL hardware interface functions (c-code)
**********************************************************************************************************************/

/**********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only  intended for use with
* Renesas products. No other uses are authorized. This software is owned by Renesas Electronics
* Corporation and is protected under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR
* ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the
* availability of this software. By using this software, you agree to the additional terms and conditions
* found by accessing the  following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_ON 0292
#endif

/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 3.1 (QAC message 0292)
* Reason:       To support automatic insertion of revision, module name etc. by the source
*               revision control system it is necessary to violate the rule, because the
*               system uses non basic characters as placeholders.
* Verification: The placeholders are used in commentars only. Therefore rule violation cannot
*               influency code compilation.
**********************************************************************************************************************/

/*********************************************************************************************************************
 * MISRA Rule:   MISRA-C 2004 rule 8.10 (QAC message 1503)
 * Reason:       This function is not used within the library, it is only used by the user
 *               application.
 * Verification: This function is part of the library API and therefore essential on the one hand.
 *               On the other hand the QAC warning is correct, because this function is never
 *               called within the library. To solve this dillemma it is neccessary and suitable
 *               to disable the message.
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * MISRA Rule:   MISRA-C 2004 rule 11.3 (QAC message 0303)
 * Reason:       For effective embedded programming, integer to pointer conversions are used
 * Verification: The converted addresses are essential for complete code execution. Incorrect
 *               conversion would result in test fails.
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * MISRA Rule:   MISRA-C 2004 rule 11.3 (QAC message 0306)
 * Reason:       For effective embedded programming, integer to pointer conversions are used
 * Verification: The converted addresses are essential for complete code execution. Incorrect
 *               conversion would result in test fails.
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * MISRA Rule:   QAC message 2814
 * Reason:       Internal library functions used with pointers passed as arguments.
 * Verification: Code review, ensure that before each function call the pointers are initialized.
 *********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_typedefs.h"
#include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl_global.h"
#include "r_fcl_env.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Exported global variables (to be accessed by other files)
**********************************************************************************************************************/
#if (defined FCL_TEST_PATCH)
    TestPatch_t TestPatch;
#endif


/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
R_FCL_STATIC uint8_t        R_FCL_I_read_memory_u08            (const uint32_t addr_u32);
R_FCL_STATIC uint16_t       R_FCL_I_read_memory_u16            (const uint32_t addr_u32);
R_FCL_STATIC uint32_t       R_FCL_I_read_memory_u32            (const uint32_t addr_u32);
R_FCL_STATIC void           R_FCL_I_write_memory_u16           (const uint32_t addr_u32, const uint16_t val_u16);
R_FCL_STATIC void           R_FCL_I_write_memory_u32           (const uint32_t addr_u32, const uint32_t val_u32);
R_FCL_STATIC void           R_FCL_I_write_unaligned_memory_u32 (const uint32_t addr_u32, const uint32_t val_u32);
R_FCL_STATIC void           R_FCL_FCUFct_ForcedStop            (void);
R_FCL_STATIC void           R_FCL_FCUFct_RestoreLastState      (void);
R_FCL_STATIC void           R_FCL_FCUFct_ClearStatus           (void);
R_FCL_STATIC void           R_FCL_FCUFct_GetDeviceInfo         (void);
R_FCL_STATIC r_fcl_status_t R_FCL_FCUFct_VerifyID              (void);
R_FCL_STATIC void           R_FCL_FCUFct_VerifyChecksum        (void);
R_FCL_STATIC void           R_FCL_FCUFct_SetFrequency          (void);
R_FCL_STATIC r_fcl_status_t R_FCL_FCUFct_SwitchMode_Check      (void);
R_FCL_STATIC r_fcl_status_t R_FCL_FCUFct_GetResult             (r_fcl_flag_t cmdSpdExec_enu);
R_FCL_STATIC r_fcl_flag_t   R_FCL_FCUFct_ChkSuspended          (void);

/**********************************************************************************************************************
 * Function name: R_FCL_I_read_memory_u32
 *********************************************************************************************************************/
/**
 * Function to read a 32-bit IO register or memory
 *
 * @param[in]     addr_u32       source address
 * @return        32-bit register contents
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC uint32_t R_FCL_I_read_memory_u32 (const uint32_t addr_u32)
{
    #if (defined FCL_TEST_PATCH)
        return (test_cb_read_u32 (addr_u32));
    #else
        return ( *( (volatile uint32_t *)(addr_u32) ) );                                          /* PRQA S 0303,2814 */
    #endif
}
/**********************************************************************************************************************
 End of function R_FCL_I_read_memory_u32
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_I_read_memory_u16
 *********************************************************************************************************************/
/**
 * Function to read a 16-bit IO register or memory
 *
 * @param[in]     addr_u32       source address
 * @return        16-bit register contents
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC uint16_t R_FCL_I_read_memory_u16 (const uint32_t addr_u32)
{
    #if (defined FCL_TEST_PATCH)
        return (test_cb_read_u16 (addr_u32));
    #else
        return ( *( (volatile uint16_t *)(addr_u32) ) );                                          /* PRQA S 0303,2814 */
    #endif
}
/**********************************************************************************************************************
 End of function R_FCL_I_read_memory_u16
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_I_read_memory_u08
 *********************************************************************************************************************/
/**
 * Function to read a 8-bit IO register or memory
 *
 * @param[in]     addr_u32       source address
 * @return        8-bit register contents
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC uint8_t R_FCL_I_read_memory_u08 (const uint32_t addr_u32)
{
    #if (defined FCL_TEST_PATCH)
        return (test_cb_read_u8 (addr_u32));
    #else
        return ( *( (volatile uint8_t *)(addr_u32) ) );                                           /* PRQA S 0303,2814 */
    #endif
}
/**********************************************************************************************************************
 End of function R_FCL_I_read_memory_u08
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_I_write_unaligned_memory_u32
 *********************************************************************************************************************/
/**
 * Function to write a 32-bit data to memory
 *
 * @param[in]     addr_u32       unaligned write destination address
 * @param[in]     val_u32        32-bit write data
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_I_write_unaligned_memory_u32 (const uint32_t addr_u32, const uint32_t val_u32)
{
    R_FCL_I_write_memory_u08 (addr_u32,       (uint8_t)val_u32);
    R_FCL_I_write_memory_u08 ((addr_u32 + 1), (uint8_t)(val_u32 >> 8));
    R_FCL_I_write_memory_u08 ((addr_u32 + 2), (uint8_t)(val_u32 >> 16));
    R_FCL_I_write_memory_u08 ((addr_u32 + 3), (uint8_t)(val_u32 >> 24));
}
/**********************************************************************************************************************
 End of function R_FCL_I_write_unaligned_memory_u32
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_I_write_memory_u32
 *********************************************************************************************************************/
/**
 * Function to write a 32-bit IO register or memory
 *
 * @param[in]     addr_u32       write destination address
 * @param[in]     val_u32        32-bit write data
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_I_write_memory_u32 (const uint32_t addr_u32, const uint32_t val_u32)
{
    #if (defined FCL_TEST_PATCH)
        test_cb_write_u32 (addr_u32, val_u32);
    #else
        ( *( (volatile uint32_t *)(addr_u32) ) ) = val_u32;                                       /* PRQA S 0303,2814 */
    #endif
}
/**********************************************************************************************************************
 End of function R_FCL_I_write_memory_u32
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_I_write_memory_u16
 *********************************************************************************************************************/
/**
 * Function to write a 16-bit IO register or memory
 *
 * @param[in]     addr_u32       write destination address
 * @param[in]     val_u16        16-bit write data
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_I_write_memory_u16 (const uint32_t addr_u32, const uint16_t val_u16)
{
    #if (defined FCL_TEST_PATCH)
        test_cb_write_u16 (addr_u32, val_u16);
    #else
        ( *( (volatile uint16_t *)(addr_u32) ) ) = val_u16;                                       /* PRQA S 0303,2814 */
    #endif
}
/**********************************************************************************************************************
 End of function R_FCL_I_write_memory_u16
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_I_write_memory_u08
 *********************************************************************************************************************/
/**
 * Function to write a 8-bit IO register or memory
 *
 * @param[in]     addr_u32       write destination address
 * @param[in]     val_u08        8-bit write data
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
void R_FCL_I_write_memory_u08 (const uint32_t addr_u32, const uint8_t val_u08)
{
    #if (defined FCL_TEST_PATCH)
        test_cb_write_u8 (addr_u32, val_u08);
    #else
        ( *( (volatile uint8_t *)(addr_u32) ) ) = val_u08;                                        /* PRQA S 0303,2814 */
    #endif
}
/**********************************************************************************************************************
 End of function R_FCL_I_write_memory_u08
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 * Function name: R_FCL_Fct_CallFromRAM
 *********************************************************************************************************************/
/**
 * This function calls the funtion to be executed from RAM
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_Fct_CallFromRAM (void)                                                                  /* PRQA S 1505 */
{
    if (R_FCL_NULL != g_fcl_data_str.fcl_executionPtr_p)
    {
        (*g_fcl_data_str.fcl_executionPtr_p)();
    }
    else
    {
        R_FCL_FCUFct_GetConfigArea ();
    }
}
/**********************************************************************************************************************
 End of function R_FCL_Fct_CallFromRAM
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_VerifyID
 *********************************************************************************************************************/
/**
 * Function to verify the given ID against the stored reference ID \n
 * The reference ID is tranferred to the check unit and the comparison result is returned to the
 * calling function. Depending on the comparison result, all further Flash operations will be
 * accepted or rejected by the hardware.
 *
 * @param[in]     ---
 * @return        R_FCL_OK              ID check passed successful
 *                R_FCL_ERR_PROTECTION  ID mismatch
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC r_fcl_status_t R_FCL_FCUFct_VerifyID ( void )
{
    uint32_t       result;
    r_fcl_status_t ret;

    ret = R_FCL_OK;

    R_FCL_I_write_memory_u32 (R_FPSYS_REGADD_AUTH_ID0_U32, (g_fcl_data_str.RTCfg_pstr->id_au32[0]));  /* PRQA S 2814 */
    R_FCL_I_write_memory_u32 (R_FPSYS_REGADD_AUTH_ID1_U32, (g_fcl_data_str.RTCfg_pstr->id_au32[1]));
    R_FCL_I_write_memory_u32 (R_FPSYS_REGADD_AUTH_ID2_U32, (g_fcl_data_str.RTCfg_pstr->id_au32[2]));
    R_FCL_I_write_memory_u32 (R_FPSYS_REGADD_AUTH_ID3_U32, (g_fcl_data_str.RTCfg_pstr->id_au32[3]));

    /* Dummy read because some devices have some cycles delay before verifying the ID */
    result = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_AUTH_ID3_U32);                                   /* PRQA S 2982 */

    /* Read verification result */
    result = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_AUTH_STATUS_U32);
    if (R_FPSYS_REGBIT_AUTH_STATUS_LOCKED == result)
    {
        ret = R_FCL_ERR_PROTECTION;
    }

    return (ret);
} /* R_FCL_FCUFct_VerifyID */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_VerifyID
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_VerifyChecksum
 *********************************************************************************************************************/
/**
 * Function to verify the FCURAM checksum \n
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_FCUFct_VerifyChecksum  (void)
{
    uint32_t       startAddr;
    uint32_t       endAddr;
    uint32_t       chkSum;
    uint32_t       chkSumCmp;

   /* Activate FCU RAM access again */
    R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FCURAME_U16, R_FPSYS_REGBIT_FCURAME_FCRME +
                              R_FPSYS_REGBIT_FCURAME_KEY);
    /* Activate BFlash */
    R_FCL_FCUFct_Switch_BFlash (0x01u);
    R_FCL_FCUFct_Clear_Cache ();
    
    endAddr = R_FCL_I_read_memory_u32 (R_FPSYS_FCURAM_AREA_CHKSUM_END);
    chkSumCmp = R_FCL_I_read_memory_u32 (R_FPSYS_FCURAM_AREA_CHKSUM);
    
    /* Deactivate BFlash */
    R_FCL_FCUFct_Switch_BFlash (0x00u);
    R_FCL_FCUFct_Clear_Cache ();
    
    /* Calculate and compare FCU RAM checksum */
    endAddr <<= 2;
    endAddr += R_FPSYS_FCURAM_AREA_BASE;
    chkSum  = 0x00000000uL;
    for (startAddr = R_FPSYS_FCURAM_AREA_BASE; startAddr < endAddr; startAddr +=2)
    {
        chkSum += (uint32_t)(R_FCL_I_read_memory_u16 (startAddr));
    }

    /* Deactivate FCU RAM access */
    R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FCURAME_U16,
                              R_FPSYS_REGBIT_FCURAME_RESET + R_FPSYS_REGBIT_FCURAME_KEY);

    if (chkSum != chkSumCmp)
    {
        g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_ERR_INTERNAL;
        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
    }
    else
    {
        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_PREP_ID_CHECK_FREQ_SET_STATE;
    }
}
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_VerifyChecksum
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_SetFrequency
 *********************************************************************************************************************/
/**
 * Function to calculate and set the FACI frequency
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_FCUFct_SetFrequency  (void)
{
    uint16_t       ffcu;

    if (g_fcl_data_str.fcl_deviceInfo_str.fVersion_u8 != 0xff)
    {
        ffcu = g_fcl_data_str.RTCfg_pstr->frequencyCpuMHz_u16;                             /* PRQA S 2814 */
       
        /* Check frequency parameter */
        if ((g_fcl_data_str.fcl_deviceInfo_str.fMin_u16 > ffcu) ||
            (g_fcl_data_str.fcl_deviceInfo_str.fMax_u16 < ffcu))
        {
            g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_ERR_PARAMETER;
        }
        else
        {
            /* FCU frequency calculation (incl. frequency round up) and setting */
            if (g_fcl_data_str.fcl_deviceInfo_str.fDivider_u8 == 0xFF)
            {
                ffcu = g_fcl_data_str.fcl_deviceInfo_str.fPClk_u16;
            }
            else
            {
                if (g_fcl_data_str.fcl_deviceInfo_str.fDivider_u8 > 0)
                {
                    ffcu = ( (ffcu + g_fcl_data_str.fcl_deviceInfo_str.fDivider_u8) - 1) /
                            g_fcl_data_str.fcl_deviceInfo_str.fDivider_u8;
                }
            }

            R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_PCKAR_U16, R_FPSYS_REGBIT_PCKAR_KEY + (uint16_t)ffcu);
            g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_OK;
        }
    }
    else
    {
        g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_OK;
    }
}
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_SetFrequency
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_SwitchMode_Start
 *********************************************************************************************************************/
/**
 * This function changes the Flash sequencer operation mode.
 *
 * @param[in]     mode_enu              R_FCL_READ_MODE, R_FCL_WRITE_MODE or R_FCL_CFG_WRITE_MODE
 * @return        R_FCL_OK              mode switched started successfully
 *                R_FCL_ERR_REJECTED    Data Flash operation on-going or the mode is already switched
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
r_fcl_status_t R_FCL_FCUFct_SwitchMode_Start (r_fcl_mode_t mode_enu)                               /* PRQA S 1503 */
{
    r_fcl_status_t ret;
    uint32_t       regFSTATR;
    uint16_t       regFENTRYR;

    ret = R_FCL_OK;

    if (R_FCL_READ_MODE == mode_enu)
    {
        regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);

        if (R_FPSYS_REGBIT_FSTATR_RESET != ( regFSTATR & (R_FPSYS_REGBIT_FSTATR_ERSERR +
                                                          R_FPSYS_REGBIT_FSTATR_PRGERR +
                                                          R_FPSYS_REGBIT_FSTATR_ILGERR)))
        {
            R_FCL_FCUFct_ClearStatus ();
        }
    } 
    else
    {
        regFENTRYR = R_FCL_I_read_memory_u16 (R_FPSYS_REGADD_FENTRYR_U16);
        if ((uint16_t)(regFENTRYR & R_FPSYS_MODE_DPE) == R_FPSYS_MODE_DPE)
        {
            /* do not touch Data Flash */
            ret = R_FCL_ERR_REJECTED;
        }
        else if ((uint16_t)(regFENTRYR & 0x00FF) == mode_enu)
        {
            /* already in requested mode */
            ret = R_FCL_ERR_REJECTED;
        }
        else
        {
            /* Data Flash or Code Flash P/E mode*/
            R_FCL_I_write_memory_u32 (R_FPSYS_REGADD_FSADR_U32, 0x00000000uL);
            R_FCL_I_write_memory_u32 (R_FPSYS_REGADD_FEADR_U32, 0x00000000uL);
        }
    }
    
    if (R_FCL_OK == ret)
    {
        R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FENTRYR_U16, (uint16_t)(mode_enu) + R_FPSYS_REGBIT_FENTRY_KEY);
        g_fcl_data_str.fcl_flashMode_enu = mode_enu;
    }
    
    return (ret);
} /* R_FCL_FCUFct_SwitchMode_Start */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_SwitchMode_Start
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_SwitchMode_Check
 *********************************************************************************************************************/
/**
 * This function checks if the Flash sequencer operation mode switch is performed correctly
 *
 * @param[in]     ---
 * @return        R_FCL_OK              mode switched successful
 *                R_FCL_BUSY            switching not performed yet
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC r_fcl_status_t R_FCL_FCUFct_SwitchMode_Check ( void )
{
    r_fcl_status_t ret;
    uint16_t       regFENTRYR;

    ret = R_FCL_BUSY;

    regFENTRYR = R_FCL_I_read_memory_u16 (R_FPSYS_REGADD_FENTRYR_U16);
    regFENTRYR &= (R_FPSYS_MODE_DPE + R_FPSYS_MODE_CPE);
    if ((uint16_t)g_fcl_data_str.fcl_flashMode_enu == regFENTRYR)
    {
        ret = R_FCL_OK;
    }

    return (ret);
} /* R_FCL_FCUFct_SwitchMode_Check */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_SwitchMode_Check
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_ForcedStop
 *********************************************************************************************************************/
/**
 * This function resets the FCU and FACI registers
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_FCUFct_ForcedStop (void)                                                   /* PRQA S 1503 */
{
    uint32_t regFSTATR;

    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_FORCED_STOP);
    do
    {
        regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);
    }
    while (R_FPSYS_REGBIT_FSTATR_FRDY != (R_FPSYS_REGBIT_FSTATR_FRDY & regFSTATR));
} /* R_FCL_FCUFct_ForcedStop */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_ForcedStop
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_ChkSuspended
 *********************************************************************************************************************/
/**
 * This function checks if an erase or write operation is suspended by checking the corresponding bits in FSTATR
 *
 * @param[in,out] ---
 * @return        R_FCL_TRUE:   either one of the following bits is set PRGSPD or ERSSPD
 *                R_FCL_FALSE:  no operation is suspended
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC r_fcl_flag_t R_FCL_FCUFct_ChkSuspended (void)
{
    uint32_t        regFSTATR;
    r_fcl_flag_t    ret;

    ret = R_FCL_FALSE;

    regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);
    if ((R_FPSYS_REGBIT_FSTATR_PRGSPD == (regFSTATR & R_FPSYS_REGBIT_FSTATR_PRGSPD)) ||
        (R_FPSYS_REGBIT_FSTATR_ERSSPD == (regFSTATR & R_FPSYS_REGBIT_FSTATR_ERSSPD)))
    {
        ret = R_FCL_TRUE;
    }
    
    return ret;
} /* R_FCL_FCUFct_ChkSuspended */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_ChkSuspended
 *********************************************************************************************************************/


/*********************************************************************************************************************
* Function name:  R_FCL_FCUFct_ClearStatus
*********************************************************************************************************************/
/**
 * Clear (error) status of the sequencer by the clear status FCU command
 *
 * @param      ---
 * @return     ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_FCUFct_ClearStatus (void)
{
    uint32_t res32;
    uint8_t  res8;

    res32 = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);

    if (R_FPSYS_REGBIT_FSTATR_ILGERR == (R_FPSYS_REGBIT_FSTATR_ILGERR & res32))
    {
        res8 = R_FCL_I_read_memory_u08 (R_FPSYS_REGADD_FASTAT_U8);
        /* Only CLDLK bit may be set, others have to be cleared */
        if (R_FPSYS_REGBIT_FASTAT_CMDLK != res8)
        {
            R_FCL_I_write_memory_u08 (R_FPSYS_REGADD_FASTAT_U8, R_FPSYS_REGBIT_FASTAT_CMDLK);
        }
    }

    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_CLEARSTAT);
} /* R_FCL_FCUFct_ClearStatus */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_ClearStatus
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_RestoreLastState
 *********************************************************************************************************************/
/**
 * This function restores the last library state after execution of a command. Depending on the previous executed
 * command the library state is prepared (no command suspended) or suspended (the previuos command need to be finished.
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_FCUFct_RestoreLastState (void)
{
    if (g_fcl_data_str.fcl_suspendInfo_str.flagSuspended_enu == R_FCL_FALSE)
    {
        g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_PREPARED;
    }
    else
    {
        g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_SUSPENDED;
    }
} /* R_FCL_FCUFct_RestoreLastState */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_RestoreLastState
 *********************************************************************************************************************/


#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
/**********************************************************************************************************************
* Function name: R_FCL_FCUFct_HandleResume
*********************************************************************************************************************/
/**
 * This function performs all operations necessary for resuming a suspended erase or write command
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_HandleResume (void)
{
    uint32_t regFSTATR;

    if (R_FCL_TRUE == g_fcl_data_str.fcl_waitAckModeSwitch)
    {
        if (R_FCL_OK == R_FCL_FCUFct_SwitchMode_Check())
        {
            R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16,
                                      g_fcl_data_str.fcl_suspendInfo_str.matSel_u16 + R_FPSYS_REGBIT_FMATSELC_KEY);

            /* Restore variables for resume */
            g_fcl_data_str.fcl_reqInt_pstr           = g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr;
            g_fcl_data_str.fcl_opVar_str.opAddr_u32  = g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opAddr_u32;
            g_fcl_data_str.fcl_opVar_str.opCnt_u16   = g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opCnt_u16;
            g_fcl_data_str.fcl_opVar_str.opStatus_enu =
                                                    g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu;
            g_fcl_data_str.fcl_opVar_str.opResult_enu =
                                                    g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opResult_enu;
        
            g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr            = R_FCL_REQUEST_POINTER_UNDEFINED;
            g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = R_FCL_OP_IDLE_STATE;
            g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu              = R_FCL_FALSE;
            g_fcl_data_str.fcl_suspendInfo_str.flagSuspended_enu              = R_FCL_FALSE;
            
            g_fcl_data_str.fcl_deviceInfo_str.matSel_u16 = g_fcl_data_str.fcl_suspendInfo_str.matSel_u16;

            regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);
            if ((R_FPSYS_REGBIT_FSTATR_PRGSPD == (regFSTATR & R_FPSYS_REGBIT_FSTATR_PRGSPD)) ||
                (R_FPSYS_REGBIT_FSTATR_ERSSPD == (regFSTATR & R_FPSYS_REGBIT_FSTATR_ERSSPD)))
            {
                R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_RESUME);
            }
            g_fcl_data_str.fcl_intStatus_enu           = R_FCL_ISTAT_BUSY;
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_BUSY;
            g_fcl_data_str.fcl_waitAckModeSwitch       = R_FCL_FALSE;
        }
    }
    else
    {
        if ( g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu != R_FCL_OP_FINISHED_STATE)
        {
            R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16,
                                      g_fcl_data_str.fcl_suspendInfo_str.matSel_u16 + R_FPSYS_REGBIT_FMATSELC_KEY);
        }

        /* Restore variables for resume */
        g_fcl_data_str.fcl_reqInt_pstr             = g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr;
        g_fcl_data_str.fcl_opVar_str.opAddr_u32    = g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opAddr_u32;
        g_fcl_data_str.fcl_opVar_str.opCnt_u16     = g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opCnt_u16;
        g_fcl_data_str.fcl_opVar_str.opStatus_enu  = g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu;
        g_fcl_data_str.fcl_opVar_str.opResult_enu  = g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opResult_enu;
        g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_BUSY;
        
        g_fcl_data_str.fcl_deviceInfo_str.matSel_u16 = g_fcl_data_str.fcl_suspendInfo_str.matSel_u16;
        
        g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr            = R_FCL_REQUEST_POINTER_UNDEFINED;
        g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = R_FCL_OP_IDLE_STATE;
        g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu              = R_FCL_FALSE;
        g_fcl_data_str.fcl_suspendInfo_str.flagSuspended_enu              = R_FCL_FALSE;
        g_fcl_data_str.fcl_intStatus_enu                                  = R_FCL_ISTAT_BUSY;
    }
}
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_HandleResume
 *********************************************************************************************************************/
#endif


/**********************************************************************************************************************
* Function name: R_FCL_FCUFct_CheckMode
*********************************************************************************************************************/
/**
 * This function checks wether the FLMD0 pin is set according to its needs.
 *
 * @param[in,out] ---
 * @return        R_FCL_TRUE            FLMD0 pin set correct
 *                R_FCL_FALSE           FLMD0 pin setting invalid
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
r_fcl_flag_t R_FCL_FCUFct_CheckMode (void)
{
    r_fcl_flag_t ret;
    uint8_t      regVal;

    ret = R_FCL_FALSE;

    regVal = R_FCL_I_read_memory_u08 (R_FPSYS_REGADD_FPMON_U8);
    if (R_FPSYS_REGBIT_SFWE == (regVal & R_FPSYS_REGBIT_SFWE))
    {
        ret = R_FCL_TRUE;
    }

    return (ret);
} /* R_FCL_FCUFct_CheckMode */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_CheckMode
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetResult
 *********************************************************************************************************************/
/**
 * This function reads the current status of the FACI.
 * reset vector.
 *
 * @param[in]     cmdSpdExec_enu        R_FCL_TRUE:     last executed FCU command is Suspend
 *                                      R_FCL_FALSE:    last executed FCU command is different than Suspend
 *
 * @return        R_FCL_BUSY            a FACI operation is ongoing
 *                R_FCL_OK              last FACI command finished successfully
 *                R_FCL_ERR_PROTECTION  command execution impossible due to set OTP flag
 *                R_FCL_ERR_ERASE       an erase error occured
 *                R_FCL_ERR_WRITE       a write error occured
 *                R_FCL_ERR_INTERNAL    any other error occured
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC r_fcl_status_t R_FCL_FCUFct_GetResult (r_fcl_flag_t cmdSpdExec_enu)
{
    r_fcl_status_t ret;
    uint32_t       regFSTATR;
    uint16_t       regFPESTAT;

    regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);

    if (R_FPSYS_REGBIT_FSTATR_FRDY == (regFSTATR & R_FPSYS_REGBIT_FSTATR_FRDY))
    {
        #if (defined FCL_TEST_PATCH)
            if (0 != TestPatch.status_patch_active)
            {
                regFSTATR = TestPatch.status_patch_value;
                if (UINT32_MAX != TestPatch.status_patch_active)
                {
                    TestPatch.status_patch_active--;
                }
            }
        #endif
        if ((R_FPSYS_REGBIT_FSTATR_FCURAMERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_FCURAMERR)) ||
            (R_FPSYS_REGBIT_FSTATR_TBLDERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_TBLDERR)) ||
            (R_FPSYS_REGBIT_FSTATR_CONFDERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_CONFDERR)) ||
            (R_FPSYS_REGBIT_FSTATR_FCUERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_FCUERR)) ||
            (R_FPSYS_REGBIT_FSTATR_OTPDERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_OTPDERR)))
        {
            ret = R_FCL_ERR_INTERNAL;
        }
        else if (R_FPSYS_REGBIT_FSTATR_ILGERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_ILGERR))
        {
            if (R_FCL_FALSE == cmdSpdExec_enu)
            {
                ret = R_FCL_ERR_INTERNAL;
            }
            else
            {
                ret = R_FCL_ERR_SUSPEND_FAILED;
            }
        }
        else if (R_FPSYS_REGBIT_FSTATR_FHVEERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_FHVEERR))
        {
            ret = R_FCL_ERR_PROTECTION;
        }
        else if (R_FPSYS_REGBIT_FSTATR_ERSERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_ERSERR)) 
        {
            regFPESTAT = R_FCL_I_read_memory_u16 (R_FPSYS_REGADD_FPESTAT_U16);
            if (R_FPSYS_REGBIT_FPESTAT_ERASELOCK != (regFPESTAT & R_FPSYS_REGBIT_FPESTAT_ERASELOCK))
            {
                ret = R_FCL_ERR_ERASE;
            }
            else
            {
                ret = R_FCL_ERR_PROTECTION;
            }
            
        }
        else if (R_FPSYS_REGBIT_FSTATR_PRGERR == (regFSTATR & R_FPSYS_REGBIT_FSTATR_PRGERR)) 
        {
            regFPESTAT = R_FCL_I_read_memory_u16 (R_FPSYS_REGADD_FPESTAT_U16);
            if (R_FPSYS_REGBIT_FPESTAT_WRITELOCK != (regFPESTAT & R_FPSYS_REGBIT_FPESTAT_WRITELOCK))
            {
                ret = R_FCL_ERR_WRITE;
            }
            else
            {
                ret = R_FCL_ERR_PROTECTION;
            }
        }
        else
        {
            ret = R_FCL_OK;
        }
    }
    else
    {
        ret = R_FCL_BUSY;
    }


    return (ret);
} /* R_FCL_FCUFct_GetResult */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetResult
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetConfigArea
 *********************************************************************************************************************/
/**
 * This function reads the Extra Area 2
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
void R_FCL_FCUFct_GetConfigArea (void)                                                             /* PRQA S 1505 */
{
    uint16_t data;
    uint16_t i;
    uint32_t bufAddr;
    uint32_t readAddr;
    uint32_t cnt;

    R_FCL_FCUFct_Switch_BFlash (0x01u);
    R_FCL_FCUFct_Clear_Cache ();

    if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_OPB)                          /* PRQA S 2814 */
    {
        readAddr = R_FPSYS_CONFIG_AREA_OPTIONBYTES;
        cnt = R_PRG_CFG_TRANSFERSIZE_OPB;
    }
    else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_ID)
    {
        readAddr = R_FPSYS_CONFIG_AREA_OCD;
        cnt = R_PRG_CFG_TRANSFERSIZE_OCD;
    }
    else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_OTP)
    {
        readAddr = g_fcl_data_str.fcl_opVar_str.opAddr_u32;
        cnt = R_PRG_CFG_TRANSFERSIZE_OTP;
    }
    else
    {
        /* Get reset vector */
        readAddr = R_FPSYS_CONFIG_AREA_RESET;
        cnt = R_PRG_CFG_TRANSFERSIZE_RESET;
    }

    bufAddr = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;

    for (i = 0; i < (2 * cnt); i++)
    {
        data = R_FCL_I_read_memory_u08 (readAddr);
        R_FCL_I_write_memory_u08 (bufAddr, (uint8_t)data);
        bufAddr++;
        readAddr++;
    }

    R_FCL_FCUFct_Switch_BFlash (0x00u);
    R_FCL_FCUFct_Clear_Cache ();

    g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;
} /* R_FCL_FCUFct_GetConfigArea */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetConfigArea
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_SetConfigArea
 *********************************************************************************************************************/
/**
 * This function prepares internal variables to write the Extra Area 2
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_SetConfigArea (void)
{
    uint32_t flag;
    uint8_t  i;

    if (R_FCL_FCUFct_CheckMode () == R_FCL_FALSE)
    {
        /* FLMD0 pin not set */
        g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_FLMD0;                              /* PRQA S 2814 */
    }
    else
    {
        if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_OPB)
        {
            g_fcl_data_str.fcl_opVar_str.buffAddr_u32 = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;
            g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_AREA_OPTIONBYTES;
            g_fcl_data_str.fcl_opVar_str.opCnt_u16 = R_PRG_CFG_TRANSFERSIZE_OPB;
        }
        else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_OTP)
        {
            g_fcl_data_str.fcl_opVar_str.opCnt_u16 = R_PRG_CFG_TRANSFERSIZE_OTP;
        }
        else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_ID)
        {
            g_fcl_data_str.fcl_opVar_str.buffAddr_u32 = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;
            g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_AREA_OCD;
            g_fcl_data_str.fcl_opVar_str.opCnt_u16 = R_PRG_CFG_TRANSFERSIZE_OCD;
        }
        else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_RESET_VECTOR)
        {
            g_fcl_data_str.fcl_opVar_str.buffAddr_u32 = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;
            g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_AREA_RESET;
            g_fcl_data_str.fcl_opVar_str.opCnt_u16 = R_PRG_CFG_TRANSFERSIZE_RESET;
        }
        else
        {
            flag = g_fcl_data_str.fcl_deviceInfo_str.secureData_u32;
            g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_AREA_SEC_FLAGS;
            g_fcl_data_str.fcl_opVar_str.opCnt_u16 = R_PRG_CFG_TRANSFERSIZE;

            if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_READ_PROTECT_FLAG)
            {
                flag = flag & ~(1u << R_RDPR_FLAG_SHIFT);
            }
            else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_WRITE_PROTECT_FLAG)
            {
                flag = flag & ~(1u << R_WRPR_FLAG_SHIFT);
            }
            else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_ERASE_PROTECT_FLAG)
            {
                flag = flag & ~(1u << R_SEPR_FLAG_SHIFT);
            }
            else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_SERIAL_PROG_DISABLED)
            {
                flag = flag & ~(1u << R_SPD_FLAG_SHIFT);
            }
            else
            {
                flag = flag & ~(1u << R_RDPR_FLAG_SHIFT);
                flag = flag & ~(1u << R_SEPR_FLAG_SHIFT);
                flag = flag & ~(1u << R_WRPR_FLAG_SHIFT);
                flag = flag & ~(1u << R_SPIE_FLAG_SHIFT);
            }
            flag = flag & 0xfffffff0u;

            /* Prepare write data  */
            g_fcl_data_str.fcl_intBuf[0] = (uint8_t)flag;
            g_fcl_data_str.fcl_intBuf[1] = (uint8_t)(flag >> 8u);
            g_fcl_data_str.fcl_intBuf[2] = (uint8_t)(flag >> 16u);
            g_fcl_data_str.fcl_intBuf[3] = (uint8_t)(flag >> 24u);
            for (i = 4; i < (2*R_PRG_CFG_TRANSFERSIZE); i++)
            {
                g_fcl_data_str.fcl_intBuf[i] = (uint8_t)0xffu;
            }
            g_fcl_data_str.fcl_opVar_str.buffAddr_u32 = (uint32_t)(&g_fcl_data_str.fcl_intBuf[0]); /* PRQA S 0306 */
        }
        g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_BUSY;
        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
    }
} /* R_FCL_FCUFct_SetConfigArea */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_SetConfigArea
 *********************************************************************************************************************/


#ifdef R_FCL_SUPPORT_SECURITYFLAGS
/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetSecurityFlag
 *********************************************************************************************************************/
/**
 * This function reads the security settings from a internal stored variable
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_GetSecurityFlag (void)                                                           /* PRQA S 1503 */
{
    uint32_t data;

    if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_READ_PROTECT_FLAG)            /* PRQA S 2814 */
    {
        data = (((~g_fcl_data_str.fcl_deviceInfo_str.secureData_u32) & R_RDPR_FLAG_MASK) >> R_RDPR_FLAG_SHIFT);
    }
    else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_WRITE_PROTECT_FLAG)
    {
        data = (((~g_fcl_data_str.fcl_deviceInfo_str.secureData_u32) & R_WRPR_FLAG_MASK) >> R_WRPR_FLAG_SHIFT);
    }
    else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_ERASE_PROTECT_FLAG)
    {
        data = (((~g_fcl_data_str.fcl_deviceInfo_str.secureData_u32) & R_SEPR_FLAG_MASK) >> R_SEPR_FLAG_SHIFT);
    }
    else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_SERIAL_PROG_DISABLED)
    {
        data = (((~g_fcl_data_str.fcl_deviceInfo_str.secureData_u32) & R_SPD_FLAG_MASK) >> R_SPD_FLAG_SHIFT);
    }
    else
    {
        data = (((~g_fcl_data_str.fcl_deviceInfo_str.secureData_u32) & R_SPIE_FLAG_MASK) >> R_SPIE_FLAG_SHIFT);
    }

    R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, data);

    g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;
} /* R_FCL_FCUFct_GetSecurityFlag */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetSecurityFlag
 *********************************************************************************************************************/
#endif /* ifdef R_FCL_SUPPORT_SECURITYFLAGS */


#ifdef R_FCL_SUPPORT_BLOCKCNT
/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetBlockCnt
 *********************************************************************************************************************/
/**
 * This function reads the amount of available blocks from a internal stored variable
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_GetBlockCnt (void)                                                               /* PRQA S 1503 */
{
    R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, 
                                        g_fcl_data_str.fcl_deviceInfo_str.blkCnt_u16);

    g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;
}
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetBlockCnt
 *********************************************************************************************************************/
#endif /* ifdef R_FCL_SUPPORT_BLOCKCNT */


#ifdef R_FCL_SUPPORT_DEVICENAME
/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetDeviceName
 *********************************************************************************************************************/
/**
 * This function reads the device name from a internal stored variable
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_GetDeviceName (void)                                                             /* PRQA S 1503 */
{
    uint32_t bufAddr;
    uint8_t  i;

    bufAddr = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;                                       /* PRQA S 2814 */

    for (i = 0; i < 16; i++)
    {
        R_FCL_I_write_memory_u08 (bufAddr, g_fcl_data_str.fcl_deviceInfo_str.deviceName[i]);
        bufAddr += 1;
    }
    g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;
}
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetDeviceName
 *********************************************************************************************************************/
#endif /* ifdef R_FCL_SUPPORT_DEVICENAME */


#ifdef R_FCL_SUPPORT_BLOCKENDADDR
/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetBlockEndAddr
 *********************************************************************************************************************/
/**
 * This function calculates the block end address depending on the given block
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_GetBlockEndAddr (void)                                                           /* PRQA S 1503 */
{
    uint32_t blk;
    uint32_t data;

      /* PRQA S 2814 1 */
    if (R_FCL_USERBOOT_AREA_SELECT == (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FCL_USERBOOT_AREA_SELECT))
    {
        /* extended user area selected */
        if ( ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FLI_USERBOOT_AREA_MASK) <
               g_fcl_data_str.fcl_deviceInfo_str.usrBlkCnt32kB_u32)
           && (g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu != R_FCL_FALSE) )
        {
            blk = ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FLI_USERBOOT_AREA_MASK) + 1);
            data = (blk * R_FLI_BLOCK_SIZE) - 1;
            data = data + R_FLI_USERBOOT_AREA_START;
            R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, data);
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;
        }
        else
        {
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        /* user area */
        if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < g_fcl_data_str.fcl_deviceInfo_str.blkCnt_u16)
        {
            if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8)
            {
                /* Code Flash 1 (8kb blocks) */
                blk = (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 + 1);
                data = (blk * R_FLI_SMALL_BLOCK_SIZE) - 1;
                R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, data);
            }
            else
            {
                /* Code Flash 1 (32kb blocks) */
                if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 <
                   (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 +
                    g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32))
                {
                    blk  = (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 + 1) -
                             g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8;
                    data = ((g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 * R_FLI_SMALL_BLOCK_SIZE) +
                            (blk * R_FLI_BLOCK_SIZE)) - 1;
                    R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, data);
                }
                else
                {
                    /* Code Flash 2 (32kb blocks) */
                    blk  = (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 + 1) -
                            (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 +
                             g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32);
                    data = (R_FLI_CFLASH2_START + (blk * R_FLI_BLOCK_SIZE)) - 1;
                    R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, data);
                }
            }
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;
        }
        else
        {
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
        }
    }
} /* R_FCL_FCUFct_GetBlockEndAddr */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetBlockEndAddr
 *********************************************************************************************************************/
#endif /* ifdef R_FCL_SUPPORT_BLOCKENDADDR */


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_PrepareEnvironment
 *********************************************************************************************************************/
/**
 * This function prepares the environment for further Flash operations:
 *          - copy the firmware to the FCURam
 *          - read out device internal settings (block, count, security settings, ...)
 *          - set device frequency
 *          - resets the FACI
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
void R_FCL_FCUFct_PrepareEnvironment (void)
{
    r_fcl_status_t ret;
    uint16_t       regFENTRYR;

    if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_OP_PREPARE_STATE)
    {
        regFENTRYR = R_FCL_I_read_memory_u16 (R_FPSYS_REGADD_FENTRYR_U16);
        if (R_FPSYS_REGBIT_FENTRY_OFF == regFENTRYR)
        {
            g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;

            /* Activate FCU RAM access */
            R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FCURAME_U16, R_FPSYS_REGBIT_FCURAME_FCRME +
                                      R_FPSYS_REGBIT_FCURAME_FRAMTRAN + R_FPSYS_REGBIT_FCURAME_KEY);

            /* Activate BFlash */
            R_FCL_FCUFct_Switch_BFlash (0x01u);
            R_FCL_FCUFct_Clear_Cache ();

            /* Get device information */
            R_FCL_FCUFct_GetDeviceInfo ();

            /* Copy firmware to RAM */
            g_fcl_data_str.fcl_fcuramAddr_u32 = R_FPSYS_FCURAM_AREA_BASE;
            g_fcl_data_str.fcl_firmAddr_u32 = R_FPSYS_FCURAM_AREA_RAM_ADD;
            (void)R_FCL_Fct_Copy_Code (g_fcl_data_str.fcl_firmAddr_u32, g_fcl_data_str.fcl_fcuramAddr_u32, 
                                       R_FPSYS_FCURAM_AREA_COPY_SIZE);
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_PREP_COPY_FW_STATE;
        }
        else
        {
            g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_ERR_REJECTED;
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_PREP_COPY_FW_STATE)
    {
        g_fcl_data_str.fcl_fcuramAddr_u32 = g_fcl_data_str.fcl_fcuramAddr_u32 + R_FPSYS_FCURAM_AREA_COPY_SIZE;
        g_fcl_data_str.fcl_firmAddr_u32 = g_fcl_data_str.fcl_firmAddr_u32 + R_FPSYS_FCURAM_AREA_COPY_SIZE;

        if (g_fcl_data_str.fcl_fcuramAddr_u32 >= (R_FPSYS_FCURAM_AREA_BASE + R_FPSYS_FCURAM_AREA_SIZE))
        {
            /* Deactivate BFlash */
            R_FCL_FCUFct_Switch_BFlash (0x00u);
            R_FCL_FCUFct_Clear_Cache ();
            
            /* Deactivate FCU RAM access */
            R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FCURAME_U16,
                                      R_FPSYS_REGBIT_FCURAME_RESET + R_FPSYS_REGBIT_FCURAME_KEY);

            ret = R_FCL_FCUFct_SwitchMode_Start (R_FCL_CFG_WRITE_MODE);
            if (R_FCL_OK == ret)
            {
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_PREP_RESET_FCU_STATE;
            }
            else
            {
                g_fcl_data_str.fcl_opVar_str.opResult_enu = ret;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
            }
        }
        else
        {
            (void)R_FCL_Fct_Copy_Code (g_fcl_data_str.fcl_firmAddr_u32, g_fcl_data_str.fcl_fcuramAddr_u32, 
                                       R_FPSYS_FCURAM_AREA_COPY_SIZE);

        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_PREP_RESET_FCU_STATE)
    {
        ret = R_FCL_FCUFct_SwitchMode_Check ();
        if (R_FCL_OK == ret)
        {
            R_FCL_FCUFct_ForcedStop ();
            R_FCL_FCUFct_ClearStatus ();

            (void)R_FCL_FCUFct_SwitchMode_Start (R_FCL_READ_MODE);
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_PREP_FCURAM_CHKSUM_STATE;
        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_PREP_FCURAM_CHKSUM_STATE)
    {
        ret = R_FCL_FCUFct_SwitchMode_Check ();
        if (R_FCL_OK == ret)
        {
            R_FCL_FCUFct_VerifyChecksum ();
        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_PREP_ID_CHECK_FREQ_SET_STATE)
    {
        /* ----------------------- ID verification --------------------------------------------------- */
        ret = R_FCL_FCUFct_VerifyID ();
        if (R_FCL_OK == ret)
        {
            R_FCL_FCUFct_SetFrequency ();
        }
        else
        {
            g_fcl_data_str.fcl_opVar_str.opResult_enu = ret;
        }
        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
    }
    else
    {
        /* operation finished state */
        g_fcl_data_str.fcl_reqInt_pstr->status_enu = g_fcl_data_str.fcl_opVar_str.opResult_enu;    /* PRQA S 2814 */
        if (g_fcl_data_str.fcl_reqInt_pstr->status_enu == R_FCL_OK)
        {
            g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_PREPARED;
        }
        else
        {
            g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_INIT;
        }
        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_IDLE_STATE;

        g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = R_FCL_OP_IDLE_STATE;
        g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr = R_FCL_REQUEST_POINTER_UNDEFINED;
        g_fcl_data_str.fcl_suspendInfo_str.flagSuspended_enu = R_FCL_FALSE;
        g_fcl_data_str.fcl_cancelReq = R_FCL_FALSE;
    }

} /* R_FCL_FCUFct_PrepareEnvironment */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_PrepareEnvironment
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetDeviceInfo
 *********************************************************************************************************************/
/**
 * This function reads out device settings from the Extra Area:
 *          - firmware version
 *          - frequency settings
 *          - amount of Code Flash blocks for all Flash macros
 *          - device name
 *          - security settings
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
R_FCL_STATIC void R_FCL_FCUFct_GetDeviceInfo (void)                                                /* PRQA S 1503 */
{
    uint32_t data;
    uint32_t dataAddr;

    /* Allow Extended User Area Programming and Erase */
    g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu = R_FCL_TRUE;
    
    /* Get firmware version */
    g_fcl_data_str.fcl_deviceInfo_str.fVersion_u8 = R_FCL_I_read_memory_u08 (R_FPSYS_EXTRA3_AREA_FPVER_U08);

    /* Get block information */
    dataAddr = R_FCL_I_read_memory_u32 (R_FPSYS_EXTRA3_AREA_SCDS_U32);

    /* Code Flash 1 */
    data = R_FCL_I_read_memory_u32 (dataAddr + R_FPSYS_EXTRA3_AREA_PRDSEL1);
    g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32 = ((data & 0xffff0000u) >> 16) / 32;
    g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 = (uint8_t)((data & 0x0000ffffu) / 8);

    /* Code Flash 2 */
    data = R_FCL_I_read_memory_u32 (dataAddr + R_FPSYS_EXTRA3_AREA_PRDSEL2);
    g_fcl_data_str.fcl_deviceInfo_str.cf2BlkCnt32kB_u32 = (data & 0x0000ffffu) / 32;

    /* user boot area */
    data = ((R_FLI_USERBOOT_AREA_END + 1) - R_FLI_USERBOOT_AREA_START);
    g_fcl_data_str.fcl_deviceInfo_str.usrBlkCnt32kB_u32 = data / R_FLI_BLOCK_SIZE;


    /* Check emulation mode */
    data = R_FCL_I_read_memory_u32 (dataAddr + R_FPSYS_EXTRA3_AREA_PRDSEL4);
    if ((data & 0x000000ffu) == 0x55u)                                                             /* CCOV 0001 */
    {
        g_fcl_data_str.fcl_deviceInfo_str.emulationMode_enu = R_FCL_TRUE;
    }
    else
    {
        g_fcl_data_str.fcl_deviceInfo_str.emulationMode_enu = R_FCL_FALSE;
    }

    /* Get device name */
    data = R_FCL_I_read_memory_u32 (dataAddr + R_FPSYS_EXTRA3_AREA_PRDNAME1);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[0]  = (uint8_t)(data & 0x000000ffu);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[1]  = (uint8_t)((data & 0x0000ff00u) >> 8);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[2]  = (uint8_t)((data & 0x00ff0000u) >> 16);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[3]  = (uint8_t)((data & 0xff000000u) >> 24);
    data = R_FCL_I_read_memory_u32 (dataAddr + R_FPSYS_EXTRA3_AREA_PRDNAME2);
    /* get clock settings */
    if (g_fcl_data_str.fcl_deviceInfo_str.fVersion_u8 != 0xff)
    {
        /* Read out PClk related data */
        if (g_fcl_data_str.fcl_deviceInfo_str.fVersion_u8 < R_FPSYS_FPVER_04)
        {
            g_fcl_data_str.fcl_deviceInfo_str.fDivider_u8 =
                R_FCL_I_read_memory_u08 (R_FPSYS_EXTRA3_AREA_PCLKDIV1_FPVER03_U32);
            g_fcl_data_str.fcl_deviceInfo_str.fPClk_u16 =
                (uint16_t)(R_FCL_I_read_memory_u32 (R_FPSYS_EXTRA3_AREA_PCLK_FPVER03_U32) / 1000000uL);
            
            /* Only for E1x devices, user boot area access is not allowed */
            if (  ((data & 0x00FFFFFFu) == R_FPSYS_PRDNAME_E1x_012x) 
               || ((data & 0x00FFFFFFu) == R_FPSYS_PRDNAME_E1x_01Zx) )                             /* CCOV 0002 */
            {
                g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu = R_FCL_FALSE;
            }
        }
        else
        {
            /* Firmware version 4 or higher */
            g_fcl_data_str.fcl_deviceInfo_str.fDivider_u8 =
                R_FCL_I_read_memory_u08 (R_FPSYS_EXTRA3_AREA_PCLKDIV1_FPVER04_U32);
            g_fcl_data_str.fcl_deviceInfo_str.fPClk_u16 =
                (uint16_t)(R_FCL_I_read_memory_u32 (R_FPSYS_EXTRA3_AREA_PCLK_FPVER04_U32) / 1000000uL);

            /* Only for E1x devices, user boot area access is not allowed */
            if (0u == R_FCL_I_read_memory_u08 (R_FPSYS_EXTRA3_AREA_UB_FPVER04_U08))                /* CCOV 0003 */
            {
                g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu = R_FCL_FALSE;
            }
        }
        
        /* Read out min and max CPU frequency in MHz */
        g_fcl_data_str.fcl_deviceInfo_str.fMax_u16 =
            (uint16_t)(R_FCL_I_read_memory_u32 (R_FPSYS_EXTRA3_AREA_MAXHCLK_U32) / 1000000uL);
        g_fcl_data_str.fcl_deviceInfo_str.fMin_u16 =
            (uint16_t)(R_FCL_I_read_memory_u32 (R_FPSYS_EXTRA3_AREA_MINHCLK_U32) / 1000000uL);
    }
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[4]  = (uint8_t)(data & 0x000000ffu);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[5]  = (uint8_t)((data & 0x0000ff00u) >> 8);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[6]  = (uint8_t)((data & 0x00ff0000u) >> 16);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[7]  = (uint8_t)((data & 0xff000000u) >> 24);
    data = R_FCL_I_read_memory_u32 (dataAddr + R_FPSYS_EXTRA3_AREA_PRDNAME3);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[8]  = (uint8_t)(data & 0x000000ffu);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[9]  = (uint8_t)((data & 0x0000ff00u) >> 8);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[10] = (uint8_t)((data & 0x00ff0000u) >> 16);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[11] = (uint8_t)((data & 0xff000000u) >> 24);
    data = R_FCL_I_read_memory_u32 (dataAddr + R_FPSYS_EXTRA3_AREA_PRDNAME4);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[12] = (uint8_t)(data & 0x000000ffu);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[13] = (uint8_t)((data & 0x0000ff00u) >> 8);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[14] = (uint8_t)((data & 0x00ff0000u) >> 16);
    g_fcl_data_str.fcl_deviceInfo_str.deviceName[15] = (uint8_t)((data & 0xff000000u) >> 24);

    /* Get security data */
    g_fcl_data_str.fcl_deviceInfo_str.secureData_u32 = R_FCL_I_read_memory_u32 (R_FPSYS_CONFIG_AREA_SEC_FLAGS);

    /* Calculate block count */
    g_fcl_data_str.fcl_deviceInfo_str.blkCnt_u16 = (uint16_t)(g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 +
                                                              g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32 +
                                                              g_fcl_data_str.fcl_deviceInfo_str.cf2BlkCnt32kB_u32);
} /* R_FCL_FCUFct_GetDeviceInfo */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetDeviceInfo
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_Erase
 *********************************************************************************************************************/
/**
 * This function prepares internal variables to erase a range of Flash blocks
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_Erase (void)
{
    uint32_t blockCnt;

    if (R_FCL_FCUFct_CheckMode () == R_FCL_FALSE)
    {
        /* FLMD0 pin not set */
        g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_FLMD0;                              /* PRQA S 2814 */
    }
    else
    {
        if (R_FCL_USERBOOT_AREA_SELECT == (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FCL_USERBOOT_AREA_SELECT))
        {
            blockCnt = (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FLI_USERBOOT_AREA_MASK);

            /* extended user area selected */
            if (  ((blockCnt + g_fcl_data_str.fcl_reqInt_pstr->cnt_u16) <= 
                    g_fcl_data_str.fcl_deviceInfo_str.usrBlkCnt32kB_u32) 
               && (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 > 0)
               && (g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu != R_FCL_FALSE))
            {
                /* setup internal variables for multi operation */
                g_fcl_data_str.fcl_opVar_str.opCnt_u16 = g_fcl_data_str.fcl_reqInt_pstr->cnt_u16;
                g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FLI_USERBOOT_AREA_START +
                                                        ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 &
                                                          R_FLI_USERBOOT_AREA_MASK) * R_FLI_BLOCK_SIZE);
                g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_BUSY;
                g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
            }
            else
            {
                /* parameter error */
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
            }
        }
        else
        {
            /* Use local variable to remove IAR warning */
            blockCnt = g_fcl_data_str.fcl_reqInt_pstr->idx_u32;

            /* user area */
            if (  ((blockCnt + g_fcl_data_str.fcl_reqInt_pstr->cnt_u16) <= g_fcl_data_str.fcl_deviceInfo_str.blkCnt_u16)
               && (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 > 0))
            {
                /* setup internal variables for multi operation */
                g_fcl_data_str.fcl_opVar_str.opCnt_u16 = g_fcl_data_str.fcl_reqInt_pstr->cnt_u16;
                if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32  < g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8)
                {
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 =
                        g_fcl_data_str.fcl_reqInt_pstr->idx_u32 * R_FLI_SMALL_BLOCK_SIZE;
                }
                else if ( g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < 
                          (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8  + 
                           g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32)
                        )
                {
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 =
                       (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 * R_FLI_SMALL_BLOCK_SIZE) +
                       ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 - g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8) 
                        * R_FLI_BLOCK_SIZE);
                }
                else
                {
                    /* Bank B available */
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FLI_CFLASH2_START +
                        ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 - g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 
                          - g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32) * R_FLI_BLOCK_SIZE);
                }
                g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_BUSY;
                g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
            }
            else
            {
                /* parameter error */
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
            }
        }
    }
} /* R_FCL_FCUFct_Erase */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_Erase
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_Write
 *********************************************************************************************************************/
/**
 * This function prepares internal variables to write a range of Flash addresses
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_Write (void)
{
    uint32_t startAddr;

    if (R_FCL_FCUFct_CheckMode () == R_FCL_FALSE)
    {
        /* FLMD0 pin not set */
        g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_FLMD0;                              /* PRQA S 2814 */
    }
    else
    {
        /* Use local variable to remove IAR warning */
        startAddr = g_fcl_data_str.fcl_reqInt_pstr->idx_u32;

        if ((((startAddr + (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 * (2 * R_FLI_BLOCK_TRANSFERSIZE))) <=
             ((g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 * R_FLI_SMALL_BLOCK_SIZE) +
              (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32 * R_FLI_BLOCK_SIZE))) ||
            (((startAddr + (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 * (2 * R_FLI_BLOCK_TRANSFERSIZE))) <=
              (R_FLI_CFLASH2_START + (g_fcl_data_str.fcl_deviceInfo_str.cf2BlkCnt32kB_u32 * R_FLI_BLOCK_SIZE))) &&
              (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 >= R_FLI_CFLASH2_START))) &&
             ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 % (2 * R_FLI_BLOCK_TRANSFERSIZE)) == 0) &&
              (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 > 0))
        {   
            /* user area and data is aligned */
            g_fcl_data_str.fcl_opVar_str.buffAddr_u32 = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;
            g_fcl_data_str.fcl_opVar_str.opCnt_u16 = g_fcl_data_str.fcl_reqInt_pstr->cnt_u16;
            g_fcl_data_str.fcl_opVar_str.opAddr_u32 = g_fcl_data_str.fcl_reqInt_pstr->idx_u32;
            g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_BUSY;
            g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
        }
        else if ((((startAddr + (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 * (2 * R_FLI_BLOCK_TRANSFERSIZE))) <= 
                   (R_FLI_USERBOOT_AREA_START +
                   (g_fcl_data_str.fcl_deviceInfo_str.usrBlkCnt32kB_u32 * R_FLI_BLOCK_SIZE))) &&
                  ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 % (2 * R_FLI_BLOCK_TRANSFERSIZE)) == 0)) &&
                   (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 > 0) && 
                   (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 >= R_FLI_USERBOOT_AREA_START) &&
                   (g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu != R_FCL_FALSE))
        {        
            /* extended user area selected and data is aligned */
            g_fcl_data_str.fcl_opVar_str.buffAddr_u32 = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;
            g_fcl_data_str.fcl_opVar_str.opCnt_u16 = g_fcl_data_str.fcl_reqInt_pstr->cnt_u16;
            g_fcl_data_str.fcl_opVar_str.opAddr_u32 = g_fcl_data_str.fcl_reqInt_pstr->idx_u32;
            g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_BUSY;
            g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
        }
        else
        {
            /* parameter error */
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
        }
    }
} /* R_FCL_FCUFct_Write */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_Write
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_HandleMultiOperation
 *********************************************************************************************************************/
/**
 * This function handles all operations which need several handler calls (e.g. erase and write)
 * Depending of the executed command, the FACI is configurured. After that the result is checked until the operation
 * is finished. At the end the result variable of the request structure is updated.
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
void R_FCL_FCUFct_HandleMultiOperation (void)
{
    r_fcl_status_t ret;
    uint32_t       regFSTATR;
    uint16_t       data;
    uint32_t       i;
    uint32_t       readData;
    r_fcl_flag_t   skipCode;
    r_fcl_flag_t   isCmdSpdExec;

    if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_OP_PREPARE_STATE)
    {
        g_fcl_data_str.fcl_deviceInfo_str.matSel_u16  = R_FCL_I_read_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16);

        if (R_FCL_FALSE == g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu)
        {
            if ((g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_LOCKBIT) ||      /* PRQA S 2814 */
                (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_LOCKBIT) ||
                (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_ERASE) ||
                (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_WRITE))
            {
                ret = R_FCL_FCUFct_SwitchMode_Start (R_FCL_WRITE_MODE);
            }
            else 
            {
                /* Extra area operations */
                ret = R_FCL_FCUFct_SwitchMode_Start (R_FCL_CFG_WRITE_MODE);
            }

            if (ret == R_FCL_OK)
            {
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_PE_MODE_SWITCH_STATE;
                g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
            }
            else
            {
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
                g_fcl_data_str.fcl_opVar_str.opResult_enu = ret;
            }
        }
        else
        {
            g_fcl_data_str.fcl_suspendInfo_str.matSel_u16 = g_fcl_data_str.fcl_deviceInfo_str.matSel_u16;
            g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = 
                                              g_fcl_data_str.fcl_opVar_str.opStatus_enu;
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_BUSY_CHECK_PE_MODE_SWITCH_STATE)
    {
        ret = R_FCL_FCUFct_SwitchMode_Check();
        if (R_FCL_OK == ret)
        {
            if (R_FCL_FALSE == g_fcl_data_str.fcl_cancelReq)
            {
                if (R_FCL_FALSE == g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu)
                {
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_START_OP_STATE;
                }
                else
                {
                    g_fcl_data_str.fcl_suspendInfo_str.matSel_u16 = g_fcl_data_str.fcl_deviceInfo_str.matSel_u16;
                    g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                }
            }
            else
            {
                R_FCL_FCUFct_ForcedStop();
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
            }
        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_BUSY_START_OP_STATE)
    {
        if (R_FCL_FALSE == g_fcl_data_str.fcl_cancelReq)
        {
            if (R_FCL_FALSE == g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu)
            {
                /* start a new operation */
                if ((g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_LOCKBIT) ||
                    (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_LOCKBIT) ||
                    (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_ERASE) ||
                    (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_WRITE))
                {
                    if (g_fcl_data_str.fcl_deviceInfo_str.enableLockBits_enu == R_FCL_TRUE)
                    {
                        R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FPROTR_U16, 
                                                  R_FPSYS_REGBIT_FPROTR_ON + R_FPSYS_REGBIT_FPROTR_KEY);
                    }
                    else
                    {
                        R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FPROTR_U16, 
                                                  R_FPSYS_REGBIT_FPROTR_OFF + R_FPSYS_REGBIT_FPROTR_KEY);
                    }

                    if (g_fcl_data_str.fcl_opVar_str.opAddr_u32 < R_FLI_USERBOOT_AREA_START)
                    {
                        R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16,
                                                  R_FPSYS_REGBIT_FMATSELC_USS + R_FPSYS_REGBIT_FMATSELC_KEY);
                    }
                    else
                    {
                        R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16,
                                                  R_FPSYS_REGBIT_FMATSELC_EX2S + R_FPSYS_REGBIT_FMATSELC_KEY);
                    }
                }
                
                R_FCL_I_write_memory_u32 (R_FPSYS_REGADD_FSADR_U32, g_fcl_data_str.fcl_opVar_str.opAddr_u32);
                if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_ERASE)
                {
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_ERASE);
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_EXE);
                    g_fcl_data_str.fcl_opVar_str.opCnt_u16 -= 1u;
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_RESULT_STATE;
                }
                else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_WRITE)
                {
                    /* setup sequencer to write */
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_WRITE);
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FLI_BLOCK_TRANSFERSIZE);

                    /* copy first half of the data to write */
                    for (i = 0; i < R_FLI_BLOCK_TRANSFERSIZE_PART; i++)
                    {
                        /* assume reading of unaligned source buffer */
                        data = (uint16_t)R_FCL_I_read_memory_u08 (g_fcl_data_str.fcl_opVar_str.buffAddr_u32 + 1u);
                        data <<= 8;
                        data += (uint16_t)R_FCL_I_read_memory_u08 (g_fcl_data_str.fcl_opVar_str.buffAddr_u32);
                        g_fcl_data_str.fcl_opVar_str.buffAddr_u32 += 2u;
                        R_FCL_I_write_memory_u16 (R_FPSYS_FLASH_ACCESS_AREA_BASE, data);
                        do
                        {
                            regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);
                        }
                        while (R_FPSYS_REGBIT_FSTATR_DBFULL == (regFSTATR & R_FPSYS_REGBIT_FSTATR_DBFULL)); /* CCOV 0004 */
                    }
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_START_WRITE_OP_STATE;
                }
                else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_LOCKBIT)
                {
                    /* setup sequencer to read lockbits */
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_READ_LOCKBIT);
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_EXE);
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_RESULT_STATE;
                }
                else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_LOCKBIT)
                {
                    /* setup sequencer to read lockbits */
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_WRITE_LOCKBIT);
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_EXE);
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_RESULT_STATE;
                }
                else
                {
                    /* write to config area */
                    if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_OTP)
                    {
                        R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_PRG_CFG_OTP);
                    }
                    else
                    {
                        R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_PRG_CFG);
                    }
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_PRG_CFG_TRANSFERSIZE);
                    for (i = 0; i < R_PRG_CFG_TRANSFERSIZE; i++)
                    {
                        /* assume reading of unaligned source buffer */
                        data = (uint16_t)R_FCL_I_read_memory_u08 (g_fcl_data_str.fcl_opVar_str.buffAddr_u32 + 1u);
                        data <<= 8;
                        data += (uint16_t)R_FCL_I_read_memory_u08 (g_fcl_data_str.fcl_opVar_str.buffAddr_u32);
                        g_fcl_data_str.fcl_opVar_str.buffAddr_u32 += 2u;
                        R_FCL_I_write_memory_u16 (R_FPSYS_FLASH_ACCESS_AREA_BASE, data);
                    }
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_EXE);
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_RESULT_STATE;
                }
            }
            else
            {
                g_fcl_data_str.fcl_suspendInfo_str.matSel_u16 = g_fcl_data_str.fcl_deviceInfo_str.matSel_u16;
                g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
            }
        }
        else
        {
            R_FCL_FCUFct_ForcedStop();
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_BUSY_START_WRITE_OP_STATE)
    {
        /* copy second half of the data to write */
        for (i = R_FLI_BLOCK_TRANSFERSIZE_PART; i < R_FLI_BLOCK_TRANSFERSIZE; i++)
        {
            /* assume reading of unaligned source buffer */
            data = (uint16_t)R_FCL_I_read_memory_u08 (g_fcl_data_str.fcl_opVar_str.buffAddr_u32 + 1u);
            data <<= 8;
            data += (uint16_t)R_FCL_I_read_memory_u08 (g_fcl_data_str.fcl_opVar_str.buffAddr_u32);
            g_fcl_data_str.fcl_opVar_str.buffAddr_u32 += 2u;
            R_FCL_I_write_memory_u16 (R_FPSYS_FLASH_ACCESS_AREA_BASE, data);
            do
            {
                regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);
            }
            while (R_FPSYS_REGBIT_FSTATR_DBFULL == (regFSTATR & R_FPSYS_REGBIT_FSTATR_DBFULL));    /* CCOV 0005 */
        }
        R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_EXE);
        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_RESULT_STATE;
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_BUSY_CHECK_RESULT_STATE)
    {
        if (R_FCL_FALSE == g_fcl_data_str.fcl_cancelReq)
        {
            isCmdSpdExec = R_FCL_FALSE;

            if ( (R_FCL_TRUE == g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu) &&
                 (g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr == R_FCL_REQUEST_POINTER_UNDEFINED) )
            {
                g_fcl_data_str.fcl_suspendInfo_str.matSel_u16 = R_FCL_I_read_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16);
                regFSTATR = R_FCL_I_read_memory_u32 (R_FPSYS_REGADD_FSTATR_U32);
                if (R_FPSYS_REGBIT_FSTATR_SUSRDY == (regFSTATR & R_FPSYS_REGBIT_FSTATR_SUSRDY))
                {
                    R_FCL_I_write_memory_u08 (R_FPSYS_FLASH_ACCESS_AREA_BASE, R_FPSYS_FLASH_ACCESS_CMD_SUSPEND);
                    g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr = g_fcl_data_str.fcl_reqInt_pstr;
                    isCmdSpdExec = R_FCL_TRUE;
                }
            }
        
            /* check if sequencer is still busy */
            ret = R_FCL_FCUFct_GetResult (isCmdSpdExec);
            if (ret != R_FCL_BUSY)
            {
                if ((g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr != R_FCL_REQUEST_POINTER_UNDEFINED) &&
                    (R_FCL_TRUE == g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu))
                {
                    if (ret != R_FCL_OK)
                    {
                        g_fcl_data_str.fcl_opVar_str.opResult_enu = ret;
                        g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
                    }
                    else
                    {
                        g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = 
                                                                                        R_FCL_BUSY_CHECK_RESULT_STATE;
                    }
                    g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                }
                else
                {
                    g_fcl_data_str.fcl_opVar_str.opResult_enu = ret;
                    if (g_fcl_data_str.fcl_opVar_str.opResult_enu != R_FCL_OK)
                    {
                        /* an error occured -> stop operation */
                        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                    }
                    else
                    {
                        if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_WRITE)
                        {
                            /* Use local variable to remove IAR warning */
                            readData = g_fcl_data_str.fcl_reqInt_pstr->idx_u32;
                            if ((g_fcl_data_str.fcl_opVar_str.opAddr_u32 + R_FLI_MIN_WRITESIZE) <
                                (readData + (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 * R_FLI_MIN_WRITESIZE)))
                            {
                                /* still some data to write */
                                g_fcl_data_str.fcl_opVar_str.opAddr_u32 += R_FLI_MIN_WRITESIZE;
                                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_START_OP_STATE;
                                g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
                            }
                            else
                            {
                                /* operation finished */
                                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                            }
                        }
                        else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_ERASE)
                        {
                            if (g_fcl_data_str.fcl_opVar_str.opCnt_u16 > 0u)
                            {
                                /* still some blocks to erase */
                                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_START_OP_STATE;
                                g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
                                /* Use local variable to remove IAR warning */
                                readData = (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FLI_USERBOOT_AREA_MASK);
                                if ((readData +
                                    (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 - g_fcl_data_str.fcl_opVar_str.opCnt_u16))
                                     <= g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8)
                                {
                                    /* last block was a 8kb block */
                                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 += R_FLI_SMALL_BLOCK_SIZE;
                                }
                                else if ((readData +
                                     (g_fcl_data_str.fcl_reqInt_pstr->cnt_u16 - g_fcl_data_str.fcl_opVar_str.opCnt_u16))
                                     == (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 + 
                                         g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32))
                                {
                                    /* bank crossing */
                                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FLI_CFLASH2_START;
                                }
                                else
                                {
                                    /* last block was a 32kb block */
                                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 += R_FLI_BLOCK_SIZE;
                                }
                            }
                            else
                            {
                                /* operation finished */
                                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                            }
                        }
                        else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_GET_LOCKBIT)
                        {
                            readData = (uint32_t)R_FCL_I_read_memory_u08 (R_FPSYS_REGADD_FLKSTAT_U8);
                            readData = ((~readData) & (uint32_t)0x01u);
                            R_FCL_I_write_unaligned_memory_u32(g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, readData);
                            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                        }
                        else if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_SET_LOCKBIT)
                        {
                            /* operation finished */
                            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                        }
                        else
                        {
                            g_fcl_data_str.fcl_opVar_str.opCnt_u16 -= R_PRG_CFG_TRANSFERSIZE;
                            if (g_fcl_data_str.fcl_opVar_str.opCnt_u16 > 0u)
                            {
                                /* still some settings to write */
                                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_START_OP_STATE;
                                g_fcl_data_str.fcl_opVar_str.opResult_enu = R_FCL_BUSY;
                                g_fcl_data_str.fcl_opVar_str.opAddr_u32 += (2u * R_PRG_CFG_TRANSFERSIZE);
                            }
                            else
                            {
                                /* operation finished */
                                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            R_FCL_FCUFct_ForcedStop();
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_SWITCH_READ_MODE_STATE;
        }
        
        if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_BUSY_SWITCH_READ_MODE_STATE)
        {
            R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16,
                                      g_fcl_data_str.fcl_deviceInfo_str.matSel_u16 + R_FPSYS_REGBIT_FMATSELC_KEY);
            R_FCL_FCUFct_ClearStatus ();
        }
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_BUSY_SWITCH_READ_MODE_STATE)
    {
        (void)R_FCL_FCUFct_SwitchMode_Start (R_FCL_READ_MODE);
        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_READ_MODE_SWITCH_STATE;
    }
    else if (g_fcl_data_str.fcl_opVar_str.opStatus_enu == R_FCL_BUSY_CHECK_READ_MODE_SWITCH_STATE)
    {
        ret = R_FCL_FCUFct_SwitchMode_Check ();
        if (R_FCL_OK == ret)
        {
            g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
        }
    }
    else
    {
        if (R_FCL_FALSE == g_fcl_data_str.fcl_cancelReq)
        {
            if (R_FCL_FALSE == g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu)
            {
                /* finish operation */
                if ((g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_ERASE) ||
                    (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_WRITE)  )
                {
                    R_FCL_FCUFct_Clear_Cache ();
                }
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = g_fcl_data_str.fcl_opVar_str.opResult_enu;
                R_FCL_FCUFct_RestoreLastState ();
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_IDLE_STATE;
            }
            else
            {
                R_FCL_I_write_memory_u16 (R_FPSYS_REGADD_FMATSELC_U16, 
                                          g_fcl_data_str.fcl_deviceInfo_str.matSel_u16 + R_FPSYS_REGBIT_FMATSELC_KEY);
                if ((g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr == R_FCL_REQUEST_POINTER_UNDEFINED) && 
                    (g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu == R_FCL_OP_IDLE_STATE))
                {
                    g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu = R_FCL_OP_FINISHED_STATE;
                }
                g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr = g_fcl_data_str.fcl_reqInt_pstr;
                g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opAddr_u32 = g_fcl_data_str.fcl_opVar_str.opAddr_u32;
                g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opCnt_u16 = g_fcl_data_str.fcl_opVar_str.opCnt_u16;
                g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opResult_enu =
                    g_fcl_data_str.fcl_opVar_str.opResult_enu;
                
                g_fcl_data_str.fcl_intStatus_enu                     = R_FCL_ISTAT_SUSPENDED;
                g_fcl_data_str.fcl_reqInt_pstr->status_enu           = R_FCL_SUSPENDED;
                g_fcl_data_str.fcl_suspendInfo_str.flagSuspended_enu = R_FCL_TRUE;
                g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
            }
        }
        else
        {
            skipCode = R_FCL_FALSE;
            ret      = R_FCL_CANCELLED;

            if (R_FCL_REQUEST_POINTER_UNDEFINED != g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr)
            {
               /* Check if there is a first command that is suspended */
                if (R_FCL_FALSE != R_FCL_FCUFct_ChkSuspended())
                {
                    ret = R_FCL_FCUFct_SwitchMode_Start (R_FCL_WRITE_MODE);
                    if (R_FCL_OK == ret)
                    {
                        g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_BUSY_CHECK_PE_MODE_SWITCH_STATE;
                        skipCode = R_FCL_TRUE;
                    }
                    else
                    {
                        ret = R_FCL_ERR_INTERNAL;
                    }
                }
            }
        
            if (R_FCL_FALSE == skipCode)
            {
                R_FCL_FCUFct_Clear_Cache ();

                g_fcl_data_str.fcl_intStatus_enu           = R_FCL_ISTAT_PREPARED;
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = ret;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu  = R_FCL_OP_IDLE_STATE;
                g_fcl_data_str.fcl_cancelReq               = R_FCL_FALSE;
                if (R_FCL_REQUEST_POINTER_UNDEFINED != g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr)
                {
                    g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr->status_enu = ret;
                    g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr = R_FCL_REQUEST_POINTER_UNDEFINED;
                    g_fcl_data_str.fcl_suspendInfo_str.flagSuspended_enu = R_FCL_FALSE;
                }
            }
        }
    }
} /* R_FCL_FCUFct_HandleMultiOperation */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_HandleMultiOperation
 *********************************************************************************************************************/


#ifdef R_FCL_SUPPORT_OTP
/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_GetOTPBit
 *********************************************************************************************************************/
/**
 * This function reads out the OTP bit of a given block
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_GetOTPBit (void)                                                                 /* PRQA S 1503 */
{
    uint8_t  readData[16];
    uint8_t  data;
    uint32_t buffAddr;


    buffAddr = g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32;
    if (R_FCL_USERBOOT_AREA_SELECT == (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FCL_USERBOOT_AREA_SELECT))
    {
        /* extended user area selected */
        if (  ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FLI_USERBOOT_AREA_MASK) <
                g_fcl_data_str.fcl_deviceInfo_str.usrBlkCnt32kB_u32)
           && (g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu != R_FCL_FALSE) )
        {
            g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32 = (uint32_t)&readData[0];                /* PRQA S 0306 */
            g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_OTP_UB_OFFSET;
            R_FCL_Fct_CallFromRAM ();
            data = (~(readData[0]));                                                               /* PRQA S 2961 */
            g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32 = buffAddr;
            R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, (uint32_t)data);
        }
        else
        {
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        /* user area */
        if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < g_fcl_data_str.fcl_deviceInfo_str.blkCnt_u16 )
        {
            g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32 = (uint32_t)&readData[0];                /* PRQA S 0306 */
            if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 >
               (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 + g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32))
            {
                g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_OTP_CF2_OFFSET + 
                    (0x10 * ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 - 
                    (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 + 
                     g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32)) / 128));
            }
            else
            {
                g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_OTP_CF1_OFFSET + 
                    (0x10 * (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 / 128));
            }
            R_FCL_Fct_CallFromRAM ();
            if (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 > 0)
            {
                data = readData[(g_fcl_data_str.fcl_reqInt_pstr->idx_u32 / 8) % 16];         
                data = (((uint8_t)(~data)) >> (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 % 8)) & 0x01u;
            }
            else
            {
                if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 == 0)
                {
                    data = ((uint8_t)(~(readData[0]))) & 0x01u;
                }
                else if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 == 1)
                {
                    data = (((uint8_t)(~(readData[0]))) >> 4) & 0x01u;
                }
                else
                {
                    data = readData[((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 + 6) / 8) % 16];    
                
                    data = (((uint8_t)(~data)) >> ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 + 6) % 8)) & 0x01u;
                }
            }
            g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32 = buffAddr;
            R_FCL_I_write_unaligned_memory_u32 (g_fcl_data_str.fcl_reqInt_pstr->bufferAdd_u32, (uint32_t)data);
        }
        else
        {
            g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
        }
    }
} /* R_FCL_FCUFct_GetOTPBit */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_GetOTPBit
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_SetOTPBit
 *********************************************************************************************************************/
/**
 * This function writes the OTP bit of a given block
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_SetOTPBit (void)                                                                 /* PRQA S 1503 */
{
    uint8_t     i;
    uint32_t    blockNo;

    if (g_fcl_data_str.fcl_deviceInfo_str.emulationMode_enu == R_FCL_TRUE)                         /* CCOV 0006 */
    {
        /* do not set OTP bits in emulation mode, but return R_FCL_OK */
        g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;
    }
    else
    {
        for (i = 0; i < 16; i++)
        {
            g_fcl_data_str.fcl_intBuf[i] = 0xffu;
        }
        
        g_fcl_data_str.fcl_opVar_str.buffAddr_u32 = (uint32_t)(&g_fcl_data_str.fcl_intBuf[0]);     /* PRQA S 0306 */
        if (R_FCL_USERBOOT_AREA_SELECT == (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FCL_USERBOOT_AREA_SELECT))
        {
            /* extended user area selected */
            if (  ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FLI_USERBOOT_AREA_MASK) <
                    g_fcl_data_str.fcl_deviceInfo_str.usrBlkCnt32kB_u32)
               && (g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu != R_FCL_FALSE) )
            {
                g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_OTP_UB_OFFSET;
                g_fcl_data_str.fcl_intBuf[0] = (uint8_t)(~0x01u);
                R_FCL_FCUFct_SetConfigArea ();
            }
            else
            {
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
            }
        }
        else
        {
            /* user area */
            if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < g_fcl_data_str.fcl_deviceInfo_str.blkCnt_u16)
            {
                /* Use local variable to remove IAR warning */
                blockNo = g_fcl_data_str.fcl_reqInt_pstr->idx_u32;

                if (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 > 0)
                {
                    g_fcl_data_str.fcl_intBuf[(blockNo / 8) % 16] = ~(0x01u << (blockNo % 8));
                }
                else
                {
                    if (blockNo  == 0)
                    {
                        g_fcl_data_str.fcl_intBuf[0] = (uint8_t)(~0x01u);
                    }
                    else if (blockNo == 1)
                    {
                        g_fcl_data_str.fcl_intBuf[0] = (uint8_t)(~0x10u);
                    }
                    else
                    {
                        g_fcl_data_str.fcl_intBuf[((blockNo + 6) / 8) % 16] = (~(0x01u << ((blockNo + 6) % 8)));
                    }
                }
                if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 > 
                    (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 + 
                     g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32))
                {
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_OTP_CF2_OFFSET + 
                        (0x10 * ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 - 
                        (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 + 
                         g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32)) / 128));
                }
                else
                {
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FPSYS_CONFIG_OTP_CF1_OFFSET + 
                        (0x10 * (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 / 128));
                }
                R_FCL_FCUFct_SetConfigArea ();
            }
            else
            {
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
            }
        }
    }
} /* R_FCL_FCUFct_SetOTPBit */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_SetOTPBit
 *********************************************************************************************************************/
#endif /* ifdef R_FCL_SUPPORT_OTP */


#ifdef R_FCL_SUPPORT_LOCKBIT
/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_HandleLockBit
 *********************************************************************************************************************/
/**
 * This function prepares internal variables to read out the lock bit of a given block
 *
 * @param[in,out] ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_HandleLockBit (void)                                                             /* PRQA S 1503 */
{
    if (R_FCL_FCUFct_CheckMode () == R_FCL_FALSE)
    {
        /* FLMD0 pin not set */
        g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_FLMD0;                              /* PRQA S 2814 */
    }
    else
    {
        if (R_FCL_USERBOOT_AREA_SELECT == (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FCL_USERBOOT_AREA_SELECT))
        {
            /* extended user area selected */
            if (  ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 & R_FLI_USERBOOT_AREA_MASK) <
                    g_fcl_data_str.fcl_deviceInfo_str.usrBlkCnt32kB_u32)
               && (g_fcl_data_str.fcl_deviceInfo_str.accessExtendedUserArea_enu != R_FCL_FALSE) )
            {
                /* setup internal variables for multi operation */
                g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FLI_USERBOOT_AREA_START +
                                                        ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 &
                                                          R_FLI_USERBOOT_AREA_MASK) * R_FLI_BLOCK_SIZE);
                g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_BUSY;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
            }
            else
            {
                /* parameter error */
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
            }
        }
        else
        {
            /* user area */
            if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < g_fcl_data_str.fcl_deviceInfo_str.blkCnt_u16 )
            {
                /* setup internal variables for multi operation */
                if (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8)
                {
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 =
                        g_fcl_data_str.fcl_reqInt_pstr->idx_u32 * R_FLI_SMALL_BLOCK_SIZE;
                }
                else if ( g_fcl_data_str.fcl_reqInt_pstr->idx_u32 < 
                          (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 + 
                           g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32))
                {
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 =
                        (g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 * R_FLI_SMALL_BLOCK_SIZE) +
                        ( (g_fcl_data_str.fcl_reqInt_pstr->idx_u32 - g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8) 
                          * R_FLI_BLOCK_SIZE);
                }
                else
                {
                    /* Bank B available */
                    g_fcl_data_str.fcl_opVar_str.opAddr_u32 = R_FLI_CFLASH2_START +
                       ((g_fcl_data_str.fcl_reqInt_pstr->idx_u32 - g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt8kB_u8 -
                         g_fcl_data_str.fcl_deviceInfo_str.cf1BlkCnt32kB_u32) * R_FLI_BLOCK_SIZE);
                }
                g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_BUSY;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
            }
            else
            {
                /* parameter error */
                g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_ERR_PARAMETER;
            }
        }
    }
} /* R_FCL_FCUFct_HandleLockBit */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_HandleLockBit
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_FCUFct_LockBitMode
 *********************************************************************************************************************/
/**
 * This function enables or disables the lockbit protection mechanism
 *
 * @param[in]     R_FPSYS_REGBIT_FPROTR_OFF     disables lockbit protection
 *                R_FPSYS_REGBIT_FPROTR_ON      enables lockbit protection
 * @return        ---
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROMRAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROMRAM"
#endif
void R_FCL_FCUFct_LockBitMode (uint16_t mode_u16)                                                  /* PRQA S 1503 */
{
    if (mode_u16 != R_FPSYS_REGBIT_FPROTR_OFF)
    {
        g_fcl_data_str.fcl_deviceInfo_str.enableLockBits_enu = R_FCL_TRUE;
    }
    else
    {
        g_fcl_data_str.fcl_deviceInfo_str.enableLockBits_enu = R_FCL_FALSE;
    }
    g_fcl_data_str.fcl_reqInt_pstr->status_enu = R_FCL_OK;                                         /* PRQA S 2814 */
} /* R_FCL_FCUFct_LockBitMode */
/**********************************************************************************************************************
 End of function R_FCL_FCUFct_LockBitMode
 *********************************************************************************************************************/
#endif /* ifdef R_FCL_SUPPORT_LOCKBIT */
