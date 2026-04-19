#ifndef DUSK_FRAME_INTERP_H
#define DUSK_FRAME_INTERP_H

#include <dolphin/mtx.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class camera_process_class;

#ifdef __cplusplus
namespace dusk {
namespace frame_interp {

void ensure_initialized();

void begin_record();
void end_record();
void interpolate(float step);
float get_interpolation_step();

void request_presentation_sync();
bool presentation_sync_active();

bool is_enabled();

// TODO: These should be phased out as UI is progressively updated to use game_clock
void set_ui_tick_pending(bool value);
bool get_ui_tick_pending();

void record_camera(::camera_process_class* cam, int camera_id);
void record_final_mtx(Mtx m, const void *key);
void record_final_mtx(Mtx m);

bool lookup_replacement(const void* key, Mtx out);
bool lookup_concat_replacement(const void* lhs, const void* rhs, Mtx out);

typedef void (*InterpolationCallBack)(void* pUserWork);
void reset_interpolation_callbacks();
// call on a sim tick, will get called during presentation
void add_interpolation_callback(InterpolationCallBack pCallBack, void* pUserWork);

void begin_presentation_camera();
void end_presentation_camera();

struct PresentationCameraScope {
    PresentationCameraScope() { begin_presentation_camera(); }
    ~PresentationCameraScope() { end_presentation_camera(); }
    PresentationCameraScope(const PresentationCameraScope&) = delete;
    PresentationCameraScope& operator=(const PresentationCameraScope&) = delete;
    PresentationCameraScope(PresentationCameraScope&&) = delete;
    PresentationCameraScope& operator=(PresentationCameraScope&&) = delete;
};

}  // namespace frame_interp
}  // namespace dusk
#endif

#endif
