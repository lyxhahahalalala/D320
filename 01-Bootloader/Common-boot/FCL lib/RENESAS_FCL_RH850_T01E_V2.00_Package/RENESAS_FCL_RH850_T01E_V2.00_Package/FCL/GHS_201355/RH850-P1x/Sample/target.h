#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: target.h $
* Mod. Revision : $Revision: 1.1 $
* Mod. Date     : $Date: 2013/10/14 14:07:55MESZ $
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
  
#ifndef TARGET_H
#define TARGET_H

#if R_FCL_COMPILER == R_FCL_COMP_GHS
    #include "dr7f701312_0.h"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #error include
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #include "dr7f701312.dvf.h"
#endif


/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK

#define FLMD0_PROTECTION_OFF    (0x01u)
#define FLMD0_PROTECTION_ON     (0x00u)

#define FCL_INIT_FLASHACCESS                                        \
            uint32_t i;                                             \
                                                                    \
            /* enable FLMD0 */                                      \
            FLMDPCMD = 0xa5;                                        \
            FLMDCNT  = FLMD0_PROTECTION_OFF;                        \
            FLMDCNT  = ~FLMD0_PROTECTION_OFF;                       \
            FLMDCNT  = FLMD0_PROTECTION_OFF;                        \
            for (i=0; i<10000; i++)                                 \
            {                                                       \
                /* do nothing ... delay time may depend on */       \
                /* external FLMD0 pin connection */                 \
            }                                                       \
                                                                    \
            /* Switch off Flash modification protection*/           \
            FHVE3 = 1;                                              \
            FHVE15 = 1;  
            
#define FCL_DISABLE_FLASHACCESS                                     \
            uint32_t i;                                             \
                                                                    \
            /* enable FLMD0 */                                      \
            FLMDPCMD = 0xa5;                                        \
            FLMDCNT  = FLMD0_PROTECTION_ON;                         \
            FLMDCNT  = ~FLMD0_PROTECTION_ON;                        \
            FLMDCNT  = FLMD0_PROTECTION_ON;                         \
            for (i=0; i<10000; i++)                                 \
            {                                                       \
                /* do nothing ... delay time may depend on */       \
                /* external FLMD0 pin connection */                 \
            }                                                       \
                                                                    \
            /* Switch on Flash modification protection*/            \
            FHVE3 = 0;                                              \
            FHVE15 = 0;  
  

/************************************************************************************************************
Typedef definitions
************************************************************************************************************/


/************************************************************************************************************
Exported global variables
************************************************************************************************************/


/************************************************************************************************************
Exported global functions (to be accessed by other files) 
************************************************************************************************************/



#endif /* end of TARGET_H */
