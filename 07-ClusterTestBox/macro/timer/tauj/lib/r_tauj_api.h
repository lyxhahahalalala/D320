/*
****************************************************************************
PROJECT : TAUJ driver
FILE    : $Id: r_tauj_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for TAUJ macro
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


#ifndef R_TAUJ_API_H_
#define R_TAUJ_API_H_


/***********************************************************
  Title: TAUJ API

  An application using the TAUJ should include this header file.

  General information:
  The TAUJ macro provides comprehensive timer functionalty for devices.
  There are 4 channels which can be set to a variety of modes including:
    - Interval timer function
    - TINn input pulse interval measurement function
    - TINn input interval timer function
    - TINn input signal width measurement function
    - Overflow interrupt output function (during TINn
      width measurement)
    - TINn input position detection function
    - TINn input period count detection function
    - Overflow interrupt output function (during TINn
      input period count detection)
    - PWM output function

    The are also 4 independent clocks available which can be set to a variety of
    frequencies and each timer channel can be assigned to one of these.

    The channels can also be linked in a series of master and slave
    configurations i.e. allowing the output of one channel to trigger
    another.
*/


/***********************************************************
  Section: Global Types
*/


/* option to use SVSTOP */
#define LOC_USE_SVSTOP        (0x0000)
#define LOC_DO_NOT_USE_SVSTOP (0x0080)

#define LOC_SVSTOP           (LOC_USE_SVSTOP)


/***********************************************************
  Enum: r_tauj_Error_t

  TAUJ macro driver error code.

  Values:
  R_TAUJ_ERR_OK                - No error
  R_TAUJ_ERR_INVALID_MODE      - option not allowed in this mode
  R_TAUJ_ERR_RANGE             - Unspecified error
  R_TAUJ_ERR_INSTANCE          - Unit, instance not supported
  R_TAUJ_ERR_PORTINIT          - port init failed
  R_TAUJ_ERR_REG_UPDATE_FAIL   - register update failed
  R_TAUJ_ERR_LAST              - Delimeter
*/

typedef enum {
    R_TAUJ_ERR_OK           = 0x00,
    R_TAUJ_ERR_INVALID_MODE = 0x01,
    R_TAUJ_ERR_RANGE        = 0x02,
    R_TAUJ_ERR_INSTANCE     = 0x03,
    R_TAUJ_ERR_PORTINIT,
    R_TAUJ_ERR_REG_UPDATE_FAIL,
    R_TAUJ_ERR_LAST
} r_tauj_Error_t;


/***********************************************************
  Enum: r_tauj_Interrupt_t

  TAUJ macro interrupt type. List of all available interrupts for this macro

  Values:
    R_TAUJ_INT_CH0-3  - General purpose interrupt
    R_TAUJ_INT_NUM    - Max number of interrupt channels, delimiter
*/

typedef enum {
    R_TAUJ_INT_CH0 = 0,
    R_TAUJ_INT_CH1,
    R_TAUJ_INT_CH2,
    R_TAUJ_INT_CH3,
    R_TAUJ_INT_NUM   /* delimiter */
} r_tauj_Interrupt_t;


/***********************************************************
  Enum: r_tauj_Mode_t

  TAUJ macro mode type, this set the mode for each channel.

  Values:
    R_TAUJ_INTERVAL_TIMER_MODE             - Interval timer mode
    R_TAUJ_RFU_JUDGE_MODE                  - RFU judge mode
    R_TAUJ_CAPTURE_MODE                    - capture mode
    R_TAUJ_RFU_EVENT_COUNT_MODE            - RFU event count mode
    R_TAUJ_ONE_COUNT_MODE                  - One count mode
    R_TAUJ_RFU_TRIGGER_START_MODE          - RFU trigger start mode
    R_TAUJ_CAPTURE_ONE_COUNT_MODE          - Capture one count mode
    R_TAUJ_RFU_JUDGE_ONE_COUNT_MODE        - RFU judge one count mode
    R_TAUJ_RFU_INTERVAL_ONE_COUNT_MODE     - RFU interval one count mode
    R_TAUJ_RFU_UP_DOWN_COUNT_MODE          - RFU up/down count mode
    R_TAUJ_RFU_PULSE_ONE_COUNT_MODE        - RFU pule one count mode
    R_TAUJ_COUNT_CAPTURE_MODE              - Count capture moe
    R_TAUJ_GATE_COUNT_MODE                 - Gate count mode
    R_TAUJ_CAPTURE_GATE_COUNT_MODE         - Capture gate count mode
*/

