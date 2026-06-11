/*
****************************************************************************
PROJECT : SFMA driver
FILE    : $Id: r_sfma_api.h 7491 2016-02-02 16:51:40Z florian.zimmermann $
============================================================================
DESCRIPTION
Driver for the SFMA(Serial Flash Memory Interface A) macro
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

/***************************************************************************
  Title: SFMA Driver API

  SFMA (Serial Flash Memory Interface A) driver API

  An application using SFMA driver should include this header file.

  Note:
  If an OS is used the SFMA driver internally uses semaphores. 

*/

#ifndef R_SFMA_API_H
#define R_SFMA_API_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
  Section: Global defines
*/
/***************************************************************************
  Group: Constants
*/

/***************************************************************************
  Constants: API Version

  This constant is the value which shows the version information of the SFMA driver.
  It refers at the <R_SFMA_VersionStringGet> function.

  R_SFMA_VERSION_HI - This constant is the high byte of the version information.
                      It is major version information.
  R_SFMA_VERSION_LO - This constant is the low byte of the version information.
                      It is minor version information.
*/
#define R_SFMA_VERSION_HI  2
#define R_SFMA_VERSION_LO  0


/***************************************************************************
  Section: Global Types
*/
/***************************************************************************
  Group: Enumerations and Types
*/
/***************************************************************************
  Define: 

  Description:
  SFMA option for performance enhance mode

  Values:
  R_SFMA_PER_EN_MODE_DISABLE - option is disabled
  R_SFMA_PER_EN_MODE_ENABLE  - option is enabled

*/
          
#define R_SFMA_PER_EN_MODE_DISABLE  (0uL)
#define R_SFMA_PER_EN_MODE_ENABLE   (1uL)

/***************************************************************************
  Enum: r_sfma_Error_t

  Description:
  SFMA driver error code.

  If an error occurs these enumerations give information about the
  reason.

  This list can be extended, it is an example only. 
  The error code is used internally by the driver when 
  calling the central error handler.

  The central error handler will pass the error code to the 
  application call-back function, if it has been set. 

  An API function may also return an error code.

  Values:
  R_SFMA_ERR_OK                   - No error has occurred.
  R_SFMA_ERR_NG                   - An error has occurred, but no specific error code
                                    is defined for it.
  R_SFMA_ERR_PARAM_INCORRECT      - A parameter provided to a function is incorrect.
                                    (It excludes the outside of the range)
  R_SFMA_ERR_RANGE_UNIT           - The unit-number is the outside of the range.
  R_SFMA_ERR_RANGE_PARAM          - A parameter is the outside of the range.
  R_SFMA_ERR_NOT_ACCEPTABLE       - A function was called in an incorrect state.
  R_SFMA_ERR_FATAL_OS             - Fatal error has occurred at OS interface.
  R_SFMA_ERR_FATAL_HW             - Fatal error has occurred at H/W.
  R_SFMA_ERR_NOT_SUPPORT_CLOCK    - The set value of the serial clock isn't supported.
  R_SFMA_ERR_NOT_SUPPORT_TRANSFER - The set transfer mode isn't supported in the 
                                    connected serial flash.
  R_SFMA_ERR_SFLASH_PROTECTED     - The serial flash memory is protected.
*/
typedef enum
{
    R_SFMA_ERR_OK = 0,
    R_SFMA_ERR_NG,
    R_SFMA_ERR_PARAM_INCORRECT,
    R_SFMA_ERR_RANGE_UNIT,
    R_SFMA_ERR_RANGE_PARAM,
    R_SFMA_ERR_NOT_ACCEPTABLE,
    R_SFMA_ERR_FATAL_OS,
    R_SFMA_ERR_FATAL_HW,
    R_SFMA_ERR_NOT_SUPPORT_CLOCK,
    R_SFMA_ERR_NOT_SUPPORT_TRANSFER,
    R_SFMA_ERR_SFLASH_PROTECTED
} r_sfma_Error_t;

