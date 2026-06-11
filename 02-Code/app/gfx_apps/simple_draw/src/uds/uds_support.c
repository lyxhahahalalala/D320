/**************************************************************************
  \file          uds-support.c
  \author     
  \mail          
  \version       0.01
  \date          2020-03-26
  \description   uds service spport code, include read/write data,
  io control and routine control
  *******************************************************************************/

/*******************************************************************************
  Include Files
  *******************************************************************************/
//#include <absacc.h>
#include "uds_support.h"

/*******************************************************************************
  Function  declaration
  *******************************************************************************/
static uint8_t rtctrl_nothing (void);
static uint8_t rtctrl_check_progrm_precond (void);
static uint8_t rtctrl_write_fl (void);
static void ioctrl_init_backlight (void);
static void ioctrl_stop_backlight (void);
static void ioctrl_init_buzzer (void);
static void ioctrl_stop_buzzer (void);
static void ioctrl_init_gages (void);
static void ioctrl_stop_gages (void);
static void ioctrl_init_display (void);
static void ioctrl_stop_display (void);
static void ioctrl_init_indicator (void);
static void ioctrl_stop_indicator (void);
/*******************************************************************************
  Global Varaibles
  *******************************************************************************/
extern uint8_t uds_session;
uint8_t ASC_boot_ver[10] = {'v', 'e', 'r', '-', '0', '.', '0', '1', 0, 0};
uint8_t ASC_sys_supplier_id[5] = {'1', '2', '3', '4', '5'};
uint8_t ASC_soft_ver[10] = {'v', 'e', 'r', '-', '0', '.', '0', '1', 0, 0};
uint8_t ASC_sys_name[10] = {'I', 'C', 'U', '-', 'm', '1', '2', 'e', 0, 0};

uint8_t eol_hmpulse[4];
uint8_t eol_biaotouxishu[2];
uint8_t eol_biaosuxiangsubifenzi;
uint8_t eol_biaosuxiangsubifenmu;
uint8_t eol_language;
uint8_t eol_atmtype;
uint8_t eol_ebs;
uint8_t eol_tpmsyesno;
uint8_t eol_ecastype;
uint8_t eol_huansuqi;
uint8_t eol_mcpbaojing;
uint8_t eol_esc;
uint8_t eol_ldwstype;
uint8_t eol_chengkecesaftybelt;
uint8_t eol_airsensortype;
uint8_t eol_vehicletype;
uint8_t eol_aebstype;
uint8_t eol_paifang;
uint8_t eol_actype;
uint8_t eol_vecutype;
uint8_t eol_drivetype;
uint8_t eol_overspeedvalue;
uint16_t B_value;
float   HM_Value;
uint8_t eol_houchuli;
uint8_t eol_fadongjiorkaji;
uint8_t eol_emsjingjizhuansu[4];
uint8_t eol_emsbaojingzhaunsu[2];
uint8_t eol_vecu;
uint8_t eol_baoyanglicheng[2];
uint8_t eol_baoyangzhouqi;
uint8_t eol_biansuxiangxinghaojisubi[2];

uint8_t eol_qiyabaojingzhi[4];
uint8_t eol_bdsconfig;
uint8_t eol_dfmsconfig;
uint8_t eol_pepsconfig;
uint8_t eol_battcoolmd = 0x01;

uint8_t eol_Tbox;
uint8_t eol_endeol;
uint8_t eol_shuiwei;
uint8_t eol_XianSuFunction; 
uint8_t eol_yingjizhuanxiang;
uint8_t eol_xingshijiluyi;
uint8_t eol_epbtype;

uint8_t eol_ranqipingconfig;
uint8_t eol_ranqixielouconfig;
uint8_t eol_ciomconfig;
uint8_t eol_dianyezhuanxiangconfig;
uint8_t eol_jiashishitype;
uint8_t eol_d600ganzaoqiconfig;
uint8_t eol_d600epbconfig;


uint8_t eol_icutype;
uint8_t eol_speed_enlarge_factor;


uint16_t atmModelSpdRatio;
uint16_t QiyaLowLmt;
uint16_t QiyaHighLmt;


const uint8_t ASC_ecu_part_num[15] = {'q','q','q','q','q','q','q','q','q','q','q','q','q','q','q'};
const uint8_t BCD_manufacture_date[3]  = {'q','q','q'};

uint8_t progrm_checkSuccFlg = FALSE;
uint8_t F110_Integra_SVN[23] = {'0','0','0','.','0','0','0','.','0','0','0','.','0','0','0','.','0','0','0','.','0','0','0'}; //主机厂ECU系统集成版本号(N/N/N)
uint8_t F111_App_SVN[23]     = {'0','0','0','.','0','0','0','.','0','0','0','.','0','0','0','.','0','0','0','.','0','0','0'}; //主机厂ECU应用软件版本号(N/N/N)
uint8_t F112_Bsw_SVN[23]     = {'0','0','0','.','0','0','0','.','0','0','0','.','0','0','0','.','0','0','0','.','0','0','0'}; //主机厂ECU基础软件版本号(N/N/N)

uint8_t F187_Spare_PN[13] = {'0','0','0','0','0','0','0','0','0','0','0','0','0'}; //主机厂备件号(N/N/N)
uint8_t F188_Ecu_SN[13]   = {'3','8','0','1','0','4','0','-','C','3','1','0','9'}; //主机厂软件零件号(R/N/R)
uint8_t F189_Ecu_SVN[13]  = {'V','0','2','1','4','0','0','2','6','0','3','2','0'}; //主机厂软件版本号(R/N/R)

