/**
 * m_Do_machine.cpp
 * Exception Management
 */

#include "m_Do/m_Do_machine.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JKernel/JKRAramStream.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "JSystem/JKernel/JKRAram.h"
#include "SSystem/SComponent/c_malloc.h"
#include "SSystem/SComponent/c_math.h"
#include "SSystem/SComponent/c_API_controller_pad.h"
#include "base/PPCArch.h"
#include "m_Do/m_Do_DVDError.h"
#include "m_Do/m_Do_MemCard.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_machine_exception.h"
#include "m_Do/m_Do_main.h"
#include "DynamicLink.h"

#if DEBUG
u8 mDoMch::mDebugFill = true;
u8 mDoMch::mDebugFillNotUse = 0xDD;
u8 mDoMch::mDebugFillNew = 0xF7;
u8 mDoMch::mDebugFillDelete = 0xDD;

u8 mDoMch::myHeapVerbose;
u8 mDoMch::myHeapCallbackCheck;
u8 mDoMch::FpscrEnableBits;
u8 mDoMch::GXWarningLevel;
u8 mDoMch::GXWarningExecuteFrame;
#else
u8 mDoMch::mDebugFill;
#endif

static int solidHeapErrors;

static int gameHeapErrors;

static int zeldaHeapErrors;

static int commandHeapErrors;

static int archiveHeapErrors;

static int unknownHeapErrors;

static u32 heapErrors;

#if VERSION == VERSION_GCN_USA || VERSION == VERSION_GCN_JPN
GXRenderModeObj g_ntscZeldaIntDf = {
    VI_TVMODE_NTSC_INT,
    608,
    448,
    448,
    27,
    16,
    666,
    448,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
    {8, 8, 10, 12, 10, 8, 8},
};

GXRenderModeObj g_ntscZeldaProg = {
    VI_TVMODE_NTSC_PROG,
    608,
    448,
    448,
    27,
    16,
    666,
    448,
    VI_XFBMODE_SF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
    {0, 0, 21, 22, 21, 0, 0},
};
#elif VERSION == VERSION_GCN_PAL
GXRenderModeObj g_ntscZeldaIntDf = {
    VI_TVMODE_PAL_INT,
    608,
    448,
    538,
    25,
    18,
    670,
    538,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
    {8, 8, 10, 12, 10, 8, 8},
};

GXRenderModeObj g_ntscZeldaProg = {
    VI_TVMODE_EURGB60_INT,
    608,
    448,
    448,
    27,
    16,
    666,
    448,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
    {8, 8, 10, 12, 10, 8, 8},
};
#else
GXRenderModeObj g_ntscZeldaIntDf = {
    VI_TVMODE_NTSC_INT,
    640,
    456,
    456,
    25,
    12,
    670,
    456,
    VI_XFBMODE_DF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
    {8, 8, 10, 12, 10, 8, 8},
};

GXRenderModeObj g_ntscZeldaProg = {
    VI_TVMODE_NTSC_PROG,
    640,
    456,
    456,
    25,
    12,
    670,
    456,
    VI_XFBMODE_SF,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
    {0, 0, 21, 22, 21, 0, 0},
};
#endif

#ifdef DEBUG
static void myGXVerifyCallback(GXWarningLevel, u32, const char*);

GXRenderModeObj g_palZeldaProg60 = {
    VI_TVMODE_EURGB60_PROG,
    640, 456, 456, 25, 12, 670, 456,
    VI_XFBMODE_SF, 0, 0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
    { 8, 8, 10, 12, 10, 8, 8 },
};
#endif

GXRenderModeObj* mDoMch_render_c::mRenderModeObj = &g_ntscZeldaIntDf;

static const char* myGetHeapTypeByString(JKRHeap* p_heap) {
    static char tmpString[5];

    if (p_heap == JKRGetSystemHeap()) {
        return "SystemHeap";
    }

    if (p_heap == mDoExt_getZeldaHeap()) {
        return "ZeldaHeap";
    }

    if (p_heap == mDoExt_getGameHeap()) {
        return "GameHeap";
    }

    if (p_heap == mDoExt_getArchiveHeap()) {
        return "ArchiveHeap";
    }

    if (p_heap == mDoExt_getCommandHeap()) {
        return "CommandHeap";
    }

    if (p_heap == mDoExt_getDbPrintHeap()) {
        return "DbPrintHeap";
    }

    if (p_heap == mDoExt_getJ2dHeap()) {
        return "J2dHeap";
    }

    u32 heapType = p_heap->getHeapType();
    switch (heapType) {
    case 'ASTH':
        return "ASTH";
    case 'EXPH':
        return "EXPH";
    case 'SLID':
        return "SLID";
    case 'STDH':
        return "STDH";
    case 'UNIT':
        return "UNIT";
    case 0:
        return "(Null)";
    default:
        u32 tmpType = heapType;
        char* typeStr = (char*)&tmpType;

        tmpString[0] = typeStr[0];
        tmpString[1] = typeStr[1];
        tmpString[2] = typeStr[2];
        tmpString[3] = typeStr[3];
        tmpString[4] = 0;
        return tmpString;
    }
}

