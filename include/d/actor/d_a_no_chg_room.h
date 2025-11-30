#ifndef D_A_D_A_NO_CHG_ROOM_H
#define D_A_D_A_NO_CHG_ROOM_H

#include "f_op/f_op_actor_mng.h"

class daNocrm_c : public fopAc_ac_c {
public:
    int execute();
    int getRoomNo(int);
    inline int create();

    s8 getReadRoomId() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getScaleType() { return fopAcM_GetParamBit(this, 8, 4); }

    /* 0x568 */ Mtx mMtx;
    /* 0x598 */ Mtx mInvMtx;
    /* 0x5C8 */ s8 mRoomNo[2];
    /* 0x5CA */ bool mRoomLoading;
};

#endif /* D_A_D_A_NO_CHG_ROOM_H */
