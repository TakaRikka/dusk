#ifndef D_A_TAG_EVENT_H
#define D_A_TAG_EVENT_H

#include "f_op/f_op_actor_mng.h"

class daTag_Event_c : public fopAc_ac_c {
public:
    enum Action_e {
        ACTION_WAIT,
        ACTION_ARRIVAL,
        ACTION_HUNT,
        ACTION_HUNT2,
        ACTION_READY,
        ACTION_NEXT,
        ACTION_EVENT,
    };

    u8 getEventNo();
    u8 getSwbit();
    u8 getSwbit2();
    u8 getType();
    u16 getInvalidEventFlag();
    u16 getAreaType();
    u16 getValidEventFlag();
    BOOL horseRodeo();
    BOOL arrivalTerms();
    void demoInitProc();
    void demoEndProc();
    void demoProc();
    int create();
    int actionNext();
    int actionEvent();
    int actionReady();
    BOOL checkArea();
    int actionHunt();
    int actionArrival();
    int actionWait();
    int actionHunt2();
    int execute();
    int draw();

    void setActio(u8 i_action) { mAction = i_action; }

    /* 0x568 */ s16 mEventIdx;
    /* 0x56A */ s16 mMapEventIdx;
    /* 0x56C */ s16 mMapToolId;
    /* 0x56E */ s16 field_0x56e;
    /* 0x570 */ s16 mHunt2Timer;
    /* 0x572 */ u8 mAction;
    /* 0x573 */ u8 field_0x573;
};

#endif /* D_A_TAG_EVENT_H */
