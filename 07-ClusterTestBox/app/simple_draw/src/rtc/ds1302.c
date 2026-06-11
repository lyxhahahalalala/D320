/* ***********************************************************************************************************
 * Filename:	hal_ds1302.c
 * File	ext:	c
 * Author:		
 * Data:		2018/10/10
 * purpose:	DS1302 Driver
 * **********************************************************************************************************/


typedef unsigned char   U8;  /*unsigned 8 bit definition */
typedef unsigned short  U16; /*unsigned 16 bit definition*/
typedef unsigned long   U32; /*unsigned 32 bit definition*/
typedef signed char     S8;   /*signed 8 bit definition */
typedef signed short    S16;  /*signed 16 bit definition*/
typedef signed long     S32;  /*signed 32 bit definition*/
typedef char            CHAR;
typedef unsigned char   BOOL;

#include "ds1302.h"
#include "iodefine.h"


/* Port Data Register */

#if 1
#define  DS1302_SCLK      PORT_ISO.P3.BIT.P3_1               //RTC_CLK   -  P3_3
#define  DS1302_SDA_R   PORT_ISO.PPR3.BIT.PPR3_0        //RTC_IO_R  -  P3_9
#define  DS1302_SDA_S   PORT_ISO.P3.BIT.P3_0               //RTC_IO_S  -  P3_9
#define  DS1302_RST       PORT_ISO.P21.BIT.P21_3               //RTC_RST   -  P3_2




static void delay_ns(U8 a_u8m);

void WriteDs1302Byte(unsigned char temp)
{
        unsigned char i = 0;
        for(i=8; i > 0; i--)
        {
                DS1302_SDA_S = temp & 0x01;
                DS1302_SCLK = 0;
		delay_ns(100);
                DS1302_SCLK = 1;
		delay_ns(100);
                temp >>= 1;
        }
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

unsigned char read_byte()
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
        DS1302_RST = 0;
        DS1302_SCLK = 0;
        DS1302_RST = 1;
	delay_ns(100);
        WriteDs1302Byte(address);
        temp = read_byte();
	delay_ns(100);
        DS1302_RST = 0;
        DS1302_SCLK = 1;
        return temp;
}

/************************************************************************************
// *　函数名称: delay_ns
// *  功　　能: 近似以ns为单位延时
// *  入口参数: 
// *  返 回 值: 
// *  作　　者: Li pengfei
// *  日　　期: 2013.10.10
// *  备    注: 
************************************************************************************/
static void delay_ns(U8 a_u8m)
{
	U8 u8x;
    
	for (u8x=0; u8x<a_u8m; u8x++)
	{
		u8x = u8x;
	} 
}

/* ***********************************************************************************************************
  * Method:    	delay_us
  * Parameter:	delay us
  * Returns:   	void
  * Author:		Li Pengfei
  * Data:		2013-10-10
  * Qualifier: 	延时函数
  * **********************************************************************************************************/ 
static void delay_us(U16 a_us)
{
    U16 a_u16xValue = 0, a_u16iValue = 0;

    for(a_u16xValue=0; a_u16xValue<a_us;a_u16xValue++)
    {
        for(a_u16iValue=0; a_u16iValue<8; a_u16iValue++)
        {
            ;
        }
    }
    
}

static void DS1302Writebyte(U8 a_u8WriteData)
{
    U8 u8LoopIndex = 0;
    U8 u8Temp = 0U;

	//u8Temp = DDR11; --del
	u8Temp |= 0x20;
	//WriteKeyProtectedRegister((U32)(&DDR11), u8Temp, BYTE_ACCESS); --del
	DS1302_SCLK = 0;
    
    delay_us(10);

    for (u8LoopIndex=0; u8LoopIndex < 8; u8LoopIndex++)
    {
        //DS1302_SCLK = 0;
        delay_ns(100);
        if(a_u8WriteData & 0x1)
        	DS1302_SDA_S = 1;
        else
        	DS1302_SDA_S = 0;
        delay_ns(100);
        DS1302_SCLK = 1;
        delay_ns(100);
        DS1302_SCLK = 0;
        a_u8WriteData >>= 1;
    }

    //DS1302_RST = 0;
    DS1302_SDA_S = 0;
}

void DS1302WriteProtect(BOOL WriteEnable)
{
	U8 u8WriteValue = 0;

	if(WriteEnable)
	{
		u8WriteValue = 0;
	}
	else
	{
		u8WriteValue = 1;
	}

	DS1302_RST = 0;
	delay_us(50);
	DS1302_RST = 1;
	DS1302Writebyte(DS1302_REGADDR_CTRL);   /* 写入寄存器地址 */
	DS1302Writebyte(u8WriteValue);   /* 写入寄存器值   */

	DS1302_RST = 0;
}


