#include "BSP.h"
#include "iodefine.h"
#include"SYSTEM.h"



#define H595_SI		PORT_ISO.P11.BIT.P11_1          
#define H595_CLK	PORT_ISO.P10.BIT.P10_10            
#define H595_CS		PORT_ISO.P10.BIT.P10_11           

DIDO_DATA  alarm_led[9];
extern uint8_t BUZZ_LED;
extern uint8_t BUZZ_Flag;

void LED_Out(void)
{
	static uint8_t led_old[8]={0,0,0,0,0,0,0,0};//add led control 20191128
	uint8_t i, j;
	uint8_t dump;
	uint8_t diffcnt = 0;
	
	for(diffcnt=0;diffcnt<8;diffcnt++)
	{
		if(led_old[diffcnt] != alarm_led[diffcnt+1].byte)
			break;
	}
	if(diffcnt == 8)
		return;
	
	//add led control 20191128
    H595_CS = 0; for(int tt = 0; tt < 100; tt++);
	for(j = 1; j < 8; j++)
	{
        dump = alarm_led[j].byte;
        for(i = 0; i < 8; i++)
        {
            H595_CLK = 0; for(int tt = 0; tt < 100; tt++);
            
            if((dump & 0x80) == 0x80)
                H595_SI = 0;
            else
                H595_SI = 1;
			
            H595_CLK = 1; for(int tt = 0; tt < 100; tt++);
            
            dump = (dump << 1);
			
        }
    }
	H595_CS = 1;
	
    if(LED_test)
    {
    	if((0x01 == LED_LOCK)&&(0x01 == LED_COFFEE))
    	{
			LED_MirrorHeat = 0x01;
    	}
    	else
    	{
			LED_MirrorHeat = 0x00;
    	}
    }
    
    for(diffcnt=0;diffcnt<8;diffcnt++)
	{
		led_old[diffcnt]=alarm_led[diffcnt+1].byte;
	}
	
}

void MotorCoollingTemperature_Display(uint8_t soc_value)
{
	static uint8_t back = 0;
	
	if(soc_value>10)
		soc_value=0;
	
	if(back<soc_value)
		back++;
	else if(back>soc_value)
		back --;
	else
		back = soc_value;
 	
	switch (back)
	{
		case 0:
			LED_SOC1 = 1;
			LED_SOC2 = 0;
			LED_SOC3 = 0;
			LED_SOC4 = 0;
			LED_SOC5 = 0;
			LED_SOC6 = 0;
			break;
		case 1:
			LED_SOC1 = 1;
			LED_SOC2 = 0;
			LED_SOC3 = 0;
			LED_SOC4 = 0;
			LED_SOC5 = 0;
			LED_SOC6 = 0;
			break;
		case 2:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 0;
			LED_SOC4 = 0;
			LED_SOC5 = 0;
			LED_SOC6 = 0;
			break;
		case 3:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 0;
			LED_SOC5 = 0;
			LED_SOC6 = 0;
		 break;
		case 4:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 1;
			LED_SOC5 = 0;
			LED_SOC6 = 0;
			break;
		case 5:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 1;
			LED_SOC5 = 1;
			LED_SOC6 = 0;
			break;
		case 6:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 1;
			LED_SOC5 = 1;
			LED_SOC6 = 1;
			break;
		case 7:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 1;
			LED_SOC5 = 1;
			LED_SOC6 = 1;
			break;
		case 8:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 1;
			LED_SOC5 = 1;
			LED_SOC6 = 1;
			break;
		case 9:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 1;
			LED_SOC5 = 1;
			LED_SOC6 = 1;
			break;
		case 10:
			LED_SOC1 = 1;
			LED_SOC2 = 1;
			LED_SOC3 = 1;
			LED_SOC4 = 1;
			LED_SOC5 = 1;
			LED_SOC6 = 1;
			break;
		default:
			break;
	}
	
}

void BatVol_Display(uint8_t temp_value)
{
	static uint8_t back = 0;
	
	if(temp_value > 10)
		temp_value = 0;
	
	if(back < temp_value)
		back++;
	else if(back > temp_value)
		back--;
	else
		back = temp_value;

	switch (back)
	{
		case 0:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 0;
			LED_Shuiwen3 = 0;
			LED_Shuiwen4 = 0; 
			LED_Shuiwen5 = 0;
			LED_Shuiwen6 = 0;
			break;
		case 1:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 0;
			LED_Shuiwen3 = 0;
			LED_Shuiwen4 = 0; 
			LED_Shuiwen5 = 0;
			LED_Shuiwen6 = 0;
			break;
		case 2:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 0;
			LED_Shuiwen4 = 0; 
			LED_Shuiwen5 = 0;
			LED_Shuiwen6 = 0;
			break;
		case 3:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 0; 
			LED_Shuiwen5 = 0;
			LED_Shuiwen6 = 0;
			break;
		case 4:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 1; 
			LED_Shuiwen5 = 0;
			LED_Shuiwen6 = 0;
			break;
		case 5:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 1; 
			LED_Shuiwen5 = 1;
			LED_Shuiwen6 = 0;
			break;
		case 6:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 1; 
			LED_Shuiwen5 = 1;
			LED_Shuiwen6 = 1;
			break;
		case 7:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 1; 
			LED_Shuiwen5 = 1;
			LED_Shuiwen6 = 1;
			break;
		case 8:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 1; 
			LED_Shuiwen5 = 1;
			LED_Shuiwen6 = 1;
			break;
		case 9:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 1; 
			LED_Shuiwen5 = 1;
			LED_Shuiwen6 = 1;
			break;
		case 10:
			LED_Shuiwen1 = 1;
			LED_Shuiwen2 = 1;
			LED_Shuiwen3 = 1;
			LED_Shuiwen4 = 1; 
			LED_Shuiwen5 = 1;
			LED_Shuiwen6 = 1;
			break;
		default:
			break;
	}
	
}

