/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_tick_main.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic TICK driver
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


#ifndef TICK_MAIN_H_
#define TICK_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Main Module Internal Interface

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

#undef DEF_EXTERN_
#ifndef TICK_MAIN_GLOBALS_
#define DEF_EXTERN_ extern 
#else
#define DEF_EXTERN_
#endif

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: r_tick_Dev_t

  Device instance specific data.
  
  All instance specific data is kept within that structure.

  Members:
  TimeMS   - Time in ms
  Isr      - Pointer to Isr callback functions
*/

typedef struct {
    volatile uint32_t   TimeMS;
    void                (*Isr)(void);
} r_tick_Dev_t;


/*******************************************************************************
  Variable: r_tick_Dev

  Array for device instance specific data.
*/

DEF_EXTERN_ r_tick_Dev_t r_tick_Dev[R_TICK_MACRO_NUM];


#ifdef __cplusplus
}
#endif

#endif /* TICK_MAIN_H_  */
