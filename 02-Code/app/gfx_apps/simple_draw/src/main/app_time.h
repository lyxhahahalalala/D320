/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_time.h
 * @brief   time display
 * @version V1.0.0
 * @author  liubing
 * @date    2019-08-19
 * @warning No Warnings
 * @note <b>history:</b>
 * 1.
 *
 */
#ifndef _TIME_H__
#define _TIME_H__

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
 void Display_Date(void);
 //void TimeFrameUpdate(void);
//void TimeFrame(unsigned char index) ;

 extern unsigned char decimal_bcd(unsigned char decimal);//10进制转换bcd格式
 extern unsigned char bcd2decimal(unsigned char bcd);//bcd进制转换10格式
 extern uint8_t CaculateWeekDay(int y,int m, int d);


/**
 * end of group BATINFOVIEW
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif
/*===========================================================================
* File Revision History (bottom to top: first revision to last revision)
*============================================================================
* $Log:$
* Rev:     Userid:       Date:       (Description on following lines: CR #, etc.)
* -------  ------------  -----------
*
*  
*===========================================================================*/
