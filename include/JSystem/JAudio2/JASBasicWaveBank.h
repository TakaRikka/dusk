#ifndef JASBASICWAVEBANK_H
#define JASBASICWAVEBANK_H

#include "JSystem/JAudio2/JASWaveArcLoader.h"
#include "JSystem/JAudio2/JASWaveInfo.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASBasicWaveBank : public JASWaveBank {
    struct TWaveHandle : public JASWaveHandle {
        TWaveHandle() { mHeap = NULL; }
        virtual int getWavePtr() const;
        virtual const JASWaveInfo* getWaveInfo() const { return &field_0x4; }
        bool compareHeap(JASHeap* heap) const { return mHeap == heap;}

        /* 0x04 */ JASWaveInfo field_0x4;
        /* 0x28 */ JASHeap* mHeap;
    };

    struct TGroupWaveInfo {
        TGroupWaveInfo() {
            field_0x0 = 0xffff;
            field_0x4 = -1;
        }

        /* 0x00 */ u16 field_0x0;
        /* 0x04 */ int field_0x4;
    };

    struct TWaveGroup : JASWaveArc {
        TWaveGroup();
        virtual ~TWaveGroup();
        void setWaveCount(u32, JKRHeap*);
        virtual void onLoadDone();
        virtual void onEraseDone();
        u32 getWaveID(int) const;

        /* 0x74 */ JASBasicWaveBank* mBank;
        /* 0x78 */ TGroupWaveInfo* mCtrlWaveArray;
        /* 0x7C */ u16 mWaveCount;

        u32 getWaveCount() const { return mWaveCount; }
    };

    JASBasicWaveBank();
    ~JASBasicWaveBank();
    TWaveGroup* getWaveGroup(u32);
    void setGroupCount(u32, JKRHeap*);
    void setWaveTableSize(u32, JKRHeap*);
    void incWaveTable(JASBasicWaveBank::TWaveGroup const*);
    void decWaveTable(JASBasicWaveBank::TWaveGroup const*);
    JASWaveHandle* getWaveHandle(u32) const;
    void setWaveInfo(JASBasicWaveBank::TWaveGroup*, int, u16, JASWaveInfo const&);
    JASWaveArc* getWaveArc(u32 param_0) { return getWaveGroup(param_0); }
    u32 getArcCount() const { return mGroupCount; }

    /* 0x04 */ OSMutex field_0x4;
    /* 0x1C */ TWaveHandle* mWaveTable;
    /* 0x20 */ TWaveGroup* mWaveGroupArray;
    /* 0x24 */ u16 mHandleCount;
    /* 0x26 */ u16 mGroupCount;

    static u32 mNoLoad;
};

#endif /* JASBASICWAVEBANK_H */
