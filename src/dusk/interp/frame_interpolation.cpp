#include "dusk/interp/frame_interpolation.h"

#include "dusk/game_clock.h"
#include "dusk/interp/dual_buffer.h"
#include "dusk/interp/lerp.h"

#include "mtx.h"

#include <absl/container/flat_hash_map.h>
#include <algorithm>
#include <vector>

namespace dusk::interp {
void camera_on_sim_tick();
void camera_on_begin_record();
bool camera_apply_presentation();
void camera_restore_presentation();
void camera_invalidate_snapshots();
}  // namespace dusk::interp

namespace {

struct Recording {
    absl::flat_hash_map<uintptr_t, Mtx> matrix_values;
};

bool s_recording = false;
bool s_replacementsActive = false;
bool s_syncPresentation = false;

float s_step = 0.0f;
bool s_uiTickPending = false;
uint64_t s_simTickSeq = 0;
uint64_t s_observedPresentationEpoch = 0;

Recording s_currentRecording;
Recording s_previousRecording;

absl::flat_hash_map<uintptr_t, Mtx> g_replacements;

int s_presentationDepth = 0;

const Mtx* resolve_replacement(const Mtx* source, Mtx* scratch) {
    if (!s_replacementsActive || source == nullptr || dusk::interp::presentation_sync_active()) {
        return source;
    }

    auto it = g_replacements.find(reinterpret_cast<uintptr_t>(source));
    if (it == g_replacements.end()) {
        return source;
    }

    MTXCopy(it->second, *scratch);
    return scratch;
}

bool has_recording_data(const Recording& recording) {
    return !recording.matrix_values.empty();
}

void clear_replacements() {
    g_replacements.clear();
}

void interpolate_replacements() {
    clear_replacements();
    s_replacementsActive = dusk::interp::is_enabled() && !s_recording && !s_syncPresentation &&
                           has_recording_data(s_currentRecording);
    if (!s_replacementsActive) {
        return;
    }
    for (auto const& old : s_previousRecording.matrix_values) {
        if (auto it = s_currentRecording.matrix_values.find(old.first);
            it != s_currentRecording.matrix_values.end())
        {
            dusk::interp::lerp(g_replacements[old.first], old.second, it->second, s_step);
        }
    }
}

struct InterpolationCallBackWork {
    dusk::interp::InterpolationCallBack pCallBack;
    void* pUserWork;
};

std::vector<InterpolationCallBackWork> s_interpolationCallBackWork;

void clear_callbacks() {
    s_interpolationCallBackWork.clear();
}

void callbacks_run() {
    for (const auto& work : s_interpolationCallBackWork) {
        if (work.pCallBack != nullptr) {
            work.pCallBack(work.pUserWork);
        }
    }
}

void clear_interpolation_history() {
    s_recording = false;
    s_replacementsActive = false;
    s_syncPresentation = false;
    s_previousRecording = {};
    s_currentRecording = {};
    clear_replacements();
    dusk::interp::clear_owned_buffers();
    clear_callbacks();
    dusk::interp::camera_invalidate_snapshots();
    s_presentationDepth = 0;
}

}  // namespace

namespace dusk::interp {

void begin_sim_tick() {
    if (!is_enabled()) {
        return;
    }

    clear_callbacks();
    camera_on_sim_tick();
    ++s_simTickSeq;
}

uint64_t sim_tick_seq() {
    return s_simTickSeq;
}

void begin_frame(float step) {
    const game_clock::FrameTiming& timing = game_clock::g_frameTiming;
    if (s_observedPresentationEpoch != timing.presentationEpoch) {
        s_observedPresentationEpoch = timing.presentationEpoch;
        clear_interpolation_history();
    }

    s_step = std::clamp(step, 0.0f, 1.0f);
    if (!is_enabled()) {
        clear_interpolation_history();
    }
}

bool is_enabled() {
    return game_clock::g_frameTiming.interpolating;
}

void begin_record() {
    if (!is_enabled()) {
        clear_interpolation_history();
        return;
    }

    s_syncPresentation = false;
    s_previousRecording = std::move(s_currentRecording);
    s_currentRecording = {};
    s_recording = true;
    s_replacementsActive = false;
    clear_replacements();
    camera_on_begin_record();
}

void end_record() {
    s_recording = false;
}

void request_presentation_sync() {
    if (!is_enabled()) {
        return;
    }
    s_syncPresentation = true;
}

bool presentation_sync_active() {
    if (!is_enabled()) {
        return false;
    }
    return s_syncPresentation;
}

float get_interpolation_step() {
    return presentation_sync_active() ? 1.0f : s_step;
}

void set_ui_tick_pending(bool value) {
    if (s_uiTickPending == value) {
        return;
    }
    s_uiTickPending = value;
}

bool get_ui_tick_pending() {
    return is_enabled() ? s_uiTickPending : true;
}

void record_final_mtx(Mtx m, const void* key) {
    if (!s_recording || m == nullptr) {
        return;
    }

    auto& it = s_currentRecording.matrix_values[reinterpret_cast<uintptr_t>(key)];
    MTXCopy(m, it);
}

void record_final_mtx(Mtx m) {
    record_final_mtx(m, m);
}

bool lookup_replacement(const void* key, Mtx out) {
    if (presentation_sync_active() || !s_replacementsActive || key == nullptr) {
        return false;
    }

    auto it = g_replacements.find(reinterpret_cast<uintptr_t>(key));
    if (it == g_replacements.end()) {
        return false;
    }

    MTXCopy(it->second, out);
    return true;
}

bool lookup_concat_replacement(const void* lhs, const void* rhs, Mtx out) {
    if (presentation_sync_active() || !s_replacementsActive || lhs == nullptr || rhs == nullptr) {
        return false;
    }

    Mtx lhs_scratch;
    Mtx rhs_scratch;
    const Mtx* resolved_lhs = resolve_replacement(reinterpret_cast<const Mtx*>(lhs), &lhs_scratch);
    const Mtx* resolved_rhs = resolve_replacement(reinterpret_cast<const Mtx*>(rhs), &rhs_scratch);
    if (resolved_lhs == reinterpret_cast<const Mtx*>(lhs) &&
        resolved_rhs == reinterpret_cast<const Mtx*>(rhs))
    {
        return false;
    }

    MTXConcat(*resolved_lhs, *resolved_rhs, out);
    return true;
}

void begin_presentation(float step) {
    begin_frame(step);
    if (!is_enabled()) {
        return;
    }

    interpolate_replacements();

    if (s_presentationDepth > 0) {
        s_presentationDepth++;
        return;
    }
    if (!camera_apply_presentation()) {
        return;
    }

    s_presentationDepth = 1;
    callbacks_run();
}

void end_presentation() {
    if (s_presentationDepth == 0) {
        return;
    }
    s_presentationDepth--;
    if (s_presentationDepth > 0) {
        return;
    }

    camera_restore_presentation();
}

bool is_presentation_active() {
    return s_presentationDepth > 0;
}

void add_interpolation_callback(InterpolationCallBack pCallBack, void* pUserWork) {
    if (!is_enabled() || is_presentation_active() || !game_clock::is_sim_frame()) {
        return;
    }
    if (pCallBack == nullptr) {
        return;
    }

    s_interpolationCallBackWork.push_back({pCallBack, pUserWork});
}

}  // namespace dusk::interp
