#ifndef JAISTREAM_H
#define JAISTREAM_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JASAramStream.h"
#include "JSystem/JSupport/JSUList.h"

class JAIStreamMgr;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAIStream : public JASPoolAllocObject<JAIStream>, public JAISound, public JSULink<JAIStream> {
public:
    struct TInner {
        JASAramStream aramStream_;
    };

    JAIStream(JAIStreamMgr* streamMgr, JAISoundStrategyMgr<JAIStream>* soundStrategyMgr);
    void JAIStreamMgr_startID_(JAISoundID id, s32 streamFileEntry,
                                              const JGeometry::TVec3<f32>* posPtr, JAIAudience* audience,
                                              int category);
    bool prepare_prepareStream_();
    void prepare_();
    void prepare_startStream_();
    void JAIStreamMgr_mixOut_(const JASSoundParams& inParams, JAISoundActivity activity);
    void die_JAIStream_();
    bool JAISound_tryDie_();
    void JAIStreamMgr_calc_();
    s32 getNumChild() const;
    JAISoundChild* getChild(int index);
    void releaseChild(int index);
    JASTrack* getTrack();
    JASTrack* getChildTrack(int);
    JAIStream* asStream();
    JAITempoMgr* getTempoMgr();

    void* JAIStreamMgr_getAramAddr_() const { return streamAramAddr_; }

    static const int NUM_CHILDREN = 6;

    /* 0x0A8 */ TInner inner_;
    /* 0x290 */ int field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ int field_0x298;
    /* 0x29C */ void* streamAramAddr_;
    /* 0x2A0 */ JAISoundChild* children_[NUM_CHILDREN];
    /* 0x2B8 */ JAIStreamMgr* streamMgr_;
    /* 0x2BC */ JAISoundStrategyMgr__unknown<JAIStream>* field_0x2bc;
    /* 0x2C0 */ JAISoundStrategyMgr<JAIStream>* field_0x2c0;
    /* 0x2C4 */ bool field_0x2c4;
    /* 0x2C5 */ u8 field_0x2c5;
    /* 0x2C6 */ u8 field_0x2c6;
};

#endif /* JAISTREAM_H */
