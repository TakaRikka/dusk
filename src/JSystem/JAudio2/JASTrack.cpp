#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASTrack.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASBank.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASDSPInterface.h"

JASTrack::JASTrack() : mDefaultChannelMgr(this), mChannelMgrCount(1), mStatus(0) {
    mChannelMgrs[0] = &mDefaultChannelMgr;
    for (int i = 1; i < 4; i++) {
        mChannelMgrs[i] = NULL;
    }
    init();
}

JASDefaultBankTable JASTrack::sDefaultBankTable;

JASTrack::TList JASTrack::sTrackList;

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
JASTrack::~JASTrack() {
    JUT_ASSERT(70, mStatus != STATUS_RUN);
    for (int i = 1; i < 4; i++) {
        delete mChannelMgrs[i];
    }
}

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
void JASTrack::setChannelMgrCount(u32 count) {
    JUT_ASSERT(79, mStatus != STATUS_RUN);
    JUT_ASSERT(80, count >= 1 && count <= CHANNEL_MGR_MAX);
    mChannelMgrCount = 1;
    for (int i = 1; i < count; i++) {
        if (mChannelMgrs[i] == NULL) {
            mChannelMgrs[i] = new TChannelMgr(this);
            if (mChannelMgrs[i] == NULL) {
                JUT_WARN(87, "%s", "Not enough JASTrackChannelMgr\n");
                return;
            }
        }
        mChannelMgrCount++;
    }
    for (u32 i = mChannelMgrCount; i < 4; i++) {
        if (mChannelMgrs[i] != NULL) {
            delete mChannelMgrs[i];
            mChannelMgrs[i] = NULL;
        }
    }
}

JASOscillator::Point const JASTrack::sAdsTable[4] = {
    {0, 0, 0x7fff},
    {0, 0, 0x7fff},
    {0, 0, 0},
    {0xe, 0, 0},
};

JASOscillator::Data const JASTrack::sEnvOsc = {0, 1.0f, NULL, NULL, 1.0f, 0.0f};

JASOscillator::Data const JASTrack::sPitchEnvOsc = {1, 1.0f, NULL, NULL, 1.0f, 0.0f};

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
void JASTrack::init() {
    JUT_ASSERT(104, mStatus == STATUS_FREE || mStatus == STATUS_STOPPED);
    mSeqCtrl.init();
    mTrackPort.init();
    initTimed();
    mRegisterParam.init();
    mOscParam[0] = sEnvOsc;
    mOscParam[1] = sPitchEnvOsc;
    for (int i = 0; i < 4; i++) {
        mOscPoint[i] = sAdsTable[i];
    }
    mParent = NULL;
    for (int i = 0; i < 16; i++) {
        mChildren[i] = NULL;
    }
    mChannelMgrs[0]->init();
    mChannelMgrCount = 1;
    for (int i = 1; i < 4; i++) {
        if (mChannelMgrs[i] != NULL) {
            delete mChannelMgrs[i];
            mChannelMgrs[i] = NULL;
        }
    }
    mBankTable = &sDefaultBankTable;
    field_0x1d8 = 1.0f;
    field_0x1dc = 1.0f;
    mVibDepth = 0.0f;
    mVibPitch = 1.0f;
    mTremDepth = 0.0f;
    mTremPitch = 1.0f;
    mVibDelay = 0;
    mTremDelay = 0;
    mPanPower = 1.0f;
    mDirectRelease = 0;
    mSkipSample = 0;
    mTempo = 120;
    mTimebase = 0x30;
    mTempoRate = 1.0f;
    updateTempo();
    mTranspose = 0;
    field_0x22b = 60;
    mBankNumber = 0;
    mProgNumber = 0xf0;
    mBendSense = 0xc;
    mNoteOnPrio = 0x40;
    mReleasePrio = 0;
    mGateRate = 100;
    mMixConfig[0] = 0x150;
    mMixConfig[1] = 0x210;
    mMixConfig[2] = 0x352;
    mMixConfig[3] = 0x412;
    mMixConfig[4] = 0;
    mMixConfig[5] = 0;
    for (int i = 0; i < 8; i++) {
        mFIR[i] = 0;
    }
    mFIR[0] = 0x7fff;
    for (int i = 0; i < 8; i++) {
        mIIR[i] = 0;
    }
    mIIR[0] = 0x7fff;
    mFilterMode = 0;
    mFlags.pause = false;
    mFlags.mute = false;
    mFlags.flag2 = true;
    mFlags.flag5 = true;
    mFlags.autoDelete = false;
    mFlags.flag4 = false;
    mFlags.flag6 = false;
    mStatus = STATUS_FREE;
}

