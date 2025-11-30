/**
 * m_Do_controller_pad.cpp
 * JUTGamePad Wrapper and Conversion
 */

#include "m_Do/m_Do_controller_pad.h"
#include "SSystem/SComponent/c_lib.h"
#include "f_ap/f_ap_game.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_main.h"

JUTGamePad* mDoCPd_c::m_gamePad[4];

interface_of_controller_pad mDoCPd_c::m_cpadInfo[4];

void mDoCPd_c::create() {
    #if PLATFORM_GCN || PLATFORM_SHIELD
    JUTGamePad* pad = new JUTGamePad(JUTGamePad::EPort1);
    m_gamePad[0] = pad;
    #endif

    if (mDoMain::developmentMode != 0) {
        #if PLATFORM_WII
        m_gamePad[0] = new JUTGamePad(JUTGamePad::EPort1);
        #endif

        m_gamePad[1] = new JUTGamePad(JUTGamePad::EPort2);
        m_gamePad[2] = new JUTGamePad(JUTGamePad::EPort3);
        m_gamePad[3] = new JUTGamePad(JUTGamePad::EPort4);
    } else {
        #if PLATFORM_WII
        m_gamePad[0] = NULL;
        #endif

        m_gamePad[1] = NULL;
        m_gamePad[2] = NULL;
        m_gamePad[3] = NULL;
    }

    #if PLATFORM_GCN || PLATFORM_SHIELD
    if (!mDoRst::isReset()) {
        JUTGamePad::clearResetOccurred();
        JUTGamePad::setResetCallback(mDoRst_resetCallBack, NULL);
    }
    #endif
    JUTGamePad::setAnalogMode(3);

    interface_of_controller_pad* cpad = &m_cpadInfo[0];
    for (int i = 0; i < 4; i++) {
        cpad->mTrigLockL = false;
        cpad->mHoldLockL = false;
        cpad->mTrigLockR = false;
        cpad->mHoldLockR = false;
        cpad++;
    }
}

void mDoCPd_c::read() {
    JUTGamePad::read();

    if (!mDoRst::isReset() && mDoRst::is3ButtonReset()) {
        JUTGamePad* pad = JUTGamePad::getGamePad(mDoRst::get3ButtonResetPort());

        if (!pad->isPushing3ButtonReset()) {
            mDoRst::off3ButtonReset();
        }
    }

    JUTGamePad** pad = m_gamePad;
    interface_of_controller_pad* interface = m_cpadInfo;

    for (u32 i = 0; i < 4; i++) {
        if (*pad == NULL) {
            cLib_memSet(interface, 0, sizeof(interface_of_controller_pad));
        } else {
            convert(interface, *pad);
            LRlockCheck(interface);
        }

        pad++;
        interface++;
    }
}

void mDoCPd_c::convert(interface_of_controller_pad* pInterface, JUTGamePad* pPad) {
    pInterface->mButtonFlags = pPad->getButton();
    pInterface->mPressedButtonFlags = pPad->getTrigger();
    pInterface->mMainStickPosX = pPad->getMainStickX();
    pInterface->mMainStickPosY = pPad->getMainStickY();
    pInterface->mMainStickValue = pPad->getMainStickValue();
    pInterface->mMainStickAngle = pPad->getMainStickAngle();
    pInterface->mCStickPosX = pPad->getSubStickX();
    pInterface->mCStickPosY = pPad->getSubStickY();
    pInterface->mCStickValue = pPad->getSubStickValue();
    pInterface->mCStickAngle = pPad->getSubStickAngle();

    mDoCPd_ANALOG_CONV(pPad->getAnalogA(), pInterface->mAnalogA);
    mDoCPd_ANALOG_CONV(pPad->getAnalogB(), pInterface->mAnalogB);
    mDoCPd_TRIGGER_CONV(pPad->getAnalogL(), pInterface->mTriggerLeft);
    mDoCPd_TRIGGER_CONV(pPad->getAnalogR(), pInterface->mTriggerRight);

    pInterface->mGamepadErrorFlags = pPad->getErrorStatus();
}

void mDoCPd_c::LRlockCheck(interface_of_controller_pad* interface) {
    f32 trig_left = interface->mTriggerLeft;
    interface->mTrigLockL = false;
    interface->mTrigLockR = false;

    if (trig_left > fapGmHIO_getLROnValue()) {
        if (interface->mHoldLockL != true) {
            interface->mTrigLockL = true;
        }
        interface->mHoldLockL = true;
    } else if (trig_left < fapGmHIO_getLROffValue()) {
        interface->mHoldLockL = false;
    }

    f32 trig_right = interface->mTriggerRight;
    if (trig_right > fapGmHIO_getLROnValue()) {
        if (interface->mHoldLockR != true) {
            interface->mTrigLockR = true;
        }
        interface->mHoldLockR = true;
    } else if (trig_right < fapGmHIO_getLROffValue()) {
        interface->mHoldLockR = false;
    }
}

void mDoCPd_c::recalibrate(void) {
    JUTGamePad::clearForReset();
    JUTGamePad::CRumble::setEnabled(PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT);
}
