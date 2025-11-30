#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTGraphFifo.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "JSystem/JUtility/JUTDbPrint.h"

s32 JFWSystem::CSetUpParam::maxStdHeaps = 2;

u32 JFWSystem::CSetUpParam::sysHeapSize = 0x400000;

JKRExpHeap* JFWSystem::rootHeap;

JKRExpHeap* JFWSystem::systemHeap;


u32 JFWSystem::CSetUpParam::fifoBufSize = 0x40000;

u32 JFWSystem::CSetUpParam::aramAudioBufSize = 0x800000;

u32 JFWSystem::CSetUpParam::aramGraphBufSize = 0x600000;

s32 JFWSystem::CSetUpParam::streamPriority = 8;

s32 JFWSystem::CSetUpParam::decompPriority = 7;

s32 JFWSystem::CSetUpParam::aPiecePriority = 6;

ResFONT* JFWSystem::CSetUpParam::systemFontRes = (ResFONT*)&JUTResFONT_Ascfont_fix12;

const GXRenderModeObj* JFWSystem::CSetUpParam::renderMode = &GXNtsc480IntDf;

u32 JFWSystem::CSetUpParam::exConsoleBufferSize = 0x24FC;

void JFWSystem::firstInit() {
    JUT_ASSERT(80, rootHeap == NULL);
    OSInit();
    DVDInit();
    rootHeap = JKRExpHeap::createRoot(CSetUpParam::maxStdHeaps, false);
    systemHeap = JKRExpHeap::create(CSetUpParam::sysHeapSize, rootHeap, false);
}

JKRThread* JFWSystem::mainThread;

JUTDbPrint* JFWSystem::debugPrint;

JUTResFont* JFWSystem::systemFont;

JUTConsoleManager* JFWSystem::systemConsoleManager;

JUTConsole* JFWSystem::systemConsole;

bool JFWSystem::sInitCalled = false;

// NONMATCHING - regalloc, equivalent
void JFWSystem::init() {
    JUT_ASSERT(101, sInitCalled == false);

    if (rootHeap == NULL) {
        firstInit();
    }
    sInitCalled = true;

    JKRAram::create(CSetUpParam::aramAudioBufSize, CSetUpParam::aramGraphBufSize,
                    CSetUpParam::streamPriority, CSetUpParam::decompPriority,
                    CSetUpParam::aPiecePriority);
    mainThread = new JKRThread(OSGetCurrentThread(), 4);

    JUTVideo::createManager(CSetUpParam::renderMode);
    JUTCreateFifo(CSetUpParam::fifoBufSize);

    JUTGamePad::init();

    JUTDirectPrint* dbPrint = JUTDirectPrint::start();

    JUTAssertion::create();

    JUTException::create(dbPrint);

    systemFont = new JUTResFont(CSetUpParam::systemFontRes, NULL);

    debugPrint = JUTDbPrint::start(NULL, NULL);
    debugPrint->changeFont(systemFont);

    systemConsoleManager = JUTConsoleManager::createManager(NULL);

    systemConsole = JUTConsole::create(60, 200, NULL);
    systemConsole->setFont(systemFont);

    if (CSetUpParam::renderMode->efbHeight < 300) {
        systemConsole->setFontSize(systemFont->getWidth() * 0.85f, systemFont->getHeight() * 0.5f);
        systemConsole->setPosition(20, 25);
    } else {
        systemConsole->setFontSize(systemFont->getWidth(), systemFont->getHeight());
        systemConsole->setPosition(20, 50);
    }

    systemConsole->setHeight(25);
    systemConsole->setVisible(false);
    systemConsole->setOutput(JUTConsole::OUTPUT_OSREPORT | JUTConsole::OUTPUT_CONSOLE);
    JUTSetReportConsole(systemConsole);
    JUTSetWarningConsole(systemConsole);

    void* buffer = systemHeap->alloc(CSetUpParam::exConsoleBufferSize, 4);
    JUTException::createConsole(buffer, CSetUpParam::exConsoleBufferSize);
}
