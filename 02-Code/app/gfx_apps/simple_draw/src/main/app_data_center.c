/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_data_center.c
 * @brief   
 * @version V1.0
 * @author  liuwenlong
 * @date    2022-6-7
 * @note <b>history:</b>
 * 1. create file
 *
 */
#include "r_typedefs.h"
#include "iodefine.h"
#include "string.h"

#include "app_data_center.h"
#include "app_can.h"
#include "uds_support.h"
#include "SYSTEM.h"
#include "adc.h"




/*  车速换算公式：
	实际车速 = （仪表单位时间内采集的脉冲数）/（8*B）
	参数说明：
	8：车速传感器一般为8个脉冲/圈；
	表头系数B：B=(1000*后桥速比)/（2π*R*里程表速比）
	R：轮胎滚动半径

	样车相关参数为：
	里程表速比：1/1；
	后桥主减速比：6.166；
	轮胎型式及规格(驱动轮；前后轮一致时，才包含转向轮)：子午胎-无内胎、295/80R22.5、静力半径487、滚动半径506.8、自由直径1044
	
	8*1000*6.166/(6.28*0.5068*1) = 8*1937.3 = 15499
	15499/3600 = 4.3
	if(freq[1]>3)
		pSpeed_tmp =  freq[1]/4.3; //todo  speed samples pwm input signal
	else
		pSpeed_tmp  = 0;
	
	N传 传动轴转速 [转/分] ，   N轮 车轮转速 [转/分] ，    主减速比 A = N传 /  N轮
	所以，N轮 = N传 /  A
	车轮半径R，车轮转动一周行走距离为 3.14 X 2 X R  [米]
	汽车行走的速度为 3.14 X 2 X R X N传 /  A  [米/分]
	= 3.14 X 2 X 60 X R X N传 / （1000 X A ） [公里/小时]
	= 3.14 X 0.12 X R X N传 / A   [公里/小时]
	= 2π*R*N/A

	B=(1000*后桥速比)/（2π*R*里程表速比）=1000*A/(2π*R)
	2π*R*B = 1000*A
	2π*R/A = 1000/B
	V = 2π*R*60*N/(2π*R*B)
	  = 60N/B



表头系数 = 1000000*后桥速比/(2*3.14*滚动半径*里程速比) （注 ：此处滚动半径单位为毫米）

里程表传感器硬线接仪表时：
输出轴转速=(变速箱输出速比的分子/变速箱输出速比的分母)*(车速传感器当前的频率/车速传感器每转脉冲数)*60
车速=频率*3600/每公里脉冲数(km/h)
每公里脉冲数（即 K 值）=表头系数 *车速传感器每转脉冲数

传感器不接仪表时，车速根据收到的输出轴转速报文计算时，在报文有效范围内：
车速 =（1920*变速箱输出轴转速报文值）/（256*表头系数）km/h
车速 =(变速箱输出轴转速/后桥速比*里程表速比）*2*R*3.14*60(R 单位是 m）= 60*输出轴转速转速/(表头系数*里程表速比）

报文超范围或者超时时，车速为 0
里程=∑车速?3600 (km)

*/

#pragma pack(1)
typedef struct
{
	uint8_t  led_backlight;     //LED背光亮度
	uint16_t aux_bat_volt;      //蓄电池电压
	uint16_t airpressure1;      //前桥气压
	uint16_t airpressure2;      //后桥气压
	uint16_t airpressure3;      //驻车气压
	
	uint32_t batt_pack_volt;    //电池组电压  比例 0.1
	int32_t  batt_pack_curr;    //电池组电流  比例 0.1
	uint8_t  batt_pack_soc;     //电池组SOC
	int32_t  batt_pack_inspower;//电池瞬时功率  比例 0.1Kw
	uint8_t  batt_pack_soh;     //电池组SOH
	int16_t  batt_pack_avg_tmp; //电池平均温度
	uint8_t  charge_status;     //当前充电状态
	uint8_t  charging_indication;    //充电指示原始值 0~3
	uint16_t remain_chg_time;   //剩余充电时间
	
	uint16_t low_batt_soc;      //低压锂电SOC
	
	int16_t  cooling_temp;      //电机冷却温度 ℃
	uint32_t mot_volt;          //电机电压  比例 0.1
	int32_t  mot_curr;          //电机电流  比例 0.1
	uint32_t mot_power;         //电机功率
	uint16_t mot_rpm;           //电机转速
	
	uint16_t shaft_rpm;         //输出轴转速 比例 0.125
	float    veh_speed;         //实际车速
	float    display_speed;     //显示车速
	uint8_t  gear_mode;         //挡位模式
	uint8_t  gear;              //当前挡位
	uint8_t  ep_mode;           //E/P模式
	uint8_t  peristalsis_mode;  //蠕动模式
	
	uint16_t pto_driving_rang;  //上装续航里程  km
	uint16_t residual_power;    //剩余功率
	uint16_t driving_range;     //可行驶里程
	uint16_t avg_power_consp;   //平均电耗
	uint32_t subtot_power_consp;//小计电耗
	uint32_t total_power_consp; //累计电耗
	
	uint8_t  H2_soc;            //氢气SOC
	uint32_t current_H2_consp;  //当前氢耗/小计氢耗
	uint32_t subtot_H2_consp;   //小计氢耗
	uint32_t total_H2_consp;    //累计氢耗
	uint32_t avg_H2_consp;      //平均氢耗
	uint32_t fcu_work_time;     //燃料电池系统工作时长
	uint16_t max_h2_pressure;   //车载氢系统最高压力
	
	
}DATA_CENTER_t;
#pragma pack()

