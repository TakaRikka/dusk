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
#include "SSystem/SComponent/c_API_graphic.h"
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

#include <filesystem>
#include <system_error>
#include <thread>
#include "SSystem/SComponent/c_API.h"
#include "dusk/app_info.hpp"
#include "dusk/crash_reporting.h"
#include "dusk/dusk.h"
#include "dusk/frame_interpolation.h"
#include "dusk/game_clock.h"
#include "dusk/gyro.h"
#include "dusk/imgui/ImGuiEngine.hpp"
#include "dusk/logging.h"
#include "dusk/main.h"
#include "dusk/imgui/ImGuiConsole.hpp"
#include "version.h"

#include <aurora/aurora.h>
#include <aurora/event.h>
#include <aurora/main.h>
#include <aurora/dvd.h>
#include <dolphin/dvd.h>

#include "SDL3/SDL_filesystem.h"
#include "cxxopts.hpp"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/config.hpp"
#include "dusk/settings.h"
#include "dusk/imgui/ImGuiConsole.hpp"
#include "dusk/discord_presence.hpp"
#include "tracy/Tracy.hpp"
#include "f_pc/f_pc_draw.h"

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
bool dusk::IsRunning = true;
bool dusk::IsShuttingDown = false;
bool dusk::IsGameLaunched = false;
bool dusk::IsFocusPaused = false;
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
AuroraStats dusk::lastFrameAuroraStats;
float dusk::frameUsagePct = 0.0f;
const char* configPath;

