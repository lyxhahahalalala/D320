#include "SYSTEM.h"
#include "string.h"
#include "r_bsp_stdio_api.h"
#include "r_dev_api.h"
#include "Motor_sm.h"

DATE timer;
uint32_t Fin1[10];
uint32_t Fin2[10];

uint8_t  CAN1_LIFE=0;
uint8_t  CAN2_LIFE=0;

uint8_t  ADCounter;
uint8_t  FinCounter;
uint32_t life_pcan;
uint32_t life_bcan;
uint8_t  RecCommand = 0;


DATA_BIT Gennal_Use[3];
AD_DATA  g_ad_data_t;
IO_BIT   g_io_fonfigure;
AD_BIT   g_ad_configure;
AD_VOL   g_ad_voltage;
FREQ_OUT g_freq;

uint8_t  error_type;
uint8_t  etu_type_io;
uint8_t  etu_type_ad;
uint8_t  num;

uint32_t freq[2];
uint32_t Tfin=0;
uint32_t fin1_counter;
uint32_t fin2_counter;
uint32_t Fin1Value;
uint32_t Fin2Value;

#define FREQ_MCU_OUT1       (PORT_ISO.P16.BIT.P16_0)
#define FREQ_MCU_OUT2       (PORT_ISO.P16.BIT.P16_1)


extern void R_TICK_WaitMS(uint32_t Unit, uint32_t TickMS) ;



void GPIO_Init(void)
{
	
	//p3-9 buzz
    PORT_ISO.PM3.UINT16                &= ~0x0200;
    PORT_ISO.PMC3.UINT16              &= ~0x0200;             //P3_9  	 IGN
    PORT_ISO.P3.UINT16 |=  (1<<9);        		        //ign on
    
	//spi chips control port pins
    PORT_ISO.PM21.UINT16              &= ~0x03ff;
    PORT_ISO.PMC21.UINT16            &=  ~0x03ff;   //P21_0-9 output mode
	
}


//系统初始化函数
void System_Init(void) {
    RS_CAN_Init();	
	// ADC_Init();
    WDT_Init();
    WDT_Clear();
	// TAUB_Init(); // 外部事件触发
    memset(&g_io_fonfigure,0,5);
    // CONFIG_IO();
    if(0x12345678 != EEP_ReadFlag())
    {
		EEP_WriteFlag();
		EEROM_WriteSpeedRatio(1) ;
    }

	RecCommand = 5; //EEROM_ReadSpeedRatio();

}
#define OUTLOW 0
#define OUTHIGH 1
#define OUTOC 2
uint16_t pSpeed = 0;
uint16_t current = 6000;
uint8_t temperature = 100;
uint16_t voltage = 5500;
static uint32_t testStep = 0;
void mode1Config(void){
	CONFIG_IO();
	if(RecCommand!=5){
		pSpeed = 60;
		current = 6000;
		temperature = 100;
		voltage = 5500;
	}
}

