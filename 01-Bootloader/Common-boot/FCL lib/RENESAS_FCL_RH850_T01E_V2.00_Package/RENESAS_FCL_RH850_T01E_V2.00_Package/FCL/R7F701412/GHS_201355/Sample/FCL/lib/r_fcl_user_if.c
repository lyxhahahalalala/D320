#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RH850 devices, based on the RV40 Flash technology
*
* File Name     : $Source: r_fcl_user_if.c $
* Lib. Version  : $RH850_FCL_LIB_VERSION_T01: V2.00 $
* Mod. Revision : $Revision: 1.29 $
* Mod. Date     : $Date: 2014/09/01 13:40:55MESZ $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : FCL user interface functions
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
 * MISRA Rule:   MISRA-C 2004 rule 14.1 (QAC message 1505)
 * Reason:       This function is not used within the library, it is only used by the user
 *               application.
 * Verification: This function is part of the library API and therefore essential on the one hand.
 *               On the other hand the QAC warning is correct, because this function is never
 *               called within the library. To solve this dillemma it is neccessary and suitable
 *               to disable the message.
 *********************************************************************************************************************/
 
/*********************************************************************************************************************
 * MISRA Rule:   MISRA-C 2004 rule 11.3 (QAC message 0305)
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


/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_typedefs.h"
#include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl_global.h"
#include "r_fcl.h"
#include "r_fcl_env.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL
    #define R_FCL_INT_STATIC    R_FCL_STATIC
#else
    #define R_FCL_INT_STATIC
#endif

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Exported global variables (to be accessed by other files)
**********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs startdata
    #pragma ghs section bss = ".R_FCL_DATA"
#elif R_FCL_COMPILER == FCL_COMP_IAR
    #pragma segment = "R_FCL_DATA"
    #pragma location = "R_FCL_DATA"
    #define R_FCL_NOINIT __no_init
#elif R_FCL_COMPILER == FCL_COMP_REC
    #pragma section r0_disp32 "R_FCL_DATA"
#endif

R_FCL_NOINIT r_fcl_data_t g_fcl_data_str;

#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section bss = default
    #pragma ghs enddata
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma dataseg = default
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section default
#endif

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL
    R_FCL_STATIC void R_FCL_Handler     (void);
    R_FCL_STATIC void R_FCL_Fct_Handler (void);
#endif


/**********************************************************************************************************************
* Function name: R_FCL_CopySections
*********************************************************************************************************************/
/**
 * User interface function:
 * This function is used to copy FCL functions to a specified destination address by copying the linker
 * segments. Internally not the segment size is used to avoid problems in case of an accidental delete of
 * holes between the segments. The function calculates size of aligned segments instead.
 *
 * @param[in,out] ---
 * @return        R_FCL_OK              Copy routine terminated successful
 *                R_FCL_ERR_REJECTED    Flow error (library not initialized)
 *                R_FCL_ERR_INTERNAL    Internal error during the copy process
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROM"
#endif
r_fcl_status_t R_FCL_CopySections (void)                                                           /* PRQA S 1503 */
{
    uint32_t       curSrcAddr;
    uint32_t       nextSrcAddr;
    uint32_t       destAddr;
    uint32_t       curSize;
    uint32_t       nextSize;
    r_fcl_status_t ret;

    ret = R_FCL_ERR_FLOW;

    if (g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_INIT)
    {
        if (g_fcl_data_str.RTCfg_pstr->addrRam_u32 != R_FCL_NULL)
        {
            destAddr = g_fcl_data_str.RTCfg_pstr->addrRam_u32;
            curSrcAddr = R_FCL_NULL;
            nextSrcAddr = R_FCL_NULL;
            curSize = 0x00;
            nextSize = 0x00;

            /* Copy section R_FCL_CODE_USRINT */
            R_FCL_Fct_CodeUsrInt_CalcRange (&curSrcAddr, &curSize);
            R_FCL_Fct_CodeUsr_CalcRange (&nextSrcAddr, &nextSize);
            curSize = nextSrcAddr - curSrcAddr;
            destAddr = R_FCL_Fct_Copy_Code (curSrcAddr, destAddr, curSize);
            
            /* Copy section R_FCL_CODE_USR */
            curSrcAddr = nextSrcAddr;
            R_FCL_Fct_CodeRam_CalcRange (&nextSrcAddr, &nextSize);
            curSize = nextSrcAddr - curSrcAddr;
            destAddr = R_FCL_Fct_Copy_Code (curSrcAddr, destAddr, curSize);
            
            /* Copy section R_FCL_CODE_RAM */
            curSrcAddr = nextSrcAddr;
            R_FCL_Fct_CodeRomRam_CalcRange (&nextSrcAddr, &nextSize);
            curSize = nextSrcAddr - curSrcAddr;
            destAddr = R_FCL_Fct_Copy_Code (curSrcAddr, destAddr, curSize);
            
            #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
                /* Copy section R_FCL_CODE_ROMRAM */
                curSrcAddr = nextSrcAddr;
                R_FCL_Fct_CodeExProt_CalcRange (&nextSrcAddr, &nextSize);
                curSize = nextSrcAddr - curSrcAddr;
                destAddr = R_FCL_Fct_Copy_Code (curSrcAddr, destAddr, curSize);
            #endif

            /* Copy section R_FCL_CODE_RAM_EX_PROT */
            R_FCL_Fct_CodeExProt_CalcRange (&curSrcAddr, &curSize);
            (void)R_FCL_Fct_Copy_Code (curSrcAddr, destAddr, curSize);
            
            #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL
            g_fcl_data_str.fcl_executionPtr_p =
                (r_fcl_fctPtr_t)R_FCL_CalcFctAddr ((uint32_t)(&R_FCL_FCUFct_GetConfigArea));       /* PRQA S 0305 */
            
                g_fcl_data_str.fcl_handlerPtr_p = (r_fcl_fctPtr_t)R_FCL_CalcFctAddr ((uint32_t)(&R_FCL_Fct_Handler));
            #endif

            ret = R_FCL_OK;
        }
        else
        {
            ret = R_FCL_ERR_INTERNAL;
        }
    }

    return (ret);
} /* R_FCL_CopySections */
/**********************************************************************************************************************
 End of function R_FCL_CopySections
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function name: R_FCL_CalcFctAddr
*********************************************************************************************************************/
/**
 * User interface function:
 * This function calculates the new address of a function copied from ROM to RAM
 *
 * @param[in]     addFct_u32            Pointer to ROM address of copied function
 * @return        New RAM address of function
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROM"
#endif
uint32_t R_FCL_CalcFctAddr (uint32_t addFct_u32)                                                   /* PRQA S 1503 */
{
    uint32_t secAdd;
    uint32_t secSize;
    uint32_t nextSecAdd;
    uint32_t nextSecSize;
    uint32_t destAdd;
    uint32_t exeOff;

    R_FCL_Fct_CodeUsrInt_CalcRange (&secAdd, &secSize);

    /* Check, if the function is in the section */
    destAdd = g_fcl_data_str.RTCfg_pstr->addrRam_u32;
    if ((addFct_u32 >= secAdd) && (addFct_u32 < (secAdd+secSize)))
    {
        destAdd += (addFct_u32 - secAdd);
    }
    else
    {
        R_FCL_Fct_CodeUsr_CalcRange (&nextSecAdd, &nextSecSize);

        /* calculate new destination address */
        secSize = nextSecAdd - secAdd;
        destAdd += secSize;

        /* Check, if the function is in the section */
        secAdd  = nextSecAdd;
        secSize = nextSecSize;
        if ((addFct_u32 >= secAdd) && (addFct_u32 < (secAdd+secSize)))
        {
            destAdd += (addFct_u32 - secAdd);
        }
        else
        {
            R_FCL_Fct_CodeRam_CalcRange (&nextSecAdd, &nextSecSize);

            /* calculate new destination address */
            secSize = nextSecAdd - secAdd;
            destAdd += secSize;

            /* Check, if the function is in the section */
            secAdd  = nextSecAdd;
            secSize = nextSecSize;
            if ((addFct_u32 >= secAdd) && (addFct_u32 < (secAdd+secSize)))
            {
                destAdd += (addFct_u32 - secAdd);
            }
            else
            {
                #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
                    R_FCL_Fct_CodeRomRam_CalcRange (&nextSecAdd, &nextSecSize);

                    /* calculate new destination address */
                    secSize = nextSecAdd - secAdd;
                    destAdd += secSize;

                    /* Check, if the function is in the section */
                    secAdd  = nextSecAdd;
                    secSize = nextSecSize;

                    if ((addFct_u32 >= secAdd) && (addFct_u32 < (secAdd + secSize)))
                    {
                        destAdd += (addFct_u32 - secAdd);
                    }
                    else
                #endif
                {
                    /*Error function address not found */
                    destAdd = 0x00000000;
                }
            }
        }
    }

    if (destAdd != 0x00000000)
    {
        /* If linked address is != execution address, consider execution offset in the result */
        exeOff = R_FCL_Fct_PrgOffset ();
        destAdd -= exeOff;
    }

    return (destAdd);
} /* R_FCL_CalcFctAddr */
/**********************************************************************************************************************
 End of function R_FCL_CalcFctAddr
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetVersionString
 *********************************************************************************************************************/
