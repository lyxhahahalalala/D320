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
* File Name     : port.c
* Version       : 1.0
* Device(s)     : R7F701403AFP RH850/D1L
* Tool-Chain    :GHS Multi
* Description   : This file is for port set.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*                  : 11.11.2016 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include "Device.h"
#include "port.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
struct GPIO_REG_SET Gpio_Reg_Sets[]=
{
	{
		&PORT_AWOPPCMD0,	&PORT_AWOPPROTS0,	&PORT_AWOPFC0,		&PORT_AWOPFCE0,	&PORT_AWOPMC0,
		&PORT_AWOPM0,		&PORT_AWOPIBC0,	&PORT_AWOPODC0,	&PORT_AWOPBDC0,	&PORT_AWOPIPC0,
		&PORT_AWOPPR0,		&PORT_AWOPU0,		&PORT_AWOPD0,		&PORT_AWOP0,		&PORT_AWOPNOT0
	},
	{
		&PORT_ISOPPCMD1,	&PORT_ISOPPROTS1,	&PORT_ISOPFC1,		&PORT_ISOPFCE1,		&PORT_ISOPMC1,
		&PORT_ISOPM1,		&PORT_ISOPIBC1,		&PORT_ISOPODC1,	&PORT_ISOPBDC1,	&PORT_ISOPIPC1,
		&PORT_ISOPPR1,		&PORT_ISOPU1,		&PORT_ISOPD1,		&PORT_ISOP1,		&PORT_ISOPNOT1
	},
	{
		&PORT_ISOPPCMD3,	&PORT_ISOPPROTS3,	&PORT_ISOPFC3,		&PORT_ISOPFCE3,		&PORT_ISOPMC3,
		&PORT_ISOPM3,		&PORT_ISOPIBC3,		&PORT_ISOPODC3,	&PORT_ISOPBDC3,	&PORT_ISOPIPC3,		
		&PORT_ISOPPR3,		&PORT_ISOPU3,		&PORT_ISOPD3,		&PORT_ISOP3,		&PORT_ISOPNOT3
	},
	{
		&PORT_ISOPPCMD10,	&PORT_ISOPPROTS10,	&PORT_ISOPFC10,		&PORT_ISOPFCE10,	&PORT_ISOPMC10,
		&PORT_ISOPM10,		&PORT_ISOPIBC10,	&PORT_ISOPODC10,	&PORT_ISOPBDC10,	NULL,	
		&PORT_ISOPPR10,		(volatile uint16*)NULL,	(volatile uint16*)NULL,	&PORT_ISOP10,		&PORT_ISOPNOT10
	},
	{
		&PORT_ISOPPCMD11,	&PORT_ISOPPROTS11,	&PORT_ISOPFC11,		(volatile uint16*)NULL,	&PORT_ISOPMC11,
		&PORT_ISOPM11,		&PORT_ISOPIBC11,	&PORT_ISOPODC11,	&PORT_ISOPBDC11,	(volatile uint16*)NULL,
		&PORT_ISOPPR11,		(volatile uint16*)NULL,	(volatile uint16*)NULL,	&PORT_ISOP11,		&PORT_ISOPNOT11
	},
	{
		&PORT_ISOPPCMD16,	&PORT_ISOPPROTS16,	&PORT_ISOPFC16,		&PORT_ISOPFCE16,	&PORT_ISOPMC16,
		&PORT_ISOPM16,		&PORT_ISOPIBC16,	&PORT_ISOPODC16,	&PORT_ISOPBDC16,	&PORT_ISOPIPC16,
		&PORT_ISOPPR16,		(volatile uint16*)NULL,	(volatile uint16*)NULL,	&PORT_ISOP16,		&PORT_ISOPNOT16
	},
	{
		&PORT_ISOPPCMD17,	&PORT_ISOPPROTS17,	&PORT_ISOPFC17,		&PORT_ISOPFCE17,	&PORT_ISOPMC17,
		&PORT_ISOPM17,		&PORT_ISOPIBC17,	&PORT_ISOPODC17,	&PORT_ISOPBDC17,	&PORT_ISOPIPC17,
		&PORT_ISOPPR17,		(volatile uint16*)NULL,	(volatile uint16*)NULL,	&PORT_ISOP17,		&PORT_ISOPNOT17
	},
	{
		&PORT_ISOPPCMD21,	&PORT_ISOPPROTS21,	&PORT_ISOPFC21,		(volatile uint16*)NULL,	&PORT_ISOPMC21,
		&PORT_ISOPM21,		&PORT_ISOPIBC21,	&PORT_ISOPODC21,	&PORT_ISOPBDC21,	&PORT_ISOPIPC21,
		&PORT_ISOPPR21,		&PORT_ISOPU21,		&PORT_ISOPD21,		&PORT_ISOP21,		&PORT_ISOPNOT21
	},
	{
		&PORT_ISOPPCMD42,	&PORT_ISOPPROTS42,	&PORT_ISOPFC42,		&PORT_ISOPFCE42,	&PORT_ISOPMC42,
		&PORT_ISOPM42,		&PORT_ISOPIBC42,	&PORT_ISOPODC42,	&PORT_ISOPBDC42,	&PORT_ISOPIPC42,
		&PORT_ISOPPR42,		&PORT_ISOPU42,		&PORT_ISOPD42,		&PORT_ISOP42,		&PORT_ISOPNOT42
	},
	{
		&PORT_ISOPPCMD43,	&PORT_ISOPPROTS43,	&PORT_ISOPFC43,		&PORT_ISOPFCE43,	&PORT_ISOPMC43,
		&PORT_ISOPM43,		&PORT_ISOPIBC43,	&PORT_ISOPODC43,	&PORT_ISOPBDC43,	&PORT_ISOPIPC43,
		&PORT_ISOPPR43,		&PORT_ISOPU43,		&PORT_ISOPD43,		&PORT_ISOP43,		&PORT_ISOPNOT43
	},
	{
		&PORT_ISOPPCMD44,	&PORT_ISOPPROTS44,	&PORT_ISOPFC44,		&PORT_ISOPFCE44,	&PORT_ISOPMC44,
		&PORT_ISOPM44,		&PORT_ISOPIBC44,	&PORT_ISOPODC44,	&PORT_ISOPBDC44,	&PORT_ISOPIPC44,
		&PORT_ISOPPR44,		&PORT_ISOPU44,		&PORT_ISOPD44,		&PORT_ISOP44,		&PORT_ISOPNOT44
	},
	{
		&PORT_ISOPPCMD44,	&PORT_ISOPPROTS45,	&PORT_ISOPFC45,		&PORT_ISOPFCE45,	&PORT_ISOPMC45,
		&PORT_ISOPM45,		&PORT_ISOPIBC45,	&PORT_ISOPODC45,	&PORT_ISOPBDC45,	&PORT_ISOPIPC45,
		&PORT_ISOPPR45,		&PORT_ISOPU45,		&PORT_ISOPD45,		&PORT_ISOP45,		&PORT_ISOPNOT45	
	},
};

