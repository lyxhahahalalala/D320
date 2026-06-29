/*
============================================================================
 PROJECT: Can project
============================================================================
                           C O P Y R I G H T
============================================================================
 Copyright (c) 2017 by SHFY. All rights reserved.

============================================================================


============================================================================
 Enviroment:  Devices:     RH850/D1M1 / R7F701404
              Debugger:    GHS Multi 2000
============================================================================
*/

/******************************************************************************
Includes
******************************************************************************/
#include "iodefine.h"
#include "r_typedefs.h"
#include "rscan.h"
#include "r_typedefs.h"
#include "uds_support.h"
#include "r_bsp_stdio_api.h"
#include "stdio.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_CAN_GRAMINIT_ON  (0x8UL)
#define R_CAN_GSLPSTS_ON   (0x4UL)
#define R_CAN_GRSTSTS_ON   (0x1UL)
#define R_CAN_CSLPSTS_ON   (0x4UL)
#define R_CAN_CHLTSTS_ON   (0x2UL)
#define R_CAN_CRSTSTS_ON   (0x1UL)
#define R_CAN_TMTRM_ON     (0x8U)
#define R_CAN_TMTR_ON      (0x1U)
#define R_CAN_AFLDAE_ON    (0x100UL)
#define R_CAN_GSLPR_MASK   (0x4UL)
#define R_CAN_GMDC_MASK    (0x3UL)
#define R_CAN_CSLPR_MASK   (0x4UL)
#define R_CAN_CHMDC_MASK   (0x3UL)

#pragma interrupt INTRCANGRECC(enable=true, callt=true, fpu=true)


#define RSCAN0_BASE          ((uint32_t)0xFFD00000)
#define RSCAN0_GAFLIDj(j)    (*((volatile uint32_t *)(RSCAN0_BASE + 0x0500 +(0x10 * j))))
#define CAN_RX_RULE_NUM      sizeof(CanDrvRxRule)/sizeof(CanRxRuleCfg_t)

