#ifndef D_EVENT_D_EVENT_DATA_H
#define D_EVENT_D_EVENT_DATA_H

#include "global.h"
#include "f_pc/f_pc_base.h"

struct msg_class;

struct event_binary_data_header {
    /* 0x00 */ u32 eventTop;  // offset to Event chunk
    /* 0x04 */ s32 eventNum;
    /* 0x08 */ u32 staffTop;  // offset to Staff chunk
    /* 0x0C */ s32 staffNum;
    /* 0x10 */ u32 cutTop;  // offset to Cut chunk
    /* 0x14 */ s32 cutNum;
    /* 0x18 */ u32 dataTop;  // offset to Data chunk
    /* 0x1C */ s32 dataNum;
    /* 0x20 */ u32 fDataTop;  // offset to FData chunk
    /* 0x24 */ s32 fDataNum;
    /* 0x28 */ u32 iDataTop;  // offset to IData chunk
    /* 0x2C */ s32 iDataNum;
    /* 0x30 */ u32 sDataTop;  // offset to SData chunk
    /* 0x34 */ s32 sDataNum;
    /* 0x38 */ u8 unk[8];
};  // Size: 0x40

enum dEvDt_State_e {
    dEvDt_State_START_e = 1,
    dEvDt_State_END_e,
};

class dEvDtData_c {
public:
    enum DataType {
        /* 0x0 */ TYPE_FLOAT,
        /* 0x1 */ TYPE_VEC,
        /* 0x3 */ TYPE_INT = 3,
        /* 0x4 */ TYPE_STRING
    };

    int getIndex() { return mDataIndex; }
    int getNumber() { return mNumber; }
    int getType() { return mType; }
    int getNext() { return mNext; }
    char* getName() { return mName; }

    /* 0x00 */ char mName[32];
    /* 0x20 */ u32 mIndex;
    /* 0x24 */ int mType;
    /* 0x28 */ int mDataIndex;
    /* 0x2C */ int mNumber;
    /* 0x30 */ int mNext;
    /* 0x34 */ u8 field_0x34[12];
};  // Size: 0x40

#if DEBUG
class dEvDtData_DBG_c : public dEvDtData_c {
public:
    void Init() {
        mName[0] = '\0';
        mIndex = 0;
        mType = 5;
        mDataIndex = 0;
        mNumber = 0;
        mNext = -1;
    }
};
#endif

class dEvDtCut_c {
public:
    int startCheck();

    int getDataTop() { return mDataTop; }
    int getFlagId() { return mFlagId; }
    char* getName() { return mName; }
    int getNext() { return mNext; }
    int getStartFlag(int idx) { return mFlags[idx]; }
    u32 getTagId() { return mTagID; }

    /* 0x00 */ char mName[32];
    /* 0x20 */ u32 mTagID;
    /* 0x24 */ u32 mIndex;
    /* 0x28 */ int mFlags[3];
    /* 0x34 */ u32 mFlagId;
    /* 0x38 */ int mDataTop;
    /* 0x3C */ int mNext;
    /* 0x40 */ u8 field_0x40[0x10];
};  // Size: 0x50

#if DEBUG
class dEvDtCut_DBG_c : public dEvDtCut_c {
public:
    void Init() {
        int i;

        mName[0] = '\0';
        mTagID = 0;
        mIndex = 0;

        for (i = 0; i < 3; i++) {
            mFlags[i] = -1;
        }

        mFlagId = 0;
        mDataTop = -1;
        mNext = -1;
    }
};
#endif

class dEvDtStaff_c {
public:
    enum StaffType {
        /* 0x0 */ TYPE_DEFAULT,
        /* 0x1 */ TYPE_ALL,
        /* 0x2 */ TYPE_CAMERA,
        /* 0x3 */ TYPE_EFFECT,
        /* 0x4 */ TYPE_TIMEKEEPER,
        /* 0x6 */ TYPE_DIRECTOR = 6,
        /* 0x7 */ TYPE_MESSAGE,
        /* 0x8 */ TYPE_SOUND,
        /* 0x9 */ TYPE_LIGHT,
        /* 0xB */ TYPE_PACKAGE = 11,
        /* 0xC */ TYPE_CREATE,
    };

