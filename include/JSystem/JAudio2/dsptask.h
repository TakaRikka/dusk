#ifndef DSPTASK_H
#define DSPTASK_H

#include <dolphin/types.h>

void DspBoot(void (*)(void*));
void DspFinishWork(u16 param_0);
int DSPSendCommands2(u32* msgs, u32 param_1, void (*param_2)(u16));

#endif /* DSPTASK_H */