/***************************************************************************
  Enum: r_sfma_MemoryNum_t

  Description:
  This type describes the number of serial flash memory to connect.

  Values:
  R_SFMA_MEMORY_SINGLE - One serial flash memory is connected.
  R_SFMA_MEMORY_DUAL   - Two serial flash memoryies are connected.
*/
typedef enum
{
    R_SFMA_MEMORY_SINGLE = 0,
    R_SFMA_MEMORY_DUAL
} r_sfma_MemoryNum_t;


/***************************************************************************
  Enum: r_sfma_Mode_t

  Description:
  This type describes the operating mode.

  Values:
  R_SFMA_MODE_EX_SPACE      - External address space read mode.
  R_SFMA_MODE_SPI_OPERATING - SPI operating mode.
*/
typedef enum
{
    R_SFMA_MODE_EX_SPACE = 0,
    R_SFMA_MODE_SPI_OPERATING
} r_sfma_Mode_t;


/***************************************************************************
  Enum: r_sfma_DataTransferMode_t

  Description:
  This type describes the transfer mode.

  Values:
  R_SFMA_SDR_SINGLE_IO - SDR with Single I/O.
  R_SFMA_SDR_DUAL_IO   - SDR with Dual I/O.
  R_SFMA_SDR_QUAD_IO   - SDR with Quad I/O.
  R_SFMA_DDR_SINGLE_IO - DDR with Single I/O.
  R_SFMA_DDR_DUAL_IO   - DDR with Dual I/O.
  R_SFMA_DDR_QUAD_IO   - DDR with Quad I/O.
*/
typedef enum
{
    R_SFMA_SDR_SINGLE_IO = 0,
    R_SFMA_SDR_DUAL_IO,
    R_SFMA_SDR_QUAD_IO,
    R_SFMA_DDR_SINGLE_IO,
    R_SFMA_DDR_DUAL_IO,
    R_SFMA_DDR_QUAD_IO
} r_sfma_DataTransferMode_t;


/***************************************************************************
  Enum: r_sfma_ProtectionMode_t

  Description:
  This type describes the protection mode of the serial flash memory.

  Values:
  R_SFMA_MODE_PROTECT   - Set to the protection mode.
  R_SFMA_MODE_UNPROTECT - Set to the un-protection mode.
*/
typedef enum
{
    R_SFMA_MODE_PROTECT = 0,
    R_SFMA_MODE_UNPROTECT
} r_sfma_ProtectionMode_t;


/***************************************************************************
  Enum: r_sfma_AddressMode_t

  Description:
  This type describes the format of address output to the serial flash memory.

  Values:
  R_SFMA_ADDRESS_24BIT - 24 bit address output.
  R_SFMA_ADDRESS_32BIT - 32 bit address output.
*/
typedef enum
{
    R_SFMA_ADDRESS_24BIT = 0,
    R_SFMA_ADDRESS_32BIT
} r_sfma_AddressMode_t;

/***************************************************************************
  Enum: r_sfma_AccessRange_t

  Description:
  This type describes the access range of the serial flash memory.

  Values:
  R_SFMA_ACCESS_RANGE_32MB  - Access range is 32 Mbytes.
  R_SFMA_ACCESS_RANGE_64MB  - Access range is 64 Mbytes.
  R_SFMA_ACCESS_RANGE_128MB - Access range is 128 Mbytes.
  R_SFMA_ACCESS_RANGE_256MB - Access range is 256 Mbytes.
  R_SFMA_ACCESS_RANGE_512MB - Access range is 512 Mbytes.
*/
typedef enum
{
    R_SFMA_ACCESS_RANGE_32MB = 0,
    R_SFMA_ACCESS_RANGE_64MB,
    R_SFMA_ACCESS_RANGE_128MB,
    R_SFMA_ACCESS_RANGE_256MB,
    R_SFMA_ACCESS_RANGE_512MB
} r_sfma_AccessRange_t;

