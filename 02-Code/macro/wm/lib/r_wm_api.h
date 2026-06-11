/*
****************************************************************************
PROJECT : WM driver
FILE    : $Id: r_wm_api.h 7909 2016-03-09 10:08:44Z matthias.nippert $
============================================================================ 
DESCRIPTION
Driver for the Window Manager
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

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

***************************************************************************
*/


#ifndef R_WM_API_H
#define R_WM_API_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: WM API
  
  Window Manager API

*/

/***********************************************************
  Constants: API Version

  High and Low number of the API version

  - R_WM_VERSION_HI
  - R_WM_VERSION_LO
*/

#define R_WM_VERSION_HI  1
#define R_WM_VERSION_LO  16


/***********************************************************
  Section: Global Types
*/

    
/***********************************************************
  Enum: r_wm_Error_t

  Description:
  WM driver error code.
  
  If an error occurs these enumerations give information about the
  reason.

*/

typedef enum
{
    R_WM_ERR_OK                                                     = 0,
    R_WM_ERR_NG                                                     = 1,
    R_WM_ERR_RANGE_WM                                               = 2,
    R_WM_ERR_PARAM_INCORRECT                                        = 3,
    R_WM_UNIT_LOCKED                                                = 4,
    R_WM_UNIT_NOTLOCKED                                             = 5,
    R_WM_ERR_NO_PHYS_WINDOW                                         = 6,
    R_WM_ERR_MALLOC_FAILED                                          = 7,
    R_WM_ERR_FREE_FAILED                                            = 8,
    R_WM_ERR_EVENT_FAILED                                           = 9,
    R_WM_ERR_VOUT                                                   = 10,
    R_WM_ERR_NOT_DISABLED                                           = 11,
    R_WM_ERR_NOT_DELETED                                            = 12,
    R_WM_ERR_COLORFMT                                               = 13,
    R_WM_ERR_VIN                                                    = 14,
    R_WM_ERR_NOT_SPRITE_WINDOW                                      = 15,
    R_WM_ERR_DISPLAY_TIMING_SET                                     = 16,
    R_WM_ERR_INVALID_WM_UNIT                                        = 17,
    R_WM_ERR_DEV_DEINIT_FAILED                                      = 18,
    R_WM_ERR_NULL_PTR_ARGUMENT                                      = 19,
    R_WM_ERR_SPRITE_NOT_FOUND                                       = 20,
    R_WM_ERR_SYS_LAYER_INIT_FAILURE                                 = 21,
    R_WM_ERR_NOT_CLUT_WIN_FMT                                       = 22,
    R_WM_ERR_NO_SUCH_WINDOW                                         = 23,
    R_WM_ERR_COULD_NOT_ENABLE_SCREEN                                = 24,
    R_WM_ERR_COULD_NOT_DISABLE_SCREEN                               = 25,
    R_WM_ERR_COULD_NOT_SET_SCREEN_BG_COLOR                          = 26,
    R_WM_ERR_COULD_NOT_REGISTER_EVENT                               = 27,
    R_WM_ERR_NOT_FB_WINDOW                                          = 28,
    R_WM_ERR_SYNC_MODE_NOT_POSSIBLE                                 = 29,
    R_WM_ERR_SYS_WIN_ALPHA_SET_FAILED                               = 30,
    R_WM_ERR_SYS_WIN_DELETE_ALL_SPRITES_FAILED                      = 31,
    R_WM_ERR_NOT_SUITABLE_CAPTURE_WINDOW                            = 32,
    R_WM_ERR_COULD_NOT_WRITE_MSG_QUEUE                              = 33,
    R_WM_ERR_SYS_CAPTURE_CREATE_FAILED                              = 34,
    R_WM_ERR_SYS_CAPTURE_DELETE_FAILED                              = 35,
    R_WM_ERR_SYS_CAPTURE_ENABLE_FAILED                              = 36,
    R_WM_ERR_CAPTURE_UNIT_COUNT_EXCEEDED                            = 37,
    R_WM_ERR_VOUT_INTERNAL                                          = 38,
    R_WM_ERR_SCREEN_TIMING_NOT_SET                                  = 39,
    R_WM_ERR_NOT_INITIALIZED                                        = 40,
    R_WM_ERR_COLOR_KEYING_NOT_SUPPORTED_FOR_LAYER                   = 41,
    R_WM_ERR_ADDRESS_ALIGNMENT                                      = 42,
    R_WM_ERR_DISPLAY_OUTPUT_FORMAT_SET                              = 43,
    R_WM_ERR_SYS_WIN_SWAP_FAILED                                    = 44,
    R_WM_ERR_SYS_WIN_DELETE_FAILED                                  = 45,
    R_WM_ERR_SYS_WIN_EXTERNAL_BUF_SET_FAILED                        = 46,
    R_WM_ERR_SYS_WIN_CREATE_FAILED                                  = 47,
    R_WM_ERR_SYS_WIN_MOVE_FAILED                                    = 48,
    R_WM_ERR_SYS_WIN_GEOMETRY_SET_FAILED                            = 49,
    R_WM_ERR_WIN_SWAP_FAILED                                        = 50,
    R_WM_ERR_SPEA_INTERNAL                                          = 51,
    R_WM_ERR_SYS_WIN_FLAG_UPDATE_FAILED                             = 52,
    R_WM_ERR_COULD_NOT_SET_SCREEN_COLOR_CURVE                       = 53,
    R_WM_ERR_COULD_NOT_SET_SCREEN_GAMMA                             = 54,

    R_WM_ERR_LAST
} r_wm_Error_t;


/***********************************************************
  Enum: r_wm_MsgId_t

  Description:
  The type is used to specify the messages, which are processed by the window manager.

  Members:

  see also <r_wm_Msg_t>.

*/

typedef enum
{
    R_WM_MSG_SCREEN_ENABLE,             /* 0 */
    R_WM_MSG_SCREEN_BG_COLOR_SET,
    R_WM_MSG_SCREEN_COLOR_CURVE_SET,
    R_WM_MSG_SCREEN_GAMMA_SET,

    R_WM_MSG_WIN_CREATE,                /* 4 */
    R_WM_MSG_WIN_DELETE,
    R_WM_MSG_WIN_ENABLE,
    R_WM_MSG_WIN_MOVE,
    R_WM_MSG_WIN_RESIZE,
    R_WM_MSG_WIN_COLOR_FMT_SET,
    R_WM_MSG_WIN_ALPHA_SET,             /* 10 */
    R_WM_MSG_WIN_PREMULT_ALPHA_ENABLE,
    R_WM_MSG_WIN_CLUT_SET,
    R_WM_MSG_WIN_EXTERNAL_BUF_SET,
    R_WM_MSG_WIN_COLOR_KEY_ENABLE,
    R_WM_MSG_WIN_FLAGS_UPDATE,
    R_WM_MSG_WIN_PAINT,
    R_WM_MSG_WIN_DELETE_ALL_SPRITES,
    R_WM_MSG_WIN_SWAP,

    R_WM_MSG_SPRITE_CREATE,             /* 19 */
    R_WM_MSG_SPRITE_DELETE,
    R_WM_MSG_SPRITE_ENABLE,
    R_WM_MSG_SPRITE_MOVE,
    R_WM_MSG_SPRITE_BUF_SET,
    
    R_WM_MSG_CAPT_CREATE,               /* 24 */
    R_WM_MSG_CAPT_DELETE,
    R_WM_MSG_CAPT_ENABLE,
    
    R_WM_MSG_EOF,
    R_WM_MSG_VBLANK,

    R_WM_MSG_LAST
} r_wm_MsgId_t;


/***********************************************************
  Enum: r_wm_WinMode_t

  Description:
  A window can be supported by hardware or in can be managed by software; 
  see also the chapter <WM Window functions>. 
  A parameter of this type is used by the application to specify, 
  if it wants to create a window with hardware support, 
  software support or if the application does not care.

  Members:
    R_WM_WINMODE_FB    - The window is a normal framebuffer window
    R_WM_WINMODE_SPRITES  - The window is a sprite-hosting window

  Note:
  this type is not used.
  Instead <R_WM_DEV_*> indicates the type of the WM device  
*/

typedef enum
{
    R_WM_WINMODE_FB,
    R_WM_WINMODE_SPRITES
} r_wm_WinMode_t;


/***********************************************************
  Enum: r_wm_WinStatus_t

  Description:
  Each window has a flag specifying its status. 
  If required, additional status could be added.
  
  Members:
    R_WM_WINSTATUS_NOTINITIALIZED - The window has not been created.
    R_WM_WINSTATUS_DISABLED       - The window is invisible on the screen.
    R_WM_WINSTATUS_ENABLED        - The window is visible on the screen.
*/

typedef enum
{
    R_WM_WINSTATUS_NOT_INITIALIZED = 0,
    R_WM_WINSTATUS_DISABLED,
    R_WM_WINSTATUS_ENABLED
} r_wm_WinStatus_t;


