#include "dusk/gyro.h"
#include "dusk/ui/ui.hpp"
#include "d/actor/d_a_alink.h"

#include <aurora/lib/window.hpp>
#include <SDL3/SDL_mouse.h>
#include <cmath>

namespace dusk::gyro {
namespace {
constexpr s32   kRollgoalTableMaxOffset = 6500;
constexpr float kGyroEmaAlphaMin = 0.05f;
constexpr float kGyroEmaAlphaMax = 1.0f;
// Smooth gravity separately so the yaw/roll blend doesn't twitch with raw accel noise.
constexpr float kGravityEmaAlpha = 0.1f;
constexpr float kMinGravityProjection = 0.2f;
// Let roll contribute more strongly as the pad approaches an upright posture.
constexpr float kRollAimBoostMax = 2.0f;
constexpr float kMousePixelToRad = 0.0025f;

bool  s_sensor_enabled        = false;
bool  s_accel_enabled         = false;
bool  s_was_aiming            = false;
bool  s_have_gravity_baseline = false;
bool  s_mouse_enabled         = false;
bool  s_mouse_relative        = false;
float s_smooth_gx             = 0.0f;
float s_smooth_gy             = 0.0f;
float s_smooth_gz             = 0.0f;
float s_gravity_y             = 0.0f;
float s_gravity_z             = 0.0f;
float s_baseline_gravity_y    = 0.0f;
float s_baseline_gravity_z    = 0.0f;
float s_yaw_rad               = 0.0f;
float s_pitch_rad             = 0.0f;
float s_roll_rad              = 0.0f;
s32   s_rollgoal_ax           = 0;
s32   s_rollgoal_az           = 0;

void reset_filter_state() {
    s_smooth_gx = s_smooth_gy = s_smooth_gz = 0.0f;
    s_gravity_y = s_gravity_z = 0.0f;
    s_baseline_gravity_y = s_baseline_gravity_z = 0.0f;
    s_was_aiming = false;
    s_have_gravity_baseline = false;
    s_mouse_enabled = false;
    s_yaw_rad = s_pitch_rad = s_roll_rad = 0.0f;
    s_rollgoal_ax = s_rollgoal_az = 0;
}

float apply_deadband(float v, float deadband_rad_s) {
    if (v > -deadband_rad_s && v < deadband_rad_s) {
        return 0.0f;
    }
    return v;
}

void disable_pad_sensors() {
    if (s_sensor_enabled) {
        PADSetSensorEnabled(PAD_CHAN0, PAD_SENSOR_GYRO, FALSE);
        s_sensor_enabled = false;
    }
    if (s_accel_enabled) {
        PADSetSensorEnabled(PAD_CHAN0, PAD_SENSOR_ACCEL, FALSE);
        s_accel_enabled = false;
    }
}
}  // namespace

bool s_sensor_keep_alive = false;
bool get_sensor_keep_alive() { return s_sensor_keep_alive; }
void set_sensor_keep_alive(bool value) { s_sensor_keep_alive = value; }

bool rollgoal_gyro_enabled() {
    return getSettings().game.enableGyroRollgoal && getSettings().game.gyroMode.getValue() != GyroMode::Mouse;
}

bool queryGyroAimContext() {
    if (!static_cast<bool>(getSettings().game.enableGyroAim)) {
        return false;
    }

    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link == nullptr) {
        return false;
    }

    return link->checkGyroAimContext() && dComIfGp_checkCameraAttentionStatus(link->field_0x317c, 0x10);
}

