#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_gameover.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_scrn_light.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/d_pane_class.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"

class dGov_HIO_c : public mDoHIO_entry_c {
public:
    dGov_HIO_c();
    virtual ~dGov_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 unk_0x4;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 mAlpha;
    /* 0x10 */ f32 mAnimSpeed;
    /* 0x14 */ GXColor mBlack;
    /* 0x18 */ GXColor mWhite;
};  // Size: 0x1C

static dGov_HIO_c l_HIO;

void dDlst_Gameover_CAPTURE_c::draw() {
    GXTexObj tex_obj;
    Mtx44 m;

    GXSetTexCopySrc(0, 0, FB_WIDTH, FB_HEIGHT);
    GXSetTexCopyDst(FB_WIDTH / 2, FB_HEIGHT / 2, GX_TF_RGB565, 1);
    GXCopyTex(mDoGph_gInf_c::mZbufferTex, 0);
    GXPixModeSync();
    GXInitTexObj(&tex_obj, mDoGph_gInf_c::mFrameBufferTex, FB_WIDTH / 2, FB_HEIGHT / 2,
                 (GXTexFmt)mDoGph_gInf_c::mFrameBufferTimg->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tex_obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                    GX_ANISO_1);
    GXLoadTexObj(&tex_obj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetZCompLoc(1);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_OR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(GX_FALSE, 0, NULL);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(1);
    C_MTXOrtho(m, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f);
    GXSetProjection(m, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(g_mDoMtx_identity, GX_PNMTX0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB8, 0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3s8(0, 0, -5);
    GXTexCoord2s8(0, 0);

    GXPosition3s8(1, 0, -5);
    GXTexCoord2s8(1, 0);

    GXPosition3s8(1, 1, -5);
    GXTexCoord2s8(1, 1);

    GXPosition3s8(0, 1, -5);
    GXTexCoord2s8(0, 1);

    GXEnd();
}

typedef void (dGameover_c::*initFunc)();
static initFunc init_process[] = {
    &dGameover_c::playerAnmWait_init, &dGameover_c::dispFadeOut_init, &dGameover_c::dispWait_init,
    &dGameover_c::demoFadeIn_init,    &dGameover_c::demoFadeOut_init, &dGameover_c::saveOpen_init,
    &dGameover_c::saveMove_init,      &dGameover_c::saveClose_init,   &dGameover_c::deleteWait_init,
};

typedef void (dGameover_c::*moveFunc)();
static moveFunc move_process[] = {
    &dGameover_c::playerAnmWait_proc, &dGameover_c::dispFadeOut_proc, &dGameover_c::dispWait_proc,
    &dGameover_c::demoFadeIn_proc,    &dGameover_c::demoFadeOut_proc, &dGameover_c::saveOpen_proc,
    &dGameover_c::saveMove_proc,      &dGameover_c::saveClose_proc,   &dGameover_c::deleteWait_proc,
};

dGov_HIO_c::dGov_HIO_c() {
    mScale = 2.7f;
    mAlpha = 1.0f;
    mAnimSpeed = 0.2f;

    mBlack.r = 0;
    mBlack.g = 0;
    mBlack.b = 0;
    mBlack.a = 0;

    mWhite.r = 255;
    mWhite.g = 220;
    mWhite.b = 125;
    mWhite.a = 255;
}

int dGameover_c::_create() {
    int phase = dComIfG_resLoad(&mPhase, "Gover");

    if (dComIfGp_isPauseFlag() ||
        (dComIfGp_isHeapLockFlag() != 0 && dComIfGp_isHeapLockFlag() != 6) ||
        dComIfGp_getMesgStatus() != 0)
    {
        return 0;
    }

    if (phase == cPhs_COMPLEATE_e) {
        if (dMeter2Info_getGameOverType() == 0) {
            mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
            dComIfGs_addDeathCount();
        }

        dRes_info_c* resInfo = dComIfG_getObjectResInfo("Gover");
        JUT_ASSERT(0, resInfo != NULL);

        mpHeap = (JKRHeap*)dComIfGp_getExpHeap2D();
        dComIfGp_setHeapLockFlag(6);
        JKRHeap* old_heap = mDoExt_setCurrentHeap(mpHeap);
        int temp = mpHeap->getTotalFreeSize();

        dgo_screen_c = new dDlst_GameOverScrnDraw_c(resInfo->getArchive());
        dMs_c = new dMenu_save_c();
        JUT_ASSERT(0, dMs_c != NULL);

        if (dMeter2Info_getGameOverType() == 1) {
            if (!strcmp(dComIfGp_getLastPlayStageName(), "D_MN10A")) {
                // Last stage was Stallord Arena
                // Remove Ooccoo from inventory
                dComIfGs_setItem(SLOT_18, fpcNm_ITEM_NONE);
                dComIfGs_resetLastWarpAcceptStage();
            }

            dMs_c->setUseType(3);
        } else if (dMeter2Info_getGameOverType() == 2) {
            dMs_c->setUseType(4);
        } else {
            dMs_c->setUseType(2);
        }

        dMs_c->_create();
        dgo_capture_c = new dDlst_Gameover_CAPTURE_c();
        JUT_ASSERT(0, dgo_capture_c != NULL);

        OS_REPORT("game over create size ===> %d\n", temp - mpHeap->getTotalFreeSize());

        mDoExt_setCurrentHeap(old_heap);
    } else {
        return phase;
    }

    field_0x116 = g_menuHIO.mGameover;

    if (dMeter2Info_getGameOverType() == 1 || dMeter2Info_getGameOverType() == 2) {
        mIsDemoSave = true;
        mProc = PROC_DEMO_FADE_IN;
        dgo_screen_c->setBackAlpha(1.0f);
    } else {
        mIsDemoSave = false;
        mProc = PROC_PLAYER_ANM_WAIT;
    }

    (this->*init_process[mProc])();
    return cPhs_COMPLEATE_e;

    return phase;
}


int dGameover_c::_execute() {
    JKRHeap* old_heap = mDoExt_setCurrentHeap(mpHeap);
    u8 old_proc = mProc;

    (this->*move_process[mProc])();
    if (mProc != old_proc) {
        (this->*init_process[mProc])();
    }

    mDoExt_setCurrentHeap(old_heap);
    return 1;
}


void dGameover_c::playerAnmWait_init() {}

void dGameover_c::playerAnmWait_proc() {
    if (mIsDemoSave) {
        mProc = PROC_DISP_FADE_OUT;
    }
}

void dGameover_c::dispFadeOut_init() {
    mTimer = 30;
    mDoGph_gInf_c::startFadeOut(15);
}

void dGameover_c::dispFadeOut_proc() {
    if (mTimer != 0) {
        mTimer--;
    } else {
        mProc = PROC_DISP_WAIT;
    }
}

void dGameover_c::dispWait_init() {
    mTimer = 90;
    mDoGph_gInf_c::startFadeIn(30);
}

void dGameover_c::dispWait_proc() {
    if (mTimer != 0) {
        mTimer--;
    } else {
        mProc = PROC_SAVE_OPEN;
    }
}

void dGameover_c::demoFadeIn_init() {
    mTimer = 0;
}

void dGameover_c::demoFadeIn_proc() {
    mTimer++;
    dgo_screen_c->setBackAlpha(1.0f);

    if (mTimer >= 30) {
        mProc = PROC_SAVE_OPEN;
    }
}

void dGameover_c::demoFadeOut_init() {
    mTimer = 30;
}

void dGameover_c::demoFadeOut_proc() {
    mTimer--;
    dgo_screen_c->setBackAlpha((mTimer * mTimer) / 900.0f);

    if (mTimer <= 0) {
        mProc = PROC_DELETE_WAIT;
    }
}

void dGameover_c::saveOpen_init() {}

void dGameover_c::saveOpen_proc() {
    if (dMs_c->_open()) {
        mProc = PROC_SAVE_MOVE;
    }
}

void dGameover_c::saveMove_init() {}

void dGameover_c::saveMove_proc() {
    dMs_c->_move();

    if (dMs_c->getSaveStatus() == 3) {
        mProc = PROC_SAVE_CLOSE;
    }
}

void dGameover_c::saveClose_init() {}

void dGameover_c::saveClose_proc() {
    if (dMs_c->getEndStatus() == 0) {
        dComIfGp_setGameoverStatus(3);
        dComIfGp_offPauseFlag();
        mDoRst::onReset();
    } else if (dMs_c->getEndStatus() == 1) {
        if (dMeter2Info_getGameOverType() == 1 || dMeter2Info_getGameOverType() == 2) {
            dComIfGp_setGameoverStatus(1);
        } else {
            dComIfGp_setGameoverStatus(2);
        }

        dComIfGp_offPauseFlag();

        // Reset Monkey lantern steal sequence flags if player hasn't regained lantern
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[226])) {
            dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[224]);
            dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[225]);
        }

        // Reset Lantern and oil status back into inventory if gameover during monkey steal sequence
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_KANTERA) && dComIfGs_getItem(SLOT_1, true) == fpcNm_ITEM_NONE) {
            dComIfGs_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
            dComIfGs_setOil(dMeter2Info_getOilGaugeBackUp());
        }

        dMeter2Info_resetMiniGameItem(false);
    }

    if (dMeter2Info_getGameOverType() == 1 && dMeter2Info_getGameOverType() == 2 &&
        dComIfGp_getGameoverStatus() == 1)
    {
        mProc = PROC_DEMO_FADE_OUT;
    } else {
        mProc = PROC_DELETE_WAIT;
    }
}

