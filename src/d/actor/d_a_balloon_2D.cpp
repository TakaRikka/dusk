/**
 * @file d_a_balloon_2D.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_balloon_2D.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JHostIO/JORServer.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_pane_class.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"

class daBalloon2D_HIO_c : public mDoHIO_entry_c {
public:
    inline daBalloon2D_HIO_c() {
        static param aParam = {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0.0f,
            1.0f,
            0.0f,
            -10.0f,
            0.8f,
            0.0f,
            -13.0f,
            1.1f,
            0.9f,
            0.7f,
            0.0f,
            18.0f,
            1.0f,
            -6.6f,
            6.6f,
            1.3f,
            -1.3f,
            9.2f,
            1.0f,
            0.0f,
            6.6f,
            1.1f,
            0.9f,
            1.0f,
            3.9f,
            -9.2f,
            0.85f,
            0.8f,
            6.6f,
            1.3f,
            0.75f,
            0.9f,
            3.9f,
            0.0f,
            0.85f,
            0.9f,
            3.9f,
            0.0f,
            0.95f,
            0.9f,
            22.0f,
            -17.0f,
            1.0f,
            20.0f,
        };
        memcpy(&m, &aParam, sizeof(aParam));
    }
    virtual ~daBalloon2D_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);

#endif

    void resetAddScoreSetFlag();

    struct param {
        /* 0x04 */ u8 mAdjustmentToggle;
        /* 0x08 */ s32 mScore;
        /* 0x0C */ u8 mComboCount;
        /* 0x0D */ u8 mBalloonSize;
        /* 0x0E */ u8 mOverheadScoreToggle;
        /* 0x10 */ s32 mOverheadScore;
        /* 0x14 */ s32 field_0x14;
        /* 0x18 */ f32 field_0x1c;
        /* 0x1C */ f32 field_0x20;
        /* 0x20 */ f32 mScorePosX;
        /* 0x24 */ f32 mScorePosY;
        /* 0x28 */ f32 mScoreSize;
        /* 0x2C */ f32 m5DNumberPosX;
        /* 0x30 */ f32 m5DNumberPosY;
        /* 0x34 */ f32 m5DNumberSizeX;
        /* 0x38 */ f32 m5DNumberSizeY;
        /* 0x3C */ f32 m5DNumberAlpha;
        /* 0x40 */ f32 m5DNumber2PosX;
        /* 0x44 */ f32 m5DNumber2PosY;
        /* 0x48 */ f32 m5DNumber2Size;
        /* 0x4C */ f32 mUnkPosX;
        /* 0x50 */ f32 mUnkPosY;
        /* 0x54 */ f32 mUnkSize;
        /* 0x58 */ f32 mComboPosX;
        /* 0x5C */ f32 mComboPosY;
        /* 0x60 */ f32 mComboSize;
        /* 0x64 */ f32 m2DNumberPosX;
        /* 0x68 */ f32 m2DNumberPosY;
        /* 0x6C */ f32 m2DNumberSizeX;
        /* 0x70 */ f32 m2DNumberSizeY;
        /* 0x74 */ f32 field_0x78;
        /* 0x78 */ f32 m2DNumberComboPosX;
        /* 0x7C */ f32 m2DNumberComboPosY;
        /* 0x80 */ f32 m2DNumberComboSize;
        /* 0x84 */ f32 m2DNumberComboAlpha;
        /* 0x88 */ f32 mBalloonLargePosX;
        /* 0x8C */ f32 mBalloonLargePosY;
        /* 0x90 */ f32 mBalloonLargeSize;
        /* 0x94 */ f32 mBalloonLargeAlpha;
        /* 0x98 */ f32 mBalloonMedPosX;
        /* 0x9C */ f32 mBalloonMedPosY;
        /* 0xA0 */ f32 mBalloonMedSize;
        /* 0xA4 */ f32 mBalloonMedAlpha;
        /* 0xA8 */ f32 mBalloonSmallPosX;
        /* 0xAC */ f32 mBalloonSmallPosY;
        /* 0xB0 */ f32 mBalloonSmallSize;
        /* 0xB4 */ f32 mBalloonSmallAlpha;
        /* 0xB8 */ f32 m2DNumberCombo2PosX;
        /* 0xBC */ f32 m2DNumberCombo2PosY;
        /* 0xC0 */ f32 m2DNumberCombo2Size;
        /* 0xC4 */ f32 m2DNumberCombo2PosX43;
    };

    param m;
};

