/*
****************************************************************************
PROJECT : DHD
FILE    : $Id: davehd_os_hw.c 4436 2014-12-02 13:27:26Z florian.zimmermann $
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
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"

#include "davehd_kernel_os.h"
#include "r_config_davehd.h"
#include "davehd_os_hw.h"

#include "davehd_debug.h"

#include <stdio.h>
#include <stdarg.h>


/*******************************************************************************
  Section: Local Defines
*/

#define LOC_MAX_MUTEX_COUNT      (32)



typedef struct dhd_sys_record_s {
  dhd_sys_handle_t handle;
  void*            DHD_Dev;
  void             (*DHD_Callback)();
} dhd_sys_record_t;




/*******************************************************************************
  Section: Local variables
*/


#if defined (R_DAVEHD_0_BASE)
static dhd_sys_record_t loc_dhd_sys_record_Unit0;
#endif

#if defined (R_DAVEHD_1_BASE)
static dhd_sys_record_t loc_dhd_sys_record_Unit1;
#endif




/***********************************************************
  Section: Local Functions
*/

#if defined (R_DAVEHD_0_BASE)
static dhd_sys_irq_t loc_isr_Unit0 = R_NULL;
#endif 

#if defined (R_DAVEHD_1_BASE)
static dhd_sys_irq_t loc_isr_Unit1 = R_NULL;
#endif


#if defined(R_DAVEHD_0_BASE)
static void loc_IsrDaveUnit0(void);
static void loc_IsrDaveUnit0(void) {
  loc_isr_Unit0((dhd_sys_handle_t) R_DAVEHD_0_BASE);
}
#endif

#if defined(R_DAVEHD_1_BASE)
static void loc_IsrDaveUnit1(void) {
  loc_isr_Unit1((dhd_sys_handle_t) R_DAVEHD_1_BASE);
}
#endif

static void loc_set_callback(dhd_sys_record_t* dhd_sys_record, dhd_irq_t a_callback);
static void loc_set_callback(dhd_sys_record_t* dhd_sys_record, dhd_irq_t a_callback) {
  dhd_sys_record->DHD_Callback = a_callback;
}

static void* loc_get_callback(dhd_sys_record_t* dhd_sys_record);
static void* loc_get_callback(dhd_sys_record_t* dhd_sys_record) {
  return dhd_sys_record->DHD_Callback;
}

static void loc_set_device(dhd_sys_record_t* dhd_sys_record, void *a_device);
static void loc_set_device(dhd_sys_record_t* dhd_sys_record, void *a_device) {
  dhd_sys_record->DHD_Dev = a_device;
}

static void* loc_get_device(dhd_sys_record_t* dhd_sys_record);
static void* loc_get_device(dhd_sys_record_t* dhd_sys_record) {
  return dhd_sys_record->DHD_Dev;
}


void dhd_sys_IsrDave(dhd_uint32_t Unit) {

  DHD_ASSERT((Unit == 0) || (Unit == 1));

  switch (Unit) {
#if defined(R_DAVEHD_0_BASE)
    case 0:
      loc_IsrDaveUnit0();
      break;
#endif
#if defined(R_DAVEHD_1_BASE)
    case 1:
      loc_IsrDaveUnit1();
      break;
#endif
    default:
      /* called from vlib/device/d1mx/src/rh850_ghs/r_d1mx_isr.c with parameter*/
      /* Unit == 0 only.*/
      break;
  }
}



void dhd_sys_register_isr(dhd_sys_handle_t a_handle, dhd_sys_irq_t a_isr) {
  switch ((dhd_uint32_t)a_handle) {
    #if defined(R_DAVEHD_0_BASE)
    case R_DAVEHD_0_BASE:
      loc_isr_Unit0 = a_isr;
      break;
    #endif

    #if defined(R_DAVEHD_1_BASE)
    case R_DAVEHD_1_BASE:
      loc_isr_Unit1 = a_isr;
      break;
    #endif

    default:
      /* a_handle comes from the gpu_config-struct, so this line should never be reached*/
      /* on a well configured system.*/
      break;
  }
}


