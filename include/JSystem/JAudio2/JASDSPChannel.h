#ifndef JASDSPCHANNEL_H
#define JASDSPCHANNEL_H

#include "JSystem/JAudio2/JASDSPInterface.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASDSPChannel {
    typedef s32 (*Callback)(u32, JASDsp::TChannel*, void*);

    enum Status {
        /* 0 */ STATUS_ACTIVE,
        /* 1 */ STATUS_INACTIVE,
        /* 2 */ STATUS_DROP,
    };

    enum CallbackType {
        /* 0 */ CB_PLAY,
        /* 1 */ CB_START,
        /* 2 */ CB_STOP,
        /* 3 */ CB_DROP,
    };

    JASDSPChannel();
    void free();
    void start();
    void drop();
    void setPriority(u8);
    void updateProc();

    static void initAll();
    static JASDSPChannel* alloc(u8, Callback, void*);
    static JASDSPChannel* allocForce(u8, Callback, void*);
    static JASDSPChannel* getLowestChannel(int);
    static JASDSPChannel* getLowestActiveChannel();
    static void updateAll();
    static int killActiveChannel();
    static JASDSPChannel* getHandle(u32);

    static JASDSPChannel* sDspChannels;

    /* 0x00 */ s32 mStatus;
    /* 0x04 */ s16 mPriority;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u32 field_0xc;
    /* 0x10 */ Callback mCallback;
    /* 0x14 */ void* mCallbackData;
    /* 0x18 */ JASDsp::TChannel* mChannel;
};  // Size: 0x1C

#endif /* JASDSPCHANNEL_H */
