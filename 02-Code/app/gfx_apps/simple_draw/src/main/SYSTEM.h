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


// #define DEBUG  1
#ifdef DEBUG

extern uint32_t debug_Tmr;        //测试时序用
extern uint32_t debug_Tmr_50ms;   //测试时序用
extern uint32_t debug_Tmr_100ms;  //测试时序用
extern uint32_t debug_Tmr_250ms;  //测试时序用
extern uint32_t debug_Tmr_500ms;  //测试时序用
extern uint32_t debug_Tmr_1000ms; //测试时序用
extern uint32_t debug_time; //测试时序用


#endif


#define IO_PWR_CTRL  PORT_ISO.P21.BIT.P21_8
#define HC595EN      PORT_ISO.P17.BIT.P17_11
#define B25_5V       PORT_ISO.P17.BIT.P17_7
#define B27_12V      PORT_ISO.P17.BIT.P17_8
#define PWR_CTRL     PORT_ISO.P1.BIT.P1_2

#define VersionH     PORT_ISO.PPR42.BIT.PPR42_12
#define VersionM     PORT_ISO.PPR42.BIT.PPR42_11
#define VersionL     PORT_ISO.PPR42.BIT.PPR42_10
#define VERSION_A    0
#define VERSION_B    1
#define VERSION_C    2
#define VERSION_D    3


#pragma pack(1)
typedef union {
	struct {
		unsigned bit0 : 1;
		unsigned bit1 : 1;
		unsigned bit2 : 1;
		unsigned bit3 : 1;
		unsigned bit4 : 1;
		unsigned bit5 : 1;
		unsigned bit6 : 1;
		unsigned bit7 : 1;
	};
	struct {
		unsigned bit01 : 2;
		unsigned bit23 : 2;
		unsigned bit45 : 2;
		unsigned bit67 : 2;
	};
	unsigned char byte;
} DATA_BIT;
#pragma pack()
extern DATA_BIT Gennal_Use[12];
#define mainview_update     Gennal_Use[0].bit0
#define mainUpdateFreq      Gennal_Use[0].bit1
#define start_LowPower_Mode Gennal_Use[0].bit2
#define from_ign_to_charge  Gennal_Use[0].bit3
#define from_charge_to_ign  Gennal_Use[0].bit4
#define charge_submenu      Gennal_Use[0].bit5
#define dm1_show_flg        Gennal_Use[0].bit6
#define PAGE                Gennal_Use[0].bit7

#define M_ON                Gennal_Use[1].bit0
#define ignoffon            Gennal_Use[1].bit1
#define TESTMODE            Gennal_Use[1].bit2
#define time_flag           Gennal_Use[1].bit3
#define M_ON_flag           Gennal_Use[1].bit4
#define return_from_menu    Gennal_Use[1].bit5
#define wake_can            Gennal_Use[1].bit6
#define START               Gennal_Use[1].bit7

#define Menu_TripInfo_f     Gennal_Use[2].bit0
#define Menu_PSNInfo_f      Gennal_Use[2].bit1
#define Menu_IC_f           Gennal_Use[2].bit2
#define Menu_VehicleInfo_f  Gennal_Use[2].bit3

#define key_resting15s      Gennal_Use[3].bit0
#define key_kazhi           Gennal_Use[3].bit1
#define key_jingxi          Gennal_Use[3].bit2
#define lightlevelset       Gennal_Use[3].bit3
#define lightadjusting      Gennal_Use[3].bit4
#define sensor_test         Gennal_Use[3].bit5
#define upgo                Gennal_Use[3].bit6
#define downgo              Gennal_Use[3].bit7

#define tm_w                Gennal_Use[4].bit0
#define tm_d                Gennal_Use[4].bit1
#define tm_month            Gennal_Use[4].bit2
#define tm_y                Gennal_Use[4].bit3
#define tm_m                Gennal_Use[4].bit4
#define tm_s                Gennal_Use[4].bit5
#define tm_h                Gennal_Use[4].bit6

