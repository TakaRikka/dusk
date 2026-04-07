/**
 * m_Do_main.cpp
 * Main Initialization
 * PC Port Version - based on Aurora integration from Vorversion
 */

#include "m_Do/m_Do_main.h"
#include <dolphin/vi.h>
#include <cstring>
#include "DynamicLink.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
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
#include "m_Do/m_Do_ext2.h"
#include "SSystem/SComponent/c_counter.h"
#include <cstring>

#include <chrono>
#include <thread>
#include "SSystem/SComponent/c_API.h"
#include "dusk/app_info.hpp"
#include "dusk/dusk.h"
#include "dusk/imgui/ImGuiEngine.hpp"
#include "dusk/logging.h"
#include "dusk/main.h"
#include "dusk/time.h"

#include <aurora/aurora.h>
#include <aurora/event.h>
#include <aurora/main.h>
#include <aurora/dvd.h>
#include <dolphin/dvd.h>

#include "SDL3/SDL_filesystem.h"
#include "cxxopts.hpp"
#include "dusk/config.hpp"

// --- GLOBALS ---
s8 mDoMain::developmentMode = -1;
OSTime mDoMain::sPowerOnTime;
OSTime mDoMain::sHungUpTime;
u32 mDoMain::memMargin = 0xFFFFFFFF;
char mDoMain::COPYDATE_STRING[18] = "??/??/?? ??:??:??";
#if TARGET_PC
const int audioHeapSize = 0x14D800 * 2;
#else
const int audioHeapSize = 0x14D800;
#endif

// =========================================================================
// LOAD_COPYDATE - PC Version
// =========================================================================
#define COPYDATE_PATH "/str/Final/Release/COPYDATE"

#if TARGET_PC
bool dusk::IsShuttingDown = false;
#endif

s32 LOAD_COPYDATE(void*) {
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));

    DVDFileInfo fi;
    if (DVDOpen(COPYDATE_PATH, &fi)) {
        u32 readLen = (fi.length < sizeof(buffer) - 1) ? fi.length : sizeof(buffer) - 1;
        // DVDReadPrio requires 32-byte aligned buffer and length rounded up to 32
        u32 alignedLen = (readLen + 31) & ~31;
        alignas(32) char readBuf[64];
        DVDReadPrio(&fi, readBuf, alignedLen, 0, 2);
        DVDClose(&fi);

        memcpy(buffer, readBuf, readLen);
        buffer[readLen] = '\0';
    } else {
        strcpy(buffer, "PC PORT BUILD");
        DuskLog.warn("COPYDATE file not found at {}", COPYDATE_PATH);
    }

    memcpy(mDoMain::COPYDATE_STRING, buffer, sizeof(mDoMain::COPYDATE_STRING) - 1);
    mDoMain::COPYDATE_STRING[sizeof(mDoMain::COPYDATE_STRING) - 1] = '\0';

    DuskLog.info("COPYDATE=[{}]", mDoMain::COPYDATE_STRING);
    return 1;
}

AuroraInfo auroraInfo;
const char* configPath;

void main01(void) {
    OS_REPORT("\x1b[m");

    // 1. Setup
    mDoMch_Create();
    mDoGph_Create();
    mDoCPd_c::create();

    // Console Setup
    JUTConsole* console = JFWSystem::getSystemConsole();
    if (console) {
        console->setOutput(mDoMain::developmentMode ? JUTConsole::OUTPUT_OSR_AND_CONSOLE :
                                                      JUTConsole::OUTPUT_NONE);
        console->setPosition(32, 42);
    }

    // Loader Init
    mDoDvdThd_callback_c::create((mDoDvdThd_callback_func)LOAD_COPYDATE, NULL);

    OSReport("Calling fapGm_Create()...\n");
    fapGm_Create();

    OSReport("Calling fopAcM_initManager()...\n");
    fopAcM_initManager();

    OSReport("Calling cDyl_InitAsync()...\n");
    cDyl_InitAsync();

    g_mDoAud_audioHeap = JKRCreateSolidHeap(audioHeapSize, JKRGetCurrentHeap(), false);
    JKRHEAP_NAME(g_mDoAud_audioHeap, "g_mDoAud_audioHeap");

    if (DUSK_AUDIO_DISABLED) {
        // Pretend the audio engine initialized already. This is a lie, but needed to boot.
        mDoAud_zelAudio_c::onInitFlag();
    }

    OSReport("Entering Main Loop (main01)...\n");


    do {
        // 1. Update Window Events
        const AuroraEvent* event = aurora_update();
        while (true) {
            switch (event->type) {
            case AURORA_NONE:
                goto eventsDone;
            case AURORA_WINDOW_RESIZED:
                mDoGph_gInf_c::setWindowSize(event->windowSize);
                break;
            case AURORA_DISPLAY_SCALE_CHANGED:
                dusk::ImGuiEngine_Initialize(event->windowSize.scale);
                break;
            case AURORA_EXIT:
                goto exit;
            }

            event++;
        }

        eventsDone:;

        static u32 frame = 0;
        frame++;

        // Game Inputs
        mDoCPd_c::read();

        VIWaitForRetrace();

#if TARGET_PC
        if (!aurora_begin_frame()) {
            DuskLog.debug("aurora_begin_frame returned false, skipping draw this frame");
            continue;
        }
#endif

        // EXECUTE GAME LOGIC & RENDER
        // This calls mDoGph_Painter -> JFWDisplay -> GX Functions
        fapGm_Execute();

        mDoAud_Execute();

        aurora_end_frame();
    } while (true);

    exit:;
}