uint8_t F1A0_RT_VN[5]     = {'0','0','0','0','0'}; //RT版本号(R/N/R)
uint8_t F1A1_App_VN[5]    = {'0','1','.','0','8'}; //应用版本号(R/N/R)
uint8_t F1A2_HW_Ver[5]    = {'0','0','.','0','4'}; //主机厂硬件版本（产品硬件总成版本）(R/N/R)
uint8_t F1A3_CalibData_VN[5] = {'0','0','.','0','0'}; //标定数据版本号(R/N/R)
uint8_t F1A4_SysSupplier_ID[5] = {'0','D','2','4','5'}; //供应商代码(WR/N/R)
uint8_t F1A5_SysSupEol_MD[6]   = {0x20, 0x20, 0x09, 0x19, 0x00, 0x00}; //供应商下线制造日期(WR/N/R)
uint8_t F1A6_SysSupEol_MSN[11] = {0x20, 0x20, 0x09, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //供应商下线制造流水号(WR/N/R)

uint8_t F190_VIN[17] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'}; //车辆VIN码(N/W/WR)
uint8_t F191_VehMan_Ecu_HN[13] = {'3','8','0','1','0','4','0','-','C','5','1','0','4'}; //主机厂零件号(R/N/R)
uint8_t F198_RepairShp_TSN[20] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'}; //维修站代码或诊断仪代码(N/N/WR)
uint8_t F19B_Calibration_Date[4] = {0x00, 0x00, 0x00, 0x00}; //最近一次标定日期(N/WR/R)
uint8_t F19D_Ecu_Ins_Date[4] = {0x00, 0x00, 0x00, 0x00}; //ECU装车日期(N/WR/R)
uint8_t F15A_Audit_Trail[17] = {0x00, 0x00, 0x00,'0','0','0','0','0','0','0','0','0','0','0','0','0','0'}; //跟踪信息（生产将fbl、APP整体写入，生产不关心该信息。服务站会关心）(N/N/R)
uint8_t F123_Fbl_VN[5] = { 0x04, 0x01, 0x23, 0x02, 0x13}; //bootloader版本号(R/N/R)

uint8_t F010_HW_Ver[8] = {'0','0','.','0','4','.','0','0'}; //硬件版本
uint8_t F011_SW_Ver[8] = {'0','2','.','1','4','.','0','1'}; //软件版本



// we add read write data and it's identify here to support service 22
const uds_rwdata_t rwdata_list[RWDATA_CNT] =
{
/* 0*/{0x0140, &eol_hmpulse[0],                  4, 0x1200, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<里程表传感器每转脉冲数>
/* 1*/{0x0141, &eol_biaotouxishu[0],             2, 0x1240, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<里程表表头系数>
/* 2*/{0x0142, &eol_biaosuxiangsubifenzi,        1, 0x1280, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<变速箱里程表速比分子>
/* 3*/{0x0143, &eol_biaosuxiangsubifenmu,        1, 0x12C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<变速箱里程表速比分母>
/* 4*/{0x0144, &eol_language,                    1, 0x1300, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<语言>
/* 5*/{0x0145, &eol_houchuli,                    1, 0x1340, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<后处理方案>
/* 6*/{0x0146, &eol_fadongjiorkaji,              1, 0x1380, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<发动机类型>
/* 7*/{0x0147, &eol_emsjingjizhuansu[0],         4, 0x13C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<发动机经济转速区>
/* 8*/{0x0148, &eol_emsbaojingzhaunsu[0],        2, 0x1400, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<发动机报警转速区>
/* 9*/{0x0149, &eol_atmtype,                     1, 0x1440, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<变速箱类型>
/*10*/{0x014A, &eol_ebs,                         1, 0x1480, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<ABS或EBS及厂家>
/*11*/{0x014B, &eol_tpmsyesno,                   1, 0x14C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<TPMS系统有无>
/*12*/{0x014C, &eol_ecastype,                    1, 0x1500, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<ECAS类型>
/*13*/{0x014D, &eol_huansuqi,                    1, 0x1540, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<缓速器类型>
/*14*/{0x014E, &eol_mcpbaojing,                  1, 0x1580, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<盘式制动器磨损报警>
/*15*/{0x014F, &eol_esc,                         1, 0x15C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<ESC及厂家>
/*16*/{0x0150, &eol_ldwstype,                    1, 0x1600, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<LDWS车道偏离预警系统>
/*17*/{0x0151, &eol_chengkecesaftybelt,          1, 0x1640, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<乘客侧安全带未系报警功能有无>
/*18*/{0x0152, &eol_airsensortype,               1, 0x1680, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<气压传感器数量>
/*19*/{0x0153, &eol_vehicletype,                 1, 0x16C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<车辆类别>
/*20*/{0x0154, &eol_aebstype,                    1, 0x1700, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<AEBS自动紧急制动系统>
/*21*/{0x0155, &eol_paifang,                     1, 0x1740, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<排放及升级技术>
/*22*/{0x0156, &eol_actype,                      1, 0x1780, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<空调调节类型>
/*23*/{0x0157, &eol_vecu,                        1, 0x17C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<整车VECU匹配>
/*24*/{0x015A, &eol_baoyanglicheng[0],           2, 0x1800, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<整车保养里程>
/*25*/{0x015B, &eol_baoyangzhouqi,               1, 0x1840, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<整车保养周期>
/*26*/{0x015C, &eol_biansuxiangxinghaojisubi[0], 2, 0x1880, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<变速箱型号及速比>
/*27*/{0x015D, &eol_drivetype,                   1, 0x18C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<驱动型式及位置>
/*28*/{0x015E, &eol_overspeedvalue,              1, 0x1900, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<超速报警值>
/*29*/{0x015F, &eol_qiyabaojingzhi[0],           4, 0x1940, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<气压报警值>
/*30*/{0x0160, &eol_bdsconfig,                   1, 0x1980, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<BSD 配置>
/*31*/{0x0161, &eol_dfmsconfig,                  1, 0x19C0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<疲劳驾驶配置>
/*32*/{0x0162, &eol_pepsconfig,                  1, 0x1A00, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<PEPS 配置----电子锁-一键启动系统配置>
/*33*/{0x0164, &eol_battcoolmd,                  1, 0x1A40, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<动力电池冷却方式>
/*34*/{0x0191, &eol_Tbox,                        1, 0x1B40, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<TBOX配置>
/*35*/{0x0112, &eol_shuiwei,                     1, 0x1BC0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<水位传感器配置>
/*36*/{0x0192, &eol_XianSuFunction,              1, 0x1B80, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<限速功能>
/*37*/{0x0193, &eol_yingjizhuanxiang,            1, 0x1C00, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<应急转向配置>
/*38*/{0x0194, &eol_xingshijiluyi,               1, 0x1C40, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<行驶记录仪>
/*39*/{0x0195, &eol_epbtype,                     1, 0x1C80, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<EPB 配置>

/*40*/{0x0320, &eol_icutype,                     1, 0x1D00, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<仪表类型>
/*41*/{0x0321, &eol_speed_enlarge_factor,        1, 0x1D40, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<车速放大系数>

/*42*/{0x0163, &eol_ranqipingconfig,             1, 0x1E00, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<燃气瓶数量>
/*43*/{0x0165, &eol_ranqixielouconfig,           1, 0x1E40, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<燃气泄漏传感器配置>
/*44*/{0x0166, &eol_ciomconfig,                  1, 0x1E80, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<CIOM 配置>
/*45*/{0x0169, &eol_dianyezhuanxiangconfig,      1, 0x1EC0, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<电液转向机配置>
/*46*/{0x0196, &eol_jiashishitype,               1, 0x1F00, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<驾驶室类型>
/*47*/{0x01A1, &eol_d600ganzaoqiconfig,          1, 0x1F40, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<D600 电控干燥器配置>
/*48*/{0x01A2, &eol_d600epbconfig,               1, 0x1F80, UDS_RWDATA_RDWR, UDS_RWDATA_DFLASH}, //<D600 EPB 配置>
/*49*/

};

const uds_rwdata_t Logicdata_list[LOGICDATA_CNT] =
{
/* 0*/{0xF110, &F110_Integra_SVN[0],            23, 0x3000, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂ECU系统集成版本号(N/N/N)
/* 1*/{0xF111, &F111_App_SVN[0],                23, 0x3040, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂ECU应用软件版本号(N/N/N)
/* 2*/{0xF112, &F112_Bsw_SVN[0],                23, 0x3080, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂ECU基础软件版本号(N/N/N)
/* 3*/{0xF187, &F187_Spare_PN[0],               13, 0x30C0, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂备件号(N/N/N)
/* 4*/{0xF188, &F188_Ecu_SN[0],                 13, 0x3100, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂软件号(R/N/R)
/* 5*/{0xF189, &F189_Ecu_SVN[0],                13, 0x3140, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂软件版本号(R/N/R)
/* 6*/{0xF1A0, &F1A0_RT_VN[0],                  5,  0x3180, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //RT版本号(R/N/R)
/* 7*/{0xF1A1, &F1A1_App_VN[0],                 5,  0x31C0, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //应用版本号(R/N/R)
/* 8*/{0xF1A2, &F1A2_HW_Ver[0],                 5,  0x3200, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂硬件版本（产品硬件总成版本）(R/N/R)
/* 9*/{0xF1A3, &F1A3_CalibData_VN[0],           5,  0x3240, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //标定数据版本号(R/N/R)
/*10*/{0xF1A4, &F1A4_SysSupplier_ID[0],         5,  0x3280, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //供应商代码(WR/N/R)
/*11*/{0xF1A5, &F1A5_SysSupEol_MD[0],           6,  0x32C0, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //供应商下线制造日期(WR/N/R)
/*12*/{0xF1A6, &F1A6_SysSupEol_MSN[0],          11, 0x3300, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //供应商下线制造流水号(WR/N/R)
/*13*/{0xF190, &F190_VIN[0],                    17, 0x3340, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //车辆VIN码(N/W/WR)
/*14*/{0xF191, &F191_VehMan_Ecu_HN[0],          13, 0x3380, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //主机厂零件号(R/N/R)
/*15*/{0xF198, &F198_RepairShp_TSN[0],          20, 0x33C0, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //维修站代码或诊断仪代码(N/N/WR)
/*16*/{0xF19B, &F19B_Calibration_Date[0],       4,  0x3400, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //最近一次标定日期(N/WR/R)
/*17*/{0xF19D, &F19D_Ecu_Ins_Date[0],           4,  0x3440, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH}, //ECU装车日期(N/WR/R)
/*18*/{0xF15A, &F15A_Audit_Trail[0],            17, 0x3480, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //跟踪信息（生产将fbl、APP整体写入，生产不关心该信息。服务站会关心）(N/N/R)
/*19*/{0xF123, &F123_Fbl_VN[0],                 5,  0x34C0, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //bootloader版本号(R/N/R)

/*20*/{0xF010, &F010_HW_Ver[0],                 8,  0x3500, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}, //硬件版本读取
/*21*/{0xF011, &F011_SW_Ver[0],                 8,  0x3540, UDS_RWDATA_RDONLY, UDS_RWDATA_DFLASH}  //软件版本读取


};

uint8_t backlight_level[2];
uint8_t buzzer[2];
uint8_t gages[2];
uint8_t segment_disp[2];
uint8_t indicator[6];

uds_ioctrl_t ioctrl_list[IOCTRL_CNT] = 
{
	{0xF092, backlight_level, 2, 0, 0, 0, &ioctrl_init_backlight, &ioctrl_stop_backlight},
	{0xF020, buzzer,          2, 0, 0, 0, &ioctrl_init_buzzer,    &ioctrl_stop_buzzer},
	{0xF021, gages,           2, 0, 0, 0, &ioctrl_init_gages,     &ioctrl_stop_gages},
	{0xF022, segment_disp,    2, 0, 0, 0, &ioctrl_init_display,   &ioctrl_stop_display},
	{0xF024, indicator,       6, 0, 0, 0, &ioctrl_init_indicator, &ioctrl_stop_indicator}
};

uds_rtctrl_t rtctrl_list[RTCTRL_CNT] =
{
	{0x1008, UDS_RT_ST_IDLE, &rtctrl_write_fl, &rtctrl_nothing, &rtctrl_nothing},
	{0x0202, UDS_RT_ST_IDLE, &rtctrl_check_progrm_precond, &rtctrl_nothing, &rtctrl_nothing}
};


/*******************************************************************************
  Function  Definition
  *******************************************************************************/
/**
 * eol_data_read - read eol data on ign on
 *
 * @void :
 *
 * returns:
 *     0 - ok
 */
void eol_data_read(void)
{
    Diagnostic_EEProm_Read(rwdata_list[BSXFM].ee_address, 1,&eol_biaosuxiangsubifenmu);

    if((0x00 == eol_biaosuxiangsubifenmu)||(0xFF == eol_biaosuxiangsubifenmu))
	{
		eol_overspeedvalue = 60;
		Diagnostic_EEProm_Write(rwdata_list[OVERSPEEDVALUE].ee_address, 1, &eol_overspeedvalue);
		
		eol_hmpulse[0] = 0x00;
		eol_hmpulse[1] = 0x08;
		eol_hmpulse[2] = 0x00;
		eol_hmpulse[3] = 0x00;
		Diagnostic_EEProm_Write(rwdata_list[HMPULSE].ee_address, 4,eol_hmpulse);
		
		eol_biaotouxishu[0] = 0x04;
		eol_biaotouxishu[1] = 0xBE;
		Diagnostic_EEProm_Write(rwdata_list[BIAOTOUXISHU].ee_address, 2,eol_biaotouxishu);
		
		eol_biaosuxiangsubifenzi = 0x01;
		Diagnostic_EEProm_Write(rwdata_list[BSXFENZI].ee_address, 1,&eol_biaosuxiangsubifenzi);
		
		eol_biaosuxiangsubifenmu = 0x01;
		Diagnostic_EEProm_Write(rwdata_list[BSXFM].ee_address, 1,&eol_biaosuxiangsubifenmu);
		
		eol_language = 0; //0-ch 1-english
		Diagnostic_EEProm_Write(rwdata_list[LANGUAGE].ee_address, 1,&eol_language);
		
		eol_ebs = 6U;
		Diagnostic_EEProm_Write(rwdata_list[ABSEBS].ee_address, 1,&eol_ebs);
		
		eol_tpmsyesno = 1U;
		Diagnostic_EEProm_Write(rwdata_list[TPMSTYPE].ee_address, 1,&eol_tpmsyesno);
		
		eol_ecastype = 3U;	
		Diagnostic_EEProm_Write(rwdata_list[ECASTYPE].ee_address, 1,&eol_ecastype);

		eol_mcpbaojing = 3U;
		Diagnostic_EEProm_Write(rwdata_list[MCPWARN].ee_address, 1,&eol_mcpbaojing);
		
		eol_ldwstype = 0U;//0-无 1,2,3有
		Diagnostic_EEProm_Write(rwdata_list[LDWSSYS].ee_address, 1,&eol_ldwstype);
		
		eol_esc = 2U;////0-无 1,2有
		Diagnostic_EEProm_Write(rwdata_list[ESCTYPE].ee_address, 1,&eol_esc);
		
		eol_chengkecesaftybelt = 0U;
		Diagnostic_EEProm_Write(rwdata_list[PASSENGERSEATBELT].ee_address, 1,&eol_chengkecesaftybelt);

		eol_airsensortype = 2U;	
		Diagnostic_EEProm_Write(rwdata_list[SENSORNUM].ee_address, 1,&eol_airsensortype);
		
		eol_vehicletype=2U;	
		Diagnostic_EEProm_Write(rwdata_list[VEHICLETYPE].ee_address, 1,&eol_vehicletype);
		
		eol_aebstype = 0U;
		Diagnostic_EEProm_Write(rwdata_list[AEBSSYS].ee_address, 1,&eol_aebstype);
		
		eol_actype = 3U;
		Diagnostic_EEProm_Write(rwdata_list[ACTYPE].ee_address, 1,&eol_actype);
		
		eol_drivetype = 1U;
		Diagnostic_EEProm_Write(rwdata_list[DRIVETYPE].ee_address, 1,&eol_drivetype);
		
		eol_houchuli = 19U;
		Diagnostic_EEProm_Write(rwdata_list[HOUCHULI].ee_address, 1,&eol_houchuli);
	
		eol_fadongjiorkaji = 38U;
		Diagnostic_EEProm_Write(rwdata_list[FADONGJIORKAJI].ee_address, 1,&eol_fadongjiorkaji);
		
		eol_emsjingjizhuansu[0] = 0x00;
		eol_emsjingjizhuansu[1] = 0x00;
		eol_emsjingjizhuansu[2] = 0x00;
		eol_emsjingjizhuansu[3] = 0x00;
		Diagnostic_EEProm_Write(rwdata_list[EMSECORPM].ee_address, 4,eol_emsjingjizhuansu);				 
		
		eol_emsbaojingzhaunsu[0] = 0x00;
		eol_emsbaojingzhaunsu[1] = 0x00;
		Diagnostic_EEProm_Write(rwdata_list[EMSOVERRPM].ee_address, 2,eol_emsbaojingzhaunsu);
		
		eol_atmtype= 34U;
		Diagnostic_EEProm_Write(rwdata_list[ATMTYPE].ee_address, 1,&eol_atmtype);				 				 
		
		eol_paifang= 9U;
		Diagnostic_EEProm_Write(rwdata_list[PAIFANG].ee_address, 1,&eol_paifang);
		
		eol_vecu= 4U;
		Diagnostic_EEProm_Write(rwdata_list[VECU].ee_address, 1,&eol_vecu);				 
		
		eol_baoyanglicheng[0]= 0x27;
		eol_baoyanglicheng[1]= 0x10;
		Diagnostic_EEProm_Write(rwdata_list[BAOYANGLICHENG].ee_address, 2,eol_baoyanglicheng);
		
		eol_baoyangzhouqi = 12U;
		Diagnostic_EEProm_Write(rwdata_list[BAOYANGZHOUQI].ee_address, 1,&eol_baoyangzhouqi);				 
		
		eol_biansuxiangxinghaojisubi[0] = 0x00;
		eol_biansuxiangxinghaojisubi[1] = 0xD4;
		Diagnostic_EEProm_Write(rwdata_list[BIANSUXIANGXINGHAoSUBI].ee_address, 2,eol_biansuxiangxinghaojisubi);
		
		eol_huansuqi = 2U;
		Diagnostic_EEProm_Write(rwdata_list[HUANSUQI].ee_address, 1,&eol_huansuqi);	
		
		eol_qiyabaojingzhi[0] = 0x02; eol_qiyabaojingzhi[1] = 0x17;
		eol_qiyabaojingzhi[2] = 0x04; eol_qiyabaojingzhi[3] = 0xB0;
		Diagnostic_EEProm_Write(rwdata_list[QIYABAOJINGZHI].ee_address, 4,eol_qiyabaojingzhi);
		
		eol_bdsconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[BSDCONFIG].ee_address, 1,&eol_bdsconfig);
		
		eol_dfmsconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[PILAOJIASHIALARMCONFIG].ee_address, 1,&eol_dfmsconfig);
		
		eol_pepsconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[PEPSCONFIG].ee_address, 1,&eol_pepsconfig);
		
		eol_battcoolmd = 1U;
		Diagnostic_EEProm_Write(rwdata_list[BATTCOOLMODE].ee_address, 1,&eol_battcoolmd);
		
		eol_Tbox = 0U;
		Diagnostic_EEProm_Write(rwdata_list[TBOX].ee_address, 1,&eol_Tbox);
		
		eol_shuiwei = 0U;
		Diagnostic_EEProm_Write(rwdata_list[SHUIWEI].ee_address, 1,&eol_shuiwei);
		
		eol_XianSuFunction = 50U;	
		Diagnostic_EEProm_Write(rwdata_list[XIANSUFUN].ee_address, 1,&eol_XianSuFunction);
		
		eol_yingjizhuanxiang = 0U;
		Diagnostic_EEProm_Write(rwdata_list[YINGJIZHUANXIANG].ee_address, 1,&eol_yingjizhuanxiang);
		
		eol_xingshijiluyi = 0U;
		Diagnostic_EEProm_Write(rwdata_list[XINGSHIJILUYI].ee_address, 1,&eol_xingshijiluyi);
		
		eol_epbtype = 0U;
		Diagnostic_EEProm_Write(rwdata_list[EPBTYPE].ee_address, 1,&eol_epbtype);
		
		
		// 0: C5104  BCAN:250k, PCAN 250k;
		// 1: C3104  BCAN:250k, PCAN 500k;
		// 2: C3106  BCAN:500k, PCAN 500k;
		eol_icutype = 1U;
		Diagnostic_EEProm_Write(rwdata_list[ICUTYPE].ee_address, 1,&eol_icutype);
		
		eol_speed_enlarge_factor = 100U;
		Diagnostic_EEProm_Write(rwdata_list[SPEED_ENLARGE_FACTOR].ee_address, 1,&eol_speed_enlarge_factor);
		
		eol_ranqipingconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[RANQIPINGCONFIG].ee_address, 1,&eol_ranqipingconfig);
		
		eol_ranqixielouconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[RANQIXIELOUCONFIG].ee_address, 1,&eol_ranqixielouconfig);
		
		eol_ciomconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[CIOMCONFIG].ee_address, 1,&eol_ciomconfig);
		
		eol_dianyezhuanxiangconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[DIANYEZHUANXIANGCONFIG].ee_address, 1,&eol_dianyezhuanxiangconfig);
		
		eol_jiashishitype = 0;
		Diagnostic_EEProm_Write(rwdata_list[JIASHISHITYPE].ee_address, 1,&eol_jiashishitype);
		
		eol_d600ganzaoqiconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[D600GANZAOQICONFIG].ee_address, 1,&eol_d600ganzaoqiconfig);
		
		eol_d600epbconfig = 0;
		Diagnostic_EEProm_Write(rwdata_list[D600EPBCONFIG].ee_address, 1,&eol_d600epbconfig);
		
		
	}
	
    Diagnostic_EEProm_Read(rwdata_list[OVERSPEEDVALUE].ee_address, 1,&eol_overspeedvalue);
    if(eol_overspeedvalue == 0)eol_overspeedvalue = 100;
	
    Diagnostic_EEProm_Read(rwdata_list[HMPULSE].ee_address, 4,eol_hmpulse);
	float temp = (eol_hmpulse[2]<<8) + eol_hmpulse[3];
	while(temp >= 1)
	{
		temp = temp / 10;
	}
	HM_Value  = (eol_hmpulse[0]<<8) + eol_hmpulse[1] + temp;
	
    Diagnostic_EEProm_Read(rwdata_list[BIAOTOUXISHU].ee_address, 2,eol_biaotouxishu);
    B_value = eol_biaotouxishu[1]+ eol_biaotouxishu[0]*256;		
    if(B_value == 0 || B_value == 0xffff)B_value = 1860;
    
    Diagnostic_EEProm_Read(rwdata_list[BSXFENZI].ee_address, 1,&eol_biaosuxiangsubifenzi);
    if(eol_biaosuxiangsubifenzi == 0)eol_biaosuxiangsubifenzi = 100;
	
    Diagnostic_EEProm_Read(rwdata_list[BSXFM].ee_address, 1,&eol_biaosuxiangsubifenmu);
    if(eol_biaosuxiangsubifenmu == 0)eol_biaosuxiangsubifenmu = 100;
	
    Diagnostic_EEProm_Read(rwdata_list[LANGUAGE].ee_address, 1,&eol_language);
	if(eol_language > 1) eol_language = 0;
	
    Diagnostic_EEProm_Read(rwdata_list[ABSEBS].ee_address, 1,&eol_ebs);
    Diagnostic_EEProm_Read(rwdata_list[TPMSTYPE].ee_address, 1,&eol_tpmsyesno);
    Diagnostic_EEProm_Read(rwdata_list[ECASTYPE].ee_address, 1,&eol_ecastype);
    Diagnostic_EEProm_Read(rwdata_list[MCPWARN].ee_address, 1,&eol_mcpbaojing);
    Diagnostic_EEProm_Read(rwdata_list[LDWSSYS].ee_address, 1,&eol_ldwstype);
    Diagnostic_EEProm_Read(rwdata_list[ESCTYPE].ee_address, 1,&eol_esc);
    Diagnostic_EEProm_Read(rwdata_list[PASSENGERSEATBELT].ee_address, 1,&eol_chengkecesaftybelt);
    Diagnostic_EEProm_Read(rwdata_list[SENSORNUM].ee_address, 1,&eol_airsensortype);
    Diagnostic_EEProm_Read(rwdata_list[VEHICLETYPE].ee_address, 1,&eol_vehicletype);
    Diagnostic_EEProm_Read(rwdata_list[AEBSSYS].ee_address, 1,&eol_aebstype);
    Diagnostic_EEProm_Read(rwdata_list[ACTYPE].ee_address, 1,&eol_actype);
    Diagnostic_EEProm_Read(rwdata_list[DRIVETYPE].ee_address, 1,&eol_drivetype);

    Diagnostic_EEProm_Read(rwdata_list[HOUCHULI].ee_address, 1,&eol_houchuli);				 
    Diagnostic_EEProm_Read(rwdata_list[FADONGJIORKAJI].ee_address, 1,&eol_fadongjiorkaji);
    Diagnostic_EEProm_Read(rwdata_list[EMSECORPM].ee_address, 4,eol_emsjingjizhuansu);				 
    Diagnostic_EEProm_Read(rwdata_list[EMSOVERRPM].ee_address, 2,eol_emsbaojingzhaunsu);
    Diagnostic_EEProm_Read(rwdata_list[ATMTYPE].ee_address, 1,&eol_atmtype);				 				 
    Diagnostic_EEProm_Read(rwdata_list[PAIFANG].ee_address, 1,&eol_paifang);
    Diagnostic_EEProm_Read(rwdata_list[VECU].ee_address, 1,&eol_vecu);				 
    Diagnostic_EEProm_Read(rwdata_list[BAOYANGLICHENG].ee_address, 2,eol_baoyanglicheng);
    Diagnostic_EEProm_Read(rwdata_list[BAOYANGZHOUQI].ee_address, 1,&eol_baoyangzhouqi);				 
    Diagnostic_EEProm_Read(rwdata_list[BIANSUXIANGXINGHAoSUBI].ee_address, 2,eol_biansuxiangxinghaojisubi);
	atmModelSpdRatio = (eol_biansuxiangxinghaojisubi[0] << 8U) + eol_biansuxiangxinghaojisubi[1];
	
    Diagnostic_EEProm_Read(rwdata_list[HUANSUQI].ee_address, 1,&eol_huansuqi);	
	
	Diagnostic_EEProm_Read(rwdata_list[QIYABAOJINGZHI].ee_address, 4,eol_qiyabaojingzhi);
	QiyaLowLmt  =  (eol_qiyabaojingzhi[0]<<8) + eol_qiyabaojingzhi[1];
	QiyaHighLmt =  (eol_qiyabaojingzhi[2]<<8) + eol_qiyabaojingzhi[3];
	if(QiyaLowLmt == 0 || QiyaLowLmt == 0xffff) QiyaLowLmt = 535;
	if(QiyaHighLmt == 0 || QiyaHighLmt == 0xffff) QiyaHighLmt = 1250;
	if(QiyaLowLmt >= QiyaHighLmt)
	{
		QiyaLowLmt = 535;
		QiyaHighLmt = 1250;
	}
	Diagnostic_EEProm_Read(rwdata_list[BSDCONFIG].ee_address, 1,&eol_bdsconfig);
	if(eol_bdsconfig == 0xff) eol_bdsconfig = 0;
	
	Diagnostic_EEProm_Read(rwdata_list[PILAOJIASHIALARMCONFIG].ee_address, 1,&eol_dfmsconfig);
	if(eol_dfmsconfig == 0xff) eol_dfmsconfig = 0;
	
	Diagnostic_EEProm_Read(rwdata_list[PEPSCONFIG].ee_address, 1,&eol_pepsconfig);
	
	Diagnostic_EEProm_Read(rwdata_list[BATTCOOLMODE].ee_address, 1,&eol_battcoolmd);
	if(eol_battcoolmd > 2)eol_battcoolmd = 1;
	
	Diagnostic_EEProm_Read(rwdata_list[TBOX].ee_address, 1,&eol_Tbox);
	Diagnostic_EEProm_Read(rwdata_list[SHUIWEI].ee_address, 1,&eol_shuiwei);
	if(eol_shuiwei == 0xff) eol_shuiwei = 0;
	
	Diagnostic_EEProm_Read(rwdata_list[XIANSUFUN].ee_address, 1,&eol_XianSuFunction);
	Diagnostic_EEProm_Read(rwdata_list[YINGJIZHUANXIANG].ee_address, 1,&eol_yingjizhuanxiang);
	Diagnostic_EEProm_Read(rwdata_list[XINGSHIJILUYI].ee_address, 1,&eol_xingshijiluyi);
	Diagnostic_EEProm_Read(rwdata_list[EPBTYPE].ee_address, 1,&eol_epbtype);
	if(eol_epbtype == 0xff) eol_epbtype = 0;
	
	Diagnostic_EEProm_Read(rwdata_list[ICUTYPE].ee_address, 1,&eol_icutype);
	if(eol_icutype > 15) eol_icutype = 0;
	
	Diagnostic_EEProm_Read(rwdata_list[SPEED_ENLARGE_FACTOR].ee_address, 1,&eol_speed_enlarge_factor);
	
	Diagnostic_EEProm_Read(rwdata_list[RANQIPINGCONFIG].ee_address, 1,&eol_ranqipingconfig);
	Diagnostic_EEProm_Read(rwdata_list[RANQIXIELOUCONFIG].ee_address, 1,&eol_ranqixielouconfig);
	Diagnostic_EEProm_Read(rwdata_list[CIOMCONFIG].ee_address, 1,&eol_ciomconfig);
	Diagnostic_EEProm_Read(rwdata_list[DIANYEZHUANXIANGCONFIG].ee_address, 1,&eol_dianyezhuanxiangconfig);
	
	Diagnostic_EEProm_Read(rwdata_list[JIASHISHITYPE].ee_address, 1,&eol_jiashishitype);
	Diagnostic_EEProm_Read(rwdata_list[D600GANZAOQICONFIG].ee_address, 1,&eol_d600ganzaoqiconfig);
	Diagnostic_EEProm_Read(rwdata_list[D600EPBCONFIG].ee_address, 1,&eol_d600epbconfig);
	
}

/**
 * rtctrl_nothing - a temp Function for routine control
 *
 * @void :
 *
 * returns:
 *     0 - ok
 */
static uint8_t rtctrl_nothing (void)
{
    return 0;
}

static uint8_t rtctrl_check_progrm_precond (void)
{
	uint8_t retval = 0x00u;
	rtctrl_list[0].rtst = UDS_RT_ST_RUNNING;
	if(1)
	{
		progrm_checkSuccFlg = TRUE;
		retval = 0x00u;
	}
	else
	{
		progrm_checkSuccFlg = FALSE;
		retval = 0x01u;
	}
	rtctrl_list[0].rtst = UDS_RT_ST_SUCCESS;
	return retval;
}
/**
 * rtctrl_write_fl- write eol info to eeprom
 *
 * @void :
 *
 * returns:
 *     0 - ok
 */
static uint8_t rtctrl_write_fl (void)
{
    rtctrl_list[0].rtst = UDS_RT_ST_RUNNING;
	/* 
	for(uint8_t i = 0; i < RWDATA_ID_NUM; i++)
	{
		Diagnostic_EEProm_Write(rwdata_list[i].ee_address, 
		                        rwdata_list[i].ee_address,
								Logicdata_list[i].p_data);
	}
	 */
    Diagnostic_EEProm_Write(rwdata_list[OVERSPEEDVALUE].ee_address, 1, &eol_overspeedvalue);
    Diagnostic_EEProm_Write(rwdata_list[HMPULSE].ee_address, 4,eol_hmpulse);
    Diagnostic_EEProm_Write(rwdata_list[BIAOTOUXISHU].ee_address, 2,eol_biaotouxishu);
    Diagnostic_EEProm_Write(rwdata_list[BSXFENZI].ee_address, 1,&eol_biaosuxiangsubifenzi);
    Diagnostic_EEProm_Write(rwdata_list[BSXFM].ee_address, 1,&eol_biaosuxiangsubifenmu);
    Diagnostic_EEProm_Write(rwdata_list[LANGUAGE].ee_address, 1,&eol_language);
    Diagnostic_EEProm_Write(rwdata_list[ABSEBS].ee_address, 1,&eol_ebs);
    Diagnostic_EEProm_Write(rwdata_list[TPMSTYPE].ee_address, 1,&eol_tpmsyesno);
    Diagnostic_EEProm_Write(rwdata_list[ECASTYPE].ee_address, 1,&eol_ecastype);
    Diagnostic_EEProm_Write(rwdata_list[MCPWARN].ee_address, 1,&eol_mcpbaojing);	
    Diagnostic_EEProm_Write(rwdata_list[LDWSSYS].ee_address, 1,&eol_ldwstype);
    Diagnostic_EEProm_Write(rwdata_list[ESCTYPE].ee_address, 1,&eol_esc);
    Diagnostic_EEProm_Write(rwdata_list[PASSENGERSEATBELT].ee_address, 1,&eol_chengkecesaftybelt);
    Diagnostic_EEProm_Write(rwdata_list[SENSORNUM].ee_address, 1,&eol_airsensortype);
    Diagnostic_EEProm_Write(rwdata_list[VEHICLETYPE].ee_address, 1,&eol_vehicletype);
    Diagnostic_EEProm_Write(rwdata_list[AEBSSYS].ee_address, 1,&eol_aebstype);
    Diagnostic_EEProm_Write(rwdata_list[ACTYPE].ee_address, 1,&eol_actype);
    Diagnostic_EEProm_Write(rwdata_list[DRIVETYPE].ee_address, 1,&eol_drivetype);
    Diagnostic_EEProm_Write(rwdata_list[HOUCHULI].ee_address, 1,&eol_houchuli);				 
    Diagnostic_EEProm_Write(rwdata_list[FADONGJIORKAJI].ee_address, 1,&eol_fadongjiorkaji);
    Diagnostic_EEProm_Write(rwdata_list[EMSECORPM].ee_address, 4,eol_emsjingjizhuansu);				 
    Diagnostic_EEProm_Write(rwdata_list[EMSOVERRPM].ee_address, 2,eol_emsbaojingzhaunsu);
    Diagnostic_EEProm_Write(rwdata_list[ATMTYPE].ee_address, 1,&eol_atmtype);				 				 
    Diagnostic_EEProm_Write(rwdata_list[PAIFANG].ee_address, 1,&eol_paifang);
    Diagnostic_EEProm_Write(rwdata_list[VECU].ee_address, 1,&eol_vecu);				 
    Diagnostic_EEProm_Write(rwdata_list[BAOYANGLICHENG].ee_address, 2,eol_baoyanglicheng);
    Diagnostic_EEProm_Write(rwdata_list[BAOYANGZHOUQI].ee_address, 1,&eol_baoyangzhouqi);				 
    Diagnostic_EEProm_Write(rwdata_list[BIANSUXIANGXINGHAoSUBI].ee_address, 2,eol_biansuxiangxinghaojisubi);
    Diagnostic_EEProm_Write(rwdata_list[HUANSUQI].ee_address, 1,&eol_huansuqi);	
	
	Diagnostic_EEProm_Write(rwdata_list[QIYABAOJINGZHI].ee_address, 4,eol_qiyabaojingzhi);
	Diagnostic_EEProm_Write(rwdata_list[BSDCONFIG].ee_address, 1,&eol_bdsconfig);
	Diagnostic_EEProm_Write(rwdata_list[PILAOJIASHIALARMCONFIG].ee_address, 1,&eol_dfmsconfig);
	Diagnostic_EEProm_Write(rwdata_list[PEPSCONFIG].ee_address, 1,&eol_pepsconfig);
	Diagnostic_EEProm_Write(rwdata_list[BATTCOOLMODE].ee_address, 1,&eol_battcoolmd);
	
	Diagnostic_EEProm_Write(rwdata_list[TBOX].ee_address, 1,&eol_Tbox);
	Diagnostic_EEProm_Write(rwdata_list[SHUIWEI].ee_address, 1,&eol_shuiwei);
	Diagnostic_EEProm_Write(rwdata_list[XIANSUFUN].ee_address, 1,&eol_XianSuFunction);
	Diagnostic_EEProm_Write(rwdata_list[YINGJIZHUANXIANG].ee_address, 1,&eol_yingjizhuanxiang);
	Diagnostic_EEProm_Write(rwdata_list[XINGSHIJILUYI].ee_address, 1,&eol_xingshijiluyi);
	Diagnostic_EEProm_Write(rwdata_list[EPBTYPE].ee_address, 1,&eol_epbtype);
	
	Diagnostic_EEProm_Write(rwdata_list[ICUTYPE].ee_address, 1,&eol_icutype);
	Diagnostic_EEProm_Write(rwdata_list[SPEED_ENLARGE_FACTOR].ee_address, 1, &eol_speed_enlarge_factor);
	
	Diagnostic_EEProm_Write(rwdata_list[RANQIPINGCONFIG].ee_address, 1,&eol_ranqipingconfig);
	Diagnostic_EEProm_Write(rwdata_list[RANQIXIELOUCONFIG].ee_address, 1,&eol_ranqixielouconfig);
	Diagnostic_EEProm_Write(rwdata_list[CIOMCONFIG].ee_address, 1,&eol_ciomconfig);
	Diagnostic_EEProm_Write(rwdata_list[DIANYEZHUANXIANGCONFIG].ee_address, 1,&eol_dianyezhuanxiangconfig);
	Diagnostic_EEProm_Write(rwdata_list[JIASHISHITYPE].ee_address, 1,&eol_jiashishitype);
	Diagnostic_EEProm_Write(rwdata_list[D600GANZAOQICONFIG].ee_address, 1,&eol_d600ganzaoqiconfig);
	Diagnostic_EEProm_Write(rwdata_list[D600EPBCONFIG].ee_address, 1,&eol_d600epbconfig);
	
    rtctrl_list[0].rtst = UDS_RT_ST_SUCCESS;
    return 0;
}


/**
 * ioctrl_init_backlight - init the backlight control
 *
 * @void : 
 *
 * returns:
 *     void
 */
static void ioctrl_init_backlight (void)
{
    //mcuctrl_set_backlight (backlight_level[0], backlight_level[1]);
}
static void ioctrl_stop_backlight (void)
{
    //mcuctrl_end_backlight ();
}

/**
 * ioctrl_init_buzzer - init the buzzer control
 *
 * @void : 
 *
 * returns:
 *     void
 */
static void ioctrl_init_buzzer (void)
{
    //mcuctrl_set_buzzer (buzzer[0], buzzer[1]);
}

static void ioctrl_stop_buzzer (void)
{
    //mcuctrl_end_buzzer (buzzer[0], buzzer[1]);
}

/**
 * ioctrl_init_gages - init the gages control
 *
 * @void : 
 *
 * returns:
 *     void
 */
static void ioctrl_init_gages (void)
{
    //mcuctrl_set_gages (gages[0], gages[1]);
}

static void ioctrl_stop_gages (void)
{
    //mcuctrl_end_gages (gages[0], gages[1]);
}

/**
 * ioctrl_init_display - init the segment display control
 *
 * @void : 
 *
 * returns:
 *     void
 */
static void ioctrl_init_display (void)
{
    //mcuctrl_set_display (segment_disp[0], segment_disp[1]);
}

static void ioctrl_stop_display (void)
{
    //mcuctrl_end_display (segment_disp[0], segment_disp[1]);
}

/**
 * ioctrl_init_indicator- init the indicators control
 *
 * @void : 
 *
 * returns:
 *     void
 */
static void ioctrl_init_indicator (void)
{
    //mcuctrl_set_indicator (indicator);
}

static void ioctrl_stop_indicator (void)
{
    //mcuctrl_end_indicator (indicator);
}

/*******************************************************************************
  Function  Definition - extern to uds
  *******************************************************************************/

/**
 * uds_ioctrl_allstop - main handle of io control
 *
 * @void : 
 *
 * returns:
 *     void
 */
void uds_ioctrl_allstop (void)
{
    uint16_t did_n;

    for (did_n = 0; did_n < IOCTRL_NUM; did_n++)
    {
        if (ioctrl_list[did_n].enable == TRUE)
        {
            /* need to mutex with 2F service UDS_IOCTRL_RETURN_TO_ECU */
            ioctrl_list[did_n].enable = FALSE;
            if (ioctrl_list[did_n].stop_ioctrl != NULL)
                ioctrl_list[did_n].stop_ioctrl ();
        }
    }
}


/**
 * uds_load_rwdata - load read / write data from eeprom to ram
 *
 * @void : 
 *
 * returns:
 *     void
 */
void uds_load_rwdata (void)      
{
    //memset (ASC_ecu_part_num, 0x55, (15+3+10+17+10+3));      // read the  rwdata_list which is in eeprom,and then assign to  rwdata_list element
}

/**
 * uds_save_rwdata - save read / write data from eeprom to ram
 *
 * @void : 
 *
 * returns:
 *     void
 */
void uds_save_rwdata (void)
{
    //memset (ASC_ecu_part_num, 0x55, (15+3+10+17+10+3));  // //  then rwdata_list element  assign to  a certern space and write to flash
}
/****************EOF****************/


/*---- End of Module -----------------------------------------------------*/

/*===========================================================================
* File Revision History (bottom to top: first revision to last revision)
*============================================================================
* $Log:$

* Rev:     Userid:       Date:       (Description on following lines: CR #, etc.)
* -------  ------------  -----------
*
*===========================================================================*/


