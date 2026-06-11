/*
****************************************************************************
PROJECT : WM driver
FILE    : $Id: r_wm_sys.h 7817 2016-02-26 11:00:35Z matthias.nippert $
============================================================================ 
DESCRIPTION
Window Manager support functions
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
****************************************************************************
*/

#ifndef R_WM_SYS_H
#define R_WM_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: WM Support Functions for VDCE (internal)
  
  Window Manager SYS API
*/


void R_WM_Sys_LockWindows(uint32_t Unit);
void R_WM_Sys_UnlockWindows(uint32_t Unit);
void R_WM_Sys_LockMsgQueue(uint32_t Unit);
uint32_t R_WM_Sys_TryLockMsgQueue(uint32_t Unit);
void R_WM_Sys_UnlockMsgQueue(uint32_t Unit);
void R_WM_Sys_LockBuffers(uint32_t Unit);
void R_WM_Sys_UnlockBuffers(uint32_t Unit);
void R_WM_Sys_LockDevice(uint32_t Unit);
void R_WM_Sys_UnlockDevice(uint32_t Unit);
void R_WM_Sys_PostMsgSema(uint32_t Unit);
void R_WM_Sys_WaitMsgSema(uint32_t Unit);
uint32_t R_WM_Sys_IsShutdownActive(uint32_t Unit);


/***********************************************************
  Function: R_WM_Sys_DevInit
  
  Description:
    Intitializes the platform specific portion of the device.
    
  Parameter:
    Unit    - The VOUT unit number.
    EventCb - Callback function for receiving events.
    
  Return value:
    1 if successful, otherwise 0.
*/

/***********************************************************
  Group: Device
*/

/***********************************************************
  Function: R_WM_Sys_DevInit
*/
uint32_t R_WM_Sys_DevInit   (   uint32_t Unit,
                                void (*EventCb)(uint32_t Unit, const r_wm_Event_t *Event)
                            );
                            

/***********************************************************
  Function: R_WM_Sys_DevRootWindowSet
  
  Description:
    Sets the root window in SYS. This way the SYS layer
    can access the windows chain.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    RootWin - Root window
    
  Return value:
    None.
*/
void R_WM_Sys_DevRootWindowSet(uint32_t Unit, r_wm_Window_t* RootWin); 

/***********************************************************
  Function: R_WM_Sys_DevRootCaptureSet
  
  Description:
    Sets the capture device root in SYS. This way the SYS layer
    can access the capture devices chain.
    
  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    RootCapt    - Capture root
    
  Return value:
    None.
*/
void R_WM_Sys_DevRootCaptureSet(uint32_t Unit, r_wm_Capture_t* RootCapt);

/***********************************************************
  Function: R_WM_Sys_DevDeinit
  
  Description:
    Deinitializes the SYS portion of the device.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_DevDeinit(uint32_t Unit);

/***********************************************************
  Function: R_WM_Sys_DevCountGet
  
  Description:
    Gets the number of available VOUT units.
    
  Parameter:
    None.
    
  Return value:
    Number of VOUT units.
*/
uint32_t R_WM_Sys_DevCountGet(void);


/***********************************************************
  Function: R_WM_Sys_DevInfoGet
  
  Description:
    Gets the platform specific information.
    
  Parameter:
    MaxLayers               -   Maximum number of layers supported
    MaxPitch                -   Maximum layer pitch (stride) supported 
    MaxWidth                -   Maximum layer width supported
    MaxHeight               -   Maximum layer height supported
    SpritesOrderAscending   -   Sprites Z-order ascending or descending.
                                Ascending means that the sprite with the greater index
                                has the greater Z-order priority (comes on top of the sprites
                                with the lower index).
*/
void  R_WM_Sys_DevInfoGet   (   uint32_t    Unit,
                                uint32_t    *MaxLayers,
                                uint32_t    *MaxPitch,
                                uint32_t    *MaxWidth,
                                uint32_t    *MaxHeight,
                                uint32_t    *SpritesOrderAscending
                            );

