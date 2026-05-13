#include "controller_config.hpp"

#include "bool_button.hpp"
#include "button.hpp"
#include "pane.hpp"
#include "number_button.hpp"

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <fmt/format.h>

#include <array>
#include <string>
#include <utility>
#include <vector>

#include "dusk/action_bindings.h"
#include "dusk/config.hpp"

namespace dusk::ui {
namespace {

bool keyboard_active(int port) {
    u32 count = 0;
    return PADGetKeyButtonBindings(static_cast<u32>(port), &count) != nullptr;
}

Rml::String current_controller_name(int port) {
    const char* name = PADGetName(port);
    if (name != nullptr) {
        return name;
    }
    return keyboard_active(port) ? "Keyboard" : "None";
}

Rml::String controller_index_name(u32 index) {
    const char* name = PADGetNameForControllerIndex(index);
    if (name == nullptr) {
        return fmt::format("Controller {}", index + 1);
    }
    return name;
}

SDL_Gamepad* gamepad_for_port(int port) {
    const s32 index = PADGetIndexForPort(port);
    if (index < 0) {
        return nullptr;
    }
    return PADGetSDLGamepadForIndex(static_cast<u32>(index));
}

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

Rml::String native_axis_name(const PADAxisMapping& mapping, SDL_Gamepad* gamepad) {
    if (mapping.nativeAxis.nativeAxis != -1) {
        Rml::String value = PADGetNativeAxisName(mapping.nativeAxis);
        if (mapping.padAxis != PAD_AXIS_TRIGGER_L && mapping.padAxis != PAD_AXIS_TRIGGER_R) {
            value += mapping.nativeAxis.sign == AXIS_SIGN_POSITIVE ? "+" : "-";
        }
        return value;
    }

    if (mapping.nativeButton != -1) {
        return native_button_name(gamepad, static_cast<u32>(mapping.nativeButton));
    }

    return "Not bound";
}

bool is_dpad_button(PADButton button) {
    return button == PAD_BUTTON_UP || button == PAD_BUTTON_DOWN || button == PAD_BUTTON_LEFT ||
           button == PAD_BUTTON_RIGHT;
}

bool is_action_button(PADButton button) {
    return button == PAD_BUTTON_A || button == PAD_BUTTON_B || button == PAD_BUTTON_X ||
           button == PAD_BUTTON_Y || button == PAD_BUTTON_START || button == PAD_TRIGGER_Z;
}

bool input_neutral(int port) {
    if (port < 0) {
        return true;
    }
    return PADGetNativeButtonPressed(port) == -1 && PADGetNativeAxisPulled(port).nativeAxis == -1;
}

// A Keydown event with KI_ESCAPE may have been dispatched from the controller bindings,
// so instead poll the keyboard input directly for Escape-to-unbind
bool keyboard_escape_pressed() {
    int keyCount = 0;
    const bool* keys = SDL_GetKeyboardState(&keyCount);
    if (keys == nullptr || SDL_SCANCODE_ESCAPE >= keyCount || !keys[SDL_SCANCODE_ESCAPE]) {
        return false;
    }
    for (int i = 0; i < keyCount; ++i) {
        if (i != SDL_SCANCODE_ESCAPE && keys[i]) {
            return false;
        }
    }
    return true;
}

Rml::String keyboard_key_name(s32 scancode) {
    if (scancode == PAD_KEY_INVALID) {
        return "Not bound";
    }
    switch (scancode) {
    case PAD_KEY_MOUSE_LEFT:
        return "Mouse Left";
    case PAD_KEY_MOUSE_MIDDLE:
        return "Mouse Middle";
    case PAD_KEY_MOUSE_RIGHT:
        return "Mouse Right";
    case PAD_KEY_MOUSE_X1:
        return "Mouse X1";
    case PAD_KEY_MOUSE_X2:
        return "Mouse X2";
    default:
        break;
    }
    if (scancode < 0) {
        return "Unknown";
    }
    const char* name = SDL_GetScancodeName(static_cast<SDL_Scancode>(scancode));
    if (name == nullptr || name[0] == '\0') {
        return "Unknown";
    }
    return name;
}

bool keyboard_neutral() {
    int keyCount = 0;
    const bool* keys = SDL_GetKeyboardState(&keyCount);
    if (keys != nullptr) {
        for (int i = 0; i < keyCount; ++i) {
            if (keys[i]) {
                return false;
            }
        }
    }
    float x, y;
    if (SDL_GetMouseState(&x, &y) != 0) {
        return false;
    }
    return true;
}

s32 keyboard_key_pressed() {
    int keyCount = 0;
    const bool* keys = SDL_GetKeyboardState(&keyCount);
    if (keys != nullptr) {
        for (int i = 1; i < keyCount; ++i) {
            if (i == SDL_SCANCODE_ESCAPE) {
                continue;
            }
            if (keys[i]) {
                return static_cast<s32>(i);
            }
        }
    }
    float x, y;
    const auto mouseButtons = SDL_GetMouseState(&x, &y);
    for (int btn = 1; btn <= 5; ++btn) {
        if (mouseButtons & (1u << (btn - 1))) {
            return -(btn + 1);  // maps to PAD_KEY_MOUSE_LEFT (-2), etc.
        }
    }
    return PAD_KEY_INVALID;
}

u16 percent_to_raw(int percent) {
    return static_cast<u16>((static_cast<float>(percent) / 100.f) * 32767.f);
}

int deadzone_raw_to_percent(u16 raw) {
    return static_cast<int>((static_cast<float>(raw) * 100.f) / 32767.f + 0.5f);
}

int rumble_raw_to_percent(u16 raw) {
    return static_cast<int>((static_cast<float>(raw) / 32767.f) * 100.f + 0.5f);
}

}  // namespace

ControllerConfigWindow::ControllerConfigWindow(bool prelaunch) {
    if (prelaunch) {
        mSuppressNavFallback = true;
    }

    listen(
        Rml::EventId::Keydown,
        [this](Rml::Event& event) {
            if (capture_active() || mSuppressNavigationUntilNeutral) {
                event.StopPropagation();
            }
        },
        true);
    if (auto* context = mDocument != nullptr ? mDocument->GetContext() : nullptr) {
        if (auto* root = context->GetRootElement()) {
            mListeners.emplace_back(std::make_unique<ScopedEventListener>(
                root, "controllerchange", [this](Rml::Event&) { refresh_controller_page(); }));
        }
    }

    for (int port = PAD_CHAN0; port < PAD_CHANMAX; ++port) {
        add_tab(fmt::format("Port {}", port + 1), [this, port](Rml::Element* content) {
            if (mPendingPort != -1 && mPendingPort != port) {
                cancel_pending_binding();
            }
            build_port_tab(content, port);
        });
    }
}

void ControllerConfigWindow::hide(bool close) {
    stop_rumble_test();
    cancel_pending_binding();
    Window::hide(close);
}

void ControllerConfigWindow::update() {
    poll_pending_binding();
    Window::update();
}

void ControllerConfigWindow::build_port_tab(Rml::Element* content, int port) {
    stop_rumble_test();
    auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
    auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);
    mRightPane = &rightPane;
    mActivePort = port;

