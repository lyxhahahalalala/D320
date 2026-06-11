/*
============================================================================
 PROJECT: Can project
============================================================================
                           C O P Y R I G H T
============================================================================
 Copyright (c) 2017 by SHFY. All rights reserved.
 
============================================================================


============================================================================
 Enviroment:  Devices:     RH850/D1M1 / R7F701404
              Debugger:    GHS Multi 2000
============================================================================
*/

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_CAN_GRAMINIT_ON  (0x8UL)
#define R_CAN_GSLPSTS_ON   (0x4UL)
#define R_CAN_GRSTSTS_ON   (0x1UL)
#define R_CAN_CSLPSTS_ON   (0x4UL)
#define R_CAN_CHLTSTS_ON   (0x2UL)
#define R_CAN_CRSTSTS_ON   (0x1UL)
#define R_CAN_TMTRM_ON     (0x8U)
#define R_CAN_TMTR_ON      (0x1U)
#define R_CAN_AFLDAE_ON    (0x100UL)
#define R_CAN_GSLPR_MASK   (0x4UL)
#define R_CAN_GMDC_MASK    (0x3UL)
#define R_CAN_CSLPR_MASK   (0x4UL)
#define R_CAN_CHMDC_MASK   (0x3UL)

#define EraseFlashID        0x03//擦出APP储存扇区数据
#define WriteBlockFlashID   0x06//以多字节形式写数据
#define ReadBlockFlashID    0x08//以多字节形式读数据
#define BlockWriteInfoID    0x05//设置多字节写数据相关参数（写起始地址，数据量）
#define BlockReadInfoID     0x07//设置多字节数据读相关参数（读起始地址，数据量）
#define OnlineCheckID       0x01//检测节点是否在线
#define CmdSuccessID        0x0A//命令执行成功
#define CmdFaildID          0x00//命令执行失败
#define SetBaudRateID       0x04//设置节点波特率
#define ExcuteAppID         0x09//执行应用程序


#define BINGO_DIVER		(0x12U)
#define BIG_DOG_DIVER	(0x13U)
#define USER_DIVER		BINGO_DIVER

#define VERSIONS_MAJOR	(2u)
#define VERSIONS_MINOR	(4u)
#define VERSIONS	"RH850 boot V1.0 2017-10-12"
#define CAN_BL_BOOT	(0x0u)

#define CAN_TX_ID	(USER_DIVER)

#define APP_STAR_ADDR		0x20000
#define APP_MAX_ADDR		0x3F000
#define APP_CHECK_STRING	"CHECK OK"
#define APP_TIMEOUT			200

 volatile BOOL CAN_CanRxMsgFlag;//接收到CAN数据后的标志
 volatile  char TimeOutConfig;
 volatile  unsigned int RunTime;

volatile char CanStop;
typedef enum _ERROR_TYPE {
              noErr = 0,
              SRecRangeError = 1,      //S-Record Out Of Range
              SRecOddError = 2,        //S-Record Size Must Be Even
              FlashProgramError = 3,  //Flash Programming Error
              FlashEraseError = 4,    //Flash Erase Error
              BadHexData = 5,          //Bad Hex Data
              SRecTooLong = 6,        //S-Record Too Long
              CheckSumErr = 7,        //Checksum Error
              UnknownPartID = 8       //Unknown Part ID
            }ERROR_TYPE;
                            
#if (USER_DIVER == BIG_DOG_DIVER)
	#define CHECK_TYPE        "S12G128 Bigdog"
#elif (USER_DIVER == BINGO_DIVER)
	#define CHECK_TYPE        "S12G128 Bingo"
#endif

//
#pragma interrupt INTRCANGRECC(enable=true, callt=true, fpu=true)
//============================================================================
//
// Source code for the RS CAN Functions
//
//============================================================================

//============================================================================
// Includes
//============================================================================
#include "iodefine.h"
#include "rscan.h"
#include "can_table.h"
#include "r_typedefs.h"
#include "BSP.h"


