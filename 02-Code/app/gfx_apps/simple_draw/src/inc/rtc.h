#ifndef HAL_RTC_H_
#define HAL_RTC_H_


typedef enum {
    RTC_DS1302 = 0,
    RTC_DS3231 = 1
	
} EXT_RTC_e;

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
} DATE_t;
extern DATE_t timer;


void Internal_RTC_init(void);
void Internal_RTC_ReadTime(DATE_t *pTime); //internal rtc
void Internal_RTC_SetTime(DATE_t *pTime);  //internal rtc


void External_RTC_init(uint8_t rtc_chip);
void External_RTC_ReadTime(DATE_t *pTime); //external rtc
void External_RTC_SetTime(DATE_t *pTime);  //external rtc




#endif


