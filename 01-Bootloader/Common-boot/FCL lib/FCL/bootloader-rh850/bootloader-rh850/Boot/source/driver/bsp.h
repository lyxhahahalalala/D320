/* 
 * File:   bsp.h
 * Author: mpszm
 *
 * Created 
 */

#ifndef BSP_H
#define	BSP_H

//#include <xc.h>
#define FCY 16000000
//#include <libpic30.h>

#include "R_typedefs.h"

/*
 // FOSC
#pragma config FOSFPR = XT_PLL8         // Oscillator (XT w/PLL 4x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FBS
#pragma config BWRP = WR_PROTECT_BOOT_OFF// Boot Segment Program Memory Write Protect (Boot Segment Program Memory may be written)
#pragma config BSS = NO_BOOT_CODE       // Boot Segment Program Flash Memory Code Protection (No Boot Segment)
#pragma config EBS = NO_BOOT_EEPROM     // Boot Segment Data EEPROM Protection (No Boot EEPROM)
#pragma config RBS = NO_BOOT_RAM        // Boot Segment Data RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WR_PROT_SEC_OFF   // Secure Segment Program Write Protect (Disabled)
#pragma config SSS = NO_SEC_CODE        // Secure Segment Program Flash Memory Code Protection (No Secure Segment)
#pragma config ESS = NO_SEC_EEPROM      // Secure Segment Data EEPROM Protection (No Segment Data EEPROM)
#pragma config RSS = NO_SEC_RAM         // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = GSS_OFF            // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)
 
 */



/* AD???? */
#define AD_E_DR  TRISBbits.TRISB5
#define AD_A_DR  TRISBbits.TRISB2
#define AD_B_DR  TRISBbits.TRISB3
#define AD_C_DR  TRISBbits.TRISB4

#define AD_E LATBbits.LATB5
#define AD_A LATBbits.LATB2
#define AD_B LATBbits.LATB3
#define AD_C LATBbits.LATB4

void ADC_Init(void);
unsigned int ADC_Read(unsigned char chs);

//?????
unsigned int read_VOL(void);
//????????,0=??1???1,????
unsigned int read_ADR(unsigned char chan);

extern unsigned int freq[2];
void INT1_Init(void);
void INT2_Init(void);
void TIMER3_Init(void);

void ClrWdt(void);
void __delay_us(int t);

void   init_wdt(void);
void   adce_init(void);
void   init_buzzer(void);
void   start_timer(void);
void   stop_buzzer(void);

/*
 CAN
 */
typedef struct 
{
    unsigned long id;
    unsigned char data[8];
    unsigned char len;
    unsigned char type;
} CAN_MESSAGE;


void init_clocks(void);
void init_led(void);
void disable_bl(void);
void enable_bl(void);

int adce_read(unsigned char channel);

void RS_CAN_init(void);     //250K
void CAN0_ReceiveMessage(CAN_MESSAGE *message);
void CAN1_ReceiveMessage(CAN_MESSAGE *message);
void CAN2_ReceiveMessage(CAN_MESSAGE *message);
void CAN0_SendMessage(CAN_MESSAGE *message);
void CAN1_SendMessage(CAN_MESSAGE *message);
void CAN2_SendMessage(CAN_MESSAGE *message);
void Process_Can(void);

void Interrupt10ms(void);

/*
 EEROM????
 */
unsigned int EEROM_ReadLowMiles(unsigned long address);
void EEROM_WriteLowMiles(unsigned long address, unsigned int value);
unsigned int EEROM_ReadWord(unsigned long dst);
void EEROM_WriteWord(unsigned long dst, unsigned int val);

/*
 ????????
 */
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

void init_io(void);
extern DATE timer;
void init_rtca(void);
void ReadTime(DATE *pTime);
void DS3231_ReadTime(DATE *pTime);
void DS3231_SetTime(DATE *pTime);

void hal_init_ds1302(void);
void DS1302_ReadTime(DATE *pTime);
void DS1302_SetTime(DATE *pTime);

extern float sm_steps[6];
extern float sm_new_value[6];
extern float sm_old_value[6];
extern unsigned char sm_speed[6];

void SM_Init(void);
void SM_Update_0(void); //?????
void SM_Update_1(void); //???
void SM_Update_2(void); //?????
void SM_Update_3(void); //???
void SM_Update_4(void); //???
void SM_Update_5(void); //???
void SM_Reset(void);       //??????
void SM_Zero(void);        //
void sm_run(int channel);
void zerosm(void);

/*
 ?????
 */
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

extern LED_DATA alarm_led[4];    //???????
void LED_Out(void);

#define BUZZ  alarm_led[0].bit0  //??? 
#define CS_A  alarm_led[0].bit4  //4051
#define CS_B  alarm_led[0].bit3 
#define CS_C  alarm_led[0].bit2 
#define CS_E  alarm_led[0].bit5 

#define PO1   alarm_led[0].bit6 //????    
#define PO2   alarm_led[0].bit7 //????  

