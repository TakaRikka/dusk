#ifndef JFWSYSTEM_H
#define JFWSYSTEM_H

#include <types.h>
#include "JSystem/JUtility/JUTAssert.h"

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
        static DUSK_GAME_DATA s32 maxStdHeaps;
        static DUSK_GAME_DATA u32 sysHeapSize;
        static DUSK_GAME_DATA u32 fifoBufSize;
        static DUSK_GAME_DATA u32 aramAudioBufSize;
        static DUSK_GAME_DATA u32 aramGraphBufSize;
        static DUSK_GAME_DATA s32 streamPriority;
        static DUSK_GAME_DATA s32 decompPriority;
        static DUSK_GAME_DATA s32 aPiecePriority;
        static DUSK_GAME_DATA ResFONT* systemFontRes;
        static DUSK_GAME_DATA const GXRenderModeObj* renderMode;
        static DUSK_GAME_DATA u32 exConsoleBufferSize;
    };

    static void firstInit();
    static void init();
#if TARGET_PC
    static void shutdown();
#endif

    static JUTConsole* getSystemConsole() { return systemConsole; }
    static JKRExpHeap* getSystemHeap() { return systemHeap; }
    static JKRExpHeap* getRootHeap() { return rootHeap; }
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

    static DUSK_GAME_DATA JKRExpHeap* rootHeap;
    static DUSK_GAME_DATA JKRExpHeap* systemHeap;
    static DUSK_GAME_DATA JKRThread* mainThread;
    static DUSK_GAME_DATA JUTDbPrint* debugPrint;
    static DUSK_GAME_DATA JUTResFont* systemFont;
    static DUSK_GAME_DATA JUTConsoleManager* systemConsoleManager;
    static DUSK_GAME_DATA JUTConsole* systemConsole;
    static DUSK_GAME_DATA bool sInitCalled;
};

#endif /* JFWSYSTEM_H */
