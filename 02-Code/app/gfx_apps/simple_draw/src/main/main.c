/*
 *Created on 2019
 *    东风股份D560电动车仪表盘设计项目工程
 *版本V1.0
 */
#include "SYSTEM.h"
#include "init.h"
#include "motor_sm.h"
#include "r_tick_api.h"
#include "app_can.h"
#include "main.h"
#include "string.h"
#include "obd_dtc.h"
#include "uds_support.h"
#include "ptmr.h"
#include "pwm.h"
#include "app_data_center.h"
#include "scheduler.h"
#include "rtc.h"
#include "adc.h"


/*
  #pragma ghs section rodata=".global_const"
  const unsigned char gDataLoc[4] =  {0x11,0x22,0x33,0x44};// 刷写标示  
  #pragma ghs section rodata= default
  */

int main(void)
{
	ptmrType_t main_tmr_50ms = 0;
	ptmrType_t main_sleep_tmr_10s = 0;
	uint8_t reset_flg = 255;
	uint8_t task = 0;
	uint8_t power_off_flag = 0;
	
	init();        /* init (clock, timer, tft screen)*/
	fdl_init();    /* data flash and eep lib init*/
	System_Init(); /* system init for peripherals and detect init power on status*/
	
	while (1)
	{
		ClrWdt();
		
		if((ptmr_Get(&main_tmr_50ms) >= 50) || (!ptmr_IsStart(&main_tmr_50ms)))
		{
			ptmr_Start(&main_tmr_50ms);
			
			SW_Input();
		}
		
		if (M_ON)
		{/* M_ON active */
			if(ignoffon /* || ChargeViewEnable */)
			{
				while(1) 
				{
					/* 等待看门狗超时，看门狗超时需 500ms */
					delay_cnt = 150U;
					HC595EN = 0;
				}
			}
			
			if(reset_flg != 1)
			{
				M_ON_flag = 1;
				power_off_flag = 0;
				CAN_WAKE_FLG = 0;
				start_LowPower_Mode = 0;
				
				wake_can_cnt = 0;
				
				ChargeViewEnable = 0;
				ChuiSaoViewEnable = 0;
				SmartRechgViewFlg = 0;
				GetOffReminderViewFlg = 0;
				
				mainview_update = 1;
				
				PWM_Update_Duty_Cycle(BG_CHANNEL, 80,PWM_PERIOD); //980hz
				B25_5V  = 1;
				B27_12V = 1;
				
				reset_flg = 1;
			}
			
			if (task == 0) 
			{
				task = 1;
				
				LCD_Exec();
			} 
			else if (task == 1) 
			{
				task = 0;
				Scheduler_DispatchTasks();
				
				if(totalmilesave)

				    
				{	   
					totalmilesave = 0;
					
					if(Miles.total_miles > e_total_miles)
					{
						save_flag = 0;
						
						e_single_miles = Miles.single_miles;
						e_total_miles  = Miles.total_miles;
						EEROM_WriteSingleMiles();
						EEROM_WriteTotalMiles();
					}
				}
				else
				{
					/* if(get_vehicle_speed() < 1) 
					{
						if(Miles.total_miles > e_total_miles)
						{
							save_flag = 0;
							
							e_single_miles = Miles.single_miles;
							e_total_miles  = Miles.total_miles;
							EEROM_WriteSingleMiles();
							EEROM_WriteTotalMiles();
						}
					} */
				}
				
				if(clear_Trip_flag)
				{
					clear_Trip_flag = 0;
					
					Miles.single_miles = 0;
					Miles.single_H2_consp = 0;
					e_single_miles = Miles.single_miles;
					e_H2_TripConsp = Miles.single_H2_consp;
					EEROM_WriteSingleMiles();
					EEROM_Write_H2();
				}
				if(saveH2TripConspflag)
				{
					saveH2TripConspflag = 0;
					
					if(Miles.single_H2_consp > e_H2_TripConsp)
					{
						e_H2_TripConsp = Miles.single_H2_consp;
						EEROM_Write_H2();
					}
				}
				if(saveRunTimeflag)
				{
					saveRunTimeflag = 0;
					e_drivetime = driving_time;
					EEROM_Write_DRIVE_TIME();
				}
			}
			else task = 0;
			
		}
		else if(GetOffReminderFlg /*下车关电提醒*/
		/* || (CAN_KEY_ACC && SmartRechgCfgFlg) */
		|| CAN_CHARGE_LINE /*充电唤醒*/
		|| CAN_H2_ChuiSao    /*吹扫进行中*/
		|| CAN_H2_LowTptChg) /*低温停车充电中*/
		{
			if(reset_flg != 2)
			{
				ChuiSaoViewEnable = 0;
				ChargeViewEnable = 0;
				SmartRechgViewFlg = 0;
				GetOffReminderViewFlg = 0;
				
				M_ON_flag = 0;
				power_off_flag = 0;
				CAN_WAKE_FLG = 1;
				start_LowPower_Mode = 0;
				
				wake_can_cnt = 0;
				mainview_update = 1;
				
				B25_5V  = 1;
				B27_12V = 1;
				
				if(Miles.total_miles > e_total_miles)
				{
					e_single_miles = Miles.single_miles;
					e_total_miles  = Miles.total_miles;
					EEROM_WriteSingleMiles();
					EEROM_WriteTotalMiles();
				}
				else
				{
					if(Miles.total_miles < e_total_miles)
					{/* 加固里程，防止里程异常减小 */
						Miles.single_miles = e_single_miles;
						Miles.total_miles  = e_total_miles;
					}
				}
				
				if(reset_flg == 1)
				{
					StepperMotormiddle();
					R_TICK_WaitMS(0, 100);
				}
				
				if(reset_flg == 0 || reset_flg == 255)
				{
					can_chip_mode_switch(CAN_1_CH,NORMAL_MODE);//!<设置进入正常模式
					can_chip_mode_switch(CAN_2_CH,NORMAL_MODE);//!<设置进入正常模式
				}
				
				reset_flg = 2;
			}
			
			ignoffon = 1;
			delay_cnt = 150U;
			frmID = 0;
			f_index = 0;
			Scheduler_DispatchTasks();
			LCD_Charge();
		}
		else
		{
			if(reset_flg != 0)
			{
				ChuiSaoViewEnable = 0;
				ChargeViewEnable = 0;
				SmartRechgViewFlg = 0;
				GetOffReminderViewFlg = 0;
				
				LED_ECAS = 0;
				
				M_ON_flag = 0;
				CAN_WAKE_FLG = 0;
				start_LowPower_Mode = 1;
				B25_5V  = 0;
				B27_12V = 0;
				ADC_AirPress_Cleanup_Cache();
				
				if(TD_MsgEnable != 0) //时间同步
				{
					TD_MsgEnable = 0;
					External_RTC_SetTime(&timer);
				}
				
				if(reset_flg == 1)
				{
					StepperMotormiddle();
					R_TICK_WaitMS(0, 300);
				}
				
				mainview_update = 1;
				frmID = 0;
				f_index = 0;
				
				PWM_Update_Duty_Cycle(BG_CHANNEL, 0 ,PWM_PERIOD);
				
				if (save_flag) 
				{ /* save miles to data flash*/
					save_flag = 0;
					
					if(Miles.total_miles > e_total_miles)
					{
						e_single_miles = Miles.single_miles;
						e_total_miles  = Miles.total_miles;
						EEROM_WriteSingleMiles();
						EEROM_WriteTotalMiles();
					}
					else
					{
						if(Miles.total_miles < e_total_miles)
						{/* 加固里程，防止里程异常减小 */
							Miles.single_miles = e_single_miles;
							Miles.total_miles  = e_total_miles;
						}
					}
					
					R_TICK_WaitUS(0, 100);
				}
				
				if(e_VehLockStus != LckVehBoundState)
				{
					e_VehLockStus = LckVehBoundState;
					
					EEROM_Write_VehLock();
					R_TICK_WaitUS(0, 100);
				}
				
				e_drivetime = driving_time;
				EEROM_Write_DRIVE_TIME();
				EEROM_Write_H2();
				R_TICK_WaitUS(0, 100);
				
				ptmr_Start(&main_sleep_tmr_10s);
				
				task = 6;
				power_off_flag = 0;
				reset_flg = 0;
			}
			
			ignoffon = 1;
			delay_cnt = 150U;
	    	frmID = 0;
	    	f_index = 0;
			
			can_rxRoutine();
	    	PWM_Update_Duty_Cycle(LCD_CHANNEL, 0, PWM_PERIOD); /*lcd screen off*/
			ClrWdt();
			
			if ((task == 6)&&(wake_up3 == 0)&&(wake_can == 0)&&(ptmr_Get(&main_sleep_tmr_10s) >= 10000))
			{
				ptmr_Stop(&main_sleep_tmr_10s);
				
				B25_5V  = 0;
				B27_12V = 0;
				
				power_off_flag = 1;
				uds_diagnostic_save_alldtc();
			}
			
			if(!LED_POWERON && power_off_flag)
			{
				can_chip_mode_switch(CAN_1_CH,NORMAL_MODE);//!<设置进入正常模式
				can_chip_mode_switch(CAN_2_CH,NORMAL_MODE);//!<设置进入正常模式
				R_TICK_WaitMS(0, 200);
				can_chip_mode_switch(CAN_1_CH,GO_TO_SLEEP_MODE);
				can_chip_mode_switch(CAN_2_CH,GO_TO_SLEEP_MODE);
				R_TICK_WaitMS(0, 25);
				
				start_LowPower_Mode = 0;
				power_off_flag = 0;
				
				PWR_CTRL = 0; //!<关机
				R_TICK_WaitMS(0, 25);
			}
		}
	}
}