/**
 * User interface function:
 * Returns the pointer to the library version string, stores in the library code section.
 *
 * @param[in,out] ---
 * @return        pointer to the version string in Flash
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROM"
    #pragma ghs section rodata=".R_FCL_CONST"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROM"
    #pragma section const "R_FCL_CONST"
#endif
const uint8_t *R_FCL_GetVersionString (void)                                                       /* PRQA S 1503 */
{
    #if R_FCL_COMPILER == R_FCL_COMP_IAR
        static const uint8_t fcl_ver_str[] @ "R_FCL_CONST" = R_FCL_VERSION_STRING;
    #else
        static const uint8_t fcl_ver_str[] = R_FCL_VERSION_STRING;
    #endif

    return (&fcl_ver_str[0]);
} /* R_FCL_GetVersionString */
/**********************************************************************************************************************
 End of function R_FCL_GetVersionString
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function name: R_FCL_Init
*********************************************************************************************************************/
/**
 * User interface function:
 * This function initializes the FCL. It must be called before any execution of a FCL function.
 * Sequence:
 * - Call the function to initialize the library RAM and do parameter checks
 *
 * In error case the library is set to not initialized (R_FCL_ISTAT_NOINIT)
 *
 * @param[in,out] descriptor_pstr       Pointer to the FCL descriptor structure
 * @return        R_FCL_OK              Initialization was successful
 *                R_FCL_ERR_PARAMETER   FCL configuration parameter error (e.g. illegal descriptor)
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_ROM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_ROM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_ROM"
#endif
r_fcl_status_t R_FCL_Init (const r_fcl_descriptor_t * descriptor_pstr)                             /* PRQA S 1503 */
{
    uint32_t       i;
    uint32_t       fcl_addr;
    r_fcl_status_t ret;

    /* ----------------------- Initially clear the complete fcl_data structure ------------------- */
    fcl_addr = (uint32_t)(&g_fcl_data_str);                                                        /* PRQA S 0306 */

    for (i = 0; i < sizeof (g_fcl_data_str); i++)
    {
        R_FCL_I_write_memory_u08 (fcl_addr, 0u);
        fcl_addr++;
    }

    ret = R_FCL_OK;
    g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_NOINIT;


    /* ----------------------- Variable initializations ------------------------------------------ */
    if (R_FCL_DESCRIPTOR_POINTER_UNDEFINED != descriptor_pstr)
    {
        g_fcl_data_str.fcl_reqInt_pstr  = R_FCL_REQUEST_POINTER_UNDEFINED;

        #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL
            g_fcl_data_str.fcl_handlerPtr_p = R_FCL_NULL;
        #endif

        g_fcl_data_str.RTCfg_pstr = descriptor_pstr;
        g_fcl_data_str.fcl_deviceInfo_str.enableLockBits_enu = R_FCL_TRUE;
        
        /* ----------------------- Update library status --------------------------------------- */
        g_fcl_data_str.fcl_intStatus_enu = R_FCL_ISTAT_INIT;
    }
    else
    {
        ret = R_FCL_ERR_PARAMETER;
    }

    return (ret);
} /* R_FCL_Init */
/**********************************************************************************************************************
 End of function R_FCL_Init
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function name: R_FCL_Handler
*********************************************************************************************************************/
/**
 * User interface function:
 * This function forwards an initiated Flash operation until it is finished. At operation end the
 * result is set in the request structure.
 * After initiation by R_FDL_Execute, the function R_FCL_HandlerFct need to be called regularly to finish
 * the operation.
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
R_FCL_INT_STATIC void R_FCL_Handler (void)                                                         /* PRQA S 1503 */
{

    if ((R_FCL_REQUEST_POINTER_UNDEFINED != g_fcl_data_str.fcl_suspendInfo_str.fcl_reqSuspend_pstr) &&
        (R_FCL_ISTAT_SUSPENDED == g_fcl_data_str.fcl_intStatus_enu))
    {
        if (g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu == R_FCL_TRUE)
        {
            #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
                R_FCL_FCUFct_HandleResume ();
            #endif
        }
    }
    else if ((R_FCL_REQUEST_POINTER_UNDEFINED != g_fcl_data_str.fcl_reqInt_pstr) &&
             (R_FCL_ISTAT_BUSY == g_fcl_data_str.fcl_intStatus_enu))
    {
        if (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_PREPARE_ENV)
        {
            R_FCL_FCUFct_PrepareEnvironment ();
        }
        else
        {
            R_FCL_FCUFct_HandleMultiOperation ();
        }
    }
    else
    {
        /* Do nothing */
    }
} /* R_FCL_Handler */
/**********************************************************************************************************************
 End of function R_FCL_Handler
 *********************************************************************************************************************/

