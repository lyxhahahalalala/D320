/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_can.c
 * @brief   can application
 * @version V1.0.0
 * @author  liubing
 * @date    2019-3-5
 * @warning none
 * @note <b>history:</b>
 * 1. create file
 *
 */

/*******************************************************************************
 * INCLUDES
 */
#include "ptmr.h"

#include <iodefine.h>
#include "bsp.h"
#include "r_dev_api.h"
#include "string.h"
#include "rscan.h"
#include "adc.h"
#include "SYSTEM.h"
#include "main.h"
#include "rtc.h"

#include "network_layer.h"
#include "uds_service.h"
#include "uds_support.h"

#include "app_time.h"
#include "app_data_center.h"
#include "app_can.h"
#include "app_telltale.h"

/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */

#define C6_UP_L_MCU        !PORT_ISO.PPR16.BIT.PPR16_11
#define C4_OK_L_MCU        !PORT_ISO.PPR16.BIT.PPR16_9
#define C5_CANCEL_L_MCU    !PORT_ISO.PPR16.BIT.PPR16_10
#define C7_DOWN_L_MCU      !PORT_ISO.PPR16.BIT.PPR16_8
#define DT_ADDTIME_PERSEC  1


/*******************************************************************************
 * LOCAL FUNCTIONS DECLEAR
 */

void can_processTMO_PCan(uint32_t id, uint8_t *buf);
void can_processTMO_BCan(uint32_t id, uint8_t *buf);
void can_processTPMS(uint32_t id, uint8_t *buf);
void tpms_LearningReadBack(uint32_t id, uint8_t *buf);


static void canRXFrame_DM1_Handler(uint32_t id, uint8_t *buf);
static void canRXFrame_LockVeh_Handler(uint32_t id, uint8_t *buf);
static void canRXFrame_recvHandle(uint32_t id, uint8_t *buffer);


/** VARIABLES **/
uint32_t wake_can_cnt = 0;

static uint8_t bcancnt = 0;
static uint8_t uds_can = 0;

static uint8_t single_dm1_tmo_flg[DM1_OWNER_NUM]  = {0};
static uint8_t mupti_dm1_tmo_flg[DM1_OWNER_NUM] = {0};

BCM_18FFE633_t Wheels[4][4];
uint8_t  wheelupdatedtimer[4][4];
uint32_t wheelupdated = 0;
uint8_t  backvalue;
uint8_t  study=0;
uint8_t  tire = 1;
uint8_t  learnstatus=0;
static uint8_t back = 0;

uint8_t isOverseasVer = 0;
uint8_t LckVehBoundState = 0;
uint8_t LckVehdiplayState = 0;
uint8_t Remaining_unlocked_times = 0;
static uint8_t SuperviseMode = 0;
static ptmrType_t LockVehBoundTmr = 0;
static uint8_t LockVehBound_cnt = 0;

static uint32_t door_state_tmr = 0;
static uint8_t  door_previous_state = 0;
static uint8_t vcu_ready_18fff531 = 0;
static uint8_t vcu_ready_04f02270 = 0;


