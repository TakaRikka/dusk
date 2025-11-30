#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_select_cursor.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "d/d_com_inf_game.h"
#include "JSystem/J2DGraph/J2DAnimation.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"

dSelect_cursorHIO_c::dSelect_cursorHIO_c() {
    field_0x8 = 1.0f;
    mXAxisExpansion = 1.0f;
    mYAxisExpansion = 1.0f;
    mOscillation = 3.0f / 10.0f;
    field_0x18 = 1.0f;
    mRatioX = 1.0f;
    mRatioY = 1.0f;
    mDebugON = false;
}

dSelect_cursor_c::dSelect_cursor_c(u8 param_0, f32 param_1, JKRArchive* param_2) {
    static char* blo_name[4] = {
        "zelda_select_cursor_4parts.blo",
        "zelda_store_select_icon.blo",
        "zelda_map_screen_portal_icon.blo",
        "zelda_map_screen_batsumark.blo",
    };

    static char* bck_name[4] = {
        "",
        "zelda_store_select_icon.bck",
        "",
        "",
    };

    static char* bpk_name[4] = {
        "zelda_select_cursor_4parts.bpk",
        "zelda_store_select_icon.bpk",
        "zelda_map_screen_portal_icon.bpk",
        "zelda_map_screen_batsumark.bpk",
    };

    static char* btk_name[4] = {
        "zelda_select_cursor_4parts.btk",
        "zelda_store_select_icon.btk",
        "",
        "",
    };

    static char* btk2_name[4] = {
        "",
        "zelda_store_select_icon_02.btk",
        "",
        "",
    };

    mpCursorHIO = new dSelect_cursorHIO_c();
    mpCursorHIO->field_0x4 = -1;
    mNameIdx = 0;
    field_0xb6 = param_0;
    field_0xb4 = 0xff;
    mUpdateFlag = true;
    mpPane = NULL;
    for (int i = 0; i < 4; i++) {
        field_0x94[i] = 0.0f;
        field_0xa4[i] = 0.0f;
        field_0x74[i] = 0.0f;
        field_0x84[i] = 0.0f;
    }
    mParam1 = mpCursorHIO->mXAxisExpansion;
    mParam2 = mpCursorHIO->mYAxisExpansion;
    mParam3 = mpCursorHIO->mOscillation;
    mParam4 = mpCursorHIO->mRatioX;
    mParam5 = mpCursorHIO->mRatioY;

    switch(field_0xb6) {
        case 1:
        case 2:
        case 3:
        case 4:
            mNameIdx = 0;
            break;
        case 5:
            mNameIdx = 2;
            break;
        case 6:
            mNameIdx = 3;
            break;
    }

    if (param_2 == NULL) {
        param_2 = dComIfGp_getMain2DArchive();
    }

    mpScreen = new J2DScreen();
    mpScreen->setPriority(blo_name[mNameIdx], 0x20000, param_2);
    dPaneClass_showNullPane(mpScreen);

    for (int i = 0; i < 4; i++) {
        field_0x1C[i] = NULL;
    }

    switch(mNameIdx) {
        case 0:
            mpPaneMgr = new CPaneMgr(mpScreen, 'n_all', 2, NULL);
            mpPaneMgr->hide();
            mpPaneMgr->setAlpha(0);

            static u64 const corner_tag[4] = {'l_u_null', 'l_d_null', 'r_u_null', 'r_d_null'};
            for (int i = 0; i< 4; i++) {
                field_0x1C[i] = new CPaneMgr(mpScreen, corner_tag[i], 0, NULL);
                field_0x94[i] = mpScreen->search(corner_tag[i])->getTranslateX();
                field_0xa4[i] = mpScreen->search(corner_tag[i])->getTranslateY();
                field_0x74[i] = field_0x94[i];
                field_0x84[i] = field_0xa4[i];
            }

            mpPaneMgr->scale(param_1, param_1);
            field_0x14 = NULL;
            field_0x18 = NULL;
            break;
        case 1:
            mpPaneMgr = new CPaneMgr(mpScreen, 'n_all', 2, NULL);
            mpPaneMgr->hide();
            mpPaneMgr->setAlpha(0);
            mpPaneMgr->scale(param_1, param_1);
            field_0x14 = new CPaneMgr(mpScreen, 'sel_po00', 0, NULL);
            field_0x14->hide();
            field_0x18 = new CPaneMgr(mpScreen, 'n_all2', 2, NULL);
            break;
        case 2:
            mpPaneMgr = new CPaneMgr(mpScreen, 'Null', 2, NULL);
            mpPaneMgr->hide();
            mpPaneMgr->setAlpha(0);
            mpPaneMgr->scale(param_1, param_1);
            field_0x14 = NULL;
            field_0x18 = NULL;
            break;
        case 3:
            mpPaneMgr = new CPaneMgr(mpScreen, 'batsu', 2, NULL);
            mpPaneMgr->hide();
            mpPaneMgr->setAlpha(0);
            mpPaneMgr->scale(param_1, param_1);
            field_0x14 = NULL;
            field_0x18 = NULL;
            break;
    }

    mpSelectIcon = NULL;
    OSInitFastCast();
    if (strcmp(bpk_name[mNameIdx], "") != 0) {
        field_0x30 = (J2DAnmColor*)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource(bpk_name[mNameIdx], param_2));
        field_0x30->searchUpdateMaterialID(mpScreen);
    } else {
        field_0x30 = NULL;
    }

    field_0x44 = 0.0f;
    if (strcmp(btk_name[mNameIdx], "") != 0) {
        field_0x34[0] = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource(btk_name[mNameIdx], param_2));
        field_0x34[0]->searchUpdateMaterialID(mpScreen);
    } else {
        field_0x34[0] = NULL;
    }

    field_0x48[0] = 0.0f;
    if (strcmp(btk2_name[mNameIdx], "") != 0) {
        field_0x34[1] = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource(btk2_name[mNameIdx], param_2));
        field_0x34[1]->searchUpdateMaterialID(mpScreen);
    } else {
        field_0x34[1] = NULL;
    }

    field_0x48[1] = 0.0f;
    if (strcmp(bck_name[mNameIdx], "") != 0) {
        field_0x2C = (J2DAnmTransformKey*)J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource(bck_name[mNameIdx], param_2));
    } else {
        field_0x2C = NULL;
    }

    field_0x40 = 0.0f;
    switch(mNameIdx) {
        case 1:
            field_0x50 = mpScreen->search('ssel_ico')->getTranslateX();
            field_0x54 = mpScreen->search('ssel_ico')->getTranslateY();
            break;
        case 0:
        case 2:
        case 3:
            field_0x50 = 0.0f;
            field_0x54 = 0.0f;
            break;
    }

    mPositionY = 0.0f;
    mPositionX = 0.0f;
}