#define BatVolUpdate        Gennal_Use[5].bit0
#define KValue_update       Gennal_Use[5].bit1
#define TpmsVol_update      Gennal_Use[5].bit2
#define update_tpmsvalue    Gennal_Use[5].bit3
#define MotorVolUpdate      Gennal_Use[5].bit4
#define PowerSonsUpdate     Gennal_Use[5].bit5
#define TimeDateUpdate      Gennal_Use[5].bit6
#define AveSpeedUpdate      Gennal_Use[5].bit7

#define WorkTimeUpdate      Gennal_Use[6].bit0
#define TripInfoUpdate      Gennal_Use[6].bit1
#define QiyaUpdate          Gennal_Use[6].bit2
#define dmUpdate            Gennal_Use[6].bit3
#define FCUWrkTimeUpdate    Gennal_Use[6].bit4
#define H2MaxPressUpdate    Gennal_Use[6].bit5
#define ChrgViewUpdate      Gennal_Use[6].bit6

#define BUZZ_STOP           Gennal_Use[7].bit0
#define LED_EAC             Gennal_Use[7].bit1
#define FRONT_COL           Gennal_Use[7].bit23
#define RIGHT_COL           Gennal_Use[7].bit4
#define LEFT_COL            Gennal_Use[7].bit5
#define Buzz_LSCU_Countdown Gennal_Use[7].bit6
#define Buzz_GearMismatched Gennal_Use[7].bit7


#define Brake_Lamp          Gennal_Use[8].bit0
#define Drive_DoorOpen      Gennal_Use[8].bit1
#define Buzz_DoorOpen       Gennal_Use[8].bit2
#define DoorOpenBuzz        Gennal_Use[8].bit3
#define Buzz_SaftBelt       Gennal_Use[8].bit4
#define WaterTempWarn       Gennal_Use[8].bit5
#define PARK_alarmflag      Gennal_Use[8].bit6
#define H2Leakage           Gennal_Use[8].bit7

#define tpmsalarm           Gennal_Use[9].bit0
#define ready_disp_flag	    Gennal_Use[9].bit1
#define ready_not_shown		Gennal_Use[9].bit2
#define bg_shown            Gennal_Use[9].bit3
#define BigIcon_Showing     Gennal_Use[9].bit4
#define alarm_update_flag	Gennal_Use[9].bit5
#define mile_key_ok_flg     Gennal_Use[9].bit6

// #define Set_Totlemiles_Flag Gennal_Use[10].bit0
#define clear_Trip_flag     Gennal_Use[10].bit1
#define totalmilesave       Gennal_Use[10].bit2
#define save_flag           Gennal_Use[10].bit3
#define saveRunTimeflag     Gennal_Use[10].bit4
#define saveH2TripConspflag Gennal_Use[10].bit5
#define SysInitOKFlag       Gennal_Use[10].bit6
#define VehicleReadyFlag    Gennal_Use[10].bit7

#define StartLearnflag      Gennal_Use[11].bit0
#define XIANSUSTUS          Gennal_Use[11].bit1
#define JianGuanMode        Gennal_Use[11].bit2
#define CAN_CHARGE_LINE     Gennal_Use[11].bit3
#define CAN_H2_ChuiSao      Gennal_Use[11].bit4
#define CAN_H2_LowTptChg    Gennal_Use[11].bit5
#define CAN_WAKE_FLG        Gennal_Use[11].bit6
#define DisplayTruckFlg     Gennal_Use[11].bit7


extern DATA_BIT CAN_KEY[5];
#define LCD_test             CAN_KEY[0].bit0
#define BUZZ_test            CAN_KEY[0].bit1
#define LED_test             CAN_KEY[0].bit2
#define Needle_Test          CAN_KEY[0].bit3
#define configFlag           CAN_KEY[0].bit4
#define CAN_TX_ENABLE        CAN_KEY[0].bit5
#define AutoLowBeamFlg       CAN_KEY[0].bit6

