/*
****************************************************************************
PROJECT : GFXBUS driver
FILE ID : $Id: r_gfxbus_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for GFXBUS macro
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

#ifndef R_GFXBUS_SYS_H_
#define R_GFXBUS_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: GFXBUS Support Functions

  The generic GFXBUS driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_GFXBUS_SYS_Config

  Configure memory bus 

  Parameters:
  Unit   - Instance number
  HCfg   - pointer to bus configuration <r_gfxbus_Cfg_t>
   
  Returns:
  see: <r_gfxbus_Error_t>
*/
r_gfxbus_Error_t R_GFXBUS_SYS_Config(uint32_t Unit, r_gfxbus_Cfg_t * Cfg); 

/*******************************************************************************
  Function: R_GFXBUS_SYS_Flush

  Unit       - Instance number
  FlushAddr  - What to flush 
  Size       - Size of data to be flushed      
  Returns:
  void
*/
void R_GFXBUS_SYS_Flush(uint32_t Unit, uint32_t Flush, uint32_t Size); 

/*******************************************************************************
  Function: R_GFXBUS_SYS_SetMode

  Flush data on bus  
  
  Parameters:
  Unit       - Instance number
  Area       - selected memory area
  Mode       - access mode, see <r_gfxbus_AreaMode_t> 
   
  Returns:
  see: <r_gfxbus_Error_t>
*/
r_gfxbus_Error_t R_GFXBUS_SYS_SetMode(uint32_t Unit, uint8_t Area, r_gfxbus_AreaMode_t Mode); 

/*******************************************************************************
  Function: R_GFXBUS_SYS_Init

  Driver init function.

  Parameters:
  Unit       - Instance number
 
  Returns:
  see: <r_gfxbus_Error_t>
*/

int32_t R_GFXBUS_SYS_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_GFXBUS_SYS_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_gfxbus_Error_t>
*/

int32_t R_GFXBUS_SYS_DeInit(uint32_t Unit);



/*******************************************************************************
  Function: R_GFXBUS_SYS_BusBusy
  Ckeck if there is a transfer ongoing on the given bus 
  
  Parameters:
  Unit       - GFXBUS number
  
  Returns:
  == TRUE    - transfer ongoing 
  != TRUE    - no activity  

*/

r_Error_t R_GFXBUS_SYS_BusBusy(uint32_t Unit); 


#ifdef __cplusplus
}
#endif


#endif /* R_GFXBUS_SYS_H_ */
