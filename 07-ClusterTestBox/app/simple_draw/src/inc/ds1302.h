#ifndef HAL_DS1302_H_
#define HAL_DS1302_H_

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

#define DS1302_REGADDR_SEC   (U8)0x80
#define DS1302_REGADDR_MIN   (U8)0x82
#define DS1302_REGADDR_HR    (U8)0x84
#define DS1302_REGADDR_DATE  (U8)0x86
#define DS1302_REGADDR_MON   (U8)0x88
#define DS1302_REGADDR_DAY   (U8)0x8A
#define DS1302_REGADDR_YEAR  (U8)0x8C
#define DS1302_REGADDR_CTRL  		(U8)0x8E
#define DS1302_REGADDR_TRICHARGER   (U8)0x90
#define DS1302_REGADDR_CLKBURST      (U8)0xBE


#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址
#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
//闹铃1            
#define DS3231_SALARM1ECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日
#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙

#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位) 

//extern void write_ds1302(U8 a_u8addr, U8 a_u8data);

//extern U8 read_ds1302(U8 a_u8addr);

//extern void hal_init_ds1302(void);

//extern void read_ds1302_burst(char *buf);

//extern void write_ds1302_burst(const U8 *buf);

//extern void DS1302WriteProtect(BOOL WriteEnable);

#endif
