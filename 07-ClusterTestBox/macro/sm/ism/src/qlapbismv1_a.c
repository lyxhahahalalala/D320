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
/* Purpose: ISM Application Level Driver */

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

#include "r_typedefs.h"

#define  DRIVER_LOCAL
#define  APPLICATION_INTERFACE

#include "r_config_ism.h"
#include "r_ism_api.h"

#include "qlapbismv1.h"
#include "qlapbismv1_p.h"

#include <stddef.h>
#include <stdlib.h>

/* test pins */
#include "config.h"

#ifdef ISM_HW_DEBUG 
 #include "r_gpio_api.h"
#endif /* end ISM_HW_DEBUG */


#if( R_ISM_MACRO_NUM > 1 )
#error "NUMBER OF UNITS NOT SUPPORTED"
#endif

/*
 * Global Settings
 */

#ifdef  R_ISM_VERBOSE
#if( R_ISM_VERBOSE == 1 )

#include "stdio.h"
#include "r_bsp_stdio_api.h"
#define printf( String ) ( R_BSP_STDIO_Printf( String ) )
#endif

#if( R_ISM_VERBOSE == 2 )
#endif

#if( R_ISM_VERBOSE == 0 )
#define printf( String ) ( ISM_A_DoNothing( String ) )
#endif

#else
#define printf( String ) ( ISM_A_DoNothing( String ) )
#endif

/*
 * Global Variables
 */

static uint8_t R_ISM_A_CURUNIT       = 0;

static uint8_t R_ISM_A_IRQ_DONE      = 0;
static uint8_t R_ISM_A_IRQ_ZPDAD     = 0;
static uint8_t R_ISM_A_IRQ_ZPD       = 0;

uint8_t R_ISM_A_IRQ_REACHED[ R_ISM_CHANNELS ] = { 0, 0, 0, 0, 0, 0 };

static uint8_t R_ISM_A_IRQ_UART      = 0;


/*
 * Local Support Functions
 */

void ISM_A_DoNothing( char *String )  /* Suppress printing action */
{
}

#ifndef VLIB
uint8_t ISM_A_CHECK_UART( )
{
  return( R_ISM_A_IRQ_UART );
}

#define R_ISM_A_CHECK_UART( ) ( ISM_A_CHECK_UART( ) )
#endif


void ISM_A_CMDONEIRQ( void )  /* CM Done Interrupt */
{
  R_ISM_A_IRQ_DONE++;
  return;
}

void ISM_A_ZPDIRQ( void )  /* ZPD Interrupt */
{
  R_ISM_A_IRQ_ZPD++;
  return;
}

void ISM_A_CMREACHEDIRQ( void )  /* ISM 0 CM Reached Interrupts */
{
  uint8_t IntAckflag;
  uint8_t Interruptflag;

  /* For each channel, only one interrupt shall be generated.
     To achieve this, the flag of the low-level-driver is cleared only,
     if the count is zero. Otherwise, the flag remains set, and the
     low-level-driver will not clear the pending flag in hardware.
     This, however, will cause that the hardware is not generating further
     interrupts for that channel.
  */

  Interruptflag = R_ISM_Interrupt_REACHED_u08[ R_ISM_A_CURUNIT ];
  IntAckflag    = 0;
  
  if( Interruptflag & R_ISM_CHANNEL_EN0 )
    {
      if( R_ISM_A_IRQ_REACHED[ 0 ] == 0 )
        {
          IntAckflag |= R_ISM_CHANNEL_EN0;
        }
      R_ISM_A_IRQ_REACHED[ 0 ]++;
    }
  if( Interruptflag & R_ISM_CHANNEL_EN1 )
    {
      if( R_ISM_A_IRQ_REACHED[ 1 ] == 0 )
        {
          IntAckflag |= R_ISM_CHANNEL_EN1;
        }
      R_ISM_A_IRQ_REACHED[ 1 ]++;
    }
  if( Interruptflag & R_ISM_CHANNEL_EN2 )
    {
      if( R_ISM_A_IRQ_REACHED[ 2 ] == 0 )
        {
          IntAckflag |= R_ISM_CHANNEL_EN2;
        }
      R_ISM_A_IRQ_REACHED[ 2 ]++;
    }
  if( Interruptflag & R_ISM_CHANNEL_EN3 )
    {
      if( R_ISM_A_IRQ_REACHED[ 3 ] == 0 )
        {
          IntAckflag |= R_ISM_CHANNEL_EN3;
        }
      R_ISM_A_IRQ_REACHED[ 3 ]++;
    }
  if( Interruptflag & R_ISM_CHANNEL_EN4 )
    {
      if( R_ISM_A_IRQ_REACHED[ 4 ] == 0 )
        {
          IntAckflag |= R_ISM_CHANNEL_EN4;
        }
      R_ISM_A_IRQ_REACHED[ 4 ]++;
    }
  if( Interruptflag & R_ISM_CHANNEL_EN5 )
    {
      if( R_ISM_A_IRQ_REACHED[ 5 ] == 0 )
        {
          IntAckflag |= R_ISM_CHANNEL_EN5;
        }
      R_ISM_A_IRQ_REACHED[ 5 ]++;
    }

  R_ISM_Interrupt_REACHED_ACK_u08[ R_ISM_A_CURUNIT ] = IntAckflag;

  return;
}

void ISM_A_UARTABORT( void )  /* Abort Function by key press */
{
  R_ISM_A_IRQ_UART++;
  return;
}

/*=============================================================================
  FunctionName: R_ISM_CompensateDeviation
  IN  : UnitNumber: Selected Macro
        Selected Channel
        Target Position Value
  OUT : Error Flag
  Description : 
  Workaround for deviation of algorithm, in order to get REACHED Interrupt
  Also compensates a deviation between mechanical and electrical zero point
  =============================================================================*/
uint8_t R_ISM_CompensateDeviation( uint32_t* Position,
                                   uint8_t   CurrentChannel )
{
  int32_t CurrentPosition = ( int32_t )( *Position );
  
  CurrentPosition += R_ISM_A_CURPARAMSET[ CurrentChannel ]->ZPDTARGETCOMPENSATION;
  
  /* always set the remainder of the value */

  *Position = CurrentPosition | 0x0000FF;
  
  return( R_ISM_ERR_OK );
}

/*
 * Application Test Functions
 */

/*=============================================================================
  FunctionName: R_ISM_DownloadCMParamset
  IN  : Selected Channel Number
  OUT : Error Flag
  Description : 
  Downloads whole Parameter set to define motor characteristic
  =============================================================================*/