void SYSTME_Logic(void) {
	static uint8_t oldrec = 0;
	//RecCommand = 1;
	if((oldrec != RecCommand)||(RecCommand == 5))
	{
		if(RecCommand == 1)//软件模式1
		{
			g_io_fonfigure.C1_GEARBOX_QULI_CTRL= OUTLOW;//变速箱取力指示 低边输入
			g_io_fonfigure.C2_REAR_LIGHT_CTRL= OUTHIGH;//后照灯指示	高边输入
			g_io_fonfigure.C3_RESERVE_CTRL= OUTLOW;//预留	高低边兼容
			g_io_fonfigure.C4_OK_CTRL= OUTHIGH;
			g_io_fonfigure.C5_CANCEL_CTRL= OUTHIGH;
			g_io_fonfigure.C6_UP_CTRL= OUTHIGH;
			g_io_fonfigure.C7_DOWN_CTRL= OUTHIGH;
			g_io_fonfigure.C8_HI_LEVEL_CTRL= OUTLOW;//高档开关	低边输入
			g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL= OUTHIGH;//进气预热开关	高边输入	输入 
			g_io_fonfigure.C10_TRAILER_ABS_CTRL= OUTLOW;//挂车ABS指示	低边输入	输入 
			g_io_fonfigure.C11_AXLE_DIFF_CTRL= OUTLOW;//轴差开关	低边输入
			g_io_fonfigure.C12_WHEEL_DIFF_CTRL= OUTLOW;//轮差开关	低边输入
			g_io_fonfigure.C13_VBAT_SW_CTRL= OUTHIGH;//POWER	高边输入	输入
			g_io_fonfigure.C14_Emergency_shutdown_CTRL= OUTLOW;//紧急切断	低边输入
			g_io_fonfigure.C15_QULI_INDICTOR_CTRL= OUTLOW;//取力指示灯	低边
			g_io_fonfigure.C16_BRAKE_SW_L1_CTRL= OUTLOW;//预留-制动蹄片磨损L1	低边输入
			g_io_fonfigure.C17_BRAKE_SW_L2_CTRL= OUTLOW;
			g_io_fonfigure.C18_BRAKE_SW_L3_CTRL= OUTLOW;
			g_io_fonfigure.C19_BRAKE_SW_L4_CTRL= OUTLOW;
			g_io_fonfigure.C20_BRAKE_SW_R1_CTRL= OUTLOW;
			g_io_fonfigure.C21_BRAKE_SW_R2_CTRL= OUTLOW;
			g_io_fonfigure.C22_BRAKE_SW_R3_CTRL= OUTLOW;
			g_io_fonfigure.C23_BRAKE_SW_R4_CTRL= OUTLOW;
			g_io_fonfigure.C24_PARK_CTRL= OUTHIGH;//驻车制动开关	低边输入-------------
			g_io_fonfigure.C25_BCM_CTRL= OUTHIGH;//BCM跛行回家开关	高边输入 	输入 
			g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL= OUTLOW; //乘客侧安全带开关	低边输入
			g_io_fonfigure.A7_AIR_FILTER_CTRL= OUTLOW;//空滤堵塞报警开关	低边输入
			g_io_fonfigure.A8_DRIVER_DOOR_CTRL= OUTLOW;//驾驶室锁止不到位报警开关	低边输入
			g_io_fonfigure.A9_SAFETY_BELT_CTRL= OUTLOW;//安全带指示开关	低边输入
			g_io_fonfigure.A10_LOW_GEAR_CTRL = OUTLOW;//低档开关	低边输入
			g_io_fonfigure.A22_DRYER_CTRL= OUTHIGH;//干燥器开关	高边输入
			g_io_fonfigure.A23_AIR_SUSPENSION_CTRL= OUTHIGH;//空气悬架开关	高边输入
			g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL= OUTHIGH;//浮动桥报警开关	高边输入
			g_io_fonfigure.A25_RESERVE_CTRL= OUTHIGH;//预留-开关信号	高边输入
			g_io_fonfigure.A26_RECHARGE_CTRL= OUTHIGH;//充电指示	低边输入
			pSpeed = 60;
			current = 6000;
			temperature = 100;
			voltage = 5500;
		}
		else if(RecCommand == 2)//chuandao
		{
			g_io_fonfigure.C1_GEARBOX_QULI_CTRL= OUTLOW;//变速箱取力指示 低边输入
			g_io_fonfigure.C2_REAR_LIGHT_CTRL= OUTHIGH;//后照灯指示	高边输入
			g_io_fonfigure.C3_RESERVE_CTRL= OUTLOW;//预留	高低边兼容
			g_io_fonfigure.C4_OK_CTRL= OUTHIGH;
			g_io_fonfigure.C5_CANCEL_CTRL= OUTHIGH;
			g_io_fonfigure.C6_UP_CTRL= OUTHIGH;
			g_io_fonfigure.C7_DOWN_CTRL= OUTHIGH;
			g_io_fonfigure.C8_HI_LEVEL_CTRL= OUTLOW;//高档开关	低边输入
			g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL= OUTHIGH;//进气预热开关	高边输入	输入 
			g_io_fonfigure.C10_TRAILER_ABS_CTRL= OUTLOW;//挂车ABS指示	低边输入	输入 
			g_io_fonfigure.C11_AXLE_DIFF_CTRL= OUTLOW;//轴差开关	低边输入
			g_io_fonfigure.C12_WHEEL_DIFF_CTRL= OUTLOW;//轮差开关	低边输入
			g_io_fonfigure.C13_VBAT_SW_CTRL= OUTHIGH;//POWER	高边输入	输入
			g_io_fonfigure.C14_Emergency_shutdown_CTRL= OUTLOW;//紧急切断	低边输入
			g_io_fonfigure.C15_QULI_INDICTOR_CTRL= OUTLOW;//取力指示灯	低边
			g_io_fonfigure.C16_BRAKE_SW_L1_CTRL= OUTLOW;//预留-制动蹄片磨损L1	低边输入
			g_io_fonfigure.C17_BRAKE_SW_L2_CTRL= OUTLOW;
			g_io_fonfigure.C18_BRAKE_SW_L3_CTRL= OUTLOW;
			g_io_fonfigure.C19_BRAKE_SW_L4_CTRL= OUTLOW;
			g_io_fonfigure.C20_BRAKE_SW_R1_CTRL= OUTLOW;
			g_io_fonfigure.C21_BRAKE_SW_R2_CTRL= OUTLOW;
			g_io_fonfigure.C22_BRAKE_SW_R3_CTRL= OUTLOW;
			g_io_fonfigure.C23_BRAKE_SW_R4_CTRL= OUTLOW;
			g_io_fonfigure.C24_PARK_CTRL= OUTHIGH;//驻车制动开关	低边输入-------------
			g_io_fonfigure.C25_BCM_CTRL= OUTHIGH;//BCM跛行回家开关	高边输入 	输入 
			g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL= OUTLOW; //乘客侧安全带开关	低边输入
			g_io_fonfigure.A7_AIR_FILTER_CTRL= OUTLOW;//空滤堵塞报警开关	低边输入
			g_io_fonfigure.A8_DRIVER_DOOR_CTRL= OUTLOW;//驾驶室锁止不到位报警开关	低边输入
			g_io_fonfigure.A9_SAFETY_BELT_CTRL= OUTLOW;//安全带指示开关	低边输入
			g_io_fonfigure.A10_LOW_GEAR_CTRL = OUTLOW;//低档开关	低边输入
			g_io_fonfigure.A22_DRYER_CTRL= OUTHIGH;//干燥器开关	高边输入
			g_io_fonfigure.A23_AIR_SUSPENSION_CTRL= OUTHIGH;//空气悬架开关	高边输入
			g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL= OUTHIGH;//浮动桥报警开关	高边输入
			g_io_fonfigure.A25_RESERVE_CTRL= OUTHIGH;//预留-开关信号	高边输入
			g_io_fonfigure.A26_RECHARGE_CTRL= OUTHIGH;//充电指示	低边输入
			pSpeed = 60;
			current = 6000;
			temperature = 100;
			voltage = 5500;

		}
		else if(RecCommand == 3)//naijiu
		{

			g_io_fonfigure.C1_GEARBOX_QULI_CTRL= OUTHIGH;//变速箱取力指示 低边输入
			g_io_fonfigure.C2_REAR_LIGHT_CTRL= OUTLOW;//后照灯指示	高边输入
			g_io_fonfigure.C3_RESERVE_CTRL= OUTHIGH;//预留	高低边兼容
			g_io_fonfigure.C4_OK_CTRL= OUTHIGH;
			g_io_fonfigure.C5_CANCEL_CTRL= OUTHIGH;
			g_io_fonfigure.C6_UP_CTRL= OUTHIGH;
			g_io_fonfigure.C7_DOWN_CTRL= OUTHIGH;
			g_io_fonfigure.C8_HI_LEVEL_CTRL= OUTHIGH;//高档开关	低边输入
			g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL= OUTLOW;//进气预热开关	高边输入	输入 
			g_io_fonfigure.C10_TRAILER_ABS_CTRL= OUTHIGH;//挂车ABS指示	低边输入	输入 
			g_io_fonfigure.C11_AXLE_DIFF_CTRL= OUTHIGH;//轴差开关	低边输入
			g_io_fonfigure.C12_WHEEL_DIFF_CTRL= OUTHIGH;//轮差开关	低边输入
			g_io_fonfigure.C13_VBAT_SW_CTRL= OUTLOW;//POWER	高边输入	输入
			g_io_fonfigure.C14_Emergency_shutdown_CTRL= OUTHIGH;//紧急切断	低边输入
			g_io_fonfigure.C15_QULI_INDICTOR_CTRL= OUTLOW;//取力指示灯	h
			g_io_fonfigure.C16_BRAKE_SW_L1_CTRL= OUTHIGH;//预留-制动蹄片磨损L1	低边输入
			g_io_fonfigure.C17_BRAKE_SW_L2_CTRL= OUTHIGH;
			g_io_fonfigure.C18_BRAKE_SW_L3_CTRL= OUTHIGH;
			g_io_fonfigure.C19_BRAKE_SW_L4_CTRL= OUTHIGH;
			g_io_fonfigure.C20_BRAKE_SW_R1_CTRL= OUTHIGH;
			g_io_fonfigure.C21_BRAKE_SW_R2_CTRL= OUTHIGH;
			g_io_fonfigure.C22_BRAKE_SW_R3_CTRL= OUTHIGH;
			g_io_fonfigure.C23_BRAKE_SW_R4_CTRL= OUTHIGH;
			g_io_fonfigure.C24_PARK_CTRL= OUTHIGH;//驻车制动开关	低边输入
			g_io_fonfigure.C25_BCM_CTRL= OUTLOW;//BCM跛行回家开关	高边输入 	输入 
			g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL= OUTHIGH; //乘客侧安全带开关	低边输入
			g_io_fonfigure.A7_AIR_FILTER_CTRL= OUTHIGH;//空滤堵塞报警开关	低边输入
			g_io_fonfigure.A8_DRIVER_DOOR_CTRL= OUTHIGH;//驾驶室锁止不到位报警开关	低边输入
			g_io_fonfigure.A9_SAFETY_BELT_CTRL= OUTHIGH;//安全带指示开关	低边输入
			g_io_fonfigure.A10_LOW_GEAR_CTRL = OUTHIGH;//低档开关	低边输入
			g_io_fonfigure.A22_DRYER_CTRL= OUTLOW;//干燥器开关	高边输入
			g_io_fonfigure.A23_AIR_SUSPENSION_CTRL= OUTLOW;//空气悬架开关	高边输入
			g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL= OUTLOW;//浮动桥报警开关	高边输入
			g_io_fonfigure.A25_RESERVE_CTRL= OUTLOW;//预留-开关信号	高边输入
			g_io_fonfigure.A26_RECHARGE_CTRL= OUTHIGH;//充电指示	低边输入
		}
		else if(RecCommand == 4)//diwen
		{
			g_io_fonfigure.C1_GEARBOX_QULI_CTRL= OUTLOW;//变速箱取力指示 低边输入
			g_io_fonfigure.C2_REAR_LIGHT_CTRL= OUTHIGH;//后照灯指示	高边输入
			g_io_fonfigure.C3_RESERVE_CTRL= OUTLOW;//预留	高低边兼容
			g_io_fonfigure.C4_OK_CTRL= OUTHIGH;
			g_io_fonfigure.C5_CANCEL_CTRL= OUTHIGH;
			g_io_fonfigure.C6_UP_CTRL= OUTHIGH;
			g_io_fonfigure.C7_DOWN_CTRL= OUTHIGH;
			g_io_fonfigure.C8_HI_LEVEL_CTRL= OUTLOW;//高档开关	低边输入
			g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL= OUTHIGH;//进气预热开关	高边输入	输入 
			g_io_fonfigure.C10_TRAILER_ABS_CTRL= OUTLOW;//挂车ABS指示	低边输入	输入 
			g_io_fonfigure.C11_AXLE_DIFF_CTRL= OUTLOW;//轴差开关	低边输入
			g_io_fonfigure.C12_WHEEL_DIFF_CTRL= OUTLOW;//轮差开关	低边输入
			g_io_fonfigure.C13_VBAT_SW_CTRL= OUTHIGH;//POWER	高边输入	输入
			g_io_fonfigure.C14_Emergency_shutdown_CTRL= OUTLOW;//紧急切断	低边输入
			g_io_fonfigure.C15_QULI_INDICTOR_CTRL= OUTHIGH;//取力指示灯	h
			g_io_fonfigure.C16_BRAKE_SW_L1_CTRL= OUTLOW;//预留-制动蹄片磨损L1	低边输入
			g_io_fonfigure.C17_BRAKE_SW_L2_CTRL= OUTLOW;
			g_io_fonfigure.C18_BRAKE_SW_L3_CTRL= OUTLOW;
			g_io_fonfigure.C19_BRAKE_SW_L4_CTRL= OUTLOW;
			g_io_fonfigure.C20_BRAKE_SW_R1_CTRL= OUTLOW;
			g_io_fonfigure.C21_BRAKE_SW_R2_CTRL= OUTLOW;
			g_io_fonfigure.C22_BRAKE_SW_R3_CTRL= OUTLOW;
			g_io_fonfigure.C23_BRAKE_SW_R4_CTRL= OUTLOW;
			g_io_fonfigure.C24_PARK_CTRL= OUTHIGH;//驻车制动开关	低边输入
			g_io_fonfigure.C25_BCM_CTRL= OUTHIGH;//BCM跛行回家开关	高边输入 	输入 
			g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL= OUTLOW; //乘客侧安全带开关	低边输入
			g_io_fonfigure.A7_AIR_FILTER_CTRL= OUTLOW;//空滤堵塞报警开关	低边输入
			g_io_fonfigure.A8_DRIVER_DOOR_CTRL= OUTLOW;//驾驶室锁止不到位报警开关	低边输入
			g_io_fonfigure.A9_SAFETY_BELT_CTRL= OUTLOW;//安全带指示开关	低边输入
			g_io_fonfigure.A10_LOW_GEAR_CTRL = OUTLOW;//低档开关	低边输入
			g_io_fonfigure.A22_DRYER_CTRL= OUTHIGH;//干燥器开关	高边输入
			g_io_fonfigure.A23_AIR_SUSPENSION_CTRL= OUTHIGH;//空气悬架开关	高边输入
			g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL= OUTHIGH;//浮动桥报警开关	高边输入
			g_io_fonfigure.A25_RESERVE_CTRL= OUTHIGH;//预留-开关信号	高边输入
			g_io_fonfigure.A26_RECHARGE_CTRL= OUTLOW;//充电指示	低边输入
		}
		else if(RecCommand == 5)//testing for mp
		{
			if(F_6s){
			g_io_fonfigure.C1_GEARBOX_QULI_CTRL= OUTHIGH;
			g_io_fonfigure.C2_REAR_LIGHT_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C3_RESERVE_CTRL= OUTHIGH;
			g_io_fonfigure.C4_OK_CTRL= OUTHIGH;
			g_io_fonfigure.C5_CANCEL_CTRL= OUTHIGH;
			g_io_fonfigure.C6_UP_CTRL= OUTHIGH;
			g_io_fonfigure.C7_DOWN_CTRL= OUTHIGH;
			g_io_fonfigure.C8_HI_LEVEL_CTRL= OUTHIGH;
			g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL= OUTHIGH;
			g_io_fonfigure.C10_TRAILER_ABS_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C11_AXLE_DIFF_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C12_WHEEL_DIFF_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C13_VBAT_SW_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C14_Emergency_shutdown_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C15_QULI_INDICTOR_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C16_BRAKE_SW_L1_CTRL= OUTHIGH;
			g_io_fonfigure.C17_BRAKE_SW_L2_CTRL= OUTHIGH;
			g_io_fonfigure.C18_BRAKE_SW_L3_CTRL= OUTHIGH;
			g_io_fonfigure.C19_BRAKE_SW_L4_CTRL= OUTHIGH;
			g_io_fonfigure.C20_BRAKE_SW_R1_CTRL= OUTHIGH;
			g_io_fonfigure.C21_BRAKE_SW_R2_CTRL= OUTHIGH;
			g_io_fonfigure.C22_BRAKE_SW_R3_CTRL= OUTHIGH;
			g_io_fonfigure.C23_BRAKE_SW_R4_CTRL= OUTHIGH;
			g_io_fonfigure.C24_PARK_CTRL= OUTHIGH;//LED
			g_io_fonfigure.C25_BCM_CTRL= OUTHIGH;
			g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL= OUTHIGH; //LED
			g_io_fonfigure.A7_AIR_FILTER_CTRL= OUTHIGH;//active
			g_io_fonfigure.A8_DRIVER_DOOR_CTRL= OUTHIGH;//active   LED
			g_io_fonfigure.A9_SAFETY_BELT_CTRL= OUTHIGH;//active   LED
			g_io_fonfigure.A10_LOW_GEAR_CTRL = OUTHIGH;//active
			g_io_fonfigure.A22_DRYER_CTRL= OUTHIGH;//active   LED
			g_io_fonfigure.A23_AIR_SUSPENSION_CTRL= OUTHIGH;//active  LED
			g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL= OUTHIGH;//active
			g_io_fonfigure.A25_RESERVE_CTRL= OUTHIGH;//active
			g_io_fonfigure.A26_RECHARGE_CTRL= OUTHIGH;//active
			pSpeed = 120;
			current = 12000;
			temperature = 100;
			voltage = 5500;
			}
			else{
			g_io_fonfigure.C1_GEARBOX_QULI_CTRL= OUTLOW;
			g_io_fonfigure.C2_REAR_LIGHT_CTRL= OUTLOW;//LED
			g_io_fonfigure.C3_RESERVE_CTRL= OUTLOW;
			g_io_fonfigure.C4_OK_CTRL= OUTLOW;
			g_io_fonfigure.C5_CANCEL_CTRL= OUTLOW;
			g_io_fonfigure.C6_UP_CTRL= OUTLOW;
			g_io_fonfigure.C7_DOWN_CTRL= OUTLOW;
			g_io_fonfigure.C8_HI_LEVEL_CTRL= OUTLOW;
			g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL= OUTLOW;
			g_io_fonfigure.C10_TRAILER_ABS_CTRL= OUTLOW;//LED
			g_io_fonfigure.C11_AXLE_DIFF_CTRL= OUTLOW;//LED
			g_io_fonfigure.C12_WHEEL_DIFF_CTRL= OUTLOW;//LED
			g_io_fonfigure.C13_VBAT_SW_CTRL= OUTLOW;//LED
			g_io_fonfigure.C14_Emergency_shutdown_CTRL= OUTLOW;//LED
			g_io_fonfigure.C15_QULI_INDICTOR_CTRL= OUTLOW;//LED
			g_io_fonfigure.C16_BRAKE_SW_L1_CTRL= OUTLOW;
			g_io_fonfigure.C17_BRAKE_SW_L2_CTRL= OUTLOW;
			g_io_fonfigure.C18_BRAKE_SW_L3_CTRL= OUTLOW;
			g_io_fonfigure.C19_BRAKE_SW_L4_CTRL= OUTLOW;
			g_io_fonfigure.C20_BRAKE_SW_R1_CTRL= OUTLOW;
			g_io_fonfigure.C21_BRAKE_SW_R2_CTRL= OUTLOW;
			g_io_fonfigure.C22_BRAKE_SW_R3_CTRL= OUTLOW;
			g_io_fonfigure.C23_BRAKE_SW_R4_CTRL= OUTLOW;
			g_io_fonfigure.C24_PARK_CTRL= OUTLOW;//LED
			g_io_fonfigure.C25_BCM_CTRL= OUTLOW;
			g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL= OUTLOW; //LED
			g_io_fonfigure.A7_AIR_FILTER_CTRL= OUTLOW;//active
			g_io_fonfigure.A8_DRIVER_DOOR_CTRL= OUTLOW;//active   LED
			g_io_fonfigure.A9_SAFETY_BELT_CTRL= OUTLOW;//active   LED
			g_io_fonfigure.A10_LOW_GEAR_CTRL = OUTLOW;//active
			g_io_fonfigure.A22_DRYER_CTRL= OUTLOW;//active   LED
			g_io_fonfigure.A23_AIR_SUSPENSION_CTRL= OUTLOW;//active  LED
			g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL= OUTLOW;//active
			g_io_fonfigure.A25_RESERVE_CTRL= OUTLOW;//active
			g_io_fonfigure.A26_RECHARGE_CTRL= OUTLOW;//active
			pSpeed = 0;
			current = 0;
			temperature = 0;
			voltage = 0;
			}
		}
		else;
		
		mode1Config();
		oldrec = RecCommand;
	}

}

