//#include "BSP.h"
#include "iodefine.h"
//#define ISM

#if defined ISM
#include "r_config_ism.h"
//#include "r_ism_api.h"     /* ISM stepper motor */

//#include "carmodel.h"     /* car model */
//#include "motor_sm.h"     /* motor control app functions */
#endif

float  sm_steps[6]        = {0, 0, 0, 0, 0, 0};
float  sm_new_value[6]    = {0, 0, 0, 0, 0, 0};
float  sm_old_value[6]    = {0, 0, 0, 0, 0, 0};
unsigned char sm_speed[6] = {0, 0, 0, 0, 0, 0};

int   steps  = 600;
static unsigned char stage = 0;
static unsigned char stage_1[6] = {0, 0, 0, 0, 0, 0};
static unsigned char stage_2[6] = {0, 0, 0, 0, 0, 0};
static unsigned char direct[6]  = {0, 0, 0, 0, 0, 0};


static unsigned short* PORT_PULSE[6] = 
{
	(unsigned short*)&(PORT_ISO.P16.UINT16),
	(unsigned short*)&(PORT_ISO.P17.UINT16)
};

unsigned short mask[4] = {0xfff0, 0xff0f, 0xf0ff, 0x0fff};

void SM_Init(void)
{
	PORT_ISO.P16.UINT16                = 0x0000;
	PORT_ISO.PM16.UINT16               = 0x0000;
	
	PORT_ISO.P17.UINT16                = 0x0000;
	PORT_ISO.PM17.UINT16               = 0x0000;
	
	PORT_ISO.PMC17.UINT16              = 0x0000;
	PORT_ISO.PM17.UINT16               = 0x0000;

	PORT_ISO.PDSC16.UINT32              = 0xffff;
	PORT_ISO.PDSC17.UINT32              = 0xffff;
}

void SM_Update_0(void)
{
	//if(sm_new_value[0] == sm_old_value[0]) return;
    if (sm_new_value[0] >= sm_old_value[0])
	{
        sm_steps[0] = sm_new_value[0] - sm_old_value[0];
        direct[0] = 1;
    }
	else
	{
        sm_steps[0] = sm_old_value[0] - sm_new_value[0];
        direct[0] = 0;
    }
    if (sm_steps[0] > 480) sm_speed[0] = 0; //最快速度
    else if (sm_steps[0] > 360) sm_speed[0] = 1; //
    else if (sm_steps[0] > 240) sm_speed[0] = 2; //     
    else if (sm_steps[0] > 180) sm_speed[0] = 3; //
    else if (sm_steps[0] > 120) sm_speed[0] = 4; //
    else if (sm_steps[0] > 80) sm_speed[0] = 5; //
    else if (sm_steps[0] > 60) sm_speed[0] = 6; //
    else if (sm_steps[0] > 45) sm_speed[0] = 7; //
    else if (sm_steps[0] > 30) sm_speed[0] = 8; //
    else if (sm_steps[0] > 20) sm_speed[0] = 9; //
    else if (sm_steps[0] > 12) sm_speed[0] = 10; //
    else if (sm_steps[0] > 6) sm_speed[0] = 20; //
    else sm_speed[0] = 25; //
}

void SM_Update_1(void)
{    

	if(sm_new_value[1] >= sm_old_value[1])
	{
        sm_steps[1] = sm_new_value[1] - sm_old_value[1];
        direct[1] = 1;
    }
	else
	{
        sm_steps[1] = sm_old_value[1] - sm_new_value[1];
        direct[1] = 0;
    }
    if (sm_steps[1] > 480) sm_speed[1] = 0; //最快速度
    else if (sm_steps[1] > 360) sm_speed[1] = 1; //
    else if (sm_steps[1] > 240) sm_speed[1] = 2; //     
    else if (sm_steps[1] > 180) sm_speed[1] = 3; //
    else if (sm_steps[1] > 120) sm_speed[1] = 4; //
    else if (sm_steps[1] > 80) sm_speed[1] = 5; //
    else if (sm_steps[1] > 60) sm_speed[1] = 6; //
    else if (sm_steps[1] > 45) sm_speed[1] = 7; //
    else if (sm_steps[1] > 30) sm_speed[1] = 8; //
    else if (sm_steps[1] > 20) sm_speed[1] = 10; //
    else if (sm_steps[1] > 12) sm_speed[1] = 11; //
    else if (sm_steps[1] > 6) sm_speed[1] = 12; //
    else sm_speed[1] = 12; //
}

