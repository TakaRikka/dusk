#ifndef D_A_OBJ_WIND_STONE_H
#define D_A_OBJ_WIND_STONE_H

#include "d/d_bg_w.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daWindStone_c
 * @brief Howling Stone (Hole)
 *
 * @details
 *
 */
class daWindStone_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daWindStone_c();
    virtual ~daWindStone_c();
    int createHeap();
    int create();
    int execute();
    int draw();
    int Delete();
    void setModelMtx();
    void init();
    bool chkWlfInRange();
    bool chkEveOccur();
    void exeModeHowl();
    void exeModeMapDisp();
    bool chkMapDispMode();
    u8 getGoldWolfIdx();

    s8 getTuneId() { return fopAcM_GetParamBit(this, 4, 4); }
    int getNextSceneId() { return fopAcM_GetParamBit(this, 0, 4); }
    u32 getSwBit1() { return home.angle.x & 0xff; }
    u32 getSwBit2() { return fopAcM_GetParamBit(this, 8, 8); }
    u32 getDelEveFlgId() { return fopAcM_GetParamBit(this, 0x10, 0x10); }

private:
    /* 0x574 */ cBgS_PolyInfo mPolyInfo;
    /* 0x584 */ J3DModel* mpModel;
    /* 0x588 */ dBgW* mpBgW;
    /* 0x58C */ Mtx field_0x58c;
    /* 0x5BC */ f32 field_0x5bc;
    /* 0x5C0 */ u32 field_0x5c0;
    /* 0x5C4 */ u8 field_0x5c4;
};

STATIC_ASSERT(sizeof(daWindStone_c) == 0x5C8);

#endif /* D_A_OBJ_WIND_STONE_H */
