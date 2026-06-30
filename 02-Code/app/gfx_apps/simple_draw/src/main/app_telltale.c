/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_telltale.c
 * @brief   telltale display
 * @version V1.0.0
 * @author  liubing
 * @date    2019-08-19
 * @warning none
 * @note <b>history:</b>
 * 1. create file
 *
 */

/*******************************************************************************
 * INCLUDES
 */
#include "SYSTEM.h"
#include "zk.h"
#include "init.h"
#include "app_telltale.h"
#include "app_can.h"
#include "uds_support.h"
#include "app_tpmsmain.h"
#include "app_data_center.h"
#include "ptmr.h"
#include "app_dm1.h"
#include "interval.h"

/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */

#define Y_LOCATION  13
#define BIG_X       400
#define BIG_Y       278
#define EPS_X_LOCATION  286
#define EPS_Y_LOCATION  105
/*******************************************************************************
 * TYPEDEFS
 */
typedef struct {
	char    *posNum;     /*显示图标的取模位置*/
	uint16_t posX;       /*显示X坐标*/
	uint16_t posY;       /*显示Y坐标*/
	uint32_t color;      /*显示颜色*/
	uint32_t showtime;   /*显示时间*/
	uint8_t *text[2][2]; /*显示文字*/
}BigIcon_t;

typedef struct {
    const BigIcon_t *IconInfo;
	
	uint8_t  *ShowReq;  /*显示请求*/
    uint8_t  *Showing;  /*正在显示*/
    uint8_t  *ShowEnd;  /*显示结束*/
	uint32_t *timer;    /*显示计时器*/
	
}PopupWin_t;


/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * Gloable VARIABLES DECLEAR
 */

uint8_t TPMS_Warn = 0;
uint8_t LDW_Warn = 0;
uint8_t FCW_Level = 0;
uint8_t AEBS_Warn = 0;
uint8_t BSD_Warn = 0;
uint8_t ADU_Warn = 0;


/*******************************************************************************
 * Local VARIABLES DECLEAR
 */

static uint8_t Zijian = 0x00;
static uint8_t LdwsWarning = 0x00;
static uint8_t FCWWarning = 0x00;
static uint8_t BSDWarning = 0x00;
static uint8_t EBSWarning = 0x00;

static uint8_t hazardRequestDisp = 0;
static uint8_t ebsRedRequestDisp = 0;
static uint8_t ebsYellowRequestDisp = 0;
static uint8_t fcwCloseRequestDisp = 0;
static uint8_t offRoadCloseRequestDisp = 0;
static uint8_t bsdCloseRequestDisp = 0;
static uint8_t bsdRequestDisp = 0;
static uint8_t tpmsRequestDisp = 0;
static uint8_t restRequestDisp = 0;
static uint8_t aebsRequestDisp  = 0;
static uint8_t absRequestDisp = 0;
static uint8_t Trailer_absRequestDisp = 0;
static uint8_t LmtSpdRequestDisp = 0;
static uint8_t aduRequestDisp = 0;
static uint8_t ptcRequestDisp = 0;
static uint8_t acRequestDisp = 0;
static uint8_t ptcSwitchOld = 0;
static uint8_t acSwitchOld = 0;
static uint8_t instrument1RequestDisp = 0;
static uint8_t instrument2RequestDisp = 0;
static uint8_t instrument3RequestDisp = 0;

static uint8_t instrument1Old = 0;
static uint8_t instrument2Old = 0;
static uint8_t instrument3Old = 0;


