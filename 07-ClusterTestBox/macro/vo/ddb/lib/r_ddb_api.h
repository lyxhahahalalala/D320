/*
****************************************************************************
PROJECT : DDB (Display DataBase) driver
FILE    : $Id: r_ddb_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
database for display timings (ddb)
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

#ifndef R_DDB_API_H_
#define R_DDB_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Display Data Base

  The header file r_ddb_api.h contains the declarations related to 
  display(s).
  
  In the module r_ddb_timing.c, a list of displays are defined.
  This list is not exhaustive and the user can add his own 
  display if it is missing.
*/



/*******************************************************************************
  Section: Enumerations 
*/

/*******************************************************************************
  Enum: r_ddb_TimingFlags_t

  DDB Timing Flags.
  List of all available Timing Flags of the Display DataBase

  Values:
  R_DDB_DISP_FLAG_NONE         - empty flag
  R_DDB_DISP_FLAG_VOEN         - display requires VOEN (DATA ENABLE) signal instead of VSYNC
  R_DDB_DISP_FLAG_CSYNC        - display requires CSYNC signal instead of H/VSYNC
  R_DDB_DISP_FLAG_NEGCLK       - display requires neg. DCLK (data out with falling edge,
                                                            data stable with rising edge of VOnCLK)
  R_DDB_DISP_FLAG_HSYNC_ACTHI  - use active high level for HSYNC
  R_DDB_DISP_FLAG_VSYNC_ACTHI  - use active high level for VSYNC
  R_DDB_DISP_FLAG_DESYNC_ACTHI - use active high level for DATE ENABLE SYNC
*/

typedef enum
{
    R_DDB_DISP_FLAG_NONE         = (uint8_t)(0uL << 0u),
    R_DDB_DISP_FLAG_VOEN         = (uint8_t)(1uL << 0u),
    R_DDB_DISP_FLAG_CSYNC        = (uint8_t)(1uL << 1u),
    R_DDB_DISP_FLAG_NEGCLK       = (uint8_t)(1uL << 2u),
    R_DDB_DISP_FLAG_HSYNC_ACTHI  = (uint8_t)(1uL << 3u),
    R_DDB_DISP_FLAG_VSYNC_ACTHI  = (uint8_t)(1uL << 4u),
    R_DDB_DISP_FLAG_DESYNC_ACTHI = (uint8_t)(1uL << 5u)
} r_ddb_TimingFlags_t;


/*******************************************************************************
  Section: Structures
*/

/*******************************************************************************
  Typedef: r_ddb_SubTiming_t

  Sub-Timing information of horizontal / vertical display line.
 
> 
>           +-----------+
>  SYNC     |           |
>  ---------+           +------------------------------------
>                        
>                                +----------------------+
>  DATA EN                       |                      |
>  ------------------------------+                      +----
>                       
>    Front      Sync      Back
>  <-Porch-><---Width--><-Porch->
>  <---- BlankWidth ------------>
>  <------------------------- Total ------------------------>
>
>  BlankWidth = Front Porch + Sync Width + Back Porch
> 
  
  Members:
  Total          - length of total line
  BlankWidth     - length of blanking period
  FrontPorch     - length of front porch
  SyncWidth      - length of active sync pulse
*/     

typedef struct
{
    int32_t Total;
    int32_t BlankWidth;
    int32_t FrontPorch;
    int32_t SyncWidth;
} r_ddb_SubTiming_t;


/*******************************************************************************
  typedef: r_ddb_Timing_t

  Timing information of display

  Members:
  Name           - display name as unique idendifier a specific display
  ScreenWidth    - width of screen (in pixel)
  ScreenHeight   - height of screen (in pixel)
  H              - horizontal sub-timing see: <r_ddb_SubTiming_t>
  V              - vertical sub-timing see: <r_ddb_SubTiming_t>
  Flags          - Timing Flags see: <r_ddb_TimingFlags_t>
  PixelClock     - pixel clock frequency (in MHz)
*/     

typedef struct
{
    const char          *Name;
    int32_t             ScreenWidth;
    int32_t             ScreenHeight;
    r_ddb_SubTiming_t   H;
    r_ddb_SubTiming_t   V;
    uint32_t            Flags;
    uint32_t            PixelClock;
} r_ddb_Timing_t;


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_DDB_GetDisplayTiming

  Get a timing struct from the database.
  
  Parameters:
  DisplayId    - character string that describes the timing in the database
    
  Returns:
  timing       - timing struct that contains display timing information
                 see <r_ddb_Timing_t> for details 
*/
r_ddb_Timing_t *R_DDB_GetDisplayTiming(const char *DisplayId);


#ifdef __cplusplus
}
#endif

#endif /* R_DDB_API_H_ */

