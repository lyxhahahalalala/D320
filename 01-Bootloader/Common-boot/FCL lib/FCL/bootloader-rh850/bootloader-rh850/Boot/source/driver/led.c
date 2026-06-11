#include "BSP.h"
#include "iodefine.h"


#define H595_SI           PORT_ISO.P21.BIT.P21_2             //MOSI-P21_2
#define H595_CLK        PORT_ISO.P21.BIT.P21_0             //MCLK-P21_0
#define H595_CS          PORT_ISO.P21.BIT.P21_5            //LED_CS-P21_5

LED_DATA alarm_led[4];

void LED_Out(void)
{
   unsigned char i, j;
   unsigned char dump;

    PORT_ISO.PM21.UINT16              &= 0xFFDA;
    PORT_ISO.PMC21.UINT16            &= 0xFFDA;   //21-0, 2, 5 output mode
   
   H595_CS = 0;

   PORT_ISO.P42.BIT.P42_15 = alarm_led[0].bit0;      //buzz
  // PORT_ISO.P42.UINT16 |= (alarm_led[0].bit0 << 15);      //buzz
   PORT_ISO.P11.UINT16 |= (alarm_led[0].bit6 << 0);        //pout1 not used
   PORT_ISO.P11.UINT16 |= (alarm_led[0].bit7 << 1);        //pout2 not used

   
    
   for(j = 1; j < 4; j++)
   {
        dump = alarm_led[j].byte;
        for(i = 0; i < 8; i++)
        {
            H595_CLK = 0;
            if((dump & 0x80) == 0x80)
                H595_SI = 0;
            else
                H595_SI = 1;
            H595_CLK = 1;
            dump = (dump << 1);
        }
    }
   
    H595_CS = 1;
    return;
}