/***********************************************************
  Enum: r_wm_WinBufStatus_t

  Description:
  A window might have more than one buffer. 
  For this case there is a status flag for each buffer. 
  The flag can be used to identify, if a buffer can be used for 
  drawing operations or if it is ready to be switched to the screen etc.

  Members:
    R_WM_WINBUF_FREE                - The buffer can be used for rendering operations and it is not visible on the screen.
                                      The next free buffer must be requested by <R_WM_WindowNewDrawBufGet>.
    R_WM_WINBUF_RENDER_STARTED      - Drawing operations have been started on the buffer. 
                                      This state of a buffer is switched to R_WM_WINBUF_RENDER_FINISHED
    R_WM_WINBUF_RENDER_FINISHED     - The drawing operations in the buffer have been completed and the buffer is ready to be displayed.
                                        
    R_WM_WINBUF_ON_SCREEN           - The buffer is scheduled to be transferred to the screen, or it is already on the screen.

*/

typedef enum
{
    R_WM_WINBUF_FREE = 0,
    R_WM_WINBUF_RENDER_STARTED,
    R_WM_WINBUF_RENDER_FINISHED,
    R_WM_WINBUF_ON_SCREEN,
} r_wm_WinBufStatus_t;

/***********************************************************
  Enum: r_wm_WinBufAllocMode_t

  Description:
  The buffers can be allocated internally by the WM or externally by the application.

  Members:
  R_WM_WINBUF_ALLOC_EXTERNAL   -   The application allocates the buffers. <R_WM_WindowCreate> will not allocate them.
                                        The application does not need to allocate the buffers before it calls <R_WM_WindowCreate>.
                                        The buffers can be set later with <r_wm_WindowExternalBufSet>.

  R_WM_WINBUF_ALLOC_INTERNAL   -   If buffers should be allocated internally by WM then the function <R_WM_WindowCreate>
                                        allocates the buffers and sets the pointer Buffer in <r_wm_Window_t>.

*/

typedef enum
{
    R_WM_WINBUF_ALLOC_EXTERNAL,
    R_WM_WINBUF_ALLOC_INTERNAL
} r_wm_WinBufAllocMode_t;


/***********************************************************
  typedef: r_wm_WinBuffer_t

  Description:
  The type is used to specify data and status of each frame-buffer of a window.

  Struct members:
    Data   - This pointer specifies the address of the bitmap data.
    Status - The parameter specifies the status of the buffer; see also <r_wm_WinBufStatus_t>.
*/

typedef struct
{
    void*                           Data;
    r_wm_WinBufStatus_t    Status;
} r_wm_WinBuffer_t;


/***********************************************************
  Enum: r_wm_WinColorFmt_t

  Description:
  A parameter of this type specifies the colour mode of a window. 

  Members:
  (A)RGB(w)xyz - The value specifies the number of bits for each colour and the alpha channel.

  supported color modes :
  (code)
  R_WM_COLORMODE_RGB565             16bit                   rrrrrggggggbbbbb
  R_WM_COLORMODE_ARGB1555           16bit                   arrrrrgggggbbbbb
  R_WM_COLORMODE_ARGB4444           16bit                   aaaarrrrggggbbbb
  R_WM_COLORMODE_RGB0444            16bit                   ----rrrrggggbbbb
  R_WM_COLORMODE_RGB888             24bit           rrrrrrrrggggggggbbbbbbbb
  R_WM_COLORMODE_RGB0888            32bit   --------rrrrrrrrggggggggbbbbbbbb
  R_WM_COLORMODE_ARGB8888           32bit   aaaaaaaarrrrrrrrggggggggbbbbbbbb
  R_WM_COLORMODE_RGBA5551           16bit                   rrrrrgggggbbbbba
  R_WM_COLORMODE_RGBA4444           16bit                   rrrrggggbbbbaaaa
  R_WM_COLORMODE_RGBA8888           32bit   rrrrrrrrggggggggbbbbbbbbaaaaaaaa
  R_WM_COLORMODE_CLUT8              8 bit   8-bit (256 colors) color-lookup index
  R_WM_COLORMODE_CLUT4              4 bit   4-bit (16 colors) color-lookup index
  R_WM_COLORMODE_CLUT1              1 bit   1-bit (2 colors) color lookup index
  R_WM_COLORFMT_RLE24ARGB8888,
  R_WM_COLORFMT_RLE18ARGB8888,
  R_WM_COLORFMT_RLE24RGB0888,
  R_WM_COLORFMT_RLE18RGB0888,
  R_WM_COLORFMT_RLE8CLUT8,
  R_WM_COLORFMT_RLE8CLUT4,
  R_WM_COLORFMT_RLE8CLUT1,

                                    In digial domain, YUV is named YCbCr
  R_WM_COLORFMT_YCBCR_444,          32bit   --------vvvvvvvvyyyyyyyyuuuuuuuu

                                    Due to chroma subsampling, two pixels share their u and v data
                                    32bit   |<-  pixel 2  ->|<-  pixel 1 ->|
  R_WM_COLORFMT_YCBCR_422,          16bit   uuuuuuuuyyyyyyyy vvvvvvvvyyyyyyyy 
  R_WM_COLORFMT_YUV_YUYV,           16bit   vvvvvvvvyyyyyyyy uuuuuuuuyyyyyyyy YCbCr 422 mode, variation
  R_WM_COLORFMT_YUV_UYVY,           16bit   yyyyyyyyvvvvvvvv yyyyyyyyuuuuuuuu YCbCr 422 mode, variation
  R_WM_COLORFMT_YUV_YVYU,           16bit   uuuuuuuuyyyyyyyy vvvvvvvvyyyyyyyy YCbCr 422 mode, equal to YCBCR_422
  R_WM_COLORFMT_YUV_VYUY,           16bit   yyyyyyyyuuuuuuuu yyyyyyyyvvvvvvvv YCbCr 422 mode, variation
  (end)

  Note:
  Not all displays support all formats.
*/

typedef enum
{
    R_WM_COLORFMT_RGB565,
    R_WM_COLORFMT_ARGB1555,
    R_WM_COLORFMT_ARGB4444,
    R_WM_COLORFMT_RGB0444,
    R_WM_COLORFMT_RGB0888,
    R_WM_COLORFMT_ARGB8888,
    R_WM_COLORFMT_RGBA5551,
    R_WM_COLORFMT_RGBA4444,
    R_WM_COLORFMT_RGBA8888,
    R_WM_COLORFMT_CLUT8,
    R_WM_COLORFMT_CLUT4,
    R_WM_COLORFMT_CLUT1,
    R_WM_COLORFMT_RLE24ARGB8888,
    R_WM_COLORFMT_RLE18ARGB8888,
    R_WM_COLORFMT_RLE24RGB0888,
    R_WM_COLORFMT_RLE18RGB0888,
    R_WM_COLORFMT_RLE8CLUT8,
    R_WM_COLORFMT_RLE8CLUT4,
    R_WM_COLORFMT_RLE8CLUT1,
    R_WM_COLORFMT_YCBCR_422,
    R_WM_COLORFMT_YCBCR_444,
    R_WM_COLORFMT_YUV_YUYV,
    R_WM_COLORFMT_YUV_UYVY,
    R_WM_COLORFMT_YUV_YVYU,
    R_WM_COLORFMT_YUV_VYUY,
    R_WM_COLORFMT_LAST
} r_wm_WinColorFmt_t;


/***********************************************************
  Enum: r_wm_OutColorFmt_t

  Description:
  A parameter of this type specifies the colour mode of the video output. 

  Members:
  (A)RGB(w)xyz - The value specifies the number of bits for each colour.

  supported color modes :
  (code)
  R_WM_OUTCOLORFMT_RGB888             24bit           rrrrrrrrggggggggbbbbbbbb
  R_WM_OUTCOLORFMT_RGB666             18bit           ------rrrrrrggggggbbbbbb
  R_WM_OUTCOLORFMT_RGB565             16bit           --------rrrrrggggggbbbbb
  (end)
  

  ***Documentation of flags it subject to be moved to system-specific implementation***
  
  supported flags :
  Result of of flags is shown as example for RGB666.
  (code)
                                                      MSB                     LSB
  R_WM_OUTCOLORFMT_FLAG_ENDIAN                        ------ rrrrrr gggggg bbbbbb
                                    FLAG=0:           ------ 765432 765432 765432
                                    FLAG=1:           ------ 234567 234567 234567
  (end)

  Note:
  Not all displays support all formats.
*/

typedef enum
{
    R_WM_OUTCOLORFMT_RGB888 = 0u,
    R_WM_OUTCOLORFMT_RGB666 = 1u,
    R_WM_OUTCOLORFMT_RGB565 = 2u,
    R_WM_OUTCOLORFMT_LAST = 3u,
    
    /* Documentation of flags is subject to be moved to system-specific implementation */
    R_WM_OUTCOLORFMT_FLAG_SWAP_BR = (uint32_t) 1u<<29u,
    R_WM_OUTCOLORFMT_FLAG_ENDIAN  = (uint32_t) 1u<<30u,
    /* We do not use the 31th bit if not needed. Saves some signedness castings */
    R_WM_OUTCOLORFMT_FLAG_MASK = (R_WM_OUTCOLORFMT_FLAG_ENDIAN + R_WM_OUTCOLORFMT_FLAG_SWAP_BR),
} r_wm_OutColorFmt_t;


