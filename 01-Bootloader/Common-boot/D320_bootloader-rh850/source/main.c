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
#include "r_typedefs.h"
#include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "target.h"
#include "fcl_descriptor.h"
#include "fcl_user.h"

#include "cpu.h"
#include "stdio.h"
#include "string.h"
#include "rscan.h"
#include "upgrade.h"



/************************************************************************************************************
  Macro definitions
  ************************************************************************************************************/
#define FLMD0_PROTECTION_OFF    (0x01u)
#define FLMD0_PROTECTION_ON     (0x00u)


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



/************************************************************************************************************
  Private global variables and functions
  ************************************************************************************************************/
//r_Error_t Clock_Init( void );
void FCL_Ctrl(void);
uint8_t FCL_Write(uint32_t address,char write_buf[]);
uint32_t (* jr_app)(void);


#define USER_AREA_STSRT_ADDRESS  0x10000
#define USER_AREA_FLAG_ADDRESS   0x002FE000

#define R_DEV_READ_REG(SIZE, ADDR)   ((*((volatile uint##SIZE##_t*)((ADDR)))))
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)); }

#define CAN1_STB  PORT_ISO.P3.BIT.P3_2  
#define CAN2_STB  PORT_ISO.P3.BIT.P3_7  
#define CAN1_EN   PORT_ISO.P3.BIT.P3_3  
#define CAN2_EN   PORT_ISO.P3.BIT.P3_8

#define KEY_SET    (PORT_ISO.PPR16.BIT.PPR16_9)
#define KEY_CLEAR  (PORT_ISO.PPR16.BIT.PPR16_10)




void Key_Input(void)
{
    PORT_ISO.PM17.UINT16   &= ~0x0870;
    PORT_ISO.PMC17.UINT16  &= ~0x0870;
	
    PORT_ISO.P17.BIT.P17_11 =  0;
	
	//IOPWR_CTL
    PORT_ISO.PM21.UINT16   &= ~0x0100;
    PORT_ISO.PMC21.UINT16  &= ~0x0100;
    PORT_ISO.P21.BIT.P21_8  =  1;
	
    for(uint32_t i = 0; i < 30000; i++);
	
    //keys initialize
    PORT_ISO.PMC16.UINT16  &= ~0x0f00;
    PORT_ISO.PM16.UINT16   |=  0x0f00;
    PORT_ISO.PIBC16.UINT16 |=  0x0f00; //P16_8 - P16_11   sw1234 input xiangyanglvxingche   
	
	//IOPWR_CTL
    PORT_ISO.PM21.UINT16   &= ~0x0100;
    PORT_ISO.PMC21.UINT16  &= ~0x0100; // P21_8  - IOPWR_CTL
    PORT_ISO.P21.BIT.P21_8  =  1;
	
}

void Target_Init(void)
{
    Key_Input();
	
    Can_Init();
	
    PORT_ISO.PM3.UINT16  &= ~0x018C;
    PORT_ISO.PMC3.UINT16 &= ~0x018C;
    
	CAN2_EN  = 1;
    CAN2_STB = 0;
    CAN1_EN  = 1;
    CAN1_STB = 0;
    
    CAN2_EN  = 1;
    CAN2_STB = 1;
    CAN1_EN  = 1;
    CAN1_STB = 1;
	
	
	
	
}

int main (void)
{
    uint32_t resf;
    r_fcl_status_t ret;
    uint32_t cnt = 0;
    uint8_t canUpdate = 0;
    uint8_t *Src;
	
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    uint32_t (* fpFct)(void);
#endif
	
    R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK
#if (defined R_FCL_DEVICE_SPECIFIC_INIT)
	R_FCL_DEVICE_SPECIFIC_INIT
#endif
	
    SET_EBASE(0xFED80000); //set Ebase to 0xFEDE0000--FCL_RAM_ADDRESS, set RINT to 1
    SET_PSW_EBV();
	
	address = USER_AREA_STSRT_ADDRESS;
	
	
    Target_Init();
	__EI();
	
    while(cnt++ < 1000000)
	{
		if(CanNode > 0)
		{
			canUpdate = 1;
			break;
		}
		else
		{
			for(uint32_t i = 0; i < 50; i++);
		}
    }
    for(uint32_t i = 0; i < 1000000; i++);
	
	resf = R_DEV_READ_REG(32, 0xFFF80760);
    if( ((resf<<31) == 0) && (canUpdate ==0) )               //no key pressed and no software reset  Read_Value == 0
    {
		__DI();
		//  ExecutiveAppHandle();
		Src = (uint8_t *)(0x10000);
		if(Src[0]  != 0xff )
		{
		    jr_app= ( uint32_t (*)() )0x10000;		
		    jr_app();
		}
    }
	
    if((resf<<31) != 0 ) // software reset 
    {
		R_DEV_WRITE_REG(32,0xFFF80768,0x7u);//reset reset flags
    }	
	
    ret = R_FCL_Init (&sampleApp_fclConfig_enu);
    if (R_FCL_OK == ret)
    {
        ret = R_FCL_CopySections ();
    }
	
    __EI(); //after copy section	 
	
    if (R_FCL_OK == ret)
    {
	#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
		fpFct = ( uint32_t (*)() )R_FCL_CalcFctAddr ( (uint32_t)(&FCL_Ctrl) );
		fpFct ();
	#else
		FCL_Ctrl ();
	#endif
    }
	
    while (1)
    {
        /* do nothing */
    }
	
	return 0;
	
} /* main */



