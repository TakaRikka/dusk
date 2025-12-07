#ifndef F_OP_ACTOR_H_
#define F_OP_ACTOR_H_

#include "d/d_kankyo_tev_str.h"
#include "f_pc/f_pc_leaf.h"
#include "global.h"

struct actor_method_class {
    /* 0x00 */ leafdraw_method_class base;
    /* 0x14 */ u8 field_0x14[0xC];  // Likely padding
};

struct actor_process_profile_definition {
    /* 0x00 */ leaf_process_profile_definition base;
    /* 0x24 */ actor_method_class* sub_method;
    /* 0x28 */ u32 status;
    /* 0x2C */ u8 group;
    /* 0x2D */ u8 cullType;
};

// Unclear what this is. Only appears in 4 profiles (BG,DSHUTTER,PATH,SCENE_EXIT)
struct actor_process_profile_definition2 {
    /* 0x00 */ actor_process_profile_definition base;
    /* 0x30 */ u32 field_0x30;
};

enum fopAc_Status_e {
    fopAcStts_NOEXEC_e = 0x00000080,
    fopAcStts_CULL_e = 0x00000100,
    fopAcStts_FREEZE_e = 0x00000400,
    fopAcStts_CARRY_e = 0x00002000,
    fopAcStts_NOPAUSE_e = 0x00020000,
    fopAcStts_NODRAW_e = 0x01000000,
    fopAcStts_BOSS_e = 0x04000000,
};

enum fopAc_Group_e {
    /* 0 */ fopAc_ACTOR_e,
    /* 1 */ fopAc_PLAYER_e,
    /* 2 */ fopAc_ENEMY_e,
    /* 3 */ fopAc_ENV_e,  // is this correct?
    /* 4 */ fopAc_NPC_e,
    /* 5 */ fopAc_UNK_GROUP_5_e,
};

enum fopAc_Condition_e {
    fopAcCnd_NOEXEC_e = 0x02,
    fopAcCnd_NODRAW_e = 0x04,
    fopAcCnd_INIT_e = 0x08,
};

enum fopAc_Cull_e {
    fopAc_CULLBOX_0_e,
    fopAc_CULLBOX_1_e,
    fopAc_CULLBOX_2_e,
    fopAc_CULLBOX_3_e,
    fopAc_CULLBOX_4_e,
    fopAc_CULLBOX_5_e,
    fopAc_CULLBOX_6_e,
    fopAc_CULLBOX_7_e,
    fopAc_CULLBOX_8_e,
    fopAc_CULLBOX_9_e,
    fopAc_CULLBOX_10_e,
    fopAc_CULLBOX_11_e,
    fopAc_CULLBOX_12_e,
    fopAc_CULLBOX_13_e,
#if DEBUG
    fopAc_CULLBOX_14_e,
#endif
    fopAc_CULLBOX_CUSTOM_e,
    fopAc_CULLSPHERE_0_e,
    fopAc_CULLSPHERE_1_e,
    fopAc_CULLSPHERE_2_e,
    fopAc_CULLSPHERE_3_e,
    fopAc_CULLSPHERE_4_e,
    fopAc_CULLSPHERE_5_e,
    fopAc_CULLSPHERE_6_e,
    fopAc_CULLSPHERE_7_e,
#if DEBUG
    fopAc_CULLSPHERE_8_e,
#endif
    fopAc_CULLSPHERE_CUSTOM_e,
};

enum fopAc_attention_type {
    /* 0x0 */ fopAc_attn_LOCK_e,
    /* 0x1 */ fopAc_attn_TALK_e,
    /* 0x2 */ fopAc_attn_BATTLE_e,

#if !PLATFORM_GCN
    fopAc_attn_UNK_3,
    fopAc_attn_UNK_4,
    fopAc_attn_UNK_5,
#endif

