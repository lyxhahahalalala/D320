/******************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    key.c
 * @brief   key app
 * @version V1.0
 * @author  liuwenlong
 * @date    2028-7-8
 * @note <b>history:</b>
 * 1. create file
 *
 ******************************************************************************/


#ifndef _KEY_H
#define	_KEY_H

typedef union
{
    struct
    {
        unsigned char bit1 : 1;
        unsigned char bit2 : 1;
        unsigned char bit3 : 1;
        unsigned char bit4 : 1;
        unsigned char bit5 : 1;
        unsigned char bit6 : 1;
        unsigned char bit7 : 1;
        unsigned char bit8 : 1;
    };
    unsigned char byte;
} KEY_BIT_t;
extern KEY_BIT_t  pKEY[3];

#define key_set 		pKEY[0].bit1 
#define pkey_set		pKEY[0].bit2 
#define bkey_set		pKEY[0].bit3
#define key_long_set	pKEY[0].bit4

#define key_up 			pKEY[0].bit5 
#define pkey_up			pKEY[0].bit6  
#define bkey_up			pKEY[0].bit7
#define key_long_up		pKEY[0].bit8

#define key_AV 			pKEY[1].bit1
#define pkey_AV			pKEY[1].bit2
#define bkey_AV			pKEY[1].bit3
#define key_long_av   	pKEY[1].bit4

#define key_clear		pKEY[1].bit5 
#define pkey_clear		pKEY[1].bit6 
#define bkey_clear		pKEY[1].bit7
#define key_long_clear	pKEY[1].bit8

#define key_duble		pKEY[2].bit1 //长按AV和UP按钮，隐藏组合按钮


void Key_Init(void);
void KeyProcess(void);



#endif	/* KEY_H */


