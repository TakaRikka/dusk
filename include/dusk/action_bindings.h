#pragma once

#include <unordered_map>

#include "dusk/config_var.hpp"

namespace dusk {

enum class ActionBinds {
    FIRST_PERSON_CAMERA,
    CALL_MIDNA,
    OPEN_DUSKLIGHT_MENU,
    TURBO_SPEED_BUTTON,
    TOGGLE_TEXTURE_PACK,
    COUNT,
};

enum class Type {
    GAMEPLAY,
    INTERFACE,
};

struct ActionBindData {
    std::array<config::ActionBindConfigVar, 4>* configVars{};
    std::string actionName{};
    Type type{};
};

struct ActionBindPressData {
    bool pressedCurFrame{false};
    bool pressedPrevFrame{false};
};

using ActionBindsMap = std::unordered_map<ActionBinds, ActionBindData>;

ActionBindsMap& getActionBinds();

bool isActionBound(ActionBinds action, u32 port);

void updateActionBindings();

bool getActionBindTrig(ActionBinds action, u32 port);

bool getActionBindHold(ActionBinds action, u32 port);

bool getActionBindHoldAnyPort(ActionBinds action);

int getActionBindButton(ActionBinds action, u32 port);

Type getActionBindType(ActionBinds action);

}