/***************************************************************************
  Enum: r_sfma_FlashRegister_t

  Description:
  This type describes the register of the serial flash memory.

  Values:
  R_SFMA_STATUS_REG - Status Register.
  R_SFMA_CONFIG_REG - Configuration Register.
  R_SFMA_NONE_REG   - Un used.
*/
typedef enum
{
    R_SFMA_STATUS_REG = 0,
    R_SFMA_CONFIG_REG,
    R_SFMA_NONE_REG
} r_sfma_FlashRegister_t;

/***************************************************************************
  Enum: r_sfma_DummyCycle_t

  Description:
  This type describes the data read dummy cycles.

  Values:
  R_SFMA_DUMMY_1CYC - 1cycle.
  R_SFMA_DUMMY_2CYC - 2cycles.
  R_SFMA_DUMMY_3CYC - 3cycles.
  R_SFMA_DUMMY_4CYC - 4cycles.
  R_SFMA_DUMMY_5CYC - 5cycles.
  R_SFMA_DUMMY_6CYC - 6cycles.
  R_SFMA_DUMMY_7CYC - 7cycles.
  R_SFMA_DUMMY_8CYC - 8cycles.
  R_SFMA_DUMMY_0CYC - Dummy cycle insertion disabled.
*/
typedef enum
{
    R_SFMA_DUMMY_1CYC = 0,
    R_SFMA_DUMMY_2CYC,
    R_SFMA_DUMMY_3CYC,
    R_SFMA_DUMMY_4CYC,
    R_SFMA_DUMMY_5CYC,
    R_SFMA_DUMMY_6CYC,
    R_SFMA_DUMMY_7CYC,
    R_SFMA_DUMMY_8CYC,
    R_SFMA_DUMMY_0CYC
} r_sfma_DummyCycle_t;

/***************************************************************************
  Enum: r_sfma_AddressBitSize_t

  Description:
  This type describes the address bit size.

  Values:
  R_SFMA_ADDRESS_SIZE_1BIT - 1bit.
  R_SFMA_ADDRESS_SIZE_2BIT - 2bits.
  R_SFMA_ADDRESS_SIZE_4BIT - 4bits.
*/
typedef enum
{
    R_SFMA_ADDRESS_SIZE_1BIT = 0,
    R_SFMA_ADDRESS_SIZE_2BIT,
    R_SFMA_ADDRESS_SIZE_4BIT
} r_sfma_AddressBitSize_t;


/***************************************************************************
  Enum: r_sfma_CacheMode_t

  Description:
  This type describes the cache settings for the SFMA interface.

  Values:
  R_SFMA_CACHE_OFF - SFMA cache off.
  R_SFMA_CACHE_BL1 .. BL16 - SFMA cache on, with set burst length
                             (BL1 = 1 data unit of 64bit, BL16 = 16 data units of 64bit)
*/
typedef enum
{
    R_SFMA_CACHE_OFF  = 0,
    R_SFMA_CACHE_BL1  = 1,
    R_SFMA_CACHE_BL2  = 2,
    R_SFMA_CACHE_BL3  = 3,
    R_SFMA_CACHE_BL4  = 4,
    R_SFMA_CACHE_BL5  = 5,
    R_SFMA_CACHE_BL6  = 6,
    R_SFMA_CACHE_BL7  = 7,
    R_SFMA_CACHE_BL8  = 8,
    R_SFMA_CACHE_BL9  = 9,
    R_SFMA_CACHE_BL10 = 10,
    R_SFMA_CACHE_BL11 = 11,
    R_SFMA_CACHE_BL12 = 12,
    R_SFMA_CACHE_BL13 = 13,
    R_SFMA_CACHE_BL14 = 14,
    R_SFMA_CACHE_BL15 = 15,
    R_SFMA_CACHE_BL16 = 16
} r_sfma_CacheMode_t;

