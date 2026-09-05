#pragma once

class camera_process_class;
class view_class;

#ifdef __cplusplus
namespace dusk::interp {

void record_camera(::camera_process_class* cam, int camera_id);
void interp_view(::view_class* view);

}  // namespace dusk::interp
#endif
