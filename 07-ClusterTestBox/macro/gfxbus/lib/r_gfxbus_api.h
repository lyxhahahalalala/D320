/*
****************************************************************************
PROJECT : Gfx bus driver (wrapper)
FILE    : $Id: r_gfxbus_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for GFX bus  macro
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

#ifndef R_GFXBUS_API_H_
#define R_GFXBUS_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_HBUS
    #define R_GFXBUS_AREAS 4u
#else 
#ifdef USE_XBUS
    #define R_GFXBUS_AREAS 2u
#endif 
#endif    
/*******************************************************************************
  Title: GFXBUS API

  This a common API (wrapper) definition  for GFX buses (eg. hbus, xbus)
  An application should only need to include this header. The correct bus beneath
  will be selected in the device make file. The according function calls are handled 
  on the device (sys) level    

*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_XBUS_VERSION_HI and R_XBUS_VERSION_LO

  Driver version information  
*/

#define R_GFXBUS_VERSION_HI 0
#define R_GFXBUS_VERSION_LO 1



/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_gfxbus_Error_t

  GFXBUS macro driver error code.

  Values:
  R_GFXBUS_ERR_OK          - No error
  R_GFXBUS_ERR_NG          - Unspecified error
  R_GFXBUS_ERR_RANGE       - wrong parameter range
  R_GFXBUS_ERR_LAST        - Delimiter 
*/

typedef enum {
    R_GFXBUS_ERR_OK         = 0x00u,
    R_GFXBUS_ERR_NG         = 0x01u,
    R_GFXBUS_ERR_RANGE      = 0x02u,
    R_GFXBUS_ERR_LAST
} r_gfxbus_Error_t;



/*******************************************************************************
  Enum: r_gfxbus_BuffFill_t

  BUffer fill method

  Values:
    R_GFXBUS_FILL_SEQUENCIAL - sequencial fill 
    R_GFXBUS_FILL_CF         - critical data first      
*/
typedef enum {
    R_GFXBUS_FILL_SEQUENCIAL = 0u,
    R_GFXBUS_FILL_CF         = 1
} r_gfxbus_BuffFill_t; 

/*******************************************************************************
  Enum: r_gfxbus_BuffPref_t

  Prefetch method. 
  
  Values:
    R_GFXBUS_PREFETCH_NO          -  no prefetch 
    R_GFXBUS_PREFETCH_INC         - incremental  
    R_GFXBUS_PREFETCH_DEC         - decremental 
    R_GFXBUS_PREFETCH_RESERVED    - do not use  

*/
typedef enum {
    R_GFXBUS_PREFETCH_NO   = 0u,
    R_GFXBUS_PREFETCH_INC  = 1u,
    R_GFXBUS_PREFETCH_DEC  = 2u,
    R_GFXBUS_PREFETCH_RESERVED 
} r_gfxbus_BuffPref_t; 

/*******************************************************************************
  Enum: r_gfxbus_AreaMode_t

  Access mode for an addres range (area) 

  Values:
    R_GFXBUS_AM_DIRECT    - direct   access
    R_GFXBUS_AM_BUFFER    - buffered access 
    R_GFXBUS_AM_CACHE     - cached   access
    R_GFXBUS_AM_RESERVED  - do not use

*/
typedef enum {
    R_GFXBUS_AM_DIRECT = 0u,
    R_GFXBUS_AM_BUFFER = 1u, 
    R_GFXBUS_AM_CACHE  = 2u,
    R_GFXBUS_AM_RESERVED 
} r_gfxbus_AreaMode_t; 

/*******************************************************************************
  Enum: r_gfxbus_MasterAccess_t

  Permission to access CPU-SS from Bus Master.

  Values:
    R_GFXBUS_MASTER_ACCESS_DENIED    - Access not allowed
    R_GFXBUS_MASTER_ACCESS_ALLOWED   - Access allowed
    R_GFXBUS_MASTER_ACCESS_RESERVED  - do not use

*/
typedef enum {
    R_GFXBUS_MASTER_ACCESS_DENIED  = 0u,
    R_GFXBUS_MASTER_ACCESS_ALLOWED = 2u,
    R_GFXBUS_MASTER_ACCESS_RESERVED
} r_gfxbus_MasterAccess_t;