    auto addPageButton = [this, &leftPane, &rightPane, port](
                             Page page, Rml::String key, auto getValue, auto isDisabled) {
        leftPane.register_control(leftPane.add_select_button({
                                      .key = std::move(key),
                                      .getValue = std::move(getValue),
                                      .isDisabled = std::move(isDisabled),
                                  }),
            rightPane, [this, port, page](Pane& pane) {
                mPage = page;
                render_page(pane, port, page);
            });
    };

    addPageButton(Page::Controller, "Controller", [port] { return current_controller_name(port); }, [] { return false; });
    addPageButton(Page::Buttons, "Buttons", [] { return Rml::String(">"); }, [] { return false; });
    addPageButton(Page::Triggers, "Triggers", [] { return Rml::String(">"); }, [] { return false; });
    addPageButton(Page::Sticks, "Sticks", [] { return Rml::String(">"); }, [] { return false; });
    addPageButton(Page::Rumble, "Rumble", [] { return Rml::String(">"); }, [port] { return !PADSupportsRumbleIntensity(static_cast<u32>(port)); });
    addPageButton(Page::Actions, "Custom Action Bindings", [] {return Rml::String(">"); }, [] { return false; });

    leftPane.add_section("Options");
    leftPane.register_control(leftPane.add_child<BoolButton>(BoolButton::Props{
                                  .key = "Enable Dead Zones",
                                  .getValue =
                                      [port] {
                                          PADDeadZones* deadZones = PADGetDeadZones(port);
                                          return deadZones != nullptr && deadZones->useDeadzones;
                                      },
                                  .setValue =
                                      [port](bool value) {
                                          if (PADDeadZones* deadZones = PADGetDeadZones(port)) {
                                              deadZones->useDeadzones = value;
                                              PADSerializeMappings();
                                          }
                                      },
                                  .isDisabled = [port] { return PADGetDeadZones(port) == nullptr; },
                              }),
        rightPane, [](Pane& pane) {
            pane.add_text("Apply configured dead zones to the sticks and analog triggers.");
        });
    leftPane.register_control(leftPane.add_child<BoolButton>(BoolButton::Props{
                                  .key = "Emulate Triggers",
                                  .getValue =
                                      [port] {
                                          PADDeadZones* deadZones = PADGetDeadZones(port);
                                          return deadZones != nullptr && deadZones->emulateTriggers;
                                      },
                                  .setValue =
                                      [port](bool value) {
                                          if (PADDeadZones* deadZones = PADGetDeadZones(port)) {
                                              deadZones->emulateTriggers = value;
                                              PADSerializeMappings();
                                          }
                                      },
                                  .isDisabled = [port] { return PADGetDeadZones(port) == nullptr; },
                              }),
        rightPane, [](Pane& pane) {
            pane.add_text("Treat analog trigger movement as digital L and R button input.");
        });

