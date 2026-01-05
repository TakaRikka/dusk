/**
 * d_a_obj_eff.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_eff.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"


int daObjEff::Act_c::_create() {
    fopAcM_ct(this, Act_c);

    if (daObj::PrmAbstract<Act_c::Prm_e>(this, PRM_8, PRM_0) == 3) {
        dComIfGp_particle_setColor(0x00E7, &current.pos, NULL, NULL, NULL, 0.0f, 55, NULL, &scale,
                                   NULL, fopAcM_GetRoomNo(this), &scale);
    }

    return cPhs_ERROR_e;
}

namespace daObjEff {
namespace {
int Mthd_Create(void* i_this) {
    return static_cast<daObjEff::Act_c*>(i_this)->_create();
}

int Mthd_Delete(void* i_this) {
    return 1;
}

int Mthd_Execute(void* i_this) {
    return 1;
}

int Mthd_Draw(void* i_this) {
    return 1;
}

int Mthd_IsDelete(void* i_this) {
    return 1;
}
};  // namespace
};  // namespace daObjEff

namespace daObjEff {
namespace {
static actor_method_class Mthd_Table = {
    (process_method_func)Mthd_Create,  (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute, (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
};
};  // namespace daObjEff

actor_process_profile_definition g_profile_Obj_Eff = {
    fpcLy_CURRENT_e,
    9,
    fpcPi_CURRENT_e,
    PROC_Obj_Eff,
    &g_fpcLf_Method.base,
    sizeof(daObjEff::Act_c),
    0,
    0,
    &g_fopAc_Method.base,
    267,
    &daObjEff::Mthd_Table,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
