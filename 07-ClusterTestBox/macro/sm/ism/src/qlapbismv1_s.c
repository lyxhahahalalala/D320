/*============================================================================
   PROJECT = ISM Type QLAPBISMV1                                              
   MODULE  = $Source$                                                         
   VERSION = $Revision: 4975 $                                                      
   DATE    = $Date: 2015-03-27 17:22:34 +0100 (Fr, 27 Mrz 2015) $                                                           
==============================================================================*/

/*============================================================================*/
/* C O P Y R I G H T                                                          */
/*============================================================================*/
/* Copyright (c) 2002 by NEC Electronics (EUROPE) GmbH. All rights reserved.  */
/* Kanzlerstrasse 2                                                           */
/* D-40472 Duesseldorf                                                        */
/*============================================================================*/
/* Purpose: ISM Application Level Parameter Sets */

/* Warranty Disclaimer */

/* Because the Product(s) is licensed free of charge, there is no warranty    */
/* of any kind whatsoever and expressly disclaimed and excluded by NEC,       */
/* either expressed or implied, including but not limited to those for        */
/* non-infringement of intellectual property, merchantability and/or          */
/* fitness for the particular purpose.                                        */
/* NEC shall not have any obligation to maintain, service or provide bug      */
/* fixes for the supplied Product(s) and/or the Application.                  */

/* Each User is solely responsible for determining the appropriateness of     */
/* using the Product(s) and assumes all risks associated with its exercise    */
/* of rights under this Agreement, including, but not limited to the risks    */
/* and costs of program errors, compliance with applicable laws, damage to    */
/* or loss of data, programs or equipment, and unavailability or              */
/* interruption of operations.                                                */

/* Limitation of Liability */

/* In no event shall NEC be liable to the User for any incidental,            */
/* consequential, indirect, or punitive damage (including but not limited     */
/* to lost profits) regardless of whether such liability is based on breach   */
/* of contract, tort, strict liability, breach of warranties, failure of      */
/* essential purpose or otherwise and even if advised of the possibility of   */
/* such damages. NEC shall not be liable for any services or products         */
/* provided by third party vendors, developers or consultants identified or   */
/* referred to the User by NEC in connection with the Product(s) and/or the   */
/* Application.                                                               */

/*============================================================================
   Environment: Devices:          All featuring QLAPBISMV1
                Assembler:        GHS MULTI 2000
                C-Compiler:       GHS MULTI 2000
                Linker:           GHS MULTI 2000
                Debugger:         GHS MULTI 2000
==============================================================================*/

#include <stddef.h>
// $$$: TODO cleanup !!!!!!
#include "r_typedefs.h"


#define  DRIVER_LOCAL
#define  APPLICATION_INTERFACE

#include "r_ism_api.h"
#include "r_config_ism.h"

#include "qlapbismv1.h"


/*============================================================================


          PWM Tables


==============================================================================*/

/* Default PWM Table: SINE on CVP, COSINE on CHP */

