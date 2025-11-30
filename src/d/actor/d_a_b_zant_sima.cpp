/**
 * d_a_b_zant_sima.cpp
 * Boss Zant Lava Island
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_zant_sima.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_obj_msima.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

#define TIMER_PLAYER 0
#define TIMER_ZANT 1

int daB_ZANTS_c::draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mpBtkAnm->entry(mpModel->getModelData());
    mpBrkAnm->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

static int daB_ZANTS_Draw(daB_ZANTS_c* i_this) {
    return i_this->draw();
}

static void zants_ride_call_back(dBgW* i_bgw, fopAc_ac_c* i_this, fopAc_ac_c* i_target) {
    if (fopAcM_GetName(i_target) == PROC_ALINK) {
        ((daB_ZANTS_c*)i_this)->mPlayerID = fopAcM_GetID(i_target);
        ((daB_ZANTS_c*)i_this)->mSinkTimers[TIMER_PLAYER] = 3;
    } else if (fopAcM_GetName(i_target) == PROC_B_ZANT) {
        ((daB_ZANTS_c*)i_this)->mBossID = fopAcM_GetID(i_target);
        ((daB_ZANTS_c*)i_this)->mSinkTimers[TIMER_ZANT] = 3;
    }
}

void daB_ZANTS_c::action() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (player->checkMagneBootsOn()) {
        mSinkTimers[TIMER_PLAYER] = 3;
        mPlayerID = fopAcM_GetID(player);
    }

    cXyz target_pos = current.pos;

    if (mSinkTimers[TIMER_ZANT] != 0) {
        fopAc_ac_c* target;
        fopAcM_SearchByID(mBossID, &target);

        if (target != NULL) {
            target_pos = target->current.pos;
        }
    }

    bool prev_sink_zant = mSinkZant;
    mSinkZant = false;
    mSinkPlayer = false;
    if (mSinkTimers[TIMER_ZANT] != 0) {
        mSinkZant = true;
    }

    if (mSinkTimers[TIMER_PLAYER] != 0) {
        mSinkPlayer = true;
    }

    f32 x = target_pos.x - current.pos.x;
    f32 z = target_pos.z - current.pos.z;
    target_pos.absXZ(current.pos);
    cLib_addCalcAngleS2(&mRotation.y, cM_atan2s(x, z), 8, 0x400);

    if (!prev_sink_zant && mSinkZant) {
        if (mRotation.x == 0) {
            mRotation.y = cM_atan2s(x, z);
        }
        mRotation.z = 0x400;
    }

    cLib_addCalcAngleS2(&mRotation.x, 0, 8, 0x40);
    mRotation.x += mRotation.z;
    if (0x1400 < mRotation.x) {
        mRotation.x = 0x1400;
        mRotation.z = 0;
    }
    cLib_chaseAngleS(&mRotation.z, 0, 0x80);

    f32 target_height = 150.0f;
    if (mSinkTimers[TIMER_PLAYER] != 0 || mSinkTimers[TIMER_ZANT] != 0) {
        target_height = 130.0f;
    }

    cLib_addCalc2(&mHeight, target_height, 0.2f, 20.0f);
    for (int i = 0; i < 2; i++) {
        if (mSinkTimers[i] != 0) {
            mSinkTimers[i]--;
        }
    }
}

int daB_ZANTS_c::execute() {
    mpBrkAnm->play();
    mpBtkAnm->play();

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mHeight, current.pos.z);
    mDoMtx_stack_c::YrotM(mRotation.y);
    mDoMtx_stack_c::XrotM(mRotation.x);
    mDoMtx_stack_c::YrotM(-mRotation.y);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(0.0f, -30.0f, 0.0f);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mMtx);

    if (mpBgW != NULL) {
        mpBgW->Move();
    }

    action();
    return 1;
}

static int daB_ZANTS_Execute(daB_ZANTS_c* i_this) {
    return i_this->execute();
}

static int daB_ZANTS_IsDelete(daB_ZANTS_c* i_this) {
    return 1;
}

int daB_ZANTS_c::_delete() {
    dComIfG_resDelete(&mPhase, "MAGNESIMA");
    if (mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    return 1;
}

static int daB_ZANTS_Delete(daB_ZANTS_c* i_this) {
    return i_this->_delete();
}

int daB_ZANTS_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::BMDR_S_MAGNE_SIMA);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return FALSE;
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return FALSE;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::BTK_S_MAGNE_SIMA);
    if (!mpBtkAnm->init(mpModel->getModelData(), pbtk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0,
                        -1))
    {
        return FALSE;
    }

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return FALSE;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::BRK_S_MAGNE_SIMA);
    if (!mpBrkAnm->init(mpModel->getModelData(), pbrk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0,
                        -1))
    {
        return FALSE;
    }

    mpBgW = new dBgW();
    if (mpBgW == NULL) {
        return FALSE;
    }

    cBgD_t* pdzb = (cBgD_t*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::DZB_S_MAGNE_SIMA);
    if (mpBgW->Set(pdzb, cBgW::MOVE_BG_e, &mMtx)) {
        return FALSE;
    }

    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    mpBgW->SetRideCallback(zants_ride_call_back);
    return TRUE;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daB_ZANTS_c*>(i_this)->CreateHeap();
}

int daB_ZANTS_c::create() {
    fopAcM_ct(this, daB_ZANTS_c);

    int phase_state = dComIfG_resLoad(&mPhase, "MAGNESIMA");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x3AA0)) {
            return cPhs_ERROR_e;
        }

        if (mpBgW && dComIfG_Bgsp().Regist(mpBgW, this)) {
            return cPhs_ERROR_e;
        }

        attention_info.flags = 0;
        mHeight = 150.0f;
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -500.0f, -500.0f);
        fopAcM_SetMax(this, 200.0f, 500.0f, 500.0f);
        daB_ZANTS_Execute(this);
    }

    return phase_state;
}

static int daB_ZANTS_Create(daB_ZANTS_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daB_ZANTS_Method = {
    (process_method_func)daB_ZANTS_Create,  (process_method_func)daB_ZANTS_Delete,
    (process_method_func)daB_ZANTS_Execute, (process_method_func)daB_ZANTS_IsDelete,
    (process_method_func)daB_ZANTS_Draw,
};

extern actor_process_profile_definition g_profile_B_ZANTS = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_B_ZANTS,
    &g_fpcLf_Method.base,
    sizeof(daB_ZANTS_c),
    0,
    0,
    &g_fopAc_Method.base,
    229,
    &l_daB_ZANTS_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
