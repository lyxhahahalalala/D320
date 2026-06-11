/*
============================================================================
 PROJECT: Can project
============================================================================
                           C O P Y R I G H T
============================================================================
 Copyright (c) 2017 by SHFY. All rights reserved.
 
============================================================================


============================================================================
 Enviroment:  Devices:     RH850/D1M1 / R7F701404
              Debugger:    GHS Multi
============================================================================
*/

/******************************************************************************
Macro definitions
******************************************************************************/
#include "iodefine.h"
#include "r_typedefs.h"
#include "string.h"
#include "adc.h"
#include "ptmr.h"
#include "SYSTEM.h"

/********** MACRO **********/

#define AD_S0  PORT_ISO.P16.BIT.P16_1
#define AD_S1  PORT_ISO.P16.BIT.P16_2
#define AD_S2  PORT_ISO.P16.BIT.P16_3
#define CH5_AD_Toggle(value)  (PORT_ISO.P16.UINT16 |= ((value << 1) & 0x000e))

#define AD_R_B1_CTRL	 PORT_ISO.P17.BIT.P17_4
#define AD_R_B5_CTRL	 PORT_ISO.P17.BIT.P17_5
#define AD_R_B18_CTRL 	 PORT_ISO.P17.BIT.P17_6

#define AD_SAMPLE_MAX_VALUE    4096U //12位AD采样，最大为0x0fff
#define AD_REF_VOL             5000U //参考电压 单位 mV
#define AD_FIFO_SIZE           15U

#define WAIT_TIME              10U



/********** LOCAL VARIABLE DECLEAR **********/
typedef struct {
    uint32_t Container[AD_FIFO_SIZE];
    uint32_t TotalValue;
    uint32_t ResultBuf;
	
}AD_FILLER_t;
static AD_FILLER_t  ADFiller[IN_AD_ALL];



/**
  Interrupt Service Routines - ISR
 */
void module_reset_release()
{
    protected_write(SYS.PROTCMDMRST.UINT32, SYS.PROTSMRST.UINT32, SYS.MRSTC.UINT32,(0x00003fff));
}

void PBG_disable(void)
{
    PBG.FSGD0BPROT0.UINT32 = 0x07FFFFFF;
    PBG.FSGD0BPROT1.UINT32 = 0x07FFFFFF;
}

static void ADC0_sg1_start(void)
{
    ADCE0.SGSTCR1.UINT32 = 0x01;   /* Scan Group 1 Start Trigger */
    // while((ADCE0.SGSTR.BIT.SGACT&0x200) != 0); /* 等待转换完成 */
    for(int i = 0; i < 2000; i++); //delay a while
	
}
static void ADC0_sg1_stop(void)
{
    ADCE0.SGSTCR1.UINT32 = 0x00;   /* Scan Group 1 Stop Trigger */
    for(int i = 0; i < 1000; i++); //delay a while
	
}

