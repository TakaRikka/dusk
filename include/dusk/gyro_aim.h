#ifndef DUSK_GYRO_AIM_H
#define DUSK_GYRO_AIM_H

namespace dusk::gyro_aim {
void read(float dt, bool context_active);
void consumeAimDeltas(float& out_yaw_rad, float& out_pitch_rad);
bool queryGyroAimItemContext();
}  // namespace dusk::gyro_aim

#endif