    /* 0x3 */ fopAc_attn_SPEAK_e,
    /* 0x4 */ fopAc_attn_CARRY_e,
    /* 0x5 */ fopAc_attn_DOOR_e,
    /* 0x6 */ fopAc_attn_JUEL_e,

#if !PLATFORM_GCN
    fopAc_attn_UNK_10,
#endif

    /* 0x7 */ fopAc_attn_ETC_e,
    /* 0x8 */ fopAc_attn_CHECK_e,

    fopAc_attn_MAX_e,
};

enum fopAc_AttentionFlag_e {
    /* 0x00000001 */ fopAc_AttnFlag_LOCK_e   = (1 << fopAc_attn_LOCK_e),
    /* 0x00000002 */ fopAc_AttnFlag_TALK_e   = (1 << fopAc_attn_TALK_e),
    /* 0x00000004 */ fopAc_AttnFlag_BATTLE_e = (1 << fopAc_attn_BATTLE_e),

    /* 0x00000008 */ fopAc_AttnFlag_SPEAK_e  = (1 << fopAc_attn_SPEAK_e),
    /* 0x00000010 */ fopAc_AttnFlag_CARRY_e  = (1 << fopAc_attn_CARRY_e),
    /* 0x00000020 */ fopAc_AttnFlag_DOOR_e   = (1 << fopAc_attn_DOOR_e),
    /* 0x00000040 */ fopAc_AttnFlag_JUEL_e   = (1 << fopAc_attn_JUEL_e),
    /* 0x00000080 */ fopAc_AttnFlag_ETC_e    = (1 << fopAc_attn_ETC_e),

    /* 0x00000100 */fopAc_AttnFlag_CHECK_e  = (1 << fopAc_attn_CHECK_e),

    /* 0x00200000 */ fopAc_AttnFlag_UNK_0x200000   = 0x200000,
    /* 0x00400000 */ fopAc_AttnFlag_UNK_0x400000   = 0x400000,
    /* 0x00800000 */ fopAc_AttnFlag_UNK_0x800000   = 0x800000,
    /* 0x02000000 */ fopAc_AttnFlag_NOTALK_e       = 0x2000000,
    /* 0x04000000 */ fopAc_AttnFlag_ENEMY_NOLOCK_e = 0x4000000,
    /* 0x08000000 */ fopAc_AttnFlag_UNK_0x8000000  = 0x8000000,
    /* 0x20000000 */ fopAc_AttnFlag_TALKCHECK_e    = 0x20000000,
    /* 0x40000000 */ fopAc_AttnFlag_TALKREAD_e     = 0x40000000,
};

#define fopAc_AttnFlags_LOCK   (fopAc_AttnFlag_LOCK_e | fopAc_AttnFlag_TALK_e | fopAc_AttnFlag_BATTLE_e)
#define fopAc_AttnFlags_ACTION (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_CARRY_e | fopAc_AttnFlag_DOOR_e | fopAc_AttnFlag_JUEL_e | fopAc_AttnFlag_ETC_e)

class JKRSolidHeap;

enum dEvt_Command_e {
    dEvtCmd_NONE_e,
    dEvtCmd_INTALK_e,
    dEvtCmd_INDEMO_e,
    dEvtCmd_INDOOR_e,
    dEvtCmd_INGETITEM_e,
    dEvtCmd_INCATCH_e = 6,
    dEvtCmd_DUMMY = 0xFFFF,
};

enum dEvt_Condition_e {
    dEvtCnd_NONE_e = 0x0000,
    dEvtCnd_CANTALK_e = 0x0001,
    dEvtCnd_CANDEMO_e = 0x0002,
    dEvtCnd_CANDOOR_e = 0x0004,
    dEvtCnd_CANGETITEM_e = 0x0008,
    dEvtCnd_CANTALKITEM_e = 0x0020,
    dEvtCnd_40_e = 0x0040,
    dEvtCnd_DUMMY = 0x8000,
};

