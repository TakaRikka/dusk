#ifndef Z2AUDIOCS_SPKSYSTEM_H
#define Z2AUDIOCS_SPKSYSTEM_H

#include "Z2AudioCS/SpkData.h"
#include "Z2AudioCS/SpkMixingBuffer.h"
#include "Z2AudioCS/SpkSound.h"
#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <revolution/types.h>

class SpkSystem : protected JASGlobalInstance<SpkSystem> {
public:
    SpkSystem(JKRHeap* heap);
    void setResource(JKRArchive* resArc, u16, u16);
    void framework(void);
    void startSound(s32 chan, s32 param_1, SpkSoundHandle* handle);
    void startLevelSound(s32 chan, s32 param_1, SpkSoundHandle* handle);
    void stopAll(s32 chan, s32 msec);
    void setMasterVolume(f32 vol);
    f32 getMasterVolume(void);
    void setConfigVolume(s32 vol);
    static void newSoundMemPool(s32 numOfSound);
    static void connect(s32 chan);
    static void disconnect(s32 chan);
    static void extensionProcess(s32, s32);

    inline SpkData* getData(void) const { return mData; }
    inline SpkMixingBuffer* getMixingBuffer(void) const { return mMixingBuffer; }

private:
    /* 0x00 */ JKRHeap* mHeap;
    /* 0x04 */ SpkData* mData;
    /* 0x08 */ SpkMixingBuffer* mMixingBuffer;
    /* 0x0C */ SpkSoundHolder* mSoundHolder;
};

#endif /* Z2AUDIOCS_SPKSYSTEM_H */
