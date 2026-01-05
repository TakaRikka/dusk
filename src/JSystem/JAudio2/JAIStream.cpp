#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAIStream.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"
#include "JSystem/JAudio2/JAISoundChild.h"
#include "JSystem/JAudio2/JAIStreamDataMgr.h"
#include "JSystem/JAudio2/JAIAudience.h"

static void JAIStream_JASAramStreamCallback_(u32 type, JASAramStream* aramStream, void* userData) {
    JAIStream* stream = (JAIStream*)userData;

    switch (type) {
    case 1:
        stream->field_0x2c5 = 1;
        break;
    case 0:
        stream->field_0x2c6 = 1;
        break;
    }
}

JAIStream::JAIStream(JAIStreamMgr* streamMgr, JAISoundStrategyMgr<JAIStream>* soundStrategyMgr) : JSULink<JAIStream>(this) {
    field_0x290 = 0;
    field_0x2c0 = soundStrategyMgr;
    streamMgr_ = streamMgr;
    streamAramAddr_ = 0;
    field_0x2c5 = 0;
    field_0x2c6 = 0;

    for (int i = 0; i < NUM_CHILDREN; i++) {
        children_[i] = NULL;
    }
}

void JAIStream::JAIStreamMgr_startID_(JAISoundID id, s32 streamFileEntry,
                                      const JGeometry::TVec3<f32>* posPtr, JAIAudience* audience,
                                      int category) {
    field_0x298 = category;
    field_0x294 = streamFileEntry;
    start_JAISound_(id, posPtr, audience);
    field_0x290 = 0;

    if (field_0x2c0 != NULL) {
        field_0x2bc = field_0x2c0->calc(id);
    } else {
        field_0x2bc = NULL;
    }
}

bool JAIStream::prepare_prepareStream_() {
    u32 local_28;
    JAIStreamAramMgr* streamAramMgr;

    switch (field_0x290) {
    case 0:
        streamAramMgr = streamMgr_->getStreamAramMgr();
        JUT_ASSERT(100, streamAramMgr);

        streamAramAddr_ = streamAramMgr->newStreamAram(&local_28);
        if (streamAramAddr_ != NULL) {
            inner_.aramStream_.init((u32)streamAramAddr_, local_28, &JAIStream_JASAramStreamCallback_, this);
            field_0x290 = 1;
            prepareCount_ = 0;
        } else {
            increasePrepareCount_JAISound_();
        }
        break;
    case 1:
        if (audible_ != NULL) {
            JASSoundParams* soundParams = audible_->getOuterParams(0);
            inner_.aramStream_.setPitch(soundParams->mPitch);
            inner_.aramStream_.setVolume(soundParams->mVolume);
            inner_.aramStream_.setPan(soundParams->mPan);
            inner_.aramStream_.setFxmix(soundParams->mFxMix);
            inner_.aramStream_.setDolby(soundParams->mDolby);
        }

        field_0x2c5 = 0;

        if (inner_.aramStream_.prepare(field_0x294, -1)) {
            field_0x290 = 2;
        }
        break;
    case 2:
        if (field_0x2c5 != 0) {
            field_0x2c5 = 0;
            field_0x290 = 3;
        } else {
            increasePrepareCount_JAISound_();
        }
        break;
    case 3:
        return true;
    case 4:
        JUT_WARN(155, "%s", "PC must not pass here.");
        break;
    }
  
    return false;
}

void JAIStream::prepare_() {
    if (field_0x2c6 != 0) {
        return;
    }

    switch(status_.state.unk) {
    case 6:
        JUT_ASSERT(171, false);
        break;
    case 0:
        if (prepare_prepareStream_()) {
            status_.state.unk = 5;
            prepare_startStream_();
        }
        break;
    case 1:
        if (prepare_prepareStream_()) {
            status_.state.unk = 3;
        }
        break;
    case 4:
        status_.state.unk = 5;
        prepare_startStream_();
        break;
    case 5:
        break;
    }
}

void JAIStream::prepare_startStream_() {
    if (inner_.aramStream_.start()) {
        field_0x2c6 = 0;
        field_0x2c4 = 0;
        field_0x290 = 4;
    }
}

