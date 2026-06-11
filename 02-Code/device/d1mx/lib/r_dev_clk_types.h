/*
****************************************************************************
PROJECT : Vlib device driver
FILE    : $Id: r_dev_clk_types.h 3922 2014-09-22 08:40:10Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for CLK macro
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

#ifndef DEV_CLK_TYPES_H_
#define DEV_CLK_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <r_typedefs.h>


/*******************************************************************************
  Title: Device specific clock tree types

*/

/*******************************************************************************
  Section: Global Constants 
*/
/*******************************************************************************
Constant:  R_DEV_MOSC_FREQ_HZ

The Main Oscillator frequency is fixed per defien due to .bss low level clock init 
issues 

*/
#define R_DEV_MOSC_FRQ_HZ       8000000uL

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_dev_ClkSel_t

  Index for all available clock selectors/dividers .

  Values:
    R_DEV_CKS_FIRST     -  starting point 
    ...
    R_DEV_CKS_LAST      -  delimiter 
*/

typedef enum {
    R_DEV_CKS_FIRST    ,
    R_DEV_CKS_WDT0 =  R_DEV_CKS_FIRST ,
    R_DEV_CKS_AWOT    ,
    R_DEV_CKS_RTC     ,
    R_DEV_CKS_FOUT    ,
    R_DEV_CKS_AWO_LAST, 
    R_DEV_CKS_CPU     = R_DEV_CKS_AWO_LAST,
    R_DEV_CKS_MLB     ,
    R_DEV_CKS_SFMA    ,
    R_DEV_CKS_RSCAN   ,
    R_DEV_CKS_RSCANXIN,
    R_DEV_CKS_SSIF    ,
    R_DEV_CKS_TAUB0   ,
    R_DEV_CKS_TAUB2   ,
    R_DEV_CKS_TAUJ    ,
    R_DEV_CKS_OSTM    ,
    R_DEV_CKS_LCBI    ,
    R_DEV_CKS_ADCE    ,
    R_DEV_CKS_ISM     ,
    R_DEV_CKS_RLIN    ,
    R_DEV_CKS_CLKJIT  ,
    R_DEV_CKS_CLKFIX  ,
    R_DEV_CKS_PLLFIX  ,
    R_DEV_CKS_PLL2IN  ,
    R_DEV_CKS_DOTCK0  ,
    R_DEV_CKS_DOTCK1  ,
    R_DEV_CKS_VDCE0CK ,
    R_DEV_CKS_VDCE1CK ,
    R_DEV_CKS_RSDSCK  ,
    R_DEV_CKS_VI0CLK  ,
    R_DEV_CKS_MIPIPLL ,
    R_DEV_CKS_MIPIPXL ,
    /* delimiter, do not remove !!! */
    R_DEV_CKS_LAST    ,
    /* Placeholder for reading frequency with 
    R_DEV_ClkFrequencyHz() 
    There is no setting available        */
    R_DEV_CKS_FIX_JIT,
    R_DEV_CKS_FIX_LOW,
    R_DEV_CKS_VI0PIX,
    R_DEV_CKS_VI1PIX,
    R_DEV_CKS_ALL
} r_dev_ClkSel_t;

#define R_DEV_CKS_PWBA      (R_DEV_CKS_CLKJIT)
#define R_DEV_CKS_PWGA      (R_DEV_CKS_FIX_JIT)
#define R_DEV_CKS_PWSA      (R_DEV_CKS_FIX_JIT)
#define R_DEV_CKS_CSIG      (R_DEV_CKS_FIX_JIT)
#define R_DEV_CKS_CSIH      (R_DEV_CKS_FIX_JIT)
#define R_DEV_CKS_SG        (R_DEV_CKS_FIX_JIT)
#define R_DEV_CKS_PCMP      (R_DEV_CKS_FIX_JIT)
#define R_DEV_CKS_RIIC      (R_DEV_CKS_FIX_JIT)
#define R_DEV_CKS_WDT1      (R_DEV_CKS_FIX_LOW)

#define R_DEV_CSIG0_CLK     (R_DEV_CKS_FIX_JIT)
#define R_DEV_CSIG1_CLK     (R_DEV_CKS_FIX_JIT)
#define R_DEV_CSIG2_CLK     (R_DEV_CKS_FIX_JIT)
#define R_DEV_CSIG3_CLK     (R_DEV_CKS_FIX_JIT)
#define R_DEV_PCMP_CLK      (R_DEV_CKS_FIX_JIT)



/*******************************************************************************
  Enum: r_dev_Bus_t

    List of all supported peripheral buses 
*/

