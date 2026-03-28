#ifndef Z2SOUNDHANDLES_H
#define Z2SOUNDHANDLES_H

#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

class Z2SoundHandlePool : public JAISoundHandle, public JSULink<Z2SoundHandlePool>, public JASPoolAllocObject<Z2SoundHandlePool> {
public:
    Z2SoundHandlePool() : JAISoundHandle(), JSULink<Z2SoundHandlePool>(this) {}
};

class Z2SoundHandles : protected JSUList<Z2SoundHandlePool> {
public:
    Z2SoundHandles();
    ~Z2SoundHandles();

    void initHandlesPool(u8 handleNum);
    void deleteHandlesPool();
    Z2SoundHandlePool* getFreeHandle();
    Z2SoundHandlePool* getLowPrioSound(JAISoundID soundID);

    bool isActive() const;

    Z2SoundHandlePool* getHandleSoundID(JAISoundID soundID);
    Z2SoundHandlePool* getHandleUserData(uintptr_t userData);

    void stopAllSounds(u32 fadeTime);

    void stopSound(JAISoundID soundID, u32 fadeTime) {
        // u32 cast is a fakematch
        JAISoundHandle* phandle = getHandleSoundID((u32)soundID);
        if (phandle != NULL) {
            (*phandle)->stop(fadeTime);
        }
    }

    void setPos(const JGeometry::TVec3<f32>& pos);

    int getNumHandles() const { return getNumLinks(); }

    Z2SoundHandlePool* getHandle(int index) {
        JUT_ASSERT(49, index >= 0);
        JUT_ASSERT(50, index < getNumLinks());
        return (Z2SoundHandlePool*)getNth(index);
    }

private:
    /* 0xC */ u8 handleNum_;
};

#endif /* Z2SOUNDHANDLES_H */
