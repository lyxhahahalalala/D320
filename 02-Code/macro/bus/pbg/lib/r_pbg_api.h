/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_pbg_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic part of the macro driver
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

#ifndef R_PBG_API_H_
#define R_PBG_API_H_

#ifndef R_DEV_API_H_
#warning The "r_pbg_api.h" requires the previous inclusion of "r_dev_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: PB GUARD Driver API 
  
  An application using PB GUARD should only need to include r_xcg_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_PBG_VERSION_HI and R_PBG_VERSION_LO

  Driver version information  
*/

#define R_PBG_VERSION_HI 0
#define R_PBG_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/


/*******************************************************************************
  Enum: r_pbg_Error_t

  PBG macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_PBG_ERR_OK           - No error
  R_PBG_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_PBG_ERR_OK         = 0x00,
    R_PBG_ERR_NG         ,
    R_PBG_ERR_RANGE      ,
    R_PBG_ERR_LOCKED     ,
    R_PBG_ERR_NOTLOCKED  ,
    R_PBG_ERR_UNINITIALISED
} r_pbg_Error_t;


/*******************************************************************************
  Enum: r_pbg_Id_t

  PBus Guard Window. 
  
  This uniquely defines one PBus Guard Window via its Macro Instance and Channel Number.

  Values:
  Instance   - Instance of the Bus Guard
  Channel    - Number of the Channel to be configured
*/

typedef struct r_pbg_Id_s {
    r_dev_PbgInstance_t Instance;
    uint32_t            Channel;
} r_pbg_Id_t;



/*******************************************************************************
  Type: r_pbg_Config_t

  PBG macro guard window description. 
  
  Defines all properties for a specific guard window.

  Values:
  PROTLOCK    - Lock this register until next reset. 0: Write Enabled; 1: Write Ignored
  PROTVM      - Virtual Machine Mode Access. 0: Native mode; 1: Native and virtual mode
  PROTUM      - User Mode Access. 0: Supervisor mode; 1: Supervisor and user mode
  PROTPEID    - PEID Access. Each Bit n: 0: Disallow PEIDn; 1: Allow PEIDn
  VCID        - VCID Access. Each Bit n: 0: Disallow VCIDn; 1: Allow VCIDn
  PROTSPID    - SPID Access. Each Bit n: 0: Disallow SPIDn; 1: Allow SPIDn
  PROTDEB     - Debug Access. 0: Disallow Debugger; 1: Allow Debugger
  PROTRDPDEF  - Default Read Protection: 0: Allow Anyone; 1: Apply filter
  PROTWRPDEF  - Default Write Protection: 0: Allow Anyone; 1: Apply filter
  PROTRD      - Reading Masters that match the filer are: 0: Prohibited 1: Permitted
  PROTWR      - Writing Masters that match the filer are: 0: Prohibited 1: Permitted
*/

typedef struct r_pbg_Config_s {
    uint32_t PROTLOCK:1;
    uint32_t PROTVM:1;
    uint32_t PROTUM:1;
    uint32_t PROTPEID:8;
    uint32_t VCID:8;
    uint32_t PROTSPID:4;
    uint32_t PROTDEB:1;
    uint32_t PROTRDPDEF:1;
    uint32_t PROTWRPDEF:1;
    uint32_t PROTRD:1;
    uint32_t PROTWR:1;
} r_pbg_Config_t;



/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_PBG_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_PBG_GetVersionStr(void); 

/*******************************************************************************
  Function: R_PBG_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_PBG_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_PBG_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_PBG_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_PBG_Init

  Guard init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_pbg_Error_t>
*/

r_pbg_Error_t R_PBG_Init(void);
r_pbg_Error_t R_PBG_InitInstance(r_dev_PbgInstance_t Instance);
r_pbg_Error_t R_PBG_InitChannel (r_pbg_Id_t Id);


/*******************************************************************************
  Function: R_PBG_ConfigureGuard
  
  Configures one PBus Guard Channel according to the settings supplied to this 
  function.
  This function will not work, if the CPU is running in User Mode.
  
  Parameters:
  Id            - Unique Identifier of the Guard and Channel to be configured
  Config        - Configuration to be written to the Guard Channel
*/
r_pbg_Error_t R_PBG_ConfigureGuard(r_pbg_Id_t Id, r_pbg_Config_t Config);


/*******************************************************************************
  Function: R_PBG_ConfigureGuardSysCall
  
  Configures one PBus Guard Channel according to the settings supplied to this 
  function.
  This function also works, if the CPU is running in User Mode.
  
  Parameters:
  Id            - Unique Identifier of the Guard and Channel to be configured
  Config        - Configuration to be written to the Guard Channel
*/
r_pbg_Error_t R_PBG_ConfigureGuardSysCall(r_pbg_Id_t Id, r_pbg_Config_t Config);


/*******************************************************************************
  Function: R_PBG_ClearErrorStatus
*/
void R_PBG_ClearErrorStatus(r_pbg_Id_t Id);


/*******************************************************************************
  Function: R_PBG_GetErrorStatus
*/
uint32_t R_PBG_GetErrorStatus(r_pbg_Id_t Id);


/*******************************************************************************
  Function: R_PBG_GetErrorDetails
*/
uint32_t R_PBG_GetErrorDetails(r_pbg_Id_t Id, uint32_t *Addr, uint32_t *Type);



#ifdef __cplusplus
}
#endif

#endif /* R_PBG_API_H_  */