static void myMemoryErrorRoutine(void* p_heap, u32 size, int alignment) {
    JKRHeap* heap = (JKRHeap*)p_heap;

    BOOL notSolidHeap = true;
    if (heap->getHeapType() == 'SLID') {
        notSolidHeap = false;
    }

    if (notSolidHeap) {
        // "Error: Can't allocate memory %d(0x%x)Bytes, %d Byte Alignment from %08x\n"
        OSReport_Error(
            "エラー: メモリを確保できません %d(0x%x)バイト、 %d バイトアライメント from %08x\n",
            size, size, alignment, p_heap);
    }

    u32 heapType = heap->getHeapType();
    u8* tmpString = (u8*)&heapType;
    if (notSolidHeap) {
        OSReport_Error(
            "FreeSize=%08x TotalFreeSize=%08x HeapType=%08x(%c%c%c%c) HeapSize=%08x %s\n",
            heap->getFreeSize(), heap->getTotalFreeSize(), heapType, tmpString[0], tmpString[1],
            tmpString[2], tmpString[3], heap->getHeapSize(), myGetHeapTypeByString(heap));
    }

    if (heapErrors == 0) {
        heap->dump_sort();
    }
    heapErrors++;

    if (p_heap == zeldaHeap) {
        zeldaHeapErrors++;
    } else if (p_heap == gameHeap) {
        gameHeapErrors++;
    } else if (p_heap == commandHeap) {
        commandHeapErrors++;
    } else if (p_heap == archiveHeap) {
        archiveHeapErrors++;
    } else if (heap->getHeapType() == 'SLID') {
        solidHeapErrors++;
    } else {
        unknownHeapErrors++;
    }
}

void myHeapCheckRecursive(JKRHeap* p_heap) {
    if (!p_heap->check()) {
        const char* type = myGetHeapTypeByString(p_heap);
        OSReport_Error("error in %08x(%s)\n", p_heap, type);
    }

    JSUTree<JKRHeap>* heapTree = &p_heap->getHeapTree();
    for (JSUTree<JKRHeap>* i = heapTree->getFirstChild(); i != NULL; i = i->getNextChild()) {
        myHeapCheckRecursive(i->getObject());
    }
}

void mDoMch_HeapCheckAll() {
    myHeapCheckRecursive(JKRHeap::sRootHeap);
}

static int developKeyCheck(u32 btnTrig, u32 btnHold) {
    static u8 key_link;
    static u8 key_ganon;
    static u8 key_zelda;

    if (btnHold == (PAD_TRIGGER_L | PAD_TRIGGER_R | PAD_TRIGGER_Z | PAD_BUTTON_DOWN) &&
        btnTrig == PAD_BUTTON_DOWN) {
        if (key_link == 3 && key_ganon == 6 && key_zelda == 5) {
            mDoMain::developmentMode = 1;
        } else {
            key_link = 0;
            key_ganon = 0;
            key_zelda = 0;
        }
    }

    if (btnHold == (PAD_TRIGGER_L | PAD_BUTTON_RIGHT) && btnTrig == PAD_TRIGGER_L) {
        key_link++;
    }

    if (btnHold == (PAD_TRIGGER_R | PAD_BUTTON_UP) && btnTrig == PAD_TRIGGER_R) {
        key_ganon++;
    }

    if (btnHold == (PAD_TRIGGER_Z | PAD_BUTTON_LEFT) && btnTrig == PAD_TRIGGER_Z) {
        key_zelda++;
    }

    return mDoMain::developmentMode;
}

BOOL mDoMch_IsProgressiveMode() {
    return OSGetProgressiveMode() == true;
}

bool exceptionReadPad(u32* p_btnTrig, u32* p_btnHold) {
    return JUTException::getManager()->readPad(p_btnTrig, p_btnHold);
}

