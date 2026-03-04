#include "assert.h"
#include <revolution/os.h>
#include <revolution/base/PPCArch.h>
#include <revolution/vi.h>
#include "console.h"
#include "mapFile.h"
#include "directPrint.h"

#include "global.h"

namespace nw4hbm {
    namespace db {
        using namespace detail;

        static OSAlarm sWarningAlarm;
        static u32 sWarningTime;
        static ConsoleHead* sAssertionConsole;
        static bool sDispWarningAuto;

        static void Assertion_Printf_(const char* fmt, ...) {
            va_list vlist;
            va_start(vlist, fmt);

            if (sAssertionConsole) {
                Console_VFPrintf(CONSOLE_OUTPUT_ALL, sAssertionConsole, fmt, vlist);
            } else {
                OSVReport(fmt, vlist);
            }

            va_end(vlist);
        }

        static bool ShowMapInfoSubroutine_(u32 address, u8 preCRFlag) {
            u8 strBuf[260];

            if (!MapFile_Exists()) {
                return false;
            }

            if (address < 0x80000000 || address > 0x82FFFFFF) {
                return false;
            }

            if (MapFile_QuerySymbol(address, strBuf, sizeof(strBuf))) {
                Assertion_Printf_("%s\n", strBuf);
                return true;  
            } else {
                return false;
            }
        }

        static void ShowStack_(u32 sp) NO_INLINE {
            Assertion_Printf_("-------------------------------- TRACE\n");
            Assertion_Printf_("Address:   BackChain   LR save\n");

            u32* p = (u32*)sp;

            for (u32 i = 0; i < 16; i++, p = (u32*)*p) {
                if (p == NULL || p == (u32*)0xFFFFFFFF || !((u32)p & 0x80000000))
                    break;

                Assertion_Printf_("%08X:  %08X    %08X ", p, p[0], p[1]);

                if (!ShowMapInfoSubroutine_(p[1], false)) {
                    Assertion_Printf_("\n");
                }
            }
        }

        DECL_WEAK void VPanic(const char* file, int line, const char* fmt, std::va_list vlist) {
            register u32 stackPointer;

            asm {
                lwz stackPointer, 0(r1)
            }

            OSDisableInterrupts();
            VISetPreRetraceCallback(NULL);
            VISetPostRetraceCallback(NULL);

            if (sAssertionConsole) {
                detail::DirectPrint_SetupFB(NULL);
            }

            ShowStack_(stackPointer);

            if (sAssertionConsole != NULL) {
                Console_Printf(sAssertionConsole, "%s:%d Panic:", file, line);
                Console_VFPrintf(CONSOLE_OUTPUT_ALL, sAssertionConsole, fmt, vlist);
                Console_Printf(sAssertionConsole, "\n");

                Console_ShowLatestLine(sAssertionConsole);
                Console_SetVisible(sAssertionConsole, true);
                Console_DrawDirect(sAssertionConsole);
            } else {
                OSReport("%s:%d Panic:", file, line);
                OSVReport(fmt, vlist);
                OSReport("\n");
            }

            PPCHalt();
        }

        DECL_WEAK void detail::Panic(const char* file, int line, const char* fmt, ...) {
            va_list vlist;
            va_start(vlist, fmt);
            VPanic(file, line, fmt, vlist);
        }

        static OSAlarm& GetWarningAlarm_();
        static void WarningAlarmFunc_(OSAlarm* alarm, OSContext* context);

        DECL_WEAK void VWarning(const char* file, int line, const char* fmt, std::va_list vlist) {
            if (sAssertionConsole != NULL) {
                Console_Printf(sAssertionConsole, "%s:%d Warning:", file, line);
                Console_VFPrintf(CONSOLE_OUTPUT_ALL, sAssertionConsole, fmt, vlist);
                Console_Printf(sAssertionConsole, "\n");
                Console_ShowLatestLine(sAssertionConsole);
                Console_SetVisible(sAssertionConsole, true);
            } else {
                OSReport("%s:%d Warning:", file, line);
                OSVReport(fmt, vlist);
                OSReport("\n");
            }
        }

        DECL_WEAK void detail::Warning(const char* file, int line, const char* fmt, ...) {
            OSAlarm& alarm = GetWarningAlarm_();

            va_list vlist;
            va_start(vlist, fmt);

            VWarning(file, line, fmt, vlist);

            va_end(vlist);

            if (sWarningTime > 0) {
                OSCancelAlarm(&sWarningAlarm);
                OSSetAlarm(&sWarningAlarm, sWarningTime, WarningAlarmFunc_);
            }
        }

        namespace detail {
            void Log(const char* fmt, ...) {
                va_list vlist;
            }
        }  // namespace detail

        ConsoleHead* Assertion_SetConsole(ConsoleHead* console) {
            ConsoleHead* before = sAssertionConsole;
            sAssertionConsole = console;
            return before;
        }

        ConsoleHead* Assertion_GetConsole() {
            return sAssertionConsole;
        }

        void Assertion_ShowConsole(u32 time) {
            if (sAssertionConsole != NULL) {
                OSAlarm& alarm = GetWarningAlarm_();
                OSCancelAlarm(&alarm);
                Console_SetVisible(sAssertionConsole, true);
                if (time != 0) {
                    OSSetAlarm(&alarm, time, WarningAlarmFunc_);
                }
            }
        }

        void Assertion_HideConsole() {
            // OSAlarm& alarm;
        }

        void Assertion_SetWarningTime(u32 time) {}

        static OSAlarm& GetWarningAlarm_() {
            static bool sInitializedAlarm;

            if (!sInitializedAlarm) {
                OSCreateAlarm(&sWarningAlarm);
                sInitializedAlarm = true;
            }

            return sWarningAlarm;
        }

        static void WarningAlarmFunc_(OSAlarm* alarm, OSContext* context) {
            Console_SetVisible(sAssertionConsole, false);
        }
    }  // namespace db
}  // namespace nw4r