void SM_Update_2(void)
{
	if(sm_new_value[2] >= sm_old_value[2])
	{
        sm_steps[2] = sm_new_value[2] - sm_old_value[2];
        direct[2] = 1;
    }
	else
	{
        sm_steps[2] = sm_old_value[2] - sm_new_value[2];
        direct[2] = 0;
    }
    if (sm_steps[2] > 480) sm_speed[2] = 5; //最快速度
    else if (sm_steps[2] > 360) sm_speed[2] = 1; //
    else if (sm_steps[2] > 240) sm_speed[2] = 2; //     
    else if (sm_steps[2] > 180) sm_speed[2] = 3; //
    else if (sm_steps[2] > 120) sm_speed[2] = 4; //
    else if (sm_steps[2] > 80) sm_speed[2] = 5; //
    else if (sm_steps[2] > 60) sm_speed[2] = 6; //
    else if (sm_steps[2] > 45) sm_speed[2] = 7; //
    else if (sm_steps[2] > 30) sm_speed[2] = 8; //
    else if (sm_steps[2] > 20) sm_speed[2] = 9; //
    else if (sm_steps[2] > 12) sm_speed[2] = 10; //
    else if (sm_steps[2] > 6) sm_speed[2] = 11; //
    else sm_speed[2] = 12; //
}

void SM_Update_3(void)
{
	if(sm_new_value[3] >= sm_old_value[3])
	{
        sm_steps[3] = sm_new_value[3] - sm_old_value[3];
        direct[3] = 1;
    }
	else
	{
        sm_steps[3] = sm_old_value[3] - sm_new_value[3];
        direct[3] = 0;
    }
    if (sm_steps[3] > 480) sm_speed[3] = 5; //最快速度
    else if (sm_steps[3] > 360) sm_speed[3] = 1; //
    else if (sm_steps[3] > 240) sm_speed[3] = 2; //     
    else if (sm_steps[3] > 180) sm_speed[3] = 3; //
    else if (sm_steps[3] > 120) sm_speed[3] = 4; //
    else if (sm_steps[3] > 80) sm_speed[3] = 5; //
    else if (sm_steps[3] > 60) sm_speed[3] = 6; //
    else if (sm_steps[3] > 45) sm_speed[3] = 7; //
    else if (sm_steps[3] > 30) sm_speed[3] = 8; //
    else if (sm_steps[3] > 20) sm_speed[3] = 9; //
    else if (sm_steps[3] > 12) sm_speed[3] = 10; //
    else if (sm_steps[3] > 6) sm_speed[3] = 11; //
    else sm_speed[3] = 12; //
}

void SM_Update_4(void)
{
	//if(sm_new_value[4] == sm_old_value[4]) return;
	if(sm_new_value[4] >= sm_old_value[4])
	{
        sm_steps[4] = sm_new_value[4] - sm_old_value[4];
        direct[4] = 1;
    }
	else
	{
        sm_steps[4] = sm_old_value[4] - sm_new_value[4];
        direct[4] = 0;
    }
    if (sm_steps[4] > 480) sm_speed[4] = 5; //最快速度
    else if (sm_steps[4] > 360) sm_speed[4] = 1; //
    else if (sm_steps[4] > 240) sm_speed[4] = 2; //     
    else if (sm_steps[4] > 180) sm_speed[4] = 3; //
    else if (sm_steps[4] > 120) sm_speed[4] = 4; //
    else if (sm_steps[4] > 80) sm_speed[4] = 5; //
    else if (sm_steps[4] > 60) sm_speed[4] = 6; //
    else if (sm_steps[4] > 45) sm_speed[4] = 7; //
    else if (sm_steps[4] > 30) sm_speed[4] = 8; //
    else if (sm_steps[4] > 20) sm_speed[4] = 9; //
    else if (sm_steps[4] > 12) sm_speed[4] = 10; //
    else if (sm_steps[4] > 6) sm_speed[4] = 11; //
    else sm_speed[4] = 12; //
}

void SM_Update_5(void)
{
	if(sm_new_value[5] >= sm_old_value[5])
	{
        sm_steps[5] = sm_new_value[5] - sm_old_value[5];
        direct[5] = 1;
    }
	else
	{
        sm_steps[5] = sm_old_value[5] - sm_new_value[5];
        direct[5] = 0;
    }
    if (sm_steps[5] > 480) sm_speed[5] = 0; //最快速度
    else if (sm_steps[5] > 360) sm_speed[5] = 1; //
    else if (sm_steps[5] > 240) sm_speed[5] = 2; //     
    else if (sm_steps[5] > 180) sm_speed[5] = 3; //
    else if (sm_steps[5] > 120) sm_speed[5] = 4; //
    else if (sm_steps[5] > 80) sm_speed[5] = 5; //
    else if (sm_steps[5] > 60) sm_speed[5] = 6; //
    else if (sm_steps[5] > 45) sm_speed[5] = 7; //
    else if (sm_steps[5] > 30) sm_speed[5] = 8; //
    else if (sm_steps[5] > 20) sm_speed[5] = 8; //
    else if (sm_steps[5] > 12) sm_speed[5] = 9; //
    else if (sm_steps[5] > 6) sm_speed[5] = 9; //
    else sm_speed[5] = 10; //
}