uint8_t R_ISM_DownloadCMParamset( uint8_t  UnitNumber,
                                 uint8_t  ChannelEnableMask )
{
  uint8_t ISM_Status_bit = R_ISM_ERR_OK;
  uint16_t BlankingTime_u16;
  uint32_t ZPDPeriod;
  uint8_t ADConnect_Dummy;
  uint8_t CurrentChannel;

  ISM_Status_bit =  R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        0 );

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit &= R_ISM_SetTimebase( UnitNumber,
                                                R_ISM_A_CURPARAMSET
                                                [ CurrentChannel ]->PWMSPEED,
                                                R_ISM_A_CURPARAMSET
                                                [ CurrentChannel ]->CMSPEED );

          ISM_Status_bit &= R_ISM_SetCMParameters( UnitNumber,
                                                    CurrentChannel,
                                                    R_ISM_A_CURPARAMSET
                                                    [ CurrentChannel ] );
  
          ISM_Status_bit &= R_ISM_SetChannelType( UnitNumber,
                                                   CurrentChannel,
                                                   R_ISM_A_CURPARAMSET
                                                   [ CurrentChannel ]->CHANNELTYPE,
                                                   R_ISM_A_CURPARAMSET
                                                   [ CurrentChannel ]->DEFAULTPRECISION );

          ISM_Status_bit &= R_ISM_SetCMPWMTable( UnitNumber,
                                                  CurrentChannel,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->PWMSTANDARDTABLE,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->PWMHIRESTABLE );

          ISM_Status_bit &= R_ISM_SetRecirculation( UnitNumber,
                                                     CurrentChannel,
                                                     1,  /* hor. recirculation */
                                                     1,  /* ver. recirculation */
                                                     R_ISM_A_CURPARAMSET
                                                     [ CurrentChannel ]->RECIRCULATIONSIDE );

          ISM_Status_bit &= R_ISM_SetCMZPDTable( UnitNumber,
                                                  CurrentChannel,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDSTANDARDTABLE );

          ISM_Status_bit &= R_ISM_SetZPDGlobals( UnitNumber,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDPERIOD,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDSUSPENDCLOCKS,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDDIGFILTERLEVEL,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDDIGFILTERDEPTH,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDANALOGUELEVEL,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDANALOGUESELECT,
                                                  0 );   /* A/D converter connection not supported */

          ISM_Status_bit &= R_ISM_GetZPDGlobals( UnitNumber,
                                                  R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDANALOGUESELECT,
                                                  &ZPDPeriod,
                                                  &R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDSUSPENDCLOCKS,
                                                  &R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDDIGFILTERLEVEL,
                                                  &R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDDIGFILTERDEPTH,
                                                  &R_ISM_A_CURPARAMSET
                                                  [ CurrentChannel ]->ZPDANALOGUELEVEL,
                                                  &ADConnect_Dummy );

          BlankingTime_u16 = ( uint16_t )( R_ISM_A_CURPARAMSET
                                        [ CurrentChannel ]->ZPDBLANKINGTIME / 
                                        ( 1.0E-6 * 
                                          ( float )( ZPDPeriod * 
                                                     ( uint32_t )( R_ISM_A_CURPARAMSET
                                                                [ CurrentChannel ]->ZPDSUSPENDCLOCKS +
                                                                R_ISM_A_CURPARAMSET
                                                                [ CurrentChannel ]->ZPDDIGFILTERDEPTH + 
                                                                2 ) ) ) );
          
          ISM_Status_bit &= R_ISM_SetZPDChannels( UnitNumber,
                                                   CurrentChannel,
                                                   R_ISM_A_CURPARAMSET
                                                   [ CurrentChannel ]->ZPDANALOGUESELECT,
                                                   BlankingTime_u16,
                                                   R_ISM_A_CURPARAMSET
                                                   [ CurrentChannel ]->ZPDVIBRATIONDAMPING );
        }
    }
  return( ISM_Status_bit );
}

/*=============================================================================
  FunctionName: R_ISM_SelectCMParamset
  IN  : Selected Channel Number
        Parameter Set to be used
  OUT : Error Flag
  Description : 
  Selects PWM table to assign for channel types and precisions
  =============================================================================*/
uint8_t R_ISM_SelectCMParamset( uint8_t  ChannelNumber,
                               uint8_t  SetNumber )
{
  if( ChannelNumber >= R_ISM_CHANNELS ) return( R_ISM_ERR_NG );

  switch( SetNumber )
    {
    case R_ISM_A_USE_CONTI2000:
      R_ISM_A_CURPARAMSET[ ChannelNumber ] = &R_ISM_A_CONTI2000;
      break;

    case R_ISM_A_USE_JCISONCEBOZ6403:
      R_ISM_A_CURPARAMSET[ ChannelNumber ] = &R_ISM_A_JCISONCEBOZ6403;
      break;

    case R_ISM_A_USE_CHINASTANDARD:
      R_ISM_A_CURPARAMSET[ ChannelNumber ] = &R_ISM_A_CHINASTANDARD;
      break;

    case R_ISM_A_USE_MARELLIPM20T:
      R_ISM_A_CURPARAMSET[ ChannelNumber ] = &R_ISM_A_MARELLIPM20T;
      break;

    default:
      return( R_ISM_ERR_NG );
    }

  return( R_ISM_ERR_OK );
}

/*=============================================================================
  FunctionName: R_ISM_MonitorZPDCMChannels
  IN  : UnitNumber: Selected Macro
        Selected Channels
        Target Position Range
        Random Movement Selection
  OUT : Error Flag
  Description : 
  Records the CM flags and values regarding ZPD for dedicated channels.
  =============================================================================*/
