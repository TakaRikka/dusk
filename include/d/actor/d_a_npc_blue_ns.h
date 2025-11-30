#ifndef D_A_NPC_BLUE_NS_H
#define D_A_NPC_BLUE_NS_H

#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_tag_yami.h"
#include "d/actor/d_a_obj_carry.h"

/**
 * @ingroup actors-npcs
 * @class daNpcBlueNS_c
 * @brief Shadow Beast (Twili)
 *
 * @details
 *
 */
class daNpcBlueNS_c : public daNpcF_c {
public:
    typedef BOOL (daNpcBlueNS_c::*EventFn)(int);

    daNpcBlueNS_c();
    
    int Create();
    int CreateHeap();
    int isDelete();
    int Delete();
    int Execute();
    int _Execute();
    void callEvt_changeYami();
    fpc_ProcID create_Yamijin(int);
    BOOL col_check();
    BOOL holyball_check_main(fopAc_ac_c*);
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void srchActor();
    void srchHollyBall();
    void srchTagYami();
    static void* _search_Tag(void*, void*);
    static void* _search_HollyBall_Y(void*, void*);
    static void* _search_HollyBall_G(void*, void*);
    void reset();
    void playMotion();
    int setAction(int (daNpcBlueNS_c::*)(int));
    void setLookMode(int);
    void lookat();
    BOOL step(s16, int);
    BOOL chkFindPlayer();
    int wait(int);
    int down(int);
    int talk(int);
    int demo(int);
    void ChgPtclDisp();
    void downSE();
    void lightsowrdSE();
    void appearSE();
    BOOL _Evt_ChgYami_Chibi(int);
    BOOL _Evt_ChgYami_Chibi_CutInit(const int&);
    BOOL _Evt_ChgYami_Chibi_CutMain(const int&);
    BOOL _Evt_ChgYami_Debu(int);
    BOOL _Evt_ChgYami_Debu_CutInit(const int&);
    BOOL _Evt_ChgYami_Debu_CutMain(const int&);
    BOOL _Evt_ChgYami_Noppo(int);
    BOOL _Evt_ChgYami_Noppo_CutInit(const int&);
    BOOL _Evt_ChgYami_Noppo_CutMain(const int&);
    BOOL _Evt_ChgYami_STNoppo(int);
    BOOL _Evt_ChgYami_STNoppo_CutInit(const int&);
    BOOL _Evt_ChgYami_STNoppo_CutMain(const int&);
    
    virtual ~daNpcBlueNS_c();
    virtual void setParam();
    virtual BOOL main();
    virtual void adjustShapeAngle() {}
    virtual void setAttnPos();
    virtual void setMotionAnm(int, f32);
    virtual void setMotion(int, f32, int);
    virtual BOOL drawDbgInfo();

    int getType() { return fopAcM_GetParam(this) & 0xFF; }
    u8 getSwitchBitNo() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }
    u8 _chk_TagPrm(fopAc_ac_c* i_tag) { return (fopAcM_GetParam(i_tag) & 0xFF) == mType % 3; }

    BOOL holyball_check_yellow() { return holyball_check_main(mpHolyBallY); }
    BOOL holyball_check_green() { return holyball_check_main(mpHolyBallG); }
    BOOL holyball_check() { return holyball_check_yellow() || holyball_check_green(); }

    int getFlowNodeNo() {
        int var_r30;

        u16 temp_r0 = home.angle.x;
        if (temp_r0 == 0xFFFF) {
            var_r30 = -1;
        } else {
            var_r30 = temp_r0;
        }

        return var_r30;
    }

    static EventFn mEvtSeqList[];

    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ u8 field_0xBD8[0xBDC - 0xBD8];
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngr[1];
    /* 0xC80 */ u8 field_0xC80[0xC84 - 0xC80];
    /* 0xC84 */ dCcD_Cyl mCyl;
    /* 0xDC0 */ u16 field_0xdc0;
    /* 0xDC4 */ int (daNpcBlueNS_c::*mAction)(int);
    /* 0xDD0 */ request_of_phase_process_class mPhase[1];
    /* 0xDD8 */ int field_0xdd8;
    /* 0xDDC */ int field_0xddc;
    /* 0xDE0 */ int field_0xde0;
    /* 0xDE4 */ int mFlowNodeNo;
    /* 0xDE8 */ s16 mLookMode;
    /* 0xDEA */ s16 field_0xdea;
    /* 0xDEC */ u8 field_0xdec;
    /* 0xDF0 */ daObjCarry_c* mpHolyBallY;
    /* 0xDF4 */ daObjCarry_c* mpHolyBallG;
    /* 0xDF8 */ daTagYami_c* mpTagYami;
    /* 0xDFC */ fopAc_ac_c* mpNpcYami;
    /* 0xE00 */ fpc_ProcID mYamijinId;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ int field_0xe08;
    /* 0xE0C */ int field_0xe0c;
    /* 0xE10 */ s8 field_0xe10;
    /* 0xE11 */ u8 mType;
    /* 0xE12 */ s8 field_0xe12;
};

STATIC_ASSERT(sizeof(daNpcBlueNS_c) == 0xe14);

struct daNpcBlueNS_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 field_0x6c;
};

class daNpcBlueNS_Param_c {
public:
    virtual ~daNpcBlueNS_Param_c() {}

    static const daNpcBlueNS_HIOParam m;
};


#endif /* D_A_NPC_BLUE_NS_H */
