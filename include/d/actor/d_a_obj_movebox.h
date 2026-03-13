#ifndef D_A_OBJ_MOVEBOX_H
#define D_A_OBJ_MOVEBOX_H

#include "d/d_a_obj.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"


/**
 * @ingroup actors-objects
 * @class daObjMovebox
 * @brief Multi-Purpose Moving Box
 *
 * @details Multi-purpose moving box actor.
 * Used for: Goron Mines Push Block, Death Mountain Block,
 *      Large Wooden Crate, Sacred Grove Block,
 *      Arbiter's Grounds Block (2 of them), Kakariko Graveyard Gravestone
 *
 */
namespace daObjMovebox {

enum Type_e { pp_field = 3 };

struct Attr_c {
    struct sMinMax3 { // fabricated
        s16 xMin;
        s16 xMax;
        s16 yMin;
        s16 yMax;
        s16 zMin;
        s16 zMax;
    };
    /* 0x00 */ s16 mFirstPushStopTime;
    /* 0x02 */ s16 mRepeatPushStopTime;
    /* 0x04 */ s16 mPushMoveTime;
    /* 0x06 */ s16 mFirstPullStopTime;
    /* 0x08 */ s16 mRepeatPullStopTime;
    /* 0x0A */ s16 mPullMoveTime;
    /* 0x0C */ f32 mUnitDistance;
    /* 0x10 */ f32 mShadowSize;
    /* 0x14 */ f32 mGravity;
    /* 0x18 */ f32 mViscosityResistance;
    /* 0x1C */ f32 mInertiaResistance;
    /* 0x20 */ f32 mFallYInitSpeed;
    /* 0x24 */ f32 mLandSmokeScale;
    /* 0x28 */ f32 mBuoyancy;
    /* 0x2C */ f32 mPlayerGravity;
    /* 0x30 */ f32 mMiscGravity;
    /* 0x34 */ f32 mWaterOscillationAccel;
    /* 0x38 */ s16 mWaterOscillationAngleSpeed;
    /* 0x3C */ f32 mWaterViscoscityResistance;
    /* 0x40 */ f32 mWaterInertiaResistance;
    /* 0x44 */ f32 mPlayerTiltPower;
    /* 0x48 */ f32 mMiscTiltPower;
    /* 0x4C */ f32 mMaxTiltPower;
    /* 0x50 */ f32 mTiltSpringFactor;
    /* 0x54 */ f32 mTiltViscoscityResistance;
    /* 0x58 */ int mModelResIdx;
    /* 0x5C */ int mMoveBGIdx;
    /* 0x60 */ u32 mHeapSize;
    /* 0x64 */ f32 mScaleY;
    /* 0x68 */ f32 field_0x68;
    /* 0x6C */ f32 mWaterBobScale; // always the reciprocal of field_0x68
    /* 0x70 */ f32 mScaleXZ;
    /* 0x74 */ f32 mTiltForceScale; // always the reciprocal of mScaleXZ
    /* 0x78 */ f32 field_0x78;
    /* 0x7C */ Z2SoundID mSoundSlip;
    /* 0x80 */ Z2SoundID mSoundLimit;
    /* 0x84 */ Z2SoundID mSoundLand;
    /* 0x88 */ Z2SoundID mSoundFallWater;
    /* 0x8C */ Z2SoundID mSoundFallMagma;
    /* 0x90 */ sMinMax3 mCull;
    /* 0x9C */ bool field_0x9c;
    /* 0x9D */ bool mDisableShadow;
    /* 0x9E */ bool mUseLargeGrid;
};

struct Hio_c : JORReflexible {
    Hio_c(daObjMovebox::Type_e);
    void default_set();
    void ct();
    void dt();
    void genMessage(JORMContext*);

    static u8 M_flag_bgc[8];
    static const char* const M_name[];

    /* 0x4 */ Type_e mType;
    /* 0x8 */ int mCount;
    /* 0xC */ daObjMovebox::Attr_c mAttr;
};

struct BgcSrc_c {
    /* 0x0 */ f32 field_0x0;
    /* 0x4 */ f32 field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xC */ f32 field_0xc;
};

class Act_c;

struct Bgc_c {
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
    void gnd_pos(daObjMovebox::Act_c const*, daObjMovebox::BgcSrc_c const*, int, f32);
    void wrt_pos(cXyz const&);
    void wall_pos(daObjMovebox::Act_c const*, daObjMovebox::BgcSrc_c const*, int, s16, f32);
    void proc_vertical(daObjMovebox::Act_c*);
    bool chk_wall_pre(daObjMovebox::Act_c const*, daObjMovebox::BgcSrc_c const*, int, s16);
    bool chk_wall_touch(daObjMovebox::Act_c const*, daObjMovebox::BgcSrc_c const*, s16);
    bool chk_wall_touch2(daObjMovebox::Act_c const*, daObjMovebox::BgcSrc_c const*, int, s16);

