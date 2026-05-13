#include "dusk/action_bindings.h"

#include "aurora/lib/input.hpp"
#include "dusk/settings.h"
#include "dusk/ui/ui.hpp"

namespace dusk {

static std::array<std::array<ActionBindPressData, static_cast<int>(ActionBinds::COUNT)>, PAD_CHANMAX> actionPressData{};

ActionBindsMap& getActionBinds() {
    static ActionBindsMap actionBinds = {
        {ActionBinds::FIRST_PERSON_CAMERA, {&getSettings().actionBindings.firstPersonCamera, "First Person Camera"}},
        {ActionBinds::CALL_MIDNA,          {&getSettings().actionBindings.callMidna,         "Call Midna"}},
        {ActionBinds::OPEN_DUSKLIGHT_MENU, {&getSettings().actionBindings.openDusklightMenu, "Open Dusklight Menu"}},
        {ActionBinds::TURBO_SPEED_BUTTON,  {&getSettings().actionBindings.turboSpeedButton,  "Turbo Speed Button"}},
    };
    return actionBinds;
}

bool isActionBound(ActionBinds action, u32 port) {
    auto& actionBinds = getActionBinds();
    // Check to make sure action is properly bound
    if (!actionBinds.contains(action)) {
        return false;
    }

    return getActionBindButton(action, port) != PAD_NATIVE_BUTTON_INVALID;
}

void updateActionBindings() {
    for (u32 port = 0; port < PAD_CHANMAX; ++port) {
        // Move the current press to the previous frame
        for (auto& pressData : actionPressData[port]) {
            pressData.pressedPrevFrame = pressData.pressedCurFrame;
            pressData.pressedCurFrame = false;
        }

        // Update current frame with whether action button is pressed
        for (auto& [action, boundAction] : getActionBinds()) {
            // If the action isn't bound, or if documents are visible and the action isn't
            // opening the dusklight menu, don't update. Otherwise, we may accidentally
            // perform actions while the dusklight menu is open.
            if (!isActionBound(action, port) ||
                (ui::any_document_visible() && action != ActionBinds::OPEN_DUSKLIGHT_MENU)) {
                continue;
            }

            int button = boundAction.configVars->at(port);

            // If keyboard is active for this port
            u32 count = 0;
            if (PADGetKeyButtonBindings(port, &count) != nullptr) {
                int numKeys = 0;
                const bool* kbState = SDL_GetKeyboardState(&numKeys);
                if (kbState[button]) {
                    actionPressData[port][static_cast<int>(action)].pressedCurFrame = true;
                }
            } else {
                // If controller is active
                auto controller = aurora::input::get_controller_for_player(port);
                if (controller) {
                    if (SDL_GetGamepadButton(controller->m_controller, static_cast<SDL_GamepadButton>(button))) {
                        actionPressData[port][static_cast<int>(action)].pressedCurFrame = true;
                    }
                }
            }
        }
    }
}

bool getActionBindTrig(ActionBinds action, u32 port) {
    return isActionBound(action, port) &&
           actionPressData[port][static_cast<int>(action)].pressedCurFrame &&
          !actionPressData[port][static_cast<int>(action)].pressedPrevFrame;
}

bool getActionBindHold(ActionBinds action, u32 port) {
    return isActionBound(action, port) &&
           actionPressData[port][static_cast<int>(action)].pressedCurFrame &&
           actionPressData[port][static_cast<int>(action)].pressedPrevFrame;
}

bool getActionBindHoldAnyPort(ActionBinds action) {
    for (u32 port = 0; port < PAD_CHANMAX; ++port) {
        if (getActionBindHold(action, port)) {
            return true;
        }
    }
    return false;
}

int getActionBindButton(ActionBinds action, u32 port) {
    return (*getActionBinds()[action].configVars)[port];
}
}
