/**
 * d_a_tag_smk_emt.cpp
 * Tag - Smoke Emit
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_smk_emt.h"
#include "f_pc/f_pc_name.h"
#include <cstring>

daTagSmkEmt_c::daTagSmkEmt_c() {
    memset(&mParticles,0,0xc);
}

daTagSmkEmt_c::~daTagSmkEmt_c() {}

int daTagSmkEmt_c::create() {
    fopAcM_ct(this, daTagSmkEmt_c);
    createSmkEmt();
    return cPhs_COMPLEATE_e;
}

int daTagSmkEmt_c::Delete() {
    deleteSmkEmt();
    return 1;
}

int daTagSmkEmt_c::draw() {
    return 1;
}

int daTagSmkEmt_c::execute() {
    return 1;
}

void daTagSmkEmt_c::createSmkEmt() {
    static u16 const PARTICLE_NAME[3] = {
        dPa_RM(ID_ZF_S_L5CHIMSMK00),
        dPa_RM(ID_ZF_S_L5CHIMSMK01),
        dPa_RM(ID_ZF_S_L5CHIMSMK02),
    };

    cXyz pos(current.pos);
    csXyz angle(0,0,0);
    cXyz pos2(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 3; i++) {
        mParticles[i] = dComIfGp_particle_set(PARTICLE_NAME[i], &pos, &angle, &pos2);
    }
}

void daTagSmkEmt_c::deleteSmkEmt() {
    for (int i = 0; i < 3; i++) {
        if (mParticles[i]) {
            mParticles[i]->becomeInvalidEmitter();
            mParticles[i] = 0;
        }
    }
}

static int daTagSmkEmt_create(daTagSmkEmt_c* i_this) {
    fopAcM_ct(i_this, daTagSmkEmt_c);
    return i_this->create();
}

static int daTagSmkEmt_Delete(daTagSmkEmt_c* i_this) {
    return i_this->Delete();
}

static int daTagSmkEmt_execute(daTagSmkEmt_c* i_this) {
    return i_this->execute();
}

static int daTagSmkEmt_draw(daTagSmkEmt_c* i_this) {
    return i_this->draw();
}

static actor_method_class daTagSmkEmt_METHODS = {
    (process_method_func)daTagSmkEmt_create,
    (process_method_func)daTagSmkEmt_Delete,
    (process_method_func)daTagSmkEmt_execute,
    (process_method_func)0,
    (process_method_func)daTagSmkEmt_draw
};

actor_process_profile_definition g_profile_Tag_SmkEmt = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_SmkEmt_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagSmkEmt_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_SmkEmt_e,
    /* Actor SubMtd */ &daTagSmkEmt_METHODS,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