#define PCAN_RX_FRAME_COUNT (sizeof(lct_PcanRxFrame) / sizeof(canRxFrameLCfg_t))
const canRxFrameLCfg_t lct_PcanRxFrame[] =
{
/*  0*/{0x0CFFEAF4,   3000, can_processTMO_PCan, canRXFrame_recvHandle},
/*  1*/{0x18FFEEF4,   5000, can_processTMO_PCan, 0},
/*  2*/{0x18FFF2F4,   5000, can_processTMO_PCan, 0},
/*  3*/{0x0CFF11EF,   5000, can_processTMO_PCan, 0},
/*  4*/{0x0CFF11F0,   5000, can_processTMO_PCan, 0},
/*  5*/{0x18F00503,   3000, can_processTMO_PCan, 0},
/*  6*/{0x18FE4A03,   3000, can_processTMO_PCan, 0},
/*  7*/{0x18FDA403,   3000, can_processTMO_PCan, 0},
/*  8*/{0x0CF00203,   3000, can_processTMO_PCan, 0},
/*  9*/{0x0CEF3103,   3000, can_processTMO_PCan, 0},
/* 10*/{0x0CFF0931,   3000, can_processTMO_PCan, 0},
/* 11*/{0x18FF1F31, 600000, can_processTMO_PCan, canRXFrame_LockVeh_Handler}, 
/* 12*/{0x18FFF531,   3000, can_processTMO_PCan, canRXFrame_recvHandle},
/* 13*/{0x18FEF131,   3000, can_processTMO_PCan, 0}, //转发
/* 14*/{0x18F0010B,   3000, can_processTMO_PCan, 0},
/* 15*/{0x18F0090B,   3000, can_processTMO_PCan, 0}, //转发
/* 16*/{0x18FDC40B,   3000, can_processTMO_PCan, 0},
/* 17*/{0x18FE4F0B,   3000, can_processTMO_PCan, 0}, //转发
/* 18*/{0x18FEBF0B,   3000, can_processTMO_PCan, 0}, //转发
/* 19*/{0x18F00010,   3000, can_processTMO_PCan, 0},
/* 20*/{0x10F007E8,   3000, can_processTMO_PCan, 0},
/* 21*/{0x18FE5BE8,   3000, can_processTMO_PCan, 0},
/* 22*/{0x18FF3C50,   3000, can_processTMO_PCan, 0},
/* 23*/{0x18F00150,   3000, can_processTMO_PCan, 0},
/* 24*/{0x0CF02F2A,   3000, can_processTMO_PCan, 0},
/* 25*/{0x0C010305,   3000, can_processTMO_PCan, 0},
/* 26*/{0x0C3DD7A7,   5000, can_processTMO_PCan, 0}, //0x0C3DD7A7 从PCAN 调到BCAN,周期由0.2s变为1s
/* 27*/{0x18FF5527,   3000, can_processTMO_PCan, 0},
/* 28*/{0x18FFC006,   3000, can_processTMO_PCan, 0},
/* 29*/{0x18FF7C4A,   3000, can_processTMO_PCan, 0},
/* 30*/{0x18FF709E,   3000, can_processTMO_PCan, 0},

/* 31*/{0x18FFA6F5,   3000, can_processTMO_PCan, canRXFrame_recvHandle}, //FCU
/* 32*/{0x18FFA7F5,   3000, can_processTMO_PCan, 0}, //FCU
/* 33*/{0x18FFA8F5,   3000, can_processTMO_PCan, 0}, //FCU
/* 34*/{0x18FFAAF5,   3000, can_processTMO_PCan, 0}, //FCU
/* 35*/{0x18FFA1F6,   3000, can_processTMO_PCan, 0}, //HMS
/* 36*/{0x18FFA2F6,   3000, can_processTMO_PCan, 0}, //HMS
/* 37*/{0x18FFA5F6,   3000, can_processTMO_PCan, 0}, //HMS
/* 38*/{0x18FFABF6,   3000, can_processTMO_PCan, 0}, //HMS
/* 39*/{0x18FF7FDE,   3000, can_processTMO_PCan, canRXFrame_LockVeh_Handler}, //
/* 40*/{0x19FF5CF3,   3000, can_processTMO_PCan, 0}, //锂电
/* 41*/{0x19FF5BF3,   3000, can_processTMO_PCan, 0}, //锂电
/* 42*/{0x0CF19682,   3000, can_processTMO_PCan, 0}, //TMS
/* 43*/{0x18FF0B03,   3000, can_processTMO_PCan, 0}, //AMT 博世两档电驱桥
/* 44*/{0x18FF0B04,   3000, can_processTMO_PCan, 0}, //AMT 博世两档电驱桥
/* 45*/

// {0x0CF00400,   3000, 0, 0}, //转发
// {0x18FF0200,   3000, 0, 0}, //转发
// {0x18FD0700,   3000, 0, 0}, //转发
// {0x18FEEE00,   3000, can_processTMO_PCan, 0}, //转发
// {0x18F00029,   3000, 0, 0}, //转发
// {0x18F0000F,   3000, 0, 0}, //转发
	
	{0x18FECA31, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //整车控制器(HCU)
	{0x18ECFF31, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF31, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA03, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //自动变速箱(AMT)
	{0x18ECFF03, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF03, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAF4, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //电池管理系统(BMS)
	{0x18ECFFF4, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFF4, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAEF, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //电机控制器(MCU)
	{0x18ECFFEF, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFEF, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAF0, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //电机控制器(MCU2)
	{0x18ECFFF0, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFF0, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA82, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //电池冷却系统(TMS)
	{0x18ECFF82, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF82, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA0B, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //ABS/EBS KNORR
	{0x18ECFF0B, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF0B, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA05, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //SCU
	{0x18ECFF05, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF05, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECADB, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //换电系统(PECU)
	{0x18ECFFDB, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFDB, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAA7, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //冷却系统(ATS)
	{0x18ECFFA7, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFA7, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAA8, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //冷却系统(ATS2)
	{0x18ECFFA8, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFA8, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA50, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //Knorr/科密瑞立 EPB
	{0x18ECFF50, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF50, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA13, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //电液转向机(EHPS)
	{0x18ECFF13, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF13, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAF5, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //燃料电池控制器(FCU)/燃电DCDC
	{0x18ECFFF5, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFF5, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAF6, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //氢管理系统(HMU)
	{0x18ECFFF6, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFF6, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA84, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //外挂DCDC
	{0x18ECFF84, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF84, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA85, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //外挂DCDC2
	{0x18ECFF85, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF85, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA8F, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //燃料电池冷却系统(FTS)
	{0x18ECFF8F, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF8F, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAE8, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //月照松/Knorr 摄像头
	{0x18ECFFE8, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFE8, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECA2A, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //Knorr 雷达
	{0x18ECFF2A, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF2A, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x18FECAA0, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler}, //月照松 雷达
	{0x18ECFFA0, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFA0, 3000, can_processTMO_PCan, canRXFrame_DM1_Handler},
	
	{0x04F02270,   3000, can_processTMO_PCan, canRXFrame_recvHandle},//lyx
	{0x04F02370,   3000, can_processTMO_PCan, canRXFrame_recvHandle},//lyx
	{0x04F02470,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02570,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02670,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02770,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02870,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02970,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02A70,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02B70,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02C70,   3000, can_processTMO_PCan, 0},//lyx
	{0x04F02D70,   3000, can_processTMO_PCan, 0},//lyx
	{0x1885EFF3,   3000, can_processTMO_PCan, 0},//lyx
	{0x1886EFF3,   3000, can_processTMO_PCan, 0},//lyx
	{0x1887EFF3,   3000, can_processTMO_PCan, 0},//lyx
	{0x1888EFF3,   3000, can_processTMO_PCan, 0},//lyx
	
};
static uint8_t l_u8PCanRxBuf[PCAN_RX_FRAME_COUNT][8];
static ptmrType_t l_tpcanRxTimeCnt[PCAN_RX_FRAME_COUNT];
static uint8_t l_u8pcanRxFrameSt[PCAN_RX_FRAME_COUNT];
static canRxFrameCfg_t l_tPCanRxFrame =
{
	lct_PcanRxFrame,
	l_u8PCanRxBuf,
	l_tpcanRxTimeCnt,
	l_u8pcanRxFrameSt
};

#define BCAN_RX_FRAME_COUNT (sizeof(lct_BcanRxFrame) / sizeof(canRxFrameLCfg_t))
const canRxFrameLCfg_t lct_BcanRxFrame[] =
{
/*  0*/{0x18FF2232, 3000, can_processTMO_BCan, canRXFrame_recvHandle},
/*  1*/{0x18FF2732, 3000, can_processTMO_BCan, 0},
/*  2*/{0x18FED932, 3000, can_processTMO_BCan, 0}, //转发
/*  3*/{0x18FDCD32, 3000, 0, 0}, //转发
/*  4*/{0x18A9E832, 3000, 0, 0}, //转发
/*  5*/{0x0C0B2A32, 3000, 0, 0}, //转发
/*  6*/{0x0CFDCC32, 3000, 0, 0}, //转发
/*  7*/{0x18FFF631, 3000, can_processTMO_BCan, 0},
/*  8*/{0x18FFF731, 3000, can_processTMO_BCan, 0},
/*  9*/{0x18F71E31, 5000, can_processTMO_BCan, 0},
/* 10*/{0x18FEEA2F, 3000, can_processTMO_BCan, 0}, //转发
/* 11*/{0x18FE592F, 3000, can_processTMO_BCan, 0}, //转发
/* 12*/{0x0CFE5A2F, 3000, can_processTMO_BCan, 0}, //转发
/* 13*/{0x10F007E8, 3000, can_processTMO_BCan, 0},
/* 14*/{0x10FE6FE8, 3000, can_processTMO_BCan, 0},
/* 15*/{0x18FE5BE8, 3000, can_processTMO_BCan, 0},
/* 16*/{0x18FF16E8, 3000, can_processTMO_BCan, 0},
/* 17*/{0x18FFE633, 3000, can_processTMO_BCan, can_processTPMS},
/* 18*/{0x18FFE833,  300, can_processTMO_BCan, tpms_LearningReadBack},
/* 19*/{0x18FF28EC, 3000, can_processTMO_BCan, 0},
/* 20*/{0x18FF0D19, 5000, 0, 0},
/* 21*/{0x1CFEC303, 3000, 0, 0}, //转发
/* 22*/{0x18FF14E7, 2000, can_processTMO_BCan, 0},
/* 23*/{0x18FED947, 3000, can_processTMO_BCan, 0},
/* 24*/{0x0C3DD7A7, 5000, can_processTMO_BCan, 0},
/* 25*/{0x18F7021E, 3000, can_processTMO_BCan, 0},
/* 26*/{0x18FF709E, 3000, can_processTMO_BCan, 0},
/* 27*/{0x18FEE6EE, 5000, can_processTMO_BCan, canRXFrame_recvHandle}, //转发 TD报文
/* 28*/{0x18FF2130, 3000, can_processTMO_BCan, 0}, //转发
/* 29*/{0x0CF6022E, 3000, can_processTMO_BCan, 0}, //转发
/* 30*/{0x18FF6DD8, 3000, can_processTMO_BCan, 0}, //转发
/* 31*/{0x18FFF931, 3000, can_processTMO_BCan, 0}, //VTD5
/* 32*/{0x11FF3F32, 3000, can_processTMO_BCan, canRXFrame_recvHandle}, //蓄电池亏电提醒（智能补电）
/* 33*/{0x18FED631, 3000, can_processTMO_BCan, canRXFrame_recvHandle}, //HCU
/* 34*/{0x18FF8F25, 3000, can_processTMO_BCan, 0}, //TBOX
/* 35*/{0x0CF6034D, 3000, can_processTMO_PCan, 0}, //低压转向

	{0x18FECA32, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF32, 5000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF32, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA41, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF41, 5000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF41, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECAE7, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFFE7, 5000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFE7, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA33, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF33, 5000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF33, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECAEC, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFFEC, 5000, 0, canRXFrame_DM1_Handler},
	{0x18EBFFEC, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA2E, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF2E, 5000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF2E, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA4D, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF4D, 5000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF4D, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA1E, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF1E, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF1E, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA30, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF30, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF30, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA1A, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF1A, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF1A, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	{0x18FECA2F, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	{0x18ECFF2F, 3000, 0, canRXFrame_DM1_Handler},
	{0x18EBFF2F, 3000, can_processTMO_BCan, canRXFrame_DM1_Handler},
	
	
};
static uint8_t l_u8BCanRxBuf[BCAN_RX_FRAME_COUNT][8];
static ptmrType_t l_tbcanRxTimeCnt[BCAN_RX_FRAME_COUNT];
static uint8_t l_u8bcanRxFrameSt[BCAN_RX_FRAME_COUNT];
static canRxFrameCfg_t l_tBCanRxFrame =
{
	lct_BcanRxFrame,
	l_u8BCanRxBuf,
	l_tbcanRxTimeCnt,
	l_u8bcanRxFrameSt
};


static ptmrType_t bcmNodeTmr = 0;
static ptmrType_t tpmsNodeTmr = 0;
static ptmrType_t absNodeTmr = 0;
static ptmrType_t ebsNodeTmr = 0;
static ptmrType_t amtNodeTmr = 0;
static ptmrType_t ecasNodeTmr = 0;
static ptmrType_t ldwsNodeTmr = 0;
static ptmrType_t aebsNodeTmr = 0;
static void NodeTmoInit(void)
{
	ptmr_Start(&bcmNodeTmr );
	ptmr_Start(&tpmsNodeTmr);
	ptmr_Start(&absNodeTmr );
	ptmr_Start(&ebsNodeTmr );
	ptmr_Start(&amtNodeTmr );
	ptmr_Start(&ecasNodeTmr);
	ptmr_Start(&ldwsNodeTmr);
	ptmr_Start(&aebsNodeTmr);
	
	BCM_NODE_TMO_FLG  = 0;
	TPMS_NODE_TMO_FLG = 0;
	ABS_NODE_TMO_FLG  = 0;
	EBS_NODE_TMO_FLG  = 0;
	AMT_NODE_TMO_FLG  = 0;
	ECAS_NODE_TMO_FLG = 0;
	LDWS_NODE_TMO_FLG = 0;
	AEBS_NODE_TMO_FLG = 0;
	
}
static void BcanNodeTmoClear(uint32_t id)
{
	switch(id)
    {
		case 0x18FF2732:
            ptmr_Start(&bcmNodeTmr);
			BCM_NODE_TMO_FLG = 0;
			break;
		case 0x18FFE633:
			if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
			{
				ptmr_Start(&tpmsNodeTmr);
				TPMS_NODE_TMO_FLG = 0;
			}
			break;
		case 0x18FE5BE8:
            if(eol_ldwstype == 3)
			{
				ptmr_Start(&ldwsNodeTmr);
				LDWS_NODE_TMO_FLG = 0;
			}
			break;
		case 0x0CFE5A2F:
			if((0x05 == eol_ecastype) || (0x04 == eol_ecastype) || (0x03 == eol_ecastype))
			{
				ptmr_Start(&ecasNodeTmr);
				ECAS_NODE_TMO_FLG = 0;
			}
            
			break;
		default:
			
			break;
    }
	
}
static void PcanNodeTmoClear(uint32_t id)
{
	switch(id)
    {
		case 0x18F0010B:
			if((eol_ebs == 1) \
			|| (eol_ebs == 2) \
			|| (eol_ebs == 3) \
			|| (eol_ebs == 4) \
			|| (eol_ebs == 7) \
			|| (eol_ebs == 8) \
			|| (eol_ebs == 9) \
			|| (eol_ebs == 11) \
			|| (eol_ebs == 12))
			{
				ptmr_Start(&absNodeTmr);
				ABS_NODE_TMO_FLG = 0;
			}
			
			if((eol_ebs ==5)||(eol_ebs == 6)||(eol_ebs ==10)|| (eol_ebs == 13))
			{
				ptmr_Start(&ebsNodeTmr);
				EBS_NODE_TMO_FLG = 0;
			}
			break;
		case 0x18F00503:
			if((33U != eol_atmtype) && (34U != eol_atmtype))
			{
				ptmr_Start(&amtNodeTmr);
				AMT_NODE_TMO_FLG = 0;
			}
			break;
		case 0x0CEF3103:
            if((33U != eol_atmtype) && (34U != eol_atmtype))
			{
				ptmr_Start(&amtNodeTmr);
				AMT_NODE_TMO_FLG = 0;
			}
			break;
		case 0x18FE5BE8:
            if((eol_ldwstype == 2) || (eol_ldwstype == 4))
			{
				ptmr_Start(&ldwsNodeTmr);
				LDWS_NODE_TMO_FLG = 0;
			}
			break;
		case 0x0CF02F2A:
			if((eol_aebstype == 2) || (eol_aebstype == 3))
			{
				ptmr_Start(&aebsNodeTmr);
				AEBS_NODE_TMO_FLG = 0;
			}
			break;
		default:
			
			break;
    }
	
}
static void canNodeTmoCheck(void)
{
	if(ptmr_Get(&bcmNodeTmr) >= 30000)
	{
		ptmr_Stop(&bcmNodeTmr);
		BCM_NODE_TMO_FLG = 1;
	}
	
	if(ptmr_Get(&tpmsNodeTmr) >= 30000)
	{
		ptmr_Stop(&tpmsNodeTmr);
		if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
			TPMS_NODE_TMO_FLG = 1;
		else
			TPMS_NODE_TMO_FLG = 0;
	}
	
	if(ptmr_Get(&absNodeTmr) >= 30000)
	{
		ptmr_Stop(&absNodeTmr);
		if((eol_ebs == 1) \
		|| (eol_ebs == 2) \
		|| (eol_ebs == 3) \
		|| (eol_ebs == 4) \
		|| (eol_ebs == 7) \
		|| (eol_ebs == 8) \
		|| (eol_ebs == 9) \
		|| (eol_ebs == 11) \
		|| (eol_ebs == 12))
			ABS_NODE_TMO_FLG = 1;
		else
			ABS_NODE_TMO_FLG = 0;
	}
	
	if(ptmr_Get(&ebsNodeTmr) >= 30000)
	{
		ptmr_Stop(&ebsNodeTmr);
		if((eol_ebs ==5)||(eol_ebs == 6)||(eol_ebs ==10)|| (eol_ebs == 13))
			EBS_NODE_TMO_FLG = 1;
		else
			EBS_NODE_TMO_FLG = 0;
		
	}
	
	if(ptmr_Get(&amtNodeTmr) >= 30000)
	{
		ptmr_Stop(&amtNodeTmr);
		if((33U != eol_atmtype) && (34U != eol_atmtype))
			AMT_NODE_TMO_FLG = 1;
		else
			AMT_NODE_TMO_FLG = 0;
	}
	
	if(ptmr_Get(&ecasNodeTmr) >= 30000)
	{
		ptmr_Stop(&ecasNodeTmr);
		if((0x05 == eol_ecastype) || (0x04 == eol_ecastype) || (0x03 == eol_ecastype))
			ECAS_NODE_TMO_FLG = 1;
		else
			ECAS_NODE_TMO_FLG = 0;
	}
	
	if(ptmr_Get(&ldwsNodeTmr) >= 30000)
	{
		ptmr_Stop(&ldwsNodeTmr);
		if((eol_ldwstype == 3) || (eol_ldwstype == 2) || (eol_ldwstype == 4))
			LDWS_NODE_TMO_FLG = 1;
		else
			LDWS_NODE_TMO_FLG = 0;
	}
	
	if(ptmr_Get(&aebsNodeTmr) >= 30000)
	{
		ptmr_Stop(&aebsNodeTmr);
		if((eol_aebstype == 2) || (eol_aebstype == 3))
			AEBS_NODE_TMO_FLG = 1;
		else
			AEBS_NODE_TMO_FLG = 0;
	}
	
}


/*******************************************************************************
 *  GLOBAL FUNCTIONS IMPLEMENTATION
 */

void can_init(void)
{
    // 初始化CAN0 数据
    for (uint32_t i = 0; i < PCAN_RX_FRAME_COUNT; i ++)
    {
        l_tPCanRxFrame.st[i] = CAN_FRAME_ST_INIT;
		
		for (uint32_t k = 0; k < 8; k ++)
		{
			l_tPCanRxFrame.buffer[i][k] = 0;
		}
		
		if(l_tPCanRxFrame.lcfg[i].id == 0x0CFFEAF4)
		{
			if(eol_battcoolmd==0)
			{
				l_tPCanRxFrame.buffer[i][3] = 0x70;
				l_tPCanRxFrame.buffer[i][4] = 0x17;
			}
			else if(eol_battcoolmd==1)
			{
				l_tPCanRxFrame.buffer[i][3] = 0x10;
				l_tPCanRxFrame.buffer[i][4] = 0x27;
			}
			else if(eol_battcoolmd==2)
			{
				l_tPCanRxFrame.buffer[i][3] = 0x30;
				l_tPCanRxFrame.buffer[i][4] = 0x75;
			}
		}
		
        ptmr_Start(&l_tPCanRxFrame.timer[i]);
    }
	
	for (uint32_t i = 0; i < BCAN_RX_FRAME_COUNT; i ++)
    {
        l_tBCanRxFrame.st[i] = CAN_FRAME_ST_INIT;
		
		for (uint32_t k = 0; k < 8; k ++)
		{
			l_tBCanRxFrame.buffer[i][k] = 0;
		}
		
        ptmr_Start(&l_tBCanRxFrame.timer[i]);
    }
	
	NodeTmoInit();
	
}
void can_chip_mode_switch(uint8_t channel,can_mode_t mode)
{
    if(channel == CAN_2_CH)
	{
		switch(mode)
		{
	   	 	case NORMAL_MODE:
				CAN2_EN = 1;
				CAN2_STB= 1;
				MCU_WAKE_CAN2 = 0;
				break;
	    	case STANDBY_MODE:
				CAN2_EN = 0;
				CAN2_STB = 0;
				MCU_WAKE_CAN2 = 0;
				break;
	    	case GO_TO_SLEEP_MODE:
				CAN2_EN = 1;
				CAN2_STB = 0;
				// MCU_WAKE_CAN2 = 1;
				break;
	 	   	default:
				break;
		}
    }
    else if(channel == CAN_1_CH)
	{
		switch(mode)
		{
	    	case NORMAL_MODE:
				CAN1_EN = 1;
				CAN1_STB = 1;
				MCU_WAKE_CAN1 = 0;
				break;
	    	case STANDBY_MODE:
				CAN1_EN = 0;
				CAN1_STB = 0;
				MCU_WAKE_CAN1 = 0;
				break;
	    	case GO_TO_SLEEP_MODE:
				CAN1_EN = 1;
				CAN1_STB = 0;
				// MCU_WAKE_CAN1 = 1;
				break;
	    	default:
				break;
		}
    }
}

void Process_Can1(void)  //BCAN
{
    CAN_MESSAGE msg;
    CAN_MESSAGE tx_msg;
	
    memset(&msg, 0, sizeof(CAN_MESSAGE));
    CAN1_ReceiveMessage(&msg);
    CANCounters.BCAN = 0;
	
    switch (msg.id)
	{
		case 0x18DA17F1:
			uds_can = 1;
			uds_get_frame(0, msg.data, msg.len);
			break; 
		case 0x18DA17F2:
			Test_Mode = msg.data[0];
			testcnt=0;
			configFlag = msg.data[6];
			if(bcancnt<255)
				bcancnt++;
			break; 
		case 0x18DA17F3:
			if(msg.data[0] == 0xF8)
			{
				/* TODO */
			}
			break; 
		case 0x0159:  //jump to boot layer
			if((msg.data[0]+msg.data[1]+msg.data[2]+msg.data[3]+msg.data[4]+msg.data[5]+msg.data[6]+msg.data[7]) == 0)
				R_DEV_WRITE_PROT_REG(0xfff80000u, 0xfff80a04ul, 0x00000001) //software reset
			else;
			break;
		case 0x0151:  //test jump from boot layer to app layer ok							  
			tx_msg.id = 0x0151;
			tx_msg.data[0]= VERSIONS_MAJOR>>8;
			tx_msg.data[1]= VERSIONS_MAJOR;
			tx_msg.data[2]= VERSIONS_MINOR>>8;
			tx_msg.data[3]= VERSIONS_MINOR;
			tx_msg.data[4]= VERSIONS_YEAR;
			tx_msg.data[5]= VERSIONS_MONTH;
			tx_msg.data[6]= VERSIONS_DAY;
			tx_msg.data[7]= CAN_BL_App;
			tx_msg.len = 8;
			tx_msg.type = 0;
			CAN1_SendFIFOMessage(&tx_msg);                                   				 
			break;
		case 0x01F9:  //专用于通用UDS固件跳转
			if((msg.data[0]+msg.data[1]+msg.data[2]+msg.data[3]+msg.data[4]+msg.data[5]+msg.data[6]+msg.data[7]) == 0)
				R_DEV_WRITE_PROT_REG(0xfff80000u, 0xfff80a04ul, 0x00000001) //software reset
			else;
			break;
		case 0x01F1:  //专用于通用UDS连接
			tx_msg.id = 0x01F1;
			tx_msg.data[0]= VERSIONS_MAJOR>>8;
			tx_msg.data[1]= VERSIONS_MAJOR;
			tx_msg.data[2]= VERSIONS_MINOR>>8;
			tx_msg.data[3]= VERSIONS_MINOR;
			tx_msg.data[4]= VERSIONS_YEAR;
			tx_msg.data[5]= VERSIONS_MONTH;
			tx_msg.data[6]= VERSIONS_DAY;
			tx_msg.data[7]= CAN_BL_App;
			tx_msg.len = 8;
			tx_msg.type = 0;
			CAN1_SendFIFOMessage(&tx_msg);                                   				 
			break;
		case 0x0888:  //test jump from boot layer to app layer ok
			timer.hour   = msg.data[2];
			timer.minute = msg.data[3];
			timer.year   = msg.data[4];
			timer.month  = msg.data[5];
			timer.day    = msg.data[6];
			External_RTC_SetTime(&timer);
			break;
		case 0x610:  //test jump from boot layer to app layer ok
			if((timer.minute == msg.data[0])
			|| (timer.hour   == msg.data[1])
			|| (timer.day    == msg.data[2])
			|| (timer.month  == msg.data[3])
			|| (timer.year   == msg.data[4]))
			{
				mile_key_ok_flg = 1;
			}
			if(msg.data[0] == 0x20 && mile_key_ok_flg == 1)
			{/* 里程修改加固 */
				e_total_miles  = msg.data[1];
				e_total_miles += msg.data[2]<<8;
				e_total_miles += msg.data[3]<<16;
				e_total_miles += msg.data[4]<<24;
				e_total_miles  = e_total_miles*10;
				EEROM_WriteTotalMiles();
				Miles.total_miles = e_total_miles;
				
				mile_key_ok_flg = 0;
			}
			break;	 
		default:
			for (int32_t i = 0; i < BCAN_RX_FRAME_COUNT; i ++)
			{
				if (l_tBCanRxFrame.lcfg[i].id == msg.id)
				{
					BcanNodeTmoClear(msg.id);
					
					memcpy(&l_tBCanRxFrame.buffer[i][0], msg.data, 8);
					ptmr_Start(&l_tBCanRxFrame.timer[i]);          // 清零计数器
					l_tBCanRxFrame.st[i] = CAN_FRAME_ST_RECVED;     // 接收状态
					if (NULL != l_tBCanRxFrame.lcfg[i].recvHandle)
					{
						l_tBCanRxFrame.lcfg[i].recvHandle(msg.id, l_tBCanRxFrame.buffer[i]);
					}
					break;
				}
			}
			break;
    }
}
void Process_Can2(void)  //PCAN
{
    CAN_MESSAGE msg;
    CAN_MESSAGE tx_msg;   
	
    memset(&msg, 0, sizeof(CAN_MESSAGE));
    CAN2_ReceiveMessage(&msg);
    CANCounters.PCAN = 0;
	
    switch (msg.id) 
	{
		case 0x18DA17F1:
			uds_can = 2;
	    	uds_get_frame(0, msg.data, msg.len);
	    	break;
		case 0x18DA17F2:
	    	Test_Mode = msg.data[0];
	    	testcnt = 0;
	    	configFlag = msg.data[6];
	    	break; 
		case 0x18DA17F3:
			if(msg.data[0] == 0xF8)
			{
			    /* TODO */
			}
	    	break; 
		case 0x0159://jump to boot layer
	    	if((msg.data[0]+msg.data[1]+msg.data[2]+msg.data[3]+msg.data[4]+msg.data[5]+msg.data[6]+msg.data[7]) == 0)
				R_DEV_WRITE_PROT_REG(0xfff80000u, 0xfff80a04ul, 0x00000001) //software reset
	    	else;
	   	 	break;
		case 0x0151://test jump from boot layer to app layer ok							  
	    	tx_msg.id = 0x0151;
	    	tx_msg.data[0]= VERSIONS_MAJOR>>8;
	    	tx_msg.data[1]= VERSIONS_MAJOR;
	    	tx_msg.data[2]= VERSIONS_MINOR>>8;
	   	 	tx_msg.data[3]= VERSIONS_MINOR;
	    	tx_msg.data[4]= VERSIONS_YEAR;
	    	tx_msg.data[5]= VERSIONS_MONTH;
	    	tx_msg.data[6]= VERSIONS_DAY;
	    	tx_msg.data[7]= CAN_BL_App;
	    	tx_msg.len = 8;
	    	tx_msg.type = 0;
			CAN2_SendFIFOMessage(&tx_msg);                                   				 
	    	break;
		case 0x01F9:  //专用于通用UDS固件跳转
			if((msg.data[0]+msg.data[1]+msg.data[2]+msg.data[3]+msg.data[4]+msg.data[5]+msg.data[6]+msg.data[7]) == 0)
				R_DEV_WRITE_PROT_REG(0xfff80000u, 0xfff80a04ul, 0x00000001) //software reset
			else;
			break;
		case 0x01F1:  //专用于通用UDS连接
			tx_msg.id = 0x01F1;
			tx_msg.data[0]= VERSIONS_MAJOR>>8;
			tx_msg.data[1]= VERSIONS_MAJOR;
			tx_msg.data[2]= VERSIONS_MINOR>>8;
			tx_msg.data[3]= VERSIONS_MINOR;
			tx_msg.data[4]= VERSIONS_YEAR;
			tx_msg.data[5]= VERSIONS_MONTH;
			tx_msg.data[6]= VERSIONS_DAY;
			tx_msg.data[7]= CAN_BL_App;
			tx_msg.len = 8;
			tx_msg.type = 0;
			CAN2_SendFIFOMessage(&tx_msg);                                   				 
			break;
		case 0x0888:  //test jump from boot layer to app layer ok
	   	 	timer.hour   = msg.data[2];
	    	timer.minute = msg.data[3];
	    	timer.year   = msg.data[4];
	    	timer.month  = msg.data[5];
	    	timer.day    = msg.data[6];
			External_RTC_SetTime(&timer);
	    	break;
		case 0x610:  //test jump from boot layer to app layer ok
			if((timer.minute == msg.data[0])
			|| (timer.hour   == msg.data[1])
			|| (timer.day    == msg.data[2])
			|| (timer.month  == msg.data[3])
			|| (timer.year   == msg.data[4]))
			{
				mile_key_ok_flg = 1;
			}
			if(msg.data[0] == 0x20 && mile_key_ok_flg == 1)
			{/* 里程修改加固 */
				e_total_miles  = msg.data[1];
				e_total_miles += msg.data[2]<<8;
				e_total_miles += msg.data[3]<<16;
				e_total_miles += msg.data[4]<<24;
				e_total_miles  = e_total_miles*10;
				EEROM_WriteTotalMiles();
				Miles.total_miles = e_total_miles;
				
				mile_key_ok_flg = 0;
			}
	    	break;	 
		default:
        	for (int32_t i = 0; i < PCAN_RX_FRAME_COUNT; i ++)
        	{
                if (l_tPCanRxFrame.lcfg[i].id == msg.id)
                {
					PcanNodeTmoClear(msg.id);
					
                    memcpy(&l_tPCanRxFrame.buffer[i][0], msg.data, 8);
                    ptmr_Start(&l_tPCanRxFrame.timer[i]);          
                    l_tPCanRxFrame.st[i] = CAN_FRAME_ST_RECVED;     
                    if (NULL != l_tPCanRxFrame.lcfg[i].recvHandle)
                    {
                        l_tPCanRxFrame.lcfg[i].recvHandle(\
						msg.id, \
						l_tPCanRxFrame.buffer[i]
						);
                	}
            		break;
        		}
        	}
	    	break;
	}
}
void Process_Can0(void)
{
   
}
void Process_Can(void)
{
    /* if((RSCAN0.RFSTS0.UINT32 & 0xFF00) > 0)
    {
        Process_Can0();
    } */
    
    if((RSCAN0.RFSTS1.UINT32 & 0xFF00) > 0)
    {
		if(can_busoff_flg[1] == 0)
			Process_Can1();
    }

    if((RSCAN0.RFSTS2.UINT32 & 0xFF00) > 0)
    {
		if(can_busoff_flg[2] == 0)
			Process_Can2();
    }
	
	wake_can = 1;
	wake_can_cnt = 0;
	
}


void can_processTMO_PCan(uint32_t id, uint8_t *buf)
{
    for (int32_t i = 0; i < PCAN_RX_FRAME_COUNT; i ++)
	{
		if (l_tPCanRxFrame.lcfg[i].id == id)
		{
		    l_tPCanRxFrame.st[i] = CAN_FRAME_ST_TIMEOUT;
			memset(&l_u8PCanRxBuf[i],0,8);
	   	 	break;
		}
    }
}
void can_processTMO_BCan(uint32_t id, uint8_t *buf)
{
	for (uint32_t i = 0; i < BCAN_RX_FRAME_COUNT; i ++)
	{
		if (l_tBCanRxFrame.lcfg[i].id == id)
		{
			l_tBCanRxFrame.st[i] = CAN_FRAME_ST_TIMEOUT;
			memset(&l_u8BCanRxBuf[i],0,8);
			break;
		}
	}
}


void can_rxRoutine(void)
{
	static uint32_t  clear_keyflg_cnt = 0;
	
	if(clear_keyflg_cnt >= 20)
	{
		mile_key_ok_flg = 0;
	}
	else 
	{
		clear_keyflg_cnt++;
	}
	
	for (int32_t i = 0; i < PCAN_RX_FRAME_COUNT; i ++)
	{
		if(ptmr_Get(&l_tPCanRxFrame.timer[i]) >= l_tPCanRxFrame.lcfg[i].tmoMs)
		{
			ptmr_Stop(&l_tPCanRxFrame.timer[i]);
			
			if (R_NULL != l_tPCanRxFrame.lcfg[i].tmoHandle)
			{
				l_tPCanRxFrame.lcfg[i].tmoHandle(l_tPCanRxFrame.lcfg[i].id, l_tPCanRxFrame.buffer[i]);
			}
			
			if(l_tPCanRxFrame.lcfg[i].id == 0x18FF7FDE)
			{
				isOverseasVer = 0;
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FFF531)//lyx
			{
    			vcu_ready_18fff531 = 0;
    			VehicleReadyFlag = (vcu_ready_18fff531 || vcu_ready_04f02270);
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x04F02270)//lyx
			{
    			vcu_ready_04f02270 = 0;
    			VehicleReadyFlag = (vcu_ready_18fff531 || vcu_ready_04f02270);
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x0CFFEAF4)
			{
				CAN_CHARGE_LINE = 0;
				BMS_SAFETY_LINE_SG = 0;
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FFA6F5)
			{
				CAN_H2_ChuiSao   = 0;
				CAN_H2_LowTptChg = 0;
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FF7C4A)
			{
				ZhongLianLockVehFlg = 0;
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x04F02370)//lyx
			{
    			CAN_CHARGE_LINE = 0;
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FF1F31)
			{
				// LckVehBoundState = 0;
				// LckVehdiplayState = 0;
				Remaining_unlocked_times = 0;
				SuperviseMode = 0;
				
				LockVehBound_cnt = 0;
				ptmr_Stop(&LockVehBoundTmr);
			}
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA82) single_dm1_tmo_flg[DM1_82] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA05) single_dm1_tmo_flg[DM1_05] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA03) single_dm1_tmo_flg[DM1_03] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAF4) single_dm1_tmo_flg[DM1_F4] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAEF) single_dm1_tmo_flg[DM1_EF] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA31) single_dm1_tmo_flg[DM1_31] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA0B) single_dm1_tmo_flg[DM1_0B] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAF5) single_dm1_tmo_flg[DM1_F5] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAE8) single_dm1_tmo_flg[DM1_E8] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAF6) single_dm1_tmo_flg[DM1_F6] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA2A) single_dm1_tmo_flg[DM1_2A] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAA0) single_dm1_tmo_flg[DM1_A0] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAA7) single_dm1_tmo_flg[DM1_A7] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAF0) single_dm1_tmo_flg[DM1_F0] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA50) single_dm1_tmo_flg[DM1_50] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA84) single_dm1_tmo_flg[DM1_84] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA85) single_dm1_tmo_flg[DM1_85] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECADB) single_dm1_tmo_flg[DM1_DB] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECAA8) single_dm1_tmo_flg[DM1_A8] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA8F) single_dm1_tmo_flg[DM1_8F] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA13) single_dm1_tmo_flg[DM1_13] = 1;
			// else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA10) single_dm1_tmo_flg[DM1_10] = 1;
			// else if(l_tPCanRxFrame.lcfg[i].id == 0x18FECA53) single_dm1_tmo_flg[DM1_53] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF82 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF82) mupti_dm1_tmo_flg[DM1_82] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF05 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF05) mupti_dm1_tmo_flg[DM1_05] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF03 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF03) mupti_dm1_tmo_flg[DM1_03] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFF4 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFF4) mupti_dm1_tmo_flg[DM1_F4] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFEF || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFEF) mupti_dm1_tmo_flg[DM1_EF] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF31 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF31) mupti_dm1_tmo_flg[DM1_31] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF0B || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF0B) mupti_dm1_tmo_flg[DM1_0B] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFF5 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFF5) mupti_dm1_tmo_flg[DM1_F5] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFE8 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFE8) mupti_dm1_tmo_flg[DM1_E8] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFF6 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFF6) mupti_dm1_tmo_flg[DM1_F6] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF2A || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF2A) mupti_dm1_tmo_flg[DM1_2A] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFA0 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFA0) mupti_dm1_tmo_flg[DM1_A0] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFA7 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFA7) mupti_dm1_tmo_flg[DM1_A7] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFF0 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFF0) mupti_dm1_tmo_flg[DM1_F0] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF50 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF50) mupti_dm1_tmo_flg[DM1_50] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF84 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF84) mupti_dm1_tmo_flg[DM1_84] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF85 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF85) mupti_dm1_tmo_flg[DM1_85] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFDB || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFDB) mupti_dm1_tmo_flg[DM1_DB] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFFA8 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFFA8) mupti_dm1_tmo_flg[DM1_A8] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF8F || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF8F) mupti_dm1_tmo_flg[DM1_8F] = 1;
			else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF13 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF13) mupti_dm1_tmo_flg[DM1_13] = 1;
			// else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF10 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF10) mupti_dm1_tmo_flg[DM1_10] = 1;
			// else if(l_tPCanRxFrame.lcfg[i].id == 0x18ECFF53 || l_tPCanRxFrame.lcfg[i].id == 0x18EBFF53) mupti_dm1_tmo_flg[DM1_53] = 1;
			else ;
		}
	}
	
	for (int32_t i = 0; i < BCAN_RX_FRAME_COUNT; i ++)
	{
		if (ptmr_Get(&l_tBCanRxFrame.timer[i]) >= l_tBCanRxFrame.lcfg[i].tmoMs)
		{
			ptmr_Stop(&l_tBCanRxFrame.timer[i]);
			
			if (R_NULL != l_tBCanRxFrame.lcfg[i].tmoHandle)
			{
				l_tBCanRxFrame.lcfg[i].tmoHandle(l_tBCanRxFrame.lcfg[i].id, l_tBCanRxFrame.buffer[i]);
			}
			
			if(l_tBCanRxFrame.lcfg[i].id == 0x18FF2232)
			{
				CAN_KEY_ACC = 0;
				GetOffReminderStatus = 0;
				door_previous_state = 0;
				ptmr_Stop(&door_state_tmr);
			}
			else if(l_tBCanRxFrame.lcfg[i].id == 0x11FF3F32)
			{
				SmartRechgCfgFlg = 0;
			}
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FED631)
			{
				MSD_SwitchStatus = 0;
			}
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FEE6EE) TD_MsgEnable = 0;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA41) single_dm1_tmo_flg[DM1_41] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECAE7) single_dm1_tmo_flg[DM1_E7] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA33) single_dm1_tmo_flg[DM1_33] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECAEC) single_dm1_tmo_flg[DM1_EC] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA32) single_dm1_tmo_flg[DM1_32] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA2E) single_dm1_tmo_flg[DM1_2E] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA4D) single_dm1_tmo_flg[DM1_4D] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA1E) single_dm1_tmo_flg[DM1_1E] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA30) single_dm1_tmo_flg[DM1_30] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA1A) single_dm1_tmo_flg[DM1_1A] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18FECA2F) single_dm1_tmo_flg[DM1_2F] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF41 || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF41) mupti_dm1_tmo_flg[DM1_41] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFFE7 || l_tBCanRxFrame.lcfg[i].id == 0x18EBFFE7) mupti_dm1_tmo_flg[DM1_E7] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF33 || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF33) mupti_dm1_tmo_flg[DM1_33] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFFEC || l_tBCanRxFrame.lcfg[i].id == 0x18EBFFEC) mupti_dm1_tmo_flg[DM1_EC] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF32 || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF32) mupti_dm1_tmo_flg[DM1_32] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF2E || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF2E) mupti_dm1_tmo_flg[DM1_2E] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF4D || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF4D) mupti_dm1_tmo_flg[DM1_4D] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF1E || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF1E) mupti_dm1_tmo_flg[DM1_1E] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF30 || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF30) mupti_dm1_tmo_flg[DM1_30] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF1A || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF1A) mupti_dm1_tmo_flg[DM1_1A] = 1;
			else if(l_tBCanRxFrame.lcfg[i].id == 0x18ECFF2F || l_tBCanRxFrame.lcfg[i].id == 0x18EBFF2F) mupti_dm1_tmo_flg[DM1_2F] = 1;
			else ;
			
		}
	}
	
	for(uint8_t i = 0; i < DM1_OWNER_NUM; i++)
	{
		if((single_dm1_tmo_flg[i] == 1) && (mupti_dm1_tmo_flg[i] == 1))
		{
			CurrentFltInfo[i].clearfltflg = 1;
			CurrentFltInfo[i].SA = 0;
			CurrentFltInfo[i].FltNum = 0;
			CurrentFltInfo[i].FltBuf.byte[0] = 0;
			CurrentFltInfo[i].FltBuf.byte[1] = 0;
			
			for(uint8_t j = 0; j < MAX_ERR_NUM; j++)
			{
				CurrentFltInfo[i].FltBuf.DTC[j].SPN = 0;
				CurrentFltInfo[i].FltBuf.DTC[j].FMI = 0;
				CurrentFltInfo[i].FltBuf.DTC[j].OC  = 0;
				CurrentFltInfo[i].FltBuf.DTC[j].CM  = 0;
			}
			
			single_dm1_tmo_flg[i] = 1;
			mupti_dm1_tmo_flg[i] = 1;
			
		}
	}
	
	canNodeTmoCheck();
	
}


