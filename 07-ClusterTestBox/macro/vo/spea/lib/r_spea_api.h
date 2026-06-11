/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_spea_api.h 7904 2016-03-09 08:54:34Z florian.zimmermann $
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

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

****************************************************************************
*/

#ifndef R_SPEA_API_H_
#define R_SPEA_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: SPEA Driver API 
  
  An application using SPEA should only need to include r_spea_api.h
  
*/

/*******************************************************************************
  Section: Global Constants 
*/

/*******************************************************************************
  Constant:  R_SPEA_VERSION_HI and R_SPEA_VERSION_LO

  Driver version information  
*/

#define R_SPEA_VERSION_HI 1
#define R_SPEA_VERSION_LO 13



/*******************************************************************************
  Section: Global Types
*/


/*******************************************************************************
  Enum: r_spea_Error_t

  SPEA macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_SPEA_ERR_OK       - No error
  R_SPEA_ERR_NG       - Unspecified error
  R_SPEA_ERR_RANGE    - Out of range error 
  R_SPEA_ERR_PARAM    - Wrong Parameter
  R_SPEA_ERR_TIME     - Timeout 
  R_SPEA_ERR_ADDR     - Wrong address (not aligned correctly) 
*/

typedef enum
{
    R_SPEA_ERR_OK     = 0x00u,
    R_SPEA_ERR_NG     = 0x01u,
    R_SPEA_ERR_RANGE  = 0x02u,
    R_SPEA_ERR_PARAM  = 0x03u,
    R_SPEA_ERR_TIME   = 0x04u, 
    R_SPEA_ERR_ADDR   = 0x05u
} r_spea_Error_t;

/*******************************************************************************
  Enum: r_spea_Color_t

  Configuration data for a single Sprite 

  Values:
  R_SPEA_CM_8BPP      - color mode:  8 bit per pixel 
  R_SPEA_CM_18BPP     - color mode: 18 bit per pixel 
  R_SPEA_CM_24BPP     - color mode: 24 bit per pixel 
  R_SPEA_CM_LIMIT     - delimiter
*/

typedef enum
{
    R_SPEA_CM_8BPP    = 0x00,
    R_SPEA_CM_18BPP   = 0x01,
    R_SPEA_CM_24BPP   = 0x02,
    R_SPEA_CM_LIMIT   = 0x03
    
} r_spea_Color_t;


/*******************************************************************************
  Enum: r_spea_Unit_t

  List of Units in one Sprite engine macro  

  Values:
  R_SPEA_RLE0          - RLE 0 
  R_SPEA_RLE1          - RLE 1  
  R_SPEA_SU_OFFSET    -  Offset for correct index calculation
  R_SPEA_SU0          - Sprite unit 0 
  R_SPEA_SU1          - Sprite unit 1 
  R_SPEA_SU2          - Sprite unit 2
  R_SPEA_SU_LAST      - delimiter 
*/

typedef enum
{
    R_SPEA_RLE0       = 0x00u,
    R_SPEA_RLE1       = 0x01u,
    R_SPEA_SU_OFFSET  = 0x02u,
    R_SPEA_SU0        = 0x02u,
    R_SPEA_SU1        = 0x03u,
    R_SPEA_SU2        = 0x04u, 
    R_SPEA_SU_LAST    = 0x05u,

} r_spea_Unit_t;

/*******************************************************************************
  Struct: r_spea_SpriteConfig_t

  Configuration data for a single Sprite 

  Members:
  Enable              - 1: Sprite enabled/visible, 0: disabled/not visible
  VSync               - Select what VSync (from VO0 or VO1) will trigger register update   
  SrcAddr             - Location of the sprite data in memory
  Height              - Sprite height in pixel 
  Width               - Sprite width in pixel
  PosX                - X Position in pixel on the virtual layer
  PosY                - y Position in pixel on the virtual layer    

*/
typedef struct 
{
    uint8_t            Enable; 
    uint8_t            VSync; 
    uint32_t           SrcAddr;
    uint16_t           Height; 
    uint16_t           Width; 
    uint16_t           PosX; 
    uint16_t           PosY; 
} r_spea_SpriteConfig_t; 


/*******************************************************************************
  Struct: r_spea_RleConfig_t

  Configuration data for a RLE unit

  Members:
  VirtStar            - Start address on the virtual layer 
  MemStart            - Start address in physical memory 
  ColorMode           - 8, 18, 24 bpp, see <r_spea_Color_t> 
  VideoOutID          - Axi busmaster ID to be served by the RLE unit    
*/

typedef struct 
{
    uint32_t          VirtStart; 
    uint32_t          MemStart; 
    r_spea_Color_t    ColorMode; 
    uint8_t           VideoOutID; 
} r_spea_RleConfig_t; 


/*******************************************************************************
  Struct: r_spea_EngineConfig_t

  Configuration data for a Sprite Engine 

  Members:
  RleActive         - 0: RLE in bus-through mode, 1 active  
  Su0Active         - 0: Sprite Unit 0 in bus-through mode, 1 active  
  Su1Active         - 0: Sprite Unit 1 in bus-through mode, 1 active  
  Su2Active         - 0: Sprite Unit 2 in bus-through mode, 1 active      
*/