DATA_CENTER_t DataCenter;

void DataCenter_Init(void)	//数据中心初始化
{
	memset(&DataCenter, 0, sizeof(DATA_CENTER_t));
}


void ICUHardwireData_Update(void)      //仪表硬线数据采集
{
	double   db_buffer;
	double   ap_sensor_scale = 0;
	
	//蓄电池电压
	DataCenter.aux_bat_volt = get_input_analog_quantity(BAT_V) / 100;
	
	//前桥气压
	ap_sensor_scale = get_input_analog_quantity(AD_V_B3) * 100 / get_input_analog_quantity(B24_5V_DET);
	if(ap_sensor_scale < 96 && ap_sensor_scale > 10)
		db_buffer = ((double)ap_sensor_scale - 10 ) / 54.422 * 1000; //单位Kpa 
	else
		db_buffer  = 0;
	DataCenter.airpressure1 = db_buffer;
	
	//后桥气压
    ap_sensor_scale = get_input_analog_quantity(AD_V_B20) * 100 / get_input_analog_quantity(B24_5V_DET);
	if(ap_sensor_scale < 96 && ap_sensor_scale > 10)
		db_buffer = ((double)ap_sensor_scale - 10) / 54.422 * 1000; //单位Kpa
    else
		db_buffer  = 0;
	DataCenter.airpressure2 = db_buffer;
	
	//驻车气压
	ap_sensor_scale = get_input_analog_quantity(AD_V_B19) * 100 / get_input_analog_quantity(B26_5V_DET);
	if(ap_sensor_scale < 96 && ap_sensor_scale > 10)
		db_buffer = ((double)ap_sensor_scale - 10) / 54.422 * 1000; //单位Kpa
    else
		db_buffer  = 0;
	DataCenter.airpressure3 = db_buffer;
	
	
}
void BMSandTravelInfoData_Update(void) //BMS与行程信息数据更新
{
	int32_t  s32_buffer;
	uint16_t u16_buffer;
	uint32_t u32_buffer;
	
	VCU_04F02370_t *pVCU_04F02370 = NULL;
	VCU_04F02270_t *pVCU_04F02270 = NULL;
	BMS_0CFFEAF4_t *pBMS_0CFFEAF4 = NULL;
	GeneralUse_t   *pBMS_18FFEEF4 = NULL;
	GeneralUse_t   *pBMS_18FFF2F4 = NULL;
	GeneralUse_t   *bHCU_18FFF631 = NULL;
	GeneralUse_t   *bHCU_18FFF731 = NULL;
	GeneralUse_t   *pFCU_18FFA7F5 = NULL;
	GeneralUse_t   *pBMS_19FF5CF3 = NULL;
	
	pBMS_0CFFEAF4 = (BMS_0CFFEAF4_t*)can_getBCanBuffer(0x0CFFEAF4);
	pBMS_18FFEEF4 = (GeneralUse_t*)can_getBCanBuffer(0x18FFEEF4);
	pBMS_18FFF2F4 = (GeneralUse_t*)can_getBCanBuffer(0x18FFF2F4);
	bHCU_18FFF631 = (GeneralUse_t*)can_getBCanBuffer(0x18FFF631);
	bHCU_18FFF731 = (GeneralUse_t*)can_getBCanBuffer(0x18FFF731);
	pBMS_19FF5CF3 = (GeneralUse_t*)can_getBCanBuffer(0x19FF5CF3);
	pVCU_04F02370 = (VCU_04F02370_t*)can_getBCanBuffer(0x04F02370);
	pVCU_04F02270 = (VCU_04F02270_t*)can_getBCanBuffer(0x04F02270);
	
	//低压锂电SOC
	if(can_getBCanRxState(0x19FF5CF3) == CAN_FRAME_ST_RECVED)
	{
		if(pBMS_19FF5CF3->byte7.byte != 0xff)
		{
			u16_buffer = pBMS_19FF5CF3->byte7.byte * 2 / 5;
			if(u16_buffer > 100)
				u16_buffer = 100;
			DataCenter.low_batt_soc = u16_buffer;
		}
		else DataCenter.low_batt_soc = 0xff;
	}
	else DataCenter.low_batt_soc = 0xff;
	
	//总电压、总电流、SOC
	if(can_getBCanRxState(0x0CFFEAF4) == CAN_FRAME_ST_RECVED)
	{
		if(pBMS_0CFFEAF4->outputvoltage <= 64255)
			DataCenter.batt_pack_volt = pBMS_0CFFEAF4->outputvoltage;
		else
			DataCenter.batt_pack_volt = 0;
		
		if(pBMS_0CFFEAF4->outputcurrent < 0xfffe)
		{
			if(eol_battcoolmd==0)
			{
				s32_buffer = pBMS_0CFFEAF4->outputcurrent - 6000;
				if(s32_buffer > 6000)
					s32_buffer = 6000;
			}
			else if(eol_battcoolmd==1)
			{
				s32_buffer = pBMS_0CFFEAF4->outputcurrent - 10000;
				if(s32_buffer > 10000)
					s32_buffer = 10000;
			}
			else if(eol_battcoolmd==2)
			{
				s32_buffer = pBMS_0CFFEAF4->outputcurrent - 30000;
				if(s32_buffer > 30000)
					s32_buffer = 30000;
			}
			else 
			{
				s32_buffer = 0;
			}
		}
		else
		{
			s32_buffer = 0;
		}
		DataCenter.batt_pack_curr = s32_buffer;
		
		if(pBMS_0CFFEAF4->soc <= 120)
		{
			DataCenter.batt_pack_soc = pBMS_0CFFEAF4->soc;
			if(DataCenter.batt_pack_soc > 100)
				DataCenter.batt_pack_soc = 100;
		}
		else DataCenter.batt_pack_soc = 0xffu;
		
		s32_buffer = (int32_t)DataCenter.batt_pack_volt * DataCenter.batt_pack_curr;
		DataCenter.batt_pack_inspower = s32_buffer / 10000;
		if(eol_fadongjiorkaji == 0x2d)
		{
			int32_t DC_current = 0x00;
			int32_t DC_voltage = 0x00;
			
			if(can_getBCanRxState(0x18FFA7F5) == CAN_FRAME_ST_RECVED)
			{
				pFCU_18FFA7F5 = (GeneralUse_t*)can_getBCanBuffer(0x18FFA7F5);
				
				u16_buffer = pFCU_18FFA7F5->byte1.byte + pFCU_18FFA7F5->byte2.byte * 256;
				
				if(u16_buffer > 0xFAFF)
				{
					DC_current = 0x00;
				}
				else
				{
					DC_current = (int32_t)u16_buffer - 5000;
				}
				
				u16_buffer = pFCU_18FFA7F5->byte3.byte + pFCU_18FFA7F5->byte4.byte * 256;
				if(u16_buffer > 0xFAFF)
				{
					DC_voltage = 0x00;
				}
				else
				{
					DC_voltage = (int32_t)u16_buffer;
				}
				s32_buffer = DC_voltage * DC_current / 10000;
				DataCenter.batt_pack_inspower = DataCenter.batt_pack_inspower + s32_buffer;
			}
		}
	}
	else
	{
		// DataCenter.batt_pack_volt = 0;
		// DataCenter.batt_pack_curr = 0;
		// DataCenter.batt_pack_soc = 0;
		DataCenter.batt_pack_volt = 0xffffu;
		DataCenter.batt_pack_curr = 0xffffu;
		DataCenter.batt_pack_soc = 0xffu;
		DataCenter.batt_pack_inspower = 0xffffu;
		
	}
	if(can_getBCanRxState(0x04F02370) == CAN_FRAME_ST_RECVED)//LYX
{
    if(pVCU_04F02370->batt_soc <= 250)
    {
        DataCenter.batt_pack_soc = pVCU_04F02370->batt_soc * 2 / 5;
        if(DataCenter.batt_pack_soc > 100)
            DataCenter.batt_pack_soc = 100;
    }
    else DataCenter.batt_pack_soc = 0xffu;

    DataCenter.batt_pack_curr = (int32_t)pVCU_04F02370->pack_current - 10000;

    if(pVCU_04F02370->pack_voltage <= 10000)
        DataCenter.batt_pack_volt = pVCU_04F02370->pack_voltage;
    else
        DataCenter.batt_pack_volt = 0;

    s32_buffer = (int32_t)DataCenter.batt_pack_volt * DataCenter.batt_pack_curr;
    DataCenter.batt_pack_inspower = s32_buffer / 10000;
}
	//电池组SOH，电池平均温度
	if(can_getBCanRxState(0x18FFEEF4) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.batt_pack_soh = pBMS_18FFEEF4->byte2.byte;
		if(DataCenter.batt_pack_soh > 100) DataCenter.batt_pack_soh = 100;
		DataCenter.batt_pack_avg_tmp = pBMS_18FFEEF4->byte3.byte - 40;
	}
	else
	{
		DataCenter.batt_pack_soh = 0xffu;
		DataCenter.batt_pack_avg_tmp = 0xffu;
	}
	
	/* 当前充电状态
	0x01-停车充电 
	0x02-预留 
	0x03-未充电 
	0x04-充电完成 
	0xfe-异常 
	0xff-无效
	*/
	//DataCenter.charge_status = pBMS_18FFF2F4->byte1.byte;
	if(can_getBCanRxState(0x04F02370) == CAN_FRAME_ST_RECVED)
{
	DataCenter.charging_indication = pVCU_04F02370->charging_indication;
    if(pVCU_04F02370->charging_indication == 1)
        DataCenter.charge_status = ParkingCharge;
    else if(pVCU_04F02370->charging_indication == 2)
        DataCenter.charge_status = ChargingOK;
    else if(pVCU_04F02370->charging_indication == 3)
        DataCenter.charge_status = ChargingAnomaly;
    else
        DataCenter.charge_status = NotCharged;
	}
	else
	{
		DataCenter.charging_indication = 0xff;
		DataCenter.charge_status = pBMS_18FFF2F4->byte1.byte;
	}
	
	
	//剩余充电时间，小计电耗
	if(can_getBCanRxState(0x18FFF731) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.remain_chg_time = bHCU_18FFF731->byte5.byte + bHCU_18FFF731->byte6.byte * 256;
		
		u32_buffer  = (uint32_t)bHCU_18FFF731->byte1.byte;
		u32_buffer += (uint32_t)bHCU_18FFF731->byte2.byte << 8;
		u32_buffer += (uint32_t)bHCU_18FFF731->byte3.byte << 16;
		u32_buffer += (uint32_t)bHCU_18FFF731->byte4.byte << 24;
		if(u32_buffer > 9999 && u32_buffer != 0xffffffffu)
			u32_buffer = 9999;
		DataCenter.subtot_power_consp = u32_buffer;
	}
	else
	{
		DataCenter.remain_chg_time = 0xffffu; //无效值
		DataCenter.subtot_power_consp = 0xffffffffu; //无效值
	}
	
	//剩余功率，可行驶里程，平均电耗，累计电耗
	if(can_getBCanRxState(0x18FFF631) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.residual_power  = bHCU_18FFF631->byte1.byte * 2;
		DataCenter.driving_range   = bHCU_18FFF631->byte3.byte * 10;
		DataCenter.avg_power_consp = bHCU_18FFF631->byte8.byte * 2;
		
		u32_buffer  = (uint32_t)bHCU_18FFF631->byte4.byte;
		u32_buffer += (uint32_t)bHCU_18FFF631->byte5.byte << 8;
		u32_buffer += (uint32_t)bHCU_18FFF631->byte6.byte << 16;
		u32_buffer += (uint32_t)bHCU_18FFF631->byte7.byte << 24;
		if(u32_buffer > 4294967296)
			u32_buffer = 0;
		DataCenter.total_power_consp = u32_buffer;
	}
	else
	{
		DataCenter.residual_power  = 0xffffu; //无效值
		DataCenter.driving_range   = 0xffffu; //无效值
		DataCenter.avg_power_consp = 0xffffu; //无效值
		DataCenter.total_power_consp = 0xffffffffu; //无效值
	}
	if(can_getBCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.aux_bat_volt =
			(uint16_t)pVCU_04F02270->lv_batt_voltage * 2;

		DataCenter.driving_range =
			pVCU_04F02270->remaining_driving_range;
	}
	//上装续航里程  km
	if(can_getBCanRxState(0x18FFF931) == CAN_FRAME_ST_RECVED)
	{
		GeneralUse_t   *bVTD5_18FFF931 = NULL;
		bVTD5_18FFF931 = (GeneralUse_t*)can_getBCanBuffer(0x18FFF931);
		
		if(bVTD5_18FFF931->byte6.bit12 == 1)
			DataCenter.pto_driving_rang   = bVTD5_18FFF931->byte5.byte * 10;
		else
			DataCenter.pto_driving_rang   = 0xffffu;
	}
	else
	{
		DataCenter.pto_driving_rang   = 0xffffu; //无效值
	}
	
}
void DrvSysInfoData_Update(void)       //驱动系统信息数据更新
{
	uint8_t  u8_buffer;
	uint16_t u16_buffer;
	int16_t  s16_buffer;
	int32_t  s32_buffer;
	double   db_buffer;
	
	uint32_t mot_power1 = 0, mot_power2 = 0;
	uint8_t mot_off1 = 0, mot_off2 = 0;
	
	MCU_0CFF11EF_t *pMCU_0CFF11EF  = (MCU_0CFF11EF_t*)can_getBCanBuffer(0x0CFF11EF);
	MCU_0CFF11EF_t *pMCU2_0CFF11F0 = NULL; //
	GeneralUse_t   *pETC1_0CF00203 = NULL; //(GeneralUse_t*)can_getBCanBuffer(0x0CF00203);
	GeneralUse_t   *pETC2_18F00503 = NULL; //(GeneralUse_t*)can_getBCanBuffer(0x18F00503);
	GeneralUse_t   *pETC7_18FE4A03 = (GeneralUse_t*)can_getBCanBuffer(0x18FE4A03);
	GeneralUse_t   *pTC1_0C010305  = (GeneralUse_t*)can_getBCanBuffer(0x0C010305);
	GeneralUse_t   *pTCU_0CEF3103  = NULL; //(GeneralUse_t*)can_getBCanBuffer(0x0CEF3103);
	VCU_18FFF531_t *pHCU_18FFF531  = NULL; //(VCU_18FFF531_t*)can_getBCanBuffer(0x18FFF531);
	GeneralUse_t   *bATS_0C3DD7A7  = NULL; //(GeneralUse_t*)can_getBCanBuffer(0x0C3DD7A7);
	GeneralUse_t   *pPROP_18FF5527 = (GeneralUse_t*)can_getBCanBuffer(0x18FF5527);
	VCU_04F02270_t *pVCU_04F02270 = (VCU_04F02270_t*)can_getBCanBuffer(0x04F02270);
	VCU_04F02A70_t *pVCU_04F02A70 = (VCU_04F02A70_t*)can_getBCanBuffer(0x04F02A70);
	VCU_04F02B70_t *pVCU_04F02B70 = (VCU_04F02B70_t*)can_getBCanBuffer(0x04F02B70);
	//电机冷却温度指示
	if(can_getBCanRxState(0x0C3DD7A7) == CAN_FRAME_ST_RECVED)
	{
		bATS_0C3DD7A7  = (GeneralUse_t*)can_getBCanBuffer(0x0C3DD7A7);
		DataCenter.cooling_temp = bATS_0C3DD7A7->byte5.byte - 40;
	}
	else if(can_getBCanRxState(0x0CF19682) == CAN_FRAME_ST_RECVED)
	{
		GeneralUse_t   *pTMS_0CF19682  = (GeneralUse_t*)can_getBCanBuffer(0x0CF19682);
		DataCenter.cooling_temp = pTMS_0CF19682->byte1.byte - 40;
	}
	else DataCenter.cooling_temp = 0;
	
	// 电机电压/电机电流/电机功率/电机转速
	if(can_getBCanRxState(0x0CFF11EF) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.mot_volt = pMCU_0CFF11EF->linevoltage;
		
		if(268U == atmModelSpdRatio) //if((268U == atmModelSpdRatio) || (175U == atmModelSpdRatio))
			s32_buffer = pMCU_0CFF11EF->linecurrent - 6000;
		else
			s32_buffer = pMCU_0CFF11EF->linecurrent - 10000;
		DataCenter.mot_curr = s32_buffer;
		
		DataCenter.mot_rpm =  pMCU_0CFF11EF->rpm * 0.25;
		mot_power1 = pMCU_0CFF11EF->rpm * pMCU_0CFF11EF->torch /9550/4;
		
	}
	else
	{
		// DataCenter.mot_rpm  = 0;
		DataCenter.mot_volt = 0xffffu;
		DataCenter.mot_curr = 0xffffu;
		DataCenter.mot_rpm  = 0xffffu;
		mot_power1 = 0;
		mot_off1 = 1;
	}
	if(can_getBCanRxState(0x0CFF11F0) == CAN_FRAME_ST_RECVED)
	{
		pMCU2_0CFF11F0 = (MCU_0CFF11EF_t*)can_getBCanBuffer(0x0CFF11F0);
		mot_power2 = pMCU2_0CFF11F0->rpm * pMCU2_0CFF11F0->torch /9550/4;
	}
	else
	{
		mot_power2 = 0;
		mot_off2 = 1;
	}
	if(mot_off1 == 1 && mot_off2 == 1)
		DataCenter.mot_power = 0xffffffffu;
	else
		DataCenter.mot_power = mot_power1 + mot_power2;
	
	//输出轴转速，车速，档位，挡位模式
	if(eol_atmtype != 33U && eol_atmtype != 34U)
	{
		if(can_getBCanRxState(0x0CF00203) == CAN_FRAME_ST_RECVED)
		{
			pETC1_0CF00203 = (GeneralUse_t*)can_getBCanBuffer(0x0CF00203);
			u16_buffer = (pETC1_0CF00203->byte2.byte +pETC1_0CF00203->byte3.byte*256);
		}
		else if(can_getBCanRxState(0x0CEF3103) == CAN_FRAME_ST_RECVED)
		{
			pTCU_0CEF3103 = (GeneralUse_t*)can_getBCanBuffer(0x0CEF3103);
			u16_buffer = (pTCU_0CEF3103->byte7.byte +pTCU_0CEF3103->byte8.byte*256);
		}
		else u16_buffer = 0;
		if(u16_buffer <= 64255)
			DataCenter.shaft_rpm = u16_buffer;
		else
			DataCenter.shaft_rpm = 0;
		
		db_buffer = 60 * DataCenter.shaft_rpm * 0.125 * eol_biaosuxiangsubifenmu / B_value / eol_biaosuxiangsubifenzi;
		if(eol_atmtype == 44U) //博世两档电驱桥
		{
			db_buffer = db_buffer / 3.61;
		}
		DataCenter.veh_speed = (float)(db_buffer);
		if(eol_speed_enlarge_factor > 0)
			DataCenter.display_speed = (float)(DataCenter.veh_speed * 1.07);
		else 
			DataCenter.display_speed = (float)(DataCenter.veh_speed);
		
		if(can_getBCanRxState(0x18F00503) == CAN_FRAME_ST_RECVED)
		{
			pETC2_18F00503 = (GeneralUse_t*)can_getBCanBuffer(0x18F00503);
			s16_buffer = pETC2_18F00503->byte4.byte - 125;
			if(s16_buffer == 0)
				DataCenter.gear = 0; //N挡
			else if((s16_buffer > 0) && (s16_buffer < 100))
				DataCenter.gear = s16_buffer; //D1~D99挡
			else if(s16_buffer == -1)
				DataCenter.gear = 100; //R挡
			else
				DataCenter.gear = 0xffu; //无效
			
			if(can_getBCanRxState(0x18FE4A03) == CAN_FRAME_ST_RECVED)
			{
				if(pETC7_18FE4A03->byte3.bit78 == 1)
					DataCenter.gear_mode = 0; //M模式
				else if(pETC7_18FE4A03->byte3.bit78 == 0)
					DataCenter.gear_mode = 1; //A模式
				else
					DataCenter.gear_mode = 0xffu; //无效
				
				// if(pETC7_18FE4A03->byte3.bit56 == 0)
					// DataCenter.ep_mode = 0; //E 经济模式
				// else if(pETC7_18FE4A03->byte3.bit56 == 1)
					// DataCenter.ep_mode = 1; //P 动力模式
				// else
					DataCenter.ep_mode = 0xffu; //无效
				
				// if(pETC7_18FE4A03->byte3.bit34 == 0)
					// DataCenter.peristalsis_mode = 0; //蠕动未激活
				// else if(pETC7_18FE4A03->byte3.bit34 == 1)
					// DataCenter.peristalsis_mode = 1; //蠕动激活
				// else
					DataCenter.peristalsis_mode = 0xffu; //无效
			}
			else
			{
				DataCenter.gear_mode = 0xffu; //无效
				
				// DataCenter.ep_mode = 0xffu; //无效
				// DataCenter.peristalsis_mode = 0xffu; //无效
			}
			
		}
		else if(can_getBCanRxState(0x0CEF3103) == CAN_FRAME_ST_RECVED)
		{
			s16_buffer = pTCU_0CEF3103->byte1.byte - 125;
			if(s16_buffer == 0)
				DataCenter.gear = 0; //N挡
			else if((s16_buffer > 0) && (s16_buffer < 7))
				DataCenter.gear = s16_buffer; //D1~D6挡
			else if(s16_buffer == 7)
				DataCenter.gear = 100; //R挡
			else
				DataCenter.gear = 0xffu; //无效
			
			if(pTCU_0CEF3103->byte4.bit12 == 0)
				DataCenter.gear_mode = 0;//M模式
			else if(pTCU_0CEF3103->byte4.bit12 == 1)
				DataCenter.gear_mode = 1;//A模式
			else
				DataCenter.gear_mode = 0xffu; //无效
			
		}
		else
		{
			DataCenter.gear = 0xffu; //无效
			
			DataCenter.gear_mode = 0xffu; //无效
		}
	}
	else
	{
		if(can_getBCanRxState(0x0CFF11EF) == CAN_FRAME_ST_RECVED)
		{
			if(eol_atmtype == 34U)
				DataCenter.shaft_rpm = pMCU_0CFF11EF->rpm * 2 / 6.243;  //比例0.125
			else
				DataCenter.shaft_rpm = pMCU_0CFF11EF->rpm * 2;   //比例0.125
		}
		else DataCenter.shaft_rpm = 0;
		
		db_buffer = 60 * DataCenter.shaft_rpm * 0.125 * eol_biaosuxiangsubifenmu / B_value / eol_biaosuxiangsubifenzi;
		DataCenter.veh_speed = (float)(db_buffer);
		if(eol_speed_enlarge_factor > 0)
			DataCenter.display_speed = (float)(db_buffer * 1.07);
		else 
			DataCenter.display_speed = (float)(db_buffer);
		
		if(can_getBCanRxState(0x18FFF531) == CAN_FRAME_ST_RECVED)
		{
			pHCU_18FFF531  = (VCU_18FFF531_t*)can_getBCanBuffer(0x18FFF531);
			u8_buffer = pHCU_18FFF531->gear;
			if(u8_buffer == 1)
				DataCenter.gear = 0; //N挡
			else if(u8_buffer == 0)
				DataCenter.gear = 1; //D挡
			else if(u8_buffer == 2)
				DataCenter.gear = 100; //R挡
			else
				DataCenter.gear = 0xffu; //无效
		}
		else
		{
			DataCenter.gear = 0xffu; //无效
		}
		
		DataCenter.gear_mode = 0xffu; //无效
	}
	if(can_getBCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)
	{
		switch(pVCU_04F02270->current_gear)
		{
			case 0:
				DataCenter.gear = 0;       //N挡
				break;

			case 1:
				DataCenter.gear = 100;     //R挡
				break;

			case 2:
				DataCenter.gear = 0xfeu;   //D挡，不带数字
				break;

			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				DataCenter.gear =
					pVCU_04F02270->current_gear - 2; //D1~D7
				break;

			default:
				DataCenter.gear = 0xffu;   //无效
				break;
		}

		DataCenter.gear_mode = 0xffu;       //无M/A模式
	}
	if(can_getBCanRxState(0x04F02370) == CAN_FRAME_ST_RECVED)//lyx
	{
		VCU_04F02370_t *pVCU_04F02370 = NULL;
		pVCU_04F02370 = (VCU_04F02370_t*)can_getBCanBuffer(0x04F02370);

		if(pVCU_04F02370->abs_vehicle_speed <= 2500)
		{
			DataCenter.veh_speed = (float)pVCU_04F02370->abs_vehicle_speed / 10;
			if(eol_speed_enlarge_factor > 0)
				DataCenter.display_speed = (float)(DataCenter.veh_speed * 1.07);
			else
				DataCenter.display_speed = DataCenter.veh_speed;
		}
	}
	if(can_getBCanRxState(0x04F02A70) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.airpressure1 = (uint16_t)pVCU_04F02A70->brk_air_pressure1 * 10U;
	}
	else
	{
		DataCenter.airpressure1 = 0U;
	}

	if(can_getBCanRxState(0x04F02B70) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.airpressure2 = (uint16_t)pVCU_04F02B70->brk_air_pressure2 * 10U;
	}
	else
	{
		DataCenter.airpressure2 = 0U;
	}
	//E/P模式
	if(eol_atmtype == 33U || eol_atmtype == 34U || eol_atmtype == 27U || eol_atmtype == 30U)
	{
		if(can_getBCanRxState(0x0C010305) == CAN_FRAME_ST_RECVED)
		{
			if(pTC1_0C010305->byte6.bit34 == 0)
				DataCenter.ep_mode = 0; //E 经济模式
			else if(pTC1_0C010305->byte6.bit34 == 1)
				DataCenter.ep_mode = 1; //P 动力模式
			else
				DataCenter.ep_mode = 0xffu; //无效
		}
		else if(can_getBCanRxState(0x18FE4A03) == CAN_FRAME_ST_RECVED)
		{
			if(pETC7_18FE4A03->byte3.bit56 == 0)
				DataCenter.ep_mode = 0; //E 经济模式
			else if(pETC7_18FE4A03->byte3.bit56 == 1)
				DataCenter.ep_mode = 1; //P 动力模式
			else
				DataCenter.ep_mode = 0xffu; //无效
		}
		else
		{
			DataCenter.ep_mode = 0xffu; //无效
		}
		
	}
	else
	{
		if(can_getBCanRxState(0x18FE4A03) == CAN_FRAME_ST_RECVED)
		{
			if(pETC7_18FE4A03->byte3.bit56 == 0)
				DataCenter.ep_mode = 0; //E 经济模式
			else if(pETC7_18FE4A03->byte3.bit56 == 1)
				DataCenter.ep_mode = 1; //P 动力模式
			else
				DataCenter.ep_mode = 0xffu; //无效
		}
		else
		{
			DataCenter.ep_mode = 0xffu; //无效
		}
	}
	
	// 蠕动模式
	// if(eol_atmtype != 35U)
	// {
		// if(can_getBCanRxState(0x18FE4A03) == CAN_FRAME_ST_RECVED
		// || can_getBCanRxState(0x0C010305) == CAN_FRAME_ST_RECVED)
		// {
			// if((pETC7_18FE4A03->byte3.bit34 == 0) && (pTC1_0C010305->byte6.bit56 == 0))
				// DataCenter.peristalsis_mode = 0; //蠕动未激活
			// else if((pETC7_18FE4A03->byte3.bit34 == 1) || (pTC1_0C010305->byte6.bit56 == 1))
				// DataCenter.peristalsis_mode = 1; //蠕动激活
			// else
				// DataCenter.peristalsis_mode = 0xffu; //无效
		// }
		// else
		// {
			// DataCenter.peristalsis_mode = 0xffu; //无效
		// }
	// }
	// else
	{
		if(can_getBCanRxState(0x18FE4A03) == CAN_FRAME_ST_RECVED
		|| can_getBCanRxState(0x18FF5527) == CAN_FRAME_ST_RECVED)
		{
			if((pETC7_18FE4A03->byte3.bit34 == 0) && (pPROP_18FF5527->byte1.bit2 == 0))
				DataCenter.peristalsis_mode = 0; //蠕动未激活
			else if((pETC7_18FE4A03->byte3.bit34 == 1) || (pPROP_18FF5527->byte1.bit2 == 1))
				DataCenter.peristalsis_mode = 1; //蠕动激活
			else
				DataCenter.peristalsis_mode = 0xffu; //无效
		}
		else
		{
			DataCenter.peristalsis_mode = 0xffu; //无效
		}
	}
	
}
void HydrogenSysInfoData_Update(void)   //氢气系统信息数据更新
{
	uint32_t u32_buffer;
	
	GeneralUse_t *pHMS_0x18FFA1F6 = NULL;
	GeneralUse_t *pHMS_0x18FFABF6 = NULL;
	GeneralUse_t *pHMS_0x18FFA2F6 = NULL;
	GeneralUse_t *pFCU_0x18FFAAF5 = NULL;
	
    pHMS_0x18FFA1F6 = (GeneralUse_t*)can_getBCanBuffer(0x18FFA1F6);
	pHMS_0x18FFABF6 = (GeneralUse_t*)can_getBCanBuffer(0x18FFABF6);
	pHMS_0x18FFA2F6 = (GeneralUse_t*)can_getBCanBuffer(0x18FFA2F6);
	pFCU_0x18FFAAF5 = (GeneralUse_t*)can_getBCanBuffer(0x18FFAAF5);
	
	if(can_getBCanRxState(0x18FFA1F6) == CAN_FRAME_ST_RECVED)
	{
		if(pHMS_0x18FFA1F6->byte2.byte <= 150)
		{
			DataCenter.H2_soc = pHMS_0x18FFA1F6->byte2.byte;
			if(DataCenter.H2_soc > 100)
				DataCenter.H2_soc = 100;
		}
		else DataCenter.H2_soc = 0xffu;
	}
	else DataCenter.H2_soc = 0xffu;
	
	if(can_getBCanRxState(0x18FFABF6) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.current_H2_consp = (pHMS_0x18FFABF6->bData[4] + pHMS_0x18FFABF6->bData[5] * 256) * 10; //放大10倍
		
		if(Miles.single_miles > 0)
		{
			u32_buffer = Miles.single_H2_consp * 1000 / Miles.single_miles; //放大10倍
			if(u32_buffer > 9999)
				u32_buffer = 9999;
			DataCenter.avg_H2_consp = u32_buffer;
		}
		else DataCenter.avg_H2_consp = 0;
		
	}
	else
	{
		DataCenter.current_H2_consp = 0;
		DataCenter.avg_H2_consp = 0xffffffffu;
	}
	
	DataCenter.subtot_H2_consp = Miles.single_H2_consp;
	
	if(can_getBCanRxState(0x18FFAAF5) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.total_H2_consp = pFCU_0x18FFAAF5->bData[3] + pFCU_0x18FFAAF5->bData[4]  * 256;
		
		u32_buffer  = (uint32_t)pFCU_0x18FFAAF5->bData[0];
		u32_buffer += (uint32_t)pFCU_0x18FFAAF5->bData[1] * 256;
		u32_buffer += (uint32_t)pFCU_0x18FFAAF5->bData[2] * 65536;
		DataCenter.fcu_work_time = u32_buffer;
	}
	else
	{
		DataCenter.total_H2_consp = 0xffffu;
		DataCenter.fcu_work_time = 0xffffffffu;
	}
	
	if(can_getBCanRxState(0x18FFA2F6) == CAN_FRAME_ST_RECVED)
	{
		DataCenter.max_h2_pressure = pHMS_0x18FFA2F6->bData[0] + pHMS_0x18FFA2F6->bData[1] * 256;
	}
	else
	{
		DataCenter.max_h2_pressure = 0xffffu;
	}
	
}


