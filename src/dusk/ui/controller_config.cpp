#include "controller_config.hpp"

#include "bool_button.hpp"
#include "button.hpp"
#include "pane.hpp"
#include "select_button.hpp"

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <fmt/format.h>

#include <array>
#include <string>
#include <utility>
#include <vector>

namespace dusk::ui {
namespace {

Rml::String current_controller_name(int port) {
    const char* name = PADGetName(port);
    return name == nullptr ? "None" : name;
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
    return keys != nullptr && SDL_SCANCODE_ESCAPE < keyCount && keys[SDL_SCANCODE_ESCAPE];
}

}  // namespace

ControllerConfigWindow::ControllerConfigWindow() {
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
    cancel_pending_binding();
    Window::hide(close);
}

void ControllerConfigWindow::update() {
    poll_pending_binding();
    Window::update();
}

void ControllerConfigWindow::build_port_tab(Rml::Element* content, int port) {
    auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
    auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);
    mRightPane = &rightPane;
    mActivePort = port;

    auto showPage = [this, &rightPane, port](Page page) {
        mPage = page;
        render_page(rightPane, port, page);
    };
    auto addPageButton = [&leftPane, showPage](Page page, Rml::String key, auto getValue) {
        leftPane
            .add_select_button({
                .key = std::move(key),
                .getValue = std::move(getValue),
            })
            .on_focus([showPage, page](Rml::Event&) { showPage(page); })
            .on_pressed([showPage, page] { showPage(page); });
    };

    addPageButton(Page::Controller, "Controller", [port] { return current_controller_name(port); });
    addPageButton(Page::Buttons, "Buttons", [] { return Rml::String(">"); });
    addPageButton(Page::Triggers, "Triggers", [] { return Rml::String(">"); });
    addPageButton(Page::Sticks, "Sticks", [] { return Rml::String(">"); });

    leftPane.add_section("Options");
    leftPane
        .add_child<BoolButton>(BoolButton::Props{
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
        })
        .on_focus([&rightPane](Rml::Event&) {
            rightPane.clear();
            rightPane.add_text("Apply configured dead zones to the sticks and analog triggers.");
        });
    leftPane
        .add_child<BoolButton>(BoolButton::Props{
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
        })
        .on_focus([&rightPane](Rml::Event&) {
            rightPane.clear();
            rightPane.add_text("Treat analog trigger movement as digital L and R button input.");
        });

    render_page(rightPane, port, mPage);
}

void ControllerConfigWindow::render_page(Pane& pane, int port, Page page) {
    pane.clear();

    switch (page) {
    case Page::Controller: {
        const u32 controllerCount = PADCount();
        if (controllerCount == 0) {
            pane.add_text("No controllers detected");
            break;
        }

        pane.add_button({
                            .text = "None",
                            .isSelected = [port] { return PADGetIndexForPort(port) < 0; },
                        })
            .on_pressed([this, port] {
                cancel_pending_binding();
                PADClearPort(port);
                PADSerializeMappings();
            });

        for (u32 i = 0; i < controllerCount; ++i) {
            pane.add_button(
                    {
                        .text = controller_index_name(i),
                        .isSelected =
                            [port, i] { return PADGetIndexForPort(port) == static_cast<s32>(i); },
                    })
                .on_pressed([this, port, i] {
                    cancel_pending_binding();
                    PADSetPortForIndex(i, port);
                    PADSerializeMappings();
                });
        }
        break;
    }
    case Page::Buttons: {
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
        break;
    }
    case Page::Sticks: {
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

        pane.add_section("C Stick");
        addAxis(PAD_AXIS_RIGHT_Y_POS);
        addAxis(PAD_AXIS_RIGHT_Y_NEG);
        addAxis(PAD_AXIS_RIGHT_X_NEG);
        addAxis(PAD_AXIS_RIGHT_X_POS);
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
    }
}

void ControllerConfigWindow::finish_pending_binding(int completedPort) {
    mPendingButtonMapping = nullptr;
    mPendingAxisMapping = nullptr;
    mPendingPort = -1;
    mPendingBindingArmed = false;
    mSuppressNavigationUntilNeutral = true;
    mSuppressNavigationPort = completedPort;
    PADSerializeMappings();
}

void ControllerConfigWindow::unmap_pending_binding() {
    if (mPendingButtonMapping == nullptr && mPendingAxisMapping == nullptr) {
        return;
    }

    const int completedPort = mPendingPort;
    if (mPendingButtonMapping != nullptr) {
        mPendingButtonMapping->nativeButton = PAD_NATIVE_BUTTON_INVALID;
    }
    if (mPendingAxisMapping != nullptr) {
        mPendingAxisMapping->nativeAxis = {-1, AXIS_SIGN_POSITIVE};
        mPendingAxisMapping->nativeButton = -1;
    }
    finish_pending_binding(completedPort);
}

bool ControllerConfigWindow::capture_active() const {
    return mPendingButtonMapping != nullptr || mPendingAxisMapping != nullptr;
}

bool ControllerConfigWindow::pending_input_neutral() const {
    return input_neutral(mPendingPort);
}

Rml::String ControllerConfigWindow::pending_button_label() const {
    return mPendingBindingArmed ? "Press a button..." : "Waiting...";
}

Rml::String ControllerConfigWindow::pending_axis_label() const {
    return mPendingBindingArmed ? "Move axis or press a button..." : "Waiting...";
}

void ControllerConfigWindow::cancel_pending_binding() {
    if (mPendingButtonMapping == nullptr && mPendingAxisMapping == nullptr &&
        !mSuppressNavigationUntilNeutral)
    {
        return;
    }
    mPendingButtonMapping = nullptr;
    mPendingAxisMapping = nullptr;
    mPendingPort = -1;
    mPendingBindingArmed = false;
    mSuppressNavigationUntilNeutral = false;
    mSuppressNavigationPort = -1;
}

}  // namespace dusk::ui
