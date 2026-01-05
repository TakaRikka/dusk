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
#include "f_op/f_op_overlap_mng.h"

static dSn_HIO_c g_snHIO;

#if VERSION == VERSION_GCN_PAL
static int mBmgStatus;
#endif

typedef void (dScnName_c::*mainProcFunc)(void);
static mainProcFunc MainProc[6] = {
    &dScnName_c::FileSelectOpen,  &dScnName_c::FileSelectMain, &dScnName_c::FileSelectClose,
    &dScnName_c::brightCheckOpen, &dScnName_c::brightCheck,    &dScnName_c::changeGameScene,
};

dSn_HIO_c::dSn_HIO_c() {
    mFileSelWaitTime = 15;
}

static s32 phase_1(char* resName) {
    mDoAud_bgmStart(-1);
    if (dComIfG_setObjectRes(resName, (u8)0, NULL) == 0) {
        return cPhs_ERROR_e;
    }

    return cPhs_NEXT_e;
}

static s32 phase_2(char* resName) {
    int syncStatus = dComIfG_syncObjectRes(resName);

    if (syncStatus < 0) {
        return cPhs_ERROR_e;
    }

    if (syncStatus > 0) {
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
    int phase_state = resLoad(&field_0x1c4, "fileSel");
    if (phase_state == cPhs_COMPLEATE_e) {
        mHeap = JKRExpHeap::create(0x180000, mDoExt_getGameHeap(), false);
        field_0x1d0 = (JKRExpHeap*)mDoExt_setCurrentHeap(mHeap);

        dRes_info_c* res = dComIfG_getObjectResInfo("fileSel");
        dFs_c = new dFile_select_c(res->getArchive());

        #if VERSION == VERSION_GCN_PAL
        for (int i = 0; i < 5; i++);
        #endif

        dFs_c->_create();

        if (fpcM_GetName(this) == PROC_NAME_SCENE) {
            dFs_c->setUseType(0);
            dComIfGs_setNoFile(0);
        }

        mBrightCheck = new dBrightCheck_c(res->getArchive());

        field_0x420 = 0;
        g_snHIO.field_0x4 = -1;
        field_0x1d0->becomeCurrentHeap();

        dComIfGp_setWindowNum(1);
        dComIfGp_setWindow(0, 0.0f, 0.0f, mDoMch_render_c::getFbWidth(),
                           mDoMch_render_c::getEfbHeight(), 0.0f, 1.0f, 0, 2);
        dDlst_window_c* window = dComIfGp_getWindow(0);
        dComIfGp_setCamera(0, (camera_class*)&mCamera);

        mCamera.near = 1.0f;
        mCamera.far = 100000.0f;
        mCamera.fovy = 45.0f;
        mCamera.aspect = mDoGph_gInf_c::getWidthF() / mDoGph_gInf_c::getHeightF();
        mCamera.lookat.eye.set(0.0f, 0.0f, -1000.0f);
        mCamera.lookat.center.set(0.0f, 0.0f, 0.0f);
        mCamera.bank = 0;

        dComIfGp_setPlayer(0, NULL);
        dComIfGd_setWindow(window);
        dComIfGd_setViewport(window->getViewPort());
        dComIfGd_setView(&mCamera);
        mDoGph_gInf_c::offAutoForcus();
        setView();

        dKy_setLight_init();
        field_0x41e = g_snHIO.mFileSelWaitTime;
        field_0x41c = 0;
        field_0x41d = 0;
        mDoGph_gInf_c::setTickRate((OS_BUS_CLOCK / 4) / 30);

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
    C_MTXPerspective(mCamera.projMtx, mCamera.fovy, mCamera.aspect, mCamera.near,
                     mCamera.far);
    mDoMtx_lookAt(mCamera.viewMtx, &mCamera.lookat.eye, &mCamera.lookat.center,
                  mCamera.bank);
    MTXInverse(mCamera.viewMtx, mCamera.invViewMtx);
    MTXCopy(mCamera.viewMtx, mCamera.viewMtxNoTrans);
    mCamera.viewMtxNoTrans[0][3] = 0.0f;
    mCamera.viewMtxNoTrans[1][3] = 0.0f;
    mCamera.viewMtxNoTrans[2][3] = 0.0f;
    MTXCopy(mCamera.viewMtx, j3dSys.mViewMtx);
    mDoMtx_concatProjView(mCamera.projMtx, mCamera.viewMtx, mCamera.projViewMtx);
}

#if VERSION == VERSION_GCN_PAL
void dScnName_c::bmg_data_set(){
    if (fopAcM_GetName(this) == PROC_NAMEEX_SCENE) {
        mBmgStatus = 10;
    }
}

void dScnName_c::tex_data_set() {}
#endif

s32 dScnName_c::execute() {
    if (fopOvlpM_IsPeek() == 0) {
        dComIfG_resetToOpening(this);
    }

    if (mDoRst::isReset() && field_0x420 == 0 && !dFs_c->getFadeFlag()) {
        return 1;
    }

    (this->*MainProc[field_0x41d])();
    return 1;
}

s32 dScnName_c::draw() {
    dComIfGp_getVibration().Run();

    switch (field_0x41c) {
    case 0:
        dFs_c->_draw();
        break;
    case 1:
        mBrightCheck->_draw();
        break;
    }

    return 1;
}

dScnName_c::~dScnName_c() {
    delete dFs_c;
    delete mBrightCheck;
    dComIfG_deleteObjectResMain("fileSel");
    mHeap->destroy();

    for (int i = 0; i < 4; i++) {
        dMeter2Info_changeWater(i + SLOT_11);
    }

    dComIfGp_setWindowNum(0);
    dComIfGd_setView(NULL);
}

void dScnName_c::FileSelectOpen() {
    if (field_0x41e != 0) {
        field_0x41e--;
    } else {
        mDoAud_bgmStreamPrepare(0x2000000);
        mDoAud_bgmStreamPlay();
        field_0x41d = 1;
    }
}

void dScnName_c::FileSelectMain() {
    dFs_c->_move();

    if (fpcM_GetName(this) == PROC_NAME_SCENE) {
        FileSelectMainNormal();
    }
}

void dScnName_c::FileSelectMainNormal() {
    switch(dFs_c->isSelectEnd()) {
    case 1:
        field_0x41e = 15;
        mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
        mDoGph_gInf_c::startFadeOut(15);
        field_0x41d = 2;
        field_0x420 = 1;
        break;
    }
}

void dScnName_c::FileSelectClose() {
    field_0x41e--;

    if (field_0x41e == 0) {
        field_0x41d = 3;
        field_0x41e = 15;
        field_0x41c = 1;
        mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
        mDoGph_gInf_c::startFadeIn(15);
        field_0x420 = 0;
    }
}

void dScnName_c::brightCheckOpen() {
    if (!mDoRst::isReset()) {
        field_0x41e--;
        if (field_0x41e == 0) {
            field_0x41d = 4;
        }
    }
}

void dScnName_c::brightCheck() {
    mBrightCheck->_move();

    if (mBrightCheck->isEnd()) {
        dComIfGs_setSaveTotalTime(dComIfGs_getTotalTime());
        dComIfGs_setSaveStartTime(OSGetTime());
        mDoAud_bgmStop(0x2D);

        field_0x41f = 0;
        field_0x41d = 5;

        dComIfGs_offItemFirstBit(fpcNm_ITEM_GREEN_RUPEE);
        dComIfGs_offItemFirstBit(fpcNm_ITEM_BLUE_RUPEE);
        dComIfGs_offItemFirstBit(fpcNm_ITEM_YELLOW_RUPEE);
        dComIfGs_offItemFirstBit(fpcNm_ITEM_RED_RUPEE);
        dComIfGs_offItemFirstBit(fpcNm_ITEM_PURPLE_RUPEE);
        dComIfGs_offItemFirstBit(fpcNm_ITEM_ORANGE_RUPEE);
        dComIfGs_offItemFirstBit(fpcNm_ITEM_SILVER_RUPEE);
    }
}

void dScnName_c::changeGameScene() {
    if (!mDoRst::isReset() && !fopOvlpM_IsPeek()) {
        dComIfGs_gameStart();
        fopScnM_ChangeReq(this, field_0x41f == 0 ? PROC_PLAY_SCENE : PROC_PLAY_SCENE, 0, 5);
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
    return 1;
}

static int dScnName_Create(scene_class* i_this) {
    new (i_this) dScnName_c();
    return static_cast<dScnName_c*>(i_this)->create();
}

static scene_method_class l_dScnName_Method = {
    (process_method_func)dScnName_Create, (process_method_func)dScnName_Delete,
    (process_method_func)dScnName_Execute, (process_method_func)dScnName_IsDelete,
    (process_method_func)dScnName_Draw,
};

scene_process_profile_definition g_profile_NAME_SCENE = {
    fpcLy_ROOT_e,
    1,
    fpcPi_CURRENT_e,
    PROC_NAME_SCENE,
    &g_fpcNd_Method.base,
    sizeof(dScnName_c),
    0,
    0,
    &g_fopScn_Method.base,
    &l_dScnName_Method,
    NULL,
};

scene_process_profile_definition g_profile_NAMEEX_SCENE = {
    fpcLy_ROOT_e,
    1,
    fpcPi_CURRENT_e,
    PROC_NAMEEX_SCENE,
    &g_fpcNd_Method.base,
    sizeof(dScnName_c),
    0,
    0,
    &g_fopScn_Method.base,
    &l_dScnName_Method,
    NULL,
};