void Count_ADR05(void)
{
    static uint32_t AD_TOTAL_VALUE[10];
    unsigned char cnt;
    AD_TOTAL_VALUE[0] = 0;
    AD_TOTAL_VALUE[1] = 0;
    AD_TOTAL_VALUE[2] = 0;
    AD_TOTAL_VALUE[3] = 0;
    AD_TOTAL_VALUE[4] = 0;
    AD_TOTAL_VALUE[5] = 0;
    AD_TOTAL_VALUE[6] = 0;
    AD_TOTAL_VALUE[7] = 0;
    AD_TOTAL_VALUE[8] = 0;
    AD_TOTAL_VALUE[9] = 0;
    for(cnt = 0;cnt<20;cnt++)
    {
	AD_TOTAL_VALUE[0] +=  g_ad_data_t.AD_PWR_OUT1_DET[cnt] ;
	AD_TOTAL_VALUE[1] +=  g_ad_data_t.AD_PWR_OUT2_DET[cnt] ;
	AD_TOTAL_VALUE[2] +=  g_ad_data_t.AD_PWR_OUT3_DET[cnt] ;
	AD_TOTAL_VALUE[3] +=  g_ad_data_t.AD_PWR_OUT4_DET[cnt] ;
	AD_TOTAL_VALUE[4] +=  g_ad_data_t.AD_299BACKIP_DET[cnt] ;
	AD_TOTAL_VALUE[5] +=  g_ad_data_t.AD_299TURN_RIGHT_DET[cnt] ;
	AD_TOTAL_VALUE[6] +=  g_ad_data_t.AD_299TURN_LEFT_DET[cnt] ;
	AD_TOTAL_VALUE[7] +=  g_ad_data_t.AD_VCC_12V0_DET[cnt] ;
	AD_TOTAL_VALUE[8] +=  g_ad_data_t.AD_VCC_12V0_OUT1_DET[cnt] ;
	AD_TOTAL_VALUE[9] +=  g_ad_data_t.AD_AD_VCC_DET[cnt] ;


   }
	g_ad_data_t.AD_PWR_OUT1_DET_OUTPUT = AD_TOTAL_VALUE[0]/20;
	g_ad_data_t.AD_PWR_OUT2_DET_OUTPUT = AD_TOTAL_VALUE[1]/20;
	g_ad_data_t.AD_PWR_OUT3_DET_OUTPUT = AD_TOTAL_VALUE[2]/20;
	g_ad_data_t.AD_PWR_OUT4_DET_OUTPUT = AD_TOTAL_VALUE[3]/20;
	g_ad_data_t.AD_299BACKIP_DET_OUTPUT= AD_TOTAL_VALUE[4]/20;
	g_ad_data_t.AD_299TURN_RIGHT_DET_OUTPUT= AD_TOTAL_VALUE[5]/20;
	g_ad_data_t.AD_299TURN_LEFT_DET_OUTPUT= AD_TOTAL_VALUE[6]/20;
	g_ad_data_t.AD_VCC_12V0_DET_OUTPUT= AD_TOTAL_VALUE[7]/20;
	g_ad_data_t.AD_VCC_12V0_OUT1_DET_OUTPUT= AD_TOTAL_VALUE[8]/20;
	g_ad_data_t.AD_AD_VCC_DET_OUTPUT= AD_TOTAL_VALUE[9]/20;



}

