/**
 * d_a_obj_mgate.cpp
 * Ordon Spring / Bridge Gates
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mgate.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_lib.h"
#include "d/d_procname.h"

static Vec const l_key_offset[2] = {
    {205.0f, 180.0f, 0.0f},
    {230.0f, 180.0f, 0.0f},
};

static f32 const l_col_offsetX[5] = {
    80.0f, 160.0f, 240.0f, 320.0f, 400.0f,
};

static Vec const l_roll_crash_check_areaL[4] = {
    {0.0f, 0.0f, -140.0f},
    {410.0f, 0.0f, -140.0f},
    {410.0f, 0.0f, 140.0f},
    {0.0f, 0.0f, 140.0f},
};

static Vec const l_roll_crash_check_areaL2[4] = {
    {0.0f, 0.0f, -140.0f},
    {460.0f, 0.0f, -140.0f},
    {460.0f, 0.0f, 140.0f},
    {0.0f, 0.0f, 140.0f},
};

void daObjMGate_c::initBaseMtx() {
    if (mpKeyModel != NULL) {
        mpKeyModel->setBaseScale(scale);
    }

    if (getType() == TYPE_LEFT) {
        scale.z = -1.0f;
    }

    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjMGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZXYrotM(mGateAngle.x, mGateAngle.y, mGateAngle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    if (mpKeyModel != NULL) {
        cXyz key_offset(l_key_offset[mKind]);

        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::ZXYrotM(mGateAngle.x, mGateAngle.y, mGateAngle.z);
        mDoMtx_stack_c::transM(key_offset);
        mDoMtx_stack_c::XrotM(mGateKeyAngle);
        mpKeyModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

int daObjMGate_c::Create() {
    initBaseMtx();

    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    return 1;
}

static char* l_arcName[] = {
    "M_IGate",
    "M_HGate",
};

static char* l_bmdName[] = {
    "M_IzumiGate.bmd",
    "M_HashiGate.bmd",
};

static char* l_dzbName[] = {
    "M_IzumiGate.dzb",
    "M_HashiGate.dzb",
};

static char* l_keyArcName = "M_GateKey";

static char* l_keyBmdName = "M_IGateKey.bmd";

int daObjMGate_c::CreateHeap() {
    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(l_arcName[mKind], l_bmdName[mKind]);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    if (!checkLock()) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_keyArcName, l_keyBmdName);
        mpKeyModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpKeyModel == NULL) {
            return 0;
        }
    }

    return 1;
}

int daObjMGate_c::create1st() {
    mKind = getKind();
    mType = getType();

    int phase = dComIfG_resLoad(&mKeyPhase, l_keyArcName);
    if (phase != cPhs_COMPLEATE_e) {
        return phase;
    }

    phase = dComIfG_resLoad(&mPhase, l_arcName[mKind]);
    if (phase == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName[mKind], l_dzbName[mKind]);

        phase =
            MoveBGCreate(l_arcName[mKind], dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x80004000, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    return phase;
}

// unused but still left in?
#pragma push
#pragma force_active on
static u8 const lit_3752[4] = {
    0x00,
    0x00,
    0x00,
    0x00,
};
#pragma pop

int daObjMGate_c::Execute(Mtx** param_0) {
    daPy_py_c* player_p = dComIfGp_getLinkPlayer();
    mCounter++;

    if (player_p->checkFrontRollCrash()) {
        cXyz check_area_posA;
        cXyz check_area_posB;

        switch (mKind) {
        case KIND_IZUMI:
            check_area_posA = l_roll_crash_check_areaL[0];
            check_area_posB = l_roll_crash_check_areaL[2];
            break;
        case KIND_HASHI:
            check_area_posA = l_roll_crash_check_areaL2[0];
            check_area_posB = l_roll_crash_check_areaL2[2];
            break;
        }

        if (dLib_checkActorInRectangle(player_p, this, &check_area_posA, &check_area_posB)) {
            if (mType == TYPE_RIGHT) {
                mGateSwingMultiplier = 700.0f;
                mGateKeySwingMultiplier = 1000.0f;

                if (checkOpen() == true) {
                    fopAcM_seStart(this, Z2SE_OBJ_IGATE_ATK_OP, 0);
                } else {
                    fopAcM_seStart(this, Z2SE_OBJ_IGATE_ATK_CL, 0);
                }
            } else {
                mGateSwingMultiplier = -700.0f;
                mGateKeySwingMultiplier = -1000.0f;

                if (checkOpen() == true) {
                    fopAcM_seStart(this, Z2SE_OBJ_IGATE_ATK_OP, 0);
                }
            }
        }
    }

    *param_0 = &mBgMtx;
    setBaseMtx();

    mGateAngle.y = mGateSwingMultiplier * cM_ssin(mCounter * 4000);
    mGateKeyAngle = mGateKeySwingMultiplier * cM_ssin(mCounter * 6000);
    cLib_addCalc0(&mGateSwingMultiplier, 0.1f, 20.0f);
    cLib_addCalc0(&mGateKeySwingMultiplier, 0.07f, 20.0f);
    return 1;
}

int daObjMGate_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    if (mpKeyModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpKeyModel, &tevStr);
        mDoExt_modelUpdateDL(mpKeyModel);
    }

    return 1;
}

int daObjMGate_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName[mKind]);
    dComIfG_resDelete(&mKeyPhase, l_keyArcName);
    return 1;
}

static int daObjMGate_create1st(daObjMGate_c* i_this) {
    fopAcM_ct(i_this, daObjMGate_c);
    return i_this->create1st();
}

static int daObjMGate_MoveBGDelete(daObjMGate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjMGate_MoveBGExecute(daObjMGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjMGate_MoveBGDraw(daObjMGate_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjMGate_METHODS = {
    (process_method_func)daObjMGate_create1st,     (process_method_func)daObjMGate_MoveBGDelete,
    (process_method_func)daObjMGate_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjMGate_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_MGate = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_MGate,
    &g_fpcLf_Method.base,
    sizeof(daObjMGate_c),
    0,
    0,
    &g_fopAc_Method.base,
    452,
    &daObjMGate_METHODS,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
