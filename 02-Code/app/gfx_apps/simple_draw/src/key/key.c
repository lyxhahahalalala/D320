/******************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    key.c
 * @brief   key process
 * @version V1.0
 * @author  liuwenlong
 * @date    2020-7-21
 * @note <b>history:</b>
 * 1. create file
 *
 ******************************************************************************/
#include "iodefine.h"
#include "key.h"
#include "SYSTEM.h"


/*按键控制接口 */
#define KEY_AV		(!PORT_ISO.PPR16.BIT.PPR16_8)
#define KEY_UP		(!PORT_ISO.PPR16.BIT.PPR16_11)
#define KEY_CLEAR	(!PORT_ISO.PPR16.BIT.PPR16_10)
#define KEY_SET		(!PORT_ISO.PPR16.BIT.PPR16_9)


KEY_BIT_t  pKEY[3];


void Key_Init(void)
{
	//keys initialize again or p16_8 not work well
	PORT_ISO.PMC16.UINT16	&= ~0x0f00; 
	PORT_ISO.PM16.UINT16	|= 0x0f00;
	PORT_ISO.PIBC16.UINT16	|= 0x0f00;	//P16_8 - P16_11   sw1234 input xiangyanglvxingche   
    
}

void KeyProcess(void)
{
	static unsigned int lpcnt2 = 0, lpcnt1 = 0;
	static unsigned int lpjingxi = 0, lpkazhi = 0, lpkav = 0, lpkup = 0, lpkclear = 0;
	
	//bit key_set
	if(!KEY_SET && pkey_set && !bkey_set) //按下
	{
		key_set = 1;
		bkey_set = pkey_set;
	}
	else
	{
		if(!KEY_SET == pkey_set) bkey_set = pkey_set;
	}
	pkey_set = KEY_SET;
	
	//bit  key_up;
	if(KEY_UP) //按下&& pkey_up && !bkey_up
	{
		if(lpkup<3000)
			lpkup ++;
		else
			lpkup = 3000;
		if((lpkup<1200)&&(lpkup>60))
			key_long_up = 1;
		else
			key_long_up = 0;
	}
	else //释放
	{
		if((lpkup<60)&&(lpkup>10))
		{
			key_up = 1; //向上按键
			bkey_up = pkey_up;
			key_long_up = 0;
		}
		else if(lpkup == 3000)
		{
			key_kazhi= 1;
			key_long_up = 0;
		}
		lpkup = 0;
	}

	//bit  key_clear;
	if(KEY_CLEAR) //按下&& pkey_up && !bkey_up
	{
		if(lpkclear<3000)
			lpkclear ++;
		else
			lpkclear = 3000;
		if((lpkclear<1200)&&(lpkclear>60))
			key_long_clear = 1;
		else
			key_long_clear = 0;
	}
	else //释放
	{
		if((lpkclear<60)&&(lpkclear>10))
		{
			key_clear = 1; //向上按键
			bkey_clear = pkey_clear;
			key_long_clear = 0;
		}
		else if(lpkclear == 3000)
		{
			key_kazhi= 1;
			key_long_clear = 0;
		}
		lpkclear = 0;
	}

	//bit  key_up;
	if(KEY_AV) //按下&& pkey_up && !bkey_up
	{
		if(lpkav<3000)
			lpkav ++;
		else
			lpkav = 3000;
		if((lpkav<1200)&&(lpkav>60))
			key_long_av = 1;
		else
			key_long_av = 0;
	}
	else //释放
	{
		if((lpkav<60)&&(lpkav>10))
		{
			key_AV= 1; //向上按键
			//bkey_up = pkey_up;
			key_long_av = 0;
		}
		else if(lpkav == 3000)
		{
			key_kazhi= 1;
			key_long_av = 0;
		}
		lpkav = 0;
	}

	if((KEY_SET)&&(KEY_AV==0)&&(KEY_UP==0) )
	{
		if (lpcnt2 >= 300) //长按SET按钮>0.6s
		{
			bkey_set= 1; //长按时取消单按键操作
			key_long_set = 1;
			lpcnt2  = 0;
		}
		else lpcnt2++;
	}
	else lpcnt2 = 0;

	if((KEY_AV==1) && (KEY_SET==1) )
	{
		if (lpcnt1 >= 300) //组合按钮，长按AV和KEY_SET按钮
		{
			//lpcnt1 = 0;
			bkey_AV = 0; //长按时取消单按键操作
			bkey_set = 0; //长按时取消单按键操作
			key_duble = 1; //隐含功能按键
		}
		else lpcnt1++;
	}
	else lpcnt1 = 0;

	if((KEY_CLEAR == 0)&&(KEY_AV==0)&&(KEY_UP==0)&&(KEY_SET==0) )// 静息
	{
		if(lpjingxi < 1500)
		{
			lpjingxi++;
		}
		else
		{
			key_resting15s = 1;
			
			if(lpjingxi<3000)
				lpjingxi++;
			else
			{
				lpjingxi = 3000;
				key_jingxi = 1;
			}
		}
			
	}
	else
	{
		lpjingxi = 0;
		key_jingxi = 0;
		// key_resting15s = 0;
	}
	
	if(KEY_CLEAR||KEY_AV||KEY_UP||KEY_SET ) // 卡纸
	{
		if(lpkazhi<3000)
			lpkazhi++;
		else
		{
			lpkazhi = 3000;
			key_kazhi= 1;
		}
	}
	else
	{
		lpkazhi = 0;
		key_kazhi = 0;
	}
}

