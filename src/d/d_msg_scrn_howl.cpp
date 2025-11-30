//
// Howl Screen
//

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_howl.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_msg_object.h"
#include "d/d_pane_class.h"
#include "d/actor/d_a_alink.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "Z2AudioLib/Z2WolfHowlMgr.h"

typedef void (dMsgScrnHowl_c::*dMsgScrnHowl_cFunc)();

static dMsgScrnHowl_cFunc init_proc[5] = {
    &dMsgScrnHowl_c::guide_on_init,
    &dMsgScrnHowl_c::guide_off_init,
    &dMsgScrnHowl_c::guide_stop_init,
    &dMsgScrnHowl_c::guide_demo_play_init,
    &dMsgScrnHowl_c::guide_off_test_init,
};

static dMsgScrnHowl_cFunc process[5] = {
    &dMsgScrnHowl_c::guide_on_proc,
    &dMsgScrnHowl_c::guide_off_proc,
    &dMsgScrnHowl_c::guide_stop_proc,
    &dMsgScrnHowl_c::guide_demo_play_proc,
    &dMsgScrnHowl_c::guide_off_test_proc,
};

dMsgScrnHowl_c::dMsgScrnHowl_c() {
    static u64 ylinen_tag[3] = {
        'ylinen00',
        'ylinen02',
        'ylinen04',
    };
    static u64 tlinen_tag[7] = {
        'tlinen00', 'tlinen01', 'tlinen02', 'tlinen03', 'tlinen04', 'tlinen05', 'tlinen06',
    };
    static u64 tline_tag[7] = {
        'tline00', 'tline01', 'tline02', 'tline03', 'tlinen04', 'tline05', 'tline06',
    };
    static u64 tlines_tag[7] = {
        'tlines00', 'tlines01', 'tlines02', 'tlines03', 'tlines04', 'tlines05', 'tlines06',
    };

    init();
    mpScreen = new J2DScreen();
    JUT_ASSERT(61, mpScreen != NULL);
    bool fg = mpScreen->setPriority("zelda_wolf_howl.blo", 0x20000, dComIfGp_getMsgArchive(5));
    JUT_ASSERT(73, fg != false);
    dPaneClass_showNullPane(mpScreen);
    mpScreen->search('line00')->hide();
    mpPmP_c = new CPaneMgr(mpScreen, 'n_all', 3, NULL);
    JUT_ASSERT(79, mpPmP_c != NULL);
    mpScreen->search('ag_n')->hide();
    field_0x1994 = 0.0f;
    field_0x1998 = 0.0f;
    mpScreen->search('wi_btn_n')->hide();
    mpButtonIcon[0] = new CPaneMgr(mpScreen, 'cbtn_n', 2, NULL);
    JUT_ASSERT(91, mpButtonIcon[0] != NULL);
    mpButtonText[0] = new CPaneMgr(mpScreen, 'g_ltxt_n', 2, NULL);
    JUT_ASSERT(93, mpButtonText[0] != NULL);
    mpButtonIcon[1] = new CPaneMgr(mpScreen, 'abt_n', 2, NULL);
    JUT_ASSERT(96, mpButtonIcon[1] != NULL);
    mpButtonText[1] = new CPaneMgr(mpScreen, 'gr_txt_n', 2, NULL);
    JUT_ASSERT(98, mpButtonText[1] != NULL);
#if VERSION == VERSION_GCN_JPN
    J2DTextBox* piStack_19c = (J2DTextBox*)mpScreen->search('g_l_info');
    J2DTextBox* piStack_1a0 = (J2DTextBox*)mpScreen->search('g_r_info');
    mpScreen->search('fgr_info')->hide();
    mpScreen->search('fgl_info')->hide();
#else
    J2DTextBox* piStack_19c = (J2DTextBox*)mpScreen->search('fgl_info');
    J2DTextBox* piStack_1a0 = (J2DTextBox*)mpScreen->search('fgr_info');
    mpScreen->search('g_l_info')->hide();
    mpScreen->search('g_r_info')->hide();
#endif
    piStack_19c->setString(0x40, "");
    piStack_19c->setFont(mDoExt_getMesgFont());
    dMeter2Info_getStringKanji(0x4d4, piStack_19c->getStringPtr(), NULL);
    piStack_1a0->setString(0x40, "");
    piStack_1a0->setFont(mDoExt_getMesgFont());
    dMeter2Info_getStringKanji(0x4d5, piStack_1a0->getStringPtr(), NULL);
    field_0x2195 = 0;
    for (int i = 0; i < 0x300; i++) {
        field_0x180[i] = 0.0f;
        field_0xd80[i] = 0.0f;
        field_0x2198[i] = 0;
        field_0x2498[i] = 0;
        field_0x1b14[i] = 0;
    }
    for (int i = 0; i < 3; i++) {
        field_0x1b08[i] = 0.0f;
    }
    field_0x2114 = 0;
    field_0x2118 = 0;
    field_0x211c = 0;
    field_0x2120 = 0;
    for (int i = 0; i < 30; i++) {
        field_0x213a[i] = 0;
        field_0x2158[i] = 0;
        field_0x2176[i] = 0;
        field_0x19a0[i] = 0.0f;
        field_0x1a18[i] = 0.0f;
        field_0x1a90[i] = 0.0f;
    }
    field_0x279b = 0x0;
    field_0x2124 = 0;
    field_0x2128 = 0;
    mPlotTime = 0;
    field_0x212c = 0;
    field_0x212e = 0;
    field_0x2126 = 0;
    field_0x2130 = 0;
    field_0x2132 = 0;
    field_0x17c = 1;
    field_0x279a = 0;
    field_0x199c = 0.0f;
    s16 howl = daAlink_getAlinkActorClass()->checkWindStoneHowl();
    if (howl != 0) {
        field_0x2798 = 3;
    } else {
        field_0x2798 = 0;
    }
    field_0x2799 = field_0x2798;
    (this->*init_proc[field_0x2798])();
    field_0x1980 = g_MsgObject_HIO_c.mHowlHIO.mDrawInterval;
    field_0x2136 = (350.0f / field_0x1980);
    field_0x2138 = 0;
    field_0x1984 = 255.0f / field_0x2136;
    mpABase = new CPaneMgr(mpScreen, 'a_base', 0, NULL);
    JUT_ASSERT(218, mpABase != NULL);
    mpLineAll = new CPaneMgr(mpScreen, 'line_all', 0, NULL);
    JUT_ASSERT(221, mpLineAll != NULL);
    f32 in_f31;
    for (int i = 0; i < 7; i++) {
        J2DPane* piStack_1b4 = mpScreen->search(tlinen_tag[i]);
        if (i == 0) {
            CPaneMgr aCStack_b0;
            Mtx mtx1;
            Vec fStack_120 = mpABase->getGlobalVtx(&mtx1, 0, true, 0);
            Vec fStack_144 = aCStack_b0.getGlobalVtxCenter(piStack_1b4, true, 0);
            in_f31 = fStack_120.x - fStack_144.x;
        }
        f32 f29 = piStack_1b4->getBounds().i.y;
        piStack_1b4->move(g_MsgObject_HIO_c.mHowlHIO.mInputStartPoint +
                              ((in_f31 + piStack_1b4->getBounds().i.x) - 3.0f),
                          f29);
    }
    for (int i = 0; i < 3; i++) {
        mpLineH[i] = new CPaneMgr(mpScreen, ylinen_tag[i], 0, NULL);
        JUT_ASSERT(242, mpLineH[i] != NULL);
    }
    for (int i = 0; i < 7; i++) {
        mpLineV[i] = new CPaneMgr(mpScreen, tlinen_tag[i], 2, NULL);
        JUT_ASSERT(247, mpLineV[i] != NULL);
    }
    Mtx mtx2;
    field_0x128 = mpLineH[0]->getGlobalVtx(&mtx2, 0, true, 0);
    field_0x134 = mpLineH[0]->getGlobalVtx(&mtx2, 3, true, 0);
    field_0x140 = mpLineH[2]->getGlobalVtx(&mtx2, 3, true, 0);
    field_0x14c = mpLineV[0]->getGlobalVtx(&mtx2, 0, true, 0);
    field_0x158 = mpLineV[0]->getGlobalVtx(&mtx2, 3, true, 0);
    field_0x164 = mpLineV[1]->getGlobalVtx(&mtx2, 0, true, 0);
    field_0x170 = mpLineV[1]->getGlobalVtx(&mtx2, 3, true, 0);
    field_0x279c = 0.0f;
    field_0x27a0 = 0.0f;
    field_0x27a4 = 1.0f;
    field_0x27a8 = 0.0f;
    ResTIMG const* res = (ResTIMG const*) dComIfGp_getMsgArchive(5)->getResource('TIMG', "tt_ginnouroko_s3tc.bti");
    mpDot = new J2DPicture(res);
    JUT_ASSERT(275, mpDot != NULL);
    mpDot->setWhite(JUtility::TColor(0xff, 0xff, 0x71, 0xff));
    res = (ResTIMG const*)dComIfGp_getMsgArchive(5)->getResource('TIMG', "tt_black_32.bti");
    mpGuideDot = new J2DPicture(res);
    JUT_ASSERT(280, mpGuideDot != NULL);
    mpGuideDot->setBlackWhite(((J2DPicture*)mpScreen->search('line00'))->getBlack(), ((J2DPicture*)mpScreen->search('line00'))->getWhite());
    res = (ResTIMG const*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_iastarRR.bti");
    mpTopBall = new J2DPicture(res);
    JUT_ASSERT(287, mpTopBall != NULL);
    mpTopBall->setBlackWhite(g_MsgObject_HIO_c.mHowlHIO.mDotBlack, g_MsgObject_HIO_c.mHowlHIO.mDotWhite);
    
    ;
    for (int i = 0; i < 5; i++) {
        mpTopBallTail[i] = new J2DPicture(res);
        JUT_ASSERT(292, mpTopBallTail[i] != NULL);
        mpTopBallTail[i]->setBlackWhite(g_MsgObject_HIO_c.mHowlHIO.mDotBlack, g_MsgObject_HIO_c.mHowlHIO.mDotWhite);
    }
    field_0x2134 = 0;
    field_0x1988 = mpScreen->search('line00')->getWidth();
    field_0x198c = mpScreen->search('line00')->getHeight();
    field_0x2194 = daAlink_getAlinkActorClass()->getCorrectCurveID();
    mCorrectLineMax = 0;
    field_0x2197 = 0;
    res = (ResTIMG const*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpWaveTex = new J2DPicture(res);
    JUT_ASSERT(307, mpWaveTex != NULL);
    mpWaveTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(255, 200, 0, 255));
    mpWaveTex->setCornerColor(
        JUtility::TColor(128, 128, 128, 0), JUtility::TColor(255, 255, 255, 255),
        JUtility::TColor(128, 128, 128, 0), JUtility::TColor(255, 255, 255, 255));
    mpWaveTex->setAlpha(0);
    if (field_0x2194 >= 0) {
        mCorrectLineMax = daAlink_getAlinkActorClass()->getCorrectLineNum();
        JUT_ASSERT(322, mCorrectLineMax < LINE_MAX);
        field_0x2197 = LINE_MAX;
        field_0x2138 = getGuideDataSize();
    }
    resetLine();
}

dMsgScrnHowl_c::~dMsgScrnHowl_c() {
    delete mpScreen;
    mpScreen = NULL;

    delete mpPmP_c;
    mpPmP_c = NULL;

    for (int i = 0; i < 2; i++) {
        delete mpButtonIcon[i];
        mpButtonIcon[i] = NULL;
        delete mpButtonText[i];
        mpButtonText[i] = NULL;
    }
    for (int i = 0; i < 3; i++) {
        delete mpLineH[i];
        mpLineH[i] = NULL;
    }
    for (int i = 0; i < 7; i++) {
        delete mpLineV[i];
        mpLineV[i] = NULL;
    }
    delete mpABase;
    mpABase = NULL;
    delete mpLineAll;
    mpLineAll = NULL;
    delete mpDot;
    mpDot = NULL;
    delete mpGuideDot;
    mpGuideDot = NULL;
    delete mpTopBall;
    mpTopBall = NULL;
    for (int i = 0; i < 5; i++) {
        delete mpTopBallTail[i];
        mpTopBallTail[i] = NULL;
    }
    delete mpWaveTex;
    mpWaveTex = NULL;
    dComIfGp_getMsgArchive(5)->removeResourceAll();;
}

void dMsgScrnHowl_c::exec() {
    field_0x2799 = field_0x2798;
    (this->*process[field_0x2798])();
    for (int i = 0; i < 0x300; i++) {
        if (field_0x1b14[i] > 0) {
            field_0x1b14[i]--;
        }
    }
    if (field_0x2799 != field_0x2798) {
        (this->*init_proc[field_0x2798])();
    }
    
    f32 alphaRate = mpPmP_c->getAlphaRate();
    f32 fVar1;
    f32 fVar2;
    if (field_0x2798 == 3) {
        fVar1 = 0.5f;
        fVar2 = 0.0f;
    } else {
        fVar1 = 1.0f;
        fVar2 = 1.0f;
    }
    if (field_0x1994 != fVar1) {
        cLib_addCalc2(&field_0x1994, fVar1, 0.2f, 1.0f);
        if (fabsf(field_0x1994 - fVar1) < 0.1f) {
            field_0x1994 = fVar1;
        }
    }
    if (field_0x1998 != fVar2) {
        cLib_addCalc2(&field_0x1998, fVar2, 0.2f, 1.0f);
        if (fabsf(field_0x1998 - fVar2) < 0.1f) {
            field_0x1998 = fVar2;
        }
    }
    mpButtonIcon[0]->setAlphaRate(field_0x1994 * alphaRate);
    mpButtonIcon[1]->setAlphaRate(field_0x1994 * alphaRate);
    mpButtonText[0]->setAlphaRate(field_0x1998 * alphaRate);
    mpButtonText[1]->setAlphaRate(field_0x1998 * alphaRate);
}

void dMsgScrnHowl_c::drawSelf() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    GXGetScissor(&field_0x2114, &field_0x2118, &field_0x211c, &field_0x2120);
    if (field_0x2138 != 0) {
        if ((field_0x2798 == 0) || field_0x2798 == 4) {
            drawGuide();
        } else {
            drawGuide2();
        }
    }
    if (field_0x2798 != 3) {
        drawWave();
    }
    grafContext->scissor(field_0x2114, field_0x2118, field_0x211c, field_0x2120);
    grafContext->setScissor();
}

