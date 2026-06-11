/*
****************************************************************************
PROJECT : WM
FILE    : $Id: r_sys_wm.h 7658 2016-02-15 14:46:21Z matthias.nippert $
============================================================================ 
DESCRIPTION
WM system functions for D1x
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

#ifndef SYS_WM_H__
#define SYS_WM_H__

/***********************************************************
  Title: r_sys_wm.h
  
  D1x WM Support Functions

  Declaration of local WM support functions for the D1x
  device.
*/


/***********************************************************
  Section: Global Defines
*/

#define LOC_VBLANK_SYNC

#define R_VDCE_MAX_LAYERS               4
#define R_VDCE_MAX_LAYER_PITCH          8192  /* 0: no pitch supported */
#define R_VDCE_MAX_LAYER_WIDTH          1280
#define R_VDCE_MAX_LAYER_HEIGHT         768
#define R_VDCE_MAX_SPRITES_PER_LAYER    16

#define R_VDCE_MAX_CAPTURE              1


/***********************************************************
  Section: Constants
*/

/* Set if RLE Unit is active. This is used by the opposite WM Unit to correctly configure their shared resources. */
#define R_WM_SYS_GLOBAL_STATUS_FLAG_RLE                 (0x00000001uL)   /*Bit 0*/
/* Set if a window was visible during the current frame. If not, special care is taken for Sprite engine updates. */
#define R_WM_SYS_GLOBAL_STATUS_FLAG_WINDOWS_VISIBLE     (0x00000002uL)   /*Bit 1*/
/* Set if capturing is activated. This is used to to prevent deletion of active capturing windows */
#define R_WM_SYS_GLOBAL_STATUS_FLAG_CAPTURE_ACTIVE      (0x00000004uL)   /*Bit 2*/
/* Set to shutdown the WM driver. This triggers early exit of some processing loops. */
#define R_WM_SYS_GLOBAL_STATUS_FLAG_SHUTDOWN            (0x80000000uL)   /*Bit 31*/

/* A sprite configuration has changed and needs to be updated */
#define R_WM_SYS_FRAME_STATUS_SPEA_UPDATE_MASK          (0x00000007uL)

#define R_WM_SYS_FRAME_STATUS_SPEA_RLE_UPDATE_BIT       (0uL)
#define R_WM_SYS_FRAME_STATUS_SPEA_SU0_UPDATE_BIT       (1uL)
#define R_WM_SYS_FRAME_STATUS_SPEA_SU1_UPDATE_BIT       (2uL)
#define R_WM_SYS_FRAME_STATUS_SPEA_SU2_UPDATE_BIT       (3uL)

/* The sprite order has changed and needs to be reassigned */
#define R_WM_SYS_FRAME_STATUS_SPEA_SU0_REASSIGN_BIT     (4uL)
#define R_WM_SYS_FRAME_STATUS_SPEA_SU1_REASSIGN_BIT     (5uL)
#define R_WM_SYS_FRAME_STATUS_SPEA_SU2_REASSIGN_BIT     (6uL)


/* The CLUT or the layer order has changed and the CLUT must be updated */
#define R_WM_SYS_FRAME_STATUS_LAYER_CLUT_UPDATE_MASK    (0x00000780uL)

#define R_WM_SYS_FRAME_STATUS_LAYER_0_CLUT_UPDATE_BIT   (7uL)
#define R_WM_SYS_FRAME_STATUS_LAYER_1_CLUT_UPDATE_BIT   (8uL)
#define R_WM_SYS_FRAME_STATUS_LAYER_2_CLUT_UPDATE_BIT   (9uL)
#define R_WM_SYS_FRAME_STATUS_LAYER_3_CLUT_UPDATE_BIT   (10uL)

/***********************************************************
  Section: Local Variables
*/


/***********************************************************
  Section: Global Variables
*/

typedef struct
{
    const r_wm_Window_t* Win;
    uint32_t Layer;
} r_wm_sys_vdce_WinLayerPair_t;


/***********************************************************
  Type: r_wm_DevSys_t

  Device instance specific data.
  
  All instance specific data is kept within that structure.

  Members:
  EvQueueRingBuf    -     Buffer for the messages of the window manager
  WinToLayerMap     -     Map for which whindow is assigned to which layer
  HwLayerEnabled    -     Which window is currently set to Enabled
  RootWin           -     pointer to first (lowest) window
  RootCapt          -     pointer to first (only) capture
  RegEvents         -     Keep track of registered events. Bitfield, check <r_wm_Event_t> for available bits.
  EventCb           -     Callback function for system events
  FrameStatusFlags  -     These flags may be set during the processing of a frame in order to toggle updates after finishing the current message queue.
                            There are changes written that need to be transferred to the shadow buffers of the SPEA
                          Bit 0: R_SPEA_RLE Register Update Required
                          Bit 1: R_SPEA_SU0 Register Update Required
                          Bit 2: R_SPEA_SU1 Register Update Required
                          Bit 3: R_SPEA_SU2 Register Update Required
                            There are changes in the order of the sprites (Create, Delete, Z-Order change)
                          Bit 4: R_SPEA_SU0 Reassignment Required
                          Bit 5: R_SPEA_SU1 Reassignment Required
                          Bit 6: R_SPEA_SU2 Reassignment Required
                          FrameStatusFlags is reset after each frame.
  GlobalStatusFlags -     Keep Track of different HW Status
  HorizontalMoveReserve - Keep track of how many pixels a layer can be moved into negative horizontal direction.
*/

typedef struct r_wm_DevSys_s
{
    r_cdi_RBuf_t                    EvQueueRingBuf;
    r_wm_sys_vdce_WinLayerPair_t    WinToLayerMap[R_VDCE_MAX_LAYERS];
    uint32_t                        HwLayerEnabled[R_VDCE_MAX_LAYERS];
    r_wm_Window_t                   *RootWin;
    r_wm_Capture_t                  *RootCapt;
    uint32_t                        RegEvents;
    void                            (*EventCb)(uint32_t Unit, const r_wm_Event_t *Event);
    uint32_t                        FrameStatusFlags;
    uint32_t                        GlobalStatusFlags;/* bit0 - RLE unit 0 in use, bit1 - RLE unit 1 in use */
    int32_t                         HorizontalMoveReserve;
} r_wm_DevSys_t;

R_EXTERN r_wm_DevSys_t r_wm_DevSys[R_WM_DEV_NUM];  /* will be initialized to zero by compiler/loader*/ 

/***********************************************************
  Variable: r_wm_DevSys

  Array for device instance specific data of type <r_wm_DevSys_t>
*/


#endif /* #define SYS_WM_H__ */