    render_page(rightPane, port, mPage);
}

void ControllerConfigWindow::render_page(Pane& pane, int port, Page page) {
    pane.clear();

    switch (page) {
    case Page::Controller: {
        pane.add_button(
                {
                    .text = "None",
                .isSelected =
                    [port] { return PADGetIndexForPort(port) < 0 && !keyboard_active(port); },
            })
            .on_pressed([this, port] {
                mDoAud_seStartMenu(kSoundItemChange);
                cancel_pending_binding();
                PADClearPort(port);
                PADSetKeyboardActive(static_cast<u32>(port), FALSE);
                PADSerializeMappings();
                ClearAllActionBindings(port);
            });

        pane.add_button({
                            .text = "Keyboard",
                            .isSelected = [port] { return keyboard_active(port); },
                        })
            .on_pressed([this, port] {
                mDoAud_seStartMenu(kSoundItemChange);
                cancel_pending_binding();
                PADClearPort(port);
                PADSetKeyboardActive(static_cast<u32>(port), TRUE);
                PADSerializeMappings();
                ClearAllActionBindings(port);
            });

        const u32 controllerCount = PADCount();
        if (controllerCount == 0) {
            pane.add_text("No controllers detected");
            break;
        }

        for (u32 i = 0; i < controllerCount; ++i) {
            pane.add_button(
                    {
                        .text = controller_index_name(i),
                        .isSelected =
                            [port, i] { return PADGetIndexForPort(port) == static_cast<s32>(i); },
                    })
                .on_pressed([this, port, i] {
                    mDoAud_seStartMenu(kSoundItemChange);
                    cancel_pending_binding();
                    PADSetKeyboardActive(static_cast<u32>(port), FALSE);
                    PADSetPortForIndex(i, port);
                    PADSerializeMappings();
                    ClearAllActionBindings(port);
                });
        }
        break;
    }
    case Page::Buttons: {
        if (keyboard_active(port)) {
            auto addKeyButton = [&](PADButton button) {
                pane.add_select_button(
                        {
                            .key = PADGetButtonName(button),
                            .getValue =
                                [this, port, button] {
                                    if (mPendingKeyButton == static_cast<int>(button)) {
                                        return pending_key_label();
                                    }
                                    u32 count = 0;
                                    PADKeyButtonBinding* bindings =
                                        PADGetKeyButtonBindings(static_cast<u32>(port), &count);
                                    if (bindings == nullptr) {
                                        return Rml::String("Not bound");
                                    }
                                    for (u32 i = 0; i < PAD_BUTTON_COUNT; ++i) {
                                        if (bindings[i].padButton == button) {
                                            return keyboard_key_name(bindings[i].scancode);
                                        }
                                    }
                                    return Rml::String("Not bound");
                                },
                        })
                    .on_pressed([this, port, button] {
                        cancel_pending_binding();
                        mPendingPort = port;
                        mPendingBindingArmed = false;
                        mPendingKeyButton = static_cast<int>(button);
                    });
            };

            pane.add_section("Buttons");
            addKeyButton(PAD_BUTTON_A);
            addKeyButton(PAD_BUTTON_B);
            addKeyButton(PAD_BUTTON_X);
            addKeyButton(PAD_BUTTON_Y);
            addKeyButton(PAD_BUTTON_START);
            addKeyButton(PAD_TRIGGER_Z);

            pane.add_section("D-Pad");
            addKeyButton(PAD_BUTTON_UP);
            addKeyButton(PAD_BUTTON_DOWN);
            addKeyButton(PAD_BUTTON_LEFT);
            addKeyButton(PAD_BUTTON_RIGHT);
            break;
        }

        u32 buttonCount = 0;
        PADButtonMapping* mappings = PADGetButtonMappings(port, &buttonCount);
        if (mappings == nullptr) {
            pane.add_text("No controller selected");
            break;
        }

        SDL_Gamepad* gamepad = gamepad_for_port(port);
        pane.add_section("Buttons");
        for (u32 i = 0; i < buttonCount; ++i) {
            PADButtonMapping& mapping = mappings[i];
            if (!is_action_button(mapping.padButton)) {
                continue;
            }

            pane.add_select_button({
                                       .key = PADGetButtonName(mapping.padButton),
                                       .getValue =
                                           [this, &mapping, gamepad] {
                                               if (mPendingButtonMapping == &mapping) {
                                                   return pending_button_label();
                                               }
                                               return native_button_name(
                                                   gamepad, mapping.nativeButton);
                                           },
                                   })
                .on_pressed([this, port, &mapping] {
                    cancel_pending_binding();
                    mPendingPort = port;
                    mPendingBindingArmed = false;
                    mPendingButtonMapping = &mapping;
                });
        }

        pane.add_section("D-Pad");
        for (u32 i = 0; i < buttonCount; ++i) {
            PADButtonMapping& mapping = mappings[i];
            if (!is_dpad_button(mapping.padButton)) {
                continue;
            }

            pane.add_select_button({
                                       .key = PADGetButtonName(mapping.padButton),
                                       .getValue =
                                           [this, &mapping, gamepad] {
                                               if (mPendingButtonMapping == &mapping) {
                                                   return pending_button_label();
                                               }
                                               return native_button_name(
                                                   gamepad, mapping.nativeButton);
                                           },
                                   })
                .on_pressed([this, port, &mapping] {
                    cancel_pending_binding();
                    mPendingPort = port;
                    mPendingBindingArmed = false;
                    mPendingButtonMapping = &mapping;
                });
        }
        break;
    }
    case Page::Triggers: {
        if (keyboard_active(port)) {
            auto addKeyButton = [&](PADButton button) {
                pane.add_select_button(
                        {
                            .key = PADGetButtonName(button),
                            .getValue =
                                [this, port, button] {
                                    if (mPendingKeyButton == static_cast<int>(button)) {
                                        return pending_key_label();
                                    }
                                    u32 count = 0;
                                    PADKeyButtonBinding* bindings =
                                        PADGetKeyButtonBindings(static_cast<u32>(port), &count);
                                    if (bindings == nullptr) {
                                        return Rml::String("Not bound");
                                    }
                                    for (u32 i = 0; i < PAD_BUTTON_COUNT; ++i) {
                                        if (bindings[i].padButton == button) {
                                            return keyboard_key_name(bindings[i].scancode);
                                        }
                                    }
                                    return Rml::String("Not bound");
                                },
                        })
                    .on_pressed([this, port, button] {
                        cancel_pending_binding();
                        mPendingPort = port;
                        mPendingBindingArmed = false;
                        mPendingKeyButton = static_cast<int>(button);
                    });
            };

            auto addKeyAxis = [&](PADAxis axis) {
                pane.add_select_button(
                        {
                            .key = PADGetAxisName(axis),
                            .getValue =
                                [this, port, axis] {
                                    if (mPendingKeyAxis == static_cast<int>(axis)) {
                                        return pending_key_label();
                                    }
                                    u32 count = 0;
                                    PADKeyAxisBinding* bindings =
                                        PADGetKeyAxisBindings(static_cast<u32>(port), &count);
                                    if (bindings == nullptr) {
                                        return Rml::String("Not bound");
                                    }
                                    for (u32 i = 0; i < PAD_AXIS_COUNT; ++i) {
                                        if (bindings[i].padAxis == axis) {
                                            return keyboard_key_name(bindings[i].scancode);
                                        }
                                    }
                                    return Rml::String("Not bound");
                                },
                        })
                    .on_pressed([this, port, axis] {
                        cancel_pending_binding();
                        mPendingPort = port;
                        mPendingBindingArmed = false;
                        mPendingKeyAxis = static_cast<int>(axis);
                    });
            };

            pane.add_section("Analog");
            addKeyAxis(PAD_AXIS_TRIGGER_L);
            addKeyAxis(PAD_AXIS_TRIGGER_R);

            pane.add_section("Digital");
            addKeyButton(PAD_TRIGGER_L);
            addKeyButton(PAD_TRIGGER_R);
            break;
        }

        u32 axisCount = 0;
        PADAxisMapping* axes = PADGetAxisMappings(port, &axisCount);
        u32 buttonCount = 0;
        PADButtonMapping* buttons = PADGetButtonMappings(port, &buttonCount);
        if (axes == nullptr && buttons == nullptr) {
            pane.add_text("No controller selected");
            break;
        }

        SDL_Gamepad* gamepad = gamepad_for_port(port);
        pane.add_section("Analog");
        constexpr std::array<PADAxis, 2> kTriggerAxes = {PAD_AXIS_TRIGGER_L, PAD_AXIS_TRIGGER_R};
        if (axes != nullptr) {
            for (PADAxis axis : kTriggerAxes) {
                if (axis >= axisCount) {
                    continue;
                }
                PADAxisMapping& mapping = axes[axis];
                pane.add_select_button({
                                           .key = PADGetAxisName(mapping.padAxis),
                                           .getValue =
                                               [this, &mapping, gamepad] {
                                                   if (mPendingAxisMapping == &mapping) {
                                                       return pending_axis_label();
                                                   }
                                                   return native_axis_name(mapping, gamepad);
                                               },
                                       })
                    .on_pressed([this, port, &mapping] {
                        cancel_pending_binding();
                        mPendingPort = port;
                        mPendingBindingArmed = false;
                        mPendingAxisMapping = &mapping;
                    });
            }
        }

        pane.add_section("Digital");
        if (buttons != nullptr) {
            for (u32 i = 0; i < buttonCount; ++i) {
                PADButtonMapping& mapping = buttons[i];
                if (mapping.padButton != PAD_TRIGGER_L && mapping.padButton != PAD_TRIGGER_R) {
                    continue;
                }
                pane.add_select_button({
                                           .key = PADGetButtonName(mapping.padButton),
                                           .getValue =
                                               [this, &mapping, gamepad] {
                                                   if (mPendingButtonMapping == &mapping) {
                                                       return pending_button_label();
                                                   }
                                                   return native_button_name(
                                                       gamepad, mapping.nativeButton);
                                               },
                                       })
                    .on_pressed([this, port, &mapping] {
                        cancel_pending_binding();
                        mPendingPort = port;
                        mPendingBindingArmed = false;
                        mPendingButtonMapping = &mapping;
                    });
            }
        }

        if (PADDeadZones* deadZones = PADGetDeadZones(port)) {
            pane.add_section("Emulated Trigger Thresholds");
            pane.add_child<NumberButton>(NumberButton::Props{
                .key = "L Threshold",
                .getValue = [deadZones] { return deadzone_raw_to_percent(deadZones->leftTriggerActivationZone); },
                .setValue =
                    [deadZones](int value) {
                        deadZones->leftTriggerActivationZone = percent_to_raw(value);
                        PADSerializeMappings();
                    },
                .isDisabled = [deadZones] { return !deadZones->emulateTriggers; },
                .min = 0,
                .max = 100,
                .step = 1,
                .suffix = "%",
            });
            pane.add_child<NumberButton>(NumberButton::Props{
                .key = "R Threshold",
                .getValue = [deadZones] { return deadzone_raw_to_percent(deadZones->rightTriggerActivationZone); },
                .setValue =
                    [deadZones](int value) {
                        deadZones->rightTriggerActivationZone = percent_to_raw(value);
                        PADSerializeMappings();
                    },
                .isDisabled = [deadZones] { return !deadZones->emulateTriggers; },
                .min = 0,
                .max = 100,
                .step = 1,
                .suffix = "%",
            });
        }
        break;
    }
    case Page::Sticks: {
        if (keyboard_active(port)) {
            auto addKeyAxis = [&](PADAxis axis) {
                pane.add_select_button(
                        {
                            .key = PADGetAxisDirectionLabel(axis),
                            .getValue =
                                [this, port, axis] {
                                    if (mPendingKeyAxis == static_cast<int>(axis)) {
                                        return pending_key_label();
                                    }
                                    u32 count = 0;
                                    PADKeyAxisBinding* bindings =
                                        PADGetKeyAxisBindings(static_cast<u32>(port), &count);
                                    if (bindings == nullptr) {
                                        return Rml::String("Not bound");
                                    }
                                    for (u32 i = 0; i < PAD_AXIS_COUNT; ++i) {
                                        if (bindings[i].padAxis == axis) {
                                            return keyboard_key_name(bindings[i].scancode);
                                        }
                                    }
                                    return Rml::String("Not bound");
                                },
                        })
                    .on_pressed([this, port, axis] {
                        cancel_pending_binding();
                        mPendingPort = port;
                        mPendingBindingArmed = false;
                        mPendingKeyAxis = static_cast<int>(axis);
                    });
            };

            pane.add_section("Control Stick");
            addKeyAxis(PAD_AXIS_LEFT_Y_POS);
            addKeyAxis(PAD_AXIS_LEFT_Y_NEG);
            addKeyAxis(PAD_AXIS_LEFT_X_NEG);
            addKeyAxis(PAD_AXIS_LEFT_X_POS);

            pane.add_section("C Stick");
            addKeyAxis(PAD_AXIS_RIGHT_Y_POS);
            addKeyAxis(PAD_AXIS_RIGHT_Y_NEG);
            addKeyAxis(PAD_AXIS_RIGHT_X_NEG);
            addKeyAxis(PAD_AXIS_RIGHT_X_POS);
            break;
        }

        u32 axisCount = 0;
        PADAxisMapping* axes = PADGetAxisMappings(port, &axisCount);
        if (axes == nullptr) {
            pane.add_text("No controller selected");
            break;
        }

        SDL_Gamepad* gamepad = gamepad_for_port(port);
        auto addAxis = [&](PADAxis axis) {
            if (axis >= axisCount) {
                return;
            }
            PADAxisMapping& mapping = axes[axis];
            pane.add_select_button({
                                       .key = PADGetAxisDirectionLabel(mapping.padAxis),
                                       .getValue =
                                           [this, &mapping, gamepad] {
                                               if (mPendingAxisMapping == &mapping) {
                                                   return pending_axis_label();
                                               }
                                               return native_axis_name(mapping, gamepad);
                                           },
                                   })
                .on_pressed([this, port, &mapping] {
                    cancel_pending_binding();
                    mPendingPort = port;
                    mPendingBindingArmed = false;
                    mPendingAxisMapping = &mapping;
                });
        };

        pane.add_section("Control Stick");
        addAxis(PAD_AXIS_LEFT_Y_POS);
        addAxis(PAD_AXIS_LEFT_Y_NEG);
        addAxis(PAD_AXIS_LEFT_X_NEG);
        addAxis(PAD_AXIS_LEFT_X_POS);
        if (PADDeadZones* deadZones = PADGetDeadZones(port)) {
            pane.add_child<NumberButton>(NumberButton::Props{
                .key = "Deadzone",
                .getValue = [deadZones] { return deadzone_raw_to_percent(deadZones->stickDeadZone); },
                .setValue =
                    [deadZones](int value) {
                        deadZones->stickDeadZone = percent_to_raw(value);
                        PADSerializeMappings();
                    },
                .isDisabled = [deadZones] { return !deadZones->useDeadzones; },
                .min = 0,
                .max = 100,
                .step = 1,
                .suffix = "%",
            });
        }

        pane.add_section("C Stick");
        addAxis(PAD_AXIS_RIGHT_Y_POS);
        addAxis(PAD_AXIS_RIGHT_Y_NEG);
        addAxis(PAD_AXIS_RIGHT_X_NEG);
        addAxis(PAD_AXIS_RIGHT_X_POS);
        if (PADDeadZones* deadZones = PADGetDeadZones(port)) {
            pane.add_child<NumberButton>(NumberButton::Props{
                .key = "Deadzone",
                .getValue = [deadZones] { return deadzone_raw_to_percent(deadZones->substickDeadZone); },
                .setValue =
                    [deadZones](int value) {
                        deadZones->substickDeadZone = percent_to_raw(value);
                        PADSerializeMappings();
                    },
                .isDisabled = [deadZones] { return !deadZones->useDeadzones; },
                .min = 0,
                .max = 100,
                .step = 1,
                .suffix = "%",
            });
        }

        break;
    }
    case Page::Rumble: {
        auto& rumbleTest = pane.add_select_button({
            .key = "Test Rumble",
            .getValue =
                [this, port] {
                    return (mRumbleTestActive && mRumbleTestPort == port) ? Rml::String("Stop")
                                                                          : Rml::String("Start");
                },
        });
        rumbleTest.on_pressed([this, port] {
            if (!PADSupportsRumbleIntensity(static_cast<u32>(port))) {
                return;
            }
            mDoAud_seStartMenu(kSoundItemChange);
            if (mRumbleTestActive && mRumbleTestPort == port) {
                PADControlMotor(port, PAD_MOTOR_STOP_HARD);
                mRumbleTestActive = false;
                mRumbleTestPort = -1;
            } else {
                if (mRumbleTestActive) {
                    PADControlMotor(mRumbleTestPort, PAD_MOTOR_STOP_HARD);
                }
                PADControlMotor(port, PAD_MOTOR_RUMBLE);
                mRumbleTestActive = true;
                mRumbleTestPort = port;
            }
        });
        pane.add_child<NumberButton>(NumberButton::Props{
            .key = "Low Rumble Frequency",
            .getValue =
                [port] {
                    u16 low = 0;
                    u16 high = 0;
                    PADGetRumbleIntensity(static_cast<u32>(port), &low, &high);
                    return rumble_raw_to_percent(low);
                },
            .setValue =
                [port](int value) {
                    u16 low = 0;
                    u16 high = 0;
                    PADGetRumbleIntensity(static_cast<u32>(port), &low, &high);
                    PADSetRumbleIntensity(static_cast<u32>(port), percent_to_raw(value), high);
                    PADSerializeMappings();
                },
            .isDisabled = [this] { return mRumbleTestActive; },
            .min = 0,
            .max = 100,
            .step = 1,
            .suffix = "%",
        });
        pane.add_child<NumberButton>(NumberButton::Props{
            .key = "High Rumble Frequency",
            .getValue =
                [port] {
                    u16 low = 0;
                    u16 high = 0;
                    PADGetRumbleIntensity(static_cast<u32>(port), &low, &high);
                    return rumble_raw_to_percent(high);
                },
            .setValue =
                [port](int value) {
                    u16 low = 0;
                    u16 high = 0;
                    PADGetRumbleIntensity(static_cast<u32>(port), &low, &high);
                    PADSetRumbleIntensity(static_cast<u32>(port), low, percent_to_raw(value));
                    PADSerializeMappings();
                },
            .isDisabled = [this] { return mRumbleTestActive; },
            .min = 0,
            .max = 100,
            .step = 1,
            .suffix = "%",
        });
        pane.add_text("Configure your desired rumble intensities, then run a test to check how they feel.");
        break;
    }
    case Page::Actions: {
        if (keyboard_active(port)) {
            auto addActionBinding = [&](auto actionBind, const std::string& key) {
                pane.add_select_button(
                        {
                            .key = key,
                            .getValue =
                                [this, actionBind] {
                                    if (mPendingActionBinding == actionBind) {
                                        return pending_key_label();
                                    }

                                    return keyboard_key_name(actionBind->getValue());
                                },
                        })
                    .on_pressed([this, port, actionBind] {
                        cancel_pending_binding();
                        mPendingPort = port;
                        mPendingBindingArmed = false;
                        mPendingActionBinding = actionBind;
                    });
            };

            pane.add_section("Custom Action Bindings");
            pane.add_text("A key bound to any action here will REPLACE the default control for"
                          " that action. Only bind buttons here that aren't used anywhere else.");
            for (auto& [configVars, actionName] : getActionBinds() | std::views::values) {
                addActionBinding(&configVars->at(port), actionName);
            }
            break;
        }

        u32 buttonCount = 0;
        PADButtonMapping* mappings = PADGetButtonMappings(port, &buttonCount);
        if (mappings == nullptr) {
            pane.add_text("No controller selected");
            break;
        }

        SDL_Gamepad* gamepad = gamepad_for_port(port);
        pane.add_section("Custom Action Bindings");
        pane.add_text("A button bound to any action here will REPLACE the default control for"
                      " that action. Only bind buttons here that aren't used anywhere else. The glyphs"
                      " shown for in game actions will not change. This is not recommended for "
                      " regular Gamecube controllers.");
        auto addActionBinding = [&](auto actionBind, const std::string& key) {
            pane.add_select_button({
                           .key = key,
                           .getValue =
                               [this, gamepad, actionBind] {
                                   if (mPendingActionBinding == actionBind) {
                                       return pending_button_label();
                                   }

                                   return native_button_name(
                                       gamepad, actionBind->getValue());
                               },
                       })
                .on_pressed([this, port, actionBind] {
                    cancel_pending_binding();
                    mPendingPort = port;
                    mPendingBindingArmed = false;
                    mPendingActionBinding = actionBind;
                });
        };

        for (auto& [configVars, actionName] : getActionBinds() | std::views::values) {
            addActionBinding(&configVars->at(port), actionName);
        }
        break;
    }
    }
}

void ControllerConfigWindow::refresh_controller_page() {
    if (!visible() || mPage != Page::Controller || mRightPane == nullptr) {
        return;
    }
    render_page(*mRightPane, mActivePort, Page::Controller);
}

void ControllerConfigWindow::poll_pending_binding() {
    if (mSuppressNavigationUntilNeutral && input_neutral(mSuppressNavigationPort)) {
        mSuppressNavigationUntilNeutral = false;
        mSuppressNavigationPort = -1;
    }

    if (!capture_active()) {
        return;
    }

    if (keyboard_escape_pressed()) {
        unmap_pending_binding();
        return;
    }

    if (!mPendingBindingArmed) {
        if (pending_input_neutral()) {
            mPendingBindingArmed = true;
        }
        return;
    }

    if (mPendingKeyButton >= 0 || mPendingKeyAxis >= 0) {
        const s32 scancode = keyboard_key_pressed();
        if (scancode != PAD_KEY_INVALID) {
            if (mPendingKeyButton >= 0) {
                PADSetKeyButtonBinding(static_cast<u32>(mPendingPort),
                    {scancode, static_cast<PADButton>(mPendingKeyButton)});
            } else {
                PADSetKeyAxisBinding(static_cast<u32>(mPendingPort),
                    {scancode, static_cast<PADAxis>(mPendingKeyAxis), 0});
            }
            finish_pending_key_binding();
        }
        return;
    }

    if (mPendingButtonMapping != nullptr) {
        const s32 nativeButton = PADGetNativeButtonPressed(mPendingPort);
        if (nativeButton != -1) {
            const int completedPort = mPendingPort;
            mPendingButtonMapping->nativeButton = static_cast<u32>(nativeButton);
            finish_pending_binding(completedPort);
        }
        return;
    }

    if (mPendingAxisMapping != nullptr) {
        const PADSignedNativeAxis nativeAxis = PADGetNativeAxisPulled(mPendingPort);
        if (nativeAxis.nativeAxis != -1) {
            const int completedPort = mPendingPort;
            mPendingAxisMapping->nativeAxis = nativeAxis;
            mPendingAxisMapping->nativeButton = -1;
            finish_pending_binding(completedPort);
            return;
        }

        const s32 nativeButton = PADGetNativeButtonPressed(mPendingPort);
        if (nativeButton != -1) {
            const int completedPort = mPendingPort;
            mPendingAxisMapping->nativeAxis = {-1, AXIS_SIGN_POSITIVE};
            mPendingAxisMapping->nativeButton = nativeButton;
            finish_pending_binding(completedPort);
        }
        return;
    }

    if (mPendingActionBinding != nullptr) {
        int button{};
        if (keyboard_active(mPendingPort)) {
            button = keyboard_key_pressed();
        } else {
            button = PADGetNativeButtonPressed(mPendingPort);
        }

        if (button != -1) {
            const int completedPort = mPendingPort;
            mPendingActionBinding->setValue(button);
            config::Save();
            finish_pending_binding(completedPort);
        }
        return;
    }
}