/***********************************************************
  typedef: r_wm_ClutEntry_t
  
  Description:
  Defines an entry of the colour lookup table.
  
  Struct members:
  B -   blue component
  G -   green component
  R -   red component
  A -   alpha component
  
*/

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t A;
} r_wm_ClutEntry_t;


/***********************************************************
  typedef: r_wm_Window_s

  forward declaration for <r_wm_Window_t>
*/
typedef struct r_wm_Window_s r_wm_Window_t; /* need forward declaration here */


/***********************************************************
    Enum: r_wm_SpriteStatus_t

    Description:

    Describes the state of the sprite. If the sprite is not assigned to any window, it will have a
    R_WM_SPRITESTATUS_NOT_INITIALIZED value. If it is assigned, it can have either R_WM_SPRITESTATUS_DISABLED
    (not visible) or R_WM_SPRITESTATUS_ENABLED (visible) state set.

    Members:
*/

typedef enum
{
    R_WM_SPRITESTATUS_NOT_INITIALIZED = 0,
    R_WM_SPRITESTATUS_DISABLED,
    R_WM_SPRITESTATUS_ENABLED
} r_wm_SpriteStatus_t;

/***********************************************************
    typedef: r_wm_Sprite_t

    Description:

    Each sprite has a data structure of this type. All sprites are linked in a queue by the window manager.

    Members:

    Window              -   pointer to the parent window
    Next                -   *internal* pointer to the next sprite in the queue
    Status              -   status of the sprite
    Data                -   pointer to the image data of a sprite
    PosX / PosY / PosZ  -   position of the sprite on the window
    Width / Height      -   size of the sprite
*/

typedef struct r_wm_Sprite_s
{
    struct r_wm_Window_s  *Window;
    struct r_wm_Sprite_s *Next;
    r_wm_SpriteStatus_t Status;
    void *Data;
    uint32_t PosX;
    uint32_t PosY;
    uint32_t PosZ;
    uint32_t Width;
    uint32_t Height;
} r_wm_Sprite_t;


/***********************************************************
    Enum: r_wm_WinFlags_t

    Description:

    On/Off Switches for different functionalities of a WM window.

    Members:
    
    R_WM_WINFLAG_V_MIRROR   - Flip the output image vertically
*/

typedef enum
{
    R_WM_WINFLAG_NONE = 0,
    R_WM_WINFLAG_V_MIRROR = 1,
    R_WM_WINFLAG_MASK = (R_WM_WINFLAG_V_MIRROR),
} r_wm_WinFlags_t;

/***********************************************************
  typedef: r_wm_Window_t

  Description:
  Each window has a data structure of this type. 
  All windows are linked in a chain by the window manager.

  Initialization instructions:
  It is required to clear the memory of <r_wm_Window_t> to zero.
  
    The following variables need to be initialised to any valid value (including zero) 
    before calling <R_WM_WindowCreate>. This is already covered by clearing everything to zero.
        PosX
        PosY
        PosZ
        UsePremultipliedAlpha
        ClutNumEntries
        ColorKey.Enabled
  
    The following varibales need use-case specific setup before calling R_WM_WindowCreate
        Mode
        ColorFmt
        Pitch
        Width
        Height
        Surface.Fb.BufNum     (if Mode is R_WM_WINMODE_FB)
        Surface.Fb.BufMode    (if Mode is R_WM_WINMODE_FB)
        Surface.Fb.Buffer     (if Mode is R_WM_WINMODE_FB and BufMode is R_WM_WINBUF_ALLOC_EXTERNAL)
        Alpha
        ClutNumEntries        (if ColorFmt is a CLUT format)
        Clut                  (if ColorFmt is a CLUT format)
  
    The following variables are set by <R_WM_WindowCreate>, so initialization value is not important
        Status
        Surface.SpritesRoot   (if Mode is R_WM_WINMODE_SPRITES)
        Next

  Struct members:
    Status                - The element keeps the status of the window; see <r_wm_WinStatus_t>.
    Mode                  - See description of <r_wm_WinMode_t>.
    ColorFmt              - The member specifies the color mode of the window; see <r_wm_WinColorFmt_t>.
    
    PosX / PosY           - These parameters specify the position of the window on the screen.
    PosZ                  - This parameter specifies the Z position of the window. 
    Pitch                 - This is the line pitch of a frame buffer. It must be greater or equal Width.
    Width / Height        - These values are the size of the window.

    [used when Mode == R_WM_WINMODE_FB]
      Surface.Fb.Buffer   - This is a pointer to an array of BufNum <r_wm_WinBuffer_t> elements. The array contains information about all frame buffers of the window.
      Surface.Fb.BufNum   - This is the number of frame buffers of the window.
      Surface.Fb.BufMode  - Buffer allocation mode; see <r_wm_WinBufAllocMode_t>
    [used when Mode == R_WM_WINMODE_SPRITES]
      Surface.SpritesRoot - *internal* pointer used to store the Sprite List assigned to this window

    Alpha                 - The parameter specifies the transparency value of the window.
    UsePremultipliedAlpha - Premultiplied Alpha Mode is active
    
    [used when ColorFmt == R_WM_COLORFMT_CLUT* ]
      ClutNumEntries      - No of entries of the CLUT assigned to this window
      Clut                - Pointer to array of CLUT entries
    
    [Color Keying. This feature cannot be used together with the Window's <Alpha>]
    [used when <ColorFmt> is an RGB Format ]
      ColorKey.In.RgbKey  - The key color for the RGBA color keying 
    [used when <ColorFmt> == R_WM_COLORFMT_CLUT* ]
      ColorKey.In.ClutKey - The key color for the color indexed color keying
    ColorKey.Out          - The 'In' key color gets substituted with the 'Out' color defined here).
    
    Flags                 - Additional flags to set certain properties of a window.
    Next                  - *internal* pointer to the next window in the chain.

*/

struct r_wm_Window_s
{
    r_wm_WinStatus_t    Status;
    r_wm_WinMode_t      Mode;
    r_wm_WinColorFmt_t  ColorFmt;
    
    int32_t             PosX;
    int32_t             PosY;
    uint32_t            PosZ;
    uint32_t            Pitch; 
    uint32_t            Width;
    uint32_t            Height;
    
    union
    {
        struct
        {
            r_wm_WinBuffer_t        *Buffer;
            uint8_t                 BufNum;
            r_wm_WinBufAllocMode_t  BufMode;
        } Fb;
        r_wm_Sprite_t               *SpritesRoot;
    } Surface;

    uint8_t                         Alpha;
    uint8_t                         UsePremultipliedAlpha;
    
    uint32_t                        ClutNumEntries;
    const r_wm_ClutEntry_t*         Clut;
    
    struct
    {
        uint8_t Enabled;
        union
        {
            struct
            {
                uint8_t             Red;
                uint8_t             Green;
                uint8_t             Blue;
            } RgbKey;
            uint8_t ClutKey;
        } In;
        struct
        {
            uint8_t                 Red;
            uint8_t                 Green;
            uint8_t                 Blue;
            uint8_t                 Alpha;
        } Out;
    } ColorKey;
    
    r_wm_WinFlags_t         Flags;
    struct r_wm_Window_s   *Next;    /*internal*/
} /*r_wm_Window_t*/;



typedef struct
{
    uint32_t Enabled;
} r_wm_MsgScreenEnable_t;

typedef struct
{
    uint8_t     Red;
    uint8_t     Green;
    uint8_t     Blue;
} r_wm_MsgScreenBgColorSet_t;

typedef struct
{
    uint32_t    NumEntries;
    const r_wm_ClutEntry_t *ColorCurve;
} r_wm_MsgScreenColorCurveSet_t;

typedef struct
{
    uint8_t     GammaRed;
    uint8_t     GammaGreen;
    uint8_t     GammaBlue;
} r_wm_MsgScreenGammaSet_t;

typedef struct
{
    r_wm_Window_t   *Win;
} r_wm_MsgWinCreateData_t;

typedef struct
{
    r_wm_Window_t   *Win;
} r_wm_MsgWinDeleteData_t;

typedef struct
{
    r_wm_Window_t   *Win;
    uint32_t Enabled;
} r_wm_MsgWinEnableData_t;

typedef struct
{
    r_wm_Window_t   *Win;
    int32_t        PosX;
    int32_t        PosY;
    int32_t        PosZ;
} r_wm_MsgWinMoveData_t;

typedef struct
{
    r_wm_Window_t   *Win;
    uint32_t        Pitch;
    uint32_t        Width;
    uint32_t        Height;
} r_wm_MsgWinResizeData_t;

typedef struct
{
    r_wm_Window_t       *Win;
    r_wm_WinColorFmt_t  ColorFmt;
} r_wm_MsgWinColorFmtSet_t;

typedef struct
{
    r_wm_Window_t   *Win;
    uint8_t         Alpha;
} r_wm_MsgWinAlphaData_t;

typedef struct
{
    r_wm_Window_t   *Win;
    uint8_t         Enabled;
} r_wm_MsgWinPremultAlphaEnableData_t;

typedef struct
{
    r_wm_Window_t       *Win;
    r_wm_WinBuffer_t    *Buf;
    uint32_t            BufNum;
    r_wm_WinColorFmt_t  ColorFormat;
} r_wm_MsgWinExternalBufSetData_t;

