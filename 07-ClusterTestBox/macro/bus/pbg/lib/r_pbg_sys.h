/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_pbg_sys.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic XPBus GUARD driver
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

#ifndef R_PBG_SYS_H_
#define R_PBG_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic PBus GUARD Support Functions

  The generic PBus GUARD driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: REG_OFFSETS

  Register offsets of the PBG Configuration registers.
*/
#define R_PBG_REG_CTL      0x00
#define R_PBG_REG_STAT     0x04
#define R_PBG_REG_ADDR     0x08
#define R_PBG_REG_TYPE     0x0C

/*******************************************************************************
  Constant: BIT_POSITIONS

  Bits of <R_PBG_REG_CTL>.
  (Omitting R_PBG_REG_CTL_ prefix as this is a driver internal file.)
*/
#define PROTLOCK_BIT    31
#define PROTVM_BIT      26
#define PROTUM_BIT      25
#define PROTPEID_BIT    17
#define VCID_BIT        9
#define PROTSPID_BIT    5
#define PROTDEB_BIT     4
#define PROTRDPDEF_BIT  3
#define PROTWRPDEF_BIT  2
#define PROTRD_BIT      1
#define PROTWR_BIT      0



/*******************************************************************************
  Section: Local Types
*/
typedef struct r_pbg_s {
    r_dev_PbgInstance_t Instance;   /* Instance of the Bus Guard */
    uint16_t ChannelActive;         /* Bit Mask of all configurable channels */
    uint32_t Base0;                 /* Base address for the guard channel configuration registers */
    uint32_t Base1;                 /* Base address for the guard registers */
} r_pbg_t;

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_PBG_SYS_HardwareInit

  Setup the HW part for the generic PBG.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_pbg_Error_t>
*/

r_pbg_Error_t R_PBG_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_PBG_SYS_HardwareDeInit

  Deinit of the HW part for the generic PBG.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_pbg_Error_t>
*/

r_pbg_Error_t R_PBG_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_PBG_SYS_BaseAddr

  Returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

r_pbg_Error_t R_PBG_SYS_GetInstance(r_pbg_Id_t Id, r_pbg_t *Instance);


/*******************************************************************************
  Function: R_PBG_ConfigureGuardSysCall_Internal
  
  Configures one PBus Guard Channel according to the settings supplied to this 
  function.
  This function also works, if the CPU is running in User Mode.
  
  This is the inernal implementation that does the configuration with elevated user rights.
  
  Parameters:
  Id            - Unique Identifier of the Guard and Channel to be configured
  Config        - Configuration to be written to the Guard Channel
*/

r_pbg_Error_t R_PBG_ConfigureGuardSysCall_Internal(r_pbg_Id_t Id, r_pbg_Config_t Config);


#if 0
/*******************************************************************************
  Function: R_PBG_SYS_EnableInt

  Enable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_pbg_Interrupt_t>
  
  Returns:
  void
*/

void R_PBG_SYS_EnableInt(uint32_t Unit, r_pbg_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_PBG_SYS_DisableInt

  Disable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_pbg_Interrupt_t>
  
  Returns:
  void
*/

void R_PBG_SYS_DisableInt(uint32_t Unit, r_pbg_Interrupt_t Interrupt);
#endif

#ifdef __cplusplus
}
#endif


#endif /* R_PBG_SYS_H_ */