void ControllerConfigWindow::finish_pending_binding(int completedPort) {
    mPendingButtonMapping = nullptr;
    mPendingAxisMapping = nullptr;
    mPendingActionBinding = nullptr;
    mPendingPort = -1;
    mPendingBindingArmed = false;
    mSuppressNavigationUntilNeutral = true;
    mSuppressNavigationPort = completedPort;
    PADSerializeMappings();
}

void ControllerConfigWindow::unmap_pending_binding() {
    if (mPendingButtonMapping == nullptr && mPendingAxisMapping == nullptr &&
        mPendingActionBinding == nullptr && mPendingKeyButton < 0 && mPendingKeyAxis < 0)
    {
        return;
    }

    const int completedPort = mPendingPort;
    if (mPendingButtonMapping != nullptr) {
        mPendingButtonMapping->nativeButton = PAD_NATIVE_BUTTON_INVALID;
        finish_pending_binding(completedPort);
    } else if (mPendingAxisMapping != nullptr) {
        mPendingAxisMapping->nativeAxis = {-1, AXIS_SIGN_POSITIVE};
        mPendingAxisMapping->nativeButton = -1;
        finish_pending_binding(completedPort);
    } else if (mPendingActionBinding != nullptr) {
        mPendingActionBinding->setValue(PAD_NATIVE_BUTTON_INVALID);
        finish_pending_binding(completedPort);
    } else if (mPendingKeyButton >= 0) {
        PADSetKeyButtonBinding(static_cast<u32>(completedPort),
            {PAD_KEY_INVALID, static_cast<PADButton>(mPendingKeyButton)});
        finish_pending_key_binding();
    } else if (mPendingKeyAxis >= 0) {
        PADSetKeyAxisBinding(static_cast<u32>(completedPort),
            {PAD_KEY_INVALID, static_cast<PADAxis>(mPendingKeyAxis), 0});
        finish_pending_key_binding();
    }
}

