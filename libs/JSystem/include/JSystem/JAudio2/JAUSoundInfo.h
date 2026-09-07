#ifndef JAUSOUNDINFO_H
#define JAUSOUNDINFO_H

#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JAudio2/JAUAudibleParam.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUSoundInfo : public JASGlobalInstance<JAUSoundInfo> {
public:
    IF_DUSK(using SoundTableReplacementSlot2 = dusk::mods::svc::audio_res::bst::SoundTableReplacementSlot);

    JAUSoundInfo(bool param_0) : JASGlobalInstance<JAUSoundInfo>(param_0) {}
    virtual u16 getAudibleSw(JAISoundID IF_DUSK_ARG(SoundTableReplacementSlot2 const*)) const = 0;
    virtual u16 getBgmSeqResourceID(JAISoundID) const = 0;
};

#endif /* JAUSOUNDINFO_H */
