/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    interval.h
 * @brief   
 * @version V1.0
 * @author  liuwenlong
 * @date    2022-6-6
 * @note <b>history:</b>
 * 1. create file
 *
 */

#ifndef _INTERVAL_H
#define	_INTERVAL_H



typedef enum {
    NONCYCLE,
    CYCLE
} CYCLE_EN;

typedef struct {
	uint8_t  req;               //启动请求
    uint8_t  status;            //标记状态
    
    const uint32_t ON_time;     //条件满足，工作时间 T = out_time * 处理间隔时间
    const CYCLE_EN Cycle_en;    //是否循环执行
    const uint32_t OFF_time;    //循环执行时，间歇时间 T = stop_time * 处理间隔时间
    const uint32_t cycle_num;   //循环次数，0为无限循环
    
	uint32_t run_time;          //已经运行时间
    uint32_t run_count;         //已经运行次数
	
} INTERVAL_t;



void Interval_Init(INTERVAL_t *tBlink, uint8_t Num);
void Interval_Count(void);
void Set_Interval_Req(bool Condition, uint8_t index);
uint8_t Get_Interval_Status(uint8_t index);




#endif	/* SYSTEM_H */


