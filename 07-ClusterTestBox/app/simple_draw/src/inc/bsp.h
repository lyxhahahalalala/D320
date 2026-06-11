/* ***********************************************************************************************************************************************
 * File:   bsp.h
 * Author: Carter Liu
 *
 * Board related info.
 * Copyright@Shanghaifangyan
 *************************************************************************************************************************************************/

#ifndef BSP_H
#define	BSP_H

#include "R_typedefs.h"
/*********************************************************************************************************************************************
*Macro Defs
*********************************************************************************************************************************************/
#define BUZZ  alarm_led[0].bit0  //

#define CS_A  alarm_led[0].bit4  //4051
#define CS_B  alarm_led[0].bit3 
#define CS_C  alarm_led[0].bit2 
#define CS_E  alarm_led[0].bit5 

#define PO1   alarm_led[0].bit6 //
#define PO2   alarm_led[0].bit7 //  

#define LED28  alarm_led[2].bit0 //      LED1
#define LED29  alarm_led[2].bit1 //    LED2
#define LED3  alarm_led[2].bit2 //
#define LED4  alarm_led[2].bit3 //     
#define LED30  alarm_led[2].bit4 //  LED5
#define LED31 alarm_led[2].bit5 //LED6
#define LED7  alarm_led[2].bit6 //
#define LED8  alarm_led[2].bit7 //
#define LED9  alarm_led[3].bit0 //
#define LED10 alarm_led[3].bit1 //              //??  ABS
#define LED11 alarm_led[3].bit2 //
#define LED12 alarm_led[3].bit3 //  
#define LED25 alarm_led[3].bit4 //         LED13
#define LED26 alarm_led[3].bit5 //LED14
#define LED27 alarm_led[3].bit6 //  //??  LED15
#define LED16 alarm_led[3].bit7 //              
#define LED17 alarm_led[4].bit0 //
#define LED18 alarm_led[4].bit1 //
#define LED19 alarm_led[4].bit2 //
#define LED20 alarm_led[4].bit3 //
#define LED21 alarm_led[4].bit4 //
#define LED22 alarm_led[4].bit5 //
#define LED23 alarm_led[4].bit6 //              //??  ????
#define LED24 alarm_led[4].bit7 //

#define LED13  alarm_led[1].bit0 //LED25
#define LED14  alarm_led[1].bit1 //
#define LED15  alarm_led[1].bit2 //LED27
#define LED1  alarm_led[1].bit3 //LED28
#define LED2  alarm_led[1].bit4 //LED29
#define LED5  alarm_led[1].bit5 //LED30
#define LED6  alarm_led[1].bit6 //LED31
#define LED32  alarm_led[1].bit7 //




#define IN1   sw_input[0].bit0     //
#define IN2   !sw_input[0].bit1     //
#define IN3   !sw_input[0].bit2     //
#define IN4   !sw_input[0].bit3     //
#define IN5   sw_input[0].bit4     //
#define IN6   sw_input[0].bit5     //
#define IN7   sw_input[0].bit6     //
#define IN8   sw_input[0].bit7     //
#define IN9   sw_input[1].bit0     //
#define IN10  sw_input[1].bit1     //
#define IN11  sw_input[1].bit2     //
#define IN12  sw_input[1].bit3     //
#define IN13  sw_input[1].bit4     //
#define IN14  sw_input[1].bit5     //
#define IN15  sw_input[1].bit6     //
#define IN16  sw_input[1].bit7     //
#define IN17  sw_input[2].bit0     //
#define IN18  sw_input[2].bit1     //
#define IN19  sw_input[2].bit2     //
#define IN20  sw_input[2].bit3     //
#define IN21  sw_input[2].bit4     //
#define IN22  sw_input[2].bit5     //
#define IN23  sw_input[2].bit6     //
#define IN24  sw_input[2].bit7     //
#define IN25  sw_input[3].bit0     //ASR??
#define IN26  sw_input[3].bit1     //
#define IN27  sw_input[3].bit2     //ABS??
#define IN28  sw_input[3].bit3     //
#define IN29  sw_input[3].bit4     //
#define IN30  sw_input[3].bit5     //
#define IN31  sw_input[3].bit6     //
#define IN32  sw_input[3].bit7     //
#define IN33  sw_input[4].bit0      //
#define IN34  sw_input[4].bit1      //
#define IN35  sw_input[4].bit2      //
#define IN36  sw_input[4].bit3      //
#define IN37  sw_input[4].bit4      //
#define wake_up1  !sw_input[4].bit5      //shaungshan
#define wake_up2 ( !sw_input[4].bit6 ||IN4)     //chongdian
#define wake_up3  (!sw_input[4].bit7 ||IN4)      // ON?


