/*
****************************************************************************
PROJECT :  Vlib device driver
FILE    : $Id: r_dev_int_types.h 3010 2014-06-17 14:48:17Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for CLK macro
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

#ifndef DEV_INT_TYPES_H_
#define DEV_INT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Device sopecific interrupt types

*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_dev_IntSel_t

  All possible interrupt control registers .

*/
typedef enum 
{
/* starter, do not remove !!! */
R_DEV_INT_FIRST                = 0, 
R_DEV_INT_WDTA0                = R_DEV_INT_FIRST,
R_DEV_INT_WDTA1                ,
R_DEV_INT_RTCA01S              ,
R_DEV_INT_RTCA0AL              ,
R_DEV_INT_RTCA0R               ,
R_DEV_INT_AWOTM0               ,
R_DEV_INT_P0                   ,
R_DEV_INT_P1                   ,
R_DEV_INT_P2                   ,
R_DEV_INT_P3                   ,
R_DEV_INT_P4                   ,
R_DEV_INT_P5                   ,
R_DEV_INT_P6                   ,
R_DEV_INT_P7                   ,
R_DEV_INT_P8                   ,
R_DEV_INT_P9                   ,
R_DEV_INT_P10                  ,
R_DEV_INT_TAUB0I0              ,
R_DEV_INT_TAUB0I1              ,
R_DEV_INT_TAUB0I2              ,
R_DEV_INT_TAUB0I3              ,
R_DEV_INT_TAUB0I4              ,
R_DEV_INT_TAUB0I5              ,
R_DEV_INT_TAUB0I6              ,
R_DEV_INT_TAUB0I7              ,
R_DEV_INT_TAUB0I8              ,
R_DEV_INT_TAUB0I9              ,
R_DEV_INT_TAUB0I10             ,
R_DEV_INT_TAUB0I11             ,
R_DEV_INT_TAUB0I12             ,
R_DEV_INT_TAUB0I13             ,
R_DEV_INT_TAUB0I14             ,
R_DEV_INT_TAUB0I15             ,
R_DEV_INT_TAUB1I0              ,
R_DEV_INT_TAUB1I1              ,
R_DEV_INT_TAUB1I2              ,
R_DEV_INT_TAUB1I3              ,
R_DEV_INT_TAUB1I4              ,
R_DEV_INT_TAUB1I5              ,
R_DEV_INT_TAUB1I6              ,
R_DEV_INT_TAUB1I7              ,
R_DEV_INT_TAUB1I8              ,
R_DEV_INT_TAUB1I9              ,
R_DEV_INT_TAUB1I10             ,
R_DEV_INT_TAUB1I11             ,
R_DEV_INT_TAUB1I12             ,
R_DEV_INT_TAUB1I13             ,
R_DEV_INT_TAUB1I14             ,
R_DEV_INT_TAUB1I15             ,
R_DEV_INT_TAUB2I0              ,
R_DEV_INT_TAUB2I1              ,
R_DEV_INT_TAUB2I2              ,
R_DEV_INT_TAUB2I3              ,
R_DEV_INT_TAUB2I4              ,
R_DEV_INT_TAUB2I5              ,
R_DEV_INT_TAUB2I6              ,
R_DEV_INT_TAUB2I7              ,
R_DEV_INT_TAUB2I8              ,
R_DEV_INT_TAUB2I9              ,
R_DEV_INT_TAUB2I10             ,
R_DEV_INT_TAUB2I11             ,
R_DEV_INT_TAUB2I12             ,
R_DEV_INT_TAUB2I13             ,
R_DEV_INT_TAUB2I14             ,
R_DEV_INT_TAUB2I15             ,
R_DEV_INT_ADC0TSN              ,
R_DEV_INT_ADC0SG1              ,
R_DEV_INT_ADC0SG2              ,
R_DEV_INT_ADC0SG3              ,
R_DEV_INT_RCANGERR             ,
R_DEV_INT_RCANGRECC            ,
R_DEV_INT_RCAN0ERR             ,
R_DEV_INT_RCAN0REC             ,
R_DEV_INT_RCAN0TRX             ,
R_DEV_INT_CSIG0RE              ,
R_DEV_INT_CSIG0R               ,
R_DEV_INT_CSIG0IC              ,
R_DEV_INT_CSIH0RE              ,
R_DEV_INT_CSIH0R               ,
R_DEV_INT_CSIH0C               ,
R_DEV_INT_CSIH0JC              ,
R_DEV_INT_RLIN30S              ,
R_DEV_INT_RLIN30R              ,
R_DEV_INT_RLIN30T              ,
R_DEV_INT_RLIN31S              ,
R_DEV_INT_RLIN31R              ,
R_DEV_INT_RLIN31T              ,
R_DEV_INT_SG0TI                ,
R_DEV_INT_SG1TI                ,
R_DEV_INT_SG2TI                ,
R_DEV_INT_SG3TI                ,
R_DEV_INT_SG4TI                ,
R_DEV_INT_DCUTDI               ,
R_DEV_INT_DMAERR               ,
R_DEV_INT_DMA0                 ,
R_DEV_INT_DMA1                 ,
R_DEV_INT_DMA2                 ,
R_DEV_INT_DMA3                 ,
R_DEV_INT_DMA4                 ,
R_DEV_INT_DMA5                 ,
R_DEV_INT_DMA6                 ,
R_DEV_INT_DMA7                 ,
R_DEV_INT_DMA8                 ,
R_DEV_INT_DMA9                 ,
R_DEV_INT_DMA10                ,
R_DEV_INT_DMA11                ,
R_DEV_INT_DMA12                ,
R_DEV_INT_DMA13                ,
R_DEV_INT_DMA14                ,
R_DEV_INT_DMA15                ,
R_DEV_INT_IIC0EE               ,
R_DEV_INT_IIC0RI               ,
R_DEV_INT_IIC0TI               ,
R_DEV_INT_IIC0TEI              ,
R_DEV_INT_IIC1EE               ,
R_DEV_INT_IIC1RI               ,
R_DEV_INT_IIC1TI               ,
R_DEV_INT_IIC1TEI              ,
R_DEV_INT_RCAN1ERR             ,
R_DEV_INT_RCAN1REC             ,
R_DEV_INT_RCAN1TRX             ,
R_DEV_INT_TAUJ0I0              ,
R_DEV_INT_TAUJ0I1              ,
R_DEV_INT_TAUJ0I2              ,
R_DEV_INT_TAUJ0I3              ,
R_DEV_INT_OSTM0                ,
R_DEV_INT_OSTM1                ,
R_DEV_INT_CSIG1RE              ,
R_DEV_INT_CSIG1R               ,
R_DEV_INT_CSIG1IC              ,
R_DEV_INT_CSIG2RE              ,
R_DEV_INT_CSIG2R               ,
R_DEV_INT_CSIG2IC              ,
R_DEV_INT_CSIG3RE              ,
R_DEV_INT_CSIG3R               ,
R_DEV_INT_CSIG3IC              ,
R_DEV_INT_CSIH1RE              ,
R_DEV_INT_CSIH1R               ,
R_DEV_INT_CSIH1C               ,
R_DEV_INT_CSIH1JC              ,
R_DEV_INT_PWSB0QFULL           ,
R_DEV_INT_PWGA0                ,
R_DEV_INT_PWGA1                ,
R_DEV_INT_PWGA2                ,
R_DEV_INT_PWGA3                ,
R_DEV_INT_PWGA4                ,
R_DEV_INT_PWGA5                ,
R_DEV_INT_PWGA6                ,
R_DEV_INT_PWGA7                ,
R_DEV_INT_PWGA8                ,
R_DEV_INT_PWGA9                ,
R_DEV_INT_PWGA10               ,
R_DEV_INT_PWGA11               ,
R_DEV_INT_PWGA12               ,
R_DEV_INT_PWGA13               ,
R_DEV_INT_PWGA14               ,
R_DEV_INT_PWGA15               ,
R_DEV_INT_PWGA16               ,
R_DEV_INT_PWGA17               ,
R_DEV_INT_PWGA18               ,
R_DEV_INT_PWGA19               ,
R_DEV_INT_PWGA20               ,
R_DEV_INT_PWGA21               ,
R_DEV_INT_PWGA22               ,
R_DEV_INT_PWGA23               ,
R_DEV_INT_ICUSTWRDY            ,
R_DEV_INT_ICUSTRRDY            ,
R_DEV_INT_167                  ,
R_DEV_INT_SCI3BRXI             ,
R_DEV_INT_SCI3BTXI             ,
R_DEV_INT_SCI3BTEI             ,
R_DEV_INT_FLERR                ,
R_DEV_INT_FLENDNM              ,
R_DEV_INT_173                  ,
R_DEV_INT_LCDBIFRDY            ,
R_DEV_INT_LCDBIFEMPTY          ,
R_DEV_INT_LCDBIFHALF           ,
R_DEV_INT_LCDBIFFULL           ,
R_DEV_INT_LCDBIFQTR            ,
R_DEV_INT_LCDBIF3QTR           ,
R_DEV_INT_RCAN2ERR             ,
R_DEV_INT_RCAN2REC             ,
R_DEV_INT_RCAN2TRX             ,
R_DEV_INT_PCMP0TFFIL           ,
R_DEV_INT_PCMP0TFERR           ,
R_DEV_INT_ISM0REACHED          ,
R_DEV_INT_ISM0DONE             ,
R_DEV_INT_ISM0ZPDAD            ,
R_DEV_INT_188                  ,
R_DEV_INT_SSIF0                ,
R_DEV_INT_SSIF0RX              ,
R_DEV_INT_SSIF0TX              ,
R_DEV_INT_SSIF1                ,
R_DEV_INT_SSIF1RX              ,
R_DEV_INT_SSIF1TX              ,
R_DEV_INT_SDRAMC               ,
R_DEV_INT_DAVEHD0PAUSE         ,
R_DEV_INT_DAVEHD0SYNC          ,
R_DEV_INT_DAVEHD0SP            ,
R_DEV_INT_VDCE0ERR             ,
R_DEV_INT_VDCE0GR3VBLANK       ,
R_DEV_INT_VDCE0S0VIVSYNC       ,
R_DEV_INT_VDCE0S0LOVSYNC       ,
R_DEV_INT_VDCE0GR3VLINE        ,
R_DEV_INT_VDCE0S0VFIELD        ,
R_DEV_INT_VDCE0S0WLINE         ,
R_DEV_INT_VDCE0S1LOVSYNC       ,
R_DEV_INT_VDCE0S1WLINE         ,
R_DEV_INT_VDCE0OIRVIVSYNC      ,
R_DEV_INT_VDCE0OIRLOVSYNC      ,
R_DEV_INT_VDCE0IRVLINE         ,
R_DEV_INT_VDCE0IRWLINE         ,
R_DEV_INT_212                  ,
R_DEV_INT_213                  ,
R_DEV_INT_214                  ,
R_DEV_INT_RLIN32S              ,
R_DEV_INT_RLIN32R              ,
R_DEV_INT_RLIN32T              ,
R_DEV_INT_RLIN33S              ,
R_DEV_INT_RLIN33R              ,
R_DEV_INT_RLIN33T              ,
R_DEV_INT_VDCE1ERR             ,
R_DEV_INT_VDCE1GR3VBLANK       ,
R_DEV_INT_VDCE1S0VIVSYNC       ,
R_DEV_INT_VDCE1S0LOVSYNC       ,
R_DEV_INT_VDCE1GR3VLINE        ,
R_DEV_INT_VDCE1S0VFIELD        ,
R_DEV_INT_VDCE1S0WLINE         ,
R_DEV_INT_VDCE1S1LOVSYNC       ,
R_DEV_INT_VDCE1S1WLINE         ,
R_DEV_INT_230                  ,
R_DEV_INT_231                  ,
R_DEV_INT_232                  ,
R_DEV_INT_233                  ,
R_DEV_INT_MLB0STA0             ,
R_DEV_INT_MLB0STA1             ,
R_DEV_INT_MLB0SYS              ,
R_DEV_INT_MLB0HBI              ,
R_DEV_INT_MIPICOVF             ,
R_DEV_INT_MIPICCTL             ,
R_DEV_INT_IMRD0  			   , 
R_DEV_INT_VOWE = R_DEV_INT_IMRD0 ,
R_DEV_INT_241                  ,
R_DEV_INT_JCU0EDI           ,
R_DEV_INT_JCU0DTI           ,
R_DEV_INT_ETH0LINE0         ,
R_DEV_INT_ETH0LINE1         ,
R_DEV_INT_ETH0LINE2         ,
R_DEV_INT_ETH0LINE3         ,
R_DEV_INT_PFSSMISRERR       ,
R_DEV_INT_VPISG1OVFERR      ,
R_DEV_INT_VPISG2OVFERR      ,
R_DEV_INT_VPISG3OVFERR      ,
R_DEV_INT_252                  ,
R_DEV_INT_VPISG5OVFERR      ,
R_DEV_INT_254                  ,
R_DEV_INT_255                  ,

/* delimiter, do not remove !!! */
R_DEV_INT_LAST

} r_dev_IntSel_t;

#define R_DEV_INT_DIRECT_BRANCH 0 
#define R_DEV_INT_TABLE         1u 

#ifdef __cplusplus
}
#endif

#endif /* DEV_INT_TYPES_H_  */