static void canRXFrame_DM1_Handler(uint32_t id, uint8_t *buf)
{
	static uint8_t  MuptiErrBuf[MAX_ERR_NUM * 4] = {0};
	static uint8_t  MuptiErrPcktNum[DM1_OWNER_NUM] = {0};
	static uint8_t  MuptiErrPcktFlg[DM1_OWNER_NUM] = {0};
	static uint8_t  Broadcast;
	static uint16_t EffectiveBytes;
	static uint8_t  FltPacketsNum;
	static uint32_t PGN;
	
	uint8_t  bms_flt_lamp_flg = 0;
	uint8_t  atm_flt_lamp_flg = 0;
	// uint8_t  mcu_flt_lamp_flg = 0;
	uint8_t  owner = DM1_NULL;
	
	J1939_CAN_ID_t canID;
	
	canID.id = id;
	
	if(canID.BIT.Priority != 6)
	{
		return;
	}
	
	switch(canID.BIT.SA)
	{
		case 0x82: owner = DM1_82; break;
		case 0x05: owner = DM1_05; break;
		case 0x03: owner = DM1_03; break;
		case 0xF4: owner = DM1_F4; break;
		case 0xEF: owner = DM1_EF; break;
		case 0xF0: owner = DM1_F0; break;
		case 0x31: owner = DM1_31; break;
		case 0xF5: owner = DM1_F5; break;
		case 0xF6: owner = DM1_F6; break;
		case 0x0B: owner = DM1_0B; break;
		case 0xE8: owner = DM1_E8; break;
		case 0x2A: owner = DM1_2A; break;
		case 0xA0: owner = DM1_A0; break;
		case 0xA7: owner = DM1_A7; break;
		case 0x50: owner = DM1_50; break;
		case 0x84: owner = DM1_84; break;
		case 0x85: owner = DM1_85; break;
		case 0xDB: owner = DM1_DB; break;
		case 0xA8: owner = DM1_A8; break;
		case 0x8F: owner = DM1_8F; break;
		case 0x13: owner = DM1_13; break;
		
		// case 0x10: owner = DM1_10; break;
		// case 0x53: owner = DM1_53; break;
		
		
		case 0x41: owner = DM1_41; break;
		case 0xE7: owner = DM1_E7; break;
		case 0x33: owner = DM1_33; break;
		case 0xEC: owner = DM1_EC; break;
		case 0x32: owner = DM1_32; break;
		case 0x2E: owner = DM1_2E; break;
		case 0x4D: owner = DM1_4D; break;
		case 0x1E: owner = DM1_1E; break;
		case 0x30: owner = DM1_30; break;
		case 0x1A: owner = DM1_1A; break;
		case 0x2F: owner = DM1_2F; break;
		
		default:
			owner = DM1_NULL;
			break;
	}
	
	if(owner == DM1_NULL)
	{
		return;
	}
	
	switch(canID.BIT.PDU)
    {
		case 0xFECA:
			single_dm1_tmo_flg[owner] = 0;
			CurrentFltInfo[owner].clearfltflg = 0;
			CurrentFltInfo[owner].SA = canID.BIT.SA;
			CurrentFltInfo[owner].FltBuf.byte[0]    = buf[0];
			CurrentFltInfo[owner].FltBuf.byte[1]    = buf[1];
			CurrentFltInfo[owner].FltBuf.DTC[0].SPN = buf[2] + buf[3]*256 + (buf[4]&0xE0)*2048;
			CurrentFltInfo[owner].FltBuf.DTC[0].FMI = (buf[4]&0x1F);
			CurrentFltInfo[owner].FltBuf.DTC[0].OC  = (buf[5]&0x7F);
			CurrentFltInfo[owner].FltBuf.DTC[0].CM  = (buf[5]&0x80)>>7;
			
			if(CurrentFltInfo[owner].FltBuf.DTC[0].SPN > 0)
				CurrentFltInfo[owner].FltNum = 1;
			else
				CurrentFltInfo[owner].FltNum = 0;
			
			for(uint8_t i = 1; i < MAX_ERR_NUM; i++)
			{
				CurrentFltInfo[owner].FltBuf.DTC[i].SPN = 0;
				CurrentFltInfo[owner].FltBuf.DTC[i].FMI = 0;
				CurrentFltInfo[owner].FltBuf.DTC[i].OC  = 0;
				CurrentFltInfo[owner].FltBuf.DTC[i].CM  = 0;
			}
			
			MuptiErrPcktFlg[owner] = 0;
			MuptiErrPcktNum[owner] = 0;
			Broadcast = 0;
			EffectiveBytes = 0;
			FltPacketsNum = 0;
			PGN = 0;
			
			if(CurrentFltInfo[owner].SA == 0xF4)
			{
				if(CurrentFltInfo[owner].FltBuf.DTC[0].SPN == 522243)
				{
					if((CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 15) \
					|| (CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 16) \
					|| (CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 0) \
					) {
						CurrentFltInfo[owner].FltBuf.byte[0] = 0;
						CurrentFltInfo[owner].FltBuf.byte[1] = 0;
					}
				} 
				/* else if(CurrentFltInfo[owner].FltBuf.DTC[0].SPN == 521308)
				{
					if((CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 15) \
					|| (CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 16) \
					|| (CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 0) \
					) {
						CurrentFltInfo[owner].FltBuf.byte[0] = 0;
						CurrentFltInfo[owner].FltBuf.byte[1] = 0;
					}
				} */
				/* else if((CurrentFltInfo[owner].FltBuf.DTC[0].SPN >= 522237) \
				&& (CurrentFltInfo[owner].FltBuf.DTC[0].SPN <= 522244))
				{
					if((CurrentFltInfo[owner].FltBuf.DTC[0].SPN != 522240) \
					&& (CurrentFltInfo[owner].FltBuf.DTC[0].SPN != 5))
					{
						CurrentFltInfo[owner].FltBuf.byte[0] = 0;
						CurrentFltInfo[owner].FltBuf.byte[1] = 0;
					}
				} */
			}
			else if(CurrentFltInfo[owner].SA == 0x03)
			{
				if((CurrentFltInfo[owner].FltBuf.DTC[0].SPN >= 520200) \
				&& (CurrentFltInfo[owner].FltBuf.DTC[0].SPN <= 520213) \
				&& (CurrentFltInfo[owner].FltBuf.DTC[0].SPN != 520206))
				{
					if(CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 7)
					{
						CurrentFltInfo[owner].FltBuf.byte[0] = 0;
						CurrentFltInfo[owner].FltBuf.byte[1] = 0;
					}
				}
			}
			/* else if(CurrentFltInfo[owner].SA == 0xEF || CurrentFltInfo[owner].SA == 0xF0)
			{
				if(CurrentFltInfo[owner].FltBuf.DTC[0].SPN == 521703)
				{
					if(CurrentFltInfo[owner].FltBuf.DTC[0].FMI == 16)
					{
						CurrentFltInfo[owner].FltBuf.byte[0] = 0;
						CurrentFltInfo[owner].FltBuf.byte[1] = 0;
					}
				}
			} */
			else ;
			break;
		case 0xECFF:
			Broadcast = buf[0];
			EffectiveBytes = buf[1] + buf[2] * 256;
			FltPacketsNum = buf[3];
			PGN = buf[5] + buf[6] *256 + buf[7] * 65536;
			
			if(Broadcast == 0x20
			&& EffectiveBytes >= 10
			&& FltPacketsNum >= 2
			&& PGN == 0x00FECA )
			{
				MuptiErrPcktFlg[owner] = 1;
				CurrentFltInfo[owner].FltNum = (EffectiveBytes - 2) / 4;
				if(CurrentFltInfo[owner].FltNum > MAX_ERR_NUM)
					CurrentFltInfo[owner].FltNum = MAX_ERR_NUM;
				MuptiErrPcktNum[owner] = FltPacketsNum;
				if(MuptiErrPcktNum[owner] > MultipDM1_MaxErrPacketNum)
					MuptiErrPcktNum[owner] = MultipDM1_MaxErrPacketNum;
			}
			break;
		case 0xEBFF:
			if(Broadcast == 0x20
			&& EffectiveBytes >= 10
			&& FltPacketsNum >= 2
			&& PGN == 0x00FECA )
			{
				if(CurrentFltInfo[owner].FltNum == 0 || MuptiErrPcktNum[owner] == 0 || MuptiErrPcktFlg[owner] == 0)
					break;
				
				mupti_dm1_tmo_flg[owner] = 0;
				CurrentFltInfo[owner].clearfltflg = 0;
				CurrentFltInfo[owner].SA = canID.BIT.SA;
				
				if(buf[0] == 1)
				{
					CurrentFltInfo[owner].FltBuf.byte[0] = buf[1];
					CurrentFltInfo[owner].FltBuf.byte[1] = buf[2];
					
					memcpy(&MuptiErrBuf[0], buf+3, 5);
					
				}
				else if(buf[0] <= MultipDM1_MaxErrPacketNum)
				{
					memcpy(&MuptiErrBuf[7*(buf[0] - 2) + 5], buf+1, 7);
				}
				else ;
				
				if(buf[0] == MuptiErrPcktNum[owner])
				{
					for(uint8_t i = 0; i < MAX_ERR_NUM; i++)
					{
						if(i < CurrentFltInfo[owner].FltNum)
						{
							CurrentFltInfo[owner].FltBuf.DTC[i].SPN = (MuptiErrBuf[i*4] + MuptiErrBuf[i*4+1]*256 + (MuptiErrBuf[i*4+2]&0xE0)*2048);
							CurrentFltInfo[owner].FltBuf.DTC[i].FMI = (MuptiErrBuf[i*4+2]&0x1F);
							CurrentFltInfo[owner].FltBuf.DTC[i].OC  = (MuptiErrBuf[i*4+3]&0x7F);
							CurrentFltInfo[owner].FltBuf.DTC[i].CM  = (MuptiErrBuf[i*4+3]&0x80)>>7;
							
							if(CurrentFltInfo[owner].SA == 0xF4)
							{
								if(CurrentFltInfo[owner].FltBuf.DTC[i].SPN == 522243)
								{
									if((CurrentFltInfo[owner].FltBuf.DTC[i].FMI != 15) \
									&& (CurrentFltInfo[owner].FltBuf.DTC[i].FMI != 16) \
									&& (CurrentFltInfo[owner].FltBuf.DTC[i].FMI != 0) \
									) {
										bms_flt_lamp_flg = 1;
									}
								}
								/* else if(CurrentFltInfo[owner].FltBuf.DTC[0].SPN == 521308)
								{
									if((CurrentFltInfo[owner].FltBuf.DTC[0].FMI != 15) \
									&& (CurrentFltInfo[owner].FltBuf.DTC[0].FMI != 16) \
									&& (CurrentFltInfo[owner].FltBuf.DTC[0].FMI != 0) \
									) {
										bms_flt_lamp_flg = 1;
									}
								} */
								else
								{
									bms_flt_lamp_flg = 1;
								}
								
								/* if((CurrentFltInfo[owner].FltBuf.DTC[i].SPN < 522237) || (CurrentFltInfo[owner].FltBuf.DTC[i].SPN > 522244) \
								|| (CurrentFltInfo[owner].FltBuf.DTC[i].SPN == 522240) || (CurrentFltInfo[owner].FltBuf.DTC[i].SPN == 5))
								{
									bms_flt_lamp_flg = 1;
								} */
							}
							else if(CurrentFltInfo[owner].SA == 0x03)
							{
								if((CurrentFltInfo[owner].FltBuf.DTC[i].SPN >= 520200) \
								&& (CurrentFltInfo[owner].FltBuf.DTC[i].SPN <= 520213) \
								&& (CurrentFltInfo[owner].FltBuf.DTC[i].SPN != 520206))
								{
									if(CurrentFltInfo[owner].FltBuf.DTC[i].FMI != 7)
									{
										atm_flt_lamp_flg = 1;
									}
								}
								else
								{
									atm_flt_lamp_flg = 1;
								}
							}
							/* else if(CurrentFltInfo[owner].SA == 0xEF || CurrentFltInfo[owner].SA == 0xF0)
							{
								if(CurrentFltInfo[owner].FltBuf.DTC[0].SPN == 521703)
								{
									if(CurrentFltInfo[owner].FltBuf.DTC[0].FMI != 16)
									{
										mcu_flt_lamp_flg = 0;
									}
								}
							} */
							else ;
						}
						else
						{
							CurrentFltInfo[owner].FltBuf.DTC[i].SPN = 0;
							CurrentFltInfo[owner].FltBuf.DTC[i].FMI = 0;
							CurrentFltInfo[owner].FltBuf.DTC[i].OC  = 0;
							CurrentFltInfo[owner].FltBuf.DTC[i].CM  = 0;
						}
					}
					
					if(CurrentFltInfo[owner].SA == 0xF4)
					{
						if(!bms_flt_lamp_flg)
						{
							CurrentFltInfo[owner].FltBuf.byte[0] = 0;
							CurrentFltInfo[owner].FltBuf.byte[1] = 0;
						}
						
					}
					else if(CurrentFltInfo[owner].SA == 0x03)
					{
						if(!atm_flt_lamp_flg)
						{
							CurrentFltInfo[owner].FltBuf.byte[0] = 0;
							CurrentFltInfo[owner].FltBuf.byte[1] = 0;
						}
						
					}
					/* else if(CurrentFltInfo[owner].SA == 0xEF || CurrentFltInfo[owner].SA == 0xF0)
					{
						if(!mcu_flt_lamp_flg)
						{
							CurrentFltInfo[owner].FltBuf.byte[0] = 0;
							CurrentFltInfo[owner].FltBuf.byte[1] = 0;
						}
						
					} */
					else ;
				}
			}
			break;
		default:
			
			break;
    }
	
}
static void canRXFrame_LockVeh_Handler(uint32_t id, uint8_t *buf)
{
	uint8_t func = 0;
	uint8_t LockVehSt = 0;
	uint8_t LmtSpdSt = 0;
	uint8_t LmtSpdWarnSt = 0;
	
	switch(id)
    {
		case 0x18FF7FDE:
			isOverseasVer = (buf[7] == 0);
			break;
		case 0x18FF7C4A:
			// 0x18FF7C4A->byte8 == 0xFF 时，确认为中联锁车方案，仪表界面不进行锁车相关提示
			ZhongLianLockVehFlg = (buf[7] == 0xFF);
			break;
        case 0x18FF1F31:
            func = buf[0];
			switch(func)
			{
				case 0x81: 
					if(buf[1] == 1) //绑定成功
					{
						LckVehBoundState = 1;
						LckVehdiplayState = 9;  //车辆受控中！
						
						LockVehBound_cnt = 0;
						ptmr_Stop(&LockVehBoundTmr);
					}
					else if(buf[1] == 0) //绑定失败
					{
						LckVehBoundState = 0;
						LckVehdiplayState = 0;  //NULL
						
						LockVehSt = 0;
						Remaining_unlocked_times = 0;
						SuperviseMode = 0;
						LmtSpdWarnSt = 0;
						LmtSpdSt = 0;
						
						LockVehBound_cnt = 0;
						ptmr_Stop(&LockVehBoundTmr);
					}
					else ;
					break;
				case 0x82: 
					if(buf[1] == 1) //解绑成功
					{
						LckVehBoundState = 0;
						LckVehdiplayState = 0;  //NULL
						
						LockVehSt = 0;
						Remaining_unlocked_times = 0;
						SuperviseMode = 0;
						LmtSpdWarnSt = 0;
						LmtSpdSt = 0;
						
						LockVehBound_cnt = 0;
						ptmr_Stop(&LockVehBoundTmr);
					}
					break;
				case 0x85: 
					if(LckVehBoundState == 0)
					{
						if(!ptmr_IsStart(&LockVehBoundTmr))
							ptmr_Start(&LockVehBoundTmr);
						if(ptmr_Get(&LockVehBoundTmr) < 180000)
						{
							LockVehBound_cnt++;
						}
						if(LockVehBound_cnt >= 3)
						{
							LckVehBoundState = 1;
							LckVehdiplayState = 9;  //车辆受控中！
							
							LockVehBound_cnt = 0;
							ptmr_Stop(&LockVehBoundTmr);
						}
						else
						{
							LckVehBoundState = 0;
							LckVehdiplayState = 0;  //NULL
						}
					}
					else
					{
						LockVehSt = (buf[4]&0x0f);
						Remaining_unlocked_times = ((buf[4]>>5)&0x07);
						SuperviseMode = (buf[6]&0x03);
						LmtSpdWarnSt = ((buf[6]>>4)&0x03);
						LmtSpdSt = ((buf[6]>>6)&0x03);
						
						if(LockVehSt == 7)
						{
							LckVehdiplayState = 1;  //临时解除限速输入模式
						}
						else if(LockVehSt == 9)
						{
							LckVehdiplayState = 2;  //临时解除限速失败,限速状态已激活
						}
						else if(LockVehSt == 8)
						{
							LckVehdiplayState = 3;  //车辆已临时解除限速,请注意安全!
						}
						else
						{
							if((SuperviseMode == 1) && (LmtSpdSt == 1))
							{
								LckVehdiplayState = 4;  //车辆即将进入锁车状态,请注意安全!
							}
							else if((SuperviseMode == 2) && (LmtSpdSt == 1))
							{
								LckVehdiplayState = 5;  //车辆限速即将激活,熄火前请联系4009005555
							}
							else if((SuperviseMode == 1) && (LmtSpdSt == 2))
							{
								LckVehdiplayState = 6;  //车辆已进入锁车状态,请注意安全!
							}
							else if((SuperviseMode == 2) && (LmtSpdSt == 2))
							{
								LckVehdiplayState = 7;  //车辆限速已激活,请联系4009005555
							}
							else if((SuperviseMode == 2) && (LmtSpdWarnSt != 0))
							{
								LckVehdiplayState = 8;  //限速预警,请联系4009005555
							}
							else if((SuperviseMode == 1) && (LmtSpdSt == 0))
							{
								LckVehdiplayState = 9;  //车辆受控中!   且仪表在菜单-车辆信息-K 值界面显示 “监管模式”
							}
							else if((SuperviseMode == 2) && (LmtSpdSt == 0))
							{
								LckVehdiplayState = 10; //仪表仅在菜单-车辆信息-K 值界面显示 "监管模式"
							}
						}
						
					}
					break;
				default:
					
					break;
			}
			break;
		default:
			
			break;
    }
	
}
static void canRXFrame_recvHandle(uint32_t id, uint8_t *buf)
{
	uint8_t door_current_state = 0;
	uint8_t temp = 0;
	
	switch(id)
	{
		case 0x11FF3F32:
			SmartRechgCfgFlg = ((buf[0]&0x03) == 0x02);
			break;
		case 0x18FF2232:
			CAN_KEY_ACC = ((buf[0]&0x30) == 0x10);
			door_current_state  = (((buf[4]&0x0C) == 0x04) || ((buf[4]&0x30) == 0x10));
			
			if(!M_ON && CAN_KEY_ACC)
			{
				if(door_current_state)  /* Door ON */
				{
					if(GetOffReminderStatus == 0 && door_previous_state == 0)
					{
						if(!ptmr_IsStart(&door_state_tmr))
							ptmr_Start(&door_state_tmr);
						
						if(ptmr_Get(&door_state_tmr) >= 1000)
						{
							ptmr_Stop(&door_state_tmr);
							GetOffReminderStatus = 1;
							
							door_previous_state = 1;
						}
					}
				}
				else /* Door OFF */
				{
					door_previous_state = 0;
					GetOffReminderStatus = 0;
					ptmr_Stop(&door_state_tmr);
				}
			}
			else 
			{
				GetOffReminderStatus = 0;
				ptmr_Stop(&door_state_tmr);
				
				if(M_ON && door_current_state)
				{
					door_previous_state = 1;
				}
			}
			break;
		case 0x18FED631:
			MSD_SwitchStatus  = ((buf[3]&0x30) == 0x10);
			break;
		case 0x18FFF531:
    		vcu_ready_18fff531 = ((buf[2]&0x10) == 0x10);
    		VehicleReadyFlag = (vcu_ready_18fff531 || vcu_ready_04f02270);
    		break;
		case 0x04F02270://lyx
    		vcu_ready_04f02270 = ((buf[0]&0x01) == 0x01);
    		VehicleReadyFlag = (vcu_ready_18fff531 || vcu_ready_04f02270);
    		break;
		case 0x04F02370://lyx
			CAN_CHARGE_LINE = ((buf[1]&0x03) != 0);
			break;
		case 0x0CFFEAF4:
			temp = (buf[0]&0x60);
			CAN_CHARGE_LINE = (temp == 0x40);
			BMS_SAFETY_LINE_SG = ((buf[0]&0x10) == 0);
			break;
		case 0x18FFA6F5:
			temp = (buf[6]&0x03);
			CAN_H2_ChuiSao   = (temp == 1);
			CAN_H2_LowTptChg = (temp == 2);
			break;
		case 0x18FEE6EE:
			TD_MsgEnable = 1;
			break;
		default:
			break;
	}
	
}


