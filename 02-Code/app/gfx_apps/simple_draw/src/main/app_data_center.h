/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_data_center.h
 * @brief   
 * @version V1.0
 * @author  liuwenlong
 * @date    2020-6-7
 * @note <b>history:</b>
 * 1. create file
 *
 */

#ifndef APP_DATA_CENTER_H
#define	APP_DATA_CENTER_H


typedef enum {
	NotDefine = 0x00,       //0x00-未定义
	ParkingCharge = 0x01,   //0x01-停车充电
	ChargeReserve = 0x02,   //0x02-预留 
	NotCharged = 0x03,      //0x03-未充电  
	ChargingOK = 0x4,       //0x04-充电完成
	ChargingAnomaly = 0xfe, //0xfe-异常 
	ChargingInvalid = 0xff, //0xff-无效
} ChrgSt_e; 



void DataCenter_Init(void);	        //数据中心初始化
void DataCenter_UpdateData(void);   //更新数据

uint16_t get_aux_bat_volt(void);         //蓄电池电压
uint16_t get_front_airpressure(void);    //前桥气压 kPa
uint16_t get_rear_airpressure(void);     //后桥气压 kPa
uint16_t get_parking_airpressure(void);  //驻车气压 kPa


uint32_t get_batt_pack_volt(void);       //电池组电压  比例 0.1
int32_t get_batt_pack_curr(void);        //电池组电流  比例 0.1
uint8_t get_batt_pack_soc(void);         //电池组SOC
int32_t get_batt_pack_inspower(void);    //电池瞬时功率  比例 0.1Kw
uint8_t get_batt_pack_soh(void);         //电池组SOH
int16_t get_batt_pack_avg_tmp(void);     //电池平均温度
uint8_t get_charge_status(void);         //当前充电状态
uint16_t get_remain_chg_time(void);      //剩余充电时间
uint16_t get_low_batt_soc(void);         //低压锂电SOC

int16_t get_mot_cooling_temp(void);      //电机冷却温度 ℃
uint32_t get_mot_volt(void);             //电机电压  比例 0.1
int32_t get_mot_curr(void);              //电机电流  比例 0.1
uint32_t get_mot_power(void);            //电机功率
uint16_t get_motor_speed(void);          //电机转速
uint16_t get_shaft_speed(void);          //输出轴转速
float get_vehicle_speed(void);           //车辆速度
float get_display_speed(void);           //显示车速
uint8_t get_vehicle_gear_mode(void);     //挡位模式
uint8_t get_vehicle_current_gear(void);  //当前挡位
uint8_t get_vehicle_ep_mode(void);       //E/P模式
uint8_t get_vehicle_peristalsis_mode(void); //蠕动模式

uint16_t get_pto_driving_rang(void);     //上装续航里程
uint16_t get_residual_power(void);       //剩余功率
uint16_t get_driving_range(void);        //可行驶里程
uint16_t get_avg_power_consp(void);      //平均电耗
uint32_t get_subtot_power_consp(void);   //小计电耗
uint32_t get_total_power_consp(void);    //累计电耗

uint8_t  get_H2_soc(void);               //氢气SOC
uint32_t get_current_H2_consp(void);     //当前氢耗/小计氢耗
uint32_t get_subtot_H2_consp(void);      //小计氢耗
uint32_t get_total_H2_consp(void);       //累计氢耗
uint32_t get_avg_H2_consp(void);         //平均氢耗
uint32_t get_fcu_work_time(void);        //燃料电池系统工作时长
uint16_t get_max_h2_pressure(void);      //车载氢系统最高压力

#endif	/* SYSTEM_H */



