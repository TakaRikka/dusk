#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "stdio.h"
#include <dolphin/vi.h>

bool sAssertVisible = true;

namespace JUTAssertion {

void create() {}

namespace {
static u32 sMessageLife;

static char sMessageFileLine[64];

static char sMessageString[256];

static s32 sDisplayTime = -1;
static s32 sDevice = 3;
static bool mVisible = true;
static s32 sMessageOwner;
static u8 mSynchro;
};  // namespace

u32 flush_subroutine() {
    if (sMessageLife == 0) {
        return 0;
    }

    if (sMessageLife != -1) {
        sMessageLife--;
    }

    if (sMessageLife >= 5) {
        return sMessageLife;
    }

    return 0;
}

void flushMessage() {
    if (flush_subroutine() && sAssertVisible == true) {
        JUTDirectPrint* manager = JUTDirectPrint::getManager();
        JUtility::TColor color = manager->getCharColor();
        manager->setCharColor(JUtility::TColor(255, 200, 200, 255));
        manager->drawString(16, 16, sMessageFileLine);
        manager->drawString(16, 24, sMessageString);
        manager->setCharColor(color);
    }
}

void flushMessage_dbPrint() {
    if (flush_subroutine() && sAssertVisible == true && JUTDbPrint::getManager() != NULL) {
        JUTFont* font = JUTDbPrint::getManager()->getFont();
        if (font != NULL) {
            u8 tmp = ((VIGetRetraceCount() & 0x3C) << 2) | 0xF;
            font->setGX();
            font->setCharColor(JUtility::TColor(255, tmp, tmp, 255));
            font->drawString(30, 36, sMessageFileLine, true);
            font->drawString(30, 54, sMessageString, true);
        }
    }
}

u32 getSDevice() {
    return sDevice;
}

void setConfirmMessage(u32 param_1, char* file, int line, bool param_4, const char* msg) {
    if (param_4 == 1) {
        return;
    }
    u32 r28 = sMessageLife;
    if (r28 == 0 && (param_1 & 1) != 0) {
        sMessageLife = sDisplayTime;
        sMessageOwner = 2;
        snprintf(sMessageFileLine, sizeof(sMessageFileLine) - 1, "FAILED [%s:%d]", file, line);
        snprintf(sMessageString, sizeof(sMessageString) - 1, "%s", msg);
    }
    if ((r28 == 0 || mSynchro == 0) && (param_1 & 2) != 0) {
        JUTWarningConsole_f("FAILED [%s:%d] %s\n", file, line, msg);
    }
}

void showAssert_f_va(u32 device, const char* file, int line, const char* msg, va_list args) {
    sMessageLife = -1;
    vsnprintf(sMessageString, 255, msg, args);

    if (device & 2) {
        OSReport("Failed assertion: %s:%d\n", file, line);
        OSReport("%s\n", sMessageString);
    }

    if ((device & 1) && JUTDirectPrint::getManager()) {
        JUtility::TColor old_color(JUTDirectPrint::getManager()->getCharColor());
        JUTDirectPrint::getManager()->setCharColor(JUtility::TColor(255, 255, 255, 255));
        JUTDirectPrint::getManager()->erase(10, 16, 306, 24);

        snprintf(sMessageFileLine, 63, "Failed assertion: %s:%d", file, line);
        JUTDirectPrint::getManager()->drawString(16, 16, sMessageFileLine);
        JUTDirectPrint::getManager()->drawString(16, 24, sMessageString);

        JUTDirectPrint::getManager()->setCharColor(old_color);
        VISetNextFrameBuffer(JUTDirectPrint::getManager()->getFrameBuffer());
        VIFlush();
        OSEnableInterrupts();

        u32 retrace_count = VIGetRetraceCount();
        u32 new_count;
        do {
            new_count = VIGetRetraceCount();
        } while (retrace_count == new_count);

        // busy loop for 2 seconds
        OSTime var1 = OSGetTime();
        while (OSTicksToMilliseconds(OSGetTime() - var1) < 2000) {
        }
    }
}

void showAssert_f(u32 device, const char* file, int line, const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    showAssert_f_va(device, file, line, msg, args);
    va_end(args);
}

void showAssert(u32 device, const char* file, int line, const char* msg) {
    showAssert_f(device, file, line, "%s", msg);
}

void setWarningMessage_f_va(u32 device, const char* file, int line, const char* msg, va_list args) {
    u32 messageLife = sMessageLife;
    bool r26 = false;
    if (messageLife == 0) {
        if (device & 1) {
            sMessageLife = sDisplayTime;
            sMessageOwner = 3;
            snprintf(sMessageFileLine, sizeof(sMessageFileLine) - 1, "WARNING [%s:%d]", file, line);
        }
        vsnprintf(sMessageString, sizeof(sMessageString) - 1, msg, args);
        r26 = true;
    }
    if ((messageLife == 0 || mSynchro == 0) && (device & 2)) {
        JUTWarningConsole_f("WARNING [%s:%d] ", file, line);
        if (!r26) {
            JUTWarningConsole_f_va(msg, args);
        } else {
            JUTWarningConsole(sMessageString);
        }
        JUTWarningConsole("\n");
    }
}

void setWarningMessage_f(u32 device, char* file, int line, const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    setWarningMessage_f_va(device, file, line, msg, args);
    va_end(args);
}

void setLogMessage_f_va(u32 device, const char* file, int line, const char* msg, va_list args) {
    u32 messageLife = sMessageLife;
    bool r26 = false;
    if (messageLife == 0) {
        if (device & 1) {
            sMessageLife = sDisplayTime;
            sMessageOwner = 4;
            snprintf(sMessageFileLine, sizeof(sMessageFileLine) - 1, "[%s:%d]", file, line);
        }
        vsnprintf(sMessageString, sizeof(sMessageString) - 1, msg, args);
        r26 = true;
    }
    if ((messageLife == 0 || mSynchro == 0) && (device & 2)) {
        JUTReportConsole_f("[%s:%d] ", file, line);
        if (!r26) {
            JUTReportConsole_f_va(msg, args);
        } else {
            JUTReportConsole(sMessageString);
        }
        JUTReportConsole("\n");
    }
}

void setLogMessage_f(u32 device, char* file, int line, const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    setLogMessage_f_va(device, file, line, msg, args);
    va_end(args);
}

void setVisible(bool visible) {
    sAssertVisible = visible;
}

void setMessageCount(int msg_count) {
    sMessageLife = msg_count <= 0 ? 0 : msg_count;
}

};  // namespace JUTAssertion
