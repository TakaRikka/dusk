#ifndef DSPPROC_H
#define DSPPROC_H

#include <dolphin/types.h>

void DSPReleaseHalt2(u32 msg);
void DsetupTable(u32 param_0, u32 param_1, u32 param_2, u32 param_3, u32 param_4);
void DsetMixerLevel(f32 level);
void DsyncFrame2ch(u32 param_0, u32 param_1, u32 param_2);
void DsyncFrame4ch(u32 param_0, u32 param_1, u32 param_2, u32 param_3, u32 param_4);

#endif /* DSPPROC_H */