uint8_t R_ISM_MonitorZPDCMChannels( uint8_t  UnitNumber,
                                   uint8_t  ChannelEnableMask,
                                   uint8_t  InterruptClearFlag,
                                   uint8_t* ZPDActive,
                                   uint8_t* ZPDDelayState,
                                   uint8_t* ZPDTableIndex,
                                   uint8_t* ZPDMeasureActive,
                                   uint8_t* ZPDCurrentResult,
                                   uint8_t* ZPDAccumulatedResult )
{
  uint8_t ISM_Status_bit = R_ISM_ERR_OK;
  uint8_t CurrentChannel;

  static uint8_t GlobalEnableFlag = 0;

  if( !GlobalEnableFlag )
    {
      ISM_Status_bit &= R_ISM_CreateInterrupt( UnitNumber,
                                                R_ISM_INT_DONE,
                                                0,
                                                ISM_A_CMDONEIRQ );
      ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                             R_ISM_CHANNELS_OFF,
                                             1,   /* DONE  Interrupt enable */
                                             0,   /* ZPDAD Interrupt disable */
                                             1 ); /* Enable Flag */
      
      if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );
      GlobalEnableFlag = 1;
    }

  R_ISM_A_IRQ_DONE = 0;

  while( !R_ISM_A_IRQ_DONE );

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit &= R_ISM_ReadZPDStatus( UnitNumber,
                                                  CurrentChannel,
                                                  &ZPDActive
                                                  [ CurrentChannel ],
                                                  &ZPDDelayState
                                                  [ CurrentChannel ],
                                                  &ZPDTableIndex
                                                  [ CurrentChannel ],
                                                  &ZPDMeasureActive
                                                  [ CurrentChannel ],
                                                  &ZPDCurrentResult
                                                  [ CurrentChannel ],
                                                  &ZPDAccumulatedResult
                                                  [ CurrentChannel ] );
        }
    }

  if( InterruptClearFlag )
    {
      ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                             R_ISM_CHANNELS_OFF,
                                             0,   /* DONE  Interrupt disable */
                                             0,   /* ZPDAD Interrupt disable */
                                             1 ); /* Enable Flag */
      ISM_Status_bit &= R_ISM_CreateInterrupt( UnitNumber,
                                                R_ISM_INT_DONE,
                                                0,
                                                NULL );
      GlobalEnableFlag = 0;
    }

  return( ISM_Status_bit );
}

/*=============================================================================
  FunctionName: R_ISM_AutoMoveCMChannel
  IN  : UnitNumber: Selected Macro
        Selected Channels
        Target Position Range
        Random Movement Selection
  OUT : Error Flag
  Description : 
  Moves the motor(s) of selected channels automatically within target position 
  ranges.
  If RandomMovement is not set, it moves up and down once for selected channels.
  If RandomMovement is set, it moves endlessly after random time random channels
  to random positions within the target position range.
  =============================================================================*/
uint8_t R_ISM_AutoMoveCMChannel( uint8_t  UnitNumber,
                                uint8_t  ChannelEnableMask,
                                uint8_t  RandomMovement )
{
  uint8_t ISM_Status_bit         = R_ISM_ERR_OK;
  uint8_t CurrentChannel;
  uint8_t MoveCounts         = R_ISM_CHANNELS*2;
  uint32_t CurTargetPosition[ R_ISM_CHANNELS ];
  uint32_t TimeoutLimit[ R_ISM_CHANNELS ];
  uint8_t ZPDMovement[ R_ISM_CHANNELS ];
  uint32_t TargetPositionRange[ R_ISM_CHANNELS ];
  float RandomPositionFactor_flt;

  struct rism_var_t DefaultVariableSet;

  ISM_Status_bit = R_ISM_CreateInterrupt( UnitNumber,
                                           R_ISM_INT_REACHED,
                                           0,
                                           ISM_A_CMREACHEDIRQ );

  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        0 );

  ISM_Status_bit &= R_ISM_SetEnableZPD( UnitNumber,
                                         1,    /* Enable ZPD Function */
                                         0 );  /* Power on Analogue HW */

  srand( 0 );       /* Initialize random number generator and targets */

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      /* All interrupts aren't acknowledged */
      R_ISM_A_IRQ_REACHED[ CurrentChannel ]    = 1;
      TimeoutLimit[ CurrentChannel ]        = 1L;
      ZPDMovement[ CurrentChannel ]         = 0;

      switch( R_ISM_A_CURPARAMSET[ CurrentChannel ]->DEFAULTPRECISION )
        {
        case R_ISM_PRECISION_STD:
          TargetPositionRange[ CurrentChannel ] = R_ISM_A_CURPARAMSET
            [ CurrentChannel ]->TARGETRANGE_LOW;
          break;
        case R_ISM_PRECISION_HI:
          TargetPositionRange[ CurrentChannel ] = R_ISM_A_CURPARAMSET
            [ CurrentChannel ]->TARGETRANGE_HIGH;
          break;
        default:
          return( R_ISM_ERR_NG );
        }
    }
  
  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        1 );

  R_ISM_A_IRQ_UART = 0;

  do
    {
      for( CurrentChannel = R_ISM_CHANNEL0;
           CurrentChannel < R_ISM_CHANNELS;
           CurrentChannel++ )
        {
          if( RandomMovement && ( !ZPDMovement[ CurrentChannel ] ) )
            TimeoutLimit[ CurrentChannel ]--;

          if( ( 1 << CurrentChannel ) & ChannelEnableMask )
            {
              if( ( R_ISM_A_IRQ_REACHED[ CurrentChannel ] ) ||
                  ( TimeoutLimit[ CurrentChannel ] ) == 0L )
                {
                  /* Disable REACHED interrupts for the currently signalled channel */

                  ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                                         ( 1 << CurrentChannel ),
                                                         0,   /* DONE  Interrupt disable */
                                                         0,   /* ZPDAD Interrupt disable */
                                                         0 ); /* Enable Flag */

                  if( ZPDMovement[ CurrentChannel ] )
                    {
                      ZPDMovement[ CurrentChannel ] = 0;
                      ISM_Status_bit &= R_ISM_SetCMTargetPos( UnitNumber,
                                                               CurrentChannel,
                                                               0L );
                      ISM_Status_bit &= R_ISM_SetCMVariables( UnitNumber,
                                                               CurrentChannel,
                                                               R_ISM_RESETALL,
                                                               &DefaultVariableSet );
                      ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                                            ( 1 << CurrentChannel ),
                                                            1 );
                    }

                  if( RandomMovement )
                    {
                      RandomPositionFactor_flt = ( float )( rand( ) ) / ( float )( RAND_MAX );
                      CurTargetPosition[ CurrentChannel ] = 
                        ( uint32_t )( ( float )TargetPositionRange[ CurrentChannel ]
                                 * RandomPositionFactor_flt );
                      TimeoutLimit[ CurrentChannel ] = ( uint32_t )( rand( ) << 4 );
                      if( rand( ) < 0x0020 )   /* in a rare case, start ZPD */
                        {
                          ZPDMovement[ CurrentChannel ] = 1;
                          TimeoutLimit[ CurrentChannel ] = 1L;
                        }
                    } 
                  else
                    {
                      if( MoveCounts > R_ISM_CHANNELS )
                        CurTargetPosition[ CurrentChannel ] = 
                          TargetPositionRange[ CurrentChannel ];
                      else
                        CurTargetPosition[ CurrentChannel ] = 0L;

                      MoveCounts--;
                    }

                  if( ZPDMovement[ CurrentChannel ] )
                    {
                      R_ISM_A_IRQ_REACHED[ CurrentChannel ] = 0;

                      ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                                            ( 1 << CurrentChannel ),
                                                            0 );
                      ISM_Status_bit &= R_ISM_ResetZPD( UnitNumber,
                                                         CurrentChannel );
                      ISM_Status_bit &= R_ISM_StartZPD( UnitNumber,
                                                         CurrentChannel );
                      ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                                            ( 1 << CurrentChannel ),
                                                            1 );
                    }
                  else
                    {
                      /* compensate for the permanent deviation of the 
                         algorithm */

                      ISM_Status_bit &= R_ISM_CompensateDeviation( &CurTargetPosition[ CurrentChannel ],
                                                                   CurrentChannel );

                      /* Set the calculated position and wait for interrupt */

                      ISM_Status_bit &= R_ISM_SetCMTargetPos( UnitNumber,
                                                               CurrentChannel,
                                                               CurTargetPosition
                                                               [ CurrentChannel ] );
                      R_ISM_A_IRQ_REACHED[ CurrentChannel ] = 0;
                    }
                  
                  /* Enable REACHED interrupts for the currently moved channel */
                      
                  ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                                         ( 1 << CurrentChannel ),
                                                         0,   /* DONE  Interrupt disable */
                                                         0,   /* ZPDAD Interrupt disable */
                                                         1 ); /* Enable Flag */
                }
            }
        }
    }

  while( MoveCounts );

  return( ISM_Status_bit );
}

