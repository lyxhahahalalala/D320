/*
****************************************************************************
PROJECT : device driver for V-Lib
FILE    : $Id: r_dev_xorchk.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
r7f701412 xor check unit functionality
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2014
                                  by 
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.

****************************************************************************
*/


/*******************************************************************************
  Title: r7f701412 interrupt serivicing 

  
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"


/* The debug output for register accesses shall be only active 
on special request, otherwise the output might get "overloaded"  
If you want to see all register accesses, activate R_DBG_SHOW_INT_REGS
in your application's Makefile */
#ifndef R_DBG_SHOW_INT_REGS
  #undef  R_DBG_PRINT_DEV_REGS
#endif   
#include "r_dev_api.h"

/*******************************************************************************
  Section: Local Constants 
*/

/* register addresses of XORCHK unit */
#define LOC_PRMR0CFG0               0xFFC06000u
#define LOC_PRMR0CFG1               0xFFC06004u
#define LOC_PRMR0CFG2               0xFFC06008u

/* register addresses of DNFA (dig. noise filter) used for Xor check unit */
#define LOC_DNFA9CTL                0xFFC30900u
#define LOC_DNFA9EN                 0xFFC30904u
#define LOC_DNFA10CTL               0xFFC30A00u
#define LOC_DNFA10EN                0xFFC30A04u

/*******************************************************************************
  Section: Local types 
*/

/*******************************************************************************
  Constant: loc_XorchkFunc_XorCtlReg
  
  Stores the PRMR0CFGn register address,
  that relates to the Xor check unit function.

*/
static const uint32_t loc_XorchkFunc_XorCtlReg[R_DEV_XORCHK_FUNC_LAST] =
{
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB0O1       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB0O3       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB0O5       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB0O7       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB0O9       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB0O11      */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB0O13      */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB1O1       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB1O3       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB1O5       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB1O7       */
    LOC_PRMR0CFG0,   /* R_XORCHK_FUNC_TAUB1O9       */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0AP0      */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0AN0      */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0BP0      */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0BN0      */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0AP1      */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0AN1      */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0BP1      */
	LOC_PRMR0CFG1,   /* R_XORCHK_FUNC_PCMP0BN1      */
	LOC_PRMR0CFG2,   /* R_XORCHK_FUNC_SG0AO         */
	LOC_PRMR0CFG2,   /* R_XORCHK_FUNC_SG0FAOL       */
	LOC_PRMR0CFG2    /* R_XORCHK_FUNC_SG0FAO        */
};

/*******************************************************************************
  Constant: loc_XorchkFunc_XorCtlRegBit
  
  Stores the PRMR0CFGn registers control bit number,
  that relates to the Xor check unit function.
  example:
   o PRMR0XDBE01 -> bit 0 (TAUB0O1)
   o PRMR0XDBE03 -> bit 1 (TAUB0O3)
   o PRMR0XDBE05 -> bit 2 (TAUB0O5)

*/
static const uint32_t loc_XorchkFunc_XorCtlRegBit[R_DEV_XORCHK_FUNC_LAST] =
{
    0u,    /* R_XORCHK_FUNC_TAUB0O1       */
    1u,    /* R_XORCHK_FUNC_TAUB0O3       */
    2u,    /* R_XORCHK_FUNC_TAUB0O5       */
    3u,    /* R_XORCHK_FUNC_TAUB0O7       */
    4u,    /* R_XORCHK_FUNC_TAUB0O9       */
    5u,    /* R_XORCHK_FUNC_TAUB0O11      */
    6u,    /* R_XORCHK_FUNC_TAUB0O13      */
          /* bit 7 not used */
    8u,    /* R_XORCHK_FUNC_TAUB1O1       */
    9u,    /* R_XORCHK_FUNC_TAUB1O3       */
    10u,   /* R_XORCHK_FUNC_TAUB1O5       */
    11u,   /* R_XORCHK_FUNC_TAUB1O7       */
    12u,   /* R_XORCHK_FUNC_TAUB1O9       */

	5u,    /* R_XORCHK_FUNC_PCMP0AP0      */
	4u,    /* R_XORCHK_FUNC_PCMP0AN0      */
	1u,    /* R_XORCHK_FUNC_PCMP0BP0      */
	6u,    /* R_XORCHK_FUNC_PCMP0BN0      */
	3u,    /* R_XORCHK_FUNC_PCMP0AP1      */
	0u,    /* R_XORCHK_FUNC_PCMP0AN1      */
	2u,    /* R_XORCHK_FUNC_PCMP0BP1      */
	7u,    /* R_XORCHK_FUNC_PCMP0BN1      */

	2u,    /* R_XORCHK_FUNC_SG0AO         */
	1u,    /* R_XORCHK_FUNC_SG0FAOL       */
	0u     /* R_XORCHK_FUNC_SG0FAO        */
};

