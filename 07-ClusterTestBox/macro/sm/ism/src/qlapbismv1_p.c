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
/* Purpose: ISM Low Level Driver */

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

#define DRIVER_LOCAL

#include <stddef.h>
// $$$: TODO cleanup !!!!!!
#include "r_typedefs.h"

#include "r_config_ism.h"
#include "r_ism_api.h"
#include "r_ism_sys.h"
#include "standardports_p.h"

#include "qlapbismv1.h"
#include "qlapbismv1_p.h"

#include <stddef.h>

#if( R_ISM_MACRO_NUM > 1 )
#error "NUMBER OF UNITS NOT SUPPORTED"
#endif

/*
 * CreateInterrupt() Jump Vector Table
 */

#if( R_ISM_MACRO_NUM == 1 )
static void ( *R_ISM_CIVector [ R_ISM_INT_PERUNIT ] )( );
#endif

/*
 * Global Variables 
 */

#if( R_ISM_MACRO_NUM == 1 )
uint8_t* R_ISM_Interrupt_DONE_bit                  = { R_ISM_NOIRQ };  
uint8_t R_ISM_Interrupt_REACHED_u08[1]               = { 0x00 };
uint8_t R_ISM_Interrupt_REACHED_ACK_u08[1]           = { 0x00 };
uint8_t* R_ISM_Interrupt_ZPDAD_bit                 = { R_ISM_NOIRQ };  
uint8_t* R_ISM_Interrupt_ZPD_bit                   = { R_ISM_NOIRQ };  
#endif

/***********************************************************
  Section: Local Statics
*/

/* The ISM Memory Mapping */  

static struct r_ism_t *rism_p[ R_ISM_MACRO_NUM ] =
{
  ( r_ism_t * )( R_ISM_MACRO_BASE0 )
};   

static union rism_pwm_t *rism_pwm_p[ R_ISM_MACRO_NUM ] =
  {
    ( rism_pwm_t * )( R_ISM_MACRO_BASE0 + R_ISM_PWMTABLEOFFSET )  /* unit 0 */
  };

static struct r_ism_zpd_t *rism_zpd_p[ R_ISM_MACRO_NUM ] =
  {
    ( r_ism_zpd_t * )( R_ISM_MACRO_BASE0 + R_ISM_ZPDTABLEOFFSET )  /* unit 0 */
  };


/*
 * Internal Support Functions
 */



/*
 * The API functions (Init / DeInit)
 */

r_ism_Error_t R_ISM_Init(uint32_t Unit)
{
    uint32_t pclk_Hz;
    
    /* initialize the ISM PCLK clock */
    pclk_Hz = R_SYS_ISM_SysSetClockHz(Unit, (80 * 1000 * 1000));
    if (pclk_Hz != (80 * 1000 * 1000))
    {
        /* error wrong pclk frequency */
        return R_ISM_ERR_NG;
    }

    /* initialize the ports */
    R_SYS_ISM_SetPort(Unit); //set ISM port and ZDP port

    /* initialize the interrupts */
    R_SYS_ISM_EnableInt(Unit, R_ISM_INT_ISM_ALL);//enable reach, done,zdp interrupt

    return R_ISM_ERR_OK;
}


r_ism_Error_t R_ISM_DeInit(uint32_t Unit)
{
    /* De-Initialize the interrupts */
    R_SYS_ISM_DisableInt(Unit, R_ISM_INT_ISM_ALL);

    /* De-Initialize the ports */
    R_SYS_ISM_ReleasePort(Unit);

    return R_ISM_ERR_OK;
}


/*
 * The interrupt Functions
 */

/*=============================================================================
 FunctionName: R_ISM_Interrupt_xxxx
 IN  :         -
 OUT :         -
 Description : Central Interrupt Entry, clears pending bits and stores status
 
 =============================================================================*/
void R_ISM_IsrDone(uint32_t Unit)
{   
  if( rism_p[ 0 ]->gip.ssdo )
    {
      rism_p[ 0 ]->gstc.cldo = 1;
      R_ISM_Interrupt_DONE_bit[ 0 ] = R_ISM_IRQ;
      if( R_ISM_CIVector[ 0 ] != NULL ) R_ISM_CIVector[ 0 ]( );
      R_ISM_Interrupt_DONE_bit[ 0 ] = R_ISM_NOIRQ;
    }
}

void R_ISM_IsrReached(uint32_t Unit)
{     
  R_ISM_Interrupt_REACHED_u08[ 0 ] = ( uint8_t )rism_p[ 0 ]->gip.ssre;
  if( R_ISM_CIVector[ 1 ] != NULL ) R_ISM_CIVector[ 1 ]( );
}

void R_ISM_IsrZpdad(uint32_t Unit)
{     
  if( rism_p[ 0 ]->gip.ssad )
    {
      rism_p[ 0 ]->gstc.clad = 1;
      R_ISM_Interrupt_ZPDAD_bit[ 0 ] = R_ISM_IRQ;
      if( R_ISM_CIVector[ 2 ] != NULL ) R_ISM_CIVector[ 2 ]( );
      R_ISM_Interrupt_ZPDAD_bit[ 0 ] = R_ISM_NOIRQ;
    }
}

void R_ISM_IsrZpd(uint32_t Unit)
{     
  R_ISM_Interrupt_ZPD_bit[ 0 ] = R_ISM_IRQ;
  if( R_ISM_CIVector[ 3 ] != NULL ) R_ISM_CIVector[ 3 ]( );
  rism_p[ 0 ]->clzp = R_ISM_CHANNEL_ENALL;
  R_ISM_Interrupt_ZPD_bit[ 0 ] = R_ISM_NOIRQ;
}

/*
 * The peripheral Support Functions
 */

/*=============================================================================
  FunctionName: R_ISM_PortEnable
  IN  : UnitNumber: Selected Macro
  OUT : Error Flag
  Description : 
  Enable ISM I/O ports
  =============================================================================*/
