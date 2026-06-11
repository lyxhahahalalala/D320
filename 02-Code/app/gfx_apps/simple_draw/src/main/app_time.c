/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_time.c
 * @brief   time display
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
#include "app_time.h"
#include "app_can.h"
#include "rtc.h"


/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */

#define  HF12  1
#define  HF24  0
#define  OFFSETVALUE_TIME  10

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
 * Locol FUNCTIONS DECLEAR
 */

/**
 * \brief miles calculate
 * \return null
 * \retval null
 * \retval null
 */
unsigned char decimal_bcd(unsigned char decimal)//10进制转换bcd格式
{
    return (unsigned char)( (decimal/10)*16 + decimal%10);
}

unsigned char bcd2decimal(unsigned char bcd)//bcd进制转换10格式
{
    return (unsigned char)((bcd/16)*10 + bcd%16);
}

uint8_t CaculateWeekDay(int y,int m, int d)
{
    if(m==1||m==2)
	{
        m = m + 12;
        y--;
    }
    return (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400+1)%7;
}

void Display_Date(void)
{
    uint8_t uniStr[64];
    static uint8_t old_dat[8];
    static uint8_t ampm,ampmold;
    GeneralUse_t *GU_18FEE6EE_t =NULL;
    static uint8_t hourformat_old = 0;
	
    if(TD_MsgEnable != 0)
	{
		GU_18FEE6EE_t =  (GeneralUse_t*)can_getBCanBuffer(0x18FEE6EE);
		if(GU_18FEE6EE_t->byte3.byte <= 23U)
		{
			timer.hour = decimal_bcd(GU_18FEE6EE_t->byte3.byte);
		}
		
		if(GU_18FEE6EE_t->byte2.byte <= 59U)
		{
			timer.minute= decimal_bcd(GU_18FEE6EE_t->byte2.byte);
		}
		
		if(GU_18FEE6EE_t->byte1.byte <= 239U)
		{
			timer.second= decimal_bcd(GU_18FEE6EE_t->byte1.byte/4U);
		}
		
		if((GU_18FEE6EE_t->byte6.byte + 1985) >= 2000U)//2000年
		{
			timer.year = decimal_bcd((uint8_t)(GU_18FEE6EE_t->byte6.byte + 1985 - 2000U));
		}
		else
		{
			timer.year = decimal_bcd((uint8_t)(GU_18FEE6EE_t->byte6.byte + 1985 - 1900U));//1985年到1999年
		}
		
		if(GU_18FEE6EE_t->byte4.byte <= 12U)
		{
			timer.month= decimal_bcd(GU_18FEE6EE_t->byte4.byte);
		}

		if(GU_18FEE6EE_t->byte5.byte <= 124)
		{
			timer.day= decimal_bcd(GU_18FEE6EE_t->byte5.byte/4);
		}
    }

	if(timer.hour >= 0x12)	//大于等于12点，说明已经是下午
		ampm = 0x01;
	else
		ampm = 0x00;

    if(hourformat_old != e_hourformat)
	{
		loc_ClearRect(285, 15, 110, 40);
		hourformat_old = e_hourformat;
    }

    start_draw();

	uint8_t hour_temp = bcd2decimal(timer.hour);	//获取十进制的小时
	uint8_t minute_temp = bcd2decimal(timer.minute);	//获取十进制的分钟
	
    if(e_hourformat == HF12)//12小时制
    {
		/*小时的十位更新*/
		if(mainview_update||old_dat[0] !=  hour_temp) 
		{		
			loc_ClearRect(293, 15, 18, 40);
			if((10U == hour_temp)||(11U == hour_temp)||(22U == hour_temp)||(23U == hour_temp))
			{
				zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
				zk_printNum(293, 15,1, 0, ZK_ALIGN_LEFT);
			}
			old_dat[0] = hour_temp;
		}
		else{}

		/*小时的个位更新*/
		if(mainview_update||old_dat[1] != hour_temp)
		{
			loc_ClearRect(293+16, 15, 18, 40);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
			zk_printNum(293+16, 15,(hour_temp%12%10), 0, ZK_ALIGN_LEFT);
			old_dat[1] = hour_temp;
		}
		else{}

		/*分钟的十位更新*/
		if(mainview_update||old_dat[2] != (minute_temp/10U))
		{
			loc_ClearRect(333, 15, 18, 40);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
			zk_printNum(333, 15,(minute_temp/10U), 0, ZK_ALIGN_LEFT);//m_h
			old_dat[2] = minute_temp/10U;
		}
		else{}

		/*分钟的个位更新*/
		if(mainview_update||old_dat[3] != (minute_temp%10U))
		{
			loc_ClearRect(333+16, 15, 18, 40);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
			zk_printNum(333+16, 15,(minute_temp%10U), 0, ZK_ALIGN_LEFT);//m_L
			old_dat[3] = (minute_temp%10U);
		}
		else{}

		if(mainview_update||ampm!= ampmold)
		{
			loc_ClearRect(368, 26, 44, 40);
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, VM_COLOR_WHITE, 0);//24piex
			
			if(hour_temp >= 12U)
				zk_oem2uni((uint8_t *)"PM", uniStr, sizeof(uniStr));
			else
				zk_oem2uni((uint8_t *)"AM", uniStr, sizeof(uniStr));
			zk_printZH(368, 26, uniStr);
			ampmold = ampm;
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)":", uniStr, sizeof(uniStr));//:
			zk_printZH(327, 18, uniStr);
		}
    }
    else//24h format
	{
		/*小时的十位更新*/
 		if(mainview_update||old_dat[0] !=  (hour_temp/10))
		{
			loc_ClearRect(293+OFFSETVALUE_TIME, 15, 18, 40);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
 			zk_printNum(293+OFFSETVALUE_TIME, 15,hour_temp/10, 0, ZK_ALIGN_LEFT);	//hour_H
			old_dat[0] = (hour_temp/10);
		}
		else{}

		/*小时的个位更新*/
		if(mainview_update||old_dat[1] !=  (hour_temp%10))
		{
			loc_ClearRect(293+16+OFFSETVALUE_TIME, 15, 18, 40);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
 			zk_printNum(293+16+OFFSETVALUE_TIME, 15,hour_temp%10, 0, ZK_ALIGN_LEFT);//hour_L
			old_dat[1] = (hour_temp%10);
		}
		else{}


		/*分钟的十位更新*/
		if(mainview_update||old_dat[2] != (minute_temp/10))
		{
			loc_ClearRect(333+OFFSETVALUE_TIME, 15, 18, 40);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
			zk_printNum(333+OFFSETVALUE_TIME, 15,minute_temp/10, 0, ZK_ALIGN_LEFT);//m_h
			old_dat[2] = minute_temp/10;
		}
		else{}

		/*分钟的个位更新*/
		if(mainview_update||old_dat[3] != (minute_temp%10))
		{
			loc_ClearRect(333+16+OFFSETVALUE_TIME, 15, 18, 40);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
			zk_printNum(333+16+OFFSETVALUE_TIME, 15,minute_temp%10, 0, ZK_ALIGN_LEFT);//m_L
			old_dat[3] = minute_temp%10;
		}
		else{}

		
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)":", uniStr, sizeof(uniStr));
		zk_printZH(327+OFFSETVALUE_TIME, 18, uniStr);
		loc_ClearRect(325+OFFSETVALUE_TIME, 18, 2, 40);
    }
    
    end_draw();
}








