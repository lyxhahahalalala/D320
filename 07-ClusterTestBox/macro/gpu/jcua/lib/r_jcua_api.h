/*
****************************************************************************
PROJECT : JCUA driver
FILE    : $Id: r_jcua_api.h 6077 2015-09-24 14:13:12Z matthias.nippert $
============================================================================
DESCRIPTION
Driver for the JCUA macro
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
  Title: JCUA Driver API
  
  JCUA (JPEG Codec Unit A) driver API

  An application using JCUA driver should include this header file.

  Note:
  If an OS is used the JCUA driver internally uses semaphores. 

*/

#ifndef R_JCUA_API_H
#define R_JCUA_API_H

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
  Constants: R_JCUA_DECODE_OPTION

  This constant is the validity of optional decode functions in
  the <R_JCUA_DecoderStart> function.
 
  R_JCUA_DECODE_OPTION_NONE          - All optional decode functions  are ineffective.
  R_JCUA_DECODE_OPTION_POSITION      - Coordinate designation is effective.
                                      See <r_jcua_Coordinate_t>.
  R_JCUA_DECODE_OPTION_PARAM         - Additional decoding function is effective.
                                      See <r_jcua_DecodeParam_t>.
  R_JCUA_DECODE_OPTION_DIVISION_MODE - Division decoding  function is effective.
                                      See <r_jcua_DivisionModeParam_t>.
*/

#define R_JCUA_DECODE_OPTION_NONE          (0x00uL)
#define R_JCUA_DECODE_OPTION_POSITION      (0x01uL)
#define R_JCUA_DECODE_OPTION_PARAM         (0x02uL)
#define R_JCUA_DECODE_OPTION_DIVISION_MODE (0x04uL)

/***************************************************************************
  Constants: R_JCUA_VERSION

  This constant is the value which shows the version information of the JCUA driver.
  It refers at the <R_JCUA_VersionStringGet> function.

  R_JCUA_VERSION_HI - This constant is the high byte of the version information.
                      It is major version information.
  R_JCUA_VERSION_LO - This constant is the low byte of the version information.
                      It is minor version information.
*/

#define R_JCUA_VERSION_HI  1
#define R_JCUA_VERSION_LO  12

/***************************************************************************
  Constants: R_JCUA_STRIDE_AUTO

  If decoding proceeds while the setting the Stride member of the 
  <r_jcua_FrameBuffer_t> structure is <R_JCUA_STRIDE_AUTO> (=0),
  the JCUA driver automatically calculates the size of the stride
  which is required for output of the image.

  R_JCUA_STRIDE_AUTO    - Stride is calculated automatically.
*/

#define R_JCUA_STRIDE_AUTO          (0)

/***************************************************************************
  Section: Global Types
*/
/***************************************************************************
  Group: Enumerations and Types
*/

/***************************************************************************
  Enum: r_jcua_Error_t

  Description:
  JCUA driver error code.
  
  If an error occurs these enumerations give information about the
  reason.

  This list can be extended, it is an example only. 
  The error code is used internally by the driver when 
  calling the central error handler. 
  The central error handler will pass the error code to the 
  application call-back function, if it has been set. 
  
  An API function may also return an error code.

  Values:
  R_JCUA_ERR_OK                   - No error has occured.
  R_JCUA_ERR_NG                   - An error has occurred, but no specific
                                   error code is defined for it.
  R_JCUA_ERR_PARAM_INCORRECT      - A parameter provided to a function is incorrect.
  R_JCUA_ERR_RANGE_UNIT           - An instance number is the outside of the range.
  R_JCUA_ERR_RANGE_PARAM          - A parameter is the outside of the range.
  R_JCUA_ERR_NOT_ACCEPTABLE       - A function was called in an incorrect state.
  R_JCUA_ERR_FATAL_OS             - Fatal error has occurred at OS interface.
  R_JCUA_ERR_FATAL_HW             - Fatal error has occurred at H/W.
  R_JCUA_ERR_BUS_TIMEOUT          - Timeout error has occurred at bus reset.
*/

