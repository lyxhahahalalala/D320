/*
****************************************************************************
PROJECT : device driver for V-Lib
FILE    : $Id: r_dev_isr.c 3010 2014-06-17 14:48:17Z florian.zimmermann $
============================================================================
DESCRIPTION
r7f701412 interrupt service routines
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.

****************************************************************************
*/


/*******************************************************************************
  Title: r7f701412 interrupt serivicing 

  
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"


/* The debug output for register accesses shall be only active 
on special request, otherwise the output might get "overloaded"  
If you want to see all register accesses, activate R_DBG_SHOW_INT_REGS
in your application's Makefile */
#ifndef R_DBG_SHOW_INT_REGS
  #undef  R_DBG_PRINT_DEV_REGS
#endif   
#include "r_dev_api.h"

/*******************************************************************************
  Section: Local Constants 
*/
/* FENMI factor status and clear registers */
#define LOC_FENMIF                  0xFF450000u      /* FENMI factor register */
#define LOC_FENMIFC                 0xFF450008u      /* FENMI factor clear register */

/* FENMI factor status and clear register bits */
#define LOC_NMI_FACTOR_NMI0F        0x1u
#define LOC_NMI_FACTOR_WDTA0NMIF    0x2u
#define LOC_NMI_FACTOR_WDTA1NMIF    0x4u
#define LOC_NMI_FACTOR_BAEXCNMIF    0x8u

/* gfxss0 Slave IP error capture and clear registers */
#define LOC_GSSLVERR                0xFF834830u
#define LOC_GSSLVERRC               0xFF834834u
#define LOC_GSSLPERR                0xFF834838u
#define LOC_GSMPUERR1               0xFF834828u
#define LOC_GSMPUERR0               0xFF834820u

/* gfxss0 Slave IP error capture and clear register bits */
#define LOC_GSSLVERR_SFMA0TTOUT   0x01u
#define LOC_GSSLVERR_SFMA1TTOUT   0x02u
#define LOC_GSSLVERR_VI0AHEINT      0x04u
#define LOC_GSSLVERR_SVO0MBE        0x80u


