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
	MainOsc,
	SubOsc,
	IntOsc,
	PLL0,
	PLL1,
	IPLL0,
	IPLL1,
}CLK_Source;

typedef enum
{
	STOP_IN_DPSP,		//stop in deepstop mod
	RUN_IN_DPSP,		//do not stop in deep stop mod
}DPSP_MOD;

typedef enum
{
	NOT_USE,
	Osc_start,
	Osc_Stop,
}OscEn_CTL;

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
  Constant: PROTCMD0
  
  Address of the AWO protection register
  
*/

#define	MOSCE  		0xFFF81100
#define	MOSCS  		0xFFF81104
#define	MOSCC	  	0xFFF81108
#define	MOSCST  	0xFFF8110C
#define	MOSCSTPM	0xFFF81118

#define	SOSCE		0xFFF81200
#define	SOSCS		0xFFF81204
#define	SOSCST		0xFFF8120C

#define	ROSCE		0xFFF81000
#define	ROSCS		0xFFF81004
#define	ROSCSTPM	0xFFF81018

#define 	PLL0E		0xFFF89000
#define 	PLL0S		0xFFF89004
#define 	PLL0C		0xFFF89008

#define	PLL1E		0xFFF89100
#define	PLL1S		0xFFF89104
#define	PLL1C		0xFFF89108

#define	IPLL0S		0xFFF89200
#define	IPLL0S_ACT	0xFFF89208

#define	IPLL1S		0xFFF89300
#define	IPLL1S_ACT	0xFFF89308

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


#define PROTCMD0     (0xfff80000u)

#define PROTS0 (0xfff80004)

/*******************************************************************************
  Constant: PROTCMD1
  
  Address of the ISO protection register
  
*/

#define PROTCMD1     (0xfff88000u)

#define PROTS1	(0xfff88004)
/*******************************************************************************
  Constant: PROTCMDD1
  
  Address of the another ISO protection cluster
  
*/

#define PROTCMDD1     (0xfff87000u)

#define PROTSD1		(0xfff87004)
/*******************************************************************************
  Constant: PBUS_RATIO
  
  Address of the PBUS CLOCK ratio setting register
  Old header: 0xfff8f504 UM: 0xfff8f510u
*/
#define PBUS_RATIO    (0xfff8f510u)

/*******************************************************************************
  Constant: PROTCMDPWRGD
  
  Address of the PBUS CLOCK ratio protection register
  
*/
#define PROTCMDPWRGD    (0xfff8f580u)


#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE*)(ADDR)) =(uint##SIZE) (VALUE)); }
#define R_DEV_READ_REG(SIZE, ADDR)   (*((volatile uint##SIZE*)(ADDR)))

#define R_DEV_WRITE_PROT_REG(PROT_ADDR, SEL_ADDR, VALUE)	{*((volatile uint32*)(PROT_ADDR))=0x000000A5;\
																	*((volatile uint32*)(SEL_ADDR))=VALUE;\
																	*((volatile uint32*)(SEL_ADDR))=~VALUE;\
																	*((volatile uint32*)(SEL_ADDR))=VALUE;}

#define MODULE_RST_RELEASE	  protected_write(SYSPROTCMDMRST,SYSPROTSMRST,SYSMRSTC,(0x00003fffu))

#endif