/***********************************************************
  Function: R_WM_Sys_DevEventRegister
  
  Description:
    Registers for the notification on the external events, i.e.
    interrupts.
    
  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    EventId     - The ID of the event
    Arg         - Generic argument (if applicable for the event)
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_DevEventRegister  (   uint32_t        Unit,
                                        r_wm_EventId_t  EventId,
                                        uint32_t        Arg
                                    );
            
/***********************************************************
  Function: R_WM_Sys_DevWaitForHwWriteReady
    
  Description:
    Wait until it is safe (in terms of time to finish) to start
    writing the HW registers.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevWaitForHwWriteReady(uint32_t Unit);

/***********************************************************
  Function: R_WM_Sys_DevWaitForHwUpdated
  
  Description:
    Wait until the HW registers update is finished.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevWaitForHwUpdated(uint32_t Unit);

/***********************************************************
  Function: R_WM_Sys_DevFrameStarted
  
  Description:
    Will be called immediately before the HW update for one frame
    starts taking place.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevFrameStarted(uint32_t Unit);

/***********************************************************
  Function: R_WM_Sys_DevFrameFinished
  
  Description:
    Will be called immediately after the HW update for one frame
    has been finished.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevFrameFinished(uint32_t Unit);

/***********************************************************
  Group: Screen (display)
*/

/***********************************************************
  Function: R_WM_Sys_ScreenBgColorSet
  
  Description:
    Sets the background color of the display. The background color 
    can be thought as the lowest Z-order constant color layer.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Red, Green, Blue    - The components of the background color
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenBgColorSet  (   uint32_t Unit,
                                        uint8_t  Red,
                                        uint8_t  Green,
                                        uint8_t  Blue
                                    );

/***********************************************************
  Function: R_WM_Sys_ScreenColorCurveSet
  
  Description:
    Set a curve to be used as custom gamma or color correction curve. 
   Using this curve, each RGB color channel is individually corrected according 
   to the given curve.
   
   For the correction in VDCE, the incoming color information of each channel 
   is split into 32 equal segments each covering 8 color values.
   For these 8 values, the same gain factor applies.
   
   To configure the segments, a start and an end value need to be given. 
   This requires 33 reference points to be passed to this function.
   
   For each segment of each color, the gain factor must be in range [0.0 .. 2.0], 
   thus the values between two reference points may have a difference in range of [0 .. 16].
   
   This function will overwrite the settings of <R_WM_ScreenGammaSet>.
  
  Parameter:
   Unit          - See description of <R_WM_DevInit>
   NumEntries    - Number of reference points
   Clut          - Pointer to Table of reference points <r_wm_ClutEntry_t>. 
                   ALPHA value of data type is unused!
                   Note: The data pointed to by the pointer <Clut> must be avalilable 
                         as long as the Window using the CLUT color format is active.
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenColorCurveSet(  uint32_t Unit,
                                        const r_wm_ClutEntry_t *ColorCurve,
                                        uint32_t NumEntries
                                      );

/***************************************************************************
  Function: R_WM_Sys_ScreenGammaSet

  Description:
    This function sets the output gamma correction.
    
    This function will overwrite the settings of <R_WM_ScreenColorCurveSet>.

  Parameter:
    Unit    - See description of <R_WM_DevInit>
    Gamma   - Gamma correction factor (0..255 => 0.25..2.0, 128=1.0)

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenGammaSet(   uint32_t Unit, 
                                    const uint8_t GammaRed, 
                                    const uint8_t GammaGreen, 
                                    const uint8_t GammaBlue 
                                );

/***********************************************************
  Function: R_WM_Sys_ScreenEnable
  
  Description:
    Enables/disables the display.
  
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Enabled             - 0 : disabled, anything else : enabled
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenEnable(uint32_t Unit, uint32_t Enabled);

/***********************************************************
  Function: R_WM_Sys_ScreenTimingSet
  
  Description:
    Sets the screen timings manually. 
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Timing  - Timings
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenTimingSet(uint32_t Unit, r_ddb_Timing_t *timing);

/***********************************************************
  Function: R_WM_Sys_ScreenTimingSetByName
  
  Description:
    Sets the video timings by referencing the display database.
  
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenTimingSetByName     (   uint32_t        Unit,
                                                const int8_t    * Name
                                            );

/***********************************************************
  Function: R_WM_Sys_ScreenColorFormatSet

  Description:
    Set the color format for the signals of the specified video output.
    Default format for the video output is RGB888.

  Parameter:
    Unit   - See <R_WM_Sys_DevInit>
    OutFmt - A color format of <r_wm_OutColorFmt_t> plus optional bit flags.

  Return value:
    1 if successful, otherwise 0.
*/

uint32_t R_WM_Sys_ScreenColorFormatSet  (   uint32_t            Unit,
                                                r_wm_OutColorFmt_t  OutFmt
                                            );

