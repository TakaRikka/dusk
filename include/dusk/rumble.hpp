#pragma once

namespace dusk::rumble {

bool supportsIphoneRumble();
bool usingIphoneRumble();
void startMotor(int port);
void stopMotor(int port, bool hardStop);

}  // namespace dusk::rumble