uint8_t * can_getPCanBuffer(uint32_t canid)
{
    uint8_t * ret = NULL;

    for (int32_t i = 0; i <PCAN_RX_FRAME_COUNT; i ++)
    {
        if (l_tPCanRxFrame.lcfg[i].id == canid)
        {
            ret = l_tPCanRxFrame.buffer[i];
            break;
        }
    }

    return ret;
}
uint8_t can_getPCanRxState(uint32_t canid)
{
    uint8_t ret = CAN_FRAME_ST_INIT;

    for (int32_t i = 0; i < PCAN_RX_FRAME_COUNT; i ++)
    {
        if (l_tPCanRxFrame.lcfg[i].id == canid)
        {
            ret = l_tPCanRxFrame.st[i];
            break;
        }
    }
	
    return ret;
}
uint8_t * can_getBCanBuffer(uint32_t canid)
{
    uint8_t * ret = NULL;

    for (int32_t i = 0; i <BCAN_RX_FRAME_COUNT; i ++)
    {
        if (l_tBCanRxFrame.lcfg[i].id == canid)
        {
            ret = l_tBCanRxFrame.buffer[i];
            break;
        }
    }

    return ret;
}
uint8_t can_getBCanRxState(uint32_t canid)
{
    uint8_t ret = CAN_FRAME_ST_INIT;

    for (int32_t i = 0; i < BCAN_RX_FRAME_COUNT; i ++)
    {
        if (l_tBCanRxFrame.lcfg[i].id == canid)
        {
            ret = l_tBCanRxFrame.st[i];
            break;
        }
    }

    return ret;
}


