/**
 * d_meter2_draw.cpp
 * Main HUD drawing
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_meter2_draw.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/actor/d_a_alink.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "d/d_kantera_icon_meter.h"
#include "d/d_meter_HIO.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_class.h"
#include "d/d_msg_object.h"
#include "d/d_pane_class.h"
#include "cstring.h"

dMeter2Draw_c::dMeter2Draw_c(JKRExpHeap* mp_heap) {
    OS_REPORT("enter dMeter2Draw_c::dMeter2Draw_c(JKRExpHeap *mp_heap)\n");

    heap = mp_heap;
    JKRHeap* heap = mDoExt_setCurrentHeap(dComIfGp_getMsgExpHeap());

    void* msg_res = JKRGetTypeResource('ROOT', "zel_00.bmg", dComIfGp_getMsgDtArchive(0));
    dMeter2Info_setMsgResource(msg_res);

    static const char* bmg_filename[] = {
        "zel_00.bmg", "zel_01.bmg", "zel_02.bmg", "zel_03.bmg", "zel_04.bmg",
        "zel_05.bmg", "zel_06.bmg", "zel_07.bmg", "zel_08.bmg", "zel_99.bmg",
    };

    void* stg_msg_res = JKRGetTypeResource('ROOT', bmg_filename[dMsgObject_getGroupID()],
                                           dComIfGp_getMsgDtArchive(1));
    dMeter2Info_setStageMsgResource(stg_msg_res);

    void* msg_unit_res = JKRGetTypeResource('ROOT', "zel_unit.bmg", dComIfGp_getMsgDtArchive(0));
    dMeter2Info_setMsgUnitResource(msg_unit_res);
    mDoExt_setCurrentHeap(heap);

    mpScreen = new J2DScreen();
    JUT_ASSERT(0, mpScreen != NULL);
    bool fg = mpScreen->setPriority("zelda_game_image.blo", 0x20000, dComIfGp_getMain2DArchive());
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpScreen);

    mpScreen->search('ju_ring5')->hide();
    field_0x73c = 0.0f;

    for (int i = 0; i < 2; i++) {
        mpKanteraMeter[i] = new dKantera_icon_c();
        JUT_ASSERT(0, mpKanteraMeter[i] != NULL);
    }

    mpKanteraScreen = new J2DScreen();
    JUT_ASSERT(0, mpKanteraScreen != NULL);
    fg = mpKanteraScreen->setPriority("zelda_game_image_kantera.blo", 0x20000,
                                      dComIfGp_getMain2DArchive());
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpKanteraScreen);

    mpPikariScreen = new J2DScreen();
    JUT_ASSERT(0, mpPikariScreen != NULL);
    fg = mpPikariScreen->setPriority("zelda_icon_pikari.blo", 0x20000, dComIfGp_getMain2DArchive());
    JUT_ASSERT(0, fg != false);
    dPaneClass_showNullPane(mpPikariScreen);

    mpPikariParent = new CPaneMgr(mpPikariScreen, 'pika_n', 0, NULL);
    JUT_ASSERT(0, mpPikariParent != NULL);
    mPikariBlack = static_cast<J2DPicture*>(mpPikariScreen->search('pika00'))->getBlack();
    mPikariWhite = static_cast<J2DPicture*>(mpPikariScreen->search('pika00'))->getWhite();

    OSInitFastCast();
    void* pikari_bck = JKRGetNameResource("zelda_icon_pikari.bck", dComIfGp_getMain2DArchive());
    mPikariBck = (J2DAnmTransformKey*)J2DAnmLoaderDataBase::load(pikari_bck);
    playPikariBckAnimation(1.0f);

    void* pikari_bpk = JKRGetNameResource("zelda_icon_pikari.bpk", dComIfGp_getMain2DArchive());
    mPikariBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(pikari_bpk);
    mPikariBpk->searchUpdateMaterialID(mpPikariScreen);
    playPikariBpkAnimation(1.0f);

    mpParent = new CPaneMgr(mpScreen, 'n_all', 2, NULL);
    JUT_ASSERT(0, mpParent != NULL);

    static u64 const a_tag[] = {'cont_at1', 'cont_at2', 'cont_at3', 'cont_at4', 'cont_at'};
    static u64 const b_tag[] = {'cont_bt1', 'cont_bt2', 'cont_bt3', 'cont_bt4', 'cont_bt'};
    static u64 const z_tag[] = {'cont_zt1', 'cont_zt2', 'cont_zt3', 'cont_zt4', 'cont_rt'};
    static u64 const x_tag[] = {'cont_xt1', 'cont_xt2', 'cont_xt3', 'cont_xt4', 'cont_xt5'};
    static u64 const y_tag[] = {'cont_yt1', 'cont_yt2', 'cont_yt3', 'cont_yt4', 'cont_yt5'};

    for (int i = 0; i < 5; i++) {
        mpAText[i] = new CPaneMgr(mpScreen, a_tag[i], 0, NULL);
        JUT_ASSERT(0, mpAText[i] != NULL);
        static_cast<J2DTextBox*>(mpAText[i]->getPanePtr())->setString(64, "");
        static_cast<J2DTextBox*>(mpAText[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpAText[i]->getPanePtr())
            ->setCharSpace(g_drawHIO.mButtonATextSpacing);

        mpBText[i] = new CPaneMgr(mpScreen, b_tag[i], 0, NULL);
        JUT_ASSERT(0, mpBText[i] != NULL);
        static_cast<J2DTextBox*>(mpBText[i]->getPanePtr())->setString(64, "");
        static_cast<J2DTextBox*>(mpBText[i]->getPanePtr())->setFont(mDoExt_getMesgFont());

        mpXYText[i][2] = new CPaneMgr(mpScreen, z_tag[i], 0, NULL);
        JUT_ASSERT(0, mpXYText[i][2] != NULL);
        static_cast<J2DTextBox*>(mpXYText[i][2]->getPanePtr())->setString(64, "");
        static_cast<J2DTextBox*>(mpXYText[i][2]->getPanePtr())->setFont(mDoExt_getMesgFont());

        mpXYText[i][0] = new CPaneMgr(mpScreen, x_tag[i], 0, NULL);
        JUT_ASSERT(0, mpXYText[i][0] != NULL);
        static_cast<J2DTextBox*>(mpXYText[i][0]->getPanePtr())->setString(64, "");
        static_cast<J2DTextBox*>(mpXYText[i][0]->getPanePtr())->setFont(mDoExt_getMesgFont());

        mpXYText[i][1] = new CPaneMgr(mpScreen, y_tag[i], 0, NULL);
        JUT_ASSERT(0, mpXYText[i][1] != NULL);
        static_cast<J2DTextBox*>(mpXYText[i][1]->getPanePtr())->setString(64, "");
        static_cast<J2DTextBox*>(mpXYText[i][1]->getPanePtr())->setFont(mDoExt_getMesgFont());
    }

    J2DTextBox::TFontSize font_size;
#if VERSION != VERSION_GCN_JPN
    font_size.mSizeX = 17.0f;
    font_size.mSizeY = 20.0f;
    for (int i = 0; i < 5; i++) {
        static_cast<J2DTextBox*>(mpAText[i]->getPanePtr())->setFontSize(font_size);
        static_cast<J2DTextBox*>(mpBText[i]->getPanePtr())->setFontSize(font_size);
        static_cast<J2DTextBox*>(mpXYText[i][0]->getPanePtr())->setFontSize(font_size);
        static_cast<J2DTextBox*>(mpXYText[i][1]->getPanePtr())->setFontSize(font_size);
        static_cast<J2DTextBox*>(mpXYText[i][2]->getPanePtr())->setFontSize(font_size);
    }
#endif

    init();
    field_0xa8 = 0;
    field_0x1e4 = 0;
    field_0x2fc = 0;
    field_0x36c = 0;

    OS_REPORT("exit dMeter2Draw_c::dMeter2Draw_c(JKRExpHeap *mp_heap)\n");
}

dMeter2Draw_c::~dMeter2Draw_c() {
    dComIfGp_getMsgDtArchive(0)->removeResource(dMeter2Info_getMsgResource());
    dComIfGp_getMsgDtArchive(0)->removeResource(dMeter2Info_getMsgUnitResource());
    dComIfGp_getItemIconArchive()->removeResourceAll();

    delete mpScreen;
    mpScreen = NULL;

    delete mpKanteraScreen;
    mpKanteraScreen = NULL;

    for (int i = 0; i < 2; i++) {
        delete mpKanteraMeter[i];
        mpKanteraMeter[i] = NULL;
    }

    delete mpParent;
    mpParent = NULL;

    delete mpPikariScreen;
    mpPikariScreen = NULL;

    delete mpPikariParent;
    mpPikariParent = NULL;

    delete mPikariBck;
    mPikariBck = NULL;

    delete mPikariBpk;
    mPikariBpk = NULL;

    for (int i = 0; i < 5; i++) {
        delete mpAText[i];
        mpAText[i] = NULL;

        delete mpBText[i];
        mpBText[i] = NULL;

        for (int j = 0; j < 3; j++) {
            delete mpXYText[i][j];
            mpXYText[i][j] = NULL;
        }
    }

    delete mpLifeParent;
    mpLifeParent = NULL;

    for (int i = 0; i < 20; i++) {
        delete mpLifeParts[i];
        mpLifeParts[i] = NULL;

        delete mpHeartMark[i];
        mpHeartMark[i] = NULL;

        delete mpHeartBase[i];
        mpHeartBase[i] = NULL;

        delete mpLifeTexture[i][0];
        mpLifeTexture[i][0] = NULL;

        delete mpLifeTexture[i][1];
        mpLifeTexture[i][1] = NULL;
    }

    delete mpBigHeart;
    mpBigHeart = NULL;

    delete mpMagicParent;
    mpMagicParent = NULL;

    delete mpMagicBase;
    mpMagicBase = NULL;

    delete mpMagicFrameL;
    mpMagicFrameL = NULL;

    delete mpMagicFrameR;
    mpMagicFrameR = NULL;

    for (int i = 0; i < 3; i++) {
        delete mpOxygenBpk[i];
        mpOxygenBpk[i] = NULL;
    }

    delete mpMagicMeter;
    mpMagicMeter = NULL;

    for (int i = 0; i < 2; i++) {
        delete mpSIParent[i];
        mpSIParent[i] = NULL;
    }

    delete mpLightDropParent;
    mpLightDropParent = NULL;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 3; j++) {
            if (mpSIParts[i][j] != NULL) {
                delete mpSIParts[i][j];
                mpSIParts[i][j] = NULL;
            }
        }
    }

    delete mpRupeeKeyParent;
    mpRupeeKeyParent = NULL;

    for (int i = 0; i < 4; i++) {
        delete mpRupeeTexture[i][0];
        mpRupeeTexture[i][0] = NULL;

        delete mpRupeeTexture[i][1];
        mpRupeeTexture[i][1] = NULL;
    }

    for (int i = 0; i < 3; i++) {
        delete mpRupeeParent[i];
        mpRupeeParent[i] = NULL;
    }

    for (int i = 0; i < 5; i++) {
        delete mpKeyTexture[i];
        mpKeyTexture[i] = NULL;
    }

    delete mpKeyParent;
    mpKeyParent = NULL;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            heap->free(mpItemBTex[i][j]);
            mpItemBTex[i][j] = NULL;
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                heap->free(mpItemXYTex[i][j][k]);
                mpItemXYTex[i][j][k] = NULL;
            }
        }
    }

    delete mpItemB;
    mpItemB = NULL;
    mpItemBPane = NULL;

    for (int i = 0; i < 2; i++) {
        if (mpItemXY[i] != NULL) {
            delete mpItemXY[i];
            mpItemXY[i] = NULL;
        }
    }

    delete mpItemR;
    mpItemR = NULL;

    for (int i = 0; i < 3; i++) {
        mpItemXYPane[i] = NULL;
    }

    delete mpLightB;
    mpLightB = NULL;

    for (int i = 0; i < 3; i++) {
        if (mpLightXY[i] != NULL) {
            delete mpLightXY[i];
            mpLightXY[i] = NULL;
        }
    }

    delete mpBTextA;
    mpBTextA = NULL;

    delete mpBTextB;
    mpBTextB = NULL;

    for (int i = 0; i < 3; i++) {
        if (mpBTextXY[i] != NULL) {
            delete mpBTextXY[i];
            mpBTextXY[i] = NULL;
        }
    }

    delete mpTextA;
    mpTextA = NULL;

    delete mpTextB;
    mpTextB = NULL;

    for (int i = 0; i < 3; i++) {
        delete mpTextXY[i];
        mpTextXY[i] = NULL;
    }

    delete mpTextI;
    mpTextI = NULL;

    delete mpTextM;
    mpTextM = NULL;

    for (int i = 0; i < 5; i++) {
        if (mpJujiI[i] != NULL) {
            delete mpJujiI[i];
            mpJujiI[i] = NULL;
        }

        if (mpJujiM[i] != NULL) {
            delete mpJujiM[i];
            mpJujiM[i] = NULL;
        }
    }

    delete mpButtonParent;
    mpButtonParent = NULL;

    delete mpButtonA;
    mpButtonA = NULL;

    delete mpButtonB;
    mpButtonB = NULL;

    delete mpButtonMidona;
    mpButtonMidona = NULL;

    for (int i = 0; i < 3; i++) {
        delete mpButtonXY[i];
        mpButtonXY[i] = NULL;
    }

    if (mpUzu != NULL) {
        delete mpUzu;
        mpUzu = NULL;
    }

    delete mpButtonCrossParent;
    mpButtonCrossParent = NULL;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            if (mpItemNumTex[i][j] != NULL) {
                delete mpItemNumTex[i][j];
                mpItemNumTex[i][j] = NULL;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (field_0x9c[i] != NULL) {
            delete field_0x9c[i];
            field_0x9c[i] = NULL;
        }
    }
}

void dMeter2Draw_c::init() {
    field_0x608 = 0.0f;
    field_0x60c = 0.0f;

    for (int i = 0; i < 3; i++) {
        field_0x620[i] = 0.0f;
        field_0x610[i] = 0.0f;
    }

    field_0x61c = 0.0f;
    for (int i = 0; i < 16; i++) {
        field_0x62c[i] = 0.0f;
        field_0x66c[i] = 0.0f;
    }

    field_0x759 = 0;
    field_0x75a = 0;
    for (int i = 0; i < 3; i++) {
        field_0x75c[i] = 0;
    }

    field_0x75f = 0;
    field_0x756 = -1;
    field_0x760 = 0;

    for (int j = 0; j < 3; j++) {
        field_0x6ac[j] = 0.0f;
        field_0x6b8[j] = 0.0f;
        field_0x6c4[j] = 0.0f;
        field_0x6d0[j] = 0.0f;
    }

    field_0x6dc = 0.0f;
    field_0x6e0 = 0.0f;
    field_0x6e4 = 0.0f;
    field_0x6e8 = 0.0f;
    field_0x6ec = 0.0f;
    field_0x6f0 = 0.0f;
    mParentScale = g_drawHIO.mParentScale;
    mParentAlpha = g_drawHIO.mParentAlpha;
    mButtonsPosX = 0.0f;
    mButtonsPosY = 0.0f;
    mButtonsScale = 1.0f;
    field_0x78c = g_drawHIO.mButtonDisplayBackScale;
    mLifeParentAlpha = 0.0f;
    mLifeParentHeartAlpha = g_drawHIO.mLifeParentHeartAlpha;
    mHeartBaseAlpha = g_drawHIO.mHeartBaseAlpha;
    mHeartAlpha = g_drawHIO.mHeartAlpha;
    mBigHeartAlpha = g_drawHIO.mBigHeartAlpha;
    field_0x7a4 = 0.0f;
    mMagicMeterAlpha = g_drawHIO.mMagicMeterAlpha;
    mMagicMeterFrameAlpha = g_drawHIO.mMagicMeterFrameAlpha;
    field_0x7b0 = 0.0f;
    mLanternMeterAlpha = g_drawHIO.mLanternMeterAlpha;
    mLanternMeterFrameAlpha = g_drawHIO.mLanternMeterFrameAlpha;
    field_0x7bc = 0.0f;
    mOxygenMeterAlpha = g_drawHIO.mOxygenMeterAlpha;
    mOxygenMeterFrameAlpha = g_drawHIO.mOxygenMeterFrameAlpha;
    field_0x7c8 = 0.0f;
    field_0x860[1] = 0;
    field_0x860[0] = 0;
    field_0x7cc = g_drawHIO.mParentAlpha;
    field_0x7d0 = g_drawHIO.mRupeeKeyAlpha;
    mRupeeAlpha = g_drawHIO.mRupeeAlpha;
    mRupeeFrameAlpha = g_drawHIO.mRupeeFrameAlpha;
    mRupeeCountAlpha = g_drawHIO.mRupeeCountAlpha;
    field_0x7e0 = g_drawHIO.mParentAlpha;
    field_0x7e4 = g_drawHIO.mRupeeKeyAlpha;
    mKeyAlpha = g_drawHIO.mKeyAlpha;
    mKeyNumAlpha = g_drawHIO.mKeyNumAlpha;
    field_0x7f0 = g_drawHIO.mParentAlpha;
    mMainHUDButtonsAlpha = g_drawHIO.mMainHUDButtonsAlpha;
    mButtonDisplayBackAlpha = g_drawHIO.mButtonDisplayBackAlpha;
    mButtonAAlpha = g_drawHIO.mButtonAAlpha;
    mButtonBAlpha = g_drawHIO.mButtonBAlpha;
    mButtonXAlpha = g_drawHIO.mButtonXAlpha;
    mButtonYAlpha = g_drawHIO.mButtonYAlpha;
    field_0x80c = g_drawHIO.field_0x168;
    field_0x810 = g_drawHIO.mButtonZAlpha;

    for (int i = 0; i < 2; i++) {
        mItemBBaseAlpha[i] = g_drawHIO.mItemBBaseAlpha[i];
        mButtonXItemBaseAlpha[i] = g_drawHIO.mButtonXItemBaseAlpha[i];
        mButtonYItemBaseAlpha[i] = g_drawHIO.mButtonYItemBaseAlpha[i];
        field_0x82c[i] = g_drawHIO.field_0x298[i];
    }

    mButtonZItemBaseAlpha = g_drawHIO.mButtonZItemBaseAlpha;
    mButtonBaseAlpha = g_drawHIO.mButtonBaseAlpha;
    mButtonATextSpacing = g_drawHIO.mButtonATextSpacing;
    mButtonCrossAlpha = g_drawHIO.mButtonCrossAlpha;
    mButtonCrossITEMAlpha = g_drawHIO.mButtonCrossITEMAlpha;
    mButtonCrossMAPAlpha = g_drawHIO.mButtonCrossMAPAlpha;

    mButtonATextColor.r = g_drawHIO.mButtonATextColor.r;
    mButtonATextColor.g = g_drawHIO.mButtonATextColor.g;
    mButtonATextColor.b = g_drawHIO.mButtonATextColor.b;
    mButtonATextColor.a = g_drawHIO.mButtonATextColor.a;

    mButtonBTextColor.r = g_drawHIO.mButtonBFontColor.r;
    mButtonBTextColor.g = g_drawHIO.mButtonBFontColor.g;
    mButtonBTextColor.b = g_drawHIO.mButtonBFontColor.b;
    mButtonBTextColor.a = g_drawHIO.mButtonBFontColor.a;

    field_0x854.r = g_drawHIO.field_0x2f0.r;
    field_0x854.g = g_drawHIO.field_0x2f0.g;
    field_0x854.b = g_drawHIO.field_0x2f0.b;
    field_0x854.a = g_drawHIO.field_0x2f0.a;

    mButtonZTextColor.r = g_drawHIO.mButtonZFontColor.r;
    mButtonZTextColor.g = g_drawHIO.mButtonZFontColor.g;
    mButtonZTextColor.b = g_drawHIO.mButtonZFontColor.b;
    mButtonZTextColor.a = g_drawHIO.mButtonZFontColor.a;

    mButtonXYTextColor.r = g_drawHIO.mButtonXYTextColor.r;
    mButtonXYTextColor.g = g_drawHIO.mButtonXYTextColor.g;
    mButtonXYTextColor.b = g_drawHIO.mButtonXYTextColor.b;
    mButtonXYTextColor.a = g_drawHIO.mButtonXYTextColor.a;

    setItemParamX(0);
    setItemParamY(0);
    setItemParamZ(0);
    setItemParamB(0);
    initLife();
    initMagic();
    initLightDrop();
    initRupeeKey();
    initButton();
    initButtonCross();
    field_0x772 = 0;
}

void dMeter2Draw_c::exec(u32 i_status) {
    if (mParentScale != g_drawHIO.mParentScale) {
        mParentScale = g_drawHIO.mParentScale;
        mpParent->scale(g_drawHIO.mParentScale, g_drawHIO.mParentScale);
    }

    if (mParentAlpha != g_drawHIO.mParentAlpha) {
        mParentAlpha = g_drawHIO.mParentAlpha;
        mpParent->setAlphaRate(g_drawHIO.mParentAlpha);
    }

    if (i_status & 0x1000000) {
        if (mButtonsPosX != g_drawHIO.mRingHUDButtonsPosX ||
            mButtonsPosY != g_drawHIO.mRingHUDButtonsPosY)
        {
            mButtonsPosX = g_drawHIO.mRingHUDButtonsPosX;
            mButtonsPosY = g_drawHIO.mRingHUDButtonsPosY;
            mpButtonParent->paneTrans(g_drawHIO.mRingHUDButtonsPosX, g_drawHIO.mRingHUDButtonsPosY);
        }

        if (mButtonsScale != g_drawHIO.mRingHUDButtonsScale) {
            mButtonsScale = g_drawHIO.mRingHUDButtonsScale;
            mpButtonParent->scale(g_drawHIO.mRingHUDButtonsScale, g_drawHIO.mRingHUDButtonsScale);
        }
    } else {
        if (mButtonsPosX != g_drawHIO.mMainHUDButtonsPosX ||
            mButtonsPosY != g_drawHIO.mMainHUDButtonsPosY)
        {
            mButtonsPosX = g_drawHIO.mMainHUDButtonsPosX;
            mButtonsPosY = g_drawHIO.mMainHUDButtonsPosY;
            mpButtonParent->paneTrans(g_drawHIO.mMainHUDButtonsPosX, g_drawHIO.mMainHUDButtonsPosY);
        }

        if (mButtonsScale != g_drawHIO.mMainHUDButtonsScale) {
            mButtonsScale = g_drawHIO.mMainHUDButtonsScale;
            mpButtonParent->scale(g_drawHIO.mMainHUDButtonsScale, g_drawHIO.mMainHUDButtonsScale);
        }
    }
}

void dMeter2Draw_c::draw() {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    graf_ctx->setup2D();

    mpScreen->draw(0.0f, 0.0f, graf_ctx);
    drawKanteraScreen(1);
    drawKanteraScreen(2);

    for (int i = 0; i < 2; i++) {
        if (mpItemXY[i] != NULL) {
            for (int j = 0; j < 3; j++) {
                f32 temp_f30 = mItemParams[i].num_scale * 16.0f;

                Vec vtx0 = mpItemXY[i]->getPanePtr()->getGlbVtx(0);
                Vec vtx3 = mpItemXY[i]->getPanePtr()->getGlbVtx(3);

                mpItemNumTex[i][j]->draw(mItemParams[i].num_pos_x +
                                             (((vtx0.x + vtx3.x) * 0.5f) + (temp_f30 * j)),
                                         mItemParams[i].num_pos_y +
                                             (((vtx0.y + vtx3.y) * 0.5f) + mpItemXY[i]->getSizeY()),
                                         temp_f30, temp_f30, false, false, false);
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        mpKanteraMeter[i]->drawSelf();
    }

    if (!dComIfGp_isPauseFlag() && mpButtonParent->getAlphaRate() != 0.0f) {
        if (field_0x608 > 0.0f) {
            drawPikari(mpBTextA, &field_0x608, g_drawHIO.mButtonAPikariScale,
                       g_drawHIO.mButtonAPikariFrontOuter, g_drawHIO.mButtonAPikariFrontInner,
                       g_drawHIO.mButtonAPikariBackOuter, g_drawHIO.mButtonAPikariBackInner,
                       g_drawHIO.mButtonAPikariAnimSpeed, field_0x759);
        }

        if (field_0x60c > 0.0f) {
            drawPikari(mpBTextB, &field_0x60c, g_drawHIO.mButtonBPikariScale,
                       g_drawHIO.mButtonBPikariFrontOuter, g_drawHIO.mButtonBPikariFrontInner,
                       g_drawHIO.mButtonBPikariBackOuter, g_drawHIO.mButtonBPikariBackInner,
                       g_drawHIO.mButtonBPikariAnimSpeed, field_0x75a);
        }

        for (int i = 0; i < 2; i++) {
            if (field_0x620[i] > 0.0f) {
                drawPikari(mpBTextXY[i], &field_0x620[i], g_drawHIO.mButtonXYPikariScale,
                           g_drawHIO.mButtonXYPikariFrontOuter, g_drawHIO.mButtonXYPikariFrontInner,
                           g_drawHIO.mButtonXYPikariBackOuter, g_drawHIO.mButtonXYPikariBackInner,
                           g_drawHIO.mButtonXYPikariAnimSpeed, field_0x75c[i]);
            }
        }
    }

    if (mpLightDropParent->getAlphaRate() != 0.0f) {
        f32 var_f28 = g_drawHIO.mLightDrop.mPikariScaleNormal;
        f32 var_f29 = g_drawHIO.mLightDrop.mDropPikariAnimSpeed;

        if (field_0x756 >= 0) {
            var_f29 = g_drawHIO.mLightDrop.mDropPikariAnimSpeed_Completed;
            int temp_r5_2 = g_drawHIO.mLightDrop.mPikariInterval * 15;

            if (field_0x756 <= temp_r5_2) {
                int temp_r4 = (field_0x756 % g_drawHIO.mLightDrop.mPikariInterval);
                int temp_r3_5 = field_0x756 / g_drawHIO.mLightDrop.mPikariInterval;

                if (temp_r4 == 0 && field_0x62c[temp_r3_5] == 0.0f) {
                    field_0x62c[temp_r3_5] = 18.0f;
                }

                var_f28 = g_drawHIO.mLightDrop.mPikariScaleComplete;
                field_0x756++;
            } else {
                int temp_r5_3 = temp_r5_2 + 1;

                if (field_0x756 == temp_r5_3) {
                    if (field_0x62c[15] == 0.0f) {
                        field_0x756++;
                    }

                    var_f28 = g_drawHIO.mLightDrop.mPikariScaleComplete;
                } else if (field_0x756 >= g_drawHIO.mLightDrop.field_0x54 + temp_r5_3) {
                    for (int i = 0; i < 16; i++) {
                        field_0x62c[i] = 18.0f - var_f29;
                        field_0x66c[i] = 18.0f - g_drawHIO.mLightDrop.mPikariLoopAnimSpeed;
                    }

                    field_0x756 = -1;
                } else {
                    field_0x756++;
                }
            }
        }

        for (int i = 0; i < 16; i++) {
            if (field_0x66c[i] > 0.0f) {
                drawPikari(mpSIParts[i][1], &g_drawHIO.mLightDrop.mPikariLoopBackStopFrame,
                           g_drawHIO.mLightDrop.mPikariLoopBackScale,
                           g_drawHIO.mLightDrop.mPikariLoopFrontOuter[1],
                           g_drawHIO.mLightDrop.mPikariLoopFrontInner[1],
                           g_drawHIO.mLightDrop.mPikariLoopBackOuter[1],
                           g_drawHIO.mLightDrop.mPikariLoopBackInner[1], 0.0f, 3);
                drawPikari(mpSIParts[i][1], &field_0x66c[i], g_drawHIO.mLightDrop.mPikariLoopScale,
                           g_drawHIO.mLightDrop.mPikariLoopFrontOuter[0],
                           g_drawHIO.mLightDrop.mPikariLoopFrontInner[0],
                           g_drawHIO.mLightDrop.mPikariLoopBackOuter[0],
                           g_drawHIO.mLightDrop.mPikariLoopBackInner[0],
                           g_drawHIO.mLightDrop.mPikariLoopAnimSpeed, 3);
            }

            if (g_drawHIO.mLightDrop.mAnimDebug) {
                if (dComIfGp_getNeedLightDropNum() !=
                    dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea()))
                {
                    field_0x66c[i] = 0.0f;
                }
            }
        }

        for (int i = 0; i < 16; i++) {
            if (field_0x62c[i] > 0.0f) {
                drawPikari(mpSIParts[i][1], &field_0x62c[i], var_f28,
                           g_drawHIO.mLightDrop.mDropPikariFrontOuter,
                           g_drawHIO.mLightDrop.mDropPikariFrontInner,
                           g_drawHIO.mLightDrop.mDropPikariBackOuter,
                           g_drawHIO.mLightDrop.mDropPikariBackInner, var_f29, field_0x75f);
            }
        }
    }

    if (field_0x738 > 0.0f) {
        drawPikari(mpButtonMidona, &field_0x738, g_drawHIO.mMidnaIconPikariScale,
                   g_drawHIO.mMidnaIconPikariFrontOuter, g_drawHIO.mMidnaIconPikariFrontInner,
                   g_drawHIO.mMidnaIconPikariBackOuter, g_drawHIO.mMidnaIconPikariBackInner,
                   g_drawHIO.mMidnaIconPikariAnimSpeed, 3);
    }
}

void dMeter2Draw_c::initLife() {
    if (dComIfGs_getLife() > dComIfGs_getMaxLifeGauge()) {
        // "●●●●●●Heart Count adjust======>%d, %d\n"
        OS_REPORT("●●●●●●ハートの数調整======>%d, %d\n", dComIfGs_getLife(),
                  dComIfGs_getMaxLifeGauge());
        dComIfGs_setLife(dComIfGs_getMaxLifeGauge());
    }

    static u64 const life_tag[] = {
        'hpb_00', 'hpb_01', 'hpb_02', 'hpb_03', 'hpb_04', 'hpb_05', 'hpb_06',
        'hpb_07', 'hpb_08', 'hpb_09', 'hpb_10', 'hpb_11', 'hpb_12', 'hpb_13',
        'hpb_14', 'hpb_15', 'hpb_16', 'hpb_17', 'hpb_18', 'hpb_19',
    };

    static u64 const mark_tag[] = {
        'heartn00', 'heartn01', 'heartn02', 'heartn03', 'heartn04', 'heartn05', 'heartn06',
        'heartn07', 'heartn08', 'heartn09', 'heartn10', 'heartn11', 'heartn12', 'heartn13',
        'heartn14', 'heartn15', 'heartn16', 'heartn17', 'heartn18', 'heartn19',
    };

    static u64 const base_tag[] = {
        'h_ba_n00', 'h_ba_n01', 'h_ba_n02', 'h_ba_n03', 'h_ba_n04', 'h_ba_n05', 'h_ba_n06',
        'h_ba_n07', 'h_ba_n08', 'h_ba_n09', 'h_ba_n10', 'h_ba_n11', 'h_ba_n12', 'h_ba_n13',
        'h_ba_n14', 'h_ba_n15', 'h_ba_n16', 'h_ba_n17', 'h_ba_n18', 'h_ba_n19',
    };

    static u64 const lifet1_tag[] = {
        'hear_00s', 'hear_01s', 'hear_02s', 'hear_03s', 'hear_04s', 'hear_05s', 'hear_06s',
        'hear_07s', 'hear_08s', 'hear_09s', 'hear_10s', 'hear_11s', 'hear_12s', 'hear_13s',
        'hear_14s', 'hear_15s', 'hear_16s', 'hear_17s', 'hear_18s', 'hear_19s',
    };

    static u64 const lifet2_tag[] = {
        'hear_00', 'hear_01', 'hear_02', 'hear_03', 'hear_04', 'hear_05', 'hear_06',
        'hear_07', 'hear_08', 'hear_09', 'hear_10', 'hear_11', 'hear_12', 'hear_13',
        'hear_14', 'hear_15', 'hear_16', 'hear_17', 'hear_18', 'hear_19',
    };

    mpLifeParent = new CPaneMgr(mpScreen, 'heart_n', 2, NULL);
    JUT_ASSERT(0, mpLifeParent != NULL);

    for (int i = 0; i < 20; i++) {
        mpLifeParts[i] = new CPaneMgr(mpScreen, life_tag[i], 0, NULL);
        JUT_ASSERT(0, mpLifeParts[i] != NULL);

        mpHeartMark[i] = new CPaneMgr(mpScreen, mark_tag[i], 2, NULL);
        JUT_ASSERT(0, mpHeartMark[i] != NULL);

        mpHeartBase[i] = new CPaneMgrAlpha(mpScreen, base_tag[i], 2, NULL);
        JUT_ASSERT(0, mpHeartBase[i] != NULL);

        mpLifeTexture[i][0] = new CPaneMgrAlpha(mpScreen, lifet1_tag[i], 0, NULL);
        JUT_ASSERT(0, mpLifeTexture[i][0] != NULL);

        mpLifeTexture[i][1] = new CPaneMgrAlpha(mpScreen, lifet2_tag[i], 0, NULL);
        JUT_ASSERT(0, mpLifeTexture[i][1] != NULL);
    }

    mpBigHeart = new CPaneMgr(mpScreen, 'bigh_n', 2, NULL);
    JUT_ASSERT(0, mpBigHeart != NULL);

    mpLifeParent->setAlphaRate(0.0f);

    f32 var_f1 = g_drawHIO.mLifeTopPosX;
    f32 var_f2 = 0.0f;

    if (dComIfGs_getMaxLife() <= 50) {
        var_f2 = g_drawHIO.mLifeTopPosY;
    }

    drawLife(dComIfGs_getMaxLife(), dComIfGs_getLife(), g_drawHIO.mLifeGaugePosX + var_f1,
             g_drawHIO.mLifeGaugePosY + var_f2);
    setAlphaLifeChange(true);
}

void dMeter2Draw_c::initMagic() {
    field_0x550 = 0.0f;
    field_0x554 = 0.0f;

    mpMagicParent = new CPaneMgr(mpKanteraScreen, 'magic_n', 2, NULL);
    JUT_ASSERT(0, mpMagicParent != NULL);

    mpMagicBase = new CPaneMgr(mpKanteraScreen, 'mm_base', 0, NULL);
    JUT_ASSERT(0, mpMagicBase != NULL);

    mpMagicFrameL = new CPaneMgr(mpKanteraScreen, 'm_w_l_n', 2, NULL);
    JUT_ASSERT(0, mpMagicFrameL != NULL);

    mpMagicFrameR =
        new CPaneMgr(mpKanteraScreen, 'm_w_r_n', 2, NULL);
    JUT_ASSERT(0, mpMagicFrameR != NULL);

    mpMagicMeter = new CPaneMgr(mpKanteraScreen, 'mm_00', 0, NULL);
    JUT_ASSERT(0, mpMagicMeter != NULL);

    OSInitFastCast();

    void* res = JKRFileLoader::getGlbResource("zelda_game_image_sanso_10percent.bpk",
                                              dComIfGp_getMain2DArchive());
    mpOxygenBpk[0] = (J2DAnmColor*)J2DAnmLoaderDataBase::load(res);
    mpOxygenBpk[0]->searchUpdateMaterialID(mpKanteraScreen);

    res = JKRFileLoader::getGlbResource("zelda_game_image_sanso_25percent.bpk",
                                        dComIfGp_getMain2DArchive());
    mpOxygenBpk[1] = (J2DAnmColor*)J2DAnmLoaderDataBase::load(res);
    mpOxygenBpk[1]->searchUpdateMaterialID(mpKanteraScreen);

    res = JKRFileLoader::getGlbResource("zelda_game_image_sanso_50percent.bpk",
                                        dComIfGp_getMain2DArchive());
    mpOxygenBpk[2] = (J2DAnmColor*)J2DAnmLoaderDataBase::load(res);
    mpOxygenBpk[2]->searchUpdateMaterialID(mpKanteraScreen);

    field_0x558 = 0.0f;
    mpMagicParent->setAlphaRate(0.0f);

    f32 offsetX = g_drawHIO.mLifeTopPosX;
    f32 offsetY = 0.0f;

    if (dComIfGs_getMaxLife() <= 50) {
        offsetY = g_drawHIO.mLifeTopPosY;
    }

    for (int i = 0; i < 3; i++) {
        mMeterAlphaRate[i] = 0.0f;
        field_0x742[i] = 0;
    }

    u8 magic = dComIfGs_getMagic();
    u8 max_magic = dComIfGs_getMaxMagic();
    drawMagic(max_magic, magic, g_drawHIO.mMagicMeterPosX + offsetX,
              g_drawHIO.mMagicMeterPosY + offsetY);
    setAlphaMagicChange(true);

    drawKantera(dComIfGs_getMaxOil(), dComIfGs_getOil(), g_drawHIO.mLanternMeterPosX + offsetX,
                g_drawHIO.mNoMagicPosY + (g_drawHIO.mLanternMeterPosY + offsetY));
    setAlphaKanteraChange(true);
}

void dMeter2Draw_c::initLightDrop() {
    mpLightDropParent = new CPaneMgr(mpScreen, 's_tuta_n', 2, NULL);
    JUT_ASSERT(0, mpLightDropParent != NULL);

    static u64 const tuta_0[] = {
        's_00', 's_01', 's_02', 's_03', 's_04', 's_05', 's_06', 's_07',
        's_08', 's_09', 's_10', 's_11', 's_12', 's_13', 's_14', 's_15',
    };

    static u64 const tuta_1[] = {
        'sl_00', 'sl_01', 'sl_02', 'sl_03', 'sl_04', 'sl_05', 'sl_06', 'sl_07',
        'sl_08', 'sl_09', 'sl_10', 'sl_11', 'sl_12', 'sl_13', 'sl_14', 'sl_15',
    };

    static u64 const tuta_2[] = {
        'scp_00', 'scp_01', 'scp_02', 'scp_03', 'scp_04', 'scp_05', 'scp_06', 'scp_07',
        'scp_08', 'scp_09', 'scp_10', 'scp_11', 'scp_12', 'scp_13', 'scp_14', 'scp_15',
    };

    for (int i = 0; i < 16; i++) {
        mpSIParts[i][0] = new CPaneMgr(mpScreen, tuta_0[i], 0, NULL);
        JUT_ASSERT(0, mpSIParts[i][0] != NULL);

        mpSIParts[i][1] = new CPaneMgr(mpScreen, tuta_1[i], 0, NULL);
        JUT_ASSERT(0, mpSIParts[i][1] != NULL);

        mpSIParts[i][2] = new CPaneMgr(mpScreen, tuta_2[i], 0, NULL);
        JUT_ASSERT(0, mpSIParts[i][2] != NULL);
    }

    mpSIParent[0] = new CPaneMgr(mpScreen, 'tuta_n', 2, NULL);
    JUT_ASSERT(0, mpSIParent[0] != NULL);

    mpSIParent[1] = new CPaneMgr(mpScreen, 's_null', 2, NULL);
    JUT_ASSERT(0, mpSIParent[1] != NULL);

    mpLightDropParent->setAlphaRate(0.0f);
    field_0x6fc = 0.0f;
    mLightDropVesselScale = g_drawHIO.mLightDrop.mVesselScale;
    field_0x6f8 = 1.0f;

    for (int i = 0; i < 3; i++) {
        if (dComIfGs_isLightDropGetFlag(i)) {
            dMeter2Info_setLightDropGetFlag(i, 0xFF);
        } else {
            dMeter2Info_setLightDropGetFlag(i, 0);
        }
    }

    drawLightDrop(dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea()),
                  dComIfGp_getNeedLightDropNum(), g_drawHIO.mLightDrop.mVesselPosX,
                  g_drawHIO.mLightDrop.mVesselPosY, g_drawHIO.mLightDrop.mVesselScale,
                  g_drawHIO.mLightDrop.mVesselAlpha[0], 0);
    setAlphaLightDropChange(true);
}

void dMeter2Draw_c::initRupeeKey() {
    mpRupeeKeyParent = new CPaneMgr(mpScreen, 'r_k_n', 2, NULL);
    JUT_ASSERT(0, mpRupeeKeyParent != NULL);
    field_0x718 = 1.0f;

    static u64 const rupeet1_tag[] = {'r_n_1_s', 'r_n_2_s', 'r_n_3_s', 'r_n_4_s'};
    static u64 const rupeet2_tag[] = {'r_n_1', 'r_n_2', 'r_n_3', 'r_n_4'};

    for (int i = 0; i < 4; i++) {
        mpRupeeTexture[i][0] = new CPaneMgr(mpScreen, rupeet1_tag[i], 0, NULL);
        JUT_ASSERT(0, mpRupeeTexture[i][0] != NULL);
        mpRupeeTexture[i][0]->getPanePtr()->setBasePosition(J2DBasePosition_4);

        mpRupeeTexture[i][1] = new CPaneMgr(mpScreen, rupeet2_tag[i], 0, NULL);
        JUT_ASSERT(0, mpRupeeTexture[i][1] != NULL);
        mpRupeeTexture[i][1]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    }

    mpRupeeParent[0] = new CPaneMgr(mpScreen, 'rupi_n', 2, NULL);
    JUT_ASSERT(0, mpRupeeParent[0] != NULL);

    mpRupeeParent[1] = new CPaneMgr(mpScreen, 'moyou_rn', 2, NULL);
    JUT_ASSERT(0, mpRupeeParent[1] != NULL);

    mpRupeeParent[2] = new CPaneMgr(mpScreen, 'moyou_ln', 2, NULL);
    JUT_ASSERT(0, mpRupeeParent[2] != NULL);

    mpRupeeParent[0]->setAlphaRate(0.0f);
    mpRupeeParent[1]->setAlphaRate(0.0f);
    mpRupeeParent[2]->setAlphaRate(0.0f);

    static u64 const key_tag[] = {'key_nul', 'k_n_1_n', 'k_n_2_n', 'k_n_3_n', 'k_n_4_n'};

    for (int i = 0; i < 5; i++) {
        mpKeyTexture[i] = new CPaneMgr(mpScreen, key_tag[i], 0, NULL);
        JUT_ASSERT(0, mpKeyTexture[i] != NULL);
        mpKeyTexture[i]->getPanePtr()->setBasePosition(J2DBasePosition_4);
        mpKeyTexture[i]->hide();
    }

    mpKeyParent = new CPaneMgr(mpScreen, 'key_n', 2, NULL);
    JUT_ASSERT(0, mpKeyParent != NULL);
    mpKeyParent->setAlphaRate(0.0f);

    drawRupee(dComIfGs_getRupee());
    drawKey(dComIfGs_getKeyNum());
    setAlphaRupeeChange(true);
    setAlphaKeyChange(true);
}

void dMeter2Draw_c::initButton() {
    field_0x761 = 1;
    field_0x762 = 1;
    field_0x763 = 1;
    field_0x764 = 1;
    field_0x765 = 1;
    field_0x766 = 1;
    field_0x767 = 1;

    for (int i = 0; i < 3; i++) {
        field_0x768[i] = 1;
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpItemBTex[i][j] = (ResTIMG*)heap->alloc(0xC00, 0x20);
            JUT_ASSERT(0, mpItemBTex[i][j] != NULL);
        }
    }

    for (int i = 0; i < 2; i++) {
        field_0x773[i] = dMeter2Info_isDirectUseItem(i);
        field_0x76c[i] = 0;

        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                mpItemXYTex[i][j][k] = (ResTIMG*)heap->alloc(0xC00, 0x20);
                JUT_ASSERT(0, mpItemXYTex[i][j][k] != NULL);
            }
        }
    }

    mpItemB = new CPaneMgr(mpScreen, 'b_itm_p', 0, NULL);
    JUT_ASSERT(0, mpItemB != NULL);
    mpItemB->getPanePtr()->setBasePosition(J2DBasePosition_4);
    mpItemB->show();

    field_0x728 = 1.0f;
    field_0x734 = 1.0f;
    field_0x76b = 0;
    mButtonBItem = 0;

    dComIfGs_setBButtonItemKey(0xFF);
    field_0x770 = 0xFF;
    field_0x771 = dComIfGs_getBButtonItemKey();

    mpItemBPane = new J2DPicture(
        'b_itm_pp',
        JGeometry::TBox2<f32>(0.0f, 0.0f, mpItemB->getInitSizeX(), mpItemB->getInitSizeY()),
        static_cast<J2DPicture*>(mpItemB->getPanePtr())->getTexture(0)->getTexInfo(), NULL);
    JUT_ASSERT(0, mpItemBPane != NULL);
    mpItemBPane->setBasePosition(J2DBasePosition_4);
    mpItemB->getPanePtr()->appendChild(mpItemBPane);

    mpItemXY[0] = new CPaneMgr(mpScreen, 'x_itm_p', 0, NULL);
    JUT_ASSERT(0, mpItemXY[0] != NULL);
    mpItemXY[0]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    dMeter2Info_setMeterItemPanePtr(0, mpItemXY[0]);

    mpItemXY[1] = new CPaneMgr(mpScreen, 'y_itm_p', 0, NULL);
    JUT_ASSERT(0, mpItemXY[1] != NULL);
    mpItemXY[1]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    dMeter2Info_setMeterItemPanePtr(1, mpItemXY[1]);

    mpItemR = NULL;
    mpBTextA = NULL;

    mpItemXYPane[0] = new J2DPicture(
        'x_itm_pp',
        JGeometry::TBox2<f32>(0.0f, 0.0f, mpItemXY[0]->getInitSizeX(), mpItemXY[0]->getInitSizeY()),
        static_cast<J2DPicture*>(mpItemXY[0]->getPanePtr())->getTexture(0)->getTexInfo(), NULL);
    JUT_ASSERT(0, mpItemXYPane[0] != NULL);
    mpItemXYPane[0]->setBasePosition(J2DBasePosition_4);
    mpItemXY[0]->getPanePtr()->appendChild(mpItemXYPane[0]);

    mpItemXYPane[1] = new J2DPicture(
        'y_itm_pp',
        JGeometry::TBox2<f32>(0.0f, 0.0f, mpItemXY[1]->getInitSizeX(), mpItemXY[1]->getInitSizeY()),
        static_cast<J2DPicture*>(mpItemXY[1]->getPanePtr())->getTexture(0)->getTexInfo(), NULL);
    JUT_ASSERT(0, mpItemXYPane[1] != NULL);
    mpItemXYPane[1]->setBasePosition(J2DBasePosition_4);
    mpItemXY[1]->getPanePtr()->appendChild(mpItemXYPane[1]);

    mpItemR = new CPaneMgr(mpScreen, 'r_itm_p', 0, NULL);
    JUT_ASSERT(0, mpItemR != NULL);
    mpItemR->getPanePtr()->setBasePosition(J2DBasePosition_4);
    dMeter2Info_setMeterItemPanePtr(2, mpItemR);

    mpItemXYPane[2] = new J2DPicture(
        'r_itm_pp',
        JGeometry::TBox2<f32>(0.0f, 0.0f, mpItemR->getInitSizeX(), mpItemR->getInitSizeY()),
        static_cast<J2DPicture*>(mpItemR->getPanePtr())->getTexture(0)->getTexInfo(), NULL);
    JUT_ASSERT(0, mpItemXYPane[2] != NULL);
    mpItemXYPane[2]->setBasePosition(J2DBasePosition_4);
    mpItemR->getPanePtr()->appendChild(mpItemXYPane[2]);

    mpLightB = new CPaneMgr(mpScreen, 'b_light', 0, NULL);
    JUT_ASSERT(0, mpLightB != NULL);
    mpLightB->getPanePtr()->setBasePosition(J2DBasePosition_4);
    field_0x72c = 1.0f;
    mpLightB->hide();

    mpLightXY[0] = new CPaneMgr(mpScreen, 'x_light', 0, NULL);
    JUT_ASSERT(0, mpLightXY[0] != NULL);
    mpLightXY[0]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    mpLightXY[0]->hide();

    mpLightXY[1] = new CPaneMgr(mpScreen, 'y_light', 0, NULL);
    JUT_ASSERT(0, mpLightXY[1] != NULL);
    mpLightXY[1]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    mpLightXY[1]->hide();

    mpLightXY[2] = new CPaneMgr(mpScreen, 'r_light', 0, NULL);
    JUT_ASSERT(0, mpLightXY[2] != NULL);
    mpLightXY[2]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    mpLightXY[2]->hide();

    mpBTextA = new CPaneMgr(mpScreen, 'b_text_a', 0, NULL);
    JUT_ASSERT(0, mpBTextA != NULL);

    mpBTextB = new CPaneMgr(mpScreen, 'b_text_b', 0, NULL);
    JUT_ASSERT(0, mpBTextB != NULL);

    mpBTextXY[0] = new CPaneMgr(mpScreen, 'b_text_x', 0, NULL);
    JUT_ASSERT(0, mpBTextXY[0] != NULL);

    mpBTextXY[1] = new CPaneMgr(mpScreen, 'b_text_y', 0, NULL);
    JUT_ASSERT(0, mpBTextXY[1] != NULL);

    mpBTextXY[2] = NULL;

    mpTextA = new CPaneMgr(mpScreen, 'a_text_n', 0, NULL);
    JUT_ASSERT(0, mpTextA != NULL);

    mpTextB = new CPaneMgr(mpScreen, 'b_text_n', 0, NULL);
    JUT_ASSERT(0, mpTextB != NULL);

    mpTextXY[2] = new CPaneMgr(mpScreen, 'z_text_n', 0, NULL);
    JUT_ASSERT(0, mpTextXY[2] != NULL);

    mpTextXY[0] = new CPaneMgr(mpScreen, 'x_text_n', 0, NULL);
    JUT_ASSERT(0, mpTextXY[0] != NULL);

    mpTextXY[1] = new CPaneMgr(mpScreen, 'y_text_n', 0, NULL);
    JUT_ASSERT(0, mpTextXY[1] != NULL);

    mpTextI = new CPaneMgr(mpScreen, 'i_text_n', 2, NULL);
    JUT_ASSERT(0, mpTextI != NULL);

    mpTextM = new CPaneMgr(mpScreen, 'm_text_n', 2, NULL);
    JUT_ASSERT(0, mpTextM != NULL);

    static u64 const juji_i_tag[] = {'ju_ring4', 'yaji_00', 'yaji_01', 'ju_ring2', 'ju_ring4'};
    static u64 const juji_m_tag[] = {'ju_ring3', 'yaji_02', 'yaji_03', 'ju_ring3', 0};

    for (int i = 0; i < 5; i++) {
        if (juji_i_tag[i] != 0) {
            mpJujiI[i] = new CPaneMgr(mpScreen, juji_i_tag[i], 0, NULL);
            JUT_ASSERT(0, mpJujiI[i] != NULL);
        } else {
            mpJujiI[i] = NULL;
        }

        if (juji_m_tag[i] != 0) {
            mpJujiM[i] = new CPaneMgr(mpScreen, juji_m_tag[i], 0, NULL);
            JUT_ASSERT(0, mpJujiM[i] != NULL);
        } else {
            mpJujiM[i] = NULL;
        }
    }

    mpTextXY[0]->hide();
    mpTextXY[1]->hide();

    mpButtonParent = new CPaneMgr(mpScreen, 'cont_n', 2, NULL);
    JUT_ASSERT(0, mpButtonParent != NULL);

    mpButtonA = new CPaneMgr(mpScreen, 'abtn_n', 2, NULL);
    JUT_ASSERT(0, mpButtonA != NULL);

    mpButtonB = new CPaneMgr(mpScreen, 'bbtn_n', 2, NULL);
    JUT_ASSERT(0, mpButtonB != NULL);
    field_0x730 = 1.0f;

    mpButtonMidona = new CPaneMgr(mpScreen, 'midona_n', 2, NULL);
    JUT_ASSERT(0, mpButtonMidona != NULL);
    mButtonZAlpha = 0.0f;
    field_0x724 = 0.0f;
    field_0x738 = 0.0f;
    field_0x740 = 0;

    mpButtonXY[0] = new CPaneMgr(mpScreen, 'xbtn_n', 2, NULL);
    JUT_ASSERT(0, mpButtonXY[0] != NULL);

    mpButtonXY[1] = new CPaneMgr(mpScreen, 'ybtn_n', 2, NULL);
    JUT_ASSERT(0, mpButtonXY[1] != NULL);

    mpButtonXY[2] = new CPaneMgr(mpScreen, 'zbtn_n', 2, NULL);
    JUT_ASSERT(0, mpButtonXY[2] != NULL);

    mpUzu = new CPaneMgrAlpha(mpScreen, 'uzu_n', 2, NULL);
    JUT_ASSERT(0, mpUzu != NULL);

    ResTIMG* timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
        'TIMG', dMeter2Info_getNumberTextureName(0));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            mpItemNumTex[i][j] = new J2DPicture(timg);
            JUT_ASSERT(0, mpItemNumTex[i][j] != NULL);
        }
    }

    mButtonBItem = fpcNm_ITEM_SWORD;
    changeTextureItemB(mButtonBItem);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            field_0x524[i][j] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        field_0x9c[i] = NULL;
    }

    field_0x560 = 0.0f;
    field_0x55c = 0.0f;
    field_0x568 = 0.0f;
    field_0x564 = 0.0f;
    field_0x56c = 0.0f;
    field_0x574 = 0.0f;
    field_0x570 = 0.0f;
    field_0x57c = 0.0f;
    field_0x578 = 0.0f;
    field_0x580 = 0.0f;
    field_0x754 = 0;

    drawButtonXY(0, dComIfGp_getSelectItem(0), dComIfGp_getXStatus(), true, false);
    drawButtonXY(1, dComIfGp_getSelectItem(1), dComIfGp_getYStatus(), true, false);
    drawButtonA(dComIfGp_getDoStatus(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, false, false);
    drawButtonB(dComIfGp_getAStatus(), true, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, false);
    drawButtonR(dComIfGs_getCollectSmell(), dComIfGp_getRStatus(), true, false);
    drawButtonZ(dComIfGp_getZStatus());
    drawButton3D(dComIfGp_get3DStatus());
    drawButtonC(dComIfGp_getCStickStatus(), true);
    drawButtonS(dComIfGp_getSButtonStatus());

    setAlphaButtonChange(true);
    mpButtonParent->setAlphaRate(0.0f);
}

void dMeter2Draw_c::initButtonCross() {
    mpButtonCrossParent = new CPaneMgr(mpScreen, 'juji_n', 2, NULL);
    JUT_ASSERT(0, mpButtonCrossParent != NULL);

    static_cast<J2DTextBox*>(mpScreen->search('cont_ju0'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju1'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju2'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju3'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju4'))->setFont(mDoExt_getMesgFont());

    static_cast<J2DTextBox*>(mpScreen->search('cont_ju0'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju1'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju2'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju3'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju4'))->setString(0x40, "");

    dMeter2Info_getString(
        0x61, static_cast<J2DTextBox*>(mpScreen->search('cont_ju0'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x61, static_cast<J2DTextBox*>(mpScreen->search('cont_ju1'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x61, static_cast<J2DTextBox*>(mpScreen->search('cont_ju2'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x61, static_cast<J2DTextBox*>(mpScreen->search('cont_ju3'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x61, static_cast<J2DTextBox*>(mpScreen->search('cont_ju4'))->getStringPtr(), NULL);

    static_cast<J2DTextBox*>(mpScreen->search('cont_ju5'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju6'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju7'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju8'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju9'))->setFont(mDoExt_getMesgFont());

    static_cast<J2DTextBox*>(mpScreen->search('cont_ju5'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju6'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju7'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju8'))->setString(0x40, "");
    static_cast<J2DTextBox*>(mpScreen->search('cont_ju9'))->setString(0x40, "");

    dMeter2Info_getString(
        0x62, static_cast<J2DTextBox*>(mpScreen->search('cont_ju5'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x62, static_cast<J2DTextBox*>(mpScreen->search('cont_ju6'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x62, static_cast<J2DTextBox*>(mpScreen->search('cont_ju7'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x62, static_cast<J2DTextBox*>(mpScreen->search('cont_ju8'))->getStringPtr(), NULL);
    dMeter2Info_getString(
        0x62, static_cast<J2DTextBox*>(mpScreen->search('cont_ju9'))->getStringPtr(), NULL);

    mpButtonCrossParent->setAlphaRate(0.0f);
    drawButtonCross(g_drawHIO.mButtonCrossOFFPosX, g_drawHIO.mButtonCrossOFFPosY);
}

void dMeter2Draw_c::playPikariBckAnimation(f32 i_frame) {
    mpPikariParent->getPanePtr()->setAnimation(mPikariBck);
    mPikariBck->setFrame(i_frame);
    mpPikariParent->getPanePtr()->animationTransform();
    mpPikariParent->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
}

void dMeter2Draw_c::setPikariBpkAnimation(J2DAnmColor* i_bpk) {
    mpPikariScreen->search('pika00')->setAnimation(i_bpk);
    mpPikariScreen->search('moya00')->setAnimation(i_bpk);
    mpPikariScreen->search('moyabs')->setAnimation(i_bpk);
}

void dMeter2Draw_c::playPikariBpkAnimation(f32 i_frame) {
    setPikariBpkAnimation(mPikariBpk);
    mPikariBpk->setFrame(i_frame);
    mpPikariScreen->animation();
    setPikariBpkAnimation(NULL);
}

void dMeter2Draw_c::playOxygenBpkAnimation(J2DAnmColor* i_bpk) {
    if (i_bpk == NULL || mpMagicMeter->getAlpha() == 0) {
        mpKanteraScreen->search('mm_00')->setAnimation((J2DAnmColor*)NULL);
    } else {
        field_0x558++;
        if (field_0x558 >= i_bpk->getFrameMax()) {
            field_0x558 -= i_bpk->getFrameMax();
        }

        mpKanteraScreen->search('mm_00')->setAnimation(i_bpk);
        i_bpk->setFrame(field_0x558);
        mpKanteraScreen->animation();

        mpKanteraScreen->search('mm_00')->setAnimation((J2DAnmColor*)NULL);
    }
}

/* 80213FF0-802140DC 20E930 00EC+00 1/1 0/0 0/0 .text
 * drawPikari__13dMeter2Draw_cFP8CPaneMgrPffQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorfUc
 */
