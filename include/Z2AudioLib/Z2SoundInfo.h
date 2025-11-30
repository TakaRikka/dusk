#ifndef Z2SOUNDINFO_H
#define Z2SOUNDINFO_H

#include "JSystem/JAudio2/JAISoundInfo.h"
#include "JSystem/JAudio2/JAIStreamDataMgr.h"
#include "JSystem/JAudio2/JAUSoundInfo.h"
#include "JSystem/JAudio2/JAUSoundTable.h"

class Z2SoundInfo : public JAISoundInfo, public JAUSoundInfo, public JAIStreamDataMgr, public JASGlobalInstance<Z2SoundInfo> {
public:
    Z2SoundInfo() : JAISoundInfo(true), JAUSoundInfo(true), JASGlobalInstance<Z2SoundInfo>(true) {}
    virtual u16 getAudibleSw(JAISoundID soundID) const;
    virtual u16 getBgmSeqResourceID(JAISoundID soundID) const;
    virtual s32 getStreamFileEntry(JAISoundID soundID);
    virtual int getSoundType(JAISoundID soundID) const;
    virtual int getCategory(JAISoundID soundID) const;
    virtual u32 getPriority(JAISoundID soundID) const;
    virtual void getSeInfo(JAISoundID soundID, JAISe* sePtr) const;
    virtual void getSeqInfo(JAISoundID soundID, JAISeq* seqPtr) const;
    virtual void getStreamInfo(JAISoundID soundID, JAIStream* streamPtr) const;
    virtual ~Z2SoundInfo() {}

    JAUAudibleParam getAudibleSwFull(JAISoundID soundID);
    const char* getStreamFilePath(JAISoundID soundID);
    int getSwBit(JAISoundID soundID) const;
    void getSoundInfo_(JAISoundID soundID, JAISound* soundPtr) const;

    BOOL isValid() const {
        return JASGlobalInstance<JAUSoundTable>::getInstance() != NULL && JASGlobalInstance<JAUSoundTable>::getInstance()->isValid();
    }
};


inline Z2SoundInfo* Z2GetSoundInfo() {
    return JASGlobalInstance<Z2SoundInfo>::getInstance();
}

#endif /* Z2SOUNDINFO_H */