static AuroraBackend ParseAuroraBackend(const std::string& value) {
    if (value == "auto") {
        return BACKEND_AUTO;
    }
    if (value == "d3d11") {
        return BACKEND_D3D11;
    }
    if (value == "d3d12") {
        return BACKEND_D3D12;
    }
    if (value == "metal") {
        return BACKEND_METAL;
    }
    if (value == "vulkan") {
        return BACKEND_VULKAN;
    }
    if (value == "opengl") {
        return BACKEND_OPENGL;
    }
    if (value == "opengles") {
        return BACKEND_OPENGLES;
    }
    if (value == "webgpu") {
        return BACKEND_WEBGPU;
    }
    if (value == "null") {
        return BACKEND_NULL;
    }

    fmt::print(stderr, "Unknown backend: {}", value);
    exit(1);
}

static void aurora_imgui_init_callback(const AuroraWindowSize* size) {
    dusk::ImGuiEngine_Initialize(size->scale);
}

static void ApplyCVarOverrides(const cxxopts::OptionValue& option) {
    if (option.count() == 0) {
        return;
    }

    const auto& cVars = option.as<std::vector<std::string>>();
    for (const auto& cvarArg : cVars) {
        const auto sep = cvarArg.find('=');
        if (sep == std::string::npos) {
            DuskLog.fatal("--cvar argument has no '=': '{}'", cvarArg);
            continue;
        }

        const auto name = std::string_view(cvarArg).substr(0, sep);
        const auto value = std::string_view(cvarArg).substr(sep + 1);

        const auto cVar = dusk::config::GetConfigVar(name);
        if (!cVar) {
            DuskLog.fatal("Unknown --cvar name: '{}'", name);
        }

        try {
            cVar->getImpl()->loadFromArg(*cVar, value);
        } catch (const std::exception& e) {
            DuskLog.fatal("Unable to parse: '{}': {}", value, e.what());
        }
    }
}

static const char* CalculateConfigPath() {
    const auto result = SDL_GetPrefPath(dusk::OrgName, dusk::AppName);
    if (!result) {
        DuskLog.fatal("Unable to get PrefPath: {}", SDL_GetError());
    }

    return result;
}