void dMsgScrnHowl_c::guide_on_init() {
}

void dMsgScrnHowl_c::guide_on_proc() {
    if (daAlink_getAlinkActorClass()->getWolfHowlTimer() == 0) {
        if (field_0x2195 == 1) {
            field_0x2195 = 2;
        }
        if (mDoCPd_c::getTrigB(0) != 0 && field_0x2195 == 2) {
            resetLine();
        }
    } else {
        if (field_0x2195 == 2) {
            resetLine();
        }
        field_0x2195 = 1;
        calcMain();
    }
}

void dMsgScrnHowl_c::guide_off_init() {
}

void dMsgScrnHowl_c::guide_off_proc() {
    calcMain();
    if (daAlink_getAlinkActorClass()->getWolfHowlMgrP()->getReleaseTimer() != 30) {
        if (daAlink_getAlinkActorClass()->getWolfHowlTimer() != 0) {
            return;
        }
    }
    field_0x2798 = 2;
    resetLine();
}

void dMsgScrnHowl_c::guide_stop_init() {
}

void dMsgScrnHowl_c::guide_stop_proc() {
    field_0x180[field_0x2124] =
        getNowPlotPitch(-daAlink_getAlinkActorClass()->getWolfHowlMgrP()->getNowInputValue());
    if (mDoCPd_c::getHoldA(0) != 0) {
        resetLine();
        field_0x2798 = 1;
    } else {
        if (mDoCPd_c::getTrigB(0) != 0) {
            resetLine();
            field_0x2798 = 3;
        }
    }
}

