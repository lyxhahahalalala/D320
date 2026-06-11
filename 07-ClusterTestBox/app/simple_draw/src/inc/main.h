/***********************************************************************************************
PROJECT : simple_draw
FILE   : $Id: main.h 1158 2013-07-30 11:51:37Z florian.zimmermann $
============================================================================ 
DESCRIPTION
header for simple_draw
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.
****************************************************************************
*/
#ifndef MAIN_H_
#define MAIN_H_


#ifdef __cplusplus
extern "C" {
#endif
//#define LOC_DISPLAY_NAME   "VESA_640x480_60Hz"
#define LOC_DISPLAY_NAME  "TFT800480-77-E"
// #define LOC_DISPLAY_NAME   "NEC_QVGA_TOUCH_240x320"

#define LOC_DISPLAY_WIDTH  800
#define LOC_DISPLAY_STRIDE 800
#define LOC_DISPLAY_HEIGHT 480
#define LOC_DISPLAY_BPP    4

#define LOC_JCUA_UNIT (0u)
#define LOC_SFMA_UNIT (0u)
#define LOC_VDCE_UNIT (0u)
/*******************************************************************************
  Section: Local Functions
*/

void loc_Error(int32_t Err);

void loc_StartSFMA(void);
void loc_StopSFMA(void);

void loc_VoweStart(void);
void loc_VoweStop(void);
void loc_Vowe_DLChange(int32_t dl_cnt);

void loc_RenderClear(int32_t Frame);
void start_draw(void);
void end_draw(void);


#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