/*******************************************************************************
  Enum: r_gfxbus_Flush_t

  What will be flushed 
  
  Values:
    R_GFXBUS_DIRECT     - No buffer (no flush action)  
    R_GFXBUS_BUFFER     - Gfxbus buffer,
    R_GFXBUS_CACHE      - Gfxbus cache  
*/
typedef enum {
    R_GFXBUS_DIRECT = 0u,
    R_GFXBUS_BUFFER = 1u,
    R_GFXBUS_CACHE  = 2u
}r_gfxbus_Flush_t; 


/*******************************************************************************
  Type: r_gfxbus_AreaCfg_t

  Configuraton settings for a given memory area. 
  Attention:   The settings only have effect if the area is enabled.  
               Otherwise only the direct access mode is set.           
 
  Values:
      Mode      - access mode       , see <r_gfxbus_AreaMode_t> 
      Enable    - enable            , boolean
      BaseAddr  - start ddress of the memory area          
      AddrMask  - addres mask for the area                 


*/
typedef struct {
    r_gfxbus_AreaMode_t Mode; 
    uint8_t             Enable; 
    uint32_t            BaseAddr; 
    uint32_t            AddrMask; 
} r_gfxbus_AreaCfg_t; 


/*******************************************************************************
  Type: r_gfxbus_Cfg_t

  Configuraton of the (A)H-Bus.
  A note about Wait Insertion: 
  It is strongly recommended to use this feature for debug purpose only and to
  disable the wait limit by setting this register to zero!

  Values:

    BuffFill;           - buffer fill method   , see: <r_gfxbus_BuffFill_t>    
    BuffPref;           - prefetch type        , see: <r_gfxbus_BuffPref_t>    
    CacheFill;          - cache fill method    , see: <r_gfxbus_BuffFill_t>    
    WaitInsertion;      - the upper limit of bus clock that the GFXBUS may request a wait. 
    AreaCfg[4];         - memory area configuration, see  <r_gfxbus_AreaCfg_t>     
    MasterAccess;       - Bus master access permission, see <r_gfxbus_MasterAccess_t>

*/
typedef struct {
    r_gfxbus_BuffFill_t     BuffFill; 
    r_gfxbus_BuffPref_t     BuffPref; 
    r_gfxbus_BuffFill_t     CacheFill; 
    uint8_t                 WaitInsertion; 
    r_gfxbus_AreaCfg_t      AreaCfg[R_GFXBUS_AREAS]; 
    r_gfxbus_MasterAccess_t MasterAccess;
} r_gfxbus_Cfg_t; 


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_GFXBUS_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_GFXBUS_GetVersionStr(void);

/*******************************************************************************
  Function: R_GFXBUS_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_GFXBUS_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_GFXBUS_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_GFXBUS_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_GFXBUS_Config

  Configure memory bus 

  Parameters:
  Unit   - Instance number
  HCfg   - pointer to bus configuration <r_gfxbus_Cfg_t>
   
  Returns:
  see: <r_gfxbus_Error_t>
*/
r_gfxbus_Error_t R_GFXBUS_Config(uint32_t Unit, r_gfxbus_Cfg_t * Cfg); 

/*******************************************************************************
  Function: R_GFXBUS_Flush

  Flush data on bus. Depending on the given Flush (either type or address) 
  the whole cache or a selected area will be flushed   
  
  Parameters:
  Unit       - Instance number
  Flush      - What to flush (buffer or cache or adress ) see also: <r_gfxbus_Flush_t>
               for reference 
  Size       - Amount of data to flush (in byte) strarting from the given address    
  Returns:
  void
*/
void R_GFXBUS_Flush(uint32_t Unit, uint32_t Flush, uint32_t Size); 

/*******************************************************************************
  Function: R_GFXBUS_SetMode

  Flush data on bus  
  
  Parameters:
  Unit       - Instance number
  Area       - selected memory area
  Mode       - access mode, see <r_gfxbus_AreaMode_t> 
   
  Returns:
  see: <r_gfxbus_Error_t>
*/
r_gfxbus_Error_t R_GFXBUS_SetMode(uint32_t Unit, uint8_t Area, r_gfxbus_AreaMode_t Mode); 

/*******************************************************************************
  Function: R_GFXBUS_Init

  Driver init function.

  Parameters:
  Unit       - Instance number
 
  Returns:
  see: <r_gfxbus_Error_t>
*/

int32_t R_GFXBUS_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_GFXBUS_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_gfxbus_Error_t>
*/

int32_t R_GFXBUS_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_GFXBUS_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback) 
                  with two paramerters (unit and error number) 
  
  Returns:
  void
 
*/

void R_GFXBUS_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error));



#ifdef __cplusplus
}
#endif

#endif /* R_GFXBUS_API_H_  */