void dMsgScrnHowl_c::guide_demo_play_init() {
    field_0x279a = 0;
}

void dMsgScrnHowl_c::guide_demo_play_proc() {
    if (mDoCPd_c::getHoldA(0) != 0) {
        if (daAlink_getAlinkActorClass()->getWolfHowlTimer() != 0) {
            resetLine();
            field_0x2798 = 1;
        }
    }
    calcMain();
    bool startGuideMelody = false;
    if (mPlotTime == 30) {
        startGuideMelody = true;
    }
    field_0x17c = daAlink_getAlinkActorClass()->getWolfHowlMgrP()->startGuideMelody(startGuideMelody);
    if (mPlotTime >= 30) {
        if (field_0x17c == 0) {
            if (field_0x279a != 0) {
                resetLine();
                field_0x279a = 0;
            }
        } else if (field_0x279a == 0) {
            field_0x279a = 1;
        }
    }
}

void dMsgScrnHowl_c::guide_off_test_init() {
}

void dMsgScrnHowl_c::guide_off_test_proc() {
}

void dMsgScrnHowl_c::fukiScale(f32 param_0) {
}

void dMsgScrnHowl_c::fukiTrans(f32 param_0, f32 param_1) {
    mpPmP_c->paneTrans(param_0, param_1);
}

