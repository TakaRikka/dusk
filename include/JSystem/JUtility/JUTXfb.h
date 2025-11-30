#ifndef JUTXFB_H
#define JUTXFB_H

#include "JSystem/JUtility/JUTVideo.h"

typedef struct _GXRenderModeObj GXRenderModeObj;
class JKRHeap;

/**
* @ingroup jsystem-jutility
*
*/
class JUTXfb {
public:
    enum EXfbNumber {  // TODO: placeholder
        UNK_0 = 0,
        UNK_1 = 1,
        UNK_2 = 2,
        UNK_3 = 3,
    };

    void clearIndex();
    void common_init(int);
    JUTXfb(_GXRenderModeObj const*, JKRHeap*, JUTXfb::EXfbNumber);
    ~JUTXfb();
    void delXfb(int);
    static JUTXfb* createManager(JKRHeap*, JUTXfb::EXfbNumber);
    static void destroyManager();
    void initiate(u16, u16, JKRHeap*, JUTXfb::EXfbNumber);

    s32 getBufferNum() const { return mBufferNum; }
    s16 getDrawnXfbIndex() const { return mDrawnXfbIndex; }
    s16 getDrawingXfbIndex() const { return mDrawingXfbIndex; }
    s16 getDisplayingXfbIndex() const { return mDisplayingXfbIndex; }
    s32 getSDrawingFlag() const { return mSDrawingFlag; }

    void* getDrawnXfb() const {
        if (mDrawnXfbIndex >= 0)
            return mBuffer[mDrawnXfbIndex];
        return NULL;
    }

    void* getDrawingXfb() const {
        if (mDrawingXfbIndex >= 0)
            return mBuffer[mDrawingXfbIndex];
        return NULL;
    }

    void* getDisplayingXfb() const {
        if (mDisplayingXfbIndex >= 0)
            return mBuffer[mDisplayingXfbIndex];
        return NULL;
    }

    void setDisplayingXfbIndex(s16 index) { mDisplayingXfbIndex = index; }
    void setSDrawingFlag(s32 flag) { mSDrawingFlag = flag; }
    void setDrawnXfbIndex(s16 index) { mDrawnXfbIndex = index; }
    void setDrawingXfbIndex(s16 index) { mDrawingXfbIndex = index; }

    static JUTXfb* getManager() { return sManager; }

private:
    static JUTXfb* sManager;

private:
    /* 0x00 */ void* mBuffer[3];
    /* 0x0C */ bool mXfbAllocated[3];
    /* 0x10 */ s32 mBufferNum;
    /* 0x14 */ s16 mDrawingXfbIndex;
    /* 0x16 */ s16 mDrawnXfbIndex;
    /* 0x18 */ s16 mDisplayingXfbIndex;
    /* 0x1C */ s32 mSDrawingFlag;
};

#endif /* JUTXFB_H */