void adce_init(void)
{
	AD_R_B1_CTRL  = 0;  //AD_R_B1  接入10K上拉电阻
	AD_R_B5_CTRL  = 0;  //AD_R_B5  接入10K上拉电阻
	AD_R_B18_CTRL = 0;  //AD_R_B18 接入10K上拉电阻
	
	//choose AD_V_B19  预留-驻车回路气压传感器信号
	AD_S0 = 0;
	AD_S1 = 0;
	AD_S2 = 1;
	
	memset(&ADFiller, 0, sizeof(ADFiller));
	
	module_reset_release();
	PBG_disable();
	
	ADCE0.VCR00.UINT32 = 0x000000010;
	ADCE0.VCR01.UINT32 = 0x000000011;
	ADCE0.VCR02.UINT32 = 0x000000012;
	ADCE0.VCR03.UINT32 = 0x000000013;
	ADCE0.VCR04.UINT32 = 0x000000014;
	ADCE0.VCR05.UINT32 = 0x000000015;
	ADCE0.VCR06.UINT32 = 0x000000016;
	ADCE0.VCR07.UINT32 = 0x000000017;
	ADCE0.VCR08.UINT32 = 0x000000018;
	ADCE0.VCR09.UINT32 = 0x000000019;
	
	/* Bits[5..0] =0 -> Select AP0_0 as Physical Channel (Can set Self-diagnostic mode)
	* Bits[7,6]  =0 -> No Upper Limit/Lower Limit  check
	* Bit[8]     =0 -> disable A/D Conversion End Interrupt Enable 
	* Bit[9]     =0 -> A/D conversion of the hold value is performed during a self-diagnostic
	* Bit[14..12]=0 -> do not set the MPX value to be transferred to an external analog multiplexer.
	* Bit[15]    =0 -> do not enable MPX
	*/
	
	ADCE0.ADCR.UINT32 = 0x00000002;   /* Bits[1,0]=10b -> Asynchronous suspend
									   * Bit[4]   =0   -> CTYP: 12-bit mode
									   * Bit[5]   =0   -> CRAC: PWDDR and ADCAnDRj are set to right align
									   * Bit[7]   =0   -> The self-diagnostic voltage circuit is turned off
									   */
	
	ADCE0.SMPCR.UINT32 = 0x00000012;  /* Bits[7..0]=00010010b   Set sampling time to 24cycles/40MHz */
	
	ADCE0.SFTCR.UINT32 = 0x00000000;  /* Bit[2]=0 -> Overwrite Error Interrupt disabled
									   * Bit[3]=0 -> Upper Limit/Lower Limit Error Interrupt disabled
									   * Bit[4]=0 -> When the A/D conversion result is read, the A/D conversion result is not cleared by hardware.
									   */
	
	ADCE0.DGCTL0.UINT32=0x00000000;   /* No setting for Self-Diagnostic voltage level */
	ADCE0.DGCTL1.UINT32=0x00000000;   /* No channels selected for Self-Diagnostic */
	
	ADCE0.PDCTL1.UINT32=0x00000000;   /* Bits[15..0]=0 -> ANIn[00:15] Pull-down enable is off */
	ADCE0.PDCTL2.UINT32=0x00000000;   /* Bits[19..0]=0 -> ANI0[16:35], ANI1[16:23] Pull-down enable is off*/
	
	ADCE0.SGTSEL1.UINT32=0x00000000;  /* Bits[8..0]=0 ->External input HW trigger is disabled   */
	ADCE0.SGTSEL2.UINT32=0x00000000;  /* Bits[8..0]=0 ->External input HW trigger is disabled   */
	ADCE0.SGTSEL3.UINT32=0x00000000;  /* Bits[8..0]=0 ->External input HW trigger is disabled   */
	
	ADCE0.SGVCSP1.UINT32=0x00000000;  /* Bits[5..0]=0 -> select the virtual channel 0 from which the scan is to be started */
	ADCE0.SGVCEP1.UINT32=0x00000009;  /* Bits[5..0]=0 -> select the virtual channel 0 from which the scan is to be ended */  
	ADCE0.SGMCYCR1.UINT32=0x00000003; /* Bits[1,0]=3 -> specify the number  of times for scanning in multicycle scan mode */ /*多重循环扫描次数*/
	
	ADCE0.SGCR1.UINT32=0x00000008;    /* Bit[0]=0 -> SGx_TRG trigger is disabled
									   * Bits[3,2]=10b -> Channel repeat times is one
									   * Bit[4]=1 -> INT_SGx is output when the scan for SGx ends
									   * Bit[5]=0 -> Multicycle scan mode
									   */
	
	// INTC2.EIC66.UINT16 = 0x004f;      /* enable Scan group 1 interrupt (INTADCE0I1) with reference table jump  */
	
}

/** @brief   AD_To_Volt  ADC转化为电压值 单位 mV
  * @param   divider_Res:分压电阻 单位 Ω/kΩ (相当于上拉电阻)
  * @param   sample_Res: 采样电阻 单位 Ω/kΩ
  * @param   ad_sample_buf: 采样值
  * @retval  返回电压值单位mV;
  */
