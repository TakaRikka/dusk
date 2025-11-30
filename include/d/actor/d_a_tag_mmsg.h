#ifndef D_A_TAG_MMSG_H
#define D_A_TAG_MMSG_H

#include "f_op/f_op_actor.h"

class daTagMmsg_c : public fopAc_ac_c {
public:
    daTagMmsg_c() {}

    int create();
    ~daTagMmsg_c();
    int execute();

    void onUseFlg() { mUseFlg = true; }
    bool checkNoAttention() { return mAttention == 0xFF; }

private:
    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 field_0x569;
    /* 0x56A */ u8 mAttention;
    /* 0x56B */ bool field_0x56b;
    /* 0x56C */ bool mUseFlg;
    /* 0x56D */ u8 field_0x56d[3];
    /* 0x570 */ u16 field_0x570;
    /* 0x572 */ u16 field_0x572;
    /* 0x574 */ f32 field_0x574;
    /* 0x578 */ f32 field_0x578;
};

#endif /* D_A_TAG_MMSG_H */