void JAIStream::JAIStreamMgr_mixOut_(const JASSoundParams& inParams, JAISoundActivity activity) {
    bool local_54;
    JASSoundParams outParams;
    params_.mixOutAll(inParams, &outParams, (status_.isMute() || activity.field_0x0.flags.flag1) ? 0.0f : fader_.getIntensity());

    if (field_0x2bc != NULL) {
        field_0x2bc->virtual4(this, outParams);
    }

    JASSoundParams* mixParams = &outParams;
    if (audible_ != NULL && audience_ != NULL) {
        for (int i = 0; i < audience_->getMaxChannels(); i++) {
            JASSoundParams* outerParams = audible_->getOuterParams(i);
            if (outerParams != NULL) {
                audience_->mixChannelOut(outParams, audible_, i);
                mixParams = outerParams;
                break;
            }
        }
    }

    for (int i = 0; i < NUM_CHILDREN; i++) {
        inner_.aramStream_.setPitch(mixParams->mPitch);
        if (children_[i] != NULL) {
            inner_.aramStream_.setChannelVolume(i, children_[i]->mMove.params_.mVolume * mixParams->mVolume);
            inner_.aramStream_.setChannelPan(i, (children_[i]->mMove.params_.mPan + mixParams->mPan) - 0.5f);
            inner_.aramStream_.setChannelFxmix(i, children_[i]->mMove.params_.mFxMix + mixParams->mFxMix);
            inner_.aramStream_.setChannelDolby(i, children_[i]->mMove.params_.mDolby + mixParams->mDolby);
        } else {
            inner_.aramStream_.setChannelVolume(i, mixParams->mVolume);
            inner_.aramStream_.setChannelPan(i, mixParams->mPan);
            inner_.aramStream_.setChannelFxmix(i, mixParams->mFxMix);
            inner_.aramStream_.setChannelDolby(i, mixParams->mDolby);
        }
    }

    prepare_();

    if (field_0x290 == 4) {
        local_54 = status_.isPaused() || activity.field_0x0.flags.flag2;
        if (local_54 != field_0x2c4) {
            inner_.aramStream_.pause(local_54);
            field_0x2c4 = local_54;
        }
    }
}

void JAIStream::die_JAIStream_() {
    die_JAISound_();

    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (children_[i] != NULL) {
            delete children_[i];
            children_[i] = NULL;
        }
    }

    if (field_0x2bc != NULL) {
        field_0x2c0->virtual4(field_0x2bc);
        field_0x2bc = NULL;
    }
}

bool JAIStream::JAISound_tryDie_() {
    if (field_0x2c6) {
        die_JAIStream_();
        return true;
    }

    switch (field_0x290) {
    case 0:
    case 1:
        die_JAIStream_();
        return true;
    case 2:
    case 3:
        field_0x290 = 5;
        inner_.aramStream_.cancel();
        break;
    case 4:
        field_0x290 = 6;
        inner_.aramStream_.stop(10);
        break;
    }

    return false;
}

void JAIStream::JAIStreamMgr_calc_() {
    if (field_0x2c6 != 0) {
        field_0x290 = 0;
        stop_JAISound_();
    }

    if (calc_JAISound_()) {
        for (int i = 0; i < NUM_CHILDREN; i++) {
            if (children_[i] != NULL) {
                children_[i]->calc();
            }
        }

        if (field_0x2bc != NULL) {
            field_0x2bc->calc(this);
        }
    }
}

s32 JAIStream::getNumChild() const {
    return NUM_CHILDREN;
}

JAISoundChild* JAIStream::getChild(int index) {
    if (children_[index] == NULL) {
         children_[index] = new JAISoundChild();
        if (children_[index] == NULL) {
            JUT_WARN(370, "%s", "JASPoolAllocObject::<JAISoundChild>::operator new failed .\n")
        }
    }
    return children_[index];
}

void JAIStream::releaseChild(int index) {
    if (children_[index] != NULL) {
        delete children_[index];
        children_[index] = NULL;
    }
}

JASTrack* JAIStream::getTrack() {
    return NULL;
}

JASTrack* JAIStream::getChildTrack(int param_0) {
    return NULL;
}

JAIStream* JAIStream::asStream() {
    return this;
}

JAITempoMgr* JAIStream::getTempoMgr() {
    return NULL;
}
