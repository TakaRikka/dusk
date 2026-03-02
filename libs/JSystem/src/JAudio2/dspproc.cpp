#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/dspproc.h"
#include "JSystem/JAudio2/dsptask.h"
#include "JSystem/JAudio2/JASDSPInterface.h"

void DSPReleaseHalt2(u32 msg) {
    u32 msgs[2];
    u16 dspMap = DSP_CreateMap2(msg);
    msgs[0] = (msg << 16) | dspMap;

    DSPSendCommands2(msgs, 0, NULL);
}

static volatile BOOL flag;

static void setup_callback(u16 param_0) {
    OS_REPORT("Finish %d\n", param_0);
    flag = FALSE;
}

void DsetupTable(u32 param_0, u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    u32 table[5];
    table[0] = (param_0 & 0xFFFF) | 0x81000000;
    table[1] = param_1;
    table[2] = param_2;
    table[3] = param_3;
    table[4] = param_4;
    OS_REPORT("Table Setup\n");

    flag = TRUE;
    DSPSendCommands2(table, 5, setup_callback);
    while (true) {
        if (!flag) {
            return;
        }
    }
}

static u16 DSP_MIXERLEVEL = 0x4000;

void DsetMixerLevel(f32 level) {
    DSP_MIXERLEVEL = 4096.0f * level;
}

void DsyncFrame2ch(u32 param_0, u32 param_1, u32 param_2) {
    u32 msgs[5];
    msgs[0] = (param_0 & 0xff) << 0x10 | 0x82000000 | DSP_MIXERLEVEL;
    msgs[1] = param_1;
    msgs[2] = param_2;
    msgs[3] = 0;
    msgs[4] = 0;
    DSPSendCommands2(msgs, 5, 0);
}

void DsyncFrame4ch(u32 param_0, u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    u32 msgs[5];
    msgs[0] = (param_0 & 0xff) << 0x10 | 0x82000000 | DSP_MIXERLEVEL;
    msgs[1] = param_1;
    msgs[2] = param_2;
    msgs[3] = param_3;
    msgs[4] = param_4;
    DSPSendCommands2(msgs, 5, 0);
}