typedef enum
{
    R_JCUA_ERR_OK = 0,
    R_JCUA_ERR_NG,
    R_JCUA_ERR_PARAM_INCORRECT,
    R_JCUA_ERR_RANGE_UNIT,
    R_JCUA_ERR_RANGE_PARAM,
    R_JCUA_ERR_NOT_ACCEPTABLE,
    R_JCUA_ERR_FATAL_OS,
    R_JCUA_ERR_FATAL_HW,
    R_JCUA_ERR_BUS_TIMEOUT
} r_jcua_Error_t;


/***************************************************************************
  Enum: r_jcua_CallbackReason_t

  Description:
  This type describes the callback reason.The value of the logical sum is used.
 
  Values:
  R_JCUA_CB_FACTOR_NONE                 - None.
  R_JCUA_CB_FACTOR_DECODE_COMPLETE      - The decode processing completed.
                                          After the decoding process has been completed, 
                                          when other callback has occurred.
  R_JCUA_CB_FACTOR_DECODE_INPUT_PAUSED  - The decode processing paused. (by the interrupt
                                               which input side division decoding).
  R_JCUA_CB_FACTOR_DECODE_OUTPUT_PAUSED - The decode processing paused. (by the interrupt
                                               which output side division decoding).
  R_JCUA_CB_FACTOR_DECODE_SIZEOVER      - The decode processing was failed. (the size of
                                               the compressed JPEG data is too large).
  R_JCUA_CB_FACTOR_DECODE_ERRORED       - The decode processing was failed. (by the
                                               interrupt which decode error occurred).
  R_JCUA_CB_FACTOR_FATAL_ERROR          - The fatal error occurred.
*/

typedef enum
{
    R_JCUA_CB_FACTOR_NONE                 = 0x00000000,
    R_JCUA_CB_FACTOR_DECODE_COMPLETE      = 0x00000001,
    R_JCUA_CB_FACTOR_DECODE_INPUT_PAUSED  = 0x00000002,
    R_JCUA_CB_FACTOR_DECODE_OUTPUT_PAUSED = 0x00000004,
    R_JCUA_CB_FACTOR_DECODE_SIZEOVER      = 0x01000008,
    R_JCUA_CB_FACTOR_DECODE_ERRORED       = 0x01000010,
    R_JCUA_CB_FACTOR_FATAL_ERROR          = 0x01000020
} r_jcua_CallbackReason_t;


/***************************************************************************
  Enum: r_jcua_PauseReason_t

  Description:
  This type describes the pause reason.
 
  Values:
  R_JCUA_PAUSE_FACTOR_INPUT_PAUSED   - The decode processing paused. (by the interrupt
                                       which input side division decoding).
  R_JCUA_PAUSE_FACTOR_OUTPUT_PAUSED  - The decode processing paused. (by the interrupt
                                       which output side division decoding).
*/

typedef enum
{
    R_JCUA_PAUSE_FACTOR_INPUT_PAUSED   = 0x00000001,
    R_JCUA_PAUSE_FACTOR_OUTPUT_PAUSED  = 0x00000002
} r_jcua_PauseReason_t;

/***************************************************************************
  Enum: r_jcua_JpegFormat_t

  Description:
  This type describes the pixel format of compressed JPEG data that is decoded.
 
  Values:
  R_JCUA_JPEG_FORMAT_YCBCR444 - YCbCr444.
  R_JCUA_JPEG_FORMAT_YCBCR422 - YCbCr422.
  R_JCUA_JPEG_FORMAT_YCBCR420 - YCbCr420.
  R_JCUA_JPEG_FORMAT_YCBCR411 - YCbCr411.
*/

typedef enum
{
    R_JCUA_JPEG_FORMAT_YCBCR444 = 0x00,
    R_JCUA_JPEG_FORMAT_YCBCR422 = 0x01,
    R_JCUA_JPEG_FORMAT_YCBCR420 = 0x02,
    R_JCUA_JPEG_FORMAT_YCBCR411 = 0x06
} r_jcua_JpegFormat_t;


