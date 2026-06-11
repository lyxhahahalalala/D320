/************************************************************************************************************
 * Filename:	ds1302.c
 * File	ext:	c
 * Author:		
 * Data:		2018/10/10
 * purpose:	DS1302 Driver
 ***********************************************************************************************************/

#include "iodefine.h"
#include "r_typedefs.h"
#include "rtc.h"
#include "ds1302.h"



/* Port Data Register */
#define DS1302_SCLK   PORT_ISO.P3.BIT.P3_1      //RTC_CLK   -  P3_1
#define DS1302_SDA_R  PORT_ISO.PPR3.BIT.PPR3_0  //RTC_IO_R  -  P3_0
#define DS1302_SDA_S  PORT_ISO.P3.BIT.P3_0      //RTC_IO_S  -  P3_0
#define DS1302_RST    PORT_ISO.P21.BIT.P21_7    //RTC_RST   -  P21_7

/* ds1302 Data Register */
#define DS1302_REGADDR_SEC         (uint8_t)0x80
#define DS1302_REGADDR_MIN         (uint8_t)0x82
#define DS1302_REGADDR_HR          (uint8_t)0x84
#define DS1302_REGADDR_DATE        (uint8_t)0x86
#define DS1302_REGADDR_MON         (uint8_t)0x88
#define DS1302_REGADDR_DAY         (uint8_t)0x8A
#define DS1302_REGADDR_YEAR        (uint8_t)0x8C
#define DS1302_REGADDR_CTRL        (uint8_t)0x8E
#define DS1302_REGADDR_TRICHARGER  (uint8_t)0x90
#define DS1302_REGADDR_CLKBURST    (uint8_t)0xBE



/************************************************************************************
 * 函数名称: delay_ns
 * 功　　能: 近似以ns为单位延时
 * 入口参数: 
 * 返 回 值: 
 * 作　　者: Li pengfei
 * 日　　期: 2013.10.10
 * 备    注: 
 ************************************************************************************/
void delay_ns(uint8_t a_u8m)
{
	uint8_t u8x;
	if(a_u8m>120)
		a_u8m = 120;//防止超出范围
    
	for (u8x=0; u8x<a_u8m*2; u8x++)
	{
		u8x = u8x;
	} 
}

/************************************************************************************
 * Method:    	delay_us
 * Parameter:	delay us
 * Returns:   	void
 * Author:		Li Pengfei
 * Data:		2013-10-10
 * Qualifier: 	延时函数
 ************************************************************************************/ 
void delay_us(uint16_t a_us)
{
    uint16_t a_u16xValue = 0, a_u16iValue = 0;

    for(a_u16xValue=0; a_u16xValue<a_us;a_u16xValue++)
    {
        for(a_u16iValue=0; a_u16iValue<8; a_u16iValue++)
        {
            ;
        }
    }
    
}


void WriteDs1302Byte(unsigned char temp)
{
	unsigned char i = 0;
	__DI();
	for(i=8; i > 0; i--)
	{
		DS1302_SDA_S = temp & 0x01;
		DS1302_SCLK = 0;
		delay_ns(100);
		DS1302_SCLK = 1;
		delay_ns(100);
		temp >>= 1;
	}
	__EI();
}
void WriteDs1302(unsigned char address, unsigned char dat)
{
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	delay_ns(100);
	WriteDs1302Byte(address);
	WriteDs1302Byte(dat);
	delay_ns(100);
	DS1302_RST = 0;
}


unsigned char ReadDs1302Byte(void)
{ 
	unsigned char i = 0, temp0 = 0;
	for(i = 8; i > 0; i--)
	{
		if(DS1302_SDA_R)
			temp0 = temp0 | 0x80;
		DS1302_SCLK = 1;
		delay_ns(100);
		DS1302_SCLK = 0;
		delay_ns(100);
		temp0 = temp0 >> 1;
	}
	return temp0;
}
unsigned char ReadDs1302(unsigned char address)
{
	unsigned char temp = 0;
	__DI();
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	delay_ns(100);
	WriteDs1302Byte(address);
	temp = ReadDs1302Byte();
	delay_ns(100);
	DS1302_RST = 0;
	DS1302_SCLK = 1;
	__EI();
	return temp;
}

