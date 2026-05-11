#pragma once

namespace dusk::mouse {
void read();
void getAimDeltas(float& out_yaw, float& out_pitch);
}  // namespace dusk::mouse
