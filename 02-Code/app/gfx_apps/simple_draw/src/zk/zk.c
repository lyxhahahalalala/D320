/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    zk.c
 * @brief   zi ku
 * @version V1.0.0
 * @author  liubing
 * @date    2018-11-15
 * @warning none
 * @note <b>history:</b>
 * 1. create file
 *
 */

/*******************************************************************************
 * INCLUDES
 */
#include "zk.h"

#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "img_format.h"
#include "write.h"
#include "r_config_d1x.h"
#include "ff.h"


/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */



/*******************************************************************************
 * TYPEDEFS
 */

/**
 * \defgroup GROUP_LOCAL_FUNCTIONS APP:ZK:LOCAL_FUNCTIONS
 */


/*******************************************************************************
 * CONSTANTS
 */
/*******************************************************************************
 * LOCAL FUNCTIONS DECLEAR
 */
/**
 * \brief get char info by unicode in font
 * \param [in] unicode - unicode value for char
 * \return char info
 * \retval NULL - not exist
 * \retval NOT NULL - char info
 */
const GUI_CHARINFO *zk_getCharInfo(uint16_t unicode);


void zk_prepWrite(r_drw2d_Device_t Dev, r_drw2d_Color_t  Color);

void zk_stopWrite(r_drw2d_Device_t Dev);

int32_t zk_getCharWidth(const GUI_CHARINFO *charInfo);
void zk_writeChar(r_drw2d_Device_t Dev, int PosX, int PosY, const GUI_CHARINFO *charInfo);
/*******************************************************************************
 * GLOBAL VARIABLES
 */


/*******************************************************************************
 * STATIC VARIABLES
 */

const GUI_FONT *s_ptZkFont = NULL;

r_drw2d_Device_t s_tDev;
r_drw2d_Color_t s_tColor;
int32_t s_i32Space = 0;


/*******************************************************************************
 * EXTERNAL VARIABLES
 */


/*******************************************************************************
 *  GLOBAL FUNCTIONS IMPLEMENTATION
 */



#define ZK_ERROR_NONE           0           // 成功
#define ZK_ERROR_NO_FONT        1           // 无字体
#define ZK_ERROR_NO_CHAR        2           // 无字符
#define ZK_ERROR_NOT_SUP        3           // 不支持
#define ZK_ERROR_ARG_ERR        4           // 参数错误

