#ifndef D_A_OBJ_TORNADO_H
#define D_A_OBJ_TORNADO_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjTrnd_c
 * @brief Wind Column
 *
 * @details
 *
 */
class daObjTrnd_c : public fopAc_ac_c {
public:
    void setPntWind();
    void cutPntWind();
    void movePntWind();
    void setCpsInfo();
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    int draw();
    int _delete();

    u32 checkSE() { return fopAcM_GetParamBit(this, 8, 1); }
    u8 getTimer() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwbit() { return fopAcM_GetParamBit(this, 0x18, 8); }

private:
    /* 0x568 */ dCcD_Stts mStts;
    /* 0x5A4 */ dCcD_Cps mCps;
    /* 0x6E8 */ cM3dGCpsS mWindCps;
    /* 0x704 */ f32 mNowLength;
    /* 0x708 */ Mtx mMtx;
    /* 0x738 */ s16 mTimer;
    /* 0x73C */ f32 mTargetLength;
    /* 0x740 */ WIND_INFLUENCE mWindInfluence;
    /* 0x76C */ f32 mWindPower;
};

STATIC_ASSERT(sizeof(daObjTrnd_c) == 0x770);

#endif /* D_A_OBJ_TORNADO_H */
