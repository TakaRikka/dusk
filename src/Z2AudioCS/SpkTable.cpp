#include "Z2AudioCS/SpkTable.h"

SpkTable::SpkTable(void) {
    mIsInitialized = false;
    mNumOfSound = 0;
    mEntryOffset = 0;
    mDataOffsets = 0;
}

struct SpkTableHeader {
    s32 resourceCount;
    s32 entryOff;
    s32 dataOffsetsStartOff;
    BOOL isDataOffsetsInitialized;
};

void SpkTable::setResource(void* res) {
    mIsInitialized = false;

    s32* cursor = (s32*)res;

    s32 resourceCount = *cursor++;
    s32 entryOff = *cursor++;
    s32 dataOffsetsStartOff = *cursor++;
    s32* pIsDataOffsetsInitialized = cursor;
    BOOL isDataOffsetsInitialized = *cursor++;

    mNumOfSound = resourceCount;

    s32 entryOffset = (s32)res + entryOff;
    mEntryOffset = entryOffset;
    s32* dataOffsets = (s32*)((s32)res + dataOffsetsStartOff);
    if (!isDataOffsetsInitialized) {
        for (s32 i = 0; i < mNumOfSound; i++) {
            dataOffsets[i] += (s32)res;
        }
    }

    s32* dataOffsetsCopy = dataOffsets;
    mDataOffsets = dataOffsetsCopy;
    *pIsDataOffsetsInitialized = TRUE;

    mIsInitialized = true;
}