#define BCM_NODE_TMO_FLG     CAN_KEY[1].bit0 //BCAN 0x18FF2732
#define TPMS_NODE_TMO_FLG    CAN_KEY[1].bit1 //BCAN 0x18FFE633
#define ABS_NODE_TMO_FLG     CAN_KEY[1].bit2 //PCAN 0x18F0010B
#define EBS_NODE_TMO_FLG     CAN_KEY[1].bit3 //PCAN 0x18F0010B
#define AMT_NODE_TMO_FLG     CAN_KEY[1].bit4 //PCAN 0x18F00503 0x0CEF3103
#define ECAS_NODE_TMO_FLG    CAN_KEY[1].bit5 //BCAN 0x0CFE5A2F
#define LDWS_NODE_TMO_FLG    CAN_KEY[1].bit6 //BCAN/PCAN 0x18FE5BE8
#define AEBS_NODE_TMO_FLG    CAN_KEY[1].bit7 //PCAN 0x0CF02F2A


#define EAC_ON               CAN_KEY[2].bit0
#define HAZARD_ON            CAN_KEY[2].bit1
#define BMS_SAFETY_LINE_SG   CAN_KEY[2].bit2
#define ChuiSaoViewEnable    CAN_KEY[2].bit3
#define ChargeViewEnable     CAN_KEY[2].bit4
#define TD_MsgEnable         CAN_KEY[2].bit5
#define ABS_SelfCheckFlg     CAN_KEY[2].bit6
#define TrailerABS_CheckFlg  CAN_KEY[2].bit7

#define EnvTempSensorFltFlg  CAN_KEY[3].bit0
#define ZhongLianLockVehFlg  CAN_KEY[3].bit1
#define CAN_KEY_ACC          CAN_KEY[3].bit2
#define SmartRechgCfgFlg     CAN_KEY[3].bit3
#define SmartRechgViewFlg    CAN_KEY[3].bit4
#define MSD_SwitchStatus     CAN_KEY[3].bit5
#define GetOffReminderFlg    CAN_KEY[3].bit6
#define GetOffReminderStatus CAN_KEY[3].bit7

#define GetOffReminderViewFlg CAN_KEY[4].bit0


#define CAN_TIME  40 //CAN节点故障报警
// #define AP_LOWER_LMT_VALUE   535
// #define AP_UPER_LMT_VALUE    1200


#define VM_COLOR_RED        0xFFFF0000
#define VM_COLOR_GREEN      0xFF00FF00
#define VM_COLOR_BLUE       0xFF0000FF
#define VM_COLOR_YELLOW     0xFFFFFF00
#define VM_COLOR_WHITE      0xFFFFFFFF
#define VM_COLOR_GRAY       0xFFB0B0B0
#define VM_COLOR_BLACK      0xFF000000
#define VM_COLOR_CLEAR      0x00000000


#define B2001_TYRE_NUM      6
#define B2003_TYRE_NUM      8
#define B2004_TYRE_NUM      10
#define B2006_TYRE_NUM      10
#define B2012_TYRE_NUM      10
#define B2007_TYRE_NUM      12
#define B2013_TYRE_NUM      6



#pragma pack(1)
typedef struct {
	uint16_t Pos_X; //X_坐标
	uint16_t Pos_Y; //Y_坐标
} COORD_t;
#pragma pack()

#pragma pack(1)
typedef union {
	struct {
		uint8_t BCAN;
		uint8_t PCAN;
	};
	uint8_t bdata[2];
} CANCounter;
#pragma pack()

typedef struct {
	uint32_t total_miles;
	uint32_t single_miles;
	uint32_t single_H2_consp;
	uint32_t single_time;
} Mile;

typedef struct {
	void (*TaskHook)(void); // 要运行的任务函数
	const uint32_t ItvTime; // 同任务运行间隔时间
	uint8_t  Run;           // 程序运行标记：0-不运行，1运行
	uint32_t Timer;         // 同任务运行间隔时间计时器
	uint8_t  req;           // 请求启动
}TASK_COMPONENTS_t;


