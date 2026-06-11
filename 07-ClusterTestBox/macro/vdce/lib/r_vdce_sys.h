/*
****************************************************************************
PROJECT : VDCE driver
FILE    : $Id: r_vdce_sys.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
VDCE support functions
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

/***************************************************************************
  Title: VDCE Driver Support Functions

  VDCE (Video Data Controller E) driver support functions

  The generic VDCE driver uses these functions. They have to be implemented 
  within the driver library for a concrete device. (e.g. D1L, D1M, D1H)

  Support functions are functions that are not part of the driver itself 
  but they must be provided to integrate the driver on a particular board.

*/

#ifndef R_VDCE_SYS_H
#define R_VDCE_SYS_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
  Section: Global Defines
*/

/***************************************************************************
  Section: Global Types
*/

/***************************************************************************
  Group: Enumerations and Types
*/

/***************************************************************************
  typedef: r_vdce_DeviceInfo_t

  Description:
  The structure holding the value of device inforamtion in the function 
  <R_VDCE_Sys_DeviceInfoGet>.

  Struct members:
  Device       - Device version number that VDCE assumes.
  VIWithDataEn - availability of video input with data enable signal.

*/
typedef struct
{
    r_dev_Device_t  Device;
    uint8_t         VIWithDataEn;
} r_vdce_DeviceInfo_t;

/***************************************************************************
  Section: Global Functions
*/
/***************************************************************************
  Group: VDCE driver Basic interface functions
*/

/***************************************************************************
  Function: R_VDCE_Sys_Init

  The initialization code of environment-depend is implemented by this function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_Init(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_DeInit

  The de-initialization code of environment-depend is implemented by this function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_DeInit(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_BaseAddrGet 

  Get base address of macro instance (unit).
  
  Description:
  This function is called during initialization phase. 
  It returns the base address of the unit in the current device.

  Parameters:
  Unit       - Instance number
  
  Returns:
  base       - base address of macro instance (unit).
*/

