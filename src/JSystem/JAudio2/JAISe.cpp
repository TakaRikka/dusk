#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISe.h"
#include "JSystem/JAudio2/JAIAudience.h"
#include "JSystem/JAudio2/JAISeMgr.h"

JAISe::JAISe(JAISeMgr* seMgr, JAISoundStrategyMgr<JAISe>* soundStrategyMgr, u32 priority) : JSULink<JAISe>(this) {
    inner_.mSoundStrategyMgr = soundStrategyMgr;
    inner_.seMgr = seMgr;
    inner_.mProperPriority = priority;
}

void JAISe::mixOut_(const JASSoundParams& params) {
    inner_.track.setTempoRate(inner_.mTempoMgr.getTempo());
    JASSoundParams mixParams;
    params_.mixOutAll(params, &mixParams, fader_.getIntensity());

    if (inner_.field_0x278) {
        inner_.field_0x278->virtual4(this, mixParams);
    }

    if (audible_) {
        int maxChannels = audience_->getMaxChannels();
        for (int i = 0; i < maxChannels; i++) {
            audience_->mixChannelOut(mixParams, audible_, i);
        }
    } else {
        inner_.mSoundParams = mixParams;
    }
}

void JAISe::stopTrack_() {
    switch (inner_.track.getStatus()) {
    case JASTrack::STATUS_FREE:
        break;
    case JASTrack::STATUS_RUN:
        inner_.track.stopSeq();
        break;
    case JASTrack::STATUS_STOP:
        break;
    }
}

void JAISe::startTrack_(const JASSoundParams& params) {
    JUT_ASSERT(72, inner_.track.getStatus() != JASTrack::STATUS_RUN);

    if (inner_.track.getStatus() == JASTrack::STATUS_STOP) {
        inner_.track.init();
    }

    if (audible_) {
        initTrack_JAISound_(&inner_.track);
    } else {
        params_.mixOutAll(params, &inner_.mSoundParams, fader_.getIntensity());
        inner_.track.setChannelMgrCount(1);
        inner_.track.assignExtBuffer(0, &inner_.mSoundParams);
    }

    inner_.field_0x26c = 1;
    inner_.track.setSeqData(inner_.mSeqData.field_0x0, inner_.mSeqData.field_0x4);
    inner_.track.startSeq();
}

void JAISe::JAISeCategoryMgr_mixOut_(bool param_0, const JASSoundParams& params, JAISoundActivity activity) {
    mixOut_(params);
    if (isStopping()) {
        stopTrack_();
    } else if (param_0) {
        if (status_.isPlaying()) {
            if (inner_.field_0x26c) {
                switch (inner_.track.getStatus()) {
                case JASTrack::STATUS_STOP:
                    if (status_.field_0x1.flags.flag1) {
                        startTrack_(params);
                    } else {
                        stop_JAISound_();
                    }
                    break;
                case JASTrack::STATUS_RUN:
                    inner_.track.pause(status_.isPaused() || activity.field_0x0.flags.flag2);
                    inner_.track.mute(status_.isMute() || activity.field_0x0.flags.flag1);
                }
            } else {
                startTrack_(params);
            }
        }
    } else if (status_.field_0x1.flags.flag1) {
        if (status_.field_0x1.flags.flag3) {
            inner_.track.pause(true);
        } else {
            stopTrack_();
        }
    } else {
        stopTrack_();
        stop_JAISound_();
    }
}

void JAISe::JAISeCategoryMgr_calc_() {
    prepare_();

    if (calc_JAISound_()) {
        inner_.mTempoMgr.calc();
        if (inner_.field_0x278) {
            inner_.field_0x278->calc(this);
        }
    }

    if (isStopping() || priority_ == -1) {
        inner_.mPriority = -1;
    } else {
        inner_.mPriority = inner_.mProperPriority + priority_;
    }
}

bool JAISe::JAISound_tryDie_() {
    switch (inner_.track.getStatus()) {
    case JASTrack::STATUS_FREE:
    case JASTrack::STATUS_STOP:
        if (inner_.field_0x278) {
            inner_.mSoundStrategyMgr->virtual4(inner_.field_0x278);
            inner_.field_0x278 = NULL;
        }
        die_JAISound_();
        return true;
    case JASTrack::STATUS_RUN:
        break;
    }
    return false;
}

void JAISe::JAISeMgr_startID_(JAISoundID id, const JGeometry::TVec3<f32>* posPtr, JAIAudience* audience) {
    JUT_ASSERT(221, inner_.track.getStatus() == JASTrack::STATUS_FREE);

    start_JAISound_(id, posPtr, audience);
    inner_.field_0x26c = 0;
    if (inner_.mSoundStrategyMgr) {
        inner_.field_0x278 = inner_.mSoundStrategyMgr->calc(id);
    } else {
        inner_.field_0x278 = NULL;
    }
}

s32 JAISe::getNumChild() const {
    return 0;
}

JAISoundChild* JAISe::getChild(int param_0) {
    return NULL;
}

void JAISe::releaseChild(int param_0) {}

bool JAISe::prepare_getSeqData_() {
    JUT_ASSERT(271, inner_.track.getStatus() != JASTrack::STATUS_RUN);
    JUT_ASSERT(272, inner_.seMgr);
    JUT_ASSERT(273, inner_.seMgr ->getSeqDataMgr());

    switch (inner_.seMgr->getSeqDataMgr()->getSeqData(getID(), &inner_.mSeqData)) {
    case 0:
        OS_REPORT("Failed In Loading Sequence Data:SE(0x%08x)\n", *(u32*)&getID());
        status_.state.unk = 2;
        return false;
    case 1:
        increasePrepareCount_JAISound_();
        return false;
    case 2:
        return true;
    default:
        JUT_ASSERT(296, false);
    }

    return false;
}

void JAISe::prepare_() {
    switch (status_.state.unk) {
    case 6:
        JUT_ASSERT(306, false);
        break;
    case 2:
        stop_JAISound_();
        break;
    case 0:
        if (prepare_getSeqData_()) {
            status_.state.unk = 5;
        }
        break;
    case 1:
        if (prepare_getSeqData_()) {
            status_.state.unk = 3;
        }
        break;
    case 4:
        status_.state.unk = 5;
        break;
    case 3:
    case 5:
        break;
    }
}

JASTrack* JAISe::getTrack() {
    return &inner_.track;
}

JASTrack* JAISe::getChildTrack(int param_0) {
    return inner_.track.getChild(param_0);
}

JAISe* JAISe::asSe() {
    return this;
}

JAITempoMgr* JAISe::getTempoMgr() {
    return &inner_.mTempoMgr;
}
