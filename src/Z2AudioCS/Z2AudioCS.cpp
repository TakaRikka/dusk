#include "Z2AudioCS/Z2AudioCS.h"

#include "Z2AudioCS/SpkSystem.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <revolution/types.h>
#include <revolution/wpad.h>

#define HANDLES_MAX 0x30

static SpkSoundHandle* sSpkHandles;

static u8 l_spkVolume;

void Z2AudioCS::newSpkSoundMemPool() {
    SpkSystem::newSoundMemPool(HANDLES_MAX);
}

int Z2AudioCS::init(JKRHeap* heap, JKRArchive* res, s32 param_2, s32 param_3) {
    JUT_ASSERT(59, heap);
    JUT_ASSERT(60, res);
    SpkSystem* spkSys = new(heap, 0) SpkSystem(heap);
    JUT_ASSERT(67, spkSys);

    sSpkHandles = new (heap, 0) SpkSoundHandle[HANDLES_MAX];
    JUT_ASSERT(71, sSpkHandles);

    spkSys->setResource(res, 2, 3);
    spkSys->setMasterVolume(1.0f);
    spkSys->setConfigVolume(15);
}

void Z2AudioCS::update() {
    if (JASGlobalInstance<SpkSystem>::getInstance() != NULL) {
        JASGlobalInstance<SpkSystem>::getInstance()->framework();
    }
}

void Z2AudioCS::connect(s32 chan) {
    SpkSystem::connect(chan);
    l_spkVolume = WPADGetSpeakerVolume();
}

void Z2AudioCS::disconnect(s32 chan) {
    SpkSystem::disconnect(chan);
}

void Z2AudioCS::extensionProcess(s32 chan, s32 param_1) {
    SpkSystem::extensionProcess(chan, param_1);
}

static SpkSoundHandle* getFreeSpkHandle(void) {
    JUT_ASSERT(125, JASGlobalInstance<SpkSystem>::getInstance());
    JUT_ASSERT(126, sSpkHandles);

    SpkSoundHandle* highestPriorityHandle = NULL;
    s32 highestPriority = 255;
    for (s32 i = 0; i < HANDLES_MAX; i++) {
        if (!sSpkHandles[i].isSoundAttached()) {
            return &sSpkHandles[i];
        }

        if (sSpkHandles[i]->getPriority() < highestPriority) {
            highestPriorityHandle = &sSpkHandles[i];
            highestPriority = sSpkHandles[i]->getPriority();
        }
    }

    return highestPriorityHandle;
}

SpkSoundHandle* Z2AudioCS::getHandleSoundID(s32 soundNum) {
    JUT_ASSERT(145, JASGlobalInstance<SpkSystem>::getInstance());
    JUT_ASSERT(146, sSpkHandles);

    for (s32 i = 0; i < HANDLES_MAX; i++) {
        if (!sSpkHandles[i].isSoundAttached()) {
            continue;
        }

        if (sSpkHandles[i]->getSoundNum() == soundNum) {
            return &sSpkHandles[i];
        }
    }

    return NULL;
}

SpkSoundHandle* Z2AudioCS::start(s32 id, s32 chan) {
    if (JASGlobalInstance<SpkSystem>::getInstance() == NULL) {
        return NULL;
    }
    if (sSpkHandles == NULL) {
        return NULL;
    }
    if (l_spkVolume == 0) {
        return NULL;
    }

    OS_REPORT("[Z2AudioCS::start] id:%d ch:%d\n", id, chan);
    SpkSoundHandle* handle = getFreeSpkHandle();
    JUT_ASSERT(172, handle);

    JASGlobalInstance<SpkSystem>::getInstance()->startSound(chan, id, handle);
    return handle;
}

SpkSoundHandle* Z2AudioCS::startLevel(s32 id, s32 chan) {
    if (JASGlobalInstance<SpkSystem>::getInstance() == NULL) {
        return NULL;
    }
    if (sSpkHandles == NULL) {
        return NULL;
    }
    if (l_spkVolume == 0) {
        return NULL;
    }

    OS_REPORT("[Z2AudioCS::startLevel] id:%d ch:%d\n", id, chan);

    SpkSoundHandle* handle = getHandleSoundID(id);
    if (handle == NULL) {
        handle = getFreeSpkHandle();
    }
    JUT_ASSERT(191, handle);

    JASGlobalInstance<SpkSystem>::getInstance()->startLevelSound(chan, id, handle);
    return handle;
}

s32 Z2AudioCS::getName(s32 num) {
    if (JASGlobalInstance<SpkSystem>::getInstance() == NULL) {
        return 0;
    }
    if (JASGlobalInstance<SpkSystem>::getInstance()->getData() == NULL) {
        return 0;
    }

    return JASGlobalInstance<SpkSystem>::getInstance()->getData()->getTableMgr().getName(num);
}

s32 Z2AudioCS::getNumOfSound(void) {
    if (JASGlobalInstance<SpkSystem>::getInstance() == NULL) {
        return 0;
    }
    if (JASGlobalInstance<SpkSystem>::getInstance()->getData() == NULL) {
        return 0;
    }

    return JASGlobalInstance<SpkSystem>::getInstance()->getData()->getTableMgr().getNumOfSound();
}

void Z2AudioCS::stopAll(s32 chan, s32 msec) {
    if (JASGlobalInstance<SpkSystem>::getInstance() == NULL) {
        return;
    }

    JASGlobalInstance<SpkSystem>::getInstance()->stopAll(chan, msec);
}

void Z2AudioCS::stop(s32 chan) {
    stopAll(chan, 0);
}
