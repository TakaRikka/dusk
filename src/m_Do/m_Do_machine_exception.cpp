/**
 * m_Do_machine_exception.cpp
 * Exception Print Functions
 */

#include "m_Do/m_Do_machine_exception.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_main.h"
#include "DynamicLink.h"

static JUTConsole* sConsole;

void print_f(char const* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sConsole->print_f_va(fmt, args);
    va_end(args);
}

void print(char const* string) {
    sConsole->print(string);
}

const char* stringBase_80374460 = "--------------------------------------\n";

void dispHeapInfo() {
    JKRExpHeap* zelda = zeldaHeap;
    JKRExpHeap* game = gameHeap;
    JKRExpHeap* archive = archiveHeap;

    u32 zeldaFree = zelda->getFreeSize();
    u32 gameFree = game->getFreeSize();
    u32 archiveFree = archive->getFreeSize();
    u32 zeldaTotal = zelda->getTotalFreeSize();
    u32 gameTotal = game->getTotalFreeSize();
    u32 archiveTotal = archive->getTotalFreeSize();

    print("-- Heap Free / TotalFree (KB) --\n");
    print_f("  Zelda %5d / %5d\n", zeldaFree / 1024, zeldaTotal / 1024);
    print_f("   Game %5d / %5d\n", gameFree / 1024, gameTotal / 1024);
    print_f("Archive %5d / %5d\n", archiveFree / 1024, archiveTotal / 1024);
    print("--------------------------------\n");

    JKRAramHeap* aramHeap = JKRAram::getAramHeap();
    if (aramHeap) {
        aramHeap->dump();
    }
    DynamicModuleControlBase::dump();
    g_dComIfG_gameInfo.mResControl.dump();

    zelda->dump_sort();
    game->dump_sort();
    archive->dump_sort();
}

void dispGameInfo() {
    print_f("Start StageName:RoomNo [%s:%d]\n", dComIfGp_getStartStageName(),
            dComIfGp_getStartStageRoomNo());
}

void dispDateInfo() {
    print("------------- Date Infomation ---------\n");
#if DEBUG
    print(" DEBUG VERSION\n");
    print("COMPILE USER: Authorized User\n");
#else
    print(" FINAL VERSION\n");
    print("COMPILE USER: FINAL\n");
#endif
    print_f("COPYDATE   : %17s\n", mDoMain::COPYDATE_STRING);

    OSCalendarTime time;
    OSTicksToCalendarTime(mDoMain::sPowerOnTime, &time);
    print_f("PowerOnTime: %04d/%2d/%2d %2d:%2d:%2d`%03d\"%03d\n", time.year, time.mon,
            time.mday, time.hour, time.min, time.sec, time.msec,
            time.usec);

    OSTicksToCalendarTime(mDoMain::sHungUpTime, &time);
    print_f("HungUpTime : %04d/%2d/%2d %2d:%2d:%2d`%03d\"%03d\n", time.year, time.mon,
            time.mday, time.hour, time.min, time.sec, time.msec,
            time.usec);

    OSTicksToCalendarTime(mDoMain::sHungUpTime - mDoMain::sPowerOnTime, &time);
    print_f("PlayTime   : %4d days, %2d:%2d:%2d`%03d\"%03d\n", time.yday, time.hour,
            time.min, time.sec, time.msec, time.usec);
    print("---------------------------------------\n");
}

void dispConsoleToTerminal() {
    JUTConsole* console = JFWSystem::getSystemConsole();
    console->dumpToTerminal(0xFFFFFFFF);
}

void exception_addition(JUTConsole* pConsole) {
    sConsole = pConsole;
    dispHeapInfo();
    dispDateInfo();
    dispConsoleToTerminal();
    dispGameInfo();
}
