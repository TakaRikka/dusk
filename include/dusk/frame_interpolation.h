#ifndef DUSK_FRAME_INTERP_H
#define DUSK_FRAME_INTERP_H

#include <dolphin/mtx.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct cXyz;
class camera_process_class;

#ifdef __cplusplus
namespace dusk {
namespace frame_interp {

void ensure_initialized();

void begin_record_camera();
void begin_record();
void end_record();
void interpolate(float step);
float get_interpolation_step();

void notify_presentation_frame();
void request_presentation_sync();
bool presentation_sync_active();

void notify_sim_tick_complete();
uint32_t begin_presentation_ui_pass();
uint32_t get_presentation_ui_advance_ticks();
void end_presentation_ui_pass();

void open_child(const void* key, int32_t id);
void close_child();
void record_camera(::camera_process_class* cam, int camera_id);
void record_final_mtx_raw(const Mtx* dest, const Mtx src);

bool lookup_replacement(const void* source, Mtx out);
bool lookup_concat_replacement(const void* lhs, const void* rhs, Mtx out);

void camera_eye_from_view_mtx(MtxP view_mtx, cXyz* o_eye);
bool build_star_view(Mtx o_view, Mtx o_cam_billboard_base, cXyz* o_anchor_eye, float* o_fovy);

}  // namespace frame_interp
}  // namespace dusk
#endif

#endif
