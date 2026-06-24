
#ifndef _INIT_H_
#define _INIT_H_

#include "write.h"
#include "img_format.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"

#ifdef __cplusplus
extern "C" {
#endif
#define LOC_SPRITE_WIN1             0U
#define LOC_SPRITE_WIN2             1U
#define LOC_SPRITE_WIN3             2U

#define LOC_VOVIAL_UNIT 0

#ifndef NULL
#define NULL	 (void*)(0)
#endif

#define VW_DATA_ST_NONE             0           // 无数据
#define VW_DATA_ST_FAULT            1           // 故障值
#define VW_DATA_ST_INVALID          2           // 无效值
#define VW_DATA_ST_NORMAL           3           // 正常值

#define DECLOCATION 10

typedef struct{
    int32_t x;
    int32_t y;
}Coord_t;


typedef struct{
    uint32_t x;
    uint32_t y;
    uint8_t  wm;    // sprite in which win
    uint8_t  spn;   // sprite number in win
    uint8_t  posZ;
}spriteRes_t;


extern r_drw2d_Device_t loc_Drw2dDev;

extern uint8_t forceupdate;

extern const Img_t Img_LmtSpd10;
extern const Img_t Img_LmtSpd20;
extern const Img_t Img_LmtSpd30;
extern const Img_t Img_LmtSpd40;
extern const Img_t Img_LmtSpd50;
extern const Img_t Img_LmtSpd60;
extern const Img_t Img_LmtSpd70;
extern const Img_t Img_LmtSpd80;
extern const Img_t Img_LmtSpd90;
extern const Img_t Img_LmtSpd100;
extern const Img_t Img_LmtSpd110;
extern const Img_t Img_LmtSpd120;
extern const Img_t Img_LmtSpd130;

extern const Img_t Img_point;
extern const Img_t Img_spdn0;
extern const Img_t Img_spdn1;
extern const Img_t Img_spdn2;
extern const Img_t Img_spdn3;
extern const Img_t Img_spdn4;
extern const Img_t Img_spdn5;
extern const Img_t Img_spdn6;
extern const Img_t Img_spdn7;
extern const Img_t Img_spdn8;
extern const Img_t Img_spdn9;

extern const Img_t Img_bg;
extern const Img_t Img_Line;
extern const Img_t Img_Box;
extern const Img_t Img_Choose;

extern const Img_t Img_upColor;
extern const Img_t Img_up;
extern const Img_t Img_down;
extern const Img_t Img_downColor;

extern const Img_t Img_BattSoc;
extern const Img_t Img_H2_SOC;
extern const Img_t Img_socBar;
extern const Img_t Img_socBarYellow;
extern const Img_t Img_socBarRed;

extern const Img_t Img_motor;
extern const Img_t Img_barRight_rpm;

extern const Img_t Img_ChgSOC_BG;
extern const Img_t Img_SOC_BG;
extern const Img_t Img_soc_2px;
extern const Img_t Img_line_white;
extern const Img_t Img_line_red;
extern const Img_t Img_ChgNormal;
extern const Img_t Img_ChgFlt;

extern const Img_t Img_E;
extern const Img_t Img_P;

extern const Img_t Img_tireError;
extern const Img_t Img_tireOk;
extern const Img_t Img_tireBody;
extern const Img_t Img_tireLearn;
extern const Img_t Img_Axis1;
extern const Img_t Img_Axis2;
extern const Img_t Img_tpmsTruck;

extern const Img_t Img_truck;
extern const Img_t Img_rightOffset;
extern const Img_t Img_leftOffset;
extern const Img_t Img_frontCollision;
extern const Img_t Img_tirePressure;

extern const Img_t Img_AutoLowBeam;
extern const Img_t Img_RearLamp;
extern const Img_t Img_EPS;

void loc_RenderRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);

void show_BG(void);
void disable_BG(void);
void display_Charge_BG(void);

int32_t init(void);
int32_t de_init(void);
void start_draw(void);
void end_draw(void);

void Update_AirPressure(unsigned char value);
void loc_spriteCreate(uint8_t wm, uint8_t spn, unsigned int x, unsigned int y, unsigned int z, const Img_t *img);
void loc_spriteDisable(uint8_t wm, uint8_t spn);
void loc_spriteEnable(uint8_t wm, uint8_t spn);
void loc_spriteDelete(uint8_t wm, uint8_t spn);
void loc_spriteSetBuf(uint8_t wm, uint8_t spn, const Img_t *img);
void loc_spriteMove(uint8_t wm, uint8_t spn, uint32_t x, uint32_t y, uint32_t z);
void loc_ClearRect(int32_t x, int32_t y, int32_t w, int32_t h);
void loc_RenderLines(unsigned int x, unsigned int y, unsigned int x1, unsigned int y1,  unsigned int color);

void loc_Render_FHP_AuxiliaryBraking(uint16_t x,uint16_t y, uint8_t color, char value[]);
void loc_Render_FHP_IntelligentDriving(uint16_t x,uint16_t y, uint8_t color, char value[]);
void loc_Render_FHP_LowBatt(uint16_t x,uint16_t y, uint8_t color, char value[]);
void loc_Render_FHP_Text(uint16_t x,uint16_t y, uint8_t color, char value[]);
void loc_Render_FHP(uint16_t x,uint16_t y, uint8_t color, char value[]);

void loc_Render_BigIcon(uint16_t x,uint16_t y, uint32_t color, char value[]);
void loc_Render_BigFHP(uint16_t x,uint16_t y, uint8_t color, char value[]);
void loc_Render_TPMSMAIN(uint16_t x,uint16_t y, uint8_t color, char value[]);
void loc_Render_Logo(uint16_t x,uint16_t y, uint32_t color, char value[]);
void loc_Render_Tire(uint16_t x,uint16_t y, uint8_t color);


void vw_clearFont31Value(int32_t x, int32_t y, uint8_t charNum);
void display_Logo(void);
void update_choose_location(uint32_t x,uint32_t y);
void update_box_location(uint32_t x,uint32_t y);
void disable_choose_image(void);
void disable_box(void);

void show_ldws_left_lane(void);
void hide_ldws_left_lane(void);
void show_ldws_right_lane(void);
void hide_ldws_right_lane(void);


#if !defined USE_INTERNAL_ROM
void disable_line_image(void)
void display_line_image(void);
void disable_tpms_image(void);
void display_tpms_image(void);
void RenderTruckArea(uint8_t display);
#endif

void loc_RenderImg(unsigned int x, unsigned int y, const Img_t *img);
void loc_RenderImg_BlendMode(unsigned int x, unsigned int y, const Img_t *img, r_drw2d_BlendMode_t BlendMode);
void loc_RenderImgScale(uint16_t x, uint16_t y, float ScaleX, float ScaleY, const Img_t *img);

void loc_RenderQuads(Coord_t *coord, uint32_t color);
void loc_RenderTriangles(Coord_t *coord, uint32_t color);
void loc_RenderBezierCurves(Coord_t *coord, uint8_t vertex_num, uint32_t color);

void loc_speedUpdate(void);


#ifdef __cplusplus
}
#endif

#endif