uint32_t R_ISM_PortEnable( uint8_t UnitNumber_u08,
                         uint8_t ChannelNumber_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  switch( UnitNumber_u08 ) {

  case R_ISM_UNIT:
    {
      switch( ChannelNumber_u08 )
        {

        case R_ISM_CHANNEL0:
#ifdef R_ISM_PORT_SINPFB0
          PORT_Enable( R_ISM_PORT_SINPFB0, R_ISM_PORT_BIT_SINPFB0,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINPFB0 );
#endif
#ifdef R_ISM_PORT_SINMFB0
          PORT_Enable( R_ISM_PORT_SINMFB0, R_ISM_PORT_BIT_SINMFB0,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINMFB0 );
#endif
#ifdef R_ISM_PORT_COSPFB0
          PORT_Enable( R_ISM_PORT_COSPFB0, R_ISM_PORT_BIT_COSPFB0,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSPFB0 );
#endif
#ifdef R_ISM_PORT_COSMFB0
          PORT_Enable( R_ISM_PORT_COSMFB0, R_ISM_PORT_BIT_COSMFB0,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSMFB0 );
#endif
          break;

        case R_ISM_CHANNEL1:
#ifdef R_ISM_PORT_SINPFB1
          PORT_Enable( R_ISM_PORT_SINPFB1, R_ISM_PORT_BIT_SINPFB1,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINPFB1 );
#endif
#ifdef R_ISM_PORT_SINMFB1
          PORT_Enable( R_ISM_PORT_SINMFB1, R_ISM_PORT_BIT_SINMFB1,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINMFB1 );
#endif
#ifdef R_ISM_PORT_COSPFB1
          PORT_Enable( R_ISM_PORT_COSPFB1, R_ISM_PORT_BIT_COSPFB1,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSPFB1 );
#endif
#ifdef R_ISM_PORT_COSMFB1
          PORT_Enable( R_ISM_PORT_COSMFB1, R_ISM_PORT_BIT_COSMFB1,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSMFB1 );
#endif
          break;

        case R_ISM_CHANNEL2:
#ifdef R_ISM_PORT_SINPFB2
          PORT_Enable( R_ISM_PORT_SINPFB2, R_ISM_PORT_BIT_SINPFB2,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINPFB2 );
#endif
#ifdef R_ISM_PORT_SINMFB2
          PORT_Enable( R_ISM_PORT_SINMFB2, R_ISM_PORT_BIT_SINMFB2,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINMFB2 );
#endif
#ifdef R_ISM_PORT_COSPFB2
          PORT_Enable( R_ISM_PORT_COSPFB2, R_ISM_PORT_BIT_COSPFB2,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSPFB2 );
#endif
#ifdef R_ISM_PORT_COSMFB2
          PORT_Enable( R_ISM_PORT_COSMFB2, R_ISM_PORT_BIT_COSMFB2,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSMFB2 );
#endif
          break;

        case R_ISM_CHANNEL3:
#ifdef R_ISM_PORT_SINPFB3
          PORT_Enable( R_ISM_PORT_SINPFB3, R_ISM_PORT_BIT_SINPFB3,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINPFB3 );
#endif
#ifdef R_ISM_PORT_SINMFB3
          PORT_Enable( R_ISM_PORT_SINMFB3, R_ISM_PORT_BIT_SINMFB3,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINMFB3 );
#endif
#ifdef R_ISM_PORT_COSPFB3
          PORT_Enable( R_ISM_PORT_COSPFB3, R_ISM_PORT_BIT_COSPFB3,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSPFB3 );
#endif
#ifdef R_ISM_PORT_COSMFB3
          PORT_Enable( R_ISM_PORT_COSMFB3, R_ISM_PORT_BIT_COSMFB3,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSMFB3 );
#endif
          break;

        case R_ISM_CHANNEL4:
#ifdef R_ISM_PORT_SINPFB4
          PORT_Enable( R_ISM_PORT_SINPFB4, R_ISM_PORT_BIT_SINPFB4,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINPFB4 );
#endif
#ifdef R_ISM_PORT_SINMFB4
          PORT_Enable( R_ISM_PORT_SINMFB4, R_ISM_PORT_BIT_SINMFB4,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINMFB4 );
#endif
#ifdef R_ISM_PORT_COSPFB4
          PORT_Enable( R_ISM_PORT_COSPFB4, R_ISM_PORT_BIT_COSPFB4,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSPFB4 );
#endif
#ifdef R_ISM_PORT_COSMFB4
          PORT_Enable( R_ISM_PORT_COSMFB4, R_ISM_PORT_BIT_COSMFB4,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSMFB4 );
#endif
          break;

        case R_ISM_CHANNEL5:
#ifdef R_ISM_PORT_SINPFB5
          PORT_Enable( R_ISM_PORT_SINPFB5, R_ISM_PORT_BIT_SINPFB5,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINPFB5 );
#endif
#ifdef R_ISM_PORT_SINMFB5
          PORT_Enable( R_ISM_PORT_SINMFB5, R_ISM_PORT_BIT_SINMFB5,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_SINMFB5 );
#endif
#ifdef R_ISM_PORT_COSPFB5
          PORT_Enable( R_ISM_PORT_COSPFB5, R_ISM_PORT_BIT_COSPFB5,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSPFB5 );
#endif
#ifdef R_ISM_PORT_COSMFB5
          PORT_Enable( R_ISM_PORT_COSMFB5, R_ISM_PORT_BIT_COSMFB5,
                       PORT_DIR_INPUT, PORT_MODE_PERIPHERAL, 
                       R_ISM_PORT_FUNC_COSMFB5 );
#endif
          break;
        }
      break;

  default: 
    return( R_ISM_ERR_NG );
    }

  } // switch( UnitNumber )

  return( R_ISM_ERR_OK );
}
    
/*=============================================================================
  FunctionName: R_ISM_PortDisable
  IN  : UnitNumber: Selected Macro
  OUT : Error Flag
  Description : 
  Release ISM I/O ports to normal port function
  =============================================================================*/
