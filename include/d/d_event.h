#ifndef D_EVENT_D_EVENT_H
#define D_EVENT_D_EVENT_H

#include "d/actor/d_a_itembase.h"

int dEv_defaultSkipProc(void* actor, int parameter);
int dEv_defaultSkipZev(void* actor, int parameter);
int dEv_defaultSkipStb(void* actor, int parameter);
int dEv_noFinishSkipProc(void* actor, int parameter);

struct dStage_MapEvent_dt_c;
class fopAc_ac_c;

class dEvt_order_c {
public:
    ~dEvt_order_c() {}
    dEvt_order_c();

    /* 0x00 */ u16 mEventType;
    /* 0x02 */ u16 mFlag;
    /* 0x04 */ u16 mHindFlag;
    /* 0x08 */ fopAc_ac_c* mpRequestActor;
    /* 0x0C */ fopAc_ac_c* mpTargetActor;
    /* 0x10 */ s16 mEventId;
    /* 0x12 */ u16 mPriority;
    /* 0x14 */ s8 mNextOrderIdx;
    /* 0x15 */ u8 mMapToolId;
};  // Size = 0x18

STATIC_ASSERT(sizeof(dEvt_order_c) == 0x18);

typedef int (*dEvt_SkipCb)(void*, int);

enum dEvt_mode_e {
    /* 0x0 */ dEvt_mode_WAIT_e,
    /* 0x1 */ dEvt_mode_TALK_e,
    /* 0x2 */ dEvt_mode_DEMO_e,
    /* 0x3 */ dEvt_mode_COMPULSORY_e
};

enum dEvt_type_e {
    /* 0x0 */ dEvt_type_TALK_e,
    /* 0x1 */ dEvt_type_DOOR_e,
    /* 0x2 */ dEvt_type_OTHER_e,
    /* 0x3 */ dEvt_type_COMPULSORY_e,
    /* 0x4 */ dEvt_type_POTENTIAL_e,
    /* 0x5 */ dEvt_type_ITEM_e,
    /* 0x6 */ dEvt_type_SHOWITEM_X_e,
    /* 0x7 */ dEvt_type_SHOWITEM_Y_e,
    /* 0xA */ dEvt_type_CATCH_e = 10,
    /* 0xB */ dEvt_type_TREASURE_e,
};

enum EventStatus {
    /* 0x0 */ STATUS_NONE,
};

class dEvt_control_c {
public:
    dEvt_control_c();

    s32 orderOld(u16 type, u16 prio, u16 flag, u16 hind, void* requestActor,
                 void* targetActor, const void* eventName);
    s32 order(u16 type, u16 prio, u16 flag, u16 hind, void* requestActor,
              void* targetActor, s16 id, u8 mapToolId);
    void setParam(dEvt_order_c* order);
    s32 beforeFlagProc(dEvt_order_c* order);
    void afterFlagProc(dEvt_order_c* order);
    int commonCheck(dEvt_order_c* order, u16 condition, u16 command);
    int talkCheck(dEvt_order_c* order);
    int talkXyCheck(dEvt_order_c* order);
    int catchCheck(dEvt_order_c* order);
    int talkEnd();
    int demoCheck(dEvt_order_c* order);
    int demoEnd();
    int potentialCheck(dEvt_order_c* order);
    int doorCheck(dEvt_order_c* order);
    int itemCheck(dEvt_order_c* order);
    int endProc();
    int change();
    int entry();
    void reset(void*);
    void reset();
    void clearSkipSystem();
    char* getSkipEventName();
    void setSkipProc(void* skipActor, dEvt_SkipCb skipCb, int skipParameter);
    void setSkipZev(void* skipActor, char* eventName);
    void onSkipFade();
    void offSkipFade();
    bool skipper();
    int Step();
    int moveApproval(void*);
    BOOL compulsory(void*, const char* eventName, u16 hind);
    void remove();
    dStage_MapEvent_dt_c* getStageEventDt();
    void sceneChange(int exitId);
    fpc_ProcID getPId(void* actor);
    fopAc_ac_c* convPId(fpc_ProcID id);
    void* getStbDemoData(char* resName);
    static dStage_MapEvent_dt_c* searchMapEventData(u8 mapToolID, s32 roomNo);
    static dStage_MapEvent_dt_c* searchMapEventData(u8 mapToolID);
    s16 runningEventID(s16 eventID);
    void setPt1(void* actor);
    void setPt2(void* actor);
    void setPtT(void* actor);
    void setPtI(void* actor);
    void setPtI_Id(fpc_ProcID id);
    void setPtD(void* actor);
    void setGtItm(u8 itemNo);

    u16 chkFlag2(u16 flag) { return flag & mFlag2; }

    BOOL runCheck() {
        #if DEBUG
        return mEventStatus != 0 || mDebugStb != 0;
        #else
        return mEventStatus != 0;
        #endif
    }