void dMeter2Draw_c::drawPikari(CPaneMgr* i_pane, f32* i_framep, f32 i_scale,
                               JUtility::TColor i_moyaBlack, JUtility::TColor i_moyaWhite,
                               JUtility::TColor i_moyabsBlack, JUtility::TColor i_moyabsWhite,
                               f32 param_7, u8 param_8) {
    Vec pos = i_pane->getGlobalVtxCenter(false, 0);
    drawPikari(pos.x, pos.y, i_framep, i_scale, i_moyaBlack, i_moyaWhite, i_moyabsBlack,
               i_moyabsWhite, param_7, param_8);
}

/* 802140DC-8021462C 20EA1C 0550+00 1/1 6/6 0/0 .text
 * drawPikari__13dMeter2Draw_cFffPffQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorfUc
 */
void dMeter2Draw_c::drawPikari(f32 i_posX, f32 i_posY, f32* i_framep, f32 i_scale,
                               JUtility::TColor i_moyaBlack, JUtility::TColor i_moyaWhite,
                               JUtility::TColor i_moyabsBlack, JUtility::TColor i_moyabsWhite,
                               f32 param_8, u8 param_9) {
    f32 var_f31 = 28.0f;

    if (param_9 == 4) {
        var_f31 = 24.0f;
        if (mpPikariScreen->search('moya00')->isVisible()) {
            mpPikariScreen->search('moya00')->hide();
        }

        if (mpPikariScreen->search('moyabs')->isVisible()) {
            mpPikariScreen->search('moyabs')->hide();
        }
    } else {
        if (!mpPikariScreen->search('moya00')->isVisible()) {
            mpPikariScreen->search('moya00')->show();
        }

        if (!mpPikariScreen->search('moyabs')->isVisible()) {
            mpPikariScreen->search('moyabs')->show();
        }
    }

    if (param_9 != 3 && param_9 != 4 && param_9 != 5 && dMsgObject_isTalkNowCheck()) {
        *i_framep = 0.0f;
    } else {
        *i_framep += param_8;
        if (*i_framep > var_f31) {
            if (param_9 == 1 || param_9 == 2 || param_9 == 3) {
                *i_framep = 18.0f;
            } else {
                *i_framep = 0.0f;
            }
        }

        if (*i_framep == 18.0f && param_9 == 1) {
            mDoAud_seStart(Z2SE_NAVI_BLINK, NULL, 0, 0);
        } else if (*i_framep == 18.0f && param_9 == 2) {
            mDoAud_seStart(Z2SE_SY_ITEM_COMBINE_ICON, NULL, 0, 0);
        }

        playPikariBckAnimation(*i_framep);
        playPikariBpkAnimation(*i_framep);

        mpPikariParent->getPanePtr()->translate(i_posX, i_posY);
        mpPikariParent->scale(i_scale * mpPikariParent->getScaleX(),
                              i_scale * mpPikariParent->getScaleY());

        if (mpPikariScreen->search('moya00')->isVisible()) {
            static_cast<J2DPicture*>(mpPikariScreen->search('moya00'))
                ->setBlackWhite(i_moyaBlack, i_moyaWhite);
        }

        if (mpPikariScreen->search('moyabs')->isVisible()) {
            static_cast<J2DPicture*>(mpPikariScreen->search('moyabs'))
                ->setBlackWhite(i_moyabsBlack, i_moyabsWhite);
        }

        if (param_9 != 4) {
            static_cast<J2DPicture*>(mpPikariScreen->search('pika00'))
                ->setBlackWhite(mPikariBlack, mPikariBlack);
        } else {
            static_cast<J2DPicture*>(mpPikariScreen->search('pika00'))
                ->setBlackWhite(g_MsgObject_HIO_c.mPikariPikaR1, g_MsgObject_HIO_c.mPikariPikaR0);
        }

        mpPikariScreen->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());
    }
}