uint32_t R_ISM_PortDisable( uint8_t UnitNumber_u08,
                          uint8_t ChannelNumber_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  switch( UnitNumber_u08 ) {

  case R_ISM_UNIT:
    {
      switch( ChannelNumber_u08 )
        {

        case R_ISM_CHANNEL0:
#ifdef R_ISM_PORT_SINPFB0
          PORT_Disable( R_ISM_PORT_SINPFB0, R_ISM_PORT_BIT_SINPFB0 );
#endif
#ifdef R_ISM_PORT_SINMFB0
          PORT_Disable( R_ISM_PORT_SINMFB0, R_ISM_PORT_BIT_SINMFB0 );
#endif
#ifdef R_ISM_PORT_COSPFB0
          PORT_Disable( R_ISM_PORT_COSPFB0, R_ISM_PORT_BIT_COSPFB0 );
#endif
#ifdef R_ISM_PORT_COSMFB0
          PORT_Disable( R_ISM_PORT_COSMFB0, R_ISM_PORT_BIT_COSMFB0 );
#endif
          break;

        case R_ISM_CHANNEL1:
#ifdef R_ISM_PORT_SINPFB1
          PORT_Disable( R_ISM_PORT_SINPFB1, R_ISM_PORT_BIT_SINPFB1 );
#endif
#ifdef R_ISM_PORT_SINMFB1
          PORT_Disable( R_ISM_PORT_SINMFB1, R_ISM_PORT_BIT_SINMFB1 );
#endif
#ifdef R_ISM_PORT_COSPFB1
          PORT_Disable( R_ISM_PORT_COSPFB1, R_ISM_PORT_BIT_COSPFB1 );
#endif
#ifdef R_ISM_PORT_COSMFB1
          PORT_Disable( R_ISM_PORT_COSMFB1, R_ISM_PORT_BIT_COSMFB1 );
#endif
          break;
          
        case R_ISM_CHANNEL2:
#ifdef R_ISM_PORT_SINPFB2
          PORT_Disable( R_ISM_PORT_SINPFB2, R_ISM_PORT_BIT_SINPFB2 );
#endif
#ifdef R_ISM_PORT_SINMFB2
          PORT_Disable( R_ISM_PORT_SINMFB2, R_ISM_PORT_BIT_SINMFB2 );
#endif
#ifdef R_ISM_PORT_COSPFB2
          PORT_Disable( R_ISM_PORT_COSPFB2, R_ISM_PORT_BIT_COSPFB2 );
#endif
#ifdef R_ISM_PORT_COSMFB2
          PORT_Disable( R_ISM_PORT_COSMFB2, R_ISM_PORT_BIT_COSMFB2 );
#endif
          break;

        case R_ISM_CHANNEL3:
#ifdef R_ISM_PORT_SINPFB3
          PORT_Disable( R_ISM_PORT_SINPFB3, R_ISM_PORT_BIT_SINPFB3 );
#endif
#ifdef R_ISM_PORT_SINMFB3
          PORT_Disable( R_ISM_PORT_SINMFB3, R_ISM_PORT_BIT_SINMFB3 );
#endif
#ifdef R_ISM_PORT_COSPFB3
          PORT_Disable( R_ISM_PORT_COSPFB3, R_ISM_PORT_BIT_COSPFB3 );
#endif
#ifdef R_ISM_PORT_COSMFB3
          PORT_Disable( R_ISM_PORT_COSMFB3, R_ISM_PORT_BIT_COSMFB3 );
#endif
          break;

        case R_ISM_CHANNEL4:
#ifdef R_ISM_PORT_SINPFB4
          PORT_Disable( R_ISM_PORT_SINPFB4, R_ISM_PORT_BIT_SINPFB4 );
#endif
#ifdef R_ISM_PORT_SINMFB4
          PORT_Disable( R_ISM_PORT_SINMFB4, R_ISM_PORT_BIT_SINMFB4 );
#endif
#ifdef R_ISM_PORT_COSPFB4
          PORT_Disable( R_ISM_PORT_COSPFB4, R_ISM_PORT_BIT_COSPFB4 );
#endif
#ifdef R_ISM_PORT_COSMFB4
          PORT_Disable( R_ISM_PORT_COSMFB4, R_ISM_PORT_BIT_COSMFB4 );
#endif
          break;
          
        case R_ISM_CHANNEL5:
#ifdef R_ISM_PORT_SINPFB5
          PORT_Disable( R_ISM_PORT_SINPFB5, R_ISM_PORT_BIT_SINPFB5 );
#endif
#ifdef R_ISM_PORT_SINMFB5
          PORT_Disable( R_ISM_PORT_SINMFB5, R_ISM_PORT_BIT_SINMFB5 );
#endif
#ifdef R_ISM_PORT_COSPFB5
          PORT_Disable( R_ISM_PORT_COSPFB5, R_ISM_PORT_BIT_COSPFB5 );
#endif
#ifdef R_ISM_PORT_COSMFB5
          PORT_Disable( R_ISM_PORT_COSMFB5, R_ISM_PORT_BIT_COSMFB5 );
#endif
          break;
        }
      break;

    default: 
      return( R_ISM_ERR_NG );
    }

  } // switch( UnitNumber )

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_CreateInterrupt
  IN  :         Unit Number, Int Number, Int Level, 
                Int Function Vector
  OUT :         Success Status
  Description : Enables and Sets Interrupt Vector to given Function
 
  =============================================================================*/
uint32_t R_ISM_CreateInterrupt( uint8_t  UnitNumber_u08,
                              uint8_t  IntNumber_u08,
                              uint16_t  SetIntLevel_u16,
                              void ( *FunctionVector )( ) )
{
  uint16_t IntLevel_u16 = SetIntLevel_u16;

  if( FunctionVector == NULL ) IntLevel_u16 = R_ISM_INTCLEAR;

  R_ISM_CIVector[ ( UnitNumber_u08 *  R_ISM_INT_PERUNIT )
                   + IntNumber_u08 ] = FunctionVector;

  switch( UnitNumber_u08 )
    {
    case 0:
      switch( IntNumber_u08 ) {

#if( defined( R_ISM_INTM0DONE ) )
      case ( R_ISM_INT_DONE ):
        R_ISM_INTM0DONE = IntLevel_u16;
        break;
#endif

#if( defined( R_ISM_INTM0REACHED ) )
      case ( R_ISM_INT_REACHED ):
        R_ISM_INTM0REACHED = IntLevel_u16;
        break;
#endif

#if( defined( R_ISM_INTM0ZPDAD ) )
      case ( R_ISM_INT_ZPDAD ):
        R_ISM_INTM0ZPDAD = IntLevel_u16;
        break;
#endif

#if( defined( R_ISM_INTM0ZPD ) )
      case ( R_ISM_INT_ZPD ):
        R_ISM_INTM0ZPD = IntLevel_u16;
        rism_p[ UnitNumber_u08 ]->clzp = R_ISM_CHANNEL_ENALL;
        break;
#endif
      default:
        return( R_ISM_ERR_NG );
        
      }    /* switch( IntNumber ) */
      break;

    default:
      return( R_ISM_ERR_NG );

    }   /* switch( UnitNumber_u08 ) */
  
  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetZPDGlobals
  IN  : UnitNumber: Selected Macro
        Global Parameters of ZPD Unit
  OUT : Error Flag
  Description : 
  Set global settings of ZPD
  =============================================================================*/
uint32_t R_ISM_SetZPDGlobals( uint8_t UnitNumber_u08,
                            float ZPDPeriod_flt,
                            uint8_t ZPDSuspendClocks_u08,
                            uint8_t ZPDFilterLength_u08,
                            uint8_t ZPDFilterDepth_u08,
                            uint8_t ZPDAnalogLevel_u08,
                            uint8_t ZPDAnalogSelection_u08,
                            uint8_t ZPDADConnect_u08 )
{
  uint32_t ZPDFactor_u32;
  uint32_t ZPDDivider_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  ZPDFactor_u32 = ( uint32_t )( ( float )R_ISM_CLOCK_HZ * ZPDPeriod_flt );

  if( ZPDFactor_u32 <     4L ) ZPDDivider_u32 = 0L;
  if( ZPDFactor_u32 >=    4L ) ZPDDivider_u32 = 0L;
  if( ZPDFactor_u32 >=    5L ) ZPDDivider_u32 = 1L;
  if( ZPDFactor_u32 >=    6L ) ZPDDivider_u32 = 2L;
  if( ZPDFactor_u32 >=    7L ) ZPDDivider_u32 = 3L;
  if( ZPDFactor_u32 >=    8L ) ZPDDivider_u32 = 4L;
  if( ZPDFactor_u32 >=    9L ) ZPDDivider_u32 = 5L;
  if( ZPDFactor_u32 >=   10L ) ZPDDivider_u32 = 6L;
  if( ZPDFactor_u32 >=   11L ) ZPDDivider_u32 = 7L;
  if( ZPDFactor_u32 >=   12L ) ZPDDivider_u32 = 8L;
  if( ZPDFactor_u32 >=   16L ) ZPDDivider_u32 = 9L;
  if( ZPDFactor_u32 >=   32L ) ZPDDivider_u32 = 10L;
  if( ZPDFactor_u32 >=   64L ) ZPDDivider_u32 = 11L;
  if( ZPDFactor_u32 >=  128L ) ZPDDivider_u32 = 12L;
  if( ZPDFactor_u32 >=  256L ) ZPDDivider_u32 = 13L;
  if( ZPDFactor_u32 >=  512L ) ZPDDivider_u32 = 14L;
  if( ZPDFactor_u32 >= 1024L ) ZPDDivider_u32 = 15L;

  rism_p[ UnitNumber_u08 ]->gzpdctl.gcs  = ( uint32_t )ZPDSuspendClocks_u08;
  rism_p[ UnitNumber_u08 ]->gzpdctl.gzf  = ( uint32_t )ZPDDivider_u32;
  rism_p[ UnitNumber_u08 ]->gzpdctl.gfd  = ( uint32_t )ZPDFilterDepth_u08;
  rism_p[ UnitNumber_u08 ]->gzpdctl.gfl  = ( uint32_t )ZPDFilterLength_u08;
  rism_p[ UnitNumber_u08 ]->gzpdctl.gzo  = ( uint32_t )ZPDADConnect_u08;

  switch( ZPDAnalogSelection_u08 )
    {
    case R_ISM_ANALOGLEVEL0:
      rism_p[ UnitNumber_u08 ]->gzpdctl.grv1 = ( uint32_t )ZPDAnalogLevel_u08;
      break;

    case R_ISM_ANALOGLEVEL1:
      rism_p[ UnitNumber_u08 ]->gzpdctl.grv2 = ( uint32_t )ZPDAnalogLevel_u08;
      break;

    default:
      return( R_ISM_ERR_NG );
    }

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetZPDGlobals
  IN  : UnitNumber: Selected Macro
  OUT : Error Flag
        Global Parameters of ZPD Unit
  Description : 
  Get global settings of ZPD
  =============================================================================*/
uint32_t R_ISM_GetZPDGlobals( uint8_t  UnitNumber_u08,
                            uint8_t  ZPDAnalogSelection_u08,
                            uint32_t* ZPDPeriod_pu32,
                            uint8_t* ZPDSuspendClocks_pu08,
                            uint8_t* ZPDFilterLength_pu08,
                            uint8_t* ZPDFilterDepth_pu08,
                            uint8_t* ZPDAnalogLevel_pu08,
                            uint8_t* ZPDADConnect_pu08 )
{
  uint32_t ZPDFactor_u32;
  uint32_t ZPDDivider_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  ZPDDivider_u32 = rism_p[ UnitNumber_u08 ]->gzpdctl.gzf;

  switch( ZPDDivider_u32 )
    {
    case 0L:  ZPDFactor_u32 = 4L;    break;
    case 1L:  ZPDFactor_u32 = 5L;    break;
    case 2L:  ZPDFactor_u32 = 6L;    break;
    case 3L:  ZPDFactor_u32 = 7L;    break;
    case 4L:  ZPDFactor_u32 = 8L;    break;
    case 5L:  ZPDFactor_u32 = 9L;    break;
    case 6L:  ZPDFactor_u32 = 10L;   break;
    case 7L:  ZPDFactor_u32 = 11L;   break;
    case 8L:  ZPDFactor_u32 = 12L;   break;
    case 9L:  ZPDFactor_u32 = 16L;   break;
    case 10L: ZPDFactor_u32 = 32L;   break;
    case 11L: ZPDFactor_u32 = 64L;   break;
    case 12L: ZPDFactor_u32 = 128L;  break;
    case 13L: ZPDFactor_u32 = 256L;  break;
    case 14L: ZPDFactor_u32 = 512L;  break;
    case 15L: ZPDFactor_u32 = 1024L; break;
    default:  return( R_ISM_ERR_NG );
    }

  *ZPDPeriod_pu32 = ( uint32_t )( 1.0E6 / ( float )R_ISM_CLOCK_HZ * 
                               ( float )ZPDFactor_u32 );

  *ZPDSuspendClocks_pu08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->gzpdctl.gcs;
  *ZPDFilterDepth_pu08   = ( uint8_t )rism_p[ UnitNumber_u08 ]->gzpdctl.gfd;
  *ZPDFilterLength_pu08  = ( uint8_t )rism_p[ UnitNumber_u08 ]->gzpdctl.gfl;
  *ZPDADConnect_pu08     = ( uint8_t )rism_p[ UnitNumber_u08 ]->gzpdctl.gzo;

  switch( ZPDAnalogSelection_u08 )
    {
    case R_ISM_ANALOGLEVEL0:
      *ZPDAnalogLevel_pu08  = ( uint8_t )rism_p[ UnitNumber_u08 ]->gzpdctl.grv1;
      break;

    case R_ISM_ANALOGLEVEL1:
      *ZPDAnalogLevel_pu08  = ( uint8_t )rism_p[ UnitNumber_u08 ]->gzpdctl.grv2;
      break;

    default:
      return( R_ISM_ERR_NG );
    }

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetTimebase
  IN  : UnitNumber: Selected Macro
        PWMFrequency: PWM Frequency to be set
        CMFrequency: Channel Management Frequency (START) to be set
  OUT : Error Flag
  Description : 
  Set Timebase of ISM
  =============================================================================*/
uint32_t R_ISM_SetTimebase( uint8_t UnitNumber_u08,
                          float PWMFrequency_flt,
                          float CMFrequency_flt )
{
  uint32_t PWMFactor_u32;
  uint32_t CMFactor_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  PWMFactor_u32 = ( uint32_t )( ( float )R_ISM_CLOCK_HZ / 
                             ( PWMFrequency_flt * ( float )R_ISM_TB_COUNTS ) );
  if( PWMFactor_u32 > R_ISM_PWMDIV_MAX ) return( R_ISM_ERR_NG );
  if( PWMFactor_u32 < 1 ) return( R_ISM_ERR_NG );

  CMFactor_u32 = ( uint32_t )( ( float )R_ISM_CLOCK_HZ / 
                            ( ( float )R_ISM_TB_COUNTS * 
                              ( float )PWMFactor_u32 * CMFrequency_flt ) );
  if( CMFactor_u32 > R_ISM_CMDIV_MAX ) return( R_ISM_ERR_NG );
  if( CMFactor_u32 < 1 ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->gcfg.gtb = PWMFactor_u32-1;
  rism_p[ UnitNumber_u08 ]->gcfg.gud = CMFactor_u32-1;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetTimebase
  IN  : UnitNumber: Selected Macro
  OUT : Error Flag
        PWMFrequency: PWM Frequency set
        CMFrequency: Channel Management Frequency (START)  set
  Description : 
  Get Timebase settings of ISM
  =============================================================================*/
uint32_t R_ISM_GetTimebase( uint8_t  UnitNumber_u08,
                          uint32_t* PWMFrequency_pu32,
                          uint32_t* CMFrequency_pu32 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  *PWMFrequency_pu32 = R_ISM_CLOCK_HZ / ( R_ISM_TB_COUNTS * 
                                        ( rism_p[ UnitNumber_u08 ]->gcfg.gtb+1 ) );
  *CMFrequency_pu32 = *PWMFrequency_pu32 / ( rism_p[ UnitNumber_u08 ]->gcfg.gud+1 );

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_ReadTimebase
  IN  : UnitNumber: Selected Macro
  OUT : Error Flag
        TimeBaseValue: Read current TB count value
  Description : 
  Get common Timebase count of ISM
  =============================================================================*/
uint32_t R_ISM_ReadTimebase( uint8_t  UnitNumber_u08,
                           uint32_t* TimeBaseValue_pu32 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  *TimeBaseValue_pu32 = rism_p[ UnitNumber_u08 ]->gcnt;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetEnableZPD
  IN  : UnitNumber: Selected Macro
        Global ZPD Enable, Global ZPD Power Saving
  OUT : Error Flag
  Description : 
  Enable ZPD Operation, set Power Save Mode of ZPD
  =============================================================================*/
uint32_t R_ISM_SetEnableZPD( uint8_t UnitNumber_u08,
                           uint8_t ZPDEnable_u08,
                           uint8_t ZPDPowerSave_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ZPDPowerSave_u08 & ZPDEnable_u08 ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->gctl.gzl    = 1L;   /* Link ISM with ZPD analogue */

  rism_p[ UnitNumber_u08 ]->gzpdctl.gzp = ( uint32_t )ZPDPowerSave_u08;
  rism_p[ UnitNumber_u08 ]->gzpdctl.gze = ( uint32_t )ZPDEnable_u08;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetEnableZPD
  IN  : UnitNumber: Selected Macro
  OUT : Error Flag
        Global ZPD Enable, Global ZPD Power Saving
  Description : 
  Check Enabled ZPD Operation, get Power Save Mode of ZPD
  =============================================================================*/
uint32_t R_ISM_GetEnableZPD( uint8_t  UnitNumber_u08,
                           uint8_t* ZPDEnable_pu08,
                           uint8_t* ZPDPowerSave_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  /* Workaround: GZP is coded inversely */

  *ZPDPowerSave_pu08 = ( uint8_t )( ( ~rism_p[ UnitNumber_u08 ]->gzpdctl.gzp ) & 0x01 );
/*   *ZPDPowerSave_pu08 = ( uint8_t )eeism_p[ UnitNumber_u08 ]->gzpdctl.gzp; */
  *ZPDEnable_pu08    = ( uint8_t )rism_p[ UnitNumber_u08 ]->gzpdctl.gze;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_StartZPD
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Start ZPD Operation with CM
  =============================================================================*/
uint32_t R_ISM_StartZPD( uint8_t UnitNumber_u08,
                       uint8_t ChannelNumber_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].czp = 1L;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_OperateZPDDirect
  IN  : UnitNumber: Selected Macro
        ChannelMask: Selected Channels to start/stop ZPD measurement simultaneously
        Input Selection of selected channels
        Start / Stop Flag
  OUT : Error Flag
  Description : 
  Start ZPD Operation with CM
  =============================================================================*/
uint32_t R_ISM_OperateZPDDirect( uint8_t UnitNumber_u08,
                               uint8_t ChannelMask_u08,
                               uint8_t InputSelection_u08,
                               uint8_t StartStopFlag_u08 )
{
  uint8_t CurrentChannel_u08;
  uint32_t Trigger_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  Trigger_u32 = rism_p[ UnitNumber_u08 ]->zpdctl.zmt;

  /* First, stop ZPD of selected channels */

  for( CurrentChannel_u08 = R_ISM_CHANNEL0;
       CurrentChannel_u08 < R_ISM_CHANNELS;
       CurrentChannel_u08++ )
    {
      if( ( 1 << CurrentChannel_u08 ) & ChannelMask_u08 )
        {
          if( !StartStopFlag_u08 )
            Trigger_u32 &= ( uint32_t )( ~( 1 << CurrentChannel_u08 ) );
        }
    }

  rism_p[ UnitNumber_u08 ]->zpdctl.zmt = Trigger_u32;

  /* Modify the input selections */

  for( CurrentChannel_u08 = R_ISM_CHANNEL0;
       CurrentChannel_u08 < R_ISM_CHANNELS;
       CurrentChannel_u08++ )
    {
      if( ( 1 << CurrentChannel_u08 ) & ChannelMask_u08 )
        {
          switch( CurrentChannel_u08 )
            {
            case R_ISM_CHANNEL0:
              rism_p[ UnitNumber_u08 ]->zpdctl.zis0 = ( uint32_t )InputSelection_u08;
              break;
            case R_ISM_CHANNEL1:
              rism_p[ UnitNumber_u08 ]->zpdctl.zis1 = ( uint32_t )InputSelection_u08;
              break;
            case R_ISM_CHANNEL2:
              rism_p[ UnitNumber_u08 ]->zpdctl.zis2 = ( uint32_t )InputSelection_u08;
              break;
            case R_ISM_CHANNEL3:
              rism_p[ UnitNumber_u08 ]->zpdctl.zis3 = ( uint32_t )InputSelection_u08;
              break;
            case R_ISM_CHANNEL4:
              rism_p[ UnitNumber_u08 ]->zpdctl.zis4 = ( uint32_t )InputSelection_u08;
              break;
            case R_ISM_CHANNEL5:
              rism_p[ UnitNumber_u08 ]->zpdctl.zis5 = ( uint32_t )InputSelection_u08;
              break;
            default:
              return( R_ISM_ERR_NG );
            }

          if( StartStopFlag_u08 )
            Trigger_u32 |= ( uint32_t )( 1 << CurrentChannel_u08 );
        }
    }

  /* Finally, start the newly activated ZPD channels */

  rism_p[ UnitNumber_u08 ]->zpdctl.zmt = Trigger_u32;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_ResetZPD
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Stop and Reset ZPD Operation with CM
  =============================================================================*/
uint32_t R_ISM_ResetZPD( uint8_t UnitNumber_u08,
                       uint8_t ChannelNumber_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].czp = 0L;
  rism_p[ UnitNumber_u08 ]->clzp = ( 1 << ChannelNumber_u08 );

  /* clear the ISMnGIP (Global Interrupt Pending) status GIP.SSRE using the 
     ISMnGSTC, incase this reports the position has already been reached by a 
     previous operation.
  */
  rism_p[ UnitNumber_u08 ]->gstc.clre   = ( 1 << ChannelNumber_u08 );
  rism_p[ UnitNumber_u08 ]->gstc.cldo   = 1;
  rism_p[ UnitNumber_u08 ]->gstc.clad   = 1;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetZPDChannels
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        Channel specific ZPD Parameters
  OUT : Error Flag
  Description : 
  Set Channel specifc settings of ZPD
  =============================================================================*/
uint32_t R_ISM_SetZPDChannels( uint8_t UnitNumber_u08,
                             uint8_t ChannelNumber_u08,
                             uint8_t AnalogLevelSelection_u08,
                             uint16_t BlankingDelay_u16,
                             uint8_t VibrationDamping_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  if( AnalogLevelSelection_u08 )
    rism_p[ UnitNumber_u08 ]->zrs |= ( 1 << ChannelNumber_u08 );
  else
    rism_p[ UnitNumber_u08 ]->zrs &= ( ~( 1 << ChannelNumber_u08 ) );

  rism_p[ UnitNumber_u08 ]->zpdcmp[ ChannelNumber_u08 ].zbt = 
    ( uint32_t )BlankingDelay_u16;
  rism_p[ UnitNumber_u08 ]->zpdcmp[ ChannelNumber_u08 ].zss = 
    ( uint32_t )VibrationDamping_u08;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetZPDChannels
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
        Channel specific ZPD Parameters
  Description : 
  Get Channel specifc settings of ZPD
  =============================================================================*/
uint32_t R_ISM_GetZPDChannels( uint8_t  UnitNumber_u08,
                             uint8_t  ChannelNumber_u08,
                             uint8_t* AnalogLevelSelection_pu08,
                             uint16_t* BlankingDelay_pu16,
                             uint8_t* VibrationDamping_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  *AnalogLevelSelection_pu08 = ( uint8_t )( ( rism_p[ UnitNumber_u08 ]->zrs 
                                            >> ChannelNumber_u08 ) & 0x01 );
  *BlankingDelay_pu16        = ( uint16_t )( rism_p[ UnitNumber_u08 ]->zpdcmp
                                          [ ChannelNumber_u08 ].zbt );
  *VibrationDamping_pu08     = ( uint8_t )( rism_p[ UnitNumber_u08 ]->zpdcmp
                                          [ ChannelNumber_u08 ].zss );

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_ReadZPDStatus
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
        Channel specific ZPD Status Information
  Description : 
  Get Channel specifc ZPD Status Information
  =============================================================================*/
uint32_t R_ISM_ReadZPDStatus( uint8_t  UnitNumber_u08,
                            uint8_t  ChannelNumber_u08,
                            uint8_t* ZPDActive_pu08,
                            uint8_t* ZPDDelayState_pu08,
                            uint8_t* ZPDTableIndex_pu08,
                            uint8_t* ZPDMeasureActive_pu08,
                            uint8_t* ZPDCurrentResult_pu08,
                            uint8_t* ZPDAccumulatedResult_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  *ZPDActive_pu08     = ( uint8_t )rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].czp;
  *ZPDDelayState_pu08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->czd[ ChannelNumber_u08 ];
  *ZPDTableIndex_pu08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->czc[ ChannelNumber_u08 ];

  *ZPDMeasureActive_pu08     = ( uint8_t )( rism_p[ UnitNumber_u08 ]->zpdctl.zmt 
                                          >> ChannelNumber_u08 ) & 0x01;
  *ZPDCurrentResult_pu08     = ( uint8_t )( rism_p[ UnitNumber_u08 ]->zdr 
                                          >> ChannelNumber_u08 ) & 0x01;
  *ZPDAccumulatedResult_pu08 = ( uint8_t )( rism_p[ UnitNumber_u08 ]->zip 
                                          >> ChannelNumber_u08 ) & 0x01;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetChannelDelay
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        HorizontalDelay: Horizontal PWM Delay in Timebase Clocks
        VerticalDelay: Vertical PWM Delay in Timebase Clocks
  OUT : Error Flag
  Description : 
  Set Channel Delay of PWM Channels
  =============================================================================*/
uint32_t R_ISM_SetChannelDelay( uint8_t UnitNumber_u08,
                              uint8_t ChannelNumber_u08,
                              uint8_t HorizontalDelay_u08,
                              uint8_t VerticalDelay_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].cdh = ( uint32_t )HorizontalDelay_u08;
  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].cdv = ( uint32_t )VerticalDelay_u08;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetChannelType
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        ChannelType, ChannelPrecision: Properties of Channel
  OUT : Error Flag
  Description : 
  Set Channel Type of PWM Channels
  =============================================================================*/
uint32_t R_ISM_SetChannelType( uint8_t UnitNumber_u08,
                             uint8_t ChannelNumber_u08,
                             uint8_t ChannelType_u08,
                             uint8_t ChannelPrecision_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].cct = ( uint32_t )ChannelType_u08;
  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].ccp = ( uint32_t )ChannelPrecision_u08;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetChannelControl
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
        Contents of Channel Control Register
  Description : 
  Get Channel Control Register content
  =============================================================================*/
uint32_t R_ISM_GetChannelControl( uint8_t  UnitNumber_u08,
                                uint8_t  ChannelNumber_u08,
                                uint8_t* ChannelType_pu08,
                                uint8_t* ChannelPrecision_pu08,
                                uint8_t* HorizontalDelay_pu08,
                                uint8_t* VerticalDelay_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  *ChannelType_pu08      = ( uint8_t )rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].cct;
  *ChannelPrecision_pu08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].ccp;
  *HorizontalDelay_pu08  = ( uint8_t )rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].cdh;
  *VerticalDelay_pu08    = ( uint8_t )rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].cdv;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetPWM
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        Horizontal & Vertical PWM Duty Cycle Level, Quadrant Information
  OUT : Error Flag
  Description : 
  Set Channel PWM Output for control without CM
  =============================================================================*/
uint32_t R_ISM_SetPWM( uint8_t UnitNumber_u08,
                     uint8_t ChannelNumber_u08,
                     uint16_t HorizontalDutyCycle_u16,
                     uint16_t VerticalDutyCycle_u16,
                     uint8_t QuadrantInfo_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].chp = ( uint32_t )HorizontalDutyCycle_u16;
  rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].cvp = ( uint32_t )VerticalDutyCycle_u16;
  rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].cqi = ( uint32_t )QuadrantInfo_u08;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetPWM
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
        Horizontal & Vertical PWM Duty Cycle Level, Quadrant Information
  Description : 
  Get Channel PWM Output Settings
  =============================================================================*/
uint32_t R_ISM_GetPWM( uint8_t  UnitNumber_u08,
                     uint8_t  ChannelNumber_u08,
                     uint16_t* HorizontalDutyCycle_pu16,
                     uint16_t* VerticalDutyCycle_pu16,
                     uint8_t* QuadrantInfo_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  *HorizontalDutyCycle_pu16 = ( uint16_t )rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].chp;
  *VerticalDutyCycle_pu16   = ( uint16_t )rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].cvp;
  *QuadrantInfo_pu08        = ( uint8_t )rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].cqi;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetRecirculation
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        Recirculation Enable, Recirculation Side - Horizontal & Vertical
  OUT : Error Flag
  Description : 
  Set Channel Output Recirculation Configuration
  =============================================================================*/
uint32_t R_ISM_SetRecirculation( uint8_t UnitNumber_u08,
                               uint8_t ChannelNumber_u08,
                               uint8_t RecirculationEnableHorizontal_u08,
                               uint8_t RecirculationEnableVertical_u08,
                               uint8_t RecirculationSide_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  if( !RecirculationEnableHorizontal_u08 )
    rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] &= ~R_ISM_RECIRC_H;
  if( !RecirculationEnableVertical_u08 )
    rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] &= ~R_ISM_RECIRC_V;

  if( RecirculationSide_u08 != 0 )
    rism_p[ UnitNumber_u08 ]->irs |= ( uint32_t )( RecirculationSide_u08 << ChannelNumber_u08 );
  else
    rism_p[ UnitNumber_u08 ]->irs &= ( uint32_t )( ~( 1 << ChannelNumber_u08 ) );

  rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] |= 
    ( ( RecirculationEnableHorizontal_u08 & 0x01 ) << R_ISM_RECIRC_H_P );
  rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] |=
    ( ( RecirculationEnableVertical_u08 & 0x01 ) << R_ISM_RECIRC_V_P );

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetRecirculation
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        Recirculation Enable, Recirculation Side - Horizontal & Vertical
  OUT : Error Flag
        Recirculation Enable, Recirculation Side - Horizontal & Vertical
  Description : 
  Get Channel Output Recirculation Configuration
  =============================================================================*/
uint32_t R_ISM_GetRecirculation( uint8_t  UnitNumber_u08,
                               uint8_t  ChannelNumber_u08,
                               uint8_t* RecirculationEnableHorizontal_pu08,
                               uint8_t* RecirculationEnableVertical_pu08,
                               uint8_t* RecirculationSide_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  *RecirculationEnableHorizontal_pu08 = 
    ( uint8_t )( ( rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] & R_ISM_RECIRC_H ) >> R_ISM_RECIRC_H_P );
  *RecirculationEnableVertical_pu08   =
    ( uint8_t )( ( rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] & R_ISM_RECIRC_V ) >> R_ISM_RECIRC_V_P );
  *RecirculationSide_pu08             = ( ( ( uint8_t )rism_p[ UnitNumber_u08 ]->irs ) >> ChannelNumber_u08 ) & 0x01;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetDirectIO
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        Direct Transistor Control Codes
  OUT : Error Flag
  Description : 
  Set Channel Output Levels constantly and directly for control without CM
  =============================================================================*/