void dMsgScrnHowl_c::fukiAlpha(f32 param_0) {
    mpPmP_c->setAlphaRate(param_0);
}

bool dMsgScrnHowl_c::isKeyCheck() {
    if (daAlink_getAlinkActorClass()->getWolfHowlTimer() == 0 && field_0x2799 == field_0x2798 &&
        (field_0x2798 == 3 || (field_0x2798 == 0 && field_0x2195 == 0)))
    {
        return true;
    }
    return false;
}

void dMsgScrnHowl_c::resetLine() {
    field_0x2195 = 0;
    field_0x2128 = 0;
    mPlotTime = 0;
    field_0x212c = 0;
    field_0x212e = 0;
    field_0x2124 = 0;
    field_0x2130 = 0;
    field_0x2126 = 0;
    field_0x279b = 0;
    for (int i = 0; i < 30; i++) {
        field_0x2158[i] = 0;
        field_0x2176[i] = 0;
        field_0x19a0[i] = 0.0f;
        field_0x1a18[i] = 0.0f;
        field_0x1a90[i] = 0.0f;
    }
    if (field_0x2138 != 0) {
        initGuideData();
    }
    moveLineV(true);
    moveBaseLength(true);
    field_0x2138 = getGuideDataSize();
}

void dMsgScrnHowl_c::drawWave() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    f32 local_e0 = mpLineH[0]->getGlobalPosX();
    s16 sVar14 = field_0x2124;
    f32 local_60 = 0.0f;
    f32 local_64 = 0.0f;
    f32 local_68 = 0.0f;
    f32 f26 = local_e0;
    f32 local_e4 = field_0x180[sVar14];
    s32 uVar6 = g_MsgObject_HIO_c.mHowlHIO.mWaveformDotLen;
    s32 local_80 = field_0x2128 - 1 - uVar6;
    f32 fVar4 =  (30 - daAlink_getAlinkActorClass()->getWolfHowlMgrP()->getReleaseTimer()) / 30.0f;
    if (local_80 < 0) {
        local_80 = 0;
    }
    s32 local_94 = 0;
    Vec fVar12 = field_0x128;
    Vec this_02 = field_0x140;
    f32 fVar1 = mDoGph_gInf_c::getWidthF() / FB_WIDTH;
    f32 fVar2 = mDoGph_gInf_c::getHeightF() / FB_HEIGHT;
    grafContext->scissor(
        (fVar12.x - mDoGph_gInf_c::getMinXF()) / fVar1 - 16.0f,
        (fVar12.y / fVar2) / fVar2 - 16.0f,
        32.0f + (this_02.x - fVar12.x) / fVar1,
        32.0f + ((this_02.y - fVar12.y) + 2.0f)
        );
    grafContext->setScissor();
    bool bVar5 = true;
    if (field_0x2798 == 0) {
        if (mPlotTime != field_0x212c) {
            field_0x212c = mPlotTime;
        } else {
            bVar5 = false;
        }
    }
    if (bVar5) {
        for (int iVar10 = 0; iVar10 < field_0x2128 - 1; iVar10++) {
            f32 local_54 = local_e0;
            f32 local_c8 = field_0x180[sVar14];
            s32 sVar3 = field_0x1b14[sVar14];
            local_e0 += field_0x1980;
            local_68 += field_0x1984;
            if (local_68 > 255.0f) {
                local_68 = 255.0f;
            }
            sVar14 = addCount(sVar14);
            f32 fVar2 = field_0x180[sVar14];
            s16 temp_r4 = field_0x1b14[sVar14];
            if (iVar10 == field_0x2128 - 2) {
                local_60 = local_e0;
                local_64 = fVar2;
                local_94 = temp_r4;
            }
            if (mPlotTime >= 30 && iVar10 >= local_80 && iVar10 >= 30) {
                f32 local_b0 = 0.0f;
                if (sVar3 > 0) {
                    local_b0 = (f32)sVar3 / (f32)uVar6;
                }
                if (fVar4 > 0.0f && local_b0 > 0.0f) {
                    f32 fVar7 = 0.1f + 0.2f * local_b0;
                    f32 fVar8 = 32.0f * g_MsgObject_HIO_c.mHowlHIO.mDotScale * fVar7;
                    f32 fVar9 = 32.0f * g_MsgObject_HIO_c.mHowlHIO.mDotScale * fVar7;
                    u8 cVar9 = 255.0f * local_b0 * fVar4;
                    if (cVar9 != 0) {
                        f32 local_c4 = local_54;
                        f32 dVar16 = 5.0f;
                        f32 f19 = (local_e0 - local_54) / 5.0f;
                        f32 f17 = (fVar2 - local_c8) / 5.0f;
                        for (int iVar15 = 0; iVar15 < 5; iVar15++) {
                            mpTopBallTail[iVar15]->setAlpha(cVar9);
                            mpTopBallTail[iVar15]->draw(local_c4 - fVar8 / 2,
                                                        local_c8 - fVar9 / 2, fVar8, fVar9,
                                                        false, false, false);
                            local_c4 += f19;
                            local_c8 += f17;
                        }
                    }
                }
            }
            local_54 = fVar1;
        }
        if (fVar4 > 0.0f) {
            f32 fVar1 = 32.0f * g_MsgObject_HIO_c.mHowlHIO.mDotScale;
            f32 fVar2 = 32.0f * g_MsgObject_HIO_c.mHowlHIO.mDotScale;
            f32 local_dc = 0.0f;
            f32 f17;
            f32 f18;
            if (field_0x2128 != 0) {
                if (local_94 > 0) {
                    local_dc = (f32)local_94 / (f32)uVar6;
                }
                f17 = local_60;
                f18 = local_64;
            } else {
                field_0x2134++;
                if (field_0x2134 > 30) {
                    field_0x2134 = 0;
                }
                if (field_0x2134 < 15) {
                    local_dc = field_0x2134 / 15.0f;
                } else {
                    local_dc = (30.0f - field_0x2134) / 15.0f;
                }
                f17 = f26;
                f18 = local_e4;
            }
            if (local_dc > 0.0f) {
                mpTopBall->setAlpha(255.0f * local_dc * fVar4);
                mpTopBall->draw(
                    f17 - fVar1 / 2,
                    f18 - fVar2 / 2, fVar1,
                    fVar2, false, false, false);
            }
        }
    }
}