dSelect_cursor_c::~dSelect_cursor_c() {
    delete mpScreen;
    mpScreen = NULL;
    if (mpPaneMgr) {
        delete mpPaneMgr;
        mpPaneMgr = NULL;
    }
    for (int i = 0; i < 4; i++) {
        if (field_0x1C[i]) {
            delete field_0x1C[i];
            field_0x1C[i] = NULL;
        }
    }
    if (field_0x14) {
        delete field_0x14;
        field_0x14 = NULL;
    }
    if (field_0x18) {
        delete field_0x18;
        field_0x18 = NULL;
    }
    if (mpSelectIcon) {
        delete mpSelectIcon;
        mpSelectIcon = NULL;
    }
    if (field_0x30) {
        delete field_0x30;
        field_0x30 = NULL;
    }
    for (int i = 0; i < 2; i++) {
        if (field_0x34[i]) {
            delete field_0x34[i];
            field_0x34[i] = NULL;
        }
    }
    if (field_0x2C) {
        delete field_0x2C;
        field_0x2C = NULL;
    }
    if (mpCursorHIO) {
        delete mpCursorHIO;
        mpCursorHIO = NULL;
    }
}

void dSelect_cursor_c::draw() {
    mpPaneMgr->getAlphaRate();
    update();
    J2DGrafContext* gphCtx = dComIfGp_getCurrentGrafPort();
    gphCtx->setup2D();
    mpScreen->draw(0.0f, 0.0f, gphCtx);
    if (mpSelectIcon) {
        mpSelectIcon->drawSelf(); // inline here, but not sure how to properly define it
    }
}

