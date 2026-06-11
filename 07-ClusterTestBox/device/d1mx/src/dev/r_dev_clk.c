/*
****************************************************************************
PROJECT : clock device driver
FILE    : $Id: r_dev_clk.c 3926 2014-09-22 11:52:07Z golczewskim $
============================================================================
DESCRIPTION
clock driver system functions for D1Mx (D1M1(H) .. D1M2(H))
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2016
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
  Title: D1Mx clock driver

*/
/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_gpio_api.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h>
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Section: Local Types
*/

/*******************************************************************************
  Type: loc_PllCfgReg_t
  
  Contents of the PLL clock control/configuration register
  
  Members:
  NI                - Integer part of Divider Nr
  P                 - Pr
  M                 - Mr
  SSMode            - Spread Spectrum Mode
  NF                - real part of Nr
  SSPerc            - Spread Spectrum Percentage
  SSFreq            - Spread Spectrum middle frequency
  FVV               - VCO output frequency range
*/

typedef struct {
    uint32_t  NI        : 7;
    uint32_t  P         : 3;
    uint32_t  M         : 2;
    uint32_t  SSMode    : 2;
    uint32_t  NF        : 4;
    uint32_t  SSPerc    : 3;
    uint32_t  SMFreq    : 5;
    uint32_t  FVV       : 3;
} loc_PllCfgReg_t;


/*******************************************************************************
  Type: loc_CksBase_t
  
  Cks Base address type
  If there is no selection, because only one source clock is there,
  the ID of the clock is put instead the selector address
  If there is no divider register, the address is 0
  
  Members:
  Sel           - Address of clock source selection register
  Div           - Address of divider
  DivIdx        - index of the corresponding entry in the  divider table
  DivMsk        - Mask - valid divider bits
*/

typedef struct {
    uint32_t  Sel;
    uint32_t  Div;
    uint16_t  DivIdx;
    uint16_t  DivMsk;
} loc_CksBase_t;


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: LOC_HIGHS_FRQ_HZ
  
  The frequency of the internal High speed oscillator
  
*/

#define LOC_HIGHS_FRQ_HZ     (8000000uL)

/*******************************************************************************
  Constant: LOC_LOWS_FRQ_HZ
  
  The frequency of the internal High speed oscillator
  
*/

#define LOC_LOWS_FRQ_HZ     (240000uL)


/*******************************************************************************
  Constants: Register Offsets for clockgenerator registers

  LOC_GEN_E       - Enable register
  LOC_GEN_S       - Status register
  LOC_GEN_C       - Control register
  LOC_GEN_STABI   - Stabilisation time
  LOC_GEN_STPMSK  - Stop mask
*/

#define LOC_GEN_E       0x00u
#define LOC_GEN_S       0x04u
#define LOC_GEN_C       0x08u
#define LOC_GEN_STABI   0x0Cu
#define LOC_GEN_STPMSK  0x18u


/*******************************************************************************
  Constants: Register Offsets for clock selector registers

 LOC_CKS_S       - Status
 LOC_CKS_A       - Active selection
 LOC_CKS_STPMSK  - Stop mask
*/

#define LOC_CKS_S       0x04u
#define LOC_CKS_A       0x08u
#define LOC_CKS_STPMSK  0x18u


/*******************************************************************************
  Constant: LOC_GEN_ACTIVE_MASK
 
*/
#define LOC_GEN_ACTIVE_MASK     (0x04u)

/*******************************************************************************
  Constant: LOC_STOP_MASKED_SET
 
*/

#define LOC_STOP_MASKED_SET     (0x01u)

/*******************************************************************************
  Constant: LOC_AWO_CKS_BASE_ADDR
  
  Register base adress for AWO clock selectors
*/
         
#define LOC_AWO_CKS_BASE_ADDR  0xfff82000u

/*******************************************************************************
  Constant: LOC_CPUSS_CKS_BASE_ADDR
  
   Register base adress for ISO clock selectors
*/
         
#define LOC_CPUSS_CKS_BASE_ADDR  0xfff8A000u

/*******************************************************************************
  Constant: LOC_ISO_BASEADDRESS
  
   Register base adress for ISO clock selectors
*/
         
#define LOC_ISO_CKS_BASE_ADDR  0xfff85000u

/*******************************************************************************
  Constant: loc_Base
  
  Register base adress for clock generators
*/
static const uint32_t loc_Base[R_DEV_CLK_GEN_NUM] =
{
    /*R_DEV_PLL0       */ 0xFFF89000u,
    /*R_DEV_PLL1       */ 0xFFF89100u,
    /*R_DEV_PLL2       */ 0xFFF850C0u,
    /*R_DEV_HS_RING    */ 0xFFF81000u,
    /*R_DEV_MOSC       */ 0xFFF81100u,
    /*R_DEV_SUB_OSC    */ 0xFFF81200u
};

/*******************************************************************************
  Constant: LOC_PROTCMD0
  
  Address of the AWO protection register
  
*/

#define LOC_PROTCMD0     (0xfff80000u)

/*******************************************************************************
  Constant: LOC_PROTCMD1
  
  Address of the ISO protection register
  
*/

#define LOC_PROTCMD1     (0xfff88000u)

/*******************************************************************************
  Constant: LOC_PROTCMDD1
  
  Address of the another ISO protection cluster
  
*/

#define LOC_PROTCMDD1     (0xfff87000u)

/*******************************************************************************
  Constant: LOC_PBUS_RATIO
  
  Address of the PBUS CLOCK ratio setting register
  Old header: 0xfff8f504 UM: 0xfff8f510u
*/
#define LOC_PBUS_RATIO    (0xfff8f510u)

/*******************************************************************************
  Constant: LOC_PROTCMDPWRGD
  
  Address of the PBUS CLOCK ratio protection register
  
*/
#define LOC_PROTCMDPWRGD    (0xfff8f580u)

/*******************************************************************************
   Constant: LOC_F
   Placeholder, marker
   If the divider slot 0 has this value in the
   allowed settings table, the divider value is not taken from any slot
   but literaly from the given register */
#define LOC_F           0xFEFEu

/*******************************************************************************
   Constant: LOC_D
   Placeholder, marker
   If the divider slot  has this value, the setting is not available
 */
#define LOC_N           0xFFFFu


static const uint32_t locBusRegAddr[R_DEV_CLK_BUS_LAST] =
{
/* R_DEV_CLK_PLL0     */    0xFFF89200u,
/* R_DEV_CLK_PLL1     */    0xFFF89300u,
/* R_DEV_CLK_PLL2     */    0xFFF85100u,
/* R_DEV_CLK_PLL0PIX  */    0xFFF85640u,
/* R_DEV_CLK_SDRB     */    0xFFF85140u,
/* R_DEV_CLK_ETNBP    */    0xFFF85280u,
/* R_DEV_CLK_MLBP     */    0xFFF852C0u,
/* R_DEV_CLK_RSCANP   */    0xFFF85240u,
/* R_DEV_CLK_XCC      */    0xFFF85180u,
/* R_DEV_CLK_ETNBXCC  */    0xFFF851C0u,
/* R_DEV_CLK_MLBXCC   */    0xFFF85200u,
};
/*******************************************************************************
  Constant: loc_ClkSelOffset
  
  Register base adress for clock selectors

  The clock selection for RLIN and RLIN30 uses the same register
  But the RLIN30 has an additional divider regiaster
 */