void daBalloon2D_c::c_list::draw() {
    dComIfGp_getCurrentGrafPort()->setup2D();
    mBalloon->drawMeter();
}

static int daBalloon2D_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daBalloon2D_c*>(i_this)->createHeap();
}

static char* l_arcName = "Balloon2D";

int daBalloon2D_c::createHeap() {
    mScreen = new J2DScreen();
    if (mScreen == NULL) {
        return  0;
    }
    dRes_info_c* resInfo = dComIfG_getObjectResInfo(l_arcName);
    JUT_ASSERT(445, resInfo != NULL);
    JKRArchive* arc = resInfo->getArchive();
    mScreen->setPriority("zelda_balloon_game.blo", 0x20000, arc);
    dPaneClass_showNullPane(mScreen);
    field_0x578 = new CPaneMgr(mScreen, 'n_all', 2, NULL);
    field_0x57c = new CPaneMgr(mScreen, 'score_tn', 0, NULL);
    field_0x580 = new CPaneMgr(mScreen, 'suji_n', 2, NULL);
    field_0x584 = new CPaneMgr(mScreen, 's_set_n', 0, NULL);
    field_0x588 = new CPaneMgr(mScreen, 'tas_n', 0, NULL);
    field_0x58c = new CPaneMgr(mScreen, 'combo_tn', 0, NULL);
    field_0x590 = new CPaneMgr(mScreen, 'num_n', 0, NULL);
    field_0x594 = new CPaneMgr(mScreen, 'co_set_n', 2, NULL);
    field_0x598 = new CPaneMgr(mScreen, 'bal_3_n', 2, NULL);
    field_0x59c = new CPaneMgr(mScreen, 'bal_2_n', 2, NULL);
    field_0x5a0 = new CPaneMgr(mScreen, 'bal_1_n', 2, NULL);
    field_0x5a4 = new CPaneMgr(mScreen, 'ba_com_n', 2, NULL);
    field_0x578->setAlphaRate(0.0f);
    field_0x5a4->setAlphaRate(0.0f);
    for (s32 i = 0; i < 10; i++) {
        ResTIMG* resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(i));
        field_0x5c4[i] = new J2DPicture(resTimg);
    }
    ResTIMG* resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getPlusTextureName());
    field_0x5ec = new J2DPicture(resTimg);
    if (field_0x578 == NULL || field_0x57c == NULL || field_0x580 == NULL ||
        field_0x584 == NULL ||field_0x588 == NULL || field_0x58c == NULL ||
        field_0x590 == NULL || field_0x594 == NULL || field_0x598 == NULL ||
        field_0x59c == NULL || field_0x5a0 == NULL || field_0x5a4 == NULL || field_0x5c4[0] == NULL || field_0x5c4[1] == NULL || field_0x5c4[2] == NULL ||
        field_0x5c4[3] == NULL || field_0x5c4[4] == NULL || field_0x5c4[5] == NULL ||
        field_0x5c4[6] == NULL || field_0x5c4[7] == NULL || field_0x5c4[8] == NULL ||
        field_0x5c4[9] == NULL || field_0x5ec == NULL)
    {
        return 0;
    }
    J2DTextBox* combos[2];
    J2DTextBox* scores[2];
    combos[0] = (J2DTextBox*)mScreen->search('combo_ts');
    combos[1] = (J2DTextBox*)mScreen->search('combo_t');
    scores[0] = (J2DTextBox*)mScreen->search('score_ts');
    scores[1] = (J2DTextBox*)mScreen->search('score_t');
    field_0x5a8[0] = (J2DPicture*)mScreen->search('suji_4');
    field_0x5a8[1] = (J2DPicture*)mScreen->search('suji_3');
    field_0x5a8[2] = (J2DPicture*)mScreen->search('suji_2');
    field_0x5a8[3] = (J2DPicture*)mScreen->search('suji_1');
    field_0x5a8[4] = (J2DPicture*)mScreen->search('suji_0');
    field_0x5a8[5] = (J2DPicture*)mScreen->search('num_1');
    field_0x5a8[6] = (J2DPicture*)mScreen->search('num_0');
    for (int i = 0; i < 2; i++) {
        combos[i]->setString(32, "");
        scores[i]->setString(32, "");
        dMeter2Info_getStringKanji(0x53f, combos[i]->getStringPtr(), 0);
        dMeter2Info_getStringKanji(0x53e, scores[i]->getStringPtr(), 0);
        combos[i]->setFont(mDoExt_getMesgFont());
        scores[i]->setFont(mDoExt_getMesgFont());
    }
    setComboNum(0);
    setBalloonSize(0);
    setScoreNum(0);
    field_0x5f0.set(this);
    if ((fopAcM_GetParam(this) & 1)) {
        show();
    } else {
        hide();
    }
    return 1;
}