typedef struct {
    volatile uint32_t GAFL_ID;
    volatile uint32_t GAFL_MSK;
    volatile uint32_t GAFL_P0;
    volatile uint32_t GAFL_P1;
} CanRxRuleCfg_t;
const CanRxRuleCfg_t CanDrvRxRule[] = \
{   /* ID        ,  MASK       , hand and DLC, FIFO bit2~3 */
	/*CAN1 - BCAN*/
	{0x18DA17F1UL, 0x1FFFFFF0UL, 0x00000000UL, 0x00000002UL}, /*1*/
	{0x00000151UL, 0x1FFFFFF0UL, 0x00000100UL, 0x00000002UL},
	{0x000001F1UL, 0x1FFFFFF0UL, 0x00000200UL, 0x00000002UL},
	{0x00000888UL, 0x1FFFFFFFUL, 0x00000300UL, 0x00000002UL},
	{0x00000610UL, 0x1FFFFFFFUL, 0x00000400UL, 0x00000002UL},
	{0x18FECA00UL, 0x1FFFFF00UL, 0x00000500UL, 0x00000002UL},
	{0x18ECFF00UL, 0x1FFFFF00UL, 0x00000600UL, 0x00000002UL},
	{0x18EBFF00UL, 0x1FFFFF00UL, 0x00000700UL, 0x00000002UL}, /*8*/

	{0x18FF2232UL, 0x1FFFFFFFUL, 0x00000800UL, 0x00000002UL},
	{0x18FF2732UL, 0x1FFFFFFFUL, 0x00000900UL, 0x00000002UL},
	{0x18FED932UL, 0x1FFFFFFFUL, 0x00000A00UL, 0x00000002UL},
	{0x18FDCD32UL, 0x1FFFFFFFUL, 0x00000B00UL, 0x00000002UL},
	{0x18A9E832UL, 0x1FFFFFFFUL, 0x00000C00UL, 0x00000002UL},
	{0x0C0B2A32UL, 0x1FFFFFFFUL, 0x00000D00UL, 0x00000002UL},
	{0x0CFDCC32UL, 0x1FFFFFFFUL, 0x00000E00UL, 0x00000002UL},
	{0x18FFF631UL, 0x1FFFFFFFUL, 0x00000F00UL, 0x00000002UL},
	{0x18FFF731UL, 0x1FFFFFFFUL, 0x00001000UL, 0x00000002UL},
	{0x18F71E31UL, 0x1FFFFFFFUL, 0x00001100UL, 0x00000002UL},
	{0x18FEEA2FUL, 0x1FFFFFFFUL, 0x00001200UL, 0x00000002UL},
	{0x18FE592FUL, 0x1FFFFFFFUL, 0x00001300UL, 0x00000002UL},
	{0x0CFE5A2FUL, 0x1FFFFFFFUL, 0x00001400UL, 0x00000002UL},
	{0x10F007E8UL, 0x1FFFFFFFUL, 0x00001500UL, 0x00000002UL},
	{0x10FE6FE8UL, 0x1FFFFFFFUL, 0x00001600UL, 0x00000002UL},
	{0x18FE5BE8UL, 0x1FFFFFFFUL, 0x00001700UL, 0x00000002UL},
	{0x18FF16E8UL, 0x1FFFFFFFUL, 0x00001800UL, 0x00000002UL},
	{0x18FFE633UL, 0x1FFFFFFFUL, 0x00001900UL, 0x00000002UL},
	{0x18FFE833UL, 0x1FFFFFFFUL, 0x00001A00UL, 0x00000002UL},
	{0x18FF28ECUL, 0x1FFFFFFFUL, 0x00001B00UL, 0x00000002UL},
	{0x18FF0D19UL, 0x1FFFFFFFUL, 0x00001C00UL, 0x00000002UL},
	{0x1CFEC303UL, 0x1FFFFFFFUL, 0x00001D00UL, 0x00000002UL},
	{0x18FF14E7UL, 0x1FFFFFFFUL, 0x00001E00UL, 0x00000002UL},
	{0x18FED947UL, 0x1FFFFFFFUL, 0x00001F00UL, 0x00000002UL},
	{0x0C3DD7A7UL, 0x1FFFFFFFUL, 0x00002000UL, 0x00000002UL},
	{0x18F7021EUL, 0x1FFFFFFFUL, 0x00002100UL, 0x00000002UL},
	{0x18FF709EUL, 0x1FFFFFFFUL, 0x00002200UL, 0x00000002UL},
	{0x18FEE6EEUL, 0x1FFFFFFFUL, 0x00002300UL, 0x00000002UL},
	{0x18FF2130UL, 0x1FFFFFFFUL, 0x00002400UL, 0x00000002UL},
	{0x0CF6022EUL, 0x1FFFFFFFUL, 0x00002500UL, 0x00000002UL},
	{0x18FF6DD8UL, 0x1FFFFFFFUL, 0x00002600UL, 0x00000002UL},
	{0x18FFF931UL, 0x1FFFFFFFUL, 0x00002700UL, 0x00000002UL},
	{0x11FF3F32UL, 0x1FFFFFFFUL, 0x00002800UL, 0x00000002UL},
	{0x18FED631UL, 0x1FFFFFFFUL, 0x00002900UL, 0x00000002UL}, /*42*/
	{0x18FF8F25UL, 0x1FFFFFFFUL, 0x00002A00UL, 0x00000002UL}, /*43*/
	{0x0CF6034DUL, 0x1FFFFFFFUL, 0x00002B00UL, 0x00000002UL}, /*44*/
	{0x04F02070UL, 0x1FFFF0FFUL, 0x00005B00UL, 0x00000002UL}, /* 0x04F02070~0x04F02F70 */
	{0x1885EFF3UL, 0x1FFFFFFFUL, 0x00006700UL, 0x00000002UL}, /**/ /* lyx */
	{0x1886EFF3UL, 0x1FFFFFFFUL, 0x00006800UL, 0x00000002UL}, /**/ /* lyx */
	{0x1887EFF3UL, 0x1FFFFFFFUL, 0x00006900UL, 0x00000002UL}, /**/ /* lyx */
	{0x1888EFF3UL, 0x1FFFFFFFUL, 0x00006A00UL, 0x00000002UL}, /**/ /* lyx */

	/*CAN2 - PCAN*/
	{0x18DA17F1UL, 0x1FFFFFF0UL, 0x00002C00UL, 0x00000004UL}, /*1*/
	{0x00000151UL, 0x1FFFFFF0UL, 0x00002D00UL, 0x00000004UL},
	{0x000001F1UL, 0x1FFFFFF0UL, 0x00002E00UL, 0x00000004UL},
	{0x00000888UL, 0x1FFFFFFFUL, 0x00002F00UL, 0x00000004UL},
	{0x00000610UL, 0x1FFFFFFFUL, 0x00003000UL, 0x00000004UL},

	{0x18FECA00UL, 0x1FFFFF00UL, 0x00003100UL, 0x00000004UL},
	{0x18ECFF00UL, 0x1FFFFF00UL, 0x00003200UL, 0x00000004UL},
	{0x18EBFF00UL, 0x1FFFFF00UL, 0x00003300UL, 0x00000004UL}, /*8*/
	{0x18FFA0F5UL, 0x1FFFF0FFUL, 0x00003400UL, 0x00000004UL},
	{0x18FFA0F6UL, 0x1FFFF0FFUL, 0x00003500UL, 0x00000004UL}, /*10*/

	{0x0CFFEAF4UL, 0x1FFFFFFFUL, 0x00003600UL, 0x00000004UL},
	{0x18FFEEF4UL, 0x1FFFFFFFUL, 0x00003700UL, 0x00000004UL},
	{0x18FFF2F4UL, 0x1FFFFFFFUL, 0x00003800UL, 0x00000004UL},
	{0x0CFF11EFUL, 0x1FFFFFFFUL, 0x00003900UL, 0x00000004UL},
	{0x0CFF11F0UL, 0x1FFFFFFFUL, 0x00003A00UL, 0x00000004UL},
	{0x18F00503UL, 0x1FFFFFFFUL, 0x00003B00UL, 0x00000004UL},
	{0x18FE4A03UL, 0x1FFFFFFFUL, 0x00003C00UL, 0x00000004UL},
	{0x18FDA403UL, 0x1FFFFFFFUL, 0x00003D00UL, 0x00000004UL},
	{0x0CF00203UL, 0x1FFFFFFFUL, 0x00003E00UL, 0x00000004UL},
	{0x0CEF3103UL, 0x1FFFFFFFUL, 0x00003F00UL, 0x00000004UL},
	{0x0CFF0931UL, 0x1FFFFFFFUL, 0x00004000UL, 0x00000004UL},
	{0x18FF1F31UL, 0x1FFFFFFFUL, 0x00004100UL, 0x00000004UL},
	{0x18FFF531UL, 0x1FFFFFFFUL, 0x00004200UL, 0x00000004UL},
	{0x18FEF131UL, 0x1FFFFFFFUL, 0x00004300UL, 0x00000004UL},
	{0x18F0010BUL, 0x1FFFFFFFUL, 0x00004400UL, 0x00000004UL},
	{0x18F0090BUL, 0x1FFFFFFFUL, 0x00004500UL, 0x00000004UL},
	{0x18FDC40BUL, 0x1FFFFFFFUL, 0x00004600UL, 0x00000004UL},
	{0x18FE4F0BUL, 0x1FFFFFFFUL, 0x00004700UL, 0x00000004UL},
	{0x18FEBF0BUL, 0x1FFFFFFFUL, 0x00004800UL, 0x00000004UL},
	{0x18F00010UL, 0x1FFFFFFFUL, 0x00004900UL, 0x00000004UL},
	{0x10F007E8UL, 0x1FFFFFFFUL, 0x00004A00UL, 0x00000004UL},
	{0x18FE5BE8UL, 0x1FFFFFFFUL, 0x00004B00UL, 0x00000004UL},
	{0x18FF3C50UL, 0x1FFFFFFFUL, 0x00004C00UL, 0x00000004UL},
	{0x18F00150UL, 0x1FFFFFFFUL, 0x00004D00UL, 0x00000004UL},
	{0x0CF02F2AUL, 0x1FFFFFFFUL, 0x00004E00UL, 0x00000004UL},
	{0x0C010305UL, 0x1FFFFFFFUL, 0x00004F00UL, 0x00000004UL},
	{0x0C3DD7A7UL, 0x1FFFFFFFUL, 0x00005000UL, 0x00000004UL},
	{0x18FF5527UL, 0x1FFFFFFFUL, 0x00005100UL, 0x00000004UL},
	{0x18FFC006UL, 0x1FFFFFFFUL, 0x00005200UL, 0x00000004UL},
	{0x18FF7C4AUL, 0x1FFFFFFFUL, 0x00005300UL, 0x00000004UL},
	{0x18FF709EUL, 0x1FFFFFFFUL, 0x00005400UL, 0x00000004UL},
	{0x18FF7FDEUL, 0x1FFFFFFFUL, 0x00005500UL, 0x00000004UL}, /*42*/
	{0x19FF5CF3UL, 0x1FFFFFFFUL, 0x00005600UL, 0x00000004UL}, /*43*/
	{0x19FF5BF3UL, 0x1FFFFFFFUL, 0x00005700UL, 0x00000004UL}, /*44*/
	{0x18FF0B03UL, 0x1FFFFFFFUL, 0x00005800UL, 0x00000004UL}, /*45*/
	{0x18FF0B04UL, 0x1FFFFFFFUL, 0x00005900UL, 0x00000004UL}, /*46*/
	{0x0CF19682UL, 0x1FFFFFFFUL, 0x00005A00UL, 0x00000004UL}, /*47*/

};