    f32 getCullRate() { return mCullRate; }
    void setCullRate(f32 rate) { mCullRate = rate; }

    u16 chkEventFlag(u16 flag) { return flag & mEventFlag; }
    void onEventFlag(u16 flag) { mEventFlag |= flag; }
    void offEventFlag(u16 flag) { mEventFlag &= ~flag; }

    u8 getMode() const { return mMode; }

    void onHindFlag(u16 flag) { mHindFlag |= flag; }
    #if DEBUG
    void offHindFlag(u16 flag) { mHindFlag &= (u16)~flag; }
    #else
    void offHindFlag(u16 flag) { mHindFlag &= ~flag; }
    #endif

    u16 checkHind(u16 flag) { return flag & mHindFlag; }
    u8 checkCompulsory() { return mCompulsory; }
    u8 getMapToolId() { return mMapToolId; }

    void onFlag2(u16 flag) { mFlag2 |= flag; }
    void offFlag2(u16 flag) { mFlag2 &= ~flag; }

    BOOL chkTalkXY() 
    {
#if PLATFORM_WII || PLATFORM_SHIELD
        return mTalkXyType == 1 || mTalkXyType == 2 || mTalkXyType == 3 || mTalkXyType == 4;
#else
        return mTalkXyType == 1 || mTalkXyType == 2;
#endif
    }

    BOOL isOrderOK() {
#if DEBUG
        return (mEventStatus == 0 || mEventStatus == 2) && !mDebugStb;
#else
        return mEventStatus == 0 || mEventStatus == 2;
#endif
    }

    fopAc_ac_c* getPt1() { return convPId(mPt1); }
    fopAc_ac_c* getPt2() { return convPId(mPt2); }
    fopAc_ac_c* getPtT() { return convPId(mPtT); }
    fopAc_ac_c* getPtI() { return convPId(mPtI); }
    fopAc_ac_c* getPtD() { return convPId(mPtd); }

    bool isChangeOK(void* actor) { return mChangeActor == actor; }
    u8 getPreItemNo() { return mPreItemNo; }
    u8 getGtItm() { return mGtItm; }
    void startCheckSkipEdge(void* actor) { setSkipProc(actor, dEv_noFinishSkipProc, 0); }
    bool checkSkipEdge() { return chkFlag2(8) != false; }
    void setDebugStb(u8 stb) { mDebugStb = stb; }

public:
    /* 0x000 */ u8 unk_0x0[4];
    /* 0x004 */ dEvt_order_c mOrder[8];
    /* 0x0C4 */ fpc_ProcID mPt1;
    /* 0x0C8 */ fpc_ProcID mPt2;
    /* 0x0CC */ fpc_ProcID mPtT;
    /* 0x0D0 */ fpc_ProcID mPtI;
    /* 0x0D4 */ fpc_ProcID mPtd;
    /* 0x0D8 */ u16 mEventFlag;
    /* 0x0DA */ u16 mFlag2;
    /* 0x0DC */ u16 mHindFlag;
    /* 0x0DE */ s16 mEventId;
    /* 0x0E0 */ s16 mUnkEventId;  // unknown purpose
    /* 0x0E2 */ s8 mNum;
    /* 0x0E3 */ s8 mOrderIdx;
    /* 0x0E4 */ u8 mMode;
    /* 0x0E5 */ u8 mEventStatus;
    /* 0x0E6 */ u8 field_0xe6;
    /* 0x0E7 */ u8 field_0xe7;
    /* 0x0E8 */ s8 field_0xe8;
    /* 0x0E9 */ u8 mDebugStb;
    /* 0x0EA */ u8 field_0xea;
    /* 0x0EB */ u8 mMapToolId;
    /* 0x0EC */ u8 field_0xec;
    /* 0x0ED */ u8 mTalkXyType;
    /* 0x0EE */ u8 mPreItemNo;
    /* 0x0EF */ u8 mGtItm;
    /* 0x0F0 */ f32 mCullRate;
    /* 0x0F4 */ dStage_MapEvent_dt_c* mStageEventDt;
    /* 0x0F8 */ void* mChangeActor;
    /* 0x0FC */ u8 unk_0xfc[4];
    /* 0x100 */ dEvt_SkipCb mSkipFunc;
    /* 0x104 */ fpc_ProcID mSkipActorId;
    /* 0x108 */ int mSkipTimer;
    /* 0x10C */ int mSkipParameter;
    /* 0x110 */ BOOL mIsSkipFade;
    /* 0x114 */ char mSkipEventName[20];
    /* 0x128 */ u8 mCompulsory;
    /* 0x129 */ bool mRoomInfoSet;
    /* 0x12C */ int mRoomNo;
#if PLATFORM_WII || PLATFORM_SHIELD
    /* 0x130 */ u8 field_0x130;
#endif
};

#endif /* D_EVENT_D_EVENT_H */
