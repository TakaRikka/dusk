#ifndef _REVOLUTION_SI_INTERNAL_H_
#define _REVOLUTION_SI_INTERNAL_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void __SISteeringEnable(s32 chan);
s32 __SISteeringTransfer(s32 chan, u32 outputBytes, u32 inputBytes, void (*proc)(s32));
void __SISteeringSyncCallback(s32 chan, s32);
s32 __SISteeringSync(s32 chan);
void __SISteeringDisable(s32 chan);
void __SITestSamplingRate(u32 tvmode);

#ifdef __cplusplus
}
#endif

#endif