#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL
    /******************************************************************************************************************
     * Function name: R_FCL_Fct_Handler
     *****************************************************************************************************************/
    /**
     * This function calls the Handler in the Internal mode
     *
     * @param[in,out] ---
     * @return        ---
     */
    /*****************************************************************************************************************/
    #if R_FCL_COMPILER == R_FCL_COMP_GHS
        #pragma ghs section text = ".R_FCL_CODE_RAM"
    #elif R_FCL_COMPILER == R_FCL_COMP_IAR
        #pragma location = "R_FCL_CODE_RAM"
    #elif R_FCL_COMPILER == R_FCL_COMP_REC
        #pragma section text "R_FCL_CODE_RAM"
    #endif
    R_FCL_STATIC void R_FCL_Fct_Handler (void)                                                     /* PRQA S 1505 */
    {
        do
        {
            R_FCL_Handler ();
        }
        while (g_fcl_data_str.fcl_reqInt_pstr->status_enu == R_FCL_BUSY);
    }
    /******************************************************************************************************************
     End of function R_FCL_Fct_Handler
     *****************************************************************************************************************/
#endif /* if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL */


/**********************************************************************************************************************
 * Function name: R_FCL_Execute
 *********************************************************************************************************************/
/**
 * User interface function:
 * This function initiates a Flash operation (Erase/Write/...).
 * After initiation, the function R_FCL_Handler needs to be called regularely to finish the operation.
 *
 * @param[in,out] request_pstr          Pointer to the operation request structure.
 *                                      - Structure input elements: \n
 *                                      command_enu, bufferAdd_u32, idx_u32, cnt_u16
 *                                      - Structure output element: \n
 *                                      status_enu
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
void R_FCL_Execute (r_fcl_request_t * request_pstr)                                                /* PRQA S 1503 */
{
    if (R_FCL_REQUEST_POINTER_UNDEFINED != request_pstr)
    {
        /* initialize the library */
        if ((g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_INIT) && 
            (request_pstr->command_enu        == R_FCL_CMD_PREPARE_ENV))
        {
            if (R_FCL_FCUFct_CheckMode () == R_FCL_TRUE)
            {
                g_fcl_data_str.fcl_reqInt_pstr            = request_pstr;
                g_fcl_data_str.fcl_intStatus_enu          = R_FCL_ISTAT_BUSY;
                g_fcl_data_str.fcl_opVar_str.opStatus_enu = R_FCL_OP_PREPARE_STATE;
                request_pstr->status_enu                  = R_FCL_BUSY;
                #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL
                    /* call the handler library internal */
                    while (R_FCL_BUSY == request_pstr->status_enu)
                    {
                        (*g_fcl_data_str.fcl_handlerPtr_p)();
                    }
                #endif
            }
            else
            {
                request_pstr->status_enu = R_FCL_ERR_FLMD0;
            }
        }
        else if ((g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_PREPARED) ||
                 (g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_SUSPENDED))
        {
            g_fcl_data_str.fcl_reqInt_pstr = request_pstr;
            request_pstr->status_enu       = R_FCL_BUSY;

            if (request_pstr->command_enu == R_FCL_CMD_ERASE)
            {
                R_FCL_FCUFct_Erase ();
            }
            else if (request_pstr->command_enu == R_FCL_CMD_WRITE)
            {
                if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                {
                    request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                }
                else
                {
                    R_FCL_FCUFct_Write ();
                }
            }
            #ifdef R_FCL_SUPPORT_LOCKBIT
                else if (request_pstr->command_enu == R_FCL_CMD_GET_LOCKBIT)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_HandleLockBit ();
                    }
                }
                else if (request_pstr->command_enu == R_FCL_CMD_SET_LOCKBIT)
                {
                    R_FCL_FCUFct_HandleLockBit ();
                }
                else if (request_pstr->command_enu == R_FCL_CMD_ENABLE_LOCKBITS)
                {
                    R_FCL_FCUFct_LockBitMode (R_FPSYS_REGBIT_FPROTR_ON);
                }
                else if (request_pstr->command_enu == R_FCL_CMD_DISABLE_LOCKBITS)
                {
                    R_FCL_FCUFct_LockBitMode (R_FPSYS_REGBIT_FPROTR_OFF);
                }
            #endif /* ifdef R_FCL_SUPPORT_LOCKBIT */
            #ifdef R_FCL_SUPPORT_OTP
                else if (request_pstr->command_enu == R_FCL_CMD_GET_OTP)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_GetOTPBit ();
                    }
                }
                else if (request_pstr->command_enu == R_FCL_CMD_SET_OTP)
                {
                    R_FCL_FCUFct_SetOTPBit ();
                }
            #endif
            #ifdef R_FCL_SUPPORT_OPB
                else if (request_pstr->command_enu == R_FCL_CMD_GET_OPB)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_Fct_CallFromRAM ();
                    }
                }
                else if (request_pstr->command_enu == R_FCL_CMD_SET_OPB)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_SetConfigArea ();
                    }
                }
            #endif /* ifdef R_FCL_SUPPORT_OPB */
            #ifdef R_FCL_SUPPORT_ID
                else if (request_pstr->command_enu == R_FCL_CMD_GET_ID)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_Fct_CallFromRAM ();
                    }
                }
                else if (request_pstr->command_enu == R_FCL_CMD_SET_ID)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_SetConfigArea ();
                    }
                }
            #endif /* ifdef R_FCL_SUPPORT_ID */
            #ifdef R_FCL_SUPPORT_RESETVECTOR
                else if (request_pstr->command_enu == R_FCL_CMD_GET_RESET_VECTOR)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_Fct_CallFromRAM ();
                    }
                }
                else if (request_pstr->command_enu == R_FCL_CMD_SET_RESET_VECTOR)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_SetConfigArea ();
                    }
                }
            #endif /* ifdef R_FCL_SUPPORT_RESETVECTOR */
            #ifdef R_FCL_SUPPORT_SECURITYFLAGS
                else if ((request_pstr->command_enu == R_FCL_CMD_SET_READ_PROTECT_FLAG) ||
                         (request_pstr->command_enu == R_FCL_CMD_SET_WRITE_PROTECT_FLAG) ||
                         (request_pstr->command_enu == R_FCL_CMD_SET_ERASE_PROTECT_FLAG) ||
                         (request_pstr->command_enu == R_FCL_CMD_SET_SERIAL_PROG_DISABLED) ||
                         (request_pstr->command_enu == R_FCL_CMD_SET_SERIAL_ID_ENABLED))
                {
                    R_FCL_FCUFct_SetConfigArea ();
                }
            #endif
            #ifdef R_FCL_SUPPORT_DEVICENAME
                else if (request_pstr->command_enu == R_FCL_CMD_GET_DEVICE_NAME)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_GetDeviceName ();
                    }
                }
            #endif
            #ifdef R_FCL_SUPPORT_BLOCKCNT
                else if (request_pstr->command_enu == R_FCL_CMD_GET_BLOCK_CNT)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_GetBlockCnt ();
                    }
                }
            #endif
            #ifdef R_FCL_SUPPORT_BLOCKENDADDR
                else if (request_pstr->command_enu == R_FCL_CMD_GET_BLOCK_END_ADDR)
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_GetBlockEndAddr ();
                    }
                }
            #endif
            #ifdef R_FCL_SUPPORT_SECURITYFLAGS
                else if ((request_pstr->command_enu == R_FCL_CMD_GET_READ_PROTECT_FLAG) ||
                         (request_pstr->command_enu == R_FCL_CMD_GET_WRITE_PROTECT_FLAG) ||
                         (request_pstr->command_enu == R_FCL_CMD_GET_ERASE_PROTECT_FLAG) ||
                         (request_pstr->command_enu == R_FCL_CMD_GET_SERIAL_PROG_DISABLED) ||
                         (request_pstr->command_enu == R_FCL_CMD_GET_SERIAL_ID_ENABLED))
                {
                    if (request_pstr->bufferAdd_u32 == R_FCL_NULL)
                    {
                        request_pstr->status_enu = R_FCL_ERR_PARAMETER;
                    }
                    else
                    {
                        R_FCL_FCUFct_GetSecurityFlag ();
                    }
                }
            #endif
            else if (request_pstr->command_enu == R_FCL_CMD_PREPARE_ENV)
            {
                request_pstr->status_enu = R_FCL_ERR_FLOW;
            }
            else
            {
                request_pstr->status_enu = R_FCL_ERR_COMMAND;
            }
            #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_INTERNAL
                /* call the handler library internal */
                while (R_FCL_BUSY == request_pstr->status_enu)
                {
                    (*g_fcl_data_str.fcl_handlerPtr_p)();
                }
            #endif
        }
        else if (g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_BUSY)
        {
            request_pstr->status_enu = R_FCL_ERR_REJECTED;
        }
        else
        {
            request_pstr->status_enu = R_FCL_ERR_FLOW;
        }
    }
} /* R_FCL_Execute */
/**********************************************************************************************************************
 End of function R_FCL_Execute
 *********************************************************************************************************************/

