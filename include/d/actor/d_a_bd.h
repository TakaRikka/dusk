#ifndef D_A_BD_H
#define D_A_BD_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class bd_class
 * @brief Bird
 *
 * @details Used by birds that sit on Coro's head.
 *
 */

enum da_BD_ANM {
    ANM_FLY = 5,
    ANM_KAKKU,
    ANM_KAKKU1,
    ANM_PITA_DOWN,
    ANM_PITA_LEFTUP,
    ANM_PITA_RIGHT,
    ANM_PYON,
};

enum Action_bd {
    ACT_GROUND = 0,
    ACT_FLY = 1,
    ACT_DROP = 2,
    ACT_LANDING = 3,
    ACT_LANDING2 = 4,
    ACT_LANDING3 = 5,
    ACT_ROPE = 6,
    ACT_KKRI = 7,
};

class bd_class {
public:
    /* 0x000 */ fopEn_enemy_c enemy;
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 field_0x5B4;
    /* 0x5B5 */ u8 field_0x5B5;
    /* 0x5B6 */ u8 field_0x5B6;
    /* 0x5B7 */ u8 field_0x5B7;
    /* 0x5B8 */ u8 field_0x5B8;
    /* 0x5B9 */ u8 field_0x5B9;
    /* 0x5B8 */ char pad5B8[0x8];           // Padding
    /* 0x5C2 */ s8 field_0x5C2;
    /* 0x5C4 */ cXyz field_0x5C4;
    /* 0x5D0 */ s16 mTargetAngleY;
    /* 0x5D4 */ actor_place* field_0x5D4;
    /* 0x5D8 */ s16 field_0x5D8;
    /* 0x5DC */ f32 field_0x5DC;
    /* 0x5E0 */ f32 field_0x5E0;
    /* 0x5E4 */ mDoExt_McaMorf* mpMorf;
    /* 0x5E8 */ mDoExt_btkAnm* mpBtk;
    /* 0x5EC */ s8 field_0x5EC;
    /* 0x5F0 */ s32 mAnmID;
    /* 0x5F4 */ s16 mChirpDist;
    /* 0x5F8 */ Z2SoundObjSimple mSound;
    /* 0x618 */ s16 field_0x618;
    /* 0x61A */ s16 mActionID;
    /* 0x61C */ s16 field_0x61C;
    /* 0x61E */ s16 field_0x61E;
    /* 0x620 */ f32 field_0x620;
    /* 0x624 */ f32 field_0x624;
    /* 0x628 */ s16 field_0x628;
    /* 0x62C */ cXyz* field_0x62C;
    /* 0x630 */ int field_0x630;
    /* 0x634 */ cXyz field_0x634;
    /* 0x640 */ s16 field_0x640;
    /* 0x642 */ s16 field_0x642;
    /* 0x644 */ char pad644[1];              // Padding
    /* 0x645 */ s8 field_0x645;
    /* 0x646 */ char pad646[0x64C - 0x646];  // Padding
    /* 0x64C */ s16 field_0x64C[4];
    /* 0x654 */ s16 field_0x654;
    /* 0x656 */ s8 field_0x656;
    /* 0x658 */ s16 field_0x658;
    /* 0x65C */ dBgS_AcchCir mAcchCir;
    /* 0x69C */ dBgS_ObjAcch mBgc;
    /* 0x874 */ dCcD_Stts mStts;
    /* 0x8B0 */ dCcD_Sph mSphere;
    /* 0x9E8 */ u8 field_0x9E8;
};

STATIC_ASSERT(sizeof(bd_class) == 0x9ec);

class daBd_HIO_c : public JORReflexible {
public:
    daBd_HIO_c();
    virtual ~daBd_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 mBasicSize;
    /* 0x0C */ f32 mFlightSpeed;
    /* 0x10 */ f32 mGroundSpeed;
    /* 0x14 */ s16 mFlightTime;
    /* 0x18 */ f32 mLinkDetectRange;
    /* 0x1C */ s16 mChirpDist;
    /* 0x1E */ s8 field_0x1E;
};

#endif /* D_A_BD_H */
