#ifndef D_A_TAG_CAMERA_H
#define D_A_TAG_CAMERA_H

#include "f_op/f_op_actor_mng.h"

class daTag_Cam_c : public fopAc_ac_c {
public:
    u8 getSwType();
    u8 getPrio();
    u8 getSwBit();
    u8 getCondition();
    u8 getCameraId();
    u8 getRailID();
    u16 getAreaType();
    u16 getAreaNoChk();
    int execute();

    inline int create();

    /* 0x568 */ u8 field_0x568[0x570 - 0x568];
    /* 0x570 */ bool (*mCheckFunc)();
    /* 0x574 */ cXyz mBoundsLo;
    /* 0x580 */ cXyz mBoundsHi;
};

#endif /* D_A_TAG_CAMERA_H */