typedef struct 
{
    uint8_t RleActive; 
    uint8_t Su0Active; 
    uint8_t Su1Active; 
    uint8_t Su2Active; 
} r_spea_EngineConfig_t; 


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_SPEA_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_SPEA_GetVersionStr(void);

/*******************************************************************************
  Function: R_SPEA_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_SPEA_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_SPEA_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_SPEA_GetMinorVersion(void); 

/*******************************************************************************
  Function: R_SPEA_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_SPEA_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_spea_Error_t>
*/

 r_spea_Error_t R_SPEA_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SPEA_SetSprite

  Set configuration for the given sprite.

  Parameters:
  Unit          - SpriteEngine Macro instance number
  SpEng         - Sprite unit number 
  SpNum         - Sprite number (0 - 16)  
  SpConf        - Pointer to sprite configuration data, see <r_spea_SpriteConfig_t> 
  DispBpp       - Byte per pixel display setting, 
                  required for pixel to byte position calculation
 
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_SetSprite(uint32_t                Unit, 
                                uint8_t                 SpEng, 
                                uint8_t                 SpNum, 
                                r_spea_SpriteConfig_t  *SpConf,
                                uint8_t                 DispBpp);


/*******************************************************************************
  Function: R_SPEA_SetRle

  Set configuration for the given RLE.
  Always make sure to assign different ID (VideoOutID) for both RLE units 
  Thie device (and thus this function) function forbids using same ID for both RLE units
  After reset both IDs are 0. So make sure to first initialise RLE 1 with an ID != 0
   

  Parameters:
  Unit           - SpriteEngine Macro instance number
  RleNum         - RLE unit number (0 or 1)  
  RleConf        - Pointer to RLE configuration data, see <r_spea_RleConfig_t> 
 
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_SetRle(uint32_t                Unit, 
                             uint8_t                 RleNum, 
                             r_spea_RleConfig_t     *RleConf);

/*******************************************************************************
  Function: R_SPEA_UnitEnable

  Enable or disable a given RLE or sprite unit 

  Parameters:
  Unit          - Macro instance number
  SpUnit        - Unit identifier, see <r_spea_Unit_t>
  Enable        - 0: disable/pass throug mode, 1: enable 
 
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_UnitEnable(uint32_t Unit, r_spea_Unit_t SpUnit, uint8_t Enable);


/*******************************************************************************
  Function: R_SPEA_SpriteEnable

  Enable or disable a given sprite within one unit 

  Parameters:
  Unit          - Macro instance number
  SpEng         - Sprite unit number (0 - 2)
  SpNum         - Sprite number (0 - 16)  
  Enable        - 0: disable/pass throug mode, 1: enable 
 
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_SpriteEnable(uint32_t      Unit, 
                                   uint8_t       SpEng, 
                                   uint8_t       SpNum, 
                                   uint8_t       Enable);


/*******************************************************************************
  Function: R_SPEA_SetSpritePos

  Set or change just the position (on the virtual layer) of a given sprite  

  Parameters:
  Unit          - Macro instance number
  SpEng         - Sprite unit number (0 - 2)
  SpNum         - Sprite number (0 - 16)  
  PosX          - Horizontal position in pixel 
  PosY          - Vertical position in pixel 
  DispBpp       - Byte per pixel display (not sprite specific) setting, 
                  required for pixel to byte position calculation
  
  Returns:
  see: <r_spea_Error_t>
*/

r_spea_Error_t R_SPEA_SetSpritePos(uint32_t      Unit, 
                                   uint8_t       SpEng, 
                                   uint8_t       SpNum, 
                                   uint16_t      PosX,
                                   uint16_t      PosY,
                                   uint8_t       DispBpp);



/*******************************************************************************
  Function: R_SPEA_UnitUpdateActive

  Get the information if a register update is onging or finished for a given unit    

  Parameters:
  Unit          - Macro instance number
  SpUnit        - Unit identifier, see <r_spea_Unit_t>
  
  Returns:
  == 0 - updeate finished 
  != 0 - register update in progress  
*/

uint32_t R_SPEA_UnitUpdateActive(uint32_t Unit,  r_spea_Unit_t SpUnit);  

/*******************************************************************************
  Function: R_SPEA_UpdateUnit

  Set the register update request for the given unit     

  Parameters:
  Unit          - Macro instance number
  SpUnit        - Unit identifier, see <r_spea_Unit_t>
  VSync         - Index of the Video Out's vsync to be used for update 
                  VOut0: 0; VOut1: 1
  Returns:
  see: <r_spea_Error_t>*/

r_spea_Error_t R_SPEA_UpdateUnit(uint32_t Unit,  r_spea_Unit_t SpUnit, uint8_t VSync);  


/*******************************************************************************
  Function: R_SPEA_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_SPEA_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));
                                 

#ifdef __cplusplus
}
#endif

#endif /* R_SPEA_API_H_  */