daBalloon2D_HIO_c l_HOSTIO;

daBalloon2D_c* daBalloon2D_c::myclass;

int daBalloon2D_c::create() {
    int rv = dComIfG_resLoad(this, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daBalloon2D_createHeap, 0)) {
            return cPhs_ERROR_e;
        }
        JUT_ASSERT(566, daBalloon2D_c::myclass == NULL);
        myclass = this;
        setHIO(true);
    }
    return rv;
}

int daBalloon2D_c::destroy() {
    dComIfG_resDelete(this, l_arcName);
    myclass = NULL;
    dMeter2Info_offSub2DStatus(1);
    return 1;
}

int daBalloon2D_c::draw() {
    if (isVisible() && !dComIfGp_isPauseFlag() &&
        !dMsgObject_isTalkNowCheck())
    {
        dComIfGd_set2DOpa(&field_0x5f0);
        dMeter2Info_onSub2DStatus(1);
    } else {
        dMeter2Info_offSub2DStatus(1);
    }
    return 1;
}


int daBalloon2D_c::execute() {
    setAllAlpha();
    setComboAlpha();
    setHIO(false);
    return 1;
}

void daBalloon2D_c::drawMeter() {
    update();
    mScreen->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());
    drawAddScore();
}


void daBalloon2D_c::setComboCount(u8 size, u8 comboNum) {
    if (mComboNum != comboNum) {
        setComboNum(comboNum);
    }
    if (mBalloonSize != size) {
        setBalloonSize(size);
    }
}

void daBalloon2D_c::setScoreCount(u32 scoreCount) {
    if (mScoreCount != scoreCount) {
        setScoreNum(scoreCount);
    }
}

void daBalloon2D_c::addScoreCount(cXyz* param_1, u32 param_2, u8 param_3) {
    for (int i = 0; i < 19; i++) {
        int current = 19 - i;
        int prev = current - 1;
        field_0x5f8[current].field_0x0.set(field_0x5f8[prev].field_0x0);
        field_0x5f8[current].field_0xc = field_0x5f8[prev].field_0xc;
        field_0x5f8[current].field_0xe = field_0x5f8[prev].field_0xe;
        field_0x5f8[current].field_0xf = field_0x5f8[prev].field_0xf;
    }
    cXyz acStack_2c;
    mDoLib_project(param_1, &acStack_2c);
    field_0x5f8[0].field_0x0.set(acStack_2c);
    field_0x5f8[0].field_0xc = param_2;
    field_0x5f8[0].field_0xe = 60;
    field_0x5f8[0].field_0xf = param_3;
}

void daBalloon2D_c::initiate() {
}

void daBalloon2D_c::update() {
}