uint8_t *can_GetCanRxBuffer(CanNode_e node_index, uint32_t canid)
{
    uint8_t * ret = NULL;
	
	switch(node_index)
	{
		case NODE_BCAN:
			for (int32_t i = 0; i <BCAN_RX_FRAME_COUNT; i ++)
			{
				if (l_tBCanRxFrame.lcfg[i].id == canid)
				{
					if(l_tBCanRxFrame.st[i] == CAN_FRAME_ST_RECVED)
						ret = l_tBCanRxFrame.buffer[i];
					break;
				}
			}
			break;
		case NODE_PCAN:
			for (int32_t i = 0; i <PCAN_RX_FRAME_COUNT; i ++)
			{
				if (l_tPCanRxFrame.lcfg[i].id == canid)
				{
					if(l_tPCanRxFrame.st[i] == CAN_FRAME_ST_RECVED)
						ret = l_tPCanRxFrame.buffer[i];
					break;
				}
			}
			break;
		default:
			break;
		
	}
    
    return ret;
	
}
uint8_t can_GetCanRxState(CanNode_e node_index, uint32_t canid)
{
    uint8_t ret = CAN_FRAME_ST_INIT;
	
	switch(node_index)
	{
		case NODE_BCAN:
			for (int32_t i = 0; i <BCAN_RX_FRAME_COUNT; i ++)
			{
				if (l_tBCanRxFrame.lcfg[i].id == canid)
				{
					ret = l_tBCanRxFrame.st[i];
					break;
				}
			}
			break;
		case NODE_PCAN:
			for (int32_t i = 0; i <PCAN_RX_FRAME_COUNT; i ++)
			{
				if (l_tPCanRxFrame.lcfg[i].id == canid)
				{
					ret = l_tPCanRxFrame.st[i];
					break;
				}
			}
			break;
		default:
			break;
		
	}
    
    return ret;
	
}



void BCAN_StartTime(uint32_t id)
{
	for (int32_t i = 0; i < BCAN_RX_FRAME_COUNT; i ++)
	{
		if (l_tBCanRxFrame.lcfg[i].id == id)
		{
			ptmr_Start(&l_tBCanRxFrame.timer[i]);        // 清零计数器
			l_tBCanRxFrame.st[i] = CAN_FRAME_ST_INIT;    // 初始化状态
		}
	}
}
void PCAN_StartTime(uint32_t id)
{
	for (int32_t i = 0; i < PCAN_RX_FRAME_COUNT; i ++)
	{
		if (l_tPCanRxFrame.lcfg[i].id == id)
		{
			ptmr_Start(&l_tPCanRxFrame.timer[i]);        // 清零计数器
			l_tPCanRxFrame.st[i] = CAN_FRAME_ST_INIT;    // 初始化状态
		}
	}
}


/**
 * uds_get_frame - uds get a can frame, then transmit to network
 *
 * @func_addr : 0 - physical addr, 1 - functional addr
 * @frame_buf : uds can frame data buffer
 * @frame_dlc : uds can frame length
 *
 * returns:
 *     void
 */
void PCAN_UDS_SEND (uint8_t msg_buf[], uint8_t length)
{
	CAN_MESSAGE msg;
	
	if(length < 9)
		memcpy(&msg.data[0], msg_buf, length);
	else
		return;
	msg.id = 0x18DAF117;
	msg.len = 0x08;
	msg.type = 0x00;
	if(uds_can == 1)
	{
		CAN1_SendFIFOMessage(&msg);
	}
	else if(uds_can == 2)
	{
		CAN2_SendFIFOMessage(&msg);
	}
	else;
	
}
void PCAN_Send_Route(uint32_t id, uint8_t *buf)
{
	CAN_MESSAGE msg;
	msg.id = id;
	memcpy(&msg.data,buf,8);
	msg.len = 0x08;
	msg.type = 0x00;
	CAN2_SendFIFOMessage(&msg);
}
void BCAN_Send_Route(uint32_t id, uint8_t *buf)
{
    CAN_MESSAGE msg;
    memcpy(&msg.data,buf,8);
    msg.id = id;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
}

