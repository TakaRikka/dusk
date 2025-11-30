#ifndef JAISTREAMMGR_H
#define JAISTREAMMGR_H

#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JAIStream.h"
#include "JSystem/JUtility/JUTAssert.h"

class JAIStreamAramMgr;
class JAIStreamDataMgr;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAIStreamMgr : public JASGlobalInstance<JAIStreamMgr> {
public:
    JAIStreamMgr(bool setInstance);
    bool startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr);
    void freeDeadStream_();
    void calc();
    void stop();
    void stop(u32 fadeTime);
    void stopSoundID(JAISoundID id);
    void mixOut();
    JAIStream* newStream_();
    bool isActive() const { return mStreamList.getNumLinks() != 0; }

    JAIAudience* getAudience() { return mAudience; }
    JAISoundParamsMove* getParams() { return &mParams; }
    JAIStreamAramMgr* getStreamAramMgr() { return mStreamAramMgr; }
    JSUList<JAIStream>* getStreamList() { return &mStreamList; }

    void setStreamDataMgr(JAIStreamDataMgr* streamDataMgr) {
        JUT_ASSERT(139, !isActive());
        streamDataMgr_ = streamDataMgr;
    }

    void setStreamAramMgr(JAIStreamAramMgr* streamDataMgr) {
        JUT_ASSERT(157, !isActive());
        mStreamAramMgr = streamDataMgr;
    }

    void pause(bool paused) { mActivity.field_0x0.flags.flag2 = paused; }

private:
    /* 0x00 */ JAISoundActivity mActivity;
    /* 0x04 */ JAISoundParamsMove mParams;
    /* 0x54 */ JAIAudience* mAudience;
    /* 0x58 */ JSUList<JAIStream> mStreamList;
    /* 0x64 */ JAIStreamDataMgr* streamDataMgr_;
    /* 0x68 */ JAIStreamAramMgr* mStreamAramMgr;
    /* 0x6C */ JAISoundStrategyMgr<JAIStream>* field_0x6c;
};  // Size: 0x70

#endif /* JAISTREAMMGR_H */