void read(float dt) {
    const bool aim_active = queryGyroAimContext();
    const bool aim_just_started = aim_active && !s_was_aiming;
    const bool aim_just_ended = !aim_active && s_was_aiming;
    s_was_aiming = aim_active;

    const bool mouse_mode = getSettings().game.gyroMode.getValue() == GyroMode::Mouse;
    const bool mouse_gyro_active = !ui::any_document_visible() && mouse_mode && (aim_active || s_sensor_keep_alive);
    SDL_Window* window = aurora::window::get_sdl_window();
    if (window != nullptr && mouse_gyro_active != s_mouse_relative &&
        SDL_SetWindowRelativeMouseMode(window, mouse_gyro_active))
    {
        s_mouse_relative = mouse_gyro_active;
    }

    if (mouse_gyro_active && !s_mouse_enabled && window != nullptr) {
        const AuroraWindowSize sz = aurora::window::get_window_size();
        const float cx = static_cast<float>(sz.width) * 0.5f;
        const float cy = static_cast<float>(sz.height) * 0.5f;
        SDL_WarpMouseInWindow(window, cx, cy);
        float discard_x = 0.0f;
        float discard_y = 0.0f;
        SDL_GetRelativeMouseState(&discard_x, &discard_y);
    }
    s_mouse_enabled = mouse_gyro_active;

    if (!s_sensor_keep_alive && !aim_active) {
        disable_pad_sensors();
        reset_filter_state();
        return;
    }

    if (aim_just_started || aim_just_ended) {
        s_gravity_y = s_gravity_z = 0.0f;
        s_baseline_gravity_y = s_baseline_gravity_z = 0.0f;
        s_have_gravity_baseline = false;
    }

    if (mouse_mode && !mouse_gyro_active) {
        s_pitch_rad = 0.0f;
        s_yaw_rad = 0.0f;
        s_roll_rad = 0.0f;
        return;
    }

    if (mouse_mode) {
        disable_pad_sensors();

        float mx_rel = 0.0f;
        float my_rel = 0.0f;
        SDL_GetRelativeMouseState(&mx_rel, &my_rel);
        // Convert pixels to radians
        s_pitch_rad = my_rel * kMousePixelToRad * getSettings().game.gyroSensitivityX;
        s_yaw_rad = -mx_rel * kMousePixelToRad * getSettings().game.gyroSensitivityY;
        s_roll_rad = 0.0f;

        s_pitch_rad = getSettings().game.gyroInvertPitch ? -s_pitch_rad : s_pitch_rad;
        s_yaw_rad = getSettings().game.gyroInvertYaw ? -s_yaw_rad : s_yaw_rad;
        s_yaw_rad = getSettings().game.enableMirrorMode ? -s_yaw_rad : s_yaw_rad;

        return;
    }

    if (!s_sensor_enabled) {
        if (!PADHasSensor(PAD_CHAN0, PAD_SENSOR_GYRO)) {
            return;
        }
        if (!PADSetSensorEnabled(PAD_CHAN0, PAD_SENSOR_GYRO, TRUE)) {
            return;
        }
        s_sensor_enabled = true;
    }

    if (!s_accel_enabled && PADHasSensor(PAD_CHAN0, PAD_SENSOR_ACCEL) &&
        PADSetSensorEnabled(PAD_CHAN0, PAD_SENSOR_ACCEL, TRUE))
    {
        // We only need accel for the gravity-aware yaw/roll mix.
        s_accel_enabled = true;
    }

    f32 gyro[3];
    if (!PADGetSensorData(PAD_CHAN0, PAD_SENSOR_GYRO, gyro, 3)) {
        return;
    }

    const float smooth_alpha = kGyroEmaAlphaMax + getSettings().game.gyroSmoothing * (kGyroEmaAlphaMin - kGyroEmaAlphaMax);
    const float deadband = getSettings().game.gyroDeadband;

    s_smooth_gx += smooth_alpha * (gyro[0] - s_smooth_gx);
    s_smooth_gy += smooth_alpha * (gyro[1] - s_smooth_gy);
    s_smooth_gz += smooth_alpha * (gyro[2] - s_smooth_gz);

    const float pitch_rate = apply_deadband(s_smooth_gx, deadband);
    const float yaw_rate = apply_deadband(s_smooth_gy, deadband);
    const float roll_rate = apply_deadband(s_smooth_gz, deadband);

    s_pitch_rad = -pitch_rate * dt * getSettings().game.gyroSensitivityX;
    s_roll_rad  = roll_rate * dt * getSettings().game.gyroSensitivityX; // GYRO NOTE: Exposing Z sensitivity seems unusual, so I'm just using X

    float horizontal_rate = yaw_rate;
    if (aim_active && s_accel_enabled) {
        f32 accel[3];
        if (PADGetSensorData(PAD_CHAN0, PAD_SENSOR_ACCEL, accel, 3)) {
            if (!s_have_gravity_baseline) {
                s_gravity_y = accel[1];
                s_gravity_z = accel[2];
            } else {
                s_gravity_y += kGravityEmaAlpha * (accel[1] - s_gravity_y);
                s_gravity_z += kGravityEmaAlpha * (accel[2] - s_gravity_z);
            }

            // Compare the current gravity projection against the gravity vector from
            // aim start so the user's resting hold angle becomes the neutral baseline.
            const float gravity_yz_len = std::sqrt((s_gravity_y * s_gravity_y) + (s_gravity_z * s_gravity_z));
            if (gravity_yz_len >= kMinGravityProjection) {
                const float current_gravity_y = s_gravity_y / gravity_yz_len;
                const float current_gravity_z = s_gravity_z / gravity_yz_len;

                if (!s_have_gravity_baseline) {
                    s_baseline_gravity_y = current_gravity_y;
                    s_baseline_gravity_z = current_gravity_z;
                    s_have_gravity_baseline = true;
                }

                const float yaw_weight =
                    (s_baseline_gravity_y * current_gravity_y) + (s_baseline_gravity_z * current_gravity_z);
                const float roll_weight =
                    (s_baseline_gravity_y * current_gravity_z) - (s_baseline_gravity_z * current_gravity_y);
                const float roll_mix = std::fabs(roll_weight);
                const float roll_boost = 1.0f + (roll_mix * (kRollAimBoostMax - 1.0f));
                horizontal_rate = (yaw_rate * yaw_weight) + (roll_rate * roll_weight * roll_boost);
            }
        }
    }

    s_yaw_rad = horizontal_rate * dt * getSettings().game.gyroSensitivityY;

    s_pitch_rad = getSettings().game.gyroInvertPitch ? -s_pitch_rad : s_pitch_rad;
    s_yaw_rad = getSettings().game.gyroInvertYaw ? -s_yaw_rad : s_yaw_rad;
    s_yaw_rad = getSettings().game.enableMirrorMode ? -s_yaw_rad : s_yaw_rad;
}

