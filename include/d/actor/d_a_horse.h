#ifndef D_A_HORSE_H
#define D_A_HORSE_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_scene_exit.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include "m_Do/m_Do_audio.h"
#include "d/d_path.h"
#include "d/d_particle_copoly.h"

class daHorseRein_c {
public:
    void setReinPos(int);
    void setReinPosPart(int);

    /* 0x00 */ cXyz* field_0x0[2];
    /* 0x08 */ int field_0x8[2];
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ cXyz field_0x1c;
};  // Size: 0x28

class daHorseFootData_c {
public:
    /* 0x00 */ u8 field_0x0;
    /* 0x00 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2[0x4 - 0x2];
    /* 0x04 */ s16 field_0x4[4];
    /* 0x0C */ cXyz field_0xc;
    /* 0x18 */ cXyz field_0x18;
    /* 0x24 */ Mtx field_0x24[4];
};  // Size: 0xE4

class daHoZelda_c;
class daHorse_hio_c;

/**
 * @ingroup actors-unsorted
 * @class daHorse_c
 * @brief Epona
 *
 * @details
 *
 */
class daHorse_c : public fopAc_ac_c {
public:
    enum daHorse_ERFLG0 {
        /* 0x001 */ ERFLG0_UNK_1 = 0x1,
        /* 0x002 */ ERFLG0_UNK_2 = 0x2,
        /* 0x004 */ ERFLG0_UNK_4 = 0x4,
        /* 0x008 */ ERFLG0_UNK_8 = 0x8,
        /* 0x010 */ ERFLG0_MOVE_ACCEPT = 0x10,
        /* 0x020 */ ERFLG0_UNK_20 = 0x20,
        /* 0x040 */ ERFLG0_UNK_40 = 0x40,
        /* 0x080 */ ERFLG0_RIDE_RUN_FLG = 0x80,
        /* 0x100 */ ERFLG0_CUT_TURN_CANCEL = 0x100,
        /* 0x200 */ ERFLG0_UNK_200 = 0x200,
        /* 0x400 */ ERFLG0_UNK_400 = 0x400,
        /* 0x800 */ ERFLG0_UNK_800 = 0x800,

        ERFLG0_UNK_18 = 0x18,
    };

    enum daHorse_RFLG0 {
        /* 0x001 */ RFLG0_UNK_1 = 1,
        /* 0x002 */ RFLG0_ENEMY_SEARCH = 2,
        /* 0x004 */ RFLG0_UNK_4 = 4,
        /* 0x008 */ RFLG0_LASH_DASH_START = 8,
        /* 0x010 */ RFLG0_TURN_STAND = 0x10,
        /* 0x020 */ RFLG0_UNK_20 = 0x20,
        /* 0x040 */ RFLG0_TURN_STAND_CAMERA = 0x40,
        /* 0x080 */ RFLG0_UNK_80 = 0x80,
        /* 0x100 */ RFLG0_UNK_100 = 0x100,
        /* 0x200 */ RFLG0_UNK_200 = 0x200,
    };

