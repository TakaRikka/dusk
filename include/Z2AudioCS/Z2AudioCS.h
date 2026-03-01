#ifndef Z2AUDIOCS_H
#define Z2AUDIOCS_H

#include "Z2AudioCS/SpkTable.h"
#include "Z2AudioCS/SpkSound.h"
#include <dolphin/types.h>

class JKRHeap;
class JKRArchive;

class Z2AudioCS {
public:
    static void newSpkSoundMemPool();
    static int init(JKRHeap* heap, JKRArchive* res, s32 param_2, s32 param_3);
    static void update();
    static void connect(s32 chan);
    static void disconnect(s32 chan);
    static void extensionProcess(s32 chan, s32 param_1);
    static SpkSoundHandle* getHandleSoundID(s32 soundNum);
    static SpkSoundHandle* start(s32 id, s32 chan);
    static SpkSoundHandle* startLevel(s32 id, s32 chan);
    static s32 getName(s32 num);
    static s32 getNumOfSound(void);
    static void stopAll(s32 chan, s32 msec);
    static void stop(s32 chan);
};

#endif /* Z2AUDIOCS_H */