/*=============================================================================
  FunctionName: R_ISM_ZPDCMChannel
  IN  : UnitNumber: Selected Macro
        Selected Channels
  OUT : Error Flag
  Description : 
  Activates ZPD of selected channels to be executed by CM
  =============================================================================*/
extern void  WDT_Clear(void);
uint8_t R_ISM_ZPDCMChannel( uint8_t  UnitNumber,
                           uint8_t  ChannelEnableMask )
{
  uint8_t ISM_Status_bit         = R_ISM_ERR_OK;
  uint8_t CurrentChannel;
  uint8_t WaitOnReached;
  uint16_t StepIndex_u16;
  int16_t  TableIndex_s16;  
  uint16_t TableIndex_u16;
  uint8_t  Quadrant;
  uint16_t HorizontalPWM_u16;
  uint16_t VerticalPWM_u16;

  uint8_t ZPDActive;
  uint8_t ZPDDelayState;
  uint8_t ZPDTableIndex;
  uint8_t ZPDMeasureActive;
  uint8_t ZPDCurrentResult;
  uint8_t ZPDAccumulatedResult;
  
  uint32_t CMSteps;
  uint32_t CMLoop;

  struct rism_var_t DefaultVariableSet;
  
  struct r_ism_motor_t *CurParameterSet = NULL;

#ifdef R_ISM_VERBOSE
  char OutputString[ 80 ];
#endif

  for( CurrentChannel = R_ISM_CHANNEL0;    /* this routine only handles */
       CurrentChannel < R_ISM_CHANNELS;    /* one kind of motor */
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
      {
        if( CurParameterSet == NULL )
          CurParameterSet = R_ISM_A_CURPARAMSET[ CurrentChannel ];
        else if( CurParameterSet != R_ISM_A_CURPARAMSET[ CurrentChannel ] )
          return( R_ISM_ERR_NG );
      }
    }
      
#ifdef  R_ISM_VERBOSE
  printf( "\nCH CZP CZD CZC ZAF ZDR ZIP\n" );
#endif

  ISM_Status_bit = R_ISM_CreateInterrupt( UnitNumber,
                                           R_ISM_INT_REACHED,
                                           0,
                                           ISM_A_CMREACHEDIRQ );

  ISM_Status_bit &= R_ISM_CreateInterrupt( UnitNumber,
                                            R_ISM_INT_DONE,
                                            0,
                                            ISM_A_CMDONEIRQ );
#ifdef R_ISM_VERBOSE
#ifdef R_ISM_VERBOSE_PORTZPD
  ISM_Status_bit &= PORT_Enable( R_ISM_VERBOSE_PORTZPD,
                                 R_ISM_VERBOSE_BITZPD,
                                 PORT_DIR_OUTPUT,
                                 PORT_MODE_IO,
                                 PORT_FUNCTION_NORMAL );
#endif
#endif

  ISM_Status_bit &= R_ISM_SetEnableZPD( UnitNumber,
                                         1,    /* Enable ZPD Function */
                                         0);  /* Power on Analogue HW */

  /* Disable CM of the selected channel(s) */

  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        ChannelEnableMask,
                                        1 );

  /* Enable REACHED interrupts for the moved channels */

  ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                         ChannelEnableMask,
                                         1,   /* DONE  Interrupt enable */
                                         0,   /* ZPDAD Interrupt disable */
                                         1 ); /* Enable Flag */

  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );
  /* Synchronize selected motors by performing 4 steps passing quadrant 0.
     We are turning left (positive mathematical direction).
     Each step is assigned for synchronization time as specified in the 
     parameter set. */
     
  for( StepIndex_u16 =  0;
       StepIndex_u16 <= 3;
       StepIndex_u16++ )
  {
    for( CurrentChannel = R_ISM_CHANNEL0;
         CurrentChannel < R_ISM_CHANNELS;
         CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
      {
        ISM_Status_bit &= R_ISM_SetPWM( UnitNumber,
                                         CurrentChannel,
                                         R_ISM_PWMDUTY_100 * ( 3 - StepIndex_u16 ) / 3,
                                         R_ISM_PWMDUTY_100 * StepIndex_u16 / 3,
                                         R_ISM_QUADRANT_1 );
      }
    }
    
    /* We are using the DONE interval of ISM as our timebase */
  
    CMSteps = ( uint32_t )( CurParameterSet->CMSPEED * CurParameterSet->ZPDSYNCTIME );
    
    for( CMLoop = 0L;
         CMLoop < CMSteps;
         CMLoop++ )
    {
      R_ISM_A_IRQ_DONE = 0;
      while( !R_ISM_A_IRQ_DONE ) WDT_Clear();
    }
  }
  
  /* Anchor position now is on border between quadrant 1 and 2 (left turning). */
     
  /* Wait for a stabilisation time of the selected motors, so that the anchor
     is fully synchronized, as specified in the parameter set. */
     
  CMSteps = ( uint32_t )( CurParameterSet->CMSPEED * CurParameterSet->ZPDSTABTIME );
  
  for( CMLoop = 0L;
       CMLoop < CMSteps;
       CMLoop++ )
  {
    R_ISM_A_IRQ_DONE = 0;
    while( !R_ISM_A_IRQ_DONE )
	WDT_Clear();
  }


  /* Accelerate selected motors by applying PWM patterns of given granularity
     during a acceleration time frame as specified in the parameter set. 
     We are starting off from border of quadrant 1 into the direction of quadrant 2
     (left turning). */

  for( StepIndex_u16 =  0;
       StepIndex_u16 < ( CurParameterSet->ACCELRESOLUTION );
       StepIndex_u16++ )
  {
    TableIndex_s16 = R_ISM_TABLESIZE_STD - 
                     ( R_ISM_QUADRANT_2 * ( R_ISM_TABLESIZE_STD / R_ISM_QUADRANTS ) ) -
                     ( ( R_ISM_TABLESIZE_STD * StepIndex_u16 ) 
                     / ( CurParameterSet->ACCELRESOLUTION ) );
    if( TableIndex_s16 < 0 ) TableIndex_u16 = ( uint16_t )( R_ISM_TABLESIZE_STD + TableIndex_s16 );
    else TableIndex_u16 = ( uint16_t )TableIndex_s16;
      
    Quadrant = ( R_ISM_TABLESIZE_STD - TableIndex_u16 ) /
                   ( R_ISM_TABLESIZE_STD / R_ISM_QUADRANTS );
      
    HorizontalPWM_u16 = ( CurParameterSet->PWMSTANDARDTABLE )->std
                        [ R_ISM_SPEED_LO ][ R_ISM_DIRECTION_DN ][ TableIndex_u16 ].chp;
    VerticalPWM_u16   = ( CurParameterSet->PWMSTANDARDTABLE )->std
                        [ R_ISM_SPEED_LO ][ R_ISM_DIRECTION_DN ][ TableIndex_u16 ].cvp;
    
    for( CurrentChannel = R_ISM_CHANNEL0;
         CurrentChannel < R_ISM_CHANNELS;
         CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
      {
        ISM_Status_bit &= R_ISM_SetPWM( UnitNumber,
                                         CurrentChannel,
                                         HorizontalPWM_u16,
                                         VerticalPWM_u16,
                                         Quadrant );
      }
    }
    
    /* We are using the DONE interval of ISM as our timebase */
    /* The speed increases in a linear manner, as the loop gets shorter.
       The final speed results in the given ACCELTIME = time per 360° rotation */
  
    CMSteps = ( uint32_t )( ( CurParameterSet->CMSPEED * CurParameterSet->ACCELTIME ) /
                           ( StepIndex_u16 + 1 ) );
                           
    if( CMSteps < 1L ) CMSteps = 1L;
    
    for( CMLoop = 0L;
         CMLoop < CMSteps;
         CMLoop++ )
    {
      R_ISM_A_IRQ_DONE = 0;
      while( !R_ISM_A_IRQ_DONE );
    }
  }
  
  /* At this point, we are again at the border of quadrant 1 and 2,
     and have accelerated to a speed of ACCELTIME / 360° rotation.
     This is where the ZPD table starts with its first steps. */
    
  /* Start ZPD operation using the ZPD table */

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit &= R_ISM_ResetZPD( UnitNumber,
                                             CurrentChannel );

          ISM_Status_bit &= R_ISM_StartZPD( UnitNumber,
                                             CurrentChannel );