typedef enum {
    R_TAUJ_INTERVAL_TIMER_MODE = 0,
    R_TAUJ_RFU_JUDGE_MODE,
    R_TAUJ_CAPTURE_MODE,
    R_TAUJ_RFU_EVENT_COUNT_MODE,
    R_TAUJ_ONE_COUNT_MODE,
    R_TAUJ_RFU_TRIGGER_START_MODE,
    R_TAUJ_CAPTURE_ONE_COUNT_MODE,
    R_TAUJ_RFU_JUDGE_ONE_COUNT_MODE,
    R_TAUJ_RFU_INTERVAL_ONE_COUNT_MODE,
    R_TAUJ_RFU_UP_DOWN_COUNT_MODE,
    R_TAUJ_RFU_PULSE_ONE_COUNT_MODE,
    R_TAUJ_COUNT_CAPTURE_MODE,
    R_TAUJ_GATE_COUNT_MODE,
    R_TAUJ_CAPTURE_GATE_COUNT_MODE
} r_tauj_Mode_t;


/***********************************************************
  Enum: r_tauj_Trigger_t

  TAUJ macro trigger type, this set the trigger source for each channel (CMOR).

  Values:
    R_TAUJ_NONE                   - Valid only in software trigger start (Deselect other trigger sources)
    R_TAUJ_VALID_EDGE_TIN         - Selects the valid-edge-detected TINn input signal. (Select the TINn 
                                     valid edge, using CMURn:TIS1 and TIS0
    R_TAUJ_VALID_EITHER_TIN       - Selects the valid-edge-detected TINn input signal as a start signal, and the
                                     reverse-edge-detected TINn input signal as a stop signal (capture signal)
                                     (Sets both rising and falling edges as the valid edge using CMURn:TIS1 and TIS0
    R_TAUJ_START_TRIG_FROM_MASTER - RFU (Selects the start trigger signal of the master channel 
    
    R_TAUJ_INT_FROM_MASTER        - Selects the INTn output signal of the master channel
    R_TAUJ_INT_FROM_UPPER         - Selects the INTn output signal of the upper channel, regardless of the 
                                     setting of the master
    R_TAUJ_TOUT_DEAD_TIME         - Selects the dead-time control trigger signal of the TOUTn generation unit
    R_TAUJ_UP_DOWN_FROM_MASTER    - Selects the up/down control trigger signal of the master channel       
*/

typedef enum {
    R_TAUJ_NONE = 0,             
    R_TAUJ_VALID_EDGE_TIN,            
    R_TAUJ_VALID_EITHER_TIN,                                 
    R_TAUJ_RFU_START_TRIG_FROM_MASTER,     
    R_TAUJ_INT_FROM_MASTER,       
    R_TAUJ_RFU_INT_FROM_UPPER,               
    R_TAUJ_RFU_TOUT_DEAD_TIME,        
    R_TAUJ_RFU_UP_DOWN_FROM_MASTER
} r_tauj_Trigger_t;


/***********************************************************
  Enum: r_tauj_TimeInterval_t

  TAUJ macro time interval selection for the CK0-3.  The period
  depends on the clock freguency selected externally and will
  further divide the frequency by the factor below.

  i.e. assume clock is 100MHz externally
         100MHz / 2^9 about 195.3KHz about 5.12us
             100MHz / 2^16 about 1.525KHz about 0.6ms

  Configuration parameter for  interval.

  Values:

  R_TAUJ_TIME_INTERVAL_0   - time interval 2^0
  R_TAUJ_TIME_INTERVAL_1   - time interval 2^1
  R_TAUJ_TIME_INTERVAL_2   - time interval 2^2
  R_TAUJ_TIME_INTERVAL_3   - time interval 2^3
  R_TAUJ_TIME_INTERVAL_4   - time interval 2^4
  R_TAUJ_TIME_INTERVAL_5   - time interval 2^5
  R_TAUJ_TIME_INTERVAL_6   - time interval 2^6
  R_TAUJ_TIME_INTERVAL_7   - time interval 2^7
  R_TAUJ_TIME_INTERVAL_8   - time interval 2^8
  R_TAUJ_TIME_INTERVAL_8   - time interval 2^9 
  R_TAUJ_TIME_INTERVAL_10  - time interval 2^10
  R_TAUJ_TIME_INTERVAL_11  - time interval 2^11
  R_TAUJ_TIME_INTERVAL_12  - time interval 2^12
  R_TAUJ_TIME_INTERVAL_13  - time interval 2^13
  R_TAUJ_TIME_INTERVAL_14  - time interval 2^14
  R_TAUJ_TIME_INTERVAL_15  - time interval 2^15
*/