    static const daObjMovebox::BgcSrc_c M_lin5[];
    static const daObjMovebox::BgcSrc_c M_lin20[];

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
#if DEBUG
    /* 0x184 */ cXyz field_0x184[92];
    /* 0x184 */ cXyz field_0x5D4[92];
    /* 0xA24 */ int field_0xA24;
    /* 0xA28 */ int field_0xA28;
#endif
};

struct Act_c : public dBgS_MoveBgActor {
    enum Prm_e {
        PRM_4_e = 0x4,
        PRM_8_e = 0x8,
        PRM_18_e = 0x18,
        PRM_1C_e = 0x1C,
    };

    int prm_get_swSave1() const;
    void prmZ_init();
    void prmX_init();
    // it seems like this signature changed between GCN and Wii?
    // GCN only matches with a const reference return type and Wii/Shield only match with non-const
#if PLATFORM_GCN
    const daObjMovebox::Attr_c& attr() const;
#else
    daObjMovebox::Attr_c& attr() const;
#endif
    void set_mtx();
    void init_mtx();
    void path_init();
    void path_save();
    static void RideCallBack(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
    static fopAc_ac_c* PPCallBack(fopAc_ac_c*, fopAc_ac_c*, s16, dBgW_Base::PushPullLabel);
    void afl_sway();
    int check_to_walk();
    void clr_moment_cnt();
    bool chk_appear() const;
    void eff_smoke_slip_start();
    void mode_wait_init();
    void mode_wait();
    void mode_walk_init();
    void mode_walk();
    void mode_afl_init();
    void mode_afl();
    void sound_slip();
    void sound_limit();
    void sound_land();
    void vib_land();
    void eff_land_smoke();
    void eff_break();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    inline void mode_proc_call();
    inline int Mthd_Create();
    inline int Mthd_Delete();

    int prm_get_swSave() const { return daObj::PrmAbstract(this, PRM_8_e, PRM_8_e); }
    int prmZ_get_swSave2() const { return (u8)(mPrmZ >> 8); }
    int prm_get_dmy() const { return 0; }
    int prmZ_get_pathId() const { return mPrmZ & 0xFF; }
    u8 prm_get_answer() const { return daObj::PrmAbstract(this, PRM_4_e, PRM_1C_e); }
    int prm_get_type() const { return daObj::PrmAbstract(this, PRM_4_e, PRM_18_e); }

    bool is_switch1() const { return fopAcM_isSwitch(this, prm_get_swSave1()); }
    int is_switch2() const { return fopAcM_isSwitch(this, prmZ_get_swSave2()) != 0; }

    void on_switch1() const { fopAcM_onSwitch(this, prm_get_swSave1()); }
    void on_switch2() const { fopAcM_onSwitch(this, prmZ_get_swSave2()); }
    void off_switch1() const { fopAcM_offSwitch(this, prm_get_swSave1()); }
    void off_switch2() const { fopAcM_offSwitch(this, prmZ_get_swSave2()); }

    int getType() { return prm_get_type(); }

    static const s16 M_dir_base[4];
    static const char* const M_arcname[8];
    static const dCcD_SrcCyl M_cyl_src;

    static const daObjMovebox::Attr_c M_attr[8];

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ int field_0x5ac;
    /* 0x5B0 */ dCcD_Stts mCcStts;
    /* 0x5EC */ dCcD_Cyl mCcCyl;
    /* 0x728 */ Bgc_c mBgc;
    /* 0x8AC */ int mType;
    /* 0x8B0 */ u16 mPrmZ;
    /* 0x8B2 */ u16 mPrmX;
    /* 0x8B4 */ dPath* mpPath;
    /* 0x8B8 */ s16 field_0x8b8;
    /* 0x8BC */ f32 field_0x8bc;
    /* 0x8C0 */ f32 field_0x8c0;
    /* 0x8C4 */ f32 field_0x8c4;
    /* 0x8C8 */ f32 field_0x8c8;
    /* 0x8CC */ f32 field_0x8cc;
    /* 0x8D0 */ f32 field_0x8d0;
    /* 0x8D4 */ f32 field_0x8d4;
    /* 0x8D8 */ f32 field_0x8d8;
    /* 0x8DC */ int field_0x8dc;
    /* 0x8E0 */ int field_0x8e0;
    /* 0x8E4 */ f32 field_0x8e4;
    /* 0x8E8 */ int field_0x8e8;
    /* 0x8EC */ dBgW::PushPullLabel mPPLabel;
    /* 0x8F0 */ s16 field_0x8f0[4];
    /* 0x8F8 */ s16 field_0x8f8;
    /* 0x8FA */ s16 field_0x8fa;
    /* 0x8FC */ s16 field_0x8fc;
    /* 0x8FE */ u8 field_0x8fe;
    /* 0x8FF */ s8 mReverb;
    /* 0x900 */ u8 field_0x900;
    /* 0x901 */ u8 field_0x901;
    /* 0x902 */ u8 field_0x902;
    /* 0x904 */ int field_0x904;
    /* 0x908 */ s8 field_0x908;
#if DEBUG
    static daObjMovebox::Hio_c M_hio[];
#endif
};
};  // namespace daObjMovebox

STATIC_ASSERT(sizeof(daObjMovebox::Act_c) == 0x90C);

#endif /* D_A_OBJ_MOVEBOX_H */
