#ifndef JAUAUDIOARCLOADER_H
#define JAUAUDIOARCLOADER_H

#include "JSystem/JAudio2/JAUAudioArcInterpreter.h"

struct JAUSection;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUAudioArcLoader : public JAUAudioArcInterpreter {
public:
    JAUAudioArcLoader(JAUSection*);
    bool load(void const*);
    virtual void readWS(u32, void const*, u32);
    virtual void readBNK(u32, void const*);
    virtual void readBSC(void const*, u32);
    virtual void readBST(void const*, u32);
    virtual void readBSTN(void const*, u32);
    virtual void readBMS(u32, void const*, u32);
    virtual void readBMS_fromArchive(u32);
    virtual void newVoiceBank(u32, u32);
    virtual void newDynamicSeqBlock(u32);
    virtual void readBSFT(void const*);
    virtual void beginBNKList(u32, u32);
    virtual void endBNKList();
    virtual void readMaxSeCategory(int, int, int);
    virtual ~JAUAudioArcLoader() {}

    /* 0x0C */ JAUSection* mSection;
};

class JAUAudioArcLoader_withoutCopy : public JAUAudioArcInterpreter {
    virtual void readWS(u32, void const*, u32);
    virtual void readBNK(u32, void const*);
    virtual void readBSC(void const*, u32);
    virtual void readBST(void const*, u32);
    virtual void readBSTN(void const*, u32);
    virtual void readBMS(u32, void const*, u32);
    virtual void readBMS_fromArchive(u32);
    virtual void newVoiceBank(u32, u32);
    virtual void newDynamicSeqBlock(u32);
    virtual void readBSFT(void const*);
    virtual void beginBNKList(u32, u32);
    virtual void endBNKList();
    virtual void readMaxSeCategory(int, int, int);
    virtual ~JAUAudioArcLoader_withoutCopy() {}

    /* 0x0C */ JAUSection* mSection;
};

#endif /* JAUAUDIOARCLOADER_H */
