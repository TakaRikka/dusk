#ifndef JASWAVEARCLOADER_H
#define JASWAVEARCLOADER_H

#include "JSystem/JAudio2/JASHeapCtrl.h"
#include <dolphin/os.h>

class JKRHeap;
class JKRSolidHeap;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASDisposer {
    JASDisposer() {}
    virtual ~JASDisposer() {}
    virtual void onDispose() {}
};

#define DIR_MAX 64

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASWaveArcLoader {
    static JASHeap* getRootHeap();
    static void setCurrentDir(char const*);
    static char* getCurrentDir();

    static char sCurrentDir[DIR_MAX];
    static JASHeap* sAramHeap;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASWaveArc : JASDisposer {
    JASWaveArc();
    bool loadSetup(u32);
    bool eraseSetup();
    static void loadToAramCallback(void*);
    bool sendLoadCmd();
    bool load(JASHeap*);
    bool loadTail(JASHeap*);
    bool erase();
    void setEntryNum(s32);
    void setFileName(char const*);

    virtual ~JASWaveArc();
    virtual void onDispose();
    virtual void onLoadDone() {}
    virtual void onEraseDone() {}

    s32 getStatus() const { return mStatus; }

    struct loadToAramCallbackParams {
        // not official struct name
        /* 0x0 */ JASWaveArc* mWavArc;
        /* 0x4 */ long mEntryNum;
        /* 0x8 */ u32 mBase;
        /* 0xC */ u32 _c;
    };

    /* 0x04 */ mutable JASHeap mHeap;
    /* 0x48 */ u32 _48;
    /* 0x4C */ volatile s32 mStatus;
    /* 0x50 */ int mEntryNum;
    /* 0x54 */ u32 mFileLength;
    /* 0x58 */ u16 _58;
    /* 0x5A */ u16 _5a;
    /* 0x5C */ OSMutex mMutex;
};

#endif /* JASWAVEARCLOADER_H */