/*----------------------------------------------------------------------------------------------------------*/
/* NOT THREAD SAFE!!! However, all locks come from DHD kernel (which is a single thread)*/
dhd_lock_t dhd_create_lock(void) {
   dhd_lock_t result = (dhd_lock_t)1;
   return result;
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_destroy_lock(dhd_lock_t a_lock) {
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_enter_lock(dhd_lock_t a_lock) {

}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_exit_lock(dhd_lock_t a_lock) {

}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_regwrite(dhd_sys_handle_t a_handle, dhd_uint32_t a_idx, dhd_uint32_t a_data) {

  *(volatile uint32_t*)(R_DAVEHD_0_BASE + (a_idx << 2)) = a_data;
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
dhd_uint32_t dhd_sys_regread(dhd_sys_handle_t a_handle, dhd_uint32_t a_idx) {

  return (*(volatile uint32_t*) (R_DAVEHD_0_BASE + (a_idx << 2)));
}


/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_set_callback(dhd_sys_handle_t a_handle, dhd_irq_t a_callback) {

  switch ((dhd_uint32_t)a_handle) {
    #if defined(R_DAVEHD_0_BASE)
    case R_DAVEHD_0_BASE:
      loc_set_callback(&loc_dhd_sys_record_Unit0, a_callback);
      break;
    #endif

    #if defined(R_DAVEHD_1_BASE)
    case R_DAVEHD_1_BASE:
      loc_set_callback(&loc_dhd_sys_record_Unit1, a_callback);
      break;
    #endif

    default:
      /* a_handle comes from the gpu_config-struct, so this line should never be reached*/
      /* on a well configured system.*/
      break;
  }
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void* dhd_sys_get_callback(dhd_sys_handle_t a_handle) {

  void* result = R_NULL;
  
  switch ((dhd_uint32_t)a_handle) {
    #if defined(R_DAVEHD_0_BASE)
    case R_DAVEHD_0_BASE:
      result = loc_get_callback(&loc_dhd_sys_record_Unit0);
      break;
    #endif

    #if defined(R_DAVEHD_1_BASE)
    case R_DAVEHD_1_BASE:
      result = loc_get_callback(&loc_dhd_sys_record_Unit1);
      break;
    #endif

    default:
      result = R_NULL;
      break;
  }
  return result;
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_set_device(dhd_sys_handle_t a_handle, void *a_device) {

  switch ((dhd_uint32_t)a_handle) {
    #if defined(R_DAVEHD_0_BASE)
    case R_DAVEHD_0_BASE:
      loc_set_device(&loc_dhd_sys_record_Unit0, a_device);
      break;
    #endif

    #if defined(R_DAVEHD_1_BASE)
    case R_DAVEHD_1_BASE:
      loc_set_device(&loc_dhd_sys_record_Unit1, a_device);
      break;
    #endif

    default:
      /* a_handle comes from the gpu_config-struct, so this line should never be reached*/
      /* on a well configured system.*/
      break;
  }
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void* dhd_sys_get_device(dhd_sys_handle_t a_handle) {

  void* result = R_NULL;
  
  switch ((dhd_uint32_t)a_handle) {
    #if defined(R_DAVEHD_0_BASE)
    case R_DAVEHD_0_BASE:
      result = loc_get_device(&loc_dhd_sys_record_Unit0);
      break;
    #endif

    #if defined(R_DAVEHD_1_BASE)
    case R_DAVEHD_1_BASE:
      result = loc_get_device(&loc_dhd_sys_record_Unit1);
      break;
    #endif

    default:
      result = R_NULL;
      break;
  }
  return result;
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_sys_sleep(dhd_uint32_t a_time) {
}


/* $Coverage: function is not covered in automatic test suite as the only use is in DHD_PANIC, which also causes a process abort$*/
/*DHD_NO_COVERAGE_BEGIN*/

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_debug_out( const dhd_char_t *a_format, ... ) {  
  va_list args;  
  va_start(args, a_format);
  vprintf(a_format, args);
  va_end(args);
}

/*----------------------------------------------------------------------------------------------------------*/
/**/
void dhd_debug_panic( const dhd_char_t *a_message, const dhd_char_t *a_file, dhd_uint32_t a_line ) {
  dhd_debug_out("DHD_PANIC (ros platform handler): %s (in %s: %d) -> halting thread execution NOW!\n", a_message, a_file, a_line);
  for (;;) { 
  }
}

/*DHD_NO_COVERAGE_END*/
