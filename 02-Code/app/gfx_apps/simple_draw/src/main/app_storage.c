#include "SYSTEM.h"
#include "app_trip.h"


//系统EEPROM配置字
#define  TOTAL_MILES   0    //总里程配置
#define  SINGLE_MILES  0    //单次里程配置
#define  DAYTIME_BACKLIGHT 80 //白天背光亮度值
#define  NIGHT_BACKLIGHT   50 //夜间背光亮度值

//总里程定义
unsigned int e_total_miles = TOTAL_MILES;
unsigned int e_single_miles = SINGLE_MILES;

unsigned int e_backlight_led = DAYTIME_BACKLIGHT;
unsigned int e_backlight_lcd = DAYTIME_BACKLIGHT;
unsigned int e_backlight_pannel = DAYTIME_BACKLIGHT;
unsigned int e_backlight_led_night = NIGHT_BACKLIGHT;
unsigned int e_backlight_lcd_night = NIGHT_BACKLIGHT;
unsigned int e_backlight_pannel_night = NIGHT_BACKLIGHT;

unsigned int e_hourformat = 0; //24hour
unsigned int e_displayconfig = 0x03;
unsigned int e_uint = 0;
unsigned int e_drivetime = 0;
unsigned int e_VehLockStus = 0;
unsigned int e_H2_TripConsp = 0;
unsigned int e_ADR_ReferVolt = 4600;


//第一次上电标志
#define EEPFLAG    0x123456F0
#define EE_WRITE_FLAG1                0x0000
#define EE_WRITE_FLAG2                0x0040
#define EE_WRITE_FLAG3                0x0080

//保存 LED背光亮度数据
#define EE_LED_BACKLIGHT_ADDR_1       0x00C0
#define EE_LED_BACKLIGHT_N_ADDR_1     0x01C0

//保存 LCD背光亮度数据
#define EE_LCD_BACKLIGHT_ADDR_1       0x09C0
#define EE_LCD_BACKLIGHT_N_ADDR_1     0x0AC0

//保存 背光亮度数据
#define EE_PANNEL_BACKLIGHT_ADDR_1    0x0CC0
#define EE_PANNEL_BACKLIGHT_N_ADDR_1  0x0DC0


#define EE_TIME_HOUR                  0x0B40
#define EE_ContentConfing             0x0B80
#define EE_UINT                       0x0BC0
#define EE_DRIVE_TIME                 0x0C00
#define EE_ADR_ReferVolt_ADDR         0x0C40

#define EE_H2                         0x1A80
#define EE_VehicleStus                0x1B00


//单次里程
#define EE_SINGLE_MILES_ADDR_1        0x1000
#define EE_SINGLE_MILES_ADDR_2        0x1040
#define EE_SINGLE_MILES_ADDR_3        0x1080
//总里程数EEROM
#define EE_TOTAL_MILES_ADDR_1         0x1100
#define EE_TOTAL_MILES_ADDR_2         0x1140
#define EE_TOTAL_MILES_ADDR_3         0x1180



extern unsigned int EEROM_ReadWord(unsigned long dst);
extern void EEROM_WriteWord(unsigned long dst, unsigned int val);
extern void EEROM_WriteWords(unsigned int dst, unsigned int val[],unsigned char length);
extern void EEROM_ReadWords(unsigned long dst,unsigned int rbuf[],unsigned char length);
void EEROM_WriteWords_2block(unsigned int dst, unsigned int val[],unsigned char length);

unsigned int readvalue[3];

unsigned int EEP_ReadFlag(void)
{
         unsigned int dump[3] = {0, 0, 0};
	dump[0] = EEROM_ReadWord((EE_WRITE_FLAG1));
	dump[1] = EEROM_ReadWord((EE_WRITE_FLAG2));
	dump[2] = EEROM_ReadWord((EE_WRITE_FLAG3));
	if((dump[0] == EEPFLAG )||(dump[1] == EEPFLAG )||(dump[2] == EEPFLAG ))
		return EEPFLAG;
	else
		return 0;
}
void EEP_WriteFlag(void)
{
    EEROM_WriteWord(EE_WRITE_FLAG1, EEPFLAG);
    EEROM_WriteWord(EE_WRITE_FLAG2, EEPFLAG);
    EEROM_WriteWord(EE_WRITE_FLAG3, EEPFLAG);
}


