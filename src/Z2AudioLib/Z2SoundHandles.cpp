#include "Z2AudioLib/Z2SoundHandles.h"
#include "JSystem/JAudio2/JAISoundInfo.h"

Z2SoundHandles::Z2SoundHandles() {
    handleNum_ = 0;
}

Z2SoundHandles::~Z2SoundHandles() {
    deleteHandlesPool();
}

void Z2SoundHandles::initHandlesPool(u8 handleNum) {
    handleNum_ = handleNum;
}

void Z2SoundHandles::deleteHandlesPool() {
    JSULink<Z2SoundHandlePool>* i;
    while (i = getFirst(), i != NULL) {
        Z2SoundHandlePool* handle = i->getObject();
        JUT_ASSERT(48, handle->getSupervisor() == this);

        remove(handle);
        delete handle;
    }

    handleNum_ = 0;
}

Z2SoundHandlePool* Z2SoundHandles::getHandleSoundID(JAISoundID soundID) {
    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        Z2SoundHandlePool* handle = i->getObject();
        if (handle->isSoundAttached()) {
            if ((*handle)->getID() == soundID) {
                return handle;
            }
        }
    }

    return NULL;
}

Z2SoundHandlePool* Z2SoundHandles::getHandleUserData(u32 userData) {
    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        Z2SoundHandlePool* handle = i->getObject();
        if (handle->isSoundAttached()) {
            if ((*handle)->getUserData() == userData) {
                return handle;
            }
        }
    }

    return NULL;
}

Z2SoundHandlePool* Z2SoundHandles::getFreeHandle() {
    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        Z2SoundHandlePool* handle = i->getObject();
        if (!handle->isSoundAttached()) {
            return handle;
        }
    }

    if (getNumHandles() < handleNum_) {
        Z2SoundHandlePool* handle = new Z2SoundHandlePool();
        if (handle != NULL) {
            append(handle);
            JUT_ASSERT(113, handle->getSupervisor() == this);
            return handle;
        }

        JUT_WARN_DEVICE(118, 1, "%s", "cannot Alloc Handle !!\n");
    }

    return NULL;
}

Z2SoundHandlePool* Z2SoundHandles::getLowPrioSound(JAISoundID soundID) {
    JAISoundInfo* soundInfo = JASGlobalInstance<JAISoundInfo>::getInstance();
    u32 low_prio = 0xFFFF;
    Z2SoundHandlePool* rethandle = NULL;

    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        Z2SoundHandlePool* handle = i->getObject();
        if (!handle->isSoundAttached()) {
            return handle;
        }

        u32 prio = soundInfo->getPriority((*handle)->getID());
        if (prio < low_prio) {
            low_prio = prio;
            rethandle = handle;
        }
    }

    #if DEBUG
    if (rethandle != NULL) {
        JUT_ASSERT(163, rethandle->getSupervisor() == this);
    }
    #endif

    if (soundInfo->getPriority(soundID) >= low_prio) {
        return rethandle;
    }

    return NULL;
}

void Z2SoundHandles::stopAllSounds(u32 fadeTime) {
    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        JAISoundHandle* handle = i->getObject();
        if (handle && (*handle)) {
            (*handle)->stop(fadeTime);
        }
    }
}

bool Z2SoundHandles::isActive() const {
    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        if (i->getObject()->isSoundAttached()) {
            return true;
        }
    }

    return false;
}

void Z2SoundHandles::setPos(const JGeometry::TVec3<f32>& pos) {
    JSULink<Z2SoundHandlePool>* i;
    for (i = getFirst(); i != NULL; i = i->getNext()) {
        Z2SoundHandlePool* handle = i->getObject();
        JUT_ASSERT(209, handle);
        JUT_ASSERT(210, handle->getSupervisor() == this);

        if (handle->isSoundAttached()) {
            (*handle)->setPos(pos);
        }
        else {
            remove(handle);
            delete handle;
        }
    }
}