void exceptionRestart() {
    mDoRst_reset(0, 0, 0);
    OSResetSystem(0, 0, 0);
    do {
    } while (true);
}

void myExceptionCallback(u16, OSContext*, u32, u32) {
    u32 btnHold;
    u32 btnTrig;

    mDoMain::sHungUpTime = OSGetTime();
    OSReportEnable();
    cAPICPad_recalibrate();
    // "Vibration stopping & resetting to default\n"
    OSReport("振動停止＆原点復帰\n");

    JUTException* manager = JUTException::getManager();

    if (manager == NULL) {
        // "Exception Manager doesn't exist\n"
        OSReport("例外マネージャがありません\n");
        PPCHalt();
    } else {
        manager->setTraceSuppress(0x80);
        if (mDoMain::developmentMode == 0) {
            JUTGamePad pad(JUTGamePad::EPort1);
            manager->setGamePad(&pad);

            if (manager != NULL) {
                OSEnableInterrupts();
                // "Accepting Key input\n"
                OSReport("キー入力を受け付けています\n");
                while (mDoMain::developmentMode == 0) {
                    exceptionReadPad(&btnTrig, &btnHold);
                    developKeyCheck(btnTrig, btnHold);
                    JUTException::waitTime(30);
                    if (JUTGamePad::C3ButtonReset::sResetOccurred) {
                        exceptionRestart();
                    }
                }
                // "JUTAssertion is visible\n"
                OSReport("JUTAssertionを可視化しました\n");
                JUTAssertion::setVisible(true);
                JUTDbPrint::getManager()->setVisible(true);
                JFWSystem::getSystemConsole()->setOutput(JUTConsole::OUTPUT_OSREPORT |
                                                         JUTConsole::OUTPUT_CONSOLE);
            } else {
                PPCHalt();
            }
        } else {
            // "Wait for 3 seconds\n"
            OSReport("3秒間停止\n");
            JUTException::waitTime(3000);
        }
    }
    DVDChangeDir("/map/Final/Release");
    JUTVideo::destroyManager();
    GXSetDrawDoneCallback(NULL);
    VISetBlack(0);
    VIFlush();
}

static void fault_callback_scroll(u16, OSContext* p_context, u32, u32) {
    JUTException* manager = JUTException::getManager();
    JUTConsole* exConsole = manager->getConsole();

    u32 srr0 = p_context->srr0 & -4;
    if (srr0 >= 0x8000000C && srr0 < 0x82FFFFFF) {
        exConsole->print_f("(SRR0-3):%08X %08X %08X %08X\n", *(u32*)(srr0 - 0xC),
                           *(u32*)(srr0 - 0x8), *(u32*)(srr0 - 0x4), *(u32*)srr0);
    }

    JUTGamePad pad(JUTGamePad::EPort1);
    manager->setGamePad(&pad);

    BOOL padDisabled = manager->isEnablePad() == false;
    if (!padDisabled) {
        exConsole->print("PUSH START BUTTON TO ADDITIONAL INFOMATION\n");
        exConsole->print("--------------------------------------\n");
        JUTConsoleManager::getManager()->drawDirect(true);
        OSEnableInterrupts();
        int holdUpCount = 0;
        int holdDownCount = 0;

        while (true) {
            u32 btnHold, btnTrig;
            exceptionReadPad(&btnTrig, &btnHold);

            if (JUTGamePad::C3ButtonReset::sResetOccurred) {
                OSResetSystem(1, 0, 0);
            }

            bool waitRetrace = false;
            if (btnTrig == PAD_BUTTON_START) {
                exception_addition(exConsole);
                waitRetrace = true;
            }

            if (btnTrig == PAD_TRIGGER_Z) {
                JUTConsole* sysConsole = JFWSystem::getSystemConsole();
                if (JUTConsoleManager::getManager()->getDirectConsole() != sysConsole) {
                    exConsole = sysConsole;
                    exConsole->setFontSize(8.0f, 6.0f);
                    exConsole->setPosition(8, 32);
                    exConsole->setHeight(23);
                    exConsole->setVisible(true);
                    exConsole->setOutput(JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT);
                } else {
                    exConsole = JUTException::getConsole();
                }
                JUTConsoleManager::getManager()->setDirectConsole(exConsole);
                waitRetrace = true;
            }

            if (btnTrig == PAD_BUTTON_A) {
                exConsole->scrollToLastLine();
                waitRetrace = true;
            }

            if (btnTrig == PAD_BUTTON_B) {
                exConsole->scrollToFirstLine();
                waitRetrace = true;
            }

            if (btnHold == PAD_BUTTON_UP) {
                exConsole->scroll(holdUpCount < 3 ? -1 : (holdUpCount < 5 ? -2 : (holdUpCount < 7 ? -4 : -8)));
                waitRetrace = true;
                holdDownCount = 0;
                holdUpCount++;
            } else if (btnHold == PAD_BUTTON_DOWN) {
                exConsole->scroll(holdDownCount < 3 ? 1 : (holdDownCount < 5 ? 2 : (holdDownCount < 7 ? 4 : 8)));
                waitRetrace = true;
                holdUpCount = 0;
                holdDownCount++;
            } else {
                holdUpCount = 0;
                holdDownCount = 0;
            }

            if (waitRetrace == true) {
                u32 count = VIGetRetraceCount();
                do {
                } while (count == VIGetRetraceCount());
                JUTConsoleManager::getManager()->drawDirect(true);
            }

            JUTException::waitTime(30);
        }
    }

    exception_addition(exConsole);
    JUTConsoleManager::getManager()->drawDirect(true);

    do {
        exConsole->scrollToFirstLine();
        JUTConsoleManager::getManager()->drawDirect(true);
        JUTException::waitTime(2000);

        do {
            for (u32 i = exConsole->getHeight(); i != 0; i--) {
                exConsole->scroll(1);
                JUTConsoleManager::getManager()->drawDirect(true);

                if ((exConsole->getUsedLine() - exConsole->getHeight()) + 1 <=
                    exConsole->getLineOffset()) {
                    break;
                }
                JUTException::waitTime(20);
            }

            JUTException::waitTime(3000);
             if ((exConsole->getUsedLine() - exConsole->getHeight()) + 1 <=
                exConsole->getLineOffset()) {
                break;
            }
        } while (true);
    } while (true);
}