void SM_Reset(void)
{
    for (int i = 0; i < 6; i++) 
    {
        sm_steps[i] = 0;
        sm_new_value[i] = 0;
        sm_old_value[i] = 0;
    }
}

void SM_Zero(void)
{
#if 0
	unsigned int i = 0;
	for(i = 0; i < 6; i++)
	{
		direct[i] = 0;
        	sm_steps[i] = sm_old_value[i];
        	sm_new_value[i] = 0;
        	sm_speed[i] = 0;
	}
	#endif
}

void zerosm(void)
{
#if 0

	switch(stage)
	    {
			case 0:
			{
				*PORT_PULSE[0]  =  0x0999;
				*PORT_PULSE[1]  =  0x0999;
				stage = 1;
				break;
			}
			case 1:
			{
				*PORT_PULSE[0]  = 0x0111;
				*PORT_PULSE[1]  = 0x0111;
				stage = 2;
				break;
			}
			case 2:
			{
				*PORT_PULSE[0]  = 0x0777;
				*PORT_PULSE[1]  = 0x0777;
				stage = 3;
				break;
			}
			case 3:
			{
				*PORT_PULSE[0]  = 0x0666;
				*PORT_PULSE[1]  = 0x0666;
				stage = 4;
				break;
			}
			case 4:
			{
				*PORT_PULSE[0]  = 0x0eee;
				*PORT_PULSE[1]  = 0x0eee;
				stage = 5;
				break;
			}
			case 5:
			{
				*PORT_PULSE[0]  = 0x0888;
				*PORT_PULSE[1]  = 0x0888;
				stage = 0;
				break;
			}
			default:
				stage = 0;
	    }
	#endif
}


void sm_run(unsigned char channel)
{
	unsigned char port    = channel / 3;
	unsigned char byte    = channel % 3;
	unsigned char bit     = byte * 4;
	#if 0
    if(direct[channel])
    {
		sm_old_value[channel] ++;
	    switch(stage_1[channel])
	    {
			case 0:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x8 << bit);
				stage_1[channel] = 1;    stage_2[channel] = 0;
				break;
			}
			case 1:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0xE << bit);
				stage_1[channel] = 2;    stage_2[channel] = 5;

				break;
			}
			case 2:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x6 << bit);
				stage_1[channel] = 3;    stage_2[channel] = 4;

				break;
			}
			case 3:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x7 << bit);
				stage_1[channel] = 4;    stage_2[channel] = 3;

				break;
			}
			case 4:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) |(0x1 << bit);
				stage_1[channel] = 5;    stage_2[channel] = 2;

				break;
			}
			case 5:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x9 << bit);
				stage_1[channel] = 0;    stage_2[channel] = 1;

				break;
			}
			default:
				stage_1[channel] = 0;    stage_2[channel] = 1;    break;
	    }
    }
    else
    {
		if(sm_old_value[channel] > 0.0) sm_old_value[channel] --;
		switch(stage_2[channel])
	    {
			case 0:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x9 << bit);
				stage_2[channel] = 1;   stage_1[channel] = 0;
				break;
			}
			case 1:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x1 << bit);
				stage_2[channel] = 2;   stage_1[channel] = 5;
				break;
			}
			case 2:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x7 << bit);
				stage_2[channel] = 3;   stage_1[channel] = 4;
				break;
			}
			case 3:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x6 << bit);
				stage_2[channel] = 4;   stage_1[channel] = 3;
				break;
			}
			case 4:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0xE << bit);
				stage_2[channel] = 5;   stage_1[channel] = 2;
				break;
			}
			case 5:
			{
				*PORT_PULSE[port]  = (*PORT_PULSE[port] & mask[byte]) | (0x8 << bit);
				stage_2[channel] = 0;   stage_1[channel] = 1;
				break;
			}
			default:
				stage_2[channel] = 0;   stage_1[channel] = 1;    break;
	    }
    }
	#endif
	
}




