/*******************************************************************************
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
* (c) 2014 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/


/******************************************************************************
* File Name     :clock.c
* Version       : 1.0
* Device(s)     : R7F701403xAFP RH850/D1L
* Tool-Chain    : GHS
* Description   : This file is a sample of the clock setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 30.09.2016 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include    "Device.h"
#include	"Dev_api.h"
#include  <stdio.h>

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define MAINOSC_8_16	0x04		//8M<mainosc<=16M
#define MAINOSC_8		0x05		//8M
#define MAINOSC_L8		0x06		//8M low power mode
#define MAINOSC_VL8		0x07		//8M very low power mode


/*******************************************************************************
  Type: loc_PllCfgReg_t
  
  Contents of the PLL clock control/configuration register
  
  Members:
  NI                - Integer part of Divider Nr
  P                 - Pr
  M                 - Mr
  SSMode            - Spread Spectrum Mode
  NF                - real part of Nr
  SSPerc            - Spread Spectrum Percentage
  SSFreq            - Spread Spectrum middle frequency
  FVV               - VCO output frequency range
*/

typedef union {
	struct
	{
		uint32  NI        : 7;
		uint32  P         : 3;
		uint32  M         : 2;
		uint32  SSMode    : 2;
		uint32  NF        : 4;
		uint32  SSPerc    : 3;
		uint32  SMFreq    : 5;
		uint32  FVV       : 3;
	}PllCfgReg_Bits;
	uint32 PllCfgReg;
} loc_PllCfgReg_t;


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
r_Error_t Clock_Init( void );

/******************************************************************************
Private global variables and functions
******************************************************************************/
static  r_Error_t osc_clock(uint32 FrqIn, uint32 Pll0Out, uint32 Pll1Out );
static  void    set_clock_domain( CPU_ClkDiv CpuDiv, CPU_ClkSel CpuSel, FIX_ClkDiv FixDiv, FIX_ClkSel FixSel, JIT_ClkDiv JitDiv, JIT_ClkSel JitSel );
void Clock_Control(CLK_Source Clk,OscEn_CTL type);
void OSC_Mode_set(CLK_Source Clk, DPSP_MOD mod);


/******************************************************************************
* Function Name : void R_CLOCK_Init( void )
* Description   : This function initializes the clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
r_Error_t Clock_Init( void )
{
	r_Error_t retVal;
    	retVal = osc_clock(8,480,480);                                /* Clock oscillation         */
	set_clock_domain(CPU_Clk_2, CPU_PLL1, FIX_PLL1CLK_12, FIX_ClkDivSel, JIT_PLL0CLK_6, JIT_ClkDivSel);          /* Domain clock setting      */
	return retVal;
}
       

/******************************************************************************
* Function Name : static void osc_clock( void )
* Description   : This function oscillates the clock (main , sub and PLL clock).
* Argument     : 
			:FrqIn	The frequency input
			:Pll0Out	PLL0 frequency output expected
			:Pll1Out	PLL1 frequency output expected
* Return Value  : none
******************************************************************************/
static r_Error_t osc_clock(uint32 FrqIn, uint32 Pll0Out, uint32 Pll1Out)
{
	uint32 i = 0;
	uint32 p_r    = 0;
	uint32 m_r    = 0;
	uint32 n_r    = 0;
	uint32 vc_out = 0;
	uint32 cfgFrqIn;
	uint32 pll0freq;
	uint32 pll1freq;
	r_Error_t retVal = R_OK;
	loc_PllCfgReg_t Pll0CfgReg;
	loc_PllCfgReg_t Pll1CfgReg;

	Pll0CfgReg.PllCfgReg = 0x0;
	Pll1CfgReg.PllCfgReg = 0x0;
	
	if((FrqIn>8)&&(FrqIn<=16))
		cfgFrqIn = 0x04;
	else if(FrqIn==8)
		cfgFrqIn = 0x05;
	else
	{
		retVal = R_PARAMETER_ERROR;
		printf("%s","Parameter error !!!");	
	}
	R_DEV_WRITE_REG(32, MOSCC, cfgFrqIn);		 /* set MainOSC gain (8 MHz) : SYSMOSCC*/
	R_DEV_WRITE_REG(32, MOSCST, 0x8000);		/* set MainOSC stabilization time to 4 ms : SYSMOSCST*/

	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD0, MOSCE, 0x01u);	/*SYSMOSCE*/
	}while(R_DEV_READ_REG(32, PROTS0)==1);			/* trigger enable (protected write) */
	while(0x4u!=(R_DEV_READ_REG(32, MOSCS)&0x04))		/* wait for active MainOSC: SYSMOSCS */