/***************************************************************************
  Enum: r_jcua_OutputFormat_t

  Description:
  This type describes the output pixel format of RAW image data.
 
  Values:
  R_JCUA_OUTPUT_FORMAT_YCBCR422 - YCbCr422.
  R_JCUA_OUTPUT_FORMAT_ARGB8888 - ARGB8888.
  R_JCUA_OUTPUT_FORMAT_RGB565   - RGB565.
*/

typedef enum
{
    R_JCUA_OUTPUT_FORMAT_YCBCR422 = 0x00,
    R_JCUA_OUTPUT_FORMAT_ARGB8888 = 0x01,
    R_JCUA_OUTPUT_FORMAT_RGB565   = 0x02
} r_jcua_OutputFormat_t;


/***************************************************************************
  Enum: r_jcua_SubSampling_t

  Description:
  This type describes the sub samples of the decoded image data.
 
  Values:
  R_JCUA_SUB_SAMPLING_1_1  - No subsampling.
  R_JCUA_SUB_SAMPLING_1_2  - Subsamples output data into 1/2.
  R_JCUA_SUB_SAMPLING_1_4  - Subsamples output data into 1/4.
  R_JCUA_SUB_SAMPLING_1_8  - Subsamples output data into 1/8.
*/

typedef enum
{
    R_JCUA_SUB_SAMPLING_1_1  = 0x00,
    R_JCUA_SUB_SAMPLING_1_2  = 0x01,
    R_JCUA_SUB_SAMPLING_1_4  = 0x02,
    R_JCUA_SUB_SAMPLING_1_8  = 0x03
} r_jcua_SubSampling_t;


/***************************************************************************
  Enum: r_jcua_ErrorDetail_t

  Description:
  This type describes the error classification of the JCUA driver decoding failure.
  See <R_JCUA_ErrorInfoGet>
  
  Values:
  R_JCUA_ERR_JDC_OK                   - No error has occurred.
  R_JCUA_ERR_JDC_SOI_NOT_FOUND        - SOI not detected: SOI not detected until EOI detected.
  R_JCUA_ERR_JDC_INVALID_SOF          - SOF1 to SOFF detected.
  R_JCUA_ERR_JDC_UNPROVIDED_SOF       - Unprovided pixel format detected.
  R_JCUA_ERR_JDC_SOF_ACCURACY         - SOF accuracy error: Other than 8 detected.
  R_JCUA_ERR_JDC_DQT_ACCURACY         - DQT accuracy error: Other than 0 detected.
  R_JCUA_ERR_JDC_COMPONENT_1          - The number of SOF0 header components detected is other
                                        than 1, 3, or 4.
  R_JCUA_ERR_JDC_COMPONENT_2          - The number of components differs between SOF0 header
                                        and SOS.
  R_JCUA_ERR_JDC_NO_SOF0_DQT_DHT      - SOF0, DQT, and DHT not detected when SOS detected.
  R_JCUA_ERR_JDC_SOS_NOT_FOUND        - SOS not detected: SOS not detected until EOI detected.
  R_JCUA_ERR_JDC_EOI_NOT_FOUND        - EOI not detected (default).
  R_JCUA_ERR_JDC_RESTART_INTERVAL_NUM - Restart interval data number error detected.
  R_JCUA_ERR_JDC_IMAGE_SIZE           - Image size error detected. There is a possibility that the
                                        stride value which is set to Stride member of <r_jcua_FrameBuffer_t>
                                        structure is wrong or the size of frame buffer is smaller
                                        than decoding image size. If this error occurs, sets a right
                                        value to Stride member of <r_jcua_FrameBuffer_t> structure or
                                        re-allocate the frame buffer, then calls the <R_JCUA_DecoderStart>
                                        function.
  R_JCUA_ERR_JDC_LAST_MCU_NUM         - Last MCU data number error detected.
  R_JCUA_ERR_JDC_BLOCK_NUM            - Block data number error detected.
  R_JCUA_ERR_JDC_UNKNOWN_FATAL_ERROR  - The fatal error occurred. There is a possibility that a memory
                                        is broken or the LSI is hung-up. If this error occurs, reset the LSI.

*/

