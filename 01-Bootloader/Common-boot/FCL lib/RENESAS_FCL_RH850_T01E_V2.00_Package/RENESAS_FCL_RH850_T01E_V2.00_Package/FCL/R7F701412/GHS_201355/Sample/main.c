#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: main.c $
* Mod. Revision : $Revision: 1.14 $
* Mod. Date     : $Date: 2014/07/08 13:40:34MESZ $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : Application sample for usage of Renesas Flash Control Library (FCL)
************************************************************************************************************/

/************************************************************************************************************
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
************************************************************************************************************/

#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_ON 0292
#endif

/************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 3.1 (QAC message 0292)
* Reason:       To support automatic insertion of revision, module name etc. by the source
*               revision control system it is necessary to violate the rule, because the
*               system uses non basic characters as placeholders.
* Verification: The placeholders are used in commentaries only. Therefore rule violation cannot
*               influence code compilation.
************************************************************************************************************/


/************************************************************************************************************
Includes   <System Includes> , "Project Includes"
************************************************************************************************************/
#include "r_typedefs.h"
#include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "target.h"
#include "fcl_descriptor.h"
#include "fcl_user.h"

/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define FLMD0_PROTECTION_OFF    (0x01u)
#define FLMD0_PROTECTION_ON     (0x00u)
#define REINITIALIZE_BUFFER     for (i = 0; i < sizeof (writeBuffer_u08); i++)          \
                                {                                                       \
                                    writeBuffer_u08[i] = i;                             \
                                }                                                       \
                                for (i = 0; i < (sizeof (readBuffer_u32) >> 2); i++)    \
                                {                                                       \
                                    readBuffer_u32[i]  = 0xaa;                          \
                                }

/************************************************************************************************************
Typedef definitions
************************************************************************************************************/


/************************************************************************************************************
Exported global variables (to be accessed by other files)
************************************************************************************************************/

/* This array reserves the copy area in the device RAM */
#define FCL_RAM_EXECUTION_AREA_SIZE 0x8000

#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs startdata
    #pragma ghs section bss = ".FCL_RESERVED"
    #define R_FCL_NOINIT
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma segment = "FCL_RESERVED"
    #pragma location = "FCL_RESERVED"
    #define R_FCL_NOINIT __no_init
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section r0_disp32 "FCL_RESERVED"
    #define R_FCL_NOINIT
#endif

R_FCL_NOINIT uint8_t FCL_Copy_area[FCL_RAM_EXECUTION_AREA_SIZE];

#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section bss = default
    #pragma ghs enddata
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #pragma dataseg = default
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section default
#endif


/************************************************************************************************************
Private global variables and functions
************************************************************************************************************/

void FCL_Ctrl (void);

/************************************************************************************************************
 * Function name: <Add function name>
 ***********************************************************************************************************/
/**
 * <Add function description>
 *
 * @param[in,out] <Add parameter>      <Add parameter description>
 * @return        <Add return value>
 */
/***********************************************************************************************************/
void main (void)
{
    r_fcl_status_t ret;
    #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
        uint32_t   (* fpFct)(void);
    #endif

    /* initialize CPU and clocks */
    R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK

    #if (defined R_FCL_DEVICE_SPECIFIC_INIT)
        R_FCL_DEVICE_SPECIFIC_INIT
    #endif

    ret = R_FCL_Init (&sampleApp_fclConfig_enu);
    if (R_FCL_OK == ret)
    {
        ret = R_FCL_CopySections ();
    }

    if (R_FCL_OK == ret)
    {
        #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
            fpFct = ( uint32_t (*)() )R_FCL_CalcFctAddr ( (uint32_t)(&FCL_Ctrl) );
            fpFct ();
        #else
            FCL_Ctrl ();
        #endif
    }

    while (1)
    {
        /* do nothing */
    }
} /* main */

#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    #if R_FCL_COMPILER == R_FCL_COMP_GHS
        #pragma ghs section text = ".R_FCL_CODE_USR"
    #elif R_FCL_COMPILER == R_FCL_COMP_IAR
        #pragma location = "R_FCL_CODE_USR"
    #elif R_FCL_COMPILER == R_FCL_COMP_REC
        #pragma section text "R_FCL_CODE_USR"
    #endif
#endif
void FCL_Ctrl (void)
{
    r_fcl_request_t     myRequest;
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
        r_fcl_request_t mySecRequest;
#endif
    uint8_t             writeBuffer_u08[512];
    uint32_t            readBuffer_u32[8];
    uint16_t            i;

    REINITIALIZE_BUFFER;

    FCLUser_Open ();

    /* prepare environment */
    myRequest.command_enu = R_FCL_CMD_PREPARE_ENV;
    R_FCL_Execute (&myRequest);
    #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
        while (R_FCL_BUSY == myRequest.status_enu)
        {
            R_FCL_Handler ();
        }
    #endif

    /* get block count */
    myRequest.command_enu   = R_FCL_CMD_GET_BLOCK_CNT;
    myRequest.bufferAdd_u32 = (uint32_t)&readBuffer_u32[0];
    R_FCL_Execute (&myRequest);

    /* get block end address of block 2 */
    REINITIALIZE_BUFFER;
    myRequest.command_enu   = R_FCL_CMD_GET_BLOCK_END_ADDR;
    myRequest.bufferAdd_u32 = (uint32_t)&readBuffer_u32[0];
    myRequest.idx_u32       = 0x2;                   /* result: 0x5fff */
    R_FCL_Execute (&myRequest);

    /* erase block 2 and 3 */
    myRequest.command_enu = R_FCL_CMD_ERASE;
    myRequest.idx_u32     = 0x2;                   /* erased range = 0x4000 to 0x7fff */
    myRequest.cnt_u16     = 2;
    R_FCL_Execute (&myRequest);
    #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
        while (R_FCL_BUSY == myRequest.status_enu)
        {
            R_FCL_Handler ();
        }
    #endif

    /* write 512 bytes to address 0x40000 (start of block 2) */
    REINITIALIZE_BUFFER;
    myRequest.command_enu = R_FCL_CMD_WRITE;
    myRequest.bufferAdd_u32 = (uint32_t)&writeBuffer_u08[0];
    myRequest.idx_u32       = 0x4000;
    myRequest.cnt_u16       = 2;                    /* written bytes = 256 * cnt_u16 */
    R_FCL_Execute (&myRequest);
    #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
        while (R_FCL_BUSY == myRequest.status_enu)
        {
            R_FCL_Handler ();
        }
    #endif

    FCLUser_Close ();
} /* FCL_Ctrl */

