#ifndef D_A_E_ZH_H
#define D_A_E_ZH_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_ZH_c
 * @brief Ball Master
 * 
 * @details 
 * 
 */

class daObjCarry_c;

class daE_ZH_c : public fopEn_enemy_c {
public:
    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    bool startDemoCheck();
    void mBallBGCheck();
    void mGateOpen();
    bool mColorSet(bool, f32, f32);
    bool mReturnLineMove();
    bool mBallBgLineCheck();
    bool mSearchMove(u8);
    void mStartParticleSet();
    bool mCutTypeCheck();
    void damage_check();
    void executeStartDemo();
    void executeWait();
    void executeBallWait();
    void executeOpenStart();
    void executeSearchMove();
    void executeBgHitSHMove();
    void executeCatchMove();
    void executeCatchFlyMove();
    void executeReturnDemo();
    void executeFlyDelete();
    void executeDamage();
    void executeDeadStop();
    void executeEntryCheck();
    void tagAction();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();
    daE_ZH_c();

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5B8 */ J3DModel* mEntranceModel;
    /* 0x5BC */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x5C0 */ Z2CreatureEnemy mSound;
    /* 0x664 */ daObjCarry_c* mS_Ball;
    /* 0x668 */ cXyz field_0x668;
    /* 0x674 */ u8 field_0x674[0x680 - 0x674];
    /* 0x680 */ cXyz field_0x680;
    /* 0x68C */ cXyz field_0x68c;
    /* 0x698 */ cXyz mRetrunStartLine;
    /* 0x6A4 */ cXyz mRetrunEndLine;
    /* 0x6B0 */ cXyz mBallHosei;
    /* 0x6BC */ cXyz field_0x6bc;
    /* 0x6C8 */ cXyz field_0x6c8[3];
    /* 0x6EC */ int field_0x6ec[3];
    /* 0x6F8 */ cXyz field_0x6f8[8];
    /* 0x758 */ u8 field_0x758[8];
    /* 0x760 */ f32 field_0x760;
    /* 0x764 */ f32 field_0x764[3];
    /* 0x770 */ f32 pmRoofPos;
    /* 0x774 */ f32 field_0x774;
    /* 0x778 */ f32 field_0x778;
    /* 0x77C */ f32 field_0x77c;
    /* 0x780 */ int mActionMode;
    /* 0x784 */ int mMoveMode;
    /* 0x788 */ u32 mShadowKey;
    /* 0x78C */ int field_0x78c;
    /* 0x790 */ int mAnm;
    /* 0x794 */ int field_0x794[3];
    /* 0x7A0 */ int field_0x7a0;
    /* 0x7A4 */ int field_0x7a4;
    /* 0x7A8 */ u8 field_0x7a8;
    /* 0x7A9 */ u8 bitSw;
    /* 0x7AA */ u8 bitSwEnd;
    /* 0x7AB */ u8 arg0;
    /* 0x7AC */ u8 field_0x7ac;
    /* 0x7AD */ u8 field_0x7ad;
    /* 0x7AE */ u8 field_0x7ae;
    /* 0x7AF */ u8 field_0x7af;
    /* 0x7B0 */ s16 mRollAngle;
    /* 0x7B2 */ s16 field_0x7b2;
    /* 0x7B4 */ dBgS_AcchCir mAcchCir;
    /* 0x7F4 */ dBgS_ObjAcch mBgc;
    /* 0x9CC */ dCcD_Stts mStts;
    /* 0xA08 */ dCcD_Sph field_0xa08;
    /* 0xB40 */ dCcD_Cyl field_0xb40;
    /* 0xC7C */ dCcD_Cyl field_0xc7c;
    /* 0xDB8 */ dCcU_AtInfo mAtInfo;
    /* 0xDDC */ u32 field_0xddc[3];
    /* 0xDE8 */ u32 field_0xde8;
    /* 0xDEC */ cXyz mDemoCamEye;
    /* 0xDF8 */ cXyz mDemoCamCenter;
    /* 0xE04 */ u8 field_0xe04;
};

STATIC_ASSERT(sizeof(daE_ZH_c) == 0xe08);

class daE_ZH_HIO_c: public JORReflexible {
public:
    daE_ZH_HIO_c();
    virtual ~daE_ZH_HIO_c() {}

    void genMessage(JORMContext* ctx);

    // ボールマスター (Ball Master)
    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 model_size;          // モデルサイズ (Model Size)
    /* 0x0C */ f32 ascent_rate;         // 上昇速度（落下後）(Rate of ascent (after falling))
    /* 0x10 */ f32 wandering_area;      // うろうろ範囲 (Wandering Area)
    /* 0x14 */ s16 wait_time;           // 玉取られ後移動開始待ち時間 (Waiting time after the stone is taken and movement begins)
    /* 0x16 */ s16 piyori_time;         // ピヨリ時間 (Piyori Time)
    /* 0x18 */ u8 dungeon_bit_check;    // ダンジョンビットチェックON／OFF (Dungeon Bit Check ON/OFF)
};

#endif /* D_A_E_ZH_H */
