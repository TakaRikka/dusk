/**
 * m_Do_main.cpp
 * Main Initialization
 * PC Port Version - based on Aurora integration from Vorversion
 */

#include "m_Do/m_Do_main.h"
#include <string>
#include "DynamicLink.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "JSystem/JUtility/JUTReport.h"
#include "SSystem/SComponent/c_counter.h"
#include "Z2AudioLib/Z2WolfHowlMgr.h"
#include "c/c_dylink.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_pad.h"
#include "d/d_s_logo.h"
#include "d/d_s_menu.h"
#include "d/d_s_play.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_msg.h"
#include "m_Do/m_Do_MemCard.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_ext2.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_machine.h"
#include "m_Do/m_Do_printf.h"

#include <chrono>
#include <thread>
#include "dusk/dvd_emu.h"
#include "SSystem/SComponent/c_API.h"

#include <aurora/aurora.h>
#include <aurora/event.h>
#include <aurora/main.h>

// --- GLOBALS ---
s8 mDoMain::developmentMode = -1;
OSTime mDoMain::sPowerOnTime;
OSTime mDoMain::sHungUpTime;
u32 mDoMain::memMargin = 0xFFFFFFFF;
char mDoMain::COPYDATE_STRING[18] = "??/??/?? ??:??:??";

// --- PC LOGGING CALLBACK ---
void aurora_log_callback(AuroraLogLevel level, const char* module, const char* message,
                         unsigned int len) {
    const char* levelStr = "??";
    FILE* out = stdout;
    switch (level) {
    case LOG_DEBUG:
        levelStr = "DEBUG";
        break;
    case LOG_INFO:
        levelStr = "INFO";
        break;
    case LOG_WARNING:
        levelStr = "WARNING";
        break;
    case LOG_ERROR:
        levelStr = "ERROR";
        out = stderr;
        break;
    case LOG_FATAL:
        levelStr = "FATAL";
        out = stderr;
        break;
    }
    fprintf(out, "[%s | %s] %s\n", levelStr, module, message);
    if (level == LOG_FATAL) {
        fflush(out);
        abort();
    }
}

// =========================================================================
// LOAD_COPYDATE - PC Version using DvdEmu
// =========================================================================
#define COPYDATE_PATH "/str/Final/Release/COPYDATE"

s32 LOAD_COPYDATE(void*) {
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));

    u32 size = 0;
    void* data = DvdEmu::loadFile(COPYDATE_PATH, &size, nullptr);

    // Fallback: Try root if not found
    if (!data) {
        data = DvdEmu::loadFile("/COPYDATE", &size, nullptr);
    }

    if (data) {
        u32 copyLen = (size < sizeof(buffer) - 1) ? size : sizeof(buffer) - 1;
        memcpy(buffer, data, copyLen);
        buffer[copyLen] = '\0';

#ifdef _WIN32
        _aligned_free(data);
#else
        free(data);
#endif
    } else {
        strcpy(buffer, "PC PORT BUILD");
        OSReport("Warning: COPYDATE file not found at %s\n", COPYDATE_PATH);
    }

    memcpy(mDoMain::COPYDATE_STRING, buffer, sizeof(mDoMain::COPYDATE_STRING) - 1);
    mDoMain::COPYDATE_STRING[sizeof(mDoMain::COPYDATE_STRING) - 1] = '\0';

    OS_REPORT("\x1b[36mCOPYDATE=[%s]\n\x1b[m", mDoMain::COPYDATE_STRING);
    return 1;
}

