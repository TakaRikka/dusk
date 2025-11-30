#ifndef D_A_NPC_WRESTLER_H
#define D_A_NPC_WRESTLER_H

#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpcWrestler_c
 * @brief Goron (Fat)
 *
 * @details
 *
 */

struct daNpcWrestler_HIOParamSub {
    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ f32 mWallH;
    /* 0x08 */ f32 mWallR;
    /* 0x0C */ f32 real_shadow_size;                   // リアル影サイズ (Real Shadow Size)
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 reasonable_distance;                  // 見合う距離 (Right Distance)
    /* 0x18 */ f32 grapple_distance;            // 組み合い距離 (Grapple Distance)
    /* 0x1C */ f32 reconfig_distance;                  // 再配置距離 (Reconfiguration Distance)
    /* 0x20 */ s16 lateral_movement_time;       // 横移動時間 (Lateral Movement Time)
    /* 0x22 */ s16 horizontal_movement_speed;   // 横移動速度 (Horizontal Movement Speed)
    /* 0x24 */ s16 stagger_time;                // ふらふら時間 (Stagger Time)
    /* 0x26 */ s16 wait_time;                   // 待ち時間 (Wait Time)
    /* 0x28 */ s16 bending_limit;                  // 曲がり限界 (Bending Limit)
    /* 0x2C */ struct {
        f32 lateral_movement_chance;            //  横移動 の確率 (Lateral Movement Chance)
        f32 hit_chance;                         //  張り手 の確率 (Hit Chance)
        f32 tackle_chance;                      // タックルの確率 (Tackle Chance)
        f32 wait_chance;                        //  待  機 の確率 (Wait Chance)
    } chances[2];
};

 struct daNpcWrestler_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ s16 ai_action;              // AI行動 (AI Action)
    /* 0x6E */ s16 ready_appearance_time;  // Ready? 出現時間 (Ready? Appearance Time)
    /* 0x70 */ s16 fight_appearance_time;  // Fight! 出現時間 (Fight! Appearance Time)
    /* 0x74 */ s32 slap_hit_impact;        // 張り手Hit衝撃 (Slap Hit Impact)
    /* 0x78 */ s16 rotation;               // 回転割り (Rotating)
    /* 0x7C */ f32 camera_rotation_angle;  // カメラ回転角 (Camera Rotation Angle)
    /* 0x80 */ f32 ring_edge_sound;             // 音 土俵際距離 (Sound, Distance From Edge Of Ring)
    /* 0x84 */ f32 ring_edge_movement;             // 動 土俵際距離 (Movement, Distance From Edge Of Ring)
    /* 0x88 */ f32 fade_speed;
    /* 0x8C */ s16 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C */ f32 field_0x9c;
    /* 0xA0 */ f32 field_0xa0;
    /* 0xA4 */ f32 field_0xa4;
    /* 0xA8 */ s16 field_0xa8;
    /* 0xAA */ s16 field_0xaa;
    /* 0xAC */ s16 field_0xac;
    /* 0xB0 */ f32 field_0xb0;
    /* 0xB4 */ s16 field_0xb4;
    /* 0xB6 */ s16 field_0xb6;
    /* 0xB8 */ Vec field_0xb8;
    /* 0xC4 */ Vec field_0xc4;
    /* 0xD0 */ Vec field_0xd0;
    /* 0xDC */ Vec field_0xdc;
    /* 0xE8 */ s16 field_0xe8;
    /* 0xEA */ s16 field_0xea;
    /* 0xEC */ s16 field_0xec;
    /* 0xEE */ s16 field_0xee;
    /* 0xF0 */ f32 field_0xf0;
    /* 0xF4 */ f32 vertical_speed;         // 垂直速度 (Vertical Speed)
    /* 0xF8 */ f32 horizontal_speed;       // 水平速度 (Horizontal Speed)
    /* 0xFC */ daNpcWrestler_HIOParamSub mTypeParams[2];
};

class daNpcWrestler_Param_c {
public:
    virtual ~daNpcWrestler_Param_c() {}

    static daNpcWrestler_HIOParam const m;
};

class daNpcWrestler_HIO_Node_c: public JORReflexible {
public:
    daNpcWrestler_HIO_Node_c() {}
    virtual ~daNpcWrestler_HIO_Node_c() {}