void dGameover_c::deleteWait_init() {}

void dGameover_c::deleteWait_proc() {}

int dGameover_c::_draw() {
    if (dgo_capture_c != NULL && dComIfGp_isPauseFlag()) {
        dComIfGd_set2DOpa(dgo_capture_c);
    }

    if (mIsDemoSave && mProc >= PROC_DISP_WAIT) {
        if (dgo_screen_c != NULL) {
            dComIfGd_set2DOpa(dgo_screen_c);
        }
        dMs_c->_draw2();
    }

    return 1;
}

int dGameover_c::_delete() {
    JKRHeap* old_heap = mDoExt_setCurrentHeap(mpHeap);

    if (dgo_screen_c != NULL) {
        delete dgo_screen_c;
        dgo_screen_c = NULL;
    }

    dMs_c->_delete();
    delete dMs_c;
    delete dgo_capture_c;

    mpHeap->freeAll();
    dComIfGp_offHeapLockFlag(0);
    mDoExt_setCurrentHeap(old_heap);

    dComIfG_resDelete(&mPhase, "Gover");
    return 1;
}

dDlst_GameOverScrnDraw_c::dDlst_GameOverScrnDraw_c(JKRArchive* i_archive) {
    mpScreen = new J2DScreen();
    mpScreen->setPriority("zelda_game_over.blo", 0x100000, i_archive);
    dPaneClass_showNullPane(mpScreen);

    mFadeColor.set(0, 0, 0, 0);

    if (dMeter2Info_getGameOverType() != 0) {
        mpScreen->search('n_base')->hide();

        if (mDoGph_gInf_c::getFadeRate() == 1.0f) {
            mFadeColor = mDoGph_gInf_c::getFadeColor();
        }
    }

    mpScreen->search('base_b')->hide();

    JUtility::TColor img_white(mFadeColor);
    JUtility::TColor img_black(mFadeColor);
    img_white.a = 0;
    img_black.a = 255;

    ResTIMG* img = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBackImg =
        new J2DPicture('PICT01', JGeometry::TBox2<f32>(0.0f, 486.0f, 0.0f, 660.0f), img, NULL);
    mpBackImg->setBlackWhite(img_white, img_black);

    J2DTextBox* gold_tbox = (J2DTextBox*)mpScreen->search('gold_00');
    gold_tbox->setFont(mDoExt_getSubFont());

    char string[64];
    dMeter2Info_getString(0x381, string, NULL);
    gold_tbox->setString(string);

    mpLight = new dMsgScrnLight_c(6, 0xFF);
    field_0x10 = 0.0f;
}


