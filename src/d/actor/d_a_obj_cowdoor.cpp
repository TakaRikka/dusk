/**
 * d_a_obj_cowdoor.cpp
 * Cow Door
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_cowdoor.h"
#include "d/d_procname.h"

void daCowdoor_c::setBaseMtx() {
    mDoMtx_stack_c::transS( current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    field_0x5a8->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daCowdoor_c::Create() {
    setBaseMtx();

    fopAcM_SetMtx(this, field_0x5a8->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, field_0x5a8->getModelData());
    fopAcM_setCullSizeFar(this, 5.0f);
    return TRUE;
}

static const char* l_arcName = "A_UHDoor";

static const char* l_bmdName = "A_UHDoor.bmd";

static const char* l_dzbName = "A_UHDoor.dzb";

int daCowdoor_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmdName);
    JUT_ASSERT(140, modelData != NULL);
    field_0x5a8 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (field_0x5a8 == NULL) {
        return FALSE;
    }
    return TRUE;
}

int daCowdoor_c::create() {
    fopAcM_ct(this, daCowdoor_c);
    int phase = dComIfG_resLoad(&field_0x5a0, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, l_dzbName);
        JUT_ASSERT(169, dzb_id != -1);
        phase = MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

int daCowdoor_c::Execute(Mtx** param_0) {
    *param_0 = &mBgMtx;
    setBaseMtx();
    return TRUE;
}

int daCowdoor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(field_0x5a8, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(field_0x5a8);
    dComIfGd_setList();
    return TRUE;
}

int daCowdoor_c::Delete() {
    dComIfG_resDelete(&field_0x5a0, l_arcName);
    return TRUE;
}

static int daCowdoor_Draw(daCowdoor_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daCowdoor_Execute(daCowdoor_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daCowdoor_Delete(daCowdoor_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    (void)"Delete -> Cowdoor(id=%d)\n";
    return i_this->MoveBGDelete();
}

static int daCowdoor_Create(fopAc_ac_c* i_this) {
    daCowdoor_c* cowDoor = (daCowdoor_c*)i_this;
    fpc_ProcID id = fopAcM_GetID(i_this);
    (void)"Create -> Cowdoor(id=%d)\n";
    return cowDoor->create();
}

static actor_method_class l_daCowdoor_Method = {
    (process_method_func)daCowdoor_Create,  (process_method_func)daCowdoor_Delete,
    (process_method_func)daCowdoor_Execute, (process_method_func)NULL,
    (process_method_func)daCowdoor_Draw,
};

actor_process_profile_definition g_profile_Obj_Cowdoor = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_Cowdoor,       // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daCowdoor_c),    // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    447,                    // mPriority
    &l_daCowdoor_Method,    // sub_method
    0x40100,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
