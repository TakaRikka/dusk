#ifndef D_A_OBJ_HAKAI_BRL_H
#define D_A_OBJ_HAKAI_BRL_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjHBarrel_c
 * @brief Destructable Barrel
 *
 * @details
 *
 */
class daObjHBarrel_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daObjHBarrel_c();
    virtual ~daObjHBarrel_c();
    int createHeap();
    int create();
    int Delete();
    int draw();
    int execute();
    void init();
    void setModelMtx();
    void hitAction();
    u8 getItemBit() {
        return fopAcM_GetParamBit(this, 8, 8);
    }
    u8 getItemNo() {
        return fopAcM_GetParamBit(this, 0, 8);
    }
    void callEmt();

    static dCcD_SrcCyl const s_CcDCyl;

private:
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dCcD_Stts mStts;
    /* 0x5B4 */ dCcD_Cyl mCyl;
    /* 0x6F0 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(daObjHBarrel_c) == 0x710);


#endif /* D_A_OBJ_HAKAI_BRL_H */
