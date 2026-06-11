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


//============================================================================
//
// Header file for the RS CAN
//
//============================================================================
#ifndef _RSCAN_H_
#define _RSCAN_H_


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


#define BINGO_DIVER		(0x15U)
#define BIG_DOG_DIVER	(0x13U)
#define USER_DIVER		BINGO_DIVER

#define VERSIONS_MAJOR	(2u)
#define VERSIONS_MINOR	(1u)//1.1 set&&clear got to boot   1.2 set go to boot
// update 1.1: can 1 and can2 can also be used for app update
//2.0 do not use key to gointo boot
#define CAN_BL_BOOT	(0x0u)

#define CAN_TX_ID	(USER_DIVER)

#define APP_STAR_ADDR		0x10000


typedef struct 
{
    volatile unsigned long id;
    volatile unsigned char data[8];
    volatile unsigned char len;
    volatile unsigned char type;
} CAN_MESSAGE;

  

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


#if COMPILER == COMP_GHS
  #if 1
  /* ==== basic type ==== */
  typedef unsigned char U8;
  typedef signed char S8;
  typedef unsigned short U16;
  typedef signed short S16;
  typedef unsigned long U32;
  typedef signed long S32;
  typedef unsigned char BOOL;
  typedef volatile unsigned char VU8;
  typedef volatile unsigned short VU16;
  typedef volatile unsigned long VU32;
  #endif
#endif


/*
*******************************************************************************
**  Register bit define
*******************************************************************************
*/

/* ---- CAN frame ----- */
typedef struct
{
    U32 ID :29;
    U32 THLEN :1;
    U32 RTR :1;
    U32 IDE :1;
    
    U32 TS :16;
    U32 LBL :12;
    U32 DLC :4;
    U8 DB[8];
} Can_FrameType;

/*---- structure for Tx Frame information --------------------*/
/* ---- CAN function return ---- */
typedef U8 Can_RtnType;

/* ---- function return value ---- */
#define CAN_RTN_OK                          0U
#define CAN_RTN_FIFO_FULL                   1U
#define CAN_RTN_BUFFER_EMPTY                2U
#define CAN_RTN_ERR                         255U


/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#ifndef CAN_ENABLE
#define CAN_ENABLE                          1U
#endif
#ifndef CAN_DISABLE
#define CAN_DISABLE                         0U
#endif

/* ---- bit value ---- */
#define CAN_SET                             1U
#define CAN_CLR                             0U

/* ---- bit mask ---- */
#define CAN_1_BIT_MASK                      1U
#define CAN_2_BIT_MASK                      3U
#define CAN_3_BIT_MASK                      7U
#define CAN_4_BIT_MASK                      0xfU
#define CAN_5_BIT_MASK                      0x1fU

/* ---- bit position ---- */
#define CAN_B0_BIT_POS                      0U
#define CAN_B1_BIT_POS                      1U
#define CAN_B2_BIT_POS                      2U
#define CAN_B3_BIT_POS                      3U
#define CAN_B4_BIT_POS                      4U
#define CAN_B5_BIT_POS                      5U
#define CAN_B6_BIT_POS                      6U
#define CAN_B7_BIT_POS                      7U
#define CAN_B8_BIT_POS                      8U
#define CAN_B9_BIT_POS                      9U
#define CAN_B10_BIT_POS                     10U
#define CAN_B11_BIT_POS                     11U
#define CAN_B12_BIT_POS                     12U
#define CAN_B13_BIT_POS                     13U
#define CAN_B14_BIT_POS                     14U
#define CAN_B15_BIT_POS                     15U

/* **** CAN channel usage **** */
#define RS_CAN_CH3                 CAN_ENABLE  //CAN_DISABLE

#define RS_CAN_CH4                 CAN_ENABLE