uint32_t R_ISM_SetDirectIO( uint8_t UnitNumber_u08,
                          uint8_t ChannelNumber_u08,
                          uint8_t HorizontalDirect_u08,
                          uint8_t VerticalDirect_u08 )
{
  uint32_t CurrentSetting_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  CurrentSetting_u32  = rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ];
  CurrentSetting_u32 &= ( ~( R_ISM_DIRECT_EN | 
                             ( R_ISM_DIRECT_MASK << R_ISM_DIRECT_H_P ) |
                             ( R_ISM_DIRECT_MASK << R_ISM_DIRECT_V_P ) ) );
  CurrentSetting_u32 |= ( ( ( uint32_t )HorizontalDirect_u08 << R_ISM_DIRECT_H_P ) |
                          ( ( uint32_t )VerticalDirect_u08   << R_ISM_DIRECT_V_P ) |
                          R_ISM_DIRECT_EN );

  /* set the channel in ZPD mode, in order to enable direct I/O */

  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].czp = 1L;
  rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] = CurrentSetting_u32;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetDirectIO
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
        Direct Transistor Control Codes
  Description : 
  Get Channel Output Levels Control Codes
  =============================================================================*/
uint32_t R_ISM_GetDirectIO( uint8_t UnitNumber_u08,
                          uint8_t ChannelNumber_u08,
                          uint8_t* HorizontalDirectEnable_pu08,
                          uint8_t* HorizontalDirect_pu08,
                          uint8_t* VerticalDirectEnable_pu08,
                          uint8_t* VerticalDirect_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  *HorizontalDirectEnable_pu08 = 
    ( uint8_t )( ( rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] & 
               R_ISM_DIRECT_H_EN ) >> R_ISM_DIRECT_H_EN_P );
  *VerticalDirectEnable_pu08   =
    ( uint8_t )( ( rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] & 
               R_ISM_DIRECT_V_EN ) >> R_ISM_DIRECT_V_EN_P );
  *HorizontalDirect_pu08       = 
    ( ( uint8_t )( rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] >> 
               R_ISM_DIRECT_H_P ) ) & R_ISM_DIRECT_MASK;
  *VerticalDirect_pu08         = 
    ( ( uint8_t )( rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] >> 
               R_ISM_DIRECT_V_P ) ) & R_ISM_DIRECT_MASK;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_ReleaseDirectIO
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Release Channel constant Output Levels (Revert to PWM)
  =============================================================================*/
