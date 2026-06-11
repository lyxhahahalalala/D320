#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: target.h $
* Mod. Revision : $Revision: 1.2 $
* Mod. Date     : $Date: 2016/05/31 13:16:53JST $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : Target specific defines
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
* Copyright (C) 2015-2016 Renesas Electronics Corporation. All rights reserved.     
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
    #include "dr7f701403.dvf.h"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
    #error include
#elif R_FCL_COMPILER == R_FCL_COMP_REC
    #include "iodefine.h"
#endif


/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define protected_write(preg,pstatus,reg,value) do {                    \
                                                (preg)=0xa5u;           \
                                                (reg)=(value);          \
                                                (reg)=~(value);         \
                                                (reg)=(value);          \
                                                }while((pstatus)==1u)

#define R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK                                                                 	 		\
    /* Prepare MainOSC */                                                                                   						\
    SYSMOSCC=0x05u;                       /* set MainOSC gain (8 MHz) */                                    				\
    SYSMOSCST=0xbb80u;                    /* set MainOSC stabilization time to 6 ms */                      			\
    protected_write(SYSPROTCMD0,SYSPROTS0,SYSMOSCE,0x01u); /* trigger enable (protected write) */     \
    while (0x4u != (SYSMOSCS&0x04u)){PORT_ISO.P42.BIT.P42_15 = 1;PORT_ISO.P1.BIT.P1_4 = 1;}    /* wait for active MainOSC */                                     			\
                                                                                                            								\
    /* Set PLL0 to 480MHz*/                                                                                 						\
    SYSPLL0C=0x6000003B;   /* PLL0 configuration:                                                           				\
                            Fixed frequency mode(dithering disabled)                                        					\
                            Nr = 60                                                                         							\
                            Pr = 1                                                                          								\
                            Mr = 1                                                                          								\
                            fPLL= MainOsc * Nr /(Mr * Pr)                                                   						\
                            fPLL=   8Mhz *  60 /( 1 *  1) = 480 MHz */                                      						\
    protected_write(SYSPROTCMD1,SYSPROTS1,SYSPLL0E,0x01u); /* trigger PLL0 Enable (protected write) */      	\
    while((SYSPLL0S&(1<<2))!=(1<<2)){}   /* wait until PLL0 stable bit is set*/                             				\
                                                                                                            									\
    /* Set PLL1 to 320MHz (PLL1CLK = 320MHz)*/                                                              					\
    SYSPLL1C=0x00000027;   /* PLL1 configuration:                                                           					\
                            Fixed frequency mode(dithering disabled)                                        						\
                            Nr = 40                                                                        									\
                            Pr = 1                                                                          									\
                            Mr = 1                                                                          									\
                            fPLL= MainOsc * Nr /(Mr * Pr)                                                   							\
                            fPLL=   8Mhz *  40 /( 1 *  1) = 320 MHz */                                     						\
    protected_write(SYSPROTCMD1,SYSPROTS1,SYSPLL1E,0x01u);   /* trigger PLL1 Enable (protected write) */    	\
    while((SYSPLL0S&(1<<2))!=(1<<2)){}  /* wait until PLL0 stable bit is set*/                              				\
                                                                                                            									\
    /* set CPU clock divider to 2 */                                                                        							\
    protected_write(SYSPROTCMD1,SYSPROTS1,SYSCKSC_ICPUCLKD_CTL,0x02u); /* divider 2 */                      	\
    while(0x02u != SYSCKSC_ICPUCLKD_ACT){}                                                                  					\
                                                                                                            									\
    /* set CPU clock to PLL1CLK (320MHz)*/                                                                  						\
    protected_write(SYSPROTCMD1,SYSPROTS1,SYSCKSC_ICPUCLKS_CTL ,0x04u); /* PLL1CLK */                    	\
    while(0x04u != SYSCKSC_ICPUCLKS_ACT){}                                                                  					\
                                                                                                            									\
    /* set CLKFIX to PLLFIXCLK/12 (40MHz) */                                                                						\
    protected_write(SYSPROTCMDD1,SYSPROTSD1,SYSCKSC_ICLKFIXS_CTL ,0x01u); /* PLLFIXCLK/12 */      	\
    while(0x01u != SYSCKSC_ICLKFIXS_ACT){}                                                                  					\
                                                                                                            									\
    /* set TAUB0 clock -> CLKFIX (40MHz) */                                                                 						\
    protected_write(SYSPROTCMDD1,SYSPROTSD1,SYSCKSC_ITAUB01S_CTL,0x01u); /* CLKFIX */                 	\
    while(0x01u !=  SYSCKSC_ITAUB01S_ACT){}


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

#define INIT_FLASHACCESS                                                                        \


#define DISABLE_FLASHACCESS                                                                     \
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
