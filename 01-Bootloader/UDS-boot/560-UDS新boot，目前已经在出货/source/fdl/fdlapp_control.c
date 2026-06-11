/*********************************************************************************************************************
 * File Name     : $Source: fdlapp_control.c $
 * Mod. Revision : $Revision: 1.8 $
 * Mod. Date     : $Date: 2014/07/30 13:14:47MESZ $
 * Device(s)     : RV40 Flash based RH850 microcontroller
 * Description   : Application sample program control module
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * DISCLAIMER 
 * This software is supplied by Renesas Electronics Corporation and is only  intended for use with Renesas products. 
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
 * applicable laws, including copyright laws. 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED 
 * OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS 
 * AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY 
 * REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH 
 * DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link: 
 * http://www.renesas.com/disclaimer 
 * 
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.     
 *********************************************************************************************************************/


/*********************************************************************************************************************
 * header files include
 *********************************************************************************************************************/
#include "r_typedefs.h"
#include "r_fdl.h"
#include "fdl_descriptor.h"
#include "target.h"
#include "Types.h"

uint32_t rBuf_au32[0x8];    /* Read buffer must be 32bit aligned as the read operation accesses it 32bit aligned */
volatile uint16_t data;
void fdl_check(void);

void fdl_prepenv(void)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
	
#ifndef R_FDL_LIB_V1_COMPATIBILITY
    /* Prepare the environment */
    req.command_enu     = R_FDL_CMD_PREPARE_ENV;
    //req.idx_u32         = 0;
    //req.cnt_u16         = 0;
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute( &req );
        
    while( R_FDL_BUSY == req.status_enu )
    {
	R_FDL_Handler();
    }
    if( R_FDL_OK != req.status_enu )
    {   
	/* Error handler */
	while( 1 )
                ;
    }
#endif
}

void fdl_erase(unsigned long dst)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
    
    req.command_enu     = R_FDL_CMD_ERASE;
    req.idx_u32         = dst / 64;
    req.cnt_u16         = 1;
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute(&req);
    
    while(R_FDL_BUSY == req.status_enu)
    {
        R_FDL_Handler();
    }
    if(R_FDL_OK != req.status_enu)
    {   
        /* Error handler */
        while(1);
    }
}

void fdl_write(unsigned long dst, unsigned int wBuf_u32)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
	
    req.command_enu     = R_FDL_CMD_WRITE;
    req.idx_u32         = dst;
    req.cnt_u16         = 1;
    req.bufAddr_u32     = (uint32_t)(&wBuf_u32);
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute(&req);
    while( R_FDL_BUSY == req.status_enu )
    {
        R_FDL_Handler();
    }
    if( R_FDL_OK != req.status_enu )
    {   
	return;
    }
}

unsigned int fdl_read(unsigned long dst)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
#pragma alignvar(4)
    uint32_t             rBuf_u32 = 0;
	    
    req.command_enu     = R_FDL_CMD_READ;
    req.idx_u32         = dst;
    req.cnt_u16         = 1;
    req.bufAddr_u32     = (uint32_t)(&rBuf_u32);
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute(&req);
    while(R_FDL_BUSY == req.status_enu)
    {
        R_FDL_Handler();
    }
    if(R_FDL_OK != req.status_enu)
    {   
	return 0x0;
    }
    return rBuf_u32;
}
void fdl_writewords(unsigned int dst, unsigned int wBuf_u32[],unsigned char length)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
	
    req.command_enu     = R_FDL_CMD_WRITE;
    req.idx_u32         = dst;
    req.cnt_u16         = length;
    req.bufAddr_u32     =   (uint32_t)(wBuf_u32);
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute(&req);
    while( R_FDL_BUSY == req.status_enu )
    {
        R_FDL_Handler();
    }
    if( R_FDL_OK != req.status_enu )
    {   
	return;
    }
}

void fdl_readwords(unsigned long dst,unsigned int rBuf_u32[],unsigned char length)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
    #pragma alignvar(4)
   // uint32_t             rBuf_u32[6];
	    
    req.command_enu     = R_FDL_CMD_READ;
    req.idx_u32         = dst;
    req.cnt_u16         = length;
    req.bufAddr_u32     = (uint32_t)(rBuf_u32);
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute(&req);
    while(R_FDL_BUSY == req.status_enu)
    {
        R_FDL_Handler();
    }
    if(R_FDL_OK != req.status_enu)
    {   
	;
    }

}