//============================================================================
// Functions
//============================================================================
/*****************************************************************************
** Function:    RS_CAN_init
** Description: Configures the CAN0 and CAN1
** Parameter:   None
** Return:      None
******************************************************************************/
void RS_CAN_init(void)
{
    /* default seting: Main Osc/1 -> CAN */
    //protected_write(SYSPROTCMDD1,SYSPROTS1,SYSCKSC_IRSCANXINS_CTL,0x01);
    //while (SYSCKSC_IRSCANXINS_ACT !=0x01); //wait for clk active
    
    //set the pull down pin
    //PORT_AWO.PM0.UINT16               &= 0xfffe;
    //PORT_AWO.PMC0.UINT16              &= 0xfffe;
    //PORT_AWO.P0.UINT16                &= 0xfffe;

    /* Set CAN0TX as P0_4 and CAN0RX as P0_5 */
    PORT_AWO.PMC0.UINT16  |= 0x0030;
    PORT_AWO.PFCE0.UINT16 |= (0x0030);
    PORT_AWO.PFC0.UINT16  |= (0x0030);
    PORT_AWO.PM0.UINT16   &= ~(1 << 4);
    PORT_AWO.PM0.UINT16   |= 1 << 5;
	
	/* Set CAN1TX as P0_6 and CAN1RX as P0_7 */
    PORT_AWO.PMC0.UINT16  |= (0x00C0);
    PORT_AWO.PFCE0.UINT16 |= (0x00C0);
    PORT_AWO.PFC0.UINT16  |= (0x00C0);
    PORT_AWO.PM0.UINT16   &= ~(1 << 6);
    PORT_AWO.PM0.UINT16   |= 1 << 7;
	
	/* Set CAN2TX as P0_8 and CAN2RX as P0_9 */
	PORT_AWO.PMC0.UINT16  |= (0x0300);
    PORT_AWO.PFCE0.UINT16 |= (0x0300);
    PORT_AWO.PFC0.UINT16  |= (0x0300);
    PORT_AWO.PM0.UINT16   &= ~(1 << 8);
    PORT_AWO.PM0.UINT16   |= 1 << 9;
    
    /* Wait while CAN RAM initialization is ongoing */
    while((RSCAN0.GSTS.UINT32 & 0x00000008));

    /* Switch to global/channel reset mode */
    //RSCAN0.GCTR.UINT32  &= 0xfffffffb;	     //set the 3rd bit to 0 -- other than global stop mode
    //RSCAN0.C0CTR.UINT32 &= 0xfffffffb;
	
	RSCAN0.GCTR.UINT32  &= ~R_CAN_GSLPR_MASK;      /* GSLPR=0 */
	RSCAN0.C0CTR.UINT32 &= ~R_CAN_CSLPR_MASK;      /* CSLPR = 0B */
	RSCAN0.C1CTR.UINT32 &= ~R_CAN_CSLPR_MASK;      /* CSLPR = 0B */
	RSCAN0.C2CTR.UINT32 &= ~R_CAN_CSLPR_MASK;

    /* Configure clk_xincan as CAN-ClockSource */
    RSCAN0.GCFG.UINT32 = 0x00000010;
    
    /* When fCAN is 8MHz, Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 8/2/16 = 0.25Mbps(250Kbps) */
	RSCAN0.C0CFG.UINT32 = 0x023a0001;                 //SJW=3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1
	RSCAN0.C1CFG.UINT32 = 0x023a0001;                 //SJW=3TQ, TSEG1=11TQ, TSEG2=4TQ, BRP=1
	RSCAN0.C2CFG.UINT32 = 0x023a0001; 

    /* ==== Rx rule setting ==== */
    Can_SetRxRule();

    /* ====receive buffer setting ==== */
    RSCAN0.RMNB.UINT32 = 0x10;  //Can_SetGlobalBuffer--16
    
    /* Set THLEIE disabled, MEIE(FIFO Message Lost Interrupt disabled)  */
    RSCAN0.GCTR.UINT32 &= 0xfffff8ff;
	
	//FIFO SETTING
	RSCAN0.RFCC0.UINT32 = 0x00007102;
	RSCAN0.RFCC1.UINT32 = 0x00007102;
	RSCAN0.RFCC2.UINT32 = 0x00007102;

    /* If GlobalChannel in halt or reset mode */
    if(RSCAN0.GSTS.UINT32 & 0x03)
    {
        RSCAN0.GCTR.UINT32 &= 0xfffffffc; //Switch to communication mode
        while((RSCAN0.GSTS.UINT32 & 0x02) == 2)
        {
            ;/* While halt mode */
        }
        while((RSCAN0.GSTS.UINT32 & 0x01) == 1)
        {
            ;/* While reset mode */
        }
    }

    /* If Channel-0 in halt or reset mode */
    if(RSCAN0.C0STS.UINT32 & 0x03)
    {
        RSCAN0.C0CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while((RSCAN0.C0STS.UINT32 & 0x02) == 2)
        {
            ;/* While halt mode */
        }
        while((RSCAN0.C0STS.UINT32 & 0x01) == 1)
        {
            ;/* While reset mode */
        }
    }
	
	/* If Channel-1 in halt or reset mode */
    if(RSCAN0.C1STS.UINT32 & 0x03)
    {
        RSCAN0.C1CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while((RSCAN0.C1STS.UINT32 & 0x02) == 2)
        {
            ;/* While halt mode */
        }
        while((RSCAN0.C1STS.UINT32 & 0x01) == 1)
        {
            ;/* While reset mode */
        }
    }
	
	/* If Channel-2 in halt or reset mode */
    if(RSCAN0.C2STS.UINT32 & 0x03)
    {
        RSCAN0.C2CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while((RSCAN0.C2STS.UINT32 & 0x02) == 2)
        {
            ;/* While halt mode */
        }
        while((RSCAN0.C2STS.UINT32 & 0x01) == 1)
        {
            ;/* While reset mode */
        }
    }
	
	RSCAN0.RFCC0.UINT32 |= 0x01;
	RSCAN0.RFCC1.UINT32 |= 0x01;
	RSCAN0.RFCC2.UINT32 |= 0x01;
	
	PBG.FSGD0BPROT0.UINT32 = 0x07FFFFFF;
       PBG.FSGD0BPROT1.UINT32 = 0x07FFFFFF;
	INTC2.EIC70.UINT16 = 0x0047u;
}

