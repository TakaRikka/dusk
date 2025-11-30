/**
 * d_meter_hakusha.cpp
 * UI Epona Dash Spurs
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_meter_hakusha.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_pane_class.h"

dMeterHakusha_c::dMeterHakusha_c(void* i_screen) {
    field_0x004 = (J2DScreen*)i_screen;
    _create();
}

dMeterHakusha_c::~dMeterHakusha_c() {
    _delete();
}

int dMeterHakusha_c::_create() {
    static u64 haku_tag[] = {
        'haku_n00', 'haku_n01', 'haku_n02', 'haku_n03', 'haku_n04', 'haku_n05',
    };

    for (int i = 0; i < 6; i++) {
        mpHakushaPos[i] = new CPaneMgr(field_0x004, haku_tag[i], 0, NULL);
        JUT_ASSERT(0, mpHakushaPos[i] != NULL);
    }

    mpHakushaParent = new CPaneMgr(field_0x004, 'hakunall', 0, NULL);
    JUT_ASSERT(0, mpHakushaParent != NULL);

    mpHakushaScreen = new J2DScreen();
    JUT_ASSERT(0, mpHakushaScreen != NULL);

    bool fg = mpHakushaScreen->setPriority("zelda_game_image_hakusha_parts.blo", 0x20000,
                                           dComIfGp_getMain2DArchive());
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpHakushaScreen);

    mpHakushaOn = new CPaneMgr(mpHakushaScreen, 'haku_n', 2, NULL);
    JUT_ASSERT(0, mpHakushaOn != NULL);

    mpHakushaOff = new CPaneMgr(mpHakushaScreen, 'haku_b_n', 2, NULL);
    JUT_ASSERT(0, mpHakushaOff != NULL);

    mpHakushaOn->setAlphaRate(0.0f);
    mpHakushaOff->setAlphaRate(0.0f);
    mpHakushaOn->hide();
    mpHakushaOff->hide();

    for (int i = 0; i < 12; i++) {
        mHakushaData[i].pos_x = 0.0f;
        mHakushaData[i].pos_y = 0.0f;
        mHakushaData[i].flags = 0;
        mHakushaAnimFrame[i] = 0.0f;
        mHakushaStatus[i] = 0;
    }

    mHakushaNum = dMeter2Info_getHorseLifeCount();

    mpButtonScreen = new J2DScreen();
    JUT_ASSERT(0, mpButtonScreen != NULL);

    fg = mpButtonScreen->setPriority("zelda_game_image_hakusha_a_btn.blo", 0x20000,
                                     dComIfGp_getMain2DArchive());
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpButtonScreen);

    mpButtonA = new CPaneMgr(mpButtonScreen, 'abtn_n', 2, NULL);
    JUT_ASSERT(0, mpButtonA != NULL);
    mpButtonA->show();
    mpButtonA->setAlphaRate(0.0f);

    mpButtonScreen->search('info_n')->translate(0.0f, 0.0f);
    field_0x100 = g_drawHIO.mButtonAHorsePosX;
    field_0x104 = g_drawHIO.mButtonAHorsePosY;

    if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0) {
        mpHakushaParent->paneTrans(g_drawHIO.mSpurBarPosX + 28.4f, g_drawHIO.mSpurBarPosY);
    } else {
        mpHakushaParent->paneTrans(g_drawHIO.mSpurBarPosX, g_drawHIO.mSpurBarPosY);
    }
    mpHakushaParent->scale(g_drawHIO.mSpurBarScale, g_drawHIO.mSpurBarScale);

    mpButtonA->paneTrans(g_drawHIO.mButtonAHorsePosX, g_drawHIO.mButtonAHorsePosY);
    mpButtonA->scale(g_drawHIO.mButtonAHorseScale, g_drawHIO.mButtonAHorseScale);

    mpHakushaOn->scale(g_drawHIO.mSpurIconScale, g_drawHIO.mSpurIconScale);
    mpHakushaOff->scale(g_drawHIO.mUsedSpurIconScale, g_drawHIO.mUsedSpurIconScale);

    mButtonAPosX = 0.0f;
    mButtonAPosY = 0.0f;
    return cPhs_COMPLEATE_e;
}

int dMeterHakusha_c::_execute(u32 i_flags) {
    updateHakusha();
    alphaAnimeHakusha(i_flags);
    return 1;
}

void dMeterHakusha_c::draw() {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    graf_ctx->setup2D();

    mpButtonA->translate(mButtonAPosX, mButtonAPosY);
    mpButtonScreen->draw(0.0f, 0.0f, graf_ctx);

    for (int i = 0; i < getHakushaNum(); i++) {
        if (mHakushaData[i].flags & 1) {
            mpHakushaOn->show();
        } else {
            mpHakushaOn->hide();
        }

        if (mHakushaData[i].flags & 2) {
            mpHakushaOff->show();
        } else {
            mpHakushaOff->hide();
        }

        mpHakushaOn->translate(mHakushaData[i].pos_x, mHakushaData[i].pos_y);
        mpHakushaOff->translate(mHakushaData[i].pos_x, mHakushaData[i].pos_y);
        mpHakushaScreen->draw(0.0f, 0.0f, graf_ctx);

        if (mHakushaData[i].flags != 0 && mHakushaAnimFrame[i] != 0.0f) {
            Vec center = mpHakushaOn->getGlobalVtxCenter(false, 0);

            if (mHakushaStatus[i] == 0) {
                dMeter2Info_getMeterClass()->getMeterDrawPtr()->drawPikariHakusha(
                    center.x, center.y, mHakushaAnimFrame[i], g_drawHIO.mSpurIconPikariScale,
                    g_drawHIO.mSpurIconPikariFrontOuter, g_drawHIO.mSpurIconPikariFrontInner,
                    g_drawHIO.mSpurIconPikariBackOuter, g_drawHIO.mSpurIconPikariBackInner);
            } else {
                dMeter2Info_getMeterClass()->getMeterDrawPtr()->drawPikariHakusha(
                    center.x, center.y, mHakushaAnimFrame[i],
                    g_drawHIO.mSpurIconRevivePikariScale,
                    g_drawHIO.mSpurIconRevivePikariFrontOuter,
                    g_drawHIO.mSpurIconRevivePikariFrontInner,
                    g_drawHIO.mSpurIconRevivePikariBackOuter,
                    g_drawHIO.mSpurIconRevivePikariBackInner);
            }
        }
    }
}

int dMeterHakusha_c::_delete() {
    for (int i = 0; i < 6; i++) {
        delete mpHakushaPos[i];
        mpHakushaPos[i] = NULL;
    }

    mpHakushaParent->paneTrans(0.0f, 0.0f);
    mpHakushaParent->scale(1.0f, 1.0f);
    delete mpHakushaParent;
    mpHakushaParent = NULL;

    delete mpHakushaScreen;
    mpHakushaScreen = NULL;

    delete mpHakushaOn;
    mpHakushaOn = NULL;

    delete mpHakushaOff;
    mpHakushaOff = NULL;

    delete mpButtonScreen;
    mpButtonScreen = NULL;

    delete mpButtonA;
    mpButtonA = NULL;
    return 1;
}

void dMeterHakusha_c::alphaAnimeHakusha(u32 i_flags) {
    if ((i_flags & 0x4000) || (i_flags & 0x40) || (i_flags & 0x100000) || (i_flags & 0x1000) ||
        (i_flags & 8) || (i_flags & 0x10) || (i_flags & 0x20) || (i_flags & 0x04000000) ||
        (i_flags & 0x08000000) || (i_flags & 0x01000000) || !(i_flags & 0x02000000) ||
        (strcmp(dComIfGp_getStartStageName(), "F_SP00") == 0 &&
         dComIfG_play_c::getLayerNo(0) == 4) ||
        (dComIfGp_getDoStatus() != 9 && dComIfGp_getDoStatus() != 0))
    {
        setAlphaHakushaAnimeMin();
        setAlphaButtonAnimeMin();
        return;
    }

    setAlphaHakushaAnimeMax();
    if (dComIfGp_getDoStatus() == 9) {
        setAlphaButtonAnimeMax();
    } else {
        setAlphaButtonAnimeMin();
    }
}

void dMeterHakusha_c::updateHakusha() {
    Vec sp2C = mpHakushaPos[0]->getGlobalVtxCenter(false, 0);
    Vec sp20 = mpHakushaPos[5]->getGlobalVtxCenter(false, 0);

    f32 abtn_x_offset = sp2C.x;
    f32 abtn_y_offset = sp2C.y;

    f32 temp_f28 = (sp20.x - sp2C.x) / (f32)getHakushaNum();

    if (mHakushaNum != dMeter2Info_getHorseLifeCount()) {
        if (mHakushaNum > dMeter2Info_getHorseLifeCount()) {
            mHakushaAnimFrame[dMeter2Info_getHorseLifeCount()] =
                18.0f - g_drawHIO.mSpurIconPikariAnimSpeed;
            mHakushaStatus[dMeter2Info_getHorseLifeCount()] = 0;
        } else if (mHakushaNum < dMeter2Info_getHorseLifeCount()) {
            for (int i = mHakushaNum; i < dMeter2Info_getHorseLifeCount(); i++) {
                mHakushaAnimFrame[i] = 18.0f - g_drawHIO.mSpurIconRevivePikariAnimSpeed;
                mHakushaStatus[i] = 1;
            }
        }

        mHakushaNum = dMeter2Info_getHorseLifeCount();
    }

    for (int i = 0; i < getHakushaNum(); i++) {
        if (mHakushaAnimFrame[i] > 0.0f) {
            if (mHakushaStatus[i] == 0) {
                mHakushaAnimFrame[i] += g_drawHIO.mSpurIconPikariAnimSpeed;
            } else {
                mHakushaAnimFrame[i] += g_drawHIO.mSpurIconRevivePikariAnimSpeed;
            }

            if (mHakushaAnimFrame[i] > 28.0f) {
                mHakushaAnimFrame[i] = 0.0f;
            }
        }

        mHakushaData[i].pos_x = abtn_x_offset;
        mHakushaData[i].pos_y = abtn_y_offset;

        if (mpHakushaOn->getAlpha() == 0) {
            mHakushaData[i].flags &= ~0x1;
        } else if (i < dMeter2Info_getHorseLifeCount() ||
                   (mHakushaAnimFrame[i] != 0.0f && mHakushaAnimFrame[i] <= 20.0f &&
                    mHakushaStatus[i] == 0) ||
                   (mHakushaAnimFrame[i] != 0.0f && mHakushaAnimFrame[i] > 20.0f &&
                    mHakushaStatus[i] == 1))
        {
            mHakushaData[i].flags |= 0x1;
        } else {
            mHakushaData[i].flags &= ~0x1;
        }

        if (mpHakushaOff->getAlpha() == 0) {
            mHakushaData[i].flags &= ~0x2;
        } else if (i < dMeter2Info_getHorseLifeCount() ||
                   (mHakushaAnimFrame[i] != 0.0f && mHakushaAnimFrame[i] <= 20.0f &&
                    mHakushaStatus[i] == 0) ||
                   (mHakushaAnimFrame[i] != 0.0f && mHakushaAnimFrame[i] > 20.0f &&
                    mHakushaStatus[i] == 1))
        {
            mHakushaData[i].flags &= ~0x2;
        } else {
            mHakushaData[i].flags |= 0x2;
        }

        abtn_x_offset += temp_f28;
    }

    mButtonAPosX = abtn_x_offset + field_0x100;
    mButtonAPosY = abtn_y_offset + field_0x104;

    if (g_drawHIO.mSpurDebug) {
        field_0x100 = g_drawHIO.mButtonAHorsePosX;
        field_0x104 = g_drawHIO.mButtonAHorsePosY;

        if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0) {
            mpHakushaParent->paneTrans(g_drawHIO.mSpurBarPosX + 28.4f, g_drawHIO.mSpurBarPosY);
        } else {
            mpHakushaParent->paneTrans(g_drawHIO.mSpurBarPosX, g_drawHIO.mSpurBarPosY);
        }
        mpHakushaParent->scale(g_drawHIO.mSpurBarScale, g_drawHIO.mSpurBarScale);

        mpButtonA->paneTrans(g_drawHIO.mButtonAHorsePosX, g_drawHIO.mButtonAHorsePosY);
        mpButtonA->scale(g_drawHIO.mButtonAHorseScale, g_drawHIO.mButtonAHorseScale);

        mpHakushaOn->scale(g_drawHIO.mSpurIconScale, g_drawHIO.mSpurIconScale);
        mpHakushaOff->scale(g_drawHIO.mUsedSpurIconScale, g_drawHIO.mUsedSpurIconScale);
    }
}

void dMeterHakusha_c::setAlphaHakushaAnimeMin() {
    if (mpHakushaOn->getAlphaRate() != 0.0f) {
        mpHakushaOn->setAlphaRate(g_drawHIO.mSpurIconAlpha);
        dMeter2Info_getMeterClass()->getMeterDrawPtr()->setAlphaAnimeMin(mpHakushaOn, 5);
    }

    if (mpHakushaOff->getAlphaRate() != 0.0f) {
        mpHakushaOff->setAlphaRate(g_drawHIO.mUsedSpurIconAlpha);
        dMeter2Info_getMeterClass()->getMeterDrawPtr()->setAlphaAnimeMin(mpHakushaOff, 5);
    }
}

void dMeterHakusha_c::setAlphaHakushaAnimeMax() {
    if (mpHakushaOn->getAlphaRate() != g_drawHIO.mSpurIconAlpha) {
        mpHakushaOn->setAlphaRate(g_drawHIO.mSpurIconAlpha);
        dMeter2Info_getMeterClass()->getMeterDrawPtr()->setAlphaAnimeMax(mpHakushaOn, 5);
    }

    if (mpHakushaOff->getAlphaRate() != g_drawHIO.mUsedSpurIconAlpha) {
        mpHakushaOff->setAlphaRate(g_drawHIO.mUsedSpurIconAlpha);
        dMeter2Info_getMeterClass()->getMeterDrawPtr()->setAlphaAnimeMax(mpHakushaOff, 5);
    }
}

void dMeterHakusha_c::setAlphaButtonAnimeMin() {
    if (mpButtonA->getAlphaRate() != 0.0f) {
        mpButtonA->setAlphaRate(1.0f);
        dMeter2Info_getMeterClass()->getMeterDrawPtr()->setAlphaAnimeMin(mpButtonA, 5);
    }
}

void dMeterHakusha_c::setAlphaButtonAnimeMax() {
    if (mpButtonA->getAlphaRate() != 1.0f) {
        mpButtonA->setAlphaRate(1.0f);
        dMeter2Info_getMeterClass()->getMeterDrawPtr()->setAlphaAnimeMax(mpButtonA, 5);
    }
}

int dMeterHakusha_c::getHakushaNum() {
    int hakusha_num = g_drawHIO.mMaxSpurAmount;
    if (hakusha_num > 12) {
        hakusha_num = 12;
    }

    return hakusha_num;
}