void JASTrack::initTimed() {
    mMoveParam.params.volume.mValue = 1.0f;
    mMoveParam.params.pitch.mValue = 0.0f;
    mMoveParam.params.pan.mValue = 0.5f;
    mMoveParam.params.fxmix.mValue = 0.0f;
    mMoveParam.params.dolby.mValue = 0.0f;
    mMoveParam.params.distFilter.mValue = 0.0f;
    for (int i = 0; i < 6; i++) {
        mMoveParam.array[i].mCount = 0;
        mMoveParam.array[i].mTarget = mMoveParam.array[i].mValue;
    }
}

void JASTrack::inherit(JASTrack const& i_parent) {
    mFlags.flag2 = i_parent.mFlags.flag2;
    mBankNumber = i_parent.mBankNumber;
    mProgNumber = i_parent.mProgNumber;
    mNoteOnPrio = i_parent.mNoteOnPrio;
    mReleasePrio = i_parent.mReleasePrio;
    mSkipSample = i_parent.mSkipSample;
    for (int i = 0; i < 8; i++) {
        mFIR[i] = i_parent.mFIR[i];
    }
    for (int i = 0; i < 8; i++) {
        mIIR[i] = i_parent.mIIR[i];
    }
    mFilterMode = i_parent.mFilterMode;
    for (int i = 0; i < 6; i++) {
        mMixConfig[i] = i_parent.mMixConfig[i];
    }
}

void JASTrack::assignExtBuffer(u32 index, JASSoundParams* i_soundParams) {
    JUT_ASSERT(249, index < mChannelMgrCount);
    mChannelMgrs[index]->mSoundParams = i_soundParams;
}

void JASTrack::setSeqData(void* param_0, u32 param_1) {
    JUT_ASSERT(257, mStatus == STATUS_FREE);
    mSeqCtrl.start(param_0, param_1);
}

void JASTrack::startSeq() {
    JUT_ASSERT(265, mParent == 0);
    JUT_ASSERT(266, mStatus == STATUS_FREE);
    JASCriticalSection critical_section;
    sTrackList.append(this);
    mStatus = STATUS_RUN;
}

void JASTrack::stopSeq() {
    JASCriticalSection critical_section;
    mFlags.flag6 = 1;
}

void JASTrack::start() {
    JUT_ASSERT(289, mParent != NULL);
    JUT_ASSERT(290, mStatus == STATUS_FREE);
    mStatus = STATUS_RUN;
}

void JASTrack::close() {
    for (int i = 0; i < 16; i++) {
        JASTrack* child = mChildren[i];
        if (child != NULL) {
            child->close();
            if (child->mFlags.autoDelete) {
                delete child;
                mChildren[i] = NULL;
            }
        }
    }
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL) {
            mChannelMgrs[i]->releaseAll();
        }
    }
    mParent = NULL;
    mStatus = STATUS_STOPPED;
}

bool JASTrack::connectChild(u32 track_no, JASTrack* i_child) {
    JUT_ASSERT(326, track_no < MAX_CHILDREN);
    if (mChildren[track_no] != NULL) {
        return false;
    }
    i_child->mParent = this;
    mChildren[track_no] = i_child;
    return true;
}

void JASTrack::closeChild(u32 track_no) {
    JUT_ASSERT(340, track_no < MAX_CHILDREN);
    JASTrack* child = mChildren[track_no];
    if (child != NULL) {
        getRootTrack()->updateSeq(false, 1.0f);
        child->close();
        if (child->mFlags.autoDelete) {
            delete child;
            mChildren[track_no] = NULL;
        }
    }
}

