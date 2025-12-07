/**
 * d_s_play.cpp
 * Main Gameplay Scene
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_play.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "SSystem/SComponent/c_counter.h"
#include "c/c_dylink.h"
#include "d/actor/d_a_player.h"
#include "d/d_demo.h"
#include "d/d_eye_hl.h"
#include "d/d_item.h"
#include "d/d_model.h"
#include "d/d_map_path_dmap.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_save_HIO.h"
#include "f_op/f_op_draw_iter.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_overlap_mng.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"
#include "d/actor/d_a_suspend.h"
#include "d/actor/d_a_ykgr.h"

#if PLATFORM_WII
#include "d/d_cursor_mng.h"
#endif

static void dScnPly_Create(scene_class*);
static int dScnPly_Delete(dScnPly_c*);
static int dScnPly_IsDelete(dScnPly_c);
static int dScnPly_Execute(dScnPly_c*);
static int dScnPly_Draw(dScnPly_c*);
static int phase_00(dScnPly_c*);
static int phase_00(dScnPly_c*);
static int phase_1(dScnPly_c*);
static int phase_1_0(dScnPly_c*);
static int phase_01(dScnPly_c*);
static int phase_0(dScnPly_c*);
static int phase_2(dScnPly_c*);
static int phase_3(dScnPly_c*);
static int phase_4(dScnPly_c*);
static int phase_5(dScnPly_c*);
static int phase_6(dScnPly_c*);
static int phase_compleate(void*);

static request_of_phase_process_class resPhase[1];

static request_of_phase_process_class dylPhase[1];

static OSTime dylPreLoadTime1;

static OSTime resPreLoadTime0;

static OSTime resPreLoadTime1;

static dScnPly_preLoad_HIO_c g_preLoadHIO;

/* 80451124 0001+00 data_80451124 None */
s8 dScnPly_c::pauseTimer;

/* 80451125 0003+00 nextPauseTimer__9dScnPly_c None */
s8 dScnPly_c::nextPauseTimer;

s8 dScnPly_c::calcPauseTimer() {
    if (nextPauseTimer != 0) {
        pauseTimer = nextPauseTimer;
        nextPauseTimer = 0;
        return pauseTimer;
    } else {
        return cLib_calcTimer<s8>(&pauseTimer);
    }
}

static const s16 T_JOINT_dylKeyTbl[1] = {
    0x0106,
};

dScnPly_env_otherHIO_c::dScnPly_env_otherHIO_c() {
    mShadowDensity = 255.0f;
    mAdjustLODBias = 1;
    mDisplayTransparentCyl = false;
}

void dScnPly_env_otherHIO_c::genMessage(JORMContext* ctx) {
}

dScnPly_env_debugHIO_c::dScnPly_env_debugHIO_c() {
    mBoxCullMinSize.set(-100.0f, -100.0f, -100.0f);
    mBoxCullMaxSize.set(100.0f, 100.0f, 100.0f);
    mSphereCullCenter.set(0.0f, 0.0f, 0.0f);
    mSphereCullRadius = 100.0f;
}

void dScnPly_env_debugHIO_c::genMessage(JORMContext* ctx) {
}