typedef enum 
{
    R_DEV_CLK_PLL0,
    R_DEV_CLK_PLL1,
    R_DEV_CLK_PLL2,
    R_DEV_CLK_PLL0PIX,
    R_DEV_CLK_SDRB,
    R_DEV_CLK_ETNBP,
    R_DEV_CLK_MLBP,
    R_DEV_CLK_RSCANP,
    R_DEV_CLK_XCC, 
    R_DEV_CLK_ETNBXCC,
    R_DEV_CLK_MLBXCC,
    R_DEV_CLK_BUS_LAST, 
    /* For the buses below, there is no stop stetting, 
       but these are listed here in order to be able 
       to get the frequency using the function R_DEV_BusFrequencyHz()*/
    R_DEV_CLK_CPU = R_DEV_CLK_BUS_LAST,
    R_DEV_CLK_PLL1_INTERNAL,
    R_DEV_CLK_ISOPCLK,
    R_DEV_CLK_FIX,    
    R_DEV_CLK_JIT,    
    R_DEV_CLK_PLLFIX, 
    R_DEV_CLK_LRNG  ,
    R_DEV_CLK_HRING ,
    R_DEV_CLK_ERING ,
    R_DEV_CLK_MOSC  ,
    R_DEV_CLK_SOSC  ,
    
    
    R_DEV_BUS_ALL
} r_dev_Bus_t; 

/*******************************************************************************
  Enum: r_dev_CksSrc_t

 For each clock domains a clock source can be selected 
 You can configure what is the FOUT source or which GFX clock (0u,1u,2 or 3)
 is used for SFMA or set R_DEV_CKS_TAUB to R_DEV_CKS_SRC_PLL0_4 (this is the pll0 clock / 8)
 This enumeration serves as index for identifying the selected clock source   
*/

typedef enum 
{
    R_DEV_CKS_SRC_DISABLED  = 0u,
    R_DEV_CKS_SRC_LRNG          ,
    R_DEV_CKS_SRC_HRING         ,
    R_DEV_CKS_SRC_ERING         ,
    R_DEV_CKS_SRC_MOSC          ,
    R_DEV_CKS_SRC_SOSC          ,
    R_DEV_CKS_SRC_PLL0          ,
    R_DEV_CKS_SRC_PLL0_2        ,
    R_DEV_CKS_SRC_PLL0_4        ,
    R_DEV_CKS_SRC_PLL1          ,
    R_DEV_CKS_SRC_PLL1_4        ,
    R_DEV_CKS_SRC_PLL2          ,
    R_DEV_CKS_SRC_PLL2_4        ,
    R_DEV_CKS_SRC_ISOCPU_4      ,
    R_DEV_CKS_SRC_CLKFIX        ,
    R_DEV_CKS_SRC_CLKFIX_8      ,
    R_DEV_CKS_SRC_CLKJIT        ,
    R_DEV_CKS_SRC_CLKJIT_2      ,
    R_DEV_CKS_SRC_CLKJIT_4      ,
    R_DEV_CKS_SRC_CLKJIT_8      ,
    R_DEV_CKS_SRC_PLLFIX        ,
    R_DEV_CKS_SRC_PLLFIX_10     ,
    R_DEV_CKS_SRC_PLLFIX_20     ,
    R_DEV_CKS_SRC_PLL0PIX       ,
    R_DEV_CKS_SRC_PLL0PIX_3     ,
    R_DEV_CKS_SRC_PLLFIX_3      , 
    R_DEV_CKS_SRC_DOTCK0        ,
    R_DEV_CKS_SRC_DOTCK0_4      ,
    R_DEV_CKS_SRC_DOTCK1        ,
    R_DEV_CKS_SRC_DOTCK1_4      ,
    R_DEV_CKS_SRC_VI0CLK        ,
    R_DEV_CKS_SRC_VI0PIX        ,
    R_DEV_CKS_SRC_VI1PIX        ,
    R_DEV_CKS_SRC_MIPIPXL       ,
    R_DEV_CKS_SRC_MIPIPLL       ,
   /* delimiter, do not remove !!! */
    R_DEV_CKS_SRC_LAST,
    /* "placeholder": using divider output of the corresponding register
    as "clock source"  */
    R_DEV_CKS_SRC_DIV,
    /* final delimiter ;) */
    R_DEV_CKS_SRC_ALL
} r_dev_CksSrc_t;





/*******************************************************************************
  Enum: r_dev_ClkGen_t

  All possible (user configurable) clock domains - AKA macro clock sources . 

  Values  :
    R_DEV_PLL_0     ,
    R_DEV_PLL_1     ,
    R_DEV_PLL_2     ,
    R_DEV_PLLS_NUM   -> indicator for the max PLL number 
    R_DEV_HS_RING     ,
    R_DEV_MOSC     ,
    R_DEV_SUB_OSC     ,

    
*/

