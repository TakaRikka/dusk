/**
 * @file d_a_obj_ikada.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ikada.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"

static f32 dummy() {
    return 0.0f;
}

void daObjIkada_c::RideOn_Angle(s16& param_1, f32 param_2, s16 param_3, f32 param_4) {
    cLib_addCalcAngleS(&param_1, param_3 * (param_2 / param_4), 1, 0x100, 1);
}

BOOL daObjIkada_c::Check_RideOn() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz& pos = fopAcM_GetPosition(player);
    f32 fVar4 = 0.0f;
    s16 target = 0;

    field_0x5d6 = 1;
    s16 targetAngleY = cLib_targetAngleY(&current.pos, &pos);
    f32 fVar1 = current.pos.abs(pos);
    f32 fVar2 = fVar1 * cM_scos(targetAngleY - shape_angle.y);
    f32 fVar3 = -fVar1 * cM_ssin(targetAngleY - shape_angle.y);
    RideOn_Angle(field_0x5c8.x, fVar2, 0x200, 250.0f);
    RideOn_Angle(field_0x5c8.z, fVar3, 0x200, 300.0f);
    fVar4 = 0.0f;
    field_0x5d6 = 1;

    if (fopAcM_GetSpeedF(player) > fVar4) {
        target = 0x400;
        field_0x5ac = 0x150;
    }

    field_0x5d0 = HREG_F(3) * (1.0f - (std::abs(fVar2) / 2500.0f) * (1.0f - std::abs(fVar3) / 300.0f));
    cLib_addCalc(&field_0x5a0, target, HREG_F(13) + 0.05f, 100.0f, 0.0f);

    return FALSE;
}

void daObjIkada_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjIkada_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static void rideCallBack(dBgW* param_1, fopAc_ac_c* a_this, fopAc_ac_c* param_3) {
    daObjIkada_c* i_this = (daObjIkada_c*)a_this;
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (i_this->field_0x5d6 == 0 && player->current.pos.y - player->old.pos.y < -1.0f) {
        i_this->field_0x5a4 = -6.0f;
    }

    i_this->Check_RideOn();
}

static int daObjIkada_Draw(daObjIkada_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjIkada_Execute(daObjIkada_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjIkada_IsDelete(daObjIkada_c* i_this) {
    return 1;
}

static int daObjIkada_Delete(daObjIkada_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    i_this->MoveBGDelete();
    return 1;
}

static char* l_arcName = "M_Ikada";

int daObjIkada_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "M_Ikada.bmd");
    JUT_ASSERT(82, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

cPhs__Step daObjIkada_c::create() {
    fopAcM_ct(this, daObjIkada_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "M_Ikada.dzb");
        JUT_ASSERT(108, dzb_id != -1);
        
        phase = (cPhs__Step)MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0xC00, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
    mAcchCir.SetWall(100.0f, 200.0f);
    field_0x5d4 = cM_rndFX(32000.0f);
    fopAcM_setCullSizeBox(this, -250.0f, -50.0f, -300.0f, 250.0f, 50.0f, 300.0f);
    
    cXyz pos(current.pos.x, current.pos.y + 300.0f, current.pos.z);
    if (fopAcM_wt_c::waterCheck(&pos)) {
        current.pos.y = fopAcM_wt_c::getWaterY();
    }

    return phase;
}

static int daObjIkada_Create(fopAc_ac_c* a_this) {
    daObjIkada_c* i_this = (daObjIkada_c*)a_this;
    fpc_ProcID id = fopAcM_GetID(a_this);
    return i_this->create();
}

int daObjIkada_c::Create() {
    initBaseMtx();
    mpBgW->SetRideCallback(rideCallBack);
    return cPhs_COMPLEATE_e;
}

int daObjIkada_c::Execute(Mtx** mtx) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz& unk = fopAcM_GetPosition(player);
    cXyz pos(current.pos.x, current.pos.y + 300.0f, current.pos.z);
    f32 posY = current.pos.y;

    if (fopAcM_wt_c::waterCheck(&pos)) {
        posY = fopAcM_wt_c::getWaterY();
    }

    #if DEBUG
    field_0x5c4 = field_0x5c4 + (((TREG_S(1) + 0x350) + (HREG_F(1) + 2.0f)) * field_0x5a0 + field_0x5a4 * HREG_F(9));
    #else
    field_0x5c4 = field_0x5c4 + (((HREG_F(1) + 2.0f) * field_0x5a0) + 848.0f) + field_0x5a4 * HREG_F(9);
    #endif

    #if DEBUG
    field_0x5c6 = field_0x5c6 + (((TREG_S(2) + 0x350) + (HREG_F(2) + 2.0f)) * field_0x5a0 + field_0x5a4 * HREG_F(9));
    #else
    field_0x5c6 = field_0x5c6 + (((HREG_F(2) + 2.0f) * field_0x5a0) + 848.0f) + field_0x5a4 * HREG_F(9);
    #endif
    
    cLib_addCalc(&current.pos.y, field_0x5d0 + (posY + field_0x5a4 * 11.0f + ((HREG_F(4) + 5.0f) * cM_ssin(field_0x5c6 + oREG_S(3) + field_0x5d4))),
                 0.1f, 15.0f, 0.1f);
    cLib_addCalcAngleS(&shape_angle.x, field_0x5c8.x + (nREG_F(0) + 1.0f) * field_0x5ac * cM_ssin(oREG_S(1) + 0x2000 + field_0x5c4 + field_0x5d4), 2, 0x100, 1);
    cLib_addCalcAngleS(&shape_angle.z, field_0x5c8.z + (nREG_F(1) + 1.0f) * field_0x5ac * cM_ssin(field_0x5c6 + oREG_S(2) + field_0x5d4), 2, 0x100, 1);

    if (field_0x5d6 == 0) {
        cLib_addCalcAngleS(&field_0x5c8.x, 0, 2, 0x100, 1);
        cLib_addCalcAngleS(&field_0x5c8.z, 0, 2, 0x100, 1);
        cLib_addCalc(&field_0x5d0, 0.0f, 0.2f, 100.0f, 0.0f);
        cLib_addCalc(&field_0x5a0, 0.0f, HREG_F(13) + 0.05f, 100.0f, 0.0f);
    }
    
    cLib_addCalc(&field_0x5a4, 0.0f, 0.2f, 100.0f, 0.0f);
    field_0x5d6 = 0;
    field_0x5ac = 0x120;
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    *mtx = &mBgMtx;
    setBaseMtx();
    mBgc.CrrPos(dComIfG_Bgsp());

    return 1;
}

int daObjIkada_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjIkada_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjIkada_Method = {
    (process_method_func)daObjIkada_Create,
    (process_method_func)daObjIkada_Delete,
    (process_method_func)daObjIkada_Execute,
    (process_method_func)daObjIkada_IsDelete,
    (process_method_func)daObjIkada_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Ikada = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Ikada,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjIkada_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  455,                    // mPriority
  &l_daObjIkada_Method,   // sub_method
  0x00040180,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
