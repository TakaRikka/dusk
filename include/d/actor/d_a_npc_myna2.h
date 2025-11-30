#ifndef D_A_NPC_MYNA2_H
#define D_A_NPC_MYNA2_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_myna2_c
 * @brief Plumm
 *
 * @details
 *
 */
class daNpc_myna2_c : public daNpcF_c {
public:
    typedef BOOL (daNpc_myna2_c::*ActionFn)(void*);
    typedef BOOL (daNpc_myna2_c::*EventFn)(int);

    daNpc_myna2_c();
    virtual ~daNpc_myna2_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getTypeFromParam();
    int isDelete();
    void reset();
    void playMotion();
    int chkAction(int (daNpc_myna2_c::*)(void*));
    int setAction(int (daNpc_myna2_c::*)(void*));
    int selectAction();
    void doNormalAction(int);
    BOOL doEvent();
    void setLookMode(int);
    int wait(void*);
    int waitHovering(void*);
    int waitFailure(void*);
    int waitGoal(void*);
    int talk(void*);
    int test(void*);
    int ECut_firstTalk(int);
    int ECut_gameFailure(int);
    int ECut_gameGoal(int);
    int ECut_gameGoalSuccess(int);
    void calcHovering(int, int);

    virtual void setParam();
    virtual BOOL main();
    virtual void setAttnPos();
    virtual bool setExpressionBtp(int);
    virtual void setMotionAnm(int, f32);
    virtual void setMotion(int, f32, int);
    virtual BOOL drawDbgInfo();
    virtual void drawOtherMdls();

    int getType() { return mType; }

    static char* mEvtCutNameList[5];
    static EventFn mEvtCutList[];

    /* 0xB48 */ u8 field_0xB48[0xB4C - 0xB48];
    /* 0xB4C */ daNpcF_Lookat_c mLookat;
    /* 0xBE8 */ Z2CreatureSumomo mSound;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMgrs[1];
    /* 0xC80 */ u8 field_0xC80[0xC84 - 0xC80];
    /* 0xC84 */ dCcD_Cyl mCyl;
    /* 0xDC0 */ ActionFn mNextAction;
    /* 0xDCC */ ActionFn mAction;
    /* 0xDD8 */ request_of_phase_process_class mPhase[3];
    /* 0xDF0 */ fpc_ProcID mItemPid;
    /* 0xDF4 */ int field_0xdf4;
    /* 0xDF8 */ int field_0xdf8;
    /* 0xDFC */ int field_0xdfc;
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ s16 mLookMode;
    /* 0xE0A */ u16 mMode;
    /* 0xE0C */ u8 mType;
    /* 0xE0D */ u8 field_0xe0d;
    /* 0xE10 */ cXyz field_0xe10;
    /* 0xE1C */ cXyz field_0xe1c;
    /* 0xE28 */ int field_0xe28;
    /* 0xE2C */ int field_0xe2c;
    /* 0xE30 */ f32 field_0xe30;
};

STATIC_ASSERT(sizeof(daNpc_myna2_c) == 0xe34);

struct daNpc_myna2_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 field_0x6c;
    /* 0x70 */ f32 field_0x70;
};

class daNpc_myna2_Param_c {
public:
    virtual ~daNpc_myna2_Param_c() {}

    static const daNpc_myna2_HIOParam m;
};


#endif /* D_A_NPC_MYNA2_H */
