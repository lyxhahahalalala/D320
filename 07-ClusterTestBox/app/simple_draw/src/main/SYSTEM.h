/* 
 * File:   SYSTEM.h
 * Author: mpszm
 *
 * Created on 2016年8月1日, 下午2:50
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#include <iodefine.h>
#include "bsp.h"
//系统EEPROM配置字
//总里程配置
#define  TOTAL_MILES   0L
//单次里程配置
#define  SINGLE_MILES  0

#define SPEEDRATIO 1
//背光亮度值
#define  BACKLIGHT     400
//定义模块地址 0x01 为前部模块 0x02 为后模块  0x03为顶部模块   0x04为仪表
#define  MT_ADDR      4
//程序版本号
#define VERSIONS_MAJOR	(1u)
#define VERSIONS_MINOR	(15u)=
#define CAN_BL_App	    (0x1u)
#define  TIME            181201
#define  VERISON         115

enum{
	JINGDIAN = 0x01,
	CHUANDAO,
	NAIJIU,
	DIWEN,
	Liangchanceshi,
	CHUCE,//功能初测模式



};
typedef struct _TASK_COMPONENTS
{
    void (*TaskHook)(void);            // 要运行的任务函数
    unsigned long ItvTime;             // 任务运行间隔时间
    unsigned char Run;                 // 程序运行标记：0-不运行，1运行
    unsigned long Timer;               // 计时器      
} TASK_COMPONENTS;                     // 任务定义

extern TASK_COMPONENTS TaskComps[];

typedef union {

    struct {
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
        unsigned bit8 : 1;
    };
    unsigned char byte;
} DATA_BIT;

typedef union {
    unsigned char byte[10];
    struct {
	 unsigned C1_GEARBOX_QULI_CTRL : 	2;
	 unsigned C2_REAR_LIGHT_CTRL :	2;
	 unsigned C3_RESERVE_CTRL :	2;
	 unsigned C4_OK_CTRL :	2;
	 unsigned C5_CANCEL_CTRL :	2;
	 unsigned C6_UP_CTRL :	2;
	 unsigned C7_DOWN_CTRL :	2;
	 unsigned C8_HI_LEVEL_CTRL :	2;
	 unsigned C9_INTAKE_PREHEAT_CTRL :	2;
	 unsigned C10_TRAILER_ABS_CTRL :	2;
	 unsigned C11_AXLE_DIFF_CTRL :	2;
	 unsigned C12_WHEEL_DIFF_CTRL :	2;
	 unsigned C13_VBAT_SW_CTRL :	2;
	 unsigned C14_Emergency_shutdown_CTRL :	2;
	 unsigned C15_QULI_INDICTOR_CTRL :	2;
	 unsigned C16_BRAKE_SW_L1_CTRL :	2;
	 unsigned C17_BRAKE_SW_L2_CTRL :	2;
	 unsigned C18_BRAKE_SW_L3_CTRL :	2;
	 unsigned C19_BRAKE_SW_L4_CTRL :	2;
	 unsigned C20_BRAKE_SW_R1_CTRL :	2;
	 unsigned C21_BRAKE_SW_R2_CTRL :	2;
	 unsigned C22_BRAKE_SW_R3_CTRL :	2;
	 unsigned C23_BRAKE_SW_R4_CTRL :	2;
	 unsigned C24_PARK_CTRL :	2;
	 unsigned C25_BCM_CTRL :	2;
	 unsigned C26_PASSAGER_SAFE_L_CTRL :	2;
	 unsigned A7_AIR_FILTER_CTRL :	2;
	 unsigned A8_DRIVER_DOOR_CTRL :	2;
	 unsigned A9_SAFETY_BELT_CTRL :	2;
	 unsigned A10_LOW_GEAR_CTRL :	2;
	 unsigned A22_DRYER_CTRL :	2;
	 unsigned A23_AIR_SUSPENSION_CTRL :	2;
	 unsigned A24_FLOATING_BRIDGE_CTRL :	2;
	 unsigned A25_RESERVE_CTRL :	2;
	 unsigned A26_RECHARGE_CTRL :	2;
	 unsigned IN_36 :	2;
	 unsigned IN_37 :	2;
	 unsigned IN_38 :	2;
	 unsigned IN_39 :	2;
	 unsigned IN_40 :	2;



    };
    
} IO_BIT;

typedef union {
    unsigned char byte[5];
    struct {
	 unsigned C1_GEARBOX_QULI_CTRL : 	1;
	 unsigned C2_REAR_LIGHT_CTRL :	1;
	 unsigned C3_RESERVE_CTRL :	1;
	 unsigned C4_OK_CTRL :	1;
	 unsigned C5_CANCEL_CTRL :	1;
	 unsigned C6_UP_CTRL :	1;
	 unsigned C7_DOWN_CTRL :	1;
	 unsigned C8_HI_LEVEL_CTRL :	1;
	 unsigned C9_INTAKE_PREHEAT_CTRL :	1;
	 unsigned C10_TRAILER_ABS_CTRL :	1;
	 unsigned C11_AXLE_DIFF_CTRL :	1;
	 unsigned C12_WHEEL_DIFF_CTRL :	1;
	 unsigned C13_VBAT_SW_CTRL :	1;
	 unsigned C14_Emergency_shutdown_CTRL :	1;
	 unsigned C15_QULI_INDICTOR_CTRL :	1;
	 unsigned C16_BRAKE_SW_L1_CTRL :	1;
	 unsigned C17_BRAKE_SW_L2_CTRL :	1;
	 unsigned C18_BRAKE_SW_L3_CTRL :	1;
	 unsigned C19_BRAKE_SW_L4_CTRL :	1;
	 unsigned C20_BRAKE_SW_R1_CTRL :	1;
	 unsigned C21_BRAKE_SW_R2_CTRL :	1;
	 unsigned C22_BRAKE_SW_R3_CTRL :	1;
	 unsigned C23_BRAKE_SW_R4_CTRL :	1;
	 unsigned C24_PARK_CTRL :	1;
	 unsigned C25_BCM_CTRL :	1;
	 unsigned C26_PASSAGER_SAFE_L_CTRL :	1;
	 unsigned A7_AIR_FILTER_CTRL :	1;
	 unsigned A8_DRIVER_DOOR_CTRL :	1;
	 unsigned A9_SAFETY_BELT_CTRL :	1;
	 unsigned A10_LOW_GEAR_CTRL :	1;
	 unsigned A22_DRYER_CTRL :	1;
	 unsigned A23_AIR_SUSPENSION_CTRL :	1;
	 unsigned A24_FLOATING_BRIDGE_CTRL :	1;
	 unsigned A25_RESERVE_CTRL :	1;
	 unsigned A26_RECHARGE_CTRL :	1;
	 unsigned IN_36 :	1;
	 unsigned IN_37 :	1;
	 unsigned IN_38 :	1;
	 unsigned IN_39 :	1;
	 unsigned IN_40 :	1;




    };
    
} IO_CONFIG_BIT;



typedef union {
    unsigned char byte[5];
    struct {
	unsigned int AD_PWR_OUT1_DET[20];
	unsigned int AD_PWR_OUT2_DET[20];
	unsigned int AD_PWR_OUT3_DET[20];
	unsigned int AD_PWR_OUT4_DET[20];
	unsigned int AD_299BACKIP_DET[20];
	unsigned int AD_299TURN_RIGHT_DET[20];
	unsigned int AD_299TURN_LEFT_DET[20];
	unsigned int AD_VCC_12V0_DET[20];
	unsigned int AD_VCC_12V0_OUT1_DET[20];
	unsigned int AD_AD_VCC_DET[20];
	
	unsigned int ad_cnt;
	
	unsigned short AD_PWR_OUT1_DET_OUTPUT;
	unsigned short AD_PWR_OUT2_DET_OUTPUT;
	unsigned short AD_PWR_OUT3_DET_OUTPUT;
	unsigned short AD_PWR_OUT4_DET_OUTPUT;
	unsigned short AD_299BACKIP_DET_OUTPUT;
	unsigned short AD_299TURN_RIGHT_DET_OUTPUT;
	unsigned short AD_299TURN_LEFT_DET_OUTPUT;
	unsigned short AD_VCC_12V0_DET_OUTPUT;
	unsigned short AD_VCC_12V0_OUT1_DET_OUTPUT;
	unsigned short AD_AD_VCC_DET_OUTPUT;
    };
    
} AD_DATA;


typedef union {

    struct {
		 unsigned AD1_Type : 	2;//00: 电压型   01: 电阻型  10: 不使用  11: NG
		 unsigned AD2_Type :	2;
		 unsigned AD3_Type :	2;
		 unsigned AD4_Type :	2;
		 unsigned AD5_Type :	2;
		 unsigned AD6_Type :	2;
		 unsigned AD7_Type :	2;
		 unsigned AD8_Type :	2;
		 unsigned AD1_SER_NUM :	2;//00: 0个   01: 1个  10: 2个  11: NG
		 unsigned AD2_SER_NUM :	2;
		 unsigned AD3_SER_NUM :	2;
		 unsigned AD4_SER_NUM :	2;
		 unsigned AD5_SER_NUM :	2;
		 unsigned AD6_SER_NUM :	2;
		 unsigned AD7_SER_NUM :	2;
		 unsigned AD8_SER_NUM :	2;


    };
    unsigned char byte[4];
} AD_BIT;
typedef union {

    struct {
		 unsigned short voltage_pwr_out1_det;//100mv/bit
		 unsigned short voltage_pwr_out2_det;//100mv/bit
		 unsigned short voltage_pwr_out3_det;//100mv/bit
		 unsigned short voltage_pwr_out4_det;//100mv/bit
		 unsigned short voltage_299bakeup_det;//100mv/bit
		 unsigned short voltage_299turnright_det;//100mv/bit
		 unsigned short voltage_299turnleft_det;//100mv/bit
		 unsigned short voltage_vcc_12v_det;//100mv/bit
		 unsigned short voltage_vcc_12v_out1_det;//100mv/bit
		 unsigned short voltage_res1_det;//100mv/bit
		 unsigned short voltage_res2_det;//100mv/bit
		 unsigned short voltage_ad_vcc_det;//100mv/bit



    };
    unsigned char byte[4];
} AD_VOL;

typedef union {

    struct {
		 unsigned short FIN_OUT1;// 1hz/bit
		 unsigned short FIN_OUT2;// 1hz/bit
		 unsigned short FREQ1;// 1hz/bit
		 unsigned short FREQ2;// 1hz/bit
    };
    unsigned char byte[8];
} FREQ_OUT;

extern DATA_BIT Gennal_Use[3];
extern IO_BIT g_io_fonfigure;
extern AD_BIT g_ad_configure;
extern AD_VOL g_ad_voltage;
extern FREQ_OUT g_freq;
extern IO_CONFIG_BIT config_io_l;
extern IO_CONFIG_BIT config_io_h;
extern AD_DATA g_ad_data_t;

#define g_bit_configure_io  Gennal_Use[0].bit1
#define g_bit_configure_ad  Gennal_Use[0].bit2
#define g_bit_1 Gennal_Use[0].bit3
#define g_bit_2  Gennal_Use[0].bit4
#define g_bit_3  Gennal_Use[0].bit5
#define F_50ms Gennal_Use[0].bit6
#define F_6s Gennal_Use[0].bit7


	
extern void CONFIG_Out(void);
extern void CONFIG_AD(void);
extern void CONFIG_IO(void);
extern void Interrupt10ms(void);
extern void Interrupt1ms(void);
extern void TAUB_Init(void);
extern void calculate_fin(void);
extern void Count_ADR05(void);
#endif	/* SYSTEM_H */

