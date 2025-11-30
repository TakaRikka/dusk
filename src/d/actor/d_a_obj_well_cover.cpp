/**
 * Object - Well Cover
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_well_cover.h"
#include "d/d_procname.h"
#include "d/actor/d_a_player.h"

void daObjWCover_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjWCover_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjWCover_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    return 1;
}

static char* l_arcName = "H_Idohuta";

int daObjWCover_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, 4));
    JUT_ASSERT(194, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

int daObjWCover_c::create() {
    fopAcM_ct(this, daObjWCover_c);
    if (fopAcM_isSwitch(this, getSwNo())) {
        return cPhs_ERROR_e;
    }

    int rv = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        rv = MoveBGCreate(l_arcName, 7, NULL, 0xd90, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
    }
    
    return rv;
}

int daObjWCover_c::Execute(Mtx** i_mtx) {
    action();
    *i_mtx = &mBgMtx;
    setBaseMtx();
    return 1;
}

typedef void (daObjWCover_c::*modeFunc)();

void daObjWCover_c::action() {
    static modeFunc l_func[] = {
        &daObjWCover_c::modeWait,
        &daObjWCover_c::modeBreak,
    };

    (this->*l_func[field_0x5b0])();
}

void daObjWCover_c::modeWait() {
    cXyz acStack_48(150.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&acStack_48, &acStack_48);
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 dVar6 = player->current.pos.absXZ(acStack_48);
    f32 fVar1 = acStack_48.y - player->current.pos.y;
    if (dVar6 < 80.0f && fVar1 > 0.0f && fVar1 < 200.0f) {
        init_modeBreak();
    }
}

static u16 const particle_id[3] = {
    0x82AB,
    0x82AC,
    0x82AD,
};

void daObjWCover_c::init_modeBreak() {
    fopAcM_onSwitch(this, getSwNo());
    cXyz acStack_28(150.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&acStack_28, &acStack_28);
    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(particle_id[i], &acStack_28, NULL, NULL);
    }
    fopAcM_delete(this);
    field_0x5b0 = 1;
}

void daObjWCover_c::modeBreak() {
    /* empty function */
}

int daObjWCover_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjWCover_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjWCover_Draw(daObjWCover_c* i_this) {
    return i_this->Draw();
}

static int daObjWCover_Execute(daObjWCover_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjWCover_Delete(daObjWCover_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjWCover_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjWCover_c*>(i_this)->create();
}

static actor_method_class l_daObjWCover_Method = {
    (process_method_func)daObjWCover_Create,
    (process_method_func)daObjWCover_Delete,
    (process_method_func)daObjWCover_Execute,
    (process_method_func)NULL,
    (process_method_func)daObjWCover_Draw,
};

extern actor_process_profile_definition g_profile_Obj_WellCover = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_WellCover,     // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daObjWCover_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    0x25E,                  // mPriority
    &l_daObjWCover_Method,   // sub_method
    0x40100,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
