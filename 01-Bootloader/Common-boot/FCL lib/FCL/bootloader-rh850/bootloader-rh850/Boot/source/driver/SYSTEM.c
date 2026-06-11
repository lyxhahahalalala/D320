#include "SYSTEM.h"
#include "string.h"
//#include "r_bsp_stdio_api.h"
unsigned char delay_cnt;
DATA_BIT gKEY[5];
DATA_BIT gCTL[8]; //仪表输出控制信息

unsigned int single_miles = 0;
unsigned long total_miles = 0;

unsigned char adcounter,adcounter2;
unsigned int advalue;
unsigned int adtmpvalue[60];

unsigned int Fcan_count = 0; //前部模块CAN
unsigned int Rcan_count = 0; //顶部模块CAN
unsigned int Mcan_count = 0; //中部模块CAN
unsigned int Ecan_count = 0; //发动机ECU模块CAN
unsigned int Vcan_count = 0; //整车控制器VCU模块CAN
unsigned int Bcan_count = 0; //电池管理器BMS模块CAN
unsigned int Dcan_count = 0; //转向助力DC/AC模块CAN
unsigned int Acan_count = 0; //空调AC模块CAN

DBCM_KEY fKEY; //前模块功率开关状态
DBCM_KEY mKEY; //中（顶）模块功率开关状态
DBCM_KEY rKEY; //后模块功率开关状态

unsigned int fFreq; //前模块频率
unsigned int mFreq; //中（顶）模块频率
unsigned int rFreq; //后模块频率

unsigned int fSpeed; //前模块折算车速
unsigned int mSpeed; //中（顶）模块折算车速
unsigned int rSpeed; //后模块折算车速
float  pSpeed; //外部CAN这算车速

unsigned int fADR[2]; //前模块电阻值
unsigned int mADR[2]; //中（顶）模块电阻值
unsigned int rADR[2]; //后模块电阻值

