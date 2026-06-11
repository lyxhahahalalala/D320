#ifdef ENABLE_QAC_TEST
#pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
 * File Name     : $Source: main.c $
 * Mod. Revision : $Revision: 1.14 $
 * Mod. Date     : $Date: 2014/07/08 13:40:34MESZ $
 * Device(s)     : RV40 Flash based RH850 microcontroller
 * Description   : Application sample for usage of Renesas Flash Control Library (FCL)
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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 ************************************************************************************************************/

#ifdef ENABLE_QAC_TEST
#pragma PRQA_MESSAGES_ON 0292
#endif

//#define _APP_MAIN
/************************************************************************************************************
 * MISRA Rule:   MISRA-C 2004 rule 3.1 (QAC message 0292)
 * Reason:       To support automatic insertion of revision, module name etc. by the source
 *               revision control system it is necessary to violate the rule, because the
 *               system uses non basic characters as placeholders.
 * Verification: The placeholders are used in commentaries only. Therefore rule violation cannot
 *               influence code compilation.
 ************************************************************************************************************/


/************************************************************************************************************
  Includes   <System Includes> , "Project Includes"
  ************************************************************************************************************/

#include "R_typedefs.h"
#include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "target.h"
#include "fcl_descriptor.h"
#include "fcl_user.h"

#include "Cpu.h"
#include "can.h"
#include "bt_uds.h"
#include "stdio.h"
#include "string.h"
#include "ostm0.h"

/************************************************************************************************************
  Macro definitions
  ************************************************************************************************************/
#define FLMD0_PROTECTION_OFF    (0x01u)
#define FLMD0_PROTECTION_ON     (0x00u)


const uint32_t* ReflashMarker = (uint32_t*)0xFEDFFFE0;
/************************************************************************************************************
  Typedef definitions
  ************************************************************************************************************/

/************************************************************************************************************
  Macro definitions
  ************************************************************************************************************/

/************************************************************************************************************
  Exported global variables (to be accessed by other files)
  ************************************************************************************************************/

/* This array reserves the copy area in the device RAM */
#define FCL_RAM_EXECUTION_AREA_SIZE 0x8000

#if R_FCL_COMPILER == R_FCL_COMP_GHS
#pragma ghs startdata
#pragma ghs section bss = ".FCL_RESERVED"
#define R_FCL_NOINIT
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
#pragma segment = "FCL_RESERVED"
#pragma location = "FCL_RESERVED"
#define R_FCL_NOINIT __no_init
#elif R_FCL_COMPILER == R_FCL_COMP_REC
#pragma section r0_disp32 "FCL_RESERVED"
#define R_FCL_NOINIT
#endif

R_FCL_NOINIT uint8_t FCL_Copy_area[FCL_RAM_EXECUTION_AREA_SIZE];

#if R_FCL_COMPILER == R_FCL_COMP_GHS
#pragma ghs section bss = default
#pragma ghs enddata
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
#pragma dataseg = default
#elif R_FCL_COMPILER == R_FCL_COMP_REC
#pragma section default
#endif

extern uint8_t CAN;
extern Can_PduType BT_TxPdu;

/************************************************************************************************************
  Private global variables and functions
  ************************************************************************************************************/
//r_Error_t Clock_Init( void );
void BT_Init ( void );
void FCL_Ctrl (void);
extern void Target_Init(void);
void BTPF_ProgDo();
void EraseDo();
void BTCCFL_Do8s16r();
void fdl_init(void);


uint32_t (* jr_app)(void);

#define USER_AREA_STSRT_ADDRESS 0x10000
#define USER_AREA_FLAG_ADDRESS 0x002FE000


#define KEY_SET                (PORT_ISO.PPR16.BIT.PPR16_9)
#define KEY_CLEAR              (PORT_ISO.PPR16.BIT.PPR16_10)


#define R_DEV_READ_REG(SIZE, ADDR)   ((*((volatile uint##SIZE##_t*)((ADDR)))))
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)); }

void Key_Input(void);

#define CAN1_STB PORT_ISO.P3.BIT.P3_2  
#define CAN2_STB PORT_ISO.P3.BIT.P3_7  
#define CAN1_EN PORT_ISO.P3.BIT.P3_3  
#define CAN2_EN PORT_ISO.P3.BIT.P3_8

bool ProgRequest;

unsigned char isJumpFrmApp = 0;

/************************************************************************************************************
 * Function name: Key_Input()
 ***********************************************************************************************************/
