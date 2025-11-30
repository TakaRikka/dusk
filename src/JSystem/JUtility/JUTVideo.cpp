#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "JSystem/JUtility/JUTXfb.h"
#include "dolphin/gx.h"
#include <dolphin/vi.h>

JUTVideo* JUTVideo::sManager;

OSTick JUTVideo::sVideoLastTick;

OSTick JUTVideo::sVideoInterval;

static bool data_80451544;

JUTVideo* JUTVideo::createManager(_GXRenderModeObj const* param_0) {
    if (sManager == NULL) {
        sManager = new JUTVideo(param_0);
    }
    return sManager;
}

void JUTVideo::destroyManager() {
    if (sManager != NULL) {
        delete sManager;
        sManager = NULL;
    }
}

JUTVideo::JUTVideo(GXRenderModeObj const* param_0) {
    mRenderObj = NULL;
    VIInit();
    mSetBlack = true;
    mSetBlackFrameCount = 2;
    setRenderMode(param_0);
    VISetBlack(1);
    VIFlush();
    field_0x8 = 0;
    mRetraceCount = VIGetRetraceCount();
    field_0x10 = 1;
    field_0x18 = 0;
    sVideoLastTick = OSGetTick();
    sVideoInterval = 670000;
    mPreRetraceCallback = VISetPreRetraceCallback(preRetraceProc);
    mPostRetraceCallback = VISetPostRetraceCallback(postRetraceProc);
    mPreCallback = NULL;
    mPostCallback = NULL;
    OSInitMessageQueue(&mMessageQueue, &mMessage, 1);
    GXSetDrawDoneCallback(drawDoneCallback);
}

JUTVideo::~JUTVideo() {
    VISetPreRetraceCallback(mPreRetraceCallback);
    VISetPostRetraceCallback(mPostRetraceCallback);
}

void JUTVideo::preRetraceProc(u32 retrace_count) {
    if (!sManager) {
        return;
    }

    if (sManager->mPreCallback) {
        (*sManager->mPreCallback)(retrace_count);
    }

    OSTick tick = OSGetTick();
    sVideoInterval = tick - sVideoLastTick;
    sVideoLastTick = tick;

    JUTXfb* xfb = JUTXfb::getManager();
    if (!xfb) {
        VISetBlack(TRUE);
        VIFlush();
        return;
    }

    static void* frameBuffer = NULL;

    if (frameBuffer) {
        JUTVideo* videoManager = JUTGetVideoManager();
        const GXRenderModeObj* renderMode = videoManager->getRenderMode();
        JUTDirectPrint* directPrint = JUTDirectPrint::getManager();
        directPrint->changeFrameBuffer(frameBuffer, renderMode->fbWidth, renderMode->efbHeight);
    }

    if (sManager->mSetBlack == 1) {
        s32 frame_count = sManager->mSetBlackFrameCount;
        if (frame_count > 0) {
            frame_count--;
        }

        sManager->mSetBlackFrameCount = frame_count;
        sManager->mSetBlack = frame_count != 0;
        VISetBlack(TRUE);
        VIFlush();
        return;
    }

    if (!xfb) {
        VISetBlack(TRUE);
        VIFlush();
        return;
    }

    if (xfb->getBufferNum() == 3 || xfb->getBufferNum() == 2) {
        if (!data_80451544) {
            s16 index = xfb->getDrawnXfbIndex();
            xfb->setDisplayingXfbIndex(index);
            if (index < 0) {
                VISetBlack(1);
                VIFlush();
            } else {
                VISetNextFrameBuffer(xfb->getDisplayingXfb());
                VIFlush();
                VISetBlack(FALSE);
                frameBuffer = xfb->getDisplayingXfb();
            }
        }
    } else if (xfb->getBufferNum() == 1) {
        if (xfb->getSDrawingFlag() == 0) {
            s16 index = xfb->getDrawnXfbIndex();
            if (index >= 0) {
                xfb->setDisplayingXfbIndex(index);
                GXCopyDisp(xfb->getDisplayingXfb(), GX_TRUE);
                GXFlush();
                xfb->setSDrawingFlag(2);
                frameBuffer = xfb->getDisplayingXfb();
                if (VIGetNextFrameBuffer()) {
                    VISetBlack(FALSE);
                }
            } else {
                VISetBlack(TRUE);
            }
        }
        VIFlush();
    }
}

void JUTVideo::drawDoneStart() {
    data_80451544 = true;
    GXSetDrawDone();
}

void JUTVideo::dummyNoDrawWait() {
    data_80451544 = false;
}

void JUTVideo::drawDoneCallback() {
    JUTXfb* xfb = JUTXfb::getManager();
    if (!xfb) {
        return;
    }

    data_80451544 = false;
    if (xfb->getBufferNum() == 1) {
        if (xfb->getSDrawingFlag() == 1) {
            xfb->setSDrawingFlag(0);

            void* frameBuffer = xfb->getDrawnXfb();
            if (frameBuffer) {
                VISetNextFrameBuffer(xfb->getDrawnXfb());
                VIFlush();
            }
        }
    }
}

void JUTVideo::postRetraceProc(u32 retrace_count) {
    if (!sManager) {
        return;
    }

    if (sManager->mPostCallback != NULL) {
        sManager->mPostCallback(retrace_count);
    }

    OSMessage message = (OSMessage*)VIGetRetraceCount();
    OSSendMessage(&sManager->mMessageQueue, message, OS_MESSAGE_NOBLOCK);
}

void JUTVideo::setRenderMode(GXRenderModeObj const* pObj) {
    if (mRenderObj != NULL && pObj->viTVmode != mRenderObj->viTVmode) {
        mSetBlack = true;
        mSetBlackFrameCount = 4;
    }
    mRenderObj = (GXRenderModeObj*)pObj;
    VIConfigure(mRenderObj);
    VIFlush();

    if (mSetBlack) {
        VIWaitForRetrace();
        VIWaitForRetrace();
    }
}

void JUTVideo::waitRetraceIfNeed() {}
