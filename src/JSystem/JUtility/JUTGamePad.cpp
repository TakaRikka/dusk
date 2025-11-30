#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTGamePad.h"
#include <math.h>

u32 JUTGamePad::CRumble::sChannelMask[4] = {
    PAD_CHAN0_BIT,
    PAD_CHAN1_BIT,
    PAD_CHAN2_BIT,
    PAD_CHAN3_BIT,
};

static u32 channel_mask[4] = {PAD_CHAN0_BIT, PAD_CHAN1_BIT, PAD_CHAN2_BIT, PAD_CHAN3_BIT};

JSUList<JUTGamePad> JUTGamePad::mPadList(false);

bool JUTGamePad::mListInitialized;

u8 JUTGamePad::mPadAssign[4];

JUTGamePad::JUTGamePad(EPadPort port) : mRumble(this), mLink(this) {
    mPortNum = port;
    if (mPortNum >= 0) {
        mPadAssign[port]++;
    }

    initList();
    mPadList.append(&mLink);
    update();
    mPadRecord = 0;
    mPadReplay = 0;
    clear();
}

JUTGamePad::~JUTGamePad() {
    if (mPortNum >= 0) {
        mPadAssign[mPortNum]--;
        mPortNum = EPortInvalid;
    }

    mPadList.remove(&mLink);
}

void JUTGamePad::initList() {
    if (!mListInitialized) {
        mPadList.initiate();
        mListInitialized = true;
    }
}

u32 JUTGamePad::sSuppressPadReset;

s32 JUTGamePad::sAnalogMode;

BOOL JUTGamePad::init() {
    PADSetSpec(PAD_SPEC_5);
    setAnalogMode(3);
    return PADInit();
}

void JUTGamePad::clear() {
    mButtonReset.mReset = false;
    field_0xa8 = 1;
}

PADStatus JUTGamePad::mPadStatus[4];

JUTGamePad::CButton JUTGamePad::mPadButton[4];

JUTGamePad::CStick JUTGamePad::mPadMStick[4];

JUTGamePad::CStick JUTGamePad::mPadSStick[4];

JUTGamePad::EStickMode JUTGamePad::sStickMode = EStickMode1;

int JUTGamePad::sClampMode = EClampStick;

u32 JUTGamePad::sRumbleSupported;

u32 JUTGamePad::read() {
    sRumbleSupported = PADRead(mPadStatus);

    switch (sClampMode) {
    case EClampStick:
        PADClamp(mPadStatus);
        break;
    case EClampCircle:
        PADClampCircle(mPadStatus);
        break;
    }

    u32 reset_mask = 0;
    u32 bittest;
    for (int i = 0; i < 4; i++) {
        bittest = PAD_CHAN0_BIT >> i;

        if (mPadStatus[i].err == 0) {
            u32 stick_status;
            stick_status = mPadMStick[i].update(mPadStatus[i].stickX, mPadStatus[i].stickY, sStickMode, EMainStick, mPadButton[i].mButton) << 0x18;
            stick_status |= (mPadSStick[i].update(mPadStatus[i].substickX, mPadStatus[i].substickY, sStickMode, ESubStick, mPadButton[i].mButton) << 0x10);
            
            mPadButton[i].update(&mPadStatus[i], stick_status);
        } else if (mPadStatus[i].err == -1) {
            mPadMStick[i].update(0, 0, sStickMode, EMainStick, 0);
            mPadSStick[i].update(0, 0, sStickMode, ESubStick, 0);
            mPadButton[i].update(NULL, 0);

            if (!(sSuppressPadReset & bittest)) {
                reset_mask |= bittest;
            }
        } else {
            mPadButton[i].mTrigger = 0;
            mPadButton[i].mRelease = 0;
            mPadButton[i].mRepeat = 0;
        }
    }

    for (JSUListIterator<JUTGamePad> pad(mPadList.getFirst()); pad != mPadList.getEnd(); pad++) {
        if (pad->getPadReplay() != NULL && pad->getPadReplay()->isActive()) {
            PADStatus status;
            pad->getPadReplay()->getStatus(&status);

            u32 stick_status;
            stick_status = pad->mMainStick.update(status.stickX, status.stickY, sStickMode,
                                                 EMainStick, pad->mButton.mButton) << 0x18;
            stick_status |= pad->mSubStick.update(status.substickX, status.substickY, sStickMode,
                                                ESubStick, pad->mButton.mButton) << 0x10;

            pad->mButton.update(&status, stick_status);
        } else {
            if (pad->mPortNum == EPortInvalid) {
                pad->assign();
            }
            pad->update();
        }

        if (pad->getPadRecord() != NULL && pad->getPadRecord()->isActive()) {
            int port = pad->mPortNum;
            if (port >= 0 && mPadStatus[port].err == 0) {
                pad->getPadRecord()->write(&mPadStatus[port]);
            }
        }
    }

    if (reset_mask != 0) {
        PADReset(reset_mask);
    }

    checkResetSwitch();
    return sRumbleSupported;
}