void zk_init(r_drw2d_Device_t dev, \
                const GUI_FONT *font, \
                r_drw2d_Color_t color, \
                int32_t space)
{
    s_tDev = dev;
    s_ptZkFont = font;
    s_tColor = color;
    s_i32Space = space;
}
int32_t zk_printNum(int32_t x, int32_t y, int32_t num, uint8_t dotPos, uint8_t align)
{
#define NUM_MAX_CHAR_CNT            10
    int32_t ret = ZK_ERROR_NONE;
    uint8_t isNeg = 0;      // 数值符号
    uint8_t charTable[NUM_MAX_CHAR_CNT + 2];      // 最多可以显示12个字符, num字符数， 小数点， 负号， 如-0.xxxxxxxxxx
    int32_t numTemp = 0;
    uint8_t charCnt;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    uint8_t writePos = 0;

    if (NULL == s_ptZkFont)
    {
        return -ZK_ERROR_NO_FONT;
    }

    if (dotPos > NUM_MAX_CHAR_CNT)
    {
        return -ZK_ERROR_ARG_ERR;
    }

    // 记录符号， 并转换为正数
    if (num < 0)
    {
        isNeg = 1;
        num = 0 - num;
    }

    zk_prepWrite(s_tDev, s_tColor);
    /**
     * \brief align
     * 左对齐，则xy为左起始坐标， 显示从高位开始显示
     * 右对齐，则xy为右起始坐标， 显示从低位开始显示
     */

    // 先获取字符
    numTemp = num;
    charCnt = 0;
    for (uint8_t i = 0; i < NUM_MAX_CHAR_CNT; i ++)
    {
        charTable[i] = (numTemp % 10) + '0';
        charCnt ++;
        numTemp /= 10;
        if (numTemp == 0)
        {
            break;
        }
    }

    // 插入小数点
    if (dotPos > 0)
    {
        // 当数字字符数小于小数点位置时， 需要补充0
        for (uint8_t i = charCnt; i <= dotPos; i ++)
        {
            charTable[i] = '0';
            charCnt ++;
        }

        // 移位， 以插入小数点
        for (uint8_t i = charCnt; i > dotPos; i -- )
        {
            charTable[i] = charTable[i - 1];
        }

        charTable[dotPos] = '.';
        charCnt ++;
    }

    // 插入负号
    if (isNeg != 0)
    {
        charTable[charCnt ++] = '-';
    }

    if (align == ZK_ALIGN_LEFT)        // 左对齐
    {
         do{
            unicode = charTable[charCnt - 1];

            // 获得字符handle
            ptCharInfo = zk_getCharInfo(unicode);

            if (ptCharInfo == NULL)
            {
                ret = ZK_ERROR_NO_CHAR;
                break;
            }

            // 显示字符
            zk_writeChar(s_tDev, x, y, ptCharInfo);

            // 计算坐标
            x += zk_getCharWidth(ptCharInfo);
            // 间隔处理
            x += s_i32Space;

            // 显示字符数
            charCnt --;

        }while(charCnt > 0);

    }
    else                                // 右对齐
    {
        do{
            unicode = charTable[writePos];

            // 获得字符handle
            ptCharInfo = zk_getCharInfo(unicode);

            if (ptCharInfo == NULL)
            {
                ret = ZK_ERROR_NO_CHAR;
                break;
            }

            // 计算坐标
            x -= zk_getCharWidth(ptCharInfo);

            // 显示字符
            zk_writeChar(s_tDev, x, y, ptCharInfo);

            // 间隔处理
            x -= s_i32Space;

            // 显示字符数
            writePos ++;

        }while(writePos < charCnt);
    }

    zk_stopWrite(s_tDev);

    return ret;
}

int32_t zk_printPosNum(int32_t x, int32_t y, uint32_t num, uint8_t dotPos, uint8_t align)
{
#define NUM_MAX_CHAR_CNT            10
    int32_t ret = ZK_ERROR_NONE;
    //uint8_t isNeg = 0;      // 数值符号
    uint8_t charTable[NUM_MAX_CHAR_CNT + 2];      // 最多可以显示12个字符, num字符数， 小数点， 负号， 如-0.xxxxxxxxxx
    uint32_t numTemp = 0;
    uint8_t charCnt;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    uint8_t writePos = 0;

    if (NULL == s_ptZkFont)
    {
        return -ZK_ERROR_NO_FONT;
    }

    if (dotPos > NUM_MAX_CHAR_CNT)
    {
        return -ZK_ERROR_ARG_ERR;
    }


    zk_prepWrite(s_tDev, s_tColor);
    /**
     * \brief align
     * 左对齐，则xy为左起始坐标， 显示从高位开始显示
     * 右对齐，则xy为右起始坐标， 显示从低位开始显示
     */

    // 先获取字符
    numTemp = num;
    charCnt = 0;
    for (uint8_t i = 0; i < NUM_MAX_CHAR_CNT; i ++)
    {
        charTable[i] = (numTemp % 10) + '0';
        charCnt ++;
        numTemp /= 10;
        if (numTemp == 0)
        {
            break;
        }
    }

    // 插入小数点
    if (dotPos > 0)
    {
        // 当数字字符数小于小数点位置时， 需要补充0
        for (uint8_t i = charCnt; i <= dotPos; i ++)
        {
            charTable[i] = '0';
            charCnt ++;
        }

        // 移位， 以插入小数点
        for (uint8_t i = charCnt; i > dotPos; i -- )
        {
            charTable[i] = charTable[i - 1];
        }

        charTable[dotPos] = '.';
        charCnt ++;
    }


    if (align == ZK_ALIGN_LEFT)        // 左对齐
    {
         do{
            unicode = charTable[charCnt - 1];

            // 获得字符handle
            ptCharInfo = zk_getCharInfo(unicode);

            if (ptCharInfo == NULL)
            {
                ret = ZK_ERROR_NO_CHAR;
                break;
            }

            // 显示字符
            zk_writeChar(s_tDev, x, y, ptCharInfo);

            // 计算坐标
            x += zk_getCharWidth(ptCharInfo);
            // 间隔处理
            x += s_i32Space;

            // 显示字符数
            charCnt --;

        }while(charCnt > 0);

    }
    else                                // 右对齐
    {
        do{
            unicode = charTable[writePos];

            // 获得字符handle
            ptCharInfo = zk_getCharInfo(unicode);

            if (ptCharInfo == NULL)
            {
                ret = ZK_ERROR_NO_CHAR;
                break;
            }

            // 计算坐标
            x -= zk_getCharWidth(ptCharInfo);

            // 显示字符
            zk_writeChar(s_tDev, x, y, ptCharInfo);

            // 间隔处理
            x -= s_i32Space;

            // 显示字符数
            writePos ++;

        }while(writePos < charCnt);
    }

    zk_stopWrite(s_tDev);

    return ret;
}

