#ifndef JASSIMPLEWAVEBANK_H
#define JASSIMPLEWAVEBANK_H

#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JASWaveInfo.h"
#include "JSystem/JKernel/JKRHeap.h"

struct JASSimpleWaveBank : JASWaveBank, JASWaveArc {
    struct TWaveHandle : JASWaveHandle {
        intptr_t getWavePtr() const;
        TWaveHandle();
        const JASWaveInfo* getWaveInfo() const;
#if TARGET_PC
        /**
         * @see JASChannel::mAramBaseAddress
         */
        [[nodiscard]] void const* getAramBaseAddress() const override { return nullptr; }
#endif

        /* 0x04 */ JASWaveInfo mWaveInfo;
        /* 0x28 */ JASHeap* mHeap;
    };

    JASSimpleWaveBank(IF_DUSK(u32 bankId));
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
