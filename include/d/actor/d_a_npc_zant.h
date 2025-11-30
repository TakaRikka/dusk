#ifndef D_A_NPC_ZANT_H
#define D_A_NPC_ZANT_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Zant_c
 * @brief Zant
 *
 * @details
 *
 */
class daNpc_Zant_c : public daNpcT_c {
public:
    typedef int (daNpc_Zant_c::*cutFunc)(int);
    typedef int (daNpc_Zant_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_Zant_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    u32 getFlowNodeNo();
    int isDelete();
    void reset();
    void afterJntAnm(int);
    void setParam();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    void drawGhost();
    int selectAction();
    int chkAction(int (daNpc_Zant_c::*)(void*));
    int setAction(int (daNpc_Zant_c::*)(void*));
    int wait(void*);
    int talk(void*);
    daNpc_Zant_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                                daNpcT_evtData_c const* param_7, char** param_8) :
                                daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8) {}
    
    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ u8 field_0xe40[0xe44 - 0xe40];
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF81 */ u8 field_0xf81[0xf84 - 0xf81];
    /* 0xF84 */ actionFunc field_0xf84;
    /* 0xF90 */ actionFunc field_0xf90;
    /* 0xF9C */ int field_0xf9c;
};

STATIC_ASSERT(sizeof(daNpc_Zant_c) == 0xfa0);

class daNpc_Zant_Param_c {
public:
    virtual ~daNpc_Zant_Param_c() {}
    struct Data {
        /* 0x00 */ f32 field_0x0;
        /* 0x04 */ f32 field_0x4;
        /* 0x08 */ f32 field_0x8;
        /* 0x0C */ f32 field_0xc;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ f32 field_0x18;
        /* 0x1C */ f32 field_0x1c;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 field_0x24;
        /* 0x28 */ f32 field_0x28;
        /* 0x2C */ f32 field_0x2c;
        /* 0x30 */ f32 field_0x30;
        /* 0x34 */ f32 field_0x34;
        /* 0x38 */ f32 field_0x38;
        /* 0x3C */ f32 field_0x3c;
        /* 0x40 */ f32 field_0x40;
        /* 0x44 */ f32 field_0x44;
        /* 0x48 */ s16 field_0x48;
        /* 0x4A */ s16 field_0x4a;
        /* 0x4C */ s16 field_0x4c;
        /* 0x4E */ s16 field_0x4e;
        /* 0x50 */ f32 field_0x50;
        /* 0x54 */ f32 field_0x54;
        /* 0x58 */ f32 field_0x58;
        /* 0x5C */ f32 field_0x5c;
        /* 0x60 */ s16 field_0x60;
        /* 0x62 */ s16 field_0x62;
        /* 0x64 */ int field_0x64;
        /* 0x68 */ int field_0x68;
        /* 0x6C */ f32 field_0x6c;
        /* 0x70 */ f32 field_0x70;
        /* 0x74 */ f32 field_0x74;
        /* 0x78 */ f32 field_0x78;
        /* 0x7C */ f32 field_0x7c;
        /* 0x80 */ f32 field_0x80;
        /* 0x84 */ f32 field_0x84;
        /* 0x88 */ f32 field_0x88;
    };

    static const Data m;
};

class daNpc_Zant_HIO_c {
public:
    virtual ~daNpc_Zant_HIO_c() {}

#if DEBUG
    daNpc_Zant_Param_c::Data param;
#endif
};

#endif /* D_A_NPC_ZANT_H */