void tpms_LearningReadBack(uint32_t id, uint8_t *buf)
{
    if((id == 0x18FFE833)&&(study == 1))
	{
		backvalue = (buf[1]&0x0f);
    }
    else
		return;
}
void can_processTPMS(uint32_t id, uint8_t *buf)
{
    if(buf[0]==0x17)
	{
		wheelupdated |= 0x01;
		wheelupdatedtimer[0][0] = 0;
		memcpy(&Wheels[0][0],buf,8);
		
		TyreLocation_pressureh[0].bit0   = (Wheels[0][0].pressure_status == 0x02)||(Wheels[0][0].pressure_status == 0x03);
		TyreLocation_pressurel[0].bit0   = (Wheels[0][0].pressure_status == 0x05)||(Wheels[0][0].pressure_status == 0x06);;
		TyreLocation_fastlostgas[0].bit0 = (Wheels[0][0].fast_pressure_lost==1);
		TyreLocation_temperature[0].bit0 = (Wheels[0][0].over_temperature_status == 0x01);
		
    }
    else if(buf[0]==0x19)
	{
		wheelupdated |= 0x02;
		wheelupdatedtimer[0][1] = 0;
		memcpy(&Wheels[0][1],buf,8);
		
		TyreLocation_pressureh[0].bit1   = (Wheels[0][1].pressure_status == 0x02)||(Wheels[0][1].pressure_status == 0x03);
		TyreLocation_pressurel[0].bit1   = (Wheels[0][1].pressure_status == 0x05)||(Wheels[0][1].pressure_status == 0x06);;
		TyreLocation_fastlostgas[0].bit1 = (Wheels[0][1].fast_pressure_lost==1);
		TyreLocation_temperature[0].bit1 = (Wheels[0][1].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x26)
	{
		wheelupdated |= 0x04;
		wheelupdatedtimer[1][0] = 0;
		memcpy(&Wheels[1][0],buf,8);
		
		TyreLocation_pressureh[1].bit0   = (Wheels[1][0].pressure_status == 0x02)||(Wheels[1][0].pressure_status == 0x03);
		TyreLocation_pressurel[1].bit0   = (Wheels[1][0].pressure_status == 0x05)||(Wheels[1][0].pressure_status == 0x06);;
		TyreLocation_fastlostgas[1].bit0 = (Wheels[1][0].fast_pressure_lost==1);
		TyreLocation_temperature[1].bit0 = (Wheels[1][0].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x27)
	{
		wheelupdated |= 0x08;
		wheelupdatedtimer[1][1] = 0;
		memcpy(&Wheels[1][1],buf,8);
		
		TyreLocation_pressureh[1].bit1   = (Wheels[1][1].pressure_status == 0x02)||(Wheels[1][1].pressure_status == 0x03);
		TyreLocation_pressurel[1].bit1   = (Wheels[1][1].pressure_status == 0x05)||(Wheels[1][1].pressure_status == 0x06);;
		TyreLocation_fastlostgas[1].bit1 = (Wheels[1][1].fast_pressure_lost==1);
		TyreLocation_temperature[1].bit1 = (Wheels[1][1].over_temperature_status == 0x01);
    }
	else if(buf[0]==0x29)
	{
		wheelupdated |= 0x10;
		wheelupdatedtimer[1][2] = 0;
		memcpy(&Wheels[1][2],buf,8);
		
		TyreLocation_pressureh[1].bit2   = (Wheels[1][2].pressure_status == 0x02)||(Wheels[1][2].pressure_status == 0x03);
		TyreLocation_pressurel[1].bit2   = (Wheels[1][2].pressure_status == 0x05)||(Wheels[1][2].pressure_status == 0x06);;
		TyreLocation_fastlostgas[1].bit2 = (Wheels[1][2].fast_pressure_lost==1);
		TyreLocation_temperature[1].bit2 = (Wheels[1][2].over_temperature_status == 0x01);
		
    }
    else if(buf[0]==0x2A)
	{
		wheelupdated |= 0x20;
		wheelupdatedtimer[1][3] = 0;
		memcpy(&Wheels[1][3],buf,8);
		
		TyreLocation_pressureh[1].bit3   = (Wheels[1][3].pressure_status == 0x02)||(Wheels[1][3].pressure_status == 0x03);
		TyreLocation_pressurel[1].bit3   = (Wheels[1][3].pressure_status == 0x05)||(Wheels[1][3].pressure_status == 0x06);;
		TyreLocation_fastlostgas[1].bit3 = (Wheels[1][3].fast_pressure_lost==1);
		TyreLocation_temperature[1].bit3 = (Wheels[1][3].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x36)
	{
		wheelupdatedtimer[2][0] = 0;
		wheelupdated |= 0x40;
		memcpy(&Wheels[2][0],buf,8);
		
		TyreLocation_pressureh[2].bit0   = (Wheels[2][0].pressure_status == 0x02)||(Wheels[2][0].pressure_status == 0x03);
		TyreLocation_pressurel[2].bit0   = (Wheels[2][0].pressure_status == 0x05)||(Wheels[2][0].pressure_status == 0x06);;
		TyreLocation_fastlostgas[2].bit0 = (Wheels[2][0].fast_pressure_lost==1);
		TyreLocation_temperature[2].bit0 = (Wheels[2][0].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x37)
	{
		wheelupdatedtimer[2][1] = 0;
		wheelupdated |= 0x80;
		memcpy(&Wheels[2][1],buf,8);
		
		TyreLocation_pressureh[2].bit1   = (Wheels[2][1].pressure_status == 0x02)||(Wheels[2][1].pressure_status == 0x03);
		TyreLocation_pressurel[2].bit1   = (Wheels[2][1].pressure_status == 0x05)||(Wheels[2][1].pressure_status == 0x06);;
		TyreLocation_fastlostgas[2].bit1 = (Wheels[2][1].fast_pressure_lost==1);
		TyreLocation_temperature[2].bit1 = (Wheels[2][1].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x39)
	{
		wheelupdatedtimer[2][2] = 0;
		wheelupdated |= 0x100;
		memcpy(&Wheels[2][2],buf,8);
		
		TyreLocation_pressureh[2].bit2   = (Wheels[2][2].pressure_status == 0x02)||(Wheels[2][2].pressure_status == 0x03);
		TyreLocation_pressurel[2].bit2   = (Wheels[2][2].pressure_status == 0x05)||(Wheels[2][2].pressure_status == 0x06);;
		TyreLocation_fastlostgas[2].bit2 = (Wheels[2][2].fast_pressure_lost==1);
		TyreLocation_temperature[2].bit2 = (Wheels[2][2].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x3A)
	{
		wheelupdatedtimer[2][3] = 0;
		wheelupdated |= 0x200;
		memcpy(&Wheels[2][3],buf,8);
		
		TyreLocation_pressureh[2].bit3   = (Wheels[2][3].pressure_status == 0x02)||(Wheels[2][3].pressure_status == 0x03);
		TyreLocation_pressurel[2].bit3   = (Wheels[2][3].pressure_status == 0x05)||(Wheels[2][3].pressure_status == 0x06);;
		TyreLocation_fastlostgas[2].bit3 = (Wheels[2][3].fast_pressure_lost==1);
		TyreLocation_temperature[2].bit3 = (Wheels[2][3].over_temperature_status == 0x01);
    }
	else if(buf[0]==0x46)
	{
		wheelupdatedtimer[3][0] = 0;
		wheelupdated |= 0x400;
		memcpy(&Wheels[3][0],buf,8);
		
		TyreLocation_pressureh[3].bit0   = (Wheels[3][0].pressure_status == 0x02)||(Wheels[3][0].pressure_status == 0x03);
		TyreLocation_pressurel[3].bit0   = (Wheels[3][0].pressure_status == 0x05)||(Wheels[3][0].pressure_status == 0x06);;
		TyreLocation_fastlostgas[3].bit0 = (Wheels[3][0].fast_pressure_lost==1);
		TyreLocation_temperature[3].bit0 = (Wheels[3][0].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x47)
	{
		wheelupdatedtimer[3][1] = 0;
		wheelupdated |= 0x800;
		memcpy(&Wheels[3][1],buf,8);
		
		TyreLocation_pressureh[3].bit1   = (Wheels[3][1].pressure_status == 0x02)||(Wheels[3][1].pressure_status == 0x03);
		TyreLocation_pressurel[3].bit1   = (Wheels[3][1].pressure_status == 0x05)||(Wheels[3][1].pressure_status == 0x06);;
		TyreLocation_fastlostgas[3].bit1 = (Wheels[3][1].fast_pressure_lost==1);
		TyreLocation_temperature[3].bit1 = (Wheels[3][1].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x49)
	{
		wheelupdatedtimer[3][2] = 0;
		wheelupdated |= 0x1000;
		memcpy(&Wheels[3][2],buf,8);
		
		TyreLocation_pressureh[3].bit2   = (Wheels[3][2].pressure_status == 0x02)||(Wheels[3][2].pressure_status == 0x03);
		TyreLocation_pressurel[3].bit2   = (Wheels[3][2].pressure_status == 0x05)||(Wheels[3][2].pressure_status == 0x06);;
		TyreLocation_fastlostgas[3].bit2 = (Wheels[3][2].fast_pressure_lost==1);
		TyreLocation_temperature[3].bit2 = (Wheels[3][2].over_temperature_status == 0x01);
    }
    else if(buf[0]==0x4A)
	{
		wheelupdatedtimer[3][3] = 0;
		wheelupdated |= 0x2000;
		memcpy(&Wheels[3][3],buf,8);
		
		TyreLocation_pressureh[3].bit3   = (Wheels[3][3].pressure_status == 0x02)||(Wheels[3][3].pressure_status == 0x03);
		TyreLocation_pressurel[3].bit3   = (Wheels[3][3].pressure_status == 0x05)||(Wheels[3][3].pressure_status == 0x06);;
		TyreLocation_fastlostgas[3].bit3 = (Wheels[3][3].fast_pressure_lost==1);
		TyreLocation_temperature[3].bit3 = (Wheels[3][3].over_temperature_status == 0x01);
    }
    else;

}

uint32_t ICU_DM1_Data_Conversion(DTC_BUF_t dtcBuf)
{
	uint32_t u32_buffer = 0;
    
	u32_buffer  = (uint32_t)(dtcBuf.SPN&0x0000ffff);
	u32_buffer += (uint32_t)dtcBuf.FMI << 16;
	u32_buffer += (uint32_t)(dtcBuf.SPN&0x00070000) << 5;
	u32_buffer += (uint32_t)dtcBuf.byte[3] << 24;
	
    return u32_buffer;
}
void PCAN_Send_ICU_DM1(void)  //100ms
{
	static uint32_t icuSendTimer = 0;
	static uint32_t icuSendCnt = 0;
	static uint8_t  validbuffer[ICU_MAX_ERR_NUM * 4];
	CAN_MESSAGE msg;
	uint32_t u32_buffer;
	uint16_t u16_buffer;
	uint8_t  u8_buffer;
	
	if(icuCrtFltNum <= 1)
	{
		if(icuSendTimer > 9)
		{
			icuSendTimer = 0;
			
			if(icuCrtFltNum == 1)
				u32_buffer = ICU_DM1_Data_Conversion(icuCrtDM1Flt[0]);
			else
				u32_buffer = 0;
			
			msg.id = 0x18FECA17;
			msg.len = 0x08;
			msg.type = 0x00;
			msg.data[0] = 0;
			msg.data[1] = 0;
			msg.data[2] = (uint8_t)(u32_buffer);
			msg.data[3] = (uint8_t)(u32_buffer>>8);
			msg.data[4] = (uint8_t)(u32_buffer>>16);
			msg.data[5] = (uint8_t)(u32_buffer>>24);
			msg.data[6] = 0xff;
			msg.data[7] = 0xff;
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
		}
		else icuSendTimer++;
	}
	else
	{
		if(icuSendCnt == 0)
		{
			u16_buffer = ErrValidDataNum(icuCrtFltNum);
			u8_buffer  = DM1ErrPacketNum(icuCrtFltNum);
			
			msg.id = 0x18ECFF17;
			msg.len = 0x08;
			msg.type = 0x00;
			msg.data[0] = 0x20;
			msg.data[1] = (uint8_t)(u16_buffer);    //传输字节数
			msg.data[2] = (uint8_t)(u16_buffer>>8); //传输字节数
			msg.data[3] = (uint8_t)(u8_buffer); //数据包个数
			msg.data[4] = (uint8_t)(0xFF);
			msg.data[5] = (uint8_t)(0xCA);
			msg.data[6] = (uint8_t)(0xFE);
			msg.data[7] = (uint8_t)(0x00);
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
			
			icuSendCnt = 1;
			
			for(uint8_t i = 0; i < ICU_MAX_ERR_NUM; i++)
			{
				if(i < icuCrtFltNum)
				{
					u32_buffer = ICU_DM1_Data_Conversion(icuCrtDM1Flt[i]);
					validbuffer[i * 4 + 0] = (uint8_t)(u32_buffer);
					validbuffer[i * 4 + 1] = (uint8_t)(u32_buffer>>8);
					validbuffer[i * 4 + 2] = (uint8_t)(u32_buffer>>16);
					validbuffer[i * 4 + 3] = (uint8_t)(u32_buffer>>24);
				}
				else
				{
					validbuffer[i * 4 + 0] = (uint8_t)(0xFF);
					validbuffer[i * 4 + 1] = (uint8_t)(0xFF);
					validbuffer[i * 4 + 2] = (uint8_t)(0xFF);
					validbuffer[i * 4 + 3] = (uint8_t)(0xFF);
				}
			}
		}
		else
		{
			msg.id = 0x18EBFF17;
			msg.len = 0x08;
			msg.type = 0x00;
			msg.data[0] = icuSendCnt;
			if(icuSendCnt == 1)
			{
				msg.data[1] = 0x00;
				msg.data[2] = 0x00;
				msg.data[3] = validbuffer[0]; 
				msg.data[4] = validbuffer[1]; 
				msg.data[5] = validbuffer[2]; 
				msg.data[6] = validbuffer[3]; 
				msg.data[7] = validbuffer[4]; 
			}
			else
			{
				for(uint8_t i = 0; i < 7; i++)
					msg.data[i+1]  = validbuffer[5 + i + (icuSendCnt-2) * 7];  
			}
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
			
			icuSendCnt = icuSendCnt % DM1ErrPacketNum(icuCrtFltNum);
			if(icuSendCnt != 0)
				icuSendCnt++;
		}
	}
	
}

void BCAN_Send_GearR(void)  //0x18F00503  100ms
{
    CAN_MESSAGE msg;
	uint8_t temp;
	
    memset(msg.data,0,8);
    msg.id = 0x18F00503;
	
	temp = get_vehicle_current_gear();
	if(temp == 100)
		msg.data[3] = 124;
	else
		msg.data[3] = 125;
	
    msg.len = 0x08;
    msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
}
void BCAN_Send_CCSV(void)   //0x18FEF100  100ms
{
	uint8_t *tmp = NULL;
	
	if(0x01 ==  can_getPCanRxState(0x18FEF131)) //CCSV
	{
		tmp = can_getPCanBuffer(0x18FEF131);
		BCAN_Send_Route(0x18FEF100,tmp);
	}
}
void BCAN_Send_Tipian(void) //0x18FF2C47  1000ms 蹄片信号
{
    CAN_MESSAGE msg;
    memset(msg.data,0,8);
    msg.id = 0x18FF2C47;

    if(eol_mcpbaojing == 1)
	{
    	msg.data[3] = msg.data[3]|(IN17);    //制动蹄片磨损L1
    	
    	msg.data[4] = msg.data[4]|(IN15);    //制动蹄片磨损R1
	}
	else if(eol_mcpbaojing == 3)
	{
    	msg.data[3] = msg.data[3]|(IN17);    //制动蹄片磨损L1
    	msg.data[3] = msg.data[3]|(IN21<<2); //制动蹄片磨损L2
    	
    	msg.data[4] = msg.data[4]|(IN15);    //制动蹄片磨损R1
    	msg.data[4] = msg.data[4]|(IN19<<2); //制动蹄片磨损R2
	}
	else if(eol_mcpbaojing == 5)
	{
    	msg.data[3] = msg.data[3]|(IN17);    //制动蹄片磨损L1
    	msg.data[3] = msg.data[3]|(IN21<<2); //制动蹄片磨损L2
    	msg.data[3] = msg.data[3]|(IN18<<4); //制动蹄片磨损L3
    	
    	msg.data[4] = msg.data[4]|(IN15);    //制动蹄片磨损R1
    	msg.data[4] = msg.data[4]|(IN19<<2); //制动蹄片磨损R2
    	msg.data[4] = msg.data[4]|(IN23<<4); //制动蹄片磨损R3
	}
	else if(eol_mcpbaojing == 7)
	{
    	msg.data[3] = msg.data[3]|(IN17);    //制动蹄片磨损L1
    	msg.data[3] = msg.data[3]|(IN21<<2); //制动蹄片磨损L2
    	msg.data[3] = msg.data[3]|(IN18<<4); //制动蹄片磨损L3
    	msg.data[3] = msg.data[3]|(IN22<<6); //制动蹄片磨损L4
    	
    	msg.data[4] = msg.data[4]|(IN15);    //制动蹄片磨损R1
    	msg.data[4] = msg.data[4]|(IN19<<2); //制动蹄片磨损R2
    	msg.data[4] = msg.data[4]|(IN23<<4); //制动蹄片磨损R3
    	msg.data[4] = msg.data[4]|(IN16<<6); //制动蹄片磨损R4
	}
	else if(eol_mcpbaojing == 9)
	{
    	msg.data[3] = msg.data[3]|(IN17);    //制动蹄片磨损L1
    	msg.data[3] = msg.data[3]|(IN21<<2); //制动蹄片磨损L2
    	msg.data[3] = msg.data[3]|(IN18<<4); //制动蹄片磨损L3
    	msg.data[3] = msg.data[3]|(IN22<<6); //制动蹄片磨损L4
    	
    	msg.data[4] = msg.data[4]|(IN15);    //制动蹄片磨损R1
    	msg.data[4] = msg.data[4]|(IN19<<2); //制动蹄片磨损R2
    	msg.data[4] = msg.data[4]|(IN23<<4); //制动蹄片磨损R3
    	msg.data[4] = msg.data[4]|(IN16<<6); //制动蹄片磨损R4
		
		msg.data[5] = msg.data[5]|(IN27);    //制动蹄片磨损L5
		msg.data[5] = msg.data[5]|(IN13<<2); //制动蹄片磨损R5
		
	}
	else{}
    msg.len = 0x08;
    msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
}
void BCAN_Send_TPMSLearning(void) //0x18FFE817
{
    CAN_MESSAGE msg;
    msg.data[0]  =0;                    
    msg.data[1] = 1;        
    back = 1;
	
	
         if(tire == 1) msg.data[2] = 0x17;       
    else if(tire == 2) msg.data[2] = 0x19;
	else ;
	
	if(eol_drivetype==0)
	{
		     if(tire == 3) msg.data[2] = 0x26;
		else if(tire == 4) msg.data[2] = 0x27;
		else if(tire == 5) msg.data[2] = 0x29;
		else if(tire == 6) msg.data[2] = 0x2A;
		else ;
	}
	else if(eol_drivetype==1)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x36;
		else if(tire == 6) msg.data[2] = 0x37;
		else if(tire == 7) msg.data[2] = 0x39;
		else if(tire == 8) msg.data[2] = 0x3A;
		else ;
	}
	else if(eol_drivetype==2)
	{
		     if(tire == 3) msg.data[2] = 0x26;
		else if(tire == 4) msg.data[2] = 0x27;
		else if(tire == 5) msg.data[2] = 0x29;
		else if(tire == 6) msg.data[2] = 0x2A;
		else if(tire == 7) msg.data[2] = 0x36;
		else if(tire == 8) msg.data[2] = 0x37;
		else if(tire == 9) msg.data[2] = 0x39;
		else if(tire ==10) msg.data[2] = 0x3A;
		else ;
	}
	else if(eol_drivetype==3)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x37;
		else if(tire == 6) msg.data[2] = 0x39;
		else if(tire == 7) msg.data[2] = 0x46;
		else if(tire == 8) msg.data[2] = 0x47;
		else if(tire == 9) msg.data[2] = 0x49;
		else if(tire ==10) msg.data[2] = 0x4A;
		else ;
	}
	else if(eol_drivetype==4)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x36;
		else if(tire == 6) msg.data[2] = 0x37;
		else if(tire == 7) msg.data[2] = 0x39;
		else if(tire == 8) msg.data[2] = 0x3A;
		else if(tire == 9) msg.data[2] = 0x47;
		else if(tire ==10) msg.data[2] = 0x49;
		else ;
	}
	else if(eol_drivetype==5)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x36;
		else if(tire == 6) msg.data[2] = 0x37;
		else if(tire == 7) msg.data[2] = 0x39;
		else if(tire == 8) msg.data[2] = 0x3A;
		else if(tire == 9) msg.data[2] = 0x46;
		else if(tire ==10) msg.data[2] = 0x47;
		else if(tire ==11) msg.data[2] = 0x49;
		else if(tire ==12) msg.data[2] = 0x4A;
		else ;
	}
	else if(eol_drivetype==6)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x37;
		else if(tire == 6) msg.data[2] = 0x39;
		else ;
	}
    msg.data[3] =0;  
	msg.data[4] =0;
    msg.data[5] =0;
    msg.data[6] =0;
    msg.data[7] =0;
    msg.id = 0x18FFE817;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
}
void BCAN_Send_TPMSLearningEnd(void) //0x18FFE817
{
    CAN_MESSAGE msg;
    back = 0;
    msg.data[0] = 0;                    
    msg.data[1] = 0;
	
         if(tire == 1) msg.data[2] = 0x17;       
    else if(tire == 2) msg.data[2] = 0x19;
	else ;
	
	if(eol_drivetype==0)
	{
		     if(tire == 3) msg.data[2] = 0x26;
		else if(tire == 4) msg.data[2] = 0x27;
		else if(tire == 5) msg.data[2] = 0x29;
		else if(tire == 6) msg.data[2] = 0x2A;
		else ;
	}
	else if(eol_drivetype==1)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x36;
		else if(tire == 6) msg.data[2] = 0x37;
		else if(tire == 7) msg.data[2] = 0x39;
		else if(tire == 8) msg.data[2] = 0x3A;
		else ;
	}
	else if(eol_drivetype==2)
	{
		     if(tire == 3) msg.data[2] = 0x26;
		else if(tire == 4) msg.data[2] = 0x27;
		else if(tire == 5) msg.data[2] = 0x29;
		else if(tire == 6) msg.data[2] = 0x2A;
		else if(tire == 7) msg.data[2] = 0x36;
		else if(tire == 8) msg.data[2] = 0x37;
		else if(tire == 9) msg.data[2] = 0x39;
		else if(tire ==10) msg.data[2] = 0x3A;
		else ;
	}
	else if(eol_drivetype==3)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x37;
		else if(tire == 6) msg.data[2] = 0x39;
		else if(tire == 7) msg.data[2] = 0x46;
		else if(tire == 8) msg.data[2] = 0x47;
		else if(tire == 9) msg.data[2] = 0x49;
		else if(tire ==10) msg.data[2] = 0x4A;
		else ;
	}
	else if(eol_drivetype==4)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x36;
		else if(tire == 6) msg.data[2] = 0x37;
		else if(tire == 7) msg.data[2] = 0x39;
		else if(tire == 8) msg.data[2] = 0x3A;
		else if(tire == 9) msg.data[2] = 0x47;
		else if(tire ==10) msg.data[2] = 0x49;
		else ;
	}
	else if(eol_drivetype==5)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x36;
		else if(tire == 6) msg.data[2] = 0x37;
		else if(tire == 7) msg.data[2] = 0x39;
		else if(tire == 8) msg.data[2] = 0x3A;
		else if(tire == 9) msg.data[2] = 0x46;
		else if(tire ==10) msg.data[2] = 0x47;
		else if(tire ==11) msg.data[2] = 0x49;
		else if(tire ==12) msg.data[2] = 0x4A;
		else ;
	}
	else if(eol_drivetype==6)
	{
		     if(tire == 3) msg.data[2] = 0x27;
		else if(tire == 4) msg.data[2] = 0x29;
		else if(tire == 5) msg.data[2] = 0x37;
		else if(tire == 6) msg.data[2] = 0x39;
		else ;
	}
    msg.data[3] =0;  
	msg.data[4] =0;
    msg.data[5] =0;
    msg.data[6] =0;
    msg.data[7] =0;     
    msg.id = 0x18FFE817;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
}
void CAN_Send_TCO1(void)    //0x0CFE6CEE  50ms   输出轴转速和车速
{
	CAN_MESSAGE msg;
	uint16_t u16_rpm;
	uint16_t u16_vspeed;
	u16_rpm = get_shaft_speed();
	u16_vspeed = get_vehicle_speed() * 256;
	
	msg.data[0] = 0;                    
	msg.data[1] = 0;       
	msg.data[2] = 0;       
	msg.data[3] = 0;      
	msg.data[4] = (unsigned char)(u16_rpm); 
	msg.data[5] = (unsigned char)(u16_rpm >> 8); 
	msg.data[6] = (unsigned char)(u16_vspeed);
	msg.data[7] = (unsigned char)(u16_vspeed >> 8);
	msg.id = 0x0CFE6CEE;
	msg.len = 0x08;
	msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
	CAN2_SendFIFOMessage(&msg);
	
}
void CAN_Send_TD(void)      //0x18FEE6EE  1000ms 时间
{
    CAN_MESSAGE msg;
    
	static uint8_t Cntr_10S = 0x00;
	
	if(Cntr_10S < 10)
	{
		Cntr_10S++;
	}
	else
	{
		if(TD_MsgEnable == 0)
		{
			msg.data[0] = bcd2decimal(timer.second) * 4;
			msg.data[1] = bcd2decimal(timer.minute);
			msg.data[2] = bcd2decimal(timer.hour);
			msg.data[3] = bcd2decimal(timer.month);
			msg.data[4] = bcd2decimal(timer.day) * 4;
 			msg.data[5] = bcd2decimal(timer.year) + 2000 - 1985;
			msg.data[6] = 125U;
			msg.data[7] = 125U;
			msg.id = 0x18FEE6EE;
			msg.len = 0x08;
			msg.type = 0x00;
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
	    }
		else
		{
			GeneralUse_t *GU_18FEE6EE_t =NULL;
			GU_18FEE6EE_t =  (GeneralUse_t*)can_getBCanBuffer(0x18FEE6EE);
			msg.data[0] = GU_18FEE6EE_t->byte1.byte;
			msg.data[1] = GU_18FEE6EE_t->byte2.byte;
			msg.data[2] = GU_18FEE6EE_t->byte3.byte;
			msg.data[3] = GU_18FEE6EE_t->byte4.byte;
			msg.data[4] = GU_18FEE6EE_t->byte5.byte;
			msg.data[5] = GU_18FEE6EE_t->byte6.byte;
			msg.data[6] = GU_18FEE6EE_t->byte7.byte;
			msg.data[7] = GU_18FEE6EE_t->byte8.byte;
			msg.id = 0x18FEE6EE;
			msg.len = 0x08;
			msg.type = 0x00;
			CAN2_SendFIFOMessage(&msg);
		}
	}
}
void CAN_Send_AMB(void)     //0x18FEF517  1000ms 环境温度
{
    CAN_MESSAGE msg;
    uint32_t offset ;
    uint32_t currenttemp;
    
    msg.data[0] = 0xCA;                    
    msg.data[1] = 0;       
    msg.data[2] = 0;         
    if((DMPackage[1348].set)||(DMPackage[1347].set)||(DMPackage[1346].set))
	{
		msg.data[4] = 0xFE;
		msg.data[3] = 0x00;
    }
    else
	{
		offset = 8736;
		currenttemp = EevTemp*32 + offset;
		msg.data[3]  = (unsigned char)currenttemp;          
		msg.data[4]  = (unsigned char)(currenttemp>>8);      
    }
    msg.data[5] = 0;      
    msg.data[6] = 0;      
    msg.data[7] = 0;      

    msg.id = 0x18FEF517;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
	CAN2_SendFIFOMessage(&msg);
	
}
void CAN_Send_VDHR(void)    //0x18FEC117  1000ms 里程
{
    CAN_MESSAGE msg;
    uint32_t total_mileage;
    uint32_t subtotal_mileage;

    total_mileage = Miles.total_miles * 20;
    subtotal_mileage = Miles.single_miles * 20;

    msg.data[0] = (uint8_t)total_mileage;
    msg.data[1] = (uint8_t)(total_mileage >> 8);
    msg.data[2] = (uint8_t)(total_mileage >> 16);
    msg.data[3] = (uint8_t)(total_mileage >> 24);

    msg.data[4] = (uint8_t)subtotal_mileage;
    msg.data[5] = (uint8_t)(subtotal_mileage >> 8);
    msg.data[6] = (uint8_t)(subtotal_mileage >> 16);
    msg.data[7] = (uint8_t)(subtotal_mileage >> 24);

    msg.id = 0x18FEC117;
    msg.len = 0x08;
    msg.type = 0x00;

    CAN1_SendFIFOMessage(&msg);
    CAN2_SendFIFOMessage(&msg);
}
void CAN_Send_MotSpeedObj1(void)
{
    CAN_MESSAGE msg;
    VCU_04F02B70_t *pVCU_04F02B70 = NULL;

    pVCU_04F02B70 =
        (VCU_04F02B70_t *)can_getPCanBuffer(0x04F02B70);

    if(can_getPCanRxState(0x04F02B70)
       == CAN_FRAME_ST_RECVED)
    {
        msg.data[0] =
            (uint8_t)pVCU_04F02B70->stir_mot_speed;

        msg.data[1] =
            (uint8_t)(pVCU_04F02B70->stir_mot_speed >> 8);

        msg.data[2] =
            (uint8_t)pVCU_04F02B70->suction_head_mot_speed;

        msg.data[3] =
            (uint8_t)(pVCU_04F02B70->suction_head_mot_speed >> 8);

        msg.data[4] =
            (uint8_t)pVCU_04F02B70->front_conveyor_mot_speed;

        msg.data[5] =
            (uint8_t)(pVCU_04F02B70->front_conveyor_mot_speed >> 8);
    }
    else
    {
        msg.data[0] = 0xFF;
        msg.data[1] = 0xFF;
        msg.data[2] = 0xFF;
        msg.data[3] = 0xFF;
        msg.data[4] = 0xFF;
        msg.data[5] = 0xFF;
    }

    msg.data[6] = 0xFF;
    msg.data[7] = 0xFF;

    msg.id = 0x18FEC218;
    msg.len = 0x08;
    msg.type = 0x00;

    CAN2_SendFIFOMessage(&msg);
}
void CAN_Send_MotSpeedObj2(void)
{
    CAN_MESSAGE msg;
    VCU_04F02C70_t *pVCU_04F02C70 = NULL;

    pVCU_04F02C70 =
        (VCU_04F02C70_t *)can_getPCanBuffer(0x04F02C70);

    if(can_getPCanRxState(0x04F02C70)
       == CAN_FRAME_ST_RECVED)
    {
        msg.data[0] =
            (uint8_t)pVCU_04F02C70->power_unit_mot_speed;

        msg.data[1] =
            (uint8_t)(pVCU_04F02C70->power_unit_mot_speed >> 8);

        msg.data[2] =
            (uint8_t)pVCU_04F02C70->sprinkle_tape_mot_speed;

        msg.data[3] =
            (uint8_t)(pVCU_04F02C70->sprinkle_tape_mot_speed >> 8);
    }
    else
    {
        msg.data[0] = 0xFF;
        msg.data[1] = 0xFF;
        msg.data[2] = 0xFF;
        msg.data[3] = 0xFF;
    }

    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFF;

    msg.id = 0x18FEC319;
    msg.len = 0x08;
    msg.type = 0x00;

    CAN2_SendFIFOMessage(&msg);
}
void CAN_Send_AIR1(void)    //0x18FEAE30/0x18F13017  1000ms 气压
{
    CAN_MESSAGE msg;
	static uint16_t ap_delay_cnt = 0;
	uint16_t FrontP = 0;
	uint16_t RearP = 0;
	uint16_t ZhuceP = 0;
	double   ap_sensor_scale = 0;
	
	if(M_ON)
	{
		if(ap_delay_cnt >= 3)
		{
			// FrontP = get_front_airpressure();
    		// RearP  = get_rear_airpressure();
   			// ZhuceP = get_parking_airpressure();
			
			//前桥气压
			ap_sensor_scale = get_input_analog_quantity(AD_V_B3) * 100 / get_input_analog_quantity(B24_5V_DET);
			if(ap_sensor_scale < 96 && ap_sensor_scale > 10)
				FrontP = ((double)ap_sensor_scale - 10 ) / 54.422 * 1000; //单位Kpa 
			else
				FrontP  = 0;
			
			//后桥气压
			ap_sensor_scale = get_input_analog_quantity(AD_V_B20) * 100 / get_input_analog_quantity(B24_5V_DET);
			if(ap_sensor_scale < 96 && ap_sensor_scale > 10)
				RearP = ((double)ap_sensor_scale - 10) / 54.422 * 1000; //单位Kpa
			else
				RearP  = 0;
			
			//驻车气压
			ap_sensor_scale = get_input_analog_quantity(AD_V_B19) * 100 / get_input_analog_quantity(B26_5V_DET);
			if(ap_sensor_scale < 96 && ap_sensor_scale > 10)
				ZhuceP = ((double)ap_sensor_scale - 10) / 54.422 * 1000; //单位Kpa
			else
				ZhuceP = 0;
			
			if(FrontP > 2000)
				FrontP = 2000;
			if(RearP > 2000)
				RearP = 2000;
			if(ZhuceP > 2000)
				ZhuceP = 2000;
		}
		else 
		{
			ap_delay_cnt++;
			
			// FrontP = 320;
			// RearP  = 320;
			// ZhuceP = 320;
		}
		
		// if(eol_icutype == 0)
		{
			msg.data[0] = (uint8_t)(FrontP/10);
			msg.data[1] = (uint8_t)(RearP/10);
			msg.data[2] = 0;
			msg.data[3] = 0;
			msg.data[4] = 0;
			msg.data[5] = 0;
			msg.data[6] = 0;
			msg.data[7] = 0;
			msg.len = 0x08;
			msg.type = 0x00;
			msg.id = 0x18F13017;
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
		}
		// else
		{
			msg.data[0] = 0;
			msg.data[1] = (uint8_t)(ZhuceP/8);
			msg.data[2] = (uint8_t)(FrontP/8);
			msg.data[3] = (uint8_t)(RearP/8);
			msg.data[4] = 0;
			msg.data[5] = 0;
			msg.data[6] = 0;
			msg.data[7] = 0;
			msg.len = 0x08;
			msg.type = 0x00;
			msg.id = 0x18FEAE30;
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
		}
	}
	else 
	{
		ap_delay_cnt = 0;
		
		// if(eol_icutype == 0)
		{
			msg.data[0] = (uint8_t)(0xffu);
			msg.data[1] = (uint8_t)(0xffu);
			msg.data[2] = 0;
			msg.data[3] = 0;
			msg.data[4] = 0;
			msg.data[5] = 0;
			msg.data[6] = 0;
			msg.data[7] = 0;
			msg.len = 0x08;
			msg.type = 0x00;
			msg.id = 0x18F13017;
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
		}
		// else
		{
			msg.data[0] = 0;
			msg.data[1] = (uint8_t)(0xffu);
			msg.data[2] = (uint8_t)(0xffu);
			msg.data[3] = (uint8_t)(0xffu);
			msg.data[4] = 0;
			msg.data[5] = 0;
			msg.data[6] = 0;
			msg.data[7] = 0;
			msg.len = 0x08;
			msg.type = 0x00;
			msg.id = 0x18FEAE30;
			CAN1_SendFIFOMessage(&msg);
			CAN2_SendFIFOMessage(&msg);
		}
	}
}
void CAN_Send_ICS(void)     //0x18FF7117  1000ms 仪表开关信号
{
    CAN_MESSAGE msg;
    BYTE_t tmpdata;
	
	tmpdata.byte = 0;
    tmpdata.bit12 = 0;
	tmpdata.bit34 = 0;
    tmpdata.bit56 = LED_LOCK;//驾驶室锁止不到位
	tmpdata.bit78 = LED_SaftyBelt; //安全带未系指示
    msg.data[0] = tmpdata.byte;
	
	tmpdata.byte = 0;
    tmpdata.bit12 = LED_CoolingLiquied;//冷却液液位过低报警
	tmpdata.bit34 = 0;
    tmpdata.bit56 = IN4;//干燥器指示
    tmpdata.bit78 = IN5;//后照灯指示
    msg.data[1] = tmpdata.byte;   
	
	tmpdata.byte = 0;
    tmpdata.bit12 = IN28;//轴间差速指示
    tmpdata.bit34 = IN26;//轮间差速指示
	tmpdata.bit56 = 0;
	tmpdata.bit78 = IN25; //挂车ABS指示
    msg.data[2] = tmpdata.byte; 
    
	tmpdata.byte = 0;
    tmpdata.bit12 = (IN2);//举升桥提升状态指示 
    tmpdata.bit34 = 0;//ECAS警告指示预留
    tmpdata.bit56 = 0;//ECAS失效指示预留
    tmpdata.bit78 = LED_Park;//驻车制动指示
    msg.data[3] = tmpdata.byte;
	
	tmpdata.byte = 0;
    tmpdata.bit12 = C4_OK_L_MCU ;	 //功能按键
    tmpdata.bit34 = C5_CANCEL_L_MCU; //功能按键
    tmpdata.bit56 = C7_DOWN_L_MCU;	 //功能按键
    tmpdata.bit78 = C6_UP_L_MCU;     //功能按键
    msg.data[4] = tmpdata.byte;  
    
    tmpdata.byte = 0;
    tmpdata.bit12 = IN3;//空气悬架指示
	tmpdata.bit34 = 0;
	if(eol_speed_enlarge_factor)
		tmpdata.bit56 = 1; /*0x01-已放大7%  0x00-未放大*/
	else
		tmpdata.bit56 = 0; /*0x01-已放大7%  0x00-未放大*/
	
	tmpdata.bit78 = 0;
    msg.data[5]=tmpdata.byte; 
	
	tmpdata.byte = 0;
    tmpdata.bit12 = IN11;//翼门开关
    msg.data[6] = tmpdata.byte; 
    msg.data[7] = 0;

    msg.id = 0x18FF7117;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN1_SendFIFOMessage(&msg);
	CAN2_SendFIFOMessage(&msg);
	
}
void PCAN_Send_AUXIO1(void) //0x18FED917  100ms
{
	static ptmrType_t abs_wait_Tmr = 0;
	CAN_MESSAGE msg;
	GeneralUse_t    *GU_18F00150_t   = NULL;
	GeneralUse_t    *GU_18FED932_t   = NULL;
	
	GU_18FED932_t   = (GeneralUse_t *)can_getBCanBuffer(0x18FED932);
	
	memset(msg.data,0,8);
	
	if(can_getPCanRxState(0x18F00150) == CAN_FRAME_ST_RECVED)
	{
		GU_18F00150_t   = (GeneralUse_t *)can_getPCanBuffer(0x18F00150);
		msg.data[0] |= (GU_18F00150_t->byte3.bit56&0x03);
	}
	else
	{
		msg.data[0] &= 0xFC;
	}
	
	//Byte2-bit34的逻辑
	if(M_ON)
	{
		if(!ptmr_IsStart(&abs_wait_Tmr))
			ptmr_Start(&abs_wait_Tmr);
		if(ptmr_Get(&abs_wait_Tmr) < 5000) 
		{
			msg.data[1] |= 0x04;//仪表上电后前 5s 由仪表默认填充 01-无故障
		}
		else
		{
			if(IN25) //挂车ABS指示
			{
				msg.data[1] &= 0xF3;//检测到低电平, 发送00 fault
			}
			else
			{
				msg.data[1] |= 0x04;//01 Normal
			}
		}
	}
	else
	{
		msg.data[1] |= 0x04;//01 Normal
		ptmr_Stop(&abs_wait_Tmr);
	}
	
	//检测挂车ABS - Byte2-bit56的逻辑
	if(TrailerABS_CheckFlg)
	{
		msg.data[1] |= 0x10;
	}
	else
	{
		msg.data[1] &= 0xCF;
	}
	
	//检测车辆是否带挂车 - Byte2-bit78的逻辑
	if(GU_18FED932_t->byte2.bit78 == 1 || TrailerABS_CheckFlg)
	{
		msg.data[1] |= 0x40;
	}
	else
	{
		msg.data[1] &= 0x3F;
	}
	
    msg.id = 0x18FED917;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN2_SendFIFOMessage(&msg);
	
}
void PCAN_Send_CollingLiquidLevel(void) //0x18FEEF17  1000ms 冷却液位信号
{
    CAN_MESSAGE msg;
    msg.data[0] = 0;                    
    msg.data[1] = 0;       
    msg.data[2] = 0;         
    msg.data[3] = 0;          
    msg.data[4] = 0;      
    msg.data[5] = 0;      
    msg.data[6] = 0;   
    if(LED_CoolingLiquied)
		msg.data[7] = 250;      
    else
		msg.data[7] = 0;      
    msg.id = 0x18FEEF17;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN2_SendFIFOMessage(&msg);
}