void JUTGamePad::assign() {
    for (int i = 0; i < 4; i++) {
        if (mPadStatus[i].err == 0 && mPadAssign[i] == 0) {
            mPortNum = i;
            mPadAssign[i] = 1;
            mPadButton[i].setRepeat(mButton.mRepeatMask, mButton.mRepeatDelay, mButton.mRepeatRate);
            mRumble.clear(this);
            break;
        }
    }
}

bool JUTGamePad::CRumble::mStatus[4];

u32 JUTGamePad::CRumble::mEnabled;

callbackFn JUTGamePad::C3ButtonReset::sCallback;

void* JUTGamePad::C3ButtonReset::sCallbackArg;

OSTime JUTGamePad::C3ButtonReset::sThreshold = (OSTime)(OS_TIMER_CLOCK / 60) * 30;

/* 80451500 0001+00 data_80451500 None */
bool JUTGamePad::C3ButtonReset::sResetSwitchPushing;

/* 80451501 0003+00 data_80451501 None */
bool JUTGamePad::C3ButtonReset::sResetOccurred;

s32 JUTGamePad::C3ButtonReset::sResetOccurredPort;

void JUTGamePad::checkResetCallback(OSTime holdTime) {
    if (holdTime >= JUTGamePad::C3ButtonReset::sThreshold) {
        JUTGamePad::C3ButtonReset::sResetOccurred = true;
        JUTGamePad::C3ButtonReset::sResetOccurredPort = mPortNum;

        if (JUTGamePad::C3ButtonReset::sCallback != NULL) {
            JUTGamePad::C3ButtonReset::sCallback(mPortNum, JUTGamePad::C3ButtonReset::sCallbackArg);
        }
    }
}

f32 JUTGamePad::CStick::sPressPoint = 0.5f;

f32 JUTGamePad::CStick::sReleasePoint = 0.25f;

u32 JUTGamePad::C3ButtonReset::sResetPattern = PAD_BUTTON_START | PAD_BUTTON_X | PAD_BUTTON_B;

u32 JUTGamePad::C3ButtonReset::sResetMaskPattern = 0x0000FFFF;

