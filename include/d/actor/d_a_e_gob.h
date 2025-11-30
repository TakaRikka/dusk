#ifndef D_A_E_GOB_H
#define D_A_E_GOB_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-enemies
 * @class e_gob_class
 * @brief Dangoro (Goron Boss)
 * 
 * @details 
 * 
 */
class e_gob_class : public fopEn_enemy_c {
public:
    e_gob_class();

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5B8 */ Z2CreatureGob mSound;
    /* 0x648 */ int mAnm;
    /* 0x64C */ J3DModel* mpArmorModel;
    /* 0x650 */ u32 field_0x650;
    /* 0x654 */ s16 mCounter;
    /* 0x656 */ s16 mAction;
    /* 0x658 */ s16 mMode;
    /* 0x65C */ cXyz field_0x65c;
    /* 0x668 */ s16 field_0x668;
    /* 0x66A */ s16 mPlayerAngleDist;
    /* 0x66C */ f32 mPlayerDist;
    /* 0x670 */ u8 field_0x670[0x674 - 0x670];
    /* 0x674 */ s16 mTimers[5];
    /* 0x67E */ s16 mDamageInvulnerabilityTimer;
    /* 0x680 */ f32 field_0x680;
    /* 0x684 */ f32 field_0x684;
    /* 0x688 */ f32 field_0x688;
    /* 0x68C */ u8 field_0x68C[0x690 - 0x68C];
    /* 0x690 */ cXyz field_0x690;
    /* 0x69C */ f32 field_0x69c;
    /* 0x6A0 */ s16 field_0x6a0;
    /* 0x6A2 */ s16 field_0x6a2;
    /* 0x6A4 */ s16 mBodyRotZ;
    /* 0x6A6 */ s16 mBodyRotY;
    /* 0x6A8 */ s16 field_0x6a8;
    /* 0x6AA */ s8 field_0x6aa;
    /* 0x6AB */ s8 field_0x6ab;
    /* 0x6AC */ s8 field_0x6ac;
    /* 0x6AE */ s16 mHeadRotY;
    /* 0x6B0 */ s16 mHeadRotZ;
    /* 0x6B2 */ s16 field_0x6b2;
    /* 0x6B4 */ s16 field_0x6b4;
    /* 0x6B6 */ s16 field_0x6b6;
    /* 0x6B8 */ cXyz field_0x6b8;
    /* 0x6C4 */ s16 field_0x6c4;
    /* 0x6C6 */ s16 field_0x6c6[4];
    /* 0x6CE */ s16 field_0x6ce[4];
    /* 0x6D6 */ s16 field_0x6d6;
    /* 0x6D8 */ s8 field_0x6d8;
    /* 0x6D9 */ s8 field_0x6d9;
    /* 0x6DA */ s8 field_0x6da;
    /* 0x6DC */ cXyz field_0x6dc;
    /* 0x6E8 */ cXyz field_0x6e8;
    /* 0x6F4 */ csXyz field_0x6f4;
    /* 0x6FC */ dBgS_AcchCir mAcchCir;
    /* 0x73C */ dBgS_ObjAcch mAcch;
    /* 0x914 */ dCcD_Stts mCcStts;
    /* 0x950 */ dCcD_Sph mAtSph;
    /* 0xA88 */ dCcD_Sph field_0xa88;
    /* 0xBC0 */ dCcD_Sph mCcSph;
    /* 0xCF8 */ dCcU_AtInfo mAtInfo;
    /* 0xD1C */ s8 field_0xd1c;
    /* 0xD1D */ s8 field_0xd1d;
    /* 0xD1E */ s8 field_0xd1e;
    /* 0xD1F */ s8 field_0xd1f;
    /* 0xD20 */ s8 field_0xd20;
    /* 0xD21 */ s8 mPlayerCutLRC;
    /* 0xD24 */ cXyz field_0xd24;
    /* 0xD30 */ u32 field_0xd30;
    /* 0xD34 */ u32 field_0xd34;
    /* 0xD38 */ s16 mDemoCamMode;
    /* 0xD3A */ s16 mDemoCamTimer;
    /* 0xD3C */ cXyz mDemoCamEye;
    /* 0xD48 */ cXyz mDemoCamCenter;
    /* 0xD54 */ cXyz mDemoCamEyeTarget;
    /* 0xD60 */ cXyz mDemoCamCenterTarget;
    /* 0xD6C */ cXyz mDemoCamEyeSpd;
    /* 0xD78 */ cXyz mDemoCamCenterSpd;
    /* 0xD84 */ f32 mDemoCamFovy;
    /* 0xD88 */ f32 field_0xd88;
    /* 0xD8C */ f32 field_0xd8c;
    /* 0xD90 */ s8 field_0xd90;
    /* 0xD94 */ dMsgFlow_c mMsgFlow;
    /* 0xDE0 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(e_gob_class) == 0xde4);

#endif /* D_A_E_GOB_H */