uint32_t R_ISM_ReleaseDirectIO( uint8_t UnitNumber_u08,
                              uint8_t ChannelNumber_u08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ] &= ( ~R_ISM_DIRECT_EN );

  /* release ZPD mode for the selected channel, in order to restore PWM */

  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].czp = 0L;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetCMEnable
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Set or Clear the global and channel specific enable flags
  =============================================================================*/
uint32_t R_ISM_SetCMEnable( uint8_t UnitNumber_u08,
                          uint8_t ChannelMask_u08,
                          uint8_t EnableFlag_u08 )
{
  uint32_t ChannelFlags_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  ChannelFlags_u32 = rism_p[ UnitNumber_u08 ]->gstr.cen;
  if( EnableFlag_u08 )
    {
      ChannelFlags_u32 |= ( uint32_t )ChannelMask_u08;
    }
  else
    {
      ChannelFlags_u32 &= ( uint32_t )( ~ChannelMask_u08 );
    }
  
  switch( ChannelMask_u08 )
    {
    case R_ISM_GLOBAL_EN:   /* switch on or off CM in general */
      rism_p[ UnitNumber_u08 ]->gctl.gce = ( uint32_t )EnableFlag_u08;
      break;

    default:                 /* handle channels according to mask */
      do
        {
          while( rism_p[ UnitNumber_u08 ]->gstr.gcb );
          rism_p[ UnitNumber_u08 ]->gstr.cen = ChannelFlags_u32;

        } while( rism_p[ UnitNumber_u08 ]->gstr.cen != ChannelFlags_u32 );
      break;
    }

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetInterrupt
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Set or Clear the global and channel specific interrupt enable flags
  =============================================================================*/
uint32_t R_ISM_SetInterrupt( uint8_t UnitNumber_u08,
                           uint8_t ChannelMask_u08,
                           uint8_t GlobalMask_u08,
                           uint8_t ZPDADMask_u08,
                           uint8_t EnableFlag_u08 )
{
  uint32_t ChannelFlags_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  ChannelFlags_u32 = rism_p[ UnitNumber_u08 ]->gctl.iere;
  if( EnableFlag_u08 )
    {
      ChannelFlags_u32 |= ( uint32_t )ChannelMask_u08;
    }
  else
    {
      ChannelFlags_u32 &= ( uint32_t )( ~ChannelMask_u08 );
    }

  rism_p[ UnitNumber_u08 ]->gctl.iere = ChannelFlags_u32;
  rism_p[ UnitNumber_u08 ]->gctl.iedo = ( uint32_t )( GlobalMask_u08 & EnableFlag_u08 );
  rism_p[ UnitNumber_u08 ]->gctl.iead = ( uint32_t )( ZPDADMask_u08 & EnableFlag_u08 );
  
  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetInterrupt
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Get the global and channel specific interrupt enable flags
  =============================================================================*/
uint32_t R_ISM_GetInterrupt( uint8_t  UnitNumber_u08,
                           uint8_t* ChannelFlags_pu08,
                           uint8_t* GlobalEnableFlag_pu08,
                           uint8_t* ZPDADEnableFlag_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  *ChannelFlags_pu08     = ( uint8_t )rism_p[ UnitNumber_u08 ]->gctl.iere;
  *GlobalEnableFlag_pu08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->gctl.iedo;
  *ZPDADEnableFlag_pu08  = ( uint8_t )rism_p[ UnitNumber_u08 ]->gctl.iead;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetCMEnable
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Get the global and channel specific enable flags
  =============================================================================*/
uint32_t R_ISM_GetCMEnable( uint8_t  UnitNumber_u08,
                          uint8_t* ChannelFlags_pu08,
                          uint8_t* GlobalEnableFlag_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  *ChannelFlags_pu08     = ( uint8_t )rism_p[ UnitNumber_u08 ]->gstr.cen;
  *GlobalEnableFlag_pu08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->gctl.gce;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetCMBusy
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Get the CM busy flag
  =============================================================================*/
uint32_t R_ISM_GetCMBusy( uint8_t  UnitNumber_u08,
                        uint8_t* CMBusyFlag_pu08 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  *CMBusyFlag_pu08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->gstr.gcb;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetCMVariables
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Set Variables of a channel under CM 
  =============================================================================*/
uint32_t R_ISM_SetCMVariables( uint8_t               UnitNumber_u08,
                             uint8_t               ChannelNumber_u08,
                             uint8_t               ResetAll_bit,
                             struct rism_var_t *CMVars_p )
{
  uint32_t CurrentTargetPos_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  /* Variable changes are only allowed, when CEN is cleared for that channel */

  if( ( ( ( rism_p[ UnitNumber_u08 ]->gstr.cen ) >> ChannelNumber_u08 ) & 0x01 ) &&
      ( rism_p[ UnitNumber_u08 ]->gctl.gce ) )
    return( R_ISM_ERR_NG );

  if( ResetAll_bit )  /* Clear all variables and set target equal to current */
    {
      CurrentTargetPos_u32 = rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ]
                                                           .target;
      CMVars_p->acceleration  = 0L;
      CMVars_p->newspeed      = 0L;
      CMVars_p->oldspeed      = 0L;
      CMVars_p->temporary     = 0L;
      CMVars_p->position      = CurrentTargetPos_u32;
      CMVars_p->virtualpos    = CurrentTargetPos_u32;
      CMVars_p->dirspeedflags = 0L;
    }

  rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].acceleration = 
                                            CMVars_p->acceleration;
  rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].newspeed = 
                                            CMVars_p->newspeed;
  rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].oldspeed = 
                                            CMVars_p->oldspeed;
  rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].temporary = 
                                            CMVars_p->temporary;
  rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].position = 
                                            CMVars_p->position;
  rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].virtualpos = 
                                            CMVars_p->virtualpos;
  rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].dirspeedflags = 
                                            CMVars_p->dirspeedflags;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetCMVariables
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Get Variables of a channel under CM 
  =============================================================================*/