void JUTGamePad::update() {
    if (mPortNum != EPortInvalid) {
        if (mPortNum >= 0 && mPortNum < 4) {
            mButton = mPadButton[mPortNum];
            mMainStick = mPadMStick[mPortNum];
            mSubStick = mPadSStick[mPortNum];
            mErrorStatus = mPadStatus[mPortNum].err;
        }

        if (field_0xa8 == 0 || C3ButtonReset::sResetPattern != (mButton.mButton & C3ButtonReset::sResetMaskPattern)) {
            mButtonReset.mReset = false;
        } else if (!JUTGamePad::C3ButtonReset::sResetOccurred) {
            if (mButtonReset.mReset == true) {
                OSTime hold_time = OSGetTime() - mResetHoldStartTime;
                checkResetCallback(hold_time);
            } else {
                mButtonReset.mReset = true;
                mResetHoldStartTime = OSGetTime();
            }
        }

        for (JSUListIterator<JUTGamePadLongPress> pad(JUTGamePadLongPress::sPatternList.getFirst()); pad != JUTGamePadLongPress::sPatternList.getEnd(); ++pad) {
            if (pad->isValid()) {
                if (mPortNum >= 0 && mPortNum < 4) {
                    if ((mButton.mButton & pad->getMaskPattern()) == pad->getPattern()) {
                        if (pad->mLongPressStatus[mPortNum] == true) {
                            OSTime hold_time = OSGetTime() - pad->mStartHoldTime[mPortNum];
                            pad->checkCallback(mPortNum, hold_time);
                        } else {
                            pad->mLongPressStatus[mPortNum] = true;
                            pad->mStartHoldTime[mPortNum] = OSGetTime();
                        }
                    } else {
                        pad->mLongPressStatus[mPortNum] = false;
                    }
                }
            }
        }

        if (mPortNum >= 0 && mPortNum < 4) {
            mRumble.update(mPortNum);
        }
    }
}

JSUList<JUTGamePadLongPress> JUTGamePadLongPress::sPatternList(false);

void JUTGamePad::checkResetSwitch() {
    if (!JUTGamePad::C3ButtonReset::sResetOccurred) {
        if (OSGetResetSwitchState()) {
            C3ButtonReset::sResetSwitchPushing = true;
        } else {
            if (C3ButtonReset::sResetSwitchPushing == true) {
                C3ButtonReset::sResetOccurred = true;
                C3ButtonReset::sResetOccurredPort = EPortInvalid;

                if (C3ButtonReset::sCallback != NULL) {
                    C3ButtonReset::sCallback(EPortInvalid, C3ButtonReset::sCallbackArg);
                }
            }
            C3ButtonReset::sResetSwitchPushing = false;
        }
    }
}

void JUTGamePad::clearForReset() {
    CRumble::setEnabled(0);
    recalibrate(PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT);
}

void JUTGamePad::CButton::clear() {
    mButton = 0;
    mTrigger = 0;
    mRelease = 0;
    mRepeat = 0;
    mAnalogA = 0;
    mAnalogB = 0;
    mAnalogL = 0;
    mAnalogR = 0;
    mRepeatCount = 0;
    mRepeatStart = 0;
    mRepeatMask = 0;
    mRepeatDelay = 0;
    mRepeatRate = 0;
}

void JUTGamePad::CButton::update(const PADStatus* padStatus, u32 stickStatus) {
    u32 buttons;
    if (padStatus != NULL) {
        buttons = padStatus->button;
    } else {
        buttons = 0;
    }

    buttons = stickStatus | buttons;
    mRepeat = 0;

    if (mRepeatDelay != 0 && mRepeatMask != 0) {
        u32 repeatButton = buttons & mRepeatMask;
        mRepeat = 0;

        if (repeatButton == 0) {
            mRepeatStart = 0;
            mRepeatCount = 0;
        } else if (mRepeatStart == repeatButton) {
            mRepeatCount++;

            if (mRepeatCount == mRepeatDelay ||
                (mRepeatCount > mRepeatDelay && (mRepeatCount - mRepeatDelay) % mRepeatRate == 0))
            {
                mRepeat = repeatButton;
            }
        } else {
            mRepeat = repeatButton & (mRepeatStart ^ 0xFFFFFFFF);
            mRepeatStart = repeatButton;
            mRepeatCount = 0;
        }
    }

    mTrigger = buttons & (buttons ^ mButton);
    mRelease = mButton & (buttons ^ mButton);
    mButton = buttons;
    mRepeat |= (mRepeatMask ^ 0xFFFFFFFF) & mTrigger;

    if (padStatus != NULL) {
        mAnalogA = padStatus->analogA;
        mAnalogB = padStatus->analogB;
        mAnalogL = padStatus->triggerLeft;
        mAnalogR = padStatus->triggerRight;
    } else {
        mAnalogA = 0;
        mAnalogB = 0;
        mAnalogL = 0;
        mAnalogR = 0;
    }

    mAnalogLf = (s32)mAnalogL / 150.0f;
    mAnalogRf = (s32)mAnalogR / 150.0f;
}