/**
 * initialize input port
 *
 * @param[in,out] no
 * @return        no
 */
/***********************************************************************************************************/
void Key_Input(void)
{
    PORT_ISO.PM17.UINT16              &= ~0x0870;
    PORT_ISO.PMC17.UINT16            &= ~0x0870;    
    PORT_ISO.P17.BIT.P17_11 = 0;

    PORT_ISO.PM21.UINT16                 &= ~0x0100;
    PORT_ISO.PMC21.UINT16              &= ~0x0100;          //OUTPUT

    PORT_ISO.P21.BIT.P21_8 = 1;
    for(uint32_t i = 0;i<30000;i++);

    //keys initialize
    PORT_ISO.PMC16.UINT16              &= ~0x0f00; 
    PORT_ISO.PM16.UINT16                |= 0x0f00;
    PORT_ISO.PIBC16.UINT16            |= 0x0f00;             //P16_8 - P16_11   sw1234 input xiangyanglvxingche   
//IOPWR_CTL
    PORT_ISO.PM21.UINT16              &= ~0x0100;
    PORT_ISO.PMC21.UINT16            &= ~0x0100;             //    P21_8  - IOPWR_CTL
    PORT_ISO.P21.BIT.P21_8 = 1;

}


/************************************************************************************************************
 * Function name: Target_Init()
 ***********************************************************************************************************/
/**
 * initialize target nitialization
 *
 * @param[in,out] no
 * @return        no
 */
/***********************************************************************************************************/
void Target_Init(void)
{
    Key_Input();
    Can_Init();
    PORT_ISO.PM42.UINT16              &= ~0x0006;
    PORT_ISO.PMC42.UINT16             &= ~0x0006;             //P42_1 P42_2   CAN_STB   CAN_EN OUTPUT
    PORT_ISO.P42.UINT16               |= 0x0006;              //P42_1 = 1 = P42_2

    PORT_ISO.PM3.UINT16               &= ~0x018C;              //P3-2 CAN1_STB P3-3 CAN1-EN   P3-7 CAN2-STB P3-8 CAN2-EN
    PORT_ISO.PMC3.UINT16              &= ~0x018C;             //
    CAN2_EN = 1;
    CAN2_STB= 0;
    CAN1_EN = 1;
    CAN1_STB= 0;
    
    CAN2_EN = 1;
    CAN2_STB= 1;
    CAN1_EN = 1;
    CAN1_STB= 1;
}

/************************************************************************************************************
 * Function name: <Add function name>
 ***********************************************************************************************************/
/**
 * <Add function description>
 *
 * @param[in,out] <Add parameter>      <Add parameter description>
 * @return        <Add return value>
 */
/***********************************************************************************************************/
void main (void)
{
    uint32_t resf;
    r_fcl_status_t ret;
    uint32_t cnt = 0;
    uint8_t canUpdate = 0;
    uint32_t *Src;
	uint32_t *App_Valid;
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    uint32_t   (* fpFct)(void);
#endif
    R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK
#if (defined R_FCL_DEVICE_SPECIFIC_INIT)
        R_FCL_DEVICE_SPECIFIC_INIT
#endif
    SET_EBASE(0xFED80000); //set Ebase to 0xFEDE0000--FCL_RAM_ADDRESS, set RINT to 1
    SET_PSW_EBV();

    ProgRequest = false;

    Target_Init();
	fdl_init();

	uint8 Fbl_VN_temp[4] = {0x00};
	Diagnostic_EEProm_Read(Logicdata_list[19].ee_address, Logicdata_list[19].dlc, Fbl_VN_temp);
	for(int i = 0x00; i < 4; i++)
	{
		if(Fbl_VN_temp[i] != F123_Fbl_VN[i])
		{
			Diagnostic_EEProm_Write(Logicdata_list[19].ee_address, 4,&F123_Fbl_VN);
			break;
		}
	}
     __EI();

     uint32_t Reflash = *ReflashMarker;
    //detect CAN reprogram request or software reset happen
    resf = R_DEV_READ_REG(32, 0xFFF80760);

    if((Reflash == 0xCAFEBEEF) &&( (resf<<31) != 0))               //reprogram request
    {


	R_DEV_WRITE_REG(32,0xFFF80768,0x7u);//reset reset flags

	memset((uint32_t)ReflashMarker, 0, 4);
	
	ProgRequest == 1;

	isJumpFrmApp = 0x1;
    }
    else
    {
	__DI();
	//  ExecutiveAppHandle();
#if 1
#ifdef _APP_MAIN	
	Src = (uint32_t *)(0x10000);
	if(Src[0]  != 0xffffffff )
	{
	    jr_app= ( uint32_t (*)() )0x10000;		
	    jr_app();
	}
	else
	{
	    
	}
#else
	Src = (uint32_t *)(0x10000);
	App_Valid = (uint32_t *)(0x10004);

	if((Src[0]  != 0xffffffff )||(App_Valid[0]!= 0xffffffff))
	{
	    jr_app= ( uint32_t (*)() )0x10000;		
	    jr_app();
	}
	else
	{
	    
	}	
#endif
	#endif
    }
    
    ret = R_FCL_Init (&sampleApp_fclConfig_enu);

    if (R_FCL_OK == ret)
    {
        ret = R_FCL_CopySections ();
    }
    //__EI(); //after copy section	 
    if (R_FCL_OK == ret)
    {
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
	fpFct = ( uint32_t (*)() )R_FCL_CalcFctAddr ( (uint32_t)(&FCL_Ctrl) );
	fpFct ();
#else
	//fdl_init();
	//Diagnostic_EEProm_Write();


	FCL_Ctrl ();
#endif
    }    
    while (1)
    {
        /* do nothing */
    }
} /* main */