// NONMATCHING JASPoolAllocObject_MultiThreaded<_> locations
JASTrack* JASTrack::openChild(u32 trk_no) {
    JUT_ASSERT(358, trk_no < MAX_CHILDREN);
    JASTrack* child = mChildren[trk_no];
    if (child != NULL) {
        switch (child->mStatus) {
        case STATUS_FREE:
            break;
        case STATUS_RUN:
            getRootTrack()->updateSeq(false, 1.0f);
            child->close();
            // no break
        case STATUS_STOPPED:
            bool auto_delete = child->mFlags.autoDelete;
            child->init();
            child->mFlags.autoDelete = auto_delete;
            mChildren[trk_no] = NULL;
            connectChild(trk_no, child);
            break;
        }
    } else {
        child = new JASTrack();
        if (child == NULL) {
            JUT_WARN(388, "%s", "Not enough JASTrack\n");
            return NULL;
        }
        child->mFlags.autoDelete = true;
        connectChild(trk_no, child);
    }
    child->setChannelMgrCount(mChannelMgrCount);
    child->inherit(*this);
    return child;
}

void JASTrack::connectBus(int line, int param_1) {
    JUT_ASSERT(404, line < JASChannel::BUSOUT_CPUCH);
    mMixConfig[line] = param_1;
}

f32 JASTrack::getVolume() const { return mMoveParam.params.volume.mValue; }
f32 JASTrack::getPitch() const { return mMoveParam.params.pitch.mValue; }
f32 JASTrack::getPan() const { return mMoveParam.params.pan.mValue; }
f32 JASTrack::getFxmix() const { return mMoveParam.params.fxmix.mValue; }
f32 JASTrack::getDolby() const { return mMoveParam.params.dolby.mValue; }

void JASTrack::setLatestKey(u8 param_0) {
    field_0x22b = param_0;
    field_0x22b += getTransposeTotal();
}

JASChannel* JASTrack::channelStart(JASTrack::TChannelMgr* i_channelMgr, u32 param_1, u32 param_2,
                                   u32 i_updateTimer) {
    JASBank* bank = NULL;
    if (mBankTable != NULL) {
        bank = mBankTable->getBank(mBankNumber);
    }
    JASChannel* channel = JASBank::noteOn(bank, mProgNumber, param_1, param_2,
                                          mNoteOnPrio | mReleasePrio << 8,
                                          channelUpdateCallback, i_channelMgr);
    if (channel == NULL) {
        return NULL;
    }
    channel->setUpdateTimer(i_updateTimer);
    channel->setSkipSamples(mSkipSample);
    channel->setVibrateDelay(mVibDelay);
    channel->setTremoloDelay(mTremDelay);
    channel->setParams(i_channelMgr->mParams);
    for (u32 i = 0; i < 6; i++) {
        channel->setMixConfig(i, mMixConfig[i]);
    }
    overwriteOsc(channel);
    if (mDirectRelease != 0) {
        channel->setDirectRelease(mDirectRelease);
    }
    return channel;
}

int JASTrack::noteOn(u32 noteid, u32 param_1, u32 param_2) {
    JUT_ASSERT(486, noteid != 0 && noteid < TChannelMgr::CHANNEL_MAX);
    if (isMute()) {
        return 0;
    }
    int ret = 1;
    param_1 += getTransposeTotal();
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL) {
            mChannelMgrs[i]->noteOff(noteid, 0);
            JASChannel* channel = channelStart(mChannelMgrs[i], param_1, param_2, 0);
            if (channel == NULL) {
                ret = 0;
            }
            mChannelMgrs[i]->mChannels[noteid] = channel;
        }
    }
    return ret;
}

int JASTrack::gateOn(u32 param_0, u32 i_velocity, f32 i_time, u32 i_flags) {
    bool result = true;
    param_0 += getTransposeTotal();
    if (mGateRate != 100) {
        i_time *= mGateRate / 100.0f;
    }
    u32 uvar2 = seqTimeToDspTime(i_time);
    u32 update_timer = (i_flags & 6) ? 0 : uvar2;
    int uvar7 = i_flags & 1 ? field_0x22b : param_0;
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* channel_mgr = mChannelMgrs[i];
        if (channel_mgr != NULL) {
            if (!mFlags.flag4) {
                channel_mgr->noteOff(0, 0);
                if (!isMute()) {
                    channel_mgr->mChannels[0] =
                        channelStart(channel_mgr, uvar7, i_velocity, update_timer);
                }
            } else {
                JASChannel* channel = channel_mgr->mChannels[0];
                if (channel != NULL) {
                    channel->setKey(uvar7 - channel->field_0xdc.field_0x4.field_0x01);
                    channel->setVelocity(i_velocity);
                    channel->setUpdateTimer(update_timer);
                }
            }
            if (i_flags & 1) {
                JASChannel* channel = channel_mgr->mChannels[0];
                if (channel != NULL) {
                    channel->setKeySweepTarget(param_0 - uvar7, uvar2);
                }
            }
        }
    }
    mFlags.flag4 = (i_flags & 2) ? true : false;
    field_0x22b = param_0;
    return result;
}