dDlst_GameOverScrnDraw_c::~dDlst_GameOverScrnDraw_c() {
    delete mpBackImg;
    delete mpScreen;
    dComIfGp_getMain2DArchive()->removeResourceAll();
}

void dDlst_GameOverScrnDraw_c::setBackAlpha(f32 i_alpha) {
    mpBackImg->setAlpha(i_alpha * 255.0f);
}

void dDlst_GameOverScrnDraw_c::draw() {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    graf_ctx->setup2D();

    if (dMeter2Info_getGameOverType() == 1 || dMeter2Info_getGameOverType() == 2) {
        if (mDoGph_gInf_c::getFadeRate() == 1.0f) {
            mFadeColor = mDoGph_gInf_c::getFadeColor();
        }

        JUtility::TColor img_black(mFadeColor);
        JUtility::TColor img_white(mFadeColor);
        img_black.a = 0;
        img_white.a = 255;

        mpBackImg->setBlackWhite(img_black, img_white);
        mpBackImg->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, false, false, false);
    } else {
        JUtility::TColor img_black;
        JUtility::TColor img_white;

        img_black.r = l_HIO.mBlack.r;
        img_black.g = l_HIO.mBlack.g;
        img_black.b = l_HIO.mBlack.b;
        img_black.a = l_HIO.mBlack.a;

        img_white.r = l_HIO.mWhite.r;
        img_white.g = l_HIO.mWhite.g;
        img_white.b = l_HIO.mWhite.b;
        img_white.a = l_HIO.mWhite.a;

        mpBackImg->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, false, false, false);

        static f32 offset[8] = {-138.0f, -96.0f, -56.0f, -18.0f, 42.0f, 75.0f, 110.0f, 143.0f};

        for (int i = 0; i < 8; i++) {
            mpLight->draw(&field_0x10, offset[i] + (FB_WIDTH / 2.0f), (FB_HEIGHT / 2), l_HIO.mScale, l_HIO.mScale,
                          l_HIO.mAlpha, l_HIO.mAnimSpeed, img_black, img_white);
        }
    }

    mpScreen->draw(0.0f, 0.0f, graf_ctx);
}


