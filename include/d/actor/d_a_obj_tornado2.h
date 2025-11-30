#ifndef D_A_OBJ_TORNADO2_H
#define D_A_OBJ_TORNADO2_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjTrnd2_c
 * @brief Strong Wind Column
 *
 * @details
 *
 */
class daObjTrnd2_c : public fopAc_ac_c {
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
    void setParticle();
    void endParticle();
    void startParticle();
    void stopParticle();
    int draw();
    int _delete();

    f32 getTargetLength() { return mTargetLength; }
    f32 getNowLength() { return mNowLength; }
    Vec& getWindEndPos() { return mWindCps.mEnd; }
    u32 getPower() { return fopAcM_GetParamBit(this, 8, 8); }
    u32 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u32 getSwType() { return fopAcM_GetParamBit(this, 16, 4); }
    u32 getOffTime() { return fopAcM_GetParamBit(this, 20, 4); }
    u32 getOnTime() { return fopAcM_GetParamBit(this, 24, 4); }
    u32 getScale() { return fopAcM_GetParamBit(this, 28, 1); }

    /* 0x568 */ dCcD_Stts mStts;
    /* 0x5A4 */ dCcD_Cps mCps;
    /* 0x6E8 */ cM3dGCpsS mWindCps;
    /* 0x704 */ f32 mNowLength;
    /* 0x708 */ Mtx mMtx;
    /* 0x738 */ f32 mTargetLength;
    /* 0x73C */ WIND_INFLUENCE mWindInfluence;
    /* 0x768 */ f32 mDefaultRadius;
    /* 0x76C */ f32 mDefaultLength;
    /* 0x770 */ int mTimer;
    /* 0x774 */ f32 mWindPower;
    /* 0x778 */ bool mColumnOn;
    /* 0x778 */ JPABaseEmitter* mParticles[3];
};

STATIC_ASSERT(sizeof(daObjTrnd2_c) == 0x788);

#endif /* D_A_OBJ_TORNADO2_H */