int JASTrack::noteOff(u32 noteid, u16 param_1) {
    JUT_ASSERT(563, noteid < TChannelMgr::CHANNEL_MAX);
    int ret = 1;
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL) {
            if (!mChannelMgrs[i]->noteOff(noteid, param_1)) {
                ret = 0;
            }
        }
    }
    return ret;
}

bool JASTrack::checkNoteStop(u32 noteid) const {
    JUT_ASSERT(581, noteid < TChannelMgr::CHANNEL_MAX);
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != NULL) {
            JASChannel* channel = mChannelMgrs[i]->mChannels[noteid];
            if (channel != NULL) {
                return false;
            }
        }
    }
    return true;
}

void JASTrack::overwriteOsc(JASChannel* i_channel) {
    for (u32 i = 0; i < 2; i++) {
        if (mOscParam[i].mTable != NULL) {
            JUT_ASSERT(599, mOscParam[i].rel_table == 0);
            i_channel->setOscInit(i, &mOscParam[i]);
        }
    }
}

void JASTrack::updateTimedParam() {
    for (int i = 0; i < 6; i++) {
        MoveParam_* param = &mMoveParam.array[i];
        if (param->mCount != 0) {
            param->mValue += (param->mTarget - param->mValue) / param->mCount;
            param->mCount--;
        }
    }
}

void JASTrack::updateTrack(f32 param_0) {
    updateTempo();
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* channel_mgr = mChannelMgrs[i];
        if (channel_mgr != NULL) {
            f32 volume = mMoveParam.params.volume.mValue;
            volume = volume * volume;
            f32 pitch = 1.0f;
            f32 fvar6 = mMoveParam.params.pitch.mValue * mBendSense * (1.0f / 3.0f);
            f32 pan = (mMoveParam.params.pan.mValue - 0.5f) * mPanPower;
            f32 fxmix = mMoveParam.params.fxmix.mValue;
            f32 dolby = mMoveParam.params.dolby.mValue;
            if (channel_mgr->mSoundParams != NULL) {
                volume *= channel_mgr->mSoundParams->mVolume;
                pitch *= channel_mgr->mSoundParams->mPitch;
                pan += channel_mgr->mSoundParams->mPan - 0.5f;
                fxmix += channel_mgr->mSoundParams->mFxMix;
                dolby += channel_mgr->mSoundParams->mDolby;
            }
            pan *= param_0;
            if (mParent == NULL) {
                channel_mgr->mParams.mVolume = volume;
                channel_mgr->mParams.mPitch = pitch;
                channel_mgr->mParams.mPan = pan;
                channel_mgr->mParams.mFxMix = fxmix;
                channel_mgr->mParams.mDolby = dolby;
                channel_mgr->mParams.field_0x8 = fvar6;
            } else {
                TChannelMgr* parent_channel_mgr = mParent->mChannelMgrs[i];
                if (parent_channel_mgr == NULL) {
                    parent_channel_mgr = mParent->mChannelMgrs[0];
                }
                channel_mgr->mParams.mVolume = parent_channel_mgr->mParams.mVolume * volume;
                channel_mgr->mParams.mPitch = parent_channel_mgr->mParams.mPitch * pitch;
                channel_mgr->mParams.mPan = (parent_channel_mgr->mParams.mPan - 0.5f) + pan;
                channel_mgr->mParams.mFxMix = parent_channel_mgr->mParams.mFxMix + fxmix;
                channel_mgr->mParams.mDolby = parent_channel_mgr->mParams.mDolby + dolby;
                channel_mgr->mParams.field_0x8 = parent_channel_mgr->mParams.field_0x8 + fvar6;
            }
            channel_mgr->mParams.mPan += 0.5f;
        }
    }
}