typedef enum {
	F_CRUISE = 0,
	F_PTO,
	F_ASR,
	F_EPS,
	F_TPMS_SELFCHECK,
	F_BUZZER_0,
	F_BUZZER_1,
	F_BUZZER_2,
	F_BUZZER_3,
	F_BUZZER_4,
	F_BUZZER_5,
	F_BUZZER_6,
	F_BUZZER_7,
	F_BUZZER_8,
	F_BUZZER_9,
	F_BUZZER_10,
	F_BUZZER_11,
	F_BUZZER_12,
	F_BUZZER_13,
	F_BUZZER_14,
	F_BUZZER_15,
	F_BUZZER_16,
	F_BUZZER_17,
	F_BUZZER_18,
	F_BUZZER_19,
	F_BUZZER_20,
	F_BUZZER_21,
	F_BUZZER_22,
	F_BUZZER_23,
	F_BUZZER_24,
	F_BUZZER_25,
	F_BUZZER_26,
	F_BUZZER_27,
	F_BUZZER_28,
	F_BUZZER_29,
	
	F_BUZZER_30,
	F_BUZZER_31,
	F_BUZZER_32,
	F_BUZZER_33,
	
	F_BUZZER_34,
	F_BUZZER_35,
	F_BUZZER_36,
	
	F_BUZZER_TEST,
	
	INTERVAL_NUM
	
} INTERVAL_NUM_e; 

enum BUZZ_TELLTAL_ID{
	BUZ_PRIORITY_0 = 0,  // 长鸣
	BUZ_PRIORITY_1,      // on_time = 500,  off_time = 500   //REESS热事件报警
	BUZ_PRIORITY_2,      // on_time = 500,  off_time = 500   //氢气泄漏
	BUZ_PRIORITY_3,      // on_time = 500,  off_time = 500   //车门未关
	BUZ_PRIORITY_4,      // on_time = 500,  off_time = 500   //安全带未系
	BUZ_PRIORITY_5,      // on_time = 1000, off_time = 120   //AEBS三级报警
	BUZ_PRIORITY_6,      // on_time = 240,  off_time = 240   //FCW碰撞预警二级警告/AEBS二级警告
	BUZ_PRIORITY_7,      // on_time = 240,  off_time = 480   //FCW碰撞预警一级警告/AEBS一级警告
	BUZ_PRIORITY_8,      // on_time = 120,  off_time = 120   //LDW车道偏离预警
	BUZ_PRIORITY_9,      // on_time = 120,  off_time = 120   //BSD盲点检测预警
	BUZ_PRIORITY_10,     // on_time = 250,  off_time = 250   //疲劳驾驶一级报警
	BUZ_PRIORITY_11,     // on_time = 500,  off_time = 500   //制动系统气压低，3min后停止
	BUZ_PRIORITY_12,     // on_time = 500,  off_time = 500   //车速大于5km/h，手刹未解除报警
	BUZ_PRIORITY_13,     // on_time = 500,  off_time = 500   //电机冷却温度过高，3min后停止
	BUZ_PRIORITY_14,     // on_time = 500,  off_time = 500   //系统严重故障
	BUZ_PRIORITY_15,     // on_time = 500,  off_time = 500   //龟速行驶
	BUZ_PRIORITY_16,     // on_time = 500,  off_time = 500   //超速报警
	BUZ_PRIORITY_17,     // on_time = 500,  off_time = 500   //MCU/MCU2:电机转速超速停机保护(521702,0)
	BUZ_PRIORITY_18,     // on_time = 500,  off_time = 500   //MCU/MCU2:电机转速超速报警保护(521702,16)
	BUZ_PRIORITY_19,     // on_time = 1000, off_time = 500   //轮间差速提示，T=2.5s，1长1短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔）
	BUZ_PRIORITY_20,     // on_time = 1000, off_time = 500   //轴间差速提示，T=2.5s，1长1短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔）
	BUZ_PRIORITY_21,     // on_time = 1000, off_time = 500   //驾驶室锁止不到位报警开，1长2短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔、短、间隔）
	BUZ_PRIORITY_22,     // on_time = 1000, off_time = 500   //取力提示，T=4.5s，1长3短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔、短、间隔、短、间隔）
	BUZ_PRIORITY_23,     // on_time = 500,  off_time = 500   //门开，位置灯未关提醒
	BUZ_PRIORITY_24,     // on_time = 500,  off_time = 500   //胎压报警
	BUZ_PRIORITY_25,     // on_time = 500,  off_time = 500   //空气悬架充/放气报警
	BUZ_PRIORITY_26,     // on_time = 500,  off_time = 500   //转向提示音
	BUZ_PRIORITY_27,     // on_time = 500,  off_time = 500   //请踩住脚刹释放驻车/气压低，驻车暂时无法解除
	BUZ_PRIORITY_28,     // on_time = 500,  off_time = 500   //
	BUZ_PRIORITY_29,     // on_time = 500,  off_time = 500   //
	BUZ_PRIORITY_30,
	BUZ_PRIORITY_31,
	BUZ_PRIORITY_32,
	BUZ_PRIORITY_33,
	BUZ_PRIORITY_34,
	BUZ_PRIORITY_35,
	BUZ_PRIORITY_36,
	BUZ_PRIORITY_TEST,     // on_time = 500, off_time = 500    //
	BUZ_PRIORITY_ALL
	
};

