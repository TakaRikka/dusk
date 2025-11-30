#ifndef JASWSPARSER_H
#define JASWSPARSER_H

#include "JSystem/JSupport/JSupport.h"

class JKRHeap;
class JASWaveBank;
class JASBasicWaveBank;
class JASSimpleWaveBank;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASWSParser {
public:
    template<class T>
    class TOffset {
    public:
        T* ptr(void const* param_0) {
            return JSUConvertOffsetToPtr<T>(param_0, mOffset);
        }
    
    private:
        /* 0x0 */ u32 mOffset;
    };

    struct TCtrlWave {
        /* 0x0 */ u32 _00;
    };

    struct TWave {
        /* 0x00 */ u8 _00;
        /* 0x01 */ u8 _01;
        /* 0x02 */ u8 _02;
        /* 0x04 */ f32 _04;
        /* 0x08 */ u32 mOffset;
        /* 0x0C */ u32 _0C;
        /* 0x10 */ u32 _10;
        /* 0x14 */ u32 _14;
        /* 0x18 */ u32 _18;
        /* 0x1C */ u32 _1C;
        /* 0x20 */ s16 _20;
        /* 0x22 */ s16 _22;
    };

    struct TWaveArchive {
        /* 0x00 */ char mFileName[0x74];  // unknown length
        /* 0x74 */ TOffset<TWave> mWaveOffsets[0];
    };

    struct TWaveArchiveBank {
        /* 0x0 */ u8 _00[8];
        /* 0x8 */ TOffset<TWaveArchive> mArchiveOffsets[0];
    };

    struct TCtrl {
        /* 0x0 */ u8 _00[4];
        /* 0x4 */ u32 mWaveCount;
        /* 0x8 */ TOffset<TCtrlWave> mCtrlWaveOffsets[0];
    };

    struct TCtrlScene {
        /* 0x0 */ u8 _00[0xC];
        /* 0xC */ TOffset<TCtrl> mCtrlOffset;
    };

    struct TCtrlGroup {
        /* 0x0 */ u8 _00[8];
        /* 0x8 */ u32 mGroupCount;
        /* 0xC */ TOffset<TCtrlScene> mCtrlSceneOffsets[0];
    };

    /** @fabricated */
    struct THeader {
        /* 0x00 */ u8 _00[0xC];
        /* 0x0C */ u32 mWaveTableSize;
        /* 0x10 */ TOffset<TWaveArchiveBank> mArchiveBankOffset;
        /* 0x14 */ TOffset<TCtrlGroup> mCtrlGroupOffset;
    };

    static u32 getGroupCount(void const*);
    static JASWaveBank* createWaveBank(void const*, JKRHeap*);
    static JASBasicWaveBank* createBasicWaveBank(void const*, JKRHeap*);
    static JASSimpleWaveBank* createSimpleWaveBank(void const*, JKRHeap*);

    static u32 sUsedHeapSize;
};

#endif /* JASWSPARSER_H */
