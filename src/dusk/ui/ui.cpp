#include "ui.hpp"

#include <RmlUi/Core.h>
#include <RmlUi_Platform_SDL.h>
#include <SDL3/SDL_filesystem.h>

#include <aurora/rmlui.hpp>

#include <chrono>
#include <cmath>
#include <filesystem>

namespace dusk::ui {
namespace {

using Clock = std::chrono::steady_clock;

constexpr auto k_repeatStartDelay = std::chrono::milliseconds{500};
constexpr auto k_repeatRate = std::chrono::milliseconds{55};
constexpr float k_stickPressThreshold = 0.55f;
constexpr float k_stickReleaseThreshold = 0.18f;

bool s_initialized = false;
bool s_active = false;
bool s_controllerPrimary = false;
bool s_awaitStickReturnX = false;
bool s_awaitStickReturnY = false;
Rml::Input::KeyIdentifier sLatestControllerKey = Rml::Input::KI_UNKNOWN;
Clock::time_point sNextRepeatTime = {};

std::filesystem::path resource_path(const char* filename) {
    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr) {
        return std::filesystem::path("res") / filename;
    }
    return std::filesystem::path(basePath) / "res" / filename;
}

void load_font(const char* filename, bool fallback = false) {
    Rml::LoadFontFace(resource_path(filename).string(), fallback);
}

Rml::Context* context() {
    return aurora::rmlui::get_context();
}

bool send_key_down(Rml::Input::KeyIdentifier key) {
    if (key == Rml::Input::KI_UNKNOWN || context() == nullptr) {
        return false;
    }

    context()->ProcessKeyDown(key, RmlSDL::GetKeyModifierState());
    return true;
}

void send_controller_key(Rml::Input::KeyIdentifier key) {
    if (!send_key_down(key)) {
        return;
    }

    s_controllerPrimary = true;
    sLatestControllerKey = key;
    sNextRepeatTime = Clock::now() + k_repeatStartDelay;
}

Rml::Input::KeyIdentifier key_from_gamepad_button(Uint8 button) {
    switch (button) {
    case SDL_GAMEPAD_BUTTON_SOUTH:
    case SDL_GAMEPAD_BUTTON_START:
        return key_for_menu_action(MenuAction::Accept);
    case SDL_GAMEPAD_BUTTON_WEST:
        return key_for_menu_action(MenuAction::Back);
    case SDL_GAMEPAD_BUTTON_BACK:
        return key_for_menu_action(MenuAction::Toggle);
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
        return key_for_menu_action(MenuAction::TabLeft);
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
        return key_for_menu_action(MenuAction::TabRight);
    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        return Rml::Input::KI_UP;
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        return Rml::Input::KI_DOWN;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        return Rml::Input::KI_LEFT;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        return Rml::Input::KI_RIGHT;
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

Rml::Input::KeyIdentifier key_from_gamepad_axis(const SDL_GamepadAxisEvent& event,
                                                float axisValue) {
    switch (event.axis) {
    case SDL_GAMEPAD_AXIS_LEFTX:
        return axisValue < 0.0f ? Rml::Input::KI_LEFT : Rml::Input::KI_RIGHT;
    case SDL_GAMEPAD_AXIS_LEFTY:
        return axisValue < 0.0f ? Rml::Input::KI_UP : Rml::Input::KI_DOWN;
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

}  // namespace

bool initialize() {
    if (s_initialized) {
        return true;
    }
    if (!aurora::rmlui::is_initialized()) {
        return false;
    }

    load_font("Inter-Regular.ttf");
    load_font("Inter-Bold.ttf");
    load_font("NotoMono-Regular.ttf", true);

    s_initialized = true;
    return true;
}

void shutdown() {
    s_active = false;
    s_initialized = false;
    sLatestControllerKey = Rml::Input::KI_UNKNOWN;
}

bool is_active() {
    return s_active;
}

void set_active(bool active) {
    s_active = active;
    if (!active) {
        sLatestControllerKey = Rml::Input::KI_UNKNOWN;
    }
}

Rml::Input::KeyIdentifier key_for_menu_action(MenuAction action) {
    switch (action) {
    case MenuAction::Accept:
        return Rml::Input::KI_RETURN;
    case MenuAction::Back:
        return Rml::Input::KI_F15;
    case MenuAction::Toggle:
        return Rml::Input::KI_ESCAPE;
    case MenuAction::TabLeft:
        return Rml::Input::KI_F16;
    case MenuAction::TabRight:
        return Rml::Input::KI_F17;
    case MenuAction::None:
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

MenuAction menu_action_from_key(Rml::Input::KeyIdentifier key) {
    if (key == key_for_menu_action(MenuAction::Accept)) {
        return MenuAction::Accept;
    }
    if (key == key_for_menu_action(MenuAction::Back)) {
        return MenuAction::Back;
    }
    if (key == key_for_menu_action(MenuAction::Toggle)) {
        return MenuAction::Toggle;
    }
    if (key == key_for_menu_action(MenuAction::TabLeft)) {
        return MenuAction::TabLeft;
    }
    if (key == key_for_menu_action(MenuAction::TabRight)) {
        return MenuAction::TabRight;
    }
    return MenuAction::None;
}

void handle_event(const SDL_Event& event) {
    if (!s_active || context() == nullptr) {
        return;
    }

    switch (event.type) {
    case SDL_EVENT_MOUSE_MOTION:
        if (!s_controllerPrimary || event.motion.xrel != 0.0f || event.motion.yrel != 0.0f) {
            s_controllerPrimary = false;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_WHEEL:
        s_controllerPrimary = false;
        break;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
        send_controller_key(key_from_gamepad_button(event.gbutton.button));
        break;
    case SDL_EVENT_GAMEPAD_BUTTON_UP: {
        const Rml::Input::KeyIdentifier key = key_from_gamepad_button(event.gbutton.button);
        if (key == sLatestControllerKey) {
            sLatestControllerKey = Rml::Input::KI_UNKNOWN;
        }
        break;
    }
    case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
        if (event.gaxis.axis != SDL_GAMEPAD_AXIS_LEFTX &&
            event.gaxis.axis != SDL_GAMEPAD_AXIS_LEFTY)
        {
            break;
        }

        const float axisValue = static_cast<float>(event.gaxis.value) / 32768.0f;
        bool& awaitStickReturn =
            event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX ? s_awaitStickReturnX : s_awaitStickReturnY;

        if (std::abs(axisValue) > k_stickPressThreshold) {
            if (!awaitStickReturn) {
                awaitStickReturn = true;
                send_controller_key(key_from_gamepad_axis(event.gaxis, axisValue));
            }
            s_controllerPrimary = true;
        } else if (awaitStickReturn && std::abs(axisValue) < k_stickReleaseThreshold) {
            const Rml::Input::KeyIdentifier key = key_from_gamepad_axis(event.gaxis, axisValue);
            if (key == sLatestControllerKey) {
                sLatestControllerKey = Rml::Input::KI_UNKNOWN;
            }
            awaitStickReturn = false;
        }
        break;
    }
    default:
        break;
    }
}

void update() {
    if (!s_active || sLatestControllerKey == Rml::Input::KI_UNKNOWN || context() == nullptr) {
        return;
    }

    const auto currentTime = Clock::now();
    if (currentTime >= sNextRepeatTime) {
        send_key_down(sLatestControllerKey);
        sNextRepeatTime += k_repeatRate;
    }
}

}  // namespace dusk::ui
