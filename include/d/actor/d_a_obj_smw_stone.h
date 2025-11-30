#ifndef D_A_OBJ_SMW_STONE_H
#define D_A_OBJ_SMW_STONE_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daSmWStone_c
 * @brief Howling Stone
 *
 * @details
 *
 */
class daSmWStone_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daSmWStone_c();
    virtual ~daSmWStone_c();
    cPhs__Step create();
    int execute();
    int draw();
    int Delete();
    void exeModeHowl();
    void setModelMtx();
    int createHeap();
    void init();
    bool chkWlfInRange();

    int getTuneId() { return 2; }
    u8 getSwBit0() { return fopAcM_GetParamBit(this, 0, 8); }
    void deleteStone() { mDelete = true; }

private:
    /* 0x574 */ cBgS_PolyInfo mPolyInfo;
    /* 0x584 */ J3DModel* mpModel;
    /* 0x588 */ dBgW* mpBgW;
    /* 0x58C */ Mtx mBgMtx;
    /* 0x5BC */ f32 mGroundH;
    /* 0x5C0 */ u32 mShadowKey;
    /* 0x5C4 */ u8 mMode;
    /* 0x5C5 */ bool mDelete;
};

STATIC_ASSERT(sizeof(daSmWStone_c) == 0x5c8);

#endif /* D_A_OBJ_SMW_STONE_H */
