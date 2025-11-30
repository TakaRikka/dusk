#ifndef JASTRACK_H
#define JASTRACK_H

#include "JSystem/JAudio2/JASBankTable.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASRegisterParam.h"
#include "JSystem/JAudio2/JASSeqCtrl.h"
#include "JSystem/JAudio2/JASTrackPort.h"
#include "JSystem/JGadget/linklist.h"
#include "global.h"

struct JASSoundParams;

namespace JASDsp {
    class TChannel;

    extern const u32 FILTER_MODE_IIR;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASTrack : public JASPoolAllocObject_MultiThreaded<JASTrack> {
    enum Status {
        STATUS_FREE,
        STATUS_RUN,
        STATUS_STOP,
    };

    struct TChannelMgr : public JASPoolAllocObject_MultiThreaded<TChannelMgr> {
        TChannelMgr(JASTrack*);
        void init();
        void releaseAll();
        bool noteOff(u32, u16);
        void setPauseFlag(bool);

        static const int CHANNEL_MAX = 8;

        /* 0x00 */ JASChannel* mChannels[CHANNEL_MAX];
        /* 0x20 */ JASChannelParams mParams;
        /* 0x38 */ s16 field_0x38[CHANNEL_MAX];
        /* 0x48 */ JASSoundParams* mSoundParams;
        /* 0x4c */ JASTrack* mTrack;
    };

    struct TList : JGadget::TLinkList<JASTrack, -0x240> {
        TList() : mCallbackRegistered(false) {}
        void append(JASTrack*);
        void seqMain();
        ~TList() {}

        static s32 cbSeqMain(void*);

        /* 0xC */ bool mCallbackRegistered;
    };

    struct MoveParam_ {
        // TODO: fix this on debug
#if VERSION != VERSION_SHIELD_DEBUG
        MoveParam_() : mValue(0.0f), mTarget(0.0f), mCount(0) {}
#endif

        /* 0x00 */ f32 mValue;
        /* 0x04 */ f32 mTarget;
        /* 0x08 */ u32 mCount;
    };

    JASTrack();
    ~JASTrack();
    void setChannelMgrCount(u32);
    void init();
    void initTimed();
    void inherit(JASTrack const&);
    void assignExtBuffer(u32, JASSoundParams*);
    void setSeqData(void*, u32);
    void startSeq();
    void stopSeq();
    void start();
    void close();
    bool connectChild(u32, JASTrack*);
    void closeChild(u32);
    JASTrack* openChild(u32);
    void connectBus(int, int);
    void setLatestKey(u8);
    JASChannel* channelStart(JASTrack::TChannelMgr*, u32, u32, u32);
    int noteOn(u32, u32, u32);
    int gateOn(u32, u32, f32, u32);
    int noteOff(u32, u16);
    bool checkNoteStop(u32) const;
    void overwriteOsc(JASChannel*);
    void updateTimedParam();
    void updateTrack(f32);
    void updateTempo();
    void updateSeq(bool, f32);
    u32 seqTimeToDspTime(f32);
    void setParam(u32, f32, u32);
    void noteOffAll(u16);
    void mute(bool);
    void setOscScale(u32, f32);
    void setOscTable(u32, JASOscillator::Point const*);
    void setOscAdsr(s16, s16, s16, s16, u16);
    void setFIR(s16 const*);
    void setIIR(s16 const*);
    u16 readPortSelf(u32);
    void writePortSelf(u32, u16);
    void writePort(u32, u16);
    u16 readPort(u32);
    void setChannelPauseFlag(bool);
    void pause(bool);
    int getTransposeTotal() const;
    bool isMute() const;
    void setTempo(u16);
    void setTempoRate(f32);
    void setTimebase(u16);
    void updateChannel(JASChannel*, JASDsp::TChannel*);
    JASTrack* getRootTrack();
    int tickProc();
    int seqMain();

    static void channelUpdateCallback(u32, JASChannel*, JASDsp::TChannel*, void*);

    static JASOscillator::Point const sAdsTable[4];
    static JASOscillator::Data const sEnvOsc;
    static JASOscillator::Data const sPitchEnvOsc;

    static JASDefaultBankTable sDefaultBankTable;
    static TList sTrackList;

    static const int MAX_CHILDREN = 16;

    JASSeqCtrl* getSeqCtrl() { return &mSeqCtrl; }
    u16 getPort(u32 param_0) const { return mTrackPort.get(param_0); }
    void setPort(u32 param_0, u16 param_1) { mTrackPort.set(param_0, param_1); }
    u32 checkPortIn(u32 param_0) const { return mTrackPort.checkImport(param_0); }
    u32 checkPort(u32 param_0) const { return mTrackPort.checkExport(param_0); }
    u32 readReg(JASRegisterParam::RegID param_0) { return mRegisterParam.read(param_0); }
    void writeReg(JASRegisterParam::RegID param_0, u32 param_1) { mRegisterParam.write(param_0, param_1); }
    JASTrack* getParent() { return mParent; }
    JASTrack* getChild(u32 index) { 
        JUT_ASSERT(115, index < MAX_CHILDREN)
        return mChildren[index];
    }
    int getChannelMgrCount() const { return mChannelMgrCount; }
    f32 getVibDepth() const { return mVibDepth; }
    void setVibDepth(f32 param_0) { mVibDepth = param_0; }
    f32 getVibPitch() const { return mVibPitch; }
    void setVibPitch(f32 param_0) { mVibPitch = param_0; }
    f32 getTremDepth() const { return mTremDepth; }
    void setTremDepth(f32 param_0) { mTremDepth = param_0; }
    f32 getTremPitch() const { return mTremPitch; }
    void setTremPitch(f32 param_0) { mTremPitch = param_0; }
    u32 getVibDelay() const { return mVibDelay; }
    void setVibDelay(u32 param_0) { mVibDelay = param_0; }
    u32 getTremDelay() const { return mTremDelay; }
    void setTremDelay(u32 param_0) { mTremDelay = param_0; }
    u8 getStatus() const { return mStatus; }
    void setAutoDelete(bool param_0) { mFlags.autoDelete = param_0; }
    f32 getPanPower() const { return mPanPower; }
    void setPanPower(f32 param_0) { mPanPower = param_0; }
    u32 getSkipSample() const { return mSkipSample; }
    void setSkipSample(u32 param_0) { mSkipSample = param_0; }
    u16 getDirectRelease() const { return mDirectRelease; }
    void setDirectRelease(u16 param_0) {mDirectRelease = param_0; }
    u16 getTimebase() const { return mTimebase; }
    int getTranspose() const { return mTranspose; }
    void setTranspose(s32 param_0) { mTranspose = param_0; }
    u16 getBankNumber() const { return mBankNumber; }
    void setBankNumber(u16 param_0) { mBankNumber = param_0; }
    u16 getProgNumber() const { return mProgNumber; }
    void setProgNumber(u16 param_0) { mProgNumber = param_0; }
    u8 getBendSense() const { return mBendSense; }
    void setBendSense(u8 param_0) { mBendSense = param_0; }
    u8 getNoteOnPrio() const { return mNoteOnPrio; }
    void setNoteOnPrio(u8 param_0) { mNoteOnPrio = param_0; }
    u8 getReleasePrio() const { return mReleasePrio; }
    void setReleasePrio(u8 param_0) { mReleasePrio = param_0; }
    u8 getGateRate() const { return mGateRate; }
    void setGateRate(u8 param_0) { mGateRate = param_0; }

    /* 0x000 */ JASSeqCtrl mSeqCtrl;
    /* 0x05C */ JASTrackPort mTrackPort;
    /* 0x080 */ JASRegisterParam mRegisterParam;
    /* 0x09C */ union {
        struct {
            MoveParam_ volume;
            MoveParam_ pitch;
            MoveParam_ fxmix;
            MoveParam_ pan;
            MoveParam_ dolby;
            MoveParam_ distFilter;
        } params;
        MoveParam_ array[6];
    } mMoveParam;
    /* 0x0e4 */ JASOscillator::Data mOscParam[2];
    /* 0x114 */ JASOscillator::Point mOscPoint[4];
    /* 0x12C */ JASTrack* mParent;
    /* 0x130 */ JASTrack* mChildren[MAX_CHILDREN];
    /* 0x170 */ TChannelMgr* mChannelMgrs[4];
    /* 0x180 */ TChannelMgr mDefaultChannelMgr;
    /* 0x1D0 */ int mChannelMgrCount;
    /* 0x1D4 */ const JASDefaultBankTable* mBankTable;
    /* 0x1D8 */ f32 field_0x1d8;
    /* 0x1DC */ f32 field_0x1dc;
    /* 0x1E0 */ f32 mVibDepth;
    /* 0x1E4 */ f32 mVibPitch;
    /* 0x1E8 */ f32 mTremDepth;
    /* 0x1EC */ f32 mTremPitch;
    /* 0x1F0 */ u16 mVibDelay;
    /* 0x1F2 */ u16 mTremDelay;
    /* 0x1F4 */ short mFIR[8];
    /* 0x204 */ short mIIR[8];
    /* 0x214 */ u8 mFilterMode;
    /* 0x215 */ volatile u8 mStatus;
    /* 0x216 */ struct {
        bool pause : 1;
        bool mute : 1;
        bool flag2 : 1;
        bool autoDelete : 1;
        bool flag4 : 1;
        bool flag5 : 1;
        bool flag6 : 1;
        bool flag7 : 1;
    } mFlags;
    /* 0x218 */ f32 mPanPower;
    /* 0x21C */ f32 mTempoRate;
    /* 0x220 */ u32 mSkipSample;
    /* 0x224 */ u16 mDirectRelease;
    /* 0x226 */ u16 mTempo;
    /* 0x228 */ u16 mTimebase;
    /* 0x22A */ s8 mTranspose;
    /* 0x22B */ u8 field_0x22b;
    /* 0x22C */ u16 mBankNumber;
    /* 0x22E */ u16 mProgNumber;
    /* 0x230 */ u8 mBendSense;
    /* 0x231 */ u8 mNoteOnPrio;
    /* 0x232 */ u8 mReleasePrio;
    /* 0x233 */ u8 mGateRate;
    /* 0x234 */ u16 mMixConfig[6];
    /* 0x240 */ JGadget::TLinkListNode field_0x240;
};

#endif /* JASTRACK_H */