static U8 DS1302Readbyte(void)
{
    U8 u8Receive_data = 0, u8LoopIndex = 0, u8Mask = 0, u8Temp = 0;

	//u8Temp = DDR11;   --del
	u8Temp &= 0xDFU;
	//WriteKeyProtectedRegister((U32)&DDR11, u8Temp, BYTE_ACCESS);
	DS1302_SCLK = 0;

    for (u8LoopIndex=0; u8LoopIndex<8; u8LoopIndex++)
    {
        if (DS1302_SDA_R == 1)
        {
        	u8Mask = (0x1<<u8LoopIndex);
            u8Receive_data = (u8Receive_data + u8Mask) ;
        }
        else
        {
        	u8Mask = ~(0x1<<u8LoopIndex);
        	u8Receive_data = (u8Receive_data&u8Mask);
        }
        delay_ns(100);
        DS1302_SCLK = 1;
        delay_ns(100);
        DS1302_SCLK = 0;
    }
    //DS1302_SCLK = 0;
    //DS1302_RST = 0;
    return(u8Receive_data);
}

/* ***********************************************************************************************************
  * Method:    	write_ds1302
  * Parameter:	register address, the writen value
  * Returns:   	none
  * Author:		Li Pengfei
  * Data:		2013-10-10
  * Qualifier: 	往DS1302的指定寄存器写入值
  * **********************************************************************************************************/ 
void write_ds1302(U8 a_u8addr, U8 a_u8data)
{
    DS1302_RST = 0;
    delay_us(30);
    DS1302_RST = 1;
    DS1302Writebyte(a_u8addr);   /* 写入寄存器地址 */
    DS1302Writebyte(a_u8data);   /* 写入寄存器值   */
    delay_us(30);
    DS1302_RST = 0;
    //DS1302_SCLK = 1;
}


/* ***********************************************************************************************************
  * Method:    	read_ds1302
  * Parameter:	register address
  * Returns:   	value
  * Author:		Li Pengfei
  * Data:		2013-10-10
  * Qualifier: 	读DS1302的值
  * **********************************************************************************************************/ 
U8 read_ds1302(U8 a_u8addr)
{
    U8 u8RetData = 0;

    DS1302_RST = 0;
    DS1302_SCLK = 0;
    delay_us(50);
    DS1302_RST = 1;
    DS1302Writebyte(a_u8addr);
    u8RetData = DS1302Readbyte();
    //DS1302_SCLK = 1;
    delay_us(50);
    DS1302_RST = 0;
    return(u8RetData);
    
}

void read_ds1302_burst(char *buf)
{
	U8 u8ByteIndex = 0, u8BitIndex = 0, u8Mask = 0, u8Receive_data = 0, u8Temp = 0;

	if(buf == 0)
	{
		return;
	}

	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	DS1302Writebyte(DS1302_REGADDR_CLKBURST | 0x01);

	//u8Temp = DDR11;
	u8Temp &= 0xDFU;
	//WriteKeyProtectedRegister((U32)&DDR11, u8Temp, BYTE_ACCESS);

	DS1302_SCLK = 0;


	for(u8ByteIndex=0; u8ByteIndex<8; u8ByteIndex++)
	{
		u8Receive_data = 0;
	    for (u8BitIndex=0; u8BitIndex<8; u8BitIndex++)
	    {
	        if (DS1302_SDA_R == 1)
	        {
	        	u8Mask = (0x1<<u8BitIndex);
	            u8Receive_data = (u8Receive_data + u8Mask) ;
	        }
	        else
	        {
	        	u8Mask = ~(0x1<<u8BitIndex);
	        	u8Receive_data = (u8Receive_data&u8Mask);
	        }
	        delay_ns(2);
	        DS1302_SCLK = 1;
	        delay_ns(2);
	        DS1302_SCLK = 0;
	    }
	    *buf++ = u8Receive_data;
	}
	DS1302_SCLK = 1;
	DS1302_RST = 0;
}

void write_ds1302_burst(const U8 *buf)
{
	U8 u8ByteLoopIndex = 0, a_u8WriteData = 0;

	if(buf == 0)
	{
		return;
	}

	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	DS1302Writebyte(DS1302_REGADDR_CLKBURST);

	for(u8ByteLoopIndex=0; u8ByteLoopIndex<8; u8ByteLoopIndex++)
	{

		a_u8WriteData = *buf++;
		DS1302Writebyte(a_u8WriteData);
	}

	DS1302_RST = 0;
}