    enum daHorse_FLG0 {
        /* 0x00000001 */ FLG0_UNK_1 = 0x1,
        /* 0x00000002 */ FLG0_UNK_2 = 0x2,
        /* 0x00000004 */ FLG0_UNK_4 = 0x4,
        /* 0x00000008 */ FLG0_UNK_8 = 0x8,
        /* 0x00000010 */ FLG0_UNK_10 = 0x10,
        /* 0x00000020 */ FLG0_RODEO_LEFT = 0x20,
        /* 0x00000040 */ FLG0_RIDE_START_FLG = 0x40,
        /* 0x00000080 */ FLG0_NO_DRAW_WAIT = 0x80,
        /* 0x00000100 */ FLG0_UNK_100 = 0x100,
        /* 0x00000200 */ FLG0_UNK_200 = 0x200,
        /* 0x00000400 */ FLG0_UNK_400 = 0x400,
        /* 0x00000800 */ FLG0_UNK_800 = 0x800,
        /* 0x00001000 */ FLG0_UNK_1000 = 0x1000,
        /* 0x00002000 */ FLG0_UNK_2000 = 0x2000,
        /* 0x00004000 */ FLG0_UNK_4000 = 0x4000,
        /* 0x00008000 */ FLG0_UNK_8000 = 0x8000,
        /* 0x00010000 */ FLG0_PLAYER_BACK_RIDE_LASH = 0x10000,
        /* 0x00020000 */ FLG0_UNK_20000 = 0x20000,
        /* 0x00040000 */ FLG0_UNK_40000 = 0x40000,
        /* 0x00080000 */ FLG0_UNK_80000 = 0x80000,
        /* 0x00100000 */ FLG0_UNK_100000 = 0x100000,
        /* 0x00200000 */ FLG0_UNK_200000 = 0x200000,
        /* 0x00400000 */ FLG0_UNK_400000 = 0x400000,
        /* 0x02000000 */ FLG0_UNK_2000000 = 0x2000000,
        /* 0x04000000 */ FLG0_UNK_4000000 = 0x4000000,
        /* 0x08000000 */ FLG0_CALL_HORSE = 0x8000000,
        /* 0x10000000 */ FLG0_UNK_10000000 = 0x10000000,
        /* 0x20000000 */ FLG0_TURN_CANCEL_KEEP = 0x20000000,
        /* 0x40000000 */ FLG0_UNK_40000000 = 0x40000000,
        /* 0x80000000 */ FLG0_RODEO_MODE = 0x80000000
    };

    enum daHorse_DEMOMODE {
        DEMO_LAST_e = 0x11,
        DEMO_NEW_ANM0_e = 0x200,
    };

    enum daHorse_PROC {
        PROC_WAIT_e,
        PROC_MOVE_e,
        PROC_STOP_e,
        PROC_TURN_e,
        PROC_JUMP_e,
        PROC_LAND_e,
        PROC_LARGE_DAMAGE_e,
        PROC_TOOL_DEMO_e,

        PROC_NONE_e = 8,
    };

    void cancelOriginalDemo() { 
        field_0x16b8 = 2;
        m_demoMode = 1;
    }

    void onDemoJumpDistance(f32 param_1, f32 param_2) {
        field_0x1768 = param_1;
        field_0x176c = param_2;
        field_0x1770 = param_1;

    }

    void changeDemoPos0(cXyz const* pos) {
        m_demoPos0 = *pos;
    }

