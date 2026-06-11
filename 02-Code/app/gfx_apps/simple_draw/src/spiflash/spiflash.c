/* ***********************************************************************************************************
 * Filename:	spiflash.c
 * File	ext:	c
 * Author:		Liubing
 * Data:		2019/9/3
 * purpose:	spiflash Driver
 * **********************************************************************************************************/

#include "iodefine.h"
#include "spiflash.h"

/* Port Data Register */
#define  SPI_CLK  	PORT_ISO.P21.BIT.P21_0             
#define  SPI_CS   	PORT_ISO.P21.BIT.P21_1 
#define  SPI_SI   	PORT_ISO.P21.BIT.P21_2 
#define  SPI_SO   	PORT_ISO.PPR21.BIT.PPR21_3
#define  WP       	PORT_ISO.PPR21.BIT.PPR21_4

#define _CPOL     0
#define _CPHA     0

#define SCK_IO    SPI_CLK
#define MOSI_IO   SPI_SI
#define MISO_IO   SPI_SO
#define SSEL_IO   SPI_CS
#define SCK_D(X)   (X?(SPI_CLK = 1):(SPI_CLK = 0))
#define MOSI_D(X)  (X?(SPI_SI = 1):(SPI_SI = 0))
#define SSEL_D(X)  (X?(SPI_CS = 1):(SPI_CS = 0))

#define MISO_I()  SPI_SO 



/************************************************
  端口方向配置  与输出初始化
  ************************************************/
void SPI_Init(void)
{
    //SPI_SCK SPI_CS SI SO
    PORT_ISO.PM21.UINT16		&= ~0x0007;
    PORT_ISO.PMC21.UINT16		&= ~0x0007;		//P21_0  - SPI_SCK   P21_1 - SPI_CS  P21_2 - SPI_SI

    PORT_ISO.PMC21.UINT16		&= ~0x0018; 
    PORT_ISO.PM21.UINT16		|= 0x0018;
    PORT_ISO.PIBC21.UINT16		|= 0x0018;		//P21_3 SPI_SO
	WP = 0;
    SSEL_D(1);
    MOSI_D(1);
#if _CPOL==0
    SCK_D(0);
#else
    SCK_D(1);
#endif
}

/**********************************************
  模式零           写数据
  ***********************************************/

void SPI_Send_Dat(unsigned char dat)
{
    unsigned char n;
    for(n=0;n<8;n++)
    {
	SCK_D(0);
	if(dat&0x80)
	    MOSI_D(1);
	else 
	    MOSI_D(0);
	dat<<=1;
	SCK_D(1);
    }

}
/*********************************************
  模式零         读数据
  *********************************************/
unsigned char SPI_Receiver_Dat(void)
{
    unsigned char n = 0;
	unsigned char dat = 0;
	
    for(n=0;n<8;n++)
    {
	SCK_D(0);
	dat<<=1;
	if(MISO_I())
	    dat|=0x01;
	else 
	    dat&=0xfe;
	SCK_D(1);
    }
    return dat;
}

void SPI_WriteEnable(void)
{
	uint8_t tmp;
	SSEL_D(0);
	SPI_Send_Dat(CMD_WREN);
	SSEL_D(1);
	tmp = SPI_ReceiveOneByte(CMD_RDSR);
	while((tmp&0x02) != 2){
		SSEL_D(0);
		SPI_Send_Dat(CMD_WREN);
		SSEL_D(1);
		tmp = SPI_ReceiveOneByte(CMD_RDSR);
	}

}
uint8_t SPI_READ_RDSR(void)
{
	uint8_t tmp;
	SSEL_D(0);
	SPI_Send_Dat(CMD_RDSR);
	SSEL_D(1);
	tmp = SPI_ReceiveOneByte(CMD_RDSR);
	return tmp ;

}
//page program
void SPI_Write_MutiData_PP(unsigned int Address, unsigned int length, uint8_t *Buf)
{
    // unsigned char n,readbackdata;
    unsigned int page = 0;
    unsigned char address[3];
    // uint8_t wren;
	uint8_t tmpdata;

    for(page = 0;page<((length/256)+1);page++){
	address[2] = 0;
	address[1] = page;
	address[0] = (page>>8);
	
	SPI_WriteEnable();
	SSEL_D(0);
	
	SPI_Send_Dat(CMD_PP);//write page
	for(unsigned char  i = 0;i<3;i++)//write address(24bits)
	    SPI_Send_Dat(address[i]);

		for(unsigned int i = 0;i<100;i++)//write address(24bits)
		{
			tmpdata = *Buf;
			SPI_Send_Dat(tmpdata);
			Buf++;
			
		}
		SSEL_D(1);
	
    }
}