#define COEF 550
void Count_ADR_ALL(void)
{
    unsigned int  ad = 4096;
    unsigned int temp;

    
    for(uint8_t i = 0; i<10; i++){//read 10 channel raw data
		if(i<9){
	       		 temp = ADC_ReadChannelValue(i);
			    temp = (unsigned int) ((462.0 * temp) / ad );
		}
		else{
			 temp = ADC_ReadChannelValue(11);
			 temp = (unsigned int) ((66.0 * temp) / ad );
		}
			if(i == 0)
	      			  g_ad_data_t.AD_PWR_OUT1_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 1)
	      			  g_ad_data_t.AD_PWR_OUT2_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 2)
	      			  g_ad_data_t.AD_PWR_OUT3_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 3)
	      			  g_ad_data_t.AD_PWR_OUT4_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 4)
	      			  g_ad_data_t.AD_299BACKIP_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 5)
	      			  g_ad_data_t.AD_299TURN_RIGHT_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 6)
	      			  g_ad_data_t.AD_299TURN_LEFT_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 7)
	      			  g_ad_data_t.AD_VCC_12V0_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 8)
	      			  g_ad_data_t.AD_VCC_12V0_OUT1_DET[g_ad_data_t.ad_cnt] = temp;
			else 	if(i == 9)
				 g_ad_data_t.AD_AD_VCC_DET[g_ad_data_t.ad_cnt] = temp;
			else;
    }

	  
	if(g_ad_data_t.ad_cnt<20)g_ad_data_t.ad_cnt++;
	else g_ad_data_t.ad_cnt = 0;

  

}

