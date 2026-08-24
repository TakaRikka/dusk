#ifndef Z2SOUNDINFO_H
#define Z2SOUNDINFO_H

#include "JSystem/JAudio2/JAISoundInfo.h"
#include "JSystem/JAudio2/JAIStreamDataMgr.h"
#include "JSystem/JAudio2/JAUSoundInfo.h"
#include "JSystem/JAudio2/JAUSoundTable.h"

#define STRM_CH_SHIFT_ 2

#define STRM_CH_CENTER 1
#define STRM_CH_LEFT   2
#define STRM_CH_RIGHT  3

class Z2SoundInfo : public JAISoundInfo, public JAUSoundInfo, public JAIStreamDataMgr, public JASGlobalInstance<Z2SoundInfo> {
public:
    Z2SoundInfo() : JAISoundInfo(true), JAUSoundInfo(true), JASGlobalInstance<Z2SoundInfo>(true) {}
    virtual u16 getAudibleSw(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) const;
    virtual u16 getBgmSeqResourceID(JAISoundID soundID) const;
    virtual s32 getStreamFileEntry(JAISoundID soundID IF_DUSK_ARG(StreamReplacementSlot const* replacement));
    virtual int getSoundType(JAISoundID soundID) const;
    virtual int getCategory(JAISoundID soundID) const;
    virtual u32 getPriority(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) const;
    virtual void getSeInfo(JAISoundID soundID, JAISe* sePtr IF_DUSK_ARG(SoundEffectReplacementSlot const* replacement)) const;
    virtual void getSeqInfo(JAISoundID soundID, JAISeq* seqPtr) const;
    virtual void getStreamInfo(JAISoundID soundID, JAIStream* streamPtr IF_DUSK_ARG(StreamReplacementSlot const* replacement)) const;
    virtual ~Z2SoundInfo() {}

    JAUAudibleParam getAudibleSwFull(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement));
    const char* getStreamFilePath(JAISoundID soundID IF_DUSK_ARG(StreamReplacementSlot const* replacement));
    int getSwBit(JAISoundID soundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) const;
    void getSoundInfo_(JAISoundID soundID, JAISound* soundPtr) const;

    BOOL isValid() const {
        return JASGlobalInstance<JAUSoundTable>::getInstance() != NULL && JASGlobalInstance<JAUSoundTable>::getInstance()->isValid();
    }
};


inline Z2SoundInfo* Z2GetSoundInfo() {
    return JASGlobalInstance<Z2SoundInfo>::getInstance();
}

#endif /* Z2SOUNDINFO_H */
