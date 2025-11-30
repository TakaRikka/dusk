/**
 * d_a_obj_suisya.cpp
 * Ordon Village Water Wheel
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_suisya.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

int daObj_Suisya_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);

    cXyz sp18;
    sp18.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, mpModel, &sp18, 400.0f, 0.0f, current.pos.y,
                                    mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daObj_Suisya_Draw(daObj_Suisya_c* i_this) {
    return i_this->draw();
}

int daObj_Suisya_c::execute() {
    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_WATERMILL_ROUND, &current.pos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
    shape_angle.x += 25;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    return 1;
}

static int daObj_Suisya_Execute(daObj_Suisya_c* i_this) {
    return i_this->execute();
}

static int daObj_Suisya_IsDelete(daObj_Suisya_c* i_this) {
    return 1;
}

int daObj_Suisya_c::_delete() {
    dComIfG_resDelete(&mPhase, "Obj_sui");
    return 1;
}

static int daObj_Suisya_Delete(daObj_Suisya_c* i_this) {
    return i_this->_delete();
}

int daObj_Suisya_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_sui", 3);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daObj_Suisya_c*>(i_this)->CreateHeap();
}

int daObj_Suisya_c::create() {
    fopAcM_ct(this, daObj_Suisya_c);

    int phase = dComIfG_resLoad(&mPhase, "Obj_sui");
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x800)) {
            return cPhs_ERROR_e;
        }

        attention_info.flags = 0;
        cullMtx = mpModel->getBaseTRMtx();
        fopAcM_SetMin(this, -200.0f, -500.0f, -500.0f);
        fopAcM_SetMax(this, 200.0f, 500.0, 500.0f);

        mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
        mAcchCir.SetWall(0.0f, 0.0f);
        health = 60;
        field_0x560 = 60;

        daObj_Suisya_Execute(this);
    }

    return phase;
}

static int daObj_Suisya_Create(daObj_Suisya_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daObj_Suisya_Method = {
    (process_method_func)daObj_Suisya_Create,  (process_method_func)daObj_Suisya_Delete,
    (process_method_func)daObj_Suisya_Execute, (process_method_func)daObj_Suisya_IsDelete,
    (process_method_func)daObj_Suisya_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_SUISYA = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_OBJ_SUISYA,
    &g_fpcLf_Method.base,
    sizeof(daObj_Suisya_c),
    0,
    0,
    &g_fopAc_Method.base,
    65,
    &l_daObj_Suisya_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