/***********************************************************
  Group: Windows
*/

/***********************************************************
  Function: R_WM_Sys_WindowCreate
  
  Description:
    Creates a window.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Window              - Window structure pointer, see <r_wm_Window_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowCreate(uint32_t Unit, const r_wm_Window_t* Win);

/***********************************************************
  Function: R_WM_Sys_WindowDelete
  
  Description:
    Deletes a window.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Window              - Window structure pointer, see <r_wm_Window_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowDelete(uint32_t Unit, const r_wm_Window_t* Win);

/***********************************************************
  Function: R_WM_Sys_WindowPosSet
  
  Description:
    Sets the window position.
  
  Parameter:
    Unit                    - See <R_WM_Sys_DevInit>
    Window                  - Window structure pointer, see <r_wm_Window_t>
    PosX, PosY, and PosZ    - New window position
  
  Return value:
    1 if successful, otherwise 0.
    
  Note:
     Setting the PosZ can yield reconfiguring all HW layers.
*/
uint32_t R_WM_Sys_WindowPosSet(uint32_t Unit, const r_wm_Window_t* Win, int32_t PosX, int32_t PosY, int32_t PosZ);

/***********************************************************
  Function: R_WM_Sys_WindowEnable
  
  Description:
    Enables/disables the window.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>
    Enabled - Enabled/Disabled
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowEnable(uint32_t Unit, const r_wm_Window_t* Win, uint32_t Enabled);

/***********************************************************
  Function: R_WM_Sys_WindowSetFb
  
  Description:
    Sets the visible (front) buffer for the window.
  
  Parameter:
    Unit    -   See <R_WM_Sys_DevInit>
    Window  -   Window structure pointer, see <r_wm_Window_t>
    Fb      -   Buffer
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowSetFb(uint32_t Unit, const r_wm_Window_t* Win, const void* Fb);

/***********************************************************
  Function: R_WM_Sys_WindowAlphaSet
  
  Description:
    Sets the constant (overall) alpha value for the window. This
    values is then used in alpha blending of the stacked windows.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>
    Alpha   - Alpha value (0-255)
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowAlphaSet(uint32_t Unit, const r_wm_Window_t* Win, uint8_t Alpha);

/***********************************************************
  Function: R_WM_Sys_WindowPremultipliedAlphaEnable
  
  Description:
    Enables or disables the premultiplied alpha mode for the window. 
    This changes the formula used in alpha blending of the stacked windows.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>
    Enabled - True/False
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowPremultipliedAlphaEnable (uint32_t Unit, const r_wm_Window_t* Win, uint8_t Enabled);

/***********************************************************
  Function: R_WM_Sys_WindowFlagsUpdate
  
  Description:
    Provides various on/off switches for different functionalities of a WM window.
    Check <r_wm_WinFlags_t> for further descriptions.
  
  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    SetFlags    - New flags to be set
    ClearFlags  - New flags to be cleared
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowFlagsUpdate (uint32_t Unit, const r_wm_Window_t* Win, r_wm_WinFlags_t SetFlags, r_wm_WinFlags_t ClearFlags);

/***********************************************************
  Function: R_WM_Sys_WindowClutSet
  
  Description:
    Sets the color lookup-table for the window. This is only meaningful
    in case of a CLUT window color format, see <r_wm_WinColorFmt_t>.
  
  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    Window      - Window structure pointer, see <r_wm_Window_t>
    NumEntries  - The number of color lookup-table entries
    Clut        - The color lookup-table pointer
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowClutSet(uint32_t Unit, const r_wm_Window_t* Win, uint32_t NumEntries, const r_wm_ClutEntry_t* Clut);

/***********************************************************
  Function: R_WM_Sys_WindowColorKeyEnable
  
  Description:
    Enables/disables the color keying for the window. The color key
    is set in the window structure during initialization.
  
  Parameter:
    Unit    -   See <R_WM_Sys_DevInit>
    Window  -   Window structure pointer, see <r_wm_Window_t>
    Enabled -   Enabled/Disabled
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowColorKeyEnable(uint32_t Unit, const r_wm_Window_t* Win, uint32_t Enabled);

/***********************************************************
  Function: R_WM_Sys_WindowGeomSet
  
  Description:
    Sets the window dimensions.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>
    Pitch   - The distance in pixels between subsequent rows in the framebuffer memory (>= Width)
    Width   - The window framebuffer width in pixels
    Height  - The window framebuffer height in pixels
  
  Return value:
    1 if successful, otherwise 0.
  
  D1Mx specific:
    The pitch must be 128 bytes aligned.
*/
uint32_t R_WM_Sys_WindowGeomSet (   uint32_t            Unit,
                                    const r_wm_Window_t *Win,
                                    uint32_t            Pitch,
                                    uint32_t            Width,
                                    uint32_t            Height
                                );
                                