/***************************************************************************
  typedef: r_sfma_FlashRegInfo_t

  Description:
  This type describes register infomation of the serial flash memory.

  Struct members:
  RegAssignment   - See <r_sfma_FlashRegister_t>
  BitPosition     - Bit position of status register or configuration register.
*/
typedef struct
{
    r_sfma_FlashRegister_t Reg;
    uint8_t                BitPosition;
} r_sfma_FlashRegInfo_t;

/***************************************************************************
  typedef: r_sfma_FlashRegSetParam_t

  Description:
  This type describes register infomation and set parameter of the serial flash memory.

  Struct members:
  RegAssignment   - See <r_sfma_FlashRegister_t>
  BitMask         - Bit mask of status register or configuration register.
  BitSet          - Set point to a register.
*/
typedef struct
{
    r_sfma_FlashRegister_t Reg;
    uint8_t                BitMask;
    uint8_t                BitSet;
} r_sfma_FlashRegSetParam_t;

/***************************************************************************
  typedef: r_sfma_FlashCommand_t

  Description:
  This type describes the serial flash commands.

  Struct members:
  QuadIoMode      - Quad I/O operation.
  BlockProtect    - Block Protection.
  WriteInProgress - Write in progress.
  DummyCycle      - Dummy cycle.
  ReadSdrDummyCycle       - Read SDR Dummy cycles.
  ReadSdrIoDualDummyCycle - Read SDR Dual Dummy cycles.
  ReadSdrIoQuadDummyCycle - Read SDR Quad Dummy cycles.
  ReadDdrDummyCycle       - Read DDR Dummy cycles.
  ReadDdrIoDualDummyCycle - Read DDR Dual Dummy cycles.
  ReadDdrIoQuadDummyCycle - Read DDR Quad Dummy cycles.
  WriteAddressBit         - Write address bit width.
  ReadSdr         - SDR read.
  ReadSdrDual     - SDR dual read.
  ReadSdrQuad     - SDR quad read.
  ReadSdrIoDual   - SDR dual I/O read.
  ReadSdrIoQuad   - SDR quad I/O read.
  ReadDdr         - DDR read.
  ReadDdrDual     - DDR dual read.
  ReadDdrQuad     - DDR quad read.
  ReadDdrIoDual   - DDR dual I/O read.
  ReadDdrioQuad   - DDR quad I/O read.
  WriteSdr        - SDR program page.
  WriteSdrDual    - SDR program page dual.
  WriteSdrQuad    - SDR program page quad.
  WriteDdr        - DDR program page.
  WriteDdrDual    - DDR program page dual.
  WriteDdrQuad    - DDR program page quad.
  WriteEnable     - Write enable.
  Erase           - Erase sector.
  ReadStatus_1    - Read status register.
  ReadStatus_2    - Read config register.
  WriteStatus     - Write status register.
*/
typedef struct
{
    r_sfma_FlashRegInfo_t      QuadIoMode;
    r_sfma_FlashRegInfo_t      BlockProtect;
    r_sfma_FlashRegInfo_t      WriteInProgress;
    r_sfma_FlashRegSetParam_t  DummyCycle;
    r_sfma_DummyCycle_t        ReadSdrDummyCycle;
    r_sfma_DummyCycle_t        ReadSdrIoDualDummyCycle;
    r_sfma_DummyCycle_t        ReadSdrIoQuadDummyCycle;
    r_sfma_DummyCycle_t        ReadDdrDummyCycle;
    r_sfma_DummyCycle_t        ReadDdrIoDualDummyCycle;
    r_sfma_DummyCycle_t        ReadDdrIoQuadDummyCycle;
    r_sfma_AddressBitSize_t    WriteAddressBit;
    uint8_t                    ReadSdr;
    uint8_t                    ReadSdrDual;
    uint8_t                    ReadSdrQuad;
    uint8_t                    ReadSdrIoDual;
    uint8_t                    ReadSdrIoQuad;
    uint8_t                    ReadDdr;
    uint8_t                    ReadDdrDual;
    uint8_t                    ReadDdrQuad;
    uint8_t                    ReadDdrIoDual;
    uint8_t                    ReadDdrIoQuad;
    uint8_t                    WriteSdr;
    uint8_t                    WriteSdrDual;
    uint8_t                    WriteSdrQuad;
    uint8_t                    WriteDdr;
    uint8_t                    WriteDdrDual;
    uint8_t                    WriteDdrQuad;
    uint8_t                    WriteEnable;
    uint8_t                    Erase;
    uint8_t                    ReadStatus1;
    uint8_t                    ReadStatus2;
    uint8_t                    WriteStatus;
    uint8_t                    ExitExtAddrSpc;
    uint8_t                    ResetEnable;
    uint8_t                    Reset;
} r_sfma_FlashCommand_t;


