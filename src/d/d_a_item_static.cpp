/**
 * d_a_item_static.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_a_item_static.h"

static void dummy(daItem_c* item) {
    item->setFlag(0);
    item->clrFlag(0);
}

u32 daItem_c::startCtrl() {
    setFlag(FLAG_NO_MOVE_e);
    return 1;
}

u32 daItem_c::startControl() {
    mStatus = STATUS_ENEMY_CARRY_e;
    return 1;
}

u32 daItem_c::endControl() {
    mStatus = STATUS_NORMAL_e;
    return 1;
}
