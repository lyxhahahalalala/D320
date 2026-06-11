#include <stdio.h>
#include "SYSTEM.h"
#include "zk.h"
#include "init.h"

#include "r_tick_api.h"
#include "string.h"
#include "motor_sm.h"
#include "sg.h"
#include "uds_support.h"

#include "key.h"
#include "pwm.h"
#include "ptmr.h"
#include "adc.h"
#include "rtc.h"

#include "app_can.h"
#include "app_trip.h"
#include "app_telltale.h"
#include "app_temperature.h"
#include "app_time.h"
#include "app_texturewarning.h"
#include "app_data_center.h"
#include "app_dm1.h"



/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * ENUM
 */

enum menu{
/* 0*/Main = 0,
/* 1*/Menu_VehicleInfo,
/* 2*/Menu_TripInfo,
/* 3*/Menu_PSNInfo,
/* 4*/Menu_IC,
/* 5*/Kvalue,
/* 6*/BatVol,
/* 7*/BrakeInfo,
/* 8*/TPMS,
/* 9*/FaultInfo,
/*10*/Maintain,
/*11*/VCUVersion,
/*12*/Supplier,
/*13*/Trip,
/*14*/PowerCons,
/*15*/ZhouHe,
/*16*/MotorVol,
/*17*/WorkingTime,
/*18*/AveSpeed,
/*19*/DateTime,
/*20*/Language,
/*21*/Unit,
/*22*/Dispeffect,
/*23*/DispInfo,
/*24*/TimeDisp,	
/*25*/Needle,
/*26*/Lamp,
/*27*/Buzzer,
/*28*/LCD,
/*29*/Sensor,
/*30*/Charge,
/*31*/FCUWrkTm,
/*32*/HMSMaxPress,

}frame_ID;

/*******************************************************************************
 * TYPEDEFS
 */

typedef struct{
    // 条目显示文字
    const uint8_t *itemStr[2];
    // 数值更新函数
    void (*updateHandle)(int32_t x, int32_t y, int32_t arg);
    int32_t arg;              // 参数
}msgItem_t;

typedef struct{
    uint32_t columnCnt;         // 列?
    uint32_t rowCnt;            // 行数
    uint32_t columnWidth;       // 列宽
    uint32_t rowHeight;         // 行高
    const msgItem_t *msg;       // 消息
    uint32_t msgCnt;            // 消息条数
    const GUI_FONT *font;       // 字体
    r_drw2d_Color_t color;      // 颜色
}tblText_t;


/**
 * \defgroup GROUP_LOCAL_FUNCTIONS APP:VIEW:LOCAL_FUNCTIONS
 */


/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * gloable varias
 */
unsigned char Index = 0;
unsigned char t = 0;
unsigned char t_add = 0;
unsigned char frmID = 0;
unsigned char f_index = 0;
unsigned char menuid = 0;
unsigned int  witdh = 0;
unsigned char index_page = 1;
uint8_t faultcodeupdate = 0x00;


extern void disable_choose(void);


/*******************************************************************************
 * LOCAL FUNCTIONS DECLEAR
 */

/**
 * \brief main view check value udpate
 * \return main view value changed or not
 * \retval true - changed
 * \retval false - not changed
 */
void vw_drawTable(int32_t x, int32_t y, uint8_t c, uint8_t r, int32_t w, int32_t h, r_drw2d_Color_t color)
{
    r_drw2d_Point_t points[2];
    r_drw2d_LineStyle_t Style = {
        R_DRW2D_LINEJOIN_NONE,
        R_DRW2D_LINECAP_BUTT,
        R_DRW2D_2X(1),
        1,
        0
    };
    // 绘制表格
    R_DRW2D_CtxIdentity(loc_Drw2dDev);
    R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
    R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
    R_DRW2D_CtxFgColor(loc_Drw2dDev, color);
    R_DRW2D_CtxLineStyle(loc_Drw2dDev, &Style);

    // 画竖线
    for (uint8_t i = 0; i <= c; i ++)
    {
        points[0].X = R_DRW2D_2X(x + w*i/c);
        points[0].Y = R_DRW2D_2X(y);
        points[1].X = points[0].X;
        points[1].Y = R_DRW2D_2X(y + h);
        R_DRW2D_DrawLines(loc_Drw2dDev, points, 2);
    }

    // 画横线
    for (uint8_t i = 0; i <= r; i ++)
    {
        points[0].X = R_DRW2D_2X(x);
        points[0].Y = R_DRW2D_2X(y + h*i/r);
        points[1].X = R_DRW2D_2X(x+w);
        points[1].Y = points[0].Y;
        R_DRW2D_DrawLines(loc_Drw2dDev, points, 2);
    }
    R_DRW2D_CtxIdentity(loc_Drw2dDev);
    R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);

}

void vw_paintAbnormalValue(int32_t x, int32_t y, uint8_t code, const GUI_FONT *fnt)
{
	const uint8_t *AbnormalText[3][2] = 
	{
		{(uint8_t *)"--",   (uint8_t *)"--"},
		{(uint8_t *)"异常", (uint8_t *)"abnormal"},
		{(uint8_t *)"无效", (uint8_t *)"invalid"}
		
	};
	
    const uint8_t *pStr = NULL;
    r_drw2d_Color_t color;
    // uint8_t uniStr[64];

    if (fnt == NULL)
    {
        return;
    }

    switch(code)
    {
        case VW_DATA_ST_NONE:
            pStr = AbnormalText[0][eol_language];
            color = VM_COLOR_WHITE;
			break;
        case VW_DATA_ST_FAULT:
            pStr = AbnormalText[1][eol_language];
            color = VM_COLOR_RED;
			break;
        case VW_DATA_ST_INVALID:
            pStr = AbnormalText[2][eol_language];
            color = VM_COLOR_YELLOW;
			break;
        default:
			break;
    }

    if (NULL != pStr)
    {
        zk_init(loc_Drw2dDev, fnt, color, 0);
        zk_printZKAlign(x, y, ZK_ALIGN_RIGHT, zk_oem2uniStr(pStr));
    }
}

static void frmpage(void (*frame)(unsigned char index, unsigned char page), void (*update)(unsigned char index), unsigned char page)
{

    if (f_index == 0)  //只更新一次
	{
        f_index = 1;
        frame(index_page, page);
    }
	else if (f_index == 1)
	{
        f_index = 1;
        update(index_page);
    }
	else
	{
        f_index = 0;
    }
    if(frmID != Charge)//充电界面不响应按键切换界面
	{
		if(page > 1)
		{
			if (key_up)
			{
				key_up = 0;
				f_index = 0;
				index_page--;
				if (index_page == 0)
				{
					index_page = page;
				}
			}
			if (key_AV)
			{
				key_AV = 0;
				f_index = 0;
				index_page++;
				if (index_page > (page))
				{
					index_page = 1;
				}
			}
		}

        if (key_set && (LCD_test == 0) && (BUZZ_test == 0) && (LED_test == 0) && (Needle_Test == 0)){//返回菜单界面
            key_set = 0;
            frmID = Menu_VehicleInfo;
            f_index = 0;
            Index = 0;
            index_page = 1;
        }

		if (key_clear && (LCD_test == 0) && (BUZZ_test == 0) && (LED_test == 0) && (Needle_Test == 0)) //返回主界面
		{
			key_clear = 0;
			
			if((frmID == TPMS) && (study == 1 || learnstatus == 1))
			{
				study = 0;
				learnstatus = 0;
				BCAN_Send_TPMSLearningEnd();
			}
			
			frmID = Main;
			f_index = 0;
			Index = 0;
			index_page = 1;
			
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			alarm_update_flag = 0;
			return_from_menu = 1;
			
			study = 0;
			learnstatus = 0;
			
		}
        if (key_long_set)
		{
            key_long_set = 0;
		}
    }
    else;
	
	static uint8_t  old_drivetype = 0xff;
	if(frmID == TPMS && old_drivetype != eol_drivetype)
	{
		old_drivetype = eol_drivetype;
		key_up = 0;
		key_AV = 0;
		key_set = 0;
		key_clear = 0;
		key_long_set = 0;
		
		f_index = 0;
	}
}


typedef struct 
{
    uint8_t  sn;      //序号
    uint8_t *Text[2]; //显示文字
    int32_t  Pos_X;   //显示位置(X)
    int32_t  Pos_Y;   //显示位置(Y)
    uint8_t *Unit;    //显示单位
} TEXT_LIST_t;
#define MAJ_MENU_ITEM_SUM   (sizeof(MajMenu)/sizeof(TEXT_LIST_t))
TEXT_LIST_t MajMenu[] =
{
	{ 0, {(uint8_t *)"车辆信息",   (uint8_t *)"Vehicle info"  }, 128, 130 + 70 * 0, NULL},
	{ 1, {(uint8_t *)"行程信息",   (uint8_t *)"Travel info"   }, 128, 130 + 70 * 1, NULL},
	{ 2, {(uint8_t *)"个性化设置", (uint8_t *)"Settings"      }, 128, 130 + 70 * 2, NULL},
	{ 3, {(uint8_t *)"仪表自诊断", (uint8_t *)"Self-diagnosis"}, 128, 130 + 70 * 3, NULL},
	
};


void mainFrame(void)
{
    uint8_t uniStr[64];
	
    LCD_Clear();
    start_draw();
    if(PAGE==0)
	{
		if(eol_language == 0)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GRAY, 0);
			
			zk_printZH( 14, 142, zk_oem2uniStr((uint8_t *)"续航里程"));
			zk_printZH(224, 142, zk_oem2uniStr((uint8_t *)"km"));
			
			zk_printZH( 14, 142+30, zk_oem2uniStr((uint8_t *)"剩余功率"));
			zk_printZH(224, 142+30, zk_oem2uniStr((uint8_t *)"kW"));
			
			if(0x2d == eol_fadongjiorkaji)
			{
				zk_printZH(520, 142, zk_oem2uniStr((uint8_t *)"小计氢耗"));
				zk_printZH(740, 142, zk_oem2uniStr((uint8_t *)"kg"));
			}
			else
			{
				zk_printZH(520, 142, zk_oem2uniStr((uint8_t *)"小计电耗"));
				zk_printZH(740, 142, zk_oem2uniStr((uint8_t *)"kWh"));
			}
			
			zk_printZH(520, 142+30, zk_oem2uniStr((uint8_t *)"电机功率"));
			zk_printZH(740, 142+30, zk_oem2uniStr((uint8_t *)"kW"));
			
		}
		else
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh24, VM_COLOR_GRAY, 0);
			
			zk_printZH( 14, 142, zk_oem2uniStr((uint8_t *)"Driving Range"));
			zk_printZH(224, 142, zk_oem2uniStr((uint8_t *)"km"));
			
			zk_printZH( 14, 142+30, zk_oem2uniStr((uint8_t *)"Residual Power"));
			zk_printZH(224, 142+30, zk_oem2uniStr((uint8_t *)"kW"));
			
			if(0x2d == eol_fadongjiorkaji)
			{
				zk_printZH(520, 142, zk_oem2uniStr((uint8_t *)"Subtot H2 Cons."));
				zk_printZH(740, 142, zk_oem2uniStr((uint8_t *)"kg"));
			}
			else
			{
				zk_printZH(520, 142, zk_oem2uniStr((uint8_t *)"Subtot PWR Cons."));
				zk_printZH(740, 142, zk_oem2uniStr((uint8_t *)"kWh"));
			}
			
			zk_printZH(520, 142+30, zk_oem2uniStr((uint8_t *)"Motor Power"));
			zk_printZH(740, 142+30, zk_oem2uniStr((uint8_t *)"kW"));
		}
	}
	else
	{
		if(eol_language == 0)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GRAY, 0);
			if(0x2d == eol_fadongjiorkaji)
			{
				zk_printZH( 14, 142, zk_oem2uniStr((uint8_t *)"平均氢耗"));
				zk_printZH(180, 142, zk_oem2uniStr((uint8_t *)"kg/100km"));
				
				zk_printZH( 14, 142+30, zk_oem2uniStr((uint8_t *)"累计氢耗"));
				zk_printZH(268, 142+30, zk_oem2uniStr((uint8_t *)"kg"));
			}
			else
			{
				zk_printZH( 14, 142, zk_oem2uniStr((uint8_t *)"平均电耗"));
				zk_printZH(180, 142, zk_oem2uniStr((uint8_t *)"kWh/100km"));
				
				zk_printZH( 14, 142+30, zk_oem2uniStr((uint8_t *)"累计电耗"));
				zk_printZH(268, 142+30, zk_oem2uniStr((uint8_t *)"kWh"));
			}
			
			zk_printZH(520, 142, zk_oem2uniStr((uint8_t *)"高压电池电压"));
			zk_printZH(760, 142, zk_oem2uniStr((uint8_t *)"V"));
			
			zk_printZH(520, 142+30, zk_oem2uniStr((uint8_t *)"低压电池电压"));
			zk_printZH(760, 142+30, zk_oem2uniStr((uint8_t *)"V"));
			
			// zk_printZH(520, 142-30, zk_oem2uniStr((uint8_t *)"电池平均温度"));
			// zk_printZH(750, 142-30, zk_oem2uniStr((uint8_t *)"℃"));
			
		}
		else
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh24, VM_COLOR_GRAY, 0);
			
			if(0x2d == eol_fadongjiorkaji)
			{
				zk_printZH( 14, 142, zk_oem2uniStr((uint8_t *)"Avg H2 Cons."));
				zk_printZH(200, 142, zk_oem2uniStr((uint8_t *)"kg/100km"));
				
				zk_printZH( 14, 142+30, zk_oem2uniStr((uint8_t *)"Tot H2 Cons."));
				zk_printZH(268, 142+30, zk_oem2uniStr((uint8_t *)"kg"));
			}
			else
			{
				zk_printZH( 14, 142, zk_oem2uniStr((uint8_t *)"Avg PWR Cons."));
				zk_printZH(200, 142, zk_oem2uniStr((uint8_t *)"kWh/100km"));
				
				zk_printZH( 14, 142+30, zk_oem2uniStr((uint8_t *)"Tot PWR Cons."));
				zk_printZH(268, 142+30, zk_oem2uniStr((uint8_t *)"kWh"));
			}
			
			zk_printZH(520, 142, zk_oem2uniStr((uint8_t *)"Power Batt.Volt."));
			zk_printZH(760, 142, zk_oem2uniStr((uint8_t *)"V"));
			
			zk_printZH(520, 142+30, zk_oem2uniStr((uint8_t *)"Auxil.Batt.Volt."));
			zk_printZH(760, 142+30, zk_oem2uniStr((uint8_t *)"V"));
			
			// zk_printZH(520, 142-30, zk_oem2uniStr((uint8_t *)"Avg Batt.Temp"));
			// zk_printZH(750, 142-30, zk_oem2uniStr((uint8_t *)"℃"));
			
		}
	}
	
	if(0U == e_uint)
	{
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GRAY, 0);
		zk_oem2uni((uint8_t *)"km", uniStr, sizeof(uniStr));
		zk_printZH(175, 438, uniStr);
		zk_printZH(745, 438, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GRAY, 0);
		zk_oem2uni((uint8_t *)"mile", uniStr, sizeof(uniStr));
		zk_printZH(175, 438, uniStr);
		zk_printZH(745, 438, uniStr);
	}
	
	end_draw();
	
}
void Update_AirPressures(void)
{
    uint8_t uniStr[64];
    static uint16_t Qiya_old[3],Qiya[3];
    static uint8_t apstatus[3] = {0};
    static uint8_t apstatusold[3] = {0};
	uint8_t  u8_color[2] = {0, 0};
	uint32_t u32PixelWidth = 0;
	
    Qiya[0] = get_front_airpressure();
	Qiya[1] = get_rear_airpressure();
	Qiya[2] = get_parking_airpressure();
	
    // if(Qiya[0] > 1300) Qiya[0] = 1300;
    // if(Qiya[1] > 1300) Qiya[1] = 1300;
    // if(Qiya[2] > 1300) Qiya[2] = 1300;
	
	if(Qiya[0] > QiyaHighLmt) Qiya[0] = QiyaHighLmt;
    if(Qiya[1] > QiyaHighLmt) Qiya[1] = QiyaHighLmt;
    if(Qiya[2] > QiyaHighLmt) Qiya[2] = QiyaHighLmt;
	
    start_draw();
	
	//前桥气压
    apstatus[0] = ( (Qiya[0] < QiyaLowLmt) || (Qiya[0] > QiyaHighLmt) );
    if(((0x00 == eol_airsensortype) || (0x02 == eol_airsensortype))
	&& (mainview_update || ((Qiya[0]/10) != Qiya_old[0]) || apstatus[0] != apstatusold[0]))
	{
		vw_clearFont31Value(245, 400-1, 6);
		if(Qiya[0] < QiyaLowLmt)
		{
			u8_color[0] = RED;
			u8_color[1] = QiYa_LOW_Alpha;
		}
		else if(Qiya[0] >= QiyaHighLmt)
		{
			u8_color[0] = YELLOW;
			u8_color[1] = QiYa_HIGH_Alpha;
		}
		else
		{
			u8_color[0] = WHITE;
			u8_color[1] = QiYa_Alpha;
		}
		
		loc_Render_FHP(12, 392, u8_color[0], "8");
		
		u32PixelWidth = Qiya[0] * 183 / QiyaHighLmt;
		LCD_DrawRect(58, 407, u32PixelWidth, 12, u8_color[1]);
		
		zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_printNum(150, 390, Qiya[0] /10, 1, ZK_ALIGN_RIGHT);//气压1
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, VM_COLOR_WHITE, 0);//24piex
		zk_oem2uni((uint8_t *)"Bar", uniStr, sizeof(uniStr));
		zk_printZH(160, 400, uniStr);

		Qiya_old[0] =  Qiya[0]/10;
		apstatusold[0] = apstatus[0];
    }
	
	//后桥气压
    apstatus[1] = ( (Qiya[1] < QiyaLowLmt) || (Qiya[1] > QiyaHighLmt) );
    if(((0x00 == eol_airsensortype) || (0x02 == eol_airsensortype) && (0x00 == PAGE))
	&& (mainview_update || ((Qiya[1]/10) != Qiya_old[1]) || apstatus[1] != apstatusold[1]) )
	{
		vw_clearFont31Value(742, 400-1, 10);
		if(Qiya[1] < QiyaLowLmt)
		{
			u8_color[0] = RED;
			u8_color[1] = QiYa_LOW_Alpha;
		}
		else if(Qiya[1] >= QiyaHighLmt)
		{
			u8_color[0] = YELLOW;
			u8_color[1] = QiYa_HIGH_Alpha;
		}
		else
		{
			u8_color[0] = WHITE;
			u8_color[1] = QiYa_Alpha;
		}
		
		loc_Render_FHP(740, 392, u8_color[0], "9");
		
		u32PixelWidth = Qiya[1] * 183 / QiyaHighLmt;
		LCD_DrawRect(554, 407, u32PixelWidth, 12, u8_color[1]);
		
		zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_printNum(670, 390, Qiya[1]/10, 1, ZK_ALIGN_RIGHT);//气压2
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, VM_COLOR_WHITE, 0);//24piex
		zk_oem2uni((uint8_t *)"Bar", uniStr, sizeof(uniStr));
		zk_printZH(680, 400, uniStr);

		Qiya_old[1] =  Qiya[1]/10;
		apstatusold[1] = apstatus[1];
    }
	
	//驻车气压
    apstatus[2] = ( (Qiya[2] < QiyaLowLmt) || (Qiya[2] > QiyaHighLmt) );
    if((0x02 == eol_airsensortype) && (0x00 != PAGE)
	&& (mainview_update || ((Qiya[2]/10) != Qiya_old[2] ) || apstatus[2]!= apstatusold[2]) )
	{
		vw_clearFont31Value(742, 400-1, 10);
		if(Qiya[2] < QiyaLowLmt)
		{
			u8_color[0] = RED;
			u8_color[1] = QiYa_LOW_Alpha;
		}
		else if(Qiya[2] >= QiyaHighLmt)
		{
			u8_color[0] = YELLOW;
			u8_color[1] = QiYa_HIGH_Alpha;
		}
		else
		{
			u8_color[0] = WHITE;
			u8_color[1] = QiYa_Alpha;
		}
		
		loc_Render_FHP(740, 392, u8_color[0], "l");
		
		u32PixelWidth = Qiya[2] * 183 / QiyaHighLmt;
		LCD_DrawRect(554, 407, u32PixelWidth, 12, u8_color[1]);
		
		zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_printNum(670, 390, Qiya[2]/10, 1, ZK_ALIGN_RIGHT);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh22, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Bar", uniStr, sizeof(uniStr));
		zk_printZH(680, 400, uniStr);

		Qiya_old[2] =  Qiya[2]/10;
		apstatusold[2] = apstatus[2];
    }
    
    end_draw();
}
void Update_Miles(void)
{
    static uint32_t old_totmiles;
    static uint32_t old_subtotmiles;
	uint32_t totmiles;
    uint32_t subtotmiles;
	
	if(0U == e_uint)
	{//km
		subtotmiles = Miles.single_miles;
		totmiles    = Miles.total_miles/10;
	}
	else
	{//mile
		subtotmiles = Miles.single_miles/1.609344;
		totmiles    = Miles.total_miles/10/1.609344;
	}
	
    if(mainview_update || old_subtotmiles != subtotmiles)
	{
		old_subtotmiles = subtotmiles;
		
		loc_ClearRect(70, 438, 100, 40);
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(170, 438, subtotmiles, 1, ZK_ALIGN_RIGHT);
    }
    if(mainview_update || old_totmiles != totmiles)
	{
		old_totmiles = totmiles;
		
		loc_ClearRect(642, 438, 100, 40);
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(742, 438, totmiles, 0, ZK_ALIGN_RIGHT);
    }
}
void Update_power_vol_current(void)
{
    uint8_t uniStr[64];
    
    static uint32_t TotalH2Consp, TotalH2Consp_old;
	static uint32_t SubtotH2Consp, SubtotH2Consp_old;
	static uint32_t AvgH2Consp, AvgH2Consp_old;
	
	
    static uint8_t displayFreqCtrl = 0;
    static uint8_t speedlimit = 0;
   
	static uint16_t leftpower, leftpower_old;
	static uint16_t extendmiles, extendmiles_old;
	static uint16_t avepower, avepower_old;
	static uint32_t trippowercons, trippowercons_old;
    static uint32_t totalpower, totalpower_old;
	
	static uint32_t motorpower, motorpower_old;
	// static int16_t  avetemp, avetemp_old;
	static uint32_t voltage, voltage_old;
    static int32_t  inspower, inspower_old;
	
	static uint16_t aux_bat, aux_bat_old;
	static uint16_t pto_driving_rang, pto_driving_rang_old;
	
	uint8_t Vspeed;
	
    if(displayFreqCtrl < 5)
		displayFreqCtrl++;
    else
	{
		displayFreqCtrl =0;
		mainUpdateFreq = !mainUpdateFreq;
    }
	
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	}
	else
	{
		zk_init(loc_Drw2dDev, &GUI_Fontwryh24, VM_COLOR_WHITE, 0);
	}
	
	if(PAGE == 0)
	{
		//续航里程
		Vspeed = get_vehicle_speed();
		extendmiles = get_driving_range();
		
		if(mainview_update
		||((extendmiles != extendmiles_old) && mainUpdateFreq)
		||(speedlimit != (Vspeed < 5)) )
		{
			extendmiles_old = extendmiles;
			speedlimit = (Vspeed < 5);
			
			start_draw();
			loc_ClearRect(220 - 56, 142, 56, 32);
			if(extendmiles == 0xffffu)
			{
				zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
				zk_printZH(222-22, 142, uniStr);
			}
			else
			{
				zk_printNum(220, 142, extendmiles, 0, ZK_ALIGN_RIGHT);
			}
			end_draw();
			
		}
		
		if(0x2d == eol_fadongjiorkaji)
		{//小计氢耗
			SubtotH2Consp = Miles.single_H2_consp;
			if(SubtotH2Consp > 999999) SubtotH2Consp = 999999;
			if(mainview_update ||((SubtotH2Consp_old != SubtotH2Consp) && mainUpdateFreq))
			{
				SubtotH2Consp_old = SubtotH2Consp;
				
				start_draw();
				if(eol_language == 0)
					loc_ClearRect(736 - 120, 142, 120, 32);
				else
					loc_ClearRect(736 - 70, 142, 70, 26);
				
				zk_printNum(736, 142, SubtotH2Consp, 1, ZK_ALIGN_RIGHT);
				
				end_draw();
			}
		}
		else
		{//小计电耗
			trippowercons = get_subtot_power_consp();
			if(mainview_update || ((trippowercons != trippowercons_old) && mainUpdateFreq))
			{
				trippowercons_old = trippowercons;
				
				start_draw();
				loc_ClearRect(736 - 56, 142, 56, 32);
				if(trippowercons == 0xffffffffu)
				{
					zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
					zk_printZH(736-22, 142, uniStr);
				}
				else
				{
					zk_printNum(736, 142, trippowercons, 0, ZK_ALIGN_RIGHT);
				}
				end_draw();
				
			}
		}
		
		//剩余功率
		leftpower = get_residual_power();
		if(mainview_update || ((leftpower != leftpower_old) && mainUpdateFreq))
		{
			leftpower_old = leftpower;
			
			start_draw();
			loc_ClearRect(220 - 56, 142+30, 56, 32);
			if(leftpower == 0xffffu)
			{
				zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
				zk_printZH(220-22, 142+30, uniStr);
			}
			else
			{
				zk_printNum(220, 142+30,leftpower, 0, ZK_ALIGN_RIGHT);
			}
			end_draw();
			
			
		}
		
		//电机功率
		motorpower = get_mot_power();
		if(mainview_update || ((motorpower != motorpower_old) && mainUpdateFreq))
		{
			motorpower_old = motorpower;
			
			start_draw();
			loc_ClearRect(736 - 84, 142+30, 84, 32);
			if(motorpower == 0xffffffffu)
			{
				zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
				zk_printZH(736-22, 142+30, uniStr);
			}
			else
			{
				zk_printNum(736, 142+30,motorpower, 0, ZK_ALIGN_RIGHT);
			}
			end_draw();
		}
		
		//底盘续航
		pto_driving_rang = get_pto_driving_rang();
		if(mainview_update ||((pto_driving_rang != pto_driving_rang_old) && mainUpdateFreq))
		{
			pto_driving_rang_old = pto_driving_rang;
			
			start_draw();
			// loc_ClearRect(746 - 56, 142-30, 56, 32);
			
			loc_ClearRect(520, 142-30, 280, 32);
			if(pto_driving_rang != 0xffffu)
			{
				if(eol_language == 0)
				{
					zk_printZH(520, 142-30, zk_oem2uniStr((uint8_t *)"底盘续航"));
					zk_printZH(750, 142-30, zk_oem2uniStr((uint8_t *)"km"));
					zk_printNum(746, 142-30, pto_driving_rang, 0, ZK_ALIGN_RIGHT);
				}
				else
				{
					zk_printZH(520, 142-30, zk_oem2uniStr((uint8_t *)"Chassis Range"));
					zk_printZH(750, 142-30, zk_oem2uniStr((uint8_t *)"km"));
					zk_printNum(746, 142-30, pto_driving_rang, 0, ZK_ALIGN_RIGHT);
				}
			}
			end_draw();
		}
	}
	else
	{
		
		if(0x2d == eol_fadongjiorkaji)
		{//平均氢耗
			AvgH2Consp = get_avg_H2_consp();
			if(mainview_update ||((AvgH2Consp_old != AvgH2Consp) && mainUpdateFreq))
			{
				AvgH2Consp_old = AvgH2Consp;
				
				start_draw();
				
				if(eol_language == 0)
					loc_ClearRect(176-66, 142, 66, 32);
				else
					loc_ClearRect(196 - 55, 142, 55, 26);
				
				if(AvgH2Consp == 0xffffffffu)
				{
					zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
					if(eol_language == 0)
						zk_printZH(176-26, 142, uniStr);
					else
						zk_printZH(196-26, 142, uniStr);
				}
				else
				{
					if(eol_language == 0)
					{
						zk_printNum(176, 142, AvgH2Consp, 1, ZK_ALIGN_RIGHT);
					}
					else
					{
						zk_printNum(196, 142, AvgH2Consp, 1, ZK_ALIGN_RIGHT);
					}
				}
				end_draw();
			}
		}
		else
		{//平均电耗
			avepower = get_avg_power_consp();
			if(mainview_update || ((avepower != avepower_old) && mainUpdateFreq))
			{
				avepower_old = avepower;
				
				start_draw();
				if(avepower == 0xffffu)
				{
					if(eol_language == 0)
					{
						loc_ClearRect(176-50, 142, 50, 32);
						zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
						zk_printZH(176-26, 142, uniStr);
					}
					else
					{
						loc_ClearRect(196-50, 142, 50, 32);
						zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
						zk_printZH(196-26, 142, uniStr);
					}
				}
				else
				{
					if(eol_language == 0)
					{
						loc_ClearRect(176-50, 142,50, 32);
						zk_printNum(176, 142,avepower, 0, ZK_ALIGN_RIGHT);
					}
					else
					{
						loc_ClearRect(196-50, 142, 50, 32);
						zk_printNum(196, 142,avepower, 0, ZK_ALIGN_RIGHT);
					}
				}
				end_draw();
			}
		}
		
		if(0x2d == eol_fadongjiorkaji)
		{//累计氢耗
			TotalH2Consp = get_total_H2_consp();
			if(mainview_update || ((TotalH2Consp_old != TotalH2Consp) && mainUpdateFreq))
			{
				TotalH2Consp_old = TotalH2Consp;
				
				start_draw();
				
				if(eol_language == 0)
					loc_ClearRect(264 - 145, 142+30, 145, 32);
				else
					loc_ClearRect(264 - 66, 142+30, 66, 26);
				
				if(TotalH2Consp >= 0xfffe)
				{
					zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
					zk_printZH(238, 142+30, uniStr);
				}
				else
				{
					zk_printPosNum(264, 142+30, TotalH2Consp, 0, ZK_ALIGN_RIGHT);
				}
				end_draw();
			}
			
		}
		else
		{//累计电耗
			totalpower = get_total_power_consp();
			if(mainview_update ||((totalpower != totalpower_old) && mainUpdateFreq))
			{
				totalpower_old = totalpower;
				
				start_draw();
				if(eol_language == 0)
					loc_ClearRect(264 - 140, 142+30, 140, 32);
				else
					loc_ClearRect(264 - 110, 142+30, 110, 32);
				
				if(totalpower == 0xffffffffu)
				{
					zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
					zk_printZH(238, 142+30, uniStr);
				}
				else
				{
					zk_printPosNum(264, 142+30,totalpower, 0, ZK_ALIGN_RIGHT);
				}
				end_draw();
			}
		}
		
		//高压电池电压
		voltage = get_batt_pack_volt();
		if(mainview_update ||((voltage != voltage_old) && mainUpdateFreq))
		{
			voltage_old = voltage;
			
			start_draw();
			loc_ClearRect(756 - 76, 142, 76, 26);
			if( voltage > 64255 )
			{
				zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
				zk_printZH(756-30, 142, uniStr);
			}
			else
			{
				zk_printNum(756, 142,voltage, 1, ZK_ALIGN_RIGHT);
			}
			end_draw();
		}
		
		//低压电池电压
		aux_bat = get_aux_bat_volt();
		if(mainview_update||((aux_bat_old != aux_bat) && mainUpdateFreq))
		{
			aux_bat_old = aux_bat;
			
			start_draw();
			loc_ClearRect(756-56, 142+30, 56, 32);
			zk_printNum(756, 142+30, aux_bat, 1, ZK_ALIGN_RIGHT);
			end_draw();
		}
		
		//电池平均温度
		/* avetemp = get_batt_pack_avg_tmp();
		if(mainview_update ||((avetemp != avetemp_old) && mainUpdateFreq))
		{
			avetemp_old = avetemp;
			
			start_draw();
			loc_ClearRect(746 - 56, 142-30, 56, 32);
			if(avetemp == 0xffu)
			{
				zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
				zk_printZH(746-22, 142-30, uniStr);
			}
			else
			{
				zk_printNum(746, 142-30,avetemp, 0, ZK_ALIGN_RIGHT);//v
			}
			end_draw();
		} */
	}

	//电池瞬时功率
	inspower = get_batt_pack_inspower();
    if(mainview_update || ((inspower!=  inspower_old) && mainUpdateFreq))
	{
		inspower_old = inspower;
		
		start_draw();
		if(inspower == 0xffffu)
		{
			loc_ClearRect(406 - 100, 395, 100, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(350+36, 395, uniStr);

		}
		else
		{
			loc_ClearRect(406 - 100, 395, 100, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(406, 395,inspower, 1, ZK_ALIGN_RIGHT);
		}
		end_draw();
    }
	
}
void Update_Soc(void)
{
    uint8_t uniStr[64];
	static uint8_t resultshowen = 0;
	
    static uint8_t  old_soc = 0;
	uint8_t  soc;
	
	uint8_t  soc_status;
	
    static uint8_t  old_charge_status = 5;
	uint8_t  charge_status;
	
    static uint16_t old_Remain_time;
	uint16_t Remain_time;
	uint8_t  Remain_hour;
	uint8_t  Remain_min;
	
	const Img_t * const ImgSocBar[] = {
		&Img_socBar,
		&Img_socBarYellow,
		&Img_socBarRed,
	};
	
	soc = get_batt_pack_soc();
	
    if(mainview_update || (old_soc != soc))
    {
		start_draw();
		
		old_soc = soc;
		
		loc_ClearRect(40, 277, 140, 56);
		
		if(soc == 0xffu)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_printZH(80, 286, zk_oem2uniStr((uint8_t *)"--"));
			zk_printZH(120-12, 288, zk_oem2uniStr((uint8_t *)"%"));
		}
		else
		{
			float scale = soc * 1.25 / 10;
			
			if(soc <= 10) //SOC过低
			{
				soc_status = 2; //红色
			}
			else if(soc > 10 && soc <= 20) //SOC低，请充电
			{
				soc_status = 1; //黄色
			}
			else
			{
				soc_status = 0; //绿色
			}
			
			// for(uint8_t i = 0;i<soc;i++)
				// loc_RenderImg(40+i*1.15, 278, ImgSocBar[soc_status]);
			loc_RenderImgScale(40, 278, scale, 1, ImgSocBar[soc_status]);
			
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_printNum(118-6, 287,soc, 0, ZK_ALIGN_RIGHT);
			zk_printZH(122-6, 288, zk_oem2uniStr((uint8_t *)"%"));
			
		}
		
		end_draw();
    }
	
	if(CAN_CHARGE_LINE)
	{
		charge_status = get_charge_status();
		Remain_time = get_remain_chg_time();
		if(mainview_update || (old_charge_status != charge_status) || (old_Remain_time != Remain_time))
		{
			old_charge_status = charge_status;
			old_Remain_time = Remain_time;
			
			start_draw();
			
			loc_ClearRect(0, 350, 185, 22);
			
			if(charge_status == ParkingCharge)
			{
				if(Remain_time != 0xffff)
				{
					Remain_hour = Remain_time / 60;
					Remain_min = Remain_time % 60;
					
					if(eol_language == 0)
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_GRAY, 0);
						zk_printZH( 10, 350, zk_oem2uniStr((uint8_t *)"剩余充电时间")); 
						zk_printZH(132, 350, zk_oem2uniStr((uint8_t *)"时"));
						zk_printZH(165, 350, zk_oem2uniStr((uint8_t *)"分"));
						
						loc_ClearRect(132 - 25, 350, 25, 22);
						loc_ClearRect(165 - 18, 350, 19, 22);
						
						zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
						zk_printPosNum(132, 350,Remain_hour, 0, ZK_ALIGN_RIGHT);
						zk_printPosNum(165, 350,Remain_min, 0, ZK_ALIGN_RIGHT);
						
					}
					else
					{
						zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_GRAY, 0);
						zk_printZH(10, 350, zk_oem2uniStr((uint8_t *)"Remaining")); 
						zk_printZH(132 - 11, 350, zk_oem2uniStr((uint8_t *)"h"));
						zk_printZH(165 - 13, 350, zk_oem2uniStr((uint8_t *)"min"));
						
						loc_ClearRect(132 - 11 - 25, 350, 25, 22);
						loc_ClearRect(165 - 13 - 18, 350, 19, 22);
						
						zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
						zk_printPosNum(132 - 11, 350,Remain_hour, 0, ZK_ALIGN_RIGHT);
						zk_printPosNum(165 - 13, 350,Remain_min, 0, ZK_ALIGN_RIGHT);
						
					}
				}
			}
			else if(charge_status == NotCharged)
			{
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
					zk_oem2uni((uint8_t *)"未充电", uniStr, sizeof(uniStr));
					zk_printZH(10, 350, uniStr); 
					
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
					zk_oem2uni((uint8_t *)"Not charging", uniStr, sizeof(uniStr));
					zk_printZH(10, 350, uniStr); 
					
				}
			}
			else if(charge_status == ChargingOK)
			{
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"充电完成", uniStr, sizeof(uniStr));
					zk_printZH(10, 350, uniStr); 
					
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"charge complete", uniStr, sizeof(uniStr));
					zk_printZH(10, 350, uniStr); 
					
				}
			}
			else if(charge_status == ChargingAnomaly)
			{
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"充电异常", uniStr, sizeof(uniStr));
					zk_printZH(10, 350, uniStr); 
					
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"Abnormal charge", uniStr, sizeof(uniStr));
					zk_printZH(10, 350, uniStr); 
					
				}
			}
			else ;
			end_draw();
		}
	}
    if(Test_Mode == 5)
	{
		start_draw();
		if(testStep < 4)
		{
			if(mainview_update)
			{
				zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_oem2uni((uint8_t *)"Testing", uniStr, sizeof(uniStr));
				zk_printZH(255, 440, uniStr);
				
				zk_printNum(465, 440, VERSIONS_DATE, 0, ZK_ALIGN_LEFT);
			}
			else;
		}
		else
		{
			if(!resultshowen)
			{
				resultshowen = 1;
				
				loc_ClearRect(255, 440, 120, 32);
				if(TestResult == 0xaa)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
					zk_oem2uni((uint8_t *)"FAILED", uniStr, sizeof(uniStr));
					zk_printZH(255, 440, uniStr);
				}
				else if(TestResult == 0x55)
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_oem2uni((uint8_t *)"PASS", uniStr, sizeof(uniStr));
					zk_printZH(255, 440, uniStr);
				}
				else
				{
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_YELLOW, 0);
					zk_oem2uni((uint8_t *)"FAILED", uniStr, sizeof(uniStr));
					zk_printZH(255, 440, uniStr);
				}
			}
		}
		end_draw();
	}
	
}
#define h2soc_offset 10
void Update_H2SOC_Img(unsigned char value)
{
	loc_RenderRect(626, 280, 150, 56,0xff000000);
    if(value < 20)
    {
		for(uint8_t i = 0;i<value;i++)
			loc_RenderImg(626+i*1.40, 281, &Img_socBarRed);
    }
    else
	{
		for(uint8_t i = 0;i<value;i++)
			loc_RenderImg(626+i*1.40, 281, &Img_socBar);
    }
}
void Update_H2soc(void)
{
    static uint8_t h2soc,h2socold;
    uint8_t uniStr[64];
	
    h2soc = get_H2_soc();
	
    start_draw();
    if(mainview_update || h2soc !=  h2socold)
	{
		h2socold = h2soc;
		
		if(h2soc == 0xffu)
		{
			loc_ClearRect(610, 275, 190, 65);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(654, 286, uniStr);
		}
		else
		{
			Update_H2SOC_Img(h2soc);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_printNum(690-h2soc_offset, 286,h2soc, 0, ZK_ALIGN_RIGHT);
		}
		
		zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"%", uniStr, sizeof(uniStr));
		zk_printZH(692-h2soc_offset, 286, uniStr);
		zk_oem2uni((uint8_t *)"(H", uniStr, sizeof(uniStr));
		zk_printZH(720-h2soc_offset, 286, uniStr);
		
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"2", uniStr, sizeof(uniStr));
		zk_printZH(753-h2soc_offset, 300, uniStr);
		
		zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)")", uniStr, sizeof(uniStr));
		zk_printZH(769-h2soc_offset, 286, uniStr);
		
    }
    end_draw();
}
#define rpm_offset 8
void Update_RPM_Img(unsigned int value)
{
	if(value > 9000)
		value = 9000;
	
    loc_ClearRect(610, 275, 190, 56);
    for(uint32_t i = 0;i<value;i+=80)
		loc_RenderImg(641+i*1.04/80, 275, &Img_barRight_rpm);

}
void Update_Rpm(void)
{
    static uint16_t rpm,rpmold;
    uint8_t uniStr[64];
	
    rpm = get_motor_speed();
    
    start_draw();
    if(mainview_update||rpm !=  rpmold)
	{
		if(rpm == 0xffffu)
		{
			loc_ClearRect(610, 275, 190, 80);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(680-rpm_offset, 290-6, uniStr);
			zk_oem2uni((uint8_t *)"rpm", uniStr, sizeof(uniStr));
			zk_printZH(700+4-rpm_offset, 290-8, uniStr);
		}
		else
		{
			Update_RPM_Img(rpm);
			// loc_ClearRect(584+46, 280, 210 - 46, 80);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(716-rpm_offset, 290-4,rpm, 0, ZK_ALIGN_RIGHT);//rpm
			zk_init(loc_Drw2dDev, &GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"rpm", uniStr, sizeof(uniStr));
			zk_printZH(718-rpm_offset, 290-8, uniStr);
			
		}
		rpmold = rpm;
    }
    end_draw();
}
void Update_Gear(void)
{
	static uint8_t gearold = 0U;
	static uint8_t gearModeOld = 0xFF;
	static uint8_t peristalsisModeOld = 0xFF;
	static uint8_t epmodeOld = 0xFF;
	uint8_t gear_mode;
	uint8_t gear;
	uint8_t ep_mode;
	uint8_t peristalsis_mode;
	char *num[10] = {"b","c","d","e","f","g","h","i","j","k"};
	
	gear_mode = get_vehicle_gear_mode();
	ep_mode = get_vehicle_ep_mode();
	gear = get_vehicle_current_gear();
	peristalsis_mode = get_vehicle_peristalsis_mode();
	
    if((mainview_update)
	|| (gear != gearold) 
	|| (gear_mode != gearModeOld) 
	|| (epmodeOld != ep_mode)
	|| (peristalsis_mode != peristalsisModeOld) )
	{
		LCD_DrawRect(360, 434, 100, 50, TOUMING);
		
		if(gear_mode == 0) //M模式
		{
			if(gear == 0)
			{
				loc_Render_FHP(378,434,GREEN,"W");//GEAR_M-N 
			}
			else if((gear > 0) && (gear < 100))
			{
				loc_Render_FHP(371,434,GREEN,"Q");//GEAR_M-D
				if(peristalsis_mode == 1)
				{
					loc_Render_FHP(420,434,GREEN, "y");
				}
				else
				{
					if(gear < 10)
					{
						loc_Render_FHP(420,434,GREEN,num[gear%10]);
					}
					else
					{
						loc_Render_FHP(420,434,GREEN,num[gear/10]);
						loc_Render_FHP(432,434,GREEN,num[gear%10]);
					}
					
				}
				
			}
			else if(gear == 100)
			{
				loc_Render_FHP(378,434,YELLOW,"X");// GEAR_M-R
				if(peristalsis_mode == 1)
				{
					loc_Render_FHP(420,434,GREEN, "y");
				}
				else
				{
					loc_Render_FHP(418,434,YELLOW,"c");// character 1
				}
			}
			else
			{
				loc_Render_FHP(371,436,WHITE,"a");//GEAR 无效
			}		
		}		
		else if(gear_mode == 1) //A模式
		{
			if(gear == 0)
			{
				loc_Render_FHP(378,434,GREEN,"O");// GEAR_A-N 
			}
			else if((gear > 0) && (gear < 100))
			{
				loc_Render_FHP(375,434,GREEN,"I");//GEAR_A-D
				if(peristalsis_mode == 1)
				{
					loc_Render_FHP(420,434,GREEN, "y");
				}
				else
				{
					if(gear < 10)
					{
						loc_Render_FHP(420,434,GREEN,num[gear%10]);
					}
					else
					{
						loc_Render_FHP(420,434,GREEN,num[gear/10]);
						loc_Render_FHP(432,434,GREEN,num[gear%10]);
					}
				}
			}
			else if(gear == 100)
			{
				loc_Render_FHP(378,434,YELLOW,"P");// GEAR_A-R 
				if(peristalsis_mode == 1)
				{
					loc_Render_FHP(420,434,YELLOW, "y");
				}
				else
				{
					loc_Render_FHP(418,434,YELLOW,"c");// character 1
				}
			}
			else
			{
				loc_Render_FHP(371,436,WHITE,"a");//GEAR 无效
				
			}
		}
		else //无档位模式
		{
			if(0 == gear)
			{
				loc_Render_FHP(395,434,GREEN,"R");//N
			}
			else if((gear > 0) && (gear < 100))
			{
				loc_Render_FHP(395,434,GREEN,"T");//D
				if(peristalsis_mode == 1)
				{
					loc_Render_FHP(420,434,GREEN, "y");
				}
				else
				{
					if(eol_atmtype != 33U && eol_atmtype != 34U)
					{
						if(gear < 10)
						{
							loc_Render_FHP(420,434,GREEN,num[gear%10]);
						}
						else
						{
							loc_Render_FHP(420,434,GREEN,num[gear/10]);
							loc_Render_FHP(432,434,GREEN,num[gear%10]);
						}
					}
				}
			}
			else if(100 == gear)
			{
				loc_Render_FHP(395,434,YELLOW,"S");//R
				if(peristalsis_mode == 1)
				{
					loc_Render_FHP(420,434,YELLOW, "y");
				}
			}
			else
			{
				loc_Render_FHP(371,436,WHITE,"a");//GEAR 无效
			}
		}
		
		if(gear <= 100)
		{
			if(ep_mode == 0)
				loc_RenderImg(382, 454, &Img_E); 
			else if(ep_mode == 1)
				loc_RenderImg(382, 454, &Img_P);
			else ;
		}
		
		gearModeOld = gear_mode;
		epmodeOld = ep_mode;
		gearold = gear;
		peristalsisModeOld = peristalsis_mode;
    }
}
void update_updown(uint8_t current_item, uint8_t total_items,uint32_t x, uint32_t y)
{
    if(current_item == 0)
		return;
    if(current_item>total_items)
		total_items = current_item;
    loc_ClearRect(x + 12 , 75+y, 800 - x - 22, 30);
	// LCD_DrawRect(x + 12 , 75+y, 800 - x - 22, 30, GRAY);
    zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
    zk_init(loc_Drw2dDev, &GUI_Fontwryh22, VM_COLOR_WHITE, 0);
    zk_printNum(x + 60 - 30, 78 + y, current_item, 0, ZK_ALIGN_RIGHT);
	zk_printZH( x + 60 - 30, 78 + y, zk_oem2uniStr((uint8_t*)"/"));
    zk_printNum(x + 60 - 22, 78 + y, total_items, 0, ZK_ALIGN_LEFT);
    
    if(y == 0)
	{
		if((current_item==1)&&(total_items>1))
		{
			loc_RenderImg(x+60, 75+y, &Img_upColor);
			loc_RenderImg(x+61, 75+18+y, &Img_down);
		}
		else if((current_item==1)&&(total_items==1))
		{
			loc_RenderImg(x+60, 75+y, &Img_up);
			loc_RenderImg(x+61, 75+18+y, &Img_down);
		}
		else if(current_item==total_items)
		{
			loc_RenderImg(x+60, 75+y, &Img_up);
			loc_RenderImg(x+61, 75+18+y, &Img_downColor);
		}
		else
		{
			loc_RenderImg(x+60, 75+y, &Img_upColor);
			loc_RenderImg(x+61, 75+18+y, &Img_downColor);
		}
    }
    else;

}
void Display_DM_Main(void)
{
	uint8_t uniStr[128];
	static uint32_t oldDMListBuffer[100] = {0};
	static uint32_t DMListBuffer[100] = {0};
	static uint32_t olddmcounter = 0;
	static uint32_t dmcounter = 0;
	static uint32_t faulttotalnumold = 0;
	uint16_t Xwidth = 0;
	uint16_t CrtShowFltNum = 0;
	
	memset(&DMListBuffer,0,sizeof(DMListBuffer));
	
	//标志位 ZhongLianLockVehFlg == 1 时，确认为中联锁车方案，仪表界面不进行锁车相关提示
	if((LckVehBoundState == 1 && LckVehdiplayState <= 9 && LckVehdiplayState > 0) && (ZhongLianLockVehFlg == 0))
	{
		CrtShowFltNum = 1;
		switch(LckVehdiplayState)
		{
			case 1: DMListBuffer[0] = 2443; break;
			case 2: DMListBuffer[0] = 2444; break;
			case 3: DMListBuffer[0] = 2445; break;
			case 4: DMListBuffer[0] = 2446; break;
			case 5: 
				if(!isOverseasVer)
					DMListBuffer[0] = 2447;
				else
					DMListBuffer[0] = 2673;
				break;
			case 6: DMListBuffer[0] = 2448; break;
			case 7: 
				if(!isOverseasVer)
					DMListBuffer[0] = 2449;
				else
					DMListBuffer[0] = 2674;
				break;
			case 8: 
				if(!isOverseasVer)
					DMListBuffer[0] = 2450;
				else
					DMListBuffer[0] = 2675;
				break;
			case 9: DMListBuffer[0] = 2451; break;
			default:
				DMListBuffer[0] = 0;
				CrtShowFltNum = 0;
				break;
		}
	}
	else CrtShowFltNum = 0;
	
	for(uint32_t cnt = 0; cnt < DM1_Ftl_Sum; cnt++)
	{
		if(DMPackage[cnt].set == 1)
		{
			DMListBuffer[CrtShowFltNum] = cnt; //copy dm number to buffer
			
			if(oldDMListBuffer[CrtShowFltNum] != cnt)
			{
				oldDMListBuffer[CrtShowFltNum] = cnt;
				alarm_update_flag = 0;
			}
			
			CrtShowFltNum++;
		}
	}
	
	if(CrtShowFltNum == 0)
	{
		alarm_update_flag = 1;
		upgo = 0;
		downgo = 0;
		dmcounter = 0;
	}
	else
	{
		if(downgo && (dmcounter > 0))
		{
			dmcounter--;
		}
		else if(upgo && (dmcounter < (CrtShowFltNum-1)))
		{
			dmcounter++;
		}
		else;
		
		upgo = 0;
		downgo = 0;
		
		if((faulttotalnumold != CrtShowFltNum))
		{
			dmcounter = 0;
		}
	}
	
	if(mainview_update || (alarm_update_flag == 0) || (faulttotalnumold != CrtShowFltNum) || (olddmcounter != dmcounter))
	{
		if(ready_disp_flag == 1)
		{
			/* 等待 READY 先擦除掉*/
			return ;
		}
		
		faulttotalnumold = CrtShowFltNum;
		
		olddmcounter = dmcounter;
		
		alarm_update_flag = 1;
		
		start_draw();
		
		loc_ClearRect(0, 71, 800, 33);
		
		if(CrtShowFltNum == 0)
		{
			dm1_show_flg = 0;
		}
		else
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni(DMPackage[DMListBuffer[dmcounter]].msg[eol_language], uniStr, sizeof(uniStr));
			// Xwidth = (strlen(DMPackage[DMListBuffer[dmcounter]].msg[eol_language])) * 6;
			Xwidth = zk_getTextWidth(DMPackage[DMListBuffer[dmcounter]].msg[eol_language]);
			zk_printZH(400 - Xwidth/2, 72, uniStr);
			update_updown(dmcounter+1, CrtShowFltNum, 400 + Xwidth/2, 0);
			
			dm1_show_flg = 1;
		}
		
		end_draw();
	}
	
	dmUpdate = 0;
	
}
void frmMain(void)
{
    if (f_index == 0)  //只更新一次
	{
        f_index = 1;
        mainFrame();
		tpmsalarm = 0;
		BigIcon_Showing = 0;
		DisplayTruckFlg = 1;
		mainview_update = 1;
		ChargeViewEnable = 0;
		ChuiSaoViewEnable = 0;
		SmartRechgViewFlg = 0;
		GetOffReminderViewFlg = 0;
    }
	else if (f_index == 1)
	{
        f_index = 2;
		
		Update_AirPressures();
    }
	else if (f_index == 2)
	{
        f_index = 3;
		
		IC_zijian();
		Display_Telltale();
		Display_TruckArea();
		loc_auxiliary_braking();
    }
	else if (f_index == 3)
	{
        f_index = 4;
		
		Display_Temperature();
		Display_Date();
		Update_Gear();
		Update_Miles();
    }
	else if (f_index == 4)
	{
        f_index = 5;
		Update_Soc();
		if(eol_fadongjiorkaji==0x2d)
			Update_H2soc();
		else
			Update_Rpm();
    }
	else if (f_index == 5)
	{
        f_index = 6;
		Update_power_vol_current();
		
    }
	else if (f_index == 6)
	{
        f_index = 7;
		disp_texture_warning();
		
    }
	else if (f_index == 7)
	{
        f_index = 1;
		
		Display_DM_Main();
		
		mainview_update = 0;
		
    }
    else 
	{

        f_index = 0;
    }
	
	if (key_up)  //调节背光灯
	{
		key_up = 0;
		upgo = 0;
		alarm_update_flag = 0;
		downgo = 1; //故障往下翻
	}

	if (key_set)
	{
		key_set = 0;
		key_long_set = 0;
		key_up = 0;
		key_AV = 0;
		key_duble = 0;
		if (time_flag == 0)
		{
			f_index = 0;
			frmID = 1;
		}
		time_flag = 0;
	}
	if (key_AV)
	{
		key_AV = 0;
		key_long_set = 0;
		key_up = 0;
		key_set = 0;
		alarm_update_flag = 0;
		upgo = 1;//故障往上翻
		downgo = 0;
	}
	if(key_clear)
	{
		key_clear = 0;
		PAGE = !PAGE;
		f_index = 0;
		mainview_update = 1;
		ready_not_shown = 1;
	}
	if (key_duble)
	{
		key_duble = 0;
		
	}
	if(key_long_set)
	{
		key_long_set = 0;
		Miles.single_miles = 0;
		Miles.single_H2_consp = 0;
		clear_Trip_flag = 1;
	}

    
}


