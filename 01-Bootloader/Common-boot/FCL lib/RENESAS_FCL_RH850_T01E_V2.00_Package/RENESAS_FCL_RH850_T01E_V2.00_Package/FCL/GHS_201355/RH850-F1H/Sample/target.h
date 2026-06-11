#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: target.h $
* Mod. Revision : $Revision: 1.1 $
* Mod. Date     : $Date: 2014/04/10 12:45:11MESZ $
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
    #include "dr7f701503_0.h"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #error include
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #include "iodefine.h"
#endif


/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define protected_write(preg, reg, value) preg = 0xa5;    \
                                          reg = value;    \
                                          reg = ~value;   \
                                          reg = value;

#define R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK                                                          \
    MOSCC = 0x05;                           /* Set MainOSC gain (8MHz < MOSC frequency =< 16MHz) */ \
    MOSCST = 0xFFFF;                        /* Set MainOSC stabilization time to max (8,19 ms) */   \
    protected_write(PROTCMD0,MOSCE,0x01);   /* Trigger Enable (protected write) */                  \
    while ((MOSCS & 0x04) != 0x4);          /* Wait for aktive MainOSC */                           \
                                                                                                    \
    /* Prepare PLL0 */                                                                              \
    PLL0C = 0x0000021D;                     /* 16 MHz MainOSC -> 120MHz PLL0 */                     \
    protected_write(PROTCMD1,PLL0E,0x01);   /* enable PLL */                                        \
    while ((PLL0S & 0x04) != 0x04);         /* Wait for aktive PLL */                               \
                                                                                                    \
    /* Prepare PLL1*/                                                                               \
    PLL1C = 0x00000a27;                     /* 16 MHz MainOSC -> 80MHz PLL1 */                      \
    protected_write(PROTCMD1,PLL1E,0x01);   /* enable PLL */                                        \
    while ((PLL1S & 0x04) != 0x04);         /* Wait for aktive PLL */                               \
                                                                                                    \
    /* CPU Clock devider = PLL0/1 */                                                                \
    protected_write(PROTCMD0,CKSC_CPUCLKD_CTL,0x01);                                                \
    while (CKSC_CPUCLKD_ACT != 0x01);                                                               \
                                                                                                    \
    /* PLL0 -> CPU Clock */                                                                         \
    protected_write(PROTCMD1,CKSC_CPUCLKS_CTL,0x03);                                                \
    while (CKSC_CPUCLKS_ACT != 0x03);


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
            for (i = 0; i < 10000; i++)                             \
            {                                                       \
                /* do nothing ... delay time may depend on */       \
                /* external FLMD0 pin connection */                 \
            }

#define FCL_DISABLE_FLASHACCESS                                     \
            uint32_t i;                                             \
                                                                    \
            /* enable FLMD0 */                                      \
            FLMDPCMD = 0xa5;                                        \
            FLMDCNT  = FLMD0_PROTECTION_ON;                         \
            FLMDCNT  = ~FLMD0_PROTECTION_ON;                        \
            FLMDCNT  = FLMD0_PROTECTION_ON;                         \
            for (i = 0; i < 10000; i++)                             \
            {                                                       \
                /* do nothing ... delay time may depend on */       \
                /* external FLMD0 pin connection */                 \
            }

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
