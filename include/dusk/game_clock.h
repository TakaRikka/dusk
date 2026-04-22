#pragma once

namespace dusk::game_clock {

void ensure_initialized();
void reset_frame_timer();

constexpr float sim_pace() { return 1.0f / 30.0f; }
constexpr float period_for_original_frames(float frame_count) { return frame_count * sim_pace(); }
constexpr float ui_maximum_dt() { return 0.05f; }
constexpr float ui_initial_dt() { return 1.0f / 60.0f; }

struct MainLoopPacer {
    float presentation_dt_seconds;
    bool is_interpolating;
    int sim_ticks_to_run;
    float sim_pace;
};

MainLoopPacer advance_main_loop();
void commit_sim_tick();
float sample_interpolation_step();

float consume_interval(const void* consumer);

} // namespace dusk::game_clock
