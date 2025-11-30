#ifndef D_A_OBJ_LV6BEMOS2_H
#define D_A_OBJ_LV6BEMOS2_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjLv6Bm_c
 * @brief Temple of Time Beamos
 *
 * @details
 *
 */
class daObjLv6Bm_c : public dBgS_MoveBgActor {
public:
    typedef void (daObjLv6Bm_c::*actionFunc)();
    typedef void (daObjLv6Bm_c::*effectFunc)();

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    cPhs__Step create1st();
    int Execute(Mtx**);
    void action();
    void calcBeam();
    int checkFindPlayer();
    s8 checkSearchPlayer();
    f32 getSearchDistance();
    f32 getBeamSearchDistance();
    void effect_proc();
    void effectWait();
    void initEffectSet0();
    void effectSet0();
    void initActionSwWait();
    void actionSwWait();
    void initActionWarning();
    void actionWarning();
    void initActionFindPlayer();
    void actionFindPlayer();
    void initActionAttack();
    void actionAttack();
    void initActionDead();
    void actionDead();
    void initActionEnd();
    void actionEnd();
    int Draw();
    int Delete();

    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwNo2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwNo3() { return fopAcM_GetParamBit(this, 16, 8); }

    s16 getHeadJoint() { return mJoints[0]; }
    s16 getBigGearJoint() { return mJoints[1]; }
    s16 getSmallGear0Joint() { return mJoints[2]; }
    s16 getSmallGear1Joint() { return mJoints[3]; }
    s16 getSmallGear2Joint() { return mJoints[4]; }

    u8 getSearchDist() { return fopAcM_GetParamBit(this, 0x1C, 4); }
    u16 getBeamSearchDist() { return field_0xa32 & 15; }

    u8 checkLockOnCamera() { return (field_0xa32 & 0x8000) >> 15; }

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mBeamosModel;
    /* 0x5AC */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x5B0 */ mDoExt_bckAnm* mpBckAnm;
    /* 0x5B4 */ J3DModel* mBeamEffectModel;
    /* 0x5B8 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x5BC */ mDoExt_btkAnm* mpBtkAnm2;
    /* 0x5C0 */ mDoExt_bckAnm* mpBckAnm2;
    /* 0x5C4 */ dCcD_Stts mStts;
    /* 0x600 */ dCcD_Sph field_0x600;
    /* 0x738 */ dCcD_Cps field_0x738;
    /* 0x87C */ dCcD_Cyl field_0x87c;
    /* 0x9B8 */ s16 mJoints[5];
    /* 0x9C2 */ s16 field_0x9c2;
    /* 0x9C4 */ s16 field_0x9c4;
    /* 0x9C6 */ s16 field_0x9c6;
    /* 0x9C8 */ s16 field_0x9c8;
    /* 0x9CA */ s16 field_0x9ca;
    /* 0x9CC */ s16 field_0x9cc;
    /* 0x9CE */ s16 field_0x9ce;
    /* 0x9D0 */ s16 field_0x9d0;
    /* 0x9D2 */ s16 field_0x9d2;
    /* 0x9D4 */ s16 field_0x9d4;
    /* 0x9D6 */ s16 field_0x9d6;
    /* 0x9D8 */ s16 field_0x9d8;
    /* 0x9DA */ s16 field_0x9da;
    /* 0x9DC */ s16 field_0x9dc;
    /* 0x9DE */ s16 field_0x9de;
    /* 0x9E0 */ J3DMaterial* mEyeMaterial;
    /* 0x9E4 */ cXyz field_0x9e4;
    /* 0x9F0 */ csXyz field_0x9f0;
    /* 0x9F8 */ cXyz field_0x9f8;
    /* 0xA04 */ u8 field_0xa04[0xa0c - 0xa04];
    /* 0xA0C */ f32 field_0xa0c;
    /* 0xA10 */ u8 field_0xa10;
    /* 0xA11 */ u8 field_0xa11;
    /* 0xA12 */ u8 field_0xa12;
    /* 0xA13 */ u8 field_0xa13;
    /* 0xA14 */ s8 field_0xa14;
    /* 0xA15 */ u8 field_0xa15;
    /* 0xA16 */ u8 mMode;
    /* 0xA17 */ u8 field_0xa17;
    /* 0xA18 */ u8 field_0xa18;
    /* 0xA19 */ s8 field_0xa19;
    /* 0xA1A */ u8 field_0xa1a;
    /* 0xA1B */ u8 field_0xa1b;
    /* 0xA1C */ u8 field_0xa1c;
    /* 0xA20 */ int field_0xa20;
    /* 0XA24 */ JPABaseEmitter* field_0xa24;
    /* 0xA28 */ f32 field_0xa28;
    /* 0xA2C */ f32 field_0xa2c;
    /* 0xA30 */ u16 field_0xa30;
    /* 0xA32 */ u16 field_0xa32;
    /* 0xA34 */ s16 field_0xa34;
    /* 0xA36 */ u8 field_0xa36;
    /* 0xA37 */ u8 field_0xa37;
    /* 0xA38 */ JPABaseEmitter* field_0xa38[2];
    /* 0xA40 */ JPABaseEmitter* field_0xa40[4];
    /* 0xA50 */ JPABaseEmitter* field_0xa50;
    /* 0xA54 */ JPABaseEmitter* field_0xa54;
    /* 0xA58 */ cXyz field_0xa58;
    /* 0xA64 */ cXyz field_0xa64;
    /* 0xA70 */ cXyz field_0xa70;
    /* 0xA7C */ Z2SoundObjSimple field_0xa7c;
    /* 0xA9C */ Z2SoundObjSimple field_0xa9c;
};

STATIC_ASSERT(sizeof(daObjLv6Bm_c) == 0xabc);


#endif /* D_A_OBJ_LV6BEMOS2_H */
