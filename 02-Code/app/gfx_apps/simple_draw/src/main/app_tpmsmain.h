/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_tpmsmainframe.h
 * @brief  tpms warn info displayed on the main screen page
 * @version V1.0.0
 * @author  liubing
 * @date    2020-03-1
 * @warning No Warnings
 * @note <b>history:</b>
 * 1.
 *
 */
#ifndef TPMSM_H__
#define TPMSM_H__

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
void tpms_mainframe(uint8_t displayswitch);



/**
 * end of group BATINFOVIEW
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif
