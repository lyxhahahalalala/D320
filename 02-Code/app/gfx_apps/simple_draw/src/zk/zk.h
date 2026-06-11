/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    zk.h
 * @brief   ziku
 * @version V1.0.0
 * @author  liubing
 * @date    2018-11-15
 * @warning No Warnings
 * @note <b>history:</b>
 * 1.
 *
 */
#ifndef ZK_H__
#define ZK_H__

/*******************************************************************************
 * INCLUDES
 */
#include "r_typedefs.h"
#include "zkfont.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "img_format.h"
#include "r_config_d1x.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup ZIKU APP:ZK
 * @{
 */


/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */
#define ZK_ALIGN_LEFT           0u
#define ZK_ALIGN_RIGHT          1u

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * GLOBAL VARIABLES DECLEAR
 */
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontwryh22;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontwryh24;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontwryh28;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontwryh31;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontwryh38_num;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontwryhbold38_num;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontwryh38bold;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontxunhang;


/*******************************************************************************
 * GLOBAL FUNCTIONS DECLEAR
 */

void zk_init(r_drw2d_Device_t dev, \
                const GUI_FONT *font, \
                r_drw2d_Color_t color, \
                int32_t space);

int32_t zk_printNum(int32_t x, int32_t y, int32_t num, uint8_t dotPos, uint8_t align);
int32_t zk_printPosNum(int32_t x, int32_t y, uint32_t num, uint8_t dotPos, uint8_t align);
int32_t zk_printHex(int32_t x, int32_t y, uint32_t num, uint8_t cnt, uint8_t align);
uint32_t zk_GetStrLen(const uint8_t *text);

int32_t zk_printZH(int32_t x, int32_t y, const uint8_t *text);
int32_t zk_printZKAlign(int32_t x, int32_t y, uint8_t align, const uint8_t *text);
int32_t zk_printAlign(int32_t x, int32_t y, const uint8_t *text);
int32_t zk_printAsciiAlign(int32_t x, int32_t y, uint8_t align, const uint8_t *text);
int32_t zk_oem2uni(const uint8_t *strGbk, uint8_t *strUni, uint32_t strUniLen);
uint8_t *zk_oem2uniStr(const uint8_t *strGbk);


int32_t zk_printZH1(int32_t x, int32_t y, const uint8_t *text,uint8_t align);
int32_t zk_printZH2(int32_t x, int32_t y, const uint8_t *text);
int32_t zk_getTextWidth(const uint8_t *strGbk);
int32_t zk_getTextWidth1(const uint8_t *strGbk);


/**
 * end of group ZK
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif


