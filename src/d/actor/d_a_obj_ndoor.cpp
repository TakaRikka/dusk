/**
 * @file d_a_obj_ndoor.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ndoor.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"

static int daObj_Ndoor_Draw(obj_ndoor_class* i_this) {
    g_env_light.settingTevStruct(0x10, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);
    return 1;
}

static int daObj_Ndoor_Execute(obj_ndoor_class* i_this) {
    cXyz cStack_28;

    if (daPy_getPlayerActorClass()->checkFrontRollCrash() &&
        fopAcM_searchPlayerDistanceXZ(i_this) < 350.0f)
    {
        i_this->field_0x567 = 1;
    }

    if (i_this->field_0x576 == 0 && i_this->mSph.ChkTgHit()) {
        i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
        at_power_check(&i_this->mAtInfo);

        if (i_this->mAtInfo.mAttackPower <= 1) {
            i_this->field_0x567 = 1;
        } else {
            i_this->field_0x567 = 2;
        }
    }

    if (i_this->field_0x567 != 0) {
        f32 tmp = ZREG_F(6) + 2500.0f;
        i_this->field_0x57c = tmp * i_this->field_0x567;
        i_this->field_0x578 = 0;
        i_this->field_0x567 = 0;
    }

    i_this->shape_angle.x =
        i_this->field_0x57c * cM_ssin(i_this->field_0x578 * (ZREG_S(2) + 0x1000));
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cStack_28 = i_this->current.pos;
    cStack_28.y -= ZREG_F(7) + 25.0f;

    if (i_this->field_0x576 != 0) {
        i_this->field_0x576--;
        cStack_28.y -= 27000.0f;
    }

    i_this->mSph.SetC(cStack_28);
    i_this->mSph.SetR(ZREG_F(8) + 30.0f);
    dComIfG_Ccsp()->Set(&i_this->mSph);
    i_this->field_0x578++;
    cLib_addCalc0(&i_this->field_0x57c, 0.05f, (ZREG_F(5) + 500.0f));
    return 1;
}

static int daObj_Ndoor_IsDelete(obj_ndoor_class* i_this) {
    return 1;
}

static int daObj_Ndoor_Delete(obj_ndoor_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "Obj_ndoor");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_ndoor_class* a_this = (obj_ndoor_class*)i_this;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_ndoor", 3);

    if (modelData == NULL) {
        JUT_ASSERT(305, modelData != NULL);
    }

    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return a_this->mpModel != NULL ? 1 : 0;
}

static int daObj_Ndoor_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                // mGObjTg
            {0x0},                                             // mGObjCo
        },                                                     // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
        }  // mSphAttr
    };

    obj_ndoor_class* a_this = (obj_ndoor_class*)i_this;
    fopAcM_ct(i_this, obj_ndoor_class);
    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&a_this->mPhase, "Obj_ndoor");

    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_NDOOR PARAM %x\n", fopAcM_GetParam(a_this));
        OS_REPORT("OBJ_NDOOR//////////////OBJ_NDOOR SET 1 !!\n");

        if (fopAcM_entrySolidHeap(i_this, useHeapInit, 0x820) == false) {
            OS_REPORT("//////////////OBJ_NDOOR SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_NDOOR SET 2 !!\n");
        a_this->mStts.Init(0xff, 0, a_this);
        a_this->mSph.Set(cc_sph_src);
        a_this->mSph.SetStts(&a_this->mStts);
        fopAcM_SetMtx(a_this, a_this->mpModel->getBaseTRMtx());
        daObj_Ndoor_Execute(a_this);
    }

    return phase;
}

static actor_method_class l_daObj_Ndoor_Method = {
    (process_method_func)daObj_Ndoor_Create,  (process_method_func)daObj_Ndoor_Delete,
    (process_method_func)daObj_Ndoor_Execute, (process_method_func)daObj_Ndoor_IsDelete,
    (process_method_func)daObj_Ndoor_Draw,
};

actor_process_profile_definition g_profile_OBJ_NDOOR = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_OBJ_NDOOR,           // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(obj_ndoor_class),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    76,                       // mPriority
    &l_daObj_Ndoor_Method,    // sub_method
    0x00040100,               // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_0_e,        // cullType
};
