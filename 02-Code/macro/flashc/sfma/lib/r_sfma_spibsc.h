/*
****************************************************************************
PROJECT : SFMA driver
FILE    : $Id: r_sfma_spibsc.h 7594 2016-02-09 14:58:03Z john.vincent $
============================================================================
DESCRIPTION
Driver for the SFMA(Serial Flash Memory Interface) macro
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
  Title: SFMA Spibsc Driver

  SFMA Main Module Internal Interface

  Only the driver modules shall include this header.
*/

#ifndef R_SFMA_SPIBSC_H
#define R_SFMA_SPIBSC_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
  Section: Global defines
*/
/***********************************************************
  Constants: R_SFMA_SPIBSC

  This constant is the setting value of SPIBSC.

  R_SFMA_SPIBSC_CMNCR_BSZ_SINGLE  - The setting value of the number of the flashes
                                    to use(1memory).
  R_SFMA_SPIBSC_CMNCR_BSZ_DUAL    - The setting value of the number of the flashes
                                    to use(2memories).
  R_SFMA_SPIBSC_1BIT              - The setting value of bit size is 1bit.
  R_SFMA_SPIBSC_2BIT              - The setting value of bit size is 2bits.
  R_SFMA_SPIBSC_4BIT              - The setingt value of bit size is 4bits.
  R_SFMA_SPIBSC_OUTPUT_DISABLE    - The setting value of disable.
  R_SFMA_SPIBSC_OUTPUT_ENABLE     - The setting value of enable.
  R_SFMA_SPIBSC_OUTPUT_ADDR_24    - The setting value of address enable is 24bits.
  R_SFMA_SPIBSC_OUTPUT_ADDR_32    - The setting value of address enable is 32bits.
  R_SFMA_SPIBSC_OUTPUT_OPD_3      - The setting value of option data enable.
  R_SFMA_SPIBSC_OUTPUT_OPD_32     - The setting value of option data enable.
  R_SFMA_SPIBSC_OUTPUT_SPID_8     - The setting value of transfer data enable is 8bits.
  R_SFMA_SPIBSC_OUTPUT_SPID_16    - The setting value of transfer data enable is 16bits.
  R_SFMA_SPIBSC_OUTPUT_SPID_32    - The setting value of transfer data enable is 32bits.
  R_SFMA_SPIBSC_SPISSL_NEGATE     - The setting value of SPBSSL signal is negated.
  R_SFMA_SPIBSC_SPISSL_KEEP       - The setting value of SPBSSL signal is maintained.
  R_SFMA_SPIBSC_SPIDATA_DISABLE   - The setting value of SPI data transfer disable.
  R_SFMA_SPIBSC_SPIDATA_ENABLE    - The setting value of SPI data transfer enable.
  R_SFMA_SPIBSC_DUMMY_CYC_DISABLE - The setting value of dummy cycle disable.
  R_SFMA_SPIBSC_DUMMY_CYC_ENABLE  - The setting value of dummy cycle enable.
  R_SFMA_SPIBSC_SDR_TRANS         - The setting value of SDR transfer enable.
  R_SFMA_SPIBSC_DDR_TRANS         - The setting value of DDR transfer enable.
  R_SFMA_PERFOR_EN_MODE_DISABLE   - The setting value of performance enhance mode to be disabled.
  R_SFMA_PERFOR_EN_MODE_ENABLE    - The setting value of performance enhance mode to be enabled.
*/

