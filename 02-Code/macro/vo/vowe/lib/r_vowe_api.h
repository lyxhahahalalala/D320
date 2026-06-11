/*
****************************************************************************
PROJECT : VOWE driver
FILE    : $Id: r_vowe_api.h 7904 2016-03-09 08:54:34Z florian.zimmermann $
============================================================================

DESCRIPTION
Driver for the VOWE(Video Output Warping Engine) macro
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
  Title: VOWE Driver API

  VOWE (Video Output Warping Engine) driver API

  An application using VOWE driver should include this header file.

  Note:
  If an OS is used the VOWE driver internally uses semaphores.
*/

#ifndef R_VOWE_API_H_
#define R_VOWE_API_H_

#ifdef __cplusplus
extern "C" {
#endif



/***************************************************************************
  Section: Global defines
*/
/***************************************************************************
  Group: Constants
*/

/***************************************************************************
  Constants: API Version

  This constant is the value which shows the version information of the VOWE driver.
  It refers at the <R_VOWE_VersionStringGet> function.

  Values:
  R_VOWE_VERSION_HI - This constant is the high byte of the version information.
                      It is major version information.
  R_VOWE_VERSION_LO - This constant is the low byte of the version information.
                      It is miner version information.
  High and Low number of the API version

*/
#define R_VOWE_VERSION_HI  1
#define R_VOWE_VERSION_LO  13


/***************************************************************************
  Constants: Interrupt Factor

  This definition describes the factor of callback.

  Values:
  R_VOWE_FACTOR_NON  - This definition specifies no factor.
  R_VOWE_FACTOR_TRA  - This definition specifies the TRA occurred. It means that
                       the warping correction processing was complete.
  R_VOWE_FACTOR_IER  - This definition specifies the IER occurred. It means that
                       VOWE H/W macro processed tha invalid display list (DL).
  R_VOWE_FACTOR_STOP - This definition specifies the the stop command finished and
                       the state is in the Idle state.

*/
#define R_VOWE_FACTOR_NON  (0x00000000uL)
#define R_VOWE_FACTOR_TRA  (0x00000001uL)
#define R_VOWE_FACTOR_IER  (0x00000002uL)
#define R_VOWE_FACTOR_STOP (0x00000008uL)


/***************************************************************************
  Constants: Pixel size

  This definition describes the pixel size.

  Values:
  R_VOWE_SIZE_OF_ARGB8888 - This definition specifies the pixel size of ARGB8888 format.
  R_VOWE_SIZE_OF_RGB565   - This definition specifies the pixel size of RGB565 format.
*/
#define R_VOWE_SIZE_OF_ARGB8888 (4)
#define R_VOWE_SIZE_OF_RGB565   (2)


/***************************************************************************
  Section: Global Types
*/
/***************************************************************************
  Group: Enumerations and Types
*/

/***************************************************************************
  Enum: r_vowe_Error_t
  Description:
  This type describes the error code of the VOWE driver.

  Values:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_NG              - The error out of the assumption.
  R_VOWE_ERR_PARAM_INCORRECT - A parameter provided to a function is incorrect.
                               ( It excludes the outside of the range )
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_RANGE_PARAM     - The parameter error in the range.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.
  R_VOWE_ERR_FATAL_HW        - Fatal error has occurred at H/W.
  R_VOWE_ERR_SYS_VDCE        - The error has occurred at the driver support function
                               of VDCE driver.
  R_VOWE_ERR_SYS_INTC        - The error has occurred at the driver support function
                               of INTC (interrupt contoroller).

*/
typedef enum
{
    R_VOWE_ERR_OK = 0,
    R_VOWE_ERR_NG,
    R_VOWE_ERR_PARAM_INCORRECT,
    R_VOWE_ERR_RANGE_UNIT,
    R_VOWE_ERR_RANGE_PARAM,
    R_VOWE_ERR_NOT_ACCEPTABLE,
    R_VOWE_ERR_FATAL_OS,
    R_VOWE_ERR_FATAL_HW,
    R_VOWE_ERR_SYS_VDCE,
    R_VOWE_ERR_SYS_INTC
} r_vowe_Error_t;


/***************************************************************************
  Typedef: r_vowe_SourceConfig_t
  Description:
  This type describes the structure of the source image information.

  Struct members:
  LineStartPos   - This member specifies the start position of source image.
                   In the detail of LineStartPosition, refer to VOWE of H/W manual.
                   (1 <= LineStartPos <= 1,019)
  SourceWidth    - This member specifies the source image width (pixel).
                   (4 <= SourceWidth <= 1,280)
  SourceHeight   - This member specifies the source image height (pixel).
                   (5 <= SourceHeight <= 1,024)
*/
typedef struct
{
    uint32_t LineStartPos;
    uint32_t SourceWidth;
    uint32_t SourceHeight;
} r_vowe_SourceConfig_t;


/***************************************************************************
  Typedef: r_vowe_DisplayList_t
  Description:
  This type describes the structure of the display list information.

  Struct members:
  DspList    - This is a pointer to the top address of the display list (DL).
               The address must be a multiple of 8 bytes.
  Size       - This member specifies the size of display list.
*/
typedef struct
{
    uint32_t * DspList;
    uint32_t   Size;
} r_vowe_DisplayList_t;


/***************************************************************************
  Enum: r_vowe_BufferMode_t
  Description:
  This type describes the work buffer mode.

  Values:
  R_VOWE_FRAME_BUFFER_MODE - This member specifies the frame buffer mode.
                             If using the WorkBuffferAddr of <r_vowe_DestConfig_t>
                             as the frame buffer, specify this mode.
  R_VOWE_RING_BUFFER_MODE  - This member specifies the ring buffer mode.
                             If using the WorkBuffferAddr of <r_vowe_DestConfig_t>
                             as the ring buffer, specify this mode.
*/
typedef enum
{
    R_VOWE_FRAME_BUFFER_MODE = 0,
    R_VOWE_RING_BUFFER_MODE
} r_vowe_BufferMode_t;


/***************************************************************************
  Enum: r_vowe_ColorFormat_t
  Description:
  This type describes the color format.

  Values:
  R_VOWE_FORMAT_ARGB8888 - This member specifies the ARGB8888 format.
  R_VOWE_FORMAT_RGB565   - This member specifies the RGB565 format.
*/
typedef enum
{
    R_VOWE_FORMAT_ARGB8888 = 0,
    R_VOWE_FORMAT_RGB565
} r_vowe_ColorFormat_t;


/***************************************************************************
  Enum: r_vowe_DestMode_t
  Description:
  This type describes the destination mode.

  Values:
  R_VOWE_DEST_MODE_NORMAL         - This member specifies the NORMAL mode.
  R_VOWE_DEST_MODE_SCREEN_SHOTS   - This member specifies the SCREEN SHOTS mode.
                                    A distortion image by VOWE is not output to 
                                    LCD panel. A distortion image data is only 
                                    extracted by the frame buffer.
*/
typedef enum
{
    R_VOWE_DEST_MODE_NORMAL = 0,
    R_VOWE_DEST_MODE_SCREEN_SHOTS
} r_vowe_DestMode_t;

/***************************************************************************
  Typedef: r_vowe_DestConfig_t
  Description:
  This type describes the structure of the destination buffer settings information.

  The application must allocate the work buffer to set to WorkBufferAddr of this structure.
  The top address of work buffer must be a multiple of 1024 bytes. Allocate the work buffer
  at the following size.

  * In case of the frame buffer mode (<R_VOWE_FRAME_BUFFER_MODE>)

      (code)
      Work buffer size (byte) = stride * source height * pixel size * 2

      stride = ((source width + (128 - 1)) / 128) * 128

      pixle size:
        - R_VOWE_FORMAT_ARGB8888: 4 bytes
        - R_VOWE_FORMAT_RGB565:   2 bytes.
      (end)

  * In case of the ring buffer mode (<R_VOWE_RING_BUFFER_MODE>)

      Work buffer size (byte) is 2 exponentiations.

      ex) The example with the work buffer size which can be chosen
      (code)
       :            :
      256 kbyte : 0x40000
      128 kbyte : 0x20000
       64 kbyte : 0x10000
       32 kbyte : 0x08000
       :            :
      (end)

  Struct members:
  WorkBufferMode  - This member specifeis the work buffer mode. See <r_vowe_BufferMode_t>.
  WorkBufferAddr  - This is a pointer to the top address of the work buffer.
  WorkBufferSize  - This member specifies the work buffer size.
  ColorFormat     - This member specifies the color format of output image.
                    See <r_vowe_ColorFormat_t>.
  DitherMode      - This member specifies the dither mode. If this member is setted true (R_TRUE),
                    dither is ON. If this member is setted false (R_FALSE), dither is OFF.
                    Note that if ColorFormat is <R_VOWE_FORMAT_ARGB8888>, this member is ignored.
  RingBufferDelay - This member specifies the number of output delay lines of VDCE.
                    Note that if WorkBufferMode is <R_VOWE_FRAME_BUFFER_MODE>, this member
                    is ignored.
                    (1 <= RingBufferDelay <= 255)
                    Recommended Delay:
                    RingBufferDelay = 16 + VerticalScreenTiming->BackPorch + Vertical Distortion Distance
                    BackPorch it the time after the Sync Pulse
                    
                    Vertical Blanking Pulse:
                    ____________|¯¯¯¯¯¯¯¯¯¯¯|___________
                    Front Porch   SyncWidth   BACK PORCH
                    
  DestMode        - This member specifies the destination mode.
                    See <r_vowe_DestMode_t>.
*/
typedef struct
{
    r_vowe_BufferMode_t   WorkBufferMode;
    uint8_t*              WorkBufferAddr;
    uint32_t              WorkBufferSize;
    r_vowe_ColorFormat_t  ColorFormat;
    uint32_t              DitherMode;
    uint32_t              RingBufferDelay;
    r_vowe_DestMode_t     DestMode;
} r_vowe_DestConfig_t;


/***************************************************************************
  Typedef: r_vowe_CallbackFunction_t
  Description:
  This type describes to callback function.
  and error.

  Struct members:
  Unit          - The parameter specifies the instance number.
  Factor        - This parameter is the interrupt factor.
  CallbackParam - This parameter is the CallbackParam which is specified the <R_VOWE_Open>
                  function.
*/
typedef void (*r_vowe_CallbackFunction_t)(
                                          const uint32_t Unit,
                                          const uint32_t Factor,
                                          const uint32_t CallbackParam);


/***************************************************************************
      Section: Global API functions
*/

/***************************************************************************
  Function: R_VOWE_Init

  Description:
  This function initializes the VOWE driver.

  <R_VOWE_Init> function shall be the first call and should be executed only one time
  without an intervening <R_VOWE_DeInit> function.
  If the function successfully execute, the return code will be <R_VOWE_ERR_OK> and
  the state will be in the Initialize state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Init(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_DeInit

  Description:
  This function de-initializes theVOWE driver.

  <R_VOWE_DeInit> function should be the last call after all VOWE driver related
  resources have been released.
  If the function successfully execute, the return code will be <R_VOWE_ERR_OK> and
  the state will be in the Uninitialize state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_DeInit(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Open

  Description:
  This function opens the VOWE driver.

  This functions sets the source image information, the display list (DL) information,
  the destination buffer information and the callback function information.
  If the function successfully execute, the return code will be <R_VOWE_ERR_OK> and the
  state will be in the Idle state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit          - The parameter specifies the instance number.
  SourceConfig  - The parameter specifies the source image information.
                  See <r_vowe_SourceConfig_t>.
  DisplayList   - The parameter specifies the display list information.
                  See <r_vowe_DisplayList_t>.
  DestConfig    - The parameter specifies the the destination buffer informatiom.
                  See <r_vowe_DestConfig_t>.
  CallbackFunc  - This parameter specifies the callback function that will be called when
                  the warping correction processing is finished or the error occurred.
                  If the callback function unnecessary, sets NULL (R_NULL).
                  See <r_vowe_CallbackFunction_t>.
  CallbackParam - This parameter specifies the value which is passed the callback function.
                  Use is freedom of an application layer. If that's unnecessary, set 0.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_PARAM_INCORRECT - A parameter provided to a function is incorrect.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Open(
                    const uint32_t                      Unit,
                    const r_vowe_SourceConfig_t * const SourceConfig,
                    const r_vowe_DisplayList_t  * const DisplayList,
                    const r_vowe_DestConfig_t   * const DestConfig,
                    const r_vowe_CallbackFunction_t     CallbackFunc,
                    const uint32_t                      CallbackParam);


/***************************************************************************
  Function: R_VOWE_Close

  Description:
  This function closes the VOWE driver.

  If the function successfully execute, the return code will be <R_VOWE_ERR_OK> and the
  state will be in the Initialize state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Close(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_Start

  Description:
  This function starts the warping correction.

  If the function successfully execute, the return code will be <R_VOWE_ERR_OK> and
  the state will be in the Execution state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit           - The parameter specifies the instance number.
  ContinuousMode - The parameter specifies the continuouse mode or one-shot mode.
                   If ContinuousMode is R_TURE, this driver processes the warping correction
                   processing every time the VSYNC interrupt occurs. The warping correction
                   processing is done until the <R_VOWE_Stop> function can be called.
                   If ContinuousMode is false (R_FALSE), this driver processes the warping correction
                   processing only once when VSYNC interrupt occurrs and this driver transfers
                   automatically to the Idle state. It does not need to call the <R_VOWE_Stop>
                   function.


  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_PARAM_INCORRECT - A parameter provided to a function is incorrect.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Start(const uint32_t Unit, const uint32_t ContinuousMode);


/***************************************************************************
  Function: R_VOWE_Stop

  Description:
  This function stops the warping correction.

  If the function successfully execute, the return code will be <R_VOWE_ERR_OK>. Whrn the warping
  correction processing stops, the callback function is called with <R_VOWE_FACTOR_STOP> and
  the state is in the Idle state.

  Note that this function can't call from interrupt handler.

  Note that when a callback function is not called with <R_VOWE_FACTOR_STOP> during
   2 VSYNC period after <R_VOWE_Stop> fuction execution, reset a system.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_Stop(const uint32_t Unit);


/***************************************************************************
  Function: R_VOWE_DLChange

  Description:
  This function changes the display list (DL).

  If the function successfully execute, the return code will be <R_VOWE_ERR_OK> and
  the new display list (DL) is reflected when <R_VOWE_Start> function is called.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit        - The parameter specifies the instance number.

  DisplayList - The parameter specifies the information of the display list.
                See <r_vowe_DisplayList_t>.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_PARAM_INCORRECT - A parameter provided to a function is incorrect.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.
  R_VOWE_ERR_NOT_ACCEPTABLE  - A function was called in an incorrect state.
  R_VOWE_ERR_FATAL_OS        - Fatal error has occurred at OS interface.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_DLChange(const uint32_t                     Unit,
                               const r_vowe_DisplayList_t * const DisplayList );


/***************************************************************************
  Function: R_VOWE_VersionStringGet

  Description:
  This fucntion returns the version string of this VOWE driver.

  Return value:
  the version string of this VOWE driver
*/
const uint8_t *R_VOWE_VersionStringGet(void);


/***************************************************************************
  Function: R_VOWE_MacroVersionGet

  Description:
  This function returns the major and minor version of the H/W macro.

  Parameter:
  Major   - This is a pointer to major version.

  Minor   - This is a pointer to minor version.

  Return value:
  R_VOWE_ERR_OK              - No error has occurred.
  R_VOWE_ERR_PARAM_INCORRECT - A parameter provided to a function is incorrect.
  R_VOWE_ERR_RANGE_UNIT      - The unit-number error.

  See <r_vowe_Error_t>.
*/
r_vowe_Error_t R_VOWE_MacroVersionGet(uint32_t *const Major,
                                      uint32_t *const Minor);


/****************************************************************************
  Function: R_VOWE_Isr0

  Description:
  This function is called from ISR (Interrupt Service Routines) and processes
  the interrupt factor.
  It depends on the CPU which is loaded into the device for ISR.

  Parameter:
  Unit     - The parameter specifies the instance number.

  Return value:
  None.
*/
void R_VOWE_Isr0(uint32_t Unit);


#ifdef __cplusplus
}
#endif


#endif /* R_VOWE_API_H_ */

