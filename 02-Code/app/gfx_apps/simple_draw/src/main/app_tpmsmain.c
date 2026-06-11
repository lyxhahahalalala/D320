/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_tpmsmainframe.c
 * @brief  tpms warn info displayed on the main screen page
 * @version V1.0.0
 * @author  liuwenlong
 * @date    2020-07-06
 * @warning none
 * @note <b>history:</b>
 * 1. create file
 *
 */

/*****INCLUDES*****/
#include "SYSTEM.h"
#include "zk.h"
#include "init.h"
#include "app_tpmsmain.h"
#include "app_can.h"
#include "uds_support.h"


/*****TYPEDEFS*****/

/*****DEBUG SWITCH MACROS*****/

/*****MACROS*****/

/*****CONSTANTS*****/

/*****LOCAL_VARIABLE*****/
static uint8_t  old_tyre_status[12];
static uint16_t old_tyre_pressure[12];
static int16_t  old_tyre_temperature[12];
static uint8_t  old_tyre_timeout_status[12];


/*****defgroup LOCAL_FUNCTIONS*****/

/* 2Öá£¬6ÂÖÌ¥  eol_drivetype = 0 */
static const COORD_t B2001_Tyre_Info_Coord[B2001_TYRE_NUM][2] = 
{
	{{370,      275}, {318, 273}},
	{{422,      275}, {478, 273}},
	
	{{370,      365}, {222, 365}},
	{{378,      365}, {318, 365}},
	{{413,      365}, {478, 365}},
	{{422,      365}, {573, 365}}
	
};
void tpms_b2001_update(void)
{
	uint8_t uniStr[64];
	uint32_t colorf;
	
	uint8_t  tyre_status[B2001_TYRE_NUM];
	
	uint16_t tyre_pressure[B2001_TYRE_NUM];
	uint16_t tyre_temperature[B2001_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2001_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2001_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2001_TYRE_NUM];
	
	tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
	tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
	tyre_status[2] = TyreLocation_pressureh[1].bit0 + TyreLocation_pressurel[1].bit0 + TyreLocation_fastlostgas[1].bit0 + TyreLocation_temperature[1].bit0;
	tyre_status[3] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
	tyre_status[4] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
	tyre_status[5] = TyreLocation_pressureh[1].bit3 + TyreLocation_pressurel[1].bit3 + TyreLocation_fastlostgas[1].bit3 + TyreLocation_temperature[1].bit3;
	
	tyre_pressure[0] = Wheels[0][0].pressure;
	tyre_pressure[1] = Wheels[0][1].pressure;
	tyre_pressure[2] = Wheels[1][0].pressure;
	tyre_pressure[3] = Wheels[1][1].pressure;
	tyre_pressure[4] = Wheels[1][2].pressure;
	tyre_pressure[5] = Wheels[1][3].pressure;
	
	tyre_temperature[0] = Wheels[0][0].temperature;
	tyre_temperature[1] = Wheels[0][1].temperature;
	tyre_temperature[2] = Wheels[1][0].temperature;
	tyre_temperature[3] = Wheels[1][1].temperature;
	tyre_temperature[4] = Wheels[1][2].temperature;
	tyre_temperature[5] = Wheels[1][3].temperature;
	
	tyre_timeout_status[0] = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1] = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2] = (wheelupdatedtimer[1][0]>=30);
	tyre_timeout_status[3] = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[4] = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[5] = (wheelupdatedtimer[1][3]>=30);
	
	tyre_pressure_status[0] = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1] = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2] = (TyreLocation_pressureh[1].bit0||TyreLocation_pressurel[1].bit0||TyreLocation_fastlostgas[1].bit0);
	tyre_pressure_status[3] = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[4] = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[5] = (TyreLocation_pressureh[1].bit3||TyreLocation_pressurel[1].bit3||TyreLocation_fastlostgas[1].bit3);
	
	tyre_temperature_status[0] = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1] = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2] = (TyreLocation_temperature[1].bit0);
	tyre_temperature_status[3] = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[4] = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[5] = (TyreLocation_temperature[1].bit3);
	
	start_draw();
	for(uint8_t i = 0; i < B2001_TYRE_NUM; i++)
	{
		if(update_tpmsvalue || old_tyre_status[i] != tyre_status[i])
		{
			old_tyre_status[i] = tyre_status[i];
			
			if(tyre_status[i] != 0)
				loc_Render_Tire(B2001_Tyre_Info_Coord[i][0].Pos_X, B2001_Tyre_Info_Coord[i][0].Pos_Y, RED);
			else
				loc_Render_Tire(B2001_Tyre_Info_Coord[i][0].Pos_X, B2001_Tyre_Info_Coord[i][0].Pos_Y, GRAY);
		}
		
		if(update_tpmsvalue)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
			zk_printZH(B2001_Tyre_Info_Coord[i][1].Pos_X, B2001_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
		}
		
		if(update_tpmsvalue || old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2001_Tyre_Info_Coord[i][1].Pos_X - 2 - 40, B2001_Tyre_Info_Coord[i][1].Pos_Y, 40, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2001_Tyre_Info_Coord[i][1].Pos_X - 2, B2001_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*0.2, 1, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2001_Tyre_Info_Coord[i][1].Pos_X - 2, B2001_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"--"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(update_tpmsvalue || old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2001_Tyre_Info_Coord[i][1].Pos_X + 10, B2001_Tyre_Info_Coord[i][1].Pos_Y, 33, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2001_Tyre_Info_Coord[i][1].Pos_X + 10, B2001_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2001_Tyre_Info_Coord[i][1].Pos_X + 10, B2001_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
	}
	
	end_draw();
}

/* 3Öá£¬8ÂÖÌ¥  eol_drivetype = 1 */
static const COORD_t B2003_Tyre_Info_Coord[B2003_TYRE_NUM][2] = 
{
	{{370,      275},    {318, 273}},
	{{422,      275},    {478, 273}},
	
	{{370,      275+40}, {318, 273+40}},
	{{422,      275+40}, {478, 273+40}},
	
	{{370,      365},    {222, 365}},
	{{378,      365},    {318, 365}},
	{{413,      365},    {478, 365}},
	{{422,      365},    {573, 365}}
	
};
void tpms_b2003_update(void)
{
	uint8_t uniStr[64];
	uint32_t colorf;
	
    uint8_t  tyre_status[B2003_TYRE_NUM];
	
	uint16_t tyre_pressure[B2003_TYRE_NUM];
	int16_t  tyre_temperature[B2003_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2003_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2003_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2003_TYRE_NUM];
	
	tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
	tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
	tyre_status[2] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
	tyre_status[3] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
	tyre_status[4] = TyreLocation_pressureh[2].bit0 + TyreLocation_pressurel[2].bit0 + TyreLocation_fastlostgas[2].bit0 + TyreLocation_temperature[2].bit0;
	tyre_status[5] = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
	tyre_status[6] = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
	tyre_status[7] = TyreLocation_pressureh[2].bit3 + TyreLocation_pressurel[2].bit3 + TyreLocation_fastlostgas[2].bit3 + TyreLocation_temperature[2].bit3;
	
	tyre_pressure[0] = Wheels[0][0].pressure;
	tyre_pressure[1] = Wheels[0][1].pressure;
	tyre_pressure[2] = Wheels[1][1].pressure;
	tyre_pressure[3] = Wheels[1][2].pressure;
	tyre_pressure[4] = Wheels[2][0].pressure;
	tyre_pressure[5] = Wheels[2][1].pressure;
	tyre_pressure[6] = Wheels[2][2].pressure;
	tyre_pressure[7] = Wheels[2][3].pressure;
	
	tyre_temperature[0] = Wheels[0][0].temperature;
	tyre_temperature[1] = Wheels[0][1].temperature;
	tyre_temperature[2] = Wheels[1][1].temperature;
	tyre_temperature[3] = Wheels[1][2].temperature;
	tyre_temperature[4] = Wheels[2][0].temperature;
	tyre_temperature[5] = Wheels[2][1].temperature;
	tyre_temperature[6] = Wheels[2][2].temperature;
	tyre_temperature[7] = Wheels[2][3].temperature;
	
	tyre_timeout_status[0] = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1] = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2] = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[3] = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[4] = (wheelupdatedtimer[2][0]>=30);
	tyre_timeout_status[5] = (wheelupdatedtimer[2][1]>=30);
	tyre_timeout_status[6] = (wheelupdatedtimer[2][2]>=30);
	tyre_timeout_status[7] = (wheelupdatedtimer[2][3]>=30);
	
	tyre_pressure_status[0] = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1] = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2] = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[3] = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[4] = (TyreLocation_pressureh[2].bit0||TyreLocation_pressurel[2].bit0||TyreLocation_fastlostgas[2].bit0);
	tyre_pressure_status[5] = (TyreLocation_pressureh[2].bit1||TyreLocation_pressurel[2].bit1||TyreLocation_fastlostgas[2].bit1);
	tyre_pressure_status[6] = (TyreLocation_pressureh[2].bit2||TyreLocation_pressurel[2].bit2||TyreLocation_fastlostgas[2].bit2);
	tyre_pressure_status[7] = (TyreLocation_pressureh[2].bit3||TyreLocation_pressurel[2].bit3||TyreLocation_fastlostgas[2].bit3);
	
	tyre_temperature_status[0] = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1] = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2] = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[3] = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[4] = (TyreLocation_temperature[2].bit0);
	tyre_temperature_status[5] = (TyreLocation_temperature[2].bit1);
	tyre_temperature_status[6] = (TyreLocation_temperature[2].bit2);
	tyre_temperature_status[7] = (TyreLocation_temperature[2].bit3);
	
	for(uint8_t i = 0; i < B2003_TYRE_NUM; i++)
	{
		if(update_tpmsvalue || old_tyre_status[i] != tyre_status[i])
		{
			old_tyre_status[i] = tyre_status[i];
			
			if(tyre_status[i] != 0)
				loc_Render_Tire(B2003_Tyre_Info_Coord[i][0].Pos_X, B2003_Tyre_Info_Coord[i][0].Pos_Y, RED);
			else
				loc_Render_Tire(B2003_Tyre_Info_Coord[i][0].Pos_X, B2003_Tyre_Info_Coord[i][0].Pos_Y, GRAY);
		}
		
		if(update_tpmsvalue)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
			zk_printZH(B2003_Tyre_Info_Coord[i][1].Pos_X, B2003_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
		}
		
		if(update_tpmsvalue || old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2003_Tyre_Info_Coord[i][1].Pos_X - 2 - 40, B2003_Tyre_Info_Coord[i][1].Pos_Y, 40, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2003_Tyre_Info_Coord[i][1].Pos_X - 2, B2003_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*0.2, 1, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2003_Tyre_Info_Coord[i][1].Pos_X - 2, B2003_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"--"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(update_tpmsvalue||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2003_Tyre_Info_Coord[i][1].Pos_X + 10, B2003_Tyre_Info_Coord[i][1].Pos_Y, 33, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2003_Tyre_Info_Coord[i][1].Pos_X + 10, B2003_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2003_Tyre_Info_Coord[i][1].Pos_X + 10, B2003_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
	}
	
	end_draw();
	
}

/* 3Öá£¬10ÂÖÌ¥  eol_drivetype = 2 */
static const COORD_t B2004_Tyre_Info_Coord[B2004_TYRE_NUM][2] = 
{
	{{370,      275},    {318, 273}},
	{{422,      275},    {478, 273}},
	
	{{370,      275+40}, {222, 273+40}},
	{{378,      275+40}, {318, 273+40}},
	{{413,      275+40}, {478, 273+40}},
	{{422,      275+40}, {573, 273+40}},
	
	{{370,      365},    {222, 365}},
	{{378,      365},    {318, 365}},
	{{413,      365},    {478, 365}},
	{{422,      365},    {573, 365}}
	
};
void tpms_b2004_update(void)
{
	uint8_t uniStr[64];
	uint32_t colorf;
	
    uint8_t  tyre_status[B2004_TYRE_NUM];
	
	uint16_t tyre_pressure[B2004_TYRE_NUM];
	int16_t  tyre_temperature[B2004_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2004_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2004_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2004_TYRE_NUM];
	
	tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
	tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
	tyre_status[2] = TyreLocation_pressureh[1].bit0 + TyreLocation_pressurel[1].bit0 + TyreLocation_fastlostgas[1].bit0 + TyreLocation_temperature[1].bit0;
	tyre_status[3] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
	tyre_status[4] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
	tyre_status[5] = TyreLocation_pressureh[1].bit3 + TyreLocation_pressurel[1].bit3 + TyreLocation_fastlostgas[1].bit3 + TyreLocation_temperature[1].bit3;
	tyre_status[6] = TyreLocation_pressureh[2].bit0 + TyreLocation_pressurel[2].bit0 + TyreLocation_fastlostgas[2].bit0 + TyreLocation_temperature[2].bit0;
	tyre_status[7] = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
	tyre_status[8] = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
	tyre_status[9] = TyreLocation_pressureh[2].bit3 + TyreLocation_pressurel[2].bit3 + TyreLocation_fastlostgas[2].bit3 + TyreLocation_temperature[2].bit3;
	
	tyre_pressure[0] = Wheels[0][0].pressure;
	tyre_pressure[1] = Wheels[0][1].pressure;
	tyre_pressure[2] = Wheels[1][0].pressure;
	tyre_pressure[3] = Wheels[1][1].pressure;
	tyre_pressure[4] = Wheels[1][2].pressure;
	tyre_pressure[5] = Wheels[1][3].pressure;
	tyre_pressure[6] = Wheels[2][0].pressure;
	tyre_pressure[7] = Wheels[2][1].pressure;
	tyre_pressure[8] = Wheels[2][2].pressure;
	tyre_pressure[9] = Wheels[2][3].pressure;
	
	tyre_temperature[0] = Wheels[0][0].temperature;
	tyre_temperature[1] = Wheels[0][1].temperature;
	tyre_temperature[2] = Wheels[1][0].temperature;
	tyre_temperature[3] = Wheels[1][1].temperature;
	tyre_temperature[4] = Wheels[1][2].temperature;
	tyre_temperature[5] = Wheels[1][3].temperature;
	tyre_temperature[6] = Wheels[2][0].temperature;
	tyre_temperature[7] = Wheels[2][1].temperature;
	tyre_temperature[8] = Wheels[2][2].temperature;
	tyre_temperature[9] = Wheels[2][3].temperature;
	
	tyre_timeout_status[0] = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1] = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2] = (wheelupdatedtimer[1][0]>=30);
	tyre_timeout_status[3] = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[4] = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[5] = (wheelupdatedtimer[1][3]>=30);
	tyre_timeout_status[6] = (wheelupdatedtimer[2][0]>=30);
	tyre_timeout_status[7] = (wheelupdatedtimer[2][1]>=30);
	tyre_timeout_status[8] = (wheelupdatedtimer[2][2]>=30);
	tyre_timeout_status[9] = (wheelupdatedtimer[2][3]>=30);
	
	tyre_pressure_status[0] = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1] = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2] = (TyreLocation_pressureh[1].bit0||TyreLocation_pressurel[1].bit0||TyreLocation_fastlostgas[1].bit0);
	tyre_pressure_status[3] = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[4] = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[5] = (TyreLocation_pressureh[1].bit3||TyreLocation_pressurel[1].bit3||TyreLocation_fastlostgas[1].bit3);
	tyre_pressure_status[6] = (TyreLocation_pressureh[2].bit0||TyreLocation_pressurel[2].bit0||TyreLocation_fastlostgas[2].bit0);
	tyre_pressure_status[7] = (TyreLocation_pressureh[2].bit1||TyreLocation_pressurel[2].bit1||TyreLocation_fastlostgas[2].bit1);
	tyre_pressure_status[8] = (TyreLocation_pressureh[2].bit2||TyreLocation_pressurel[2].bit2||TyreLocation_fastlostgas[2].bit2);
	tyre_pressure_status[9] = (TyreLocation_pressureh[2].bit3||TyreLocation_pressurel[2].bit3||TyreLocation_fastlostgas[2].bit3);
	
	tyre_temperature_status[0] = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1] = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2] = (TyreLocation_temperature[1].bit0);
	tyre_temperature_status[3] = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[4] = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[5] = (TyreLocation_temperature[1].bit3);
	tyre_temperature_status[6] = (TyreLocation_temperature[2].bit0);
	tyre_temperature_status[7] = (TyreLocation_temperature[2].bit1);
	tyre_temperature_status[8] = (TyreLocation_temperature[2].bit2);
	tyre_temperature_status[9] = (TyreLocation_temperature[2].bit3);
	
	start_draw();
    for(uint8_t i = 0; i < B2004_TYRE_NUM; i++)
	{
		if(update_tpmsvalue || old_tyre_status[i] != tyre_status[i])
		{
			old_tyre_status[i] = tyre_status[i];
			
			if(tyre_status[i] != 0)
				loc_Render_Tire(B2004_Tyre_Info_Coord[i][0].Pos_X, B2004_Tyre_Info_Coord[i][0].Pos_Y, RED);
			else
				loc_Render_Tire(B2004_Tyre_Info_Coord[i][0].Pos_X, B2004_Tyre_Info_Coord[i][0].Pos_Y, GRAY);
		}
		
		if(update_tpmsvalue)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
			zk_printZH(B2004_Tyre_Info_Coord[i][1].Pos_X, B2004_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
		}
		
		if(update_tpmsvalue || old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2004_Tyre_Info_Coord[i][1].Pos_X - 2 - 40, B2004_Tyre_Info_Coord[i][1].Pos_Y, 40, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2004_Tyre_Info_Coord[i][1].Pos_X - 2, B2004_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*0.2, 1, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2004_Tyre_Info_Coord[i][1].Pos_X - 2, B2004_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"--"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(update_tpmsvalue || old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2004_Tyre_Info_Coord[i][1].Pos_X + 10, B2004_Tyre_Info_Coord[i][1].Pos_Y, 33, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2004_Tyre_Info_Coord[i][1].Pos_X + 10, B2004_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2004_Tyre_Info_Coord[i][1].Pos_X + 10, B2004_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
	}
	
	end_draw();
}

/* 4Öá£¬10ÂÖÌ¥  eol_drivetype = 3 */
static const COORD_t B2006_Tyre_Info_Coord[B2006_TYRE_NUM][2] = 
{
	{{370,      275},    {318, 273}},
	{{422,      275},    {478, 273}},
	
	{{370,      275+30}, {318, 273+30}},
	{{422,      275+30}, {478, 273+30}},
	
	{{370,      365-30}, {318, 365-30}},
	{{422,      365-30}, {478, 365-30}},
	
	{{370,      365},    {222, 365}},
	{{378,      365},    {318, 365}},
	{{413,      365},    {478, 365}},
	{{422,      365},    {573, 365}}
	
};
void tpms_b2006_update(void)
{
	uint8_t uniStr[64];
	uint32_t colorf;
	
	uint8_t  tyre_status[B2006_TYRE_NUM];
	
	uint16_t tyre_pressure[B2006_TYRE_NUM];
	int16_t  tyre_temperature[B2006_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2006_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2006_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2006_TYRE_NUM];
	
	tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
	tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
	tyre_status[2] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
	tyre_status[3] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
	tyre_status[4] = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
	tyre_status[5] = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
	tyre_status[6] = TyreLocation_pressureh[3].bit0 + TyreLocation_pressurel[3].bit0 + TyreLocation_fastlostgas[3].bit0 + TyreLocation_temperature[3].bit0;
	tyre_status[7] = TyreLocation_pressureh[3].bit1 + TyreLocation_pressurel[3].bit1 + TyreLocation_fastlostgas[3].bit1 + TyreLocation_temperature[3].bit1;
	tyre_status[8] = TyreLocation_pressureh[3].bit2 + TyreLocation_pressurel[3].bit2 + TyreLocation_fastlostgas[3].bit2 + TyreLocation_temperature[3].bit2;
	tyre_status[9] = TyreLocation_pressureh[3].bit3 + TyreLocation_pressurel[3].bit3 + TyreLocation_fastlostgas[3].bit3 + TyreLocation_temperature[3].bit3;
	
	tyre_pressure[0] = Wheels[0][0].pressure;
	tyre_pressure[1] = Wheels[0][1].pressure;
	tyre_pressure[2] = Wheels[1][1].pressure;
	tyre_pressure[3] = Wheels[1][2].pressure;
	tyre_pressure[4] = Wheels[2][1].pressure;
	tyre_pressure[5] = Wheels[2][2].pressure;
	tyre_pressure[6] = Wheels[3][0].pressure;
	tyre_pressure[7] = Wheels[3][1].pressure;
	tyre_pressure[8] = Wheels[3][2].pressure;
	tyre_pressure[9] = Wheels[3][3].pressure;
	
	tyre_temperature[0] = Wheels[0][0].temperature;
	tyre_temperature[1] = Wheels[0][1].temperature;
	tyre_temperature[2] = Wheels[1][1].temperature;
	tyre_temperature[3] = Wheels[1][2].temperature;
	tyre_temperature[4] = Wheels[2][1].temperature;
	tyre_temperature[5] = Wheels[2][2].temperature;
	tyre_temperature[6] = Wheels[3][0].temperature;
	tyre_temperature[7] = Wheels[3][1].temperature;
	tyre_temperature[8] = Wheels[3][2].temperature;
	tyre_temperature[9] = Wheels[3][3].temperature;
	
	tyre_timeout_status[0] = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1] = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2] = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[3] = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[4] = (wheelupdatedtimer[2][1]>=30);
	tyre_timeout_status[5] = (wheelupdatedtimer[2][2]>=30);
	tyre_timeout_status[6] = (wheelupdatedtimer[3][0]>=30);
	tyre_timeout_status[7] = (wheelupdatedtimer[3][1]>=30);
	tyre_timeout_status[8] = (wheelupdatedtimer[3][2]>=30);
	tyre_timeout_status[9] = (wheelupdatedtimer[3][3]>=30);
	
	tyre_pressure_status[0] = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1] = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2] = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[3] = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[4] = (TyreLocation_pressureh[2].bit1||TyreLocation_pressurel[2].bit1||TyreLocation_fastlostgas[2].bit1);
	tyre_pressure_status[5] = (TyreLocation_pressureh[2].bit2||TyreLocation_pressurel[2].bit2||TyreLocation_fastlostgas[2].bit2);
	tyre_pressure_status[6] = (TyreLocation_pressureh[3].bit0||TyreLocation_pressurel[3].bit0||TyreLocation_fastlostgas[3].bit0);
	tyre_pressure_status[7] = (TyreLocation_pressureh[3].bit1||TyreLocation_pressurel[3].bit1||TyreLocation_fastlostgas[3].bit1);
	tyre_pressure_status[8] = (TyreLocation_pressureh[3].bit2||TyreLocation_pressurel[3].bit2||TyreLocation_fastlostgas[3].bit2);
	tyre_pressure_status[9] = (TyreLocation_pressureh[3].bit3||TyreLocation_pressurel[3].bit3||TyreLocation_fastlostgas[3].bit3);
	
	tyre_temperature_status[0] = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1] = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2] = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[3] = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[4] = (TyreLocation_temperature[2].bit1);
	tyre_temperature_status[5] = (TyreLocation_temperature[2].bit2);
	tyre_temperature_status[6] = (TyreLocation_temperature[3].bit0);
	tyre_temperature_status[7] = (TyreLocation_temperature[3].bit1);
	tyre_temperature_status[8] = (TyreLocation_temperature[3].bit2);
	tyre_temperature_status[9] = (TyreLocation_temperature[3].bit3);
	
	start_draw();
    for(uint8_t i = 0; i < B2006_TYRE_NUM; i++)
	{
		if(update_tpmsvalue || old_tyre_status[i] != tyre_status[i])
		{
			old_tyre_status[i] = tyre_status[i];
			
			if(tyre_status[i] != 0)
				loc_Render_Tire(B2006_Tyre_Info_Coord[i][0].Pos_X, B2006_Tyre_Info_Coord[i][0].Pos_Y, RED);
			else
				loc_Render_Tire(B2006_Tyre_Info_Coord[i][0].Pos_X, B2006_Tyre_Info_Coord[i][0].Pos_Y, GRAY);
		}
		
		if(update_tpmsvalue)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
			zk_printZH(B2006_Tyre_Info_Coord[i][1].Pos_X, B2006_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
		}
		
		if(update_tpmsvalue || old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2006_Tyre_Info_Coord[i][1].Pos_X - 2 - 40, B2006_Tyre_Info_Coord[i][1].Pos_Y, 40, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2006_Tyre_Info_Coord[i][1].Pos_X - 2, B2006_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*0.2, 1, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2006_Tyre_Info_Coord[i][1].Pos_X - 2, B2006_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"--"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(update_tpmsvalue || old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2006_Tyre_Info_Coord[i][1].Pos_X + 10, B2006_Tyre_Info_Coord[i][1].Pos_Y, 33, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2006_Tyre_Info_Coord[i][1].Pos_X + 10, B2006_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2006_Tyre_Info_Coord[i][1].Pos_X + 10, B2006_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
	}
	
	end_draw();
}

/* 4Öá£¬10ÂÖÌ¥  eol_drivetype = 4 */
static const COORD_t B2012_Tyre_Info_Coord[B2012_TYRE_NUM][2] = 
{
	{{370,      275},    {318, 273}},
	{{422,      275},    {478, 273}},
	
	{{370,      275+30}, {318, 273+30}},
	{{422,      275+30}, {478, 273+30}},
	
	{{370,      365-30}, {222, 365-30}},
	{{378,      365-30}, {318, 365-30}},
	{{413,      365-30}, {478, 365-30}},
	{{422,      365-30}, {573, 365-30}},
	
	{{370,      365},    {318, 365}},
	{{422,      365},    {478, 365}}
	
};
void tpms_b2012_update(void)
{
	uint8_t uniStr[64];
	uint32_t colorf;
	
	uint8_t  tyre_status[B2012_TYRE_NUM];
	
	uint16_t tyre_pressure[B2012_TYRE_NUM];
	int16_t  tyre_temperature[B2012_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2012_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2012_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2012_TYRE_NUM];
	
	tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
	tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
	tyre_status[2] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
	tyre_status[3] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
	tyre_status[4] = TyreLocation_pressureh[2].bit0 + TyreLocation_pressurel[2].bit0 + TyreLocation_fastlostgas[2].bit0 + TyreLocation_temperature[2].bit0;
	tyre_status[5] = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
	tyre_status[6] = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
	tyre_status[7] = TyreLocation_pressureh[2].bit3 + TyreLocation_pressurel[2].bit3 + TyreLocation_fastlostgas[2].bit3 + TyreLocation_temperature[2].bit3;
	tyre_status[8] = TyreLocation_pressureh[3].bit1 + TyreLocation_pressurel[3].bit1 + TyreLocation_fastlostgas[3].bit1 + TyreLocation_temperature[3].bit1;
	tyre_status[9] = TyreLocation_pressureh[3].bit2 + TyreLocation_pressurel[3].bit2 + TyreLocation_fastlostgas[3].bit2 + TyreLocation_temperature[3].bit2;
	
	tyre_pressure[0] = Wheels[0][0].pressure;
	tyre_pressure[1] = Wheels[0][1].pressure;
	tyre_pressure[2] = Wheels[1][1].pressure;
	tyre_pressure[3] = Wheels[1][2].pressure;
	tyre_pressure[4] = Wheels[2][0].pressure;
	tyre_pressure[5] = Wheels[2][1].pressure;
	tyre_pressure[6] = Wheels[2][2].pressure;
	tyre_pressure[7] = Wheels[2][3].pressure;
	tyre_pressure[8] = Wheels[3][1].pressure;
	tyre_pressure[9] = Wheels[3][2].pressure;
	
	tyre_temperature[0] = Wheels[0][0].temperature;
	tyre_temperature[1] = Wheels[0][1].temperature;
	tyre_temperature[2] = Wheels[1][1].temperature;
	tyre_temperature[3] = Wheels[1][2].temperature;
	tyre_temperature[4] = Wheels[2][0].temperature;
	tyre_temperature[5] = Wheels[2][1].temperature;
	tyre_temperature[6] = Wheels[2][2].temperature;
	tyre_temperature[7] = Wheels[2][3].temperature;
	tyre_temperature[8] = Wheels[3][1].temperature;
	tyre_temperature[9] = Wheels[3][2].temperature;
	
	tyre_timeout_status[0] = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1] = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2] = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[3] = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[4] = (wheelupdatedtimer[2][0]>=30);
	tyre_timeout_status[5] = (wheelupdatedtimer[2][1]>=30);
	tyre_timeout_status[6] = (wheelupdatedtimer[2][2]>=30);
	tyre_timeout_status[7] = (wheelupdatedtimer[2][3]>=30);
	tyre_timeout_status[8] = (wheelupdatedtimer[3][1]>=30);
	tyre_timeout_status[9] = (wheelupdatedtimer[3][2]>=30);
	
	tyre_pressure_status[0] = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1] = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2] = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[3] = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[4] = (TyreLocation_pressureh[2].bit0||TyreLocation_pressurel[2].bit0||TyreLocation_fastlostgas[2].bit0);
	tyre_pressure_status[5] = (TyreLocation_pressureh[2].bit1||TyreLocation_pressurel[2].bit1||TyreLocation_fastlostgas[2].bit1);
	tyre_pressure_status[6] = (TyreLocation_pressureh[2].bit2||TyreLocation_pressurel[2].bit2||TyreLocation_fastlostgas[2].bit2);
	tyre_pressure_status[7] = (TyreLocation_pressureh[2].bit3||TyreLocation_pressurel[2].bit3||TyreLocation_fastlostgas[2].bit3);
	tyre_pressure_status[8] = (TyreLocation_pressureh[3].bit1||TyreLocation_pressurel[3].bit1||TyreLocation_fastlostgas[3].bit1);
	tyre_pressure_status[9] = (TyreLocation_pressureh[3].bit2||TyreLocation_pressurel[3].bit2||TyreLocation_fastlostgas[3].bit2);
	
	tyre_temperature_status[0] = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1] = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2] = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[3] = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[4] = (TyreLocation_temperature[2].bit0);
	tyre_temperature_status[5] = (TyreLocation_temperature[2].bit1);
	tyre_temperature_status[6] = (TyreLocation_temperature[2].bit2);
	tyre_temperature_status[7] = (TyreLocation_temperature[2].bit3);
	tyre_temperature_status[8] = (TyreLocation_temperature[3].bit1);
	tyre_temperature_status[9] = (TyreLocation_temperature[3].bit2);
	
	start_draw();
	for(uint8_t i = 0; i < B2012_TYRE_NUM; i++)
	{
		if(update_tpmsvalue || old_tyre_status[i] != tyre_status[i])
		{
			old_tyre_status[i] = tyre_status[i];
			
			if(tyre_status[i] != 0)
				loc_Render_Tire(B2012_Tyre_Info_Coord[i][0].Pos_X, B2012_Tyre_Info_Coord[i][0].Pos_Y, RED);
			else
				loc_Render_Tire(B2012_Tyre_Info_Coord[i][0].Pos_X, B2012_Tyre_Info_Coord[i][0].Pos_Y, GRAY);
		}
		
		if(update_tpmsvalue)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
			zk_printZH(B2012_Tyre_Info_Coord[i][1].Pos_X, B2012_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
		}
		
		if(update_tpmsvalue || old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2012_Tyre_Info_Coord[i][1].Pos_X - 2 - 40, B2012_Tyre_Info_Coord[i][1].Pos_Y, 40, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2012_Tyre_Info_Coord[i][1].Pos_X - 2, B2012_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*0.2, 1, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2012_Tyre_Info_Coord[i][1].Pos_X - 2, B2012_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"--"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(update_tpmsvalue || old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2012_Tyre_Info_Coord[i][1].Pos_X + 10, B2012_Tyre_Info_Coord[i][1].Pos_Y, 33, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2012_Tyre_Info_Coord[i][1].Pos_X + 10, B2012_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2012_Tyre_Info_Coord[i][1].Pos_X + 10, B2012_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
	}
	
	end_draw();
}

/* 4Öá£¬12ÂÖÌ¥  eol_drivetype = 5 */
static const COORD_t B2007_Tyre_Info_Coord[B2007_TYRE_NUM][2] = 
{
	{{370,      275},    {318, 273}},
	{{422,      275},    {478, 273}},
	
	{{370,      275+30}, {318, 273+30}},
	{{422,      275+30}, {478, 273+30}},
	
	{{370,      365-30}, {222, 365-30}},
	{{378,      365-30}, {318, 365-30}},
	{{413,      365-30}, {478, 365-30}},
	{{422,      365-30}, {573, 365-30}},
	
	{{370,      365},    {222, 365}},
	{{378,      365},    {318, 365}},
	{{413,      365},    {478, 365}},
	{{422,      365},    {573, 365}}
	
};
void tpms_b2007_update(void)
{
	uint8_t uniStr[64];
	uint32_t colorf;
	
	uint8_t  tyre_status[B2007_TYRE_NUM];
	
	uint16_t tyre_pressure[B2007_TYRE_NUM];
	int16_t  tyre_temperature[B2007_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2007_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2007_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2007_TYRE_NUM];
	
	tyre_status[0]  = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
	tyre_status[1]  = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
	tyre_status[2]  = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
	tyre_status[3]  = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
	tyre_status[4]  = TyreLocation_pressureh[2].bit0 + TyreLocation_pressurel[2].bit0 + TyreLocation_fastlostgas[2].bit0 + TyreLocation_temperature[2].bit0;
	tyre_status[5]  = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
	tyre_status[6]  = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
	tyre_status[7]  = TyreLocation_pressureh[2].bit3 + TyreLocation_pressurel[2].bit3 + TyreLocation_fastlostgas[2].bit3 + TyreLocation_temperature[2].bit3;
	tyre_status[8]  = TyreLocation_pressureh[3].bit0 + TyreLocation_pressurel[3].bit0 + TyreLocation_fastlostgas[3].bit0 + TyreLocation_temperature[3].bit0;
	tyre_status[9]  = TyreLocation_pressureh[3].bit1 + TyreLocation_pressurel[3].bit1 + TyreLocation_fastlostgas[3].bit1 + TyreLocation_temperature[3].bit1;
	tyre_status[10] = TyreLocation_pressureh[3].bit2 + TyreLocation_pressurel[3].bit2 + TyreLocation_fastlostgas[3].bit2 + TyreLocation_temperature[3].bit2;
	tyre_status[11] = TyreLocation_pressureh[3].bit3 + TyreLocation_pressurel[3].bit3 + TyreLocation_fastlostgas[3].bit3 + TyreLocation_temperature[3].bit3;
	
	tyre_pressure[0]  = Wheels[0][0].pressure;
	tyre_pressure[1]  = Wheels[0][1].pressure;
	tyre_pressure[2]  = Wheels[1][1].pressure;
	tyre_pressure[3]  = Wheels[1][2].pressure;
	tyre_pressure[4]  = Wheels[2][0].pressure;
	tyre_pressure[5]  = Wheels[2][1].pressure;
	tyre_pressure[6]  = Wheels[2][2].pressure;
	tyre_pressure[7]  = Wheels[2][3].pressure;
	tyre_pressure[8]  = Wheels[3][0].pressure;
	tyre_pressure[9]  = Wheels[3][1].pressure;
	tyre_pressure[10] = Wheels[3][2].pressure;
	tyre_pressure[11] = Wheels[3][3].pressure;
	
	tyre_temperature[0]  = Wheels[0][0].temperature;
	tyre_temperature[1]  = Wheels[0][1].temperature;
	tyre_temperature[2]  = Wheels[1][1].temperature;
	tyre_temperature[3]  = Wheels[1][2].temperature;
	tyre_temperature[4]  = Wheels[2][0].temperature;
	tyre_temperature[5]  = Wheels[2][1].temperature;
	tyre_temperature[6]  = Wheels[2][2].temperature;
	tyre_temperature[7]  = Wheels[2][3].temperature;
	tyre_temperature[8]  = Wheels[3][0].temperature;
	tyre_temperature[9]  = Wheels[3][1].temperature;
	tyre_temperature[10] = Wheels[3][2].temperature;
	tyre_temperature[11] = Wheels[3][3].temperature;
	
	tyre_timeout_status[0]  = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1]  = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2]  = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[3]  = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[4]  = (wheelupdatedtimer[2][0]>=30);
	tyre_timeout_status[5]  = (wheelupdatedtimer[2][1]>=30);
	tyre_timeout_status[6]  = (wheelupdatedtimer[2][2]>=30);
	tyre_timeout_status[7]  = (wheelupdatedtimer[2][3]>=30);
	tyre_timeout_status[8]  = (wheelupdatedtimer[3][0]>=30);
	tyre_timeout_status[9]  = (wheelupdatedtimer[3][1]>=30);
	tyre_timeout_status[10] = (wheelupdatedtimer[3][2]>=30);
	tyre_timeout_status[11] = (wheelupdatedtimer[3][3]>=30);
	
	tyre_pressure_status[0]  = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1]  = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2]  = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[3]  = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[4]  = (TyreLocation_pressureh[2].bit0||TyreLocation_pressurel[2].bit0||TyreLocation_fastlostgas[2].bit0);
	tyre_pressure_status[5]  = (TyreLocation_pressureh[2].bit1||TyreLocation_pressurel[2].bit1||TyreLocation_fastlostgas[2].bit1);
	tyre_pressure_status[6]  = (TyreLocation_pressureh[2].bit2||TyreLocation_pressurel[2].bit2||TyreLocation_fastlostgas[2].bit2);
	tyre_pressure_status[7]  = (TyreLocation_pressureh[2].bit3||TyreLocation_pressurel[2].bit3||TyreLocation_fastlostgas[2].bit3);
	tyre_pressure_status[8]  = (TyreLocation_pressureh[3].bit0||TyreLocation_pressurel[3].bit0||TyreLocation_fastlostgas[3].bit0);
	tyre_pressure_status[9]  = (TyreLocation_pressureh[3].bit1||TyreLocation_pressurel[3].bit1||TyreLocation_fastlostgas[3].bit1);
	tyre_pressure_status[10] = (TyreLocation_pressureh[3].bit2||TyreLocation_pressurel[3].bit2||TyreLocation_fastlostgas[3].bit2);
	tyre_pressure_status[11] = (TyreLocation_pressureh[3].bit3||TyreLocation_pressurel[3].bit3||TyreLocation_fastlostgas[3].bit3);
	
	tyre_temperature_status[0]  = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1]  = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2]  = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[3]  = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[4]  = (TyreLocation_temperature[2].bit0);
	tyre_temperature_status[5]  = (TyreLocation_temperature[2].bit1);
	tyre_temperature_status[6]  = (TyreLocation_temperature[2].bit2);
	tyre_temperature_status[7]  = (TyreLocation_temperature[2].bit3);
	tyre_temperature_status[8]  = (TyreLocation_temperature[3].bit0);
	tyre_temperature_status[9]  = (TyreLocation_temperature[3].bit1);
	tyre_temperature_status[10] = (TyreLocation_temperature[3].bit2);
	tyre_temperature_status[11] = (TyreLocation_temperature[3].bit3);
	
	start_draw();
	for(uint8_t i = 0; i < B2007_TYRE_NUM; i++)
	{
		if(update_tpmsvalue || old_tyre_status[i] != tyre_status[i])
		{
			old_tyre_status[i] = tyre_status[i];
			
			if(tyre_status[i] != 0)
				loc_Render_Tire(B2007_Tyre_Info_Coord[i][0].Pos_X, B2007_Tyre_Info_Coord[i][0].Pos_Y, RED);
			else
				loc_Render_Tire(B2007_Tyre_Info_Coord[i][0].Pos_X, B2007_Tyre_Info_Coord[i][0].Pos_Y, GRAY);
		}
		
		if(update_tpmsvalue)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
			zk_printZH(B2007_Tyre_Info_Coord[i][1].Pos_X, B2007_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
		}
		
		if(update_tpmsvalue || old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2007_Tyre_Info_Coord[i][1].Pos_X - 2 - 40, B2007_Tyre_Info_Coord[i][1].Pos_Y, 40, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2007_Tyre_Info_Coord[i][1].Pos_X - 2, B2007_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*0.2, 1, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2007_Tyre_Info_Coord[i][1].Pos_X - 2, B2007_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"--"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(update_tpmsvalue || old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2007_Tyre_Info_Coord[i][1].Pos_X + 10, B2007_Tyre_Info_Coord[i][1].Pos_Y, 33, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2007_Tyre_Info_Coord[i][1].Pos_X + 10, B2007_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2007_Tyre_Info_Coord[i][1].Pos_X + 10, B2007_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
	}
	
	end_draw();
	
}

/* 3Öá£¬6ÂÖÌ¥  eol_drivetype = 6 */
static const COORD_t B2013_Tyre_Info_Coord[B2013_TYRE_NUM][2] = 
{
	{{370,      275},    {318, 273}},
	{{422,      275},    {478, 273}},
	
	{{370,      365-30}, {318, 365-30}},
	{{422,      365-30}, {478, 365-30}},
	
	{{370,      365},    {318, 365}},
	{{422,      365},    {478, 365}}
	
};
void tpms_b2013_update(void)
{
	uint8_t uniStr[64];
	uint32_t colorf;
	
	uint8_t  tyre_status[B2013_TYRE_NUM];
	
	uint16_t tyre_pressure[B2013_TYRE_NUM];
	int16_t  tyre_temperature[B2013_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2013_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2013_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2013_TYRE_NUM];
	
	tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
	tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
	tyre_status[2] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
	tyre_status[3] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
	tyre_status[4] = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
	tyre_status[5] = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
	
	tyre_pressure[0] = Wheels[0][0].pressure;
	tyre_pressure[1] = Wheels[0][1].pressure;
	tyre_pressure[2] = Wheels[1][1].pressure;
	tyre_pressure[3] = Wheels[1][2].pressure;
	tyre_pressure[4] = Wheels[2][1].pressure;
	tyre_pressure[5] = Wheels[2][2].pressure;
	
	tyre_temperature[0] = Wheels[0][0].temperature;
	tyre_temperature[1] = Wheels[0][1].temperature;
	tyre_temperature[2] = Wheels[1][1].temperature;
	tyre_temperature[3] = Wheels[1][2].temperature;
	tyre_temperature[4] = Wheels[2][1].temperature;
	tyre_temperature[5] = Wheels[2][2].temperature;
	
	tyre_timeout_status[0] = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1] = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2] = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[3] = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[4] = (wheelupdatedtimer[2][1]>=30);
	tyre_timeout_status[5] = (wheelupdatedtimer[2][2]>=30);
	
	tyre_pressure_status[0] = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1] = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2] = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[3] = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[4] = (TyreLocation_pressureh[2].bit1||TyreLocation_pressurel[2].bit1||TyreLocation_fastlostgas[2].bit1);
	tyre_pressure_status[5] = (TyreLocation_pressureh[2].bit2||TyreLocation_pressurel[2].bit2||TyreLocation_fastlostgas[2].bit2);
	
	tyre_temperature_status[0] = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1] = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2] = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[3] = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[4] = (TyreLocation_temperature[2].bit1);
	tyre_temperature_status[5] = (TyreLocation_temperature[2].bit2);
	
	start_draw();
    for(uint8_t i = 0; i < B2013_TYRE_NUM; i++)
	{
		if(update_tpmsvalue || old_tyre_status[i] != tyre_status[i])
		{
			old_tyre_status[i] = tyre_status[i];
			
			if(tyre_status[i] != 0)
				loc_Render_Tire(B2013_Tyre_Info_Coord[i][0].Pos_X, B2013_Tyre_Info_Coord[i][0].Pos_Y, RED);
			else
				loc_Render_Tire(B2013_Tyre_Info_Coord[i][0].Pos_X, B2013_Tyre_Info_Coord[i][0].Pos_Y, GRAY);
		}
		
		if(update_tpmsvalue)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
			zk_printZH(B2013_Tyre_Info_Coord[i][1].Pos_X, B2013_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
		}
		
		if(update_tpmsvalue || old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2013_Tyre_Info_Coord[i][1].Pos_X - 2 - 40, B2013_Tyre_Info_Coord[i][1].Pos_Y, 40, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2013_Tyre_Info_Coord[i][1].Pos_X - 2, B2013_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*0.2, 1, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2013_Tyre_Info_Coord[i][1].Pos_X - 2, B2013_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"--"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(update_tpmsvalue || old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2013_Tyre_Info_Coord[i][1].Pos_X + 10, B2013_Tyre_Info_Coord[i][1].Pos_Y, 33, 20, TOUMING);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, colorf, 0);
				zk_printNum(B2013_Tyre_Info_Coord[i][1].Pos_X + 10, B2013_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, 0xffffffff, 0);
				zk_printZH1(B2013_Tyre_Info_Coord[i][1].Pos_X + 10, B2013_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
	}
	
	end_draw();
}


