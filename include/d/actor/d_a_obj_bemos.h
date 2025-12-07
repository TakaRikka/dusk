#ifndef D_A_OBJ_BEMOS_H
#define D_A_OBJ_BEMOS_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "JSystem/JParticle/JPAEmitter.h"

/**
 * @ingroup actors-objects
 * @class daObjBm_c
 * @brief Beamos (Movable Object)
 *
 * @details
 *
 */
class daObjBm_c : public dBgS_MoveBgActor {
public:
    typedef void (daObjBm_c::*procFunc)();
    typedef void (daObjBm_c::*modeFunc)();
    typedef void (daObjBm_c::*effectFunc)();

    class BgcSrc_c {
    public:
        /* 0x0 */ f32 field_0x0;
        /* 0x4 */ f32 field_0x4;
        /* 0x8 */ f32 field_0x8;
        /* 0xC */ f32 field_0xc;
    };

    class Bgc_c {
    public:
        enum State_e {
            STATE_0_e = 0x0,
            STATE_1_e = 0x1,
            STATE_2_e = 0x2,
            STATE_4_e = 0x4,
            STATE_8_e = 0x8,
            STATE_10_e = 0x10,
            STATE_20_e = 0x20,
            STATE_40_e = 0x40,
        };

        Bgc_c();
        void wall_pos(fopAc_ac_c const*, daObjBm_c::BgcSrc_c const*, int, s16, f32);
        bool chk_wall_pre(fopAc_ac_c const*, daObjBm_c::BgcSrc_c const*, int, s16);

        static daObjBm_c::BgcSrc_c M_lin5[];
        static const daObjBm_c::BgcSrc_c M_lin20[];

        static dBgS_ObjGndChk M_gnd_work[23];
        static dBgS_WtrChk M_wrt_work;
        static dBgS_ObjLinChk M_wall_work[23];

        /* 0x000 */ f32 field_0x0[23];
        /* 0x05C */ int field_0x5c;
        /* 0x060 */ f32 field_0x60;
        /* 0x064 */ cXyz field_0x64[23];
        /* 0x178 */ int field_0x178;
        /* 0x17C */ f32 field_0x17c;
        /* 0x180 */ State_e mState;
    };

    u8 getMoveType() { return fopAcM_GetParamBit(this, 24, 4); }
    s16 getHeadJoint() { return mJoints[0]; }
    s16 getBigGearJoint() { return mJoints[1]; }
    s16 getSmallGear0Joint() { return mJoints[2]; }
    s16 getSmallGear1Joint() { return mJoints[3]; }
    s16 getSmallGear2Joint() { return mJoints[4]; }
    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwNo2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwNo3() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getSearchDist() { return fopAcM_GetParamBit(this, 28, 4); }
    u8 getBeamSearchDist() { return field_0x100a & 15; }
    u8 checkLockOnCamera() { return (field_0x100a & 0x8000) >> 15; }

    static fopAc_ac_c* PPCallBack(fopAc_ac_c*, fopAc_ac_c*, s16,
                                                 dBgW_Base::PushPullLabel);
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void main_proc_call();
    void initActionEnBemos();
    void initActionObjBemos();
    void actionEnBemos();
    void actionObjBemos();
    void setCrawCO();
    void calcBeamPos();
    void calcBeamLenAndAt();
    s8 checkFindPlayer();
    s8 checkSearchPlayer();
    f32 getSearchDistance();
    f32 getBeamSearchDistance();
    void effect_proc();
    void effectWait();
    void initEffectSet0();
    void effectSet0();
    void effectSet1();
    void effectEnd();
    int check_to_walk();
    void mode_wait_init();
    void mode_wait();
    void mode_walk_init();
    void mode_walk();
    void mode_afl();
    void mode_dead_init();
    void mode_dead();
    void clr_moment_cnt();
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
    int Draw();
#if DEBUG
    /* 0x000000 */ void debugDraw();
#endif
    int Delete();

