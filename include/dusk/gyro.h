#ifndef DUSK_GYRO_H
#define DUSK_GYRO_H

namespace dusk::gyro {
void read(float dt);
void consumeAimDeltas(float& out_yaw_rad, float& out_pitch_rad);
bool queryGyroAimItemContext();

void rollgoalTick(bool play_active, s16 camera_yaw);
void rollgoalTableOffset(s16& out_add_x, s16& out_add_z);

extern bool s_sensor_keep_alive;
bool get_sensor_keep_alive();
void set_sensor_keep_alive(bool value);
}  // namespace dusk::gyro

#endif
