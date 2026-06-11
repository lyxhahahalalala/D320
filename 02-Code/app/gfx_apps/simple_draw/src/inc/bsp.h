/* 
 * File:   bsp.h
 * Author: mpszm
 *
 * Created 
 */

#ifndef _BSP_H
#define	_BSP_H

#include "r_typedefs.h"

#pragma pack(1)
typedef union 
{
    struct 
    {
        unsigned bit0 : 1;
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
    };
    unsigned char byte;
} DIDO_DATA;
 #pragma pack()

extern DIDO_DATA  alarm_led[9];
#define BUZZ                alarm_led[0].bit6 

#define LED_SOC1            alarm_led[7].bit0 	//D318     
#define LED_SOC2            alarm_led[7].bit1 	//D322      
#define LED_SOC3            alarm_led[7].bit2 	//D319
#define LED_SOC4            alarm_led[7].bit3 	//D323      
#define LED_SOC5            alarm_led[7].bit4 	//D320   
#define LED_SOC6            alarm_led[7].bit5 	//D324 

#define LED_Shuiwen1	    alarm_led[7].bit6 	//D321   
#define LED_Shuiwen2 	    alarm_led[7].bit7 	//D325 
#define LED_Shuiwen3	    alarm_led[6].bit0 	//D310
#define LED_Shuiwen4	    alarm_led[6].bit1 	//D314
#define LED_Shuiwen5 	    alarm_led[6].bit2 	//D311
#define LED_Shuiwen6 	    alarm_led[6].bit3 	//D315

#define LED_DCDC 			alarm_led[6].bit4	//D312 -- dcdc故障
#define LED_LOCK  			alarm_led[6].bit5	//D316 -- 驾驶室锁止不到位
#define LED_HighVolBatFault alarm_led[6].bit6	//D313 -- 动力电池故障
#define LED_CoolingLiquied 	alarm_led[6].bit7	//D317 -- 水位

#define LED_SysFault 		alarm_led[5].bit0	//D34 -- 整车故障
#define LED_SocL  			alarm_led[5].bit1	//D35 -- SOC过低报警
#define LED_SaftyBelt 		alarm_led[5].bit2	//D36 -- 安全
#define LED_Park 			alarm_led[5].bit3	//D37 -- 驻车手刹
#define LED_BatOverTmp 		alarm_led[5].bit4	//D38 -- 动力电池过温
#define LED_MotorOverTmp	alarm_led[5].bit5	//D39 -- 电机过温
#define LED_InsulationFault alarm_led[5].bit6	//D40 -- 绝缘报警
#define LED_Water			alarm_led[5].bit7	//D41 -- 水温高报警灯
#define LED_MirrorHeat		PORT_AWO.P0.BIT.P0_0 //后视镜

#define LED_OverSpeed		alarm_led[4].bit0	//D261 -- 超速
#define LED_DoorOpen 		alarm_led[4].bit1	//D259 -- 门开
#define LED_MotorFault 		alarm_led[4].bit2	//D331 -- 电机故障
#define LED_POWERON  		alarm_led[4].bit3	//D330 -- 电源指示
#define LED_AirPressureHigh alarm_led[4].bit6	//D342 -- 黄色气压指示
#define LED_ChargeConnected	alarm_led[4].bit5	//D341 -- 充电连接
#define LED_AirPressureLow 	alarm_led[4].bit4	//D48  -- 红色制动指示
#define LED_ASR  			alarm_led[4].bit7	//D49  -- ASR

#define LED_AxleDiff 		alarm_led[3].bit0	//D336 -- 轴间差速锁
#define LED_ESC		 		alarm_led[3].bit1	//D334 -- ESP / ESC
#define LED_ECAS   			alarm_led[3].bit2	//D335 -- 空气悬架报警
#define LED_BrakeDes 		alarm_led[3].bit3	//D339 -- 刹车片报
#define LED_PowerBatCutOff 	alarm_led[3].bit4	//D340 -- 动力电池切断
#define LED_TrailerRight 	alarm_led[3].bit5	//D338 -- 右挂车灯
#define LED_TrailerLeft		alarm_led[3].bit6	//        左挂车灯
#define LED_WheelDiff 		alarm_led[3].bit7	//D337 -- 轮间差速锁

#define LED_ABS  		    alarm_led[2].bit0	//D328 -- ABS
#define LED_RearFog 	    alarm_led[2].bit1	//D326 -- 后雾
#define LED_LightCheck 	    alarm_led[2].bit2	//D327 -- 灯丝检
#define LED_Dryer		    alarm_led[2].bit3	//D53  -- 干燥
#define LED_TPMS 		    alarm_led[2].bit4	//D274 -- 胎压异常
#define LED_Disu  		    alarm_led[2].bit5	//D272 -- 龟
#define LED_PTO 		    alarm_led[2].bit6	//D276 -- 取力
#define LED_Warning  	    alarm_led[2].bit7	//D329 -- 三角感叹号黄