    void coHitCallbackBoarJump(fopAc_ac_c*);
    void coHitCallbackBoarHit(fopAc_ac_c*, dCcD_GObjInf*);
    void coHitCallbackCowHit(fopAc_ac_c*);
    int modelCallBack(int);
    int createHeap();
    static BOOL checkEnding();
    int create();
    void setBasAnime(int);
    void resetBasAnime();
    int setDoubleAnime(f32, f32, f32, u16, u16, f32);
    int setSingleAnime(u16, f32, f32, s16, f32, int);
    void animePlay();
    int checkDemoAction();
    void setStickRodeoMove();
    void setStickCallMove();
    void setDemoMoveData(u32*, cXyz const*);
    void setDemoData();
    void acceptPlayerRide();
    void setStickData();
    void setTurnStartVibration();
    void setTurnVibration();
    void setMoveAnime(f32);
    int checkHorseNoMove(int);
    BOOL checkTurnPlayerState();
    int setSpeedAndAngle();
    BOOL checkWaitTurn() const;
    void setRoomInfo(int);
    void setBodyPart();
    void setMatrix();
    void setDashEffect(u32*);
    void setEffect();
    void setCollision();
    void autoGroundHit();
    void setTailAngle();
    void setNeckAngle();
    void copyFootMatrix();
    int setFootMatrix();
    int setLegAngle(f32, int, int, s16*);
    void footBgCheck();
    void setReinPosMoveInit(int);
    void copyReinPos();
    void setReinPosHandSubstance(int);
    void setReinPosNormalSubstance();
    void bgCheck();
    bool checkSpecialWallHitSubstance(cXyz const&) const;
    void setServiceWaitTimer();
    BOOL checkServiceWaitAnime();
    BOOL checkTurnInput();
    BOOL checkTgHitTurn();
    BOOL checkTurnAfterFastMove(f32);
    void setNeckAnimeMorf();
    void setNeckAnime(u16, f32, f32, s16);
    void resetNeckAnime();
    void setLashCnt();
    void setTalkModeWolf();
    void setBoarHit(fopAc_ac_c*, int);
    void savePos();
    int callHorseSubstance(cXyz const*);
    void setHorsePosAndAngleSubstance(cXyz const*, s16);
    BOOL checkPlayerHeavy();
    void setTgCoGrp(u32, u32);
    void onRideFlgSubstance();
    void offRideFlgSubstance();
    int procWaitInit();
    int procWait();
    int procMoveInit();
    int procMove();
    int procStopInit();
    int procStop();
    int procTurnInit(int);
    int procTurn();
    int procJumpInit(int);
    int procJump();
    int procLandInit(f32, int);
    int procLand();
    int procLargeDamageInit();
    int procLargeDamage();
    int procToolDemoInit();
    int procToolDemo();
    void searchSceneChangeArea(fopAc_ac_c*);
    int execute();
    int draw();
    ~daHorse_c();
    
    u32 getLashDashStart() const { return checkResetStateFlg0(RFLG0_LASH_DASH_START); }
    

    bool checkNoBombProc() const { return m_procID == PROC_WAIT_e || m_procID == PROC_MOVE_e; }
    BOOL checkResetStateFlg0(daHorse_RFLG0 flag) const { return m_resetStateFlg0 & flag; }
    bool checkEndResetStateFlg0(daHorse_ERFLG0 flag) const { return m_endResetStateFlg0 & flag; }
    u32 checkStateFlg0(daHorse_FLG0 flag) const { return m_stateFlg0 & flag; }
    f32 getNormalMaxSpeedF() { return m_normalMaxSpeedF; }
    f32 getLashMaxSpeedF() { return m_lashMaxSpeedF; }
    void changeDemoMoveAngle(s16 angle) { m_demoMoveAngle = angle; }
    void setDemoStickR(f32 stick) { m_demoStickR = stick; }
    void changeDemoMode(u32 param_0, int param_1) { m_demoMode = param_0; field_0x1728 = param_1; }
    void changeOriginalDemo() { field_0x16b8 = 3; field_0x1728 = 0; }
    void setHorsePosAndAngle(cXyz const* i_pos, s16 i_angle) { (this->*m_setHorsePosAngle)(i_pos, i_angle); }
    void onRideFlg() { (this->*m_onRideFlg)(); }
    void offRideFlg() { (this->*m_offRideFlg)(); }
    void onStateFlg0(daHorse_FLG0 flag) { m_stateFlg0 |= flag; }
    void offStateFlg0(daHorse_FLG0 flag) { m_stateFlg0 &= ~flag; }
    void onEndResetStateFlg0(daHorse_ERFLG0 i_flag) { m_endResetStateFlg0 |= i_flag;}
    void offEndResetStateFlg0(daHorse_ERFLG0 i_flag) { m_endResetStateFlg0 &= ~i_flag;}
    void onResetStateFlg0(daHorse_RFLG0 i_flag) { m_resetStateFlg0 |= i_flag;}
    void offNoDrawWait() { offStateFlg0(FLG0_NO_DRAW_WAIT); }
    bool checkSpecialWallHit(const cXyz& param_0) const { return (this->*m_checkSpecialWallHit)(param_0); }
    MtxP getSaddleMtx() { return m_model->getAnmMtx(21); }
    MtxP getRootMtx() { return m_model->getAnmMtx(0); }
    f32 getAnmFrameMax(int i_idx) const { return m_frameCtrl[i_idx].getEnd(); }
    f32 getAnmFrame(int i_idx) const { return m_frameCtrl[i_idx].getFrame(); }
    s16 getAimNeckAngleY() const { return m_aimNeckAngleY; }
    f32 getMorfFrame() const { return m_morf_frame; }
    f32 getBlendRate() { return m_anmRatio[1].getRatio(); }
    u16 getAnmIdx(int i_idx) const { return m_anmIdx[i_idx]; }
    int callHorse(const cXyz* param_0) { return (this->*m_callHorse)(param_0); }