/* 8021462C-802147CC 20EF6C 01A0+00 0/0 1/1 0/0 .text
 * drawPikariHakusha__13dMeter2Draw_cFffffQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColor
 */
void dMeter2Draw_c::drawPikariHakusha(f32 i_posX, f32 i_posY, f32 i_frame, f32 i_scale,
                                      JUtility::TColor i_moyaBlack, JUtility::TColor i_moyaWhite,
                                      JUtility::TColor i_moyabsBlack,
                                      JUtility::TColor i_moyabsWhite) {
    playPikariBckAnimation(i_frame);
    playPikariBpkAnimation(i_frame);

    mpPikariParent->getPanePtr()->translate(i_posX, i_posY);
    mpPikariParent->scale(i_scale * mpPikariParent->getScaleX(),
                          i_scale * mpPikariParent->getScaleY());

    static_cast<J2DPicture*>(mpPikariScreen->search('moya00'))
        ->setBlackWhite(i_moyaBlack, i_moyaWhite);
    static_cast<J2DPicture*>(mpPikariScreen->search('moyabs'))
        ->setBlackWhite(i_moyabsBlack, i_moyabsWhite);

    mpPikariScreen->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());
}

void dMeter2Draw_c::changeTextureLife(int i_no, bool param_1, u8 i_quarterNum) {
    if (param_1) {
        if (i_quarterNum == 0xFF) {
            mpLifeTexture[i_no][0]->show();
            mpLifeTexture[i_no][1]->show();
        } else {
            mpLifeTexture[i_no][0]->hide();
            mpLifeTexture[i_no][1]->hide();

            static u64 const tag_bigh[] = {'bigh_00', 'bigh_01', 'bigh_02', 'bigh_03'};

            for (int i = 0; i < 4; i++) {
                if (i == i_quarterNum) {
                    mpScreen->search(tag_bigh[i])->show();
                } else {
                    mpScreen->search(tag_bigh[i])->hide();
                }
            }

            static u64 const tag_du[] = {'heart_ln', 'heart_un'};

            mpBigHeart->translate(mpLifeParts[i_no]->getTranslateX() +
                                      (mpScreen->search(tag_du[i_no / 10])->getTranslateX() -
                                       mpScreen->search(tag_du[0])->getTranslateX()),
                                  mpScreen->search(tag_du[i_no / 10])->getTranslateY() +
                                      mpLifeParts[i_no]->getTranslateY());
        }
    } else {
        mpLifeTexture[i_no][0]->hide();
        mpLifeTexture[i_no][1]->hide();
    }
}

void dMeter2Draw_c::drawLife(s16 i_maxLife, s16 i_life, f32 i_posX, f32 i_posY) {
    s16 max_heart_cnt = i_maxLife / 5;
    s16 heart_cnt = i_life / 4;
    s16 heart_quarters = i_life % 4;
    if (i_life == max_heart_cnt * 4) {
        heart_quarters = 0;
    }

    if (heart_quarters == 0) {
        heart_cnt--;
    }

    if (i_life == 0) {
        mpBigHeart->hide();
    } else {
        mpBigHeart->show();
    }

    for (int i = 0; i < 20; i++) {
        if (i < max_heart_cnt) {
            mpLifeParts[i]->show();

            if (i == heart_cnt) {
                changeTextureLife(i, true, heart_quarters);
            } else if (i < heart_cnt) {
                changeTextureLife(i, true, 0xFF);
            } else {
                changeTextureLife(i, false, 0xFF);
            }
        } else {
            mpLifeParts[i]->hide();
            changeTextureLife(i, false, 0xFF);
        }
    }

    mpLifeParent->scale(g_drawHIO.mLifeParentScale, g_drawHIO.mLifeParentScale);

    for (int i = 0; i < 20; i++) {
        mpHeartMark[i]->scale(g_drawHIO.mHeartMarkScale, g_drawHIO.mHeartMarkScale);
    }

    for (int i = 0; i < 20; i++) {
        mpBigHeart->scale(g_drawHIO.mBigHeartScale, g_drawHIO.mBigHeartScale);
    }

    mpLifeParent->paneTrans(i_posX, i_posY);
}