/******************************************************************************
** Function:    Can_SetRxRule
** Description: Set Rx rules for CAN0 and CAN1 and CAN2
** Parameter:   None
** Return:       None
******************************************************************************/
static void Can_SetRxRule(void)
{
    /* Set Rx rule number per channel */
    RSCAN0.GAFLCFG0.UINT32 |= 0x06060600;   //Channel-0 rules number is 6 (this register is only for the rules' number setting.)

    /* Receive Rule Table Write Enable, and select page 0
    RSCAN0GAFLECTR- Receive Rule Entry Control Register
    b31:b 9                  - Reserved set to 0
    b 8             AFLDAE   - Receive Rule Table Write Enable     - Receive rule table write is enabled. Set to 1
    b 7:b 5                  - Reserved set to 0
    b 4:b 0         AFLPN    - Receive Rule Table Page Number
                               Configuration                       - select pages 0. set to 00000'b */
    RSCAN0.GAFLECTR.UINT32   = R_CAN_AFLDAE_ON;

    /* Setting of the reception rule ID
    RSCAN0GAFLIDj - Receive Rule ID Register 0
    b31             GAFLIDE  - IDE Select                          - Standard ID. set to 0
    b30             GAFLRTR  - RTR Select                          - Data frame. set to 0
    b29             GAFLLB   - Receive Rule Target Message Select  - 0: When a message transmitted from another CAN node is received.
                                                                     1: When the own transmitted message is received.
    b28:b 0         GAFLID   - ID of the receive rule              - reception rule ID 0. set to 0 */
    RSCAN0.GAFLID0.UINT32      = 0x00000000UL; /* Standard ID, Data frame */
	RSCAN0.GAFLID6.UINT32      = 0x80000000UL;
	RSCAN0.GAFLID12.UINT32     = 0x80000000UL;

    /* Setting of the reception rule mask
    RSCAN0GAFLMj  - Receive Rule Mask Register 0
    b31             GAFLIDEM - IDE Mask                            - The IDE bit is compared. set to 1
    b30             GAFLRTRM - RTR Mask                            - The RTR bit is compared. set to 1
    b29                      - Reserved set to 0
    b28:b 0         GAFLIDM  - ID Mask                             - not compare the ID bit. set to 0 */
    RSCAN0.GAFLM0.UINT32      = 0x00000000UL; /* Consider IDE and RTR */
	RSCAN0.GAFLM6.UINT32      = 0x00000000UL;
	RSCAN0.GAFLM12.UINT32     = 0x00000000UL;

    /*Set RSCAN0GAFLP0j register.
    RSCAN0GAFLP0j - Receive Rule Pointer 0 Register 0
    b31:b28         GAFLDLC  - Receive Rule DLC                    - DLC check is disabled. set to 0000'b
    b27:b16         GAFLPTR  - Receive Rule Label                  - Rule Label 0. set to 000000000000'b
    b15             GAFLRMV  - Receive Buffer Enable               - receive buffer is used. set to 1
    b14:b 8         GAFLRMDP - Receive Buffer Number Select        - receive buffer number to store receive messages.
                                                                     set to 0000000'b
    b 7:b 0                  - Reserved set to 0 */
    //RSCAN0.GAFLP00.UINT32    = 0x00008000UL; /* No DLC check, use Rx buffer 0 */
	//RSCAN0.GAFLP06.UINT32    = 0x00008100UL;
	RSCAN0.GAFLP00.UINT32     = 0x00000000UL;
	RSCAN0.GAFLP06.UINT32     = 0x00000000UL;
	RSCAN0.GAFLP012.UINT32    = 0x00000000UL;

    /*Set RSCAN0GAFLP1j register.
    RSCAN0GAFLP1j - Receive Rule Pointer 1 Register 0
    b31:b26                  - Reserved set to 0
    b25:b 8         GAFLFDP  - Tx/Rx FIFO Buffer k Select          - not use FIFO. set to 0000000000'b
    b 7:b 0         GAFLFDP  - Receive FIFO Buffer x Select        - not use FIFO. set to 000000000000000'b */
    //RSCAN0.GAFLP10.UINT32    = 0x00000000UL; /* not use FIFO */
	//RSCAN0.GAFLP16.UINT32    = 0x00000000UL;
	RSCAN0.GAFLP10.UINT32     = 0x00000001UL;
	RSCAN0.GAFLP16.UINT32     = 0x00000002UL;
	RSCAN0.GAFLP112.UINT32    = 0x00000004UL;

    /* Receive Rule Table Write Disabel
    RSCAN0GAFLECTR- Receive Rule Entry Control Register
    b31:b 9                  - Reserved set to 0
    b 8             AFLDAE   - Receive Rule Table Write Enable     - Receive rule table write is disabled. Set to 0
    b 7:b 5                  - Reserved set to 0
    b 4:b 0         AFLPN    - Receive Rule Table Page Number
                               Configuration                       - unused. set to 00000'b */
    RSCAN0.GAFLECTR.UINT32   = 0x00000000UL;
}