    daHoZelda_c* getZeldaActor() { return (daHoZelda_c*)m_zeldaActorKeep.getActor(); }
    void setZeldaActor(fopAc_ac_c* i_actor) { m_zeldaActorKeep.setData(i_actor); }

    u32 checkTurnStandCamera() const { return checkResetStateFlg0(RFLG0_TURN_STAND_CAMERA); }
    BOOL checkTurnStand() const { return checkResetStateFlg0(RFLG0_TURN_STAND); }
    u32 checkRodeoMode() const { return checkStateFlg0(FLG0_RODEO_MODE); }
    bool checkCutTurnCancel() const { return checkEndResetStateFlg0(ERFLG0_CUT_TURN_CANCEL); }
    bool checkTurnCancelKeep() const { return checkStateFlg0(FLG0_TURN_CANCEL_KEEP); }
    BOOL checkRodeoLeft() const { return checkStateFlg0(FLG0_RODEO_LEFT); }
    BOOL checkHorseCallWait() const { return checkStateFlg0(FLG0_NO_DRAW_WAIT); }
    BOOL checkTurn() const { return m_procID == PROC_TURN_e && field_0x1720 == 0; }
    BOOL checkStop() const { return m_procID == PROC_STOP_e; }
    BOOL checkJump() const { return m_procID == PROC_JUMP_e; }
    bool checkWait() const { return m_procID == PROC_WAIT_e; }
    bool checkLand() const { return m_procID == PROC_LAND_e && field_0x171a == 0; }
    bool checkGetOff() const { return fabsf(speedF) < 3.0f; }
    BOOL checkEnemySearch() { return checkResetStateFlg0(RFLG0_ENEMY_SEARCH); }
    bool checkOriginalDemo() const { return field_0x16b8 == 3; }
    bool checkHorseDemoMode() { return field_0x16b8 != 0; }
    s16 checkCowHit() const { return m_cowHit; }
    s16 getCowHitAngle() const { return m_cowHitAngle; }
    u8 getRodeoPointCnt() const { return m_rodeoPointCnt; }

    void onTagJump(f32 param_0, f32 param_1, f32 param_2) {
        field_0x1768 = param_0;
        field_0x176c = param_1;
        field_0x1770 = param_2;
        onEndResetStateFlg0(ERFLG0_UNK_1);
    }

    void onMoveAccept() { onEndResetStateFlg0(ERFLG0_MOVE_ACCEPT); }
    void onPlayerLash() { onEndResetStateFlg0(ERFLG0_UNK_18); }
    void offPlayerBackRideLash() { offStateFlg0(FLG0_PLAYER_BACK_RIDE_LASH); }
    void onRodeoMode() { onStateFlg0(FLG0_RODEO_MODE); }
    void onRideStartFlg() { onStateFlg0(FLG0_RIDE_START_FLG); }
    void onRideRunFlg() { onEndResetStateFlg0(ERFLG0_RIDE_RUN_FLG); }
    void onPlayerBackRideLash() { onStateFlg0(FLG0_PLAYER_BACK_RIDE_LASH); }
    void onCutTurnCancel() { onEndResetStateFlg0(ERFLG0_CUT_TURN_CANCEL); }
    void offCutTurnCancel() { offEndResetStateFlg0(ERFLG0_CUT_TURN_CANCEL); }
    void onTurnCancelKeep() { onStateFlg0(FLG0_TURN_CANCEL_KEEP); }
    void offTurnCancelKeep() { offStateFlg0(FLG0_TURN_CANCEL_KEEP); }