static int dScnPly_Draw(dScnPly_c* i_this) {
    static s16 l_wipeType[] = {
        0x0000, 0x0000, 0x0011, 0x0002, 0x0002, 0x0001, 0x0003, 0x0001, 0x0004, 0x0004, 0x0005, 0x0005,
        0x0006, 0x0007, 0x0000, 0x0000, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0008, 0x0008,
    };

    dComIfG_Ccsp()->Move();
    dComIfG_Bgsp().ClrMoveFlag();

    u8 useWhiteColor;
    if (!fopOvlpM_IsPeek() && !dComIfG_resetToOpening(i_this)) {
        if (dComIfGp_isEnableNextStage()) {
            u8 wipe = dComIfGp_getNextStageWipe();
            fopScnM_ChangeReq(i_this, PROC_PLAY_SCENE, l_wipeType[wipe], 5);

            int hour = dKy_getdaytime_hour();
            useWhiteColor = hour >= 6 && hour < 18 ? true : false;
            BOOL tmp = useWhiteColor == 0 ? 1 : 0;

            if (wipe == 1 || wipe == 2 || wipe == 7 || wipe == 17 || wipe == 21 ||
                ((wipe == 8 || wipe == 10 || wipe == 18) && tmp) ||
                ((wipe == 9 || wipe == 11 || wipe == 19) && !tmp))
            {
                mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_saftyWhiteColor);
            } else if (wipe == 14 || wipe == 20) {
                GXColor color = {0x2A, 0x1E, 0x46, 0xFF};
                if (dKy_darkworld_check()) {
                    color.r = 0xFF;
                    color.g = 0xCF;
                    color.b = 0xB4;
                }
                mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&color);
            } else {
                mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
            }
        }
    }
    dMdl_mng_c::reset();

    if (!dComIfGp_isPauseFlag() && dScnPly_c::pauseTimer == 0) {
        if (fpcM_GetName(i_this) == PROC_PLAY_SCENE) {
            dComIfGp_getVibration().Run();
        }
        daSus_c::execute();
        dComIfG_Bgsp().Move();
        dComIfGp_particle_calc3D();
        dComIfGp_particle_calc2D();
        cCt_execCounter();
    } else {
        dPa_control_c::onStatus(1);
        if (dScnPly_c::pauseTimer == 0) {
            dPa_control_c::onStatus(2);
        }
        if (dScnPly_c::pauseTimer == 0) {
            dComIfGp_getVibration().Pause();
        }
    }

    for (create_tag_class* i = fopDwIt_Begin(); i != NULL; i = fopDwIt_Next(i)) {
        fpcM_Draw(i->mpTagData);
    }

    if (!dComIfGp_isPauseFlag()) {
        dEyeHL_mng_c::update();
        dComIfG_Ccsp()->Draw();
        dComIfGp_getAttention()->Draw();
    }

    return 1;
}

static int dScnPly_Execute(dScnPly_c* i_this) {
    i_this->offReset();
    dStage_roomControl_c::offNoChangeRoom();
    dStage_roomControl_c::setRoomReadId(0xFF);

    if (!fopOvlpM_IsPeek()) {
        if (mDoAud_zelAudio_c::isBgmSet()) {
            mDoAud_sceneBgmStart();
            mDoAud_load2ndDynamicWave();
            mDoAud_zelAudio_c::offBgmSet();
        }

        if (i_this->calcPauseTimer() != 0) {
            return 1;
        }
    }

    dKy_itudemo_se();
    if (!dComIfGp_isPauseFlag()) {
        dDemo_c::update();
        dComIfGp_getEvent().Step();
        dComIfGp_getAttention()->Run();
    }
    return 1;
}

static int dScnPly_IsDelete(dScnPly_c i_this) {
    dComIfGp_particle_cleanup();
    return 1;
}

struct PreLoadInfo {
    const char* stageName;
    const s16* profNameTbl;
    const char** resNameTbl;
    u8 dylKeyTblNum;
    u8 resNameNum;
};

static const char* T_JOINT_resName[1] = {"Always"};

static PreLoadInfo const PreLoadInfoT[1] = {
    {
        "T_JOINT",
        T_JOINT_dylKeyTbl,
        T_JOINT_resName,
        1,
        1,
    },
};

static s8 preLoadNo = -1;

static u8 doPreLoad = 1;

static int dScnPly_Delete(dScnPly_c* i_this) {
    daSus_c::reset();
    dMpath_c::remove();
    dTres_c::remove();

    dComIfGp_getAttention()->~dAttention_c();
    dComIfGp_getVibration().Remove();

    dComIfG_Bgsp().Dt();
    dComIfG_Ccsp()->Dt();
    dStage_Delete();

    dComIfGp_event_remove();

    dComIfGp_particle_removeScene(0);

    dComIfGp_getMsgDtArchive(1)->removeResourceAll();
    JKRUnmountArchive(dComIfGp_getMsgDtArchive(1));

    dDemo_c::remove();

    fopMsgM_destroyExpHeap(dComIfGp_getExpHeap2D());
    dComIfGp_setExpHeap2D(NULL);
    fopMsgM_destroyExpHeap(dComIfGp_getMsgExpHeap());
    dComIfGp_setMsgExpHeap(NULL);

    dComIfGp_deleteSimpleModel();
    dMdl_mng_c::remove();

    mDoGph_gInf_c::getBloom()->remove();

    dComIfGs_offPlayerFieldLastStayFieldDataExistFlag();
    dComIfGp_setWindowNum(0);
    dComIfGd_setView(NULL);

    if (preLoadNo >= 0) {
        const char** resname_table = PreLoadInfoT[preLoadNo].resNameTbl;
        int res_num = PreLoadInfoT[preLoadNo].resNameNum;
        if (resname_table != NULL && (*resname_table != NULL)) {
            for (int i = 0; i < res_num; i++) {
                dComIfG_resDelete(&resPhase[i], resname_table[i]);
            }
        }

        const s16* profname_table = PreLoadInfoT[preLoadNo].profNameTbl;
        int prof_num = PreLoadInfoT[preLoadNo].dylKeyTblNum;
        if (profname_table != NULL && *profname_table != 0) {
            for (int i = 0; i < prof_num; i++) {
                cDylPhs::Unlink(&dylPhase[i], profname_table[i]);
            }
        }
    }

    dComIfGp_init();

    #if PLATFORM_WII
    data_8053a730 = 0;
    #endif

    JUTAssertion::setMessageCount(0);
    return 1;
}

bool dScnPly_c::resetGame() {
    if (fpcM_GetName(this) == PROC_OPENING_SCENE) {
        if (!dStage_roomControl_c::resetArchiveBank(0)) {
            return false;
        }

        if (!mDoAud_resetRecover()) {
            return false;
        }

        if (mDoRst::isReset()) {
            field_0x1d4 = 1;
        }
    } else {
        if (dComIfGp_getNextStagePoint() == -4 || (dComIfGs_getLastSceneMode() & 0xF) == 0xC ||
            !strcmp(dComIfGp_getNextStageName(), "F_SP109") ||
            (!strcmp(dComIfGp_getNextStageName(), "F_SP116") &&
             dComIfGp_getNextStageRoomNo() == 1 && dComIfGp_getNextStageLayer() == 9))
        {
            if (!dStage_roomControl_c::resetArchiveBank(0)) {
                return false;
            }
        }
    }

    dComIfG_setBrightness(255);
    mDoGph_gInf_c::offFade();
    dDlst_list_c::offWipe();
    return true;
}

void dScnPly_c::offReset() {
    if (field_0x1d4 != 0 && !fopOvlpM_IsPeek()) {
        mDoRst::offReset();
        mDoRst::offResetPrepare();
        JUTGamePad::C3ButtonReset::sResetOccurred = false;
        JUTGamePad::setResetCallback(mDoRst_resetCallBack, NULL);
        field_0x1d4 = 0;
    }
}

static int phase_00(dScnPly_c* i_this) {
    if (!i_this->resetGame()) {
        return cPhs_INIT_e;
    } else {
        #if PLATFORM_WII
        data_8053a730 = 1;
        #endif

        mDoGph_gInf_c::offBlure();
        return cPhs_NEXT_e;
    }
}

static int phase_01(dScnPly_c* i_this) {
    mDoAud_setHour(dKy_getdaytime_hour());
    mDoAud_setMinute(dKy_getdaytime_minute());
    mDoAud_setWeekday(dKy_get_dayofweek());

    if (dKy_darkworld_check()) {
        mDoAud_setInDarkness(true);
    } else {
        mDoAud_setInDarkness(false);
    }

    mDoAud_setSceneName(dComIfGp_getStartStageName(),
                        dComIfGp_getStartStageRoomNo(),
                        dComIfG_play_c::getLayerNo_common(dComIfGp_getStartStageName(), dComIfGp_getStartStageRoomNo(),
                                                                 dComIfGp_getStartStageLayer()));

    if (!mDoAud_load1stDynamicWave()) {
        return cPhs_INIT_e;
    } else {
        return cPhs_NEXT_e;
    }
}

static int phase_0(dScnPly_c* i_this) {
    return cPhs_NEXT_e;
}

