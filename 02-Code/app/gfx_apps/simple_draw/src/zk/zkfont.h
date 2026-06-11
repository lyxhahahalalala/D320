/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    zkfont.h
 * @brief   ziku font
 * @version V1.0.0
 * @author  liubing
 * @date    2018-11-15
 * @warning No Warnings
 * @note <b>history:</b>
 * 1.
 *
 */
#ifndef ZKFONT_H__
#define ZKFONT_H__

/*******************************************************************************
 * INCLUDES
 */
#include "r_typedefs.h"
#include "zkfont.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup font APP:ZK
 * @{
 */


/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */
#ifndef NULL
#define NULL	 (void*)(0)
#endif

#define GUI_FONTTYPE_PROP_AA2       1
#define GUI_FONTTYPE_PROP_AA4       2
#define GUI_CONST_STORAGE           const

/*******************************************************************************
 * TYPEDEFS
 */
typedef struct {
    uint8_t xSize; //字体X轴宽度，指的是显示信息宽度
    uint8_t xDist; //字体X轴距离，指的是字体占用宽度，XDist>=XSize
    uint8_t bytesPerLine; //字体X轴字节数
    const uint8_t* pData;
} GUI_CHARINFO;

typedef struct {
    uint8_t xSize;
    uint8_t ySize;
    int8_t xPos;
    int8_t yPos;
    uint8_t xDist;
    const uint8_t* pData;
}GUI_CHARINFO_EXT;

typedef struct tag_GUI_FONT_PROP {
    unsigned short first; //区域第一个字符的编码
    unsigned short last;  //区域最好一个字符的编码
    const GUI_CHARINFO  * paCharInfo; //属于该区域字符的字体信息
    const struct tag_GUI_FONT_PROP * pNext; //指向字体下一个编码区域
} GUI_FONT_PROP;

typedef struct tag_GUI_FONT_PROP_EXT {
    unsigned short first; //区域第一个字符的编码
    unsigned short last;  //区域最好一个字符的编码
    const GUI_CHARINFO_EXT  * paCharInfo; //属于该区域字符的字体信息
    const struct tag_GUI_FONT_PROP_EXT * pNext; //指向字体下一个编码区域
} GUI_FONT_PROP_EXT;

typedef struct{
    uint8_t fontType;
    uint8_t ySize; //该字体的Y轴高度，指的是显示信息高度
    uint8_t yDist; //该字体的Y轴距离，指的是字体占用高度，YDist>=YSize
    int8_t xMag; //该字体的X轴放大
    int8_t yMag; //该字体的Y轴放大
    const GUI_FONT_PROP * prop;
    uint8_t baseLine;   //该字体Y轴行数
    uint8_t lHeight;     /* height of a small lower case character(a,x) */
    uint8_t cHeight;     /* height of a small upper case character(A,X) */
}GUI_FONT;

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
 * end of group font
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif
