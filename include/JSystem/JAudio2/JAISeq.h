#ifndef JAISEQ_H
#define JAISEQ_H

#include "JSystem/JAudio2/JAISeqDataMgr.h"
#include "JSystem/JAudio2/JASTrack.h"
#include "JSystem/JSupport/JSUList.h"

class JAISeqMgr;
class JAISoundChild;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISeq : public JAISound, public JSULink<JAISeq>, public JASPoolAllocObject<JAISeq> {
public:
    static const int NUM_CHILDREN = 32;

    class TInner {
    public:
        TInner() : mSeqData(NULL, 0) {}

        /* 0x000 */ JASTrack outputTrack;
        /* 0x248 */ JAISoundChild* mSoundChild[NUM_CHILDREN];
        /* 0x2C8 */ JAITempoMgr mTempoMgr;
        /* 0x2D8 */ JASSoundParams mSoundParams;
        /* 0x2EC */ JAISeqData mSeqData;
        /* 0x2F4 */ s32 field_0x39c;
        /* 0x2F8 */ JAISeqMgr* seqMgr;
        /* 0x2FC */ JAISoundStrategyMgr<JAISeq>* strategyMgr;
    };

    virtual s32 getNumChild() const;
    virtual JAISoundChild* getChild(int index);
    virtual void releaseChild(int index);
    virtual JAISeq* asSeq();
    virtual JASTrack* getTrack();
    virtual JASTrack* getChildTrack(int);
    virtual JAITempoMgr* getTempoMgr();
    virtual bool JAISound_tryDie_();

    JAISeq(JAISeqMgr* seqMgr, JAISoundStrategyMgr<JAISeq>* soundStrategyMgr);
    void JAISeqMgr_startID_(JAISoundID id, const JGeometry::TVec3<f32>* posPtr, JAIAudience* audience,
                                           int category, int);
    void playSeqData_(const JASSoundParams& params, JAISoundActivity activity);
    void reserveChildTracks_(int);
    void releaseChildTracks_();
    bool prepare_getSeqData_();
    bool prepare_(const JASSoundParams& params, JAISoundActivity activity);
    void JAISeqMgr_calc_();
    void die_();
    void mixOut_(const JASSoundParams& params, JAISoundActivity activity);
    void JAISeqMgr_mixOut_(const JASSoundParams& params, JAISoundActivity activity);

    const JAISeqData& getSeqData() const { return inner_.mSeqData; }
    s32 getCategory() const { return inner_.field_0x39c; }

    /* 0x0A8 */ TInner inner_;
    /* 0x3A8 */ JAISoundStrategyMgr__unknown<JAISeq>* field_0x3a8;
};

#endif /* JAISEQ_H */
