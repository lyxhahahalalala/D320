
#include "bsp.h"
#include "main.h"
#include "pwm.h"
#include "SYSTEM.h"


void LCD_BackLight(unsigned char back)   //LCD背关亮度控制 0-7 0关闭 7最亮
{
    if(back == 0)
	{
		PWM_Update_Duty_Cycle(LCD_CHANNEL,0,PWM_PERIOD);//980hz
	}
    if(back == 7)
    {
    	if(!LED_LocationLight)
    		PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd,PWM_PERIOD);//980hz
		else
    		PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd_night,PWM_PERIOD);//980hz
    }
}

void LCD_Clear(void) //清屏 屏变黑色
{
    loc_RenderClear(0);
}

void LCD_Color(uint32_t color) //清屏 屏变yanse
{
    loc_RenderClear(color);
}

unsigned int color_convert(unsigned char color_src)
{
    switch(color_src)
    {
        case 1:  return 0xFF000000;
        case 2:  return 0xFF0000FF;
        case 3:  return 0xFF00FF00;
        case 4:  return 0xFF00FFFF;
        case 5:  return 0xFFFF0000;
        case 6:  return 0xFFFF00FF;
        case 7:  return 0xFFFFFF00;
        case 8:  return 0xFFFFFFFF;
        case 9:  return 0xFF202020;
        case 10: return 0xFFFFDAB9;
		case 11: return 0x00000000;
		case 12: return 0xD0009B7C;
		case 13: return 0xD0D00000;
		case 14: return 0xD0D0D000;
		case 15: return 0xFF505050;
		case 16: return 0xFF1B73E3;
        default:
            return 0xFFFFFFFF;
    }
}

void LCD_DrawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char color) //W的值为0时，会溢出，画矩形
{
    unsigned int color_d = color_convert(color);
    loc_RenderRect(x, y, w, h, color_d);
}


