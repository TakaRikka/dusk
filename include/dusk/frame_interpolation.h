#pragma once

#include <dolphin/mtx.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class camera_process_class;
class view_class;

#ifdef __cplusplus
namespace dusk {
namespace frame_interp {

void ensure_initialized();

void begin_record();
void end_record();
void begin_sim_tick();
uint64_t sim_tick_seq();
void begin_frame(bool enabled, bool is_sim_frame, float step);
void interpolate();
float get_interpolation_step();

void request_presentation_sync();
bool presentation_sync_active();

bool is_enabled();

// TODO: These should be phased out as UI is progressively updated to use game_clock
void set_ui_tick_pending(bool value);
bool get_ui_tick_pending();

bool is_sim_frame();

void record_camera(::camera_process_class* cam, int camera_id);
void interp_view(::view_class* view);
void record_final_mtx(Mtx m, const void *key);
void record_final_mtx(Mtx m);

bool lookup_replacement(const void* key, Mtx out);
bool lookup_concat_replacement(const void* lhs, const void* rhs, Mtx out);

typedef void (*InterpolationCallBack)(bool isSimFrame, void* pUserWork);
// call on a sim tick, will get called during presentation
void add_interpolation_callback(InterpolationCallBack pCallBack, void* pUserWork);

void begin_presentation_camera();
void end_presentation_camera();

}  // namespace frame_interp
}  // namespace dusk
#endif
