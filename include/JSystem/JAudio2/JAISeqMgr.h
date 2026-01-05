#ifndef JAISEQMGR_H
#define JAISEQMGR_H

#include "JSystem/JAudio2/JAISeqDataMgr.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JSupport/JSUList.h"

struct JAIAudience;
class JAISeq;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISeqMgr : public JASGlobalInstance<JAISeqMgr>, public JAISeqDataUser {
public:
    enum ReleaseSeqResult {
        RELEASE_SEQ_1 = 1,
        RELEASE_SEQ_2 = 2,
    };
    JAISeqMgr(bool setInstance);
    void freeDeadSeq_();
    bool startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr);
    void calc();
    void stop();
    void stop(u32 fadeTime);
    void stopSoundID(JAISoundID id);
    void mixOut();
    JAISeq* beginStartSeq_();
    bool endStartSeq_(JAISeq* seq, JAISoundHandle* handle);

    /* 0x04 */ JAISoundActivity mActivity;

    virtual ~JAISeqMgr() {}
    virtual bool isUsingSeqData(const JAISeqDataRegion& seqDataRegion);
    virtual int releaseSeqData(const JAISeqDataRegion& seqDataRegion);

    void setAudience(JAIAudience* audience) { mAudience = audience; }
    JAIAudience* getAudience() { return mAudience; }

    JAISeqDataMgr* getSeqDataMgr() { return seqDataMgr_; }

    void setSeqDataMgr(JAISeqDataMgr* seqDataMgr) {
        JUT_ASSERT(124, !isActive());
        resetSeqDataMgr();
        seqDataMgr_ = seqDataMgr;
        seqDataMgr_->setSeqDataUser(this);
    }

    void resetSeqDataMgr() {
        JUT_ASSERT(131, !isActive());
        if (seqDataMgr_) {
            seqDataMgr_->setSeqDataUser(NULL);
            seqDataMgr_ = NULL;
        }
    }

    JAISoundParamsMove* getParams() { return &mMove; }
    bool isActive() const { return mSeqList.getNumLinks() != 0; }
    int getNumActiveSeqs() const { return mSeqList.getNumLinks(); }
    void pause(bool paused) { mActivity.field_0x0.flags.flag2 = paused; }

private:
    /* 0x08 */ JAIAudience* mAudience;
    /* 0x0C */ JAISeqDataMgr* seqDataMgr_;
    /* 0x10 */ JAISoundStrategyMgr<JAISeq>* field_0x10;
    /* 0x14 */ JAISoundParamsMove mMove;
    /* 0x64 */ JSUList<JAISeq> mSeqList;
    /* 0x70 */ int field_0x70;
};  // Size: 0x74

#endif /* JAISEQMGR_H */