static uint16_t AD_To_Volt(float divider_Res, float sample_Res, uint32_t ad_sample_buf)
{
	double temp = 0;
	double volt = 0;
	if ((divider_Res == 0) || (sample_Res == 0))
	{
		volt = (double)AD_REF_VOL * ad_sample_buf / AD_SAMPLE_MAX_VALUE;
		
	}
	else
	{
		temp = (double)(divider_Res + sample_Res)/sample_Res;
		volt = (double)AD_REF_VOL * temp * ad_sample_buf / AD_SAMPLE_MAX_VALUE;
		
	}
	
	return (uint16_t)volt;
	
}
/** @brief   AD_To_Res1  AD值转化为电阻值 单位 Ω
  * @param   Vu:上拉电压值 单位 mv
  * @param   Ru:上拉电阻阻值 单位 Ω
  * @param   Rd: 并联分流电阻阻值 单位 Ω
  * @param   ad_sample_buf: 采样值
  * @retval  返回电阻值 单位 Ω
  */
static uint32_t AD_To_Res(uint32_t Vu, uint32_t Ru, uint32_t Rd, uint32_t ad_sample_buf)
{
	/*
	Vu : 经过二极管降压后的上拉电压
	Vo  : 采样电压
	Ru  : 上拉电压值
	Rd  : 下拉并联分流电阻阻值
	Rx  : 输入电阻
	R   : 采样电阻（Rx与Rd并联之后的电阻）
	
	
	若SER 接地 , 即 Rx = 0  
		Vo = 0 
	
	若 SER 悬空
			  Vu * Rd      AD_REF_VOL * adc_sample 
		Vo = —————————— = —————————————————————————
			 (Ru + Rd)      AD_SAMPLE_MAX_VALUE
		
					  Vu * Rd * AD_SAMPLE_MAX_VALUE   
		adc_sample = ————————————————————————————————   //ADC采样值
					     (Ru + Rd) * AD_REF_VOL 
		
								  Vu * Rd * AD_SAMPLE_MAX_VALUE 
		即，若采样值 adc_sample >= ——————————————————————————————— 时，SER悬空
						            Ru + Rd) * AD_REF_VOL 
	
	若 SER 接入电阻 Rx
		若Rd = 0时
				R = Rx
				
					 Vu * R    Vu * Rx      AD_REF_VOL * adc_sample 
				Vo = ——————— = ————————— = —————————————————————————
					 Ru + R    Ru + Rx      AD_SAMPLE_MAX_VALUE
				
				
						     AD_REF_VOL * adc_sample * Ru
				Rx = ————————————————————————————————————————————————————
					  Vu * AD_SAMPLE_MAX_VALUE - AD_REF_VOL * adc_sample
		
		若Rd ≠ 0时
					 Rx * Rd 
				R  = ————————
					 Rx + Rd
				
					 Vu * R         Vu * Rx * Rd                   Vu * Rd * Rx            AD_REF_VOL * adc_sample 
				Vo = ——————— = —————————————————————————— =  —————————————————————————— = —————————————————————————
					 Ru + R     Ru * (Rx + Rd) + Rx * Rd      Ru * Rd + (Ru + Rd) * Rx      AD_SAMPLE_MAX_VALUE
				
				
									AD_REF_VOL * adc_sample * Ru * Rd 
				Rx = ——————————————————————————————————————————————————————————————————————
					  Vu * AD_SAMPLE_MAX_VALUE * Rd - AD_REF_VOL * adc_sample * (Ru + Rd)
	*/
	double temp1;
	double temp2;
	double Rx;
	double Vo;
	if (Ru == 0)
	{
		Rx = 0;
	}
	else
	{
		if(Rd == 0)
		{
			// Rx = (double)Ru * ad_sample_buf * AD_REF_VOL / (double)(AD_SAMPLE_MAX_VALUE *  Vu - ad_sample_buf * AD_REF_VOL);
			Vo = (double)AD_REF_VOL * ad_sample_buf / AD_SAMPLE_MAX_VALUE;
			if(Vu > Vo)
			{
				Rx = Vo * Ru / (Vu - Vo);
			}
			else
			{
				Rx = 0xffffffff; //无穷大 20251031
			}
		}
		else
		{
			// Rx = Ru * Rd * ad_sample_buf * AD_REF_VOL / (AD_SAMPLE_MAX_VALUE *  Vu * Rd - ad_sample_buf * AD_REF_VOL * (Ru + Rd ));
			temp1 = (double)ad_sample_buf * AD_REF_VOL * Ru * Rd;
			temp2 = (double)AD_SAMPLE_MAX_VALUE *  Vu * Rd - (double)ad_sample_buf * AD_REF_VOL * (Ru + Rd );
			if(temp2 <= 0)
			{
				Rx = Rd; //无穷大 20251031
			}
			else
			{
				Rx = temp1 / temp2;
			}
		}
	}
	return (uint32_t)Rx;
}

