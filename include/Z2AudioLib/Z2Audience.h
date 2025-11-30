#ifndef Z2AUDIENCE_H
#define Z2AUDIENCE_H

#include "JSystem/JAudio2/JAIAudience.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JAIAudible.h"
#include "JSystem/JAudio2/JASSoundParams.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JAUAudibleParam.h"
#include "JSystem/TPosition3.h"

struct Z2Audible;

struct Z2AudibleAbsPos {
    void calc(const JGeometry::TVec3<f32>& pos);
    void init(JGeometry::TVec3<f32>*, const JGeometry::TVec3<f32>&,
                             const JGeometry::TVec3<f32>*);

    /* 0x00 */ JGeometry::TVec3<f32> field_0x0;
    /* 0x0C */ JGeometry::TVec3<f32> velocity_;
};

struct Z2AudioCamera {
    Z2AudioCamera();
    void init();
    void setCameraState(f32 (*)[4], Vec& pos, Vec&, f32, f32, bool, bool);
    void setCameraState(f32 const (*)[4], Vec& pos, bool);
    void convertAbsToRel(Z2Audible* audible, int channelNum);
    bool convertAbsToRel(Vec& src, Vec* dst) const;
    bool isInSight(Vec&) const;

    const JGeometry::TVec3<f32>* getPos() const { return &mPos; }
    f32 getVolCenterZ() const { return mVolCenterZ; }
    void setMainCamera(bool param_0) { mSetMainCamera = param_0; }

    void setTargetVolume(f32 vol) { 
        JUT_ASSERT(281, vol <= 1.f);
        if (vol < 0.0f) {
            vol = 0.0f;
        }
        mTargetVolume = vol;
    }

    f32 getDolbyCenterZ() const { return mDolbyCenterZ; }
    f32 getFovySin() const { return mFovySin; }
    const JGeometry::TVec3<f32>* getVel() const { return &mVel; }

    /* 0x00 */ JGeometry::TPosition3f32 field_0x0;
    /* 0x30 */ JGeometry::TVec3<f32> mVel;
    /* 0x3C */ JGeometry::TVec3<f32> mPos;
    /* 0x48 */ JGeometry::TVec3<f32> field_0x48;
    /* 0x54 */ f32 mFovySin;
    /* 0x58 */ f32 mVolCenterZ;
    /* 0x5C */ f32 mTargetVolume;
    /* 0x60 */ f32 mDolbyCenterZ;
    /* 0x64 */ f32 mCamDist;
    /* 0x68 */ f32 field_0x68;
    /* 0x6C */ f32 field_0x6c;
    /* 0x70 */ bool mSetMainCamera;
};  // Size: 0x74

struct Z2SpotMic {
    Z2SpotMic();
    void clearMicState(int camID);
    void calcVolumeFactor(int camID);
    void setMicState(Z2AudioCamera* camera, int camID);
    f32 calcMicDist(Z2Audible* audible);
    u32 calcMicPriority(f32);
    f32 calcMicVolume(f32, int camID, f32);

    void setPosPtr(Vec* posPtr) { mPosPtr = posPtr; }
    bool isOn() { return mMicOn; }

    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ Z2AudioCamera* field_0x10[1];
    /* 0x14 */ Vec* mPosPtr;
    /* 0x18 */ f32 field_0x18[1];
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20[1];
    /* 0x24 */ bool mIgnoreIfOut;
    /* 0x25 */ bool mMicOn;
    /* 0x26 */ bool field_0x26[1];
};  // Size: 0x28

struct Z2Audience3DSetting {
    Z2Audience3DSetting();
    void init();
    void initVolumeDist();
    void updateVolumeDist(f32);
    void initDolbyDist();
    void updateDolbyDist(f32, f32);

    void calcVolumeFactorAll() {
        field_0x0[1] = 1.25f * field_0x0[0];
        field_0x0[2] = 1.5f * field_0x0[0];
        field_0x0[3] = 2.0f * field_0x0[0];
        field_0x0[4] = 3.0f * field_0x0[0];
        field_0x0[5] = 4.0f * field_0x0[0];
        field_0x0[6] = 6.0f * field_0x0[0];
        field_0x0[7] = 8.0f * field_0x0[0];
        field_0x0[8] = 0.9f * field_0x0[0];
        field_0x0[9] = 0.8f * field_0x0[0];
        field_0x0[10] = 0.7f * field_0x0[0];
        field_0x0[11] = 0.6f * field_0x0[0];
        field_0x0[12] = 0.5f * field_0x0[0];
        field_0x0[13] = 0.4f * field_0x0[0];
        field_0x0[14] = 0.3f * field_0x0[0];
        for (int i = 0; i < 15; i++) {
            field_0x70[i] = (field_0x40 - 1.0f) / (field_0x0[i] - field_0x3c);
        }
    }

    void calcPriorityFactorAll() {
        for (int i = 0; i < 15; i++) {
            field_0xac[i] = field_0x64 / (field_0x0[i] - field_0x3c);
        }
    }

    void calcFxMixFactorAll() {
        for (int i = 0; i < 15; i++) {
            field_0xe8[i] = (field_0x54 - field_0x50) / (field_0x0[i] - field_0x3c);
        }
    }

