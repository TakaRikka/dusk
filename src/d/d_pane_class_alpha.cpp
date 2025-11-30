#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_pane_class_alpha.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "m_Do/m_Do_ext.h"

CPaneMgrAlpha::CPaneMgrAlpha() {}

CPaneMgrAlpha::CPaneMgrAlpha(J2DScreen* p_screen, u64 tag, u8 flags, JKRExpHeap* p_heap) {
    J2DPane* pane = p_screen->search(tag);
    mFlags = flags;
    initiateAlpha(pane, p_heap);
}

CPaneMgrAlpha::~CPaneMgrAlpha() {
    if (mpFirstStackAlpha != NULL) {
        heap->free(mpFirstStackAlpha);
        mpFirstStackAlpha = NULL;
    }
}

void CPaneMgrAlpha::initiateAlpha(J2DPane* p_pane, JKRExpHeap* p_heap) {
    mPane = p_pane;

    if (p_heap != NULL) {
        heap = p_heap;
    } else {
        heap = (JKRExpHeap*)mDoExt_getCurrentHeap();
    }

    mpFirstStackAlpha = NULL;
    field_0x10 = NULL;
    mChildPaneCount = 0;

    if (mFlags != 0) {
        childPaneCount(mPane->getFirstChildPane());

        if (mFlags & 2) {
            mpFirstStackAlpha = heap->alloc(mChildPaneCount, 0x20);
            field_0x10 = (u8*)mpFirstStackAlpha;
            childPaneGetAlpha(mPane->getFirstChildPane());
        }
    }

    if (p_pane->getKind() == 'PAN2') {
        mInitAlpha = 255;
    } else {
        mInitAlpha = p_pane->getAlpha();
    }

    mAlphaTimer = 0;
}

void CPaneMgrAlpha::show() {
    if (!isVisible()) {
        mPane->show();
    }
}

void CPaneMgrAlpha::hide() {
    if (isVisible() == true) {
        mPane->hide();
    }
}

bool CPaneMgrAlpha::isVisible() {
    return mPane->isVisible();
}

f32 CPaneMgrAlpha::rateCalc(s16 maxTimer, s16 curTimer, u8 calcType) {
    if (maxTimer <= curTimer) {
        return 1.0f;
    }

    switch (calcType) {
    case 1:
        return (f32)(curTimer * curTimer) / (f32)(maxTimer * maxTimer);
    case 2:
        return (f32)((maxTimer - curTimer) * (maxTimer - curTimer)) / (f32)(maxTimer * maxTimer);
    default:
        return (f32)curTimer / (f32)maxTimer;
    }
}

void CPaneMgrAlpha::setAlpha(u8 alpha) {
    if (mFlags & 2) {
        field_0x10 = (u8*)mpFirstStackAlpha;
        childPaneSetAlpha(mPane->getFirstChildPane(), alpha);
    }
    mPane->setAlpha(alpha);
}

void CPaneMgrAlpha::setAlphaRate(f32 rate) {
    setAlpha(mInitAlpha * rate);
}

f32 CPaneMgrAlpha::getAlphaRate() {
    f32 rate = 0.0f;

    if (mInitAlpha != 0) {
        rate = getAlpha() / (f32)mInitAlpha;
    }

    return rate;
}

bool CPaneMgrAlpha::alphaAnime(s16 timer, u8 startAlpha, u8 endAlpha, u8 calcType) {
    if (mAlphaTimer < timer - 1) {
        mAlphaTimer++;
        f32 rate = rateCalc(timer, mAlphaTimer, calcType);
        setAlpha(startAlpha + rate * (f32)(endAlpha - startAlpha));
    } else {
        mAlphaTimer = timer;
        setAlpha(endAlpha);
        return true;
    }

    return false;
}

bool CPaneMgrAlpha::alphaAnimeLoop(s16 param_0, u8 param_1, u8 param_2, u8 param_3) {
    bool ret = false;
    s16 temp_r4 = param_0 / 2;

    mAlphaTimer++;

    if (mAlphaTimer >= param_0) {
        mAlphaTimer = 0;
        ret = true;
    }

    s16 tmp;
    if (mAlphaTimer < temp_r4) {
        tmp = mAlphaTimer;
    } else {
        tmp = param_0 - mAlphaTimer;
    }

    f32 rate = rateCalc(temp_r4, tmp, param_3);
    setAlpha(param_1 + rate * (f32)(param_2 - param_1));

    return ret;
}

