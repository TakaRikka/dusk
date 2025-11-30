/**
 * f_op_actor.cpp
 * Actor base process class
*/

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_actor.h"
#include "d/d_com_static.h"
#include "d/d_demo.h"
#include "d/d_s_play.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_tag.h"
#include "f_op/f_op_draw_tag.h"
#include "f_pc/f_pc_manager.h"
#include "f_pc/f_pc_debug_sv.h"
#include "c/c_dylink.h"
#include "m_Do/m_Do_printf.h"

#if DEBUG
class print_error_check_c {
public:
    struct param_s {
        const char* name;
        s32 timeMs;
    };

    print_error_check_c(fopAc_ac_c*, const param_s&);
    ~print_error_check_c();
    void start();
    void check();

    static u8 mPriorityMaximum;
    static u8 mPrintDisable;
    static u8 mThresholdEnable;

    static param_s sEXECUTE;
    static param_s sCREATE;
    static param_s sDELETE;
    static param_s sIS_DELETE;
    static param_s sDRAW;

    /* 0x00 */ fopAc_ac_c* mpActor;
    /* 0x04 */ const char* mName;
    /* 0x08 */ int mTimeMs;
    /* 0x0C */ int mPrintErrors;
    /* 0x10 */ u32 mTick;
    /* 0x14 */ int mThreadPrio;
    /* 0x18 */ int mPrintInit;
};

u8 print_error_check_c::mPriorityMaximum;
u8 print_error_check_c::mPrintDisable;
u8 print_error_check_c::mThresholdEnable;

print_error_check_c::print_error_check_c(fopAc_ac_c* i_actor, const param_s& i_param) {
    mpActor = i_actor;
    mName = i_param.name;
    mTimeMs = i_param.timeMs;
    start();
}

print_error_check_c::~print_error_check_c() {
    check();
}

void print_error_check_c::start() {
    if (!cCc_Check()) {
        OS_REPORT("\x1b[36mPRE  %s_SUBMETHOD %08x %08x %s %s\n\x1b[m",
            mName, mpActor, mpActor->base.base.name, fpcDbSv_getNameString(mpActor->base.base.name), fopAcM_getProcNameString(mpActor));
    }

    mPrintErrors = print_errors;

    if (mPrintDisable) {
        mPrintInit = print_initialized;
        OSReportDisable();
    } else {
        mPrintInit = -1;
    }

    if (mPriorityMaximum) {
        OSThread* thread = OSGetCurrentThread();
        mThreadPrio = OSGetThreadPriority(thread);
        OSSetThreadPriority(thread, 0);
    } else {
        mThreadPrio = -1;
    }

    mTick = OSGetTick();
}

void print_error_check_c::check() {
    if (!cCc_Check()) {
        OS_REPORT("\x1b[36mPOST %s_SUBMETHOD %08x %08x %s %s\n\x1b[m",
            mName, mpActor, mpActor->base.base.name, fpcDbSv_getNameString(mpActor->base.base.name), fopAcM_getProcNameString(mpActor));
    }

    OSTick tick = OSGetTick();

    if (mThreadPrio != -1) {
        OSThread* thread = OSGetCurrentThread();
        OSSetThreadPriority(thread, mThreadPrio);
    }

    if (mPrintInit != -1) {
        print_initialized = mPrintInit;
    }

    if (mPrintErrors != print_errors) {
        static char l_name[8];
        fopAcM_getNameString(mpActor, l_name);
        OS_REPORT("\x1b[36m↑%s_SUBMETHOD %08x %08x %s %s\n\x1b[m",
            mName, mpActor, mpActor->base.base.name, fpcDbSv_getNameString(mpActor->base.base.name), l_name);
    }

    if (mThresholdEnable) {
        OSTick tick_diff = tick - mTick;
        if (mTimeMs != 0 && tick_diff > OSMicrosecondsToTicks(mTimeMs)) {
            static char l_name[8];
            fopAcM_getNameString(mpActor, l_name);
            OSReport_Warning("%s_SUBMETHOD 処理時間ながすぎ %4d us [%s] %s\n",
                mName, OSTicksToMicroseconds(tick_diff), fpcDbSv_getNameString(mpActor->base.base.name), l_name);
        }
    }
}

print_error_check_c::param_s print_error_check_c::sEXECUTE = {
    "EXECUTE",
    3000,
};

print_error_check_c::param_s print_error_check_c::sCREATE = {
    "CREATE",
    3000,
};

print_error_check_c::param_s print_error_check_c::sDELETE = {
    "DELETE",
    3000,
};

print_error_check_c::param_s print_error_check_c::sIS_DELETE = {
    "IS_DELETE",
    30,
};

print_error_check_c::param_s print_error_check_c::sDRAW = {
    "DRAW",
    3000,
};

class fopac_HIO_c : public JORReflexible {
public:
    fopac_HIO_c();
    ~fopac_HIO_c();

    void genMessage(JORMContext*);

    /* 0x04 */ s8 mId;
    /* 0x05 */ u8 mStopExecute;
    /* 0x06 */ u8 mStopDraw;
    /* 0x07 */ u8 field_0x7;
    /* 0x08 */ u8 field_0x8;
    /* 0x0A */ s16 mActorNum;
    /* 0x0C */ u8 mBBtnInfoDisp;
};

fopac_HIO_c::~fopac_HIO_c() {}

fopac_HIO_c::fopac_HIO_c() {
    mId = -1;
    mStopExecute = false;
    mStopDraw = false;
    mActorNum = 0;
    mBBtnInfoDisp = false;
}

static fopac_HIO_c l_HIO;

void fopac_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("処理速度調査用", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("アクター番号", &mActorNum, 0, 4000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("動作静止", &mStopExecute, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("描画停止", &mStopDraw, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("Bボタンでアクター番号を表示", &mBBtnInfoDisp, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("メソッド速度調査", &print_error_check_c::mThresholdEnable, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("最高優先度", &print_error_check_c::mPriorityMaximum, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("PRINT抑制", &print_error_check_c::mPrintDisable, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("メソッド閾値", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("EXECUTE", &print_error_check_c::sEXECUTE.timeMs, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("CREATE", &print_error_check_c::sCREATE.timeMs, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("DELETE", &print_error_check_c::sDELETE.timeMs, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("IS_DELETE", &print_error_check_c::sIS_DELETE.timeMs, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("DRAW", &print_error_check_c::sDRAW.timeMs, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

fopAc_ac_c::fopAc_ac_c() {}

fopAc_ac_c::~fopAc_ac_c() {}

int g_fopAc_type;

BOOL fopAc_IsActor(void* i_actor) {
    return fpcM_IsJustType(g_fopAc_type, ((fopAc_ac_c*)i_actor)->actor_type);
}

u32 fopAc_ac_c::stopStatus;

static int fopAc_Draw(void* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    int ret = 1;

    #if DEBUG
    fapGm_HIO_c::startCpuTimer();

    if (mDoCPd_c::getTrigB(PAD_1) && l_HIO.mBBtnInfoDisp) {
        static char l_name[8];
        fopAcM_getNameString(actor, l_name);

        OSReport("%7s %3d %5d x %10.2f y %10.2f z %10.2f\n",
            l_name, fopAcM_GetName(actor), actor->setID, actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    }

    if (l_HIO.mActorNum == fopAcM_GetName(actor) && l_HIO.mStopDraw) {
        if (l_HIO.field_0x8 != l_HIO.mStopDraw) {
            static char l_name[8];
            fopAcM_getNameString(actor, l_name);
            OSReport("<%s> の描画を停止します\n", l_name);
        }

        l_HIO.field_0x8 = l_HIO.mStopDraw;
        return 1;
    }

    l_HIO.field_0x8 = l_HIO.mStopDraw;
    #endif

    if (!dComIfGp_isPauseFlag()) {
        int var_r28 = dComIfGp_event_moveApproval(actor);
        if ((var_r28 == 2 || (!fopAcM_CheckStatus(actor, fopAc_ac_c::stopStatus) &&
            (!fopAcM_CheckStatus(actor, fopAcStts_CULL_e) || !fopAcM_cullingCheck(actor)))) &&
            !fopAcM_CheckStatus(actor, 0x21000000))
        {
            fopAcM_OffCondition(actor, fopAcCnd_NODRAW_e);

            #if DEBUG
            {
            print_error_check_c error_check(actor, print_error_check_c::sDRAW);
            #endif

            ret = fpcLf_DrawMethod((leafdraw_method_class*)actor->sub_method, actor);

            #if DEBUG
            }

            if (g_envHIO.mOther.mDisplayCullBox) {
                GXColor color = {0xFF, 0x00, 0xFF, 0x80};
                fopAcM_DrawCullingBox(actor, color);
            }
            #endif
        } else {
            fopAcM_OnCondition(actor, fopAcCnd_NODRAW_e);
        }

        fopAcM_OffStatus(actor, fopAcStts_NODRAW_e);
    }

    #if DEBUG
    char sp38[40];
    char sp10[8];
    fopAcM_getNameString(actor, sp10);
    sprintf(sp38, "%s（描画処理）", sp10);
    fapGm_HIO_c::stopCpuTimer(sp38);
    #endif

    return ret;
}

static int fopAc_Execute(void* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    int ret = 1;

    #if DEBUG
    fapGm_HIO_c::startCpuTimer();

    if (l_HIO.mActorNum == fopAcM_GetName(actor) && l_HIO.mStopExecute) {
        if (l_HIO.field_0x7 != l_HIO.mStopExecute) {
            static char l_name[8];
            fopAcM_getNameString(actor, l_name);
            OSReport("<%s> の処理を停止します\n", l_name);
        }

        l_HIO.field_0x7 = l_HIO.mStopExecute;
        return 1;
    }

    l_HIO.field_0x7 = l_HIO.mStopExecute;

    JUT_ASSERT(662, !isnan(actor->current.pos.x));
    JUT_ASSERT(663, !isnan(actor->current.pos.y));
    JUT_ASSERT(664, !isnan(actor->current.pos.z));

    if (!(-1.0e32f < actor->current.pos.x && actor->current.pos.x < 1.0e32f && -1.0e32f < actor->current.pos.y && actor->current.pos.y < 1.0e32f && -1.0e32f < actor->current.pos.z && actor->current.pos.z < 1.0e32f))
    {
        static char l_name[8];
        fopAcM_getNameString(actor, l_name);
        OSReport("！！！<%s> が範囲外にいます！！！\n", l_name);
    }

    JUT_ASSERT(685, -1.0e32f < actor->current.pos.x && actor->current.pos.x < 1.0e32f && -1.0e32f < actor->current.pos.y && actor->current.pos.y < 1.0e32f && -1.0e32f < actor->current.pos.z && actor->current.pos.z < 1.0e32f);
    #endif

    if (!dComIfGp_isPauseFlag() && dScnPly_c::isPause()) {
        if (!dComIfA_PauseCheck()) {
            daSus_c::check(actor);
            actor->eventInfo.beforeProc();
            s32 move = dComIfGp_event_moveApproval(i_this);
            fopAcM_OffStatus(actor, 0x40000000);

            if (!fopAcM_CheckStatus(actor, 0x20000000) &&
                (move == 2 ||
                 (move != 0 && !fopAcM_CheckStatus(actor, fopAc_ac_c::stopStatus) &&
                  (!fopAcM_CheckStatus(actor, fopAcStts_NOEXEC_e) || !fopAcM_CheckCondition(actor, fopAcCnd_NODRAW_e)))))
            {
                fopAcM_OffCondition(actor, fopAcCnd_NOEXEC_e);
                actor->old = actor->current;

                #if DEBUG
                {
                print_error_check_c error_check(actor, print_error_check_c::sEXECUTE);
                #endif

                ret = fpcMtd_Execute((process_method_class*)actor->sub_method, actor);

                #if DEBUG
                }
                #endif
            } else {
                actor->eventInfo.suspendProc(actor);
                fopAcM_OnCondition(actor, fopAcCnd_NOEXEC_e);
            }

            if (fopAcM_CheckStatus(actor, 0x20) &&
                actor->home.pos.y - actor->current.pos.y > 5000.0f)
            {
                fopAcM_delete(actor);
            }

            JUT_ASSERT(750, !isnan(actor->current.pos.x));
            JUT_ASSERT(751, !isnan(actor->current.pos.y));
            JUT_ASSERT(752, !isnan(actor->current.pos.z));

            if (actor->current.pos.y < -1e31f) {
                actor->current.pos.y = -1e31f;
            }

            JUT_ASSERT(762, -1.0e32f < actor->current.pos.x && actor->current.pos.x < 1.0e32f && -1.0e32f < actor->current.pos.y && actor->current.pos.y < 1.0e32f && -1.0e32f < actor->current.pos.z && actor->current.pos.z < 1.0e32f);

            dKy_depth_dist_set(actor);
        }
    }

    #if DEBUG
    char sp38[40];
    char sp10[8];
    fopAcM_getNameString(actor, sp10);
    sprintf(sp38, "%s（計算処理）", sp10);
    fapGm_HIO_c::stopCpuTimer(sp38);
    #endif

    return ret;
}

static int fopAc_IsDelete(void* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    int ret;

    #if DEBUG
    {
    print_error_check_c error_check(actor, print_error_check_c::sIS_DELETE);
    #endif

    ret = fpcMtd_IsDelete((process_method_class*)actor->sub_method, actor);
    
    #if DEBUG
    }
    #endif

    if (ret == TRUE) {
        fopDwTg_DrawQTo(&actor->draw_tag);
    }

    return ret;
}

static int fopAc_Delete(void* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    int ret;

    #if DEBUG
    {
    print_error_check_c error_check(actor, print_error_check_c::sDELETE);
    #endif

    ret = fpcMtd_Delete((process_method_class*)actor->sub_method, actor);
    
    #if DEBUG
    }
    #endif

    if (ret == TRUE) {
        fopAcTg_ActorQTo(&actor->actor_tag);
        fopDwTg_DrawQTo(&actor->draw_tag);
        fopAcM_DeleteHeap(actor);

        dDemo_actor_c* demoAc = dDemo_c::getActor(actor->demoActorID);
        if (demoAc != NULL) {
            demoAc->setActor(NULL);
        }

        mDoAud_seDeleteObject(&actor->eyePos);
        mDoAud_seDeleteObject(&actor->current.pos);

        #if DEBUG
        dComIfG_Bgsp().ChkDeleteActorRegist(actor);
        dComIfG_Ccsp()->ChkActor(actor);
        #endif
    }

    return ret;
}

static int fopAc_Create(void* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    int ret;

    if (fpcM_IsFirstCreating(i_this)) {
        actor_process_profile_definition* profile =
            (actor_process_profile_definition*)fpcM_GetProfile(i_this);
        actor->actor_type = fpcBs_MakeOfType(&g_fopAc_type);
        actor->sub_method = (profile_method_class*)profile->sub_method;

        fopAcTg_Init(&actor->actor_tag, actor);
        fopAcTg_ToActorQ(&actor->actor_tag);
        fopDwTg_Init(&actor->draw_tag, actor);

        actor->actor_status = profile->status;
        actor->group = profile->group;
        actor->cullType = profile->cullType;

        fopAcM_prm_class* append = fopAcM_GetAppend(actor);
        if (append != NULL) {
            fopAcM_SetParam(actor, append->base.parameters);
            actor->home.pos = append->base.position;
            actor->home.angle = append->base.angle;
            actor->shape_angle = append->base.angle;
            actor->parentActorID = append->parent_id;
            actor->argument = append->argument;
            actor->scale.set(append->scale.x * 0.1f, append->scale.y * 0.1f,
                              append->scale.z * 0.1f);
            actor->setID = append->base.setID;
            actor->home.roomNo = append->room_no;
        }

        actor->old = actor->home;
        actor->current = actor->home;
        actor->eyePos = actor->home.pos;
        actor->maxFallSpeed = -100.0f;
        actor->attention_info.distances[fopAc_attn_LOCK_e] = 1;
        actor->attention_info.distances[fopAc_attn_TALK_e] = 2;
        actor->attention_info.distances[fopAc_attn_BATTLE_e] = 3;
        actor->attention_info.distances[fopAc_attn_SPEAK_e] = 5;
        actor->attention_info.distances[fopAc_attn_CARRY_e] = 6;
        actor->attention_info.distances[fopAc_attn_ETC_e] = 14;
        actor->attention_info.distances[fopAc_attn_DOOR_e] = 15;
        actor->attention_info.distances[fopAc_attn_JUEL_e] = 15;
        actor->attention_info.distances[fopAc_attn_CHECK_e] = 51;

        #if !PLATFORM_GCN
        actor->attention_info.distances[fopAc_attn_UNK_3] = 79;
        actor->attention_info.distances[fopAc_attn_UNK_4] = 2;
        actor->attention_info.distances[fopAc_attn_UNK_5] = 3;
        actor->attention_info.distances[fopAc_attn_UNK_10] = 2;
        #endif

        actor->attention_info.position = actor->home.pos;
        actor->attention_info.field_0xa = 30;
        dKy_tevstr_init(&actor->tevStr, actor->home.roomNo, -1);

        dStage_FileList_dt_c* sp14 = NULL;
        camera_class* sp10 = dComIfGp_getCamera(0);

        if (dComIfGp_roomControl_getStayNo() >= 0) {
            sp14 = dComIfGp_roomControl_getStatusRoomDt(dComIfGp_roomControl_getStayNo())->getFileListInfo();
        }

        dStage_FileList_dt_c* filelist = NULL;
        if (actor->home.roomNo >= 0) {
            filelist =
                dComIfGp_roomControl_getStatusRoomDt(actor->home.roomNo)->getFileListInfo();
        }

        if (filelist != NULL) {
            if (!dStage_FileList_dt_GetEnemyAppear1Flag(filelist)) {
                u8 sw = dStage_FileList_dt_GetBitSw(filelist);
                if (sw != 0xFF && dComIfGs_isSwitch(sw, actor->home.roomNo) &&
                    profile->group == fopAc_ENEMY_e)
                {
                    OS_WARNING("f_op_actor.cpp マップツール設定により敵グループは削除されました！\n");
                    return cPhs_ERROR_e;
                }
            } else {
                u8 sw = dStage_FileList_dt_GetBitSw(filelist);
                if (sw != 0xFF && !dComIfGs_isSwitch(sw, actor->home.roomNo) &&
                    profile->group == fopAc_ENEMY_e)
                {
                    OS_WARNING("f_op_actor.cpp マップツール設定により敵グループは削除されました！\n");
                    return cPhs_ERROR_e;
                }
            }
        }
    }

    #if DEBUG
    {
    print_error_check_c error_check(actor, print_error_check_c::sCREATE);
    #endif

    ret = fpcMtd_Create((process_method_class*)actor->sub_method, actor);
    
    #if DEBUG
    }

    fopAcM_assert(1113, actor, fopAcM_CheckCondition(actor, fopAcCnd_INIT_e), "fopAcM_ct No Call !!");
    #endif
    
    if (ret == cPhs_COMPLEATE_e) {
        fopDwTg_ToDrawQ(&actor->draw_tag, fpcM_DrawPriority(actor));
    } else if (ret == cPhs_ERROR_e) {
        fopAcM_OnCondition(actor, 0x10);
    }

    #if DEBUG
    if (l_HIO.mId < 0) {
        l_HIO.mId = mDoHIO_CREATE_CHILD("処理速度チェック用", &l_HIO);
    }
    #endif

    return ret;
}

BOOL fopEn_enemy_c::initBallModel() {
    mBallModel = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes("Alink", daAlink_c::getBallModelIdx()), 0x80000, 0x11000284);

    if (!mBallModel) {
        return false;
    } else {
        mBallModel->setBaseScale(cXyz::Zero);

        mBtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Alink", daAlink_c::getBallBtkIdx());
        mBtk->searchUpdateMaterialID(mBallModel->getModelData());

        mBrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Alink", daAlink_c::getBallBrkIdx());
        mBrk->searchUpdateMaterialID(mBallModel->getModelData());
    }

    return true;
}
bool fopEn_enemy_c::checkBallModelDraw() {
    return (mFlags & fopEn_flag_Down) && !(mFlags & (fopEn_flag_WolfDownPullEnd | fopEn_flag_WolfDownPull | fopEn_flag_CutDownHit));
}

void fopEn_enemy_c::setBallModelEffect(dKy_tevstr_c* i_tevstr) {
    if (mBallModel) {
        mAnmFrame += 1.0f;

        if (mAnmFrame >= mBtk->getFrameMax()) {
            mAnmFrame -= mBtk->getFrameMax();
        }

        if (checkBallModelDraw()) {
            Vec* base_scale = mBallModel->getBaseScale();
            cLib_chaseF(&base_scale->x, 1.0f, 0.1f);
            base_scale->y = base_scale->z = base_scale->x;

            mEffectID1 = dComIfGp_particle_set(mEffectID1, dPa_RM(ID_ZI_S_ID_SETA_A), &mDownPos, i_tevstr);
            mEffectID2 = dComIfGp_particle_set(mEffectID2, dPa_RM(ID_ZI_S_ID_SETA_B), &mDownPos, i_tevstr);
        }
    }
}

void fopEn_enemy_c::drawBallModel(dKy_tevstr_c* i_tevstr) {
    f32 scale_target;

    if (mBallModel) {
        Vec* base_scale = mBallModel->getBaseScale();

        if (checkBallModelDraw()) {
            scale_target = 1.0f;
        } else {
            scale_target = 0.0f;
        }

        cLib_chaseF(&base_scale->x, scale_target, 0.05f);

        base_scale->y = base_scale->x;
        base_scale->z = base_scale->x;

        mBallModel->setBaseScale(*base_scale);

        if (base_scale->x > 0.01f) {
            mDoMtx_trans(mBallModel->getBaseTRMtx(), mDownPos.x, mDownPos.y, mDownPos.z);
            g_env_light.setLightTevColorType_MAJI(mBallModel, i_tevstr);

            mBtk->setFrame(mAnmFrame);
            mBallModel->getModelData()->entryTexMtxAnimator(mBtk);

            mBrk->setFrame(mAnmFrame);
            mBallModel->getModelData()->entryTevRegAnimator(mBrk);

            mDoExt_modelUpdateDL(mBallModel);
        }
    }
}

extern actor_method_class g_fopAc_Method = {
    (process_method_func)fopAc_Create,  (process_method_func)fopAc_Delete,
    (process_method_func)fopAc_Execute, (process_method_func)fopAc_IsDelete,
    (process_method_func)fopAc_Draw,
};

class JAUSectionHeap;
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;
