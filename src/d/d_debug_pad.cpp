#include "d/d_debug_pad.h"
#include "m_Do/m_Do_controller_pad.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTReport.h"
#include "f_ap/f_ap_game.h"

dDebugPad_c::dDebugPad_c() {
    mIsActive = false;
    mMode = MODE_CAMERA_e;
    mTrigger = false;
}

const char dDebugPad_c::mode_name[MODE_MAX_e][9] = {
    "CAMERA",
    "BsLight",
    "WindTest",
    "SDCARD",
};

bool dDebugPad_c::Update() {
    mTrigger = false;

    if (mDoCPd_c::getTrigStart(PAD_3)) {
        mIsActive = mIsActive ? false : true;
        mTrigger = true;
    }

    if (mIsActive) {
        if (mDoCPd_c::getTrigZ(PAD_3)) {
            mMode = (mMode + 1) % MODE_MAX_e;
        }

        int x = 10;
        int y = 40;
        int line_height = 15;
        JUtility::TColor sp40(0xC8, 0x80, 0xFF, 0xC0);
        JUtility::TColor sp3C(0xFF, 0xC0, 0xC0, 0xFF);

        Report(x, y, sp40, "+-DEBUG-PAD-+");

        int i;
        for (i = 0; i < MODE_MAX_e; i++) {
            y += line_height;

            if (i == mMode) {
                Report(x, y, sp40, "|           |");
                Report(x, y, sp3C, "  @%-8s  ", mode_name[i]);
            } else {
                Report(x, y, sp40, "|  %-8s |", mode_name[i]);
            }
        }

        for (i = 0; i < 1; i++) {
            y += line_height;
            Report(x, y, sp40, "|           |");
        }

        y += line_height;
        Report(x, y, sp40, "+-----------+");
    }

    return mIsActive;
}

bool dDebugPad_c::Report(int x, int y, JUtility::TColor color, const char* message, ...) {
    char buffer[256];

    static JUtility::TColor ShadowDarkColor(0, 0, 0, 0x80);

    if (mIsActive) {
        va_list list;
        va_start(list, message);
        vsnprintf(buffer, sizeof(buffer), message, list);
        va_end(list);
    
        JUTDbPrint::getManager()->flush();
        JUTDbPrint::getManager()->setCharColor(ShadowDarkColor);
        JUTReport(x + 2, y + 2, buffer);
        JUTDbPrint::getManager()->flush();
        JUTDbPrint::getManager()->setCharColor(color);
        JUTReport(x, y, buffer);
        JUTDbPrint::getManager()->flush();
        JUTDbPrint::getManager()->setCharColor(g_HIO.mColor);
        return true;
    }

    return false;
}

dDebugPad_c dDebugPad;
