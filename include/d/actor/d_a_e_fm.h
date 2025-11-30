#ifndef D_A_E_FM_H
#define D_A_E_FM_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"
#include "Z2AudioLib/Z2Creature.h"

struct chain_s {
    /* 0x0000 */ J3DModel* model[26];
    /* 0x0068 */ cXyz field_0x68;
    /* 0x0074 */ cXyz pos[26];
    /* 0x01AC */ cXyz field_0x1ac[26];
    /* 0x02E4 */ cXyz field_0x2e4[26];
    /* 0x041C */ csXyz rot[26];
    /* 0x04B8 */ dKy_tevstr_c tevstr[26];
    /* 0x6088 */ cXyz sound_pos;
    /* 0x6094 */ cXyz field_0x6094;
    /* 0x60A0 */ u8 field_0x60A0[0x6170 - 0x60A0];
    /* 0x6170 */ int field_0x6170;
    /* 0x6174 */ f32 field_0x6174;
    /* 0x6178 */ f32 field_0x6178;
    /* 0x617C */ s8 field_0x617c;
    /* 0x617D */ s8 field_0x617d;
    /* 0x617E */ s8 field_0x617e;
    /* 0x6180 */ cXyz field_0x6180;
    /* 0x618C */ cXyz field_0x618c;
    /* 0x6198 */ f32 field_0x6198;
    /* 0x619C */ s8 field_0x619c;
    /* 0x619E */ s16 field_0x619e;
    /* 0x61A0 */ f32 field_0x61a0;
    /* 0x61A4 */ f32 field_0x61a4;
    /* 0x61A8 */ u8 field_0x61A8[0x61D0 - 0x61A8];
    /* 0x61D0 */ int field_0x61d0;
};

/**
 * @ingroup actors-enemies
 * @class e_fm_class
 * @brief Fyrus (Fire Man)
 * 
 * @details 
 * 
 */
class e_fm_class : public fopEn_enemy_c {
public:
    e_fm_class();

    BOOL checkCameraFree() { return field_0x1b07c; }

