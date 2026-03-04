#ifndef _REVOLUTION_VI_INTERNAL_H_
#define _REVOLUTION_VI_INTERNAL_H_

#include <revolution/vi.h>

#ifdef __cplusplus
extern "C" {
#endif

/* i2c.c */

int __VISendI2CData(u8 slaveAddr, u8* pData, int nBytes);

/* vi.c */

void __VIInit(VITVMode mode);
void __VISetAdjustingValues(s16 x, s16 y);
void __VIGetAdjustingValues(s16* x, s16* y);
void __VIGetCurrentPosition(s16* x, s16* y);
BOOL __VIResetSIIdle();
BOOL __VIResetRFIdle();
BOOL __VIResetDev0Idle();

/* vi3in1.c */
void __VISetFilter4EURGB60(VIBool enable);
void __VISetGamma1_0(void);
void __VISetYUVSEL(VIBool outsel);
void __VISetCGMS(void);
void __VISetWSS(void);
void __VISetClosedCaption(void);
void __VISetMacrovision(void);
void __VISetGamma(void);
void __VISetTrapFilter(void);
void __VISetRGBOverDrive(void);
void __VISetRGBModeImm(void);
void __VISetRevolutionModeSimple(void);

#ifdef __cplusplus
}
#endif

#endif
