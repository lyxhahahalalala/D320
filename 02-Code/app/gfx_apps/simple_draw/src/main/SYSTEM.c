
#include "SYSTEM.h"
#include "string.h"
#include "r_bsp_stdio_api.h"
#include "r_dev_api.h"
#include "motor_sm.h"
#include "r_tick_api.h"
#include "ptmr.h"
#include "app_can.h"
#include "main.h"
#include "app_trip.h"
#include "app_texturewarning.h"
#include "sg.h"
#include "spiflash.h"
#include "r_sfma_api.h"
#include "uds_service.h"
#include "uds_support.h"
#include "obd_dtc.h"
#include "app_telltale.h"
#include "r_bsp_hmi_knob.h"
#include "key.h"
#include "pwm.h"
#include "rscan.h"
#include "interval.h"
#include "app_data_center.h"
#include "init.h"
#include "adc.h"
#include "scheduler.h"
#include "app_dm1.h"
#include "rtc.h"


DATA_BIT   Gennal_Use[12];
DATA_BIT   CAN_KEY[5];
DATE_t     timer;
Mile       Miles;
CANCounter CANCounters;
DATA_BIT   TyreLocation_pressureh[4];
DATA_BIT   TyreLocation_pressurel[4];
DATA_BIT   TyreLocation_temperature[4];
DATA_BIT   TyreLocation_fastlostgas[4];

uint16_t delay_cnt = 150U;
uint32_t fin1_counter;
uint32_t fin2_counter;
uint8_t  SleepTicks = 0;
uint32_t WorkTime;
uint32_t aveSpeed;
uint32_t freq[2];
uint32_t Tfin=0;
uint8_t  HWVersion;
uint8_t  LightLevel[6];
uint32_t driving_time=0;
int EevTemp;

uint8_t BootVer[16];

static uint16_t tonefreqHZ;
static uint8_t  toneVolume = 0;
static uint8_t second = 0;
static uint8_t minmute = 0;

static uint32_t backlight_led = 50;
static uint32_t backlight_pannel = 50;
static uint32_t backlight_lcd = 50;




#ifdef DEBUG
uint32_t debug_Tmr = 0;        //测试时序用
uint32_t debug_Tmr_50ms = 0;   //测试时序用
uint32_t debug_Tmr_100ms = 0;  //测试时序用
uint32_t debug_Tmr_250ms = 0;  //测试时序用
uint32_t debug_Tmr_500ms = 0;  //测试时序用
uint32_t debug_Tmr_1000ms = 0; //测试时序用
uint32_t debug_time = 0; //测试时序用

#endif


INTERVAL_t  Interval_List[] =
{
	[F_CRUISE]        = {0,0, 25,CYCLE,  25, 0}, //每隔 250ms 闪烁输出 250ms (循环输出)
	[F_PTO]           = {0,0, 50,CYCLE,  50, 0}, //每隔 500ms 闪烁输出 500ms (循环输出)
	[F_ASR]           = {0,0, 50,CYCLE,  50, 0}, //每隔 500ms 闪烁输出 500ms (循环输出)
	[F_EPS]           = {0,0, 50,CYCLE,  50, 0}, //每隔 500ms 闪烁输出 500ms (循环输出)
	
	[F_TPMS_SELFCHECK]= {0,0,300,CYCLE,   1, 1}, //10ms 闪烁输出 1次 3s //TPMS 上电自检，符号片点亮 3s
	
	[F_BUZZER_0]      = {0,0, 0xffffffff,CYCLE,   1, 0}, //长鸣  蜂鸣器报警0
	[F_BUZZER_1]      = {0,0, 12,CYCLE,  12, 0}, //REESS热事件报警
	[F_BUZZER_2]      = {0,0, 25,CYCLE,  25, 0}, //氢气泄漏
	[F_BUZZER_3]      = {0,0, 50,CYCLE,  50, 0}, //车门未关
	[F_BUZZER_4]      = {0,0, 50,CYCLE,  50, 0}, //安全带未系
	[F_BUZZER_5]      = {0,0,100,CYCLE,  12, 0}, //AEBS三级报警
	[F_BUZZER_6]      = {0,0, 24,CYCLE,  24, 0}, //FCW碰撞预警二级警告/AEBS二级警告
	[F_BUZZER_7]      = {0,0, 24,CYCLE,  48, 0}, //FCW碰撞预警一级警告/AEBS一级警告
	[F_BUZZER_8]      = {0,0, 24,CYCLE,  24, 0}, //LDW车道偏离预警
	[F_BUZZER_9]      = {0,0, 12,CYCLE,  12, 0}, //BSD盲点检测预警
	[F_BUZZER_10]     = {0,0, 25,CYCLE,  25, 0}, //疲劳驾驶 一级报警
	[F_BUZZER_11]     = {0,0, 50,CYCLE,  50, 0}, //制动系统气压低，3min后停止
	[F_BUZZER_12]     = {0,0, 50,CYCLE,  50, 0}, //车速大于5km/h，手刹未解除报警
	[F_BUZZER_13]     = {0,0, 50,CYCLE,  50, 0}, //电机冷却温度过高，3min后停止
	[F_BUZZER_14]     = {0,0, 50,CYCLE,  50, 0}, //系统严重故障
	[F_BUZZER_15]     = {0,0, 50,CYCLE,  50, 0}, //龟速行驶
	[F_BUZZER_16]     = {0,0, 50,CYCLE,  50, 0}, //超速报警
	[F_BUZZER_17]     = {0,0, 50,CYCLE,  50, 0}, //MCU/MCU2:电机转速超速停机保护(521702,0)	
	[F_BUZZER_18]     = {0,0, 50,CYCLE,  50, 0}, //MCU/MCU2:电机转速超速报警保护(521702,16)
	[F_BUZZER_19]     = {0,0,100,CYCLE,  50, 0}, //轮间差速提示，T=2.5s，1长1短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔）
	[F_BUZZER_20]     = {0,0,100,CYCLE,  50, 0}, //轴间差速提示，T=2.5s，1长1短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔）
	[F_BUZZER_21]     = {0,0,100,CYCLE,  50, 0}, //驾驶室锁止不到位报警开，1长2短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔、短、间隔）
	[F_BUZZER_22]     = {0,0,100,CYCLE,  50, 0}, //取力提示，T=4.5s，1长3短，其周期分别为1s和0.5s，间隔时间0.5s（长、间隔、短、间隔、短、间隔、短、间隔）
	[F_BUZZER_23]     = {0,0, 50,CYCLE,  50, 0}, //门开，位置灯未关提醒
	[F_BUZZER_24]     = {0,0, 50,CYCLE,  50, 90}, //胎压报警
	[F_BUZZER_25]     = {0,0, 50,CYCLE,  50, 0}, //空气悬架充/放气报警
	[F_BUZZER_26]     = {0,0, 50,CYCLE,  50, 0}, //转向提示音
	[F_BUZZER_27]     = {0,0, 50,CYCLE,  50, 3}, //请踩住脚刹释放驻车/气压低，驻车暂时无法解除
	[F_BUZZER_28]     = {0,0, 50,CYCLE,  50, 3}, //胎压学习
	[F_BUZZER_29]     = {0,0,300,CYCLE,   1, 1}, //胎压学习失败
	
	[F_BUZZER_30]     = {0,0, 25,CYCLE,  25, 0}, //ADU 2Hz
	[F_BUZZER_31]     = {0,0, 50,CYCLE,  50, 0}, //ADU 1Hz
	[F_BUZZER_32]     = {0,0, 100,CYCLE,  1, 1}, //ADU 单声
	[F_BUZZER_33]     = {0,0, 12,CYCLE,  12, 0}, //车路云 智驾弹窗  请立即接管
	[F_BUZZER_34]     = {0,0, 25,CYCLE,  25, 0}, //车路云 智驾弹窗  请准备接管
	[F_BUZZER_35]     = {0,0, 50,CYCLE,  50, 0}, //车路云 智驾弹窗  请注意观察
	[F_BUZZER_36]     = {0,0, 50,CYCLE,  50, 0}, //
	
	[F_BUZZER_TEST]   = {0,0, 50,CYCLE,  50, 10}, //蜂鸣测试
	
	
};

void UUID_data_update(void)
{
	uint8_t i = 0;
	
	const uint8_t ICU_PartNum[5][13] = 
	{
		{'3','8','0','1','0','4','0','-','C','5','1','0','4'},
		{'3','8','0','1','0','4','0','-','C','3','1','0','4'},
		{'3','8','0','1','0','4','0','-','C','3','1','0','6'},
		{'3','8','0','1','0','5','0','-','C','3','1','0','8'},
		{'3','8','0','1','0','5','0','-','C','3','1','0','9'},
	};
	const uint8_t VendorCode[2][5] = {
		{'0','D','2','4','5'},
		{'0','I','Q','6','4'},
	};
	uint8_t AppVer[13] = {0};
	uint8_t HwVer[8] = {0};
	uint8_t SwVer[8] = {0};
	
	
	if(eol_icutype > 4)
	{
		return ;
	}
	
	Diagnostic_EEProm_Read(Logicdata_list[INDEX_F191].ee_address, 13, Logicdata_list[INDEX_F191].p_data);
	for(i = 0; i < 13; i++)
	{
		if(ICU_PartNum[eol_icutype][i] != Logicdata_list[INDEX_F191].p_data[i])
		{
			memcpy (Logicdata_list[INDEX_F188].p_data, &ICU_PartNum[eol_icutype][0], 13);
			memcpy (Logicdata_list[INDEX_F191].p_data, &ICU_PartNum[eol_icutype][0], 13);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F188].ee_address, 13, Logicdata_list[INDEX_F188].p_data);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F191].ee_address, 13, Logicdata_list[INDEX_F191].p_data);
			
			break;
		}
	}
	
	Diagnostic_EEProm_Read(Logicdata_list[INDEX_F1A4].ee_address, 5, Logicdata_list[INDEX_F1A4].p_data);
	for(i = 0; i < 5; i++)
	{
		// if(VendorCode[0][i] != Logicdata_list[INDEX_F1A4].p_data[i] 
		// && VendorCode[1][i] != Logicdata_list[INDEX_F1A4].p_data[i])
		if(VendorCode[1][i] != Logicdata_list[INDEX_F1A4].p_data[i])
		{
			memcpy (Logicdata_list[INDEX_F1A4].p_data, &VendorCode[1][0], 5);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A4].ee_address, 5, Logicdata_list[INDEX_F1A4].p_data);
			break;
		}
	}
	
	
	
	AppVer[0]  = ICU_VERSIONS[0];
	AppVer[1]  = ICU_VERSIONS[1];
	AppVer[2]  = ICU_VERSIONS[2];
	AppVer[3]  = ICU_VERSIONS[4];
	AppVer[4]  = ICU_VERSIONS[5];
	AppVer[5]  = ICU_VERSIONS[7];
	AppVer[6]  = ICU_VERSIONS[8];
	AppVer[7]  = 0x30 + VERSIONS_YEAR / 10;
	AppVer[8]  = 0x30 + VERSIONS_YEAR % 10;
	AppVer[9]  = 0x30 + VERSIONS_MONTH / 10;
	AppVer[10] = 0x30 + VERSIONS_MONTH % 10;
	AppVer[11] = 0x30 + VERSIONS_DAY / 10;
	AppVer[12] = 0x30 + VERSIONS_DAY % 10;
	Diagnostic_EEProm_Read(Logicdata_list[INDEX_F189].ee_address, 13, Logicdata_list[INDEX_F189].p_data);
	for(i = 0; i < 13; i++)
	{
		if(AppVer[i] != Logicdata_list[INDEX_F189].p_data[i])
		{
			memcpy (Logicdata_list[INDEX_F189].p_data, &AppVer[0], 13);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F189].ee_address, 13, Logicdata_list[INDEX_F189].p_data);
			break;
		}
	}
	
	
	uint8_t HWVersion =  read_HwVersion();
	HwVer[0]  = '0';
	HwVer[1]  = '0';
	HwVer[2]  = '.';
	HwVer[3]  = '0';
	HwVer[4]  = 0x30 + HWVersion + 1;
	HwVer[5]  = '.';
	HwVer[6]  = '0';
	HwVer[7]  = '0';
	Diagnostic_EEProm_Read(Logicdata_list[INDEX_F010].ee_address, 8, Logicdata_list[INDEX_F010].p_data);
	for(i = 0; i < 8; i++)
	{
		if(HwVer[i] != Logicdata_list[INDEX_F010].p_data[i])
		{
			memcpy (Logicdata_list[INDEX_F010].p_data, &HwVer[0], 8);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F010].ee_address, 8, Logicdata_list[INDEX_F010].p_data);
			break;
		}
	}
	
	
	SwVer[0]  = ICU_VERSIONS[1];
	SwVer[1]  = ICU_VERSIONS[2];
	SwVer[2]  = '.';
	SwVer[3]  = ICU_VERSIONS[4];
	SwVer[4]  = ICU_VERSIONS[5];
	SwVer[5]  = '.';
	SwVer[6]  = ICU_VERSIONS[7];
	SwVer[7]  = ICU_VERSIONS[8];
	Diagnostic_EEProm_Read(Logicdata_list[INDEX_F011].ee_address, 8, Logicdata_list[INDEX_F011].p_data);
	for(i = 0; i < 8; i++)
	{
		if(SwVer[i] != Logicdata_list[INDEX_F011].p_data[i])
		{
			memcpy (Logicdata_list[INDEX_F011].p_data, &SwVer[0], 8);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F011].ee_address, 8, Logicdata_list[INDEX_F011].p_data);
			break;
		}
	}
	
	
	Diagnostic_EEProm_Read(Logicdata_list[INDEX_F123].ee_address, 5, F123_Fbl_VN);
	BootVer[0]  = 'V';
	BootVer[1]  = 0x30 + F123_Fbl_VN[0]/16; //转换为ASCII码
	BootVer[2]  = 0x30 + F123_Fbl_VN[0]%16;
	BootVer[3]  = '.';
	BootVer[4]  = 0x30 + F123_Fbl_VN[1]/16;
	BootVer[5]  = 0x30 + F123_Fbl_VN[1]%16;
	BootVer[6]  = '.';
	BootVer[7]  = 0x30 + F123_Fbl_VN[2]/16;
	BootVer[8]  = 0x30 + F123_Fbl_VN[2]%16;
	BootVer[9]  = 0x30 + F123_Fbl_VN[3]/16;
	BootVer[10] = 0x30 + F123_Fbl_VN[3]%16;
	BootVer[11] = 0x30 + F123_Fbl_VN[4]/16;
	BootVer[12] = 0x30 + F123_Fbl_VN[4]%16;
	
	BootVer[13] = 0;
	BootVer[14] = 0;
	BootVer[15] = 0;
}

