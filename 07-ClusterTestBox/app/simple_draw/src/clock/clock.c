/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* (c) 2014 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/


/******************************************************************************
* File Name     :clock.c
* Version       : 1.0
* Device(s)     : R7F701403xAFP RH850/D1L
* Tool-Chain    : GHS
* Description   : This file is a sample of the clock setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 30.09.2016 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include    "iodefine.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void   init_clocks( void );

/******************************************************************************
Private global variables and functions
******************************************************************************/
static  void    osc_clock( void );
static  void    set_clock_domain( void );


/******************************************************************************
* Function Name : void R_CLOCK_Init( void )
* Description   : This function initializes the clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
void init_clocks( void )
{
    osc_clock();                                /* Clock oscillation         */
    set_clock_domain();                         /* Domain clock setting      */
}

/******************************************************************************
* Function Name : static void osc_clock( void )
* Description   : This function oscillates the clock (main , sub and PLL clock).
* Argument      : none
* Return Value  : none
******************************************************************************/
static void osc_clock( void )
{
    SYS.MOSCC.UINT32  = 0x05u;                       /* set MainOSC gain (8 MHz) */
    SYS.MOSCST.UINT32 = 0x8000u;                     /* set MainOSC stabilization time to 4 ms */

    protected_write(SYS.PROTCMD0.UINT32, SYS.PROTS0.UINT32, SYS.MOSCE.UINT32, 0x01u); /* trigger enable (protected write) */

    while(0x4u != (SYS.MOSCS.UINT32 & 0x04u)){}    /* wait for active MainOSC */

    /* set PLL0 to 480MHz */
    SYS.PLL0C.UINT32 = 0x6000003B;   /* PLL0 configuration:
                            fixed frequency mode(dithering disabled) =
                            Nr = 60
                            Pr = 1 =1
                            Mr = 1 =1
                            fPLL = MainOsc * Nr /(Mr * Pr)
                            fPLL = 8Mhz * 60 / (1 *  1) = 480 MHz */
    protected_write(SYS.PROTCMD1.UINT32, SYS.PROTS1.UINT32, SYS.PLL0E.UINT32, 0x01u); /* trigger PLL0 enable (protected write) */
    while((SYS.PLL0S.UINT32 & (1<<2)) != (1<<2)){}   /* wait until PLL0 stable bit is set */

    /* Set PLL1 to 480MHz (PLL1CLK = 480 MHz) */
    SYS.PLL1C.UINT32 = 0x6000003B;   /* PLL1 configuration:
                            fixed frequency mode (dithering disabled)
                            Nr = 120
                            Pr = 1
                            Mr = 1
                            fPLL = MainOsc * Nr /(Mr * Pr)
                            fPLL = 8Mhz *  120 /(1 *  1) = 960 MHz */
    protected_write(SYS.PROTCMD1.UINT32, SYS.PROTS1.UINT32, SYS.PLL1E.UINT32, 0x01u);   /* trigger PLL1 enable (protected write) */
    while((SYS.PLL1S.UINT32 & (1<<2)) != (1<<2)){}  /* wait until PLL1 stable bit is set */
 }

/******************************************************************************
* Function Name : static void set_clock_domain( void )
* Description   : This function sets the domain clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
static void set_clock_domain( void )
{
    /* set CPU clock divider to 4 */
    protected_write(SYS.PROTCMD1.UINT32, SYS.PROTS1.UINT32, SYS.CKSC_ICPUCLKD_CTL.UINT32, 0x03u); /* divider 4 */
    while(0x03u != SYS.CKSC_ICPUCLKD_ACT.UINT32){}

    /* set CPU clock to PLL0CLK */
    protected_write(SYS.PROTCMD1.UINT32, SYS.PROTS1.UINT32, SYS.CKSC_ICPUCLKS_CTL.UINT32, 0x03u); /* PLL0CLK */
    while(0x03u != SYS.CKSC_ICPUCLKS_ACT.UINT32){}
  
    /* set PLLFIXCLK to PLL1CLK (480MHz) */
    protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_IPLLFIXS_CTL.UINT32, 0x01u); /* PLL1CLK */
    while(0x01u != SYS.CKSC_IPLLFIXS_ACT.UINT32){}
  
    /* set clock divider for CLKFIX to 12 (480MHz / 6 = 80 MHz)*/
    protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKDV_ICLKFIXD_CTL.UINT32, 0x06u); /* PLLFIXCLK / 6 */
    while(0x03u != SYS.CKDV_ICLKFIXD_STAT.UINT32){}
  
    /* set CLKFIX to PLLFIXCLK / 6 (80 MHz) */
    protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_ICLKFIXS_CTL.UINT32, 0x01u); /* PLLFIXCLK / 6 */
    while(0x01u != SYS.CKSC_ICLKFIXS_ACT.UINT32){}
  
    /* set CLKJIT divider to PLL0 / 6 (80 MHz) */
    protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKDV_ICLKJITD_CTL.UINT32, 0x06u); /* divider 6 selection */
    while(0x03u != SYS.CKDV_ICLKJITD_STAT.UINT32){}
  
    /* set CLKJIT to CKDV_ICLKJITD_CTL (80 MHz) */
    protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_ICLKJITS_CTL.UINT32, 0x01u); /* CKDV_ICLKJITD_CTL selection */
    while(0x01u != SYS.CKSC_ICLKJITS_ACT.UINT32){}

    /* set ADCE clock -> CKLJIT /4 (20 MHz) */
    protected_write(SYS.PROTCMDD1.UINT32,SYS.PROTSD1.UINT32,SYS.CKSC_IADCED_CTL.UINT32 ,0x02u); /* CKLJIT /4 */
    while(0x01u !=  SYS.CKSC_IADCED_ACT.UINT32){}
}