/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/



/******************************************************************************
* Function Name : void Port_Init( void )
* Description   : This function initialize ports. 
* Argument      : 	port			port selected
				pin			pin selected
				func			pin function choose
				mode		pin mode set
				IP_Ctl		ip control
				pUpD		pull-up or pull-down
				IB_Func		input buffer set	
				PBD_Func	bi-direction control
				T_Port_OPBF	output buffer function set
* Return Value  : none
******************************************************************************/
void Port_Init(T_PORT port, T_PIN pin, T_Func func, T_Port_Mod mode,T_Port_IP_Ctl IP_Ctl,
			T_Port_pUpD pUpD,T_Port_IB IB_Func,T_Port_PBD	PBD_Func,T_Port_OPBF OPB_Func)
{	
	uint32 reg32_value = 0;
	/* Set an input buffer for enables or disables.
	PIBCn  - Port Input Buffer Control Register
	b15:b0   PIBCn_n  - Enables/disables the input buffer.         */
	if(IB_Func)
		*(Gpio_Reg_Sets[port].PIBC) |= (uint16)1<<pin;
	else
		*(Gpio_Reg_Sets[port].PIBC) &= ~((uint16)1<<pin);

	/*port IP control,	Specifies the I/O control mode.
					0: I/O mode is selected by PMn.PMn_m (software I/O control).
					1: I/O mode is selected by the peripheral function (direct I/O control)
		there is no PFCE register with port11 and port21, if use these ports as altermative function, set the PIPC to 1 */
	if(IP_Ctl)
		*Gpio_Reg_Sets[port].PIPC |=(uint16)1<<pin;
	else
	{
		if((port==11||port==21)&&(func>0))
			*Gpio_Reg_Sets[port].PIPC |=(uint16)1<<pin;
		else
			*Gpio_Reg_Sets[port].PIPC &=~((uint16)1<<pin);
	}
		
	/* Set an interactive mode for enables or disables.
	PBDCn  - Port Bidirection Control Register
	b15:b0   PBDCn_n  - Enables/disables bidirectional mode of the corresponding pin.           */
	if(PBD_Func)
		*Gpio_Reg_Sets[port].PBDC |= (uint16)1<<pin;
	else
		*Gpio_Reg_Sets[port].PBDC &= ~((uint16)1<<pin);

	/* Set a whether connect an internal pull-down resistor.
	PDn    - Pull-Down Option Register
	b15:b0   PDn_n    - Specifies whether to connect an internal pull-down resistor to the corresponding pin.                         */
	if(port!=10||port!=11||port!=16||port!=17)
	{
		if(pUpD==2)
			*Gpio_Reg_Sets[port].PD |= (uint16)1<<pin;
		else
			*Gpio_Reg_Sets[port].PD |= ~((uint16)1<<pin);
	}

	/* Set a whether connect an internal pull-up resistor.
	PUn    - Pull-Up Option Register
	b15:b0   PUn_n    - Specifies whether to connect an
	                    internal pull-up resistor to
	                    the corresponding pin.                - P42_7 set to pull-up enable                  */
	if(pUpD==1)
		*Gpio_Reg_Sets[port].PU |= (uint16)1<<pin;
	else
		*Gpio_Reg_Sets[port].PU &= ~((uint16)1<<pin);
		
	/* Set the function of the output buffer to a push-pull or an open drain
	PODCn  - Port Open Drain Control Register
	b15:b0   PODCn_n  - Specifies the output buffer function. - P42_8 set to push-pull mode                  */
	do
	{
		reg32_value = *Gpio_Reg_Sets[port].PODC;
		if(OPB_Func)
			reg32_value   |= (uint32)(1<<pin);
		else
			reg32_value   &= ~(uint32)(1<<pin);			
		*Gpio_Reg_Sets[port].PORT_PPCMD = 0x000000A5UL;                          /* Protection release the PODC42 register     */
		*Gpio_Reg_Sets[port].PODC = reg32_value;
		*Gpio_Reg_Sets[port].PODC = ~reg32_value;
		*Gpio_Reg_Sets[port].PODC = reg32_value;
	} while ( *Gpio_Reg_Sets[port].PORT_PPROTS != 0x00000000UL );
	while ( (*Gpio_Reg_Sets[port].PODC & ((uint16)1<<pin))!= (uint32)(OPB_Func<<pin))
	{
	    /* Waiting for PODC8 to set.  */
	}

	/* Set a port mode for input port mode or output port mode
	PMn    - Port Mode Register
	b15:b0   PMn_n    - Specifies input/output mode of the corresponding pin.                        */
	if(mode)
		*Gpio_Reg_Sets[port].PM |= (uint16)(1<<pin);
	else
		*Gpio_Reg_Sets[port].PM &= ~(uint16)(1<<pin);

	/* Set a port mode or alternative mode.
	PMCn   - Port Mode Control Register
	PFC\PFCE -Port altermative function select, D1L2H has no PFCAE register*/
	if(func>0)
	{
		*Gpio_Reg_Sets[port].PMC |= (uint16)1<<pin;
		*Gpio_Reg_Sets[port].PFC |= ((uint16)(func-1) & 0x0001)<<pin;
		if(port!=11||port!=21)
			*Gpio_Reg_Sets[port].PFCE |= ((uint16)((func-1)>>1) & 0x0001)<<pin;
	}
	else
		*Gpio_Reg_Sets[port].PMC &= ~((uint16)1<<pin);

	/*if the pin is port mode, set the port initial value to low level*/
	if(func==0)
		*Gpio_Reg_Sets[port].P &= ~((uint16)1<<pin);
}


