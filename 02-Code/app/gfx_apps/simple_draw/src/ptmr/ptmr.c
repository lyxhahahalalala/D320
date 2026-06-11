/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    ptmr.c
 * @brief   poll timer
 * @version V1.0.0
 * @author  liubing
 * @date    2018-11-26
 * @warning none
 * @note <b>history:</b>
 * 1. create file
 *
 */

/*******************************************************************************
 * INCLUDES
 */
#include "ptmr.h"
#include "r_tick_api.h"
/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */
#define TICK_MAX                ((ptmrType_t)-1)
#define NULL                    ((void*)0)

/*******************************************************************************
 * TYPEDEFS
 */



/**
 * \defgroup GROUP_LOCAL_FUNCTIONS APP:PTMR:LOCAL_FUNCTIONS
 */


/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * LOCAL FUNCTIONS DECLEAR
 */



/*******************************************************************************
 * EXTERNAL VARIABLES
 */


/*******************************************************************************
 * GLOBAL VARIABLES
 */


/*******************************************************************************
 * STATIC VARIABLES
 */
static ptmrType_t     ptmrTick;


/*******************************************************************************
 *  GLOBAL FUNCTIONS IMPLEMENTATION
 */

void ptmr_Start(ptmrType_t* timer)
{
	// asAssert(timer!=NULL);

	if (ptmrTick == 0U)
	{
		*timer = 1;
	}
	else
	{
		*timer = ptmrTick;
	}
}

void ptmr_Stop(ptmrType_t* timer)
{
	// asAssert(timer!=NULL);
	*timer = 0;
}

ptmrType_t ptmr_Get(ptmrType_t* timer)
{
	ptmrType_t diff;
	// asAssert(timer!=NULL);

	if(0 == *timer)
	{
		diff = 0U;
	}
	else
	{
		if (ptmrTick >= *timer)
		{
			diff = ptmrTick - *timer;
		}
		else
		{
			diff = (TICK_MAX - *timer) + ptmrTick;
		}
	}
	return diff;
}

bool ptmr_IsStart(ptmrType_t *timer)
{
    return *timer != 0;
}

void ptmr_IncTick( void )
{
    ptmrTick ++;
}

ptmrType_t system_uptime(void)
{
	return ptmrTick;
}

void system_wait_us(ptmrType_t tickUS)
{
	R_TICK_WaitUS(0, tickUS);
}
void system_wait_ms(ptmrType_t tickMS)
{
	R_TICK_WaitMS(0, tickMS);
}

/*******************************************************************************
 * LOCAL FUNCTIONS IMPLEMENTATION
 */