void fdl_read2(void)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
	
    /* -----------------------------------------------------------------------
       Read data:
       addresses 0x00000020 - 0x00000023 
       ... Most probably resulting in ECC error
       ----------------------------------------------------------------------- */   
    req.command_enu     = R_FDL_CMD_READ;
    req.idx_u32         = 0x0020;
    req.cnt_u16         = 0x1;
    req.bufAddr_u32     = (uint32_t)(&rBuf_au32[0]);
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute(&req);
    while(R_FDL_BUSY == req.status_enu)
    {
        R_FDL_Handler();
    }
    
    if((R_FDL_ERR_ECC_SED != req.status_enu) &&
        (R_FDL_ERR_ECC_DED != req.status_enu))
    {   
        /* Error handler */
        while(1);
    }
}

/*********************************************************************************************************************
 * Module internal function prototypes
 *********************************************************************************************************************/
unsigned int EEROM_ReadWord(unsigned long dst)
{
    return fdl_read(dst);
}

void EEROM_WriteWord(unsigned long dst, unsigned int val) 
{
    fdl_erase(dst);
    fdl_write(dst, val);
}

/*********************************************************************************************************************
 * Module internal function prototypes
 *********************************************************************************************************************/
void EEROM_ReadWords(unsigned long dst,unsigned int rbuf[],unsigned char length)
{
fdl_readwords(dst,rbuf,length);
}

void EEROM_WriteWords(unsigned int dst, unsigned int val[],unsigned char length) 
{
    fdl_erase(dst);
    fdl_writewords(dst, val,length);
}
void EEROM_WriteWords_2block(unsigned int dst, unsigned int val[],unsigned char length) 
{
    fdl_erase(dst);
      fdl_erase(dst+0x40);
    fdl_writewords(dst, val,length);
}
/*********************************************************************************************************************
   Function name:  SampleApp_FDL_Control
 *********************************************************************************************************************/
/**
    EEL sample function to handle FDL functionality. Executing a set of dummy Flash operations

    @param          ---
    @return         ---
 */  
/*********************************************************************************************************************/

void SampleApp_FDL_Control(void)
{
    r_fdl_status_t            fdlRet;
    unsigned int              val  =  56;
    
    /* 1st initialize the FDL */
    fdlRet = R_FDL_Init(&sampleApp_fdlConfig_enu);
    if(R_FDL_OK != fdlRet)
    {   
        /* Error handler */
        while(1);
    }
    fdl_prepenv();
    //fdl_check();	
    return;
}


void fdl_check(void)
{
    r_fdl_status_t            fdlRet;
    r_fdl_request_t           req;
	
    /* -----------------------------------------------------------------------
       Example...
       Read a Word --> address 0x0004 of the Data Flash
       1) Do a blank check to distinguish if the read value is based on erased
          data or written data. We can only trust written data
       2) read the data 
       ----------------------------------------------------------------------- */   
    req.command_enu     = R_FDL_CMD_BLANKCHECK;
    req.idx_u32         = 0x0000;
    req.cnt_u16         = 1;
    req.accessType_enu  = R_FDL_ACCESS_USER;
    R_FDL_Execute( &req );
    
    while( R_FDL_BUSY == req.status_enu )
    {
        R_FDL_Handler();
    }
    
    if( R_FDL_OK == req.status_enu )
    {   
        /* The half word is blank... we may not read */
    }
    else if( R_FDL_ERR_BLANKCHECK == req.status_enu )
    {
        /* Read the data */
        //data = (*(uint16_t*)0xff200000);
    }
    else
    {
        /* Error handler.. on an internal error */
        while( 1 );
    }
	
    if(fdl_read(0) == 0x11)
    {
	EEROM_WriteWord(0x00, 0);
	EEROM_WriteWord(0x40, 0);
	EEROM_WriteWord(0x80, 0);//totail miles
		
	EEROM_WriteWord(0x06C0, 0);
	EEROM_WriteWord(0x07C0, 0);
	EEROM_WriteWord(0x08C0, 0);
		
	EEROM_WriteWord(0x00C0, 300);
	EEROM_WriteWord(0x01C0, 300);
	EEROM_WriteWord(0x02C0, 300);//backlight
		
	EEROM_WriteWord(0x03C0, 4);
	EEROM_WriteWord(0x04C0, 4);
	EEROM_WriteWord(0x05C0, 4);//address
		
	EEROM_WriteWord(0x1400, 0);
	EEROM_WriteWord(0x1440, 0);
	EEROM_WriteWord(0x1480, 0);//single miles
    }
}

uint8_t Diagnostic_EEProm_Write(uint32_t address, uint8_t size, uint8_t data[])
{ 
    uint32_t writebuf[32];
    if((address%64) != 0)return FALSE;
    if(size<65){
	for(uint8_t i = 0;i<16;i++){
	    writebuf[i] = data[i*4]+(data[i*4+1]<<8)+(data[i*4+2]<<16)+(data[i*4+3]<<24);

	}
    }
    else{
	for(uint8_t i = 0;i<32;i++){
	    writebuf[i] = data[i*4]+(data[i*4+1]<<8)+(data[i*4+2]<<16)+(data[i*4+3]<<24);

	}
    }
    if(size<65){
	if((size%4) == 0)
	    EEROM_WriteWords(address,writebuf,size/4);
	else
	    EEROM_WriteWords(address,writebuf,size/4+1);
    }
    else{
	if((size%4) == 0)
	    EEROM_WriteWords_2block(address,writebuf,size/4);
	else
	    EEROM_WriteWords_2block(address,writebuf,size/4+1);
    }
	
    return 1;

}


