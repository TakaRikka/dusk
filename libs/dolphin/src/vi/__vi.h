#ifndef _DOLPHIN_VI_INTERNAL_H_
#define _DOLPHIN_VI_INTERNAL_H_

#include <dolphin/vi.h>

#ifdef __cplusplus
extern "C" {
#endif

/* gpioexi.c */

void __VIInitI2C(void);
void __VISetSCL(int value);
int __VIGetSCL(void);
void __VISetSDA(int value);
int __VIGetSDA(void);

/* i2c.c */

int __VISendI2CData(u8 slaveAddr, u8* pData, int nBytes);

/* initphilips.c */

void __VIInitPhilips(void);

/* vi.c */

void __VIInit(VITVMode mode);
void __VISetAdjustingValues(s16 x, s16 y);
void __VIGetAdjustingValues(s16* x, s16* y);
void __VIGetCurrentPosition(s16* x, s16* y);

#ifdef __cplusplus
}
#endif

#endif
