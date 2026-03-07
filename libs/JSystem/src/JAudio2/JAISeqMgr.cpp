#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAISeq.h"
#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JAISoundInfo.h"

bool JAISeqMgr::isUsingSeqData(const JAISeqDataRegion& seqDataRegion) {
    JSULink<JAISeq>* link;
    for (link = mSeqList.getFirst(); link != NULL; link = link->getNext()) {
        if (seqDataRegion.intersects(link->getObject()->getSeqData())) {
            return true;
        }
    }
    return false;
}

int JAISeqMgr::releaseSeqData(const JAISeqDataRegion& seqDataRegion) {
    bool bVar1 = false;
    JSULink<JAISeq>* link;
    for (link = mSeqList.getFirst(); link != NULL; link = link->getNext()) {
        if (seqDataRegion.intersects(link->getObject()->getSeqData())) {
            link->getObject()->stop();
            bVar1 = true;
        }
    }

    return bVar1 ? RELEASE_SEQ_1 : RELEASE_SEQ_2;
}

JAISeqMgr::JAISeqMgr(bool setInstance) : JASGlobalInstance<JAISeqMgr>(setInstance), mAudience(NULL)  {
    seqDataMgr_ = NULL;
    field_0x10 = NULL;
    field_0x70 = 16;
    mMove.init();
    mActivity.init();
}

void JAISeqMgr::freeDeadSeq_() {
    JSULink<JAISeq>* i = mSeqList.getFirst();
    while (i != NULL) {
        JAISeq* seq = i->getObject();
        JSULink<JAISeq>* next = i->getNext();
        if (seq->status_.isDead()) {
            mSeqList.remove(i);
            delete seq;
        }
        i = next;
    }
}

bool JAISeqMgr::startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr) {
    if (handle != NULL && *handle) {
        (*handle)->stop();
    }

    JAISoundInfo* soundInfo = JASGlobalInstance<JAISoundInfo>::getInstance();
    int category = (soundInfo != NULL) ? soundInfo->getCategory(id) : -1;
    JAISeqData seqData(NULL, 0);

    JUT_ASSERT(81, seqDataMgr_);
    if (seqDataMgr_->getSeqData(id, &seqData) == JAISeqDataMgr::SeqDataReturnValue_0) {
        return false;
    }

    JAISeq* seq = beginStartSeq_();
    if (seq != NULL) {
        seq->JAISeqMgr_startID_(id, posPtr, mAudience, category, field_0x70);
        if (endStartSeq_(seq, handle)) {
            if (soundInfo != NULL) {
                soundInfo->getSeqInfo(id, seq);
            }
            return true;
        }
    }

    return false;
}

void JAISeqMgr::calc() {
    mMove.calc();

    JSULink<JAISeq>* i;
    for (i = mSeqList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->JAISeqMgr_calc_();
    }
    freeDeadSeq_();
}

void JAISeqMgr::stop() {
    JSULink<JAISeq>* i;
    for (i = mSeqList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->stop();
    }
}

void JAISeqMgr::stop(u32 fadeTime) {
    JSULink<JAISeq>* i;
    for (i = mSeqList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->stop(fadeTime);
    }
}

void JAISeqMgr::stopSoundID(JAISoundID id) {
    JSULink<JAISeq>* i;
    if (!id.isAnonymous()) {
        for (i = mSeqList.getFirst(); i != NULL; i = i->getNext()) {
            if ((u32)i->getObject()->getID() == (u32)id) {
                i->getObject()->stop();
            }
        }
    }
}

void JAISeqMgr::mixOut() {
    JSULink<JAISeq>* i;
    for (i = mSeqList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->JAISeqMgr_mixOut_(mMove.params_, mActivity);
    }
}

JAISeq* JAISeqMgr::beginStartSeq_() {
    JAISeq* seq = new JAISeq(this, field_0x10);
    if (seq == NULL) {
        JUT_WARN(273, "%s", "JASPoolAllocObject::<JAISeq>::operator new failed .\n");
    }
    return seq;
}

bool JAISeqMgr::endStartSeq_(JAISeq* seq, JAISoundHandle* handle) {
    JAISeq* sound = seq->getObject();
    if (sound != NULL) {
        if (sound->status_.isAlive()) {
            mSeqList.append(seq);
            if (handle != NULL) {
                sound->attachHandle(handle);
            }
            return true;
        }
        delete sound;
    }
    return false;
}
