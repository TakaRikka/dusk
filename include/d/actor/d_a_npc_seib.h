#ifndef D_A_NPC_SEIB_H
#define D_A_NPC_SEIB_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_seiB_c
 * @brief Faron Spirit
 *
 * @details
 *
 */

class daNpc_seiB_Param_c {
public:
    virtual ~daNpc_seiB_Param_c() {};
    struct Data {
        /* 0x00 */ f32 field_0x00;
        /* 0x04 */ f32 mGravity;
        /* 0x08 */ f32 mScale;
        /* 0x0C */ f32 field_0xc;
        /* 0x10 */ f32 mWeight;
        /* 0x14 */ f32 mCylH;
        /* 0x18 */ f32 mWallH;
        /* 0x1C */ f32 mWallR;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 field_0x24;
        /* 0x28 */ f32 field_0x28;
        /* 0x2C */ f32 field_0x2c;
        /* 0x30 */ f32 field_0x30;
        /* 0x34 */ f32 field_0x34;
        /* 0x38 */ f32 field_0x38;
        /* 0x3C */ f32 field_0x3c;
        /* 0x40 */ f32 field_0x40;
        /* 0x44 */ f32 mMorfFrames;
        /* 0x48 */ f32 field_0x48;
        /* 0x4C */ f32 field_0x4c;
        /* 0x50 */ f32 field_0x50;
        /* 0x54 */ f32 field_0x54;
        /* 0x58 */ f32 field_0x58;
        /* 0x5C */ f32 field_0x5c;
        /* 0x60 */ f32 field_0x60;
        /* 0x64 */ f32 field_0x64;
        /* 0x68 */ f32 field_0x68;
        /* 0x6C */ f32 field_0x6c;
        /* 0x70 */ f32 field_0x70;
        /* 0x74 */ f32 field_0x74;
        /* 0x78 */ f32 field_0x78;
        /* 0x7C */ f32 field_0x7c;
        /* 0x80 */ f32 field_0x80;
        /* 0x84 */ f32 field_0x84;
        /* 0x88 */ f32 field_0x88;
        /* 0x8C */ f32 field_0x8c;
        /* 0x90 */ f32 mDist;
    };

    static const Data m;
};
class daNpc_seiB_c : public daNpcT_c {
public:
    typedef int (daNpc_seiB_c::*cutFunc)(int);
    typedef int (daNpc_seiB_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_seiB_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getType();
    int getFlowNodeNo();
    int isDelete();
    void reset();
    void setParam();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    int drawDbgInfo();
    bool afterSetMotionAnm(int, int, f32, int);
    int selectAction();
    int chkAction(int (daNpc_seiB_c::*)(void*));
    int setAction(int (daNpc_seiB_c::*)(void*));
    void ctrlWaitAnm();
    int wait(void*);
    int talk(void*);
    daNpc_seiB_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                                daNpcT_evtData_c const* param_7, char** param_8) :
                                daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8)
                                {}

    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ daNpc_seiB_Param_c* mpParam;
    /* 0xE44 */ u8 mType;
    /* 0xE48 */ actionFunc mActionFunc1;
    /* 0xE54 */ actionFunc mActionFunc2;
    /* 0xE60 */ int mAnim;
    /* 0xE64 */ void* field_0xe64;
};

STATIC_ASSERT(sizeof(daNpc_seiB_c) == 0xe68);

#endif /* D_A_NPC_SEIB_H */