void dMsgScrnHowl_c::drawGuide() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    Vec local_b0 = field_0x128;
    Vec local_bc = field_0x140;
    grafContext->scissor(
        (local_b0.x - mDoGph_gInf_c::getMinXF()) / (mDoGph_gInf_c::getWidthF() / FB_WIDTH),
        field_0x2118, (local_bc.x - local_b0.x) / (mDoGph_gInf_c::getWidthF() / FB_WIDTH),
        field_0x2120);
    grafContext->setScissor();
    f32 local_cc = mpLineH[0]->getGlobalPosX();
    s16 sVar12 = 0;
    f32 local_d0 = 0.0f;
    f32 local_d4 = 0.0f;
    f32 local_d8 = 0.0f;
    f32 local_dc = 0.0f;
    for (int i = 0; i < 0x15d; i++) {
        f32 local_e0 = local_cc;
        f32 local_e4 = field_0xd80[sVar12];
        int dVar4 = field_0x2198[sVar12];
        local_cc += field_0x1980;
        sVar12 = addCountGuide(sVar12);
        f32 local_e8 = local_cc;
        f32 local_ec = field_0xd80[sVar12];
        if (local_e4 > 0.0f && local_ec > 0.0f && local_e4 == local_ec) {
            f32 local_f0 = 0.5f * (local_e0 + local_cc);
            f32 local_f4 = local_e4;
            f32 local_f8 = field_0x1988 * g_MsgObject_HIO_c.mHowlHIO.mLineThickness;
            f32 local_fc = field_0x198c * g_MsgObject_HIO_c.mHowlHIO.mLineThickness;
            int iVar15 = getOnLineNum(field_0x212e + i - 1);
            f32 dVar16 = 0.0f;
            if (iVar15 >= 0 && field_0x213a[iVar15] != 0 && field_0x2158[iVar15] != 0) {
                dVar16 = ((f32)field_0x2158[iVar15] / (f32)field_0x213a[iVar15]);
                if (dVar16 > 1.0f) {
                    dVar16 = 1.0f;
                }
                JUtility::TColor local_100;
                JUtility::TColor local_104;
                JUtility::TColor local_108;
                JUtility::TColor local_10c;
                JUtility::TColor local_110;
                JUtility::TColor TStack_114;
                local_10c.set(0xff, 0xff, 0xff, 0xff);
                TStack_114.set(0xff, 0xff, 0xff, 0xff);
                f32 local_118;
                f32 local_11c;
                switch(dVar4) {
                case 1:
                    local_108.set(g_MsgObject_HIO_c.mHowlHIO.mGuideUpperBlack);
                    local_110.set(g_MsgObject_HIO_c.mHowlHIO.mAnswerUpperBlack);
                    local_118 = g_MsgObject_HIO_c.mHowlHIO.mGuideUpperAlpha;
                    local_11c = g_MsgObject_HIO_c.mHowlHIO.mAnswerUpperAlpha;
                    break;
                case 2:
                    local_108.set(g_MsgObject_HIO_c.mHowlHIO.mGuideMidBlack);
                    local_110.set(g_MsgObject_HIO_c.mHowlHIO.mAnswerMidBlack);
                    local_118 = g_MsgObject_HIO_c.mHowlHIO.mGuideMidAlpha;
                    local_11c = g_MsgObject_HIO_c.mHowlHIO.mAnswerMidAlpha;
                    break;
                case 3:
                    local_108.set(g_MsgObject_HIO_c.mHowlHIO.mGuideLowerBlack);
                    local_110.set(g_MsgObject_HIO_c.mHowlHIO.mAnswerLowerBlack);
                    local_118 = g_MsgObject_HIO_c.mHowlHIO.mGuideLowerAlpha;
                    local_11c = g_MsgObject_HIO_c.mHowlHIO.mAnswerLowerAlpha;
                    break;
                }

                local_100.r = local_108.r + dVar16 * (local_110.r - local_108.r);
                local_100.g = local_108.g + dVar16 * (local_110.g - local_108.g);
                local_100.b = local_108.b + dVar16 * (local_110.b - local_108.b);
                local_100.a = 0;
                local_104.set(0xff, 0xff, 0xff, 0xff);
                mpGuideDot->setBlackWhite(local_100, local_104);
                f32 fVar3;
                if (field_0x2798 == 4) {
                    local_100.r = local_108.r * dVar16;
                    local_100.g = local_108.g * dVar16;
                    local_100.b = local_108.b * dVar16;
                    local_100.a = local_108.a * dVar16;
                    local_104.r = local_10c.r * dVar16;
                    local_104.g = local_10c.g * dVar16;
                    local_104.b = local_10c.b * dVar16;
                    local_104.a = local_10c.a * dVar16;
                    dVar16 = local_118 * dVar16;
                } else {
                    dVar16 = local_118 + (dVar16 * (local_11c - local_118));
                }
            } else {
                switch (dVar4) {
                case 1:
                    mpGuideDot->setBlackWhite(g_MsgObject_HIO_c.mHowlHIO.mGuideUpperBlack,
                                              g_MsgObject_HIO_c.mHowlHIO.mGuideUpperWhite);
                    dVar16 = g_MsgObject_HIO_c.mHowlHIO.mGuideUpperAlpha;
                    break;
                case 2:
                    mpGuideDot->setBlackWhite(g_MsgObject_HIO_c.mHowlHIO.mGuideMidBlack,
                                              g_MsgObject_HIO_c.mHowlHIO.mGuideMidWhite);
                    dVar16 = g_MsgObject_HIO_c.mHowlHIO.mGuideMidAlpha;
                    break;
                case 3:
                    mpGuideDot->setBlackWhite(g_MsgObject_HIO_c.mHowlHIO.mGuideLowerBlack,
                                              g_MsgObject_HIO_c.mHowlHIO.mGuideLowerWhite);
                    dVar16 = g_MsgObject_HIO_c.mHowlHIO.mGuideLowerAlpha;
                    break;
                }
                if (field_0x2798 == 4) {
                    dVar16 = 0.0f;
                }
            }
            if (dVar16 > 0.0f) {
                mpGuideDot->setAlpha(
                    dVar16 * (mpScreen->search('line00')->getAlpha() * mpPmP_c->getAlphaRate()));
                mpGuideDot->draw((2.0f + (local_f0 - local_f8 / 2)), (local_f4 - local_fc / 2),
                                 local_f8, local_fc, false, false, false);
            }
        }
        if (i == 0) {
            local_d0 = local_e0;
            local_d4 = local_e4;
        }
        if (i == field_0x2128 - 2) {
            local_d8 = local_e8;
            local_dc = local_ec;
        }
    }
}

