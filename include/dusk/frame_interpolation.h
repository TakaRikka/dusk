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

void set_ui_tick_pending(bool value);
bool get_ui_tick_pending();

void open_child(const void* key, int32_t id);
void close_child();
void record_camera(::camera_process_class* cam, int camera_id);
void record_final_mtx_raw(const Mtx* dest, const Mtx src);
void record_final_mtx_raw_tagged(const Mtx* dest, const Mtx src, uint64_t stable_tag);

bool lookup_replacement(const void* source, Mtx out);
bool lookup_concat_replacement(const void* lhs, const void* rhs, Mtx out);

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

uint64_t alloc_simple_shadow_pair_base();
}  // namespace frame_interp
}  // namespace dusk
#endif

#endif
