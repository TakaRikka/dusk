#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISeq.h"
#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAISoundChild.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JAIAudience.h"

namespace {

static bool JASTrack_isFreeOrStopped(JASTrack* track) {
    u32 status = track->getStatus();
    return status == JASTrack::STATUS_FREE || status == JASTrack::STATUS_STOPPED;
}

}  // namespace

JAISeq::JAISeq(JAISeqMgr* seqMgr, JAISoundStrategyMgr<JAISeq>* soundStrategyMgr) : JSULink<JAISeq>(this) {
    inner_.seqMgr = seqMgr;
    inner_.strategyMgr = soundStrategyMgr;
    inner_.mSoundParams.init();
    for (int i = 0; i < 32; i++) {
        inner_.mSoundChild[i] = NULL;
    }
}

void JAISeq::JAISeqMgr_startID_(JAISoundID id, const JGeometry::TVec3<f32>* posPtr,
                                JAIAudience* audience, int category, int param_4) {
    inner_.field_0x39c = category;
    reserveChildTracks_(param_4);
    start_JAISound_(id, posPtr, audience);

    if (inner_.strategyMgr) {
        field_0x3a8 = inner_.strategyMgr->calc(id);
    } else {
        field_0x3a8 = NULL;
    }
}

void JAISeq::playSeqData_(const JASSoundParams& params, JAISoundActivity activity) {
    JUT_ASSERT(72, JASTrack_isFreeOrStopped( & inner_.outputTrack ));

    inner_.outputTrack.setSeqData(inner_.mSeqData.field_0x0, inner_.mSeqData.field_0x4);
    if (audible_) {
        initTrack_JAISound_(&inner_.outputTrack);
    } else {
        inner_.outputTrack.setChannelMgrCount(1);
        inner_.outputTrack.assignExtBuffer(0, &inner_.mSoundParams);
    }

    mixOut_(params, activity);
    inner_.outputTrack.startSeq();
}

void JAISeq::reserveChildTracks_(int param_0) {
    JUT_ASSERT(92, inner_.outputTrack.getStatus() == JASTrack::STATUS_FREE);

    for (int i = 0; i < 2; i++) {
        JASTrack* track = new JASTrack();
        if (track) {
            track->setAutoDelete(true);
            inner_.outputTrack.connectChild(i, track);

            for (int j = 0; j < 16; j++) {
                int sp18 = i * 16 + j;
                if (sp18 >= param_0) {
                    continue;
                }

                JASTrack* track2 = new JASTrack();
                if (track2) {
                    track2->setAutoDelete(true);
                    track->connectChild(j, track2);
                } else {
                    JUT_WARN(117, "%s", "JASPoolAllocObject::<JASTrack>::operator new failed .\n");
                    continue;
                }
            }
        } else {
            JUT_WARN(124, "%s", "JASPoolAllocObject::<JASTrack>::operator new failed .\n");
            continue;
        }
    }
}

void JAISeq::releaseChildTracks_() {
    for (u32 i = 0; i < 2; i++) {
        JASTrack* track = inner_.outputTrack.getChild(i);
        if (track) {
            for (u32 j = 0; j < 16; j++) {
                JASTrack* track2 = track->getChild(j);
                if (track2) {
                    delete track2;
                }
            }
            delete track;
        }
    }
}

bool JAISeq::prepare_getSeqData_() {
    if (getID().isAnonymous()) {
        return true;
    }

    JAISeqDataMgr* seqDataMgr = inner_.seqMgr->getSeqDataMgr();
    JUT_ASSERT(162, seqDataMgr);

    switch (seqDataMgr->getSeqData(getID(), &inner_.mSeqData)) {
    case 0:
        OS_REPORT("Failed In Loading Sequence Data:SEQ(0x%08x)\n", getID());
        status_.state.unk = 2;
        break;
    case 1:
        increasePrepareCount_JAISound_();
        return false;
    case 2:
        return true;
    }

    return false;
}

bool JAISeq::prepare_(const JASSoundParams& params, JAISoundActivity activity) {
    switch (status_.state.unk) {
    case 6:
        JUT_ASSERT(193, false);
        break;
    case 2:
        stop_JAISound_();
        break;
    case 0:
        if (prepare_getSeqData_()) {
            status_.state.unk = 5;
            playSeqData_(params, activity);
            return true;
        }
        break;
    case 1:
        if (prepare_getSeqData_()) {
            status_.state.unk = 3;
        }
        return false;
    case 4:
        status_.state.unk = 5;
        playSeqData_(params, activity);
        return true;
    case 5:
        return true;
    }

    return false;
}