void DataCenter_UpdateData(void)   //更新数据
{
	ICUHardwireData_Update();
	BMSandTravelInfoData_Update();
	DrvSysInfoData_Update();
	if(0x2d == eol_fadongjiorkaji)
		HydrogenSysInfoData_Update();
}


uint16_t get_aux_bat_volt(void)         //蓄电池电压
{
	return DataCenter.aux_bat_volt;
}
uint16_t get_front_airpressure(void)    //前桥气压 kPa
{
	return DataCenter.airpressure1;
}
uint16_t get_rear_airpressure(void)     //后桥气压 kPa
{
	return DataCenter.airpressure2;
}
uint16_t get_parking_airpressure(void)  //驻车气压 kPa
{
	return DataCenter.airpressure3;
}


uint32_t get_batt_pack_volt(void)       //电池组电压  比例 0.1
{
	return DataCenter.batt_pack_volt;
}
int32_t get_batt_pack_curr(void)        //电池组电流  比例 0.1
{
	return DataCenter.batt_pack_curr;
}
uint8_t get_batt_pack_soc(void)         //电池组SOC
{
	return DataCenter.batt_pack_soc;
}
int32_t get_batt_pack_inspower(void)    //电池瞬时功率  比例 0.1Kw
{
	return DataCenter.batt_pack_inspower;
}
uint8_t get_batt_pack_soh(void)         //电池组SOH
{
	return DataCenter.batt_pack_soh;
}
int16_t get_batt_pack_avg_tmp(void)     //电池平均温度
{
	return DataCenter.batt_pack_avg_tmp;
}
uint8_t get_charge_status(void)         //当前充电状态
{
	return DataCenter.charge_status;
}
uint16_t get_remain_chg_time(void)      //剩余充电时间
{
	return DataCenter.remain_chg_time;
}
uint16_t get_low_batt_soc(void)         //低压锂电SOC
{
	return DataCenter.low_batt_soc;
}
uint8_t get_charging_indication(void)	//充电指示状态
{
    return DataCenter.charging_indication;
}

