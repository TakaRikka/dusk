#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTProcBar.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTVideo.h"
#include <stdint.h>

JUTProcBar::JUTProcBar() {
    mVisible = true;
    mHeapBarVisible = true;
    field_0x108 = 0;
    s32 height = JUTVideo::getManager()->getEfbHeight();
    if (height > 400) {
        mParams.setBarWidth(2);
        mParams.setPosition(39, height - 40);
        mParams.setWidth(562);
        mParams.setUserPosition(height - 70);
    } else {
        mParams.setBarWidth(1);
        mParams.setPosition(39, height - 20);
        mParams.setWidth(562);
        mParams.setUserPosition(height - 35);
    }
    field_0x110 = 1;
    field_0x128 = 0;
    mWatchHeap = NULL;
}

JUTProcBar* JUTProcBar::sManager;

JUTProcBar::~JUTProcBar() {
    sManager = NULL;
}

JUTProcBar* JUTProcBar::create() {
    if (!sManager) {
        sManager = new JUTProcBar();
    }
    return sManager;
}

void JUTProcBar::destroy() {
    if (sManager) {
        delete sManager;
    }
    sManager = NULL;
}

static f32 oneFrameRate = 8.0f;

static f32 oneFrameRateUser = 10.0f;

void JUTProcBar::clear() {
    sManager->idleStart();
    sManager->cpuStart();
    sManager->gpStart();
    sManager->wholeLoopStart();
    sManager->setCostFrame(0);
    oneFrameRate = 8.0f;
    oneFrameRateUser = 10.0f;
}

void JUTProcBar::bar_subroutine(int param_0, int param_1, int param_2, int param_3, int param_4,
                                int param_5, int param_6, JUtility::TColor param_7,
                                JUtility::TColor param_8) {
    int var1 = param_6 * param_3 / param_4;
    J2DFillBox(param_0, param_1, param_5 * param_3 / param_4, param_2, param_7);
    if (var1 < 0) {
        return;
    }
    if (var1 < 6) {
        J2DFillBox(param_0, param_1, var1, param_2, param_8);
    } else {
        J2DFillBox(param_0 + var1 - 6, param_1, 6.0f, param_2, param_8);
    }
}

void JUTProcBar::adjustMeterLength(u32 param_0, f32* param_1, f32 param_2, f32 param_3,
                                   int* param_4) {
    BOOL var2 = false;
    float var1 = *param_1;
    while (var1 > param_2) {
        if (param_0 * var1 * 20.0f / 16666.0f <= mParams.mWidth - 30.0f)
            break;

        var1 -= (1.0f / 10.0f);
        var2 = true;
    }

    if (var1 >= param_3)
        *param_4 = 0;
    if (var1 > param_3 - (1.0f / 5.0f))
        var1 = param_3;

    while (!var2 && var1 < param_3) {
        (*param_4)++;
        if (*param_4 < 0x1e)
            break;
        if ((param_0 * var1 * 20.0f / 16666.0f) < (mParams.mWidth - 60.0f))
            var1 += (1.0f / 5.0f);
        break;
    }
    *param_1 = var1;
}

void JUTProcBar::draw() {
    drawProcessBar();
    drawHeapBar();
}