uint32_t R_ISM_GetCMVariables( uint8_t               UnitNumber_u08,
                             uint8_t               ChannelNumber_u08,
                             struct rism_var_t *CMVars_p )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  CMVars_p->acceleration  = rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].acceleration;
  CMVars_p->newspeed      = rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].newspeed;
  CMVars_p->oldspeed      = rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].oldspeed;
  CMVars_p->temporary     = rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].temporary;
  CMVars_p->position      = rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].position;
  CMVars_p->virtualpos    = rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].virtualpos;
  CMVars_p->dirspeedflags = rism_p[ UnitNumber_u08 ]->var[ ChannelNumber_u08 ].dirspeedflags;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetCMParameters
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Set Parameters of a channel under CM 
  =============================================================================*/
uint32_t R_ISM_SetCMParameters( uint8_t                 UnitNumber_u08,
                              uint8_t                 ChannelNumber_u08,
                              struct r_ism_motor_t *ConfigSet_p )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  /* Parameter changes are only allowed, when CEN is cleared for that channel */

  if( ( ( ( rism_p[ UnitNumber_u08 ]->gstr.cen ) >> ChannelNumber_u08 ) & 0x01 ) &&
      ( rism_p[ UnitNumber_u08 ]->gctl.gce ) )
    return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].pdf = ConfigSet_p->PDF;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].pal = ConfigSet_p->PAL;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].pdl = ConfigSet_p->PDL;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].pms = ConfigSet_p->PMS;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].phc = ConfigSet_p->PHC;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].ps1 = ConfigSet_p->PS1;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].ps2 = ConfigSet_p->PS2;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].ps3 = ConfigSet_p->PS3;
  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].ps4 = ConfigSet_p->PS4;

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetCMZPDTable
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        Pointer to array of ZPD table entries
  OUT : Error Flag
  Description : 
  Set ZPD table of a channel type under CM into memory and set channel's table
  length
  =============================================================================*/