void StoredData_Read(void)
{
	if(0x123456F0 != EEP_ReadFlag())
	{
		R_TICK_WaitMS(0, 50);
		ClrWdt();
		R_TICK_WaitMS(0, 50);
		ClrWdt();
		if(0x123456F0 != EEP_ReadFlag())
		{
			EEP_WriteFlag();
			ClrWdt();
			e_total_miles = 0;
			e_single_miles = 0;
			EEROM_WriteTotalMiles();	
			EEROM_WriteSingleMiles();
			
			e_backlight_lcd = 80;
			e_backlight_led = 80;
			e_backlight_pannel= 80;
			EEROM_WriteBackLight_lcd();
			EEROM_WriteBackLight_led();
			EEROM_WriteBackLight_pannel();
			
			e_backlight_lcd_night = 50;
			e_backlight_led_night = 50;
			e_backlight_pannel_night= 50;
			EEROM_WriteBackLight_Night_lcd();
			EEROM_WriteBackLight_Night_led();
			EEROM_WriteBackLight_Night_pannel();
			
			e_hourformat = 0;
			EEROM_Write_HOUR();
			
			e_displayconfig = 0x03;
			EEROM_Write_ContentConfing();
			
			e_uint = 0;
			EEROM_Write_UINT();
			
			e_drivetime = 0;
			EEROM_Write_DRIVE_TIME();
			
			e_H2_TripConsp = 0x00;
			EEROM_Write_H2();
			
			e_VehLockStus = 0x00;
			EEROM_Write_VehLock();
			
			EEROM_Write_ADR_ReferVolt();
			
			
			
			//以下可以2E服务修改，其余可22服务读，新板子上电，默认写一遍
			// 0xF1A4 //供应商代码(WR/N/R)
			// 0xF1A5 //供应商下线制造日期(WR/N/R)
			// 0xF1A6 //供应商下线制造流水号(WR/N/R)
			// 0xF190 //车辆VIN码(N/W/WR)
			// 0xF198 //维修站代码或诊断仪代码(N/N/WR)
			// 0xF19B //最近一次标定日期(N/WR/R)
			// 0xF19D //ECU装车日期(N/WR/R)
			
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F110].ee_address, 23, F110_Integra_SVN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F111].ee_address, 23, F111_App_SVN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F112].ee_address, 23, F112_Bsw_SVN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F187].ee_address, 13, F187_Spare_PN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F188].ee_address, 13, F188_Ecu_SN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F189].ee_address, 13, F189_Ecu_SVN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A0].ee_address,  5, F1A0_RT_VN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A1].ee_address,  5, F1A1_App_VN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A2].ee_address,  5, F1A2_HW_Ver);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A3].ee_address,  5, F1A3_CalibData_VN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A4].ee_address,  5, F1A4_SysSupplier_ID);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A5].ee_address,  6, F1A5_SysSupEol_MD);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F1A6].ee_address, 11, F1A6_SysSupEol_MSN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F190].ee_address, 17, F190_VIN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F191].ee_address, 13, F191_VehMan_Ecu_HN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F198].ee_address, 20, F198_RepairShp_TSN);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F19B].ee_address,  4, F19B_Calibration_Date);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F19D].ee_address,  4, F19D_Ecu_Ins_Date);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F15A].ee_address, 17, F15A_Audit_Trail);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F010].ee_address,  8, F010_HW_Ver);
			Diagnostic_EEProm_Write(Logicdata_list[INDEX_F011].ee_address,  8, F011_SW_Ver);
			
			uds_eeprom_init();
			
		}
	}
	
	EEROM_ReadTotalMiles();
    EEROM_ReadSingleMiles();
    EEROM_ReadBackLight_led();
    EEROM_ReadBackLight_lcd();
    EEROM_ReadBackLight_pannel();
    EEROM_ReadBackLight_Night_led();
    EEROM_ReadBackLight_Night_lcd();
    EEROM_ReadBackLight_Night_pannel();
    EEROM_Read_HOUR();
	EEROM_Read_ContentConfing();
	EEROM_Read_UINT();
	EEROM_Read_DRIVE_TIME();
    EEROM_Read_H2();
	EEROM_Read_VehLock();
	EEROM_Read_ADR_ReferVolt();
	
	Miles.single_miles = e_single_miles;
    Miles.total_miles = e_total_miles;
	Miles.single_H2_consp = e_H2_TripConsp;
	Miles.single_time = 0;
	driving_time = e_drivetime ;
	LckVehBoundState = e_VehLockStus;
	
    if((e_backlight_lcd<20)||(e_backlight_lcd>100))
	{
		e_backlight_lcd = 80;
		EEROM_WriteBackLight_lcd();
    }
    if((e_backlight_led<20)||(e_backlight_led>100))
	{
		e_backlight_led = 80;
		EEROM_WriteBackLight_led();
    }
    if((e_backlight_pannel<20)||(e_backlight_pannel>100))
	{
		e_backlight_pannel = 80;
		EEROM_WriteBackLight_pannel();
    }
    if((e_backlight_lcd_night<10)||(e_backlight_lcd_night>90))
	{
		e_backlight_lcd_night = 50;
		EEROM_WriteBackLight_Night_lcd();
    }
    if((e_backlight_led_night<10)||(e_backlight_led_night>90))
	{
		e_backlight_led_night = 50;
		EEROM_WriteBackLight_Night_led();
    }
    if((e_backlight_pannel_night<10)||(e_backlight_pannel_night>90))
	{
		e_backlight_pannel_night = 50;
		EEROM_WriteBackLight_Night_pannel();
    }
    LightLevel[0] = e_backlight_lcd;
	LightLevel[1] = e_backlight_led;
	LightLevel[2] = e_backlight_pannel;
    LightLevel[3] = e_backlight_lcd_night;
	LightLevel[4] = e_backlight_led_night;
	LightLevel[5] = e_backlight_pannel_night;
    
	backlight_led = e_backlight_led;
	backlight_pannel = e_backlight_pannel;
	backlight_lcd = e_backlight_lcd;
	
    eol_data_read();
    
	// UUID_data_update();
	
	// StorageInitOKflag = 1;
	
	
}
static void gpio_init(void)
{
	// P02 P03 P04 P05   Unused pin handler
	PORT_AWO.PMC0.UINT16	&= ~0x003C; // GPIO
	PORT_AWO.PM0.UINT16		|=  0x003C; //
	PORT_AWO.PM0.UINT16		&= ~0x0001; // P00 output     Inputs (PMn_m = 1)
	PORT_AWO.PIBC0.UINT16	|=  0x003C; //
	PORT_AWO.PD0.UINT16		|=  0x003C; // 1: On-chip pull-down resistor is connected.
	
	//P1_3 p1_4  Unused pin handler
	PORT_ISO.PMC1.UINT16	&= ~0x0018; //GPIO
	PORT_ISO.PM1.UINT16		|=  0x0018; //
	PORT_ISO.PIBC1.UINT16	|=  0x0018; //
	PORT_ISO.PD1.UINT16		|=  0x0018; // 1: On-chip pull-down resistor is connected.
	
	//P1_2 p1_5
	PORT_ISO.PM1.UINT16		&= ~0x0024;
	PORT_ISO.PMC1.UINT16	&= ~0x0024; //P1_2 MCU_PWR_HOLD  P1.5 LCD reset
	PORT_ISO.PD1.UINT16		|=  0x0024; //enable pulldown
	
	//P1_0 p1_1
	PORT_ISO.PM1.UINT16		&= ~0x0003;
	PORT_ISO.PMC1.UINT16	&= ~0x0003; //P1_0,1 MCU_WAKE_CAN2 MCU_WAKE_CAN1
	PORT_ISO.P1.BIT.P1_0     = 0;  //MCU_WAKE_CAN2
	PORT_ISO.P1.BIT.P1_1     = 0;  //MCU_WAKE_CAN1
	
	//P21_9   Unused pin handler
	PORT_ISO.PMC21.UINT16	&= ~0x0100; //GPIO
	PORT_ISO.PM21.UINT16	|=  0x0100; //
	PORT_ISO.PIBC21.UINT16	|=  0x0100; //
	PORT_ISO.PD21.UINT16	|=  0x0100; // 1: On-chip pull-down resistor is connected.
	
	//IOPWR_CTL
	PORT_ISO.PM21.UINT16	&= ~0x0100;
	PORT_ISO.PMC21.UINT16	&= ~0x0100;
	PORT_ISO.P21.BIT.P21_8   = 1;       // P21_8 - IOPWR_CTL
	IO_PWR_CTRL = 1;
	
	//21-6  TAUB0I5
	PORT_ISO.PMC21.UINT16	|=  0x0040; //Alternative function
	PORT_ISO.PIPC21.UINT16 	&= ~0x0040;
	PORT_ISO.PFC21.UINT16	|=  0x0040;
	
	//P16_0   Unused pin handler
	PORT_ISO.PMC16.UINT16	&= ~0x0001; //GPIO
	PORT_ISO.PM16.UINT16	|=  0x0001; //
	PORT_ISO.PIBC16.UINT16	|=  0x0001; //
	// PORT_ISO.PD16.UINT16	|=  0x0001; // 1: On-chip pull-down resistor is connected.
	
	//keys initialize
	PORT_ISO.PMC16.UINT16	&= ~0x0f00; 
	PORT_ISO.PM16.UINT16	|=  0x0f00;
	PORT_ISO.PIBC16.UINT16	|=  0x0f00; //P16_8 - P16_11   sw1234 input xiangyanglvxingche
	
	//AD_S0 - S2  
	PORT_ISO.PM16.UINT16	&= ~0x000E;
	PORT_ISO.PMC16.UINT16	&= ~0x000E; //OUTPUT
	
	//AD_R_B1_CTRL  AD_R_B5_CTRL  D_R_B18_CTRL P17_456   595EN P17_11
	PORT_ISO.PM17.UINT16	&= ~0x01F0;
	PORT_ISO.PMC17.UINT16	&= ~0x01F0;    
	PORT_ISO.P17.BIT.P17_4   = 1;  //AD_R_B1  接入10K上拉电阻
	PORT_ISO.P17.BIT.P17_5   = 1;  //AD_R_B5  接入10K上拉电阻
	PORT_ISO.P17.BIT.P17_6   = 1;  //AD_R_B18 接入10K上拉电阻
	PORT_ISO.P17.BIT.P17_7   = 1;  //B24_5V_MCU
	PORT_ISO.P17.BIT.P17_8   = 1;  //B27_12V_MCU
	
	//P17-9 10   Unused pin handler
	PORT_ISO.PMC17.UINT16	&= ~0x0600; //GPIO
	PORT_ISO.PM17.UINT16	|=  0x0600; //
	PORT_ISO.PIBC17.UINT16	|=  0x0600; //
	
	//P3_6
	PORT_ISO.PMC3.UINT16	&= ~0x0040; //GPIO
	PORT_ISO.PM3.UINT16		|=  0x0040; //
	PORT_ISO.PIBC3.UINT16	|=  0x0040; //
	PORT_ISO.PD3.UINT16		|=  0x0040; // 1: On-chip pull-down resistor is connected.
	
	//P42_0-5 P42_8
	PORT_ISO.PMC42.UINT16	&= ~0x013F; //GPIO
	PORT_ISO.PM42.UINT16	|=  0x013F; //
	PORT_ISO.PIBC42.UINT16	|=  0x013F; //
	PORT_ISO.PD42.UINT16	|=  0x013F; // 1: On-chip pull-down resistor is connected.
	
	//LED ON Control
	PORT_ISO.PM17.UINT16	&= ~0x0800;
	PORT_ISO.PMC17.UINT16	&= ~0x0800; //P17_11  74HC595_EN_N
	
	// LED_MCLK  LED_CS  P10_7 - P10_11
	PORT_ISO.PM10.UINT16	&= ~0x0C00;
	PORT_ISO.PMC10.UINT16	&= ~0x0C00; //OUTPUT
	
	//LED_MOSI  P11_1     P11_2 PL    P11_3 MCLK
	PORT_ISO.PM11.UINT16	&= ~0x000E;
	PORT_ISO.PMC11.UINT16	&= ~0x000E; //OUTPUT
	
	//MOSI  P11_0 Input 
	PORT_ISO.PMC11.UINT16	&= ~0x0001; 
	PORT_ISO.PM11.UINT16	|=  0x0001;
	PORT_ISO.PIBC11.UINT16	|=  0x0001; //P16_8 - P16_11   sw1234 input xiangyanglvxingche   
	
	//PWM output
	PORT_ISO.PMC42.UINT16	|=  0x8240; // 2nd Alternative function
	PORT_ISO.PFCE42.UINT16	&= ~0x8240;
	PORT_ISO.PM42.UINT16	&= ~0x8240;   
	PORT_ISO.PFC42.UINT16	|=  0x8240; //P42_15  BL--11  P42_9  LED_DRV--14  P42_6 LCD_CTRL
	
	//FO1 FO2 Control
	PORT_ISO.PM42.UINT16	&= ~0x6000;
	PORT_ISO.PMC42.UINT16	&= ~0x6000; //P42_13 - FO2    P42_14 - FO1 
	
	//LCD_Gamma_SD  P42_7
	PORT_ISO.PM42.UINT16	&= ~0x0080;
	PORT_ISO.PMC42.UINT16	&= ~0x0080; //OUTPUT
	// PORT_ISO.P42.BIT.P42_7= 1;//LCD_GM
	
	//version input
	PORT_ISO.PMC42.UINT16	&= ~0x1C00; 
	PORT_ISO.PM42.UINT16	|=  0x1C00;
	PORT_ISO.PIBC42.UINT16	|=  0x1C00; //P42_10,11,12 version control
	
	HWVersion =  read_HwVersion();
	if(HWVersion == VERSION_A)	//第一版，没有增加反相器电路  1-off  0-on
		HC595EN = 1;
	else						//后面版本增加反相器电路
		HC595EN = 0;
	
	if(HWVersion >= VERSION_B)//第2版，增加can接受唤醒
	{
		PORT_ISO.PM3.UINT16		&= ~0x018C; //P3-2 CAN1_STB  P3-3 CAN1-EN  P3-7 CAN2-STB  P3-8 CAN2-EN
		PORT_ISO.PMC3.UINT16	&= ~0x018C; //
		
		PORT_ISO.PM3.UINT16   	&= ~0x0210; //P3-4 CAN1_ERR  P3-9 CAN2_ERR
		PORT_ISO.PM3.UINT16	    |=  0x0210; //
		PORT_ISO.PIBC3.UINT16	|=  0x0210; //
	}
	
}
static void Hardware_Init(void)
{
	RS_CAN_init();
	Stepper_Motor_Init();
	gpio_init();
	PWR_CTRL = 1;
	adce_init();
	External_RTC_init(RTC_DS1302);
	can_chip_mode_switch(CAN_1_CH,GO_TO_SLEEP_MODE);
	can_chip_mode_switch(CAN_2_CH,GO_TO_SLEEP_MODE);
	
	UUID_data_update();
	
	init_wdt();
	ClrWdt();
	can_chip_mode_switch(CAN_1_CH,NORMAL_MODE);//!<设置进入正常模式
	can_chip_mode_switch(CAN_2_CH,NORMAL_MODE);//!<设置进入正常模式
	TAUB_Init();     // 外部事件触发
	PWM_Init();      // 频率输出	
	SoundGenInit();
	SoundStop();
}
static void Software_Init(void)
{
	ignoffon = 0;
	BUZZ = 0;
	memset(&Gennal_Use,0,12);
	// can_init();
	StoredData_Read();
	Interval_Init(Interval_List, INTERVAL_NUM);
	DataCenter_Init();
	DM1_Flt_Init();
	uds_init();
	
}