typedef enum
{
    R_JCUA_ERR_JDC_OK                   = 0x00,
    R_JCUA_ERR_JDC_SOI_NOT_FOUND        = 0x01,
    R_JCUA_ERR_JDC_INVALID_SOF          = 0x02,
    R_JCUA_ERR_JDC_UNPROVIDED_SOF       = 0x03,
    R_JCUA_ERR_JDC_SOF_ACCURACY         = 0x04,
    R_JCUA_ERR_JDC_DQT_ACCURACY         = 0x05,
    R_JCUA_ERR_JDC_COMPONENT_1          = 0x06,
    R_JCUA_ERR_JDC_COMPONENT_2          = 0x07,
    R_JCUA_ERR_JDC_NO_SOF0_DQT_DHT      = 0x08,
    R_JCUA_ERR_JDC_SOS_NOT_FOUND        = 0x09,
    R_JCUA_ERR_JDC_EOI_NOT_FOUND        = 0x0A,
    R_JCUA_ERR_JDC_RESTART_INTERVAL_NUM = 0x0B,
    R_JCUA_ERR_JDC_IMAGE_SIZE           = 0x0C,
    R_JCUA_ERR_JDC_LAST_MCU_NUM         = 0x0D,
    R_JCUA_ERR_JDC_BLOCK_NUM            = 0x0E,
    R_JCUA_ERR_JDC_UNKNOWN_FATAL_ERROR  = 0x0F
} r_jcua_ErrorDetail_t;

/***************************************************************************
  Enum: r_jcua_Swap_t

  Description:
  This type describes the data swap setting in memory. The data is swapped every 8 bytes.
  The data image in memory assume "(1)-(2)-(3)-(4)-(5)-(6)-(7)-(8)" before swapping data.

  Values:
  R_JCUA_SWAP_NONE                   - No swap.(1)-(2)-(3)-(4)-(5)-(6)-(7)-(8)
  R_JCUA_SWAP_BYTE                   - Byte swap.(2)-(1)-(4)-(3)-(6)-(5)-(8)-(7)
  R_JCUA_SWAP_WORD                   - Word swap.(3)-(4)-(1)-(2)-(7)-(8)-(5)-(6)
  R_JCUA_SWAP_WORD_AND_BYTE          - Word-byte swap.(4)-(3)-(2)-(1)-(8)-(7)-(6)-(5)
  R_JCUA_SWAP_LONG                   - Longword swap.(5)-(6)-(7)-(8)-(1)-(2)-(3)-(4)
  R_JCUA_SWAP_LONG_AND_BYTE          - Longword-byte swap.(6)-(5)-(8)-(7)-(2)-(1)-(4)-(3)
  R_JCUA_SWAP_LONG_AND_WORD          - Longword-word swap.(7)-(8)-(5)-(6)-(3)-(4)-(1)-(2)
  R_JCUA_SWAP_LONG_AND_WORD_AND_BYTE - Longword-word-byte swap.(8)-(7)-(6)-(5)-(4)-(3)-(2)-(1)
*/

typedef enum
{
    R_JCUA_SWAP_NONE                   = 0x00,
    R_JCUA_SWAP_BYTE                   = 0x01,
    R_JCUA_SWAP_WORD                   = 0x02,
    R_JCUA_SWAP_WORD_AND_BYTE          = 0x03,
    R_JCUA_SWAP_LONG                   = 0x04,
    R_JCUA_SWAP_LONG_AND_BYTE          = 0x05,
    R_JCUA_SWAP_LONG_AND_WORD          = 0x06,
    R_JCUA_SWAP_LONG_AND_WORD_AND_BYTE = 0x07
} r_jcua_Swap_t;


/***************************************************************************
  typedef: r_jcua_CallbackFunction_t

  Description:  
  This type describes the callback function.

*/

typedef void (*r_jcua_CallbackFunction_t)(uint32_t Unit,
                                          r_jcua_CallbackReason_t Factor,
                                          uint32_t Param);


/***************************************************************************
  typedef: r_jcua_Coordinate_t

  Description:  
  This type describes the position.
  
  Struct members:
  PosX - The horizontal position.
  PosY - The vertical position.
*/

