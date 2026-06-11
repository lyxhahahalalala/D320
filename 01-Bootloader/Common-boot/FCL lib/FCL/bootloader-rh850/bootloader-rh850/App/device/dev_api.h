/*****************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*****************************************************************************/
/*******************************************************************************
* File Name : dev_api.h
* Description : This file is for port define.
******************************************************************************/

#ifndef _DEV_API_H_

#define _DEV_API_H_



typedef enum
{
	R_PARAMETER_ERROR,
	R_OK,
}r_Error_t;


typedef enum
{
	CPU_EMCLK = 1,
	CPU_PLL0    = 3,
	CPU_PLL1    = 4,
}CPU_ClkSel;

typedef enum
{
	CPU_Clk_1 = 1,		//CPU_ClkSel	
	CPU_Clk_2,			//CPU_ClkSel/2
	CPU_Clk_4,			//CPU_ClkSel/4
	CPU_Clk_8,			//CPU_ClkSel/8
	CPU_Clk_3,			//CPU_ClkSel/3
}CPU_ClkDiv;

typedef enum
{
	FIX_ClkDivSel = 1,	//CKDV_CLKFIXD_CTL selection
	FIX_HS_IntOsc,		//high speed intOSC
}FIX_ClkSel;

typedef enum
{
	FIX_Disable		= 0,		//CLKFIX disable
	FIX_PLL1CLK_6	= 6,		//PLL1CLK/6
	FIX_PLL1CLK_8	= 8,		//PLL1CLK/8
	FIX_PLL1CLK_10	= 10,	//PLL1CLK/10
	FIX_PLL1CLK_12	= 12,	//PLL1CLK/12
}FIX_ClkDiv;

typedef enum
{
	JIT_ClkDivSel = 1,	//CKDV_CLKJITD_CTL selection
	JIT_HS_IntOsc,		//high speed intOsc
}JIT_ClkSel;

typedef enum
{
	JIT_Disable		= 0,		//CLKJIT disable
	JIT_PLL0CLK_4	= 4,		//PLL0CLK/4
	JIT_PLL0CLK_5	= 5,		//PLL0CLK/5
	JIT_PLL0CLK_6	= 6,		//PLL0CLK/6
	JIT_PLL0CLK_8	= 8,		//PLL0CLK/8
	JIT_PLL0CLK_10	= 10,	//PLL0CLK/10
	JIT_PLL0CLK_12	= 12,	//PLL0CLK/12
}JIT_ClkDiv;
	
/*******************************************************************************
  Constant: LOC_PROTCMD0
  
  Address of the AWO protection register
  
*/

#define	MOSCE  		0xFFF81100
#define	MOSCS  		0xFFF81104
#define	MOSCC	  	0xFFF81108
#define	MOSCST  	0xFFF8110C

#define 	PLL0E		0xFFF89000
#define 	PLL0S		0xFFF89004
#define 	PLL0C		0xFFF89008

#define	PLL1E		0xFFF89100
#define	PLL1S		0xFFF89104
#define	PLL1C		0xFFF89108

#define	CPUCLKS		0xFFF8A000
#define	CPUCLKS_ACT	0xFFF8A008
#define	CPUCLKD		0xFFF8A100
#define	CPUCLKD_ACT	0xFFF8A108

#define	FIXCLKD			0xFFF85700
#define	FIXCLKD_STAT	0xFFF85704
#define	FIXCLKS			0xFFF85740
#define	FIXCLKS_ACT	0xFFF85748

#define	JITCLKD			0xFFF853C0
#define	JITCLKD_STAT	0xFFF853C4
#define	JITCLKS			0xFFF85400
#define	JITCLKS_ACT	0xFFF85408


#define LOC_PROTCMD0     (0xfff80000u)

#define LOC_PROTS0 (0xfff80004)

/*******************************************************************************
  Constant: LOC_PROTCMD1
  
  Address of the ISO protection register
  
*/

#define LOC_PROTCMD1     (0xfff88000u)

#define LOC_PROTS1	(0xfff88004)
/*******************************************************************************
  Constant: LOC_PROTCMDD1
  
  Address of the another ISO protection cluster
  
*/

#define LOC_PROTCMDD1     (0xfff87000u)

#define LOC_PROTSD1		(0xfff87004)
/*******************************************************************************
  Constant: LOC_PBUS_RATIO
  
  Address of the PBUS CLOCK ratio setting register
  Old header: 0xfff8f504 UM: 0xfff8f510u
*/
#define LOC_PBUS_RATIO    (0xfff8f510u)

/*******************************************************************************
  Constant: LOC_PROTCMDPWRGD
  
  Address of the PBUS CLOCK ratio protection register
  
*/
#define LOC_PROTCMDPWRGD    (0xfff8f580u)


#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE*)(ADDR)) =(uint##SIZE) (VALUE)); }
#define R_DEV_READ_REG(SIZE, ADDR)   (*((volatile uint##SIZE*)(ADDR)))

#define R_DEV_WRITE_PROT_REG(PROT_ADDR, SEL_ADDR, VALUE)	{*((volatile uint32*)(PROT_ADDR))=0x000000A5;\
																	*((volatile uint32*)(SEL_ADDR))=VALUE;\
																	*((volatile uint32*)(SEL_ADDR))=~VALUE;\
																	*((volatile uint32*)(SEL_ADDR))=VALUE;}

#endif
