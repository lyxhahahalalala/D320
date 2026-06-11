/*
****************************************************************************
PROJECT : VDCE driver
FILE    : $Id: r_vdce_api.h 7921 2016-03-10 04:23:11Z shinya.tomari $
============================================================================
DESCRIPTION
Driver for the VDCE macro
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

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

****************************************************************************
*/

/***************************************************************************
  Title: VDCE Driver API

  VDCE (Video Data Controller E) driver API.

  An application using VDCE should include this header file.

  Dependency:
  The VDCE driver depends on r_ddb_api.h, see <Display Data Base>.

  Note:
  If an OS is used the VDCE driver internally uses semaphores.
*/

#ifndef R_VDCE_API_H
#define R_VDCE_API_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
  Section: Global Defines
*/

/***************************************************************************
  Constants: API Version

  This constant is the value which shows the version information of the VDCE driver.
  It refers at the <R_VDCE_VersionStringGet> function.
  High and Low number of the API version

  Values:
  R_VDCE_VERSION_HI - This constant is the high byte of the version information.
                      It is major version information.
  R_VDCE_VERSION_LO - This constant is the low byte of the version information.
                      It is miner version information.
*/

#define R_VDCE_VERSION_HI  1
#define R_VDCE_VERSION_LO  13

/***************************************************************************
  Constants: Layer Number

  The definition of the layer to specify a layer.

  R_VDCE_LAYER_SCALER0 - This constant is Scaler 0.
  R_VDCE_LAYER_SCALER1 - This constant is Scaler 1.
  R_VDCE_LAYER_IMAGE2  - This constant is Image Synthesizer 2.
  R_VDCE_LAYER_IMAGE3  - This constant is Image Synthesizer 3.
  R_VDCE_LAYER_INPUT   - This constant is Input Controller.
*/

#define R_VDCE_LAYER_SCALER0           (0u)
#define R_VDCE_LAYER_SCALER1           (R_VDCE_LAYER_SCALER0 + 1u)
#define R_VDCE_LAYER_IMAGE2            (R_VDCE_LAYER_SCALER1 + 1u)
#define R_VDCE_LAYER_IMAGE3            (R_VDCE_LAYER_IMAGE2  + 1u)
#define R_VDCE_LAYER_LAST              (R_VDCE_LAYER_IMAGE3  + 1u)

#define R_VDCE_LAYER_INPUT             (R_VDCE_LAYER_LAST    + 1u)

/***************************************************************************
  Constants: Capture without display

  This is used when specifying capturing without the display by <R_VDCE_CapEnable>.

  Values:
  R_VDCE_CAP_NO_DISPLAY - Capture without display.
*/

#define R_VDCE_CAP_NO_DISPLAY  (0xFFFFFFFFu)


/***************************************************************************
  Constants: Number of Gamma correction area 

  The definition of the number of area to gamma correction.

  Values:
  R_VDCE_GAMMA_AREA_NUM - Number of Gamma correction area.
*/

#define R_VDCE_GAMMA_AREA_NUM  (32u)

/***************************************************************************
  Section: Global Types
*/
/***************************************************************************
  Group: Enumerations and Types
*/

/***************************************************************************
  Enum: r_vdce_Error_t

  Description:
  The type describes the error code of VDCE driver functions.

  Values:
  R_VDCE_ERR_OK                    - No error has occurred.
  R_VDCE_ERR_NG                    - An error has occurred,
                                     but no specific error code is defined for it.
  R_VDCE_ERR_PARAM_INCORRECT       - A parameter provided to a function is incorrect.
  R_VDCE_ERR_RANGE_UNIT            - The unit-number is the outside of the range.
  R_VDCE_ERR_RANGE_PARAM           - A parameter is the outside of the range.
  R_VDCE_ERR_NOT_ACCEPTABLE        - A function was called in an incorrect state.
  R_VDCE_ERR_FATAL_OS              - Fatal Error has occurred at OS interface.
  R_VDCE_ERR_FATAL_HW              - Fatal Error has occurred at H/W.
  R_VDCE_ERR_RANGE_LAYER           - The layer-number is the outside of the range.
  R_VDCE_ERR_UNIT_LOCKED           - VDCE is already initialized.
  R_VDCE_ERR_UNIT_NOTLOCKED        - VDCE is not initialized.
  R_VDCE_ERR_DISPLAY_NO_TIMING     - <R_VDCE_DisplayTimingSet> is not done.
  R_VDCE_ERR_LAYER_NO_FORMAT       - <R_VDCE_LayerFormatSet> or <R_VDCE_OirFormatSet> is not done.
  R_VDCE_ERR_LAYER_NO_MEM_GEOMETRY - <R_VDCE_LayerMemGeometrySet> or <R_VDCE_OirMemGeometrySet>
                                     is not done.
  R_VDCE_ERR_LAYER_NO_VIEW_PORT    - <R_VDCE_LayerViewPortSet> or <R_VDCE_OirViewPortSet>
                                     is not done.
  R_VDCE_ERR_LAYER_NO_BASE         - <R_VDCE_LayerBaseSet> or <R_VDCE_OirBaseSet> is not done.
  R_VDCE_ERR_CAP_NO_BUF_GEOMETRY   - <R_VDCE_CapBufGeometrySetup> is not done.
  R_VDCE_ERR_CAP_NO_MODE           - <R_VDCE_CapModeSet> is not done.
  R_VDCE_ERR_PIXELCLOCK            - It failed in the pixel clock setting.
  R_VDCE_ERR_STRIDE_INCORRECT      - The setting of Stride is incorrect.
  R_VDCE_ERR_CHROMAKEY_INCORRECT   - The setting of Chroma-key is incorrect.
  R_VDCE_ERR_SCALED_SIZE_INCORRECT - The setting of Scaled size is incorrect.
  R_VDCE_ERR_INTERRUPT_ENABLED     - Enabled interrupt is remained.
*/

typedef enum
{
    R_VDCE_ERR_OK = 0,
    R_VDCE_ERR_NG,
    R_VDCE_ERR_PARAM_INCORRECT,
    R_VDCE_ERR_RANGE_UNIT,
    R_VDCE_ERR_RANGE_PARAM,
    R_VDCE_ERR_NOT_ACCEPTABLE,
    R_VDCE_ERR_FATAL_OS,
    R_VDCE_ERR_FATAL_HW,
    R_VDCE_ERR_RANGE_LAYER,
    R_VDCE_ERR_UNIT_LOCKED,
    R_VDCE_ERR_UNIT_NOTLOCKED,
    R_VDCE_ERR_DISPLAY_NO_TIMING,
    R_VDCE_ERR_LAYER_NO_FORMAT,
    R_VDCE_ERR_LAYER_NO_MEM_GEOMETRY,
    R_VDCE_ERR_LAYER_NO_VIEW_PORT,
    R_VDCE_ERR_LAYER_NO_BASE,
    R_VDCE_ERR_CAP_NO_BUF_GEOMETRY,
    R_VDCE_ERR_CAP_NO_MODE,
    R_VDCE_ERR_PIXEL_CLOCK,
    R_VDCE_ERR_STRIDE_INCORRECT,
    R_VDCE_ERR_CHROMAKEY_INCORRECT,
    R_VDCE_ERR_SCALED_SIZE_INCORRECT,
    R_VDCE_ERR_INTERRUPT_ENABLED,
    R_VDCE_ERR_LAST
} r_vdce_Error_t;

/***************************************************************************
  Enum: r_vdce_IntType_t

  Description:
  This type holds the different interrupt types.
  In the function <R_VDCE_IntcEnable> they must be order to enable multiple interrupts
  at the same time.

  Values:
  R_VDCE_INTC_NONE                   - No interrupt event.
  R_VDCE_INTC_VBLANK                 - VBLANK signal from timing controller 0.
  R_VDCE_INTC_VBLANK_1               - VBLANK signal from timing controller 1.
  R_VDCE_INTC_SCANLINE               - This event is caused if a special scan line in video
                                       output is hit.
  R_VDCE_INTC_NO_VI_VSYNC_SIGNAL     - No VSync signal from video input at video layer 0.
  R_VDCE_INTC_NO_VI_VSYNC_SIGNAL_1   - No VSync signal from video input at video layer 1.
  R_VDCE_INTC_OIR_SCANLINE           - This event is caused if a special scan line in video
                                       output is hit.
  R_VDCE_INTC_OIR_SCANLINE_WRITE     - Undefined.
  R_VDCE_INTC_OIR_VBLANK             - VBLANK signal from timing controller OIR.
  R_VDCE_INTC_OIR_VSCYNC_WRITE       - Vsync signal from timing controller OIR.
  R_VDCE_INTC_ERR_OIR_UNDERFLOW      - Underflow during reading in output image renderer interface.
  R_VDCE_INTC_ERR_LAYER0_UNDERFLOW   - Underflow during reading in layer 0 interface.
  R_VDCE_INTC_ERR_LAYER1_UNDERFLOW   - Underflow during reading in layer 1 interface.
  R_VDCE_INTC_ERR_LAYER2_UNDERFLOW   - Underflow during reading in layer 2 interface.
  R_VDCE_INTC_ERR_LAYER3_UNDERFLOW   - Underflow during reading in layer 3 interface.
  R_VDCE_INTC_CAP_SCANLINE_WRITE     - This event is caused if the location of the capturing line input 
                                       to Scaler 0/1 layer matches a specified scan line.
  R_VDCE_INTC_CAP_VBLANK             - Captured external VSYNC input. 
  R_VDCE_INTC_CAP_END_OF_FIELD       - Undefined.
  R_VDCE_INTC_ERR_CAP_WRITE_OVERFLOW - Frame buffer write overflow with captured data.
 
*/

typedef enum
{
    R_VDCE_INTC_NONE                   = 0,
    R_VDCE_INTC_VBLANK                 = 1, /* R_VDCE_S0_LO_VSYNC */
    R_VDCE_INTC_VBLANK_1               = 2, /* R_VDCE_S1_LO_VSYNC */
    R_VDCE_INTC_VBLANK_DELAY           = 3, /* R_VDCE_GR3_VBLANK delayed for discom */
    R_VDCE_INTC_SCANLINE               = 4, /* R_VDCE_GR3_VLINE */
    R_VDCE_INTC_OIR_SCANLINE           = 5, /* R_VDCE_OIR_VLINE */
    R_VDCE_INTC_OIR_SCANLINE_WRITE     = 6, /* R_VDCE_OIR_WLINE */
    R_VDCE_INTC_OIR_VBLANK             = 7, /* R_VDCE_OIR_LO_VSYNC */
    R_VDCE_INTC_OIR_VSCYNC_WRITE       = 8, /* R_VDCE_OIR_VI_VSYNC */
    R_VDCE_INTC_ERROR                  = 9, /* R_VDCE_S0_LO_VSYNC */ 
    R_VDCE_INTC_NO_VI_VSYNC_SIGNAL     = 10, /* R_VDCE_S0_VSYNCERR */
    R_VDCE_INTC_NO_VI_VSYNC_SIGNAL_1   = 11, /* R_VDCE_S1_VSYNCERR */
    R_VDCE_INTC_ERR_OIR_UNDERFLOW      = 12, /* R_VDCE_IV8_VBUFERR */
    R_VDCE_INTC_ERR_LAYER0_UNDERFLOW   = 13, /* R_VDCE_IV3_VBUFERR */
    R_VDCE_INTC_ERR_LAYER1_UNDERFLOW   = 14, /* R_VDCE_IV4_VBUFERR */
    R_VDCE_INTC_ERR_LAYER2_UNDERFLOW   = 15, /* R_VDCE_IV5_VBUFERR */
    R_VDCE_INTC_ERR_LAYER3_UNDERFLOW   = 16, /* R_VDCE_IV6_VBUFERR */
    R_VDCE_INTC_CAP_SCANLINE_WRITE     = 17, /* R_VDCE_S0_WLINE */
    R_VDCE_INTC_CAP_VBLANK             = 18, /* R_VDCE_S0_VI_VSYNC */
    R_VDCE_INTC_CAP_END_OF_FIELD       = 19, /* R_VDCE_S0_VFIELD */
    R_VDCE_INTC_ERR_CAP_WRITE_OVERFLOW = 20, /* R_VDCE_IV1_VBUFERR */
    R_VDCE_INTC_MAX                    = 21  /* End of table */
} r_vdce_IntType_t;