/**
 * \brief draw table text
 * \return none
 */
void vw_drawTblText(int32_t x, int32_t y, uint16_t index, const tblText_t *pTxt)
{
    int32_t posX, posY;
    uint8_t uniStr[64];

    if (R_NULL == pTxt)
    {
        return ;
    }

    zk_init(loc_Drw2dDev, pTxt->font, pTxt->color, 0);

    for (uint8_t c = 0; c < pTxt->columnCnt; c++)
    {
        posX = x + pTxt->columnWidth * c;
        posY = y;

        for (int32_t i = 0; (i < pTxt->rowCnt) && (index < pTxt->msgCnt); i++)
        {
            zk_oem2uni(pTxt->msg[index].itemStr[eol_language], uniStr, sizeof(uniStr));
            zk_printZH(posX, posY, uniStr);
            posY += pTxt->rowHeight;
            index ++;
        }
    }
}
/******************************************************/


void UndefinedFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[100];
	
    start_draw();
	LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"本车型无此功能!", uniStr, sizeof(uniStr));
		zk_printZH(200, 240, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"This-Function-Is-Not-Available!", uniStr, sizeof(uniStr));
		zk_printZH(200, 240, uniStr);
	}
    
    disable_box();
    disable_choose_image();
    end_draw();


}
void UndefinedFrameUpdate(unsigned char index)
{
    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}