const struct rism_pwmstd_t R_ISM_A_STDPWMTABLE1 = {//view

  /*  CVP     CHP   CQI   IVR IHR */
{
{{{ 0x000L, 0x3FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 0 */
  { 0x032L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x3L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x3L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x3L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x3L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x3L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x3L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x3L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x3L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x3L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x3L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x3L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x3L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x3L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x3L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x3L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x3L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x3L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x3L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 0 */
  { 0x3FDL, 0x032L, 0x2L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x2L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x2L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x2L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x2L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x2L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x2L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x2L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x2L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x2L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x2L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x2L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x2L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x2L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x2L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x2L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x2L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x2L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x2L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x2L, 1L, 1L, 0L },

  { 0x000L, 0x3FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 0 */
  { 0x032L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x1L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x1L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x1L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x1L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x1L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x1L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x1L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x1L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x1L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x1L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x1L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x1L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x1L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 0 */
  { 0x3FDL, 0x032L, 0x0L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x0L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x0L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x0L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x0L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x0L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x0L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x0L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x0L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x0L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x0L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x0L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x0L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x0L, 1L, 1L, 0L } },

{ { 0x000L, 0x3FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 0 */
  { 0x032L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x3L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x3L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x3L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x3L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x3L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x3L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x3L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x3L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x3L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x3L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x3L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x3L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x3L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x3L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x3L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x3L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x3L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x3L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 0 */
  { 0x3FDL, 0x032L, 0x2L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x2L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x2L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x2L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x2L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x2L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x2L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x2L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x2L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x2L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x2L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x2L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x2L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x2L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x2L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x2L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x2L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x2L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x2L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x2L, 1L, 1L, 0L },

  { 0x000L, 0x3FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 0 */
  { 0x032L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x1L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x1L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x1L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x1L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x1L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x1L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x1L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x1L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x1L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x1L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x1L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x1L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x1L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 0 */
  { 0x3FDL, 0x032L, 0x0L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x0L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x0L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x0L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x0L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x0L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x0L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x0L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x0L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x0L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x0L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x0L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x0L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x0L, 1L, 1L, 0L } } },

{{{ 0x000L, 0x3FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 1 */
  { 0x032L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x3L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x3L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x3L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x3L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x3L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x3L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x3L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x3L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x3L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x3L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x3L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x3L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x3L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x3L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x3L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x3L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x3L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x3L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 1 */
  { 0x3FDL, 0x032L, 0x2L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x2L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x2L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x2L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x2L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x2L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x2L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x2L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x2L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x2L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x2L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x2L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x2L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x2L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x2L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x2L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x2L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x2L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x2L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x2L, 1L, 1L, 0L },

  { 0x000L, 0x3FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 1 */
  { 0x032L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x1L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x1L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x1L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x1L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x1L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x1L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x1L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x1L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x1L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x1L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x1L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x1L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x1L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 1 */
  { 0x3FDL, 0x032L, 0x0L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x0L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x0L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x0L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x0L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x0L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x0L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x0L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x0L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x0L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x0L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x0L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x0L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x0L, 1L, 1L, 0L } },

{ { 0x000L, 0x3FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 1 */
  { 0x032L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x3L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x3L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x3L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x3L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x3L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x3L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x3L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x3L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x3L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x3L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x3L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x3L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x3L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x3L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x3L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x3L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x3L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x3L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 1 */
  { 0x3FDL, 0x032L, 0x2L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x2L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x2L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x2L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x2L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x2L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x2L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x2L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x2L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x2L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x2L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x2L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x2L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x2L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x2L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x2L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x2L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x2L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x2L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x2L, 1L, 1L, 0L },

  { 0x000L, 0x3FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 1 */
  { 0x032L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x1L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x1L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x1L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x1L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x1L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x1L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x1L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x1L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x1L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x1L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x1L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x1L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x1L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 1 */
  { 0x3FDL, 0x032L, 0x0L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x0L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x0L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x0L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x0L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x0L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x0L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x0L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x0L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x0L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x0L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x0L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x0L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x0L, 1L, 1L, 0L } } } }
};

/* CONTI pattern PWM Table: adjusted SINE on CVP, COSINE on CHP */

const struct rism_pwmstd_t R_ISM_A_STDPWMTABLE2 = {

  /*  CVP     CHP   CQI   IVR IHR */
{
{{{ 0x010L, 0x3FEL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 0 */
  { 0x03DL, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x068L, 0x3F9L, 0x3L, 1L, 1L, 0L },
  { 0x08FL, 0x3F4L, 0x3L, 1L, 1L, 0L },
  { 0x0B4L, 0x3EEL, 0x3L, 1L, 1L, 0L },
  { 0x0D7L, 0x3E7L, 0x3L, 1L, 1L, 0L },
  { 0x0FAL, 0x3DFL, 0x3L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x3L, 1L, 1L, 0L },
  { 0x13EL, 0x3CBL, 0x3L, 1L, 1L, 0L },
  { 0x160L, 0x3BFL, 0x3L, 1L, 1L, 0L },
  { 0x184L, 0x3B2L, 0x3L, 1L, 1L, 0L },
  { 0x1A9L, 0x3A2L, 0x3L, 1L, 1L, 0L },
  { 0x1CFL, 0x38FL, 0x3L, 1L, 1L, 0L },
  { 0x1F8L, 0x379L, 0x3L, 1L, 1L, 0L },
  { 0x224L, 0x35FL, 0x3L, 1L, 1L, 0L },
  { 0x252L, 0x340L, 0x3L, 1L, 1L, 0L },
  { 0x283L, 0x31BL, 0x3L, 1L, 1L, 0L },
  { 0x2B5L, 0x2EFL, 0x3L, 1L, 1L, 0L },
  { 0x2EAL, 0x2BBL, 0x3L, 1L, 1L, 0L },
  { 0x31FL, 0x27EL, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x380L, 0x1ECL, 0x3L, 1L, 1L, 0L },
  { 0x3A8L, 0x19BL, 0x3L, 1L, 1L, 0L },
  { 0x3C8L, 0x149L, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0FAL, 0x3L, 1L, 1L, 0L },
  { 0x3EFL, 0x0B2L, 0x3L, 1L, 1L, 0L },
  { 0x3F8L, 0x073L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x03CL, 0x3L, 1L, 1L, 0L },
  { 0x3FFL, 0x00CL, 0x3L, 1L, 1L, 0L },
  { 0x3FEL, 0x01DL, 0x3L, 1L, 1L, 0L },
  { 0x3FCL, 0x040L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x059L, 0x3L, 1L, 1L, 0L },

  { 0x3F7L, 0x076L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 0 */
  { 0x3F3L, 0x091L, 0x2L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AAL, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C2L, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D8L, 0x2L, 1L, 1L, 0L },
  { 0x3E2L, 0x0EEL, 0x2L, 1L, 1L, 0L },
  { 0x3DCL, 0x103L, 0x2L, 1L, 1L, 0L },
  { 0x3D7L, 0x116L, 0x2L, 1L, 1L, 0L },
  { 0x3D1L, 0x12BL, 0x2L, 1L, 1L, 0L },
  { 0x3CAL, 0x13FL, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x155L, 0x2L, 1L, 1L, 0L },
  { 0x3BBL, 0x16BL, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x184L, 0x2L, 1L, 1L, 0L },
  { 0x3A6L, 0x19EL, 0x2L, 1L, 1L, 0L },
  { 0x399L, 0x1BBL, 0x2L, 1L, 1L, 0L },
  { 0x389L, 0x1DCL, 0x2L, 1L, 1L, 0L },
  { 0x376L, 0x200L, 0x2L, 1L, 1L, 0L },
  { 0x35FL, 0x227L, 0x2L, 1L, 1L, 0L },
  { 0x343L, 0x252L, 0x2L, 1L, 1L, 0L },
  { 0x322L, 0x280L, 0x2L, 1L, 1L, 0L },
  { 0x2FAL, 0x2B0L, 0x2L, 1L, 1L, 0L },
  { 0x2CBL, 0x2E1L, 0x2L, 1L, 1L, 0L },
  { 0x294L, 0x313L, 0x2L, 1L, 1L, 0L },
  { 0x254L, 0x345L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x373L, 0x2L, 1L, 1L, 0L },
  { 0x1C1L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x172L, 0x3BDL, 0x2L, 1L, 1L, 0L },
  { 0x123L, 0x3D7L, 0x2L, 1L, 1L, 0L },
  { 0x0D9L, 0x3E9L, 0x2L, 1L, 1L, 0L },
  { 0x095L, 0x3F4L, 0x2L, 1L, 1L, 0L },
  { 0x058L, 0x3FBL, 0x2L, 1L, 1L, 0L },
  { 0x020L, 0x3FEL, 0x2L, 1L, 1L, 0L },

  { 0x010L, 0x3FEL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 0 */
  { 0x03DL, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x068L, 0x3F9L, 0x1L, 1L, 1L, 0L },
  { 0x08FL, 0x3F4L, 0x1L, 1L, 1L, 0L },
  { 0x0B4L, 0x3EEL, 0x1L, 1L, 1L, 0L },
  { 0x0D7L, 0x3E7L, 0x1L, 1L, 1L, 0L },
  { 0x0FAL, 0x3DFL, 0x1L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x1L, 1L, 1L, 0L },
  { 0x13EL, 0x3CBL, 0x1L, 1L, 1L, 0L },
  { 0x160L, 0x3BFL, 0x1L, 1L, 1L, 0L },
  { 0x184L, 0x3B2L, 0x1L, 1L, 1L, 0L },
  { 0x1A9L, 0x3A2L, 0x1L, 1L, 1L, 0L },
  { 0x1CFL, 0x38FL, 0x1L, 1L, 1L, 0L },
  { 0x1F8L, 0x379L, 0x1L, 1L, 1L, 0L },
  { 0x224L, 0x35FL, 0x1L, 1L, 1L, 0L },
  { 0x252L, 0x340L, 0x1L, 1L, 1L, 0L },
  { 0x283L, 0x31BL, 0x1L, 1L, 1L, 0L },
  { 0x2B5L, 0x2EFL, 0x1L, 1L, 1L, 0L },
  { 0x2EAL, 0x2BBL, 0x1L, 1L, 1L, 0L },
  { 0x31FL, 0x27EL, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x380L, 0x1ECL, 0x1L, 1L, 1L, 0L },
  { 0x3A8L, 0x19BL, 0x1L, 1L, 1L, 0L },
  { 0x3C8L, 0x149L, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0FAL, 0x1L, 1L, 1L, 0L },
  { 0x3EFL, 0x0B2L, 0x1L, 1L, 1L, 0L },
  { 0x3F8L, 0x073L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x03CL, 0x1L, 1L, 1L, 0L },
  { 0x3FFL, 0x00CL, 0x1L, 1L, 1L, 0L },
  { 0x3FEL, 0x01DL, 0x1L, 1L, 1L, 0L },
  { 0x3FCL, 0x040L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x05EL, 0x1L, 1L, 1L, 0L },

  { 0x3F7L, 0x076L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 0 */
  { 0x3F3L, 0x091L, 0x0L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AAL, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C2L, 0x0L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D8L, 0x0L, 1L, 1L, 0L },
  { 0x3E2L, 0x0EEL, 0x0L, 1L, 1L, 0L },
  { 0x3DCL, 0x103L, 0x0L, 1L, 1L, 0L },
  { 0x3D7L, 0x116L, 0x0L, 1L, 1L, 0L },
  { 0x3D1L, 0x12BL, 0x0L, 1L, 1L, 0L },
  { 0x3CAL, 0x13FL, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x155L, 0x0L, 1L, 1L, 0L },
  { 0x3BBL, 0x16BL, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x184L, 0x0L, 1L, 1L, 0L },
  { 0x3A6L, 0x19EL, 0x0L, 1L, 1L, 0L },
  { 0x399L, 0x1BBL, 0x0L, 1L, 1L, 0L },
  { 0x389L, 0x1DCL, 0x0L, 1L, 1L, 0L },
  { 0x376L, 0x200L, 0x0L, 1L, 1L, 0L },
  { 0x35FL, 0x227L, 0x0L, 1L, 1L, 0L },
  { 0x343L, 0x252L, 0x0L, 1L, 1L, 0L },
  { 0x322L, 0x280L, 0x0L, 1L, 1L, 0L },
  { 0x2FAL, 0x2B0L, 0x0L, 1L, 1L, 0L },
  { 0x2CBL, 0x2E1L, 0x0L, 1L, 1L, 0L },
  { 0x294L, 0x313L, 0x0L, 1L, 1L, 0L },
  { 0x254L, 0x345L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x373L, 0x0L, 1L, 1L, 0L },
  { 0x1C1L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x172L, 0x3BDL, 0x0L, 1L, 1L, 0L },
  { 0x123L, 0x3D7L, 0x0L, 1L, 1L, 0L },
  { 0x0D9L, 0x3E9L, 0x0L, 1L, 1L, 0L },
  { 0x095L, 0x3F4L, 0x0L, 1L, 1L, 0L },
  { 0x058L, 0x3FBL, 0x0L, 1L, 1L, 0L },
  { 0x020L, 0x3FEL, 0x0L, 1L, 1L, 0L } },

{ { 0x170L, 0x3B9L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 0 */
  { 0x14FL, 0x3C5L, 0x3L, 1L, 1L, 0L },
  { 0x12DL, 0x3D0L, 0x3L, 1L, 1L, 0L },
  { 0x10AL, 0x3DAL, 0x3L, 1L, 1L, 0L },
  { 0x0E6L, 0x3E3L, 0x3L, 1L, 1L, 0L },
  { 0x0C0L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x097L, 0x3F2L, 0x3L, 1L, 1L, 0L },
  { 0x06AL, 0x3F8L, 0x3L, 1L, 1L, 0L },
  { 0x038L, 0x3FCL, 0x3L, 1L, 1L, 0L },
  { 0x000L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x03DL, 0x3FBL, 0x3L, 1L, 1L, 0L },
  { 0x084L, 0x3F4L, 0x3L, 1L, 1L, 0L },
  { 0x0D2L, 0x3E6L, 0x3L, 1L, 1L, 0L },
  { 0x126L, 0x3D0L, 0x3L, 1L, 1L, 0L },
  { 0x17CL, 0x3B2L, 0x3L, 1L, 1L, 0L },
  { 0x1D0L, 0x38CL, 0x3L, 1L, 1L, 0L },
  { 0x21FL, 0x35FL, 0x3L, 1L, 1L, 0L },
  { 0x267L, 0x32DL, 0x3L, 1L, 1L, 0L },
  { 0x2A6L, 0x2FAL, 0x3L, 1L, 1L, 0L },
  { 0x2DCL, 0x2C6L, 0x3L, 1L, 1L, 0L },
  { 0x309L, 0x295L, 0x3L, 1L, 1L, 0L },
  { 0x32FL, 0x267L, 0x3L, 1L, 1L, 0L },
  { 0x34FL, 0x23AL, 0x3L, 1L, 1L, 0L },
  { 0x36AL, 0x211L, 0x3L, 1L, 1L, 0L },
  { 0x37FL, 0x1ECL, 0x3L, 1L, 1L, 0L },
  { 0x392L, 0x1C9L, 0x3L, 1L, 1L, 0L },
  { 0x3A1L, 0x1AAL, 0x3L, 1L, 1L, 0L },
  { 0x3ADL, 0x18DL, 0x3L, 1L, 1L, 0L },
  { 0x3B8L, 0x174L, 0x3L, 1L, 1L, 0L },
  { 0x3C1L, 0x15BL, 0x3L, 1L, 1L, 0L },
  { 0x3C9L, 0x145L, 0x3L, 1L, 1L, 0L },
  { 0x3D0L, 0x12FL, 0x3L, 1L, 1L, 0L },

  { 0x3D6L, 0x11BL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 0 */
  { 0x3DCL, 0x107L, 0x2L, 1L, 1L, 0L },
  { 0x3E1L, 0x0F2L, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0DEL, 0x2L, 1L, 1L, 0L },
  { 0x3EAL, 0x0C9L, 0x2L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B2L, 0x2L, 1L, 1L, 0L },
  { 0x3F2L, 0x09BL, 0x2L, 1L, 1L, 0L },
  { 0x3F6L, 0x081L, 0x2L, 1L, 1L, 0L },
  { 0x3F9L, 0x065L, 0x2L, 1L, 1L, 0L },
  { 0x3FBL, 0x047L, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x025L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x002L, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x02FL, 0x2L, 1L, 1L, 0L },
  { 0x3F9L, 0x062L, 0x2L, 1L, 1L, 0L },
  { 0x3F1L, 0x09BL, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D9L, 0x2L, 1L, 1L, 0L },
  { 0x3D4L, 0x11BL, 0x2L, 1L, 1L, 0L },
  { 0x3BDL, 0x162L, 0x2L, 1L, 1L, 0L },
  { 0x39FL, 0x1AAL, 0x2L, 1L, 1L, 0L },
  { 0x37BL, 0x1F1L, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x235L, 0x2L, 1L, 1L, 0L },
  { 0x325L, 0x273L, 0x2L, 1L, 1L, 0L },
  { 0x2F7L, 0x2ABL, 0x2L, 1L, 1L, 0L },
  { 0x2C8L, 0x2DBL, 0x2L, 1L, 1L, 0L },
  { 0x29BL, 0x305L, 0x2L, 1L, 1L, 0L },
  { 0x26FL, 0x329L, 0x2L, 1L, 1L, 0L },
  { 0x244L, 0x349L, 0x2L, 1L, 1L, 0L },
  { 0x21BL, 0x363L, 0x2L, 1L, 1L, 0L },
  { 0x1F5L, 0x37AL, 0x2L, 1L, 1L, 0L },
  { 0x1D0L, 0x38EL, 0x2L, 1L, 1L, 0L },
  { 0x1ADL, 0x39FL, 0x2L, 1L, 1L, 0L },
  { 0x18BL, 0x3AEL, 0x2L, 1L, 1L, 0L },

  { 0x169L, 0x3BCL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 0 */
  { 0x148L, 0x3C8L, 0x1L, 1L, 1L, 0L },
  { 0x126L, 0x3D3L, 0x1L, 1L, 1L, 0L },
  { 0x103L, 0x3DCL, 0x1L, 1L, 1L, 0L },
  { 0x0DFL, 0x3E5L, 0x1L, 1L, 1L, 0L },
  { 0x0B9L, 0x3EDL, 0x1L, 1L, 1L, 0L },
  { 0x090L, 0x3F4L, 0x1L, 1L, 1L, 0L },
  { 0x063L, 0x3F9L, 0x1L, 1L, 1L, 0L },
  { 0x033L, 0x3FCL, 0x1L, 1L, 1L, 0L },
  { 0x003L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x040L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x084L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x0CFL, 0x3E6L, 0x1L, 1L, 1L, 0L },
  { 0x120L, 0x3D1L, 0x1L, 1L, 1L, 0L },
  { 0x173L, 0x3B5L, 0x1L, 1L, 1L, 0L },
  { 0x1C5L, 0x390L, 0x1L, 1L, 1L, 0L },
  { 0x213L, 0x365L, 0x1L, 1L, 1L, 0L },
  { 0x25BL, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x29BL, 0x303L, 0x1L, 1L, 1L, 0L },
  { 0x2D2L, 0x2D0L, 0x1L, 1L, 1L, 0L },
  { 0x300L, 0x2A0L, 0x1L, 1L, 1L, 0L },
  { 0x327L, 0x271L, 0x1L, 1L, 1L, 0L },
  { 0x348L, 0x244L, 0x1L, 1L, 1L, 0L },
  { 0x364L, 0x21AL, 0x1L, 1L, 1L, 0L },
  { 0x37BL, 0x1F3L, 0x1L, 1L, 1L, 0L },
  { 0x38EL, 0x1D0L, 0x1L, 1L, 1L, 0L },
  { 0x39EL, 0x1B0L, 0x1L, 1L, 1L, 0L },
  { 0x3ABL, 0x193L, 0x1L, 1L, 1L, 0L },
  { 0x3B6L, 0x178L, 0x1L, 1L, 1L, 0L },
  { 0x3C0L, 0x15FL, 0x1L, 1L, 1L, 0L },
  { 0x3C8L, 0x148L, 0x1L, 1L, 1L, 0L },
  { 0x3CFL, 0x133L, 0x1L, 1L, 1L, 0L },

  { 0x3D5L, 0x11EL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 0 */
  { 0x3DBL, 0x10AL, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F5L, 0x0L, 1L, 1L, 0L },
  { 0x3E5L, 0x0E1L, 0x0L, 1L, 1L, 0L },
  { 0x3EAL, 0x0CBL, 0x0L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B3L, 0x0L, 1L, 1L, 0L },
  { 0x3F2L, 0x09BL, 0x0L, 1L, 1L, 0L },
  { 0x3F6L, 0x080L, 0x0L, 1L, 1L, 0L },
  { 0x3F9L, 0x063L, 0x0L, 1L, 1L, 0L },
  { 0x3FBL, 0x044L, 0x0L, 1L, 1L, 0L },
  { 0x3FDL, 0x021L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x006L, 0x0L, 1L, 1L, 0L },
  { 0x3FCL, 0x033L, 0x0L, 1L, 1L, 0L },
  { 0x3F8L, 0x067L, 0x0L, 1L, 1L, 0L },
  { 0x3F0L, 0x0A0L, 0x0L, 1L, 1L, 0L },
  { 0x3E4L, 0x0DFL, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x122L, 0x0L, 1L, 1L, 0L },
  { 0x3BAL, 0x168L, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1AFL, 0x0L, 1L, 1L, 0L },
  { 0x378L, 0x1F6L, 0x0L, 1L, 1L, 0L },
  { 0x34EL, 0x239L, 0x0L, 1L, 1L, 0L },
  { 0x322L, 0x276L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2ACL, 0x0L, 1L, 1L, 0L },
  { 0x2C7L, 0x2DCL, 0x0L, 1L, 1L, 0L },
  { 0x29BL, 0x305L, 0x0L, 1L, 1L, 0L },
  { 0x270L, 0x328L, 0x0L, 1L, 1L, 0L },
  { 0x246L, 0x347L, 0x0L, 1L, 1L, 0L },
  { 0x21EL, 0x362L, 0x0L, 1L, 1L, 0L },
  { 0x1F8L, 0x379L, 0x0L, 1L, 1L, 0L },
  { 0x1D4L, 0x38CL, 0x0L, 1L, 1L, 0L },
  { 0x1B1L, 0x39DL, 0x0L, 1L, 1L, 0L },
  { 0x190L, 0x3ACL, 0x0L, 1L, 1L, 0L } } },

{{{ 0x010L, 0x3FEL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 1 */
  { 0x03DL, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x068L, 0x3F9L, 0x3L, 1L, 1L, 0L },
  { 0x08FL, 0x3F4L, 0x3L, 1L, 1L, 0L },
  { 0x0B4L, 0x3EEL, 0x3L, 1L, 1L, 0L },
  { 0x0D7L, 0x3E7L, 0x3L, 1L, 1L, 0L },
  { 0x0FAL, 0x3DFL, 0x3L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x3L, 1L, 1L, 0L },
  { 0x13EL, 0x3CBL, 0x3L, 1L, 1L, 0L },
  { 0x160L, 0x3BFL, 0x3L, 1L, 1L, 0L },
  { 0x184L, 0x3B2L, 0x3L, 1L, 1L, 0L },
  { 0x1A9L, 0x3A2L, 0x3L, 1L, 1L, 0L },
  { 0x1CFL, 0x38FL, 0x3L, 1L, 1L, 0L },
  { 0x1F8L, 0x379L, 0x3L, 1L, 1L, 0L },
  { 0x224L, 0x35FL, 0x3L, 1L, 1L, 0L },
  { 0x252L, 0x340L, 0x3L, 1L, 1L, 0L },
  { 0x283L, 0x31BL, 0x3L, 1L, 1L, 0L },
  { 0x2B5L, 0x2EFL, 0x3L, 1L, 1L, 0L },
  { 0x2EAL, 0x2BBL, 0x3L, 1L, 1L, 0L },
  { 0x31FL, 0x27EL, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x380L, 0x1ECL, 0x3L, 1L, 1L, 0L },
  { 0x3A8L, 0x19BL, 0x3L, 1L, 1L, 0L },
  { 0x3C8L, 0x149L, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0FAL, 0x3L, 1L, 1L, 0L },
  { 0x3EFL, 0x0B2L, 0x3L, 1L, 1L, 0L },
  { 0x3F8L, 0x073L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x03CL, 0x3L, 1L, 1L, 0L },
  { 0x3FFL, 0x00CL, 0x3L, 1L, 1L, 0L },
  { 0x3FEL, 0x01DL, 0x3L, 1L, 1L, 0L },
  { 0x3FCL, 0x040L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x059L, 0x3L, 1L, 1L, 0L },

  { 0x3F7L, 0x076L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 1 */
  { 0x3F3L, 0x091L, 0x2L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AAL, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C2L, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D8L, 0x2L, 1L, 1L, 0L },
  { 0x3E2L, 0x0EEL, 0x2L, 1L, 1L, 0L },
  { 0x3DCL, 0x103L, 0x2L, 1L, 1L, 0L },
  { 0x3D7L, 0x116L, 0x2L, 1L, 1L, 0L },
  { 0x3D1L, 0x12BL, 0x2L, 1L, 1L, 0L },
  { 0x3CAL, 0x13FL, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x155L, 0x2L, 1L, 1L, 0L },
  { 0x3BBL, 0x16BL, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x184L, 0x2L, 1L, 1L, 0L },
  { 0x3A6L, 0x19EL, 0x2L, 1L, 1L, 0L },
  { 0x399L, 0x1BBL, 0x2L, 1L, 1L, 0L },
  { 0x389L, 0x1DCL, 0x2L, 1L, 1L, 0L },
  { 0x376L, 0x200L, 0x2L, 1L, 1L, 0L },
  { 0x35FL, 0x227L, 0x2L, 1L, 1L, 0L },
  { 0x343L, 0x252L, 0x2L, 1L, 1L, 0L },
  { 0x322L, 0x280L, 0x2L, 1L, 1L, 0L },
  { 0x2FAL, 0x2B0L, 0x2L, 1L, 1L, 0L },
  { 0x2CBL, 0x2E1L, 0x2L, 1L, 1L, 0L },
  { 0x294L, 0x313L, 0x2L, 1L, 1L, 0L },
  { 0x254L, 0x345L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x373L, 0x2L, 1L, 1L, 0L },
  { 0x1C1L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x172L, 0x3BDL, 0x2L, 1L, 1L, 0L },
  { 0x123L, 0x3D7L, 0x2L, 1L, 1L, 0L },
  { 0x0D9L, 0x3E9L, 0x2L, 1L, 1L, 0L },
  { 0x095L, 0x3F4L, 0x2L, 1L, 1L, 0L },
  { 0x058L, 0x3FBL, 0x2L, 1L, 1L, 0L },
  { 0x020L, 0x3FEL, 0x2L, 1L, 1L, 0L },

  { 0x010L, 0x3FEL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 1 */
  { 0x03DL, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x068L, 0x3F9L, 0x1L, 1L, 1L, 0L },
  { 0x08FL, 0x3F4L, 0x1L, 1L, 1L, 0L },
  { 0x0B4L, 0x3EEL, 0x1L, 1L, 1L, 0L },
  { 0x0D7L, 0x3E7L, 0x1L, 1L, 1L, 0L },
  { 0x0FAL, 0x3DFL, 0x1L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x1L, 1L, 1L, 0L },
  { 0x13EL, 0x3CBL, 0x1L, 1L, 1L, 0L },
  { 0x160L, 0x3BFL, 0x1L, 1L, 1L, 0L },
  { 0x184L, 0x3B2L, 0x1L, 1L, 1L, 0L },
  { 0x1A9L, 0x3A2L, 0x1L, 1L, 1L, 0L },
  { 0x1CFL, 0x38FL, 0x1L, 1L, 1L, 0L },
  { 0x1F8L, 0x379L, 0x1L, 1L, 1L, 0L },
  { 0x224L, 0x35FL, 0x1L, 1L, 1L, 0L },
  { 0x252L, 0x340L, 0x1L, 1L, 1L, 0L },
  { 0x283L, 0x31BL, 0x1L, 1L, 1L, 0L },
  { 0x2B5L, 0x2EFL, 0x1L, 1L, 1L, 0L },
  { 0x2EAL, 0x2BBL, 0x1L, 1L, 1L, 0L },
  { 0x31FL, 0x27EL, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x380L, 0x1ECL, 0x1L, 1L, 1L, 0L },
  { 0x3A8L, 0x19BL, 0x1L, 1L, 1L, 0L },
  { 0x3C8L, 0x149L, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0FAL, 0x1L, 1L, 1L, 0L },
  { 0x3EFL, 0x0B2L, 0x1L, 1L, 1L, 0L },
  { 0x3F8L, 0x073L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x03CL, 0x1L, 1L, 1L, 0L },
  { 0x3FFL, 0x00CL, 0x1L, 1L, 1L, 0L },
  { 0x3FEL, 0x01DL, 0x1L, 1L, 1L, 0L },
  { 0x3FCL, 0x040L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x05EL, 0x1L, 1L, 1L, 0L },

  { 0x3F7L, 0x076L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 1 */
  { 0x3F3L, 0x091L, 0x0L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AAL, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C2L, 0x0L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D8L, 0x0L, 1L, 1L, 0L },
  { 0x3E2L, 0x0EEL, 0x0L, 1L, 1L, 0L },
  { 0x3DCL, 0x103L, 0x0L, 1L, 1L, 0L },
  { 0x3D7L, 0x116L, 0x0L, 1L, 1L, 0L },
  { 0x3D1L, 0x12BL, 0x0L, 1L, 1L, 0L },
  { 0x3CAL, 0x13FL, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x155L, 0x0L, 1L, 1L, 0L },
  { 0x3BBL, 0x16BL, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x184L, 0x0L, 1L, 1L, 0L },
  { 0x3A6L, 0x19EL, 0x0L, 1L, 1L, 0L },
  { 0x399L, 0x1BBL, 0x0L, 1L, 1L, 0L },
  { 0x389L, 0x1DCL, 0x0L, 1L, 1L, 0L },
  { 0x376L, 0x200L, 0x0L, 1L, 1L, 0L },
  { 0x35FL, 0x227L, 0x0L, 1L, 1L, 0L },
  { 0x343L, 0x252L, 0x0L, 1L, 1L, 0L },
  { 0x322L, 0x280L, 0x0L, 1L, 1L, 0L },
  { 0x2FAL, 0x2B0L, 0x0L, 1L, 1L, 0L },
  { 0x2CBL, 0x2E1L, 0x0L, 1L, 1L, 0L },
  { 0x294L, 0x313L, 0x0L, 1L, 1L, 0L },
  { 0x254L, 0x345L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x373L, 0x0L, 1L, 1L, 0L },
  { 0x1C1L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x172L, 0x3BDL, 0x0L, 1L, 1L, 0L },
  { 0x123L, 0x3D7L, 0x0L, 1L, 1L, 0L },
  { 0x0D9L, 0x3E9L, 0x0L, 1L, 1L, 0L },
  { 0x095L, 0x3F4L, 0x0L, 1L, 1L, 0L },
  { 0x058L, 0x3FBL, 0x0L, 1L, 1L, 0L },
  { 0x020L, 0x3FEL, 0x0L, 1L, 1L, 0L } },

{ { 0x170L, 0x3B9L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 1 */
  { 0x14FL, 0x3C5L, 0x3L, 1L, 1L, 0L },
  { 0x12DL, 0x3D0L, 0x3L, 1L, 1L, 0L },
  { 0x10AL, 0x3DAL, 0x3L, 1L, 1L, 0L },
  { 0x0E6L, 0x3E3L, 0x3L, 1L, 1L, 0L },
  { 0x0C0L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x097L, 0x3F2L, 0x3L, 1L, 1L, 0L },
  { 0x06AL, 0x3F8L, 0x3L, 1L, 1L, 0L },
  { 0x038L, 0x3FCL, 0x3L, 1L, 1L, 0L },
  { 0x000L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x03DL, 0x3FBL, 0x3L, 1L, 1L, 0L },
  { 0x084L, 0x3F4L, 0x3L, 1L, 1L, 0L },
  { 0x0D2L, 0x3E6L, 0x3L, 1L, 1L, 0L },
  { 0x126L, 0x3D0L, 0x3L, 1L, 1L, 0L },
  { 0x17CL, 0x3B2L, 0x3L, 1L, 1L, 0L },
  { 0x1D0L, 0x38CL, 0x3L, 1L, 1L, 0L },
  { 0x21FL, 0x35FL, 0x3L, 1L, 1L, 0L },
  { 0x267L, 0x32DL, 0x3L, 1L, 1L, 0L },
  { 0x2A6L, 0x2FAL, 0x3L, 1L, 1L, 0L },
  { 0x2DCL, 0x2C6L, 0x3L, 1L, 1L, 0L },
  { 0x309L, 0x295L, 0x3L, 1L, 1L, 0L },
  { 0x32FL, 0x267L, 0x3L, 1L, 1L, 0L },
  { 0x34FL, 0x23AL, 0x3L, 1L, 1L, 0L },
  { 0x36AL, 0x211L, 0x3L, 1L, 1L, 0L },
  { 0x37FL, 0x1ECL, 0x3L, 1L, 1L, 0L },
  { 0x392L, 0x1C9L, 0x3L, 1L, 1L, 0L },
  { 0x3A1L, 0x1AAL, 0x3L, 1L, 1L, 0L },
  { 0x3ADL, 0x18DL, 0x3L, 1L, 1L, 0L },
  { 0x3B8L, 0x174L, 0x3L, 1L, 1L, 0L },
  { 0x3C1L, 0x15BL, 0x3L, 1L, 1L, 0L },
  { 0x3C9L, 0x145L, 0x3L, 1L, 1L, 0L },
  { 0x3D0L, 0x12FL, 0x3L, 1L, 1L, 0L },

  { 0x3D6L, 0x11BL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 1 */
  { 0x3DCL, 0x107L, 0x2L, 1L, 1L, 0L },
  { 0x3E1L, 0x0F2L, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0DEL, 0x2L, 1L, 1L, 0L },
  { 0x3EAL, 0x0C9L, 0x2L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B2L, 0x2L, 1L, 1L, 0L },
  { 0x3F2L, 0x09BL, 0x2L, 1L, 1L, 0L },
  { 0x3F6L, 0x081L, 0x2L, 1L, 1L, 0L },
  { 0x3F9L, 0x065L, 0x2L, 1L, 1L, 0L },
  { 0x3FBL, 0x047L, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x025L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x002L, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x02FL, 0x2L, 1L, 1L, 0L },
  { 0x3F9L, 0x062L, 0x2L, 1L, 1L, 0L },
  { 0x3F1L, 0x09BL, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D9L, 0x2L, 1L, 1L, 0L },
  { 0x3D4L, 0x11BL, 0x2L, 1L, 1L, 0L },
  { 0x3BDL, 0x162L, 0x2L, 1L, 1L, 0L },
  { 0x39FL, 0x1AAL, 0x2L, 1L, 1L, 0L },
  { 0x37BL, 0x1F1L, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x235L, 0x2L, 1L, 1L, 0L },
  { 0x325L, 0x273L, 0x2L, 1L, 1L, 0L },
  { 0x2F7L, 0x2ABL, 0x2L, 1L, 1L, 0L },
  { 0x2C8L, 0x2DBL, 0x2L, 1L, 1L, 0L },
  { 0x29BL, 0x305L, 0x2L, 1L, 1L, 0L },
  { 0x26FL, 0x329L, 0x2L, 1L, 1L, 0L },
  { 0x244L, 0x349L, 0x2L, 1L, 1L, 0L },
  { 0x21BL, 0x363L, 0x2L, 1L, 1L, 0L },
  { 0x1F5L, 0x37AL, 0x2L, 1L, 1L, 0L },
  { 0x1D0L, 0x38EL, 0x2L, 1L, 1L, 0L },
  { 0x1ADL, 0x39FL, 0x2L, 1L, 1L, 0L },
  { 0x18BL, 0x3AEL, 0x2L, 1L, 1L, 0L },

  { 0x169L, 0x3BCL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 1 */
  { 0x148L, 0x3C8L, 0x1L, 1L, 1L, 0L },
  { 0x126L, 0x3D3L, 0x1L, 1L, 1L, 0L },
  { 0x103L, 0x3DCL, 0x1L, 1L, 1L, 0L },
  { 0x0DFL, 0x3E5L, 0x1L, 1L, 1L, 0L },
  { 0x0B9L, 0x3EDL, 0x1L, 1L, 1L, 0L },
  { 0x090L, 0x3F4L, 0x1L, 1L, 1L, 0L },
  { 0x063L, 0x3F9L, 0x1L, 1L, 1L, 0L },
  { 0x033L, 0x3FCL, 0x1L, 1L, 1L, 0L },
  { 0x003L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x040L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x084L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x0CFL, 0x3E6L, 0x1L, 1L, 1L, 0L },
  { 0x120L, 0x3D1L, 0x1L, 1L, 1L, 0L },
  { 0x173L, 0x3B5L, 0x1L, 1L, 1L, 0L },
  { 0x1C5L, 0x390L, 0x1L, 1L, 1L, 0L },
  { 0x213L, 0x365L, 0x1L, 1L, 1L, 0L },
  { 0x25BL, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x29BL, 0x303L, 0x1L, 1L, 1L, 0L },
  { 0x2D2L, 0x2D0L, 0x1L, 1L, 1L, 0L },
  { 0x300L, 0x2A0L, 0x1L, 1L, 1L, 0L },
  { 0x327L, 0x271L, 0x1L, 1L, 1L, 0L },
  { 0x348L, 0x244L, 0x1L, 1L, 1L, 0L },
  { 0x364L, 0x21AL, 0x1L, 1L, 1L, 0L },
  { 0x37BL, 0x1F3L, 0x1L, 1L, 1L, 0L },
  { 0x38EL, 0x1D0L, 0x1L, 1L, 1L, 0L },
  { 0x39EL, 0x1B0L, 0x1L, 1L, 1L, 0L },
  { 0x3ABL, 0x193L, 0x1L, 1L, 1L, 0L },
  { 0x3B6L, 0x178L, 0x1L, 1L, 1L, 0L },
  { 0x3C0L, 0x15FL, 0x1L, 1L, 1L, 0L },
  { 0x3C8L, 0x148L, 0x1L, 1L, 1L, 0L },
  { 0x3CFL, 0x133L, 0x1L, 1L, 1L, 0L },

  { 0x3D5L, 0x11EL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 1 */
  { 0x3DBL, 0x10AL, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F5L, 0x0L, 1L, 1L, 0L },
  { 0x3E5L, 0x0E1L, 0x0L, 1L, 1L, 0L },
  { 0x3EAL, 0x0CBL, 0x0L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B3L, 0x0L, 1L, 1L, 0L },
  { 0x3F2L, 0x09BL, 0x0L, 1L, 1L, 0L },
  { 0x3F6L, 0x080L, 0x0L, 1L, 1L, 0L },
  { 0x3F9L, 0x063L, 0x0L, 1L, 1L, 0L },
  { 0x3FBL, 0x044L, 0x0L, 1L, 1L, 0L },
  { 0x3FDL, 0x021L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x006L, 0x0L, 1L, 1L, 0L },
  { 0x3FCL, 0x033L, 0x0L, 1L, 1L, 0L },
  { 0x3F8L, 0x067L, 0x0L, 1L, 1L, 0L },
  { 0x3F0L, 0x0A0L, 0x0L, 1L, 1L, 0L },
  { 0x3E4L, 0x0DFL, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x122L, 0x0L, 1L, 1L, 0L },
  { 0x3BAL, 0x168L, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1AFL, 0x0L, 1L, 1L, 0L },
  { 0x378L, 0x1F6L, 0x0L, 1L, 1L, 0L },
  { 0x34EL, 0x239L, 0x0L, 1L, 1L, 0L },
  { 0x322L, 0x276L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2ACL, 0x0L, 1L, 1L, 0L },
  { 0x2C7L, 0x2DCL, 0x0L, 1L, 1L, 0L },
  { 0x29BL, 0x305L, 0x0L, 1L, 1L, 0L },
  { 0x270L, 0x328L, 0x0L, 1L, 1L, 0L },
  { 0x246L, 0x347L, 0x0L, 1L, 1L, 0L },
  { 0x21EL, 0x362L, 0x0L, 1L, 1L, 0L },
  { 0x1F8L, 0x379L, 0x0L, 1L, 1L, 0L },
  { 0x1D4L, 0x38CL, 0x0L, 1L, 1L, 0L },
  { 0x1B1L, 0x39DL, 0x0L, 1L, 1L, 0L },
  { 0x190L, 0x3ACL, 0x0L, 1L, 1L, 0L } } } }
};

/* Alternative CONTI PWM Table: adjusted COSINE on CVP, SINE on CHP */

const struct rism_pwmstd_t R_ISM_A_STDPWMTABLE3 = {

  /*  CVP     CHP   CQI   IVR IHR */
{
{{{ 0x3FEL, 0x010L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 0 */
  { 0x3FDL, 0x03DL, 0x3L, 1L, 1L, 0L },
  { 0x3F9L, 0x068L, 0x3L, 1L, 1L, 0L },
  { 0x3F4L, 0x08FL, 0x3L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B4L, 0x3L, 1L, 1L, 0L },
  { 0x3E7L, 0x0D7L, 0x3L, 1L, 1L, 0L },
  { 0x3DFL, 0x0FAL, 0x3L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x3L, 1L, 1L, 0L },
  { 0x3CBL, 0x13EL, 0x3L, 1L, 1L, 0L },
  { 0x3BFL, 0x160L, 0x3L, 1L, 1L, 0L },
  { 0x3B2L, 0x184L, 0x3L, 1L, 1L, 0L },
  { 0x3A2L, 0x1A9L, 0x3L, 1L, 1L, 0L },
  { 0x38FL, 0x1CFL, 0x3L, 1L, 1L, 0L },
  { 0x379L, 0x1F8L, 0x3L, 1L, 1L, 0L },
  { 0x35FL, 0x224L, 0x3L, 1L, 1L, 0L },
  { 0x340L, 0x252L, 0x3L, 1L, 1L, 0L },
  { 0x31BL, 0x283L, 0x3L, 1L, 1L, 0L },
  { 0x2EFL, 0x2B5L, 0x3L, 1L, 1L, 0L },
  { 0x2BBL, 0x2EAL, 0x3L, 1L, 1L, 0L },
  { 0x27EL, 0x31FL, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x1ECL, 0x380L, 0x3L, 1L, 1L, 0L },
  { 0x19BL, 0x3A8L, 0x3L, 1L, 1L, 0L },
  { 0x149L, 0x3C8L, 0x3L, 1L, 1L, 0L },
  { 0x0FAL, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x0B2L, 0x3EFL, 0x3L, 1L, 1L, 0L },
  { 0x073L, 0x3F8L, 0x3L, 1L, 1L, 0L },
  { 0x03CL, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x00CL, 0x3FFL, 0x3L, 1L, 1L, 0L },
  { 0x01DL, 0x3FEL, 0x3L, 1L, 1L, 0L },
  { 0x040L, 0x3FCL, 0x3L, 1L, 1L, 0L },
  { 0x059L, 0x3FAL, 0x3L, 1L, 1L, 0L },
                                        
  { 0x076L, 0x3F7L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 0 */
  { 0x091L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x0AAL, 0x3EFL, 0x2L, 1L, 1L, 0L },
  { 0x0C2L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x0D8L, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x0EEL, 0x3E2L, 0x2L, 1L, 1L, 0L },
  { 0x103L, 0x3DCL, 0x2L, 1L, 1L, 0L },
  { 0x116L, 0x3D7L, 0x2L, 1L, 1L, 0L },
  { 0x12BL, 0x3D1L, 0x2L, 1L, 1L, 0L },
  { 0x13FL, 0x3CAL, 0x2L, 1L, 1L, 0L },
  { 0x155L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x16BL, 0x3BBL, 0x2L, 1L, 1L, 0L },
  { 0x184L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x19EL, 0x3A6L, 0x2L, 1L, 1L, 0L },
  { 0x1BBL, 0x399L, 0x2L, 1L, 1L, 0L },
  { 0x1DCL, 0x389L, 0x2L, 1L, 1L, 0L },
  { 0x200L, 0x376L, 0x2L, 1L, 1L, 0L },
  { 0x227L, 0x35FL, 0x2L, 1L, 1L, 0L },
  { 0x252L, 0x343L, 0x2L, 1L, 1L, 0L },
  { 0x280L, 0x322L, 0x2L, 1L, 1L, 0L },
  { 0x2B0L, 0x2FAL, 0x2L, 1L, 1L, 0L },
  { 0x2E1L, 0x2CBL, 0x2L, 1L, 1L, 0L },
  { 0x313L, 0x294L, 0x2L, 1L, 1L, 0L },
  { 0x345L, 0x254L, 0x2L, 1L, 1L, 0L },
  { 0x373L, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1C1L, 0x2L, 1L, 1L, 0L },
  { 0x3BDL, 0x172L, 0x2L, 1L, 1L, 0L },
  { 0x3D7L, 0x123L, 0x2L, 1L, 1L, 0L },
  { 0x3E9L, 0x0D9L, 0x2L, 1L, 1L, 0L },
  { 0x3F4L, 0x095L, 0x2L, 1L, 1L, 0L },
  { 0x3FBL, 0x058L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x020L, 0x2L, 1L, 1L, 0L },
                                        
  { 0x3FEL, 0x010L, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 0 */
  { 0x3FDL, 0x03DL, 0x1L, 1L, 1L, 0L },
  { 0x3F9L, 0x068L, 0x1L, 1L, 1L, 0L },
  { 0x3F4L, 0x08FL, 0x1L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B4L, 0x1L, 1L, 1L, 0L },
  { 0x3E7L, 0x0D7L, 0x1L, 1L, 1L, 0L },
  { 0x3DFL, 0x0FAL, 0x1L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x1L, 1L, 1L, 0L },
  { 0x3CBL, 0x13EL, 0x1L, 1L, 1L, 0L },
  { 0x3BFL, 0x160L, 0x1L, 1L, 1L, 0L },
  { 0x3B2L, 0x184L, 0x1L, 1L, 1L, 0L },
  { 0x3A2L, 0x1A9L, 0x1L, 1L, 1L, 0L },
  { 0x38FL, 0x1CFL, 0x1L, 1L, 1L, 0L },
  { 0x379L, 0x1F8L, 0x1L, 1L, 1L, 0L },
  { 0x35FL, 0x224L, 0x1L, 1L, 1L, 0L },
  { 0x340L, 0x252L, 0x1L, 1L, 1L, 0L },
  { 0x31BL, 0x283L, 0x1L, 1L, 1L, 0L },
  { 0x2EFL, 0x2B5L, 0x1L, 1L, 1L, 0L },
  { 0x2BBL, 0x2EAL, 0x1L, 1L, 1L, 0L },
  { 0x27EL, 0x31FL, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x1ECL, 0x380L, 0x1L, 1L, 1L, 0L },
  { 0x19BL, 0x3A8L, 0x1L, 1L, 1L, 0L },
  { 0x149L, 0x3C8L, 0x1L, 1L, 1L, 0L },
  { 0x0FAL, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x0B2L, 0x3EFL, 0x1L, 1L, 1L, 0L },
  { 0x073L, 0x3F8L, 0x1L, 1L, 1L, 0L },
  { 0x03CL, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x00CL, 0x3FFL, 0x1L, 1L, 1L, 0L },
  { 0x01DL, 0x3FEL, 0x1L, 1L, 1L, 0L },
  { 0x040L, 0x3FCL, 0x1L, 1L, 1L, 0L },
  { 0x05EL, 0x3FAL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x076L, 0x3F7L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 0 */
  { 0x091L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x0AAL, 0x3EFL, 0x0L, 1L, 1L, 0L },
  { 0x0C2L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x0D8L, 0x3E6L, 0x0L, 1L, 1L, 0L },
  { 0x0EEL, 0x3E2L, 0x0L, 1L, 1L, 0L },
  { 0x103L, 0x3DCL, 0x0L, 1L, 1L, 0L },
  { 0x116L, 0x3D7L, 0x0L, 1L, 1L, 0L },
  { 0x12BL, 0x3D1L, 0x0L, 1L, 1L, 0L },
  { 0x13FL, 0x3CAL, 0x0L, 1L, 1L, 0L },
  { 0x155L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x16BL, 0x3BBL, 0x0L, 1L, 1L, 0L },
  { 0x184L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x19EL, 0x3A6L, 0x0L, 1L, 1L, 0L },
  { 0x1BBL, 0x399L, 0x0L, 1L, 1L, 0L },
  { 0x1DCL, 0x389L, 0x0L, 1L, 1L, 0L },
  { 0x200L, 0x376L, 0x0L, 1L, 1L, 0L },
  { 0x227L, 0x35FL, 0x0L, 1L, 1L, 0L },
  { 0x252L, 0x343L, 0x0L, 1L, 1L, 0L },
  { 0x280L, 0x322L, 0x0L, 1L, 1L, 0L },
  { 0x2B0L, 0x2FAL, 0x0L, 1L, 1L, 0L },
  { 0x2E1L, 0x2CBL, 0x0L, 1L, 1L, 0L },
  { 0x313L, 0x294L, 0x0L, 1L, 1L, 0L },
  { 0x345L, 0x254L, 0x0L, 1L, 1L, 0L },
  { 0x373L, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1C1L, 0x0L, 1L, 1L, 0L },
  { 0x3BDL, 0x172L, 0x0L, 1L, 1L, 0L },
  { 0x3D7L, 0x123L, 0x0L, 1L, 1L, 0L },
  { 0x3E9L, 0x0D9L, 0x0L, 1L, 1L, 0L },
  { 0x3F4L, 0x095L, 0x0L, 1L, 1L, 0L },
  { 0x3FBL, 0x058L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x020L, 0x0L, 1L, 1L, 0L } },
                                        
{ { 0x3B9L, 0x170L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 0 */
  { 0x3C5L, 0x14FL, 0x3L, 1L, 1L, 0L },
  { 0x3D0L, 0x12DL, 0x3L, 1L, 1L, 0L },
  { 0x3DAL, 0x10AL, 0x3L, 1L, 1L, 0L },
  { 0x3E3L, 0x0E6L, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C0L, 0x3L, 1L, 1L, 0L },
  { 0x3F2L, 0x097L, 0x3L, 1L, 1L, 0L },
  { 0x3F8L, 0x06AL, 0x3L, 1L, 1L, 0L },
  { 0x3FCL, 0x038L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x000L, 0x3L, 1L, 1L, 0L },
  { 0x3FBL, 0x03DL, 0x3L, 1L, 1L, 0L },
  { 0x3F4L, 0x084L, 0x3L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D2L, 0x3L, 1L, 1L, 0L },
  { 0x3D0L, 0x126L, 0x3L, 1L, 1L, 0L },
  { 0x3B2L, 0x17CL, 0x3L, 1L, 1L, 0L },
  { 0x38CL, 0x1D0L, 0x3L, 1L, 1L, 0L },
  { 0x35FL, 0x21FL, 0x3L, 1L, 1L, 0L },
  { 0x32DL, 0x267L, 0x3L, 1L, 1L, 0L },
  { 0x2FAL, 0x2A6L, 0x3L, 1L, 1L, 0L },
  { 0x2C6L, 0x2DCL, 0x3L, 1L, 1L, 0L },
  { 0x295L, 0x309L, 0x3L, 1L, 1L, 0L },
  { 0x267L, 0x32FL, 0x3L, 1L, 1L, 0L },
  { 0x23AL, 0x34FL, 0x3L, 1L, 1L, 0L },
  { 0x211L, 0x36AL, 0x3L, 1L, 1L, 0L },
  { 0x1ECL, 0x37FL, 0x3L, 1L, 1L, 0L },
  { 0x1C9L, 0x392L, 0x3L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A1L, 0x3L, 1L, 1L, 0L },
  { 0x18DL, 0x3ADL, 0x3L, 1L, 1L, 0L },
  { 0x174L, 0x3B8L, 0x3L, 1L, 1L, 0L },
  { 0x15BL, 0x3C1L, 0x3L, 1L, 1L, 0L },
  { 0x145L, 0x3C9L, 0x3L, 1L, 1L, 0L },
  { 0x12FL, 0x3D0L, 0x3L, 1L, 1L, 0L },
                                        
  { 0x11BL, 0x3D6L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 0 */
  { 0x107L, 0x3DCL, 0x2L, 1L, 1L, 0L },
  { 0x0F2L, 0x3E1L, 0x2L, 1L, 1L, 0L },
  { 0x0DEL, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x0C9L, 0x3EAL, 0x2L, 1L, 1L, 0L },
  { 0x0B2L, 0x3EEL, 0x2L, 1L, 1L, 0L },
  { 0x09BL, 0x3F2L, 0x2L, 1L, 1L, 0L },
  { 0x081L, 0x3F6L, 0x2L, 1L, 1L, 0L },
  { 0x065L, 0x3F9L, 0x2L, 1L, 1L, 0L },
  { 0x047L, 0x3FBL, 0x2L, 1L, 1L, 0L },
  { 0x025L, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x002L, 0x3FEL, 0x2L, 1L, 1L, 0L },
  { 0x02FL, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x062L, 0x3F9L, 0x2L, 1L, 1L, 0L },
  { 0x09BL, 0x3F1L, 0x2L, 1L, 1L, 0L },
  { 0x0D9L, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x11BL, 0x3D4L, 0x2L, 1L, 1L, 0L },
  { 0x162L, 0x3BDL, 0x2L, 1L, 1L, 0L },
  { 0x1AAL, 0x39FL, 0x2L, 1L, 1L, 0L },
  { 0x1F1L, 0x37BL, 0x2L, 1L, 1L, 0L },
  { 0x235L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x273L, 0x325L, 0x2L, 1L, 1L, 0L },
  { 0x2ABL, 0x2F7L, 0x2L, 1L, 1L, 0L },
  { 0x2DBL, 0x2C8L, 0x2L, 1L, 1L, 0L },
  { 0x305L, 0x29BL, 0x2L, 1L, 1L, 0L },
  { 0x329L, 0x26FL, 0x2L, 1L, 1L, 0L },
  { 0x349L, 0x244L, 0x2L, 1L, 1L, 0L },
  { 0x363L, 0x21BL, 0x2L, 1L, 1L, 0L },
  { 0x37AL, 0x1F5L, 0x2L, 1L, 1L, 0L },
  { 0x38EL, 0x1D0L, 0x2L, 1L, 1L, 0L },
  { 0x39FL, 0x1ADL, 0x2L, 1L, 1L, 0L },
  { 0x3AEL, 0x18BL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x3BCL, 0x169L, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 0 */
  { 0x3C8L, 0x148L, 0x1L, 1L, 1L, 0L },
  { 0x3D3L, 0x126L, 0x1L, 1L, 1L, 0L },
  { 0x3DCL, 0x103L, 0x1L, 1L, 1L, 0L },
  { 0x3E5L, 0x0DFL, 0x1L, 1L, 1L, 0L },
  { 0x3EDL, 0x0B9L, 0x1L, 1L, 1L, 0L },
  { 0x3F4L, 0x090L, 0x1L, 1L, 1L, 0L },
  { 0x3F9L, 0x063L, 0x1L, 1L, 1L, 0L },
  { 0x3FCL, 0x033L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x003L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x040L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x084L, 0x1L, 1L, 1L, 0L },
  { 0x3E6L, 0x0CFL, 0x1L, 1L, 1L, 0L },
  { 0x3D1L, 0x120L, 0x1L, 1L, 1L, 0L },
  { 0x3B5L, 0x173L, 0x1L, 1L, 1L, 0L },
  { 0x390L, 0x1C5L, 0x1L, 1L, 1L, 0L },
  { 0x365L, 0x213L, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x25BL, 0x1L, 1L, 1L, 0L },
  { 0x303L, 0x29BL, 0x1L, 1L, 1L, 0L },
  { 0x2D0L, 0x2D2L, 0x1L, 1L, 1L, 0L },
  { 0x2A0L, 0x300L, 0x1L, 1L, 1L, 0L },
  { 0x271L, 0x327L, 0x1L, 1L, 1L, 0L },
  { 0x244L, 0x348L, 0x1L, 1L, 1L, 0L },
  { 0x21AL, 0x364L, 0x1L, 1L, 1L, 0L },
  { 0x1F3L, 0x37BL, 0x1L, 1L, 1L, 0L },
  { 0x1D0L, 0x38EL, 0x1L, 1L, 1L, 0L },
  { 0x1B0L, 0x39EL, 0x1L, 1L, 1L, 0L },
  { 0x193L, 0x3ABL, 0x1L, 1L, 1L, 0L },
  { 0x178L, 0x3B6L, 0x1L, 1L, 1L, 0L },
  { 0x15FL, 0x3C0L, 0x1L, 1L, 1L, 0L },
  { 0x148L, 0x3C8L, 0x1L, 1L, 1L, 0L },
  { 0x133L, 0x3CFL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x11EL, 0x3D5L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 0 */
  { 0x10AL, 0x3DBL, 0x0L, 1L, 1L, 0L },
  { 0x0F5L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x0E1L, 0x3E5L, 0x0L, 1L, 1L, 0L },
  { 0x0CBL, 0x3EAL, 0x0L, 1L, 1L, 0L },
  { 0x0B3L, 0x3EEL, 0x0L, 1L, 1L, 0L },
  { 0x09BL, 0x3F2L, 0x0L, 1L, 1L, 0L },
  { 0x080L, 0x3F6L, 0x0L, 1L, 1L, 0L },
  { 0x063L, 0x3F9L, 0x0L, 1L, 1L, 0L },
  { 0x044L, 0x3FBL, 0x0L, 1L, 1L, 0L },
  { 0x021L, 0x3FDL, 0x0L, 1L, 1L, 0L },
  { 0x006L, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x033L, 0x3FCL, 0x0L, 1L, 1L, 0L },
  { 0x067L, 0x3F8L, 0x0L, 1L, 1L, 0L },
  { 0x0A0L, 0x3F0L, 0x0L, 1L, 1L, 0L },
  { 0x0DFL, 0x3E4L, 0x0L, 1L, 1L, 0L },
  { 0x122L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x168L, 0x3BAL, 0x0L, 1L, 1L, 0L },
  { 0x1AFL, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x1F6L, 0x378L, 0x0L, 1L, 1L, 0L },
  { 0x239L, 0x34EL, 0x0L, 1L, 1L, 0L },
  { 0x276L, 0x322L, 0x0L, 1L, 1L, 0L },
  { 0x2ACL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x2DCL, 0x2C7L, 0x0L, 1L, 1L, 0L },
  { 0x305L, 0x29BL, 0x0L, 1L, 1L, 0L },
  { 0x328L, 0x270L, 0x0L, 1L, 1L, 0L },
  { 0x347L, 0x246L, 0x0L, 1L, 1L, 0L },
  { 0x362L, 0x21EL, 0x0L, 1L, 1L, 0L },
  { 0x379L, 0x1F8L, 0x0L, 1L, 1L, 0L },
  { 0x38CL, 0x1D4L, 0x0L, 1L, 1L, 0L },
  { 0x39DL, 0x1B1L, 0x0L, 1L, 1L, 0L },
  { 0x3ACL, 0x190L, 0x0L, 1L, 1L, 0L } } },
                                        
{{{ 0x3FEL, 0x010L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 1 */
  { 0x3FDL, 0x03DL, 0x3L, 1L, 1L, 0L },
  { 0x3F9L, 0x068L, 0x3L, 1L, 1L, 0L },
  { 0x3F4L, 0x08FL, 0x3L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B4L, 0x3L, 1L, 1L, 0L },
  { 0x3E7L, 0x0D7L, 0x3L, 1L, 1L, 0L },
  { 0x3DFL, 0x0FAL, 0x3L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x3L, 1L, 1L, 0L },
  { 0x3CBL, 0x13EL, 0x3L, 1L, 1L, 0L },
  { 0x3BFL, 0x160L, 0x3L, 1L, 1L, 0L },
  { 0x3B2L, 0x184L, 0x3L, 1L, 1L, 0L },
  { 0x3A2L, 0x1A9L, 0x3L, 1L, 1L, 0L },
  { 0x38FL, 0x1CFL, 0x3L, 1L, 1L, 0L },
  { 0x379L, 0x1F8L, 0x3L, 1L, 1L, 0L },
  { 0x35FL, 0x224L, 0x3L, 1L, 1L, 0L },
  { 0x340L, 0x252L, 0x3L, 1L, 1L, 0L },
  { 0x31BL, 0x283L, 0x3L, 1L, 1L, 0L },
  { 0x2EFL, 0x2B5L, 0x3L, 1L, 1L, 0L },
  { 0x2BBL, 0x2EAL, 0x3L, 1L, 1L, 0L },
  { 0x27EL, 0x31FL, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x1ECL, 0x380L, 0x3L, 1L, 1L, 0L },
  { 0x19BL, 0x3A8L, 0x3L, 1L, 1L, 0L },
  { 0x149L, 0x3C8L, 0x3L, 1L, 1L, 0L },
  { 0x0FAL, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x0B2L, 0x3EFL, 0x3L, 1L, 1L, 0L },
  { 0x073L, 0x3F8L, 0x3L, 1L, 1L, 0L },
  { 0x03CL, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x00CL, 0x3FFL, 0x3L, 1L, 1L, 0L },
  { 0x01DL, 0x3FEL, 0x3L, 1L, 1L, 0L },
  { 0x040L, 0x3FCL, 0x3L, 1L, 1L, 0L },
  { 0x059L, 0x3FAL, 0x3L, 1L, 1L, 0L },
                                        
  { 0x076L, 0x3F7L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 1 */
  { 0x091L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x0AAL, 0x3EFL, 0x2L, 1L, 1L, 0L },
  { 0x0C2L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x0D8L, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x0EEL, 0x3E2L, 0x2L, 1L, 1L, 0L },
  { 0x103L, 0x3DCL, 0x2L, 1L, 1L, 0L },
  { 0x116L, 0x3D7L, 0x2L, 1L, 1L, 0L },
  { 0x12BL, 0x3D1L, 0x2L, 1L, 1L, 0L },
  { 0x13FL, 0x3CAL, 0x2L, 1L, 1L, 0L },
  { 0x155L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x16BL, 0x3BBL, 0x2L, 1L, 1L, 0L },
  { 0x184L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x19EL, 0x3A6L, 0x2L, 1L, 1L, 0L },
  { 0x1BBL, 0x399L, 0x2L, 1L, 1L, 0L },
  { 0x1DCL, 0x389L, 0x2L, 1L, 1L, 0L },
  { 0x200L, 0x376L, 0x2L, 1L, 1L, 0L },
  { 0x227L, 0x35FL, 0x2L, 1L, 1L, 0L },
  { 0x252L, 0x343L, 0x2L, 1L, 1L, 0L },
  { 0x280L, 0x322L, 0x2L, 1L, 1L, 0L },
  { 0x2B0L, 0x2FAL, 0x2L, 1L, 1L, 0L },
  { 0x2E1L, 0x2CBL, 0x2L, 1L, 1L, 0L },
  { 0x313L, 0x294L, 0x2L, 1L, 1L, 0L },
  { 0x345L, 0x254L, 0x2L, 1L, 1L, 0L },
  { 0x373L, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1C1L, 0x2L, 1L, 1L, 0L },
  { 0x3BDL, 0x172L, 0x2L, 1L, 1L, 0L },
  { 0x3D7L, 0x123L, 0x2L, 1L, 1L, 0L },
  { 0x3E9L, 0x0D9L, 0x2L, 1L, 1L, 0L },
  { 0x3F4L, 0x095L, 0x2L, 1L, 1L, 0L },
  { 0x3FBL, 0x058L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x020L, 0x2L, 1L, 1L, 0L },
                                        
  { 0x3FEL, 0x010L, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 1 */
  { 0x3FDL, 0x03DL, 0x1L, 1L, 1L, 0L },
  { 0x3F9L, 0x068L, 0x1L, 1L, 1L, 0L },
  { 0x3F4L, 0x08FL, 0x1L, 1L, 1L, 0L },
  { 0x3EEL, 0x0B4L, 0x1L, 1L, 1L, 0L },
  { 0x3E7L, 0x0D7L, 0x1L, 1L, 1L, 0L },
  { 0x3DFL, 0x0FAL, 0x1L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x1L, 1L, 1L, 0L },
  { 0x3CBL, 0x13EL, 0x1L, 1L, 1L, 0L },
  { 0x3BFL, 0x160L, 0x1L, 1L, 1L, 0L },
  { 0x3B2L, 0x184L, 0x1L, 1L, 1L, 0L },
  { 0x3A2L, 0x1A9L, 0x1L, 1L, 1L, 0L },
  { 0x38FL, 0x1CFL, 0x1L, 1L, 1L, 0L },
  { 0x379L, 0x1F8L, 0x1L, 1L, 1L, 0L },
  { 0x35FL, 0x224L, 0x1L, 1L, 1L, 0L },
  { 0x340L, 0x252L, 0x1L, 1L, 1L, 0L },
  { 0x31BL, 0x283L, 0x1L, 1L, 1L, 0L },
  { 0x2EFL, 0x2B5L, 0x1L, 1L, 1L, 0L },
  { 0x2BBL, 0x2EAL, 0x1L, 1L, 1L, 0L },
  { 0x27EL, 0x31FL, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x1ECL, 0x380L, 0x1L, 1L, 1L, 0L },
  { 0x19BL, 0x3A8L, 0x1L, 1L, 1L, 0L },
  { 0x149L, 0x3C8L, 0x1L, 1L, 1L, 0L },
  { 0x0FAL, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x0B2L, 0x3EFL, 0x1L, 1L, 1L, 0L },
  { 0x073L, 0x3F8L, 0x1L, 1L, 1L, 0L },
  { 0x03CL, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x00CL, 0x3FFL, 0x1L, 1L, 1L, 0L },
  { 0x01DL, 0x3FEL, 0x1L, 1L, 1L, 0L },
  { 0x040L, 0x3FCL, 0x1L, 1L, 1L, 0L },
  { 0x05EL, 0x3FAL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x076L, 0x3F7L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 1 */
  { 0x091L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x0AAL, 0x3EFL, 0x0L, 1L, 1L, 0L },
  { 0x0C2L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x0D8L, 0x3E6L, 0x0L, 1L, 1L, 0L },
  { 0x0EEL, 0x3E2L, 0x0L, 1L, 1L, 0L },
  { 0x103L, 0x3DCL, 0x0L, 1L, 1L, 0L },
  { 0x116L, 0x3D7L, 0x0L, 1L, 1L, 0L },
  { 0x12BL, 0x3D1L, 0x0L, 1L, 1L, 0L },
  { 0x13FL, 0x3CAL, 0x0L, 1L, 1L, 0L },
  { 0x155L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x16BL, 0x3BBL, 0x0L, 1L, 1L, 0L },
  { 0x184L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x19EL, 0x3A6L, 0x0L, 1L, 1L, 0L },
  { 0x1BBL, 0x399L, 0x0L, 1L, 1L, 0L },
  { 0x1DCL, 0x389L, 0x0L, 1L, 1L, 0L },
  { 0x200L, 0x376L, 0x0L, 1L, 1L, 0L },
  { 0x227L, 0x35FL, 0x0L, 1L, 1L, 0L },
  { 0x252L, 0x343L, 0x0L, 1L, 1L, 0L },
  { 0x280L, 0x322L, 0x0L, 1L, 1L, 0L },
  { 0x2B0L, 0x2FAL, 0x0L, 1L, 1L, 0L },
  { 0x2E1L, 0x2CBL, 0x0L, 1L, 1L, 0L },
  { 0x313L, 0x294L, 0x0L, 1L, 1L, 0L },
  { 0x345L, 0x254L, 0x0L, 1L, 1L, 0L },
  { 0x373L, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1C1L, 0x0L, 1L, 1L, 0L },
  { 0x3BDL, 0x172L, 0x0L, 1L, 1L, 0L },
  { 0x3D7L, 0x123L, 0x0L, 1L, 1L, 0L },
  { 0x3E9L, 0x0D9L, 0x0L, 1L, 1L, 0L },
  { 0x3F4L, 0x095L, 0x0L, 1L, 1L, 0L },
  { 0x3FBL, 0x058L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x020L, 0x0L, 1L, 1L, 0L } },
                                        
{ { 0x3B9L, 0x170L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 1 */
  { 0x3C5L, 0x14FL, 0x3L, 1L, 1L, 0L },
  { 0x3D0L, 0x12DL, 0x3L, 1L, 1L, 0L },
  { 0x3DAL, 0x10AL, 0x3L, 1L, 1L, 0L },
  { 0x3E3L, 0x0E6L, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C0L, 0x3L, 1L, 1L, 0L },
  { 0x3F2L, 0x097L, 0x3L, 1L, 1L, 0L },
  { 0x3F8L, 0x06AL, 0x3L, 1L, 1L, 0L },
  { 0x3FCL, 0x038L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x000L, 0x3L, 1L, 1L, 0L },
  { 0x3FBL, 0x03DL, 0x3L, 1L, 1L, 0L },
  { 0x3F4L, 0x084L, 0x3L, 1L, 1L, 0L },
  { 0x3E6L, 0x0D2L, 0x3L, 1L, 1L, 0L },
  { 0x3D0L, 0x126L, 0x3L, 1L, 1L, 0L },
  { 0x3B2L, 0x17CL, 0x3L, 1L, 1L, 0L },
  { 0x38CL, 0x1D0L, 0x3L, 1L, 1L, 0L },
  { 0x35FL, 0x21FL, 0x3L, 1L, 1L, 0L },
  { 0x32DL, 0x267L, 0x3L, 1L, 1L, 0L },
  { 0x2FAL, 0x2A6L, 0x3L, 1L, 1L, 0L },
  { 0x2C6L, 0x2DCL, 0x3L, 1L, 1L, 0L },
  { 0x295L, 0x309L, 0x3L, 1L, 1L, 0L },
  { 0x267L, 0x32FL, 0x3L, 1L, 1L, 0L },
  { 0x23AL, 0x34FL, 0x3L, 1L, 1L, 0L },
  { 0x211L, 0x36AL, 0x3L, 1L, 1L, 0L },
  { 0x1ECL, 0x37FL, 0x3L, 1L, 1L, 0L },
  { 0x1C9L, 0x392L, 0x3L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A1L, 0x3L, 1L, 1L, 0L },
  { 0x18DL, 0x3ADL, 0x3L, 1L, 1L, 0L },
  { 0x174L, 0x3B8L, 0x3L, 1L, 1L, 0L },
  { 0x15BL, 0x3C1L, 0x3L, 1L, 1L, 0L },
  { 0x145L, 0x3C9L, 0x3L, 1L, 1L, 0L },
  { 0x12FL, 0x3D0L, 0x3L, 1L, 1L, 0L },
                                        
  { 0x11BL, 0x3D6L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 1 */
  { 0x107L, 0x3DCL, 0x2L, 1L, 1L, 0L },
  { 0x0F2L, 0x3E1L, 0x2L, 1L, 1L, 0L },
  { 0x0DEL, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x0C9L, 0x3EAL, 0x2L, 1L, 1L, 0L },
  { 0x0B2L, 0x3EEL, 0x2L, 1L, 1L, 0L },
  { 0x09BL, 0x3F2L, 0x2L, 1L, 1L, 0L },
  { 0x081L, 0x3F6L, 0x2L, 1L, 1L, 0L },
  { 0x065L, 0x3F9L, 0x2L, 1L, 1L, 0L },
  { 0x047L, 0x3FBL, 0x2L, 1L, 1L, 0L },
  { 0x025L, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x002L, 0x3FEL, 0x2L, 1L, 1L, 0L },
  { 0x02FL, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x062L, 0x3F9L, 0x2L, 1L, 1L, 0L },
  { 0x09BL, 0x3F1L, 0x2L, 1L, 1L, 0L },
  { 0x0D9L, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x11BL, 0x3D4L, 0x2L, 1L, 1L, 0L },
  { 0x162L, 0x3BDL, 0x2L, 1L, 1L, 0L },
  { 0x1AAL, 0x39FL, 0x2L, 1L, 1L, 0L },
  { 0x1F1L, 0x37BL, 0x2L, 1L, 1L, 0L },
  { 0x235L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x273L, 0x325L, 0x2L, 1L, 1L, 0L },
  { 0x2ABL, 0x2F7L, 0x2L, 1L, 1L, 0L },
  { 0x2DBL, 0x2C8L, 0x2L, 1L, 1L, 0L },
  { 0x305L, 0x29BL, 0x2L, 1L, 1L, 0L },
  { 0x329L, 0x26FL, 0x2L, 1L, 1L, 0L },
  { 0x349L, 0x244L, 0x2L, 1L, 1L, 0L },
  { 0x363L, 0x21BL, 0x2L, 1L, 1L, 0L },
  { 0x37AL, 0x1F5L, 0x2L, 1L, 1L, 0L },
  { 0x38EL, 0x1D0L, 0x2L, 1L, 1L, 0L },
  { 0x39FL, 0x1ADL, 0x2L, 1L, 1L, 0L },
  { 0x3AEL, 0x18BL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x3BCL, 0x169L, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 1 */
  { 0x3C8L, 0x148L, 0x1L, 1L, 1L, 0L },
  { 0x3D3L, 0x126L, 0x1L, 1L, 1L, 0L },
  { 0x3DCL, 0x103L, 0x1L, 1L, 1L, 0L },
  { 0x3E5L, 0x0DFL, 0x1L, 1L, 1L, 0L },
  { 0x3EDL, 0x0B9L, 0x1L, 1L, 1L, 0L },
  { 0x3F4L, 0x090L, 0x1L, 1L, 1L, 0L },
  { 0x3F9L, 0x063L, 0x1L, 1L, 1L, 0L },
  { 0x3FCL, 0x033L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x003L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x040L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x084L, 0x1L, 1L, 1L, 0L },
  { 0x3E6L, 0x0CFL, 0x1L, 1L, 1L, 0L },
  { 0x3D1L, 0x120L, 0x1L, 1L, 1L, 0L },
  { 0x3B5L, 0x173L, 0x1L, 1L, 1L, 0L },
  { 0x390L, 0x1C5L, 0x1L, 1L, 1L, 0L },
  { 0x365L, 0x213L, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x25BL, 0x1L, 1L, 1L, 0L },
  { 0x303L, 0x29BL, 0x1L, 1L, 1L, 0L },
  { 0x2D0L, 0x2D2L, 0x1L, 1L, 1L, 0L },
  { 0x2A0L, 0x300L, 0x1L, 1L, 1L, 0L },
  { 0x271L, 0x327L, 0x1L, 1L, 1L, 0L },
  { 0x244L, 0x348L, 0x1L, 1L, 1L, 0L },
  { 0x21AL, 0x364L, 0x1L, 1L, 1L, 0L },
  { 0x1F3L, 0x37BL, 0x1L, 1L, 1L, 0L },
  { 0x1D0L, 0x38EL, 0x1L, 1L, 1L, 0L },
  { 0x1B0L, 0x39EL, 0x1L, 1L, 1L, 0L },
  { 0x193L, 0x3ABL, 0x1L, 1L, 1L, 0L },
  { 0x178L, 0x3B6L, 0x1L, 1L, 1L, 0L },
  { 0x15FL, 0x3C0L, 0x1L, 1L, 1L, 0L },
  { 0x148L, 0x3C8L, 0x1L, 1L, 1L, 0L },
  { 0x133L, 0x3CFL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x11EL, 0x3D5L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 1 */
  { 0x10AL, 0x3DBL, 0x0L, 1L, 1L, 0L },
  { 0x0F5L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x0E1L, 0x3E5L, 0x0L, 1L, 1L, 0L },
  { 0x0CBL, 0x3EAL, 0x0L, 1L, 1L, 0L },
  { 0x0B3L, 0x3EEL, 0x0L, 1L, 1L, 0L },
  { 0x09BL, 0x3F2L, 0x0L, 1L, 1L, 0L },
  { 0x080L, 0x3F6L, 0x0L, 1L, 1L, 0L },
  { 0x063L, 0x3F9L, 0x0L, 1L, 1L, 0L },
  { 0x044L, 0x3FBL, 0x0L, 1L, 1L, 0L },
  { 0x021L, 0x3FDL, 0x0L, 1L, 1L, 0L },
  { 0x006L, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x033L, 0x3FCL, 0x0L, 1L, 1L, 0L },
  { 0x067L, 0x3F8L, 0x0L, 1L, 1L, 0L },
  { 0x0A0L, 0x3F0L, 0x0L, 1L, 1L, 0L },
  { 0x0DFL, 0x3E4L, 0x0L, 1L, 1L, 0L },
  { 0x122L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x168L, 0x3BAL, 0x0L, 1L, 1L, 0L },
  { 0x1AFL, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x1F6L, 0x378L, 0x0L, 1L, 1L, 0L },
  { 0x239L, 0x34EL, 0x0L, 1L, 1L, 0L },
  { 0x276L, 0x322L, 0x0L, 1L, 1L, 0L },
  { 0x2ACL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x2DCL, 0x2C7L, 0x0L, 1L, 1L, 0L },
  { 0x305L, 0x29BL, 0x0L, 1L, 1L, 0L },
  { 0x328L, 0x270L, 0x0L, 1L, 1L, 0L },
  { 0x347L, 0x246L, 0x0L, 1L, 1L, 0L },
  { 0x362L, 0x21EL, 0x0L, 1L, 1L, 0L },
  { 0x379L, 0x1F8L, 0x0L, 1L, 1L, 0L },
  { 0x38CL, 0x1D4L, 0x0L, 1L, 1L, 0L },
  { 0x39DL, 0x1B1L, 0x0L, 1L, 1L, 0L },
  { 0x3ACL, 0x190L, 0x0L, 1L, 1L, 0L } } } }
};

/* Test PWM Table: fixed codings for different directions and speeds */

const struct rism_pwmstd_t R_ISM_A_STDPWMTABLE4 = {

  /*  CVP     CHP   CQI   IVR IHR */
{
{{{ 0x000L, 0x1FFL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 0 */
  { 0x001L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x002L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x003L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x004L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x005L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x006L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x007L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x008L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x009L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x010L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x011L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x012L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x013L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x014L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x015L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x016L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x017L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x018L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x019L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
                                        
  { 0x020L, 0x1FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 0 */
  { 0x021L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x022L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x023L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x024L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x025L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x026L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x027L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x028L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x029L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x02AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x02BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x02CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x02DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x02EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x02FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x030L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x031L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x032L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x033L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x034L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x035L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x036L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x037L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x038L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x039L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x03AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x03BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x03CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x03DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x03EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x03FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x040L, 0x1FFL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 0 */
  { 0x041L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x042L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x043L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x044L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x045L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x046L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x047L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x048L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x049L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x04AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x04BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x04CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x04DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x04EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x04FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x050L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x051L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x052L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x053L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x054L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x055L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x056L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x057L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x058L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x059L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x05AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x05BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x05CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x05DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x05EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x05FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x060L, 0x1FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 0 */
  { 0x061L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x062L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x063L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x064L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x065L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x066L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x067L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x068L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x069L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x06AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x06BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x06CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x06DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x06EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x06FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x070L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x071L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x072L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x073L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x074L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x075L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x076L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x077L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x078L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x079L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x07AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x07BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x07CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x07DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x07EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x07FL, 0x1FFL, 0x3L, 1L, 1L, 0L } },
                                        
{ { 0x3FFL, 0x1FFL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 0 */
  { 0x3FEL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3FDL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3FCL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3FBL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3FAL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F9L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F8L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F7L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F6L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F5L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F4L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F3L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F2L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F1L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3F0L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3EFL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3EEL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3EDL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3ECL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3EAL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E9L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E8L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E7L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E6L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E5L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E4L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E3L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E2L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E1L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x1FFL, 0x0L, 1L, 1L, 0L },
                                        
  { 0x3DFL, 0x1FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 0 */
  { 0x3DEL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DDL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DCL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DBL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DAL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D9L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D8L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D7L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D6L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D5L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D4L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D3L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D1L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D0L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DFL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DEL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DDL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DCL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DBL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3DAL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D9L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D8L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D7L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D6L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D5L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D4L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D3L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D1L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x3D0L, 0x1FFL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x3CFL, 0x1FFL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 0 */
  { 0x3CEL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CDL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CCL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CBL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CAL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C9L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C8L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C7L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C6L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C5L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C4L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C2L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C1L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C0L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CFL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CEL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CDL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CCL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CBL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3CAL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C9L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C8L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C7L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C6L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C5L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C4L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C2L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C1L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x3C0L, 0x1FFL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x3BFL, 0x1FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 0 */
  { 0x3BEL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BDL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BCL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BBL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BAL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B9L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B8L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B7L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B6L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B5L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B4L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B3L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B2L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B0L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BFL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BEL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BDL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BCL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BBL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3BAL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B9L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B8L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B7L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B6L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B5L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B4L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B3L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B2L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x3B0L, 0x1FFL, 0x3L, 1L, 1L, 0L } } },
                                        
{{{ 0x100L, 0x1FFL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 1 */
  { 0x101L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x102L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x103L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x104L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x105L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x106L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x107L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x108L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x109L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x10AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x10BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x10CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x10DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x10EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x10FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x110L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x111L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x112L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x113L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x114L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x115L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x116L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x117L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x118L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x119L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x11AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x11BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x11CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x11DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x11EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x11FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
                                        
  { 0x120L, 0x1FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 1 */
  { 0x121L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x122L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x123L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x124L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x125L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x126L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x127L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x129L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x130L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x131L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x132L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x133L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x134L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x135L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x136L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x137L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x138L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x139L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x140L, 0x1FFL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 1 */
  { 0x141L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x142L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x143L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x144L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x145L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x146L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x147L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x148L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x149L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x14AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x14BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x14CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x14DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x14EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x14FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x150L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x151L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x152L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x153L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x154L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x155L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x156L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x157L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x158L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x159L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x15AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x15BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x15CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x15DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x15EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x15FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x160L, 0x1FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 1 */
  { 0x161L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x162L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x163L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x164L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x165L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x166L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x167L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x168L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x169L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x16AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x16BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x16CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x16DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x16EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x16FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x170L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x171L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x172L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x173L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x174L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x175L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x176L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x177L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x178L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x179L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x17AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x17BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x17CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x17DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x17EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x17FL, 0x1FFL, 0x3L, 1L, 1L, 0L } },
                                        
{ { 0x2FFL, 0x1FFL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 1 */
  { 0x2FEL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2FDL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2FCL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2FBL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2FAL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F9L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F8L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F7L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F6L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F4L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F3L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F2L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F1L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2F0L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2EFL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2EEL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2EDL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2ECL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2EBL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2EAL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E9L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E8L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E7L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E6L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E5L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E4L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E3L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E2L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E1L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x2E0L, 0x1FFL, 0x0L, 1L, 1L, 0L },
                                        
  { 0x2DFL, 0x1FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 1 */
  { 0x2DEL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DDL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DCL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DBL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DAL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D9L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D8L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D7L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D6L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D5L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D4L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D2L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D1L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D0L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DFL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DEL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DDL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DCL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DBL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2DAL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D9L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D8L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D7L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D6L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D5L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D4L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D2L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D1L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x2D0L, 0x1FFL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x2CFL, 0x1FFL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 1 */
  { 0x2CEL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CDL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CCL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CBL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CAL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C9L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C8L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C7L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C6L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C5L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C4L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C3L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C2L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C1L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C0L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CFL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CEL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CDL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CCL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CBL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2CAL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C9L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C8L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C7L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C6L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C5L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C4L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C3L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C2L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C1L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x2C0L, 0x1FFL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x2BFL, 0x1FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 1 */
  { 0x2BEL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BDL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BCL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BBL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BAL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B9L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B8L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B7L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B6L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B5L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B4L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B3L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B2L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B1L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B0L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BFL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BEL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BDL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BCL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BBL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2BAL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B9L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B8L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B7L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B6L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B5L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B4L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B3L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B2L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B1L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x2B0L, 0x1FFL, 0x3L, 1L, 1L, 0L } } } }
};

/* Test PWM Table: alternate fixed codings for different directions and speeds */

const struct rism_pwmstd_t R_ISM_A_STDPWMTABLE5 = {

  /*  CVP     CHP   CQI   IVR IHR */
{
{{{ 0x1FFL, 0x000L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 0 */
  { 0x1FFL, 0x001L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x002L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x003L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x004L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x005L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x006L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x007L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x008L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x009L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x010L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x011L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x012L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x013L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x014L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x015L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x016L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x017L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x018L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x019L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01FL, 0x0L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x020L, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 0 */
  { 0x1FFL, 0x021L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x022L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x023L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x024L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x025L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x026L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x027L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x028L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x029L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x02AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x02BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x02CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x02DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x02EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x02FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x030L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x031L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x032L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x033L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x034L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x035L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x036L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x037L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x038L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x039L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x03AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x03BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x03CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x03DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x03EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x03FL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x040L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 0 */
  { 0x1FFL, 0x041L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x042L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x043L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x044L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x045L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x046L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x047L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x048L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x049L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x04AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x04BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x04CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x04DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x04EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x04FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x050L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x051L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x052L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x053L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x054L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x055L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x056L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x057L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x058L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x059L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x05AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x05BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x05CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x05DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x05EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x05FL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x060L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 0 */
  { 0x1FFL, 0x061L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x062L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x063L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x064L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x065L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x066L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x067L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x068L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x069L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x06AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x06BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x06CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x06DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x06EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x06FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x070L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x071L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x072L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x073L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x074L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x075L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x076L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x077L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x078L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x079L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x07AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x07BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x07CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x07DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x07EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x07FL, 0x3L, 1L, 1L, 0L } },
                                        
{ { 0x1FFL, 0x3FFL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 0 */
  { 0x1FFL, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3FDL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3FCL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3FBL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3FAL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F9L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F8L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F7L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F6L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F5L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F4L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F2L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F1L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3F0L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3EFL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3EEL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3EDL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3ECL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3EAL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E9L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E8L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E7L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E6L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E5L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E4L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E3L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E2L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E1L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x3E0L, 0x0L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x3DFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 0 */
  { 0x1FFL, 0x3DEL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DDL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DCL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DBL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DAL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D9L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D8L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D7L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D6L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D5L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D4L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D3L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D1L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D0L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DFL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DEL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DDL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DCL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DBL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3DAL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D9L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D8L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D7L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D6L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D5L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D4L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D3L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D1L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x3D0L, 0x1L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x3CFL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 0 */
  { 0x1FFL, 0x3CEL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CDL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CCL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CBL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CAL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C9L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C8L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C7L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C6L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C5L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C4L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C2L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C1L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C0L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CFL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CEL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CDL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CCL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CBL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3CAL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C9L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C8L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C7L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C6L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C5L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C4L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C2L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C1L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x3C0L, 0x2L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x3BFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 0 */
  { 0x1FFL, 0x3BEL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BDL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BCL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BBL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BAL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B9L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B8L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B7L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B6L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B5L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B4L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B3L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B2L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B0L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BFL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BEL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BDL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BCL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BBL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3BAL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B9L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B8L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B7L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B6L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B5L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B4L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B3L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B2L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x3B0L, 0x3L, 1L, 1L, 0L } } },
                                        
{{{ 0x1FFL, 0x100L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 1 */
  { 0x1FFL, 0x101L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x102L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x103L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x104L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x105L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x106L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x107L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x108L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x109L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x10AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x10BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x10CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x10DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x10EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x10FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x110L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x111L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x112L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x113L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x114L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x115L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x116L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x117L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x118L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x119L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x11AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x11BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x11CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x11DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x11EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x11FL, 0x0L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x120L, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 1 */
  { 0x1FFL, 0x121L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x122L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x123L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x124L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x125L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x126L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x127L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x129L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x130L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x131L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x132L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x133L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x134L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x135L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x136L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x137L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x138L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x139L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13FL, 0x1L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x140L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 1 */
  { 0x1FFL, 0x141L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x142L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x143L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x144L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x145L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x146L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x147L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x148L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x149L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x14AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x14BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x14CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x14DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x14EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x14FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x150L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x151L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x152L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x153L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x154L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x155L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x156L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x157L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x158L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x159L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x15AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x15BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x15CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x15DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x15EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x15FL, 0x2L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x160L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 1 */
  { 0x1FFL, 0x161L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x162L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x163L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x164L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x165L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x166L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x167L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x168L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x169L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x16AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x16BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x16CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x16DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x16EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x16FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x170L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x171L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x172L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x173L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x174L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x175L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x176L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x177L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x178L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x179L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x17AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x17BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x17CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x17DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x17EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x17FL, 0x3L, 1L, 1L, 0L } },
                                        
{ { 0x1FFL, 0x2FFL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 1 */
  { 0x1FFL, 0x2FEL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2FDL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2FCL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2FBL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2FAL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F9L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F8L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F7L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F6L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F4L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F3L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F2L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F1L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2F0L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2EFL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2EEL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2EDL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2ECL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2EBL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2EAL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E9L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E8L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E7L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E6L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E5L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E4L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E3L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E2L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E1L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x2E0L, 0x0L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x2DFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 1 */
  { 0x1FFL, 0x2DEL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DDL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DCL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DBL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DAL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D9L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D8L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D7L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D6L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D5L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D4L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D2L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D1L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D0L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DFL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DEL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DDL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DCL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DBL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2DAL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D9L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D8L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D7L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D6L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D5L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D4L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D2L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D1L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x2D0L, 0x1L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x2CFL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 1 */
  { 0x1FFL, 0x2CEL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CDL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CCL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CBL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CAL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C9L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C8L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C7L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C6L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C5L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C4L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C3L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C2L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C1L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C0L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CFL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CEL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CDL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CCL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CBL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2CAL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C9L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C8L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C7L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C6L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C5L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C4L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C3L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C2L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C1L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x2C0L, 0x2L, 1L, 1L, 0L },
                                        
  { 0x1FFL, 0x2BFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 1 */
  { 0x1FFL, 0x2BEL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BDL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BCL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BBL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BAL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B9L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B8L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B7L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B6L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B5L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B4L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B3L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B2L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B1L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B0L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BFL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BEL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BDL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BCL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BBL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2BAL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B9L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B8L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B7L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B6L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B5L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B4L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B3L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B2L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B1L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x2B0L, 0x3L, 1L, 1L, 0L } } } }
};

/* China Motor PWM Table: derived from VID data sheet, equal settings
   for directions and speeds, ONLY works with VID motor */

const struct rism_pwmstd_t R_ISM_A_STDPWMTABLE6 = {

  /*  CVP     CHP   CQI   IVR IHR */
{
{{{ 851, 904,  0, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 0 */
  { 815, 921,  0, 1L, 1L, 0L },
  { 779, 938,  0, 1L, 1L, 0L },
  { 743, 955,  0, 1L, 1L, 0L },
  { 707, 972,  0, 1L, 1L, 0L },
  { 671, 989,  0, 1L, 1L, 0L },
  { 621, 996,  0, 1L, 1L, 0L },
  { 571, 1003, 0, 1L, 1L, 0L },
  { 521, 1010, 0, 1L, 1L, 0L },
  { 471, 1017, 0, 1L, 1L, 0L },
  { 421, 1023, 0, 1L, 1L, 0L },
  { 380, 1017, 0, 1L, 1L, 0L },
  { 339, 1010, 0, 1L, 1L, 0L },
  { 297, 1003, 0, 1L, 1L, 0L },
  { 256, 996,  0, 1L, 1L, 0L },
  { 215, 989,  0, 1L, 1L, 0L },
  { 179, 972,  0, 1L, 1L, 0L },
  { 143, 955,  0, 1L, 1L, 0L },
  { 107, 938,  0, 1L, 1L, 0L },
  { 71,  921,  0, 1L, 1L, 0L },
  { 35,  904,  0, 1L, 1L, 0L },
  { 0,   887,  3, 1L, 1L, 0L },
  { 43,  843,  3, 1L, 1L, 0L },
  { 86,  800,  3, 1L, 1L, 0L },
  { 129, 757,  3, 1L, 1L, 0L },
  { 172, 714,  3, 1L, 1L, 0L },
  { 215, 671,  3, 1L, 1L, 0L },
  { 256, 621,  3, 1L, 1L, 0L },
  { 297, 571,  3, 1L, 1L, 0L },
  { 339, 521,  3, 1L, 1L, 0L },
  { 380, 471,  3, 1L, 1L, 0L },
  { 421, 421,  3, 1L, 1L, 0L },
                                        
  { 462,  386, 3, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 0 */
  { 504,  352, 3, 1L, 1L, 0L },
  { 546,  318, 3, 1L, 1L, 0L },
  { 587,  284, 3, 1L, 1L, 0L },
  { 629,  250, 3, 1L, 1L, 0L },
  { 671,  215, 3, 1L, 1L, 0L },
  { 714,  172, 3, 1L, 1L, 0L },
  { 757,  129, 3, 1L, 1L, 0L },
  { 800,  86,  3, 1L, 1L, 0L },
  { 843,  43,  3, 1L, 1L, 0L },
  { 887,  0,   3, 1L, 1L, 0L },
  { 907,  43,  2, 1L, 1L, 0L },
  { 928,  86,  2, 1L, 1L, 0L },
  { 948,  129, 2, 1L, 1L, 0L },
  { 969,  172, 2, 1L, 1L, 0L },
  { 989,  215, 2, 1L, 1L, 0L },
  { 995,  250, 2, 1L, 1L, 0L },
  { 1001, 284, 2, 1L, 1L, 0L },
  { 1006, 318, 2, 1L, 1L, 0L },
  { 1012, 352, 2, 1L, 1L, 0L },
  { 1018, 386, 2, 1L, 1L, 0L },
  { 1023, 421, 2, 1L, 1L, 0L },
  { 1017, 471, 2, 1L, 1L, 0L },
  { 1010, 521, 2, 1L, 1L, 0L },
  { 1003, 571, 2, 1L, 1L, 0L },
  { 996,  621, 2, 1L, 1L, 0L },
  { 989,  671, 2, 1L, 1L, 0L },
  { 969,  714, 2, 1L, 1L, 0L },
  { 948,  757, 2, 1L, 1L, 0L },
  { 928,  800, 2, 1L, 1L, 0L },
  { 907,  843, 2, 1L, 1L, 0L },
  { 887,  887, 2, 1L, 1L, 0L },
                                        
  { 851, 904,  2, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 0 */
  { 815, 921,  2, 1L, 1L, 0L },
  { 779, 938,  2, 1L, 1L, 0L },
  { 743, 955,  2, 1L, 1L, 0L },
  { 707, 972,  2, 1L, 1L, 0L },
  { 671, 989,  2, 1L, 1L, 0L },
  { 621, 996,  2, 1L, 1L, 0L },
  { 571, 1003, 2, 1L, 1L, 0L },
  { 521, 1010, 2, 1L, 1L, 0L },
  { 471, 1017, 2, 1L, 1L, 0L },
  { 421, 1023, 2, 1L, 1L, 0L },
  { 380, 1017, 2, 1L, 1L, 0L },
  { 339, 1010, 2, 1L, 1L, 0L },
  { 297, 1003, 2, 1L, 1L, 0L },
  { 256, 996,  2, 1L, 1L, 0L },
  { 215, 989,  2, 1L, 1L, 0L },
  { 179, 972,  2, 1L, 1L, 0L },
  { 143, 955,  2, 1L, 1L, 0L },
  { 107, 938,  2, 1L, 1L, 0L },
  { 71,  921,  2, 1L, 1L, 0L },
  { 35,  904,  2, 1L, 1L, 0L },
  { 0,   887,  1, 1L, 1L, 0L },
  { 43,  843,  1, 1L, 1L, 0L },
  { 86,  800,  1, 1L, 1L, 0L },
  { 129, 757,  1, 1L, 1L, 0L },
  { 172, 714,  1, 1L, 1L, 0L },
  { 215, 671,  1, 1L, 1L, 0L },
  { 256, 621,  1, 1L, 1L, 0L },
  { 297, 571,  1, 1L, 1L, 0L },
  { 339, 521,  1, 1L, 1L, 0L },
  { 380, 471,  1, 1L, 1L, 0L },
  { 421, 421,  1, 1L, 1L, 0L },
                                        
  { 462,  386, 1, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 0 */
  { 504,  352, 1, 1L, 1L, 0L },
  { 546,  318, 1, 1L, 1L, 0L },
  { 587,  284, 1, 1L, 1L, 0L },
  { 629,  250, 1, 1L, 1L, 0L },
  { 671,  215, 1, 1L, 1L, 0L },
  { 714,  172, 1, 1L, 1L, 0L },
  { 757,  129, 1, 1L, 1L, 0L },
  { 800,  86,  1, 1L, 1L, 0L },
  { 843,  43,  1, 1L, 1L, 0L },
  { 887,  0,   0, 1L, 1L, 0L },
  { 907,  43,  0, 1L, 1L, 0L },
  { 928,  86,  0, 1L, 1L, 0L },
  { 948,  129, 0, 1L, 1L, 0L },
  { 969,  172, 0, 1L, 1L, 0L },
  { 989,  215, 0, 1L, 1L, 0L },
  { 995,  250, 0, 1L, 1L, 0L },
  { 1001, 284, 0, 1L, 1L, 0L },
  { 1006, 318, 0, 1L, 1L, 0L },
  { 1012, 352, 0, 1L, 1L, 0L },
  { 1018, 386, 0, 1L, 1L, 0L },
  { 1023, 421, 0, 1L, 1L, 0L },
  { 1017, 471, 0, 1L, 1L, 0L },
  { 1010, 521, 0, 1L, 1L, 0L },
  { 1003, 571, 0, 1L, 1L, 0L },
  { 996,  621, 0, 1L, 1L, 0L },
  { 989,  671, 0, 1L, 1L, 0L },
  { 969,  714, 0, 1L, 1L, 0L },
  { 948,  757, 0, 1L, 1L, 0L },
  { 928,  800, 0, 1L, 1L, 0L },
  { 907,  843, 0, 1L, 1L, 0L },
  { 887,  887, 0, 1L, 1L, 0L } },
                                        
{ { 851, 904,  0, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 0 */
  { 815, 921,  0, 1L, 1L, 0L },
  { 779, 938,  0, 1L, 1L, 0L },
  { 743, 955,  0, 1L, 1L, 0L },
  { 707, 972,  0, 1L, 1L, 0L },
  { 671, 989,  0, 1L, 1L, 0L },
  { 621, 996,  0, 1L, 1L, 0L },
  { 571, 1003, 0, 1L, 1L, 0L },
  { 521, 1010, 0, 1L, 1L, 0L },
  { 471, 1017, 0, 1L, 1L, 0L },
  { 421, 1023, 0, 1L, 1L, 0L },
  { 380, 1017, 0, 1L, 1L, 0L },
  { 339, 1010, 0, 1L, 1L, 0L },
  { 297, 1003, 0, 1L, 1L, 0L },
  { 256, 996,  0, 1L, 1L, 0L },
  { 215, 989,  0, 1L, 1L, 0L },
  { 179, 972,  0, 1L, 1L, 0L },
  { 143, 955,  0, 1L, 1L, 0L },
  { 107, 938,  0, 1L, 1L, 0L },
  { 71,  921,  0, 1L, 1L, 0L },
  { 35,  904,  0, 1L, 1L, 0L },
  { 0,   887,  3, 1L, 1L, 0L },
  { 43,  843,  3, 1L, 1L, 0L },
  { 86,  800,  3, 1L, 1L, 0L },
  { 129, 757,  3, 1L, 1L, 0L },
  { 172, 714,  3, 1L, 1L, 0L },
  { 215, 671,  3, 1L, 1L, 0L },
  { 256, 621,  3, 1L, 1L, 0L },
  { 297, 571,  3, 1L, 1L, 0L },
  { 339, 521,  3, 1L, 1L, 0L },
  { 380, 471,  3, 1L, 1L, 0L },
  { 421, 421,  3, 1L, 1L, 0L },
                                        
  { 462,  386, 3, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 0 */
  { 504,  352, 3, 1L, 1L, 0L },
  { 546,  318, 3, 1L, 1L, 0L },
  { 587,  284, 3, 1L, 1L, 0L },
  { 629,  250, 3, 1L, 1L, 0L },
  { 671,  215, 3, 1L, 1L, 0L },
  { 714,  172, 3, 1L, 1L, 0L },
  { 757,  129, 3, 1L, 1L, 0L },
  { 800,  86,  3, 1L, 1L, 0L },
  { 843,  43,  3, 1L, 1L, 0L },
  { 887,  0,   3, 1L, 1L, 0L },
  { 907,  43,  2, 1L, 1L, 0L },
  { 928,  86,  2, 1L, 1L, 0L },
  { 948,  129, 2, 1L, 1L, 0L },
  { 969,  172, 2, 1L, 1L, 0L },
  { 989,  215, 2, 1L, 1L, 0L },
  { 995,  250, 2, 1L, 1L, 0L },
  { 1001, 284, 2, 1L, 1L, 0L },
  { 1006, 318, 2, 1L, 1L, 0L },
  { 1012, 352, 2, 1L, 1L, 0L },
  { 1018, 386, 2, 1L, 1L, 0L },
  { 1023, 421, 2, 1L, 1L, 0L },
  { 1017, 471, 2, 1L, 1L, 0L },
  { 1010, 521, 2, 1L, 1L, 0L },
  { 1003, 571, 2, 1L, 1L, 0L },
  { 996,  621, 2, 1L, 1L, 0L },
  { 989,  671, 2, 1L, 1L, 0L },
  { 969,  714, 2, 1L, 1L, 0L },
  { 948,  757, 2, 1L, 1L, 0L },
  { 928,  800, 2, 1L, 1L, 0L },
  { 907,  843, 2, 1L, 1L, 0L },
  { 887,  887, 2, 1L, 1L, 0L },
                                        
  { 851, 904,  2, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 0 */
  { 815, 921,  2, 1L, 1L, 0L },
  { 779, 938,  2, 1L, 1L, 0L },
  { 743, 955,  2, 1L, 1L, 0L },
  { 707, 972,  2, 1L, 1L, 0L },
  { 671, 989,  2, 1L, 1L, 0L },
  { 621, 996,  2, 1L, 1L, 0L },
  { 571, 1003, 2, 1L, 1L, 0L },
  { 521, 1010, 2, 1L, 1L, 0L },
  { 471, 1017, 2, 1L, 1L, 0L },
  { 421, 1023, 2, 1L, 1L, 0L },
  { 380, 1017, 2, 1L, 1L, 0L },
  { 339, 1010, 2, 1L, 1L, 0L },
  { 297, 1003, 2, 1L, 1L, 0L },
  { 256, 996,  2, 1L, 1L, 0L },
  { 215, 989,  2, 1L, 1L, 0L },
  { 179, 972,  2, 1L, 1L, 0L },
  { 143, 955,  2, 1L, 1L, 0L },
  { 107, 938,  2, 1L, 1L, 0L },
  { 71,  921,  2, 1L, 1L, 0L },
  { 35,  904,  2, 1L, 1L, 0L },
  { 0,   887,  1, 1L, 1L, 0L },
  { 43,  843,  1, 1L, 1L, 0L },
  { 86,  800,  1, 1L, 1L, 0L },
  { 129, 757,  1, 1L, 1L, 0L },
  { 172, 714,  1, 1L, 1L, 0L },
  { 215, 671,  1, 1L, 1L, 0L },
  { 256, 621,  1, 1L, 1L, 0L },
  { 297, 571,  1, 1L, 1L, 0L },
  { 339, 521,  1, 1L, 1L, 0L },
  { 380, 471,  1, 1L, 1L, 0L },
  { 421, 421,  1, 1L, 1L, 0L },
                                        
  { 462,  386, 1, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 0 */
  { 504,  352, 1, 1L, 1L, 0L },
  { 546,  318, 1, 1L, 1L, 0L },
  { 587,  284, 1, 1L, 1L, 0L },
  { 629,  250, 1, 1L, 1L, 0L },
  { 671,  215, 1, 1L, 1L, 0L },
  { 714,  172, 1, 1L, 1L, 0L },
  { 757,  129, 1, 1L, 1L, 0L },
  { 800,  86,  1, 1L, 1L, 0L },
  { 843,  43,  1, 1L, 1L, 0L },
  { 887,  0,   0, 1L, 1L, 0L },
  { 907,  43,  0, 1L, 1L, 0L },
  { 928,  86,  0, 1L, 1L, 0L },
  { 948,  129, 0, 1L, 1L, 0L },
  { 969,  172, 0, 1L, 1L, 0L },
  { 989,  215, 0, 1L, 1L, 0L },
  { 995,  250, 0, 1L, 1L, 0L },
  { 1001, 284, 0, 1L, 1L, 0L },
  { 1006, 318, 0, 1L, 1L, 0L },
  { 1012, 352, 0, 1L, 1L, 0L },
  { 1018, 386, 0, 1L, 1L, 0L },
  { 1023, 421, 0, 1L, 1L, 0L },
  { 1017, 471, 0, 1L, 1L, 0L },
  { 1010, 521, 0, 1L, 1L, 0L },
  { 1003, 571, 0, 1L, 1L, 0L },
  { 996,  621, 0, 1L, 1L, 0L },
  { 989,  671, 0, 1L, 1L, 0L },
  { 969,  714, 0, 1L, 1L, 0L },
  { 948,  757, 0, 1L, 1L, 0L },
  { 928,  800, 0, 1L, 1L, 0L },
  { 907,  843, 0, 1L, 1L, 0L },
  { 887,  887, 0, 1L, 1L, 0L } } },

{{{ 851, 904,  0, 1L, 1L, 0L },    /* 1st quadrant dir. 0, speed 1 */
  { 815, 921,  0, 1L, 1L, 0L },
  { 779, 938,  0, 1L, 1L, 0L },
  { 743, 955,  0, 1L, 1L, 0L },
  { 707, 972,  0, 1L, 1L, 0L },
  { 671, 989,  0, 1L, 1L, 0L },
  { 621, 996,  0, 1L, 1L, 0L },
  { 571, 1003, 0, 1L, 1L, 0L },
  { 521, 1010, 0, 1L, 1L, 0L },
  { 471, 1017, 0, 1L, 1L, 0L },
  { 421, 1023, 0, 1L, 1L, 0L },
  { 380, 1017, 0, 1L, 1L, 0L },
  { 339, 1010, 0, 1L, 1L, 0L },
  { 297, 1003, 0, 1L, 1L, 0L },
  { 256, 996,  0, 1L, 1L, 0L },
  { 215, 989,  0, 1L, 1L, 0L },
  { 179, 972,  0, 1L, 1L, 0L },
  { 143, 955,  0, 1L, 1L, 0L },
  { 107, 938,  0, 1L, 1L, 0L },
  { 71,  921,  0, 1L, 1L, 0L },
  { 35,  904,  0, 1L, 1L, 0L },
  { 0,   887,  3, 1L, 1L, 0L },
  { 43,  843,  3, 1L, 1L, 0L },
  { 86,  800,  3, 1L, 1L, 0L },
  { 129, 757,  3, 1L, 1L, 0L },
  { 172, 714,  3, 1L, 1L, 0L },
  { 215, 671,  3, 1L, 1L, 0L },
  { 256, 621,  3, 1L, 1L, 0L },
  { 297, 571,  3, 1L, 1L, 0L },
  { 339, 521,  3, 1L, 1L, 0L },
  { 380, 471,  3, 1L, 1L, 0L },
  { 421, 421,  3, 1L, 1L, 0L },
                                        
  { 462,  386, 3, 1L, 1L, 0L },    /* 2nd quadrant dir. 0, speed 1 */
  { 504,  352, 3, 1L, 1L, 0L },
  { 546,  318, 3, 1L, 1L, 0L },
  { 587,  284, 3, 1L, 1L, 0L },
  { 629,  250, 3, 1L, 1L, 0L },
  { 671,  215, 3, 1L, 1L, 0L },
  { 714,  172, 3, 1L, 1L, 0L },
  { 757,  129, 3, 1L, 1L, 0L },
  { 800,  86,  3, 1L, 1L, 0L },
  { 843,  43,  3, 1L, 1L, 0L },
  { 887,  0,   3, 1L, 1L, 0L },
  { 907,  43,  2, 1L, 1L, 0L },
  { 928,  86,  2, 1L, 1L, 0L },
  { 948,  129, 2, 1L, 1L, 0L },
  { 969,  172, 2, 1L, 1L, 0L },
  { 989,  215, 2, 1L, 1L, 0L },
  { 995,  250, 2, 1L, 1L, 0L },
  { 1001, 284, 2, 1L, 1L, 0L },
  { 1006, 318, 2, 1L, 1L, 0L },
  { 1012, 352, 2, 1L, 1L, 0L },
  { 1018, 386, 2, 1L, 1L, 0L },
  { 1023, 421, 2, 1L, 1L, 0L },
  { 1017, 471, 2, 1L, 1L, 0L },
  { 1010, 521, 2, 1L, 1L, 0L },
  { 1003, 571, 2, 1L, 1L, 0L },
  { 996,  621, 2, 1L, 1L, 0L },
  { 989,  671, 2, 1L, 1L, 0L },
  { 969,  714, 2, 1L, 1L, 0L },
  { 948,  757, 2, 1L, 1L, 0L },
  { 928,  800, 2, 1L, 1L, 0L },
  { 907,  843, 2, 1L, 1L, 0L },
  { 887,  887, 2, 1L, 1L, 0L },
                                        
  { 851, 904,  2, 1L, 1L, 0L },    /* 3rd quadrant dir. 0, speed 1 */
  { 815, 921,  2, 1L, 1L, 0L },
  { 779, 938,  2, 1L, 1L, 0L },
  { 743, 955,  2, 1L, 1L, 0L },
  { 707, 972,  2, 1L, 1L, 0L },
  { 671, 989,  2, 1L, 1L, 0L },
  { 621, 996,  2, 1L, 1L, 0L },
  { 571, 1003, 2, 1L, 1L, 0L },
  { 521, 1010, 2, 1L, 1L, 0L },
  { 471, 1017, 2, 1L, 1L, 0L },
  { 421, 1023, 2, 1L, 1L, 0L },
  { 380, 1017, 2, 1L, 1L, 0L },
  { 339, 1010, 2, 1L, 1L, 0L },
  { 297, 1003, 2, 1L, 1L, 0L },
  { 256, 996,  2, 1L, 1L, 0L },
  { 215, 989,  2, 1L, 1L, 0L },
  { 179, 972,  2, 1L, 1L, 0L },
  { 143, 955,  2, 1L, 1L, 0L },
  { 107, 938,  2, 1L, 1L, 0L },
  { 71,  921,  2, 1L, 1L, 0L },
  { 35,  904,  2, 1L, 1L, 0L },
  { 0,   887,  1, 1L, 1L, 0L },
  { 43,  843,  1, 1L, 1L, 0L },
  { 86,  800,  1, 1L, 1L, 0L },
  { 129, 757,  1, 1L, 1L, 0L },
  { 172, 714,  1, 1L, 1L, 0L },
  { 215, 671,  1, 1L, 1L, 0L },
  { 256, 621,  1, 1L, 1L, 0L },
  { 297, 571,  1, 1L, 1L, 0L },
  { 339, 521,  1, 1L, 1L, 0L },
  { 380, 471,  1, 1L, 1L, 0L },
  { 421, 421,  1, 1L, 1L, 0L },
                                        
  { 462,  386, 1, 1L, 1L, 0L },    /* 4th quadrant dir. 0, speed 1 */
  { 504,  352, 1, 1L, 1L, 0L },
  { 546,  318, 1, 1L, 1L, 0L },
  { 587,  284, 1, 1L, 1L, 0L },
  { 629,  250, 1, 1L, 1L, 0L },
  { 671,  215, 1, 1L, 1L, 0L },
  { 714,  172, 1, 1L, 1L, 0L },
  { 757,  129, 1, 1L, 1L, 0L },
  { 800,  86,  1, 1L, 1L, 0L },
  { 843,  43,  1, 1L, 1L, 0L },
  { 887,  0,   0, 1L, 1L, 0L },
  { 907,  43,  0, 1L, 1L, 0L },
  { 928,  86,  0, 1L, 1L, 0L },
  { 948,  129, 0, 1L, 1L, 0L },
  { 969,  172, 0, 1L, 1L, 0L },
  { 989,  215, 0, 1L, 1L, 0L },
  { 995,  250, 0, 1L, 1L, 0L },
  { 1001, 284, 0, 1L, 1L, 0L },
  { 1006, 318, 0, 1L, 1L, 0L },
  { 1012, 352, 0, 1L, 1L, 0L },
  { 1018, 386, 0, 1L, 1L, 0L },
  { 1023, 421, 0, 1L, 1L, 0L },
  { 1017, 471, 0, 1L, 1L, 0L },
  { 1010, 521, 0, 1L, 1L, 0L },
  { 1003, 571, 0, 1L, 1L, 0L },
  { 996,  621, 0, 1L, 1L, 0L },
  { 989,  671, 0, 1L, 1L, 0L },
  { 969,  714, 0, 1L, 1L, 0L },
  { 948,  757, 0, 1L, 1L, 0L },
  { 928,  800, 0, 1L, 1L, 0L },
  { 907,  843, 0, 1L, 1L, 0L },
  { 887,  887, 0, 1L, 1L, 0L } },
                                        
{ { 851, 904,  0, 1L, 1L, 0L },    /* 1st quadrant dir. 1, speed 1 */
  { 815, 921,  0, 1L, 1L, 0L },
  { 779, 938,  0, 1L, 1L, 0L },
  { 743, 955,  0, 1L, 1L, 0L },
  { 707, 972,  0, 1L, 1L, 0L },
  { 671, 989,  0, 1L, 1L, 0L },
  { 621, 996,  0, 1L, 1L, 0L },
  { 571, 1003, 0, 1L, 1L, 0L },
  { 521, 1010, 0, 1L, 1L, 0L },
  { 471, 1017, 0, 1L, 1L, 0L },
  { 421, 1023, 0, 1L, 1L, 0L },
  { 380, 1017, 0, 1L, 1L, 0L },
  { 339, 1010, 0, 1L, 1L, 0L },
  { 297, 1003, 0, 1L, 1L, 0L },
  { 256, 996,  0, 1L, 1L, 0L },
  { 215, 989,  0, 1L, 1L, 0L },
  { 179, 972,  0, 1L, 1L, 0L },
  { 143, 955,  0, 1L, 1L, 0L },
  { 107, 938,  0, 1L, 1L, 0L },
  { 71,  921,  0, 1L, 1L, 0L },
  { 35,  904,  0, 1L, 1L, 0L },
  { 0,   887,  3, 1L, 1L, 0L },
  { 43,  843,  3, 1L, 1L, 0L },
  { 86,  800,  3, 1L, 1L, 0L },
  { 129, 757,  3, 1L, 1L, 0L },
  { 172, 714,  3, 1L, 1L, 0L },
  { 215, 671,  3, 1L, 1L, 0L },
  { 256, 621,  3, 1L, 1L, 0L },
  { 297, 571,  3, 1L, 1L, 0L },
  { 339, 521,  3, 1L, 1L, 0L },
  { 380, 471,  3, 1L, 1L, 0L },
  { 421, 421,  3, 1L, 1L, 0L },
                                        
  { 462,  386, 3, 1L, 1L, 0L },    /* 2nd quadrant dir. 1, speed 1 */
  { 504,  352, 3, 1L, 1L, 0L },
  { 546,  318, 3, 1L, 1L, 0L },
  { 587,  284, 3, 1L, 1L, 0L },
  { 629,  250, 3, 1L, 1L, 0L },
  { 671,  215, 3, 1L, 1L, 0L },
  { 714,  172, 3, 1L, 1L, 0L },
  { 757,  129, 3, 1L, 1L, 0L },
  { 800,  86,  3, 1L, 1L, 0L },
  { 843,  43,  3, 1L, 1L, 0L },
  { 887,  0,   3, 1L, 1L, 0L },
  { 907,  43,  2, 1L, 1L, 0L },
  { 928,  86,  2, 1L, 1L, 0L },
  { 948,  129, 2, 1L, 1L, 0L },
  { 969,  172, 2, 1L, 1L, 0L },
  { 989,  215, 2, 1L, 1L, 0L },
  { 995,  250, 2, 1L, 1L, 0L },
  { 1001, 284, 2, 1L, 1L, 0L },
  { 1006, 318, 2, 1L, 1L, 0L },
  { 1012, 352, 2, 1L, 1L, 0L },
  { 1018, 386, 2, 1L, 1L, 0L },
  { 1023, 421, 2, 1L, 1L, 0L },
  { 1017, 471, 2, 1L, 1L, 0L },
  { 1010, 521, 2, 1L, 1L, 0L },
  { 1003, 571, 2, 1L, 1L, 0L },
  { 996,  621, 2, 1L, 1L, 0L },
  { 989,  671, 2, 1L, 1L, 0L },
  { 969,  714, 2, 1L, 1L, 0L },
  { 948,  757, 2, 1L, 1L, 0L },
  { 928,  800, 2, 1L, 1L, 0L },
  { 907,  843, 2, 1L, 1L, 0L },
  { 887,  887, 2, 1L, 1L, 0L },
                                        
  { 851, 904,  2, 1L, 1L, 0L },    /* 3rd quadrant dir. 1, speed 1 */
  { 815, 921,  2, 1L, 1L, 0L },
  { 779, 938,  2, 1L, 1L, 0L },
  { 743, 955,  2, 1L, 1L, 0L },
  { 707, 972,  2, 1L, 1L, 0L },
  { 671, 989,  2, 1L, 1L, 0L },
  { 621, 996,  2, 1L, 1L, 0L },
  { 571, 1003, 2, 1L, 1L, 0L },
  { 521, 1010, 2, 1L, 1L, 0L },
  { 471, 1017, 2, 1L, 1L, 0L },
  { 421, 1023, 2, 1L, 1L, 0L },
  { 380, 1017, 2, 1L, 1L, 0L },
  { 339, 1010, 2, 1L, 1L, 0L },
  { 297, 1003, 2, 1L, 1L, 0L },
  { 256, 996,  2, 1L, 1L, 0L },
  { 215, 989,  2, 1L, 1L, 0L },
  { 179, 972,  2, 1L, 1L, 0L },
  { 143, 955,  2, 1L, 1L, 0L },
  { 107, 938,  2, 1L, 1L, 0L },
  { 71,  921,  2, 1L, 1L, 0L },
  { 35,  904,  2, 1L, 1L, 0L },
  { 0,   887,  1, 1L, 1L, 0L },
  { 43,  843,  1, 1L, 1L, 0L },
  { 86,  800,  1, 1L, 1L, 0L },
  { 129, 757,  1, 1L, 1L, 0L },
  { 172, 714,  1, 1L, 1L, 0L },
  { 215, 671,  1, 1L, 1L, 0L },
  { 256, 621,  1, 1L, 1L, 0L },
  { 297, 571,  1, 1L, 1L, 0L },
  { 339, 521,  1, 1L, 1L, 0L },
  { 380, 471,  1, 1L, 1L, 0L },
  { 421, 421,  1, 1L, 1L, 0L },
                                        
  { 462,  386, 1, 1L, 1L, 0L },    /* 4th quadrant dir. 1, speed 1 */
  { 504,  352, 1, 1L, 1L, 0L },
  { 546,  318, 1, 1L, 1L, 0L },
  { 587,  284, 1, 1L, 1L, 0L },
  { 629,  250, 1, 1L, 1L, 0L },
  { 671,  215, 1, 1L, 1L, 0L },
  { 714,  172, 1, 1L, 1L, 0L },
  { 757,  129, 1, 1L, 1L, 0L },
  { 800,  86,  1, 1L, 1L, 0L },
  { 843,  43,  1, 1L, 1L, 0L },
  { 887,  0,   0, 1L, 1L, 0L },
  { 907,  43,  0, 1L, 1L, 0L },
  { 928,  86,  0, 1L, 1L, 0L },
  { 948,  129, 0, 1L, 1L, 0L },
  { 969,  172, 0, 1L, 1L, 0L },
  { 989,  215, 0, 1L, 1L, 0L },
  { 995,  250, 0, 1L, 1L, 0L },
  { 1001, 284, 0, 1L, 1L, 0L },
  { 1006, 318, 0, 1L, 1L, 0L },
  { 1012, 352, 0, 1L, 1L, 0L },
  { 1018, 386, 0, 1L, 1L, 0L },
  { 1023, 421, 0, 1L, 1L, 0L },
  { 1017, 471, 0, 1L, 1L, 0L },
  { 1010, 521, 0, 1L, 1L, 0L },
  { 1003, 571, 0, 1L, 1L, 0L },
  { 996,  621, 0, 1L, 1L, 0L },
  { 989,  671, 0, 1L, 1L, 0L },
  { 969,  714, 0, 1L, 1L, 0L },
  { 948,  757, 0, 1L, 1L, 0L },
  { 928,  800, 0, 1L, 1L, 0L },
  { 907,  843, 0, 1L, 1L, 0L },
  { 887,  887, 0, 1L, 1L, 0L } } } }
};

/* ------------ High Precision Tables ------------ */

/* Default PWM Table: SINE on CVP, COSINE on CHP */

const struct rism_pwmhi_t R_ISM_A_HIPWMTABLE1 = {

  /*  CVP     CHP   CQI   IVR IHR */

{ { 0x000L, 0x3FFL, 0x3L, 1L, 1L, 0L },    /* 1st quadrant */
  { 0x00CL, 0x3FEL, 0x3L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x3L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x3L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x3L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x3L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x3L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x3L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x3L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x3L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x3L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x3L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x3L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x3L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x3L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x3L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x3L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x3L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x3L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x3L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x3L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x3L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x3L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x3L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x3L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x3L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x3L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x3L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x3L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x3L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x3L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x3L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x3L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x3L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x3L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x3L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x3L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x3L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x3L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x3L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x3L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x3L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x3L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x3L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x3L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x3L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x3L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x3L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x3L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x3L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x3L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x3L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x3L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x3L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x3L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x3L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x3L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x3L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x3L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x3L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x3L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x3L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x3L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x3L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x3L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x3L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x3L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x3L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x3L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x3L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x3L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x3L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x3L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x3L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x3L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x3L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x3L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x3L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x3L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x3L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x3L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x3L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x3L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x3L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x3L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x3L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x3L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x3L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x3L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x3L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x3L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x3L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x3L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x3L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x3L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x3L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x3L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x3L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x3L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x3L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x3L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x3L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x3L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x3L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x3L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x3L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x3L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x3L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x3L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x3L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x3L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x3L, 1L, 1L, 0L },
  { 0x3FEL, 0x00CL, 0x3L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x2L, 1L, 1L, 0L },    /* 2nd quadrant */
  { 0x3FEL, 0x00CL, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x2L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x2L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x2L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x2L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x2L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x2L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x2L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x2L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x2L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x2L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x2L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x2L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x2L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x2L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x2L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x2L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x2L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x2L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x2L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x2L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x2L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x2L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x2L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x2L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x2L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x2L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x2L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x2L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x2L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x2L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x2L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x2L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x2L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x2L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x2L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x2L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x2L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x2L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x2L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x2L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x2L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x2L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x2L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x2L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x2L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x2L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x2L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x2L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x2L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x2L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x2L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x2L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x2L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x2L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x2L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x2L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x2L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x2L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x2L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x2L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x2L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x2L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x2L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x2L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x2L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x2L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x2L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x2L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x2L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x2L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x2L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x2L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x2L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x2L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x2L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x2L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x2L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x2L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x2L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x2L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x2L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x2L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x2L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x2L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x2L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x2L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x2L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x2L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x2L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x2L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x2L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x2L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x2L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x2L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x2L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x2L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x2L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x2L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x2L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x2L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x2L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x2L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x2L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x2L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x2L, 1L, 1L, 0L },
  { 0x00CL, 0x3FEL, 0x2L, 1L, 1L, 0L },

  { 0x000L, 0x3FFL, 0x1L, 1L, 1L, 0L },    /* 3rd quadrant */
  { 0x00CL, 0x3FEL, 0x1L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x1L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x1L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x1L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x1L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x1L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x1L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x1L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x1L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x1L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x1L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x1L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x1L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x1L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x1L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x1L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x1L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x1L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x1L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x1L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x1L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x1L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x1L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x1L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x1L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x1L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x1L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x1L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x1L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x1L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x1L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x1L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x1L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x1L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x1L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x1L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x1L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x1L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x1L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x1L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x1L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x1L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x1L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x1L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x1L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x1L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x1L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x1L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x1L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x1L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x1L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x1L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x1L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x1L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x1L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x1L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x1L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x1L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x1L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x1L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x1L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x1L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x1L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x1L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x1L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x1L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x1L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x1L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x1L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x1L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x1L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x1L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x1L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x1L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x1L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x1L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x1L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x1L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x1L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x1L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x1L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x1L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x1L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x1L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x1L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x1L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x1L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x1L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x1L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x1L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x1L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x1L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x1L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x1L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x1L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x1L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x1L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x1L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x1L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x1L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x1L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x1L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x1L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x1L, 1L, 1L, 0L },
  { 0x3FEL, 0x00CL, 0x1L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x0L, 1L, 1L, 0L },    /* 4th quadrant */
  { 0x3FEL, 0x00CL, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x0L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x0L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x0L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x0L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x0L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x0L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x0L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x0L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x0L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x0L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x0L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x0L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x0L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x0L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x0L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x0L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x0L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x0L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x0L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x0L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x0L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x0L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x0L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x0L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x0L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x0L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x0L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x0L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x0L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x0L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x0L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x0L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x0L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x0L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x0L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x0L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x0L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x0L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x0L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x0L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x0L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x0L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x0L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x0L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x0L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x0L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x0L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x0L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x0L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x0L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x0L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x0L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x0L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x0L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x0L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x0L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x0L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x0L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x0L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x0L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x0L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x0L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x0L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x0L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x0L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x0L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x0L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x0L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x0L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x0L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x0L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x0L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x0L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x0L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x0L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x0L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x0L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x0L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x0L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x0L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x0L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x0L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x0L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x0L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x0L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x0L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x0L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x0L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x0L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x0L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x0L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x0L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x0L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x0L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x0L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x0L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x0L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x0L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x0L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x0L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x0L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x0L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x0L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x0L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x0L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x00CL, 0x3FEL, 0x0L, 1L, 1L, 0L } }
};

/* Alternative PWM Table: COSINE on CVP, SINE on CHP */

const struct rism_pwmhi_t R_ISM_A_HIPWMTABLE2 = {

  /*  CVP     CHP   CQI   IVR IHR */

{ { 0x3FFL, 0x000L, 0x3L, 1L, 1L, 0L },    /* 1st quadrant */
  { 0x3FEL, 0x00CL, 0x3L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x3L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x3L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x3L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x3L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x3L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x3L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x3L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x3L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x3L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x3L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x3L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x3L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x3L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x3L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x3L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x3L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x3L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x3L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x3L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x3L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x3L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x3L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x3L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x3L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x3L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x3L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x3L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x3L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x3L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x3L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x3L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x3L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x3L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x3L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x3L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x3L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x3L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x3L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x3L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x3L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x3L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x3L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x3L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x3L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x3L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x3L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x3L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x3L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x3L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x3L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x3L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x3L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x3L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x3L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x3L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x3L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x3L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x3L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x3L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x3L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x3L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x3L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x3L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x3L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x3L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x3L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x3L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x3L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x3L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x3L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x3L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x3L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x3L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x3L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x3L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x3L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x3L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x3L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x3L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x3L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x3L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x3L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x3L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x3L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x3L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x3L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x3L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x3L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x3L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x3L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x3L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x3L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x3L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x3L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x3L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x3L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x3L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x3L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x3L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x3L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x3L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x3L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x3L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x3L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x3L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x3L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x3L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x3L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x3L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x3L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x3L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x3L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x3L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x3L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x3L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x3L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x3L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x3L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x3L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x3L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x3L, 1L, 1L, 0L },
  { 0x00CL, 0x3FEL, 0x3L, 1L, 1L, 0L },

  { 0x000L, 0x3FFL, 0x2L, 1L, 1L, 0L },    /* 2nd quadrant */
  { 0x00CL, 0x3FEL, 0x2L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x2L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x2L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x2L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x2L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x2L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x2L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x2L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x2L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x2L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x2L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x2L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x2L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x2L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x2L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x2L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x2L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x2L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x2L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x2L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x2L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x2L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x2L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x2L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x2L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x2L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x2L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x2L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x2L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x2L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x2L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x2L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x2L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x2L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x2L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x2L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x2L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x2L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x2L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x2L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x2L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x2L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x2L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x2L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x2L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x2L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x2L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x2L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x2L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x2L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x2L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x2L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x2L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x2L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x2L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x2L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x2L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x2L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x2L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x2L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x2L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x2L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x2L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x2L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x2L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x2L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x2L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x2L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x2L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x2L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x2L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x2L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x2L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x2L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x2L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x2L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x2L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x2L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x2L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x2L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x2L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x2L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x2L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x2L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x2L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x2L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x2L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x2L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x2L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x2L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x2L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x2L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x2L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x2L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x2L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x2L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x2L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x2L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x2L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x2L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x2L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x2L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x2L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x2L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x2L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x2L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x2L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x2L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x2L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x2L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x2L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x2L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x2L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x2L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x2L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x2L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x2L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x2L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x2L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x2L, 1L, 1L, 0L },
  { 0x3FEL, 0x00CL, 0x2L, 1L, 1L, 0L },

  { 0x3FFL, 0x000L, 0x1L, 1L, 1L, 0L },    /* 3rd quadrant */
  { 0x3FEL, 0x00CL, 0x1L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x1L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x1L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x1L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x1L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x1L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x1L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x1L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x1L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x1L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x1L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x1L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x1L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x1L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x1L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x1L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x1L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x1L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x1L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x1L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x1L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x1L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x1L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x1L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x1L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x1L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x1L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x1L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x1L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x1L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x1L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x1L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x1L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x1L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x1L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x1L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x1L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x1L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x1L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x1L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x1L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x1L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x1L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x1L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x1L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x1L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x1L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x1L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x1L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x1L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x1L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x1L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x1L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x1L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x1L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x1L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x1L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x1L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x1L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x1L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x1L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x1L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x1L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x1L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x1L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x1L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x1L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x1L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x1L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x1L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x1L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x1L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x1L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x1L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x1L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x1L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x1L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x1L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x1L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x1L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x1L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x1L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x1L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x1L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x1L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x1L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x1L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x1L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x1L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x1L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x1L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x1L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x1L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x1L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x1L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x1L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x1L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x1L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x1L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x1L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x1L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x1L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x1L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x1L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x1L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x1L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x1L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x1L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x1L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x1L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x1L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x1L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x1L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x1L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x1L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x1L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x1L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x1L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x1L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x1L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x1L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x1L, 1L, 1L, 0L },
  { 0x00CL, 0x3FEL, 0x1L, 1L, 1L, 0L },

  { 0x000L, 0x3FFL, 0x0L, 1L, 1L, 0L },    /* 4th quadrant */
  { 0x00CL, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x019L, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x025L, 0x3FEL, 0x0L, 1L, 1L, 0L },
  { 0x032L, 0x3FDL, 0x0L, 1L, 1L, 0L },
  { 0x03EL, 0x3FDL, 0x0L, 1L, 1L, 0L },
  { 0x04BL, 0x3FCL, 0x0L, 1L, 1L, 0L },
  { 0x057L, 0x3FBL, 0x0L, 1L, 1L, 0L },
  { 0x064L, 0x3FAL, 0x0L, 1L, 1L, 0L },
  { 0x070L, 0x3F8L, 0x0L, 1L, 1L, 0L },
  { 0x07DL, 0x3F7L, 0x0L, 1L, 1L, 0L },
  { 0x089L, 0x3F5L, 0x0L, 1L, 1L, 0L },
  { 0x096L, 0x3F3L, 0x0L, 1L, 1L, 0L },
  { 0x0A2L, 0x3F2L, 0x0L, 1L, 1L, 0L },
  { 0x0AEL, 0x3EFL, 0x0L, 1L, 1L, 0L },
  { 0x0BBL, 0x3EDL, 0x0L, 1L, 1L, 0L },
  { 0x0C7L, 0x3EBL, 0x0L, 1L, 1L, 0L },
  { 0x0D3L, 0x3E8L, 0x0L, 1L, 1L, 0L },
  { 0x0E0L, 0x3E6L, 0x0L, 1L, 1L, 0L },
  { 0x0ECL, 0x3E3L, 0x0L, 1L, 1L, 0L },
  { 0x0F8L, 0x3E0L, 0x0L, 1L, 1L, 0L },
  { 0x104L, 0x3DDL, 0x0L, 1L, 1L, 0L },
  { 0x110L, 0x3D9L, 0x0L, 1L, 1L, 0L },
  { 0x11CL, 0x3D6L, 0x0L, 1L, 1L, 0L },
  { 0x128L, 0x3D2L, 0x0L, 1L, 1L, 0L },
  { 0x134L, 0x3CFL, 0x0L, 1L, 1L, 0L },
  { 0x140L, 0x3CBL, 0x0L, 1L, 1L, 0L },
  { 0x14CL, 0x3C7L, 0x0L, 1L, 1L, 0L },
  { 0x158L, 0x3C3L, 0x0L, 1L, 1L, 0L },
  { 0x164L, 0x3BEL, 0x0L, 1L, 1L, 0L },
  { 0x170L, 0x3BAL, 0x0L, 1L, 1L, 0L },
  { 0x17BL, 0x3B5L, 0x0L, 1L, 1L, 0L },
  { 0x187L, 0x3B1L, 0x0L, 1L, 1L, 0L },
  { 0x193L, 0x3ACL, 0x0L, 1L, 1L, 0L },
  { 0x19EL, 0x3A7L, 0x0L, 1L, 1L, 0L },
  { 0x1AAL, 0x3A2L, 0x0L, 1L, 1L, 0L },
  { 0x1B5L, 0x39CL, 0x0L, 1L, 1L, 0L },
  { 0x1C0L, 0x397L, 0x0L, 1L, 1L, 0L },
  { 0x1CBL, 0x391L, 0x0L, 1L, 1L, 0L },
  { 0x1D7L, 0x38CL, 0x0L, 1L, 1L, 0L },
  { 0x1E2L, 0x386L, 0x0L, 1L, 1L, 0L },
  { 0x1EDL, 0x380L, 0x0L, 1L, 1L, 0L },
  { 0x1F8L, 0x37AL, 0x0L, 1L, 1L, 0L },
  { 0x203L, 0x373L, 0x0L, 1L, 1L, 0L },
  { 0x20DL, 0x36DL, 0x0L, 1L, 1L, 0L },
  { 0x218L, 0x366L, 0x0L, 1L, 1L, 0L },
  { 0x223L, 0x360L, 0x0L, 1L, 1L, 0L },
  { 0x22DL, 0x359L, 0x0L, 1L, 1L, 0L },
  { 0x238L, 0x352L, 0x0L, 1L, 1L, 0L },
  { 0x242L, 0x34BL, 0x0L, 1L, 1L, 0L },
  { 0x24DL, 0x344L, 0x0L, 1L, 1L, 0L },
  { 0x257L, 0x33DL, 0x0L, 1L, 1L, 0L },
  { 0x261L, 0x335L, 0x0L, 1L, 1L, 0L },
  { 0x26BL, 0x32EL, 0x0L, 1L, 1L, 0L },
  { 0x275L, 0x326L, 0x0L, 1L, 1L, 0L },
  { 0x27FL, 0x31EL, 0x0L, 1L, 1L, 0L },
  { 0x288L, 0x316L, 0x0L, 1L, 1L, 0L },
  { 0x292L, 0x30EL, 0x0L, 1L, 1L, 0L },
  { 0x29CL, 0x306L, 0x0L, 1L, 1L, 0L },
  { 0x2A5L, 0x2FEL, 0x0L, 1L, 1L, 0L },
  { 0x2AFL, 0x2F5L, 0x0L, 1L, 1L, 0L },
  { 0x2B8L, 0x2EDL, 0x0L, 1L, 1L, 0L },
  { 0x2C1L, 0x2E4L, 0x0L, 1L, 1L, 0L },
  { 0x2CAL, 0x2DCL, 0x0L, 1L, 1L, 0L },
  { 0x2D3L, 0x2D3L, 0x0L, 1L, 1L, 0L },
  { 0x2DCL, 0x2CAL, 0x0L, 1L, 1L, 0L },
  { 0x2E4L, 0x2C1L, 0x0L, 1L, 1L, 0L },
  { 0x2EDL, 0x2B8L, 0x0L, 1L, 1L, 0L },
  { 0x2F5L, 0x2AFL, 0x0L, 1L, 1L, 0L },
  { 0x2FEL, 0x2A5L, 0x0L, 1L, 1L, 0L },
  { 0x306L, 0x29CL, 0x0L, 1L, 1L, 0L },
  { 0x30EL, 0x292L, 0x0L, 1L, 1L, 0L },
  { 0x316L, 0x288L, 0x0L, 1L, 1L, 0L },
  { 0x31EL, 0x27FL, 0x0L, 1L, 1L, 0L },
  { 0x326L, 0x275L, 0x0L, 1L, 1L, 0L },
  { 0x32EL, 0x26BL, 0x0L, 1L, 1L, 0L },
  { 0x335L, 0x261L, 0x0L, 1L, 1L, 0L },
  { 0x33DL, 0x257L, 0x0L, 1L, 1L, 0L },
  { 0x344L, 0x24DL, 0x0L, 1L, 1L, 0L },
  { 0x34BL, 0x242L, 0x0L, 1L, 1L, 0L },
  { 0x352L, 0x238L, 0x0L, 1L, 1L, 0L },
  { 0x359L, 0x22DL, 0x0L, 1L, 1L, 0L },
  { 0x360L, 0x223L, 0x0L, 1L, 1L, 0L },
  { 0x366L, 0x218L, 0x0L, 1L, 1L, 0L },
  { 0x36DL, 0x20DL, 0x0L, 1L, 1L, 0L },
  { 0x373L, 0x203L, 0x0L, 1L, 1L, 0L },
  { 0x37AL, 0x1F8L, 0x0L, 1L, 1L, 0L },
  { 0x380L, 0x1EDL, 0x0L, 1L, 1L, 0L },
  { 0x386L, 0x1E2L, 0x0L, 1L, 1L, 0L },
  { 0x38CL, 0x1D7L, 0x0L, 1L, 1L, 0L },
  { 0x391L, 0x1CBL, 0x0L, 1L, 1L, 0L },
  { 0x397L, 0x1C0L, 0x0L, 1L, 1L, 0L },
  { 0x39CL, 0x1B5L, 0x0L, 1L, 1L, 0L },
  { 0x3A2L, 0x1AAL, 0x0L, 1L, 1L, 0L },
  { 0x3A7L, 0x19EL, 0x0L, 1L, 1L, 0L },
  { 0x3ACL, 0x193L, 0x0L, 1L, 1L, 0L },
  { 0x3B1L, 0x187L, 0x0L, 1L, 1L, 0L },
  { 0x3B5L, 0x17BL, 0x0L, 1L, 1L, 0L },
  { 0x3BAL, 0x170L, 0x0L, 1L, 1L, 0L },
  { 0x3BEL, 0x164L, 0x0L, 1L, 1L, 0L },
  { 0x3C3L, 0x158L, 0x0L, 1L, 1L, 0L },
  { 0x3C7L, 0x14CL, 0x0L, 1L, 1L, 0L },
  { 0x3CBL, 0x140L, 0x0L, 1L, 1L, 0L },
  { 0x3CFL, 0x134L, 0x0L, 1L, 1L, 0L },
  { 0x3D2L, 0x128L, 0x0L, 1L, 1L, 0L },
  { 0x3D6L, 0x11CL, 0x0L, 1L, 1L, 0L },
  { 0x3D9L, 0x110L, 0x0L, 1L, 1L, 0L },
  { 0x3DDL, 0x104L, 0x0L, 1L, 1L, 0L },
  { 0x3E0L, 0x0F8L, 0x0L, 1L, 1L, 0L },
  { 0x3E3L, 0x0ECL, 0x0L, 1L, 1L, 0L },
  { 0x3E6L, 0x0E0L, 0x0L, 1L, 1L, 0L },
  { 0x3E8L, 0x0D3L, 0x0L, 1L, 1L, 0L },
  { 0x3EBL, 0x0C7L, 0x0L, 1L, 1L, 0L },
  { 0x3EDL, 0x0BBL, 0x0L, 1L, 1L, 0L },
  { 0x3EFL, 0x0AEL, 0x0L, 1L, 1L, 0L },
  { 0x3F2L, 0x0A2L, 0x0L, 1L, 1L, 0L },
  { 0x3F3L, 0x096L, 0x0L, 1L, 1L, 0L },
  { 0x3F5L, 0x089L, 0x0L, 1L, 1L, 0L },
  { 0x3F7L, 0x07DL, 0x0L, 1L, 1L, 0L },
  { 0x3F8L, 0x070L, 0x0L, 1L, 1L, 0L },
  { 0x3FAL, 0x064L, 0x0L, 1L, 1L, 0L },
  { 0x3FBL, 0x057L, 0x0L, 1L, 1L, 0L },
  { 0x3FCL, 0x04BL, 0x0L, 1L, 1L, 0L },
  { 0x3FDL, 0x03EL, 0x0L, 1L, 1L, 0L },
  { 0x3FDL, 0x032L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x025L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x019L, 0x0L, 1L, 1L, 0L },
  { 0x3FEL, 0x00CL, 0x0L, 1L, 1L, 0L } }
};

/* Test PWM Table: fixed codings */

const struct rism_pwmhi_t R_ISM_A_HIPWMTABLE3 = {

  /*  CVP     CHP   CQI   IVR IHR */

{ { 0x000L, 0x1FFL, 0x0L, 1L, 1L, 0L },    /* 1st quadrant */
  { 0x001L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x002L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x003L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x004L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x005L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x006L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x007L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x008L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x009L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x00FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x010L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x011L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x012L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x013L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x014L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x015L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x016L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x017L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x018L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x019L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x01FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x020L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x021L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x022L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x023L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x024L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x025L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x026L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x027L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x028L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x029L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x02AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x02BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x02CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x02DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x02EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x02FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x030L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x031L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x032L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x033L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x034L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x035L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x036L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x037L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x038L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x039L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x03AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x03BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x03CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x03DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x03EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x03FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x040L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x041L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x042L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x043L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x044L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x045L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x046L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x047L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x048L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x049L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x04AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x04BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x04CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x04DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x04EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x04FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x050L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x051L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x052L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x053L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x054L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x055L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x056L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x057L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x058L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x059L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x05AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x05BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x05CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x05DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x05EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x05FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x060L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x061L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x062L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x063L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x064L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x065L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x066L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x067L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x068L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x069L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x06AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x06BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x06CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x06DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x06EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x06FL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x070L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x071L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x072L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x073L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x074L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x075L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x076L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x077L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x078L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x079L, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x07AL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x07BL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x07CL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x07DL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x07EL, 0x1FFL, 0x0L, 1L, 1L, 0L },
  { 0x07FL, 0x1FFL, 0x0L, 1L, 1L, 0L },

  { 0x100L, 0x1FFL, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant */
  { 0x101L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x102L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x103L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x104L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x105L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x106L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x107L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x108L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x109L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x10AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x10BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x10CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x10DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x10EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x10FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x110L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x111L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x112L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x113L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x114L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x115L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x116L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x117L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x118L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x119L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x11AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x11BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x11CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x11DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x11EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x11FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x120L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x121L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x122L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x123L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x124L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x125L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x126L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x127L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x128L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x129L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x12FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x130L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x131L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x132L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x133L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x134L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x135L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x136L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x137L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x138L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x139L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x13FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x140L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x141L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x142L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x143L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x144L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x145L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x146L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x147L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x148L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x149L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x14AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x14BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x14CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x14DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x14EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x14FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x150L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x151L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x152L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x153L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x154L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x155L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x156L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x157L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x158L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x159L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x15AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x15BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x15CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x15DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x15EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x15FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x160L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x161L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x162L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x163L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x164L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x165L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x166L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x167L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x168L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x169L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x16AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x16BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x16CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x16DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x16EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x16FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x170L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x171L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x172L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x173L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x174L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x175L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x176L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x177L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x178L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x179L, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x17AL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x17BL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x17CL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x17DL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x17EL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  { 0x17FL, 0x1FFL, 0x1L, 1L, 1L, 0L },
  
  { 0x200L, 0x1FFL, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant */
  { 0x201L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x202L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x203L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x204L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x205L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x206L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x207L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x208L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x209L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x20AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x20BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x20CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x20DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x20EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x20FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x210L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x211L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x212L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x213L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x214L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x215L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x216L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x217L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x218L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x219L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x21AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x21BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x21CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x21DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x21EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x21FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x220L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x221L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x222L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x223L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x224L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x225L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x226L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x227L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x228L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x229L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x22AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x22BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x22CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x22DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x22EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x22FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x230L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x231L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x232L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x233L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x234L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x235L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x236L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x237L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x238L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x239L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x23AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x23BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x23CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x23DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x23EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x23FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x240L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x241L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x242L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x243L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x244L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x245L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x246L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x247L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x248L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x249L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x24AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x24BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x24CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x24DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x24EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x24FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x250L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x251L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x252L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x253L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x254L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x255L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x256L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x257L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x258L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x259L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x25AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x25BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x25CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x25DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x25EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x25FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x260L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x261L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x262L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x263L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x264L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x265L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x266L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x267L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x268L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x269L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x26AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x26BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x26CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x26DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x26EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x26FL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x270L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x271L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x272L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x273L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x274L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x275L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x276L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x277L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x278L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x279L, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x27AL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x27BL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x27CL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x27DL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x27EL, 0x1FFL, 0x2L, 1L, 1L, 0L },
  { 0x27FL, 0x1FFL, 0x2L, 1L, 1L, 0L },

  { 0x300L, 0x1FFL, 0x3L, 1L, 1L, 0L },    /* 4th quadrant */
  { 0x301L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x302L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x303L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x304L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x305L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x306L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x307L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x308L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x309L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x30AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x30BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x30CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x30DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x30EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x30FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x310L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x311L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x312L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x313L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x314L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x315L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x316L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x317L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x318L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x319L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x31AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x31BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x31CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x31DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x31EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x31FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x320L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x321L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x322L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x323L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x324L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x325L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x326L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x327L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x328L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x329L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x32AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x32BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x32CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x32DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x32EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x32FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x330L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x331L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x332L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x333L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x334L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x335L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x336L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x337L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x338L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x339L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x33AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x33BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x33CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x33DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x33EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x33FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x340L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x341L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x342L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x343L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x344L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x345L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x346L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x347L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x348L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x349L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x34AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x34BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x34CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x34DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x34EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x34FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x350L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x351L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x352L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x353L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x354L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x355L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x356L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x357L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x358L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x359L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x35AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x35BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x35CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x35DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x35EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x35FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x360L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x361L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x362L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x363L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x364L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x365L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x366L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x367L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x368L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x369L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x36AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x36BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x36CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x36DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x36EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x36FL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x370L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x371L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x372L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x373L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x374L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x375L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x376L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x377L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x378L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x379L, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x37AL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x37BL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x37CL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x37DL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x37EL, 0x1FFL, 0x3L, 1L, 1L, 0L },
  { 0x37FL, 0x1FFL, 0x3L, 1L, 1L, 0L } }
                                      
};

/* Test PWM Table: alternate fixed codings */

const struct rism_pwmhi_t R_ISM_A_HIPWMTABLE4 = {

  /*  CVP     CHP   CQI   IVR IHR */

{ { 0x1FFL, 0x000L, 0x0L, 1L, 1L, 0L },    /* 1st quadrant */
  { 0x1FFL, 0x001L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x002L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x003L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x004L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x005L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x006L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x007L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x008L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x009L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x00FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x010L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x011L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x012L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x013L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x014L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x015L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x016L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x017L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x018L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x019L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x01FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x020L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x021L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x022L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x023L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x024L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x025L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x026L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x027L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x028L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x029L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x02AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x02BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x02CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x02DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x02EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x02FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x030L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x031L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x032L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x033L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x034L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x035L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x036L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x037L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x038L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x039L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x03AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x03BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x03CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x03DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x03EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x03FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x040L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x041L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x042L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x043L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x044L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x045L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x046L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x047L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x048L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x049L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x04AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x04BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x04CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x04DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x04EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x04FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x050L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x051L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x052L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x053L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x054L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x055L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x056L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x057L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x058L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x059L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x05AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x05BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x05CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x05DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x05EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x05FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x060L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x061L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x062L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x063L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x064L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x065L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x066L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x067L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x068L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x069L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x06AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x06BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x06CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x06DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x06EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x06FL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x070L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x071L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x072L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x073L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x074L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x075L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x076L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x077L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x078L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x079L, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x07AL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x07BL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x07CL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x07DL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x07EL, 0x0L, 1L, 1L, 0L },
  { 0x1FFL, 0x07FL, 0x0L, 1L, 1L, 0L },

  { 0x1FFL, 0x100L, 0x1L, 1L, 1L, 0L },    /* 2nd quadrant */
  { 0x1FFL, 0x101L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x102L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x103L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x104L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x105L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x106L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x107L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x108L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x109L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x10AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x10BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x10CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x10DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x10EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x10FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x110L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x111L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x112L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x113L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x114L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x115L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x116L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x117L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x118L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x119L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x11AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x11BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x11CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x11DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x11EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x11FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x120L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x121L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x122L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x123L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x124L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x125L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x126L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x127L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x128L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x129L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x12FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x130L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x131L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x132L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x133L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x134L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x135L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x136L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x137L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x138L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x139L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x13FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x140L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x141L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x142L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x143L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x144L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x145L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x146L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x147L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x148L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x149L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x14AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x14BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x14CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x14DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x14EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x14FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x150L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x151L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x152L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x153L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x154L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x155L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x156L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x157L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x158L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x159L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x15AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x15BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x15CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x15DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x15EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x15FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x160L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x161L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x162L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x163L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x164L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x165L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x166L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x167L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x168L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x169L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x16AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x16BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x16CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x16DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x16EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x16FL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x170L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x171L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x172L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x173L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x174L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x175L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x176L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x177L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x178L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x179L, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x17AL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x17BL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x17CL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x17DL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x17EL, 0x1L, 1L, 1L, 0L },
  { 0x1FFL, 0x17FL, 0x1L, 1L, 1L, 0L },
  
  { 0x1FFL, 0x200L, 0x2L, 1L, 1L, 0L },    /* 3rd quadrant */
  { 0x1FFL, 0x201L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x202L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x203L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x204L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x205L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x206L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x207L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x208L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x209L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x20AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x20BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x20CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x20DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x20EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x20FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x210L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x211L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x212L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x213L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x214L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x215L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x216L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x217L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x218L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x219L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x21AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x21BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x21CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x21DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x21EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x21FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x220L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x221L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x222L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x223L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x224L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x225L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x226L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x227L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x228L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x229L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x22AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x22BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x22CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x22DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x22EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x22FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x230L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x231L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x232L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x233L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x234L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x235L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x236L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x237L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x238L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x239L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x23AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x23BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x23CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x23DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x23EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x23FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x240L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x241L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x242L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x243L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x244L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x245L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x246L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x247L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x248L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x249L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x24AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x24BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x24CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x24DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x24EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x24FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x250L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x251L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x252L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x253L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x254L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x255L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x256L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x257L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x258L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x259L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x25AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x25BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x25CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x25DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x25EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x25FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x260L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x261L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x262L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x263L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x264L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x265L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x266L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x267L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x268L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x269L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x26AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x26BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x26CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x26DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x26EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x26FL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x270L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x271L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x272L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x273L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x274L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x275L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x276L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x277L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x278L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x279L, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x27AL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x27BL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x27CL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x27DL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x27EL, 0x2L, 1L, 1L, 0L },
  { 0x1FFL, 0x27FL, 0x2L, 1L, 1L, 0L },

  { 0x1FFL, 0x300L, 0x3L, 1L, 1L, 0L },    /* 4th quadrant */
  { 0x1FFL, 0x301L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x302L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x303L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x304L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x305L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x306L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x307L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x308L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x309L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x30AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x30BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x30CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x30DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x30EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x30FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x310L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x311L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x312L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x313L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x314L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x315L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x316L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x317L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x318L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x319L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x31AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x31BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x31CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x31DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x31EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x31FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x320L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x321L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x322L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x323L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x324L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x325L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x326L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x327L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x328L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x329L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x32AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x32BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x32CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x32DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x32EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x32FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x330L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x331L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x332L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x333L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x334L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x335L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x336L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x337L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x338L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x339L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x33AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x33BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x33CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x33DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x33EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x33FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x340L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x341L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x342L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x343L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x344L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x345L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x346L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x347L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x348L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x349L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x34AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x34BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x34CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x34DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x34EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x34FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x350L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x351L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x352L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x353L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x354L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x355L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x356L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x357L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x358L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x359L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x35AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x35BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x35CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x35DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x35EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x35FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x360L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x361L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x362L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x363L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x364L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x365L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x366L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x367L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x368L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x369L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x36AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x36BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x36CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x36DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x36EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x36FL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x370L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x371L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x372L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x373L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x374L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x375L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x376L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x377L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x378L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x379L, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x37AL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x37BL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x37CL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x37DL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x37EL, 0x3L, 1L, 1L, 0L },
  { 0x1FFL, 0x37FL, 0x3L, 1L, 1L, 0L } }
                                      
};

/*============================================================================


          ZPD Tables


==============================================================================*/


/* Table 0: First try on SCIT. Rotate and measure on every quadrant */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE0 =
  {
    11,    /* Table contains 10 entries */
    {
      /* Acceleration and positioning of the motor */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),             /* Move to SINP and wait for 2 ms */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),             /* Move to COSM and wait for 2 ms */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),         /* Position to SINM and wait for 2 ms */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),         /* Position to COSP and wait for 2 ms */

      /* Quadrant 0 */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),             /* Move to SINP and wait for 2 ms */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) |
      ( 0x1F << R_ISM_CZD_P ),                            /* Measure at COSM */

      /* Quadrant 3 */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),             /* Move to COSM and wait for 2 ms */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) |
      ( 0x1F << R_ISM_CZD_P ),                            /* Measure at SINM */

      /* Quadrant 2 */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),             /* Move to SINM and wait for 2 ms */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) |
      ( 0x1F << R_ISM_CZD_P ),                            /* Measure at COSP */

      /* Quadrant 1 */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),             /* Move to COSP and wait for 2 ms */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) |
      ( 0x1F << R_ISM_CZD_P )                             /* Measure at SINP */
    }
  };

/* Table 1: First try on Device with Conti Motor. Rotate and measure on one quadrant only.
            Experiments showed that on every quadrant, the shape of the pulse is 
            slightly different, what makes it difficult to adjust the ZPD to work
            in all quadrants. This difficult work can be done only by many experiments,
            for which I do not have enough time right now. */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE1 =
  {
    8 + 3,    /* Table contains 12 entries */
    {
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),        /* Initial Rotation: Position to SINP  */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),        /* Initial Rotation: Position to COSM  */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),         /* Initial Rotation: Position to SINM  */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),         /* Initial Rotation: Position to COSP  */

      /* Quadrant 0 */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x01 << R_ISM_CZD_P ),                       /* Move to SINP and wait */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) |
      ( 0x0E << R_ISM_CZD_P ),                            /* Measure at COSM */

      /* Quadrant 3 */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x01 << R_ISM_CZD_P ),                      /* Move to COSM and wait */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) |
      ( 0x0E << R_ISM_CZD_P ),                           /*  Measure at SINM */

      /* Quadrant 2 */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x01 << R_ISM_CZD_P ),                      /* Move to SINM and wait */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) |
      ( 0x0E << R_ISM_CZD_P ),                          /*  Measure at COSP */

      /* Quadrant 1 */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x01 << R_ISM_CZD_P ),                      /* Move to COSP and wait */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) |
      ( 0x0E << R_ISM_CZD_P )                             /* Measure at SINP */
    }
  };

/* Table 2a: Table for China Motor.
   Measurements in all Quadrants.
*/

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE2a =
  {
    19,    /* Table contains 20 entries */
    {
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),        /* Initial Rotation: Position to SINP  */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),        /* Initial Rotation: Position to COSM  */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),         /* Initial Rotation: Position to SINM  */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),         /* Initial Rotation: Position to COSP  */

      /* Measurement in Quadrant 0 */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),                                /* Move to SINP */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),    /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                            /* Measure at COSM */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P ),                        /* End Measure at COSM */

      /* Measurement in Quadrant 3 */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),                               /* Move to COSM */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),   /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                           /*  Measure at SINM */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P ),                        /* End Measure at SINM */

      /* Measurement in Quadrant 2 */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),                               /* Move to SINM */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),   /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                          /*  Measure at COSP */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P ),                       /* End Measure at COSP */

      /* Measurement in Quadrant 1 */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),                              /* Move to COSP */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),  /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                           /* Measure at SINP */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P )                         /*End Measure at SINP */
    }
  };

/* Table 2b: Investigation Table for JCI Motor.
   Measurements in one Quadrant, Maximum speed @ 1kHz stepping,
   in order to get the full shape of the induction.
*/

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE2b =
  {
    10,    /* Table contains 11 entries */
    {
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P ),        /* Initial Rotation: Position to SINP  */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P ),        /* Initial Rotation: Position to COSM  */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P ),         /* Initial Rotation: Position to SINM  */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P ),         /* Initial Rotation: Position to COSP  */

      /* Measurement in Quadrant 0 */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),                                /* Move to SINP */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),    /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                            /* Measure at COSM */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x08 << R_ISM_CZD_P ),                        /* End Measure at COSM */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P ),        /* Complete Rotation: Position to COSM  */

      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P ),        /* Complete Rotation: Position to SINM  */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P )         /* Complete Rotation: Position to COSP  */
    }
  };



/* Table 2c: Final Algorithm Table for Sonceboz Motor.
   Measurements in four quadrants with microstep movement (14.4 ms per rotation)
   To be executed at 0.45 ms per step of the ZPD table, 3.6 ms per quadrant
*/
#define R_ISM_ZMT_TIME_SON        (0)
#define R_ISM_WAI_TIME_SON_RF     (0)
#define R_ISM_WAI_TIME_SON_BACK   (0)
#define R_ISM_MICRO_STEP_SON      (1)

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE2c =
  {
   15,    /* Table contains 16 entries */
    {
      /* SINP */
      ( 0x000 << R_ISM_PWM_H_P ) |
      ( 0x3FF << R_ISM_PWM_V_P ) |
      ( 1 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_WAI_TIME_SON_RF << R_ISM_CZD_P ),            /* SINP */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) |
      ( R_ISM_ZMT_TIME_SON << R_ISM_CZD_P ),               /* Measure */

      ( ( 0x1FF ) << R_ISM_PWM_H_P ) |
      ( ( 0x375 ) << R_ISM_PWM_V_P ) |
      ( 1 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* SINP */

      ( ( 0x375 ) << R_ISM_PWM_H_P ) |
      ( ( 0x1FF ) << R_ISM_PWM_V_P ) |
      ( 1 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* SINP */

      /* COSM */

      ( 0x3FF << R_ISM_PWM_H_P ) |
      ( 0x000 << R_ISM_PWM_V_P ) |
      ( 2 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_WAI_TIME_SON_RF << R_ISM_CZD_P ),            /* COSM */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) |
      ( R_ISM_ZMT_TIME_SON << R_ISM_CZD_P ),               /* Measure */

      ( ( 0x375 ) << R_ISM_PWM_H_P ) |
      ( ( 0x1FF ) << R_ISM_PWM_V_P ) |
      ( 2 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* COSM */

      ( ( 0x1FF ) << R_ISM_PWM_H_P ) |
      ( ( 0x375 ) << R_ISM_PWM_V_P ) |
      ( 2 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* COSM */

      /* SINM */

      ( 0x000 << R_ISM_PWM_H_P ) |
      ( 0x3FF << R_ISM_PWM_V_P ) |
      ( 3 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_WAI_TIME_SON_RF << R_ISM_CZD_P ),            /* SINM */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) |
      ( R_ISM_ZMT_TIME_SON << R_ISM_CZD_P ),               /* Measure */

      ( ( 0x1FF ) << R_ISM_PWM_H_P ) |
      ( ( 0x375 ) << R_ISM_PWM_V_P ) |
      ( 3 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* SINM */

      ( ( 0x375 ) << R_ISM_PWM_H_P ) |
      ( ( 0x1FF ) << R_ISM_PWM_V_P ) |
      ( 3 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* SINM */

      /* COSP */

      ( 0x3FF << R_ISM_PWM_H_P ) |
      ( 0x000 << R_ISM_PWM_V_P ) |
      ( 0 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_WAI_TIME_SON_RF << R_ISM_CZD_P ),            /* COSP */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) |
      ( R_ISM_ZMT_TIME_SON << R_ISM_CZD_P ),               /* Measure */

      ( ( 0x375 ) << R_ISM_PWM_H_P ) |
      ( ( 0x1FF ) << R_ISM_PWM_V_P ) |
      ( 0 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* COSP */

      ( ( 0x1FF ) << R_ISM_PWM_H_P ) |
      ( ( 0x375 ) << R_ISM_PWM_V_P ) |
      ( 0 << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( R_ISM_MICRO_STEP_SON << R_ISM_CZD_P ),             /* COSP */

    }
  };


/* Table 2d: First try on Device with Marelli Motor. 
   Rotate in 20 microsteps per quadrant and measure on one quadrant.
*/

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE2d =
  {
    51,    /* Table contains 52 entries */
    {
      ( 0x3F2L   << R_ISM_PWM_H_P ) |
      ( 0x0A2L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 1/10 Step Rotation: Position to SINP  */

      ( 0x3CBL   << R_ISM_PWM_H_P ) |
      ( 0x140L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 2/10 Step Rotation: Position to SINP  */

      ( 0x38CL   << R_ISM_PWM_H_P ) |
      ( 0x1D7L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 3/10 Step Rotation: Position to SINP  */

      ( 0x335L   << R_ISM_PWM_H_P ) |
      ( 0x261L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 4/10 Step Rotation: Position to SINP  */

      ( 0x2CAL   << R_ISM_PWM_H_P ) |
      ( 0x2DCL   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 5/10 Step Rotation: Position to SINP  */

      ( 0x24DL   << R_ISM_PWM_H_P ) |
      ( 0x344L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 6/10 Step Rotation: Position to SINP  */

      ( 0x1C0L   << R_ISM_PWM_H_P ) |
      ( 0x397L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 7/10 Step Rotation: Position to SINP  */

      ( 0x128L   << R_ISM_PWM_H_P ) |
      ( 0x3D2L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 8/10 Step Rotation: Position to SINP  */

      ( 0x089L   << R_ISM_PWM_H_P ) |
      ( 0x3F5L   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 9/10 Step Rotation: Position to SINP  */

      ( 0x000L   << R_ISM_PWM_H_P ) |
      ( 0x3FFL   << R_ISM_PWM_V_P ) |
      ( 0x0L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),     /* 10/10 Step Rotation: Position to SINP  */

      /* Measurement in Quadrant 0 */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),    /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                            /* Measure at COSM */

      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P ),                        /* End Measure at COSM */

      ( 0x096L   << R_ISM_PWM_H_P ) |
      ( 0x3F3L   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 1/10 Step Rotation: Position to COSM  */

      ( 0x134L   << R_ISM_PWM_H_P ) |
      ( 0x3CFL   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 2/10 Step Rotation: Position to COSM  */

      ( 0x1CBL   << R_ISM_PWM_H_P ) |
      ( 0x391L   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 3/10 Step Rotation: Position to COSM  */

      ( 0x257L   << R_ISM_PWM_H_P ) |
      ( 0x33DL   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 4/10 Step Rotation: Position to COSM  */

      ( 0x2D3L   << R_ISM_PWM_H_P ) |
      ( 0x2D3L   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 5/10 Step Rotation: Position to COSM  */

      ( 0x33DL   << R_ISM_PWM_H_P ) |
      ( 0x257L   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 6/10 Step Rotation: Position to COSM  */

      ( 0x391L   << R_ISM_PWM_H_P ) |
      ( 0x1CBL   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 7/10 Step Rotation: Position to COSM  */

      ( 0x3CFL   << R_ISM_PWM_H_P ) |
      ( 0x134L   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 8/10 Step Rotation: Position to COSM  */

      ( 0x3F3L   << R_ISM_PWM_H_P ) |
      ( 0x096L   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 9/10 Step Rotation: Position to COSM  */

      ( 0x3FFL   << R_ISM_PWM_H_P ) |
      ( 0x000L   << R_ISM_PWM_V_P ) |
      ( 0x1L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) |
      ( 0x00 << R_ISM_CZD_P ),     /* 10/10 Step Rotation: Position to COSM  */

      /* Measurement in Quadrant 1 */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),   /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                           /*  Measure at SINM */

      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_PTOVSS_ZIS23 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P ),                        /* End Measure at SINM */

      ( 0x3F2L   << R_ISM_PWM_H_P ) |
      ( 0x0A2L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 1/10 Step Rotation: Position to SINM  */

      ( 0x3CBL   << R_ISM_PWM_H_P ) |
      ( 0x140L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 2/10 Step Rotation: Position to SINM  */

      ( 0x38CL   << R_ISM_PWM_H_P ) |
      ( 0x1D7L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 3/10 Step Rotation: Position to SINM  */

      ( 0x335L   << R_ISM_PWM_H_P ) |
      ( 0x261L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 4/10 Step Rotation: Position to SINM  */

      ( 0x2CAL   << R_ISM_PWM_H_P ) |
      ( 0x2DCL   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 5/10 Step Rotation: Position to SINM  */

      ( 0x24DL   << R_ISM_PWM_H_P ) |
      ( 0x344L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 6/10 Step Rotation: Position to SINM  */

      ( 0x1C0L   << R_ISM_PWM_H_P ) |
      ( 0x397L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 7/10 Step Rotation: Position to SINM  */

      ( 0x128L   << R_ISM_PWM_H_P ) |
      ( 0x3D2L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 8/10 Step Rotation: Position to SINM  */

      ( 0x089L   << R_ISM_PWM_H_P ) |
      ( 0x3F5L   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 9/10 Step Rotation: Position to SINM  */

      ( 0x000L   << R_ISM_PWM_H_P ) |
      ( 0x3FFL   << R_ISM_PWM_V_P ) |
      ( 0x2L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),     /* 10/10 Step Rotation: Position to SINM  */

      /* Measurement in Quadrant 2 */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),   /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                          /*  Measure at COSP */

      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P ),                       /* End Measure at COSP */

      ( 0x096L   << R_ISM_PWM_H_P ) |
      ( 0x3F3L   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 1/10 Step Rotation: Position to COSM  */

      ( 0x134L   << R_ISM_PWM_H_P ) |
      ( 0x3CFL   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 2/10 Step Rotation: Position to COSM  */

      ( 0x1CBL   << R_ISM_PWM_H_P ) |
      ( 0x391L   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 3/10 Step Rotation: Position to COSM  */

      ( 0x257L   << R_ISM_PWM_H_P ) |
      ( 0x33DL   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 4/10 Step Rotation: Position to COSM  */

      ( 0x2D3L   << R_ISM_PWM_H_P ) |
      ( 0x2D3L   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 5/10 Step Rotation: Position to COSM  */

      ( 0x33DL   << R_ISM_PWM_H_P ) |
      ( 0x257L   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 6/10 Step Rotation: Position to COSM  */

      ( 0x391L   << R_ISM_PWM_H_P ) |
      ( 0x1CBL   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 7/10 Step Rotation: Position to COSM  */

      ( 0x3CFL   << R_ISM_PWM_H_P ) |
      ( 0x134L   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 8/10 Step Rotation: Position to COSM  */

      ( 0x3F3L   << R_ISM_PWM_H_P ) |
      ( 0x096L   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P ),     /* 9/10 Step Rotation: Position to COSM  */

      ( 0x3FFL   << R_ISM_PWM_H_P ) |
      ( 0x000L   << R_ISM_PWM_V_P ) |
      ( 0x3L     << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) |
      ( 0x00 << R_ISM_CZD_P ),     /* 10/10 Step Rotation: Position to COSM  */

      /* Measurement in Quadrant 3 */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x01 << R_ISM_CZD_P ),  /* Set the time window & let the spike pass */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( R_ISM_ZMT_EN ) |
      ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) |
      ( 0x03 << R_ISM_CZD_P ),                           /* Measure at SINP */

      ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) |
      ( R_ISM_DIRECT_MTOVSS_ZIS01 << R_ISM_DIRECT_V_P ) |
      ( R_ISM_DIRECT_EN ) |
      ( 0x00 << R_ISM_CZD_P )                         /*End Measure at SINP */
    }
  };


/* Table 3: Generates a slow positive rotation, no measurements are taken,
            decreasing delay by 8 per step */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE3 =
  {
    3,    /* Table contains 4 entries */
    {
      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_1  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x1F << R_ISM_CZD_P ),
      
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_2  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x17 << R_ISM_CZD_P ),
      
      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_3  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x0F << R_ISM_CZD_P ),
      
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_4  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x07 << R_ISM_CZD_P )
    }
  };

/* Table 4: Generates a fast negative rotation, no measurements are taken,
            increasing delay per step */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE4 =
  {
    3,    /* Table contains 4 entries */
    {
      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_1  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P ),
      
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_4  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x01 << R_ISM_CZD_P ),

      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_3  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x02 << R_ISM_CZD_P ),
      
      ( R_ISM_PWMDUTY_0   << R_ISM_PWM_H_P ) |
      ( R_ISM_PWMDUTY_100 << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_2  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x03 << R_ISM_CZD_P )
    }
  };

/* Table 5: Generates alternating PWM, one cycle fast, one slow */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE5 =
  {
    1,    /* Table contains 2 entries */
    {
      ( 0x2FF << R_ISM_PWM_H_P ) |
      ( 0x0FF << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_1  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x1F << R_ISM_CZD_P ),

      ( 0x0FF << R_ISM_PWM_H_P ) |
      ( 0x2FF << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_4  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P )
    }
  };

/* Table 6: Generates a fixed 50% PWM */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE6 =
  {
    0,    /* Table contains 1 entry */
    {
      ( 0x1FF << R_ISM_PWM_H_P ) |
      ( 0x1FF << R_ISM_PWM_V_P ) |
      ( R_ISM_QUADRANT_1  << R_ISM_CQI_P ) |
      ( R_ISM_RECIRC_H ) | ( R_ISM_RECIRC_V ) |
      ( 0x00 << R_ISM_CZD_P )
    }
  };

/* Table 7: Generates increasing Vertical PWM with full table length */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE7 =
  {
    127,    /* Table contains 128 entries */
    {
        0,
        8,  16,  24,  32,  40,  48,  56,  64,  72,  80,
       88,  96, 104, 112, 120, 128, 136, 144, 152, 160,
      168, 176, 184, 192, 200, 208, 216, 224, 232, 240,
      248, 256, 264, 272, 280, 288, 296, 304, 312, 320,
      328, 336, 344, 352, 360, 368, 376, 384, 392, 400,
      408, 416, 424, 432, 440, 448, 456, 464, 472, 480,
      488, 496, 504, 512, 520, 528, 536, 544, 552, 560,
      568, 576, 584, 592, 600, 608, 616, 624, 632, 640,
      648, 656, 664, 672, 680, 688, 696, 704, 712, 720,
      728, 736, 744, 752, 760, 768, 776, 784, 792, 800,
      808, 816, 824, 832, 840, 848, 856, 864, 872, 880,
      888, 896, 904, 912, 920, 928, 936, 944, 952, 960,
      968, 976, 984, 992, 1000, 1008, 1016
    }
  };

/* Tables 8-11: Generates direct I/O and ZPD Measurement with full table length,
                each one step in length, tables are varied in phase of I/O,
                at end of table, one ZPD measurement takes place on high driven input */

#define R_ISM_S_DIRECT0 ( ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) )
#define R_ISM_S_DIRECT1 ( ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) )
#define R_ISM_S_DIRECT2 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) )
#define R_ISM_S_DIRECT3 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) )
#define R_ISM_S_DRVZPD0 ( ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) )
#define R_ISM_S_DRVZPD1 ( ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) )
#define R_ISM_S_DRVZPD2 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_CURRENT_P    << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) )
#define R_ISM_S_DRVZPD3 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_CURRENT_M    << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) )
#define R_ISM_S_FNDZPD0 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) )
#define R_ISM_S_FNDZPD1 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) )
#define R_ISM_S_FNDZPD2 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) )
#define R_ISM_S_FNDZPD3 ( ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VSS   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) )
#define R_ISM_S_MESZPD0 ( ( R_ISM_DIRECT_MTOVDD_ZIS01 << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VDD   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_COS_P << R_ISM_ZIS_P ) )
#define R_ISM_S_MESZPD1 ( ( R_ISM_DIRECT_PTOVDD_ZIS23 << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_RECIRC_VDD   << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_COS_M << R_ISM_ZIS_P ) )
#define R_ISM_S_MESZPD2 ( ( R_ISM_DIRECT_RECIRC_VDD   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_PTOVDD_ZIS23 << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_SIN_M << R_ISM_ZIS_P ) )
#define R_ISM_S_MESZPD3 ( ( R_ISM_DIRECT_RECIRC_VDD   << R_ISM_DIRECT_H_P ) | \
                           ( R_ISM_DIRECT_MTOVDD_ZIS01 << R_ISM_DIRECT_V_P ) | \
                           ( R_ISM_DIRECT_EN ) | \
                           ( R_ISM_ZMT_EN ) | \
                           ( R_ISM_ZIS_SIN_P << R_ISM_ZIS_P ) )

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE8 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, 
      R_ISM_S_DRVZPD0
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE9 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, 
      R_ISM_S_DRVZPD1
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE10 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, 
      R_ISM_S_DRVZPD2
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE11 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, 
      R_ISM_S_DRVZPD3
    }
  };

/* Tables 12-15: Generates direct I/O and ZPD Measurement with full table length,
                 each one step in length, tables are varied in phase of I/O,
                 at end of table, one ZPD measurement takes place on low driven input */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE12 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, 
      R_ISM_S_FNDZPD0
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE13 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, 
      R_ISM_S_FNDZPD1
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE14 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, 
      R_ISM_S_FNDZPD2
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE15 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, 
      R_ISM_S_FNDZPD3
    }
  };

/* Tables 16-19: Generates direct I/O and ZPD Measurement with full table length,
                 each one step in length, tables are varied in phase of I/O,
                 at end of table, one ZPD measurement takes place on open input */

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE16 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, R_ISM_S_DIRECT3,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT2, 
      R_ISM_S_MESZPD0
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE17 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT3, R_ISM_S_DIRECT2, R_ISM_S_DIRECT1, 
      R_ISM_S_MESZPD1
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE18 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, R_ISM_S_DIRECT0,
      R_ISM_S_DIRECT2, R_ISM_S_DIRECT3, R_ISM_S_DIRECT1, 
      R_ISM_S_MESZPD2
    }
  };

const struct rism_zpdtcfg_t R_ISM_A_STDZPDTABLE19 =
  {
    127,    /* Table contains 128 entries */
    {
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, R_ISM_S_DIRECT2,
      R_ISM_S_DIRECT0, R_ISM_S_DIRECT1, R_ISM_S_DIRECT3, 
      R_ISM_S_MESZPD3
    }
  };


/*============================================================================


       Parameter Sets of Motors 


==============================================================================*/

/* Selected Parameter Set, Default Settings to CONTI "2000" Motors */

struct r_ism_motor_t R_ISM_A_CONTI2000;
struct r_ism_motor_t R_ISM_A_JCISONCEBOZ6403;
struct r_ism_motor_t R_ISM_A_CHINASTANDARD;
struct r_ism_motor_t R_ISM_A_MARELLIPM20T;

struct r_ism_motor_t *R_ISM_A_CURPARAMSET[ R_ISM_CHANNELS ] =
  {
    &R_ISM_A_CONTI2000,
    &R_ISM_A_CONTI2000,
    &R_ISM_A_CONTI2000,
    &R_ISM_A_CONTI2000,
    &R_ISM_A_CONTI2000,
    &R_ISM_A_CONTI2000
  };

/* Parameter Set 1: Conti "2000" Motor */

uint8_t R_ISM_SetMotor_CONTI2000( uint8_t ChannelNumber_u08 )
{
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  R_ISM_A_CONTI2000.PDF = 0x0000006L; /* PDF */
  R_ISM_A_CONTI2000.PAL = 0x0000020L; /* PAL */
  R_ISM_A_CONTI2000.PDL = 0x0000020L; /* PDL */
  R_ISM_A_CONTI2000.PMS = 0x00009C0L; /* PMS */
  R_ISM_A_CONTI2000.PHC = 0x0000700L; /* PHC */
  R_ISM_A_CONTI2000.PS1 = 0x000010EL; /* PS1 */
  R_ISM_A_CONTI2000.PS2 = 0x0000620L; /* PS2 */
  R_ISM_A_CONTI2000.PS3 = 0x00001C3L; /* PS3 */
  R_ISM_A_CONTI2000.PS4 = 0x000056BL; /* PS4 */

#ifdef USE_ISM_CHANNEL_WORKAROUND
  R_ISM_A_CONTI2000.CHANNELTYPE = R_ISM_CHANNEL4;
#else
  R_ISM_A_CONTI2000.CHANNELTYPE = R_ISM_CHANNEL0;
#endif

  R_ISM_A_CONTI2000.TARGETRANGE_LOW  = 0x0174000L; /* Target Position Range Low Resolution */
  R_ISM_A_CONTI2000.TARGETRANGE_HIGH = 0x05D0000L; /* Target Position Range High Resolution */

  R_ISM_A_CONTI2000.PWMSTANDARDTABLE = ( &R_ISM_A_STDPWMTABLE1 ); /* Standard PWM Table */
  R_ISM_A_CONTI2000.PWMHIRESTABLE    = ( &R_ISM_A_HIPWMTABLE1 );  /* High Precision PWM Table */
  R_ISM_A_CONTI2000.ZPDSTANDARDTABLE = ( &R_ISM_A_STDZPDTABLE1 ); /* ZPD Table */

  R_ISM_A_CONTI2000.RECIRCULATIONSIDE = R_ISM_RECIRC_SIDE_L;   /* Recirculation Side */
  R_ISM_A_CONTI2000.DEFAULTPRECISION  = R_ISM_PRECISION_STD;   /* Standard Precision */

  R_ISM_A_CONTI2000.ZPDBLANKINGTIME     = 1.0E-3;   /* ZPD Blanking Time */
  R_ISM_A_CONTI2000.ZPDVIBRATIONDAMPING = 0;     /* ZPD Vibration Damping disabled */

  /* Global Settings: Must match with other used motors! */

  R_ISM_A_CONTI2000.PWMSPEED  = 16.0E3;   /* PWM Frequency */
  R_ISM_A_CONTI2000.CMSPEED   = 2500.0;    /* CM Frequency */
  R_ISM_A_CONTI2000.ZPDPERIOD = 16.0E-6;  /* ZPD Period */

  R_ISM_A_CONTI2000.ZPDSUSPENDCLOCKS  = 3;     /* Measurement Cycle Suspend Clocks */
  R_ISM_A_CONTI2000.ZPDDIGFILTERLEVEL = 6;     /* Digital Filter Detection Level */
  R_ISM_A_CONTI2000.ZPDDIGFILTERDEPTH = 6;     /* Measurement Cycle Measurement Count */


//  R_ISM_A_CONTI2000.ZPDANALOGUESELECT = 0;     /* Dx4: Analogue level ref. selection */
//  R_ISM_A_CONTI2000.ZPDANALOGUELEVEL  = 11;    /* Dx4: Comparator Level: 450 mV (ref0) */
 

/* decide which table to use 
    D1x: Comparator Level: 450 mV is a starting point
*/
#ifndef ISM_USE_ZPDVREF /* table 0 RV1 */
  R_ISM_A_CONTI2000.ZPDANALOGUELEVEL  = 4;    /* */
  R_ISM_A_CONTI2000.ZPDANALOGUESELECT = 0;     /* D1x: Analogue level ref. selection
                                                    Table 1 (ISMnGGRV1[3:0])
                                                     0=215mV
                                                     1=230mV
                                                     2=235mV
                                                     3=245mV
                                                     4=450mV
                                                     5=480mV
                                                     6=500mV
                                                     7=550mV
                                                     8=100mV
                                                     9=150mV
                                                     10=250mV
                                                     11=350mV 
                                                     12=650mV
                                                     13=750mV
                                                     14=850mv
                                                     15=extVREF x 0.2 */
#else /* table 1 RV2 with ZPDREF */
    R_ISM_A_CONTI2000.ZPDANALOGUELEVEL  = 15;    /* D1x: Comparator Level: 450 mV (ref1) */ 
    R_ISM_A_CONTI2000.ZPDANALOGUESELECT = 1;     /* D1x: Analogue level ref. selection 
                                                    Table 1 (ISMnGGRV2[3:0])
                                                     0=215mV
                                                     1=225mV
                                                     2=235mV
                                                     3=245mV
                                                     4=470mV
                                                     5=480mV
                                                     6=490mV
                                                     7=500mV
                                                     8=150mV
                                                     9=350mV
                                                     10=450mV
                                                     11=550mV 
                                                     12=650mV
                                                     13=750mV
                                                     14=850mv
                                                     15=extVREF x 0.2 */
#endif /* table selection */

  R_ISM_A_CONTI2000.ZPDSYNCTIME       = 0.0;      /* Synchronization interval time before ZPD operation */
  R_ISM_A_CONTI2000.ZPDSTABTIME       = 0.0;      /* Stabilization interval time before ZPD operation */
  R_ISM_A_CONTI2000.ACCELTIME         = 0.0;      /* Period of 3608 electrical at ZPD target speed */
  R_ISM_A_CONTI2000.ACCELRESOLUTION   = 0;        /* Number of acceleration steps during interval */

  R_ISM_A_CONTI2000.ZPDTARGETCOMPENSATION = 0L;   /* Initial value of deviation of ZP from PWM zero */


  R_ISM_A_CURPARAMSET[ ChannelNumber_u08 ] = &R_ISM_A_CONTI2000;

  return( R_ISM_ERR_OK );
}

/* Parameter Set 2 JCI SONCEBOZ 6403 Motor */

uint8_t R_ISM_SetMotor_JCISONCEBOZ6403( uint8_t ChannelNumber_u08 )
{
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  R_ISM_A_JCISONCEBOZ6403.PDF = 0x0000007L; /* PDF */
  R_ISM_A_JCISONCEBOZ6403.PAL = 0x0000010L; /* PAL */
  R_ISM_A_JCISONCEBOZ6403.PDL = 0x0000010L; /* PDL */
  R_ISM_A_JCISONCEBOZ6403.PMS = 0x0000B00L; /* PMS */
  R_ISM_A_JCISONCEBOZ6403.PHC = 0x0000700L; /* PHC */
  R_ISM_A_JCISONCEBOZ6403.PS1 = 0x0000100L; /* PS1 */
  R_ISM_A_JCISONCEBOZ6403.PS2 = 0x0000600L; /* PS2 */
  R_ISM_A_JCISONCEBOZ6403.PS3 = 0x0000200L; /* PS3 */
  R_ISM_A_JCISONCEBOZ6403.PS4 = 0x0000580L; /* PS4 */


#ifdef USE_ISM_CHANNEL_WORKAROUND
  R_ISM_A_JCISONCEBOZ6403.CHANNELTYPE = R_ISM_CHANNEL5;
#else
  R_ISM_A_JCISONCEBOZ6403.CHANNELTYPE = R_ISM_CHANNEL1;
#endif

  R_ISM_A_JCISONCEBOZ6403.TARGETRANGE_LOW  = 0x0610000L; /* Target Position Range Low Resolution */
  R_ISM_A_JCISONCEBOZ6403.TARGETRANGE_HIGH = 0x0000000L; /* No High Resolution Support */

  R_ISM_A_JCISONCEBOZ6403.PWMSTANDARDTABLE = ( &R_ISM_A_STDPWMTABLE1 ); /* Standard PWM Table */
  R_ISM_A_JCISONCEBOZ6403.PWMHIRESTABLE    = ( NULL );                   /* No High Precision PWM Table */
  R_ISM_A_JCISONCEBOZ6403.ZPDSTANDARDTABLE = ( &R_ISM_A_STDZPDTABLE2c ); /* ZPD Table */

  R_ISM_A_JCISONCEBOZ6403.RECIRCULATIONSIDE = R_ISM_RECIRC_SIDE_L;   /* Recirculation Side */
  R_ISM_A_JCISONCEBOZ6403.DEFAULTPRECISION  = R_ISM_PRECISION_STD;   /* Standard Precision */

  R_ISM_A_JCISONCEBOZ6403.ZPDBLANKINGTIME     = 205.0E-6; /* No ZPD Blanking Time */
  R_ISM_A_JCISONCEBOZ6403.ZPDVIBRATIONDAMPING = 0;        /* ZPD Vibration Damping disabled */

  /* Global Settings: Must match with other used motors! */

  /* These settings are based on 20 MHz clock of ISM */

  R_ISM_A_JCISONCEBOZ6403.PWMSPEED  = 19.0E3;   /* PWM Frequency ~19.5 kHz */
  R_ISM_A_JCISONCEBOZ6403.CMSPEED   = 1500.0;   /* CM Frequency */
  R_ISM_A_JCISONCEBOZ6403.ZPDPERIOD = 12.9E-6;  /* ZPD Period ~52 us */

                                                    /* a max. possible high length to be found */
  R_ISM_A_JCISONCEBOZ6403.ZPDDIGFILTERLEVEL = 7;   /* Digital Filter Detection Level */
  R_ISM_A_JCISONCEBOZ6403.ZPDDIGFILTERDEPTH = 7;   /* Window length max. open */
  R_ISM_A_JCISONCEBOZ6403.ZPDSUSPENDCLOCKS  = 7;   /* Max. Suspend Clocks for highest accuracy */

#if 0
  R_ISM_A_JCISONCEBOZ6403.ZPDANALOGUESELECT = 1;   /* Dx4: Analogue level ref. selection */
  R_ISM_A_JCISONCEBOZ6403.ZPDANALOGUELEVEL  = 8;   /* Dx4: Comparator Level: 150 mV (ref1) */
#else
  R_ISM_A_JCISONCEBOZ6403.ZPDANALOGUESELECT = 0;   /* D1x: Analogue level ref. selection
                                                    (ref0: levels  8=100mV /  9=150mV /  0=215mV /  1=230mV /
                                                                   2=235mV /  3=245mV / 10=250mV / 11=350mV /
                                                                   4=450mV /  5=480mV /  6=500mV /  7=550mV /
                                                                  12=650mV / 13=750mV / 14=850mV / 15=extVREF */
  R_ISM_A_JCISONCEBOZ6403.ZPDANALOGUELEVEL  = 9;   /* D1x: Comparator Level: 150 mV (ref0) */
#endif

  R_ISM_A_JCISONCEBOZ6403.ZPDSYNCTIME       = 40E-3;    /* Synchronization interval time before ZPD operation */
  R_ISM_A_JCISONCEBOZ6403.ZPDSTABTIME       = 60E-3;    /* Stabilization interval time before ZPD operation */
  R_ISM_A_JCISONCEBOZ6403.ACCELTIME         = 16E-3;    /* Period of 3608 electrical at ZPD target speed */
  R_ISM_A_JCISONCEBOZ6403.ACCELRESOLUTION   = 24;       /* Number of acceleration steps during 1 rotation,
                                                            1/24 of a turn using 1 period of CM would yield 16 ms
                                                            for the full rotation */

  R_ISM_A_JCISONCEBOZ6403.ZPDTARGETCOMPENSATION = 0L;   /* Initial value of deviation of ZP from PWM zero */

  R_ISM_A_CURPARAMSET[ ChannelNumber_u08 ] = &R_ISM_A_JCISONCEBOZ6403;

  return( R_ISM_ERR_OK );
}

/* Parameter Set 3 Chinese Standard Motor */

uint8_t R_ISM_SetMotor_CHINASTANDARD( uint8_t ChannelNumber_u08 )
{
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  R_ISM_A_CHINASTANDARD.PDF = 0x0000007L; /* PDF */
  R_ISM_A_CHINASTANDARD.PAL = 0x0000018L; /* PAL */
  R_ISM_A_CHINASTANDARD.PDL = 0x0000018L; /* PDL */
  R_ISM_A_CHINASTANDARD.PMS = 0x0000F00L; /* PMS */
  R_ISM_A_CHINASTANDARD.PHC = 0x0000800L; /* PHC */
  R_ISM_A_CHINASTANDARD.PS1 = 0x0000280L; /* PS1 */
  R_ISM_A_CHINASTANDARD.PS2 = 0x0000480L; /* PS2 */
  R_ISM_A_CHINASTANDARD.PS3 = 0x0000280L; /* PS3 */
  R_ISM_A_CHINASTANDARD.PS4 = 0x0000480L; /* PS4 */

#ifdef USE_ISM_CHANNEL_WORKAROUND
  R_ISM_A_CHINASTANDARD.CHANNELTYPE = R_ISM_CHANNEL5;
#else
  R_ISM_A_CHINASTANDARD.CHANNELTYPE = R_ISM_CHANNEL1;
#endif

  R_ISM_A_CHINASTANDARD.TARGETRANGE_LOW  = 0x0430000L; /* Target Position Range Low Resolution */
  R_ISM_A_CHINASTANDARD.TARGETRANGE_HIGH = 0x0000000L; /* No High Resolution Support */

  R_ISM_A_CHINASTANDARD.PWMSTANDARDTABLE = ( &R_ISM_A_STDPWMTABLE6 ); /* Standard PWM Table */
  R_ISM_A_CHINASTANDARD.PWMHIRESTABLE    = ( NULL );                   /* No High Precision PWM Table */
  R_ISM_A_CHINASTANDARD.ZPDSTANDARDTABLE = ( &R_ISM_A_STDZPDTABLE2a ); /* ZPD Table */

  R_ISM_A_CHINASTANDARD.RECIRCULATIONSIDE = R_ISM_RECIRC_SIDE_L;   /* Recirculation Side */
  R_ISM_A_CHINASTANDARD.DEFAULTPRECISION  = R_ISM_PRECISION_STD;   /* Standard Precision */

  R_ISM_A_CHINASTANDARD.ZPDBLANKINGTIME     = 0.0E-6;   /* No ZPD Blanking Time */
  R_ISM_A_CHINASTANDARD.ZPDVIBRATIONDAMPING = 0;        /* ZPD Vibration Damping disabled */

  /* Global Settings: Must match with other used motors! */

  /* These settings are based on 20 MHz clock of ISM */

  R_ISM_A_CHINASTANDARD.PWMSPEED  = 19.0E3;   /* PWM Frequency ~19.5 kHz */
  R_ISM_A_CHINASTANDARD.CMSPEED   = 1000.0;   /* CM Frequency */
  R_ISM_A_CHINASTANDARD.ZPDPERIOD = 52.0E-6;  /* ZPD Period ~52 us */

                                                  /* a max. possible high length to be found */
  R_ISM_A_CHINASTANDARD.ZPDDIGFILTERLEVEL = 7;   /* Digital Filter Detection Level */
  R_ISM_A_CHINASTANDARD.ZPDDIGFILTERDEPTH = 15;  /* Window length max. open */
  R_ISM_A_CHINASTANDARD.ZPDSUSPENDCLOCKS  = 7;   /* Max. Suspend Clocks for highest accuracy */

#if 0
  R_ISM_A_CHINASTANDARD.ZPDANALOGUESELECT = 1;   /* Dx4: Analogue level ref. selection */
  R_ISM_A_CHINASTANDARD.ZPDANALOGUELEVEL  = 8;   /* Dx4: Comparator Level: 150 mV (ref1) */
#else
  R_ISM_A_CHINASTANDARD.ZPDANALOGUESELECT = 0;   /* D1x: Analogue level ref. selection
                                                    (ref0: levels  8=100mV /  9=150mV /  0=215mV /  1=230mV /
                                                                   2=235mV /  3=245mV / 10=250mV / 11=350mV /
                                                                   4=450mV /  5=480mV /  6=500mV /  7=550mV /
                                                                  12=650mV / 13=750mV / 14=850mV / 15=extVREF */
  R_ISM_A_CHINASTANDARD.ZPDANALOGUELEVEL  = 9;   /* D1x: Comparator Level: 150 mV (ref0) */
#endif

  R_ISM_A_CHINASTANDARD.ZPDSYNCTIME       = 0.0;      /* Synchronization interval time before ZPD operation */
  R_ISM_A_CHINASTANDARD.ZPDSTABTIME       = 0.0;      /* Stabilization interval time before ZPD operation */
  R_ISM_A_CHINASTANDARD.ACCELTIME         = 0.0;      /* Period of 3608 electrical at ZPD target speed */
  R_ISM_A_CHINASTANDARD.ACCELRESOLUTION   = 0;        /* Number of acceleration steps during interval */

  R_ISM_A_CHINASTANDARD.ZPDTARGETCOMPENSATION = 0L;   /* Initial value of deviation of ZP from PWM zero */

  R_ISM_A_CURPARAMSET[ ChannelNumber_u08 ] = &R_ISM_A_CHINASTANDARD;

  return( R_ISM_ERR_OK );
}

/* Parameter Set 4 Marelli PM20T Motor */

uint8_t R_ISM_SetMotor_MARELLIPM20T( uint8_t ChannelNumber_u08 )
{
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  R_ISM_A_MARELLIPM20T.PDF = 0x0000006L; /* PDF */
  R_ISM_A_MARELLIPM20T.PAL = 0x0000009L; /* PAL */
  R_ISM_A_MARELLIPM20T.PDL = 0x0000009L; /* PDL */
  R_ISM_A_MARELLIPM20T.PMS = 0x0000300L; /* PMS */
  R_ISM_A_MARELLIPM20T.PHC = 0x0000200L; /* PHC */
  R_ISM_A_MARELLIPM20T.PS1 = 0x0000080L; /* PS1 */
  R_ISM_A_MARELLIPM20T.PS2 = 0x0000180L; /* PS2 */
  R_ISM_A_MARELLIPM20T.PS3 = 0x0000080L; /* PS3 */
  R_ISM_A_MARELLIPM20T.PS4 = 0x0000180L; /* PS4 */

#ifdef USE_ISM_CHANNEL_WORKAROUND
  R_ISM_A_MARELLIPM20T.CHANNELTYPE = R_ISM_CHANNEL5;
#else
  R_ISM_A_MARELLIPM20T.CHANNELTYPE = R_ISM_CHANNEL1;
#endif

  R_ISM_A_MARELLIPM20T.TARGETRANGE_LOW  = 0x0040000L; /* Target Position Range Low Resolution */
  R_ISM_A_MARELLIPM20T.TARGETRANGE_HIGH = 0x0100000L; /* Target Position Range High Resolution */

  R_ISM_A_MARELLIPM20T.PWMSTANDARDTABLE = ( &R_ISM_A_STDPWMTABLE1 );  /* Standard PWM Table */
  R_ISM_A_MARELLIPM20T.PWMHIRESTABLE    = ( &R_ISM_A_HIPWMTABLE1 );   /* High Precision PWM Table */
  R_ISM_A_MARELLIPM20T.ZPDSTANDARDTABLE = ( &R_ISM_A_STDZPDTABLE2d ); /* ZPD Table */

  R_ISM_A_MARELLIPM20T.RECIRCULATIONSIDE = R_ISM_RECIRC_SIDE_L;   /* Recirculation Side */
  R_ISM_A_MARELLIPM20T.DEFAULTPRECISION  = R_ISM_PRECISION_HI;    /* Default High Precision */

  R_ISM_A_MARELLIPM20T.ZPDBLANKINGTIME     = 1.3E-3;   /* One ZPD cycle ZPD Blanking Time */
  R_ISM_A_MARELLIPM20T.ZPDVIBRATIONDAMPING = 0;        /* ZPD Vibration Damping disabled */

  /* Global Settings: Must match with other used motors! */

  /* These settings are based on 20 MHz clock of ISM */

  R_ISM_A_MARELLIPM20T.PWMSPEED  = 19.0E3;   /* PWM Frequency ~19.5 kHz */
  R_ISM_A_MARELLIPM20T.CMSPEED   = 1000.0;   /* CM Frequency */
  R_ISM_A_MARELLIPM20T.ZPDPERIOD = 52.0E-6;  /* ZPD Period ~52 us */

                                                 /* a max. possible high length to be found */
  R_ISM_A_MARELLIPM20T.ZPDDIGFILTERLEVEL = 7;    /* Digital Filter Detection Level */
  R_ISM_A_MARELLIPM20T.ZPDDIGFILTERDEPTH = 15;   /* Window length max. open */
  R_ISM_A_MARELLIPM20T.ZPDSUSPENDCLOCKS  = 7;    /* Max. Suspend Clocks for highest accuracy */

#if 0
  R_ISM_A_MARELLIPM20T.ZPDANALOGUESELECT = 1;    /* Dx4: Analogue level ref. selection */
  R_ISM_A_MARELLIPM20T.ZPDANALOGUELEVEL  = 8;    /* Dx4: Comparator Level: 150 mV (ref1) */
#else
  R_ISM_A_MARELLIPM20T.ZPDANALOGUESELECT = 0;    /* D1x: Analogue level ref. selection
                                                    (ref0: levels  8=100mV /  9=150mV /  0=215mV /  1=230mV /
                                                                   2=235mV /  3=245mV / 10=250mV / 11=350mV /
                                                                   4=450mV /  5=480mV /  6=500mV /  7=550mV /
                                                                  12=650mV / 13=750mV / 14=850mV / 15=extVREF */
  R_ISM_A_MARELLIPM20T.ZPDANALOGUELEVEL  = 9;    /* D1x: Comparator Level: 150 mV (ref0) */
#endif

  R_ISM_A_MARELLIPM20T.ZPDSYNCTIME       = 0.0;      /* Synchronization interval time before ZPD operation */
  R_ISM_A_MARELLIPM20T.ZPDSTABTIME       = 0.0;      /* Stabilization interval time before ZPD operation */
  R_ISM_A_MARELLIPM20T.ACCELTIME         = 0.0;      /* Period of 3608 electrical at ZPD target speed */
  R_ISM_A_MARELLIPM20T.ACCELRESOLUTION   = 0;        /* Number of acceleration steps during interval */
  
  R_ISM_A_MARELLIPM20T.ZPDTARGETCOMPENSATION = 0L;   /* Initial value of deviation of ZP from PWM zero */

  R_ISM_A_CURPARAMSET[ ChannelNumber_u08 ] = &R_ISM_A_MARELLIPM20T;

  return( R_ISM_ERR_OK );
}