#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
/**********************************************************************************************************************
 * Function name: R_FCL_SuspendRequest
 *********************************************************************************************************************/
/**
 * User interface function:
 * Sets the suspend request flag for the write or erase operation
 *
 * Note:
 * The operation is not immediately suspended. Suspend handling is controlled by R_FCL_Handler.
 * The suspend status is returned by the status variable of the request structure (Set in
 * R_FCL_Handler).
 *
 * @param[in,out] ---
 * @return        R_FCL_OK           Suspend request accepted
 *                R_FCL_ERR_FLOW     Suspend request rejected -> currently no operation ongoing or 
 *                                                               a cancel or suspend request is already accepted 
 *                R_FCL_ERR_REJECTED Suspend request rejected -> flow is correct, but the operation to be suspended is
 *                                                               not an Erase or Write
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
    r_fcl_status_t R_FCL_SuspendRequest (void)                                                     /* PRQA S 1503 */
    {
        r_fcl_status_t ret;

        ret = R_FCL_ERR_FLOW;

        if ((R_FCL_ISTAT_BUSY == g_fcl_data_str.fcl_intStatus_enu) &&
            (R_FCL_FALSE == g_fcl_data_str.fcl_cancelReq) &&
            (R_FCL_FALSE == g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu ))
        {
            if ((g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_ERASE) || 
                (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_WRITE))
            {
                g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_TRUE;
                g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu = R_FCL_FALSE;
                ret = R_FCL_OK;
            }
            else
            {
                ret = R_FCL_ERR_REJECTED;
            }
        }

        return (ret);
    }