typedef struct
{
    uint32_t        PosX;
    uint32_t        PosY;
} r_jcua_Coordinate_t;


/***************************************************************************
  typedef: r_jcua_FrameBuffer_t

  Description:  
  This type describes the frame buffer information that is 
  an output destination of data which decoded compressed
  JPEG data.
  
  Struct members:
  Address - The address of frame buffer. This address must be a multiple of 8 bytes.
  Size    - The size of frame buffer. This address must be a multiple of 8 bytes.
            This value must match the complete decoded image size, also if output division mode is used.
  Stride  - The stride of frame buffer. This stride must be a multiple of 8 bytes.
            If the stride sets to zero, that is reset to number which decoded
            compressed JPEG data just fits. 
  Format  - The output pixel format of frame buffer. See <r_jcua_OutputFormat_t>.
  Swap    - The data swap setting in memroy. See <r_jcua_Swap_t>.
*/

typedef struct
{
    void*                  Address;
    uint32_t               Size;
    int16_t                Stride;
    r_jcua_OutputFormat_t  Format;
    r_jcua_Swap_t          Swap;
} r_jcua_FrameBuffer_t;


/***************************************************************************
  typedef: r_jcua_DecodeParam_t

  Description:  
  This type describes the decode with subsample parameter information.
  
  Struct members:
  VertlSubSampling - Horizontal Subsampling. See <r_jcua_SubSampling_t>.
  HorizSubSampling - Vertical Subsampling. See <r_jcua_SubSampling_t>.
  Alpha            - Alpha value (0 to 255). If the output pixel format of RAW image data
                     isn't ARGB8888, the alpha value has to be zero.

*/

typedef struct
{
    r_jcua_SubSampling_t     VertlSubSampling;
    r_jcua_SubSampling_t     HorizSubSampling;
    uint8_t                  Alpha;
} r_jcua_DecodeParam_t;


/***************************************************************************
  typedef: r_jcua_DivisionBufferInfo_t

  Description:  
  This type describes the division decoding buffer information. 
  
  Struct members:
  IsEnable       - true (R_TRUE):   Enable the input/output buffer division decoding.
                   false (R_FALSE): Disable the input/output buffer division decoding.
  IsInitAddress  - true (R_TRUE):   When decoding paused, the input/output buffer address is
                                    initialized by restart_address.
                   false (R_FALSE): When decoding paused, the input/output buffer address isn't
                                    initialized.
  RestartAddress - If "IsInitAddress" is "true (R_TRUE)", the input/output buffer address is
                   initialized by this "RestartAddress".
  DataCount      - Division decoding count. In case that the input buffer, the "DataCount" must
                   be a multiple of 8 bytes. In case that the output buffer, the "DataCount" must
                   be a multiple of 16 lines.
*/

typedef struct
{
    uint8_t     IsEnable;
    uint8_t     IsInitAddress;
    uint32_t*   RestartAddress;
    uint32_t    DataCount;
} r_jcua_DivisionBufferInfo_t;


/***************************************************************************
  typedef: r_jcua_DivisionModeParam_t

  Description:  
  This type describes the division decoding parameter information. 
  
  Struct members:
  InputBuffer  - Input buffer information of division decoding.
  OutputBuffer - Output buffer information of division decoding.
*/

typedef struct
{
    r_jcua_DivisionBufferInfo_t InputBuffer;
    r_jcua_DivisionBufferInfo_t OutputBuffer;
} r_jcua_DivisionModeParam_t;


/***************************************************************************
  typedef: r_jcua_ImageInfo_t

  Description:  
  This type describes the decoded JPEG image information.
  
  Struct members:
  Width          - The width of the original compressed JPEG data is stored.
  Height         - The height of the original compressed JPEG data is stored.
  DecodeWidth    - The width of the decoded RAW image is stored. The value is multiple of MCU.
                   Meaning of MCU is "Minimum coded unit"(e.g. MCU size of YCbCr422 is 16x8).
  DecodeHeight   - The height of the decoded RAW image is stored. The value is multiple of MCU.
  Stride         - The stride(byte) of the frame buffer.
  StridePixel    - The stride(pixel) of the frame buffer.
  EncodedFormat  - The pixel format of compressed JPEG data.
*/

