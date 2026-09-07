#ifndef JAISTREAMDATAMGR_H
#define JAISTREAMDATAMGR_H

#include "JSystem/JAudio2/JAISound.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAIStreamDataMgr {
    IF_DUSK(using StreamReplacementSlot2 = dusk::mods::svc::audio_res::bst::StreamReplacementSlot);

    virtual s32 getStreamFileEntry(JAISoundID IF_DUSK_ARG(StreamReplacementSlot2 const*)) = 0;
    virtual ~JAIStreamDataMgr();
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAIStreamAramMgr {
    virtual void* newStreamAram(u32*) = 0;
    virtual bool deleteStreamAram(u32) = 0;
    virtual ~JAIStreamAramMgr();
};

#endif /* JAISTREAMDATAMGR_H */