void Count_Fin100ms(void)
{
    Fin1[FinCounter] 	=   fin1_counter;
    Fin2[FinCounter++]  =   fin2_counter;
    if(FinCounter>9)FinCounter = 0;
    fin1_counter = 0;
    fin2_counter = 0;
	   
}
void Count_FinFilter(void)
{
    unsigned char cnt;
    for(cnt = 0;cnt<10;cnt++)
    {
	Fin1Value += Fin1[cnt];
	Fin2Value += Fin2[cnt];
    }

    freq[0]= Fin1Value ;
    freq[1] = Fin2Value;	

    Fin1Value = 0;
    Fin2Value = 0;

}


void BCAN_IO_CONFIGURED(void) {
    CAN_MESSAGE message_POUT;
    
    if(g_bit_1){
		g_bit_1 = 0;
		message_POUT.id = 0x701;
		message_POUT.data[0] = 0x88;
		message_POUT.data[1] = 0x01;
		message_POUT.data[2] = num;
		message_POUT.data[3] = 0;
		message_POUT.data[4] = 0;
		message_POUT.data[5] = 0;
		message_POUT.data[6] = etu_type_io;
		message_POUT.data[7] = 0;
		message_POUT.len = 8;
		message_POUT.type = 1;
		CAN1_SendMessage(&message_POUT);//522
		CAN2_SendMessage(&message_POUT);//d94
	}
	else;
}

void BCAN_AD_CONFIGURED(void) {
    CAN_MESSAGE msg;
	if(g_bit_2){
		g_bit_2 = 0;
		msg.data[0] = 0x88; 
		msg.data[1] =0x02; 
		msg.data[2] =0; 
		msg.data[3] = 0; 
		msg.data[4] = 0;
		msg.data[5] = 0;
		msg.data[6] = etu_type_ad;
		msg.data[7] = 0;
		msg.id = 0x702;
		msg.len = 0x08;
		msg.type = 0x01;
		CAN1_SendMessage(&msg);//522
	//    CAN2_SendMessage(&msg);//d94
	}
	else;

}
void BCAN_SEND_AD1(void) {
    CAN_MESSAGE msg;
    g_ad_voltage.voltage_pwr_out1_det = g_ad_data_t.AD_PWR_OUT1_DET_OUTPUT;
    g_ad_voltage.voltage_pwr_out1_det=	g_ad_data_t.AD_PWR_OUT2_DET_OUTPUT;
    g_ad_voltage.voltage_pwr_out3_det=	g_ad_data_t.AD_PWR_OUT3_DET_OUTPUT;
    g_ad_voltage.voltage_pwr_out4_det = g_ad_data_t.AD_PWR_OUT4_DET_OUTPUT;

    msg.data[0] = (uint8_t)g_ad_voltage.voltage_pwr_out1_det; 
    msg.data[1] = (uint8_t)(g_ad_voltage.voltage_pwr_out1_det>>8); 
    msg.data[2] = (uint8_t)g_ad_voltage.voltage_pwr_out2_det;
    msg.data[3] = (uint8_t)(g_ad_voltage.voltage_pwr_out2_det>>8); 
    msg.data[4] = (uint8_t)g_ad_voltage.voltage_pwr_out3_det; 
    msg.data[5] = (uint8_t)(g_ad_voltage.voltage_pwr_out3_det>>8); 
    msg.data[6] = (uint8_t)g_ad_voltage.voltage_pwr_out4_det; 
    msg.data[7] = (uint8_t)(g_ad_voltage.voltage_pwr_out4_det>>8); 
	

    msg.id = 0x703;
    msg.len = 0x08;
    msg.type = 0x01;
    CAN1_SendMessage(&msg);//522
   // CAN2_SendMessage(&msg);//d94
}

void BCAN_SEND_AD2(void) {
    CAN_MESSAGE msg;

    g_ad_voltage.voltage_299bakeup_det=g_ad_data_t.AD_299BACKIP_DET_OUTPUT;
    g_ad_voltage.voltage_299turnright_det = g_ad_data_t.AD_299TURN_RIGHT_DET_OUTPUT;
    g_ad_voltage.voltage_299turnleft_det = g_ad_data_t.AD_299TURN_LEFT_DET_OUTPUT;
    g_ad_voltage.voltage_vcc_12v_det = g_ad_data_t.AD_VCC_12V0_DET_OUTPUT;

    msg.data[0] = (uint8_t)g_ad_voltage.voltage_299bakeup_det; 
    msg.data[1] = (uint8_t)(g_ad_voltage.voltage_299bakeup_det>>8); 
    msg.data[2] = (uint8_t)g_ad_voltage.voltage_299turnright_det;
    msg.data[3] = (uint8_t)(g_ad_voltage.voltage_299turnright_det>>8); 
    msg.data[4] = (uint8_t)g_ad_voltage.voltage_299turnleft_det; 
    msg.data[5] = (uint8_t)(g_ad_voltage.voltage_299turnleft_det>>8); 
    msg.data[6] = (uint8_t)g_ad_voltage.voltage_vcc_12v_det; 
    msg.data[7] = (uint8_t)(g_ad_voltage.voltage_vcc_12v_det>>8); 
	

    msg.id = 0x704;
    msg.len = 0x08;
    msg.type = 0x01;
    CAN1_SendMessage(&msg);//522
 //   CAN2_SendMessage(&msg);//d94

}