uint32_t R_ISM_SetCMZPDTable( uint8_t                         UnitNumber_u08,
                            uint8_t                         ChannelNumber_u08,
                            const struct rism_zpdtcfg_t *ZPDCmdSet_p )
{
  uint8_t ChannelType_u08;
  uint16_t ZPDTableIndex_u16;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  /* ZPD Table changes are only allowed, when GCE is cleared */

  if( rism_p[ UnitNumber_u08 ]->gctl.gce ) return( R_ISM_ERR_NG );

  ChannelType_u08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->
                             ccmr[ ChannelNumber_u08 ].cct;

  for( ZPDTableIndex_u16 = 0;
       ZPDTableIndex_u16 <= ( ZPDCmdSet_p->TableLength );
       ZPDTableIndex_u16++ )
    {
      rism_zpd_p[ UnitNumber_u08 ]->cmd[ ChannelType_u08 ]
        [ ZPDTableIndex_u16 ] = ZPDCmdSet_p->Table[ ZPDTableIndex_u16 ];
    }

  rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].ctl = 
    ZPDCmdSet_p->TableLength;
  
  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetCMPWMTable
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
        Pointer to array of PWM table entries
        (only one of the two pointers is used at a time, depending on the 
         precision of the selected channel)
  OUT : Error Flag
  Description : 
  Set PWM table of a channel under CM 
  =============================================================================*/