typedef struct
{
    r_wm_Window_t   *Win;
    r_wm_Sprite_t   *Sprite;
} r_wm_MsgWinSpriteData_t;

typedef struct
{
    r_wm_Window_t           *Win;
    uint32_t                NumEntries;
    const r_wm_ClutEntry_t  *Clut;
} r_wm_MsgWinClutSetData_t;

typedef struct
{
    r_wm_Window_t   *Win;
    uint32_t        Enabled;
} r_wm_MsgWinColorKeyEnableData_t;

typedef struct
{
    r_wm_Window_t   *Win;
    r_wm_WinFlags_t  SetFlags;
    r_wm_WinFlags_t  ClearFlags;
} r_wm_MsgWinFlagsUpdateData_t;

typedef struct
{
    r_wm_Window_t   *Win;
} r_wm_MsgWinPaintData_t;

typedef struct
{
    r_wm_Window_t   *Win;
} r_wm_MsgWinSwapData_t;

typedef struct
{
    r_wm_Window_t   *Win;
} r_wm_MsgWinDeleteAllSprites_t;

typedef struct
{
    r_wm_Window_t   *Win;
} r_wm_MsgSwapData_t;


typedef struct
{
    uint32_t    Id;
} r_wm_MsgEofData_t;

typedef struct
{
    r_wm_Sprite_t   *Sprite;
} r_wm_MsgSpriteCreateData_t;

typedef struct
{
    r_wm_Sprite_t   *Sprite;
} r_wm_MsgSpriteDeleteData_t;

typedef struct
{
    r_wm_Sprite_t   *Sprite;
    uint32_t        Enabled;
} r_wm_MsgSpriteEnableData_t;

typedef struct
{
    r_wm_Sprite_t   *Sprite;
    uint32_t        PosX;
    uint32_t        PosY;
    uint32_t        PosZ;
} r_wm_MsgSpriteMoveData_t;

typedef struct
{
    r_wm_Sprite_t   *Sprite;
    void            *Buf;
} r_wm_MsgSpriteBufSetData_t;

typedef struct r_wm_Capture_s r_wm_Capture_t;

typedef struct
{
    r_wm_Capture_t  *Capt;
} r_wm_MsgCaptCreateData_t;

typedef struct
{
    r_wm_Capture_t  *Capt;
} r_wm_MsgCaptDeleteData_t;

typedef struct
{
    r_wm_Capture_t  *Capt;
    uint32_t        Enabled;
} r_wm_MsgCaptEnableData_t;

typedef union
{
    r_wm_MsgScreenEnable_t              ScrEnable;
    r_wm_MsgScreenBgColorSet_t          ScrBgColorSet;
    r_wm_MsgScreenColorCurveSet_t       ScrColorCurveSet;
    r_wm_MsgScreenGammaSet_t            ScrGammaSet;
    r_wm_MsgWinCreateData_t             WinCreate;
    r_wm_MsgWinDeleteData_t             WinDelete;
    r_wm_MsgWinEnableData_t             WinEnable;
    r_wm_MsgWinMoveData_t               WinMove;
    r_wm_MsgWinResizeData_t             WinResize;
    r_wm_MsgWinAlphaData_t              WinAlphaSet;
    r_wm_MsgWinPremultAlphaEnableData_t WinPremultAlphaEnable;
    r_wm_MsgWinSpriteData_t             WinSprite;
    r_wm_MsgWinClutSetData_t            WinClutSet;
    r_wm_MsgWinColorFmtSet_t            WinColorFmtSet;
    r_wm_MsgWinFlagsUpdateData_t        WinFlags;
    r_wm_MsgWinPaintData_t              WinPaint;
    r_wm_MsgWinSwapData_t               WinSwap;
    r_wm_MsgWinExternalBufSetData_t     WinExternalBufSet;
    r_wm_MsgWinColorKeyEnableData_t     WinColorKeyEnable;
    r_wm_MsgWinDeleteAllSprites_t       WinDeleteAllSprites;
    r_wm_MsgSpriteCreateData_t          SpriteCreate;
    r_wm_MsgSpriteDeleteData_t          SpriteDelete;
    r_wm_MsgSpriteEnableData_t          SpriteEnable;
    r_wm_MsgSpriteMoveData_t            SpriteMove;
    r_wm_MsgSpriteBufSetData_t          SpriteBufSet;
    r_wm_MsgCaptCreateData_t            CaptCreate;
    r_wm_MsgCaptDeleteData_t            CaptDelete;
    r_wm_MsgCaptEnableData_t            CaptEnable;
    r_wm_MsgSwapData_t                  Swap;
    r_wm_MsgEofData_t                   Eof;
} r_wm_MsgGenericData;


/***********************************************************
  typedef: r_wm_Msg_t

  Description:
  The window manager stores all requests in an internal msg queue.
  During initialization the application provides an array of 
  <r_wm_Msg_t>, which the driver will use as a msg queue storage.

  Struct members:
  Id            - See <r_wm_MsgId_t>.
  Data          - A union to hold event specific data.
*/

typedef struct r_wm_Msg_s r_wm_Msg_t;

struct r_wm_Msg_s
{
    r_wm_MsgId_t            Id;
    r_wm_MsgGenericData     Data;
};


/***********************************************************
  typedef: r_wm_EventId_t

  Description:
  External event id. The events are dispatched to the callback function specified
  during the R_WM_DevInit call. These events can be used for implementing the asynchronous
  drawing mechanism (see the appropriate application note).
  
  R_WM_EVENT_VBLANK     - VBLANK interrupt notification
  R_WM_EVENT_SCANLINE   - SCANLINE interrupt notification
  R_WM_EVENT_VI_VBLANK  - Video Input VBLANK interrupt notification
*/

typedef enum
{
    R_WM_EVENT_VBLANK       = 0x1,
    R_WM_EVENT_SCANLINE     = 0x2,
    R_WM_EVENT_VI_VBLANK    = 0x3,
    R_WM_EVENT_LAST
} r_wm_EventId_t;

/***********************************************************
  typedef: r_wm_Event_t

  Description:
  The external event description. The events are dispatched to the callback function specified
  during the R_WM_DevInit call. These events can be used for implementing the asynchronous
  drawing mechanism (see the appropriate application note).

  Member:
  
  Id    - Event ID
  Data  - Data associated with the event
*/

typedef struct 
{
    r_wm_EventId_t  Id;
    uint32_t        Data;
} r_wm_Event_t;

/***********************************************************
  Enum: r_wm_Memory_t

  Description:
  The window manager requires different access types to the memory. 
  Therefore the function R_WM_SysAlloc is called with a parameter, 
  which indicates the memory type requested by the window manager.

  Members:
    R_WM_MEM_CPU   - The memory needs to be accessible by the CPU.
    R_WM_MEM_VIDEO - It has to be possible to access the memory by the VOUT, VIN, GPU and CPU.
*/

typedef enum
{
    R_WM_MEM_CPU = 0,
    R_WM_MEM_VIDEO,
} r_wm_Memory_t;


/***********************************************************
  typedef: r_wm_CapMode_t

  Description:
  There are different modes of video capturing, especially for interlaced videos. 
  This type is used to describe, which method shall be used for a capturing surface.

  Members:
  One of these six flags must be selected or the function call will fail :

  R_WM_CAPMODE_YUV_ITU656 - Select YUV itu 656 mode
  R_WM_CAPMODE_YUV_8BIT   - Select YUV raw 8bit mode with external sync
  R_WM_CAPMODE_YUV_16BIT  - Select YUV raw 16bit mode with external sync
  R_WM_CAPMODE_RGB_16BPP  - Select RGB 565 bit mode with external sync
  R_WM_CAPMODE_RGB_18BPP  - Select RGB 666 bit mode with external sync
  R_WM_CAPMODE_RGB_24BPP  - Select RGB 888 bit mode with external sync

  The following flags are optional and can be omitted :

  Dithering can be used with framebuffer format R_VI_FB_FORMAT_RGB565.  
  Dithering is disabled on default and can be enabled with one of the following flags.
  If none of these is selected dithering is disabled :

  R_WM_CAPMODE_DITHER           - Enable dithering

  some additional flags can be used :

  R_WM_CAPMODE_YUV_Y_UV_INVERT  - Invert the position of Y and UV in the ITU stream (YUYV -> UYVY)
  R_WM_CAPMODE_VSYNC_INVERT     - Invert the polarity of the incoming vsync signals
  R_WM_CAPMODE_HSYNC_INVERT     - Invert the polarity of the incoming vsync signals
  R_WM_CAPMODE_DATA_CLK_INVERT  - Data sampling point
  R_WM_CAPMODE_VSYNC_CLK_INVERT - vsync sampling point
  R_WM_CAPMODE_HSYNC_CLK_INVERT - hsync sampling point
  R_WM_CAPMODE_FIXED_VSYNC      - Video Output VSYNC is fixed to internal pixel clock.

  data can be captured using CSync, HSync/VSync or Data Enable Sync.
  If none of the following flags is set then HSync/VSync is used.
  
  One of these three flags must be selected :

  R_WM_CAPMODE_NO_INTERLACE - A video without interlacing is captured to the surface.
  R_WM_CAPMODE_WEAVE        - The weave method is used to deinterlace the video.
  R_WM_CAPMODE_BOB          - Deinterlacing is done by bob mode.
*/