static int dGameover_Draw(dGameover_c* i_this) {
    return i_this->_draw();
}

static int dGameover_Execute(dGameover_c* i_this) {
    return i_this->_execute();
}

static int dGameover_IsDelete(dGameover_c* i_this) {
    return 1;
}

static int dGameover_Delete(dGameover_c* i_this) {
    return i_this->_delete();
}

static int dGameover_Create(msg_class* i_this) {
    return static_cast<dGameover_c*>(i_this)->_create();
}

s32 d_GameOver_Create(u8 i_gameoverType) {
    dMeter2Info_setGameOverType(i_gameoverType);
    return fopMsgM_create(PROC_GAMEOVER, NULL, NULL, NULL, NULL, NULL);
}

bool d_GameOver_Delete(fpc_ProcID& i_id) {
    if (i_id != fpcM_ERROR_PROCESS_ID_e) {
        fopMsgM_Delete(fopMsgM_SearchByID(i_id));
        i_id = fpcM_ERROR_PROCESS_ID_e;
        dMeter2Info_setGameOverType(0);
        return true;
    }

    return false;
}

static leafdraw_method_class l_dGameover_Method = {
    (process_method_func)dGameover_Create,
    (process_method_func)dGameover_Delete,
    (process_method_func)dGameover_Execute,
    (process_method_func)dGameover_IsDelete,
    (process_method_func)dGameover_Draw,
};

msg_process_profile_definition g_profile_GAMEOVER = {
    fpcLy_CURRENT_e,
    0x0C,
    fpcPi_CURRENT_e,
    PROC_GAMEOVER,
    &g_fpcLf_Method.base,
    sizeof(dGameover_c),
    0,
    0,
    &g_fopMsg_Method,
    0x303,
    &l_dGameover_Method,
};