void app_task_50ms(void)
{
	if(M_ON_flag)
	{
		update_motor_point();
	}
}
void app_task_100ms(void)
{
	ADC_Filter();
	DataCenter_UpdateData();
}
void app_task_250ms(void)
{
	can_rxRoutine();
}
void app_task_500ms(void)
{
	uds_diagnostic_dtc_process();
	if(TD_MsgEnable == 0)
		External_RTC_ReadTime(&timer);
	
}
void app_task_1000ms(void)
{
	DM1_Flt_Scan_Check();
}
static void app_task_Init(void)
{
	Scheduler_Init();
	Scheduler_AddTask(app_task_50ms, TASK_PERIOD_MS(50));
	Scheduler_AddTask(app_task_100ms, TASK_PERIOD_MS(100));
	Scheduler_AddTask(app_task_250ms, TASK_PERIOD_MS(250));
	Scheduler_AddTask(app_task_500ms, TASK_PERIOD_MS(500));
	Scheduler_AddTask(app_task_1000ms, TASK_PERIOD_MS(1000));
	
}

void System_Init(void) //系统初始化函数
{
	uint8_t power_off_flag = 0;
	ptmrType_t Tick1 = 0;
	ptmrType_t SleepTmr = 0;
	
	START = 0;
	SysInitOKFlag = 0;
	TrailerABS_CheckFlg = 0;
	TD_MsgEnable = 0;
	
	Software_Init();
	Hardware_Init();
	app_task_Init();
	ClrWdt();
	
	SysInitOKFlag = 1;
	wake_can = 0;
	ready_not_shown = 1;
	IO_PWR_CTRL = 1;
	ptmr_Start(&Tick1);
	ptmr_Start(&SleepTmr);
	while (1)
	{
		ClrWdt(); 
		
		if(ptmr_Get(&Tick1) >= 25)
		{
			ptmr_Start(&Tick1);
			
			SW_Input();
			delay_cnt = 150U;
			
			if(M_ON            /*钥匙ON*/
			|| wake_can)       /*CAN唤醒*/
			{
				ptmr_Stop(&Tick1);
				ptmr_Stop(&SleepTmr);
				
				break; // 如果是ON唤醒或者充电唤醒则正常退出
			}
			else
			{
				start_LowPower_Mode = 1;
				if(!ptmr_IsStart(&SleepTmr))
				{
					ptmr_Start(&SleepTmr);
				}
				else if (ptmr_Get(&SleepTmr) >= 10000)
				{
					ptmr_Stop(&SleepTmr);
					
					B25_5V  = 0;
					B27_12V = 0;
					
					power_off_flag = 1;
					start_LowPower_Mode = 0;
				}
				
				if(!LED_POWERON && power_off_flag)
				{
					can_chip_mode_switch(CAN_1_CH,NORMAL_MODE);//!<设置进入正常模式
					can_chip_mode_switch(CAN_2_CH,NORMAL_MODE);//!<设置进入正常模式
					R_TICK_WaitMS(0, 200);
					can_chip_mode_switch(CAN_1_CH,GO_TO_SLEEP_MODE); //!<设置进入预睡眠模式
					can_chip_mode_switch(CAN_2_CH,GO_TO_SLEEP_MODE); //!<设置进入预睡眠模式
					R_TICK_WaitMS(0, 25);
					
					power_off_flag = 0;
					
					PWR_CTRL = 0; //!<关机
					R_TICK_WaitMS(0, 25);
				}
			}		
		}
    }
    
	if(wake_up3)
	{
		ClrWdt();
		StepperMotorZero();
		R_TICK_WaitMS(0, 500);
		ClrWdt();
	}
	
	Key_Init();
	
	ClrWdt();
	R_TICK_WaitMS(0, 100);
	ClrWdt();
	
	if(wake_up3)
	{
		display_Logo();
	}
	
	mainview_update = 1;
	External_RTC_ReadTime(&timer);     
	second = timer.second;
	minmute = timer.minute;
	
	can_init();
	
	START = 1;
	
}

uint8_t read_HwVersion(void)
{
    uint8_t version;
    version = 4*VersionH + 2*VersionM +VersionL;
    return(version);
}
uint8_t check_flash(void)
{
    uint8_t result;
    if(0x123456F0 != EEP_ReadFlag())
		result = 2; //failed
    else
		result = 0; //pass
	
	return result;

}
uint8_t check_rtc(void)
{
    uint8_t result;
    static uint8_t cnt = 0;
    if(cnt < 5)
	{
		cnt++;
		result = 0; //pass
    }
    else
	{
		cnt = 0;
		External_RTC_ReadTime(&timer);     
		if((second != timer.second)||(minmute != timer.minute))
		{
			second = timer.second;
			minmute = timer.minute;
			result = 0; //pass
		}
		else
		{
			result = 2;
		}
    }
	
	return result;
	

}


uint8_t  Test_Mode = 0;
uint16_t testcnt = 0;
uint8_t  TestResult = 0;
uint8_t  testResultData[8];
uint8_t  testStep = 0;
static uint8_t led_off = 1;
void Test_Needle(void)
{
	static uint8_t testdelaycnt = 0;
	
	if(testcnt < 200)
		testcnt++;
	if(testcnt > 20)
		Test_Mode = 0;
	
	memset(&alarm_led,0,sizeof(alarm_led));
	
	if(Test_Mode == 5)	//功能测试模式/老化测试模式
	{
		LED_test = 1;
		Needle_Test = 1;
		if(testStep == 0)
		{
			// PCAN_Send_TestRequest(0xff);//all on
			testStep = 1;
			testdelaycnt = 0;
		}
		else if(testStep == 1)
		{
			if(configFlag == 1)
			{
				if(testdelaycnt<100)
					testdelaycnt ++;
				if(testdelaycnt >10)
				{
					testdelaycnt  = 0;
					testResultData[0] = 0;
					testResultData[1] = 0;
					testResultData[2] = 0;
					testResultData[3] = 0;
					testResultData[0] |= ((~sw_input[0].byte)&0xfe);
					testResultData[1] |= ((~sw_input[1].byte)&0xff);
					testResultData[2] |= ((~sw_input[2].byte)&0xff);
					testResultData[3] |= ((~sw_input[3].byte)&0xff);
					//PCAN_Send_TestRequest(0x00);//all on
					testStep = 2;
					testResultData[4]  = testStep;
				}
			}
		}
		else if(testStep == 2)
		{
			if(configFlag == 0)
			{
				if(testdelaycnt<100)
					testdelaycnt ++;
				if(testdelaycnt >10)
				{
					testdelaycnt  = 0;
					testResultData[0] |= (sw_input[0].byte);
					testResultData[1] |= (sw_input[1].byte&0xFE); //滤过A19钥匙ON档
					testResultData[2] |= (sw_input[2].byte);
					testResultData[3] |= (sw_input[3].byte&0xEF); //滤过A26充电指示
					testStep = 3;//ea a7 34 b8
					testResultData[4]  = testStep;
					testResultData[5]  = 0;
					testResultData[6]  = 0;
					testResultData[7]  = 0;
					
					if((get_input_analog_quantity(AD_V_B3) > 4000) || (get_input_analog_quantity(AD_V_B3) < 3000))
						testResultData[5] |=0x01;
					else
						testResultData[5] &=~0x01;
					
					if((get_input_analog_quantity(AD_R_B4) > 110) || (get_input_analog_quantity(AD_R_B4) < 70))
						testResultData[5] |=0x02;
					else
						testResultData[5] &=~0x02;
					
					if((get_input_analog_quantity(AD_R_B17) > 90200) || (get_input_analog_quantity(AD_R_B17) < 73800))
						testResultData[5] |=0x04;
					else
						testResultData[5] &=~0x04;
					if((get_input_analog_quantity(AD_R_B18) > 1100) || (get_input_analog_quantity(AD_R_B18) < 900))
						testResultData[5] |=0x08;
					else
						testResultData[5] &=~0x08;
					
					if((get_input_analog_quantity(AD_V_B20) > 4000) || (get_input_analog_quantity(AD_V_B20) < 3000))
						testResultData[5] |=0x10;
					else
						testResultData[5] &=~0x10;
					
					if((get_input_analog_quantity(AD_V_B19) > 4000) || (get_input_analog_quantity(AD_V_B19) < 3000))
						testResultData[5] |=0x20;
					else
						testResultData[5] &=~0x20;
					
					if((get_input_analog_quantity(BAT_V) > 26000) || (get_input_analog_quantity(BAT_V) < 20000))
						testResultData[5] |=0x40;
					else
						testResultData[5] &=~0x40;
					
					if((get_input_analog_quantity(B24_5V_DET) > 5200) || ( get_input_analog_quantity(B24_5V_DET) < 4700))
						testResultData[5] |=0x80;
					else
						testResultData[5] &=~0x80;
					
					if((get_input_analog_quantity(B25_5V_DET) > 5200) || ( get_input_analog_quantity(B25_5V_DET) < 4700))
						testResultData[6] |=0x01;
					else
						testResultData[6] &=~0x01;
					
					if((get_input_analog_quantity(B26_5V_DET) > 5200) || (get_input_analog_quantity(B26_5V_DET) < 4700))
						testResultData[6] |=0x02;
					else
						testResultData[6] &=~0x02;
					
					if((get_input_analog_quantity(B27_12V_DET) > 13000) || (get_input_analog_quantity(B27_12V_DET) < 11000))
						testResultData[6] |=0x04;
					else
						testResultData[6] &=~0x04;
				}
			}
		}
		else;
		PCAN_Send_TestResult();
	}
	else if(Test_Mode == 6)	//老化测试模式
	{
		LED_test = 0;
		Needle_Test = 0;
		memset(&alarm_led,0xff,sizeof(alarm_led));
		BUZZ = 0;
		SoundStop();
	}
	else if(Test_Mode == 1)	//工装盒要求，内部模式1 硬线走硬件控制
	{
		LED_test = 0;
		Needle_Test = 0;
		memset(&alarm_led,0xff,sizeof(alarm_led));//all led on
		LED_Water = 0;
		LED_LowBeam = 0;
		LED_SOC4 = 0;
		LED_SOC5 = 0;
		LED_SOC6 = 0;
		LED_Shuiwen4 = 0; 
		LED_Shuiwen5 = 0;
		LED_Shuiwen6 = 0;
		LED_SocL = 0;
		BUZZ = 0;
		LED_OverSpeed = 0;
		LED_LOCK = IN31;
		LED_Park =  IN20;
		LED_ChargeConnected = IN29;
		LED_WheelDiff = IN26;
		LED_AxleDiff =  IN28;
		LED_SaftyBelt = IN32||IN24;
		LED_ABS= IN25;
		LED_BrakeDes = (IN17||IN19||IN21||IN15||IN18||IN22||IN23||IN16);
		LED_ECAS =  IN3;
		LED_POWERON =  IN7;
		BUZZ = 0;
		if(led_off){
			LED_Out();
			led_off = 0;
		}
		SoundStop();
	}
	else if(Test_Mode == 2)	//CE/RE测试，内部模式2 硬线灯假亮
	{
		LED_test = 0;
		Needle_Test = 0;
		memset(&alarm_led,0xff,sizeof(alarm_led));//all led on
		LED_Water = 0;
		LED_LowBeam = 0;
		LED_SOC4 = 0;
		LED_SOC5 = 0;
		LED_SOC6 = 0;
		LED_Shuiwen4 = 0; 
		LED_Shuiwen5 = 0;
		LED_Shuiwen6 = 0;
		LED_SocL = 0;
		BUZZ = 0;
		LED_OverSpeed = 0;
		LED_LOCK = 1;
		LED_Park =  0;
		LED_ChargeConnected =  0;
		LED_WheelDiff = 1;
		LED_AxleDiff =  1;
		LED_SaftyBelt = 1;
		LED_ABS= 1;
		LED_BrakeDes = 1;
		LED_ECAS =  1;
		LED_POWERON =  1;
		BUZZ = 0;
		if(led_off)
		{
			LED_Out();
			led_off = 0;
		}
		SoundStop();
	}
	else ;
	
}
void Test_LED(void)  //将所有的LED灯点亮。只为测试使用
{
	static unsigned char led_test_count = 0;
	
	unsigned char led_test_buf[61][7]=
	{
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//all off
		
		{0x00,0x00,0x00,0x00,0x00,0x00,0x01},// 1
		{0x00,0x00,0x00,0x00,0x00,0x00,0x02},// 2
		{0x00,0x00,0x00,0x00,0x00,0x00,0x04},// 3
		{0x00,0x00,0x00,0x00,0x00,0x00,0x08},// 4
		{0x00,0x00,0x00,0x00,0x00,0x00,0x10},// 5
		{0x00,0x00,0x00,0x00,0x00,0x00,0x20},// 6
		
		{0x00,0x00,0x00,0x01,0x00,0x00,0x00},// d261
		{0x00,0x20,0x00,0x00,0x00,0x00,0x00},// d272
		{0x00,0x00,0x00,0x00,0x01,0x00,0x00},// d34
		{0x00,0x00,0x00,0x80,0x00,0x00,0x00},// d49
		{0x02,0x00,0x00,0x00,0x00,0x00,0x00},// d278
		{0x00,0x00,0x40,0x00,0x00,0x00,0x00},// 左挂车灯
		
		{0x00,0x08,0x00,0x00,0x00,0x00,0x00},// d53
		{0x00,0x00,0x00,0x00,0x00,0x40,0x00},// d313
		{0x00,0x00,0x00,0x02,0x00,0x00,0x00},// d259
		{0x01,0x00,0x00,0x00,0x00,0x00,0x00},// d277 
		
		{0x80,0x00,0x00,0x00,0x00,0x00,0x00},// d282
		{0x08,0x00,0x00,0x00,0x00,0x00,0x00},// d279
		{0x10,0x00,0x00,0x00,0x00,0x00,0x00},// d280
		
		{0x00,0x00,0x00,0x00,0x00,0x10,0x00},// d312
		{0x00,0x00,0x00,0x10,0x00,0x00,0x00},// d342
		{0x00,0x00,0x00,0x40,0x00,0x00,0x00},// d48
		{0x00,0x10,0x00,0x00,0x00,0x00,0x00},// d274
		
		{0x00,0x00,0x00,0x08,0x00,0x00,0x00},// d330
		{0x00,0x00,0x00,0x00,0x08,0x00,0x00},// d37
		{0x00,0x00,0x00,0x04,0x00,0x00,0x00},// d331
		
		{0x00,0x40,0x00,0x00,0x00,0x00,0x00},// d278
		{0x20,0x00,0x00,0x00,0x00,0x00,0x00},// d281
		{0x00,0x00,0x00,0x00,0x00,0x80,0x00},// d317
		{0x00,0x02,0x00,0x00,0x00,0x00,0x00},// d326
		{0x00,0x04,0x00,0x00,0x00,0x00,0x00},// d327
		
		{0x00,0x01,0x00,0x00,0x00,0x00,0x00},// d328
		{0x00,0x80,0x00,0x00,0x00,0x00,0x00},// d329
		{0x00,0x00,0x00,0x00,0x04,0x00,0x00},// d36
		{0x00,0x00,0x08,0x00,0x00,0x00,0x00},// d339
		{0x00,0x00,0x20,0x00,0x00,0x00,0x00},// 右挂车灯
		{0x04,0x00,0x00,0x00,0x00,0x00,0x00},// d268 
		{0x00,0x00,0x10,0x00,0x00,0x00,0x00},// d340
		{0x00,0x00,0x00,0x00,0x20,0x00,0x00},// d39
		{0x00,0x00,0x00,0x00,0x80,0x00,0x00},// d41
		{0x00,0x00,0x00,0x20,0x00,0x00,0x00},// d341
		
		{0x00,0x00,0x00,0x00,0x10,0x00,0x00},// d38
		{0x00,0x00,0x00,0x00,0x02,0x00,0x00},// d35
		{0x00,0x00,0x02,0x00,0x00,0x00,0x00},// d334
		{0x00,0x00,0x04,0x00,0x00,0x00,0x00},// d335
		
		{0x00,0x00,0x00,0x00,0x40,0x00,0x00},// d40
		{0x00,0x00,0x01,0x00,0x00,0x00,0x00},// d336
		{0x00,0x00,0x80,0x00,0x00,0x00,0x00},// d337
		
		{0x00,0x00,0x00,0x00,0x00,0x20,0x00},// d316
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},// all off 后视镜加热灯
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},// all off 后视镜加热灯
		{0x40,0x00,0x00,0x00,0x00,0x00,0x00},// d333
		
		{0x00,0x00,0x00,0x00,0x00,0x00,0x40},// soc7
		{0x00,0x00,0x00,0x00,0x00,0x00,0x80},// soc8
		{0x00,0x00,0x00,0x00,0x00,0x01,0x00},// soc9
		{0x00,0x00,0x00,0x00,0x00,0x02,0x00},// soc10
		{0x00,0x00,0x00,0x00,0x00,0x04,0x00},// 5.0
		{0x00,0x00,0x00,0x00,0x00,0x08,0x00},// 5.6
		
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//
		
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff}                              
	};
	
	if(led_test_count < 61)
	{
		alarm_led[1].byte=led_test_buf[led_test_count][0];
		alarm_led[2].byte=led_test_buf[led_test_count][1];
		alarm_led[3].byte=led_test_buf[led_test_count][2];
		alarm_led[4].byte=led_test_buf[led_test_count][3];
		alarm_led[5].byte=led_test_buf[led_test_count][4];  
		alarm_led[6].byte=led_test_buf[led_test_count][5];  
		alarm_led[7].byte=led_test_buf[led_test_count][6];  
		if(51U == led_test_count)
		{
			LED_MirrorHeat = 0x01;
		}
		else
		{
			LED_MirrorHeat = 0x00;
		}
	}
	else if(led_test_count == 61)
	{
		alarm_led[0].byte=0xff;
		alarm_led[1].byte=0xff;
		alarm_led[2].byte=0xff;
		alarm_led[3].byte=0xff;
		alarm_led[4].byte=0xff;
		alarm_led[5].byte=0xff;
		alarm_led[6].byte=0xff;
		alarm_led[7].byte=0xff;	
		LED_MirrorHeat = 0x01;
		BUZZ=1;		//蜂鸣器 
		SG_NewTone(800, 50);
	}
	else if(led_test_count == 62)
	{
		alarm_led[0].byte=0x00;
		alarm_led[1].byte=0x00;
		alarm_led[2].byte=0x00;
		alarm_led[3].byte=0x00;
		alarm_led[4].byte=0x00;
		alarm_led[5].byte=0x00;
		alarm_led[6].byte=0x00;
		alarm_led[7].byte=0x00;
		LED_MirrorHeat = 0x00;
		BUZZ=0;  	//关闭蜂鸣器 
		SoundStop();
	}
	else led_test_count = 0;
	
	led_test_count++;
	
	LED_Out();
	
}

