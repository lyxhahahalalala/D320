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
* File Name : port.h
* Description : This file is for port define.
******************************************************************************/

#ifndef    _R_PORT_

#define    _R_PORT_

/******************************************************************************
Typedef definitions
******************************************************************************/
#define NULL ((void *)0)

typedef unsigned int bool;

struct GPIO_REG_SET
{
	volatile uint32* PORT_PPCMD;
	const volatile uint32* PORT_PPROTS;
	volatile uint16* PFC;
	volatile uint16* PFCE;
	volatile uint16* PMC;
	volatile uint16* PM;
	volatile uint16* PIBC;
	volatile uint32* PODC;
	volatile uint16* PBDC;
	volatile uint16* PIPC;
	const volatile uint16* PPR;
	volatile uint16* PU;
	volatile uint16* PD;
	volatile uint16* P;
	volatile uint16* PNOT;
};


enum
{
	false,
	true,
};

typedef enum
{
	PORT0 	= 0,
	PORT1,
	PORT3,
	PORT10,
	PORT11,
	PORT16,
	PORT17,
	PORT21,
	PORT42,
	PORT43,
	PORT44,
	PORT45,
}T_PORT;

typedef enum
{
	PIN0 	= 0,
	PIN1	=1,
	PIN2	=2,
	PIN3	=3,
	PIN4	=4,
	PIN5	=5,
	PIN6	=6,
	PIN7	=7,
	PIN8	=8,
	PIN9	=9,
	PIN10	=10,
	PIN11	=11,
	PIN12	=12,
	PIN13	=13,
	PIN14	=14,
	PIN15	=15,
}T_PIN;

typedef enum
{
	Pin_Out,	
	Pin_In,
}T_Port_Mod;

typedef enum
{
	Normal_Port,
	Alter_Func1,
	Alter_Func2,
	Alter_Func3,
	Alter_Func4,
	Alter_Func5,
	Alter_Func6,
}T_Func;

typedef enum
{
	Port_IP_SoftCtl,
	Port_IP_DirCtl,	
}T_Port_IP_Ctl;

typedef enum
{
	PuPd_No_Set,
	Port_Pu,
	Port_Pd,
}T_Port_pUpD;

typedef enum
{
	Port_IB_Off,
	Port_IB_On,
}T_Port_IB;

typedef enum
{
	Port_PBD_Off,
	Port_PBD_On,
}T_Port_PBD;

typedef enum
{
	Port_Push_Pull,
	Port_Open_Drain,	
}T_Port_OPBF;


#define Port_Init_Simple(a,b,c,d) Port_Init(a,b,c,d,Port_IP_SoftCtl,PuPd_No_Set,Port_IB_Off,Port_PBD_Off,Port_Push_Pull)

void Port_Init(T_PORT port, T_PIN pin, T_Func func, T_Port_Mod mode, T_Port_IP_Ctl IP_Ctl,
			T_Port_pUpD pUpD,T_Port_IB IB_Func,T_Port_PBD	PBD_Func,T_Port_OPBF OPB_Func);
void Port_Set(T_PORT port, T_PIN pin);
void Port_Clear(T_PORT port, T_PIN pin);
void Port_Toggle(T_PORT port, T_PIN pin);
uint16 Port_Read(T_PORT port,T_PIN pin);

#endif


