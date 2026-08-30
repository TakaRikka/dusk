#include "dusk/game_clock.h"

#include <algorithm>
#include <aurora/time.hpp>
#include <chrono>
#include <cmath>
#include <dusk/frame_interpolation.h>
#include <unordered_map>

namespace dusk::game_clock {

using native_clock = aurora::time::native_clock;
using game_clock = aurora::time::game_clock;

FrameTiming g_frameTiming;

namespace {
bool s_initialized = false;
bool s_fixedStepActive = false;
bool s_simTickActive = false;
native_clock::time_point s_previousNativeSample{};
game_clock::time_point s_latestGameSample{};
game_clock::time_point s_currentSnapshotTime{};
game_clock::time_point s_pendingSimTime{};

std::unordered_map<uintptr_t, game_clock::time_point> s_intervalLastSample;

constexpr game_clock::duration kSimPeriodDuration =
    std::chrono::duration_cast<game_clock::duration>(std::chrono::duration<float>(kSimPeriod));
constexpr native_clock::duration kAbnormalGapResetThreshold = std::chrono::milliseconds(250);
constexpr int kMaxSimTicksPerFrame = static_cast<int>(aurora::time::kMaximumTimeScale) * 4;
}  // namespace

void initialize() {
    if (s_initialized) {
        return;
    }
    s_previousNativeSample = native_clock::now();
    s_latestGameSample = game_clock::now();
    s_currentSnapshotTime = s_latestGameSample;
    s_pendingSimTime = s_latestGameSample;
    s_initialized = true;
}

void reset() {
    s_previousNativeSample = native_clock::now();
    s_latestGameSample = game_clock::now();
    s_currentSnapshotTime = s_latestGameSample - kSimPeriodDuration;
    s_pendingSimTime = s_currentSnapshotTime;
    s_simTickActive = false;
}

void set_sim_rate(float hz) {
    const float maximumHz = aurora::time::kMaximumTimeScale / kSimPeriod;
    aurora::time::set_scale(std::clamp(hz, 1.0f, maximumHz) * kSimPeriod);
    reset();
}

float get_sim_rate() {
    return aurora::time::scale() / kSimPeriod;
}

const FrameTiming& advance() {
    const auto nativeNow = native_clock::now();
    const auto gameNow = game_clock::now();
    const auto nativeFrameGap = nativeNow - s_previousNativeSample;
    s_previousNativeSample = nativeNow;
    s_latestGameSample = gameNow;

    auto& out = g_frameTiming;
    out = {.dt = std::chrono::duration<float>().count()};

    const float timeScale = aurora::time::scale();
    const bool interpolating =
        getSettings().game.enableFrameInterpolation.getValue() != FrameInterpMode::Off;
    const bool separatePresentation = interpolating || timeScale != 1.0f;
    out.interpolating = interpolating;
    out.separatePresentation = separatePresentation;
    s_fixedStepActive = separatePresentation;

    if (!separatePresentation) {
        s_currentSnapshotTime = gameNow;
        out.numSimTicks = 1;
        return out;
    }

    const auto simulationTarget = interpolating ? gameNow - kSimPeriodDuration : gameNow;
    if (timeScale == 0.f || nativeFrameGap > kAbnormalGapResetThreshold) {
        s_currentSnapshotTime = simulationTarget;
        out.numSimTicks = 0;
        return out;
    }

    int numSimTicks = 0;
    auto projectedSnapshotTime = s_currentSnapshotTime;
    while (numSimTicks < kMaxSimTicksPerFrame) {
        const bool tickDue = interpolating ?
                                 projectedSnapshotTime < simulationTarget :
                                 projectedSnapshotTime + kSimPeriodDuration <= simulationTarget;
        if (!tickDue) {
            break;
        }
        projectedSnapshotTime += kSimPeriodDuration;
        numSimTicks++;
    }
    out.numSimTicks = numSimTicks;
    return out;
}

void begin_sim_tick() {
    s_pendingSimTime =
        s_fixedStepActive ? s_currentSnapshotTime + kSimPeriodDuration : s_latestGameSample;
    s_simTickActive = true;
}

void commit_sim_tick() {
    if (s_simTickActive) {
        s_currentSnapshotTime = s_pendingSimTime;
        s_simTickActive = false;
    } else {
        s_currentSnapshotTime += kSimPeriodDuration;
    }
}

float sample_interpolation_step() {
    const float step =
        std::chrono::duration<float>(game_clock::now() - s_currentSnapshotTime).count() /
        kSimPeriod;
    return std::clamp(step, 0.0f, 1.0f);
}

float consume_interval(const void* consumer) {
    const auto key = reinterpret_cast<uintptr_t>(consumer);
    const auto now = s_simTickActive ? s_pendingSimTime : game_clock::now();
    const float timeScale = aurora::time::scale();
    float dt = kUiInitialDt * timeScale;
    if (const auto it = s_intervalLastSample.find(key); it != s_intervalLastSample.end()) {
        dt = std::chrono::duration<float>(now - it->second).count();
        const float maximumDt = std::max(kUiMaximumDt * timeScale, kSimPeriod);
        dt = std::min(dt, maximumDt);
    }
    s_intervalLastSample[key] = now;
    return dt;
}

}  // namespace dusk::game_clock
