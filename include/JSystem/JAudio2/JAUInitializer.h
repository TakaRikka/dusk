#ifndef JAUINITIALIZER_H
#define JAUINITIALIZER_H

#include "dolphin/types.h"

class JKRSolidHeap;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAU_JASInitializer {
public:
    JAU_JASInitializer();
    void initJASystem(JKRSolidHeap* heap);

    /* 0x00 */ u32 audioMemory_;
    /* 0x04 */ u32 audioMemSize_;
    /* 0x08 */ int heapSize_;
    /* 0x0C */ int dvdThreadPriority_;
    /* 0x10 */ int audioThreadPriority_;
    /* 0x14 */ int dvdThreadId_;
    /* 0x18 */ int audioThreadId_;
    /* 0x1C */ int field_0x1c;
    /* 0x20 */ int field_0x20;
    /* 0x24 */ int aramBlockSize_;
    /* 0x28 */ int aramChannelNum_;
    /* 0x2C */ f32 dspLevel_;
    /* 0x30 */ char* waveArcDir_;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAU_JAIInitializer {
public:
    JAU_JAIInitializer();
    void initJAInterface();

    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
};

#endif /* JAUINITIALIZER_H */