/**********************************************************************************************************************
 End of function R_FCL_SuspendRequest
 *********************************************************************************************************************/
#endif /* if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER */

#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
/**********************************************************************************************************************
 * Function name: R_FCL_ResumeRequest
 *********************************************************************************************************************/
/**
 * User interface function:
 * Sets the resume request flag for the write or erase operation
 *
 * Note:
 * The operation is not immediately resumed. Resume handling is controlled by R_FCL_Handler.
 * The status is returned by the status variable of the request structure (Set in
 * R_FCL_Handler).
 *
 * @param[in,out] ---
 * @return        R_FCL_OK          Resume request accepted
 *                R_FCL_ERR_FLOW    Resume request rejected -> currently no operation supended
 *                                                          -> on-going data flash operation
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
    r_fcl_status_t R_FCL_ResumeRequest (void)                                                      /* PRQA S 1503 */
    {
        r_fcl_status_t ret;

        ret = R_FCL_ERR_FLOW;

        if ((g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_SUSPENDED) &&
            (g_fcl_data_str.fcl_cancelReq == R_FCL_FALSE) &&
            (g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu == R_FCL_FALSE))
        {
            if (R_FCL_BUSY_CHECK_RESULT_STATE == g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu)
            {
                if (R_FCL_OK == R_FCL_FCUFct_SwitchMode_Start (R_FCL_WRITE_MODE))
                {
                    g_fcl_data_str.fcl_waitAckModeSwitch = R_FCL_TRUE;
                    g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
                    g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu = R_FCL_TRUE;
                    ret = R_FCL_OK;
                }
            }
            else
            {
                g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
                g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu = R_FCL_TRUE;
                ret = R_FCL_OK;
            }
        }

        return (ret);
    }
