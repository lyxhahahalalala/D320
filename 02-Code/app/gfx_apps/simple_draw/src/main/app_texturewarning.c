/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_texturewarning.c
 * @brief  texture warn info display on the top of soc cover
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

#include "string.h"
#include "SYSTEM.h"
#include "zk.h"
#include "init.h"
#include "uds_support.h"

#include "adc.h"
#include "ptmr.h"
#include "app_texturewarning.h"
#include "app_can.h"
#include "app_telltale.h"
#include "app_data_center.h"
#include "app_dm1.h"

/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */

#define BLANK    300
#define HEIGHT   73


/*******************************************************************************
 * TYPEDEFS
 */


/**
 * \ variables
 */


/*******************************************************************************
 * CONSTANTS
 */



typedef struct {
	uint8_t priority;
    const uint8_t *text[2]; //报警文字
	uint32_t fcolor;
    uint8_t condition;
} TEXT_WARN_t;
#define TEXT_WARN_LIST_NUM   (sizeof(TextWarnList) / sizeof(TEXT_WARN_t))
static TEXT_WARN_t  TextWarnList[] = 
{
/* 0*/{ 0, {(uint8_t *)"气压过低",                        (uint8_t *)"Air pressure too low"},  VM_COLOR_YELLOW,0},
/* 1*/{ 0, {(uint8_t *)"气压过高",                        (uint8_t *)"Air pressure too high"}, VM_COLOR_YELLOW,0},

/* 2*/{ 0, {(uint8_t *)"气压传感器短路到地/开路",         (uint8_t *)"Air pressure sensor shorted to ground ,or open circuit"}, VM_COLOR_YELLOW,0},
/* 3*/{ 0, {(uint8_t *)"气压传感器短路到电源",            (uint8_t *)"Air pressure sensor shorted to power"}, VM_COLOR_YELLOW,0},
/* 4*/{ 0, {(uint8_t *)"车速传感器电源短路到地/开路",     (uint8_t *)"Speed sensor power supply short to ground ,or open circuit"}, VM_COLOR_YELLOW,0},
/* 5*/{ 0, {(uint8_t *)"车速传感器电源短路到电源",        (uint8_t *)"Speed sensor power supply shorted to power"}, VM_COLOR_YELLOW,0},
/* 6*/{ 0, {(uint8_t *)"环境温度传感器短路到地/开路",     (uint8_t *)"Ambient temp sensor short to ground ,or open circuit"}, VM_COLOR_YELLOW,0},
/* 7*/{ 0, {(uint8_t *)"环境温度传感器短路到电源/开路",   (uint8_t *)"Ambient temp sensor shorted to power ,or open circuit"}, VM_COLOR_YELLOW,0},

/* 8*/{ 0, {(uint8_t *)"手刹未解除",                      (uint8_t *)"Please release handbrake"}, VM_COLOR_YELLOW,0},
/* 9*/{ 0, {(uint8_t *)"疲劳驾驶警告",                    (uint8_t *)"Fatigue Driving Warning"}, VM_COLOR_YELLOW,0},
/*10*/{ 0, {(uint8_t *)"电机冷却液过温报警",              (uint8_t *)"Motor coolant overheated warning"}, VM_COLOR_YELLOW,0},
/*11*/{ 0, {(uint8_t *)"您已超速,请减速慢行!",            (uint8_t *)"Overspeed, please slow down!"}, VM_COLOR_YELLOW,0},
/*12*/{ 0, {(uint8_t *)"空气弹簧处于充放气状态",          (uint8_t *)"Air spring is inflated or deflated"}, VM_COLOR_YELLOW,0},
/*13*/{ 0, {(uint8_t *)"空气悬架偏离正常高度",            (uint8_t *)"ECAS deviated from normal height"}, VM_COLOR_YELLOW,0},
/*14*/{ 0, {(uint8_t *)"ECAS高度传感器异常",              (uint8_t *)"ECAS altitude sensor abnormal"}, VM_COLOR_YELLOW,0},

/*15*/{ 0, {(uint8_t *)"快速漏气报警",                    (uint8_t *)"Tire quick leak warning"}, VM_COLOR_YELLOW,0},
/*16*/{ 0, {(uint8_t *)"胎压过低报警",                    (uint8_t *)"Tire low pressure warning"}, VM_COLOR_YELLOW,0},
/*17*/{ 0, {(uint8_t *)"胎压过高报警",                    (uint8_t *)"Tire high pressure warning"}, VM_COLOR_YELLOW,0},
/*18*/{ 0, {(uint8_t *)"胎温过高报警",                    (uint8_t *)"Excessive tire temp warning"}, VM_COLOR_YELLOW,0},
/*19*/{ 0, {(uint8_t *)"TPMS系统故障",                    (uint8_t *)"TPMS fault warning"}, VM_COLOR_YELLOW,0},

/*20*/{ 0, {(uint8_t *)"高压未上电",                      (uint8_t *)"High voltage is not powered"}, VM_COLOR_YELLOW,0},
/*21*/{ 0, {(uint8_t *)"高压已上电，车辆可正常运行",      (uint8_t *)"High voltage powered on"}, VM_COLOR_GREEN,0},
/*22*/{ 0, {(uint8_t *)"能量回收",                        (uint8_t *)"Energy recovery"}, VM_COLOR_GREEN,0},
/*23*/{ 0, {(uint8_t *)"车辆处于可行驶模式,下车注意安全", (uint8_t *)"Driving mode, Caution！"}, VM_COLOR_YELLOW,0},

/*24*/{ 0, {(uint8_t *)"SOC低，请注意",                   (uint8_t *)"Low SOC"}, VM_COLOR_YELLOW,0},
/*25*/{ 0, {(uint8_t *)"SOC低，请充电",                   (uint8_t *)"Low SOC, please charge"}, VM_COLOR_YELLOW,0},
/*26*/{ 0, {(uint8_t *)"SOC过低，请立即充电",             (uint8_t *)"SOC is too low, please charge now"}, VM_COLOR_YELLOW,0},
/*27*/{ 0, {(uint8_t *)"电池管理系统故障",                (uint8_t *)"Battery management system failure"}, VM_COLOR_YELLOW,0},

/*28*/{ 0, {(uint8_t *)"吹扫进行中",                      (uint8_t *)"Purging in progress"}, VM_COLOR_GREEN,0},
/*29*/{ 0, {(uint8_t *)"加氢盖已打开",                    (uint8_t *)"H2 cap is open"}, VM_COLOR_RED,0},
/*30*/{ 0, {(uint8_t *)"氢气剩余量低,请加氢",             (uint8_t *)"Low H2 residue, Please add"}, VM_COLOR_RED,0},

/*31*/{ 0, {(uint8_t *)"临时停车制动启动",                (uint8_t *)"Temporary parking brake activated"}, VM_COLOR_YELLOW,0},
/*32*/{ 0, {(uint8_t *)"请踩住脚刹释放驻车",              (uint8_t *)"Depress the footbrake to release"}, VM_COLOR_YELLOW,0},
/*33*/{ 0, {(uint8_t *)"气压低，驻车暂时无法解除",        (uint8_t *)"Low air pressure! Parking brake locked"}, VM_COLOR_YELLOW,0},
/*34*/{ 0, {(uint8_t *)"换挡开关未回N档",                 (uint8_t *)"Please put the gear lever to N"}, VM_COLOR_YELLOW,0},

/*35*/{ 0, {(uint8_t *)"EPB功能失效，使用车轮楔块",       (uint8_t *)"EPB disabler, Use wheel wedges"}, VM_COLOR_YELLOW,0},
/*36*/{ 0, {(uint8_t *)"手动操作EPB开关功能失效",         (uint8_t *)"The manual EPB switch fails"}, VM_COLOR_YELLOW,0},
/*37*/{ 0, {(uint8_t *)"车间模式，熄火自动驻车制动无法应用", (uint8_t *)"Workshop Model"}, VM_COLOR_YELLOW,0},
/*38*/{ 0, {(uint8_t *)"EPB系统失效",                     (uint8_t *)"The EPB system fails"}, VM_COLOR_YELLOW,0},
/*39*/{ 0, {(uint8_t *)"驻车制动正在执行挂车检查测试",    (uint8_t *)"EPB trailer inspection test"}, VM_COLOR_YELLOW,0},

/*40*/{ 0, {(uint8_t *)"翼门已开启",                      (uint8_t *)"Wing doors is opened"}, VM_COLOR_YELLOW,0},

/*41*/{ 0, {(uint8_t *)"电池加热中，请勿起动车辆",          NULL}, VM_COLOR_YELLOW,0},
/*42*/{ 0, {(uint8_t *)"电池加热完成，可以起动车辆",        NULL}, VM_COLOR_YELLOW,0},
/*43*/{ 0, {(uint8_t *)"电池电芯温度低，请开启电池加热",     NULL}, VM_COLOR_YELLOW,0},
/*44*/{ 0, {(uint8_t *)"未授权零部件连接告警",             NULL}, VM_COLOR_YELLOW,0},

/*45*/{ 0, {(uint8_t *)"转向助力即将失效，请立即靠边停车", NULL}, VM_COLOR_YELLOW,0},

/*46*/{ 0, {(uint8_t *)"拖车模式已激活",              (uint8_t *)"The trailer mode has been activated"}, VM_COLOR_GREEN,0},
/*47*/{ 0, {(uint8_t *)"拖车模式激活失败",             (uint8_t *)"Trailer mode activation failed"}, VM_COLOR_RED,0},
/*48*/{ 0, {(uint8_t *)"拖车模式已退出",              (uint8_t *)"The trailer mode has been exited"}, VM_COLOR_YELLOW,0},

/*49*/{ 0, {(uint8_t *)"请重新挂挡",                 (uint8_t *)"Please change gears again"}, VM_COLOR_YELLOW,0},
/*50*/{ 0, {(uint8_t *)"请尽快离开车辆！",             (uint8_t *)"Please leave the vehicle!"}, VM_COLOR_RED,0},
/*51*/{ 0, {(uint8_t *)"驻车故障，请安全停车检查！",   (uint8_t *)"Parking fault, stop safely!"}, VM_COLOR_RED,0},
/*52*/{ 0, {(uint8_t *)"高压未断开，请驻车下电！",     (uint8_t *)"High voltage remains on!"}, VM_COLOR_RED,0},



};
static void TextWarn_condition_check(void)
{
	uint8_t ret = 0;
    uint8_t Vspeed;
	
    VCU_18FFF531_t *VCU_18FFF531_d  = NULL;
	EPB_18FF3C50_t *pEPB_18FF3C50_t = NULL;
	GeneralUse_t   *GU_0CFE5A2F_t   = NULL;
	GeneralUse_t   *GU_18FECA2F_t   = NULL;
	
	pEPB_18FF3C50_t = (EPB_18FF3C50_t*)can_getPCanBuffer(0x18FF3C50);
    VCU_18FFF531_d  = (VCU_18FFF531_t*)can_getPCanBuffer(0x18FFF531);
	GU_0CFE5A2F_t   = (GeneralUse_t *)can_getBCanBuffer(0x0CFE5A2F);
	GU_18FECA2F_t   = (GeneralUse_t *)can_getBCanBuffer(0x18FECA2F);
	
	Vspeed = get_vehicle_speed();
	
	TextWarnList[0].condition  = ((get_front_airpressure() < QiyaLowLmt) || (get_rear_airpressure() < QiyaLowLmt)); // 气压过低
	TextWarnList[1].condition  = ((get_front_airpressure() > QiyaHighLmt) || (get_rear_airpressure() > QiyaHighLmt));   // 气压过高
	TextWarnList[2].condition  = (0); // 气压传感器短路到地/开路
	TextWarnList[3].condition  = (0); // 气压传感器短路到电源
    TextWarnList[5].condition  = (0); // 车速传感器电源短路到电源
    TextWarnList[6].condition  = (0); // 环境温度传感器短路到地/开路
    TextWarnList[7].condition  = (0); // 环境温度传感器短路到电源
	TextWarnList[8].condition  = (PARK_alarmflag); // 手刹未解除
    TextWarnList[9].condition  = (LED_COFFEE);     // 疲劳驾驶警告
    TextWarnList[10].condition = (LED_Water);      // 电机冷却液过温报警
	TextWarnList[11].condition = (LED_OverSpeed);  // 您已超速，请减速慢行！
	TextWarnList[12].condition = (LED_ECAS); //空气弹簧处于充放气状态
	
	ret  = ((GU_0CFE5A2F_t->byte1.byte&0xf0) != 0x10);
	ret &= ((GU_0CFE5A2F_t->byte1.byte&0xf0) != 0x20);
	ret &= ((GU_0CFE5A2F_t->byte1.byte&0xf0) != 0x30);
	ret &= (eol_ecastype == 3 || eol_ecastype == 4 || eol_ecastype == 5); //增加了标定限定
	TextWarnList[13].condition = ((ret == 1)&&(Vspeed > 30)); //空气悬架偏离正常高度
	
	ret  = ((eol_ecastype == 3 || eol_ecastype == 4 || eol_ecastype == 5) && (GU_18FECA2F_t->byte1.bit34 == 0x01));
	ret |= ((eol_ecastype == 3 || eol_ecastype == 4) && (GU_18FECA2F_t->byte1.bit56 == 0x01));
	TextWarnList[14].condition = (ret); //ECAS高度传感器异常
	
	TextWarnList[15].condition = (TPMS_Warn == 5); //快速漏气报警
	TextWarnList[16].condition = (TPMS_Warn == 4); //胎压过低报警 
	TextWarnList[17].condition = (TPMS_Warn == 3); //胎压过高报警
	TextWarnList[18].condition = (TPMS_Warn == 2); //胎温过高报警
	TextWarnList[19].condition = (TPMS_Warn == 1); //TPMS系统故障
	
	TextWarnList[20].condition = (VCU_18FFF531_d->status == 0 && can_getPCanRxState(0x18FFF531) == CAN_FRAME_ST_RECVED); //高压未上电
	TextWarnList[21].condition = (VCU_18FFF531_d->status == 1); //高压已上电，车辆可正常运行
	TextWarnList[22].condition = ((VCU_18FFF531_d->status == 3 ) && (Vspeed > 10)); //能量回收
	TextWarnList[23].condition = (Drive_DoorOpen); // 车辆出于可行驶模式，下车注意安全
	
	TextWarnList[24].condition = (get_dm1_flt_status(DM1_F4, 521309, 17) == true); // SOC低，请注意
    TextWarnList[25].condition = (get_dm1_flt_status(DM1_F4, 521309, 18) == true); // SOC低，请充电
    TextWarnList[26].condition = (get_dm1_flt_status(DM1_F4, 521309,  1) == true); // SOC过低
	TextWarnList[27].condition = (0); // 电池管理系统故障
	
	if(eol_fadongjiorkaji == 0x2d)
	{
		GeneralUse_t   *FCV_0x18FFA1F6  = NULL;
		FCV_0x18FFA1F6  = (GeneralUse_t*)can_getPCanBuffer(0x18FFA1F6);
		TextWarnList[28].condition = 0; //吹扫进行中
		TextWarnList[29].condition = ((0x40 == (0xC0 & FCV_0x18FFA1F6->bData[0]))); //加氢盖已打开
		TextWarnList[30].condition = (FCV_0x18FFA1F6->byte2.byte <= 13); //氢气剩余量低,请加氢
	}
	else
	{
		TextWarnList[28].condition = (0); //吹扫进行中
		TextWarnList[29].condition = (0); //加氢盖已打开
		TextWarnList[30].condition = (0); //氢气剩余量低,请加氢
	}
	TextWarnList[31].condition = (pEPB_18FF3C50_t->temporary_parking_brake_activated == 1); //临时停车制动启动
	TextWarnList[32].condition = (pEPB_18FF3C50_t->handbrake_release == 1); //请踩住脚刹释放驻车
    TextWarnList[33].condition = (pEPB_18FF3C50_t->handbrake_release == 3); //气压低，驻车暂时无法解除
	TextWarnList[34].condition = (VCU_18FFF531_d->gear_status == 1); //换挡开关未回N档
	
	TextWarnList[35].condition = (eol_epbtype == 2 && (pEPB_18FF3C50_t->work_mode == 7) || (pEPB_18FF3C50_t->work_mode == 254)); //EPB功能失效，使用车轮楔块
	TextWarnList[36].condition = (eol_epbtype == 2 && pEPB_18FF3C50_t->work_mode == 2); //手动操作EPB开关功能失效
	TextWarnList[37].condition = (eol_epbtype == 2 && pEPB_18FF3C50_t->work_mode == 1); //车间模式，熄火自动驻车制动无法应用
	TextWarnList[38].condition = (eol_epbtype == 2 && can_getPCanRxState(0x18FF3C50) == CAN_FRAME_ST_TIMEOUT); //EPB系统失效
	TextWarnList[39].condition = (eol_epbtype == 2 && pEPB_18FF3C50_t->Trailer_test == 1); //驻车制动正在执行挂车检查测试
	
	TextWarnList[40].condition = (IN11); //翼门已开启
	
	GeneralUse_t *pBMS_19FF5BF3 = (GeneralUse_t*)can_getPCanBuffer(0x19FF5BF3);
	TextWarnList[41].condition = (pBMS_19FF5BF3->byte1.bit1234 == 7); //电池加热中，请勿起动车辆
	TextWarnList[42].condition = (pBMS_19FF5BF3->byte1.bit1234 == 8); //电池加热完成，可以起动车辆
	TextWarnList[43].condition = (pBMS_19FF5BF3->byte1.bit1234 == 9); //电池电芯温度低，请开启电池加热
	
	GeneralUse_t *bTBOX_18FF8F25 = (GeneralUse_t*)can_getBCanBuffer(0x18FF8F25);
	TextWarnList[44].condition  = (bTBOX_18FF8F25->byte2.bit1234 == 2); //未授权零部件连接告警(TBOX 不匹配整车 VIN)
	TextWarnList[44].condition |= (bTBOX_18FF8F25->byte2.bit5678 == 2); //未授权零部件连接告警(智能座舱不匹配整车 VIN)
	TextWarnList[44].condition |= (bTBOX_18FF8F25->byte3.bit1234 == 2); //未授权零部件连接告警(行驶记录仪不匹配整车 VIN)
	
	
	GeneralUse_t *LSCU_0x0CF6034D = (GeneralUse_t*)can_getBCanBuffer(0x0CF6034D);
	Buzz_LSCU_Countdown = (LSCU_0x0CF6034D->byte1.bit12 == 1 && Vspeed > 3);
	TextWarnList[45].condition = (Buzz_LSCU_Countdown); //转向助力即将失效，请立即靠边停车
	
	
	if(eol_atmtype == 44U) //博世两档电驱桥
	{
		static uint8_t TrailerModeFlg = 0;
		
		GeneralUse_t *TCU_0x18FF0B03 =  (GeneralUse_t*)can_getPCanBuffer(0x18FF0B03);
		
		if(TCU_0x18FF0B03->byte7.bit12 == 3 || TCU_0x18FF0B03->byte7.bit12 == 2)
		{
			TextWarnList[46].condition = (TCU_0x18FF0B03->byte7.bit12 == 3); //拖车模式已激活
			TextWarnList[47].condition = (TCU_0x18FF0B03->byte7.bit12 == 2); //拖车模式激活失败
			TextWarnList[48].condition = (0); //拖车模式已退出
			TrailerModeFlg = 1;
		}
		else
		{
			TextWarnList[46].condition = (0); //拖车模式已激活
			TextWarnList[47].condition = (0); //拖车模式激活失败
			TextWarnList[48].condition = (TrailerModeFlg); //拖车模式已退出
			TrailerModeFlg = 0; //显示3s
		}
	}
	else
	{
		TextWarnList[46].condition = 0; //拖车模式已激活
		TextWarnList[47].condition = 0; //拖车模式激活失败
		TextWarnList[48].condition = 0; //拖车模式已退出
	}
	
	GeneralUse_t *pPROP_18FF5527 = (GeneralUse_t *)can_getPCanBuffer(0x18FF5527);
	Buzz_GearMismatched = (pPROP_18FF5527->byte1.bit3 == 1);
	TextWarnList[49].condition = (Buzz_GearMismatched); //请重新挂挡
	
	VCU_04F02270_t *pVCU_04F02270 = (VCU_04F02270_t*)can_getPCanBuffer(0x04F02270);

	if(can_getPCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)
	{
		TextWarnList[50].condition = pVCU_04F02270->instrument_display_1;
		TextWarnList[51].condition = pVCU_04F02270->instrument_display_2;
		TextWarnList[52].condition = pVCU_04F02270->instrument_display_3;
	}
	else
	{
		TextWarnList[50].condition = 0;
		TextWarnList[51].condition = 0;
		TextWarnList[52].condition = 0;
	}
	
}
static void autoScrollShowTextWarn(void)
{
	static ptmrType_t textWarnTmr = 0;
	static uint8_t textWarnFlg = 0;
	static uint8_t TextID = 0;
	static uint16_t Xwidth = 0;
	uint8_t uniStr[200];
	
	if((ptmr_Get(&textWarnTmr) > 3000) || (!ptmr_IsStart(&textWarnTmr)))
	{
		TextWarn_condition_check();
		for(uint8_t i = 0, n = TextID + 1; i< TEXT_WARN_LIST_NUM; i++, n++)
		{
			if (n >= TEXT_WARN_LIST_NUM)
			{
				n -= TEXT_WARN_LIST_NUM;
			}
			
			if(TextWarnList[n].condition) 
			{
				
				textWarnFlg = 1;
				TextID = n;
				
				ptmr_Start(&textWarnTmr);
				
				break;
			}
		}
		
		start_draw();
		
		// LCD_DrawRect(8, 200, 370, 50, TOUMING);
		LCD_DrawRect(8, 204, Xwidth + 4, 32, TOUMING);
		if(textWarnFlg)
		{
			textWarnFlg = 0;
			
			if(eol_language == 0)
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, TextWarnList[TextID].fcolor, 0);
				zk_oem2uni(TextWarnList[TextID].text[eol_language], uniStr, sizeof(uniStr));
				Xwidth = zk_getTextWidth(TextWarnList[TextID].text[eol_language]);
				zk_printZH(10, 204, uniStr);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh24, TextWarnList[TextID].fcolor, 0);
				zk_oem2uni(TextWarnList[TextID].text[eol_language], uniStr, sizeof(uniStr));
				Xwidth = zk_getTextWidth(TextWarnList[TextID].text[eol_language]);
				zk_printZH(10, 204, uniStr);
			}
			
		}
		end_draw();
    }
	
}