void dMeter2Draw_c::setAlphaLifeChange(bool param_0) {
    bool var_r31 = false;
    bool var_r30 = false;
    bool var_r29 = false;
    bool var_r28 = false;

    if (mLifeParentAlpha != mpLifeParent->getAlphaRate() || param_0) {
        mLifeParentAlpha = mpLifeParent->getAlphaRate();
        var_r31 = true;
    }

    if (mLifeParentHeartAlpha != g_drawHIO.mLifeParentHeartAlpha || param_0) {
        mLifeParentHeartAlpha = g_drawHIO.mLifeParentHeartAlpha;
        var_r30 = true;
    }

    if (mHeartAlpha != g_drawHIO.mHeartAlpha || param_0) {
        mHeartAlpha = g_drawHIO.mHeartAlpha;
        var_r30 = true;
    }

    if (mHeartBaseAlpha != g_drawHIO.mHeartBaseAlpha || param_0) {
        mHeartBaseAlpha = g_drawHIO.mHeartBaseAlpha;
        var_r29 = true;
    }

    if (mBigHeartAlpha != g_drawHIO.mBigHeartAlpha || param_0) {
        mBigHeartAlpha = g_drawHIO.mBigHeartAlpha;
        var_r28 = true;
    }

    if (var_r31 || var_r30 || param_0) {
        for (int i = 0; i < 20; i++) {
            mpHeartMark[i]->setAlphaRate((mHeartAlpha * mLifeParentHeartAlpha) * mLifeParentAlpha);
        }
    }

    if (var_r31 || var_r29 || param_0) {
        for (int i = 0; i < 20; i++) {
            mpHeartBase[i]->setAlphaRate(mHeartBaseAlpha * mLifeParentAlpha);
        }
    }

    if (var_r31 || var_r28 || param_0) {
        mpBigHeart->setAlphaRate(mBigHeartAlpha * mLifeParentAlpha);
    }
}

void dMeter2Draw_c::setAlphaLifeAnimeMin() {
    if (mpLifeParent->getAlphaRate() != 0.0f) {
        mpLifeParent->setAlphaRate(g_drawHIO.mParentAlpha);
        setAlphaAnimeMin(mpLifeParent, 5);
    }
}

void dMeter2Draw_c::setAlphaLifeAnimeMax() {
    if (mpLifeParent->getAlphaRate() != g_drawHIO.mParentAlpha) {
        mpLifeParent->setAlphaRate(g_drawHIO.mParentAlpha);
        setAlphaAnimeMax(mpLifeParent, 5);
    }
}

void dMeter2Draw_c::drawKanteraScreen(u8 i_meterType) {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();

    mpMagicParent->setAlphaRate(mMeterAlphaRate[i_meterType]);

    if (i_meterType == 0) {
        JUtility::TColor black = mpMagicMeter->getInitBlack();
        black.a = 255;

        mpMagicMeter->setBlackWhite(black, mpMagicMeter->getInitWhite());
        setAlphaMagicChange(true);
    } else if (i_meterType == 1) {
        mpMagicMeter->setBlackWhite(JUtility::TColor(255, 255, 140, 255),
                                    JUtility::TColor(230, 170, 0, 255));
        setAlphaKanteraChange(true);
    } else if (i_meterType == 2) {
        f32 oxygen_percent = (f32)dComIfGp_getOxygen() / (f32)dComIfGp_getMaxOxygen();

        if (oxygen_percent <= 0.25f) {
            mpMagicMeter->setBlackWhite(JUtility::TColor(255, 100, 100, 255),
                                        JUtility::TColor(255, 10, 10, 255));
            playOxygenBpkAnimation(mpOxygenBpk[0]);

            if (mMeterAlphaRate[i_meterType] > 0.0f) {
                Z2GetAudioMgr()->seStartLevel(Z2SE_SWIM_TIMER_BLINK_2, NULL, 0, 0, 1.0f, 1.0f,
                                              -1.0f, -1.0f, 0);
            }
        } else if (oxygen_percent <= 0.5f) {
            mpMagicMeter->setBlackWhite(JUtility::TColor(200, 200, 255, 255),
                                        JUtility::TColor(80, 180, 255, 255));
            playOxygenBpkAnimation(mpOxygenBpk[1]);

            if (mMeterAlphaRate[i_meterType] > 0.0f) {
                Z2GetAudioMgr()->seStartLevel(Z2SE_SWIM_TIMER_BLINK_1, NULL, 0, 0, 1.0f, 1.0f,
                                              -1.0f, -1.0f, 0);
            }
        } else {
            mpMagicMeter->setBlackWhite(JUtility::TColor(200, 200, 255, 255),
                                        JUtility::TColor(80, 180, 255, 255));
        }

        setAlphaOxygenChange(true);
    }

    mpMagicMeter->resize(field_0x584[i_meterType], field_0x590[i_meterType]);
    mpMagicFrameR->move(field_0x59c[i_meterType], field_0x5a8[i_meterType]);
    mpMagicBase->resize(field_0x5b4[i_meterType], field_0x5c0[i_meterType]);
    mpMagicParent->scale(field_0x5cc[i_meterType], field_0x5d8[i_meterType]);
    mpMagicParent->paneTrans(field_0x5e4[i_meterType], field_0x5f0[i_meterType]);
    mpKanteraScreen->draw(0.0f, 0.0f, graf_ctx);
}

void dMeter2Draw_c::drawMagic(s16 i_max, s16 i_magic, f32 i_posX, f32 i_posY) {
    f32 temp_f30 = mpMagicFrameL->getInitPosX();
    f32 temp_f31 = mpMagicFrameR->getInitPosX() - temp_f30;

    field_0x584[0] = i_magic * mpMagicMeter->getInitSizeX() / 32;
    field_0x590[0] = mpMagicMeter->getInitSizeY();

    field_0x59c[0] = (temp_f31 * (f32)i_max / 32) + mpMagicFrameL->getInitPosX();
    field_0x5a8[0] = mpMagicFrameL->getInitPosY();

    field_0x5b4[0] = (f32)i_max * mpMagicBase->getInitSizeX() / 32;
    field_0x5c0[0] = mpMagicBase->getInitSizeY();

    field_0x5cc[0] = g_drawHIO.mMagicMeterScale;
    field_0x5d8[0] = g_drawHIO.mMagicMeterScale;

    field_0x5e4[0] = i_posX;
    field_0x5f0[0] = i_posY;
}

void dMeter2Draw_c::setAlphaMagicChange(bool i_forceSet) {
    bool meter_parent_alpha_set = false;
    bool meter_alpha_set = false;
    bool meter_frame_alpha_set = false;

    if (field_0x7a4 != mpMagicParent->getAlphaRate() || i_forceSet) {
        field_0x7a4 = mpMagicParent->getAlphaRate();
        meter_parent_alpha_set = true;
    }

    if (mMagicMeterAlpha != g_drawHIO.mMagicMeterAlpha || i_forceSet) {
        mMagicMeterAlpha = g_drawHIO.mMagicMeterAlpha;
        meter_alpha_set = true;
    }

    if (mMagicMeterFrameAlpha != g_drawHIO.mMagicMeterFrameAlpha || i_forceSet) {
        mMagicMeterFrameAlpha = g_drawHIO.mMagicMeterFrameAlpha;
        meter_frame_alpha_set = true;
    }

    if (meter_parent_alpha_set || meter_alpha_set || i_forceSet) {
        mpMagicMeter->setAlphaRate(mMagicMeterAlpha * field_0x7a4);
    }

    if (meter_parent_alpha_set || meter_frame_alpha_set || i_forceSet) {
        mpMagicFrameL->setAlphaRate(mMagicMeterFrameAlpha * field_0x7a4);
        mpMagicFrameR->setAlphaRate(mMagicMeterFrameAlpha * field_0x7a4);
    }
}

// Fake function to fix float literal order
static f32 dummyLiteralOrder() {
    return 5.0f;
}

void dMeter2Draw_c::drawKantera(s32 i_max, s32 i_oil, f32 i_posX, f32 i_posY) {
    f32 var_f6 = mpMagicFrameR->getInitPosX() - mpMagicFrameL->getInitPosX();
    f32 var_f7 = 0.0f;
    f32 var_f4 = 0.0f;

    if (dComIfGs_getMaxOil() != 0) {
        var_f7 = (f32)i_oil / (f32)dComIfGs_getMaxOil();
        var_f4 = (f32)i_max / (f32)dComIfGs_getMaxOil();
    }

    field_0x584[1] = var_f7 * mpMagicMeter->getInitSizeX();
    field_0x590[1] = mpMagicMeter->getInitSizeY();
    field_0x59c[1] = (var_f6 * var_f4) + mpMagicFrameL->getInitPosX();
    field_0x5a8[1] = mpMagicFrameL->getInitPosY();
    field_0x5b4[1] = var_f4 * mpMagicBase->getInitSizeX();
    field_0x5c0[1] = mpMagicBase->getInitSizeY();
    field_0x5cc[1] = g_drawHIO.mLanternMeterScale;
    field_0x5d8[1] = g_drawHIO.mLanternMeterScale;
    field_0x5e4[1] = i_posX;
    field_0x5f0[1] = i_posY;
}

void dMeter2Draw_c::setAlphaKanteraChange(bool i_forceSet) {
    bool meter_parent_alpha_set = false;
    bool meter_alpha_set = false;
    bool meter_frame_alpha_set = false;

    if (field_0x7b0 != mpMagicParent->getAlphaRate() || i_forceSet) {
        field_0x7b0 = mpMagicParent->getAlphaRate();
        meter_parent_alpha_set = true;
    }

    if (mLanternMeterAlpha != g_drawHIO.mLanternMeterAlpha || i_forceSet) {
        mLanternMeterAlpha = g_drawHIO.mLanternMeterAlpha;
        meter_alpha_set = true;
    }

    if (mLanternMeterFrameAlpha != g_drawHIO.mLanternMeterFrameAlpha || i_forceSet) {
        mLanternMeterFrameAlpha = g_drawHIO.mLanternMeterFrameAlpha;
        meter_frame_alpha_set = true;
    }

    if (meter_parent_alpha_set || meter_alpha_set || i_forceSet) {
        mpMagicMeter->setAlphaRate(mLanternMeterAlpha * field_0x7b0);
    }

    if (meter_parent_alpha_set || meter_frame_alpha_set || i_forceSet) {
        mpMagicFrameL->setAlphaRate(mLanternMeterFrameAlpha * field_0x7b0);
        mpMagicFrameR->setAlphaRate(mLanternMeterFrameAlpha * field_0x7b0);
    }
}

void dMeter2Draw_c::setAlphaKanteraAnimeMin() {
    if (field_0x742[1] <= 0) {
        mMeterAlphaRate[1] = 0.0f;
    } else {
        field_0x742[1]--;
        if (field_0x742[1] < 0) {
            field_0x742[1] = 0;
        }

        mMeterAlphaRate[1] = (field_0x742[1] / 5.0f) * g_drawHIO.mParentAlpha;
    }
}

void dMeter2Draw_c::setAlphaKanteraAnimeMax() {
    if (field_0x742[1] >= 5) {
        mMeterAlphaRate[1] = g_drawHIO.mParentAlpha;
    } else {
        field_0x742[1]++;
        if (field_0x742[1] > 5) {
            field_0x742[1] = 5;
        }

        mMeterAlphaRate[1] = (field_0x742[1] / 5.0f) * g_drawHIO.mParentAlpha;
    }
}

void dMeter2Draw_c::drawOxygen(s32 i_max, s32 i_oxygen, f32 i_posX, f32 i_posY) {
    f32 var_f6 = mpMagicFrameR->getInitPosX() - mpMagicFrameL->getInitPosX();
    f32 var_f7 = 0.0f;
    f32 var_f4 = 0.0f;

    if (dComIfGp_getMaxOxygen() > 0) {
        var_f7 = (f32)i_oxygen / (f32)dComIfGp_getMaxOxygen();
        var_f4 = (f32)i_max / (f32)dComIfGp_getMaxOxygen();
    }

    field_0x584[2] = var_f7 * mpMagicMeter->getInitSizeX();
    field_0x590[2] = mpMagicMeter->getInitSizeY();
    field_0x59c[2] = (var_f6 * var_f4) + mpMagicFrameL->getInitPosX();
    field_0x5a8[2] = mpMagicFrameL->getInitPosY();
    field_0x5b4[2] = var_f4 * mpMagicBase->getInitSizeX();
    field_0x5c0[2] = mpMagicBase->getInitSizeY();
    field_0x5cc[2] = g_drawHIO.mOxygenMeterScale;
    field_0x5d8[2] = g_drawHIO.mOxygenMeterScale;
    field_0x5e4[2] = i_posX;
    field_0x5f0[2] = i_posY;
}

void dMeter2Draw_c::setAlphaOxygenChange(bool i_forceSet) {
    bool meter_parent_alpha_set = false;
    bool meter_alpha_set = false;
    bool meter_frame_alpha_set = false;

    if (field_0x7bc != mpMagicParent->getAlphaRate() || i_forceSet) {
        field_0x7bc = mpMagicParent->getAlphaRate();
        meter_parent_alpha_set = true;
    }

    if (mOxygenMeterAlpha != g_drawHIO.mOxygenMeterAlpha || i_forceSet) {
        mOxygenMeterAlpha = g_drawHIO.mOxygenMeterAlpha;
        meter_alpha_set = true;
    }

    if (mOxygenMeterFrameAlpha != g_drawHIO.mOxygenMeterFrameAlpha || i_forceSet) {
        mOxygenMeterFrameAlpha = g_drawHIO.mOxygenMeterFrameAlpha;
        meter_frame_alpha_set = true;
    }

    if (meter_parent_alpha_set || meter_alpha_set || i_forceSet) {
        mpMagicMeter->setAlphaRate(mOxygenMeterAlpha * field_0x7bc);
    }

    if (meter_parent_alpha_set || meter_frame_alpha_set || i_forceSet) {
        mpMagicFrameL->setAlphaRate(mOxygenMeterFrameAlpha * field_0x7bc);
        mpMagicFrameR->setAlphaRate(mOxygenMeterFrameAlpha * field_0x7bc);
    }
}

void dMeter2Draw_c::setAlphaOxygenAnimeMin() {
    if (field_0x742[2] <= 0) {
        mMeterAlphaRate[2] = 0.0f;
    } else {
        field_0x742[2]--;
        if (field_0x742[2] < 0) {
            field_0x742[2] = 0;
        }

        mMeterAlphaRate[2] = (field_0x742[2] / 5.0f) * g_drawHIO.mParentAlpha;
    }
}

void dMeter2Draw_c::setAlphaOxygenAnimeMax() {
    if (field_0x742[2] >= 5) {
        mMeterAlphaRate[2] = g_drawHIO.mParentAlpha;
    } else {
        field_0x742[2]++;
        if (field_0x742[2] > 5) {
            field_0x742[2] = 5;
        }

        mMeterAlphaRate[2] = (field_0x742[2] / 5.0f) * g_drawHIO.mParentAlpha;
    }
}

void dMeter2Draw_c::drawLightDrop(u8 i_num, u8 i_needNum, f32 i_posX, f32 i_posY, f32 i_vesselScale,
                                  f32 param_5, u8 param_6) {
    for (int i = 0; i < 16; i++) {
        if (mpSIParts[i][0] != NULL) {
            mpSIParts[i][0]->scale(g_drawHIO.mLightDrop.mDropScale,
                                   g_drawHIO.mLightDrop.mDropScale);
        } else {
            mpSIParts[i][1]->scale(g_drawHIO.mLightDrop.mDropScale,
                                   g_drawHIO.mLightDrop.mDropScale);
            mpSIParts[i][2]->scale(g_drawHIO.mLightDrop.mDropScale,
                                   g_drawHIO.mLightDrop.mDropScale);
        }

        if (i < i_num) {
            static_cast<J2DPicture*>(mpSIParts[i][1]->getPanePtr())
                ->setBlackWhite(g_drawHIO.mLightDrop.mDropOnColorBlack,
                                g_drawHIO.mLightDrop.mDropOnColorWhite);
            mpSIParts[i][1]->setAlphaRate(mpSIParent[1]->getAlphaRate());
            mpSIParts[i][2]->setAlphaRate(mpSIParent[1]->getAlphaRate());
        } else {
            static_cast<J2DPicture*>(mpSIParts[i][1]->getPanePtr())
                ->setBlackWhite(g_drawHIO.mLightDrop.mDropOffColorBlack,
                                g_drawHIO.mLightDrop.mDropOffColorWhite);
            mpSIParts[i][1]->setAlphaRate(mpSIParent[1]->getAlphaRate() * 0.3f);
            mpSIParts[i][2]->setAlphaRate(mpSIParent[1]->getAlphaRate() * 0.3f);
        }
    }

    if (param_6 == 1) {
        field_0x760 = param_6;
        field_0x756 = -1;

        if (field_0x620[i_num + 2] == 0.0f) {
            field_0x620[i_num + 2] = 18.0f;
        }
    }

    if (param_6 == 2) {
        field_0x760 = param_6;
        field_0x756 = 0;

        if (field_0x62c[0] == 0.0f) {
            field_0x62c[0] = 18.0f;
        }
    }

    field_0x6fc = param_5;
    mLightDropVesselScale = i_vesselScale;
    mpLightDropParent->scale(mLightDropVesselScale * field_0x6f8,
                             mLightDropVesselScale * field_0x6f8);
    mpLightDropParent->paneTrans(i_posX, i_posY);
}

void dMeter2Draw_c::setAlphaLightDropChange(bool unused) {}

f32 dMeter2Draw_c::getNowLightDropRateCalc() {
    f32 drop_num = dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea());
    f32 need_drop_num = dComIfGp_getNeedLightDropNum();
    f32 rate;

    if (need_drop_num <= 0) {
        rate = 0.0f;
    } else if (drop_num >= need_drop_num) {
        rate = 1.0f;
    } else {
        rate = drop_num / need_drop_num;
    }

    return rate;
}

void dMeter2Draw_c::setAlphaLightDropAnimeMin() {
    if (mpLightDropParent->getAlphaRate() != 0.0f) {
        mpLightDropParent->setAlphaRate(g_drawHIO.mParentAlpha * field_0x6fc);
        setAlphaAnimeMin(mpLightDropParent, 5);

        for (int i = 0; i < 2; i++) {
            mpSIParent[i]->setAlphaRate(g_drawHIO.mLightDrop.mVesselAlpha[i + 2] *
                                        mpLightDropParent->getAlphaRate());
        }

        for (int i = 0; i < 16; i++) {
            if (i < dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea())) {
                mpSIParts[i][1]->setAlphaRate(mpSIParent[1]->getAlphaRate());
                mpSIParts[i][2]->setAlphaRate(mpSIParent[1]->getAlphaRate());
            } else {
                mpSIParts[i][1]->setAlphaRate(mpSIParent[1]->getAlphaRate() * 0.3f);
                mpSIParts[i][2]->setAlphaRate(mpSIParent[1]->getAlphaRate() * 0.3f);
            }
        }
    }
}

void dMeter2Draw_c::setAlphaLightDropAnimeMax() {
    f32 temp_f31 = g_drawHIO.mParentAlpha * field_0x6fc;

    if (dMeter2Info_getLightDropGetFlag(dComIfGp_getStartStageDarkArea()) > 1 &&
        dMeter2Info_getLightDropGetFlag(dComIfGp_getStartStageDarkArea()) != 0xFF)
    {
        if (mpLightDropParent->getAlphaTimer() == 0) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_LIGHT_POT_EQUIP, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }

        if (temp_f31 != mpLightDropParent->getAlphaRate()) {
            mpLightDropParent->setAlphaRate(temp_f31);
            setAlphaAnimeMax(mpLightDropParent, g_drawHIO.mLightDrop.mDropGetScaleAnimFrameNum);

            for (int i = 0; i < 2; i++) {
                mpSIParent[i]->setAlphaRate(g_drawHIO.mLightDrop.mVesselAlpha[i + 2] *
                                            mpLightDropParent->getAlphaRate());
            }
        }

        if (g_drawHIO.mLightDrop.mDropGetScaleAnimFrameNum > 1) {
            field_0x6f8 = 1.0f + (((g_drawHIO.mLightDrop.mDropGetScale - 1.0f) *
                                   ((f32)g_drawHIO.mLightDrop.mDropGetScaleAnimFrameNum -
                                    (f32)mpLightDropParent->getAlphaTimer())) /
                                  ((f32)g_drawHIO.mLightDrop.mDropGetScaleAnimFrameNum - 1.0f));
        } else {
            field_0x6f8 = 1.0f;
        }

        mpLightDropParent->scale(mLightDropVesselScale * field_0x6f8,
                                 mLightDropVesselScale * field_0x6f8);

        if (g_drawHIO.mLightDrop.mDropGetScaleAnimFrameNum == mpLightDropParent->getAlphaTimer()) {
            dMeter2Info_setLightDropGetFlag(dComIfGp_getStartStageDarkArea(), 0xFF);
        }
    } else if (temp_f31 != mpLightDropParent->getAlphaRate()) {
        mpLightDropParent->setAlphaRate(temp_f31);
        setAlphaAnimeMax(mpLightDropParent, 5);

        for (int i = 0; i < 2; i++) {
            mpSIParent[i]->setAlphaRate(g_drawHIO.mLightDrop.mVesselAlpha[i + 2] *
                                        mpLightDropParent->getAlphaRate());
        }
    } else {
        for (int i = 0; i < 2; i++) {
            if (field_0x860[i] != g_drawHIO.mLightDrop.mVesselAlpha[i + 2]) {
                field_0x860[i] = g_drawHIO.mLightDrop.mVesselAlpha[i + 2];
                mpSIParent[i]->setAlphaRate(g_drawHIO.mLightDrop.mVesselAlpha[i + 2] *
                                            mpLightDropParent->getAlphaRate());
            }
        }
    }

    for (int i = 0; i < 16; i++) {
        if (i < dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea())) {
            mpSIParts[i][1]->setAlphaRate(mpSIParent[1]->getAlphaRate());
            mpSIParts[i][2]->setAlphaRate(mpSIParent[1]->getAlphaRate());
        } else {
            mpSIParts[i][1]->setAlphaRate(mpSIParent[1]->getAlphaRate() * 0.3f);
            mpSIParts[i][2]->setAlphaRate(mpSIParent[1]->getAlphaRate() * 0.3f);
        }
    }
}

