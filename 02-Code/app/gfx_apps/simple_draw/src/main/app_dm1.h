/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_dm1.c
 * @brief   
 * @version V1.0
 * @author  liuwenlong
 * @date    2022-7-18
 * @note <b>history:</b>
 * 1. create file
 *
 */

#ifndef _APP_DM1_H_
#define _APP_DM1_H_



enum{
/* 0*/DM1_82 = 0,   //TMS  电池冷却系统
/* 1*/DM1_05,   //SCU
/* 2*/DM1_03,   //AMT  自动变速箱系统
/* 3*/DM1_F4,   //BMS
/* 4*/DM1_EF,   //MCU
/* 5*/DM1_F0,   //MCU2
/* 6*/DM1_31,   //HCU
/* 7*/DM1_F5,   //FCU
/* 8*/DM1_F6,   //HMS
/* 9*/DM1_0B,   //ABS_EBS
/*10*/DM1_E8,   //Camera
/*11*/DM1_2A,   //KnorrRadar
/*12*/DM1_A0,   //YZSRadar
/*13*/DM1_A7,   //ATS 冷却系统
/*14*/DM1_50,   //EPB
/*15*/DM1_84,   //外挂DCDC
/*16*/DM1_85,   //外挂DCDC2
/*17*/DM1_10,   //
/*18*/DM1_53,   //
/*19*/DM1_DB,   //换电系统
/*20*/DM1_A8,   //ATS 冷却系统
/*21*/DM1_8F,   //燃料电池冷却系统（FTS）
/*22*/DM1_13,   //燃料电池冷却系统（FTS）

/* 0*/DM1_41,   //HJYX 环境影像
/* 1*/DM1_E7,   //DMS  疲劳驾驶
/* 2*/DM1_33,   //TPMS 胎压系统
/* 3*/DM1_EC,   //DCM
/* 4*/DM1_32,   //BCM
/* 5*/DM1_2E,   //HSCU 高压转向
/* 6*/DM1_4D,   //LSCU 低压转向
/* 7*/DM1_1E,   //PDU  高压配电盒
/* 8*/DM1_30,   //ACU  空压机
/* 9*/DM1_1A,   //DCDC
/*10*/DM1_2F,   //ECAS
	DM1_NULL
};
#define DM1_OWNER_NUM    DM1_NULL //DM1报文原地址总数
#define MAX_ERR_NUM      10  //最多缓存10个故障
#define Multip_ERR_FILL  ((((MAX_ERR_NUM * 4 + 2) % 7) > 0) ? 1 : 0)
#define MultipDM1_MaxErrPacketNum   ((MAX_ERR_NUM * 4 + 2) / 7 + Multip_ERR_FILL) //最多接收的数据包数量

#define ICU_MAX_ERR_NUM     50  
#define ErrValidDataNum(n)  (n * 4 + 2)
#define ErrPacketFill(n)    (((ErrValidDataNum(n) % 7) > 0) ? 1 : 0)
#define DM1ErrPacketNum(n)  ((n * 4 + 2) / 7 + ErrPacketFill(n)) //接收的数据包数量

#pragma pack(1)
typedef union
{
	struct{
		uint32_t SPN:19;
		uint8_t  FMI:5;
		uint8_t  OC:7;
		uint8_t  CM:1;
	};
	uint8_t byte[4];
}DTC_BUF_t;

typedef union
{
	struct{
		
		uint8_t  ProtectLamp:2,
				 AmberWarnLamp:2,
				 RedStopLamp:2,
				 MalfunctionLamp:2;
		uint8_t  Flash_ProtectLamp:2,
				 Flash_AmberWarnLamp:2,
				 Flash_RedStopLamp:2,
				 Flash_MalfunctionLamp:2;
		
		DTC_BUF_t DTC[MAX_ERR_NUM];
	};
	uint8_t byte[MAX_ERR_NUM * 4 + 2];
}DM1_BUF_t;
#pragma pack()

#pragma pack(1)
typedef struct{
	uint8_t SA;
	uint8_t clearfltflg;
	uint8_t FltNum;
	DM1_BUF_t FltBuf;
	
}CURRENNT_FLT_INFO_t;
#pragma pack()

extern CURRENNT_FLT_INFO_t  CurrentFltInfo[DM1_OWNER_NUM];

extern uint16_t icuCrtFltNum;      //当前仪表发出的故障数量
extern DTC_BUF_t icuCrtDM1Flt[ICU_MAX_ERR_NUM]; //当前仪表发出的故障



typedef struct {
	const uint8_t  SourceAdress;
    const uint32_t spn;
    const uint8_t  fmi;
    const uint8_t  *msg[2];
	uint8_t set;
	
}canDMInterpret_t;

extern uint16_t TotalDM;
extern uint16_t DM1_Ftl_Sum;
extern canDMInterpret_t  DMPackage[];


extern uint16_t bmsListCrtFltPos[100]; //电池当前列表中故障发生位置
extern uint16_t bmsCrtFltNum;      //当前电池发出的故障数量



void DM1_Flt_Init(void);
void DM1_Flt_Scan_Check(void); //故障扫描检测
bool get_dm1_flt_status(uint8_t owner, uint32_t spn, uint8_t fmi);


#endif