    /* 0x005AC */ request_of_phase_process_class mPhase;
    /* 0x005B4 */ u8 arg0;
    /* 0x005B5 */ u8 arg1;
    /* 0x005B6 */ u8 arg3;
    /* 0x005B7 */ u8 field_0x5B7[0x005C4 - 0x005B7];
    /* 0x005C4 */ s16 field_0x5c4;
    /* 0x005C6 */ s16 mAngleToPlayer;
    /* 0x005C8 */ int field_0x5c8;
    /* 0x005CC */ f32 mKankyoBlend;
    /* 0x005D0 */ mDoExt_McaMorf* mpFmModelMorf;
    /* 0x005D4 */ mDoExt_btkAnm* mpFmBtk[9];
    /* 0x005F8 */ mDoExt_brkAnm* mpFmBrk[9];
    /* 0x0061C */ int mPlayTexAnmNo;
    /* 0x00620 */ int mAnm;
    /* 0x00624 */ s8 mDrawDemoModel;
    /* 0x00628 */ mDoExt_McaMorf* mpDemoFmModelMorf;
    /* 0x0062C */ u8 field_0x62C[0x00630 - 0x0062C];
    /* 0x00630 */ mDoExt_brkAnm* mpDemoFmBrk;
    /* 0x00634 */ f32 mDemoModelSize;
    /* 0x00638 */ f32 mDemoModelSizeStep;
    /* 0x0063C */ Z2CreatureFM mSound;
    /* 0x00760 */ J3DModel* mpCoreModel;
    /* 0x00764 */ mDoExt_btkAnm* mpCoreBtk;
    /* 0x00768 */ mDoExt_brkAnm* mpCoreBrk;
    /* 0x0076C */ f32 mCoreBrkFrame;
    /* 0x00770 */ s16 field_0x770;
    /* 0x00772 */ s8 mDoCreateBa;
    /* 0x00773 */ u8 field_0x773;
    /* 0x00774 */ f32 mChainColorR;
    /* 0x00778 */ f32 field_0x778;
    /* 0x0077C */ mDoExt_McaMorf* mpAttackEfModelMorf[2];
    /* 0x00784 */ mDoExt_btkAnm* mpAttackEfBtk[2];
    /* 0x0078C */ mDoExt_brkAnm* mpAttackEfBrk;
    /* 0x00790 */ s16 field_0x790;
    /* 0x00792 */ s8 field_0x792;
    /* 0x00794 */ f32 field_0x794;
    /* 0x00798 */ f32 field_0x798;
    /* 0x0079C */ f32 field_0x79c;
    /* 0x007A0 */ s16 mCounter;
    /* 0x007A2 */ s16 mAction;
    /* 0x007A4 */ s16 mMode;
    /* 0x007A8 */ u32 mShadowKey;
    /* 0x007AC */ s16 mTimers[4];
    /* 0x007B4 */ s16 mDamageInvulnerabilityTimer;
    /* 0x007B6 */ s8 field_0x7b6;
    /* 0x007B8 */ f32 field_0x7b8;
    /* 0x007BC */ u8 field_0x7BC[0x007C0 - 0x007BC];
    /* 0x007C0 */ s8 field_0x7c0;
    /* 0x007C2 */ s16 mBodyRotX;
    /* 0x007C4 */ s16 mHeadRotZ;
    /* 0x007C8 */ cXyz mFootPos[2];
    /* 0x007E0 */ int field_0x7e0;
    /* 0x007E4 */ cXyz field_0x7e4[2];
    /* 0x007FC */ f32 field_0x7fc;
    /* 0x00800 */ s16 field_0x800;
    /* 0x00802 */ s8 mDownCnt;
    /* 0x00804 */ s16 field_0x804;
    /* 0x00806 */ s16 field_0x806;
    /* 0x00808 */ s8 field_0x808;
    /* 0x00809 */ s8 field_0x809;
    /* 0x0080C */ dBgS_AcchCir mAcchCir;
    /* 0x0084C */ dBgS_ObjAcch mAcch;
    /* 0x00A24 */ dCcD_Stts field_0xa24;
    /* 0x00A60 */ dCcD_Stts field_0xa60;
    /* 0x00A9C */ dCcD_Sph mCcSph[8];
    /* 0x0145C */ dCcD_Sph mAtSph;
    /* 0x01594 */ dCcD_Sph mCoreSph;
    /* 0x016CC */ dCcD_Sph mEffAtSph;
    /* 0x01804 */ dCcU_AtInfo mAtInfo;
    /* 0x01828 */ s8 field_0x1828;
    /* 0x01829 */ s8 field_0x1829;
    /* 0x0182A */ s8 field_0x182a;
    /* 0x0182B */ s8 field_0x182b;
    /* 0x0182C */ s8 field_0x182c;
    /* 0x0182D */ s8 mHideCore;
    /* 0x01830 */ f32 field_0x1830;
    /* 0x01834 */ f32 field_0x1834;
    /* 0x01838 */ u32 field_0x1838[14];
    /* 0x01870 */ u32 field_0x1870;
    /* 0x01874 */ u32 field_0x1874;
    /* 0x01878 */ u32 field_0x1878;
    /* 0x0187C */ u32 field_0x187c[2];
    /* 0x01884 */ u32 field_0x1884;
    /* 0x01888 */ u32 field_0x1888[4];
    /* 0x01898 */ fopAc_ac_c* mHashiraActors[10];
    /* 0x018C0 */ u8 field_0x18c0[0x018D0 - 0x018C0];
    /* 0x018D0 */ cXyz field_0x18d0[8];
    /* 0x01930 */ u8 field_0x1930[0x1990 - 0x01930];
    /* 0x01990 */ dCcD_Sph mFEffAtSph[8];
    /* 0x02350 */ u32 field_0x2350;
    /* 0x02354 */ u32 field_0x2354[8];
    /* 0x02374 */ u32 field_0x2374[6];
    /* 0x0238C */ u32 field_0x238c[3];
    /* 0x02398 */ s8 field_0x2398;
    /* 0x02399 */ s8 field_0x2399;
    /* 0x0239C */ LIGHT_INFLUENCE mPlight;
    /* 0x023BC */ chain_s mChain[4];
    /* 0x1AB0C */ s8 field_0x1ab0c[4];
    /* 0x1AB10 */ dCcD_Sph mChainAtSph[4];
    /* 0x1AFF0 */ u8 field_0x1aff0;
    /* 0x1AFF2 */ s16 mDemoCamMode;
    /* 0x1AFF4 */ s16 mDemoCamTimer;
    /* 0x1AFF6 */ s16 field_0x1aff6;
    /* 0x1AFF8 */ cXyz mDemoCamEye;
    /* 0x1B004 */ cXyz mDemoCamCenter;
    /* 0x1B010 */ cXyz mDemoCamEyeTarget;
    /* 0x1B01C */ cXyz mDemoCamCenterTarget;
    /* 0x1B028 */ cXyz mDemoCamEyeSpd;
    /* 0x1B034 */ cXyz mDemoCamCenterSpd;
    /* 0x1B040 */ cXyz field_0x1b040;
    /* 0x1B04C */ f32 mDemoCamFovy;
    /* 0x1B050 */ f32 field_0x1b050;
    /* 0x1B054 */ f32 field_0x1b054;
    /* 0x1B058 */ u8 field_0x1B058[0x1B05C - 0x1B058];
    /* 0x1B05C */ f32 field_0x1b05c;
    /* 0x1B060 */ u8 field_0x1B060[0x1B064 - 0x1B060];
    /* 0x1B064 */ f32 field_0x1b064;
    /* 0x1B068 */ f32 field_0x1b068;
    /* 0x1B06C */ f32 field_0x1b06c;
    /* 0x1B070 */ f32 field_0x1b070;
    /* 0x1B074 */ f32 field_0x1b074;
    /* 0x1B078 */ s8 field_0x1b078;
    /* 0x1B07C */ int field_0x1b07c;
    /* 0x1B080 */ s8 field_0x1b080;
    /* 0x1B081 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(e_fm_class) == 0x1b084);

#endif /* D_A_E_FM_H */