#ifdef ISM_HW_DEBUG 
            /* start ZDP trigger */
            R_GPIO_WritePin(TEST_PORT, TEST_PIN7, 1);
            R_GPIO_WritePin(TEST_PORT, TEST_PIN7, 0);
#endif /* end ISM_HW_DEBUG */

          R_ISM_A_IRQ_REACHED[ CurrentChannel ] = 0;
          if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );
        }
    }

  /* Enable CM */

  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        1 );
  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        ChannelEnableMask,
                                        1 );
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  R_ISM_A_IRQ_UART = 0;

  do   /* verify all selected channels to reach ZPD */
    {
      WaitOnReached = R_ISM_CHANNELS;
      WDT_Clear();
  
      for( CurrentChannel = R_ISM_CHANNEL0;
           CurrentChannel < R_ISM_CHANNELS;
           CurrentChannel++ )
        {
          if( ( 1 << CurrentChannel ) & ChannelEnableMask )
            {
              if( R_ISM_A_IRQ_REACHED[ CurrentChannel ] ) 
                {
                  WaitOnReached--;

  
                }
              else
                {

#ifdef ISM_HW_DEBUG
                      /* D0 Measurement active 
                         D1 Current result chan 0 (high = above comparator level including filter etc)
                         D2 Current result chan 1 (high = above comparator level including filter etc)
                         D3 Current result chan 2 (high = above comparator level including filter etc)
                         D4 Current result chan 3 (high = above comparator level including filter etc)
                         D7 Measurement start trigger
                      */

                      ISM_Status_bit &= R_ISM_ReadZPDStatus( UnitNumber,
                                                              CurrentChannel,
                                                              &ZPDActive,
                                                              &ZPDDelayState,
                                                              &ZPDTableIndex,
                                                              &ZPDMeasureActive,
                                                              &ZPDCurrentResult,
                                                              &ZPDAccumulatedResult );



                      if(0 != ZPDMeasureActive)
                      {
                          R_GPIO_WritePin(TEST_PORT, TEST_PIN0, 1);
                      }
                      else
                      {
                          R_GPIO_WritePin(TEST_PORT, TEST_PIN0, 0);
                      }
                      if(CurrentChannel == 0)
                      {
                          if((0 != ZPDCurrentResult) && (0 != ZPDMeasureActive))
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN1, 1);
                          }
                          else
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN1, 0);
                          }
                      }
                      else if(CurrentChannel == 1)
                      {
                          if((0 != ZPDCurrentResult) && (0 != ZPDMeasureActive))
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN2, 1);
                          }
                          else
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN2, 0);
                          }
                      }
                      else if(CurrentChannel == 2)
                      {
                          if((0 != ZPDCurrentResult) && (0 != ZPDMeasureActive))
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN3, 1);
                          }
                          else
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN3, 0);
                          }
                      }
                      else if(CurrentChannel == 3)
                      {
                          if((0 != ZPDCurrentResult) && (0 != ZPDMeasureActive))
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN4, 1);
                          }
                          else
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN4, 0);
                          }
                      }
                      else if(CurrentChannel == 4)
                      {
                          if((0 != ZPDCurrentResult) && (0 != ZPDMeasureActive))
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN3, 1);
                          }
                          else
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN3, 0);
                          }
                      }
                      else if(CurrentChannel == 5)
                      {
                          if((0 != ZPDCurrentResult) && (0 != ZPDMeasureActive))
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN4, 1);
                          }
                          else
                          {
                              R_GPIO_WritePin(TEST_PORT, TEST_PIN4, 0);
                          }
                      }
                      else
                      {
                          /* nothing */
                      }
