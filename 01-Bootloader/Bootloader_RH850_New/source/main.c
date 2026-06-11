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
// #include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "target.h"
#include "fcl_descriptor.h"
#include "fcl_user.h"

#include "fcl.h"

#include "cpu.h"
#include "stdio.h"
#include "string.h"
#include "rscan.h"
#include "upgrade.h"
#include "bt_uds.h"
#include "bt_pub.h"
#include "ostm0.h"

// #include "dev_api.h"

#define R_DEV_READ_REG(SIZE, ADDR)   ((*((volatile uint##SIZE##_t*)((ADDR)))))
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)); }

/**********************************************************************************************
  variables and functions
***********************************************************************************************/

void fdl_init(void);
void FCL_Ctrl_UDS(void);
void FCL_Ctrl_Commom(void);
uint32_t (* jr_app)(void);

uint8_t eol_icutype;
bool    udsProgRequest;

/*该地址存储的值，是在APP中更新。在boot判断，若更新，进入UDS编程，并复位该地址值*/
const uint32_t* ReflashMarker = (uint32_t*)0xFEDFFFE0; 



void icu_type_init(void)
{
	Diagnostic_EEProm_Read(0x1D00, 1, &eol_icutype);
	if(eol_icutype > 15)
	{
		eol_icutype = 0;
		Diagnostic_EEProm_Write(0x1D00, 1, &eol_icutype);
	}
}

void uds_boot_version_init(void)
{
	uint8_t Fbl_VN_temp[5] = {0x00};
	Diagnostic_EEProm_Read(Logicdata_list[INDEX_F123].ee_address, Logicdata_list[INDEX_F123].dlc, Fbl_VN_temp);
	for(int i = 0; i < 5; i++)
	{
		if(Fbl_VN_temp[i] != F123_Fbl_VN[i])
		{
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F123].ee_address, Logicdata_list[INDEX_F123].dlc, F123_Fbl_VN);
			break;
		}
	}
}

void gpio_init(void)
{
    PORT_ISO.PM17.UINT16   &= ~0x0870;
    PORT_ISO.PMC17.UINT16  &= ~0x0870;
    PORT_ISO.P17.BIT.P17_11 =  0;  //关闭 74HC595D_EN_N，熄灭所有LED
	
	//IOPWR_CTL
    PORT_ISO.PM21.UINT16   &= ~0x0100;
    PORT_ISO.PMC21.UINT16  &= ~0x0100;
    PORT_ISO.P21.BIT.P21_8  =  1;
	
    for(uint32_t i = 0; i < 10000; i++);
	
}

void Target_Init(void)
{
	fdl_init();
	icu_type_init();
	uds_boot_version_init();
    gpio_init();
    // Can_Init();
}

int main (void)
{
    r_fcl_status_t ret;
    bool  GU_ProgRequest;
	uint32_t resf;
    uint32_t *Src;
	uint32_t Reflash;
	
    R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK
    SET_EBASE(0xFED80000); //set Ebase to 0xFEDE0000--FCL_RAM_ADDRESS, set RINT to 1
    SET_PSW_EBV();
	
    Target_Init();
	
	__EI();
	
	GU_ProgRequest = false;
    udsProgRequest = false;
	user_address = APP_START_ADDR;
	Reflash = *ReflashMarker;
	if(Reflash == 0xCAFEBEE0)
	{
		R_DEV_WRITE_REG(32, 0xFFF80768, 0x7u); //reset reset flags
		memset((uint32_t *)ReflashMarker, 0, 4);
	}
    resf = R_DEV_READ_REG(32, 0xFFF80760); //detect CAN reprogram request or software reset happen
    if( (resf<<31) != 0 )  //reprogram request
    {
		Can_Init();
		if((Reflash == 0xCAFEBEEF) || (Reflash == 0xCAFEBEE8))
		{
			memset((uint32_t *)ReflashMarker, 0, 4);
			
			udsProgRequest = true;
			
			GU_ProgRequest = false;
		}
		else
		{
			udsProgRequest = false;
			
			for(uint32_t i = 0; i < 1000000; i++)
			{
				if(CanNode > 0)
				{
					GU_ProgRequest = true;
					break;
				}
				else
				{
					for(uint32_t j = 0; j < 50; j++);
				}
			}
		}
		
		R_DEV_WRITE_REG(32, 0xFFF80768, 0x7u); //reset reset flags
		
    }
    else
    {
		Src = (uint32_t *)(0x10000);
		if(Src[0] != 0 && Src[0] != 0xffffffff)
		{
			__DI();
		    jr_app= ( uint32_t (*)() )0x10000;		
		    jr_app();
		}
		else
		{
			Can_Init();
			while(1)
			{
				if(firmware_update_request == 1)
				{
					udsProgRequest = true;
					GU_ProgRequest = false;
					break;
				}
				else if(firmware_update_request == 2)
				{
					udsProgRequest = false;
					GU_ProgRequest = true;
					break;
				}
				else
				{
					/*等待收到升级请求*/
				}
			}
			
		}
    }
    
    ret = R_FCL_Init (&sampleApp_fclConfig_enu);
    if (ret == R_FCL_OK)
    {
        ret = R_FCL_CopySections();
    }
	
    if (ret == R_FCL_OK)
    {
		if(udsProgRequest)
		{
			FCL_Ctrl_UDS();
		}
		else if(GU_ProgRequest)
		{
			FCL_Ctrl_Commom();
		}
		else ;
    }
	
    while (1)
    {
        /* do nothing */
    }
	
	return 0;
	
} /* main */


void FCL_Ctrl_UDS(void)
{
    BT_Init();
    OSTM0_INIT();
    OSTM0_START();
	
    __EI();
	
    while(1)
    {
		EraseDo();
		BTPF_ProgDo();
        BTCCFL_Do8s16r();
    }
    
}
void FCL_Ctrl_Commom(void)
{
	char FclHeadBuffer[1024];
	uint8_t  ret = R_FCL_OK;
	CAN_MESSAGE  tx_msg;
	
	__EI();
	
    while(1)
    {
		CAN_BOOT_ExecutiveCommand();
		if(CanStop)
		{
			if(user_address == APP_START_ADDR)
			{
				for(uint32_t i = 0; i < 1024; i++)
				{
					FclHeadBuffer[i] = rx_buf.buf[i];
				}
				ret = R_FCL_OK;
			}
			else
			{
				ret = FCL_prog(user_address, rx_buf.buf);
			}
			
			if(R_FCL_OK == ret)
			{
				user_address += 1024;
				memset(&rx_buf,0, sizeof(CAN_DATA_FIFO));
				tx_msg.id = 0x156;
				tx_msg.len = 0;
				
				__EI();
				if(CanNode == 1) CAN1_SendFIFOMessage(&tx_msg);
				else if(CanNode == 2) CAN2_SendFIFOMessage(&tx_msg);
				else;
				
				CanStop = 0;
				
				if(Last_Frame) 
				{
					__DI();
					ret =  FCL_prog(APP_START_ADDR, FclHeadBuffer);
					__EI();
					if(R_FCL_OK == ret)
					{
					#if 0
						__DI();
						jr_app= ( uint32_t (*)() )0x10000;
						jr_app();
					#endif
					}
				}
			}
			
		}
    }
}


