#ifndef JFWSYSTEM_H
#define JFWSYSTEM_H

#include "dolphin/types.h"
#include "JSystem/JUtility/JUTAssert.h"

typedef struct _GXRenderModeObj GXRenderModeObj;
class JKRExpHeap;
class JKRThread;
class JUTConsole;
class JUTConsoleManager;
class JUTDbPrint;
class JUTResFont;
struct ResFONT;

/**
 * @ingroup jsystem-jframework
 * 
 */
struct JFWSystem {
    struct CSetUpParam {
        static s32 maxStdHeaps;
        static u32 sysHeapSize;
        static u32 fifoBufSize;
        static u32 aramAudioBufSize;
        static u32 aramGraphBufSize;
        static s32 streamPriority;
        static s32 decompPriority;
        static s32 aPiecePriority;
        static ResFONT* systemFontRes;
        static const GXRenderModeObj* renderMode;
        static u32 exConsoleBufferSize;
    };

    static void firstInit();
    static void init();

    static JUTConsole* getSystemConsole() { return systemConsole; }
    static JKRExpHeap* getSystemHeap() { return systemHeap; }
    static JUTResFont* getSystemFont() { return systemFont; }

    static void setMaxStdHeap(int max) {
        JUT_ASSERT(47, sInitCalled == FALSE);
        CSetUpParam::maxStdHeaps = max;
    }
    static void setSysHeapSize(u32 size) {
        JUT_ASSERT(50, sInitCalled == FALSE);
        CSetUpParam::sysHeapSize = size;
    }
    static void setFifoBufSize(u32 size) {
        JUT_ASSERT(53, sInitCalled == FALSE);
        CSetUpParam::fifoBufSize = size;
    }
    static void setAramAudioBufSize(u32 size) {
        JUT_ASSERT(58, sInitCalled == FALSE);
        CSetUpParam::aramAudioBufSize = size;
    }
    static void setAramGraphBufSize(u32 size) {
        JUT_ASSERT(61, sInitCalled == FALSE);
        CSetUpParam::aramGraphBufSize = size;
    }
    static void setRenderMode(const GXRenderModeObj* p_modeObj) {
        JUT_ASSERT(80, sInitCalled == FALSE);
        CSetUpParam::renderMode = p_modeObj;
    }

    static JKRExpHeap* rootHeap;
    static JKRExpHeap* systemHeap;
    static JKRThread* mainThread;
    static JUTDbPrint* debugPrint;
    static JUTResFont* systemFont;
    static JUTConsoleManager* systemConsoleManager;
    static JUTConsole* systemConsole;
    static bool sInitCalled;
};

#endif /* JFWSYSTEM_H */
