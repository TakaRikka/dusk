#ifndef JASWSPARSER_H
#define JASWSPARSER_H

#include "JSystem/JSupport/JSupport.h"

class JKRHeap;
class JASWaveBank;
struct JASBasicWaveBank;
struct JASSimpleWaveBank;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASWSParser {
public:
    template<class T>
    class TOffset {
    public:
        T* ptr(void const* param_0) const {
            return JSUConvertOffsetToPtr<T>(param_0, mOffset);
        }
    
    private:
        /* 0x0 */ BE(u32) mOffset;
    };

    struct TCtrlWave {
        /* 0x0 */ BE(u32) _00;
    };

    struct TWave {
        /* 0x00 */ u8 _00;
        /* 0x01 */ u8 mWaveFormat;
        /* 0x02 */ u8 mBaseKey;
        /* 0x04 */ BE(f32) mSampleRate;
        /* 0x08 */ BE(u32) mAWOffsetStart;
        /* 0x0C */ BE(u32) mAWOffsetEnd;
        /* 0x10 */ BE(u32) mLoopFlags;
        /* 0x14 */ BE(u32) mLoopStartSample;
        /* 0x18 */ BE(u32) mLoopEndSample;
        /* 0x1C */ BE(u32) mSampleCount;
        /* 0x20 */ BE(s16) mpLast;
        /* 0x22 */ BE(s16) mpPenult;
    };

    struct TWaveArchive {
        /* 0x00 */ char mFileName[0x70];
        /* 0x70 */ BE(u32) mWaveCount;
        /* 0x74 */ TOffset<TWave> mWaveOffsets[0];
    };

    struct TWaveArchiveBank {
        /* 0x0 */ BE(u32) mMagic; // 'WINF'
        /* 0x0 */ BE(u32) mArchiveCounts;
        /* 0x8 */ TOffset<TWaveArchive> mArchiveOffsets[0];
    };

    struct TCtrl {
        /* 0x0 */ u8 _00[4];
        /* 0x4 */ BE(u32) mWaveCount;
        /* 0x8 */ TOffset<TCtrlWave> mCtrlWaveOffsets[0];
    };

    struct TCtrlScene {
        /* 0x0 */ u8 _00[0xC];
        /* 0xC */ TOffset<TCtrl> mCtrlOffset;
    };

    struct TCtrlGroup {
        /* 0x0 */ BE(u32) mMagic; // 'WBCT'
        /* 0x4 */ u32 mUnknown;
        /* 0x8 */ BE(u32) mGroupCount;
        /* 0xC */ TOffset<TCtrlScene> mCtrlSceneOffsets[0];
    };

    /** @fabricated */
    struct THeader {
        /* 0x00 */ BE(u32) mMagic; // 'WSYS'
        /* 0x04 */ BE(u32) mSize;
        /* 0x08 */ BE(u32) mId;
        /* 0x0C */ BE(u32) mWaveTableSize;
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