typedef struct
{
    uint32_t            Width;
    uint32_t            Height;
    uint32_t            DecodeWidth;
    uint32_t            DecodeHeight;
    uint32_t            Stride;
    uint32_t            StridePixel;
    r_jcua_JpegFormat_t EncodedFormat;
} r_jcua_ImageInfo_t;


/***************************************************************************
  typedef: r_jcua_DecodeSetting_t

  Description:  
  This type describes the decode parameter information.
  
  Struct members:
  OptionFlag   - The flag indicates the validity of optional decode functions in the
                 <R_JCUA_DecoderStart> function. See <R_JCUA_DECODE_OPTION>.
                 This flag can set the value of the logical sum of <R_JCUA_DECODE_OPTION>.
  Position     - The address of output position information of decompressed JPEG data.
                 When the decompressed JPEG data overflowed the frame buffer,
                 an error has occurred. See <r_jcua_Coordinate_t>.
  Parameter    - Additional decoding function information. See <r_jcua_DecodeParam_t>.
  DivisionMode - Division decoding function information. See <r_jcua_DivisionModeParam_t>.
  ImgInfo      - The pointer to variable of compressed JPEG data information. The entity 
                 of pointer must be kept till decode is finished.
                 If compressed JPEG data information is unnecessary, set NULL (R_NULL).

*/
typedef struct
{
    uint32_t                       OptionFlag;
    r_jcua_Coordinate_t            Position;
    r_jcua_DecodeParam_t           Parameter;
    r_jcua_DivisionModeParam_t     DivisionMode;
    r_jcua_ImageInfo_t*            ImgInfo;
} r_jcua_DecodeSetting_t;
    

/***************************************************************************
  Section: Global API Functions
*/


