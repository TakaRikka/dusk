#ifndef D_A_OBJ_EFF_H
#define D_A_OBJ_EFF_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_a_obj.h"

/**
 * @ingroup actors-objects
 * @namespace daObjEff
 * @brief Object Effect?
 *
 * @details
 *
 */
namespace daObjEff {
/**
 * @class Act_c
 * @brief Actor Object Effect?
 *
 * @details
 *
 */
class Act_c : public fopAc_ac_c {
public:
    enum Prm_e {
        PRM_0 = 0,
        PRM_8 = 8,
    };

    int _create();

    static void make_land_smoke(cXyz* i_pos, f32 i_scale) {
        cXyz scale(i_scale, i_scale, i_scale);
        scale *= 1.6666666f;
        fopAcM_create(PROC_Obj_Eff, 3, i_pos, -1, NULL, &scale, -1);
    }
};
};  // namespace daObjEff

#endif /* D_A_OBJ_EFF_H */
