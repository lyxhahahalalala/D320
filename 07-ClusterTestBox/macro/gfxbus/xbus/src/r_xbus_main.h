/*
****************************************************************************
PROJECT : VLIB Macro Driver 
FILE    : $Id: r_xbus_main.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic XBUS driver
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


#ifndef R_XBUS_MAIN_H_
#define R_XBUS_MAIN_H_

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

#undef  DEF_EXTERN_
#ifndef XBUS_MAIN_GLOBALS_
#define DEF_EXTERN_ extern 
#else
#define DEF_EXTERN_
#endif

/*******************************************************************************
  Section: Global Types
*/


/*******************************************************************************
  Function: R_XBUS_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_xbus_Error_t>
  
  Returns:
  void
*/

void R_XBUS_ErrorHandler(uint32_t Unit, r_xbus_Error_t Error);

/*******************************************************************************
  Function: R_XBUS_CheckMacroParams
  
  Performs a range check for standard parameters. 
  Calls error handler in case something does not match. 
  
  For unused parameters fill in a 0 there  

 Parameters:
  Unit          - Instance number
  
  Returns:
  void 

*/
r_xbus_Error_t R_XBUS_CheckMacroParams(uint32_t Unit); 



#ifdef __cplusplus
}
#endif

#endif /* R_XBUS_MAIN_H_  */