// =========================================================================
// PC ENTRY POINT
// =========================================================================
int game_main(int argc, char* argv[]) {
    dusk::registerSettings();
    dusk::config::FinishRegistration();

    cxxopts::ParseResult parsed_arg_options;

    try {
        cxxopts::Options arg_options("Dusk", "PC Port of The Legend of Zelda: Twilight Princess");

        arg_options.add_options()
            ("l,log-level", "Log level from " + std::to_string(AuroraLogLevel::LOG_DEBUG) + " to " + std::to_string(AuroraLogLevel::LOG_FATAL), cxxopts::value<uint8_t>()->default_value("0"))
            ("h,help", "Print usage")
            ("dvd", "Path to DVD image file", cxxopts::value<std::string>()->default_value("game.iso"))
            ("backend", "Graphics API backend to use (auto, d3d11, d3d12, metal, vulkan, opengl, opengles, webgpu, null)", cxxopts::value<std::string>()->default_value("auto"))
            ("cvar", "Override configuration variables without modifying config", cxxopts::value<std::vector<std::string>>());

        arg_options.parse_positional({"dvd"});
        arg_options.positional_help("<dvd-image>");
        arg_options.allow_unrecognised_options();

        parsed_arg_options = arg_options.parse(argc, argv);

        if (parsed_arg_options.count("help"))
        {
            printf("%s", (arg_options.help() + "\n").c_str());
            exit(0);
        }
    }
    catch (const cxxopts::exceptions::exception& e) {
        fprintf(stderr, "Argument Error: %s\n", e.what());
        exit(1);
    }

    configPath = CalculateConfigPath();

    dusk::config::LoadFromUserPreferences();
    ApplyCVarOverrides(parsed_arg_options["cvar"]);

    AuroraConfig config{};
    config.appName = dusk::AppName;
    config.configPath = configPath;
    config.vsync = dusk::getSettings().video.enableVsync;
    config.startFullscreen = dusk::getSettings().video.enableFullscreen;
    config.windowPosX = -1;
    config.windowPosY = -1;
    config.windowWidth = FB_WIDTH * 2;
    config.windowHeight = FB_HEIGHT * 2;
    config.desiredBackend = ParseAuroraBackend(parsed_arg_options["backend"].as<std::string>());
    config.logCallback = &aurora_log_callback;
    config.logLevel = (AuroraLogLevel)parsed_arg_options["log-level"].as<uint8_t>();
    config.mem1Size = 256 * 1024 * 1024;
    config.mem2Size = 24 * 1024 * 1024;
    config.allowJoystickBackgroundEvents = true;
    config.imGuiInitCallback = &aurora_imgui_init_callback;

    auroraInfo = aurora_initialize(argc, argv, &config);

    const auto& dvd_path = parsed_arg_options["dvd"].as<std::string>();
    DuskLog.info("Loading DVD image: {}", dvd_path);
    if (!aurora_dvd_open(dvd_path.c_str())) {
        DuskLog.fatal("Failed to open DVD image: {}", dvd_path);
    }

    OSInit();

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
    mDoDvdThd::SyncWidthSound = false;

    OSReport("Starting main01 (Game Loop)...\n");


    main01();

    fflush(stdout);
    fflush(stderr);

    mDoMch_Destroy();

    // Notifies all CVs and causes threads to exit
    OSResetSystem(OS_RESET_SHUTDOWN, 0, 0);

    aurora_shutdown();

    return 0;
}


bool JKRHeap::dump_sort() {
    return true;
}

#ifdef __MWERKS__
template <typename T>
JHIComPortManager<T>* JHIComPortManager<T>::instance = nullptr;

template <>
JHIComPortManager<JHICmnMem>* JHIComPortManager<JHICmnMem>::instance = nullptr;

template<>
Z2WolfHowlMgr* JASGlobalInstance<Z2WolfHowlMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2EnvSeMgr* JASGlobalInstance<Z2EnvSeMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2FxLineMgr* JASGlobalInstance<Z2FxLineMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2Audience* JASGlobalInstance<Z2Audience>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SoundObjMgr* JASGlobalInstance<Z2SoundObjMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SoundInfo* JASGlobalInstance<Z2SoundInfo>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAUSoundInfo* JASGlobalInstance<JAUSoundInfo>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAUSoundNameTable* JASGlobalInstance<JAUSoundNameTable>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAUSoundTable* JASGlobalInstance<JAUSoundTable>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAISoundInfo* JASGlobalInstance<JAISoundInfo>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SoundMgr* JASGlobalInstance<Z2SoundMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAIStreamMgr* JASGlobalInstance<JAIStreamMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAISeqMgr* JASGlobalInstance<JAISeqMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAISeMgr* JASGlobalInstance<JAISeMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SpeechMgr2* JASGlobalInstance<Z2SpeechMgr2>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SoundStarter* JASGlobalInstance<Z2SoundStarter>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JAISoundStarter* JASGlobalInstance<JAISoundStarter>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2StatusMgr* JASGlobalInstance<Z2StatusMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SceneMgr* JASGlobalInstance<Z2SceneMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SeqMgr* JASGlobalInstance<Z2SeqMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
Z2SeMgr* JASGlobalInstance<Z2SeMgr>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JASAudioThread* JASGlobalInstance<JASAudioThread>::sInstance JAS_GLOBAL_INSTANCE_INIT;

template<>
JASDefaultBankTable* JASGlobalInstance<JASDefaultBankTable>::sInstance JAS_GLOBAL_INSTANCE_INIT;
#endif // __MWERKS__