#define R_SFMA_SPIBSC_CMNCR_BSZ_SINGLE     (0uL)
#define R_SFMA_SPIBSC_CMNCR_BSZ_DUAL       (1uL)
#define R_SFMA_SPIBSC_1BIT                 (0uL)
#define R_SFMA_SPIBSC_2BIT                 (1uL)
#define R_SFMA_SPIBSC_4BIT                 (2uL)
#define R_SFMA_SPIBSC_OUTPUT_DISABLE       (0uL)
#define R_SFMA_SPIBSC_OUTPUT_ENABLE        (1uL)
#define R_SFMA_SPIBSC_OUTPUT_ADDR_24       (0x07uL)
#define R_SFMA_SPIBSC_OUTPUT_ADDR_32       (0x0fuL)
#define R_SFMA_SPIBSC_OUTPUT_OPD_3         (0x08uL)
#define R_SFMA_SPIBSC_OUTPUT_OPD_32        (0x0cuL)
#define R_SFMA_SPIBSC_OUTPUT_SPID_8        (0x08uL)
#define R_SFMA_SPIBSC_OUTPUT_SPID_16       (0x0cuL)
#define R_SFMA_SPIBSC_OUTPUT_SPID_32       (0x0fuL)
#define R_SFMA_SPIBSC_SPISSL_NEGATE        (0uL)
#define R_SFMA_SPIBSC_SPISSL_KEEP          (1uL)
#define R_SFMA_SPIBSC_SPIDATA_DISABLE      (0uL)
#define R_SFMA_SPIBSC_SPIDATA_ENABLE       (1uL)
#define R_SFMA_SPIBSC_DUMMY_CYC_DISABLE    (0uL)
#define R_SFMA_SPIBSC_DUMMY_CYC_ENABLE     (1uL)
#define R_SFMA_SPIBSC_SDR_TRANS            (0uL)
#define R_SFMA_SPIBSC_DDR_TRANS            (1uL)
#define R_SFMA_SPIBSC_PER_EN_MODE_DISABLE  (0uL)
#define R_SFMA_SPIBSC_PER_EN_MODE_ENABLE   (1uL)
#define R_SFMA_SPIBSC_MODE_EXTERNAL        (0uL)
#define R_SFMA_SPIBSC_MODE_SPI             (1uL)


/***********************************************************
  Constants: R_SFMA_SHIFT

  A constant which is used to shift.

  R_SFMA_SHIFT_BYTE  - Data shift byte.
  R_SFMA_SHIFT_2BYTE - Data shift 2bytes.
  R_SFMA_SHIFT_3BYTE - Data shift 3bytes.

*/
#define R_SFMA_SHIFT_BYTE                  (8uL)
#define R_SFMA_SHIFT_2BYTE                 (16uL)
#define R_SFMA_SHIFT_3BYTE                 (24uL)

/***********************************************************
  Constants: R_SFMA_NOT_SUPPORT_COMMAND

  This constant is command not to support.

*/
#define R_SFMA_NOT_SUPPORT_COMMAND         (0xFF)


/***************************************************************************
  Section: Global Types
*/
/***********************************************************
  Enum: r_sfma_OptionData_t

  Description:
  This type describes the number of option data.

  Values:
  R_SFMA_OPTION_DATA_0   - It means option data 0.
  R_SFMA_OPTION_DATA_1   - It means option data 1.
  R_SFMA_OPTION_DATA_2   - It means option data 2.
  R_SFMA_OPTION_DATA_3   - It means option data 3.
  R_SFMA_OPTION_DATA_NUM - The number of the option data.
*/
typedef enum
{
    R_SFMA_OPTION_DATA_0 = 0,
    R_SFMA_OPTION_DATA_1,
    R_SFMA_OPTION_DATA_2,
    R_SFMA_OPTION_DATA_3,
    R_SFMA_OPTION_DATA_NUM
} r_sfma_OptionData_t;

/***********************************************************
  Enum: r_sfma_SmwdrData_t

  Description:
  This type describes the number of the data write register.

  Values:
  R_SFMA_SMWDR_DATA_0   - It means the write data register 0.
  R_SFMA_SMWDR_DATA_1   - It means the write data register 1.
  R_SFMA_SMWDR_DATA_NUM - The number of the write data register.
*/
typedef enum
{
    R_SFMA_SMWDR_DATA_0 = 0,
    R_SFMA_SMWDR_DATA_1,
    R_SFMA_SMWDR_DATA_NUM
} r_sfma_SmwdrData_t;

/***********************************************************
  Enum: r_sfma_SmrdrData_t

  Description:
  This type describes the number of the data read  register.

  Values:
  R_SFMA_SMWDR_DATA_0   - It means the SPI mode read data register 0.
  R_SFMA_SMWDR_DATA_1   - It means the SPI mode read data register 1.
  R_SFMA_SMWDR_DATA_NUM - The number of the SPI mode read data register.
*/
typedef enum
{
    R_SFMA_SMRDR_DATA_0 = 0,
    R_SFMA_SMRDR_DATA_1,
    R_SFMA_SMRDR_DATA_NUM
} r_sfma_SmrdrData_t;


