#ifndef Z2AUDIOCS_SPKWAVE_H
#define Z2AUDIOCS_SPKWAVE_H

#include <revolution/types.h>

struct WaveData {
    s32 size;
    u32 loopStartPos;
    u32 loopEndPos;
    s16 wave[0];
};

class SpkWave {
public:
    SpkWave(void);
    void setResource(void*);
    s32 getNumOfWaves(void) const;
    s32 getWaveSize(s32 num) const;
    u32 getLoopStartPos(s32 num) const;
    u32 getLoopEndPos(s32 num) const;
    s16* getWave(s32 num) const;
    WaveData* getWaveData(s32 num) const;

    inline bool isValid(void) const { return mWaveData != NULL; }
    inline const void* getResource(void) const { return mWaveData; }
    inline void* getResource(void) { return mWaveData; }

private:
    /* 0x00 */ void* mWaveData;
};

#endif /* Z2AUDIOCS_SPKWAVE_H */