/*	PLL0 output frequency configuration	*/
	pll0freq = Pll0Out;

	if (pll0freq > 480u)
	{
		/* the desired frequency is not valid */
		printf("%s","Parameter error !!!");
		return R_PARAMETER_ERROR;
	}
	else if (pll0freq > 320u)
	{
	 	p_r = 1u;
		Pll0CfgReg.PllCfgReg_Bits.P= 0;
	}
	else if (pll0freq > 160u)
	{
		p_r = 2;
		Pll0CfgReg.PllCfgReg_Bits.P = 1u;
	}
	else if (pll0freq > 80u)
	{
		p_r = 4u;
		Pll0CfgReg.PllCfgReg_Bits.P = 2u;
	}
	else if (pll0freq > 40u)
	{
		p_r = 8;
		Pll0CfgReg.PllCfgReg_Bits.P = 3u;
	}
	else if (pll0freq > 20u)
	{
		p_r = 16u;
		Pll0CfgReg.PllCfgReg_Bits.P = 4u;
	}
	else
	{
		/* the desired frequency is not valid */
		printf("%s","Parameter error !!!");
		return R_PARAMETER_ERROR;
	}

	for(m_r=1;m_r<4;m_r++)
	{
		n_r = pll0freq*m_r*p_r/FrqIn;
		if((n_r>11)&&(n_r<81))
		{
			vc_out = pll0freq*p_r;
			Pll0CfgReg.PllCfgReg_Bits.M = (m_r-1);
			Pll0CfgReg.PllCfgReg_Bits.NI = (n_r-1);
			while((320+i*40u)<vc_out)
			{
				i++;
			}
			if(i<9)
				Pll0CfgReg.PllCfgReg_Bits.FVV = i-1;
			else
				Pll0CfgReg.PllCfgReg_Bits.FVV = 3;	//init value
			break;
		}
	}
	if(m_r==4)
	{
		printf("%s","Parameter error !!!");
		retVal = R_PARAMETER_ERROR;
	}

	R_DEV_WRITE_REG(32, PLL0C, Pll0CfgReg.PllCfgReg); 	/* PLL0 configuration: SYSPLL0C*/

	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD1, PLL0E, 0x01);		/*SYSPLL0E*/
	}while(R_DEV_READ_REG(32,PROTS1)==1);		 /* trigger PLL0 enable (protected write) */
	while(0x04!=(R_DEV_READ_REG(32, PLL0S)&0x04)){}	 /* wait until PLL0 stable bit is set: SYSPLL0S */


