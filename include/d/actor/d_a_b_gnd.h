#ifndef D_A_B_GND_H
#define D_A_B_GND_H

#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"

/**
 * @ingroup actors-enemies
 * @class b_gnd_class
 * @brief Ganondorf
 * 
 * @details Hyrule Castle dungeon boss.
 * 
 */
 
struct g_himo_s {
    /* 0x0 */ cXyz field_0x0[16];
};

class b_gnd_class : public fopEn_enemy_c {
public:
    b_gnd_class();

    bool checkAttackChance() { return field_0x2740 != 0; }
    bool checkPiyo() { return field_0x1e08 != 0; }
    bool checkRide() { return mDrawHorse != 0; }
    BOOL checkZeldaEndDemoCut() { return FALSE; }

    /* 0x05AC */ request_of_phase_process_class mPhaseReq;
    /* 0x05B4 */ request_of_phase_process_class mHorsePhaseReq;
    /* 0x05BC */ s16 mMoveMode;
    /* 0x05BE */ u8 field_0x5be;
    /* 0x05C0 */ cXyz field_0x5c0;
    /* 0x05CC */ s16 field_0x5cc;
    /* 0x05D0 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05D4 */ mDoExt_McaMorfSO* mpHorseMorf;
    /* 0x05D8 */ mDoExt_McaMorf* mpZeldaModel;
    /* 0x05DC */ Z2CreatureEnemy mGndSound;
    /* 0x0680 */ Z2CreatureEnemy mHorseSound;
    /* 0x0724 */ mDoExt_brkAnm* mpGndCoreBrk;
    /* 0x0728 */ mDoExt_btkAnm* mpGndEyeBtk[6];
    /* 0x0740 */ u8 field_0x740[0x0744 - 0x0740];
    /* 0x0744 */ mDoExt_btpAnm* mpGndEyeBtp;
    /* 0x0748 */ int mGndEyeBtkNo;
    /* 0x074C */ mDoExt_btpAnm* mpZeldaBtp;
    /* 0x0750 */ mDoExt_btkAnm* mpZeldaBtk;
    /* 0x0754 */ u8 mDrawZelda;
    /* 0x0758 */ int mAnmID;
    /* 0x075C */ int mHorseAnmID;
    /* 0x0760 */ f32 mPlaySpeed;
    /* 0x0764 */ u8 field_0x764[0x0768 - 0x0764];
    /* 0x0768 */ J3DModel* mpSwordModel;
    /* 0x076C */ J3DModel* mpSheathModel;
    /* 0x0770 */ u8 field_0x770;
    /* 0x0771 */ u8 mHideSheath;
    /* 0x0772 */ s16 field_0x772;
    /* 0x0774 */ dKy_tevstr_c mSwordTevstr;
    /* 0x0AFC */ s16 mCounter;
    /* 0x0AFE */ s16 mActionMode;
    /* 0x0B00 */ s16 field_0xb00;
    /* 0x0B02 */ u8 field_0xB02[0x0B94 - 0x0B02];
    /* 0x0B94 */ J3DModel* mpMantShadowModel[36];
    /* 0x0C24 */ s16 mSwordBlurAlpha;
    /* 0x0C26 */ s16 mSwordBlurType;
    /* 0x0C28 */ s16 field_0xc28;
    /* 0x0C2C */ J3DModel* mpSwordBlurModel[2];
    /* 0x0C34 */ u8 field_0xC34[0x0C38 - 0x0C34];
    /* 0x0C38 */ f32 mPlayerDistXZ;
    /* 0x0C3C */ s16 mPlayerAngleY;
    /* 0x0C40 */ u32 mShadowID;
    /* 0x0C44 */ s16 field_0xc44[10];
    /* 0x0C58 */ s16 mDamageInvulnerabilityTimer;
    /* 0x0C5A */ s16 field_0xc5a;
    /* 0x0C5C */ s16 mHorseLegRot;
    /* 0x0C60 */ int field_0xc60;
    /* 0x0C64 */ int field_0xc64;
    /* 0x0C68 */ s16 field_0xc68;
    /* 0x0C6A */ s16 field_0xc6a;
    /* 0x0C6C */ f32 field_0xc6c;
    /* 0x0C70 */ s16 field_0xc70;
    /* 0x0C72 */ s16 field_0xc72;
    /* 0x0C74 */ s16 field_0xc74;
    /* 0x0C76 */ u8 mGakeChkType;
    /* 0x0C77 */ u8 field_0xc77;
    /* 0x0C78 */ u8 field_0xc78;
    /* 0x0C79 */ u8 field_0xc79;
    /* 0x0C7A */ u8 field_0xc7a;
    /* 0x0C7B */ u8 field_0xc7b;
    /* 0x0C7C */ u8 field_0xc7c;
    /* 0x0C7D */ s8 field_0xc7d;
    /* 0x0C80 */ cXyz field_0xc80;
    /* 0x0C8C */ s16 mGndBodyRotX;
    /* 0x0C8E */ s16 mGndHeadRotZ;
    /* 0x0C90 */ s16 field_0xc90;
    /* 0x0C92 */ s16 field_0xc92;
    /* 0x0C94 */ dBgS_AcchCir mAcchCir;
    /* 0x0CD4 */ dBgS_ObjAcch mAcch;
    /* 0x0EAC */ s16 field_0xeac;
    /* 0x0EB0 */ f32 field_0xeb0;
    /* 0x0EB4 */ cXyz field_0xeb4;
    /* 0x0EC0 */ csXyz field_0xec0;
    /* 0x0EC8 */ dCcD_Stts mHorseCcStts;
    /* 0x0F04 */ dCcD_Sph mHorseBodySph[2];
    /* 0x1174 */ dCcD_Sph mHorseFootSph[4];
    /* 0x1654 */ dCcD_Stts mGndCcStts;
    /* 0x1690 */ dCcD_Sph mGndSph[3];
    /* 0x1A38 */ dCcD_Sph mAtSph;
    /* 0x1B70 */ dCcD_Sph mDefSph;
    /* 0x1CA8 */ dCcD_Cyl mCoCyl;
    /* 0x1DE4 */ dCcU_AtInfo mAtInfo;
    /* 0x1E08 */ u8 field_0x1e08;
    /* 0x1E09 */ u8 field_0x1e09;
    /* 0x1E0A */ u16 field_0x1e0a;
    /* 0x1E0C */ u16 field_0x1e0c;
    /* 0x1E0E */ u8 field_0x1e0e;
    /* 0x1E0F */ s8 field_0x1e0f;
    /* 0x1E10 */ f32 field_0x1e10;
    /* 0x1E14 */ cXyz field_0x1e14;
    /* 0x1E20 */ cXyz field_0x1e20;
    /* 0x1E2C */ Z2SoundObjSimple field_0x1e2c;
    /* 0x1E4C */ f32 field_0x1e4c;
    /* 0x1E50 */ csXyz field_0x1e50;
    /* 0x1E56 */ s8 field_0x1e56[5];
    /* 0x1E5C */ cXyz mLightBallPos[5];
    /* 0x1E98 */ cXyz field_0x1e98[5];
    /* 0x1ED4 */ cXyz field_0x1ed4[5];
    /* 0x1F10 */ Z2SoundObjSimple mLightBallSound[5];
    /* 0x1FB0 */ u8 mInitHIO;
    /* 0x1FB4 */ fpc_ProcID mMantChildID;
    /* 0x1FB8 */ cXyz field_0x1fb8;
    /* 0x1FC4 */ int mDrawHorse;
    /* 0x1FC8 */ s16 field_0x1fc8;
    /* 0x1FCA */ s16 mGndArmRRotX;
    /* 0x1FCC */ s16 mGndShoulderLRotY;
    /* 0x1FD0 */ f32 field_0x1fd0;
    /* 0x1FD4 */ s16 field_0x1fd4;
    /* 0x1FD6 */ s16 field_0x1fd6;
    /* 0x1FD8 */ cXyz field_0x1fd8[2];
    /* 0x1FF0 */ g_himo_s field_0x1ff0[2];
    /* 0x2170 */ mDoExt_3DlineMat1_c mHorseReins[2];
    /* 0x21E8 */ mDoExt_3DlineMat1_c field_0x21e8;
    /* 0x2224 */ dKy_tevstr_c field_0x2224;
    /* 0x25AC */ u32 field_0x25ac[2];
    /* 0x25B4 */ u32 field_0x25b4;
    /* 0x25B8 */ u32 field_0x25b8;
    /* 0x25BC */ u32 field_0x25bc;
    /* 0x25C0 */ u32 field_0x25c0;
    /* 0x25C4 */ u32 field_0x25c4;
    /* 0x25C8 */ u32 field_0x25c8;
    /* 0x25CC */ u32 field_0x25cc;
    /* 0x25D0 */ u32 field_0x25d0;
    /* 0x25D4 */ u32 field_0x25d4;
    /* 0x25D8 */ u32 field_0x25d8;
    /* 0x25DC */ u32 field_0x25dc;
    /* 0x25E0 */ u32 field_0x25e0;
    /* 0x25E4 */ u32 field_0x25e4[3];
    /* 0x25F0 */ u32 field_0x25f0[6];
    /* 0x2608 */ u32 field_0x2608[5][6];
    /* 0x2680 */ u32 field_0x2680[2];
    /* 0x2688 */ u32 field_0x2688;
    /* 0x268C */ u32 field_0x268c[3];
    /* 0x2698 */ u8 field_0x2698;
    /* 0x2699 */ u8 field_0x2699;
    /* 0x269C */ cXyz field_0x269c;
    /* 0x26A8 */ cXyz field_0x26a8[2];
    /* 0x26C0 */ s16 mGndLegRotX;
    /* 0x26C2 */ s16 field_0x26c2;
    /* 0x26C4 */ s16 mDemoCamMode;
    /* 0x26C6 */ s16 mDemoCamTimer;
    /* 0x26C8 */ cXyz mDemoCamEye;
    /* 0x26D4 */ cXyz mDemoCamCenter;
    /* 0x26E0 */ cXyz mDemoCamEyeTarget;
    /* 0x26EC */ cXyz mDemoCamCenterTarget;
    /* 0x26F8 */ cXyz mDemoCamEyeSpd;
    /* 0x2704 */ cXyz mDemoCamCenterSpd;
    /* 0x2710 */ cXyz field_0x2710;
    /* 0x271C */ s16 field_0x271c;
    /* ox2720 */ cXyz field_0x2720;
    /* 0x272C */ s16 field_0x272c;
    /* 0x2730 */ f32 mTubazeriPushValue;
    /* 0x2734 */ f32 mTubazeriPushAmount;
    /* 0x2738 */ u8 mNoDrawTimer;
    /* 0x2739 */ u8 field_0x2739;
    /* 0x273C */ f32 mKankyoBlend;
    /* 0x2740 */ u8 field_0x2740;
    /* 0x2744 */ dMsgFlow_c mMsgFlow;
};

STATIC_ASSERT(sizeof(b_gnd_class) == 0x2790);

#endif /* D_A_B_GND_H */