void LED_Logic(void) 
{
	static uint8_t oldbat = 0;
	static uint8_t oldtemp = 0;
	static uint8_t LED_LocaLight_ON_Old = 0x88;
	static uint8_t LED_LocaLight_OFF_Old = 0x88;
	static ptmrType_t postionlamp_delay_tmr1 = 0;
	static ptmrType_t CoolingTempBar_Tmr = 0;
	static ptmrType_t BatVolBar_Tmr = 0;
	uint32_t batt_volt;
	int16_t  mot_cooling_temp;
	uint8_t  ret = 0;
	uint8_t  dm_mask = 0;
	
	GeneralUse_t   *GU_18FE4F0B_t = NULL;
	GeneralUse_t   *GU_18F0010B_t = NULL;
	GeneralUse_t   *GU_18FF14E7_t = NULL;
	GeneralUse_t   *GU_18FEF131_t = NULL;
	VCU_18FFF531_t *VCU_18FFF531_d = NULL;
	VCU_04F02270_t *VCU_04F02270_d = NULL;
	GeneralUse_t   *VCU_18FDA403_d = NULL;
	GeneralUse_t   *GU_0CEF3103_t = NULL;
	// GeneralUse_t   *GU_18F7021E_t = NULL;
	EPB_18FF3C50_t *pEPB_18FF3C50_t = NULL;
	
	GeneralUse_t   *GU_18FF2232_t = NULL;
	GeneralUse_t   *BCMI_18FF2732 = NULL;
	GeneralUse_t   *DCMD_18FF28EC = NULL;
	
	uint8_t Vspeed = get_vehicle_speed();
	VCU_04F02270_d = (VCU_04F02270_t*)can_getPCanBuffer(0x04F02270);
	if(Test_Mode != 0)
	{
		Test_Needle();
	}
	else if(M_ON && M_ON_flag) //wake_up3
    {
		GU_18FE4F0B_t =  (GeneralUse_t*)can_getPCanBuffer(0x18FE4F0B);
		GU_18F0010B_t =  (GeneralUse_t*)can_getPCanBuffer(0x18F0010B);
		GU_18FF14E7_t =  (GeneralUse_t*)can_getBCanBuffer(0x18FF14E7);
		GU_18FEF131_t =  (GeneralUse_t*)can_getPCanBuffer(0x18FEF131);
		VCU_18FFF531_d = (VCU_18FFF531_t*)can_getPCanBuffer(0x18FFF531);
		VCU_18FDA403_d = (GeneralUse_t *)can_getPCanBuffer(0x18FDA403);
		GU_0CEF3103_t = (GeneralUse_t*)can_getPCanBuffer(0x0CEF3103);
		// GU_18F7021E_t = (GeneralUse_t*)can_getPCanBuffer(0x18F7021E);
		pEPB_18FF3C50_t = (EPB_18FF3C50_t*)can_getPCanBuffer(0x18FF3C50);
		
		GU_18FF2232_t =  (GeneralUse_t*)can_getBCanBuffer(0x18FF2232);
		BCMI_18FF2732 =  (GeneralUse_t*)can_getBCanBuffer(0x18FF2732);
		DCMD_18FF28EC =  (GeneralUse_t*)can_getBCanBuffer(0x18FF28EC);
		
		
		static ptmrType_t overSpeed_Tmr = 0;
		static uint8_t overSpeed_flg = 0;
		if(eol_overspeedvalue > 0 && eol_overspeedvalue != 0xFF && (eol_language == 0))
		{
			uint8_t Spd = get_display_speed();
			if(Spd > eol_overspeedvalue) //超速报警
			{
				if(!ptmr_IsStart(&overSpeed_Tmr))
					ptmr_Start(&overSpeed_Tmr);
				if(ptmr_Get(&overSpeed_Tmr) >= 3000)  //根据车速进行判断，车速超过100km/h持续3s 1、文字提醒；2、蜂鸣器报警。
					overSpeed_flg = 1;
				else
					overSpeed_flg = 0;
			}
			else
			{
				ptmr_Stop(&overSpeed_Tmr);
				if(overSpeed_flg)
				{
					if(Spd <= (eol_overspeedvalue - 3)) //超速报警
					{
						overSpeed_flg = 0;
					}
				}
			}
		}
		LED_OverSpeed = (overSpeed_flg);
		LED_OverSpeed |= VCU_04F02270_d->over_speed;
		/* static ptmrType_t driverLock_Tmr = 0;
		if(IN31) //驾驶室锁止不到位报警开关
		{
			if(!ptmr_IsStart(&driverLock_Tmr))
				ptmr_Start(&driverLock_Tmr);
			if(ptmr_Get(&driverLock_Tmr) >= 3000)
				ret = 1;
			else
				ret = 0;
		}
		else
		{
			ptmr_Stop(&driverLock_Tmr);
			ret = 0;
		} */
		LED_LOCK = (IN31); //(ret);
		
		LED_Park  = IN20;
		LED_Park |= (GU_18FEF131_t->byte1.bit34 == 1);
		LED_Park |= (pEPB_18FF3C50_t->parking_brake == 1); //驻车制动指示
		LED_Park |= VCU_04F02270_d->park_switch_status;//lyx
		LED_PowerBatCutOff = ((get_mot_volt() < 2000) && (Vspeed > 5)); //动力电池切断
		LED_PowerBatCutOff |= VCU_04F02270_d->hv_batt_cutoff;//lyx
		//巡航指示
		ret = (VCU_18FFF531_d->cruise_switch == 1); //巡航开关
		Set_Interval_Req(ret, F_CRUISE);
		LED_XunHang  = Get_Interval_Status(F_CRUISE);
		LED_XunHang |= (VCU_18FFF531_d->xunhang_status == 1); //行车巡航工作状态
		LED_XunHang |= (VCU_18FFF531_d->pto_status == 1); //PTO巡航工作状态
		
		LED_COFFEE  = ((GU_18FF14E7_t->byte3.bit12 != 0) && (GU_18FF14E7_t->byte1.bit12 == 1)); //疲劳驾驶 一级报警
		// LED_COFFEE |= ((GU_18FF14E7_t->byte3.bit12 != 0) && (GU_18FF14E7_t->byte1.bit12 == 2)); //疲劳驾驶 二级报警(三合一才支持)
		
		//水温高报警
		mot_cooling_temp = get_mot_cooling_temp();
		if((mot_cooling_temp <= 120) && (mot_cooling_temp >= 65))
		{//≥65℃
			LED_Water = 1;
		}
		else if(mot_cooling_temp < 62)
		{//<62℃
			if(LED_Water == 1)
				LED_Water = 0;
		}
		else LED_Water = 0;
		
		//电机故障
		LED_MotorFault  = (CurrentFltInfo[DM1_EF].FltBuf.AmberWarnLamp == 1);
		LED_MotorFault |= (CurrentFltInfo[DM1_EF].FltBuf.RedStopLamp == 1);
		LED_MotorFault |= (can_getPCanRxState(0x0CFF11EF) == CAN_FRAME_ST_TIMEOUT);
		LED_MotorFault |= (CurrentFltInfo[DM1_F0].FltBuf.AmberWarnLamp == 1);
		LED_MotorFault |= (CurrentFltInfo[DM1_F0].FltBuf.RedStopLamp == 1);
		LED_MotorFault |= VCU_04F02270_d->motor_warning;
		//绝缘报警
		LED_InsulationFault = (get_dm1_flt_status(DM1_F4, 521300, 18) == true); //绝缘电阻一般低
		LED_InsulationFault|= (get_dm1_flt_status(DM1_F4, 521300,  1) == true); //绝缘电阻严重低
		LED_InsulationFault |= VCU_04F02270_d->isolation_warning;
		//dcdc故障
		LED_DCDC  = (CurrentFltInfo[DM1_1A].FltBuf.AmberWarnLamp == 1);
		LED_DCDC |= (CurrentFltInfo[DM1_1A].FltBuf.RedStopLamp == 1);
		
		//动力电池故障
		LED_HighVolBatFault  = (CurrentFltInfo[DM1_F4].FltBuf.AmberWarnLamp == 1);
		LED_HighVolBatFault |= (CurrentFltInfo[DM1_F4].FltBuf.RedStopLamp == 1);
		LED_HighVolBatFault |= (can_getPCanRxState(0x0CFFEAF4) == CAN_FRAME_ST_TIMEOUT);
		LED_HighVolBatFault |= VCU_04F02270_d->hv_batt_warning;
		//充电连接
		LED_ChargeConnected = (CAN_CHARGE_LINE); //充电连接 20200311
		
		//动力电池过温	REESS热事件报警
		LED_BatOverTmp      = (get_dm1_flt_status(DM1_F4, 521301, 16) == true); //单体温度一般高
		LED_BatOverTmp     |= (get_dm1_flt_status(DM1_F4, 521301,  0) == true); //单体温度严重高
		LED_BatOverTmp     |= (get_dm1_flt_status(DM1_F4, 521335,  3) == true); //REESS热事件报警
		LED_BatOverTmp |= VCU_04F02270_d->batt_pack_temp_warning;
		//SOC过低报警
		// LED_SocL  = (get_dm1_flt_status(DM1_F4, 521309, 18) == true); //电池SOC一般低
		// LED_SocL |= (get_dm1_flt_status(DM1_F4, 521309,  1) == true); //电池SOC严重低
		LED_SocL  = 0;
		LED_SocL |= VCU_04F02270_d->soc_low_warning;//lyx
		//电机过温
		LED_MotorOverTmp  = (get_dm1_flt_status(DM1_EF, 521703, 16) == true);
		LED_MotorOverTmp |= (get_dm1_flt_status(DM1_EF, 521704, 16) == true);
		LED_MotorOverTmp |= VCU_04F02270_d->vcu_mot_over_temp;//lyx
		//EAC
		LED_EAC  = (CurrentFltInfo[DM1_EF].FltBuf.AmberWarnLamp == 1);
		LED_EAC |= (CurrentFltInfo[DM1_F0].FltBuf.AmberWarnLamp == 1);
		
		LED_Dryer = (IN4); //干燥器
		
		dm_mask = ((CurrentFltInfo[DM1_1E].FltNum == 1 && !get_dm1_flt_status(DM1_1E, 521564, 2)) || CurrentFltInfo[DM1_1E].FltNum > 1);
		
		// Warning报警灯 三角感叹号黄色
		if((CurrentFltInfo[DM1_F4].FltBuf.AmberWarnLamp == 1) /*电池管理*/ \
		|| (CurrentFltInfo[DM1_EF].FltBuf.AmberWarnLamp == 1) /*电机1控制*/ \
		|| (CurrentFltInfo[DM1_F0].FltBuf.AmberWarnLamp == 1) /*电机2控制*/ \
		|| (CurrentFltInfo[DM1_30].FltBuf.AmberWarnLamp == 1) /*空压机*/ \
		|| (CurrentFltInfo[DM1_1A].FltBuf.AmberWarnLamp == 1) /*DCDC*/ \
		|| (CurrentFltInfo[DM1_03].FltBuf.AmberWarnLamp == 1) /*自动变速箱*/ \
		|| (CurrentFltInfo[DM1_2E].FltBuf.AmberWarnLamp == 1) /*高压转向*/ \
		|| (CurrentFltInfo[DM1_1E].FltBuf.AmberWarnLamp == 1 && dm_mask) /*高压配电盒*/ \
		|| (CurrentFltInfo[DM1_4D].FltBuf.AmberWarnLamp == 1) /*低压转向*/ \
		|| (CurrentFltInfo[DM1_31].FltBuf.AmberWarnLamp == 1) /*HCU*/ \
		|| (CurrentFltInfo[DM1_82].FltBuf.AmberWarnLamp == 1) /*TMS*/ \
		|| (CurrentFltInfo[DM1_F5].FltBuf.AmberWarnLamp == 1) /*燃电系统 FCU*/ \
		|| (CurrentFltInfo[DM1_F6].FltBuf.AmberWarnLamp == 1) /*氢系统 HMS*/ \
		|| (CurrentFltInfo[DM1_84].FltBuf.AmberWarnLamp == 1) /*外挂DCDC1*/ \
		|| (CurrentFltInfo[DM1_85].FltBuf.AmberWarnLamp == 1) /*外挂DCDC2*/ \
		|| (CurrentFltInfo[DM1_A7].FltBuf.AmberWarnLamp == 1) /*冷却系统ATS*/ \
		|| (CurrentFltInfo[DM1_A8].FltBuf.AmberWarnLamp == 1) /*冷却系统ATS*/ \
		|| (CurrentFltInfo[DM1_8F].FltBuf.AmberWarnLamp == 1) /*燃料电池冷却系统 FTS*/ \
		|| (CurrentFltInfo[DM1_50].FltBuf.AmberWarnLamp == 1) /*EPB*/ \
		)
			LED_Warning = 1; //20200312 add last 3 items
		else
			LED_Warning = 0;
		
		//水位低(预留)
		static ptmrType_t CoolingLiquied_Tmr = 0;
		if(eol_shuiwei > 0 && get_input_analog_quantity(AD_R_B17) > 140000) //水位低(预留)
		{
			if(!ptmr_IsStart(&CoolingLiquied_Tmr))
				ptmr_Start(&CoolingLiquied_Tmr);
			if(ptmr_Get(&CoolingLiquied_Tmr) >= 30000) //当传感器电阻低于140KΩ超过30s时，点亮水位过低报警灯
				ret = 1;
			else
				ret = 0;
		}
		else
		{
			ptmr_Stop(&CoolingLiquied_Tmr);
			ret = 0;
		}
		LED_CoolingLiquied = (ret);
		
		// 整车故障 高压系统故障
		if((CurrentFltInfo[DM1_F4].FltBuf.RedStopLamp == 1) \
		|| (CurrentFltInfo[DM1_EF].FltBuf.RedStopLamp == 1) \
		|| (CurrentFltInfo[DM1_F0].FltBuf.RedStopLamp == 1) \
		|| (CurrentFltInfo[DM1_30].FltBuf.RedStopLamp == 1) \
		|| (CurrentFltInfo[DM1_1A].FltBuf.RedStopLamp == 1) \
		|| (CurrentFltInfo[DM1_03].FltBuf.RedStopLamp == 1) \
		|| (CurrentFltInfo[DM1_2E].FltBuf.RedStopLamp == 1) \
		|| (CurrentFltInfo[DM1_1E].FltBuf.RedStopLamp == 1) \
		)
			LED_SysFault= 1;
		else
			LED_SysFault = 0;
		if(can_getPCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)
{
    LED_SysFault |= VCU_04F02270_d->ep_fault_level_warning;
}
		//取力指示  //(BCMI_18FF2732->byte4.bit34 == 1)
		ret = ((VCU_18FDA403_d->byte1.bit78 == 1)&&(VCU_18FDA403_d->byte5.bit78 != 1)); //取力开关=1，取力控制≠1，取力指示闪烁，1Hz。
		Set_Interval_Req(ret, F_PTO);
		LED_PTO  = Get_Interval_Status(F_PTO);
		LED_PTO |= (VCU_18FDA403_d->byte5.bit78 == 1);
		LED_PTO |= (GU_0CEF3103_t->byte6.bit78 == 1);//取力指示灯
		// LED_PTO |= ((GU_18F7021E_t->byte2.byte&0x20) == 0x20);//取力指示灯
		
		if(eol_atmtype == 44U) //博世两档电驱桥
		{
			GeneralUse_t *TCU_0x18FF0B03 =  (GeneralUse_t*)can_getPCanBuffer(0x18FF0B03);
			GeneralUse_t *TCU_0x18FF0B04 =  (GeneralUse_t*)can_getPCanBuffer(0x18FF0B04);
			LED_WheelDiff = (IN26 || TCU_0x18FF0B03->byte1.bit12 == 1 || TCU_0x18FF0B04->byte1.bit12 == 1); //轮间差速锁止
		}
		else
		{
			LED_WheelDiff = (IN26); //轮间差速锁止
		}
		LED_AxleDiff      = (IN28); //轴间差速锁止
		LED_FarBeam       = (BCMI_18FF2732->byte3.bit34 == 1); //远光灯
		LED_LowBeam       = (BCMI_18FF2732->byte3.bit56 == 1); //近光灯
		LED_RearFog       = (BCMI_18FF2732->byte2.bit34 == 1); //后雾灯
		LED_FrontFog      = (BCMI_18FF2732->byte2.bit12 == 1); //前雾灯
		LED_TurnLeft      = (BCMI_18FF2732->byte1.bit12 == 1); //左转向
		LED_TurnRight     = (BCMI_18FF2732->byte1.bit34 == 1); //右转向
		LED_TrailerLeft   = (BCMI_18FF2732->byte1.bit56 == 1); //挂车左转向
		LED_TrailerRight  = (BCMI_18FF2732->byte1.bit78 == 1); //挂车右转向
		LED_LocationLight = (BCMI_18FF2732->byte3.bit12 == 1); //位置灯
		LED_LightCheck    = (BCMI_18FF2732->byte2.bit78 == 1); //灯丝检测
		LED_DoorOpen      = ((GU_18FF2232_t->byte5.bit34 == 1) || (GU_18FF2232_t->byte5.bit56== 1)); //门开指示
		AutoLowBeamFlg    = (BCMI_18FF2732->byte3.bit78 == 1); //自动大灯
		
		if(ignoffon)
		{//从关机到开机，不再执行亮灯动作
			PWM_Update_Duty_Cycle(LED_CHANNEL,0, PWM_PERIOD);//980hz
			PWM_Update_Duty_Cycle(BG_CHANNEL,0, PWM_PERIOD);//980hz
			PWM_Update_Duty_Cycle(LCD_CHANNEL,0, PWM_PERIOD);//980hz
		}
		else
		{
			if(LED_LocaLight_ON_Old != LED_LocationLight)	//开机，位置灯未打开
			{
				if(LED_LocationLight)
				{
					if(backlight_led != e_backlight_led_night
					|| backlight_pannel != e_backlight_pannel_night
					|| backlight_lcd != e_backlight_lcd_night
					)
					{
						if(!ptmr_IsStart(&postionlamp_delay_tmr1))
							ptmr_Start(&postionlamp_delay_tmr1);
						if(ptmr_Get(&postionlamp_delay_tmr1) >= 100)
						{
							backlight_led -= 1;
							if(backlight_led < e_backlight_led_night)
								backlight_led = e_backlight_led_night;
							
							backlight_pannel -= 1;
							if(backlight_pannel < e_backlight_pannel_night)
								backlight_pannel = e_backlight_pannel_night;
							
							backlight_lcd -= 1;
							if(backlight_lcd < e_backlight_lcd_night)
								backlight_lcd = e_backlight_lcd_night;
							
							ptmr_Stop(&postionlamp_delay_tmr1);
						}
					}
					else
					{
						LED_LocaLight_ON_Old = LED_LocationLight;
					}
				}
				else
				{
					if(backlight_led != e_backlight_led
					|| backlight_pannel != e_backlight_pannel
					|| backlight_lcd != e_backlight_lcd
					)
					{
						if(!ptmr_IsStart(&postionlamp_delay_tmr1))
							ptmr_Start(&postionlamp_delay_tmr1);
						if(ptmr_Get(&postionlamp_delay_tmr1) >= 100)
						{
							backlight_led += 1;
							if(backlight_led > e_backlight_led)
								backlight_led = e_backlight_led;
							
							backlight_pannel += 1;
							if(backlight_pannel > e_backlight_pannel)
								backlight_pannel = e_backlight_pannel;
							
							backlight_lcd += 1;
							if(backlight_lcd > e_backlight_lcd)
								backlight_lcd = e_backlight_lcd;
							
							ptmr_Stop(&postionlamp_delay_tmr1);
						}
					}
					else
					{
						LED_LocaLight_ON_Old = LED_LocationLight;
					}
				}
				
				PWM_Update_Duty_Cycle(LED_CHANNEL, backlight_led,    PWM_PERIOD);
				PWM_Update_Duty_Cycle(BG_CHANNEL,  backlight_pannel, PWM_PERIOD);
				PWM_Update_Duty_Cycle(LCD_CHANNEL, backlight_lcd,    PWM_PERIOD);
			}	
		}
		
		// 安全带未系指示
		LED_SaftyBelt  = (IN32); //安全带指示开关低电平信号
		LED_SaftyBelt |= ((eol_chengkecesaftybelt == 1) && (IN24) && (get_input_analog_quantity(AD_R_B4) <= 140)); //乘客侧安全带未系开关低电平信号及乘客侧气压开关传感器信号0~150Ω)
		
		//后视镜加热指示
		LED_MirrorHeat = (DCMD_18FF28EC->byte2.bit12== 1 || IN1); //特殊LED，直接IO驱动
		
		if((eol_ebs ==1)||(eol_ebs ==2)||(eol_ebs ==3)||(eol_ebs ==4)||(eol_ebs ==7)||(eol_ebs ==8)||(eol_ebs ==9)||(eol_ebs ==11) ||(eol_ebs == 12))
		{
			LED_ABS  = (ABS_SelfCheckFlg); //ABS自检
			LED_ABS |= ((GU_18F0010B_t->byte6.bit56 == 1) || (can_getPCanRxState(0x18F0010B) == CAN_FRAME_ST_TIMEOUT)); //ABS指示
		}
		else
		{
			LED_ABS = 0;
		}
		
		//ASR指示 //取消 ASR 指示
		// ret = ((GU_18F0010B_t->byte1.bit12 == 1) || (GU_18F0010B_t->byte1.bit34 == 1));  
		// Set_Interval_Req(ret, F_ASR);
		// LED_ASR  = Get_Interval_Status(F_ASR);
		// LED_ASR |= ((GU_18F0010B_t->byte3.bit34 == 1) || (can_getPCanRxState(0x18F0010B) == CAN_FRAME_ST_TIMEOUT));
		LED_ASR = 0;
		
		//ESC指示
		uint8_t asr_work_status = 0;
		uint8_t asr_lamp_indicator = 0;
		uint8_t esc_fun_abnormal = 0;
		uint8_t esc_fun_normal = 0;
		if(GU_18F0010B_t->byte3.bit34 == 1) //ESC OFF
		{
			LED_ESC = 0;
		}
		else
		{
			asr_work_status = ((GU_18F0010B_t->byte1.bit12 == 1) || (GU_18F0010B_t->byte1.bit34 == 1)); 
			asr_lamp_indicator = (GU_18F0010B_t->byte6.bit78 == 1);
			
			esc_fun_normal = ((eol_esc > 0) && (GU_18FE4F0B_t->byte1.bit12 == 1)&&(GU_18FE4F0B_t->byte1.bit34 == 1));
			
			esc_fun_abnormal = ((eol_esc > 0) && (can_getPCanRxState(0x18FE4F0B) == CAN_FRAME_ST_TIMEOUT));
			esc_fun_abnormal|= ((eol_esc > 0) && (GU_18FE4F0B_t->byte1.bit12 == 1)&&(GU_18FE4F0B_t->byte1.bit34 == 0));
			
			if(asr_work_status || esc_fun_normal)
			{
				Set_Interval_Req(true, F_EPS);
				LED_ESC = Get_Interval_Status(F_EPS);
			}
			else
			{
				LED_ESC = (asr_lamp_indicator || esc_fun_abnormal);
			}
			
		}
		
		//刹车片报警
		LED_BrakeDes  = ((eol_mcpbaojing == 1) && (IN17 || IN15)); //L1 R1
		LED_BrakeDes |= ((eol_mcpbaojing == 3) && (IN17 || IN21 || IN15 || IN19)); //L1 L2 R1 R2
		LED_BrakeDes |= ((eol_mcpbaojing == 5) && (IN17 || IN21 || IN18 || IN15 || IN19 || IN23)); //L1 L2 L3 R1 R2 R3
		LED_BrakeDes |= ((eol_mcpbaojing == 7) && (IN17 || IN21 || IN18 || IN15 || IN19 || IN23 || IN22 || IN16)); //L1 L2 L3 R1 R2 R3 L4 R4
		LED_BrakeDes |= ((eol_mcpbaojing == 9) && (IN17 || IN21 || IN18 || IN15 || IN19 || IN23 || IN22 || IN16 || IN27 || IN13)); //L1 L2 L3 R1 R2 R3 L4 R4 L5 R5
		
		static ptmrType_t ap_low_Tmr = 0;
		static ptmrType_t ap_high_Tmr = 0;
		if((get_front_airpressure() < QiyaLowLmt) 
		|| (get_rear_airpressure() < QiyaLowLmt))
		{
			if(!ptmr_IsStart(&ap_low_Tmr))
				ptmr_Start(&ap_low_Tmr);
			if(ptmr_Get(&ap_low_Tmr) >= 5000)  //前后回路气压 < 535kPa，且持续5s，点亮气压过低报警灯
				ret = 1;
			else
				ret = 0;
		}
		else 
		{
			ptmr_Stop(&ap_low_Tmr);
			ret = 0;
		}
		LED_AirPressureLow  = (ret); //前后回路气压低
		if((get_front_airpressure() > QiyaHighLmt) 
		|| (get_rear_airpressure() > QiyaHighLmt))
		{
			if(!ptmr_IsStart(&ap_high_Tmr))
				ptmr_Start(&ap_high_Tmr);
			if(ptmr_Get(&ap_high_Tmr) >= 5000)  //前后回路气压 > 1200kPa，且持续5s，点亮气压过高报警灯
				ret = 1;
			else
				ret = 0;
		}
		else 
		{
			ptmr_Stop(&ap_high_Tmr);
			ret = 0;
		}
		LED_AirPressureHigh = (!LED_AirPressureLow && ret); //气压高
		
		//空气悬架报警
		static ptmrType_t ecas_delay_Tmr = 0;
		static uint8_t ecas_lamp_status = 0;
		if(IN3)
		{
			ptmr_Start(&ecas_delay_Tmr);
			ecas_lamp_status = 1;
		}
		else
		{
			if((!ptmr_IsStart(&ecas_delay_Tmr))
			|| (ptmr_Get(&ecas_delay_Tmr) >= 300000)) //空气悬架开关闭合信号（高电平），信号消失后，报警持续5min
			{
				ptmr_Stop(&ecas_delay_Tmr);
				ecas_lamp_status = 0;
			}
		}
		LED_ECAS = (ecas_lamp_status); 
		
		LED_POWERON = (IN7); //电源指示
		LED_Disu = (VCU_18FFF531_d->powerlimit == 0x1u); //驱动功率限制
		
		//胎压异常
		if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
		{
			Set_Interval_Req(M_ON, F_TPMS_SELFCHECK);
			LED_TPMS  = (Get_Interval_Status(F_TPMS_SELFCHECK));
			LED_TPMS |= (TPMS_Warn > 0 && !study);
		}
		else
		{
			LED_TPMS = 0;
		}
		
		
		mot_cooling_temp = get_mot_cooling_temp();
		if(mot_cooling_temp != 0xffu) //电机冷却温度指示
		{
			if((ptmr_Get(&CoolingTempBar_Tmr) >= 1000) || (!ptmr_IsStart(&CoolingTempBar_Tmr)))
			{
				ptmr_Start(&CoolingTempBar_Tmr);
				
				
				if(mot_cooling_temp > 120)//-40℃-120℃
				{
					oldtemp = 1;
					MotorCoollingTemperature_Display(1);
				}
				else if(mot_cooling_temp >= 81) //≥81℃
				{
					oldtemp = 6;
					MotorCoollingTemperature_Display(6);
				}
				else if(mot_cooling_temp >= 71) //≥71℃
				{
					oldtemp = 5;
					MotorCoollingTemperature_Display(5);
				}
				else if(mot_cooling_temp >= 61) //≥61℃
				{
					oldtemp = 4;
					MotorCoollingTemperature_Display(4);
				}
				else if(mot_cooling_temp >= 51) //≥51℃
				{
					oldtemp = 3;
					MotorCoollingTemperature_Display(3);
				}
				else if(mot_cooling_temp >= 41) //≥41℃
				{
					oldtemp = 2;
					MotorCoollingTemperature_Display(2);
				}
				else
				{
					oldtemp = 1;
					MotorCoollingTemperature_Display(1);
				}
			}
			else MotorCoollingTemperature_Display(oldtemp);
			
		}
		else MotorCoollingTemperature_Display(1);
		
		batt_volt = get_batt_pack_volt();
		if(batt_volt != 0xffffu) //电池电压
		{
			if((ptmr_Get(&BatVolBar_Tmr) >= 1000) || (!ptmr_IsStart(&BatVolBar_Tmr)))
			{
				ptmr_Start(&BatVolBar_Tmr);
				
				if(batt_volt > 64255)
				{
					oldbat = 1;
					BatVol_Display(1);
				}
				else if(batt_volt>6500)
				{
					oldbat = 6;
					BatVol_Display(6);//电池电压
				}
				else if(batt_volt>6000)
				{
					oldbat = 5;
					BatVol_Display(5);//电池电压
				}
				else if(batt_volt>5500)
				{
					oldbat = 4;
					BatVol_Display(4);//电池电压
				}
				else if(batt_volt>5000)
				{
					oldbat = 3;
					BatVol_Display(3);//电池电压
				}
				else if(batt_volt>4500)
				{
					oldbat = 2;
					BatVol_Display(2);//电池电压
				}
				else
				{
					oldbat = 1;
					BatVol_Display(1);//电池电压
				}
			}
			else
			{
				BatVol_Display(oldbat);//电池电压
			}
		}
		else BatVol_Display(1);//电池电压
		
		LED_LocaLight_OFF_Old = 0x88;
		
	}
	else
	{
		LED_LocaLight_ON_Old = 0x88;
		
		GU_18FF2232_t =  (GeneralUse_t*)can_getBCanBuffer(0x18FF2232);
		BCMI_18FF2732 =  (GeneralUse_t*)can_getBCanBuffer(0x18FF2732);
		DCMD_18FF28EC =  (GeneralUse_t*)can_getBCanBuffer(0x18FF28EC);
		
		alarm_led[1].byte = 0x00;
		alarm_led[2].byte = 0x00;
		alarm_led[3].byte = 0x00;
		alarm_led[4].byte = 0x00;
		alarm_led[5].byte = 0x00;
		alarm_led[6].byte = 0x00;
		alarm_led[7].byte = 0x00;
		LED_MirrorHeat    = 0; //后视镜加热；特殊LED，直接IO驱动
		LED_POWERON       = (IN7); //电源指示
		LED_LocationLight = (BCMI_18FF2732->byte3.bit12== 1); //位置灯
		LED_DoorOpen      = ((GU_18FF2232_t->byte5.bit34== 1) || (GU_18FF2232_t->byte5.bit56== 1)); //门开指示
		LED_TurnLeft      = (BCMI_18FF2732->byte1.bit12 == 1); //左转向
		LED_TurnRight     = (BCMI_18FF2732->byte1.bit34 == 1); //右转向
		LED_TrailerLeft   = (BCMI_18FF2732->byte1.bit56 == 1); //挂车左转向
		LED_TrailerRight  = (BCMI_18FF2732->byte1.bit78 == 1); //挂车右转向
		
		if(CAN_CHARGE_LINE)
		{
			//充电连接
			LED_ChargeConnected = (CAN_CHARGE_LINE); //充电连接 20200311
			
			LED_InsulationFault = (get_dm1_flt_status(DM1_F4, 521300, 18) == true); //绝缘电阻一般低
			LED_InsulationFault|= (get_dm1_flt_status(DM1_F4, 521300,  1) == true); //绝缘电阻严重低
			LED_InsulationFault |= VCU_04F02270_d->isolation_warning;
			
			LED_SocL            = (get_dm1_flt_status(DM1_F4, 521309, 18) == true); //电池SOC一般低
			LED_SocL           |= (get_dm1_flt_status(DM1_F4, 521309,  1) == true); //电池SOC严重低
			LED_SocL           |= VCU_04F02270_d->soc_low_warning;//lyx
			LED_HighVolBatFault = (CurrentFltInfo[DM1_F4].FltBuf.RedStopLamp == 1); //动力电池故障
			LED_HighVolBatFault |= VCU_04F02270_d->hv_batt_warning;
			
			LED_BatOverTmp      = (get_dm1_flt_status(DM1_F4, 521301, 16) == true); //单体温度一般高
			LED_BatOverTmp     |= (get_dm1_flt_status(DM1_F4, 521301,  0) == true); //单体温度严重高
			LED_BatOverTmp     |= (get_dm1_flt_status(DM1_F4, 521335,  3) == true); //REESS热事件报警
			LED_BatOverTmp      |= VCU_04F02270_d->batt_pack_temp_warning;
			if((CurrentFltInfo[DM1_F4].FltBuf.RedStopLamp == 1) \
			|| (CurrentFltInfo[DM1_EF].FltBuf.RedStopLamp == 1) \
			|| (CurrentFltInfo[DM1_F0].FltBuf.RedStopLamp == 1) \
			|| (CurrentFltInfo[DM1_30].FltBuf.RedStopLamp == 1) \
			|| (CurrentFltInfo[DM1_1A].FltBuf.RedStopLamp == 1) \
			|| (CurrentFltInfo[DM1_03].FltBuf.RedStopLamp == 1) \
			|| (CurrentFltInfo[DM1_2E].FltBuf.RedStopLamp == 1) \
			|| (CurrentFltInfo[DM1_1E].FltBuf.RedStopLamp == 1) \
			)
			{
				LED_SysFault= 1; //整车故障
			}
			else
			{
				LED_SysFault = 0;
			}
			if(can_getPCanRxState(0x04F02270) == CAN_FRAME_ST_RECVED)
{
    LED_SysFault |= VCU_04F02270_d->ep_fault_level_warning;
}
		}
		
		if(LED_LocaLight_OFF_Old != LED_LocationLight) //开机，位置灯未打开
		{
			if(LED_LocationLight)
			{
				if(backlight_led != e_backlight_led_night
				|| backlight_pannel != e_backlight_pannel_night
				|| backlight_lcd != e_backlight_lcd_night
				)
				{
					if(!ptmr_IsStart(&postionlamp_delay_tmr1))
						ptmr_Start(&postionlamp_delay_tmr1);
					if(ptmr_Get(&postionlamp_delay_tmr1) >= 100)
					{
						backlight_led -= 1;
						if(backlight_led < e_backlight_led_night)
							backlight_led = e_backlight_led_night;
						
						backlight_pannel -= 1;
						if(backlight_pannel < e_backlight_pannel_night)
							backlight_pannel = e_backlight_pannel_night;
						
						backlight_lcd -= 1;
						if(backlight_lcd < e_backlight_lcd_night)
							backlight_lcd = e_backlight_lcd_night;
						
						ptmr_Stop(&postionlamp_delay_tmr1);
					}
				}
				else
				{
					LED_LocaLight_OFF_Old = LED_LocationLight;
				}
			}
			else
			{
				if(backlight_led != e_backlight_led
				|| backlight_pannel != e_backlight_pannel
				|| backlight_lcd != e_backlight_lcd
				)
				{
					if(!ptmr_IsStart(&postionlamp_delay_tmr1))
						ptmr_Start(&postionlamp_delay_tmr1);
					if(ptmr_Get(&postionlamp_delay_tmr1) >= 100)
					{
						backlight_led += 1;
						if(backlight_led > e_backlight_led)
							backlight_led = e_backlight_led;
						
						backlight_pannel += 1;
						if(backlight_pannel > e_backlight_pannel)
							backlight_pannel = e_backlight_pannel;
						
						backlight_lcd += 1;
						if(backlight_lcd > e_backlight_lcd)
							backlight_lcd = e_backlight_lcd;
						
						ptmr_Stop(&postionlamp_delay_tmr1);
					}
				}
				else
				{
					LED_LocaLight_OFF_Old = LED_LocationLight;
				}
			}
			
			if(HWVersion == VERSION_A)	//第一版，没有增加反相器电路
				HC595EN = 0;
			else						//后面版本增加反相器电路
				HC595EN = 1;
			
			if(GetOffReminderFlg
			/* || (CAN_KEY_ACC && SmartRechgCfgFlg)  */
			|| CAN_CHARGE_LINE   /*充电唤醒*/
			|| CAN_H2_ChuiSao    /*吹扫进行中*/
			|| CAN_H2_LowTptChg) /*低温停车充电中*/
			{
				PWM_Update_Duty_Cycle(LCD_CHANNEL, backlight_lcd, PWM_PERIOD);
			}
			else
			{
				PWM_Update_Duty_Cycle(LCD_CHANNEL, 0/* backlight_lcd */, PWM_PERIOD);
			}
			
			PWM_Update_Duty_Cycle(BG_CHANNEL, 0/* backlight_pannel */, PWM_PERIOD);
			PWM_Update_Duty_Cycle(LED_CHANNEL, backlight_led, PWM_PERIOD);
		}
	}
	
	LED_Out();
	
}
void Buzzer_Logic(void)
{
	static ptmrType_t water_overtemp_beep_Tmr = 0;
	static uint8_t Waterbeep_180S = 0;
	
	static ptmrType_t OverSpeed_beep_Tmr = 0;
	static uint8_t OverSpeedBeep = 0;
	
	static ptmrType_t LmtSpdBeep_Tmr = 0;
	static uint8_t LmtSpdBeepFlg = 0;
	
	static ptmrType_t ap_beep_Tmr = 0;
	static uint8_t QYbeep_30S = 0;
	
	uint8_t  buzzer_warn_flg[BUZ_PRIORITY_ALL] = {0};
	uint8_t  gear_temp;
	
	VCU_18FFF531_t *VCU_18FFF531_d  = NULL;
	EPB_18FF3C50_t *pEPB_18FF3C50_t = NULL;
	
	VCU_18FFF531_d = (VCU_18FFF531_t*)can_getPCanBuffer(0x18FFF531);
	pEPB_18FF3C50_t = (EPB_18FF3C50_t*)can_getPCanBuffer(0x18FF3C50);
	
	uint8_t  Vspeed = get_vehicle_speed();
	uint16_t mot_rpm = get_motor_speed();
	
	if(M_ON && !ignoffon)
	{
		if( ((get_front_airpressure() < QiyaLowLmt) || (get_rear_airpressure() < QiyaLowLmt)) 
		&& (mot_rpm > 0 && mot_rpm != 0xffffu) )
		{
			if(!ptmr_IsStart(&ap_beep_Tmr))
				ptmr_Start(&ap_beep_Tmr);
			if(ptmr_Get(&ap_beep_Tmr) <= 30000) 
				QYbeep_30S = 1;
			else
				QYbeep_30S = 0;
		}
		else
		{
			ptmr_Stop(&ap_beep_Tmr);
			QYbeep_30S = 0;
		}
		
		if(LED_DoorOpen) //门开指示
		{
			if(Vspeed > 10)
				Buzz_DoorOpen = 1; //门开蜂鸣
			else if(Vspeed < 5)
				Buzz_DoorOpen = 0;
			else ;
			
			gear_temp = get_vehicle_current_gear();
			if(((gear_temp != 0) && (gear_temp != 0xff)) && (VCU_18FFF531_d->ready ==1)) //车辆出于可行驶模式，下车注意安全
				Drive_DoorOpen = 1;
			else
				Drive_DoorOpen = 0;
			
		}
		else
		{
			Buzz_DoorOpen = 0;
			Drive_DoorOpen = 0;
		}
		
		if(LED_SaftyBelt) //安全带未系
		{
			if(Vspeed > 10)
				Buzz_SaftBelt = 1; //安全带未系蜂鸣
			else if(Vspeed < 5)
				Buzz_SaftBelt = 0;
			else ;
		}
		else Buzz_SaftBelt = 0;
		
		if(LED_Park)
		{
			if(Vspeed > 5)
				PARK_alarmflag = 1;
			else if(Vspeed < 3)
				PARK_alarmflag = 0;
			else ;
		}
		else PARK_alarmflag = 0;
		
		if(LED_Water)
		{
			if(!ptmr_IsStart(&water_overtemp_beep_Tmr))
				ptmr_Start(&water_overtemp_beep_Tmr);
			if(ptmr_Get(&water_overtemp_beep_Tmr) <= 180000) 
				Waterbeep_180S = 1;
			else
				Waterbeep_180S = 0;
		}
		else
		{
			ptmr_Stop(&water_overtemp_beep_Tmr);
			Waterbeep_180S = 0;
		}
		
		if(LED_OverSpeed)
		{
			if(!ptmr_IsStart(&OverSpeed_beep_Tmr))
				ptmr_Start(&OverSpeed_beep_Tmr);
			if(ptmr_Get(&OverSpeed_beep_Tmr) <= 10000) 
				OverSpeedBeep = 1;
			else
				OverSpeedBeep = 0;
		}
		else
		{
			ptmr_Stop(&OverSpeed_beep_Tmr);
			OverSpeedBeep = 0;
		}
		
		if(XIANSUSTUS)
		{
			if(!ptmr_IsStart(&LmtSpdBeep_Tmr))
				ptmr_Start(&LmtSpdBeep_Tmr);
			if(ptmr_Get(&LmtSpdBeep_Tmr) <= 5000) 
				LmtSpdBeepFlg = 1;
			else
				LmtSpdBeepFlg = 0;
		}
		else
		{
			ptmr_Stop(&LmtSpdBeep_Tmr);
			LmtSpdBeepFlg = 0;
		}
		
		buzzer_warn_flg[BUZ_PRIORITY_1]  = (get_dm1_flt_status(DM1_F4, 521335,  3) == true); //REESS热事件报警
		buzzer_warn_flg[BUZ_PRIORITY_2]  = ((eol_fadongjiorkaji==0x2d) && (H2Leakage==1));   //氢气泄漏
		buzzer_warn_flg[BUZ_PRIORITY_2] |= (Buzz_LSCU_Countdown);   //转向助力即将失效，请立即靠边停车
		buzzer_warn_flg[BUZ_PRIORITY_3]  = (Buzz_DoorOpen || (IN11 && Vspeed > 0)); //车门未关 || 翼门开启，且车速大于0
		buzzer_warn_flg[BUZ_PRIORITY_3] |= (Drive_DoorOpen); //车辆出于可行驶模式，下车注意安全
		buzzer_warn_flg[BUZ_PRIORITY_3] |= (Buzz_GearMismatched); //手柄档位和实际档位不一致时，请重新挂挡
		buzzer_warn_flg[BUZ_PRIORITY_4]  = (Buzz_SaftBelt);        //安全带未系
		buzzer_warn_flg[BUZ_PRIORITY_5]  = (AEBS_Warn==3);         //AEBS三级报警
		buzzer_warn_flg[BUZ_PRIORITY_6]  = ((FCW_Level ==2||AEBS_Warn==2)); //FCW碰撞预警二级警告/AEBS二级警告
		buzzer_warn_flg[BUZ_PRIORITY_7]  = ((FCW_Level ==1||AEBS_Warn==1)); //FCW碰撞预警一级警告/AEBS一级警告
		buzzer_warn_flg[BUZ_PRIORITY_8]  = (LDW_Warn == 1||LDW_Warn == 2); //LDW车道偏离预警
		buzzer_warn_flg[BUZ_PRIORITY_9]  = (BSD_Warn>0);          //BSD盲点检测预警
		buzzer_warn_flg[BUZ_PRIORITY_10] = (LED_COFFEE);          //疲劳驾驶
		buzzer_warn_flg[BUZ_PRIORITY_11] = (QYbeep_30S == 1);     //制动系统气压低
		buzzer_warn_flg[BUZ_PRIORITY_12] = (PARK_alarmflag);      //车速大于5km/h，手刹未解除报警
		buzzer_warn_flg[BUZ_PRIORITY_13] = (Waterbeep_180S == 1); //电机冷却温度过高
		buzzer_warn_flg[BUZ_PRIORITY_14] = (BUZZ_STOP);           //系统严重故障
		buzzer_warn_flg[BUZ_PRIORITY_15] = (LED_Disu);            //龟速行驶
		buzzer_warn_flg[BUZ_PRIORITY_16] = (OverSpeedBeep || LmtSpdBeepFlg); //超速报警
		
		buzzer_warn_flg[BUZ_PRIORITY_17] = ((get_dm1_flt_status(DM1_EF, 521702,  0) == true) || (get_dm1_flt_status(DM1_F0, 521702,  0) == true)); //MCU/MCU2:电机转速超速停机保护(521702,0)	
		buzzer_warn_flg[BUZ_PRIORITY_18] = ((get_dm1_flt_status(DM1_EF, 521702, 16) == true) || (get_dm1_flt_status(DM1_F0, 521702, 16) == true)); //MCU/MCU2:电机转速超速报警保护(521702,16)
		buzzer_warn_flg[BUZ_PRIORITY_19] = (LED_WheelDiff);    //轮间差速提示 
		buzzer_warn_flg[BUZ_PRIORITY_20] = (LED_AxleDiff);    //轴间差速提示
		buzzer_warn_flg[BUZ_PRIORITY_21] = (LED_LOCK);    //驾驶室锁止不到位报警开
		buzzer_warn_flg[BUZ_PRIORITY_22] = (LED_PTO); //取力提示
		buzzer_warn_flg[BUZ_PRIORITY_23] = 0; //(LED_LocationLight && LED_DoorOpen); //门开，位置灯未关提醒
		buzzer_warn_flg[BUZ_PRIORITY_24] = (TPMS_Warn > 1); //胎压报警
		buzzer_warn_flg[BUZ_PRIORITY_25] = (LED_EAC||LED_ECAS); //空气悬架充/放气报警
		buzzer_warn_flg[BUZ_PRIORITY_26] = (LED_TurnLeft || LED_TurnRight); //转向提示音
		
		buzzer_warn_flg[BUZ_PRIORITY_27] = (pEPB_18FF3C50_t->handbrake_release == 1); //请踩住脚刹释放驻车
		buzzer_warn_flg[BUZ_PRIORITY_27]|= (pEPB_18FF3C50_t->handbrake_release == 3); //气压低，驻车暂时无法解除
		buzzer_warn_flg[BUZ_PRIORITY_27]|= (pEPB_18FF3C50_t->work_mode == 7); //EPB功能失效，使用车轮楔块
		buzzer_warn_flg[BUZ_PRIORITY_27]|= (pEPB_18FF3C50_t->work_mode == 254); //EPB功能失效，使用车轮楔块
		buzzer_warn_flg[BUZ_PRIORITY_27]|= (pEPB_18FF3C50_t->work_mode == 2); //手动操作EPB开关功能失效
		buzzer_warn_flg[BUZ_PRIORITY_27]|= (pEPB_18FF3C50_t->work_mode == 1); //车间模式，熄火自动驻车制动无法应用
		buzzer_warn_flg[BUZ_PRIORITY_27]|= (eol_epbtype == 2 && can_getPCanRxState(0x18FF3C50) == CAN_FRAME_ST_TIMEOUT); //EPB系统失效
		
		buzzer_warn_flg[BUZ_PRIORITY_28] = (backvalue == 3);
		buzzer_warn_flg[BUZ_PRIORITY_29] = ((backvalue != 0) && (backvalue != 3) && (backvalue != 2));
		
		buzzer_warn_flg[BUZ_PRIORITY_30] = (ADU_Warn == 4 || ADU_Warn == 3); //
		buzzer_warn_flg[BUZ_PRIORITY_31] = (ADU_Warn == 2); //
		buzzer_warn_flg[BUZ_PRIORITY_32] = (ADU_Warn == 1); //
		buzzer_warn_flg[BUZ_PRIORITY_33] = (0); //
		buzzer_warn_flg[BUZ_PRIORITY_34] = (0); //
		buzzer_warn_flg[BUZ_PRIORITY_35] = (0); //
		buzzer_warn_flg[BUZ_PRIORITY_36] = (0); //
		
		buzzer_warn_flg[BUZ_PRIORITY_TEST] = (BUZZ_test);
		
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_1], F_BUZZER_1);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_2], F_BUZZER_2);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_3], F_BUZZER_3);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_4], F_BUZZER_4);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_5], F_BUZZER_5);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_6], F_BUZZER_6);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_7], F_BUZZER_7);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_8], F_BUZZER_8);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_9], F_BUZZER_9);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_10], F_BUZZER_10);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_11], F_BUZZER_11);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_12], F_BUZZER_12);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_13], F_BUZZER_13);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_14], F_BUZZER_14);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_15], F_BUZZER_15);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_16], F_BUZZER_16);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_17], F_BUZZER_17);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_18], F_BUZZER_18);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_19], F_BUZZER_19);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_20], F_BUZZER_20);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_21], F_BUZZER_21);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_22], F_BUZZER_22);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_23], F_BUZZER_23);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_24], F_BUZZER_24);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_25], F_BUZZER_25);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_26], F_BUZZER_26);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_27], F_BUZZER_27);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_28], F_BUZZER_28);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_29], F_BUZZER_29);
		
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_30], F_BUZZER_30);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_31], F_BUZZER_31);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_32], F_BUZZER_32);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_33], F_BUZZER_33);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_34], F_BUZZER_34);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_35], F_BUZZER_35);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_36], F_BUZZER_36);
		
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_TEST], F_BUZZER_TEST);
		
		
			 if(buzzer_warn_flg[BUZ_PRIORITY_1]) {BUZZ = Get_Interval_Status(F_BUZZER_1);  tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_2]) {BUZZ = Get_Interval_Status(F_BUZZER_2);  tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_3]) {BUZZ = Get_Interval_Status(F_BUZZER_3);  tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_4]) {BUZZ = Get_Interval_Status(F_BUZZER_4);  tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_5]) {BUZZ = Get_Interval_Status(F_BUZZER_5);  tonefreqHZ = 1250;toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_6]) {BUZZ = Get_Interval_Status(F_BUZZER_6);  tonefreqHZ = 1250;toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_7]) {BUZZ = Get_Interval_Status(F_BUZZER_7);  tonefreqHZ = 1250;toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_8]) {BUZZ = Get_Interval_Status(F_BUZZER_8);  tonefreqHZ = 4000;toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_9]) {BUZZ = Get_Interval_Status(F_BUZZER_9);  tonefreqHZ = 2500;toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_10]) {BUZZ = Get_Interval_Status(F_BUZZER_10);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_11]) {BUZZ = Get_Interval_Status(F_BUZZER_11);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_12]) {BUZZ = Get_Interval_Status(F_BUZZER_12);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_13]) {BUZZ = Get_Interval_Status(F_BUZZER_13);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_14]) {BUZZ = Get_Interval_Status(F_BUZZER_14);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_15]) {BUZZ = Get_Interval_Status(F_BUZZER_15);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_16]) {BUZZ = Get_Interval_Status(F_BUZZER_16);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_17]) {BUZZ = Get_Interval_Status(F_BUZZER_17);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_18]) {BUZZ = Get_Interval_Status(F_BUZZER_18);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_19]) {BUZZ = Get_Interval_Status(F_BUZZER_19);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_20]) {BUZZ = Get_Interval_Status(F_BUZZER_20);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_21]) {BUZZ = Get_Interval_Status(F_BUZZER_21);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_22]) {BUZZ = Get_Interval_Status(F_BUZZER_22);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_23]) {BUZZ = Get_Interval_Status(F_BUZZER_23);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_24]) {BUZZ = Get_Interval_Status(F_BUZZER_24);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_25]) {BUZZ = Get_Interval_Status(F_BUZZER_25);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_26]) {BUZZ = 1; /*Get_Interval_Status(F_BUZZER_26);*/tonefreqHZ = 800; toneVolume = 5;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_27]) {BUZZ = Get_Interval_Status(F_BUZZER_27);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_28]) {BUZZ = Get_Interval_Status(F_BUZZER_28);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_29]) {BUZZ = Get_Interval_Status(F_BUZZER_29);tonefreqHZ = 800; toneVolume = 50;}
		
		else if(buzzer_warn_flg[BUZ_PRIORITY_30]) {BUZZ = Get_Interval_Status(F_BUZZER_30);tonefreqHZ = 1250;toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_31]) {BUZZ = Get_Interval_Status(F_BUZZER_31);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_32]) {BUZZ = Get_Interval_Status(F_BUZZER_32);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_33]) {BUZZ = Get_Interval_Status(F_BUZZER_33);tonefreqHZ =1250; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_34]) {BUZZ = Get_Interval_Status(F_BUZZER_34);tonefreqHZ =1250; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_35]) {BUZZ = Get_Interval_Status(F_BUZZER_35);tonefreqHZ = 800; toneVolume = 50;}
		
		else if(buzzer_warn_flg[BUZ_PRIORITY_36]) {BUZZ = Get_Interval_Status(F_BUZZER_36);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_TEST]) {BUZZ = Get_Interval_Status(F_BUZZER_TEST);tonefreqHZ = 800; toneVolume = 50;}
		else BUZZ = 0;
		
	}
	else if(CAN_CHARGE_LINE)
	{
		buzzer_warn_flg[BUZ_PRIORITY_1]  = (get_dm1_flt_status(DM1_F4, 521335,  3) == true); //REESS热事件报警
		buzzer_warn_flg[BUZ_PRIORITY_14] = (LED_SysFault); //系统严重故障
		buzzer_warn_flg[BUZ_PRIORITY_23] = (LED_LocationLight && LED_DoorOpen); //门开，位置灯未关提醒
		
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_1], F_BUZZER_1);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_14], F_BUZZER_14);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_23], F_BUZZER_23);
		
			if(buzzer_warn_flg[BUZ_PRIORITY_1]) {BUZZ = Get_Interval_Status(F_BUZZER_1); tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_14]){BUZZ = Get_Interval_Status(F_BUZZER_14);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_23]){BUZZ = Get_Interval_Status(F_BUZZER_23);tonefreqHZ = 800; toneVolume = 50;}
		else BUZZ = 0;
		
	}
	else
	{
		buzzer_warn_flg[BUZ_PRIORITY_1]  = (get_dm1_flt_status(DM1_F4, 521335,  3) == true); //REESS热事件报警
		buzzer_warn_flg[BUZ_PRIORITY_3]  = (GetOffReminderFlg); //下车关电提醒
		buzzer_warn_flg[BUZ_PRIORITY_23] = (LED_LocationLight && LED_DoorOpen); //门开，位置灯未关提醒
		buzzer_warn_flg[BUZ_PRIORITY_26] = (LED_TurnLeft || LED_TurnRight); //转向提示音
		
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_1], F_BUZZER_1);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_3], F_BUZZER_3);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_23], F_BUZZER_23);
		Set_Interval_Req(buzzer_warn_flg[BUZ_PRIORITY_26], F_BUZZER_26);
		
			 if(buzzer_warn_flg[BUZ_PRIORITY_1]) {BUZZ = Get_Interval_Status(F_BUZZER_1); tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_3]) {BUZZ = Get_Interval_Status(F_BUZZER_3); tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_23]){BUZZ = Get_Interval_Status(F_BUZZER_23);tonefreqHZ = 800; toneVolume = 50;}
		else if(buzzer_warn_flg[BUZ_PRIORITY_26]){BUZZ =  1; /*Get_Interval_Status(F_BUZZER_26);*/tonefreqHZ = 800; toneVolume = 5;}
		else BUZZ = 0;
		
	}
	
	if(BUZZ_test == 0)
	{
		if(BUZZ)
		{
			SG_NewTone(tonefreqHZ, toneVolume);
		}
		else
		{
			SoundStop();
		}
	}
	else
	{
		if(Get_Interval_Status(F_BUZZER_TEST))
		{
			SG_NewTone(800, 50);
		}
		else
		{
			SoundStop();
		}
		
	}
	
}
void SYSTME_Logic(void)
{
	LED_Logic();
	Buzzer_Logic();
	
}


