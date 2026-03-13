#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_a_horse_static.h"
#include "d/actor/d_a_e_wb.h"
#include "d/actor/d_a_tag_hstop.h"

BOOL e_wb_class::checkWait() {
    return ride_mode == 0x2A;
}

void e_wb_class::setPlayerRideNow() {
    action = ACT_PL_RIDE_NOW;
    ride_mode = 0;
    status_flag |= u16(3);
}

void e_wb_class::setPlayerRide() {
    action = ACT_PL_RIDE;
    ride_mode = 0;
    status_flag |= u16(3);

    sound.setLinkRiding(true);
}

void e_wb_class::getOff() {
    if (!checkDownDamage() || action == ACT_PL_RIDE_NOW) {
        action = 0;
    } else {
        old_action = 0;
    }

    ride_mode = 0;
    status_flag &= u16(~3);
    sound.setLinkRiding(false);
}

BOOL e_wb_class::checkDownDamage() {
    return action != ACT_PL_RIDE && action != ACT_PL_RIDE2 && action != ACT_S_DAMAGE;
}

BOOL e_wb_class::checkNormalRideMode() const {
    return action != ACT_PL_RIDE2 || ride_mode < 1;
}

void e_wb_class::setRunRideMode() {
    if (action == ACT_PL_RIDE) {
        ride_mode = 0;
        action = ACT_S_DAMAGE;
        old_action = ACT_PL_RIDE;
    }
}

daTagHstop_c* daTagHstop_c::m_top;