/*******************************************************************************
  Section: Local types 
*/
/*******************************************************************************
  Constant: loc_IntCtrl
  
  Stores the IC register address of all interrupts
  
*/
static const uint32_t loc_IntCtrl[R_DEV_INT_LAST] =
{
/* R_DEV_INT_WDTA0             */  0xFFFEEA00u,  
/* R_DEV_INT_WDTA1             */  0xFFFEEA02u,  
/* R_DEV_INT_RTCA01S           */  0xFFFEEA04u,  
/* R_DEV_INT_RTCA0AL           */  0xFFFEEA06u,  
/* R_DEV_INT_RTCA0R            */  0xFFFEEA08u,  
/* R_DEV_INT_AWOTM0            */  0xFFFEEA0Au,    
/* R_DEV_INT_P0                */  0xFFFEEA0Cu,  
/* R_DEV_INT_P1                */  0xFFFEEA0Eu,  
/* R_DEV_INT_P2                */  0xFFFEEA10u,  
/* R_DEV_INT_P3                */  0xFFFEEA12u,  
/* R_DEV_INT_P4                */  0xFFFEEA14u,  
/* R_DEV_INT_P5                */  0xFFFEEA16u,  
/* R_DEV_INT_P6                */  0xFFFEEA18u,  
/* R_DEV_INT_P7                */  0xFFFEEA1Au,  
/* R_DEV_INT_P8                */  0xFFFEEA1Cu,  
/* R_DEV_INT_P9                */  0xFFFEEA1Eu,  
/* R_DEV_INT_P10               */  0xFFFEEA20u,  
/* R_DEV_INT_TAUB0I0           */  0xFFFEEA22u,  
/* R_DEV_INT_TAUB0I1           */  0xFFFEEA24u,  
/* R_DEV_INT_TAUB0I2           */  0xFFFEEA26u,  
/* R_DEV_INT_TAUB0I3           */  0xFFFEEA28u,  
/* R_DEV_INT_TAUB0I4           */  0xFFFEEA2Au,  
/* R_DEV_INT_TAUB0I5           */  0xFFFEEA2Cu,  
/* R_DEV_INT_TAUB0I6           */  0xFFFEEA2Eu,  
/* R_DEV_INT_TAUB0I7           */  0xFFFEEA30u,  
/* R_DEV_INT_TAUB0I8           */  0xFFFEEA32u,  
/* R_DEV_INT_TAUB0I9           */  0xFFFEEA34u,  
/* R_DEV_INT_TAUB0I10          */  0xFFFEEA36u,  
/* R_DEV_INT_TAUB0I11          */  0xFFFEEA38u,  
/* R_DEV_INT_TAUB0I12          */  0xFFFEEA3Au,  
/* R_DEV_INT_TAUB0I13          */  0xFFFEEA3Cu,  
/* R_DEV_INT_TAUB0I14          */  0xFFFEEA3Eu,  
/* R_DEV_INT_TAUB0I15          */  0xFFFFB040u,  
/* R_DEV_INT_TAUB1I0           */  0xFFFFB042u,  
/* R_DEV_INT_TAUB1I1           */  0xFFFFB044u,  
/* R_DEV_INT_TAUB1I2           */  0xFFFFB046u,  
/* R_DEV_INT_TAUB1I3           */  0xFFFFB048u,  
/* R_DEV_INT_TAUB1I4           */  0xFFFFB04Au,  
/* R_DEV_INT_TAUB1I5           */  0xFFFFB04Cu,  
/* R_DEV_INT_TAUB1I6           */  0xFFFFB04Eu,  
/* R_DEV_INT_TAUB1I7           */  0xFFFFB050u,  
/* R_DEV_INT_TAUB1I8           */  0xFFFFB052u,  
/* R_DEV_INT_TAUB1I9           */  0xFFFFB054u,  
/* R_DEV_INT_TAUB1I10          */  0xFFFFB056u,  
/* R_DEV_INT_TAUB1I11          */  0xFFFFB058u,  
/* R_DEV_INT_TAUB1I12          */  0xFFFFB05Au,  
/* R_DEV_INT_TAUB1I13          */  0xFFFFB05Cu,  
/* R_DEV_INT_TAUB1I14          */  0xFFFFB05Eu,  
/* R_DEV_INT_TAUB1I15          */  0xFFFFB060u,  
/* R_DEV_INT_TAUB2I0           */  0xFFFFB062u,  
/* R_DEV_INT_TAUB2I1           */  0xFFFFB064u,  
/* R_DEV_INT_TAUB2I2           */  0xFFFFB066u,  
/* R_DEV_INT_TAUB2I3           */  0xFFFFB068u,  
/* R_DEV_INT_TAUB2I4           */  0xFFFFB06Au,  
/* R_DEV_INT_TAUB2I5           */  0xFFFFB06Cu,  
/* R_DEV_INT_TAUB2I6           */  0xFFFFB06Eu,  
/* R_DEV_INT_TAUB2I7           */  0xFFFFB070u,  
/* R_DEV_INT_TAUB2I8           */  0xFFFFB072u,  
/* R_DEV_INT_TAUB2I9           */  0xFFFFB074u,  
/* R_DEV_INT_TAUB2I10          */  0xFFFFB076u,  
/* R_DEV_INT_TAUB2I11          */  0xFFFFB078u,  
/* R_DEV_INT_TAUB2I12          */  0xFFFFB07Au,  
/* R_DEV_INT_TAUB2I13          */  0xFFFFB07Cu,  
/* R_DEV_INT_TAUB2I14          */  0xFFFFB07Eu,  
/* R_DEV_INT_TAUB2I15          */  0xFFFFB080u,  
/* R_DEV_INT_ADC0TSN           */  0xFFFFB082u,  
/* R_DEV_INT_ADC0SG1           */  0xFFFFB084u,  
/* R_DEV_INT_ADC0SG2           */  0xFFFFB086u,  
/* R_DEV_INT_ADC0SG3           */  0xFFFFB088u,  
/* R_DEV_INT_RCANGERR          */  0xFFFFB08Au,  
/* R_DEV_INT_RCANGRECC         */  0xFFFFB08Cu,  
/* R_DEV_INT_RCAN0ERR          */  0xFFFFB08Eu,  
/* R_DEV_INT_RCAN0REC          */  0xFFFFB090u,  
/* R_DEV_INT_RCAN0TRX          */  0xFFFFB092u,  
/* R_DEV_INT_CSIG0RE           */  0xFFFFB094u,  
/* R_DEV_INT_CSIG0R            */  0xFFFFB096u,  
/* R_DEV_INT_CSIG0IC           */  0xFFFFB098u,  
/* R_DEV_INT_CSIH0RE           */  0xFFFFB09Au,  
/* R_DEV_INT_CSIH0R            */  0xFFFFB09Cu,  
/* R_DEV_INT_CSIH0C            */  0xFFFFB09Eu,  
/* R_DEV_INT_CSIH0JC           */  0xFFFFB0A0u,  
/* R_DEV_INT_RLIN30S           */  0xFFFFB0A2u,  
/* R_DEV_INT_RLIN30R           */  0xFFFFB0A4u,  
/* R_DEV_INT_RLIN30T           */  0xFFFFB0A6u,  
/* R_DEV_INT_RLIN31S           */  0xFFFFB0A8u,  
/* R_DEV_INT_RLIN31R           */  0xFFFFB0AAu,  
/* R_DEV_INT_RLIN31T           */  0xFFFFB0ACu,  
/* R_DEV_INT_SG0TI             */  0xFFFFB0AEu,  
/* R_DEV_INT_SG1TI             */  0xFFFFB0B0u,  
/* R_DEV_INT_SG2TI             */  0xFFFFB0B2u,  
/* R_DEV_INT_SG3TI             */  0xFFFFB0B4u,  
/* R_DEV_INT_SG4TI             */  0xFFFFB0B6u,  
/* R_DEV_INT_DCUTDI            */  0xFFFFB0B8u,  
/* R_DEV_INT_DMAERR            */  0xFFFFB0BAu,  
/* R_DEV_INT_DMA0              */  0xFFFFB0BCu,  
/* R_DEV_INT_DMA1              */  0xFFFFB0BEu,  
/* R_DEV_INT_DMA2              */  0xFFFFB0C0u,  
/* R_DEV_INT_DMA3              */  0xFFFFB0C2u,  
/* R_DEV_INT_DMA4              */  0xFFFFB0C4u,  
/* R_DEV_INT_DMA5              */  0xFFFFB0C6u,  
/* R_DEV_INT_DMA6              */  0xFFFFB0C8u,  
/* R_DEV_INT_DMA7              */  0xFFFFB0CAu,  
/* R_DEV_INT_DMA8              */  0xFFFFB0CCu,  
/* R_DEV_INT_DMA9              */  0xFFFFB0CEu,  
/* R_DEV_INT_DMA10             */  0xFFFFB0D0u,  
/* R_DEV_INT_DMA11             */  0xFFFFB0D2u,  
/* R_DEV_INT_DMA12             */  0xFFFFB0D4u,  
/* R_DEV_INT_DMA13             */  0xFFFFB0D6u,  
/* R_DEV_INT_DMA14             */  0xFFFFB0D8u,  
/* R_DEV_INT_DMA15             */  0xFFFFB0DAu,  
/* R_DEV_INT_IIC0EE            */  0xFFFFB0DCu,  
/* R_DEV_INT_IIC0RI            */  0xFFFFB0DEu,  
/* R_DEV_INT_IIC0TI            */  0xFFFFB0E0u,  
/* R_DEV_INT_IIC0TEI           */  0xFFFFB0E2u,  
/* R_DEV_INT_IIC1EE            */  0xFFFFB0E4u,  
/* R_DEV_INT_IIC1RI            */  0xFFFFB0E6u,  
/* R_DEV_INT_IIC1TI            */  0xFFFFB0E8u,  
/* R_DEV_INT_IIC1TEI           */  0xFFFFB0EAu,  
/* R_DEV_INT_RCAN1ERR          */  0xFFFFB0ECu,  
/* R_DEV_INT_RCAN1REC          */  0xFFFFB0EEu,  
/* R_DEV_INT_RCAN1TRX          */  0xFFFFB0F0u,  
/* R_DEV_INT_TAUJ0I0           */  0xFFFFB0F2u,  
/* R_DEV_INT_TAUJ0I1           */  0xFFFFB0F4u,  
/* R_DEV_INT_TAUJ0I2           */  0xFFFFB0F6u,  
/* R_DEV_INT_TAUJ0I3           */  0xFFFFB0F8u,  
/* R_DEV_INT_OSTM0             */  0xFFFFB0FAu,  
/* R_DEV_INT_OSTM1             */  0xFFFFB0FCu,  
/* R_DEV_INT_CSIG1RE           */  0xFFFFB0FEu,  
/* R_DEV_INT_CSIG1R            */  0xFFFFB100u,  
/* R_DEV_INT_CSIG1IC           */  0xFFFFB102u,  
/* R_DEV_INT_CSIG2RE           */  0xFFFFB104u,                          
/* R_DEV_INT_CSIG2R            */  0xFFFFB106u,                          
/* R_DEV_INT_CSIG2IC           */  0xFFFFB108u,  
/* R_DEV_INT_CSIG3RE           */  0xFFFFB10Au,  
/* R_DEV_INT_CSIG3R            */  0xFFFFB10Cu,  
/* R_DEV_INT_CSIG3IC           */  0xFFFFB10Eu,  
/* R_DEV_INT_CSIH1RE           */  0xFFFFB110u,  
/* R_DEV_INT_CSIH1R            */  0xFFFFB112u,  
/* R_DEV_INT_CSIH1C            */  0xFFFFB114u,  
/* R_DEV_INT_CSIH1JC           */  0xFFFFB116u,  
/* R_DEV_INT_PWSB0QFULL        */  0xFFFFB118u,  
/* R_DEV_INT_PWGA0             */  0xFFFFB11Au,  
/* R_DEV_INT_PWGA1             */  0xFFFFB11Cu,  
/* R_DEV_INT_PWGA2             */  0xFFFFB11Eu,  
/* R_DEV_INT_PWGA3             */  0xFFFFB120u,  
/* R_DEV_INT_PWGA4             */  0xFFFFB122u,  
/* R_DEV_INT_PWGA5             */  0xFFFFB124u,  
/* R_DEV_INT_PWGA6             */  0xFFFFB126u,  
/* R_DEV_INT_PWGA7             */  0xFFFFB128u,  
/* R_DEV_INT_PWGA8             */  0xFFFFB12Au,  
/* R_DEV_INT_PWGA9             */  0xFFFFB12Cu,  
/* R_DEV_INT_PWGA10            */  0xFFFFB12Eu,  
/* R_DEV_INT_PWGA11            */  0xFFFFB130u,  
/* R_DEV_INT_PWGA12            */  0xFFFFB132u,  
/* R_DEV_INT_PWGA13            */  0xFFFFB134u,  
/* R_DEV_INT_PWGA14            */  0xFFFFB136u,  
/* R_DEV_INT_PWGA15            */  0xFFFFB138u,  
/* R_DEV_INT_PWGA16            */  0xFFFFB13Au,  
/* R_DEV_INT_PWGA17            */  0xFFFFB13Cu,  
/* R_DEV_INT_PWGA18            */  0xFFFFB13Eu,  
/* R_DEV_INT_PWGA19            */  0xFFFFB140u,  
/* R_DEV_INT_PWGA20            */  0xFFFFB142u,  
/* R_DEV_INT_PWGA21            */  0xFFFFB144u,  
/* R_DEV_INT_PWGA22            */  0xFFFFB146u,  
/* R_DEV_INT_PWGA23            */  0xFFFFB148u,  
/* R_DEV_INT_ICUSTWRDY         */  0xFFFFB14Au,  
/* R_DEV_INT_ICUSTRRDY         */  0xFFFFB14Cu,  
/* R_DEV_INT_167               */  0xFFFFB14Eu,  
/* R_DEV_INT_SCI3BRXI          */  0xFFFFB150u,  
/* R_DEV_INT_SCI3BTXI          */  0xFFFFB152u,  
/* R_DEV_INT_SCI3BTEI          */  0xFFFFB154u,  
/* R_DEV_INT_FLERR             */  0xFFFFB156u,  
/* R_DEV_INT_FLENDNM           */  0xFFFFB158u,  
/* R_DEV_INT_173               */  0xFFFFB15Au,  
/* R_DEV_INT_LCDBIFRDY         */  0xFFFFB15Cu,  
/* R_DEV_INT_LCDBIFEMPTY       */  0xFFFFB15Eu,  
/* R_DEV_INT_LCDBIFHALF        */  0xFFFFB160u,  
/* R_DEV_INT_LCDBIFFULL        */  0xFFFFB162u,  
/* R_DEV_INT_LCDBIFQTR         */  0xFFFFB164u,  
/* R_DEV_INT_LCDBIF3QTR        */  0xFFFFB166u,  
/* R_DEV_INT_RCAN2ERR          */  0xFFFFB168u,  
/* R_DEV_INT_RCAN2REC          */  0xFFFFB16Au,  
/* R_DEV_INT_RCAN2TRX          */  0xFFFFB16Cu,  
/* R_DEV_INT_PCMP0TFFIL        */  0xFFFFB16Eu,  
/* R_DEV_INT_PCMP0TFERR        */  0xFFFFB170u,  
/* R_DEV_INT_ISM0REACHED       */  0xFFFFB172u,  
/* R_DEV_INT_ISM0DONE          */  0xFFFFB174u,  
/* R_DEV_INT_ISM0ZPDAD         */  0xFFFFB176u,  
/* R_DEV_INT_188               */  0xFFFFB178u,  
/* R_DEV_INT_SSIF0             */  0xFFFFB17Au,  
/* R_DEV_INT_SSIF0RX           */  0xFFFFB17Cu,  
/* R_DEV_INT_SSIF0TX           */  0xFFFFB17Eu,  
/* R_DEV_INT_SSIF1             */  0xFFFFB180u,  
/* R_DEV_INT_SSIF1RX           */  0xFFFFB182u,  
/* R_DEV_INT_SSIF1TX           */  0xFFFFB184u,  
/* R_DEV_INT_SDRAMC            */  0xFFFFB186u,  
/* R_DEV_INT_DAVEHD0PAUSE      */  0xFFFFB188u,  
/* R_DEV_INT_DAVEHD0SYNC       */  0xFFFFB18Au,  
/* R_DEV_INT_DAVEHD0SP         */  0xFFFFB18Cu,  
/* R_DEV_INT_VDCE0ERR          */  0xFFFFB18Eu,  
/* R_DEV_INT_VDCE0GR3VBLANK    */  0xFFFFB190u,  
/* R_DEV_INT_VDCE0S0VIVSYNC    */  0xFFFFB192u,  
/* R_DEV_INT_VDCE0S0LOVSYNC    */  0xFFFFB194u,  
/* R_DEV_INT_VDCE0GR3VLINE     */  0xFFFFB196u,  
/* R_DEV_INT_VDCE0S0VFIELD     */  0xFFFFB198u,  
/* R_DEV_INT_VDCE0S0WLINE      */  0xFFFFB19Au,  
/* R_DEV_INT_VDCE0S1LOVSYNC    */  0xFFFFB19Cu,  
/* R_DEV_INT_VDCE0S1WLINE      */  0xFFFFB19Eu,  
/* R_DEV_INT_VDCE0OIRVIVSYNC   */  0xFFFFB1A0u,  
/* R_DEV_INT_VDCE0OIRLOVSYNC   */  0xFFFFB1A2u,  
/* R_DEV_INT_VDCE0IRVLINE      */  0xFFFFB1A4u,  
/* R_DEV_INT_VDCE0IRWLINE      */  0xFFFFB1A6u,  
/* R_DEV_INT_212               */  0xFFFFB1A8u,  
/* R_DEV_INT_213               */  0xFFFFB1AAu,  
/* R_DEV_INT_214               */  0xFFFFB1ACu,  
/* R_DEV_INT_RLIN32S           */  0xFFFFB1AEu,  
/* R_DEV_INT_RLIN32R           */  0xFFFFB1B0u,  
/* R_DEV_INT_RLIN32T           */  0xFFFFB1B2u,  
/* R_DEV_INT_RLIN33S           */  0xFFFFB1B4u,  
/* R_DEV_INT_RLIN33R           */  0xFFFFB1B6u,  
/* R_DEV_INT_RLIN33T           */  0xFFFFB1B8u,  
/* R_DEV_INT_VDCE1ERR          */  0xFFFFB1BAu,  
/* R_DEV_INT_VDCE1GR3VBLANK    */  0xFFFFB1BCu,  
/* R_DEV_INT_VDCE1S0VIVSYNC    */  0xFFFFB1BEu,  
/* R_DEV_INT_VDCE1S0LOVSYNC    */  0xFFFFB1C0u,  
/* R_DEV_INT_VDCE1GR3VLINE     */  0xFFFFB1C2u,  
/* R_DEV_INT_VDCE1S0VFIELD     */  0xFFFFB1C4u,  
/* R_DEV_INT_VDCE1S0WLINE      */  0xFFFFB1C6u,  
/* R_DEV_INT_VDCE1S1LOVSYNC    */  0xFFFFB1C8u,  
/* R_DEV_INT_VDCE1S1WLINE      */  0xFFFFB1CAu,  
/* R_DEV_INT_230               */  0xFFFFB1CCu,  
/* R_DEV_INT_231               */  0xFFFFB1CEu,  
/* R_DEV_INT_232               */  0xFFFFB1D0u,  
/* R_DEV_INT_233               */  0xFFFFB1D2u,  
/* R_DEV_INT_MLB0STA0          */  0xFFFFB1D4u,  
/* R_DEV_INT_MLB0STA1          */  0xFFFFB1D6u,  
/* R_DEV_INT_MLB0SYS           */  0xFFFFB1D8u,  
/* R_DEV_INT_MLB0HBI           */  0xFFFFB1DAu,  
/* R_DEV_INT_MIPICOVF          */  0xFFFFB1DCu,  
/* R_DEV_INT_MIPICCTL          */  0xFFFFB1DEu,  
/* R_DEV_INT_IMRD0 / _VOWE     */  0xFFFFB1E0u,  
/* R_DEV_INT_241               */  0xFFFFB1E2u,  
/* R_DEV_INT_JCU0EDI           */  0xFFFFB1E4u,  
/* R_DEV_INT_JCU0DTI           */  0xFFFFB1E6u,  
/* R_DEV_INT_ETH0LINE0         */  0xFFFFB1E8u,  
/* R_DEV_INT_ETH0LINE1         */  0xFFFFB1EAu,  
/* R_DEV_INT_ETH0LINE2         */  0xFFFFB1ECu,  
/* R_DEV_INT_ETH0LINE3         */  0xFFFFB1EEu,  
/* R_DEV_INT_PFSSMISRERR       */  0xFFFFB1F0u,  
/* R_DEV_INT_VPISG1OVFERR      */  0xFFFFB1F2u,  
/* R_DEV_INT_VPISG2OVFERR      */  0xFFFFB1F4u,  
/* R_DEV_INT_VPISG3OVFERR      */  0xFFFFB1F6u,  
/* R_DEV_INT_252               */  0xFFFFB1F8u,  
/* R_DEV_INT_VPISG5OVFERR      */  0xFFFFB1FAu,  
/* R_DEV_INT_254               */  0xFFFFB1FCu,  
/* R_DEV_INT_255               */  0xFFFFB1FEu
                  
};


