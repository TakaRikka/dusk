#ifndef JUTPROCBAR_H
#define JUTPROCBAR_H

#include "JSystem/JUtility/TColor.h"
#include <dolphin/os.h>

class JKRHeap;

/**
* @ingroup jsystem-jutility
* 
*/
class JUTProcBar {
public:
    class CTime {
    public:
        CTime() { clear(); }

        void clear() {
            mCost = 0;
            field_0x8 = 0;
            field_0xc = 0;
        }

        void start(u8 param_0, u8 param_1, u8 param_2) {
            mR = param_0;
            mG = param_1;
            mB = param_2;
            mTick = OSGetTick();
        }

        void end() {
            mCost = ((OSGetTick() - mTick) * 8) / ((*(u32*)0x800000F8 / 4) / 125000);
            if (mCost == 0) {
                mCost = 1;
            }
        }

        void accumePeek() {
			if (++field_0xc >= 0x10 || mCost >= field_0x8) {
				field_0x8 = mCost;
				field_0xc = 0;
			}
		}

        int calcBarSize(int p1, int p2) { return mCost * p1 / p2; }

        /* 0x00 */ u32 mTick;
        /* 0x04 */ u32 mCost;
        /* 0x08 */ u32 field_0x8;
        /* 0x0C */ u32 field_0xc;
        /* 0x10 */ u8 mR;
        /* 0x11 */ u8 mG;
        /* 0x12 */ u8 mB;
    };

    class CParamSet {
    public:
        /* 0x00 */ int mBarWidth;
        /* 0x04 */ int mPosX;
        /* 0x08 */ int mPosY;
        /* 0x0C */ int mWidth;
        /* 0x10 */ int mUserPosition;

        void setBarWidth(int width) { mBarWidth = width; }
        void setPosition(int x, int y) {
            mPosX = x;
            mPosY = y;
        }
        void setWidth(int width) { mWidth = width; }
        void setUserPosition(int userPos) { mUserPosition = userPos; }
    };

    JUTProcBar();
    ~JUTProcBar();
    static JUTProcBar* create();
    static void destroy();
    static void clear();
    void bar_subroutine(int, int, int, int, int, int, int, JUtility::TColor,
                                       JUtility::TColor);
    void adjustMeterLength(u32, f32*, f32, f32, int*);
    void draw();
    void drawProcessBar();
    void drawHeapBar();

    void cpuStart() { mCpu.start(255, 129, 30); }
    void cpuEnd() { mCpu.end(); }
    void gpWaitStart() { mGpWait.start(255, 129, 30); }
    void gpWaitEnd() { mGpWait.end(); }
    void gpStart() { mGp.start(255, 129, 30); }
    void gpEnd() { mGp.end(); }
    void wholeLoopStart() { mWholeLoop.start(255, 129, 30); }
    void wholeLoopEnd() { mWholeLoop.end(); }
    void idleStart() { mIdle.start(255, 129, 30); }
    void idleEnd() { mIdle.end(); }
    void setCostFrame(int frame) { mCostFrame = frame; }
    void setVisible(bool visible) { mVisible = visible; }
    void setVisibleHeapBar(bool visible) { mHeapBarVisible = visible; }
    void setWatchHeap(JKRHeap* pHeap) { mWatchHeap = pHeap; }

    static JUTProcBar* getManager() { return sManager; }

    static JUTProcBar* sManager;

private:
    /* 0x000 */ CTime mIdle;
    /* 0x014 */ CTime mGp;
    /* 0x028 */ CTime mCpu;
    /* 0x03C */ CTime mGpWait;
    /* 0x050 */ CTime mWholeLoop;
    /* 0x064 */ CTime mUsers[8];
    /* 0x104 */ int mCostFrame;
    /* 0x108 */ int field_0x108;
    /* 0x10C */ bool mVisible;
    /* 0x110 */ int field_0x110;
    /* 0x114 */ CParamSet mParams;
    /* 0x128 */ int field_0x128;
    /* 0x12C */ JKRHeap* mWatchHeap;
    /* 0x130 */ bool mHeapBarVisible;
};

#endif /* JUTPROCBAR_H */
