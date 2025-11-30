#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_title.h"
#include "d/d_demo.h"
#include "d/d_pane_class_alpha.h"
#include "d/d_menu_collect.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "d/d_com_inf_game.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "f_op/f_op_overlap_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_scene_mng.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "m_Do/m_Do_graphic.h"

class daTit_HIO_c {
public:
    daTit_HIO_c();

    virtual ~daTit_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mPSScaleX;
    /* 0x0C */ f32 mPSScaleY;
    /* 0x10 */ f32 mPSPosX;
    /* 0x14 */ f32 mPSPosY;
    /* 0x18 */ u8 mAppear;
    /* 0x19 */ u8 mArrow;
    /* 0x1A */ u8 field_0x1a;
};

static daTit_HIO_c g_daTitHIO;

static u8 const lit_3772[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#if VERSION == VERSION_GCN_PAL
static char const l_arcName[] = "TitlePal";
#else
static char const l_arcName[] = "Title";
#endif

static procFunc daTitleProc[6] = {
    &daTitle_c::loadWait_proc, &daTitle_c::logoDispWait, &daTitle_c::logoDispAnm,
    &daTitle_c::keyWait, &daTitle_c::nextScene_proc, &daTitle_c::fastLogoDisp,
};

daTit_HIO_c::daTit_HIO_c() {
    mPSScaleX = 1.0f;
    mPSScaleY = 1.0f;

    #if VERSION == VERSION_GCN_PAL

    switch (OSGetLanguage()) {
    case OS_LANGUAGE_ENGLISH:
    case OS_LANGUAGE_GERMAN:
    case OS_LANGUAGE_SPANISH:
    case OS_LANGUAGE_ITALIAN:
    case OS_LANGUAGE_DUTCH:
        mPSPosX = 303.0f;
        break;
    case OS_LANGUAGE_FRENCH:
        mPSPosX = FB_WIDTH / 2;
        break;
    }
    #else
    mPSPosX = 303.0f;
    #endif

    mPSPosY = 347.0f;
    mAppear = 15;
    mArrow = 60;
    field_0x1a = 15;
}

int daTitle_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 10);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000285);

    if (mpModel == NULL) {
        return 0;
    }

    void* res = dComIfG_getObjectRes(l_arcName, 7);
    mBck.init((J3DAnmTransform*)res, 1, 0, 2.0f, 0, -1, false);

    res = dComIfG_getObjectRes(l_arcName, 13);
    mBpk.init(modelData, (J3DAnmColor*)res, 1, 0, 2.0f, 0, -1);

    res = dComIfG_getObjectRes(l_arcName, 16);
    mBrk.init(modelData, (J3DAnmTevRegKey*)res, 1, 0, 2.0f, 0, -1);

    res = dComIfG_getObjectRes(l_arcName, 19);
    mBtk.init(modelData, (J3DAnmTextureSRTKey*)res, 1, 0, 2.0f, 0, -1);

    return 1;
}

int daTitle_c::create() {
    fopAcM_ct(this, daTitle_c);
    
    int phase_state = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x4000)) {
        return cPhs_ERROR_e;
    }

    mpMount = mDoDvdThd_mountArchive_c::create("/res/Layout/Title2D.arc", 0, NULL);
    field_0x5f8 = 0;
    field_0x5f9 = 0;

    m2DHeap = JKRExpHeap::create(0x8000, mDoExt_getGameHeap(), false);
    loadWait_init();
    g_daTitHIO.field_0x4 = -1;

    return phase_state;
}

int daTitle_c::createHeapCallBack(fopAc_ac_c* title) {
    return ((daTitle_c*)title)->CreateHeap();
}

int daTitle_c::Execute() {
    if (fopOvlpM_IsPeek()) {
        return 1;
    }

    dMenu_Collect3D_c::mViewOffsetY = 0.0f;

    if (mDoRst::isReset()) {
        return 1;
    }

    (this->*daTitleProc[mProcID])();
    KeyWaitAnm();
    return 1;
}