/***************************************************************************
  typedef: r_sfma_Config_t

  Description:
  This type describes the configuration of the unit.

  Struct members:
  Mode                        - See <r_sfma_Mode_t>.
  MemoryNum                   - See <r_sfma_MemoryNum_t>.
  DataTransferMode            - See <r_sfma_DataTransferMode_t>.
  AddressMode                 - See <r_sfma_AddressMode_t>.
  SerialFlashMemoryPageSize   - Page size of connected serial flash memory (Byte).
  SerialFlashMemoryMaxClock   - Specify maximum clock speed of connected serial flash memory (Hz).
                                Bit rate of SFMA is decided by SerialFlashMemoryMaxClock and
                                "B_phi clock" which is input to SFMA H/W macro. and setting of
                                "B_Phi clock" is outside RGL. Refer to chapter 17.4.3.1 of H/W user's
                                manual.
                                For example, in case "B_phi clock" is set to 160 MHz, if
                                SerialFlashMemoryMaxClock is set to 80 MHz, "B_Phi clock" is divided
                                by 2 and bit rate of SFMA is set to 80 MHz. But if SerialFlashMemoryClock
                                is set to 60 MHz, "B_Phi clock" is divided by 4 (because division ratio
                                can be set only to even number) then bit rate of SFMA is set to 40 MHz.
  SerialFlashMemorySectorSize - Erase Sector Size of connected serial flash memory (Byte).
                                If the MemoryNum of the <r_sfma_Config_t> structure is
                                <R_SFMA_MEMORY_DUAL>, this size must set double size of
                                sector size of serial flash device. 
                                (e.g. This size is 8 Kbytes when connecting two MX25L51245G.)
  SerialFlashMemorySize       - Size of connected serial flash memory (Byte).
                                This size is total size of connected serial flash device.
                                (e.g. This size is 128 MBytes(128*1024*1024), 
                                when connecting two MX25L51245G)
  Command                     - See <r_sfma_FlashCommand_t>.
  CacheMode                   - See <r_sfma_CacheMode_t>.
  Calibration                 - Calibration setting for the device, clock and phase
  PerformanceEnMode           - option to enable the performance enhance mode  
*/
typedef struct
{
    r_sfma_Mode_t             Mode;
    r_sfma_MemoryNum_t        MemoryNum;
    r_sfma_DataTransferMode_t DataTransferMode;
    r_sfma_AddressMode_t      AddressMode;
    uint32_t                  SerialFlashMemoryPageSize;
    uint32_t                  SerialFlashMemoryMaxClock;
    uint32_t                  SerialFlashMemorySectorSize;
    uint64_t                  SerialFlashMemorySize;
    r_sfma_FlashCommand_t     *Command;
    r_sfma_CacheMode_t        CacheMode;
    uint32_t                  Calibration;
    uint32_t                  PerformanceEnMode;
} r_sfma_Config_t;


/***************************************************************************
  Section: Global API functions
*/