void dMsgScrnHowl_c::drawGuide2() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    if (field_0x2798 == 3) {
        drawEffect();
    } else {
        field_0x199c = 0.0f;
    }
    Vec local_58 = field_0x128;
    Vec local_64 = field_0x140;
    f32 local_70 = mDoGph_gInf_c::getHeightF() / FB_HEIGHT;
    grafContext->scissor(
        (local_58.x - mDoGph_gInf_c::getMinXF()) / (mDoGph_gInf_c::getWidthF() / FB_WIDTH),
        field_0x2118, (local_64.x - local_58.x) / (mDoGph_gInf_c::getWidthF() / FB_WIDTH),
        field_0x2120);
    grafContext->setScissor();
    f32 local_74 = mpLineH[0]->getGlobalPosX();
    s16 local_134 = 0;
    f32 local_78 = 0.0f;
    f32 local_7c = 0.0f;
    f32 local_80 = 0.0f;
    f32 local_84 = 0.0f;
    int local_88;
    if (mPlotTime < field_0x2136) {
        local_88 = 0;
    } else {
        local_88 = mPlotTime - field_0x2136;
    }
    for (int i = 0; i < 0x15d; i++) {
        f32 local_8c = local_74;
        f32 local_90 = field_0xd80[local_134];
        u8 dVar8 = field_0x2198[local_134];
        u8 dVar2 = field_0x2498[i + local_88];
        f32 local_94 = 1.0f;
        local_74 += field_0x1980;
        local_134 = addCountGuide(local_134);
        f32 local_98 = local_74;
        f32 local_9c = field_0xd80[local_134];
        if (local_90 > 0.0f && local_9c > 0.0f && local_90 == local_9c) {
            f32 local_a0 = (local_8c + local_74) * 0.5f;
            f32 local_a4 = local_90;
            f32 local_a8 = field_0x1988 * g_MsgObject_HIO_c.mHowlHIO.mLineThickness;
            f32 local_ac = field_0x198c * g_MsgObject_HIO_c.mHowlHIO.mLineThickness;
            int local_b0 = getOnLineNum(field_0x212e + i - 1);
            f32 guideAlpha = 0.0f;
            if (local_b0 >= 0 && dVar2 != 0) {
                JUtility::TColor local_b4;
                JUtility::TColor local_b8;
                switch (dVar8) {
                case 1:
                    local_b4.set(g_MsgObject_HIO_c.mHowlHIO.mGuideUpperBlack);
                    local_b8.set(g_MsgObject_HIO_c.mHowlHIO.mGuideUpperWhite);
                    guideAlpha = g_MsgObject_HIO_c.mHowlHIO.mGuideUpperAlpha;
                    break;
                case 2:
                    local_b4.set(g_MsgObject_HIO_c.mHowlHIO.mGuideMidBlack);
                    local_b8.set(g_MsgObject_HIO_c.mHowlHIO.mGuideMidWhite);
                    guideAlpha = g_MsgObject_HIO_c.mHowlHIO.mGuideMidAlpha;
                    break;
                case 3:
                    local_b4.set(g_MsgObject_HIO_c.mHowlHIO.mGuideLowerBlack);
                    local_b8.set(g_MsgObject_HIO_c.mHowlHIO.mGuideLowerWhite);
                    guideAlpha = g_MsgObject_HIO_c.mHowlHIO.mGuideLowerAlpha;
                    break;
                }

                mpGuideDot->setBlackWhite(local_b4, local_b8);
            } else {
                guideAlpha = 0.0f;
            }
            if (field_0x2798 == 3 && mPlotTime > 0 && mPlotTime == local_88 + i) {
                u8 onlineNum = getOnLineNum(mPlotTime);
                if (onlineNum == field_0x279b && field_0x2176[field_0x279b] == 0) {
                    field_0x2176[field_0x279b] = 1;
                    field_0x19a0[field_0x279b] = local_a0;
                    field_0x1a18[field_0x279b] = local_a4;
                    field_0x1a90[field_0x279b] = 18.0f - g_MsgObject_HIO_c.mHowlHIO.mAnmSpeed;
                    field_0x279b++;
                }
            }
            if (guideAlpha > 0.0f && local_94 > 0.0f) {
                mpGuideDot->setAlpha(local_94 *
                                     (guideAlpha * (mpScreen->search('line00')->getAlpha() *
                                                    mpPmP_c->getAlphaRate())));
                mpGuideDot->draw(2.0f + (local_a0 - local_a8 / 2), local_a4 - local_ac / 2,
                                 local_a8, local_ac, false, false, false);
            }
        }
        if (i == 0) {
            local_78 = local_8c;
            local_7c = local_90;
        }
        if (i == field_0x2128 - 2) {
            local_80 = local_98;
            local_84 = local_9c;
        }
    }
    for (int i = 0; i < 30; i++) {
        if (field_0x1a90[i] > 0.0f) {
            dMeter2Info_getMeterClass()->getMeterDrawPtr()->drawPikari(
                field_0x19a0[i], field_0x1a18[i], &field_0x1a90[i],
                g_MsgObject_HIO_c.mHowlHIO.mHighlightScale,
                g_MsgObject_HIO_c.mHowlHIO.mHighlightMoyaR1,
                g_MsgObject_HIO_c.mHowlHIO.mHighlightMoyaR0,
                g_MsgObject_HIO_c.mHowlHIO.mHighlightMoyabsR1,
                g_MsgObject_HIO_c.mHowlHIO.mHighlightMoyabsR0, g_MsgObject_HIO_c.mHowlHIO.mAnmSpeed,
                5);
        }
    }
}