typedef enum
{
    R_DEV_PLL0     ,
    R_DEV_PLL1     ,
    R_DEV_PLL2     ,
    R_DEV_PLL_NUM  ,
    R_DEV_HS_RING  = R_DEV_PLL_NUM , /* we don't want to waste space in arrays */
    R_DEV_MOSC      ,
    R_DEV_SOSC   ,
    /* delimiter, do not remove !!! */
    R_DEV_CLK_GEN_NUM
} r_dev_ClkGen_t;

/*******************************************************************************
  struct: r_dev_ClkSelConfig_t
    
    Storest the settings for configuring a clock domain
    members:
    CksId       - clock selector,        see <r_dev_ClkSel_t> 
    SrcId       - clock source ID,       see <r_dev_CksSrc_t> 
    Div         - divider setting for the clock domain
    StpReqMsk   - the stop request mask, 1: mask set 0: no mask       
     
    
*/
typedef struct  
{
    r_dev_ClkSel_t  Cks;
    r_dev_CksSrc_t  SrcId; 
    uint16_t        Div; 
    uint8_t         StpReqMsk;   
} r_dev_ClkSelConfig_t;


/*******************************************************************************
  Enum: r_dev_PllMode_t
        
    PLL Modes 
    
    Values: 
    R_DEV_PLL_OFF    - PLL disabled  
    R_DEV_PLL_SSCG   - PLL on, dithering enabled  
    R_DEV_PLL_ON     - PLL on, dithering disabled  
        
*/

typedef enum 
{
    R_DEV_PLL_OFF   ,
    R_DEV_PLL_SSCG  ,
    R_DEV_PLL_ON    
} r_dev_PllMode_t;

/*******************************************************************************
  Enum: r_dev_PllDithMode_t
        
    Dithering modes for SSCG PLL
    
    Values: 
    R_DEV_PLL_DITH_FIXED    - No frequency dithering 
    R_DEV_PLL_DITH_NO       - No frequency dithering
    R_DEV_PLL_DITH_DOWN     - down spread frequency dithering,
    R_DEV_PLL_DITH_UP       - up spread frequency dithering
    R_DEV_PLL_DITH_CENTER   - center spread frequency dithering
        
*/

typedef enum 
{
    R_DEV_PLL_DITH_FIXED   ,
    R_DEV_PLL_DITH_NO      ,
    R_DEV_PLL_DITH_DOWN    ,
    R_DEV_PLL_DITH_UP      ,
    R_DEV_PLL_DITH_CENTER  ,
    R_DEV_PLL_DITH_LAST    
} r_dev_PllDithMode_t;


/*******************************************************************************
  Struct: PLL r_dev_PllCfg_t 

    The configuration for a PLL is defined by this structure 
    
    Members: 
    Mode        - PLL mode: disabled, sscg or pll, see <r_dev_PllMode_t>
    DithMode    - dithering modes, see <r_dev_PllDithMode_t>    
    DithRange   - Range of dithering in percent
    ModFreq     - modulation frequency for dithering (40, 50 ,60) in KHz
    FrequencyHz - Output frequency in Hz
    StabiTimeNs - stabilisation time in ns (125ns - 255875ns)
    StpReqMsk   - If set a stand by stop  request is masked  
                  (clock generator does not stop)
*/

typedef struct 
{
    r_dev_PllMode_t     Mode;   
    r_dev_PllDithMode_t DithMode;    
    uint8_t             DithRange;    
    uint32_t            ModFreq;      
    uint32_t            FrequencyHz;
    uint32_t            StabiTimeNs;
    uint8_t             StpReqMsk; 
} r_dev_PllCfg_t;


/*******************************************************************************
  Struct: r_dev_MoscCfg_t 

    The configuration for the Main Osc is defined by this structure 

    Members: 
    FrequencyHz - Output frequency in Hz
    StabiTimeNs - stabilisation time in ns (125ns - (2^17-1)*125ns= 16383875ns)
    Gain        - Oscillator gain    (0 - 3)
    Running     - If set, the Oscillator is (or will be) enabled 
    StpReqMsk   - If set a stand by stop  request is masked
                  (clock generator does not stop)    
*/

typedef struct 
{
    uint32_t            FrequencyHz;
    uint32_t            StabiTimeNs;
    uint8_t             Gain; 
    uint8_t             Running; 
    uint8_t             StpReqMsk; 
} r_dev_MoscCfg_t;