static void Can_SetRxRule(void)
{
	volatile CanRxRuleCfg_t* RuleCfg;
	uint16_t RxRuleIdx;
	uint8_t  PageRxRuleIdx;

	/* Set Rx rule number per channel */
    RSCAN0.GAFLCFG0.UINT32 = 0x002F2D00;//lyx

	/* Receive Rule Table Write Enable, and select page 0
    RSCAN0GAFLECTR- Receive Rule Entry Control Register
    b31:b 9                  - Reserved set to 0
    b 8             AFLDAE   - Receive Rule Table Write Enable     - Receive rule table write is enabled. Set to 1
    b 7:b 5                  - Reserved set to 0
    b 4:b 0         AFLPN    - Receive Rule Table Page Number
                               Configuration                       - select pages 0. set to 00000'b */
    RSCAN0.GAFLECTR.UINT32  = 0x100UL;

	/* Get access base address of Rx rule */
    RuleCfg = (volatile CanRxRuleCfg_t*)&(RSCAN0_GAFLIDj(0));

	for (RxRuleIdx = 0U; RxRuleIdx < CAN_RX_RULE_NUM; RxRuleIdx++) 
	{
		PageRxRuleIdx = (uint8_t) (RxRuleIdx & 0x0fu);

		/* Update target Rx rule page if necessary. */
		if (PageRxRuleIdx == 0U) 
		{
			RSCAN0.GAFLECTR.BIT.AFLPN = (RxRuleIdx >> 4u);
		}

		/* Set a single Rx rule.*/
		RuleCfg[PageRxRuleIdx] = CanDrvRxRule[RxRuleIdx];
	}

    /* Receive Rule Table Write Disabel
    RSCAN0GAFLECTR- Receive Rule Entry Control Register
    b31:b 9                  - Reserved set to 0
    b 8             AFLDAE   - Receive Rule Table Write Enable     - Receive rule table write is disabled. Set to 0
    b 7:b 5                  - Reserved set to 0
    b 4:b 0         AFLPN    - Receive Rule Table Page Number
                               Configuration                       - unused. set to 00000'b */
    RSCAN0.GAFLECTR.UINT32 &= ~0x100UL;

}