/*
P10_0  AD_V_B3      前桥回路气压传感器信号  0.3V~5V
P10_1  AD_R_B4      乘客侧压力开关，受到压力后，阻值0Ω~150Ω
P10_2  AD_R_B17     水位传感器信号  >140KΩ报警
P10_3  AD_R_B18     环境温度传感器信号  83Ω~96KΩ
P10_4  AD_V_B20     后桥回路气压传感器信号  0.3V~5V

P10_5  通过 74HC4051D 切换输入
AD_S2=0, AD_S1=0, AD_S0=0,  AD_R_B1      预留-模拟信号  20Ω~37kΩ
AD_S2=0, AD_S1=0, AD_S0=1,  AD_R_B2      预留-模拟信号  0~180Ω
AD_S2=0, AD_S1=1, AD_S0=0,  AD_R_B5      预留-模拟信号  0~4.6MΩ
AD_S2=0, AD_S1=1, AD_S0=1,  AD_R_B6      预留-模拟信号  电阻
AD_S2=1, AD_S1=0, AD_S0=0,  AD_V_B19     预留-驻车回路气压传感器信号  0.3V~5V
AD_S2=1, AD_S1=0, AD_S0=1,  AD_V_B21     预留-模拟信号  电压
AD_S2=1, AD_S1=1, AD_S0=0,  AD_V_B22     预留-模拟信号  电压
AD_S2=1, AD_S1=1, AD_S0=1,  B27_12V_DET  车速传感器电源  12V±0.3V

P10_6  BAT_V        蓄电池电压
P10_7  B24_5V_DET   前桥回路气压传感器电源  5V±0.1V/8.0mA
P10_8  B25_5V_DET   前桥气压传感器电源（预留）  5V±0.1V/8.0mA
P10_9  B26_5V_DET   预留-驻车气压传感器电源  5V±0.1V/8.0mA 
*/
static uint16_t ADC_ReadChannelValue(uint8_t index)	/*ADCE0 Scan group 1 read () */
{
    uint32_t iRet = 0;
    
	ADC0_sg1_start();
	switch(index)
    {
		case AD_SAMPLE_CH_0:
		    iRet = (uint16_t)(ADCE0.DIR00.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_1:
		    iRet = (uint16_t)(ADCE0.DIR01.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_2:
		    iRet = (uint16_t)(ADCE0.DIR02.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_3:
		    iRet = (uint16_t)(ADCE0.DIR03.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_4:
		    iRet = (uint16_t)(ADCE0.DIR04.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_5:
		    iRet = (uint16_t)(ADCE0.DIR05.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_6:
		    iRet = (uint16_t)(ADCE0.DIR06.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_7:
		    iRet = (uint16_t)(ADCE0.DIR07.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_8:
		    iRet = (uint16_t)(ADCE0.DIR08.UINT32 & 0x0fff);
		    break;
		case AD_SAMPLE_CH_9:
		    iRet = (uint16_t)(ADCE0.DIR09.UINT32 & 0x0fff);
		    break;
		default:
			iRet = 0;
		    break;
    }
    ADC0_sg1_stop();
	return iRet;
	
}

static uint32_t ADC_SampleConversionValue(uint8_t index)	/*ADCE0 Scan group 1 read () */
{
	const uint32_t Vin  = 4750;  // 4600 mV  ,经过二极管降压后的电压
    uint32_t sample_value = 0;
    uint32_t conversion_value = 0;
	uint64_t temp = 0;
	uint64_t tmptdata = 0;
	
	switch(index)
    {
		case AD_V_B3:  /* 前桥回路气压传感器信号 0.3V~5V */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_0);
			conversion_value = AD_To_Volt(0, 0, sample_value) + 0; //单位 mv  补偿 0 mv
		    break;
		case AD_R_B4:  /* 乘客侧压力开关，受到压力后，阻值0Ω~150Ω */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_1);
			// if(sample_value >= 3920) //悬空值  针对C7一下板子这个值是3768，C7以上的是3915
			// {
				// sample_value = 0;
			// }
			conversion_value = AD_To_Res(Vin, 100, 0, sample_value); //单位 Ω
		    break;
		case AD_R_B17:  /* 水位传感器信号  >140KΩ报警 */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_2);
			if(sample_value >= 3892) //悬空值 = Vin * AD_SAMPLE_MAX_VALUE / AD_REF_VOL
				sample_value = 0;
			conversion_value = AD_To_Res(Vin, 82000, 0, sample_value); //单位 Ω
		    break;
		case AD_R_B18:  /* 环境温度传感器信号  83Ω~96KΩ */
			#if 1
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_3);
			if(sample_value < 3690) //MCU引脚阻抗设置为220k/1200k
				temp = (uint64_t) (12000000.0 * sample_value/ (1200 * AD_SAMPLE_MAX_VALUE - 1210 * sample_value));
			else
				temp = 0;
			if((temp < 470000) && (temp > 0))
			{
				tmptdata = temp * 470000 / ( 470000 - temp) ;
			}
			
			if(tmptdata < 340)
				conversion_value = (uint32_t)(tmptdata * 1.12);
			else if(tmptdata < 440)
				conversion_value = (uint32_t)(tmptdata * 1.082);
			else if(tmptdata < 800)
				conversion_value = (uint32_t)(tmptdata * 1.02);
			else
				conversion_value = (uint32_t)(tmptdata * 1.03);
			#else
			AD_R_B18_CTRL = 0;  //AD_R_B18 接入10K上拉电阻
			system_wait_us(WAIT_TIME);
			sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_3);
			if(sample_value >= 3690)
				sample_value = 0;
			conversion_value = AD_To_Res(Vin, 10000, 470000, sample_value); //单位 Ω
			if(conversion_value < 2500)
			{
				AD_R_B18_CTRL = 1;  //AD_R_B18 接入1K上拉电阻
				system_wait_us(WAIT_TIME);
				sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_3);
				if(sample_value >= 3760) //悬空值
					sample_value = 0;
				conversion_value = AD_To_Res(Vin, 1000, 470000, sample_value); //单位 Ω
			}
			#endif
		    break;
		case AD_V_B20:  /* 后桥回路气压传感器信号 0.3V~5V */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_4);
			conversion_value = AD_To_Volt(0, 0, sample_value) + 0; //单位 mv  补偿 0 mv
		    break;
		
		
		case AD_R_B1:  /* 预留-模拟信号  20Ω~37kΩ */
			// AD_R_B1_CTRL  = 0;  //AD_R_B1  接入10K上拉电阻
			AD_S0 = 0;
			AD_S1 = 0;
			AD_S2 = 0;
			system_wait_us(WAIT_TIME);
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			if(sample_value >= 3690)
				sample_value = 0;
			conversion_value = AD_To_Res(Vin, 10000, 470000, sample_value); //单位 Ω
			/* if(conversion_value < 2500)
			{
				AD_R_B1_CTRL = 1;  //AD_R_B1 接入1K上拉电阻
				system_wait_us(WAIT_TIME);
				sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_3);
				if(sample_value >= 3760) //悬空值
					sample_value = 0;
				conversion_value = AD_To_Res(Vin, 1000, 470000, sample_value); //单位 Ω
			} */
		    break;
		case AD_R_B2:  /* 预留-模拟信号  0~180Ω */
			AD_S0 = 1;
			AD_S1 = 0;
			AD_S2 = 0;
			system_wait_us(WAIT_TIME);
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			if(sample_value >= 3768) //悬空值
				sample_value = 0;
			conversion_value = AD_To_Res(Vin, 100, 470000, sample_value); //单位 Ω
		    break;
		case AD_R_B5:  /* 预留-模拟信号  0~4.6MΩ */
			// AD_R_B5_CTRL  = 0;  //AD_R_B5  接入10K上拉电阻
			AD_S0 = 0;
			AD_S1 = 1;
			AD_S2 = 0;
			system_wait_us(WAIT_TIME);
			sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			if(sample_value >= 3690)
				sample_value = 0;
			conversion_value = AD_To_Res(Vin, 10000, 470000, sample_value); //单位 Ω
			/* if(conversion_value < 2500)
			{
				AD_R_B5_CTRL = 1;  //AD_R_B5 接入1K上拉电阻
				system_wait_us(WAIT_TIME);
				sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_3);
				if(sample_value >= 3760) //悬空值
					sample_value = 0;
				conversion_value = AD_To_Res(Vin, 1000, 470000, sample_value); //单位 Ω
			} */
		    break;
		case AD_R_B6:  /* 预留-模拟信号  电阻 */
			AD_S0 = 1;
			AD_S1 = 1;
			AD_S2 = 0;
			system_wait_us(WAIT_TIME);
			sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			if(sample_value >= 3768) //悬空值
				sample_value = 0;
			conversion_value = AD_To_Res(Vin, 100, 470000, sample_value); //单位 Ω
		    break;
		case AD_V_B19:  /* 预留-驻车回路气压传感器信号 0.3V~5V */
			AD_S0 = 0;
			AD_S1 = 0;
			AD_S2 = 1;
			system_wait_us(WAIT_TIME);
			sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			conversion_value = AD_To_Volt(0, 0, sample_value) + 0; //单位 mv  补偿 0 mv
		    break;
		case AD_V_B21:  /* 预留-模拟信号  电压 */
			AD_S0 = 1;
			AD_S1 = 0;
			AD_S2 = 1;
			system_wait_us(WAIT_TIME);
			sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			conversion_value = AD_To_Volt(0, 0, sample_value) + 0; //单位 mv  补偿 0 mv
		    break;
		case AD_V_B22:  /* 预留-模拟信号  电压  */
			AD_S0 = 0;
			AD_S1 = 1;
			AD_S2 = 1;
			system_wait_us(WAIT_TIME);
			sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			conversion_value = AD_To_Volt(0, 0, sample_value) + 0; //单位 mv  补偿 0 mv
		    break;
		case B27_12V_DET:  /* 车速传感器电源  12V±0.3V */
			AD_S0 = 1;
			AD_S1 = 1;
			AD_S2 = 1;
			system_wait_us(WAIT_TIME);
			sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_5);
			conversion_value = AD_To_Volt(56, 10, sample_value) + 0; //单位 mv  补偿 0 mv
		    break;
		
		
		case BAT_V:  /* 蓄电池电压 */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_6);
			conversion_value = AD_To_Volt(51, 5.1, sample_value) + 150; //单位 mv  补偿 0 mv
		    break;
		case B24_5V_DET:  /* 前桥回路气压传感器电源  5V±0.1V/8.0mA */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_7);
			conversion_value = AD_To_Volt(56, 0, sample_value); //单位 mv  补偿 0 mv
		    break;
		case B25_5V_DET:  /* 前桥气压传感器电源（预留）  5V±0.1V/8.0mA */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_8);
			conversion_value = AD_To_Volt(56, 0, sample_value); //单位 mv  补偿 0 mv
		    break;
		case B26_5V_DET:  /* 预留-驻车气压传感器电源  5V±0.1V/8.0mA  */
		    sample_value = (uint32_t)ADC_ReadChannelValue(AD_SAMPLE_CH_9);
			conversion_value = AD_To_Volt(56, 0, sample_value); //单位 mv  补偿 0 mv
			break;
		default:
			sample_value = 0;
			conversion_value = 0;
		    break;
    }
	
    return conversion_value;
	
}