Can_RtnType Can_C0TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame)
{
    VU8* pTBSR;
    Can_FrameType* pTxBuffer;
    VU8* pTBCR;

    pTBSR = &(RSCAN0.TMSTS0.UINT8);
    pTBCR = &(RSCAN0.TMC0.UINT8);

    /* ---- Return if Tx Buffer is transmitting. ---- */    
    if((pTBSR[TxBufIdx] & (VU8)0x01) == CAN_TBTST_TRANSMITTING)
    {
        return CAN_RTN_ERR;
    }

    /* Clear Tx buffer status */
    do 
    {
        pTBSR[TxBufIdx] = CAN_CLR;
    }
	while(pTBSR[TxBufIdx] != CAN_CLR);

    /* Store message to tx buffer */
    pTxBuffer = (Can_FrameType*) &(RSCAN0.TMID0.UINT32);
    pTxBuffer[TxBufIdx] = *pFrame;

    /* Set transmission request */
    pTBCR[TxBufIdx] = CAN_TBCR_TRM;

    return CAN_RTN_OK;
}

    /* Create Can_FrameType for send and receive data */
    const Can_FrameType CANTraStandData={
    //CiTBpA
    0x18,
    0,
    0,
    0,        
    
    //CiTBpB
    0x0000,                            
    0x000,                            
    0x8,    
    
    {
    0x12,                            //DB0
    0x34,                            //DB1
    0x56,                            //DB2
    0x78,                            //DB3
    //CiTBpD
    0x87,                            //DB4
    0x65,                            //DB5
    0x43,                            //DB6
    0x21                             //DB7
    }
    };
    
    Can_FrameType CANRecData={
    //CiTBpA
    0x00,
    0,
    0,
    0,
    
    //CiTBpB
    0x0000,                                
    0x000,                            
    0x0,                        

    //CiTBpC
    {
    0x00,                            //DB0
    0x00,                            //DB1
    0x00,                            //DB2
    0x00,                            //DB3
    //CiTBpD
    0x00,                            //DB4
    0x00,                            //DB5
    0x00,                            //DB6
    0x00                             //DB7
    }
	};


