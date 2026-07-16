#pragma once

#include <unordered_map>

#include "dusk/config_var.hpp"

namespace dusk {

enum class ActionBinds {
    FIRST_PERSON_CAMERA,
    CALL_MIDNA,
    OPEN_MAP_SCREEN,
    TOGGLE_MINIMAP,
    OPEN_DUSKLIGHT_MENU,
    TURBO_SPEED_BUTTON,
    COUNT,
};

struct ActionBindData {
    std::array<config::ActionBindConfigVar, 4>* configVars{};
    std::string actionName{};
};

struct ActionBindPressData {
    bool pressedCurFrame{false};
    bool pressedPrevFrame{false};
};

using ActionBindsMap = std::unordered_map<ActionBinds, ActionBindData>;

ActionBindsMap& getActionBinds();

bool isActionBound(ActionBinds action, u32 port);

void updateActionBindings();

void setVirtualActionBind(ActionBinds action, u32 port, bool pressed, bool available = true);

void clearVirtualActionBind(ActionBinds action, u32 port);

void clearAllVirtualActionBinds();

bool getActionBindTrig(ActionBinds action, u32 port);

bool getActionBindHold(ActionBinds action, u32 port);

bool getActionBindHoldAnyPort(ActionBinds action);

int getActionBindButton(ActionBinds action, u32 port);

}