#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
#if R_FCL_COMPILER == R_FCL_COMP_GHS
#pragma ghs section text = ".R_FCL_CODE_USR"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
#pragma location = "R_FCL_CODE_USR"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
#pragma section text "R_FCL_CODE_USR"
#endif
#endif
/**
 * Function name: FCL_Ctrl()
 * execute fcl function
 *
 * @param[in,out] no
 * @return        no
 */
void FCL_Ctrl (void)
{
	r_fcl_status_t  ret;
	CAN_MESSAGE  tx_msg;
	
    while(1)
    {
		CAN_BOOT_ExecutiveCommand();
		if(CanStop)
		{	       
			ret =  FCL_Write(address, rx_buf.buf);
			if(R_FCL_OK == ret)
			{
				address += 1024;
				memset(&rx_buf,0, sizeof(CAN_DATA_FIFO));
				tx_msg.id = 0x156;
				tx_msg.len = 0;
				__EI();
				if(CanNode == 1) CAN1_SendMessage(&tx_msg);
				else if(CanNode == 2) CAN2_SendMessage(&tx_msg);
				else;
				CanStop = 0;
				if(Last_Frame) 
				{
					FCLUser_Close();
				#if 0
					jr_app= ( uint32_t (*)() )0x10000;
					jr_app();
				#endif
				}						
			}
			else erased = 0;			
		}
    }
} /* FCL_Ctrl */

/**
 * Function name: FCL_Write()
 * execute fcl erease function. not used in this part
 *
 * @param[in,out] address to write to, write content
 * @return        1: failed
 0: success
 */
uint8_t FCL_Write(uint32_t address,char write_buf[])
{
    r_fcl_request_t     myRequest;  
    // uint16_t            i;
    FCLUser_Open ();//get access to flash

    /* write 1024 bytes to address  */
    myRequest.command_enu = R_FCL_CMD_WRITE;
    myRequest.bufferAdd_u32 = (uint32_t)&write_buf[0];
    myRequest.idx_u32       = address;
    myRequest.cnt_u16       = 4;                    /* written bytes = 256 * cnt_u16 */
    R_FCL_Execute (&myRequest);
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    while (R_FCL_BUSY == myRequest.status_enu)
    {
		R_FCL_Handler ();
    }
#endif

    if (R_FCL_OK != myRequest.status_enu) //error
    {
		FCLUser_Close ();
		return 1;
    }
    FCLUser_Close ();
    return 0;
}

#if 0
/**
 * initialize TAU0
 * @param[in,out] no
 * @return        no
 */
/***********************************************************************************************************/
void init_TAUB0(void) {                 /* initialize timer TAUB0 channel 0 to interval timer mode */
    TAUB0TPS=0x0007;                      /* specify clock frequency of CK0...CK3  (CK0 -> 40 MHz / (2 ^ 7) = 312.5 KHz) */
    TAUB0CMOR0=0x0000;                    /* interval timer mode */
    TAUB0CDR0=31250;                      /* compare register in interval timer mode (312.5KHz / 31250 = 10 Hz) */
    TAUB0TS=0x0001;                       /* start channel 0 */
}

#pragma ghs section text = ".R_FCL_CODE_USRINT"
/*

  contains user interrupt routines that may be executed in parallel 
  with FCL operation when code flash is unavailable. 
  */
/*
 * TAUB0 interrupt service routine
 * Not used in this part
 */
__interrupt void INTTAUB0I0(void) {     /* TAUB0 interrupt service routine */

    TimerCounter++;
    if(TimerCounter >= 5)
    {
	TimerCounter  = 0;
 	TimeOut = 1;
    }

}

#endif