/*****************************************************************************
** Function:    RS_CAN_init
** Description: Configures the CAN0 and CAN1
** Parameter:   None
** Return:      None
******************************************************************************/
void RS_CAN_init(void)
{
    /* default seting: Main Osc/1 -> CAN */
    //protected_write(SYSPROTCMDD1,SYSPROTS1,SYSCKSC_IRSCANXINS_CTL,0x01);
    //while (SYSCKSC_IRSCANXINS_ACT !=0x01); //wait for clk active

    //set the pull down pin
    //PORT_AWO.PM0.UINT16               &= 0xfffe;
    //PORT_AWO.PMC0.UINT16              &= 0xfffe;
    //PORT_AWO.P0.UINT16                &= 0xfffe;

    /* Set CAN0TX as P0_4 and CAN0RX as P0_5 */
    /* PORT_AWO.PMC0.UINT16  |= 0x0030;
    PORT_AWO.PFCE0.UINT16 |= (0x0030);
    PORT_AWO.PFC0.UINT16  |= (0x0030);
    PORT_AWO.PM0.UINT16   &= ~(1 << 4);
    PORT_AWO.PM0.UINT16   |= 1 << 5; */

	/* Set CAN1TX as P0_6 and CAN1RX as P0_7 */
    PORT_AWO.PMC0.UINT16  |= (0x00C0);
    PORT_AWO.PFCE0.UINT16 |= (0x00C0);
    PORT_AWO.PFC0.UINT16  |= (0x00C0);
    PORT_AWO.PM0.UINT16   &= ~(1 << 6);
    PORT_AWO.PM0.UINT16   |= 1 << 7;

	/* Set CAN2TX as P0_8 and CAN2RX as P0_9 */
	PORT_AWO.PMC0.UINT16  |= (0x0300);
    PORT_AWO.PFCE0.UINT16 |= (0x0300);
    PORT_AWO.PFC0.UINT16  |= (0x0300);
    PORT_AWO.PM0.UINT16   &= ~(1 << 8);
    PORT_AWO.PM0.UINT16   |= 1 << 9;

	#if 0
    /* Wait while CAN RAM initialization is ongoing */
    while((RSCAN0.GSTS.UINT32 & 0x00000008));

	/* Switch to global/channel reset mode */
	RSCAN0.GCTR.UINT32  &= ~R_CAN_GSLPR_MASK; /* GSLPR = 0 */
	// RSCAN0.C0CTR.UINT32 &= ~R_CAN_CSLPR_MASK; /* CSLPR = 0 */
	RSCAN0.C1CTR.UINT32 &= ~R_CAN_CSLPR_MASK; /* CSLPR = 0 */
	RSCAN0.C2CTR.UINT32 &= ~R_CAN_CSLPR_MASK; /* CSLPR = 0 */

	while ((RSCAN0.GSTS.UINT32 & R_CAN_GSLPSTS_ON) != 0UL);
	while ((RSCAN0.C1STS.UINT32 & R_CAN_CSLPSTS_ON) != 0UL);
	while ((RSCAN0.C2STS.UINT32 & R_CAN_CSLPSTS_ON) != 0UL);

	#else
	RSCAN0.GCTR.UINT32   = 0x00000005UL;
	while((RSCAN0.GSTS.UINT32 & 0x00000008));
	RSCAN0.GCTR.UINT32  &= 0xFFFFFFFB;
	RSCAN0.GCTR.UINT32  |= 0x00000001;
	while ((RSCAN0.GSTS.UINT32 & 0x00000001) != 0x00000001);

	RSCAN0.C1CTR.UINT32   = 0x00000005UL;
	while((RSCAN0.C1STS.UINT32 & 0x00000008));
	RSCAN0.C1CTR.UINT32  &= 0xFFFFFFFB;
	RSCAN0.C1CTR.UINT32  |= 0x00000001;
	while ((RSCAN0.C1STS.UINT32 & 0x00000001) != 0x00000001);

	RSCAN0.C2CTR.UINT32   = 0x00000005UL;
	while((RSCAN0.C2STS.UINT32 & 0x00000008));
	RSCAN0.C2CTR.UINT32  &= 0xFFFFFFFB;
	RSCAN0.C2CTR.UINT32  |= 0x00000001;
	while ((RSCAN0.C2STS.UINT32 & 0x00000001) != 0x00000001);

	#endif

    /* Configure clk_xincan as CAN-ClockSource */
    RSCAN0.GCFG.UINT32 = 0x00000010;

	/**
	Bits[9..0]   = 0  -> BRP
    Bits[19..16] = 12 -> TSEG1
    Bits[22..20] = 1  -> TSEG2
    Bits[25,24]  = 0  -> SJW
	必须满足 TSEG2 >= SJW
	当前 fCAN = 8MHz
    Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2)
	Sample  =(1+TSEG1)/(1+TSEG1+TSEG2) = ( 1 + 13 ) / ( 1 + 13 + 2 ) = 87.5%
	*/
	// RSCAN0.C0CFG.UINT32 = 0x001C0000; //SJW=1TQ, TSEG1=13TQ, TSEG2=2TQ, BRP=0 -->> Bitrate 500k
	// RSCAN0.C1CFG.UINT32 = 0x001C0001; //SJW=1TQ, TSEG1=13TQ, TSEG2=2TQ, BRP=1 -->> Bitrate 250k
	// RSCAN0.C2CFG.UINT32 = 0x001C0000; //SJW=1TQ, TSEG1=13TQ, TSEG2=2TQ, BRP=0 -->> Bitrate 500k
	if(eol_icutype == 0) //C5104
	{
		RSCAN0.C1CFG.UINT32 = 0x001C0001; //BCAN1  Bitrate 250k
		RSCAN0.C2CFG.UINT32 = 0x001C0001; //PCAN2  Bitrate 250k
	}
	else if(eol_icutype == 1 || eol_icutype == 3) //C3104 C3108
	{
		RSCAN0.C1CFG.UINT32 = 0x001C0001; //BCAN1  Bitrate 250K
		RSCAN0.C2CFG.UINT32 = 0x001C0000; //PCAN2  Bitrate 500K
	}
	else if(eol_icutype == 2) //C3106
	{
		RSCAN0.C1CFG.UINT32 = 0x001C0000; //BCAN1  Bitrate 500k
		RSCAN0.C2CFG.UINT32 = 0x001C0000; //PCAN2  Bitrate 500k
	}
	else
	{
		RSCAN0.C1CFG.UINT32 = 0x001C0001; //BCAN1  Bitrate 250k
		RSCAN0.C2CFG.UINT32 = 0x001C0001; //PCAN2  Bitrate 250k
	}

    /* ==== Rx rule setting ==== */
    Can_SetRxRule();

    /* ====receive buffer setting ==== */
    RSCAN0.RMNB.UINT32 = 0x00; //Can_SetGlobalBuffer--16

	/* Set THLEIE disabled, MEIE(FIFO Message Lost Interrupt disabled)  */
    RSCAN0.GCTR.UINT32 &= 0xfffff8ff;

	/* receive FIFO buffer configuration and control setting */
	// RSCAN0.RFCC0.UINT32 = 0x00007102;
	RSCAN0.RFCC1.UINT32 = 0x0000F702;
	RSCAN0.RFCC2.UINT32 = 0x0000F702;

	/* transmit/receive FIFO buffer configuration and control setting */
	// RSCAN0.CFCC0.UINT32 = 0x00016104;
	RSCAN0.CFCC3.UINT32 = 0x8015F300;
	RSCAN0.CFCC6.UINT32 = 0x8015F300;

    /* If GlobalChannel in halt or reset mode */
    if(RSCAN0.GSTS.UINT32 & 0x03)
    {
        RSCAN0.GCTR.UINT32 &= 0xfffffffc; //Switch to communication mode
        while((RSCAN0.GSTS.UINT32 & 0x02) == 2)
        {
            ;/* While halt mode */
        }
        while((RSCAN0.GSTS.UINT32 & 0x01) == 1)
        {
            ;/* While reset mode */
        }
    }
	#if 0
    /* If Channel-0 in halt or reset mode */
    if(RSCAN0.C0STS.UINT32 & 0x03) 
    {
		RSCAN0.C0CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
		while((RSCAN0.C0STS.UINT32 & 0x02) == 2)
		{
			;/* While halt mode */
		}
		while((RSCAN0.C0STS.UINT32 & 0x01) == 1)
		{
			;/* While reset mode */
		}
    }
	#endif
	/* If Channel-1 in halt or reset mode */
    if(RSCAN0.C1STS.UINT32 & 0x03)
    {
        RSCAN0.C1CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while((RSCAN0.C1STS.UINT32 & 0x02) == 2)
        {
            ;/* While halt mode */
        }
        while((RSCAN0.C1STS.UINT32 & 0x01) == 1)
        {
            ;/* While reset mode */
        }
    }

	/* If Channel-2 in halt or reset mode */
    if(RSCAN0.C2STS.UINT32 & 0x03)
    {
        RSCAN0.C2CTR.UINT32 &= 0xfffffffc;    //Switch to communication mode
        while((RSCAN0.C2STS.UINT32 & 0x02) == 2)
        {
            ;/* While halt mode */
        }
        while((RSCAN0.C2STS.UINT32 & 0x01) == 1)
        {
            ;/* While reset mode */
        }
    }

	// RSCAN0.RFCC0.UINT32 |= 0x01;
	RSCAN0.RFCC1.UINT32 |= 0x01;
	RSCAN0.RFCC2.UINT32 |= 0x01;

	// RSCAN0.CFCC0.UINT32 |= 0x01;
	RSCAN0.CFCC3.UINT32 |= 0x01;
	RSCAN0.CFCC6.UINT32 |= 0x01;

	PBG.FSGD0BPROT0.UINT32 = 0x07FFFFFF;
	PBG.FSGD0BPROT1.UINT32 = 0x07FFFFFF;
	INTC2.EIC70.UINT16 = 0x0047u;

}


