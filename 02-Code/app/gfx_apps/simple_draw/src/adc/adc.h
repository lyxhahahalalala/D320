

#ifndef __ADC_H_
#define __ADC_H_

typedef enum {
	AD_SAMPLE_CH_0 = 0,
    AD_SAMPLE_CH_1,
    AD_SAMPLE_CH_2,
	AD_SAMPLE_CH_3,
	AD_SAMPLE_CH_4,
	AD_SAMPLE_CH_5,
	AD_SAMPLE_CH_6,
	AD_SAMPLE_CH_7,
	AD_SAMPLE_CH_8,
	AD_SAMPLE_CH_9,
	AD_SAMPLE_CH_ALL
	
} AD_SAMPLE_CH_e;

typedef enum{
/* 0*/AD_V_B3 = 0,    /*前桥回路气压传感器信号*/
/* 1*/AD_R_B4,        /*乘客侧压力开关*/
/* 2*/AD_R_B17,       /*水位传感器信号*/
/* 3*/AD_R_B18,       /*环境温度传感器信号*/
/* 4*/AD_V_B20,       /*后桥回路气压传感器信号*/
	
/* 5*/AD_R_B1,        /*预留-模拟信号*/
/* 6*/AD_R_B2,        /*预留-模拟信号*/
/* 7*/AD_R_B5,        /*预留-模拟信号*/
/* 8*/AD_R_B6,        /*预留-模拟信号*/
/* 9*/AD_V_B19,       /*预留-驻车回路气压传感器信号*/
/*10*/AD_V_B21,       /*预留-模拟信号*/
/*11*/AD_V_B22,       /*预留-模拟信号*/
/*12*/B27_12V_DET,    /*车速传感器电源*/
	
/*13*/BAT_V,          /*蓄电池电压*/
/*14*/B24_5V_DET,     /*前桥回路气压传感器电源*/
/*15*/B25_5V_DET,     /*前桥气压传感器电源（预留）*/
/*16*/B26_5V_DET,     /*预留-驻车气压传感器电源*/
	
	IN_AD_ALL
	
}AD_IN_ID_e;


void adce_init(void);
void ADC_Filter(void);
uint32_t get_input_analog_quantity(AD_IN_ID_e index);
void ADC_AirPress_Cleanup_Cache(void);


#endif