//address shoud be 256bytes allign
void SPI_Write_MutiData_CP(unsigned int Address, unsigned int length, uint8_t *Buf)
{
	unsigned char readbackdata;
    unsigned char address[3] = {0};

	SSEL_D(0);
	SPI_Send_Dat(CMD_WREN); //write enable
	readbackdata = SPI_ReceiveOneByte(CMD_RDSR);
	while((readbackdata&0x02)==0)
	    readbackdata = SPI_ReceiveOneByte(CMD_RDSR);
	SPI_Send_Dat(CMD_CP); //write continusely
	//if(SPI_ReceiveOneByte(CMD_RDSR) == )
	
	for(unsigned char  i = 0;i<3;i++) //write address(24bits)
	    SPI_Send_Dat(address[i]);
	
	for(unsigned int i = 0;i<length/2;i++) //write address(24bits)
	{
		SPI_Send_Dat(*Buf);
		Buf++;
		SPI_Send_Dat(*Buf);
		Buf++;
		SSEL_D(1);
		SSEL_D(0);
		SSEL_D(1);
		SSEL_D(0);    
		
	}
	SPI_Send_Dat(CMD_WRDI);//write disable
	SSEL_D(1);

}

uint8_t SPI_ReceiveOneByte(uint8_t CMD)
{
    unsigned char n;
    uint8_t dat = 0;
    SSEL_D(0);
    for(n=0;n<8;n++)
    {
	SCK_D(0);
	if(CMD&0x80)
	    MOSI_D(1);
	else 
	    MOSI_D(0);
	CMD<<=1;
	SCK_D(1);
    }
    SCK_D(0);
    dat = SPI_Receiver_Dat();
    SSEL_D(1);
    return(dat);


}
void SPI_ReceiveStatusMultiByte(uint8_t CMD, uint8_t recbuf[], uint32_t length)
{
    SSEL_D(0);
    SPI_Send_Dat(CMD);
    for(uint32_t m = 0; m < length; m++)
	recbuf[m] = SPI_Receiver_Dat();
    SSEL_D(1);

}
void SPI_ReceiveDataMultiByte(unsigned int Address, uint8_t recbuf[], uint32_t length)
{
    unsigned char address[3];

    address[2] = (unsigned char) Address;
    address[1] = (unsigned char) (Address>>8);
    address[0] = (unsigned char) (Address>>16);
    SSEL_D(0);
    SPI_Send_Dat(CMD_READ);//read
    for(unsigned char  i = 0;i<3;i++)//write address(24bits)
		SPI_Send_Dat(address[i]);

	//SPI_Send_Dat(0);

    for(uint32_t m = 0; m < length; m++)
	recbuf[m] = SPI_Receiver_Dat();
    SSEL_D(1);

}
void Chip_Erase(void)
{
    uint8_t readbackdata = 0;
    SSEL_D(0);

    SPI_Send_Dat(CMD_WREN);//write enable
    SPI_Send_Dat(CMD_CE);//Chip rease
    SSEL_D(1);
    readbackdata = SPI_ReceiveOneByte(CMD_RDSR);

    while((readbackdata&0x01)==1)
	readbackdata = SPI_ReceiveOneByte(CMD_RDSR);





}





