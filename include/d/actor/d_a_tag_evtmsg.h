#ifndef D_A_TAG_EVTMSG_H
#define D_A_TAG_EVTMSG_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_msg_flow.h"

class daTag_EvtMsg_c;
typedef BOOL (daTag_EvtMsg_c::*EvtCutFunc)(int);

class daTag_EvtMsg_c : public fopAc_ac_c {
public:
    /* 0x568 */ int mTimer;
    /* 0x56C */ int mFlowID;
    /* 0x570 */ s16 mEventID;
    /* 0x572 */ u8 field_0x572;
    /* 0x574 */ dMsgFlow_c mMsgFlow;

public:
    int create();
    int Delete();
    int Execute();
    int Draw();
    u32 getOnEvtBit();
    u32 getOffEvtBit();
    u8 getOnSwBit();
    u8 getOffSwBit();
    u8 getProcType();
    BOOL isDelete();
    BOOL chkPointInArea(cXyz);
    BOOL ECut_talk(int);
    BOOL ECut_wait(int);

    virtual ~daTag_EvtMsg_c();

    static char* mEvtCutNameList[];
    static EvtCutFunc mEvtCutList[];
};

#endif /* D_A_TAG_EVTMSG_H */
