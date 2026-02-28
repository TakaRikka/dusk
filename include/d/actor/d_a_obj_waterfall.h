#ifndef D_A_OBJ_WATERFALL_H
#define D_A_OBJ_WATERFALL_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjWaterFall_c
 * @brief Waterfall Collision
 *
 * @details Collision added to waterfalls that player shouldn't move through (e.g. those in Zora's Domain). Doesn't actually draw the waterfall.
 */
class daObjWaterFall_c : public fopAc_ac_c {
public:
    void search_bomb();
    void search_arrow();
    void initBaseMtx();
    void setBaseMtx();
    cPhs_Step Create();
    cPhs_Step create();
    int execute();
    void push_player();
    int draw();
    int _delete();
private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 pad[4];
    /* 0x574 */ dCcD_Stts mCylColliderStts;
    /* 0x5B0 */ dCcD_Tri mUnusedTriCollider[2];
    /* 0x868 */ dCcD_Cyl mCylCollider;
    /* 0x9A4 */ cXyz mColOscPosTargets[2];
    /* 0x9BC */ cXyz mColCenter;
    /* 0x9C8 */ s8 mColOscDir;

    enum Type_e {
        ALLOW_ARROWS_e
    };

    BOOL checkFallOut() {
        return (BOOL) fopAcM_GetParamBit(this, 10, 4) == TRUE;
    }

    u8 getType() {
        return fopAcM_GetParamBit(this, 8, 2);
    }

    u8 getSwbit() {
        return fopAcM_GetParamBit(this, 0, 8);
    }
};

STATIC_ASSERT(sizeof(daObjWaterFall_c) == 0x9cc);


#endif /* D_A_OBJ_WATERFALL_H */