void daBalloon2D_c::setComboNum(u8 comboNum) {
    if (comboNum > 99) {
        comboNum = 99;
    }
    mComboNum = comboNum;
    ResTIMG* resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(comboNum / 10));
    field_0x5a8[5]->changeTexture(resTimg, 0);
    resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(comboNum % 10));
    field_0x5a8[6]->changeTexture(resTimg, 0);
    setComboAlpha();
}

void daBalloon2D_c::setBalloonSize(u8 balloonSize) {
    mBalloonSize = balloonSize;
    switch(balloonSize) {
    case 0:
        field_0x598->show();
        field_0x59c->hide();
        field_0x5a0->hide();
        break;
    case 1:
        field_0x598->hide();
        field_0x59c->show();
        field_0x5a0->hide();
        break;
    case 2:
        field_0x598->hide();
        field_0x59c->hide();
        field_0x5a0->show();
        break;
    }
}

void daBalloon2D_c::setScoreNum(int scoreNum) {
    if (scoreNum > 99999) {
        scoreNum = 99999;
    }
    mScoreCount = scoreNum;
    int digit = scoreNum / 10000;
    int num = scoreNum % 10000;
    ResTIMG* resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(digit));
    field_0x5a8[0]->changeTexture(resTimg, 0);
    digit = num / 1000;
    int num2 = num % 1000;
    resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(digit));
    field_0x5a8[1]->changeTexture(resTimg, 0);
    digit = num2 / 100;
    int num3 = num2 % 100;
    resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(digit));
    field_0x5a8[2]->changeTexture(resTimg, 0);
    digit = num3 / 10;
    num3 %= 10;
    resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(digit));
    field_0x5a8[3]->changeTexture(resTimg, 0);
    resTimg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(num3));
    field_0x5a8[4]->changeTexture(resTimg, 0);
}

void daBalloon2D_c::setAllAlpha() {
    mAllAlpha = field_0x578->getAlphaRate();
    if (isVisible()) {
        if (mAllAlpha != 1.0f) {
            cLib_addCalc2(&mAllAlpha, 1.0f, 0.4f, 0.5f);
            if (fabsf(mAllAlpha - 1.0f) < 0.1f) {
                mAllAlpha = 1.0f;
            }
            field_0x578->setAlphaRate(mAllAlpha);
            field_0x580->setAlphaRate(l_HOSTIO.m.m5DNumberAlpha * mAllAlpha);
            field_0x594->setAlphaRate(l_HOSTIO.m.m2DNumberComboAlpha * mAllAlpha);
            field_0x598->setAlphaRate(l_HOSTIO.m.mBalloonLargeAlpha * mAllAlpha);
            field_0x59c->setAlphaRate(l_HOSTIO.m.mBalloonMedAlpha * mAllAlpha);
            field_0x5a0->setAlphaRate(l_HOSTIO.m.mBalloonSmallAlpha * mAllAlpha);
        }
    } else if (mAllAlpha != 1.0f) {
        cLib_addCalc2(&mAllAlpha, 0.0f, 0.4f, 0.5f);
        if (fabsf(mAllAlpha) < 0.1f) {
            mAllAlpha = 0.0f;
        }
        field_0x578->setAlphaRate(mAllAlpha);
        field_0x580->setAlphaRate(l_HOSTIO.m.m5DNumberAlpha * mAllAlpha);
        field_0x594->setAlphaRate(l_HOSTIO.m.m2DNumberComboAlpha * mAllAlpha);
        field_0x598->setAlphaRate(l_HOSTIO.m.mBalloonLargeAlpha * mAllAlpha);
        field_0x59c->setAlphaRate(l_HOSTIO.m.mBalloonMedAlpha * mAllAlpha);
        field_0x5a0->setAlphaRate(l_HOSTIO.m.mBalloonSmallAlpha * mAllAlpha);
    }
}