typedef enum {
    R_TAUJ_TIME_INTERVAL_0 = 0,
    R_TAUJ_TIME_INTERVAL_1,
    R_TAUJ_TIME_INTERVAL_2,
    R_TAUJ_TIME_INTERVAL_3,
    R_TAUJ_TIME_INTERVAL_4,
    R_TAUJ_TIME_INTERVAL_5,
    R_TAUJ_TIME_INTERVAL_6,
    R_TAUJ_TIME_INTERVAL_7,
    R_TAUJ_TIME_INTERVAL_8,       
    R_TAUJ_TIME_INTERVAL_9,
    R_TAUJ_TIME_INTERVAL_10,
    R_TAUJ_TIME_INTERVAL_11,
    R_TAUJ_TIME_INTERVAL_12,
    R_TAUJ_TIME_INTERVAL_13,
    R_TAUJ_TIME_INTERVAL_14,
    R_TAUJ_TIME_INTERVAL_15
} r_tauj_TimeInterval_t;


/***********************************************************
  Enum: r_tauj_Channel_t

  TAUJ channel selection 0 to xx

  Values:
  R_TAUJ_CHANNEL_0        - Channel 0
  R_TAUJ_CHANNEL_1        - Channel 1
  R_TAUJ_CHANNEL_2        - Channel 2
  R_TAUJ_CHANNEL_3        - Channel 3   
  R_TAUJ_CHANNEL_ALL      - option to combine all the channels
*/

typedef enum {
  R_TAUJ_CHANNEL_0   = 0,
  R_TAUJ_CHANNEL_1   = 1u,
  R_TAUJ_CHANNEL_2   = 2u,
  R_TAUJ_CHANNEL_3   = 3u,
  R_TAUJ_CHANNEL_ALL = 4u
} r_tauj_Channel_t;

/* 4 timer channels */
#define R_TAUJ_NUM_CHANNELS (4u)


/***********************************************************
  Enum: r_tauj_Clock_t

  TAUJ clock selection 0 to 3

  Values:
  R_TAUJ_CK_0        - CK0
  R_TAUJ_CK_1        - CK1
  R_TAUJ_CK_2        - CK2
  R_TAUJ_CK_3        - CK3
*/

typedef enum {
  R_TAUJ_CK_0 = 0,
  R_TAUJ_CK_1,
  R_TAUJ_CK_2,
  R_TAUJ_CK_3
} r_tauj_Clock_t;


/***********************************************************
  Enum: r_tauj_CountClock_t

  TAUJ count clock selection

  Values:
  R_TAUJ_COUNT_CLK_PRESCALER       - clock pre-scaler
  R_TAUJ_COUNT_CLK_TIN             - Tin clock
  R_TAUJ_COUNT_CLK_RFU             - RFU clock
  R_TAUJ_COUNT_CLK_MASTER_INT_RFU  - clock master RFU
*/

typedef enum {
  R_TAUJ_COUNT_CLK_PRESCALER = 0,
  R_TAUJ_COUNT_CLK_TIN,
  R_TAUJ_COUNT_CLK_RFU,
  R_TAUJ_COUNT_CLK_MASTER_INT_RFU
} r_tauj_CountClock_t;