#endif /* end ISM_HW_DEBUG */        


#ifdef R_ISM_VERBOSE

                  /* Monitoring of ZPD function via terminal output function */

                  if( R_ISM_A_IRQ_DONE )
                    {
                      ISM_Status_bit &= R_ISM_ReadZPDStatus( UnitNumber,
                                                              CurrentChannel,
                                                              &ZPDActive,
                                                              &ZPDDelayState,
                                                              &ZPDTableIndex,
                                                              &ZPDMeasureActive,
                                                              &ZPDCurrentResult,
                                                              &ZPDAccumulatedResult );
#ifdef R_ISM_VERBOSE_PORTZPD
                      ISM_Status_bit &= PORT_Write( R_ISM_VERBOSE_PORTZPD,
                                                    ( u16 )( R_ISM_VERBOSE_BITZPD *
                                                             ZPDMeasureActive ) );
#else
                      sprintf( OutputString, "%d  %d   %02x   %02x   %d   %d   %d\n",
                               ( int )CurrentChannel,
                               ( int )ZPDActive,
                               ( int )ZPDDelayState,
                               ( int )ZPDTableIndex,
                               ( int )ZPDMeasureActive,
                               ( int )ZPDCurrentResult,
                               ( int )ZPDAccumulatedResult );
                      printf( OutputString );

#endif                      
                      R_ISM_A_IRQ_DONE = 0;
                    }
#endif
                }
            }
          else WaitOnReached--;
        }

    } while( WaitOnReached );


  /* Read out last electrical position and correlate it with the PWM table.
     This is now or "zero" position to be used from now on.
     If this is not considered, the restart of the CM would use the beginning of 
     the PWM table (index 0) as zero, which could cause a jump of the pointer. */
     
  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit &= R_ISM_GetPWM( UnitNumber,
                                           CurrentChannel,
                                           &HorizontalPWM_u16,
                                           &VerticalPWM_u16,
                                           &Quadrant );
          for( TableIndex_u16 = 0;
               TableIndex_u16 < R_ISM_TABLESIZE_STD;
               TableIndex_u16++ )
          {
            if( ( CurParameterSet->PWMSTANDARDTABLE )->std
                  [ R_ISM_SPEED_LO ][ R_ISM_DIRECTION_UP ]
                  [ TableIndex_u16 ].cqi == Quadrant )
            {
              if( ( ( ( CurParameterSet->PWMSTANDARDTABLE )->std
                      [ R_ISM_SPEED_LO ][ R_ISM_DIRECTION_UP ]
                      [ TableIndex_u16 ].chp <= HorizontalPWM_u16 ) &&
                    ( ( CurParameterSet->PWMSTANDARDTABLE )->std
                      [ R_ISM_SPEED_LO ][ R_ISM_DIRECTION_UP ]
                      [ TableIndex_u16+1 ].chp >= HorizontalPWM_u16 ) ) ||
                  ( ( ( CurParameterSet->PWMSTANDARDTABLE )->std
                      [ R_ISM_SPEED_LO ][ R_ISM_DIRECTION_UP ]
                      [ TableIndex_u16 ].chp >= HorizontalPWM_u16 ) &&
                    ( ( CurParameterSet->PWMSTANDARDTABLE )->std
                      [ R_ISM_SPEED_LO ][ R_ISM_DIRECTION_UP ]
                      [ TableIndex_u16+1 ].chp <= HorizontalPWM_u16 ) ) )
              {
                /* only the horizontal PWM is used to locate the table index */
                CurParameterSet->ZPDTARGETCOMPENSATION = ( uint32_t )TableIndex_u16;
                break;
              }
            }
          }
        }
      }

  /* Reset Target Position and Current Position, then re-enable channel(s) for CM */

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit &= R_ISM_SetCMTargetPos( UnitNumber,
                                                   CurrentChannel,
                                                   CurParameterSet->ZPDTARGETCOMPENSATION );
          ISM_Status_bit &= R_ISM_SetCMVariables( UnitNumber,
                                                   CurrentChannel,
                                                   R_ISM_RESETALL,
                                                   &DefaultVariableSet );
        }
    }

  ISM_Status_bit &= R_ISM_SetEnableZPD( UnitNumber,
                                         0,    /* Disable ZPD Function */
                                         1 );  /* Power off Analogue HW */

  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        ChannelEnableMask,
                                        1 );
  return( ISM_Status_bit );
}


/*=============================================================================
  FunctionName: R_ISM_SetupCMChannel
  IN  : UnitNumber: Selected Macro
        PWM and Channel Update Frequencies
        Selected Channels
  OUT : Error Flag
  Description : 
  Initializes the CM for the selected channels and globally
  =============================================================================*/
uint8_t R_ISM_SetupCMChannel( uint8_t UnitNumber,
                             uint8_t ChannelEnableMask,
                             uint8_t ClearTargetPosFlag )
{
  uint8_t ISM_Status_bit         = R_ISM_ERR_OK;
  uint8_t CurrentChannel;

  struct rism_var_t DefaultVariableSet;

  /* Initialize global settings */

  R_ISM_A_CURUNIT = UnitNumber;

  ISM_Status_bit =  R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        0 );

  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit &= R_ISM_SetChannelDelay( UnitNumber,
                                                    CurrentChannel,
                                                    CurrentChannel*2,
                                                    CurrentChannel*2+1 );
          if( ClearTargetPosFlag )
            {
              R_ISM_A_CURPARAMSET[ CurrentChannel ]->ZPDTARGETCOMPENSATION = 0L;
              ISM_Status_bit &= R_ISM_SetCMTargetPos( UnitNumber,
                                                       CurrentChannel,
                                                       0L );
            }
          
          ISM_Status_bit &= R_ISM_SetCMVariables( UnitNumber,
                                                   CurrentChannel,
                                                   R_ISM_RESETALL,
                                                   &DefaultVariableSet );
        }
    }
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  /* Enable selected channels and CM */

  ISM_Status_bit =  R_ISM_SetCMEnable( UnitNumber,
                                        ChannelEnableMask,
                                        1 );
  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        1 );

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit &= R_ISM_PortEnable( UnitNumber,
                                               CurrentChannel );
        }
    }
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  return( ISM_Status_bit );
}


/*=============================================================================
  FunctionName: R_ISM_InitializeMovements
  IN  : UnitNumber: Selected Macro
        Parameter Set
  OUT : Error Flag
  Description : 
  Initializes all available channels of a ISM Unit as equal motor types
  =============================================================================*/