void dMeter2Draw_c::drawRupee(s16 i_rupeeNum) {
    mpRupeeTexture[3][0]->hide();
    mpRupeeTexture[3][1]->hide();

    // digits are descending order (3, 2, 1, 0)
    int digit_3 = i_rupeeNum / 1000;
    int num = i_rupeeNum % 1000;

    if (i_rupeeNum < 1000) {
        mpRupeeTexture[3][0]->hide();
        mpRupeeTexture[3][1]->hide();
    } else {
        mpRupeeTexture[3][0]->show();
        mpRupeeTexture[3][1]->show();

        ResTIMG* timg = getNumberTexture(digit_3);
        static_cast<J2DPicture*>(mpRupeeTexture[3][0]->getPanePtr())->changeTexture(timg, 0);
        static_cast<J2DPicture*>(mpRupeeTexture[3][1]->getPanePtr())->changeTexture(timg, 0);
    }
    int digit_2 = num / 100;
    num %= 100;

    ResTIMG* timg = getNumberTexture(digit_2);
    static_cast<J2DPicture*>(mpRupeeTexture[2][0]->getPanePtr())->changeTexture(timg, 0);
    static_cast<J2DPicture*>(mpRupeeTexture[2][1]->getPanePtr())->changeTexture(timg, 0);

    int digit_1 = num / 10;
    num %= 10;

    timg = getNumberTexture(digit_1);
    static_cast<J2DPicture*>(mpRupeeTexture[1][0]->getPanePtr())->changeTexture(timg, 0);
    static_cast<J2DPicture*>(mpRupeeTexture[1][1]->getPanePtr())->changeTexture(timg, 0);

    timg = getNumberTexture(num);
    static_cast<J2DPicture*>(mpRupeeTexture[0][0]->getPanePtr())->changeTexture(timg, 0);
    static_cast<J2DPicture*>(mpRupeeTexture[0][1]->getPanePtr())->changeTexture(timg, 0);

    mpRupeeKeyParent->scale(g_drawHIO.mRupeeKeyScale * field_0x718,
                            g_drawHIO.mRupeeKeyScale * field_0x718);
    mpRupeeKeyParent->paneTrans(g_drawHIO.mRupeeKeyPosX, g_drawHIO.mRupeeKeyPosY);

    mpRupeeParent[0]->scale(g_drawHIO.mRupeeScale, g_drawHIO.mRupeeScale);
    mpRupeeParent[0]->paneTrans(g_drawHIO.mRupeePosX, g_drawHIO.mRupeePosY);

    mpRupeeParent[1]->scale(g_drawHIO.mRupeeFramePosY, g_drawHIO.mRupeeFramePosY);
    mpRupeeParent[1]->paneTrans(g_drawHIO.mRupeeFrameScale, g_drawHIO.mRupeeFramePosX);

    mpRupeeParent[2]->scale(g_drawHIO.mRupeeFramePosY, g_drawHIO.mRupeeFramePosY);
    mpRupeeParent[2]->paneTrans(g_drawHIO.mRupeeFrameScale, g_drawHIO.mRupeeFramePosX);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            mpRupeeTexture[i][j]->scale(g_drawHIO.mRupeeCountScale, g_drawHIO.mRupeeCountScale);
            mpRupeeTexture[i][j]->paneTrans(g_drawHIO.mRupeeCountPosX, g_drawHIO.mRupeeCountPosY);
        }
    }
}

void dMeter2Draw_c::setAlphaRupeeChange(bool param_0) {
    bool set_parent = false;
    bool set_rupeekey = false;
    bool set_rupee = false;
    bool set_rupeeframe = false;
    bool set_rupeecount = false;

    if (field_0x7cc != g_drawHIO.mParentAlpha || param_0) {
        field_0x7cc = g_drawHIO.mParentAlpha;
        set_parent = true;
    }

    if (field_0x7d0 != g_drawHIO.mRupeeKeyAlpha || param_0) {
        field_0x7d0 = g_drawHIO.mRupeeKeyAlpha;
        set_rupeekey = true;
    }

    if (mRupeeAlpha != g_drawHIO.mRupeeAlpha || param_0) {
        mRupeeAlpha = g_drawHIO.mRupeeAlpha;
        set_rupee = true;
    }

    if (mRupeeFrameAlpha != g_drawHIO.mRupeeFrameAlpha || param_0) {
        mRupeeFrameAlpha = g_drawHIO.mRupeeFrameAlpha;
        set_rupeeframe = true;
    }

    if (mRupeeCountAlpha != g_drawHIO.mRupeeCountAlpha || param_0) {
        mRupeeCountAlpha = g_drawHIO.mRupeeCountAlpha;
        set_rupeecount = true;
    }

    if (set_parent || set_rupeekey || param_0) {
        mpRupeeKeyParent->setAlphaRate(field_0x7cc * field_0x7d0);
    }

    if (set_parent || set_rupeekey || set_rupee || param_0) {
        mpRupeeParent[0]->setAlphaRate(field_0x7d0 * (mRupeeAlpha * field_0x7cc));
    }

    if (set_parent || set_rupeekey || set_rupeeframe || param_0) {
        mpRupeeParent[1]->setAlphaRate(field_0x7d0 * (mRupeeFrameAlpha * field_0x7cc));
        mpRupeeParent[2]->setAlphaRate(field_0x7d0 * (mRupeeFrameAlpha * field_0x7cc));
    }

    if (set_parent || set_rupeekey || set_rupeecount || param_0) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                mpRupeeTexture[i][j]->setAlphaRate(
                    field_0x7d0 * (field_0x7cc * (mRupeeCountAlpha * mRupeeAlpha)));
            }
        }
    }
}

void dMeter2Draw_c::setAlphaRupeeAnimeMin() {
    f32 alphas[3];
    alphas[0] = g_drawHIO.mRupeeAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mRupeeKeyAlpha);
    alphas[1] = g_drawHIO.mRupeeFrameAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mRupeeKeyAlpha);
    alphas[2] = alphas[1];

    for (int i = 0; i < 3; i++) {
        if (mpRupeeParent[i]->getAlphaRate() != 0.0f) {
            mpRupeeParent[i]->setAlphaRate(alphas[i]);
            setAlphaAnimeMin(mpRupeeParent[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaRupeeAnimeMax() {
    f32 alphas[3];
    alphas[0] = g_drawHIO.mRupeeAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mRupeeKeyAlpha);
    alphas[1] = g_drawHIO.mRupeeFrameAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mRupeeKeyAlpha);
    alphas[2] = alphas[1];

    for (int i = 0; i < 3; i++) {
        if (mpRupeeParent[i]->getAlphaRate() != alphas[i]) {
            mpRupeeParent[i]->setAlphaRate(alphas[i]);
            setAlphaAnimeMax(mpRupeeParent[i], 5);
        }
    }
}

void dMeter2Draw_c::drawKey(s16 i_keyNum) {
    if (i_keyNum > 5) {
        i_keyNum = 5;
    }

    for (int i = 0; i < 5; i++) {
        if (i < i_keyNum) {
            if (mpKeyTexture[i]->isVisible() == false) {
                mpKeyTexture[i]->show();
            }
        } else {
            if (mpKeyTexture[i]->isVisible() == true) {
                mpKeyTexture[i]->hide();
            }
        }
    }

    mpKeyParent->scale(g_drawHIO.mKeyScale, g_drawHIO.mKeyScale);
    mpKeyParent->paneTrans(g_drawHIO.mKeyPosX, g_drawHIO.mKeyPosY);
}

void dMeter2Draw_c::setAlphaKeyChange(bool param_0) {
    bool set_parent = false;
    bool set_rupeekey = false;
    bool set_key = false;
    bool set_keynum = false;

    if (field_0x7e0 != g_drawHIO.mParentAlpha || param_0) {
        field_0x7e0 = g_drawHIO.mParentAlpha;
        set_parent = true;
    }

    if (field_0x7e4 != g_drawHIO.mRupeeKeyAlpha || param_0) {
        field_0x7e4 = g_drawHIO.mRupeeKeyAlpha;
        set_rupeekey = true;
    }

    if (mKeyAlpha != g_drawHIO.mKeyAlpha || param_0) {
        mKeyAlpha = g_drawHIO.mKeyAlpha;
        set_key = true;
    }

    if (mKeyNumAlpha != g_drawHIO.mKeyNumAlpha || param_0) {
        mKeyNumAlpha = g_drawHIO.mKeyNumAlpha;
        set_keynum = true;
    }

    if (set_parent || set_rupeekey || set_key || param_0) {
        mpKeyParent->setAlphaRate(field_0x7e0 * (mKeyAlpha * field_0x7e4));
    }

    if (set_parent || set_rupeekey || set_keynum || param_0) {
        for (int i = 0; i < 5; i++) {
            mpKeyTexture[i]->setAlphaRate(field_0x7e0 * (field_0x7e4 * (mKeyNumAlpha * mKeyAlpha)));
        }
    }
}

void dMeter2Draw_c::setAlphaKeyAnimeMin() {
    f32 alpha = g_drawHIO.mKeyAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mRupeeKeyAlpha);

    if (mpKeyParent->getAlphaRate() != 0.0f) {
        mpKeyParent->setAlphaRate(alpha);
        setAlphaAnimeMin(mpKeyParent, 5);
    }
}

void dMeter2Draw_c::setAlphaKeyAnimeMax() {
    f32 alpha = g_drawHIO.mKeyAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mRupeeKeyAlpha);

    if (mpKeyParent->getAlphaRate() != alpha) {
        mpKeyParent->setAlphaRate(alpha);
        setAlphaAnimeMax(mpKeyParent, 5);
    }
}

void dMeter2Draw_c::drawButtonA(u8 i_action, f32 i_posX, f32 i_posY, f32 i_textPosX, f32 i_textPosY,
                                f32 i_scale, bool param_6, bool param_7) {
    char* mp_string = getActionString(i_action, 1, &field_0x761);
    f32 var_f31 = g_drawHIO.mButtonAScale;
    f32 var_f30 = g_drawHIO.mButtonATextScale;

    if (isBButtonShow(false)) {
        var_f31 = g_drawHIO.mButtonATalkScale[0];
        var_f30 = g_drawHIO.mButtonATalkScale[1];
    } else if (param_7) {
        var_f31 = g_drawHIO.mButtonATalkAScale;
        var_f30 = g_drawHIO.mButtonATextTalkAScale;
    }

    if (dComIfGp_isDoSetFlag(2) || dComIfGp_isDoSetFlag(4)) {
        field_0x761 = 7;
    }

    if (param_6) {
        if (i_action == 0x25) {
            if (field_0x608 == 0.0f) {
                field_0x608 = 18.0f - g_drawHIO.mButtonAPikariAnimSpeed;
            }
            field_0x759 = 1;
        } else {
            if (i_action != 0) {
                field_0x608 = 18.0f - g_drawHIO.mButtonAPikariAnimSpeed;
            }
            field_0x759 = 0;
        }
    }

    if (*mp_string != 0 && ((dComIfGp_getDoStatus() != dComIfGp_getAStatus() &&
                             dComIfGp_getDoStatus() != dComIfGp_getAStatusForce()) ||
                            !isEmphasisA()))
    {
        mpTextA->show();
        mpTextA->show();
    } else {
        mpTextA->hide();
    }

    JUT_ASSERT(0, strlen(mp_string) < (64));

    if (daPy_getPlayerActorClass()->getSumouMode()) {
        mpTextA->show();
        mp_string = getActionString(0x15, 1, NULL);

        for (int i = 0; i < 5; i++) {
            strcpy(static_cast<J2DTextBox*>(mpAText[i]->getPanePtr())->getStringPtr(), mp_string);
        }
    } else {
        for (int i = 0; i < 5; i++) {
            strcpy(static_cast<J2DTextBox*>(mpAText[i]->getPanePtr())->getStringPtr(), mp_string);
        }
    }

    mpButtonA->scale(var_f31 * i_scale, var_f31 * i_scale);
    mpButtonA->paneTrans(i_posX, i_posY);
    mpTextA->scale(var_f30 * i_scale, var_f30 * i_scale);
    mpTextA->paneTrans(g_drawHIO.mButtonATextPosX + i_textPosX,
                       g_drawHIO.mButtonATextPosY + i_textPosY);
}

void dMeter2Draw_c::drawButtonB(u8 i_action, bool param_1, f32 i_posX, f32 i_posY, f32 i_textPosX,
                                f32 i_textPosY, f32 i_scale, bool param_7) {
    int var_r31 = 0;
    f32 var_f31 = g_drawHIO.mButtonBScale;
    f32 var_f30 = g_drawHIO.mButtonBFontScale;

    if (isBButtonShow(true)) {
        var_f31 = g_drawHIO.mButtonBTalkScale[0];
        var_f30 = g_drawHIO.mButtonBTalkScale[1];
    }

    field_0x6ec = i_posX;
    field_0x6f0 = i_posY;

    char* mp_string = getActionString(i_action, 1, &field_0x762);

    if (dComIfGp_isASetFlag(2) || dComIfGp_isASetFlag(4)) {
        field_0x762 = 7;
    }

    if (*mp_string != 0 && i_action != 0 && i_action != 0x2E &&
        ((dComIfGp_getDoStatus() == 0 ||
          (dComIfGp_getDoStatus() != dComIfGp_getAStatus() &&
           dComIfGp_getDoStatus() != dComIfGp_getAStatusForce())) ||
         !isEmphasisB()))
    {
        mpTextB->show();
    } else {
        mpTextB->hide();
    }

    if (param_7) {
        if (i_action == 0x10) {
            if (field_0x60c == 0.0f) {
                field_0x60c = 18.0f - g_drawHIO.mButtonBPikariAnimSpeed;
            }
            field_0x75a = 1;
        } else {
            if (i_action != 0 && field_0x60c == 0.0f) {
                field_0x60c = 18.0f - g_drawHIO.mButtonBPikariAnimSpeed;
            }
            field_0x75a = 0;
        }
    }

    JUT_ASSERT(0, strlen(mp_string) < (64));

    for (int i = 0; i < 5; i++) {
        strcpy(static_cast<J2DTextBox*>(mpBText[i]->getPanePtr())->getStringPtr(), mp_string);
    }

    if (i_action == 0x26 || i_action == 0x2E) {
        mpScreen->search('item_b_n')->show();
        var_r31 = 1;

        if (mButtonBItem != dComIfGs_getSelectEquipSword()) {
            mButtonBItem = dComIfGs_getSelectEquipSword();

            switch (dComIfGs_getSelectEquipSword()) {
            case fpcNm_ITEM_WOOD_STICK:
            case fpcNm_ITEM_SWORD:
            case fpcNm_ITEM_MASTER_SWORD:
            case fpcNm_ITEM_LIGHT_SWORD:
                changeTextureItemB(mButtonBItem);
                break;
            default:
                mpScreen->search('item_b_n')->hide();
                var_r31 = 0;
            }
        }
    } else if (param_1 == true && i_action == 0x4F) {
        mpScreen->search('item_b_n')->show();
        mButtonBItem = fpcNm_ITEM_LURE_ROD;
        changeTextureItemB(fpcNm_ITEM_LURE_ROD);
        var_r31 = 2;
    } else {
        mpScreen->search('item_b_n')->hide();
    }

    mpItemB->getPanePtr()->rotate(mpItemB->getSizeX() * 0.5f, mpItemB->getSizeY() * 0.5f, ROTATE_Z,
                                  g_drawHIO.mButtonBItemRotation[var_r31]);

    field_0x730 = var_f31 * i_scale;
    mpButtonB->scale(field_0x730 * field_0x734, field_0x730 * field_0x734);
    mpButtonB->paneTrans(g_drawHIO.mButtonBPosX + i_posX, g_drawHIO.mButtonBPosY + i_posY);

    field_0x728 = g_drawHIO.mButtonBItemScale[var_r31] * i_scale;
    mpItemB->scale(field_0x728 * field_0x734, field_0x728 * field_0x734);
    mpItemB->paneTrans(field_0x6dc + (g_drawHIO.mButtonBItemPosX[var_r31] + i_posX),
                       field_0x6e0 + (g_drawHIO.mButtonBItemPosY[var_r31] + i_posY));

    field_0x72c = g_drawHIO.mItemBBaseScale[0] * i_scale;
    mpLightB->scale(field_0x72c * field_0x734, field_0x72c * field_0x734);
    mpLightB->paneTrans(g_drawHIO.mItemBBasePosX[0] + i_posX, g_drawHIO.mItemBBasePosY[0] + i_posY);

    mpTextB->scale(var_f30 * i_scale, var_f30 * i_scale);
    mpTextB->paneTrans(g_drawHIO.mButtonBFontPosX + i_textPosX,
                       g_drawHIO.mButtonBFontPosY + i_textPosY);
}

void dMeter2Draw_c::drawButtonR(u8 unused0, u8 i_action, bool unused1, bool unused2) {
    mpScreen->search('item_r_n')->hide();
    mpTextXY[2]->show();

    getActionString(i_action, 1, &field_0x768[2]);
    if (dComIfGp_isRSetFlag(2) || dComIfGp_isRSetFlag(4)) {
        field_0x768[2] = 7;
    }
}

void dMeter2Draw_c::drawButtonZ(u8 i_action) {
    char* mp_string = getActionString(i_action, 1, &field_0x764);

    if (dComIfGp_isZSetFlag(2) || dComIfGp_isZSetFlag(4)) {
        field_0x764 = 7;
    }

    if (i_action == 0x27) {
        mpTextXY[2]->hide();
        mpButtonMidona->hide();
    } else if (*mp_string != 0 && i_action != 0x2F && i_action != 8) {
        mpTextXY[2]->show();
        mpButtonMidona->hide();
    } else {
        mpTextXY[2]->hide();
        mpButtonMidona->show();
    }

    JUT_ASSERT(0, strlen(mp_string) < (64));

    for (int i = 0; i < 5; i++) {
        strcpy(static_cast<J2DTextBox*>(mpXYText[i][2]->getPanePtr())->getStringPtr(), mp_string);
    }

    mpButtonXY[2]->scale(g_drawHIO.mButtonZScale, g_drawHIO.mButtonZScale);
    mpButtonXY[2]->paneTrans(g_drawHIO.mButtonZPosX, g_drawHIO.mButtonZPosY);

    mpItemR->scale(g_drawHIO.mButtonZItemScale, g_drawHIO.mButtonZItemScale);
    mpItemR->paneTrans(g_drawHIO.mButtonZItemPosX + field_0x6ac[2],
                       g_drawHIO.mButtonZItemPosY + field_0x6b8[2]);

    mpLightXY[2]->scale(g_drawHIO.mButtonZItemBaseScale, g_drawHIO.mButtonZItemBaseScale);
    mpLightXY[2]->paneTrans(g_drawHIO.mButtonZItemBasePosX, g_drawHIO.mButtonZItemBasePosY);

    mpTextXY[2]->scale(g_drawHIO.mButtonZFontScale, g_drawHIO.mButtonZFontScale);
    mpTextXY[2]->paneTrans(g_drawHIO.mButtonZFontPosX, g_drawHIO.mButtonZFontPosY);
}

void dMeter2Draw_c::drawButton3D(u8 i_action) {
    getActionString(i_action, 1, &field_0x765);
    if (i_action != 0) {
        field_0x765 = 7;
    }
}

void dMeter2Draw_c::drawButtonC(u8 i_action, bool unused) {
    getActionString(i_action, 1, &field_0x766);
    if (i_action != 0 && (dComIfGp_isCStickSetFlag(2) & 1 || dComIfGp_isCStickSetFlag(4))) {
        field_0x766 = 7;
    }
}

void dMeter2Draw_c::drawButtonS(u8 i_action) {
    getActionString(i_action, 1, &field_0x767);
    if (i_action != 0) {
        field_0x767 = 7;
    }
}

void dMeter2Draw_c::drawButtonBin(u8 i_action) {
    int bottle_num = 0;
    for (int i = 0; i < 4; i++) {
        if (dComIfGs_getItem((u8)(SLOT_11 + i), true) != fpcNm_ITEM_NONE) {
            bottle_num++;
        }
    }

    if (bottle_num != 0) {
        getActionString(i_action, 1, &field_0x763);
        if (i_action != 0) {
            field_0x763 = 7;
        }
    }
}