void CPaneMgrAlpha::childPaneCount(J2DPane* p_pane) {
    if (p_pane != NULL) {
        mChildPaneCount++;
        childPaneCount(p_pane->getFirstChildPane());
        childPaneCount(p_pane->getNextChildPane());
    }
}

void CPaneMgrAlpha::childPaneGetAlpha(J2DPane* p_pane) {
    if (p_pane != NULL) {
        u8 alpha;
        if (p_pane->getKind() == 'PAN2') {
            alpha = 255;
        } else {
            alpha = p_pane->getAlpha();
        }

        *field_0x10 = alpha;
        field_0x10++;
        childPaneGetAlpha(p_pane->getFirstChildPane());
        childPaneGetAlpha(p_pane->getNextChildPane());
    }
}

void CPaneMgrAlpha::childPaneSetAlpha(J2DPane* p_pane, u8 alpha) {
    if (p_pane != NULL) {
        if (!p_pane->mIsInfluencedAlpha) {
            f32 tmp;
            if (mInitAlpha == 0) {
                tmp = 0.0f;
            } else {
                tmp = (f32)alpha / (f32)mInitAlpha;
            }

            p_pane->setAlpha(*field_0x10 * tmp);
        }
        
        field_0x10++;
        childPaneSetAlpha(p_pane->getFirstChildPane(), alpha);
        childPaneSetAlpha(p_pane->getNextChildPane(), alpha);
    }
}

CPaneMgrAlphaMorf::CPaneMgrAlphaMorf(J2DScreen* p_screen, u64 tag, u8 flags,
                                         JKRExpHeap* p_heap)
    : CPaneMgrAlpha(p_screen, tag, flags, p_heap) {
    initiateAlphaMorf();
}

CPaneMgrAlphaMorf::~CPaneMgrAlphaMorf() {
    if (mpFirstSaveAlpha != NULL) {
        heap->free(mpFirstSaveAlpha);
        mpFirstSaveAlpha = NULL;
    }

    if (mpFirstGetAlpha != NULL) {
        heap->free(mpFirstGetAlpha);
        mpFirstGetAlpha = NULL;
    }
}

void CPaneMgrAlphaMorf::initiateAlphaMorf() {
    mpFirstSaveAlpha = NULL;
    field_0x20 = NULL;

    mpFirstGetAlpha = NULL;
    field_0x28 = NULL;

    if (mpFirstStackAlpha != NULL) {
        mpFirstSaveAlpha = heap->alloc(mChildPaneCount, 0x20);
        field_0x20 = (u8*)mpFirstSaveAlpha;

        mpFirstGetAlpha = heap->alloc(mChildPaneCount, 0x20);
        field_0x28 = (u8*)mpFirstGetAlpha;
        setBackupAlpha();
    }
}

void CPaneMgrAlphaMorf::setBackupAlpha() {
    if (mpFirstStackAlpha != NULL) {
        field_0x20 = (u8*)mpFirstSaveAlpha;
        field_0x28 = (u8*)mpFirstGetAlpha;

        childPaneBackupAlpha(mPane->getFirstChildPane());
    }
}

void CPaneMgrAlphaMorf::setAlphaMorfRate(f32 rate) {
    if (mpFirstStackAlpha != NULL) {
        field_0x20 = (u8*)mpFirstSaveAlpha;
        field_0x28 = (u8*)mpFirstGetAlpha;

        childPaneSetAlphaMorf(mPane->getFirstChildPane(), rate);
    } else {
        setAlphaRate(rate);
    }
}

void CPaneMgrAlphaMorf::childPaneBackupAlpha(J2DPane* p_pane) {
    if (p_pane != NULL) {
        *field_0x20 = p_pane->getAlpha();
        field_0x20++;

        *field_0x28 = p_pane->getAlpha();
        field_0x28++;

        childPaneBackupAlpha(p_pane->getFirstChildPane());
        childPaneBackupAlpha(p_pane->getNextChildPane());
    }
}

void CPaneMgrAlphaMorf::childPaneSetAlphaMorf(J2DPane* p_pane, f32 morf) {
    if (p_pane != NULL) {
        if (*field_0x20 == *field_0x28) {
            p_pane->setAlpha(*field_0x20 * morf);
        } else {
            *field_0x20 = *field_0x28;
        }
        field_0x20++;
        field_0x28++;

        childPaneSetAlphaMorf(p_pane->getFirstChildPane(), morf);
        childPaneSetAlphaMorf(p_pane->getNextChildPane(), morf);
    }
}
