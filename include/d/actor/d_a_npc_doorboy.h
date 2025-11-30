#ifndef D_A_NPC_DOORBOY_H
#define D_A_NPC_DOORBOY_H

#include "d/actor/d_a_npc4.h"
#include "d/d_msg_object.h"

/**
 * @ingroup actors-npcs
 * @class daNpcDoorBoy_c
 * @brief Door Boy (This isn't Soal?)
 *
 * @details
 *
*/

struct daNpcDoorBoy_HIOParam {
    /* 0x0 */ daNpcF_HIOParam common;
};

class daNpcDoorBoy_c : public daNpcF_c {
public:
    typedef bool (daNpcDoorBoy_c::*actionFunc)(void*);
    typedef BOOL (daNpcDoorBoy_c::*EventFn)(int);

    daNpcDoorBoy_c();
    ~daNpcDoorBoy_c();
    cPhs__Step Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void setMotion(int, f32, int);
    void reset();
    inline BOOL setAction(bool (daNpcDoorBoy_c::*)(void*));
    bool wait(void*);
    bool fear(void*);
    bool talk(void*);
    bool demo(void*);
    bool dummyTalk(void*);
    void setParam();
    BOOL main();
    void setAttnPos();
    inline void lookat();
    void setMotionAnm(int, f32);
    BOOL drawDbgInfo();
    void adjustShapeAngle() {}

    inline u16 getMessageNo() { return fopAcM_GetParam(this) >> 8; }
    inline void playMotion();
    inline bool chkFindPlayer();
    inline void setLookMode(int);
    inline BOOL step(s16, int);
    inline s16 dMsgObject_getNowTalkFlowNo() { return dMsgObject_getMsgObjectClass()->getNowTalkFlowNo(); }
    inline int getTimeHour();
    inline bool isDummyTalk();

    static EventFn mEvtSeqList[1];

private:
    /* 0xB48 */ Z2CreatureCitizen mSound;
    /* 0xBEC */ u8 field_0xbec[0xbf0 - 0xbec];
    /* 0xBF0 */ daNpcF_Lookat_c mLookat;
    /* 0xC8C */ daNpcF_ActorMngr_c mActorMngr[1];
    /* 0xC95 */ u8 field_0xc94[0xc98 - 0xc94];
    /* 0xC98 */ dCcD_Cyl field_0xc98;
    /* 0xDD4 */ actionFunc mAction;
    /* 0xDE0 */ request_of_phase_process_class mPhases[2];
    /* 0xDF0 */ fpc_ProcID field_0xdf0;
    /* 0xDF4 */ int field_0xdf4;
    /* 0xDF8 */ int field_0xdf8;
    /* 0xDFC */ int mMessageNo;
    /* 0xE00 */ s16 mLookMode;
    /* 0xE02 */ s16 field_0xe02;
    /* 0xE04 */ u16 mMode;
    /* 0xE06 */ u8 field_0xe06;
};

STATIC_ASSERT(sizeof(daNpcDoorBoy_c) == 0xe08);

class daNpcDoorBoy_Param_c {
public:
    virtual ~daNpcDoorBoy_Param_c() {}

    static daNpcDoorBoy_HIOParam const m;
};


#endif /* D_A_NPC_DOORBOY_H */