uint32_t R_VDCE_Sys_BaseAddrGet(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_PixelClockSet

  Description:
  Set the pixel clock and setting up the divider accordingly. 
  The return value is the clock that was set, which will differ from the Clock parameter 
  because the clock setup has some restriction and can not achieve exactly the same value. 
  This function is called in case the pixel clock has to be adjusted 
  because pixel clock adjustment is not part of the video output macro.
  
  Parameters:
  Unit  -    It specifies the video output number.
  Clock -    This parameter specifies the wanted pixelclock. 
              The pixelclock that is really set can differ from 
              this value due to hardware restrictions.
  
  Returns:
  It returns the pixel clock really set or 0 if an error occurred.
*/

uint32_t R_VDCE_Sys_PixelClockSet(const uint32_t Unit,
                                  const uint32_t Clock);

/***************************************************************************
  Function: R_VDCE_Sys_VdceVersionGet

  This function returns the current macro version.
  It is used to check if the macro is supporting pre-multiplied 
  alpha blending.
  
  Parameters:
  None
  
  Returns:
  Current macro version
*/
uint32_t R_VDCE_Sys_VdceVersionGet(void);

/***************************************************************************
  Function: R_VDCE_Sys_IntcInit

  Init interrupts for the given macro instance.

  Parameters:
  Unit       - Instance number

  The macro has 3 interrupt channel 0,1 and 2. 
  All 3 Channels will be connected in the final device. 
  VBlank, VSync and Scanline interrupt are handled by interrupt channel 0, 1 and 2 respectively.
  The remaining interrupts are routed to channel 0, too.

  See also <r_vdce_IntType_t> 

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_IntcInit(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_IntcDeinit

  Disable all video output interrupts of interrupt controller (INTC).  

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_IntcDeinit(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_IntcEnable

  Enable the video output interrupts on the CPU side.  
  This function is called during initialization of the driver because 
  CPU interrupts controller is not part of the video output macro. 
  
  Parameters:
  Unit   -    It specifies the video output number.
  IntSel -    Interrupt sel
  
  Returns:
  See <r_vdce_Error_t> for more details.
*/

r_vdce_Error_t R_VDCE_Sys_IntcEnable(const uint32_t       Unit,
                                     const r_dev_IntSel_t IntSel);

/***************************************************************************
  Function: R_VDCE_Sys_IntcDisable

  Disable the video output interrupts on the CPU side.  

  The disable is needed during deinitialization phase.

  Parameters:
  Unit   -    It specifies the video output number.
  IntSel -    Interrupt sel

  Returns:
  See <r_vdce_Error_t> for more details.
*/

r_vdce_Error_t R_VDCE_Sys_IntcDisable(const uint32_t       Unit,
                                      const r_dev_IntSel_t IntSel);

/***************************************************************************
  Function: R_VDCE_Sys_PortInit

  Init port for the given macro instance.
  
  Description:
  This function is called during initialization phase. 
  It sets up the output ports of the video output macro and checks for pin multiplexing.  
  If the ports cannot be acquired due to resource restrictions an error is returned.

  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_PortInit(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_PortDeInit

  Set back port settings

  Description:
  This function is called during deinitialization phase. 
  It sets the ports of the video output macro back to the original setting. 
  
  Parameters:
  Unit -    It specifies the video output number.
  
  Returns:
  See <r_vdce_Error_t> for more details.
*/

r_vdce_Error_t R_VDCE_Sys_PortDeInit(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_HsyncActLevelSet

  Set Hsync signal level to be active high or active low by
  macro external hardware. In case Hsync level should be
  handled by VDCE itself, this function needs to return 0.
  
  Parameters:
  Unit       - Instance number
  Level      - Hsync level to be set.
               == 0 Active Low
               != 0 Active High
  
  Returns:
  == 0       - use VDCE internal Hsync level handling
  != 0       - Hsync level is handled outside the VDCE macro
*/
uint32_t R_VDCE_Sys_HsyncActLevelSet(const uint32_t Unit, const int32_t Level);

/***************************************************************************
  Function: R_VDCE_Sys_VsyncActLevelSet

  Set Vsync signal level to be active high or active low by
  macro external hardware. In case Vsync level should be
  handled by VDCE itself, this function needs to return 0.
  
  Parameters:
  Unit       - Instance number
  Level      - Vsync level to be set.
               == 0 Active Low
               != 0 Active High
  
  Returns:
  == 0       - use VDCE internal Hsync level handling
  != 0       - Vsync level is handled outside the VDCE macro
*/
uint32_t R_VDCE_Sys_VsyncActLevelSet(const uint32_t Unit, const int32_t Level);

/***************************************************************************
  Function: R_VDCE_Sys_ClockActEdgeSet

  Set pixel clock active transition (clock / data relation)
  to be active on falling or rising edge by macro external
  hardware. In case clock active edge should be
  handled by VDCE itself, this function needs to return 0.
  
  Parameters:
  Unit       - Instance number
  Edge       - active transition to be set.
               == 0 data change with rising edge of clock (positive clock)
               != 0 data change with falling edge of clock (negativ clock)
  
  Returns:
  == 0       - use VDCE internal Clock edge handling
  != 0       - Clock edge is handled outside the VDCE macro
*/
uint32_t R_VDCE_Sys_ClockActEdgeSet(const uint32_t Unit, const int32_t Edge);

/***************************************************************************
  Function: R_VDCE_Sys_DesyncActLevelSet

  Set DEsync signal level to be active high or active low by
  macro external hardware. In case DEsync level should be
  handled by VDCE itself, this function needs to return 0.
  
  Parameters:
  Unit       - Instance number
  Level      - DEsync level to be set.
               == 0 Active Low
               != 0 Active High
  
  Returns:
  == 0       - use VDCE internal Hsync level handling
  != 0       - Vsync level is handled outside the VDCE macro
*/
uint32_t R_VDCE_Sys_DesyncActLevelSet(const uint32_t Unit, const int32_t Level);


/***************************************************************************
  Group: VDCE driver OS interface functions
*/

/***************************************************************************
  Function: R_VDCE_Sys_Lock

  This function locks the VDCE driver access to the specified unit for other threads.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_Lock(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_Unlock

  This function unlocks the VDCE driver access to the specified unit for other threads.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_Unlock(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_AllLock

  This function locks the VDCE driver access to the all unit for other threads.

  Parameters:
  None.

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_AllLock(void);

/***************************************************************************
  Function: R_VDCE_Sys_AllUnlock

  This function unlocks the VDCE driver access to the all unit for other threads.

  Parameters:
  None.

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_AllUnlock(void);

/***************************************************************************
  Function: R_VDCE_Sys_VIChannelCheck

  This function returns the availability of specifed video input channel.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_VIChannelCheck(const uint32_t Unit);

/***************************************************************************
  Function: R_VDCE_Sys_MaxResolutionGet

  This function returns the resolution information.

  Parameters:
  ResolutionHmax - This is a pointer to the max horizontal resolution.
  ResolutionVmax - This is a pointer to the max vertical resolution.

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_MaxResolutionGet ( uint32_t* ResolutionHmax,
                                             uint32_t* ResolutionVmax );


/***************************************************************************
  Function: R_VDCE_Sys_DeviceInfoGet

  This function returns the device information.

  Parameters:
  DevInfo - This is a pointer to information that is different from each devices.
            See <r_vdce_DeviceInfo_t>.

  Returns:
  see: <r_vdce_Error_t>
*/

r_vdce_Error_t R_VDCE_Sys_DeviceInfoGet(r_vdce_DeviceInfo_t* DevInfo);


#ifdef __cplusplus
}
#endif

#endif /* R_VDCE_SYS_H */