/***************************************************************************
  Enum: r_vdce_OutEndian_t

  Description:
  This type is used to select the output data endian in the function <R_VDCE_DisplayOutEndianSet>.
  According to the combination with <r_vdce_OutEndian_t> and <r_vdce_OutFormat_t>, LCD data is as followed.

> r_vdce_OutFormat_t |            RGB888              |            RGB666              |            RGB565              |
> r_vdce_OutSwap_t   |  OFF     OFF      ON     ON    |  OFF     OFF      ON     ON    |  OFF     OFF      ON     ON    |
> r_vdce_OutEndian_t | LITTLE   BIG    LITTLE   BIG   | LITTLE   BIG    LITTLE   BIG   | LITTLE   BIG    LITTLE   BIG   |
> -------------------+--------------------------------+--------------------------------+--------------------------------+
>    LCD_DATA23      |  R[7]    R[0]    B[7]    B[0]  |   0       0       0       0    |   0       0       0       0    |
>         :          |   :       :       :       :    |   :       :       :       :    |   :       :       :       :    |
>    LCD_DATA18      |  R[2]    R[5]    B[2]    B[5]  |   0       0       0       0    |   0       0       0       0    |
>    LCD_DATA17      |  R[1]    R[6]    B[1]    B[6]  |  R[7]    R[2]    B[7]    B[2]  |   0       0       0       0    |
>    LCD_DATA16      |  R[0]    R[7]    B[0]    B[7]  |  R[6]    R[3]    B[6]    B[3]  |   0       0       0       0    |
>    LCD_DATA15      |  G[7]    G[0]    G[7]    G[0]  |  R[5]    R[4]    B[5]    B[4]  |  R[7]    R[3]    B[7]    B[3]  |
>    LCD_DATA14      |  G[6]    G[1]    G[6]    G[1]  |  R[4]    R[5]    B[4]    B[5]  |  R[6]    R[4]    B[6]    B[4]  |
>    LCD_DATA13      |  G[5]    G[2]    G[5]    G[2]  |  R[3]    R[6]    B[3]    B[6]  |  R[5]    R[5]    B[5]    B[5]  |
>    LCD_DATA12      |  G[4]    G[3]    G[4]    G[3]  |  R[2]    R[7]    B[2]    B[7]  |  R[4]    R[6]    B[4]    B[6]  |
>    LCD_DATA11      |  G[3]    G[4]    G[3]    G[4]  |  G[7]    G[2]    G[7]    G[2]  |  R[3]    R[7]    B[3]    B[7]  |
>    LCD_DATA10      |  G[2]    G[5]    G[2]    G[5]  |  G[6]    G[3]    G[6]    G[3]  |  G[7]    G[2]    G[7]    G[2]  |
>    LCD_DATA9       |  G[1]    G[6]    G[1]    G[6]  |  G[5]    G[4]    G[5]    G[4]  |  G[6]    G[3]    G[6]    G[3]  |
>    LCD_DATA8       |  G[0]    G[7]    G[0]    G[7]  |  G[4]    G[5]    G[4]    G[5]  |  G[5]    G[4]    G[5]    G[4]  |
>    LCD_DATA7       |  B[7]    B[0]    R[7]    R[0]  |  G[3]    G[6]    G[3]    G[6]  |  G[4]    G[5]    G[4]    G[5]  |
>    LCD_DATA6       |  B[6]    B[1]    R[6]    R[1]  |  G[2]    G[7]    G[2]    G[7]  |  G[3]    G[6]    G[3]    G[6]  |
>    LCD_DATA5       |  B[5]    B[2]    R[5]    R[2]  |  B[7]    B[2]    R[7]    R[2]  |  G[2]    G[7]    G[2]    G[7]  |
>    LCD_DATA4       |  B[4]    B[3]    R[4]    R[3]  |  B[6]    B[3]    R[6]    R[3]  |  B[7]    B[3]    R[7]    R[3]  |
>         :          |   :       :       :       :    |   :       :       :       :    |   :       :       :       :    |
>    LCD_DATA0       |  B[0]    B[7]    R[0]    R[7]  |  B[2]    B[7]    R[2]    R[7]  |  B[3]    B[7]    R[3]    R[7]  |
> -------------------+--------------------------------+--------------------------------+--------------------------------+


  Values:
  R_VDCE_OUT_ENDIAN_LITTLE - Output data is little endian.
  R_VDCE_OUT_ENDIAN_BIG    - Output data is big endian.
*/
typedef enum
{
    R_VDCE_OUT_ENDIAN_LITTLE = 0,
    R_VDCE_OUT_ENDIAN_BIG,
    R_VDCE_OUT_ENDIAN_LAST
} r_vdce_OutEndian_t;

/***************************************************************************
  Enum: r_vdce_OutSwap_t

  Description:
  This type is used to select whether the output data' blue and red is swapped <R_VDCE_DisplayOutSwapBR>.

  Values:
  R_VDCE_OUT_SWAP_BR_OFF - Output data red & blue is not changed
  R_VDCE_OUT_SWAP_BR_ON  - Output data' blue channel is swapped with red
*/
typedef enum
{
    R_VDCE_OUT_SWAP_BR_OFF = 0,
    R_VDCE_OUT_SWAP_BR_ON,
    R_VDCE_OUT_SWAP_BR_LAST
} r_vdce_OutSwap_t;

/***************************************************************************
  typedef: r_vdce_Hsync_t

  Description:
  The structure holding the value of TCON reference timing in the function 
  <R_VDCE_DisplayHsyncSet> and <R_VDCE_DisplayHsyncGet>.

  Struct members:
  Half     - Clock count from the rising edge of the Hsync signal as the counting timing 
             of horizontal counter. 1/2fH Timing. Range is 0 to 0x7FF.
  Offset   - Offset Hsync Signal Timing. The clock cycle count from the rising edge of 
             the Hsync signal. Range is 0 to 0x7FF.
*/

typedef struct
{
    uint16_t Half;
    uint16_t Offset;
} r_vdce_Hsync_t;

/***************************************************************************
  Enum: r_vdce_Pin_t

  Description:
  This type is used to select a specific signal in the function <R_VDCE_DisplaySignalSet>, 
  <R_VDCE_DisplaySignalGet>, <R_VDCE_DisplayTconPinSet> and <R_VDCE_DisplayTconPinGet>.

  Values:
  R_VDCE_PIN_ENABLE  - DE        Data enable signal.
  R_VDCE_PIN_VSYNC   - STVA/VS   Vsync signal.
  R_VDCE_PIN_HSYNC   - STH/SP/HS Hsync signal.
  R_VDCE_PIN_VSYNC_E - STVB/VE   Gate start signal / Vertical enable signal.
  R_VDCE_PIN_HSYNC_E - STB/LP/HE Source strobe signal / Horizontal enable signal.
  R_VDCE_PIN_CPV_GCK - CPV/GCK   Gate clock signal.
  R_VDCE_PIN_POLA    - POLA      VCOM voltage polarity control signal A.
  R_VDCE_PIN_POLB    - POLB      VCOM voltage polarity control signal B.
*/

typedef enum
{
    R_VDCE_PIN_ENABLE = 0,
    R_VDCE_PIN_VSYNC,
    R_VDCE_PIN_HSYNC,
    R_VDCE_PIN_VSYNC_E,
    R_VDCE_PIN_HSYNC_E,
    R_VDCE_PIN_CPV_GCK,
    R_VDCE_PIN_POLA,
    R_VDCE_PIN_POLB,
    R_VDCE_PIN_LAST
} r_vdce_Pin_t;

/***************************************************************************
  Enum: r_vdce_TconPolmode_t

  Description:
  This type is used to select the signal generation mode in the function 
  <R_VDCE_DisplaySignalSet> and <R_VDCE_DisplaySignalGet>.

  Values:
  R_VDCE_TCON_POLMD_NORMAL - Normal mode. 
                             Generates the signal that changes twice a horizontal period.
  R_VDCE_TCON_POLMD_1X1REV - 1x1 reverse mode. 
                             Generates the signal whose polarity is inverted every horizontal period.
  R_VDCE_TCON_POLMD_1X2REV - 1x2 reverse mode
                             Generates the signal whose polarity is inverted in the first horizontal 
                             period and is subsequently inverted every two horizontal periods.
  R_VDCE_TCON_POLMD_2X2REV - 2x2 reverse mode
                             Generates the signal whose polarity is inverted every two horizontal periods.
*/

typedef enum
{
    R_VDCE_TCON_POLMD_NORMAL = 0,
    R_VDCE_TCON_POLMD_1X1REV,
    R_VDCE_TCON_POLMD_1X2REV,
    R_VDCE_TCON_POLMD_2X2REV,
    R_VDCE_TCON_POLMD_LAST
} r_vdce_TconPolmode_t;

/***************************************************************************
  Enum: r_vdce_TconRefsel_t

  Description:
  This type is used to select the signal operating reference in the function 
  <R_VDCE_DisplaySignalSet> and <R_VDCE_DisplaySignalGet>.

  Values:
  R_VDCE_TCON_REFSEL_HSYNC    - Hsync signal reference
  R_VDCE_TCON_REFSEL_OFFSET_H - Offset Hsync signal reference
*/

typedef enum
{
    R_VDCE_TCON_REFSEL_HSYNC = 0,
    R_VDCE_TCON_REFSEL_OFFSET_H,
    R_VDCE_TCON_REFSEL_LAST
} r_vdce_TconRefsel_t;

/***************************************************************************
  typedef: r_vdce_Signal_t

  Description:
  The structure holding the value of TCON signal configuration in the function 
  <R_VDCE_DisplaySignalSet> and <R_VDCE_DisplaySignalGet>.

  Struct members:
  TconHsvs  - Signal Pulse Start Position (First Changing Timing). 
              If signal type is <R_VDCE_PIN_VSYNC> or <R_VDCE_PIN_VSYNC_E>, 
              Starts pulse output after the time specified by this value from the rising edge of 
              the Vsync signal (1/2fH cycles). Range is 0 to 0x7FF.
              If signal type is <R_VDCE_PIN_HSYNC>, <R_VDCE_PIN_HSYNC_E> or <R_VDCE_PIN_CPV_GCK>, 
              Starts pulse output after the time specified by this value + 1 from the rising edge of 
              the Hsync signal (clock cycles). Range is 0 to 0x7FF.
              If signal type is <R_VDCE_PIN_POLA> or <R_VDCE_PIN_POLB>, 
              Starts pulse output after the time specified by this value from the rising edge of 
              the Hsync signal (clock cycles). 
              If signal generation mode is <R_VDCE_TCON_POLMD_NORMAL>, range is 0 to 0x7FF.
              If signal generation mode is not <R_VDCE_TCON_POLMD_NORMAL>, range is 1 to 0x7FF.
  TconHwvw  - Pulse Width (Second Changing Timing). Range is 0 to 0x7FF.
              Outputs a pulse of the duration of this value.
              If signal type is <R_VDCE_PIN_VSYNC> or <R_VDCE_PIN_VSYNC_E>, the value is by the 
              1/2fH cycles. Otherwise, the value is by the clock cycles.
  TconMd    - Signal generation mode select. 
              This is valid when signal type is <R_VDCE_PIN_POLA> or <R_VDCE_PIN_POLB>.
  TconHsSel - Signal operating reference select.
              This is valid when signal type is not <R_VDCE_PIN_VSYNC> or <R_VDCE_PIN_VSYNC_E>.
*/

typedef struct
{
    uint16_t             TconHsvs;
    uint16_t             TconHwvw;
    r_vdce_TconPolmode_t TconMd;
    r_vdce_TconRefsel_t  TconHsSel;
} r_vdce_Signal_t;

/***************************************************************************
  Enum: r_vdce_TconPin_t

  Description:
  This type is used to select the output pin for LCD driving signal 
  in the function <R_VDCE_DisplayTconPinSet> and <R_VDCE_DisplayTconPinGet>.

  Values:
  R_VDCE_TCON_PIN_0   - Pin0
  R_VDCE_TCON_PIN_1   - Pin1
  R_VDCE_TCON_PIN_2   - Pin2
  R_VDCE_TCON_PIN_3   - Pin3
  R_VDCE_TCON_PIN_4   - Pin4
  R_VDCE_TCON_PIN_5   - Pin5
  R_VDCE_TCON_PIN_6   - Pin6
*/

typedef enum
{
    R_VDCE_TCON_PIN_0 = 0,
    R_VDCE_TCON_PIN_1,
    R_VDCE_TCON_PIN_2,
    R_VDCE_TCON_PIN_3,
    R_VDCE_TCON_PIN_4,
    R_VDCE_TCON_PIN_5,
    R_VDCE_TCON_PIN_6,
    R_VDCE_TCON_PIN_LAST
} r_vdce_TconPin_t;

/***************************************************************************
  Enum: r_vdce_SigEdge_t

  Description:
  This type is used to select the signal edge in the function <R_VDCE_DisplayTconPinSet>
  and <R_VDCE_DisplayTconPinGet>.

  Values:
  R_VDCE_SIG_EDGE_RISING  - Rising edge
  R_VDCE_SIG_EDGE_FALLING - Falling edge
*/

typedef enum
{
    R_VDCE_SIG_EDGE_RISING = 0,
    R_VDCE_SIG_EDGE_FALLING,
    R_VDCE_SIG_EDGE_LAST
} r_vdce_SigEdge_t;

