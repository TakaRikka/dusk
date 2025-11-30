#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTXfb.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/gx.h"

void JUTXfb::clearIndex() {
    mDrawingXfbIndex = -1;
    mDrawnXfbIndex = -1;
    mDisplayingXfbIndex = -1;
}

void JUTXfb::common_init(int bufNum) {
    mBufferNum = bufNum;
    clearIndex();
    mSDrawingFlag = 99;
}

JUTXfb::JUTXfb(GXRenderModeObj const* pObj, JKRHeap* pHeap, JUTXfb::EXfbNumber xfbNum) {
    common_init(xfbNum);

    if (pObj) {
        initiate(pObj->fbWidth, pObj->xfbHeight, pHeap, xfbNum);
    } else {
        u16 fbWidth = (u32)JUTVideo::getManager()->getRenderMode()->fbWidth;
        u16 xfbHeight = (u32)JUTVideo::getManager()->getRenderMode()->xfbHeight;
        u16 efbHeight = (u32)JUTVideo::getManager()->getRenderMode()->efbHeight;
        f32 scale_factor = GXGetYScaleFactor(efbHeight, xfbHeight);
        u16 xfb_lines = GXGetNumXfbLines(efbHeight, scale_factor);

        initiate(fbWidth, xfb_lines, pHeap, xfbNum);
    }
}

JUTXfb* JUTXfb::sManager;

JUTXfb::~JUTXfb() {
    for (int i = 0; i < 3; i++) {
        delXfb(i);
    }
    sManager = NULL;
}

void JUTXfb::delXfb(int xfbIdx) {
    if (mXfbAllocated[xfbIdx] && mBuffer[xfbIdx]) {
        delete mBuffer[xfbIdx];
    }
}

JUTXfb* JUTXfb::createManager(JKRHeap* pHeap, JUTXfb::EXfbNumber xfbNum) {
    JUT_CONFIRM(273, sManager == NULL);
    if (sManager == NULL) {
        sManager = new JUTXfb(NULL, pHeap, xfbNum);
    }
    return sManager;
}

void JUTXfb::destroyManager() {
    JUT_CONFIRM(344, sManager);
    delete sManager;
    sManager = NULL;
}

void JUTXfb::initiate(u16 width, u16 height, JKRHeap* pHeap, JUTXfb::EXfbNumber xfbNum) {
    if (pHeap == NULL) {
        pHeap = JKRGetSystemHeap();
    }

    int size = (u16)((u16)width + 0xf & ~0xf) * height * 2;

    mBuffer[0] = ::operator new[](size, pHeap, 0x20);
    mXfbAllocated[0] = true;

    if (xfbNum >= 2) {
        mBuffer[1] = ::operator new[](size, pHeap, 0x20);
        mXfbAllocated[1] = true;
    } else {
        mBuffer[1] = NULL;
        mXfbAllocated[1] = false;
    }

    if (xfbNum >= 3) {
        mBuffer[2] = ::operator new[](size, pHeap, 0x20);
        mXfbAllocated[2] = true;
    } else {
        mBuffer[2] = NULL;
        mXfbAllocated[2] = false;
    }
}