#define LED1  alarm_led[1].bit0 //????        
#define LED2  alarm_led[1].bit1 //????                //??  ??         
#define LED3  alarm_led[1].bit2 //??????  
#define LED4  alarm_led[1].bit3 //?????      
#define LED5  alarm_led[1].bit4 //????    
#define LED6  alarm_led[1].bit5 //?????? 
#define LED7  alarm_led[1].bit6 //?????  
#define LED8  alarm_led[1].bit7 //?????  
#define LED9  alarm_led[2].bit0 //?????
#define LED10 alarm_led[2].bit1 //?????              //??  ABS
#define LED11 alarm_led[2].bit2 //?????
#define LED12 alarm_led[2].bit3 //???????   
#define LED13 alarm_led[2].bit4 //??????            //??  WARN???????
#define LED14 alarm_led[2].bit5 //OBD??                 //??  STOP???????
#define LED15 alarm_led[2].bit6 //??? ??????     //??  ??????
#define LED16 alarm_led[2].bit7 //????                
#define LED17 alarm_led[3].bit0 //?????  
#define LED18 alarm_led[3].bit1 //??????
#define LED19 alarm_led[3].bit2 //?????
#define LED20 alarm_led[3].bit3 //?????
#define LED21 alarm_led[3].bit4 //???
#define LED22 alarm_led[3].bit5 //????? 
#define LED23 alarm_led[3].bit6 //?????               //??  ????
#define LED24 alarm_led[3].bit7 //????

/*
 ????????
 */
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

extern SW_DATA sw_input[5];       //????????
void SW_Input(void);

#define IN1   sw_input[0].bit0     //??
#define IN2   !sw_input[0].bit1     //??
#define IN3   !sw_input[0].bit2     //??
#define IN4   !sw_input[0].bit3     //??
#define IN5   sw_input[0].bit4     //??
#define IN6   sw_input[0].bit5     //??
#define IN7   sw_input[0].bit6     //??
#define IN8   sw_input[0].bit7     //??
#define IN9   sw_input[1].bit0     //??
#define IN10  sw_input[1].bit1     //??
#define IN11  sw_input[1].bit2     //??
#define IN12  sw_input[1].bit3     //
#define IN13  sw_input[1].bit4     //
#define IN14  sw_input[1].bit5     //???????????   ??,????
#define IN15  sw_input[1].bit6     //??????????? 
#define IN16  sw_input[1].bit7     //
#define IN17  sw_input[2].bit0     //
#define IN18  sw_input[2].bit1     //??1????
#define IN19  sw_input[2].bit2     //??2????
#define IN20  sw_input[2].bit3     //???????
#define IN21  sw_input[2].bit4     //?????
#define IN22  sw_input[2].bit5     //????????
#define IN23  sw_input[2].bit6     //???????
#define IN24  sw_input[2].bit7     //???????
#define IN25  sw_input[3].bit0     //ASR??
#define IN26  sw_input[3].bit1     //??????
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
#define wake_up2  !sw_input[4].bit6      //chongdian
#define wake_up3  !sw_input[4].bit7      // ON?

/*
 LCD ????
 */
#define  BLACK        1 //0XFF000000   //??
#define  BLUE          2 //0XFF0000FF   //?? 0X03
#define  GREEN        3 //0XFF00FF00   //?? 0X1C
#define  CYAN          4 //0xFF00FFFF   //??
#define  RED            5 //0xFFFF0000   //??
#define  MAGENTA    6 //0xFFFF00FF   //??
#define  YELLOW      7 //0xFFFFFF00   //??
#define  WHITE        8 //0XFFFFFFFF   //?? 0XFF
#define  GRAY          9 //0xFFD3D3D3   //??
#define  ORANGE      0 //0xFFFFDAB9   //??

//2016-10-31 ??1-DAV 2-AV1 6-AV2 
void LCD_CHS(unsigned char mode);       //????????? 1-AV2 2-MCU 4-AV1  ???1-DAV 2-AV1 6-AV2 
void LCD_BackLight(unsigned char back); //LCD?????? 0-7 0?? 7??
void LCD_Clear(void);                   //?? ????
void LCD_Init(void);                    //???
void LCD_DrawImage1BPP(unsigned int x, unsigned int y,
        const unsigned char *img,
        unsigned char fcolor, unsigned char bcolor);                 //???,8??,???
void LCD_DrawImage1BPPZOOM_1(unsigned int x, unsigned int y,
        const  unsigned char *img,
        unsigned char fcolor, unsigned char bcolor);                 //???,1??,???
void LCD_DrawImage1BPPZOOM_4(unsigned int x, unsigned int y,
        const unsigned char *img,
        unsigned char fcolor, unsigned char bcolor);                 //???,??4?,??? 
void LCD_DrawRect(unsigned int x, unsigned int y,                    //W???0?,???
        unsigned int w, unsigned int h, unsigned char color);        //???

/*
 ?????
 */
void TIMER1_Init(void);   //???? //10ms ??
void BL_Init(void);       //?????????


#endif	/* BSP_H */