void daTitle_c::KeyWaitAnm() {
    if (field_0x5f9 != 0) {
        if (field_0x604 == 0) {
            if (field_0x5fa != 0) {
                field_0x600->alphaAnime(g_daTitHIO.mArrow, 0, 255, 0);
            } else {
                field_0x600->alphaAnimeLoop(g_daTitHIO.mArrow, 255, 128, 0);
            }

            if (field_0x600->getAlpha() == 255) {
                if (field_0x5fa != 0) {
                    field_0x5fa = 0;
                }
                field_0x604 = g_daTitHIO.field_0x1a;
            }
        }

        if (field_0x604 != 0) {
            field_0x604--;
        }
    }
}

void daTitle_c::loadWait_init() {
    mProcID = 0;
}

void daTitle_c::loadWait_proc() {
    if (mpMount->sync()) {
        JKRHeap* heap = mDoExt_setCurrentHeap(m2DHeap);
        mpHeap = heap;

        mpFont = mDoExt_getMesgFont();
        mTitle.Scr = new J2DScreen();

        mTitle.Scr->setPriority("zelda_press_start.blo", 0x100000, mpMount->getArchive());

        J2DTextBox* text[7];
        text[0] = (J2DTextBox*)mTitle.Scr->search('t_s_00');
        text[1] = (J2DTextBox*)mTitle.Scr->search('t_s_01');
        text[2] = (J2DTextBox*)mTitle.Scr->search('t_s_02');
        text[3] = (J2DTextBox*)mTitle.Scr->search('t_s_03');
        text[4] = (J2DTextBox*)mTitle.Scr->search('t_s_04');
        text[5] = (J2DTextBox*)mTitle.Scr->search('t_s_05');
        text[6] = (J2DTextBox*)mTitle.Scr->search('t_o');

        for (int i = 0; i < 7; i++) {
            text[i]->setFont(mpFont);
            text[i]->setString(0x80, "");
            fopMsgM_messageGet(text[i]->getStringPtr(), 100);
        }

        field_0x600 = new CPaneMgrAlpha(mTitle.Scr, 'n_all', 2, NULL);
        field_0x600->setAlpha(0);
        J2DPane* pane = mTitle.Scr->search('n_all');
        pane->translate(g_daTitHIO.mPSPosX, g_daTitHIO.mPSPosY);
        pane->scale(g_daTitHIO.mPSScaleX, g_daTitHIO.mPSScaleY);
        mpHeap->becomeCurrentHeap();
        logoDispWaitInit();
    }
}

void daTitle_c::logoDispWaitInit() {
    mProcID = 1;
}

void daTitle_c::logoDispWait() {
    if (mDoCPd_c::getTrigA(PAD_1) || mDoCPd_c::getTrigStart(PAD_1)) {
        fastLogoDispInit();
    } else if (getDemoPrm() == 1) {
        logoDispAnmInit();
    }
}

void daTitle_c::logoDispAnmInit() {
    mBck.setPlaySpeed(1.0f);
    mBpk.setPlaySpeed(1.0f);
    mBrk.setPlaySpeed(1.0f);
    mBtk.setPlaySpeed(1.0f);
    field_0x5f8 = 1;
    mProcID = 2;
}

void daTitle_c::logoDispAnm() {
    mBck.play();
    mBpk.play();
    mBrk.play();
    mBtk.play();

    if (mBrk.isStop() && mBtk.isStop() && mBck.isStop() && mBpk.isStop()) {
        field_0x600->alphaAnimeStart(0);
        field_0x604 = 0;
        field_0x5f9 = 1;
        field_0x5fa = 1;
        keyWaitInit();
    }
}

void daTitle_c::keyWaitInit() {
    mProcID = 3;
}