/**
 * Diagnostic_EEProm_Read - reas eeprom data by address and length
 *
 * @address : the target address
 * @data : data pointer of readback data
 * @size : data length to read
 *
 * returns:
 *     failed or pass
 */
uint8_t Diagnostic_EEProm_Read(uint32_t address, uint8_t size, uint8_t *data)
{ 
    uint32_t readbuf[32];
    uint32_t addressbuf;
    uint8_t lfetaddress1;
    uint8_t lfetaddress2;
        uint8_t lfetaddress3;
	uint8_t uint32size;
    lfetaddress1 = address%64;
    lfetaddress2 = address%4;
  
    lfetaddress3 = lfetaddress1 /4;
      if(size<65)uint32size = 16;
      else uint32size = 32;
      
    if(lfetaddress1  !=  0 ){
	addressbuf = address/64 * 64 ;

	EEROM_ReadWords(addressbuf,readbuf,uint32size);
	if(lfetaddress2 == 0){
	    for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
		if((4*i)<size)
		    data[4*i] = readbuf[i+lfetaddress3 ];
		if((4*i+1)<size)
		    data[4*i+1] = (readbuf[i+lfetaddress3]>>8);
		if((4*i+2)<size)
		    data[4*i+2] = (readbuf[i+lfetaddress3]>>16);
		if((4*i+3)<size)
		    data[4*i+3] = (readbuf[i+lfetaddress3]>>24);
	    }
	}
	else if(lfetaddress2 == 1){
	    data[0] = (readbuf[lfetaddress3]>>8);
	    if(size<2)return 1;
	    data[1] = (readbuf[lfetaddress3]>>16);
	     if(size<3)return 1;
	    data[2] = (readbuf[lfetaddress3]>>24);
	     if(size<4)return 1;
	    for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
		if((4*i+3)<size)
		    data[4*i+3] = readbuf[i+lfetaddress3 +1];
		if((4*i+4)<size)
		    data[4*i+4] = (readbuf[i+lfetaddress3+1]>>8);
		if((4*i+5)<size)
		    data[4*i+5] = (readbuf[i+lfetaddress3+1]>>16);
		if((4*i+6)<size)
		    data[4*i+6] = (readbuf[i+lfetaddress3+1]>>24);
	    }
	}
	else if(lfetaddress2 == 2){
	    data[0] = (readbuf[lfetaddress3]>>16);
	     if(size<2)return 1;
	    data[1] = (readbuf[lfetaddress3]>>24);
	     if(size<3)return 1;
	    for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
		if((4*i+2)<size)
		    data[4*i+2] = readbuf[i+lfetaddress3 +1];
		if((4*i+3)<size)
		    data[4*i+3] = (readbuf[i+lfetaddress3+1]>>8);
		if((4*i+4)<size)
		    data[4*i+4] = (readbuf[i+lfetaddress3+1]>>16);
		if((4*i+5)<size)
		    data[4*i+5] = (readbuf[i+lfetaddress3+1]>>24);
	    }
	}
	else if(lfetaddress2 == 3){
	    data[0] = (readbuf[lfetaddress3]>>24);
	     if(size<2)return 1;
	    for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
		if((4*i+1)<size)
		    data[4*i+1] = readbuf[i+lfetaddress3+1 ];
		if((4*i+2)<size)
		    data[4*i+2] = (readbuf[i+lfetaddress3+1]>>8);
		if((4*i+3)<size)
		    data[4*i+3] = (readbuf[i+lfetaddress3+1]>>16);
		if((4*i+4)<size)
		    data[4*i+4] = (readbuf[i+lfetaddress3+1]>>24);
	    }
	}
	else;


}
    else{

	EEROM_ReadWords(address,readbuf,uint32size);
	    for(uint8_t i = 0;i<uint32size-lfetaddress3;i++){
		if((4*i)<size)
		    data[4*i] = readbuf[i+lfetaddress3 ];
		if((4*i+1)<size)
		    data[4*i+1] = (readbuf[i+lfetaddress3]>>8);
		if((4*i+2)<size)
		    data[4*i+2] = (readbuf[i+lfetaddress3]>>16);
		if((4*i+3)<size)
		    data[4*i+3] = (readbuf[i+lfetaddress3]>>24);
	}

    }
 
    return 1;
}