void dMsgScrnHowl_c::drawEffect() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    Vec vec1 = field_0x128;
    Vec vec2 = field_0x140;
    mDoGph_gInf_c::getHeightF();
    grafContext->scissor(
        (vec1.x - mDoGph_gInf_c::getMinXF()) / (mDoGph_gInf_c::getWidthF() / FB_WIDTH), field_0x2118,
        12.0f + ((vec2.x - vec1.x) / (mDoGph_gInf_c::getWidthF() / FB_WIDTH)), field_0x2120);
    grafContext->setScissor();
    u8 timer = daAlink_getAlinkActorClass()->getWolfHowlMgrP()->getReleaseTimer();
    u8 screenAlpha = mpScreen->search('line00')->getAlpha();
    mpWaveTex->setAlpha((screenAlpha * mpPmP_c->getAlphaRate()) * ((30 - timer) / 30.0f));
    f32 fVar2 = field_0x2128 * field_0x1980;
    f32 fVar3 = mpLineH[0]->getGlobalPosX() - field_0x27a8;
    field_0x199c = fVar3 + fVar2;
    mpWaveTex->draw(field_0x199c - 50.0f, mpLineH[0]->getGlobalPosY(), 50.0f,
                    (mpLineH[2]->getGlobalPosY() + mpLineH[2]->getSizeY()) -
                        mpLineH[0]->getGlobalPosY(),
                    false, false, false);
}

void dMsgScrnHowl_c::calcMain() {
    if (mPlotTime < field_0x2138 + 380) {
        mPlotTime++;
    } else {
        field_0x212c--;
    }
    calcWave();
    calcGuide();
}

void dMsgScrnHowl_c::calcWave() {
    if (field_0x2128 < field_0x2136) {
        field_0x2132 = field_0x2128;
        field_0x2128++;
        moveBaseLength(true);
    } else {
        if (field_0x212e < field_0x2138 + 380) {
            field_0x212e++;
        }
        field_0x2132 = field_0x2124;
        field_0x2124 = addCount(field_0x2124);
        moveLineV(false);
        moveBaseLength(false);
        for (int iVar5 = 0; iVar5 < 30; iVar5++) {
            if (field_0x1a90[iVar5] > 0.0f) {
                field_0x19a0[iVar5] -= field_0x1980;
            }
        }
    }
    if (field_0x2798 != 3) {
        s8 i_onNum = daAlink_getAlinkActorClass()->getOnLineNum();
        if (i_onNum >= 0 && mPlotTime >= 30 && daAlink_getAlinkActorClass()->getWolfHowlTimer() != 0) {
            JUT_ASSERT(1439, i_onNum < LINE_MAX);
            JUT_ASSERT(1439, mPlotTime <= PLOT_BUFFER_MAX_e);
            field_0x2158[i_onNum]++;
            field_0x2498[mPlotTime] = 1;
        } else {
            field_0x2498[mPlotTime] = 0;
        }
    }

    field_0x180[field_0x2132] = getNowPlotPitch(-daAlink_getAlinkActorClass()->getWolfHowlMgrP()->getNowInputValue());
    field_0x1b14[field_0x2132] = g_MsgObject_HIO_c.mHowlHIO.mWaveformDotLen;
}

void dMsgScrnHowl_c::calcGuide() {
    if (field_0x2138 != 0) {
        if ((daAlink_getAlinkActorClass()->getWolfHowlTimer() != 0) || (field_0x2798 == 3)) {
            calcPitchLevel();
            for (int i = 0; i < 350; i++) {
                if (field_0x2130 < field_0x2136) {
                    if (i < field_0x2138) {
                        u8 dataType = getGuideDataType(i);
                        field_0xd80[i] = getPlotPitch(dataType);
                        field_0x2198[i] = dataType;
                    } else {
                        field_0xd80[i] = 0.0f;
                        field_0x2198[i] = 0;
                    }
                } else if (i + (field_0x2130 - field_0x2136) < field_0x2138) {
                    u8 dataType = getGuideDataType(i + (field_0x2130 - field_0x2136));
                    field_0xd80[i] = getPlotPitch(dataType);
                    field_0x2198[i] = dataType;
                } else {
                    field_0xd80[i] = 0.0f;
                    field_0x2198[i] = 0;
                }
            }
            if (field_0x2130 < (field_0x2138 + field_0x2136)) {
                field_0x2130++;
            }
        }
    }
}