int16_t get_mot_cooling_temp(void)      //电机冷却温度 ℃
{
	return DataCenter.cooling_temp;
}
uint32_t get_mot_volt(void)             //电机电压  比例 0.1
{
	return DataCenter.mot_volt;
}
int32_t get_mot_curr(void)              //电机电流  比例 0.1
{
	return DataCenter.mot_curr;
}
uint32_t get_mot_power(void)            //电机功率
{
	return DataCenter.mot_power;
}
uint16_t get_motor_speed(void)          //电机转速
{
	return DataCenter.mot_rpm;
}
uint16_t get_shaft_speed(void)          //输出轴转速
{
	return DataCenter.shaft_rpm;
}
float get_vehicle_speed(void)           //车辆速度
{
	return DataCenter.veh_speed;
}
float get_display_speed(void)           //显示车速
{
	return DataCenter.display_speed;
}
uint8_t get_vehicle_gear_mode(void)     //挡位模式
{
	return DataCenter.gear_mode;
}
uint8_t get_vehicle_current_gear(void)  //当前挡位
{
	return DataCenter.gear;
}
uint8_t get_vehicle_ep_mode(void)       //E/P模式
{
	return DataCenter.ep_mode;
}
uint8_t get_vehicle_peristalsis_mode(void) //蠕动模式
{
	return DataCenter.peristalsis_mode;
}

