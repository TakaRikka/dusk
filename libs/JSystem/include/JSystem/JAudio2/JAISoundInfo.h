#ifndef JAISOUNDINFO_H
#define JAISOUNDINFO_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JASGadget.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundInfo : public JASGlobalInstance<JAISoundInfo> {
    IF_DUSK(using SoundTableReplacementSlot = dusk::mods::svc::audio_res::bst::SoundTableReplacementSlot);
    IF_DUSK(using SoundEffectReplacementSlot = dusk::mods::svc::audio_res::bst::SoundEffectReplacementSlot);
    IF_DUSK(using StreamReplacementSlot = dusk::mods::svc::audio_res::bst::StreamReplacementSlot);

    JAISoundInfo(bool);
    virtual int getSoundType(JAISoundID) const = 0;
    virtual int getCategory(JAISoundID) const = 0;
    virtual u32 getPriority(JAISoundID IF_DUSK_ARG(SoundTableReplacementSlot const* replacement)) const = 0;
    virtual void getSeInfo(JAISoundID, JAISe* IF_DUSK_ARG(SoundEffectReplacementSlot const* replacement)) const = 0;
    virtual void getSeqInfo(JAISoundID, JAISeq*) const = 0;
    virtual void getStreamInfo(JAISoundID, JAIStream* IF_DUSK_ARG(StreamReplacementSlot const* replacement)) const = 0;
    virtual ~JAISoundInfo();
};

#endif /* JAISOUNDINFO_H */