/***********************************************************
  Enum: r_tauj_TinEdge_t

  TAUJ TIN edge selection

  Values:
  R_TAUJ_FALLING_EDGE      - falling edge
  R_TAUJ_RISING_EDGE       - rising edge
  R_TAUJ_BOTH_LOW_WIDTH    - both low width
  R_TAUJ_BOTH_HIGH_WIDTH   - both high width

*/

typedef enum {
  R_TAUJ_FALLING_EDGE = 0,
  R_TAUJ_RISING_EDGE,
  R_TAUJ_BOTH_LOW_WIDTH,
  R_TAUJ_BOTH_HIGH_WIDTH
} r_tauj_TinEdge_t;


/***********************************************************
  Enum: r_tauj_Overflow_t

  TAUJ TIN overflow selection (COSC)

  Values:
  
  R_TAUJ_OVERFLOW_OPTION_0 - Captures the OVF and counter values upon detection of a valid edge of capture input.

  R_TAUJ_OVERFLOW_OPTION_1 - Sets OVF when the counter overflows (FFFFH ? 0000H).
                              Captures the counter value upon detection of a valid edge of capture input.

  R_TAUJ_OVERFLOW_OPTION_2 - Captures the OVF value upon detection of a valid edge of capture input.
                              Captures FFFFH and stores it to CDRn when the counter overflows (FFFFH ? 0000H).
                              The valid edge of the capture input detected the next time is ignored.

  R_TAUJ_OVERFLOW_OPTION_3 - Sets OVF when the counter overflows (FFFFH ? 0000H) and captures FFFFH and stores it
                              to CDRn.
                              The valid edge of the capture input detected the next time is ignored.
*/

typedef enum {
  R_TAUJ_OVERFLOW_OPTION_0 = 0,
  R_TAUJ_OVERFLOW_OPTION_1,
  R_TAUJ_OVERFLOW_OPTION_2,
  R_TAUJ_OVERFLOW_OPTION_3
} r_tauj_Overflow_t;


/***********************************************************
  Enum: r_tauj_SimultaneousTrigger_t

  TAUJ simultaneous rewrite mode

  Values:
  R_TAUJ_FROM_MASTER - Controls simultaneous rewrite of the data register by 
                        using the signal of master channel
  R_TAUJ_FROM_UPPER  - Controls simultaneous rewrite of the data register by 
                        using the signal of another upper channel
*/

typedef enum {
  R_TAUJ_FROM_MASTER = 0,
  R_TAUJ_FROM_UPPER
} r_tauj_SimultaneousTrigger_t;

/***********************************************************
  Section: Global Functions
  
*/

/***********************************************************
  Function: R_TAUJ_Init

  Driver init function. Provides a complete initialization of the TAUJ.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_Init(uint32_t Unit);


/***********************************************************
  Function: R_TAUJ_InitChannel

  Initialisation function for TAUJ channel. Provides a complete initialization of one of
  the TAUJ Channels.

  Parameters:
  Unit             - Instance number
  Channel          - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_InitChannel(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_SetClockPeriod

  Set the timer period for one of the four channel clocks CK0-3.

  Parameters:
  Unit          - Instance number
  int           - TAUJ clock <r_tauj_Clock_t>
  TimeInterval  - TAUJ time interval <r_tauj_TimeInterval_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetClockPeriod(uint32_t Unit, r_tauj_Clock_t Clock, r_tauj_TimeInterval_t TimeInterval);


/***********************************************************
  Function: R_TAUJ_SetCK3BaudRateClock

  Set the timer period for the CK3 channel clocks CK0-3.

  Parameters:
  Unit       - Instance number
  Period     - TAUJ baud rate clock divisor, range is 0 - 255

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetCK3BaudRateClock(uint32_t Unit, uint8_t Period);


/***********************************************************
  Function: R_TAUJ_SetClkSource

  Set the clock source for a TAUJ channel can be either CK0-3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  Clock      - TAUJ clock <r_tauj_Clock_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetClkSource(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_Clock_t Clock);


/***********************************************************
  Function: R_TAUJ_SetMode

  Set the mode for a TAUJ channel 0-3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  Mode       - TAUJ Mode <r_tauj_Mode_t>

   Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetMode(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_Mode_t Mode);


/***********************************************************
  Function: R_TAUJ_SetCaptureOverflow

  Set the Capture overflow option for a TAUJ channel 0-3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  Overflow   - TAUJ Mode <r_tauj_Overflow_t>

   Returns:
  see: <r_tauj_Overflow_t>
*/

