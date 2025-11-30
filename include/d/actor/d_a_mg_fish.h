#ifndef D_A_MG_FISH_H
#define D_A_MG_FISH_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-unsorted
 * @class mf_ke_s
 * @brief Fishing line ("keburu")
 *
 * @details
 */
struct mf_ke_s {
    /* 0x00 */ cXyz mControlPoints[5];
    /* 0x3C */ cXyz mMovementDir;
};

/**
 * @ingroup actors-unsorted
 * @class mg_fish_class
 * @brief Fish
 *
 * @details
 *
 */
class mg_fish_class {
public:
    /* 0x0   */ fopEn_enemy_c actor;
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ s16 mBobTimer;
    /* 0x5B6 */ s16 mCurAction;
    /* 0x5B8 */ s16 mActionPhase;
    /* 0x5BC */ f32 mJointScale;
    /* 0x5C0 */ cXyz mNextPos;
    /* 0x5CC */ s16 mMovementYaw;
    /* 0x5CE */ s16 mMovementPitch;
    /* 0x5D0 */ s16 mMaxStep;
    /* 0x5D2 */ s16 mCurSwimStep;
    /* 0x5D4 */ f32 field_0x5d4;
    /* 0x5D8 */ f32 mSurfaceY;
    /* 0x5DC */ f32 mGroundY;
    /* 0x5E0 */ cXyz field_0x5e0;
    /* 0x5EC */ f32 field_0x5ec;
    /* 0x5F0 */ f32 mDistToPlayer;
    /* 0x5F4 */ s16 mYawToPlayer;
    /* 0x5F6 */ s16 field_0x5f6;
    /* 0x5F8 */ s16 field_0x5f8;
    /* 0x5FC */ char* mResName;
    /* 0x600 */ mDoExt_McaMorf* mpMorf;
    /* 0x604 */ J3DAnmTransform* mAnmTransform;
    /* 0x608 */ u32 mShadowId;
    /* 0x60C */ int mAnmID;
    /* 0x610 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x614 */ J3DModel* mpModel[3];
    /* 0x620 */ mDoExt_btpAnm* mpBtpAnm;
    /* 0x624 */ s16 field_0x624[5];
    /* 0x62E */ s16 field_0x62e;
    /* 0x630 */ s16 mRemainingHookTime;
    /* 0x632 */ s16 field_0x632;
    /* 0x634 */ u8 mGedouKind;
    /* 0x635 */ u8 mKind2;
    /* 0x638 */ cXyz field_0x638;
    /* 0x644 */ s16 mRotZ;
    /* 0x648 */ u32 mBaitId;
    /* 0x64C */ u32 mRodId;
    /* 0x650 */ f32 field_0x650;
    /* 0x654 */ f32 field_0x654;
    /* 0x658 */ s8 mHookedState;
    /* 0x659 */ s8 field_0x659;
    /* 0x65A */ s8 field_0x65a;
    /* 0x65B */ u8 field_0x65b;
    /* 0x65C */ u8 field_0x65c;
    /* 0x65D */ u8 field_0x65d;
    /* 0x660 */ f32 field_0x660;
    /* 0x664 */ cXyz mJointTranslations[10];
    /* 0x6DC */ csXyz mJointRotations[10];
    /* 0x718 */ s16 jointYaws1[10];
    /* 0x72C */ s32 mNumJoints;
    /* 0x730 */ s16 jointYaws2[6];
    /* 0x73C */ f32 field_0x73c;
    /* 0x740 */ f32 field_0x740;
    /* 0x744 */ f32 mBobAmp;
    /* 0x748 */ f32 field_0x748;
    /* 0x74C */ f32 field_0x74c;
    /* 0x750 */ u8 field_0x750;
    /* 0x754 */ mf_ke_s field_0x754[2];
    /* 0x7E4 */ mDoExt_3DlineMat0_c mLineMat;
    /* 0x800 */ Z2Creature mSound;
    /* 0x890 */ dBgS_AcchCir mAcchCir;
    /* 0x8D0 */ dBgS_ObjAcch mAcch;
    /* 0xAA8 */ dCcD_Stts mCcStatus;
    /* 0xAE4 */ dCcD_Cyl mCcCyl;
    /* 0xC20 */ u32 field_0xc20;
    /* 0xC24 */ u8 field_0xc24[0xc28 - 0xc24];
    /* 0xC28 */ s32 mEmitterIds[3];
    /* 0xC34 */ u32 field_0xc34;
    /* 0xC38 */ u32 field_0xc38;
    /* 0xC3C */ u8 field_0xc3c;
    /* 0xC3D */ u8 mCaughtType;
    /* 0xC3E */ u8 field_0xc3e;
    /* 0xC3F */ u8 field_0xc3f;
    /* 0xC40 */ f32 field_0xc40;
    /* 0xC44 */ u8 field_0xc44;
    /* 0xC48 */ J3DJointCallBack mNodeCallBack;
    /* 0xC4C */ u32 mSkullfishId;
};

STATIC_ASSERT(sizeof(mg_fish_class) == 0xC50);

class daMg_Fish_HIO_c {
public:
    daMg_Fish_HIO_c();
    virtual ~daMg_Fish_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xC */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u8 field_0x1a;
    /* 0x1B */ u8 field_0x1b;
};


#endif /* D_A_MG_FISH_H */
