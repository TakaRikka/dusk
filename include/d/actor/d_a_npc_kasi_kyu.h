#ifndef D_A_NPC_KASI_KYU_H
#define D_A_NPC_KASI_KYU_H

#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_tag_escape.h"

/**
 * @ingroup actors-npcs
 * @class daNpcKasiKyu_c
 * @brief Kili
 *
 * @details
 *
*/

struct daNpcKasiKyu_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ s16 escape_time;         // 逃げるまでの時間 - Escape Time
    /* 0x70 */ f32 escape_spd;          // 逃げる速度 - Escape Speed
};

class daNpcKasiKyu_HIO_c
#if DEBUG
: public mDoHIO_entry_c
#endif
{
    /* 0x8 */ daNpcKasiKyu_HIOParam param;
};

class daNpcKasiKyu_c : public daNpcF_c {
public:
    typedef int (daNpcKasiKyu_c::*actionFunc)(int);
    typedef BOOL (daNpcKasiKyu_c::*EventFn)(int);

    daNpcKasiKyu_c();
    ~daNpcKasiKyu_c();
    cPhs__Step Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void setParam();
    BOOL main();
    void setAttnPos();
    void setMotionAnm(int, f32);
    void setMotion(int, f32, int);
    BOOL drawDbgInfo();
    void reset();
    void playMotion();
    void playMotionAnmLoop(daNpcF_c::daNpcF_anmPlayData***);
    BOOL setAction(int (daNpcKasiKyu_c::*)(int));
    void setLookMode(int);
    void lookat();
    BOOL step(s16, int);
    BOOL chkFindPlayer();
    int wait(int);
    int fear(int);
    daTagEscape_c* srchWolfTag();
    static void* _srch_escape_tag(void*, void*);
    int getWolfPathNearIdx();
    int chace_st(int);
    int chace(int);
    cXyz getChacePos();
    int turn_link(int);
    int turn_home(int);
    int turn_center(int);
    int talk_dummy(int);
    int kya(int);
    int kya2(int);
    int kya_stop(int);
    int iyan(int);
    int iyan_look(int);
    int turn_hana(int);
    int escape(int);
    int wait_dummy(int);
    int cheer(int);
    BOOL _turn_to_link(s16);
    BOOL _turn_pos(cXyz const&, s16);
    BOOL actor_front_check(fopAc_ac_c*);
    void _getOffset(cXyz const&, cXyz&);
    void adjustShapeAngle() {}

    s8 getType() {
        s8 rv = fopAcM_GetParam(this) & 0xFF;
        
        if (rv == 0xFF) {
            rv = 0;
        }

        return rv;
    }
    int getMessageNo() { return home.angle.x; }
    u8 getRailNo() { return fopAcM_GetParam(this) >> 8; }
    void setSygnal(int i_sygnal) { mSygnal = i_sygnal; }
    void setChacePos(const cXyz& i_chacePos) { mChacePos = i_chacePos; }
    void setCenterPos(const cXyz& i_centerPos) { mCenterPos = i_centerPos; }
    void setPlPoint(const int& i_plPoint) { mPlPoint = i_plPoint; }
    void off_talked() { mTalked = false; }
    BOOL is_talked() { return mTalked; }
    BOOL is_fear() { return mFear; }
    BOOL is_escape() { return mEscape; }
    void chgWeightHeavy() { mCcStts.SetWeight(0xFE); }
    void chgWeightLight() { mCcStts.SetWeight(0xD8); }
    BOOL pl_front_check() { return actor_front_check(daPy_getPlayerActorClass()); }

    static EventFn mEvtSeqList[1];
    static daTagEscape_c* mTargetTag;
    static f32 mTargetTagDist;
    static s16 mWolfAngle;

private:
    /* 0x0B48 */ Z2CreatureCitizen mSound;
    /* 0x0BEC */ u8 field_0xbec[0xbf0 - 0xbec];
    /* 0x0BF0 */ daNpcF_Lookat_c mLookat;
    /* 0x0C8C */ daNpcF_ActorMngr_c mActorMngr[1];
    /* 0x0C94 */ daNpcF_Path_c mPath;
    /* 0x12C4 */ daNpcKasiKyu_HIO_c* mHIO;
    /* 0x12C8 */ dCcD_Cyl mCyl;
    /* 0x1404 */ s16 mMode;
    /* 0x1408 */ actionFunc mAction;
    /* 0x1414 */ request_of_phase_process_class mPhases[3];
    /* 0x142C */ fpc_ProcID field_0x142c;
    /* 0x1430 */ int field_0x1430;
    /* 0x1434 */ int field_0x1434;
    /* 0x1438 */ int mMessageNo;
    /* 0x143C */ s16 mLookMode;
    /* 0x143E */ s16 field_0x143e;
    /* 0x1440 */ int mSygnal;
    /* 0x1444 */ cXyz mChacePos;
    /* 0x1450 */ cXyz mCenterPos;
    /* 0x145C */ int mPlPoint;
    /* 0x1460 */ int field_0x1460;
    /* 0x1464 */ s8 mType;
    /* 0x1465 */ s8 field_0x1465;
    /* 0x1466 */ bool mTalked;
    /* 0x1467 */ bool mFear;
    /* 0x1468 */ bool mEscape;
    /* 0x1469 */ u8 field_0x1469;
};

STATIC_ASSERT(sizeof(daNpcKasiKyu_c) == 0x146c);

class daNpcKasiKyu_Param_c {
public:
    virtual ~daNpcKasiKyu_Param_c() {}

    static daNpcKasiKyu_HIOParam const m;
};

#endif /* D_A_NPC_KASI_KYU_H */
