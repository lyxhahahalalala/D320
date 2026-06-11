#include "SYSTEM.h"

//系统EEPROM配置字
//总里程配置
#define  TOTAL_MILES   0L
//单次里程配置
#define  SINGLE_MILES  0
//百米脉冲数
#define  HM_PLUSE     103
//背光亮度值
#define  BACKLIGHT     400
//定义模块地址 0x01 为前部模块 0x02 为后模块  0x03为顶部模块   0x04为仪表
#define  MT_ADDR      4
//程序版本号
#define EEPFLAG 0x12345678
#define EE_WRITE_FLAG 0x0Ac0  //第一次商店标志

//总里程定义
unsigned int e_total_miles  = TOTAL_MILES;

int e_address      = MT_ADDR;          //EEPROM存储地址
int e_single_miles = SINGLE_MILES;
int e_single_milesB  = TOTAL_MILES;
int e_backlight    = BACKLIGHT;
int e_hm_pulse     = HM_PLUSE;

//KValue and SpeedRatio

int e_Kvalue = 1;
int e_TestMode = 1;

//总里程数EEROM  0x4600-0x7700
#define  EE_TOTAL_MILES_ADDR_1  0x4600
#define  EE_TOTAL_MILES_ADDR_2  0x4640
#define  EE_TOTAL_MILES_ADDR_3  0x4680

//单次里程
// 0x1400 - 0x4500
#define EE_SINGLE_MILES_ADDR_1  0x1400
#define EE_SINGLE_MILES_ADDR_2  0x1440
#define EE_SINGLE_MILES_ADDR_3  0x1480

///保存 背光亮度数据
#define  EE_BACKLIGHT_ADDR_1  0x00C0
#define  EE_BACKLIGHT_ADDR_2  0x01C0
#define  EE_BACKLIGHT_ADDR_3  0x02C0

////kvalue and speed ratio
#define  EE_KVALUE_1  0x03C0
#define  EE_KVALUE_2  0x04C0
#define  EE_KVALUE_3  0x05C0

//百米脉冲数EEROM备份
#define  EE_SPEEDRATIO_ADDR_1  0x06C0
#define  EE_SPEEDRATIO_ADDR_2  0x07C0
#define  EE_SPEEDRATIO_ADDR_3  0x08C0

extern unsigned int EEROM_ReadWord(unsigned long dst);
extern void EEROM_WriteWord(unsigned long dst, unsigned int val);
extern void fdl_readwords(unsigned int dst,unsigned char length);
extern void fdl_writewords(unsigned int dst, unsigned int wBuf_u32[],unsigned char length);
extern void EEROM_WriteWords(unsigned int dst, unsigned int val[],unsigned char length) ;
extern void EEROM_ReadWords(unsigned long dst,unsigned char length);
unsigned int singleMileB[3] = {0,0,0};
unsigned int singlemileTemp[6] = {0,0,0,0,0,0};
unsigned int singlemileRead[6] = {0,0,0,0,0,0};
unsigned int readvalue[3];
void EEP_WriteSingleMileB(void)
{

	//EEROM_WriteWords(0x7800,test,3);

}
void EEP_ReadSingleMileB(void)
{
	EEROM_ReadWords(0x7800,3);
	e_single_milesB = readvalue[0];

	    if (readvalue[0] == readvalue[1]) 
		e_single_milesB = readvalue[0];
    else if (readvalue[1] == readvalue[2]) 
		e_single_milesB = readvalue[1];
    else if (readvalue[2] == readvalue[1]) 
		e_single_milesB = readvalue[2];
    else 
		e_single_milesB = 0;
}
unsigned int EEP_ReadFlag(void)
{

    return(EEROM_ReadWord(EE_WRITE_FLAG));

}
void EEP_WriteFlag(void)
{

    EEROM_WriteWord(EE_WRITE_FLAG, EEPFLAG);

}
void EEROM_WriteBackLight(void)
{
    EEROM_WriteWord((EE_BACKLIGHT_ADDR_1), e_backlight);
    EEROM_WriteWord((EE_BACKLIGHT_ADDR_2), e_backlight);
    EEROM_WriteWord((EE_BACKLIGHT_ADDR_3), e_backlight);
}

void EEROM_ReadBackLight(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_BACKLIGHT_ADDR_1));
    dump[1] = EEROM_ReadWord((EE_BACKLIGHT_ADDR_2));
    dump[2] = EEROM_ReadWord((EE_BACKLIGHT_ADDR_3));

    if (dump[0] == dump[1]) 
		e_backlight = dump[0];
    else if (dump[1] == dump[2]) 
		e_backlight = dump[1];
    else if (dump[2] == dump[1]) 
		e_backlight = dump[2];
    else 
		e_backlight = BACKLIGHT;
}

void EEROM_WriteSpeedRatio(uint32_t testmode) 
{
    EEROM_WriteWord((EE_SPEEDRATIO_ADDR_1), testmode);
    EEROM_WriteWord((EE_SPEEDRATIO_ADDR_2), testmode);
    EEROM_WriteWord((EE_SPEEDRATIO_ADDR_3), testmode);
}

