#ifndef D_A_OBJ_RCIRCLE_H
#define D_A_OBJ_RCIRCLE_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjRCircle_c
 * @brief River Circle
 *
 * @details
 *
 */
class daObjRCircle_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daObjRCircle_c();
    virtual ~daObjRCircle_c();
    int createHeap();
    int create();
    int Delete();
    int draw();
    int execute();
    void init();
    void setModelMtx();

    int getSwBit() { return fopAcM_GetParamBit(this, 0, 8) & 0xFF; }

private:
    /* 0x574 */ mDoExt_brkAnm mBrk;
    /* 0x58C */ mDoExt_btkAnm mBtk;
    /* 0x5A4 */ J3DModel* mpModel;
    /* 0x5A8 */ u8 mSwitch;
};

STATIC_ASSERT(sizeof(daObjRCircle_c) == 0x5AC);

#endif /* D_A_OBJ_RCIRCLE_H */
