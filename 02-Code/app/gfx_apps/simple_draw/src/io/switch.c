#include "bsp.h"
#include "iodefine.h"


#define H165_SER   PORT_ISO.PPR11.BIT.PPR11_0  //MISO-P11_0  input mode
#define H165_CLK   PORT_ISO.P11.BIT.P11_3      //MCLK-P11_3  output mode
#define H165_CS    PORT_ISO.P11.BIT.P11_2      //PL-P11_2    output mode

extern uint8_t  Test_Mode;
DIDO_DATA  sw_input[4];

void SW_Input(void)
{
    unsigned char dump;
    unsigned char i, j;

    H165_CS = 1;
	for(j = 0; j < 4; j++)
	{ 
		dump = 0;
		for(i = 0; i < 8; i++)
		{
			H165_CLK = 0; for(int tt = 0; tt < 2000; tt++);
			dump = (dump << 1);
			dump |= H165_SER;
			H165_CLK = 1; for(int tt = 0; tt < 2000; tt++);                                                   
		}
		sw_input[3-j].byte = dump; 
		
	}
	H165_CS = 0;
	
	if(Test_Mode == 2)
	{
		sw_input[1].bit3 = 1; //紧急切断  emc test
		sw_input[1].bit0 = 1; //IGN
	}
    return;
}

