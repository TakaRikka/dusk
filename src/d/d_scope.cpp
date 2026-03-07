#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_scope.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "m_Do/m_Do_graphic.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"

typedef void (dScope_c::*initFunc)();
initFunc init_process[] = {
    &dScope_c::open_init,
    &dScope_c::move_init,
    &dScope_c::close_init,
};

typedef void (dScope_c::*moveFunc)();
moveFunc move_process[] = {
    &dScope_c::open_proc,
    &dScope_c::move_proc,
    &dScope_c::close_proc,
};

dScope_c::dScope_c(u8 param_0) : field_0x58(-1), field_0x5c(-1) {
    field_0x8d = param_0;
    ResTIMG* mp_image = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "wipe_00.bti");

    mHawkEyeScrn = NULL;
    mHawkEyeRootPane = NULL;
    mZoomInOutScrn = NULL;
    mZoomInOutRootPane = NULL;

    for (int i = 0; i < 3; i++) {
        mHawkEyePanes[i] = NULL;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            mZoomInOutPanes[i][j] = NULL;
        }
    }

    mpWipeTex = new J2DPicture(mp_image);
    mpWipeTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
    mWidth = mp_image->width;
    mHeight = mp_image->height;

    mp_image = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(mp_image);
    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
    mScale = 3.0f;
    mAlpha = 0.0f;
    mOpenTimer = 0;
    field_0x8a = 0;
    mProcess = PROC_OPEN;
    mIsDead = false;
    (this->*init_process[mProcess])();
}

dScope_c::~dScope_c() {
    if (mHawkEyeScrn != NULL) {
        delete mHawkEyeScrn;
        mHawkEyeScrn = NULL;
    }

    if (mHawkEyeRootPane != NULL) {
        delete mHawkEyeRootPane;
        mHawkEyeRootPane = NULL;
    }

    for (int i = 0; i < 3; i++) {
        if (mHawkEyePanes[i] != NULL) {
            delete mHawkEyePanes[i];
            mHawkEyePanes[i] = NULL;
        }
    }

    if (mZoomInOutScrn != NULL) {
        delete mZoomInOutScrn;
        mZoomInOutScrn = NULL;
    }

    if (mZoomInOutRootPane != NULL) {
        delete mZoomInOutRootPane;
        mZoomInOutRootPane = NULL;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            if (mZoomInOutPanes[i][j] != NULL) {
                delete mZoomInOutPanes[i][j];
                mZoomInOutPanes[i][j] = NULL;
            }
        }
    }

    delete mpWipeTex;
    mpWipeTex = NULL;

    delete mpBlackTex;
    mpBlackTex = NULL;

    dMeter2Info_setScopeZoomPointer(0);
}

int dScope_c::_execute(u32) {
    u8 old_proc = mProcess;
    (this->*move_process[mProcess])();

    if (!dComIfGp_checkCameraAttentionStatus(0, 8)) {
        mProcess = PROC_CLOSE;
    }

    if (mProcess != old_proc) {
        (this->*init_process[mProcess])();
    }

    if (mProcess != PROC_CLOSE) {
        dComIfGp_setCStickStatusForce(61, 10, 3);

        if (dComIfGp_checkPlayerStatus0(0, 0x1000)) {
            dComIfGp_setRStatusForce(0x11, 3);
        }
    }

    return 1;
}

void dScope_c::draw() {
    dComIfGp_getCurrentGrafPort()->setup2D();
    f32 temp_f1 = mScale;
    f32 temp_f31 = mWidth * temp_f1;
    f32 temp_f30 = mHeight * temp_f1;
    u8 alpha = mAlpha * 255.0f;

    if (dComIfGp_checkPlayerStatus0(0, 0x1000)) {
        J2DDrawLine(304.0f, mDoGph_gInf_c::getMinYF(), 304.0f, mDoGph_gInf_c::getMaxYF(),
                    JUtility::TColor(255, 0, 0, alpha), 6);
        J2DDrawLine(mDoGph_gInf_c::getMinXF(), 224.0f, mDoGph_gInf_c::getMaxXF(), 224.0f,
                    JUtility::TColor(255, 0, 0, alpha), 6);
    }

    mpWipeTex->setAlpha(alpha);
    mpBlackTex->setAlpha(alpha);

    f32 temp_f29 = 304.0f - temp_f31;
    f32 temp_f28 = 304.0f + temp_f31;
    f32 temp_f27 = 224.0f - temp_f30;
    f32 temp_f26 = 224.0f + temp_f30;

    mpWipeTex->draw(temp_f29, temp_f27, temp_f31, temp_f30, false, false, false);
    mpWipeTex->draw(304.0f, temp_f27, temp_f31, temp_f30, true, false, false);
    mpWipeTex->draw(temp_f29, 224.0f, temp_f31, temp_f30, false, true, false);
    mpWipeTex->draw(304.0f, 224.0f, temp_f31, temp_f30, true, true, false);

    mpBlackTex->draw(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                     mDoGph_gInf_c::getWidthF(), temp_f27 - mDoGph_gInf_c::getMinYF(), false, false,
                     false);
    mpBlackTex->draw(mDoGph_gInf_c::getMinXF(), temp_f26, mDoGph_gInf_c::getWidthF(),
                     mDoGph_gInf_c::getMaxYF() - temp_f26, false, false, false);
    mpBlackTex->draw(mDoGph_gInf_c::getMinXF(), temp_f27, temp_f29 - mDoGph_gInf_c::getMinXF(),
                     temp_f26 - temp_f27, false, false, false);
    mpBlackTex->draw(temp_f28, temp_f27, mDoGph_gInf_c::getMaxXF() - temp_f28, temp_f26 - temp_f27,
                     false, false, false);
}


bool dScope_c::isDead() {
    return mIsDead != false ? 1 : 0;
}

void dScope_c::open_init() {
    mScale = 3.0f;
    mAlpha = 0.0f;
    mOpenTimer = 0;
}

void dScope_c::open_proc() {
    mOpenTimer++;
    mScale = 3.0f - (mOpenTimer / 5.0f) * 1.5f;
    mAlpha = 1.0f;

    if (mOpenTimer >= 5) {
        mScale = 1.5f;
        mAlpha = 1.0f;
        mProcess = PROC_MOVE;
    }
}

void dScope_c::move_init() {}

void dScope_c::move_proc() {}

void dScope_c::close_init() {}

void dScope_c::close_proc() {
    if (mOpenTimer > 0) {
        mOpenTimer--;
        mScale = 3.0f - (mOpenTimer / 5.0f) * 1.5f;
        mAlpha = 1.0f;
    } else {
        mScale = 3.0f;
        mAlpha = 0.0f;
        mIsDead = true;
    }
}
