/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_trip.c
 * @brief   trip info calculate
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
#include "app_can.h"
#include "uds_support.h"
#include "app_data_center.h"
/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */



/*******************************************************************************
 * TYPEDEFS
 */



/**
 * \defgroup GROUP_LOCAL_FUNCTIONS APP:VIEW:LOCAL_FUNCTIONS
 */


/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * Gloable FUNCTIONS DECLEAR
 */

static uint8_t power_on_update_flg = 0;

void Calculate_Miles(void)
{
	static float mile = 0;
	static float miletotal = 0;
	float speed = 0;
	
	if(Miles.total_miles < e_total_miles)
	{/* 加固里程，防止里程异常减小 */
		Miles.total_miles = e_total_miles;
	}
	
	// 100ms
	//计算累计里程    里程保存算法：正常关机时候，如果小计里程>100m则保存一次总里程和小计里程
	//非关机时，如果总里程走到了5km，则强制保存一次；
	//同时清除一下总里程的临时变量，以防止多次保存
	if(M_ON_flag && VehicleReadyFlag && (Test_Mode==0) && (Needle_Test == 0))	//非测试模式
	{
		speed = get_vehicle_speed();
		if (speed >= 1) //车速大于0时才累计
		{
			mile = mile + (float) ((speed / 3.6)* 0.1); //单位 m(米)
			miletotal = miletotal + (float) ((speed / 3.6)* 0.1);
			if (mile >= 100.0)
			{
				mile = mile - 100;
				
				save_flag = 1;
				
				if (Miles.single_miles >= 99999)
				{
					Miles.single_miles = 0;
					
					Miles.single_H2_consp = 0;
					e_H2_TripConsp = 0;
					power_on_update_flg = 0;
					saveH2TripConspflag = 1;
				}
				else
				{
					Miles.single_miles++;
				}
				
				if (Miles.total_miles >= 9999999)
				{
					Miles.total_miles = 9999999;
				}
				else
				{
					Miles.total_miles++;
				}
			}
			if(miletotal>=5000.0)
			{
				miletotal = 0;
				totalmilesave = 1;
			}
		}
	}
}

void Calculate_H2Consp(void)
{
	static uint16_t PreviousCanMsgH2Consp = 0x00;
	
	uint16_t CrtCanMsgH2Consp = get_current_H2_consp();
	
	// 根据报文，可以得到实时的累计耗氢量 X(精度 kg). 
	// 在小计氢耗开始计算的时间点 T1,读数当时累计耗氢量 X1；
	// 然后实时读取 can 报文中累计耗氢量 X2, 仪表显示的小计氢耗 N=X2-X1;
	if(M_ON_flag && CrtCanMsgH2Consp > 0 && CrtCanMsgH2Consp != 0xffffu)
	{
		if(power_on_update_flg == 0 || PreviousCanMsgH2Consp > CrtCanMsgH2Consp)
		{
			PreviousCanMsgH2Consp = CrtCanMsgH2Consp;
			
			power_on_update_flg = 1;
		}
		else
		{
			if(PreviousCanMsgH2Consp != CrtCanMsgH2Consp)
			{
				Miles.single_H2_consp = Miles.single_H2_consp + CrtCanMsgH2Consp - PreviousCanMsgH2Consp;
				
				PreviousCanMsgH2Consp = CrtCanMsgH2Consp;
			}
		}
		
		if((Miles.single_H2_consp - e_H2_TripConsp) >= 10) //每消耗1kg，保存一次
		{
			saveH2TripConspflag = 1;
		}
	}
}


/*-----------------------------------------------End of file----------------------------------------------------------*/