static int phase_1(dScnPly_c* i_this) {
    u32 id = fopScnM_GetID(i_this);
    dStage_roomControl_c::setProcID(id);

    dComIfGp_setStartStage(dComIfGp_getNextStartStage());

    if (dComIfGp_getStartStageLayer() < 0 && daPy_py_c::checkRoomRestartStart()) {
        const char* stage = dComIfGp_getStartStageName();
        s8 room = dComIfGp_getStartStageRoomNo();
        s8 layer = dComIfGp_getStartStageLayer();
        if (dComIfG_play_c::getLayerNo_common_common(stage, room, layer) < 0) {
            dComIfGp_setStartStageLayer(dComIfGp_getLayerOld());
        }
    }
    dComIfGp_offEnableNextStage();

    // Stage: Faron Woods, Room: Faron Spring
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP108") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStagePoint() == 3)
    {
        dComIfGs_onDarkClearLV(0);
        execItemGet(fpcNm_ITEM_WEAR_KOKIRI);
    }
    // Stage: Kakariko Village, Room: Kakariko Village
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP109") &&
             dComIfGp_getStartStageRoomNo() == 0 && dComIfGp_getStartStagePoint() == 30)
    {
        dComIfGs_onDarkClearLV(1);
    }
    // Stage: Lake Hylia, Room: Fountain
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP115") &&
             dComIfGp_getStartStageRoomNo() == 1 && dComIfGp_getStartStagePoint() == 20)
    {
        dComIfGs_onDarkClearLV(2);
    }
    // Stage: Sacred Grove, Room: Lost Woods
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP117") &&
             dComIfGp_getStartStageRoomNo() == 1 && dComIfGp_getStartStagePoint() == 99)
    {
        dComIfGs_onDarkClearLV(3);
    }

    // Stage: Ordon Spring, Room: Ordon Spring
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP104") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStagePoint() == 23 && dComIfGp_getStartStageLayer() == 12)
    {
        dComIfGs_onItemFirstBit(fpcNm_ITEM_HORSE_FLUTE);
        dComIfGs_setItem(SLOT_21, fpcNm_ITEM_HORSE_FLUTE);
    }

    if ((u8)dKy_darkworld_stage_check(dComIfGp_getStartStageName(),
                                      dComIfGp_getStartStageRoomNo()) == true)
    {
        dComIfGp_world_dark_set(1);
    } else if ((u8)dKy_darkworld_spot_check(dComIfGp_getStartStageName(),
                                            dComIfGp_getStartStageRoomNo()) == true)
    {
        dComIfGp_world_dark_set(2);
    } else {
        dComIfGp_world_dark_set(0);
    }

    // Stage: Lake Hylia, Room: Fountain
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP115") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStageLayer() < 0)
    {
        const char* stage = dComIfGp_getStartStageName();
        s8 room = dComIfGp_getStartStageRoomNo();
        s8 layer = dComIfGp_getStartStageLayer();
        if (dComIfG_play_c::getLayerNo_common(stage, room, layer) == 9) {
            dComIfGp_setStartStageLayer(9);
        }
    }

    // Stage: Faron Woods, Room: South Faron
    // Stage: Faron Woods Cave
    // Stage: Kakariko Village, Room: Kakariko Village
    // Stage: Lake Hylia, Room: Fountain
    if ((!strcmp(dComIfGp_getStartStageName(), "F_SP108") && dComIfGp_getStartStageRoomNo() == 0 &&
         dComIfGp_getStartStageLayer() == 7) ||
        (!strcmp(dComIfGp_getStartStageName(), "D_SB10") && dComIfG_play_c::getLayerNo(0) == 14) ||
        (!strcmp(dComIfGp_getStartStageName(), "F_SP109") && dComIfGp_getStartStageRoomNo() == 0 &&
         dComIfGp_getStartStageLayer() == 8) ||
        (!strcmp(dComIfGp_getStartStageName(), "F_SP115") && dComIfGp_getStartStageRoomNo() == 1 &&
         dComIfGp_getStartStageLayer() == 8) ||
        (!strcmp(dComIfGp_getStartStageName(), "F_SP115") && dComIfGp_getStartStageRoomNo() == 1 &&
         dComIfGp_getStartStageLayer() == 9))
    {
        dComIfGp_world_dark_set(1);
    }

    // Stage: Zant Boss Fight, Room: Throne Room
    if (!strcmp(dComIfGp_getStartStageName(), "D_MN08D") && dComIfGp_getStartStageRoomNo() == 50 &&
        dComIfGp_getStartStagePoint() == 20)
    {
        dComIfGs_onSaveDunSwitch(30);
    }

    dKy_darkworld_Area_set(dComIfGp_getStartStageName(), dComIfGp_getStartStageRoomNo());

    // Stage: Hyrule Castle Sewers, Room: Prison Cell
    if (!strcmp(dComIfGp_getStartStageName(), "R_SP107") && dComIfGp_getStartStageRoomNo() == 0 &&
        (dComIfGp_getStartStagePoint() == 24 || dComIfGp_getStartStagePoint() == 0))
    {
        dComIfGs_onTransformLV(0);
    }
    // Stage: Hyrule Field, Room: Eldin Gorge south entrance
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") &&
             dComIfGp_getStartStageRoomNo() == 2 && dComIfGp_getStartStagePoint() == 10)
    {
        dComIfGs_onTransformLV(1);
    }
    // Stage: Hyrule Field, Room: Lanayru Field north entrance
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") &&
             dComIfGp_getStartStageRoomNo() == 9 && dComIfGp_getStartStagePoint() == 10)
    {
        dComIfGs_onTransformLV(2);
    }
    // Stage: Hyrule Field, Room: Lanayru Field
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") &&
             dComIfGp_getStartStageRoomNo() == 10 &&
             (dComIfGp_getStartStagePoint() == 23 || dComIfGp_getStartStagePoint() == 20))
    {
        dComIfGs_onTransformLV(3);
    }

    // Stage: Fishing Pond, Room: Fishing Pond
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP127") && dComIfGp_getStartStageRoomNo() == 0 &&
        dComIfGp_getStartStagePoint() == 2)
    {
        g_env_light.fishing_hole_season++;

        if (g_env_light.fishing_hole_season > 4) {
            g_env_light.fishing_hole_season = 1;
        }
    }

    dComIfGs_BossLife_public_Set(0xFF);
    g_env_light.light_mask_type = 0;

    JUTReportConsole_f("Start StageName:RoomNo [%s:%d]\n", dComIfGp_getStartStageName(),
                       dComIfGp_getStartStageRoomNo());
    dComIfGp_setStatus(0);
    if (dComIfG_syncStageRes("Stg_00") < 0) {
        dComIfG_setStageRes("Stg_00", NULL);
    }
    return cPhs_NEXT_e;
}

static int phase_1_0(dScnPly_c* i_this) {
    static char camparamarc[10] = "CamParam";

    if (dComIfG_syncStageRes("Stg_00")) {
        return cPhs_INIT_e;
    } else {
        dStage_infoCreate();
        dComIfG_setObjectRes("Event", (u8)0, NULL);
        dComIfGp_setCameraParamFileName(0, camparamarc);
        dComIfG_setObjectRes("CamParam", (u8)0, NULL);
        return cPhs_NEXT_e;
    }
}

static int phase_2(dScnPly_c* i_this) {
    int tmp = dComIfG_syncAllObjectRes();
    if (tmp >= 0 && tmp != 0) {
        return cPhs_INIT_e;
    }

    u8 particle_no = dStage_stagInfo_GetParticleNo(dComIfGp_getStage()->getStagInfo(),
                                                   dComIfG_play_c::getLayerNo(0));
    if (particle_no == 255) {
        particle_no = dStage_stagInfo_GetParticleNo(dComIfGp_getStage()->getStagInfo());
    }

    dComIfGp_particle_readScene(particle_no, &i_this->sceneCommand);
    dMsgObject_readMessageGroup(&i_this->field_0x1d0);
    return cPhs_NEXT_e;
}

static int phase_3(dScnPly_c* i_this) {
    if ((i_this->sceneCommand != NULL && !i_this->sceneCommand->sync()) || mDoAud_check1stDynamicWave()) {
        return cPhs_INIT_e;
    }

    if (!i_this->field_0x1d0 == NULL && !i_this->field_0x1d0->sync()) {
        return cPhs_INIT_e;
    }

    return cPhs_NEXT_e;
}

dScnPly_reg_HIO_c g_regHIO;

dScnPly_env_HIO_c g_envHIO;

#if DEBUG
dScnPly_preset_HIO_c g_presetHIO;
#endif

static int phase_4(dScnPly_c* i_this) {
    if (i_this->sceneCommand) {
        dComIfGp_particle_createScene(i_this->sceneCommand->getMemAddress());
        i_this->sceneCommand->destroy();
    } else {
        dComIfGp_particle_createScene(NULL);
    }

    if (i_this->field_0x1d0 != NULL) {
        dComIfGp_setMsgDtArchive(1, i_this->field_0x1d0->getArchive());
        i_this->field_0x1d0->destroy();
    }

    dComIfGp_calcNowRegion();
    dComIfG_Bgsp().Ct();
    fopAcM_lc_c::getLineCheck()->ClearPi();
    fopAcM_gc_c::getGroundCheck()->ClearPi();
    fopAcM_rc_c::getRoofCheck()->ClearPi();
    fopAcM_wt_c::getWaterCheck()->ClearPi();
    dComIfG_Ccsp()->Ct();
    dDemo_c::create();

    dComIfGp_setPlayerInfo(0, NULL, 0);
    for (int i = 0; i < 2; i++) {
        dComIfGp_setPlayerPtr(i, NULL);
    }

    dComIfGp_setWindow(0, 0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0.0f, 1.0f, 0, 2);
    dComIfGp_setCameraInfo(0, NULL, 0, 0, -1);
    dComIfGd_setWindow(NULL);
    dComIfGd_setViewport(NULL);
    dComIfGd_setView(NULL);

    dComIfGp_setExpHeap2D(fopMsgM_createExpHeap(0xBB800, NULL));
    dComIfGp_setMsgExpHeap(fopMsgM_createExpHeap(0xA800, NULL));

    if (fpcM_GetName(i_this) == PROC_OPENING_SCENE) {
        fopAcM_create(PROC_TITLE, 0, NULL, -1, NULL, NULL, -1);
        dComIfGs_init();
        dComIfGs_setOptPointer(0);
        dComIfGs_setLife(12);
        dMeter2Info_setCloth(fpcNm_ITEM_WEAR_KOKIRI, false);
        dMeter2Info_setSword(fpcNm_ITEM_SWORD, false);
        dMeter2Info_setShield(fpcNm_ITEM_HYLIA_SHIELD, false);
        dComIfGs_onEventBit(0x0601);  // Epona Tamed
    }

    dMpath_c::create();
    dTres_c::create();
    dStage_Create();
    dComIfGp_createSimpleModel();
    dMdl_mng_c::create();

    mDoGph_gInf_c::setTickRate((OS_BUS_CLOCK / 4) / 30);
    g_envHIO.field_0x4 = -1;
    g_save_bit_HIO.field_0x4 = -1;
    new (dComIfGp_getAttention()) dAttention_c(dComIfGp_getPlayer(0), 0);
    dComIfGp_getVibration().Init();
    daYkgr_c::init();

    dComIfG_setBrightness(255);
    mDoGph_gInf_c::offFade();
    mDoAud_zelAudio_c::onBgmSet();
    dScnPly_c::pauseTimer = 0;
    dScnPly_c::nextPauseTimer = 0;
    preLoadNo = -1;

    if (doPreLoad != 0) {
        const char* stageName = dComIfGp_getStartStageName();
        OS_REPORT("stageName=%s\n", stageName);
        for (u32 i = 0; i < 1; i++) {
            if (strcmp(stageName, PreLoadInfoT[i].stageName) == 0) {
                preLoadNo = i;
            }
        }
    }

    if (preLoadNo < 0) {
        return cPhs_COMPLEATE_e;
    }

    resPreLoadTime0 = OSGetTime();
    return cPhs_NEXT_e;
}