#define TEXT_TABLE_LIST_NUM   (sizeof(firstLineTable) / sizeof(TEXT_WARN_t))
static TEXT_WARN_t firstLineTable[] = 
{
	{ 0, {(uint8_t *)"动力电池严重过温,请速安全撤离!", (uint8_t *)"Power battery overtemp,Evacuate safely!"}, VM_COLOR_RED, 0},
	{ 1, {(uint8_t *)"低压电池电压低!",                (uint8_t *)"Auxiliary battery voltage is low"}, VM_COLOR_RED, 0},
	{ 2, {(uint8_t *)"吹扫进行中",                     (uint8_t *)"Purging in progress"}, VM_COLOR_GREEN, 0},
	{ 3, {(uint8_t *)"FCS自检中",                      (uint8_t *)"FCS self-check"}, VM_COLOR_GREEN, 0},
	{ 4, {(uint8_t *)"FCS自检完成",                    (uint8_t *)"FCS ready"}, VM_COLOR_GREEN, 0},
	{ 5, {(uint8_t *)"FCS待机",                        (uint8_t *)"FCS standby"}, VM_COLOR_GREEN, 0},
	{ 6, {(uint8_t *)"FCS启动中",                      (uint8_t *)"FCS start"}, VM_COLOR_GREEN, 0},
	{ 7, {(uint8_t *)"FCS功率输出",                    (uint8_t *)"FCS output"}, VM_COLOR_GREEN, 0},
	{ 8, {(uint8_t *)"FCS维护模式",                    (uint8_t *)"FCS maintain"}, VM_COLOR_GREEN, 0},
	{ 9, {(uint8_t *)"FCS紧急停机",                    (uint8_t *)"FCS emergency shutdown"}, VM_COLOR_RED, 0},
	{10, {(uint8_t *)"FCS故障停机",                    (uint8_t *)"FCS accidental shutdown"}, VM_COLOR_RED, 0},
	{11, {(uint8_t *)"FCS正常停机",                    (uint8_t *)"FCS over"}, VM_COLOR_GREEN, 0},
	{12, {(uint8_t *)"远程功能禁用开关已打开,请关闭!", (uint8_t *)"Service mode,Remote charge disabled!"}, VM_COLOR_YELLOW, 0},
	{13, {(uint8_t *)"请将变速箱挂入 M1/M2 档",        (uint8_t *)"Please shift the gear into M1/M2 gear"}, VM_COLOR_YELLOW, 0},
	
	{14, {(uint8_t *)"NULL",                      (uint8_t *)"NULL"}, 0, 0} 
	
};
static void TableLogicCheck(void)
{
	static ptmrType_t auxbatt_low_Tmr = 0;
	static uint8_t auxbatt_low_flg = 0;
	static ptmrType_t auxbatt_low_diplay_Tmr = 0;
	static uint8_t auxbatt_low_diplay_flg = 0;
	
	if(get_aux_bat_volt() <= 210)
	{
		if(!ptmr_IsStart(&auxbatt_low_Tmr))
			ptmr_Start(&auxbatt_low_Tmr);
		else
		{
			if(ptmr_Get(&auxbatt_low_Tmr) >= 3000)
				auxbatt_low_flg = 1;
			else
				auxbatt_low_flg = 0;
		}
	}
	else
	{
		ptmr_Stop(&auxbatt_low_Tmr);
		auxbatt_low_flg = 0;
	}
	
	if(auxbatt_low_flg)
	{
		if(!ptmr_IsStart(&auxbatt_low_diplay_Tmr))
			ptmr_Start(&auxbatt_low_diplay_Tmr);
		
		if(ptmr_Get(&auxbatt_low_diplay_Tmr) < 5000)
		{
			if(auxbatt_low_diplay_flg == 0)
			{
				auxbatt_low_diplay_flg = 1;
				
			}
		}
		else
		{
			if(auxbatt_low_diplay_flg == 1)
			{
				auxbatt_low_diplay_flg = 0;
			}
		}
	}
	else
	{
		ptmr_Stop(&auxbatt_low_diplay_Tmr);
		auxbatt_low_diplay_flg = 0;
	}
	firstLineTable[ 0].condition = (get_dm1_flt_status(DM1_F4, 521335,  3) == true); //动力电池严重过温,请速安全撤离!
	firstLineTable[ 1].condition = (auxbatt_low_diplay_flg == 1);      //低压电池电压低!
	
	if(0x2d == eol_fadongjiorkaji)
	{
		GeneralUse_t *pFCV_18FFA6F5 = (GeneralUse_t*)can_getPCanBuffer(0x18FFA6F5);
		firstLineTable[ 2].condition = ((pFCV_18FFA6F5->bData[0] == 0x07) || (pFCV_18FFA6F5->bData[0] == 0x0A));  //吹扫进行中
		firstLineTable[ 3].condition = (pFCV_18FFA6F5->bData[0] == 0xFF);  //FCS自检中
		firstLineTable[ 4].condition = ((pFCV_18FFA6F5->bData[0] == 0x00) && (0x01 == can_getPCanRxState(0x18FFA6F5)));  //FCS自检完成
		firstLineTable[ 5].condition = (pFCV_18FFA6F5->bData[0] == 0x01);  //FCS待机 
		firstLineTable[ 6].condition = ((pFCV_18FFA6F5->bData[0] == 0x02) || (pFCV_18FFA6F5->bData[0] == 0x03));  //FCS启动中 
		firstLineTable[ 7].condition = (pFCV_18FFA6F5->bData[0] == 0x04);  //FCS功率输出
		firstLineTable[ 8].condition = (pFCV_18FFA6F5->bData[0] == 0x05);  //FCS维护模式
		firstLineTable[ 9].condition = (pFCV_18FFA6F5->bData[0] == 0x08);  //FCS紧急停机
		firstLineTable[10].condition = (pFCV_18FFA6F5->bData[0] == 0x09);  //FCS故障停机
		firstLineTable[11].condition = ((pFCV_18FFA6F5->bData[0] == 0x06) || (pFCV_18FFA6F5->bData[0] > 0x0A && pFCV_18FFA6F5->bData[0] < 0xFF));  //FCS正常停机
	}
	else
	{
		firstLineTable[ 2].condition = (0);  //吹扫进行中
		firstLineTable[ 3].condition = (0);  //FCS自检中
		firstLineTable[ 4].condition = (0);  //FCS自检完成
		firstLineTable[ 5].condition = (0);  //FCS待机 
		firstLineTable[ 6].condition = (0);  //FCS启动中 
		firstLineTable[ 7].condition = (0);  //FCS功率输出
		firstLineTable[ 8].condition = (0);  //FCS维护模式
		firstLineTable[ 9].condition = (0);  //FCS紧急停机
		firstLineTable[10].condition = (0);  //FCS故障停机
		firstLineTable[11].condition = (0);  //FCS正常停机
	}
	
	firstLineTable[12].condition = (MSD_SwitchStatus); //远程功能禁用开关已打开,请关闭!
	
	static ptmrType_t diplay13_Tmr = 0;
	static uint8_t diplay13_flg = 0;
	GeneralUse_t *pPROP_18FF5527 = (GeneralUse_t*)can_getPCanBuffer(0x18FF5527);
	if(((pPROP_18FF5527->bData[0]&0x18) == 0x08 || (pPROP_18FF5527->bData[0]&0x18) == 0x10)
	&& (get_vehicle_gear_mode() == 1))
	{
		if(!ptmr_IsStart(&diplay13_Tmr))
			ptmr_Start(&diplay13_Tmr);
		
		if(ptmr_Get(&diplay13_Tmr) < 5000)
		{
			diplay13_flg = 1;
		}
		else
		{
			diplay13_flg = 0;
		}
	}
	else 
	{
		ptmr_Stop(&diplay13_Tmr);
		diplay13_flg = 0;
	}
	firstLineTable[13].condition = (diplay13_flg); //请将变速箱挂入 M1/M2 档
	
	firstLineTable[14].condition = (0); //
	
}
static void autoPriorityShowTextWarn(void)
{
	uint8_t  uniStr[128] = {0};
	uint8_t *text = NULL;
	uint32_t fcolor = 0;
	uint8_t  status = 0;
	static uint8_t old_status = 0;
    static uint16_t Xwidth = 0;
	
	TableLogicCheck();
	
	for(uint8_t i = 0; i < TEXT_TABLE_LIST_NUM; i++)
	{
		if(firstLineTable[i].condition == 1)
		{
			status = i + 1;
			text = (uint8_t*)firstLineTable[i].text[eol_language];
			fcolor = firstLineTable[i].fcolor;
			break;
		}
		else
		{
			status = 255;
			text = NULL;
			fcolor = 0;
		}
	}
	
	if(old_status != status || mainview_update)
	{
		old_status = status;
		
		start_draw();
		loc_ClearRect(790 - Xwidth - 1, 204, Xwidth + 2, 32);
		if(text != NULL)
		{
			if(eol_language == 0)
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, fcolor, 0);
			else
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh24, fcolor, 0);
			
			zk_oem2uni(text, uniStr, sizeof(uniStr));
			Xwidth = zk_getTextWidth(text);
			zk_printZH(790 - Xwidth, 204, uniStr);
		}
		end_draw();
		
	}
	
}



