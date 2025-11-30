#ifndef D_A_TAG_SS_DRINK_H
#define D_A_TAG_SS_DRINK_H

#include "d/actor/d_a_obj_ss_base.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"

class daTag_SSDrink_c;
typedef int (daTag_SSDrink_c::*ProcessFunc)(void*);

class daTag_SSDrink_c : public daObj_SSBase_c {
public:
    int create();
    int Delete();
    int Execute();
    int Draw();
    int getTypeFromParam();
    u8 getSwitchFromParam();
    u16 getFlowNodeNum();
    u16 getValue();
    void restart();
    void initialize();
    int checkProcess(ProcessFunc);
    int setProcess(ProcessFunc);
    void setAttnPos();
    int chkEvent();
    int orderEvent();
    int wait(void*);
    int talk(void*);
    ~daTag_SSDrink_c();

    s32 fopAcM_seenPlayerAngleY() { return fopAcM_seenActorAngleY(this, dComIfGp_getPlayer(0)); }
    bool checkShopOil() const { return field_0x5D2 == 4; }

    /* 0x578 */ dMsgFlow_c field_0x578;
    /* 0x5C4 */ ProcessFunc mProcessFunc;
    /* 0x5D0 */ u16 mEventType;
    /* 0x5D2 */ u8 field_0x5D2;
    /* 0x5D3 */ u8 field_0x5D3;
    /* 0x5D4 */ u8 field_0x5D4;
    /* 0x5D5 */ u8 field_0x5D5;
};  // Size: 0x5D8
STATIC_ASSERT(sizeof(daTag_SSDrink_c) == 0x5D8);

#endif /* D_A_TAG_SS_DRINK_H */