uint32_t  EEROM_ReadSpeedRatio(void) 
{
    unsigned int dump[3] = {0, 0, 0};
	uint32_t temp;
    dump[0] = EEROM_ReadWord((EE_SPEEDRATIO_ADDR_1));
    dump[1] = EEROM_ReadWord((EE_SPEEDRATIO_ADDR_1));
    dump[2] = EEROM_ReadWord((EE_SPEEDRATIO_ADDR_1));

    if (dump[0] == dump[1]) 
		temp = dump[0];
    else if (dump[1] == dump[2]) 
		temp = dump[1];
    else if (dump[2] == dump[1]) 
		temp = dump[2];
    else 
		temp = 1;
	return  temp;
}

void EEROM_WriteSingleMiles(void) //0x1400 - 0x1400 + 0x100 * 49  0x1400 - 0x4500
{
    uint8_t cnt;

    for(cnt = 0;cnt<50;cnt++)
    {
	if(e_total_miles< 200000*(cnt+1)) // within 2w km,use address part cnt
	{
	   // EEROM_WriteWord((EE_SINGLE_MILES_ADDR_1 + 0x100 *cnt), e_single_miles);
	   // EEROM_WriteWord((EE_SINGLE_MILES_ADDR_2 + 0x100 *cnt), e_single_miles);
	  //  EEROM_WriteWord((EE_SINGLE_MILES_ADDR_3 + 0x100 *cnt), e_single_miles);
	  EEROM_WriteWords(EE_SINGLE_MILES_ADDR_1 + 0x100 *cnt,singlemileTemp,6);//
	    break;
	}
    }
}

void EEROM_ReadSingleMiles(void) 
{
    unsigned int dump[3];
    uint8_t cnt;
    for(cnt = 0;cnt<50;cnt++)
    {
    	if(e_total_miles< (200000*(cnt+1) +1)) // within 2w km,use address part cnt
	{
	   // dump[0] = EEROM_ReadWord((EE_SINGLE_MILES_ADDR_1 + 0x100 *cnt));
	   // dump[1] = EEROM_ReadWord((EE_SINGLE_MILES_ADDR_2 + 0x100 *cnt));
	   // dump[2] = EEROM_ReadWord((EE_SINGLE_MILES_ADDR_3 + 0x100 *cnt));
	   EEROM_ReadWords(EE_SINGLE_MILES_ADDR_1 + 0x100 *cnt,6);
	    break;
	}
    }

    if (singlemileRead[0] == singlemileRead[1]) 
	e_single_miles = singlemileRead[0];
    else if (singlemileRead[1] == singlemileRead[2]) 
	e_single_miles = singlemileRead[1];
    else if (singlemileRead[2] == singlemileRead[0]) 
	e_single_miles = singlemileRead[2];
    else 
	e_single_miles = 0;

   if (singlemileRead[3] == singlemileRead[4]) 
	e_single_milesB = singlemileRead[3];
    else if (singlemileRead[4] == singlemileRead[5]) 
	e_single_milesB = singlemileRead[4];
    else if (singlemileRead[1] == singlemileRead[5]) 
	e_single_miles = singlemileRead[5];
    else 
	e_single_milesB = 0;
	
}

void EEROM_WriteTotalMiles(void)
{


}

void EEROM_ReadTotalMiles(void) 
{
    uint8_t cnt;
    unsigned int temp[3] = {0, 0, 0};
    unsigned  int e_total_miles_temp = 201000;


   for(cnt = 0;cnt<50;cnt++)
    {

	    temp[0] = EEROM_ReadWord((EE_TOTAL_MILES_ADDR_1 + 0x100 *cnt));
	    temp[1] = EEROM_ReadWord((EE_TOTAL_MILES_ADDR_2 + 0x100 *cnt));
	    temp[2] = EEROM_ReadWord((EE_TOTAL_MILES_ADDR_3 + 0x100 *cnt));
	    if(temp[0] == temp[1])
	  	 e_total_miles = temp[0];
    	   else if (temp[1] == temp[2])
		e_total_miles = temp[1];
          else if (temp[2] == temp[0])
		e_total_miles = temp[2];
          else 
		e_total_miles = TOTAL_MILES;	  	  
	   if((e_total_miles ==0)&&(e_total_miles_temp == 201000))//第一个单元里面为0
	   	{
	   	e_total_miles = 0;   	
		break;
	   	}
	   else if(e_total_miles ==0)
	   {
	   	e_total_miles = e_total_miles_temp ;   	
		break;

	   }
		e_total_miles_temp = e_total_miles;

    }

}

void EEROM_WriteHMPulse(void) 
{
    EEROM_WriteWord((EE_KVALUE_1), e_hm_pulse);
    EEROM_WriteWord((EE_KVALUE_2), e_hm_pulse);
    EEROM_WriteWord((EE_KVALUE_3), e_hm_pulse);
}

void EEROM_ReadHMPulse(void) 
{
    unsigned int dump[3] = {0, 0, 0};
    dump[0] = EEROM_ReadWord((EE_KVALUE_1));
    dump[1] = EEROM_ReadWord((EE_KVALUE_2));
    dump[2] = EEROM_ReadWord((EE_KVALUE_3));

    if (dump[0] == dump[1]) 
		e_hm_pulse = dump[0];
    else if (dump[1] == dump[2]) 
		e_hm_pulse = dump[1];
    else if (dump[2] == dump[1]) 
		e_hm_pulse = dump[2];
    else 
		e_hm_pulse = HM_PLUSE;
}