#define  BLACK        1 //0XFF000000   //
#define  BLUE          2 //0XFF0000FF   //
#define  GREEN        3 //0XFF00FF00   //
#define  CYAN          4 //0xFF00FFFF   //
#define  RED            5 //0xFFFF0000   //
#define  MAGENTA    6 //0xFFFF00FF   //
#define  YELLOW      7 //0xFFFFFF00   //
#define  WHITE        8 //0XFFFFFFFF   //
#define  GRAY          9 //0xFFD3D3D3   //
#define  ORANGE      0 //0xFFFFA500   //

/*********************************************************************************************************************************************
*Type Defs
*********************************************************************************************************************************************/
typedef struct 
{
    unsigned long id;
    unsigned char data[8];
    unsigned char len;
    unsigned char type;
} CAN_MESSAGE;

typedef union 
{

    struct 
    {
        unsigned char second; //[00]
        unsigned char minute; //[01]
        unsigned char hour;   //[02]
        unsigned char week;   //[03]
        unsigned char day;    //[04]
        unsigned char month;  //[05]
        unsigned char year;   //[06]
    };
    unsigned char byte[7];
} DATE;

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
} LED_DATA;

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
} SW_DATA;

/*********************************************************************************************************************************************
* function prototype
*********************************************************************************************************************************************/
void Delay_Nus(int t);

void  ADC_Init(void);



void   TIMER_Start(void);

void EEROM_WriteBackLight(void);
void EEROM_ReadBackLight(void);
void EEROM_WriteHMPulse(void);
void EEROM_ReadHMPulse(void);
void EEROM_WriteSingleMiles(void);
void EEROM_ReadSingleMiles(void);
void EEROM_WriteTotalMiles(void);
void EEROM_ReadTotalMiles(void);
void EEROM_WriteSpeedRatio(uint32_t testmode) ;
uint32_t  EEROM_ReadSpeedRatio(void);
void EEP_WriteSingleMileB(void);
void EEP_ReadSingleMileB(void);
unsigned int EEP_ReadFlag(void);
void EEP_WriteFlag(void);

unsigned int ADC_ReadChannelValue(unsigned char chan);

void  WDT_Init(void);
void  WDT_Clear(void);

void LCD_PowerOFF(void);
void LCD_PowerON(void);

void RS_CAN_Init(void);     //250K
void CAN0_ReceiveMessage(CAN_MESSAGE *message);
void CAN1_ReceiveMessage(CAN_MESSAGE *message);
void CAN2_ReceiveMessage(CAN_MESSAGE *message);
void CAN0_SendMessage(CAN_MESSAGE *message);
void CAN1_SendMessage(CAN_MESSAGE *message);
void CAN2_SendMessage(CAN_MESSAGE *message);
void Process_Can(void);

void RTCA_Init(void);


void DS3231_ReadTime(DATE *pTime);
void DS3231_SetTime(DATE *pTime);

void DS1302_Init(void);
void hal_init_ds1302(void);
void DS1302_ReadTime(DATE *pTime);
void DS1302_SetTime(DATE *pTime);

void SW_Input(void);
void LED_Out(void);

/*********************************************************************************************************************************************
* extern variable declarations
*********************************************************************************************************************************************/

extern DATE timer;
extern LED_DATA alarm_led[5];   
extern SW_DATA sw_input[5];       


#endif	/* BSP_H */