/*******************************************************************************
  Section: Local functions
*/
/* Notify function for other modules */
static void (*loc_NmiCallback)(void);
static uint32_t loc_FeNmiIsr(void);


/*******************************************************************************
  Function: R_DEV_IntSys_FeNmiIsr

  local ISR for handling the FE-NMI interrupt(s) 
  Parameters:
  void
  
  Returns:
  NMI code (reason factor) 
  
*/
static uint32_t loc_FeNmiIsr(void) 
{
    uint32_t nmifactor, subfactor;
    
    nmifactor = R_DEV_READ_REG(32, LOC_FENMIF);
    if ((nmifactor & LOC_NMI_FACTOR_NMI0F) != 0u) 
    {
        /* User can implement handling of NMI0F factor here. */
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_NMI0F);
        for (;;)
        {
        }
    }
    if ((nmifactor & LOC_NMI_FACTOR_WDTA0NMIF) != 0u) 
    {
        /* User can implement handling of WDTA0NMIF factor here. */
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_WDTA0NMIF);
        for (;;)
        {
        }
    }
    if ((nmifactor & LOC_NMI_FACTOR_WDTA1NMIF) != 0u) 
    {
        /* User can implement handling of WDTA1NMIF factor here. */
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_WDTA1NMIF);
        for (;;)
        {
        }
    }
    if ((nmifactor & LOC_NMI_FACTOR_BAEXCNMIF) != 0u) 
    {
        /* handle (gfxss) bus access exceptions (all BAECXNMI sources) */
        
        /* list of all BAECXNMI sources:
           GSSLPERR    (EMC0 - SDECC1ERR to SDECC2ERR, ILASDM1ERR to ILASDM4ERR)
           GSSLVERR    (SVO0MBEINT, VI0AHBEINT, SFMA1TTOUT, SFMA0TTOUT)
           GSMPUERR1   (SFMA1MPUERR, SFMA0MPUERR, EMCMPUERR, CPUHBSMPUERR)
           GSMPUERR0   (GMPU3)
        */
        
        /* from EMC0
           GSSLPERR    (EMC0 - SDECC1ERR to SDECC2ERR, ILASDM1ERR to ILASDM4ERR) */
        subfactor = R_DEV_READ_REG(32, LOC_GSSLPERR);
        if (subfactor != 0u) 
        {
            /* User can implement handling of GSSLPERR factor here. */
            for (;;)
            {
                /* EMC0 error */
            }
        }
               
        /* GSSLVERR from SVO, VI and SFMA
            (SVO0MBEINT, VI0AHBEINT, SFMA1TTOUT, SFMA0TTOUT) */
        subfactor = R_DEV_READ_REG(32, LOC_GSSLVERR);
        if ((subfactor & LOC_GSSLVERR_SFMA0TTOUT) != 0u) 
        {
            /* User can implement handling of SFMA0TTOUT factor here. */
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_SFMA0TTOUT);
            for (;;)
            {
            }
        }
        if ((subfactor & LOC_GSSLVERR_SFMA1TTOUT) != 0u) 
        {
            /* User can implement handling of SFMA1TTOUT factor here. */
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_SFMA1TTOUT);
            for (;;)
            {
            }
        }
        if ((subfactor & LOC_GSSLVERR_VI0AHEINT) != 0u) 
        {
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_VI0AHEINT);
        }
        if ((subfactor & LOC_GSSLVERR_SVO0MBE) != 0u) 
        {
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_SVO0MBE);
        }
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_BAEXCNMIF);
        
        subfactor = R_DEV_READ_REG(32, LOC_GSMPUERR1);
        if (subfactor != 0u) 
        {
            for (;;)
            {
                /* User can implement handling of GMPU4, GMPU3,
                                           GMPU1, and GMPU0 factors here. */
                /* error: (protection violation errors)
                     SFMA1MPUERR (GMPU4),
                     SFMA0MPUERR (GMPU3),
                     EMCMPUERR   (GMPU1) or
                     CPUHBSMPUERR  (GMPU0) */
            }
        }
        
        /*  GSMPUERR0   (GMPU3)*/
        subfactor = R_DEV_READ_REG(32, LOC_GSMPUERR0);
        if (subfactor != 0u) 
        {
            for (;;)
            {
                /* User can implement handling of GMPU3 factor here. */
                /* GMPU3 error (unmapped access errors) */
                /* Hint: You might have used a DS2.0 device with the
                         XBus limitation, but did not activated the
                         software GFXBUS_WORKAROUND */

            
            }
        }
    }
    return (nmifactor);
}