r_tauj_Error_t R_TAUJ_SetCaptureOverflow(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_Overflow_t Overflow);


/***********************************************************
  Function: R_TAUJ_SetIntAtStartUp

  Enables the INT at startup, bit MD0 in the CMOR.

  Parameters:
  Unit          - Instance number
  Channel       - TAUJ channel number <r_tauj_Channel_t>
  IntAtStartUp  - TRUE enable int at start up, FALSE do not

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_EnableIntAtStartUp(uint32_t Unit, r_tauj_Channel_t Channel, uint8_t IntAtStartUp);


/***********************************************************
  Function: R_TAUJ_SetMaster

  Set the mode for a TAUJ channel 0 to 3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  Master     - Sets either a Master channel (TRUE) or Slave (FALSE)

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetMaster(uint32_t Unit, r_tauj_Channel_t Channel, uint8_t Master);


/***********************************************************
  Function: R_TAUJ_SetTrigger

  Set the trigger source for a TAUJ channel 0 to 3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  Trigger    - see <r_tauj_Trigger_t> 

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetTrigger(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_Trigger_t Trigger);


/***********************************************************
  Function: R_TAUJ_ClearOverflow

  Clear the overflow flag for a channel.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_ClearOverflow(uint32_t Unit, r_tauj_Channel_t Channel);

/***********************************************************
  Function: R_TAUJ_EnableSimultaneousRewrite
  
  Set the trigger source for a TAUJ channel 0 to 3.

  Parameters:
  Unit                      - Instance number
  Channel                   - TAUJ channel number <r_tauj_Channel_t>
  SimultaneousTriggerSource - see <r_tauj_SimultaneousTrigger_t> 

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_EnableSimultaneousRewrite(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_SimultaneousTrigger_t SimultaneousTriggerSource);


/***********************************************************
  Function: R_TAUJ_EnableSynchronousChannelOperation
  
  Enables Synchronous Channel Operation for a TAUJ channel 0 to 3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>


  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_EnableSynchronousChannelOperation(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_SetCountClock

  Set the count clock source for a TAUJ channel 0 to 3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  CountClock - see <r_tauj_CountClock_t> 

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetCountClock(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_CountClock_t CountClock);


/***********************************************************
  Function: R_TAUJ_SetTinEdge

  Set the edge for the TIN detection TAUJ channel 0 to 3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  CountClock - see <r_tauj_TinEdge_t> 

  Returns:
  see: <r_tauj_Error_t>
*/

 r_tauj_Error_t R_TAUJ_SetTinEdge(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_TinEdge_t Edge);


/***********************************************************
  Function: R_TAUJ_SetPeriod

  Set the period for the channel counter, this is used for the CDR this is
  effectivety a count value for the CDR register.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  Period     - TAUJ count period

  Returns:
  see: <r_tauj_Error_t>
*/

 r_tauj_Error_t R_TAUJ_SetPeriod(uint32_t Unit, r_tauj_Channel_t Channel, uint32_t Period);


/***********************************************************
  Function: R_TAUJ_Start

  Starts the selected timer channel.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_Start(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_Stop

  Stops the selected timer channel.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_Stop(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_OutputEnable

  Enables the selected timer channel output enable pin for use
  with the count operation.  

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_OutputEnable(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_OutputWrite

  Writes the specified value to the timer output. For use with
  Independent Channel Output Mode Controlled by Software.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>
  Value      - Value to write to the output

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_OutputWrite(uint32_t Unit, r_tauj_Channel_t Channel, uint8_t Value);


/***********************************************************
  Function: R_TAUJ_OutputDisable

  Disables the selected timer channel output enable pin for use
  with the count operation.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_OutputDisable(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_Disable

  Disables the selected timer channel.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_Disable(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_SetInvertOutputTOUT

  Invert the output of the TOUTn pin.

  Parameters:
  Unit          - Instance number
  Channel       - TAUJ channel number <r_tauj_Channel_t>
  InvertOutput  - TRUE invert output, FALSE normal no invert

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_SetInvertOutputTOUT(uint32_t Unit, r_tauj_Channel_t Channel, uint8_t InvertOutput);


/***********************************************************
  Function: R_TAUJ_ReloadDataTrigger

  specifies the channel for which simultaneous rewrite is executed

  Parameters:
  Unit          - Instance number
  Channel       - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  see: <r_tauj_Error_t>
*/

