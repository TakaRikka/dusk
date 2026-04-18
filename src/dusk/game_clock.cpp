#include "dusk/game_clock.h"

#include <algorithm>
#include <chrono>
#include <unordered_map>

namespace dusk {
namespace game_clock {

using clock = std::chrono::steady_clock;

bool s_initialized = false;
clock::time_point s_previous_sample{};
float s_sim_accumulator = 0.0f;

std::unordered_map<uintptr_t, clock::time_point> s_interval_last_sample;

void ensure_initialized() {
    if (s_initialized) {
        return;
    }
    s_previous_sample = clock::now();
    s_sim_accumulator = sim_pace();
    s_initialized = true;
}

void reset_accumulator() {
    ensure_initialized();
    s_sim_accumulator = 0.0f;
}

MainLoopPacer advance_main_loop() {
    ensure_initialized();

    const clock::time_point now = clock::now();
    const float presentation_dt = std::chrono::duration<float>(now - s_previous_sample).count();
    s_previous_sample = now;

    s_sim_accumulator += presentation_dt;

    MainLoopPacer out{};
    out.presentation_dt_seconds = presentation_dt;

    const bool should_interpolate = dusk::getSettings().game.enableFrameInterpolation && !dusk::getTransientSettings().skipFrameRateLimit;
    out.is_interpolating = should_interpolate;
    out.sim_pace = sim_pace();

    if (!should_interpolate) {
        s_sim_accumulator = 0.0f;
        out.do_sim_tick = true;
        out.interpolation_step = 0.0f;
        return out;
    } else {
        out.do_sim_tick = s_sim_accumulator >= sim_pace();
        out.interpolation_step = out.do_sim_tick ? 0.0f : s_sim_accumulator / sim_pace();
        return out;
    }
}

float consume_interval(const void* consumer) {
    ensure_initialized();
    const uintptr_t key = reinterpret_cast<uintptr_t>(consumer);
    const clock::time_point now = clock::now();

    float dt = ui_initial_dt();
    const auto it = s_interval_last_sample.find(key);
    if (it != s_interval_last_sample.end()) {
        dt = std::chrono::duration<float>(now - it->second).count();
        dt = std::min(dt, ui_maximum_dt());
    }
    s_interval_last_sample[key] = now;
    return dt;
}

}  // namespace game_clock
}  // namespace dusk