/************************************************************************************************************
 * Function name: FCL_Ctrl()
 ***********************************************************************************************************/
/**
 * execute fcl function
 *
 * @param[in,out] no
 * @return        no
 */
/***********************************************************************************************************/
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
#if R_FCL_COMPILER == R_FCL_COMP_GHS
#pragma ghs section text = ".R_FCL_CODE_USR"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
#pragma location = "R_FCL_CODE_USR"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
#pragma section text "R_FCL_CODE_USR"
#endif
#endif
void FCL_Ctrl (void)
{
    
    BT_Init();
    OSTM0_INIT(1);
    OSTM0_START();

 //   if(isJumpFrmApp != 0x0)
    {
	//unsigned char sendbuf[3] = {0x02,0x50,0x02};
	//BT_TxPdu.sdu = sendbuf;
	//Can_Write(0, &BT_TxPdu);
    }
 
    __EI();

    while(1)
    {
	BTPF_ProgDo();
        EraseDo();
        BTCCFL_Do8s16r();
    }
    
} /* FCL_Ctrl */



#ifdef _APP_MAIN

Can_PduType  App_TxPdu;
uint8_t App_SendBuf[8] = {0x02};

#pragma ghs section text = ".bcode"
void app_main(void)
{

    uint8_t recvBuf[4];
    uint8_t App_SendBuf[8] = {0x02,0x51,0x03,0x00,0x00,0x0,0x0,0x0};
    uint32_t setReflashMarker;

    memset((uint32_t)ReflashMarker, 0, 4);
    setReflashMarker = 0xCAFEBEEF;
    
    App_TxPdu.id = 0x120;
	App_TxPdu.length = 8;
	App_TxPdu.sdu = App_SendBuf;

	while(1)
	{
	    Can_Write(0, &App_TxPdu);
	    for(uint32_t i = 0;i<10000000;i++)
	    ;

	    if((RSCAN0.RFSTS1.UINT32 & 0xFF00) > 0)
	    {
		*((int*)&(recvBuf[0]))  = RSCAN0.RFDF01.UINT32;
		if((recvBuf[0] == 0x02)&& (recvBuf[1] == 0x11) && (recvBuf[2] == 0x03) )
		{
		            /* Clear status. */
		    RSCAN0.TMSTS16.UINT8     = 0x00U;
		    RSCAN0.TMID16.UINT32     = (0x6F0 & 0x7FF);
		    RSCAN0.TMPTR16.UINT32    = (8 & 0xF) << 28;
		    RSCAN0.TMDF016.UINT32    = *((uint32_t *)&(App_SendBuf[0]));
		    RSCAN0.TMDF116.UINT32    = *((uint32_t *)&(App_SendBuf[4]));
		    RSCAN0.TMC16.UINT8       = 0x01;

		    memcpy((uint32_t)ReflashMarker, &setReflashMarker , 4);
		    protected_write(SYSPROTCMD0,SYSPROTS0,SYSSWRESA,0x01u); /* trigger enable (protected write) */
		}
	    }
    
	}
}

#endif
