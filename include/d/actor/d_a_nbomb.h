#ifndef D_A_NBOMB_H
#define D_A_NBOMB_H

#include "d/d_bomb.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"

/**
 * @ingroup actors-unsorted
 * @class daNbomb_c
 * @brief Bomb
 *
 * @details Multi-purpose bomb actor. Used for normal bombs, water bombs and bomblings.
 *
 */
class daNbomb_c : public dBomb_c {
public:
    enum daNbomb_FLG0 {
        FLG0_PLAYER_MAKE = 0x1,
        FLG0_BOMB_HIT = 0x4,
        FLG0_WATER_BOMB = 0x10,
        FLG0_UNDERWATER = 0x20,
        FLG0_UNK_40 = 0x40,
        FLG0_NO_HIT_PLAYER = 0x80,
        FLG0_CARGO_CARRY = 0x100,
        FLG0_FROZEN = 0x200,
        FLG0_INSECT_BOMB = 0x400,
        FLG0_UNK_800 = 0x800,
        FLG0_DELETE_BOMB = 0x1000,
        FLG0_UNK_2000 = 0x2000,
        FLG0_SET_HOOKSHOT_OFFSET = 0x4000,
        FLG0_UNK_8000 = 0x8000,
        FLG0_UNK_10000 = 0x10000,
        FLG0_UNK_20000 = 0x20000,
    };

    enum daNbomb_TYPE {
        TYPE_NORMAL_PLAYER,
        TYPE_WATER_PLAYER,
        TYPE_INSECT_PLAYER,
        TYPE_FLOWER,
        TYPE_INSECT_ENEMY,
        TYPE_WATER_ENEMY,
    };

    void coHitCallback(fopAc_ac_c*);
    void tgHitCallback(dCcD_GObjInf*);
    int searchEnemy(fopAc_ac_c*);
    int createHeap();
    int create();
    ~daNbomb_c();
    bool checkTimerStop();
    BOOL checkExplode();
    void setRoomInfo();
    void setSmokePos();
    void setEffect();
    void setHookshotOffset();
    void setFreeze();
    BOOL checkWaterIn();
    BOOL insectLineCheck();
    void setHitPolygon(int);
    BOOL procExplodeInit();
    BOOL procExplode();
    BOOL procCarryInit();
    BOOL procCarry();
    BOOL procWaitInit();
    BOOL procWait();
    BOOL procFlowerWaitInit();
    BOOL procFlowerWait();
    BOOL procBoomerangMoveInit(dCcD_GObjInf*);
    BOOL procBoomerangMove();
    BOOL procInsectMoveInit();
    BOOL procInsectMove();
    int execute();
    int draw();

    virtual u8 checkExplodeNow() { return field_0xb51 != 0; }
    virtual void deleteBombAndEffect() {
        fopAcM_delete(this);
        onStateFlg0(FLG0_UNK_40);
    }
    virtual void setCargoBombExplode() { onStateFlg0(FLG0_BOMB_HIT); }

    void onStateFlg0(daNbomb_FLG0 i_flag) { mStateFlg0 |= i_flag; }
    void offStateFlg0(daNbomb_FLG0 i_flag) { mStateFlg0 &= ~i_flag; }
    u32 checkStateFlg0(daNbomb_FLG0 i_flag) const { return mStateFlg0 & i_flag; }

    u32 checkWaterBomb() const { return checkStateFlg0(FLG0_WATER_BOMB); }
    u32 checkPlayerMake() const { return checkStateFlg0(FLG0_PLAYER_MAKE); }

    s16 getExTime() { return mExTime; }

    static const char* m_arcNameList[6];

    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgS_BombAcch mAcch;
    /* 0x750 */ dBgS_AcchCir mAcchCir;
    /* 0x790 */ dCcD_Stts mCcStts;
    /* 0x7CC */ dCcD_Sph mCcSph;
    /* 0x904 */ dCcD_Sph mSph2;
    /* 0xA3C */ mDoExt_bckAnm* mpBck;
    /* 0xA40 */ Mtx field_0xa40;
    /* 0xA70 */ Mtx field_0xa70;
    /* 0xAA0 */ dBgS_BombLinChk mLineChk;
    /* 0xB10 */ cBgS_PolyInfo mInsectHitPolyInfo;
    /* 0xB20 */ Z2SoundObjSimple mSound;
    /* 0xB40 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0xB4C */ u32 mStateFlg0;
    /* 0xB50 */ u8 mExplodeMode;
    /* 0xB51 */ u8 field_0xb51;
    /* 0xB52 */ u8 mNoHitPlayerTimer;
    /* 0xB53 */ s8 mSoundReverb;
    /* 0xB54 */ u8 mType;
    /* 0xB55 */ u8 field_0xB55[0xB58 - 0xB55];
    /* 0xB58 */ s16 mExTime;
    /* 0xB5A */ s16 field_0xb5a;
    /* 0xB5C */ s16 field_0xb5c;
    /* 0xB5E */ s16 mFreezeTimer;
    /* 0xB60 */ LIGHT_INFLUENCE mLightInfluence;
    /* 0xB80 */ WIND_INFLUENCE mWindInfluence;
    /* 0xBAC */ f32 mExplosionStrength;
    /* 0xBB0 */ f32 field_0xbb0;
    /* 0xBB4 */ u32 mPolySound;
    /* 0xBB8 */ u32 mEffectEmitterIDs[5];
    /* 0xBCC */ cXyz mEffectPosition;
    /* 0xBD8 */ cXyz mEffectLastPosition;
    /* 0xBE4 */ cXyz field_0xbe4;
    /* 0xBF0 */ cXyz field_0xbf0;
    /* 0xBFC */ cXyz field_0xbfc;
    /* 0xC08 */ cXyz field_0xc08;
    /* 0xC14 */ cXyz field_0xc14;
    /* 0xC20 */ cXyz field_0xc20;
    /* 0xC2C */ cXyz field_0xc2c;
    /* 0xC38 */ BOOL (daNbomb_c::*mProcFunc)();
};

#endif /* D_A_NBOMB_H */