/******************************************************/
/** 车辆信息菜单 **/
void MenuVehcleInfoFrame(void)
{
    uint8_t uniStr[64];
    uint16_t width;
	
	TEXT_LIST_t VehInfoMenu[] =
	{
		{ 0, {(uint8_t *)"SOH与K值",       (uint8_t *)"SOH and K Factor"     }, 295, 130 + 55 * 0, NULL},
		{ 1, {(uint8_t *)"制动系统信息",   (uint8_t *)"Brake system info"    }, 295, 130 + 55 * 1, NULL},
		{ 2, {(uint8_t *)"胎压监测系统",   (uint8_t *)"TPMS"                 }, 295, 130 + 55 * 2, NULL},
		{ 3, {(uint8_t *)"电池信息",       (uint8_t *)"Batt. Info."         }, 295, 130 + 55 * 3, NULL},
		{ 4, {(uint8_t *)"供应商",         (uint8_t *)"Suppliers"            }, 295, 130 + 55 * 4, NULL},
		
		{ 5, {(uint8_t *)"查看实时故障",   (uint8_t *)"Curr. Errors"         }, 550, 130 + 55 * 0, NULL},
		{ 6, {(uint8_t *)"整车保养信息",   (uint8_t *)"Maintenance info"     }, 550, 130 + 55 * 1, NULL},
		{ 7, {(uint8_t *)"轴荷信息",       (uint8_t *)"Axle load info"       }, 550, 130 + 55 * 2, NULL},
		{ 8, {(uint8_t *)"电机电压",       (uint8_t *)"Motor Voltage"        }, 550, 130 + 55 * 3, NULL},
		{ 9, {(uint8_t *)"查看控制器版本", (uint8_t *)"ICU version"          }, 550, 130 + 55 * 4, NULL},
		
	};
	TEXT_LIST_t VehInfoMenu2[] =
	{
		{ 0, {(uint8_t *)"SOH与K值",       (uint8_t *)"SOH and K Factor"     }, 295, 130 + 45 * 0, NULL},
		{ 1, {(uint8_t *)"制动系统信息",   (uint8_t *)"Brake system info"    }, 295, 130 + 45 * 1, NULL},
		{ 2, {(uint8_t *)"胎压监测系统",   (uint8_t *)"TPMS"                 }, 295, 130 + 45 * 2, NULL},
		{ 3, {(uint8_t *)"电池信息",       (uint8_t *)"Batt. Info."         }, 295, 130 + 45 * 3, NULL},
		{ 4, {(uint8_t *)"供应商",         (uint8_t *)"Suppliers"            }, 295, 130 + 45 * 4, NULL},
		{ 5, {(uint8_t *)"查看实时故障",   (uint8_t *)"Curr. Errors"         }, 295, 130 + 45 * 5, NULL},
		
		{ 6, {(uint8_t *)"燃料电池系统",   (uint8_t *)"FCU"                  }, 550, 130 + 45 * 0, NULL},
		{ 7, {(uint8_t *)"车载氢系统",     (uint8_t *)"HMS"                  }, 550, 130 + 45 * 1, NULL},
		{ 8, {(uint8_t *)"整车保养信息",   (uint8_t *)"Maintenance info"     }, 550, 130 + 45 * 2, NULL},
		{ 9, {(uint8_t *)"轴荷信息",       (uint8_t *)"Axle load info"       }, 550, 130 + 45 * 3, NULL},
		{10, {(uint8_t *)"电机电压",       (uint8_t *)"Motor Voltage"        }, 550, 130 + 45 * 4, NULL},
		{11, {(uint8_t *)"查看控制器版本", (uint8_t *)"ICU version"          }, 550, 130 + 45 * 5, NULL},
		
	};
	
	
    start_draw();
    if(!Menu_VehicleInfo_f)
	{
		LCD_Clear();
		
		for(uint8_t i = 0; i < MAJ_MENU_ITEM_SUM; i++)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_oem2uni(MajMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
			width = zk_getTextWidth(MajMenu[i].Text[eol_language]);
			zk_printZH(MajMenu[i].Pos_X - width / 2,  MajMenu[i].Pos_Y, uniStr);
		}
		
		if(eol_fadongjiorkaji==0x2d)
		{
			for(uint8_t i = 0; i < 12; i++)
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni(VehInfoMenu2[i].Text[eol_language], uniStr, sizeof(uniStr));
				zk_printZH(VehInfoMenu2[i].Pos_X,  VehInfoMenu2[i].Pos_Y, uniStr);
			}
		}
		else
		{
			for(uint8_t i = 0; i < 10; i++)
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni(VehInfoMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
				zk_printZH(VehInfoMenu[i].Pos_X,  VehInfoMenu[i].Pos_Y, uniStr);
			}
		}
		
		
		
    }
    update_box_location(22,124);
    disable_choose_image();
    //  Menu_VehicleInfo_f = 0;
    Menu_TripInfo_f = 0;
    Menu_PSNInfo_f = 0;
    Menu_IC_f = 0;

    end_draw();

}
void MenuVehicleInfoUpdate(void)
{
    static unsigned char i = 8 ;
    static unsigned char ichoose = 10 ;
	
    if(!Menu_VehicleInfo_f)
	{
		if (key_up) {//箭头向下走
			key_up = 0;
			i++;
		}
		
		if (key_AV) {//箭头向上走
			key_AV = 0;
			i--;
		}
		if(i==6 ||i ==10)
			i=8;
		if (i == 8) {
			if (key_set) {
				key_set = 0;
				// f_index = 0;
				frmID = Menu_VehicleInfo;
				Menu_VehicleInfo_f = 1;
				i=10;
				ichoose = 0;
				if(eol_fadongjiorkaji==0x2d)
					update_choose_location(270, 134 + 45 * 0);
				else
					update_choose_location(270, 134 + 55 * 0);
			}
		} else if (i == 9 ) {//
			f_index = 0;
			i=8;
			frmID = Menu_TripInfo;
		} else if (i == 7 ) {//
			f_index = 0;
			i=8;
			frmID = Menu_IC;
		} else;
    }
    else
	{
		if (key_AV) {//箭头向下走
			key_AV = 0;
			
			if(eol_fadongjiorkaji==0x2d)
			{
				if(ichoose == 0)
					ichoose = 11;
				else
					ichoose --; 
				
				if(ichoose < 6)
					update_choose_location(270, 134 + 45 * ichoose);
				else
					update_choose_location(525, 134 + 45 * (ichoose - 6));
			}
			else
			{
				if(ichoose == 0)
					ichoose = 9;
				else
					ichoose --; 
				
				if(ichoose < 5)
					update_choose_location(270, 134 + 55 * ichoose);
				else
					update_choose_location(525, 134 + 55 * (ichoose - 5));
			}
		}

		if (key_up) {//箭头向上走
			key_up = 0;
			
			if(eol_fadongjiorkaji==0x2d)
			{
				if(ichoose == 11)
					ichoose = 0;
				else
					ichoose ++;
				if(ichoose < 6)
					update_choose_location(270, 134 + 45 * ichoose);
				else
					update_choose_location(525, 134 + 45 * (ichoose - 6));
			}
			else
			{
				if(ichoose == 9)
					ichoose = 0;
				else
					ichoose ++; 
				
				
				if(ichoose < 5)
					update_choose_location(270, 134 + 55 * ichoose);
				else
					update_choose_location(525, 134 + 55 * (ichoose - 5));
			}
		}

		if (key_set) { //子菜单间切换
			key_set = 0;
			f_index = 0;
			
			if(eol_fadongjiorkaji==0x2d)
			{
				     if (ichoose == 0) frmID = Kvalue;
				else if (ichoose == 1) frmID = BrakeInfo;
				else if (ichoose == 2) frmID = TPMS; 
				else if (ichoose == 3) frmID = BatVol;
				else if (ichoose == 4) frmID = Supplier;
				else if (ichoose == 5) frmID = FaultInfo;
				
				else if (ichoose == 6) frmID = FCUWrkTm;
				else if (ichoose == 7) frmID = HMSMaxPress;
				else if (ichoose == 8) frmID = Maintain;
				else if (ichoose == 9) frmID = ZhouHe;
				else if (ichoose ==10) frmID = MotorVol;
				else if (ichoose ==11) frmID = VCUVersion;
				else ;
			}
			else
			{
				     if (ichoose == 0) frmID = Kvalue;
				else if (ichoose == 1) frmID = BrakeInfo;
				else if (ichoose == 2) frmID = TPMS; 
				else if (ichoose == 3) frmID = BatVol;
				else if (ichoose == 4) frmID = Supplier;
				else if (ichoose == 5) frmID = FaultInfo;
				else if (ichoose == 6) frmID = Maintain;
				else if (ichoose == 7) frmID = ZhouHe;
				else if (ichoose == 8) frmID = MotorVol;
				else if (ichoose == 9) frmID = VCUVersion;
				else ;
			}
		}
    }
	
}
void frmMenuVehicleInfo(void)
{
    if (f_index == 0) { //只更新一次
        f_index = 1;
		//VINSHOWED = 0;
		MenuVehcleInfoFrame();
    } else if (f_index == 1) {
        f_index = 1;
        MenuVehicleInfoUpdate();
    } else {
        f_index = 0;
    }
    if (key_long_set) {
        key_long_set = 0;
        key_set = 0;
        key_up = 0;
        key_AV = 0;
		//  time_flag = 1;
    }

    if (key_duble) {
	key_duble = 0;
	TESTMODE = 1;
	if (time_flag == 0) {
	    f_index = 0;
	    frmID = 7;
	}
    }
    if (key_clear) {//返回主界面
	key_clear = 0;
	frmID = 0;
	f_index = 0;
	Index = 0;
	index_page = 1;
	Menu_VehicleInfo_f = 0;
	alarm_update_flag = 0;
	return_from_menu = 1;
	//upgo = 1;
    }
}
/** 车辆信息——SOH与K值**/
void KValueFrame(unsigned char index, unsigned char page)
{
    uint8_t  uniStr[64];
	uint32_t K_Value;
	
	TEXT_LIST_t KVText[] =
	{
		{ 0, {(uint8_t *)"SOH:",  (uint8_t *)"SOH:"     }, 360, 200 + 31 * 0, (uint8_t *)"%"},
		{ 1, {(uint8_t *)"K 值:", (uint8_t *)"K Factor:"}, 360, 200 + 31 * 1, NULL},
		
		// { 1, {(uint8_t *)"监管模式",  (uint8_t *)"Regulatory model"}, 360, 220 + 31 * 2, NULL},
		// { 1, {(uint8_t *)"监管信息:", (uint8_t *)"Regulatory Info:"}, 360, 220 + 31 * 3, NULL},
		
	};
	
    start_draw();
	LCD_Clear();
    disable_box();
    disable_choose_image();
	
	for(uint8_t i = 0; i < 2; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni(KVText[i].Text[eol_language], uniStr, sizeof(uniStr));
		zk_printZH1(KVText[i].Pos_X,  KVText[i].Pos_Y, uniStr, ZK_ALIGN_RIGHT);
		
		if(NULL != KVText[i].Unit)
		{
			zk_oem2uni(KVText[i].Unit, uniStr, sizeof(uniStr));
			zk_printZH(KVText[i].Pos_X + 125,  KVText[i].Pos_Y, uniStr);
		}
	}
	
	K_Value = B_value*HM_Value;
	zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	zk_printNum(KVText[1].Pos_X  + 120, KVText[1].Pos_Y , K_Value, 0, ZK_ALIGN_RIGHT);//k值
	
    end_draw();
    KValue_update = 1;
	
}
void KvalueUpdate(unsigned char index)
{
	uint8_t  uniStr[64];
	
	static uint8_t soh_old = 0;
	static uint8_t old_LckVehdiplayState = 0;
	static uint8_t old_JianGuanInfo = 0;
	
	uint8_t  soh;
	uint16_t num;
	uint16_t width;
	
	soh = get_batt_pack_soh();
	
	start_draw();
	
	if(soh_old !=  soh || KValue_update)
	{
		soh_old = soh;
		
		loc_ClearRect(360 + 120 - 45, 200 + 31 * 0, 45, 31);
		
		if(soh != 0xffu)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(360 + 120, 200 + 31 * 0, soh, 0, ZK_ALIGN_RIGHT);//SOH
		}
		else
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH1(360 + 120, 200 + 31 * 0, uniStr, ZK_ALIGN_RIGHT);
		}
	}
	
	//标志位 ZhongLianLockVehFlg == 1 时，确认为中联锁车方案，仪表界面不进行锁车相关提示
	if((LckVehBoundState == 1) && (ZhongLianLockVehFlg == 0))
	{
		if(old_LckVehdiplayState != LckVehdiplayState || KValue_update)
		{
			old_LckVehdiplayState = LckVehdiplayState;
			
			loc_ClearRect(10, 220+31*2, 780, 31);
			
			if((LckVehdiplayState == 9) || (LckVehdiplayState == 10))
			{
				if(eol_language == 0)
				{
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_oem2uni((uint8_t *)"监管模式", uniStr, sizeof(uniStr));
					zk_printZH1(360, 220+31*2, uniStr, ZK_ALIGN_RIGHT);
				}
				else
				{
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_oem2uni((uint8_t *)"Regulatory model", uniStr, sizeof(uniStr));
					zk_printZH1(360, 220+31*2, uniStr, ZK_ALIGN_RIGHT);
				}
			}
			else
			{
				switch(LckVehdiplayState)
				{
					case 1: num = 2443; break;
					case 2: num = 2444; break;
					case 3: num = 2445; break;
					case 4: num = 2446; break;
					case 5: num = 2447; break;
					case 6: num = 2448; break;
					case 7: num = 2449; break;
					case 8: num = 2450; break;
					case 9: num = 2451; break;
					default:
						num = 0;
						break;
				}
				
				if(num > 0)
				{
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_oem2uni(DMPackage[num].msg[eol_language], uniStr, sizeof(uniStr));
					width = zk_getTextWidth(DMPackage[num].msg[eol_language]);
					zk_printZH(400 - width / 2, 220+31*2, uniStr);
				}
			}
		}
		
		if(old_JianGuanInfo != Remaining_unlocked_times || KValue_update)
		{
			old_JianGuanInfo = Remaining_unlocked_times;
			
			loc_ClearRect(10, 220+31*3, 780, 31);
			
			if(eol_language == 0)
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"监管信息:", uniStr, sizeof(uniStr));
				zk_printZH1(360, 220+31*3, uniStr, ZK_ALIGN_RIGHT);
				zk_printNum(360 + 150, 220+31*3, Remaining_unlocked_times, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"Regulatory Info:", uniStr, sizeof(uniStr));
				zk_printZH1(360, 220+31*3, uniStr, ZK_ALIGN_RIGHT);
				zk_printNum(360 + 150, 220+31*3, Remaining_unlocked_times, 0, ZK_ALIGN_LEFT);
			}
		}
	}
	else
	{
		loc_ClearRect(10, 220+31*2, 780, 31);
		loc_ClearRect(10, 220+31*3, 780, 31);
	}
	
	end_draw();
	
	KValue_update = 0;
	
	if (key_up)  //子菜单切换
	{
		key_up = 0;
		f_index = 0;
		frmID = Menu_VehicleInfo;       
	}
	
	if (key_set)
	{
		key_set = 0;
		f_index = 0;
		frmID = Menu_VehicleInfo;                   
	}
	
	if (key_AV)
	{
		key_AV = 0;
		f_index = 0;
		frmID = Menu_VehicleInfo;             
	}
	
}
/** 车辆信息——制动系统信息**/
void BrakeInfoFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[100];
	
    start_draw();
	LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"前回路气压值:", uniStr, sizeof(uniStr));
		zk_printZH(200, 200, uniStr);
		zk_oem2uni((uint8_t *)"后回路气压值:", uniStr, sizeof(uniStr));
		zk_printZH(200, 240, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Front loop :", uniStr, sizeof(uniStr));
		zk_printZH(200, 200, uniStr);
		zk_oem2uni((uint8_t *)"Rear loop :", uniStr, sizeof(uniStr));
		zk_printZH(200, 240, uniStr);
	}
    

    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);//24piex
    zk_oem2uni((uint8_t *)"Bar", uniStr, sizeof(uniStr));
    zk_printZH(500, 200, uniStr);
    zk_printZH(500, 240, uniStr);
    disable_box();
    disable_choose_image();
    end_draw();
    QiyaUpdate = 1;


}
void BrakeInfoFrameUpdate(unsigned char index)
{
    static uint32_t Qiya[2] ,Qiya_old[2];
    
	Qiya[0] = get_front_airpressure();//单位Kpa 
	Qiya[1] = get_rear_airpressure(); //单位Kpa
    
    if(Qiya[0] > 1300) Qiya[0] = 1300;
    if(Qiya[1] > 1300) Qiya[1] = 1300;
	
	start_draw();
    if((Qiya[0] != Qiya_old[0] ) || QiyaUpdate)
	{
		loc_ClearRect(400, 200, 100, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(496, 200 , Qiya[0] /10, 1, ZK_ALIGN_RIGHT);//
		Qiya_old[0] = Qiya[0];
    }
    if((Qiya[1] != Qiya_old[1] ) || QiyaUpdate)
	{
		loc_ClearRect(400, 240, 100, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(496, 240 ,  Qiya[1] /10, 1, ZK_ALIGN_RIGHT);//
		Qiya_old[1] =Qiya[1];
    }
    QiyaUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}
/** 车辆信息——高压电池电压->改为电池信息**/
void BatVolFrame(unsigned char index, unsigned char page)
{
    start_draw();
	LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printZH(212, 200, zk_oem2uniStr((uint8_t *)"高压电池电压:"));
		zk_printZH(212, 240, zk_oem2uniStr((uint8_t *)"低压电池电压:"));
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printZH(120, 200, zk_oem2uniStr((uint8_t *)"Power Battery Voltage:"));
		zk_printZH(120, 240, zk_oem2uniStr((uint8_t *)"Auxiliary Battery Voltage:"));
	}
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
    zk_printZH(500, 200, zk_oem2uniStr((uint8_t *)"V"));
    zk_printZH(500, 240, zk_oem2uniStr((uint8_t *)"V"));
    
    BatVolUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();


}
void BatVolFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static uint32_t voltage_old;
	static uint32_t low_batt_volt_old;
    static uint32_t low_batt_soc_old;
    uint32_t voltage;
    uint32_t low_batt_volt;
    uint32_t low_batt_soc;
    
	voltage = get_batt_pack_volt();
	low_batt_volt = get_aux_bat_volt();
    
    start_draw();

    if(BatVolUpdate||voltage != voltage_old)//电池电压
	{
		voltage_old = voltage;
		
		loc_ClearRect(410, 200, 90, 31);
		if(voltage > 64255)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(496-20, 200, uniStr);
		}
		else{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(496, 200, voltage, 1, ZK_ALIGN_RIGHT);
		}
    }
	
	if(BatVolUpdate||low_batt_volt != low_batt_volt_old)
	{
		low_batt_volt_old = low_batt_volt;
		
		loc_ClearRect(410, 240, 90, 31);
		if(low_batt_volt == 0xffff)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(496-20, 240, uniStr);
		}
		else{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(496, 240, low_batt_volt, 1, ZK_ALIGN_RIGHT);
		}
    }
	
	//低压锂电SOC
	if(can_getPCanRxState(0x19FF5CF3) == CAN_FRAME_ST_RECVED)
	{
		GeneralUse_t *pBMS_19FF5CF3 = (GeneralUse_t*)can_getPCanBuffer(0x19FF5CF3);
		if(pBMS_19FF5CF3->byte7.byte != 0xff)
		{
			low_batt_soc = pBMS_19FF5CF3->byte7.byte * 2 / 5;
			if(low_batt_soc > 100)
				low_batt_soc = 100;
		}
		else low_batt_soc = 0xffu;
	}
	else 
	{
		low_batt_soc = 0xffffu;
	}
	if(BatVolUpdate||low_batt_soc != low_batt_soc_old)
	{
		low_batt_soc_old = low_batt_soc;
		
		loc_ClearRect(120, 280, 480, 31);
		if(low_batt_soc != 0xffffu)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			if(eol_language == 0)
			{
				
				zk_printZH(212, 280, zk_oem2uniStr((uint8_t *)"低压锂电SOC:"));
			}
			else
			{
				zk_printZH(120, 280, zk_oem2uniStr((uint8_t *)"Auxiliary Battery SOC:"));
			}
			
			if(low_batt_soc == 0xffu)
			{
				zk_printZH(496-20, 280, zk_oem2uniStr((uint8_t *)"--"));
			}
			else
			{
				zk_printNum(496, 280, low_batt_soc, 0, ZK_ALIGN_RIGHT);
			}
			
			zk_printZH(500, 280, zk_oem2uniStr((uint8_t *)"%"));
		}
    }
	
    BatVolUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}
/** 车辆信息——供应商**/
void SupplierFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[100];
    static const msgItem_t lc_atMsgs[] = 
	{
		{{(uint8_t *)"自动空调供应商", (uint8_t *)"Air conditioner supplier"}, NULL, 0},
		{{(uint8_t *)"仪表供应商",     (uint8_t *)"IC supplier"},              NULL, 0},
		{{(uint8_t *)"仪表硬件版本",   (uint8_t *)"IC hardware version"},      NULL, 0},
		{{(uint8_t *)"仪表软件版本",   (uint8_t *)"IC software version"},      NULL, 0},
		
    };
#define TABLE_MSG_COUNT                 (sizeof(lc_atMsgs)/sizeof(msgItem_t))
#define TBL_MSG_DISPLAY                 (tblTxt.columnCnt * tblTxt.rowCnt)
    static const tblText_t tblTxt = {
		.columnCnt = 1,              // 列数
		.rowCnt = 8,                 // 行数
		.columnWidth = 396,          // 列宽
		.rowHeight = 34,             // 行高
		.msg = lc_atMsgs,            // 消息
		.msgCnt = TABLE_MSG_COUNT,   // 消息条数
		.font = &GUI_Fontwryh31,  // 字体
		.color = VM_COLOR_WHITE      // 颜色
    };
    uint8_t  version;
	
	uint8_t  *SupplerText[2] = {(uint8_t *)"方堰", (uint8_t *)"FangYan"};
	
    start_draw();
	
	LCD_Clear();
	
	/* 绘制表格 */
    vw_drawTable(4, 100, 2, 9, 792, 308, VM_COLOR_GREEN);
    /* 表格文字 */
    vw_drawTblText(8, 100, 0, &tblTxt);

    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
    zk_oem2uni(SupplerText[eol_language], uniStr, sizeof(uniStr));
    zk_printZH1(394, 101+34*1, uniStr, ZK_ALIGN_RIGHT);
	
    version = read_HwVersion();
    if(version == 0)
    	zk_oem2uni((uint8_t *)"V00.01", uniStr, sizeof(uniStr));
    else if(version == 1)
    	zk_oem2uni((uint8_t *)"V00.02", uniStr, sizeof(uniStr));
    else if(version == 2)
    	zk_oem2uni((uint8_t *)"V00.03", uniStr, sizeof(uniStr));
    else if(version == 3)
		zk_oem2uni((uint8_t *)"V00.04", uniStr, sizeof(uniStr));
    else if(version == 4)
    	zk_oem2uni((uint8_t *)"V00.05", uniStr, sizeof(uniStr));
    else if(version == 5)
    	zk_oem2uni((uint8_t *)"V00.06", uniStr, sizeof(uniStr));
    else if(version == 6)
    	zk_oem2uni((uint8_t *)"V00.07", uniStr, sizeof(uniStr));
    else;
    zk_printZH1(394, 101+34*2, uniStr, ZK_ALIGN_RIGHT);
	
	zk_oem2uni((uint8_t *)ICU_VERSIONS, uniStr, sizeof(uniStr));//仪表软件版本
    zk_printZH1(394, 101+34*3, uniStr, ZK_ALIGN_RIGHT);
	
    disable_box();
    disable_choose_image();
    end_draw();
	
	
}


/** 车辆信息——查看实时故障**/
uint32_t faultcnt = 0;
void DMFrame(unsigned char index, unsigned char page)
{
	uint8_t  uniStr[64];
	uint16_t width;
	
    start_draw();
	LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"故障信息查询", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"故障信息查询");
		zk_printZH(400 - width / 2, 90, uniStr);
		zk_oem2uni((uint8_t *)"故障描述", uniStr, sizeof(uniStr));
		zk_printZH(100, 152+80, uniStr);
		
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"SPN", uniStr, sizeof(uniStr));
		zk_printZH(100, 150, uniStr);
		zk_oem2uni((uint8_t *)"FMI", uniStr, sizeof(uniStr));
		zk_printZH(220, 150, uniStr);
		
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"故障总数", uniStr, sizeof(uniStr));
		zk_printZH(100, 380, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Fault Information Query", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"Fault Information Query");
		zk_printZH(400 - width / 2, 90, uniStr);
		zk_oem2uni((uint8_t *)"Error description", uniStr, sizeof(uniStr));
		zk_printZH(100, 152+80, uniStr);
		
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"SPN", uniStr, sizeof(uniStr));
		zk_printZH(100, 150, uniStr);
		zk_oem2uni((uint8_t *)"FMI", uniStr, sizeof(uniStr));
		zk_printZH(220, 150, uniStr);
		
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Error Sum", uniStr, sizeof(uniStr));
		zk_printZH(100, 380, uniStr);
	}
	
    dmUpdate = 1;
	
    disable_box();
    disable_choose_image();
    end_draw();


}
void DMFrameUpdate(unsigned char index)
{
    uint8_t uniStr[100];
    static uint32_t al_index = 0;
    static uint8_t faulttotalnum = 0;
    static uint8_t faulttotalnumold = 0;
	static uint32_t tempspn_old = 0;
	
	if(TotalDM == 1)
	{
		for(uint32_t cnt = 0; cnt < DM1_Ftl_Sum; cnt++)
		{
			if(DMPackage[cnt].set == 1)
			{
				if(tempspn_old != cnt)
				{
					tempspn_old = cnt; //find spn num
					alarm_update_flag = 0;
					dmUpdate = 1;
				}
				break;
			}
		}
	}
	
    start_draw();
	
    faulttotalnum = TotalDM;
    if(dmUpdate||faulttotalnumold != faulttotalnum)
	{
		loc_ClearRect(220, 380, 60, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(220, 380 , faulttotalnum, 0, ZK_ALIGN_LEFT);//
		faulttotalnumold = faulttotalnum;
		if(faulttotalnum==0)
			loc_ClearRect(100, 152+120, 700, 31);
		dmUpdate = 0;
    }

    if((alarm_update_flag == 0)||(0x01 == faultcodeupdate))
	{
		if(0x01 == faultcodeupdate)
		{
			faultcodeupdate = 0x02;			
		}
		
		for(; al_index < DM1_Ftl_Sum; al_index++)
		{
			if( DMPackage[al_index].set == 1)
			{
				loc_ClearRect(100, 152+40, 700, 31);
				loc_ClearRect(100, 152+120, 700, 31);
				
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_printNum(100, 152+40 , DMPackage[al_index].spn, 0, ZK_ALIGN_LEFT);
				zk_printNum(220, 152+40 , DMPackage[al_index].fmi , 0, ZK_ALIGN_LEFT);
				
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_RED, 0);
				zk_oem2uni(DMPackage[al_index].msg[eol_language], uniStr, sizeof(uniStr));
				zk_printZH(100, 152+120, uniStr);
				
				alarm_update_flag = 1;
				if(al_index<DM1_Ftl_Sum)
					al_index++;
				else 
					al_index = 0;
				break;
			}
			else
			{
				if(al_index<DM1_Ftl_Sum)
					al_index++;
				else 
					al_index = 0;
				if( DMPackage[al_index].set == 1)
					break;
			}
		}
		if(al_index >= DM1_Ftl_Sum)
			al_index = 0;
    }
    end_draw();

    if (key_up)  //子菜单切换
	{
		key_up = 0;
		f_index = 0;
		frmID = Menu_VehicleInfo;       
    }

    if (key_set)
	{
		key_set = 0;
		f_index = 0;
		frmID = Menu_VehicleInfo;                   
    }

    if (key_AV)
	{
		key_AV = 0;
		f_index = 0;
		frmID = Menu_VehicleInfo;             
    }
}
/** 车辆信息——轴荷信息**/
void ZhouHeFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[64];
	
	TEXT_LIST_t AxleText[] =
	{
		{ 0, {(uint8_t *)"前桥轴荷(T):      --", (uint8_t *)"Front axle load :"},  280, 160 + 31 * 0, NULL},
		{ 1, {(uint8_t *)"中桥轴荷(T):      --", (uint8_t *)"Middle axle load :"}, 280, 160 + 31 * 1, NULL},
		{ 2, {(uint8_t *)"后桥轴荷(T):      --", (uint8_t *)"Rear axle load :"},   280, 160 + 31 * 2, NULL},
		
	};
	
    start_draw();
	LCD_Clear();
	for(uint8_t i = 0; i < 3; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni(AxleText[i].Text[eol_language], uniStr, sizeof(uniStr));
		zk_printZH1(AxleText[i].Pos_X,  AxleText[i].Pos_Y, uniStr, ZK_ALIGN_RIGHT);
		
		if(NULL != AxleText[i].Unit)
		{
			zk_oem2uni(AxleText[i].Unit, uniStr, sizeof(uniStr));
			zk_printZH(AxleText[i].Pos_X + 125,  AxleText[i].Pos_Y, uniStr);
		}
	}
	
    disable_box();
    disable_choose_image();
    end_draw();


}
void ZhouHeFrameUpdate(unsigned char index)
{
    // uint8_t uniStr[64];
	
    start_draw();

	GeneralUse_t *Gu_0x18FEEA2F = NULL;
    Gu_0x18FEEA2F =  (GeneralUse_t*)can_getBCanBuffer(0x18FEEA2F);

    uint32_t temp = (Gu_0x18FEEA2F->byte2.byte + Gu_0x18FEEA2F->byte3.byte * 256) * 5 / 1000;

	if((0x05 == eol_ecastype) || (0x03 == eol_ecastype))
	{
		if(0x0F == Gu_0x18FEEA2F->byte1.byte)
		{
			loc_ClearRect(300, 160, 100, 31);
		    zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(300, 160,temp, 1, ZK_ALIGN_LEFT);
		}
		else if(0x1F == Gu_0x18FEEA2F->byte1.byte)
		{
			loc_ClearRect(300, 190, 100, 31);
		    zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(300, 190,temp, 1, ZK_ALIGN_LEFT);
		}
		else if(0x2F == Gu_0x18FEEA2F->byte1.byte)
		{
			loc_ClearRect(300, 220, 100, 31);
		    zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(300, 220,temp, 1, ZK_ALIGN_LEFT);		
		}
		else{}
	}
	else if(0x04 == eol_ecastype)
	{	
		if((0x0F == Gu_0x18FEEA2F->byte1.byte)&&(0xFFFF != temp))
		{
			loc_ClearRect(300, 160, 100, 31);
		    zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(300, 160,temp, 1, ZK_ALIGN_LEFT);
		}
		else if((0x1F == Gu_0x18FEEA2F->byte1.byte)&&(0xFFFF != temp))
		{
			loc_ClearRect(300, 190, 100, 31);
		    zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(300, 190,temp, 1, ZK_ALIGN_LEFT);
		}
		else if((0x2F == Gu_0x18FEEA2F->byte1.byte)&&(0xFFFF != temp))
		{
			loc_ClearRect(300, 220, 100, 31);
		    zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(300, 220,temp, 1, ZK_ALIGN_LEFT);		
		}
		else{}
	}
	else{}
	
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}
/** 车辆信息——电机电压**/
void MotorVolFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[64];

    start_draw();
	LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"电机电压:", uniStr, sizeof(uniStr));
		zk_printZH(260, 200, uniStr);
		zk_oem2uni((uint8_t *)"V", uniStr, sizeof(uniStr));
		zk_printZH(500, 200, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Motor Volt:", uniStr, sizeof(uniStr));
		zk_printZH(220, 200, uniStr);
		zk_oem2uni((uint8_t *)"V", uniStr, sizeof(uniStr));
		zk_printZH(500, 200, uniStr);
	}
	
    MotorVolUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();


}
void MotorVolFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static uint32_t linevol,linevolold;
	
    linevol = get_mot_volt();

    start_draw();

    if(MotorVolUpdate||linevol != linevolold) //电机电压
	{
		if(linevol == 0xffffu)
		{
			loc_ClearRect(400, 200, 100, 31);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(496-20, 200, uniStr);
		}
		else
		{
			loc_ClearRect(400, 200, 100, 31);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(496, 200,linevol, 1, ZK_ALIGN_RIGHT);//v
		}
		linevolold = linevol;
		
    }


    MotorVolUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}
/** 车辆信息——查看控制器版本**/
void ControllerVersionFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[100];
    
    static const msgItem_t lc_atMsgs[] =
	{
		{{(uint8_t *)"仪表硬件版本", (uint8_t *)"IC hardware version"}, NULL, 0},
		{{(uint8_t *)"仪表软件版本", (uint8_t *)"IC software version"}, NULL, 0},
		{{(uint8_t *)"仪表软件时间", (uint8_t *)"IC software time"},    NULL, 0},
		{{(uint8_t *)"仪表零件号",   (uint8_t *)"IC Part numbers"},     NULL, 0},
		{{(uint8_t *)"仪表Boot版本", (uint8_t *)"IC bootloader version"},     NULL, 0},
		
    };
	#define TABLE_MSG_COUNT		(sizeof(lc_atMsgs)/sizeof(msgItem_t))
	#define TBL_MSG_DISPLAY		(tblTxt.columnCnt * tblTxt.rowCnt)
    static const tblText_t tblTxt = {
	.columnCnt = 2,         	// 列数
	.rowCnt = 8,           		// 行数
	.columnWidth = 396,			// 列宽
	.rowHeight = 34,        	// 行高
	.msg = lc_atMsgs,       	// 消息
	.msgCnt = TABLE_MSG_COUNT,	// 消息条数
	.font = &GUI_Fontwryh31,	// 字体
	.color = VM_COLOR_WHITE		// 颜色
    };
    
    uint8_t version;
    
    start_draw();
	
    LCD_Clear();
	
    /* 绘制表格 */
    vw_drawTable(4, 100, 2, 9, 792, 308, VM_COLOR_GREEN);
    /* 表格文字 */
    vw_drawTblText(8, 100, 0, &tblTxt);

    version = read_HwVersion();
	
	zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
    if(version == 0)
    	zk_oem2uni((uint8_t *)"V00.01", uniStr, sizeof(uniStr));
    else if(version == 1)
    	zk_oem2uni((uint8_t *)"V00.02", uniStr, sizeof(uniStr));
    else if(version == 2)
    	zk_oem2uni((uint8_t *)"V00.03", uniStr, sizeof(uniStr));
    else if(version == 3)
		zk_oem2uni((uint8_t *)"V00.04", uniStr, sizeof(uniStr));
    else if(version == 4)
    	zk_oem2uni((uint8_t *)"V00.05", uniStr, sizeof(uniStr));
    else if(version == 5)
    	zk_oem2uni((uint8_t *)"V00.06", uniStr, sizeof(uniStr));
    else if(version == 6)
    	zk_oem2uni((uint8_t *)"V00.07", uniStr, sizeof(uniStr));
    else;
    zk_printZH1(394, 101+34*0, uniStr, ZK_ALIGN_RIGHT);
	
	
	zk_oem2uni((uint8_t *)ICU_VERSIONS, uniStr, sizeof(uniStr));//仪表软件版本
    zk_printZH1(394, 101+34*1, uniStr, ZK_ALIGN_RIGHT);
	zk_printNum(394, 101+34*2, VERSIONS_DATE, 0, ZK_ALIGN_RIGHT);
	
	if(eol_icutype == 0)
		zk_oem2uni((uint8_t *)"3801040-C5104", uniStr, sizeof(uniStr));
	else if(eol_icutype == 1)
		zk_oem2uni((uint8_t *)"3801040-C3104", uniStr, sizeof(uniStr));
	else if(eol_icutype == 2)
		zk_oem2uni((uint8_t *)"3801040-C3106", uniStr, sizeof(uniStr));
	else if(eol_icutype == 3)
		zk_oem2uni((uint8_t *)"3801050-C3108", uniStr, sizeof(uniStr));
	else ;
	zk_printZH1(394, 101+34*3, uniStr, ZK_ALIGN_RIGHT);
	
	if(BootVer[1] == 0x30 && BootVer[2] >= 0x34)
	{
		zk_oem2uni((uint8_t *)BootVer, uniStr, sizeof(uniStr));
		zk_printZH1(394, 101+34*4, uniStr, ZK_ALIGN_RIGHT);
	}
	
    disable_box();
    disable_choose_image();
    end_draw();
	
}
void ControllerVersionFrameUpdate(unsigned char index)
{
    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}

/** 车辆信息——燃料电池系统工作时长**/
void FCUFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[64];
	
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"燃料电池系统工作时长:", uniStr, sizeof(uniStr));
		zk_printZH(200, 200, uniStr);
		zk_oem2uni((uint8_t *)"h", uniStr, sizeof(uniStr));
		zk_printZH(600, 200, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"FCU Working hours :", uniStr, sizeof(uniStr));
		zk_printZH(200, 200, uniStr);
		zk_oem2uni((uint8_t *)"h", uniStr, sizeof(uniStr));
		zk_printZH(600, 200, uniStr);
	}
	
    FCUWrkTimeUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();


}
void FCUFrameUpdate(unsigned char index)
{
    static uint32_t buff_old;
	
	uint32_t buff = get_fcu_work_time();
    
    start_draw();

    if(FCUWrkTimeUpdate || buff_old != buff)
	{
		buff_old = buff;
		
		loc_ClearRect(595 - 100, 200, 100, 31);
		if(buff >= 0xfffffe)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printZH1(595, 200, zk_oem2uniStr((uint8_t *)"--"), ZK_ALIGN_RIGHT);
		}
		else
		{
			if(buff > 1500000)buff = 1500000;
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(595, 200, buff, 1, ZK_ALIGN_RIGHT);
		}
    }
	
    FCUWrkTimeUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}
/** 车辆信息——车载氢系统最高压力**/
void HMSFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[64];
	
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"车载氢系统最高压力:", uniStr, sizeof(uniStr));
		zk_printZH(200, 200, uniStr);
		zk_oem2uni((uint8_t *)"Mpa", uniStr, sizeof(uniStr));
		zk_printZH(600, 200, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Maximum hydrogen pressure :", uniStr, sizeof(uniStr));
		zk_printZH(150, 200, uniStr);
		zk_oem2uni((uint8_t *)"Mpa", uniStr, sizeof(uniStr));
		zk_printZH(600, 200, uniStr);
	}
	
    H2MaxPressUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();


}
void HMSFrameUpdate(unsigned char index)
{
    static uint16_t buff_old;
	
	uint16_t buff = get_max_h2_pressure();
    
    start_draw();

    if(H2MaxPressUpdate || buff_old != buff)
	{
		buff_old = buff;
		
		loc_ClearRect(595 - 70, 200, 70, 31);
		if(buff >= 0xfffe)
		{
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printZH1(595, 200, zk_oem2uniStr((uint8_t *)"--"), ZK_ALIGN_RIGHT);
		}
		else
		{
			if(buff > 10000)buff = 10000;
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(595, 200, buff, 1, ZK_ALIGN_RIGHT);
		}
    }
	
    H2MaxPressUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_VehicleInfo;             
    }
}