#define BIG_ICON_NUM  (sizeof(BigIconInfo) / sizeof(BigIcon_t))
static const BigIcon_t BigIconInfo[] = 
{
	/*取模位置    X坐标         Y坐标          显示颜色     显示时间   显示文字 */
/* 0*/{   "F",    BIG_X - 90,  BIG_Y,       0xffffff00,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*黄色-紧急故障 HAZARD*/
/* 1*/{   "W",    BIG_X - 54,  BIG_Y,       0xffffff00,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*黄色-主车 ABS 警告*/
/* 2*/{   "V",    BIG_X - 40,  BIG_Y,       0xffffff00,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*黄色-挂车 ABS 故障*/
/* 3*/{   "G",    BIG_X - 54,  BIG_Y,       0xffff0000,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*红色-EBS:故障或失效*/
/* 4*/{   "G",    BIG_X - 54,  BIG_Y,       0xffffff00,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*黄色-EBS:警告*/
/* 5*/{   "X",    BIG_X - 38,  BIG_Y,       0xffffff00,   1500,  {{(uint8_t *)"车速限制指示",  NULL}, {NULL,  NULL}}}, /*黄色-LIM:限速报警*/
/* 6*/{   "Y",    BIG_X - 36,  BIG_Y,       0xffffff00,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*黄色-智能补电禁用开关提醒 */
/* 7*/{   "N",    BIG_X - 45,  BIG_Y,       0xffffff00,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*黄色-BSD一二级报警*/
/* 8*/{   "P",    BIG_X - 38,  BIG_Y,       0xffffff00,   1500,  {{NULL,  NULL}, {NULL,  NULL}}}, /*黄色-DFMS:疲劳预警*/
/* 9*/{   "K",    BIG_X - 79,  BIG_Y,       0xffffff00,   1500,  {{(uint8_t *)"前方碰撞预警系统已关闭",  NULL}, {(uint8_t *)"FCWS OFF",  NULL}}}, /*黄色-FCWS:碰撞预警系统关闭*/
/*10*/{   "K",    BIG_X - 79,  BIG_Y,       0xffffff00,   1500,  {{(uint8_t *)"自动紧急制动系统已关闭",  NULL}, {(uint8_t *)"AEBS OFF",  NULL}}}, /*黄色-AEBS:自动紧急制动系统关闭*/
/*11*/{   "I",    BIG_X - 40,  BIG_Y,       0xffffff00,   1500,  {{(uint8_t *)"车道偏离预警系统已关闭",  NULL}, {(uint8_t *)"LDWS OFF",  NULL}}}, /*黄色-LDWS:车道偏离系统关闭*/
/*12*/{   "M",    BIG_X - 45,  BIG_Y,       0xffffff00,   1500,  {{(uint8_t *)"盲区检测系统已关闭",  NULL}, {(uint8_t *)"BDS OFF",  NULL}}}, /*黄色-BDS:BSD系统关闭*/
/*13*/{   "H",    BIG_X - 40,  BIG_Y,       0xffffff00,   1500,  {{(uint8_t *)"TPMS 报警",  NULL}, {(uint8_t *)"TPMS Alarm",  NULL}}}, /*黄色-TPMS:胎压系统报警*/
/*14*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请挂D挡",       NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please shift to D gear"}}}, /*红色-智驾 A 类报警:激活失败，请挂D挡*/
/*15*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请锁车门",      NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please lock the door"}}}, /*红色-智驾 A 类报警:激活失败，请锁车门*/
/*16*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请系好安全带",   NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please fasten your seat belt"}}}, /*红色-智驾 A 类报警:激活失败，请系好安全带*/
/*17*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请回到驾驶座位", NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please return to the driver's seat"}}}, /*红色-智驾 A 类报警:激活失败，请回到驾驶座位*/
/*18*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请松开刹车",    NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please release the brake"}}}, /*红色-智驾 A 类报警:激活失败，请松开刹车*/
/*19*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请松开手刹",    NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please release the handbrake"}}}, /*红色-智驾 A 类报警:激活失败，请松开手刹*/
/*20*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请注视前方",    NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please look ahead"}}}, /*红色-智驾 A 类报警:激活失败，请注视前方*/
/*21*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，请手握方向盘",   NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please hold the steering wheel"}}}, /*红色-智驾 A 类报警:激活失败，请手握方向盘*/
/*22*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，车速过高请减速", NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Please slow down，speed is too high"}}}, /*红色-智驾 A 类报警:激活失败，车速过高请减速*/
/*23*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，未进入可用区域", NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Not entering the available area"}}}, /*红色-智驾 A 类报警:激活失败，未进入可用区域*/
/*24*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，目的地距离过远", NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Destination is too far away"}}}, /*红色-智驾 A 类报警:激活失败，目的地距离过远*/
/*25*/{   "S",    BIG_X - 40,  BIG_Y - 31,  0xffff0000,   5000,  {{(uint8_t *)"激活失败，路径规划失败",   NULL}, {(uint8_t *)"Activation failed", (uint8_t *)"Path planning failed"}}}, /*红色-智驾 A 类报警:激活失败，路径规划失败*/
/*26*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"外灯故障，建议接管车辆",   NULL}, {(uint8_t *)"External light malfunction", (uint8_t *)"Advised to take over the vehicle"}}}, /*黄色-智驾 A 类报警:外灯故障，建议接管车辆*/
/*27*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"雨刮故障，建议接管车辆",   NULL}, {(uint8_t *)"Wiper malfunction", (uint8_t *)"Advised to take over the vehicle"}}}, /*黄色-智驾 A 类报警:雨刮故障，建议接管车辆*/
/*28*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"续航里程低，建议接管车辆",   NULL}, {(uint8_t *)"Low driving range", (uint8_t *)"Advised to take over the vehicle"}}}, /*黄色-智驾 A 类报警:续航里程低，建议接管车辆*/
/*29*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"即将超出自动驾驶可用范围", (uint8_t *)"建议接管车辆"}, {(uint8_t *)"bout to exceed the available range", (uint8_t *)"Advised to take over the vehicle"}}}, /*黄色-智驾 A 类报警:即将超出自动驾驶可用范围，建议接管车辆*/
/*30*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"请注意安全驾驶",   NULL}, {(uint8_t *)"Please drive safely", NULL}}}, /*黄色-智驾 A 类报警:请注意安全驾驶*/
/*31*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"检测到吸烟，请注意安全",   NULL}, {(uint8_t *)"Smoking detected", (uint8_t *)"Please be safe"}}}, /*黄色-智驾 A 类报警:检测到吸烟，请注意安全*/
/*32*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"检测到使用手机，请注意安全",   NULL}, {(uint8_t *)"Cell phone use detected", (uint8_t *)"Please be safe"}}}, /*黄色-智驾 A 类报警:检测到使用手机，请注意安全*/
/*33*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"检测到视线偏移，请注意安全",   NULL}, {(uint8_t *)"Eye drift detected", (uint8_t *)"Please be safe"}}}, /*黄色-智驾 A 类报警:检测到视线偏移，请注意安全*/
/*34*/{   "T",    BIG_X - 40,  BIG_Y - 31,  0xffffff00,   5000,  {{(uint8_t *)"驾驶员疲劳",   (uint8_t *)"建议前往服务区休息"}, {(uint8_t *)"Driver fatigue, advised to", (uint8_t *)"Go to the service area to rest"}}}, /*黄色-智驾 A 类报警:驾驶员疲劳，建议前往服务区休息*/
/*35*/{   NULL,    BIG_X - 79,  BIG_Y,       0xffffff00,   3000,  {{(uint8_t *)"驾驶室取暖已开启", NULL}, {(uint8_t *)"Cab heater ON", NULL}}}, /*黄色-驾驶室取暖开启*/
/*36*/{   NULL,    BIG_X - 79,  BIG_Y,       0xffffff00,   3000,  {{(uint8_t *)"驾驶室空调已开启", NULL}, {(uint8_t *)"Cab A/C ON", NULL}}}, /*黄色-驾驶室空调开启*/
/*37*/{	  NULL,    BIG_X, 		BIG_Y, 		 0xffff0000,   5000,  {{(uint8_t *)"请尽快离开车辆！", NULL}, {(uint8_t *)"Please leave the vehicle!", NULL}}},
/*38*/{   NULL,    BIG_X,       BIG_Y,       0xffff0000,   5000,  {{(uint8_t *)"驻车故障，请安全停车检查！", NULL}, {(uint8_t *)"Parking fault, stop safely!", NULL}}},
/*39*/{ NULL,      BIG_X,       BIG_Y,       0xffff0000,   5000,  {{(uint8_t *)"高压未断开，请驻车下电！", NULL},   {(uint8_t *)"High voltage remains on!", NULL}}},
	
};

static uint8_t  BigIconShowReq[BIG_ICON_NUM];  /*显示请求*/
static uint8_t  BigIconShowing[BIG_ICON_NUM];  /*正在显示*/
static uint8_t  BigIconShowEnd[BIG_ICON_NUM];  /*显示结束*/
static uint32_t BigIconTimer[BIG_ICON_NUM];    /*显示计时器*/
static PopupWin_t PopupWinBigIcon =
{
	BigIconInfo,
	BigIconShowReq,
	BigIconShowing,
	BigIconShowEnd,
	BigIconTimer
};


void Clear_TruckArea(void)
{
	Coord_t crd1[4] = {
		{400 - 88,  235},
		{400 + 88,  235},
		{400 + 235, 235 + 158},
		{400 - 235, 235 + 158},
	};
	
	hide_ldws_left_lane();
	hide_ldws_right_lane();
	
	loc_RenderQuads(crd1, VM_COLOR_CLEAR); //0x80333333  //VM_COLOR_CLEAR
	// LCD_DrawRect(185, 280, 430, 112, TOUMING); //TOUMING YELLOW
	LCD_DrawRect(185, 315, 430, 78, TOUMING);
}

void Display_TruckArea(void)
{
	uint8_t uniStr[128];
	uint16_t width;
	static ptmrType_t adas_250ms_Tmr = 0;
	static ptmrType_t adcu_blink_Tmr = 0;
	static uint8_t retLDW = 0;
	static uint8_t retFCW = 0;
	static uint8_t retADCU = 0;
	
	start_draw();
	if(0xFF == Zijian)
	{
		if((PTC_SwitchStatus == 1) && (ptcSwitchOld == 0))
		{
			ptcRequestDisp = 1;
		}

		if((AC_SwitchStatus == 1) && (acSwitchOld == 0))
		{
			acRequestDisp = 1;
		}

		ptcSwitchOld = PTC_SwitchStatus;
		acSwitchOld = AC_SwitchStatus;
		VCU_04F02270_t *pVCU_04F02270 = NULL;

pVCU_04F02270 =
    (VCU_04F02270_t*)can_getBCanBuffer(0x04F02270);

if(can_getBCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)
{
    if((pVCU_04F02270->instrument_display_1 == 1)
    && (instrument1Old == 0))
    {
        instrument1RequestDisp = 1;
    }

    if((pVCU_04F02270->instrument_display_2 == 1)
    && (instrument2Old == 0))
    {
        instrument2RequestDisp = 1;
    }

    if((pVCU_04F02270->instrument_display_3 == 1)
    && (instrument3Old == 0))
    {
        instrument3RequestDisp = 1;
    }

    instrument1Old = pVCU_04F02270->instrument_display_1;
    instrument2Old = pVCU_04F02270->instrument_display_2;
    instrument3Old = pVCU_04F02270->instrument_display_3;
}
else
{
    instrument1Old = 0;
    instrument2Old = 0;
    instrument3Old = 0;
}
		
		BigIconShowReq[ 0] = hazardRequestDisp;  /*黄色-紧急故障 HAZARD*/
		BigIconShowReq[ 1] = absRequestDisp;  /*黄色-主车 ABS 警告*/
		BigIconShowReq[ 2] = Trailer_absRequestDisp;  /*黄色-挂车 ABS 故障*/
		BigIconShowReq[ 3] = ebsRedRequestDisp;  /*红色-EBS:故障或失效*/
		BigIconShowReq[ 4] = ebsYellowRequestDisp;  /*黄色-EBS:警告*/
		BigIconShowReq[ 5] = LmtSpdRequestDisp;  /*黄色-LIM:限速报警*/
		BigIconShowReq[ 6] = MSD_SwitchStatus;  /*黄色-智能补电禁用开关提醒 */
		BigIconShowReq[ 7] = bsdRequestDisp;  /*黄色-BSD一二级报警*/
		BigIconShowReq[ 8] = restRequestDisp;  /*黄色-DFMS:疲劳预警*/
		BigIconShowReq[ 9] = fcwCloseRequestDisp;  /*黄色-FCWS:碰撞预警系统关闭*/
		BigIconShowReq[10] = aebsRequestDisp;  /*黄色-AEBS:自动紧急制动系统关闭*/
		BigIconShowReq[11] = offRoadCloseRequestDisp;  /*黄色-LDWS:车道偏离系统关闭*/
		BigIconShowReq[12] = bsdCloseRequestDisp;  /*黄色-BDS:BSD系统关闭*/
		BigIconShowReq[13] = tpmsRequestDisp;  /*黄色-TPMS:胎压系统报警*/
		
		BigIconShowReq[14] = (aduRequestDisp == 116); /*红色-智驾 A 类报警:激活失败，请挂D挡*/
		BigIconShowReq[15] = (aduRequestDisp == 117); /*红色-智驾 A 类报警:激活失败，请锁车门*/
		BigIconShowReq[16] = (aduRequestDisp == 118); /*红色-智驾 A 类报警:激活失败，请系好安全带*/
		BigIconShowReq[17] = (aduRequestDisp == 119); /*红色-智驾 A 类报警:激活失败，请回到驾驶座位*/
		BigIconShowReq[18] = (aduRequestDisp == 120); /*红色-智驾 A 类报警:激活失败，请松开刹车*/
		BigIconShowReq[19] = (aduRequestDisp == 121); /*红色-智驾 A 类报警:激活失败，请松开手刹*/
		BigIconShowReq[20] = (aduRequestDisp == 122); /*红色-智驾 A 类报警:激活失败，请注视前方*/
		BigIconShowReq[21] = (aduRequestDisp == 123); /*红色-智驾 A 类报警:激活失败，请手握方向盘*/
		BigIconShowReq[22] = (aduRequestDisp == 124); /*红色-智驾 A 类报警:激活失败，车速过高请减速*/
		BigIconShowReq[23] = (aduRequestDisp == 125); /*红色-智驾 A 类报警:激活失败，未进入可用区域*/
		BigIconShowReq[24] = (aduRequestDisp == 126); /*红色-智驾 A 类报警:激活失败，目的地距离过远*/
		BigIconShowReq[25] = (aduRequestDisp == 127); /*红色-智驾 A 类报警:激活失败，路径规划失败*/
		BigIconShowReq[26] = (aduRequestDisp == 139); /*黄色-智驾 A 类报警:外灯故障，建议接管车辆*/
		BigIconShowReq[27] = (aduRequestDisp == 140); /*黄色-智驾 A 类报警:雨刮故障，建议接管车辆*/
		BigIconShowReq[28] = (aduRequestDisp == 141); /*黄色-智驾 A 类报警:续航里程低，建议接管车辆*/
		BigIconShowReq[29] = (aduRequestDisp == 142); /*黄色-智驾 A 类报警:即将超出自动驾驶可用范围，建议接管车辆*/
		BigIconShowReq[30] = (aduRequestDisp == 144); /*黄色-智驾 A 类报警:请注意安全驾驶*/
		BigIconShowReq[31] = (aduRequestDisp == 145); /*黄色-智驾 A 类报警:检测到吸烟，请注意安全*/
		BigIconShowReq[32] = (aduRequestDisp == 146); /*黄色-智驾 A 类报警:检测到使用手机，请注意安全*/
		BigIconShowReq[33] = (aduRequestDisp == 147); /*黄色-智驾 A 类报警:检测到视线偏移，请注意安全*/
		BigIconShowReq[34] = (aduRequestDisp == 148); /*黄色-智驾 A 类报警:驾驶员疲劳，建议前往服务区休息*/
		BigIconShowReq[35] = ptcRequestDisp;
		BigIconShowReq[36] = acRequestDisp;
		BigIconShowReq[37] = 0;
		BigIconShowReq[38] = 0;
		BigIconShowReq[39] = 0;
		for(uint8_t i = 0; i < BIG_ICON_NUM; i++)
		{
			if(PopupWinBigIcon.ShowReq[i] == 1)
			{
				if(PopupWinBigIcon.ShowEnd[i] == 0) //控制循环
				{
					if(BigIcon_Showing == 0) //控制单次显示,正在显示的图形有没有的标志位
					{
						BigIcon_Showing = 1;
						
						Clear_TruckArea();
						
						if(PopupWinBigIcon.IconInfo[i].posNum != NULL)
						{
    						loc_Render_BigIcon(PopupWinBigIcon.IconInfo[i].posX, \
							PopupWinBigIcon.IconInfo[i].posY, \
							PopupWinBigIcon.IconInfo[i].color, \
							PopupWinBigIcon.IconInfo[i].posNum);
						}
						
						if(PopupWinBigIcon.IconInfo[i].text[eol_language][0] != NULL)
						{
							zk_init(loc_Drw2dDev, &GUI_Fontwryh31, PopupWinBigIcon.IconInfo[i].color, 0);
							zk_oem2uni(PopupWinBigIcon.IconInfo[i].text[eol_language][0], uniStr, sizeof(uniStr));
							width = zk_getTextWidth(PopupWinBigIcon.IconInfo[i].text[eol_language][0]);
							zk_printZH(400-width/2, PopupWinBigIcon.IconInfo[i].posY + 81, uniStr);
						}
						
						if(PopupWinBigIcon.IconInfo[i].text[eol_language][1] != NULL)
						{
							zk_init(loc_Drw2dDev, &GUI_Fontwryh31, PopupWinBigIcon.IconInfo[i].color, 0);
							zk_oem2uni(PopupWinBigIcon.IconInfo[i].text[eol_language][1], uniStr, sizeof(uniStr));
							width = zk_getTextWidth(PopupWinBigIcon.IconInfo[i].text[eol_language][1]);
							zk_printZH(400-width/2, PopupWinBigIcon.IconInfo[i].posY + 81 + 31, uniStr);
						}
						
						PopupWinBigIcon.Showing[i] = 1;
						
						ptmr_Start(&PopupWinBigIcon.timer[i]);
					}
					else
					{
						if(PopupWinBigIcon.Showing[i] == 1)
						{
							if(ptmr_Get(&PopupWinBigIcon.timer[i]) >= PopupWinBigIcon.IconInfo[i].showtime)
							{
								BigIcon_Showing = 0;
								
								PopupWinBigIcon.ShowEnd[i] = 1;
								PopupWinBigIcon.Showing[i] = 0;
								// PopupWinBigIcon.ShowReq[i] = 0;
								if(i == 35)
								{
   								 ptcRequestDisp = 0;
								}
								else if(i == 36)
								{
   								 acRequestDisp = 0;
								}
								else if(i == 37)
								{
    								instrument1RequestDisp = 0;
								}
								else if(i == 38)
								{
    								instrument2RequestDisp = 0;
								}
								else if(i == 39)
								{
    								instrument3RequestDisp = 0;
								}
								Clear_TruckArea();
								
								ptmr_Stop(&PopupWinBigIcon.timer[i]);
							}
						}
					}
				}
			}
			else
			{
				PopupWinBigIcon.ShowEnd[i] = 0;
				if(PopupWinBigIcon.Showing[i] == 1)
				{
					BigIcon_Showing = 0;
					Clear_TruckArea();
					PopupWinBigIcon.Showing[i] = 0;
					ptmr_Stop(&PopupWinBigIcon.timer[i]);
				}
			}
		}
	}
	
	if((BigIcon_Showing == 0) && (0xFF == Zijian))
	{
		if((aduRequestDisp >= 111 && aduRequestDisp <= 115)
		|| (aduRequestDisp >= 128 && aduRequestDisp <= 138)
		|| (aduRequestDisp == 143)
		|| (aduRequestDisp == 149)
		|| (aduRequestDisp == 150) )
		{/*智驾 B 类报警*/
			DisplayTruckFlg = 1;
			tpmsalarm = 0;
			update_tpmsvalue = 1;
			Clear_TruckArea();
			
			if(aduRequestDisp == 111)
			{
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"自动驾驶系统自检中", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"自动驾驶系统自检中");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"Autopilot system self-checking", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Autopilot system self-checking");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
				}
			}
			else if(aduRequestDisp == 112)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "S");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"自动驾驶系统不可用，系统故障", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"自动驾驶系统不可用，系统故障");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Autopilot system is unavailable. ", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Autopilot system is unavailable. ");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"System failure", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"System failure");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
					
				}
			}
			else if(aduRequestDisp == 113)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "S");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"自动驾驶系统不可用，硬件故障", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"自动驾驶系统不可用，硬件故障");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Autopilot system is unavailable.", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Autopilot system is unavailable. ");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"Hardware failure", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Hardware failure");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
					
				}
			}
			else if(aduRequestDisp == 114)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, GREEN, "R");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"自动驾驶系统已就绪，可激活", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"自动驾驶系统已就绪，可激活");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"Autopilot is ready for activation", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Autopilot is ready for activation");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
				}
			}
			else if(aduRequestDisp == 115)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, GREEN, "Q");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"自动驾驶系统激活中", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"自动驾驶系统激活中");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"Autopilot system is activating", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Autopilot system is activating");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
				}
			}
			else if(aduRequestDisp == 128)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, GREEN, "R");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"自动驾驶系统已激活", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"自动驾驶系统已激活");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
					zk_oem2uni((uint8_t *)"请松开方向盘和脚踏", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"请松开方向盘和脚踏");
					zk_printZH(400-width/2, BIG_Y + 81, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"Autopilot is active.", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Autopilot is active.");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"Release the steering wheel and pedal", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Release the steering wheel and pedal");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 129)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"硬件故障，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"硬件故障，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Hardware failure. ", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Hardware failure. ");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 130)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"系统故障，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"系统故障，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"System failure. ", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"System failure. ");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 131)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"续航里程过低，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"续航里程过低，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Driving range is too low.", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Driving range is too low.");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 132)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"超出自动驾驶范围，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"超出自动驾驶范围，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Out of autopilot range", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Out of autopilot range");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 133)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"天气状况不良，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"天气状况不良，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Bad weather", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Bad weather");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 134)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"驾驶员离座，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"驾驶员离座，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Driver out of seat", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Driver out of seat");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 135)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"驾驶员疲劳，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"驾驶员疲劳，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Driver fatigue", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Driver fatigue");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 136)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"胎压异常，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"胎压异常，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Abnormal tire pressure", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Abnormal tire pressure");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 137)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"外部环境不良，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"外部环境不良，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Bad external environment", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Bad external environment");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 138)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, RED, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"门未锁，请立即接管！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"门未锁，请立即接管！");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Door unlocked", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Door unlocked");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"please take over immediately!", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"please take over immediately!");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
			}
			else if(aduRequestDisp == 143)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, WHITE, "U");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"自动驾驶系统关闭", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"自动驾驶系统关闭");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"请注意安全驾驶", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"请注意安全驾驶");
					zk_printZH(400-width/2, BIG_Y + 81, uniStr);
					
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"Autopilot is off", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Autopilot is off");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"Please drive safely", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Please drive safely");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
				}
				
			}
			else if(aduRequestDisp == 149)
			{
				loc_Render_BigFHP(BIG_X - 40, BIG_Y - 31, YELLOW, "T");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"请驾驶员立即回座", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"请驾驶员立即回座");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
					
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"Please return to your seat", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Please return to your seat");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
					zk_oem2uni((uint8_t *)"immediately！", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"immediately！");
					zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
					
					
				}
				
			}
			else if(aduRequestDisp == 150)
			{
				loc_Render_BigFHP(BIG_X - 79, BIG_Y - 31, YELLOW, "L");
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"紧急制动", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"紧急制动");
					zk_printZH(400-width/2, BIG_Y + 81 - 31, uniStr);
					
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"Emergency braking", uniStr, sizeof(uniStr));
					width = zk_getTextWidth((uint8_t *)"Emergency braking");
					zk_printZH(BIG_X-width/2, BIG_Y+81 - 31, uniStr);
					
				}
			}
		}
		else if(FCW_Level == 2 || AEBS_Warn == 2 || AEBS_Warn == 3)	//前碰撞
		{
			DisplayTruckFlg = 1;
			tpmsalarm = 0;
			update_tpmsvalue = 1;
			Clear_TruckArea();
			if((ptmr_Get(&adas_250ms_Tmr) >= 250) || (!ptmr_IsStart(&adas_250ms_Tmr)))
			{
				ptmr_Start(&adas_250ms_Tmr);
				retFCW = !retFCW;
			}
			// if(Get_Interval_Status(F_BUZZER_5) || Get_Interval_Status(F_BUZZER_6))
			if(retFCW)
				loc_RenderImg(346, 245, &Img_frontCollision);
			else
				loc_RenderImg(352, 284, &Img_truck);
		}
		else if(FCW_Level == 1 || AEBS_Warn == 1) //前碰撞
		{
			DisplayTruckFlg = 1;
			tpmsalarm = 0;
			update_tpmsvalue = 1;
			Clear_TruckArea();
			
			if((ptmr_Get(&adas_250ms_Tmr) >= 500) || (!ptmr_IsStart(&adas_250ms_Tmr)))
			{
				ptmr_Start(&adas_250ms_Tmr);
				retFCW = !retFCW;
			}
			// if(Get_Interval_Status(F_BUZZER_7))
			if(retFCW)
				loc_RenderImg(346, 245, &Img_frontCollision);
			else
				loc_RenderImg(352, 284, &Img_truck);
		}
		else if(LDW_Warn == 1) //左偏离
		{
			DisplayTruckFlg = 1;
			tpmsalarm = 0;
			update_tpmsvalue = 1;
			Clear_TruckArea();
			if((ptmr_Get(&adas_250ms_Tmr) >= 250) || (!ptmr_IsStart(&adas_250ms_Tmr)))
			{
				ptmr_Start(&adas_250ms_Tmr);
				retLDW = !retLDW;
			}
			if(retLDW)
			{
				show_ldws_left_lane();
				loc_RenderImg(352 - 69, 284, &Img_truck);
				// loc_RenderImg_BlendMode(352 - 69, 284,&Img_truck, R_DRW2D_BLENDMODE_CUSTOM);
			}
		}
		else if(LDW_Warn == 2) //右偏离
		{
			DisplayTruckFlg = 1;
			tpmsalarm = 0;
			update_tpmsvalue = 1;
			Clear_TruckArea();
			if((ptmr_Get(&adas_250ms_Tmr) >= 250) || (!ptmr_IsStart(&adas_250ms_Tmr)))
			{
				ptmr_Start(&adas_250ms_Tmr);
				retLDW = !retLDW;
			}
			if(retLDW)
			{
				show_ldws_right_lane();
				loc_RenderImg(352 + 69, 284, &Img_truck);
				// loc_RenderImg_BlendMode(352 + 69, 284,&Img_truck, R_DRW2D_BLENDMODE_CUSTOM); 
			}
		}
		else if(TPMS_Warn > 1)
		{
			DisplayTruckFlg = 1;
			if(tpmsalarm == 0)
			{
				tpmsalarm = 1;
				Clear_TruckArea();
				loc_Render_TPMSMAIN(180+180, 235,WHITE,"2");
			}
			tpms_mainframe(1);
		}
		else
		{
			if(DisplayTruckFlg == 1)
			{
				DisplayTruckFlg = 0;
				
				retLDW = 0;
				retFCW = 0;
				tpmsalarm = 0;
				update_tpmsvalue = 1;
				Clear_TruckArea();
				loc_RenderImg(352, 284, &Img_truck);
				
				ptmr_Stop(&adas_250ms_Tmr);
			}
		}
    }
	else
	{
		if(DisplayTruckFlg == 0)
		{
			DisplayTruckFlg = 1;
		}
		
		tpmsalarm = 0;
		update_tpmsvalue = 1;
	}
	
	end_draw();
}