/*	PLL1 output frequency configuration	*/
	pll1freq = Pll1Out;

	if (pll1freq > 480u)
	{
		/* the desired frequency is not valid */
		printf("%s","Parameter error !!!");
		return R_PARAMETER_ERROR;
	}
	else if (pll1freq > 240u)
	{
	 	p_r = 1u;
		Pll1CfgReg.PllCfgReg_Bits.P = 0;
	}
	else if (pll1freq > 120u)
	{
		p_r = 2;
		Pll1CfgReg.PllCfgReg_Bits.P = 1u;
	}
	else if (pll1freq > 60u)
	{
		p_r = 4u;
		Pll1CfgReg.PllCfgReg_Bits.P = 2u;
	}
	else if (pll1freq > 40u)
	{
		p_r = 6;
		Pll1CfgReg.PllCfgReg_Bits.P = 3u;
	}
	else if (pll1freq > 30u)
	{
		p_r = 8;
		Pll1CfgReg.PllCfgReg_Bits.P = 4u;
	}
	else if (pll1freq > 20u)
	{
		p_r = 16u;
		Pll1CfgReg.PllCfgReg_Bits.P = 5u;
	}
	else
	{
		/* the desired frequency is not valid */
		printf("%s","Parameter error !!!");
		return R_PARAMETER_ERROR;
	}

	for(m_r=1;m_r<4;m_r++)
	{
		n_r = pll1freq*m_r*p_r/FrqIn;
		if((n_r>11)&&(n_r<81)&&(n_r%2==0))
		{
			vc_out = pll1freq*p_r;
			Pll1CfgReg.PllCfgReg_Bits.M = (m_r-1);
			Pll1CfgReg.PllCfgReg_Bits.NI = (n_r-1);
			break;
		}
	}
	if(m_r==4)
	{
		printf("%s","Parameter error !!!");
		retVal = R_PARAMETER_ERROR;
	}

	R_DEV_WRITE_REG(32, PLL1C, Pll1CfgReg.PllCfgReg); 	/* PLL1 configuration:SYSPLL1C*/
	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD1, PLL1E, 0x01);		/*SYSPLL1E*/
	}while(R_DEV_READ_REG(32,PROTS1)==1);		 /* trigger PLL1 enable (protected write) */
	while(0x04!=(R_DEV_READ_REG(32, PLL1S)&0x04)){}	 /* wait until PLL0 stable bit is set: SYSPLL1S*/
	
	return retVal;
 }


/******************************************************************************
* Function Name : static void set_clock_domain( void )
* Description   : This function sets the domain clock.
* Argument      : 
			:CpuDiv	CPU clock divider select
			:CpuSel	CPU clock source select
			:FixDiv	FIXCLK divider select
			:FixSel	FIXCLK source select
			:JitDiv	JITCLK divider select
			:JitSel	JITCLK source select
* Return Value  : none
******************************************************************************/
static void set_clock_domain( CPU_ClkDiv CpuDiv, CPU_ClkSel CpuSel, FIX_ClkDiv FixDiv, FIX_ClkSel FixSel, JIT_ClkDiv JitDiv, JIT_ClkSel JitSel)
{
    /* set CPU clock divider to 4 */
 	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD1, CPUCLKD, CpuDiv);
	}while(R_DEV_READ_REG(32,PROTS1)==1);
	while(CpuDiv!=R_DEV_READ_REG(32, CPUCLKD_ACT));

    /* set CPU clock to PLL0CLK */
	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD1, 0xFFF8A000, CpuSel);
	}while(R_DEV_READ_REG(32,PROTS1)==1);
	while(0x0==R_DEV_READ_REG(32, CPUCLKS_ACT));
	
	/* set clock divider for CLKFIX to 6 (480MHz / 6 = 80 MHz)*/
 	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD1, FIXCLKD, FixDiv);
	}while(R_DEV_READ_REG(32,PROTS1)==1);
	while(0x03!=R_DEV_READ_REG(32, FIXCLKD_STAT));
  
	/* set CLKFIX to PLLFIXCLK / 6 (80 MHz) */
 	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD1, FIXCLKS, FixSel);
	}while(R_DEV_READ_REG(32,PROTS1)==1);
	while(0x0==R_DEV_READ_REG(32, FIXCLKS_ACT));

  
	/* set CLKJIT divider to PLL0 / 6 (80 MHz) */
 	do
	{
		R_DEV_WRITE_PROT_REG(PROTCMD1, JITCLKD, JitDiv);
	}while(R_DEV_READ_REG(32,PROTS1)==1);
	while(0x03!=R_DEV_READ_REG(32, JITCLKD_STAT));
  
	/* set CLKJIT to CKDV_ICLKJITD_CTL (80 MHz) */

}

