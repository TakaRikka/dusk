#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_a_horse_static.h"
#include "d/actor/d_a_e_wb.h"
#include "d/actor/d_a_tag_hstop.h"

BOOL e_wb_class::checkWait() {
    return mActionMode == 0x2A;
}

void e_wb_class::setPlayerRideNow() {
    mActionID = ACT_PL_RIDE_NOW;
    mActionMode = 0;
    mStatusFlags |= 3;
}

void e_wb_class::setPlayerRide() {
    mActionID = ACT_PL_RIDE;
    mActionMode = 0;
    mStatusFlags |= 3;

    mZ2Ride.setLinkRiding(true);
}

void e_wb_class::getOff() {
    if (!checkDownDamage() || mActionID == ACT_PL_RIDE_NOW) {
        mActionID = 0;
    } else {
        field_0x692 = 0;
    }

    mActionMode = 0;
    mStatusFlags &= ~3;
    mZ2Ride.setLinkRiding(false);
}

BOOL e_wb_class::checkDownDamage() {
    return mActionID != ACT_PL_RIDE && mActionID != 0x66 && mActionID != ACT_S_DAMAGE;
}

BOOL e_wb_class::checkNormalRideMode() const {
    return mActionID != 0x66 || mActionMode < 1;
}

void e_wb_class::setRunRideMode() {
    if (mActionID == ACT_PL_RIDE) {
        mActionMode = 0;
        mActionID = ACT_S_DAMAGE;
        field_0x692 = 0x65;
    }
}

daTagHstop_c* daTagHstop_c::m_top;
