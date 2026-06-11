#ifndef _UPGRADE_H_
#define _UPGRADE_H_



/******************************************************************************
 * Macro define
 */
#define OnlineCheckID       0x01//检测节点是否在线
#define EraseFlashID        0x03//擦出APP储存扇区数据
// #define SetBaudRateID       0x04//设置节点波特率
#define BlockWriteInfoID    0x05//设置多字节写数据相关参数（写起始地址，数据量）
#define WriteBlockFlashID   0x06//以多字节形式写数据
// #define BlockReadInfoID     0x07//设置多字节数据读相关参数（读起始地址，数据量）
// #define ReadBlockFlashID    0x08//以多字节形式读数据
#define ExcuteAppID         0x09//执行应用程序
// #define CmdSuccessID        0x0A//命令执行成功
// #define CmdFaildID          0x00//命令执行失败


#define BINGO_DIVER		(0x15U)
#define USER_DIVER		 BINGO_DIVER

#define CAN_TX_ID	    (USER_DIVER)
#define APP_START_ADDR   0x10000


#define VERSIONS_MAJOR	(4u)
#define VERSIONS_MINOR	(5u)
#define CAN_BL_BOOT	    (0x0u)
#define COMPILE_DATE    (251016)
#define COMPILE_YEAR    (COMPILE_DATE/10000)
#define COMPILE_MONTH   (COMPILE_DATE%10000/100)
#define COMPILE_DAY     (COMPILE_DATE%100)

#define BCD(dec)  (uint8_t)((dec/10)*16 + dec%10)

#define CanTp_Tx_0x18DAF117     0
#define	CanTp_Rx_0x18DA17FF     1
#define CanTp_Rx_0x18DA17F1     2


/*******************************************************************************
 * TYPEDEFS
 */

typedef enum _ERROR_TYPE {
	noErr = 0,
	SRecRangeError = 1,      //S-Record Out Of Range
	SRecOddError = 2,        //S-Record Size Must Be Even
	FlashProgramError = 3,   //Flash Programming Error
	FlashEraseError = 4,     //Flash Erase Error
	BadHexData = 5,          //Bad Hex Data
	SRecTooLong = 6,         //S-Record Too Long
	CheckSumErr = 7,         //Checksum Error
	UnknownPartID = 8        //Unknown Part ID
}ERROR_TYPE;


#define BUFFER_SIZE		0x402
typedef struct 
{
	char buf[BUFFER_SIZE];
	int r;
	int w;
}CAN_DATA_FIFO;
extern CAN_DATA_FIFO  rx_buf;


typedef enum
{
    CAN_OK,
    CAN_NOT_OK,
    CAN_BUSY
	
}Can_ReturnType;

typedef uint8_t Can_HwHandleType;
typedef uint32_t Can_IdType;
typedef uint16_t PduIdType;
typedef uint16_t PduLengthType;

typedef struct
{
    uint8_t* sdu;
    Can_IdType id;
    PduIdType  swPduHandle;
    uint8_t  length;

}Can_PduType;


extern uint8_t Last_Frame;
extern uint8_t CanStop;
extern uint8_t CanNode;

extern volatile uint32_t user_address;

extern const uint32_t* ReflashMarker;

extern uint8_t firmware_update_request;


/*****************************************************************************
 * Function define
 */

void Can_Init(void);
Can_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType *PduInfo);
void Can_MainFunction_Write(void);
void CAN_BOOT_ExecutiveCommand(void);




#endif