/******************************************************************************
* Function Name : static void Clock_Control( void )
* Description   : This function starts or stops the select clock source.
* Argument      : 
				Clk	:	the clock source selected
				type	:	the action selected
* Return Value  : none
******************************************************************************/
void Clock_Control(CLK_Source Clk,OscEn_CTL type)
{
	switch(Clk)
	{
		case MainOsc:
			do
			{
				R_DEV_WRITE_PROT_REG(PROTCMD0, MOSCE, type);	/*SYSMOSCE*/
			}while(R_DEV_READ_REG(32, PROTS0)==1);			/* trigger enable (protected write) */
			while((R_DEV_READ_REG(32, MOSCS)&0x04)!=(((type==Osc_start)?1:0)<<2))		/* wait for active MainOSC: SYSMOSCS */
			break;
		case SubOsc:
			do
			{
				R_DEV_WRITE_PROT_REG(PROTCMD0, SOSCE, type);	/*SYSSOSCE*/
			}while(R_DEV_READ_REG(32, PROTS0)==1);			/* trigger enable (protected write) */
			while((R_DEV_READ_REG(32, SOSCS)&0x04)!=(((type==Osc_start)?1:0)<<2))		/* wait for active SubOSC: SYSSOSCS */
			break;
		case IntOsc:
			if(type == Osc_Stop)
			{
				do
				{
					R_DEV_WRITE_PROT_REG(PROTCMD0, ROSCE, type);	/*SYSROSCE*/
				}while(R_DEV_READ_REG(32, PROTS0)==1);			/* trigger enable (protected write) */
				while(R_DEV_READ_REG(32, ROSCS)&0x04);		/* wait for active IntOSC: SYSROSCS */
			}
			break;
		case PLL0:
			do
			{
				R_DEV_WRITE_PROT_REG(PROTCMD0, PLL0E, type);	/*SYSPLL0E*/
			}while(R_DEV_READ_REG(32, PROTS0)==1);			/* trigger enable (protected write) */
			while((R_DEV_READ_REG(32, PLL0S)&0x04)!=(((type==Osc_start)?1:0)<<2))		/* wait for active PLL0: SYSPLL0S */
			break;
		case PLL1:
			do
			{
				R_DEV_WRITE_PROT_REG(PROTCMD0, MOSCE, type);	/*SYSPLL1E*/
			}while(R_DEV_READ_REG(32, PROTS0)==1);			/* trigger enable (protected write) */
			while((R_DEV_READ_REG(32, MOSCS)&0x04)!=(((type==Osc_start)?1:0)<<2))		/* wait for active PLL1: SYSPLL1S */
			break;
		case IPLL0:
			do
			{
				R_DEV_WRITE_PROT_REG(PROTCMD1, IPLL0S, (type==Osc_start?1:0));	/*SYSPLL1E*/
			}while(R_DEV_READ_REG(32, PROTS1)==1);			/* trigger enable (protected write) */
			while(R_DEV_READ_REG(32, MOSCS)!=((type==Osc_start)?1:0))		/* wait for active PLL1: SYSPLL1S */
			break;
		case IPLL1:
			do
			{
				R_DEV_WRITE_PROT_REG(PROTCMD1, IPLL1, (type==Osc_start?1:0));	/*SYSPLL1E*/
			}while(R_DEV_READ_REG(32, PROTS1)==1);			/* trigger enable (protected write) */
			while(R_DEV_READ_REG(32, MOSCS)!=((type==Osc_start)?1:0))		/* wait for active PLL1: SYSPLL1S */
			break;
		default:
			printf("%s","paremeter error !!!");
			break;
	}
}


/******************************************************************************
* Function Name : static void assist_osc_set( void )
* Description   : This function sets the clock selected stop or operation in deepstop mode.
* Argument      : 
				Clk	:	The clock selected
				mod	:	IntOsc stop or operation in deepstop mode 
* Return Value  : none
******************************************************************************/
void OSC_Mode_set(CLK_Source Clk, DPSP_MOD mod)
{
	switch(Clk)
	{
		case MainOsc:
			R_DEV_WRITE_REG(32, MOSCSTPM, (0x02|mod));	/* set IntOSC operation mode in deepstop mode*/
			break;
		case IntOsc:
			R_DEV_WRITE_REG(32, ROSCSTPM, (0x02|mod));	/* set IntOSC operation mode in deepstop mode*/
			break;
		default:
			printf("%s","The clock selected does not have this function !!!");
			break;
	}
}

