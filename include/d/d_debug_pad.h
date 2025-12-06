#ifndef D_DEBUG_PAD_H
#define D_DEBUG_PAD_H

#include "m_Do/m_Do_controller_pad.h"
#include "JSystem/JUtility/TColor.h"

class dDebugPad_c {
public:
    dDebugPad_c();

    bool Active();
    bool Update();
    bool Report(int, int, JUtility::TColor, const char*, ...);
    bool Enable(s32);
    bool Trigger();

    /* 0x0 */ u8 mIsActive;
    /* 0x1 */ u8 mTrigger;
    /* 0x4 */ s32 mMode;
};

extern dDebugPad_c dDebugPad;

#endif