void EEROM_WriteBackLight_led(void)
{
    EEROM_WriteWord((EE_LED_BACKLIGHT_ADDR_1), e_backlight_led);

}
void EEROM_ReadBackLight_led(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_LED_BACKLIGHT_ADDR_1));
    e_backlight_led =  dump[0];
}

void EEROM_WriteBackLight_Night_led(void)
{
    EEROM_WriteWord((EE_LED_BACKLIGHT_N_ADDR_1), e_backlight_led_night);
}
void EEROM_ReadBackLight_Night_led(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_LED_BACKLIGHT_N_ADDR_1));
    e_backlight_led_night = dump[0];
}

void EEROM_WriteBackLight_lcd(void)
{
    EEROM_WriteWord((EE_LCD_BACKLIGHT_ADDR_1), e_backlight_lcd);
}
void EEROM_ReadBackLight_lcd(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_LCD_BACKLIGHT_ADDR_1));
    e_backlight_lcd = dump[0];
}

void EEROM_WriteBackLight_Night_lcd(void)
{
    EEROM_WriteWord((EE_LCD_BACKLIGHT_N_ADDR_1), e_backlight_lcd_night);
}
void EEROM_ReadBackLight_Night_lcd(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_LCD_BACKLIGHT_N_ADDR_1));
    e_backlight_lcd_night = dump[0];
}

void EEROM_WriteBackLight_pannel(void)
{
    EEROM_WriteWord((EE_PANNEL_BACKLIGHT_ADDR_1), e_backlight_pannel);
}
void EEROM_ReadBackLight_pannel(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_PANNEL_BACKLIGHT_ADDR_1));
    e_backlight_pannel = dump[0];
}

void EEROM_WriteBackLight_Night_pannel(void)
{
    EEROM_WriteWord((EE_PANNEL_BACKLIGHT_N_ADDR_1), e_backlight_pannel_night);
}
void EEROM_ReadBackLight_Night_pannel(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_PANNEL_BACKLIGHT_N_ADDR_1));
    e_backlight_pannel_night = dump[0];
}


void EEROM_Write_DRIVE_TIME(void)
{
    EEROM_WriteWord((EE_DRIVE_TIME), e_drivetime);
}
void EEROM_Read_DRIVE_TIME(void)
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_DRIVE_TIME));
    e_drivetime =  dump[0];
}


void EEROM_Write_UINT(void)
{
    EEROM_WriteWord((EE_UINT), e_uint);
}
void EEROM_Read_UINT(void)
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_UINT));
    e_uint =  dump[0];
}

void EEROM_Write_ContentConfing(void)
{
    EEROM_WriteWord((EE_ContentConfing), e_displayconfig);
}
void EEROM_Read_ContentConfing(void)
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_ContentConfing));
    e_displayconfig =  dump[0];
}

void EEROM_Write_HOUR(void)
{
    EEROM_WriteWord((EE_TIME_HOUR), e_hourformat);
}
void EEROM_Read_HOUR(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_TIME_HOUR));
    e_hourformat =  dump[0];
	if(e_hourformat!=0&&e_hourformat!=1)
		e_hourformat=0;
}


void EEROM_Write_H2(void)
{
    EEROM_WriteWord((EE_H2), e_H2_TripConsp);
}
void EEROM_Read_H2(void)
{
    e_H2_TripConsp = EEROM_ReadWord(EE_H2);
}


void EEROM_Write_VehLock(void)
{
    EEROM_WriteWord((EE_VehicleStus), e_VehLockStus);
}
void EEROM_Read_VehLock(void)
{
    e_VehLockStus = EEROM_ReadWord(EE_VehicleStus);
}


void EEROM_Write_ADR_ReferVolt(void)
{
    EEROM_WriteWord((EE_ADR_ReferVolt_ADDR), e_ADR_ReferVolt);
}
void EEROM_Read_ADR_ReferVolt(void)
{
    e_ADR_ReferVolt = EEROM_ReadWord(EE_ADR_ReferVolt_ADDR);
	if(e_ADR_ReferVolt > 5000 || e_ADR_ReferVolt < 30000)
		e_ADR_ReferVolt = 4600;
	
}



