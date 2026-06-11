#include "bsp.h"
#include "iodefine.h"

SW_DATA  sw_input[5];
extern void R_TICK_WaitMS(uint32_t Unit, uint32_t TickMS) ;
extern  uint32_t R_TICK_GetTimeUS(uint32_t Unit) ;

unsigned short* IO_PMC[8] = 
{
	(unsigned short*)&(PORT_ISO.PMC17.UINT16),
	
};

unsigned short* IO_PM[8] = 
{
	(unsigned short*)&(PORT_ISO.PM17.UINT16),
	
};

unsigned short* IO_PICB[8] = 
{
	(unsigned short*)&(PORT_ISO.PIBC17.UINT16),
	
};

unsigned short* IO_PPR[8] = 
{
	(unsigned short*)&(PORT_ISO.PPR17.UINT16),
	
};

#define H165_SER           PORT_ISO.PPR11.BIT.PPR11_0       //MISO-P11_0     input mode
#define H165_CLK           PORT_ISO.P11.BIT.P11_3              //MCLK-P11_3    output mode
#define H165_CS             PORT_ISO.P11.BIT.P11_2             //PL-P11_2         output mode

void SW_Input(void)
{
    unsigned char dump;
    unsigned char i, j;



    //H165_CS = 0;
    //R_TICK_WaitMS(0, 1);
    H165_CS = 1;
	
	for(j = 0; j < 5; j++)
	{ 
		dump = 0;
		for(i = 0; i < 8; i++)
		{
			H165_CLK = 0;   for(int tt = 0; tt < 2000; tt++);  //R_TICK_WaitMS(0, 1);
			
			dump = (dump << 1);
			dump |= H165_SER;

			H165_CLK = 1; for(int tt = 0; tt < 2000; tt++);  //R_TICK_WaitMS(0, 1); //for(int tt = 0; tt < 200; tt++);
		}
		sw_input[4-j].byte = ~dump; 
	}
	H165_CS = 0;
    return;
}