/***********************************************************
  Enum: r_sfma_Req_t

  Description:
  This type describes the request to the serial flash memory.

  Values:
  R_SFMA_REQ_PROTECT    - The parameter specifies the request of protect.
  R_SFMA_REQ_UNPROTECT  - The parameter specifies the request of unprotect.
  R_SFMA_REQ_SERIALMODE - The parameter specifies the request of Serial or Dual mode.
  R_SFMA_REQ_QUADMODE   - The parameter specifies the request of Quad mode.
*/
typedef enum
{
    R_SFMA_REQ_PROTECT = 0,    /* request protect          */
    R_SFMA_REQ_UNPROTECT,      /* release protect          */
    R_SFMA_REQ_SERIALMODE,     /* request Serial/Dual mode */
    R_SFMA_REQ_QUADMODE        /* request Quad mode        */
} r_sfma_Req_t;

/***********************************************************
  typedef: r_sfma_SpibscSpimdReg_t

  Description:
  This type describes the setting of SPI mode register.

  Struct members:
  Cdb    - The parameter specifies the command bit width.
           (<R_SFMA_SPIBSC_1BIT>, <R_SFMA_SPIBSC_2BIT>, <R_SFMA_SPIBSC_4BIT>)
  Ocdb   - The parameter specifies the optional command bit width.
           (<R_SFMA_SPIBSC_1BIT>, <R_SFMA_SPIBSC_2BIT>, <R_SFMA_SPIBSC_4BIT>)
  Adb    - The parameter specifies the address bit width.
           (<R_SFMA_SPIBSC_1BIT>, <R_SFMA_SPIBSC_2BIT>, <R_SFMA_SPIBSC_4BIT>)
  Opdb   - The parameter specifies the option data bit width.
           (<R_SFMA_SPIBSC_1BIT>, <R_SFMA_SPIBSC_2BIT>, <R_SFMA_SPIBSC_4BIT>)
  Spidb  - The parameter specifies the transfer data bit width.
           (<R_SFMA_SPIBSC_1BIT>, <R_SFMA_SPIBSC_2BIT>, <R_SFMA_SPIBSC_4BIT>)
  Cde    - The parameter specifies the command enable.
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>, <R_SFMA_SPIBSC_OUTPUT_ENABLE>)
  Ocde   - The parameter specifies the optional command enable.
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>, <R_SFMA_SPIBSC_OUTPUT_ENABLE>)
  Ade    - The parameter specifies the address enable.
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>,
            <R_SFMA_SPIBSC_OUTPUT_ADDR_24>,
            <R_SFMA_SPIBSC_OUTPUT_ADDR_32>)
  Opde   - The parameter specifies the option data enable.
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>,
            <R_SFMA_SPIBSC_OUTPUT_OPD_3>,
            <R_SFMA_SPIBSC_OUTPUT_OPD_32>)
  Spide  - The parameter specifies the transfer data enable
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>,
            <R_SFMA_SPIBSC_OUTPUT_SPID_8>,
            <R_SFMA_SPIBSC_OUTPUT_SPID_16>,
            <R_SFMA_SPIBSC_OUTPUT_SPID_32>)
  Sslkp  - The parameter specifies the SPBSSL Signal Level.
           (<R_SFMA_SPIBSC_SPISSL_NEGATE>, <R_SFMA_SPIBSC_SPISSL_KEEP>)
  Spire  - The parameter specifies the data read enable.
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>, <R_SFMA_SPIBSC_OUTPUT_ENABLE>)
  Spiwe  - The parameter specifies the data write enable.
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>, <R_SFMA_SPIBSC_OUTPUT_ENABLE>)
  Dme    - The parameter specifies the dummy cycle enable.
           (<R_SFMA_SPIBSC_OUTPUT_DISABLE>, <R_SFMA_SPIBSC_OUTPUT_ENABLE>)
  Addre  - The parameter specifies the address DDR enable.
           (<R_SFMA_SPIBSC_SDR_TRANS>, <R_SFMA_SPIBSC_DDR_TRANS>)
  Opdre  - The parameter specifies the option data DDR enable.
           (<R_SFMA_SPIBSC_SDR_TRANS>, <R_SFMA_SPIBSC_DDR_TRANS>)
  Spidre - The parameter specifies the transfer data DDR enable.
           (<R_SFMA_SPIBSC_SDR_TRANS>, <R_SFMA_SPIBSC_DDR_TRANS>)
  Addr   - The parameter specifies the write address of the serial flash memory.
  Smrdr  - The parameter specifies the data read in SPI operating mode.
  Smwdr  - The parameter specifies the data to be written in SPI operating mode.
  Dmdb   - The parameter specifies the dummy cycle bit width.
           (<R_SFMA_SPIBSC_1BIT>, <R_SFMA_SPIBSC_2BIT>, <R_SFMA_SPIBSC_4BIT>)
  Dmcyc  - The parameter specifies the number of dummy cycles.
  Cmd    - See r_sfma_FlashCommand_t.
  Ocmd   - The parameter specifies the optional command.
  Opd    - The parameter specifies the option data in SPI operating mode.
*/
typedef struct
{
    uint32_t Cdb;
    uint32_t Ocdb;
    uint32_t Adb;
    uint32_t Opdb;
    uint32_t Spidb;
    uint32_t Cde;
    uint32_t Ocde;
    uint32_t Ade;
    uint32_t Opde;
    uint32_t Spide;
    uint32_t Sslkp;
    uint32_t Spire;
    uint32_t Spiwe;
    uint32_t Dme;     /* Dummy cycle enable at the time of a SPI mode */
    uint32_t Addre;   /* Address DDR enable                           */
    uint32_t Opdre;   /* Option data DDRenable                        */
    uint32_t Spidre;  /* Transmission data DDR enable                 */
    uint32_t Addr;
    uint32_t Smrdr[R_SFMA_SMRDR_DATA_NUM];
    uint32_t Smwdr[R_SFMA_SMWDR_DATA_NUM];
    uint8_t  Dmdb;    /* The dummy cycle bit width of the time of a SPI mode  */
    uint8_t  Dmcyc;   /* The number of dummy cycles of the time of a SPI mode */
    uint8_t  Cmd;
    uint8_t  Ocmd;
    uint8_t  Opd[R_SFMA_OPTION_DATA_NUM];
} r_sfma_SpibscSpimdReg_t;


