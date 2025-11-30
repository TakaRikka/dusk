#ifndef D_A_OBJ_PROP_H
#define D_A_OBJ_PROP_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjProp_c
 * @brief City in the Sky Propeller?
 *
 * @details
 *
 */
class daObjProp_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daObjProp_c();
    virtual ~daObjProp_c();
    bool createHeap();
    int create();
    int Delete();
    int draw();
    int execute();
    void init();
    void setModelMtx();

    u32 getNameArg_0() { return fopAcM_GetParamBit(this, 0, 4); }
    u32 getArg_0() { return fopAcM_GetParamBit(this, 4, 4); }

private:
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ u8 mArg;
};

STATIC_ASSERT(sizeof(daObjProp_c) == 0x57C);

#endif /* D_A_OBJ_PROP_H */