/******************************************************************************
* Function Name : CAN2_SendMessage
* Description   : This function sends the message by Tx buffer 0.
* Argument      : message -
*                     pointer to stored data
******************************************************************************/
void CAN2_SendMessage(CAN_MESSAGE *message)
{
	int iTimeOut = 0;
    /* Check the Transmit Buffer Transmit Request Status
    RSCAN0TMSTSp  - Transmit Buffer Status Register 0
    b7:b5                    - Reserved set to 0
    b4:             TMTARM   - Transmit Buffer Transmit Abort Request Status Flag
    b3:             TMTRM    - Transmit Buffer Transmit Request Status Flag
    b2:b1           TMTRF    - Transmit Buffer Transmit Result Status Flag
    b0:             TMTSTS   - Transmit Buffer Transmit Status Flag */    
    if((RSCAN0.TMSTS32.UINT8 & R_CAN_TMTRM_ON) == 0x0U)
    {
        /* Clear status. */
        RSCAN0.TMSTS32.UINT8           = 0x00U;

        /* Set message data. */
        /* Set RSCAN0TMIDp register.
        RSCAN0TMIDp   - Transmit Buffer ID Register 0
        b31             TMIDE   - Transmit Buffer IDE                - Standard ID. set to 0
        b30             TMRTR   - Transmit Buffer RTR                - Data frame. set to 0
        b29             THLEN   - Transmit History Data Store Enable - Transmit history data is not stored
                                                                       in the buffer. set to 0
        b28:b0          TMID    - Transmit Buffer ID Data */
		RSCAN0.TMID32.UINT32     = (message->id & 0x1FFFFFFF);
		RSCAN0.TMID32.UINT32     |= 0x80000000;                   //Ext

        /* Set RSCAN0TMIDp register.
        RSCAN0TMPTRp  - Transmit Buffer Pointer Register 0
        b31:b28         TMDLC   - Transmit Buffer DLC Data
        b27:b24                 - Reserved set to 0
        b23:b16         TMPTR   - Transmit Buffer Label Data
        b15:b 0                 - Reserved set to 0  */
        RSCAN0.TMPTR32.UINT32    = (message->len & 0xF) << 28;

        /* Set RSCAN0TMDF0p register.
        RSCAN0TMDF0p  - Transmit Buffer Data Field 0 Register 0
        b31:b24         TMDB3   - Transmit Buffer Data Byte 3
        b23:b16         TMDB2   - Transmit Buffer Data Byte 2
        b15:b 8         TMDB1   - Transmit Buffer Data Byte 1
        b 7:b 0         TMDB0   - Transmit Buffer Data Byte 0 */
        RSCAN0.TMDF032.UINT32    = *((uint32_t *)&(message->data[0]));

        /* Set RSCAN0TMDF1p register.
        RSCAN0TMDF1p  - Transmit Buffer Data Field 0 Register 1
        b31:b24         TMDB7   - Transmit Buffer Data Byte 7
        b23:b16         TMDB6   - Transmit Buffer Data Byte 6
        b15:b 8         TMDB5   - Transmit Buffer Data Byte 5
        b 7:b 0         TMDB4   - Transmit Buffer Data Byte 4 */
        RSCAN0.TMDF132.UINT32    = *((uint32_t *)&(message->data[4]));

        /* Set transmission request.
        RSCAN0TMCp    - Transmit Buffer Control Register 0
        b 7:b 3                 - Reserved set to 0
        b 2             TMOM    - One-Shot Transmission Enable       - unused
        b 1             TMTAR   - Transmit Abort Request             - unused
        b 0             TMTR    - Transmit Request                   - Transmission is requested. set to 1 */
        RSCAN0.TMC32.UINT8             = R_CAN_TMTR_ON;

        /* Normal completion or time out. */
        while((RSCAN0.TMSTS32.UINT8 & 0x4) != 0x4)if(iTimeOut++ > 50000)break;
    } 
}


