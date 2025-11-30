#ifndef D_A_TAG_MYNA_LIGHT_H
#define D_A_TAG_MYNA_LIGHT_H

#include "f_op/f_op_actor_mng.h"

class daTag_MynaLight_c : public fopAc_ac_c {
public:
    int create();
    int Delete();
    int Execute();
    int Draw();
    bool setTurnOnOffChange();
    u32 getTurnOnTime();
    u32 getTurnOffTime();
    void initialize();

    virtual ~daTag_MynaLight_c();

    bool checkTurnOnOffChange() {
        return (field_0x578 != mTurnOnFlag) ? true : false;
    }

    bool getTurnOnFlag() { return mTurnOnFlag; }

    /* 0x568 */  // vtable
    /* 0x56C */ f32 field_0x56c;
    /* 0x570 */ JPABaseEmitter* field_0x570[2];
    /* 0x578 */ bool field_0x578;
    /* 0x579 */ bool mTurnOnFlag;
};  // Size: 0x57C

STATIC_ASSERT(sizeof(daTag_MynaLight_c) == 0x57C);

#endif /* D_A_TAG_MYNA_LIGHT_H */