void JUTGamePad::CStick::clear() {
    mPosX = 0.0f;
    mPosY = 0.0f;
    mValue = 0.0f;
    mAngle = 0;
}

u32 JUTGamePad::CStick::update(s8 x, s8 y, JUTGamePad::EStickMode mode,
                               JUTGamePad::EWhichStick stick, u32 buttons) {
    s32 clamp;
    switch (sClampMode) {
    case EClampStick:
        clamp = stick == EMainStick ? 54 : 42;
        break;
    case EClampCircle:
        clamp = stick == EMainStick ? 38 : 29;
        break;
    default:
        clamp = stick == EMainStick ? 69 : 57;
        break;
    }

    mRawX = x;
    mRawY = y;
    mPosX = (f32)x / (f32)clamp;
    mPosY = (f32)y / (f32)clamp;
    mValue = sqrtf((mPosX * mPosX) + (mPosY * mPosY));

    if (mValue > 1.0f) {
        if (mode == EStickMode1) {
            mPosX /= mValue;
            mPosY /= mValue;
        }
        mValue = 1.0f;
    }

    if (mValue > 0.0f) {
        if (mPosY == 0.0f) {
            if (mPosX > 0.0f) {
                mAngle = 0x4000;
            } else {
                mAngle = -0x4000;
            }
        } else {
            mAngle = (0x8000 / 3.1415926f) * atan2f(mPosX, -mPosY);
        }
    }

    u32 button_shift = stick == EMainStick ? 0x18 : 0x10;
    return getButton(buttons >> button_shift);
}

u32 JUTGamePad::CStick::getButton(u32 buttons) {
    u32 button = buttons & 0xF;

    if (-sReleasePoint < mPosX && mPosX < sReleasePoint) {
        button = button & ~0x3;
    } else if (mPosX <= -sPressPoint) {
        button = (button & ~0x2) | 1;
    } else if (mPosX >= sPressPoint) {
        button = (button & ~0x1) | 2;
    }

    if (-sReleasePoint < mPosY && mPosY < sReleasePoint) {
        button = button & ~0xC;
    } else if (mPosY <= -sPressPoint) {
        button = (button & ~0x8) | 4;
    } else if (mPosY >= sPressPoint) {
        button = (button & ~0x4) | 8;
    }

    return button;
}

void JUTGamePad::CRumble::clear() {
    mFrame = 0;
    mLength = 0;
    mPattern = NULL;
    mFrameCount = 0;
    field_0x10 = 0;
    mEnabled = (PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT);
}

void JUTGamePad::CRumble::clear(JUTGamePad* pad) {
    if (pad->getPortNum() >= 0 && pad->getPortNum() < 4) {
        mStatus[pad->getPortNum()] = false;
        pad->stopMotorHard();
    }
    clear();
}

void JUTGamePad::CRumble::startMotor(int port) {
    if (isEnabledPort(port)) {
        PADControlMotor(port, PAD_MOTOR_RUMBLE);
        mStatus[port] = true;
    }
}

void JUTGamePad::CRumble::stopMotor(int port, bool hard_stop) {
    if (isEnabledPort(port)) {
        u8 command = hard_stop ? PAD_MOTOR_STOP_HARD : PAD_MOTOR_STOP;
        PADControlMotor(port, command);
        mStatus[port] = false;
    }
}

static bool getNumBit(u8* pattern, int index) {
    u8 bit = pattern[index >> 3] & (0x80 >> (index & 7));
    return bit != 0;
}

