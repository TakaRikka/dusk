/**
 * @file d_a_obj_ice_l.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ice_l.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "d/d_s_play.h"
#include <cmath.h>

f32 dummyLiteral() {
    return 0.0f;
}

void daObjIce_l_c::RideOn_Angle(s16& i_angle, f32 param_1, s16 param_2, f32 param_3) {
    cLib_addCalcAngleS(&i_angle, param_2 * (param_1 / param_3), 1, 0x100, 1);
}

int daObjIce_l_c::Check_RideOn(cXyz param_0) {
    fopAc_ac_c* player_p = (fopAc_ac_c*)daPy_getPlayerActorClass();
    f32 player_speed = 0.0f;
    s16 var_r29 = 0;
    field_0x5e6 = 1;

    s16 target = cLib_targetAngleY(&current.pos, &param_0);
    f32 dist_to_pos = current.pos.abs(param_0);

    f32 var_f31 = dist_to_pos * cM_scos(target - shape_angle.y);
    f32 var_f30 = -dist_to_pos * cM_ssin(target - shape_angle.y);

    RideOn_Angle(field_0x5d8.x, var_f31, 352.0f / field_0x5b4.x, field_0x5b4.x * 600.0f);
    RideOn_Angle(field_0x5d8.z, var_f30, 352.0f / field_0x5b4.x, field_0x5b4.x * 500.0f);
    f32 var_f27 = 0.0f;
    field_0x5e6 = 1;

    player_speed = fopAcM_GetSpeedF(player_p);
    if (player_speed > 0.0f) {
        var_r29 = (48.0f + HREG_F(15)) * player_speed;
        field_0x5ac = 0x150;
    }

    f32 var_f26 = 1.0f - std::abs(var_f30) / 300.0f;
    field_0x5e0 = (1.0f - ((std::abs(var_f31) / 2500.0f) * var_f26)) * HREG_F(3);

    cLib_addCalc(&field_0x5a0, var_r29, 0.015f + HREG_F(13), 100.0f, 0.0f);
    return 0;
}

void daObjIce_l_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjIce_l_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(field_0x5b4);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

static void rideCallBack(dBgW* param_0, fopAc_ac_c* param_1, fopAc_ac_c* param_2) {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    cXyz* ball_pos = player_p->getIronBallCenterPos();
    cXyz& player_pos = fopAcM_GetPosition(player_p);
    daObjIce_l_c* ice_p = static_cast<daObjIce_l_c*>(param_1);

    //!@bug Missing parentheses causes this comparison to always evaluate to false
    if (!fopAcM_GetName(param_2) == PROC_ALINK) {
        ice_p->field_0x5f4 = 0x100;
        ice_p->field_0x5a4 = -1.0f;
        ice_p->field_0x5ac = 0x500;
    } else {
        ice_p->Check_RideOn(player_pos);
    }

    cXyz* ice_pos = &fopAcM_GetPosition(ice_p);
    if (ball_pos != NULL && ice_pos != NULL &&
        ice_pos->absXZ(*ball_pos) < ice_p->field_0x5b4.x * 600.0f)
    {
        ice_p->field_0x5f4 = 0x400;
        ice_p->field_0x5a4 = -6.0f;
        ice_p->field_0x5ac = 0x700;
        ice_p->Check_RideOn(*ball_pos);
    }

    if (player_p->checkBootsOrArmorHeavy()) {
        ice_p->field_0x5a4 = -6.0f;
        ice_p->field_0x5ac = 0x200;
        ice_p->Check_RideOn(player_pos);
    }

    if (ice_p->field_0x5b1 == 0xFF && fopAcM_GetName(param_2) == PROC_ALINK) {
        ice_p->field_0x5f4 = 0x300;
        ice_p->field_0x5a4 = -6.0f;
        ice_p->field_0x5ac = 0x1000;
        ice_p->Check_RideOn(player_pos);
    }
}

static int daObjIce_l_Draw(daObjIce_l_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjIce_l_Execute(daObjIce_l_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjIce_l_IsDelete(daObjIce_l_c* i_this) {
    return 1;
}

static int daObjIce_l_Delete(daObjIce_l_c* i_this) {
    i_this->MoveBGDelete();
    return 1;
}

static int daObjIce_l_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjIce_l_c*>(i_this)->create();
}

static char* l_arcName = "V_Ice_l";

int daObjIce_l_c::create() {
    fopAcM_ct(this, daObjIce_l_c);

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "Ice_l.dzb");
        JUT_ASSERT(108, dzb_id != -1);

        phase_state = MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x1440, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    field_0x5b1 = fopAcM_GetParam(this) & 0xFF;
    if (field_0x5b1 == 0) {
        field_0x5b0 = 1;
    }

    if (field_0x5b0 != 0) {
        if (dComIfGs_getCollectSmell() == fpcNm_ITEM_SMELL_FISH) {
            field_0x5b1 = 1;
        } else {
            field_0x5b1 = 0xFF;
        }
    }

    u8 prm1 = (fopAcM_GetParam(this) >> 8) & 0xFF;
    if (prm1 == 0xFF) {
        prm1 = 0;
    }
    field_0x5b4.set(prm1 + 1.0f, 1.0f, prm1 + 1.0f);

    cXyz gndchk_pos(current.pos.x, current.pos.y + 300.0f, current.pos.z);
    field_0x5e4 = cM_rndFX(32000.0f);

    dBgS_ObjGndChk_Spl gndchk;
    gndchk.SetPos(&gndchk_pos);
    field_0x5c0 = current.pos.y;

    f32 cull_size = field_0x5b4.x * 1000.0f;
    fopAcM_setCullSizeFar(this, 3.0f);
    fopAcM_setCullSizeBox(this, -cull_size, -cull_size, -cull_size, cull_size, cull_size,
                          cull_size);
    return phase_state;
}


int daObjIce_l_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "Ice_l.bmd");
    JUT_ASSERT(82, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObjIce_l_c::Create() {
    initBaseMtx();
    mpBgW->SetRideCallback(rideCallBack);
    return cPhs_COMPLEATE_e;
}

int daObjIce_l_c::Execute(Mtx** param_0) {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    cXyz& pos = fopAcM_GetPosition(player_p);
    cXyz sp68(current.pos.x, current.pos.y - 300.0f, current.pos.z);
    f32 var_f31 = 0.0f;
    var_f31 = 10.0f;

    cLib_chaseF(&field_0x5a4, 0.0f, yREG_F(3) + 0.04f);
    field_0x5d4 += (TREG_S(1) + 848) + ((HREG_F(1) + 1.5f) * field_0x5a0) + (field_0x5a4 * HREG_F(9));
    field_0x5d6 += (TREG_S(2) + 848) + ((HREG_F(2) + 1.5f) * field_0x5a0) + (field_0x5a4 * HREG_F(9));

    cLib_addCalc(&current.pos.y,
                 field_0x5e0 + (field_0x5c0 + (field_0x5a4 * (11.0f + var_f31)) +
                                (cM_ssin(field_0x5d6 + oREG_S(3) + field_0x5e4) * (HREG_F(4) + 5.0f))),
                 0.1f, 15.0f, 0.1f);
    cLib_addCalcAngleS(&shape_angle.x,
                       field_0x5d8.x +
                           ((nREG_F(0) + 0.5f) * field_0x5ac * cM_ssin(0x2000 + field_0x5d4 + oREG_S(1) + field_0x5e4)),
                       2, 0x100, 1);
    cLib_addCalcAngleS(&shape_angle.z,
                       field_0x5d8.z + ((nREG_F(1) + 0.5f) * field_0x5ac * cM_ssin(field_0x5d6 + oREG_S(2) + field_0x5e4)), 2,
                       0x100, 1);

    if (field_0x5e6 == 0) {
        cLib_addCalcAngleS(&field_0x5d8.x, 0, 2, 0x100, 1);
        cLib_addCalcAngleS(&field_0x5d8.z, 0, 2, 0x100, 1);
        cLib_addCalc(&field_0x5e0, 0.0f, 0.2f, 100.0f, 0.0f);
        cLib_addCalc(&field_0x5a0, 0.0f, 0.05f + HREG_F(13), 100.0f, 0.0f);
        cLib_chaseAngleS(&field_0x5ac, 0x120, 0x10);
    }

    field_0x5e6 = 0;
    *param_0 = &mBgMtx;
    setBaseMtx();
    return 1;
}

int daObjIce_l_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListDarkBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjIce_l_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjIce_l_Method = {
    (process_method_func)daObjIce_l_Create,
    (process_method_func)daObjIce_l_Delete,
    (process_method_func)daObjIce_l_Execute,
    (process_method_func)daObjIce_l_IsDelete,
    (process_method_func)daObjIce_l_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Ice_l = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Ice_l,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  0x00000640,             // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  456,                    // mPriority
  &l_daObjIce_l_Method,   // sub_method
  0x00044180,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
