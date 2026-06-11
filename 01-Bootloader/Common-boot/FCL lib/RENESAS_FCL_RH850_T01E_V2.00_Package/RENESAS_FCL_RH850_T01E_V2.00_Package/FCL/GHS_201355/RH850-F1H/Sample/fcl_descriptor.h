#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: fcl_descriptor.h $
* Mod. Revision : $Revision: 1.4 $
* Mod. Date     : $Date: 2016/01/18 20:11:20MEZ $
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
* Verification: The placeholders are used in commentaries only. Therefore rule violation cannot 
*               influence code compilation.
************************************************************************************************************/

#ifndef FCL_DESCRIPTOR_H
#define FCL_DESCRIPTOR_H

/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define FCL_CPU_FREQUENCY_MHZ  (120)                          /*!< CPU frequency in MHz */
#define FCL_AUTHENTICATION_ID {0xFFFFFFFF, \
                               0xFFFFFFFF, \
                               0xFFFFFFFF, \
                               0xFFFFFFFF}                    /*!< 128Bit authentication ID */
#define FCL_RAM_ADDRESS        0xFEDD0000                     /*!< RAM address range blocked for FCL */

/************************************************************************************************************
Typedef definitions
************************************************************************************************************/


/************************************************************************************************************
Exported global variables
************************************************************************************************************/
extern r_fcl_descriptor_t sampleApp_fclConfig_enu; /*!< FCL descriptor variable */


/************************************************************************************************************
Exported global functions (to be accessed by other files) 
************************************************************************************************************/


#endif /* end of FCL_DESCRIPTOR_H */
