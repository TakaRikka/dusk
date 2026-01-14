#ifndef D_A_PERU_H
#define D_A_PERU_H

#include "d/actor/d_a_tag_evtarea.h"
#include "dolphin/types.h"
#include "d/actor/d_a_npc.h"
#include "SSystem/SComponent/c_counter.h"

//TODO: this might be the same struct that's used in d_a_obj_sekizoa's HIO
struct daPeru_HIO_inner {
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3C;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ s16 field_0x48;
    /* 0x4A */ s16 field_0x4A;
    /* 0x4C */ s16 field_0x4C;
    /* 0x4E */ s16 field_0x4E;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5C;
    /* 0x60 */ s16 field_0x60;
    /* 0x62 */ s16 field_0x62;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ f32 field_0x68;
    /* 0x6C */ f32 field_0x6C;
    /* 0x70 */ f32 field_0x70;
    /* 0x74 */ f32 field_0x74;
    /* 0x78 */ f32 field_0x78;
    /* 0x7C */ f32 field_0x7C;
    /* 0x80 */ f32 field_0x80;
    /* 0x84 */ f32 field_0x84;
    /* 0x88 */ f32 field_0x88;
};

struct daPeru_HIOParam {
    /* 0x00 */ daPeru_HIO_inner inner;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
};

class daPeru_Param_c {
public:
    virtual ~daPeru_Param_c() {}

    static daPeru_HIOParam const m;
};

#if DEBUG
class daPeru_HIO_c : public mDoHIO_entry_c {
public:
    daPeru_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daPeru_HIOParam m;
};

#define PERU_HIO_CLASS daPeru_HIO_c
#else
#define PERU_HIO_CLASS daPeru_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daPeru_c
 * @brief Louise
 *
 * @details
 *
 */
class daPeru_c : public daNpcT_c {
public:
    typedef int (daPeru_c::*cutAppearFunc)(int);
    typedef int (daPeru_c::*actionFunc)(int);

    virtual ~daPeru_c();
    int create();
    int CreateHeap();
    int typeInitialize();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    int isDelete();
    void reset();
    void setParam();
    void setAfterTalkMotion();
    void srchActors();
    int evtTalk();
    int evtCutProc();
    void action();
    void setAttnPos();
    void setCollision();
    BOOL drawDbgInfo();
    int setAction(actionFunc, int);
    int wait(int);
    BOOL is_AppearDemo_start();
    void _AppearDemoTag_delete();
    int talk(int);
    int jump_st(int);
    int jump_ed(int);
    int sniff(int);
    int demo_appear(int);
    int demo_walk_to_link(int);
    int demo_walk_circle(int);
    int demo_walk_to_window(int);
    int demo_walk_to_pathway(int);
    int cutAppear(int);
    int _cutAppear_Init(int const&);
    int _cutAppear_Main(int const&);
    void _catdoor_open();
    void _catdoor_open_demoskip();
    int cutAppear_skip(int);
    int _cutAppear_skip_Init(int const&);
    int _cutAppear_skip_Main(int const&);
    daPeru_c(daNpcT_faceMotionAnmData_c const* param_1,
                            daNpcT_motionAnmData_c const* param_2,
                            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                            daNpcT_evtData_c const* param_7, char** param_8)
        : daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8) {
            OS_REPORT("|%06d:%x|daPeru_c -> コンストラクト\n", g_Counter.mCounter0, this);
        }
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    int checkChangeJoint(int param_1) { return param_1 == 4; }
    int checkRemoveJoint(int param_1) { return param_1 == 5; }
    void beforeMove() {
        if (checkHide() || mNoDraw) {
            attention_info.flags = 0;
        }
    }

    u8 getType() {
        u8 type = fopAcM_GetParam(this);
        if (type == 0xff) {
            type = 0;
        }
        return type;
    }

    int getFlowNodeNo() {
        u16 flowNode = home.angle.x;
        if (flowNode == 0xffff) {
            return -1;
        }
        return flowNode;
    }

    bool chkAction(actionFunc func) { return mActionFunc == func; }
    void _AppearDemoStart() { mEvtNo = 1; }

    BOOL decTimer() {
        field_0xe6c--; 
        return field_0xe6c <= 0;
    }

    BOOL decCnt() {
        field_0xe6a--; 
        return field_0xe6a <= 0;
    }

    void setTimer(int timer) { field_0xe6c = timer; }
    void setCnt(int cnt) { field_0xe6a = cnt; }
    int getCnt() { return field_0xe6a; }

    static const char* mCutNameList[3];
    static cutAppearFunc mCutList[3];

private:
    /* 0x0E40 */ daNpcT_ActorMngr_c mActors[3];
    /* 0x0E58 */ int field_0xe58;
    /* 0x0E5C */ actionFunc mActionFunc;
    /* 0x0E68 */ s16 field_0xe68;
    /* 0x0E6A */ s16 field_0xe6a;
    /* 0x0E6C */ s16 field_0xe6c;
    /* 0x0E6E */ s16 field_0xe6e;
    /* 0x0E70 */ u8 field_0xe70[0xe7c - 0xe70];
    /* 0x0E7C */ s8 field_0xe7c;
    /* 0x0E80 */ int field_0xe80;
    /* 0x0E84 */ PERU_HIO_CLASS* mpHIO;
    /* 0x0E88 */ dCcD_Cyl mCyls[2];
    /* 0x1100 */ daNpcT_Path_c mPath;
    /* 0x1128 */ u8 mType;
    /* 0x1129 */ u8 field_0x1129;
};

STATIC_ASSERT(sizeof(daPeru_c) == 0x112c);

#endif /* D_A_PERU_H */
