#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "dusk/imgui/ImGuiMenuEnhancements.hpp"

void daAlink_c::handleQuickTransform() {
    if (!dusk::settings::enhancements::QuickTransform.getValue()) {
        return;
    }

    // Ensure that link is not in a cutscene.
    if (checkEventRun()) {
        return;
    }

    // Check to see if Link has the ability to transform.
    if (!dComIfGs_isEventBit(dSv_event_flag_c::M_077)) {
        return;
    }

    // Make sure Link isn't riding anything (horse, boar, etc.)
    if (checkRide()) {
        return;
    }

    switch (mProcID) {
        // Make sure Link is not underwater or talking to someone.
        case PROC_TALK:
        case PROC_SWIM_UP:
        case PROC_SWIM_DIVE:
            return;
        // If Link is targeting or pulling a chain, we don't want to remove the ability to use items in combat accidently.
        case PROC_ATN_ACTOR_MOVE:
        case PROC_ATN_ACTOR_WAIT:
        case PROC_WOLF_ATN_AC_MOVE:
            break;
    default:
            // Disable the input that was just pressed, as sometimes it could cause items to be used or Wolf Link to dig.
            mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags = 0;
            break;
    }

    // Ensure there is a proper pointer to the mMeterClass and mpMeterDraw structs in g_meter2_info.
    const auto meterClassPtr = g_meter2_info.getMeterClass();
    if (!meterClassPtr) {
        return;
    }

    const auto meterDrawPtr = meterClassPtr->getMeterDrawPtr();
    if (!meterDrawPtr) {
        return;
    }

    // Ensure that the Z Button is not dimmed
    if (meterDrawPtr->getButtonZAlpha() != 1.f) {
        return;
    }

    // The game will crash if trying to quick transform while holding the Ball and Chain
    if (mEquipItem == dItemNo_IRONBALL_e) {
        return;
    }

    // Use the game's default checks for if the player can currently transform
    if (!m_midnaActor->checkMetamorphoseEnableBase()) {
        return;
    }

    OSReport("Running quick transform!");
    procCoMetamorphoseInit();
}