void IC_zijian(void)
{
	uint8_t uniStr[64];
	static ptmrType_t self_check_Tmr = 0;
	static uint8_t CarShow = 0x00;
	static uint8_t TPMS_FirstShow = 0x00;
	
	static uint8_t XiaoTuBiaozijan = 0x00;
	static uint8_t XiaoTubiaoTimer = 0x00;
	
	static uint8_t DaTuBiaoZiJian_FistShow = 0x00;
	static uint8_t DaTuBiaoZiJian = 0x00;
	static uint8_t DaTuBiaoZiJian_ABS = 0x00;
	static uint8_t DaTuBiaoZiJian_EBS = 0x00;
	static uint8_t DaTuBiaoZiJian_TrailerABS = 0x00;
	static uint8_t DaTuBiaoZiJian_LDWS = 0x00;
	static uint8_t DaTuBiaoZiJian_FCWS = 0x00;
	static uint8_t DaTuBiaoZiJian_AEBS = 0x00;
	static uint8_t DaTuBiaoZiJian_LIM = 0x00;
	static uint8_t DaTuBiaoZiJian_TPMS = 0x00;
	
	uint16_t width;
	uint8_t fcolor[5];
	
	/* 无TPMS时, 显示卡车图标 */
	if((0x00 == CarShow)&&((eol_tpmsyesno == 0) || (eol_tpmsyesno >= 6)))
	{
		start_draw();
		loc_RenderImg(352, 284, &Img_truck);
		end_draw();
		CarShow = 0x01;
	}
	
	if(0xFF != Zijian)
	{
		DisplayTruckFlg = 1;
		
		start_draw();
		
		//小图标自检 EBS  车道右偏移  前向碰撞预警  限速
		if(0x00 == XiaoTuBiaozijan)
		{
			/*	计时小于1.5S */
			if(XiaoTubiaoTimer < 6)
			{
				XiaoTubiaoTimer++;
				
				fcolor[0] = YELLOW;
				fcolor[1] = YELLOW;
				fcolor[2] = YELLOW;
				fcolor[3] = YELLOW;
				fcolor[4] = YELLOW;
				
			}
			else
			{
				XiaoTuBiaozijan = 0x01; //小图标自检结束
				XiaoTubiaoTimer = 0x00;
				
				fcolor[0] = TOUMING;
				fcolor[1] = TOUMING;
				fcolor[2] = TOUMING;
				fcolor[3] = TOUMING;
				fcolor[4] = TOUMING;
			}
			
			if(TrailerABS_CheckFlg)
				loc_Render_FHP(625,238, fcolor[0], "m");//挂车ABS
			
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
				if(!XiaoTuBiaozijan)
					ABS_SelfCheckFlg = 1;
				else
					ABS_SelfCheckFlg = 0;
			}
			else if((eol_ebs == 5)||(eol_ebs == 6)||(eol_ebs ==10) || (eol_ebs == 13))
			{
				loc_Render_FHP(158, Y_LOCATION, fcolor[1], "3");//EBS
			}
			else ;
			
			if((0x02 == eol_ldwstype)||(0x03 == eol_ldwstype)||(0x04 == eol_ldwstype))
			{
				loc_Render_FHP(10, Y_LOCATION, fcolor[2], "0"); //车道右偏移
			}
			
			if((0x02 == eol_aebstype)||(0x03 == eol_aebstype)||(0x03 == eol_ldwstype))
			{
				loc_Render_FHP(56,Y_LOCATION, fcolor[3], "1"); //前向碰撞预警
			}
			
			loc_Render_FHP(535, Y_LOCATION, fcolor[4], "J"); //限速图标
			
		}
		
		//大图标自检
		if(DaTuBiaoZiJian == 0)
		{
			if(DaTuBiaoZiJian_FistShow == 0)
			{
				DaTuBiaoZiJian_FistShow = 1;
				
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
					DaTuBiaoZiJian_ABS = 1;
				}
				else if(eol_ebs == 5||eol_ebs == 6||eol_ebs ==10||eol_ebs == 13)
				{
					DaTuBiaoZiJian_EBS = 1;
				}
				else ;
				
				if(TrailerABS_CheckFlg)
					DaTuBiaoZiJian_TrailerABS = 1;
				/* 
				if(eol_ldwstype == 2 || eol_ldwstype == 3 || eol_ldwstype == 4)
				{
					DaTuBiaoZiJian_LDWS = 1;
				}
				if(eol_ldwstype == 3)
				{
					DaTuBiaoZiJian_FCWS = 1;
				}
				if(eol_aebstype == 2 || eol_aebstype == 3)
				{
					DaTuBiaoZiJian_AEBS = 1;
				} */
				
				if(eol_language == 0)
					DaTuBiaoZiJian_LIM = 1;
				
				if(eol_tpmsyesno > 0 && eol_tpmsyesno < 6)
				{
					DaTuBiaoZiJian_TPMS = 1;
				}
				
			}
			
			if(DaTuBiaoZiJian_ABS)
			{//ABS
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 1500)
				{
					Clear_TruckArea();
					loc_Render_BigFHP(BIG_X-54, BIG_Y, YELLOW, "W");
				}
				else
				{
					DaTuBiaoZiJian_ABS = 0;
					ptmr_Stop(&self_check_Tmr);
				}
			}
			else if(DaTuBiaoZiJian_EBS)
			{//EBS
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 1500)
				{
					Clear_TruckArea();
					loc_Render_BigFHP(BIG_X-54, BIG_Y, YELLOW, "G");
				}
				else
				{
					DaTuBiaoZiJian_EBS = 0;
					ptmr_Stop(&self_check_Tmr);
				}
			}
			else if(DaTuBiaoZiJian_TrailerABS)
			{//挂车ABS
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 1500)
				{
					Clear_TruckArea();
					loc_Render_BigFHP(BIG_X-40, BIG_Y, YELLOW, "V");
				}
				else
				{
					DaTuBiaoZiJian_TrailerABS = 0;
					ptmr_Stop(&self_check_Tmr);
				}
			}
			else if(DaTuBiaoZiJian_LDWS)
			{//LDWS
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 1500)
				{
					Clear_TruckArea();
					loc_Render_BigFHP(BIG_X-40, BIG_Y, YELLOW, "J");
					
					if(eol_language == 0)
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
						zk_oem2uni((uint8_t *)"车道偏离预警系统自检", uniStr, sizeof(uniStr));
						zk_printZH(BIG_X-24*10/2, BIG_Y+81, uniStr);
					}
					else
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
						zk_oem2uni((uint8_t *)"LDWS Self-inspection", uniStr, sizeof(uniStr));
						width = zk_getTextWidth((uint8_t *)"LDWS Self-inspection");
						zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
					}
				}
				else
				{
					DaTuBiaoZiJian_LDWS = 0;
					ptmr_Stop(&self_check_Tmr);
				}
				
			}
			else if(DaTuBiaoZiJian_FCWS)
			{//FCWS
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 1500)
				{
					Clear_TruckArea();
					loc_Render_BigFHP(BIG_X-78, BIG_Y, YELLOW, "L");
					
					if(eol_language == 0)
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
						zk_oem2uni((uint8_t *)"碰撞预警系统自检", uniStr, sizeof(uniStr));
						zk_printZH(BIG_X-24*8/2, BIG_Y+81, uniStr);
					}
					else
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
						zk_oem2uni((uint8_t *)"FCWS Self-inspection", uniStr, sizeof(uniStr));
						width = zk_getTextWidth((uint8_t *)"FCWS Self-inspection");
						zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
					}
				}
				else
				{
					DaTuBiaoZiJian_FCWS = 0;
					ptmr_Stop(&self_check_Tmr);
				}
			}
			else if(DaTuBiaoZiJian_AEBS)
			{//AEBS
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 1500)
				{
					Clear_TruckArea();
					loc_Render_BigFHP(BIG_X-78, BIG_Y, YELLOW, "L");
					
					if(eol_language == 0)
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
						zk_oem2uni((uint8_t *)"紧急制动系统自检", uniStr, sizeof(uniStr));
						zk_printZH(BIG_X-24*8/2, BIG_Y+81, uniStr);
					}
					else
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
						zk_oem2uni((uint8_t *)"AEBS Self-inspection", uniStr, sizeof(uniStr));
						width = zk_getTextWidth((uint8_t *)"AEBS Self-inspection");
						zk_printZH(BIG_X-width/2, BIG_Y+81, uniStr);
					}
				}
				else
				{
					DaTuBiaoZiJian_AEBS = 0;
					ptmr_Stop(&self_check_Tmr);
				}
			}
			else if(DaTuBiaoZiJian_LIM)
			{//车速限制指示
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 1500)
				{
					Clear_TruckArea();
					loc_Render_BigFHP(BIG_X-38,BIG_Y,YELLOW,"X");
					
					if(eol_language == 0)
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
						zk_oem2uni((uint8_t *)"车速限制指示", uniStr, sizeof(uniStr));
						zk_printZH(BIG_X-24*6/2, BIG_Y+81, uniStr);
					}
				}
				else
				{
					DaTuBiaoZiJian_LIM = 0;
					ptmr_Stop(&self_check_Tmr);
				}
			}
			else if(DaTuBiaoZiJian_TPMS)
			{//TPMS
				/*	计时小于8S	 */
				if(!ptmr_IsStart(&self_check_Tmr))
					ptmr_Start(&self_check_Tmr);
				if(ptmr_Get(&self_check_Tmr) < 8000)
				{
					if(0x00 == TPMS_FirstShow)
					{
						TPMS_FirstShow = 0x01;
						Clear_TruckArea();
						loc_Render_TPMSMAIN(180+180, 235,WHITE,"2");
					}
					tpms_mainframe(1);
				}
				else
				{
					TPMS_FirstShow = 0x00;
					DaTuBiaoZiJian_TPMS = 0;
				}
			}
			else
			{
				Clear_TruckArea();
				loc_RenderImg(352, 284, &Img_truck);
				DaTuBiaoZiJian = 0x01;
			}
		}
		
		if((0x00 != XiaoTuBiaozijan)&&(0x00 != DaTuBiaoZiJian))
		{
			Zijian = 0xFF;/* 自检结束 */
		}
		else
		{
			/* 等待自检结束 */
		}
		
		end_draw();
	}
	
}