void BCAN_SEND_AD3(void) {
    CAN_MESSAGE msg;
    g_ad_voltage.voltage_vcc_12v_out1_det = g_ad_data_t.AD_VCC_12V0_OUT1_DET_OUTPUT;
    g_ad_voltage.voltage_ad_vcc_det = g_ad_data_t.AD_AD_VCC_DET_OUTPUT;
    msg.data[0] = (uint8_t)g_ad_voltage.voltage_vcc_12v_out1_det; 
    msg.data[1] = (uint8_t)(g_ad_voltage.voltage_vcc_12v_out1_det>>8); 
    msg.data[2] = (uint8_t)g_ad_voltage.voltage_ad_vcc_det;
    msg.data[3] = (uint8_t)(g_ad_voltage.voltage_ad_vcc_det>>8); 
    msg.data[4] = 0;
    msg.data[5] = 0;
    msg.data[6] = 0;
    msg.data[7] = 0;
    msg.id = 0x705;
    msg.len = 0x08;
    msg.type = 0x01;
    CAN1_SendMessage(&msg);//522
  //  CAN2_SendMessage(&msg);//d94

}

void BCAN_SEND_FREQ(void) {
    CAN_MESSAGE msg;

    
    g_freq.FIN_OUT1 = freq[0];
    g_freq.FIN_OUT2 =  freq[1];
    g_freq.FREQ1 = 1000;
    g_freq.FREQ2 = 500;
    msg.data[0] =  (uint8_t)g_freq.FIN_OUT1; 
    msg.data[1] =  (uint8_t)(g_freq.FIN_OUT1>>8);
    msg.data[2] =  (uint8_t)g_freq.FIN_OUT2; 
    msg.data[3] =  (uint8_t)(g_freq.FIN_OUT2>>8); 
    msg.data[4] =  (uint8_t)g_freq.FREQ1;
    msg.data[5] =  (uint8_t)(g_freq.FREQ1>>8);
    msg.data[6] =  (uint8_t)g_freq.FREQ2;
    msg.data[7] =  (uint8_t)(g_freq.FREQ2>>8);
    msg.id = 0x708;
    msg.len = 0x08;
    msg.type = 0x01;
    CAN1_SendMessage(&msg);//522
   // CAN2_SendMessage(&msg);//d94

}

void BCAN_SEND_LIFE(void) {
    CAN_MESSAGE msg;

    msg.data[0] =  (uint8_t)life_bcan; 
    msg.data[1] =  (uint8_t)(life_bcan>>8); 
    msg.data[2] =   (uint8_t)(life_bcan>>16); 
    msg.data[3] =   (uint8_t)(life_bcan>>24); 
    msg.data[4] = life_bcan;
    msg.data[5] = 0;
    msg.data[6] = 0;
    msg.data[7] = 0;
    msg.id = 0x605;
    msg.len = 0x08;
    msg.type = 0x01;
    msg.type = 0x00;
    CAN1_SendMessage(&msg);//522
  
   // CAN2_SendMessage(&msg);//d94

}

void Test_18DA17F1_ModeSelection(void){
	CAN_MESSAGE msg;
	msg.id = 0x18DA17F2;
		 if(RecCommand == 1) msg.data[0] = JINGDIAN;       //静电
	else if(RecCommand == 2) msg.data[0] = CHUANDAO;       //传导
	else if(RecCommand == 3) msg.data[0] = NAIJIU;         //耐久
	else if(RecCommand == 4) msg.data[0] = DIWEN;          //低温
	else if(RecCommand == 5) msg.data[0] = Liangchanceshi; //量产测试
	else if(RecCommand == 6) msg.data[0] = CHUCE;          //初测
	else msg.data[0] = 0;
	
	msg.data[1] =  0; 
	msg.data[2] =  0; 
	msg.data[3] =  0; 
	msg.data[4] =  0; 
	msg.data[5] =  0; 
	msg.data[6] =  F_6s;  
	msg.data[7] = testStep;  
	msg.len = 8;
	msg.type = 0;
	CAN2_SendMessage(&msg);
	CAN1_SendMessage(&msg);
	
}

/****************************************************
* ID 0x18DA17F1  TBOX -> CLUSTER
* BYTE1                 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7    BYTE8
* RecCommand      RES     RES       RES    RES       RES     F_6s    testStep
*****************************************************/

/****************************************************
* ID 0x781 PC -> TBOX  off line use
* BYTE1                 BYTE2                 BYTE3 		BYTE4 		   BYTE5 		    BYTE6 			BYTE7    		   BYTE8
* SWITCH 1-4      SWITCH 5-8       SWITCH 9-12     SWITCH 13-16   SWITCH 17-20  SWITCH 21-24     SWITCH 25-28   frameLength
*****************************************************/

/****************************************************
* ID 0x785    PC -> TBOX
* BYTE1                 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7    BYTE8
* RecCommand      RES     RES       RES    RES       RES     RES         RES
*****************************************************/