/***********************************************************
  Function: R_WM_Sys_WindowColorFmtSet
  
  Description:
    Changes the color format of the window. Every window has an initial
    color format specified in its structure during, and set after issuing
    <R_WM_WindowCreate>, which can be changed during window life-time
    by using this function.
  
  Parameter:
    Unit        -   See <R_WM_Sys_DevInit>
    Window      -   Window structure pointer, see <r_wm_Window_t>
    ColorFmt    -   See <r_wm_WinColorFmt_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowColorFmtSet (   uint32_t                Unit,
                                        const r_wm_Window_t     *Win,
                                        r_wm_WinColorFmt_t      ColorFmt
                                    );

/***********************************************************
  Function: R_WM_Sys_WindowDeleteAllSprites
  
  Description:
    Deletes all sprites on the window. This function is guaranteed
    to be faster than deleting sprites by issuing <R_WM_SpriteDelete>
    on all sprites on the window.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowDeleteAllSprites    (   uint32_t Unit,
                                                const r_wm_Window_t* Win
                                            );
/***********************************************************
  Group: Sprites
*/
/***********************************************************
  Function: R_WM_Sys_SpriteCreate
  
  Description:
    Add a sprite to the the window. The window must have its 'Mode' set
    to R_WM_WINMODE_SPRITES. Upon creation sprites are disabled. Sprite origin
    are calculated relative to the parent window origin.
    After successful creation, the window's 'Status' field will be set to
    R_WM_SPRITESTATUS_ENABLED.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Sprite  - Sprite structure pointer, see <r_wm_Sprite_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteCreate(uint32_t Unit, const r_wm_Sprite_t *Sprite);

/***********************************************************
  Function: R_WM_Sys_SpriteEnable
  
  Description:
    Enables/disabled the sprite. Upon creation sprites are disabled.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Sprite  - Sprite structure pointer, see <r_wm_Sprite_t>
    Enabled - Enabled/disabled
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteEnable(uint32_t Unit, const r_wm_Sprite_t *Sprite, uint32_t Enabled);

/***********************************************************
  Function: R_WM_Sys_SpriteDelete
  
  Description:
    Deletes the sprite from the window. After successful deletion, 
    the sprite's 'Status' field will be changed to 
    R_WM_SPRITESTATUS_NOT_INITIALIZED.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Sprite  - Sprite structure pointer, see <r_wm_Sprite_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteDelete(uint32_t Unit, const r_wm_Sprite_t *Sprite);

/***********************************************************
  Function: R_WM_Sys_SpriteMove
  
  Description:
    Moves the sprite on the parent window.
  
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Sprite              - Sprite structure pointer, see <r_wm_Sprite_t>
    PosX, PosY, PosZ    - New sprite position
  
  Return value:
    1 if successful, otherwise 0.
    
  Note:
    Changing PosZ can yield HW reconfiguration of all sprites on the same window.
*/
uint32_t R_WM_Sys_SpriteMove(uint32_t Unit, const r_wm_Sprite_t *Sprite, uint32_t PosX, uint32_t PosY, uint32_t PosZ);

/***********************************************************
  Function: R_WM_Sys_SpriteBufSet
  
  Description:
    Sets the sprite's buffer.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteBufSet(uint32_t Unit, const r_wm_Sprite_t *Sprite, void* Buf);

/***********************************************************
  Group: Video Capture
*/
/***********************************************************
  Function: R_WM_Sys_CaptureCreate
  
  Description:
    The HW specific part of the <R_WM_CaptureCreate>.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Capt    - Capture device object pointer, see <r_wm_Capture_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_CaptureCreate(uint32_t Unit, const r_wm_Capture_t *Capt);

/***********************************************************
  Function: R_WM_Sys_CaptureDelete
  
  Description:
    The HW specific part of the <R_WM_CaptureDelete>.

  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Capt    - Capture device pointer, see <r_wm_Capture_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_CaptureDelete(uint32_t Unit, const r_wm_Capture_t *Capt);

/***********************************************************
  Function: R_WM_Sys_CaptureEnable
  
  Description:
    The HW specific part of the <R_WM_CaptureEnable> and <R_WM_CaptureDisable>.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>
    Enabled - Enabled/disabled flag
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_CaptureEnable(uint32_t Unit, const r_wm_Capture_t *Cap, uint32_t Enabled);

/***********************************************************
  Group: Message Queue
*/

