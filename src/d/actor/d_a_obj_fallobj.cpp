/**
 * d_a_obj_fallobj.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_fallobj.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_debug_viewer.h"
#include "d/d_procname.h"
#include "d/d_s_play.h"


static char* l_arcName = "K_drock00";

static cull_box l_cull_box = {
    {-200.0f, -10000.0f, -200.0f},
    {200.0f, 600.0f, 200.0f},
};

#if DEBUG

daObjFallObj_HIO_c::daObjFallObj_HIO_c() {
    mMovementShake = 0;
    mStaticShake = 0;
    mNoFall = 0;
    mCheckDisplay = 0;
}

void daObjFallObj_HIO_c::genMessage(JORMContext* ctx) {
    // Falling Object
    ctx->genLabel("落下ＯＢＪ", 0);
    // Movement Shake
    ctx->genSlider("移動時揺れ", &mMovementShake, 0, 1000);
    // Static Shake
    ctx->genSlider("静止時揺れ", &mStaticShake, 0, 1000);
    // No Fall
    ctx->genCheckBox("落下しない", &mNoFall, 0x1);
    // Check Display
    ctx->genCheckBox("チェック表示", &mCheckDisplay, 0x1);
}

static daObjFallObj_HIO_c l_HIO;
#endif

void daObjFallObj_c::initBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjFallObj_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjFallObj_c::Create() {
    u8 uVar1 = field_0x5d8 - 1;
    if (uVar1 != 0) {
        cXyz acStack_30(0.0f, -300.0f, 0.0f);
        u32 params = uVar1 | (getTime() << 8);
        mChildId = fopAcM_createChildFromOffset(
            PROC_Obj_FallObj, fopAcM_GetID(this), params, &acStack_30, fopAcM_GetRoomNo(this), &shape_angle,
            &scale, -1, 0);
    } else {
        mChildId = fpcM_ERROR_PROCESS_ID_e;
    }

    mFallTime = getFallTime();
    initBaseMtx();
    fopAcM_SetMtx(this, mMtx);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);
    fopAcM_SetGravity(this, -6.0f);
    mSoundObj.init(&current.pos, 1);
    return 1;
}

s16 daObjFallObj_c::getFallTime() {
    u8 time = getTime();
    if (time == 0) {
        return 1;
    }

    if (time == 0xff) {
        return 150;
    }

    return time * 15;
}

f32 daObjFallObj_c::getFallHeight() {
    u8 pos = getPos();
    f32 ret;
    if (pos == 0xff) {
        ret = 450.0f;
    } else {
        ret = pos * 50.0f + 300.0f;
    }
    return ret;
}

int daObjFallObj_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(260, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return FALSE;
    }
    return TRUE;
}

int daObjFallObj_c::create1st() {
    field_0x5d8 = 1;
    cPhs__Step phase = (cPhs__Step) dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase =
            (cPhs__Step) MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_Typical, 0x2700, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
              
        #if DEBUG
        // Falling obj
        l_HIO.entryHIO("落下ＯＢＪ");
        #endif
    }
    return phase;
}

// TODO: Debug Nonmatching / Fakematch (bVar1)
int daObjFallObj_c::Execute(Mtx** i_mtx) {
    cXyz cStack_d8(0.0f, 150.0f, 160.0f + KREG_F(0));

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&cStack_d8, &cStack_d8);

    attention_info.position = cStack_d8;
    eyePos = cStack_d8;

    daPy_py_c* player = daPy_getPlayerActorClass();

    f32 dVar12 = std::fabs(player->current.pos.y - cStack_d8.y);
    f32 dVar13 = cStack_d8.absXZ(player->current.pos);
    f32 dVar14 = player->current.pos.abs(cStack_d8);

    dBgS_LinChk linChk;
    linChk.Set(&player->eyePos, &attention_info.position, this);

    bool bVar1 = false;

#if DEBUG
    int check3 = 0;
    int check2 = 0;
    int check1 = 0;
    if (!dComIfG_Bgsp().LineCross(&linChk) && !checkHang2()) {
        check1 = 1;
    }
    if (check1 && dVar12 - KREG_F(4) < dVar13) {
        check2 = 1;
    }
    if (check2 && dVar14 < 2150.0f - KREG_F(5)) {
        check3 = 1;
    }
    if (check3 && mAction == ACTION_WAIT) {
        bVar1 = true;
    }
#else
    if (!dComIfG_Bgsp().LineCross(&linChk) && !checkHang2() && dVar12 - KREG_F(4) < dVar13 && dVar14 < 2150.0f - KREG_F(5) && mAction == ACTION_WAIT)  {
        bVar1 = true;
    }
#endif

    if (((bool) bVar1) != 0) {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0x22;
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    } else {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0x0;
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    }

    field_0x5e0 += 1;
    action();
    *i_mtx = &mBgMtx;
    setBaseMtx();
    mSoundObj.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

void daObjFallObj_c::action() {
    bool isHang = checkHang();

    switch (mAction) {
    case ACTION_WAIT:
        action_wait(isHang);
        break;
    case ACTION_COUNTDOWN:
        action_countdown();
        break;
    case ACTION_FALL:
        action_fall();
        break;
    case ACTION_FALL_START:
        action_fallStart();
        break;
    case ACTION_END:
        action_end();
        break;
    case ACTION_FOLLOW_FALL:
        action_follow_fall();
        break;
    }
    
    if (mVibrationOn && !field_0x5e4) {
        dComIfGp_getVibration().StopQuake(0x1f);
        mVibrationOn = false;
        // Shaking Stop!
        OS_REPORT("揺れ停止！\n");
    }
    field_0x5e5 = field_0x5e4;
    field_0x5e4 = false;
}

bool daObjFallObj_c::action_wait(bool param_1) {
    bool cond = param_1;
    if (cond) {
        mTimer = 10;
        mAction = ACTION_FALL_START;
    }

    if (parentActorID != fpcM_ERROR_PROCESS_ID_e) {
        daObjFallObj_c* parent = (daObjFallObj_c*) fopAcM_SearchByID(parentActorID);
        if (parent != NULL && parent->checkFallStart()) {
            mAction = ACTION_FOLLOW_FALL;
        }
    }
    return cond;
}



bool daObjFallObj_c::action_fallStart() {
    cLib_calcTimer(&mTimer);
    if (mTimer >= 8) {
        speed.y = -25.0f;
        current.pos.y += speed.y;
        if (mTimer == 9) {
            mSoundObj.startSound(Z2SE_OBJ_POLE_FALL_START, 0, -1);
        }
    } else {
        if (mTimer == 7) {
            dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            // Start vibration failed
            OS_REPORT("落下スタート振動！\n");
        } else if (mTimer == 0) {
            dComIfGp_getVibration().StartQuake(4, 0x1e, cXyz(0.0f, 1.0f, 0.0f));
            // Shaking started
            OS_REPORT("揺れ開始！\n");
            mVibrationOn = true;
            mAction = ACTION_COUNTDOWN;
        }
    }

    return false;
}

void daObjFallObj_c::action_countdown() {
    bool noFall = false;
#if DEBUG
    if (l_HIO.mNoFall) {
        noFall = true;
    }
#endif

    daPy_py_c* player = daPy_getPlayerActorClass();
    if (!noFall) {
        speed.y = -(getFallHeight() - 50.0f) / getFallTime();
        current.pos.y += speed.y;
        mSoundObj.startLevelSound(Z2SE_OBJ_POLE_FALLING, 0, -1);
        if (cLib_calcTimer(&mFallTime) == 0) {
            mpBgW->OffPushPullOk();
            speed.y = 0;
            mAction = ACTION_FALL;
        }
    }

    if (player->getSpeedF() != 0.0f) {
#if DEBUG
        field_0x5de = l_HIO.mMovementShake;
    } else {
        field_0x5de = l_HIO.mStaticShake;
#else
        field_0x5de = 0;
    } else {
        field_0x5de = 0;
#endif
    }

    cLib_chaseS(&field_0x5dc, field_0x5de, 0x32);
    shape_angle.x = field_0x5dc * cM_scos(field_0x5e0 * (KREG_S(2) + 1500));
    shape_angle.z = field_0x5dc * cM_ssin(field_0x5e0 * (KREG_S(4) + 2000));
    if (parentActorID != fpcM_ERROR_PROCESS_ID_e) {
        daObjFallObj_c* parent = (daObjFallObj_c*) fopAcM_SearchByID(parentActorID);
        if (parent != NULL && parent->checkFallStart()) {
            mAction = ACTION_FOLLOW_FALL;
        }
    }
}

void daObjFallObj_c::action_fall() {
    speed.y += gravity;
    if (speed.y < fopAcM_GetMaxFallSpeed(this)) {
        speed.y = fopAcM_GetMaxFallSpeed(this);
    }

    current.pos.y += speed.y;
    if (std::fabs(home.pos.y - current.pos.y) > 10000.0f) {
        fopAcM_delete(this);
    }
}

void daObjFallObj_c::action_end() {}

void daObjFallObj_c::action_follow_fall() {
    if (parentActorID != fpcM_ERROR_PROCESS_ID_e) {
        fopAc_ac_c* parent = fopAcM_SearchByID(parentActorID);
        if (parent != NULL) {
            current.pos.y += parent->speed.y;
        }

        if (std::fabs(home.pos.y - current.pos.y) > 10000.0f) {
            fopAcM_delete(this);
        }
    }
}

bool daObjFallObj_c::checkHang() {
    bool rv = false;
    daPy_py_c* player = daPy_getPlayerActorClass();
    
    u32 status = dComIfGp_checkPlayerStatus1(0, 0x2000000);
    if (status && fopAcM_searchPlayerDistanceXZ(this) < 250.0f &&
        player->current.pos.y > current.pos.y - 100.0f &&
        player->current.pos.y < current.pos.y + 300.0f)
    {
        rv = true;
        field_0x5e4 = true;
    }
    return rv;
}

bool daObjFallObj_c::checkHang2() {
    fpc_ProcID procId;
    if (parentActorID == fpcM_ERROR_PROCESS_ID_e) {
        procId = fopAcM_GetID(this);
    } else {
        fpc_ProcID currentProcId = fopAcM_GetID(this);
        procId = parentActorID;
        while (true) {
            fopAc_ac_c* parentActor = fopAcM_SearchByID(procId);
            if (parentActor != NULL) {
                currentProcId = procId;
                procId = parentActor->parentActorID;
            } else {
                procId = currentProcId;
                break;
            }

            if (procId == fpcM_ERROR_PROCESS_ID_e) {
                procId = currentProcId;
                break;
            }
        }
    }

    fpc_ProcID currentProcId = procId;
    while (true) {
        daObjFallObj_c* fallObj = (daObjFallObj_c*)fopAcM_SearchByID(currentProcId);
        if (fallObj == NULL) {
            return false;
        }
        if (fallObj->checkHang()) {
            return true;
        }

        currentProcId = fallObj->mChildId;
        if (currentProcId == fpcM_ERROR_PROCESS_ID_e) {
            return false;
        }
    }
    
    return false;
}

int daObjFallObj_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    #if DEBUG
    if (l_HIO.mCheckDisplay) {
        mpBgW->DebugDraw();
        daPy_py_c* player = daPy_getPlayerActorClass();
        GXColor color = (GXColor){0xff, 0, 0, 0xff};
        cXyz cStack_1c(attention_info.position);
        cStack_1c.y = player->current.pos.y + 10.0f;
        dDbVw_drawCircleXlu(cStack_1c, 2000.0f, color, 1, 0xc);
    }
    #endif

    return 1;
}

int daObjFallObj_c::Delete() {
    mSoundObj.deleteObject();
    dComIfG_resDelete(&mPhaseReq, l_arcName);

    #if DEBUG
    l_HIO.removeHIO();
    #endif
    return 1;
}

static int daObjFallObj_create1st(daObjFallObj_c* i_this) {
    fopAcM_ct(i_this, daObjFallObj_c);
    return i_this->create1st();
}

static int daObjFallObj_MoveBGDelete(daObjFallObj_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjFallObj_MoveBGExecute(daObjFallObj_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjFallObj_MoveBGDraw(daObjFallObj_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjFallObj_METHODS = {
    (process_method_func)daObjFallObj_create1st,
    (process_method_func)daObjFallObj_MoveBGDelete,
    (process_method_func)daObjFallObj_MoveBGExecute,
    NULL,
    (process_method_func)daObjFallObj_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_FallObj = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_FallObj,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjFallObj_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  552,                    // mPriority
  &daObjFallObj_METHODS,  // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
