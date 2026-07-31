#ifndef Z2AUDIENCE_H
#define Z2AUDIENCE_H

#include "JSystem/JAudio2/JAIAudience.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JAIAudible.h"
#include "JSystem/JAudio2/JASSoundParams.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JAUAudibleParam.h"
#include "JSystem/TPosition3.h"

#define Z2_AUDIO_PLAYERS 1

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

    f32 getTargetVolume() const { return mTargetVolume; }
    f32 getCamDist() const { return mCamDist; }


    /* 0x00 */ JGeometry::TPosition3f32 mViewMatrix;
    /* 0x30 */ JGeometry::TVec3<f32> mVel;
    /* 0x3C */ JGeometry::TVec3<f32> mPos;

    /**
     *
     */
    /* 0x48 */ JGeometry::TVec3<f32> mLastPos;
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

    void calcPriorityFactor();
    void setIgnoreIfOut(bool value) { mIgnoreIfOut = value; }
    void setMicOn(bool value) { mMicOn = value; }

    void setPosPtr(Vec* posPtr) { mPosPtr = posPtr; }
    bool isOn() { return mMicOn; }

    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ Z2AudioCamera* field_0x10[Z2_AUDIO_PLAYERS];
    /* 0x14 */ Vec* mPosPtr;
    /* 0x18 */ f32 field_0x18[Z2_AUDIO_PLAYERS];
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20[Z2_AUDIO_PLAYERS];
    /* 0x24 */ bool mIgnoreIfOut;
    /* 0x25 */ bool mMicOn;
    /* 0x26 */ bool field_0x26[Z2_AUDIO_PLAYERS];
};  // Size: 0x28

struct Z2Audience3DSetting {
    Z2Audience3DSetting();
    void init();
    void initVolumeDist();
    void updateVolumeDist(f32);
    void initDolbyDist();
    void updateDolbyDist(f32, f32);

    void calcVolumeFactorAll() {
        mDistanceMaxes[1] = 1.25f * mDistanceMaxes[0];
        mDistanceMaxes[2] = 1.5f * mDistanceMaxes[0];
        mDistanceMaxes[3] = 2.0f * mDistanceMaxes[0];
        mDistanceMaxes[4] = 3.0f * mDistanceMaxes[0];
        mDistanceMaxes[5] = 4.0f * mDistanceMaxes[0];
        mDistanceMaxes[6] = 6.0f * mDistanceMaxes[0];
        mDistanceMaxes[7] = 8.0f * mDistanceMaxes[0];
        mDistanceMaxes[8] = 0.9f * mDistanceMaxes[0];
        mDistanceMaxes[9] = 0.8f * mDistanceMaxes[0];
        mDistanceMaxes[10] = 0.7f * mDistanceMaxes[0];
        mDistanceMaxes[11] = 0.6f * mDistanceMaxes[0];
        mDistanceMaxes[12] = 0.5f * mDistanceMaxes[0];
        mDistanceMaxes[13] = 0.4f * mDistanceMaxes[0];
        mDistanceMaxes[14] = 0.3f * mDistanceMaxes[0];
        for (int i = 0; i < 15; i++) {
            mVolumeFactor[i] = (mMinDistanceVolume - 1.0f) / (mDistanceMaxes[i] - mMaxVolumeDistance);
        }
    }

    void calcPriorityFactorAll() {
        for (int i = 0; i < 15; i++) {
            mPriorityFactor[i] = mMaxDistancePriority / (mDistanceMaxes[i] - mMaxVolumeDistance);
        }
    }

    void calcFxMixFactorAll() {
        for (int i = 0; i < 15; i++) {
            mFxMixFactor[i] = (mMaxDistanceFxMix - mMinDistanceFxMix) / (mDistanceMaxes[i] - mMaxVolumeDistance);
        }
    }

    /**
     * Maximum distance a sound can reach before being "far away"
     * Being far away affects stuff like culling, lowering its priority, forcibly stopping it, etc.
     * Sounds select which entry they use based on their VolBits.
     */
    /* 0x000 */ f32 mDistanceMaxes[15];

    /**
     * Distance at which the max volume of a sound is reached.
     * i.e. sounds do *not* get louder if they get closer than this.
     */
    /* 0x03C */ f32 mMaxVolumeDistance;

    /**
     * FX Mix value at maximum distance (@ref mDistanceMaxes)
     */
    /* 0x040 */ f32 mMinDistanceVolume;
    /* 0x044 */ f32 mDolbyFrontDistanceMax;
    /* 0x048 */ f32 mDolbyBehindDistanceMax;
    /* 0x04C */ f32 mDolbyCenterValue;

    /**
     * FX Mix value at minimum distance (@ref mMaxVolumeDistance)
     */
    /* 0x050 */ f32 mMinDistanceFxMix;

    /**
     * FX Mix value at maximum distance (@ref mDistanceMaxes)
     */
    /* 0x054 */ f32 mMaxDistanceFxMix;
    /* 0x058 */ f32 mPanFactor;
    /* 0x05C */ f32 mSonicSpeed; // Used for doppler effect calculations.
    /* 0x060 */ f32 field_0x60;

    /**
     * Priority that sounds receive when "far away".
     * @see mDistanceMaxes
     */
    /* 0x064 */ u32 mMaxDistancePriority;
    /* 0x068 */ f32 field_0x68;
    /* 0x06C */ f32 field_0x6c;
    /* 0x070 */ f32 mVolumeFactor[15];
    /* 0x0AC */ f32 mPriorityFactor[15];
    /* 0x0E8 */ f32 mFxMixFactor[15];
    /* 0x124 */ bool mVolumeDistInit;
    /* 0x125 */ bool mDolbyDistInit;
};  // Size: 0x128

struct Z2AudibleRelPos {
    /* 0x00 */ JGeometry::TVec3<f32> mCameraRelative;

    /**
     * Distance from mCameraRelative. This is from the object root and not the
     * exact distance used for volume/priority calculations.
     */
    /* 0x0C */ f32 mTrueDistance;

    /**
     * Distance from mCameraRelative but offset by mVolCenterZ.
     * This presumably means the distance is more centered on the object than mTrueDistance.
     */
    /* 0x10 */ f32 mCenterDistance;
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
    /* 0x14 */ Z2AudibleRelPos mRelPos;
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
    /* 0x2C */ Z2AudibleChannel mChannel[Z2_AUDIO_PLAYERS];
    /* 0x64 */ f32 mMicDistances[Z2_AUDIO_PLAYERS];
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

    bool isActive() const;

    Z2SpotMic* getLinkMic() { return mLinkMic; }
    JGeometry::TVec3<f32> getAudioCamPos(int camID) {
        return *mAudioCamera[camID].getPos();
    }
    Z2Audience3DSetting* getSetting() { return &mSetting; }

    const Z2AudioCamera* getAudioCamera(int camID) const { return &mAudioCamera[camID]; } 

    void setUsingOffMicVol(bool value) { mUsingOffMicVol = value; }

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

DUSK_GAME_EXTERN s8 data_80451358;
DUSK_GAME_EXTERN s8 data_80451359;

#endif /* Z2AUDIENCE_H */
