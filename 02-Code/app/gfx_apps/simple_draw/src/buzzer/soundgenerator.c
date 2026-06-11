/* ***********************************************************************************************************
 * Filename:	soundgenerator.c
 * File	ext:	c
 * Author:		Liubing
 * Data:		2019/9/4
 * purpose:	sound Driver
 * **********************************************************************************************************/

#include "iodefine.h"
#include "r_typedefs.h"
#include "sg.h"

#define SGCLKDIV    SG3.CTL.BIT.CLKDIV


void SoundGenInit(void)
{
	//PWM output
    PORT_ISO.PMC3.UINT16    |=  0x0060; //  3rd Alternative function
    PORT_ISO.PFCE3.UINT16   |=  0x0060;
    PORT_ISO.PM3.UINT16     &= ~0x0060;   
    PORT_ISO.PFC3.UINT16    &= ~0x0060; //P3-5 3-6
	
	SG3.CTL.BIT.EN  = 0;
	SG3.CTL.BIT.CLKDIV = 4;
	SG3.CONF.UINT32 = 0x04;
	
	SG3.FL.UINT32   = 0;
	SG3.FH.UINT32   = 0;
	SG3.PWM.UINT32  = 0;
	
}

void SoundStop(void)
{
	SG3.CTL.UINT32 &= ~0x01;//stop operation
	PORT_ISO.P3.BIT.P3_5 = 0;   
}

void SoundStart(void)
{
	SG3.CTL.UINT32 |= 0x01; //start operation
	
}

void SoundGenNewTone(unsigned int tonefrequence)
{
	SG3.CTL.UINT32 &= ~0x01;//stop operation
	while(SG3.CTL.UINT32&0x01)
	{
		;
	}
	SG3.CTL.UINT32 = 0x08;//fsgnclk = fpclk/5 = 80/5 = 16M     SgnEN = 1  sg clk 
	tonefrequence = tonefrequence*3/2;
	

	SG3.FH.UINT32 = 31250/tonefrequence -1;
	SG3.FL.UINT32 = 400;//ftone = 947hz
	SG3.PWM.UINT32 = 50;//

	//fPWM = fSGnCLK/(SGnFL + 1)
	//If [SGnPWM buffer] > [SGnFL buffer]:Duty cycle = 100 %
	//If 0 =<[SGnPWM buffer] <=[SGnFL buffer]:
	//Duty cycle = [SGnPWM buffer] / ([SGnFL buffer] + 1)
	
	SG3.CONF.UINT32 = 0x4;//0: Low level output at SGTFAOL 0: Buffer update by 
	//interrupt INTSGnTI disabled
	//Buffer update by write to volume register SGnPWM enabled
	//0: Tone frequency signal SGnOF output at SGTFAO
	//00B: Continuous sound output selected
	SG3.CTL.UINT32 |= 0x01;//start operation

}

void SG_NewTone(uint32_t tonefreqHZ, uint8_t duty)
{
	uint32_t sgFL_Buferr;
	uint32_t sgFH_Buferr;
	uint32_t sgPWM_Buferr;
	const uint32_t pwmBaseKHZ = 32;  //kHz
	
	/*********************************************
	预分量器输出时钟的频率计算为
	fSGnCLK = fPCLK / (SGnCLKDIV+1)
	
	PWM基本频率由频率比较缓冲区SGnFL确定。可以计算为：
	         fSGnCLK
	fPWM = ------------
            SGnFL + 1
	
	音调频率可以计算为：
	                 fSGnCLK                 fPWM
	fTONE = ------------------------- = ---------------
            2*(SGnFL + 1)*(SGnFH + 1)    2*(SGnFH + 1)
	
	
	fSGnCLK：预分量器输出时钟的频率SGnCLK
	SGnFL：SGnFL缓冲区的内容
	
	PWM音量信号的占空比计算如下：
	If [SGnPWM buffer] > [SGnFL buffer]:
		Duty cycle = 100 %
	If 0 =<[SGnPWM buffer] <=[SGnFL buffer]:
		Duty cycle = [SGnPWM buffer] / ([SGnFL buffer] + 1)
	[SGnPWM buffer]: SGnPWM缓冲区的内容
	[SGnFL buffer]:  SGnFL缓冲区的内容
	
	*********************************************/
	
	SoundStop();
	
	do{
		if(SG3.CTL.UINT32&0x01)
		{
			break; /** 等待 SG3.CTL.BIT.EN = 0 **/
		}
		
		// 音频频率 tonefreqHZ 固定，
		// PWM基本频率 pwmBaseKHZ = 0 ~ 31kHz 声音会有不同变化，pwmBaseKHZ > 31kHz以后，声音没明显变化
		
		// PWM基本频率 pwmBaseKHZ 固定，
		// 音频频率 tonefreqHZ 越大，声音越尖细
		
		sgFL_Buferr  = 80 * 1000 / (SGCLKDIV + 1) /pwmBaseKHZ - 1;
		sgFH_Buferr  = pwmBaseKHZ * 1000 / (tonefreqHZ * 2) - 1;
		sgPWM_Buferr = duty * (sgFL_Buferr + 1) / 100;
		
		SG3.FL.UINT32  = sgFL_Buferr; 
		SG3.FH.UINT32  = sgFH_Buferr;;
		SG3.PWM.UINT32 = sgPWM_Buferr;
		
	}while(0);
	
	SoundStart();
	
}

void speaker_test(void)
{
	static uint32_t beepCount = 0;
	static uint32_t beepStatus = 0;
	static uint32_t tonefreqHZ = 800;
	// static uint32_t pwmBaseKHZ = 32;
	static uint8_t  duty = 5;
	
	if(beepCount < 50)
	{
		if(beepStatus == 0)
			SG_NewTone(tonefreqHZ, duty + 5);
		beepStatus = 1;
		
		if (beepCount > 6)
			SG_NewTone(tonefreqHZ, 0);
		
		
	}
	else if(beepCount < 100)
	{
		if(beepStatus == 1)
			SG_NewTone(tonefreqHZ - 400, duty);
		beepStatus = 0;
		
		if(beepCount > 56)
			SG_NewTone(tonefreqHZ - 400, 0);
		
	}
	else
	{
		beepStatus = 0;
		beepCount = 0;
		SoundStop();
		
	}
	beepCount++;
	
	static uint32_t sgfh_cnt = 0;
	if(sgfh_cnt >= 200)
	{
		sgfh_cnt = 0;
		SG_NewTone(tonefreqHZ, 0);
		SoundStop();
		tonefreqHZ += 10;
		
	}
	else sgfh_cnt++;
	
	
	
	
}


