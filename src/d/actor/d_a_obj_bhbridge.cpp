/**
 * @file d_a_obj_bhbridge.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bhbridge.h"
#include "SSystem/SComponent/c_lib.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_player.h"
#include <cmath.h>

void daObjBhbridge_c::RideOn_Angle(s16& param_1, f32 param_2, s16 param_3, f32 param_4) {
    cLib_addCalcAngleS(&param_1, param_3 * (param_2 / param_4), 1, 0x100, 1);
}

int daObjBhbridge_c::Check_RideOn() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz& playerPos = fopAcM_GetPosition(player);
    f32 playerSpeed = 0.0f;
    s16 sVar8 = 0;
    field_0x5de = 1;
    s16 sVar7 = cLib_targetAngleY(&current.pos, &playerPos);
    f32 playerDist = current.pos.abs(playerPos);
    f32 dVar12 = playerDist * cM_scos(sVar7 - shape_angle.y);
    f32 dVar10 = -playerDist * cM_ssin(sVar7 - shape_angle.y);
    RideOn_Angle(field_0x5ce.x, dVar12, 0x50, 180.0f);
    f32 var_f27 = 0.0f;
    field_0x5de = 1;
    playerSpeed = fopAcM_GetSpeedF(player);
    if (playerSpeed > 0.0f) {
        sVar8 = 0x400;
        field_0x5b0 = 0x150;
    }
    f32 dVar13 = 1.0f - (std::abs(dVar10) / 300.0f);
    field_0x5d4 = HREG_F(3) * (1.0f - (std::abs(dVar12) / 2500.0f) * dVar13);
    cLib_addCalc(&field_0x5a0, sVar8, HREG_F(13) + 0.05f, 100.0f, 0);
    return 0;
}

void daObjBhbridge_c::initBaseMtx() {
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjBhbridge_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

static void rideCallBack(dBgW* param_1, fopAc_ac_c* i_this, fopAc_ac_c* param_3) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (!((daObjBhbridge_c*)i_this)->field_0x5de &&
        player->current.pos.y - player->old.pos.y < -1.0f)
    {
        ((daObjBhbridge_c*)i_this)->field_0x5a8 = -6.0f;
    }
    ((daObjBhbridge_c*)i_this)->Check_RideOn();
}

static int daObjBhbridge_Draw(daObjBhbridge_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjBhbridge_Execute(daObjBhbridge_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjBhbridge_IsDelete(daObjBhbridge_c* i_this) {
    return 1;
}

static int daObjBhbridge_Delete(daObjBhbridge_c* i_this) {
    i_this->MoveBGDelete();
    return 1;
}

static char* l_arcName = "BHBridge";

inline int daObjBhbridge_c::create() {
    fopAcM_ct(this, daObjBhbridge_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "M_BHbridge.dzb");
        JUT_ASSERT(109, dzb_id != -1);
        rv = MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x15a0, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcch.CrrPos(dComIfG_Bgsp());
        cXyz cStack_90(current.pos.x, current.pos.y + 300.0f + HREG_F(1), current.pos.z);
        dBgS_ObjGndChk_Spl adStack_84;
        adStack_84.SetPos(&cStack_90);
        f32 ground = dComIfG_Bgsp().GroundCross(&adStack_84);
        if (ground != -G_CM3D_F_INF) {
            current.pos.y = ground + 100.0f;
        }
    }
    return rv;
}

inline int daObjBhbridge_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "M_BHbridge.bmd");
    JUT_ASSERT(83, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mModel != NULL ? TRUE : FALSE;
}

static int daObjBhbridge_Create(fopAc_ac_c* a_this) {
    daObjBhbridge_c* i_this = static_cast<daObjBhbridge_c*>(a_this);
    fpc_ProcID id = fopAcM_GetID(a_this);
    return i_this->create();
}

inline int daObjBhbridge_c::Create() {
    fopAcM_setCullSizeBox(this, -400.0f, -800.0f, -1000.0f, 400.0f, 300.0f, 1000.0f);
    initBaseMtx();
    mpBgW->SetRideCallback(rideCallBack);
    return cPhs_COMPLEATE_e;
}

inline int daObjBhbridge_c::Execute(f32 (**i_mtxP)[3][4]) {
    cXyz cStack_5c(current.pos.x, current.pos.y + HREG_F(0), current.pos.z);
    f32 dVar10 = current.pos.y;
    if (fopAcM_wt_c::waterCheck(&cStack_5c)) {
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_BH_HASHI_LV, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                      -1.0f, 0);
        dVar10 = HREG_F(7) + fopAcM_wt_c::getWaterY();
        field_0x5a4 = true;
    } else {
        cStack_5c.y += HREG_F(1);
        current.pos.y = mAcch.GetGroundH();
    }
    if (field_0x5a4) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz& local_60 = fopAcM_GetPosition(player);
        field_0x5ca += TREG_S(1) + 0x350 + field_0x5a0 * 2.0f + field_0x5a8 * HREG_F(9);
        field_0x5cc += TREG_S(1) + 0x350 + field_0x5a0 * 2.0f + field_0x5a8 * HREG_F(9);
        if (field_0x5a0 > 512.0f && !field_0x5c6) {
            field_0x5c6 = true;
            if (field_0x5a4) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_BH_HASHI_TRG, &current.pos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
            }
        } else {
            if (field_0x5a0 < 256.0f) {
                field_0x5c6 = false;
            }
        }
        cLib_addCalc(&current.pos.y,
                     (HREG_F(4) + 5.0f) * cM_ssin(field_0x5cc + oREG_S(3) + field_0x5dc) +
                         (dVar10 + field_0x5a8 * 11.0f) + field_0x5d4,
                     0.1f, 15.0f, 0.1f);
        cLib_addCalcAngleS(&shape_angle.x, field_0x5ce.x + ((nREG_F(0) + 0.5f) * field_0x5b0) * cM_ssin(field_0x5ca + oREG_S(1) + 0x2000 + field_0x5dc), 2, 0x100, 1);
        cLib_addCalcAngleS(&shape_angle.z, field_0x5ce.z + ((nREG_F(1) + 1.0f) * field_0x5b0) * cM_ssin(field_0x5cc + oREG_S(2) + field_0x5dc), 2, 0x100, 1);
        if (!field_0x5de) {
            cLib_addCalcAngleS(&field_0x5ce.x, 0, 2, 0x100, 1);
            cLib_addCalcAngleS(&field_0x5ce.z, 0, 2, 0x100, 1);
            cLib_addCalc(&field_0x5d4, 0.0f, 0.2f, 100.0f, 0.0f);
            cLib_addCalc(&field_0x5a0, 0.0f, HREG_F(13) + 0.05f, 100.0f, 0.0f);
        }
        cLib_addCalc(&field_0x5a8, 0.0f, 0.2f, 100.0f, 0.0f);
        field_0x5de = false;
        field_0x5b0 = 0x120;
    }
    *i_mtxP = &mBgMtx;
    setBaseMtx();
    mAcch.CrrPos(dComIfG_Bgsp());
    return 1;
}

inline int daObjBhbridge_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

inline int daObjBhbridge_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjBhbridge_Method = {
    (process_method_func)daObjBhbridge_Create,
    (process_method_func)daObjBhbridge_Delete,
    (process_method_func)daObjBhbridge_Execute,
    (process_method_func)daObjBhbridge_IsDelete,
    (process_method_func)daObjBhbridge_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Bhbridge = {
  fpcLy_CURRENT_e,         // mLayerID
  3,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_Bhbridge,       // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjBhbridge_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  459,                     // mPriority
  &l_daObjBhbridge_Method, // sub_method
  0x00040180,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
