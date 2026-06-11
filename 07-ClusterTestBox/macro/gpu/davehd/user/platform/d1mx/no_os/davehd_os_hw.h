/*
****************************************************************************
PROJECT : DHD
FILE    : $Id: davehd_os_hw.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
DHD system abstraction
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2012
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

#ifndef DAVEHD_OS_HW_H__
#define DAVEHD_OS_HW_H__

#include "davehd_kernel_os.h"
#include "davehd_kernel_driver.h"
#include "davehd_kernel_hw.h"

typedef  void * dhd_sys_handle_t;                                   /* abstract handle*/
typedef  void (* dhd_sys_irq_t)(dhd_sys_handle_t a_caller);    /* irq callback function pointer*/


/*----------------------------------------------------------------------------------------------------------*/
/**/
dhd_lock_t dhd_sys_create_lock(void);

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_destroy_lock(dhd_lock_t a_lock);
/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_enter_lock(dhd_lock_t a_lock) ;
/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_exit_lock(dhd_lock_t a_lock);
/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_regwrite(dhd_sys_handle_t a_handle, dhd_uint32_t a_idx, dhd_uint32_t a_data) ;
/*----------------------------------------------------------------------------------------------------------*/
/**/
dhd_uint32_t dhd_sys_regread(dhd_sys_handle_t a_handle, dhd_uint32_t a_idx);

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_register_isr(dhd_sys_handle_t a_handle, dhd_sys_irq_t a_isr);

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_set_callback(dhd_sys_handle_t a_handle, dhd_irq_t a_data) ;

/*----------------------------------------------------------------------------------------------------------*/
/**/
void* dhd_sys_get_callback(dhd_sys_handle_t a_handle);

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_set_device(dhd_sys_handle_t a_handle, void *a_data) ;

/*----------------------------------------------------------------------------------------------------------*/
/**/
void* dhd_sys_get_device(dhd_sys_handle_t a_handle);

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_sleep(dhd_uint32_t a_time) ;


void dhd_sys_IsrDave(dhd_uint32_t Unit);


#endif /* DAVEHD_OS_HW_H__ */