uint8_t R_ISM_InitializeMovements( uint8_t  UnitNumber,
                                  uint8_t  ParameterSet )
{
  uint8_t ISM_Status_bit         = R_ISM_ERR_OK;
  uint8_t ChannelNumber;

  ISM_Status_bit &= R_ISM_Reset( UnitNumber );

  for( ChannelNumber = 0; 
       ChannelNumber < R_ISM_CHANNELS;
       ChannelNumber++ )
    {
      ISM_Status_bit &= R_ISM_SelectCMParamset(  ChannelNumber,
                                                  ParameterSet );
    }

  ISM_Status_bit &= R_ISM_DownloadCMParamset( UnitNumber,
                                               R_ISM_CHANNEL_ENALL );

  ISM_Status_bit &= R_ISM_SetupCMChannel( UnitNumber,
                                           R_ISM_CHANNEL_ENALL, /* all channels */
                                           1 );                  /* clear target position */

  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        0 );

  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        1 );
  return( ISM_Status_bit );
}

/*=============================================================================
  FunctionName: R_ISM_MoveCMChannel
  IN  : UnitNumber: Selected Macro
        Selected Channels
        Target Position
  OUT : Error Flag
  Description : 
  Moves the motor(s) of selected channels to given target positions
  =============================================================================*/
uint8_t R_ISM_MoveCMChannel( uint8_t  UnitNumber,
                            uint8_t  ChannelEnableMask,
                            uint32_t  TargetPosition )
{
  uint8_t ISM_Status_bit         = R_ISM_ERR_OK;
  uint8_t CurrentChannel;
  uint8_t EnabledChannel;
  uint8_t GlobalEnable;
  uint8_t WaitOnReached;

  ISM_Status_bit = R_ISM_CreateInterrupt( UnitNumber,
                                           R_ISM_INT_REACHED,
                                           0,
                                           ISM_A_CMREACHEDIRQ );
  ISM_Status_bit &= R_ISM_CreateInterrupt( UnitNumber,
                                            R_ISM_INT_DONE,
                                            0,
                                            ISM_A_CMDONEIRQ );

  /* Enable REACHED interrupts for the moved channels */

  ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                         ChannelEnableMask,
                                         1,   /* DONE  Interrupt enable */
                                         0,   /* ZPDAD Interrupt disable */
                                         1 ); /* Enable Flag */

  /* Enable and set target position for all selected channels sequentially */

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      R_ISM_A_IRQ_REACHED[ CurrentChannel ] = 0;

      if( ( 1 << CurrentChannel ) & ChannelEnableMask )
        {
          ISM_Status_bit = R_ISM_GetCMEnable( UnitNumber,
                                               &EnabledChannel,
                                               &GlobalEnable );
          if( !( EnabledChannel & ( 1 << CurrentChannel ) ) )
            {
              R_ISM_A_IRQ_DONE = 0;           /* wait for another CM pass */
              while( R_ISM_A_IRQ_DONE == 0 );
              ISM_Status_bit =  R_ISM_SetCMEnable( UnitNumber,
                                                    ( 1 << CurrentChannel ),
                                                    1 );
            }
          ISM_Status_bit &= R_ISM_CompensateDeviation( &TargetPosition,
                                                        CurrentChannel );
          ISM_Status_bit &= R_ISM_SetCMTargetPos( UnitNumber,
                                                   CurrentChannel,
                                                   TargetPosition );
        }
    }

  do   /* verify all selected channels to reach their position */
    {
      WaitOnReached = R_ISM_CHANNELS;
  
      for( CurrentChannel = R_ISM_CHANNEL0;
           CurrentChannel < R_ISM_CHANNELS;
           CurrentChannel++ )
        {
          if( ( 1 << CurrentChannel ) & ChannelEnableMask )
            {
              if( R_ISM_A_IRQ_REACHED[ CurrentChannel ] ) 
                {
                  WaitOnReached--;
                }
            }
          else WaitOnReached--;
        }
    } while( WaitOnReached );

  /* Disable REACHED interrupts */

  ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                         ChannelEnableMask,
                                         0,   /* DONE  Interrupt disable */
                                         0,   /* ZPDAD Interrupt disable */
                                         0 ); /* Enable Flag */

  return( ISM_Status_bit );
}


/*=============================================================================
  FunctionName: R_ISM_GetCMCharacteristic
  IN  : UnitNumber: Selected Macro
        PWM and Channel Update Frequencies
        Activated Channels
  OUT : Error Flag
        Count times while CM Busy Flag was detected by polling it
  Description : 
  Gets the counts while CM Busy Flag is set, depending on active channels
  The function uses a fixed default parameter and variable set
  =============================================================================*/
uint8_t R_ISM_GetCMCharacteristic( uint8_t  UnitNumber,
                                  float  PWMFrequency_flt,
                                  float  UpdateFrequency_flt,
                                  uint8_t  ChannelEnableMask,
                                  uint32_t* BusyTimeCount_pu32 )
{
  uint8_t ISM_Status_bit         = R_ISM_ERR_OK;
  uint8_t CurrentChannel;
  uint32_t BusyTimeCounter;
  uint8_t BusyFlag;

  struct rism_var_t DefaultVariableSet;

  /* Initialize global settings */

  ISM_Status_bit =  R_ISM_SetTimebase( UnitNumber,
                                        PWMFrequency_flt,
                                        UpdateFrequency_flt );

  for( CurrentChannel = R_ISM_CHANNEL0;
       CurrentChannel < R_ISM_CHANNELS;
       CurrentChannel++ )
    {
      ISM_Status_bit &= R_ISM_SetCMParameters( UnitNumber,
                                                CurrentChannel,
                                                R_ISM_A_CURPARAMSET[ 0 ] );
      ISM_Status_bit &= R_ISM_SetCMTargetPos( UnitNumber,
                                               CurrentChannel,
                                               0L );
      ISM_Status_bit &= R_ISM_SetCMVariables( UnitNumber,
                                               CurrentChannel,
                                               R_ISM_RESETALL,
                                               &DefaultVariableSet );
    }
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  /* Enable selected channels and CM */

  ISM_Status_bit =  R_ISM_SetCMEnable( UnitNumber,
                                        ChannelEnableMask,
                                        1 );
  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        1 );
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  BusyTimeCounter = 0L;
  BusyFlag        = 0;

  /* poll the busy flag and count its active time */

  while( !BusyFlag )
    {
      ISM_Status_bit &= R_ISM_GetCMBusy( UnitNumber,
                                          &BusyFlag );
    }
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );
  while( BusyFlag )
    {
      BusyTimeCounter++;
      ISM_Status_bit &= R_ISM_GetCMBusy( UnitNumber,
                                          &BusyFlag );
    }
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  *BusyTimeCount_pu32 = BusyTimeCounter;

  /* shut down ISM */

  ISM_Status_bit  = R_ISM_Reset( UnitNumber );

  return( ISM_Status_bit );
}

