/**
 * @file d_a_obj_togeTrap.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_togeTrap.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

daTogeTrap_HIO_c::daTogeTrap_HIO_c() {
    mInitialSpeed = 0.0f;
    mMaxSpeed = 250.0f;
    mInitialDownSpeed = 0.0f;
    mMaxDownSpeed = 10.0f;
    mSpinnerSpeed = 20.0f;
    mVibration = 3;
    mRange = 500.0f;
}

static daTogeTrap_HIO_c l_HIO;

const dCcD_SrcGObjInf daTogeTrap_c::mCcDObjInfo = {
    {0, {{2, 1, 0x1D}, {0x480028, 0x11}, {0}}},
    {dCcD_SE_SWORD, 0, 0, 0, 0},
    {dCcD_SE_STONE, 0, 0, 0, 0},
    {0},
};

dCcD_SrcCyl daTogeTrap_c::mCcDCyl = {
    daTogeTrap_c::mCcDObjInfo,
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        0.0f,                // mRadius
        0.0f                 // mHeight
    }  // mCyl
};

void daTogeTrap_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mModel->setBaseScale(scale);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(home.pos.x, home.pos.y, home.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mWolfEyeModel->setBaseScale(scale);
    mWolfEyeModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daTogeTrap_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("P_Toge", 4);
    JUT_ASSERT(251, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return 0;
    }
    modelData = (J3DModelData*)dComIfG_getObjectRes("P_Toge", 5);
    JUT_ASSERT(262, modelData != NULL);
    mWolfEyeModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mWolfEyeModel == NULL) {
        return 0;
    }

    return 1;
}

int daTogeTrap_c::create() {
    fopAcM_ct(this, daTogeTrap_c);
    mSwBit = getSwBit();
    if (mSwBit != 0xff &&
        fopAcM_isSwitch(this, mSwBit))
    {
        return cPhs_ERROR_e;
    }
    int phase = dComIfG_resLoad(&mPhase, "P_Toge");
    if (phase == 4) {
        if (MoveBGCreate("P_Toge", 8, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mModel->getModelData());
        mpBgW->SetRideCallback(rideCallBack);
        mStts.Init(0xff, 0xff, this);
        for (int i = 0; i < 4; i++) {
            mCyls[i].Set(mCcDCyl);
            mCyls[i].SetStts(&mStts);
            mCyls[i].OnTgSetBit();
        }
        current.pos.y -= 300.0f;
        mIsPlayerInArea = playerAreaCheck();
        mIsWolfEye = 0;
        init_modeWait();
        setBaseMtx();

        #if DEBUG
        // Spike traps
        l_HIO.entryHIO("トゲトラップ");
        #endif
    }
    return phase;
}

void daTogeTrap_c::rideCallBack(dBgW* param_1, fopAc_ac_c* param_2, fopAc_ac_c* param_3) {
    if (fopAcM_GetName(param_3) == PROC_Obj_Movebox) {
        ((daTogeTrap_c*)param_2)->field_0xadd = 1;
    }
}

int daTogeTrap_c::Execute(Mtx** i_mtx) {
    moveMain();
    *i_mtx = &mModel->getBaseTRMtx();
    setBaseMtx();
    if (!dComIfGs_wolfeye_effect_check()) {
        mIsWolfEye = false;
    }
    field_0xadd = 0;
    return 1;
}

void daTogeTrap_c::moveMain() {
    static daTogeTrap_c::modeFunc mode_proc[4] = {
        &daTogeTrap_c::modeWait,
        &daTogeTrap_c::modeMoveUp,
        &daTogeTrap_c::modeMoveDown,
        &daTogeTrap_c::modeBreak,
    };

    int areaCheck = playerAreaCheck();
    if (areaCheck != mIsPlayerInArea) {
        if (areaCheck == 1) {
            init_modeMoveUp();
        } else {
            init_modeMoveDown();
        }
        mIsPlayerInArea = areaCheck;
    }
    for (int i = 0; i < 4; i++) {
        if (mCyls[i].ChkTgHit()) {
            cCcD_ObjHitInf* hitObj = mCyls[i].GetTgHitGObj();
            if (hitObj->GetAtType() & (AT_TYPE_IRON_BALL | AT_TYPE_BOMB)) {
                init_modeBreak();
                break;
            }
            daPy_py_c* player = (daPy_py_c*)dComIfGp_getLinkPlayer();
            if (player->checkSpinnerRide() && (player->getSpinnerRideSpeed() >= l_HIO.mSpinnerSpeed ||
                player->checkSpinnerReflectEffect()))
            {
                init_modeBreak();
                break;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        mCyls[i].SetR(60.0f);
        mCyls[i].SetH(290.0f);
        cXyz local_30(0.0f, 0.0f, 0.0f);
        local_30.x = (i * 100.0f) - 150.0f;
        mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
        mDoMtx_stack_c::multVec(&local_30, &local_30);
        cXyz cStack_3c = current.pos;
        cStack_3c += local_30;
        mCyls[i].SetC(cStack_3c);
        dComIfG_Ccsp()->Set(&mCyls[i]);
    }
    if (field_0xadd != 0) {
        init_modeBreak();
    }
    (this->*mode_proc[mMode])();
}

BOOL daTogeTrap_c::playerAreaCheck() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz local_48 = current.pos - player->current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&local_48, &local_48);
    f32 absZ = fabsf(local_48.z);
    f32 absX = fabsf(local_48.x);
    BOOL rv = FALSE;
    if (absZ <= l_HIO.mRange && absX <= 225.0f) {
        rv = TRUE;
    }
    return rv;
}

void daTogeTrap_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daTogeTrap_c::modeWait() {}

void daTogeTrap_c::init_modeMoveUp() {
    fopAcM_SetSpeedF(this, l_HIO.mInitialSpeed);
    mDoAud_seStart(Z2SE_OBJ_SWD_FENCE_POP, &home.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    for (int i = 0; i < 4; i++) {
        mCyls[i].OnAtSetBit();
        mCyls[i].OnTgSetBit();
    }
    this->mMode = MODE_MOVE_UP;
}

void daTogeTrap_c::modeMoveUp() {
    cLib_chaseF(&speedF, l_HIO.mMaxSpeed, l_HIO.mMaxSpeed / 30.0f);
    if (cLib_addCalc(&current.pos.y, home.pos.y, 1.0f, fopAcM_GetSpeedF(this), 1.0f) == 0.0f) {
        dComIfGp_getVibration().StartShock(l_HIO.mVibration, 0xf, cXyz(0.0f, 1.0f, 0.0f));
        dComIfGp_particle_set(0x8ad9, &current.pos, &shape_angle, 0);
        for (int i = 0; i < 4; i++) {
            mCyls[i].OffAtSetBit();
        }
        if (dComIfGs_wolfeye_effect_check()) {
            mIsWolfEye = true;
        }
        init_modeWait();
    }
}

void daTogeTrap_c::init_modeMoveDown() {
    fopAcM_SetSpeedF(this, l_HIO.mInitialDownSpeed);
    this->mMode = MODE_MOVE_DOWN;
}

void daTogeTrap_c::modeMoveDown() {
    cLib_chaseF(&speedF, l_HIO.mMaxDownSpeed, l_HIO.mMaxDownSpeed / 30.0f);
    if (cLib_addCalc(&current.pos.y, home.pos.y - 300.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f) == 0.0f) {
        for (int i = 0; i < 4; i++) {
            mCyls[i].OffTgSetBit();
            mCyls[i].OffAtSetBit();
        }
        init_modeWait();
    }
}

void daTogeTrap_c::init_modeBreak() {
    daPy_py_c* player = dComIfGp_getLinkPlayer();
    if (player->getSpinnerRideSpeed() >= l_HIO.mSpinnerSpeed && mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    mDoAud_seStart(Z2SE_OBJ_TRAP_BREAK_STN, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    this->mMode = MODE_BREAK;
}

void daTogeTrap_c::modeBreak() {
    dComIfGp_particle_set(0x8ada, &current.pos, &shape_angle, 0);
    dComIfGp_particle_set(0x8adb, &current.pos, &shape_angle, 0);
    dComIfGp_particle_set(0x8adc, &current.pos, &shape_angle, 0);
    dComIfGp_particle_set(0x8add, &current.pos, &shape_angle, 0);
    if (mSwBit != 0xff) {
        fopAcM_onSwitch(this, mSwBit);
    }
    fopAcM_delete(this);
}

int daTogeTrap_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    if (mIsWolfEye) {
        mDoExt_modelUpdateDL(mWolfEyeModel);
    }
    dComIfGd_setList();
    return 1;
}

int daTogeTrap_c::Delete() {
    dComIfG_resDelete(&mPhase, "P_Toge");

    #if DEBUG
    l_HIO.removeHIO(this);
    #endif

    return 1;
}

static int daTogeTrap_Draw(daTogeTrap_c* i_this) {
    return i_this->Draw();
}

static int daTogeTrap_Execute(daTogeTrap_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daTogeTrap_Delete(daTogeTrap_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daTogeTrap_Create(fopAc_ac_c* i_this) {
    return ((daTogeTrap_c*)i_this)->create();
}

static actor_method_class l_daTogeTrap_Method = {
    (process_method_func)daTogeTrap_Create,
    (process_method_func)daTogeTrap_Delete,
    (process_method_func)daTogeTrap_Execute,
    0,
    (process_method_func)daTogeTrap_Draw,
};

extern actor_process_profile_definition g_profile_Obj_TogeTrap = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_TogeTrap,      // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daTogeTrap_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  524,                    // mPriority
  &l_daTogeTrap_Method,   // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