    BOOL checkTurnCancelFrame() const { return checkTurn() && field_0x177C < m_frameCtrl[0].getFrame(); }
    BOOL checkStopCancelFrame() const { return checkStop() && field_0x177C < m_frameCtrl[0].getFrame(); }

    void initHorseMtx() {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        m_model->setBaseTRMtx(mDoMtx_stack_c::get());
        m_model->calc();
    }

    void offRodeoMode() {
        offStateFlg0(daHorse_FLG0(FLG0_RODEO_MODE | FLG0_UNK_400000));
        field_0x16bc = 0;
        mDoAud_subBgmStop();
    }

    void setSpeedF(f32 i_speed) { speedF = i_speed; }
    void setWalkSpeedF() { speedF = field_0x1764; }

    MtxP getLeftStirrupMtx() { return m_model->getAnmMtx(0x17); }
    MtxP getRightStirrupMtx() { return m_model->getAnmMtx(0x19); }
    void calcWeightEnvMtx() { m_model->calcWeightEnvelopeMtx(); }

    void setReinPosNormal() { (this->*m_setReinPosNormal)(); }
    void setReinPosHand(int param_0) { (this->*m_setReinPosHand)(param_0); }

    u32 getShadowID() const { return m_shadowID; }

    bool checkInputOnR() const { return m_padStickValue > 0.05f; }

    void onBagMaterial() {
        m_modelData->getMaterialNodePointer(5)->getShape()->show();
    }

    void offBagMaterial() {
        m_modelData->getMaterialNodePointer(5)->getShape()->hide();
    }

    static u16 const m_footJointTable[];
    static f32 const m_callLimitDistance2;

