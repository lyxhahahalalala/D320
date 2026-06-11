#ifndef HAL_DS1302_H_
#define HAL_DS1302_H_


void DS1302_init(void);
void DS1302_ReadTime(DATE_t *pTime);
void DS1302_SetTime(DATE_t *pTime);



#endif