    struct StaffWork {
        fpc_ProcID l_msgId;
        msg_class* l_msg;
        u32 l_msgNo;
        struct {
            int n;   // substance num
            int* d;  // data
        } msgs;
    };

    struct MessageData {
        s16 unk;
    };

    struct SoundData {
        s16 unk;
        s16 timer;
    };

    struct TimerKeeperData {
        s32 timer;
    };

    struct DirectorData {
        s16 unk;
        s16 unk2;
    };

    struct EffectData {
        u8 pad[8];
        s32 unk;
    };

    void specialProc_WaitStart(int index);
    void specialProc_WaitProc(int);
    void specialProc();
    void init();
    void advanceCut(int cut);
    void specialProcLight();
    void specialProcMessage();
    void specialProcSound();
    void specialProcCreate();
    void specialProcDirector();
    void specialProcPackage();
    void specialProcTimekeeper();
    void specialProcEffect();

    int getType() { return mType; }
    char* getName() { return mName; }
    s32 getTagID() { return mTagID; }
    int getCurrentCut() { return mCurrentCut; }
    int getStartCut() { return mStartCut; }

    // private:
    /* 0x00 */ char mName[8];
    /* 0x08 */ u8 mWork[0x18];
    /* 0x20 */ s32 mTagID;
    /* 0x24 */ u32 mIndex;
    /* 0x28 */ u32 mFlagID;
    /* 0x2C */ int mType;
    /* 0x30 */ int mStartCut;
    /* 0x34 */ s16 field_0x34;
    /* 0x36 */ s16 mWaitTimer;
    /* 0x38 */ int mCurrentCut;
    /* 0x3C */ s32 field_0x3c;
    /* 0x40 */ bool field_0x40;
    /* 0x41 */ bool field_0x41;
    /* 0x42 */ u8 mData[0x50 - 0x42];
};  // Size: 0x50

STATIC_ASSERT(sizeof(dEvDtStaff_c) == 0x50);

#if DEBUG
class dEvDtStaff_DBG_c : public dEvDtStaff_c {
public:
    void Init() {
        mName[0] = '\0';
        mTagID = 0;
        mIndex = 0;
        mFlagID = 0;
        mType = 13;
        mStartCut = 0;
        field_0x34 = 0;
    }
};
#endif

class dEvDtEvent_c {
public:
    int finishCheck();
    int forceFinish();
    void specialStaffProc(dEvDtStaff_c* p_staff);
    int getNStaff() { return mNStaff; }
    int getStaff(int idx) { return mStaff[idx]; }
    char* getName() { return mName; }
    int getPriority() { return mPriority; }

    /* 0x00 */ char mName[32];
    /* 0x20 */ u32 mIndex;
    /* 0x24 */ int field_0x24;
    /* 0x28 */ int mPriority;
    /* 0x2C */ int mStaff[20];
    /* 0x7C */ int mNStaff;
    /* 0x80 */ int field_0x80;
    /* 0x84 */ int field_0x84;
    /* 0x88 */ int mFlags[3];
    /* 0x94 */ bool mPlaySound;
    /* 0x96 */ s16 field_0x96;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C */ f32 field_0x9c;
    /* 0xA0 */ f32 field_0xa0;
    /* 0xA4 */ int mEventState;
    /* 0xA8 */ int field_0xa8;
    /* 0xAC */ u8 field_0xac[4];
};  // Size: 0xB0

#if DEBUG
class dEvDtEvent_DBG_c : public dEvDtEvent_c {
public:
    void Init() {
        int i;

        mName[0] = '\0';
        mIndex = 0;
        mStaff[0] = 0;
        mNStaff = 0;

        for (i = 0; i < 3; i++) {
            mFlags[i] = -1;
        }

        field_0x80 = field_0x84 = 0;
        mPlaySound = false;
        field_0x24 = 1;
        mPriority = 0;
        mEventState = 0;
        field_0xa8 = 0;
        field_0x96 = 0;
        field_0x98 = 0.0f;
        field_0x9c = 0.0f;
        field_0xa0 = 0.0f;
    }
};  // Size: 0xB0
#endif