/***************************************************************************
  typedef: r_vdce_TconSig_t

  Description:
  The structure holding the value of TCON pin setting in the function  <R_VDCE_DisplayTconPinSet>
  and <R_VDCE_DisplayTconPinGet>.

  Struct members:
  SigType - Signal type to output
  Edge    - Signal edge 
*/

typedef struct
{
    r_vdce_Pin_t     SigType;
    r_vdce_SigEdge_t Edge;
} r_vdce_TconSig_t;

/***************************************************************************
  Enum: r_vdce_Polarity_t

  Description:
  This type is used to select the polarity in the function <R_VDCE_DisplayPolaritySet>.

  Values:
  R_VDCE_POLARITY_POSITIVE - Polarity is positive.
  R_VDCE_POLARITY_NEGATIVE - Polarity is negative.
*/

typedef enum
{
    R_VDCE_POLARITY_POSITIVE = 0,
    R_VDCE_POLARITY_NEGATIVE,
    R_VDCE_POLARITY_LAST
} r_vdce_Polarity_t;

/***************************************************************************
  typedef: r_vdce_Bright_t

  Description:
  The structure holding the value of Brightness DC in the function <R_VDCE_DisplayCalibrationSet>.

  Struct members:
  B - Brightness (DC) adjustment of B signal.
  G - Brightness (DC) adjustment of G signal.
  R - Brightness (DC) adjustment of R signal.
*/

typedef struct
{
    uint16_t B;
    uint16_t G;
    uint16_t R;
} r_vdce_Bright_t;

/***************************************************************************
  typedef: r_vdce_Contrast_t

  Description:
  The structure holding the value of Contrast gain in the function <R_VDCE_DisplayCalibrationSet>.

  Struct members:
  B - Contrast (gain) adjustment of B signal.
  G - Contrast (gain) adjustment of G signal.
  R - Contrast (gain) adjustment of R signal.
*/

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
} r_vdce_Contrast_t;

/***************************************************************************
  typedef: r_vdce_DitherMd_t

  Description:
  The structure holding the value of dithering mode in the function <R_VDCE_DisplayCalibrationSet>.

  Values:
  R_VDCE_DTH_MD_TRU  - Truncate.
  R_VDCE_DTH_MD_RDOF - Round-off.
  R_VDCE_DTH_MD_2X2  - 2x2 pattern dither.
  R_VDCE_DTH_MD_RAND - Random pattern dither.
*/

typedef enum
{
    R_VDCE_DTH_MD_TRU = 0,
    R_VDCE_DTH_MD_RDOF,
    R_VDCE_DTH_MD_2X2,
    R_VDCE_DTH_MD_RAND,
    R_VDCE_DTH_MD_NUM
} r_vdce_DitherMd_t;

/***************************************************************************
  typedef: r_vdce_Dither_t

  Description:
  Panel dithering

  Struct members:
  Sel - Panel dither operation mode. The default value is R_VDCE_DTH_MD_TRU.
  Pa  - Pattern value (A) of 2x2 pattern dither. The default value is 3.
  Pb  - Pattern value (B) of 2x2 pattern dither. The default value is 0.
  Pc  - Pattern value (C) of 2x2 pattern dither. The default value is 2.
  Pd  - Pattern value (D) of 2x2 pattern dither. The default value is 1.
*/
typedef struct
{
    r_vdce_DitherMd_t Sel;
    uint8_t           Pa;
    uint8_t           Pb;
    uint8_t           Pc;
    uint8_t           Pd;
} r_vdce_Dither_t;

/***************************************************************************
  typedef: r_vdce_GammaCorrect_t

  Description:
  Gamma correction parameters. This is used by <r_vdce_Gamma_t>.
  Gamma correction is carried out by dividing an input signal having 256 gradation 
  levels into 32 and controlling the gain of each area. 
  Gain coefficient of each area can be set as 0 to approx. 2.0 [times].

  Struct members:
  Area[n] - Start threshold of area#n.  
            The range of area#n is as followed.  
              Area#0  is from Area[0] to Area[1].  
              Area#1  is from Area[1] to Area[2].  
              ...
              Area#31 is from Area[31] to 255.  
            Each value should be set as following conditions.
              Area[0] = 0.
              Area[n-1] < Area[n]  < Area[n+1] (1 <= n <=30). 
              Area[31] <= 255.
            Initial value of Area[n] = n*8.
            
  Gain[n] - Gain coefficient of area#n. 
            The value of gain coefficient is Gain[n]/1024 [times].
            The range of Gain[n] is from 0 to 2047.
            Initial value of Gain[n] = 1024 (coefficient = 1).
*/

typedef struct
{
    uint8_t            Area[R_VDCE_GAMMA_AREA_NUM];
    uint16_t           Gain[R_VDCE_GAMMA_AREA_NUM];
} r_vdce_GammaCorrect_t;

/***************************************************************************
  typedef: r_vdce_Gamma_t

  Description:
  This structure is used to set the Gamma correction in the function 
  <R_VDCE_DisplayGammaCorrectSet>.

  Struct members:
  B - Pointer to structure of Gamma adjustment of B signal. 
      If this is set to NULL, Gamma adjustment of B signal is not changed.
  G - Pointer to structure of Gamma adjustment of G signal.
      If this is set to NULL, Gamma adjustment of G signal is not changed.
  R - Pointer to structure of Gamma adjustment of R signal.
      If this is set to NULL, Gamma adjustment of R signal is not changed.
*/
typedef struct
{
    r_vdce_GammaCorrect_t*   B;
    r_vdce_GammaCorrect_t*   G;
    r_vdce_GammaCorrect_t*   R;
} r_vdce_Gamma_t;

/***************************************************************************
  typedef: r_vdce_OutFormat_t

  Description:
  This type is used to select the output format in the function <R_VDCE_DisplayOutFormatSet>.

  Values:
    R_VDCE_OUT_FORMAT_RGB888 - RGB888.
    R_VDCE_OUT_FORMAT_RGB666 - RGB666.
    R_VDCE_OUT_FORMAT_RGB565 - RGB565.
*/

typedef enum
{
    R_VDCE_OUT_FORMAT_RGB888 = 0,
    R_VDCE_OUT_FORMAT_RGB666,
    R_VDCE_OUT_FORMAT_RGB565,
    R_VDCE_OUT_FORMAT_LAST
} r_vdce_OutFormat_t;


/***************************************************************************
  Enum: r_vdce_Format_t

  Description:
  The type is used to specify the Frame buffer format in the function <R_VDCE_LayerFormatSet>.

  Values:
  R_VDCE_RGB565    - RGB565.
  R_VDCE_RGB0888   - RGB888.
  R_VDCE_ARGB1555  - ARGB1555.
  R_VDCE_ARGB4444  - ARGB4444.
  R_VDCE_ARGB8888  - ARGB8888.
  R_VDCE_CLUT8     - CLUT8.
  R_VDCE_CLUT4     - CLUT4.
  R_VDCE_CLUT1     - CLUT1.
  R_VDCE_YCBCR_422 - YCbCr422.
  R_VDCE_YCBCR_444 - YCbCr444.
  R_VDCE_RGBA5551  - RGBA5551.
  R_VDCE_RGBA8888  - RGBA8888.
  R_VDCE_YUV_YUYV  - YUV422.
  R_VDCE_YUV_UYVY  - YUV422.
  R_VDCE_YUV_YVYU  - YUV422.
  R_VDCE_YUV_VYUY  - YUV422.
*/

typedef enum
{
    R_VDCE_RGB565 = 0,
    R_VDCE_RGB0888,
    R_VDCE_ARGB1555,
    R_VDCE_ARGB4444,
    R_VDCE_ARGB8888,
    R_VDCE_CLUT8,
    R_VDCE_CLUT4,
    R_VDCE_CLUT1,
    R_VDCE_YCBCR_422,
    R_VDCE_YCBCR_444,
    R_VDCE_RGBA5551,
    R_VDCE_RGBA8888,
    R_VDCE_YUV_YUYV,
    R_VDCE_YUV_UYVY,
    R_VDCE_YUV_YVYU,
    R_VDCE_YUV_VYUY,
    R_VDCE_FORMAT_LAST
} r_vdce_Format_t;

/***************************************************************************
  typedef: r_vdce_Matrix_t

  Description:
  The structure holding the conversion coefficients for the YUV conversion matrix
  in the function <R_VDCE_LayerMatrixSet>.

  Struct members:
  RY            - Value of RY.
  RU            - Value of RU.
  RV            - Value of RV.
  GY            - Value of GY.
  GU            - Value of GU.
  GV            - Value of GV.
  BY            - Value of BY.
  BU            - Value of BU.
  BV            - Value of BV.
  Y_OFF         - Value of offset of Y.
  U_OFF         - Value of offset of U.
  V_OFF         - Value of offset of V.
  UV_SHIFT_ZERO - When this value is 0, it converts GBR into GBR.
                  When this value is 1, it converts GBR into YCbCr. (<R_VDCE_LAYER_INPUT> only)
                  When this value is 2, it converts YCbCr to GBR.
                  When this value is 3, it converts YCbCr to YCbCr. (<R_VDCE_LAYER_INPUT> only)
*/

typedef struct
{ 
    float32_t RY;
    float32_t RU;
    float32_t RV;
    float32_t GY;
    float32_t GU;
    float32_t GV;
    float32_t BY;
    float32_t BU;
    float32_t BV;
    int32_t Y_OFF;
    int32_t U_OFF;
    int32_t V_OFF;
    uint32_t UV_SHIFT_ZERO;
} r_vdce_Matrix_t;

/***************************************************************************
  Enum: r_vdce_Scale_t

  Description:
  The type is used to specify the scaling strategy, which is used by the driver.
  If a scaling is desired, which cannot be achieved exactly by the hardware,
  the driver has to decide, which scaling factor will be used.
  The driver can either select a larger image or a smaller image.
  See also functions <R_VDCE_LayerImgScaleX> and <R_VDCE_LayerImgScaleY>.

  Values:
  R_VDCE_SCALING_LARGER  - Select the next larger value.
  R_VDCE_SCALING_SMALLER - Select the next smaller value.
  R_VDCE_SCALING_DISABLE - Scaling is disabled.
*/

typedef enum
{
    R_VDCE_SCALING_LARGER =  0,
    R_VDCE_SCALING_SMALLER,
    R_VDCE_SCALING_DISABLE,
    R_VDCE_SCALING_LAST
} r_vdce_Scale_t;

/***************************************************************************
  Enum: r_vdce_ScaleMode_t

  Description:
  The type is used to specify the optional mode for enlargement and reduction.
  See also function <R_VDCE_LayerImgScaleModeSet>.

  Values:
  R_VDCE_SCALE_MODE_NONE       - All flags are off.
  R_VDCE_SCALE_H_PREFILTER     - Enable a horizontal prefilter.
                                 Horizontal prefilter can be used when the horizontal scale is 
                                 reduced. If this flag is on, the frequency band for Brightness 
                                 (Y) and RGB signals are suppressed. If this flag is off, 
                                 horizontal prefilter is disabled.
  R_VDCE_SCALE_H_HOLD_INTERPOL - Use hold interpolation mode as horizontal interpolation.
                                 Horizontal interpolation mode can be selected when the 
                                 horizontal scale is enlarged or reduced. If this flag is off, 
                                 linear interpolation mode is used. When the interpolation 
                                 position is between input pixels Xn and Xn+1, the Xinterpo 
                                 interpolation value is defined as follows. phase depends on 
                                 interruption position.
                                 > hold mode  : Xinterpo = Xn
                                 > linear mode: Xinterpo = (Xn*(4096-phase)+Xn+1*phase)/4096
  R_VDCE_SCALE_V_HOLD_INTERPOL - Use hold interpolation mode as vertical interpolation.
                                 Vertical interpolation mode can be selected when the vertical 
                                 scale is enlarged or reduced. If this flag is off, linear 
                                 interpolation mode is used.
*/

typedef enum
{
    R_VDCE_SCALE_MODE_NONE        = 0,
    R_VDCE_SCALE_H_PREFILTER      = (int32_t)(1uL<< 0),
    R_VDCE_SCALE_H_HOLD_INTERPOL  = (int32_t)(1uL<< 1),
    R_VDCE_SCALE_V_HOLD_INTERPOL  = (int32_t)(1uL<< 2),
} r_vdce_ScaleMode_t;

/***************************************************************************
  typedef: r_vdce_ClutARGB32_t

  Description:
  The structure holding the value of ARGB for CLUT in the function <R_VDCE_LayerClutSet>.

  Struct members:
  B - Value of Blue.
  G - Value of Green.
  R - Value of Red.
  A - Value of Alpha.
*/

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t A;
} r_vdce_ClutARGB32_t;

/***************************************************************************
  typedef: r_vdce_CkARGB32_t

  Description:
  The structure holding the value of ARGB for Chroma-key
  in the function <R_VDCE_LayerChromaKeyEnable>.

  Struct members:
  B - Value of Blue.
  G - Value of Green.
  R - Value of Red.
  A - Value of Alpha.
*/

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t A;
} r_vdce_CkARGB32_t;