/*******************************************************************************
  Section: Global functions
*/

/*******************************************************************************
  Function: R_DEV_FeNmiIsr

  See: <r_dev_api.h> for details 
*/
void R_DEV_FeNmiIsr(void)
{    
    uint32_t ret;
    /* handle FENMI via device specific sys callback */
    ret = loc_FeNmiIsr();
    if (0u == ret)
    {        
        /* otherwise use standard callback */
        if (0u != loc_NmiCallback)
        {
            loc_NmiCallback();
        } else 
        {
            for (;;)
            {
                /* FeNmi Error occurred */
            }
        }
    }
}


/*******************************************************************************
  Function: void R_DEV_IntSetPrio

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntSetPrio(r_dev_IntSel_t Int, uint8_t Prio) 
{
    uint8_t regval = 0 ;
    R_DBG_PRINT(R_DBG_MSG_DET, "Setting Interrupt priority"); 
    /* first read the actual value */
    regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]); 
    /* modifiy value */
    regval &= 0xF0u;
    regval |= Prio; 
    /* write the new value */
    R_DEV_WRITE_REG(8, loc_IntCtrl[Int], regval); 
}

/*******************************************************************************
  Function: void R_DEV_IntClearFlag

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntClearFlag(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        /* modifiy value */
        regval &= (uint8_t)(~0x10u);
        /* write the new value */
        R_DEV_WRITE_REG(8, loc_IntCtrl[Int]+1u, regval); 
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }

}