typedef enum {
	ID_GEAR_N = 0,
	ID_GEAR_R,
	ID_GEAR_D,
	ID_GEAR_D_1,
	ID_GEAR_D_2,
	ID_GEAR_D_3,
	ID_GEAR_D_4,
	ID_GEAR_D_5,
	ID_GEAR_D_6,
	ID_GEAR_D_7,
	ID_GEAR_D_8,
	ID_GEAR_D_9,
	ID_GEAR_D_10,
	ID_GEAR_D_11,
	ID_GEAR_D_12,
	ID_GEAR_D_R1,
	ID_GEAR_NULL
	
} GEAR_ID_e; 


/***************************************************************************/



/*****************分包处理CAN*****************/
extern unsigned char frmID;
extern unsigned char f_index;

extern Mile  Miles;
extern CANCounter  CANCounters;
extern DATA_BIT TyreLocation_pressureh[4];
extern DATA_BIT TyreLocation_pressurel[4];
extern DATA_BIT TyreLocation_temperature[4];
extern DATA_BIT TyreLocation_fastlostgas[4];


extern uint16_t delay_cnt;
extern uint32_t fin2_counter;
extern uint8_t  LightLevel[6];
extern uint32_t driving_time;
extern uint32_t WorkTime;
extern uint32_t aveSpeed;
extern int      EevTemp;
extern uint8_t  HWVersion;

extern uint8_t isOverseasVer;
extern uint8_t LckVehBoundState;
extern uint8_t Remaining_unlocked_times;
extern uint8_t LckVehdiplayState;

extern uint8_t BootVer[16];

extern uint16_t testcnt;
extern uint8_t  TestResult ;
extern uint8_t  testResultData[8];
extern uint8_t  testStep;



/******************************************************************/
extern unsigned int e_total_miles;
extern unsigned int e_single_miles;
extern unsigned int e_backlight_lcd;
extern unsigned int e_backlight_led;
extern unsigned int e_backlight_pannel;
extern unsigned int e_backlight_lcd_night;
extern unsigned int e_backlight_led_night;
extern unsigned int e_backlight_pannel_night;
extern unsigned int e_hourformat;
extern unsigned int e_displayconfig;
extern unsigned int e_uint;
extern unsigned int e_drivetime;
extern unsigned int e_VehLockStus;
extern unsigned int e_H2_TripConsp;
extern unsigned int e_ADR_ReferVolt;


unsigned int EEP_ReadFlag(void);
void EEP_WriteFlag(void);

