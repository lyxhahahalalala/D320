/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* (c) 2014 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/


/******************************************************************************
* File Name     : r_iic_master.c
* Version       : 1.0
* Device(s)     : R7F701035xAFP RH850/F1L
* Tool-Chain    : CubeSuite+(V2.01.00)
* Description   : This file is a sample of the IIC.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 20.03.2014 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include    "r_typedefs.h"
#include    "iodefine.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define IDLE_STATE               ( 0x00U )

#define ADDR_W_STATE             ( 0x10U )
#define ADDR_R_STATE             ( 0x11U )

#define DATA_SEND_STATE          ( 0x20U )

#define DATA_SEND_END_STATE      ( 0x22U )

#define DATA_RECV_STATE          ( 0x30U )
#define DATA_RECV_STATE_2        ( 0x31U )
#define DATA_RECV_STATE_3        ( 0x32U )
#define DATA_RECV_STATE_4        ( 0x33U )

#define DATA_RECV_END_STATE      ( 0x40U )
#define DATA_RECV_END_STATE_2    ( 0x41U )

#define SLAVE_ADDR               ( 0x10U )

#define IIC_DIR_W                ( 0x00U )
#define IIC_DIR_R                ( 0x01U )

typedef struct tst_iic_param {
    uint32_t    iic_state;

    uint32_t    tx_count;
    uint32_t    tx_len;
    uint8_t    *tx_data_p;

    uint32_t    rx_count;
    uint32_t    rx_len;
    uint8_t    *rx_data_p;
} IIC_PARAM;


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
        void        R_IIC_Init(void);

        void        R_IIC_Master_Transmit(uint32_t , uint8_t*);
        void        R_IIC_Master_Reception(uint32_t , uint8_t*);

        IIC_PARAM   iic_prm;


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name : void R_IIC_Init( void )
* Description   : This function initialize IIC.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_IIC_Init(void)
{
    uint32_t   reg32_value;

    /* Set the interrupt flags */
    /*
    MKRIIC0TI           = 1U;
    RFRIIC0TI           = 0U;
    TBRIIC0TI           = 0U;

    MKRIIC0TEI          = 1U;
    RFRIIC0TEI          = 0U;
    TBRIIC0TEI          = 0U;

    MKRIIC0RI           = 1U;
    RFRIIC0RI           = 0U;
    TBRIIC0RI           = 0U;

    MKRIIC0EE           = 1U;
    RFRIIC0EE           = 0U;
    TBRIIC0EE           = 0U;
    */

    /* Disabled I2C Bus Interface
    RIICnCR1            - I2C Bus Control Register 1
    b31:b 8             - Reserved set to 0
    b 7          ICE    - I2C Bus Interface Enable                    - Disabled (the RIICnSCL and RIICnSDA pins are not driven). set to 0
    b 6          IICRST - I2C Bus Interface Internal Reset            - not change.
    b 5          CLO    - Extra SCL Clock Cycle Output                - not change.
    b 4          SOWP   - SCLO/SDAO Write Protect                     - not change.
    b 3          SCLO   - SCL Output Control/Monitor                  - not change.
    b 2          SDAO   - SDA Output Control/Monitor                  - not change.
    b 1          SCLI   - SCL Line Monitor                            - not change.
    b 0          SDAI   - SDA Line Monitor                            - not change. */
    RIIC0.CR1.UINT32    &= 0xFFFFFF7FUL;


    /* I2C Bus Interface Internal Reset
    RIICnCR1            - I2C Bus Control Register 1
    b31:b 8             - Reserved set to 0
    b 7          ICE    - I2C Bus Interface Enable                    - not change.
    b 6          IICRST - I2C Bus Interface Internal Reset            - Initiates the RIIC reset or internal reset. set to 1
    b 5          CLO    - Extra SCL Clock Cycle Output                - not change.
    b 4          SOWP   - SCLO/SDAO Write Protect                     - not change.
    b 3          SCLO   - SCL Output Control/Monitor                  - not change.
    b 2          SDAO   - SDA Output Control/Monitor                  - not change.
    b 1          SCLI   - SCL Line Monitor                            - not change.
    b 0          SDAI   - SDA Line Monitor                            - not change. */
    RIIC0.CR1.UINT32    |= 0x00000040UL;


    /* Enabled I2C Bus Interface
    RIICnCR1            - I2C Bus Control Register 1
    b31:b 8             - Reserved set to 0
    b 7          ICE    - I2C Bus Interface Enable                    - Enabled (the RIICnSCL and RIICnSDA pins driven). set to 1
    b 6          IICRST - I2C Bus Interface Internal Reset            - not change.
    b 5          CLO    - Extra SCL Clock Cycle Output                - not change.
    b 4          SOWP   - SCLO/SDAO Write Protect                     - not change.
    b 3          SCLO   - SCL Output Control/Monitor                  - not change.
    b 2          SDAO   - SDA Output Control/Monitor                  - not change.
    b 1          SCLI   - SCL Line Monitor                            - not change.
    b 0          SDAI   - SDA Line Monitor                            - not change. */
    RIIC0.CR1.UINT32    |= 0x00000080UL;


    RIIC0.SAR0.UINT32   = (uint32_t)SLAVE_ADDR;                       /* SAR0 7bit address */


    /* Slave address in RIICnSAR0 is enabled.
    RIICnSER            - I2C Bus Status Enable Register
    b31:b 6             - Reserved set to 0
    b 5          DIDE   - Extra SCL Clock Cycle Output                - Device-ID address detection is disabled. set to 0
    b 4                 - Reserved set to 0
    b 3          GCE    - SCL Output Control/Monitor                  - General call address detection is disabled. set to 0
    b 2          SAR2   - SDA Output Control/Monitor                  - Slave address in RIICnSAR2 is disabled. set to 0
    b 1          SAR1   - SCL Line Monitor                            - Slave address in RIICnSAR1 is disabled. set to 0
    b 0          SAR0   - SDA Line Monitor                            - Slave address in RIICnSAR0 is enabled. set to 1 */
    RIIC0.SER.UINT32    = 0x00000001UL;


    /* Internal Reference Clock Selection.
    RIICnMR1            - I2C Bus Mode Register 1
    b31:b 8             - Reserved set to 0
    b 7          MTWP   - MST/TRS Write Protect                       - not change.
    b 6:b 4      CKS    - Internal Reference Clock Selection          - PCLK/16 clock. set to 100'b
    b 3          BCWP   - BC Write Protect                            - not change.
    b 2:b 0      BC     - Bit Counter                                 - not change. */
    RIIC0.MR1.UINT32    |= 0x00000040UL;                              /* 8MHz / 16  */

    RIIC0.BRH.UINT32    = 0xF6U;                                      /* 10kbps     */
    RIIC0.BRL.UINT32    = 0xF9U;


    /* Set the I2C Bus Mode.
    RIICnMR2            - I2C Bus Mode Register 2
    b31:b 8             - Reserved set to 0
    b 7          DLCS   - SDA Output Delay Clock Source Selection     - not change.
    b 6:b 4      SDDL   - SDA Output Delay Counter                    - not change.
    b 3                 - Reserved set to 0
    b 2          TMOH   - Timeout H Count Control                     - not change.
    b 1          TMOL   - Timeout L Count Control                     - not change.
    b 0          TMOS   - Timeout Detection Time Selection            - not change. */
    RIIC0.MR2.UINT32    = 0x00000006UL;


    /* Set the I2C Bus Mode.
    RIICnMR3            - I2C Bus Mode Register 3
    b31:b 7             - Reserved set to 0
    b 6          WAIT   - WAIT                                        - No WAIT. set to 0
    b 5          RDRFS  - RDRF Flag Set Timing Selection              - The RDRF flag is set at the rising edge of the
                                                                        ninth SCL clock cycle. set to 0
    b 4          ACKWP  - ACKBT Write Protect                         - Modification of the ACKBT bit is enabled. set to 1
    b 3          ACKBT  - Transmit Acknowledge                        - A 0 is sent as the acknowledge bit. set to 0
    b 2          ACKBR  - Receive Acknowledge                         - A 0 is received as the acknowledge bit. set to 0
    b 1:b 0      NF     - Digital noise Filter Stage Selection        - Noise of up to one IIC cycle is filtered out. set to 00'b */
    RIIC0.MR3.UINT32    = 0x00000010UL;


    /* Set the I2C Bus functions.
    RIICnFER            - I2C Bus Function Enable Register
    b31:b 7             - Reserved set to 0
    b 6          SCLE   - SCL Synchronous Circuit Enable              - An SCL synchronous circuit is unused. set to 0
    b 5          NFE    - Digital Noise Filter Circuit Enable         - A digital noise filter circuit is unused. set to 0
    b 4          NACKE  - NACK Reception Transfer Suspension Enable   - Transfer operation is suspended during NACK reception. set to 1
    b 3          SALE   - Slave Arbitration-Lost Detection Enable     - Slave arbitration-lost detection is disabled. set to 0
    b 2          NALE   - NACK Transmission Arbitration-Lost
                          Detection Enable                            - NACK transmission arbitration-lost detection is disabled. set to 0
    b 1          MALE   - Master Arbitration-Lost Detection Enable    - Master arbitration-lost detection is disabled. set to 0
    b 0          TMOE   - Timeout Function Enable                     - The timeout function is disabled. set to 0 */
    RIIC0.FER.UINT32    = 0x00000010UL;


    /* Set the Interrupts.
    RIICnIER            - I2C Bus Interrupt Enable Register
    b31:b 8             - Reserved set to 0
    b 7          TIE    - Transmit Data Empty Interrupt Enable        - Transmit data empty interrupt request (INTIICnTI) is enabled. set to 1
    b 6          TEIE   - Transmit End Interrupt Enable               - Transmit end interrupt request (INTIICnTEI) is enabled. set to 1
    b 5          RIE    - Receive Complete Interrupt Enable           - Receive complete interrupt request (INTIICnRI) is enabled. set to 1
    b 4          NAKIE  - NACK Reception Interrupt Enable             - NACK reception interrupt request (NAKI) is disabled. set to 0
    b 3          SPIE   - Stop Condition Detection Interrupt Enable   - Stop condition detection interrupt request (SPI) is enabled. set to 1
    b 2          STIE   - Start Condition Detection Interrupt Enable  - Start condition detection interrupt request (STI) is disabled. set to 0
    b 1          ALIE   - Arbitration-Lost Interrupt Enabl       e    - Arbitration-lost interrupt request (ALI) is disabled. set to 0
    b 0          TMOIE  - Timeout Interrupt Enable                    - Timeout interrupt request (TMOI) is disabled. set to 0 */
    RIIC0.IER.UINT32    = 0x000000E8UL;


    /* Alternative function port settings */
    /* RIIC0SDA(P10_2:CN1D-110pin) 2'nd Alternative (output/input) */
    /* RIIC0SCL(P10_3:CN1D-109pin) 2'nd Alternative (output)  */
 /*    do
    {
        reg32_value     = PODC10 | 0x0000000CUL;
        PPCMD0          = 0xA5U;
        PODC10          = reg32_value;
        PODC10          = (uint32_t)(~reg32_value);
        PODC10          = reg32_value;
    } while ( PPROTS0 != 0x00000000UL );
   
    PBDC10              |= 0x000CU;
    PFCAE10             &= 0xFFF3U;
    PFCE10              &= 0xFFF3U;
    PFC10               |= 0x000CU;
    PMC10               |= 0x000CU;
    PM10                &= 0xFFF3U;
    */

    /* I2C Bus Interface Internal Reset
    RIICnCR1            - I2C Bus Control Register 1
    b31:b 8             - Reserved set to 0
    b 7          ICE    - I2C Bus Interface Enable                    - not change.
    b 6          IICRST - I2C Bus Interface Internal Reset            - Clears the RIIC reset or internal reset. set to 0
    b 5          CLO    - Extra SCL Clock Cycle Output                - not change.
    b 4          SOWP   - SCLO/SDAO Write Protect                     - not change.
    b 3          SCLO   - SCL Output Control/Monitor                  - not change.
    b 2          SDAO   - SDA Output Control/Monitor                  - not change.
    b 1          SCLI   - SCL Line Monitor                            - not change.
    b 0          SDAI   - SDA Line Monitor                            - not change. */
    RIIC0.CR1.UINT32    &= 0xFFFFFFBFUL;


    /* interrupt mask release */
    /*
    MKRIIC0TI           = 0U;
    MKRIIC0TEI          = 0U;
    MKRIIC0RI           = 0U;
    MKRIIC0EE           = 0U;
    */
}