void Bcan_to_Pcan_N10ms(void)
{
	static uint8_t pSendCtrl_10ms = 0x00;
	uint8_t *tmp = NULL;
	
	/* 50ms */
	if(0x00 == (pSendCtrl_10ms % 5))
	{
		//关联标定项“AEBS 自动紧急制动系统”，为非 0 时，才转发
		if (eol_aebstype > 0)
		{
			if(0x01 ==  can_getBCanRxState(0x0C0B2A32)) //AEBS2
			{
				tmp = can_getBCanBuffer(0x0C0B2A32);
				PCAN_Send_Route(0x0C0B2A32,tmp);
			}
		}
	}
	
	if(0x09 == pSendCtrl_10ms)
	{
		pSendCtrl_10ms = 0x00;
	}
	else
	{
		pSendCtrl_10ms++;
	}
	
}
void Pcan_to_Bcan_N10ms(void)
{
	static uint8_t bSendCtrl_10ms = 0x00;
	uint8_t *tmp = NULL;
	
	/* 20ms */
	if(0x00 == (bSendCtrl_10ms % 2))
	{
		//BSD，转向角度信号
		if(0x01 ==  can_getPCanRxState(0x18F0090B))
		{
			tmp = can_getPCanBuffer(0x18F0090B);
			BCAN_Send_Route(0x18F0090B,tmp);
		}
	}
	
	if(0x09 == bSendCtrl_10ms)
	{
		bSendCtrl_10ms = 0x00;
	}
	else
	{
		bSendCtrl_10ms++;
	}
	
}
void Task10ms_can_send(void)
{
	if(eol_icutype == 0 || eol_icutype == 1 || eol_icutype == 3)
	{
		Bcan_to_Pcan_N10ms();
		
		Pcan_to_Bcan_N10ms();
	}
}