bool ControllerConfigWindow::capture_active() const {
    return mPendingButtonMapping != nullptr || mPendingAxisMapping != nullptr ||
           mPendingActionBinding != nullptr || mPendingKeyButton >= 0 || mPendingKeyAxis >= 0;
}

bool ControllerConfigWindow::pending_input_neutral() const {
    if (mPendingKeyButton >= 0 || mPendingKeyAxis >= 0) {
        return keyboard_neutral();
    }
    return input_neutral(mPendingPort);
}

Rml::String ControllerConfigWindow::pending_button_label() const {
    return mPendingBindingArmed ? "Press a button..." : "Waiting...";
}

Rml::String ControllerConfigWindow::pending_axis_label() const {
    return mPendingBindingArmed ? "Move axis or press a button..." : "Waiting...";
}

void ControllerConfigWindow::cancel_pending_binding() {
    if (mPendingButtonMapping == nullptr && mPendingAxisMapping == nullptr && mPendingActionBinding == nullptr &&
        !mSuppressNavigationUntilNeutral && mPendingKeyButton < 0 && mPendingKeyAxis < 0)
    {
        return;
    }
    mPendingButtonMapping = nullptr;
    mPendingAxisMapping = nullptr;
    mPendingActionBinding = nullptr;
    mPendingKeyButton = -1;
    mPendingKeyAxis = -1;
    mPendingPort = -1;
    mPendingBindingArmed = false;
    mSuppressNavigationUntilNeutral = false;
    mSuppressNavigationPort = -1;
}

void ControllerConfigWindow::finish_pending_key_binding() {
    mPendingKeyButton = -1;
    mPendingKeyAxis = -1;
    mPendingPort = -1;
    mPendingBindingArmed = false;
    PADSerializeMappings();
}

Rml::String ControllerConfigWindow::pending_key_label() const {
    return mPendingBindingArmed ? "Press a key or mouse button..." : "Waiting...";
}

void ControllerConfigWindow::stop_rumble_test() {
    if (!mRumbleTestActive) {
        return;
    }
    if (mRumbleTestPort >= PAD_CHAN0 && mRumbleTestPort < PAD_CHANMAX) {
        PADControlMotor(mRumbleTestPort, PAD_MOTOR_STOP_HARD);
    }
    mRumbleTestActive = false;
    mRumbleTestPort = -1;
}

Rml::String native_button_name(SDL_Gamepad* gamepad, u32 buttonUntyped) {
    if (buttonUntyped == PAD_NATIVE_BUTTON_INVALID) {
        return "Not bound";
    }

    auto button = static_cast<SDL_GamepadButton>(buttonUntyped);
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

    if (const char* name = PADGetNativeButtonName(buttonUntyped)) {
        return name;
    }
    return "Unknown";
}

}  // namespace dusk::ui
