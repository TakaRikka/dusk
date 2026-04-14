#include "dusk/gyro_aim.h"

#include <SDL3/SDL.h>
#include "d/actor/d_a_alink.h"

namespace dusk::gyro_aim {
namespace {
// TODO: Make deadband and smoothing configurable
constexpr float kDeadbandRadS = 0.04f;
constexpr float kSmoothAlpha  = 0.35f;
bool  s_sensor_enabled        = false;
float s_smooth_gx             = 0.0f;
float s_smooth_gy             = 0.0f;
float s_pending_yaw_rad       = 0.0f;
float s_pending_pitch_rad     = 0.0f;

void reset_filter_state() {
    s_smooth_gx = s_smooth_gy = 0.0f;
    s_pending_yaw_rad = s_pending_pitch_rad = 0.0f;
}

float apply_deadband(float v) {
    if (v > -kDeadbandRadS && v < kDeadbandRadS) {
        return 0.0f;
    }
    return v;
}
}  // namespace

void read(float dt, bool context_active) {
    if (!context_active || !static_cast<bool>(dusk::getSettings().game.enableGyroAim)) {
        SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(0);
        if (pad != nullptr && s_sensor_enabled) {
            SDL_SetGamepadSensorEnabled(pad, SDL_SENSOR_GYRO, false);
            s_sensor_enabled = false;
        }
        reset_filter_state();
        return;
    }

    SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(0);
    if (pad == nullptr || !SDL_GamepadHasSensor(pad, SDL_SENSOR_GYRO)) {
        return;
    }

    if (!s_sensor_enabled) {
        if (!SDL_SetGamepadSensorEnabled(pad, SDL_SENSOR_GYRO, true)) {
            return;
        }
        s_sensor_enabled = true;
        reset_filter_state();
    }

    float gyro[3];
    if (!SDL_GetGamepadSensorData(pad, SDL_SENSOR_GYRO, gyro, 3)) {
        return;
    }

    s_smooth_gx += kSmoothAlpha * (gyro[0] - s_smooth_gx);
    s_smooth_gy += kSmoothAlpha * (gyro[1] - s_smooth_gy);
    float yaw_rate = apply_deadband(s_smooth_gy);
    float pitch_rate = apply_deadband(s_smooth_gx);

    s_pending_yaw_rad += yaw_rate * dt * dusk::getSettings().game.gyroAimSensitivityX;
    s_pending_pitch_rad += pitch_rate * dt * dusk::getSettings().game.gyroAimSensitivityY;
}

void consumeAimDeltas(float& out_yaw_rad, float& out_pitch_rad) {
    out_yaw_rad = s_pending_yaw_rad;
    out_pitch_rad = s_pending_pitch_rad;
    s_pending_yaw_rad = 0.0f;
    s_pending_pitch_rad = 0.0f;
}

bool queryGyroAimItemContext() {
    if (!static_cast<bool>(dusk::getSettings().game.enableGyroAim)) {
        return false;
    }

    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link == nullptr) {
        return false;
    }

    return link->checkGyroAimItemContext() && dComIfGp_checkCameraAttentionStatus(link->field_0x317c, 0x10);
}
}  // namespace dusk::gyro_aim