class dEvt_info_c {
public:
    dEvt_info_c();
    virtual ~dEvt_info_c() {}
    void setEventName(char*);
    char* getEventName();
    void beforeProc();
    void onCondition(u16 cond) { mCondition |= cond; }
    void offCondition(u16 cond) { mCondition &= ~cond; }
    BOOL checkCommandCatch() { return mCommand == dEvtCmd_INCATCH_e; }
    BOOL checkCommandDoor() { return mCommand == dEvtCmd_INDOOR_e; }
    BOOL checkCommandDemoAccrpt() { return mCommand == dEvtCmd_INDEMO_e; }
    BOOL checkCommandTalk() { return mCommand == dEvtCmd_INTALK_e; }
    BOOL checkCommandItem() { return mCommand == dEvtCmd_INGETITEM_e; }

    void setCommand(u16 command) { mCommand = command; }
    void setMapToolId(u8 id) { mMapToolId = id; }
    void setEventId(s16 id) { mEventId = id; }
    void setCondition(u16 condition) { mCondition = condition; }
    u16 getCondition() { return mCondition; }
    void setArchiveName(char* name) { mArchiveName = name; }
    u8 getMapToolId() { return mMapToolId; }
    s16 getEventId() { return mEventId; }
    s16 getIdx() { return (s8)mIndex; }
    void setIdx(u8 i_idx) { mIndex = i_idx; }
    char* getArchiveName() { return mArchiveName; }
    BOOL chkCondition(u16 condition) { return (mCondition & condition) == condition; }

    void suspendProc(void* actor) {
        if (field_0x10 != NULL) {
            field_0x14(actor);
        }
    }

    /* 0x04 */ u16 mCommand;
    /* 0x06 */ u16 mCondition;
    /* 0x08 */ s16 mEventId;
    /* 0x0A */ u8 mMapToolId;
    /* 0x0B */ u8 mIndex;
    /* 0x0C */ char* mArchiveName;
    /* 0x10 */ u8 field_0x10;
    /* 0x14 */ void (*field_0x14)(void*);
};  // Size = 0x18

struct actor_place {
    /* 0x00 */ cXyz pos;
    /* 0x0C */ csXyz angle;
    /* 0x12 */ s8 roomNo;
    /* 0x13 */ u8 field_0x13;
};

struct actor_attention_types {
    /* 0x00 */ u8 distances[fopAc_attn_MAX_e];
    /* 0x0A */ s16 field_0xa;
    /* 0x0C */ cXyz position;
    /* 0x18 */ u32 flags;
};  // Size = 0x1C

class dJntCol_c;

struct cull_sphere {
    /* 0x0 */ Vec center;
    /* 0xC */ f32 radius;
};

struct cull_box {
    /* 0x0 */ Vec min;
    /* 0xC */ Vec max;
};

class fopAc_ac_c {
public:
    /* 0x000 */ leafdraw_class base;
    /* 0x0C0 */ int actor_type;
    /* 0x0C4 */ create_tag_class actor_tag;
    /* 0x0D8 */ create_tag_class draw_tag;
    /* 0x0EC */ profile_method_class* sub_method;
    /* 0x0F0 */ JKRSolidHeap* heap;
    /* 0x0F4 */ dEvt_info_c eventInfo;
    /* 0x10C */ dKy_tevstr_c tevStr;
    /* 0x494 */ u16 setID;
    /* 0x496 */ u8 group;
    /* 0x497 */ u8 cullType;
    /* 0x498 */ u8 demoActorID;
    /* 0x499 */ s8 argument;
    /* 0x49A */ u8 carryType;
    /* 0x49C */ u32 actor_status;
    /* 0x4A0 */ u32 actor_condition;
    /* 0x4A4 */ fpc_ProcID parentActorID;
    /* 0x4A8 */ actor_place home;
    /* 0x4BC */ actor_place old;
    /* 0x4D0 */ actor_place current;
    /* 0x4E4 */ csXyz shape_angle;
    /* 0x4EC */ cXyz scale;
    /* 0x4F8 */ cXyz speed;
    /* 0x504 */ MtxP cullMtx;
    union {
        /* 0x508 */ cull_box box;
        /* 0x508 */ cull_sphere sphere;
    } cull;
    /* 0x520 */ f32 cullSizeFar;
    /* 0x524 */ J3DModel* model;
    /* 0x528 */ dJntCol_c* jntCol;
    /* 0x52C */ f32 speedF;
    /* 0x530 */ f32 gravity;
    /* 0x534 */ f32 maxFallSpeed;
    /* 0x538 */ cXyz eyePos;
    /* 0x544 */ actor_attention_types attention_info;
    /* 0x560 */ s16 field_0x560;
    /* 0x562 */ s16 health;
    /* 0x564 */ u8 field_0x564;
    /* 0x565 */ u8 field_0x565;
    /* 0x566 */ s8 field_0x566;
    /* 0x567 */ s8 field_0x567;