/***************************************************************************
  Function: R_JCUA_Init

  Description:
  This function initializes the JCUA driver.
  Before this driver initialization, the system function <R_JCUA_Sys_Init> is called.
  The <R_JCUA_Sys_Init> function sets the priority of interrupt, and conducts processing for
  system environment.
  If the function successfully execute, the return code will be <R_JCUA_ERR_OK> and the state
  will be in the Initialized state. If this function execute in the state which is not the
  Uninitialized state, the return code will be <R_JCUA_ERR_NOT_ACCEPTABLE>.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit  - The parameter specifies the instance number.
 
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Init(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_DeInit

  Description:
  This function de-initializes the JCUA driver. 
  After this driver de-initialization, the system function <R_JCUA_Sys_DeInit> is called.
  The <R_JCUA_Sys_DeInit> function clears the priority of interrupt, and conducts processing for
  system environment.
  If the function successfully execute, the return code will be <R_JCUA_ERR_OK> and the state will
  be in the Uninitialized state. If this function execute in the state which is not the Initialized
  state, the return code will be <R_JCUA_ERR_NOT_ACCEPTABLE>. If this API function is called in the
  Initialized state or the Executing (decode-paused) state, this function conducts de-initialize
  and the return code will be <R_JCUA_ERR_OK>. 

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_DeInit(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_DecoderOpen

  Description:
  This function opens the JCUA driver by specified operation mode and registers the callback
  function. 
  When decoding is completed, failed, or paused, the registered callback function is called.
  See <r_jcua_CallbackReason_t>.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit             - The parameter specifies the instance number.
  CallbackFunction - The parameter specifies the pointer to callback function.
  CallbackParam    - This parameter specifies the value which is passed the callback function.
  
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_DecoderOpen(const uint32_t Unit,
                                  const r_jcua_CallbackFunction_t CallbackFunction,
                                  const uint32_t CallbackParam);


/***************************************************************************
  Function: R_JCUA_DecoderClose

  Description:
  This function closes the JCUA driver by specified operation mode and unregisters the callback
  function. 

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit  - The parameter specifies the instance number.

  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_DecoderClose(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_DecoderStart

  Description:
  This function begins decoding the compressed JPEG data. 
  This function sets the decode parameters and starts the decode processing. 
  If this function execute in the state which is not the Idle state, the return code will be
  <R_JCUA_ERR_NOT_ACCEPTABLE>. If any decoding error (the callback reason is <R_JCUA_CB_FACTOR_DECODE_ERRORED>,
  <R_JCUA_CB_FACTOR_DECODE_SIZEOVER> or <R_JCUA_CB_FACTOR_FATAL_ERROR>) occurred after calling
  this function, the JCUA driver must re-do from the Initialization (<R_JCUA_Init> function).

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit          - The parameter specifies the instance number.
  SourceAddress - The parameter specifies the address of decoding compressed JPEG data. This
                  address must be a multiple of eight bytes.
  FrameBuffer   - The parameter specifies the address of frame buffer information.
                  See <r_jcua_FrameBuffer_t>. 
  Decode        - The parameter specifies the address of optional decode function information.
                  See <r_jcua_DecodeSetting_t>.
  
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_DecoderStart(const uint32_t Unit,
                                   const uint8_t* const SourceAddress,
                                   const r_jcua_FrameBuffer_t* const FrameBuffer,
                                   r_jcua_DecodeSetting_t* const Decode);



/***************************************************************************
  Function: R_JCUA_DecoderContinue

  Description:
  This function continues the decode processing. 
  If the decode processing is failed, this function returns error code.
  If the function successfully execute, the return code will be <R_JCUA_ERR_OK>. If this function
  execute in the state which is not the Executing (decode-paused) state, the return code will
  be <R_JCUA_ERR_NOT_ACCEPTABLE>.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit   - The parameter specifies the instance number.
  Factor - The parameter specifies the pause factor.

  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_DecoderContinue(const uint32_t Unit, const r_jcua_PauseReason_t Factor);


/***************************************************************************
  Function: R_JCUA_ErrorInfoGet

  Description:
  This function gets the error information. 
  When an error occurred after call the <R_JCUA_DecoderStart> function or
  the <R_JCUA_DecoderContinue> function, it's possible to confirm error cause.
  If the function successfully execute, the return code will be <R_JCUA_ERR_OK>. If this
  function execute in the state which is not the Idle state, the return code will be
  <R_JCUA_ERR_NOT_ACCEPTABLE>.

  Parameter:
  Unit  - The parameter specifies the instance number.
 
  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_ErrorInfoGet(const uint32_t Unit, r_jcua_ErrorDetail_t* const ErrorCode);


/***************************************************************************
  Function: R_JCUA_VersionStringGet

  Description:
  This function returns the version string of this JCUA driver. 

  Parameter:
  none
  
  Return value:
  Pointer of string.
*/
const uint8_t* R_JCUA_VersionStringGet(void);


/***************************************************************************
  Function: R_JCUA_MacroVersionGet

  Description:
  This function returns the major and minor version of the macro.

  Parameter:
  Major - This is a pointer to the major version.
  Minor - This is a pointer to the minor version.

  Return value:
  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_MacroVersionGet(uint32_t* const Major, uint32_t* const Minor);


/***************************************************************************
  Interrupt functions
*/

/***************************************************************************
  Function: R_JCUA_IsrStop

  Description:
  This function is executed in response to decompression processing interrupt
  (JEDI interrupt) requests from the JCUA H/W macro. Set calling of this function
  from the JEDI interrupt handler.
  Regarding the JEDI interrupt, refer to the user's manual for the device you are using.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  none. 
*/
void R_JCUA_IsrStop(uint32_t Unit);

/***************************************************************************
  Function: R_JCUA_IsrFinish

  Description:
  This function is executed in response to the data transfer processing interrupt
  (JDTI interrupt). Set up calling of this function from the JDTI interrupt handler. 
  Regarding the JDTI interrupt, refer to the user's manual for the device you are using.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  none. 
*/
void R_JCUA_IsrFinish(uint32_t Unit);

#ifdef __cplusplus
}
#endif


#endif /* R_JCUA_API_H */

