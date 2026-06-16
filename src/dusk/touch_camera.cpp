#include "dusk/touch_camera.h"

namespace dusk::touch_camera {
namespace {
float s_yaw_dp = 0.0f;
float s_pitch_dp = 0.0f;
}  // namespace

void add_delta(float yaw_dp, float pitch_dp) noexcept {
    s_yaw_dp += yaw_dp;
    s_pitch_dp += pitch_dp;
}

bool consume_delta(float& yaw_dp, float& pitch_dp) noexcept {
    yaw_dp = s_yaw_dp;
    pitch_dp = s_pitch_dp;
    clear();
    return yaw_dp != 0.0f || pitch_dp != 0.0f;
}

void clear() noexcept {
    s_yaw_dp = 0.0f;
    s_pitch_dp = 0.0f;
}

}  // namespace dusk::touch_camera