typedef enum
{
    R_WM_CAPMODE_NONE             = 0,
    R_WM_CAPMODE_YUV_ITU656       = (int32_t)(1uL<< 0),
    R_WM_CAPMODE_YUV_8BIT         = (int32_t)(1uL<< 1),
    R_WM_CAPMODE_YUV_16BIT        = (int32_t)(1uL<< 2),
    R_WM_CAPMODE_RGB_16BPP        = (int32_t)(1uL<< 3),
    R_WM_CAPMODE_RGB_18BPP        = (int32_t)(1uL<< 4),
    R_WM_CAPMODE_RGB_24BPP        = (int32_t)(1uL<< 5),
    R_WM_CAPMODE_DITHER           = (int32_t)(1uL<< 6),
    
    /* default Y1UY2V, inverted UY1VY2 */
    R_WM_CAPMODE_YUV_Y_UV_INVERT  = (int32_t)(1uL<< 10),
    /* vsync polarity (default: pos polarity, inverted: neg polarity) */
    R_WM_CAPMODE_VSYNC_INVERT     = (int32_t)(1uL<< 11),
    /* hsync polarity (default: pos polarity, inverted: neg polarity) */
    R_WM_CAPMODE_HSYNC_INVERT     = (int32_t)(1uL<< 12),
    /* data sampling point  (default: falling clock edge, inverted: rising clock edge) */
    R_WM_CAPMODE_DATA_CLK_INVERT  = (int32_t)(1uL<< 15),
    /* vsync sampling point (default: falling clock edge, inverted: rising clock edge) */
    R_WM_CAPMODE_VSYNC_CLK_INVERT = (int32_t)(1uL<< 16),
    /* hsync sampling point (default: falling clock edge, inverted: rising clock edge) */
    R_WM_CAPMODE_HSYNC_CLK_INVERT = (int32_t)(1uL<< 17),
    R_WM_CAPMODE_H_MIRRORING      = (int32_t)(1uL<< 18),
    R_WM_CAPMODE_V_MIRRORING      = (int32_t)(1uL<< 19),
    /* internal fixed vsync generation (default: vo vsync is synchronised to vi vsync) */
    R_WM_CAPMODE_FIXED_VSYNC      = (int32_t)(1uL<< 20),
    
    /* Bits 0-20 mirror the VDCE data type <r_vdce_CapMode_t>. 
       The following entries are additional. */
    
    R_WM_CAPMODE_NO_INTERLACE = (int32_t)(1ul<< 29),    /* Currently not used by driver */
    R_WM_CAPMODE_WEAVE        = (int32_t)(1ul<< 30),
    R_WM_CAPMODE_BOB          = (int32_t)(1ul<< 31)
} r_wm_CapMode_t;


/***********************************************************
  typedef: r_wm_Capture_t

  Description:
  The type describes the settings of a video capturing surface. 
  The video capturing surface is always associated with a window surface. 

  Members:
    Window              - The member is a pointer to the associated window. 
                            The window has to match the requirements of the captured video; 
                            e.g. size, colour mode.
    Mode                - The member specifies the capturing mode.
    Status              - see <R_WM_WinStatus_t>
    StartX              - This is the X starting position in the video.
    StrideX             - The parameter specifies the stride in the frame buffer.
    StartY1 / StartY2   - This is the Y starting position for the field 1 and field 2.
    Width / Height      - This is the size of the video.
    ScaledWidth/Height  - The target window will be scaled to these dimensions
    Delay               - When VI- and VO-VSYNC are synchronized, this value delays VO-VSYNC measured in lines.
    CapUnit             - The Video Channel to be used for capturing. 
                            If another Unit is to be used than the assigned Window, 
                            please make sure that this unit is already initialised.
    Next                - *internal* All capturing surfaces are linked in a list. 
                            This pointer points to the next capturing surface.
*/

struct r_wm_Capture_s
{
    r_wm_Window_t           *Window;
    r_wm_CapMode_t          Mode;
    uint32_t                StartX;
    uint32_t                StrideX;
    uint32_t                StartY1;
    uint32_t                StartY2;
    uint32_t                Width;
    uint32_t                Height;
    uint32_t                ScaledWidth;
    uint32_t                ScaledHeight;
    uint32_t                Delay;
    uint32_t                CapUnit;
    struct r_wm_Capture_s   *Next;
};


/***********************************************************
  Section: Global API functions
*/


/***********************************************************
  Group: Device

  The section describes driver functions, 
  which are required for general use of the driver, 
  but which are related to a specific functionality of the macro itself.
*/


/***********************************************************
  Function: R_WM_DevInit

  Init WM unit.
  
  Description:
  This function initializes the driver and the hardware as far as necessary. 
  The driver makes sure, that the macro is set into a default configuration. 
  It is also in the responsibility of the driver to keep track of the macro 
  instance status and to avoid, that the macro instance is initialized more 
  than one time or that any other function can be executed before the 
  initialization has been completed successfully.
  
  Parameter:
  Unit          -   The parameter specifies the screen number
                    The window manager can support more than one physical screen 
                    Valid values are defined by <R_WM_DEV_*>
                    The number of valid devices is <R_WM_DEV_NUM>
  MsgQueue      -   This is a pointer to Size elements of the <R_WM_Msg_t> type
                    The array is used by the window manager to store the window manager requests
  Size          -   The number specified the number of elements in the message queue
  EventCb       -   External event notification callback function
  CpuHeap       -   Pointer to your managed cpu heap
  VidHeap       -   Pointer to your managed video heap
  
  Return value:
  See <R_WM_Error_t>. 
*/

r_wm_Error_t R_WM_DevInit   (  uint32_t        Unit,
                                r_wm_Msg_t      *MsgQueue, 
                                uint32_t        Size,
                                void(*EventCb)(uint32_t Unit, const r_wm_Event_t *Event),
                                const void *CpuHeap,
                                const void *VidHeap
                            );


/***********************************************************
  Function: R_WM_DevEventRegister

  Description:
  Register for retrieving the notification on an event.
  Only the external events registered with this function will
  be received by the device callback function.
  
  Parameter:
  
  Unit      -   See description of <R_WM_DevInit>
  EventId   -   The ID of the event to be registered for receiving
  Arg       -   Generic argument if applicable
  
  Generic argument values:
  
  R_WM_EVENT_VBLANK     - any (ignored)
  R_WM_EVENT_SCANLINE   - scan line number on which the interrupt will trigger
*/
r_wm_Error_t R_WM_DevEventRegister  (   uint32_t        Unit,
                                        r_wm_EventId_t  EventId,
                                        uint32_t        Arg
                                    );

/***********************************************************
  Function: R_WM_DevDeinit

  DeInit WM unit.

  Description:
  This function deinitializes the driver and the hardware. 
  In case the function is called for an instance of the macro, 
  which has not been initialized before, the function shall return an error. 
  
  Parameter:
  Unit -  See description of <R_WM_DevInit>
  
  Return value:
  See <r_wm_Error_t>.
*/

r_wm_Error_t R_WM_DevDeinit(uint32_t Unit);



/***********************************************************
  Function: R_WM_DevInfoGet

  Description:
  Get information of screen and window parameters.

  Parameter:
  Unit      - See description of <R_WM_DevInit>
  LayerNum  - pointer to a variable to return number of usable layers
  PitchMax  - pointer to a variable to return maximum pitch of a layer. If 0 is returned then a pitch is not supported.
  WidthMax  - pointer to a variable to return maximum width of a layer
  HeightMax - pointer to a variable to return maximum height of a layer

  Return value:
  See <r_wm_Error_t>.
*/

r_wm_Error_t R_WM_DevInfoGet    (   uint32_t  Unit,
                                    uint32_t *LayerNum,
                                    uint32_t *PitchMax,
                                    uint32_t *WidthMax,
                                    uint32_t *HeightMax
                                );


/***********************************************************
  Function: R_WM_GetVersionString

  Returns:
  
  version string of this WM driver
*/

const int8_t *R_WM_GetVersionString(void);


/***********************************************************
  Group: Screen

  A screen is a physical video output unit. 
  There is no frame-buffer associated with a screen, 
  but a screen might have a background color.
*/


/***********************************************************
  Function: R_WM_ScreenTimingSet

  Description:
  Create a screen on the specified video output.
  The new values will be active after placing a call to <R_WM_ScreenEnable> 
  into the message queue and executing this part of the queue.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Timing - The timing parameter contains all the information 
             to drive the display see the description of <r_ddb_Timing_t> for details.

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_ScreenTimingSet   (   uint32_t        Unit,
                                        r_ddb_Timing_t  *Timing
                                    );


/***********************************************************
  Function: R_WM_ScreenTimingSetByName

  Description:
  Create a screen on the specified video output.
  The new values will be active after placing a call to <R_WM_ScreenEnable> 
  into the message queue and executing this part of the queue.

  Parameter:
    Unit - See description of <R_WM_DevInit>
    Name - The name is an identifier, which is used to lookup 
            for the correct screen timing in the display data base (see: <r_ddb_Timing_t>).

  Return value:
  See <r_wm_Error_t>.
*/