    /* 0x0568 */ request_of_phase_process_class m_phase;
    /* 0x0570 */ J3DModel* m_model;
    /* 0x0574 */ J3DModelData* m_modelData;
    /* 0x0578 */ mDoExt_btpAnm m_btp;
    /* 0x0590 */ mDoExt_MtxCalcAnmBlendTblOld* m_mtxcalc;
    /* 0x0594 */ mDoExt_AnmRatioPack m_anmRatio[3];
    /* 0x05AC */ mDoExt_MtxCalcOldFrame* m_oldFrame;
    /* 0x05B0 */ daPy_frameCtrl_c m_frameCtrl[3];
    /* 0x05F8 */ daHorse_hio_c* m_hio;
    /* 0x05FC */ dBgS_AcchCir m_acchcir[3];
    /* 0x06BC */ dBgS_HorseAcch m_acch;
    /* 0x0894 */ dCcD_Stts m_cc_stts;
    /* 0x08D0 */ dBgS_HorseLinChk m_linechk;
    /* 0x0940 */ dCcD_Cyl m_tgco_cyl[3];
    /* 0x0CF4 */ dCcD_Cyl m_boar_cyl;
    /* 0x0E30 */ dCcD_Cyl m_at_cyl;
    /* 0x0F6C */ dCcD_Sph m_head_sph;
    /* 0x10A4 */ Z2CreatureRide m_sound;
    /* 0x1140 */ void* field_0x1140;
    /* 0x1144 */ daPy_frameCtrl_c* field_0x1144;
    /* 0x1148 */ dPath* m_path;
    /* 0x114C */ dPath* m_rodeoPath;
    /* 0x1150 */ daHorseRein_c m_rein[3];
    /* 0x11C8 */ mDoExt_3DlineMat1_c m_reinLine;
    /* 0x1204 */ int field_0x1204;
    /* 0x1208 */ dMsgFlow_c m_msgflow;
    /* 0x1254 */ daPy_actorKeep_c m_zeldaActorKeep;
    /* 0x125C */ daScex_c* m_scnChg_buffer[50];
    /* 0x1324 */ daHorseFootData_c m_footData[4];
    /* 0x16B4 */ u8 m_procID;
    /* 0x16B5 */ s8 m_reverb;
    /* 0x16B6 */ u8 field_0x16b6;
    /* 0x16B7 */ u8 field_0x16b7;
    /* 0x16B8 */ u8 field_0x16b8;
    /* 0x16B9 */ u8 m_poly_sound;
    /* 0x16BA */ u8 m_rodeoPoint;
    /* 0x16BB */ u8 m_rodeoPointCnt;
    /* 0x16BC */ u8 field_0x16bc;
    /* 0x16BD */ u8 field_0x16bd;
    /* 0x16BE */ s8 m_scnChg_num;
    /* 0x16C0 */ u16 m_flowID;
    /* 0x16C2 */ s16 field_0x16c2;
    /* 0x16C4 */ u16 m_anmIdx[3];
    /* 0x16CA */ u8 field_0x16ca[0x16CE - 0x16CA];
    /* 0x16CE */ s16 field_0x16ce;
    /* 0x16CE */ s16 field_0x16d0;
    /* 0x16D2 */ u8 field_0x16d2[0x16D4 - 0x16D2];
    /* 0x16D4 */ s16 field_0x16d4[3];
    /* 0x16DA */ u8 field_0x16da[0x16E0 - 0x16DA];
    /* 0x16E0 */ s16 field_0x16e0[3];
    /* 0x16E6 */ u8 field_0x16e6[0x16E8 - 0x16E6];
    /* 0x16E8 */ s16 field_0x16e8;
    /* 0x16EA */ s16 field_0x16ea;
    /* 0x16EC */ s16 m_padStickAngleY;
    /* 0x16EE */ s16 m_serviceWaitTimer;
    /* 0x16F0 */ s16 field_0x16f0;
    /* 0x16F2 */ s16 m_aimNeckAngleY;
    /* 0x16F4 */ s16 m_btpFrame;
    /* 0x16F6 */ u16 field_0x16f6;
    /* 0x16F8 */ s16 m_lashAccelerationTime;
    /* 0x16FA */ s16 field_0x16fa;
    /* 0x16FC */ s16 m_demoMoveAngle;
    /* 0x16FE */ s16 m_lashRecoverTime;
    /* 0x1700 */ s16 m_lashCnt;
    /* 0x1702 */ s16 field_0x1702;
    /* 0x1704 */ s16 field_0x1704;
    /* 0x1706 */ s16 m_cowHitAngle;
    /* 0x1708 */ s16 m_cowHit;
    /* 0x170A */ s16 field_0x170a;
    /* 0x170C */ s16 field_0x170c;
    /* 0x170E */ s16 field_0x170e;
    /* 0x1710 */ s16 field_0x1710;
    /* 0x1712 */ s16 field_0x1712;
    /* 0x1714 */ s16 field_0x1714;
    /* 0x1716 */ s16 m_callMoveTimer;
    /* 0x1718 */ s16 field_0x1718;
    /* 0x171A */ s16 field_0x171a;
    /* 0x171C */ s16 field_0x171c;
    /* 0x171E */ s16 field_0x171e;
    /* 0x1720 */ s16 field_0x1720;
    /* 0x1722 */ s16 field_0x1722;
    /* 0x1724 */ u8 field_0x1724[0x1728 - 0x1724];
    /* 0x1728 */ int field_0x1728;
    /* 0x172C */ int m_demoStaffId;
    /* 0x1730 */ int field_0x1730;
    /* 0x1734 */ u32 m_shadowID;
    /* 0x1738 */ u32 field_0x1738[2];
    /* 0x1740 */ u32 m_demoMode;
    /* 0x1744 */ u32 m_stateFlg0;
    /* 0x1748 */ u32 m_resetStateFlg0;
    /* 0x174C */ u32 m_endResetStateFlg0;
    /* 0x1750 */ f32 m_morf_frame;
    /* 0x1754 */ f32 field_0x1754;
    /* 0x1758 */ f32 m_padStickValue;
    /* 0x175C */ f32 field_0x175c;
    /* 0x1760 */ f32 field_0x1760;
    /* 0x1764 */ f32 field_0x1764;
    /* 0x1768 */ f32 field_0x1768;
    /* 0x176C */ f32 field_0x176c;
    /* 0x1770 */ f32 field_0x1770;
    /* 0x1774 */ f32 field_0x1774;
    /* 0x1778 */ f32 field_0x1778;
    /* 0x177C */ f32 field_0x177C;
    /* 0x1780 */ f32 field_0x1780;
    /* 0x1784 */ f32 field_0x1784;
    /* 0x1788 */ f32 m_demoStickR;
    /* 0x178C */ f32 m_normalMaxSpeedF;
    /* 0x1790 */ f32 m_lashMaxSpeedF;
    /* 0x1794 */ f32 field_0x1794;
    /* 0x1798 */ f32 field_0x1798;
    /* 0x179C */ f32 m_lashAddSpeed;
    /* 0x17A0 */ cXyz field_0x17a0;
    /* 0x17AC */ cXyz m_bodyEyePos;
    /* 0x17B8 */ cXyz field_0x17b8;
    /* 0x17C4 */ cXyz m_demoPos0;
    /* 0x17D0 */ cXyz field_0x17d0;
    /* 0x17DC */ dPaPoF_c field_0x17dc;
    /* 0x18A4 */ int (daHorse_c::*m_proc)();
    /* 0x18B0 */ bool (daHorse_c::*m_checkSpecialWallHit)(const cXyz&) const;
    /* 0x18BC */ void (daHorse_c::*m_setReinPosHand)(int);
    /* 0x18C8 */ void (daHorse_c::*m_setReinPosNormal)();
    /* 0x18D4 */ int (daHorse_c::*m_callHorse)(const cXyz*);
    /* 0x18E0 */ void (daHorse_c::*m_setHorsePosAngle)(const cXyz* i_pos, s16 i_angle);
    /* 0x18EC */ void (daHorse_c::*m_onRideFlg)();
    /* 0x18F8 */ void (daHorse_c::*m_offRideFlg)();
};

