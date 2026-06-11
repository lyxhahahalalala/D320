/*
****************************************************************************
PROJECT : RIIC driver
FILE    : $Id: r_riic_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for RIIC macro
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2014
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

#ifndef R_RIIC_API_H_
#define R_RIIC_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: RIIC API

  An application using RIIC should include this header file.
*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_riic_Error_t

  RIIC macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_RIIC_ERR_OK           - No error
  R_RIIC_ERR_NG           - Unspecified error
  R_RIIC_ERR_RANGE        - Unit do not exist
  R_RIIC_ERR_PORTINIT     - RIIC port init failed
  R_RIIC_ERR_BUFOVERRUN   - Buffer is already full
  R_RIIC_ERR_RBINIT       - Ring buffer init failed
  R_RIIC_ERR_NODATA       - No data to send
  R_RIIC_ERR_LAST         - Delimiter 
*/

typedef enum
{
    R_RIIC_ERR_OK         = 0x00,
    R_RIIC_ERR_NG         = 0x01,
    R_RIIC_ERR_RANGE      = 0x02,
    R_RIIC_ERR_PORTINIT,
    R_RIIC_ERR_BUFOVERRUN,
    R_RIIC_ERR_RBINIT,
    R_RIIC_ERR_NODATA,
    R_RIIC_ERR_LAST
} r_riic_Error_t;


/*******************************************************************************
  Type: r_riic_Parameter_t

  RIIC macro configuration data

  Members:
  Baudrate       - Baud rate value
*/     

typedef struct {
    uint32_t                    Baudrate;   
} r_riic_Parameter_t;
     


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_RIIC_Init

  Driver init function. Most parameters are set automatically (for standard 
  application)

  Parameters:
  Unit          - Instance number
  Config        - see: <r_riic_Parameter_t>
 
  Returns:
  see: <r_riic_Error_t>
*/

 r_riic_Error_t R_RIIC_Init(uint32_t Unit, r_riic_Parameter_t *Config);


/*******************************************************************************
  Function: R_RIIC_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_riic_Error_t>
*/

 r_riic_Error_t R_RIIC_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_RIIC_data_receive

  Read received data out of the RIIC RX register.
  
  This function simply returns the RX register value, it does
  not check for availability of data.

  Parameters:
  Unit       - Instance number

  Returns:
  Received data
*/
void R_RIIC_data_receive(uint32_t Unit, uint8_t* rx_buffer);

/*******************************************************************************
  Function: R_RIIC_data_send

  Write one value to the RIIC TX register.

  The function waits until any ongoing transmission is completed
  and writes then the data to the TX register.

  Parameters:
  Unit         - Instance number
  TxData       - Value to be transmitted

  Returns:
  see: <r_riic_Error_t>
*/

r_riic_Error_t R_RIIC_data_send(uint32_t Unit, uint8_t* TxData);   


void R_RIIC_Master_start_condition(uint32_t Unit);
void R_RIIC_Master_stop_condition(uint32_t Unit, uint8_t* RxBuffer);
r_riic_Error_t R_RIIC_Slave_address_send(uint32_t Unit, uint8_t *TxData);

/*******************************************************************************
  Function: R_RIIC_transmit_to_slave

  Write a series of values to a IIC slave.

  IIC slave address is assumed to by first data value in tdata.
  Function generates a complete Start-Condition, Slave-Address-Send,
  Data-Send and End-Condition cycle.

  Parameters:
  Unit         - Instance number
  tdata        - array with data to be send
  size         - number of data to be send (size of tdata array)

  Returns:
  see: <r_riic_Error_t>
*/
r_riic_Error_t R_RIIC_transmit_to_slave(uint32_t Unit, uint8_t *Tdata, uint32_t Size);


/*******************************************************************************
  Function: R_RIIC_receive_from_slave16b

  Read 1 or 2 bytes from IIC slave with a 16bit address internal address for the device.
  (note: not all devices may support this format)
 
    <slave address 7bit> <internal address 16 bit> <read 2 bytes>

  Parameters:
  Unit         - Instance number
  SlaveAddress - slave address 7bit
  InternalAddress - the device's internal addtess (16bit)
  Rdata        - buffer to store data read from slave
  size         - number of data bytes to be read 1 or 2

  Returns:
  see: <r_riic_Error_t>
*/
r_riic_Error_t R_RIIC_receive_from_slave16b(uint32_t Unit, uint8_t SlaveAddress, uint16_t InternalAddress, uint8_t *Rdata, uint32_t Size);


/*******************************************************************************
  Function: R_RIIC_receive_from_slave8b

  Read 1 or 2 bytes from IIC slave with a 8bit address internal address for the device.
  (note: not all devices may support this format)
 
    <slave address 7bit> <internal address 8 bit> <read 2 bytes>

  Parameters:
  Unit         - Instance number
  SlaveAddress - slave address 7bit
  InternalAddress - the device's internal addtess (8bit)
  Rdata        - buffer to store data read from slave
  size         - number of data bytes to be read 1 or 2

  Returns:
  see: <r_riic_Error_t>
*/
r_riic_Error_t R_RIIC_receive_from_slave8b(uint32_t Unit, uint8_t SlaveAddress, uint8_t InternalAddress, uint8_t *Rdata, uint32_t Size);


#ifdef __cplusplus
}
#endif

#endif /* R_RIIC_API_H_  */
