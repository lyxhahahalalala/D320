#ifndef _SG_H_
#define _SG_H_


void SoundGenInit(void);
void SoundStop(void);
void SoundGenNewTone(unsigned int tonefrequence);

void SG_NewTone(uint32_t tonefreqHZ, uint8_t duty);


#endif


