/*
============================================================================
 PROJECT: Example project
============================================================================
                           C O P Y R I G H T
============================================================================
 Copyright (c) 2015 by Renesas Electronics GmbH. All rights reserved.
 Arcadiastr. 10
 D-40472 Duesseldorf
 Germany
============================================================================

 Warranty disclaimer:
 Because the product(s) is licensed free of charge, there is no warranty of
 any kind whatsoever and expressly disclaimed and excluded by Renesas, either
 expressed or implied, including but not limited to those for non-
 infringement of intellectual property, merchantability and/or fitness for
 the particular purpose. Renesas shall not have any obligation to maintain,
 service or provide bug fixes for the supplied Product(s) and/or the
 Application.

 Each user is solely responsible for determining the appropriateness of
 using the product(s) and assumes all risks associated with its exercise
 of rights under this agreement, including, but not limited to the risks
 and costs of program errors, compliance with applicable laws, damage to
 or loss of data, programs or equipment, and unavailability or
 interruption of operations.

 Limitation of liability:
 In no event shall Renesas be liable to the user for any incidental,
 consequential, indirect, or punitive damage (including but not limited to
 lost profits) regardless of whether such liability is based on breach of
 contract, tort, strict liability, breach of warranties, failure of
 essential purpose or otherwise and even if advised of the possibility of
 such damages. Renesas shall not be liable for any services or products provided
 by third party vendors, developers or consultants identified or referred
 to the user by Renesas in connection with the product(s) and/or the
 application.

============================================================================
 Enviroment:  Devices:     RH850/D1M2H / R7F701412
              Debugger:    GHS Multi 2000
============================================================================

 GHS Multi example project for use with RH850/D1M2H application board
 (Y-RH850-D1X-MB-T1-V1, Y-RH850-D1M2H-PB-TET-V1) 

============================================================================
*/

#ifndef _UPGRADE_H_
#define _UPGRADE_H_



/******************************************************************************
 * Macro define
 */
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


#define BINGO_DIVER		(0x15U)
#define BIG_DOG_DIVER	(0x13U)
#define USER_DIVER		BINGO_DIVER

#define VERSIONS_MAJOR	(2u)
#define VERSIONS_MINOR	(1u)//1.1 set&&clear got to boot   1.2 set go to boot
// update 1.1: can 1 and can2 can also be used for app update
//2.0 do not use key to gointo boot
#define CAN_BL_BOOT	(0x0u)

#define CAN_TX_ID	(USER_DIVER)

#define APP_START_ADDR		0x10000


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


extern unsigned char Last_Frame;
extern volatile char CanStop;
extern uint8_t CanNode;


extern volatile uint32_t address;
extern unsigned char erased;



/*****************************************************************************
 * Function define
 */

void CAN1_RX_Task(void);
void CAN2_RX_Task(void);

extern void FCL_Erase(void);
extern void CAN_BOOT_ExecutiveCommand(void);
extern unsigned char CheckApp(void);







#endif