int32_t zk_printHex(int32_t x, int32_t y, uint32_t num, uint8_t cnt, uint8_t align)
{
    int32_t ret = ZK_ERROR_NONE;
    uint8_t val;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    uint8_t writePos = 0;

    if (NULL == s_ptZkFont)
    {
        return -ZK_ERROR_NO_FONT;
    }

    zk_prepWrite(s_tDev, s_tColor);
    /**
     * \brief align
     * 左对齐，则xy为左起始坐标， 显示从高位开始显示
     * 右对齐，则xy为右起始坐标， 显示从低位开始显示
     */

    if (align != ZK_ALIGN_RIGHT)        // 左对齐
    {
        ret = -ZK_ERROR_NOT_SUP;       // 暂时不支持左对齐
    }
    else                                // 右对齐
    {
        do{
            // 先获取显示的字符

            val = num & 0x0F;
            num >>= 4;

            if (val < 10)
            {
                unicode = val + '0';
            }
            else
            {
                unicode = val - 10 + 'A';
            }


            // 获得字符handle
            ptCharInfo = zk_getCharInfo(unicode);

            if (ptCharInfo == NULL)
            {
                ret = ZK_ERROR_NO_CHAR;
                break;
            }

            // 计算坐标
            x -= zk_getCharWidth(ptCharInfo);

            // 显示字符
            zk_writeChar(s_tDev, x, y, ptCharInfo);

            // 间隔处理
            x -= s_i32Space;

            // 显示字符数
            writePos ++;

        }while(writePos < cnt) ;

    }

    zk_stopWrite(s_tDev);

    return ret;
}


uint32_t zk_GetStrLen(const uint8_t *text)
{
	uint32_t len = 0x00;
    uint16_t unicode = 0;
	const uint8_t *pText = text;
    const GUI_CHARINFO *ptCharInfo = NULL;

	if (NULL == text)
    {
        len = 0x00;
    }
    else
    {
	    while((0 != pText[0]) || (0 != pText[1]))
	    {
	        unicode = pText[0];
	        unicode = (unicode << 8) | pText[1];

	        // 获得字符handle
	        ptCharInfo = zk_getCharInfo(unicode);
			
	        if (ptCharInfo == NULL)
	        {
	            len = 0x00;
	            break;
	        }
			
	        len += zk_getCharWidth(ptCharInfo);
			
	        // 间隔处理
	        len += s_i32Space;
			
	        pText += 2;
	    }
    }
	
	return len;
}

