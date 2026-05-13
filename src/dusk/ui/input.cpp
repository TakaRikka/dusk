#include "input.hpp"

#include "ui.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_touch.h>
#include <aurora/rmlui.hpp>
#include <dolphin/pad.h>

#include <algorithm>
#include <array>

#include "dusk/action_bindings.h"

namespace dusk::ui::input {
namespace {

constexpr double kGamepadRepeatInitialDelay = 0.32;
constexpr double kGamepadRepeatStartInterval = 0.12;
constexpr double kGamepadRepeatMinInterval = 0.045;
constexpr double kGamepadRepeatRampDuration = 1.0;
constexpr double kGamepadMenuChordGraceDuration = 0.12;
constexpr Sint16 kGamepadAxisPressThreshold = 16384;
constexpr Sint16 kGamepadAxisReleaseThreshold = 12000;
constexpr int kGamepadAxisDirectionCount = SDL_GAMEPAD_AXIS_COUNT * 2;
constexpr int kMenuTapFingerCount = 3;
constexpr float kMenuTapMoveThreshold = 12.0f;
constexpr double kMenuTapMaxDownSpan = 0.18;
constexpr double kMenuTapMaxDuration = 0.55;

struct GamepadRepeatState {
    Rml::Input::KeyIdentifier key = Rml::Input::KI_UNKNOWN;
    double pressedAt = 0.0;
    double nextRepeatAt = 0.0;
    bool held = false;
    bool repeatable = false;
    bool pending = false;
};

struct TouchTapFinger {
    SDL_FingerID id = 0;
    Rml::Vector2f startPosition;
    bool active = false;
};

struct TouchTapState {
    std::array<TouchTapFinger, kMenuTapFingerCount> fingers;
    int activeCount = 0;
    double firstDownAt = 0.0;
    bool candidate = false;
    bool failed = false;
};

bool sPadInputBlocked = false;
std::array<GamepadRepeatState, SDL_GAMEPAD_BUTTON_COUNT> sGamepadButtonRepeats;
std::array<GamepadRepeatState, kGamepadAxisDirectionCount> sGamepadAxisRepeats;
std::array<u32, PAD_MAX_CONTROLLERS> sPadHoldMasks;
std::array<bool, PAD_MAX_CONTROLLERS> sMenuChordConsumed;
TouchTapState sTouchMenuTap;

double now_seconds() noexcept {
    return static_cast<double>(SDL_GetTicksNS()) / 1000000000.0;
}

bool is_menu_chord_part(PADButton button) noexcept {
    return button == PAD_TRIGGER_R || button == PAD_BUTTON_START;
}

bool has_menu_chord_part_held(u32 port) noexcept {
    if (port >= sPadHoldMasks.size()) {
        return false;
    }

    const u32 held = sPadHoldMasks[port];
    return (held & (PAD_TRIGGER_R | PAD_BUTTON_START)) != 0;
}

const char* controller_change_type(Uint32 eventType) noexcept {
    switch (eventType) {
    case SDL_EVENT_GAMEPAD_ADDED:
        return "added";
    case SDL_EVENT_GAMEPAD_REMOVED:
        return "removed";
    case SDL_EVENT_GAMEPAD_REMAPPED:
        return "remapped";
    default:
        return nullptr;
    }
}

void dispatch_controller_change_event(const SDL_Event& event) noexcept {
    const char* type = controller_change_type(event.type);
    if (type == nullptr) {
        return;
    }

    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return;
    }
    auto* root = context->GetRootElement();
    if (root == nullptr) {
        return;
    }