/***************************************************************************
  Function: R_SFMA_Init

  Description:
  This function initializes the SFMA driver.

  If the function successfully execute, the return code will be <R_SFMA_ERR_OK>
  and the state will be in the Initialize state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Init(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_DeInit

  Description:
  This function de-initializes the SFMA driver.

  If the function successfully execute, the return code will be <R_SFMA_ERR_OK>
  and the state will be in the UnInitialize state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_DeInit(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_Open

  Description:
  This function opens the SFMA driver at an operating mode, number of memories
  that is specified.

  If the function successfully execute, the return code will be <R_SFMA_ERR_OK>
  and the state will be in the Executing state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.
  Config  - This is a pointer to the <r_sfma_Config_t> structure to the
            configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Open(const uint32_t                Unit,
                           const r_sfma_Config_t * const Config);


/***************************************************************************
  Function: R_SFMA_Close

  Description:
  This function closes the SFMA driver.

  If the function successfully execute, the return code will be <R_SFMA_ERR_OK>
  and the state will be in the Initialize state.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_Close(const uint32_t Unit);


/***************************************************************************
  Function: R_SFMA_AccessAddressSet

  Description:
  This function sets the address of the serial flash memory.

  In the external address space mode it is possible to access MAX 8 Gbytes space of
  serial flash memory. But accessible space which is mapped to CPU address space is
  MAX 512 Mbytes. This function sets the access address and the access range.

  Note: When the <R_SFMA_Open> function is executed, the access address and
        the access range is automatically set to 0x00000000 and 32 Mbytes.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.
  Addr    - The parameter specifies the access address of the serial flash memory.
            This parameter aligns in the access range of "Range" of the argument.
            (e.g. if "Addr" is "0x2000000" and "Range" is 64MB, the set a will be "0x0".)
  Range   - The parameter specifies the access range of the of the serial flash memory.
            See <r_sfma_AccessRange_t>.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_AccessAddressSet(const uint32_t             Unit,
                                       const uint64_t             Addr,
                                       const r_sfma_AccessRange_t Range);


/***************************************************************************
  Function: R_SFMA_AccessAddressGet

  Description:
  This function gets the address of the serial flash memory.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.
  Addr    - This is a pointer to the access address of the serial flash memory.
  Range   - This is a pointer to the access range of the serial flash memory.
            See <r_sfma_AccessRange_t>.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_AccessAddressGet(const uint32_t                     Unit,
                                             uint64_t             * const Addr,
                                             r_sfma_AccessRange_t * const Range);


/***************************************************************************
  Function: R_SFMA_ProtectionModeSet

  Description:
  This function sets protection mode of the serial flash memory.

  The write and erase access to serial flash memory is prohibited if protection is set.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.
  Mode    - The parameter specifies the protection mode of the serial flash memory.
            See <r_sfma_ProtectionMode_t>.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_ProtectionModeSet(const uint32_t                Unit,
                                        const r_sfma_ProtectionMode_t Mode);


/***************************************************************************
  Function: R_SFMA_DataErase

  Description:
  This function erases the data of designated address in the serial flash memory.

  Note: Erasing of data is performed on the sector unit. Therefore this function
        erases data of the sector including the size from the address.

  Note: Erase sector size become double size when MemoryNum of the <r_sfma_Config_t> structure
        is <R_SFMA_MEMORY_DUAL>.

  Note: This function has the possibility that the processing takes time.
        Therefore, <R_SFMA_Sys_Relax> is sometimes executed.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.
  Addr    - The parameter specifies the erase address of the serial flash memory.
  Size    - The parameter specifies the data size (in bytes) to erase.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_DataErase(const uint32_t Unit, const uint64_t Addr, const int32_t Size);


/***************************************************************************
  Function: R_SFMA_DataWrite

  Description:
  This function writes data to the serial flash memory.

  In order to write data, the data of the sector must have been previously erased.

  Note: This function has the possibility that the processing takes time.
        Therefore, <R_SFMA_Sys_Relax> is sometimes executed.

  Note: This function always executes by the SDR transfer mode.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.
  Addr    - The parameter specifies the write address of the serial flash memory.
            This parameter aligns in the page size of the serial flash memory.
  Buf     - This is a pointer to the address of buffer.
  Size    - The parameter specifies the data size (in bytes) to write.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_DataWrite(const uint32_t Unit,
                                const uint64_t Addr,
                                const uint8_t  *Buf,
                                const int32_t  Size);


/***************************************************************************
  Function: R_SFMA_DataRead

  Description:
  This function reads data from the serial flash memory.

  Note: This function has the possibility that the processing takes time.
        Therefore, <R_SFMA_Sys_Relax> is sometimes executed.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit    - The parameter specifies the instance number.
  Addr    - The parameter specifies the read address of the serial flash memory.
  Buf     - This is a pointer to the address of buffer.
  Size    - The parameter specifies the data size (in bytes) to read.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_DataRead(const uint32_t Unit,
                               const uint64_t Addr,
                                     uint8_t  *Buf,
                               const int32_t  Size);


/***************************************************************************
  Function: R_SFMA_JEDECRead

  Description:
  This function reads basic JEDEC data.

  Note that this function can't call from interrupt handler.

  Parameter:
  Unit           - The parameter specifies the instance number.
  ManufacturerID - This is a pointer to the manufacturer ID.
  MemoryType     - This is a pointer to the memory type.
  Capacity       - This is a pointer to the memory capacity.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_JEDECRead(const uint32_t        Unit,
                                      uint8_t * const ManufacturerID,
                                      uint8_t * const MemoryType,
                                      uint8_t * const Capacity);

/***************************************************************************
  Function: R_SFMA_VersionStringGet

  Description:
  This function returns the version string of this SFMA driver.

  Parameter:
  None

  Return value:
  Pointer of string.
*/
const uint8_t* R_SFMA_VersionStringGet(void);

