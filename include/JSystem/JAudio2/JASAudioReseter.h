#ifndef JASAUDIORESETER_H
#define JASAUDIORESETER_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASAudioReseter {
    JASAudioReseter();
    ~JASAudioReseter();
    bool start(u32, bool);
    void resume();
    s32 checkDone() const;
    s32 calc();
    static s32 callback(void*);

    /* 0x0 */ u32 field_0x0;
    /* 0x4 */ f32 mDSPLevel;
    /* 0x8 */ s32 mDoneFlag;
    /* 0xC */ bool mThreadStopFlag;
};  // Size: 0x10

#endif /* JASAUDIORESETER_H */