void daBalloon2D_c::setComboAlpha() {
    mComboAlpha = field_0x5a4->getAlphaRate() * field_0x578->getAlphaRate();
    if (mComboNum != 0) {
        if (mComboAlpha != 1.0f) {
            cLib_addCalc2(&mComboAlpha, 1.0f, 0.4f, 0.5f);
            if (fabsf(mComboAlpha - 1.0f) < 0.1f) {
                mComboAlpha = 1.0f;
            }
            field_0x5a4->setAlphaRate(mComboAlpha);
        }
    } else if (mComboAlpha != 0.0f) {
        mComboAlpha = 0.0f;
        field_0x5a4->setAlphaRate(mComboAlpha);
    }
}

void daBalloon2D_c::drawAddScore() {
    for (s32 i = 19; i >= 0; i--) {
        if (field_0x5f8[i].field_0xe != 0) {
            field_0x5f8[i].field_0xe--;
            s32 score3;
            s32 score2;
            s32 score = field_0x5f8[i].field_0xc;
            s16 temp0 = (field_0x5f8[i].field_0xe % 60) * 1024;
            u8 local_88 = 0xff;
            f32 dVar11 = 30.0f;
            f32 dVar9 = 30.0f;
            field_0x5f8[i].field_0x0.x += cM_ssin(temp0) * 0.3f;
            field_0x5f8[i].field_0x0.y -= 1.0f;
            if (field_0x5f8[i].field_0xe < 10) {
                f32 fVar5 = field_0x5f8[i].field_0xe / 10.0f;
                local_88 = fVar5 * 255.0f;
                dVar11 *= fVar5;
                dVar9 *= fVar5;
            }
            for (s32 j = 0; j < 10; j++) {
                field_0x5c4[j]->setAlpha(local_88);
            }
            field_0x5ec->setAlpha(local_88);
            f32 dVar8 = (field_0x5f8[i].field_0x0.y - (dVar9 / 2.0f));
            s32 digit;
            if (score >= 10000) {
                f32 dVar10 = (field_0x5f8[i].field_0x0.x -
                                  (dVar11 / 2.0f) * 6.0f);
                field_0x5ec->draw(dVar10, dVar8, dVar11, dVar9, false, false, false);
                digit = score / 10000;
                score = score % 10000;
                field_0x5c4[digit]->draw(dVar10 + dVar11, dVar8, dVar11, dVar9, false, false, false);
                digit = score / 1000;
                score %= 1000;
                field_0x5c4[digit]->draw(dVar10 + dVar11 * 2.0f, dVar8, dVar11, dVar9, false, false, false);
                digit = score / 100;
                score %= 100;
                field_0x5c4[digit]->draw(dVar10 + dVar11 * 3.0f, dVar8, dVar11, dVar9, false, false, false);
                digit = score / 10;
                score %= 10;
                field_0x5c4[digit]->draw(dVar10 + dVar11 * 4.0f, dVar8, dVar11, dVar9, false, false, false);
                field_0x5c4[score]->draw(dVar10 + dVar11 * 5.0f, dVar8, dVar11, dVar9, false, false, false);
            } else if (score >= 1000) {
                f32 dVar10 = (field_0x5f8[i].field_0x0.x -
                                    (dVar11 / 2.0f) * 5.0f);
                field_0x5ec->draw(dVar10, dVar8, dVar11, dVar9, false, false, false);
                digit = score / 1000;
                score2 = score % 1000;
                field_0x5c4[digit]->draw(dVar10 + dVar11, dVar8, dVar11, dVar9, false, false, false);
                digit = score2 / 100;
                score2 %= 100;
                field_0x5c4[digit]->draw(dVar10 + dVar11 * 2.0f, dVar8, dVar11, dVar9, false, false, false);
                digit = score2 / 10;
                score2 %= 10;
                field_0x5c4[digit]->draw(dVar10 + dVar11 * 3.0f, dVar8, dVar11, dVar9, false, false, false);
                field_0x5c4[score2]->draw(dVar10 + dVar11 * 4.0f, dVar8, dVar11, dVar9, false, false, false);
            } else if (score >= 100) {
                f32 dVar10 = (field_0x5f8[i].field_0x0.x -
                                    (dVar11 / 2.0f) * 4.0f);
                field_0x5ec->draw(dVar10, dVar8, dVar11, dVar9, false, false, false);
                digit = score / 100;
                score3 = score % 100;
                field_0x5c4[digit]->draw(dVar10 + dVar11, dVar8, dVar11, dVar9, false, false, false);
                digit = score3 / 10;
                score3 %= 10;
                field_0x5c4[digit]->draw(dVar10 + dVar11 * 2.0f, dVar8, dVar11, dVar9, false, false, false);
                field_0x5c4[score3]->draw(dVar10 + dVar11 * 3.0f, dVar8, dVar11, dVar9, false, false, false);
            } else if (score >= 10) {
                f32 dVar10 = (field_0x5f8[i].field_0x0.x -
                                    (dVar11 / 2.0f) * 3.0f);
                field_0x5ec->draw(dVar10, dVar8, dVar11, dVar9, false, false, false);
                digit = score / 10;
                score3 = score % 10;
                field_0x5c4[digit]->draw(dVar10 + dVar11, dVar8, dVar11, dVar9, false, false, false);
                field_0x5c4[score3]->draw(dVar10 + dVar11 * 2.0f, dVar8, dVar11, dVar9, false, false, false);
            } else {
                f32 dVar10 = (field_0x5f8[i].field_0x0.x -
                                    (dVar11 / 2.0f) * 2.0f);
                field_0x5ec->draw(dVar10, dVar8, dVar11, dVar9, false, false, false);
                field_0x5c4[score]->draw(dVar10 + dVar11, dVar8, dVar11, dVar9, false, false, false);
            }
        }
    }
}

