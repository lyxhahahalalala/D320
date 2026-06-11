#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: target.h $
* Mod. Revision : $Revision: 1.1 $
* Mod. Date     : $Date: 2013/10/14 14:07:25MESZ $
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
    #include "DR7F701Z07.h"

#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #error include
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #include "DR7F701Z07.h"
#endif


/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK                                                          \
    /* PLL */                                                                                       \
    while(SYS.PLL0CLKS.UINT32 != 0x00000007)                                                        \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /* SSCG */                                                                                      \
    SYS.PROT1PHCMD.UINT32 = 0x000000A5;                                                             \
    SYS.CKSC0CTL.UINT32 = 0x00000023;                                                               \
    SYS.CKSC0CTL.UINT32 = 0xFFFFFFDC;                                                               \
    SYS.CKSC0CTL.UINT32 = 0x00000023;                                                               \
    while(SYS.PROT1PS.UINT32 != 0x00000000)                                                         \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /*  */                                                                                          \
    while(SYS.CKSC0ACT.UINT32 != 0x00000023)                                                        \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /* PLL0 */                                                                                      \
    SYS.PROT1PHCMD.UINT32 = 0x000000A5;                                                             \
    SYS.CLKD0DIV.UINT32 = 0x00000002;                                                               \
    SYS.CLKD0DIV.UINT32 = 0xFFFFFFFD;                                                               \
    SYS.CLKD0DIV.UINT32 = 0x00000002;                                                               \
    while(SYS.PROT1PS.UINT32 != 0x00000000)                                                         \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /* PLL1 */                                                                                      \
    SYS.PROT1PHCMD.UINT32 = 0x000000A5;                                                             \
    SYS.CKSC1CTL.UINT32 = 0x00000022;                                                               \
    SYS.CKSC1CTL.UINT32 = 0xFFFFFFDD;                                                               \
    SYS.CKSC1CTL.UINT32 = 0x00000022;                                                               \
    while(SYS.PROT1PS.UINT32 != 0x00000000)                                                         \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /*  */                                                                                          \
    while((SYS.CLKD0STAT.UINT32 != 0x00000001) || (SYS.CKSC1ACT.UINT32 != 0x00000022))              \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /* PLL0 */                                                                                      \
    SYS.PROT1PHCMD.UINT32 = 0x000000A5;                                                             \
    SYS.CLKD0DIV.UINT32 = 0x00000001;                                                               \
    SYS.CLKD0DIV.UINT32 = 0xFFFFFFFE;                                                               \
    SYS.CLKD0DIV.UINT32 = 0x00000001;                                                               \
    while(SYS.PROT1PS.UINT32 != 0x00000000)                                                         \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /* PLL1 */                                                                                      \
    SYS.PROT1PHCMD.UINT32 = 0x000000A5;                                                             \
    SYS.CKSC1CTL.UINT32 = 0x00000024;                                                               \
    SYS.CKSC1CTL.UINT32 = 0xFFFFFFDB;                                                               \
    SYS.CKSC1CTL.UINT32 = 0x00000024;                                                               \
    while(SYS.PROT1PS.UINT32 != 0x00000000)                                                         \
    {                                                                                               \
    }                                                                                               \
                                                                                                    \
    /*  */                                                                                          \
    while((SYS.CLKD0STAT.UINT32 != 0x00000001) || (SYS.CKSC1ACT.UINT32 != 0x00000024))              \
    {                                                                                               \
    }                                                                                               \


#define FCL_INIT_FLASHACCESS                                                                        \
    /* Switch off Flash modification protection*/                                                   \
    FLASHFHVE3  = 1;                                                                                \
    FLASHFHVE15 = 1;                                                                       
                                                                                           
#define FCL_DISABLE_FLASHACCESS                                                                     \
    /* Switch on Flash modification protection*/                                                    \
    FLASHFHVE3  = 0;                                                                                \
    FLASHFHVE15 = 0;


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
