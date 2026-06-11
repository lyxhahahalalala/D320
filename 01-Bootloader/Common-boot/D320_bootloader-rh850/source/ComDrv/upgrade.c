/******************************************************************************
 * Includes
 */
#include "iodefine.h"
#include "r_typedefs.h"
#include "rscan.h"
#include "stdio.h"
#include "string.h"
#include "crc16.h"
#include "upgrade.h"



volatile uint32_t address;
uint8_t erased = 0;

uint8_t Last_Frame = 0;
uint8_t CanStop = 0;
uint8_t CanNode = 0;
CAN_DATA_FIFO rx_buf;

static CAN_MESSAGE  msg_get;
static uint32_t CAN_RxId = 0;
static uint8_t  CAN_CanRxMsgFlag = 0; //接收到CAN数据后的标志
static uint8_t  Communication_Error = 0;
static unsigned long data_index = 0;
static unsigned long data_size = 0;


void Can_Init(void)
{
    RS_CAN_init();
}


void CAN1_RX_Task(void)
{
	CAN_MESSAGE  message;
	
	memset(&message, 0, sizeof(CAN_MESSAGE));
    CAN1_ReceiveMessage(&message);
	
	CAN_RxId = message.id;
	if(CAN_RxId == 0x0151) CanNode = 1;
	memcpy(&msg_get, &message, sizeof(CAN_MESSAGE));
	
	CAN_CanRxMsgFlag = 1;
	
	
}

void CAN2_RX_Task(void)
{
	CAN_MESSAGE  message;
	
	memset(&message, 0, sizeof(CAN_MESSAGE));
    CAN2_ReceiveMessage(&message);
	
	CAN_RxId = message.id;
	if(CAN_RxId == 0x0151) CanNode = 2;
	memcpy(&msg_get, &message, sizeof(CAN_MESSAGE));
	
	CAN_CanRxMsgFlag = 1;
	
}


/**
 * 校验函数，返回版本信息，停止自动跳转到APP
 */
void ExecutiveCheckHandle(void)
{
    CAN_MESSAGE tx_msg;
    tx_msg.id = (BINGO_DIVER<<4) | OnlineCheckID;
    tx_msg.data[0] = VERSIONS_MAJOR>>8;
    tx_msg.data[1] = VERSIONS_MAJOR;
    tx_msg.data[2] = VERSIONS_MINOR>>8;
    tx_msg.data[3] = VERSIONS_MINOR;
    tx_msg.data[4] = 0;
    tx_msg.data[5] = 0;
    tx_msg.data[6] = 0;
    tx_msg.data[7] = CAN_BL_BOOT;
    tx_msg.len = 8;
    if(CanNode == 1) CAN1_SendMessage(&tx_msg);
    else if(CanNode == 2) CAN2_SendMessage(&tx_msg);
    else;
}

/**
 * 擦除flash,此处不执行
 */
void ExecutiveEraseFlashHandle(void)
{	
    CAN_MESSAGE tx_msg;
    tx_msg.id = CAN_TX_ID<<4;
    tx_msg.id |= EraseFlashID;
    tx_msg.len = 0;
    __DI();
    FCL_Erase(0x08, 118);
    __EI();
    if(CanNode == 1) CAN1_SendMessage(&tx_msg);
    else if(CanNode == 2) CAN2_SendMessage(&tx_msg);
    else;
	
}

/**
 * 接收数据长度，用于计算是否接收完成一帧数据
 */
void ExecutiveBlockWriteInfoHandle(void)
{
    CAN_MESSAGE tx_msg;
    data_index = 0;
    data_size = msg_get.data[4];
    data_size <<=8;
    data_size = msg_get.data[5];
    data_size <<=8;
    data_size = msg_get.data[6];//get receive data size
    data_size <<=8;
    data_size += msg_get.data[7];
    if(data_size<0x3ff)
    {
		Last_Frame = 1;
    }
    tx_msg.id = (CAN_TX_ID<<4) | BlockWriteInfoID;
    tx_msg.len = 0;
    if(CanNode == 1) CAN1_SendMessage(&tx_msg);
    else if(CanNode == 2) CAN2_SendMessage(&tx_msg);
    else;
}

/**
 *接收数据并存入buffer
 */
void ExecutiveWriteBlockFlashHandle(void)
{
    int i;
    uint16_t crc_data;
    uint8_t crcH,crcL;
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
		crc_data = crc16_ccitt(rx_buf.buf, data_size-2); //对接收到的数据做CRC校验，保证数据完整性
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
			Communication_Error  = 1;
			erased = 0;
			address = 0x10000;

		}
    }
	
}

/**
 * 检查app区是否有数据，有数据正常返回，无数据返回1
 */
uint8_t CheckApp(void)
{
    uint8_t *Src;
    int i;
    unsigned char tmp[8];
    Src = (uint8_t *)0x10000;
    for(i=0;i<8;i++){
	tmp[i] = Src[i];
	if(tmp[i]  != 0) return 0;
    }

    return 1;
}

/**
 * 检查app区是否有数据，有数据则开始执行app
 */
void ExecutiveAppHandle(void)
{
	//excute f/w jump, first check f/w information, then execute this function
    int8_t tmp;
    uint32_t (* jr_app)(void);
    tmp = CheckApp();   
    if (tmp == 0)
    {
		__DI();
		jr_app= ( uint32_t (*)() )0x10000;
		jr_app();
		//  asm call 0xC000,00;
    }
	
}

/**
 * 分支跳转控制
 */
void CAN_BOOT_ExecutiveCommand(void)
{
    if (CAN_CanRxMsgFlag)
	{
		switch(CAN_RxId)
		{
			case ((BINGO_DIVER<<4) | OnlineCheckID):
			{
			   if(CanNode == 1)
			   {
				   RSCAN0.RFCC2.UINT32 &= ~0x01;
				   RSCAN0.RFCC1.UINT32 |= 0x01;
			   }
			   else  if(CanNode == 2)
			   {
				   RSCAN0.RFCC1.UINT32 &= ~0x01;
				   RSCAN0.RFCC2.UINT32 |= 0x01;
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
		
		memset(&msg_get,0,sizeof(CAN_MESSAGE));
		CAN_CanRxMsgFlag = 0;
		CAN_RxId = 0;
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