void daBalloon2D_c::setHIO(bool param_1) {
    if ((l_HOSTIO.m.mAdjustmentToggle != 0) || param_1) {
#if DEBUG
        if (l_HOSTIO.m.mOverheadScoreToggle != 0) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            cXyz acStack_28;
            acStack_28.set(player->getHeadTopPos());
            addScoreCount(&acStack_28, l_HOSTIO.m.mOverheadScore, 0);
            l_HOSTIO.resetAddScoreSetFlag();
        }
#endif

        field_0x57c->paneTrans(l_HOSTIO.m.mScorePosX, l_HOSTIO.m.mScorePosY);
        field_0x57c->scale(l_HOSTIO.m.mScoreSize, l_HOSTIO.m.mScoreSize);
        field_0x580->paneTrans(l_HOSTIO.m.m5DNumberPosX, l_HOSTIO.m.m5DNumberPosY);
        field_0x580->scale(l_HOSTIO.m.m5DNumberSizeX, l_HOSTIO.m.m5DNumberSizeY);
        field_0x580->setAlphaRate(l_HOSTIO.m.m5DNumberAlpha * mAllAlpha);
        field_0x584->paneTrans(l_HOSTIO.m.m5DNumber2PosX, l_HOSTIO.m.m5DNumber2PosY);
        field_0x584->scale(l_HOSTIO.m.m5DNumber2Size, l_HOSTIO.m.m5DNumber2Size);
        field_0x588->paneTrans(l_HOSTIO.m.mUnkPosX, l_HOSTIO.m.mUnkPosY);
        field_0x588->scale(l_HOSTIO.m.mUnkSize, l_HOSTIO.m.mUnkSize);
        field_0x58c->paneTrans(l_HOSTIO.m.mComboPosX, l_HOSTIO.m.mComboPosY);
        field_0x58c->scale(l_HOSTIO.m.mComboSize, l_HOSTIO.m.mComboSize);
        field_0x590->paneTrans(l_HOSTIO.m.m2DNumberPosX, l_HOSTIO.m.m2DNumberPosY);
        field_0x590->scale(l_HOSTIO.m.m2DNumberSizeX, l_HOSTIO.m.m2DNumberSizeY);
        field_0x594->paneTrans(l_HOSTIO.m.m2DNumberComboPosX, l_HOSTIO.m.m2DNumberComboPosY);
        field_0x594->scale(l_HOSTIO.m.m2DNumberComboSize, l_HOSTIO.m.m2DNumberComboSize);
        field_0x594->setAlphaRate(l_HOSTIO.m.m2DNumberComboAlpha * mAllAlpha);
        field_0x598->paneTrans(l_HOSTIO.m.mBalloonLargePosX, l_HOSTIO.m.mBalloonLargePosY);
        field_0x598->scale(l_HOSTIO.m.mBalloonLargeSize, l_HOSTIO.m.mBalloonLargeSize);
        field_0x598->setAlphaRate(l_HOSTIO.m.mBalloonLargeAlpha * mAllAlpha);
        field_0x59c->paneTrans(l_HOSTIO.m.mBalloonMedPosX, l_HOSTIO.m.mBalloonMedPosY);
        field_0x59c->scale(l_HOSTIO.m.mBalloonMedSize, l_HOSTIO.m.mBalloonMedSize);
        field_0x59c->setAlphaRate(l_HOSTIO.m.mBalloonMedAlpha * mAllAlpha);
        field_0x5a0->paneTrans(l_HOSTIO.m.mBalloonSmallPosX, l_HOSTIO.m.mBalloonSmallPosY);
        field_0x5a0->scale(l_HOSTIO.m.mBalloonSmallSize, l_HOSTIO.m.mBalloonSmallSize);
        field_0x5a0->setAlphaRate(l_HOSTIO.m.mBalloonSmallAlpha * mAllAlpha);
#if WIDESCREEN_SUPPORT
        if (mDoGph_gInf_c::isWide()) {
            field_0x5a4->paneTrans(l_HOSTIO.m.m2DNumberCombo2PosX, l_HOSTIO.m.m2DNumberCombo2PosY);
        } else {
            field_0x5a4->paneTrans(l_HOSTIO.m.m2DNumberCombo2PosX43, l_HOSTIO.m.m2DNumberCombo2PosY);
        }
#else
        field_0x5a4->paneTrans(l_HOSTIO.m.m2DNumberCombo2PosX, l_HOSTIO.m.m2DNumberCombo2PosY);
#endif
        field_0x5a4->scale(l_HOSTIO.m.m2DNumberCombo2Size, l_HOSTIO.m.m2DNumberCombo2Size);
    }
}