void JUTGamePad::CRumble::update(s16 port) {
    if (isEnabledPort(port) == false) {
        mFrame = 0;
        mLength = 0;
        mPattern = NULL;
        mFrameCount = 0;
        field_0x10 = NULL;
    }

    if (mLength == 0) {
        return;
    }

    if (mFrame >= mLength) {
        stopMotorHard(port);
        mLength = 0;
    } else if (mFrameCount == 0) {
        if (mStatus[port] == 0) {
            startMotor(port);
        }
        return;
    } else {
        bool enabled = getNumBit(mPattern, mFrame % mFrameCount);
        u8 status = mStatus[port] != false;

        if (enabled && !status) {
            startMotor(port);
        } else if (!enabled) {
            bool hard_stop = false;
            if (field_0x10) {
                hard_stop = getNumBit(field_0x10, mFrame % mFrameCount);
            }

            if (status) {
                stopMotor(port, hard_stop);
            } else if (hard_stop) {
                stopMotorHard(port);
            }
        }
    }

    mFrame++;
}

void JUTGamePad::CRumble::triggerPatternedRumble(u32 length) {
    if (mPattern != NULL && mFrameCount != 0) {
        mLength = length;
        mFrame = 0;
    }
}

void JUTGamePad::CRumble::startPatternedRumble(void* data, JUTGamePad::CRumble::ERumble rumble,
                                               u32 length) {
    mFrameCount = ((*(u8*)data) << 8) + *((u8*)data + 1);
    mPattern = (u8*)data + 2;
    
    switch (rumble) {
    case JUTGamePad::CRumble::VAL_0:
        triggerPatternedRumble(mFrameCount);
        break;
    case JUTGamePad::CRumble::VAL_1:
        triggerPatternedRumble(-1);
        break;
    case JUTGamePad::CRumble::VAL_2:
        triggerPatternedRumble(length);
        break;
    }
}

void JUTGamePad::CRumble::stopPatternedRumble(s16 port) {
    mLength = 0;
    stopMotorHard(port);
}

void JUTGamePad::CRumble::stopPatternedRumbleAtThePeriod() {
    mLength = (mFrame + mFrameCount - 1) % mFrameCount;
}

JUTGamePad* JUTGamePad::getGamePad(int port) {
    JSUListIterator<JUTGamePad> pad(mPadList.getFirst());

    for (; pad != mPadList.getEnd(); pad++) {
        if (port == pad->mPortNum) {
            return pad.getObject();
        }
    }

    return NULL;
}

void JUTGamePad::CRumble::setEnabled(u32 mask) {
    for (int i = 0; i < 4; i++) {
        if ((mEnabled & channel_mask[i]) == 0) {
            if (mStatus[i]) {
                stopMotor(i, false);
            }

            JUTGamePad* pad = getGamePad(i);
            if (pad != NULL) {
                pad->mRumble.stopPatternedRumble(pad->mPortNum);
            }
        }
    }

    mEnabled = (mask & (PAD_CHAN3_BIT | PAD_CHAN2_BIT | PAD_CHAN1_BIT | PAD_CHAN0_BIT));
}

void JUTGamePad::CButton::setRepeat(u32 mask, u32 delay, u32 rate) {
    mRepeatStart = 0;
    mRepeatCount = 0;
    mRepeatMask = mask;
    mRepeatDelay = delay;
    mRepeatRate = rate;
}

bool JUTGamePad::recalibrate(u32 mask) {
    for (int i = 0; i < 4; i++) {
        if (sSuppressPadReset & channel_mask[i]) {
            mask &= channel_mask[i] ^ 0xFFFFFFFF;
        }
    }

    return PADRecalibrate(mask);
}

void JUTGamePadLongPress::checkCallback(int port, u32 hold_time) {
    if (port >= 0 && hold_time >= mThreshold) {
        field_0x11 = true;
        field_0x48[port] = true;

        if (mCallback != NULL) {
            mCallback(port, this, field_0x50);
        }
    }
}
