#ifndef Z2SOUNDMGR_H
#define Z2SOUNDMGR_H

#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"

u16 seqCallback(JASTrack* track, u16 command);

class Z2SoundMgr : public JASGlobalInstance<Z2SoundMgr> {
public:
    Z2SoundMgr();
    void calc();
    void setIIR(JAISound* sound, const s16* iir);
    void setFilterOff(JAISound* sound);
    void resetFilterAll();
    void mixOut();
    void framework();
    void pauseAllGameSound(bool paused);
    void stopSoundID(JAISoundID soundID);
    void stopSync();
    void stop();
    void initParams();
    void multiVolumeSoundID(JAISoundID soundID, f32 volume);
    bool isPlayingSoundID(JAISoundID soundID);

    virtual bool startSound(JAISoundID soundID, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr);

    JAISeMgr* getSeMgr() { return &seMgr_; }
    const JAISeMgr* getSeMgr() const { return &seMgr_; }
    JAISeqMgr* getSeqMgr() { return &seqMgr_; }
    const JAISeqMgr* getSeqMgr() const { return &seqMgr_; }
    JAIStreamMgr* getStreamMgr() { return &streamMgr_; }

private:
    /* 0x004 */ JAISeMgr seMgr_;
    /* 0x728 */ JAISeqMgr seqMgr_;
    /* 0x79C */ JAIStreamMgr streamMgr_;
    /* 0x80C */ JAISoundID soundID_;
};  // Size: 0x810

#if VERSION != VERSION_SHIELD_DEBUG
STATIC_ASSERT(sizeof(Z2SoundMgr) == 0x810);
#endif

inline Z2SoundMgr* Z2GetSoundMgr() {
    return JASGlobalInstance<Z2SoundMgr>::getInstance();
}

#endif /* Z2SOUNDMGR_H */