/* ***********************************************************************************************************
  * Method:    	hal_init_ds1302
  * Parameter:	void
  * Returns:   	void
  * Author:		Liubing
  * Data:		2018-1-2
  * Qualifier: 	初始化并启动DS1302
  * **********************************************************************************************************/ 
void hal_init_ds1302(void)
{
    U8 u8Ret = 0;
    U8 sec_tmp;
    //PORT_ISO.PM3.UINT16              &= ~0x020C;            //P3-2, 3 output mode
   // PORT_ISO.PMC3.UINT16            &= ~0x020C;            //P3-2, 3 9  port  mode
    PORT_ISO.PM3.UINT16              &= ~0x0003;
    PORT_ISO.PMC3.UINT16            &=  ~0x0003;   //rtc p3.1 scl P3.0   sda

    PORT_ISO.PM21.UINT16              &= ~0x0008;
    PORT_ISO.PMC21.UINT16            &=  ~0x0008;   //rtc  p21.3 rst
    
    PORT_ISO.PBDC3.UINT16            |= 0x0001;          //P3-9, Bi Direction mode
    PORT_ISO.PIBC3.UINT16            |= 0x0001;          //P3-9 input buffer enable

    PORT_ISO.PU3.BIT.PU3_0           |= 1;           //P3-0 pull up
    PORT_ISO.PODC3.BIT.PODC3_0       |= 1;                //p3-0 open drain



    WriteDs1302(0x8E, 0x00);	                          /* 允许写1302    */
   // WriteDs1302(0x80, 0x80);                          /* 关闭晶振   */

    u8Ret = ReadDs1302(0xc1);
    sec_tmp = ReadDs1302(DS1302_REGADDR_SEC | 0x01);
    if(u8Ret != 'L')
    {
        WriteDs1302(0xc0, 'L');               //write ram flag
	u8Ret = ReadDs1302(0xc1);
		
        WriteDs1302(0x8c, 0x18);				    /* 写年                */
        WriteDs1302(0x88, 0x01);				    /* 写月                */
        WriteDs1302(0x86, 0x08);				    /* 写日                */
        WriteDs1302(0x8A, 0x02);				    /*  week day*/
        WriteDs1302(0x84, 0x12);			           /* 写小时           */
        WriteDs1302(0x82, 0x00);			           /* 写分钟           */
	WriteDs1302(0x80, (sec_tmp & 0x7F));                      /* 启动晶震     */
	  
    }
      
      WriteDs1302(0x8E, 0x00);                          /* 禁止写1302    */
}

static U8 convert_hex_dec(U8 a_u8data)
{
	U8 u8temp = 0;

    u8temp = a_u8data & 0x0F;
	a_u8data = (a_u8data >> 4) * 10;
	u8temp = a_u8data + u8temp;

    return (u8temp);
}

void DS1302_ReadTime(DATE *pTime)
{
    U8 u8Ret = 0;
	
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

    u8Ret = ReadDs1302(DS1302_REGADDR_SEC | 0x01);
    pTime->second    =  u8Ret;
}

void DS1302_SetTime(DATE *pTime)
{
    volatile unsigned short temp = 0;
	#if 0
    PORT_ISO.PM3.UINT16              &= ~0x020C;
    PORT_ISO.PMC3.UINT16            &= ~0x020C;	
    PORT_ISO.PBDC3.UINT16            |= 0x0200;
    PORT_ISO.PIBC3.UINT16            |= 0x0200;
    PORT_ISO.PU3.BIT.PU3_9           = 0x0200;
    PORT_ISO.PODC3.BIT.PODC3_9       = 1;
#endif
    WriteDs1302(0x8E, 0x00);	                          /* 允许写1302    */
    WriteDs1302(0x80, 0x80);                          /* 关闭晶振   */

    WriteDs1302(0x8c, pTime->year);				    /* 写年                */
    WriteDs1302(0x88, pTime->month);				    /* 写月                */
    WriteDs1302(0x86, pTime->day);				    /* 写日                */
    WriteDs1302(0x8A, pTime->week);				    /*  week day*/
    WriteDs1302(0x84, pTime->hour);			           /* 写小时           */
    WriteDs1302(0x82, pTime->minute);			           /* 写分钟           */
	
    WriteDs1302(0x80, 0x00);                          /* 启动晶震     */
    WriteDs1302(0x8E, 0x00);                          /* 禁止写1302    */
}


#endif

