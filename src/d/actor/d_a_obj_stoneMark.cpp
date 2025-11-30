/**
 * d_a_obj_stoneMark.cpp
 * Actor - Object Stone Mark
 *
 * Visual mark under the stone actor (d_a_obj_stone)
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_stoneMark.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_lib.h"

static char* l_arcName = "A_IwaAto";

void daObjSMark_c::initBaseMtx() {
    setBaseMtx();
}

void daObjSMark_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    MTXCopy(mDoMtx_stack_c::get(), cullMtx);
}

int daObjSMark_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, cullMtx);
    fopAcM_setCullSizeSphere(this, 0.0f, 0.0f, 0.0f, 100.0f);
    mAcchCir.SetWall(30.0f, 30.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                 fopAcM_GetSpeed_p(this), 0, 0);
    mObjAcch.CrrPos(dComIfG_Bgsp());
    mDoLib_setResTimgObj((ResTIMG*)dComIfG_getObjectRes(l_arcName, 3), &mTexObj, 0, 0);
    return 1;
}

int daObjSMark_c::create() {
    fopAcM_ct(this, daObjSMark_c);

    int phase = dComIfG_resLoad(&mpPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }

    return phase;
}

int daObjSMark_c::draw() {
    static const f32 l_shadow_size[2] = {55.0f, 85.0f};
    dComIfGd_setSimpleShadow(&current.pos, mObjAcch.GetGroundH(),
                             l_shadow_size[fopAcM_GetParam(this) & 0xFF], mObjAcch.m_gnd,
                             shape_angle.y, (-0.6f - g_env_light.field_0x1238 * (0.7f - 0.3f)),
                             &this->mTexObj);
    return 1;
}

int daObjSMark_c::_delete() {
    dComIfG_resDelete(&mpPhase, l_arcName);
    return 1;
}
static int daObjSMark_Draw(daObjSMark_c* i_this) {
    return i_this->draw();
}

static int daObjSMark_Delete(daObjSMark_c* i_this) {
    return i_this->_delete();
}

static int daObjSMark_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjSMark_c*>(i_this)->create();
}

static actor_method_class l_daObjSMark_Method = {
    (process_method_func)daObjSMark_Create,
    (process_method_func)daObjSMark_Delete,
    (process_method_func)NULL,
    (process_method_func)NULL,
    (process_method_func)daObjSMark_Draw,
};

extern actor_process_profile_definition g_profile_Obj_StoneMark = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_StoneMark,     // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daObjSMark_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    534,                    // mPriority
    &l_daObjSMark_Method,   // sub_method
    0x40100,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLSPHERE_CUSTOM_e,   // cullType
};