void Display_Telltale(void)
{
	GeneralUse_t *GU_18F0010B_t = NULL;
	GeneralUse_t *GU_10F007E8_t = NULL;
	GeneralUse_t *GU_10FE6FE8_t = NULL;
	GeneralUse_t *GU_18FF16E8_t = NULL;
	GeneralUse_t *GU_18FF14E7_t = NULL;
	GeneralUse_t *GU_18FE5BE8_t = NULL;
	GeneralUse_t *Pcan_18FE5BE8_t = NULL;
	GeneralUse_t *Pcan_10F007E8 = NULL;
	GeneralUse_t *Pcan_0CF02F2A = NULL;
	VCU_18FFF531_t *VCU_18FFF531_d = NULL;
	GeneralUse_t *pFCV_18FFA6F5 = NULL;
	GeneralUse_t *GU_0x18F71E31_t = NULL;
	GeneralUse_t *EBC5_0x18FDC40B =NULL;
	
	GeneralUse_t *GU_18FECA2F_t = NULL;
	GeneralUse_t *GU_0CFE5A2F_t = NULL;
	
	GeneralUse_t *pPROP_18FF5527 = NULL;
	GeneralUse_t *pBMS_19FF5BF3 = NULL;
	VCU_04F02270_t *pVCU_04F02270 = NULL;
	
	VCU_18FFF531_d = (VCU_18FFF531_t*)can_getBCanBuffer(0x18FFF531);
	GU_18F0010B_t  =  (GeneralUse_t*)can_getBCanBuffer(0x18F0010B);
	pVCU_04F02270 = (VCU_04F02270_t*)can_getBCanBuffer(0x04F02270);
	
	start_draw();
	
	//LDWS、FCWS、AEBS、BSD、疲劳驾驶报警、EBS
	if(0xFF == Zijian)
	{
		loc_Render_FHP( 10, Y_LOCATION, TOUMING, "u"); //清除ECAS的显示
		loc_Render_FHP( 56, Y_LOCATION, TOUMING, "t"); //高度控制指示
		loc_Render_FHP(110, Y_LOCATION, TOUMING, "v"); //举升桥提升状态指示
		loc_Render_FHP(158, Y_LOCATION, TOUMING, "w"); //驱动辅助状态指示
		
	    if(eol_ldwstype == 3)	//东软、佑驾、南斗
		{
			//车道偏离系统LDW
			if(can_getBCanRxState(0x18FE5BE8) == CAN_FRAME_ST_RECVED)
			{
				GU_18FE5BE8_t = (GeneralUse_t*)can_getBCanBuffer(0x18FE5BE8);
				if(GU_18FE5BE8_t->byte1.bit78 == 0)//LDWS系统关闭/工作	
				{
					loc_Render_FHP(10,Y_LOCATION,YELLOW,"F"); //车道偏离系统关闭
					LdwsWarning  = 0x01;
					LDW_Warn = 0;
					offRoadCloseRequestDisp = 1;	//车道偏离系统关闭标识-----大图标
				}
				else
				{
					offRoadCloseRequestDisp = 0;
					
					GU_10F007E8_t = (GeneralUse_t*)can_getBCanBuffer(0x10F007E8);
					if(GU_10F007E8_t->byte1.bit12== 1)	//车道左偏离
					{
						loc_Render_FHP(10,Y_LOCATION,YELLOW,"D");
						LdwsWarning  = 0x01;
						LDW_Warn = 1;
					}
					else if(GU_10F007E8_t->byte1.bit34 == 1)	//车道右偏离
					{
						loc_Render_FHP(10,Y_LOCATION,YELLOW,"0");
						LdwsWarning  = 0x01;
						LDW_Warn = 2;
					}
					else if((GU_10F007E8_t->byte2.byte&0x0f) ==1 /*LDW：系统故障*/
					||(GU_10F007E8_t->byte2.byte&0x0f) ==2		 /*LDW：前视相机未标定*/
					||(GU_10F007E8_t->byte2.byte&0x0f) ==3		 /*LDW：系统故障*/
					||(GU_10F007E8_t->byte2.byte&0x0f) ==4)		 /*LDW：标定未完成*/
					{
						loc_Render_FHP(10,Y_LOCATION,YELLOW,"0");//车道右偏离
						LdwsWarning = 0x01;
						LDW_Warn = 0;
					}
					else
					{
						loc_Render_FHP(10,Y_LOCATION,TOUMING,"0");
						LdwsWarning = 0x00;
						LDW_Warn = 0;
					}
				}
			}
			else
			{
				if(CAN_FRAME_ST_TIMEOUT == can_getBCanRxState(0x18FE5BE8))
				{				
					loc_Render_FHP(10,Y_LOCATION,YELLOW,"0"); //RIGHTOFFROAD
					LdwsWarning = 0x01;
				}
				else
				{
					loc_Render_FHP(10,Y_LOCATION,TOUMING,"F");
					LdwsWarning = 0x00;
				}
				LDW_Warn = 0;		
				offRoadCloseRequestDisp = 0;
			}
			
			//前方碰撞预警系统FCW
			if(can_getBCanRxState(0x10FE6FE8) == CAN_FRAME_ST_RECVED)
			{
				GU_10FE6FE8_t = (GeneralUse_t*)can_getBCanBuffer(0x10FE6FE8);
				if((GU_10FE6FE8_t->byte8.byte&0x0f) == 0)	//前向碰撞警告系统关闭
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"G");
					FCWWarning = 0x01;
					FCW_Level = 0;
					fcwCloseRequestDisp = 1;		//碰撞预警系统关闭-----大图标
				}
				else if(((GU_10FE6FE8_t->byte8.byte&0x0f)==2)	//FCWS:前方碰撞系统前视相机未标定
				||((GU_10FE6FE8_t->byte8.byte&0x0f)==3))		//FCWS:前方碰撞系统前视相机故障
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
					FCWWarning = 0x01;
					FCW_Level = 0;
					fcwCloseRequestDisp = 0;
				}
				else
				{
					fcwCloseRequestDisp = 0;
					if(GU_10FE6FE8_t->byte7.bit78 == 2)	//FCWS碰撞预警二级警告
					{
						loc_Render_FHP(56,Y_LOCATION,RED,"1");
						FCWWarning = 0x01;
						FCW_Level = 2;
					}
					else if(GU_10FE6FE8_t->byte7.bit78 == 1)	//FCWS碰撞预警一级警告
					{

						loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
						FCWWarning = 0x01;
						FCW_Level = 1;
					}
					else if(GU_10FE6FE8_t->byte7.bit78 == 3)	//FCWS:前方碰撞系统故障
					{
						loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
						FCWWarning = 0x01;
						FCW_Level = 0;
					}
					else
					{
						loc_Render_FHP(56,Y_LOCATION,TOUMING,"1");
						FCWWarning = 0x00;
						FCW_Level = 0;
					}
				}
			}
			else
			{
				if(can_getBCanRxState(0x10FE6FE8) == CAN_FRAME_ST_TIMEOUT)
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
					FCWWarning = 0x01;
				}
				else
				{
					loc_Render_FHP(56,Y_LOCATION,TOUMING,"G");
					FCWWarning = 0x00;
				}
				FCW_Level = 0;
				fcwCloseRequestDisp = 0;
				
			}
		}
		else if((eol_ldwstype == 2) /*Knorr*/
		|| (eol_ldwstype == 4))	/*月照松*/
		{
			//LDWS
			if(can_getBCanRxState(0x18FE5BE8) == CAN_FRAME_ST_RECVED)
			{
				Pcan_18FE5BE8_t = (GeneralUse_t*)can_getBCanBuffer(0x18FE5BE8);
				if((Pcan_18FE5BE8_t->byte7.byte&0x0f)==2)//车道偏离预警系统关闭 
				{
					loc_Render_FHP(10,Y_LOCATION,YELLOW,"F");
					LdwsWarning = 0x01;
					LDW_Warn = 0;
					offRoadCloseRequestDisp = 1;	//车道偏离系统关闭标识-----大图标
				}
				else //if(GU_18FE5BE8_t->byte1.bit78 == 1)//not closed
				{
					offRoadCloseRequestDisp = 0;
					
					Pcan_10F007E8 = (GeneralUse_t*)can_getBCanBuffer(0x10F007E8);
					if(Pcan_10F007E8->byte1.bit78== 1)		//车道左偏离
					{
						loc_Render_FHP(10,Y_LOCATION,YELLOW,"D");//LEFTOFFROAD  -- 38
						LdwsWarning  = 0x01;
						LDW_Warn = 1;
					}
					else if(Pcan_10F007E8->byte1.bit56 == 1)	//车道右偏离
					{
						loc_Render_FHP(10,Y_LOCATION,YELLOW,"0");//RIGHTOFFROAD
						LdwsWarning = 0x01;
						LDW_Warn = 2;
					}
					else if(CurrentFltInfo[DM1_E8].FltBuf.AmberWarnLamp == 1) //黄色报警
					{
						loc_Render_FHP(10,Y_LOCATION,YELLOW,"0");//RIGHTOFFROAD
						LdwsWarning = 0x01;
						LDW_Warn = 0;
					}
					else
					{
						loc_Render_FHP(10,Y_LOCATION,TOUMING,"0");
						LdwsWarning = 0x00;
						LDW_Warn = 0;
					}
				}
			}
			else
			{
				if(can_getBCanRxState(0x18FE5BE8) == 2)
				{
					loc_Render_FHP(10,Y_LOCATION,YELLOW,"0");//RIGHTOFFROAD
					LdwsWarning = 0x01;
				}
				else
				{
					loc_Render_FHP(10,Y_LOCATION,TOUMING,"F");
					LdwsWarning = 0x00;
				}
				LDW_Warn = 0;
				offRoadCloseRequestDisp = 0;
			}	
			
		}
		else ;
		
		if(eol_aebstype == 2) //AEBS: 2-Knorr
		{
			if(can_getBCanRxState(0x0CF02F2A) == 1)
			{
				Pcan_0CF02F2A = (GeneralUse_t*)can_getBCanBuffer(0x0CF02F2A);
				
				if((Pcan_0CF02F2A->byte1.byte&0x0F) == 2)		//AEBS系统关闭
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"G");
					FCWWarning = 0x01;
					AEBS_Warn = 0;
					aebsRequestDisp = 1;			//自动紧急制动系统已关闭-----大图标
				}
				else if((Pcan_0CF02F2A->byte1.byte&0x0F) == 7)	//三级报警
				{
					
					loc_Render_FHP(56,Y_LOCATION,RED,"1");
					FCWWarning = 0x01;
					AEBS_Warn = 3;
					aebsRequestDisp = 0;
				}
				else if((Pcan_0CF02F2A->byte1.byte&0x0F) == 6)	//二级报警
				{
					loc_Render_FHP(56,Y_LOCATION,RED,"1");
					FCWWarning = 0x01;
					AEBS_Warn = 2;
					aebsRequestDisp = 0;
				}
				else if((Pcan_0CF02F2A->byte1.byte&0x0F) == 5)	//一级报警
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
					FCWWarning = 0x01;
					AEBS_Warn = 1;
					aebsRequestDisp = 0;
				}
				else if(CurrentFltInfo[DM1_2A].FltBuf.AmberWarnLamp == 1)
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
					FCWWarning = 0x01;
					AEBS_Warn = 0;
					aebsRequestDisp = 0;
				}
				else
				{
					loc_Render_FHP(56,Y_LOCATION,TOUMING,"1");
					FCWWarning = 0x00;
					AEBS_Warn = 0;
					aebsRequestDisp = 0;
				}
			}
			else
			{
				if(0x02 == (can_getBCanRxState(0x0CF02F2A)))
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
					FCWWarning = 0x01;
				}
				else
				{
					loc_Render_FHP(56,Y_LOCATION,TOUMING,"1");
					FCWWarning = 0x00;
				}
				AEBS_Warn = 0;
			}
		}
		else if(eol_aebstype == 3) //AEBS: 3-月照松
		{
			if(can_getBCanRxState(0x0CF02F2A) == 1)
			{
				Pcan_0CF02F2A = (GeneralUse_t*)can_getBCanBuffer(0x0CF02F2A);
				
				if((Pcan_0CF02F2A->byte1.byte&0x0F) == 2)  //AEBS系统关闭
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"G");  //FCW关闭
					FCWWarning = 0x01;
					AEBS_Warn = 0;
					aebsRequestDisp = 1;			//自动紧急制动系统已关闭”-----大图标
					// firsttime_KL2 = 0;			//紧急制动系统自检-----大图标
				}
				else
				{
					aebsRequestDisp = 0;
					if((Pcan_0CF02F2A->byte1.byte&0x0F) == 7||(Pcan_0CF02F2A->byte1.byte&0x0F) == 10)	//三级报警
					{
						loc_Render_FHP(56,Y_LOCATION,RED,"1");
						FCWWarning = 0x01;
						AEBS_Warn = 3;
					}
					else if((Pcan_0CF02F2A->byte1.byte&0x0F) == 6||(Pcan_0CF02F2A->byte1.byte&0x0F) == 9)	//二级报警
					{
						loc_Render_FHP(56,Y_LOCATION,RED,"1");
						FCWWarning = 0x01;
						AEBS_Warn = 2;
					}
					else if((Pcan_0CF02F2A->byte1.byte&0x0F) == 5||(Pcan_0CF02F2A->byte1.byte&0x0F) == 8)	//一级报警
					{
						loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
						FCWWarning = 0x01;
						AEBS_Warn = 1;
					}
					else if(CurrentFltInfo[DM1_A0].FltBuf.AmberWarnLamp == 1)
					{
						loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
						FCWWarning = 0x01;
						AEBS_Warn = 0;
					}
					else
					{
						loc_Render_FHP(56,Y_LOCATION,TOUMING,"1");
						FCWWarning = 0x00;
						AEBS_Warn=0;
						
					}
				}
			}
			else
			{
				if(can_getBCanRxState(0x0CF02F2A) == 2)
				{
					loc_Render_FHP(56,Y_LOCATION,YELLOW,"1");
					FCWWarning = 0x01;
				}
				else
				{
					loc_Render_FHP(56,Y_LOCATION,TOUMING,"1");
					FCWWarning = 0x00;
				}

				AEBS_Warn = 0;
			}
		}
		else ;
		
		//BSD 盲区探测
		if(eol_bdsconfig > 0)
		{
			if(can_getBCanRxState(0x18FF16E8) == 1)
			{
				GU_18FF16E8_t = (GeneralUse_t*)can_getBCanBuffer(0x18FF16E8);
				if((GU_18FF16E8_t->byte2.byte&0x0f) ==0)
				{
					loc_Render_FHP(110,Y_LOCATION,YELLOW,"H");  //BSD关闭
					BSDWarning = 0x01;
					BSD_Warn = 0;
					bsdCloseRequestDisp = 0;
					bsdRequestDisp = 0;
				}
				else if((GU_18FF16E8_t->byte2.byte&0x0f) ==2||(GU_18FF16E8_t->byte2.byte&0x0f) ==3)
				{
					loc_Render_FHP(110,Y_LOCATION,YELLOW,"2");  //右盲区探测系统未标定/右盲区探测系统相机故障
					BSDWarning = 0x01;
					BSD_Warn = 0;
					bsdCloseRequestDisp = 0;
					bsdRequestDisp = 0;
				}
				else
				{
					bsdCloseRequestDisp = 0;
					if(GU_18FF16E8_t->byte1.bit12 == 1)
					{
						loc_Render_FHP(110,Y_LOCATION,YELLOW,"2");//右盲区探测预警
						BSDWarning = 0x01;
						BSD_Warn = 1;
						bsdRequestDisp = 1;
					}
					else
					{
						loc_Render_FHP(110,Y_LOCATION,TOUMING,"2");//右盲区探测预警
						BSDWarning = 0x00;
						BSD_Warn = 0;
						bsdRequestDisp = 0;
					}
				}
			}
			else
			{
				loc_Render_FHP(110,Y_LOCATION,TOUMING,"H");//BSD关闭
				BSDWarning = 0x00;
				BSD_Warn = 0;
				bsdCloseRequestDisp = 0;
				bsdRequestDisp = 0;
			}
		}
		
		//疲劳驾驶报警
		if(eol_dfmsconfig > 0)
		{
			if(can_getBCanRxState(0x18FF14E7) == 1)
			{
				GU_18FF14E7_t = (GeneralUse_t*)can_getBCanBuffer(0x18FF14E7);
				
				if(GU_18FF14E7_t->byte3.bit12 == 0)
				{
					restRequestDisp = 0;
					loc_Render_FHP(760,236,YELLOW,"Z"); //疲劳驾驶报警
				}
				else
				{
					if(LED_COFFEE)
						restRequestDisp = 1;
					else
						restRequestDisp = 0;
					
					loc_Render_FHP(760,236,TOUMING,"Z");	
				}
			}
			else
			{
				restRequestDisp = 0;
				loc_Render_FHP(760,236,TOUMING,"Z");
			}
		}
		
		if((eol_ebs == 1) \
		| (eol_ebs == 2) \
		| (eol_ebs == 3) \
		| (eol_ebs == 4) \
		| (eol_ebs == 7) \
		| (eol_ebs == 8) \
		| (eol_ebs == 9) \
		| (eol_ebs == 11) \
		| (eol_ebs == 12))
		{
			if(LED_ABS)
				absRequestDisp = 1;
			else
				absRequestDisp = 0;
		}
		else if((eol_ebs == 5)||(eol_ebs == 6)||(eol_ebs ==10) || (eol_ebs == 13))
		{//标定为EBS系统时，显示EBS
			if(can_getBCanRxState(0x18F0010B) == CAN_FRAME_ST_RECVED)
			{
				if((GU_18F0010B_t->byte6.bit34) == 0x01)
				{
					loc_Render_FHP(158,Y_LOCATION,RED,"3");//EBS
					EBSWarning = 0x01;
					ebsRedRequestDisp = 1;
					ebsYellowRequestDisp = 0;
				}
				else if((GU_18F0010B_t->byte6.bit56) == 0x01)
				{	
					loc_Render_FHP(158,Y_LOCATION,YELLOW,"3");//EBS
					EBSWarning = 0x01;
					ebsRedRequestDisp = 0;
					ebsYellowRequestDisp = 1;
				}
				else
				{
					loc_Render_FHP(158,Y_LOCATION,TOUMING,"3");//EBS
					EBSWarning = 0x00;
					ebsRedRequestDisp = 0;
					ebsYellowRequestDisp = 0;
				}
			}
			else
			{
				if(can_getBCanRxState(0x18F0010B) == CAN_FRAME_ST_TIMEOUT)
				{
					loc_Render_FHP(158,Y_LOCATION,YELLOW,"3");//EBS
					EBSWarning = 0x01;
					ebsRedRequestDisp = 0;
					ebsYellowRequestDisp = 1;
				}
				else
				{
					loc_Render_FHP(158,Y_LOCATION,TOUMING,"3");//EBS
					EBSWarning = 0x00;
					ebsRedRequestDisp = 0;
					ebsYellowRequestDisp = 0;
				}
			}
		}
		else ;
	}

	//ECAS失效、ECAS警告灯、高度控制指示、举升桥提升状态指示、驱动辅助状态指示
	if(0xFF == Zijian)
	{
		// loc_Render_FHP( 10, Y_LOCATION, YELLOW, "u"); //ECAS警告/失效
		// loc_Render_FHP( 56, Y_LOCATION, YELLOW, "t"); //高度控制指示
		// loc_Render_FHP(110, Y_LOCATION, YELLOW, "v"); //举升桥提升状态指示
		// loc_Render_FHP(158, Y_LOCATION, YELLOW, "w"); //驱动辅助状态指示
		
		if((0x03 == eol_ecastype)  //Wabco ECAS
		|| (0x04 == eol_ecastype)  //Knorr ELC
		|| (0x05 == eol_ecastype)) //瑞丽ECAS
		{
			GU_18FECA2F_t = (GeneralUse_t *)can_getBCanBuffer(0x18FECA2F);		
			GU_0CFE5A2F_t = (GeneralUse_t *)can_getBCanBuffer(0x0CFE5A2F);
			
			if(0x00 == LdwsWarning)
			{
				if((0x01 == GU_18FECA2F_t->byte1.bit34)
				||((0x05 != eol_ecastype) && (0x01 == GU_18FECA2F_t->byte1.bit56)))
				{
					loc_Render_FHP(10,Y_LOCATION,RED,"u"); //ECAS失效 	对应的图标宽度：42
				}
				else if(((0x01 == (GU_0CFE5A2F_t->byte1.byte & 0xf0) >> 4))
					||(0x02 == (GU_0CFE5A2F_t->byte1.byte & 0xf0) >> 4)
					||(0x03 == (GU_0CFE5A2F_t->byte1.byte & 0xf0) >> 4))
				{		
					;
				}
				else
				{
					loc_Render_FHP(10,Y_LOCATION,YELLOW,"u"); //ECAS警告灯
				}
				
			}
			
			if(0x00 == FCWWarning)
			{
				 if((0x02 == (GU_0CFE5A2F_t->byte1.byte & 0xf0) >> 4)
					||(0x03 == (GU_0CFE5A2F_t->byte1.byte & 0xf0) >> 4))
				 {					 
					 loc_Render_FHP(56,Y_LOCATION,YELLOW,"t"); //高度控制指示  
				 }
				 else{}

			}
			
			if(0x00 == BSDWarning)//举升桥提升状态指示
			{
				if(0x01 == GU_0CFE5A2F_t->byte5.bit78  || IN2)
				{					
					loc_Render_FHP(110,Y_LOCATION,YELLOW,"v"); //举升桥提升状态指示
				}
			}
			
			if(0x00 == EBSWarning)
			{
				if(((0x01 == (GU_0CFE5A2F_t->byte4.byte & 0xf0) >> 4))
					||(0x02 == (GU_0CFE5A2F_t->byte4.byte & 0xf0) >> 4)
					||(0x07 == (GU_0CFE5A2F_t->byte4.byte & 0xf0) >> 4))//驱动辅助状态指示
				{
					loc_Render_FHP(158,Y_LOCATION,YELLOW,"w"); //驱动辅助状态指示
				}	
			}
		}
		
		//三桥提升报警指示
		if(0x00 == BSDWarning)
		{
			if(eol_drivetype == 11)
			{
				if(IN10) 
				{
					loc_Render_FHP(110,Y_LOCATION,YELLOW,"v"); //举升桥提升状态指示
				}
				else
				{
					loc_Render_FHP(110,Y_LOCATION,TOUMING,"v"); //举升桥提升状态指示
				}
			}
		}
	}
	
	//READY
    if((TotalDM == 0) && (dm1_show_flg == 0))
	{
		//if(VCU_18FFF531_d->ready)
		if(VehicleReadyFlag)//lyx
		{
			loc_Render_FHP(339, 71, GREEN, "4"); // READY
			
			ready_disp_flag = 1;
		}
		else
		{
			loc_Render_FHP(339, 71, TOUMING, "4");//READY
			
			ready_disp_flag = 0;
		}
    }
	else
	{
		if(ready_disp_flag == 1)
		{
			loc_Render_FHP(339, 71, TOUMING, "4");//READY
		}
		ready_disp_flag = 0;
	}
	
	//STOP、 限速、 HAZARD 紧急故障、 EAC 空压机故障、 锂电相关
	if(0xFF == Zijian)  
	{
		BUZZ_STOP  = (CurrentFltInfo[DM1_F4].FltBuf.RedStopLamp == 1); /*电池管理*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_EF].FltBuf.RedStopLamp == 1); /*电机1控制*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_F0].FltBuf.RedStopLamp == 1); /*电机2控制*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_30].FltBuf.RedStopLamp == 1); /*空压机*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_1A].FltBuf.RedStopLamp == 1); /*DCDC*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_03].FltBuf.RedStopLamp == 1); /*自动变速箱*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_2E].FltBuf.RedStopLamp == 1); /*高压转向*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_1E].FltBuf.RedStopLamp == 1); /*高压配电盒*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_4D].FltBuf.RedStopLamp == 1); /*低压转向*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_31].FltBuf.RedStopLamp == 1); /*HCU*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_82].FltBuf.RedStopLamp == 1); /*TMS*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_F5].FltBuf.RedStopLamp == 1); /*燃电系统 FCU*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_F6].FltBuf.RedStopLamp == 1); /*氢系统 HMS*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_84].FltBuf.RedStopLamp == 1); /*外挂DCDC1*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_85].FltBuf.RedStopLamp == 1); /*外挂DCDC2*/ 
		BUZZ_STOP |= (CurrentFltInfo[DM1_50].FltBuf.RedStopLamp == 1); /*EPB*/ 
		BUZZ_STOP |= (get_dm1_flt_status(DM1_F4, 521335,  3) == true); /*BMS:电池包火灾故障*/
		
		//限速
		if((M_ON)
		&& (eol_language == 0)
		&& ((VCU_18FFF531_d->bData[0]&0xC0) != 0) 
		&& can_getBCanRxState(0x18FFF531) == CAN_FRAME_ST_RECVED)
		{
			if(get_display_speed() > eol_XianSuFunction)
			{
				XIANSUSTUS = 0x01;
				LmtSpdRequestDisp = 1;
			}
			else if(0x01 == XIANSUSTUS)
			{
				if(get_display_speed() <= (eol_XianSuFunction - 3))
				{
					XIANSUSTUS = 0x00;
					LmtSpdRequestDisp = 0;
				}
			}
		}
		else
		{
			XIANSUSTUS = 0x00;
			LmtSpdRequestDisp = 0;
		}
		
		//HAZARD  紧急故障
		HAZARD_ON = (IN12 || BMS_SAFETY_LINE_SG);
		hazardRequestDisp = HAZARD_ON;
		
		//EAC 空压机故障
		EAC_ON = ((CurrentFltInfo[DM1_30].FltBuf.AmberWarnLamp == 1) || (CurrentFltInfo[DM1_30].FltBuf.RedStopLamp == 1)); 
		
		if(BUZZ_STOP || HAZARD_ON || EAC_ON )
		{
			loc_ClearRect(535, 13, 265, 38);
			
			if(BUZZ_STOP)	
			{
				loc_Render_FHP(535, Y_LOCATION, RED, "5"); //STOP
				XIANSUSTUS = 0x00;
			}
			else if(XIANSUSTUS)
			{
				loc_Render_FHP(535, Y_LOCATION, YELLOW, "J");//限速图标
			}
			else
			{
				// 锂电相关
				pBMS_19FF5BF3 = (GeneralUse_t*)can_getBCanBuffer(0x19FF5BF3);
				if((can_getBCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)&& (pVCU_04F02270->low_battery_warning == 1))
				{
					loc_RenderImg(535, Y_LOCATION, &Img_LowBatteryWarning);
				}
				if(pBMS_19FF5BF3->byte1.bit1234 == 7) //电池加热中
				{
					loc_Render_FHP_LowBatt(535, Y_LOCATION, YELLOW, "#");
				}
				else if(pBMS_19FF5BF3->byte1.bit1234 == 8) //电池加热完成
				{
					loc_Render_FHP_LowBatt(535, Y_LOCATION, GREEN, "#");
				}
				else if(pBMS_19FF5BF3->byte1.bit1234 == 4) //锂电SOC
				{
					loc_Render_FHP_LowBatt(535, Y_LOCATION, GREEN, "\"");
				}
			}
			
			//HAZARD  紧急故障
			if(HAZARD_ON)
			{
				loc_Render_FHP(623, Y_LOCATION, YELLOW, "6");//HAZARD
			}
			
			//EAC 空压机故障
			if(EAC_ON)
			{	
				loc_Render_FHP(728, Y_LOCATION, RED, "7");//EAC
			}
		}
		else
		{
			loc_ClearRect(535, 13, 65, 38);
			
			if(XIANSUSTUS)
			{
				loc_Render_FHP(535,Y_LOCATION,YELLOW,"J");//限速图标
			}
			else
			{
				// 锂电相关
				pBMS_19FF5BF3 = (GeneralUse_t*)can_getBCanBuffer(0x19FF5BF3);
				if((can_getBCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)&& (pVCU_04F02270->low_battery_warning == 1))
				{
					loc_RenderImg(535, Y_LOCATION, &Img_LowBatteryWarning);
				}
				if(pBMS_19FF5BF3->byte1.bit1234 == 7) //电池加热中
				{
					loc_Render_FHP_LowBatt(535, Y_LOCATION, YELLOW, "#");
				}
				else if(pBMS_19FF5BF3->byte1.bit1234 == 8) //电池加热完成
				{
					loc_Render_FHP_LowBatt(535, Y_LOCATION, GREEN, "#");
				}
				else if(pBMS_19FF5BF3->byte1.bit1234 == 4) //锂电SOC
				{
					loc_Render_FHP_LowBatt(535, Y_LOCATION, GREEN, "\"");
				}
			}
		}
	}
	
	//PTO上装指示灯
	//转向锁止 / 变速箱故障
	//FCV/EV，燃料电池冷却液温度过高报警，燃电系统故障，氢气泄露
	//挂车ABS
	//巡航里程低
	//ESC OFF
	if(0xFF == Zijian)
	{
		//PTO上装指示灯
		GU_0x18F71E31_t = (GeneralUse_t*)can_getBCanBuffer(0x18F71E31);	
		if((GU_0x18F71E31_t->bData[1]&0x01) == 1)
			loc_Render_FHP(10,105,YELLOW,"x"); //PTO上装指示灯
		else
			loc_Render_FHP(10,105,TOUMING,"x"); //PTO上装指示灯
		
		//转向锁止
		if(eol_drivetype == 11)
		{
			if(!IN30 )
				loc_Render_FHP(56,105,YELLOW,"z"); //转向锁止
			else
				loc_Render_FHP(56,105,TOUMING,"z"); //转向锁止
		}
		
		//变速箱故障
		if(eol_atmtype != 26)
		{
			if(CurrentFltInfo[DM1_03].FltBuf.RedStopLamp == 1)
				loc_Render_FHP(92,105,RED,"~");
			else if(CurrentFltInfo[DM1_03].FltBuf.AmberWarnLamp == 1)
				loc_Render_FHP(92,105,YELLOW,"~");
			else
				loc_Render_FHP(92,105,TOUMING,"~");
		}
		else
		{
			static ptmrType_t atm_flt_Tmr = 0;
			
			GeneralUse_t *pETC7_18FE4A03 = (GeneralUse_t*)can_getBCanBuffer(0x18FE4A03);
			if(pETC7_18FE4A03->byte6.bit34 == 1)
			{
				loc_Render_FHP(92,105,YELLOW,"~");
			}
			else if(pETC7_18FE4A03->byte6.bit34 == 2)
			{
				if((ptmr_Get(&atm_flt_Tmr) >= 500) || (!ptmr_IsStart(&atm_flt_Tmr)))
				{
					loc_Render_FHP(92,105,YELLOW,"~");
					ptmr_Start(&atm_flt_Tmr);
				}
				else
				{
					loc_Render_FHP(92,105,TOUMING,"~");
				}
			}
			else
			{
				loc_Render_FHP(92,105,TOUMING,"~");
			}
		}
		
		//FCV/EV，燃料电池冷却液温度过高报警，燃电系统故障，氢气泄露
		if(eol_fadongjiorkaji == 0x2d)
		{
			if(eol_language == 0)
			{
				if(can_getBCanRxState(0x18FFF531) == 1)
				{
					if(VCU_18FFF531_d->mode == 0)
						loc_Render_FHP_Text(60,236, GREEN, "!");
					else if(VCU_18FFF531_d->mode == 1)
						loc_Render_FHP_Text(60,236, GREEN, "\"");
					else
						loc_Render_FHP_Text(60,236, TOUMING, "!");
				}
				else
				{
					loc_Render_FHP_Text(60,236, TOUMING, "!");
				}
			}
			else
			{
				if(can_getBCanRxState(0x18FFF531) == 1)
				{
					if(VCU_18FFF531_d->mode == 0)
						loc_Render_FHP(132,236,GREEN,"]");//EV
					else if(VCU_18FFF531_d->mode == 1)
						loc_Render_FHP(132,236,GREEN,"^");//FCV
					else
						loc_Render_FHP(132,236,TOUMING,"^");
				}
				else
				{
					loc_Render_FHP(132,236,TOUMING,"^");
				}
			}
			
			if(get_dm1_flt_status(DM1_F5, 11187, 0) == true)
				loc_Render_FHP(710, 350, RED,"["); //燃料电池冷却液温度过高报警
			else
				loc_Render_FHP(710, 350, TOUMING,"[");	
			
			pFCV_18FFA6F5 = (GeneralUse_t*)can_getBCanBuffer(0x18FFA6F5);	
			if(pFCV_18FFA6F5->bData[0]==9)
				loc_Render_FHP(750, 350,YELLOW,"\\"); //燃电系统故障
			else
				loc_Render_FHP(750, 350,TOUMING,"\\");
			
			
			FCV_0x18FFA5F6_t *pFCV_18FFA5F6=NULL;
			pFCV_18FFA5F6 =  (FCV_0x18FFA5F6_t*)can_getBCanBuffer(0x18FFA5F6);
			if(pFCV_18FFA5F6->H2concentration <= 60000)
			{
				if(pFCV_18FFA5F6->H2concentration > 6000)
				{
					H2Leakage = 1;
				}
				else if(H2Leakage==1&&pFCV_18FFA5F6->H2concentration < 5500)
				{
					H2Leakage = 0;
				}
				else ;
			}
			else
			{
				H2Leakage=0;
			}
			if(H2Leakage == 1)
				loc_Render_FHP(670, 350,RED,"_"); //氢气泄露
			else
				loc_Render_FHP(670, 350,TOUMING,"_");
			
			if(get_H2_soc() <= 15)
				loc_Render_FHP(630, 350,RED,"K"); //加氢
			else
				loc_Render_FHP(630, 350,TOUMING,"K"); 
		}
		else
		{
			loc_Render_FHP(630, 350, TOUMING, "K"); //加氢
			loc_Render_FHP(670, 350, TOUMING, "_"); //氢气泄露
			loc_Render_FHP(710, 350, TOUMING, "[");	//燃料电池冷却液温度过高报警
			loc_Render_FHP(750, 350, TOUMING, "\\"); //燃电系统故障
		}
		
		//挂车ABS
		if(IN25)
		{
			Trailer_absRequestDisp = 1;
			loc_Render_FHP(625,238, YELLOW, "m"); //挂车ABS
		}
		else
		{
			Trailer_absRequestDisp = 0;
			loc_Render_FHP(625,238, TOUMING, "m"); //挂车ABS
		}
		//续航里程低
		if(get_batt_pack_soc() < 20U)
			loc_Render_FHP(14, 236,RED,"A");
		else
			loc_Render_FHP(14, 236,TOUMING,"A");
		
		//ESC OFF 指示
		if(GU_18F0010B_t->byte3.bit34 == 1) 
			loc_Render_FHP(598, 236,YELLOW,"{");
		else
			loc_Render_FHP(598, 236,TOUMING,"{");
		
		if(eol_atmtype == 17 /*ZF、自动 12 档*/
		|| eol_atmtype == 20 /*东风、自动 6 档*/
		|| eol_atmtype == 22 /*东风、自动 14 档*/
		|| eol_atmtype == 28 /*伊顿、自动 12 档*/
		|| eol_atmtype == 29 /*陕齿、自动 8 档*/
		|| eol_atmtype == 31 /*陕齿、自动 6 档*/
		|| eol_atmtype == 32 /*东风自主 AMT 二代*/
		)
		{
			static ptmrType_t podao_Tmr = 0;
			static uint8_t    podao_status = 0;
			
			EBC5_0x18FDC40B = (GeneralUse_t*)can_getBCanBuffer(0x18FDC40B);
			if(GU_18F0010B_t->byte3.bit56 == 1) //坡道起步开关开启
			{
				if((EBC5_0x18FDC40B->bData[0]&0xE0) == 0x20)
				{
					loc_Render_FHP(666,236, GREEN, "|"); //坡道起步模式
				}
				else if((EBC5_0x18FDC40B->bData[0]&0xE0) == 0x40)
				{
					if((ptmr_Get(&podao_Tmr) >= 200) || (!ptmr_IsStart(&podao_Tmr)))
					{
						ptmr_Start(&podao_Tmr);
						podao_status = !podao_status;
					}
					if(podao_status)
					{
						loc_Render_FHP(666,236, GREEN, "|"); //坡道起步模式
					}
					else
					{
						loc_Render_FHP(666,236, TOUMING, "|"); //坡道起步模式
					}
				}
				else
				{
					if((ptmr_Get(&podao_Tmr) >= 500) || (!ptmr_IsStart(&podao_Tmr)))
					{
						ptmr_Start(&podao_Tmr);
						podao_status = !podao_status;
					}
					if(podao_status)
					{
						loc_Render_FHP(666,236, WHITE, "|"); //坡道起步开关开启
					}
					else
					{
						loc_Render_FHP(666,236, TOUMING, "|"); //坡道起步
					}
				}
			}
			else
			{
				loc_Render_FHP(666,236, TOUMING, "|"); //坡道起步模式
			}
		}
		
		if(eol_drivetype != 11)
		{
			if(IN30)
			{
				loc_Render_FHP(715,236, YELLOW, "}"); //空滤指示
			}
			else
			{
				loc_Render_FHP(715,236, TOUMING, "}");
			}
		}
		
		pPROP_18FF5527 = (GeneralUse_t*)can_getBCanBuffer(0x18FF5527);	
		if((pPROP_18FF5527->bData[0]&0x01) == 1)
			loc_Render_FHP(134,105,GREEN,"&"); //牵引车 空车头模式
		else
			loc_Render_FHP(134,105,TOUMING,"&"); //牵引车 空车头模式
		
		if(IN5) //后照灯指示
			loc_RenderImg(190, 105, &Img_RearLamp);
		else
			LCD_DrawRect(190, 105, 40, 38, TOUMING);
		if((can_getBCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)
		&& (pVCU_04F02270->eps_fault == 1))
		{
			loc_ClearRect(EPS_X_LOCATION, EPS_Y_LOCATION, 55, 40);
			loc_RenderImg(EPS_X_LOCATION, EPS_Y_LOCATION, &Img_EPS);
		}
		else
		{
			loc_ClearRect(EPS_X_LOCATION, EPS_Y_LOCATION, 55, 40);
		}
		// loc_Render_FHP(10, 105, YELLOW,"x"); //PTO上装指示灯
		// loc_Render_FHP(56, 105, YELLOW,"z"); //转向锁止
		// loc_Render_FHP(92, 105, RED,"~");    //变速箱故障
		// loc_Render_FHP(134, 105, GREEN,"&"); //牵引车 空车头模式
		// loc_RenderImg(190, 105, &Img_RearLamp); //后照灯
		// loc_RenderImg(240, 105, &Img_LmtSpd90); //限速90
		
		// loc_Render_FHP(598, 236, YELLOW,"{"); //ESC OFF 指示
		// loc_Render_FHP(625, 236, YELLOW, "m"); //挂车ABS
		// loc_Render_FHP(666, 236, GREEN, "|"); //坡道起步
		// loc_Render_FHP(715, 236, YELLOW, "}"); //空滤指示
		// loc_Render_FHP(760, 236, YELLOW,"Z"); //疲劳驾驶报警
		
		// // loc_Render_FHP(132, 236, GREEN,"^");//FCV
		
		// loc_Render_FHP(14, 236, RED,"A"); //续航里程低
		// loc_Render_FHP_Text(60, 236, GREEN, "!"); //纯电优先
		// loc_Render_FHP(630, 350, RED,"K"); //加氢
		// loc_Render_FHP(670, 350, RED,"_"); //氢气泄露
		// loc_Render_FHP(710, 350, RED,"["); //燃料电池冷却液温度过高报警
		// loc_Render_FHP(750, 350, YELLOW,"\\"); //燃电系统故障
		
	}
	
	//TPMS
	if(0xFF == Zijian)
	{
		if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
		{
			if((TyreLocation_fastlostgas[0].byte>0)
			||(TyreLocation_fastlostgas[1].byte>0)
			||(TyreLocation_fastlostgas[2].byte>0)
			||(TyreLocation_fastlostgas[3].byte>0)
			){
				tpmsRequestDisp = 5;
				TPMS_Warn = 5;
			}
			else if(
			(TyreLocation_pressurel[0].byte>0)
			||(TyreLocation_pressurel[1].byte>0)
			||(TyreLocation_pressurel[2].byte>0)
			||(TyreLocation_pressurel[3].byte>0)
			){
				tpmsRequestDisp = 4;
				TPMS_Warn = 4;
			}
			else if(
			(TyreLocation_pressureh[0].byte>0)
			||(TyreLocation_pressureh[1].byte>0)
			||(TyreLocation_pressureh[2].byte>0)
			||(TyreLocation_pressureh[3].byte>0)
			){
				tpmsRequestDisp = 3;
				TPMS_Warn = 3;
			}
			else if(
			(TyreLocation_temperature[0].byte>0)
			||(TyreLocation_temperature[1].byte>0)
			||(TyreLocation_temperature[2].byte>0)
			||(TyreLocation_temperature[3].byte>0)
			){
				tpmsRequestDisp = 2;
				TPMS_Warn = 2;
			}
			else if(
			(CurrentFltInfo[DM1_33].FltBuf.AmberWarnLamp == 1)
			||(2 == can_getBCanRxState(0x18FFE633))
			){
				tpmsRequestDisp = 1;
				TPMS_Warn = 1;
			}
			else 
			{
				tpmsRequestDisp = 0;
				TPMS_Warn = 0;
			}
	    }
	}
	
	//智能驾驶状态指示 / 自动大灯
	if(0xFF == Zijian)
	{
		static ptmrType_t aduStatusTmr = 0;
		static uint8_t aduStatusBlinkFlg = 0;
		
		uint8_t  LmtSpdNum = 0;
		const Img_t * const ImgLmtSpd[] = {
			&Img_LmtSpd10,
			&Img_LmtSpd20,
			&Img_LmtSpd30,
			&Img_LmtSpd40,
			&Img_LmtSpd50,
			&Img_LmtSpd60,
			&Img_LmtSpd70,
			&Img_LmtSpd80,
			&Img_LmtSpd90,
			&Img_LmtSpd100,
			&Img_LmtSpd110,
			&Img_LmtSpd120,
			&Img_LmtSpd130,
		};
		
		if(can_getBCanRxState(0x18FF709E) == CAN_FRAME_ST_RECVED)
		{
			GeneralUse_t *pADU_0x18FF709E = (GeneralUse_t*)can_getBCanBuffer(0x18FF709E);
			
			loc_ClearRect(218, Y_LOCATION, 60, 38);
			
			if((pADU_0x18FF709E->bData[0]&0x0f) == 7)
			{
				if((ptmr_Get(&aduStatusTmr) >= 250) || (!ptmr_IsStart(&aduStatusTmr)))
				{
					ptmr_Start(&aduStatusTmr);
					
					aduStatusBlinkFlg = !aduStatusBlinkFlg;
					
					if(aduStatusBlinkFlg)
						loc_Render_FHP(218, Y_LOCATION, RED, "!"); //
					else
						loc_Render_FHP(218, Y_LOCATION, TOUMING,"!"); //
				}
			}
			else if((pADU_0x18FF709E->bData[0]&0x0f) == 6)
			{
				aduStatusBlinkFlg = 0;
				ptmr_Stop(&aduStatusTmr);
				loc_Render_FHP(218, Y_LOCATION, RED, "!"); //
			}
			else if((pADU_0x18FF709E->bData[0]&0x0f) == 5)
			{
				if((ptmr_Get(&aduStatusTmr) >= 500) || (!ptmr_IsStart(&aduStatusTmr)))
				{
					ptmr_Start(&aduStatusTmr);
					
					aduStatusBlinkFlg = !aduStatusBlinkFlg;
					
					if(aduStatusBlinkFlg)
						loc_Render_FHP(218, Y_LOCATION, GREEN, "!"); //
					else
						loc_Render_FHP(218, Y_LOCATION, TOUMING,"!"); //
				}
				
			}
			else if((pADU_0x18FF709E->bData[0]&0x0f) == 4)
			{
				aduStatusBlinkFlg = 0;
				ptmr_Stop(&aduStatusTmr);
				loc_Render_FHP(218, Y_LOCATION, GREEN, "!"); //
			}
			else if((pADU_0x18FF709E->bData[0]&0x0f) == 3)
			{
				if((ptmr_Get(&aduStatusTmr) >= 500) || (!ptmr_IsStart(&aduStatusTmr)))
				{
					ptmr_Start(&aduStatusTmr);
					
					aduStatusBlinkFlg = !aduStatusBlinkFlg;
					
					if(aduStatusBlinkFlg)
						loc_Render_FHP(218, Y_LOCATION, WHITE, "!"); //
					else
						loc_Render_FHP(218, Y_LOCATION, TOUMING,"!"); //
				}
			}
			else if((pADU_0x18FF709E->bData[0]&0x0f) == 2)
			{
				aduStatusBlinkFlg = 0;
				ptmr_Stop(&aduStatusTmr);
				loc_Render_FHP(218, Y_LOCATION, WHITE, "!"); //
			}
			else if((pADU_0x18FF709E->bData[0]&0x0f) == 1)
			{
				aduStatusBlinkFlg = 0;
				ptmr_Stop(&aduStatusTmr);
				loc_Render_FHP(218, Y_LOCATION, GRAY2, "!"); //
			}
			else
			{
				aduStatusBlinkFlg = 0;
				ptmr_Stop(&aduStatusTmr);
				// loc_Render_FHP(218, Y_LOCATION, TOUMING,"!"); //
				if(AutoLowBeamFlg)
				{
					loc_RenderImg(238, Y_LOCATION, &Img_AutoLowBeam);
				}
				else
				{
					LCD_DrawRect(238, Y_LOCATION, 38, 38, TOUMING);
				}
			}
			
			LmtSpdNum = (pADU_0x18FF709E->bData[3]&0x0f);
			if(LmtSpdNum > 0 && LmtSpdNum <= 0x0C)
				loc_RenderImg(240, 105, ImgLmtSpd[LmtSpdNum - 1]);
			else
				LCD_DrawRect(240, 105, 40, 40, TOUMING);
			
			
			aduRequestDisp = pADU_0x18FF709E->bData[1];
			
			if((aduRequestDisp >= 111 && aduRequestDisp <= 115)
			|| (aduRequestDisp >= 128 && aduRequestDisp <= 138)
			|| (aduRequestDisp == 143)
			|| (aduRequestDisp == 149)
			|| (aduRequestDisp == 150) )
			{/*B 类报警*/
				if(aduRequestDisp >= 129 && aduRequestDisp <= 138)
				{
					ADU_Warn = 4; //2Hz
				}
				else if((aduRequestDisp == 112)
				|| (aduRequestDisp == 113)
				|| (aduRequestDisp == 149) )
				{
					ADU_Warn = 2; //1Hz
				}
				else if(aduRequestDisp == 150)
				{
					ADU_Warn = 3; //FCWS二级报警声音
				}
				else
				{
					ADU_Warn = 1; //单声报警
				}
			}
			else if((aduRequestDisp >= 116 && aduRequestDisp <= 127)
			|| (aduRequestDisp >= 139 && aduRequestDisp <= 142)
			|| (aduRequestDisp >= 144 && aduRequestDisp <= 148) )
			{/*A 类报警*/
				if(aduRequestDisp >= 116 && aduRequestDisp <= 142)
				{
					ADU_Warn = 2; //1Hz
				}
				else 
				{
					ADU_Warn = 1; //单声报警
				}
			}
			else
			{
				aduRequestDisp = 0;
				ADU_Warn = 0;
			}
		}
		else
		{
			aduStatusBlinkFlg = 0;
			aduRequestDisp = 0;
			ADU_Warn = 0;
			ptmr_Stop(&aduStatusTmr);
			// loc_Render_FHP(218, Y_LOCATION, TOUMING,"!");
			LCD_DrawRect(240, 105, 40, 40, TOUMING);
			loc_ClearRect(218, Y_LOCATION, 60, 38);
			if(AutoLowBeamFlg) //自动大灯
			{
				loc_RenderImg(238, Y_LOCATION, &Img_AutoLowBeam); //自动大灯
			}
			else
			{
				LCD_DrawRect(238, Y_LOCATION, 38, 38, TOUMING);
			}
		}
	}
	
	end_draw();
	
}

