#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASSimpleWaveBank.h"

JASSimpleWaveBank::JASSimpleWaveBank() {
    mWaveTable = NULL;
    mWaveTableSize = 0;
}

JASSimpleWaveBank::~JASSimpleWaveBank() {
    delete[] mWaveTable;
}

void JASSimpleWaveBank::setWaveTableSize(u32 size, JKRHeap* heap) {
    delete[] mWaveTable;
    mWaveTable = new (heap, 0) TWaveHandle[size];
    mWaveTableSize = size;
}

JASWaveHandle* JASSimpleWaveBank::getWaveHandle(u32 no) const {
    if (no >= mWaveTableSize) {
        return NULL;
    }
    return &mWaveTable[no];
}

void JASSimpleWaveBank::setWaveInfo(u32 no, JASWaveInfo const& waveInfo) {
    mWaveTable[no].mWaveInfo = waveInfo;
    mWaveTable[no].mWaveInfo.field_0x20 = &_48;
    mWaveTable[no].mHeap = &mHeap;
}

JASWaveArc* JASSimpleWaveBank::getWaveArc(u32 no) {
    if (no != 0) {
        return NULL;
    }
    return this;
}

int JASSimpleWaveBank::TWaveHandle::getWavePtr() const {
    void* base = mHeap->getBase();
    if (base == NULL) {
        return NULL;
    }
    return (int)base + mWaveInfo.field_0x08;
}

JASSimpleWaveBank::TWaveHandle::TWaveHandle() {
    mHeap = NULL;
}

const JASWaveInfo* JASSimpleWaveBank::TWaveHandle::getWaveInfo() const {
    return &mWaveInfo;
}

u32 JASSimpleWaveBank::getArcCount() const {
    return 1;
}
