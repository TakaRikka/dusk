#ifndef JASSIMPLEWAVEBANK_H
#define JASSIMPLEWAVEBANK_H

#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JASWaveInfo.h"
#include "JSystem/JKernel/JKRHeap.h"

struct JASSimpleWaveBank : JASWaveBank, JASWaveArc {
    struct TWaveHandle : JASWaveHandle {
        int getWavePtr() const;
        TWaveHandle();
        const JASWaveInfo* getWaveInfo() const;

        /* 0x04 */ JASWaveInfo mWaveInfo;
        /* 0x28 */ JASHeap* mHeap;
    };

    JASSimpleWaveBank();
    ~JASSimpleWaveBank();
    void setWaveTableSize(u32, JKRHeap*);
    JASWaveHandle* getWaveHandle(u32) const;
    void setWaveInfo(u32, JASWaveInfo const&);
    JASWaveArc* getWaveArc(u32);
    u32 getArcCount() const;

    /* 0x78 */ TWaveHandle* mWaveTable;
    /* 0x7C */ u32 mWaveTableSize;
};

#endif /* JASSIMPLEWAVEBANK_H */