uint32_t R_ISM_SetCMPWMTable( uint8_t                        UnitNumber_u08,
                            uint8_t                        ChannelNumber_u08,
                            const struct rism_pwmstd_t *ConfigSetStd_p,
                            const struct rism_pwmhi_t  *ConfigSetHi_p )
{
  uint8_t ChannelType_u08;
  uint8_t ChannelPrecision_u08;
  uint16_t PWMTableIndex_u16;
  uint8_t DirectionIndex_u08;
  uint8_t SpeedIndex_u08;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  /* PWM Table changes are only allowed, when GCE is cleared */

  if( rism_p[ UnitNumber_u08 ]->gctl.gce ) return( R_ISM_ERR_NG );

  ChannelType_u08      = ( uint8_t )rism_p[ UnitNumber_u08 ]->
                                     ccmr[ ChannelNumber_u08 ].cct;
  ChannelPrecision_u08 = ( uint8_t )rism_p[ UnitNumber_u08 ]->
                                     ccmr[ ChannelNumber_u08 ].ccp;

  switch( ChannelPrecision_u08 )
    {
    case R_ISM_PRECISION_HI:
      if( ConfigSetHi_p == NULL ) return( R_ISM_ERR_NG );
      for( PWMTableIndex_u16 = 0;
           PWMTableIndex_u16 < R_ISM_TABLESIZE_HI;
           PWMTableIndex_u16++ )
        {
          rism_pwm_p[ UnitNumber_u08 ]->hi[ ChannelType_u08 ]
                                           [ PWMTableIndex_u16 ].cvp =
            ConfigSetHi_p->hi[ PWMTableIndex_u16 ].cvp;

          rism_pwm_p[ UnitNumber_u08 ]->hi[ ChannelType_u08 ]
                                           [ PWMTableIndex_u16 ].chp =
            ConfigSetHi_p->hi[ PWMTableIndex_u16 ].chp;

          rism_pwm_p[ UnitNumber_u08 ]->hi[ ChannelType_u08 ]
                                           [ PWMTableIndex_u16 ].cqi =
            ConfigSetHi_p->hi[ PWMTableIndex_u16 ].cqi;

          rism_pwm_p[ UnitNumber_u08 ]->hi[ ChannelType_u08 ]
                                           [ PWMTableIndex_u16 ].ihr =
            ConfigSetHi_p->hi[ PWMTableIndex_u16 ].ihr;

          rism_pwm_p[ UnitNumber_u08 ]->hi[ ChannelType_u08 ]
                                           [ PWMTableIndex_u16 ].ivr =
            ConfigSetHi_p->hi[ PWMTableIndex_u16 ].ivr;
        }
      break;

    case R_ISM_PRECISION_STD:
      if( ConfigSetStd_p == NULL ) return( R_ISM_ERR_NG );
      for( SpeedIndex_u08 = 0;
           SpeedIndex_u08 < R_ISM_SPEEDS;
           SpeedIndex_u08++ )
        {
          for( DirectionIndex_u08 = 0;
               DirectionIndex_u08 < R_ISM_DIRECTIONS;
               DirectionIndex_u08++ )
            {
              for( PWMTableIndex_u16 = 0;
                   PWMTableIndex_u16 < R_ISM_TABLESIZE_STD;
                   PWMTableIndex_u16++ )
                {
                  rism_pwm_p[ UnitNumber_u08 ]->std[ ChannelType_u08 ]
                                                    [ SpeedIndex_u08 ]
                                                    [ DirectionIndex_u08 ]
                                                    [ PWMTableIndex_u16 ].cvp =
                    ConfigSetStd_p->std[ SpeedIndex_u08 ]
                                       [ DirectionIndex_u08 ]
                                       [ PWMTableIndex_u16 ].cvp;

                  rism_pwm_p[ UnitNumber_u08 ]->std[ ChannelType_u08 ]
                                                    [ SpeedIndex_u08 ]
                                                    [ DirectionIndex_u08 ]
                                                    [ PWMTableIndex_u16 ].chp =
                    ConfigSetStd_p->std[ SpeedIndex_u08 ]
                                       [ DirectionIndex_u08 ]
                                       [ PWMTableIndex_u16 ].chp;

                  rism_pwm_p[ UnitNumber_u08 ]->std[ ChannelType_u08 ]
                                                    [ SpeedIndex_u08 ]
                                                    [ DirectionIndex_u08 ]
                                                    [ PWMTableIndex_u16 ].cqi =
                    ConfigSetStd_p->std[ SpeedIndex_u08 ]
                                       [ DirectionIndex_u08 ]
                                       [ PWMTableIndex_u16 ].cqi;

                  rism_pwm_p[ UnitNumber_u08 ]->std[ ChannelType_u08 ]
                                                    [ SpeedIndex_u08 ]
                                                    [ DirectionIndex_u08 ]
                                                    [ PWMTableIndex_u16 ].ivr =
                    ConfigSetStd_p->std[ SpeedIndex_u08 ]
                                       [ DirectionIndex_u08 ]
                                       [ PWMTableIndex_u16 ].ivr;

                  rism_pwm_p[ UnitNumber_u08 ]->std[ ChannelType_u08 ]
                                                    [ SpeedIndex_u08 ]
                                                    [ DirectionIndex_u08 ]
                                                    [ PWMTableIndex_u16 ].ihr =
                    ConfigSetStd_p->std[ SpeedIndex_u08 ]
                                       [ DirectionIndex_u08 ]
                                       [ PWMTableIndex_u16 ].ihr;
                }
            }
        }
      break;

    default: 
      break;
    }

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_SetTargetPos
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Set Target Position of a channel under CM 
  =============================================================================*/
uint32_t R_ISM_SetCMTargetPos( uint8_t UnitNumber_u08,
                             uint8_t ChannelNumber_u08,
                             uint32_t TargetPosition_u32 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].target = 
    TargetPosition_u32;

  /* Clear those REACHED IRQ, which have been processed by the application layers */

  rism_p[ UnitNumber_u08 ]->gstc.clre = 
    R_ISM_Interrupt_REACHED_ACK_u08[ UnitNumber_u08 ];

  R_ISM_Interrupt_REACHED_ACK_u08[ UnitNumber_u08 ] = 0x00;
  
  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_GetTargetPos
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Get Target Position of a channel under CM 
  =============================================================================*/
uint32_t R_ISM_GetCMTargetPos( uint8_t  UnitNumber_u08,
                             uint8_t  ChannelNumber_u08,
                             uint32_t* TargetPosition_pu32 )
{
  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );
  if( ChannelNumber_u08 >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  *TargetPosition_pu32 = rism_p[ UnitNumber_u08 ]->set.ch[ ChannelNumber_u08 ].target;
  
  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_Reset
  IN  : UnitNumber: Selected Macro
        ChannelNumber: Selected Channel
  OUT : Error Flag
  Description : 
  Stop ISM by using the Soft Reset Function
  =============================================================================*/
uint32_t R_ISM_Reset( uint8_t UnitNumber_u08 )
{
  uint8_t ChannelNumber_u08;
  uint32_t RecoveryDelay_u32;

  if( UnitNumber_u08 >= R_ISM_MACRO_NUM ) return( R_ISM_ERR_NG );

  /* Force the soft reset, this causes a recirculation via VDD */

  rism_p[ UnitNumber_u08 ]->gctl.gen = 0;

  /* Clear registers that shall be cleared after a soft reset */

  rism_p[ UnitNumber_u08 ]->gctl.gce    = 0;
  rism_p[ UnitNumber_u08 ]->gstr.cen    = R_ISM_CHANNELS_OFF;
  rism_p[ UnitNumber_u08 ]->gstc.clre   = R_ISM_CHANNEL_ENALL;
  rism_p[ UnitNumber_u08 ]->gstc.cldo   = 1;
  rism_p[ UnitNumber_u08 ]->gstc.clad   = 1;
  rism_p[ UnitNumber_u08 ]->zpdctl.zmt  = R_ISM_CHANNELS_OFF;
  rism_p[ UnitNumber_u08 ]->clzp        = R_ISM_CHANNELS_OFF;

  for( ChannelNumber_u08 = 0; 
       ChannelNumber_u08 < R_ISM_CHANNELS; 
       ChannelNumber_u08++ )
    {
      rism_p[ UnitNumber_u08 ]->ccmr[ ChannelNumber_u08 ].czp =  0;
      rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].cqi =  R_ISM_QUADRANT_1;
      rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].chp =  R_ISM_PWMDUTY_0;
      rism_p[ UnitNumber_u08 ]->ccmp[ ChannelNumber_u08 ].cvp =  R_ISM_PWMDUTY_0;
      rism_p[ UnitNumber_u08 ]->cioc[ ChannelNumber_u08 ]    &= ~R_ISM_DIRECT_EN;
    }

  /* SW Delay to allow the recirculation calm down */

  for( RecoveryDelay_u32 = 0;
       RecoveryDelay_u32 < R_ISM_RECOVERYDELAY;
       RecoveryDelay_u32++ );

  /* Disable all outputs, after inductive load is discharged */
  /* Requirement for this depends on implementation of drivers */
  
#if( R_ISM_SWRESET_PDISABLE )
  for( ChannelNumber_u08 = 0; 
       ChannelNumber_u08 < R_ISM_CHANNELS; 
       ChannelNumber_u08++ )
    {
      R_ISM_PortDisable( UnitNumber_u08, ChannelNumber_u08 );
    }
#endif

  /* Re-Enable ISM */

  rism_p[ UnitNumber_u08 ]->gctl.gen = 1;

  /* SW Delay to feed the cleared PWM settings */

  for( RecoveryDelay_u32 = 0;
       RecoveryDelay_u32 < R_ISM_RECOVERYDELAY;
       RecoveryDelay_u32++ );

  return( R_ISM_ERR_OK );
}