void JUTProcBar::drawProcessBar() {
    if (mVisible) {
        int frameDuration = 16666;  // duration in miliseconds? for how long a frame takes,
        if (JUTVideo::getManager() && ((JUTVideo::getManager()->getRenderMode()->viTVmode >> 2) &
                                       0x0f) == 1)  // possibly a define
            frameDuration = 20000;                  // duration for PAL

        static int cnt = 0;
        adjustMeterLength(mWholeLoop.mCost, &oneFrameRate, 1.0f, 10.0f, &cnt);
        int r28 = oneFrameRate * 20.0f;
        int r27 = mParams.mBarWidth * 8;
        int r26 = mParams.mBarWidth * 2;
        int r25 = mParams.mBarWidth * 10;
        int r24 = (mParams.mWidth - 4 + r28) / r28;

        mIdle.accumePeek();
        mGp.accumePeek();
        mCpu.accumePeek();

        u32 totalTime = (mGp.mCost - mGpWait.mCost) - mCpu.mCost;  // unsure of types
        u32 gpuTime = (mGp.mCost - mGpWait.mCost);
        J2DFillBox(mParams.mPosX, mParams.mPosY, mParams.mWidth, r27,
                   JUtility::TColor(0, 0, 50, 200));
        J2DDrawFrame(mParams.mPosX, mParams.mPosY, mParams.mWidth, r27,
                     JUtility::TColor(50, 50, 150, 255), 6);
        if (mCostFrame > r24)
            J2DFillBox(mParams.mPosX, mParams.mPosY + r27 + 1, mParams.mWidth, 1.0f,
                       JUtility::TColor(250, 0, 0, 200));
        else
            J2DFillBox(mParams.mPosX, mParams.mPosY + r27 + 1, mCostFrame * r28 + 2, 1.0f,
                       JUtility::TColor(0, 250, 250, 200));

        int stack92 = mWholeLoop.mCost * r28 / frameDuration;
        if (stack92 > mParams.mWidth)
            J2DFillBox(mParams.mPosX, mParams.mPosY, mParams.mWidth, 1.0f,
                       JUtility::TColor(255, 100, 0, 255));
        else
            J2DFillBox(mParams.mPosX, mParams.mPosY, stack92, 1.0f,
                       JUtility::TColor(50, 255, 0, 255));

        if (field_0x110 == 0) {
            int r23 = mParams.mPosY + mParams.mBarWidth;
            bar_subroutine(mParams.mPosX + 1, r23, r26, r28, frameDuration, mGp.mCost,
                           mGp.field_0x8, JUtility::TColor(80, 255, 80, 255),
                           JUtility::TColor(100, 255, 120, 255));
            r23 += mParams.mBarWidth * 2;
            bar_subroutine(mParams.mPosX + 1, r23, r26, r28, frameDuration, mCpu.mCost,
                           mCpu.field_0x8, JUtility::TColor(255, 80, 80, 255),
                           JUtility::TColor(255, 100, 100, 255));
            r23 += mParams.mBarWidth * 2;
            bar_subroutine(mParams.mPosX + 1, r23, r26, r28, frameDuration, mIdle.mCost,
                           mIdle.field_0x8, JUtility::TColor(180, 180, 160, 255),
                           JUtility::TColor(200, 200, 200, 255));
        } else {
            int r22 = mParams.mPosY + mParams.mBarWidth;
            int r21 = mParams.mPosX + 1;
            bar_subroutine(r21, r22, r26, r28, frameDuration, gpuTime, -1,
                           JUtility::TColor(80, 255, 80, 255), JUtility::TColor(80, 255, 80, 255));
            int thingy1 = gpuTime * r28 / frameDuration + r21;  // inline or define?
            J2DFillBox(thingy1, r22, mGpWait.calcBarSize(r28, frameDuration), r26,
                       JUtility::TColor(0, 255, 0, 255));
            int r30 = mGp.calcBarSize(r28, frameDuration) + r21;
            r21 += totalTime * r28 / frameDuration;
            r22 += mParams.mBarWidth * 2;
            bar_subroutine(r21, r22, r26, r28, frameDuration, mCpu.mCost, -1,
                           JUtility::TColor(255, 80, 80, 255), JUtility::TColor(255, 80, 80, 255));
            r22 += mParams.mBarWidth * 2;
            bar_subroutine(r30, r22, r26, r28, frameDuration, mIdle.mCost, -1,
                           JUtility::TColor(180, 180, 160, 255),
                           JUtility::TColor(180, 180, 160, 255));
        }
        for (int i = 1; i < r24; i++) {
            int temp2 = mParams.mPosX + i * r28 + 1;
            J2DDrawLine(temp2, mParams.mPosY + mParams.mBarWidth, temp2,
                        mParams.mPosY + r27 - mParams.mBarWidth,
                        (i % 5) != 0 ? JUtility::TColor(100, 100, 255, 255) :
                                       JUtility::TColor(180, 255, 255, 255),
                        12);
        }
        u32 temp3 = 0;
        for (int i = 0; i < 8; i++) {
            CTime* time = &mUsers[i];
            if (++time->field_0xc >= 0x10 || time->mCost > time->field_0x8) {
                time->field_0x8 = time->mCost;
                time->field_0xc = 0;
            }
            if (time->field_0x8 > temp3)
                temp3 = time->field_0x8;
        }
        if ((bool)temp3 == true) {
            static int cntUser = 0;
            adjustMeterLength(temp3, &oneFrameRateUser, 1.0f, 10.0f, &cntUser);
            int r21 = oneFrameRateUser * 20.0f;
            J2DFillBox(mParams.mPosX, mParams.mUserPosition, mParams.mWidth, r25,
                       JUtility::TColor(0, 0, 50, 200));
            J2DDrawFrame(mParams.mPosX, mParams.mUserPosition, mParams.mWidth, r25,
                         JUtility::TColor(50, 50, 150, 255), 6);
            for (int i = 0; i < 8; i++) {
                CTime* time = &mUsers[i];
                if (++time->field_0xc >= 0x10 || time->mCost > time->field_0x8) {
                    time->field_0x8 = time->mCost;
                    time->field_0xc = 0;
                }
                if (time->mCost != 0 || time->field_0x8 != 0) {
                    int temp4 = time->mCost * r21 / frameDuration;
                    int temp5 = time->field_0x8 * r21 / frameDuration;
                    time->mCost = 0;
                    J2DFillBox(mParams.mPosX + 1,
                               mParams.mUserPosition + mParams.mBarWidth + i * mParams.mBarWidth,
                               temp4, mParams.mBarWidth,
                               JUtility::TColor(time->mR, time->mG, time->mB, 255));

                    if (temp5 < 3u)
                        J2DFillBox(mParams.mPosX,
                                   mParams.mUserPosition + mParams.mBarWidth +
                                       i * mParams.mBarWidth,
                                   temp5, mParams.mBarWidth, JUtility::TColor(255, 200, 50, 255));
                    else
                        J2DFillBox(mParams.mPosX + temp5 - 3,
                                   mParams.mUserPosition + mParams.mBarWidth +
                                       i * mParams.mBarWidth,
                                   3.0f, mParams.mBarWidth, JUtility::TColor(255, 200, 50, 255));
                }
            }

            int r22 = (mParams.mWidth - 4 + r21) / r21;

            for (int i = 1; i < r22; i++) {
                int temp6 = mParams.mPosX + i * r21 + 1;
                J2DDrawLine(temp6, mParams.mUserPosition + mParams.mBarWidth, temp6,
                            mParams.mUserPosition + r25 - mParams.mBarWidth,
                            (i % 5) != 0 ? JUtility::TColor(100, 100, 255, 255) :
                                           JUtility::TColor(180, 255, 255, 255),
                            12);
            }
        }
        field_0x108 = 0;
    }
}