void DS1302_ReadTime(DATE_t *pTime)
{
    uint8_t u8Ret = 0;
	
    u8Ret = ReadDs1302(DS1302_REGADDR_SEC | 0x01);
    pTime->second    =  u8Ret;
	
    u8Ret = ReadDs1302(DS1302_REGADDR_MIN | 0x01);
    pTime->minute    =  u8Ret;

    u8Ret = ReadDs1302(DS1302_REGADDR_HR | 0x01);
	pTime->hour       =  u8Ret;

    u8Ret = ReadDs1302(DS1302_REGADDR_DAY | 0x01);
    pTime->week       =  u8Ret;

    u8Ret = ReadDs1302(DS1302_REGADDR_DATE | 0x01);
    pTime->day         =  u8Ret;

    u8Ret = ReadDs1302(0x89);
    pTime->month     =  u8Ret;

    u8Ret = ReadDs1302(DS1302_REGADDR_YEAR | 0x01);
    pTime->year        =  u8Ret;
}

void DS1302_SetTime(DATE_t *pTime)
{
    volatile unsigned short temp = 0;
	
    WriteDs1302(0x8E, 0x00);          /* 允许写1302    */
    WriteDs1302(0x80, 0x80);          /* 关闭晶振   */

    WriteDs1302(0x8c, pTime->year);   /* 写年 */
    WriteDs1302(0x88, pTime->month);  /* 写月 */
    WriteDs1302(0x86, pTime->day);    /* 写日 */
    WriteDs1302(0x8A, pTime->week);   /* 写星期 */

    WriteDs1302(0x84, pTime->hour);   /* 写小时 */
    WriteDs1302(0x82, pTime->minute); /* 写分钟 */
	
    WriteDs1302(0x80, 0x00);          /* 启动晶震 */
    WriteDs1302(0x8E, 0x00);          /* 禁止写1302 */
	
}

void DS1302_init(void)
{
    uint8_t u8Ret = 0;
    uint8_t sec_tmp;

    PORT_ISO.PM3.UINT16   &= ~0x0003; //P3-1 output mode
    PORT_ISO.PMC3.UINT16  &= ~0x0003; //P3-1  port  mode

    PORT_ISO.PM21.UINT16  &= ~0x0080; //P21-7 output mode
    PORT_ISO.PMC21.UINT16 &= ~0x0080; //P21-7  port  mode
	
    PORT_ISO.PBDC3.UINT16 |=  0x0001; //P3-0, Bi Direction mode
    PORT_ISO.PIBC3.UINT16 |=  0x0001; //P3-0 input buffer enable

    PORT_ISO.PU3.BIT.PU3_0 = 1; //P3-0 pull up
    PORT_ISO.PODC3.BIT.PODC3_0 = 1; //p3-0 open drain
	
	
	WriteDs1302(0x8E, 0x00); /* 允许写1302    */
	// WriteDs1302(0x80, 0x80); /* 关闭晶振   */
	
    u8Ret = ReadDs1302(0xc1);
    sec_tmp = ReadDs1302(DS1302_REGADDR_SEC | 0x01);
    if(u8Ret != 'L')
    {
		WriteDs1302(0xc0, 'L');    //write ram flag
		u8Ret = ReadDs1302(0xc1);
		
		WriteDs1302(0x8c, 0x18);   /* 写年 */
		WriteDs1302(0x88, 0x01);   /* 写月 */
		WriteDs1302(0x86, 0x08);   /* 写日 */
		WriteDs1302(0x8A, 0x02);   /* 写星期 */
		WriteDs1302(0x84, 0x12);   /* 写小时 */
		WriteDs1302(0x82, 0x00);   /* 写分钟 */
		WriteDs1302(0x80, (sec_tmp & 0x7F)); /* 启动晶震 */
	  
    }
	WriteDs1302(0x8E, 0x00);  /* 禁止写1302 */
	
}

uint8_t convert_hex_dec(uint8_t a_u8data)
{
	uint8_t u8temp = 0;

    u8temp = a_u8data & 0x0F;
	a_u8data = (a_u8data >> 4) * 10;
	u8temp = a_u8data + u8temp;

    return (u8temp);
}



