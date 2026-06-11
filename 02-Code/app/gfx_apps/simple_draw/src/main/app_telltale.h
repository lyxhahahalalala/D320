/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
* @file    app_telltale.h
 * @brief   telltale display
 * @version V1.0.0
 * @author  liubing
 * @date    2019-08-19
 * @warning No Warnings
 * @note <b>history:</b>
 * 1.
 *
 */
#ifndef TTB_H__
#define TTB_H__

/*******************************************************************************
 * INCLUDES
 */
#include "r_typedefs.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup BATINFOVIEW APP:VIEW
 * @{
 */


/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */


/*******************************************************************************
 * TYPEDEFS
 */
enum TELLTALE_ID {
	TELLTAL_START = 0,
	TELLTAL_LCD_TRAILER_ABS = TELLTAL_START, /*黄色-挂车ABS*/
	TELLTAL_LCD_PTO_UPLOAD,                  /*黄色-PTO上装指示*/
	TELLTAL_LCD_STEERING_LOCK,               /*黄色-转向锁止*/
	
	TELLTAL_LCD_LDWS_OFF,                    /*黄色-LDWS:车道偏离系统关闭*/
	TELLTAL_LCD_LDWS_LEFT,                   /*黄色-LDWS:车道左偏移*/
	TELLTAL_LCD_LDWS_RIGHT,                  /*黄色-LDWS:车道右偏移*/
	TELLTAL_LCD_LDWS_FLT,                    /*黄色-LDWS:车道偏离系统故障*/
	
	TELLTAL_LCD_FCWS_OFF,                    /*黄色-FCWS:碰撞预警系统关闭*/
	TELLTAL_LCD_FCWS_FLT,                    /*黄色-FCWS:前方碰撞系统故障*/
	TELLTAL_LCD_FCWS_LVL2,                   /*红色-FCWS:碰撞预警二级警告*/
	TELLTAL_LCD_FCWS_LVL1,                   /*黄色-FCWS:碰撞预警一级警告*/
	
	TELLTAL_LCD_AEBS_OFF,                    /*黄色-AEBS:自动紧急制动系统关闭*/
	TELLTAL_LCD_AEBS_LVL3,                   /*红色-AEBS:自动紧急制动三级警告*/
	TELLTAL_LCD_AEBS_LVL2,                   /*红色-AEBS:自动紧急制动二级警告*/
	TELLTAL_LCD_AEBS_LVL1,                   /*黄色-AEBS:自动紧急制动一级警告*/
	TELLTAL_LCD_AEBS_FLT,                    /*黄色-AEBS:自动紧急制动系统故障*/
	
	TELLTAL_LCD_BDS_OFF,                     /*黄色-BDS:盲区探测系统关闭*/
	TELLTAL_LCD_BDS_LEFT_LVL1,               /*黄色-BDS:左盲区探测一级预警*/
	TELLTAL_LCD_BDS_LEFT_LVL2,               /*黄色-BDS:左盲区探测二级预警*/
	TELLTAL_LCD_BDS_RIGHT_LVL1,              /*黄色-BDS:右盲区探测一级预警*/
	TELLTAL_LCD_BDS_RIGHT_LVL2,              /*黄色-BDS:右盲区探测二级预警*/
	TELLTAL_LCD_BDS_FLT,                     /*黄色-BDS:右盲区探测系统故障*/
	
	TELLTAL_LCD_DFMS_OFF,                    /*黄色-DFMS:疲劳预警系统关闭*/
	TELLTAL_LCD_DFMS_WARN_LVL1,              /*黄色-DFMS:疲劳预警一线*/
	TELLTAL_LCD_DFMS_WARN_LVL2,              /*黄色-DFMS:疲劳预警二级*/
	
	TELLTAL_LCD_EBS_FLT,                     /*红色-EBS:故障或失效*/
	TELLTAL_LCD_EBS_WARN,                    /*黄色-EBS:警告*/
	
	TELLTAL_LCD_ECAS_FLT,                    /*红色-ECAS:失效*/
	TELLTAL_LCD_ECAS_WARN,                   /*黄色-ECAS:警告*/
	TELLTAL_LCD_ECAS_HEIGHT_CTRL,            /*黄色-ECAS:高度控制指示*/
	TELLTAL_LCD_ECAS_BRIDGE_LIFT,            /*黄色-ECAS:举升桥提升状态指示*/
	TELLTAL_LCD_ECAS_DRIVE_ASSIST,           /*黄色-ECAS:驱动辅助状态指示*/
	
	TELLTAL_LCD_READY,                       /*绿色-READY*/
	TELLTAL_LCD_STOP,                        /*红色-STOP*/
	TELLTAL_LCD_SPEED_LMT,                   /*黄色-限速报警*/
	TELLTAL_LCD_HAZARD,                      /*黄色-HAZARD 紧急故障*/
	TELLTAL_LCD_EAC_FLT,                     /*红色-EAC 空压机故障*/
	TELLTAL_LCD_DRIVING_RANG_LOW,            /*红色-续航里程低*/
	
	TELLTAL_LCD_EV_MODE,                     /*绿色-EV纯电模式*/
	TELLTAL_LCD_FCV_MODE,                    /*绿色-FCV混动模式*/
	TELLTAL_LCD_FTS_OVER_TEMP,               /*红色-燃料电池冷却液温度过高报警*/
	TELLTAL_LCD_FCS_FLT,                     /*黄色-燃电系统故障*/
	TELLTAL_LCD_H2_LEAKAGE,                  /*红色-氢气泄露*/
	TELLTAL_LCD_H2_LOW,                      /*红色-氢气量低，加氢*/
	
	TELLTAL_LCD_ADU_DISABLE,                 /*灰色-ADU:自动驾驶系统不可用*/
	TELLTAL_LCD_ADU_READY,                   /*白色-ADU:自动驾驶系统就绪*/
	TELLTAL_LCD_ADU_ACTIVE,                  /*绿色-ADU:自动驾驶系统激活*/
	TELLTAL_LCD_ADU_FLT,                     /*红色-ADU:自动驾驶系统故障*/
	
	TELLTAL_LCD_ADU_MSG_ACTIVE_FAIL,         /*红色-ADU:不可用或激活失败*/
	TELLTAL_LCD_ADU_MSG_READY,               /*绿色-ADU:自动驾驶系统已就绪或已激活*/
	TELLTAL_LCD_ADU_MSG_TAKEOVER,            /*红色-ADU:请立即接管车辆*/
	TELLTAL_LCD_ADU_MSG_NOTICE,              /*黄色-ADU:请注意安全*/
	TELLTAL_LCD_ADU_MSG_OFF,                 /*白色-ADU:自动驾驶系统关闭，请注意安全驾驶*/
	
	TELLTAL_LCD_TPMS_WARN,                   /*黄色-TPMS报警*/
	
	
	TELLTALE_LAST
	
};


enum POPUP_ID {
	POPUP_START = 0,
	
	POPUP_HAZARD = POPUP_START,
	POPUP_LDWS_OFF,
	POPUP_FCWS_OFF,
	POPUP_AEBS_OFF,
	POPUP_LMT_SPD,
	POPUP_BDS_ON,
	POPUP_BDS_OFF,
	POPUP_DMS_ON,
	POPUP_EBS_RED,
	POPUP_EBS_YELLOW,
	POPUP_ABS,
	POPUP_ABS_TRAILER,
	POPUP_TPMS,
	
	POPUP_LAST
	
};


/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * GLOBAL VARIABLES DECLEAR
 */



/*******************************************************************************
 * GLOBAL FUNCTIONS DECLEAR
 */
/**
 * \brief update miles
 * \return none
 */

void IC_zijian(void);
void Display_Telltale(void);
void Display_TruckArea(void);
void loc_auxiliary_braking(void);

extern uint8_t LDW_Warn;
extern uint8_t FCW_Level;
extern uint8_t AEBS_Warn;
extern uint8_t BSD_Warn;
extern uint8_t ADU_Warn;
extern uint8_t TPMS_Warn;






/**
 * end of group BATINFOVIEW
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif
