/**
 * d_s_name.cpp
 * File Select Scene
*/

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_name.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "f_op/f_op_scene_mng.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_machine.h"
#include "m_Do/m_Do_mtx.h"
#include "m_Do/m_Do_main.h"
#include "f_op/f_op_overlap_mng.h"
#include "dusk/memory.h"
#include "dusk/settings.h"

#if TARGET_PC
#define SHOW_TV_SETTINGS_SCREEN (this->mShowTvSettingsScreen)
#else
#define SHOW_TV_SETTINGS_SCREEN (1)
#endif

static dSn_HIO_c g_snHIO;

#if VERSION == VERSION_GCN_PAL
static int mBmgStatus;
#endif

dSn_HIO_c::dSn_HIO_c() {
    mFileSelWaitTime = 15;
}

#if DEBUG
void dSn_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("\n*****表示調整用*****", 0);
    mctx->genSlider("ファイル選択開始WaitTime", &mFileSelWaitTime, 0, 120);
}
#endif

static s32 phase_1(char* i_resName) {
    mDoAud_bgmStart(-1);
    if (dComIfG_setObjectRes(i_resName, (u8)0, NULL) == 0) {
        return cPhs_ERROR_e;
    }

    return cPhs_NEXT_e;
}

static s32 phase_2(char* i_resName) {
    int rt = dComIfG_syncObjectRes(i_resName);
    if (rt < 0) {
        return cPhs_ERROR_e;
    }

    if (rt > 0) {
        return cPhs_INIT_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

static s32 resLoad(request_of_phase_process_class* i_phase, char* i_resName) {
    static request_of_phase_process_fn l_method[2] = {
        (request_of_phase_process_fn)phase_1, 
        (request_of_phase_process_fn)phase_2
    };

    return dComLbG_PhaseHandler(i_phase, l_method, i_resName);
}

s32 dScnName_c::create() {
    int phase_state = resLoad(&phase, "fileSel");
    if (phase_state == cPhs_COMPLEATE_e) {
        mHeap = JKRCreateExpHeap(HEAP_SIZE(0x180000, 0x1C0000), mDoExt_getGameHeap(), false);
        JUT_ASSERT(289, mHeap != NULL);
        JKRHEAP_NAME(mHeap, "File select");

        field_0x1d0 = (JKRExpHeap*)mDoExt_setCurrentHeap(mHeap);

        dRes_info_c* resInfo = dComIfG_getObjectResInfo("fileSel");
        JUT_ASSERT(293, resInfo != NULL);

        dFs_c = JKR_NEW dFile_select_c(resInfo->getArchive());
        JUT_ASSERT(297, dFs_c != NULL);


        #if VERSION == VERSION_GCN_PAL
        for (int i = 0; i < 5; i++);
        #endif

        dFs_c->_create();

        if (fpcM_GetName(this) == fpcNm_NAME_SCENE_e) {
            dFs_c->setUseType(0);

            #if !PLATFORM_SHIELD
            dComIfGs_setNoFile(0);
            #endif
        }

        mBrightCheck = JKR_NEW dBrightCheck_c(resInfo->getArchive());

        field_0x420 = 0;
        g_snHIO.id = mDoHIO_CREATE_CHILD("名前登録シーン", &g_snHIO);
        JKRSetCurrentHeap(field_0x1d0);

        dComIfGp_setWindowNum(1);
        dComIfGp_setWindow(0, 0.0f, 0.0f, mDoMch_render_c::getFbWidth(),
                           mDoMch_render_c::getEfbHeight(), 0.0f, 1.0f, 0, 2);
        dComIfGp_setCamera(0, &mCamera);

        dDlst_window_c* window = dComIfGp_getWindow(0);
        view_port_class* viewport = window->getViewPort();

        fopCamM_SetNear(&mCamera, 1.0f);
        fopCamM_SetFar(&mCamera, 100000.0f);
        fopCamM_SetFovy(&mCamera, 45.0f);
        fopCamM_SetAspect(&mCamera, mDoGph_gInf_c::getWidthF() / mDoGph_gInf_c::getHeightF());
        fopCamM_SetEye(&mCamera, 0.0f, 0.0f, -1000.0f);
        fopCamM_SetCenter(&mCamera, 0.0f, 0.0f, 0.0f);
        fopCamM_SetBank(&mCamera, 0);

        dComIfGp_setPlayer(0, NULL);
        dComIfGd_setWindow(window);
        dComIfGd_setViewport(viewport);
        dComIfGd_setView(&mCamera.view);
        mDoGph_gInf_c::offAutoForcus();
        setView();

        dKy_setLight_init();
        mWaitTimer = g_snHIO.mFileSelWaitTime;
        mDrawProc = 0;
        mProc = dScnName_PROC_FileSelectOpen;
        mDoGph_gInf_c::setTickRate(OS_TIMER_CLOCK / 30);

        #if VERSION == VERSION_GCN_PAL
        mBmgStatus = 0;
        #endif

        dComIfGp_getVibration().Init();
    }
    return phase_state;
}

#if VERSION == VERSION_GCN_PAL
static const char* dummyString(int i) {
    switch (i) {
    case 0: return "/res/Msguk/bmgres.arc";
    case 1: return "/res/Msgde/bmgres.arc";
    case 2: return "/res/Msgfr/bmgres.arc";
    case 3: return "/res/Msgsp/bmgres.arc";
    case 4: return "/res/Msgit/bmgres.arc";
    }
}
#endif

void dScnName_c::setView() {
    dDlst_window_c* window = dComIfGp_getWindow(0);
    view_port_class* viewport = window->getViewPort();
    camera_class* camera = &mCamera;

    C_MTXPerspective(camera->view.projMtx, camera->view.fovy, camera->view.aspect, camera->view.near_,
                     camera->view.far_);
    mDoMtx_lookAt(camera->view.viewMtx, &camera->view.lookat.eye, &camera->view.lookat.center,
                  camera->view.bank);
    cMtx_inverse(camera->view.viewMtx, camera->view.invViewMtx);
    MTXCopy(camera->view.viewMtx, camera->view.viewMtxNoTrans);
    camera->view.viewMtxNoTrans[0][3] = 0.0f;
    camera->view.viewMtxNoTrans[1][3] = 0.0f;
    camera->view.viewMtxNoTrans[2][3] = 0.0f;
    j3dSys.setViewMtx(camera->view.viewMtx);
    cMtx_concatProjView(camera->view.projMtx, camera->view.viewMtx, camera->view.projViewMtx);
}

#if PLATFORM_WII || PLATFORM_SHIELD
static void setBaseCsrColor(int i_no) {
    static const GXColor l_csrColor[] = {
        {255, 0, 0 , 0},
        {255, 0, 255 , 0},
        {0, 182, 255 , 0},
        {0, 255, 0 , 0},
        {255, 255, 0 , 0},
    };

    JUT_ASSERT(656, 0 <= i_no && i_no < (sizeof(l_csrColor)/sizeof(GXColor)));

    dComIfG_inf_c::baseCsr_c* baseCsr = dComIfG_inf_c::getBaseCsr();
    JUT_ASSERT(658, baseCsr != NULL);

    dDlst_blo_c* baseCsrBlo = baseCsr->getCsr();
    JUT_ASSERT(660, baseCsrBlo != NULL);

    const JUtility::TColor& color = l_csrColor[i_no];
    baseCsrBlo->setBlackColor('cursor00', color);
}
#endif

#if VERSION == VERSION_GCN_PAL
void dScnName_c::bmg_data_set() {
    if (fopAcM_GetName(this) == fpcNm_NAMEEX_SCENE_e) {
        mBmgStatus = 10;
    }
}

void dScnName_c::tex_data_set() {}
#endif

typedef void (dScnName_c::*mainProcFunc)(void);
static mainProcFunc MainProc[6] = {
    &dScnName_c::FileSelectOpen,  &dScnName_c::FileSelectMain, &dScnName_c::FileSelectClose,
    &dScnName_c::brightCheckOpen, &dScnName_c::brightCheck,    &dScnName_c::changeGameScene,
};

s32 dScnName_c::execute() {
    if (!fopOvlpM_IsPeek()) {
        dComIfG_resetToOpening(this);
    }

    if (mDoRst::isReset() && field_0x420 == 0 && !dFs_c->getFadeFlag()) {
        return 1;
    }

    (this->*MainProc[mProc])();

    #if DEBUG
    if (!fopOvlpM_IsPeek() && dComIfG_isSceneResetButton()) {
        fopScnM_ChangeReq(this, fpcNm_MENU_SCENE_e, 0, 5);
        dComIfGs_init();
        dComIfG_playerStatusD();
    }
    #endif

    return 1;
}

s32 dScnName_c::draw() {
    dComIfGp_getVibration().Run();

    switch (mDrawProc) {
    case 0:
        dFs_c->draw();
        break;
    case 1:
        mBrightCheck->draw();
        break;
    }

    #if VERSION == VERSION_SHIELD_DEBUG
    dComIfGp_particle_calc3D();
    dComIfGp_particle_calc2D();
    #endif
    return 1;
}

dScnName_c::~dScnName_c() {
    mDoHIO_DELETE_CHILD(g_snHIO.id);

    JKR_DELETE(dFs_c);
    JKR_DELETE(mBrightCheck);
    dComIfG_deleteObjectResMain("fileSel");
    mHeap->destroy();

    #if PLATFORM_WII || PLATFORM_SHIELD
    dComIfG_inf_c::baseCsr_c* baseCsr = dComIfG_inf_c::getBaseCsr();
    JUT_ASSERT(870, baseCsr != NULL);
    baseCsr->onNavi();
    setBaseCsrColor(2);
    #endif

    // Reset Hotspring Water bottles to normal water
    for (int i = 0; i < 4; i++) {
        dMeter2Info_changeWater(i + SLOT_11);
    }

    dComIfGp_setWindowNum(0);
    dComIfGd_setView(NULL);
}

void dScnName_c::FileSelectOpen() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    } else {
        mDoAud_bgmStreamPrepare(0x2000000);
        mDoAud_bgmStreamPlay();
        mProc = dScnName_PROC_FileSelectMain;
    }
}

void dScnName_c::FileSelectMain() {
    dFs_c->_move();

    if (fpcM_GetName(this) == fpcNm_NAME_SCENE_e) {
        FileSelectMainNormal();
    }
}

void dScnName_c::FileSelectMainNormal() {
#if TARGET_PC
    mShowTvSettingsScreen = !dusk::getSettings().game.hideTvSettingsScreen;
#endif

    switch(dFs_c->isSelectEnd()) {
    case 1:
        if (SHOW_TV_SETTINGS_SCREEN) {
            mWaitTimer = 15;
            mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
            mDoGph_gInf_c::startFadeOut(15);
        } else {
            mWaitTimer = 1;
        }

        mProc = dScnName_PROC_FileSelectClose;
        field_0x420 = 1;

        if (!SHOW_TV_SETTINGS_SCREEN) {
            mDoAud_seStart(Z2SE_ENTER_GAME, NULL, 0, 0);
        }

        break;
    }
}

void dScnName_c::FileSelectClose() {
    mWaitTimer--;

    if (mWaitTimer == 0) {
        if (SHOW_TV_SETTINGS_SCREEN) {
            mProc = dScnName_PROC_BrightCheckOpen;
            mWaitTimer = 15;
            mDrawProc = 1;
            mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
            mDoGph_gInf_c::startFadeIn(15);
            field_0x420 = 0;
        } else {
            doPreLoadSetup();
            field_0x420 = 0;
        }
    }
}

