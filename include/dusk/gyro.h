#ifndef DUSK_GYRO_H
#define DUSK_GYRO_H

namespace dusk::gyro {
void read(float dt);
void getAimDeltas(float& out_yaw, float& out_pitch);
bool queryGyroAimContext();

void rollgoalTick(bool play_active, s16 camera_yaw);
void rollgoalTableOffset(s16& out_ax, s16& out_az);

extern bool s_sensor_keep_alive;
bool get_sensor_keep_alive();
void set_sensor_keep_alive(bool value);
bool rollgoal_gyro_enabled();
}  // namespace dusk::gyro

#endif