    Rml::Dictionary parameters;
    parameters["type"] = Rml::String(type);
    parameters["which"] = static_cast<int>(event.gdevice.which);
    root->DispatchEvent("controllerchange", parameters);
}

PADButton pad_button_from_axis(PADAxis axis) noexcept {
    switch (axis) {
    case PAD_AXIS_TRIGGER_R:
        return PAD_TRIGGER_R;
    case PAD_AXIS_TRIGGER_L:
        return PAD_TRIGGER_L;
    default:
        return 0;
    }
}

void set_pad_button_held(u32 port, PADButton button, bool held) noexcept {
    if (port >= sPadHoldMasks.size() || button == 0) {
        return;
    }

    if (held) {
        sPadHoldMasks[port] |= button;
    } else {
        sPadHoldMasks[port] &= ~button;
    }
}

bool is_menu_chord(u32 port) noexcept {
    if (port >= sPadHoldMasks.size()) {
        return false;
    }

    const u32 held = sPadHoldMasks[port];
    return (held & PAD_TRIGGER_R) != 0 && (held & PAD_BUTTON_START) != 0;
}

bool any_menu_chord() noexcept {
    return std::any_of(sPadHoldMasks.begin(), sPadHoldMasks.end(),
        [](u32 held) { return (held & PAD_TRIGGER_R) != 0 && (held & PAD_BUTTON_START) != 0; });
}

Rml::Input::KeyIdentifier map_pad_button(PADButton button) noexcept {
    switch (button) {
    case PAD_BUTTON_UP:
        return Rml::Input::KI_UP;
    case PAD_BUTTON_DOWN:
        return Rml::Input::KI_DOWN;
    case PAD_BUTTON_LEFT:
        return Rml::Input::KI_LEFT;
    case PAD_BUTTON_RIGHT:
        return Rml::Input::KI_RIGHT;
    case PAD_BUTTON_B:
        return Rml::Input::KI_ESCAPE;
    case PAD_BUTTON_A:
        return Rml::Input::KI_RETURN;
    case PAD_TRIGGER_R:
        return Rml::Input::KI_NEXT;
    case PAD_TRIGGER_L:
        return Rml::Input::KI_PRIOR;
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

Rml::Input::KeyIdentifier map_pad_axis(PADAxis axis) noexcept {
    switch (axis) {
    case PAD_AXIS_LEFT_X_POS:
        return Rml::Input::KI_RIGHT;
    case PAD_AXIS_LEFT_X_NEG:
        return Rml::Input::KI_LEFT;
    case PAD_AXIS_LEFT_Y_POS:
        return Rml::Input::KI_UP;
    case PAD_AXIS_LEFT_Y_NEG:
        return Rml::Input::KI_DOWN;
    case PAD_AXIS_TRIGGER_R:
        return Rml::Input::KI_NEXT;
    case PAD_AXIS_TRIGGER_L:
        return Rml::Input::KI_PRIOR;
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

Rml::Input::KeyIdentifier map_raw_gamepad_button(SDL_GamepadButton button) noexcept {
    switch (button) {
    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        return Rml::Input::KI_UP;
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        return Rml::Input::KI_DOWN;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        return Rml::Input::KI_LEFT;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        return Rml::Input::KI_RIGHT;
    case SDL_GAMEPAD_BUTTON_EAST:
        return Rml::Input::KI_ESCAPE;
    case SDL_GAMEPAD_BUTTON_SOUTH:
        return Rml::Input::KI_RETURN;
    case SDL_GAMEPAD_BUTTON_BACK:
        if (isActionBound(ActionBinds::OPEN_DUSKLIGHT_MENU, PAD_CHAN0)) {
            return Rml::Input::KI_UNKNOWN;
        }
        return Rml::Input::KI_F1;
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
        return Rml::Input::KI_NEXT;
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
        return Rml::Input::KI_PRIOR;
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

Rml::Input::KeyIdentifier map_raw_button_alias(SDL_GamepadButton button) noexcept {
    switch (button) {
    case SDL_GAMEPAD_BUTTON_BACK:
        if (isActionBound(ActionBinds::OPEN_DUSKLIGHT_MENU, PAD_CHAN0)) {
            return Rml::Input::KI_UNKNOWN;
        }
        return Rml::Input::KI_F1;
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
        return Rml::Input::KI_NEXT;
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
        return Rml::Input::KI_PRIOR;
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

Rml::Input::KeyIdentifier map_raw_gamepad_axis(SDL_GamepadAxis axis, PADAxisSign sign) noexcept {
    switch (axis) {
    case SDL_GAMEPAD_AXIS_LEFTX:
        return sign == AXIS_SIGN_POSITIVE ? Rml::Input::KI_RIGHT : Rml::Input::KI_LEFT;
    case SDL_GAMEPAD_AXIS_LEFTY:
        return sign == AXIS_SIGN_NEGATIVE ? Rml::Input::KI_UP : Rml::Input::KI_DOWN;
    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
        return sign == AXIS_SIGN_POSITIVE ? Rml::Input::KI_NEXT : Rml::Input::KI_UNKNOWN;
    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
        return sign == AXIS_SIGN_POSITIVE ? Rml::Input::KI_PRIOR : Rml::Input::KI_UNKNOWN;
    default:
        return Rml::Input::KI_UNKNOWN;
    }
}

bool find_event_port(SDL_JoystickID instance, u32& port) noexcept {
    for (u32 candidate = 0; candidate < PAD_MAX_CONTROLLERS; ++candidate) {
        const s32 index = PADGetIndexForPort(candidate);
        if (index < 0) {
            continue;
        }

        SDL_Gamepad* gamepad = PADGetSDLGamepadForIndex(static_cast<u32>(index));
        if (gamepad == nullptr) {
            continue;
        }

        SDL_Joystick* joystick = SDL_GetGamepadJoystick(gamepad);
        if (joystick != nullptr && SDL_GetJoystickID(joystick) == instance) {
            port = candidate;
            return true;
        }
    }
    return false;
}

bool find_mapped_pad_button(u32 port, SDL_GamepadButton nativeButton, PADButton& button) noexcept {
    u32 buttonCount = 0;
    PADButtonMapping* buttons = PADGetButtonMappings(port, &buttonCount);
    if (buttons != nullptr) {
        for (u32 i = 0; i < buttonCount; ++i) {
            if (buttons[i].nativeButton == static_cast<u32>(nativeButton)) {
                button = buttons[i].padButton;
                return true;
            }
        }
    }

    u32 axisCount = 0;
    PADAxisMapping* axes = PADGetAxisMappings(port, &axisCount);
    if (axes != nullptr) {
        for (u32 i = 0; i < axisCount; ++i) {
            if (axes[i].nativeButton == nativeButton) {
                button = pad_button_from_axis(axes[i].padAxis);
                return button != 0;
            }
        }
    }

    return false;
}

bool find_mapped_pad_axis(
    u32 port, SDL_GamepadAxis nativeAxis, PADAxisSign sign, PADAxis& axis) noexcept {
    u32 buttonCount = 0;
    PADGetButtonMappings(port, &buttonCount);

    u32 axisCount = 0;
    PADAxisMapping* axes = PADGetAxisMappings(port, &axisCount);
    if (axes == nullptr) {
        return false;
    }

    for (u32 i = 0; i < axisCount; ++i) {
        const PADSignedNativeAxis mappedAxis = axes[i].nativeAxis;
        if (mappedAxis.nativeAxis == nativeAxis && mappedAxis.sign == sign) {
            axis = axes[i].padAxis;
            return true;
        }
    }

    return false;
}

bool find_event_pad_button(
    const SDL_GamepadButtonEvent& event, u32& port, PADButton& button) noexcept {
    return find_event_port(event.which, port) &&
           find_mapped_pad_button(port, static_cast<SDL_GamepadButton>(event.button), button);
}

Rml::Input::KeyIdentifier map_gamepad_button(const SDL_GamepadButtonEvent& event) noexcept {
    const auto nativeButton = static_cast<SDL_GamepadButton>(event.button);
    u32 port = 0;
    bool foundEventPort = find_event_port(event.which, port);
    if (foundEventPort) {
        int openMenuButton = getActionBindButton(ActionBinds::OPEN_DUSKLIGHT_MENU, port);
        if (openMenuButton != PAD_NATIVE_BUTTON_INVALID && openMenuButton == nativeButton) {
            return Rml::Input::KI_F1;
        }
    }

    if (nativeButton == SDL_GAMEPAD_BUTTON_BACK && !isActionBound(ActionBinds::OPEN_DUSKLIGHT_MENU, port)) {
        return Rml::Input::KI_F1;
    }

    if (!foundEventPort) {
        return map_raw_gamepad_button(nativeButton);
    }

    PADButton button = 0;
    if (find_mapped_pad_button(port, nativeButton, button)) {
        const auto key = map_pad_button(button);
        return key == Rml::Input::KI_UNKNOWN ? map_raw_button_alias(nativeButton) : key;
    }

    return map_raw_button_alias(nativeButton);
}

Rml::Input::KeyIdentifier map_gamepad_axis(
    const SDL_GamepadAxisEvent& event, PADAxisSign sign) noexcept {
    u32 port = 0;
    if (!find_event_port(event.which, port)) {
        return map_raw_gamepad_axis(static_cast<SDL_GamepadAxis>(event.axis), sign);
    }

    PADAxis axis = 0;
    if (find_mapped_pad_axis(port, static_cast<SDL_GamepadAxis>(event.axis), sign, axis)) {
        return map_pad_axis(axis);
    }

    return Rml::Input::KI_UNKNOWN;
}

bool is_repeatable_key(Rml::Input::KeyIdentifier key) noexcept {
    switch (key) {
    case Rml::Input::KI_UP:
    case Rml::Input::KI_DOWN:
    case Rml::Input::KI_LEFT:
    case Rml::Input::KI_RIGHT:
    case Rml::Input::KI_NEXT:
    case Rml::Input::KI_PRIOR:
        return true;
    default:
        return false;
    }
}

double repeat_interval(double heldFor) noexcept {
    const double ramp = std::clamp(heldFor / kGamepadRepeatRampDuration, 0.0, 1.0);
    return kGamepadRepeatStartInterval +
           (kGamepadRepeatMinInterval - kGamepadRepeatStartInterval) * ramp;
}

GamepadRepeatState* button_repeat_state(SDL_GamepadButton button) noexcept {
    const auto index = static_cast<int>(button);
    if (index < 0 || index >= static_cast<int>(sGamepadButtonRepeats.size())) {
        return nullptr;
    }
    return &sGamepadButtonRepeats[index];
}

GamepadRepeatState* axis_repeat_state(SDL_GamepadAxis axis, PADAxisSign sign) noexcept {
    const auto axisIndex = static_cast<int>(axis);
    if (axisIndex < 0 || axisIndex >= SDL_GAMEPAD_AXIS_COUNT) {
        return nullptr;
    }

    const int directionOffset = sign == AXIS_SIGN_POSITIVE ? 0 : 1;
    return &sGamepadAxisRepeats[axisIndex * 2 + directionOffset];
}

void clear_gamepad_repeats() noexcept {
    for (auto& repeat : sGamepadButtonRepeats) {
        repeat = {};
    }
    for (auto& repeat : sGamepadAxisRepeats) {
        repeat = {};
    }
    sPadHoldMasks.fill(0);
    sMenuChordConsumed.fill(false);
}

void reset_touch_menu_tap() noexcept {
    sTouchMenuTap = {};
}

Rml::Vector2f touch_position(const SDL_TouchFingerEvent& event, Rml::Context& context) noexcept {
    const auto dimensions = context.GetDimensions();
    return {
        event.x * static_cast<float>(dimensions.x),
        event.y * static_cast<float>(dimensions.y),
    };
}

TouchTapFinger* find_touch_finger(SDL_FingerID id) noexcept {
    for (auto& finger : sTouchMenuTap.fingers) {
        if (finger.active && finger.id == id) {
            return &finger;
        }
    }
    return nullptr;
}

TouchTapFinger* find_free_touch_finger() noexcept {
    for (auto& finger : sTouchMenuTap.fingers) {
        if (!finger.active) {
            return &finger;
        }
    }
    return nullptr;
}

bool touch_moved_too_far(
    const TouchTapFinger& finger, Rml::Vector2f position, Rml::Context& context) noexcept {
    const Rml::Vector2f delta = position - finger.startPosition;
    const float threshold =
        kMenuTapMoveThreshold * std::max(context.GetDensityIndependentPixelRatio(), 1.0f);
    return delta.SquaredMagnitude() > threshold * threshold;
}

void dispatch_menu_key(Rml::Context& context) noexcept {
    context.ProcessMouseLeave();
    context.ProcessKeyDown(Rml::Input::KI_F1, 0);
    context.ProcessKeyUp(Rml::Input::KI_F1, 0);
}

bool handle_touch_menu_tap(Rml::Context& context, const SDL_Event& event) noexcept {
    switch (event.type) {
    case SDL_EVENT_FINGER_DOWN: {
        const double now = now_seconds();
        if (sTouchMenuTap.activeCount == 0) {
            reset_touch_menu_tap();
            sTouchMenuTap.firstDownAt = now;
        }

        if (sTouchMenuTap.candidate || sTouchMenuTap.activeCount >= kMenuTapFingerCount ||
            find_touch_finger(event.tfinger.fingerID) != nullptr)
        {
            sTouchMenuTap.failed = true;
            return false;
        }

        auto* finger = find_free_touch_finger();
        if (finger == nullptr) {
            sTouchMenuTap.failed = true;
            return false;
        }

        *finger = TouchTapFinger{
            .id = event.tfinger.fingerID,
            .startPosition = touch_position(event.tfinger, context),
            .active = true,
        };
        sTouchMenuTap.activeCount++;

        if (now - sTouchMenuTap.firstDownAt > kMenuTapMaxDownSpan) {
            sTouchMenuTap.failed = true;
        }
        if (sTouchMenuTap.activeCount == kMenuTapFingerCount) {
            sTouchMenuTap.candidate = true;
        }
        return false;
    }
    case SDL_EVENT_FINGER_MOTION: {
        auto* finger = find_touch_finger(event.tfinger.fingerID);
        if (finger == nullptr) {
            return false;
        }
        if (touch_moved_too_far(*finger, touch_position(event.tfinger, context), context)) {
            sTouchMenuTap.failed = true;
        }
        return false;
    }
    case SDL_EVENT_FINGER_UP: {
        auto* finger = find_touch_finger(event.tfinger.fingerID);
        if (finger == nullptr) {
            return false;
        }

        const double now = now_seconds();
        if (!sTouchMenuTap.candidate ||
            touch_moved_too_far(*finger, touch_position(event.tfinger, context), context))
        {
            sTouchMenuTap.failed = true;
        }

        *finger = {};
        sTouchMenuTap.activeCount--;
        if (sTouchMenuTap.activeCount > 0) {
            return false;
        }

        const bool shouldDispatch = sTouchMenuTap.candidate && !sTouchMenuTap.failed &&
                                    now - sTouchMenuTap.firstDownAt <= kMenuTapMaxDuration;
        reset_touch_menu_tap();
        if (shouldDispatch) {
            dispatch_menu_key(context);
            return true;
        }
        return false;
    }
    case SDL_EVENT_FINGER_CANCELED:
        reset_touch_menu_tap();
        return false;
    default:
        return false;
    }
}

void begin_gamepad_key(GamepadRepeatState& repeat, Rml::Input::KeyIdentifier key) noexcept {
    if (repeat.held) {
        return;
    }

    const double now = now_seconds();
    repeat.key = key;
    repeat.pressedAt = now;
    repeat.held = true;
    repeat.repeatable = is_repeatable_key(key);
    repeat.nextRepeatAt = repeat.repeatable ? now + kGamepadRepeatInitialDelay : 0.0;
    repeat.pending = false;
}

void begin_pending_gamepad_key(GamepadRepeatState& repeat, Rml::Input::KeyIdentifier key) noexcept {
    if (repeat.held) {
        return;
    }

    const double now = now_seconds();
    repeat.key = key;
    repeat.pressedAt = now;
    repeat.held = true;
    repeat.repeatable = is_repeatable_key(key);
    repeat.nextRepeatAt = 0.0;
    repeat.pending = true;
}

void consume_menu_chord(u32 port, Rml::Context& context) noexcept {
    if (port < sMenuChordConsumed.size()) {
        sMenuChordConsumed[port] = true;
    }

    auto cancel_next = [&context](GamepadRepeatState& repeat) {
        if (!repeat.held || repeat.key != Rml::Input::KI_NEXT) {
            return;
        }
        if (!repeat.pending) {
            context.ProcessKeyUp(repeat.key, 0);
        }
        repeat = {};
    };

    for (auto& repeat : sGamepadButtonRepeats) {
        cancel_next(repeat);
    }
    for (auto& repeat : sGamepadAxisRepeats) {
        cancel_next(repeat);
    }
}

void update_menu_chord_release(u32 port) noexcept {
    if (port >= sMenuChordConsumed.size() || has_menu_chord_part_held(port)) {
        return;
    }

    sMenuChordConsumed[port] = false;
}

bool should_defer_menu_chord_part(PADButton button, Rml::Input::KeyIdentifier key) noexcept {
    return button == PAD_TRIGGER_R && key == Rml::Input::KI_NEXT;
}

void process_axis_direction(
    Rml::Context& context, const SDL_GamepadAxisEvent& event, PADAxisSign sign) noexcept {
    GamepadRepeatState* repeat = axis_repeat_state(static_cast<SDL_GamepadAxis>(event.axis), sign);
    if (repeat == nullptr) {
        return;
    }

    const bool active = sign == AXIS_SIGN_POSITIVE ? event.value >= kGamepadAxisPressThreshold :
                                                     event.value <= -kGamepadAxisPressThreshold;
    const bool released = sign == AXIS_SIGN_POSITIVE ? event.value <= kGamepadAxisReleaseThreshold :
                                                       event.value >= -kGamepadAxisReleaseThreshold;

    u32 port = 0;
    PADAxis padAxis = 0;
    const bool hasPadAxis =
        find_event_port(event.which, port) &&
        find_mapped_pad_axis(port, static_cast<SDL_GamepadAxis>(event.axis), sign, padAxis);
    const PADButton heldPadButton = hasPadAxis ? pad_button_from_axis(padAxis) : 0;

    if (repeat->held) {
        if (released) {
            if (!repeat->pending) {
                context.ProcessKeyUp(repeat->key, 0);
            }
            set_pad_button_held(port, heldPadButton, false);
            *repeat = {};
            update_menu_chord_release(port);
        }
        return;
    }

    if (!active) {
        return;
    }

    set_pad_button_held(port, heldPadButton, true);
    const bool chorded = heldPadButton == PAD_TRIGGER_R && is_menu_chord(port) &&
                         (port >= sMenuChordConsumed.size() || !sMenuChordConsumed[port]);
    if (chorded) {
        consume_menu_chord(port, context);
    }
    const auto key = chorded && !isActionBound(ActionBinds::OPEN_DUSKLIGHT_MENU, port) ? Rml::Input::KI_F1 : map_gamepad_axis(event, sign);
    if (key == Rml::Input::KI_UNKNOWN) {
        return;
    }

    if (!chorded && should_defer_menu_chord_part(heldPadButton, key)) {
        begin_pending_gamepad_key(*repeat, key);
        return;
    }

    begin_gamepad_key(*repeat, key);
    context.ProcessMouseLeave();
    context.ProcessKeyDown(key, 0);
}

}  // namespace

void sync_input_block() noexcept {
    const bool shouldBlock = any_document_visible();
    if (sPadInputBlocked == shouldBlock) {
        return;
    }

    PADBlockInput(shouldBlock);
    sPadInputBlocked = shouldBlock;
}

void release_input_block() noexcept {
    if (!sPadInputBlocked) {
        return;
    }

    PADBlockInput(false);
    sPadInputBlocked = false;
}

void reset_input_state() noexcept {
    clear_gamepad_repeats();
    reset_touch_menu_tap();
}

void handle_event(const SDL_Event& event) noexcept {
    if (event.type == SDL_EVENT_GAMEPAD_REMOVED || event.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
        reset_input_state();
        sync_input_block();
        if (event.type != SDL_EVENT_GAMEPAD_REMOVED) {
            return;
        }
    }
    dispatch_controller_change_event(event);

    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return;
    }

    if (event.type == SDL_EVENT_FINGER_DOWN || event.type == SDL_EVENT_FINGER_MOTION ||
        event.type == SDL_EVENT_FINGER_UP || event.type == SDL_EVENT_FINGER_CANCELED)
    {
        if (handle_touch_menu_tap(*context, event)) {
            sync_input_block();
        }
        return;
    }

    if (event.type != SDL_EVENT_GAMEPAD_BUTTON_DOWN && event.type != SDL_EVENT_GAMEPAD_BUTTON_UP &&
        event.type != SDL_EVENT_GAMEPAD_AXIS_MOTION)
    {
        return;
    }

    if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
        process_axis_direction(*context, event.gaxis, AXIS_SIGN_POSITIVE);
        process_axis_direction(*context, event.gaxis, AXIS_SIGN_NEGATIVE);
        sync_input_block();
        return;
    }

    auto* repeat = button_repeat_state(static_cast<SDL_GamepadButton>(event.gbutton.button));
    u32 port = 0;
    PADButton button = 0;
    const bool hasPadButton = find_event_pad_button(event.gbutton, port, button);
    if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        set_pad_button_held(port, button, true);
        const bool chorded = hasPadButton && is_menu_chord_part(button) && is_menu_chord(port);
        if (chorded) {
            consume_menu_chord(port, *context);
        }
        const auto key = chorded && !isActionBound(ActionBinds::OPEN_DUSKLIGHT_MENU, port) ? Rml::Input::KI_F1 : map_gamepad_button(event.gbutton);
        if (key != Rml::Input::KI_UNKNOWN) {
            bool deferred = false;
            if (repeat != nullptr) {
                if (!chorded && should_defer_menu_chord_part(button, key)) {
                    begin_pending_gamepad_key(*repeat, key);
                    deferred = true;
                } else {
                    begin_gamepad_key(*repeat, key);
                }
            }
            if (!deferred) {
                context->ProcessMouseLeave();
                context->ProcessKeyDown(key, 0);
            }
        }
    } else {
        const auto key = repeat != nullptr && repeat->held ? repeat->key : Rml::Input::KI_UNKNOWN;
        const bool wasPending = repeat != nullptr && repeat->pending;
        set_pad_button_held(port, button, false);
        update_menu_chord_release(port);
        if (key != Rml::Input::KI_UNKNOWN) {
            if (repeat != nullptr) {
                *repeat = {};
            }
            if (!wasPending) {
                context->ProcessKeyUp(key, 0);
            }
        }
    }
    sync_input_block();
}

void update_input() noexcept {
    auto* context = aurora::rmlui::get_context();
    if (context != nullptr) {
        const double now = now_seconds();
        auto process_repeats = [context, now](auto& repeats) {
            for (auto& repeat : repeats) {
                if (!repeat.held) {
                    continue;
                }

                if (repeat.pending) {
                    if (now < repeat.pressedAt + kGamepadMenuChordGraceDuration) {
                        continue;
                    }

                    repeat.pending = false;
                    repeat.pressedAt = now;
                    repeat.nextRepeatAt =
                        repeat.repeatable ? now + kGamepadRepeatInitialDelay : 0.0;
                    context->ProcessMouseLeave();
                    context->ProcessKeyDown(repeat.key, 0);
                    continue;
                }

                if (!repeat.repeatable || now < repeat.nextRepeatAt ||
                    (repeat.key == Rml::Input::KI_NEXT && any_menu_chord()))
                {
                    continue;
                }

                context->ProcessKeyDown(repeat.key, 0);
                const double heldFor = now - repeat.pressedAt;
                repeat.nextRepeatAt = now + repeat_interval(heldFor);
            }
        };
        process_repeats(sGamepadButtonRepeats);
        process_repeats(sGamepadAxisRepeats);
    } else {
        reset_input_state();
    }
}

}  // namespace dusk::ui
