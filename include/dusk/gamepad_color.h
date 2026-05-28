#pragma once

#ifndef GAMEPAD_COLOR_H
#define GAMEPAD_COLOR_H

namespace dusk::input {
    void handleGamepadColor();
    bool pad_has_led(int port) noexcept;
}

#endif