/*=============================================================================
  FunctionName: R_ISM_PWMMoveTest
  IN  : UnitNumber: Selected Macro
        PWM and Channel Update Frequencies
        Activated Channels
        Recirculation Side
  OUT : Error Flag
  Description : 
  Creates a positive rotation of a motor, by moving the PWM in direct controls
  =============================================================================*/
uint8_t R_ISM_PWMMoveTest( uint8_t UnitNumber,
                          float PWMFrequency_flt,
                          float UpdateFrequency_flt,
                          uint8_t ChannelEnableMask,
                          uint8_t RecirculationSide )
{
  uint8_t ISM_Status_bit         = R_ISM_ERR_OK;
  uint8_t CurChannel;
  uint16_t CurHDutyCycleValue_u16 = R_ISM_PWMDUTY_100;
  uint16_t CurVDutyCycleValue_u16 = R_ISM_PWMDUTY_0;
  uint8_t CurQuadrantInfo    = R_ISM_QUADRANT_1;

  /* Initialize global settings */

  ISM_Status_bit =  R_ISM_SetTimebase( UnitNumber,
                                        PWMFrequency_flt,
                                        UpdateFrequency_flt );
  ISM_Status_bit &= R_ISM_CreateInterrupt( UnitNumber,
                                            R_ISM_INT_DONE,
                                            0,
                                            ISM_A_CMDONEIRQ );
  ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber,
                                         R_ISM_CHANNELS_OFF,
                                         1,   /* DONE  Interrupt enable */
                                         0,   /* ZPDAD Interrupt disable */
                                         1 ); /* Enable Flag */
  ISM_Status_bit &= R_ISM_SetCMEnable( UnitNumber,
                                        R_ISM_GLOBAL_EN,
                                        1 );
  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );

  /* Initialize selected channels */

  for( CurChannel = R_ISM_CHANNEL0;
       CurChannel < R_ISM_CHANNELS;
       CurChannel++ )
    {
      if( ChannelEnableMask & ( 1 << CurChannel ) )
        {
          ISM_Status_bit  = R_ISM_SetRecirculation( UnitNumber,
                                                     CurChannel,
                                                     1,  /* hor. recirculation */
                                                     1,  /* ver. recirculation */
                                                     RecirculationSide );
          ISM_Status_bit &= R_ISM_SetChannelDelay( UnitNumber,
                                                    CurChannel,
                                                    CurChannel*2,
                                                    CurChannel*2+1 );
          ISM_Status_bit &= R_ISM_SetPWM( UnitNumber,
                                           CurChannel,
                                           CurHDutyCycleValue_u16,
                                           CurVDutyCycleValue_u16,
                                           CurQuadrantInfo );
          ISM_Status_bit &= R_ISM_PortEnable( UnitNumber,
                                               CurChannel );
          if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );
        }
    }

  /* PWM updating loop */

  R_ISM_A_IRQ_DONE = 0;
  R_ISM_A_IRQ_UART = 0;

  while( 1 )                          /* run forever */
  {
      if( R_ISM_A_IRQ_DONE )    /* update all PWM on DONE interrupt */
        {
          R_ISM_A_IRQ_DONE = 0;

          switch( CurQuadrantInfo )  /* generate a left rotation */
            {
            case R_ISM_QUADRANT_1:
              CurVDutyCycleValue_u16++;
              CurHDutyCycleValue_u16--;
              if( CurVDutyCycleValue_u16 > R_ISM_PWMDUTY_100 )
                {
                  CurVDutyCycleValue_u16 = R_ISM_PWMDUTY_100;
                  CurHDutyCycleValue_u16 = R_ISM_PWMDUTY_0;
                  CurQuadrantInfo    = R_ISM_QUADRANT_2;
                }
              break;
            case R_ISM_QUADRANT_2:
              CurVDutyCycleValue_u16--;
              CurHDutyCycleValue_u16++;
              if( CurHDutyCycleValue_u16 > R_ISM_PWMDUTY_100 )
                {
                  CurHDutyCycleValue_u16 = R_ISM_PWMDUTY_100;
                  CurVDutyCycleValue_u16 = R_ISM_PWMDUTY_0;
                  CurQuadrantInfo    = R_ISM_QUADRANT_3;
                }
              break;
            case R_ISM_QUADRANT_3:
              CurVDutyCycleValue_u16++;
              CurHDutyCycleValue_u16--;
              if( CurVDutyCycleValue_u16 > R_ISM_PWMDUTY_100 )
                {
                  CurVDutyCycleValue_u16 = R_ISM_PWMDUTY_100;
                  CurHDutyCycleValue_u16 = R_ISM_PWMDUTY_0;
                  CurQuadrantInfo    = R_ISM_QUADRANT_4;
                }
              break;
            case R_ISM_QUADRANT_4:
              CurVDutyCycleValue_u16--;
              CurHDutyCycleValue_u16++;
              if( CurHDutyCycleValue_u16 > R_ISM_PWMDUTY_100 )
                {
                  CurHDutyCycleValue_u16 = R_ISM_PWMDUTY_100;
                  CurVDutyCycleValue_u16 = R_ISM_PWMDUTY_0;
                  CurQuadrantInfo = R_ISM_QUADRANT_1;
                }
              break;
            }

          for( CurChannel = R_ISM_CHANNEL0;
               CurChannel < R_ISM_CHANNELS;
               CurChannel++ )
            {
              if( ChannelEnableMask & ( 1 << CurChannel ))
                {
                  ISM_Status_bit = R_ISM_SetPWM( UnitNumber,
                                                    CurChannel,
                                                    CurHDutyCycleValue_u16,
                                                    CurVDutyCycleValue_u16,
                                                    CurQuadrantInfo);
                  if( ISM_Status_bit == R_ISM_ERR_NG ) return( R_ISM_ERR_NG );
                }
            }
        }
  }

  ISM_Status_bit  = R_ISM_Reset( UnitNumber );

  for( CurChannel = R_ISM_CHANNEL0;
       CurChannel < R_ISM_CHANNELS;
       CurChannel++ )
    {
      ISM_Status_bit &= R_ISM_PortDisable( UnitNumber,
                                            CurChannel );
    }

  return( ISM_Status_bit );
}

void StepperMotorSetSpeed(float speed)
{

 R_ISM_SetTimebase( 0,
                                                 16.0E3,
                                               speed );

}

   