/**********************************************************************************************************************
 End of function R_FCL_ResumeRequest
 *********************************************************************************************************************/
#endif /* if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER */

#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
/**********************************************************************************************************************
 * Function name: R_FCL_CancelRequest
 *********************************************************************************************************************/
/**
 * User interface function:
 * Sets the cancel request flag for the write or erase operation
 *
 * Note:
 * The operation is not immediately cancelled. Cancel handling is controlled by R_FCL_Handler.
 * The cancel status is returned by the status variable of the request structure (Set in
 * R_FCL_Handler).
 *
 * @param[in,out] ---
 * @return        R_FCL_OK           Cancel request accepted
 *                R_FCL_ERR_FLOW     Cancel request rejected -> currently no operation ongoing or suspended
 *                                                           -> on-going data flash operation
 *                                                           -> no previous cancel request pending
 *                R_FCL_ERR_REJECTED Cancel request rejected -> flow is correct, but the operation to be cancelled is
 *                                                              not an Erase or Write
 */
/*********************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section text = ".R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma location = "R_FCL_CODE_RAM"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section text "R_FCL_CODE_RAM"
#endif
    r_fcl_status_t R_FCL_CancelRequest (void)                                                      /* PRQA S 1503 */
    {
        r_fcl_status_t ret;

        ret = R_FCL_ERR_FLOW;

        if (((g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_BUSY) || 
             (g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_SUSPENDED)) &&
            (g_fcl_data_str.fcl_cancelReq == R_FCL_FALSE))
        {
            if ((g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_ERASE) || 
                (g_fcl_data_str.fcl_reqInt_pstr->command_enu == R_FCL_CMD_WRITE))
            {
                if (g_fcl_data_str.fcl_intStatus_enu == R_FCL_ISTAT_SUSPENDED)
                {
                    if (R_FCL_BUSY_CHECK_RESULT_STATE == g_fcl_data_str.fcl_suspendInfo_str.fcl_opSuspend_str.opStatus_enu)
                    {
                        if (R_FCL_FALSE == g_fcl_data_str.fcl_waitAckModeSwitch)
                        {
                            if (R_FCL_OK == R_FCL_FCUFct_SwitchMode_Start (R_FCL_WRITE_MODE))
                            {
                                g_fcl_data_str.fcl_waitAckModeSwitch = R_FCL_TRUE;
                                g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu = R_FCL_TRUE;
                                g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
                                g_fcl_data_str.fcl_cancelReq = R_FCL_TRUE;
                                ret = R_FCL_OK;
                            }
                        }
                        else
                        {
                            g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
                            g_fcl_data_str.fcl_cancelReq = R_FCL_TRUE;
                            ret = R_FCL_OK;
                        }
                    }
                    else
                    {
                        g_fcl_data_str.fcl_suspendInfo_str.spdResRequest_enu = R_FCL_TRUE;
                        g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
                        g_fcl_data_str.fcl_cancelReq = R_FCL_TRUE;
                        ret = R_FCL_OK;
                    }
                }
                else
                {
                    g_fcl_data_str.fcl_suspendInfo_str.spdSpdRequest_enu = R_FCL_FALSE;
                    g_fcl_data_str.fcl_cancelReq = R_FCL_TRUE;
                    ret = R_FCL_OK;
                }
            }
            else
            {
                ret = R_FCL_ERR_REJECTED;
            }
        }

        return (ret);
    }
/**********************************************************************************************************************
 End of function R_FCL_CancelRequest
 *********************************************************************************************************************/
#endif /* if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER */

