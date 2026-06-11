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

#define H165_SER           PORT_ISO.PPR21.BIT.PPR21_3       //MISO-P21_3     input mode
#define H165_CLK           PORT_ISO.P21.BIT.P21_0              //MCLK-P21_0     output mode
#define H165_CS             PORT_ISO.P21.BIT.P21_1             //PL-P21_1          output mode

void SW_Input(void)
{
    unsigned char dump;
    unsigned char i, j;

    PORT_ISO.PMC21.UINT16      &=    0xFFF4;                    //P21-0, 1, 3  port mode 
    PORT_ISO.PM21.UINT16        &=    0xFFFC;                    //P21-0, 1     output mode

    PORT_ISO.PM21.UINT16        |=    0x0008;                    //P21-3         input mode
    PORT_ISO.PIBC21.UINT16     |=    0x0008;                    //P21-3         port input buffer enable

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