    fopAc_ac_c();
    ~fopAc_ac_c();

    static void setStopStatus(u32 status) { stopStatus = status; }

    static u32 stopStatus;
};  // Size: 0x568

STATIC_ASSERT(sizeof(fopAc_ac_c) == 0x568);

class fopEn_enemy_c : public fopAc_ac_c {
public:
    enum fopEn_flag {
        fopEn_flag_Down            = 0x1,
        fopEn_flag_CutDownHit      = 0x2,
        fopEn_flag_WolfDownStart   = 0x4,
        fopEn_flag_Dead            = 0x8,
        fopEn_flag_WolfDownPull    = 0x10,
        fopEn_flag_WolfDownPullEnd = 0x20,
        fopEn_flag_WolfBiteDamage  = 0x40,
        fopEn_flag_HeadLock        = 0x80,
        
        fopEn_flag_WolfNoLock      = 0x200,
    };

    enum fopEn_throwMode {
        fopEn_throwMode_Catch      = 0x2,
        fopEn_throwMode_Dash       = 0x4,
        fopEn_throwMode_ThrowLeft  = 0x8,
        fopEn_throwMode_ThrowRight = 0x10,
    };

    BOOL initBallModel();
    bool checkBallModelDraw();
    void setBallModelEffect(dKy_tevstr_c*);
    void drawBallModel(dKy_tevstr_c*);

    BOOL checkWolfNoLock() const { return mFlags & fopEn_flag_WolfNoLock; }
    BOOL checkHeadLockFlg() const { return mFlags & fopEn_flag_HeadLock; }
    BOOL checkWolfBiteDamage() const { return mFlags & fopEn_flag_WolfBiteDamage; }
    BOOL checkWolfDownPullFlg() const { return mFlags & fopEn_flag_WolfDownPull; }
    BOOL checkDownFlg() { return mFlags & fopEn_flag_Down; }
    BOOL checkCutDownHitFlg() const { return mFlags & fopEn_flag_CutDownHit; }
    BOOL checkWolfDownStartFlg() const { return mFlags & fopEn_flag_WolfDownStart; }
    BOOL checkDeadFlg() const { return mFlags & fopEn_flag_Dead; }
    BOOL checkThrowMode(u8 param_1) const { return mThrowMode & param_1; }

    u32* getMidnaBindID(int i_idx) { return mMidnaBindID + i_idx; }
    u8 getMidnaBindMode() { return mMidnaBindMode; }
    cXyz& getDownPos() { return mDownPos; }
    cXyz& getHeadLockPos() { return mHeadLockPos; }