/******************************************************************************
* Function Name 	: void Port_Set( void )
* Description   	: This function sets the pin high. 
* Argument      	: port	the port selected
				  pin		the pin selected
* Return Value  	: none
******************************************************************************/
void Port_Set(T_PORT port, T_PIN pin)
{
	*Gpio_Reg_Sets[port].P |= (uint16)1<<pin;
}


/******************************************************************************
* Function Name 	: void Port_Clear( void )
* Description   	: This function sets the pin level to low. 
* Argument      	: port	the port selected
				  pin		the pin selected 	
* Return Value  	: none
******************************************************************************/
void Port_Clear(T_PORT port, T_PIN pin)
{
	*Gpio_Reg_Sets[port].P &= ~((uint16)1<<pin);
}


/******************************************************************************
* Function Name 	: void Port_Toggle( void )
* Description   	: This function toggles the pin level. 
* Argument      	: port	the port selected
				  pin		the pin selected 	
* Return Value  	: none
******************************************************************************/
void Port_Toggle(T_PORT port, T_PIN pin)
{
	*Gpio_Reg_Sets[port].PNOT |= (uint16)1<<pin;
}


/******************************************************************************
* Function Name 	: void Port_Read( void )
* Description   	: This function sets the pin level. 
* Argument      	: port	the port selected
				  pin		the pin selected 	
* Return Value  	: the level of the pin selected
******************************************************************************/
uint16 Port_Read(T_PORT port,T_PIN pin)
{		
	return ((*Gpio_Reg_Sets[port].PPR) & ((uint16)1<<pin));
}

