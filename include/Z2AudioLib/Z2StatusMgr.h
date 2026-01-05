#ifndef Z2STATUSMGR_H
#define Z2STATUSMGR_H

#include "JSystem/JAudio2/JASGadget.h"
#include <dolphin/mtx.h>

struct Z2StatusMgr : public JASGlobalInstance<Z2StatusMgr> {
    Z2StatusMgr();
    void heartGaugeOn();
    void processHeartGaugeSound();
    void talkIn();
    void talkOut();
    void menuIn();
    void menuOut();
    bool isMovieDemo();
    void setDemoName(char* demoName);
    void processTime();
    bool checkDayTime();
    void setEventBit(void* eventBit);
    void setCameraPolygonPos(Vec* polygonPos);
    void setCameraGroupInfo(u8);
    void setCameraInWaterDepth(f32 depth);

    u8 getDemoStatus() { return mDemoStatus; }
    void setHour(s32 hour) { mHour = hour; }
    void setMinute(s32 min) { mMinute = min; }
    void setWeekday(s32 day) { mWeekday = day; }
    f32 getCameraInWaterDepth() { return mUnderwaterDepth; }
    f32 getCameraInWaterDepthRatio() { return mCameraInWaterDepthRatio; }
    u8 getHour() { return mHour; }
    u8 isPaused() { return mPauseFlag; }
    bool isHeartGuageOn() { return mHeartGaugeOn; }
    void getCameraMapInfo(u32 info) { mCameraMapInfo = info; }
    void setPauseFlag(u8 flag) { mPauseFlag = flag; }

    /* 0x00 */ u8 mHour;
    /* 0x01 */ u8 mMinute;
    /* 0x02 */ u8 mWeekday;
    /* 0x03 */ u8 field_0x03;
    /* 0x04 */ u16 mTime;
    /* 0x08 */ void* mEventBit;
    /* 0x0C */ u8 mPauseFlag;
    /* 0x10 */ u32 mCameraMapInfo;
    /* 0x14 */ u32 field_0x14;
    /* 0x18 */ f32 mUnderwaterDepth;
    /* 0x1C */ f32 mCameraInWaterDepthRatio;
    /* 0x20 */ Vec mPolygonPosition;
    /* 0x2C */ u8 mDemoStatus;
    /* 0x2D */ u8 mHeartGaugeOn;
};  // Size = 0x30

inline Z2StatusMgr* Z2GetStatusMgr() {
    return JASGlobalInstance<Z2StatusMgr>::getInstance();
}

#endif /* Z2STATUSMGR_H */
