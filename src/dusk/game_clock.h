#pragma once

namespace dusk::game_clock {

// Amount of time that a simulation tick advances
constexpr float kSimPeriod = 1.0f / 30.0f;
constexpr float kUiMaximumDt = 0.05f;
constexpr float kUiInitialDt = 1.0f / 60.0f;

struct FrameTiming {
    // Amount of time elapsed in seconds since the last advance
    float dt;
    // Whether interpolation is active
    bool interpolating;
    // Run simulation and presentation separately (for interpolation or time scaling)
    bool separatePresentation;
    // Number of simulation ticks to run
    int numSimTicks;
};
extern FrameTiming g_frameTiming;

void initialize();
void reset();
const FrameTiming& advance();
void begin_sim_tick();
void commit_sim_tick();
float sample_interpolation_step();

float consume_interval(const void* consumer);

// Sets the effective simulation rate through the game clock time scale.
void set_sim_rate(float hz);
float get_sim_rate();

} // namespace dusk::game_clock
