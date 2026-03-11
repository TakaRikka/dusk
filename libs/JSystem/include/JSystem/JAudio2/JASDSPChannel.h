#ifndef JASDSPCHANNEL_H
#define JASDSPCHANNEL_H

#include "JSystem/JAudio2/JASDSPInterface.h"

/**
 * @ingroup jsystem-jaudio
 * Responsible for allocating and prioritizing the available hardware DSP channels.
 */
struct JASDSPChannel {
    /**
     * Callback used to update DSP channel information on a regular basis.
     * Parameters are a CallbackType, hardware channel, and specified user data.
     * Return -1 to immediately abort playback.
     */
    typedef s32 (*Callback)(u32, JASDsp::TChannel*, void*);

    enum Status {
        /* 0 */ STATUS_ACTIVE,
        /* 1 */ STATUS_INACTIVE,
        /* 2 */ STATUS_DROP,
    };

    enum CallbackType {
        /**
         * Fired on a regular basis during play to update parameters.
         */
        /* 0 */ CB_PLAY,

        /**
         * Fired once, when the channel starts playing.
         */
        /* 1 */ CB_START,

        /**
         * Fired once, when the channel naturally finishes playing.
         */
        /* 2 */ CB_STOP,

        /**
         * Fired once, if the channel is abruptly stopped due to an error or prioritization.
         */
        /* 3 */ CB_DROP,
    };

    JASDSPChannel();
    void free();
    void start();
    void drop();
    void setPriority(u8);
    void updateProc();
    u8 getStatus() const { return mStatus; }

    static void initAll();
    static JASDSPChannel* alloc(u8, Callback, void*);
    static JASDSPChannel* allocForce(u8, Callback, void*);
    static JASDSPChannel* getLowestChannel(int);
    static JASDSPChannel* getLowestActiveChannel();
    static void updateAll();
    static int killActiveChannel();
    static JASDSPChannel* getHandle(u32);
    static u32 getNumUse();
    static u32 getNumFree();
    static u32 getNumBreak();

    static JASDSPChannel* sDspChannels;

    /* 0x00 */ s32 mStatus;

    /**
     * Priority of this DSP channel. Used to
     */
    /* 0x04 */ s16 mPriority;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u32 mUpdateCounter;
    /* 0x10 */ Callback mCallback;
    /* 0x14 */ void* mCallbackData;
    /* 0x18 */ JASDsp::TChannel* mChannel;
};  // Size: 0x1C

#endif /* JASDSPCHANNEL_H */
