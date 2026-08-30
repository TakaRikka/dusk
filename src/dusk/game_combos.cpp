#include "dusk/game_combos.h"

#include "SSystem/SComponent/c_API_controller_pad.h"
#include "SSystem/SComponent/c_xyz.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "dusk/settings.h"
#include "m_Do/m_Do_controller_pad.h"

namespace dusk {
namespace {

cXyz s_savedTeleportPos{};
s16 s_savedTeleportAngle = 0;
bool s_hasTeleportPos = false;

static daAlink_c* getPlayer() {
    return (daAlink_c*)dComIfGp_getPlayer(0);
}

static void consumeButtons(u32 mask) {
    mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags &= ~mask;
    mDoCPd_c::getCpadInfo(PAD_1).mButtonFlags &= ~mask;
}

// Table: holdMask, trigMask, strict, condition, action, consumeMask, exclusive
static const GameCombo kCombos[] = {
    // Move Link (L+R+Y), pass-through, non-exclusive
    {
        PAD_TRIGGER_R | PAD_TRIGGER_L,
        PAD_BUTTON_Y,
        false,
        [] { return (bool)getSettings().game.enableMoveLinkCombo; },
        [] { getTransientSettings().moveLinkActive = !getTransientSettings().moveLinkActive; },
        0,
        false,
    },
    // Quick Transform (R+Y, strictly only R held)
    {
        PAD_TRIGGER_R,
        PAD_BUTTON_Y,
        true,
        [] { return getPlayer() != nullptr; },
        [] { getPlayer()->handleQuickTransform(); },
        PAD_BUTTON_Y,
        false,
    },
    // Wolf Howl (R+X)
    {
        PAD_TRIGGER_R,
        PAD_BUTTON_X,
        false,
        [] { return getPlayer() != nullptr; },
        [] { getPlayer()->handleWolfHowl(); },
        PAD_BUTTON_X,
        false,
    },
    // Teleport save (R+D-pad Up), consumes D-pad Up, exclusive
    {
        PAD_TRIGGER_R,
        PAD_BUTTON_UP,
        false,
        [] { return getSettings().game.enableTeleportCombo && getPlayer() != nullptr; },
        [] {
            auto* p = getPlayer();
            s_savedTeleportPos = p->current.pos;
            s_savedTeleportAngle = p->shape_angle.y;
            s_hasTeleportPos = true;
        },
        PAD_BUTTON_UP,
        true,
    },
    // Teleport load (R+D-pad Down), consumes D-pad Down, exclusive
    {
        PAD_TRIGGER_R,
        PAD_BUTTON_DOWN,
        false,
        [] {
            return getSettings().game.enableTeleportCombo && s_hasTeleportPos &&
                   getPlayer() != nullptr;
        },
        [] {
            auto* p = getPlayer();
            p->current.pos = s_savedTeleportPos;
            p->shape_angle.y = s_savedTeleportAngle;
            p->mNormalSpeed = 0.0f;
        },
        PAD_BUTTON_DOWN,
        true,
    },
    // Moon Jump (R+A, hold), continuous, pass-through, non-exclusive
    {
        PAD_TRIGGER_R | PAD_BUTTON_A,
        0,
        false,
        [] { return getSettings().game.moonJump && getPlayer() != nullptr; },
        [] { getPlayer()->speed.y = 56.0f; },
        0,
        false,
    },
};

}  // namespace

void processGameCombos() {
    if (!getSettings().game.enableMoveLinkCombo) {
        getTransientSettings().moveLinkActive = false;
    }

    const u32 held = mDoCPd_c::getHold(PAD_1);
    const u32 trig = mDoCPd_c::getTrig(PAD_1);

    for (const auto& combo : kCombos) {
        if ((held & combo.holdMask) != combo.holdMask) {
            continue;
        }
        if (combo.strict && (held & ~combo.trigMask) != combo.holdMask) {
            continue;
        }
        if (combo.trigMask != 0 && !(trig & combo.trigMask)) {
            continue;
        }
        if (combo.condition != nullptr && !combo.condition()) {
            continue;
        }
        combo.action();
        if (combo.consumeMask != 0) {
            consumeButtons(combo.consumeMask);
        }
        if (combo.exclusive) {
            return;
        }
    }
}

}  // namespace dusk
