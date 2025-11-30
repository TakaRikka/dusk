#ifndef JUTVIDEO_H
#define JUTVIDEO_H

#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/os.h>

typedef u8 (*Pattern)[2];

/**
* @ingroup jsystem-jutility
*
*/
class JUTVideo {
public:
    typedef void (*Callback)(u32);

    JUTVideo(GXRenderModeObj const*);
    virtual ~JUTVideo();

    // TODO: return types not confirmed
    static JUTVideo* createManager(GXRenderModeObj const*);
    static void destroyManager();
    static void drawDoneStart();
    static void dummyNoDrawWait();
    void setRenderMode(GXRenderModeObj const*);
    void waitRetraceIfNeed();

    static void preRetraceProc(u32);
    static void postRetraceProc(u32);
    static void drawDoneCallback();

    u16 getFbWidth() const { return mRenderObj->fbWidth; }
    u16 getEfbHeight() const { return mRenderObj->efbHeight; }
    void getBounds(u16& width, u16& height) const {
        width = (u16)getFbWidth();
        height = (u16)getEfbHeight();
    }
    u16 getXfbHeight() const { return mRenderObj->xfbHeight; }
    u32 isAntiAliasing() const { return mRenderObj->aa; }
    Pattern getSamplePattern() const { return mRenderObj->sample_pattern; }
    u8* getVFilter() const { return mRenderObj->vfilter; }
    OSMessageQueue* getMessageQueue() { return &mMessageQueue; }

    static JUTVideo* getManager() { return sManager; }
    static OSTick getVideoInterval() { return sVideoInterval; }
    static OSTick getVideoLastTick() { return sVideoLastTick; }

    GXRenderModeObj* getRenderMode() const { return mRenderObj; }

private:
    static JUTVideo* sManager;
    static OSTick sVideoLastTick;
    static OSTick sVideoInterval;

private:
    /* 0x04 */ _GXRenderModeObj* mRenderObj;
    /* 0x08 */ u32 field_0x8;
    /* 0x0C */ u32 mRetraceCount;
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u32 field_0x14;
    /* 0x18 */ u32 field_0x18;
    /* 0x1C */ VIRetraceCallback mPreRetraceCallback;
    /* 0x20 */ VIRetraceCallback mPostRetraceCallback;
    /* 0x24 */ Callback mPreCallback;
    /* 0x28 */ Callback mPostCallback;
    /* 0x2C */ bool mSetBlack;
    /* 0x30 */ s32 mSetBlackFrameCount;
    /* 0x34 */ OSMessage mMessage;
    /* 0x38 */ OSMessageQueue mMessageQueue;
};

inline JUTVideo* JUTGetVideoManager() {
    return JUTVideo::getManager();
}

#endif /* JUTVIDEO_H */