void dMeter2Draw_c::drawButtonXY(int i_no, u8 i_itemNo, u8 i_action, bool param_3, bool param_4) {
    JUT_ASSERT(0, i_no < SELECT_MAX_e);

    static u64 const tag[] = {'item_x_n', 'item_y_n'};

    if (!param_3) {
        mpScreen->search(tag[i_no])->hide();

        int var_r26;
        if (i_no == SELECT_X_e) {
            var_r26 = dComIfGp_isXSetFlag(2) | dComIfGp_isXSetFlag(4);
        } else if (i_no == SELECT_Y_e) {
            var_r26 = dComIfGp_isYSetFlag(2) | dComIfGp_isYSetFlag(4);
        }

        char* mp_string = getActionString(i_action, 1, &field_0x768[i_no]);
        if (var_r26) {
            field_0x768[i_no] = 7;
        }

        if (param_4) {
            if (i_action == 0x10) {
                if (field_0x620[i_no] == 0.0f) {
                    field_0x620[i_no] = 18.0f - g_drawHIO.mButtonXYPikariAnimSpeed;
                }

                field_0x75c[i_no] = 1;
            } else {
                if (i_action != 0) {
                    field_0x620[i_no] = 18.0f - g_drawHIO.mButtonXYPikariAnimSpeed;
                }

                field_0x75c[i_no] = 0;
            }
        }

        if (*mp_string != 0) {
            mpTextXY[i_no]->show();
        } else {
            mpTextXY[i_no]->hide();
        }

        JUT_ASSERT(0, strlen(mp_string) < (64));

        for (int i = 0; i < 5; i++) {
            strcpy(static_cast<J2DTextBox*>(mpXYText[i][i_no]->getPanePtr())->getStringPtr(),
                   mp_string);
        }

        if (i_no == SELECT_X_e) {
            mpTextXY[i_no]->scale(g_drawHIO.mButtonXYTextScale, g_drawHIO.mButtonXYTextScale);
            mpTextXY[i_no]->paneTrans(g_drawHIO.mButtonXYTextPosX, g_drawHIO.mButtonXYTextPosY);
        } else if (i_no == SELECT_Y_e) {
            mpTextXY[i_no]->scale(g_drawHIO.mButtonXYTextScale, g_drawHIO.mButtonXYTextScale);
            mpTextXY[i_no]->paneTrans(g_drawHIO.mButtonXYTextPosX, g_drawHIO.mButtonXYTextPosY);
        }
    } else {
        mpScreen->search(tag[i_no])->show();
        mpTextXY[i_no]->hide();

        int var_r29;
        if (i_itemNo == fpcNm_ITEM_NONE || i_itemNo == 0) {
            mpScreen->search(tag[i_no])->hide();
            var_r29 = 1;
        } else {
            mpScreen->search(tag[i_no])->show();
            changeTextureItemXY(i_no, i_itemNo);
            var_r29 = 0;
        }

        switch (i_no) {
        case SELECT_X_e:
            setItemParamX(i_itemNo);
            break;
        case SELECT_Y_e:
            setItemParamY(i_itemNo);
            break;
        case SELECT_Z_e:
            setItemParamZ(i_itemNo);
            break;
        }

        mpItemXY[i_no]->getPanePtr()->rotate(mpItemXY[i_no]->getSizeX() * 0.5f,
                                             mpItemXY[i_no]->getSizeY() * 0.5f, ROTATE_Z,
                                             mItemParams[i_no].rotation);

        if (i_no == SELECT_X_e) {
            mpButtonXY[0]->scale(g_drawHIO.mButtonXScale, g_drawHIO.mButtonXScale);
            mpButtonXY[0]->paneTrans(g_drawHIO.mButtonXPosX, g_drawHIO.mButtonXPosY);
            f32 temp_f31 = mItemParams[SELECT_X_e].scale;

            if (field_0x773[0] != dMeter2Info_isDirectUseItem(0)) {
                field_0x773[0] = dMeter2Info_isDirectUseItem(0);

                if (dMeter2Info_isDirectUseItem(0) && field_0x610[0] == 0.0f) {
                    field_0x610[0] = 18.0f - g_drawHIO.field_0x4e0;
                }
            }

            dMeter2Info_isDirectUseItem(0);

            temp_f31 *= g_drawHIO.field_0x54c;
            mpItemXY[0]->scale(temp_f31, temp_f31);
            mpItemXY[0]->paneTrans(mItemParams[SELECT_X_e].pos_x + field_0x6ac[0],
                                   mItemParams[SELECT_X_e].pos_y + field_0x6b8[0]);

            mpLightXY[0]->scale(g_drawHIO.mButtonXItemBaseScale[var_r29],
                                g_drawHIO.mButtonXItemBaseScale[var_r29]);
            mpLightXY[0]->paneTrans(g_drawHIO.mButtonXItemBasePosX[var_r29],
                                    g_drawHIO.mButtonXItemBasePosY[var_r29]);
            mpLightXY[0]->setAlphaRate(mButtonXItemBaseAlpha[var_r29] * field_0x7f0);

            mpTextXY[i_no]->scale(g_drawHIO.mButtonXYTextScale, g_drawHIO.mButtonXYTextScale);
            mpTextXY[i_no]->paneTrans(g_drawHIO.mButtonXYTextPosX, g_drawHIO.mButtonXYTextPosY);
        } else if (i_no == SELECT_Y_e) {
            mpButtonXY[1]->scale(g_drawHIO.mButtonYScale, g_drawHIO.mButtonYScale);
            mpButtonXY[1]->paneTrans(g_drawHIO.mButtonYPosX, g_drawHIO.mButtonYPosY);
            f32 temp_f31 = mItemParams[SELECT_Y_e].scale;

            if (field_0x773[1] != dMeter2Info_isDirectUseItem(1)) {
                field_0x773[1] = dMeter2Info_isDirectUseItem(1);

                if (dMeter2Info_isDirectUseItem(1) && field_0x610[1] == 0.0f) {
                    field_0x610[1] = 18.0f - g_drawHIO.field_0x4e0;
                }
            }

            dMeter2Info_isDirectUseItem(1);

            temp_f31 *= g_drawHIO.field_0x54c;
            mpItemXY[1]->scale(temp_f31, temp_f31);
            mpItemXY[1]->paneTrans(mItemParams[SELECT_Y_e].pos_x + field_0x6ac[1],
                                   mItemParams[SELECT_Y_e].pos_y + field_0x6b8[1]);

            mpLightXY[1]->scale(g_drawHIO.mButtonYItemBaseScale[var_r29],
                                g_drawHIO.mButtonYItemBaseScale[var_r29]);
            mpLightXY[1]->paneTrans(g_drawHIO.mButtonYItemBasePosX[var_r29],
                                    g_drawHIO.mButtonYItemBasePosY[var_r29]);
            mpLightXY[1]->setAlphaRate(mButtonYItemBaseAlpha[var_r29] * field_0x7f0);

            mpTextXY[i_no]->scale(g_drawHIO.mButtonXYTextScale, g_drawHIO.mButtonXYTextScale);
            mpTextXY[i_no]->paneTrans(g_drawHIO.mButtonXYTextPosX, g_drawHIO.mButtonXYTextPosY);
        }
    }
}

f32 dMeter2Draw_c::getButtonCrossParentInitTransY() {
    return mpButtonCrossParent->getInitCenterPosY();
}

void dMeter2Draw_c::drawButtonCross(f32 i_posX, f32 i_posY) {
    mpButtonCrossParent->scale(g_drawHIO.mButtonCrossScale, g_drawHIO.mButtonCrossScale);
    mpTextI->scale(g_drawHIO.mButtonCrossTextScale, g_drawHIO.mButtonCrossTextScale);
    mpTextM->scale(g_drawHIO.mButtonCrossTextScale, g_drawHIO.mButtonCrossTextScale);
    mpButtonCrossParent->paneTrans(i_posX, i_posY);
}

void dMeter2Draw_c::setAlphaButtonCrossAnimeMin() {
    if (mpButtonCrossParent->getAlphaRate() != 0.0f) {
        mpButtonCrossParent->setAlphaRate(g_drawHIO.mParentAlpha);
        setAlphaAnimeMin(mpButtonCrossParent, 5);
    }

    setAlphaButtonCrossItemAnimeMin();
    setAlphaButtonCrossMapAnimeMin();
}

void dMeter2Draw_c::setAlphaButtonCrossAnimeMax() {
    if (g_drawHIO.mParentAlpha != mpButtonCrossParent->getAlphaRate()) {
        mpButtonCrossParent->setAlphaRate(g_drawHIO.mParentAlpha);
        setAlphaAnimeMax(mpButtonCrossParent, 5);
    }

    if (dMeter2Info_isItemOpenCheck()) {
        setAlphaButtonCrossItemAnimeMax();
    } else {
        setAlphaButtonCrossItemAnimeMin();
    }

    if (dMeter2Info_isMapOpenCheck()) {
        setAlphaButtonCrossMapAnimeMax();
    } else {
        setAlphaButtonCrossMapAnimeMin();
    }
}

