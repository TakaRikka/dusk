#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JAIAudience.h"
#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JASReport.h"
#include "JSystem/JAudio2/JASTrack.h"

void JAISoundHandle::releaseSound() {
    if (sound_ != NULL) {
        sound_->handle_ = NULL;
        sound_ = NULL;
    }
}

void JAISound::releaseHandle() {
    if (handle_ != NULL) {
        handle_->sound_ = NULL;
        handle_ = NULL;
    }
}

void JAISound::attachHandle(JAISoundHandle* handle) {
    JUT_ASSERT(55, handle);

    if (handle->isSoundAttached()) {
        handle->getSound()->stop();
    }
    if (isHandleAttached()) {
        releaseHandle();
    }
    handle_ = handle;
    handle_->sound_ = this;
}

s32 JAISoundStatus_::lockWhenPrepared() {
    if (state.unk == 0) {
        state.unk = 1;
        return 1;
    }
    return 0;
}

s32 JAISoundStatus_::unlockIfLocked() {
    if (state.unk == 3) {
        state.unk = 4;
        return 1;
    }
    if (state.unk == 1) {
        state.unk = 0;
        return 1;
    }
    return 0;
}

void JAISoundParams::mixOutAll(const JASSoundParams& inParams, JASSoundParams* outParams, f32 param_2) {
    outParams->mVolume = move_.params_.mVolume * (inParams.mVolume * property_.field_0x0) * param_2;
    outParams->mFxMix = move_.params_.mFxMix + (inParams.mFxMix + property_.field_0x4);
    outParams->mPitch = move_.params_.mPitch * (inParams.mPitch * property_.field_0x8);
    outParams->mPan = (inParams.mPan + move_.params_.mPan) - 0.5f;
    outParams->mDolby = inParams.mDolby + move_.params_.mDolby;
}

JAISound::JAISound() : params_() {}

void JAISound::start_JAISound_(JAISoundID id, const JGeometry::TVec3<f32>* posPtr, JAIAudience* audience) {
    handle_ = NULL;
    soundID_ = id;
    status_.init();
    params_.init();
    fader_.forceIn();
    audience_ = audience;
    prepareCount_ = 0;
    count_ = 0;

    if (posPtr != NULL && audience_ != NULL) {
        audible_ = audience_->newAudible(*posPtr, soundID_, NULL, 0);
    } else {
        audible_ = NULL;
    }

    priority_ = 0;
}

bool JAISound::acceptsNewAudible() const {
    bool accepts = false;
    if (audible_ == NULL && status_.state.flags.calcedOnce == 0) {
        accepts = true;
    }
    return accepts;
}

void JAISound::newAudible(const JGeometry::TVec3<f32>& pos,
                          const JGeometry::TVec3<f32>* param_1, u32 param_2, JAIAudience* audience) {
    JUT_ASSERT(151, acceptsNewAudible());
    if (audience != NULL) {
        audience_ = audience;
    }

    JUT_ASSERT(157, audience_);
    audible_ = audience_->newAudible(pos, soundID_, param_1, param_2);
}

void JAISound::stop(u32 fadeTime) {
    JUT_ASSERT(164, status_.isAlive());
    if (fadeTime == 0) {
        stop();
    } else {
        fader_.fadeOut(fadeTime);
        removeLifeTime_();
        status_.state.flags.flag5 = 1;
        status_.state.flags.flag1 = 1;
    }
}

void JAISound::stop() {
    stop_JAISound_();
    releaseHandle();
}

JAISe* JAISound::asSe() {
    return NULL;
}

JAISeq* JAISound::asSeq() {
    return NULL;
}

JAIStream* JAISound::asStream() {
    return NULL;
}

void JAISound::die_JAISound_() {
    if (audible_ != NULL) {
        audience_->deleteAudible(audible_);
        audible_ = NULL;
        audience_ = NULL;
    }
    fader_.forceOut();
    releaseHandle();
    status_.state.unk = 6;
}

void JAISound::increasePrepareCount_JAISound_() {
    if ((++prepareCount_ & 0xFF) == 0) {
        JASReport("It cost %d steps to prepare Sound(ID:%08x, Address%08x).\n", prepareCount_,
                  *(u32*)&soundID_, this);
    }
}

bool JAISound::calc_JAISound_() {
    status_.state.flags.calcedOnce = 1;
    if (isStopping() && JAISound_tryDie_()) {
        return false;
    }

    JUT_ASSERT(230, status_.isAlive());
    bool isPlaying = status_.isPlaying();
    if (isPlaying) {
        count_++;
    }

    bool isPaused = status_.isPaused();
    if (isPaused == false) {
        fader_.calc();
    }

    bool playing = isPlaying && isPaused == false;
    if (playing) {
        params_.move_.calc();
        if (audible_ != NULL) {
            audible_->calc();
        }
        if (status_.field_0x1.flags.flag2 != 0) {
            if (lifeTime_ == 0) {
                stop_JAISound_();
            } else {
                lifeTime_--;
            }
        }
    }

    if (audience_ != NULL && audible_ != NULL) {
        if ((priority_ = audience_->calcPriority(audible_)) == 0xFFFFFFFF && status_.field_0x1.flags.flag1 == 0) {
            stop_JAISound_();
        }
    } else {
        priority_ = 0;
    }

    return playing;
}

void JAISound::initTrack_JAISound_(JASTrack* track) {
    JASSoundParams* soundParams[JASTrack::TChannelMgr::CHANNEL_MAX];
    JUT_ASSERT(286, audience_);
    JUT_ASSERT(287, audible_);

    int numChannels = 0;
    JUT_ASSERT(290, audience_->getMaxChannels() < JASTrack::TChannelMgr::CHANNEL_MAX);
    
    for (int i = 0; i < audience_->getMaxChannels(); i++) {
        JASSoundParams* currentParams = audible_->getOuterParams(i);
        if (currentParams != NULL) {
            soundParams[numChannels++] = currentParams;
        }
    }

    JUT_ASSERT(298, numChannels >= 1);
    track->setChannelMgrCount(numChannels);

    for (u32 i = 0; i < track->getChannelMgrCount(); i++) {
        track->assignExtBuffer(i, soundParams[i]);
    }
}
