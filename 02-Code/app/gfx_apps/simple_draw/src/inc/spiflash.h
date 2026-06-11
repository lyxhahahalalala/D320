#ifndef _SPIFLASH_H_
#define _SPIFLASH_H_

#include "r_typedefs.h"

#define CMD_WREN 0x06
#define CMD_WRDI 0x04
#define CMD_RDID 0x9F
#define CMD_RDSR 0x05
#define CMD_CP 0xAD
#define CMD_PP 0x02
#define CMD_CE 0x60
#define CMD_BE 0xd8
#define CMD_BE_32k 0x52

#define CMD_WRSR 0x01
#define CMD_READ 0x03//The sequence of issuing READ instruction is: CS# goes low¡ú sending READ instruction code¡ú3-byte address
					  //on SI ¡údata out on SO¡ú to end READ operation can use CS# to high at any time during data out.
#define CMD_FAST_READ 0x0B //The sequence of issuing FAST_READ instruction is: CS# goes low¡ú sending FAST_READ instruction code¡ú
//3-byte address on SI¡ú1-dummy byte (default) address on SI¡ú data out on SO¡ú to end FAST_READ operation
//can use CS# to high at any time during data out.
#define CMD_DREAD 0x3B //The sequence of issuing DREAD instruction is: CS# goes low ¡ú sending DREAD instruction ¡ú 3-byte address
//on SI ¡ú 8-bit dummy cycle ¡ú data out interleave on SO1 & SO0 ¡ú to end DREAD operation can use CS# to
//high at any time during data out.
void SPI_Init(void);
void Chip_Erase(void);
void SPI_Write_MutiData_CP(unsigned int Address, unsigned int length, uint8_t  *Buf);
void SPI_Write_MutiData_PP(unsigned int Address, unsigned int length, uint8_t  *Buf);
uint8_t  SPI_ReceiveOneByte(uint8_t CMD);
void SPI_ReceiveStatusMultiByte(uint8_t CMD,uint8_t recbuf[],uint32_t length);
void SPI_ReceiveDataMultiByte( unsigned int Address, uint8_t recbuf[],uint32_t length);

#endif