/*******************************************************************************
  Function: void R_DEV_IntSetMode

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntSetMode(r_dev_IntSel_t Int, uint8_t Mode) 
{
    uint8_t regval = 0 ;
    uint8_t check  = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]); 
        if (R_DEV_INT_DIRECT_BRANCH == Mode)
        {
            /* modifiy value */
            regval &= (uint8_t)(~((uint8_t)0x40u));
        }
        else 
        {
            /* modifiy value */
            regval |= (uint8_t)0x40u;
            
        }
        /* write the new value */
        R_DEV_WRITE_REG(8, loc_IntCtrl[Int], regval); 
        check = R_DEV_READ_REG(8, loc_IntCtrl[Int]);
        if (regval != check)
        {
            R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "Setting mismatch!");  
        }     

    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }


}

/*******************************************************************************
  Function: void R_DEV_IntSetFlag

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntSetFlag(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        /* modifiy value */
        regval |= (uint8_t)0x10u;
        /* write the new value */
        R_DEV_WRITE_REG(8, loc_IntCtrl[Int]+1u, regval); 
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }

}

/*******************************************************************************
  Function: void R_DEV_IntGetFlag

  See: <r_dev_api.h> for details 
*/

uint8_t R_DEV_IntGetFlag(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        regval = (regval >> 4u) & 0x01u;
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }
    return regval; 

}

/*******************************************************************************
  Function: void R_DEV_IntGetMask

  See: <r_dev_api.h> for details 
*/

uint8_t R_DEV_IntGetMask(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        regval = (regval >> 7u) & 0x01u;
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }
    return regval; 
}

/*******************************************************************************
  Function: R_DEV_IntEnable

  See: <r_dev_api.h> for details 
*/
void R_DEV_IntEnable(r_dev_IntSel_t Int, uint8_t Enable) 
{
    uint8_t regval = 0 ;
    
    /* first read the actual value */
    regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]); 

    /* clear bit 7 to enable the interrupt */
    if (0u != Enable) 
    {
        regval &= (uint8_t)(~(0x80u)); 
    } 
    /* Set bit 7 to disable interrupt */
    else {
        regval |= 0x80u; 
    }
    /* write the new value */
    R_DEV_WRITE_REG(8, loc_IntCtrl[Int], regval); 

}