    static s16 const M_dir_base[4];
    // private:
    /* 0x05A0 */ request_of_phase_process_class mPhase;
    /* 0x05A8 */ J3DModel* mpModel;
    /* 0x05AC */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x05B0 */ mDoExt_bckAnm* mpBckAnm;
    /* 0x05B4 */ J3DModel* mpModel2;
    /* 0x05B8 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x05BC */ mDoExt_btkAnm* mpBtkAnm2;
    /* 0x05C0 */ mDoExt_bckAnm* mpBckAnm2;
    /* 0x05C4 */ J3DModel* mpModel3;
    /* 0x05C8 */ mDoExt_bckAnm* mpBckAnm3;
    /* 0x05CC */ dBgS_ObjAcch mAcch;
    /* 0x07A4 */ dBgS_AcchCir mAcchCir;
    /* 0x07E4 */ dCcD_Stts mStts;
    /* 0x0820 */ dCcD_Sph mSph;
    /* 0x0958 */ dCcD_Cps mCps;
    /* 0x0A9C */ dCcD_Cyl mCyl[4];
    /* 0x0F8C */ s16 mJoints[5];
    /* 0x0F96 */ s16 field_0xf96;
    /* 0x0F98 */ s16 field_0xf98;
    /* 0x0F9A */ s16 field_0xf9a;
    /* 0x0F9C */ s16 field_0xf9c;
    /* 0x0F9E */ s16 field_0xf9e;
    /* 0x0FA0 */ s16 field_0xfa0;
    /* 0x0FA2 */ s16 field_0xfa2;
    /* 0x0FA4 */ s16 field_0xfa4;
    /* 0x0FA6 */ s16 field_0xfa6;
    /* 0x0FA8 */ s16 field_0xfa8;
    /* 0x0FAA */ s16 field_0xfaa;
    /* 0x0FAC */ s16 field_0xfac;
    /* 0x0FAE */ s16 field_0xfae;
    /* 0x0FB0 */ s16 field_0xfb0;
    /* 0x0FB2 */ s16 field_0xfb2;
    /* 0x0FB4 */ J3DMaterial* mpMaterial;
    /* 0x0FB8 */ cXyz field_0xfb8;
    /* 0x0FC4 */ csXyz field_0xfc4;
    /* 0x0FCA */ u8 field_0xfca[2];
    /* 0x0FCC */ cXyz field_0xfcc;
    /* 0x0FD8 */ cXyz field_0xfd8;
    /* 0x0FE4 */ u8 field_0xfe4;
    /* 0x0FE5 */ u8 field_0xfe5;
    /* 0x0FE6 */ u8 field_0xfe6;
    /* 0x0FE7 */ s8 field_0xfe7;
    /* 0x0FE8 */ u8 field_0xfe8;
    /* 0x0FE9 */ u8 field_0xfe9;
    /* 0x0FEA */ u8 field_0xfea;
    /* 0x0FEB */ u8 field_0xfeb;
    /* 0x0FEC */ u8 field_0xfec;
    /* 0x0FED */ u8 field_0xfed;
    /* 0x0FF0 */ u32 field_0xff0;
    /* 0x0FF4 */ u8 field_0xff4;
    /* 0x0FF5 */ u8 field_0xff5;
    /* 0x0FF6 */ u8 field_0xff6;
    /* 0x0FF7 */ u8 field_0xff7;
    /* 0x0FF8 */ JPABaseEmitter* field_0xff8[2];
    /* 0x1000 */ f32 field_0x1000;
    /* 0x1004 */ f32 field_0x1004;
    /* 0x1008 */ u16 field_0x1008;
    /* 0x100A */ u16 field_0x100a;
    /* 0x100C */ u16 field_0x100c;
    /* 0x100E */ u8 field_0x100e;
    /* 0x100F */ u8 field_0x100f;
    /* 0x1010 */ JPABaseEmitter* field_0x1010[2];
    /* 0x1018 */ JPABaseEmitter* field_0x1018[2];
    /* 0x1010 */ JPABaseEmitter* field_0x1020[3];
    /* 0x102C */ JPABaseEmitter* field_0x102c;
    /* 0x1030 */ JPABaseEmitter* field_0x1030;
    /* 0x1034 */ JPABaseEmitter* field_0x1034;
    /* 0x1038 */ s8 field_0x1038;
    /* 0x1039 */ s8 field_0x1039;
    /* 0x103C */ cXyz field_0x103c;
    /* 0x1048 */ cXyz field_0x1048;
    /* 0x1054 */ cXyz field_0x1054;
    /* 0x1060 */ Z2SoundObjSimple mSound;
    /* 0x1080 */ Z2SoundObjSimple mSound2;
    /* 0x10A0 */ cXyz field_0x10a0;
    /* 0x10AC */ int field_0x10ac;
    /* 0x10B0 */ int field_0x10b0;
    /* 0x10B4 */ int field_0x10b4;
    /* 0x10B8 */ f32 field_0x10b8;
    /* 0x10BC */ int field_0x10bc;
    /* 0x10C0 */ dBgW::PushPullLabel mPPLabel;
    /* 0x10C4 */ s16 mMomentCnt[4];
    /* 0x10CC */ s16 field_0x10cc;
    /* 0x10CE */ s16 field_0x10ce;
    /* 0x10D0 */ s16 field_0x10d0;
    /* 0x10D2 */ u8 field_0x10d2;
    /* 0x10D3 */ u8 field_0x10d3;
    /* 0x10D4 */ Bgc_c mBgc;
};

STATIC_ASSERT(sizeof(daObjBm_c) == 0x1258);

#endif /* D_A_OBJ_BEMOS_H */