void JAISeq::JAISeqMgr_calc_() {
    if (inner_.outputTrack.getStatus() == JASTrack::STATUS_STOPPED || (inner_.outputTrack.getStatus() == JASTrack::STATUS_FREE && status_.state.unk == 2)) {
        die_();
        return;
    }

    if (calc_JAISound_()) {
        inner_.mTempoMgr.calc();
        for (int i = 0; i < 32; i++) {
            if (inner_.mSoundChild[i]) {
                inner_.mSoundChild[i]->calc();
            }
        }
        if (field_0x3a8) {
            field_0x3a8->calc(this);
        }
    }
}

void JAISeq::die_() {
    JUT_ASSERT(269, JASTrack_isFreeOrStopped( & inner_.outputTrack ));

    for (int i = 0; i < 32; i++) {
        if (inner_.mSoundChild[i]) {
            delete inner_.mSoundChild[i];
            inner_.mSoundChild[i] = NULL;
        }
    }

    if (field_0x3a8) {
        JUT_ASSERT(281, inner_.strategyMgr);
        inner_.strategyMgr->virtual4(field_0x3a8);
        field_0x3a8 = NULL;
    }

    releaseChildTracks_();
    die_JAISound_();
}

bool JAISeq::JAISound_tryDie_() {
    if (JASTrack_isFreeOrStopped(&inner_.outputTrack)) {
        die_();
        return true;
    }

    fader_.forceOut();
    status_.state.flags.flag1 = 1;

    {
        JASCriticalSection cs;
        inner_.outputTrack.stopSeq();
    }

    return false;
}

void JAISeq::mixOut_(const JASSoundParams& params, JAISoundActivity activity) {
    inner_.outputTrack.setTempoRate(inner_.mTempoMgr.getTempo());
    inner_.outputTrack.mute(activity.field_0x0.flags.flag1 || status_.isMute());
    inner_.outputTrack.pause(activity.field_0x0.flags.flag2 || status_.isPaused());

    JASSoundParams outParams;
    params_.mixOutAll(params, &outParams, fader_.getIntensity());

    if (field_0x3a8) {
        field_0x3a8->virtual4(this, outParams);
    }

    if (audible_) {
        int maxChannels = audience_->getMaxChannels();
        for (int i = 0; i < maxChannels; i++) {
            audience_->mixChannelOut(outParams, audible_, i);
        }
    } else {
        inner_.mSoundParams = outParams;
        for (int i = 0; i < NUM_CHILDREN; i++) {
            if (inner_.mSoundChild[i]) {
                JASTrack* track = JAISeq::getChildTrack(i);
                if (track) {
                    inner_.mSoundChild[i]->mixOut(track);
                }
            }
        }
    }
}

void JAISeq::JAISeqMgr_mixOut_(const JASSoundParams& params, JAISoundActivity activity) {
    if (prepare_(params, activity) && inner_.outputTrack.getStatus() == JASTrack::STATUS_RUN) {
        mixOut_(params, activity);
    }
}

s32 JAISeq::getNumChild() const {
    return NUM_CHILDREN;
}

JAISoundChild* JAISeq::getChild(int index) {
    JUT_ASSERT(371, status_.isAlive());
    JUT_ASSERT(372, index >= 0);
    JUT_ASSERT(373, index < NUM_CHILDREN);

    if (inner_.mSoundChild[index]) {
        return inner_.mSoundChild[index];
    }

    inner_.mSoundChild[index] = new JAISoundChild();
    if (!inner_.mSoundChild[index]) {
        JUT_WARN(379, "%s", "JASPoolAllocObject::<JAISoundChild>::operator new failed .\n");
        return NULL;
    }

    return inner_.mSoundChild[index];
}


void JAISeq::releaseChild(int index) {
    if (inner_.outputTrack.getStatus() == JASTrack::STATUS_RUN && inner_.mSoundChild[index]) {
        JASTrack* track = getChildTrack(index);
        if (track) {
            track->assignExtBuffer(0, NULL);
        }
        delete inner_.mSoundChild[index];
        inner_.mSoundChild[index] = NULL;
    }
}

JASTrack* JAISeq::getTrack() {
    return &inner_.outputTrack;
}

JASTrack* JAISeq::getChildTrack(int param_0) {
    u32 index = u32(param_0) >> 4;
    JASTrack* track = inner_.outputTrack.getChild(index);

    if (track) {
        u32 var_r28 = param_0 - (index << 4);
        JASTrack* track2 = track->getChild(var_r28);
        return track2;
    }

    return NULL;
}

JAISeq* JAISeq::asSeq() {
    return this;
}

JAITempoMgr* JAISeq::getTempoMgr() {
    return &inner_.mTempoMgr;
}