// =========================================================================
// MAIN01 - THE GAME LOOP
// =========================================================================
void main01(void) {
    OS_REPORT("\x1b[m");

    // 1. Setup heaps, RNG, Threads
    // Nutzt den Speicher, den wir in game_main() per OSSetArena gesetzt haben
    mDoMch_Create();

    // 2. Setup GFX (FrameBuffer, ZBuffer)
    mDoGph_Create();

    // 3. Setup Pads
    //mDoCPd_c::create();

    // 4. Console Setup
    JUTConsole* console = JFWSystem::getSystemConsole();
    if (console) {
        console->setOutput(mDoMain::developmentMode ? JUTConsole::OUTPUT_OSR_AND_CONSOLE :
                                                      JUTConsole::OUTPUT_NONE);
        console->setPosition(32, 42);
    }

    // 5. Init Framework & Loader
    mDoDvdThd_callback_c::create((mDoDvdThd_callback_func)LOAD_COPYDATE, NULL);

    OSReport("Calling fapGm_Create()...\n");
    fapGm_Create();

    OSReport("Calling fopAcM_initManager()...\n");
    fopAcM_initManager();

    OSReport("Calling cDyl_InitAsync()...\n");
    cDyl_InitAsync();

    // -----------------------------------------------------------
    // THE GAME LOOP
    // -----------------------------------------------------------
    OSReport("Entering Main Loop (main01)...\n");
    do {
        // --- Window Events & Frame Start ---
        const AuroraEvent* event = aurora_update();
        if (event && event->type == AURORA_EXIT)
            break;

        if (!aurora_begin_frame()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        static u32 frame;
        frame++;

        mDoCPd_c::read();  // Read Controller

        // --- EXECUTE GAME LOGIC & RENDER ---
        fapGm_Execute();

        // --- Frame End & Limiter ---
        aurora_end_frame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS Cap

    } while (true);
}

// =========================================================================
// PC ENTRY POINT
// =========================================================================
int game_main(int argc, char* argv[]) {
    // 1. Aurora Init
    AuroraConfig config{};
    config.appName = "Zelda: Twilight Princess";
    config.desiredBackend = BACKEND_VULKAN;
    config.windowPosX = 100;
    config.windowPosY = 100;
    config.windowWidth = 640;
    config.windowHeight = 480;
    config.configPath = ".";
    config.logCallback = &aurora_log_callback;

    aurora_initialize(argc, argv, &config);

    // 2. Setup Virtual Game RAM
    // Simuliert den GameCube RAM (24MB + Audio etc, nehmen wir 256MB)
#define GAME_RAM_SIZE (256 * 1024 * 1024)
    void* virtualGameRam = malloc(GAME_RAM_SIZE);
    if (!virtualGameRam) {
        printf("Fatal: Failed to allocate game RAM\n");
        return -1;
    }
    memset(virtualGameRam, 0, GAME_RAM_SIZE);
    OSSetArenaLo(virtualGameRam);
    OSSetArenaHi((char*)virtualGameRam + GAME_RAM_SIZE);

    // 3. Init DVD Emulation
    DvdEmu::setBasePath("data");

    mDoMain::sPowerOnTime = OSGetTime();

    // Reset Data
    static mDoRstData sResetData = {0};
    mDoRst::setResetData(&sResetData);
    mDoRst::offReset();
    mDoRst::setLogoScnFlag(0);

    // Global Context Init
    dComIfG_ct();

    // Development Mode
    mDoMain::developmentMode = 1;  // Force Dev Mode for Debugging

    // START GAME
    OSReport("Starting main01 (Game Loop)...\n");

    // Auf PC rufen wir main01 direkt im Main Thread auf.
    // Kein OSCreateThread noetig (und auch gefaehrlich wegen Window-Loop).
    main01();

    // Cleanup (Erreicht nur bei Break der Loop)
    aurora_shutdown();
    free(virtualGameRam);

    return 0;
}

// =========================================================================
// STUBS & TEMPLATE INSTANTIATIONS
// =========================================================================
bool JKRHeap::dump_sort() {
    return true;
}

template<typename T>
JHIComPortManager<T>* JHIComPortManager<T>::instance = nullptr;

template<>
JHIComPortManager<JHICmnMem>* JHIComPortManager<JHICmnMem>::instance = nullptr;

template<>
Z2WolfHowlMgr* JASGlobalInstance<Z2WolfHowlMgr>::sInstance;

template<>
Z2EnvSeMgr* JASGlobalInstance<Z2EnvSeMgr>::sInstance;

template<>
Z2FxLineMgr* JASGlobalInstance<Z2FxLineMgr>::sInstance;

template<>
Z2Audience* JASGlobalInstance<Z2Audience>::sInstance;

template<>
Z2SoundObjMgr* JASGlobalInstance<Z2SoundObjMgr>::sInstance;

template<>
Z2SoundInfo* JASGlobalInstance<Z2SoundInfo>::sInstance;

template<>
JAUSoundInfo* JASGlobalInstance<JAUSoundInfo>::sInstance;

template<>
JAUSoundNameTable* JASGlobalInstance<JAUSoundNameTable>::sInstance;

template<>
JAUSoundTable* JASGlobalInstance<JAUSoundTable>::sInstance;

template<>
JAISoundInfo* JASGlobalInstance<JAISoundInfo>::sInstance;

template<>
Z2SoundMgr* JASGlobalInstance<Z2SoundMgr>::sInstance;

template<>
JAIStreamMgr* JASGlobalInstance<JAIStreamMgr>::sInstance;

template<>
JAISeqMgr* JASGlobalInstance<JAISeqMgr>::sInstance;

template<>
JAISeMgr* JASGlobalInstance<JAISeMgr>::sInstance;

template<>
Z2SpeechMgr2* JASGlobalInstance<Z2SpeechMgr2>::sInstance;

template<>
Z2SoundStarter* JASGlobalInstance<Z2SoundStarter>::sInstance;

template<>
JAISoundStarter* JASGlobalInstance<JAISoundStarter>::sInstance;

template<>
Z2StatusMgr* JASGlobalInstance<Z2StatusMgr>::sInstance;

template<>
Z2SceneMgr* JASGlobalInstance<Z2SceneMgr>::sInstance;

template<>
Z2SeqMgr* JASGlobalInstance<Z2SeqMgr>::sInstance;

template<>
Z2SeMgr* JASGlobalInstance<Z2SeMgr>::sInstance;

template<>
JASAudioThread* JASGlobalInstance<JASAudioThread>::sInstance;

template<>
JASDefaultBankTable* JASGlobalInstance<JASDefaultBankTable>::sInstance;