void dScnName_c::brightCheckOpen() {
    if (!mDoRst::isReset()) {
        mWaitTimer--;
        if (mWaitTimer == 0) {
            mProc = dScnName_PROC_BrightCheck;
        }
    }
}

void dScnName_c::brightCheck() {
    mBrightCheck->_move();

    if (mBrightCheck->isEnd()) {
        doPreLoadSetup();
    }
}

void dScnName_c::doPreLoadSetup() {
    dComIfGs_setSaveTotalTime(dComIfGs_getTotalTime());
    dComIfGs_setSaveStartTime(OSGetTime());
    mDoAud_bgmStop(45);

    field_0x41f = 0;
    mProc = dScnName_PROC_ChangeGameScene;

    #if TARGET_PC
    if (dusk::getSettings().game.disableRupeeCutscenes) {
        return;
    }
    #endif

    // Reset rupee "first-time collection" flags so the collection cutscene will play again
    dComIfGs_offItemFirstBit(dItemNo_GREEN_RUPEE_e);
    dComIfGs_offItemFirstBit(dItemNo_BLUE_RUPEE_e);
    dComIfGs_offItemFirstBit(dItemNo_YELLOW_RUPEE_e);
    dComIfGs_offItemFirstBit(dItemNo_RED_RUPEE_e);
    dComIfGs_offItemFirstBit(dItemNo_PURPLE_RUPEE_e);
    dComIfGs_offItemFirstBit(dItemNo_ORANGE_RUPEE_e);
    dComIfGs_offItemFirstBit(dItemNo_SILVER_RUPEE_e);
}