void daTitle_c::keyWait() {
    if (mDoCPd_c::getTrigA(PAD_1) || mDoCPd_c::getTrigStart(PAD_1)) {
        mDoAud_seStart(Z2SE_TITLE_ENTER, NULL, 0, 0);
        nextScene_init();
    }
}

void daTitle_c::nextScene_init() {
    mProcID = 4;
}

void daTitle_c::nextScene_proc() {
    if (!fopOvlpM_IsPeek() && !mDoRst::isReset()) {
        scene_class* playScene = fopScnM_SearchByID(dStage_roomControl_c::getProcID());
        JUT_ASSERT(706, playScene != NULL);
        fopScnM_ChangeReq(playScene, 13, 0, 5);
#if VERSION != VERSION_SHIELD_DEBUG
        mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
#endif
    }
}

void daTitle_c::fastLogoDispInit() {
    mBck.setFrame(mBck.getEndFrame() - 1.0f);
    mBpk.setFrame(mBpk.getEndFrame() - 1.0f);
    mBrk.setFrame(mBrk.getEndFrame() - 1.0f);
    mBtk.setFrame(mBtk.getEndFrame() - 1.0f);

    field_0x600->alphaAnimeStart(0);
    field_0x604 = 0;
    field_0x5fc = 30;
    mProcID = 5;
}

void daTitle_c::fastLogoDisp() {
    if (field_0x5fc != 0) {
        field_0x5fc--;
        return;
    }

    field_0x5f9 = 1;
    field_0x5fa = 1;
    field_0x5f8 = 1;
    keyWaitInit();
}

int daTitle_c::getDemoPrm() {
    dDemo_actor_c* demoActor = dDemo_c::getActor(demoActorID);
    dDemo_prm_c* prm;
    if (demoActor != NULL && demoActor->checkEnable(1) &&
        (prm = demoActor->getPrm()))
    {
        void* data = (void*)prm->getData();
        JStudio::stb::TParseData_fixed<49> aTStack_30(data);
        TValueIterator_raw<u8> iter = aTStack_30.begin();
        return *iter & 0xff;
    }
    return -1;
}

int daTitle_c::Draw() {
    J3DModelData* modelData = mpModel->getModelData();
    MTXTrans(mpModel->getBaseTRMtx(), 0.0f, 0.0f, -430.0f);
    mpModel->getBaseScale()->x = -1.0f;

    mBck.entry(modelData);
    mBpk.entry(modelData);
    mBrk.entry(modelData);
    mBtk.entry(modelData);

    dComIfGd_setListItem3D();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    if (field_0x5f8) {
        dComIfGd_set2DOpaTop(&mTitle);
    }

    return 1;
}

int daTitle_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    delete mTitle.Scr;
    delete field_0x600;
    
    mpMount->getArchive()->removeResourceAll();
    mpMount->getArchive()->unmount();
    delete mpMount;

    if (m2DHeap != NULL) {
        m2DHeap->destroy();
    }

    return 1;
}

static int daTitle_Draw(daTitle_c* i_this) {
    return i_this->Draw();
}

static int daTitle_Execute(daTitle_c* i_this) {
    return i_this->Execute();
}

static int daTitle_Delete(daTitle_c* i_this) {
    return i_this->Delete();
}

static int daTitle_Create(fopAc_ac_c* i_this) {
    return static_cast<daTitle_c*>(i_this)->create();
}

void dDlst_daTitle_c::draw() {
    J2DGrafContext* ctx = dComIfGp_getCurrentGrafPort();
    Scr->draw(0.0f, 0.0f, ctx);
}

static actor_method_class l_daTitle_Method = {
    (process_method_func)daTitle_Create,
    (process_method_func)daTitle_Delete,
    (process_method_func)daTitle_Execute,
    (process_method_func)NULL,
    (process_method_func)daTitle_Draw,
};

extern actor_process_profile_definition g_profile_TITLE = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_TITLE,              // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daTitle_c),       // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  0xa,                     // mPriority
  &l_daTitle_Method, // sub_method
  0x00044000,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
