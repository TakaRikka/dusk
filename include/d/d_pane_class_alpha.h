#ifndef D_PANE_D_PANE_CLASS_ALPHA_H
#define D_PANE_D_PANE_CLASS_ALPHA_H

#include "JSystem/J2DGraph/J2DPane.h"

class J2DPane;
class J2DScreen;
class JKRExpHeap;

class CPaneMgrAlpha {
public:
    virtual ~CPaneMgrAlpha();
    virtual void setAlpha(u8);

    CPaneMgrAlpha();
    CPaneMgrAlpha(J2DScreen*, u64, u8, JKRExpHeap*);
    void initiateAlpha(J2DPane*, JKRExpHeap*);
    void show();
    void hide();
    bool isVisible();
    f32 rateCalc(s16, s16, u8);
    void setAlphaRate(f32);
    f32 getAlphaRate();
    bool alphaAnime(s16 timer, u8 startAlpha, u8 endAlpha, u8 calcType);
    bool alphaAnimeLoop(s16, u8, u8, u8);
    void childPaneCount(J2DPane*);
    void childPaneGetAlpha(J2DPane*);
    void childPaneSetAlpha(J2DPane*, u8);

    J2DPane* getPanePtr() { return mPane; }
    u8 getAlpha() { return getPanePtr()->getAlpha(); }
    s16 getAlphaTimer() { return mAlphaTimer; }
    void alphaAnimeStart(s16 start) { mAlphaTimer = start; }
    u8 getInitAlpha() { return mInitAlpha; }

    /* 0x04 */ J2DPane* mPane;
    /* 0x08 */ JKRExpHeap* heap;
    /* 0x0C */ void* mpFirstStackAlpha;
    /* 0x10 */ u8* field_0x10;
    /* 0x14 */ s16 mChildPaneCount;
    /* 0x16 */ s16 mAlphaTimer;
    /* 0x18 */ u8 mInitAlpha;
    /* 0x19 */ u8 mFlags;
};

class CPaneMgrAlphaMorf : public CPaneMgrAlpha {
public:
    CPaneMgrAlphaMorf(J2DScreen*, u64, u8, JKRExpHeap*);
    void initiateAlphaMorf();
    void setBackupAlpha();
    void setAlphaMorfRate(f32);
    void childPaneBackupAlpha(J2DPane*);
    void childPaneSetAlphaMorf(J2DPane*, f32);

    virtual ~CPaneMgrAlphaMorf();

private:
    /* 0x1C */ void* mpFirstSaveAlpha;
    /* 0x20 */ u8* field_0x20;
    /* 0x24 */ void* mpFirstGetAlpha;
    /* 0x28 */ u8* field_0x28;
};

#endif /* D_PANE_D_PANE_CLASS_ALPHA_H */