class dEvDtFlag_c {
public:
    dEvDtFlag_c() {}
    ~dEvDtFlag_c() {}
    BOOL flagCheck(int flag);
    BOOL flagSet(int flag);
    BOOL flagMaxCheck(int flag);
    void init();

    static const int FlagMax = 0x2800;

private:
    u32 mFlags[320];
};  // Size = 0x500

class dEvDtBase_c {
public:
    dEvDtBase_c() { init(); }
    ~dEvDtBase_c() {}
    int init();
    int init(char* p_data, int roomNo);
    void advanceCut(dEvDtEvent_c* p_event);
    BOOL advanceCutLocal(dEvDtStaff_c* p_staff);

    event_binary_data_header* getHeaderP() { return mHeaderP; }
    dEvDtStaff_c* getStaffP(int i) { return &mStaffP[i]; }
    dEvDtStaff_c* getStaffP() { return mStaffP; }
    dEvDtEvent_c* getEventP(int i) { return &mEventP[i]; }
    dEvDtEvent_c* getEventP() { return mEventP; }
    dEvDtData_c* getDataP(int i) { return &mDataP[i]; }
    dEvDtCut_c* getCutP(int i) { return &mCutP[i]; }
    char* getEventName(int i) { return mEventP[i].getName(); }
    int getStaffCurrentCut(int i) { return mStaffP[i].getCurrentCut(); }
    dEvDtCut_c* getCutStaffCurrentCutP(int i) { return getCutP(getStaffCurrentCut(i)); }
    int getStaffStartCut(int i) { return mStaffP[i].getStartCut(); }
    dEvDtCut_c* getCutStaffStartCutP(int i) { return getCutP(getStaffStartCut(i)); }
    int* getIDataP(int i) { return &mIDataP[i]; }
    f32* getFDataP(int i) { return &mFDataP[i]; }
    char* getSDataP(int i) { return &mSDataP[i]; }
    u32 getEventTop() { return mHeaderP->eventTop; }
    s32 getEventNum() { return mHeaderP->eventNum; }
    u32 getStaffTop() { return mHeaderP->staffTop; }
    s32 getStaffNum() { return mHeaderP->staffNum; }
    u32 getCutTop() { return mHeaderP->cutTop; }
    s32 getCutNum() { return mHeaderP->cutNum; }
    u32 getDataTop() { return mHeaderP->dataTop; }
    s32 getDataNum() { return mHeaderP->dataNum; }
    u32 getFDataTop() { return mHeaderP->fDataTop; }
    s32 getFDataNum() { return mHeaderP->fDataNum; }
    u32 getIDataTop() { return mHeaderP->iDataTop; }
    s32 getIDataNum() { return mHeaderP->iDataNum; }
    u32 getSDataTop() { return mHeaderP->sDataTop; }
    s32 getSDataNum() { return mHeaderP->sDataNum; }
    s32 roomNo() { return mRoomNo; }

    void setHeaderP(event_binary_data_header* p_header) { mHeaderP = p_header; }
    void setEventP(dEvDtEvent_c* p_event) { mEventP = p_event; }
    void setStaffP(dEvDtStaff_c* p_staff) { mStaffP = p_staff; }
    void setCutP(dEvDtCut_c* p_cut) { mCutP = p_cut; }
    void setDataP(dEvDtData_c* p_data) { mDataP = p_data; }
    void setFDataP(f32* p_fdata) { mFDataP = p_fdata; }
    void setIDataP(int* p_idata) { mIDataP = p_idata; }
    void setSDataP(char* p_sdata) { mSDataP = p_sdata; }

    // private:
    /* 0x00 */ event_binary_data_header* mHeaderP;
    /* 0x04 */ dEvDtEvent_c* mEventP;
    /* 0x08 */ dEvDtStaff_c* mStaffP;
    /* 0x0C */ dEvDtCut_c* mCutP;
    /* 0x10 */ dEvDtData_c* mDataP;
    /* 0x14 */ f32* mFDataP;
    /* 0x18 */ int* mIDataP;
    /* 0x1C */ char* mSDataP;
    /* 0x20 */ s32 mRoomNo;
};  // Size = 0x24

STATIC_ASSERT(sizeof(dEvDtBase_c) == 0x24);

#endif /* D_EVENT_D_EVENT_DATA_H */
