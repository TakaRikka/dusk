#ifndef D_A_NPC_PRAYER_H
#define D_A_NPC_PRAYER_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpcPray_c
 * @brief Charlo
 *
 * @details
 *
 */
class daNpcPray_c : public daNpcF_c {
public:
    typedef BOOL (daNpcPray_c::*EvtSeq)(int);

    daNpcPray_c();
    int Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void reset();
    void playMotion();
    int setAction(bool (daNpcPray_c::*)(void*));
    void setLookMode(int);
    void lookat();
    bool step(s16, int);
    bool chkFindPlayer();
    bool wait(void*);
    bool fear(void*);
    bool talk(void*);
    bool demo(void*);
    fpc_ProcID createHeart();
    BOOL _Evt_GetHeart(int);
    BOOL _Evt_GetHeart_CutInit(int const&);
    BOOL _Evt_GetHeart_CutMain(int const&);
    
    virtual ~daNpcPray_c();
    virtual void setParam();
    virtual BOOL main();
    virtual void adjustShapeAngle() {}
    virtual void setAttnPos();
    virtual void setMotionAnm(int, f32);
    virtual void setMotion(int, f32, int);
    virtual BOOL drawDbgInfo();

    s16 getMessageNo() { return (fopAcM_GetParam(this) >> 8) & 0xFFFF; } 

    static EvtSeq mEvtSeqList[];

private:
    /* 0xB48 */ Z2CreatureCitizen mSound;
    /* 0xBEC */ u8 field_0xBEC[0xBF0 - 0xBEC];
    /* 0xBF0 */ daNpcF_Lookat_c mLookat;
    /* 0xC8C */ daNpcF_ActorMngr_c mActorMngr[2];
    /* 0xC9C */ u8 field_0xC9C[0xCA0 - 0xC9C];
    /* 0xCA0 */ dCcD_Cyl mCcCyl;
    /* 0xDDC */ bool (daNpcPray_c::*mAction)(void*);
    /* 0xDE8 */ request_of_phase_process_class mPhase[2];
    /* 0xDF8 */ int field_0xdf8;
    /* 0xDFC */ int field_0xdfc;
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ int mMessageNo;
    /* 0xE08 */ s16 mLookMode;
    /* 0xE0A */ s16 field_0xe0a;
    /* 0xE0C */ u16 mActionMode;
    /* 0xE0E */ u8 field_0xe0e;
};

STATIC_ASSERT(sizeof(daNpcPray_c) == 0xe10);

struct daNpcPray_HIOParam {
    daNpcF_HIOParam common;
};

class daNpcPray_Param_c {
public:
    virtual ~daNpcPray_Param_c() {}

    static const daNpcPray_HIOParam m;
};


#endif /* D_A_NPC_PRAYER_H */