void Bcan_to_Pcan_N100ms(void)
{
	static uint8_t pSendCtrl_100ms = 0x00;
	uint8_t *tmp = NULL;

	/* 200ms */
	if(0x00 == (pSendCtrl_100ms % 2))
	{
		//关联标定项“AEBS 自动紧急制动系统”，为非 0 时，才转发
		if(eol_aebstype > 0)
		{
			// if(0x01 ==  can_getBCanRxState(0x0CFDCC32))  //OEL 
			// {
				// tmp = can_getBCanBuffer(0x0CFDCC32);
				// PCAN_Send_Route(0x0CFDCC32,tmp);
			// }
			
			if(0x01 ==  can_getBCanRxState(0x18A9E832)) //FLIC
			{
				tmp = can_getBCanBuffer(0x18A9E832);
				if(LDW_Warn == 1 || LDW_Warn == 2)
					tmp[7] |=  0x01;
				else
					tmp[7] &= 0xfc;
				PCAN_Send_Route(0x18A9E832,tmp);
			}
			
			if(0x01 ==  can_getBCanRxState(0x18FDCD32)) //OWW
			{
				tmp = can_getBCanBuffer(0x18FDCD32);
				PCAN_Send_Route(0x18FDCD32,tmp);
			}
		}
		
		if(0x01 ==  can_getBCanRxState(0x0CFDCC32))  //OEL  针对中联搅拌车
		{
			tmp = can_getBCanBuffer(0x0CFDCC32);
			PCAN_Send_Route(0x0CFDCC32,tmp);
		}
		
		if(0x01 ==  can_getBCanRxState(0x18FF2130)) /*TZ8C*/
		{
			tmp = can_getBCanBuffer(0x18FF2130);
			PCAN_Send_Route(0x18FF2130,tmp);
		}
	}
	/* 100ms */
	if(0x01 ==  can_getBCanRxState(0x0CF6022E)) /*TZ8C*/
	{
		tmp = can_getBCanBuffer(0x0CF6022E);
		PCAN_Send_Route(0x0CF6022E,tmp);
	}
	if(0x01 ==  can_getBCanRxState(0x18FF6DD8)) /*TZ8C*/
	{
		tmp = can_getBCanBuffer(0x18FF6DD8);
		PCAN_Send_Route(0x18FF6DD8,tmp);
	}
	
	if(eol_aebstype > 0)
	{
		if(0x01 ==	can_getBCanRxState(0x18FF2C32)) //BCMS
		{
			tmp = can_getBCanBuffer(0x18FF2C32);
			PCAN_Send_Route(0x18FF2C32,tmp);
		}
	}
	
	if(0x01 ==	can_getBCanRxState(0x18FF2732)) //BCMI
	{
		tmp = can_getBCanBuffer(0x18FF2732);
		PCAN_Send_Route(0x18FF2732,tmp);
	}
	
	if(0x01 ==	can_getBCanRxState(0x1CFEC303))
	{
		tmp = can_getBCanBuffer(0x1CFEC303);
		PCAN_Send_Route(0x1CFEC303,tmp);
	}
	
	if((eol_ebs == 5)||(eol_ebs == 6)||(eol_ebs ==10)|| (eol_ebs == 13))//标定为EBS系统时，显示EBS
	{
		if(0x01 ==	can_getBCanRxState(0x18FEEA2F))
		{
			tmp = can_getBCanBuffer(0x18FEEA2F);
			PCAN_Send_Route(0x18FEEA2F,tmp);
		}
		
		if(0x01 ==	can_getBCanRxState(0x0CFE5A2F))
		{
			tmp = can_getBCanBuffer(0x0CFE5A2F);
			PCAN_Send_Route(0x0CFE5A2F,tmp);
		}
		
		if(0x01 ==	can_getBCanRxState(0x18FE592F))
		{
			tmp = can_getBCanBuffer(0x18FE592F);
			PCAN_Send_Route(0x18FE592F,tmp);
		}
	}
	
	if(0x09 == pSendCtrl_100ms)
	{
		pSendCtrl_100ms = 0x00;
	}
	else
	{
		pSendCtrl_100ms++;
	}

}
void Pcan_to_Bcan_N100ms(void)
{
	static uint8_t  bSendCtrl_100ms = 0x00;
	uint8_t *tmp = NULL;

	/* 100ms */
	if(0x00 == (bSendCtrl_100ms % 1))
	{
		if(0x01 ==  can_getPCanRxState(0x18FE4F0B))
		{
			tmp = can_getPCanBuffer(0x18FE4F0B);
			BCAN_Send_Route(0x18FE4F0B,tmp);
		}

		if(0x01 ==  can_getPCanRxState(0x18F0010B))
		{
			tmp = can_getPCanBuffer(0x18F0010B);
			BCAN_Send_Route(0x18F0010B,tmp);
		}

		if(0x01 ==  can_getPCanRxState(0x18FEBF0B))
		{
			tmp = can_getPCanBuffer(0x18FEBF0B);
			BCAN_Send_Route(0x18FEBF0B,tmp);
		}
	}
	
	if(0x09 == bSendCtrl_100ms)
	{
		bSendCtrl_100ms = 0x00;
	}
	else
	{
		bSendCtrl_100ms++;
	}
	
}
void Task100ms_can_send(void) //100ms task
{
    static unsigned char sendcontrol = 0;
	static uint8_t mot_speed_send_cnt = 0;
    if(Test_Mode!=0) //测试模式不外发报文
		return;
	mot_speed_send_cnt++;

if(mot_speed_send_cnt >= 2)
{
    mot_speed_send_cnt = 0;

    CAN_Send_MotSpeedObj1();
    CAN_Send_MotSpeedObj2();
}
	for(uint8_t num = 0; num < 4; num++)
	{
		for(uint8_t i = 0;i < 4; i++)
		{
			if(wheelupdatedtimer[num][i] < 30)
				wheelupdatedtimer[num][i]++;
			else
			{
				wheelupdatedtimer[num][i] = 30;
				
				Wheels[num][i].wheelID = 0;
				Wheels[num][i].temperature = 0;
				Wheels[num][i].pressure = 0;
				Wheels[num][i].fast_pressure_lost = 0;
				Wheels[num][i].pressure_status = 0;
				Wheels[num][i].over_temperature_status = 0;
				
				if(i==0)
				{
					TyreLocation_pressureh[num].bit0 = 0;
					TyreLocation_pressurel[num].bit0 = 0;
					TyreLocation_fastlostgas[num].bit0 = 0;
					TyreLocation_temperature[num].bit0 = 0;
				}
				if(i==1)
				{
					TyreLocation_pressureh[num].bit1 = 0;
					TyreLocation_pressurel[num].bit1 = 0;
					TyreLocation_fastlostgas[num].bit1 = 0;
					TyreLocation_temperature[num].bit1 = 0;
				}
				if(i==2)
				{
					TyreLocation_pressureh[num].bit2 = 0;
					TyreLocation_pressurel[num].bit2 = 0;
					TyreLocation_fastlostgas[num].bit2 = 0;
					TyreLocation_temperature[num].bit2 = 0;
				}
				if(i==3)
				{
					TyreLocation_pressureh[num].bit3 = 0;
					TyreLocation_pressurel[num].bit3 = 0;
					TyreLocation_fastlostgas[num].bit3 = 0;
					TyreLocation_temperature[num].bit3 = 0;
				}
			}
		}
	}
	
	if(learnstatus==1)
	{
		if(study==1)
		{
			if(StartLearnflag == 1)
			{
				StartLearnflag = 0;
				BCAN_StartTime(0x18FFE833);
				BCAN_Send_TPMSLearning();
			}
			
			if((can_getBCanRxState(0x18FFE833)==2)&&back)
			{
				study = 0;
				BCAN_Send_TPMSLearningEnd();
				backvalue = 16;
			}
			else
			{
				if(backvalue==2||backvalue==3)
				{
					BCAN_Send_TPMSLearning();
				}
			}
		}
	}
	
	if(eol_icutype == 0 || eol_icutype == 1 || eol_icutype == 3)
	{
		Bcan_to_Pcan_N100ms();
		Pcan_to_Bcan_N100ms();
	}
	
	if((can_getPCanRxState(0x0CEF3103) == CAN_FRAME_ST_RECVED)
	|| (can_getPCanRxState(0x18F00503) == CAN_FRAME_ST_RECVED)
	|| (can_getPCanRxState(0x18FFF531) == CAN_FRAME_ST_RECVED))
		BCAN_Send_GearR();
	BCAN_Send_CCSV();
	PCAN_Send_AUXIO1();
	
	if(Test_Mode == 0)
	{
		if(M_ON && delay_cnt<1)
		{
			PCAN_Send_ICU_DM1();
		}
	}
	
    switch (sendcontrol)
	{
		case 0:
		    CAN_Send_AMB();     //0x18FEF517  1000ms 环境温度
		    sendcontrol++;
		    break;
		case 1:
	    	CAN_Send_ICS();     //0x18FF7117  1000ms 仪表开关信号
	    	sendcontrol++;
	    	break;
		case 2:
	    	CAN_Send_AIR1();    //0x18FEAE30  1000ms 气压
	    	sendcontrol++;
	    	break;
		case 3:
			BCAN_Send_Tipian(); //0x18FF2C47  10000ms 蹄片信号
		    sendcontrol++;
		    break;
		case 4:
		    CAN_Send_VDHR();    //0x18FEC1EE  1000ms 里程
		    sendcontrol++;
		    break;
		case 5:
		    CAN_Send_TD();      //0x18FEE6EE  1000ms 时间
		    sendcontrol++;
		    break;
		case 6:
		    PCAN_Send_CollingLiquidLevel(); //0x18FEEF17  1000ms 冷却液位信号
		    sendcontrol++;
		    break;
		case 7:
	    	sendcontrol++;
	    	break;
		case 8:
	    	sendcontrol++;
	    	break;
		case 9:
	    	sendcontrol = 0;
	    	break;
		default:
			sendcontrol = 0;
	    	break;
    }
}


void PCAN_Send_TestRequest(uint8_t req)
{
    CAN_MESSAGE msg;
    msg.data[0] = req;              
    msg.data[1] = 0;       
    msg.data[2] = 0;       
    msg.data[3] = 0;       
    msg.data[4] = 0;
    msg.data[5] = 0;
    msg.data[6] = 0;
    msg.data[7] = 0;
    msg.id = 0x18FFA1A0;
    msg.len = 0x08;
    msg.type = 0x00;
	CAN2_SendFIFOMessage(&msg);
	
}
void PCAN_Send_TestResult(void) //Miles
{
	CAN_MESSAGE msg;
	/*
	msg.data[0]: IN1-IN8
	msg.data[1]: IN9-IN16
	msg.data[2]: IN17-IN24
	msg.data[3]: IN25-IN40
	*/
	msg.data[0] = testResultData[0]; //switch test status
	msg.data[1] = testResultData[1]; //switch test status
	msg.data[2] = testResultData[2]; //switch test status
	msg.data[3] = testResultData[3]; //switch test status
	msg.data[4] = testResultData[4]; //step
	
	msg.data[5] = testResultData[5]; //ad test result 1-8
	msg.data[6] = testResultData[6]; //9-10
	msg.data[7] = !(bcancnt>5); //
	
	if((msg.data[0] == 0) \
	&& (msg.data[1] == 0) \
	&& (msg.data[2] == 0) \
	&& (msg.data[3] == 0) \
	&& (msg.data[4] == 3) \
	&& (msg.data[5] == 0) \
	&& ((msg.data[6]&0x07) == 0) \
	&& (msg.data[7] == 0))
		TestResult = 0x55;
	else
		TestResult = 0xAA;//failed
	
	if(testStep == 3)
		testStep = 4;
	
	msg.id = 0x18FEFF20; //
	msg.len = 0x08;
	msg.type = 0x00;
	
	CAN2_SendFIFOMessage(&msg);
	
}


uint8_t  can_busoff_flg[CAN_CHN] = {0, 0, 0};
void canBusOffDetectionTask(void)
{
	static uint32_t can_busoff_recover_cnt[CAN_CHN] = {0, 0, 0}; //bus off 恢复次数
	static uint32_t can_busoff_time_cnt[CAN_CHN] = {0, 0, 0};    //
	
	uint8_t recover_time[CAN_CHN];
	
	for(uint8_t i = 1; i < CAN_CHN; i++)
	{
		if(can_busoff_flg[i] == 1)
		{
			if(can_busoff_recover_cnt[i] <= 4)
				recover_time[i] = 4;
			else
				recover_time[i] = 19;
			
			if(can_busoff_time_cnt[i] > recover_time[i])
			{
				can_busoff_flg[i] = 0;
				
				can_busoff_recover_cnt[i]++;
				
				RS_CAN_BusOff_Recovery(i);
				
			}
			else can_busoff_time_cnt[i]++;
		}
		else can_busoff_time_cnt[i] = 0;
	}
	
}