r_wm_Error_t R_WM_ScreenTimingSetByName (   uint32_t        Unit,
                                            const int8_t    *Name
                                        );


/***********************************************************
  Function: R_WM_ScreenColorFormatSet

  Description:
  Set the color format for the signals of the specified video output.
  Default format for the video output is RGB888.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    OutFmt - A color format of <r_wm_OutColorFmt_t> plus optional bit flags.

  Return value:
  See <r_wm_Error_t>.
*/

r_wm_Error_t R_WM_ScreenColorFormatSet (    uint32_t            Unit,
                                            r_wm_OutColorFmt_t  OutFmt
                                        );


/***********************************************************
  Function: R_WM_ScreenBgColorSet

  Description:
  Set the screen background color that is seen, if no window 
  (or a transparent one) is on top of it. 
  If the selected screen does not support a background color, 
  the function will return with an error.

  Parameter:
  Unit           - See description of <R_WM_DevInit>
  Red/Green/Blue - The individual color components of the background color.

  Return value:
  See <r_wm_Error_t>. 
*/
r_wm_Error_t R_WM_ScreenBgColorSet  (   uint32_t Unit,
                                        uint8_t  Red, 
                                        uint8_t  Green, 
                                        uint8_t  Blue
                                    );


/***********************************************************
  Function: R_WM_ScreenColorCurveSet

  Description:
  Set a curve to be used as custom gamma or color correction curve. 
  Using this curve, each RGB color channel is individually corrected according 
  to the given curve.
  
  For the correction in VDCE, the incoming color information of each channel 
  is split into 32 equally sized segments each covering a range 8 color values.
  For each of these 8 values in a segment, the same gain factor applies.
  
  To configure the segments, a start and an end value need to be given. 
  This requires 33 reference points to be passed to this function.
  
  For each segment of each color, the gain factor must be in range [0.0 .. +2.0], 
  thus the values between two reference points may have a difference in range of [0 .. +16].
  The origin is always at point zero: In = Out = RGB(0,0,0).
  The slope is always positive.
  
  If any value except [0] exceeds above limitation, the curve will be approximated
  as close as possible while staying within the HW limitations.
  
  The reference points are RGB values which define the color output expected 
  for that certain reference point. 
  E.g. 
    Reference point 1 defines the desired output color for the input color RGB(8,8,8).
    Reference point 3 defines the desired output color for the input color RGB(24,24,24).
  
  This function will overwrite the settings of <R_WM_ScreenGammaSet>.
  
  Parameter:
  Unit          - See description of <R_WM_DevInit>
  NumEntries    - Number of reference points
  Clut          - Pointer to Table of reference points <r_wm_ClutEntry_t>. 
                  ALPHA value of data type is unused!
                  Note: The data pointed to by the pointer <ColorCurve> must be available
                        until the message queue containing this command has been processed.
  Return value:
  See <r_wm_Error_t>. 
*/
r_wm_Error_t R_WM_ScreenColorCurveSet ( uint32_t Unit,
                                        uint32_t NumEntries,
                                        const r_wm_ClutEntry_t *ColorCurve
                                      );


/***************************************************************************
  Function: R_WM_ScreenGammaSet

  Description:
    This function sets the output gamma correction.
    
    This function will overwrite the settings of <R_WM_ScreenColorCurveSet>.

  Parameter:
    Unit    - See description of <R_WM_DevInit>
    Gamma   - Gamma correction factor (0..255 => 0.25..2.0, 128=1.0)

  Return value:
  See <r_wm_Error_t>. 
*/
r_wm_Error_t R_WM_ScreenGammaSet (  uint32_t Unit,
                                    const uint8_t GammaRed,
                                    const uint8_t GammaGreen,
                                    const uint8_t GammaBlue
                                 );


/***********************************************************
  Function: R_WM_ScreenEnable

  Description:
  Switch on the display.

  Parameter:
  Unit - See description of <R_WM_DevInit>

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_ScreenEnable(uint32_t Unit);


/***********************************************************
  Function: R_WM_ScreenDisable

  Description:
  Switch off the display.

  Parameter:
  Unit - See description of <R_WM_DevInit>

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_ScreenDisable(uint32_t Unit);


/***********************************************************
  Group: Windows
  
  Description:
  A window is an area with graphical content, which is placed 
  on a screen. 
  Windows may cover the whole screen, but they could also cover 
  part of the screen only. 

  It is possible to stack windows and to define transparency 
  for a window. 
  If overlapping windows have the same Z position, it is not 
  defined, which window will be on the top. 

  A video output can have hardware support for windows (e.g. 
  hardware layers). 

  Note:
  If an OS is used the Window functions internally use semaphores. 
  This must be taken into account if they are called by an interrupt service routine.
*/


/***********************************************************
  Function: R_WM_WindowCreate

  Description:
  Create a window as specified in the Window parameter on screen Unit. 

  For variable locations of <r_wm_Window_t> that are not initialized 
  during startup, e.g. local variables or allocated variables, please
  make sure to initialize all members of the struct.
  Unused values may be set to zero.
  For example: (code)
  memset(&Window, 0, sizeof(r_wm_Window_t))
  (end)

  If the <r_wm_WinBufAllocMode_t> is R_WM_WINBUF_ALLOC_EXTERNAL 
  the pointer Buffer of the window structure has to point to 
  an array of BufNum elements of <r_wm_WinBuffer_t> type.
  
  VDCE Specific:
  - RLE-Type buffer addresses must be 128-Byte aligned.
  - RLE-Type buffer strides must be 128-byte aligned and increased by another 128-byte.
  - Normal-Type buffer addresses must be 128-Byte aligned.
  
  Alternatively the Buffer does not need to be set yet. 
  It can be set later by <r_wm_WindowExternalBufSet>.

  If the <r_wm_WinBufAllocMode_t> is R_WM_WINBUF_ALLOC_INTERNAL
  the function will allocate the Buffer data for each buffer itself. 
  Buffer must be set to NULL.

  The Next pointer value is set by the function; any value 
  in it will be ignored. 
  
  The window will not be visible until it is enabled. 

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a pointer to an <r_wm_Window_t> structure. 
             The structure has to be filled by the application 
             before calling the function.
  
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowCreate  (   uint32_t       Unit,
                                    r_wm_Window_t *Window
                                );


/***********************************************************
  Function: R_WM_WindowDelete

  Description:
  The function will delete the specified window. 
  All internally allocated data buffer will be freed by the function. 

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a window's structure <r_wm_Window_t> pointer
  
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowDelete  (   uint32_t        Unit,
                                    r_wm_Window_t   *Window
                                );


/***********************************************************
  Function: R_WM_WindowEnable

  Description:
  Enable the window. The window will be visible on the screen 
  after calling this function.

  This function triggers a config event, see <r_wm_MsgId_t>.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a window's structure <r_wm_Window_t> pointer

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowEnable  (   uint32_t        Unit,
                                    r_wm_Window_t   *Window
                                );


/***********************************************************
  Function: R_WM_WindowDisable

  Description:
  Disable the window. The window will be invisible on the screen 
  after calling this function.

  This function triggers a config event, see <r_wm_MsgId_t>.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a Window's structure <r_wm_Window_t> pointer

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowDisable (   uint32_t Unit,
                                    r_wm_Window_t *Window
                                );


/***********************************************************
  Function: R_WM_WindowMove

  Description:
  Move the window to the specified position.

  This function triggers a move event, see <r_wm_MsgId_t>.

  Parameter:
    Unit           - See description of <R_WM_DevInit>
    Window         - This is a window's structure <r_wm_Window_t> pointer
    PosX/PosY/PosZ - These parameters specify the new absolute position of the window on the screen.
                     For negative X coordinates, please note that moving is restricted 
                     by the horizontal blanking width. Negative PosX values may not fall 
                     below (-r_ddb_Timing_t.H.BlankWidth+16). For negative PosX that exceed 
                     this limit, their value is clamped to (-r_ddb_Timing_t.H.BlankWidth+16).
                     
                     An exception that allows for free moving in negative X direction is 
                     that (r_ddb_Timing_t.H.BlankWidth - 16) >= (128 / BPP).
    
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowMove    (   uint32_t       Unit,
                                    r_wm_Window_t  *Window,
                                    int32_t        PosX,
                                    int32_t        PosY,
                                    int32_t        PosZ
                                );

/***********************************************************
  Function: R_WM_WindowResize

  Description:
  Resizes the window.

  Parameter:
    Unit                - See description of <R_WM_DevInit>
    Window              - This is a window's structure <r_wm_Window_t> pointer
    Pitch/Width/Height  - These parameters specify the new geometry of the window.

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowResize    ( uint32_t        Unit,
                                    r_wm_Window_t   *Window,
                                    uint32_t        Pitch,
                                    uint32_t        Width,
                                    uint32_t        Height
                                );

/***********************************************************
  Function: R_WM_WindowColorFmtSet

  Description:
  Sets the color format of the window.

  Parameter:
    Unit                - See description of <R_WM_DevInit>
    Window              - Pointer to the window's structure <r_wm_Window_t>
    ColorFmt            - Window color format, see the description of <r_wm_WinColorFmt_t>

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowColorFmtSet (   uint32_t                Unit,
                                        r_wm_Window_t           *Window,
                                        r_wm_WinColorFmt_t      ColorFmt
                                    );


/***********************************************************
  Function: R_WM_WindowAlphaSet

  Description:
  Change the window's alpha value.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a window's structure <r_wm_Window_t> pointer
    Alpha  - New alpha value of the window.
  
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowAlphaSet(   uint32_t        Unit,
                                    r_wm_Window_t  *Window,
                                    uint8_t         Alpha);


/***********************************************************
  Function: R_WM_WindowPremultipliedAlphaEnable

  Description:
  Enable the window's premultiplied alpha mode.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a window's structure <r_wm_Window_t> pointer
  
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowPremultipliedAlphaEnable(   uint32_t        Unit,
                                                    r_wm_Window_t  *Window);


/***********************************************************
  Function: R_WM_WindowPremultipliedAlphaDisable

  Description:
  Disable the window's premultiplied alpha mode.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a window's structure <r_wm_Window_t> pointer
  
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowPremultipliedAlphaDisable(  uint32_t        Unit,
                                                    r_wm_Window_t  *Window);


/***********************************************************
  Function: R_WM_WindowVerticalMirrorEnable

  Description:
  Enable window's vertical mirroring.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a window's structure <r_wm_Window_t> pointer
  
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowVerticalMirrorEnable(   uint32_t        Unit,
                                                r_wm_Window_t  *Window);


/***********************************************************
  Function: R_WM_WindowVerticalMirrorDisable

  Description:
  Disable window's vertical mirroring.

  Parameter:
    Unit   - See description of <R_WM_DevInit>
    Window - This is a window's structure <r_wm_Window_t> pointer
  
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowVerticalMirrorDisable(  uint32_t        Unit,
                                                r_wm_Window_t  *Window);


/***********************************************************
  Function: R_WM_WindowSwap

  Description:
  If the window is a multi-buffer window, the background buffer 
  is switched to the window surface (see: <r_wm_WinBufStatus_t>). 

  Parameter:
    Unit       - See description of <R_WM_DevInit>
    Window     - Pointer to the window's structure <r_wm_Window_t>
    
  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_WindowSwap    (   uint32_t        Unit,
                                    r_wm_Window_t   *Window
                                );



/***********************************************************
  Function: R_WM_WindowNewDrawBufGet

  Description:
  Returns the next free buffer of the specified window.

  Parameter:
  Unit   - See description of <R_WM_DevInit>
  Window - This is a window's structure <r_wm_Window_t> pointer

  Return value:
  Address of the next frame buffer, which can be used for drawing operations.
  Zero if no free buffer is available.
*/

