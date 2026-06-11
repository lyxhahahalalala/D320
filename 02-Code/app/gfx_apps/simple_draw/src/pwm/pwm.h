
/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    pwm.h
 * @brief   pwm application
 * @version V1.0
 * @author  liuwenlong
 * @date    2022-7-8
 * @note <b>history:</b>
 * 1. create file
 *
 */
#ifndef PWM_H
#define	PWM_H



typedef enum {
    PWM_CHANNEL_0 =0,
	PWM_CHANNEL_1,
	PWM_CHANNEL_2,
	PWM_CHANNEL_3,
	PWM_CHANNEL_4,
	PWM_CHANNEL_5,
	PWM_CHANNEL_6,
	PWM_CHANNEL_7,
	PWM_CHANNEL_8,
	PWM_CHANNEL_9,
	PWM_CHANNEL_10,
	PWM_CHANNEL_11,
	PWM_CHANNEL_12,
	PWM_CHANNEL_13,
	PWM_CHANNEL_14,
	PWM_CHANNEL_15
	
} PWM_CHANNEL_t; 

#define PWM_PERIOD      0x02

#define BUZZ_CHANNEL 	PWM_CHANNEL_13
#define LED_CHANNEL 	PWM_CHANNEL_14
#define BG_CHANNEL 		PWM_CHANNEL_11
#define LCD_CHANNEL 	PWM_CHANNEL_6


void PWM_Init( void );
void PWM_Start(unsigned char channel);
void PWM_Stop(unsigned char channel);
void PWM_Update_Duty_Cycle( unsigned char channel, float duty, unsigned int period);




#endif	/* PWM_H */