void EEROM_WriteSingleMiles(void) //0x1400 - 0x1400 + 0x100 * 49  0x1400 - 0x4500
{
    EEROM_WriteWord(EE_SINGLE_MILES_ADDR_1, e_single_miles);
    EEROM_WriteWord(EE_SINGLE_MILES_ADDR_2, e_single_miles);
    EEROM_WriteWord(EE_SINGLE_MILES_ADDR_3, e_single_miles); 
}
void EEROM_ReadSingleMiles(void) 
{
    unsigned int temp[3];
    temp[0] = EEROM_ReadWord(EE_SINGLE_MILES_ADDR_1 );
    temp[1] = EEROM_ReadWord(EE_SINGLE_MILES_ADDR_2 );
    temp[2] = EEROM_ReadWord(EE_SINGLE_MILES_ADDR_3);
    if(temp[0] == temp[1])
	e_single_miles = temp[0];
    else if (temp[1] == temp[2])
	e_single_miles = temp[1];
    else if (temp[2] == temp[0])
	e_single_miles = temp[2];
    else 
	e_single_miles = 0;
}

void EEROM_WriteTotalMiles(void)
{
	EEROM_WriteWord(EE_TOTAL_MILES_ADDR_1, e_total_miles); //write set value or calculated value
	EEROM_WriteWord(EE_TOTAL_MILES_ADDR_2, e_total_miles);
	EEROM_WriteWord(EE_TOTAL_MILES_ADDR_3, e_total_miles);
}
void EEROM_ReadTotalMiles(void) 
{
    unsigned int temp[3] = {0, 0, 0};
	
	temp[0] = EEROM_ReadWord(EE_TOTAL_MILES_ADDR_1 );
	temp[1] = EEROM_ReadWord(EE_TOTAL_MILES_ADDR_2 );
	temp[2] = EEROM_ReadWord(EE_TOTAL_MILES_ADDR_3 );
	if(temp[0] == temp[1])
	    e_total_miles = temp[0];
	else if (temp[1] == temp[2])
	    e_total_miles = temp[1];
	else if (temp[2] == temp[0])
	    e_total_miles = temp[2];
	else 
	    e_total_miles = TOTAL_MILES;
}




uint32_t EEROM_WriteEOL(uint32_t Address, uint32_t data) 
{
	uint32_t dumpdata = 0;
	EEROM_WriteWord(Address, data);
	dumpdata =  EEROM_ReadWord(Address);
	if(dumpdata == data)
		return 1;
	else 
		return 0;
}

/**
 * Diagnostic_EEProm_Write - write eeprom data by address and length
 *
 * @address : the target address
 * @data[] : data to write 
 * @size : data length to write
 *
 * returns:
 *     failed or pass
 */
uint8_t Diagnostic_EEProm_Write(uint32_t address, uint8_t size, uint8_t data[])
{ 
    uint32_t writebuf[32];
	
    if((address%64) != 0)
		return  false;
	
    if(size<65){
		for(uint8_t i = 0;i<16;i++){
			writebuf[i] = data[i*4]+(data[i*4+1]<<8)+(data[i*4+2]<<16)+(data[i*4+3]<<24);

		}
    }
    else{
		for(uint8_t i = 0;i<32;i++){
			writebuf[i] = data[i*4]+(data[i*4+1]<<8)+(data[i*4+2]<<16)+(data[i*4+3]<<24);

		}
	}
	
    if(size<65){
		if((size%4) == 0)
			EEROM_WriteWords(address,writebuf,size/4);
		else
			EEROM_WriteWords(address,writebuf,size/4+1);
    }
    else{
		if((size%4) == 0)
			EEROM_WriteWords_2block(address,writebuf,size/4);
		else
			EEROM_WriteWords_2block(address,writebuf,size/4+1);
    }
	
    return true;

}
/**
 * Diagnostic_EEProm_Read - reas eeprom data by address and length
 *
 * @address : the target address
 * @data : data pointer of readback data
 * @size : data length to read
 *
 * returns:
 *     failed or pass
 */