int32_t zk_printZH(int32_t x, int32_t y, const uint8_t *text)
{
    int32_t ret = ZK_ERROR_NONE;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText = text;

    if (NULL == s_ptZkFont)
    {
        return -ZK_ERROR_NO_FONT;
    }

    if (NULL == text)
    {
        return -ZK_ERROR_ARG_ERR;
    }

    zk_prepWrite(s_tDev, s_tColor);

    while((0 != pText[0]) || (0 != pText[1]))
    {
        unicode = pText[0];
        unicode = (unicode << 8) | pText[1];

        // 获得字符handle
        ptCharInfo = zk_getCharInfo(unicode);

        if (ptCharInfo == NULL)
        {
            ret = ZK_ERROR_NO_CHAR;
            break;
        }

        // 显示字符
        zk_writeChar(s_tDev, x, y, ptCharInfo);

        // 计算坐标
        x += zk_getCharWidth(ptCharInfo);

        // 间隔处理
        x += s_i32Space;

        pText += 2;
    }


    zk_stopWrite(s_tDev);
    return ret;
}
int32_t zk_printZH1(int32_t x, int32_t y, const uint8_t *text,uint8_t align)
{
    int32_t ret = ZK_ERROR_NONE;
    uint16_t unicode = 0;
	uint16_t charTable[100];
	uint8_t charCnt = 0;
	uint8_t writePos = 0;
	uint8_t charSum = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText = text;
	
    if (NULL == s_ptZkFont)
    {
        return -ZK_ERROR_NO_FONT;
    }
	
    if (NULL == text)
    {
        return -ZK_ERROR_ARG_ERR;
    }
	
    zk_prepWrite(s_tDev, s_tColor);
	
	//先获得显示的字符
	while((0 != pText[0]) || (0 != pText[1]))
	{
		charTable[charCnt] = (pText[0] << 8) | pText[1];
		pText += 2;
		charCnt++;
	}
	charSum=charCnt;
	
	if(align == ZK_ALIGN_LEFT)       // 左对齐
	{
		do{
			unicode = charTable[writePos];

			// 获得字符handle
			ptCharInfo = zk_getCharInfo(unicode);

			if (ptCharInfo == NULL)
			{
				ret = ZK_ERROR_NO_CHAR;
				break;
			}
			
			// 显示字符
			zk_writeChar(s_tDev, x, y, ptCharInfo);
			
			// 计算坐标
			x += zk_getCharWidth(ptCharInfo);
			
			// 间隔处理
			x += s_i32Space;
			
			writePos++;
			
		}while(writePos < charSum);
	}
	else if(align == ZK_ALIGN_RIGHT) //右对齐
	{
		do{
			unicode = charTable[charCnt-1];
			
			// 获得字符handle
			ptCharInfo = zk_getCharInfo(unicode);
			
			if (ptCharInfo == NULL)
			{
				ret = ZK_ERROR_NO_CHAR;
				break;
			}
			
			// 计算坐标
			x -= zk_getCharWidth(ptCharInfo);
			
			// 显示字符
			zk_writeChar(s_tDev, x, y, ptCharInfo);
			
			// 间隔处理
			x -= s_i32Space;
			
			charCnt--;
			
		}while(charCnt > 0);
		
	}
	else ;
	
    zk_stopWrite(s_tDev);
    return ret;
}
int32_t zk_printZH2(int32_t x, int32_t y, const uint8_t *text)
{
    int32_t ret = ZK_ERROR_NONE;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText = text;
	int32_t posX = 0;
	int32_t posY = 0;
	int32_t rowNum = 0;
	
    if (NULL == s_ptZkFont)
    {
        return -ZK_ERROR_NO_FONT;
    }

    if (NULL == text)
    {
        return -ZK_ERROR_ARG_ERR;
    }
	
    zk_prepWrite(s_tDev, s_tColor);
	
	posX = x;
	posY = y;
	
    while((0 != pText[0]) || (0 != pText[1]))
    {
        unicode = pText[0];
        unicode = (unicode << 8) | pText[1];
		
		if(unicode == 0x000A)
		{
			rowNum++;
			posX = x;
			posY = y + s_ptZkFont->yDist * rowNum;
		}
		else
		{
			// 获得字符handle
			ptCharInfo = zk_getCharInfo(unicode);
			
			if (ptCharInfo == NULL)
			{
				ret = ZK_ERROR_NO_CHAR;
				break;
			}
			
			// 显示字符
			zk_writeChar(s_tDev, posX, posY, ptCharInfo);
			
			// 计算字符宽度
			posX += zk_getCharWidth(ptCharInfo);
			// 间隔处理
			posX += s_i32Space;
		}
		
		pText += 2;
    }
	
    zk_stopWrite(s_tDev);
    return ret;
}