static const loc_CksBase_t loc_CkcsBaseAddr[R_DEV_CKS_LAST] =
{

  /*    Clock selector      sourceID or selector addr     , divider addr or 0                DivIdx  DivMsk*/
  /*  R_DEV_CKS_WDT0    */ {R_DEV_CKS_SRC_LRNG,             LOC_AWO_CKS_BASE_ADDR           , 0    , 0x7Eu},
  /*  R_DEV_CKS_AWOT    */ {LOC_AWO_CKS_BASE_ADDR + 0x100u, LOC_AWO_CKS_BASE_ADDR +   0x200u, 1    , 0x3Eu},
  /*  R_DEV_CKS_RTCA    */ {LOC_AWO_CKS_BASE_ADDR + 0x300u, LOC_AWO_CKS_BASE_ADDR +   0x400u, 2    , 0x3Fu},
  /*  R_DEV_CKS_FOUT    */ {LOC_AWO_CKS_BASE_ADDR + 0x700u, LOC_AWO_CKS_BASE_ADDR +   0x800u, 3    , 0xFFu},
  /*  R_DEV_CKS_CPU     */ {LOC_CPUSS_CKS_BASE_ADDR       , LOC_CPUSS_CKS_BASE_ADDR + 0x100u, 4    , 0x3Eu},
  /*  R_DEV_CKS_MLB     */ {LOC_ISO_CKS_BASE_ADDR + 0x300u,                                0, 0xFFu, 0x0} ,
  /*  R_DEV_CKS_SFMA    */ {LOC_ISO_CKS_BASE_ADDR + 0x340u, LOC_ISO_CKS_BASE_ADDR +   0x380u, 5    , 0x1Fu},
  /*  R_DEV_CKS_RSCAN   */ {R_DEV_CKS_SRC_PLLFIX          , LOC_ISO_CKS_BASE_ADDR +   0x780u, 6    , 0x0Fu},
  /*  R_DEV_CKS_RSCANXIN*/ {LOC_ISO_CKS_BASE_ADDR + 0x800u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_SSIF    */ {R_DEV_CKS_SRC_PLLFIX          , LOC_ISO_CKS_BASE_ADDR +   0x7C0u, 7    , 0x01u},
  /*  R_DEV_CKS_TAUB0   */ {LOC_ISO_CKS_BASE_ADDR + 0x540u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_TAUB2   */ {LOC_ISO_CKS_BASE_ADDR + 0x580u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_TAUJ    */ {LOC_ISO_CKS_BASE_ADDR + 0x5C0u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_OSTM    */ {LOC_ISO_CKS_BASE_ADDR + 0x600u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_LCBI    */ {LOC_ISO_CKS_BASE_ADDR + 0x4C0u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_ADCE    */ {R_DEV_CKS_SRC_CLKJIT          , LOC_ISO_CKS_BASE_ADDR +   0x480u, 8    , 0x06u},
  /*  R_DEV_CKS_ISM     */ {LOC_ISO_CKS_BASE_ADDR + 0x440u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_RLIN    */ {LOC_ISO_CKS_BASE_ADDR + 0x500u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_CLKJIT  */ {LOC_ISO_CKS_BASE_ADDR + 0x400u, LOC_ISO_CKS_BASE_ADDR +   0x3C0u, 9    , 0x02u},
  /*  R_DEV_CKS_CLKFIX  */ {LOC_ISO_CKS_BASE_ADDR + 0x740u, LOC_ISO_CKS_BASE_ADDR +   0x700u, 10   , 0x02u},
  /*  R_DEV_CKS_PLLFIX  */ {LOC_ISO_CKS_BASE_ADDR         ,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_PLL2IN  */ {LOC_ISO_CKS_BASE_ADDR + 0x080u, LOC_ISO_CKS_BASE_ADDR +   0x040u, 11   , 0x02u},
  /*  R_DEV_CKS_DOTCK0  */ {LOC_ISO_CKS_BASE_ADDR + 0x940u, LOC_ISO_CKS_BASE_ADDR +   0x9C0u, 12   , 0x02u},
  /*  R_DEV_CKS_DOTCK1  */ {LOC_ISO_CKS_BASE_ADDR + 0x980u, LOC_ISO_CKS_BASE_ADDR +   0xA00u, 13   , 0x02u},
  /*  R_DEV_CKS_VDCE0CK */ {LOC_ISO_CKS_BASE_ADDR + 0x880u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_VDCE1CK */ {LOC_ISO_CKS_BASE_ADDR + 0x8C0u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_RSDSCK  */ {LOC_ISO_CKS_BASE_ADDR + 0xA40u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_VIN0CK  */ {LOC_ISO_CKS_BASE_ADDR + 0x840u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_MIPIPLL */ {LOC_ISO_CKS_BASE_ADDR + 0x680u,                                0, 0xFFu, 0x0},
  /*  R_DEV_CKS_MIPIPXL */ {R_DEV_CKS_SRC_MIPIPLL         , LOC_ISO_CKS_BASE_ADDR +   0x6C0u, 14   , 0x0Fu}
 };
 
#define LOC_DIS        (R_DEV_CKS_SRC_DISABLED)
#define LOC_LRNG       (R_DEV_CKS_SRC_LRNG)
#define LOC_HRING      (R_DEV_CKS_SRC_HRING)
#define LOC_EM         (R_DEV_CKS_SRC_ERING)
#define LOC_MOSC       (R_DEV_CKS_SRC_MOSC)
#define LOC_SOSC       (R_DEV_CKS_SRC_SOSC)
#define LOC_PLL0       (R_DEV_CKS_SRC_PLL0)
#define LOC_PLL0_2     (R_DEV_CKS_SRC_PLL0_2)
#define LOC_PLL0_4     (R_DEV_CKS_SRC_PLL0_4)
#define LOC_PLL1       (R_DEV_CKS_SRC_PLL1)
#define LOC_PLL1_4     (R_DEV_CKS_SRC_PLL1_4)
#define LOC_PLL2       (R_DEV_CKS_SRC_PLL2)
#define LOC_PLL2_4     (R_DEV_CKS_SRC_PLL2_4)
#define LOC_ISOCPU_4   (R_DEV_CKS_SRC_ISOCPU_4)
#define LOC_CLKFIX     (R_DEV_CKS_SRC_CLKFIX)
#define LOC_CLKFIX_8   (R_DEV_CKS_SRC_CLKFIX_8)
#define LOC_CLKJIT     (R_DEV_CKS_SRC_CLKJIT)
#define LOC_CLKJIT_2   (R_DEV_CKS_SRC_CLKJIT_2)
#define LOC_CLKJIT_4   (R_DEV_CKS_SRC_CLKJIT_4)
#define LOC_CLKJIT_8   (R_DEV_CKS_SRC_CLKJIT_8)
#define LOC_PLLFIX_10  (R_DEV_CKS_SRC_PLLFIX_10)
#define LOC_PLLFIX_20  (R_DEV_CKS_SRC_PLLFIX_20)
#define LOC_PLL0PIX    (R_DEV_CKS_SRC_PLL0PIX)
#define LOC_PLLFIX     (R_DEV_CKS_SRC_PLLFIX)
#define LOC_PLL0PIX_3  (R_DEV_CKS_SRC_PLL0PIX_3)
#define LOC_PLLFIX_3   (R_DEV_CKS_SRC_PLLFIX_3)
#define LOC_DOTC0      (R_DEV_CKS_SRC_DOTCK0)
#define LOC_DOTC0_4    (R_DEV_CKS_SRC_DOTCK0_4)
#define LOC_DOTC1      (R_DEV_CKS_SRC_DOTCK1)
#define LOC_DOTC1_4    (R_DEV_CKS_SRC_DOTCK1_4)
#define LOC_V1PIX      (R_DEV_CKS_SRC_VI1PIX)
#define LOC_V0PIX      (R_DEV_CKS_SRC_VI0PIX)
#define LOC_VI0CLK     (R_DEV_CKS_SRC_VI0CLK)
#define LOC_MIPIPIX    (R_DEV_CKS_SRC_MIPIPXL)
#define LOC_MIPIPLL    (R_DEV_CKS_SRC_MIPIPLL)
#define LOC_XX         (R_DEV_CKS_SRC_LAST)
#define LOC_DIV        (R_DEV_CKS_SRC_DIV)

static const r_dev_CksSrc_t loc_ClockTree[R_DEV_CKS_LAST][8] =
{
/* there are max 8 slots,
   LOC_XX means forbidden value/selection */
   
   /* Clock selector domain    cks source slot
                              0        1              2             3           4             5           6             7   */
   /* R_DEV_CKS_WDT0    */{LOC_XX,     LOC_LRNG,      LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_AWOT    */{LOC_DIS,    LOC_HRING,     LOC_MOSC,     LOC_LRNG,   LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_RTCA    */{LOC_DIS,    LOC_SOSC,      LOC_MOSC,     LOC_LRNG,   LOC_HRING,    LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_FOUT    */{LOC_DIS,    LOC_MOSC,      LOC_HRING,    LOC_LRNG,   LOC_SOSC,     LOC_PLL0_4, LOC_PLL1_4,   LOC_PLL2_4},
   /* R_DEV_CKS_CPU     */{LOC_XX,     LOC_EM,        LOC_XX,       LOC_PLL0,   LOC_PLL1,     LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_MLB     */{LOC_DIS,    LOC_PLL0_2,    LOC_PLL0_4,   LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_SFMA    */{LOC_XX,     LOC_PLL0,      LOC_PLL1,     LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_RSCAN   */{LOC_PLLFIX, LOC_XX,        LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_RSCANXIN*/{LOC_DIS,    LOC_MOSC,      LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_SSIF    */{LOC_PLLFIX, LOC_XX,        LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_TAUB0   */{LOC_DIS,    LOC_CLKFIX,    LOC_CLKFIX_8, LOC_CLKJIT, LOC_CLKJIT_8, LOC_MOSC,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_TAUB2   */{LOC_DIS,    LOC_CLKFIX,    LOC_CLKFIX_8, LOC_CLKJIT, LOC_CLKJIT_8, LOC_MOSC,   LOC_XX,       LOC_XX },
   /* R_DEV_CKS_TAUJ    */{LOC_DIS,    LOC_CLKFIX,    LOC_CLKFIX_8, LOC_CLKJIT, LOC_CLKJIT_8, LOC_MOSC,   LOC_ISOCPU_4, LOC_XX },
   /* R_DEV_CKS_OSTM    */{LOC_DIS,    LOC_CLKJIT,    LOC_ISOCPU_4, LOC_MOSC,   LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_LCBI    */{LOC_DIS,    LOC_CLKJIT,    LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_ADCE    */{LOC_XX,     LOC_CLKJIT_2,  LOC_CLKJIT_4, LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_ISM     */{LOC_DIS,    LOC_CLKJIT,    LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_RLIN    */{LOC_XX,     LOC_PLLFIX_10, LOC_CLKJIT,   LOC_PLLFIX_20,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_CLKJIT  */{LOC_XX,     LOC_DIV,       LOC_HRING,    LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_CLKFIX  */{LOC_XX,     LOC_DIV,       LOC_HRING,    LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_PLLFIX  */{LOC_XX,     LOC_PLL1,      LOC_PLL2,     LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_PLL2IN  */{LOC_XX,     LOC_DIV,       LOC_MOSC,     LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_DOTCK0  */{LOC_DIS,    LOC_PLL0PIX,   LOC_PLL1,     LOC_PLL2,   LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_DOTCK1  */{LOC_DIS,    LOC_PLL0PIX,   LOC_PLLFIX,   LOC_PLL2,   LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_VDCE0CK */{LOC_DIS,    LOC_DOTC0,     LOC_DOTC0_4,  LOC_V1PIX,  LOC_V0PIX,    LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_VDCE1CK */{LOC_DIS,    LOC_DOTC1,     LOC_DOTC1_4,  LOC_V1PIX,  LOC_V0PIX,    LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_RSDSCK  */{LOC_XX,     LOC_DOTC1,     LOC_DOTC0,    LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_VIN0PIX */{LOC_XX,     LOC_VI0CLK,    LOC_MIPIPIX,  LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_MIPIPLL */{LOC_XX,     LOC_PLL0PIX_3, LOC_PLLFIX_3, LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX },
   /* R_DEV_CKS_MIPIPXL */{LOC_XX,     LOC_MIPIPLL,   LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX,     LOC_XX,       LOC_XX }
};


/*******************************************************************************
  Constant: loc_ClkDivSel
  
  For each divider, the register value has different meaning.
  Sometimes it is a divider selection, sometimes it is the divider value itself
  
  Explanation of settings:
  LOC_N     : Setting not valid  (the corresponding value is not allowed for the given register)
  LOC_F     : Full register is used (no translation of values) as divider for the selected source
              The source selection is determined by the value stored in  loc_ClockTree
              That means if for the sourceID the div value in the corresponding slot is LOC_F,
              the register value of the divider register is used as divider for the corresponding
              clock source
  any number: Divider for the selected source clock
  
  Following selectors have dividers:
  R_DEV_CKS_WDT0    7E
  R_DEV_CKS_AWOT    3E
  R_DEV_CKS_RTC     3F
  R_DEV_CKS_FOUT    FF
  R_DEV_CKS_CPU     3E
  R_DEV_CKS_SFMA    1F
  R_DEV_CKS_RSCAN   0F
  R_DEV_CKS_SSIF    01
  R_DEV_CKS_ADCE    06
  R_DEV_CKS_CLKJIT  02
  R_DEV_CKS_CLKFIX  02
  R_DEV_CKS_PLL2IN  02
  R_DEV_CKS_DOTCK0  02
  R_DEV_CKS_DOTCK1  02
  R_DEV_CKS_MIPIPXL 0F
*/

static const uint16_t loc_ClkSelDiv[15][8] =
{
   /*                       (0xFF - setting prohibited)   */
   /*          div slot          0,     1,     2,     3,     4,     5,     6 ,    7*/
   /* R_DEV_CKS_WDT0    */  {LOC_N,  128u,    1u,  256u,  512u, 1024u, 2048u, LOC_N},
   /* R_DEV_CKS_AWOT    */  {LOC_N,    1u,    2u,    4u,    8u,   16u, LOC_N, LOC_N},
   /* R_DEV_CKS_RTC     */  {    0,    1u,    2u,    4u,    8u,   16u, LOC_N, LOC_N},
   /* R_DEV_CKS_FOUT    */  {LOC_F, LOC_F, LOC_F, LOC_F, LOC_F, LOC_F, LOC_F, LOC_F},
   /* R_DEV_CKS_CPU     */  {LOC_N,    1u,    2u,    4u,    8u,    3u, LOC_N, LOC_N},
   /* R_DEV_CKS_SFMA    */  {    0,    2u,    3u,    4u,    6u, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_RSCAN   */  {    0,   30u,   40u,   60u,   12u,   24u, LOC_N, LOC_N},
   /* R_DEV_CKS_SSIF    */  {LOC_F, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_ADCE    */  {LOC_N,    2u,    4u, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_CLKJIT  */  {LOC_N, LOC_F, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_CLKFIX  */  {LOC_N, LOC_F, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_PLL2IN  */  {LOC_N, LOC_F, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_DOTCK0  */  {LOC_N, LOC_F, LOC_F, LOC_F, LOC_N, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_DOTCK1  */  {LOC_N, LOC_F, LOC_F, LOC_F, LOC_N, LOC_N, LOC_N, LOC_N},
   /* R_DEV_CKS_MIPIPXL */  {    0,    3u,    6u,   12u, LOC_N, LOC_N, LOC_N, LOC_N},
   
};

static const uint16_t loc_SscgDiv[32] =
{
    10  ,
    12  ,
    18  ,
    20  ,
    22  ,
    26  ,
    28  ,
    30  ,
    34  ,
    38  ,
    40  ,
    44  ,
    50  ,
    56  ,
    58  ,
    60  ,
    62  ,
    66  ,
    72  ,
    76  ,
    80  ,
    84  ,
    86  ,
    100 ,
    120 ,
    126 ,
    134 ,
    150 ,
    166 ,
    200 ,
    250 ,
    300
};

static const uint8_t loc_SscgPercent[8] =
{
    1  ,
    2  ,
    3  ,
    4  ,
    5  ,
    6  ,
    8  ,
    10 ,
};

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: LocPbusRatio
  Store the PBUS clock ration (it's configurable for D1M1 only)

*/
volatile static uint32_t LocPbusRatio = 2;

/*
   loc_ClkGenCfg - local Clock generator configuration
*/
static r_dev_ClkGenConfig_t loc_ClkGenCfg;

/*******************************************************************************
  Variable: loc_Vdce0ViFreq
  Store the frequency (in Hz) of the external input VDCE0_VI_CLK
*/
static uint32_t loc_Vdce0ViFreq = 0;

/*******************************************************************************
  Variable: loc_Vdce1ViFreq
  Store the frequency (in Hz) of the external input VDCE1_VI_CLK
*/
static uint32_t loc_Vdce1ViFreq = 0;


/*******************************************************************************
  Section: Local functions

*/
static void loc_WritePllCfg (r_dev_ClkGen_t Pll, loc_PllCfgReg_t * PllCfgReg);
static r_Error_t loc_CalcFreqPll0(uint32_t FrqIn, loc_PllCfgReg_t * PllCfgReg, uint32_t *FrqOut);
static r_Error_t loc_CalcFreqPll1(uint32_t FrqIn, loc_PllCfgReg_t * PllCfgReg, uint32_t *FrqOut);
static r_Error_t loc_CalcFreqPll2(uint32_t FrqIn, loc_PllCfgReg_t * PllCfgReg, uint32_t *FrqOut);
static uint32_t loc_SrcFrequencyHz(r_dev_CksSrc_t ClkSrc);
static r_Error_t loc_CalcSscgPll0(uint32_t FrqIn, loc_PllCfgReg_t * PllCfgReg, const r_dev_PllCfg_t * PllCfg);
static void loc_PllCfgStore(uint32_t FrqIn, r_dev_ClkGen_t Pll, const r_dev_PllCfg_t *PllCfg);

/*******************************************************************************
  Function: loc_PllCfgStore

    this is a copy of the PllConfig function, used to store the settings after low init
    whre all the structures were deleted
*/

static void loc_PllCfgStore(uint32_t FrqIn, r_dev_ClkGen_t Pll, const r_dev_PllCfg_t *PllCfg)
{
    uint32_t          freq_out  = PllCfg->FrequencyHz/1000000u;
    r_Error_t         ret_val   = R_NG;
    uint32_t          regval    = 0;
    loc_PllCfgReg_t   pllcfg_reg;
    /* This one is not needed here */
    Pll = Pll;
    
    /* Clear Config */
    pllcfg_reg.NI       = 0;
    pllcfg_reg.P        = 0;
    pllcfg_reg.M        = 0;
    pllcfg_reg.NF       = 0;
    pllcfg_reg.SSMode   = 0;
    pllcfg_reg.SSPerc   = 0;
    pllcfg_reg.SMFreq   = 0;
    
    /* VC = freq = fin * Nr/Pr */
    /* Set/Check the frequency */
    switch (Pll)
    {
    case R_DEV_PLL0:
        ret_val =  loc_CalcFreqPll0(FrqIn/1000000, &pllcfg_reg, &freq_out);
        if ((R_ERR_OK == ret_val) && (R_DEV_PLL_SSCG == PllCfg->Mode))
        {
            /*The input frequency is provided in KHz here */
            ret_val = loc_CalcSscgPll0(FrqIn/1000, &pllcfg_reg, PllCfg);
        }
        break;
    case R_DEV_PLL1:
        ret_val =  loc_CalcFreqPll1(FrqIn/1000000, &pllcfg_reg, &freq_out);
        break;
    case R_DEV_PLL2 :
        /* First, get the input frquency  */
        regval  =  R_DEV_ClkFrequencyHz(R_DEV_CKS_PLL2IN);
        ret_val =  loc_CalcFreqPll2(regval/1000000, &pllcfg_reg, &freq_out);
        pllcfg_reg.FVV       = 0; /* Init value */
        pllcfg_reg.NF        = 1;
        break;
    default:
        ret_val = R_PARAMETER_RANGE;
        break;
    }
    /* If everything went smooth, store the values */
    if (R_ERR_OK == ret_val)
    {
        /* Store the config */
        loc_ClkGenCfg.Pll[Pll].Mode         = PllCfg->Mode         ;
        loc_ClkGenCfg.Pll[Pll].DithMode     = PllCfg->DithMode     ;
        loc_ClkGenCfg.Pll[Pll].DithRange    = PllCfg->DithRange    ;
        loc_ClkGenCfg.Pll[Pll].ModFreq      = PllCfg->ModFreq      ;
        loc_ClkGenCfg.Pll[Pll].FrequencyHz  = freq_out * 1000000u  ;
        loc_ClkGenCfg.Pll[Pll].StabiTimeNs  = PllCfg->StabiTimeNs  ;
        loc_ClkGenCfg.Pll[Pll].StpReqMsk    = PllCfg->StpReqMsk    ;
    }
}

/*******************************************************************************
  Function: loc_WritePLLCfg
  
  Write the PLL configuration to the PLL config register
  
  Parameters:
  
  Returns:
  void

    r_dev_PllMode_t     Mode
    r_dev_PllDithMode_t DithMode
    uint8_t             DithRange
    uint32_t            ModFreq
    uint32_t            FrequencyHz
    uint32_t            StabiTimeNs
    uint8_t             StpReqMsk

*/
static void loc_WritePllCfg (r_dev_ClkGen_t Pll, loc_PllCfgReg_t * PllCfgReg)
{

    uint32_t addr    = 0;
    uint32_t reg_val = 0;
    /* Allign the inforemation */
    reg_val    |= ((uint32_t) PllCfgReg->NI     << 0u);
    reg_val    |= ((uint32_t) PllCfgReg->P      << 8u);
    reg_val    |= ((uint32_t) PllCfgReg->M      << 11u);
    reg_val    |= ((uint32_t) PllCfgReg->SSMode << 13u);
    reg_val    |= ((uint32_t) PllCfgReg->NF     << 16u);
    reg_val    |= ((uint32_t) PllCfgReg->SSPerc << 20u);
    reg_val    |= ((uint32_t) PllCfgReg->SMFreq << 24u);
    reg_val    |= ((uint32_t) PllCfgReg->FVV    << 29u);

    /* Write the value */
    addr    = loc_Base[Pll] + LOC_GEN_C;
    R_DEV_WRITE_REG(32, addr, reg_val);
}


/*******************************************************************************
  Function: loc_CalcSscgPll0
  Calculate the right paramaters to set the SSCG function for PLL0
    
    Parameters:
    FrqIn         - input frequency in KHz
    PllCfgReg     - pointer to PLLC register structure, see <loc_PllCfgReg_t>
    PllCfg        - pointer to PLLC config structure, see <r_dev_PllCfg_t>
    
    Returns:
       == R_ERR_OK - found apropriate settings
       != R_ERR_OK - no matching parameters found
    
    
*/
static r_Error_t loc_CalcSscgPll0(uint32_t FrqIn, loc_PllCfgReg_t *PllCfgReg, const r_dev_PllCfg_t *PllCfg)
{

    uint32_t  vc_in  = 0;
    uint16_t  d_r    = 0;
    uint32_t  i      = 0;
    uint16_t  perc   = 0;
    r_Error_t retval = R_ERR_OK;

    if (R_DEV_PLL_DITH_CENTER == PllCfg->DithMode)
    {
        PllCfgReg->SSMode    = 3u;
        if ((0 < PllCfg->DithRange) && (PllCfg->DithRange < 6))
        {
            perc = PllCfg->DithRange * 2u;
        }
        else
        {
            return R_PARAMETER_INCORRECT;
        }
    }
    else
    {
        PllCfgReg->SSMode    = 2u;
        if ((0 < PllCfg->DithRange) && (PllCfg->DithRange < 11))
        {
            perc = PllCfg->DithRange;
        }
        else
        {
            return R_PARAMETER_INCORRECT;
        }
    }
    d_r = 0;
    if(PllCfgReg->M != 0)
    {
        vc_in = FrqIn / PllCfgReg->M;
    }
    else
    {
        vc_in = FrqIn;
    }
    /* F_mod = vc_in / (4* Dr)
       Dr    = vc_in / (4 * f_mod)
    */
    if ((PllCfg->ModFreq > 19u) && (PllCfg->ModFreq < 101u))
     {
        d_r = vc_in  / (4u * PllCfg->ModFreq);
        
    }
    else
    {
        return R_PARAMETER_INCORRECT;
    }
    for (i = 0; i < 32; i ++)
    {
        if (loc_SscgDiv[i] >= d_r)
        {
            break;
        }
    }
    /* check if the search was successfull */
    if (i < 32)
    {
        /* Yes, we found some */
        PllCfgReg->SMFreq = i;
    }
    else
    {
        return R_PARAMETER_INCORRECT;
    }
    for (i = 0; i < 8; i ++)
    {
        if (loc_SscgPercent[i] >= perc)
        {
            break;
        }
    }
    if (i < 8)
    {
        /* Yes, we found some */
        PllCfgReg->SSPerc    = i;
    }
    else
    {
        return R_PARAMETER_INCORRECT;
    }
    return retval;
}


/*******************************************************************************
  Function: loc_CalcFreqPlln
    Calculate the right paramaters to set the desired PLL Frequency
    
    Parameters:
    FrqIn         - input frequency in MHz
    PllCfgReg     - pointer to PLLC register structure, see <loc_PllCfgReg_t>
    *FreqOut      - desired output frequency in Mhz
    
    Returns:
       == R_ERR_OK - found apropriate settings
       != R_ERR_OK - no matching parameters found
    
    
*/
static r_Error_t loc_CalcFreqPll0(uint32_t FrqIn, loc_PllCfgReg_t * PllCfgReg, uint32_t *FrqOut)
{

    /* Caculate the right values */
    /*F = (Fx * Nr) / (Mr * Pr) :
      Mr = PLLkC.M[1:0] + 1
      Pr = 1, 2, 3, 4, 8 16
      P[2:0]  = log_2(Pr); 2^(P[]) = Pr
      Nr = PLLkC.NI + 1 + PLLkC.NF.
      We set PLLkC.NF to 0
      Nr = PLLkC.NI + 1
      PLL0:
      NI[6:0] = Nr - 1
      PLL1:
      NI[6:0] = Nr/2 - 1
    */
    uint32_t  p_r    = 0;
    uint32_t  m_r    = 0;
    uint32_t  n_r    = 0;
    uint32_t  freq   = 0;
    uint32_t  vc_out = 0;
    uint32_t  i      = 1;
    r_Error_t retval = R_NG;

    PllCfgReg->NI    = 0;
    PllCfgReg->P     = 0;
    PllCfgReg->M     = 0;

    freq = *FrqOut;

    if (freq > 480u)
    {
        /* the desired frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }
    else if (freq > 320u)
    {
        p_r = 1u;
        PllCfgReg->P = 0;
    }
    else if (freq > 160u)
    {
        p_r = 2;
        PllCfgReg->P = 1u;
    }
    else if (freq > 80u)
    {
        p_r = 4u;
        PllCfgReg->P = 2u;
    }
    else if (freq > 40u)
    {
        p_r = 8;
        PllCfgReg->P = 3u;
    }
    else if (freq > 20u)
    {
        p_r = 16u;
        PllCfgReg->P = 4u;
    }
    else
    {
        /* the desired frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }

    if (24u == FrqIn)
    {
       m_r = 3u;
    }
    else if (16u <= FrqIn)
    {
       m_r = 2u;
    }
    else if  (8u <= FrqIn)
    {
       m_r = 1u;
    }
    else
    {
        /* the input frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }
    
    n_r = (freq * m_r * p_r)/FrqIn;
    /* we omit the check of Nr is even */
    /* as all parameters are even and we expect the frequency input to be even too */
    /* nr will be also even */
    /* do we have a valid parameter */
    if ((n_r > 11) && (n_r < 81) )
    {
        PllCfgReg->NI = (uint8_t) n_r - 1u ;
        PllCfgReg->M  = (uint8_t) m_r - 1u ;
        *FrqOut       = (FrqIn * n_r)/(m_r * p_r);
        /* Calculate FVV setting */
        vc_out        = *FrqOut * p_r;
        while ((320u + (i * 40u)) <  vc_out)
        {
            i++;
        }
        if (i < 9)
        {
             PllCfgReg->FVV = i - 1u;
        }
        else
        {
            PllCfgReg->FVV  = 3; /* init value */
        }
        retval        = R_ERR_OK;
    }
    else
    {
        *FrqOut         = 0;
        PllCfgReg->NI   = 0;
        PllCfgReg->M    = 0;
        PllCfgReg->P    = 0;
        PllCfgReg->FVV  = 3; /* Init value */
        retval        = R_PARAMETER_INCORRECT;
    }
    return retval;
}

static r_Error_t loc_CalcFreqPll1(uint32_t FrqIn, loc_PllCfgReg_t * PllCfgReg, uint32_t *FrqOut)
{
    /* Caculate the right values */
    /*F = (Fx * Nr) / (Mr * Pr) :
      Mr = PLLkC.M[1:0] + 1
      Pr = 1, 2, 3, 4, 8 16
      P[2:0]  = log_2(Pr); 2^(P[]) = Pr
      Nr = PLLkC.NI + 1 + PLLkC.NF.
      We set PLLkC.NF to 0
      Nr = PLLkC.NI + 1
      PLL0:
      NI[6:0] = Nr - 1
      PLL1:
      NI[6:0] = Nr/2 - 1
    */
    uint32_t  p_r    = 0;
    uint32_t  m_r    = 0;
    uint32_t  n_r    = 0;
    uint32_t  freq   = 0;
    uint32_t  vc_out = 0;
    uint32_t  i      = 1;
    
    r_Error_t retval = R_NG;
    r_dev_Device_t device;

    PllCfgReg->NI    = 0;
    PllCfgReg->P     = 0;
    PllCfgReg->M     = 0;

    freq = *FrqOut;
    
    /* These functions can cover all devices of the d1l/d1m family including the D1M2(H). */
    /* The D1M2(H) has a different HW-configuration for PLL1 which is reflected by the values shown here. */
    /* For this function to work with all lower devices, their FrqOut has to be multiplied by 2 */
    device = R_DEV_GetDev();
    if ((device >= R_DEV_R7F701400) && (device <= R_DEV_R7F701407))
    {
        freq = freq * 2;
    }
    
    if (freq > 960u)
    {
        /* the desired frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }
    else if (freq > 533u)
    {
        p_r = 1u;
        PllCfgReg->P = 0;
        
    }
    else if (freq > 266u)
    {
        p_r = 2;
        PllCfgReg->P = 1u;
    }
    else if (freq > 160u)
    {
        p_r = 4u;
        PllCfgReg->P = 2u;
    }
    else if (freq > 67u)
    {
        p_r = 8;
        PllCfgReg->P = 3u;
    }
    else if (freq > 40u)
    {
        p_r = 16u;
        PllCfgReg->P = 4u;
    }
    else
    {
        /* the desired frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }

    if (24u == FrqIn)
    {
       m_r = 3u;
    }
    else if (16u <= FrqIn)
    {
       m_r = 2u;
    }
    else if  (8u <= FrqIn)
    {
       m_r = 1u;
    }
    else
    {
        /* the input frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }
    
    n_r = (freq * m_r * p_r)/FrqIn;

    /* do we have a valid parameter */
    if ((n_r > 23) && (n_r < 161) )
    {
        *FrqOut       = (FrqIn * n_r)/(m_r * p_r);
        PllCfgReg->NI = (uint8_t) (n_r/2) - 1u ;
        PllCfgReg->M  = (uint8_t) m_r     - 1u ;
        /* Calculate FVV setting */
        vc_out        = *FrqOut * p_r;
        i = 1;
        while ((640u + (i * 80u)) <  vc_out)
        {
            i++;
        }
        if (i < 8)
        {
             PllCfgReg->FVV = i -1u;
        }
        else
        {
            PllCfgReg->FVV  = 3; /* init value */
        }
        /* D1Lx and D1M1(H): See comment at begin of function. */
        /* => Divide back the factor of 2 added at the begin of this function. */
        /* D1M2(H): The PLL1 is followed by a DIVIDE 2, so in the end, */
        /* we ALWAYS divide by two. */
        *FrqOut /= 2;
        
        retval        = R_ERR_OK;
    }
    else
    {
        *FrqOut       = 0;
        PllCfgReg->NI = 0;
        PllCfgReg->M  = 0;
        PllCfgReg->P  = 0;
        retval        = R_PARAMETER_INCORRECT;
    }
    return retval;
}

static r_Error_t loc_CalcFreqPll2(uint32_t FrqIn, loc_PllCfgReg_t * PllCfgReg, uint32_t *FrqOut)
{
    /* Caculate the right values */
    /*F = (Fx * Nr) / (Mr * Pr) :
      Mr = PLLkC.M[1:0] + 1
      Pr = 1, 2, 3, 4, 8 16
      P[2:0]  = log_2(Pr); 2^(P[]) = Pr
      Nr = PLLkC.NI + 1 + PLLkC.NF.
      We set PLLkC.NF to 0
      Nr = PLLkC.NI + 1
      PLL0:
      NI[6:0] = Nr - 1
      PLL1:
      NI[6:0] = Nr/2 - 1
    */
    uint32_t  p_r    = 0;
    uint32_t  m_r    = 0;
    uint32_t  n_r    = 0;
    uint32_t  freq   = 0;
    r_Error_t retval = R_NG;

    PllCfgReg->NI    = 0;
    PllCfgReg->P     = 0;
    PllCfgReg->M     = 0;

    freq = *FrqOut;

    if (freq > 480u)
    {
        /* the desired frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }
    else if (freq > 240u)
    {
        p_r = 1u;
        PllCfgReg->P = 0;
    }
    else if (freq > 120u)
    {
        p_r = 2;
        PllCfgReg->P = 1u;
    }
    else if (freq > 60u)
    {
        p_r = 4u;
        PllCfgReg->P = 2u;
    }
    else if (freq > 40u)
    {
        p_r = 6;
        PllCfgReg->P = 3u;
    }
    else if (freq > 30u)
    {
        p_r = 8u;
        PllCfgReg->P = 4u;
    }
    else if (freq > 20u)
    {
        p_r = 16u;
        PllCfgReg->P = 4u;
    }
    else
    {
        /* the desired frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }

    if (24u == FrqIn)
    {
       m_r = 3u;
    }
    else if (16u <= FrqIn)
    {
       m_r = 2u;
    }
    else if  (8u <= FrqIn)
    {
       m_r = 1u;
    }
    else
    {
        /* the input frequency is not valid */
        return R_PARAMETER_INCORRECT;
    }
    
    n_r = (freq * m_r * p_r)/FrqIn;

    /* do we have a valid parameter */
    if ((n_r > 19) && (n_r < 61) )
    {
        *FrqOut       = (FrqIn * n_r)/(m_r * p_r);
        PllCfgReg->NI = (uint8_t) n_r - 1u;
        PllCfgReg->M  = (uint8_t) m_r - 1u;

        retval        = R_ERR_OK;
    }
    else
    {
        *FrqOut       = 0;
        PllCfgReg->NI = 0;
        PllCfgReg->M  = 0;
        PllCfgReg->P  = 0;
        retval        = R_PARAMETER_INCORRECT;
    }
    return retval;
}


/*******************************************************************************
  Function: loc_SrcFrequencyHz

    Get the frequency setting for the given source
*/


static uint32_t loc_SrcFrequencyHz(r_dev_CksSrc_t ClkSrc)
{
    uint32_t freq    = 0;
    switch (ClkSrc)
    {
        case R_DEV_CKS_SRC_DISABLED :
            freq = 0;
            break;
        case R_DEV_CKS_SRC_LRNG     :
            freq = LOC_LOWS_FRQ_HZ;
            break;
        case R_DEV_CKS_SRC_HRING    :
            freq = LOC_HIGHS_FRQ_HZ;
            break;
        case R_DEV_CKS_SRC_ERING    :
           /* select the right OSC */
            if (0 != R_DEV_ClkGenActive(R_DEV_HS_RING))
            {
                freq = LOC_HIGHS_FRQ_HZ;
            }
            else
            {
                 freq = LOC_LOWS_FRQ_HZ;
            }
            break;
        case R_DEV_CKS_SRC_MOSC     :
            if (0 != R_DEV_ClkGenActive(R_DEV_MOSC))
            {
                freq = R_DEV_MOSC_FRQ_HZ;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_SOSC     :
            if (0 != R_DEV_ClkGenActive(R_DEV_SOSC))
            {
                freq = loc_ClkGenCfg.SubOsc.FrequencyHz;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_PLL0     :
            if (0 != R_DEV_ClkGenActive(R_DEV_PLL0))
            {
                freq = loc_ClkGenCfg.Pll[0].FrequencyHz;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_PLL0_2   :
            if (0 != R_DEV_ClkGenActive(R_DEV_PLL0))
            {
                freq = loc_ClkGenCfg.Pll[0].FrequencyHz / 2;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_PLL0_4   :
            if (0 != R_DEV_ClkGenActive(R_DEV_PLL0))
            {
                freq = loc_ClkGenCfg.Pll[0].FrequencyHz / 4;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_PLL1     :
            if (0 != R_DEV_ClkGenActive(R_DEV_PLL1))
            {
                freq = loc_ClkGenCfg.Pll[1].FrequencyHz / 1;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_PLL1_4   :
            if (0 != R_DEV_ClkGenActive(R_DEV_PLL1))
            {
                freq = loc_ClkGenCfg.Pll[1].FrequencyHz / 4;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_PLL2     :
            if (0 != R_DEV_ClkGenActive(R_DEV_PLL2))
            {
                freq = loc_ClkGenCfg.Pll[2].FrequencyHz;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_PLL2_4   :
            if (0 != R_DEV_ClkGenActive(R_DEV_PLL2))
            {
                freq = loc_ClkGenCfg.Pll[2].FrequencyHz / 4;
            }
            else
            {
                freq = 0;
            }
            break;
        case R_DEV_CKS_SRC_ISOCPU_4 :
            freq = R_DEV_BusFrequencyHz(R_DEV_CLK_CPU) / 4;
            break;
        case R_DEV_CKS_SRC_CLKFIX   :
            freq = R_DEV_BusFrequencyHz(R_DEV_CLK_FIX);
            break;
        case R_DEV_CKS_SRC_CLKFIX_8 :
            freq = R_DEV_BusFrequencyHz(R_DEV_CLK_FIX) / 8;
            break;
        case R_DEV_CKS_SRC_CLKJIT   :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CLKJIT);
            break;
        case R_DEV_CKS_SRC_CLKJIT_2 :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CLKJIT) / 2;
            break;
        case R_DEV_CKS_SRC_CLKJIT_4 :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CLKJIT) / 4;
            break;
        case R_DEV_CKS_SRC_CLKJIT_8 :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CLKJIT) / 8;
            break;
        case R_DEV_CKS_SRC_PLLFIX   :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_PLLFIX);
            break;
        case R_DEV_CKS_SRC_PLLFIX_3 :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_PLLFIX) / 3;
            break;
        case R_DEV_CKS_SRC_PLLFIX_10:
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_PLLFIX) / 10;
            break;
        case R_DEV_CKS_SRC_PLLFIX_20:
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_PLLFIX) / 20;
            break;
        case R_DEV_CKS_SRC_PLL0PIX:
            freq = R_DEV_BusFrequencyHz(R_DEV_CLK_PLL0PIX);
            break;
        case R_DEV_CKS_SRC_PLL0PIX_3:
            freq = R_DEV_BusFrequencyHz(R_DEV_CLK_PLL0PIX) / 3;
            break;
        case R_DEV_CKS_SRC_DOTCK0   :
            freq  = R_DEV_ClkFrequencyHz(R_DEV_CKS_DOTCK0);
            break;
        case R_DEV_CKS_SRC_DOTCK0_4 :
            freq  = R_DEV_ClkFrequencyHz(R_DEV_CKS_DOTCK0) / 4;
            break;
        case R_DEV_CKS_SRC_DOTCK1   :
            freq  = R_DEV_ClkFrequencyHz(R_DEV_CKS_DOTCK1);
            break;
        case R_DEV_CKS_SRC_DOTCK1_4 :
            freq  = R_DEV_ClkFrequencyHz(R_DEV_CKS_DOTCK1) / 4;
            break;
        case R_DEV_CKS_SRC_VI0CLK   :
            freq  = loc_Vdce0ViFreq;
            break;
        case R_DEV_CKS_SRC_VI0PIX   :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_VI0CLK);
            break;
        case R_DEV_CKS_SRC_VI1PIX   :
            freq  = loc_Vdce1ViFreq;
            break;
        case R_DEV_CKS_SRC_MIPIPXL  :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_MIPIPXL);
            break;
        case R_DEV_CKS_SRC_MIPIPLL  :
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_MIPIPLL);
            break;
        default:
            R_DEV_ERROR((uint32_t)ClkSrc, R_DEV_ERR_IDX, "Wrong ClockID!");
        break;
    }
    return freq;
}

/*******************************************************************************
  Section: Global API Functions
*/


/*******************************************************************************
  Function: R_DEV_ClkGenActive
    
    
    see: <r_dev_api.h> for details
*/

uint8_t R_DEV_ClkGenActive(r_dev_ClkGen_t ClkGen)
{
    uint32_t addr    = 0;
    uint8_t  ret_val = 0;

    if (ClkGen < R_DEV_CLK_GEN_NUM)
    {
        addr = loc_Base[ClkGen] + LOC_GEN_S;
        if (LOC_GEN_ACTIVE_MASK == (R_DEV_READ_REG(32, addr) & LOC_GEN_ACTIVE_MASK))
        {
            ret_val = 1;
        }
    }
    else
    {
        R_DEV_ERROR((uint32_t)ClkGen, R_DEV_ERR_IDX, "Wrong clock generator selected!");
        
    }
    return ret_val;
}


/*******************************************************************************
  Function: R_DEV_ClkIdSet

    see: <r_dev_api.h> for details
*/


r_Error_t R_DEV_ClkIdSet(const r_dev_ClkSelConfig_t * ClkSelCfg)
{
    uint32_t  sel_addr = 0;
    uint32_t  div_addr = 0;
    uint16_t  div_idx  = 0;
    uint32_t  reg_val  = 0;
    r_Error_t ret_val  = R_NG;
    uint8_t   i        = 0;
    uint16_t  cks_sel  = 0;
    uint16_t  div_sel  = 0;
    r_dev_Device_t device;
    r_dev_CksSrc_t pll_sub;
    r_dev_CksSrc_t clk_src;
    
    /* D1M1 translation */
    /* Double substitution:
        1. replace source selection Pll2 -> Pll1
        2. replace allowed setting Pll2 -> Pll1 */
    device = R_DEV_GetDev();
    clk_src = ClkSelCfg->SrcId;
    /* 1. PLL2 -> Pll1 */
    if ((device >= R_DEV_R7F701401) && (device <= R_DEV_R7F701407))
    {
        if (LOC_PLL2 == clk_src)
        {
            clk_src = LOC_PLL1;
        }
    }

    /* Check range */
    if (ClkSelCfg->Cks < R_DEV_CKS_LAST)
    {
        sel_addr     = loc_CkcsBaseAddr[ClkSelCfg->Cks].Sel;
        div_addr     = loc_CkcsBaseAddr[ClkSelCfg->Cks].Div;
        div_idx      = loc_CkcsBaseAddr[ClkSelCfg->Cks].DivIdx;
    }
    else
    {
        R_DEV_ERROR((uint32_t)ClkSelCfg->Cks, R_DEV_ERR_IDX, "Wrong clock domain selected!");
    }

    /* Find the right ID setting */
    for (i = 0; i < 8; i++) /* Search for the ID*/
    {
        pll_sub = loc_ClockTree[ClkSelCfg->Cks][i];
        if ((device >= R_DEV_R7F701401) && (device <= R_DEV_R7F701407))
        {
            /* 2. replace allowed setting Pll2 -> Pll1 */
            if (LOC_PLL2 == pll_sub)
            {
                pll_sub = LOC_PLL1;
            }
        }
        if (pll_sub == clk_src)
        {
            break;
        }
    }

    if (i < 8) /* we found a match */
    {
        cks_sel = i;
    }
    else
    {
        R_DEV_ERROR((uint32_t)clk_src, R_DEV_ERR_IDX, "Bad clock config!");
        
    }
    if (0 != div_addr) /* we do have a divider register */
    {
        /* Find out what kind of divider we have */
        
        /* Find the right Div setting */
        /* Using cks_sel to access the loc_ClkSelDiv table is safe as long
           as we do not have more valid clock ids then divider ids */
        if (LOC_F == loc_ClkSelDiv[div_idx][cks_sel])
        {
            /* For the selected source the divider value is written directly to the register */
            if (ClkSelCfg->Cks < R_DEV_CKS_AWO_LAST)
            {
                R_DEV_WRITE_PROT_REG(LOC_PROTCMD0, div_addr, (uint32_t)ClkSelCfg->Div);
            }
            else if(div_addr >= LOC_CPUSS_CKS_BASE_ADDR)
            {
                R_DEV_WRITE_PROT_REG(LOC_PROTCMD1, div_addr, (uint32_t)ClkSelCfg->Div);
            }
            else
            {
                R_DEV_WRITE_PROT_REG(LOC_PROTCMDD1, div_addr, (uint32_t)ClkSelCfg->Div);
            }
            div_sel = ClkSelCfg->Div;
            /* check setting */
            if (0 == div_sel) /* Clock set to disabled */
            {
                while (((R_DEV_READ_REG(32, div_addr + LOC_CKS_S) & 0x2ul) == 0x20ul))
                {
                }
            }
            else /* clock src might be not set up yet,
                    do not wait here for status active, will be checked later */
            {
            }
        }
        else if (LOC_N == loc_ClkSelDiv[div_idx][cks_sel])
        {
            /* For the selected clock source, there is no divider setting at all  */
            div_sel = LOC_N;
        }
        else /* all others dividers settings */
        {
            for (i = 0; i < 8; i++)
            {
                if (loc_ClkSelDiv[div_idx][i] == (uint16_t) ClkSelCfg->Div)
                {
                    break;
                }
            }
            if (i < 8) /* we found a match */
            {
                /* Write ID */
                if (ClkSelCfg->Cks < R_DEV_CKS_AWO_LAST)
                {
                    R_DEV_WRITE_PROT_REG(LOC_PROTCMD0, div_addr, (uint32_t)i);
                }
                else if(div_addr >= LOC_CPUSS_CKS_BASE_ADDR)
                {
                    R_DEV_WRITE_PROT_REG(LOC_PROTCMD1, div_addr, (uint32_t)i);
                }
                else
                {
                    R_DEV_WRITE_PROT_REG(LOC_PROTCMDD1, div_addr, (uint32_t)i);
                }
                div_sel = i;
            }
            else
            {
                R_DEV_ERROR((uint32_t)ClkSelCfg->Div, R_DEV_ERR_RANGE, "Bad divider config!");
            }
            /* in case of CPU, make sure to wait for the divider first */
            if (R_DEV_CKS_CPU == ClkSelCfg->Cks)
            {
                while (R_DEV_READ_REG(32, div_addr + LOC_CKS_A) != (uint32_t)div_sel)
                {
                   /* Wait for setting to become effective */
                }
            }

        }
    }
    else /* if (0 != loc_CkcsBaseAddr[ClkSelCfg->Cks].Div) ->  we do NOT have a divider */
    {

    }
     /* check if we have an address or a direct source clock setting */
    if (sel_addr > (uint32_t) R_DEV_CKS_SRC_LAST)
    {
        /* The address is valid */
        /* Write ID */
        if (ClkSelCfg->Cks < R_DEV_CKS_AWO_LAST)
        {
            R_DEV_WRITE_PROT_REG(LOC_PROTCMD0, sel_addr, (uint32_t)cks_sel);
        }
        else if(sel_addr >= LOC_CPUSS_CKS_BASE_ADDR)
        {
            R_DEV_WRITE_PROT_REG(LOC_PROTCMD1, sel_addr, (uint32_t)cks_sel);
        }
        else
        {
            R_DEV_WRITE_PROT_REG(LOC_PROTCMDD1, sel_addr, (uint32_t)cks_sel);
        }
    }
 
    /* Write stop request mask */
    /* It is mandatory to set bit 1 in  any case */
    if (0 != ClkSelCfg->StpReqMsk)
    {
         reg_val = 0x3u;
    }
    else
    {
         reg_val = 0x2u;
    }
    /* Stop mask for AWO only */
    if (ClkSelCfg->Cks < R_DEV_CKS_FOUT)
    {
        R_DEV_WRITE_REG(32, div_addr + LOC_CKS_STPMSK, reg_val);
    }
    else if (ClkSelCfg->Cks == R_DEV_CKS_FOUT)
    {
        R_DEV_WRITE_REG(32, sel_addr + LOC_CKS_STPMSK, reg_val);
    }
    else
    {
        /* nothing to do here */
    }
    /* Now check the selector settings */
    /* check if we have an address or a direct source clock setting
       If we have an address, there also exist a status register  */

    if (sel_addr > (uint32_t) R_DEV_CKS_SRC_LAST)
    {
        while (R_DEV_READ_REG(32, sel_addr + LOC_CKS_A) != cks_sel)
        {}
    }
    /* Now, check the divider */
    if ((LOC_N != div_sel) && (0xFFu != div_idx) && (0 != div_sel))
    {
        if (LOC_F != loc_ClkSelDiv[div_idx][cks_sel])
        {
            while (R_DEV_READ_REG(32, div_addr + LOC_CKS_A) != (uint32_t)div_sel)
            {
               /* Wait for setting to become effective */
            }
        }
        else
        {
            while (R_DEV_READ_REG(32, div_addr + LOC_CKS_S) != 0x3u)
            {
            }
        }
    }
    ret_val  = R_ERR_OK;
    return ret_val;
}

/*******************************************************************************
  Function: R_DEV_FoutDivSet

    se: <r_dev_api.h> for details
*/

r_Error_t R_DEV_FoutDivSet(uint16_t Div)
{
    r_Error_t ret_val = R_PARAMETER_RANGE;
    uint32_t addr;
    if (Div < 512)
    {
        addr = loc_CkcsBaseAddr[R_DEV_CKS_FOUT].Div;
        R_DEV_WRITE_REG(32, addr, Div);
        /* Wait for activation */
        while (0 != R_DEV_READ_REG(32, addr + LOC_CKS_S))
        {}
        ret_val = R_ERR_OK;
    }
    return ret_val;
}
/*******************************************************************************
  Function: R_DEV_ClkFrequencyHz

    see: <r_dev_api.h> for details
*/
uint32_t R_DEV_ClkFrequencyHz(r_dev_ClkSel_t ClkSel)
{
    uint32_t        sel_addr = 0;
    uint32_t        div_addr = 0;
    uint8_t         div_idx  = 0;
    uint32_t        reg_val  = 0;
    uint8_t         i        = 0;
    r_dev_CksSrc_t  cks_src  = R_DEV_CKS_SRC_DISABLED;
    uint16_t        cks_div  = 0;
    uint32_t        cks_act  = 0;
    
    
    /* Some translation to get frequency for
       macros that do not allow configuration */
    if (R_DEV_CKS_FIX_LOW == ClkSel)
    {
        reg_val = loc_SrcFrequencyHz(R_DEV_CKS_SRC_LRNG);
        return reg_val;
    }
    if (R_DEV_CKS_VI0PIX == ClkSel)
    {
        reg_val = loc_SrcFrequencyHz(R_DEV_CKS_SRC_VI0PIX);
        return reg_val;
    }
    if (R_DEV_CKS_VI1PIX == ClkSel)
    {
        reg_val = loc_SrcFrequencyHz(R_DEV_CKS_SRC_VI1PIX);
        return reg_val;
    }
    if (R_DEV_CKS_FIX_JIT == ClkSel)
    {
        ClkSel = R_DEV_CKS_CLKJIT;
    }
    /* now we can continue */
    if (ClkSel < R_DEV_CKS_LAST)
    {
        sel_addr     = loc_CkcsBaseAddr[ClkSel].Sel;
        div_addr     = loc_CkcsBaseAddr[ClkSel].Div;
        div_idx      = loc_CkcsBaseAddr[ClkSel].DivIdx;
    }
    else
    {
        R_DEV_ERROR((uint32_t)ClkSel, R_DEV_ERR_IDX, "Wrong clock domain selected!");
    }
    
    if (sel_addr < (uint32_t) R_DEV_CKS_SRC_LAST)
    {
        /* There is no selector, the source clock is fixed */
        /* But there is a divider for sure, and sel_addr is the  R_DEV_CKS_SRC
           in this case */
        cks_src = (r_dev_CksSrc_t)sel_addr;
        /* Get Frequency here, if result is 0, the Clock is inactive */
        cks_act = loc_SrcFrequencyHz(cks_src);
    }
    else
    {
        reg_val = R_DEV_READ_REG(32, sel_addr);
        cks_src = loc_ClockTree[ClkSel][reg_val];
        cks_act = R_DEV_READ_REG(32, sel_addr + LOC_CKS_A);
     
    }
    if (LOC_DIV == cks_src)
    {
        /* The source clock is not stored in the clock tree if the selection is like:
           use divider output or an internal OSC, here we have three "special" casess  */
        switch (ClkSel)
        {
        case R_DEV_CKS_CLKJIT:
            cks_src =  R_DEV_CKS_SRC_PLL0;
            break;
        case R_DEV_CKS_CLKFIX:
            cks_src =  R_DEV_CKS_SRC_PLLFIX;
            break;
        case R_DEV_CKS_PLL2IN:
            cks_src =  R_DEV_CKS_SRC_PLL0;
            break;
        default:
            break;
        }
        cks_act = loc_SrcFrequencyHz(cks_src);

    }
    else
    {
        /* now the other way round, we need to check if the
        divider output is selected or a fixed clock
        (see PLL2In fro example
        - per selection we have divider output, or MOSC w/o divider */
        switch (ClkSel)
        {
        case R_DEV_CKS_CLKJIT:
        case R_DEV_CKS_CLKFIX:
        case R_DEV_CKS_PLL2IN:
            /* The source clock was stored in the selector,
            and in this case the divider does not count */
            div_addr = 0;
            break;
        default:
            break;
        }
        cks_act = loc_SrcFrequencyHz(cks_src);
    }
    /* Selected disabled clock or clock source inactive,
      we're done here                                     */
    if ((LOC_DIS == cks_src) || (0 == cks_act))
    {
        /* Clock disabled */
        return 0;
    }
    
    /* Get divider */
    if (0 != div_addr)
    {
        for (i = 0; i < 8; i++ )
        {
            if (LOC_F == loc_ClkSelDiv[div_idx][i])
            {
                break;
            }
        }
        reg_val = R_DEV_READ_REG(32, div_addr);
        if (i < 8 )
        {
           cks_div = reg_val;
        }
        else
        {
           cks_div = loc_ClkSelDiv[div_idx][reg_val];
        }
    }
    else
    {
        cks_div = 1;
    }
    /* Divider disabled */
    if (0 == cks_div)
    {
        /* Clock disabled */
        reg_val =  0;
    }
    else
    {
        reg_val = loc_SrcFrequencyHz(cks_src) /  cks_div;
    }
    return  reg_val;
}

/*******************************************************************************
  Function: R_DEV_GetClkConfig

    see: <r_dev_api.h> for details
*/
uint32_t R_DEV_GetClkConfig(r_dev_ClkSelConfig_t *ClkData)
{
    uint32_t        sel_addr = 0;
    uint32_t        div_addr = 0;
    uint8_t         div_idx  = 0;
    uint32_t        reg_val  = 0;
    uint8_t         i        = 0;
    r_dev_CksSrc_t  cks_src  = R_DEV_CKS_SRC_DISABLED;
    uint16_t        cks_div  = 0;
    uint32_t        cks_act  = 0;
    
    /* Just dummy */
    ClkData->StpReqMsk  = 0;

   /* Some translation to get frequency for
       macros that do not allow configuration */
    if (R_DEV_CKS_FIX_LOW == ClkData->Cks)
    {
        reg_val = loc_SrcFrequencyHz(R_DEV_CKS_SRC_LRNG);
        ClkData->SrcId      = R_DEV_CKS_SRC_LRNG;
        ClkData->Div        = 1;
        return reg_val;
    }
    if (R_DEV_CKS_VI0PIX == ClkData->Cks)
    {
        reg_val = loc_SrcFrequencyHz(R_DEV_CKS_SRC_VI0PIX);
        ClkData->SrcId      = R_DEV_CKS_SRC_VI0PIX;
        ClkData->Div        = 1;
        return reg_val;
    }
    if (R_DEV_CKS_VI1PIX == ClkData->Cks)
    {
        reg_val = loc_SrcFrequencyHz(R_DEV_CKS_SRC_VI1PIX);
        ClkData->SrcId      = R_DEV_CKS_SRC_VI1PIX;
        ClkData->Div        = 1;
        return reg_val;
    }

    if (R_DEV_CKS_FIX_JIT == ClkData->Cks)
    {
        ClkData->Cks = R_DEV_CKS_CLKJIT;
    }
    /* now we can continue */
    if (ClkData->Cks < R_DEV_CKS_LAST)
    {
        sel_addr     = loc_CkcsBaseAddr[ClkData->Cks].Sel;
        div_addr     = loc_CkcsBaseAddr[ClkData->Cks].Div;
        div_idx      = loc_CkcsBaseAddr[ClkData->Cks].DivIdx;
    }
    else
    {
        R_DEV_ERROR((uint32_t)(ClkData->Cks), R_DEV_ERR_IDX, "Wrong clock domain selected!");
    }
    
    if (sel_addr < (uint32_t) R_DEV_CKS_SRC_LAST)
    {
        /* There is no selector, the source clock is fixed */
        /* But there is a divider for sure, and sel_addr is the  R_DEV_CKS_SRC
           in this case */
        cks_src         = (r_dev_CksSrc_t)sel_addr;
        /* Get Frequency here, if result is 0, the Clock is inactive */
        cks_act         = loc_SrcFrequencyHz(cks_src);
    }
    else
    {
        reg_val         = R_DEV_READ_REG(32, sel_addr);
        cks_src         = loc_ClockTree[ClkData->Cks][reg_val];
        cks_act         = R_DEV_READ_REG(32, sel_addr + LOC_CKS_A);
     
    }
    if (LOC_DIV == cks_src)
    {
        /* The source clock is not stored in the clock tree if the selection is like:
           use divider output or an internal OSC, here we have three "special" casess  */
        switch (ClkData->Cks)
        {
        case R_DEV_CKS_CLKJIT:
            cks_src =  R_DEV_CKS_SRC_PLL0;
            break;
        case R_DEV_CKS_CLKFIX:
            cks_src =  R_DEV_CKS_SRC_PLLFIX;
            break;
        case R_DEV_CKS_PLL2IN:
            cks_src =  R_DEV_CKS_SRC_PLL0;
            break;
        default:
            break;
        }
        cks_act = loc_SrcFrequencyHz(cks_src);

    }
    else
    {
        /* now the other way round, we need to check if the
        divider output is selected or a fixed clock
        (see PLL2In fro example
        - per selection we have divider output, or MOSC w/o divider */
        switch (ClkData->Cks)
        {
        case R_DEV_CKS_CLKJIT:
        case R_DEV_CKS_CLKFIX:
        case R_DEV_CKS_PLL2IN:
            /* The source clock was stored in the selector,
            and in this case the divider does not count */
            div_addr = 0;
            break;
        default:
            break;
        }
        cks_act = loc_SrcFrequencyHz(cks_src);
    }
    
    /* Selected disabled clock or clock source inactive,
      we're done here                                     */
    if ((LOC_DIS == cks_src) || (0 == cks_act))
    {
        /* Clock disabled */
        ClkData->SrcId      = R_DEV_CKS_SRC_DISABLED;
        ClkData->Div        = 0;
        
        /* Stop mask for AWO only */
        if (ClkData->Cks < R_DEV_CKS_FOUT)
        {
            ClkData->StpReqMsk  = R_DEV_READ_REG(32, div_addr + LOC_CKS_STPMSK);
            /* We just want the stop mask here */
            ClkData->StpReqMsk &= 0x1u;
            
        }
        else if (ClkData->Cks == R_DEV_CKS_FOUT)
        {
            ClkData->StpReqMsk  = R_DEV_READ_REG(32, sel_addr + LOC_CKS_STPMSK);
            /* We just want the stop mask here */
            ClkData->StpReqMsk &= 0x1u;
        }
        else
        {
            /* nothing to do here */
            ClkData->StpReqMsk  = 0;
        }
        return 0;
    }
    
    /* Get divider */
    if (0 != div_addr)
    {
        for (i = 0; i < 8; i++ )
        {
            if (LOC_F == loc_ClkSelDiv[div_idx][i])
            {
                break;
            }
        }
        reg_val = R_DEV_READ_REG(32, div_addr);
        if (i < 8 )
        {
           cks_div = reg_val;
        }
        else
        {
           cks_div = loc_ClkSelDiv[div_idx][reg_val];
        }
    }
    else
    {
        cks_div = 1;
    }
    /* Divider disabled */
    if (0 == cks_div)
    {
        /* Clock disabled */
        reg_val =  0;
    }
    else
    {
        reg_val = loc_SrcFrequencyHz(cks_src) /  cks_div;
    }
    ClkData->SrcId      = cks_src;
    ClkData->Div        = cks_div;
    /* Stop mask for AWO only */
    if (ClkData->Cks < R_DEV_CKS_FOUT)
    {
        ClkData->StpReqMsk  = R_DEV_READ_REG(32, div_addr + LOC_CKS_STPMSK);
        /* We just want the stop mask here */
        ClkData->StpReqMsk &= 0x1u;
    }
    else if (ClkData->Cks == R_DEV_CKS_FOUT)
    {
        ClkData->StpReqMsk  = R_DEV_READ_REG(32, sel_addr + LOC_CKS_STPMSK);
        /* We just want the stop mask here */
        ClkData->StpReqMsk &= 0x1u;
    }
    else
    {
        /* nothing to do here */
        ClkData->StpReqMsk  = 0;
    }
    return  reg_val;
}



/*******************************************************************************
  Function: R_DEV_SetViClk0

    se: <r_dev_clk_types.h> for details
*/
void R_DEV_SetViClk0(uint32_t Freq)
{
    loc_Vdce0ViFreq = Freq;
}

/*******************************************************************************
  Function: R_DEV_SetViClk0

    se: <r_dev_clk_types.h> for details
*/
void R_DEV_SetViClk1(uint32_t Freq)
{
    loc_Vdce1ViFreq = Freq;
}

/*******************************************************************************
  Function: R_DEV_BusFrequencyHz

    se: <r_dev_api.h> for details
*/
uint32_t R_DEV_BusFrequencyHz(r_dev_Bus_t Bus)
{
    uint32_t freq  = 0;
    uint32_t ratio = 1;
    uint32_t check1, check2;
    r_dev_Device_t device;
    
    device = R_DEV_GetDev();
  
    if (Bus < R_DEV_CLK_BUS_LAST) /* there is an enable setting */
    {
        if (0 == R_DEV_READ_REG(32, locBusRegAddr[Bus]+ LOC_CKS_A))
        {
            return 0;
        }
    }
    ratio = LocPbusRatio;

    switch (Bus)
    {
    case R_DEV_CLK_XCC      :
        /* check if the XCC itself is "on"*/
        if (0 != R_DEV_READ_REG(32, locBusRegAddr[R_DEV_CLK_XCC]+ LOC_CKS_A))
        {
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU) / 2;
        }
        else
        {
            freq = 0;
        }
        break;
    case R_DEV_CLK_PLL0     :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_PLL0);
        break;
    case R_DEV_CLK_PLL1     :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_PLL1);
        break;
    case R_DEV_CLK_PLL2     :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_PLL2);
        break;
    case R_DEV_CLK_PLL0PIX  :
        freq = loc_ClkGenCfg.Pll[0].FrequencyHz;
        break;
    case R_DEV_CLK_SDRB     :
        if ((device >= R_DEV_R7F701406) && (device <= R_DEV_R7F701407))
        {
            freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_PLL0) / 2u;
        }
        else if ((device >= R_DEV_R7F701408) && (device <= R_DEV_R7F701412))
        {
            freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_PLL1);
        }
        else
        {
            freq = 0u;
        }
        break;
    case R_DEV_CLK_ETNBP    :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU) / (2 * ratio) ;
       break;
    case R_DEV_CLK_ETNBXCC  :
        /** hail to the MISRA  god */
        check1 = R_DEV_READ_REG(32, locBusRegAddr[R_DEV_CLK_ETNBXCC] + LOC_CKS_A);
        check2 = R_DEV_READ_REG(32, locBusRegAddr[R_DEV_CLK_XCC]+ LOC_CKS_A);
        
        /* check if the XCC itself is "on"*/
        if ((0 != check1) || (0 != check2))
        {
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU) / 2;
        }
        else
        {
            freq = 0;
        }
        break;
    case R_DEV_CLK_MLBP     :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU) / (2 * ratio) ;
        break;
    case R_DEV_CLK_MLBXCC   :
        /** hail to the MISRA  god */
        check1 = R_DEV_READ_REG(32, locBusRegAddr[R_DEV_CLK_MLBXCC] + LOC_CKS_A);
        check2 = R_DEV_READ_REG(32, locBusRegAddr[R_DEV_CLK_XCC]+ LOC_CKS_A);
        
        /* check if the XCC itself is "on"*/
        if ((0 != check1) || (0 != check2))
        /* check if the XCC itself is "on"*/
        {
            freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU) / 2;
        }
        else
        {
            freq = 0;
        }
        break;
    case R_DEV_CLK_RSCANP   :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU) / (2 * ratio) ;
        break;
    case R_DEV_CLK_CPU      :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU);
        break;
    case R_DEV_CLK_PLL1_INTERNAL:
        if ((device >= R_DEV_R7F701400) && (device <= R_DEV_R7F701407))
        {
            freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_PLL1) * 1;
        }
        else
        {
            /* For D1M2(H), the PLL1 is always divided by 2, so this value is returned by */
            /* loc_SrcFrequencyHz, as that function gives the frequency input into other macros. */
            /* If we request the internal working frequency of PLL1, we have to double it up. */
            freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_PLL1) * 2;
        }
        break;
    case R_DEV_CLK_ISOPCLK  :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CPU) / (2 * ratio) ;
        break;
    case R_DEV_CLK_FIX      :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CLKFIX);
        break;
    case R_DEV_CLK_JIT      :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_CLKJIT);
        break;
    case R_DEV_CLK_PLLFIX   :
        freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_PLLFIX);
        break;
    case R_DEV_CLK_LRNG     :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_LRNG);
        break;
    case R_DEV_CLK_HRING    :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_HRING);
        break;
    case R_DEV_CLK_ERING    :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_ERING);
        break;
    case R_DEV_CLK_MOSC     :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_MOSC);
        break;
    case R_DEV_CLK_SOSC     :
        freq = loc_SrcFrequencyHz(R_DEV_CKS_SRC_SOSC);
        break;
    default                 :
        R_DEV_ERROR((uint32_t)Bus, R_DEV_ERR_UNIT, "Wher's the Bus!");
        break;
    }
    return (freq);
}

/*******************************************************************************
  Function: R_DEV_BusEnable

  see: <r_dev_clk_types.h> for details
*/
r_Error_t R_DEV_BusEnable(r_dev_Bus_t BusClk, uint8_t EnDis)
{
    uint32_t   regval = 0;
    r_Error_t  retval = R_ERR_OK;
    regval     = (0 != EnDis)? 1:0;
    
    if (BusClk < R_DEV_CLK_BUS_LAST)
    {
        if (BusClk < R_DEV_CLK_PLL2)
        {
            R_DEV_WRITE_PROT_REG(LOC_PROTCMD1, locBusRegAddr[BusClk],regval);
        }
        else
        {
            R_DEV_WRITE_PROT_REG(LOC_PROTCMDD1, locBusRegAddr[BusClk],regval);
        }
        
        while (R_DEV_READ_REG(32, locBusRegAddr[BusClk]+ LOC_CKS_A) != regval)
        {
            /* Just wait until setting becomes active */
        }
    }
    else
    {
        retval = R_PARAMETER_RANGE;
    }
    return retval;
}

/*******************************************************************************
  Function: R_DEV_ClkGenStop

    se: <r_dev_api.h> for details
*/

r_Error_t R_DEV_ClkGenStop(r_dev_ClkGen_t ClkGen)
{
    uint32_t addr      = loc_Base[ClkGen] + LOC_GEN_E;
    uint32_t reg_val   = 0;
    uint32_t prot_addr = 0;
    r_Error_t ret_val = R_NG;
    switch (ClkGen)
    {
    case R_DEV_HS_RING:
    case R_DEV_MOSC:
    case R_DEV_SOSC:
        prot_addr = LOC_PROTCMD0;
        break;
    case R_DEV_PLL0:
    case R_DEV_PLL1:
        prot_addr = LOC_PROTCMD1;
        break;
    case R_DEV_PLL2:
        prot_addr = LOC_PROTCMDD1;
        break;
    default:
       ret_val = R_PARAMETER_RANGE;
        break;
    }
    if (R_PARAMETER_RANGE != ret_val)
    {
        /* now set stop trigger */
        reg_val |= 0x2u;
       // R_DEV_WRITE_PROT_REG(prot_addr, addr, reg_val);
        /* wait for status */
     //   while (0 != R_DEV_ClkGenActive(ClkGen))
        {
        }
        ret_val = R_ERR_OK;
    }
    return ret_val;
}

/*******************************************************************************
  Function: R_DEV_ClkGenStart

    se: <r_dev_api.h> for details
*/

r_Error_t R_DEV_ClkGenStart(r_dev_ClkGen_t ClkGen)
{
    uint32_t addr      = loc_Base[ClkGen] + LOC_GEN_E;
    uint32_t reg_val   = 0;
    uint32_t prot_addr = 0;
    
    r_Error_t ret_val = R_NG;
    switch (ClkGen)
    {
    case R_DEV_HS_RING:
    case R_DEV_MOSC:
    case R_DEV_SOSC:
        prot_addr = LOC_PROTCMD0;
        break;
    case R_DEV_PLL0:
    case R_DEV_PLL1:
        prot_addr = LOC_PROTCMD1;
        break;
    case R_DEV_PLL2:
        prot_addr = LOC_PROTCMDD1;
        break;
    default:
       ret_val = R_PARAMETER_RANGE;
       break;
    }
    if (R_PARAMETER_RANGE != ret_val)
    {
        /* now set start trigger */
        reg_val |= 0x1u;
        R_DEV_WRITE_PROT_REG(prot_addr, addr, reg_val);
        /* wait for status */
        while (0 == R_DEV_ClkGenActive(ClkGen))
        {
             
        }
        ret_val = R_ERR_OK;
    }
    return ret_val;
}

/*******************************************************************************
  Function: R_DEV_MoscCfg

  see: <r_dev_clk_types.h> for details
*/
r_Error_t R_DEV_MoscCfg(const r_dev_MoscCfg_t * ClkCfg)
{
    uint32_t  addr    = 0 ;
    uint32_t  reg_val = 0 ;
    r_Error_t ret_val = R_NG;

    /* We have to stop before the settings can be written  */
    if (0 != R_DEV_ClkGenActive(R_DEV_MOSC))
    {
         R_DEV_ClkGenStop(R_DEV_MOSC);//26308
         //while (0 != R_DEV_ClkGenActive(R_DEV_MOSC))
         {
         }
    }
    /* Stabi time: for X1X it's
        reg_val = (ClkCfg->StabiTimeNs * 8) /  1000   HS ring active:
        or      = (ClkCfg->StabiTimeNs *24) / 100000 HS ring stopped :
    */
    if (0 != R_DEV_ClkGenActive(R_DEV_HS_RING))
    {
        reg_val = (ClkCfg->StabiTimeNs * 8u) /  1000u;
    }
    else
    {
        reg_val = (ClkCfg->StabiTimeNs * 24u) /  100000u;
    }
    addr    = loc_Base[R_DEV_MOSC] + LOC_GEN_STABI;
    R_DEV_WRITE_REG(32, addr, reg_val);

    /* Gain: */
    reg_val = ClkCfg->Gain;
    /* required setting according to UM */
    reg_val |= 0x4u;
    addr    = loc_Base[R_DEV_MOSC] + LOC_GEN_C;
    R_DEV_WRITE_REG(32, addr, reg_val);
   
    /* Check if register values fit  */
    if (R_DEV_READ_REG(32, addr) == reg_val)
    {
        /* Continue configuraton */
        if (0 != ClkCfg->Running)
        {
            addr    = loc_Base[R_DEV_MOSC] + LOC_GEN_E;
            R_DEV_WRITE_PROT_REG(LOC_PROTCMD0, addr, (uint32_t)1u);
        }
        /* It is mandatory to set bit 1 in  any case */
        if (0 != ClkCfg->StpReqMsk)
        {
           
            reg_val = 0x03u;
        }
        else
        {
            reg_val = 0x02u;
        }
        addr    = loc_Base[R_DEV_MOSC] + LOC_GEN_STPMSK;
        R_DEV_WRITE_REG(32, addr, reg_val);

        /* Do not store the settings - this has to be done separately  */
        
        /* Proceed - wait for the setting to take effect */
        while (R_DEV_ClkGenActive(R_DEV_MOSC) != ClkCfg->Running)
        {
                        	 R_GPIO_WritePin(1, 4, 0);   //buzz off
	    			 R_GPIO_WritePin(1, 4, 1);   //buzz on  
	    			// R_GPIO_WritePin(42, 15, 0);   //bg off
	    			// R_GPIO_WritePin(42, 15, 1);   //bg on
        }
        ret_val = R_ERR_OK;
    }
    return ret_val;
}

/*******************************************************************************
  Function: R_DEV_SubOscCfg
 
    see: <r_dev_clk_types.h> for details

*/
r_Error_t R_DEV_SubOscCfg(const r_dev_SubOscCfg_t * ClkCfg)
{
    uint32_t  addr    = 0 ;
    uint32_t  reg_val = 0 ;
    r_Error_t ret_val = R_NG;
    
    addr    = loc_Base[R_DEV_SOSC] + LOC_GEN_E;
    /* Do we have a status change? */
    if (0u != ClkCfg->Running)
    {
        reg_val |= 0x01u;
    }
    else
    {
        reg_val |= 0x02u;
    }
    R_DEV_WRITE_PROT_REG(LOC_PROTCMD0, addr, reg_val);
    /* Do not store the settings - this has to be done separately  */

    /* Proceed - wait for the setting to take effect */
    while (R_DEV_ClkGenActive(R_DEV_SOSC) != ClkCfg->Running)
    {
         
    }
    ret_val = R_ERR_OK;
    return ret_val;
}

/*******************************************************************************
  Function: R_DEV_SetGenStopMask

    se: <r_dev_api.h> for details

*/
r_Error_t R_DEV_SetGenStopMask(r_dev_ClkGen_t ClkGen, uint8_t Val)
{
    uint32_t  addr    = 0 ;
    uint32_t  reg_val = 0 ;
    r_Error_t ret_val = R_NG;

    addr    = loc_Base[ClkGen] + LOC_GEN_STPMSK;
    switch (ClkGen)
    {
    case R_DEV_HS_RING:
        loc_ClkGenCfg.RoscStpReqMsk       = Val;
        break;
    case R_DEV_MOSC:
        loc_ClkGenCfg.MainOsc.StpReqMsk   = Val;
        break;
    default:
        ret_val = R_PARAMETER_RANGE;
        break;
    }
    if (R_PARAMETER_RANGE != ret_val)
    {
        if (0u != Val)
        {
            reg_val = LOC_STOP_MASKED_SET;
        }
        /* It is mandatory to set bit 1 in  any case */
        reg_val |= 0x2u;
        /* Write the value */
        R_DEV_WRITE_REG(32, addr, reg_val);
        /* Check the value */
        if ((R_DEV_READ_REG(32, addr) & (LOC_STOP_MASKED_SET | 0x2u)) == reg_val)
        {
            ret_val = R_ERR_OK;
        }
    }
    return ret_val;
}

/*******************************************************************************
  Function: R_DEV_ClkPllCfg

    se: <r_dev_clk_types.h> for details
*/

r_Error_t R_DEV_PllCfg(uint32_t FrqIn, r_dev_ClkGen_t Pll, const r_dev_PllCfg_t *PllCfg)
{
    uint32_t          freq_out  = PllCfg->FrequencyHz/1000000u;
    r_Error_t         ret_val   = R_NG;
    uint32_t          regval    = 0;
    loc_PllCfgReg_t   pllcfg_reg;
    /* This one is not needed here */
    Pll = Pll;
    
    /* Clear Config */
    pllcfg_reg.NI       = 0;
    pllcfg_reg.P        = 0;
    pllcfg_reg.M        = 0;
    pllcfg_reg.NF       = 0;
    pllcfg_reg.SSMode   = 0;
    pllcfg_reg.SSPerc   = 0;
    pllcfg_reg.SMFreq   = 0;
    
    /* VC= freq = fin * Nr/Pr */
    /* Set/Check the frequency  */
    switch (Pll)
    {
    case R_DEV_PLL0:
        ret_val =  loc_CalcFreqPll0(FrqIn/1000000, &pllcfg_reg, &freq_out);
        if ((R_ERR_OK == ret_val) && (R_DEV_PLL_SSCG == PllCfg->Mode))
        {
            /*The input frequency is provided in KHz here */
            ret_val = loc_CalcSscgPll0(FrqIn/1000, &pllcfg_reg, PllCfg);
        }
        break;
    case R_DEV_PLL1:
        ret_val =  loc_CalcFreqPll1(FrqIn/1000000, &pllcfg_reg, &freq_out);
        break;
    case R_DEV_PLL2 :
        /* First, get the input frquency  */
        regval  =  R_DEV_ClkFrequencyHz(R_DEV_CKS_PLL2IN);
        ret_val =  loc_CalcFreqPll2(regval/1000000, &pllcfg_reg, &freq_out);
        pllcfg_reg.FVV       = 0; /* Init value */
        pllcfg_reg.NF        = 1;
        break;
    default:
        ret_val = R_PARAMETER_RANGE;
        break;
    }
    /* If everything went smooth, store the values */
    if (R_ERR_OK == ret_val)
    {
        /* Do NOT Store the config here, use the store function instead */
        /* .bss area is not working */
        
        /* Make sure that PLL is stopped */
        if (0 != R_DEV_ClkGenActive(Pll))
        {
            R_DEV_ClkGenStop(Pll);
        }
        
        loc_WritePllCfg(Pll, &pllcfg_reg);
    }
    return ret_val;
}

/*******************************************************************************
  Function: R_DEV_ClkPrepare

    se: <r_dev_api.h> for details
*/
void R_DEV_ClkPrepare(void)
{
    
        /* Init Device Clocks  */
    loc_ClkGenCfg.MainOsc.FrequencyHz          = 8000000;
    loc_ClkGenCfg.MainOsc.StabiTimeNs          = 6000000;
    loc_ClkGenCfg.MainOsc.Gain                 = 1;
    loc_ClkGenCfg.MainOsc.Running              = 0;
    loc_ClkGenCfg.MainOsc.StpReqMsk            = 1;
                                                 ;
    loc_ClkGenCfg.SubOsc.FrequencyHz           = 32768;
    loc_ClkGenCfg.SubOsc.Running               = 0;
                                               
    loc_ClkGenCfg.RoscStpReqMsk                = 1;
                                               
    loc_ClkGenCfg.Pll[0].Mode                  = R_DEV_PLL_OFF;
    loc_ClkGenCfg.Pll[0].DithMode              = R_DEV_PLL_DITH_CENTER;
    loc_ClkGenCfg.Pll[0].DithRange             = 3;
    loc_ClkGenCfg.Pll[0].ModFreq               = 50;
    loc_ClkGenCfg.Pll[0].FrequencyHz           = 120000000;
    loc_ClkGenCfg.Pll[0].StabiTimeNs           = 4000000;
    loc_ClkGenCfg.Pll[0].StpReqMsk              =0;
                                               
    loc_ClkGenCfg.Pll[1].Mode                  = R_DEV_PLL_OFF;
    loc_ClkGenCfg.Pll[1].DithMode              = R_DEV_PLL_DITH_FIXED;
    loc_ClkGenCfg.Pll[1].DithRange             = 0;
    loc_ClkGenCfg.Pll[1].ModFreq               = 0;
    loc_ClkGenCfg.Pll[1].FrequencyHz           = 120000000;
    loc_ClkGenCfg.Pll[1].StabiTimeNs           = 1000000;
    loc_ClkGenCfg.Pll[1].StpReqMsk             = 0;
                                               
    loc_ClkGenCfg.Pll[2].Mode                  = R_DEV_PLL_OFF;
    loc_ClkGenCfg.Pll[2].DithMode              = R_DEV_PLL_DITH_FIXED;
    loc_ClkGenCfg.Pll[2].DithRange             = 0;
    loc_ClkGenCfg.Pll[2].ModFreq               = 00;
    loc_ClkGenCfg.Pll[2].FrequencyHz           = 120000000;
    loc_ClkGenCfg.Pll[2].StabiTimeNs           = 1000000;
    loc_ClkGenCfg.Pll[2].StpReqMsk             = 0;

    loc_ClkGenCfg.BusEnable[R_DEV_CLK_XCC]     = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL0]    = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL1]    = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL2]    = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL0PIX] = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_SDRB]    = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_ETNBP]   = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_ETNBXCC] = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_MLBP]    = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_MLBXCC]  = 0;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_RSCANP]  = 0;

}


/*******************************************************************************
  Function: R_DEV_ClkGenInit

    se: <r_dev_api.h> for details
*/
void R_DEV_ClkGenInit(r_dev_ClkGenConfig_t * ClkGenCfg, const r_dev_ClkSelConfig_t * ClkSelCfg)
{
    r_dev_ClkGen_t  i;
    r_dev_Bus_t     bus;
    uint32_t        j = 0;
    r_dev_Device_t device;
    
    
    /*D1M1 PBUS divider */
    device = R_DEV_GetDev();
    if ((device >= R_DEV_R7F701404) && (device <= R_DEV_R7F701405))
    {
        /* change PBUS divider (4-> 2) */
        R_DEV_WRITE_PROT_REG(LOC_PROTCMDPWRGD, LOC_PBUS_RATIO, 0u);
        LocPbusRatio = 1;
    }

    R_DEV_MoscCfg(&(ClkGenCfg->MainOsc));
    //R_DEV_SubOscCfg(&(ClkGenCfg->SubOsc));
     
    /* Config and start PLL (but not yet PLL2) */
    for (i = R_DEV_PLL0; i < R_DEV_PLL2; i++)
    {
        R_DEV_PllCfg(ClkGenCfg->MainOsc.FrequencyHz, i, &(ClkGenCfg->Pll[i]));
        if (R_DEV_PLL_OFF != ClkGenCfg->Pll[i].Mode)
        {
            R_DEV_ClkGenStart(i);

        }
    }

        /* Switch PLL buses (but not yet PLL2) */
    for (bus = R_DEV_CLK_PLL0; bus < R_DEV_CLK_PLL2; bus++)
    {
        R_DEV_BusEnable(bus, ClkGenCfg->BusEnable[bus]);
    }

    /*
    First we need to configure the input selector for PLL2 */
    j = 0;
    while (R_DEV_CKS_LAST != ClkSelCfg[j].Cks)
    {
         if (R_DEV_CKS_PLL2IN == ClkSelCfg[j].Cks)
         {
            R_DEV_ClkIdSet(&(ClkSelCfg[j]));
            break;
         }
         j++;
    }
    
    /* Now the PLL2 */
    R_DEV_PllCfg(ClkGenCfg->MainOsc.FrequencyHz, R_DEV_PLL2, &(ClkGenCfg->Pll[R_DEV_PLL2]));
    if (R_DEV_PLL_OFF != ClkGenCfg->Pll[R_DEV_PLL2].Mode)
    {
        R_DEV_ClkGenStart(R_DEV_PLL2);
    }
 
    /* And finally its bus */
    R_DEV_BusEnable(R_DEV_CLK_PLL2, ClkGenCfg->BusEnable[R_DEV_CLK_PLL2]);
    /* Last but not least, config all clock domians */
    R_DEV_ClkSelection(ClkSelCfg);
    /* ... and switch remaining buses */
    for (bus = R_DEV_CLK_PLL0PIX; bus < R_DEV_CLK_BUS_LAST; bus++)
    {
        R_DEV_BusEnable(bus, ClkGenCfg->BusEnable[bus]);
    }

}

/*******************************************************************************
  Function: R_DEV_ClkGenInitStore

    se: <r_dev_api.h> for details
*/
void R_DEV_ClkGenInitStore(r_dev_ClkGenConfig_t * ClkGenCfg)
{
    r_dev_ClkGen_t  i;
    r_dev_Device_t  device;
    
    /* Just to be clean */
    R_DEV_ClkPrepare();
    loc_ClkGenCfg.MainOsc.FrequencyHz          = ClkGenCfg->MainOsc.FrequencyHz ;
    loc_ClkGenCfg.MainOsc.StabiTimeNs          = ClkGenCfg->MainOsc.StabiTimeNs ;
    loc_ClkGenCfg.MainOsc.Gain                 = ClkGenCfg->MainOsc.Gain        ;
    loc_ClkGenCfg.MainOsc.Running              = ClkGenCfg->MainOsc.Running     ;
    loc_ClkGenCfg.MainOsc.StpReqMsk            = ClkGenCfg->MainOsc.StpReqMsk   ;
                                               
    loc_ClkGenCfg.SubOsc.FrequencyHz           = ClkGenCfg->SubOsc.FrequencyHz  ;
    loc_ClkGenCfg.SubOsc.Running               = ClkGenCfg->SubOsc.Running      ;
    loc_ClkGenCfg.RoscStpReqMsk                = ClkGenCfg->RoscStpReqMsk;
 
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_XCC]     = ClkGenCfg->BusEnable[R_DEV_CLK_XCC]     ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL0]    = ClkGenCfg->BusEnable[R_DEV_CLK_PLL0]    ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL1]    = ClkGenCfg->BusEnable[R_DEV_CLK_PLL1]    ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL2]    = ClkGenCfg->BusEnable[R_DEV_CLK_PLL2]    ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_PLL0PIX] = ClkGenCfg->BusEnable[R_DEV_CLK_PLL0PIX] ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_SDRB]    = ClkGenCfg->BusEnable[R_DEV_CLK_SDRB]    ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_ETNBP]   = ClkGenCfg->BusEnable[R_DEV_CLK_ETNBP]   ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_ETNBXCC] = ClkGenCfg->BusEnable[R_DEV_CLK_ETNBXCC] ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_MLBP]    = ClkGenCfg->BusEnable[R_DEV_CLK_MLBP]    ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_MLBXCC]  = ClkGenCfg->BusEnable[R_DEV_CLK_MLBXCC]  ;
    loc_ClkGenCfg.BusEnable[R_DEV_CLK_RSCANP]  = ClkGenCfg->BusEnable[R_DEV_CLK_RSCANP]  ;
    
    R_DEV_SetGenStopMask(R_DEV_HS_RING, ClkGenCfg->RoscStpReqMsk);

    for (i = R_DEV_PLL0; i <= R_DEV_PLL2; i++)
    {
        loc_PllCfgStore(ClkGenCfg->MainOsc.FrequencyHz, i, &(ClkGenCfg->Pll[i]));
    }
        /*D1M1 PBUS divider */
    device = R_DEV_GetDev();
    if ((device >= R_DEV_R7F701404) && (device <= R_DEV_R7F701405))
    {
        LocPbusRatio = 1;
    }
    else
    {
        LocPbusRatio = 2;
    }
}



/*******************************************************************************
  Function: R_DEV_ClkSelection

    se: <r_dev_api.h> for details
*/
void R_DEV_ClkSelection(const r_dev_ClkSelConfig_t * ClkSelCfg)
{
    uint16_t i = 0;
    
     /* assuming we got a config table, we go through it until
     we see the delimiter */
     while (R_DEV_CKS_LAST != ClkSelCfg[i].Cks)
     {
         R_DEV_ClkIdSet(&(ClkSelCfg[i]));
         i++;
     }
}

  