/*******************************************************************************
  Constant: loc_XorchkFunc_DnfCtlReg
  
  Stores the DNFAnCTL register address,
  that relates to the Xor check unit function.

*/
static const uint32_t loc_XorchkFunc_DnfCtlReg[R_DEV_XORCHK_FUNC_LAST] =
{
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB0O1       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB0O3       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB0O5       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB0O7       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB0O9       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB0O11      */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB0O13      */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB1O1       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB1O3       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB1O5       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB1O7       */
    LOC_DNFA9CTL,    /* R_XORCHK_FUNC_TAUB1O9       */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0AP0      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0AN0      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0BP0      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0BN0      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0AP1      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0AN1      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0BP1      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_PCMP0BN1      */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_SG0AO         */
	LOC_DNFA10CTL,   /* R_XORCHK_FUNC_SG0FAOL       */
	LOC_DNFA10CTL    /* R_XORCHK_FUNC_SG0FAO        */
};

/*******************************************************************************
  Constant: loc_XorchkFunc_DnfEnReg
  
  Stores the DNFAnEN register address,
  that relates to the Xor check unit function.

*/
static const uint32_t loc_XorchkFunc_DnfEnReg[R_DEV_XORCHK_FUNC_LAST] =
{
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB0O1       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB0O3       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB0O5       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB0O7       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB0O9       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB0O11      */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB0O13      */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB1O1       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB1O3       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB1O5       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB1O7       */
    LOC_DNFA9EN,    /* R_XORCHK_FUNC_TAUB1O9       */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0AP0      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0AN0      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0BP0      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0BN0      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0AP1      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0AN1      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0BP1      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_PCMP0BN1      */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_SG0AO         */
	LOC_DNFA10EN,   /* R_XORCHK_FUNC_SG0FAOL       */
	LOC_DNFA10EN    /* R_XORCHK_FUNC_SG0FAO        */
};

/*******************************************************************************
  Constant: loc_XorchkFunc_DnfEnRegBit
  
  Stores the DNFAnEN registers control bit number,
  that relates to the Xor check unit function.
  example:
   o DNFAnNFENL0 -> bit 0
   o DNFAnNFENL7 -> bit 7
   o DNFAnNFENH0 -> bit 8

*/
static const uint32_t loc_XorchkFunc_DnfEnRegBit[R_DEV_XORCHK_FUNC_LAST] =
{
    0u,    /* R_XORCHK_FUNC_TAUB0O1       */
    1u,    /* R_XORCHK_FUNC_TAUB0O3       */
    2u,    /* R_XORCHK_FUNC_TAUB0O5       */
    3u,    /* R_XORCHK_FUNC_TAUB0O7       */
    4u,    /* R_XORCHK_FUNC_TAUB0O9       */
    5u,    /* R_XORCHK_FUNC_TAUB0O11      */
    6u,    /* R_XORCHK_FUNC_TAUB0O13      */
          /* bit 7 not used */
    8u,    /* R_XORCHK_FUNC_TAUB1O1       */
    9u,    /* R_XORCHK_FUNC_TAUB1O3       */
    10u,   /* R_XORCHK_FUNC_TAUB1O5       */
    11u,   /* R_XORCHK_FUNC_TAUB1O7       */
    12u,   /* R_XORCHK_FUNC_TAUB1O9       */

	0u,    /* R_XORCHK_FUNC_PCMP0AP0      */
	1u,    /* R_XORCHK_FUNC_PCMP0AN0      */
	2u,    /* R_XORCHK_FUNC_PCMP0BP0      */
	3u,    /* R_XORCHK_FUNC_PCMP0BN0      */
	4u,    /* R_XORCHK_FUNC_PCMP0AP1      */
	5u,    /* R_XORCHK_FUNC_PCMP0AN1      */
	6u,    /* R_XORCHK_FUNC_PCMP0BP1      */
	7u,    /* R_XORCHK_FUNC_PCMP0BN1      */

	8u,    /* R_XORCHK_FUNC_SG0AO         */
	8u,    /* R_XORCHK_FUNC_SG0FAOL  Note: all 3 SG functions share same DNF (DNF10.8) */
	8u     /* R_XORCHK_FUNC_SG0FAO   Note: all 3 SG functions share same DNF (DNF10.8) */
};