    void onCutDownHitFlg() { mFlags |= fopEn_flag_CutDownHit; }
    void onWolfBiteDamage() { mFlags |= fopEn_flag_WolfBiteDamage; }
    void onWolfDownStartFlg() { mFlags |= (fopEn_flag_WolfDownPull | fopEn_flag_WolfDownStart); }
    void onWolfDownPullEndFlg() { mFlags |= fopEn_flag_WolfDownPullEnd; }
    void onWolfNoLock() { mFlags |= (u16)fopEn_flag_WolfNoLock; }
    void onDownFlg() { mFlags |= (u16)fopEn_flag_Down; }
    void onHeadLockFlg() { mFlags |= (u16)fopEn_flag_HeadLock; }

    #if DEBUG
    void offWolfBiteDamage() { mFlags &= (u16)~fopEn_flag_WolfBiteDamage; }
    void offCutDownHitFlg() { mFlags &= (u16)~fopEn_flag_CutDownHit; }
    void offWolfDownPullFlg() { mFlags &= ~fopEn_flag_WolfDownPull; }
    void offDownFlg() { mFlags &= (u16)~(fopEn_flag_WolfDownPull | fopEn_flag_WolfDownStart | fopEn_flag_CutDownHit | fopEn_flag_Down); }
    void offWolfNoLock() { mFlags &= (u16)~fopEn_flag_WolfNoLock; }
    void offHeadLockFlg() { mFlags &= (u16)~fopEn_flag_HeadLock; }
    void offThrowMode(u8 throwMode) { mThrowMode &= (u8)~throwMode; }
    #else
    void offWolfBiteDamage() { mFlags &= ~fopEn_flag_WolfBiteDamage; }
    void offCutDownHitFlg() { mFlags &= ~fopEn_flag_CutDownHit; }
    void offWolfDownPullFlg() { mFlags &= ~fopEn_flag_WolfDownPull; }
    void offDownFlg() { mFlags &= ~(fopEn_flag_WolfDownPull | fopEn_flag_WolfDownStart | fopEn_flag_CutDownHit | fopEn_flag_Down); }
    void offWolfNoLock() { mFlags &= ~fopEn_flag_WolfNoLock; }
    void offHeadLockFlg() { mFlags &= ~fopEn_flag_HeadLock; }
    void offThrowMode(u8 throwMode) { mThrowMode &= ~(throwMode & 0xFF); }
    #endif

    void setMidnaBindMode(u8 i_bindMode) { mMidnaBindMode = i_bindMode; }
    void setMidnaBindID(u8 i_idx, u32 i_bindID) { mMidnaBindID[i_idx] = i_bindID; }
    void setThrowModeCatch() { mThrowMode |= fopEn_throwMode_Catch; }
    void setThrowModeDash() { mThrowMode |= fopEn_throwMode_Dash; }
    void setThrowModeThrowRight() { mThrowMode |= fopEn_throwMode_ThrowRight; }
    void setThrowModeThrowLeft() { mThrowMode |= fopEn_throwMode_ThrowLeft; }
    void setDownPos(const cXyz* i_pos) { mDownPos = *i_pos; }
    void setHeadLockPos(const cXyz* i_pos) { mHeadLockPos = *i_pos; }

    /* 0x568 */ cXyz mDownPos;
    /* 0x574 */ cXyz mHeadLockPos;
    /* 0x580 */ J3DModel* mBallModel;
    /* 0x584 */ J3DAnmTextureSRTKey* mBtk;
    /* 0x588 */ J3DAnmTevRegKey* mBrk;
    /* 0x58C */ u8 field_0x58c;
    /* 0x58D */ u8 mThrowMode;
    /* 0x58E */ u16 mFlags;
    /* 0x590 */ f32 mAnmFrame;
    /* 0x594 */ u32 mEffectID1;
    /* 0x598 */ u32 mEffectID2;
    /* 0x59C */ u32 mMidnaBindID[3];
    /* 0x5A8 */ u8 mMidnaBindMode;
};  // Size: 0x5AC

BOOL fopAc_IsActor(void* i_actor);

extern actor_method_class g_fopAc_Method;

#endif
