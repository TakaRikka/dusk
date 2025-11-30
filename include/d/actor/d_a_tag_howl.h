#ifndef D_A_TAG_HOWL_H
#define D_A_TAG_HOWL_H

#include "f_op/f_op_actor.h"

class daTag_Howl_c : public fopAc_ac_c {
public:
    /* 0x568 */ s8 mCurveID;
    /* 0x569 */ u8 field_0x569;

    s8 getCurveID() { return mCurveID; }

    int create();
    int destroy();
    int execute();
    int draw();
    u8 isAreaCheck();
    int getParam();
    u8 getSwitchBit();
    virtual ~daTag_Howl_c() {}
};  // Size: 0x570

#endif /* D_A_TAG_HOWL_H */