/***********************************************************
  typedef: r_sfma_SpiConfig_t

  Description:
  This type describes the configuration of the SPI.

  Struct members:
  MemoryNum    - The parameter specifies the number of the connecting serial flash memories.
                 <R_SFMA_SPIBSC_CMNCR_BSZ_SINGLE> One serial flash memory is connected.
                 <R_SFMA_SPIBSC_CMNCR_BSZ_DUAL> Two serial flash memories are connected.
  MaxClock     - The parameter specifies the maximum clock of connected the 
                 serial flash memory (Hz).
  PageSize     - The parameter specifies the page size of connected serial flash memory (Byte).
  EraseSectorSize  - The parameter specifies the erase sector size of connected
                     serial flash memory (Byte).
  FlashSize    - Size of connected the serial flash memory (byte).
  Command      - See <r_sfma_FlashCommand_t>.
  DataWidth    - The parameter specifies the data bus width.
                 <R_SFMA_SPIBSC_1BIT> 1 bit data bus width.
                 <R_SFMA_SPIBSC_2BIT> 2 bits data bus width.
                 <R_SFMA_SPIBSC_4BIT> 4 bits data bus width.

  TransferMode - The parameter specifies the transfer mode.
                 <R_SFMA_SPIBSC_SDR_TRANS> SDR transfer.
                 <R_SFMA_SPIBSC_DDR_TRANS> DDR transfer.
  AddressMode  - The parameter specifies the format of address output to the serial flash memory.
                 <R_SFMA_SPIBSC_OUTPUT_ADDR_24> 24 bit address output.
                 <R_SFMA_SPIBSC_OUTPUT_ADDR_32> 32 bit address output.
  Cachemode    - R_SFMA_CACHE_OFF - SFMA cache off.
                 R_SFMA_CACHE_BL1 .. BL16 - SFMA cache on, with set burst length
                 (BL1 = 1 data unit of 64bit, BL16 = 16 data units of 64bit)

  Calibration  - The parameter specifies the calibration value to use for the clock delay

  PerformanceEnMode - The parameter specifies if performance enhance mode should be used

*/
typedef struct
{
    uint32_t              MemoryNum;
    uint32_t              MaxClock;
    uint32_t              PageSize;
    uint32_t              EraseSectorSize;
    uint64_t              FlashSize;
    /* Flash memory control command. */
    r_sfma_FlashCommand_t Command;
    uint8_t               DataWidth;
    uint8_t               TransferMode;
    uint8_t               AddressMode;
    uint32_t              Cachemode;
    uint32_t              PerformanceEnMode;
    uint32_t              Mode;
} r_sfma_SpiConfig_t;