void update_motor_point(void)
{
	static ptmrType_t needle_test_tmr = 0;
	
	static double sm_curr = 0;
	static double sm_speed = 0;
	
	double temp = 0;
	
	if(Needle_Test)
	{
		if((ptmr_Get(&needle_test_tmr) >= 6000) || (!ptmr_IsStart(&needle_test_tmr)))
		{
			ptmr_Start(&needle_test_tmr);
			
			if(sm_speed == 120)
			{
				sm_speed = 0;
				sm_curr = 1200;
			}
			else if(sm_speed == 0)
			{
				sm_speed = 120;
				sm_curr = 0;
			}
			else;
			
		}
	}
	else
	{
		sm_speed = get_display_speed(); //get_vehicle_speed();
		
		temp = get_batt_pack_curr();
		if(temp == 0xffff)
			temp = 0;
		else if(temp > 6000)
			temp = 6000;
		else if(temp < -6000)
			temp = -6000;
		else ;
		if(temp > 0)
		{
			if(temp <= 2000)  //600~400
				sm_curr = (600 - temp * 0.2);  //0000 ~ 2000
			else              //400~0
				sm_curr = (300 - temp * 0.05); //2000 ~ 6000
		}
		else
		{
			if(temp >= -2000) //800~600
				sm_curr = (600 - temp * 0.2);  //-2000 ~ -0000
			else              //1200~800
				sm_curr = (900 - temp * 0.05); //-6000 ~ -2000
		}
	}
	
	SetMotSm_SpdTgtPos(sm_speed);
	SetMotSm_CurrTgtPos(sm_curr);
	StepperMotorUpdate();
}

