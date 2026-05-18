#pragma once

#include <unordered_map>

#include "dusk/config_var.hpp"

namespace dusk {

const std::size_t ITEM_ACCESS_SLOTS=2;

enum class ActionBinds {
    /* Do nothing */
    NOBIND,

    /* Dusk Controls */
    OPEN_DUSKLIGHT_MENU,
    TURBO_SPEED_BUTTON,

    MENU_UP,
    MENU_LEFT,
    MENU_DOWN,
    MENU_RIGHT,
    MENU_CONFIRM,
    MENU_REJECT,

    MENU_X, /* Analog that resolves to MENU_LEFT or MENU_RIGHT */
    MENU_Y, /* Analog that resolves to MENU_UP or MENU_DOWN */

    /* Button Presses */
    SWORD_ATTACK, /* Attack with Sword */
    QUICK_SPIN, /* Quick bind for the C stick quick spin */
    ITEM_ACCESS_X, /* Access FIrst Quick Item slot */
    ITEM_ACCESS_Y, /* Access Second Quick Item slot */
    SHIELD_USE, /* For Shield attacks */

    INTERACT, /* Talk or Examine. Also Ingame confirm. */
    REJECT, /* Reject Ingame Options */

    ROLL, /* Roll on Ground */
    MOVE_MODIFIER_TOGGLE, /* For Players using Digital Movement Input */

    TARGET, /* Target to perform techs */
    DIG, /* Dig as Wolf Link */
    SENSE, /* Sense as Wolf Link */

    GRAB_HEAVY, /* Grab Blocks and chains to push or pull */
    GRAB_LIGHT, /* Pickup lighter items (Boulder, Pots, Pets) */

    CALL_MIDNA, /* Call on Midna, or any other available other companion like Hena during lure fishing */
    INGAME_PAUSE,  /* Pause to the Game's Menu */
    FIRST_PERSON_CAMERA, /* First Person Camera Toggle */

    MAP_CYCLE_UP,   /* Analogous to the right D-pad press during free exploration */
    MAP_CYCLE_DOWN, /* Analogous to The left D-pad press during free exploration */
    PEEK_MAP,   /* Peek Map, or Peek out if in Map */
    TOGGLE_MAP_SCREEN, /* Toggle HUD Map Screen */
    HUD_VISIBILITY, /* Toggle the HUD Visibility */

    ITEM_CYCLE_NEXT, /* Hold Down and press corresponding item access button to cycle to next item in wheel without opening it. (Default: Unbound) */
    ITEM_CYCLE_PREV, /* Hold Down and press corresponding item access button to cycle to previous item in wheel without opening it. (Default: Unbound) */

    /* Minimimal Button Bindings */
    GRAB_ALL, /* Both GRAB_HEAVY and GRAB_LIGHT */
    DUSKLIGHT_SPECIAL, /* Special Keybind for Dusklight Things (Reset/Cheats etc.) */

    /* Classical Button Bindings */
    BUTTON_A,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_Z,
    BUTTON_START,

    DPAD_UP,
    DPAD_LEFT,
    DPAD_DOWN,
    DPAD_RIGHT,

    DIGITAL_L,
    DIGITAL_R,

    /* "Analog" Binds (Cursor Input, Analog Sticks, Analog triggers etc.) */
    /* Movement input for Walking, Horseriding and Swimming */
    MOVE_X, MOVE_Y,
    /* Camera Movement during third person play */
    CAM_X, CAM_Y,
    /* Camera Movement during aiming/First Person view */
    AIM_X, AIM_Y,
    /* Item Wheel */
    WHEEL_X, WHEEL_Y,

    /* Minimal Analog Bindings */
    /* Third Person, First Person and Wheel */
    ALLCAM_X, ALLCAM_Y,

    /* Classical Analog Bindings */
    MAIN_STICK_X, MAIN_STICK_Y,
    C_STICK_X, C_STICK_Y,
    TRIGGER_L, TRIGGER_R,

    COUNT, /* Member Count. This is very hacky. I didn't realize it's purpose there for a bit. */
};

enum class ActionBindContext {
    DUSK, /* Used by Dusklight in menus */
    VERB, /* A verb of the game */
    CLASSICAL, /* Original Controller Mapping */
};

enum class ActionBindType {
    BUTTON,
    ANALOG,
};

struct ActionBindData {
    ActionBindContext actionContext{};
    ActionBindType actionType{};
    std::array<config::ActionBindConfigVar, 4>* configVars{};
    std::string actionName{};
};

struct ActionBindPressData {
    bool pressedCurFrame{false};
    bool pressedPrevFrame{false};
    int bindCurAnalogData{0};
};

using ActionBindsMap = std::unordered_map<ActionBinds, ActionBindData>;

ActionBindsMap& getActionBinds();

ActionBinds getActionItemVerb(size_t item_index);

ActionBinds getActionItemClassic(size_t item_index);

bool isActionBound(ActionBinds action, u32 port);

bool isActionBoundAnyPort(ActionBinds action);

void updateActionBindings();

bool getActionBindTrig(ActionBinds action, u32 port);

bool getActionBindTrigAnyPort(ActionBinds action);

bool getActionBindHold(ActionBinds action, u32 port);

bool getActionBindHoldAnyPort(ActionBinds action);

int getActionBindButton(ActionBinds action, u32 port);

}