/*总电压、总电流、SOC*/
void PCAN_0x0CFFEAF4(void) {
    CAN_MESSAGE msg;
    uint16_t temp;
    
    msg.data[0] = 0; 
	
	temp = 6000; //总电压
    msg.data[1] = (uint8_t) (temp); 
    msg.data[2] = (uint8_t) (temp/256); 
	
	temp = 1000; //总电流
    msg.data[3] = (uint8_t) (temp); 
    msg.data[4] = (uint8_t) (temp/256); 
    msg.data[5] = 0; 
    msg.data[6] = 0;  
    msg.data[7] = 100; //SOC
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x0CFFEAF4;
	CAN2_SendMessage(&msg);
}
/*电池组SOH，电池平均温度*/
void PCAN_0x18FFEEF4(void) {
    CAN_MESSAGE msg;
    
    msg.data[0] = 0; 
    msg.data[1] = 70; //电池组SOH
    msg.data[2] = 71; //电池平均温度
    msg.data[3] = 0;
    msg.data[4] = 0; 
    msg.data[5] = 0; 
    msg.data[6] = 0;  
    msg.data[7] = 0;  
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x18FFEEF4;
	CAN2_SendMessage(&msg);
}
/*电机电压/电机电流/电机功率/电机转速*/
void PCAN_0x0CFF11EF(void) {
    CAN_MESSAGE msg;
    
	uint16_t temp;
	
	temp = 5900; //电机电压
	msg.data[0] = (uint8_t) (temp);
    msg.data[1] = (uint8_t) (temp/256);
	
	temp = 11900; //电机电流
    msg.data[2] = (uint8_t) (temp);
    msg.data[3] = (uint8_t) (temp/256);
	
	temp = 0; //电机转速
    msg.data[4] = (uint8_t) (temp);
    msg.data[5] = (uint8_t) (temp/256);
	
	temp = 0; //电机扭矩
    msg.data[6] = (uint8_t) (temp);
    msg.data[7] = (uint8_t) (temp/256);
	
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x0CFF11EF;
	CAN2_SendMessage(&msg);
	
}
/*输出轴转速*/
void PCAN_0x0CF00203(void) {
    CAN_MESSAGE msg;
    
	uint16_t temp;
	
	temp = 0;
	msg.data[0] = (uint8_t) (0);
    msg.data[1] = (uint8_t) (temp);
    msg.data[2] = (uint8_t) (temp/256);
    msg.data[3] = (uint8_t) (0);
    msg.data[4] = (uint8_t) (0);
    msg.data[5] = (uint8_t) (0);
    msg.data[6] = (uint8_t) (0);
    msg.data[7] = (uint8_t) (0);
	
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x0CF00203;
	CAN2_SendMessage(&msg);
	
}
/*档位*/
void PCAN_0x18F00503(void) {
    CAN_MESSAGE msg;
    
	msg.data[0] = (uint8_t) (0);
    msg.data[1] = (uint8_t) (0);
    msg.data[2] = (uint8_t) (0);
    msg.data[3] = (uint8_t) (126);
    msg.data[4] = (uint8_t) (0);
    msg.data[5] = (uint8_t) (0);
    msg.data[6] = (uint8_t) (0);
    msg.data[7] = (uint8_t) (0);
	
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x18F00503;
	CAN2_SendMessage(&msg);
	
}
/*A/M模式，E/P模式，蠕动模式*/
void PCAN_0x18FE4A03(void) {
    CAN_MESSAGE msg;
	uint8_t temp = 0;
	static uint32_t ctrl_cnt = 0;
	
	if(ctrl_cnt >= 60)
	{
		ctrl_cnt = 0;
		temp = 0x54;
	}
	else
	{
		if(ctrl_cnt >= 30)
		{
			temp = 0x50;
		}
		ctrl_cnt++;
	}
	
	msg.data[0] = (uint8_t) (0);
    msg.data[1] = (uint8_t) (0);
    msg.data[2] = (uint8_t) (temp);
    msg.data[3] = (uint8_t) (0);
    msg.data[4] = (uint8_t) (0);
    msg.data[5] = (uint8_t) (0);
    msg.data[6] = (uint8_t) (0);
    msg.data[7] = (uint8_t) (0);
	
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x18FE4A03;
	CAN2_SendMessage(&msg);
	
}
/*档位*/
void PCAN_0x18FFF531(void) {
    CAN_MESSAGE msg;
    
	msg.data[0] = (uint8_t) (0);
    msg.data[1] = (uint8_t) (0);
    msg.data[2] = (uint8_t) (0);
    msg.data[3] = (uint8_t) (0);
    msg.data[4] = (uint8_t) (0);
    msg.data[5] = (uint8_t) (0);
    msg.data[6] = (uint8_t) (0);
    msg.data[7] = (uint8_t) (0);
	
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x18FFF531;
	CAN2_SendMessage(&msg);
	
}

/*小计电耗*/
void BCAN_0x18FFF731(void) {
    CAN_MESSAGE msg;
    uint32_t temp = 6999;
	
    msg.data[0] = (uint8_t)(temp);     //小计电耗
    msg.data[1] = (uint8_t)(temp>>8); 
    msg.data[2] = (uint8_t)(temp>>16); 
    msg.data[3] = (uint8_t)(temp>>24);
    msg.data[4] = 0; 
    msg.data[5] = 0; 
    msg.data[6] = 0;  
    msg.data[7] = 0;  
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x18FFF731;
	CAN1_SendMessage(&msg);
}
/*剩余功率，可行驶里程，平均电耗，累计电耗*/
void BCAN_0x18FFF631(void) {
    CAN_MESSAGE msg;
    uint32_t temp = 1999;
	
    msg.data[0] = (uint8_t)(210); //剩余功率
    msg.data[1] = (uint8_t)(0);   //
    msg.data[2] = (uint8_t)(99);  //可行驶里程
	
	temp = 79999;
    msg.data[3] = (uint8_t)(temp);     //累计电耗
    msg.data[4] = (uint8_t)(temp>>8); 
    msg.data[5] = (uint8_t)(temp>>16); 
    msg.data[6] = (uint8_t)(temp>>24);
    msg.data[7] = (uint8_t)(99);       //平均电耗
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x18FFF631;
	CAN1_SendMessage(&msg);
}
/*上装续航里程*/
void BCAN_0x18FFF931(void) {
    CAN_MESSAGE msg;
    
    msg.data[0] = (uint8_t)(0); 
    msg.data[1] = (uint8_t)(0); 
    msg.data[2] = (uint8_t)(0); 
    msg.data[3] = (uint8_t)(0); 
    msg.data[4] = (uint8_t)(105);  //上装续航里程
    msg.data[5] = (uint8_t)(0x01); //上装标志
    msg.data[6] = (uint8_t)(0);
    msg.data[7] = (uint8_t)(0);
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x18FFF931;
	CAN1_SendMessage(&msg);
}
/*电机冷却温度*/
void BCAN_0x0C3DD7A7(void) {
    CAN_MESSAGE msg;
	
    msg.data[0] = 0; 
    msg.data[1] = 0; 
    msg.data[2] = 0; 
    msg.data[3] = 0; 
    msg.data[4] = 90; //电机冷却温度
    msg.data[5] = 0; 
    msg.data[6] = 0;  
    msg.data[7] =0;  
    msg.len = 8;
    msg.type = 0;
	msg.id = 0x0C3DD7A7;
	CAN1_SendMessage(&msg);
}