/***********************************************************
  Function: R_WM_Sys_MsgQueueSetup
  
  Description:
    Setup the message queue.
  
  Parameter:
    Unit                -   See <R_WM_Sys_DevInit>
    MsgQueueStorage     -   Storage for the message queue
    Size                -   Number of elements in the message queue
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_MsgQueueSetup(uint32_t Unit, void* MsgQueueStorage, uint32_t Size);

/***********************************************************
  Function: R_WM_Sys_MsgQueueRead
  
  Description:
    Reads one message from the queue.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Msg     - Pointer to the message, see <r_wm_Msg_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_MsgQueueRead(uint32_t Unit, r_wm_Msg_t *Msg);

/***********************************************************
  Function: R_WM_Sys_MsgQueueWrite
  
  Description:
    Writes one message to the queue.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Msg     - Pointer to the message, see <r_wm_Msg_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_MsgQueueWrite(uint32_t Unit, r_wm_Msg_t *Msg);

/***********************************************************
  Group: Utility
*/

/***********************************************************
  Function: R_WM_Sys_Alloc
  
  Description:
    Allocates memory. This function will be used by the framework
    if the window FB mode is R_WM_WINBUF_ALLOC_INTERNAL.
  
  Parameter:
    Size                - Size of the memory block
    MemType             - The type of the memory to be allocated, <see r_wm_Memory_t>
    
  Return value:
    Pointer to the allocated memory.
*/
void *R_WM_Sys_Alloc    (   uint32_t      Size,
                            r_wm_Memory_t MemType
                        );

/***********************************************************
  Function: R_WM_Sys_Free
  
  Description:
    Deallocates memory allocated with <R_WM_Sys_Free>. This function will
    be used by the framework if the window FB mode is R_WM_WINBUF_ALLOC_INTERNAL.
    
  Parameter:
    Memory      - Memory pointer
    MemType     - The type of the memory to be allocated, <see r_wm_Memory_t>
    
  Return value:
    1 if successful, otherwise 0.
   
*/
uint32_t R_WM_Sys_Free  (   void         *Memory,
                            r_wm_Memory_t MemType
                        );

/***********************************************************
  Function: R_WM_Sys_Heap_Set

  Description:
    This will set the platform specific heaps.

*/
void R_WM_Sys_Heap_Set(const void *Cpu, const void *Video);

/***********************************************************
  Function: R_WM_Sys_GetLastError

  Request information about the last error that happened in the SYS layer of the WM.
  Calling this function will reset the overflow flag and allows for a new error to be recorded.
  
  Parameter:
  Unit              - Reference parameter to optionally get the WM Unit that caused the error
  AdditionalInfo    - Reference parameter to optionally get additional error codes from VDCE or SPEA
  Overflow          - Reference parameter to optionally get information if errors happened after this one occurred
  New               - Reference parameter to optionally check if this is a new error with respect to the last call to R_WM_Priv_GetLastError
  
  Returns:
  r_wm_Error_t      - Error code of the error.
*/

r_wm_Error_t R_WM_Sys_GetLastError( uint32_t *Unit, 
                                        uint32_t *AdditionalInfo, 
                                        uint32_t *Overflow, 
                                        uint32_t *New);

/***********************************************************
  Function: R_WM_Cap_CapBufGet

  Description:
  To be used on Video Output Sync.
  Grabs a new buffer to be assigned for video input capturing.

  Returns:
  0   - if Buffer States do not match
  Buf - if Buffer State change successful
*/
r_wm_WinBuffer_t *R_WM_Cap_CapBufGet( uint32_t Unit, r_wm_Window_t* Window );
                                
/***********************************************************
  Function: R_WM_Cap_DispBufGet

  Description:
  To be used on Video Input Sync.
  Grabs a captured buffer to be assigned for video output display.

  Returns:
  0   - if Buffer States do not match
  Buf - if Buffer State change successful
*/
r_wm_WinBuffer_t *R_WM_Cap_DispBufGet( uint32_t Unit, r_wm_Window_t*  Window );




#ifdef __cplusplus
}
#endif  


#endif /* R_WM_SYS_H__ */
