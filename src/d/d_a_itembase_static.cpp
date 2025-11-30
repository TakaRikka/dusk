/**
 * d_a_itembase_static.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_a_itembase_static.h"
#include "SSystem/SComponent/c_lib.h"
#include "d/actor/d_a_itembase.h"
#include "d/d_item_data.h"

u8 daItemBase_c::getItemNo() {
    return m_itemNo;
}

void daItemBase_c::hide() {
    cLib_offBit<u8>(field_0x92b, 1);
}

void daItemBase_c::show() {
    cLib_onBit<u8>(field_0x92b, 1);
}

void daItemBase_c::changeDraw() {
    if (chkDraw()) {
        hide();
    } else {
        show();
    }
}

bool daItemBase_c::chkDraw() {
    return cLib_checkBit<u8>(field_0x92b, 1);
}

void daItemBase_c::dead() {
    cLib_onBit<u8>(field_0x92b, 2);
}

bool daItemBase_c::chkDead() {
    return cLib_checkBit<u8>(field_0x92b, 2);
}

int CheckItemCreateHeap(fopAc_ac_c* i_this) {
    daItemBase_c* a_this = static_cast<daItemBase_c*>(i_this);

    u8 item_no = a_this->getItemNo();
    return a_this->CreateItemHeap(dItem_data::getArcName(item_no), dItem_data::getBmdName(item_no),
                                  dItem_data::getBtkName(item_no), dItem_data::getBpkName(item_no),
                                  dItem_data::getBckName(item_no), dItem_data::getBxaName(item_no),
                                  dItem_data::getBrkName(item_no), dItem_data::getBtpName(item_no));
}

int CheckFieldItemCreateHeap(fopAc_ac_c* i_this) {
    daItemBase_c* a_this = static_cast<daItemBase_c*>(i_this);

    u8 item_no = a_this->getItemNo();
    return a_this->CreateItemHeap(
        dItem_data::getFieldArc(item_no), dItem_data::getItemBmdName(item_no),
        dItem_data::getItemBtkName(item_no), dItem_data::getItemBpkName(item_no),
        dItem_data::getItemBckName(item_no), dItem_data::getItemBxaName(item_no),
        dItem_data::getItemBrkName(item_no), dItem_data::getItemBtpName(item_no));
}

const daItemBase_data daItemBase_c::m_data = {
    -4.5f,  // mGravity
    0.62f,  // mGroundReflect
    45.0f,  // mLaunchSpeed
    10.0f,  // mScalingTime
    6.0f,   // mSpeedH
    1,      // mFlashCycleTime
    240,    // mWaitTime
    60,     // mDisappearTime
    4000,   // mRotateXSpeed
    120,    // mRotateYSpeed
    -4.0f,  // mHeartFallSpeed
    3.5f,   // mHeartAmplitude
    1100,   // mHeartFallCycleTime
    3000,   // mHeartTilt
    23.0f,  // mGetDemoLaunchSpeed
    -6.0f,  // mGetDemoGravity
    13,     // mSimpleExistTime
    10,     // mNoGetTime
};