/***************************************************************************
  Enum: r_vdce_CapMode_t

  Description:
  The type is used to specify the mode in the function <R_VDCE_CapModeSet>.

  Values:
  R_VDCE_CAP_MODE_YUV_ITU656       - Select ITU-R BT.656 mode with external sync.
  R_VDCE_CAP_MODE_YUV_8BIT         - Select ITU-R BT.601 mode with external sync.
  R_VDCE_CAP_MODE_YUV_16BIT        - Select YCbCr422 mode with external sync.
  R_VDCE_CAP_MODE_RGB_16BPP        - Select RGB565 mode with external sync.
  R_VDCE_CAP_MODE_RGB_18BPP        - Select RGB666 mode with external sync.
  R_VDCE_CAP_MODE_RGB_24BPP        - Select RGB888 / YCbCr444 mode with external sync.
  R_VDCE_CAP_MODE_DITHER           - Use a dithering. Dithering can be used with frame buffer format 
                                     R_VDCE_FB_FORMAT_RGB565
  R_VDCE_CAP_MODE_YUV_Y_UV_INVERT  - Invert the position of Y and UV in the ITU stream. (YUYV -> UYVY) 
  R_VDCE_CAP_MODE_VSYNC_INVERT     - Invert the polarity of the vsync signals. 
                                     (default: pos polarity, inverted: neg polarity)
  R_VDCE_CAP_MODE_HSYNC_INVERT     - Invert the polarity of the hsync signals. 
                                     (default: pos polarity, inverted: neg polarity)
  R_VDCE_CAP_MODE_DATA_CLK_INVERT  - Invert the clock phase for the incoming data signals
                                     (default: falling clock edge, inverted: rising clock edge)
  R_VDCE_CAP_MODE_VSYNC_CLK_INVERT - Invert the Vsync Clock. default neg clock edge.
                                     (default: falling clock edge, inverted: rising clock edge)
  R_VDCE_CAP_MODE_HSYNC_CLK_INVERT - Invert the Hsync Clock. default neg clock edge.
                                     (default: falling clock edge, inverted: rising clock edge)
  R_VDCE_CAP_MODE_H_MIRRORING      - Use horizontal mirroring.
  R_VDCE_CAP_MODE_V_MIRRORING      - Use vertical mirroring.
  R_VDCE_CAP_MODE_FIXED_VSYNC      - Output Vsync from connecting layer is fixed as internally generated 
                                     free-running Vsync signal. 
                                     If this flag is not set, output Vsync changes to externally input 
                                     Vsync signal while the capturing is enabled. 
  
  Note:
  If R_VDCE_CAP_MODE_YUV_ITU656 or R_VDCE_CAP_MODE_YUV_8BIT is selected, each pixels are 
  captured twice. Therefore width of <R_VDCE_CapBufGeometrySetup> must be doubled and 
  horizontal size must be scaled down by half in order to keep original input size.

  Note:
  If R_VDCE_CAP_MODE_FIXED_VSYNC is set, the interrupt R_VDCE_INTC_NO_VI_VSYNC_SIGNAL or 
  R_VDCE_INTC_NO_VI_VSYNC_SIGNAL_1 does not occur.
  If R_VDCE_CAP_MODE_FIXED_VSYNC is not set, flickering may be displayed at the capture enable/disable 
  timing. 
  
*/

typedef enum
{
    R_VDCE_CAP_MODE_NONE             = 0,
    R_VDCE_CAP_MODE_YUV_ITU656       = (int32_t)(1uL<< 0),
    R_VDCE_CAP_MODE_YUV_8BIT         = (int32_t)(1uL<< 1),
    R_VDCE_CAP_MODE_YUV_16BIT        = (int32_t)(1uL<< 2),
    R_VDCE_CAP_MODE_RGB_16BPP        = (int32_t)(1uL<< 3),
    R_VDCE_CAP_MODE_RGB_18BPP        = (int32_t)(1uL<< 4),
    R_VDCE_CAP_MODE_RGB_24BPP        = (int32_t)(1uL<< 5),
    R_VDCE_CAP_MODE_DITHER           = (int32_t)(1uL<< 6),
    R_VDCE_CAP_MODE_YUV_Y_UV_INVERT  = (int32_t)(1uL<< 10),
    R_VDCE_CAP_MODE_VSYNC_INVERT     = (int32_t)(1uL<< 11),
    R_VDCE_CAP_MODE_HSYNC_INVERT     = (int32_t)(1uL<< 12),
    R_VDCE_CAP_MODE_DATA_CLK_INVERT  = (int32_t)(1uL<< 15),
    R_VDCE_CAP_MODE_VSYNC_CLK_INVERT = (int32_t)(1uL<< 16),
    R_VDCE_CAP_MODE_HSYNC_CLK_INVERT = (int32_t)(1uL<< 17),
    R_VDCE_CAP_MODE_H_MIRRORING      = (int32_t)(1uL<< 18),
    R_VDCE_CAP_MODE_V_MIRRORING      = (int32_t)(1uL<< 19),
    R_VDCE_CAP_MODE_FIXED_VSYNC      = (int32_t)(1uL<< 20)
} r_vdce_CapMode_t;

/***************************************************************************
  Enum: r_vdce_Fbformat_t

  Description:
  The type is used to specify the Frame buffer format in the function <R_VDCE_CapBufGeometrySetup>.

  Values:
  R_VDCE_FB_FORMAT_YCBCR_422 - YCbCr422.
  R_VDCE_FB_FORMAT_RGB565    - RGB565.
  R_VDCE_FB_FORMAT_RGB0888   - RGB0888.
  R_VDCE_FB_FORMAT_YCBCR_444 - YCbCr444.
  R_VDCE_FB_RB_SWAP          - Swap R and B in case of a RGB Frame buffer format.
*/

typedef enum
{
    R_VDCE_FB_FORMAT_YCBCR_422   = 0,
    R_VDCE_FB_FORMAT_RGB565      = 1,
    R_VDCE_FB_FORMAT_RGB0888     = 2,
    R_VDCE_FB_FORMAT_YCBCR_444   = 3,
    R_VDCE_FB_RB_SWAP            = (int32_t)(1uL << 16)
} r_vdce_Fbformat_t;

/***************************************************************************
  Enum: r_vdce_LayerMode_t

  Description:
  The type is used to specify the optional mode for layer.
  See also function <R_VDCE_LayerModeSet>.

  Values:
  R_VDCE_LAYER_MODE_NONE       - All flags are off.
  R_VDCE_LAYER_MODE_V_MIRORING - Enable vertical mirroring. 
                                 This flag can be selected when graphic data is input.
                                 When capture data is input, vertical mirroring should be set 
                                 by <R_VDCE_CapModeSet>.
*/

typedef enum
{
    R_VDCE_LAYER_MODE_NONE       = 0,
    R_VDCE_LAYER_MODE_V_MIRORING = (int32_t)(1uL<< 0),
} r_vdce_LayerMode_t;

/***************************************************************************
  Enum: r_vdce_OirMode_t

  Description:
  This type is used to select the OIR mode in the function <R_VDCE_OirModeSet>.

  Values:
  R_VDCE_OIR_MODE_NORMAL       - OIR mode is normal.
  R_VDCE_OIR_MODE_SCREEN_SHOTS - OIR mode is screen shots. 
                                 A distortion image by VOWE is not output to 
                                 LCD panel. A distortion image data is only 
                                 extracted by the frame buffer of OIR.
*/

typedef enum
{
    R_VDCE_OIR_MODE_NORMAL = 0,
    R_VDCE_OIR_MODE_SCREEN_SHOTS,
    R_VDCE_OIR_MODE_LAST
} r_vdce_OirMode_t;

/***************************************************************************
  Section: Global API functions
*/
/***************************************************************************
  Group: Basic functions
*/

/***************************************************************************
  Function: R_VDCE_Init

  Description:
  This function initializes the driver and the hardware as far as necessary. 
  The Unit parameter holds a number that specifies the video output number being initialized. 
  A used flag is set inside the driver structure to lock it being initialized and currently used.
  The driver makes sure, that the macro is set into a default configuration.
  It is also in the responsibility of the driver to keep track of the macro instance status and
  to avoid, that the macro instance is initialized more than one time or that
  any other function can be executed before the initialization has been completed successfully.
  If the function successfully execute, the return code will be <R_VDCE_ERR_OK> and
  the state will be in the initialized state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number. It can be 0 or 1 in D1M2x and 0 in D1M1x, D1L2x version.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - OK [All (VDCE, layer, OIR and capture) status will be changed to "Initialized".]
  Initialized   - NG
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_Init(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_DeInit

  Description:
  This function deinitializes the driver and the hardware as far as 
  necessary and frees the resources by deleting the used flag.
  The function disables all layers and the display.
  If the function successfully execute, the return code will be <R_VDCE_ERR_OK> and
  the state will be in the uninitialized state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - OK (Do nothing)
  Initialized   - OK [All (VDCE, layer, OIR and capture) will be changed to "Uninitialized".]
  Idle          - OK [All (VDCE, layer, OIR and capture) will be changed to "Uninitialized".]
  Executing     - OK [All (VDCE, layer, OIR and capture) will be changed to "Uninitialized".]
*/

