#ifndef NW4R_DB_CONSOLE_H
#define NW4R_DB_CONSOLE_H

#include <revolution/types.h>

#include "assert.h"
#include "../ut/CharWriter.h"
#include "../ut/TextWriterBase.h"

namespace nw4hbm {
    namespace db {
        namespace detail {
            struct ConsoleHead {
                /* 0x00 */ u8* textBuf;
                /* 0x04 */ u16 width;
                /* 0x06 */ u16 height;
                /* 0x08 */ u16 priority;
                /* 0x0A */ u16 attr;
                /* 0x0C */ u16 printTop;
                /* 0x0E */ u16 printXPos;
                /* 0x10 */ u16 printTopUsed;
                /* 0x12 */ u16 ringTop;
                /* 0x14 */ s32 ringTopLineCnt;
                /* 0x18 */ s32 viewTopLine;
                /* 0x1C */ s16 viewPosX;
                /* 0x1E */ s16 viewPosY;
                /* 0x20 */ u16 viewLines;
                /* 0x22 */ bool isVisible;
                /* 0x23 */ u8 padding_[1];
                /* 0x24 */ ut::TextWriterBase<char>* writer;
                /* 0x28 */ ConsoleHead* next;
            };
        }  // namespace detail

        enum ConsoleOutputType {
            CONSOLE_OUTPUT_NONE,
            CONSOLE_OUTPUT_TERMINAL,
            CONSOLE_OUTPUT_DISPLAY,
            CONSOLE_OUTPUT_ALL,
        };

        typedef detail::ConsoleHead* ConsoleHandle;

        typedef void (*VisitStringCallback)(detail::ConsoleHead* console, u8* r4, long r5, u32 r6);

        detail::ConsoleHead* Console_Create(void* buffer, u16 width, u16 height, u16 viewHeight,
                                            u16 priority, u16 attr);
        void Console_Destroy(detail::ConsoleHead* console);
        void Console_Clear(detail::ConsoleHead* console);
        void Console_Draw(detail::ConsoleHead* console, ut::TextWriterBase<char>& writer);
        void Console_DrawDirect(detail::ConsoleHead* console);
        void Console_DrawAll();
        void Console_DrawDirectAll();
        void Console_VFPrintf(ConsoleOutputType type, detail::ConsoleHead* console,
                              const char* format, std::va_list vlist);
        void Console_FPrintf(ConsoleOutputType type, detail::ConsoleHead* console,
                             const char* format);
        void Console_Printf(detail::ConsoleHead* console, const char* format, ...);
        void Console_PrintfD(detail::ConsoleHead* console, const char* format, ...);
        void Console_PrintfT(detail::ConsoleHead* console, const char* format, ...);
        u16 Console_ChangePriority(detail::ConsoleHead* console, u16 r4);
        void Console_VisitString(detail::ConsoleHead* console, VisitStringCallback visitor);
        long Console_GetTotalLines(detail::ConsoleHead* console);

        static long Console_SetViewBaseLine(detail::ConsoleHead* console, long line);
        static u16 Console_GetViewHeight(detail::ConsoleHead* console);

        static void Console_VPrintf(detail::ConsoleHead* console, const char* format, std::va_list vlist);

        static long Console_ShowLatestLine(detail::ConsoleHead* console) {
            long baseLine = Console_GetTotalLines(console) - Console_GetViewHeight(console);

            if (baseLine < 0)
                baseLine = 0;

            Console_SetViewBaseLine(console, baseLine);

            return baseLine;
        }

        static u16 Console_GetViewHeight(detail::ConsoleHead* console) {
            NW4R_ASSERT_CHECK_NULL(434, console);
            return console->viewLines;
        }

        static bool Console_SetVisible(detail::ConsoleHead* console, bool isVisible) {
            NW4R_ASSERT_CHECK_NULL(497, console);

            bool before = console->isVisible;
            console->isVisible = isVisible;
            return before;
        }

        static long Console_SetViewBaseLine(detail::ConsoleHead* console, long line) {
            NW4R_ASSERT_CHECK_NULL(557, console);
            long before = console->viewTopLine;
            console->viewTopLine = line;
            return before;
        }

    }  // namespace db
}  // namespace nw4hbm

#endif
