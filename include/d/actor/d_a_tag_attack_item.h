#ifndef D_A_TAG_ATTACK_ITEM_H
#define D_A_TAG_ATTACK_ITEM_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_event_lib.h"
#include "d/d_cc_d.h"

class daTagAtkItem_c : public fopAc_ac_c , public dEvLib_callback_c {
public:
    daTagAtkItem_c() : dEvLib_callback_c(this) {}

    void setBaseMtx();
    int Create();
    int create();
    int execute();
    void action();
    BOOL checkHit();
    void createItem();
    int _delete();

    virtual ~daTagAtkItem_c() {}
    virtual bool eventStart();
    virtual bool eventEnd();

    u8 getEvId() { return fopAcM_GetParamBit(this, 0x18, 8); }
    u8 getNum() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getItemBit() { return fopAcM_GetParamBit(this, 0x10, 8); }
    u8 getItemNo() { return fopAcM_GetParamBit(this, 0, 8); }

    /* 0x57C */ u8 field_0x57C[0x584 - 0x57C];
    /* 0x584 */ dCcD_Stts mCcStts;
    /* 0x5C0 */ dCcD_Cyl mCyl;
};

#endif /* D_A_TAG_ATTACK_ITEM_H */
