/**
 * c_API_controller_pad.cpp
 * Controller Pad API utilities
 */

#include "SSystem/SComponent/c_API_controller_pad.h"
#include "m_Do/m_Do_controller_pad.h"
#include "global.h"

#if PLATFORM_WII
#include "m_Do/m_Re_controller_pad.h"
#endif

void cAPICPad_recalibrate() {
    mDoCPd_c::recalibrate();

    #if PLATFORM_WII
    mReCPd::stopMoter();
    #endif
}

u32 cAPICPad_ANY_BUTTON(u32 portNo) {
    return mDoCPd_c::getTrig(portNo) & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START);
}

#if PLATFORM_WII
u32 cAPICPad_BUTTON(u32 i_padNo) {
    return mReCPd::getHold(i_padNo);
}

u32 cAPICPad_Z_BUTTON(u32 i_padNo) {
    return mReCPd::getHoldUp(i_padNo);
}

u32 cAPICPad_R_BUTTON(u32 i_padNo) {
    return 0;
}

u32 cAPICPad_L_BUTTON(u32 i_padNo) {
    return mReCPd::getHoldZ1(i_padNo);
}

u32 cAPICPad_A_BUTTON(u32 i_padNo) {
    return mReCPd::getHoldA(i_padNo);
}

u32 cAPICPad_B_BUTTON(u32 i_padNo) {
    return mReCPd::getHoldB(i_padNo);
}

u32 cAPICPad_X_BUTTON(u32 i_padNo) {
    return mReCPd::getHoldRight(i_padNo);
}

u32 cAPICPad_Y_BUTTON(u32 i_padNo) {
    return mReCPd::getHoldLeft(i_padNo);
}

u32 cAPICPad_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrig(i_padNo);
}

u32 cAPICPad_Z_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrigUp(i_padNo);
}

u32 cAPICPad_R_TRIGGER(u32 i_padNo) {
    return 0;
}

u32 cAPICPad_UP_TRIGGER(u32 i_padNo) {
    return 0;
}

u32 cAPICPad_DOWN_TRIGGER(u32 i_padNo) {
    return 0;
}

u32 cAPICPad_LEFT_TRIGGER(u32 i_padNo) {
    return 0;
}

u32 cAPICPad_RIGHT_TRIGGER(u32 i_padNo) {
    return 0;
}

u32 cAPICPad_L_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrigZ1(i_padNo);
}

u32 cAPICPad_A_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrigA(i_padNo);
}

u32 cAPICPad_B_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrigB(i_padNo);
}

u32 cAPICPad_X_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrigRight(i_padNo);
}

u32 cAPICPad_Y_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrigLeft(i_padNo);
}

u32 cAPICPad_START_TRIGGER(u32 i_padNo) {
    return mReCPd::getTrigStart(i_padNo);
}

f32 cAPICPad_X_STICK(u32 i_padNo) {
    return mReCPd::getPad(i_padNo).field_0x4[0].ex_status.fs.stick.x;
}

f32 cAPICPad_X_STICK_3D(u32 i_padNo) {
    return mReCPd::getPad(i_padNo).m_stick3D;
}

f32 cAPICPad_Y_STICK(u32 i_padNo) {
    return mReCPd::getPad(i_padNo).field_0x4[0].ex_status.fs.stick.y;
}

f32 cAPICPad_VALUE_STICK(u32 i_padNo) {
    return mReCPd::getPad(i_padNo).m_stickValue;
}

s16 cAPICPad_ANGLE_STICK(u32 i_padNo) {
    return mReCPd::getPad(i_padNo).m_stickAngle;
}

s16 cAPICPad_ANGLE_STICK_3D(u32 i_padNo) {
    return mReCPd::getFSStickAngle3DRev(i_padNo);
}

f32 cAPICPad_X_SUBSTICK(u32 i_padNo) {
    return 0.0f;
}

f32 cAPICPad_Y_SUBSTICK(u32 i_padNo) {
    return 0.0f;
}

f32 cAPICPad_VALUE_SUBSTICK(u32 i_padNo) {
    return 0.0f;
}

s16 cAPICPad_ANGLE_SUBSTICK(u32 i_padNo) {
    return 0;
}

f32 cAPICPad_L_ANALOG(u32 i_padNo) {
    return 0.0f;
}

f32 cAPICPad_R_ANALOG(u32 i_padNo) {
    return 0.0f;
}
#endif