void *R_WM_WindowNewDrawBufGet  (   uint32_t        Unit,
                                    r_wm_Window_t   *Window
                                );


/***********************************************************
  Function: R_WM_WindowVisibleBufGet

  Description:
  Returns the visible buffer of the specified window.

  Parameter:
  Unit   - See description of <R_WM_DevInit>
  Window - This is a window's structure <r_wm_Window_t> pointer

  Return value:
  Address of the next frame buffer, which is currently on screen (or scheduled to be on screen)
*/

void *R_WM_WindowVisibleBufGet  (   uint32_t        Unit,
                                    r_wm_Window_t   *Window
                                );


/***********************************************************
  Function: R_WM_WindowCurrentDrawBufGet

  Description:
  Return the current drawing frame buffer of the specified window.

  Parameter:
  Unit   - See description of <R_WM_DevInit>
  Window - This is a window's structure <r_wm_Window_t> pointer

  Return value:
  Address of the current frame buffer.
  Zero if error.
*/
void *R_WM_WindowCurrentDrawBufGet  (   uint32_t        Unit,
                                        r_wm_Window_t   *Window
                                    );


/***********************************************************
  Function: R_WM_WindowExternalBufSet

  Description:
  Dynamically set the framebuffers in desired format (not neccessarilly the same as during window creation)
  for windows with externally allocated buffers.
  
  The window must have been created with the <r_wm_WinBufAllocMode_t> R_WM_WINBUF_ALLOC_EXTERNAL.

  VDCE Specific:
  - RLE-Type buffer addresses must be 128-Byte aligned.
  - RLE-Type buffer strides must be 128-byte aligned and increased by another 128-byte.
  - Normal-Type buffer addresses must be 128-Byte aligned.

  Parameter:
    Unit        - See description of <R_WM_DevInit>
    Window      - Pointer to the window's structure <r_wm_Window_t>
    Buf         - Buffer array
    BufNum      - Number of buffers in the array
    ColorFormat - Color format of the buffers

  Return value:
  See <r_wm_Error_t>.
*/

r_wm_Error_t R_WM_WindowExternalBufSet  (   uint32_t            Unit,
                                            r_wm_Window_t       *Window,
                                            r_wm_WinBuffer_t    *Buf,
                                            uint32_t            BufNum,
                                            r_wm_WinColorFmt_t  ColorFormat
                                        );


/***********************************************************
  Function: R_WM_WindowColorKeyEnable

  Description:
  Enable the colorkeying of the selected Window.
  Colorkeying will override the <r_wm_Window_t.Alpha> setting.

  Parameters:
    Unit    - See description of <R_WM_DevInit>
    Window  - This is a window's structure <r_wm_Window_t> pointer

  Returns value:
  See <r_wm_Error_t>.
*/

r_wm_Error_t R_WM_WindowColorKeyEnable(uint32_t Unit, r_wm_Window_t  *Window);

/***********************************************************
  Function: R_WM_WindowColorKeyDisable

  Description:
  Disable the colorkeying of the selected Window

  Parameters:
    Unit    - See description of <R_WM_DevInit>
    Window - window's structure <r_wm_Window_t> pointer

  Returns value:
  See <r_wm_Error_t>.
*/
r_wm_Error_t R_WM_WindowColorKeyDisable(uint32_t Unit, r_wm_Window_t *Window);

/***********************************************************
  Function: R_WM_WindowClutSet
  
  Description:
  
  Sets the colour lookup table. Only applicable for the
  windows with CLUT colour modes specified.
  
  Parameters:
    Unit        - See description of <R_WM_DevInit>
    Window      - window's structure <r_wm_Window_t> pointer
    NumEntries  - Number of the color lookup-table entries
    Clut        - Color lookup-table pointer <r_wm_ClutEntry_t>. 
                  Note: The data pointed to by the pointer <Clut> must be avalilable 
                        as long as the Window using the CLUT color format is active.
*/

r_wm_Error_t R_WM_WindowClutSet (   uint32_t Unit,
                                    r_wm_Window_t *Window,
                                    uint32_t NumEntries,
                                    const r_wm_ClutEntry_t  *Clut
                                );

/***********************************************************
  Function: R_WM_WindowRemoveAllSprites

  Description:
  Deletes all sprites associated to the window. This function is guaranteed
  to execute faster than deleting sprite by sprite manually.
  
  Parameters:
  Unit        - See description of <R_WM_DevInit>
  Window      - window's structure <r_wm_Window_t> pointer
*/

r_wm_Error_t R_WM_WindowDeleteAllSprites    (   uint32_t Unit,
                                                r_wm_Window_t *Window
                                            );


/***********************************************************
  Group: Message Queue
*/


/***********************************************************
  Function: R_WM_MsgEnqueue

  Description:
  Enqueues the WM requests to the message queue.
  
  Parameter:
    Unit      - See description of <R_WM_DevInit>
    Msg       - Event description, see <r_wm_Msg_t>.

  Return value:
  See <r_wm_Error_t>. 

  Note:
  This function must not be called by an ISR! 
*/

r_wm_Error_t R_WM_MsgEnqueue    (   uint32_t        Unit,
                                    r_wm_Msg_t      *Msg
                                );

/***********************************************************
  Function: R_WM_FrameEndMark

  Description:
  Frame delimiter - marks the end of the sequence of the WM requests to be executed
  during one frame redraw.
  
  Parameters:
  Unit  - See description of <R_WM_DevInit>
  Id    - The sequence ID (should be used in subsequent R_WM_FrameWait call)
  
  Return value:
  See <r_wm_Error_t>.
*/
                                
r_wm_Error_t R_WM_FrameEndMark(uint32_t Unit, uint16_t Id);