/******************************************************************************
* Function Name : CAN2_ReceiveMessage
* Description   : This function receives the message by Rx fifo buffer 2.
* Argument      : message -
*                     pointer to stored data
******************************************************************************/
void CAN2_ReceiveMessage(CAN_MESSAGE *message)
{
	message->id  = RSCAN0.RFID2.UINT32 & 0x1FFFFFFF;
	message->len = RSCAN0.RFPTR2.UINT32 >> 28;
	
	*((int*)&(message->data[0]))  = RSCAN0.RFDF02.UINT32;
	*((int*)&(message->data[4]))  = RSCAN0.RFDF12.UINT32;
	memcpy(&msg_get,&message,sizeof(CAN_MESSAGE));
	memset(&message,0,sizeof(CAN_MESSAGE));
	
	RSCAN0.RFPCTR2.UINT32 = 0xFF;
	
	if((RSCAN0.RFSTS2.UINT32 & 0xFF00) == 0)
	{
		RSCAN0.RFSTS2.UINT32 &= 0xFFF7;
	}
}

#define BUFFER_SIZE		0x400
typedef struct 
{
	char buf[BUFFER_SIZE];
	int r;
	int w;
}
CAN_DATA_FIFO;

typedef void (*fCommandHandle)(CAN_MESSAGE *pRxMessage);
typedef struct _ExecutiveCommandType
{
	unsigned int ID;
	fCommandHandle fHandle;
}
ExecutiveCommandType;

volatile CAN_DATA_FIFO rx_buf;
void CAN_BOOT_ExecutiveCommandHandle(CAN_MESSAGE *pRxMessage,ExecutiveCommandType *Table,int size)
{
	int i;
	 
	for (i = 0; i<size; i++)
	{
		if ((pRxMessage.id&0x0f) == Table[i].ID)
		{
			Table[i].fHandle(pRxMessage);
			return ;
		}
	}
}
/**
 * 校验函数，返回版本信息，停止自动跳转到APP
 */
void ExecutiveCheckHandle(CAN_MESSAGE *pRxMessage)
{
	CAN_MESSAGE tx_msg;
	//TIE   = 0;//关定时中断取消计时
	tx_msg.id = (0x12<<4) | OnlineCheckID;
	tx_msg.data[0] = VERSIONS_MAJOR>>8;
	tx_msg.data[1] = VERSIONS_MAJOR;
	tx_msg.data[2] = VERSIONS_MINOR>>8;
	tx_msg.data[3] = VERSIONS_MINOR;
	tx_msg.data[4] = 0;
	tx_msg.data[5] = 0;
	tx_msg.data[6] = 0;
	tx_msg.data[7] = CAN_BL_BOOT;
	tx_msg.len = 8;
	CAN2_SendMessage(tx_msg);
}
/**
 * 接收过程不执行烧写防止中断影响flash操作
 */