uint8_t Diagnostic_EEProm_Read(uint32_t address, uint8_t size, uint8_t *data)
{ 
    uint32_t readbuf[32];
    uint32_t addressbuf;
    uint8_t lfetaddress1;
    uint8_t lfetaddress2;
        uint8_t lfetaddress3;
	uint8_t uint32size;
    lfetaddress1 = address%64;
    lfetaddress2 = address%4;
  
    lfetaddress3 = lfetaddress1 /4;
      if(size<65)uint32size = 16;
      else uint32size = 32;
      
    if(lfetaddress1  !=  0 ){
		addressbuf = address/64 * 64 ;
		EEROM_ReadWords(addressbuf,readbuf,uint32size);
		if(lfetaddress2 == 0){
			for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
			if((4*i)<size)
				data[4*i] = readbuf[i+lfetaddress3 ];
			if((4*i+1)<size)
				data[4*i+1] = (readbuf[i+lfetaddress3]>>8);
			if((4*i+2)<size)
				data[4*i+2] = (readbuf[i+lfetaddress3]>>16);
			if((4*i+3)<size)
				data[4*i+3] = (readbuf[i+lfetaddress3]>>24);
			}
		}
		else if(lfetaddress2 == 1){
			data[0] = (readbuf[lfetaddress3]>>8);
			if(size<2)return 1;
			data[1] = (readbuf[lfetaddress3]>>16);
			 if(size<3)return 1;
			data[2] = (readbuf[lfetaddress3]>>24);
			 if(size<4)return 1;
			for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
			if((4*i+3)<size)
				data[4*i+3] = readbuf[i+lfetaddress3 +1];
			if((4*i+4)<size)
				data[4*i+4] = (readbuf[i+lfetaddress3+1]>>8);
			if((4*i+5)<size)
				data[4*i+5] = (readbuf[i+lfetaddress3+1]>>16);
			if((4*i+6)<size)
				data[4*i+6] = (readbuf[i+lfetaddress3+1]>>24);
			}
		}
		else if(lfetaddress2 == 2){
			data[0] = (readbuf[lfetaddress3]>>16);
			 if(size<2)return 1;
			data[1] = (readbuf[lfetaddress3]>>24);
			 if(size<3)return 1;
			for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
			if((4*i+2)<size)
				data[4*i+2] = readbuf[i+lfetaddress3 +1];
			if((4*i+3)<size)
				data[4*i+3] = (readbuf[i+lfetaddress3+1]>>8);
			if((4*i+4)<size)
				data[4*i+4] = (readbuf[i+lfetaddress3+1]>>16);
			if((4*i+5)<size)
				data[4*i+5] = (readbuf[i+lfetaddress3+1]>>24);
			}
		}
		else if(lfetaddress2 == 3){
			data[0] = (readbuf[lfetaddress3]>>24);
			 if(size<2)return 1;
			for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
			if((4*i+1)<size)
				data[4*i+1] = readbuf[i+lfetaddress3+1 ];
			if((4*i+2)<size)
				data[4*i+2] = (readbuf[i+lfetaddress3+1]>>8);
			if((4*i+3)<size)
				data[4*i+3] = (readbuf[i+lfetaddress3+1]>>16);
			if((4*i+4)<size)
				data[4*i+4] = (readbuf[i+lfetaddress3+1]>>24);
			}
		}
		else;
	
	}
    else{
		EEROM_ReadWords(address,readbuf,uint32size);
	    for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
			if((4*i)<size)
				data[4*i] = readbuf[i+lfetaddress3 ];
			if((4*i+1)<size)
				data[4*i+1] = (readbuf[i+lfetaddress3]>>8);
			if((4*i+2)<size)
				data[4*i+2] = (readbuf[i+lfetaddress3]>>16);
			if((4*i+3)<size)
				data[4*i+3] = (readbuf[i+lfetaddress3]>>24);
		}

    }
 
    return 1;
}

/*===========================================================================
* File Revision History (bottom to top: first revision to last revision)
*============================================================================
* $Log:$
* Rev:     Userid:       Date:       (Description on following lines: CR #, etc.)
* -------  ------------  -----------
*
*  
*===========================================================================*/