void daBalloon2D_HIO_c::resetAddScoreSetFlag() {
    JORMContext* ctx = attachJORMContext(8);
    ctx->startUpdateNode(this);
    m.mOverheadScoreToggle = 0x0;
    ctx->updateCheckBox(2, &m.mOverheadScoreToggle, 1, 0);
    ctx->endUpdateNode();
    releaseJORMContext(ctx);
}

static int daBalloon2D_create(daBalloon2D_c* i_this) {
    fopAcM_ct(i_this, daBalloon2D_c);
    return i_this->create();
}

daBalloon2D_c::CHeadScore::~CHeadScore() {
}

daBalloon2D_c::CHeadScore::CHeadScore() {
}

static int daBalloon2D_destroy(daBalloon2D_c* i_this) {
    return i_this->destroy();
}

static int daBalloon2D_execute(daBalloon2D_c* i_this) {
    return i_this->execute();
}

static int daBalloon2D_draw(daBalloon2D_c* i_this) {
    return i_this->draw();
}

#if DEBUG
void daBalloon2D_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genCheckBox("調整ＯＮ", &m.mAdjustmentToggle, 1, 0, NULL, 0xffff,
                               0xffff, 0x200, 0x18);
    ctx->genSlider("スコア", &m.mScore, 0, 99999, 0, NULL, 0xffff, 0xffff, 0x200,
                             0x18);
    ctx->genSlider("コンボ数", &m.mComboCount, 0, 99, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->startComboBox("風船のサイズ", &m.mBalloonSize, 0, NULL, 0xffff, 0xffff, 0x100,
                                 0x1a);
    ctx->genComboBoxItem("大", 0); // large
    ctx->genComboBoxItem("中", 1); // medium
    ctx->genComboBoxItem("小", 2); // small
    ctx->endComboBox();
    ctx->genCheckBox("頭上スコアＯＮ", &m.mOverheadScoreToggle, 1, 0, NULL, 0xffff, 0xffff,
                               0x200, 0x18);
    ctx->genSlider("頭上スコア", &m.mOverheadScore, 0, 99999, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genLabel("*****「SCORE」（score_tn）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.mScorePosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.mScorePosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.mScoreSize, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    // 5-digit number
    ctx->genLabel("*****５ケタの数字（suji_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.m5DNumberPosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.m5DNumberPosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズＸ", &m.m5DNumberSizeX, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズＹ", &m.m5DNumberSizeY, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("アルファ", &m.m5DNumberAlpha, 0.0, 1.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    // "SCORE" & 5-digit number
    ctx->genLabel("*****「SCORE」＆５ケタの数字（suji_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.m5DNumber2PosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.m5DNumber2PosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.m5DNumber2Size, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genLabel("*****「＋」（tas_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.mUnkPosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.mUnkPosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.mUnkSize, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genLabel("*****「COMBO」\u3000（combo_tn）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.mComboPosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.mComboPosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.mComboSize, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genLabel("*****２ケタの数字（num_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.m2DNumberPosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.m2DNumberPosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズＸ", &m.m2DNumberSizeX, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズＹ", &m.m2DNumberSizeY, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    // "+" & 2-digit number & "COMBO"
    ctx->genLabel("*****「＋」＆２ケタの数字＆「COMBO」（co_set_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.m2DNumberComboPosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.m2DNumberComboPosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.m2DNumberComboSize, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("アルファ", &m.m2DNumberComboAlpha, 0.0, 1.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    // balloon "large"
    ctx->genLabel("*****風船「大」（bal_1_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.mBalloonLargePosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.mBalloonLargePosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.mBalloonLargeSize, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("アルファ", &m.mBalloonLargeAlpha, 0.0, 1.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    // balloon "medium"
    ctx->genLabel("*****風船「中」（bal_2_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.mBalloonMedPosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.mBalloonMedPosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.mBalloonMedSize, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("アルファ", &m.mBalloonMedAlpha, 0.0, 1.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    // balloon "small"
    ctx->genLabel("*****風船「小」（bal_3_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("位置Ｘ", &m.mBalloonSmallPosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.mBalloonSmallPosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.mBalloonSmallSize, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("アルファ", &m.mBalloonSmallAlpha, 0.0, 1.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    // "Balloon" & "+" & two-digit number & "COMBO"
    ctx->genLabel("*****「風船」＆「＋」＆２ケタの数字＆「COMBO」（ba_com_n）*****", 0, 0, NULL, 0xffff, 0xffff, 0x200,
                            0x18);
    ctx->genSlider("位置Ｘ", &m.m2DNumberCombo2PosX, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｘ(4:3)", &m.m2DNumberCombo2PosX43, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("位置Ｙ", &m.m2DNumberCombo2PosY, -300.0, 300.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
    ctx->genSlider("サイズ", &m.m2DNumberCombo2Size, 0.0, 3.0, 0, NULL, 0xffff, 0xffff,
                             0x200, 0x18);
}
#endif

static actor_method_class daBalloon2D_METHODS = {
    (process_method_func)daBalloon2D_create,
    (process_method_func)daBalloon2D_destroy,
    (process_method_func)daBalloon2D_execute,
    0,
    (process_method_func)daBalloon2D_draw,
};

actor_process_profile_definition g_profile_BALLOON2D = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_BALLOON2D,         // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daBalloon2D_c), // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    765,                    // mPriority
    &daBalloon2D_METHODS,   // sub_method
    0x00040000,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_0_e,      // cullType
  };