static int phase_5(dScnPly_c* i_this) {
    if (preLoadNo >= 0) {
        int phase_state = cPhs_NEXT_e;

        const char** resNames = PreLoadInfoT[preLoadNo].resNameTbl;
        s32 resNameNum = PreLoadInfoT[preLoadNo].resNameNum;
        if (resNames != NULL && *resNames != NULL) {
            JUT_ASSERT(2830, resNameNum <= ARRAY_SIZEU(resPhase));
            int goodLoads = 0;
            int loadNum = 0;
            for (int i = 0; i < resNameNum; i++) {
                if (dComIfG_resLoad(&resPhase[i], resNames[i]) == 4) {
                    goodLoads++;
                } else {
                    phase_state = cPhs_INIT_e;
                }
                loadNum++;
            }
        }
    
        if (phase_state == cPhs_COMPLEATE_e) {
            resPreLoadTime1 = OSGetTime();
        }
    
        return phase_state;
    }

    return cPhs_NEXT_e;
}

static int phase_6(dScnPly_c* i_this) {
    if (preLoadNo >= 0) {
        int phase_state = cPhs_NEXT_e;

        const s16* dylKeyTbl = PreLoadInfoT[preLoadNo].profNameTbl;
        s32 dylKeyTblNum = PreLoadInfoT[preLoadNo].dylKeyTblNum;
        if (dylKeyTbl != NULL && *dylKeyTbl != 0) {
            JUT_ASSERT(2864, dylKeyTblNum <= ARRAY_SIZEU(dylPhase));
            int goodLoads = 0;
            int loadNum = 0;
            for (int i = 0; i < dylKeyTblNum; i++) {
                if (cDylPhs::Link(&dylPhase[i], dylKeyTbl[i]) == cPhs_COMPLEATE_e) {
                    goodLoads++;
                } else {
                    phase_state = cPhs_INIT_e;
                }
                loadNum++;
            }
        }

        if (phase_state == cPhs_COMPLEATE_e) {
            dylPreLoadTime1 = OSGetTime();
        }

        return phase_state;
    }

    return cPhs_NEXT_e;
}

static int phase_compleate(void* i_this) {
    return cPhs_COMPLEATE_e;
}

static void dScnPly_Create(scene_class* i_this) {
    static request_of_phase_process_fn l_method[] = {
        (request_of_phase_process_fn)phase_00,        (request_of_phase_process_fn)phase_1,
        (request_of_phase_process_fn)phase_1_0,       (request_of_phase_process_fn)phase_01,
        (request_of_phase_process_fn)phase_0,         (request_of_phase_process_fn)phase_2,
        (request_of_phase_process_fn)phase_3,         (request_of_phase_process_fn)phase_4,
        (request_of_phase_process_fn)phase_5,         (request_of_phase_process_fn)phase_6,
        (request_of_phase_process_fn)phase_compleate,
    };

    dComLbG_PhaseHandler(&static_cast<dScnPly_c*>(i_this)->field_0x1c4, l_method, i_this);
}

static leafdraw_method_class l_dScnPly_Method = {
    (process_method_func)dScnPly_Create,  (process_method_func)dScnPly_Delete,
    (process_method_func)dScnPly_Execute, (process_method_func)dScnPly_IsDelete,
    (process_method_func)dScnPly_Draw,
};

scene_process_profile_definition g_profile_PLAY_SCENE = {
    fpcLy_ROOT_e,                              // mLayerID
    1,                                         // mListID
    fpcPi_CURRENT_e,                           // mListPrio
    PROC_PLAY_SCENE,                           // mProcName
    &g_fpcNd_Method.base,                     // sub_method
    sizeof(dScnPly_c),                         // mSize
    0,                                         // mSizeOther
    0,                                         // mParameters
    &g_fopScn_Method.base,                    // sub_method
    (process_method_class*)&l_dScnPly_Method,  // mpMtd
};

scene_process_profile_definition g_profile_OPENING_SCENE = {
    fpcLy_ROOT_e,                              // mLayerID
    1,                                         // mListID
    fpcPi_CURRENT_e,                           // mListPrio
    PROC_OPENING_SCENE,                        // mProcName
    &g_fpcNd_Method.base,                     // sub_method
    sizeof(dScnPly_c),                         // mSize
    0,                                         // mSizeOther
    0,                                         // mParameters
    &g_fopScn_Method.base,                    // sub_method
    (process_method_class*)&l_dScnPly_Method,  // mpMtd
};