/** 行程信息菜单 **/
void MenuTripInfoFrame(void)
{
    uint8_t uniStr[64];
    uint16_t width;
	
	TEXT_LIST_t TripInfoMenu[] =
	{
		{ 0, {(uint8_t *)"查看行程信息", (uint8_t *)"Travel info detail"}, 295, 130 + 55 * 0, NULL},
		{ 1, {(uint8_t *)"日期及时间",   (uint8_t *)"Time and date"     }, 295, 130 + 55 * 1, NULL},
		// { 3, {(uint8_t *)"工作时间",     (uint8_t *)"working time"      }, 295, 130 + 55 * 3, NULL},
		
	};
	
    start_draw();
    if(!Menu_TripInfo_f)
	{
		LCD_Clear();
		
		for(uint8_t i = 0; i < MAJ_MENU_ITEM_SUM; i++)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_oem2uni(MajMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
			width = zk_getTextWidth(MajMenu[i].Text[eol_language]);
			zk_printZH(MajMenu[i].Pos_X - width / 2,  MajMenu[i].Pos_Y, uniStr);
		}
		
		for(uint8_t i = 0; i < 2; i++)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni(TripInfoMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
			zk_printZH(TripInfoMenu[i].Pos_X,  TripInfoMenu[i].Pos_Y, uniStr);
		}
		
    }
    update_box_location(22,124+70*1);
    disable_choose_image();
    Menu_VehicleInfo_f = 0;
    //Menu_TripInfo_f = 0;
    Menu_PSNInfo_f = 0;
    Menu_IC_f = 0;
	
    end_draw();

}
void MenuTripInfoUpdate(void)
{
    static unsigned char i = 8 ;

    if(!Menu_TripInfo_f)
	{
		if (key_up) //箭头向下走
		{
			key_up = 0;
			i++;
		}
		
		if (key_AV) //箭头向上走
		{
			key_AV = 0;
			i--;
		}
		if(i == 6 ||i == 10)
			i = 8;
		
		//子菜单间切换
		if (i == 8)
		{
			update_box_location(22,124+70);
			if (key_set)
			{
				key_set = 0;
				if (time_flag == 0)
				{
					f_index = 0;
					frmID = Menu_TripInfo;
					Menu_TripInfo_f = 1;
					i=8;
				}
				time_flag = 0;
			}
		}   
		
		if (i == 9 )
		{
			update_box_location(22,124+70*2);
			f_index = 0;
			i = 8;
			frmID = Menu_PSNInfo;
		}
		
		if (i == 7)
		{
			update_box_location(22,124);
			f_index = 0;
			i = 8;
			frmID = Menu_VehicleInfo;
		}
    }
    else
	{
		if (key_up) //箭头向下走
		{
			key_up = 0;
			i--;
		}
		
		if (key_AV) //箭头向上走
		{
			key_AV = 0;
			i++;
		}
		
		if(i == 6) // 3  --5单   5-3单 4  4单  6 2单
			i = 8;
		if(i == 9)
			i = 7; // 4  -6
		
		//子菜单间切换
		if (i == 8)
		{
			update_choose_location(270,134+55*0);
			if (key_set)
			{
				key_set = 0;
				if (time_flag == 0)
				{
					f_index = 0;
					frmID = AveSpeed;//PowerCons;//查看信息
					i = 8;
				}
				time_flag = 0;
			}
		} 
		
		if (i == 7)
		{
			update_choose_location(270,134+55*1);
			if (key_set)
			{
				key_set = 0;
				if (time_flag == 0)
				{
					f_index = 0;
					frmID = DateTime ;
					i = 8;
				}
				time_flag = 0;
			}
		}
		
		if ( i == 6)
		{
			update_choose_location(270,134+55*2);
			if (key_set)
			{
				key_set = 0;
				if (time_flag == 0)
				{
					f_index = 0;
					frmID = Trip;
					i = 8;
				}
				time_flag = 0;
			}
		}
		
		if ( i == 5)
		{
			update_choose_location(270,134+55*3);
			if (key_set)
			{
				key_set = 0;
				if (time_flag == 0)
				{
					f_index = 0;
					frmID = WorkingTime;
					i = 8;
				}
				time_flag = 0;
			}
		}
		
		if ( i == 4)
		{
			update_choose_location(270,134+55*4);
			if (key_set)
			{
				key_set = 0;
				if (time_flag == 0)
				{
					f_index = 0;
					frmID =  AveSpeed;
					i = 8;
				}
				time_flag = 0;
			}
		}
		
    }

}
void frmMenuTripInfo(void)
{
    if (f_index == 0) { //只更新一次
        f_index = 1;
		MenuTripInfoFrame();
    } else if (f_index == 1) {
        f_index = 1;
        MenuTripInfoUpdate();
    } else {
        f_index = 0;
    }
    if (key_long_set) {
        key_long_set = 0;
        key_set = 0;
        key_up = 0;
        key_AV = 0;
    }
	
    if (key_duble) {
		key_duble = 0;
		TESTMODE = 1;
		if (time_flag == 0) {
			f_index = 0;
			frmID = 7;
		}
    }
    if (key_clear) {//返回主界面
		key_clear = 0;
		frmID = 0;
		f_index = 0;
		Index = 0;
		index_page = 1;
		alarm_update_flag = 0;
		return_from_menu = 1;
		//upgo = 1;
    }
}
/** 行程信息——查看里程信息 **/
void TripInfoFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[100];
    TEXT_LIST_t TripText[] =
	{
		{ 0, {(uint8_t *)"里程信息",  (uint8_t *)"Mileage information"}, 300, 160 + 40 * 0, NULL},
		{ 1, {(uint8_t *)"总里程:",   (uint8_t *)"Total mileage :"},     300, 160 + 40 * 1, (uint8_t *)"Km"},
		{ 2, {(uint8_t *)"小计里程:", (uint8_t *)"Subtotal mileage :"},  300, 160 + 40 * 2, (uint8_t *)"Km"},
		
	};
	
    start_draw();
    LCD_Clear();
	for(uint8_t i = 0; i < 3; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni(TripText[i].Text[eol_language], uniStr, sizeof(uniStr));
		zk_printZH1(TripText[i].Pos_X,  TripText[i].Pos_Y, uniStr, ZK_ALIGN_RIGHT);
		
		if(NULL != TripText[i].Unit)
		{
			zk_oem2uni(TripText[i].Unit, uniStr, sizeof(uniStr));
			zk_printZH(TripText[i].Pos_X + 200,  TripText[i].Pos_Y, uniStr);
		}
	}
	
    TripInfoUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();
}
void TripInfoFrameUpdate(unsigned char index)
{
    // uint8_t uniStr[100];
    static uint32_t trip[2];
    start_draw();
    if((Miles.total_miles != trip[0]  ) ||TripInfoUpdate){

	loc_ClearRect(400, 200, 100, 31);
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	zk_printNum(496, 200 , Miles.total_miles, 1, ZK_ALIGN_RIGHT);//
	trip[0] = Miles.total_miles;
    }
    if((Miles.single_miles != trip[1]  ) ||TripInfoUpdate){

	loc_ClearRect(400, 240, 100, 31);
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	zk_printNum(496, 240 , Miles.single_miles, 1, ZK_ALIGN_RIGHT);//
	trip[1] =Miles.single_miles;

    }
    TripInfoUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_TripInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_TripInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_TripInfo;             
    }
}
/** 行程信息——查看行程信息 **/
void AveSpeedFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[100];
	uint16_t width;
	uint8_t* Title[2] = {(uint8_t *)"行程信息", (uint8_t *)"Travel info"};
	TEXT_LIST_t TravelText[] =
	{
		{ 0, {(uint8_t *)"续航里程",     (uint8_t *)"Driving range"},     300, 140 + 31 * 0, (uint8_t *)"Km"},
		{ 1, {(uint8_t *)"剩余功率",     (uint8_t *)"Residual Power"},    300, 140 + 31 * 1, (uint8_t *)"kW"},
		{ 2, {(uint8_t *)"累计电耗",     (uint8_t *)"Total Power Cons."}, 300, 140 + 31 * 2, (uint8_t *)"kWh"},
		{ 3, {(uint8_t *)"累计里程",     (uint8_t *)"Total Mileage"},     300, 140 + 31 * 3, (uint8_t *)"Km"},
		{ 4, {(uint8_t *)"电池平均温度", (uint8_t *)"Avg Batt Temp"},     300, 140 + 31 * 4, (uint8_t *)"℃"},
		{ 5, {(uint8_t *)"平均车速",     (uint8_t *)"Avg speed"},         300, 140 + 31 * 5, (uint8_t *)"km/h"},
		
	};
	
    start_draw();
    LCD_Clear();
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
	zk_oem2uni(Title[eol_language], uniStr, sizeof(uniStr));
	width = zk_getTextWidth(Title[eol_language]);
	zk_printZH(400 - width / 2,  20, uniStr);
	
	
	for(uint8_t i = 0; i < 6; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni(TravelText[i].Text[eol_language], uniStr, sizeof(uniStr));
		zk_printZH1(TravelText[i].Pos_X,  TravelText[i].Pos_Y, uniStr, ZK_ALIGN_RIGHT);
		
		if(NULL != TravelText[i].Unit)
		{
			zk_oem2uni(TravelText[i].Unit, uniStr, sizeof(uniStr));
			zk_printZH(TravelText[i].Pos_X + 204,  TravelText[i].Pos_Y, uniStr);
		}
	}
	
    AveSpeedUpdate = 1;
    disable_box();
    disable_choose_image();
	
    end_draw();


}
void AveSpeedFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static uint16_t leftpower,leftpower_old;
    static uint16_t extendmiles,extendmiles_old;
    static uint32_t totalpower,totalpower_old;
    static int16_t  avetemp,avetemp_old;
	static uint8_t  Average_VSpeed,old_Average_VSpeed;
	
	leftpower = get_residual_power();     //剩余功率
	extendmiles = get_driving_range();    //可行驶里程
	totalpower = get_total_power_consp(); //累计电耗
	avetemp = get_batt_pack_avg_tmp();    //电池平均温度
	
	start_draw();
	
	if(AveSpeedUpdate || (extendmiles != extendmiles_old)) //续航里程
	{
	    start_draw();
	    if(extendmiles == 0xffffu)
	    {
			loc_ClearRect(500 - 80, 140, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(500-20, 140, uniStr);
	    }
	    else
		{
			loc_ClearRect(500 - 80, 140, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(500, 140,extendmiles, 0, ZK_ALIGN_RIGHT);//v
	    }
	    end_draw();
	    extendmiles_old = extendmiles;
	}
	if(AveSpeedUpdate || (leftpower != leftpower_old)) //剩余功率
	{
	    start_draw();
	    if(leftpower == 0xffffu)
	    {
			loc_ClearRect(500 - 80, 140+31, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(500-20, 140+31, uniStr);
	    }
	    else
		{
			loc_ClearRect(500 - 80, 140+31, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(500, 140+31,leftpower, 0, ZK_ALIGN_RIGHT);//v
	    }
	    end_draw();
	    leftpower_old = leftpower;
	}
	if(AveSpeedUpdate || (totalpower != totalpower_old)) //累计电耗
	{
	    start_draw();
	    if(totalpower == 0xffffffffu)
	    {
			loc_ClearRect(500 - 80, 140+31*2, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(500-20, 140+31*2, uniStr);
	    }
	    else
		{
			loc_ClearRect(500 - 80,140+31*2, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printPosNum(500, 140+31*2,totalpower, 0, ZK_ALIGN_RIGHT);//v
	    }
	    end_draw();
	    totalpower_old = totalpower;
	}
	if(AveSpeedUpdate) //总里程
	{
	    start_draw();
	  
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(500, 140+31*3,Miles.total_miles/10, 0, ZK_ALIGN_RIGHT);//v
	    
	    end_draw();
	}
	if(AveSpeedUpdate||((avetemp != avetemp_old) ))//电池平均温度
	{
	    start_draw();
	    if(avetemp == 0xffu)
	    {
			loc_ClearRect(500 - 80, 140+31*4, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(500-20, 140+31*4, uniStr);
	    }
	    else
		{
			loc_ClearRect(500 - 80, 140+31*4, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(500, 140+31*4,avetemp, 0, ZK_ALIGN_RIGHT);//v
	    }
	    end_draw();
	    avetemp_old = avetemp;
	}
	
	Average_VSpeed=(double)Miles.single_miles/driving_time*360;//单位比例0.1km/h
	if(AveSpeedUpdate||old_Average_VSpeed != Average_VSpeed)//平均车速
	{
	    start_draw();
	    if(Miles.single_miles < 10)
	    {
			loc_ClearRect(500 - 80, 140+31*5, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"--", uniStr, sizeof(uniStr));
			zk_printZH(500-20, 140+31*5, uniStr);
	    }
	    else
		{
			loc_ClearRect(500 - 80, 140+31*5, 80, 32);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printNum(500, 140+31*5,Average_VSpeed, 0, ZK_ALIGN_RIGHT);
	    }
	    end_draw();
	    old_Average_VSpeed = Average_VSpeed;
	}
	
    AveSpeedUpdate = 0;
    end_draw();

    if (key_up)  //子菜单切换
	{
		key_up = 0;
		f_index = 0;
		frmID = Menu_TripInfo;       
    }

    if (key_set)
	{
		key_set = 0;
		f_index = 0;
		frmID = Menu_TripInfo;                   
    }

    if (key_AV)
	{
		key_AV = 0;
		f_index = 0;
		frmID = Menu_TripInfo;             
    }
}
/** 行程信息——日期及时间 **/
#define OFFESTTime 50
void TimeFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[100];
	
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		
		zk_oem2uni((uint8_t *)"日期:", uniStr, sizeof(uniStr));
		zk_printZH(260 - OFFESTTime, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"年", uniStr, sizeof(uniStr));
		zk_printZH(260+150 - OFFESTTime, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"月", uniStr, sizeof(uniStr));
		zk_printZH(260+150+70 - OFFESTTime, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"日", uniStr, sizeof(uniStr));
		zk_printZH(260+150+70+70 - OFFESTTime, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"星期", uniStr, sizeof(uniStr));
		zk_printZH(260+150+70+70+30 - OFFESTTime, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"时间:", uniStr, sizeof(uniStr));
		zk_printZH(260 - OFFESTTime, 240, uniStr);
		
		zk_oem2uni((uint8_t *)"时", uniStr, sizeof(uniStr));
		zk_printZH(260+150 - OFFESTTime, 240, uniStr);
		
		zk_oem2uni((uint8_t *)"分", uniStr, sizeof(uniStr));
		zk_printZH(260+150+70 - OFFESTTime, 240, uniStr);
		
		zk_oem2uni((uint8_t *)"秒", uniStr, sizeof(uniStr));
		zk_printZH(260+150+70+70 - OFFESTTime, 240, uniStr);
		
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		
		zk_oem2uni((uint8_t *)"Date:", uniStr, sizeof(uniStr));
		zk_printZH(210, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
		zk_printZH(380 + 16 * 2 + 3, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"/", uniStr, sizeof(uniStr));
		zk_printZH(380 + 16 * 5 + 3, 200, uniStr);
		
		zk_oem2uni((uint8_t *)"Time:", uniStr, sizeof(uniStr));
		zk_printZH(210, 240, uniStr);
		
		zk_oem2uni((uint8_t *)":", uniStr, sizeof(uniStr));
		zk_printZH(300 + 16 * 2 + 3, 240, uniStr);
		
		zk_oem2uni((uint8_t *)":", uniStr, sizeof(uniStr));
		zk_printZH(300 + 16 * 5 + 3, 240, uniStr);
		
	}
	
    TimeDateUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();


}
void TimeFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static DATE_t timer_old;
	
    const unsigned char *WeekText[7][2] = {
		{(uint8_t *)"日", (uint8_t *)"Sun."},
		{(uint8_t *)"一", (uint8_t *)"Mon."},
		{(uint8_t *)"二", (uint8_t *)"Tue."},
		{(uint8_t *)"三", (uint8_t *)"Wed."},
		{(uint8_t *)"四", (uint8_t *)"Thu."},
		{(uint8_t *)"五", (uint8_t *)"Fri."},
		{(uint8_t *)"六", (uint8_t *)"Sat."}
	};
	
	uint16_t year_temp = 0x00;
	uint8_t  month_temp = 0x00;
	uint8_t  day_temp = 0x00;
	uint8_t  week_temp = 0x00;
	uint8_t  hour_temp = 0x00;
	uint8_t  minute_temp = 0x00;
	uint8_t  second_temp = 0x00;
	
	GeneralUse_t *GU_18FEE6EE_t =NULL;
    
    if(TD_MsgEnable != 0)
	{
		GU_18FEE6EE_t =  (GeneralUse_t*)can_getBCanBuffer(0x18FEE6EE);
		
		if(GU_18FEE6EE_t->byte3.byte <= 23U)
		{
			timer.hour = decimal_bcd(GU_18FEE6EE_t->byte3.byte);
		}
		if(GU_18FEE6EE_t->byte2.byte <= 59U)
		{
			timer.minute= decimal_bcd(GU_18FEE6EE_t->byte2.byte);
		}
		if(GU_18FEE6EE_t->byte1.byte <= 239U)
		{
			timer.second= decimal_bcd(GU_18FEE6EE_t->byte1.byte/4U);
		}

		if((GU_18FEE6EE_t->byte6.byte + 1985) >= 2000U)//2000年
		{
			timer.year = decimal_bcd((uint8_t)(GU_18FEE6EE_t->byte6.byte + 1985 - 2000U));
		}
		else
		{
			timer.year = decimal_bcd((uint8_t)(GU_18FEE6EE_t->byte6.byte + 1985 - 1900U));//1985年到1999年
		}
		
		if(GU_18FEE6EE_t->byte4.byte <= 12U)
		{
			timer.month= decimal_bcd(GU_18FEE6EE_t->byte4.byte);
		}

		if(GU_18FEE6EE_t->byte5.byte <= 124)
		{
			timer.day= decimal_bcd(GU_18FEE6EE_t->byte5.byte/4);
		}
	
    }
	
	if(timer.year >= 0x85)
	{
		year_temp = bcd2decimal(timer.year) + 1900;
	}
	else
	{
		year_temp = bcd2decimal(timer.year) + 2000;
	}
	month_temp = bcd2decimal(timer.month);
	day_temp = bcd2decimal(timer.day);
	
	hour_temp = bcd2decimal(timer.hour);
	minute_temp = bcd2decimal(timer.minute);
	second_temp = bcd2decimal(timer.second);
	
    /* 使用公式来计算日期对应的星期 */
	week_temp = CaculateWeekDay(year_temp,month_temp, day_temp);
	
    if(!time_flag)
	{
		start_draw();
		
		if(eol_language == 0)
		{
			if((timer_old.year != (year_temp-2000)) ||TimeDateUpdate)
			{
				timer_old.year  = (year_temp-2000);
			    loc_ClearRect(340 - OFFESTTime, 200, 70, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_printNum(406 - OFFESTTime, 200 , year_temp, 0, ZK_ALIGN_RIGHT);
			    
			}
			
			if((timer_old.month!= month_temp) ||TimeDateUpdate)
			{
			    loc_ClearRect(435 - OFFESTTime, 200, 41, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    if(month_temp<10)
			    {
					zk_printNum(476-16 - OFFESTTime, 200 , 0, 0, ZK_ALIGN_RIGHT);
					zk_printNum(476 - OFFESTTime, 200 , month_temp, 0, ZK_ALIGN_RIGHT);
			    }
			    else
			    {
					zk_printNum(476 - OFFESTTime, 200 , month_temp, 0, ZK_ALIGN_RIGHT);
				}
			    timer_old.month  = month_temp;
			}
			
			if((timer_old.day != day_temp) ||TimeDateUpdate)
			{
			    loc_ClearRect(505 - OFFESTTime, 200, 41, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    if(day_temp<10){
				zk_printNum(546-16 - OFFESTTime, 200 , 0, 0, ZK_ALIGN_RIGHT);//
				zk_printNum(546 - OFFESTTime, 200 , day_temp, 0, ZK_ALIGN_RIGHT);//
			    }
			    else
				zk_printNum(546 - OFFESTTime, 200 , day_temp, 0, ZK_ALIGN_RIGHT);//
			    timer_old.day  = day_temp;
			}
			
			if((timer_old.week!= week_temp) ||TimeDateUpdate)
			{

			    loc_ClearRect(636 - OFFESTTime, 200, 60, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    zk_oem2uni(WeekText[week_temp][eol_language], uniStr, sizeof(uniStr));
			    zk_printZH(636 - OFFESTTime, 200,uniStr);
			    timer_old.week  = week_temp;
			}
			
			if((timer_old.hour!= hour_temp) ||TimeDateUpdate)
			{
			    loc_ClearRect(340 - OFFESTTime, 240, 70, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    if(hour_temp<10)
			    {
					zk_printNum(406-16 - OFFESTTime, 240 , 0, 0, ZK_ALIGN_RIGHT);
					zk_printNum(406 - OFFESTTime, 240 , hour_temp, 0, ZK_ALIGN_RIGHT);
			    }
			    else
			    {
					zk_printNum(406 - OFFESTTime, 240 , hour_temp, 0, ZK_ALIGN_RIGHT);
				}
			    timer_old.hour  = hour_temp;
			}
			
			if((timer_old.minute != minute_temp) ||TimeDateUpdate)
			{
			    loc_ClearRect(435 - OFFESTTime, 240, 41, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    if(minute_temp<10)
			    {
					zk_printNum(476-16 - OFFESTTime, 240 , 0, 0, ZK_ALIGN_RIGHT);//
					zk_printNum(476 - OFFESTTime, 240 , minute_temp, 0, ZK_ALIGN_RIGHT);//
			    }
			    else
			    {
					zk_printNum(476 - OFFESTTime, 240 ,minute_temp, 0, ZK_ALIGN_RIGHT);//
				}
			    timer_old.minute  = minute_temp;
			}
			
			if((timer_old.second!= second_temp) ||TimeDateUpdate)
			{
			    loc_ClearRect(505 - OFFESTTime, 240, 41, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    if(second_temp<10)
			    {
					zk_printNum(546-16 - OFFESTTime, 240 , 0, 0, ZK_ALIGN_RIGHT);//
					zk_printNum(546 - OFFESTTime, 240 , second_temp, 0, ZK_ALIGN_RIGHT);//
			    }
			    else
			    {
					zk_printNum(546 - OFFESTTime, 240 , second_temp, 0, ZK_ALIGN_RIGHT);//
				}
			    timer_old.second  = second_temp;
			}		
			
		}
		else
		{
			if((timer_old.week != week_temp) || TimeDateUpdate)
			{
				timer_old.week = week_temp;
			    loc_ClearRect(300, 200, 80, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    zk_oem2uni(WeekText[week_temp][eol_language], uniStr, sizeof(uniStr));
			    zk_printZH(300, 200, uniStr);
			    
			}
			
			if((timer_old.month != month_temp) || TimeDateUpdate)
			{
				timer_old.month = month_temp;
			    loc_ClearRect(380 + 16 * 0, 200, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_printNum(380 + 16 * 0, 200 , month_temp/10, 0, ZK_ALIGN_LEFT);
				zk_printNum(380 + 16 * 1, 200 , month_temp%10, 0, ZK_ALIGN_LEFT);
			    
			}
			
			if((timer_old.day != day_temp) ||TimeDateUpdate)
			{
				timer_old.day = day_temp;
			    loc_ClearRect(380 + 16 * 3, 200, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    zk_printNum(380 + 16 * 3, 200 , day_temp/10, 0, ZK_ALIGN_LEFT);//
				zk_printNum(380 + 16 * 4, 200 , day_temp%10, 0, ZK_ALIGN_LEFT);//
				
			}
			
			if((timer_old.year != (year_temp - 2000)) ||TimeDateUpdate)
			{
				timer_old.year  = (year_temp - 2000);
			    loc_ClearRect(380 + 16 * 6, 200, 64, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_printNum(380 + 16 * 6, 200 , year_temp, 0, ZK_ALIGN_LEFT);
			    
			}
			
			
			if((timer_old.hour!= hour_temp) ||TimeDateUpdate)
			{
				timer_old.hour = hour_temp;
			    loc_ClearRect(300 + 16 * 0, 240, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    zk_printNum(300 + 16 * 0, 240 , hour_temp/10, 0, ZK_ALIGN_LEFT);
				zk_printNum(300 + 16 * 1, 240 , hour_temp%10, 0, ZK_ALIGN_LEFT);
				
			}
			
			if((timer_old.minute != minute_temp) ||TimeDateUpdate)
			{
				timer_old.minute = minute_temp;
			    loc_ClearRect(300 + 16 * 3, 240, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			    zk_printNum(300 + 16 * 3, 240 , minute_temp/10, 0, ZK_ALIGN_LEFT);//
				zk_printNum(300 + 16 * 4, 240 , minute_temp%10, 0, ZK_ALIGN_LEFT);//
			    
			}
			
			if((timer_old.second!= second_temp) ||TimeDateUpdate)
			{
				timer_old.second = second_temp;
			    loc_ClearRect(300 + 16 * 6, 240, 41, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_printNum(300 + 16 * 6, 240 , second_temp/10, 0, ZK_ALIGN_LEFT);//
				zk_printNum(300 + 16 * 7, 240 , second_temp%10, 0, ZK_ALIGN_LEFT);//
			    
			}		
			
		}
		
		end_draw();
		
		TimeDateUpdate = 0;
		
	}
    else
   	{
		
   		const uint8_t MonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		
		start_draw();
		
		if(eol_language == 0)
		{
			//set time
			if(t == 0)
			{
				if(t_add)
				{
					t_add = 0;
					hour_temp++;
					if(hour_temp == 24)
						hour_temp = 0;
					timer.hour = decimal_bcd(hour_temp);			
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(340 - OFFESTTime, 240, 70, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(406-16 - OFFESTTime, 240 , hour_temp/10, 0, ZK_ALIGN_RIGHT);
				zk_printNum(406 - OFFESTTime, 240 , hour_temp%10, 0, ZK_ALIGN_RIGHT);
				
				tm_h = 1;
				
			}
			else
			{
				if(tm_h)
				{
					tm_h = 0;
					loc_ClearRect(340 - OFFESTTime, 240, 70, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(406-16 - OFFESTTime, 240 , hour_temp/10, 0, ZK_ALIGN_RIGHT);
					zk_printNum(406 - OFFESTTime, 240 , hour_temp%10, 0, ZK_ALIGN_RIGHT);
				}
			}
			
			//Set the minutes
			if(t == 1)
			{
				if(t_add)
				{
					t_add = 0;
					minute_temp++;
					if(minute_temp == 60)
						minute_temp = 0;
					timer.minute = decimal_bcd(minute_temp);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(435 - OFFESTTime, 240, 41, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(476-16 - OFFESTTime, 240 , minute_temp/10, 0, ZK_ALIGN_RIGHT);
				zk_printNum(476 - OFFESTTime, 240 , minute_temp%10, 0, ZK_ALIGN_RIGHT);
				
				tm_m = 1;
				
			}
			else
			{
				if(tm_m)
				{
					tm_m = 0;
					loc_ClearRect(435 - OFFESTTime, 240, 41, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
					zk_printNum(476-16 - OFFESTTime, 240 , minute_temp/10, 0, ZK_ALIGN_RIGHT);
					zk_printNum(476 - OFFESTTime, 240 , minute_temp%10, 0, ZK_ALIGN_RIGHT);
				}
			}
			
			//Set up the second
			if(t == 2) 
			{
				if(t_add)
				{
					t_add = 0;
					second_temp++;
					if(second_temp == 60)
						second_temp = 0;
					timer.minute = decimal_bcd(second_temp);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(505 - OFFESTTime, 240, 41, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(546-16 - OFFESTTime, 240 , second_temp/10, 0, ZK_ALIGN_RIGHT);
				zk_printNum(546 - OFFESTTime, 240 , second_temp%10, 0, ZK_ALIGN_RIGHT);

				tm_s = 1;
				
			}
			else
			{
				if(tm_s)
				{
					tm_s = 0;
					loc_ClearRect(505 - OFFESTTime, 240, 41, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(546-16 - OFFESTTime, 240 , second_temp/10, 0, ZK_ALIGN_RIGHT);
					zk_printNum(546 - OFFESTTime, 240 , second_temp%10, 0, ZK_ALIGN_RIGHT);
				}
			}
			
			//Set up the year
			if(t == 3) 
			{
				if(t_add)
				{
					t_add = 0;
					year_temp++;
					if(year_temp == 2100)
						year_temp = 0;
					timer.year = decimal_bcd(year_temp - 2000);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(340 - OFFESTTime, 200, 70, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(406 - OFFESTTime, 200 ,  year_temp, 0, ZK_ALIGN_RIGHT);//
				
				tm_y = 1;
				
			}
			else
			{
				if(tm_y)
				{
					tm_y = 0;
					loc_ClearRect(340 - OFFESTTime, 200, 70, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(406 - OFFESTTime, 200 ,  year_temp, 0, ZK_ALIGN_RIGHT);//
				}
			}
			
			//Set the month
			if(t == 4) 
			{
				if(t_add)
				{
					t_add = 0;
					month_temp++;
					if(month_temp == 13)
						month_temp = 0;
					timer.month = decimal_bcd(month_temp);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(435 - OFFESTTime, 200, 41, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(476-16 - OFFESTTime, 200 , month_temp/10, 0, ZK_ALIGN_RIGHT);//
				zk_printNum(476 - OFFESTTime, 200 , month_temp%10, 0, ZK_ALIGN_RIGHT);//
				
				tm_month = 1;
				
			}
			else
			{
				if(tm_month)
				{
					tm_month = 0;
					loc_ClearRect(435 - OFFESTTime, 200, 41, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(476-16 - OFFESTTime, 200 , month_temp/10, 0, ZK_ALIGN_RIGHT);//
					zk_printNum(476 - OFFESTTime, 200 , month_temp%10, 0, ZK_ALIGN_RIGHT);//
				}
			}
			
			//Set the Day
			if(t == 5) 
			{
				if(t_add)
				{
					t_add = 0;
					if(month_temp == 2)
					{
						// 普通闰年是公历年份是4的倍数的，且不是100的倍数
						// 世纪闰年是公历年份是整百数的，必须是400的倍数才是世纪闰年
						if((year_temp%4) == 0 || (year_temp%400) == 0)
						{
							if((year_temp%100) == 0)
							{
								if(28U == day_temp)
								{
									day_temp = 0x01;
								}
								else
								{
									day_temp++;
								}
							}
							else
							{
								if(29U == day_temp)
								{
									day_temp = 0x01;
								}
								else
								{
									day_temp++;
								}
							}
						}
						else
						{
							if(28U == day_temp)
							{
								day_temp = 0x01;
							}
							else
							{
								day_temp++;
							}
						}
						
					}
					else if(MonthDays[month_temp - 1] == day_temp)
					{
						day_temp = 0x01;
					}
					else
					{
						day_temp++;
					}
					timer.day = decimal_bcd(day_temp);
					External_RTC_SetTime(&timer);
				
				}
				
				loc_ClearRect(505 - OFFESTTime, 200, 41, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(546-16 - OFFESTTime, 200 , day_temp/10, 0, ZK_ALIGN_RIGHT);//
				zk_printNum(546 - OFFESTTime, 200 , day_temp%10, 0, ZK_ALIGN_RIGHT);//
				
				tm_d = 1;
				
			}
			else
			{
				if(tm_d)
				{
					tm_d = 0;
					loc_ClearRect(505 - OFFESTTime, 200, 41, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(546-16 - OFFESTTime, 200 , day_temp/10, 0, ZK_ALIGN_RIGHT);//
					zk_printNum(546 - OFFESTTime, 200 , day_temp%10, 0, ZK_ALIGN_RIGHT);//
				}
			}
			
			//Set the week
			if(t == 6) 
			{
				if(t_add)
				{
					t_add = 0;
					// week_temp
					if( (timer.week&0x0F) < 9)
						timer.week ++;
					else
					{
						timer.week += 0x10;
						timer.week &= 0xF0;
						
					}
					if ((timer.week > 0x07)||(timer.week < 0x01))
					{
						timer.week = 1;
					}
					External_RTC_SetTime(&timer);
				}
				
				loc_ClearRect(636 - OFFESTTime, 200, 41, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_oem2uni(WeekText[timer.week-1][eol_language], uniStr, sizeof(uniStr));
				zk_printZH(636 - OFFESTTime, 200,uniStr); //
				
				tm_w = 1;
				
			}
			else
			{
				if(tm_w)
				{
					tm_w = 0;
					loc_ClearRect(636 - OFFESTTime, 200, 41, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);

					zk_oem2uni(WeekText[timer.week-1][eol_language], uniStr, sizeof(uniStr));
					zk_printZH(636 - OFFESTTime, 200,uniStr);
				}
			}
			
		}
		else
		{
			//set time
			if(t == 0)
			{
				if(t_add)
				{
					t_add = 0;
					hour_temp++;
					if(hour_temp == 24)
						hour_temp = 0;
					timer.hour = decimal_bcd(hour_temp);			
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(300 + 16 * 0, 240, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
			    zk_printNum(300 + 16 * 0, 240 , hour_temp/10, 0, ZK_ALIGN_LEFT);
				zk_printNum(300 + 16 * 1, 240 , hour_temp%10, 0, ZK_ALIGN_LEFT);
				
				tm_h = 1;
				
			}
			else
			{
				if(tm_h)
				{
					tm_h = 0;
					loc_ClearRect(300 + 16 * 0, 240, 32, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(300 + 16 * 0, 240 , hour_temp/10, 0, ZK_ALIGN_LEFT);
					zk_printNum(300 + 16 * 1, 240 , hour_temp%10, 0, ZK_ALIGN_LEFT);
				}
			}
			
			//Set the minutes
			if(t == 1)
			{
				if(t_add)
				{
					t_add = 0;
					minute_temp++;
					if(minute_temp == 60)
						minute_temp = 0;
					timer.minute = decimal_bcd(minute_temp);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(300 + 16 * 3, 240, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
			    zk_printNum(300 + 16 * 3, 240 , minute_temp/10, 0, ZK_ALIGN_LEFT);//
				zk_printNum(300 + 16 * 4, 240 , minute_temp%10, 0, ZK_ALIGN_LEFT);//
				
				tm_m = 1;
				
			}
			else
			{
				if(tm_m)
				{
					tm_m = 0;
					loc_ClearRect(300 + 16 * 3, 240, 32, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(300 + 16 * 3, 240 , minute_temp/10, 0, ZK_ALIGN_LEFT);//
					zk_printNum(300 + 16 * 4, 240 , minute_temp%10, 0, ZK_ALIGN_LEFT);//
				}
			}
			
			//Set up the second
			if(t == 2) 
			{
				if(t_add)
				{
					t_add = 0;
					second_temp++;
					if(second_temp == 60)
						second_temp = 0;
					timer.minute = decimal_bcd(second_temp);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(300 + 16 * 6, 240, 41, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(300 + 16 * 6, 240 , second_temp/10, 0, ZK_ALIGN_LEFT);//
				zk_printNum(300 + 16 * 7, 240 , second_temp%10, 0, ZK_ALIGN_LEFT);//

				tm_s = 1;
				
			}
			else
			{
				if(tm_s)
				{
					tm_s = 0;
					loc_ClearRect(300 + 16 * 6, 240, 41, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(300 + 16 * 6, 240 , second_temp/10, 0, ZK_ALIGN_LEFT);//
					zk_printNum(300 + 16 * 7, 240 , second_temp%10, 0, ZK_ALIGN_LEFT);//
				}
			}
			
			//Set the month
			if(t == 3) 
			{
				if(t_add)
				{
					t_add = 0;
					month_temp++;
					if(month_temp == 13)
						month_temp = 0;
					timer.month = decimal_bcd(month_temp);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(380 + 16 * 0, 200, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(380 + 16 * 0, 200 , month_temp/10, 0, ZK_ALIGN_LEFT);
				zk_printNum(380 + 16 * 1, 200 , month_temp%10, 0, ZK_ALIGN_LEFT);
				
				tm_month = 1;
				
			}
			else
			{
				if(tm_month)
				{
					tm_month = 0;
					loc_ClearRect(380 + 16 * 0, 200, 32, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(380 + 16 * 0, 200 , month_temp/10, 0, ZK_ALIGN_LEFT);
					zk_printNum(380 + 16 * 1, 200 , month_temp%10, 0, ZK_ALIGN_LEFT);
				}
			}
			
			//Set the Day
			if(t == 4) 
			{
				if(t_add)
				{
					t_add = 0;
					if(month_temp == 2)
					{
						// 普通闰年是公历年份是4的倍数的，且不是100的倍数
						// 世纪闰年是公历年份是整百数的，必须是400的倍数才是世纪闰年
						if((year_temp%4) == 0 || (year_temp%400) == 0)
						{
							if((year_temp%100) == 0)
							{
								if(28U == day_temp)
								{
									day_temp = 0x01;
								}
								else
								{
									day_temp++;
								}
							}
							else
							{
								if(29U == day_temp)
								{
									day_temp = 0x01;
								}
								else
								{
									day_temp++;
								}
							}
						}
						else
						{
							if(28U == day_temp)
							{
								day_temp = 0x01;
							}
							else
							{
								day_temp++;
							}
						}
						
					}
					else if(MonthDays[month_temp - 1] == day_temp)
					{
						day_temp = 0x01;
					}
					else
					{
						day_temp++;
					}
					timer.day = decimal_bcd(day_temp);
					External_RTC_SetTime(&timer);
				
				}
				
				loc_ClearRect(380 + 16 * 3, 200, 32, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
			    zk_printNum(380 + 16 * 3, 200 , day_temp/10, 0, ZK_ALIGN_LEFT);//
				zk_printNum(380 + 16 * 4, 200 , day_temp%10, 0, ZK_ALIGN_LEFT);//
				
				tm_d = 1;
				
			}
			else
			{
				if(tm_d)
				{
					tm_d = 0;
					loc_ClearRect(380 + 16 * 3, 200, 32, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(380 + 16 * 3, 200 , day_temp/10, 0, ZK_ALIGN_LEFT);//
					zk_printNum(380 + 16 * 4, 200 , day_temp%10, 0, ZK_ALIGN_LEFT);//
				}
			}
			
			//Set up the year
			if(t == 5) 
			{
				if(t_add)
				{
					t_add = 0;
					year_temp++;
					if(year_temp == 2100)
						year_temp = 0;
					timer.year = decimal_bcd(year_temp - 2000);
					External_RTC_SetTime(&timer);
					
				}
				
				loc_ClearRect(380 + 16 * 6, 200, 64, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
				zk_printNum(380 + 16 * 6, 200 , year_temp, 0, ZK_ALIGN_LEFT);
				
				tm_y = 1;
				
			}
			else
			{
				if(tm_y)
				{
					tm_y = 0;
					loc_ClearRect(380 + 16 * 6, 200, 64, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_printNum(380 + 16 * 6, 200 , year_temp, 0, ZK_ALIGN_LEFT);
				}
			}
			
			//Set the week
			if(t == 6) 
			{
				if(t_add)
				{
					t_add = 0;
					// week_temp
					if( (timer.week&0x0F) < 9)
						timer.week ++;
					else
					{
						timer.week += 0x10;
						timer.week &= 0xF0;
						
					}
					if ((timer.week > 0x07)||(timer.week < 0x01))
					{
						timer.week = 1;
					}
					External_RTC_SetTime(&timer);
				}
				
				loc_ClearRect(300, 200, 80, 31);
			    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_GREEN, 0);
			    zk_oem2uni(WeekText[week_temp][eol_language], uniStr, sizeof(uniStr));
			    zk_printZH(300, 200, uniStr);
				
				tm_w = 1;
				
			}
			else
			{
				if(tm_w)
				{
					tm_w = 0;
					loc_ClearRect(300, 200, 100, 31);
					zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					zk_oem2uni(WeekText[week_temp][eol_language], uniStr, sizeof(uniStr));
					zk_printZH(300, 200, uniStr);
				}
			}
			
		}
		
		end_draw();
		
	}
	
	if (key_up) //箭头向下走
	{
		key_up = 0;
		if (time_flag) 
		{
			t++; //时钟设置
			if (t == 6)
				t = 0;
		}
	}
	
	if(key_AV)
	{
		key_AV = 0;
		if (time_flag)
			t_add = 1;
		else
			t_add = 0;
	}
	
	if(key_long_av)
	{
		key_long_av = 0;
		if (time_flag)
			t_add = 1;
		else
			t_add = 0;
	}
	
	if (key_set)
	{
		key_set = 0;
		f_index = 0;
		
		if(!time_flag)
		{
			frmID = 2; 
		}
		else
		{
			time_flag = 0; 
		}
	}
	
	if (key_long_set)
	{
		key_long_set = 0;
		key_set = 0;
		key_up = 0;
		key_AV = 0;
		
		if(!time_flag)
		{
			if(TD_MsgEnable == 0) //td有接收到则不设置时间
				time_flag = 1;
		}
		else
		{
			time_flag = 0;
		}
	}
	
	
	
}
/** 行程信息——工作时间 **/
void WorkTimeFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[64];
    
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"工作时间", uniStr, sizeof(uniStr));
		zk_printZH1(360, 200, uniStr, ZK_ALIGN_RIGHT);
		zk_oem2uni((uint8_t *)"分钟", uniStr, sizeof(uniStr));
		zk_printZH(500, 200, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"working time", uniStr, sizeof(uniStr));
		zk_printZH1(360, 200, uniStr, ZK_ALIGN_RIGHT);
		zk_oem2uni((uint8_t *)"min", uniStr, sizeof(uniStr));
		zk_printZH(500, 200, uniStr);
	}
	
    WorkTimeUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();
	
}
void WorkTimeFrameUpdate(unsigned char index)
{
    // uint8_t uniStr[64];
    static uint32_t worktime_old = 0;
    start_draw();
    WorkTime = Miles.single_time/120;
    if((worktime_old != WorkTime ) ||WorkTimeUpdate)
	{
		loc_ClearRect(400, 200, 100, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(490, 200 , WorkTime, 0, ZK_ALIGN_RIGHT);//
		worktime_old = WorkTime;
    }
	
	WorkTimeUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_TripInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_TripInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_TripInfo;             
    }
}
/** 行程信息——电耗 **/
void PowerConsFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[64];
	
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"电耗 :", uniStr, sizeof(uniStr));
		zk_printZH1(360, 200, uniStr, ZK_ALIGN_RIGHT);
		zk_oem2uni((uint8_t *)"Kwh/100Km", uniStr, sizeof(uniStr));
		zk_printZH(500, 200, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Power Cons. :", uniStr, sizeof(uniStr));
		zk_printZH1(360, 200, uniStr, ZK_ALIGN_RIGHT);
		zk_oem2uni((uint8_t *)"Kwh/100Km", uniStr, sizeof(uniStr));
		zk_printZH(500, 200, uniStr);
	}
	
    PowerSonsUpdate = 1;
    disable_box();
    disable_choose_image();
    end_draw();
	
}
void PowerConsFrameUpdate(unsigned char index)
{
    // uint8_t uniStr[64];
    static uint32_t powercons_old = 0;;
    start_draw();
    if((powercons_old != aveSpeed ) ||PowerSonsUpdate){

	loc_ClearRect(400, 200, 100, 31);
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	zk_printNum(496, 200 , aveSpeed, 1, ZK_ALIGN_RIGHT);//
	powercons_old = aveSpeed;
    }

    PowerSonsUpdate = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_TripInfo;       
    }

    if (key_set) {
	key_set = 0;
	f_index = 0;
	frmID = Menu_TripInfo;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_TripInfo;             
    }
}



/** 个性化设置菜单 **/
void MenuPSNInfoFrame(void)
{
    uint8_t uniStr[64];
    uint16_t width;
	
	TEXT_LIST_t SetInfoMenu[] =
	{
		{ 0, {(uint8_t *)"语言",         (uint8_t *)"Language"       }, 295, 130 + 55 * 0, NULL},
		{ 1, {(uint8_t *)"计量单位",     (uint8_t *)"Units"          }, 295, 130 + 55 * 1, NULL},
		{ 2, {(uint8_t *)"显示效果",     (uint8_t *)"Display effect" }, 295, 130 + 55 * 2, NULL},
		{ 3, {(uint8_t *)"显示内容",     (uint8_t *)"Display content"}, 295, 130 + 55 * 3, NULL},
		{ 4, {(uint8_t *)"时间显示方式", (uint8_t *)"Time Format"    }, 295, 130 + 55 * 4, NULL},
	};
	
    start_draw();
    if(!Menu_PSNInfo_f)
	{
		LCD_Clear();
		
		for(uint8_t i = 0; i < MAJ_MENU_ITEM_SUM; i++)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_oem2uni(MajMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
			width = zk_getTextWidth(MajMenu[i].Text[eol_language]);
			zk_printZH(MajMenu[i].Pos_X - width / 2,  MajMenu[i].Pos_Y, uniStr);
		}
		
		for(uint8_t i = 0; i < 5; i++)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni(SetInfoMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
			zk_printZH(SetInfoMenu[i].Pos_X,  SetInfoMenu[i].Pos_Y, uniStr);
		}
		
    }
    update_box_location(22,124+70*2);
    disable_choose_image();
    Menu_VehicleInfo_f = 0;
    Menu_TripInfo_f = 0;
    //Menu_PSNInfo_f = 0;
    Menu_IC_f = 0;
	
    end_draw();

}
void MenuPSNInfoUpdate(void)
{
    static unsigned char i = 8 ;

    if(!Menu_PSNInfo_f){
		if (key_up) {//箭头向下走
			key_up = 0;
			i++;
		}

		if (key_AV) {//箭头向上走
			key_AV = 0;
			i--;
		}
		
		if(i<=6 ||i >=10)
		{
			i=8;
		}
		
		update_box_location(22, 124 + 70 * (i - 6));
		
		//子菜单间切换
		if (i == 8) {
			if (key_set) {
				key_set = 0;
				f_index = 0;
				frmID = Menu_PSNInfo;
				Menu_PSNInfo_f = 1;
			}
		} else if (i == 9 ) {
			f_index = 0;
			i = 8;
			frmID = Menu_IC;
		} else if (i == 7) {
			f_index = 0;
			i = 8;
			frmID = Menu_TripInfo;
		} else;
    }
    else{
		if (key_up) {//箭头向下走
			key_up = 0;
			i--;
		}

		if (key_AV) {//箭头向上走
			key_AV = 0;
			i++;
		}
		
		if(i <= 3) i=8;
		if(i >=9 ) i=4;
		
		update_choose_location(270, 134 + 55 * (8 - i));
		
		//子菜单间切换
		if (key_set) {
			key_set = 0;
			f_index = 0;
			
			     if (i == 8) frmID = Language;
			else if (i == 7) frmID = Unit;
			else if (i == 6) frmID = Dispeffect;
			else if (i == 5) frmID = DispInfo;
			else if (i == 4) frmID = TimeDisp;
			
			i = 8;
		}
    }

}
void frmMenuPSNInfo(void)
{
    if (f_index == 0) { //只更新一次
        f_index = 1;
	MenuPSNInfoFrame();
    } else if (f_index == 1) {
        f_index = 1;
        MenuPSNInfoUpdate();
    } else {
        f_index = 0;
    }
    if (key_long_set) {
        key_long_set = 0;
        key_set = 0;
        key_up = 0;
        key_AV = 0;
    }

    if (key_duble) {
	key_duble = 0;
	TESTMODE = 1;
	if (time_flag == 0) {
	    f_index = 0;
	    frmID = 7;
	}
    }
    if (key_clear) {//返回主界面
	key_clear = 0;
	frmID = 0;
	f_index = 0;
	Index = 0;
	index_page = 1;
	alarm_update_flag = 0;
	return_from_menu = 1;
	//upgo = 1;
    }
}
/** 个性化设置——语言 **/
void LanguageFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[64];
	
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"语言设置 : ", uniStr, sizeof(uniStr));
		zk_printZH1(400, 240, uniStr, ZK_ALIGN_RIGHT);
		zk_oem2uni((uint8_t *)"简体中文", uniStr, sizeof(uniStr));
		zk_printZH(450, 240, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Language : ", uniStr, sizeof(uniStr));
		zk_printZH1(400, 240, uniStr, ZK_ALIGN_RIGHT);
		zk_oem2uni((uint8_t *)"English", uniStr, sizeof(uniStr));
		zk_printZH(450, 240, uniStr);
	}
	
    disable_box();
    disable_choose_image();
    end_draw();


}
void LanguageFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
	uint8_t* LanguageText[2] = {(uint8_t *)"简体中文", (uint8_t *)"English"};
	
    if (key_up || key_AV)  //子菜单切换
	{
		key_up = 0;
		key_AV = 0;
		
		eol_language = !eol_language;
		
		start_draw();
		loc_ClearRect(450, 240, 120, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni(LanguageText[eol_language], uniStr, sizeof(uniStr));
		zk_printZH(450, 240, uniStr);
		end_draw();
		
    }

    if (key_set||key_clear)
	{
		key_clear = 0;
		key_set = 0;
		f_index = 0;
		frmID = Menu_PSNInfo;
		
		Diagnostic_EEProm_Write(rwdata_list[LANGUAGE].ee_address, 1,&eol_language);
		
    }
	
}
/** 个性化设置——计量单位 **/
static unsigned int old_uint;
void UnitFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[100];
	
    start_draw();
    LCD_Clear();
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	if(eol_language == 0)
	{
		zk_oem2uni((uint8_t *)"单位设置", uniStr, sizeof(uniStr));
		zk_printZH1(400, 240, uniStr, ZK_ALIGN_RIGHT);
	}
	else
	{
		zk_oem2uni((uint8_t *)"Unit setting", uniStr, sizeof(uniStr));
		zk_printZH1(400, 240, uniStr, ZK_ALIGN_RIGHT);
	}
	
	if(e_uint==0)
		zk_oem2uni((uint8_t *)"Km", uniStr, sizeof(uniStr));
	else if(e_uint==1)
		zk_oem2uni((uint8_t *)"Mile", uniStr, sizeof(uniStr));
	else ;
	
    zk_printZH(450, 240, uniStr);
    disable_box();
    disable_choose_image();
    end_draw();
	
	old_uint = e_uint;
}
void UnitFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    if (key_up)  //子菜单切换
	{
		key_up = 0;
		if(e_uint == 0)
		{
			e_uint = 1;
			start_draw();
			loc_ClearRect(450, 240, 100, 31);
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"Mile", uniStr, sizeof(uniStr));
			zk_printZH(450, 240, uniStr);
			end_draw();
		}
		else
		{
			e_uint = 0;
			start_draw();
			loc_ClearRect(450, 240, 100, 31);
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"Km", uniStr, sizeof(uniStr));
			zk_printZH(450, 240, uniStr);
			end_draw();
		}
    }

    if (key_set||key_clear)
	{
		key_clear = 0;
		key_set = 0;
		f_index = 0;
		frmID = Menu_PSNInfo;
    }

    if (key_AV)
	{
		key_AV = 0;
		if(e_uint == 0)
		{
			e_uint = 1;
			start_draw();
			loc_ClearRect(450, 240, 100, 31);
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"Mile", uniStr, sizeof(uniStr));
			zk_printZH(450, 240, uniStr);
			end_draw();
		}
		else
		{
			e_uint = 0;
			start_draw();
			loc_ClearRect(450, 240, 100, 31);
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni((uint8_t *)"Km", uniStr, sizeof(uniStr));
			zk_printZH(450, 240, uniStr);
			end_draw();
		}          
    }
	
	if(old_uint != e_uint)
	{
		old_uint = e_uint;
		EEROM_Write_UINT();
	}
}
/** 个性化设置——显示效果 **/
void DispeffectFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[64];
    
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"LCD显示器亮度调节(白天):", uniStr, sizeof(uniStr));
		zk_printZH(160, 120, uniStr);
		zk_oem2uni((uint8_t *)"LED报警灯亮度调节(白天):", uniStr, sizeof(uniStr));
		zk_printZH(160, 160, uniStr);
		zk_oem2uni((uint8_t *)"仪表背光亮度调节(白天):", uniStr, sizeof(uniStr));
		zk_printZH(160, 200, uniStr);
		zk_oem2uni((uint8_t *)"LCD显示器亮度调节(夜晚):", uniStr, sizeof(uniStr));
		zk_printZH(160, 240, uniStr);
		zk_oem2uni((uint8_t *)"LED报警灯亮度调节(夜晚):", uniStr, sizeof(uniStr));
		zk_printZH(160, 280, uniStr);
		zk_oem2uni((uint8_t *)"仪表背光亮度调节(夜晚):", uniStr, sizeof(uniStr));
		zk_printZH(160, 320, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"LCD brightness (daytime):", uniStr, sizeof(uniStr));
		zk_printZH(160, 120, uniStr);
		zk_oem2uni((uint8_t *)"LED brightness (daytime):", uniStr, sizeof(uniStr));
		zk_printZH(160, 160, uniStr);
		zk_oem2uni((uint8_t *)"Backlight brightness (daytime):", uniStr, sizeof(uniStr));
		zk_printZH(160, 200, uniStr);
		zk_oem2uni((uint8_t *)"LCD brightness (night):", uniStr, sizeof(uniStr));
		zk_printZH(160, 240, uniStr);
		zk_oem2uni((uint8_t *)"LED brightness (night):", uniStr, sizeof(uniStr));
		zk_printZH(160, 280, uniStr);
		zk_oem2uni((uint8_t *)"Backlight brightness (night):", uniStr, sizeof(uniStr));
		zk_printZH(160, 320, uniStr);
	}

    zk_oem2uni((uint8_t *)"%", uniStr, sizeof(uniStr));
    zk_printZH(580, 120, uniStr);
    zk_printZH(580, 160, uniStr);
    zk_printZH(580, 200, uniStr);
    zk_printZH(580, 240, uniStr);
    zk_printZH(580, 280, uniStr);
    zk_printZH(580, 320, uniStr);

    lightlevelset = 1;
    disable_box();
    //disable_choose_image();
    update_choose_location(120,120);
    end_draw();
	
	
}
void DispeffectFrameUpdate(unsigned char index)
{
    static uint8_t lightlevel_old[6] = {0};
    static unsigned chooseditem = 0;
	
    start_draw();
    if(chooseditem == 0)
		update_choose_location(120,120);
    else if(chooseditem == 1)
		update_choose_location(120,160);
    else if(chooseditem == 2)
		update_choose_location(120,200);
    else if(chooseditem == 3)
		update_choose_location(120,240);
    else if(chooseditem == 4)
		update_choose_location(120,280);
    else if(chooseditem == 5)
		update_choose_location(120,320);
    else;

    if(lightlevelset||(lightlevel_old[0] != LightLevel[0]))
	{
		loc_ClearRect(570 - 50, 120, 50, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(570, 120 , LightLevel[0], 0, ZK_ALIGN_RIGHT);//
		lightlevel_old[0] = LightLevel[0];
    }
    if(lightlevelset||(lightlevel_old[1] != LightLevel[1]))
	{
		loc_ClearRect(570 - 50, 160, 50, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(570, 160 , LightLevel[1], 0, ZK_ALIGN_RIGHT);//
		lightlevel_old[1] = LightLevel[1];
    }
    if(lightlevelset||(lightlevel_old[2] != LightLevel[2]))
	{
		loc_ClearRect(570 - 50, 200, 50, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(570, 200 , LightLevel[2], 0, ZK_ALIGN_RIGHT);//
		lightlevel_old[2] = LightLevel[2];
    }
    if(lightlevelset||(lightlevel_old[3] != LightLevel[3]))
	{
		loc_ClearRect(570 - 50, 240, 50, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(570, 240 , LightLevel[3], 0, ZK_ALIGN_RIGHT);//
		lightlevel_old[3] = LightLevel[3];
    }
    if(lightlevelset||(lightlevel_old[4] != LightLevel[4]))
	{
		loc_ClearRect(570 - 50, 280, 50, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(570, 280 , LightLevel[4], 0, ZK_ALIGN_RIGHT);//
		lightlevel_old[4] = LightLevel[4];
    }
    if(lightlevelset||(lightlevel_old[5] != LightLevel[5]))
	{
		loc_ClearRect(570 - 50, 320, 50, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(570, 320 , LightLevel[5], 0, ZK_ALIGN_RIGHT);//
		lightlevel_old[5] = LightLevel[5];
    }
    end_draw();
	
	lightlevelset = 0;
	
    if (key_up)  //子菜单切换
	{
		key_up = 0;
		if(chooseditem==0)  
		{
			if(LightLevel[0]<100)
			LightLevel[0] += 20;
			else
			LightLevel[0] = 20;
		}
		else if(chooseditem==1)  
		{
			if(LightLevel[1]<100)
			LightLevel[1] += 20;
			else
			LightLevel[1] = 20;
		}
		else if(chooseditem==2)  
		{
			if( LightLevel[2]<100)
			LightLevel[2] += 20;
			else
			LightLevel[2] = 20;
		}
		else if(chooseditem==3)  
		{
			if(  LightLevel[3]<90)
			LightLevel[3] += 20;
			else
			LightLevel[3] = 10;
		}
		else if(chooseditem==4)  
		{
			if(LightLevel[4]<90)
			LightLevel[4] += 20;
			else
			LightLevel[4] = 10;
		}
		else if(chooseditem==5)  
		{
			if(LightLevel[5]<90)
			LightLevel[5] += 20;
			else
			LightLevel[5] = 10;
		}
		else;
    }

    if(e_backlight_lcd != LightLevel[0])
	{
		e_backlight_lcd = LightLevel[0];
		EEROM_WriteBackLight_lcd();
		PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd, PWM_PERIOD);//980hz
    }
    if(e_backlight_led != LightLevel[1])
	{
		e_backlight_led = LightLevel[1];
		EEROM_WriteBackLight_led();
		PWM_Update_Duty_Cycle(LED_CHANNEL,e_backlight_led, PWM_PERIOD);//980hz
    }
    if(e_backlight_pannel!= LightLevel[2])
	{
		e_backlight_pannel= LightLevel[2];
		EEROM_WriteBackLight_pannel();
		PWM_Update_Duty_Cycle(BG_CHANNEL,e_backlight_pannel, PWM_PERIOD);//980hz
    }
    
    if(e_backlight_lcd_night!= LightLevel[3])
	{
		e_backlight_lcd_night = LightLevel[3];
		EEROM_WriteBackLight_Night_lcd();
		PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd_night, PWM_PERIOD);//980hz
    }
    if(e_backlight_led_night!= LightLevel[4])
	{
		e_backlight_led_night= LightLevel[4];
		EEROM_WriteBackLight_Night_led();
		PWM_Update_Duty_Cycle(LED_CHANNEL,e_backlight_led_night, PWM_PERIOD);//980hz
    }
    if(e_backlight_pannel_night!= LightLevel[5])
	{
		e_backlight_pannel_night= LightLevel[5];
		EEROM_WriteBackLight_Night_pannel();
		PWM_Update_Duty_Cycle(BG_CHANNEL,e_backlight_pannel_night, PWM_PERIOD);//980hz
    }

    if (key_set)
	{
		key_set = 0;
		if(chooseditem<5)
			chooseditem++;
		else
			chooseditem = 0;
    }
    if (key_clear)
	{
		key_clear = 0;
		key_set = 0;
		f_index = 0;
		frmID = Menu_IC;                   
    }
    if (key_AV)
	{
		key_AV = 0;
		if(chooseditem==0)  
		{
			if(  LightLevel[0]>20)
			LightLevel[0] -= 20;
			else
			LightLevel[0] = 100;
		}
		else if(chooseditem==1)  
		{
			if(  LightLevel[1]>20)
			LightLevel[1] -= 20;
			else
			LightLevel[1] = 100;
		}
		else if(chooseditem==2)  
		{
			if(  LightLevel[2]>20)
			LightLevel[2] -= 20;
			else
			LightLevel[2] = 100;
		}
		else if(chooseditem==3)  
		{
			if(  LightLevel[3]>20)
			LightLevel[3] -= 20;
			else
			LightLevel[3] = 90;
		}
		else if(chooseditem==4)  
		{
			if(  LightLevel[4]>20)
			LightLevel[4] -= 20;
			else
			LightLevel[4] = 90;
		}
		else if(chooseditem==5)  
		{
			if(  LightLevel[5]>20)
			LightLevel[5] -= 20;
			else
			LightLevel[5] = 90;
		}
		else;
    }
}
/** 个性化设置——显示内容 **/
static unsigned int old_disconfig;
void ConfigDisFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[100];
    
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"请选择显示内容:", uniStr, sizeof(uniStr));
		zk_printZH(200, 200, uniStr);
		zk_oem2uni((uint8_t *)"前回路气压", uniStr, sizeof(uniStr));
		zk_printZH(300, 240, uniStr);
		zk_oem2uni((uint8_t *)"后回路气压", uniStr, sizeof(uniStr));
		zk_printZH(300, 280, uniStr);
		zk_oem2uni((uint8_t *)"驻车回路气压", uniStr, sizeof(uniStr));
		zk_printZH(300, 320, uniStr);
		zk_oem2uni((uint8_t *)"蓄电池电压", uniStr, sizeof(uniStr));
		zk_printZH(300, 360, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Select the display contents:", uniStr, sizeof(uniStr));
		zk_printZH(200, 200, uniStr);
		zk_oem2uni((uint8_t *)"Front loop", uniStr, sizeof(uniStr));
		zk_printZH(300, 240, uniStr);
		zk_oem2uni((uint8_t *)"Rear loop", uniStr, sizeof(uniStr));
		zk_printZH(300, 280, uniStr);
		zk_oem2uni((uint8_t *)"Parking loop", uniStr, sizeof(uniStr));
		zk_printZH(300, 320, uniStr);
		zk_oem2uni((uint8_t *)"Auxil.Batt.Volt.", uniStr, sizeof(uniStr));
		zk_printZH(300, 360, uniStr);
	}
	
	zk_oem2uni((uint8_t *)"√", uniStr, sizeof(uniStr));
    if(e_displayconfig&0x01)
		zk_printZH(500, 240, uniStr);
    if(e_displayconfig&0x02)
		zk_printZH(500, 280, uniStr);
    if(e_displayconfig&0x04)
		zk_printZH(500, 320, uniStr); 
	if(e_displayconfig&0x08)
		zk_printZH(500, 360, uniStr); 
    disable_box();
    //disable_choose_image();
    update_choose_location(260, 240);
    end_draw();
	
	old_disconfig=e_displayconfig;
}
void ConfigDisFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static unsigned chooseditem = 0;
	
    if(chooseditem == 0)
		update_choose_location(260,240);
    else if(chooseditem == 1)
		update_choose_location(260,280);
    else if(chooseditem == 2)
		update_choose_location(260,320);
	else if(chooseditem == 3)
		update_choose_location(260,360);
	else ;
	
    if (key_up || key_AV)  //子菜单切换
	{
		key_up = 0;
		key_AV = 0;
		
		if(chooseditem == 0)
		{
			if(e_displayconfig & 1)
			{
				e_displayconfig &= ~0x01;
				start_draw();
				loc_ClearRect(500, 240, 100, 31);
				end_draw();
			}
			else
			{
				e_displayconfig |= 0x01;
				start_draw();
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"√", uniStr, sizeof(uniStr));
				zk_printZH(500, 240, uniStr);
				end_draw();
			}
		}
		else if(chooseditem == 1)
		{
			if(e_displayconfig & 2)
			{
				e_displayconfig &= ~0x02;
				start_draw();
				loc_ClearRect(500, 280, 100, 31);
				end_draw();
			}
			else
			{
				e_displayconfig |= 0x02;
				start_draw();
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"√", uniStr, sizeof(uniStr));
				zk_printZH(500, 280, uniStr);
				end_draw();
			}
		}
		else if(chooseditem == 2)
		{
			if(e_displayconfig & 4)
			{
				e_displayconfig &= ~0x04;
				start_draw();
				loc_ClearRect(500, 320, 100, 31);
				end_draw();
			}
			else
			{
				e_displayconfig |= 0x04;
				start_draw();
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"√", uniStr, sizeof(uniStr));
				zk_printZH(500, 320, uniStr);
				end_draw();
			}
		}
		else if(chooseditem == 3)
		{
			if(e_displayconfig & 4)
			{
				e_displayconfig &= ~0x08;
				start_draw();
				loc_ClearRect(500, 360, 100, 31);
				end_draw();
			}
			else
			{
				e_displayconfig |= 0x08;
				start_draw();
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"√", uniStr, sizeof(uniStr));
				zk_printZH(500, 360, uniStr);
				end_draw();
			}
		}
		else ;
		
	}
	
    if (key_set)
	{
		key_set = 0;
		if(chooseditem < 3)
			chooseditem++;
		else
			chooseditem = 0;
    }
	
	if (key_clear)
	{
		key_clear = 0;
		chooseditem = 0;
		disable_choose_image();
		f_index = 0;
		frmID = Menu_PSNInfo;                   
    }
	
	if(old_disconfig != e_displayconfig)
	{
		old_disconfig = e_displayconfig;
		EEROM_Write_ContentConfing();
	}
}
/** 个性化设置——时间显示方式 **/
static unsigned char old_hourformat;
void TimeShowFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[64];
    // unsigned char color = YELLOW;
	
    start_draw();
    LCD_Clear();
	
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"请选择时间显示格式:", uniStr, sizeof(uniStr));
		zk_printZH(200, 240, uniStr);

		if(e_hourformat==1)
		zk_oem2uni((uint8_t *)"12小时制", uniStr, sizeof(uniStr));
		else
		zk_oem2uni((uint8_t *)"24小时制", uniStr, sizeof(uniStr));
		zk_printZH(450, 240, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Select the display contents:", uniStr, sizeof(uniStr));
		zk_printZH(150, 240, uniStr);

		if(e_hourformat==1)
		zk_oem2uni((uint8_t *)"12h", uniStr, sizeof(uniStr));
		else
		zk_oem2uni((uint8_t *)"24h", uniStr, sizeof(uniStr));
		zk_printZH(500, 240, uniStr);
	}
	
    disable_box();
    disable_choose_image();
    end_draw();
	
	old_hourformat = e_hourformat;
}
void TimeShowFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
	
    if (key_up)  //子菜单切换
	{
		key_up = 0;
		
		if(eol_language == 0)
		{
			if(e_hourformat== 0)
			{
				e_hourformat = 1;//从24h制改为12h制
				
				start_draw();
				loc_ClearRect(450, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"12小时制", uniStr, sizeof(uniStr));
				zk_printZH(450, 240, uniStr);
				end_draw();
			}
			else//从12h制改为24h制
			{
				e_hourformat = 0;			
				start_draw();
				loc_ClearRect(450, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"24小时制", uniStr, sizeof(uniStr));
				zk_printZH(450, 240, uniStr);
				end_draw();
			}
		}
		else
		{
			if(e_hourformat== 0)
			{
				e_hourformat = 1;//从24h制改为12h制
				
				start_draw();
				loc_ClearRect(500, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"12h", uniStr, sizeof(uniStr));
				zk_printZH(500, 240, uniStr);
				end_draw();
			}
			else//从12h制改为24h制
			{
				e_hourformat = 0;			
				start_draw();
				loc_ClearRect(500, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"24h", uniStr, sizeof(uniStr));
				zk_printZH(500, 240, uniStr);
				end_draw();
			}
		}
		
    }
	
    if (key_AV)
	{
		key_AV = 0;
		
		if(eol_language == 0)
		{
			if(e_hourformat== 0)
			{
				e_hourformat = 1;//从24h制改为12h制
				
				start_draw();
				loc_ClearRect(450, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"12小时制", uniStr, sizeof(uniStr));
				zk_printZH(450, 240, uniStr);
				end_draw();
			}
			else//从12h制改为24h制
			{
				e_hourformat = 0;			
				start_draw();
				loc_ClearRect(450, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"24小时制", uniStr, sizeof(uniStr));
				zk_printZH(450, 240, uniStr);
				end_draw();
			}
		}
		else
		{
			if(e_hourformat== 0)
			{
				e_hourformat = 1;//从24h制改为12h制
				
				start_draw();
				loc_ClearRect(500, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"12h", uniStr, sizeof(uniStr));
				zk_printZH(500, 240, uniStr);
				end_draw();
			}
			else//从12h制改为24h制
			{
				e_hourformat = 0;			
				start_draw();
				loc_ClearRect(500, 240, 100, 31);
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_oem2uni((uint8_t *)"24h", uniStr, sizeof(uniStr));
				zk_printZH(500, 240, uniStr);
				end_draw();
			}
		}
		
    }
	
	if (key_set||key_clear)
	{
		key_clear = 0;
		key_set = 0;
		f_index = 0;
		frmID = Menu_PSNInfo;
		if(old_hourformat != e_hourformat)
		{
			old_hourformat = e_hourformat;
			EEROM_Write_HOUR();
		}
    }
}



/** 仪表自诊断信息菜单 **/
void MenuICInfoFrame(void)
{
    uint8_t uniStr[64];
    uint16_t width;
	
	TEXT_LIST_t ICSelfInfoMenu[] =
	{
		{ 0, {(uint8_t *)"指针",       (uint8_t *)"Pointer"        }, 295, 130 + 55 * 0, NULL},
		{ 1, {(uint8_t *)"报警灯",     (uint8_t *)"Alarm indicator"}, 295, 130 + 55 * 1, NULL},
		{ 2, {(uint8_t *)"扬声器",     (uint8_t *)"Buzzer"         }, 295, 130 + 55 * 2, NULL},
		{ 3, {(uint8_t *)"LCD显示",    (uint8_t *)"LCD screen"     }, 295, 130 + 55 * 3, NULL},
		{ 4, {(uint8_t *)"传感器信息", (uint8_t *)"Sensors info"   }, 295, 130 + 55 * 4, NULL},
	};
	
    start_draw();
    if(!Menu_IC_f)
	{
		LCD_Clear();
		
		for(uint8_t i = 0; i < MAJ_MENU_ITEM_SUM; i++)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh38bold, VM_COLOR_WHITE, 0);
			zk_oem2uni(MajMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
			width = zk_getTextWidth(MajMenu[i].Text[eol_language]);
			zk_printZH(MajMenu[i].Pos_X - width / 2,  MajMenu[i].Pos_Y, uniStr);
		}
		
		for(uint8_t i = 0; i < 5; i++)
		{
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_oem2uni(ICSelfInfoMenu[i].Text[eol_language], uniStr, sizeof(uniStr));
			zk_printZH(ICSelfInfoMenu[i].Pos_X,  ICSelfInfoMenu[i].Pos_Y, uniStr);
		}
		
    }
	
    update_box_location(22,124+70*3);
    disable_choose_image();
    Menu_VehicleInfo_f = 0;
    Menu_TripInfo_f = 0;
    Menu_PSNInfo_f = 0;
    //Menu_IC_f = 0;
	
    end_draw();

}
void MenuICInfoUpdate(void)
{
    static unsigned char i = 8 ;
	
    if(!Menu_IC_f){
	if (key_up) {//箭头向下走
	    key_up = 0;
	    i++;
	}

	if (key_AV) {//箭头向上走
	    key_AV = 0;
	    i--;
	}
	if(i==6 ||i ==10)
	    i=8;
	//子菜单间切换
	if (i == 8) {//
	    update_box_location(22,124+70*3);
	    if (key_set) {
		key_set = 0;
		if (time_flag == 0) {
		    f_index = 0;
		    frmID = Menu_IC;
		    Menu_IC_f = 1;
		}
		time_flag = 0;
	    }
	} else;   

	if (i == 9 ) {//
	    update_box_location(22,124+70*0);
	    f_index = 0;
	    i = 8;
	    frmID = Menu_VehicleInfo;
	} else;

	if (i == 7) {//
	    update_box_location(22,124+70*2);
	    f_index = 0;
	    i = 8;
	    frmID = Menu_PSNInfo;

	} else;


    }
    else{
	if (key_up) {//箭头向下走
	    key_up = 0;
	    i--;
	}

	if (key_AV) {//箭头向上走
	    key_AV = 0;
	    i++;
	}

	if(i==3)
	    i=8;
	if(i==9)
	    i=4;
	//子菜单间切换
	if (i == 8) {//
	    update_choose_location(270,134+55*0);
	    if (key_set) {
		key_set = 0;
		if (time_flag == 0) {
		    f_index = 0;
		    frmID = Needle;
		    i = 8;

		}
		time_flag = 0;
	    }
	} else;   

	if (i == 9 || i == 7) {//
	    update_choose_location(270,134+55*1);
	    if (key_set) {
		key_set = 0;
		if (time_flag == 0) {
		    f_index = 0;
		    frmID = Lamp;
		    i = 8;
		}
		time_flag = 0;
	    }
	} else;

	if (i == 10 || i ==6) {//
	    update_choose_location(270,134+55*2);
	    if (key_set) {
		key_set = 0;
		if (time_flag == 0) {
		    f_index = 0;
		    frmID = Buzzer;
		    i = 8;
		}
		time_flag = 0;
	    }
	} else;

	if (i == 11 || i == 5) {//IC
	    update_choose_location(270,134+55*3);
	    if (key_set) {
		key_set = 0;
		if (time_flag == 0) {
		    f_index = 0;
		    frmID = LCD;
		    i = 8;
		}
		time_flag = 0;
	    }
	} else;
	if (i == 12 || i == 4) {//IC
	    update_choose_location(270,134+55*4);
	    if (key_set) {
		key_set = 0;
		if (time_flag == 0) {
		    f_index = 0;
		    frmID = Sensor;
		    i = 8;
		}
		time_flag = 0;
	    }
	} else;
    }

}
void frmMenuICInfo(void)
{
    if (f_index == 0) { //只更新一次
        f_index = 1;
	MenuICInfoFrame();
    } else if (f_index == 1) {
        f_index = 1;
        MenuICInfoUpdate();
    } else {
        f_index = 0;
    }
    if (key_long_set) {
        key_long_set = 0;
        key_set = 0;
        key_up = 0;
        key_AV = 0;
    }

    if (key_duble) {
	key_duble = 0;
	TESTMODE = 1;
	if (time_flag == 0) {
	    f_index = 0;
	    frmID = 7;
	}
    }
    if (key_clear) {//返回主界面
	key_clear = 0;
	frmID = 0;
	f_index = 0;
	Index = 0;
	index_page = 1;
	alarm_update_flag = 0;
	return_from_menu = 1;

	//upgo = 1;
    }
}
/** 仪表自诊断信息——指针 **/
void NeedleFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[128];
    uint16_t width;
	
    start_draw();
    LCD_Clear();
    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	if(eol_language == 0)
	{
		zk_oem2uni((uint8_t *)"请按确认键开始指针测试", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"请按确认键开始指针测试");
		zk_printZH(400 - width / 2, 200, uniStr);
		
	}
	else
	{
		zk_oem2uni((uint8_t *)"Please press the confirm key to start the pointer test", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"Please press the confirm key to start the pointer test");
		zk_printZH(400 - width / 2, 200, uniStr);
	}
	
    disable_box();
    disable_choose_image();
    end_draw();


}
void NeedleFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static uint32_t start = 0;
	
	if(eol_language == 0)
	{
		if(start ==1)
		{
			start = 3;
			
			start_draw();
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"开始测试", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			end_draw();
			
			needleTesting();
			
		}
		if(start ==3)
		{
			start = 4;
			
			Needle_Test = 0;
			
			start_draw();
			loc_ClearRect(10, 200, 780, 31);
			zk_oem2uni((uint8_t *)"测试结束", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			end_draw();
			
			StepperMotormiddle();
			
			start_draw();
			loc_ClearRect(10, 240, 780, 31);
			zk_oem2uni((uint8_t *)"请按取消键返回", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			end_draw();
			
		}
	}
	else
	{
		if(start ==1)
		{
			start = 3;
			
			start_draw();
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"Start Test", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			end_draw();
			
			needleTesting();
			
		}
		if(start == 3)
		{
			start = 4;
			Needle_Test = 0;
			
			start_draw();
			loc_ClearRect(10, 200, 780, 31);
			zk_oem2uni((uint8_t *)"End Of Test", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			end_draw();
			
			StepperMotormiddle();
			
			start_draw();
			loc_ClearRect(10, 240, 780, 31);
			zk_oem2uni((uint8_t *)"Please press Cancel to return", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			end_draw();
			
		}
	}
	
    if (key_up&&(Needle_Test == 0)){ //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_IC;       
    }

    if ((key_set)&&(Needle_Test == 0)) {
	key_set = 0;
	start = 1;              
	Needle_Test = 1;
    }

    if (key_clear&&(start == 2)) {
	key_clear = 0;
	key_set = 0;
	start = 3;
	Needle_Test = 0;

	//f_index = 0;
	//frmID = Menu_TripInfo;                   
    }
    if (key_clear&&(start == 4)) {
	key_clear = 0;
	key_set = 0;
	start = 0;
	Needle_Test = 0;
	f_index = 0;
	frmID = Menu_IC;                   
    }

    if (key_AV&&(Needle_Test == 0)) {
	key_AV = 0;
	f_index = 0;
	Needle_Test = 0;
	frmID = Menu_IC;             
    }
}
/** 仪表自诊断信息——报警灯 **/
void WarnLedFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[128];
    uint16_t width;
	
    start_draw();
    LCD_Clear();
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	if(eol_language == 0)
	{
		zk_oem2uni((uint8_t *)"请按确认键开始报警灯测试", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"请按确认键开始报警灯测试");
		zk_printZH(400 - width / 2, 200, uniStr);
		
	}
	else
	{
		zk_oem2uni((uint8_t *)"Please press the confirm button to start the alarm light test", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"Please press the confirm button to start the alarm light test");
		zk_printZH(400 - width / 2, 200, uniStr);
	}
	
    disable_box();
    disable_choose_image();
    end_draw();


}
void WarnLedFrameUpdate(unsigned char index)
{
    uint8_t uniStr[100];
    static uint32_t start = 0;
	
    start_draw();
	if(eol_language == 0)
	{
		if(start ==1)
		{
			start =2;
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"开始测试", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"请按取消键退出测试", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		if(start ==3)
		{
			start =4;
			LED_test = 0;
			
			loc_ClearRect(10, 200, 780, 80);
			
			zk_oem2uni((uint8_t *)"测试结束", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"请按取消键返回", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
	}
	else
	{
		if(start ==1)
		{
			start =2;
			loc_ClearRect(10, 200, 780, 80);
			
			zk_oem2uni((uint8_t *)"Start Testing", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"Press Cancel to exit the test", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		if(start ==3)
		{
			start =4;
			LED_test = 0;
			
			loc_ClearRect(10, 200, 780, 80);
			
			zk_oem2uni((uint8_t *)"End Of Test", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"Please press Cancel to return", uniStr, sizeof(uniStr));
			zk_printZH(200, 240, uniStr);
			
		}
	}
    
    end_draw();

    if (key_up&&(LED_test == 0)){ //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_IC;       
    }

    if (key_set&&(LED_test == 0)) {
	key_set = 0;
	start = 1;                 
	LED_test = 1;
    }

      if (key_clear&&(start == 2)) {
	key_clear = 0;
	key_set = 0;
	start = 3;
	LED_test = 0;                   
    }
      if (key_clear&&(start == 4)) {
	key_clear = 0;
	key_set = 0;
	start = 0;
	LED_test = 0;
	f_index = 0;
	frmID = Menu_IC;                   
    }

    if (key_AV) {
	key_AV = 0;
	LED_test = 0;
	f_index = 0;
	frmID = Menu_IC;             
    }
}
/** 仪表自诊断信息——扬声器 **/
void SpeakerFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[128];
    uint16_t width;
	
	start_draw();
    LCD_Clear();
	zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	if(eol_language == 0)
	{
		zk_oem2uni((uint8_t *)"请按确认键开始蜂鸣器测试", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"请按确认键开始蜂鸣器测试");
		zk_printZH(400 - width / 2, 200, uniStr);
		
	}
	else
	{
		zk_oem2uni((uint8_t *)"Please press the confirm button to start the buzzer test", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"Please press the confirm button to start the buzzer test");
		zk_printZH(400 - width / 2, 200, uniStr);
	}
	
    disable_box();
    disable_choose_image();
    end_draw();


}
void SpeakerFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static uint32_t start = 0;
	
    start_draw();
	
	if(eol_language == 0)
	{
		if(start ==1)
		{
			start =2;
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"开始测试", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"请按取消键退出测试", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		if(start ==3)
		{
			start =4;
			BUZZ_test = 0;   
			
			SoundStop();
			
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"测试结束", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"请按取消键返回", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		
	}
	else
	{
		if(start ==1)
		{
			start = 2;
			loc_ClearRect(10, 200, 780, 80);
			
			zk_oem2uni((uint8_t *)"Start Test", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"Press Cancel to exit the test", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		if(start ==3)
		{
			start =4;
			BUZZ_test = 0;   
			SoundStop();
			
			loc_ClearRect(10, 200, 780, 80);
			
			zk_oem2uni((uint8_t *)"End Of Test", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"Please press Cancel to return", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		
	}
	
    end_draw();

    if (key_up&&(BUZZ_test == 0)) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_IC;       
    }

    if (key_set&&BUZZ_test == 0) {
	key_set = 0;
	BUZZ_test = 1;
	start = 1;                  
    }

    if (key_clear&&(start == 2)) {
	key_clear = 0;
	key_set = 0;
	start = 3;
	BUZZ_test = 0;                  
    }
    if (key_clear&&(start == 4)) {
	key_clear = 0;
	key_set = 0;
	start = 0;
	BUZZ_test = 0;      
	f_index = 0;
	frmID = Menu_IC;                   
    }

    if (key_AV&&BUZZ_test == 0)
	{
		key_AV = 0;
		f_index = 0;
		BUZZ_test = 0;      
		frmID = Menu_IC;             
    }
    if(BUZZ_test ==0)
	{
		SoundStop();
	}
}
/** 仪表自诊断信息——LCD显示 **/
void LCDTestFrame(unsigned char index, unsigned char page) 
{
	uint8_t uniStr[128];
    uint16_t width;
	
    start_draw();
    LCD_Clear();
    zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
	if(eol_language == 0)
	{
		zk_oem2uni((uint8_t *)"请按确认键开始LCD测试", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"请按确认键开始LCD测试");
		zk_printZH(400 - width / 2, 200, uniStr);
		
	}
	else
	{
		zk_oem2uni((uint8_t *)"Please press the confirm button to start the LCD test", uniStr, sizeof(uniStr));
		width = zk_getTextWidth((uint8_t *)"Please press the confirm button to start the LCD test");
		zk_printZH(400 - width / 2, 200, uniStr);
	}
	
    disable_box();
    disable_choose_image();
    end_draw();


}
void LCDTestFrameUpdate(unsigned char index)
{
    uint8_t uniStr[64];
    static uint32_t start = 0;
	
    start_draw();
	if(eol_language == 0)
	{
		if(start ==1)
		{
			start =2;
			
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"开始测试", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"请按取消键退出测试", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		
		if(start ==2)
		{
			start =3;
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			LCD_Color(0xffff0000);//红色
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			end_draw();
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			LCD_Color(0xff00ff00);//红色
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			end_draw();
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			ClrWdt();

			LCD_Color(0xff0000ff);//红色
			ClrWdt();
			R_TICK_WaitMS(0, 400);

			end_draw();
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			ClrWdt();
			LCD_Clear();
			//zk_oem2uni((uint8_t *)"测试结束", uniStr, sizeof(uniStr));
			
		}
		
		if(start ==3)
		{
			start = 4;
			LCD_test = 0;
			
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"测试结束", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"请按取消键返回", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
	}
	else
	{
		if(start ==1)
		{
			start =2;
			
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"Start Test", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"Press Cancel to exit the test", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
		
		if(start ==2)
		{
			start =3;
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			LCD_Color(0xffff0000);//红色
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			end_draw();
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			LCD_Color(0xff00ff00);//红色
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			end_draw();
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			ClrWdt();

			LCD_Color(0xff0000ff);//红色
			ClrWdt();
			R_TICK_WaitMS(0, 400);

			end_draw();
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			R_TICK_WaitMS(0, 400);
			ClrWdt();
			ClrWdt();
			LCD_Clear();
			//zk_oem2uni((uint8_t *)"测试结束", uniStr, sizeof(uniStr));
			
		}
		
		if(start ==3)
		{
			start = 4;
			LCD_test = 0;
			
			loc_ClearRect(10, 200, 780, 80);
			zk_oem2uni((uint8_t *)"End Of Test", uniStr, sizeof(uniStr));
			zk_printZH(260, 200, uniStr);
			
			zk_oem2uni((uint8_t *)"Please press Cancel to return", uniStr, sizeof(uniStr));
			zk_printZH(260, 240, uniStr);
			
		}
	}
    end_draw();

    if (key_up&&LCD_test == 0) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_IC;       
    }

    if (key_set&&LCD_test == 0)  {
	key_set = 0;
	start = 1;       
	LCD_test = 1;
    }

    if (key_clear&&(start == 2)) {
	key_clear = 0;
	key_set = 0;
	start = 3;
	LCD_test = 0;              
    }
    if (key_clear&&(start ==4)) {
	key_clear = 0;
	key_set = 0;
	start = 0;
	LCD_test = 0;      
	f_index = 0;
	frmID = Menu_IC;                   
    }

    if (key_AV&&LCD_test == 0)  {
	key_AV = 0;
	LCD_test = 0;      
	f_index = 0;
	frmID = Menu_IC;             
    }
}
/** 仪表自诊断信息——传感器 **/
void SensorTestFrame(unsigned char index, unsigned char page)
{
	uint8_t uniStr[100];
    
	/*
	  P10_0 前桥回路气压传感器信号0.3V~5V
	  P10_1 乘客侧压力开关    受到压力后，阻值0Ω~150Ω
	  P10_2 水位传感器信号>140KΩ报警
	  P10_3 环境温度传感器信号83Ω~96KΩ
	  P10_4 后桥回路气压传感器信号	0.3V~5V
	  P10_5 车速传感器电源	12V±0.3V 
	  P10_6 蓄电池电压
	  P10_7 前桥回路气压传感器电源	5V±0.1V/8.0mA
	  P10_8 前桥气压传感器电源（预留）	5V±0.1V/8.0mA
	  P10_9 预留-驻车气压传感器电源	5V±0.1V/8.0mA 
	  */
	
    start_draw();
    LCD_Clear();
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"前回路气压传感器供电电压(V):", uniStr, sizeof(uniStr));
		zk_printZH(80, 120, uniStr);
		zk_oem2uni((uint8_t *)"信号(V):", uniStr, sizeof(uniStr));
		zk_printZH(520, 120, uniStr);
		zk_oem2uni((uint8_t *)"后回路气压传感器供电电压(V):", uniStr, sizeof(uniStr));
		zk_printZH(80, 160, uniStr);
		zk_oem2uni((uint8_t *)"信号(V):", uniStr, sizeof(uniStr));
		zk_printZH(520, 160, uniStr);
		zk_oem2uni((uint8_t *)"驻车气压传感器电源电压(V):", uniStr, sizeof(uniStr));
		zk_printZH(80, 200, uniStr);
		zk_oem2uni((uint8_t *)"环境温度传感器信号电阻(Ω):", uniStr, sizeof(uniStr));
		zk_printZH(80, 240, uniStr);
		zk_oem2uni((uint8_t *)"乘客侧压力开关电阻(Ω):", uniStr, sizeof(uniStr));
		zk_printZH(80, 280, uniStr);
		zk_oem2uni((uint8_t *)"水位传感器信号(KΩ):", uniStr, sizeof(uniStr));
		zk_printZH(80, 320, uniStr);
		zk_oem2uni((uint8_t *)"蓄电池电压(V):", uniStr, sizeof(uniStr));
		zk_printZH(80, 360, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"FL Pressure Supply Volt (V):", uniStr, sizeof(uniStr));
		zk_printZH(40, 120, uniStr);
		zk_oem2uni((uint8_t *)"Signal(V):", uniStr, sizeof(uniStr));
		zk_printZH(530, 120, uniStr);
		zk_oem2uni((uint8_t *)"RL Pressure Supply Volt (V):", uniStr, sizeof(uniStr));
		zk_printZH(40, 160, uniStr);
		zk_oem2uni((uint8_t *)"Signal(V):", uniStr, sizeof(uniStr));
		zk_printZH(530, 160, uniStr);
		zk_oem2uni((uint8_t *)"PL Pressure Supply Volt (V):", uniStr, sizeof(uniStr));
		zk_printZH(40, 200, uniStr);
		zk_oem2uni((uint8_t *)"Ambient temp sensor (Ω):", uniStr, sizeof(uniStr));
		zk_printZH(40, 240, uniStr);
		zk_oem2uni((uint8_t *)"Passenger side pressure switch(Ω):", uniStr, sizeof(uniStr));
		zk_printZH(40, 280, uniStr);
		zk_oem2uni((uint8_t *)"Water level sensor signal (KΩ):", uniStr, sizeof(uniStr));
		zk_printZH(40, 320, uniStr);
		zk_oem2uni((uint8_t *)"Auxiliary Battery Volt(V):", uniStr, sizeof(uniStr));
		zk_printZH(40, 360, uniStr);
	}
	
    sensor_test = 1;
    disable_box();
    disable_choose_image();
    end_draw();


}
void SensorTestFrameUpdate(unsigned char index)
{
    // uint8_t uniStr[64];
	static uint32_t ad_old[10] = {0};
	uint32_t ad_buffer[10] = {0};
    start_draw();
	
	//P10_7 前桥回路气压传感器电源	5V±0.1V/8.0mA
	ad_buffer[0] = get_input_analog_quantity(B24_5V_DET)/10;
    if(sensor_test||(ad_old[0]!= ad_buffer[0]))
	{
		loc_ClearRect(430, 120, 70, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(496, 120 , ad_buffer[0], 2, ZK_ALIGN_RIGHT);//
		ad_old[0]= ad_buffer[0];
    }
	
	//P10_0--前桥回路气压传感器信号0.3V~5V
	ad_buffer[1] = get_input_analog_quantity(AD_V_B3)/10;
    if(sensor_test||(ad_old[1]!= ad_buffer[1]))
	{
		loc_ClearRect(640, 120, 80, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(640, 120 , ad_buffer[1], 2, ZK_ALIGN_LEFT);//
		ad_old[1]= ad_buffer[1];
    }
	
	//P10_7 前桥回路气压传感器电源	5V±0.1V/8.0mA
	ad_buffer[2] = get_input_analog_quantity(B24_5V_DET)/10;
    if(sensor_test||(ad_old[2]!= ad_buffer[2]))
	{
		loc_ClearRect(430, 160, 70, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(496, 160 , ad_buffer[2], 2, ZK_ALIGN_RIGHT);//
		ad_old[2]= ad_buffer[2];
    }
	
	//P10_4 后桥回路气压传感器信号	0.3V~5V
	ad_buffer[3] = get_input_analog_quantity(AD_V_B20)/10;
    if(sensor_test||(ad_old[3]!= ad_buffer[3])){
		loc_ClearRect(640, 160, 80, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(640, 160 , ad_buffer[3], 2, ZK_ALIGN_LEFT);//
		ad_old[3]= ad_buffer[3];
    }
    
	//P10_9 驻车气压传感器电源	12V±0.3V 
	ad_buffer[4] = get_input_analog_quantity(B26_5V_DET)/10;
	if(sensor_test||(ad_old[4]!= ad_buffer[4]))
	{
		loc_ClearRect(430, 200, 70, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(496, 200 , ad_buffer[4], 2, ZK_ALIGN_RIGHT);//
		ad_old[4]= ad_buffer[4];
    }
	
	//P10_3 环境温度传感器信号83Ω~96KΩ
	ad_buffer[5] = get_input_analog_quantity(AD_R_B18);
    if(sensor_test||(ad_old[5]!= ad_buffer[5]))
	{
		loc_ClearRect(445, 240, 150, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(445, 240 , ad_buffer[5], 0, ZK_ALIGN_LEFT);//
		ad_old[5]=ad_buffer[5];
    }
    
	//P10_1  乘客侧压力开关    受到压力后，阻值0Ω~150Ω
	ad_buffer[6] = get_input_analog_quantity(AD_R_B4);
	if(sensor_test||(ad_old[6]!= ad_buffer[6]))
	{
		loc_ClearRect(445, 280, 150, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(445, 280, ad_buffer[6], 0, ZK_ALIGN_LEFT);//
		ad_old[6]=ad_buffer[6];
    }
	
	//P10_2  水位信号>140k报警
	ad_buffer[8] = get_input_analog_quantity(AD_R_B17)/1000; //单位 kΩ
    if(sensor_test||(ad_old[8]!= ad_buffer[8]))
	{
		loc_ClearRect(445, 320, 150, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(445, 320 , ad_buffer[8],0, ZK_ALIGN_LEFT);//
		ad_old[8]=ad_buffer[8];
    }
	
	//P10_6 蓄电池电压
	ad_buffer[7] = get_input_analog_quantity(BAT_V)/100;
    if(sensor_test||(ad_old[7]!= ad_buffer[7])){
		loc_ClearRect(430, 360, 70, 31);
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printNum(496, 360 , ad_buffer[7], 1, ZK_ALIGN_RIGHT);//
		ad_old[7]=ad_buffer[7];
    }
    sensor_test = 0;
    end_draw();

    if (key_up) { //子菜单切换
	key_up = 0;
	f_index = 0;
	frmID = Menu_IC;       
    }

    if (key_set) {
	key_set = 0;

    }

  if (key_clear) {
	key_clear = 0;
	key_set = 0;
	f_index = 0;
	frmID = Menu_IC;                   
    }

    if (key_AV) {
	key_AV = 0;
	f_index = 0;
	frmID = Menu_IC;             
    }
}



/** 车辆信息——胎压信息 **/
static uint8_t  old_tyre_status[12];
static uint16_t old_tyre_pressure[12];
static int16_t  old_tyre_temperature[12];
static uint8_t  old_tyre_timeout_status[12];
#define TPMS_TRUCK_POS_X   332
#define TPMS_TRUCK_POS_Y   105
/* 2轴，6轮胎  eol_drivetype = 0 */
static const COORD_t B2001_Tyre_Info_Coord[B2001_TYRE_NUM][2] = 
{
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 75}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 75}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 268}}
	
};
void TPMS_B2001(void)
{
	uint8_t uniStr[64];
	
	// start_draw();
	
	/* 第二轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 283, 48, 3, 0xff8e8e8e);
	
	for(uint8_t i = 0; i < B2001_TYRE_NUM; i++)
		loc_RenderImg(B2001_Tyre_Info_Coord[i][0].Pos_X, B2001_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
	
	for(uint8_t i = 0; i < B2001_TYRE_NUM; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xff8e8e8e, 0);
		zk_oem2uni((uint8_t*)"/", uniStr, sizeof(uniStr));
		zk_printZH(B2001_Tyre_Info_Coord[i][1].Pos_X, B2001_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
	}
	
	// end_draw();
	
}
void TPMS_B2001_Update(void)
{
	uint8_t  tyre_status[B2001_TYRE_NUM];
	
	uint16_t tyre_pressure[B2001_TYRE_NUM];
	uint16_t tyre_temperature[B2001_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2001_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2001_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2001_TYRE_NUM];
	
	uint32_t colorf;
	
	start_draw();
	if(learnstatus)
	{
		for(uint8_t i = 0; i < 6; i++)
		{
			if((i+1) == tire)
				loc_RenderImg(B2001_Tyre_Info_Coord[i][0].Pos_X, B2001_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireLearn);
			else
				loc_RenderImg(B2001_Tyre_Info_Coord[i][0].Pos_X, B2001_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
		}
	}
	else
	{
		tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
		tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
		tyre_status[2] = TyreLocation_pressureh[1].bit0 + TyreLocation_pressurel[1].bit0 + TyreLocation_fastlostgas[1].bit0 + TyreLocation_temperature[1].bit0;
		tyre_status[3] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
		tyre_status[4] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
		tyre_status[5] = TyreLocation_pressureh[1].bit3 + TyreLocation_pressurel[1].bit3 + TyreLocation_fastlostgas[1].bit3 + TyreLocation_temperature[1].bit3;
		
		for(uint8_t i = 0; i < B2001_TYRE_NUM; i++)
		{
			if(old_tyre_status[i] != tyre_status[i] || TpmsVol_update)
			{
				old_tyre_status[i] = tyre_status[i];
				
				if(tyre_status[i] != 0)
					loc_RenderImg(B2001_Tyre_Info_Coord[i][0].Pos_X, B2001_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireError);
				else
					loc_RenderImg(B2001_Tyre_Info_Coord[i][0].Pos_X, B2001_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
			}
		}
		
	}
	
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
	
	for(uint8_t i = 0; i < B2001_TYRE_NUM; i++)
	{
		if(TpmsVol_update||old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2001_Tyre_Info_Coord[i][1].Pos_X - 5 - 66, B2001_Tyre_Info_Coord[i][1].Pos_Y, 66, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2001_Tyre_Info_Coord[i][1].Pos_X - 5, B2001_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*2, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2001_Tyre_Info_Coord[i][1].Pos_X - 5, B2001_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(TpmsVol_update||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2001_Tyre_Info_Coord[i][1].Pos_X + 15, B2001_Tyre_Info_Coord[i][1].Pos_Y, 44, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2001_Tyre_Info_Coord[i][1].Pos_X + 15, B2001_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2001_Tyre_Info_Coord[i][1].Pos_X + 15, B2001_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
		
	}
	end_draw();
	
}

/* 3轴，8轮胎  eol_drivetype = 1 */
static const COORD_t B2003_Tyre_Info_Coord[B2003_TYRE_NUM][2] = 
{
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 75}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 75}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 140}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 140}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 268}}
	
};
void TPMS_B2003(void)
{
	uint8_t uniStr[64];
	
	// start_draw();
	
	/* 第二轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 153, 80, 3, 0xff8e8e8e);
	
	/* 第三轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 283, 48, 3, 0xff8e8e8e);
	
	for(uint8_t i = 0; i < B2003_TYRE_NUM; i++)
		loc_RenderImg(B2003_Tyre_Info_Coord[i][0].Pos_X, B2003_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
	
	for(uint8_t i = 0; i < B2003_TYRE_NUM; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xff8e8e8e, 0);
		zk_oem2uni((uint8_t*)"/", uniStr, sizeof(uniStr));
		zk_printZH(B2003_Tyre_Info_Coord[i][1].Pos_X, B2003_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
	}
	
	// end_draw();
	
}
void TPMS_B2003_Update(void)
{
	uint8_t  tyre_status[B2003_TYRE_NUM];
	
	uint16_t tyre_pressure[B2003_TYRE_NUM];
	int16_t  tyre_temperature[B2003_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2003_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2003_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2003_TYRE_NUM];
	
	uint32_t colorf;
	
	start_draw();
	if(learnstatus)
	{
		for(uint8_t i = 0; i < B2003_TYRE_NUM; i++)
		{
			if((i+1) == tire)
				loc_RenderImg(B2003_Tyre_Info_Coord[i][0].Pos_X, B2003_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireLearn);
			else
				loc_RenderImg(B2003_Tyre_Info_Coord[i][0].Pos_X, B2003_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
		}
	}
	else
	{
		tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
		tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
		tyre_status[2] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
		tyre_status[3] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
		tyre_status[4] = TyreLocation_pressureh[2].bit0 + TyreLocation_pressurel[2].bit0 + TyreLocation_fastlostgas[2].bit0 + TyreLocation_temperature[2].bit0;
		tyre_status[5] = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
		tyre_status[6] = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
		tyre_status[7] = TyreLocation_pressureh[2].bit3 + TyreLocation_pressurel[2].bit3 + TyreLocation_fastlostgas[2].bit3 + TyreLocation_temperature[2].bit3;
		
		for(uint8_t i = 0; i < B2003_TYRE_NUM; i++)
		{
			if(old_tyre_status[i] != tyre_status[i] || TpmsVol_update)
			{
				old_tyre_status[i] = tyre_status[i];
				
				if(tyre_status[i] != 0)
					loc_RenderImg(B2003_Tyre_Info_Coord[i][0].Pos_X, B2003_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireError);
				else
					loc_RenderImg(B2003_Tyre_Info_Coord[i][0].Pos_X, B2003_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
			}
		}
		
	}
	
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
		if(TpmsVol_update||old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2003_Tyre_Info_Coord[i][1].Pos_X - 5 - 66, B2003_Tyre_Info_Coord[i][1].Pos_Y, 66, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2003_Tyre_Info_Coord[i][1].Pos_X - 5, B2003_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*2, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2003_Tyre_Info_Coord[i][1].Pos_X - 5, B2003_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(TpmsVol_update||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2003_Tyre_Info_Coord[i][1].Pos_X + 15, B2003_Tyre_Info_Coord[i][1].Pos_Y, 44, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2003_Tyre_Info_Coord[i][1].Pos_X + 15, B2003_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2003_Tyre_Info_Coord[i][1].Pos_X + 15, B2003_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
		
	}
	end_draw();
	
}

/* 3轴，10轮胎  eol_drivetype = 2 */
static const COORD_t B2004_Tyre_Info_Coord[B2004_TYRE_NUM][2] = 
{
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 75}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 75}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 208}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 268}}
	
};
void TPMS_B2004(void)
{
	uint8_t uniStr[64];
	
	// start_draw();
	
	/* 第二轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 223, 48, 3, 0xff8e8e8e);
	
	/* 第三轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 283, 48, 3, 0xff8e8e8e);
	
	for(uint8_t i = 0; i < B2004_TYRE_NUM; i++)
		loc_RenderImg(B2004_Tyre_Info_Coord[i][0].Pos_X, B2004_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
	
	for(uint8_t i = 0; i < B2004_TYRE_NUM; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xff8e8e8e, 0);
		zk_oem2uni((uint8_t*)"/", uniStr, sizeof(uniStr));
		zk_printZH(B2004_Tyre_Info_Coord[i][1].Pos_X, B2004_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
	}
	
	// end_draw();
	
}
void TPMS_B2004_Update(void)
{
	uint8_t  tyre_status[B2004_TYRE_NUM];
	
	uint16_t tyre_pressure[B2004_TYRE_NUM];
	int16_t  tyre_temperature[B2004_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2004_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2004_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2004_TYRE_NUM];
	
	uint32_t colorf;
	
	start_draw();
	if(learnstatus)
	{
		for(uint8_t i = 0; i < B2004_TYRE_NUM; i++)
		{
			if((i+1) == tire)
				loc_RenderImg(B2004_Tyre_Info_Coord[i][0].Pos_X, B2004_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireLearn);
			else
				loc_RenderImg(B2004_Tyre_Info_Coord[i][0].Pos_X, B2004_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
		}
	}
	else
	{
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
		
		for(uint8_t i = 0; i < B2004_TYRE_NUM; i++)
		{
			if(old_tyre_status[i] != tyre_status[i] || TpmsVol_update)
			{
				old_tyre_status[i] = tyre_status[i];
				
				if(tyre_status[i] != 0)
					loc_RenderImg(B2004_Tyre_Info_Coord[i][0].Pos_X, B2004_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireError);
				else
					loc_RenderImg(B2004_Tyre_Info_Coord[i][0].Pos_X, B2004_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
			}
		}
		
	}
	
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
	
	for(uint8_t i = 0; i < B2004_TYRE_NUM; i++)
	{
		if(TpmsVol_update||old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2004_Tyre_Info_Coord[i][1].Pos_X - 5 - 66, B2004_Tyre_Info_Coord[i][1].Pos_Y, 66, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2004_Tyre_Info_Coord[i][1].Pos_X - 5, B2004_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*2, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2004_Tyre_Info_Coord[i][1].Pos_X - 5, B2004_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(TpmsVol_update||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2004_Tyre_Info_Coord[i][1].Pos_X + 15, B2004_Tyre_Info_Coord[i][1].Pos_Y, 44, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2004_Tyre_Info_Coord[i][1].Pos_X + 15, B2004_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2004_Tyre_Info_Coord[i][1].Pos_X + 15, B2004_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
		
	}
	end_draw();
	
}

/* 4轴，10轮胎  eol_drivetype = 3 */
static const COORD_t B2006_Tyre_Info_Coord[B2006_TYRE_NUM][2] = 
{
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 75}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 75}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 140}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 140}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 208}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 268}}
	
};
void TPMS_B2006(void)
{
	uint8_t uniStr[64];
	
	// start_draw();
	
	/* 第二轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 153, 80, 3, 0xff8e8e8e);
	
	/* 第三轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 221, 80, 3, 0xff8e8e8e);
	
	/* 第四轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 283, 48, 3, 0xff8e8e8e);
	
	for(uint8_t i = 0; i < B2006_TYRE_NUM; i++)
		loc_RenderImg(B2006_Tyre_Info_Coord[i][0].Pos_X, B2006_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
	
	for(uint8_t i = 0; i < B2006_TYRE_NUM; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xff8e8e8e, 0);
		zk_oem2uni((uint8_t*)"/", uniStr, sizeof(uniStr));
		zk_printZH(B2006_Tyre_Info_Coord[i][1].Pos_X, B2006_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
	}
	
	// end_draw();
	
}
void TPMS_B2006_Update(void)
{
	uint8_t  tyre_status[B2006_TYRE_NUM];
	
	uint16_t tyre_pressure[B2006_TYRE_NUM];
	int16_t  tyre_temperature[B2006_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2006_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2006_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2006_TYRE_NUM];
	
	uint32_t colorf;
	
	start_draw();
	if(learnstatus)
	{
		for(uint8_t i = 0; i < B2006_TYRE_NUM; i++)
		{
			if((i+1) == tire)
				loc_RenderImg(B2006_Tyre_Info_Coord[i][0].Pos_X, B2006_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireLearn);
			else
				loc_RenderImg(B2006_Tyre_Info_Coord[i][0].Pos_X, B2006_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
		}
	}
	else
	{
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
		
		for(uint8_t i = 0; i < B2006_TYRE_NUM; i++)
		{
			if(old_tyre_status[i] != tyre_status[i] || TpmsVol_update)
			{
				old_tyre_status[i] = tyre_status[i];
				
				if(tyre_status[i] != 0)
					loc_RenderImg(B2006_Tyre_Info_Coord[i][0].Pos_X, B2006_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireError);
				else
					loc_RenderImg(B2006_Tyre_Info_Coord[i][0].Pos_X, B2006_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
			}
		}
		
	}
	
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
	
	for(uint8_t i = 0; i < B2006_TYRE_NUM; i++)
	{
		if(TpmsVol_update||old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2006_Tyre_Info_Coord[i][1].Pos_X - 5 - 66, B2006_Tyre_Info_Coord[i][1].Pos_Y, 66, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2006_Tyre_Info_Coord[i][1].Pos_X - 5, B2006_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*2, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2006_Tyre_Info_Coord[i][1].Pos_X - 5, B2006_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(TpmsVol_update||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2006_Tyre_Info_Coord[i][1].Pos_X + 15, B2006_Tyre_Info_Coord[i][1].Pos_Y, 44, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2006_Tyre_Info_Coord[i][1].Pos_X + 15, B2006_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2006_Tyre_Info_Coord[i][1].Pos_X + 15, B2006_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
		
	}
	end_draw();
	
}

/* 4轴，10轮胎  eol_drivetype = 4 */
static const COORD_t B2012_Tyre_Info_Coord[B2012_TYRE_NUM][2] = 
{
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 75}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 75}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 140}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 140}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 208}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 268}}
	
};
void TPMS_B2012(void)
{
	uint8_t uniStr[64];
	
	// start_draw();
	
	/* 第二轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 153, 80, 3, 0xff8e8e8e);
	
	/* 第三轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 221, 48, 3, 0xff8e8e8e);
	
	/* 第四轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 283, 80, 3, 0xff8e8e8e);
	
	for(uint8_t i = 0; i < B2012_TYRE_NUM; i++)
		loc_RenderImg(B2012_Tyre_Info_Coord[i][0].Pos_X, B2012_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
	
	for(uint8_t i = 0; i < B2012_TYRE_NUM; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xff8e8e8e, 0);
		zk_oem2uni((uint8_t*)"/", uniStr, sizeof(uniStr));
		zk_printZH(B2012_Tyre_Info_Coord[i][1].Pos_X, B2012_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
	}
	
	// end_draw();
	
}
void TPMS_B2012_Update(void)
{
	uint8_t  tyre_status[B2012_TYRE_NUM];
	
	uint16_t tyre_pressure[B2012_TYRE_NUM];
	int16_t  tyre_temperature[B2012_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2012_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2012_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2012_TYRE_NUM];
	
	uint32_t colorf;
	
	start_draw();
	if(learnstatus)
	{
		for(uint8_t i = 0; i < B2012_TYRE_NUM; i++)
		{
			if((i+1) == tire)
				loc_RenderImg(B2012_Tyre_Info_Coord[i][0].Pos_X, B2012_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireLearn);
			else
				loc_RenderImg(B2012_Tyre_Info_Coord[i][0].Pos_X, B2012_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
		}
	}
	else
	{
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
		
		for(uint8_t i = 0; i < B2012_TYRE_NUM; i++)
		{
			if(old_tyre_status[i] != tyre_status[i] || TpmsVol_update)
			{
				old_tyre_status[i] = tyre_status[i];
				
				if(tyre_status[i] != 0)
					loc_RenderImg(B2012_Tyre_Info_Coord[i][0].Pos_X, B2012_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireError);
				else
					loc_RenderImg(B2012_Tyre_Info_Coord[i][0].Pos_X, B2012_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
			}
		}
		
	}
	
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
	
	for(uint8_t i = 0; i < B2012_TYRE_NUM; i++)
	{
		if(TpmsVol_update||old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2012_Tyre_Info_Coord[i][1].Pos_X - 5 - 66, B2012_Tyre_Info_Coord[i][1].Pos_Y, 66, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2012_Tyre_Info_Coord[i][1].Pos_X - 5, B2012_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*2, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2012_Tyre_Info_Coord[i][1].Pos_X - 5, B2012_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(TpmsVol_update||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2012_Tyre_Info_Coord[i][1].Pos_X + 15, B2012_Tyre_Info_Coord[i][1].Pos_Y, 44, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2012_Tyre_Info_Coord[i][1].Pos_X + 15, B2012_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2012_Tyre_Info_Coord[i][1].Pos_X + 15, B2012_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
		
	}
	end_draw();
	
}

/* 4轴，12轮胎  eol_drivetype = 5 */
static const COORD_t B2007_Tyre_Info_Coord[B2007_TYRE_NUM][2] = 
{
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 75}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 75}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 140}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 140}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 208}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 208}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 208}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 - 16 + 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84 - 17, TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 354, TPMS_TRUCK_POS_Y + 268}}
	
};
void TPMS_B2007(void)
{
	uint8_t uniStr[64];
	
	// start_draw();
	
	/* 第二轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 153, 80, 3, 0xff8e8e8e);
	
	/* 第三轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 221, 48, 3, 0xff8e8e8e);
	
	/* 第四轴 轴线 */
	loc_RenderRect(376, TPMS_TRUCK_POS_Y + 283, 48, 3, 0xff8e8e8e);
	
	for(uint8_t i = 0; i < B2007_TYRE_NUM; i++)
		loc_RenderImg(B2007_Tyre_Info_Coord[i][0].Pos_X, B2007_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
	
	for(uint8_t i = 0; i < B2007_TYRE_NUM; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xff8e8e8e, 0);
		zk_oem2uni((uint8_t*)"/", uniStr, sizeof(uniStr));
		zk_printZH(B2007_Tyre_Info_Coord[i][1].Pos_X, B2007_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
	}
	
	// end_draw();
	
}
void TPMS_B2007_Update(void)
{
	uint8_t  tyre_status[B2007_TYRE_NUM];
	
	uint16_t tyre_pressure[B2007_TYRE_NUM];
	int16_t  tyre_temperature[B2007_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2007_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2007_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2007_TYRE_NUM];
	
	uint32_t colorf;
	
	start_draw();
	if(learnstatus)
	{
		for(uint8_t i = 0; i < B2007_TYRE_NUM; i++)
		{
			if((i+1) == tire)
				loc_RenderImg(B2007_Tyre_Info_Coord[i][0].Pos_X, B2007_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireLearn);
			else
				loc_RenderImg(B2007_Tyre_Info_Coord[i][0].Pos_X, B2007_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
		}
	}
	else
	{
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
		
		for(uint8_t i = 0; i < B2007_TYRE_NUM; i++)
		{
			if(old_tyre_status[i] != tyre_status[i] || TpmsVol_update)
			{
				old_tyre_status[i] = tyre_status[i];
				
				if(tyre_status[i] != 0)
					loc_RenderImg(B2007_Tyre_Info_Coord[i][0].Pos_X, B2007_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireError);
				else
					loc_RenderImg(B2007_Tyre_Info_Coord[i][0].Pos_X, B2007_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
			}
		}
		
	}
	
	tyre_pressure[0] = Wheels[0][0].pressure;
	tyre_pressure[1] = Wheels[0][1].pressure;
	tyre_pressure[2] = Wheels[1][1].pressure;
	tyre_pressure[3] = Wheels[1][2].pressure;
	tyre_pressure[4] = Wheels[2][0].pressure;
	tyre_pressure[5] = Wheels[2][1].pressure;
	tyre_pressure[6] = Wheels[2][2].pressure;
	tyre_pressure[7] = Wheels[2][3].pressure;
	tyre_pressure[8] = Wheels[3][0].pressure;
	tyre_pressure[9] = Wheels[3][1].pressure;
	tyre_pressure[10] = Wheels[3][2].pressure;
	tyre_pressure[11] = Wheels[3][3].pressure;
	
	tyre_temperature[0] = Wheels[0][0].temperature;
	tyre_temperature[1] = Wheels[0][1].temperature;
	tyre_temperature[2] = Wheels[1][1].temperature;
	tyre_temperature[3] = Wheels[1][2].temperature;
	tyre_temperature[4] = Wheels[2][0].temperature;
	tyre_temperature[5] = Wheels[2][1].temperature;
	tyre_temperature[6] = Wheels[2][2].temperature;
	tyre_temperature[7] = Wheels[2][3].temperature;
	tyre_temperature[8] = Wheels[3][0].temperature;
	tyre_temperature[9] = Wheels[3][1].temperature;
	tyre_temperature[10] = Wheels[3][2].temperature;
	tyre_temperature[11] = Wheels[3][3].temperature;
	
	tyre_timeout_status[0] = (wheelupdatedtimer[0][0]>=30);
	tyre_timeout_status[1] = (wheelupdatedtimer[0][1]>=30);
	tyre_timeout_status[2] = (wheelupdatedtimer[1][1]>=30);
	tyre_timeout_status[3] = (wheelupdatedtimer[1][2]>=30);
	tyre_timeout_status[4] = (wheelupdatedtimer[2][0]>=30);
	tyre_timeout_status[5] = (wheelupdatedtimer[2][1]>=30);
	tyre_timeout_status[6] = (wheelupdatedtimer[2][2]>=30);
	tyre_timeout_status[7] = (wheelupdatedtimer[2][3]>=30);
	tyre_timeout_status[8] = (wheelupdatedtimer[3][0]>=30);
	tyre_timeout_status[9] = (wheelupdatedtimer[3][1]>=30);
	tyre_timeout_status[10] = (wheelupdatedtimer[3][2]>=30);
	tyre_timeout_status[11] = (wheelupdatedtimer[3][3]>=30);
	
	tyre_pressure_status[0] = (TyreLocation_pressureh[0].bit0||TyreLocation_pressurel[0].bit0||TyreLocation_fastlostgas[0].bit0);
	tyre_pressure_status[1] = (TyreLocation_pressureh[0].bit1||TyreLocation_pressurel[0].bit1||TyreLocation_fastlostgas[0].bit1);
	tyre_pressure_status[2] = (TyreLocation_pressureh[1].bit1||TyreLocation_pressurel[1].bit1||TyreLocation_fastlostgas[1].bit1);
	tyre_pressure_status[3] = (TyreLocation_pressureh[1].bit2||TyreLocation_pressurel[1].bit2||TyreLocation_fastlostgas[1].bit2);
	tyre_pressure_status[4] = (TyreLocation_pressureh[2].bit0||TyreLocation_pressurel[2].bit0||TyreLocation_fastlostgas[2].bit0);
	tyre_pressure_status[5] = (TyreLocation_pressureh[2].bit1||TyreLocation_pressurel[2].bit1||TyreLocation_fastlostgas[2].bit1);
	tyre_pressure_status[6] = (TyreLocation_pressureh[2].bit2||TyreLocation_pressurel[2].bit2||TyreLocation_fastlostgas[2].bit2);
	tyre_pressure_status[7] = (TyreLocation_pressureh[2].bit3||TyreLocation_pressurel[2].bit3||TyreLocation_fastlostgas[2].bit3);
	tyre_pressure_status[8] = (TyreLocation_pressureh[3].bit0||TyreLocation_pressurel[3].bit0||TyreLocation_fastlostgas[3].bit0);
	tyre_pressure_status[9] = (TyreLocation_pressureh[3].bit1||TyreLocation_pressurel[3].bit1||TyreLocation_fastlostgas[3].bit1);
	tyre_pressure_status[10] = (TyreLocation_pressureh[3].bit2||TyreLocation_pressurel[3].bit2||TyreLocation_fastlostgas[3].bit2);
	tyre_pressure_status[11] = (TyreLocation_pressureh[3].bit3||TyreLocation_pressurel[3].bit3||TyreLocation_fastlostgas[3].bit3);
	
	tyre_temperature_status[0] = (TyreLocation_temperature[0].bit0);
	tyre_temperature_status[1] = (TyreLocation_temperature[0].bit1);
	tyre_temperature_status[2] = (TyreLocation_temperature[1].bit1);
	tyre_temperature_status[3] = (TyreLocation_temperature[1].bit2);
	tyre_temperature_status[4] = (TyreLocation_temperature[2].bit0);
	tyre_temperature_status[5] = (TyreLocation_temperature[2].bit1);
	tyre_temperature_status[6] = (TyreLocation_temperature[2].bit2);
	tyre_temperature_status[7] = (TyreLocation_temperature[2].bit3);
	tyre_temperature_status[8] = (TyreLocation_temperature[3].bit0);
	tyre_temperature_status[9] = (TyreLocation_temperature[3].bit1);
	tyre_temperature_status[10] = (TyreLocation_temperature[3].bit2);
	tyre_temperature_status[11] = (TyreLocation_temperature[3].bit3);
	
	for(uint8_t i = 0; i < B2007_TYRE_NUM; i++)
	{
		if(TpmsVol_update||old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2007_Tyre_Info_Coord[i][1].Pos_X - 5 - 66, B2007_Tyre_Info_Coord[i][1].Pos_Y, 66, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2007_Tyre_Info_Coord[i][1].Pos_X - 5, B2007_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*2, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2007_Tyre_Info_Coord[i][1].Pos_X - 5, B2007_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(TpmsVol_update||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2007_Tyre_Info_Coord[i][1].Pos_X + 15, B2007_Tyre_Info_Coord[i][1].Pos_Y, 44, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2007_Tyre_Info_Coord[i][1].Pos_X + 15, B2007_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2007_Tyre_Info_Coord[i][1].Pos_X + 15, B2007_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
		
	}
	end_draw();
	
}

/* 3轴，6轮胎  eol_drivetype = 6 */
static const COORD_t B2013_Tyre_Info_Coord[B2013_TYRE_NUM][2] = 
{
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 75}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 75},  {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 75}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 140}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 140}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 140}},
	
	{{TPMS_TRUCK_POS_X + 26 - 16,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X - 82,  TPMS_TRUCK_POS_Y + 268}},
	{{TPMS_TRUCK_POS_X + 26 + 84,      TPMS_TRUCK_POS_Y + 268}, {TPMS_TRUCK_POS_X + 218, TPMS_TRUCK_POS_Y + 268}}
	
};
void TPMS_B2013(void)
{
	uint8_t uniStr[64];
	
	// start_draw();
	
	/* 第二轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 153, 80, 3, 0xff8e8e8e);
	
	/* 第三轴 轴线 */
	loc_RenderRect(360, TPMS_TRUCK_POS_Y + 283, 80, 3, 0xff8e8e8e);
	
	for(uint8_t i = 0; i < B2013_TYRE_NUM; i++)
		loc_RenderImg(B2013_Tyre_Info_Coord[i][0].Pos_X, B2013_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
	
	for(uint8_t i = 0; i < B2013_TYRE_NUM; i++)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xff8e8e8e, 0);
		zk_oem2uni((uint8_t*)"/", uniStr, sizeof(uniStr));
		zk_printZH(B2013_Tyre_Info_Coord[i][1].Pos_X, B2013_Tyre_Info_Coord[i][1].Pos_Y, uniStr);
	}
	
	// end_draw();
	
}
void TPMS_B2013_Update(void)
{
	uint8_t  tyre_status[B2013_TYRE_NUM];
	
	uint16_t tyre_pressure[B2013_TYRE_NUM];
	int16_t  tyre_temperature[B2013_TYRE_NUM];
	
	uint8_t  tyre_timeout_status[B2013_TYRE_NUM];
	uint8_t  tyre_pressure_status[B2013_TYRE_NUM];
	uint8_t  tyre_temperature_status[B2013_TYRE_NUM];
	
	uint32_t colorf;
	
	start_draw();
	if(learnstatus)
	{
		for(uint8_t i = 0; i < B2013_TYRE_NUM; i++)
		{
			if((i+1) == tire)
				loc_RenderImg(B2013_Tyre_Info_Coord[i][0].Pos_X, B2013_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireLearn);
			else
				loc_RenderImg(B2013_Tyre_Info_Coord[i][0].Pos_X, B2013_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
		}
	}
	else
	{
		tyre_status[0] = TyreLocation_pressureh[0].bit0 + TyreLocation_pressurel[0].bit0 + TyreLocation_fastlostgas[0].bit0 + TyreLocation_temperature[0].bit0;
		tyre_status[1] = TyreLocation_pressureh[0].bit1 + TyreLocation_pressurel[0].bit1 + TyreLocation_fastlostgas[0].bit1 + TyreLocation_temperature[0].bit1;
		tyre_status[2] = TyreLocation_pressureh[1].bit1 + TyreLocation_pressurel[1].bit1 + TyreLocation_fastlostgas[1].bit1 + TyreLocation_temperature[1].bit1;
		tyre_status[3] = TyreLocation_pressureh[1].bit2 + TyreLocation_pressurel[1].bit2 + TyreLocation_fastlostgas[1].bit2 + TyreLocation_temperature[1].bit2;
		tyre_status[4] = TyreLocation_pressureh[2].bit1 + TyreLocation_pressurel[2].bit1 + TyreLocation_fastlostgas[2].bit1 + TyreLocation_temperature[2].bit1;
		tyre_status[5] = TyreLocation_pressureh[2].bit2 + TyreLocation_pressurel[2].bit2 + TyreLocation_fastlostgas[2].bit2 + TyreLocation_temperature[2].bit2;
		
		for(uint8_t i = 0; i < B2013_TYRE_NUM; i++)
		{
			if(old_tyre_status[i] != tyre_status[i] || TpmsVol_update)
			{
				old_tyre_status[i] = tyre_status[i];
				
				if(tyre_status[i] != 0)
					loc_RenderImg(B2013_Tyre_Info_Coord[i][0].Pos_X, B2013_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireError);
				else
					loc_RenderImg(B2013_Tyre_Info_Coord[i][0].Pos_X, B2013_Tyre_Info_Coord[i][0].Pos_Y, &Img_tireOk);
			}
		}
		
	}
	
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
	
	for(uint8_t i = 0; i < B2013_TYRE_NUM; i++)
	{
		if(TpmsVol_update||old_tyre_pressure[i] != tyre_pressure[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_pressure[i] = tyre_pressure[i];
			
			LCD_DrawRect(B2013_Tyre_Info_Coord[i][1].Pos_X - 5 - 66, B2013_Tyre_Info_Coord[i][1].Pos_Y, 66, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_pressure[i] <= 1018))
			{
				if(tyre_pressure_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2013_Tyre_Info_Coord[i][1].Pos_X - 5, B2013_Tyre_Info_Coord[i][1].Pos_Y, tyre_pressure[i]*2, 0, ZK_ALIGN_RIGHT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2013_Tyre_Info_Coord[i][1].Pos_X - 5, B2013_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_RIGHT);
				
			}
		}
		
		if(TpmsVol_update||old_tyre_temperature[i] != tyre_temperature[i] || old_tyre_timeout_status[i] != tyre_timeout_status[i])
		{
			old_tyre_temperature[i] = tyre_temperature[i];
			
			LCD_DrawRect(B2013_Tyre_Info_Coord[i][1].Pos_X + 15, B2013_Tyre_Info_Coord[i][1].Pos_Y, 44, 31, BLACK);
			
			if((tyre_timeout_status[i] == 0) && (tyre_temperature[i] <= 250))
			{
				if(tyre_temperature_status[i])
					colorf = 0xffff0000;
				else
					colorf = 0xffffffff;
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, colorf, 0);
				zk_printNum(B2013_Tyre_Info_Coord[i][1].Pos_X + 15, B2013_Tyre_Info_Coord[i][1].Pos_Y, tyre_temperature[i] - 40, 0, ZK_ALIGN_LEFT);
			}
			else
			{
				zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, 0xffffffff, 0);
				zk_printZH1(B2013_Tyre_Info_Coord[i][1].Pos_X + 15, B2013_Tyre_Info_Coord[i][1].Pos_Y, zk_oem2uniStr((uint8_t*)"---"),ZK_ALIGN_LEFT);
				
			}
		}
		
		old_tyre_timeout_status[i] = tyre_timeout_status[i];
		
		
	}
	end_draw();
	
}

void TPMSFrame(unsigned char index, unsigned char page)
{
    uint8_t uniStr[64];
	
	start_draw();
    LCD_Clear();
    disable_box();
    disable_choose_image();
	loc_RenderImg(TPMS_TRUCK_POS_X, TPMS_TRUCK_POS_Y, &Img_tpmsTruck);
	if(eol_language == 0)
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"自学习开启:", uniStr, sizeof(uniStr));
		zk_printZH(40, 20, uniStr);
		zk_oem2uni((uint8_t *)"自学习状态:", uniStr, sizeof(uniStr));
		zk_printZH(410, 20, uniStr);
		zk_oem2uni((uint8_t *)"当前选择轮胎:", uniStr, sizeof(uniStr));
		zk_printZH(40, 430, uniStr);
		zk_oem2uni((uint8_t *)"(kPa/°C)", uniStr, sizeof(uniStr));
		zk_printZH(355, 70, uniStr);
	}
	else
	{
		zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_oem2uni((uint8_t *)"Sensor Learn:", uniStr, sizeof(uniStr));
		zk_printZH1(170, 20, uniStr, ZK_ALIGN_RIGHT);
		zk_oem2uni((uint8_t *)"Learning State:", uniStr, sizeof(uniStr));
		zk_printZH(350, 20, uniStr);
		zk_oem2uni((uint8_t *)"Select the tire:", uniStr, sizeof(uniStr));
		zk_printZH(40, 430, uniStr);
		zk_oem2uni((uint8_t *)"(kPa/°C)", uniStr, sizeof(uniStr));
		zk_printZH(355, 70, uniStr);
	}
	
	if(eol_drivetype==0)
	{
		TPMS_B2001();
	}
	else if(eol_drivetype==1)
	{
		TPMS_B2003();
	}
	else if(eol_drivetype==2)
	{
		TPMS_B2004();
	}
	else if(eol_drivetype==3)
	{
		TPMS_B2006();
	}
	else if(eol_drivetype==4)
	{
		TPMS_B2012();
	}
	else if(eol_drivetype==5)
	{
		TPMS_B2007();
	}
	else if(eol_drivetype==6)
	{
		TPMS_B2013();
	}
	else ;
	end_draw();
	
    TpmsVol_update = 1;
    study = 0;
    tire = 0;
	learnstatus=0;
	
}
void TPMSFrameUpdate(unsigned char index)
{
	static uint8_t clearcnt=0;
	static uint8_t oldstatus[3]={88,88,88};
	
	const uint8_t *valTbl_set_status[2][2] =
	{
		{(uint8_t *)"关闭", (uint8_t *)"OFF"},
		{(uint8_t *)"开启", (uint8_t *)"ON"},
    };
    const uint8_t *valTbl_study_status[][2] =
	{
		{(uint8_t *)"未启动",               (uint8_t *)"Inactive"},
		{(uint8_t *)"启动",                 (uint8_t *)"Activate"},
		{(uint8_t *)"学习中，请激活传感器", (uint8_t *)"Please  Activate sensor"},
		{(uint8_t *)"学习成功",             (uint8_t *)"Learning success"},
		{(uint8_t *)"失败:学习超时",        (uint8_t *)"Failure:time out"},
		{(uint8_t *)"失败:ID冲突",          (uint8_t *)"Failure:ID conflict"},
		{(uint8_t *)"失败:发动机转速不为0", (uint8_t *)"Failure:Engine speed is not 0"},
		{(uint8_t *)"失败:制动踏板信号OFF", (uint8_t *)"Failure:Brake pedal signal OFF"},
		{(uint8_t *)"失败:车速不为0",       (uint8_t *)"Failure:Vehicle speed is not 0"},
		{(uint8_t *)"学习失败9",            (uint8_t *)"Learning failure"},
		{(uint8_t *)"学习失败A",            (uint8_t *)"Learning failure"},
		{(uint8_t *)"学习失败B",            (uint8_t *)"Learning failure"},
		{(uint8_t *)"学习失败C",            (uint8_t *)"Learning failure"},
		{(uint8_t *)"学习失败D",            (uint8_t *)"Learning failure"},
		{(uint8_t *)"学习失败E",            (uint8_t *)"Learning failure"},
		{(uint8_t *)"学习失败F",            (uint8_t *)"Learning failure"},
		{(uint8_t *)"学习失败",             (uint8_t *)"Learning failure"},
    };
	
	const uint8_t *valTbl_tire0[][2] =
	{
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
		{(uint8_t *)"第一轴左轮",   (uint8_t *)"1st axle left tire"},
		{(uint8_t *)"第一轴右轮",   (uint8_t *)"1st axle right tire"},
		{(uint8_t *)"第二轴左外轮", (uint8_t *)"2nd axle left outer tire"},
		{(uint8_t *)"第二轴左内轮", (uint8_t *)"2nd axle left inner tire"},
		{(uint8_t *)"第二轴右内轮", (uint8_t *)"2nd axle right inner tire"},
		{(uint8_t *)"第二轴右外轮", (uint8_t *)"2nd axle right outer tire"},
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
    };
	const uint8_t *valTbl_tire1[][2] =
	{
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
		{(uint8_t *)"第一轴左轮",   (uint8_t *)"1st axle left tire"},
		{(uint8_t *)"第一轴右轮",   (uint8_t *)"1st axle right tire"},
		{(uint8_t *)"第二轴左轮",   (uint8_t *)"2nd axle left tire"},
		{(uint8_t *)"第二轴右轮",   (uint8_t *)"2nd axle right tire"},
		{(uint8_t *)"第三轴左外轮", (uint8_t *)"3rd axle left outer tire"},
		{(uint8_t *)"第三轴左内轮", (uint8_t *)"3rd axle left inner tire"},
		{(uint8_t *)"第三轴右内轮", (uint8_t *)"3rd axle right inner tire"},
		{(uint8_t *)"第三轴右外轮", (uint8_t *)"3rd axle right outer tire"},
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
		
    };
    const uint8_t *valTbl_tire2[][2] =
	{
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
		{(uint8_t *)"第一轴左轮",   (uint8_t *)"1st axle left tire"},
		{(uint8_t *)"第一轴右轮",   (uint8_t *)"1st axle right tire"},
		{(uint8_t *)"第二轴左外轮", (uint8_t *)"2nd axle left outer tire"},
		{(uint8_t *)"第二轴左内轮", (uint8_t *)"2nd axle left inner tire"},
		{(uint8_t *)"第二轴右内轮", (uint8_t *)"2nd axle right inner tire"},
		{(uint8_t *)"第二轴右外轮", (uint8_t *)"2nd axle right outer tire"},
		{(uint8_t *)"第三轴左外轮", (uint8_t *)"3rd axle left outer tire"},
		{(uint8_t *)"第三轴左内轮", (uint8_t *)"3rd axle left inner tire"},
		{(uint8_t *)"第三轴右内轮", (uint8_t *)"3rd axle right inner tire"},
		{(uint8_t *)"第三轴右外轮", (uint8_t *)"3rd axle right outer tire"},
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
    };
	const uint8_t *valTbl_tire3[][2] =
	{
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
		{(uint8_t *)"第一轴左轮",   (uint8_t *)"1st axle left tire"},
		{(uint8_t *)"第一轴右轮",   (uint8_t *)"1st axle right tire"},
		{(uint8_t *)"第二轴左轮",   (uint8_t *)"2nd axle left tire"},
		{(uint8_t *)"第二轴右轮",   (uint8_t *)"2nd axle right tire"},
		{(uint8_t *)"第三轴左轮",   (uint8_t *)"3rd axle left tire"},
		{(uint8_t *)"第三轴右轮",   (uint8_t *)"3rd axle right tire"},
		{(uint8_t *)"第四轴左外轮", (uint8_t *)"4th axle left outer tire"},
		{(uint8_t *)"第四轴左内轮", (uint8_t *)"4th axle left inner tire"},
		{(uint8_t *)"第四轴右内轮", (uint8_t *)"4th axle right inner tire"},
		{(uint8_t *)"第四轴右外轮", (uint8_t *)"4th axle right outer tire"},
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
    };
	const uint8_t *valTbl_tire4[][2] =
	{
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
		{(uint8_t *)"第一轴左轮",   (uint8_t *)"1st axle left tire"},
		{(uint8_t *)"第一轴右轮",   (uint8_t *)"1st axle right tire"},
		{(uint8_t *)"第二轴左轮",   (uint8_t *)"2nd axle left tire"},
		{(uint8_t *)"第二轴右轮",   (uint8_t *)"2nd axle right tire"},
		{(uint8_t *)"第三轴左外轮", (uint8_t *)"3rd axle left outer tire"},
		{(uint8_t *)"第三轴左内轮", (uint8_t *)"3rd axle left inner tire"},
		{(uint8_t *)"第三轴右内轮", (uint8_t *)"3rd axle right inner tire"},
		{(uint8_t *)"第三轴右外轮", (uint8_t *)"3rd axle right outer tire"},
		{(uint8_t *)"第四轴左轮",   (uint8_t *)"4th axle left tire"},
		{(uint8_t *)"第四轴右轮",   (uint8_t *)"4th axle right tire"},
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
    };
	const uint8_t *valTbl_tire5[][2] =
	{
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
		{(uint8_t *)"第一轴左轮",   (uint8_t *)"1st axle left tire"},
		{(uint8_t *)"第一轴右轮",   (uint8_t *)"1st axle right tire"},
		{(uint8_t *)"第二轴左轮",   (uint8_t *)"2nd axle left tire"},
		{(uint8_t *)"第二轴右轮",   (uint8_t *)"2nd axle right tire"},
		{(uint8_t *)"第三轴左外轮", (uint8_t *)"3rd axle left outer tire"},
		{(uint8_t *)"第三轴左内轮", (uint8_t *)"3rd axle left inner tire"},
		{(uint8_t *)"第三轴右内轮", (uint8_t *)"3rd axle right inner tire"},
		{(uint8_t *)"第三轴右外轮", (uint8_t *)"3rd axle right outer tire"},
		{(uint8_t *)"第四轴左外轮", (uint8_t *)"4th axle left outer tire"},
		{(uint8_t *)"第四轴左内轮", (uint8_t *)"4th axle left inner tire"},
		{(uint8_t *)"第四轴右内轮", (uint8_t *)"4th axle right inner tire"},
		{(uint8_t *)"第四轴右外轮", (uint8_t *)"4th axle right outer tire"},
		{(uint8_t *)"NULL",         (uint8_t *)"NULL"},
    };
	const uint8_t *valTbl_tire6[][2] =
	{
		{(uint8_t *)"NULL",       (uint8_t *)"NULL"},
		{(uint8_t *)"第一轴左轮", (uint8_t *)"1st axle left tire"},
		{(uint8_t *)"第一轴右轮", (uint8_t *)"1st axle right tire"},
		{(uint8_t *)"第二轴左轮", (uint8_t *)"2nd axle left tire"},
		{(uint8_t *)"第二轴右轮", (uint8_t *)"2nd axle right tire"},
		{(uint8_t *)"第三轴左轮", (uint8_t *)"3rd axle left tire"},
		{(uint8_t *)"第三轴右轮", (uint8_t *)"3rd axle right tire"},
		{(uint8_t *)"NULL",       (uint8_t *)"NULL"},
    };
	start_draw();
	
	if(oldstatus[0] != learnstatus||TpmsVol_update)
	{
		oldstatus[0] = learnstatus;
		loc_ClearRect(180, 20, 70, 31);
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
		zk_printZKAlign(180,  20, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_set_status[learnstatus][eol_language]));
    }
	
	if(study == 1)
	{
		if(clearcnt==0)
		{
			clearcnt = 1;
			
			oldstatus[1] = 88;
		}
		TpmsVol_update = 1;
		if(oldstatus[1] != backvalue||TpmsVol_update)
		{
			oldstatus[1] = backvalue;
			loc_ClearRect(580-30, 20, 250, 31);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printZKAlign(580-30,  20, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_study_status[backvalue][eol_language]));
			
		}
	}
	else
	{
		clearcnt=0;
		
		if(oldstatus[1] != backvalue||TpmsVol_update)
		{
			oldstatus[1] = backvalue;
			loc_ClearRect(580-30, 20, 250, 31);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			zk_printZKAlign(580-30,  20, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_study_status[backvalue][eol_language]));
		}
	}
	
    
	if((oldstatus[2] != tire && learnstatus==1)||TpmsVol_update)
	{
		oldstatus[2] = tire;
		loc_ClearRect(210, 430, 300, 31);
		
		zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);	
		if(eol_drivetype == 0)
		{
			zk_printZKAlign(210,  430, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_tire0[tire][eol_language]));
		}
		else if(eol_drivetype == 1)
		{
			zk_printZKAlign(210,  430, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_tire1[tire][eol_language]));
		}
		else if(eol_drivetype == 2)
		{
			zk_printZKAlign(210,  430, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_tire2[tire][eol_language]));
		}
		else if(eol_drivetype == 3)
		{
			zk_printZKAlign(210,  430, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_tire3[tire][eol_language]));
		}
		else if(eol_drivetype == 4)
		{
			zk_printZKAlign(210,  430, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_tire4[tire][eol_language]));
		}
		else if(eol_drivetype == 5)
		{
			zk_printZKAlign(210,  430, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_tire5[tire][eol_language]));
		}
		else if(eol_drivetype == 6)
		{
			zk_printZKAlign(210,  430, ZK_ALIGN_LEFT, zk_oem2uniStr(valTbl_tire6[tire][eol_language]));
		}
		else ;
    }
	
	end_draw();
    
    ClrWdt();
	
	if(eol_drivetype==0)
	{
		TPMS_B2001_Update();
	}
	else if(eol_drivetype==1)
	{
		TPMS_B2003_Update();
	}
	else if(eol_drivetype==2)
	{
		TPMS_B2004_Update();
	}
	else if(eol_drivetype==3)
	{
		TPMS_B2006_Update();
	}
	else if(eol_drivetype==4)
	{
		TPMS_B2012_Update();
	}
	else if(eol_drivetype==5)
	{
		TPMS_B2007_Update();
	}
	else if(eol_drivetype==6)
	{
		TPMS_B2013_Update();
	}
	else ;
	
	TpmsVol_update = 0;
    ClrWdt();
	
    if (key_up)
	{
		key_up = 0;
		study = 0;
		backvalue=0;
		oldstatus[1] = 88;
		
		tire++;
		if(eol_drivetype == 0)
		{
			if(tire >7)tire=1;
		}
		else if(eol_drivetype == 1)
		{
			if(tire >9)tire=1;
		}
		else if(eol_drivetype == 2)
		{
			if(tire >11)tire=1;
		}
		else if(eol_drivetype == 3)
		{
			if(tire >11)tire=1;
		}
		else if(eol_drivetype == 4)
		{
			if(tire >11)tire=1;
		}
		else if(eol_drivetype == 5)
		{
			if(tire >13)tire=1;
		}
		else if(eol_drivetype == 6)
		{
			if(tire >7)tire=1;
		}
		else tire=0;
    }

    if (key_set)
	{
		key_set = 0;
		if(learnstatus==0)
		{
			if(study==0)
				learnstatus=1;
			
		}
		else if(learnstatus==1)
		{
			if((eol_drivetype == 0&&tire>0&&tire <7)
		    || (eol_drivetype == 1&&tire>0&&tire <9)
		    || (eol_drivetype == 2&&tire>0&&tire <11)
		    || (eol_drivetype == 3&&tire>0&&tire <11)
		    || (eol_drivetype == 4&&tire>0&&tire <11)
		    || (eol_drivetype == 5&&tire>0&&tire <13)
			|| (eol_drivetype == 6&&tire>0&&tire <7))
			{
				study = 1;
				StartLearnflag = 1;
			}
			else
			{
				study = 0;
				learnstatus = 0;
				backvalue = 0;
			}
		}
		else
		{
			learnstatus=0;
			study=0;
		}
    }

    if (key_AV)
	{
		key_AV = 0;
		study = 0;
		backvalue=0;
		oldstatus[1] = 88;
		
		if(tire >0)
		{
			tire--;
		}
		else 
		{
			if(0x00 == eol_drivetype)
			{
				tire = 6;
			}
			else if(0x01 == eol_drivetype)
			{
				tire = 8;
			}
			else if(0x02 == eol_drivetype)
			{
				tire = 10;
			}
			else if(0x03 == eol_drivetype)
			{
				tire = 10;
			}
			else if(0x04 == eol_drivetype)
			{
				tire = 10;
			}
			else if(0x05 == eol_drivetype)
			{
				tire = 12;
			}
			else if(0x06 == eol_drivetype)
			{
				tire = 6;
			}
			else tire = 0;
		}
    }
}