/*******************************************************************************
  Section: Local functions
*/

/*******************************************************************************
  Section: Global functions
*/


/*******************************************************************************
  Function: R_DEV_Xorchk_Config
  
  See: <r_dev_api.h> for details 
*/

void R_DEV_Xorchk_Config(r_dev_XorchkFunc_t Func, r_dev_xorchk_Parameter_t *Config) 
{
    uint32_t regval8 = 0;

    /* DNF detailed setup (filter config) */
    /*  set sample number before requection (2 .. 5) */
    if ((Config->SampleRejectAbility >= 2)
        && (Config->SampleRejectAbility <= 5))
    {
        /* nothing */
    } else
    {
        /* invalid value for "SampleRejectAbility"
            -> use default value 2 */
        Config->SampleRejectAbility = 2;
    }
    regval8 |= (((Config->SampleRejectAbility) - 2) << 5);
        
    /*  set sample clock prescaler (0 .. 7)
         -> equals 2^SampleClockPrescaler */
    if (Config->SampleClockPrescaler > 0x7)
    {
        /* invalid value for "SampleClockPrescaler"
            -> use default value 0 (equals 2^0 = 1) */
        Config->SampleClockPrescaler = 0;
    }
    regval8 |= ((Config->SampleClockPrescaler) << 0);
    
    /* write the value to DNFAnCTL register */
    R_DEV_WRITE_REG(8, loc_XorchkFunc_DnfCtlReg[Func], regval8);
}


/*******************************************************************************
  Function: R_DEV_Xorchk_FunctionCheckEnable
  
  See: <r_dev_api.h> for details 
*/

void R_DEV_Xorchk_FunctionCheckEnable(r_dev_XorchkFunc_t Func, uint8_t Enable)
{
    uint16_t regval16 = 0 ;
    uint32_t regval32 = 0 ;
    
    /* DNF setup (only enable / disable) */
    /* first read the actual value of DNFAnEN register */
    regval16 = R_DEV_READ_REG(16, loc_XorchkFunc_DnfEnReg[Func]);
    
    /* clear DNFA enable bit to disable XOR check unit */
    if (R_FALSE == Enable)
    {
        regval16 &= (uint16_t)(~(1uL << loc_XorchkFunc_DnfEnRegBit[Func]));
    } 
    /* set DNFA enable bit to enable XOR check unit */
    else {
        regval16 |= (uint16_t)(1uL << loc_XorchkFunc_DnfEnRegBit[Func]);
    }
    /* write the new value to DNFAnEN register */
    R_DEV_WRITE_REG(16, loc_XorchkFunc_DnfEnReg[Func], regval16);
    
    /* PRMR0CFGn setup */
    /* first read the actual value of PRMR0CFGn register */
    regval32 = R_DEV_READ_REG(32, loc_XorchkFunc_XorCtlReg[Func]); 
    
    /* clear Ctrl enable bit to disable XOR check unit */
    if (R_FALSE == Enable)
    {
        regval32 &= (uint32_t)~(1uL << loc_XorchkFunc_XorCtlRegBit[Func]); 
    } 
    /* Set Ctrl enable bit to enable XOR check unit */
    else {
        regval32 |= (uint32_t)(1uL << loc_XorchkFunc_XorCtlRegBit[Func]);
    }
    /* write the new value to PRMR0CFGn register */
    R_DEV_WRITE_REG(32, loc_XorchkFunc_XorCtlReg[Func], regval32); 
}