void RS_CAN_RECV_Disabled(uint8_t channel)
{
	switch(channel)
	{
		case 1:
			RSCAN0.RFCC1.UINT32 &= ~0x01;
			break;
		case 2:
			RSCAN0.RFCC2.UINT32 &= ~0x01;
			break;
		default: 
			break;
	}
}
void RS_CAN_RECV_Enabled(uint8_t channel)
{
	switch(channel)
	{
		case 1:
			RSCAN0.RFCC1.UINT32 |= 0x01;
			break;
		case 2:
			RSCAN0.RFCC2.UINT32 |= 0x01;
			break;
		default: 
			break;
	}
}

uint8_t isCanBusOff(uint8_t channel)
{
	uint8_t  ret = 0;
	uint32_t canSTS = 0;

	switch(channel)
	{
		case 1:
			canSTS = RSCAN0.C1STS.UINT32;
			// if(RSCAN0.C1STS.BIT.BOSTS == 1)
			if((canSTS&0xB0) == 0xB0)
			{
				ret = 1;
				RSCAN0.C1CTR.BIT.CHMDC = 0x02;
			}
			break;
		case 2:
			canSTS = RSCAN0.C2STS.UINT32;
			// if(RSCAN0.C2STS.BIT.BOSTS == 1)
			if((canSTS&0xB0) == 0xB0)
			{
				ret = 1;
				RSCAN0.C2CTR.BIT.CHMDC = 0x02;
			}
			break;
		default:
			ret = 0;
			break;
	}

	return ret;
}