void JASTrack::updateTempo() {
    if (mParent == NULL) {
        field_0x1dc = mTempoRate * (mTimebase * mTempo * (4.0f / 3.0f) / JASDriver::getDacRate());
    } else {
        mTempo = mParent->mTempo;
        mTimebase = mParent->mTimebase;
        field_0x1dc = mParent->field_0x1dc;
    }
}

void JASTrack::updateSeq(bool param_0, f32 param_1) {
    if (!param_0) {
        param_0 = mFlags.flag5;
    }
    mFlags.flag5 = false;
    if (param_0) {
        updateTrack(param_1);
    }
    f32 fvar1 = param_1 * mPanPower;
    for (int i = 0; i < 16; i++) {
        JASTrack* child = mChildren[i];
        if (child != NULL && child->mStatus == STATUS_RUN) {
            child->updateSeq(param_0, fvar1);
        }
    }
}

u32 JASTrack::seqTimeToDspTime(f32 i_seqTime) {
    if (mFlags.flag2) {
        i_seqTime = i_seqTime / field_0x1dc;
    } else {
        i_seqTime = i_seqTime * (120.0f / mTimebase);
        i_seqTime *= JASDriver::getSubFrames() / 10.0f;
    }
    return i_seqTime;
}

void JASTrack::setParam(u32 target, f32 i_target, u32 i_count) {
    JUT_ASSERT(739, target < TIMED_PARAMS);
    MoveParam_* param = &mMoveParam.array[target];
    param->mTarget = i_target;
    if (i_count == 0) {
        param->mValue = param->mTarget;
    }
    param->mCount = i_count;
}

void JASTrack::noteOffAll(u16 param_0) {
    for (u8 i = 0; i < 8; i++) {
        noteOff(i, param_0);
    }
    for (int i = 0; i < 16; i++) {
        JASTrack* child = mChildren[i];
        if (child != NULL && child->mStatus == STATUS_RUN) {
            child->noteOffAll(param_0);
        }
    }
}

void JASTrack::mute(bool i_mute) {
    mFlags.mute = i_mute;
    if (i_mute) {
        noteOffAll(10);
    }
}

void JASTrack::setOscScale(u32 osc_no, f32 i_scale) {
    JUT_ASSERT(792, osc_no < OSC_NUM);
    mOscParam[osc_no].mScale = i_scale;
}

void JASTrack::setOscTable(u32 osc_no, JASOscillator::Point const* i_table) {
    JUT_ASSERT(799, osc_no < OSC_NUM)
    mOscParam[osc_no].mTable = i_table;
    if (osc_no != 0) {
        mOscParam[osc_no].rel_table = NULL;
    }
}

void JASTrack::setOscAdsr(s16 param_0, s16 param_1, s16 param_2, s16 param_3, u16 i_directRelease) {
    mOscParam[0] = sEnvOsc;
    mOscParam[0].mTable = mOscPoint;
    mOscPoint[0]._2 = param_0;
    mOscPoint[1]._2 = param_1;
    mOscPoint[2]._2 = param_2;
    mOscPoint[2]._4 = param_3;
    mDirectRelease = i_directRelease;
}

const u32 JASDsp::FILTER_MODE_IIR = 0x00000020;

void JASTrack::setFIR(s16 const* i_FIR) {
    for (int i = 0; i < 8; i++) {
        mFIR[i] = i_FIR[i];
    }
    mFilterMode &= JASDsp::FILTER_MODE_IIR;
    mFilterMode |= 8;
}

void JASTrack::setIIR(s16 const* i_IIR) {
    for (int i = 0; i < 8; i++) {
        mIIR[i] = i_IIR[i];
    }
    mFilterMode |= JASDsp::FILTER_MODE_IIR;
}

u16 JASTrack::readPortSelf(u32 param_0) {
    return mTrackPort.readImport(param_0);
}

void JASTrack::writePortSelf(u32 param_0, u16 param_1) {
    mTrackPort.writeExport(param_0, param_1);
}

