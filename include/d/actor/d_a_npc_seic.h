#ifndef D_A_NPC_SEIC_H
#define D_A_NPC_SEIC_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_seiC_c
 * @brief Eldin Spirit
 *
 * @details
 *
 */

 struct daNpc_seiC_HIOParam {
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 mGravity;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 mSttsWeight;
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
    /* 0x90 */ f32 field_0x90;
};

class daNpc_seiC_Param_c {
    public:
        virtual ~daNpc_seiC_Param_c() {};

        static const daNpc_seiC_HIOParam m;
};

class daNpc_seiC_HIO_c
#if DEBUG
: public mDoHIO_entry_c 
#endif
{
public:
    void genMessage(JORMContext*);

    #if DEBUG
    /* 0x08 */ daNpc_seiC_HIOParam field_0x8;
    /* 0x94 */ f32 field_0x94;                  // "強制会話距離" "Forced conversation distance" | Slider
    /* 0x98 */ f32 field_0x98;                  // "会話距離" "Conversation distance" | Slider
    #endif
};

class daNpc_seiC_c : public daNpcT_c {
public:
    typedef int (daNpc_seiC_c::*cutFunc)(int);
    typedef int (daNpc_seiC_c::*actionFunc)(void*);
    

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_seiC_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getType();
    u32 getFlowNodeNo();
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
    int chkAction(int (daNpc_seiC_c::*)(void*));
    int setAction(int (daNpc_seiC_c::*)(void*));
    void ctrlWaitAnm();
    int wait(void*);
    int talk(void*);
    daNpc_seiC_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData, daNpcT_motionAnmData_c const* i_motionAnmData,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData, int i_faceMotionStepNum,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
                                daNpcT_evtData_c const* i_evtData, char** i_arcNames)         
                                : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                                           i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                                           i_arcNames) {};

    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ daNpc_seiC_HIO_c* field_0xe40;
    /* 0xE44 */ u8 mType;
    /* 0xE45 */ u8 field_0xe45[0xe48 - 0xe45];
    /* 0xE48 */ actionFunc mAction;
    /* 0xE54 */ actionFunc mAction2;
    /* 0xE60 */ int field_0xe60;
};

STATIC_ASSERT(sizeof(daNpc_seiC_c) == 0xe64);

#endif /* D_A_NPC_SEIC_H */
