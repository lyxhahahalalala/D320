/******************************************************************************
 * Includes
 */

#include "r_typedefs.h"
#include "rscan.h"
#include "stdio.h"
#include "string.h"
#include "crc16.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "target.h"
#include "fcl_cfg.h"
#include "fcl_descriptor.h"
#include "fcl_user.h"
#include "fcl.h"
#include "cpu.h"
#include "bt_pub.h"
#include "upgrade.h"


#define CAN1_STB  PORT_ISO.P3.BIT.P3_2  
#define CAN2_STB  PORT_ISO.P3.BIT.P3_7  
#define CAN1_EN   PORT_ISO.P3.BIT.P3_3  
#define CAN2_EN   PORT_ISO.P3.BIT.P3_8



extern uint8_t BT_TxSt;

volatile uint32_t user_address;

uint8_t firmware_update_request = 0;


uint8_t Last_Frame = 0;
uint8_t CanStop = 0;
uint8_t CanNode = 0;
CAN_DATA_FIFO rx_buf;

static CAN_MESSAGE  msg_get;
static uint8_t  CAN_CanRxMsgFlag = 0; //接收到CAN数据后的标志
// static uint8_t  Communication_Error = 0;
static unsigned long data_index = 0;
static unsigned long data_size = 0;


void Can_Init(void)
{
    RS_CAN_init();
	
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


Can_ReturnType Can_Write (Can_HwHandleType Hth, const Can_PduType *PduInfo)
{
    uint8_t idx;
    CAN_MESSAGE message;
	
    for(idx = 0; idx < 8; idx++)
    {
		if(idx < PduInfo->length)
			message.data[idx] = PduInfo->sdu[idx];
		else
			message.data[idx] = 0x00u;
    }
	message.id = PduInfo->id;
    message.len = 8u;
    CAN1_SendFIFOMessage(&message);
	
    return CAN_OK;
	
}
void Can_MainFunction_Write(void)
{
	uint8_t stCAN1;
    stCAN1 = RSCAN0.TMSTS16.UINT8; //发送结果状态标志
    if(stCAN1  == 0x4) //发送中断已完成
    {
		RSCAN0.TMSTS16.UINT8 = 0x00;
		BT_TxSt = 1;
    }
}


void CAN1_RX_Task(void)
{
	CAN_MESSAGE  message;
	
	memset(&message, 0, sizeof(CAN_MESSAGE));
    CAN1_ReceiveMessage(&message);
	
	memcpy(&msg_get, &message, sizeof(CAN_MESSAGE));
	
	CAN_CanRxMsgFlag = 1;
	
	if(msg_get.id == 0x0151)
	{
	    CanNode = 1;
		firmware_update_request = 2;
	}
	
	if(msg_get.id == 0x18DA17F1)
	{
		firmware_update_request = 1;
		
	    BT_TpRxmsg ((uint8_t)CanTp_Rx_0x18DA17F1, msg_get.len, (uint8_t*)msg_get.data);
	}
	
}
void CAN2_RX_Task(void)
{
	CAN_MESSAGE  message;
	
	memset(&message, 0, sizeof(CAN_MESSAGE));
    CAN2_ReceiveMessage(&message);
	
	memcpy(&msg_get, &message, sizeof(CAN_MESSAGE));
	
	CAN_CanRxMsgFlag = 1;
	
	if(msg_get.id == 0x0151)
	{
	    CanNode = 2;
		firmware_update_request = 2;
	}
	
	
}


/** 校验函数，返回版本信息，停止自动跳转到APP **/
void ExecutiveCheckHandle(void)
{
    CAN_MESSAGE tx_msg;
    tx_msg.id = (BINGO_DIVER<<4) | OnlineCheckID;
    tx_msg.data[0] = VERSIONS_MAJOR>>8;
    tx_msg.data[1] = VERSIONS_MAJOR;
    tx_msg.data[2] = VERSIONS_MINOR>>8;
    tx_msg.data[3] = VERSIONS_MINOR;
    tx_msg.data[4] = COMPILE_YEAR;
    tx_msg.data[5] = COMPILE_MONTH;
    tx_msg.data[6] = COMPILE_DAY;
    tx_msg.data[7] = CAN_BL_BOOT;
    tx_msg.len = 8;
    if(CanNode == 1) CAN1_SendFIFOMessage(&tx_msg);
    else if(CanNode == 2) CAN2_SendFIFOMessage(&tx_msg);
    else;
}

/** 擦除flash,此处不执行 **/
void ExecutiveEraseFlashHandle(void)
{	
    CAN_MESSAGE tx_msg;
    tx_msg.id = CAN_TX_ID<<4;
    tx_msg.id |= EraseFlashID;
    tx_msg.len = 0;
    __DI();
    FCL_Erase(0x08, 118);
    __EI();
    if(CanNode == 1) CAN1_SendFIFOMessage(&tx_msg);
    else if(CanNode == 2) CAN2_SendFIFOMessage(&tx_msg);
    else;
	
}

/** 接收数据长度，用于计算是否接收完成一帧数据 **/
void ExecutiveBlockWriteInfoHandle(void)
{
    CAN_MESSAGE tx_msg;
    data_index = 0;
    data_size = msg_get.data[4];
    data_size <<=8;
    data_size = msg_get.data[5];
    data_size <<=8;
    data_size = msg_get.data[6]; //get receive data size
    data_size <<=8;
    data_size += msg_get.data[7];
    if(data_size < 0x3ff)
    {
		Last_Frame = 1;
    }
    tx_msg.id = (CAN_TX_ID<<4) | BlockWriteInfoID;
    tx_msg.len = 0;
    if(CanNode == 1) CAN1_SendFIFOMessage(&tx_msg);
    else if(CanNode == 2) CAN2_SendFIFOMessage(&tx_msg);
    else;
}

/** 接收数据并存入buffer **/
void ExecutiveWriteBlockFlashHandle(void)
{
    int i;
    uint16_t crc_data;
    uint8_t  crcH, crcL;
    uint16_t crc_data_received;
    if(data_index<data_size)         
    {
		for (i=0;i<msg_get.len;i++)
		{
			data_index++;
			if(data_index<=data_size)
			{
				rx_buf.buf[(rx_buf.w++)] = msg_get.data[i];
			}
		}
    }
    if(data_index>=data_size)
	{
		rx_buf.w = 0;
		crc_data = crc16_ccitt((unsigned char*)rx_buf.buf, data_size-2); //对接收到的数据做CRC校验，保证数据完整性
		crcH = rx_buf.buf[data_size-2];
		crcL = rx_buf.buf[data_size-1];
		crc_data_received = (crcH<<8) + crcL;
		if(crc_data == crc_data_received)
		{
			__DI();
			CanStop = 1;
		}
		else
		{
			// Communication_Error  = 1;
			user_address = APP_START_ADDR;

		}
    }
	
}

/** 检查app区是否有数据，有数据则开始执行app **/
void ExecutiveAppHandle(void)
{
    uint32_t *Src;
    uint32_t (* jr_app)(void);
	
    Src = (uint32_t *)(0x10000);
	if(Src[0] != 0 && Src[0] != 0xffffffff)
    {
		__DI();
		jr_app = ( uint32_t (*)() )0x10000;
		jr_app();
    }
}

/** 分支跳转控制 **/
void CAN_BOOT_ExecutiveCommand(void)
{
    if (CAN_CanRxMsgFlag)
	{
		switch(msg_get.id)
		{
			case ((BINGO_DIVER<<4) | OnlineCheckID):
			{
			   if(CanNode == 1)
			   {
				   RS_CAN_RECV_Enabled(CAN_CH1);
				   RS_CAN_RECV_Disabled(CAN_CH2);
			   }
			   else  if(CanNode == 2)
			   {
				   RS_CAN_RECV_Enabled(CAN_CH2);
				   RS_CAN_RECV_Disabled(CAN_CH1);
			   }
			   else;
			   ExecutiveCheckHandle();
			   break;			
			}  
			case ((BINGO_DIVER<<4) | EraseFlashID):
			{
				ExecutiveEraseFlashHandle();
				break;			
			}  
			case ((BINGO_DIVER<<4) | BlockWriteInfoID):
			{
				ExecutiveBlockWriteInfoHandle();
				break;			
			}  
			case ((BINGO_DIVER<<4) | WriteBlockFlashID):
			{
				ExecutiveWriteBlockFlashHandle();
				break;			
			}  
			case ((BINGO_DIVER<<4) | ExcuteAppID):
			{
				ExecutiveAppHandle();
				break;		
			}  
			
			default:break;
		}
		
		memset(&msg_get, 0, sizeof(CAN_MESSAGE));
		CAN_CanRxMsgFlag = 0;
		
    }
    else ;
	
}


#pragma ghs section text = ".R_FCL_CODE_USRINT"
/*
 * CAN2 interrupt service routine
 * Receive data from can2 port
 */
__interrupt void INTRCANGRECC(void)
{
	if((RSCAN0.RFSTS1.UINT32 & 0xFF00) > 0)
    {
		CAN1_RX_Task();
    }
	
    if((RSCAN0.RFSTS2.UINT32 & 0xFF00) > 0)
    {
		CAN2_RX_Task();
    }
	
}