void getAimDeltas(float& out_yaw, float& out_pitch) {
    out_yaw = s_yaw_rad;
    out_pitch = s_pitch_rad;
}

void rollgoalTick(bool play_active, s16 camera_yaw) {
    if (!play_active) {
        reset_filter_state();
        return;
    }

    float pitch_rad = -s_pitch_rad * getSettings().game.gyroSensitivityRollgoal;
    float roll_rad = s_roll_rad * getSettings().game.gyroSensitivityRollgoal;
    roll_rad = getSettings().game.enableMirrorMode ? -roll_rad : roll_rad;

    s_rollgoal_az += cM_rad2s(roll_rad);
    cXyz in(roll_rad, 0.0f, pitch_rad);
    cXyz out;
    cMtx_YrotS(*calc_mtx, -camera_yaw);
    MtxPosition(&in, &out);

    s_rollgoal_ax += cM_rad2s(out.z);
    s_rollgoal_az += cM_rad2s(out.x);
    s_rollgoal_ax = std::clamp(s_rollgoal_ax, -kRollgoalTableMaxOffset, kRollgoalTableMaxOffset);
    s_rollgoal_az = std::clamp(s_rollgoal_az, -kRollgoalTableMaxOffset, kRollgoalTableMaxOffset);
}

void rollgoalTableOffset(s16& out_ax, s16& out_az) {
    out_ax = static_cast<s16>(s_rollgoal_ax);
    out_az = static_cast<s16>(s_rollgoal_az);
}
}  // namespace dusk::gyro
