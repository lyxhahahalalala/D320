#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: fcl_descriptor.c $
* Mod. Revision : $Revision: 1.8 $
* Mod. Date     : $Date: 2014/09/01 13:40:47MESZ $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : FCL descriptor variable definition
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
* Verification: The placeholders are used in commentars only. Therefore rule violation cannot 
*               influency code compilation.
************************************************************************************************************/


/************************************************************************************************************
Includes   <System Includes> , "Project Includes"
************************************************************************************************************/
#include "r_typedefs.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "fcl_descriptor.h"

/************************************************************************************************************
Macro definitions
************************************************************************************************************/


/************************************************************************************************************
Typedef definitions
************************************************************************************************************/


/************************************************************************************************************
Exported global variables (to be accessed by other files) 
************************************************************************************************************/
/*!< FCL decriptor structure */
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section rodata=".R_FCL_CONST"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section const "R_FCL_CONST"
#endif
#if R_FCL_COMPILER == R_FCL_COMP_IAR
    const r_fcl_descriptor_t sampleApp_fclConfig_enu @ "R_FCL_CONST" =
#else
    const r_fcl_descriptor_t sampleApp_fclConfig_enu = 
#endif
{
    FCL_AUTHENTICATION_ID,      /*!< Authentication ID */
    FCL_RAM_ADDRESS,            /*!< Start of RAM range blocked for FCL */             
    FCL_CPU_FREQUENCY_MHZ       /*!< CPU frequency in MHz */                  
};     

/************************************************************************************************************
Private global variables and functions 
************************************************************************************************************/
