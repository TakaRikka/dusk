#include "dusk/action_bindings.h"

#include "aurora/lib/input.hpp"
#include "dusk/settings.h"
#include "dusk/ui/ui.hpp"

namespace dusk {

static std::array<std::array<ActionBindPressData, static_cast<int>(ActionBinds::COUNT)>, PAD_CHANMAX> actionPressData{};

ActionBindsMap& getActionBinds() {
    static ActionBindsMap actionBinds = {
        {ActionBinds::OPEN_DUSKLIGHT_MENU,  {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.openDusklightMenu,    "Open Dusklight Menu"}},
        {ActionBinds::TURBO_SPEED_BUTTON,   {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.turboSpeedButton, "Turbo Speed Button"}},
        {ActionBinds::MENU_UP,  {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.menuUp,   "Menu Up"}},
        {ActionBinds::MENU_LEFT,    {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.menuLeft, "Menu Left"}},
        {ActionBinds::MENU_DOWN,    {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.menuDown, "Menu Down"}},
        {ActionBinds::MENU_RIGHT,   {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.menuRight,    "Menu Right"}},
        {ActionBinds::MENU_CONFIRM, {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.menuConfirm,  "Confirm"}},
        {ActionBinds::MENU_REJECT,  {ActionBindContext::DUSK,  ActionBindType::BUTTON,    &getSettings().actionBindings.menuReject,   "Reject"}},

        {ActionBinds::MENU_X,   {ActionBindContext::DUSK,  ActionBindType::ANALOG,    &getSettings().actionBindings.menuX,    "Menu Left/Right"}},
        {ActionBinds::MENU_Y,   {ActionBindContext::DUSK,  ActionBindType::ANALOG,    &getSettings().actionBindings.menuY,    "Menu Up/Down"}},

        {ActionBinds::SWORD_ATTACK, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.swordAttack,  "Use Sword"}},
        {ActionBinds::ITEM_ACCESS_X,    {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.itemAccessX,  "Item Slot 2"}},
        {ActionBinds::ITEM_ACCESS_Y,    {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.itemAccessY,  "Item Slot 1"}},
        {ActionBinds::SHIELD_USE,   {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.shieldUse,    "Use Shield"}},
        {ActionBinds::INTERACT, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.interact, "Interact"}},
        {ActionBinds::ROLL, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.roll, "Roll"}},
        {ActionBinds::MOVE_MODIFIER_TOGGLE, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.moveModifierToggle,   "Run/Walk"}},
        {ActionBinds::TARGET,   {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.target,   "Target"}},
        {ActionBinds::GRAB_HEAVY,   {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.grabHeavy,    "Grab blocks and chains"}},
        {ActionBinds::GRAB_LIGHT,   {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.grabLight,    "Pick up"}},
        {ActionBinds::CALL_MIDNA,   {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.callMidna,    "Call Companion"}},
        {ActionBinds::INGAME_PAUSE, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.ingamePause,  "Character Menu"}},
        {ActionBinds::FIRST_PERSON_CAMERA,  {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.firstPersonCamera,    "First Person Camera"}},
        {ActionBinds::MAP_CYCLE_UP, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.mapCycleUp,   "Map Cycle Up"}},
        {ActionBinds::MAP_CYCLE_DOWN,   {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.mapCycleDown, "Map Cycle Down"}},
        {ActionBinds::PEEK_MAP, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.peekMap,  "Peek Map"}},
        {ActionBinds::TOGGLE_MAP_SCREEN,    {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.toggleMapScreen,  "Toggle Map Screen"}},
        {ActionBinds::HUD_VISIBILITY,   {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.hudVisibility,    "Toggle HUD Visibility"}},
        {ActionBinds::ITEM_CYCLE_NEXT,  {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.itemCycleNext,    "Next Item"}},
        {ActionBinds::ITEM_CYCLE_PREV,  {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.itemCyclePrev,    "Previous Item"}},
        {ActionBinds::GRAB_ALL, {ActionBindContext::VERB,  ActionBindType::BUTTON,    &getSettings().actionBindings.grabAll,  "Grab and Pick up"}},

        {ActionBinds::MOVE_X,   {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.moveX,    "Move Left/Right"}},
        {ActionBinds::MOVE_Y,   {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.moveY,    "Move Forwards/Backwards"}},
        {ActionBinds::CAM_X,    {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.camX, "Camera Left/Right (Third Person)"}},
        {ActionBinds::CAM_Y,    {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.camY, "Camera Up/Down (Third Person)"}},
        {ActionBinds::AIM_X,    {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.aimX, "Aim Left/Right"}},
        {ActionBinds::AIM_Y,    {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.aimY, "Aim Up/Down"}},
        {ActionBinds::WHEEL_X,  {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.wheelX,   "Wheel Left/Right"}},
        {ActionBinds::WHEEL_Y,  {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.wheelY,   "Wheel Up/Down"}},
        {ActionBinds::ALLCAM_X, {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.allcamX,  "Camera Left/Right"}},
        {ActionBinds::ALLCAM_Y, {ActionBindContext::VERB,  ActionBindType::ANALOG,    &getSettings().actionBindings.allcamY,  "Camera Up/Down"}},

        {ActionBinds::BUTTON_A, {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.buttonA,  "A"}},
        {ActionBinds::BUTTON_B, {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.buttonB,  "B"}},
        {ActionBinds::BUTTON_X, {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.buttonX,  "X"}},
        {ActionBinds::BUTTON_Y, {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.buttonY,  "Y"}},
        {ActionBinds::BUTTON_Z, {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.buttonZ,  "Z"}},
        {ActionBinds::BUTTON_START, {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.buttonStart,  "START"}},
        {ActionBinds::DPAD_UP,  {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.dpadUp,   "Dpad UP"}},
        {ActionBinds::DPAD_LEFT,    {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.dpadLeft, "Dpad LEFT"}},
        {ActionBinds::DPAD_DOWN,    {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.dpadDown, "Dpad DOWN"}},
        {ActionBinds::DPAD_RIGHT,   {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.dpadRight,    "Dpad RIGHT"}},
        {ActionBinds::DIGITAL_L,    {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.digitalL, "Digital L"}},
        {ActionBinds::DIGITAL_R,    {ActionBindContext::CLASSICAL, ActionBindType::BUTTON,    &getSettings().actionBindings.digitalR, "Digital R"}},

        {ActionBinds::MAIN_STICK_X, {ActionBindContext::CLASSICAL, ActionBindType::ANALOG,    &getSettings().actionBindings.mainStickX,   "Main Stick X"}},
        {ActionBinds::MAIN_STICK_Y, {ActionBindContext::CLASSICAL, ActionBindType::ANALOG,    &getSettings().actionBindings.mainStickY,   "Main Stick Y"}},
        {ActionBinds::C_STICK_X,    {ActionBindContext::CLASSICAL, ActionBindType::ANALOG,    &getSettings().actionBindings.cStickX,  "C-Stick X"}},
        {ActionBinds::C_STICK_Y,    {ActionBindContext::CLASSICAL, ActionBindType::ANALOG,    &getSettings().actionBindings.cStickY,  "C-Stick Y"}},
        {ActionBinds::TRIGGER_L,    {ActionBindContext::CLASSICAL, ActionBindType::ANALOG,    &getSettings().actionBindings.triggerL, "Analog L"}},
        {ActionBinds::TRIGGER_R,    {ActionBindContext::CLASSICAL, ActionBindType::ANALOG,    &getSettings().actionBindings.triggerR, "Analog R"}},
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
            // a dusklight bound one, don't update. Otherwise, we may accidentally
            // perform actions while the dusklight menu is open.
            // Also skip updating Analog bindings for now
            if (!isActionBound(action, port) ||
                (ui::any_document_visible() && getActionBinds()[action].actionContext != ActionBindContext::DUSK) ||
                (getActionBinds()[action].actionType != ActionBindType::BUTTON)) {
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
