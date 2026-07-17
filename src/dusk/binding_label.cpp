#include "binding_label.hpp"

#include <SDL3/SDL_keyboard.h>
#include <dolphin/pad.h>

#include <vector>

namespace dusk::actions {
namespace {

struct SpecificButtonName {
    SDL_GamepadType type;
    const char* name;
};

struct ButtonNames {
    SDL_GamepadButton button;
    std::vector<SpecificButtonName> names;
};

// clang-format off
const std::vector<ButtonNames> kGamepadButtonNames = {
    { SDL_GAMEPAD_BUTTON_LEFT_STICK, {
        {SDL_GAMEPAD_TYPE_PS3, "L3"},
        {SDL_GAMEPAD_TYPE_PS4, "L3"},
        {SDL_GAMEPAD_TYPE_PS5, "L3"},
        {SDL_GAMEPAD_TYPE_XBOX360, "Left Stick"},
        {SDL_GAMEPAD_TYPE_XBOXONE, "Left Stick"},
        {SDL_GAMEPAD_TYPE_GAMECUBE, "Control Stick"},
    }},
    { SDL_GAMEPAD_BUTTON_RIGHT_STICK, {
        {SDL_GAMEPAD_TYPE_PS3, "R3"},
        {SDL_GAMEPAD_TYPE_PS4, "R3"},
        {SDL_GAMEPAD_TYPE_PS5, "R3"},
        {SDL_GAMEPAD_TYPE_XBOX360, "Right Stick"},
        {SDL_GAMEPAD_TYPE_XBOXONE, "Right Stick"},
        {SDL_GAMEPAD_TYPE_GAMECUBE, "C Stick"},
    }},
    { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, {
        {SDL_GAMEPAD_TYPE_PS3, "L1"},
        {SDL_GAMEPAD_TYPE_PS4, "L1"},
        {SDL_GAMEPAD_TYPE_PS5, "L1"},
        {SDL_GAMEPAD_TYPE_XBOX360, "LB"},
        {SDL_GAMEPAD_TYPE_XBOXONE, "LB"},
    }},
    { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, {
        {SDL_GAMEPAD_TYPE_PS3, "R1"},
        {SDL_GAMEPAD_TYPE_PS4, "R1"},
        {SDL_GAMEPAD_TYPE_PS5, "R1"},
        {SDL_GAMEPAD_TYPE_XBOX360, "RB"},
        {SDL_GAMEPAD_TYPE_XBOXONE, "RB"},
        {SDL_GAMEPAD_TYPE_GAMECUBE, "Z"},
    }},
    { SDL_GAMEPAD_BUTTON_BACK, {
        {SDL_GAMEPAD_TYPE_PS3, "Select"},
        {SDL_GAMEPAD_TYPE_PS4, "Share"},
        {SDL_GAMEPAD_TYPE_PS5, "Create"},
        {SDL_GAMEPAD_TYPE_XBOX360, "Back"},
        {SDL_GAMEPAD_TYPE_XBOXONE, "View"},
    }},
    { SDL_GAMEPAD_BUTTON_START, {
        {SDL_GAMEPAD_TYPE_PS3, "Start"},
        {SDL_GAMEPAD_TYPE_PS4, "Options"},
        {SDL_GAMEPAD_TYPE_PS5, "Options"},
        {SDL_GAMEPAD_TYPE_XBOX360, "Start"},
        {SDL_GAMEPAD_TYPE_XBOXONE, "Menu"},
        {SDL_GAMEPAD_TYPE_GAMECUBE, "Start/Pause"},
    }},
};
// clang-format on

}  // namespace

std::string format_gamepad_button_label(SDL_Gamepad* gamepad, const SDL_GamepadButton button) {
    if (gamepad != nullptr) {
        switch (SDL_GetGamepadButtonLabel(gamepad, button)) {
        case SDL_GAMEPAD_BUTTON_LABEL_A:
            return "A";
        case SDL_GAMEPAD_BUTTON_LABEL_B:
            return "B";
        case SDL_GAMEPAD_BUTTON_LABEL_X:
            return "X";
        case SDL_GAMEPAD_BUTTON_LABEL_Y:
            return "Y";
        case SDL_GAMEPAD_BUTTON_LABEL_CROSS:
            return "Cross";
        case SDL_GAMEPAD_BUTTON_LABEL_CIRCLE:
            return "Circle";
        case SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE:
            return "Triangle";
        case SDL_GAMEPAD_BUTTON_LABEL_SQUARE:
            return "Square";
        default:
            break;
        }
    }

    const SDL_GamepadType type =
        gamepad != nullptr ? SDL_GetGamepadType(gamepad) : SDL_GAMEPAD_TYPE_UNKNOWN;
    for (const auto& buttonNames : kGamepadButtonNames) {
        if (buttonNames.button != button) {
            continue;
        }
        for (const auto& name : buttonNames.names) {
            if (name.type == type) {
                return name.name;
            }
        }
        break;
    }

    switch (button) {
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        return "D-pad left";
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        return "D-pad right";
    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        return "D-pad up";
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        return "D-pad down";
    default:
        break;
    }

    if (const char* name = PADGetNativeButtonName(static_cast<u32>(button))) {
        return name;
    }
    return "Unknown";
}

std::string format_gamepad_axis_label(SDL_Gamepad* gamepad, const SDL_GamepadAxis axis) {
    const SDL_GamepadType type =
        gamepad != nullptr ? SDL_GetGamepadType(gamepad) : SDL_GAMEPAD_TYPE_UNKNOWN;
    switch (axis) {
    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
        switch (type) {
        case SDL_GAMEPAD_TYPE_PS3:
        case SDL_GAMEPAD_TYPE_PS4:
        case SDL_GAMEPAD_TYPE_PS5:
            return "L2";
        case SDL_GAMEPAD_TYPE_XBOX360:
        case SDL_GAMEPAD_TYPE_XBOXONE:
            return "LT";
        case SDL_GAMEPAD_TYPE_GAMECUBE:
            return "L";
        default:
            return "Left Trigger";
        }
    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
        switch (type) {
        case SDL_GAMEPAD_TYPE_PS3:
        case SDL_GAMEPAD_TYPE_PS4:
        case SDL_GAMEPAD_TYPE_PS5:
            return "R2";
        case SDL_GAMEPAD_TYPE_XBOX360:
        case SDL_GAMEPAD_TYPE_XBOXONE:
            return "RT";
        case SDL_GAMEPAD_TYPE_GAMECUBE:
            return "R";
        default:
            return "Right Trigger";
        }
    default:
        return "Unknown";
    }
}

std::string format_binding_label(const Binding& binding, SDL_Gamepad* gamepad) {
    switch (binding.kind) {
    case BindingKind::Keyboard: {
        const char* name = SDL_GetScancodeName(binding.scancode);
        if (name == nullptr || name[0] == '\0') {
            return "Unknown";
        }
        return name;
    }
    case BindingKind::GamepadButton:
        return format_gamepad_button_label(gamepad, binding.gamepad_button);
    case BindingKind::GamepadAxis:
        return format_gamepad_axis_label(gamepad, binding.gamepad_axis);
    case BindingKind::Unbound:
    default:
        return "Not Bound";
    }
}

}  // namespace dusk::actions