int32_t zk_printZKAlign(int32_t x, int32_t y, uint8_t align, const uint8_t *text)
{
    int32_t ret = ZK_ERROR_NONE;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText = text;
    int32_t width = 0;

    do{
        if (NULL == s_ptZkFont)
        {
            ret = -ZK_ERROR_NO_FONT;
        }

        if (NULL == text)
        {
            ret = -ZK_ERROR_ARG_ERR;
        }

        if (ZK_ALIGN_RIGHT == align)
        {
            // calc start for left align
            width = 0;
            while((0 != pText[0]) || (0 != pText[1]))
            {
                unicode = pText[0];
                unicode = (unicode << 8) | pText[1];

                // 获得字符handle
                ptCharInfo = zk_getCharInfo(unicode);

                if (ptCharInfo == NULL)
                {
                    ret = ZK_ERROR_NO_CHAR;
                    break;
                }

                // 计算坐标
                width += zk_getCharWidth(ptCharInfo);

                // 间隔处理
                width += s_i32Space;

                pText += 2;
            }

            x -= width;
            if (x < 0)
            {
                x = 0;
            }

        }

        ret = zk_printZH(x, y, text);

    }while(0);

    return ret;
}


int32_t zk_printAscii(int32_t x, int32_t y, const uint8_t *text)
{
    int32_t ret = ZK_ERROR_NONE;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText = text;

    if (NULL == s_ptZkFont)
    {
        return -ZK_ERROR_NO_FONT;
    }

    if (NULL == text)
    {
        return -ZK_ERROR_ARG_ERR;
    }

    zk_prepWrite(s_tDev, s_tColor);

    while(0 != pText[0])
    {
        unicode = pText[0];

        // 获得字符handle
        ptCharInfo = zk_getCharInfo(unicode);

        if (ptCharInfo == NULL)
        {
            ret = ZK_ERROR_NO_CHAR;
            break;
        }

        // 显示字符
        zk_writeChar(s_tDev, x, y, ptCharInfo);

        // 计算坐标
        x += zk_getCharWidth(ptCharInfo);

        // 间隔处理
        x += s_i32Space;

        pText ++;
    }


    zk_stopWrite(s_tDev);
    return ret;
}


int32_t zk_printAsciiAlign(int32_t x, int32_t y, uint8_t align, const uint8_t *text)
{
    int32_t ret = ZK_ERROR_NONE;
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText = text;
    int32_t width = 0;

    do{
        if (NULL == s_ptZkFont)
        {
            ret = -ZK_ERROR_NO_FONT;
        }

        if (NULL == text)
        {
            ret = -ZK_ERROR_ARG_ERR;
        }

        if (ZK_ALIGN_RIGHT == align)
        {
            // calc start for left align
            width = 0;
            while((0 != pText[0]))
            {
                unicode = pText[0];

                // 获得字符handle
                ptCharInfo = zk_getCharInfo(unicode);

                if (ptCharInfo == NULL)
                {
                    ret = ZK_ERROR_NO_CHAR;
                    break;
                }

                // 计算坐标
                width += zk_getCharWidth(ptCharInfo);

                // 间隔处理
                width += s_i32Space;

                pText ++;
            }

            x -= width;
            if (x < 0)
            {
                x = 0;
            }

        }

        ret = zk_printAscii(x, y, text);

    }while(0);

    return ret;
}