/* ---- CAN resource ---- */
/*#define CAN_CHNL_NUM                        3U
#define CAN_RFIFO_NUM                       8U
#define CAN_CHNL_TRFIFO_NUM                 3U
#define CAN_TRFIFO_NUM                      (CAN_CHNL_TRFIFO_NUM * CAN_CHNL_NUM)
#define CAN_FIFO_NUM                        (CAN_RFIFO_NUM + CAN_TRFIFO_NUM)
#define CAN_CHNL_TX_BUF_NUM                 16U
*/
#define CAN_CRBRCF0_RX_BUF_NUM              32U
#define CAN_CRBRCF1_RX_BUF_NUM              32U
#define CAN_CRBRCF2_RX_BUF_NUM              32U
#define CAN_PAGE_RX_RULE_IDX_MASK           0xfU
#define CAN_RX_RULE_PAGE_IDX_BIT_POS        4U
#define CAN_RAM_LW_SIZE                     (0x1C20U >> 2U)
#define CAN_RAM_PAGE_IDX_BIT_POS            6U
#define CAN_RAM_DATA_IDX_MASK               0x3fU

#define RX_RULE_NUM_MAX              (192U)       /* Max Rx Rule number */
#define RX_RULE_PAGE_NUM             (1U)         /* Rx Rule Table page number */


/* ==== Rx rule table (Refer to can_table.h) ==== */
#define CAN_C0RN                       RX_RULE_NUM_CH0
#define CAN_C1RN                       RX_RULE_NUM_CH1
#define CAN_C2RN                       RX_RULE_NUM_CH2
#define CAN_C3RN                       RX_RULE_NUM_CH3
#define CAN_C4RN                       RX_RULE_NUM_CH4
#define CAN_RX_RULE_NUM                (CAN_C0RN + CAN_C1RN + CAN_C2RN + CAN_C3RN + CAN_C4RN)
#define CAN_RX_RULE_TABLE              RX_RULE_TABLE_LIST
#define CAN_RX_RULE_TYPE               can_cre_type

/* ---- TX buffer, TX status flag ---- */
#define CAN_TBTST_NOT_TRANSMITTING          0U
#define CAN_TBTST_TRANSMITTING              1U

/* ---- bit operations ---- */
#define GET_BIT(reg, pos)              (((reg) >> (pos)) & 1U)
#define SET_BIT(reg, pos)              ((reg) |= 1U << (pos))
#define CLR_BIT(reg, pos)              ((reg) &= ~(1UL << (pos)))

/* ---- CiTBCRj ---- */
#define CAN_TBTR_BIT_POS                    0U
#define CAN_TBAR_BIT_POS                    1U
#define CAN_TBOE_BIT_POS                    2U

/* transmission command */
#define CAN_TBCR_TRM                        (CAN_ENABLE << CAN_TBTR_BIT_POS)


#define BUFFER_SIZE		0x402
typedef struct 
{
	char buf[BUFFER_SIZE];
	int r;
	int w;
}
CAN_DATA_FIFO;
/*
*******************************************************************************
**  Function define
******************************************************************************/

void RS_CAN_init(void);
//void CAN_BOOT_ExecutiveCommand(void);
extern void FCL_Erase(void);
extern volatile char CanStop;
extern void CAN_BOOT_ExecutiveCommand(void);
extern unsigned char CheckApp(void);


extern CAN_DATA_FIFO rx_buf;
Can_RtnType Can_ReadRxBuffer(Can_FrameType* pFrame);
Can_RtnType Can_C0TrmByTxBuf(U8 TxBufIdx, const Can_FrameType* pFrame);

static void Can_SetRxRule(void);
void CAN2_ReceiveMessage(CAN_MESSAGE *message);
void CAN2_SendMessage(CAN_MESSAGE *message);

void CAN1_ReceiveMessage(CAN_MESSAGE *message);
void CAN1_SendMessage(CAN_MESSAGE *message);


extern  CAN_MESSAGE  message_Rec;
extern volatile unsigned long address;
extern unsigned char erased;
extern unsigned char Last_Frame;

#endif