void JASTrack::writePort(u32 param_0, u16 param_1) {
    JASSeqCtrl::IntrType intr;
    mTrackPort.writeImport(param_0, param_1);
    if (param_0 == 0 || param_0 == 1) {
        if (param_0 == 0) {
            intr = JASSeqCtrl::INTRTYPE_VALUE_2;
        } else {
            intr = JASSeqCtrl::INTRTYPE_VALUE_3;
        }
        mSeqCtrl.interrupt(intr);
    }
}

u16 JASTrack::readPort(u32 param_0) {
    return mTrackPort.readExport(param_0);
}

void JASTrack::setChannelPauseFlag(bool i_pause) {
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* mgr = mChannelMgrs[i];
        if (mgr != NULL) {
            mgr->setPauseFlag(i_pause);
        }
    }
    for (int i = 0; i < 16; i++) {
        JASTrack* child = mChildren[i];
        if (child != NULL) {
            child->setChannelPauseFlag(i_pause);
        }
    }
}

void JASTrack::pause(bool i_pause) {
    if (mFlags.pause != i_pause) {
        mFlags.pause = i_pause;
        setChannelPauseFlag(i_pause);
        mSeqCtrl.interrupt(i_pause ? JASSeqCtrl::INTRTYPE_VALUE_0 : JASSeqCtrl::INTRTYPE_VALUE_1);
    }
}

int JASTrack::getTransposeTotal() const {
    if (mParent != NULL) {
        return mTranspose + mParent->getTransposeTotal();
    } else {
        return mTranspose;
    }
}

bool JASTrack::isMute() const {
    if (mParent != NULL) {
        return mFlags.mute || mParent->isMute();
    } else {
        return mFlags.mute;
    }
}

void JASTrack::setTempo(u16 i_tempo) {
    JUT_ASSERT(961, mParent == 0);
    mTempo = i_tempo;
    mFlags.flag5 = true;
    updateTempo();
}

void JASTrack::setTempoRate(f32 i_tempoRate) {
    JUT_ASSERT(969, mParent == 0);
    mTempoRate = i_tempoRate;
    mFlags.flag5 = true;
    updateTempo();
}

void JASTrack::setTimebase(u16 i_timebase) {
    JUT_ASSERT(977, mParent == 0);
    mTimebase = i_timebase;
    mFlags.flag5 = true;
    updateTempo();
}

void JASTrack::updateChannel(JASChannel* param_0, JASDsp::TChannel* param_1) {
    param_0->setVibrate(mVibDepth, mVibPitch);
    param_0->setTremolo(mTremDepth, mTremPitch);
    if (mFilterMode & 0x20) {
        param_1->setIIRFilterParam(mIIR);
    }
    if (mFilterMode & 0x1f) {
        param_1->setFIR8FilterParam(mFIR);
    }
    param_1->setFilterMode(mFilterMode);
    param_1->setDistFilter(mMoveParam.params.distFilter.mValue * 32767.0f);
}


void JASTrack::channelUpdateCallback(u32 param_0, JASChannel* param_1,
                                     JASDsp::TChannel* param_2, void* param_3) {
    TChannelMgr* channel_mgr = static_cast<TChannelMgr*>(param_3);
    JASTrack* track = channel_mgr->mTrack;
    switch (param_0) {
    case 0:
    case 1:
        param_1->setParams(channel_mgr->mParams);
        track->updateChannel(param_1, param_2);
        break;
    case 3:
        param_1->release(0);
        param_1->free();
        channel_mgr->mChannels[0] = NULL;
        break;
    case 2:
        for (int i = 0; i < 8; i++) {
            if (param_1 == channel_mgr->mChannels[i]) {
                channel_mgr->mChannels[i] = NULL;
                break;
            }
        }
        break;
    }
}

int JASTrack::getChannelCount() const {
    JASCriticalSection cs;
    int count = 0;
    for (int i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i]) {
            for (int j = 0; j < 8; j++) {
                if (mChannelMgrs[i]->mChannels[j]) {
                    count++;
                }
            }
        }
    }
    return count;
}

JASTrack* JASTrack::getRootTrack() {
    JASTrack* track = this;
    while (track->mParent != NULL) {
        track = track->mParent;
    }
    return track;
}