void dMsgScrnHowl_c::moveLineV(bool param_0) {
    f32 dVar7 = (mpLineH[0]->getInitPosX() - mpLineV[0]->getInitCenterPosX());
    f32 fVar1;
    if (param_0) {
        fVar1 = 0.0f;
    } else {
        fVar1 = (mpLineV[0]->getCenterPosX() - mpLineV[0]->getInitCenterPosX());
        fVar1 -= field_0x1980;
    }
    f32 dVar8 = (mpLineV[0]->getInitCenterPosX() - mpLineV[1]->getInitCenterPosX()) - g_MsgObject_HIO_c.mHowlHIO.mLineSpaceOffset;
    if (fVar1 < dVar7) {
        fVar1 -= dVar8;
    }
    if (fVar1 < (dVar7 - (dVar8 / 2))) {
        f32 fVar4 = ((dVar7 - (dVar8 / 2)) - fVar1);
        dVar7 = fVar4 / (-dVar8 / 2);
        mpLineV[0]->setAlphaRate(1.0f - dVar7);
        mpLineV[6]->setAlphaRate(1.0f);
    } else {
        f32 fVar4 = fVar1 - (dVar7 - (dVar8 / 2));
        dVar7 = fVar4 / (-dVar8 / 2);
        mpLineV[0]->setAlphaRate(1.0f);
        mpLineV[6]->setAlphaRate((1.0f - dVar7));
    }
    f32 fVar3 = 0.0f;
    for (int i = 0; i < 7; i++) {
        if (0 < i && i < 6) {
            mpLineV[i]->setAlphaRate(1.0f);
        }
        mpLineV[i]->move((fVar3 + (fVar1 + mpLineV[i]->getInitPosX())), mpLineV[i]->getInitPosY());
        fVar3 += g_MsgObject_HIO_c.mHowlHIO.mLineSpaceOffset;
    }
}

void dMsgScrnHowl_c::moveBaseLength(bool param_0) {
    if (param_0) {
        field_0x1990 = 0.0f;
    } else {
        field_0x1990 -= field_0x1980;
    }
    f32 dVar6 = mpLineH[0]->getGlobalPosX();
    f32 dVar8 = mpABase->getGlobalPosX() + mpABase->getSizeX();
    f32 dVar7 = field_0x1990 + (mpABase->getInitGlobalPosX() - field_0x27a8);
    if (dVar7 < dVar6) {
        dVar7 = dVar6;
    }
    f32 dVar9 = dVar7 - mpABase->getInitGlobalPosX();
    mpABase->resize((dVar8 - dVar7), mpABase->getSizeY());
    mpABase->move(dVar9 + mpABase->getInitPosX(), mpABase->getPosY());
}

s8 dMsgScrnHowl_c::getOnLineNum(int param_0) {
    if (param_0 < 0) {
        return -1;
    }

    if (param_0 >= 30 && param_0 < field_0x2138 + 380) {
        int iVar1 = 30;
        for (int i = 0; i < mCorrectLineMax; i++) {
            iVar1 += field_0x213a[i];
            if (param_0 < iVar1) {
                return i;
            }
        }
    }
    return -1;
}

s16 dMsgScrnHowl_c::addCount(s16 param_0) {
    if (++param_0 >= field_0x2136) {
        param_0 -= field_0x2136;
    }
    return param_0;
}

s16 dMsgScrnHowl_c::addCountGuide(s16 param_0) {
    if (++param_0 >= 350) {
        param_0 -= 350;
    }
    return param_0;
}

f32 dMsgScrnHowl_c::getNowPlotPitch(f32 param_0) {
    if (param_0 < 0.0f) {
        param_0 *= -1.0f;
        return mpLineH[1]->getGlobalCenterPosY() + (param_0 * (mpLineH[0]->getGlobalCenterPosY() - mpLineH[1]->getGlobalCenterPosY()));
    }
    return mpLineH[1]->getGlobalCenterPosY() + (param_0 * (mpLineH[2]->getGlobalCenterPosY() -  mpLineH[1]->getGlobalCenterPosY()));
}

f32 dMsgScrnHowl_c::getPlotPitch(f32 param_0) {
    if (1.0f == param_0) {
        return field_0x1b08[0];
    }
    if (2.0f == param_0) {
        return field_0x1b08[1];
    }
    if (3.0f == param_0) {
        return field_0x1b08[2];
    }
    return 0.0f;
}

void dMsgScrnHowl_c::calcPitchLevel() {
    field_0x1b08[0] = mpLineH[0]->getGlobalCenterPosY();
    field_0x1b08[1] = mpLineH[1]->getGlobalCenterPosY();
    field_0x1b08[2] = mpLineH[2]->getGlobalCenterPosY();
}

void dMsgScrnHowl_c::initGuideData() {
    calcPitchLevel();
    for (int i = 0; i < 350; i++) {
        if (field_0x2138 <= 350) {
            if (i < field_0x2138) {
                u8 dataType = getGuideDataType(i);
                field_0xd80[i] = getPlotPitch(dataType);
                field_0x2198[i] = dataType;
            } else {
                field_0xd80[i] = 0.0f;
                field_0x2198[i] = 0;
            }
        } else {
            u8 dataType = getGuideDataType(i);
            field_0xd80[i] = getPlotPitch(dataType);
            field_0x2198[i] = dataType;
        }
    }
}

u16 dMsgScrnHowl_c::getGuideDataSize() {
    u16 dataSize = field_0x2197;
    for (int i = 0; i < mCorrectLineMax; i++) {
        field_0x213a[i] = daAlink_getAlinkActorClass()->getCorrectLine(i).length;
        dataSize += field_0x213a[i];
    }
    return dataSize;
}

u8 dMsgScrnHowl_c::getGuideDataType(int param_0) {
    u16 uVar2 = field_0x2197;
    u16 uVar3 = field_0x2197;
    for (int i = 0; i < mCorrectLineMax; i++) {
        uVar3 += daAlink_getAlinkActorClass()->getCorrectLine(i).length;
        if (uVar2 <= param_0 && uVar3 > param_0) {
            return daAlink_getAlinkActorClass()->getCorrectLine(i).type;
        }
        uVar2 = uVar3;
    }
    return 0;
}