#define LED_LocationLight	alarm_led[1].bit0	//D277 -- 位置
#define LED_TurnLeft 		alarm_led[1].bit1	//D278 -- 左转
#define LED_TurnRight 		alarm_led[1].bit2	//D268 -- 右转
#define LED_XunHang			alarm_led[1].bit3	//D279 -- 巡航
#define LED_LowBeam			alarm_led[1].bit4	//D280 -- 近光
#define LED_FrontFog 		alarm_led[1].bit5	//D281 -- 前雾
#define LED_COFFEE 			alarm_led[1].bit6	//D333 -- 疲劳驾驶
#define LED_FarBeam  		alarm_led[1].bit7	//D282 -- 远光


extern DIDO_DATA sw_input[4];
#define IN1    sw_input[0].bit0  //A25-高边-Reserved  后视镜
#define IN2    sw_input[0].bit1  //A24-高边-浮动桥报警开
#define IN3    sw_input[0].bit2  //A23-高边-空气悬架开
#define IN4    sw_input[0].bit3  //A22-高边-干燥器开
#define IN5    sw_input[0].bit4  //C2--高边-后照灯指
#define IN6    sw_input[0].bit5  //C9--高边-进气预热开
#define IN7    sw_input[0].bit6  //C13-高边-预留 电源开关指示
#define IN8    sw_input[0].bit7  //C25-高边-BCM跛行回家开

#define IN9    sw_input[1].bit0  //A19-高边-IGN
#define IN10   sw_input[1].bit1  //C3--低边-预留 三桥提升  悬空报警
#define IN11  !sw_input[1].bit2  //C15-低边-取力指示  翼门开
#define IN12  !sw_input[1].bit3  //C14-低边-紧急切断
#define IN13   sw_input[1].bit4  //A10-低边-低档开 制动蹄片磨损R5（低边，悬空报警）
#define IN14  !sw_input[1].bit5  //C1--低边-变速箱取力指示
#define IN15   sw_input[1].bit6  //C20-低边-制动蹄片磨损R1（低边，悬空报警）
#define IN16   sw_input[1].bit7  //C23-低边-制动蹄片磨损R4（低边，悬空报警）

#define IN17   sw_input[2].bit0  //C16-低边-制动蹄片磨损L1（低边，悬空报警）
#define IN18   sw_input[2].bit1  //C18-低边-制动蹄片磨损L3（低边，悬空报警）
#define IN19   sw_input[2].bit2  //C21-低边-制动蹄片磨损R2（低边，悬空报警）
#define IN20  !sw_input[2].bit3  //C24-低边-驻车制动开
#define IN21   sw_input[2].bit4  //C17-低边-制动蹄片磨损L2（低边，悬空报警）
#define IN22   sw_input[2].bit5  //C19-低边-制动蹄片磨损L4（低边，悬空报警）
#define IN23   sw_input[2].bit6  //C22-低边-制动蹄片磨损R3（低边，悬空报警）
#define IN24  !sw_input[2].bit7  //C26-低边-乘客侧安全带开

#define IN25  !sw_input[3].bit0  //C10-低边-挂车ABS指示
#define IN26  !sw_input[3].bit1  //C12-低边-轮差开关
#define IN27   sw_input[3].bit2  //C8--低边-高档开 制动蹄片磨损L5（低边，悬空报警）
#define IN28  !sw_input[3].bit3  //C11-低边-轴差开关
#define IN29  !sw_input[3].bit4  //A26-低边-充电指示
#define IN30  !sw_input[3].bit5  //A7--低边-空滤堵塞报警开  转向锁止报警开关（低边，悬空报警）
#define IN31  !sw_input[3].bit6  //A8--低边-驾驶室锁止不到位报警开
#define IN32  !sw_input[3].bit7  //A9--低边-司机侧安全带指示开

#define wake_up1   0   //not key input
#define wake_up2   0   //not key input
#define wake_up3   IN9

/* LCD */
#define BLACK            1  //0xFF000000
#define BLUE             2  //0xFF0000FF
#define GREEN            3  //0xFF00FF00
#define CYAN             4  //0xFF00FFFF
#define RED              5  //0xFFFF0000
#define MAGENTA          6  //0xFFFF00FF
#define YELLOW           7  //0xFFFFFF00
#define WHITE            8  //0xFFFFFFFF
#define GRAY             9  //0xFF202020
#define ORANGE           0  //0xFFFFDAB9
#define TOUMING          11 //0x00000000
#define QiYa_Alpha       12 //0xD0009B7C
#define QiYa_LOW_Alpha   13 //0xD0D00000
#define QiYa_HIGH_Alpha  14 //0xD0D0D000
#define GRAY2            15 //0xFF505050
#define BLUE2            16 //0xFF1B73E3


void LED_Out(void);
void SW_Input(void);

void LCD_BackLight(unsigned char back);
void LCD_Clear(void);
void LCD_Color(uint32_t color);
void LCD_DrawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char color);


void init_wdt(void);
void ClrWdt(void);

unsigned int EEROM_ReadWord(unsigned long dst);
void EEROM_WriteWord(unsigned long dst, unsigned int val);





#endif	/* BSP_H */




