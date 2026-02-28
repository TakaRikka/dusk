#include "Z2AudioCS/SpkWave.h"

#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <revolution/os.h>
#include <revolution/types.h>

SpkWave::SpkWave(void) {
    mWaveData = NULL;
}

void SpkWave::setResource(void* res) {
    JUT_ASSERT(30, res);

    JASCriticalSection cs;

    mWaveData = res;
}

s32 SpkWave::getNumOfWaves(void) const {
    if (mWaveData == NULL) {
        return 0;
    }

    return ((s32*)mWaveData)[1];
}

s32 SpkWave::getWaveSize(s32 num) const {
    JUT_ASSERT(57, num >= 0);
    if (!mWaveData) {
        return 0;
    }
    JUT_ASSERT(60, num < getNumOfWaves());

    WaveData* data = getWaveData(num);
    JUT_ASSERT(63, data);
    return data->size;
}

u32 SpkWave::getLoopStartPos(s32 num) const {
    JUT_ASSERT(76, num >= 0);
    if (!mWaveData) {
        return 0;
    }
    JUT_ASSERT(79, num < getNumOfWaves());

    WaveData* data = getWaveData(num);
    JUT_ASSERT(82, data);
    return data->loopStartPos;
}

u32 SpkWave::getLoopEndPos(s32 num) const {
    JUT_ASSERT(95, num >= 0);
    if (!mWaveData) {
        return 0;
    }
    JUT_ASSERT(98, num < getNumOfWaves());

    WaveData* data = getWaveData(num);
    JUT_ASSERT(101, data);
    return data->loopEndPos;
}

s16* SpkWave::getWave(s32 num) const {
    JUT_ASSERT(114, num >= 0);
    if (!mWaveData) {
        return 0;
    }
    JUT_ASSERT(117, num < getNumOfWaves());

    WaveData* data = getWaveData(num);
    JUT_ASSERT(120, data);
    return data->wave;
}

WaveData* SpkWave::getWaveData(s32 num) const {
    JUT_ASSERT(138, num >= 0);
    JUT_ASSERT(139, mWaveData);
    JUT_ASSERT(140, num < getNumOfWaves());

    WaveData* data = (WaveData*)((u32)mWaveData + *(u32*)((u32)mWaveData + num * 4 + 8));
    return data;
}