#define TEST_WARN_LIST_NUM   (sizeof(TestWarnList) / sizeof(TEXT_WARN_t))
static TEXT_WARN_t  TestWarnList[] = 
{
/* 0*/{ 0, {(uint8_t *)"A25-故障",   (uint8_t *)"A25-故障"}, VM_COLOR_RED},
/* 1*/{ 0, {(uint8_t *)"A24-故障",   (uint8_t *)"A24-故障"}, VM_COLOR_RED},
/* 2*/{ 0, {(uint8_t *)"A23-故障",   (uint8_t *)"A23-故障"}, VM_COLOR_RED},
/* 3*/{ 0, {(uint8_t *)"A22-故障",   (uint8_t *)"A22-故障"}, VM_COLOR_RED},
/* 4*/{ 0, {(uint8_t *)"C2--故障",   (uint8_t *)"C2--故障"}, VM_COLOR_RED},
/* 5*/{ 0, {(uint8_t *)"C9--故障",   (uint8_t *)"C9--故障"}, VM_COLOR_RED},
/* 6*/{ 0, {(uint8_t *)"C13-故障",   (uint8_t *)"C13-故障"}, VM_COLOR_RED},
/* 7*/{ 0, {(uint8_t *)"C25-故障",   (uint8_t *)"C25-故障"}, VM_COLOR_RED},
/* 8*/{ 0, {(uint8_t *)"A19-故障",   (uint8_t *)"A19-故障"}, VM_COLOR_RED},
/* 9*/{ 0, {(uint8_t *)"C3--故障",   (uint8_t *)"C3--故障"}, VM_COLOR_RED},
/*10*/{ 0, {(uint8_t *)"C15-故障",   (uint8_t *)"C15-故障"}, VM_COLOR_RED},
/*11*/{ 0, {(uint8_t *)"C14-故障",   (uint8_t *)"C14-故障"}, VM_COLOR_RED},
/*12*/{ 0, {(uint8_t *)"A10-故障",   (uint8_t *)"A10-故障"}, VM_COLOR_RED},
/*13*/{ 0, {(uint8_t *)"C1--故障",   (uint8_t *)"C1--故障"}, VM_COLOR_RED},
/*14*/{ 0, {(uint8_t *)"C20-故障",   (uint8_t *)"C20-故障"}, VM_COLOR_RED},
/*15*/{ 0, {(uint8_t *)"C23-故障",   (uint8_t *)"C23-故障"}, VM_COLOR_RED},
/*16*/{ 0, {(uint8_t *)"C16-故障",   (uint8_t *)"C16-故障"}, VM_COLOR_RED},
/*17*/{ 0, {(uint8_t *)"C18-故障",   (uint8_t *)"C18-故障"}, VM_COLOR_RED},
/*18*/{ 0, {(uint8_t *)"C21-故障",   (uint8_t *)"C21-故障"}, VM_COLOR_RED},
/*19*/{ 0, {(uint8_t *)"C24-故障",   (uint8_t *)"C24-故障"}, VM_COLOR_RED},
/*20*/{ 0, {(uint8_t *)"C17-故障",   (uint8_t *)"C17-故障"}, VM_COLOR_RED},
/*21*/{ 0, {(uint8_t *)"C19-故障",   (uint8_t *)"C19-故障"}, VM_COLOR_RED},
/*22*/{ 0, {(uint8_t *)"C22-故障",   (uint8_t *)"C22-故障"}, VM_COLOR_RED},
/*23*/{ 0, {(uint8_t *)"C26-故障",   (uint8_t *)"C26-故障"}, VM_COLOR_RED},
/*24*/{ 0, {(uint8_t *)"C10-故障",   (uint8_t *)"C10-故障"}, VM_COLOR_RED},
/*25*/{ 0, {(uint8_t *)"C12-故障",   (uint8_t *)"C12-故障"}, VM_COLOR_RED},
/*26*/{ 0, {(uint8_t *)"C8--故障",   (uint8_t *)"C8--故障"}, VM_COLOR_RED},
/*27*/{ 0, {(uint8_t *)"C11-故障",   (uint8_t *)"C11-故障"}, VM_COLOR_RED},
/*28*/{ 0, {(uint8_t *)"A26-故障",   (uint8_t *)"A26-故障"}, VM_COLOR_RED},
/*29*/{ 0, {(uint8_t *)"A7--故障",   (uint8_t *)"A7--故障"}, VM_COLOR_RED},
/*30*/{ 0, {(uint8_t *)"A8--故障",   (uint8_t *)"A8--故障"}, VM_COLOR_RED},
/*31*/{ 0, {(uint8_t *)"A9--故障",   (uint8_t *)"A9--故障"}, VM_COLOR_RED},

/*32*/{ 0, {(uint8_t *)"B3--故障",   (uint8_t *)"B3--故障"}, VM_COLOR_RED},
/*33*/{ 0, {(uint8_t *)"B4--故障",   (uint8_t *)"B4--故障"}, VM_COLOR_RED},
/*34*/{ 0, {(uint8_t *)"B17-故障",   (uint8_t *)"B17-故障"}, VM_COLOR_RED},
/*35*/{ 0, {(uint8_t *)"B18-故障",   (uint8_t *)"B18-故障"}, VM_COLOR_RED},
/*36*/{ 0, {(uint8_t *)"B20-故障",   (uint8_t *)"B20-故障"}, VM_COLOR_RED},
/*37*/{ 0, {(uint8_t *)"B19-故障",   (uint8_t *)"B19-故障"}, VM_COLOR_RED},
/*38*/{ 0, {(uint8_t *)"蓄电池故障", (uint8_t *)"蓄电池故障"}, VM_COLOR_RED},
/*39*/{ 0, {(uint8_t *)"B24-故障",   (uint8_t *)"B24-故障"}, VM_COLOR_RED},
/*40*/{ 0, {(uint8_t *)"B25-故障",   (uint8_t *)"B25-故障"}, VM_COLOR_RED},
/*41*/{ 0, {(uint8_t *)"B26-故障",   (uint8_t *)"B26-故障"}, VM_COLOR_RED},
/*42*/{ 0, {(uint8_t *)"B27-故障",   (uint8_t *)"B27-故障"}, VM_COLOR_RED},

};
static void TestWarn_condition_check(void)
{
	TestWarnList[0].condition  = (testResultData[0]&0x01); //
	TestWarnList[1].condition  = (testResultData[0]&0x02); //
	TestWarnList[2].condition  = (testResultData[0]&0x04); //
	TestWarnList[3].condition  = (testResultData[0]&0x08); //
	TestWarnList[4].condition  = (testResultData[0]&0x10); //
	TestWarnList[5].condition  = (testResultData[0]&0x20); //
	TestWarnList[6].condition  = (testResultData[0]&0x40); //
	TestWarnList[7].condition  = (testResultData[0]&0x80); //
	TestWarnList[8].condition  = (testResultData[1]&0x01); //
	TestWarnList[9].condition  = (testResultData[1]&0x02); //
	TestWarnList[10].condition = (testResultData[1]&0x04); //
	TestWarnList[11].condition = (testResultData[1]&0x08); //
	TestWarnList[12].condition = (testResultData[1]&0x10); //
	TestWarnList[13].condition = (testResultData[1]&0x20); //
	TestWarnList[14].condition = (testResultData[1]&0x40); //
	TestWarnList[15].condition = (testResultData[1]&0x80); //
	TestWarnList[16].condition = (testResultData[2]&0x01); //
	TestWarnList[17].condition = (testResultData[2]&0x02); //
	TestWarnList[18].condition = (testResultData[2]&0x04); //
	TestWarnList[19].condition = (testResultData[2]&0x08); //
	TestWarnList[20].condition = (testResultData[2]&0x10); //
	TestWarnList[21].condition = (testResultData[2]&0x20); //
	TestWarnList[22].condition = (testResultData[2]&0x40); //
	TestWarnList[23].condition = (testResultData[2]&0x80); //
	TestWarnList[24].condition = (testResultData[3]&0x01); //
	TestWarnList[25].condition = (testResultData[3]&0x02); //
	TestWarnList[26].condition = (testResultData[3]&0x04); //
	TestWarnList[27].condition = (testResultData[3]&0x08); //
	TestWarnList[28].condition = (testResultData[3]&0x10); //
	TestWarnList[29].condition = (testResultData[3]&0x20); //
	TestWarnList[30].condition = (testResultData[3]&0x40); //
	TestWarnList[31].condition = (testResultData[3]&0x80); //
	
	TestWarnList[32].condition = (testResultData[5]&0x01); //B3--故障
	TestWarnList[33].condition = (testResultData[5]&0x02); //B4--故障
	TestWarnList[34].condition = (testResultData[5]&0x04); //B17-故障
	TestWarnList[35].condition = (testResultData[5]&0x08); //B18-故障
	TestWarnList[36].condition = (testResultData[5]&0x10); //B20-故障
	TestWarnList[37].condition = (testResultData[5]&0x20); //B19-故障
	TestWarnList[38].condition = (testResultData[5]&0x40); //蓄电池故障
	TestWarnList[39].condition = (testResultData[5]&0x80); //B24-故障
	TestWarnList[40].condition = (testResultData[6]&0x01); //B25-故障
	TestWarnList[41].condition = (testResultData[6]&0x02); //B26-故障
	TestWarnList[42].condition = (testResultData[6]&0x04); //B27-故障
	
	
}
static void autoScrollShowTestWarn(void)
{
	static ptmrType_t testWarnTmr = 0;
	static uint8_t testWarnFlg = 0;
	static uint8_t testID = 0;
	static uint16_t Xwidth = 0;
	uint8_t uniStr[200];
	
	if((ptmr_Get(&testWarnTmr) > 1500) || (!ptmr_IsStart(&testWarnTmr)))
	{
		TestWarn_condition_check();
		for(uint8_t i = 0, n = testID + 1; i < TEST_WARN_LIST_NUM; i++, n++)
		{
			if (n >= TEST_WARN_LIST_NUM)
			{
				n -= TEST_WARN_LIST_NUM;
			}
			
			if(TestWarnList[n].condition) 
			{
				
				testWarnFlg = 1;
				testID = n;
				
				ptmr_Start(&testWarnTmr);
				
				break;
			}
		}
		
		start_draw();
		
		// LCD_DrawRect(8, 200, 370, 50, TOUMING);
		LCD_DrawRect(8, 204, Xwidth + 4, 32, TOUMING);
		if(testWarnFlg)
		{
			testWarnFlg = 0;
			
			if(eol_language == 0)
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, TestWarnList[testID].fcolor, 0);
				zk_oem2uni(TestWarnList[testID].text[eol_language], uniStr, sizeof(uniStr));
				Xwidth = zk_getTextWidth(TestWarnList[testID].text[eol_language]);
				zk_printZH(10, 204, uniStr);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh24, TestWarnList[testID].fcolor, 0);
				zk_oem2uni(TestWarnList[testID].text[eol_language], uniStr, sizeof(uniStr));
				Xwidth = zk_getTextWidth(TestWarnList[testID].text[eol_language]);
				zk_printZH(10, 204, uniStr);
			}
			
		}
		end_draw();
    }
	
}


void disp_texture_warning(void)
{
	if(Needle_Test)
	{
		autoScrollShowTestWarn();
	}
	else
	{
		autoScrollShowTextWarn();
		autoPriorityShowTextWarn();
	}
    
}