/******************************************************************************
* Function Name : void R_IIC_Master_Transmit( uint32_t len , uint8_t *transmission_data )
* Description   : This function transmits the master mode.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_IIC_Master_Transmit(uint32_t len , uint8_t *transmission_data)
{
    while(RIIC0.CR2.UINT32 & 0x00000080UL);                         // Wait to a bus-free state

    iic_prm.iic_state   = ADDR_W_STATE;
    iic_prm.tx_data_p   = transmission_data;
    iic_prm.tx_len      = len;
    iic_prm.tx_count    = 0UL;

    /* Requests to issue a start condition.
    RIICnCR2            - I2C Bus Control Register 2
    b31:b 8             - Reserved set to 0
    b 7          BBSY   - Bus Busy Detection Flag                     - not change.
    b 6          MST    - Master/Slave Mode                           - not change.
    b 5          TRS    - Transmit/Receive Mode                       - not change.
    b 4                 - Reserved set to 0
    b 3          SP     - Stop Condition Issuance Request             - not change.
    b 2          RS     - Restart Condition Issuance Request          - not change.
    b 1          ST     - Start Condition Issuance Request            - Requests to issue a start condition. set to 1
    b 0                 - Reserved set to 0 */
    RIIC0.CR2.UINT32    |= 0x00000002UL;

}

