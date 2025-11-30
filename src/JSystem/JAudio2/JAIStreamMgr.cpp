#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAIStreamMgr.h"
#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JAIStreamDataMgr.h"
#include "JSystem/JAudio2/JAISoundInfo.h"

JAIStreamMgr::JAIStreamMgr(bool setInstance) : JASGlobalInstance<JAIStreamMgr>(setInstance) {
    streamDataMgr_ = NULL;
    mStreamAramMgr = NULL;
    field_0x6c = NULL;
    mAudience = NULL;
    mParams.init();
    mActivity.init();
}

bool JAIStreamMgr::startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr) {
    JUT_ASSERT(37, streamDataMgr_);
    if (handle != NULL && *handle) {
        (*handle)->stop();
    }

    s32 streamFileEntry = streamDataMgr_->getStreamFileEntry(id);
    if (streamFileEntry < 0) {
        JUT_WARN(46, "Cannot find the stream file entry for ID:%08x\n", id.id_.composite_)
        return false;
    } 

    JAIStream* stream = newStream_();
    JAISoundInfo* soundInfo = JASGlobalInstance<JAISoundInfo>::getInstance();

    int category = -1;
    if (soundInfo != NULL) {
        category = soundInfo->getCategory(id);
    }

    if (stream == NULL) {
        return false;
    }

    stream->JAIStreamMgr_startID_(id, streamFileEntry, posPtr, mAudience, category);
    if (soundInfo != NULL) {
        soundInfo->getStreamInfo(id, stream);
    }

    if (handle != NULL) {
        stream->attachHandle(handle);
    }

    return false;
}

void JAIStreamMgr::freeDeadStream_() {
    JSULink<JAIStream>* i = mStreamList.getFirst();
    while (i != NULL) {
        JAIStream* stream = i->getObject();
        JSULink<JAIStream>* next = i->getNext();
        if (stream->status_.isDead()) {
            mStreamList.remove(i);
            void* aramAddr = stream->JAIStreamMgr_getAramAddr_();
            if (aramAddr != NULL) {
                bool result = mStreamAramMgr->deleteStreamAram((u32)aramAddr);
                JUT_ASSERT(105, result);
            }
            
            delete stream;
        }
        i = next;
    }
}

void JAIStreamMgr::calc() {
    JSULink<JAIStream>* i;
    mParams.calc();
    for (i = mStreamList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->JAIStreamMgr_calc_();
    }
    freeDeadStream_();
}

void JAIStreamMgr::stop() {
    JSULink<JAIStream>* i;
    for (i = mStreamList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->stop();
    }
}

void JAIStreamMgr::stop(u32 fadeTime) {
    JSULink<JAIStream>* i;
    for (i = mStreamList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->stop(fadeTime);
    }
}

void JAIStreamMgr::stopSoundID(JAISoundID id) {
    JSULink<JAIStream>* i;
    for (i = mStreamList.getFirst(); i != NULL; i = i->getNext()) {
        if ((u32)i->getObject()->getID() == (u32)id) {
            i->getObject()->stop();
        }
    }
}

void JAIStreamMgr::mixOut() {
    JSULink<JAIStream>* i;
     for (i = mStreamList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->JAIStreamMgr_mixOut_(mParams.params_, mActivity);
    }
}

JAIStream* JAIStreamMgr::newStream_() {
    if (mStreamAramMgr == NULL) {
        JUT_WARN(229, "%s", "JAIStreamAramMgr must be set.\n");
        return NULL;
    } 

    JAIStream* stream = new JAIStream(this, field_0x6c);
    if (stream == NULL) {
        JUT_WARN(235, "%s", "JASPoolAllocObject::<JAIStream>::operator new failed .\n");
        return NULL;
    }

    mStreamList.append(stream);
    return stream;
}
