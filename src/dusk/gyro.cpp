#include "dusk/gyro.h"
#include "d/actor/d_a_alink.h"

namespace dusk::gyro {
namespace {
constexpr s32   kRollgoalTableMaxOffset = 12000;
constexpr float kGyroEmaAlphaMin = 0.05f;
constexpr float kGyroEmaAlphaMax = 1.0f;

bool  s_sensor_enabled        = false;
float s_smooth_gx             = 0.0f;
float s_smooth_gy             = 0.0f;
float s_smooth_gz             = 0.0f;
float s_yaw_rad               = 0.0f;
float s_pitch_rad             = 0.0f;
float s_roll_rad              = 0.0f;
s32   s_rollgoal_ax           = 0;
s32   s_rollgoal_az           = 0;

void reset_filter_state() {
    s_smooth_gx = s_smooth_gy = s_smooth_gz = 0.0f;
    s_yaw_rad = s_pitch_rad = s_roll_rad = 0.0f;
    s_rollgoal_ax = s_rollgoal_az = 0;
}

float apply_deadband(float v, float deadband_rad_s) {
    if (v > -deadband_rad_s && v < deadband_rad_s) {
        return 0.0f;
    }
    return v;
}
}  // namespace

bool s_sensor_keep_alive = false;
bool get_sensor_keep_alive() { return s_sensor_keep_alive; }
void set_sensor_keep_alive(bool value) { s_sensor_keep_alive = value; }

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

void read(float dt) {
    if (!s_sensor_keep_alive && !(dusk::getSettings().game.enableGyroAim && queryGyroAimItemContext())) {
        if (s_sensor_enabled) {
            PADSetSensorEnabled(PAD_CHAN0, PAD_SENSOR_GYRO, FALSE);
            s_sensor_enabled = false;
        }
        reset_filter_state();
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

    f32 gyro[3];
    if (!PADGetSensorData(PAD_CHAN0, PAD_SENSOR_GYRO, gyro, 3)) {
        return;
    }

    const float smooth_alpha = kGyroEmaAlphaMax + dusk::getSettings().game.gyroSmoothing * (kGyroEmaAlphaMin - kGyroEmaAlphaMax);
    const float deadband = dusk::getSettings().game.gyroDeadband;

    s_smooth_gx += smooth_alpha * (gyro[0] - s_smooth_gx);
    s_smooth_gy += smooth_alpha * (gyro[1] - s_smooth_gy);
    s_smooth_gz += smooth_alpha * (gyro[2] - s_smooth_gz);

    s_pitch_rad = -apply_deadband(s_smooth_gx, deadband) * dt * dusk::getSettings().game.gyroSensitivityX;
    s_yaw_rad   = apply_deadband(s_smooth_gy, deadband) * dt * dusk::getSettings().game.gyroSensitivityY;
    s_roll_rad  = apply_deadband(s_smooth_gz, deadband) * dt * dusk::getSettings().game.gyroSensitivityX; // GYRO NOTE: Exposing Z sensitivity seems unusual, so I'm just using X

    s_pitch_rad = dusk::getSettings().game.gyroInvertPitch ? -s_pitch_rad : s_pitch_rad;
    s_yaw_rad = dusk::getSettings().game.gyroInvertYaw ? -s_yaw_rad : s_yaw_rad;
    s_yaw_rad = dusk::getSettings().game.enableMirrorMode ? -s_yaw_rad : s_yaw_rad;
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

    float pitch_rad = -s_pitch_rad * dusk::getSettings().game.gyroSensitivityRollgoal;
    float roll_rad = s_roll_rad * dusk::getSettings().game.gyroSensitivityRollgoal;
    roll_rad = dusk::getSettings().game.enableMirrorMode ? -roll_rad : roll_rad;

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