void dMeter2Draw_c::setAlphaButtonCrossItemAnimeMin() {
    if (mpTextI->getAlphaRate() != 0.0f) {
        mpTextI->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mButtonCrossITEMAlpha);
        setAlphaAnimeMin(mpTextI, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (mpJujiI[i] != NULL && mpJujiI[i]->getAlphaRate() != 0.0f) {
            mpJujiI[i]->setAlphaRate(g_drawHIO.mParentAlpha);
            setAlphaAnimeMin(mpJujiI[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonCrossItemAnimeMax() {
    if (g_drawHIO.mParentAlpha * g_drawHIO.mButtonCrossITEMAlpha != mpTextI->getAlphaRate()) {
        mpTextI->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mButtonCrossITEMAlpha);
        setAlphaAnimeMax(mpTextI, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (mpJujiI[i] != NULL && mpJujiI[i]->getAlphaRate() != g_drawHIO.mParentAlpha) {
            mpJujiI[i]->setAlphaRate(g_drawHIO.mParentAlpha);
            setAlphaAnimeMax(mpJujiI[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonCrossMapAnimeMin() {
    if (mpTextM->getAlphaRate() != 0.0f) {
        mpTextM->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mButtonCrossMAPAlpha);
        setAlphaAnimeMin(mpTextM, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (mpJujiM[i] != NULL && mpJujiM[i]->getAlphaRate() != 0.0f) {
            mpJujiM[i]->setAlphaRate(g_drawHIO.mParentAlpha);
            setAlphaAnimeMin(mpJujiM[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonCrossMapAnimeMax() {
    if (g_drawHIO.mParentAlpha * g_drawHIO.mButtonCrossMAPAlpha != mpTextM->getAlphaRate()) {
        mpTextM->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mButtonCrossMAPAlpha);
        setAlphaAnimeMax(mpTextM, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (mpJujiM[i] != NULL && mpJujiM[i]->getAlphaRate() != g_drawHIO.mParentAlpha) {
            mpJujiM[i]->setAlphaRate(g_drawHIO.mParentAlpha);
            setAlphaAnimeMax(mpJujiM[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonChange(bool param_0) {
    bool set_parent = false;
    bool set_buttonA = false;
    bool set_buttonB = false;
    bool set_buttonX = false;
    bool set_buttonY = false;
    bool set_buttonZ = false;
    bool set_buttonXItem = false;
    bool set_buttonYItem = false;
    bool set_buttonBase = false;
    bool set_buttonZItem = false;

    if (field_0x7f0 != g_drawHIO.mParentAlpha || param_0) {
        field_0x7f0 = g_drawHIO.mParentAlpha;
        set_parent = true;
    }

    if (mMainHUDButtonsAlpha != g_drawHIO.mMainHUDButtonsAlpha || param_0) {
        field_0x7f0 = g_drawHIO.mMainHUDButtonsAlpha;
        set_parent = true;
    }

    if (mButtonAAlpha != g_drawHIO.mButtonAAlpha || param_0) {
        mButtonAAlpha = g_drawHIO.mButtonAAlpha;
        set_buttonA = true;
    }

    if (mButtonBAlpha != g_drawHIO.mButtonBAlpha || param_0) {
        mButtonBAlpha = g_drawHIO.mButtonBAlpha;
        set_buttonB = true;
    }

    if (mButtonXAlpha != g_drawHIO.mButtonXAlpha || param_0) {
        mButtonXAlpha = g_drawHIO.mButtonXAlpha;
        set_buttonX = true;
    }

    if (mButtonYAlpha != g_drawHIO.mButtonYAlpha || param_0) {
        mButtonYAlpha = g_drawHIO.mButtonYAlpha;
        set_buttonY = true;
    }

    if (field_0x80c != g_drawHIO.field_0x168 || param_0) {
        field_0x80c = g_drawHIO.field_0x168;
    }

    if (field_0x810 != g_drawHIO.mButtonZAlpha || param_0) {
        field_0x810 = g_drawHIO.mButtonZAlpha;
        set_buttonZ = true;
    }

    int sp44[4];
    for (int i = 0; i < 2; i++) {
        if (mpTextXY[i]->isVisible()) {
            sp44[i] = 0;
        } else {
            sp44[i] = 1;
        }
    }
    sp44[2] = 0;
    sp44[3] = 0;

    if (mItemBBaseAlpha[sp44[3]] != g_drawHIO.mItemBBaseAlpha[sp44[3]] || param_0) {
        mItemBBaseAlpha[sp44[3]] = g_drawHIO.mItemBBaseAlpha[sp44[3]];
    }

    if (mButtonXItemBaseAlpha[sp44[0]] != g_drawHIO.mButtonXItemBaseAlpha[sp44[0]] || param_0) {
        mButtonXItemBaseAlpha[sp44[0]] = g_drawHIO.mButtonXItemBaseAlpha[sp44[0]];
        set_buttonXItem = true;
    }

    if (mButtonYItemBaseAlpha[sp44[1]] != g_drawHIO.mButtonYItemBaseAlpha[sp44[1]] || param_0) {
        mButtonYItemBaseAlpha[sp44[1]] = g_drawHIO.mButtonYItemBaseAlpha[sp44[1]];
        set_buttonYItem = true;
    }

    if (mButtonZItemBaseAlpha != g_drawHIO.mButtonZItemBaseAlpha || param_0) {
        mButtonZItemBaseAlpha = g_drawHIO.mButtonZItemBaseAlpha;
        set_buttonZItem = true;
    }

    if (mButtonBaseAlpha != g_drawHIO.mButtonBaseAlpha || param_0) {
        mButtonBaseAlpha = g_drawHIO.mButtonBaseAlpha;
        set_buttonBase = true;
    }

    if (mButtonDisplayBackAlpha != g_drawHIO.mButtonDisplayBackAlpha || param_0) {
        mButtonDisplayBackAlpha = g_drawHIO.mButtonDisplayBackAlpha;
    }

    if (set_parent || set_buttonA || param_0) {
        mpButtonA->setAlphaRate(mButtonAAlpha * field_0x7f0);
    }

    if (set_parent || set_buttonB || param_0) {
        mpButtonB->setAlphaRate(mButtonBAlpha * field_0x7f0);
    }

    if (set_parent || set_buttonX || param_0) {
        mpButtonXY[0]->setAlphaRate(mButtonXAlpha * field_0x7f0);
    }

    if (set_parent || set_buttonY || param_0) {
        mpButtonXY[1]->setAlphaRate(mButtonYAlpha * field_0x7f0);
    }

    if (set_parent || set_buttonZ || param_0) {
        mpButtonXY[2]->setAlphaRate(field_0x810 * field_0x7f0);
    }

    if (set_parent || set_buttonXItem || param_0) {
        mpLightXY[0]->setAlphaRate(mButtonXItemBaseAlpha[sp44[0]] * field_0x7f0);
    }

    if (set_parent || set_buttonYItem || param_0) {
        mpLightXY[1]->setAlphaRate(mButtonYItemBaseAlpha[sp44[1]] * field_0x7f0);
    }

    if (set_parent || param_0) {
        mpLightXY[2]->setAlphaRate(field_0x82c[sp44[2]] * field_0x7f0);
    }

    if (set_parent || set_buttonZItem || param_0) {
        mpLightXY[2]->setAlphaRate(mButtonZItemBaseAlpha * field_0x7f0);
    }

    if (mpUzu != NULL && (set_parent || set_buttonBase || param_0)) {
        mpUzu->setAlphaRate(mButtonBaseAlpha * field_0x7f0);
    }

    if (mButtonATextSpacing != g_drawHIO.mButtonATextSpacing || param_0) {
        mButtonATextSpacing = g_drawHIO.mButtonATextSpacing;

        for (int i = 0; i < 5; i++) {
            static_cast<J2DTextBox*>(mpAText[i]->getPanePtr())->setCharSpace(mButtonATextSpacing);
        }
    }

    if (mButtonATextColor.r != g_drawHIO.mButtonATextColor.r ||
        mButtonATextColor.g != g_drawHIO.mButtonATextColor.g ||
        mButtonATextColor.b != g_drawHIO.mButtonATextColor.b ||
        mButtonATextColor.a != g_drawHIO.mButtonATextColor.a || param_0)
    {
        mButtonATextColor.r = g_drawHIO.mButtonATextColor.r;
        mButtonATextColor.g = g_drawHIO.mButtonATextColor.g;
        mButtonATextColor.b = g_drawHIO.mButtonATextColor.b;
        mButtonATextColor.a = g_drawHIO.mButtonATextColor.a;

        static_cast<J2DTextBox*>(mpAText[4]->getPanePtr())
            ->setFontColor(g_drawHIO.mButtonATextColor, g_drawHIO.mButtonATextColor);
    }

    if (mButtonBTextColor.r != g_drawHIO.mButtonBFontColor.r ||
        mButtonBTextColor.g != g_drawHIO.mButtonBFontColor.g ||
        mButtonBTextColor.b != g_drawHIO.mButtonBFontColor.b ||
        mButtonBTextColor.a != g_drawHIO.mButtonBFontColor.a || param_0)
    {
        mButtonBTextColor.r = g_drawHIO.mButtonBFontColor.r;
        mButtonBTextColor.g = g_drawHIO.mButtonBFontColor.g;
        mButtonBTextColor.b = g_drawHIO.mButtonBFontColor.b;
        mButtonBTextColor.a = g_drawHIO.mButtonBFontColor.a;

        static_cast<J2DTextBox*>(mpBText[4]->getPanePtr())
            ->setFontColor(g_drawHIO.mButtonBFontColor, g_drawHIO.mButtonBFontColor);
    }

    if (mButtonZTextColor.r != g_drawHIO.mButtonZFontColor.r ||
        mButtonZTextColor.g != g_drawHIO.mButtonZFontColor.g ||
        mButtonZTextColor.b != g_drawHIO.mButtonZFontColor.b ||
        mButtonZTextColor.a != g_drawHIO.mButtonZFontColor.a || param_0)
    {
        mButtonZTextColor.r = g_drawHIO.mButtonZFontColor.r;
        mButtonZTextColor.g = g_drawHIO.mButtonZFontColor.g;
        mButtonZTextColor.b = g_drawHIO.mButtonZFontColor.b;
        mButtonZTextColor.a = g_drawHIO.mButtonZFontColor.a;

        static_cast<J2DTextBox*>(mpXYText[4][2]->getPanePtr())
            ->setFontColor(g_drawHIO.mButtonZFontColor, g_drawHIO.mButtonZFontColor);
    }

    if (mButtonXYTextColor.r != g_drawHIO.mButtonXYTextColor.r ||
        mButtonXYTextColor.g != g_drawHIO.mButtonXYTextColor.g ||
        mButtonXYTextColor.b != g_drawHIO.mButtonXYTextColor.b ||
        mButtonXYTextColor.a != g_drawHIO.mButtonXYTextColor.a || param_0)
    {
        mButtonXYTextColor.r = g_drawHIO.mButtonXYTextColor.r;
        mButtonXYTextColor.g = g_drawHIO.mButtonXYTextColor.g;
        mButtonXYTextColor.b = g_drawHIO.mButtonXYTextColor.b;
        mButtonXYTextColor.a = g_drawHIO.mButtonXYTextColor.a;

        static_cast<J2DTextBox*>(mpXYText[4][0]->getPanePtr())
            ->setFontColor(g_drawHIO.mButtonXYTextColor, g_drawHIO.mButtonXYTextColor);

        static_cast<J2DTextBox*>(mpXYText[4][1]->getPanePtr())
            ->setFontColor(g_drawHIO.mButtonXYTextColor, g_drawHIO.mButtonXYTextColor);
    }
}

void dMeter2Draw_c::setAlphaButtonAnimeMin() {
    if (mpButtonParent->getAlphaRate() != 0.0f) {
        mpButtonParent->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
        setAlphaAnimeMin(mpButtonParent, 5);

        if (mpUzu != NULL) {
            mpUzu->setAlphaRate(mButtonBaseAlpha * mpButtonParent->getAlphaRate());
        }
    }
}

void dMeter2Draw_c::setAlphaButtonAnimeMax() {
    if (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha != mpButtonParent->getAlphaRate()) {
        mpButtonParent->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
        setAlphaAnimeMax(mpButtonParent, 5);

        if (mpUzu != NULL) {
            mpUzu->setAlphaRate(mButtonBaseAlpha * mpButtonParent->getAlphaRate());
        }
    }

    if (mpButtonParent->getAlphaTimer() == 5) {
        if (dMeter2Info_getLightDropGetFlag(dComIfGp_getStartStageDarkArea()) == 1) {
            dMeter2Info_setLightDropGetFlag(dComIfGp_getStartStageDarkArea(), 2);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonAAnimeMin() {
    if (mpButtonA->getAlphaRate() != 0.0f) {
        mpButtonA->setAlphaRate(g_drawHIO.mButtonAAlpha *
                                (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha));
        setAlphaAnimeMin(mpButtonA, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (mpAText[i]->getAlphaRate() != 0.0f) {
            mpAText[i]->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
            setAlphaAnimeMin(mpAText[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonAAnimeMax() {
    if (g_drawHIO.mButtonAAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha) !=
        mpButtonA->getAlphaRate())
    {
        mpButtonA->setAlphaRate(g_drawHIO.mButtonAAlpha *
                                (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha));
        setAlphaAnimeMax(mpButtonA, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha != mpAText[i]->getAlphaRate()) {
            mpAText[i]->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
            setAlphaAnimeMax(mpAText[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonBAnimeMin() {
    if (mpButtonB->getAlphaRate() != 0.0f) {
        mpButtonB->setAlphaRate(g_drawHIO.mButtonBAlpha *
                                (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha));
        setAlphaAnimeMin(mpButtonB, 5);

        mpLightB->setAlphaRate(g_drawHIO.mButtonBAlpha *
                               (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha));
        setAlphaAnimeMin(mpLightB, 5);
    }

    if (mpItemB->getAlphaRate() != 0.0f) {
        mpItemB->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
        setAlphaAnimeMin(mpItemB, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (mpBText[i]->getAlphaRate() != 0.0f) {
            mpBText[i]->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
            setAlphaAnimeMin(mpBText[i], 5);
        }
    }
}

void dMeter2Draw_c::setAlphaButtonBAnimeMax() {
    if (mpButtonB->getAlphaRate() !=
        g_drawHIO.mButtonBAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha))
    {
        mpButtonB->setAlphaRate(g_drawHIO.mButtonBAlpha *
                                (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha));
        setAlphaAnimeMax(mpButtonB, 5);

        mpLightB->setAlphaRate(g_drawHIO.mButtonBAlpha *
                               (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha));
        setAlphaAnimeMax(mpLightB, 5);
    }

    if (mpItemB->getAlphaRate() != g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha) {
        mpItemB->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
        setAlphaAnimeMax(mpItemB, 5);
    }

    for (int i = 0; i < 5; i++) {
        if (mpBText[i]->getAlphaRate() != g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha) {
            mpBText[i]->setAlphaRate(g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
            setAlphaAnimeMax(mpBText[i], 5);
        }
    }
}

void dMeter2Draw_c::setButtonIconAAlpha(u8 unused0, u32 unused1, bool unused2) {
    if (mpButtonA->isVisible()) {
        u8 alpha = g_drawHIO.mButtonAAlpha *
                   (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha) *
                   (f32)mpButtonA->getInitAlpha();
        f32 alpha_rate = mpButtonParent->getAlphaRate();

        if (!dMeter2Info_isUseButton(1) && !dComIfGp_isDoSetFlag(4)) {
            alpha = g_drawHIO.mButtonXYBaseDimAlpha;
        }

        mpButtonA->setAlpha(alpha * alpha_rate);

        for (int i = 0; i < 5; i++) {
            u8 alpha = mpAText[i]->getInitAlpha();
            if (!dMeter2Info_isUseButton(1)) {
                alpha = (f32)alpha * ((f32)g_drawHIO.field_0x42c / 255.0f);
            }

            mpAText[i]->setAlpha(alpha * alpha_rate);
        }
    }
}

void dMeter2Draw_c::setButtonIconBAlpha(u8 unused0, u32 unused1, bool param_2) {
    if (mpItemB->isVisible() || mpLightB->isVisible() || mpButtonB->isVisible()) {
        f32 temp_f30 =
            g_drawHIO.mButtonBAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);

        f32 var_f2;
        if (mpItemB->isVisible()) {
            var_f2 = mItemBBaseAlpha[0];
        } else {
            var_f2 = mItemBBaseAlpha[1];
        }

        u8 var_r25 = mpItemB->getInitAlpha();
        u8 var_r26 = var_f2 * (temp_f30 * (f32)mpLightB->getInitAlpha());
        u8 var_r27 = temp_f30 * (f32)mpButtonB->getInitAlpha();
        f32 temp_f31 = mpButtonParent->getAlphaRate();

        if (!(dMeter2Info_isUseButton(2) & 1) && !dMeter2Info_isSub2DStatus(1)) {
            var_r25 = g_drawHIO.mButtonXYItemDimAlpha;
            var_r26 = g_drawHIO.mButtonXYItemDimAlpha;
            var_r27 = g_drawHIO.mButtonXYBaseDimAlpha;
        }

        if (!param_2) {
            var_r26 = 0;
        }

        mpItemB->setAlpha(var_r25 * temp_f31);
        mpLightB->setAlpha(var_r26 * temp_f31);
        mpButtonB->setAlpha(var_r27 * temp_f31);

        for (int i = 0; i < 5; i++) {
            u8 alpha = mpBText[i]->getInitAlpha();
            if (!dMeter2Info_isUseButton(2) && !dMeter2Info_isSub2DStatus(1)) {
                alpha = (f32)alpha * ((f32)g_drawHIO.field_0x42c / 255.0f);
            }

            mpBText[i]->setAlpha(alpha * temp_f31);
        }
    }
}

void dMeter2Draw_c::setButtonIconMidonaAlpha(u32 param_0) {
    mpButtonMidona->scale(g_drawHIO.mMidnaIconScale, g_drawHIO.mMidnaIconScale);
    mpButtonMidona->paneTrans(g_drawHIO.mMidnaIconPosX, g_drawHIO.mMidnaIconPosY);

    if (mpButtonMidona->isVisible()) {
        f32 temp_f30 =
            g_drawHIO.mMidnaIconAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
        f32 var_f29 = (f32)mpButtonMidona->getInitAlpha() / 255.0f;
        f32 temp_f31 = mpButtonParent->getAlphaRate();
        bool var_r31 = 1;

        if (getCanoeFishing() ||
              /*dSv_event_flag_c::M_009 - Cutscene - [cutscene: 6B] Prison escape - Midna rides on back */
            (!dComIfGs_isEventBit(0x0540) && !dMeter2Info_isUseButton(0x800)) ||
             /* dSv_event_flag_c::M_067 - Main Event - Midna riding / not riding (ON == riding) */
            !dComIfGs_isEventBit(0x0C10) ||
            /* dSv_event_flag_c::F_0800 - Cutscene - After returning to Ordon Woods, until Midna comes out of the shadows 
                                                     (If 800 is ON, Midna can't be called) */
            dComIfGs_isEventBit(0x6140))
        {
            var_f29 = 0.0f;
        } else if (dComIfGp_isPauseFlag()) {
            var_f29 = 0.0f;
        } else if (!dMeter2Info_isUseButton(0x800)) {
            var_f29 = g_drawHIO.mButtonXYItemDimAlpha / 255.0f;
        } else if (isEmphasisZ() && !(param_0 & 0x40000000) && temp_f31 > 0.0f) {
            if (field_0x738 == 0.0f) {
                field_0x738 = 18.0f;
            }

            if (field_0x738 == 18.0f) {
                mDoAud_seStart(Z2SE_SY_HINT_BUTTON_BLINK, NULL, 0, 0);
                dMeter2Info_set2DVibration();
            }

            mButtonZAlpha = var_f29;
            var_r31 = 0;
        }

        if (var_r31) {
            if (field_0x738 != 0.0f) {
                field_0x738 = 0.0f;
            }

            if (mButtonZAlpha != var_f29) {
                cLib_addCalc2(&mButtonZAlpha, var_f29, 0.4f, 0.5f);

                if (fabsf(mButtonZAlpha - var_f29) < 0.1f) {
                    mButtonZAlpha = var_f29;
                }
            }
        }

        mpButtonMidona->setAlpha(temp_f30 * (255.0f * mButtonZAlpha * temp_f31));
    } else {
        mButtonZAlpha = 0.0f;
    }

    f32 var_f29_2 =
        (g_drawHIO.mButtonZAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha) *
         (f32)mpButtonXY[2]->getInitAlpha()) /
        255.0f;
    f32 temp_f30_2 = mpButtonParent->getAlphaRate();
    if (param_0 & 0x1000000) {
        var_f29_2 = 0.0f;
    } else if (!dMeter2Info_isUseButton(0x800)) {
        var_f29_2 = (f32)g_drawHIO.mButtonXYBaseDimAlpha / 255.0f;
    }

    if (field_0x724 != var_f29_2) {
        cLib_addCalc2(&field_0x724, var_f29_2, 0.4f, 0.5f);

        if (fabsf(field_0x724 - var_f29_2) < 0.1f) {
            field_0x724 = var_f29_2;
        }
    }

    mpButtonXY[2]->setAlpha(255.0f * field_0x724 * temp_f30_2);
}

void dMeter2Draw_c::setButtonIconAlpha(int i_no, u8 unused0, u32 unused1, bool unused2) {
    JUT_ASSERT(0, i_no < SELECT_MAX_e);

    if (mpItemXY[i_no]->isVisible() || mpLightXY[i_no]->isVisible() ||
        mpButtonXY[i_no]->isVisible())
    {
        f32 var_f30 = 1.0f;

        int var_r26 = 1;
        if (mpItemXY[i_no]->isVisible()) {
            var_r26 = 0;
        }

        f32 var_f2;
        if (i_no == 0) {
            var_f2 =
                g_drawHIO.mButtonXAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
            var_f30 = g_drawHIO.mButtonXItemBaseAlpha[var_r26];
        } else if (i_no == 1) {
            var_f2 =
                g_drawHIO.mButtonYAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
            var_f30 = g_drawHIO.mButtonYItemBaseAlpha[var_r26];
        } else {
            var_f2 =
                g_drawHIO.mButtonZAlpha * (g_drawHIO.mParentAlpha * g_drawHIO.mMainHUDButtonsAlpha);
        }

        u8 var_r28 = mpItemXY[i_no]->getInitAlpha();
        u8 var_r27 = var_f30 * (var_f2 * (f32)mpLightXY[i_no]->getInitAlpha());
        u8 var_r26_2 = var_f2 * (f32)mpButtonXY[i_no]->getInitAlpha();
        f32 temp_f31 = mpButtonParent->getAlphaRate();

        if (i_no == 0) {
            if (!dMeter2Info_isUseButton(4)) {
                if (getFishingType()) {
                    var_r28 = 0;
                } else {
                    var_r28 = g_drawHIO.mButtonXYItemDimAlpha;
                }

                var_r27 = g_drawHIO.mButtonXYItemDimAlpha;
                var_r26_2 = g_drawHIO.mButtonXYBaseDimAlpha;
            }
        } else if (i_no == 1) {
            if (!dMeter2Info_isUseButton(8)) {
                if (getFishingType()) {
                    var_r28 = 0;
                } else {
                    var_r28 = g_drawHIO.mButtonXYItemDimAlpha;
                }

                var_r27 = g_drawHIO.mButtonXYItemDimAlpha;
                var_r26_2 = g_drawHIO.mButtonXYBaseDimAlpha;
            }
        }

        mpItemXY[i_no]->setAlpha((f32)var_r28 * temp_f31);
        mpLightXY[i_no]->setAlpha((f32)var_r27 * temp_f31);
        mpButtonXY[i_no]->setAlpha((f32)var_r26_2 * temp_f31);

        int var_r26_3 = 1;
        if (i_no == 0) {
            if (!dMeter2Info_isUseButton(4)) {
                var_r26_3 = 0;
            }
        } else if (i_no == 1) {
            if (!dMeter2Info_isUseButton(8)) {
                var_r26_3 = 0;
            }
        }

        for (int i = 0; i < 5; i++) {
            u8 var_r0 = mpXYText[i][i_no]->getInitAlpha();
            if (!var_r26_3) {
                var_r0 = (f32)var_r0 * (g_drawHIO.field_0x42c / 255.0f);
            }

            mpXYText[i][i_no]->setAlpha((f32)var_r0 * temp_f31);
        }
    }
}

ResTIMG* dMeter2Draw_c::getNumberTexture(int i) {
    return (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG',
                                                              dMeter2Info_getNumberTextureName(i));
}

char* dMeter2Draw_c::getActionString(u8 i_action, u8 i_type, u8* param_2) {
    static u32 i_action_num[] = {
        0,    1001, 1002, 1003, 1004, 1005, 1006, 1007, 1387, 1009, 1010, 1011, 1012, 1013, 1014,
        1015, 1033, 1016, 1017, 1018, 1019, 1020, 1045, 1022, 1026, 1027, 1028, 1029, 1030, 1024,
        1031, 1025, 1023, 1045, 1036, 1032, 93,   0,    0,    0,    1035, 1034, 1037, 1038, 1039,
        1040, 1041, 998,  1042, 1043, 1044, 1046, 1047, 1048, 1049, 1050, 1051, 1052, 1053, 1054,
        1055, 1056, 1057, 1058, 1059, 1070, 1060, 1061, 1062, 1063, 1064, 1067, 1065, 1066, 1211,
        1069, 1021, 1072, 1073, 0,    1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
        1084, 1085, 1086, 1087, 1088, 1089, 1090, 1092, 1093, 1094, 1095, 1096, 1904, 1097, 1098,
        1099, 1100, 1150, 1149, 1148, 1377, 1147, 1145, 1146, 1161, 1162, 1163, 1164, 1165, 1166,
        1144, 982,  983,  1143, 1160, 1319, 1314, 1399, 1008,
    };

    static char i_text_buf[32];
    strcpy(i_text_buf, "");

    if (param_2 != NULL) {
        *param_2 = 1;
    }

    if (i_action == 0x81) {
        if (g_drawHIO.mButtonATextActionID != 0) {
            JMSMesgEntry_c mesg_entry;

            if (i_type == 1) {
                dMeter2Info_getStringKana(g_drawHIO.mButtonATextActionID, i_text_buf, &mesg_entry);
            } else {
                dMeter2Info_getStringKanji(g_drawHIO.mButtonATextActionID, i_text_buf, &mesg_entry);
            }

            if (param_2 != NULL) {
                *param_2 = mesg_entry.output_type;

                if (g_drawHIO.mButtonATextActionID == 0x3E6) {
                    *param_2 = 7;
                }
            }
        }
    } else if (i_action < 0x82 && i_action_num[i_action] != 0) {
        JMSMesgEntry_c mesg_entry;

        if (i_type == 1) {
            dMeter2Info_getStringKana(i_action_num[i_action], i_text_buf, &mesg_entry);
        } else {
            dMeter2Info_getStringKanji(i_action_num[i_action], i_text_buf, &mesg_entry);
        }

        if (param_2 != NULL) {
            *param_2 = mesg_entry.output_type;

            if (i_action_num[i_action] == 0x3E6) {
                *param_2 = 7;
            }
        }
    }

    return i_text_buf;
}

void dMeter2Draw_c::changeTextureItemB(u8 i_itemNo) {
    int var_r31 = 0;
    if (i_itemNo == fpcNm_ITEM_LURE_ROD) {
        var_r31 = 2;
    } else if (i_itemNo == fpcNm_ITEM_SWORD || i_itemNo == fpcNm_ITEM_MASTER_SWORD || i_itemNo == fpcNm_ITEM_WOOD_STICK ||
               i_itemNo == fpcNm_ITEM_LIGHT_SWORD)
    {
        var_r31 = 1;
    }

    if (field_0x76b == 0) {
        field_0x76b = 1;
    } else {
        field_0x76b = 0;
    }

    setItemParamB(i_itemNo);
    if (dMeter2Info_readItemTexture(i_itemNo, mpItemBTex[field_0x76b][0],
                                    (J2DPicture*)mpItemB->getPanePtr(), mpItemBTex[field_0x76b][1],
                                    mpItemBPane, NULL, NULL, NULL, NULL, -1) <= 1)
    {
        mpItemBPane->hide();
    } else {
        mpItemBPane->show();
    }

    f32 var_f4;
    if (g_drawHIO.mItemScaleAdjustON == true) {
        var_f4 = g_drawHIO.mItemScalePercent / 100.0f;
    } else {
        var_f4 = dItem_data::getTexScale(i_itemNo) / 100.0f;
    }

    field_0x6e4 = var_f4 * ((mpItemBTex[field_0x76b][0]->width * mpItemB->getInitSizeX()) / 48.0f);
    field_0x6e8 = var_f4 * ((mpItemBTex[field_0x76b][0]->height * mpItemB->getInitSizeY()) / 48.0f);
    field_0x6dc = (mpItemB->getInitSizeX() - field_0x6e4) * 0.5f;
    field_0x6e0 = (mpItemB->getInitSizeY() - field_0x6e8) * 0.5f;
    mpItemB->resize(field_0x6e4, field_0x6e8);
    mpItemB->paneTrans(field_0x6dc + (g_drawHIO.mButtonBItemPosX[var_r31] + field_0x6ec),
                       field_0x6e0 + (g_drawHIO.mButtonBItemPosY[var_r31] + field_0x6f0));

    mpItemBPane->resize(field_0x6e4, field_0x6e8);
}

void dMeter2Draw_c::changeTextureItemXY(int i_no, u8 i_itemNo) {
    JUT_ASSERT(0, i_no < SELECT_MAX_e);

    if (i_itemNo == fpcNm_ITEM_LIGHT_ARROW) {
        i_itemNo = fpcNm_ITEM_BOW;
    }

    if (field_0x76c[i_no] == 0) {
        field_0x76c[i_no] = 1;
    } else {
        field_0x76c[i_no] = 0;
    }

    if (dMeter2Info_readItemTexture(i_itemNo, mpItemXYTex[i_no][field_0x76c[i_no]][0],
                                    (J2DPicture*)mpItemXY[i_no]->getPanePtr(),
                                    mpItemXYTex[i_no][field_0x76c[i_no]][1], mpItemXYPane[i_no],
                                    NULL, NULL, NULL, NULL, -1) <= 1)
    {
        mpItemXYPane[i_no]->hide();
    } else {
        mpItemXYPane[i_no]->show();
    }

    f32 var_f4;
    if (g_drawHIO.mItemScaleAdjustON == true) {
        var_f4 = g_drawHIO.mItemScalePercent / 100.0f;
    } else {
        var_f4 = dItem_data::getTexScale(i_itemNo) / 100.0f;
    }

    field_0x6c4[i_no] =
        var_f4 *
        ((mpItemXYTex[i_no][field_0x76c[i_no]][0]->width * mpItemXY[i_no]->getInitSizeX()) / 48.0f);
    field_0x6d0[i_no] =
        var_f4 *
        ((mpItemXYTex[i_no][field_0x76c[i_no]][0]->height * mpItemXY[i_no]->getInitSizeY()) /
         48.0f);
    field_0x6ac[i_no] = (mpItemXY[i_no]->getInitSizeX() - field_0x6c4[i_no]) * 0.5f;
    field_0x6b8[i_no] = (mpItemXY[i_no]->getInitSizeY() - field_0x6d0[i_no]) * 0.5f;
    mpItemXY[i_no]->resize(field_0x6c4[i_no], field_0x6d0[i_no]);

    if (i_no == SELECT_X_e) {
        mpItemXY[i_no]->paneTrans(mItemParams[SELECT_X_e].pos_x + field_0x6ac[i_no],
                                  mItemParams[SELECT_X_e].pos_y + field_0x6b8[i_no]);
    } else {
        mpItemXY[i_no]->paneTrans(mItemParams[SELECT_Y_e].pos_x + field_0x6ac[i_no],
                                  mItemParams[SELECT_Y_e].pos_y + field_0x6b8[i_no]);
    }

    mpItemXYPane[i_no]->resize(field_0x6c4[i_no], field_0x6d0[i_no]);
}

void dMeter2Draw_c::setAlphaAnimeMin(CPaneMgrAlpha* i_pane, s16 i_min) {
    f32 alpha_rate = i_pane->getAlphaRate();
    s16 alpha_timer = i_pane->getAlphaTimer();
    if (alpha_timer > i_min) {
        alpha_timer = i_min;
    }

    if (alpha_timer <= 0) {
        if (i_pane->getAlphaRate() != 0.0f) {
            i_pane->setAlphaRate(0.0f);
        }
    } else {
        alpha_timer--;
        if (alpha_timer < 0) {
            alpha_timer = 0;
        }

        i_pane->alphaAnimeStart(alpha_timer);
        i_pane->setAlphaRate(alpha_rate * ((f32)alpha_timer / (f32)i_min));
    }
}

void dMeter2Draw_c::setAlphaAnimeMax(CPaneMgrAlpha* i_pane, s16 i_max) {
    f32 alpha_rate = i_pane->getAlphaRate();
    s16 alpha_timer = i_pane->getAlphaTimer();

    if (alpha_timer >= i_max) {
        if (i_pane->getAlphaRate() != alpha_rate) {
            i_pane->setAlphaRate(alpha_rate);
        }
    } else {
        alpha_timer++;
        if (alpha_timer > i_max) {
            alpha_timer = i_max;
        }

        i_pane->alphaAnimeStart(alpha_timer);
        i_pane->setAlphaRate(alpha_rate * ((f32)alpha_timer / (f32)i_max));
    }
}

void dMeter2Draw_c::setItemNum(u8 i_button, u8 i_num, u8 i_max) {
    JUT_ASSERT(0, i_button < SELECT_MAX_e);

    if (i_num > i_max) {
        i_num = i_max;
    }

    JUtility::TColor black;
    JUtility::TColor white;

    if (i_num == i_max) {
        black.set(30, 30, 30, 0);
        white.set(255, 200, 50, 255);
    } else if (i_num == 0) {
        black.set(30, 30, 30, 0);
        white.set(180, 180, 180, 255);
    } else {
        black.set(0, 0, 0, 0);
        white.set(255, 255, 255, 255);
    }

    for (int i = 0; i < 3; i++) {
        mpItemNumTex[i_button][i]->setBlackWhite(black, white);
    }

    if (i_num < 100) {
        ResTIMG* timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_num / 10));
        mpItemNumTex[i_button][0]->changeTexture(timg, 0);

        timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_num % 10));
        mpItemNumTex[i_button][1]->changeTexture(timg, 0);

        mpItemNumTex[i_button][2]->hide();
    } else {
        ResTIMG* timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_num / 100));
        mpItemNumTex[i_button][0]->changeTexture(timg, 0);

        i_num = i_num % 100;
        timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_num / 10));

        mpItemNumTex[i_button][1]->changeTexture(timg, 0);

        timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_num % 10));
        mpItemNumTex[i_button][2]->changeTexture(timg, 0);

        mpItemNumTex[i_button][2]->show();
    }
}

void dMeter2Draw_c::drawItemNum(u8 i_button, f32 i_alpha) {
    JUT_ASSERT(0, i_button < SELECT_MAX_e);

    if (i_alpha == 1.0f) {
        i_alpha = mpItemXY[i_button]->getAlphaRate();
    }

    for (int i = 0; i < 3; i++) {
        mpItemNumTex[i_button][i]->setAlpha(i_alpha * 255.0f);
    }
}

void dMeter2Draw_c::drawKanteraMeter(u8 i_button, f32 i_alphaRate) {
    JUT_ASSERT(0, i_button < SELECT_MAX_e);

    CPaneMgr* pane = mpItemXY[i_button];
    f32 sp10[2] = {0.0f};
    f32 sp8[2] = {0.0f};

    if (i_alphaRate == 1.0f) {
        i_alphaRate = mpItemXY[i_button]->getAlphaRate();
    }

    Vec vtx0 = pane->getPanePtr()->getGlbVtx(0);
    Vec vtx3 = pane->getPanePtr()->getGlbVtx(3);

    mpKanteraMeter[i_button]->setPos(((vtx0.x + vtx3.x) * 0.5f) + 9.0f + sp10[i_button],
                                     vtx3.y + sp8[i_button]);
    mpKanteraMeter[i_button]->setScale(0.6f, 0.6f);
    mpKanteraMeter[i_button]->setNowGauge(dComIfGs_getMaxOil(), dComIfGs_getOil());
    mpKanteraMeter[i_button]->setAlphaRate(i_alphaRate);
}

u8 dMeter2Draw_c::isButtonVisible() {
    return (mpButtonParent->getAlphaRate() == 0.0f) ^ 1;
}

void dMeter2Draw_c::setItemParamX(u8 i_itemNo) {
    if (g_drawHIO.mButtonDebug[0]) {
        mItemParams[SELECT_X_e].pos_x = g_drawHIO.mButtonXItemPosX;
        mItemParams[SELECT_X_e].pos_y = g_drawHIO.mButtonXItemPosY;
        mItemParams[SELECT_X_e].scale = g_drawHIO.mButtonXItemScale;
        mItemParams[SELECT_X_e].rotation = g_drawHIO.mButtonItemRotation[0];
        mItemParams[SELECT_X_e].num_pos_x = g_drawHIO.mXItemNumPosX;
        mItemParams[SELECT_X_e].num_pos_y = g_drawHIO.mXItemNumPosY;
        mItemParams[SELECT_X_e].num_scale = g_drawHIO.mXItemNumScale;
    } else {
        switch (i_itemNo) {
        case fpcNm_ITEM_BOOMERANG:
            mItemParams[SELECT_X_e].pos_x = 4.0f;
            mItemParams[SELECT_X_e].pos_y = -11.0f;
            mItemParams[SELECT_X_e].scale = 1.7f;
            mItemParams[SELECT_X_e].rotation = 17.0f;
            mItemParams[SELECT_X_e].num_pos_x = -4.0f;
            mItemParams[SELECT_X_e].num_pos_y = -9.2f;
            mItemParams[SELECT_X_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_BOW:
            mItemParams[SELECT_X_e].pos_x = 9.2f;
            mItemParams[SELECT_X_e].pos_y = -14.0f;
            mItemParams[SELECT_X_e].scale = 2.0f;
            mItemParams[SELECT_X_e].rotation = 0.0f;
            mItemParams[SELECT_X_e].num_pos_x = -4.0f;
            mItemParams[SELECT_X_e].num_pos_y = -9.2f;
            mItemParams[SELECT_X_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_HVY_BOOTS:
            mItemParams[SELECT_X_e].pos_x = 4.0f;
            mItemParams[SELECT_X_e].pos_y = -14.0f;
            mItemParams[SELECT_X_e].scale = 1.7f;
            mItemParams[SELECT_X_e].rotation = 0.0f;
            mItemParams[SELECT_X_e].num_pos_x = -4.0f;
            mItemParams[SELECT_X_e].num_pos_y = -9.2f;
            mItemParams[SELECT_X_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_HOOKSHOT:
            mItemParams[SELECT_X_e].pos_x = 17.0f;
            mItemParams[SELECT_X_e].pos_y = -14.0f;
            mItemParams[SELECT_X_e].scale = 1.5f;
            mItemParams[SELECT_X_e].rotation = 350.0f;
            mItemParams[SELECT_X_e].num_pos_x = -4.0f;
            mItemParams[SELECT_X_e].num_pos_y = -9.2f;
            mItemParams[SELECT_X_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_KANTERA:
            mItemParams[SELECT_X_e].pos_x = -2.0f;
            mItemParams[SELECT_X_e].pos_y = -15.0f;
            mItemParams[SELECT_X_e].scale = g_drawHIO.mButtonXItemScale;
            mItemParams[SELECT_X_e].rotation = g_drawHIO.mButtonItemRotation[0];
            mItemParams[SELECT_X_e].num_pos_x = g_drawHIO.mXItemNumPosX;
            mItemParams[SELECT_X_e].num_pos_y = g_drawHIO.mXItemNumPosY;
            mItemParams[SELECT_X_e].num_scale = g_drawHIO.mXItemNumScale;
            break;
        default:
            if (isBottleItem(i_itemNo)) {
                mItemParams[SELECT_X_e].pos_x = -1.3f;
                mItemParams[SELECT_X_e].pos_y = -16.0f;
                mItemParams[SELECT_X_e].scale = 1.888889f;
                mItemParams[SELECT_X_e].rotation = g_drawHIO.mButtonItemRotation[0];
                mItemParams[SELECT_X_e].num_pos_x = g_drawHIO.mXItemNumPosX;
                mItemParams[SELECT_X_e].num_pos_y = g_drawHIO.mXItemNumPosY;
                mItemParams[SELECT_X_e].num_scale = g_drawHIO.mXItemNumScale;
            } else {
                mItemParams[SELECT_X_e].pos_x = g_drawHIO.mButtonXItemPosX;
                mItemParams[SELECT_X_e].pos_y = g_drawHIO.mButtonXItemPosY;
                mItemParams[SELECT_X_e].scale = g_drawHIO.mButtonXItemScale;
                mItemParams[SELECT_X_e].rotation = g_drawHIO.mButtonItemRotation[0];
                mItemParams[SELECT_X_e].num_pos_x = g_drawHIO.mXItemNumPosX;
                mItemParams[SELECT_X_e].num_pos_y = g_drawHIO.mXItemNumPosY;
                mItemParams[SELECT_X_e].num_scale = g_drawHIO.mXItemNumScale;
            }
            break;
        }
    }
}

void dMeter2Draw_c::setItemParamY(u8 i_itemNo) {
    if (g_drawHIO.mButtonDebug[1]) {
        mItemParams[SELECT_Y_e].pos_x = g_drawHIO.mButtonYItemPosX;
        mItemParams[SELECT_Y_e].pos_y = g_drawHIO.mButtonYItemPosY;
        mItemParams[SELECT_Y_e].scale = g_drawHIO.mButtonYItemScale;
        mItemParams[SELECT_Y_e].rotation = g_drawHIO.mButtonItemRotation[1];
        mItemParams[SELECT_Y_e].num_pos_x = g_drawHIO.mYItemNumPosX;
        mItemParams[SELECT_Y_e].num_pos_y = g_drawHIO.mYItemNumPosY;
        mItemParams[SELECT_Y_e].num_scale = g_drawHIO.mYItemNumScale;
    } else {
        switch (i_itemNo) {
        case fpcNm_ITEM_BOOMERANG:
            mItemParams[SELECT_Y_e].pos_x = 14.0f;
            mItemParams[SELECT_Y_e].pos_y = 1.3f;
            mItemParams[SELECT_Y_e].scale = 1.7f;
            mItemParams[SELECT_Y_e].rotation = 17.0f;
            mItemParams[SELECT_Y_e].num_pos_x = -9.2f;
            mItemParams[SELECT_Y_e].num_pos_y = -9.2f;
            mItemParams[SELECT_Y_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_BOW:
            mItemParams[SELECT_Y_e].pos_x = -1.3f;
            mItemParams[SELECT_Y_e].pos_y = -6.6f;
            mItemParams[SELECT_Y_e].scale = 2.0f;
            mItemParams[SELECT_Y_e].rotation = 0.0f;
            mItemParams[SELECT_Y_e].num_pos_x = -9.2f;
            mItemParams[SELECT_Y_e].num_pos_y = -9.2f;
            mItemParams[SELECT_Y_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_HVY_BOOTS:
            mItemParams[SELECT_Y_e].pos_x = 1.3f;
            mItemParams[SELECT_Y_e].pos_y = 1.3f;
            mItemParams[SELECT_Y_e].scale = 1.7f;
            mItemParams[SELECT_Y_e].rotation = 0.0f;
            mItemParams[SELECT_Y_e].num_pos_x = -9.2f;
            mItemParams[SELECT_Y_e].num_pos_y = -9.2f;
            mItemParams[SELECT_Y_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_HOOKSHOT:
            mItemParams[SELECT_Y_e].pos_x = 6.6f;
            mItemParams[SELECT_Y_e].pos_y = -4.0f;
            mItemParams[SELECT_Y_e].scale = 1.5f;
            mItemParams[SELECT_Y_e].rotation = 350.0f;
            mItemParams[SELECT_Y_e].num_pos_x = -9.2f;
            mItemParams[SELECT_Y_e].num_pos_y = -9.2f;
            mItemParams[SELECT_Y_e].num_scale = 0.95f;
            break;
        case fpcNm_ITEM_KANTERA:
            mItemParams[SELECT_Y_e].pos_x = -1.8f;
            mItemParams[SELECT_Y_e].pos_y = g_drawHIO.mButtonYItemPosY;
            mItemParams[SELECT_Y_e].scale = g_drawHIO.mButtonYItemScale;
            mItemParams[SELECT_Y_e].rotation = g_drawHIO.mButtonItemRotation[1];
            mItemParams[SELECT_Y_e].num_pos_x = g_drawHIO.mYItemNumPosX;
            mItemParams[SELECT_Y_e].num_pos_y = g_drawHIO.mYItemNumPosY;
            mItemParams[SELECT_Y_e].num_scale = g_drawHIO.mYItemNumScale;
            break;
        default:
            if (isBottleItem(i_itemNo)) {
                mItemParams[SELECT_Y_e].pos_x = 0.3f;
                mItemParams[SELECT_Y_e].pos_y = -16.0f;
                mItemParams[SELECT_Y_e].scale = 1.888889f;
                mItemParams[SELECT_Y_e].rotation = g_drawHIO.mButtonItemRotation[1];
                mItemParams[SELECT_Y_e].num_pos_x = g_drawHIO.mYItemNumPosX;
                mItemParams[SELECT_Y_e].num_pos_y = g_drawHIO.mYItemNumPosY;
                mItemParams[SELECT_Y_e].num_scale = g_drawHIO.mYItemNumScale;
            } else {
                mItemParams[SELECT_Y_e].pos_x = g_drawHIO.mButtonYItemPosX;
                mItemParams[SELECT_Y_e].pos_y = g_drawHIO.mButtonYItemPosY;
                mItemParams[SELECT_Y_e].scale = g_drawHIO.mButtonYItemScale;
                mItemParams[SELECT_Y_e].rotation = g_drawHIO.mButtonItemRotation[1];
                mItemParams[SELECT_Y_e].num_pos_x = g_drawHIO.mYItemNumPosX;
                mItemParams[SELECT_Y_e].num_pos_y = g_drawHIO.mYItemNumPosY;
                mItemParams[SELECT_Y_e].num_scale = g_drawHIO.mYItemNumScale;
            }
            break;
        }
    }
}

void dMeter2Draw_c::setItemParamZ(u8 i_itemNo) {
    if (g_drawHIO.mButtonDebug[2]) {
        mItemParams[SELECT_Z_e].pos_x = g_drawHIO.field_0x1cc;
        mItemParams[SELECT_Z_e].pos_y = g_drawHIO.field_0x1d0;
        mItemParams[SELECT_Z_e].scale = g_drawHIO.field_0x1d4;
        mItemParams[SELECT_Z_e].rotation = g_drawHIO.mButtonItemRotation[2];
        mItemParams[SELECT_Z_e].num_pos_x = g_drawHIO.field_0x1f8;
        mItemParams[SELECT_Z_e].num_pos_y = g_drawHIO.field_0x208;
        mItemParams[SELECT_Z_e].num_scale = g_drawHIO.field_0x218;
    } else {
        switch (i_itemNo) {
        case fpcNm_ITEM_BOOMERANG:
            mItemParams[SELECT_Z_e].pos_x = 4.0f;
            mItemParams[SELECT_Z_e].pos_y = 1.3f;
            mItemParams[SELECT_Z_e].scale = 2.2f;
            mItemParams[SELECT_Z_e].rotation = 17.0f;
            mItemParams[SELECT_Z_e].num_pos_x = 14.0f;
            mItemParams[SELECT_Z_e].num_pos_y = -30.0f;
            mItemParams[SELECT_Z_e].num_scale = 0.75f;
            break;
        case fpcNm_ITEM_BOW:
            mItemParams[SELECT_Z_e].pos_x = -1.3f;
            mItemParams[SELECT_Z_e].pos_y = 1.3f;
            mItemParams[SELECT_Z_e].scale = 2.2f;
            mItemParams[SELECT_Z_e].rotation = 0.0f;
            mItemParams[SELECT_Z_e].num_pos_x = 14.0f;
            mItemParams[SELECT_Z_e].num_pos_y = -30.0f;
            mItemParams[SELECT_Z_e].num_scale = 0.75f;
            break;
        case fpcNm_ITEM_HVY_BOOTS:
            mItemParams[SELECT_Z_e].pos_x = -6.6f;
            mItemParams[SELECT_Z_e].pos_y = 9.2f;
            mItemParams[SELECT_Z_e].scale = 2.2f;
            mItemParams[SELECT_Z_e].rotation = 0.0f;
            mItemParams[SELECT_Z_e].num_pos_x = 14.0f;
            mItemParams[SELECT_Z_e].num_pos_y = -30.0f;
            mItemParams[SELECT_Z_e].num_scale = 0.8f;
            break;
        case fpcNm_ITEM_HOOKSHOT:
            mItemParams[SELECT_Z_e].pos_x = -4.0f;
            mItemParams[SELECT_Z_e].pos_y = 1.3f;
            mItemParams[SELECT_Z_e].scale = 1.7f;
            mItemParams[SELECT_Z_e].rotation = 0.0f;
            mItemParams[SELECT_Z_e].num_pos_x = 1.4f;
            mItemParams[SELECT_Z_e].num_pos_y = -30.0f;
            mItemParams[SELECT_Z_e].num_scale = 0.8f;
            break;
        case fpcNm_ITEM_KANTERA:
            mItemParams[SELECT_Z_e].pos_x = -6.6f;
            mItemParams[SELECT_Z_e].pos_y = 6.6f;
            mItemParams[SELECT_Z_e].scale = g_drawHIO.field_0x1d4;
            mItemParams[SELECT_Z_e].rotation = g_drawHIO.mButtonItemRotation[2];
            mItemParams[SELECT_Z_e].num_pos_x = g_drawHIO.field_0x1f8;
            mItemParams[SELECT_Z_e].num_pos_y = g_drawHIO.field_0x208;
            mItemParams[SELECT_Z_e].num_scale = g_drawHIO.field_0x218;
            break;
        default:
            if (isBottleItem(i_itemNo)) {
                mItemParams[SELECT_Z_e].pos_x = -3.5f;
                mItemParams[SELECT_Z_e].pos_y = 17.0f;
                mItemParams[SELECT_Z_e].scale = 1.888889f;
                mItemParams[SELECT_Z_e].rotation = g_drawHIO.mButtonItemRotation[2];
                mItemParams[SELECT_Z_e].num_pos_x = g_drawHIO.field_0x1f8;
                mItemParams[SELECT_Z_e].num_pos_y = g_drawHIO.field_0x208;
                mItemParams[SELECT_Z_e].num_scale = g_drawHIO.field_0x218;
            } else {
                mItemParams[SELECT_Z_e].pos_x = g_drawHIO.field_0x1cc;
                mItemParams[SELECT_Z_e].pos_y = g_drawHIO.field_0x1d0;
                mItemParams[SELECT_Z_e].scale = g_drawHIO.field_0x1d4;
                mItemParams[SELECT_Z_e].rotation = g_drawHIO.mButtonItemRotation[2];
                mItemParams[SELECT_Z_e].num_pos_x = g_drawHIO.field_0x1f8;
                mItemParams[SELECT_Z_e].num_pos_y = g_drawHIO.field_0x208;
                mItemParams[SELECT_Z_e].num_scale = g_drawHIO.field_0x218;
            }
            break;
        }
    }
}

void dMeter2Draw_c::setItemParamB(u8 i_itemNo) {
    if (g_drawHIO.mButtonDebug[3]) {
        mItemParams[3].pos_x = g_drawHIO.mButtonBItemPosX[0];
        mItemParams[3].pos_y = g_drawHIO.mButtonBItemPosY[0];
        mItemParams[3].scale = g_drawHIO.mButtonBItemScale[0];
        mItemParams[3].rotation = g_drawHIO.mButtonBItemRotation[0];
        mItemParams[3].num_pos_x = g_drawHIO.mItemBNumPosX;
        mItemParams[3].num_pos_y = g_drawHIO.mItemBNumPosY;
        mItemParams[3].num_scale = g_drawHIO.mItemBNumScale;
    } else {
        if (isBottleItem(i_itemNo)) {
            mItemParams[3].pos_x = 17.3f;
            mItemParams[3].pos_y = -46.8f;
            mItemParams[3].scale = 1.7f;
            mItemParams[3].rotation = 0.0f;
            mItemParams[3].num_pos_x = 14.0f;
            mItemParams[3].num_pos_y = -30.0f;
            mItemParams[3].num_scale = 0.75f;
        } else {
            switch (i_itemNo) {
            case fpcNm_ITEM_LURE_ROD:
            case fpcNm_ITEM_BEE_ROD:
            case fpcNm_ITEM_JEWEL_ROD:
            case fpcNm_ITEM_JEWEL_BEE_ROD:
            case fpcNm_ITEM_JEWEL_WORM_ROD:
            case fpcNm_ITEM_HORSE_FLUTE:
            case fpcNm_ITEM_FISHING_ROD_1:
                mItemParams[3].pos_x = 17.3f;
                mItemParams[3].pos_y = -46.8f;
                mItemParams[3].scale = 1.7f;
                mItemParams[3].rotation = 0.0f;
                mItemParams[3].num_pos_x = 14.0f;
                mItemParams[3].num_pos_y = -30.0f;
                mItemParams[3].num_scale = 0.75f;
                break;
            case fpcNm_ITEM_BOOMERANG:
                mItemParams[3].pos_x = 26.0f;
                mItemParams[3].pos_y = -54.7f;
                mItemParams[3].scale = 2.2f;
                mItemParams[3].rotation = 17.0f;
                mItemParams[3].num_pos_x = 14.0f;
                mItemParams[3].num_pos_y = -30.0f;
                mItemParams[3].num_scale = 0.75f;
                break;
            case fpcNm_ITEM_BOW:
                mItemParams[3].pos_x = 20.7f;
                mItemParams[3].pos_y = -55.7f;
                mItemParams[3].scale = 2.2f;
                mItemParams[3].rotation = 0.0f;
                mItemParams[3].num_pos_x = 14.0f;
                mItemParams[3].num_pos_y = -30.0f;
                mItemParams[3].num_scale = 0.75f;
                break;
            case fpcNm_ITEM_HVY_BOOTS:
                mItemParams[3].pos_x = 15.4f;
                mItemParams[3].pos_y = -47.8f;
                mItemParams[3].scale = 2.2f;
                mItemParams[3].rotation = 0.0f;
                mItemParams[3].num_pos_x = 14.0f;
                mItemParams[3].num_pos_y = -30.0f;
                mItemParams[3].num_scale = 0.8f;
                break;
            case fpcNm_ITEM_SPINNER:
            case fpcNm_ITEM_HAWK_EYE:
                mItemParams[3].pos_x = 23.3f;
                mItemParams[3].pos_y = -47.8f;
                mItemParams[3].scale = 1.7f;
                mItemParams[3].rotation = 0.0f;
                mItemParams[3].num_pos_x = 1.4f;
                mItemParams[3].num_pos_y = -30.0f;
                mItemParams[3].num_scale = 0.8f;
                break;
            case fpcNm_ITEM_COPY_ROD:
            case fpcNm_ITEM_W_HOOKSHOT:
            case fpcNm_ITEM_IRONBALL:
                mItemParams[3].pos_x = 29.3f;
                mItemParams[3].pos_y = -47.8f;
                mItemParams[3].scale = 1.7f;
                mItemParams[3].rotation = 0.0f;
                mItemParams[3].num_pos_x = 1.4f;
                mItemParams[3].num_pos_y = -30.0f;
                mItemParams[3].num_scale = 0.8f;
                break;
            case fpcNm_ITEM_PACHINKO:
            case fpcNm_ITEM_BOMB_BAG_LV1:
            case fpcNm_ITEM_NORMAL_BOMB:
            case fpcNm_ITEM_WATER_BOMB:
            case fpcNm_ITEM_POKE_BOMB:
            case fpcNm_ITEM_RAFRELS_MEMO:
            case fpcNm_ITEM_TKS_LETTER:
                mItemParams[3].pos_x = 20.3f;
                mItemParams[3].pos_y = -47.8f;
                mItemParams[3].scale = 1.7f;
                mItemParams[3].rotation = 0.0f;
                mItemParams[3].num_pos_x = 12.0f;
                mItemParams[3].num_pos_y = -22.0f;
                mItemParams[3].num_scale = 0.8f;
                break;
            case fpcNm_ITEM_HOOKSHOT:
                mItemParams[3].pos_x = 21.0f;
                mItemParams[3].pos_y = -46.7f;
                mItemParams[3].scale = 1.7f;
                mItemParams[3].rotation = 0.0f;
                mItemParams[3].num_pos_x = 1.4f;
                mItemParams[3].num_pos_y = -30.0f;
                mItemParams[3].num_scale = 0.8f;
                break;
            case fpcNm_ITEM_KANTERA:
                mItemParams[3].pos_x = 19.0f;
                mItemParams[3].pos_y = -45.0f;
                mItemParams[3].scale = g_drawHIO.mButtonBItemScale[0];
                mItemParams[3].rotation = g_drawHIO.mButtonBItemRotation[0];
                mItemParams[3].num_pos_x = g_drawHIO.mItemBNumPosX;
                mItemParams[3].num_pos_y = g_drawHIO.mItemBNumPosY;
                mItemParams[3].num_scale = g_drawHIO.mItemBNumScale;
                break;
            default:
                mItemParams[3].pos_x = g_drawHIO.mButtonBItemPosX[0];
                mItemParams[3].pos_y = g_drawHIO.mButtonBItemPosY[0];
                mItemParams[3].scale = g_drawHIO.mButtonBItemScale[0];
                mItemParams[3].rotation = g_drawHIO.mButtonBItemRotation[0];
                mItemParams[3].num_pos_x = g_drawHIO.mItemBNumPosX;
                mItemParams[3].num_pos_y = g_drawHIO.mItemBNumPosY;
                mItemParams[3].num_scale = g_drawHIO.mItemBNumScale;
                break;
            }
        }
    }
}

bool dMeter2Draw_c::getFishingType() {
    if (dMeter2Info_isGameStatus(1) && getCanoeFishing()) {
        return true;
    } else {
        return false;
    }
}

// unused
static const char* tex_name[] = {"tt_heart_00.bti", "tt_heart_01.bti", "tt_heart_02.bti",
                                 "tt_heart_03.bti"};

bool dMeter2Draw_c::getCanoeFishing() {
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP127") &&
        ((daPy_py_c*)dComIfGp_getPlayer(0))->checkCanoeRide())
    {
        return TRUE;
    } else {
        return FALSE;
    }
}

bool dMeter2Draw_c::getCameraSubject() {
    return dCam_getBody()->Mode() == 4 ? TRUE : FALSE;
}

bool dMeter2Draw_c::getItemSubject() {
    if (dCam_getBody()->Mode() != 4 && (dCam_getBody()->Mode() == 7 || dCam_getBody()->Mode() == 8))
    {
        return true;
    }

    return false;
}

bool dMeter2Draw_c::getPlayerSubject() {
    return dComIfGp_checkPlayerStatus0(0, 0x8000000);
}

bool dMeter2Draw_c::isBButtonShow(bool param_0) {
    if (dMsgObject_getMsgObjectClass() == NULL) {
        return false;
    }

    if (dMeter2Info_isShopTalkFlag() || dMsgObject_getMsgObjectClass()->isHowlMessage() ||
        daPy_getPlayerActorClass()->checkHawkWait() || dMeter2Info_getItemExplainWindowStatus() ||
        (daPy_getPlayerActorClass()->checkGrassWhistle() && param_0) ||
        (!dComIfGp_event_checkHind(4) && dComIfGp_event_runCheck()))
    {
        return true;
    }

    return false;
}

s16 dMeter2Draw_c::getButtonTimer() {
    return mpButtonParent->getAlphaTimer();
}

// unused
static u64 const tag_7759[] = {
    'item_x_n',
    'item_y_n',
    'item_j_n',
};