void update_output_pulse(void)
{
	double temp = 0;
	float speed = get_vehicle_speed();
	
	if(!Needle_Test)
	{
		if(speed == 0xff) speed = 0;
		if(speed > 120) speed = 120;
		temp = speed*1.5;   // 1km/h   对应1.5hz   1:1.5
		if(temp>0)
		{              
			Tfin = (unsigned int)(31250/(temp));  //16US 中断一次用于频率输出高的场合              
		}
		else
		{
			Tfin = 0;  
		}
	}
	
}

/** 中断中执行的任务调度函数，对时间要求严格的任务可添加，其他的在Scheduler任务中执行 **/
TASK_COMPONENTS_t  TaskComps[] = 
{
	{Task10ms_can_send , 1},
	{CAN_Send_TCO1, 5},
    {NULL}
};
static void TaskRemarks(void) //计时处理，执行10ms中断
{
    unsigned char i;
	
	TaskComps[0].req = (CAN_TX_ENABLE);
	TaskComps[1].req = (CAN_TX_ENABLE);
	
    for (i = 0; TaskComps[i].TaskHook != NULL; i++) // 逐个任务时间处理
    {
		if(!TaskComps[i].req)
			continue;
		
        if (TaskComps[i].Timer) // 时间不为0
        {
            TaskComps[i].Timer--; // 减去一个节拍
        }
        if (TaskComps[i].Timer == 0) // 时间减完了
        {
            if (TaskComps[i].Run == 0)
			{
                TaskComps[i].Timer = TaskComps[i].ItvTime; // 恢复计时器值，从新下一次
                TaskComps[i].Run = 1; // 任务可以运行
            }
        }
    }
}
static void TaskProcess(void) //执行处理，执行在1ms中断
{
    static uint8_t Flag = 0;
	static uint8_t poll_time = 0;
	
    //任务处理,两个任务间隔取决于poll_time * 运行时间间隔
    for (uint8_t i = 0; TaskComps[i].TaskHook != NULL; i++)  // 逐个任务时间处理
    {
        if (TaskComps[i].Run && Flag)   // 时间不为0
        {
            TaskComps[i].TaskHook();    // 运行任务
            TaskComps[i].Run = 0;       // 标志清0 
            Flag = 0;
        }
    }
	
    if (poll_time > 0)
	{
        poll_time = 0;
        Flag = 1;
    }
	else if (!Flag)
	{
        poll_time++;
    }
	
}


