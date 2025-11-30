#ifndef D_A_OBJ_GEYSER_H
#define D_A_OBJ_GEYSER_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjGeyser_c
 * @brief Geyser
 *
 * @details
 *
 */
class daObjGeyser_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void action();
    void actionOff2Init();
    void actionOff2();
    void actionOnWait2Init();
    void actionOnWait2();
    void actionOn2Init();
    void actionOn2();
    void actionDisappearInit(u8);
    void actionDisappear();
    void actionOffInit();
    void actionOff();
    void actionOnWaitInit();
    void actionOnWait();
    void actionOnInit();
    void actionOn();
    void setPntWind();
    void cutPntWind();
    void movePntWind();
    void setEmitterPre00();
    void setEmitterSrc00Sand();
    void setEmitterSrc01Smk();
    void stopEmitterPre00();
    void stopEmitterSrc00Sand();
    void stopEmitterSrc01Smk();
    void setEmitterClm00();
    void setEmitterSmk01();
    void setEmitterSmk02();
    void stopEmitterClm00();
    void stopEmitterSmk01();
    void stopEmitterSmk02();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getType() { return (field_0x766 >> 8) & 0xF; }
    u8 getArg0() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getArg1() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getArg2() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getArg3() { return fopAcM_GetParamBit(this, 24, 8); }
    u8 getSwNo() { return 0xFF; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dCcD_Stts mCcStts;
    /* 0x5E8 */ dCcD_Cps mCcCps;
    /* 0x72C */ u8 field_0x72C[0x730 - 0x72C];
    /* 0x730 */ f32 field_0x730;
    /* 0x734 */ u8 field_0x734[0x744 - 0x734];
    /* 0x744 */ cM3dGCpsS mCpsSrc;
    /* 0x760 */ u16 field_0x760;
    /* 0x762 */ u8 mAction;
    /* 0x763 */ u8 field_0x763;
    /* 0x764 */ u8 mIsPrmInit;
    /* 0x766 */ u16 field_0x766;
    /* 0x768 */ u16 field_0x768;
    /* 0x76C */ JPABaseEmitter* mpEmitterClm00;
    /* 0x770 */ JPABaseEmitter* mpEmitterSmk01;
    /* 0x774 */ JPABaseEmitter* mpEmitterSmk02;
    /* 0x778 */ JPABaseEmitter* mpEmitterPre00;
    /* 0x77C */ JPABaseEmitter* mpEmitterSrc00Sand;
    /* 0x780 */ JPABaseEmitter* mpEmitterSrc01Smk;
    /* 0x784 */ u8 field_0x784;
    /* 0x785 */ u8 field_0x785;
    /* 0x788 */ cXyz mParticleScale;
    /* 0x794 */ WIND_INFLUENCE mWind;
    /* 0x7C0 */ Mtx mMtx;
};

STATIC_ASSERT(sizeof(daObjGeyser_c) == 0x7f0);


#endif /* D_A_OBJ_GEYSER_H */
