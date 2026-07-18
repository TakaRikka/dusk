#include <dolphin/types.h>
#include <d/d_kankyo.h>
#include <d/d_debug_pad.h>
#include "dusk/dusk.h"
#include "dusk/main.h"

bool dusk::IsRunning = true;
bool dusk::IsShuttingDown = false;
bool dusk::IsGameLaunched = false;
bool dusk::RestartRequested = false;
uint8_t dusk::SaveRequested = 0;
dusk::StageRequest dusk::StageRequested{"", false};
std::filesystem::path dusk::ConfigPath;
std::filesystem::path dusk::CachePath;
AuroraStats dusk::lastFrameAuroraStats;
float dusk::frameUsagePct = 0.0f;

void dusk::RequestRestart() noexcept {
    RestartRequested = SupportsProcessRestart;
    IsRunning = false;
}

u8 g_printOtherHeapDebug;

dKankyo_HIO_c g_kankyoHIO;

dDebugPad_c dDebugPad;

u32 __OSFpscrEnableBits;

// DSP
#include <dolphin/dsp.h>
DSPTaskInfo* __DSP_first_task;
DSPTaskInfo* __DSP_curr_task;

// mDo_dvd
#include <m_Do/m_Do_dvd_thread.h>
u8 mDoDvdThd::DVDLogoMode;