int IsRxEmpty(void)
{
//  _FEED_COP();
	if (CanStop == 0){
	return 1;
	}
	return rx_buf.w == rx_buf.r;
}
uint8_t Getchar(void)
{
	CAN_MESSAGE tx_msg;
	while(IsRxEmpty())
	{
		if (1 == CanStop){
			tx_msg.id = (CAN_TX_ID<<4) | WriteBlockFlashID;
			tx_msg.len = 0;
			(void)CAN2_SendMessage(tx_msg);
			CanStop = 0;
		}
		CAN_BOOT_ExecutiveCommand();
	}
	return rx_buf.buf[(rx_buf.r++)&(BUFFER_SIZE-1)];
}

void GetRxBuffer(unsigned  char * buffer, int len)
{
	int i;
	i= 0;
	while(i<len) 
	{
		buffer[i++] = Getchar();
	}
}

void ExecutiveEraseFlashHandle(CAN_MESSAGE *pRxMessage)
{
	//!<擦除过程中关闭中断,防止程序异常;
	unsigned long addr;
	ERROR_TYPE err = EraseFlashID;
	CAN_MESSAGE tx_msg;
	tx_msg.id = CAN_TX_ID<<4;
	tx_msg.id |= EraseFlashID;
	tx_msg.len = 0;
	__DI();
	for (addr = APP_STAR_ADDR;addr<APP_MAX_ADDR;){
		//err = PFlash_EraseSector(addr);
		if(err != noErr)
			break;
		addr += 0x200;
		//_FEED_COP();
	}
	__EI();
	CAN2_SendMessage(tx_msg);
	
}
static unsigned long data_index = 0,data_size = 0;
void ExecutiveBlockWriteInfoHandle(CAN_MESSAGE *pRxMessage)
{
	CAN_MESSAGE tx_msg;
	data_index = 0;
	data_size = pRxMessage->data[4];
	data_size <<=8;
	data_size = pRxMessage->data[5];
	data_size <<=8;
	data_size = pRxMessage->data[6];
	data_size <<=8;
	data_size += pRxMessage->data[7];
	tx_msg.id = (CAN_TX_ID<<4) | BlockWriteInfoID;
	tx_msg.len = 0;
	CAN2_SendMessage(tx_msg);
}
void ExecutiveWriteBlockFlashHandle(CAN_MESSAGE *pRxMessage)
{
	int i;
	char len;
	len = pRxMessage->len;
	
	if(data_index<data_size)         
	{
		for (i=0;i<pRxMessage->len;i++){
			data_index++;
			if(data_index+2<=data_size){
				rx_buf.buf[(rx_buf.w++) & (BUFFER_SIZE-1)] = pRxMessage->data[i];
			}
		}
	}
	if(data_index>=data_size){
		CanStop = 1;
	}
}

uint8_t CheckApp(void)
{
	uint8_t   out;
	uint8_t *Src;
	int i;
	unsigned char tmp[8];
	Src = (uint8_t *)0xEE00;
	for(i=0;i<8;i++){
		tmp[i] = Src[i];
	}
	
	out = strncmp(APP_CHECK_STRING,Src,8);
	return out;
}
void ExecutiveAppHandle(CAN_MESSAGE *pRxMessage) {
	int8_t tmp;
	tmp = CheckApp();
	if (tmp == 0){
    // DisableInterrupts;
	//  asm call 0xC000,00;
	}
}
static ExecutiveCommandType commandTable[]=
{
	{OnlineCheckID,ExecutiveCheckHandle},
	{EraseFlashID,ExecutiveEraseFlashHandle},
	{BlockWriteInfoID,ExecutiveBlockWriteInfoHandle},
	{WriteBlockFlashID,ExecutiveWriteBlockFlashHandle},
	{ExcuteAppID,ExecutiveAppHandle},
};

void CAN_BOOT_ExecutiveCommand(void)
{
	if (CAN_CanRxMsgFlag){
		CAN_BOOT_ExecutiveCommandHandle(&msg_get,commandTable,sizeof(commandTable)/sizeof(ExecutiveCommandType));
		CAN_CanRxMsgFlag = 0;
	}
}