void Interrupt16us(void)
{
    static unsigned int Tcount=0;
    if(Tfin>0)
	{
        if(Tcount>=Tfin)
		{
			Tcount=0;
			//FOUT=!FOUT;
        }
		else Tcount++;
    }
	else
	{
		;//FOUT=0;
    }	
}
void Interrupt1ms(void) 
{
	if(CAN_TX_ENABLE)	
		TaskProcess();
	
	/* if(isCanBusOff(CAN_CH0))
		can_busoff_flg[CAN_CH0] = 1; */
	
	if(isCanBusOff(CAN_CH1))
		can_busoff_flg[CAN_CH1] = 1;
	
	if(isCanBusOff(CAN_CH2))
		can_busoff_flg[CAN_CH2] = 1;
	
}
void Interrupt10ms(void) 
{
    static uint32_t cnt0 = 0;
    static uint32_t cnt1 = 0;
    static uint32_t cnt2 = 0;
    static uint32_t cnt3 = 0;
    static uint32_t cnt4 = 0;
	static uint32_t cnt5 = 0;
	static uint32_t cnt6 = 0;
	static uint32_t count1 = 0;
	static uint32_t count2 = 0;
	static uint32_t count3 = 0;
	static uint32_t count4 = 0;
	static uint32_t drivingcnt=0;
	
	uint8_t Vspeed = get_vehicle_speed();
	
	canBusOffDetectionTask();
    Interval_Count();
	if(CAN_TX_ENABLE && SysInitOKFlag)
	{
		TaskRemarks();
	}
	
	if(count1 < 600 && SysInitOKFlag)
	{
		count1++;
		ADC_Filter(); //开机5s后停止在此处执行
		
		//检测车辆是否带挂车ABS
		if(M_ON && !START && !TrailerABS_CheckFlg && count1 > 99)
		{
			SW_Input();
			if(IN25)
			{
				if(count2 > 4)
				{
					TrailerABS_CheckFlg = 1;
				}
				else count2++;
			}
		}
	}
	
	if(wake_up3)
	{
		KeyProcess();
		if(Vspeed > 3) //车速大于3km/h时候取消按键功能
		{
			key_set = 0;
		}
    }
	
	//报警延时3s标记
	if(alarm_update_flag == 1)
	{
		if(count3 >= 300)
		{
			count3 = 0;	
			alarm_update_flag = 0;
		} 
		else count3++;
	} 
	else count3 = 0;
    
	if(delay_cnt > 0)//延时计数器 
		delay_cnt--; 
	
    if (cnt0 >= 4) //50ms
	{
		cnt0 = 0;
		
		// if(CAN_TX_ENABLE)	
			// CAN_Send_TCO1();
		
		if(!TESTMODE && !LED_test)
		{
			if(START && (M_ON_flag || start_LowPower_Mode || CAN_CHARGE_LINE || GetOffReminderFlg /* || (CAN_KEY_ACC && SmartRechgCfgFlg) */))
			{
				SYSTME_Logic();
			}
		}
		else
		{
			if(cnt4 >= 7) // 0.4s
			{
				cnt4 = 0;
				if(Test_Mode != 0)
				{
					Test_Needle();
				}
				Test_LED();
			}
			else cnt4++;
		}
    }
	else cnt0++;
	
	if(!M_ON && GetOffReminderStatus)
	{
		if(count4 >= 999)
		{
			GetOffReminderFlg = 0;
		}
		else 
		{
			GetOffReminderFlg = 1;
			count4++;
		}
	}
	else
	{
		GetOffReminderStatus = 0;
		GetOffReminderFlg = 0;
		count4 = 0;
	}
	
	if(wake_up3) 
	{
        if(cnt1 > 9)
        {
			M_ON = 1;
		}	
        else cnt1++;
		
		if(M_ON && Test_Mode != 0)	//test mode add 3s ign off delay
		{
	    	cnt1 = 300;
		}
		start_LowPower_Mode = 0;
    } 
	else 
	{
        if (cnt1 < 1) 
		{
			M_ON = 0;
		}
        else cnt1--;
    }
	
	if(M_ON_flag || CAN_WAKE_FLG)
	{
		CAN_TX_ENABLE = 1;
		
		cnt5 = 0;
		cnt6 = 0;
	}
	else
	{
		if(wake_up3
		|| CAN_CHARGE_LINE 
		|| CAN_H2_ChuiSao 
		|| CAN_H2_LowTptChg 
		/* || (CAN_KEY_ACC && SmartRechgCfgFlg) */)
		{
			if(cnt6 > 5)
				CAN_TX_ENABLE = 1;
			else
				cnt6++;
			
			cnt5 = 0;
		}
		else
		{
			cnt6 = 0;
			
			if(cnt5 >= 1000)
			{
				CAN_TX_ENABLE = 0;
			}
			else cnt5++;
		}
	}
	
	if(wake_can_cnt >= 1000)
	{
		wake_can = 0;
		wake_can_cnt = 1000;
    }
    else wake_can_cnt++;
	
	if(Vspeed > 3)
	{
		if(cnt2 >= 100) //1s
		{
			cnt2 = 0;
			if(Miles.single_miles > 0)
				driving_time++; //累计计时，单位秒
			else
				driving_time = 0;
			
			drivingcnt++;
			if(drivingcnt > 1800)//0.5h
			{
				saveRunTimeflag = 1;
				drivingcnt = 0;
			}
		}
		else cnt2++;
		
		if(cnt3 >= 49) //500ms
		{
			cnt3 = 0;
			Miles.single_time++;
		}
		else cnt3++;
		
	}
	else 
	{
		cnt2 = 0;
		cnt3 = 0;
	}
	
}
void Interrupt100ms(void) 
{
	if(M_ON_flag && SysInitOKFlag)
	{
		Calculate_Miles();
		if(eol_fadongjiorkaji == 0x2d)
			Calculate_H2Consp();
	}
	
	if(CAN_TX_ENABLE && SysInitOKFlag)
		Task100ms_can_send();
}


void freqcalcute_I05(void)
{
    fin2_counter++;
    TAUB0.CDR5 =0; 
}
void calculate_fin(void) //call this function in 1s interrupt routine
{
    freq[1] = fin2_counter;
    fin2_counter = 0;
}


void reset_mcu(void)
{
    //R_DEV_WRITE_PROT_REG(0xfff80000u, 0xfff80a04ul, 0x00000001) //software reset
    while(1);
}
void software_reset(void)
{
    R_DEV_WRITE_PROT_REG(0xfff80000u, 0xfff80a04ul, 0x00000001) //software reset

}