void Process_Can1(void)
{
	CAN_MESSAGE message;
	memset(&message, 0, sizeof(CAN_MESSAGE));
	CAN1_ReceiveMessage(&message); //数据有效
	switch (message.id) 
	{
		case 0x781://io configure data
		
			if(message.data[7] == 0){//total frame = 1, current frame = 1
				memcpy(&g_io_fonfigure, message.data, 6);
				g_bit_configure_io = 1;
			}
			else{
				if((message.data[7]>>4) > 1)//
					error_type |= 1;//配置数据长度出错
				else
					error_type &= ~0x01;
				if((error_type&0x01) == 0){//配置数据长度正确
					etu_type_io = message.data[6];//读取ETU类型
					num = (message.data[7]&0x0f) ;//读取配置数据长度
					if((message.data[7]&0x0f) == 0)//第一帧数据
						memcpy(&g_io_fonfigure, message.data, 6);
					else if((message.data[7]&0x0f) == 1){//第二帧数据
						memcpy(&g_io_fonfigure.byte[6], message.data, 4);
						g_bit_configure_io = 1;
					}
				}
			}
			break;
		case 0x782:
			break;	
		case 0x785:
			RecCommand = message.data[0];
			EEROM_WriteSpeedRatio(RecCommand) ;
			break;
		case 0x601:
			life_bcan = message.data[0];
			break;
		default:
			break;
	}
}
void Process_Can2(void)
{
	CAN_MESSAGE msg;
	memset(&msg, 0, sizeof(CAN_MESSAGE));
	
	CAN2_ReceiveMessage(&msg);
	switch (msg.id) {
		case 0x781://io configure data  d94
			if(msg.data[7] == 0){//total frame = 1, current frame = 1
				memcpy(&g_io_fonfigure, msg.data, 6);
				g_bit_configure_io = 1;
			}
			else{
				if((msg.data[7]>>4) > 1)//
					error_type |= 1;//配置数据长度出错
				else
					error_type &= ~0x01;
				if((error_type&0x01) == 0){//配置数据长度正确
					etu_type_io = msg.data[6];//读取ETU类型
					num = (msg.data[7]&0x0f) ;//读取配置数据长度
					if((msg.data[7]&0x0f) == 0)//第一帧数据
						memcpy(&g_io_fonfigure, msg.data, 6);
					else if((msg.data[7]&0x0f) == 1){//第二帧数据
						memcpy(&g_io_fonfigure.byte[6], msg.data, 4);
						g_bit_configure_io = 1;
					}
				}
			}
			break;
		case 0x785:
			RecCommand = msg.data[0];
			EEROM_WriteSpeedRatio(RecCommand) ;
			break;
		case 0x18FF0016:
			life_pcan = msg.data[0] ;
			break;
		default:
			break;
	}

}
void Process_Can(void)
{
  //  if((RSCAN0.RFSTS0.UINT32 & 0xFF00) > 0)
  //	Process_Can0();
	
    if((RSCAN0.RFSTS1.UINT32 & 0xFF00) > 0)
	Process_Can1();
	
    if((RSCAN0.RFSTS2.UINT32 & 0xFF00) > 0)
	Process_Can2();
}


//CAN任务调度函数添加
TASK_COMPONENTS TaskComps[] = {
    {BCAN_IO_CONFIGURED, 20},
    {BCAN_AD_CONFIGURED, 20},
    {BCAN_SEND_AD1, 200},
    {BCAN_SEND_AD2, 200},
    {Test_18DA17F1_ModeSelection, 5},
	
    {PCAN_0x0CFFEAF4, 100},
    {PCAN_0x18FFEEF4, 100},
    {PCAN_0x0CFF11EF, 100},
    {PCAN_0x0CF00203, 100},
    {PCAN_0x18F00503, 100},
    {PCAN_0x18FE4A03, 100},
	
    {PCAN_0x18FFF531, 100},
    {BCAN_0x18FFF731, 100},
    {BCAN_0x18FFF631, 100},
    {BCAN_0x18FFF931, 100},
    {BCAN_0x0C3DD7A7, 100},

    {NULL}
};
/**************************************************************************************
 * FunctionName   : TaskRemarks()
 * Description    : 任务标志处理
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
static void TaskRemarks(void) {
    unsigned char i;
    for (i = 0; TaskComps[i].TaskHook != NULL; i++) // 逐个任务时间处理
    {
        if (TaskComps[i].Timer) // 时间不为0
        {
            TaskComps[i].Timer--; // 减去一个节拍
        }
        if (TaskComps[i].Timer == 0) // 时间减完了
        {
            if (TaskComps[i].Run == 0) {
                TaskComps[i].Timer = TaskComps[i].ItvTime; // 恢复计时器值，从新下一次
                TaskComps[i].Run = 1; // 任务可以运行
            }
        }
    }
}
/**************************************************************************************
 * FunctionName   : TaskProcess()
 * Description    : 任务处理
 * EntryParameter : None
 * ReturnValue    : None
 **************************************************************************************/
static void TaskProcess(void) {
    unsigned char i = 0;
    static unsigned char Flag = 0, poll_time = 0;
    //任务处理,两个任务间隔取决于poll_time * 运行时间间隔
    for (i = 0; TaskComps[i].TaskHook != NULL; i++) { // 逐个任务时间处理
        if (TaskComps[i].Run && Flag) // 时间不为0
        {
            TaskComps[i].TaskHook(); // 运行任务
            TaskComps[i].Run = 0; // 标志清0 
            Flag = 0;
        }
    }

    if (poll_time > 0) {
        poll_time = 0;
        Flag = 1;
    } else if (!Flag) {
        poll_time++;
    }

    //任务处理，每次循环只执行一个任务
    /*if (TaskComps[i].TaskHook != NULL) // 逐个任务时间处理
    {
        if (TaskComps[i].Run) // 时间不为0
        {
            TaskComps[i].TaskHook(); // 运行任务
            TaskComps[i].Run = 0; // 标志清0 
        }
        i++;
    }
    if(TaskComps[i].TaskHook == NULL){
        i = 0;
    }*/
}


void Interrupt16us(void)
{
#if 0
    static unsigned int Tcount=0;
    if(Tfin>0){
        if(Tcount>=Tfin){
          Tcount=0;
          FREQ_MCU_OUT1=!FREQ_MCU_OUT1;
        }else Tcount++;
    }else{
         FREQ_MCU_OUT1=0;
    }	
	#endif
}
void Interrupt1ms(void)
{
    //static unsigned int cnt = 0, out_K = 0;
    //out_K = 721; // 比例系数K = 1000 * 3600/公里脉冲数P

    //CAN任务处理
   //  FREQ_MCU_OUT2=!FREQ_MCU_OUT2;//500hz output
    TaskProcess();      
}
void Interrupt10ms(void)                  //10ms中断一次
{
    static unsigned int cnt0 = 0;
    static unsigned int cnt1 = 0;
	static unsigned int cnt2= 0;

    //Count_ADR_ALL();
 
	TaskRemarks();  
	
	if(cnt0 < 100) {
		cnt0++;
	} else {
		cnt0 = 0;
		calculate_fin();
	}
   
	if(cnt1 < 5) {
		cnt1++;
	} else {
		F_50ms = 1;
		// FREQ_MCU_OUT1=!FREQ_MCU_OUT1;// 1khz
		 cnt1 = 0;

	}
	
	if(cnt2 < 600) {
		cnt2++;
	} else {
		F_6s = !F_6s;
		cnt2 = 0;

	}
    
	SYSTME_Logic();

}


void freqcalcute2(void)
{
    fin2_counter++;
    TAUB0.CDR5 =0; 
}
void freqcalcute1(void)
{
    fin1_counter++;
    TAUB0.CDR7 =0; 
 }
void calculate_fin(void)//call this function in 1s interrupt routine
{
    freq[0] = fin1_counter;;
    freq[1] = fin2_counter;;
    fin1_counter = 0;fin2_counter = 0;
}


void LED_AllON(void)
{
	GPIO_Init();

}