void dSelect_cursor_c::update() {
    f32 fVar1 = 1.0f;
    if (field_0xb6 == 3) {
        fVar1 = 0.5f;
    }
    mpPaneMgr->translate(mPositionX, mPositionY);
    if (mpCursorHIO->mDebugON) {
        mParam1 = mpCursorHIO->mXAxisExpansion;
        mParam2 = mpCursorHIO->mYAxisExpansion;
        mParam3 = mpCursorHIO->mOscillation;
        mParam4 = mpCursorHIO->mRatioX;
        mParam5 = mpCursorHIO->mRatioY;
    }

    if (mUpdateFlag) {
        if (field_0x30) {
            if (chkPlayAnime(0)) {
                if (mNameIdx == 1) {
                    field_0x44 += mpCursorHIO->field_0x8 * fVar1;
                } else {
                    field_0x44 += fVar1;
                }

                if (field_0x44 >= field_0x30->getFrameMax()) {
                    field_0x44 -= field_0x30->getFrameMax();
                }

                field_0x30->setFrame(field_0x44);
                setBpkAnimation(field_0x30);
            } else {
                if (field_0x44 != 1.0f) {
                    field_0x44 = 1.0f;
                    field_0x30->setFrame(field_0x44);
                    setBpkAnimation(field_0x30);
                }
            }
        }

        for (int i = 0; i < 2; i++) {
            if (field_0x34[i]) {
                if ((i == 0 && chkPlayAnime(2)) || (i == 1 && chkPlayAnime(3))) {
                    if (mNameIdx == 1) {
                        field_0x48[i] += mpCursorHIO->field_0x8 * fVar1;
                    } else {
                        field_0x48[i] += fVar1;
                    }
                    if (field_0x48[i] >= field_0x34[i]->getFrameMax()) {
                        field_0x48[i] -= field_0x34[i]->getFrameMax();
                    }

                    field_0x34[i]->setFrame(field_0x48[i]);
                }
                setBtk0Animation(field_0x34[i]);
            }
        }

        if (field_0x2C && chkPlayAnime(1)) {
            if (mNameIdx == 1) {
                    field_0x40 += mpCursorHIO->field_0x8 * fVar1;
                } else {
                    field_0x40 += fVar1;
                }
                if (field_0x40 >= field_0x2C->getFrameMax()) {
                    field_0x40 -= field_0x2C->getFrameMax();
                }
                
                field_0x2C->setFrame(field_0x40);
                setBckAnimation(field_0x2C);

        }

        if (chkPlayAnime(1) && mNameIdx == 0) {
            setCursorAnimation();
        }

        mpScreen->animation();
    }

    if (mpSelectIcon && field_0x14) {
        Vec res = field_0x14->getGlobalVtxCenter(field_0x14->mPane, false, 0);
        mpSelectIcon->setPos(field_0x14->getPanePtr(), res.x, res.y);
        mpSelectIcon->animation();
        mpSelectIcon->setAlpha(field_0x14->getPanePtr()->getAlpha());
    }

    for (int i = 0; i < 4; i++) {
        if (field_0x1C[i]) {
            field_0x1C[i]->scale(mParam4, mParam5);
        }
    }
}

void dSelect_cursor_c::setPos(f32 i_posX, f32 i_posY, J2DPane* i_pane, bool i_scaleBounds) {
    mpPane = i_pane;
    setPos(i_posX,i_posY);

    if (!i_pane) {
        return;
    }

    f32 val1 = 1.0f;
    f32 width = (i_pane->getWidth()) * val1 * 0.5f;
    f32 height = i_pane->getHeight() * 0.5f;

    if (i_scaleBounds) {
        width *= i_pane->getScaleX();
        height *= i_pane->getScaleY();
    }

    f32 tmp7 = -1.0f;

    for (int i = 0; i < 4; i++) {
        
        field_0x94[i] = width;
        field_0xa4[i] = height;

        if (i < 2) {
            field_0x94[i] *= tmp7;
        }

        if (i % 2 == 0) {
            field_0xa4[i] *= tmp7;
        }
    }

}

void dSelect_cursor_c::setParam(f32 i_param1, f32 i_param2, f32 i_param3, f32 i_param4,
                                    f32 i_param5) {
    mParam1 = i_param1;
    mParam2 = i_param2;
    mParam3 = i_param3;
    mParam4 = i_param4;
    mParam5 = i_param5;
}

void dSelect_cursor_c::setScale(f32 i_scale) {
    J2DPane* pane = mpPaneMgr->getPanePtr();
    pane->scale(i_scale,i_scale);

    switch (mNameIdx) {
    case 1:
        field_0x50 = mpScreen->search('ssel_ico')->getTranslateX();
        field_0x54 = mpScreen->search('ssel_ico')->getTranslateY();
        break;
    case 0:
    case 2:
    case 3:
        field_0x50 = 0.0f;
        field_0x54 = 0.0f;
        break;
    }
}

