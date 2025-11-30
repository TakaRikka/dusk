#ifndef D_A_TAG_EVT_H
#define D_A_TAG_EVT_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor.h"

class daTag_Evt_c : public fopAc_ac_c {
public:
    int create();
    int destroy();
    int execute();
    int draw();
    int getParam();
    int isDelete();
    int doEvtCutWait(int);
    int doEvtCutTalk(int);
    int doEvtCutNext(int);

    static char* mEvtCutList[3];

    /* 0x568 */ char field_0x568[8];
    /* 0x570 */ u16 field_0x570;
    /* 0x572 */ s16 field_0x572;
    /* 0x574 */ int field_0x574;
    /* 0x578 */ dMsgFlow_c mMsgFlow;
    /* 0x5C4 */ request_of_phase_process_class mPhase;
    /* 0x5CC */ int mTimer;
    /* 0x5D0 */ int field_0x5D0;
    /* 0x5D4 */ int field_0x5D4;
    /* 0x5D8 */ int field_0x5D8;
    /* 0x5DC */ u8 field_0x5DC;
    /* 0x5DD */ u8 field_0x5DD;
    /* 0x5DE */ u8 field_0x5DE;
    /* 0x5E0 */ u16 field_0x5E0;
    /* 0x5E2 */ u16 field_0x5E2;
    /* 0x5E4 */ s8 field_0x5E4;

    virtual ~daTag_Evt_c();
};

STATIC_ASSERT(sizeof(daTag_Evt_c) == 0x5EC);

#endif /* D_A_TAG_EVT_H */