/***************************************************************************
  Section: Global functions
*/

/***************************************************************************
  Function: R_SFMA_SpibscOpen

  Description:
  This function opens the unit by specified configuration mode.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscOpen(const uint32_t                   Unit,
                                 const r_sfma_SpiConfig_t * const SpiConfig);

/***************************************************************************
  Function: R_SFMA_SpibscJEDECRead

  Description:
  This function reads basic JEDEC data.

  Parameter:
  Unit - The parameter specifies the instance number.
  ManufacturerID - This is a pointer to the manufacturer ID.
  MemoryType     - This is a pointer to the memory type.
  Capacity       - This is a pointer to the memory capacity.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscJEDECRead(const uint32_t Unit,
                                      uint8_t * const ManufacturerID,
                                      uint8_t * const MemoryType,
                                      uint8_t * const Capacity);

/***************************************************************************
  Function: R_SFMA_SpibscAccessAddressSet

  Description:
  This function sets the address of the serial flash memory.

  Parameter:
  Unit  - The parameter specifies the instance number.
  Addr  - The parameter specifies the access address of the serial flash memory.
  Range - The parameter specifies the access range of the serial flash memory.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscAccessAddressSet(const uint32_t Unit,
                                             const uint64_t Addr,
                                             const uint8_t  Range);

/***************************************************************************
  Function: R_SFMA_SpibscReadStatusRegister

  Description:
  This function reads the status register from the serial flash memory.

  Parameter:
  Unit : The parameter specifies the instance number.
  Status1 : This is a pointer to the parameter of the read status register.
  Status2 : This is a pointer to the parameter of the read status register.
  SpiConfig : This is a pointer to the <r_sfma_SpibscSpimdReg_t> structure to
              the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscReadStatusRegister(const uint32_t           Unit,
                                                     uint8_t * const    Status1,
                                                     uint8_t * const    Status2,
                                               const r_sfma_SpiConfig_t *SpiConfig);

/***************************************************************************
  Function: R_SFMA_SpibscReadConfigRegister

  Description:
  This function reads the configuration register from the serial flash memory.

  Parameter:
  Unit      - The parameter specifies the instance number.
  Config1   - This is a pointer to the parameter of the read configuration register.
  Config2   - This is a pointer to the parameter of the read configuration register.
  SpiConfig - This is a pointer to the <r_sfma_SpiConfig_t> structure to
              the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscReadConfigRegister(const uint32_t          Unit,
                                                     uint8_t * const   Config1,
                                                     uint8_t * const   Config2,
                                              const r_sfma_SpiConfig_t *SpiConfig);

/***************************************************************************
  Function: R_SFMA_SpibscWriteEnableRegister

  Description:
  This function writes enable register to the serial flash memory.

  Parameter:
  Unit      - The parameter specifies the instance number.
  SpiConfig - This is a pointer to the <r_sfma_SpiConfig_t> structure to
              the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscWriteEnableRegister(const uint32_t           Unit,
                                                const r_sfma_SpiConfig_t *SpiConfig);

/***************************************************************************
  Function: R_SFMA_SpibscWriteStatusRegister

  Description:
  This function writes status  register to the serial flash memory.

  Parameter:
  Unit   - The parameter specifies the instance number.
  Status - The parameter specifies the value to write in
           the status register.
  Config - The parameter specifies the value to write in
           the configuration register.
  SpiConfig - This is a pointer to the <r_sfma_SpiConfig_t> structure to
              the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscWriteStatusRegister(const uint32_t           Unit,
                                                const uint8_t            Status,
                                                const uint8_t            Config,
                                                const r_sfma_SpiConfig_t *SpiConfig);

/***************************************************************************
  Function: R_SFMA_SpibscBusyWait

  Description:
  This function waits for the flash to accept command.

  Parameter:
  Unit      - The parameter specifies the instance number.
  SpiConfig - This is a pointer to the <r_sfma_SpiConfig_t> structure to
              the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscBusyWait(const uint32_t           Unit,
                                     const r_sfma_SpiConfig_t *SpiConfig);

/***************************************************************************
  Function: R_SFMA_SpibscRegsetInit

  Description:
  This function sets the initial parameter for the SPI operating mode.

  Parameter:
  Regset - This is a pointer to the <r_sfma_SpibscSpimdReg_t> structure
           to transfer the data.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscRegsetInit(r_sfma_SpibscSpimdReg_t * const Regset);


/***************************************************************************
  Function: R_SFMA_SpibscCommonInit

  Description:
  This function initializes the operating mode. Makes initial
                 settings necessary to control the serial flash memory.

  Parameter:
  Unit - The parameter specifies the instance number.
  Bsz  - The parameter specifies the number of serial flash memories to connect.
  Spbr - The parameter specifies the value of the bit rate.
  Brdv - The parameter specifies the value of the bit rate frequency division.
  DataWidth    - The parameter specifies the data bus width.
  TransferMode - The parameter specifies the data transfer mode.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscCommonInit(const uint32_t Unit,
                                       const uint32_t Bsz,
                                       const uint8_t  Spbr,
                                       const uint8_t  Brdv,
                                       const uint8_t  DataWidth,
                                       const uint8_t  TransferMode);

/***************************************************************************
  Function: R_SFMA_SpibscDrInit

  Description:
  This function sets initial settings necessary to use the SPIBSC
  in external address space read mode.

  Parameter:
  Unit      - The parameter specifies the instance number.
  SpiConfig - This is a pointer to the <r_sfma_SpiConfig_t> structure
              to the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscDrInit(const uint32_t                   Unit,
                                   const r_sfma_SpiConfig_t * const SpiConfig);

/***************************************************************************
  Function: R_SFMA_SpibscClose

  Description:
  This function closes the unit.

  Parameter:
  Unit - The parameter specifies the instance number.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscClose(const uint32_t Unit);

/***************************************************************************
  Function: R_SFMA_SpibscCommandTransfer

  Description:
  This function transfers the command to the serial flash memory.
  Issues commands to the serial flash memory according to arguments.

  Parameter:
  Unit   - The parameter specifies the instance number.
  Regset - This is a pointer to the <r_sfma_SpibscSpimdReg_t>
           structure to transfer the data.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscCommandTransfer(const uint32_t                        Unit,
                                                  r_sfma_SpibscSpimdReg_t * const Regset);

/***************************************************************************
  Function: R_SFMA_SpibscWriteTransfer

  Description:
  This function transfers write command and data to the serial flash memory.

  Parameter:
  Unit   - The parameter specifies the instance number.
  Regset - This is a pointer to the <r_sfma_SpibscSpimdReg_t>
           structure to transfer the data.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscWriteTransfer(const uint32_t                        Unit,
                                          const r_sfma_SpibscSpimdReg_t * const Regset);

/***************************************************************************
  Function: R_SFMA_SpibscReadTransfer

  Description:
  This function reads data from the serial flash memory after
  sending read command to the serial flash memory.

  Parameter:
  Unit   - The parameter specifies the instance number.
  Regset - This is a pointer to the <r_sfma_SpibscSpimdReg_t>
           structure to transfer the data.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscReadTransfer(const uint32_t                        Unit,
                                               r_sfma_SpibscSpimdReg_t * const Regset);

/***************************************************************************
  Function: R_SFMA_SpibscDrAddr

  Description:
  This function sets the data read extended address.

  Parameter:
  Unit - The parameter specifies the instance number.
  Eav  - The parameter specifies the upper address value of the external address.
  Eac  - The parameter specifies the range of the external address.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscDrAddr(const uint32_t Unit,
                                   const uint8_t  Eav,
                                   const uint8_t  Eac);

/***************************************************************************
  Function: R_SFMA_SpibscWriteExitExtAddr

  Description:
  This function exits the read extended address.

  Parameter:
  Unit - The parameter specifies the instance number.
  SpiConfig - This is a pointer to the <r_sfma_SpiConfig_t> structure
              to the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscWriteExitExtAddr(const uint32_t Unit,
                                             const r_sfma_SpiConfig_t *SpiConfig);


/***************************************************************************
  Function: R_SFMA_SpibscWriteReset

  Description:
  This function writes a software reset.

  Parameter:
  Unit - The parameter specifies the instance number.
  SpiConfig - This is a pointer to the <r_sfma_SpiConfig_t> structure
              to the configuration of the unit.

  Return value:
  See <r_sfma_Error_t>.
*/
r_sfma_Error_t R_SFMA_SpibscWriteReset(const uint32_t Unit,
                                       const r_sfma_SpiConfig_t *SpiConfig);





#ifdef __cplusplus
}
#endif

#endif /* R_SFMA_SPIBSC_H_ */
