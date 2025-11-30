#ifndef D_A_NPC_SHOE_H
#define D_A_NPC_SHOE_H

#include "d/actor/d_a_npc4.h"

struct daNpcShoe_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
};

class daNpcShoe_Param_c {
public:
    virtual ~daNpcShoe_Param_c() {}

    static const daNpcShoe_HIOParam m;
};

STATIC_ASSERT(sizeof(daNpcShoe_Param_c::m) == 0x6C);

#if DEBUG
class daNpcShoe_HIO_c : public mDoHIO_entry_c {
public:
    daNpcShoe_HIO_c();

    void genMessage(JORMContext*);

    daNpcShoe_HIOParam m;
};

#define NPC_SHOE_HIO_CLASS daNpcShoe_HIO_c
#else
#define NPC_SHOE_HIO_CLASS daNpcShoe_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpcShoe_c
 * @brief Soal
 *
 * @details
 *
 */
class daNpcShoe_c : public daNpcF_c {
public:
    typedef bool (daNpcShoe_c::*actionFunc)(void*);
    typedef BOOL (daNpcShoe_c::*EventFn)(int);

    enum Animation {
        /* 0x01 */ ANM_M_SITWAIT_A,
        /* 0x02 */ ANM_M_SITTALK_A,
        /* 0x03 */ ANM_M_SITTALK_A_B,
        /* 0x04 */ ANM_SHOE_TALK_A,
        /* 0x05 */ ANM_M_SIT_TO_WOLF_A
    };

    enum Motion {
        /* 0x01 */ MOT_SITWAIT_A,
        /* 0x02 */ MOT_SITTALK_A,
        /* 0x03 */ MOT_SITTALK_A_B,
        /* 0x04 */ MOT_SHOE_TALK_A,
        /* 0x05 */ MOT_SIT_TO_WOLF_A,
    };

    daNpcShoe_c();
    ~daNpcShoe_c();
    cPhs__Step Create();
    BOOL CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    bool ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static BOOL ctrlJointCallBack(J3DJoint*, int);
    void setMotion(int, f32, BOOL);
    void reset();
    inline int setAction(actionFunc);
    bool wait(void*);
    bool fear(void*);
    bool talk(void*);
    bool demo(void*);
    void setParam();
    BOOL main();
    void setAttnPos();
    inline void lookat();
    void setMotionAnm(int, f32);
    inline BOOL drawDbgInfo();
    inline void adjustShapeAngle() {}

    s16 getMessageNo() { return fopAcM_GetParam(this) >> 0x10; }

    int getTime() { return getTimeMinute() + getTimeHour() * 60; }
    int getTimeHour() {
        if (dKy_darkworld_check() != 0) {
            return dKy_getDarktime_hour();
        } else {
            return dKy_getdaytime_hour();
        }
    }
    int getTimeMinute() {
        if (dKy_darkworld_check() != 0) {
            return dKy_getDarktime_minute();
        } else {
            return dKy_getdaytime_minute();
        }
    }

    int getStartTime() { return fopAcM_GetParam(this) & 0xff; }
    int getEndTime() { return (fopAcM_GetParam(this) >> 8) & 0xff; }

    inline void setLookMode(int i_lookMode);
    inline bool chkFindPlayer();
    inline void playMotion();

    static EventFn mEvtSeqList[1];

private:
    /* 0xB48 */ J3DModel* mpModel1;
    /* 0xB4C */ J3DModel* mpModel2;
    /* 0xB50 */ Z2CreatureCitizen mCreature;
    /* 0xBF4 */ u8 field_0xbf4[0xbf8 - 0xbf4];
    /* 0xBF8 */ daNpcF_Lookat_c mLookat;
    /* 0xC94 */ daNpcF_ActorMngr_c mActorMngr[1];
    /* 0xC9C */ NPC_SHOE_HIO_CLASS* mpHIO;
    /* 0xCA0 */ dCcD_Cyl mCcCyl;
    /* 0xDDC */ actionFunc mpActionFn;
    /* 0xDE8 */ request_of_phase_process_class mPhases[3];
    /* 0xE00 */ u32 field_0xe00;
    /* 0xE04 */ u32 field_0xe04;
    /* 0xE08 */ u32 field_0xe08;
    /* 0xE0C */ u32 field_0xe0c;
    /* 0xE10 */ s16 mLookMode;
    /* 0xE12 */ u16 mStartTime;
    /* 0xE14 */ u16 mEndTime;
    /* 0xE16 */ u16 field_0xe16;
    /* 0xE18 */ u16 mMode;
    /* 0xE1A */ u8 field_0xe1a;
    /* 0xE1B */ u8 field_0xe1b;  // Probably isDaytime or isNighttime
};

STATIC_ASSERT(sizeof(daNpcShoe_c) == 0xe1c);

#endif /* D_A_NPC_SHOE_H */
