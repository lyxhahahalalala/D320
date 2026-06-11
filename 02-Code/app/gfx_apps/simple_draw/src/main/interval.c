/******************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    interval.c
 * @brief   interval application
 * @version V1.0
 * @author  liuwenlong
 * @date    2022-6-6
 * @note <b>history:</b>
 * 1. create file
 *
 */

#include "r_typedefs.h"
#include "iodefine.h"
#include "interval.h"
#ifndef NULL
#define NULL  ((void*)0)
#endif

static INTERVAL_t  *locBlink;
static uint8_t  locBlinkNum;


void Interval_Init(INTERVAL_t *tBlink, uint8_t Num)
{
	if(tBlink != NULL)
	{
		locBlink = tBlink;
		
		locBlinkNum = Num;
		
	}
}

void Interval_Count(void) //10ms task
{
    unsigned char i = 0; 
	
    for (i = 0; i < locBlinkNum; i++)
    {
		if(locBlink[i].ON_time != 0)
		{
			if (locBlink[i].req && ((locBlink[i].run_count < locBlink[i].cycle_num) || (!locBlink[i].cycle_num)))
				locBlink[i].run_time = locBlink[i].run_time + 1;
			else if (locBlink[i].run_time > 0 && locBlink[i].run_time < (locBlink[i].ON_time + locBlink[i].OFF_time))
				locBlink[i].run_time = locBlink[i].run_time + 1;
			else if ((locBlink[i].run_count > 0) && (locBlink[i].run_count < locBlink[i].cycle_num))
				locBlink[i].run_time = locBlink[i].run_time + 1;
			else
			{
				locBlink[i].run_time = 0;
				if (!locBlink[i].req)
					locBlink[i].run_count = 0;
			}

			if (locBlink[i].run_time > 0 && locBlink[i].run_time < locBlink[i].ON_time)
				locBlink[i].status = 1;
			else if (!locBlink[i].Cycle_en)
				locBlink[i].status = 0;
			else if (locBlink[i].run_time < (locBlink[i].ON_time + locBlink[i].OFF_time))
				locBlink[i].status = 0;
			else
			{
				locBlink[i].run_time = 0;
				locBlink[i].run_count++;
				locBlink[i].status = 0;
			}
		}
    }
}

void Set_Interval_Req(bool Condition, uint8_t index)
{
    if (index < locBlinkNum)
    {
        if(Condition)
        {
            locBlink[index].req = 1;
        } 
        else
        {
            locBlink[index].req = 0;
        }
    }
}

uint8_t Get_Interval_Status(uint8_t index)
{
	return locBlink[index].status;
}




