#ifndef _INIT_H_
#define _INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

int32_t init(void);
int32_t de_init(void);
void start_draw(void);
void end_draw(void);
unsigned int color_convert(unsigned char color_src);

#ifdef __cplusplus
}
#endif

#endif
