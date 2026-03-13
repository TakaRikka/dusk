// Translation Unit: d_a_tag_lightball

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_lightball.h"
#include "d/d_com_inf_game.h"
#include "f_pc/f_pc_name.h"

void daTagLightBall_c::initBaseMtx() {
    setBaseMtx();
}

void daTagLightBall_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daTagLightBall_c::Create() {
    initBaseMtx();
    u8 swbit = getSwbit();
    if (fopAcM_GetRoomNo(this) == 7) {
        if (!fopAcM_isSwitch(this, 70)) {
            fopAcM_offSwitch(this, swbit);
        }
    }

    mInfluence.mPos = current.pos;
    mInfluence.field_0xc = scale.x * 100.0f;

    if (swbit != 0xFF) {
        if (getType() == 15) {
            if (fopAcM_isSwitch(this, swbit)) {
                dKy_dalkmist_inf_set(&mInfluence);
                mInfSet = true;
            }
        } else {
            if (!fopAcM_isSwitch(this, swbit)) {
                dKy_dalkmist_inf_set(&mInfluence);
                mInfSet = true;
            }
        }
    }

    field_0x57c = fopAcM_isSwitch(this, swbit);
    return 1;
}

int daTagLightBall_c::create() {
    fopAcM_ct(this, daTagLightBall_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daTagLightBall_c::execute() {
    u8 swbit = getSwbit();
    if (swbit != 0xFF) {
        if (getType() == 15) {
            if (field_0x57c && !fopAcM_isSwitch(this, swbit)) {
                dKy_dalkmist_inf_cut(&mInfluence);
                mInfSet = false;
            } else if (!field_0x57c && fopAcM_isSwitch(this, swbit)) {
                dKy_dalkmist_inf_set(&mInfluence);
                mInfSet = true;
            }
        } else if (!field_0x57c && fopAcM_isSwitch(this, swbit)) {
            dKy_dalkmist_inf_cut(&mInfluence);
            mInfSet = false;
        } else if (field_0x57c && !fopAcM_isSwitch(this, swbit)) {
            dKy_dalkmist_inf_set(&mInfluence);
            mInfSet = true;
        }
    }

    mInfluence.mPos = current.pos;
    field_0x57c = fopAcM_isSwitch(this, swbit);
    return 1;
}

int daTagLightBall_c::draw() {
    return 1;
}

int daTagLightBall_c::_delete() {
    if (mInfSet) {
        dKy_dalkmist_inf_cut(&mInfluence);
        mInfSet = false;
    }

    return 1;
}

static int daTagLightBall_Draw(daTagLightBall_c* i_this) {
    return i_this->draw();
}

static int daTagLightBall_Execute(daTagLightBall_c* i_this) {
    return i_this->execute();
}

static int daTagLightBall_Delete(daTagLightBall_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daTagLightBall_Create(fopAc_ac_c* i_this) {
    daTagLightBall_c* lightBall = static_cast<daTagLightBall_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return lightBall->create();
}

static actor_method_class l_daTagLightBall_Method = {
    (process_method_func)daTagLightBall_Create,  (process_method_func)daTagLightBall_Delete,
    (process_method_func)daTagLightBall_Execute, (process_method_func)NULL,
    (process_method_func)daTagLightBall_Draw,
};

actor_process_profile_definition g_profile_Tag_LightBall = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_LightBall_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagLightBall_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_LightBall_e,
    /* Actor SubMtd */ &l_daTagLightBall_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