void ADC_Filter(void)
{
	static uint16_t ADC_Counter = 0;
	
	for(uint16_t index = 0; index < IN_AD_ALL; index++)
	{
		switch(index)
		{
			case AD_V_B3:  /* 前桥回路气压传感器信号 0.3V~5V */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_R_B4:  /* 乘客侧压力开关，受到压力后，阻值0Ω~150Ω */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_R_B17:  /* 水位传感器信号  >140KΩ报警 */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_R_B18:  /* 环境温度传感器信号  83Ω~96KΩ */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_V_B20:  /* 后桥回路气压传感器信号 0.3V~5V */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			
			case AD_R_B1:  /* 预留-模拟信号  20Ω~37kΩ */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = (0); //ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_R_B2:  /* 预留-模拟信号  0~180Ω */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = (0); //ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_R_B5:  /* 预留-模拟信号  0~4.6MΩ */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = (0); //ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_R_B6:  /* 预留-模拟信号  电阻 */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = (0); //ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_V_B19:  /* 预留-驻车回路气压传感器信号 0.3V~5V */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_V_B21:  /* 预留-模拟信号  电压 */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = (0); //ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case AD_V_B22:  /* 预留-模拟信号  电压  */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = (0); //ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case B27_12V_DET:  /* 车速传感器电源  12V±0.3V */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			
			case BAT_V:  /* 蓄电池电压 */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case B24_5V_DET:  /* 前桥回路气压传感器电源  5V±0.1V/8.0mA */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case B25_5V_DET:  /* 前桥气压传感器电源（预留）  5V±0.1V/8.0mA */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			case B26_5V_DET:  /* 预留-驻车气压传感器电源  5V±0.1V/8.0mA  */
				if(ADFiller[index].TotalValue > ADFiller[index].Container[ADC_Counter])
					ADFiller[index].TotalValue -= ADFiller[index].Container[ADC_Counter];
				else
					ADFiller[index].TotalValue = 0;
				ADFiller[index].Container[ADC_Counter] = ADC_SampleConversionValue(index);
				ADFiller[index].TotalValue += ADFiller[index].Container[ADC_Counter];
				ADFiller[index].ResultBuf   = ADFiller[index].TotalValue / AD_FIFO_SIZE;
				break;
			default:
				
				break;
		}
	}
	
    ADC_Counter++;
	if(ADC_Counter == AD_FIFO_SIZE)
		ADC_Counter = 0;
	
}

uint32_t get_input_analog_quantity(AD_IN_ID_e index)
{
	if(index < IN_AD_ALL)
		return ADFiller[index].ResultBuf;
	else
		return 0;
	
}

void ADC_AirPress_Cleanup_Cache(void)
{
	memset(&ADFiller[AD_V_B3], 0, sizeof(AD_FILLER_t));
	memset(&ADFiller[AD_V_B20], 0, sizeof(AD_FILLER_t));
	memset(&ADFiller[AD_V_B19], 0, sizeof(AD_FILLER_t));
	memset(&ADFiller[B24_5V_DET], 0, sizeof(AD_FILLER_t));
	memset(&ADFiller[B25_5V_DET], 0, sizeof(AD_FILLER_t));
	memset(&ADFiller[B26_5V_DET], 0, sizeof(AD_FILLER_t));
}




