/*
****************************************************************************
PROJECT : OSTM driver
FILE    : $Id: r_ostm_main.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for OSTM macro
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


#ifndef OSTM_MAIN_H_
#define OSTM_MAIN_H_

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
#ifndef OSTM_MAIN_GLOBALS_
#define DEF_EXTERN_ extern 
#else
#define DEF_EXTERN_
#endif


/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: R_OSTM_Dev_t

  Device instance specific data.
  All instance specific data is kept within that structure.

  Members:
  BaseAddr      - Basis address
  IsrCallback   -   Pointer to Isr callback functions
*/

typedef struct {
    uint32_t     BaseAddr;
    void (*IsrCallback[R_OSTM_INT_NUM])(void);
} R_OSTM_Dev_t;



/*******************************************************************************
  Section: Global Variables
*/

/*******************************************************************************
  Variable: R_OSTM_Dev

  Array for device instance specific data.
*/

DEF_EXTERN_ R_OSTM_Dev_t R_OSTM_Dev[R_OSTM_MACRO_NUM];



/*******************************************************************************
  Section: Global Functions
*/



/*******************************************************************************
  Function: R_OSTM_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_ostm_Error_t>
  
  Returns:
  void
*/

void R_OSTM_ErrorHandler(uint32_t Unit, r_ostm_Error_t Error);




#ifdef __cplusplus
}
#endif

#endif /* OSTM_MAIN_H_  */
