#include "JSystem/JAudio2/dspproc.h"
#include "JSystem/JAudio2/osdsp_task.h"
#include "JSystem/JAudio2/dsptask.h"
#include "global.h"
#include "os.h"

void DSPReleaseHalt2(u32) {
    CRASH("We do not directly emulate the DSP");
}
void DsetupTable(u32, u32, u32, u32, u32) {
    // Nada.
}
void DsetMixerLevel(f32) {
    // Nada for now, but maybe we should care about this?
}
void DsyncFrame2ch(u32, u32, u32) {
    CRASH("We do not directly emulate the DSP");
}
void DsyncFrame4ch(u32, u32, u32, u32, u32) {
    CRASH("We do not directly emulate the DSP");
}

void DspBoot(void (*)(void*)) {
    CRASH("We do not directly emulate the DSP");
}
void DspFinishWork(u16) {
    CRASH("We do not directly emulate the DSP");
}
int DSPSendCommands2(u32*, u32, void (*)(u16)) {
    CRASH("We do not directly emulate the DSP");
}

void DsyncFrame2(u32, u32, u32) {
    CRASH("We do not directly emulate the DSP");
}