r_vdce_Error_t R_VDCE_DeInit(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_VersionStringGet

  Description:
  This function returns the version string of this VDCE driver.

  Parameter:
  None.

  Return value:
  Pointer of version string.

  Executable VDCE Status:
  Uninitialized - OK
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

const uint8_t *R_VDCE_VersionStringGet(void);

/***************************************************************************
  Function: R_VDCE_MacroVersionGet

  Description:
  This function returns the major and minor version of the H/W macro.

  Parameter:
  Major - The macro version as a major version.
  Minor - The macro version as a minor version.

  Return value:
  See <r_vdce_Error_t>.

  Uninitialized - OK
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_MacroVersionGet(uint32_t * const Major,
                                      uint32_t * const Minor);

/***************************************************************************
  Function: R_VDCE_ErrorCallbackSet

  Description:
  This function sets a callback function that is called in case of an error.
  If ErrorCallback is NULL (R_NULL) no callback function will be called in case an error occurs and
  just the error code is returned. The ErrorCallback is global for all VDCE units.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  ErrorCallback - A function that is called in case an error occurred.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - OK
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_ErrorCallbackSet(void (* const ErrorCallback)(const uint32_t       Unit,
                                                                    const r_vdce_Error_t Error));


/***************************************************************************
  Group: Interrupt functions
*/

/***************************************************************************
  Function: R_VDCE_IntcCallbackSet

  Description:
  This function sets a callback for a special interrupt type.
  The function is called in case an Interrupt of IntType occurs and the interrupt is enabled.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  IntType - The interrupt type specifies the Interrupt event, which can be selected.
            See <r_vdce_IntType_t>.
  Isr     - A function that is called in case an interrupt occurred.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcCallbackSet(const uint32_t         Unit,
                                      const r_vdce_IntType_t IntType,
                                      void                   (* const Isr)(void));

/***************************************************************************
  Function: R_VDCE_IntcCallbackGet

  Description:
  This function gets the pointer to the callback for the given interrupt type.

  Parameter:
  Unit    - It specifies the video output number.
  IntType - The interrupt type specifies the Interrupt event, which can be selected.
            See <r_vdce_IntType_t>.
  Isr     - A function that is called in case an interrupt occurred.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcCallbackGet(const uint32_t         Unit,
                                      const r_vdce_IntType_t IntType,
                                      void                   (** const Isr)(void));

/***************************************************************************
  Function: R_VDCE_IntcEnable

  Description:
  This function enables the activation of an interrupt of type IntType.
  IntType can be an order value of <r_vdce_IntType_t>.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  IntType - The interrupt type specifies the Interrupt event, which can be selected.
            See <r_vdce_IntType_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcEnable(const uint32_t         Unit,
                                 const r_vdce_IntType_t IntType);

/***************************************************************************
  Function: R_VDCE_IntcDisable

  Description:
  This function disables the activation of an interrupt of type IntType.
  IntType can be an order value of <r_vdce_IntType_t>.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  IntType - The interrupt type specifies the Interrupt event, which can be selected.
            See <r_vdce_IntType_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcDisable(const uint32_t         Unit,
                                  const r_vdce_IntType_t IntType);

/****************************************************************************
  Function: R_VDCE_IntcScanlineSet

  Description:
  This function sets the interrupt occurrence timing by the location of image line at Graphic 3 
  layer.  When the location of image line read by Graphic 3 layer matches this setting, 
  R_VDCE_INTC_SCANLINE callback is notified. 
  The interrupt occurs even if Graphic 3 layer is disabled.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Scanline - It specifies the interrupt occurrence timing by the location of image line at the 
             Graphics 3 layer.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcScanlineSet(const uint32_t Unit,
                                      const uint32_t Scanline);

/****************************************************************************
  Function: R_VDCE_IntcScanlineGet

  Description:
  This function gets the current location of image line being read on Graphics 3 layer.

  Parameter:
  Unit     - It specifies the video output number.
  Scanline - This is a pointer to the current location of image line being read on Graphics 3 
             layer.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcScanlineGet(const uint32_t   Unit,
                                      uint32_t * const Scanline);

/****************************************************************************
  Function: R_VDCE_IntcOirScanlineSet

  Description:
  This function sets interrupt occurrence timing by the location of image line at OIR layer. 
  When the location of image line output from OIR layer matches this setting, 
  R_VDCE_INTC_OIR_SCANLINE callback is notified.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Scanline - It specifies the interrupt occurrence timing by the location of image line at OIR layer.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcOirScanlineSet(const uint32_t Unit,
                                         const uint32_t Scanline);

/****************************************************************************
  Function: R_VDCE_IntcOirScanlineGet

  Description:
  This function gets the current location of image line output from OIR layer.

  Parameter:
  Unit     - It specifies the video output number.
  Scanline - This is a pointer to the current location of image line output from OIR layer.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcOirScanlineGet(const uint32_t   Unit,
                                         uint32_t * const Scanline);

/****************************************************************************
  Function: R_VDCE_IntcCapScanlineSet

  Description:
  This function sets interrupt occurrence timing by the location of capturing line. When the 
  location of capturing line input to Scaler 0 (or Scaler 1) layer matches this setting, 
  R_VDCE_INTC_CAP_SCANLINE_WRITE callback is notified.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - VDCE unit number to capture.
  Scanline - It specifies the interrupt occurrence timing by the location of capturing line.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_IntcCapScanlineSet(const uint32_t Unit,
                                         const uint32_t Scanline);

/****************************************************************************
  Function: R_VDCE_IntcCapScanlineGet

  Description:
  This function gets the current location of capturing line input to Scaler 0 (or Scaler 1) layer.

  Parameter:
  Unit     - VDCE unit number to capture.
  Scanline - This is a pointer to the current location of capturing line input to Scaler 0 (or 
             Scaler 1) layer.

  Return value:
  See <r_vdce_Error_t>.
*/

r_vdce_Error_t R_VDCE_IntcCapScanlineGet(const uint32_t   Unit,
                                         uint32_t * const Scanline);

/****************************************************************************
  Function: R_VDCE_Isr

  Description:
  This function is called from ISR (Interrupt Service Routines) and processes
  the interrupt factor which excludes the error of VDCE.
  It depends on the CPU which is loaded into the device for ISR.

  Parameter:
  Unit     - It specifies the video output number.
  IntType - The interrupt type specifies the Interrupt event, which can be selected.
            See <r_vdce_IntType_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - OK
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/
r_vdce_Error_t R_VDCE_Isr(const uint32_t         Unit,
                          const r_vdce_IntType_t IntType);

/****************************************************************************
  Function: R_VDCE_IsrError

  Description:
  This function is called from ISR (Interrupt Service Routines) and processes
  the error interrupt factor of VDCE.
  It depends on the CPU which is loaded into the device for ISR.

  Parameter:
  Unit     - It specifies the video output number.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - OK
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/
r_vdce_Error_t R_VDCE_IsrError(const uint32_t Unit);


/***************************************************************************
  Group: Display functions
*/

/***************************************************************************
  Function: R_VDCE_DisplayTimingSet

  Description:
  This function sets the display timing, including display resolution,
  sync position blank widths and pixel clocks.
  Include the header r_ddb_api.h to use this function, see <Display Data Base>.
  It contains the <r_ddb_Timing_t> parameter.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit   - It specifies the video output number.
  Timing - The timing parameter contains all the information to driver the display.
           See <r_ddb_Timing_t>.
           
           > Range of Timing->ScreenWidth   is 16 to 1280(D1M2x)/1024(D1M1x)/480(D1L2x).
           > Range of Timing->ScreenHeight  is 16 to 1024(D1M2x)/1024(D1M1x)/320(D1L2x).
           > Range of Timing->H.Total       is 32 to 2048.
           > Range of Timing->V.Total       is 20 to 2048.
           > Range of Timing->H.BlankWidth  is 16 to 2032.
           > Range of Timing->V.BlankWidth  is 4  to 2032.
           > Range of Timing->H.SyncWidth   is 1  to (Timing->H.BlankWidth - 1).
           > Range of Timing->V.SyncWidth   is 1  to (Timing->V.BlankWidth - 1).
           > Range of Timing->H.FrontPorch  is 0  to (Timing->H.BlankWidth - 2).
           > Range of Timing->V.FrontPorch  is 0  to (Timing->V.BlankWidth - 2).
           > Range of horizontal back porch is 1  to (Timing->H.BlankWidth - 1).
           > Range of vertical   back porch is 1  to (Timing->V.BlankWidth - 1).
           >     *back porch = BlankWidth - SyncWidth - FrontPorch
           > 
           > Following equations must be satisfied.
           > (Timing->H.BlankWidth + Timing->ScreenWidth)  = Timing->H.Total
           > (Timing->V.BlankWidth + Timing->ScreenHeight) = Timing->V.Total

  Return value:
  See <r_vdce_Error_t>.
  
  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayTimingSet(const uint32_t               Unit,
                                       const r_ddb_Timing_t * const Timing);

/***************************************************************************
  Function: R_VDCE_DisplayTimingAdjust

  Description:
  This function adjusts the display timing.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit   - It specifies the video output number.
  VLines - It specifies an adjustment by the number of the vertical lines.
           When 0 is set, it doesn't adjust display timing.
           The upper limit of the set value is 16 lines.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK 
  Idle          - OK (Parameters become effective at the timing of next Vsync.)
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayTimingAdjust(const uint32_t Unit,
                                          const uint32_t VLines);

/***************************************************************************
  Function: R_VDCE_DisplayOutEndianSet

  Description:
  This function sets the video output data endian.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit      - It specifies the video output number.
  OutEndian - It specifies the video output data endian. See <r_vdce_OutEndian_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayOutEndianSet(const uint32_t           Unit, 
                                          const r_vdce_OutEndian_t OutEndian);

/***************************************************************************
  Function: R_VDCE_DisplayOutSwapBR

  Description:
  This function swaps the video output data red and blue channel.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  OutSwap - It specifies the video output data swap. See <r_vdce_OutSwap_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayOutSwapBR(const uint32_t           Unit,
                                       const r_vdce_OutSwap_t   OutSwap);

/***************************************************************************
  Function: R_VDCE_DisplayColorSet

  Description:
  This function sets the display background color.
  It is seen, if no layer (or a transparent one) is on top of it.
  The default background color is black (All value is 0).
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit           - It specifies the video output number.
  Red/Green/Blue - The individual color components of the background color.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK (Parameters become effective at the timing of next Vsync.)
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_DisplayColorSet(const uint32_t Unit,
                                      const uint8_t  Red,
                                      const uint8_t  Green,
                                      const uint8_t  Blue);

/***************************************************************************
  Function: R_VDCE_DisplayHsyncSet

  Description:
  This function sets the TCON reference timing of Hsync. 
  Execute <R_VDCE_DisplayTimingSet> before executing this function.
  This setting is valid until <R_VDCE_DisplayTimingSet>, <R_VDCE_DisplayTimingAdjust>, or <R_VDCE_DeInit>
  is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Hsync    - This is a pointer to the value of TCON reference timing of Hsync. See <r_vdce_Hsync_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayHsyncSet(const uint32_t               Unit,
                                      const r_vdce_Hsync_t * const Hsync);

/***************************************************************************
  Function: R_VDCE_DisplayHsyncGet

  Description:
  This function gets the TCON reference timing of Hsync. 
  Execute <R_VDCE_DisplayTimingSet> before executing this function.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Hsync    - This is a pointer to the value of TCON reference timing of Hsync. See <r_vdce_Hsync_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_DisplayHsyncGet(const uint32_t         Unit,
                                      r_vdce_Hsync_t * const Hsync);

/***************************************************************************
  Function: R_VDCE_DisplaySignalSet

  Description:
  This function sets the TCON signal configuration.
  Execute <R_VDCE_DisplayTimingSet> before executing this function
  This setting is valid until <R_VDCE_DisplayTimingSet>, <R_VDCE_DisplayTimingAdjust>, or <R_VDCE_DeInit>
  is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  SigType  - Here the TCON signal type can be selected from <r_vdce_Pin_t> except for DE signal.
  Signal   - This is a pointer to the value of TCON signal configuration. See <r_vdce_Signal_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplaySignalSet(const uint32_t                Unit,
                                       const r_vdce_Pin_t            SigType,
                                       const r_vdce_Signal_t * const Signal);

/***************************************************************************
  Function: R_VDCE_DisplaySignalGet

  Description:
  This function gets the TCON signal configuration.
  Execute <R_VDCE_DisplayTimingSet> before executing this function
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  SigType  - Here the TCON signal type can be selected from <r_vdce_Pin_t> except for DE signal.
  Signal   - This is a pointer to the value of TCON signal configuration. See <r_vdce_Signal_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_DisplaySignalGet(const uint32_t          Unit,
                                       const r_vdce_Pin_t      SigType,
                                       r_vdce_Signal_t * const Signal);

/***************************************************************************
  Function: R_VDCE_DisplayTconPinSet

  Description:
  This function sets the TCON signal type which is output from the specified Tcon pin.
  This setting is valid until <R_VDCE_DeInit> is executed.
  The signal type of Pin0(Vsync) and Pin2(Hsync) cannot be changed. Only edge is changeable.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Pin      - Here the TCON pin type can be selected from <r_vdce_TconPin_t>.
  TconSig  - This is a pointer to the value of TCON signal type. See <r_vdce_TconSig_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayTconPinSet(const uint32_t                 Unit,
                                        const r_vdce_TconPin_t         Pin,
                                        const r_vdce_TconSig_t * const TconSig);

/***************************************************************************
  Function: R_VDCE_DisplayTconPinGet

  Description:
  This function gets the TCON signal type which is output from the specified Tcon pin.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Pin      - Here the TCON pin type can be selected from <r_vdce_TconPin_t>.
  TconSig  - This is a pointer to the value of TCON signal type. See <r_vdce_TconSig_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK
*/

r_vdce_Error_t R_VDCE_DisplayTconPinGet(const uint32_t           Unit,
                                        const r_vdce_TconPin_t   Pin,
                                        r_vdce_TconSig_t * const TconSig);

/***************************************************************************
  Function: R_VDCE_DisplayPolaritySet

  Description:
  This function sets the polarity of the specified TCON signal type.
  But, the polarity of some TCON signals has to be set by <R_VDCE_DisplayTimingSet> (Timing->Flags).
  About R_VDCE_PIN_ENABLE, <R_VDCE_Sys_DesyncActLevelSet> is called with the polarity setting 
  by <R_VDCE_DisplayTimingSet>. If <R_VDCE_Sys_DesyncActLevelSet> returns 0, the polarity setting 
  by <R_VDCE_DisplayTimingSet> is effective for VDCE hardware. This function does not have effect.
  If <R_VDCE_Sys_DesyncActLevelSet> returns non-0, the polarity setting by this function is 
  effective for VDCE hardware.
  About R_VDCE_PIN_VSYNC, R_VDCE_Sys_VsyncActLevelSet is called with the polarity setting by 
  <R_VDCE_DisplayTimingSet>. If <R_VDCE_Sys_VsyncActLevelSet> returns 0, the polarity setting by 
  <R_VDCE_DisplayTimingSet> is effective for VDCE hardware. This function does not have effect.
  If <R_VDCE_Sys_VsyncActLevelSet> returns non-0, the polarity setting by this function is effective 
  for VDCE hardware.
  About R_VDCE_PIN_HSYNC, <R_VDCE_Sys_HsyncActLevelSet> is called with the polarity setting by 
  <R_VDCE_DisplayTimingSet>. If <R_VDCE_Sys_HsyncActLevelSet> returns 0, the polarity setting by 
  <R_VDCE_DisplayTimingSet> is effective for VDCE hardware. This function does not have effect.
  If <R_VDCE_Sys_HsyncActLevelSet> returns non-0, the polarity setting by this function is effective 
  for VDCE hardware.
  About other signals, the polarity setting by this function is always effective for VDCE hardware.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Pin      - Here the pin can be selected from <r_vdce_Pin_t>.
  Polarity - Here the polarity can be set to <r_vdce_Polarity_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayPolaritySet(const uint32_t          Unit,
                                         const r_vdce_Pin_t      Pin,
                                         const r_vdce_Polarity_t Polarity);

/***************************************************************************
  Function: R_VDCE_DisplayCalibrationSet

  Description:
  This function sets the brightness, contrast and panel dithering.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Bright   - This is a pointer to the value of Brightness. When not changing, set NULL (R_NULL).
             Each default value is 512. See <r_vdce_Bright_t>.
  Contrast - This is a pointer to the value of Contrast gain. When not changing, set NULL (R_NULL).
             Each default value is 128. See <r_vdce_Contrast_t>.
  Dither   - This is a pointer to the value of panel dithering. When not changing, set NULL (R_NULL).
             See <r_vdce_Dither_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK (Parameters become effective at the timing of next Vsync.)
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_DisplayCalibrationSet(const uint32_t                  Unit,
                                            const r_vdce_Bright_t * const   Bright, 
                                            const r_vdce_Contrast_t * const Contrast,
                                            const r_vdce_Dither_t *const Dither);

/***************************************************************************
  Function: R_VDCE_DisplayGammaCorrectSet

  Description:
  This function sets the RGB gamma correction parameters.
  If Gamma is set to R_NULL, this function disables the gamma correction.
  Gamma correction is processed after brightness and contrast control
  (Brightness -> Contrast -> Gamma correction).
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  Gamma    - This is a pointer to the value of Gamma Correction parameter.
             When the gamma correction is disabled, set to R_NULL.
             See <r_vdce_Gamma_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK (Parameters become effective at the timing of next Vsync.)
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_DisplayGammaCorrectSet(const uint32_t                  Unit,
                                             const r_vdce_Gamma_t * const    Gamma);

/***************************************************************************
  Function: R_VDCE_DisplayOutFormatSet

  Description:
  This function sets the output format.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit      - It specifies the video output number.
  OutFormat - Here the output format can be set to <r_vdce_OutFormat_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayOutFormatSet ( const uint32_t Unit, 
                                            const r_vdce_OutFormat_t OutFormat );


/***************************************************************************
  Function: R_VDCE_DisplayVsyncProtectionSet

  Description:
  This function sets the Vsync protection.
  Vsync protection timing is made by PixelClock set by <R_VDCE_DisplayTimingSet>.
  The maximum value of MaskMs and LackMs is about (8,388,480,000 / PixelClock[Hz])[msec].
  If setting value is over the range, this function assumes that the maximum value is set.
  If Vsync lack is detected by compensation protection, R_VDCE_INTC_NO_VI_VSYNC_SIGNAL or 
  R_VDCE_INTC_NO_VI_VSYNC_SIGNAL_1 interrupt occurs.
  Vsync protections are disabled by default. 
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit      - It specifies the video output number.
  MaskMs    - Prevent vsyncs coming faster than MaskMs msec.
              If non-0 value is set, vsync masking is enabled. 
              If 0 is set, vsync masking is disabled.
  LackMs    - Compensate vsyncs coming slower than LackMs msec. 
              If non-0 value is set, vsync compensation is enabled. 
              If 0 is set, vsync compensation is disabled.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - NG
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayVsyncProtectionSet(const uint32_t Unit,
                                                const uint16_t MaskMs,
                                                const uint16_t LackMs);

/***************************************************************************
  Function: R_VDCE_DisplayEnable

  Description:
  This function enables the display output.
  Execute <R_VDCE_DisplayTimingSet> before executing this function.
  If the function successfully execute, the return code will be <R_VDCE_ERR_OK> and
  the state will be in the idle state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - NG
  Initialized   - OK [All (VDCE, layer, OIR and capture) will be changed to "Idle".]
  Idle          - OK (Do nothing)
  Executing     - OK (Do nothing)
*/

r_vdce_Error_t R_VDCE_DisplayEnable(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_DisplayDisable

  Description:
  This function disables the display output.
  All interrupt must be disabled by <R_VDCE_IntcDisable> before executing this function.
  If the function successfully execute, the return code will be R_VDCE_ERR_OK and
  the state will be in the initialized state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number.

  Return value:
  See <r_vdce_Error_t>.

  Executable VDCE Status:
  Uninitialized - OK (Do nothing)
  Initialized   - OK (Do nothing)
  Idle          - OK [All (VDCE, layer, OIR and capture) will be changed to "Initialized".]
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_DisplayDisable(const uint32_t Unit);


/***************************************************************************
  Group: Layer functions
*/

/***************************************************************************
  Function: R_VDCE_LayerBaseSet

  Description:
  This function sets the layers base address.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3  
  Address - This is the 32 bit address of the layer buffer.
            Lower 7 bits are clamped to 0. If this is set to 0 then returns error.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_LayerBaseSet(const uint32_t Unit,
                                   const uint32_t LayerNr,
                                   const uint32_t Address);

/***************************************************************************
  Function: R_VDCE_OirBaseSet

  Description:
  This function sets the output image renderer read layer base address.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  Address - This is the 32 bit address of the layer buffer.
            Lower 7 bits are clamped to 0. If this is set to 0 then returns error.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_OirBaseSet(const uint32_t Unit,
                                 const uint32_t Address);

/***************************************************************************
  Function: R_VDCE_LayerMemGeometrySet

  Description:
  This function sets the layers memory geometry.
  It is the width and height of the layer's virtual memory window.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit      - It specifies the video output number.
  LayerNr   - It specifies the layer from the following.
                R_VDCE_LAYER_SCALER0 / 
                R_VDCE_LAYER_SCALER1 / 
                R_VDCE_LAYER_IMAGE2  / 
                R_VDCE_LAYER_IMAGE3  
  Stride    - This is the width of the virtual memory window in pixel. Range is 1 to 4096.
              Lower 7 bits of Stride multiplied by bytes per pixel are clamped to 0.
  MemHeight - This is the height of the virtual memory window. Range is 1 to 4096.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerMemGeometrySet(const uint32_t Unit,
                                          const uint32_t LayerNr,
                                          const uint32_t Stride,
                                          const uint32_t MemHeight);

/***************************************************************************
  Function: R_VDCE_OirMemGeometrySet

  Description:
  This function sets the output image renderer read layer memory geometry.
  It is the width and height of the layer's virtual memory window.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit      - It specifies the video output number.
  Stride    - This is the width of the virtual memory window in pixel. Range is 1 to 4096.
              Lower 7 bits of Stride multiplied by bytes per pixel are clamped to 0.
  MemHeight - This is the height of the virtual memory window. Range is 1 to 4096.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after OIR status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_OirMemGeometrySet(const uint32_t Unit,
                                        const uint32_t Stride,
                                        const uint32_t MemHeight);

/***************************************************************************
  Function: R_VDCE_LayerFormatSet

  Description:
  This function sets the color format for the layer.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3  
  Format  - The color format. See <r_vdce_Format_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (This function executes R_VDCE_LayerDisable and R_VDCE_LayerEnable internally.)
*/

r_vdce_Error_t R_VDCE_LayerFormatSet(const uint32_t        Unit,
                                     const uint32_t        LayerNr,
                                     const r_vdce_Format_t Format);

/***************************************************************************
  Function: R_VDCE_OirFormatSet

  Description:
  This function sets the color format for the output image renderer read layer.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit   - It specifies the video output number.
  Format - It specifies the the color format from the following. 
              R_VDCE_RGB565 /
              R_VDCE_ARGB8888 
           See <r_vdce_Format_t>.
  
  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (This function executes R_VDCE_OirDisable and R_VDCE_OirEnable internally.)
*/

r_vdce_Error_t R_VDCE_OirFormatSet(const uint32_t        Unit,
                                   const r_vdce_Format_t Format);

/***************************************************************************
  Function: R_VDCE_LayerViewPortSet

  Description:
  This function sets the layers viewport parameters.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit       - It specifies the video output number.
  LayerNr    - It specifies the layer from the following.
                 R_VDCE_LAYER_SCALER0 / 
                 R_VDCE_LAYER_SCALER1 / 
                 R_VDCE_LAYER_IMAGE2  / 
                 R_VDCE_LAYER_IMAGE3  
  MemPosX    - This is the memory position x coordinate. Range is 0 only.
  MemPosY    - This is the memory position y coordinate. Range is 0 only.
  DispPosX   - This is the display position x coordinate. Range is -1280 to 1279.
               Hs(Horizontal signal start position) is the sum of "display position x"
               and "horizontal blank width". When the sum(Hs) falls below the LOC_HS_MIN, 
               Hs is LOC_HS_MIN value. LOC_HS_MIN is 16. 
               For example, in case of "horizontal blank width"=-160, the lower range of 
               "display position x" is -144(=-160+16).
  DispPosY   - This is the display position y coordinate. Range is -1024 to 1023.
               Vs(Vertical signal start position) is the sum of "display position y"
               and "vertical blank width". When the sum(Vs) falls below the LOC_VS_MIN, 
               Hs is LOC_VS_MIN value. LOC_VS_MIN is 4. 
               For example, in case of "vertical blank width"=-10, the lower range of 
               "display position y" is -6(=-10+4).
  DispWidth  - This is the displayed width of the layer. Range is 3 to 1280.
  DispHeight - This is the displayed height of the layer. Range is 1 to 1024.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_LayerViewPortSet(const uint32_t Unit,
                                       const uint32_t LayerNr,
                                       const int32_t  MemPosX,
                                       const int32_t  MemPosY,
                                       const int32_t  DispPosX,
                                       const int32_t  DispPosY,
                                       const int32_t  DispWidth,
                                       const int32_t  DispHeight);

/***************************************************************************
  Function: R_VDCE_OirViewPortSet

  Description:
  This function sets the output image renderer read layers viewport parameters.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit       - It specifies the video output number.
  MemPosX    - This is the memory position x coordinate. Range is 0 only.
  MemPosY    - This is the memory position y coordinate. Range is 0 only.
  DispPosX   - This is the display position x coordinate. Range is -1280 to 1279.
  DispPosY   - This is the display position y coordinate. Range is -1024 to 1023.
  DispWidth  - This is the displayed width of the layer. Range is 3 to 1280.
  DispHeight - This is the displayed height of the layer. Range is 1 to 1024.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_OirViewPortSet(const uint32_t Unit,
                                     const int32_t  MemPosX,
                                     const int32_t  MemPosY,
                                     const int32_t  DispPosX,
                                     const int32_t  DispPosY,
                                     const int32_t  DispWidth,
                                     const int32_t  DispHeight);

/***************************************************************************
  Function: R_VDCE_LayerRingBufferEnable

  Description:
  This function switches from a full framebuffers for the layer to a ring buffer usage.
  The size of the ring buffer in lines is given in Size parameter.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1  
  Size    - Size of the ring buffer in lines,
            e.g. 1 -> one line ring buffer, 2 -> two line ring buffer etc.
            Range is 1 to 2048.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerRingBufferEnable(const uint32_t Unit,
                                            const uint32_t LayerNr,
                                            const uint32_t Size);

/***************************************************************************
  Function: R_VDCE_OirRingBufferEnable

  Description:
  This function switches from a full framebuffers for the output image renderer read layer
  to a ring buffer usage. The size of the ring buffer in lines is given in Size parameter.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number.
  Size - Size of the ring buffer in lines,
         e.g. 1 -> one line ring buffer, 2 -> two line ring buffer etc.
         Range is 1 to 2048.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after OIR status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_OirRingBufferEnable(const uint32_t Unit,
                                          const uint32_t Size);

/***************************************************************************
  Function: R_VDCE_LayerRingBufferDisable

  Description:
  This function switches back from ring buffer a full framebuffers usage of the layer.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1  

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerRingBufferDisable(const uint32_t Unit,
                                             const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_OirRingBufferDisable

  Description:
  This function switches back from ring buffer a full framebuffers usage of the
  output image renderer read layer.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after OIR status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_OirRingBufferDisable(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_LayerVSyncDelaySet

  Description:
  This function sets the Vsync signal delay between input Vsync to the scaler and output Vsync 
  from the scaler.
  This delay can be adjusted the reading timing from frame buffer. 
  It is useful when the input data is captured.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1  
  Delay   - It specifies the delays in the output Hsync period units.
            e.g. 1 -> 1 line delay, 2 -> 2 line delay etc.
            Default value is 0. Range is 0 to 255.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerVSyncDelaySet(const uint32_t Unit,
                                         const uint32_t LayerNr,
                                         const uint32_t Delay);

/***************************************************************************
  Function: R_VDCE_OirVSyncDelaySet

  Description:
  This function adjusts the read delay between output from Graphics 3 and input
  of output image renderer read layer.
  The output image renderer read layer is always running in sync with the output
  from the 4 graphics layers (Graphics 0 - Graphics 3).
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit  - It specifies the video output number.
  Delay - It specifies the delays between output from Graphics 3 and input
          of output image renderer read layer.
          The delay is specified in line units,
          e.g. 1 -> 1 line delay, 2 -> 2 line delay etc.
          Default value is 1. Range is 1 to 255.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after OIR status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_OirVSyncDelaySet(const uint32_t Unit,
                                       const uint32_t Delay);

/****************************************************************************
  Function: R_VDCE_LayerModeSet

  Description:
  This function sets the optional mode for layer.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit         - It specifies the video output number.
  LayerNr      - It specifies the layer from the following.
                   R_VDCE_LAYER_SCALER0 / 
                   R_VDCE_LAYER_SCALER1 / 
                   R_VDCE_LAYER_IMAGE2  / 
                   R_VDCE_LAYER_IMAGE3  
  LayerMode    - The layer mode. The flags of type <r_vdce_ScaleMode_t> can be combined with '|'.
                 See <r_vdce_ScaleMode_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerModeSet(const uint32_t           Unit,
                                   const uint32_t           LayerNr,
                                   const r_vdce_LayerMode_t LayerMode);

/***************************************************************************
  Function: R_VDCE_OirModeSet

  Description:
  This function sets the OIR layer mode.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  OirMode - It specifies the OIR layer mode.
            See <r_vdce_OirMode_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - NG
*/

r_vdce_Error_t R_VDCE_OirModeSet(const uint32_t         Unit,
                                 const r_vdce_OirMode_t OirMode);

/***************************************************************************
  Function: R_VDCE_LayerEnable

  Description:
  This function enables the layer display.
  Execute <R_VDCE_LayerFormatSet>, <R_VDCE_LayerMemGeometrySet> and <R_VDCE_LayerViewPortSet>
  before executing this function.
  If the function successfully execute, the return code will be <R_VDCE_ERR_OK> and
  the state will be in the executing state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3  

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - NG
  Idle          - OK [The layer status will be changed to "Executing". 
                      VDCE status will also be changed to "Executing" if VDCE status is "Idle". ]
  Executing     - OK (Do nothing)
*/

r_vdce_Error_t R_VDCE_LayerEnable(const uint32_t Unit,
                                  const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_OirEnable

  Description:
  This function enables the output image renderer layer display.
  Execute <R_VDCE_OirFormatSet>, <R_VDCE_OirMemGeometrySet> and <R_VDCE_OirViewPortSet>
  before executing this function.
  If the function successfully execute, the return code will be <R_VDCE_ERR_OK> and
  the state will be in the executing state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - NG
  Initialized   - NG
  Idle          - OK [OIR status will be changed to "Executing". 
                      VDCE status will also be changed to "Executing" if VDCE status is "Idle". ]
  Executing     - OK (Do nothing)
*/

r_vdce_Error_t R_VDCE_OirEnable(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_LayerDisable

  Description:
  This function disables the layer display.
  If the function successfully execute, the return code will be R_VDCE_ERR_OK and
  the state will be in the idle state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3  

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - OK (Do nothing)
  Initialized   - OK (Do nothing)
  Idle          - NG
  Executing     - OK [The layer status will be changed to "Idle". 
                      VDCE status will also be changed to "Idle" if all block (layer, OIR and capture) status are "Idle". ]
*/

r_vdce_Error_t R_VDCE_LayerDisable(const uint32_t Unit,
                                   const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_OirDisable

  Description:
  This function disables the output image renderer layer display.
  If the function successfully execute, the return code will be R_VDCE_ERR_OK and
  the state will be in the idle state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - It specifies the video output number.

  Return value:
  See <r_vdce_Error_t>.

  Executable OIR Status:
  Uninitialized - OK (Do nothing)
  Initialized   - OK (Do nothing)
  Idle          - NG
  Executing     - OK [OIR status will be changed to "Idle". 
                      VDCE status will also be changed to "Idle" if all block (layer, OIR and capture) status are "Idle".]
*/

r_vdce_Error_t R_VDCE_OirDisable(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_LayerMatrixSet

  Description:
  This function sets the color conversion matrix of layer 0, layer 1 and video input.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_INPUT  
  Matrix  - Contains the conversion matrix including offsets. See <r_vdce_Matrix_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_LayerMatrixSet(const uint32_t                Unit,
                                     const uint32_t                LayerNr,
                                     const r_vdce_Matrix_t * const Matrix);

/***************************************************************************
  Function: R_VDCE_LayerMatrixBT601Set

  Description:
  This function sets the color conversion matrix of layer 0, layer 1 and video input to BT601 spec.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_INPUT  

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_LayerMatrixBT601Set(const uint32_t Unit,
                                          const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerMatrixJPEGSet

  Description:
  This function sets the color conversion matrix of layer 0, layer 1 and video input to JPEG spec.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_INPUT  

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_LayerMatrixJPEGSet(const uint32_t Unit,
                                         const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerMatrixUnitySet

  Description:
  This function sets the color conversion matrix of layer 0, layer 1 and video input to bypass mode.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER0 / 
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_INPUT  

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_LayerMatrixUnitySet(const uint32_t Unit,
                                          const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerImgScaleX

  Description:
  This function sets the scaling parameters in X direction. 
  Scaling is done according to the hardware capabilities.
  Therefore the exact chosen value can possibly not be achieved.
  The achieved value is given back as a return type.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit        - It specifies the video output number.
  LayerNr     - It specifies the layer from the following.
                  R_VDCE_LAYER_SCALER0 / 
                  R_VDCE_LAYER_SCALER1 
  ScaledWidth - The parameter defines the desired scaled width of the video.
                This is used to calculate the scaling ratio. Range is 4 and over.
                If Scaling is <R_VDCE_SCALING_LARGER>, range is less than DispWidth of
                <R_VDCE_LayerViewPortSet>.
                If Scaling is <R_VDCE_SCALING_SMALLER>, range is less than Width of
                <R_VDCE_CapBufGeometrySetup> and range is 1024 and less. Lower 2 bits 
                are clamped to 0.
  Scaling     - Scaling defines the behavior if the exact scale value cannot be reached.
                Depending on the value, the function will use a smaller or larger size.
                See <r_vdce_Scale_t>.

  Return value:
  uint32_t.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)

  Note:
  If <R_VDCE_SCALING_LARGER> is selected, following function is not effective in the layer.
  - alpha channel per pixel data.
  - premultiplied alpha channel.
  - constant alpha channel 
  - chroma key

*/

uint32_t R_VDCE_LayerImgScaleX(const uint32_t       Unit,
                               const uint32_t       LayerNr,
                               const uint32_t       ScaledWidth,
                               const r_vdce_Scale_t Scaling);

/***************************************************************************
  Function: R_VDCE_LayerImgScaleY

  Description:
  This function sets the scaling parameters in Y direction. 
  Scaling is done according to the hardware capabilities.
  Therefore the exact chosen value can possibly not be achieved.
  The achieved value is given back as a return type.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit         - It specifies the video output number.
  LayerNr      - It specifies the layer from the following.
                   R_VDCE_LAYER_SCALER0 / 
                   R_VDCE_LAYER_SCALER1 
  ScaledHeight - The parameter defines the desired scaled height of the video.
                 This is used to calculate the scaling ratio. Range is 4 and over.
                 If Scaling is <R_VDCE_SCALING_LARGER>, range is less than DispHeight of
                 <R_VDCE_LayerViewPortSet>.
                 If Scaling is <R_VDCE_SCALING_SMALLER>, range is less than Height of
                 <R_VDCE_CapBufGeometrySetup> and range is 1024 and less. Lower 2 bits 
                 are clamped to 0.
  Scaling      - Scaling defines the behavior if the exact scale value cannot be reached.
                 Depending on the value, the function will use a smaller or larger size.
                 See <r_vdce_Scale_t>.

  Return value:
  uint32_t.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
  
  Note:
  If <R_VDCE_SCALING_LARGER> is selected, following function is not effective in the layer.
  - alpha channel per pixel data
  - premultiplied alpha channel
  - constant alpha channel 
  - chroma key

*/

uint32_t R_VDCE_LayerImgScaleY(const uint32_t       Unit,
                               const uint32_t       LayerNr,
                               const uint32_t       ScaledHeight,
                               const r_vdce_Scale_t Scaling);


/***************************************************************************
  Function: R_VDCE_LayerImgScaleModeSet

  Description:
  This function sets the optional mode for enlargement and reduction.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit         - It specifies the video output number.
  LayerNr      - It specifies the layer from the following.
                   R_VDCE_LAYER_SCALER0 / 
                   R_VDCE_LAYER_SCALER1 
  Mode         - The scaler mode. The flags of type <r_vdce_ScaleMode_t> can be combined with '|'.
                 See <r_vdce_ScaleMode_t>.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the next following timing.)
                  - R_VDCE_LayerImgScaleX is exectuted while layer is executing.
                  - R_VDCE_LayerImgScaleY is exectuted while layer is executing.
                  - The layer status is changed from Idle to Executing.
*/

r_vdce_Error_t R_VDCE_LayerImgScaleModeSet(const uint32_t           Unit,
                                           const uint32_t           LayerNr,
                                           const r_vdce_ScaleMode_t Mode);

/***************************************************************************
  Group: Extended Layer functions
*/

/***************************************************************************
  Function: R_VDCE_LayerAlphaChannelEnable

  Description:
  This function re-enables the alpha channel per pixel data that is disabled 
  by <R_VDCE_LayerAlphaChannelDisable>. 
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3    

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerAlphaChannelEnable(const uint32_t Unit,
                                              const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerAlphaChannelDisable

  Description:
  This function disables the alpha channel per pixel data.
  If color format set by <R_VDCE_LayerFormatSet> has alpha channel (i.e. ARGBxxxx, RGBAxxxx, CLUTx ),
  the alpha channel per pixel data is enabled at first. This function can disable this alpha channel.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3    

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerAlphaChannelDisable(const uint32_t Unit,
                                               const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerPremultipliedAlphaEnable

  Description:
  This function enables the layers premultiplied alpha channel.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3    

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerPremultipliedAlphaEnable(const uint32_t Unit,
                                                    const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerPremultipliedAlphaDisable

  Description:
  This function disables the layers premultiplied alpha channel.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3    

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerPremultipliedAlphaDisable(const uint32_t Unit,
                                                     const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerAlphaConstEnable

  Description:
  This function enables the layers alpha constant alpha function.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit       - It specifies the video output number.
  LayerNr    - It specifies the layer from the following.
                 R_VDCE_LAYER_SCALER1 / 
                 R_VDCE_LAYER_IMAGE2  / 
                 R_VDCE_LAYER_IMAGE3    
  AlphaConst - This is a constant alpha value from 0-255 that is used for every pixel in the layer.
               It is multiplied with the alpha value from alpha channel
               to construct the final alpha value that is used to blend the pixel.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)

  Note:
  If constant alpha channel is enabled, following function is not effective in the layer.
  - chroma key
  
*/

r_vdce_Error_t R_VDCE_LayerAlphaConstEnable(const uint32_t Unit,
                                            const uint32_t LayerNr,
                                            const uint8_t  AlphaConst);

/***************************************************************************
  Function: R_VDCE_LayerAlphaConstDisable

  Description:
  This function disables the layers alpha constant alpha function.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit    - It specifies the video output number.
  LayerNr - It specifies the layer from the following.
              R_VDCE_LAYER_SCALER1 / 
              R_VDCE_LAYER_IMAGE2  / 
              R_VDCE_LAYER_IMAGE3    

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerAlphaConstDisable(const uint32_t Unit,
                                             const uint32_t LayerNr);

/***************************************************************************
  Function: R_VDCE_LayerClutSet

  Description:
  This function updates and switches between these two CLUTs.
  The CLUT in VDCE is double buffered. One CLUT is shown and one can be written to.
  This function can be executed only once in Vsync period. 
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - It specifies the video output number.
  LayerNr  - It specifies the layer from the following.
               R_VDCE_LAYER_SCALER0 / 
               R_VDCE_LAYER_SCALER1 / 
               R_VDCE_LAYER_IMAGE2  / 
               R_VDCE_LAYER_IMAGE3    
  Offset   - ID of the first CLUT Entry to be written. Range is 0 to ClutSize -1.
  ClutSize - Number of Clut Entry to be written.
             When format is <R_VDCE_CLUT1>, range is 1 to 2.
             When format is <R_VDCE_CLUT4>, range is 1 to 16.
             When format is <R_VDCE_CLUT8>, range is 1 to 256.
  Clut     - The Color Look up Table.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_LayerClutSet(const uint32_t                    Unit,
                                   const uint32_t                    LayerNr,
                                   const uint32_t                    Offset,
                                   const uint32_t                    ClutSize,
                                   const r_vdce_ClutARGB32_t * const Clut);

/***************************************************************************
  Function: R_VDCE_LayerChromaKeyEnable

  Description:
  This function enables the layers Chroma key.
  If color format is YCbCr, this setting is invalid.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit      - It specifies the video output number.
  LayerNr   - It specifies the layer from the following.
                R_VDCE_LAYER_SCALER1 / 
                R_VDCE_LAYER_IMAGE2  / 
                R_VDCE_LAYER_IMAGE3    
  CkTarget  - The parameter specifies the color signal to process. Range changes by the format.
              (e.g. color format is ARGB4444, range of CkTarget.R,G,B is 0 to 15)
              When a format is CLUT8, CLUT4 or CLUT1, it sets an index to CkTarget.A and
              the range of CkTarget.R,G,B is only 0.
              When a format is RGB format of the other, the range of CkTarget.A is only 0.
  CkReplace - The parameter specifies the color signal to replace. Range changes by the format.
              (e.g. color format is ARGB4444, range of CkReplace.R,G,B is 0 to 15)
              When a format is CLUT8, CLUT4 or CLUT1, the range of CkReplace.A,R,G,B is 0 to 255.
              When a format is RGB format of the other, the range of CkReplace.A is 0 to 255.

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)

  Note:
  If chroma-key is enabled, following function is not effective in the layer.
  - alpha channel per pixel data
  - premultiplied alpha channel

*/

r_vdce_Error_t R_VDCE_LayerChromaKeyEnable(const uint32_t                  Unit,
                                           const uint32_t                  LayerNr,
                                           const r_vdce_CkARGB32_t * const CkTarget,
                                           const r_vdce_CkARGB32_t * const CkReplace);

/***************************************************************************
  Function: R_VDCE_LayerChromaKeyDisable

  Description:
  This function disables the layers Chroma key.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit      - It specifies the video output number.
  LayerNr   - It specifies the layer from the following.
                R_VDCE_LAYER_SCALER1 / 
                R_VDCE_LAYER_IMAGE2  / 
                R_VDCE_LAYER_IMAGE3    

  Return value:
  See <r_vdce_Error_t>.

  Executable Layer Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the layer status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_LayerChromaKeyDisable(const uint32_t Unit,
                                            const uint32_t LayerNr);


/***************************************************************************
  Group: Capture functions
*/

/***************************************************************************
  Function: R_VDCE_CapBufGeometrySetup

  Description:
  This function sets the parameters for the capture buffers and the size of the incoming video data.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - VDCE unit number to capture.
             In case of D1M2, Unit = 1 only available.
  Buffer1  - Address of the first buffer, which is used to capture the first field.
             Lower 7 bits are clamped to 0. If this is set to 0 then returns error.
  Buffer2  - Address of the second buffer, which is used to capture the second field.
             Lower 7 bits are clamped to 0. If this is set to 0 then Buffer2 = Buffer1.
  Stride   - When switching to the next line of the capture buffer, Stride pixels will be added.
             Range is 1 to 4096.
             Lower 7 bits of Stride multiplied by bytes per pixel are clamped to 0.
  Width    - The parameter specifies the width of the incoming video in pixels.
             Range is 4 to 2011. Lower 2 bits are clamped to 0.
             Range of sum of StartX and Width is 2015 and less.
             When Width is greater than 1024 pixels, horizontal image size must be reduced to 1024 
             pixels or less.
  Height   - The parameter specifies the Height of the incoming video in pixels.
             For an interlaced video, the height will be the height of one frame.
             Range is 4 to 1024. Lower 2 bits are clamped to 0.
             Range of sum of StartY and Height is 2039 and less.
  StartX   - The parameter specifies the synchronized mode and skipped pixels. 
             When StartX is set to the range of 16 to 2011, video input synchronize with Hsync signal 
             and StartX pixels are skipped after horizontal synchronization, before capturing starts. 
             When StartX is set to 0, video input synchronize with Data Enable signal and capture starts 
             from first cycle of data enable. Only D1M2x device support this mode and interlace input 
             can't be supported.
  StartY   - When StartY is set to the range of 4 to 2035, StartY+1 lines are skipped after vertical 
             synchronization, before capturing of the first field starts.
             When StartY is set to 0, no line is skipped. Only D1M2x device support this mode. 
             When video input synchronize with Data Enable signal (StartX = 0), StartY must be setting 
             to 0.
             When video data is input from MIPI in D1M2x, capturing should start from top/left screen if 
             StartX = 17 and StartY = 0 are set. 
             
  FBformat - It specifies the frame buffer format. See <r_vdce_Fbformat_t>.
                - R_VDCE_FB_FORMAT_YCBCR_422  
                - R_VDCE_FB_FORMAT_RGB565    (| R_VDCE_FB_RB_SWAP) 
                - R_VDCE_FB_FORMAT_RGB0888   (| R_VDCE_FB_RB_SWAP) 
                - R_VDCE_FB_FORMAT_YCBCR_444  

  Return value:
  See <r_vdce_Error_t>.

  Executable Capture Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the capture status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_CapBufGeometrySetup(const uint32_t          Unit,
                                          const uint32_t          Buffer1,
                                          const uint32_t          Buffer2,
                                          const uint32_t          Stride,
                                          const uint32_t          Width,
                                          const uint32_t          Height,
                                          const uint32_t          StartX,
                                          const uint32_t          StartY,
                                          const r_vdce_Fbformat_t FBformat);

/***************************************************************************
  Function: R_VDCE_CapModeSet

  Description:
  This function sets the capturing mode.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - VDCE unit number to capture.
         In case of D1M2, Unit = 1 only available.
  Mode - The following flags of type <r_vdce_CapMode_t> can be combined with '|'.
         
         One of these six flags must be selected.
           -  R_VDCE_CAP_MODE_YUV_ITU656
           -  R_VDCE_CAP_MODE_YUV_8BIT  
           -  R_VDCE_CAP_MODE_YUV_16BIT 
           -  R_VDCE_CAP_MODE_RGB_16BPP 
           -  R_VDCE_CAP_MODE_RGB_18BPP 
           -  R_VDCE_CAP_MODE_RGB_24BPP 
         
         The following flags are optional.
           -  R_VDCE_CAP_MODE_DITHER    
           -  R_VDCE_CAP_MODE_YUV_Y_UV_INVERT 
           -  R_VDCE_CAP_MODE_VSYNC_INVERT    
           -  R_VDCE_CAP_MODE_HSYNC_INVERT    
           -  R_VDCE_CAP_MODE_DATA_CLK_INVERT 
           -  R_VDCE_CAP_MODE_VSYNC_CLK_INVERT
           -  R_VDCE_CAP_MODE_HSYNC_CLK_INVERT
           -  R_VDCE_CAP_MODE_H_MIRRORING     
           -  R_VDCE_CAP_MODE_V_MIRRORING     
           -  R_VDCE_CAP_MODE_FIXED_VSYNC     

  Return value:
  See <r_vdce_Error_t>.

  Executable Capture Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective after the capture status is changed from Idle to Executing.)
*/

r_vdce_Error_t R_VDCE_CapModeSet(const uint32_t         Unit,
                                 const r_vdce_CapMode_t Mode);

/***************************************************************************
  Function: R_VDCE_CapBufSet

  Description:
  This function sets the parameters for the capture.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - VDCE unit number to capture.
             In case of D1M2, Unit = 1 only available.
  Buffer1  - Address of the first buffer, which is used to capture the first field.
             Lower 7 bits are clamped to 0. If this is set to 0 then returns error.
             This parameter is the same as Buffer1 of <R_VDCE_CapBufGeometrySetup>.
  Buffer2  - Address of the second buffer, which is used to capture the second field.
             Lower 7 bits are clamped to 0. If this is set to 0 then returns error.
             This parameter is the same as Buffer2 of <R_VDCE_CapBufGeometrySetup>.

  Return value:
  See <r_vdce_Error_t>.

  Executable Capture Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_CapBufSet(const uint32_t Unit,
                                const uint32_t Buffer1,
                                const uint32_t Buffer2);

/***************************************************************************
  Function: R_VDCE_CapBufFieldSetup1

  Description:
  This function sets the parameters for the capture.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - VDCE unit number to capture.
             In case of D1M2, Unit = 1 only available.
  Buffer1  - Address of the first buffer, which is used to capture the first field.
             Lower 7 bits are clamped to 0. If this is set to 0 then returns error.
             This parameter is the same as Buffer1 of <R_VDCE_CapBufGeometrySetup>.
  StartY   - When StartY is set to the range of 4 to 2035, StartY+1 lines are skipped after vertical 
             synchronization, before capturing of the first field starts.
             When StartY is set to 0, no line is skipped. Only D1M2x device support this mode. 
             This parameter is the same as StartY of <R_VDCE_CapBufGeometrySetup>.

  Return value:
  See <r_vdce_Error_t>.

  Executable Capture Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_CapBufFieldSetup1(const uint32_t Unit,
                                        const uint32_t Buffer1,
                                        const uint32_t StartY);

/***************************************************************************
  Function: R_VDCE_CapBufFieldSetup2

  Description:
  This function sets the parameters for the capture.
  This setting is valid until <R_VDCE_DeInit> is executed.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit     - VDCE unit number to capture.
             In case of D1M2, Unit = 1 only available.
  Buffer2  - Address of the second buffer, which is used to capture the second field.
             Lower 7 bits are clamped to 0.
             If this is set to 0 then Buffer2 = Buffer1 of <R_VDCE_CapBufGeometrySetup>.
             This parameter is the same as Buffer2 of <R_VDCE_CapBufGeometrySetup>.
  StartY   - When StartY is set to the range of 4 to 2035, StartY+1 lines are skipped after vertical 
             synchronization, before capturing of the first field starts.
             When StartY is set to 0, no line is skipped. Only D1M2x device support this mode. 
             This parameter is the same as StartY of <R_VDCE_CapBufGeometrySetup>.

  Return value:
  See <r_vdce_Error_t>.

  Executable Capture Status:
  Uninitialized - NG
  Initialized   - OK
  Idle          - OK
  Executing     - OK (Parameters become effective at the timing of next Vsync.)
*/

r_vdce_Error_t R_VDCE_CapBufFieldSetup2(const uint32_t Unit,
                                        const uint32_t Buffer2,
                                        const uint32_t StartY);

/***************************************************************************
  Function: R_VDCE_CapEnable

  Description:
  This function enables the video capturing.
  Execute <R_VDCE_CapBufGeometrySetup> and <R_VDCE_CapModeSet> before executing this function.
  If the function successfully execute, the return code will be <R_VDCE_ERR_OK> and
  the state will be in the executing state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.
  
  Capture data is connected to the following layer by conformation of geometry setup. 
  If OutputUnit is set as 0 or 1, this function enables the connecting layer. So Execute 
  <R_VDCE_LayerFormatSet>, <R_VDCE_LayerMemGeometrySet> and <R_VDCE_LayerViewPortSet>
  before executing this function. Layer control like a Scaling-down should be set to connecting 
  layer.
  > +---------------------------+------------------+
  > |  parameter setting        |                  |
  > +-----+---------------------+                  |
  > |Unit |     OutputUnit      | Connecting Layer |
  > |==============================================|
  > |  0  |         0           | Scaler0 of Unit0 |
  > |  1  |         1           | Scaler0 of Unit1 |
  > |  0  |         1           | Scaler1 of Unit1 |
  > |  1  |         0           | Scaler1 of Unit0 |
  > |  0  |R_VDCE_CAP_NO_DISPLAY| No connection    |
  > |  1  |R_VDCE_CAP_NO_DISPLAY| No connection    |
  > +-----+---------------------+------------------+
  
  If R_VDCE_CAP_NO_DISPLAY is specified, the capturing starts independently of the layer control and 
  displaying. So, layer control like a Scaling-down is not available. And, capture option of 
  R_VDCE_CAP_MODE_H_MIRRORING and R_VDCE_CAP_MODE_V_MIRRORING are not available too.
  
  
  Parameter:
  Unit       - VDCE unit number to capture.
               In case of D1M2, Unit = 1 only available.
  OutputUnit - VDCE unit number to display.
               R_VDCE_CAP_NO_DISPLAY is also selectable when capture is independently of displaying.
  
  Return value:
  See <r_vdce_Error_t>.

  Executable Capture Status:
  Uninitialized - NG
  Initialized   - NG (when OutputUnit is 0 or 1.) / OK (when OutputUnit is R_VDCE_CAP_NO_DISPLAY.)
  Idle          - OK [Capture status will be changed to "Executing". 
                      VDCE status will also be changed to "Executing" if VDCE status is "Idle". ]
  Executing     - OK (Do nothing)
*/

r_vdce_Error_t R_VDCE_CapEnable(const uint32_t Unit,
                                const uint32_t OutputUnit);

/***************************************************************************
  Function: R_VDCE_CapDisable

  Description:
  This function disables the video capturing.
  If the function successfully execute, the return code will be <R_VDCE_ERR_OK> and
  the state will be in the idle state.
  This API function uses <R_VDCE_Sys_Lock> and <R_VDCE_Sys_Unlock> to prevent multiple execution.
  When executing this API function by the interrupt handler, be careful of <R_VDCE_Sys_Lock> and
  <R_VDCE_Sys_Unlock> having to be able to be executed by the interrupt handler.

  Parameter:
  Unit - VDCE unit number to capture.
         In case of D1M2, Unit = 1 only available.

  Return value:
  See <r_vdce_Error_t>.

  Executable Capture Status:
  Uninitialized - OK (Do nothing)
  Initialized   - OK (Do nothing)
  Idle          - NG 
  Executing     - OK [Capture status will be changed to "Idle". 
                      VDCE status will also be changed to "Idle" if all block (layer, OIR and capture) status are "Idle".]
*/

r_vdce_Error_t R_VDCE_CapDisable(const uint32_t Unit);


#ifdef __cplusplus
}
#endif


#endif /* R_VDCE_API_H */