static int addrToXPos(void* param_0, int param_1) {
    return param_1 * (((uintptr_t)param_0 - 0x80000000) / (float)JKRHeap::mMemorySize);
}

static int byteToXLen(int param_0, int param_1) {
    return param_1 * (param_0 / (float)JKRHeap::mMemorySize);
}

static void heapBar(JKRHeap* param_0, int param_1, int param_2, int param_3, int param_4,
                    int param_5) {
    int stack52 = param_1 + addrToXPos(param_0->getStartAddr(), param_4);
    int var1 = param_1 + addrToXPos(param_0->getEndAddr(), param_4);
    int stack36 = byteToXLen(param_0->getTotalFreeSize(), param_4);
    J2DFillBox(stack52, param_2 - param_5 * 2 + param_5 / 2, var1 - stack52, param_5 / 2,
               JUtility::TColor(255, 0, 200, 255));
    J2DFillBox(stack52, param_2 - param_5 * 2 + param_5 / 2, stack36, param_5 / 2,
               JUtility::TColor(255, 180, 250, 255));
}

void JUTProcBar::drawHeapBar() {
    if (mHeapBarVisible) {
        int start;  // required/workaround for regswaps, end might be a shared variable too, however
                    // doesn't seem to be needed?
        int posX = mParams.mPosX;
        int posY = mParams.mPosY;
        int barHeight = mParams.mBarWidth * 2;
        int width = mParams.mWidth;
        int height = mParams.mBarWidth * 2;

        // draw main box in opaque bordeaux red and main frame in purple?
        J2DFillBox(posX, posY - (height * 2), width, height, JUtility::TColor(100, 0, 50, 200));
        J2DDrawFrame(posX, posY - (height * 2), width, height, JUtility::TColor(100, 50, 150, 255),
                     6);

        // Draws a pink line that shows the size of the memstart to start of arenalow?
        start = posX + addrToXPos(JKRHeap::getCodeStart(), width);
        int codeEnd = posX + addrToXPos(JKRHeap::getCodeEnd(), width);
        J2DFillBox(start, posY - (height * 2), codeEnd - start, height,
                   JUtility::TColor(255, 50, 150, 255));

        // draws a dark blue line that shows how much memory is free?
        start = posX + addrToXPos(JKRHeap::getUserRamStart(), width);
        int userEnd = posX + addrToXPos(JKRHeap::getUserRamEnd(), width);
        J2DFillBox(start, posY - (height * 2), userEnd - start, height,
                   JUtility::TColor(0, 50, 150, 255));

        // draws a light blue line that shows how much memory is free in the root heap(blends to
        // light pink, not sure how this works)
        int size = byteToXLen(JKRHeap::getRootHeap()->getTotalFreeSize(), width);
        J2DFillBox(start, posY - (height * 2), size, height / 2,
                   JUtility::TColor(0, 250, 250, 255));
        if (field_0x128 == 0) {
            // draws a line of either the watch heap(if available), otherwise draw the current heap
            JKRHeap* heap = mWatchHeap ? mWatchHeap : JKRGetCurrentHeap();
            if (heap != JKRHeap::getSystemHeap()) {
                heapBar(heap, posX, posY, barHeight, width, height);
            }
        }
    }
}
