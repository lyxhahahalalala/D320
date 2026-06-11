/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_xbus_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
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

#ifndef R_XBUS_API_H_
#define R_XBUS_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: XBUS Driver API 
  
  An application using XBUS should only need to include r_xbus_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_XBUS_VERSION_HI and R_XBUS_VERSION_LO

  Driver version information  
*/

#define R_XBUS_VERSION_HI 0
#define R_XBUS_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_xbus_Error_t

  XBUS macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_XBUS_ERR_OK           - No error
  R_XBUS_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_XBUS_ERR_OK         = 0x00,
    R_XBUS_ERR_NG         ,
    R_XBUS_ERR_RANGE      ,
    R_XBUS_ERR_LOCKED     ,
    R_XBUS_ERR_NOTLOCKED  
} r_xbus_Error_t;

/*******************************************************************************
  Enum: r_xbus_AreaMode_t

  Access mode for an addres range (area) 

  Values:
    R_XBUS_AM_DIRECT    - direct    access
    R_XBUS_AM_CACHE     - cached    access
    R_XBUS_AM_BUFF      - buffered  access
    R_XBUS_AM_RESERVED  - do not use

*/
typedef enum {
    R_XBUS_AM_DIRECT = 0u,
    R_XBUS_AM_CACHE  = 1u,
    R_XBUS_AM_BUFF   = 2u,
    R_XBUS_AM_RESERVED 
} r_xbus_AreaMode_t; 

/*******************************************************************************
  Type: r_xbus_AreaCfg_t

  Configuraton settings for a given memory area. 
  Attention:   The settings only have effect if the area is enabled.  
               Otherwise only the direct access mode is set.           
 
  Values:
      Mode      - access mode       , see <r_xbus_AreaMode_t> 
      Enable    - enable            , boolean
      BaseAddr  - start ddress of the memory area          
      AddrMask  - addres mask for the area                 


*/
typedef struct {
    r_xbus_AreaMode_t   Mode; 
    uint8_t             Enable;     
    uint32_t            BaseAddr; 
    uint32_t            AddrMask; 
} r_xbus_AreaCfg_t; 


/*******************************************************************************
  Type: r_xbus_Cfg_t

  Configuraton of the AXI-Bus.
  A note about Wait Insertion: 
  It is strongly recommended to use this feature for debug purpose only and to
  disable the wait limit by setting this register to zero!

  Values:

    BuffFill;           - buffer fill method   , see: <r_xbus_BuffFill_t>    
    BuffPref;           - prefetch type        , see: <r_xbus_BuffPref_t>    
    CacheFill;          - cache fill method    , see: <r_xbus_BuffFill_t>    
    WaitInsertion;      - the upper limit of bus clock that the XBUS may request a wait. 
    AreaCfg[4];         - memory area configuration, see  <r_xbus_AreaCfg_t>
                          Note: Area 0/1 is cached or direct, area 2/3 is bufferd or direct, 
                          DO NOT MIX      
    xbusMasterAccess;   - xbus master access permission, see <r_xbus_MasterAccess_t>

*/
typedef struct {
    uint8_t               WaitInsertion; 
    uint8_t               SecRewrite; 
    r_xbus_AreaCfg_t      AreaCfg[4]; 
} r_xbus_Cfg_t; 



/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_XBUS_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_XBUS_GetVersionStr(void);

/*******************************************************************************
  Function: R_XBUS_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_XBUS_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_XBUS_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_XBUS_GetMinorVersion(void); 

/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_XBUS_Config

  Configure memory bus. 

  Parameters:
  Unit   - Instance number
  Cfg    - pointer to bus configuration, see <r_xbus_Cfg_t>
           Note: Area 0/1 is cached or direct, area 2/3 is bufferd or direct, 
                          DO NOT MIX   
   
  Returns:
  see: <r_xbus_Error_t>
*/
r_xbus_Error_t R_XBUS_Config(uint32_t Unit, r_xbus_Cfg_t * Cfg); 


/*******************************************************************************
  Function: R_XBUS_AreaConfig

  Configure one cache area  

  Parameters:
  Unit   - Instance number
  Area   - Index of the cache area 
  Cfg    - pointer to area configuration, see  <r_xbus_AreaCfg_t>
   
  Returns:
  see: <r_xbus_Error_t>
*/

r_xbus_Error_t R_XBUS_AreaConfig(uint32_t Unit, uint8_t Area, r_xbus_AreaCfg_t * Cfg); 

/*******************************************************************************
  Function: R_XBUS_SetMode

  Flush data on bus  
  
  Parameters:
  Unit       - Instance number
  Area       - selected memory area
  Mode       - access mode, see <r_xbus_AreaMode_t> 
   
  Returns:
  see: <r_xbus_Error_t>
*/
r_xbus_Error_t R_XBUS_SetMode(uint32_t Unit, uint8_t Area, r_xbus_AreaMode_t Mode); 

/*******************************************************************************
  Function: R_XBUS_Flush

  Flush the cache and/or buffer data on the x-bus. 
  The flush operation to be performed is depending on the parameters provided     
  
  Parameters:
  Unit       - Instance number
  FlushAddr  - What to flush  0: flush All
                              1: flush all Cache
                              2: flush all Buffer
                             >2: Flush address 
  FlushSize  - Size of the data to be flushed (starting from the given address >2) 
                
  Returns:
  void 
*/
void R_XBUS_Flush(uint32_t Unit, uint32_t FlushAddr, uint32_t FlushSize);

/*******************************************************************************
  Function: R_XBUS_Invalidate

  invalidate the whole cache. Note: pending data is not written.  
  Parameters:
  Unit       - Instance number
                 
  Returns:
  void 
*/

 r_xbus_Error_t R_XBUS_Invalidate(uint32_t Unit); 

/*******************************************************************************
  Function: R_XBUS_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_xbus_Error_t>
*/

 r_xbus_Error_t R_XBUS_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_XBUS_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_xbus_Error_t>
*/

 r_xbus_Error_t R_XBUS_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_XBUS_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_XBUS_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));


/*******************************************************************************
  Function: R_XBUS_XCopy

  Copy data from <Src> to <Dst> until reaching source <End> address,
  This function is optimised for cached high-latency access over the XC Bus,
  such as VRAM, SDRA/B, Serial Flash on D1Lx and D1Mx.

  Parameters:
  Src   - Source address for copy operation
  Dst   - Destination address for copy operation
  End   - Source end address for copy operation
  
  Returns:
  void
*/
void R_XBUS_XCopy(uint32_t Src, uint32_t Dst, uint32_t End); 


#ifdef __cplusplus
}
#endif

#endif /* R_XBUS_API_H_  */
