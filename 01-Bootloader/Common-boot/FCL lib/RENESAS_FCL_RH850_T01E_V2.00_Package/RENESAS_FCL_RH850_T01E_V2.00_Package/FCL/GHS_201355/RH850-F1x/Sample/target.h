#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: target.h $
* Mod. Revision : $Revision: 1.8 $
* Mod. Date     : $Date: 2014/07/23 15:33:44MESZ $
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
    #include "dr7f701035_0.h"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #include "ior7f701035xafp.h"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #include "iodefine.h"
#endif


/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK                                                      \
    /* Prepare 8MHz MainOsz */                                                                  \
    MOSCC=0x05;             /* Set MainOSC gain (8MHz < MOSC frequency =< 16MHz) */             \
    MOSCST=0xFFFF;          /* Set MainOSC stabilization time to max (8,19 ms) */               \
                                                                                                \
    PROTCMD0=0xa5;          /* Trigger Enable (protected write) */                              \
    MOSCE=0x01;                                                                                 \
    MOSCE=(~0x01);                                                                              \
    MOSCE=0x01;                                                                                 \
    while (MOSCS != 0x7);   /* Wait for stable MainOSC */                                       \
                                                                                                \
    /* Prepare PLL*/                                                                            \
    PLLC=0x00000a27;        /*16 MHz MainOSC -> 80MHz PLL */                                    \
    PROTCMD1=0xa5;          /* enable PLL (protected write) */                                  \
    PLLE=0x01;                                                                                  \
    PLLE=(~0x01);                                                                               \
    PLLE=0x01;                                                                                  \
    while(PLLS!=0x07);     /*Wait for stable PLL */                                             \
                                                                                                \
    /* CPU Clock devider = 1 */                                                                 \
    PROTCMD1=0xa5;                                                                              \
    CKSC_CPUCLKD_CTL=0x01;                                                                      \
    CKSC_CPUCLKD_CTL=(~0x01);                                                                   \
    CKSC_CPUCLKD_CTL=0x01;                                                                      \
    while(CKSC_CPUCLKD_ACT!=0x01);                                                              \
                                                                                                \
    /* PLL -> CPU Clock */                                                                      \
    PROTCMD1=0xa5;                                                                              \
    CKSC_CPUCLKS_CTL=0x03;                                                                      \
    CKSC_CPUCLKS_CTL=(~0x03);                                                                   \
    CKSC_CPUCLKS_CTL=0x03;                                                                      \
    while(CKSC_CPUCLKS_ACT!=0x03);


#define FLMD0_PROTECTION_OFF    (0x01u)
#define FLMD0_PROTECTION_ON     (0x00u)

#define FCL_INIT_FLASHACCESS                                        \
            volatile uint32_t i;                                    \
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
            }                                                       
            
#define FCL_DISABLE_FLASHACCESS                                     \
            volatile uint32_t i;                                    \
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
            }                                                       

/* F1L WS2.0 needs Code Flash authentication prior to switch mode */
#define R_FCL_DEVICE_SPECIFIC_INIT                                  \
            *(volatile uint32_t *)0xFFA08000 = 0xFFFFFFFF;          \
            *(volatile uint32_t *)0xFFA08004 = 0xFFFFFFFF;          \
            *(volatile uint32_t *)0xFFA08008 = 0xFFFFFFFF;          \
            *(volatile uint32_t *)0xFFA0800C = 0xFFFFFFFF;

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