uint16_t get_pto_driving_rang(void)     //上装续航里程
{
	return DataCenter.pto_driving_rang;
}
uint16_t get_residual_power(void)       //剩余功率
{
	return DataCenter.residual_power;
}
uint16_t get_driving_range(void)        //可行驶里程
{
	return DataCenter.driving_range;
}
uint16_t get_avg_power_consp(void)      //平均电耗
{
	return DataCenter.avg_power_consp;
}
uint32_t get_subtot_power_consp(void)   //小计电耗
{
	return DataCenter.subtot_power_consp;
}
uint32_t get_total_power_consp(void)    //累计电耗
{
	return DataCenter.total_power_consp;
}


uint8_t  get_H2_soc(void)               //氢气SOC
{
	return DataCenter.H2_soc;
}
uint32_t get_current_H2_consp(void)     //当前氢耗/小计氢耗
{
	return DataCenter.current_H2_consp;
}
uint32_t get_subtot_H2_consp(void)      //小计氢耗
{
	return DataCenter.subtot_H2_consp;
}
uint32_t get_total_H2_consp(void)       //累计氢耗
{
	return DataCenter.total_H2_consp;
}
uint32_t get_avg_H2_consp(void)         //平均氢耗
{
	return DataCenter.avg_H2_consp;
}
uint32_t get_fcu_work_time(void)        //燃料电池系统工作时长
{
	return DataCenter.fcu_work_time;
}
uint16_t get_max_h2_pressure(void)      //车载氢系统最高压力
{
	return DataCenter.max_h2_pressure;
}