void loc_auxiliary_braking(void) //辅助制动、巡航、PTO取力
{
	VCU_18FFF531_t *VCU_18FFF531_d = NULL;
	GeneralUse_t   *pPROP_18FF5527 = NULL;
	GeneralUse_t   *pECR1_18F00010 = NULL;
	GeneralUse_t   *pTP1_18FFC006 = NULL;
	GeneralUse_t   *Pcan_0x0CFF0931 = NULL; 
	
	VCU_18FFF531_d = (VCU_18FFF531_t*)can_getBCanBuffer(0x18FFF531);
	pPROP_18FF5527 = (GeneralUse_t *)can_getBCanBuffer(0x18FF5527);
	pECR1_18F00010 = (GeneralUse_t *)can_getBCanBuffer(0x18F00010);
	pTP1_18FFC006  = (GeneralUse_t *)can_getBCanBuffer(0x18FFC006);
	Pcan_0x0CFF0931 = (GeneralUse_t*)can_getBCanBuffer(0x0CFF0931);
	
	uint8_t zhuliFlg = 0;
	uint8_t FuzhuZhiDong = 0;
	uint8_t Dangwei = 0;
	int16_t brake_torque = 0;
	float   mcu_current = 0;
	static ptmrType_t FuZhuZhiDongGear0_Tmr = 0;
	
	if(0x01 == pPROP_18FF5527->bData[2])
	{
		Dangwei = 0x01;
	}
	else if(0x03 == pPROP_18FF5527->bData[2])
	{
		Dangwei = 0x02;
	}
	else if(0x0F == pPROP_18FF5527->bData[2])
	{
		Dangwei = 0x03;
	}
	else if(0x3F == pPROP_18FF5527->bData[2])
	{
		Dangwei = 0x04;
	}
	else if(0x7F == pPROP_18FF5527->bData[2])
	{
		Dangwei = 0x05;
	}
	else 
	{
		Dangwei = 0x00;
	}
	
	if( (pECR1_18F00010->bData[1] < 125U) && (pECR1_18F00010->bData[1] >= 25U) )
		brake_torque = pECR1_18F00010->bData[1] - 125;
	else
		brake_torque = 0;
	
	mcu_current = get_mot_curr();
	
	FuzhuZhiDong = ((brake_torque != 0) || (mcu_current < 0U));  //判断辅助制动工作
	zhuliFlg     = ((0x00 != FuzhuZhiDong) || (0x00 != Dangwei));
	
	uint8_t steep_slope_descent_status = (pPROP_18FF5527->bData[0]&0x18);
	
	if((0x00 == HAZARD_ON)&&(0x00 == EAC_ON)&&(0x00 == BUZZ_STOP))
	{
		if((0x08 == steep_slope_descent_status) 
		|| (0x10 == steep_slope_descent_status))
		{
			start_draw();
			loc_ClearRect(600, 13, 200, 38);
			
			if(0x08 == steep_slope_descent_status) 
			{
				loc_Render_FHP_AuxiliaryBraking(600, 13, WHITE, ")");
				zk_init(loc_Drw2dDev, &GUI_Fontxunhang, VM_COLOR_WHITE, 0);
				zk_printPosNum(681, 18, (pPROP_18FF5527->byte2.byte), 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				loc_Render_FHP_AuxiliaryBraking(600, 13, GREEN, ")");
				zk_init(loc_Drw2dDev, &GUI_Fontxunhang, VM_COLOR_GREEN, 0);
				zk_printPosNum(681, 18, (pPROP_18FF5527->byte2.byte), 0, ZK_ALIGN_RIGHT);
			}
			
			if((0x01 == eol_huansuqi)||(0x01 == pTP1_18FFC006->byte1.bit78))
			{
				if(0x01 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "!");  /* 符号: !  含义:助力制动A档 */
				}
				else if(0x02 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "\""); /* 符号: "  含义:助力制动1档 */
				}
				else if(0x03 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "#");  /* 符号: #  含义:助力制动2档 */
				}
				else if(0x04 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "$");  /* 符号: $  含义:助力制动3档 */
				}
				else if(0x05 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "%");  /* 符号: %  含义:助力制动4档 */
				}
				else
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, TOUMING, "!");
				}
			}
			else
			{
				if(0x01 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "\""); /* 符号: "  含义:助力制动1档 */
				}
				else if(0x02 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "#");  /* 符号: #  含义:助力制动2档 */
				}
				else if(0x03 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "$");  /* 符号: $  含义:助力制动3档 */
				}
				else if(0x04 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "%");  /* 符号: %  含义:助力制动4档 */
				}
				else if(0x05 == Dangwei)
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "&");  /* 符号: &  含义:助力制动5档 */
				}
				else
				{
					loc_Render_FHP_AuxiliaryBraking(732, 13, TOUMING, "!");
				}
			}
			
			end_draw();
			
			ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
		}
		else if(0x01 == Pcan_0x0CFF0931->byte3.bit12)
		{
			start_draw();
			loc_ClearRect(600, 13, 200, 38);
			zk_init(loc_Drw2dDev, &GUI_Fontxunhang, VM_COLOR_GREEN, 0);
			zk_printZH(630, 13, zk_oem2uniStr((uint8_t *)"SET"));
			zk_printPosNum(725, 13,((Pcan_0x0CFF0931->byte3.byte)>>2U), 1, ZK_ALIGN_RIGHT);
			zk_printZH(725, 13, zk_oem2uniStr((uint8_t *)"km/h"));
			end_draw();
			
			ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
		}
		else if(0x01 == VCU_18FFF531_d->xunhang_status)
		{
			start_draw();
			loc_ClearRect(600, 13, 200, 38);
			zk_init(loc_Drw2dDev, &GUI_Fontxunhang, VM_COLOR_GREEN, 0);
			zk_printZH(630, 13, zk_oem2uniStr((uint8_t *)"SET"));
			if(250 >= VCU_18FFF531_d->xunhangspeed)
			{
				zk_printPosNum(725, 13, VCU_18FFF531_d->xunhangspeed, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_printPosNum(725, 13,250, 0, ZK_ALIGN_RIGHT);
			}
			zk_printZH(725, 13, zk_oem2uniStr((uint8_t *)"km/h"));
			end_draw();
			
			ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
		}
		else if(0x01 == VCU_18FFF531_d->pto_status)
		{
			start_draw();
			loc_ClearRect(600, 13, 200, 38);
			zk_init(loc_Drw2dDev, &GUI_Fontxunhang, VM_COLOR_GREEN, 0);
			zk_printZH(605, 13, zk_oem2uniStr((uint8_t *)"SET"));
			zk_printPosNum(735, 13,VCU_18FFF531_d->piorpm, 0, ZK_ALIGN_RIGHT);
			zk_printZH(735, 13, zk_oem2uniStr((uint8_t *)"rpm"));
			end_draw();
		}
		else 
		{
			start_draw();
			
			loc_ClearRect(600, 13, 200, 38);
			
			if(0x01 == zhuliFlg)
			{
				if((0x01 == eol_huansuqi)||(0x01 == pTP1_18FFC006->byte1.bit78))
				{
					if(0x01 == FuzhuZhiDong)
					{
						if(0x00 == Dangwei)
						{
							if(!ptmr_IsStart(&FuZhuZhiDongGear0_Tmr))
								ptmr_Start(&FuZhuZhiDongGear0_Tmr);
							if(ptmr_Get(&FuZhuZhiDongGear0_Tmr) >= 1000)
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "'");  /* 符号: '  含义:助力制动_无数字 */
						}
						else
						{
							ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
							
							if(0x01 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "!");  /* 符号: !  含义:助力制动A档 */
							}
							else if(0x02 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "\""); /* 符号: "  含义:助力制动1档 */
							}
							else if(0x03 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "#");  /* 符号: #  含义:助力制动2档 */
							}
							else if(0x04 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "$");  /* 符号: $  含义:助力制动3档 */
							}
							else if(0x05 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "%");  /* 符号: %  含义:助力制动4档 */
							}
							else
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, TOUMING, "!");
							}
						}
					}
					else
					{
						ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
						
						if(0x01 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "!");  /* 符号: !  含义:助力制动A档 */
						}
						else if(0x02 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "\""); /* 符号: "  含义:助力制动1档 */
						}
						else if(0x03 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "#");  /* 符号: #  含义:助力制动2档 */
						}
						else if(0x04 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "$");  /* 符号: $  含义:助力制动3档 */
						}
						else if(0x05 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "%");  /* 符号: %  含义:助力制动4档 */
						}
						else
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, TOUMING, "!");
						}
					}
				}
				else
				{
					if(0x01 == FuzhuZhiDong)
					{
						if(0x00 == Dangwei)
						{
							if(!ptmr_IsStart(&FuZhuZhiDongGear0_Tmr))
								ptmr_Start(&FuZhuZhiDongGear0_Tmr);
							if(ptmr_Get(&FuZhuZhiDongGear0_Tmr) >= 1000)
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "'");  /* 符号: '  含义:助力制动_无数字 */
							
						}
						else
						{
							ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
							
							if(0x01 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "\""); /* 符号: "  含义:助力制动1档 */
							}
							else if(0x02 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "#");  /* 符号: #  含义:助力制动2档 */
							}
							else if(0x03 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "$");  /* 符号: $  含义:助力制动3档 */
							}
							else if(0x04 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "%");  /* 符号: %  含义:助力制动4档 */
							}
							else if(0x05 == Dangwei)
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, GREEN, "&");  /* 符号: &  含义:助力制动5档 */
							}
							else
							{
								loc_Render_FHP_AuxiliaryBraking(732, 13, TOUMING, "!");
							}
						}
					}
					else
					{
						ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
						
						if(0x01 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "\""); /* 符号: "  含义:助力制动1档 */
						}
						else if(0x02 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "#");  /* 符号: #  含义:助力制动2档 */
						}
						else if(0x03 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "$");  /* 符号: $  含义:助力制动3档 */
						}
						else if(0x04 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "%");  /* 符号: %  含义:助力制动4档 */
						}
						else if(0x05 == Dangwei)
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, WHITE, "&");  /* 符号: &  含义:助力制动5档 */
						}
						else
						{
							loc_Render_FHP_AuxiliaryBraking(732, 13, TOUMING, "!");
						}
					}
				}
			}
			else
			{
				ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
			}
			
			end_draw();
		}
	}
	else
	{
		ptmr_Stop(&FuZhuZhiDongGear0_Tmr);
	}
}