/** 充电信息 **/
void ChargeFrame(void)
{
	
}
void update_charge(void)
{
	static uint8_t  BMSErrorPre = 0x00;
	uint8_t BMSError = 0x00;
	
	static ptmrType_t chg_bar_tmr = 0;
	static uint8_t  MovePos = 0x00;
	
	static ptmrType_t flt_code_Tmr = 0;
	static uint8_t WarnIndex = 0xFF;
	
	static uint16_t Remain_timePre = 0xFFFF;
	uint16_t Remain_time = 0;
	uint8_t  Remain_hour = 0;
	uint8_t  Remain_min = 0;
	
	static uint8_t  ChargeStatusPre = 0x00;
	uint8_t  ChargeStatus = 0;
	
	static uint32_t voltagePre = 0x00;
	uint32_t chg_volt = 0;
	
	static int32_t currentPre = 0x00;
	int32_t  chg_curr = 0;
	
	static uint16_t socPre = 0x00;
	uint8_t  chg_soc = 0;
	
	static uint8_t  ChgFltUpdateFlg = 0x00;
	
	if(ChargeViewEnable == 0)
	{
		start_draw();
		LCD_Clear();
		display_Charge_BG();
		// loc_RenderImg(294, 143, &Img_SOC_BG);	
		end_draw();
		
		MovePos = 0;
		
		forceupdate = 1;
		
		BMSErrorPre = 0;
		Remain_timePre = 0;
		ChargeStatusPre = 0;
		voltagePre = 0;
		currentPre = 0;
		socPre = 0;
		
		ChgFltUpdateFlg = 1;
		
		ChrgViewUpdate = 1;
		
		ChargeViewEnable = 1;
	
		f_index = 0;
		bg_shown = 0;
		
	}
	
	ChargeStatus = get_charge_status();
	BMSError = ((ChargeStatus != ParkingCharge) && (bmsCrtFltNum > 0));
	
	if(ChgFltUpdateFlg || BMSErrorPre != BMSError)
	{
		BMSErrorPre = BMSError;
		ChgFltUpdateFlg = 0;
		
		start_draw();
		
		MovePos = 0;
		LCD_DrawRect(294, 143, 234, 94, TOUMING);
		LCD_DrawRect(60, 275, 692, 115, TOUMING);
		
		if(BMSError == 0)
		{	
			loc_RenderImg(64, 314 , &Img_line_white);
			loc_RenderImg(64, 376, &Img_line_white);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			if(eol_language == 0)
			{
				zk_printZH(188, 282, zk_oem2uniStr((uint8_t*)"充电电压:"));
				zk_printPosNum(355, 282,0, 0, ZK_ALIGN_RIGHT);	
				zk_printZH(355, 282, zk_oem2uniStr((uint8_t*)"V"));
				
				zk_printZH(424, 282, zk_oem2uniStr((uint8_t*)"充电电流:"));
				zk_printPosNum(608, 282,0, 0, ZK_ALIGN_RIGHT);	
				zk_printZH(608, 282, zk_oem2uniStr((uint8_t*)"A"));
				
				zk_printZH(188, 345, zk_oem2uniStr((uint8_t*)"SOC:"));
				zk_printPosNum(288, 345,0, 0, ZK_ALIGN_RIGHT);	
				zk_printZH(288, 345, zk_oem2uniStr((uint8_t*)"%"));
				
				// zk_printZH(370, 345, zk_oem2uniStr((uint8_t*)"剩余充电时间:"));
				
			}
			else
			{
				zk_printZH(188, 282, zk_oem2uniStr((uint8_t*)"Volt:"));
				zk_printPosNum(355, 282,0, 0, ZK_ALIGN_RIGHT);
				zk_printZH(355, 282, zk_oem2uniStr((uint8_t*)"V"));
				
				zk_printZH(424, 282, zk_oem2uniStr((uint8_t*)"Curr:"));
				zk_printPosNum(608, 282,0, 0, ZK_ALIGN_RIGHT);
				zk_printZH(608, 282, zk_oem2uniStr((uint8_t*)"A"));
				
				zk_printZH(188, 345, zk_oem2uniStr((uint8_t*)"SOC:"));
				zk_printPosNum(288, 345,0, 0, ZK_ALIGN_RIGHT);
				zk_printZH(288, 345, zk_oem2uniStr((uint8_t*)"%"));
				
				// zk_printZH(370, 345, zk_oem2uniStr((uint8_t*)"Remaining time:"));
			}
		}
		else
		{
			loc_RenderImg_BlendMode(384, 159, &Img_ChgFlt, R_DRW2D_BLENDMODE_CUSTOM); 
			loc_RenderImg(64, 314 , &Img_line_red);
		}
		end_draw();
	}
	
	if(BMSError == 0)
	{
		chg_volt = get_batt_pack_volt();
		chg_curr = get_batt_pack_curr();
		chg_soc  = get_batt_pack_soc();
		
		start_draw();
		if(ChrgViewUpdate || voltagePre != chg_volt)
		{
			voltagePre = chg_volt;
			
			LCD_DrawRect(298, 282, 57, 31,TOUMING);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			if(chg_volt > 0xFAFF)
			{
				zk_printZH(317, 282, zk_oem2uniStr((uint8_t*)"--"));
			}
			else
			{
				zk_printPosNum(355, 282, chg_volt/10, 0, ZK_ALIGN_RIGHT);
			}
		}
		
		if(ChrgViewUpdate || currentPre != chg_curr)
		{
			currentPre = chg_curr;
			
			LCD_DrawRect(537, 282, 71, 31,TOUMING); 
			
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			if(chg_curr > 0)
			{
				zk_printZH(548, 282, zk_oem2uniStr((uint8_t*)"--"));
			}
			else
			{
				zk_printNum(608, 282, chg_curr/10, 0, ZK_ALIGN_RIGHT);
			}
		}
		
		if(ChrgViewUpdate || socPre != chg_soc)
		{
			socPre = chg_soc;
			
			LCD_DrawRect(248, 345, 40, 31,TOUMING); 
			
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			if(chg_soc > 120)
			{
				zk_printZH(258, 345, zk_oem2uniStr((uint8_t*)"--"));
			}
			else
			{
				zk_printPosNum(288, 345, chg_soc, 0, ZK_ALIGN_RIGHT);	
			}
		}
		
		if(ChrgViewUpdate || ChargeStatusPre != ChargeStatus)
		{
			ChargeStatusPre = ChargeStatus;
			
			LCD_DrawRect(370, 345, 400, 32,TOUMING); 
			
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
			if(ChargeStatus == ParkingCharge) //停车充电
			{
				MovePos = 0;
				LCD_DrawRect(294, 143, 234, 94, TOUMING);
				
			}
			else if(ChargeStatus == ChargingOK) //充电完成
			{
				if(eol_language == 0)
				{
					zk_printZH(450, 345, zk_oem2uniStr((uint8_t*)"充电完成"));
				}
				else
				{
					zk_printZH(450, 345, zk_oem2uniStr((uint8_t*)"charge complete"));
				}
				
				MovePos = 0;
				LCD_DrawRect(294, 143, 234, 94, TOUMING);
				for(uint16_t i = 0; i < (chg_soc * 194 / 100); i++)
					loc_RenderImg_BlendMode(310 + i, 146, &Img_soc_2px, R_DRW2D_BLENDMODE_LIGHTEN);
				loc_RenderImg_BlendMode(394,159, &Img_ChgNormal, R_DRW2D_BLENDMODE_CUSTOM);		
			}
			else if(ChargeStatus == NotCharged) //未充电
			{
				if(eol_language == 0)
				{
					zk_printZH(450, 345, zk_oem2uniStr((uint8_t*)"未充电"));
				}
				else
				{
					zk_printZH(450, 345, zk_oem2uniStr((uint8_t*)"Not charging"));
				}
				
				MovePos = 0;
				LCD_DrawRect(294, 143, 234, 94, TOUMING);
				for(uint16_t i = 0; i < 194; i++)
					loc_RenderImg_BlendMode(310 + i, 146, &Img_soc_2px, R_DRW2D_BLENDMODE_LIGHTEN);
				loc_RenderImg_BlendMode(394,159, &Img_ChgNormal, R_DRW2D_BLENDMODE_CUSTOM);	
			}	
			else ;
		}
		
		if(ChargeStatus == ParkingCharge) //停车充电
		{
			Remain_time = get_remain_chg_time();
			if(ChrgViewUpdate || Remain_timePre != Remain_time)
			{
				Remain_timePre = Remain_time;
				
				LCD_DrawRect(370, 345, 400, 32, TOUMING); 
				
				if(Remain_time != 0xffffu)
				{
					if(Remain_time < 600)
					{
						Remain_hour = Remain_time / 60;
						Remain_min = Remain_time % 60;
					}
					else
					{
						Remain_hour = 10;
						Remain_min = 0;
					}
					
					zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
					if(eol_language == 0)
					{
						zk_printZH(370, 345, zk_oem2uniStr((uint8_t*)"剩余充电时间:"));
						zk_printPosNum(560, 345,Remain_hour, 0, ZK_ALIGN_RIGHT);
						zk_printPosNum(640, 345,Remain_min, 0, ZK_ALIGN_RIGHT);
						zk_printZH(560, 345, zk_oem2uniStr((uint8_t*)"小时"));
						zk_printZH(640, 345, zk_oem2uniStr((uint8_t*)"分"));
					}
					else
					{
						zk_printZH(370, 345, zk_oem2uniStr((uint8_t*)"Remaining time:"));
						zk_printPosNum(560 + 30, 345,Remain_hour, 0, ZK_ALIGN_RIGHT);
						zk_printPosNum(640, 345,Remain_min, 0, ZK_ALIGN_RIGHT);
						zk_printZH(560+30, 345, zk_oem2uniStr((uint8_t*)"h"));
						zk_printZH(640, 345, zk_oem2uniStr((uint8_t*)"min"));
					}
				}
			}
			
			if((ptmr_Get(&chg_bar_tmr) >= 25) || (!ptmr_IsStart(&chg_bar_tmr)))
			{
				ptmr_Start(&chg_bar_tmr);
				
				if(MovePos < 194)
				{
					MovePos++;
					loc_RenderImg_BlendMode(310 + MovePos, 146, &Img_soc_2px, R_DRW2D_BLENDMODE_LIGHTEN);
				}
				else
				{
					MovePos = 0;
					LCD_DrawRect(294, 143, 234, 94, TOUMING);
				}
				loc_RenderImg_BlendMode(394,159, &Img_ChgNormal, R_DRW2D_BLENDMODE_CUSTOM);
			}
		}
		end_draw();
		
		ChrgViewUpdate = 0;
	}
	else
	{
		ChrgViewUpdate = 1;
		
		if((ptmr_Get(&flt_code_Tmr) >= 3000) || (!ptmr_IsStart(&flt_code_Tmr)))
		{
			ptmr_Start(&flt_code_Tmr);
			
			uint8_t uniStrTemp[90];
			
			start_draw();
			LCD_DrawRect(10, 280, 780, 34,TOUMING);
			zk_init(loc_Drw2dDev, (GUI_FONT*)&GUI_Fontwryh31, VM_COLOR_RED, 0);
			zk_oem2uni(DMPackage[bmsListCrtFltPos[WarnIndex]].msg[eol_language], uniStrTemp, sizeof(uniStrTemp));
			zk_printZH(400 - (zk_GetStrLen(uniStrTemp)/2), 284, uniStrTemp);
			end_draw();
			
			WarnIndex++;
			if(WarnIndex >= bmsCrtFltNum)
				WarnIndex = 0;
			
		}
	}
}
void LCD_Charge(void)
{
	uint8_t uniStr[64];
	
	if(CAN_CHARGE_LINE)
	{
		ChuiSaoViewEnable = 0;
		SmartRechgViewFlg = 0;
		GetOffReminderViewFlg = 0;
		update_charge();
		ClrWdt();
	}
	else
	{
		ChargeViewEnable = 0;
		
		if(CAN_H2_ChuiSao    /*吹扫进行中*/
		|| CAN_H2_LowTptChg) /*低温停车充电中*/
		{
			if(ChuiSaoViewEnable == 0)
			{
				ClrWdt();
				start_draw();
				LCD_Clear();
				disable_BG();
				
				zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_printZH(5, 438, zk_oem2uniStr((uint8_t*)"TRIP"));
				zk_printZH(571, 438, zk_oem2uniStr((uint8_t*)"ODO"));
				if(0U == e_uint)
				{
					zk_oem2uni((uint8_t *)"km", uniStr, sizeof(uniStr));
					zk_printZH(175, 438, uniStr);
					zk_printZH(745, 438, uniStr);
				}
				else
				{
					zk_oem2uni((uint8_t *)"mile", uniStr, sizeof(uniStr));
					zk_printZH(175, 438, uniStr);
					zk_printZH(745, 438, uniStr);
				}
				Update_Miles();
				
				end_draw();
				
				StepperMotormiddle();
				ClrWdt();
				GetOffReminderViewFlg = 0;
				SmartRechgViewFlg = 0;
				ChuiSaoViewEnable = 1;
			}
			
			start_draw();
			LCD_DrawRect(335, 205, 160, 30, TOUMING);
			zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_GREEN, 0);
			if(CAN_H2_LowTptChg)  /*低温停车充电中*/
			{
				if(eol_language == 0)
				{
					zk_printZH(335, 205, zk_oem2uniStr((uint8_t*)"低温停车充电"));
				}
				else
				{
					zk_printZH(335, 205, zk_oem2uniStr((uint8_t*)"Charging at low Temp"));
				}
			}
			else if(CAN_H2_ChuiSao) /*吹扫进行中*/
			{
				if(eol_language == 0)
				{
					zk_printZH(345, 205, zk_oem2uniStr((uint8_t*)"吹扫进行中"));
				}
				else
				{
					zk_printZH(345, 205, zk_oem2uniStr((uint8_t*)"Purging in progress"));
				}
			}
			else ;
			end_draw();
			
			Display_Temperature();
			Display_Date();
			
		}
		#if 0
		else if(CAN_KEY_ACC && SmartRechgCfgFlg)
		{
			if(!SmartRechgViewFlg)
			{
				ClrWdt();
				start_draw();
				LCD_Clear();
				display_Charge_BG();
				
				zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				if(eol_language == 0)
				{
					zk_printZH(300, 282, zk_oem2uniStr((uint8_t*)"SOC "));
					zk_printZH(450, 282, zk_oem2uniStr((uint8_t*)"%"));
					
					zk_printZH(300, 345, zk_oem2uniStr((uint8_t*)"低压蓄电池电压 "));
					zk_printZH(560, 345, zk_oem2uniStr((uint8_t*)"V"));
				}
				else
				{
					zk_printZH(300, 282, zk_oem2uniStr((uint8_t*)"SOC:"));
					zk_printZH(450, 282, zk_oem2uniStr((uint8_t*)"%"));
					
					zk_printZH(300, 345, zk_oem2uniStr((uint8_t*)"Auxil.Batt.Volt. "));
					zk_printZH(560, 345, zk_oem2uniStr((uint8_t*)"V"));
				}
				end_draw();
				
				GetOffReminderViewFlg = 0;
				// SmartRechgViewFlg = 0;
				ChuiSaoViewEnable = 0;
				
			}
			
			static uint8_t old_soc;
			uint8_t soc;
			static uint8_t old_aux_bat_volt;
			uint16_t aux_bat_volt;
			uint32_t color;
			
			start_draw();
			//动力电池SOC
			soc  = get_batt_pack_soc();
			if(!SmartRechgViewFlg || old_soc != soc)
			{
				old_soc = soc;
				
				LCD_DrawRect(294, 143, 234, 94, TOUMING);
				LCD_DrawRect(385, 282, 60, 31,TOUMING); 
				
				zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				if(soc > 120)
				{
					zk_printZH1(445, 282, zk_oem2uniStr((uint8_t*)"--"), ZK_ALIGN_RIGHT);
				}
				else
				{
					zk_printPosNum(445, 282, soc, 0, ZK_ALIGN_RIGHT);	
					
					if(soc <= 10)
					{
						color = 0xffff0000;
					}
					else if(soc <= 20)
					{
						color = 0xffffff00;
					}
					else
					{
						color = 0xff00ff00;
					}
					
					if(soc > 100) soc = 100;
					loc_RenderRect(304, 153, (soc * 198 / 100), 73, color);
				}
			}
			
			//低压电池电压
			aux_bat_volt = get_aux_bat_volt();
			if(!SmartRechgViewFlg || old_aux_bat_volt != aux_bat_volt)
			{
				old_aux_bat_volt = aux_bat_volt;
				
				LCD_DrawRect(495, 345, 60, 31,TOUMING);
				zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_WHITE, 0);
				zk_printPosNum(555, 345, aux_bat_volt, 1, ZK_ALIGN_RIGHT);
			}
			end_draw();
			
			SmartRechgViewFlg = 1;
			
		}
		#endif
		else if(GetOffReminderFlg)
		{
			if(!GetOffReminderViewFlg)
			{
				ClrWdt();
				start_draw();
				LCD_Clear();
				disable_BG();
				
				zk_init(loc_Drw2dDev, &GUI_Fontwryh31, VM_COLOR_RED, 0);
				if(eol_language == 0)
				{
					zk_printZH(200, 240, zk_oem2uniStr((uint8_t*)"下车时，请将点火钥匙拧到 OFF 档"));
				}
				else
				{
					zk_printZH(200, 240, zk_oem2uniStr((uint8_t*)"Getting off, Please turn the key off"));
				}
				
				end_draw();
				
				
				// GetOffReminderViewFlg = 0;
				SmartRechgViewFlg = 0;
				ChuiSaoViewEnable = 0;
			}
			
			
			GetOffReminderViewFlg = 1;
		}
	}
	
	if(LED_LocationLight)
		PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd_night, PWM_PERIOD);//980hz
	else
		PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd, PWM_PERIOD);//980hz
	
}


