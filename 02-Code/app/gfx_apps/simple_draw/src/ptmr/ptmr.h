/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    ptmr.h
 * @brief   poll timer
 * @version V1.0.0
 * @author  liubing
 * @date    2018-11-26
 * @warning No Warnings
 * @note <b>history:</b>
 * 1.
 *
 */
#ifndef PTMR_H__
#define PTMR_H__

/*******************************************************************************
 * INCLUDES
 */
#include "r_typedefs.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup PTMR APP:PTMR
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
/* Kernel Timer.
 * If Tick is 1ms per Tick,
 * then counter max time is (0xFFFFFFFF)ms = 49.71 days.
 */
typedef uint32_t ptmrType_t;

/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * GLOBAL VARIABLES DECLEAR
 */

/*******************************************************************************
 * GLOBAL FUNCTIONS DECLEAR
 */


void ptmr_Start(ptmrType_t* timer);
void ptmr_Stop(ptmrType_t* timer);
ptmrType_t ptmr_Get(ptmrType_t* timer);
bool ptmr_IsStart(ptmrType_t *timer);
void ptmr_IncTick( void );
ptmrType_t system_uptime(void);
void system_wait_us(ptmrType_t tickUS);
void system_wait_ms(ptmrType_t tickMS);




/**
 * end of group PTMR
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif
