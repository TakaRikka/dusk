#ifndef D_A_NPC_GWOLF_H
#define D_A_NPC_GWOLF_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_GWolf_c
 * @brief Golden Wolf
 *
 * @details
 *
*/

struct daNpc_GWolf_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 attack_spd_horizontal;       // 攻撃速度横 - Attack Speed Horizontal
    /* 0x70 */ f32 attack_spd_vertical;         // 攻撃速度縦 - Attack Speed Vertical
    /* 0x74 */ f32 attack_spd_horizontal_horse; // 攻撃速度横(馬) - Attack Speed Horizontal (Horse)
    /* 0x78 */ f32 attack_spd_vertical_horse;   // 攻撃速度縦(馬) - Attack Speed Vertical (Horse)  
    /* 0x7C */ f32 demo_start_dist;             // デモ開始距離 - Demo Start Distance
    /* 0x80 */ f32 blur_scale;                  // ブラーのスケール - Blur Scale
    /* 0x84 */ f32 blur_opacity;                // ブラーの透明度 - Blur Opacity
    /* 0x88 */ f32 warp_start_dist;             // ワープ開始距離 - Warp Start Distance
};

class daNpc_GWolf_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpc_GWolf_HIOParam param;
};

class daNpc_GWolf_Param_c {
public:
    virtual ~daNpc_GWolf_Param_c() {}

    static daNpc_GWolf_HIOParam const m;
};

class daNpc_GWolf_c : public daNpcF_c {
public:
    typedef BOOL (daNpc_GWolf_c::*actionFunc)(void*);
    typedef BOOL (daNpc_GWolf_c::*cutFunc)(int);

    daNpc_GWolf_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int draw(int, int, f32, GXColorS10*, int);
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getTypeFromParam();
    int getModeFromParam();
    BOOL isDelete();
    void reset();
    void playMotion();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    BOOL selectAction();
    void doNormalAction(int);
    BOOL doEvent();
    void setLookMode(int);
    void lookat();
    BOOL ECut_attackWarp(int);
    BOOL ECut_attackWarpHorse(int);
    BOOL ECut_howlingSessionA(int);
    void meetGWolfSkip();
    BOOL ECut_meetGWolf(int);
    BOOL wait(void*);
    BOOL waitHowling(void*);
    BOOL waitBrave(void*);
    BOOL talk(void*);
    BOOL test(void*);
    s16 getGroundSlope(dBgS_ObjAcch&, s16);

    virtual ~daNpc_GWolf_c();
    virtual void setParam();
    virtual BOOL main();
    virtual BOOL ctrlBtk();
    virtual void adjustShapeAngle();
    virtual void setAttnPos();
    virtual bool setExpressionAnm(int, bool);
    virtual void setExpression(int, f32);
    virtual void setMotionAnm(int, f32);
    virtual void setMotion(int, f32, int);
    virtual BOOL drawDbgInfo();
    virtual void drawOtherMdls();

    int getSwBitFromParam() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }
    u16 getSaveTableFromParam() { return home.angle.z & 0xFF; }
    void setHowlingEndFlag() { field_0xe1c = 2; }
    void setHowlingFlag() { field_0xe1c = 1; }

    static char* mEvtCutNameList[5];
    static cutFunc mEvtCutList[5];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngrs[2];
    /* 0xC88 */ daNpc_GWolf_HIO_c* mHIO;
    /* 0xC8C */ dCcD_Cyl mCyl;
    /* 0xDC8 */ actionFunc mNextAction;
    /* 0xDD4 */ actionFunc mAction;
    /* 0xDE0 */ request_of_phase_process_class mPhases[3];
    /* 0xDF8 */ int field_0xdf8;
    /* 0xDFC */ int field_0xdfc;
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ int field_0xe08;
    /* 0xE0C */ s16 mLookMode;
    /* 0xE0E */ u16 mMode;
    /* 0xE10 */ u8 mType;
    /* 0xE11 */ s8 mParamMode;
    /* 0xE14 */ int mExitId;
    /* 0xE18 */ int mAnm;
    /* 0xE1C */ u8 field_0xe1c;
    /* 0xE1D */ u8 field_0xe1d;
    /* 0xE1E */ u8 swBit;
    /* 0xE1F */ u8 mSaveTable;
};

STATIC_ASSERT(sizeof(daNpc_GWolf_c) == 0xe20);

#endif /* D_A_NPC_GWOLF_H */