void RS_CAN_BusOff_Recovery(uint8_t channel)
{
	switch(channel)
	{
		case 1:
			RSCAN0.C1CTR.BIT.CHMDC = 0x00;
			break;
		case 2:
			RSCAN0.C2CTR.BIT.CHMDC = 0x00;
			break;
		default:
			break;
	}

}


/******************************************************************************
* Function Name : CAN1_SendMessage
* Description   : This function sends the message by Tx buffer 0.
* Argument      : message - pointer to stored data
******************************************************************************/
void CAN1_SendMessage(CAN_MESSAGE *message)
{
	int iTimeOut = 0;

	/* Check the Transmit Buffer Transmit Request Status
    RSCAN0TMSTSp  - Transmit Buffer Status Register 0
    b7:b5                    - Reserved set to 0
    b4:             TMTARM   - Transmit Buffer Transmit Abort Request Status Flag
    b3:             TMTRM    - Transmit Buffer Transmit Request Status Flag
    b2:b1           TMTRF    - Transmit Buffer Transmit Result Status Flag
    b0:             TMTSTS   - Transmit Buffer Transmit Status Flag */    
    if((RSCAN0.TMSTS16.UINT8 & R_CAN_TMTRM_ON) == 0x0U)
    {
        /* Clear status. */
        RSCAN0.TMSTS16.UINT8  = 0x00U;

        /* Set message data. */
        /* Set RSCAN0TMIDp register.
        RSCAN0TMIDp   - Transmit Buffer ID Register 0
        b31             TMIDE   - Transmit Buffer IDE                - Standard ID. set to 0
        b30             TMRTR   - Transmit Buffer RTR                - Data frame. set to 0
        b29             THLEN   - Transmit History Data Store Enable - Transmit history data is not stored
                                                                       in the buffer. set to 0
        b28:b0          TMID    - Transmit Buffer ID Data */
		RSCAN0.TMID16.UINT32  = (message->id & 0x1FFFFFFF);
		RSCAN0.TMID16.UINT32 |= 0x80000000;                   //Ext

        /* Set RSCAN0TMIDp register.
        RSCAN0TMPTRp  - Transmit Buffer Pointer Register 0
        b31:b28         TMDLC   - Transmit Buffer DLC Data
        b27:b24                 - Reserved set to 0
        b23:b16         TMPTR   - Transmit Buffer Label Data
        b15:b 0                 - Reserved set to 0  */
        RSCAN0.TMPTR16.UINT32 = (message->len & 0xF) << 28;

        /* Set RSCAN0TMDF0p register.
        RSCAN0TMDF0p  - Transmit Buffer Data Field 0 Register 0
        b31:b24         TMDB3   - Transmit Buffer Data Byte 3
        b23:b16         TMDB2   - Transmit Buffer Data Byte 2
        b15:b 8         TMDB1   - Transmit Buffer Data Byte 1
        b 7:b 0         TMDB0   - Transmit Buffer Data Byte 0 */
        RSCAN0.TMDF016.UINT32 = *((uint32_t *)&(message->data[0]));

        /* Set RSCAN0TMDF1p register.
        RSCAN0TMDF1p  - Transmit Buffer Data Field 0 Register 1
        b31:b24         TMDB7   - Transmit Buffer Data Byte 7
        b23:b16         TMDB6   - Transmit Buffer Data Byte 6
        b15:b 8         TMDB5   - Transmit Buffer Data Byte 5
        b 7:b 0         TMDB4   - Transmit Buffer Data Byte 4 */
        RSCAN0.TMDF116.UINT32 = *((uint32_t *)&(message->data[4]));

        /* Set transmission request.
        RSCAN0TMCp    - Transmit Buffer Control Register 0
        b 7:b 3                 - Reserved set to 0
        b 2             TMOM    - One-Shot Transmission Enable       - unused
        b 1             TMTAR   - Transmit Abort Request             - unused
        b 0             TMTR    - Transmit Request                   - Transmission is requested. set to 1 */
        RSCAN0.TMC16.UINT8    = R_CAN_TMTR_ON;

        /* Normal completion or time out. */
        while((RSCAN0.TMSTS16.UINT8 & 0x4) != 0x4)if(iTimeOut++ > 50000)break;

	}
}
/******************************************************************************
* Function Name : CAN2_SendMessage
* Description   : This function sends the message by Tx buffer 0.
* Argument      : message - pointer to stored data
******************************************************************************/
void CAN2_SendMessage(CAN_MESSAGE *message)
{
	int iTimeOut = 0;
    /* Check the Transmit Buffer Transmit Request Status
    RSCAN0TMSTSp  - Transmit Buffer Status Register 0
    b7:b5                    - Reserved set to 0
    b4:             TMTARM   - Transmit Buffer Transmit Abort Request Status Flag
    b3:             TMTRM    - Transmit Buffer Transmit Request Status Flag
    b2:b1           TMTRF    - Transmit Buffer Transmit Result Status Flag
    b0:             TMTSTS   - Transmit Buffer Transmit Status Flag */    
    if((RSCAN0.TMSTS32.UINT8 & R_CAN_TMTRM_ON) == 0x0U)
    {
        /* Clear status. */
        RSCAN0.TMSTS32.UINT8  = 0x00U;

        /* Set message data. */
        /* Set RSCAN0TMIDp register.
        RSCAN0TMIDp   - Transmit Buffer ID Register 0
        b31             TMIDE   - Transmit Buffer IDE                - Standard ID. set to 0
        b30             TMRTR   - Transmit Buffer RTR                - Data frame. set to 0
        b29             THLEN   - Transmit History Data Store Enable - Transmit history data is not stored
                                                                       in the buffer. set to 0
        b28:b0          TMID    - Transmit Buffer ID Data */
		RSCAN0.TMID32.UINT32  = (message->id & 0x1FFFFFFF);
		RSCAN0.TMID32.UINT32 |= 0x80000000;                   //Ext

        /* Set RSCAN0TMIDp register.
        RSCAN0TMPTRp  - Transmit Buffer Pointer Register 0
        b31:b28         TMDLC   - Transmit Buffer DLC Data
        b27:b24                 - Reserved set to 0
        b23:b16         TMPTR   - Transmit Buffer Label Data
        b15:b 0                 - Reserved set to 0  */
        RSCAN0.TMPTR32.UINT32 = (message->len & 0xF) << 28;

        /* Set RSCAN0TMDF0p register.
        RSCAN0TMDF0p  - Transmit Buffer Data Field 0 Register 0
        b31:b24         TMDB3   - Transmit Buffer Data Byte 3
        b23:b16         TMDB2   - Transmit Buffer Data Byte 2
        b15:b 8         TMDB1   - Transmit Buffer Data Byte 1
        b 7:b 0         TMDB0   - Transmit Buffer Data Byte 0 */
        RSCAN0.TMDF032.UINT32 = *((uint32_t *)&(message->data[0]));

        /* Set RSCAN0TMDF1p register.
        RSCAN0TMDF1p  - Transmit Buffer Data Field 0 Register 1
        b31:b24         TMDB7   - Transmit Buffer Data Byte 7
        b23:b16         TMDB6   - Transmit Buffer Data Byte 6
        b15:b 8         TMDB5   - Transmit Buffer Data Byte 5
        b 7:b 0         TMDB4   - Transmit Buffer Data Byte 4 */
        RSCAN0.TMDF132.UINT32 = *((uint32_t *)&(message->data[4]));

        /* Set transmission request.
        RSCAN0TMCp    - Transmit Buffer Control Register 0
        b 7:b 3                 - Reserved set to 0
        b 2             TMOM    - One-Shot Transmission Enable       - unused
        b 1             TMTAR   - Transmit Abort Request             - unused
        b 0             TMTR    - Transmit Request                   - Transmission is requested. set to 1 */
        RSCAN0.TMC32.UINT8    = R_CAN_TMTR_ON;

        /* Normal completion or time out. */
        while((RSCAN0.TMSTS32.UINT8 & 0x4) != 0x4)if(iTimeOut++ > 50000)break;

    }
}


