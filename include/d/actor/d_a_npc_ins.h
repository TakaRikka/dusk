#ifndef D_A_NPC_INS_H
#define D_A_NPC_INS_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpcIns_c
 * @brief Agitha
 *
 * @details
 *
*/

struct daNpcIns_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x70 */ f32 walk_speed;          // 歩行速度 - Walking Speed
};

class daNpcIns_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpcIns_HIOParam param;
};

class daNpcIns_Param_c {
public:
    virtual ~daNpcIns_Param_c() {}

    static daNpcIns_HIOParam const m;
};

struct insect_param_data {
    int evt_bit_no;
    s16 msg_no;
    u8 field_0x6;
    u8 field_0x7;
};

class daNpcIns_c : public daNpcF_c {
public:
    typedef int (daNpcIns_c::*actionFunc)(void*);
    typedef BOOL (daNpcIns_c::*eventFunc)(int);

    daNpcIns_c();
    ~daNpcIns_c();
    cPhs__Step Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void setMotionAnm(int, f32);
    void reset();
    int waitShop(void*);
    void setMotion(int, f32, int);
    void setExpression(int, f32);
    int waitPresent(void*);
    int waitOutSide1(void*);
    int waitOutSide2(void*);
    int goHome(void*);
    int talk(void*);
    int demo(void*);
    bool isInsectComplete();
    const insect_param_data& getInsectParamData(int);
    void setWaitAction();
    bool setPath(int);
    bool checkPoint(cXyz&, f32);
    bool setNextPoint();
    inline void getTargetPoint(int, Vec*);
    void pathMoveF();
    inline void setParam();
    inline BOOL main();
    inline void playMotion();
    inline BOOL ctrlBtk();
    inline void setAttnPos();
    inline void lookat();
    inline void setMtx();
    inline int drawDbgInfo();

    int getStartTime() { return fopAcM_GetParam(this) & 0xFF; }
    int getEndTime() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }
    int getTimeHour() { return dKy_getdaytime_hour(); }
    int getTimeMinute() { return dKy_getdaytime_minute(); }
    int getTime() { return getTimeMinute() + getTimeHour() * 60; }
    s16 getMessageNo() { return shape_angle.x; }
    int getPathID1() { return (fopAcM_GetParam(this) >> 16) & 0xFF; }
    int getPathID2() {  return fopAcM_GetParam(this) >> 24; }
    inline BOOL setAction(actionFunc);
    inline void waitEventMng();
    inline BOOL step(s16, int);
    void setLookMode(int i_lookMode) { if (i_lookMode >= 0 && i_lookMode < 4 && i_lookMode != mLookMode) mLookMode = i_lookMode; }
    inline BOOL chkFindPlayer();
    inline void checkPlayerSearch();
    bool isInsect(int type) {
        bool rv = false;

        if (type >= fpcNm_ITEM_M_BEETLE && type <= fpcNm_ITEM_F_MAYFLY) {
            rv = true;
        }

        return rv;
    }
    s16 getInsectMessageNo(int type) { return getInsectParamData(type).msg_no; }
    u32 getInsectEvtBitNo(int type) { return getInsectParamData(type).evt_bit_no; }
    inline void setExpressionTalkAfter();
    inline void playExpression();
    BOOL chkAction(actionFunc action) { return action == mAction; }

    static eventFunc mEvtSeqList[1];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ J3DModel* mpUmbrellaModel;
    /* 0xBDC */ J3DModel* mpKagoModel;
    /* 0xBE0 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBE4 */ daNpcF_Lookat_c mLookat;
    /* 0xC80 */ daNpcF_ActorMngr_c mActorMngr[1];
    /* 0xC88 */ daNpcIns_HIO_c* mHIO;
    /* 0xC8C */ dCcD_Cyl mCyl;
    /* 0xDC8 */ actionFunc mAction;
    /* 0xDD4 */ actionFunc mPrevAction;
    /* 0xDE0 */ request_of_phase_process_class mPhases[3];
    /* 0xDF8 */ fpc_ProcID mItemID;
    /* 0xDFC */ int field_0xdfc;
    /* 0xE00 */ int mRndValue;
    /* 0xE04 */ dPath* mPath;
    /* 0xE08 */ int mPntIndex;
    /* 0xE0C */ f32 field_0xe0c;
    /* 0xE10 */ f32 mCurrentPosY;
    /* 0xE14 */ s16 field_0xe14;
    /* 0xE16 */ s16 field_0xe16;
    /* 0xE18 */ s16 mInsectMsgNo;
    /* 0xE1A */ s16 mLookMode;
    /* 0xE1C */ u16 mGoHomeTime;
    /* 0xE1E */ u16 mMode;
    /* 0xE20 */ u8 mOrderSpeakEvent;
    /* 0xE21 */ u8 mType;
};

STATIC_ASSERT(sizeof(daNpcIns_c) == 0xe24);

#endif /* D_A_NPC_INS_H */
