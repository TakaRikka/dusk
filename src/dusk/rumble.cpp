#include "dusk/rumble.hpp"

#include "dusk/iphone_haptics.hpp"
#include "dusk/settings.h"

#include <algorithm>
#include <pad.h>

namespace dusk::rumble {
namespace {

float iphone_intensity() {
    return std::clamp(getSettings().game.iphoneRumbleIntensity.getValue(), 0.0f, 1.0f);
}

}  // namespace

bool supportsIphoneRumble() {
    return iphone_haptics::isAvailable();
}

bool usingIphoneRumble() {
    return getSettings().game.useIphoneRumble.getValue() && supportsIphoneRumble();
}

void startMotor(int port) {
    if (usingIphoneRumble()) {
        iphone_haptics::start(iphone_intensity());
        return;
    }

    iphone_haptics::stop();
    PADControlMotor(port, PAD_MOTOR_RUMBLE);
}

void stopMotor(int port, bool hardStop) {
    iphone_haptics::stop();
    if (usingIphoneRumble()) {
        return;
    }

    PADControlMotor(port, hardStop ? PAD_MOTOR_STOP_HARD : PAD_MOTOR_STOP);
}

}  // namespace dusk::rumble
