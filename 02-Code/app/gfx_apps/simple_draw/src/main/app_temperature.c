/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_temperature.c
 * @brief   temperature calculate and dispaly on the temperature area
 * @version V1.0.0
 * @author  liubing
 * @date    2019-08-19
 * @warning none
 * @note <b>history:</b>
 * 1. create file
 *
 */

/*******************************************************************************
 * INCLUDES
 */
#include "SYSTEM.h"
#include "zk.h"
#include "init.h"
#include "app_temperature.h"
#include "adc.h"

/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */



/*******************************************************************************
 * TYPEDEFS
 */

typedef struct{
    int degree;
    float Rt;
    float Rl;
    float Rh;

}TA;


static TA TemperatureArray[]={
    {-40,  52872,  50557,  55187}, //0
    {-40,  52872,  50557,  55187},
    {-35,  39999,  38376,  41623},
    {-30,  30470,  29327,  31613},
    {-25,  23340,  22534,  24147},
    {-20,  18041,  17469,  18613},
    {-15,  14005,  13598,  14411},
    {-10,  10965,  10675,  11254},
    { -5,   8652,   8446,   8858},
    {  0,   6880,   6732,   7027},
    {  5,   5498,   5393,   5603}, //10
    { 10,   4425,   4351,   4499},
    { 15,   3581,   3528,   3633},
    { 20,   2917,   2880,   2954},
    { 25,   2400,   2376,   2424},
    { 30,   1991,   1967,   2016},
    { 35,   1647,   1624,   1671},
    { 40,   1370,   1348,   1393},
    { 45,   1152,   1132,   1173},
    { 50,  973.1,  953.9,  992.3},
    { 55,  820.1,  802.5,  837.6}, //20
    { 60,  694.8,  678.8,  710.8},
    { 65,  593.1,  578.5,  607.7},
    { 70,  508.4,  495.1,  521.7},
    { 75,  436.7,  424.6,  448.7},
    { 80,  376.3,  365.4,  387.3},
    { 85,  326.2,  316.2,  336.2},
    { 90,  283.7,  274.7,  292.8},
    { 95,  248.2,  239.9,  256.4},
    {100,  217.8,  210.3,  225.3},
    {105,  191.1,  184.3,  198.0}, //30
    {110,  168.2,  162.0,  174.5},
    {115,  148.5,  142.9,  154.2},
    {120,  131.5,  126.3,  136.7},
    {125,  116.9,  112.2,  121.6},
    {130,  104.2,  99.86,  108.5},
    {135,  92.93,  88.97,  96.90},
    {140,  83.08,  79.44,  86.71}, //37
};

void Display_Temperature(void)
{
    static uint8_t sensor_error = 0;
    static uint8_t sensor_error_old = 0;
    static int sensordata = -50;
    uint32_t tmpdata;
	
    tmpdata = get_input_analog_quantity(AD_R_B18);
    if((tmpdata < 250) || (tmpdata > 55187))
		sensor_error = 1;
    else
	{
		sensor_error = 0;
		for(uint8_t i = 0; i < 38; i++)
		{
			if((TemperatureArray[i].Rl < tmpdata) && (TemperatureArray[i].Rh > tmpdata))
			{
				EevTemp = TemperatureArray[i].degree;
				break;
			}
			
			if(i == 37)
			{
				for(uint8_t j = 0; j < 38; j++)
				{
					if((TemperatureArray[j].Rt > tmpdata) && (TemperatureArray[j+1].Rt < tmpdata))
					{
						EevTemp = TemperatureArray[j].degree + 5 - 5*(tmpdata - TemperatureArray[j+1].Rt )/( TemperatureArray[j].Rt-TemperatureArray[j+1].Rt );
						break;
					}
				}
			}
		}
    }
	
    start_draw();
	
	if(mainview_update|| sensordata != EevTemp || sensor_error_old != sensor_error)
	{
		sensordata = EevTemp;
		sensor_error_old = sensor_error;
		
		loc_ClearRect(468 - 53, 15, 53, 40);
		
		if(sensor_error == 0)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38_num, VM_COLOR_WHITE, 0);
			zk_printNum(468, 15, EevTemp, 0, ZK_ALIGN_RIGHT);
		}
		else
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printZH1(468, 18, zk_oem2uniStr((uint8_t *)"--"), ZK_ALIGN_RIGHT);
		}
	}
	
    if(mainview_update)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printZH(473, 18, zk_oem2uniStr((uint8_t *)"¡æ"));
    }
	
	end_draw();

}















