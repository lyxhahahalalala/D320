#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: fcl_descriptor.h $
* Mod. Revision : $Revision: 1.1 $
* Mod. Date     : $Date: 2014/05/27 15:31:50MESZ $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : Target specific defines
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
  
#ifndef FCL_DESCRIPTOR_H
#define FCL_DESCRIPTOR_H

/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define FCL_CPU_FREQUENCY_MHZ  (240)                          /*!< CPU frequency in MHz */
#define FCL_AUTHENTICATION_ID {0xFFFFFFFF, \
                               0xFFFFFFFF, \
                               0xFFFFFFFF, \
                               0xFFFFFFFF}                    /*!< 128Bit authentication ID */
#define FCL_RAM_ADDRESS        0xFED80000                     /*!< RAM address range blocked for FCL */

/************************************************************************************************************
Typedef definitions
************************************************************************************************************/


/************************************************************************************************************
Exported global variables
************************************************************************************************************/
#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #pragma ghs section rodata=".R_FCL_CONST"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #pragma section const "R_FCL_CONST"
#endif
#if R_FCL_COMPILER == R_FCL_COMP_IAR
    extern const r_fcl_descriptor_t sampleApp_fclConfig_enu @ "R_FCL_CONST"; /*!< FCL descriptor variable */
#else
    extern const r_fcl_descriptor_t sampleApp_fclConfig_enu; /*!< FCL descriptor variable */
#endif


/************************************************************************************************************
Exported global functions (to be accessed by other files) 
************************************************************************************************************/



#endif /* end of FCL_DESCRIPTOR_H */