void LCD_Exec(void)
{
    static uint8_t old_backtomain = 0;
    static uint8_t frmIDPre = 0x00;
	static ptmrType_t spd_show_Tmr = 0;
	
    switch (frmID)
	{
        case Main:	
			if((ptmr_Get(&spd_show_Tmr) >= 50) || (!ptmr_IsStart(&spd_show_Tmr)) || mainview_update)
			{
				ptmr_Start(&spd_show_Tmr);
				loc_speedUpdate();
			}
			
			if(!bg_shown)
			{
				show_BG();
				bg_shown = 1;
			}
			frmMain();
			break;
        case Menu_VehicleInfo:
			mainview_update = 1;
			ready_not_shown = 1;
			if(bg_shown)
				disable_BG();
			disable_choose();
			frmMenuVehicleInfo();
			break;
		case Menu_TripInfo:
			mainview_update = 1;
			ready_not_shown = 1;
			if(bg_shown)
				disable_BG();
			disable_choose();
			frmMenuTripInfo();
			break;
		case Menu_PSNInfo:
			mainview_update = 1;
			ready_not_shown = 1;
			if(bg_shown)
				disable_BG();
			disable_choose();
			frmMenuPSNInfo();
			break;
		case Menu_IC:
			mainview_update = 1;
			ready_not_shown = 1;
			alarm_update_flag = 0;
			if(bg_shown)
				disable_BG();
			disable_choose();
			frmMenuICInfo();
			break;
		case Kvalue:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(KValueFrame, KvalueUpdate, 1);
			break;
		case BatVol:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(BatVolFrame, BatVolFrameUpdate, 1);
			break;
		case BrakeInfo:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(BrakeInfoFrame, BrakeInfoFrameUpdate, 1);
			break;
		case FaultInfo: 
			Gennal_Use[2].byte = 0;
			mainview_update = 1;

			if(faultcodeupdate < 1)
			{
				faultcodeupdate++;
			}
			frmpage(DMFrame, DMFrameUpdate, 1);
			break;
		case TPMS:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			if((eol_tpmsyesno > 0) && (eol_tpmsyesno < 6))
				frmpage(TPMSFrame, TPMSFrameUpdate, 1);
			else
				frmpage(UndefinedFrame, UndefinedFrameUpdate, 1);
			break;
		case Maintain:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(UndefinedFrame, UndefinedFrameUpdate, 1);
			break;
		case VCUVersion:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(ControllerVersionFrame, ControllerVersionFrameUpdate, 1);
			break;
		case Supplier:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(SupplierFrame, UndefinedFrameUpdate, 1);
			break;
		case FCUWrkTm:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(FCUFrame, FCUFrameUpdate, 1);
			break;
		case HMSMaxPress:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(HMSFrame, HMSFrameUpdate, 1);
			break;
		
		case Trip:
			Gennal_Use[2].byte = 0;
			mainview_update = 1;
			frmpage(TripInfoFrame, TripInfoFrameUpdate, 1);
			break;

		case WorkingTime:
			Gennal_Use[2].byte = 0;
			frmpage(WorkTimeFrame, WorkTimeFrameUpdate, 1);
			break;

		case AveSpeed:
			Gennal_Use[2].byte = 0;
			frmpage(AveSpeedFrame, AveSpeedFrameUpdate, 1);
			break;

		case DateTime:
			Gennal_Use[2].byte = 0;
			frmpage(TimeFrame, TimeFrameUpdate, 1);
			break;
		
		
		case MotorVol:
			Gennal_Use[2].byte = 0;
			frmpage(MotorVolFrame, MotorVolFrameUpdate, 1);
			break;
		case ZhouHe:
			Gennal_Use[2].byte = 0;
			frmpage(ZhouHeFrame, ZhouHeFrameUpdate, 1);
			break;

		case PowerCons:
			Gennal_Use[2].byte = 0;
			frmpage(PowerConsFrame, PowerConsFrameUpdate, 1);
			break;

		case Language:
			Gennal_Use[2].byte = 0;
			// frmpage(UndefinedFrame, UndefinedFrameUpdate, 1);
			frmpage(LanguageFrame, LanguageFrameUpdate, 1);
			break;

		case Unit:
			Gennal_Use[2].byte = 0;
			frmpage(UnitFrame, UnitFrameUpdate, 1);
			break;

		case Dispeffect:
			Gennal_Use[2].byte = 0;
			frmpage(DispeffectFrame, DispeffectFrameUpdate, 1);
			break;

		case DispInfo:
			Gennal_Use[2].byte = 0;
			frmpage(ConfigDisFrame, ConfigDisFrameUpdate, 1);
			break;

		case TimeDisp:
			Gennal_Use[2].byte = 0;
			frmpage(TimeShowFrame, TimeShowFrameUpdate, 1);
			break;

		case Needle:
			Gennal_Use[2].byte = 0;
			frmpage(NeedleFrame, NeedleFrameUpdate, 1);
			break;

		case Lamp:
			Gennal_Use[2].byte = 0;
			frmpage(WarnLedFrame, WarnLedFrameUpdate, 1);
			break;

		case Buzzer:
			Gennal_Use[2].byte = 0;
			frmpage(SpeakerFrame, SpeakerFrameUpdate, 1);
			break;

		case LCD:
			Gennal_Use[2].byte = 0;
			frmpage(LCDTestFrame, LCDTestFrameUpdate, 1);
			break;

		case Sensor:
			Gennal_Use[2].byte = 0;
			frmpage(SensorTestFrame, SensorTestFrameUpdate, 1);
			break;


		default:
            key_up = 0;
            key_set = 0;
            key_AV = 0;
            LCD_Clear();
            break;
    }
    
	if((Dispeffect == frmIDPre)&&(Dispeffect != frmID))
	{
		if(LED_LocationLight)
		{
			PWM_Update_Duty_Cycle(LED_CHANNEL,e_backlight_led_night, PWM_PERIOD);//980hz
			PWM_Update_Duty_Cycle(BG_CHANNEL,e_backlight_pannel_night, PWM_PERIOD);//980hz
			PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd_night, PWM_PERIOD);//980hz
		}
		else
		{
			PWM_Update_Duty_Cycle(LED_CHANNEL,e_backlight_led, PWM_PERIOD);//980hz
			PWM_Update_Duty_Cycle(BG_CHANNEL,e_backlight_pannel, PWM_PERIOD);//980hz
			PWM_Update_Duty_Cycle(LCD_CHANNEL,e_backlight_lcd, PWM_PERIOD);//980hz
		}
	}
    frmIDPre = frmID;
	
    if(FaultInfo != frmID)
	{
		faultcodeupdate = 0x00;
	}
	
    if (key_duble)
	{
        key_duble = 0;
    }
    if(learnstatus==0)
	{
		if((key_jingxi||key_kazhi)&&(frmID != Main)&&(frmID!=LCD)&&(frmID!=Needle)&&(frmID!=Lamp)&&(frmID!=Buzzer))//自动翻自动返回到主界面
		{
			if(old_backtomain!=1)
			{
				old_backtomain = 1;
				f_index = 0;
				frmID= Main;
				Gennal_Use[2].byte = 0;
				mainview_update = 1;
				LCD_test = 0;       
				Needle_Test = 0;
				LED_test = 0;
				ready_not_shown = 1;
			}
			else;
		}
	}
	else old_backtomain = 0;

	if((0!= frmID)&&(key_jingxi||key_kazhi)&&(TPMS != frmID))
	{
		old_backtomain = 1;
		f_index = 0;
		frmID= Main;
		Gennal_Use[2].byte = 0;
		mainview_update = 1;
		LCD_test = 0;       
		Needle_Test = 0;
		LED_test = 0;
		ready_not_shown = 1;
	}
	
	if(TPMS != frmID)
	{
		backvalue = 0x00;
	}
	
}