bool launchUILoop() {
    while (dusk::IsRunning && !dusk::IsGameLaunched) {
        const AuroraEvent* event = aurora_update();
        while (event != nullptr && event->type != AURORA_NONE) {
            switch (event->type) {
            case AURORA_SDL_EVENT:
                dusk::g_imguiConsole.HandleSDLEvent(event->sdl);
                break;
            case AURORA_DISPLAY_SCALE_CHANGED:
                dusk::ImGuiEngine_Initialize(event->windowSize.scale);
                break;
            case AURORA_EXIT:
                return false;
            }

            event++;
        }

        if (!aurora_begin_frame()) {
            DuskLog.debug("aurora_begin_frame returned false, skipping draw this frame");
            continue;
        }

        dusk::g_imguiConsole.PreDraw();

        dusk::g_imguiConsole.PostDraw();

        aurora_end_frame();
    }

    return dusk::IsRunning;
}

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

    dusk::game_clock::ensure_initialized();

    do {
        // 1. Update Window Events
        const AuroraEvent* event = aurora_update();
        while (true) {
            switch (event->type) {
            case AURORA_NONE:
                goto eventsDone;
            case AURORA_SDL_EVENT:
                dusk::g_imguiConsole.HandleSDLEvent(event->sdl);
                if (event->sdl.type == SDL_EVENT_WINDOW_FOCUS_LOST &&
                    dusk::getSettings().game.pauseOnFocusLost) {
                    dusk::IsFocusPaused = true;
                    dusk::audio::SetPaused(true);
                } else if (event->sdl.type == SDL_EVENT_WINDOW_FOCUS_GAINED &&
                           dusk::IsFocusPaused) {
                    dusk::IsFocusPaused = false;
                    dusk::audio::SetPaused(false);
                    dusk::game_clock::reset_frame_timer();
                }
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

        if (dusk::IsFocusPaused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }

        const dusk::game_clock::MainLoopPacer pacing = dusk::game_clock::advance_main_loop();

        VIWaitForRetrace();

        dusk::lastFrameAuroraStats = *aurora_get_stats();
        if (!aurora_begin_frame()) {
            DuskLog.debug("aurora_begin_frame returned false, skipping draw this frame");
            continue;
        }

        mDoGph_gInf_c::updateRenderSize();

        dusk::frame_interp::begin_frame(pacing.is_interpolating, pacing.do_sim_tick, pacing.interpolation_step);
        if (pacing.is_interpolating) {
            if (pacing.do_sim_tick) {
                dusk::frame_interp::set_ui_tick_pending(true);
                mDoCPd_c::read();
                DuskDebugPad();
                dusk::gyro::read(pacing.sim_pace);
                fapGm_Execute();
                mDoAud_Execute();
                dusk::game_clock::reset_accumulator();
            }
            dusk::frame_interp::interpolate();
            dusk::frame_interp::begin_presentation_camera();
            if (!pacing.do_sim_tick) {
                // run draw functions for anything specially marked to handle interp on non-sim
                // ticks
                fpcM_DrawIterater((fpcM_DrawIteraterFunc)fpcM_Draw);
            }
            cAPIGph_Painter();
            dusk::frame_interp::end_presentation_camera();
            dusk::frame_interp::set_ui_tick_pending(false);
        } else {
            dusk::frame_interp::set_ui_tick_pending(true);

            // Game Inputs
            mDoCPd_c::read();
            dusk::gyro::read(pacing.presentation_dt_seconds);

            // EXECUTE GAME LOGIC & RENDER
            // This calls mDoGph_Painter -> JFWDisplay -> GX Functions
            fapGm_Execute();

            mDoAud_Execute();
        }

        aurora_end_frame();

        FrameMark;

#ifdef DUSK_DISCORD_RPC
        dusk::discord::RunCallbacks();
        dusk::discord::UpdatePresence();
#endif
    } while (dusk::IsRunning);

    exit:;
}

static bool IsBackendAvailable(AuroraBackend backend) {
    if (backend == BACKEND_AUTO) {
        return true;
    }

    size_t availableBackendCount = 0;
    const AuroraBackend* availableBackends = aurora_get_available_backends(&availableBackendCount);
    for (size_t i = 0; i < availableBackendCount; ++i) {
        if (availableBackends[i] == backend) {
            return true;
        }
    }

    return false;
}

static AuroraBackend ResolveDesiredBackend(const cxxopts::ParseResult& parsedArgOptions) {
    AuroraBackend desiredBackend = BACKEND_AUTO;

    if (parsedArgOptions.count("backend") != 0) {
        const std::string backendArg = parsedArgOptions["backend"].as<std::string>();
        if (!dusk::try_parse_backend(backendArg, desiredBackend)) {
            fmt::print(stderr, "Unknown backend: {}\n", backendArg);
            exit(1);
        }
    } else if (!dusk::try_parse_backend(
                   static_cast<const std::string&>(dusk::getSettings().backend.graphicsBackend),
                   desiredBackend))
    {
        DuskLog.warn("Unknown configured backend '{}', falling back to Auto",
                     static_cast<const std::string&>(dusk::getSettings().backend.graphicsBackend));
        desiredBackend = BACKEND_AUTO;
    }

    if (!IsBackendAvailable(desiredBackend)) {
        DuskLog.warn("Requested backend '{}' is unavailable, falling back to Auto",
                     dusk::backend_name(desiredBackend));
        desiredBackend = BACKEND_AUTO;
    }

    return desiredBackend;
}

static void aurora_imgui_init_callback(const AuroraWindowSize* size) {
    dusk::ImGuiEngine_Initialize(size->scale);
    dusk::ImGuiEngine_AddTextures();
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

static void EnsureInitialPipelineCache(const char* configDir) {
    if (configDir == nullptr) {
        return;
    }

    const std::filesystem::path configPathFs(configDir);
    const std::filesystem::path pipelineCachePath = configPathFs / "pipeline_cache.db";
    if (std::filesystem::exists(pipelineCachePath)) {
        return;
    }

    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr) {
        DuskLog.warn("Unable to resolve base path while seeding pipeline cache: {}", SDL_GetError());
        return;
    }

    const std::filesystem::path initialPipelineCachePath =
        std::filesystem::path(basePath) / "initial_pipeline_cache.db";
    if (!std::filesystem::exists(initialPipelineCachePath)) {
        DuskLog.info("No bundled initial pipeline cache found at '{}'", initialPipelineCachePath.string());
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(configPathFs, ec);
    if (ec) {
        DuskLog.warn("Failed to create config directory '{}' for pipeline cache: {}",
                     configPathFs.string(), ec.message());
        return;
    }

    std::filesystem::copy_file(initialPipelineCachePath, pipelineCachePath, std::filesystem::copy_options::none, ec);
    if (ec) {
        DuskLog.warn("Failed to seed pipeline cache from '{}' to '{}': {}",
                     initialPipelineCachePath.string(), pipelineCachePath.string(), ec.message());
        return;
    }

    DuskLog.info("Seeded pipeline cache from '{}'", initialPipelineCachePath.string());
}

static constexpr PADDefaultMapping defaultPadMapping = {
    .buttons = {
        {SDL_GAMEPAD_BUTTON_SOUTH, PAD_BUTTON_A},
        {SDL_GAMEPAD_BUTTON_EAST, PAD_BUTTON_B},
        {SDL_GAMEPAD_BUTTON_WEST, PAD_BUTTON_X},
        {SDL_GAMEPAD_BUTTON_NORTH, PAD_BUTTON_Y},
        {SDL_GAMEPAD_BUTTON_START, PAD_BUTTON_START},
        {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, PAD_TRIGGER_Z},
        {PAD_NATIVE_BUTTON_INVALID, PAD_TRIGGER_L},
        {PAD_NATIVE_BUTTON_INVALID, PAD_TRIGGER_R},
        {SDL_GAMEPAD_BUTTON_DPAD_UP, PAD_BUTTON_UP},
        {SDL_GAMEPAD_BUTTON_DPAD_DOWN, PAD_BUTTON_DOWN},
        {SDL_GAMEPAD_BUTTON_DPAD_LEFT, PAD_BUTTON_LEFT},
        {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, PAD_BUTTON_RIGHT},
    },
    .axes = {
        {{SDL_GAMEPAD_AXIS_LEFTX, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_X_POS},
        {{SDL_GAMEPAD_AXIS_LEFTX, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_X_NEG},
        // SDL's gamepad y-axis is inverted from GC's
        {{SDL_GAMEPAD_AXIS_LEFTY, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_Y_POS},
        {{SDL_GAMEPAD_AXIS_LEFTY, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_Y_NEG},
        {{SDL_GAMEPAD_AXIS_RIGHTX, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_X_POS},
        {{SDL_GAMEPAD_AXIS_RIGHTX, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_X_NEG},
        // see above
        {{SDL_GAMEPAD_AXIS_RIGHTY, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_Y_POS},
        {{SDL_GAMEPAD_AXIS_RIGHTY, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_Y_NEG},
        {{SDL_GAMEPAD_AXIS_LEFT_TRIGGER, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_TRIGGER_L},
        {{SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_TRIGGER_R},
    },
};

static bool mainCalled = false;

// =========================================================================
// PC ENTRY POINT
// =========================================================================
int game_main(int argc, char* argv[]) {
    // On iOS, when connected to an external monitor, SDLUIKitSceneDelegate scene:willConnectToSession:
    // can call our main function again. Explicitly guard against this reinitialization.
    if (mainCalled) {
        return 0;
    }
    mainCalled = true;

    dusk::registerSettings();
    dusk::config::FinishRegistration();

    cxxopts::ParseResult parsed_arg_options;

    try {
        cxxopts::Options arg_options("Dusk", "PC Port of The Legend of Zelda: Twilight Princess");

        arg_options.add_options()
            ("l,log-level", "Log level from " + std::to_string(AuroraLogLevel::LOG_DEBUG) + " to " + std::to_string(AuroraLogLevel::LOG_FATAL), cxxopts::value<uint8_t>()->default_value("0"))
            ("h,help", "Print usage")
            ("console", "Show the Windows console window for logs", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
            ("dvd", "Path to DVD image file", cxxopts::value<std::string>())
            ("backend", "Graphics API backend to use (auto, d3d12, metal, vulkan, null)", cxxopts::value<std::string>())
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
    const auto startupLogLevel = static_cast<AuroraLogLevel>(parsed_arg_options["log-level"].as<uint8_t>());
    dusk::InitializeFileLogging(configPath, startupLogLevel);

    dusk::config::LoadFromUserPreferences();
    ApplyCVarOverrides(parsed_arg_options["cvar"]);
    dusk::InitializeCrashReporting();
    EnsureInitialPipelineCache(configPath);

    AuroraConfig config{};
    config.appName = dusk::AppName;
    config.configPath = configPath;
    config.vsync = dusk::getSettings().video.enableVsync;
    config.startFullscreen = dusk::getSettings().video.enableFullscreen;
    config.windowPosX = -1;
    config.windowPosY = -1;
    config.windowWidth = defaultWindowWidth * 2;
    config.windowHeight = defaultWindowHeight * 2;
    config.desiredBackend = ResolveDesiredBackend(parsed_arg_options);
    config.logCallback = &aurora_log_callback;
    config.logLevel = startupLogLevel;
    config.mem1Size = 256 * 1024 * 1024;
    config.mem2Size = 24 * 1024 * 1024;
    config.allowJoystickBackgroundEvents = true;
    config.imGuiInitCallback = &aurora_imgui_init_callback;
    config.allowTextureReplacements = true;
    config.allowTextureDumps = false;

    PADSetDefaultMapping(&defaultPadMapping);

    auroraInfo = aurora_initialize(argc, argv, &config);

#ifdef DUSK_DISCORD_RPC
    dusk::discord::Initialize();
#endif

    VISetWindowTitle(
        fmt::format("Dusk {} [{}]", DUSK_WC_DESCRIBE, dusk::backend_name(auroraInfo.backend))
        .c_str());

    if (dusk::getSettings().video.lockAspectRatio) {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
    } else {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_STRETCH);
    }
    VISetFrameBufferScale(dusk::getSettings().game.internalResolutionScale.getValue());

    dusk::audio::SetMasterVolume(dusk::getSettings().audio.masterVolume / 100.0f);
    dusk::audio::SetEnableReverb(dusk::getSettings().audio.enableReverb);

    std::string dvd_path;
    bool dvd_opened = false;
    if (parsed_arg_options.count("dvd")) {
        dvd_path = parsed_arg_options["dvd"].as<std::string>();
        DuskLog.info("Loading DVD image from command line: {}", dvd_path);
        dvd_opened = aurora_dvd_open(dvd_path.c_str());
        if (!dvd_opened) {
            DuskLog.warn("Failed to open DVD image from command line: {}, opening prelaunch UI", dvd_path);
        } else {
            dusk::getSettings().backend.isoPath.setValue(dvd_path);
            dusk::config::Save();
            dusk::IsGameLaunched = true;
        }
    }

    if (!dvd_opened) {
        // pre game launch ui main loop
        if (!launchUILoop()) {
            dusk::ShutdownCrashReporting();
#ifdef DUSK_DISCORD_RPC
            dusk::discord::Shutdown();
#endif
            aurora_shutdown();
            return 0;
        }

        dvd_path = dusk::getSettings().backend.isoPath;

        if (dvd_path.empty()) {
            DuskLog.fatal("No DVD image specified, unable to boot!");
        }
        DuskLog.info("Loading DVD image: {}", dvd_path);
        if (!aurora_dvd_open(dvd_path.c_str())) {
            DuskLog.fatal("Failed to open DVD image: {}", dvd_path);
        }
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

    dusk::ShutdownCrashReporting();
    dusk::ShutdownFileLogging();
    fflush(stdout);
    fflush(stderr);

    mDoMch_Destroy();

    // Notifies all CVs and causes threads to exit
    OSResetSystem(OS_RESET_SHUTDOWN, 0, 0);

#ifdef DUSK_DISCORD_RPC
    dusk::discord::Shutdown();
#endif
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
