#ifndef JFWDISPLAY_H
#define JFWDISPLAY_H

#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "JSystem/JUtility/JUTFader.h"
#include "JSystem/JUtility/JUTXfb.h"
#include <dolphin/os.h>

typedef struct _GXColor GXColor;
typedef struct _GXRenderModeObj GXRenderModeObj;
class JKRHeap;

typedef void (*JFWDisplayUnkFunc)(void);

/**
 * @ingroup jsystem-jframework
 * 
 */
class JFWAlarm : public OSAlarm {
public:
    JFWAlarm() : mLink(this) {}
    ~JFWAlarm() {}
    void createAlarm() { OSCreateAlarm(this); }
    void cancelAlarm() { OSCancelAlarm(this); }
    void removeLink() { sList.remove(&mLink); }
    void appendLink() { sList.append(&mLink); }
    OSThread* getThread() const { return mThread; }
    void setThread(OSThread* thread) { mThread = thread; }

    static JSUList<JFWAlarm> sList;

public:
    /* 0x28 */ OSThread* mThread;
    /* 0x2C */ JSULink<JFWAlarm> mLink;
};

/**
 * @ingroup jsystem-jframework
 * 
 */
class JFWDisplay {
public:
    enum EDrawDone {
        /* 0x0 */ UNK_METHOD_0 = 0,
        /* 0x1 */ UNK_METHOD_1 = 1
    };

    void ctor_subroutine(bool);
    JFWDisplay(JKRHeap*, JUTXfb::EXfbNumber, bool);
    static JFWDisplay* createManager(_GXRenderModeObj const*, JKRHeap*,
                                                    JUTXfb::EXfbNumber, bool);
    void prepareCopyDisp();
    void drawendXfb_single();
    void exchangeXfb_double();
    void exchangeXfb_triple();
    void copyXfb_triple();
    void preGX();
    void endGX();
    void waitBlanking(int);
    void threadSleep(s64);
    void clearEfb_init();
    void clearEfb(int, int, int, int, _GXColor);
    void clearEfb();
    void clearEfb(_GXColor);
    void calcCombinationRatio();

    virtual void beginRender();
    virtual void endRender();
    virtual void endFrame();
    virtual ~JFWDisplay();

    static JFWDisplay* getManager() { return sManager; }

    static JFWDisplay* createManager(JKRHeap* i_heap, JUTXfb::EXfbNumber i_xfbNum, bool param_2) {
        return createManager(NULL, i_heap, i_xfbNum, param_2);
    }

    bool startFadeOut(int param_0) {
        if (mFader != NULL) {
            return mFader->startFadeOut(param_0);
        }
        return 1;
    }

    int startFadeIn(int param_0) {
        if (mFader != NULL) {
            return mFader->startFadeIn(param_0);
        }
        return 1;
    }

    void setTickRate(u32 rate) {
        mTickRate = rate;
        mFrameRate = 0;
    }

    void setFrameRate(u16 rate) {
        mFrameRate = rate;
        mTickRate = 0;
    }

    void setDrawDoneMethod(EDrawDone drawDone) { mDrawDoneMethod = drawDone; }
    void setFader(JUTFader* fader) { mFader = fader; }
    void resetFader() { setFader(NULL); }
    JUTFader* getFader() const { return mFader; }
    void setClearColor(JUtility::TColor color) { mClearColor = color; }
    int getEfbWidth() const { return JUTVideo::getManager()->getFbWidth(); }
    int getEfbHeight() const { return JUTVideo::getManager()->getEfbHeight(); }
    JUTXfb* getXfbManager() const { return mXfbManager; }

    static JFWDisplay* sManager;

private:
    /* 0x04 */ JUTFader* mFader;
    /* 0x08 */ JUtility::TColor mClearColor;
    /* 0x0C */ u32 mZClear;
    /* 0x10 */ JUTXfb* mXfbManager;
    /* 0x14 */ u16 mGamma;
    /* 0x18 */ EDrawDone mDrawDoneMethod;
    /* 0x1C */ u16 mFrameRate;
    /* 0x20 */ u32 mTickRate;
    /* 0x24 */ bool mEnableAlpha;
    /* 0x26 */ u16 mClamp;
    /* 0x28 */ f32 mCombinationRatio;
    /* 0x2C */ u32 field_0x2c;
    /* 0x30 */ u32 field_0x30;
    /* 0x34 */ u32 field_0x34;
    /* 0x38 */ int field_0x38;
    /* 0x3C */ int field_0x3c;
    /* 0x40 */ bool field_0x40;
    /* 0x44 */ JFWDisplayUnkFunc field_0x44;
    /* 0x48 */ s16 field_0x48;
    /* 0x4A */ u8 field_0x4a;
};

inline void JUTChangeFrameBuffer(void* buffer, u16 height, u16 width) {
    JUTDirectPrint::getManager()->changeFrameBuffer(buffer, width, height);
}

static void JFWDrawDoneAlarm();
static void JFWThreadAlarmHandler(OSAlarm*, OSContext*);
static void JFWGXAbortAlarmHandler(OSAlarm*, OSContext*);
static void waitForTick(u32, u16);
static void diagnoseGpHang();

#endif /* JFWDISPLAY_H */