    void genMessage(JORMContext*);
    void setParam(daNpcWrestler_HIOParamSub* param_1) { field_0x4 = param_1; }

    /* 0x04 */ daNpcWrestler_HIOParamSub* field_0x4;
};

class daNpcWrestler_HIO_c : public mDoHIO_entry_c {
public:
    daNpcWrestler_HIO_c();
    virtual ~daNpcWrestler_HIO_c() {}

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x008 */ daNpcWrestler_HIOParam m;
    /* 0x19C */ daNpcWrestler_HIO_Node_c field_0x19c[2];
};

#if DEBUG
#define NPC_WRESTLER_HIO_CLASS daNpcWrestler_HIO_c
#else
#define NPC_WRESTLER_HIO_CLASS daNpcWrestler_Param_c
#endif

struct WrestlerParamList {
    daNpc_GetParam1* bck_list;
    daNpc_GetParam2* face_list;
    daNpc_GetParam1* btp_list;
    daNpc_GetParam1* btk_list;
};

struct DemoCamera_c {
    cXyz mDemoCamCenter;
    cXyz mDemoCamEye;
    cXyz field_0x18;
    cXyz field_0x24;
    cXyz field_0x30;
    cXyz field_0x3c;
};

class daNpcWrestler_c : public daNpcF_c {
public:
    typedef bool (daNpcWrestler_c::*actionFunc)(void*);
    typedef BOOL (daNpcWrestler_c::*EventFn)(int);

    daNpcWrestler_c();
    cPhs__Step Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    bool checkStartUp();
    void reset();
    inline int setAction(actionFunc);
    void checkArenaInfo();
    bool checkArenaSub(fopAc_ac_c*);
    void setOnToArena(f32);
    inline bool wait(void*);
    inline bool talk(void*);
    inline bool demo(void*);
    inline bool gotoArena(void*);
    inline bool gotoLiving(void*);
    inline bool sumouReady(void*);
    inline bool sumouWait(void*);
    inline bool checkOutOfArenaP();
    inline bool setNextAction();
    inline bool sumouPunchHit(void*);
    inline bool sumouPunchChaseHit(void*);
    inline bool sumouPunchMiss(void*);
    inline bool checkOutOfArenaW();
    cXyz getJointPos(int);
    inline bool sumouPunchDraw(void*);
    inline bool sumouTackleHit(void*);
    inline bool sumouTackleMiss(void*);
    inline bool sumouTackleDraw(void*);
    inline bool sumouSideStep(void*);
    inline bool sumouLostBalance(void*);
    inline bool sumouPunchShock(void*);
    inline bool sumouPunchChaseShock(void*);
    inline bool sumouPunchStagger(void*);
    inline bool sumouTackleShock(void*);
    inline bool sumouTackleStagger(void*);
    inline bool sumouTackleStaggerRelease(void*);
    inline bool sumouTacklePush(void*);
    inline bool sumouTackleRelease(void*);
    inline bool demoSumouReady(void*);
    inline bool demoSumouWin(void*);
    inline bool demoSumouLose(void*);
    inline bool demoSumouWin2(void*);
    inline bool demoSumouLose2(void*);
    inline bool demoSumouUnilateralWin(void*);
    inline bool demoTalkAfterLose(void*);
    inline BOOL EvCut_grDSEntry(int);
    inline BOOL EvCut_grDSEntry2(int);
    inline BOOL EvCut_grDSEntry3_4(int);
    inline BOOL EvCut_grDSEntry5(int);
    inline BOOL EvCut_grDSLose(int);
    inline void setWrestlerVoice();
    inline void playMotion();
    inline void lookat();

    virtual ~daNpcWrestler_c();
    inline void setParam();
    inline BOOL main();
    inline BOOL ctrlBtk();
    void adjustShapeAngle() {}
    inline void setAttnPos();
    inline bool setExpressionAnm(int, bool);
    inline bool setExpressionBtp(int);
    inline void setExpression(int, f32);
    inline void setMotionAnm(int, f32);
    void setMotion(int, f32, int);
    inline BOOL drawDbgInfo();
    inline void drawOtherMdls();