static void dummy_string() {
    DEAD_STRING("\x1B[32m%-24s = size=%d KB\n\x1B[m");
}

static void my_PrintHeap(char const* heapName, u32 heapSize) {
    OS_REPORT("\x1b[32m%-24s = size=%d KB\n\x1b[m", heapName, heapSize / 1024);
}

void my_SysPrintHeap(char const* message, void* start, u32 size) {
    uintptr_t end = (uintptr_t)start + size;
    OSReport_System("\x1b[32m%-24s = %08x-%08x size=%d KB\n\x1b[m", message, start,
                    end, size / 1024);
}

int mDoMch_Create() {
    if (mDoMain::developmentMode == 0 || !(OSGetConsoleType() & 0x10000000)) {
        OSReportDisable();
    }

    JKRHeap::setDefaultDebugFill(mDoMch::mDebugFill);
    #ifdef DEBUG
    JKRSetDebugFillNotuse(mDoMch::mDebugFillNotUse);
    JKRSetDebugFillNew(mDoMch::mDebugFillNew);
    JKRSetDebugFillDelete(mDoMch::mDebugFillDelete);
    #endif
    JFWSystem::setMaxStdHeap(1);

    #ifndef DEBUG
    uintptr_t arenaHi = (uintptr_t)OSGetArenaHi();
    uintptr_t arenaLo = (uintptr_t)OSGetArenaLo();

    if (arenaHi > 0x81800000 && arenaHi - 0x1800000 > arenaLo) {
        OSSetArenaHi((void*)(arenaHi - 0x1800000));
    }
    #endif

    u32 arenaSize = ((uintptr_t)OSGetArenaHi() - (uintptr_t)OSGetArenaLo()) - 0xF0;
    my_PrintHeap("アリーナ", arenaSize);

    if (mDoMain::memMargin != -1) {
        arenaSize += mDoMain::memMargin;
    }

    JKRHeap* heap; // sp4C
    u32 archiveHeapSize; // r30
    u32 j2dHeapSize; // sp48
    u32 gameHeapSize; // sp44
    u32 commandHeapSize; // sp40
    u32 dynamicLinkHeapSize; // sp3C
    u32 dbPrintHeapSize; // sp38
    dbPrintHeapSize = 0x1800;
    commandHeapSize = 0x1000;
    gameHeapSize = 0x400000;
    archiveHeapSize = 0x740000;
    archiveHeapSize += 0xED000;
    archiveHeapSize += 0xA0000;
    gameHeapSize += 0x4E000;
    archiveHeapSize += 0x30000;
    archiveHeapSize += 0x2000;
    archiveHeapSize += 0x4B000;
    archiveHeapSize -= 0xC0000;
    archiveHeapSize += 0x4800;
    j2dHeapSize = 0xBB800;
    archiveHeapSize += 0x10000;
    archiveHeapSize += 0x2400;
    archiveHeapSize += 0x80000;
    archiveHeapSize += 0x100000;
    archiveHeapSize += 0x400000;
    archiveHeapSize += 0x100000;
    gameHeapSize += 0x200000;
    gameHeapSize += 0x100000;
    dynamicLinkHeapSize = 0x180000;

    #ifndef DEBUG
    // Fakematch because the heap sizes differ between debug and retail.
    // Maybe the actual calculations above use sizeof or constants and that's why it's different?
    archiveHeapSize -= 0x641800;
    j2dHeapSize -= 0x3E800;
    gameHeapSize -= 0x300000;
    #endif
    #if VERSION == VERSION_GCN_JPN
    archiveHeapSize += 0x6C00;
    gameHeapSize += 0xC800;
    #endif

    #ifdef DEBUG
    if (mDoMain::archiveHeapSize != -1) {
        OSReport_Error("アーカイブヒープサイズ指定！\n");
        archiveHeapSize = mDoMain::archiveHeapSize;
    } else {
        mDoMain::archiveHeapSize = archiveHeapSize + 0x14E000;
    }
    if (mDoMain::gameHeapSize != -1) {
        OSReport_Error("ゲームヒープサイズ指定！\n");
        gameHeapSize = mDoMain::gameHeapSize;
    } else {
        mDoMain::gameHeapSize = gameHeapSize;
    }
    arenaSize -= dynamicLinkHeapSize;
    arenaSize -= gameHeapSize + commandHeapSize;
    #endif

    arenaSize -= (dbPrintHeapSize + 0x10);
    arenaSize -= 0x120;
    #ifndef DEBUG
    arenaSize -= 0xDAB400;
    #endif
    #if VERSION == VERSION_GCN_JPN
    arenaSize -= 0x6C00;
    arenaSize -= 0xC800;
    #endif
    JFWSystem::setSysHeapSize(arenaSize);
    my_PrintHeap("システムヒープ", arenaSize);

    if (arenaSize) {
        // Fakematch to force arenaSize into a register for debug.
    }

    JFWSystem::setFifoBufSize(0xA0000);
    #ifdef DEBUG
    JFWSystem::setAramAudioBufSize(0xB00000);
    #else
    JFWSystem::setAramAudioBufSize(0xA00000);
    #endif
    JFWSystem::setAramGraphBufSize(-1);

    #ifdef DEBUG
    VIInit();
    if (VIGetDTVStatus() != 0 && mDoMch_IsProgressiveMode()) {
        mDoMch_render_c::setProgressiveMode();
        OSReport("=== プログレッシブモードです ===\n");
    } else if (VIGetTvFormat() == 5) {
        mDoMch_render_c::setRenderModeObj(&g_palZeldaProg60);
    }
    #elif VERSION != VERSION_GCN_PAL
    if ((OSGetResetCode() >> 31) == 0) {
        if (VIGetDTVStatus() == 0) {
            OSSetProgressiveMode(0);
        }
    } else if (mDoRst::getProgSeqFlag() != 0) {
        if (VIGetDTVStatus() != 0 && mDoMch_IsProgressiveMode()) {
            mDoMch_render_c::setProgressiveMode();
        }
    }
    #else
    if ((int)(OSGetResetCode() >> 31) == 1) {
        if (mDoRst::getProgSeqFlag() != 0) {
            if (OSGetEuRgb60Mode() == OS_EURGB60_ON) {
                mDoMch_render_c::setProgressiveMode();
            }
        }
    }
    #endif

    JFWSystem::setRenderMode(mDoMch_render_c::getRenderModeObj());
    JFWSystem::firstInit();
    JKRExpHeap* dbPrintHeap = mDoExt_createDbPrintHeap(dbPrintHeapSize, JKRGetRootHeap());
    JUTDbPrint::start(NULL, dbPrintHeap);
    mDoExt_createAssertHeap(JKRGetRootHeap());
    JFWSystem::init();

    if (mDoMain::developmentMode == 0) {
        JUTAssertion::setVisible(false);
        JUTDbPrint::getManager()->setVisible(false);
    }

    JKRSetErrorHandler(myMemoryErrorRoutine);
    JKRSetErrorFlag(JKRHeap::getRootHeap(), true);
    JKRSetErrorFlag(JFWSystem::getSystemHeap(), true);

    JKRHeap* rootHeap = (JKRHeap*)JKRGetRootHeap();
    #ifdef DEBUG
    JKRHeap* rootHeap2 = JKRGetRootHeap2();
    #else
    JKRHeap* rootHeap2 = rootHeap;
    #endif

    // Command Heap size: 4 KB
    heap = mDoExt_createCommandHeap(commandHeapSize, rootHeap);
    my_SysPrintHeap("コマンドヒープ", heap, commandHeapSize);

    #ifdef DEBUG
    heap = DynamicModuleControlBase::createHeap(dynamicLinkHeapSize, rootHeap);
    my_SysPrintHeap("ダイナミックリンクヒープ", heap, dynamicLinkHeapSize);
    #endif

    // Archive Heap size: 9085 KB
    heap = mDoExt_createArchiveHeap(archiveHeapSize, rootHeap2);
    my_SysPrintHeap("アーカイブヒープ", heap, archiveHeapSize);

    // J2D Heap size: 500 KB
    heap = mDoExt_createJ2dHeap(j2dHeapSize, rootHeap2);
    my_SysPrintHeap("Ｊ２Ｄ用ヒープ", heap, j2dHeapSize);

    // Game Heap size: 4408 KB
    heap = mDoExt_createGameHeap(gameHeapSize, rootHeap);
    my_SysPrintHeap("ゲームヒープ", heap, gameHeapSize);

    #ifdef DEBUG
    JKRHeap* sp28 = rootHeap2;
    u32 hostIOHeapSize = 0x71450;
    hostIOHeapSize += 0x32000;
    JKRHeap* hostIOHeap = mDoExt_createHostIOHeap(hostIOHeapSize, sp28);
    my_SysPrintHeap("HostIOヒープ", hostIOHeap, hostIOHeapSize);
    #endif

    JKRHeap* systemHeap = JKRGetSystemHeap();
    s32 size = systemHeap->getFreeSize();
    size -= 0x10000;
    JUT_ASSERT(1549, size > 0);
    JKRHeap* zeldaHeap = mDoExt_createZeldaHeap(size, systemHeap);
    my_SysPrintHeap("ゼルダヒープ", zeldaHeap, size);
    JKRSetCurrentHeap(zeldaHeap);

    #ifdef DEBUG
    my_PrintHeap("システムヒープ", JKRGetSystemHeap()->getTotalFreeSize());
    my_PrintHeap("ルートヒープ", JKRGetRootHeap()->getTotalFreeSize());
    my_PrintHeap("ルートヒープ2", JKRGetRootHeap2()->getTotalFreeSize());
    #endif

    JKRSetAramTransferBuffer(NULL, 0x2000, JKRGetSystemHeap());
    JKRThreadSwitch::createManager(NULL);
    { // Fakematch to fix stack on debug, unsure where this block is supposed to go
    JKRThread* thread = new JKRThread(OSGetCurrentThread(), 0);
    }

    JUTConsole* sysConsole = JFWSystem::getSystemConsole();
    sysConsole->setOutput(JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT);
    sysConsole->setPosition(16, 42);

#ifdef DEBUG
    JUTException::setMapFile("/map/RVL/Debug/RframeworkD.map");
#else
    JUTException::appendMapFile("/map/Final/Release/frameworkF.map");
#endif
    JUTException::setPreUserCallback(myExceptionCallback);
    JUTException::setPostUserCallback(fault_callback_scroll);

    cMl::init(mDoExt_getZeldaHeap());
    cM_initRnd(100, 100, 100);
    #ifdef DEBUG
    GXSetVerifyLevel((GXWarningLevel)mDoMch::GXWarningLevel);
    GXSetVerifyCallback((GXVerifyCallback)&myGXVerifyCallback);
    #endif
    JKRDvdRipper::setSZSBufferSize(0x4000);
    JKRDvdAramRipper::setSZSBufferSize(0x4000);
    JKRAram::setSZSBufferSize(0x2000);
    mDoDvdThd::create(OSGetThreadPriority(OSGetCurrentThread()) - 2);
    mDoDvdErr_ThdInit();
    mDoMemCd_ThdInit();

    return 1;
}
