#ifndef D_EVENT_D_EVENT_MANAGER_H
#define D_EVENT_D_EVENT_MANAGER_H

#include "SSystem/SComponent/c_xyz.h"
#include "d/d_event_data.h"
#include "d/d_event_debug.h"

class fopAc_ac_c;

class dEvent_exception_c {
public:
    dEvent_exception_c() { mEventInfoIdx = -1; }
    ~dEvent_exception_c() {}
    void init();
    int setStartDemo(int mapToolID);
    const char* getEventName();

    /* 0x0 */ int mEventInfoIdx;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6;
    /* 0x7 */ u8 field_0x7;
    /* 0x8 */ int mState;
};

#define EVT_SE_RIDDLE_A 1
#define EVT_SE_RIDDLE_B 2

class dEvent_manager_c {
public:
    enum {
        BASE_NULL,
        BASE_KEEP,
        BASE_ACTOR,
        BASE_STAGE,
        BASE_ROOM0,
        BASE_ROOM1,
        BASE_ROOM2,
        BASE_ROOM3,
        BASE_ROOM4,
        BASE_ROOM5,
        BASE_DEMO,

        #if DEBUG
        BASE_DEBUG,
        #endif

        BASE_MAX,
    };

    void* getSubstance(dEvDtData_c* evData, int type);
    void setDbgData(const char* srcData);
    dEvent_manager_c();
    ~dEvent_manager_c() {}
    int create();
    bool setObjectArchive(char* arcname);
    void demoInit();
    void roomInit(int roomNo);
    void roomFinish(int roomNo);
    int orderStartDemo();
    void remove();
    void startProc(dEvDtEvent_c* event);
    void closeProc(dEvDtEvent_c* event);
    void endProc(s16 evId, BOOL isClose);
    void Sequencer();
    void Experts();
    dEvDtEvent_c* getEventData(s16 evCompositId);
    dEvDtEvent_c* getEventData(s16 evIdx, int evType);
    s16 getEventIdx(const char* eventName, u8 mapToolID, s32 roomNo);
    s16 getEventIdx(fopAc_ac_c* actor, u8 mapToolID);
    s16 getEventIdx(fopAc_ac_c* actor, const char* eventName, u8 mapToolID);
    int isMapToolCamera(u8 mapToolID, s32 roomNo);
    int order(s16 evCompositId);
    int startCheck(s16 evCompositId);
    int startCheckOld(const char* eventname);
    int endCheck(s16 eventID);
    int endCheckOld(const char* eventname);
    int getMyStaffId(const char* staffName, fopAc_ac_c* actor, int tagId);
    int getIsAddvance(int staffId);
    int getMyActIdx(int staffId, const char* const* action, int n_action, BOOL param_3, BOOL param_4);
    char* getMyNowCutName(int staffId);
    dEvDtData_c* getMyDataP(int staffId, const char* dataname, BOOL isSeekStart);
    void* getMySubstanceP(int staffId, const char* dataname, int datatype);
    int getMySubstanceNum(int staffId, const char* dataname);
    void cutEnd(int staffId);
    int getEventPrio(fopAc_ac_c* actor, s16 evCompositId);
    void issueStaff(const char* staffname);
    void cancelStaff(const char* staffname);
    void setGoal(cXyz* pos);
    cXyz* getGoal();
    const char* getRunEventName();
    fopAc_ac_c* specialCast_Shutter(s16 actorName, BOOL param_1);
    fopAc_ac_c* specialCast(const char* staffname, BOOL param_1);
    int ChkPresentEnd();
    int checkStartDemo();

    void debugBfProc();

    dEvDtBase_c& getBase() { return mEventList[mCurrentEvType]; }
    event_binary_data_header* getHeader(u8 type) {return mEventList[type].getHeaderP(); }
    dEvDtEvent_c* getEventList(u8 type) { return mEventList[type].getEventP(); }
    int cameraPlay() { return mCameraPlay; }
    void setCameraPlay(int status) { mCameraPlay = status; }
    int setStartDemo(int mapToolId) { return mException.setStartDemo(mapToolId); }
    dEvDtFlag_c& getFlags() { return mFlags; }
    int flagCheck(int flag) { return mFlags.flagCheck(flag); }
    void setFlag(int flag) { mFlags.flagSet(flag); }
    bool dataLoaded() { return mDataLoaded; }

    #if DEBUG
    dEvM_HIO_c& getEventHIO();
    #endif

private:
    /* 0x000 */ dEvDtBase_c mEventList[BASE_MAX];
    /* 0x18C */ int mCameraPlay;
    /* 0x190 */ dEvent_exception_c mException;
    /* 0x19C */ cXyz mGoal;
    /* 0x1A8 */ u8 unk_0x1a8[2];
    /* 0x1AA */ s16 mCurrentEvId;
    /* 0x1AC */ u8 unk_0x1ac[4];
    /* 0x1B0 */ int mRoomNo;
    /* 0x1B4 */ int mCurrentEvType;
    /* 0x1B8 */ s32 unk_0x1b8;
    /* 0x1BC */ dEvDtFlag_c mFlags;
    /* 0x6BC */ bool mDataLoaded;
};

#endif /* D_EVENT_D_EVENT_MANAGER_H */
