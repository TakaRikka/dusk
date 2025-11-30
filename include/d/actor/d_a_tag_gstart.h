#ifndef D_A_TAG_GSTART_H
#define D_A_TAG_GSTART_H

#include "f_op/f_op_actor.h"

class daTagGstart_c : public fopAc_ac_c {
public:
    int create();
    ~daTagGstart_c();
    int execute();

    /* 0x568 */ u8 mSwNo;
    /* 0x569 */ u8 mSwNo2;
    /* 0x56A */ u8 field_0x56a;
    /* 0x56B */ u8 mType;
};

#endif /* D_A_TAG_GSTART_H */