    s16 getMessageNo() { return (fopAcM_GetParam(this) >> 8) & 0xFFFF; }
    int getWrestlerAction() { return mWrestlerAction; }
    u8 getType() { return argument & 0x7F; }
    u8 getWrestlerType() { return getType(); }
    bool chkAction(actionFunc i_action) { return i_action == field_0xdcc; }
    inline bool selectAction();
    s8 getArenaNo() { return (u8)fopAcM_GetParam(this); }
    u32 getStatusNo() { return fopAcM_GetParam(this) >> 24; }
    inline void setLookMode(int);
    inline BOOL chkFindPlayer();
    inline BOOL step(s16, int);
    inline bool setTalkAngle();
    inline void setStepAngle();
    inline void setExpressionTalkAfter();
    void initTalkAngle() { mTurnMode = 0; }
    inline void setNextSumouEvent(int);
    inline void setBackToLiving();
    void offWrestlerNoDraw() { mWrestlerNoDraw = 0; }
    void onWrestlerNoDraw() { mWrestlerNoDraw = 1; }
    inline void initDemoCamera_ArenaSide();
    inline void sumouAI();
    inline s16 oppositeToPlayer();
    inline void correctGraspPosAngle(bool);
    inline void initDemoCamera_ReadyLink();
    inline void initDemoCamera_ReadyWrestler();
    inline void playExpression();

    static EventFn mEvtSeqList[7];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ const daNpcWrestler_HIOParam* field_0xbd8;
    /* 0xBDC */ const daNpcWrestler_HIOParamSub* field_0xbdc;
    /* 0xBE0 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBE4 */ daNpcF_Lookat_c mLookAt;
    /* 0xC80 */ daNpcF_ActorMngr_c mActorMngr[2];
    /* 0xC90 */ dCcD_Cyl field_0xc90;
    /* 0xDCC */ actionFunc field_0xdcc;
    /* 0xDD8 */ JPABaseEmitter* field_0xdd8;
    /* 0xDDC */ JPABaseEmitter* field_0xddc;
    /* 0xDE0 */ JPABaseEmitter* field_0xde0;
    /* 0xDE4 */ JPABaseEmitter* field_0xde4;
    /* 0xDE8 */ u32 field_0xde8;
    /* 0xDEC */ u32 field_0xdec;
    /* 0xDF0 */ Vec mArenaPos;
    /* 0xDFC */ f32 mArenaExtent;
    /* 0xE00 */ s16 mArenaAngle;
    /* 0xE02 */ struct {
        u8 chkFlag;
    } mArenaInfo;
    /* 0xE03 */ u8 field_0xe03;
    /* 0xE04 */ WrestlerParamList* field_0xe04;
    /* 0xE08 */ DemoCamera_c mDemoCam;
    /* 0xE50 */ f32 mDemoCamFovy;
    /* 0xE54 */ f32 field_0xe54;
    /* 0xE58 */ f32 field_0xe58;
    /* 0xE5C */ s16 field_0xe5c;
    /* 0xE5E */ s16 field_0xe5e;
    /* 0xE60 */ u8 field_0xe60[0xe64 - 0xe60];
    /* 0XE64 */ request_of_phase_process_class mPhase;
    /* 0xE6C */ request_of_phase_process_class mPhase2;
    /* 0xE74 */ int mWrestlerAction;
    /* 0xE78 */ fpc_ProcID field_0xe78;
    /* 0xE7C */ int mItemNo;
    /* 0xE80 */ int field_0xe80;
    /* 0xE84 */ int field_0xe84;
    /* 0xE88 */ int mMsgNo;
    /* 0xE8C */ f32 field_0xe8c;
    /* 0xE90 */ s16 mLookMode;
    /* 0xE92 */ s16 field_0xe92;
    /* 0xE94 */ s16 mStepAngle;
    /* 0xE96 */ u16 field_0xe96;
    /* 0xE98 */ u8 field_0xe98;
    /* 0xE99 */ u8 field_0xe99;
    /* 0xE9A */ u8 field_0xe9a;
    /* 0xE9B */ u8 mType;
    /* 0xE9C */ u8 mWrestlerNoDraw;
};
STATIC_ASSERT(sizeof(daNpcWrestler_c) == 0xea0);

#endif /* D_A_NPC_WRESTLER_H */