int32_t zk_oem2uni(const uint8_t *strGbk, uint8_t *strUni, uint32_t strUniLen)
{
    int32_t ret = 0;
    const uint8_t *ptr = strGbk;
    uint32_t idx = 0;
    WCHAR gbk = 0;
    WCHAR uni = 0;

    if ((NULL == ptr) || (strUniLen < 2))
    {
        ret  = -1;
    }
    else
    {
        while((idx + 2 < strUniLen) && (*ptr != 0))
        {
            gbk = *ptr++;
            if (gbk > 0x80)
            {
                gbk <<= 8;
                gbk |= *ptr++;
            }

            uni = ff_oem2uni(gbk, 936);
            strUni[idx++] = (uint8_t)(uni >> 8);
            strUni[idx++] = (uint8_t)uni;
        }

        strUni[idx++] = 0;
        strUni[idx++] = 0;

    }

    return ret;
}

static uint8_t l_au8StrUni[128];

uint8_t *zk_oem2uniStr(const uint8_t *strGbk)
{
    uint8_t *ret = NULL;

    if (0 == zk_oem2uni(strGbk, l_au8StrUni, sizeof(l_au8StrUni)))
    {
        ret = l_au8StrUni;
    }

    return ret;
}

/*******************************************************************************
 * LOCAL FUNCTIONS IMPLEMENTATION
 */
/**
 * \brief get char info by unicode in font
 * \param [in] unicode - unicode value for char
 * \return char info
 * \retval NULL - not exist
 * \retval NOT NULL - char info
 */
const GUI_CHARINFO *zk_getCharInfo(uint16_t unicode)
{
    const GUI_CHARINFO *ptInfo = NULL;
    const GUI_FONT_PROP *ptProp = s_ptZkFont->prop;

    while (NULL != ptProp)
    {
        // not exist
        if (unicode < ptProp->first)
        {
            break;
        }
        else if (unicode <= ptProp->last)   // find in this prop
        {
            ptInfo = &ptProp->paCharInfo[unicode - ptProp->first];
            break;
        }
        else    // may be exist in next prop
        {
            ptProp = ptProp->pNext;
        }
    }

    return ptInfo;
}

static r_drw2d_Rect_t rect;
static r_drw2d_Rect_t src_rect;
static r_drw2d_Texture_t tex;
r_drw2d_EffectStage_t effect[2];

void zk_prepWrite(r_drw2d_Device_t Dev, r_drw2d_Color_t  Color)
{
    volatile r_drw2d_Error_t err;


    tex.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_AL4;
    tex.Buffer.Size.Height = s_ptZkFont->ySize;
    tex.Flags = R_DRW2D_TEX_BILINEAR;
    tex.Handle = 0;


    err = R_DRW2D_CtxIdentity(Dev);
    err = R_DRW2D_CtxTextureIdentity(Dev);

    err = R_DRW2D_CtxBlendMode(Dev, R_DRW2D_BLENDMODE_SRC_OVER);
    err = R_DRW2D_CtxFillMode(Dev, R_DRW2D_FILLMODE_TEXTURE);

    effect[0].Name = R_DRW2D_EFFECT_MODULATE;
    effect[0].Args[0].Source = R_DRW2D_EFFECT_SOURCE_TEXTURE_UNIT;
    effect[0].Args[0].Param.Color.Source.TextureUnit = 0;
    effect[0].Args[0].Param.Color.Operand = R_DRW2D_EFFECT_COLOR_OPERAND_RGBA;
    effect[0].Args[1].Source = R_DRW2D_EFFECT_SOURCE_CONSTANT_COLOR;
    effect[0].Args[1].Param.Color.Source.ConstantColor = Color;
    effect[0].Args[1].Param.Color.Operand = R_DRW2D_EFFECT_COLOR_OPERAND_RGBA;

    err = R_DRW2D_CtxEffectsSet(Dev, effect, 1);



}

