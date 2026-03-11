#ifndef JASWAVEINFO_H
#define JASWAVEINFO_H

#include <types.h>

class JASWaveArc;

#define WAVE_FORMAT_ADPCM4 0
#define WAVE_FORMAT_ADPCM2 1
#define WAVE_FORMAT_PCM8   2
#define WAVE_FORMAT_PCM16  3

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASWaveInfo {
    JASWaveInfo() {
        mBaseKey = 0x3c;
        field_0x20 = &one;
    }

    /* 0x00 */ u8 mWaveFormat;
    /* 0x01 */ u8 mBaseKey;
    /* 0x02 */ u8 mLoopFlag;
    /* 0x04 */ f32 mSampleRate;
    /* 0x08 */ int mOffsetStart;
    /* 0x0C */ int mOffsetLength;
    /* 0x10 */ u32 mLoopStartSample;
    /* 0x14 */ int mLoopEndSample;
    /* 0x18 */ int mSampleCount;
    /* 0x1C */ s16 mpLast;
    /* 0x1E */ s16 mpPenult;
    /* 0x20 */ const u32* field_0x20;

    static u32 one;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASWaveHandle {
public:
    virtual ~JASWaveHandle() {}
    virtual const JASWaveInfo* getWaveInfo() const = 0;
    virtual intptr_t getWavePtr() const = 0;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASWaveBank {
public:
    virtual ~JASWaveBank() {}
    virtual JASWaveHandle* getWaveHandle(u32) const = 0;
    virtual JASWaveArc* getWaveArc(u32) = 0;
    virtual u32 getArcCount() const = 0;
};

#endif /* JASWAVEINFO_H */