void EEROM_WriteBackLight_lcd(void);
void EEROM_ReadBackLight_lcd(void);
void EEROM_WriteBackLight_led(void);
void EEROM_ReadBackLight_led(void);
void EEROM_WriteBackLight_pannel(void);
void EEROM_ReadBackLight_pannel(void);
void EEROM_WriteBackLight_Night_lcd(void);
void EEROM_ReadBackLight_Night_lcd(void);
void EEROM_WriteBackLight_Night_led(void);
void EEROM_ReadBackLight_Night_led(void);
void EEROM_WriteBackLight_Night_pannel(void);
void EEROM_ReadBackLight_Night_pannel(void);
void EEROM_WriteSingleMiles(void);
void EEROM_ReadSingleMiles(void);
void EEROM_WriteTotalMiles(void);
void EEROM_ReadTotalMiles(void);

void EEROM_Write_HOUR(void);
void EEROM_Read_HOUR(void);
void EEROM_Write_ContentConfing(void);
void EEROM_Read_ContentConfing(void);
void EEROM_Write_UINT(void);
void EEROM_Read_UINT(void);
void EEROM_Write_DRIVE_TIME(void);
void EEROM_Read_DRIVE_TIME(void);
void EEROM_Read_H2(void);
void EEROM_Write_H2(void);
void EEROM_Write_VehLock(void);
void EEROM_Read_VehLock(void);
void EEROM_Write_ADR_ReferVolt(void);
void EEROM_Read_ADR_ReferVolt(void);



void fdl_init(void);
void TAUB_Init(void);
void DM1_Flt_Handler(void);

uint8_t read_HwVersion(void);
void System_Init(void);
void SYSTME_Logic(void);
void LED_BG_ON(void);
void LED_BG_OFF(void);
void update_motor_point(void);
void update_output_pulse(void);
void calculate_fin(void); //call this function in 1s interrupt routine
void software_reset(void);

//dispaly
void LCD_Charge(void);
void LCD_Exec(void);
void BatVol_Display(uint8_t temp_value);
void MotorCoollingTemperature_Display(uint8_t soc_value);
unsigned int color_convert(unsigned char color_src);

extern void IOConfig_Send_1st(void);
extern void IOConfig_Send_2st(void);
extern void IOConfig_Send_3st(void);
extern void IOConfig_Send_4st(void);
extern void ADConfig_Send(void);


#endif	/* SYSTEM_H */

#ifndef __COMM_H_
#define __COMM_H_

#ifndef NULL
#define NULL    (void*)(0)
#endif

#define RANGE_LOFF_ROFF(VARIABLE,LOW,HIGH)    ((((VARIABLE)>=(LOW)) && ((VARIABLE) <= (HIGH)))? 1:0) /*全闭区间*/
#define RANGE_LOPEN_ROPEN(VARIABLE,LOW,HIGH)  ((((VARIABLE)>(LOW)) && ((VARIABLE) < (HIGH)))? 1:0)	 /*全开区间*/
#define RANGE_LOPEN_ROFF(VARIABLE,LOW,HIGH)   ((((VARIABLE)>(LOW)) && ((VARIABLE) <= (HIGH)))? 1:0)  /*左开右闭区间*/
#define RANGE_LOFF_ROPEN(VARIABLE,LOW,HIGH)   ((((VARIABLE)>=(LOW)) && ((VARIABLE) < (HIGH)))? 1:0)  /*左闭右开区间*/

#define CNT_ADD_OVER(name,max_value)  (((name)++) >= (max_value))?(1):(0)  /* 加计数溢出 */
#define CNT_SUB_OVER(name,min_value)  (((name)--) <= (min_value))?(1):(0)  /* 减计数溢出 */

#define MAX(A,B)         (((A) > (B))?1:0)
#define IS_SAME(A,B)     (((A) == (B))?1:0)
#define IS_UN_SAME(A,B)  (((A) == (B))?0:1)

#define SET_BIT(VAR,n)   ((VAR) |=  (1<<n))
#define CLR_BIT(VAR,n)   ((VAR) &= ~(1<<n))

#endif	/* __COMM_H_ */