/******************************************************************************
* Function Name : CAN1_ReceiveMessage
* Description   : This function receives the message by Rx fifo buffer 1.
* Argument      : message - pointer to stored data
******************************************************************************/
void CAN1_ReceiveMessage(CAN_MESSAGE *message)
{
	message->id  = RSCAN0.RFID1.UINT32 & 0x1FFFFFFF;
	message->len = RSCAN0.RFPTR1.UINT32 >> 28;

	*((int*)&(message->data[0]))  = RSCAN0.RFDF01.UINT32;
	*((int*)&(message->data[4]))  = RSCAN0.RFDF11.UINT32;

	RSCAN0.RFPCTR1.UINT32 = 0xFF;

	if((RSCAN0.RFSTS1.UINT32 & 0xFF00) == 0)
	{
		RSCAN0.RFSTS1.UINT32 &= 0xFFF7;
	}
}
/******************************************************************************
* Function Name : CAN2_ReceiveMessage
* Description   : This function receives the message by Rx fifo buffer 2.
* Argument      : message - pointer to stored data
******************************************************************************/
void CAN2_ReceiveMessage(CAN_MESSAGE *message)
{
	message->id  = RSCAN0.RFID2.UINT32 & 0x1FFFFFFF;
	message->len = RSCAN0.RFPTR2.UINT32 >> 28;

	*((int*)&(message->data[0]))  = RSCAN0.RFDF02.UINT32;
	*((int*)&(message->data[4]))  = RSCAN0.RFDF12.UINT32;

	RSCAN0.RFPCTR2.UINT32 = 0xFF;

	if((RSCAN0.RFSTS2.UINT32 & 0xFF00) == 0)
	{
		RSCAN0.RFSTS2.UINT32 &= 0xFFF7;
	}
}