/***************************************************************************
  Function: R_SFMA_MacroVersionGet

  Description:
  This function returns the major and minor version of the H/W macro.

  Parameter:
  Major - This is a pointer to the major version.
  Minor - This is a pointer to the minor version.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_MacroVersionGet(uint32_t * const Major,
                                      uint32_t * const Minor);

/***************************************************************************
  Function: R_SFMA_GetCal

  Description:
  This function retrieves the calibration value.

  Parameter:
  Unit           - The parameter specifies the instance number.
  Cal            - This is a pointer to store the calibration value.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_GetCal(const uint32_t   Unit,
                                   uint32_t * Cal);


/***************************************************************************
  Function: R_SFMA_GetStatus

  Description:
  This function retrieves the status for the device in SPI operating mode.

  Parameter:
  Unit           - The parameter specifies the instance number.
  Status1        - This is a pointer to store the Status1 value.
  Status2        - This is a pointer to store the Status2 value.


  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_GetStatus(const uint32_t Unit, 
                                      uint8_t * const Status1,
                                      uint8_t * const Status2);

/***************************************************************************
  Function: R_SFMA_GetConfig

  Description:
  This function retrieves the config for the device in SPI operating mode.

  Parameter:
  Unit           - The parameter specifies the instance number.
  Config1        - This is a pointer to store the Config1 value.
  Config2        - This is a pointer to store the Config2 value.


  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_GetConfig(const uint32_t Unit, 
                                      uint8_t * const Config1,
                                      uint8_t * const Config2);


/***************************************************************************
  Function: R_SFMA_WriteStatusConfig

  Description:
  This function writes the Status and config for the device in SPI operating mode.

  Parameter:
  Unit           - The parameter specifies the instance number.
  Status         - This is the Status value to write.
  Config         - This is the Config value to write.


  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_WriteStatusConfig(const uint32_t Unit, 
                                              uint8_t Status,
                                              uint8_t Config);


/***************************************************************************
  Function: R_SFMA_WriteReset

  Description:
  This function writes a software reset for the device in SPI operating mode.

  Parameter:
  Unit           - The parameter specifies the instance number.


  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_WriteReset(const uint32_t Unit);


#ifdef __cplusplus
}
#endif

#endif /* R_SFMA_API_H */