unsigned int ADV[9]; //仪表模拟信号采集值
unsigned int ADR[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //仪表模拟信号采集值
unsigned int __ADR[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int F1200ms = 0;
unsigned int adCount = 0;
unsigned int bOk = 0;

unsigned char PRESS[2]; //气压值

DBCM_POUT fPOUT; //前模块功率输出状态
DBCM_POUT mPOUT; //中（顶）模块功率输出状态
DBCM_POUT rPOUT; //后模块功率输出状态

unsigned char fPF[16]; //前模块功率输出故障
unsigned char mPF[16]; //中（顶）模块功率故障
unsigned char rPF[16]; //后模块功率输出故障

unsigned int fpcur[15];//前模块功率输出电流
unsigned int mpcur[15];//中模块功率输出电流
unsigned int rpcur[15];//后模块功率输出电流

unsigned int fSingle_miles;
unsigned int mSingle_miles;
unsigned int rSingle_miles;

unsigned char ccvs_eng_req; //启动请求

unsigned char dm_hcu_display[32]; //故障类型
unsigned char dm_tcu_display[32];

unsigned char poweron_flag;

extern unsigned int bl_pwm_cnt;
extern unsigned int uBL;
extern unsigned char poweron_flag;

//系统初始化函数
void System_Init(void) { 
	RS_CAN_init();
	gpio_init();
	//init_wdt();
	//start_timer();
    alarm_led[0].byte = 0xff;//消除开机瞬间LED和蜂鸣器闪烁
    alarm_led[1].byte = 0x00;
    alarm_led[2].byte = 0x00;
    alarm_led[3].byte = 0x00;
    LED_Out();
    
    PWR_CTRL = 0;
    
    //TIMER1_Init();
    delay_cnt = 10; //延时0.1s
    while (delay_cnt > 1) {
        //ClrWdt();
        //SW_Input(); //开关读取
    }
    
}


void BCAN_SendCtl(void) {
    CAN_MESSAGE message_POUT;
	#if 0
    message_POUT.id = 0x6a4;
    message_POUT.data[0] = gCTL[0].byte;
    message_POUT.data[1] = gCTL[1].byte;
    message_POUT.data[2] = gCTL[2].byte;
    message_POUT.data[3] = gCTL[3].byte;
    message_POUT.data[4] = gCTL[4].byte;
    message_POUT.data[5] = gCTL[5].byte;
    message_POUT.data[6] = ((M_ON && wake_up3) || wake_up2) + (IN14 << 1)+(IN15 << 2)+(IN16 << 3)+(IN21 << 4);//模块雨刮控制
    message_POUT.data[7] = 0;
    message_POUT.len = 8;
    message_POUT.type = 1;
	#endif
    CAN1_SendMessage(&message_POUT);
}

void BCAN_send_mile(void) {
    CAN_MESSAGE msg;
	#if 0
    msg.data[0] = (unsigned char) (e_total_miles); //低八位
    msg.data[1] = (unsigned char) (e_total_miles >> 8); //二级八位
    msg.data[2] = (unsigned char) (e_total_miles >> 16); //三级八位
    msg.data[3] = (unsigned char) (e_total_miles >> 24); //高八位
    msg.data[4] = 0; 
    msg.data[5] = 0;
    msg.data[6] = 0;
    msg.data[7] = 0;
    msg.id = 0x454;
    msg.len = 0x08;
    msg.type = 0x01;
	#endif
    CAN1_SendMessage(&msg);

}

void PCAN_CCVS(void) {
    CAN_MESSAGE message_CCVS;
	#if 0
    message_CCVS.id = 0x0C19A7A1;
    message_CCVS.data[0] = (unsigned char) (M_ON << 6)//ON档
            + (unsigned char) (IN4 << 5)//ST档
            + (unsigned char) (IN27 << 4)//后门开信号
            + (unsigned char) (IN20 << 3)//前面开信号
            + (unsigned char) (rKL15 << 2)//后舱门开信号
            + (unsigned char) (rKL8 << 1)//驻车信号
            + (unsigned char) (rKL10 || rKL12) ;//气压低报警信号
    message_CCVS.data[1] = (unsigned char) (LED4 << 7)//左转向
            + (unsigned char) (LED17 << 6)//远光灯
            + (unsigned char) (LED19 << 5)//前雾灯
            + (unsigned char) (LED8 << 4)//右转向
            + (unsigned char) (IN33 << 3)//油量报警
            + (unsigned char) (0 << 2)//发动机预热
            + (unsigned char) (LED22 << 1)//近光灯
            + (unsigned char) (rLED_flag) ;//后雾灯
    message_CCVS.data[2] = 0xff;
    message_CCVS.data[3] = (unsigned char) (pSpeed * 2);
    message_CCVS.data[4] = PRESS[0]*100;
    message_CCVS.data[5] = (unsigned char) (PRESS[0]*100 >> 8);
    message_CCVS.data[6] = PRESS[1]*100;
    message_CCVS.data[7] = (unsigned char) (PRESS[1]*100 >> 8);
    message_CCVS.len = 8;
    message_CCVS.type = 0;
	#endif
    CAN2_SendMessage(&message_CCVS);
}

void PCAN_send_mile(void) {
    CAN_MESSAGE msg;
	#if 0
    msg.data[0] = (unsigned char) (e_total_miles); //低八位
    msg.data[1] = (unsigned char) (e_total_miles >> 8); //二级八位
    msg.data[2] = (unsigned char) (e_total_miles >> 16); //三级八位
    msg.data[3] = (unsigned char) (e_total_miles >> 24); //高八位
    msg.data[4] = 0; 
    msg.data[5] = 0;
    msg.data[6] = 0;
    msg.data[7] = 0;
    msg.id = 0x0C1AA7A1;
    msg.len = 0x08;
    msg.type = 0x00;
	#endif
    CAN2_SendMessage(&msg);

}

void PCAN_send_req(void) {
    CAN_MESSAGE msg;
	#if 0
    msg.data[0] = 0xff;
    msg.data[1] = (unsigned char) 0x0f
            + (unsigned char) ((ccvs_eng_req & 0x03) << 4)
            +(unsigned char) (0x03 << 6);
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFF;
    msg.id = 0x18FED917;
    msg.len = 0x08;
    msg.type = 0x00;
	#endif
    CAN2_SendMessage(&msg);

}


void Process_Can1(void)
{
    unsigned char i=0,j=0;
    CAN_MESSAGE message;
    memset(&message, 0, sizeof(CAN_MESSAGE));

    { //数据有效
        CAN1_ReceiveMessage(&message);

        switch (message.id) 
	{
            case 0x681:
                Fcan_count = 0;
                fKEY.BYTES[0] = message.data[0];
                fKEY.BYTES[1] = message.data[1];
                fKEY.BYTES[2] = message.data[2];
                fFreq = message.data[3]+((unsigned int) message.data[4] << 8);
                fSpeed = message.data[5]+((unsigned int) message.data[6] << 8);
                break;
           
                            
                    
                }

        
    }
    //C1INTF = 0; /* the individual flag register cleared */  __del
    //C1RX0CONbits.RXFUL = 0;                                 __del
    //_C1IF = 0;                                              __del
}

/*******************分包处理*****************/

DM1 CAN_18FECAEF;
unsigned char CAN_18FECAEF_len;
DM1 CAN_18FECA03;
unsigned char CAN_18FECA03_len;
DM1 CAN_18FECAF5;
unsigned char CAN_18FECAF5_len;
DM1 CAN_18FECA00;
unsigned char CAN_18FECA00_len;
CAN_CACHE m_can;
CAN_CACHE n_can;
CAN_CACHE c_can;
CAN_CACHE e_can;
/*******************分包处理*****************/

unsigned int moto_voltage; //电机电压
unsigned int moto_current=10000; //电机电流
unsigned int moto_speed; //电机转速
unsigned char Motor_Temperature=40; //电机温度
unsigned char Inverter_t=40; //逆变器温度

unsigned char Accelerator; //油门踏板
unsigned int Engine_Speed; //发动机转速

unsigned int Fule_rate; //油耗
unsigned char water_temp=40; //水温
unsigned char Engine_Fuhe; //发动机负荷
unsigned char Niaosu; //尿素;
unsigned char Accelerator_Shiji; //油门踏板实际
unsigned char Engine_oil_press; //机油压力
unsigned char ambient_air_temperature=40; //周围空气温度

unsigned char Current_Gear; //档位
unsigned char TCU_Position; //离合位置
unsigned char TCU_warn; //变速箱故障
unsigned char STOP; //停车故障
unsigned int Speed_rmp; //输出轴转速
unsigned char Car_mode; //整车模式
unsigned char TCU_code; //变速箱故障代码
unsigned char TCU_level; //变速箱故障等级

DATA_BIT CAN_KEY[4];
unsigned char Brake_percent; //制动踏板开度
unsigned char Speed_percent; //加速踏板开度
unsigned char Digit_input3; //数字量输入3

unsigned long Program; //程序版本
unsigned long tcu_spn; //TCU故障码
unsigned char tcu_fmi;
unsigned long tcu_cm;
unsigned char tcu_oc;
unsigned char Car_LIFE; //整车控制器

unsigned int BMS_V=10000; //电池电压
unsigned char BMS_SOC; //电池SOC
unsigned int BMS_A=10000; //电池电流
unsigned char Status_Flag1;
unsigned char Status_Flag2;
unsigned char Status_Flag3;

unsigned int BMS_A_charge=10000; //最大充电电流
unsigned int BMS_A_discharge=10000; //最大电流
unsigned char Warn_level; //故障等级
unsigned int BMS_V_average=10000; //电池平均电压
unsigned char BMS_T_H; //电池最高温度

unsigned int Oum_zheng; //绝缘正阻值
unsigned int Oum_fu; //绝缘负阻值
unsigned int Battery_single_H=10000; //电池单体高电压
unsigned int Battery_single_L=10000; //电池单体低电压

unsigned char Battery_number[10]; //电池编号
unsigned int Battery_voltage[10]; //电池单体电压

unsigned char Battery_number_t[10]; //电池温度编号
unsigned int Battery_temp[10]; //电池单体温度

unsigned int DCAC_W=10000;
unsigned int DCAC_V=10000;
unsigned int DCAC_U=10000;
unsigned char Sanreqi_t=40; //散热器温度
unsigned char DCAC_Warn_code; //故障代码

unsigned char AC_Warn_code; //AC故障代码
DATA_BIT AC_KEY[2];
unsigned char Indoor_t=60; //室内温度
unsigned char Outdoor_t=60; //室外温度
unsigned char Set_t=60; //设定
unsigned char AC_req; //制冷请求
unsigned char AC_LIFE; //AC生命



/**************************************************/
unsigned char battle_t; //电池均温
unsigned char battle_t_warn; //电池过温报警
unsigned char battle_jueyuan_warn; //绝缘过低报警
unsigned char battle_higtv1_warn; //高压电池1故障
unsigned char battle_higtv2_warn; //高压电池2故障
unsigned char moto_t_warn; //电机过温
unsigned char moto_ready; //ready信号
unsigned char xudc; //蓄电池异常
unsigned char moto_warn; //电机故障

unsigned char BMS_offline_warn; //电池不在线
unsigned char MCU_offline_warn; //电机不在线
unsigned char TCU_offline_warn; //变速箱不在线
unsigned char Engine_offline_warn; //发动机不在线
unsigned char Engine_wait; //发动机怠速
unsigned char EV; //模式
unsigned char Signal_charge; //充电信号

unsigned char mil_lamp_request; //黄色三级报警
unsigned char fail_mode; //文字报警
unsigned char mil_buzz_request; //蜂鸣器响
unsigned long hcu_spn; //HCU故障码
unsigned char hcu_fmi;

unsigned char Eng_static_test; //发动机稳态转速测试
unsigned char tcu_start_allow; //发动机运行启动

unsigned char Lamp_status_YELLOW; //变速箱黄灯状态
unsigned char Lamp_status_RED; //变速箱红灯状态

unsigned long ccu_spn; //CCU故障码
unsigned char ccu_fmi;

unsigned long ecu_spn; //ECU故障码
unsigned char ecu_fmi;

unsigned int ENG_start_req; //仪表控制启动
unsigned long mot_work_time; //电机工作累积时间
unsigned char mot_work_percent; //电机工作时间比
unsigned char DCDC_ctrl; //DCDC状态
unsigned char hcu_starter_forbid; //起动机工作
unsigned char aircompress_enable; //压缩机
unsigned char Speed_lim_mode; //限速模式

unsigned char water_level; //水位

unsigned char Wait_start; //等待启动
unsigned char Maintain_warn; //维护报警
unsigned char Stop_warn; //发动机停机报警
unsigned char Engine_warn; //发动机故障

void Process_Can2(void)
{
    unsigned char i=0; 
    CAN_MESSAGE msg;
    memset(&msg, 0, sizeof(CAN_MESSAGE));

    { //数据有效
        CAN2_ReceiveMessage(&msg);
		#if 0
        switch (msg.id) {
            case 0x0C03A1A7://仪表报文MET_C_1
                Vcan_count=0;
                moto_voltage = (unsigned char) msg.data[0]+((unsigned int) msg.data[1] << 8);
                moto_current = (unsigned char) msg.data[2]+((unsigned int) msg.data[3] << 8);
                moto_speed = (unsigned char) msg.data[4]+((unsigned int) msg.data[5] << 8);
                Motor_Temperature = msg.data[6];
                Inverter_t = msg.data[7];
                break;

            case 0x0C04A1A7://仪表报文MET_C_2
                Vcan_count=0;
                Accelerator = (unsigned char) msg.data[4];
                Engine_Speed = ((unsigned char) msg.data[5]+((unsigned int) msg.data[6] << 8))*0.125;
                break;

            case 0x0C05A1A7://仪表报文MET_C_3
                Vcan_count=0;
                Fule_rate = (unsigned char) msg.data[0]+((unsigned int) msg.data[1] << 8);
                water_temp = msg.data[2];
                Engine_Fuhe = msg.data[3];
                Niaosu = msg.data[4];
                Accelerator_Shiji = msg.data[5]; //油门踏板实际
                Engine_oil_press = msg.data[6]; //机油压力
                ambient_air_temperature = msg.data[7]; //周围空气温度
                break;

        }
		#endif
    }
    //C2INTF = 0; /* the individual flag register cleared */   __del
    //C2RX0CONbits.RXFUL = 0;                                  __del
    //IFS2bits.C2IF = 0;                                       __del
}

void Process_Can0(void)
{
    CAN_MESSAGE message;
    memset(&message, 0, sizeof(CAN_MESSAGE));
    CAN0_ReceiveMessage(&message);
}

void Interrupt10ms(void)                  //10ms中断一次
{
#if 0
    static unsigned int cnt0 = 0;
    static unsigned int cnt1 = 3;
    static unsigned int cnt2 = 7;
    static unsigned int cnt3 = 9;
    static unsigned int cnt4 = 0;
    static unsigned int cnt5 = 0;
    static unsigned int cnt1s = 0;
    static unsigned int lpcnt2 = 0, lpcnt1 = 0, lpcount = 0;
    static unsigned int Fcount = 0, Kcount = 0;
    static unsigned int count6 = 0;
    static float mile = 0;
    
    __Count_ADR();
    //报警延时3s标记
    if (alarm_update_flag == 1) {
        if (count6 >= 300) {//
            count6 = 0;


		
            alarm_update_flag = 0;
        } else {
            count6++;
        }
    } else {
        count6 = 0;
    }
cnt1s++;
if(cnt1s>=50)
{
		cnt1s=0;
		if((wake_up1)&&(!M_ON_flag)&&(poweron_flag==0))
			{
          		 // LED_Logic();
           		//  LED_Out();
           		//  SYSTME_Logic();
			 }

}
    if (delay_cnt > 0) delay_cnt--; //延时计数器 

    if (cnt0 >= 10) {//50ms
        cnt0 = 0;              
        BCAN_send_mile();
        BCAN_SendCtl();
        if (M_ON_flag && (wake_up3 || wake_up2)) {//wake_up3置0时，M_ON不会立马置0，因为存在消抖延时
            LED_Logic();
            LED_Out();
            SYSTME_Logic();
        }

        F50ms = 1;
    } else cnt0++;

    if (cnt1 >= 9) {//100ms
        cnt1 = 0;
        F100ms= 1;    
       PCAN_CCVS();
        //计算累计里程
        if (pSpeed > 0) {//车速大于0时才累计
            mile = mile + (float) (pSpeed / 3.6 * 0.1);
            if (mile >= 100.0) {
                mile = 0.0;
                save_flag = 1;
                if (single_miles >= 9999) single_miles = 0;
                else single_miles++;
                if (total_miles >= 9999999) total_miles = 0;
                else total_miles++;
            }
        }
        //CAN节点故障计时3s
        if (Fcan_count >= CAN_TIME)Fcan_count = CAN_TIME;
        else Fcan_count++;
        if (Rcan_count >= CAN_TIME)Rcan_count = CAN_TIME;
        else Rcan_count++;
        if (Mcan_count >= CAN_TIME)Mcan_count = CAN_TIME;
        else Mcan_count++;
        if (Ecan_count >= CAN_TIME)Ecan_count = CAN_TIME;
        else Ecan_count++;
        if (Vcan_count >= CAN_TIME)Vcan_count = CAN_TIME;
        else Vcan_count++;
        if (Bcan_count >= CAN_TIME)Bcan_count = CAN_TIME;
        else Bcan_count++;
        if (Dcan_count >= CAN_TIME)Dcan_count = CAN_TIME;
        else Dcan_count++;
        if (Acan_count >= CAN_TIME)Acan_count = CAN_TIME;
        else Acan_count++;

        if (cnt4 >= 9) {//1s
            cnt4 = 0;
            F1000ms = 1;
            FLASH_1s = !FLASH_1s;
          PCAN_send_mile();
        } else cnt4++;
    } else cnt1++;

    if (Fcount >= 39) {//0.4s
        Fcount = 0;
        F400ms = 1;
        FLASH = !FLASH;
    } else Fcount++;

    if (Kcount >= 29) {//300ms
        Kcount = 0;
        F300ms = 1;
        kFLASH = !kFLASH;
    } else Kcount++;

    if (cnt2 >= 49) {//500ms
        cnt2 = 0;
        F500ms = 1;     
        F500ms_LCD = !F500ms_LCD;
    } else cnt2++;

    if(F1200ms++ > 120)
    {
	bOk = 1;
	F1200ms = 0;
    }

    if (wake_up3) { //ON开关判断//消抖//M_ON在掉电的一瞬间会使上拉电源无效，而M_ON在断电会保持300ms，从而影响开关量采集
        if (cnt3 > 30) M_ON = 1;
        else cnt3++;
    } else {
        if (cnt3 < 1) M_ON = 0;
        else cnt3--;
    }
    
    if (IN10 && IN6 && (IN7 || IN8) && IN9) { //后雾灯开关是常开开关，消抖
        if (cnt5 > 10) rLED_flag = 1;
        else cnt5++;       
    }else if((IN6 && (IN7 || IN8) && IN9)==0){
        rLED_flag = 0;
        cnt5=0;
    }

    //bit  key_set;  
    if (!KEY_SET && pkey_set && !bkey_set) {
        key_set = 1;
        bkey_set = pkey_set;
    } else {
        if (!KEY_SET == pkey_set) bkey_set = pkey_set;
    }
    pkey_set = KEY_SET;

    //bit  key_up;
    if (!KEY_UP && pkey_up && !bkey_up) {
        key_up = 1; //向上按键
        bkey_up = pkey_up;
    } else {
        if (!KEY_UP == pkey_up) bkey_up = pkey_up;
    }
    pkey_up = KEY_UP;

    //bit key_AV;
    if (!KEY_AV && pkey_AV && !bkey_AV) {
        key_AV = 1;
        bkey_AV = pkey_AV;
    } else {
        if (!KEY_AV == pkey_AV)bkey_AV = pkey_AV;
    }
    pkey_AV = KEY_AV;


    if (KEY_SET) {
        if (lpcnt2 >= 300) {//长按SET按钮
            //lpcnt2 = 0;
            bkey_set = 1; //长按时取消单按键操作
            key_long_set = 1;           
        } else lpcnt2++;
    } else {
        lpcnt2 = 0;
    }

    if (KEY_AV && KEY_UP) {
        if (lpcnt1 >= 300) {//组合按钮，长按AV和UP按钮
            //lpcnt1 = 0;
            bkey_AV = 1; //长按时取消单按键操作
            bkey_up = 1; //长按时取消单按键操作
            key_duble = 1; //隐含功能按键
        } else lpcnt1++;
    } else {
        lpcnt1 = 0;
    }

    if (KEY_AV && (KEY_UP == 0)) { //长按 2S 清零单里程 长按AV
        if (lpcount >= 200) {
            single_miles = 0;
            lpcount = 0;
            save_flag = 1;
            bkey_AV = 1; //长按时取消单按键操作
            lpcount = 100;
        } else lpcount++;
    } else if (lpcount > 0) lpcount--;

    //_T1IF = 0;   __del
	#endif
}
/*
void TIMER4_Init(void) {
    TMR4=0;
    T4CON = 0x0010; // 1：8分频 Fcy/8 16MHZ/8=2MHz
    PR4 = 19999;  //10ms中断一次 
    _T4IF = 0;
    _T4IE = 1;
    T4CONbits.TON=1;
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void){
    static unsigned int cnt = 0;
    if (cnt >= 5) {//50ms
        cnt = 0;
        fFreq++;
    }else cnt++;
    
    
    
    
    _T4IF = 0;  
}*/

void Process_Can(void)
{
	if((RSCAN0.RFSTS0.UINT32 & 0xFF00) > 0)
	{
		Process_Can0();
	}
	
	if((RSCAN0.RFSTS1.UINT32 & 0xFF00) > 0)
	{
		Process_Can1();
	}
	
	if((RSCAN0.RFSTS2.UINT32 & 0xFF00) > 0)
	{
		Process_Can2();
	}
}

void gpio_init(void)
{
#if 1
	PORT_ISO.PM1.UINT16                &= ~0x0004;
	PORT_ISO.PMC1.UINT16              &= ~0x0004;             //P1_2   power control  output

	PORT_ISO.PM1.UINT16                &= ~0x0010;
	PORT_ISO.PMC1.UINT16              &= ~0x0010;             //P1_4  	BL PWM OUTPUT

	PORT_ISO.PM11.UINT16              &= ~0x0003;
	PORT_ISO.PMC11.UINT16            &= ~0x0003;             //P11_0 P11_1  POUT1 POUT2  output

	PORT_ISO.PM42.UINT16              &= ~0x8000;
	PORT_ISO.PMC42.UINT16            &= ~0x8000;             //P42_15   BUZZ output
#endif

#if 1
	PORT_ISO.PMC3.UINT16              &= ~0x01F0;             //P3_45678  key1 key2 ... 3 4 5
	PORT_ISO.PM3.UINT16                |= 0x01F0;
	PORT_ISO.PIBC3.UINT16             |= 0x01F0;                //key input setting
	
	PORT_ISO.PMC10.UINT16              &= ~0x0100;           //P10_8      key6
	PORT_ISO.PM10.UINT16                |= 0x0100;
	PORT_ISO.PIBC10.UINT16             |= 0x0100;              //key input setting
#endif
}

void pwr_on(void)
{
	PORT_ISO.P1.UINT16                 |= 0x0002;                 //P1_2 = 1
}

void pwr_off(void)
{
	PORT_ISO.P1.UINT16                 &= ~0x0002;              //P1_2 = 0
}


