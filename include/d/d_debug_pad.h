#ifndef D_DEBUG_PAD_H
#define D_DEBUG_PAD_H

#include "JSystem/JUtility/TColor.h"

class dDebugPad_c {
public:
    enum Mode_e {
        MODE_CAMERA_e,
        MODE_LIGHT_e,
        MODE_WINDTEST_e,
        MODE_SDCARD_e,

        MODE_MAX_e,
    };

    dDebugPad_c();

    bool Active() { return mIsActive; }
    bool Update();
    bool Report(int, int, JUtility::TColor, const char*, ...);
    bool Enable(s32);
    bool Trigger();

    static const char mode_name[MODE_MAX_e][9];

    /* 0x0 */ bool mIsActive;
    /* 0x1 */ u8 mTrigger;
    /* 0x4 */ s32 mMode;
};

extern dDebugPad_c dDebugPad;

#endif