r_tauj_Error_t R_TAUJ_ReloadDataTrigger(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_GetCaptureData

  Retrevies the data count value in the CDR register.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  uint32_t   - CDR register value
*/

uint32_t R_TAUJ_GetCaptureData(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_GetCntData
   Retrevies the data count value in the CNT register.

  Parameters:
  Unit       - Instance number
  Channel    - TAUJ channel number <r_tauj_Channel_t>

  Returns:
  uint32_t   - CNT register value
*/

uint32_t R_TAUJ_GetCntData(uint32_t Unit, r_tauj_Channel_t Channel);


/***********************************************************
  Function: R_TAUJ_ReadReg

  Retrevies the value in one the TAUJ registers.

  Parameters:
  Unit       - Instance number
  Offset     - Register offset

  Returns:
  see: <r_tauj_Error_t>
*/

 uint16_t R_TAUJ_ReadReg(uint32_t Unit, uint32_t Offset);


/***********************************************************
  Function: R_TAUJ_WriteReg

  Writes the value in one the TAUJ registers.

  Parameters:
  Unit       - Instance number
  Offset     - Register offset
  Data       - Data to write

  Returns:
  none
*/

 void R_TAUJ_WriteReg(uint32_t Unit, uint32_t Offset, uint16_t Data);


/***********************************************************
  Function: R_TAUJ_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback)
                  with two paramerters (unit and error number)

  Returns:
  void

*/

 void R_TAUJ_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error));


/***********************************************************
  Function: R_TAUJ_SetIsrCallback

  Assign a user callback for the given interrupt of a unit.

  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  IntType       - type of interrupt; see: <r_tauj_Interrupt_t>
  Isr           - Isr function pointer

  Returns:
  see: <r_tauj_Error_t>
*/

 r_tauj_Error_t R_TAUJ_SetIsrCallback(uint32_t Unit, r_tauj_Interrupt_t IntType,
                                 void (*Isr)(void));


/***********************************************************
  Function: R_TAUJ_Isr

  Driver interrupt service routine. 
  
  This function shall be called by sevice interrupt handler 

  Parameters:
  Unit       - Instance number
  Interrupt  - channel interrupt
  
  Returns:
  void
*/

 void R_TAUJ_Isr(uint32_t Unit, r_tauj_Interrupt_t Interrupt);


/***********************************************************
  Function: R_TAUJ_EnableInt

  Enable interrupt 'Id'.
  
  Parameters:
  Unit       - Unit number of TAUJ in device
  Interrupt  - Identifier of interrupt
    
  Returns:
  void
*/
 r_tauj_Error_t R_TAUJ_EnableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt);


/***********************************************************
  Function: R_TAUJ_DisableInt

  Disable interrupt 'Id'.
  
  Parameters:
  Unit       - Unit number of TAUJ in device
  Interrupt  - Identifier of interrupt
    
  Returns:
  void
*/
 r_tauj_Error_t R_TAUJ_DisableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt);


/***********************************************************
  Function: R_TAUJ_PortInit

  Initialise the port

  Parameters:
  None

  Returns:
  void

*/

 void R_TAUJ_PortInit(void);



/***********************************************************
  Function: R_TAUJ_GetClock

  Gets the clock for the selected TauJ unit

  Parameters:
  Unit         - TAUJ instance number

  Returns:
  clock frequency

*/
 uint32_t R_TAUJ_GetClock(uint32_t Unit);


/***********************************************************
  Function: R_TAUJ_SetFilterBypass

  Sets the filter bypass for the selected TauJ unit

  Parameters:
  Unit      - TAUJ isntance number
  ID        - clock domain Clock Selector ID
  
  Returns:
  0            - TAUJ unit in range
  1            - TAUJ unit out of range

*/
 uint32_t R_TAUJ_SetFilterBypass(uint32_t Unit);

#endif /* R_TAUJ_API_H_  */