void dSelect_cursor_c::setAlphaRate(f32 i_alphaRate) {
    if (i_alphaRate > 0.0f) {
        if (mpPaneMgr->isVisible() == 0) {
            mpPaneMgr->show();
        }
    } else {
        i_alphaRate = 0.0f;
        if (mpPaneMgr->isVisible() == 1) {
            mpPaneMgr->hide();
        }
    }

    mpPaneMgr->setAlphaRate(i_alphaRate);
}

int dSelect_cursor_c::addAlpha() {
    s16 alpha_timer = mpPaneMgr->getAlphaTimer();

    if (mpPaneMgr->isVisible() == 0) {
        mpPaneMgr->show();
    }

    if (alpha_timer >= 5) {
        return 1;
    } else {
        alpha_timer++;
        mpPaneMgr->alphaAnimeStart(alpha_timer);
        mpPaneMgr->setAlphaRate(alpha_timer/5.0f);
    }

    return 0;
}

int dSelect_cursor_c::decAlpha() {
    s16 alpha_timer = mpPaneMgr->getAlphaTimer();

    if (alpha_timer <= 0) {
        if (mpPaneMgr->isVisible() == 1) {
            mpPaneMgr->hide();
        }
        return 1;
    } else {
        alpha_timer--;
        mpPaneMgr->alphaAnimeStart(alpha_timer);
        mpPaneMgr->setAlphaRate(alpha_timer/5.0f);
    }

    return 0;
}

void dSelect_cursor_c::setBpkAnimation(J2DAnmColor* param_0) {
    switch (mNameIdx) {
    case 0: {
        static u64 const tag_4181[4] = {'l_ld', 'l_lu', 'l_rd', 'l_ru'};
        for (int i = 0; i < 4; i++) {
            mpScreen->search(tag_4181[i])->setAnimation(param_0);
        }
        break;
    }
    case 2:
        mpScreen->search('light')->setAnimation(param_0);
        mpScreen->search('back_l')->setAnimation(param_0);
        break;
    case 3:
        mpScreen->search('batsu')->setAnimation(param_0);
        break;
    default:
        mpScreen->search('light00')->setAnimation(param_0);
        break;
    }
}

void dSelect_cursor_c::setBtk0Animation(J2DAnmTextureSRTKey* param_0) {
    switch (mNameIdx) {
    case 0: {
        static u64 const tag_4197[8] = {
            'i_c_ld1', 'i_c_ld2', 'i_c_lu1', 'i_c_lu2', 'i_c_rd1', 'i_c_rd2', 'i_c_ru1', 'i_c_ru2', 
        };
        for (int i = 0; i < 8; i++) {
            mpScreen->search(tag_4197[i])->setAnimation(param_0);
        }
        break;
    }
    case 1: {
        static u64 const tag_4204[2] = {'gold01', 'gold02'};
        for (int i = 0; i < 2; i++) {
            mpScreen->search(tag_4204[i])->setAnimation(param_0);
        }
        break;
    }
    default:
        mpScreen->search('gold01')->setAnimation(param_0);
        break;
    }
}

void dSelect_cursor_c::setCursorAnimation() {
    f32 fVar1 = 1.0f;
    if (field_0xb6 == 3) {
        fVar1 = 0.5f;
    }

    field_0x40 += fVar1;
    if (field_0x40 >= 20.0f) {
        field_0x40 -= 20.0f;
    }
    f32 fVar2;
    f32 param3 = mParam3;
    fVar2 = field_0x40;
    if (fVar2 < 10.0f) {
        fVar2 /= 10.0f;
    } else {
        fVar2 = (20.0f - fVar2) / 10.0f;
    }

    for (int i = 0; i < 4; i++) {
        field_0x74[i] = mParam1 * (field_0x94[i] * ((1.0f - param3) + fVar2 * param3));
        field_0x84[i] = mParam2 * (field_0xa4[i] * ((1.0f - param3) + fVar2 * param3));
    }

    moveCenter(mpScreen->search('l_u_null'), field_0x74[0], field_0x84[0]);
    moveCenter(mpScreen->search('l_d_null'), field_0x74[1], field_0x84[1]);
    moveCenter(mpScreen->search('r_u_null'), field_0x74[2], field_0x84[2]);
    moveCenter(mpScreen->search('r_d_null'), field_0x74[3], field_0x84[3]);
}


void dSelect_cursor_c::setBckAnimation(J2DAnmTransformKey* param_0) {
    mpScreen->search('ssel_ico')->setAnimation((J2DAnmTransform*)param_0);
}

void dSelect_cursor_c::moveCenter(J2DPane* i_pane, f32 i_x, f32 i_y) {
    i_pane->translate(i_x,i_y);
}