/*******************************************************************************
  Struct: r_dev_SubOscCfg_t 

    The configuration for the Sub Osc is defined by this structure 
    
    Members: 
    FrequencyHz - Output frequency in Hz
    Running     - If set, the Oscillator is (or will be) enabled 
    StpReqMsk   - If set a stand by stop request is masked
                  (clock generator does not stop)    
*/
typedef struct 
{
    uint32_t            FrequencyHz;
    uint8_t              Running; 
    uint8_t              StpReqMsk; 
} r_dev_SubOscCfg_t;

/*******************************************************************************
  Struct: r_dev_ClkGenConfig_t 
    A structure definig all clock generator settings for a device 
    
    Members: 
    MainOsc                       - Main Oscillator config, see <r_dev_MoscCfg_t>
    SubOsc                        - Sub Oscillator config, see <r_dev_SubOscCfg_t>
    Pll[R_DEV_PLLS_NUM]           - Config for every PLL,  see <r_dev_PllCfg_t>
    RoscStpReqMsk;                - If set a stand by stop request for the HS Ring 
                                  is masked,    
    BusEnable[R_DEV_CLK_BUS_LAST] - Enable or disable all busses 
                                  (which do not have any config settings)   

    
*/
typedef struct 
{
    r_dev_MoscCfg_t     MainOsc;
    r_dev_SubOscCfg_t   SubOsc;
    r_dev_PllCfg_t      Pll[R_DEV_PLL_NUM]; 
    uint8_t             RoscStpReqMsk; 

    uint8_t            BusEnable[R_DEV_CLK_BUS_LAST]; 
} r_dev_ClkGenConfig_t; 


/*******************************************************************************
  Section: Global API Functions
  This function will be visible in the API only if this device was selected 
*/

/*******************************************************************************
  Function: R_DEV_PllCfg
  
  configure a given PLL with the provided parameters 
  Parameters:
  FrqIn     - Input Freqency
  Pll       - PLL Id
  *PllCfg   - Pointer to the parameter set for the PLL, see <R_DEV_ClkPllCfg_t> 
  
  Returns:
  == R_ERR_OK  -    Success   , see <r_Error_t>
  != R_ERR_OK  -     Fail     , see <r_Error_t>
   
*/

r_Error_t R_DEV_PllCfg(uint32_t FrqIn, r_dev_ClkGen_t Pll, const r_dev_PllCfg_t *PllCfg);

/******************************************************************************* 
  Function: R_DEV_SubOscCfg 
  configure the Main OSC with the provided parameters 
  
  Parameters:
  *ClkCfg   - Pointer to the parameter set for the Main OSC, see <r_dev_MoscCfg_t> 
  
  Returns:
  == R_ERR_OK  -    Success   , see <r_Error_t>
  != R_ERR_OK  -     Fail     , see <r_Error_t>

*/ 
r_Error_t R_DEV_MoscCfg(const r_dev_MoscCfg_t * ClkCfg);

/******************************************************************************* 
  Function: R_DEV_SubOscCfg 
  configure the Sub OSC with the provided parameters 
  
  Parameters:
  *ClkCfg   - Pointer to the parameter set for the Sub OSC, see <r_dev_SubOscCfg_t> 
  
  Returns:
  == R_ERR_OK  -    Success   , see <r_Error_t>
  != R_ERR_OK  -     Fail     , see <r_Error_t>

*/ 
r_Error_t R_DEV_SubOscCfg(const r_dev_SubOscCfg_t * ClkCfg);  
 
/******************************************************************************* 
  Function: R_DEV_BusEnable 
 Enable or disable a bus block  
  
  Parameters:
  BusClk    - The clock to be switched on or off, see <r_dev_Bus_t> 
  EnDis     - Enable (1) or Disable (2) 
   
  Returns:
  == R_ERR_OK   -    Success   , see <r_Error_t>
  != R_ERR_OK   -    Fail      , see <r_Error_t>

*/ 
r_Error_t R_DEV_BusEnable(r_dev_Bus_t BusClk, uint8_t EnDis); 


/******************************************************************************* 
  Function: R_DEV_SetViClk0  
  Set the frequence for the external video clock 0 (VDCE0_VI_CLK)  
  
  Parameters:
  Freq      - Frequency in Hz 
  
  Returns:
  void 

*/ 
void R_DEV_SetViClk0(uint32_t Freq);  
 
/******************************************************************************* 
  Function: R_DEV_SetViClk1  
  Set the frequence for the external video clock 1 (VDCE1_VI_CLK)  
  
  Parameters:
  Freq      - Frequency in Hz 
  
  Returns:
  void 

*/ 
void R_DEV_SetViClk1(uint32_t Freq);  

#ifdef __cplusplus
}
#endif

#endif /* DEV_CLK_TYPES_H_  */