void dScnName_c::changeGameScene() {
    if (!mDoRst::isReset() && !fopOvlpM_IsPeek()) {
        dComIfGs_gameStart();

        #if DEBUG
        // Goto Map Select debug menu if opening File 1 with development mode on
        if (dFs_c->getSelectNum() == 0 && mDoMain::developmentMode) {
            fopScnM_ChangeReq(this, fpcNm_MENU_SCENE_e, 0, 5);
            dComIfGs_init();
            dComIfG_playerStatusD();
            return;
        }
        #endif

        #if VERSION == VERSION_SHIELD_DEBUG
        fopScnM_ChangeReq(this, fpcNm_PLAY_SCENE_e, 0, 5);
        #else
        fopScnM_ChangeReq(this, field_0x41f == 0 ? fpcNm_PLAY_SCENE_e : fpcNm_PLAY_SCENE_e, 0, 5);
        #endif
        dComIfGp_offEnableNextStage();

        if (dFs_c->isDataNew(dFs_c->getSelectNum())) {
            dComIfGp_setNextStage("F_SP108", 21, 1, 13);
        }
        
        dKy_clear_game_init();
        dComIfGs_resetDan();
        dComIfGs_setRestartRoomParam(0);
    }
}

static int dScnName_Draw(dScnName_c* i_this) {
    return i_this->draw();
}

static int dScnName_Execute(dScnName_c* i_this) {
    return i_this->execute();
}

static int dScnName_IsDelete(dScnName_c*) {
    dComIfGp_particle_cleanup();
    return 1;
}

static int dScnName_Delete(dScnName_c* i_this) {
    #if VERSION == VERSION_GCN_PAL
    i_this->bmg_data_set();
    i_this->tex_data_set();
    mBmgStatus = 0;
    #endif

    i_this->~dScnName_c();

    #if VERSION == VERSION_SHIELD_DEBUG
    dComIfGp_particle_removeScene(true);
    #endif

    return 1;
}

static int dScnName_Create(scene_class* i_this) {
    JKR_NEW_ARGS (i_this) dScnName_c;
    return static_cast<dScnName_c*>(i_this)->create();
}

static scene_method_class l_dScnName_Method = {
    (process_method_func)dScnName_Create, (process_method_func)dScnName_Delete,
    (process_method_func)dScnName_Execute, (process_method_func)dScnName_IsDelete,
    (process_method_func)dScnName_Draw,
};

scene_process_profile_definition g_profile_NAME_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NAME_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(dScnName_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_dScnName_Method,
                       0,
};

scene_process_profile_definition g_profile_NAMEEX_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NAMEEX_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(dScnName_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_dScnName_Method,
                       0,
};