class daHorse_hio_c1 {
public:
    /* 0x000 */ s16 max_turn;
    /* 0x002 */ s16 min_turn;
    /* 0x004 */ s16 lash_acceleration_time;
    /* 0x006 */ s16 jump_end_frame;
    /* 0x008 */ s16 air_end_frame;
    /* 0x00A */ s16 land_end_frame;
    /* 0x00C */ s16 spur_recovery_time;
    /* 0x00E */ s16 full_spur_recovery_time;
    /* 0x010 */ s16 continuous_spur_recovery_time;
    /* 0x012 */ s16 cliff_jump_end_frame;
    /* 0x014 */ s16 cliff_air_end_frame;
    /* 0x016 */ s16 cliff_land_end_frame;
    /* 0x018 */ s16 min_jump_time;
    /* 0x01A */ s16 kakariko_lash_acceleration_time;
    /* 0x01C */ f32 max_speed;
    /* 0x020 */ f32 wait_anm_speed;
    /* 0x024 */ f32 walk_anm_speed;
    /* 0x028 */ f32 fast_walk_anm_speed;
    /* 0x02C */ f32 run_anm_speed;
    /* 0x030 */ f32 fast_run_anm_speed;
    /* 0x034 */ f32 backwalk_anm_speed;
    /* 0x038 */ f32 wait_to_walk_rate;
    /* 0x03C */ f32 walk_to_fastwalk_rate;
    /* 0x040 */ f32 fastwalk_to_run_rate;
    /* 0x044 */ f32 run_to_fastrun_rate;
    /* 0x048 */ f32 walk_run_interpolation;
    /* 0x04C */ f32 acceleration;
    /* 0x050 */ f32 deceleration;
    /* 0x054 */ f32 stopping_deceleration;
    /* 0x058 */ f32 max_backward_speed;
    /* 0x05C */ f32 max_backward_acceleration;
    /* 0x060 */ f32 backward_idle_to_walk_rate;
    /* 0x064 */ f32 faststop_anm_speed;
    /* 0x068 */ f32 faststop_stand_anm_speed;
    /* 0x06C */ f32 faststop_interpolation;
    /* 0x070 */ f32 fast_stop_cancel_frame;
    /* 0x074 */ f32 faststop_deceleration_slow;
    /* 0x078 */ f32 faststop_deceleration;
    /* 0x07C */ f32 stand_anm_speed;
    /* 0x080 */ f32 stand_interpolation;
    /* 0x084 */ f32 faststop_stand_cancel_frame;
    /* 0x088 */ f32 stand_cancel_frame;
    /* 0x08C */ f32 add_lash_speed;
    /* 0x090 */ f32 lash_acceleration;
    /* 0x094 */ f32 min_lash_speed;
    /* 0x098 */ f32 jump_anm_speed;
    /* 0x09C */ f32 jump_start_frame;
    /* 0x0A0 */ f32 jump_interpolation;
    /* 0x0A4 */ f32 air_start_frame;
    /* 0x0A8 */ f32 air_interpolation;
    /* 0x0AC */ f32 land_anm_speed;
    /* 0x0B0 */ f32 land_start_frame;
    /* 0x0B4 */ f32 land_interpolation;
    /* 0x0B8 */ f32 land_cancel_frame;
    /* 0x0BC */ f32 cliff_rise_rate;
    /* 0x0C0 */ f32 boar_jump_horizontal;
    /* 0x0C4 */ f32 boar_jump_height;
    /* 0x0C8 */ f32 water_depth_limit;
    /* 0x0CC */ f32 enemy_search_range;
    /* 0x0D0 */ f32 m0D0;
    /* 0x0D4 */ f32 floor_angle_limit;
    /* 0x0D8 */ f32 dash_anm_speed;
    /* 0x0DC */ f32 field_0xdc;
    /* 0x0E0 */ f32 walk_min_speed;
    /* 0x0E4 */ f32 cliff_jump_anm_speed;
    /* 0x0E8 */ f32 cliff_jump_start_frame;
    /* 0x0EC */ f32 cliff_jump_interpolation;
    /* 0x0F0 */ f32 cliff_air_anm_speed;
    /* 0x0F4 */ f32 cliff_air_start_frame;
    /* 0x0F8 */ f32 cliff_air_interpolation;
    /* 0x0FC */ f32 cliff_land_anm_speed;
    /* 0x100 */ f32 cliff_land_start_frame;
    /* 0x104 */ f32 cliff_land_interpolation;
    /* 0x108 */ f32 cliff_land_cancel_frame;
    /* 0x10C */ f32 cliff_jump_horizontal;
    /* 0x110 */ f32 cliff_jump_vertical;
    /* 0x114 */ f32 kakariko_max_speed;
    /* 0x118 */ f32 kakariko_run_anm_speed;
    /* 0x11C */ f32 kakariko_fastrun_anm_speed;
    /* 0x120 */ f32 kakariko_add_lash_speed;
};

class daHorse_hio_c0 {
public:
    static const daHorse_hio_c1 m;
};

class daHorse_hio_c : public JORReflexible {
public:
    daHorse_hio_c() {
#if DEBUG
        mParameters = daHorse_hio_c0::m;
#endif
    }
#if DEBUG
    virtual ~daHorse_hio_c() {}
    void genMessage(JORMContext*);
    /* 0x4 */ s8 id;
    /* 0x8 */ daHorse_hio_c1 mParameters;
#endif
};

#endif /* D_A_HORSE_H */