void CAN1_SendFIFOMessage(CAN_MESSAGE *message)
{
	if((RSCAN0.CFSTS3.UINT32&0x02)==0x0)
    {
		RSCAN0.CFID3.UINT32   = (message->id & 0x1FFFFFFF);
		RSCAN0.CFID3.UINT32  |= 0x80000000; //Ext

        RSCAN0.CFPTR3.UINT32  = (message->len & 0xF) << 28;

        RSCAN0.CFDF03.UINT32  = *((uint32_t *)&(message->data[0]));
        RSCAN0.CFDF13.UINT32  = *((uint32_t *)&(message->data[4]));

        RSCAN0.CFPCTR3.UINT32 = 0xFF;

	}
}

void CAN2_SendFIFOMessage(CAN_MESSAGE *message)
{
	if((RSCAN0.CFSTS6.UINT32&0x02)==0x0)
    {
		RSCAN0.CFID6.UINT32   = (message->id & 0x1FFFFFFF);
		RSCAN0.CFID6.UINT32  |= 0x80000000; //Ext

        RSCAN0.CFPTR6.UINT32  = (message->len & 0xF) << 28;

        RSCAN0.CFDF06.UINT32  = *((uint32_t *)&(message->data[0]));
        RSCAN0.CFDF16.UINT32  = *((uint32_t *)&(message->data[4]));

        RSCAN0.CFPCTR6.UINT32 = 0xFF;

	}
}