int JASTrack::tickProc() {
    if (mFlags.pause) {
        return 0;
    }
    int ret = mSeqCtrl.tickProc(this);
    updateTimedParam();
    mFlags.flag5 = true;
    if (ret < 0) {
        return -1;
    }
    for (int i = 0; i < 16; i++) {
        JASTrack* child = mChildren[i];
        if (child != NULL && child->mStatus == STATUS_RUN) {
            JUT_ASSERT(1090, this == child->mParent);
            ret = child->tickProc();
            if (ret < 0) {
                getRootTrack()->updateSeq(false, 1.0f);
                child->close();
                if (child->mFlags.autoDelete) {
                    delete child;
                    mChildren[i] = NULL;
                }
            }
        }
    }
    return 0;
}

int JASTrack::seqMain() {
    JUT_ASSERT(1108, mStatus == STATUS_RUN);
    if (mFlags.flag6) {
        updateSeq(true, 1.0f);
        close();
        return -1;
    } else {
        while (field_0x1d8 >= 1.0f) {
            field_0x1d8 -= 1.0f;
            int ret = tickProc();
            if (ret < 0) {
                updateSeq(false, 1.0f);
                close();
                return -1;
            }
        }
        field_0x1d8 += field_0x1dc;
        updateSeq(false, 1.0f);
        return 0;
    }
}

s32 JASTrack::TList::cbSeqMain(void* i_this) {
    JASTrack::TList* list = static_cast<JASTrack::TList*>(i_this);
    list->seqMain();
    return 0;
}

void JASTrack::TList::append(JASTrack* i_track) {
    if (!mCallbackRegistered) {
        if (!JASDriver::registerSubFrameCallback(cbSeqMain, this)) {
            JUT_WARN(1157, "%s", "Failed: registerSubFrameCallback");
            return;
        }
        mCallbackRegistered = true;
    }
    Push_back(i_track);
}

void JASTrack::TList::seqMain() {
#if DEBUG
    iterator it, it2;
    for (it = begin(); it != end(); it = it2) {
        it2 = it;
        ++it2;
        int seqMainRes = it->seqMain();
        if (seqMainRes < 0) {
            Remove(&*it);
            if (it->mFlags.autoDelete) {
                delete &*it;
            }
        }
    }
#else
    iterator it;
    for (it = begin(); it != end();) {
        // Fakematch: Debug shows that it2 should be declared outside the loop and then this line
        // should use operator=, but that results in a missing load instruction for retail, so we
        // use the copy constructor instead of the assignment operator, which fixes it for some
        // reason. Might be related to the fakematch in the assignment operator?
        iterator it2 = it;
        ++it2;
        int seqMainRes = it->seqMain();
        if (seqMainRes < 0) {
            Remove(&*it);
            if (it->mFlags.autoDelete) {
                delete &*it;
            }
        }
        it = it2;
    }
#endif
}

JASTrack::TChannelMgr::TChannelMgr(JASTrack* i_track) : mSoundParams(NULL), mTrack(i_track) {
    for (int i = 0; i < 8; i++) {
        mChannels[i] = NULL;
    }
    for (int i = 0; i < 8; i++) {
        field_0x38[i] = 0;
    }
}


void JASTrack::TChannelMgr::init() {
    mSoundParams = NULL;
    mParams.init();
    for (int i = 0; i < 8; i++) {
        mChannels[i] = NULL;
    }
    for (int i = 0; i < 8; i++) {
        field_0x38[i] = 0;
    }
}


void JASTrack::TChannelMgr::releaseAll() {
    for (int i = 0; i < 8; i++) {
        JASChannel* channel = mChannels[i];
        if (channel != NULL) {
            channel->release(0);
            channel->free();
            mChannels[i] = NULL;
        }
    }
}

bool JASTrack::TChannelMgr::noteOff(u32 noteid, u16 param_1) {
    JUT_ASSERT(1224, noteid < CHANNEL_MAX);
    JASChannel* channel = mChannels[noteid];
    if (channel == NULL) {
        return false;
    }
    if (param_1 == 0) {
        channel->release(0);
    } else {
        channel->release(param_1);
    }
    channel->free();
    mChannels[noteid] = NULL;
    return true;
}

void JASTrack::TChannelMgr::setPauseFlag(bool i_pause) {
    for (int i = 0; i < 8; i++) {
        JASChannel* channel = mChannels[i];
        if (channel != NULL) {
            channel->setPauseFlag(i_pause);
        }
    }
}