    /* 0x000 */ f32 field_0x0[15];
    /* 0x03C */ f32 field_0x3c;
    /* 0x040 */ f32 field_0x40;
    /* 0x044 */ f32 field_0x44;
    /* 0x048 */ f32 field_0x48;
    /* 0x04C */ f32 field_0x4c;
    /* 0x050 */ f32 field_0x50;
    /* 0x054 */ f32 field_0x54;
    /* 0x058 */ f32 field_0x58;
    /* 0x05C */ f32 field_0x5c;
    /* 0x060 */ f32 field_0x60;
    /* 0x064 */ u32 field_0x64;
    /* 0x068 */ f32 field_0x68;
    /* 0x06C */ f32 field_0x6c;
    /* 0x070 */ f32 field_0x70[15];
    /* 0x0AC */ f32 field_0xac[15];
    /* 0x0E8 */ f32 field_0xe8[15];
    /* 0x124 */ bool mVolumeDistInit;
    /* 0x125 */ bool mDolbyDistInit;
};  // Size: 0x128

struct Z2AudibleRelPos {
    /* 0x00 */ JGeometry::TVec3<f32> field_0x00;
    /* 0x0C */ f32 field_0xC;
    /* 0x10 */ f32 field_0x10;
};

struct Z2AudibleChannel {
    Z2AudibleChannel();
    void init() {
        mParams.init();
        field_0x28 = -1.0f;
        mPan = 0.5f;
        mDolby = 0.0f;
        field_0x34 = 1.0f;
    }

    /* 0x00 */ JASSoundParams mParams;
    /* 0x14 */ Z2AudibleRelPos field_0x14;
    /* 0x28 */ f32 field_0x28;
    /* 0x2c */ f32 mPan;
    /* 0x30 */ f32 mDolby;
    /* 0x34 */ f32 field_0x34;
};

struct Z2Audible : public JAIAudible, public JASPoolAllocObject<Z2Audible> {
    Z2Audible(const JGeometry::TVec3<f32>& pos, const JGeometry::TVec3<f32>*, u32 channel, bool);
    void calc();
    JASSoundParams* getOuterParams(int index);
    void setOuterParams(const JASSoundParams& outParams, const JASSoundParams& inParams, int index);
    Z2AudibleChannel* getChannel(int index);
    u32 getDistVolBit();
    ~Z2Audible();

    bool isDoppler() {
        return ((*(u8*)&mParam.field_0x0) >> 4) & 0xf;
    }

    const JGeometry::TVec3<f32>& getPos() const { return mPos; }

    JAUAudibleParam* getAudibleParam() { return &mParam; }
    const JAUAudibleParam* getAudibleParam() const { return &mParam; }
    void setAudibleParam(JAUAudibleParam param) { mParam.field_0x0.raw = param.field_0x0.raw; }
    const JGeometry::TVec3<f32>* getVel() const { return &mAbsPos.velocity_; }

    /* 0x10 */ JAUAudibleParam mParam;
    /* 0x14 */ Z2AudibleAbsPos mAbsPos;
    /* 0x2C */ Z2AudibleChannel mChannel[1];
    /* 0x64 */ f32 field_0x64[1];
};

struct Z2Audience : public JAIAudience, public JASGlobalInstance<Z2Audience> {
    Z2Audience();
    void setAudioCamera(f32 (*)[4], Vec&, Vec&, f32, f32, bool, int camID, bool);
    f32 calcOffMicSound(f32);
    void setTargetVolume(f32 volume, int index);
    bool convertAbsToRel(Vec& src, Vec* dst, int camID);
    f32 calcRelPosVolume(const Vec&, f32, int camID);
    f32 calcRelPosPan(const Vec&, int camID);
    f32 calcRelPosDolby(const Vec&, int camID);
    f32 calcVolume_(f32, int distVolBit) const;
    u32 calcDeltaPriority_(f32, int distVolBit, bool) const;
    f32 calcPitchDoppler_(const JGeometry::TVec3<f32>&,
                                          const JGeometry::TVec3<f32>&,
                                          const JGeometry::TVec3<f32>&, f32) const;
    f32 calcFxMix_(f32, int distVolBit) const;
    f32 calcPitch_(Z2AudibleChannel* channel, const Z2Audible* audible, const Z2AudioCamera* camera) const;

    virtual ~Z2Audience();
    virtual JAIAudible* newAudible(const JGeometry::TVec3<f32>& pos, JAISoundID soundID,
                                                  const JGeometry::TVec3<f32>*, u32);
    virtual int getMaxChannels();
    virtual void deleteAudible(JAIAudible* audible);
    virtual u32 calcPriority(JAIAudible* audible);
    virtual void mixChannelOut(const JASSoundParams& outParams, JAIAudible* audible, int channelNum);

    Z2SpotMic* getLinkMic() { return mLinkMic; }
    JGeometry::TVec3<f32> getAudioCamPos(int camID) {
        return *mAudioCamera[camID].getPos();
    }
    Z2Audience3DSetting* getSetting() { return &mSetting; }

    const Z2AudioCamera* getAudioCamera(int camID) const { return &mAudioCamera[camID]; } 

    /* 0x004 */ f32 field_0x4;
    /* 0x008 */ u8 field_0x8;
    /* 0x00C */ Z2Audience3DSetting mSetting;
    /* 0x134 */ Z2AudioCamera mAudioCamera[1];
    /* 0x1A8 */ Z2SpotMic mSpotMic[1];
    /* 0x1D0 */ Z2SpotMic* mLinkMic;
    /* 0x1D4 */ s32 mNumPlayers;
    /* 0x1D8 */ u8 field_0x1d8[4];
    /* 0x1DC */ bool mUsingOffMicVol;
};  // Size: 0x1E0

inline Z2Audience* Z2GetAudience() {
    return JASGlobalInstance<Z2Audience>::getInstance();
}

extern s8 data_80451358;
extern s8 data_80451359;

#endif /* Z2AUDIENCE_H */