/*****Gloable FUNCTIONS DECLEAR*****/
void tpms_mainframe(uint8_t displayswitch)
{
    uint8_t uniStr[64];
    static uint8_t displayswitch_old = 6;
    static uint8_t contr = 10;
	
    if(10U == contr)
	{
		update_tpmsvalue = 0x01;
		contr = 0;
	}
	else
	{
		contr++;
	}
	
	start_draw();
	if((displayswitch_old != displayswitch)||update_tpmsvalue)
	{
		displayswitch_old = displayswitch;
		zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"bar/¡æ", uniStr, sizeof(uniStr));
		loc_ClearRect(450,240,60,18);
		zk_printZH(450, 240, uniStr);
		
    }
	end_draw();
	
	ClrWdt();
	
	if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
	{
		if(eol_drivetype==0)
		{
			tpms_b2001_update();
		}
		else if(eol_drivetype==1)
		{
			tpms_b2003_update();
		}
		else if(eol_drivetype==2)
		{
			tpms_b2004_update();
		}
		else if(eol_drivetype==3)
		{
			tpms_b2006_update();
		}
		else if(eol_drivetype==4)
		{
			tpms_b2012_update();
		}
		else if(eol_drivetype==5)
		{
			tpms_b2007_update();
		}
		else if(eol_drivetype==6)
		{
			tpms_b2013_update();
		}
		else ;
	}
	
	update_tpmsvalue = 0;
	
	
}

/*-----------------------------------------------End of file----------------------------------------------------------*/