/******************************************************************************
* Function Name : void R_IIC_Master_Reception( uint32_t len , uint8_t *reception_data )
* Description   : This function reception the master mode.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_IIC_Master_Reception(uint32_t len , uint8_t *reception_data)
{
    while(RIIC0.CR2.UINT32 & 0x00000080UL);                         // Wait to a bus-free state

    iic_prm.iic_state   = ADDR_R_STATE;
    iic_prm.rx_data_p   = reception_data;
    iic_prm.rx_len      = len;
    iic_prm.rx_count    = 0UL;

    /* Requests to issue a start condition.
    RIICnCR2            - I2C Bus Control Register 2
    b31:b 8             - Reserved set to 0
    b 7          BBSY   - Bus Busy Detection Flag                     - not change.
    b 6          MST    - Master/Slave Mode                           - not change.
    b 5          TRS    - Transmit/Receive Mode                       - not change.
    b 4                 - Reserved set to 0
    b 3          SP     - Stop Condition Issuance Request             - not change.
    b 2          RS     - Restart Condition Issuance Request          - not change.
    b 1          ST     - Start Condition Issuance Request            - Requests to issue a start condition. set to 1
    b 0                 - Reserved set to 0 */
    RIIC0.CR2.UINT32    |= 0x00000002UL;
}

