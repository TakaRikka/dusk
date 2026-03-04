#include <dolphin/dolphin.h>
#include <dolphin/pad.h>
#include <dolphin/demo.h>

#include "__demo.h"

static u32 PadChanMask[4] = {
    PAD_CHAN0_BIT,
    PAD_CHAN1_BIT,
    PAD_CHAN2_BIT,
    PAD_CHAN3_BIT,
};

static PADStatus Pad[4];
DEMODMPad DemoPad[4];

u32 DemoNumValidPads;

// prototypes
static void DEMOPadCopy(PADStatus* pad, DEMODMPad* dmpad);

static void DEMOPadCopy(PADStatus* pad, DEMODMPad* dmpad) {
    u16 dirs;

    if (pad->err != -3) {
        dirs = 0;
        if (pad->stickX < -0x30) {
            dirs |= 0x4000;
        }
        if (pad->stickX > 0x30) {
            dirs |= 0x8000;
        }
        if (pad->stickY < -0x30) {
            dirs |= 0x2000;
        }
        if (pad->stickY > 0x30) {
            dirs |= 0x1000;
        }
        if (pad->substickX < -0x30) {
            dirs |= 0x400;
        }
        if (pad->substickX > 0x30) {
            dirs |= 0x800;
        }
        if (pad->substickY < -0x30) {
            dirs |= 0x200;
        }
        if (pad->substickY > 0x30) {
            dirs |= 0x100;
        }

        dmpad->dirsNew = (dirs & (dmpad->dirs ^ dirs));
        dmpad->dirsReleased = (dmpad->dirs & (dmpad->dirs ^ dirs));
        dmpad->dirs = dirs;
        dmpad->buttonDown = (pad->button & (dmpad->pst.button ^ pad->button));
        dmpad->buttonUp = (dmpad->pst.button & (dmpad->pst.button ^ pad->button));
        dmpad->stickDeltaX = (pad->stickX - dmpad->pst.stickX);
        dmpad->stickDeltaY = (pad->stickY - dmpad->pst.stickY);
        dmpad->substickDeltaX = (pad->substickX - dmpad->pst.substickX);
        dmpad->substickDeltaY = (pad->substickY - dmpad->pst.substickY);
        dmpad->pst = *pad;
    } else {
        dmpad->dirsNew = dmpad->dirsReleased = 0;
        dmpad->buttonDown = dmpad->buttonUp = 0;
        dmpad->stickDeltaX = dmpad->stickDeltaY = 0;
        dmpad->substickDeltaX = dmpad->substickDeltaY = 0;
    }
}

void DEMOPadRead(void) {
    s32 i;
    u32 ResetReq = 0;

    PADRead(&Pad[0]);
    PADClamp(&Pad[0]);

    DemoNumValidPads = 0;

    for (i = 0; i < 4; i++) {
        if (Pad[i].err == 0 || Pad[i].err == -3) {
            DemoNumValidPads++;
        } else if (Pad[i].err == -1) {
            ResetReq |= PadChanMask[i];
        }

        DEMOPadCopy(&Pad[i], &DemoPad[i]);
    }

    if (ResetReq != 0) {
        PADReset(ResetReq);
    }
}

void DEMOPadInit(void) {
    s32 i;

    PADInit();

    for (i = 0; i < 4; i++) {
        DemoPad[i].pst.button = 0;
        DemoPad[i].pst.stickX = 0;
        DemoPad[i].pst.stickY = 0;
        DemoPad[i].pst.substickX = 0;
        DemoPad[i].pst.substickY = 0;
        DemoPad[i].pst.triggerLeft = 0;
        DemoPad[i].pst.triggerRight = 0;
        DemoPad[i].pst.analogA = 0;
        DemoPad[i].pst.analogB = 0;
        DemoPad[i].pst.err = 0;
        DemoPad[i].buttonDown = 0;
        DemoPad[i].buttonUp = 0;
        DemoPad[i].dirs = 0;
        DemoPad[i].dirsNew = 0;
        DemoPad[i].dirsReleased = 0;
        DemoPad[i].stickDeltaX = 0;
        DemoPad[i].stickDeltaY = 0;
        DemoPad[i].substickDeltaX = 0;
        DemoPad[i].substickDeltaY = 0;
    }
}
