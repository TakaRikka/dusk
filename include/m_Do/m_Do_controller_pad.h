#ifndef M_DO_M_DO_CONTROLLER_PAD_H
#define M_DO_M_DO_CONTROLLER_PAD_H

#include "JSystem/JUtility/JUTGamePad.h"
#include "SSystem/SComponent/c_API_controller_pad.h"

// Controller Ports 1 - 4
enum { PAD_1, PAD_2, PAD_3, PAD_4 };

class mDoCPd_c {
public:
    static void create();
    static void read();
    static void convert(interface_of_controller_pad*, JUTGamePad*);
    static void LRlockCheck(interface_of_controller_pad*);
    static void recalibrate();

    static interface_of_controller_pad& getCpadInfo(u32 pad) { return m_cpadInfo[pad]; }
    static JUTGamePad* getGamePad(u32 pad) { return m_gamePad[pad]; }
    static u32 getTrig(u32 pad) { return getCpadInfo(pad).mPressedButtonFlags; }
    static u32 getTrigLockL(u32 pad) { return getCpadInfo(pad).mTrigLockL; }
    static u32 getTrigLockR(u32 pad) { return getCpadInfo(pad).mTrigLockR; }
    static u32 getTrigUp(u32 pad) { return getTrig(pad) & PAD_BUTTON_UP; }
    static u32 getTrigDown(u32 pad) { return getTrig(pad) & PAD_BUTTON_DOWN; }
    static u32 getTrigLeft(u32 pad) { return getTrig(pad) & PAD_BUTTON_LEFT; }
    static u32 getTrigRight(u32 pad) { return getTrig(pad) & PAD_BUTTON_RIGHT; }
    static u32 getTrigL(u32 pad) { return getTrig(pad) & PAD_TRIGGER_L; }
    static u32 getTrigR(u32 pad) { return getTrig(pad) & PAD_TRIGGER_R; }
    static u32 getTrigA(u32 pad) { return getTrig(pad) & PAD_BUTTON_A; }
    static u32 getTrigB(u32 pad) { return getTrig(pad) & PAD_BUTTON_B; }
    static u32 getTrigZ(u32 pad) { return getTrig(pad) & PAD_TRIGGER_Z; }
    static u32 getTrigY(u32 pad) { return getTrig(pad) & PAD_BUTTON_Y; }
    static u32 getTrigX(u32 pad) { return getTrig(pad) & PAD_BUTTON_X; }
    static u32 getTrigStart(u32 pad) { return getTrig(pad) & PAD_BUTTON_START; }
    static u32 getHold(u32 pad) { return getCpadInfo(pad).mButtonFlags; }
    static u32 getHoldLockL(u32 pad) { return getCpadInfo(pad).mHoldLockL; }
    static u32 getHoldLockR(u32 pad) { return getCpadInfo(pad).mHoldLockR; }
    static u32 getHoldUp(u32 pad) { return getHold(pad) & PAD_BUTTON_UP; }
    static u32 getHoldDown(u32 pad) { return getHold(pad) & PAD_BUTTON_DOWN; }
    static u32 getHoldLeft(u32 pad) { return getHold(pad) & PAD_BUTTON_LEFT; }
    static u32 getHoldRight(u32 pad) { return getHold(pad) & PAD_BUTTON_RIGHT; }
    static u32 getHoldL(u32 pad) { return getHold(pad) & PAD_TRIGGER_L; }
    static u32 getHoldR(u32 pad) { return getHold(pad) & PAD_TRIGGER_R; }
    static u32 getHoldA(u32 pad) { return getHold(pad) & PAD_BUTTON_A; }
    static u32 getHoldB(u32 pad) { return getHold(pad) & PAD_BUTTON_B; }
    static u32 getHoldZ(u32 pad) { return getHold(pad) & PAD_TRIGGER_Z; }
    static u32 getHoldY(u32 pad) { return getHold(pad) & PAD_BUTTON_Y; }
    static u32 getHoldX(u32 pad) { return getHold(pad) & PAD_BUTTON_X; }
    static f32 getStickX(u32 pad) { return getCpadInfo(pad).mMainStickPosX; }
    static f32 getStickY(u32 pad) { return getCpadInfo(pad).mMainStickPosY; }
    static f32 getStickX3D(u32 pad) { return getCpadInfo(pad).mMainStickPosX; }
    static f32 getStickValue(u32 pad) { return getCpadInfo(pad).mMainStickValue; }
    static s16 getStickAngle(u32 pad) { return getCpadInfo(pad).mMainStickAngle; }
    static s16 getStickAngle3D(u32 pad) { return getCpadInfo(pad).mMainStickAngle; }
    static f32 getSubStickX3D(u32 pad) { return getCpadInfo(pad).mCStickPosX; }
    static f32 getSubStickX(u32 pad) { return getCpadInfo(pad).mCStickPosX; }
    static f32 getSubStickY(u32 pad) { return getCpadInfo(pad).mCStickPosY; }
    static f32 getSubStickValue(u32 pad) { return getCpadInfo(pad).mCStickValue; }
    static s16 getSubStickAngle(u32 pad) { return getCpadInfo(pad).mCStickAngle; }
    static f32 getAnalogR(u32 pad) { return getCpadInfo(pad).mTriggerRight; }
    static f32 getAnalogL(u32 pad) { return getCpadInfo(pad).mTriggerLeft; }
    static BOOL isConnect(u32 pad) { return JUTGamePad::getPortStatus((JUTGamePad::EPadPort)pad) == 0; }
    static void startMotorWave(u32 pad, void* data, JUTGamePad::CRumble::ERumble rumble, u32 length) {
        m_gamePad[pad]->startMotorWave(data, rumble, length);
    }
    static void stopMotor(u32 pad) { m_gamePad[pad]->stopMotor(); }
    static void stopMotorWave(u32 pad) { m_gamePad[pad]->stopMotorWave(); }
    static void stopMotorHard(u32 pad) { return m_gamePad[pad]->stopMotorHard(); }
    static void stopMotorWaveHard(u32 pad) { return m_gamePad[pad]->stopMotorWaveHard(); }

    static JUTGamePad* m_gamePad[4];
    static interface_of_controller_pad m_cpadInfo[4];
    static interface_of_controller_pad m_debugCpadInfo[4];
};

inline void mDoCPd_ANALOG_CONV(u8 analog, f32& param_1) {
    param_1 = analog * (1.0f / 15.0f);
    if (param_1 > 1.0f) {
        param_1 = 1.0f;
    }
}

inline void mDoCPd_TRIGGER_CONV(u8 analog, f32& param_1) {
    param_1 = analog * 0.0071428571827709675f;
    if (param_1 > 1.0f) {
        param_1 = 1.0f;
    }
}

#endif /* M_DO_M_DO_CONTROLLER_PAD_H */
