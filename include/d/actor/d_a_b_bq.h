#ifndef D_A_B_BQ_H
#define D_A_B_BQ_H

#include "f_op/f_op_actor_mng.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class b_bq_class
 * @brief Diababa
 *
 * @details Forest Temple dungeon boss.
 *
*/

class b_bq_class : public fopEn_enemy_c {
public:
    b_bq_class();

    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x05B8 */ mDoExt_brkAnm* mpDeadBrk;
    /* 0x05BC */ mDoExt_btkAnm* mpTodomeBtk;
    /* 0x05C0 */ s8 mPlayTodomeBtk;
    /* 0x05C4 */ J3DModel* mpEyeballModel;
    /* 0x05C8 */ s8 field_0x5c8;
    /* 0x05CC */ cXyz field_0x5cc;
    /* 0x05D8 */ f32 field_0x5d8;
    /* 0x05DC */ s16 field_0x5dc;
    /* 0x05E0 */ Z2CreatureEnemy mSound;
    /* 0x0684 */ Z2SoundObjSimple field_0x684;
    /* 0x06A4 */ Z2SoundObjSimple field_0x6a4;
    /* 0x06C4 */ int mAnmID;
    /* 0x06C8 */ s16 field_0x6c8;
    /* 0x06CA */ s16 mAction;
    /* 0x06CC */ s16 mMode;
    /* 0x06CE */ s16 mAngleToPlayer;
    /* 0x06D0 */ f32 mDistToPlayer;
    /* 0x06D4 */ s16 mTimers[5];
    /* 0x06DE */ s16 field_0x6de;
    /* 0x06E0 */ fpc_ProcID mTentacleIDs[2];
    /* 0x06E8 */ fpc_ProcID mMonkeyBombID;
    /* 0x06EC */ s8 field_0x6ec;
    /* 0x06ED */ s8 mColpatType;
    /* 0x06F0 */ f32 mColpatBlend;
    /* 0x06F4 */ s16 mHeadRot;
    /* 0x06F6 */ s16 field_0x6f6;
    /* 0x06F8 */ s8 mDisableDraw;
    /* 0x06F9 */ s8 field_0x6f9;
    /* 0x06FA */ s8 field_0x6fa;
    /* 0x06FB */ s8 field_0x6fb;
    /* 0x06FC */ s8 field_0x6fc;
    /* 0x06FD */ s8 field_0x6fd;
    /* 0x06FE */ s16 field_0x6fe;
    /* 0x0700 */ dCcD_Stts mCcStts;
    /* 0x073C */ dCcD_Sph mCcSph;
    /* 0x0874 */ dCcD_Stts mCcCoStts;
    /* 0x08B0 */ dCcD_Sph mCcCoreSph;
    /* 0x09E8 */ dCcD_Sph mCcCoreCoSph;
    /* 0x0B20 */ dCcD_Sph mCcMizuSph[4];
    /* 0x1000 */ dCcD_Sph mCcBodyCoSph;
    /* 0x1138 */ cXyz field_0x1138;
    /* 0x1144 */ cXyz field_0x1144;
    /* 0x1150 */ s8 mDamageBackCount;
    /* 0x1151 */ u8 field_0x1151;
    /* 0x1154 */ dCcU_AtInfo mAtInfo;
    /* 0x1178 */ f32 field_0x1178;
    /* 0x117C */ u8 field_0x117C[0x1180 - 0x117C];
    /* 0x1180 */ u32 field_0x1180;
    /* 0x1184 */ u32 field_0x1184;
    /* 0x1188 */ u32 field_0x1188;
    /* 0x118C */ u32 field_0x118c;
    /* 0x1190 */ u32 field_0x1190;
    /* 0x1194 */ u32 field_0x1194[5];
    /* 0x11A8 */ u32 field_0x11a8[12];
    /* 0x11D8 */ f32 field_0x11d8;
    /* 0x11DC */ u8 field_0x11DC[0x11FC - 0x11DC];
    /* 0x11FC */ s8 field_0x11fc;
    /* 0x1200 */ u32 mMizutamaEmtrIDs[4];
    /* 0x1210 */ u32 mSmokeEmtrIDs[2];
    /* 0x1218 */ u8 field_0x1218[0x1220 - 0x1218];
    /* 0x1220 */ u32 field_0x1220;
    /* 0x1224 */ u32 field_0x1224;
    /* 0x1228 */ u32 field_0x1228[4];
    /* 0x1238 */ s16 mDemoMode;
    /* 0x123A */ s16 mDemoModeTimer;
    /* 0x123C */ u8 field_0x123c;
    /* 0x1240 */ cXyz mDemoCamEye;
    /* 0x124C */ cXyz mDemoCamCenter;
    /* 0x1258 */ cXyz mDemoCamEyeTarget;
    /* 0x1264 */ cXyz field_0x1264;
    /* 0x1270 */ cXyz mDemoCamCenterTarget;
    /* 0x127C */ cXyz field_0x127c;
    /* 0x1288 */ cXyz field_0x1288;
    /* 0x1294 */ u8 field_0x1294[0x1298 - 0x1294];
    /* 0x1298 */ f32 field_0x1298;
    /* 0x129C */ f32 field_0x129c;
    /* 0x12A0 */ f32 field_0x12a0;
    /* 0x12A4 */ f32 mDemoCamFovy;
    /* 0x12A8 */ f32 field_0x12a8;
    /* 0x12AC */ s16 mBlureRate;
    /* 0x12AE */ s16 mBlureRateTarget;
    /* 0x12B0 */ cXyz mMizuAtStartPos;
    /* 0x12BC */ cXyz field_0x12bc;
    /* 0x12C8 */ cXyz field_0x12c8[16];
    /* 0x1388 */ int field_0x1388;
    /* 0x138C */ s16 field_0x138c;
    /* 0x138E */ s16 field_0x138e;
    /* 0x1390 */ s16 field_0x1390;
    /* 0x1392 */ u8 field_0x1392;
    /* 0x1393 */ s8 field_0x1393;
    /* 0x1394 */ s8 field_0x1394;
    /* 0x1395 */ s8 mSetBossExplode;
    /* 0x1396 */ s8 field_0x1396;
    /* 0x1397 */ s8 mSetDeadColor;
    /* 0x1398 */ f32 mDeadColor;
    /* 0x139C */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(b_bq_class) == 0x13A0);

#endif /* D_A_B_BQ_H */