/***********************************************************
  Function: R_WM_FrameWait

  Description:
  Executes the requests from the message queue up to the specicied frame delimiter. 

  Parameters:
  Unit  - See description of <R_WM_DevInit>
  Id    - ID of the frame up to which all the requests should be executed

  Return value:
  See <r_wm_Error_t>.
  
  Remarks:
  This function blocks until all the requests scheduled up to the specified frame delimiter
  are executed, and the corresponding VOUT HW update is finished.
  
  Specific implementation for the VDCE:
  - Before processing the message queue, this function waits until the Hardware is ready to be 
    reconfigured. The point of time of this "Scanline Event" can be configured with the function 
    <R_WM_Sys_DevEventRegister>. The default value is (ScreenHeight / 2).
  - After processing the queue, this function waits for the "VSync Event" until the Hardware 
    accepted the new configuration.
  - Please note, if the "Scanline Event" already occurred for the current frame, this function 
    will wait for the next event, thus it will skip one frame.
    The same applies to the "VSync Event". If processing the message queue took too much time to 
    miss the current event, this function will wait for the next event and skip one frame.
    
  Default sequence:
  The call to <R_WM_FrameWait> consists of 
  - WAIT (for "Scanline Event")
  - EXEC (process the message queue)
  - WAIT (for "VSync Event")
  
  (code)
  VSync      ScanLine        VSync      ScanLine         VSync
    |            |             |            |             |   
    | DRW |      |             | DRW |      |             |   
    |     | WAIT |             |     | WAIT |             |   
    |            | EXEC |      |            | EXEC |      |   
    |            |      | WAIT |            |      | WAIT |   
  (end)

  Problematic sequence:
  Case 1 - The "Scanline Event" is configured to occur while drawing is still active.
  The execution of the Message Queue will happen on the following "Scanline Event".
  
  (code)
  VSync      ScanLine        VSync      ScanLine         VSync
    |            |             |            |             |   
    | DRW --------> |          |            |             |   
    |            |  | WAIT ---------------> |             |   
    |            |             |            | EXEC |      |   
    |            |             |            |      | WAIT |   
  (end)
  
  Case 2 - The "Scanline Event" is configured to occur too close to the "Vsync Event".
  Control will be given back to user application only after the following "VSync Event"
  
  (code)
  VSync            ScanLine  VSync            ScanLine   VSync
    |                  |       |                  |       |
    | DRW |            |       |                  |       |
    |     | WAIT ----> |       |                  |       |
    |                  | EXEC --> |               |       |
    |                  |       |  | WAIT ---------------> |
  (end)
  
*/
r_wm_Error_t R_WM_FrameWait(uint32_t Unit, uint16_t Id);

/***********************************************************
  Function: R_WM_FrameExecuteNext

  Description:
  Requests to WM are stored in the message queue. 
  This function executes the requests in the queue up to the next frame delimiter. 

  Parameters:
  Unit  - See description of <R_WM_DevInit>

  Return value:
  The encountered frame-end Id .
  
  Remarks:
  This function can be used for implementing the custom drawing mechanism, apart from the
  synchronous one provided by default.
*/
uint32_t R_WM_FrameExecuteNext(uint32_t Unit);

/***********************************************************
  Group: Video Capture

  The window manager offers the possibility to create surfaces 
  for the video capturing.
*/

/***********************************************************
  Function: R_WM_CaptureCreate

  Description:
  Create a video capture surface inside a specific window on the screen Unit. 
  The pointer Window of the capture structure has to point to a valid window,
  which needs to be of the 'frame buffer' type.
  It is also the responsibility of the caller to ensure
  that the window is suitable for the desired video capturing parameters. 
  The capturing surface will not be visible until it is enabled. 

  Parameter:
  Unit    - See description of <R_WM_DevInit>
  Capture - This is a pointer to an <r_wm_Capture_t> structure. 
            The structure has to be filled by the application before calling the function.

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_CaptureCreate (   uint32_t    Unit,
                                    r_wm_Capture_t *Capture
                                );
/***********************************************************
  Function: R_WM_CaptureDelete

  Description:
  Delete the specified capturing surface. 
  It will not delete the window, which is used for the capturing.

  Parameter:
  Unit    - See description of <R_WM_DevInit>s
  Capture - This is a pointer to the capturing surface’s structure <r_wm_Capture_t>.

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_CaptureDelete(uint32_t             Unit,
                                      r_wm_Capture_t *Capture);


/***********************************************************
  Function: R_WM_CaptureEnable

  Description:
  Enable the video capturing surface and start the capturing. 
  The capturing is visible inside the specified window.


  Parameter:
  Unit    - See description of <R_WM_DevInit>
  Capture - This is a pointer to the capturing surface’s structure <r_wm_Capture_t>

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_CaptureEnable(uint32_t             Unit,
                                      r_wm_Capture_t *Capture);


/***********************************************************
  Function: R_WM_CaptureDisable

  Description:
  Disable the video capturing. The last frame will remain in the window's framebuffer.

  Parameter:
  Unit    - See description of <R_WM_DevInit>
  Capture - This is a pointer to the capturing surface’s structure <r_wm_Capture_t>.

  Return value:
  See <r_wm_Error_t>. 
*/

r_wm_Error_t R_WM_CaptureDisable    (   uint32_t             Unit,
                                        r_wm_Capture_t *Capture
                                    );

/***********************************************************
  Group: Sprites
*/

/***********************************************************
  Function: R_WM_SpriteCreate

  Description:
  Create a sprite as specified in the Sprite parameter on window Window of screen Unit.
  The sprite will not be visible until it is enabled.

  Parameter:
  Unit      -   See description of <R_WM_DevInit>
  Sprite    -   <R_WM_Sprite_t> structure pointer
                The structure has to be filled by the application before calling the function.

  Return value:
  See R_WM_Error_t.
*/
r_wm_Error_t R_WM_SpriteCreate(uint32_t Unit, r_wm_Sprite_t *Sprite);

/***********************************************************
  Function: R_WM_SpriteEnable
  
  Description:
  Enable the specified sprite. The sprite becomes visible on the screen.

  Parameter:
  Unit      -   See description of <R_WM_DevInit>
  Sprite    -   <R_WM_Sprite_t> structure pointer

  Return value:
  See R_WM_Error_t.
*/
r_wm_Error_t R_WM_SpriteEnable(uint32_t Unit, r_wm_Sprite_t *Sprite);

/***********************************************************
  Function: R_WM_SpriteDisable
  
  Description:
  Disable the specified sprite. The sprite becomes not visible on the screen.

  Parameter:
  Unit      -   See description of <R_WM_DevInit>
  Sprite    -   <R_WM_Sprite_t> structure pointer

  Return value:
  See R_WM_Error_t.
*/

r_wm_Error_t R_WM_SpriteDisable(uint32_t Unit, r_wm_Sprite_t *Sprite);

/***********************************************************
  Function: R_WM_SpriteMove
  
  Description:
  Move the sprite to the specified X/Y and Z-order location.

  Parameter:
  Unit              -   See description of <R_WM_DevInit>
  Sprite            -   <R_WM_Sprite_t> structure pointer
  PosX/PosY/PosZ    -   New sprite position
  
  Return value:
  See R_WM_Error_t.
  
  Remarks:
  
  The PosZ specifies the relative position of the sprite in the sprites Z-order.
  Changing the PosZ of one sprite can yield VOUT HW registers writes for all the sprites
  defined within a window.
*/

r_wm_Error_t R_WM_SpriteMove    (   uint32_t            Unit,
                                    r_wm_Sprite_t       *Sprite,
                                    uint32_t            PosX,
                                    uint32_t            PosY,
                                    uint32_t            PosZ
                                );

/***********************************************************
  Function: R_WM_SpriteBufSet
  
  Description:
    Set the sprite buffer. This way the buffer initially
    set upon completion of the <R_WM_SpriteCreate> can be changed.

  Parameter:
    Unit              -   See description of <R_WM_DevInit>
    Sprite            -   <R_WM_Sprite_t> structure pointer
    Buffer            -   New sprite buffer
  
  Return value:
    See R_WM_Error_t.
*/

r_wm_Error_t R_WM_SpriteBufSet(uint32_t Unit, r_wm_Sprite_t *Sprite, void *Buffer);

/***********************************************************
  Function: R_WM_SpriteDelete
  
  Description:
  Remove the sprite from the host window.
  
  Parameter:
  Unit              -   See description of <R_WM_DevInit>
  Sprite            -   <R_WM_Sprite_t> structure pointer
  
  Return value:
  See R_WM_Error_t.
*/

r_wm_Error_t R_WM_SpriteDelete(uint32_t Unit, r_wm_Sprite_t *Sprite);

/***********************************************************
  Group: General
*/

/***********************************************************
  Function: R_WM_ErrorCallbackSet

  Description:
  Set the error callback function.

  Parameter:
  Unit    - See description of <R_WM_DevInit>
  ErrorCb - Error callback function

  Return value:
  See <r_wm_Error_t>. 
*/
r_wm_Error_t R_WM_ErrorCallbackSet  (   uint32_t Unit,
                                        void (*ErrorCb) (   uint32_t Unit,
                                                            r_wm_Error_t Error
                                                        )
                                    );


/***********************************************************
  Function: R_WM_ErrorHandler

  Description:
  The function is the driver’s central error handler. 
  If the application has set an error handler call-back function, 
  the central error handler shall call it and then return to its caller. 
  The central error handler shall return in case no 
  error handler call-back function has been set.
  
  Unit   -   See description of <R_WM_DevInit>
  Error  -   See <r_wm_Error_t>.

  Return value:
  No return value.

*/
void R_WM_ErrorHandler  (   uint32_t      Unit, 
                            r_wm_Error_t  Error
                        );



/***********************************************************
  Function: R_WM_ColorModeBitsPerPixGet

  Description:
  Return the bits per pixel count for the specified format.
  
  Parameter:
  Format    -   Color format
  
  Return value:
  Bits per pixel count of the specified format.
*/
uint8_t R_WM_ColorFmtBitsPerPixGet(r_wm_WinColorFmt_t Format);

#ifdef __cplusplus
}
#endif


#endif /* R_WM_API_H__ */
