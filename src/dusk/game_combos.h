#pragma once

#include <dolphin/types.h>

namespace dusk {

struct GameCombo {
    using ConditionFn = bool(*)();
    using ActionFn    = void(*)();

    u32         holdMask;    // all of these must be held (getHold)
    u32         trigMask;    // at least one must be newly triggered (getTrig); 0 = fires every frame while holdMask is held
    bool        strict;      // if true: (held & ~trigMask) must equal holdMask exactly, no extra buttons held
    ConditionFn condition;   // extra game-state guard; nullptr = no extra check
    ActionFn    action;      // runs when the combo is fired
    u32         consumeMask; // buttons to clear after firing; 0 = pass-through
    bool        exclusive;   // stop evaluating future combos if this fires
};

void processGameCombos();

}  // namespace dusk