void zk_stopWrite(r_drw2d_Device_t Dev)
{
    R_DRW2D_CtxEffectsDelete(Dev);
}


int32_t zk_getCharWidth(const GUI_CHARINFO *charInfo)
{
    return charInfo->xDist;
}

void zk_writeChar(r_drw2d_Device_t Dev, int PosX, int PosY, const GUI_CHARINFO *charInfo)
{
    int       xsrc;
    int       hsrc;
    int       wsrc;
    volatile r_drw2d_Error_t err;
#if 1
    tex.Buffer.Data = (void*)charInfo->pData;
    tex.Buffer.Pitch = charInfo->bytesPerLine * 2;
    tex.Buffer.Size.Width = charInfo->bytesPerLine * 2;
    err = R_DRW2D_CtxTextureSet(Dev, 0, &tex);
#endif


    xsrc = 0;
    hsrc = s_ptZkFont->ySize;
    wsrc = charInfo->bytesPerLine * 2;  //(charInfo->xSize + 0) >> 0;

    rect.Pos.X = R_DRW2D_2X(PosX);
    rect.Pos.Y = R_DRW2D_2X(PosY);
    rect.Size.Height = R_DRW2D_2X(hsrc);
    rect.Size.Width = R_DRW2D_2X(wsrc);

    src_rect.Pos.X = R_DRW2D_2X(xsrc);
    src_rect.Pos.Y = R_DRW2D_2X(0);
    src_rect.Size.Height = R_DRW2D_2X(hsrc);
    src_rect.Size.Width = R_DRW2D_2X(wsrc);

    err = R_DRW2D_TextureBlit(Dev, &src_rect, &rect);
}


int32_t zk_getTextWidth(const uint8_t *strGbk)
{
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText;
	uint16_t Width = 0;
	
	pText=zk_oem2uniStr(strGbk);
	
    if (NULL == s_ptZkFont)
    {
        return Width;
    }

    if (NULL == pText)
    {
        return Width;
    }
	
    while((0 != pText[0]) || (0 != pText[1]))
    {
        unicode = pText[0];
        unicode = (unicode << 8) | pText[1];

        // 获得字符handle
        ptCharInfo = zk_getCharInfo(unicode);

        if (ptCharInfo == NULL)
        {
            break;
        }

        // 计算坐标
        Width += zk_getCharWidth(ptCharInfo);

        // 间隔处理
        Width += s_i32Space;
		
        pText += 2;
    }
    return Width;
}

int32_t zk_getTextWidth1(const uint8_t *strGbk)
{
    uint16_t unicode = 0;
    const GUI_CHARINFO *ptCharInfo = NULL;
    const uint8_t *pText;
	uint16_t Width = 0;
	uint16_t Width1 = 0;
	
	pText=zk_oem2uniStr(strGbk);
	
    if (NULL == s_ptZkFont)
    {
        return Width;
    }

    if (NULL == pText)
    {
        return Width;
    }
	
    while((0 != pText[0]) || (0 != pText[1]))
    {
        unicode = pText[0];
        unicode = (unicode << 8) | pText[1];
		if(unicode == 0x000A)
		{
			if(Width1 > Width)
			{
				Width = Width1;
				Width1 = 0;
			}
		}
		else
		{
			// 获得字符handle
			ptCharInfo = zk_getCharInfo(unicode);

			if (ptCharInfo == NULL)
			{
				break;
			}
			
			// 计算坐标
			Width1 += zk_getCharWidth(ptCharInfo);

			// 间隔处理
			Width1 += s_i32Space;
		}
		
        pText += 2;
    }
	
	if(Width1 > Width)
	{
		Width = Width1;
		Width1 = 0;
	}
	
    return Width;
}





