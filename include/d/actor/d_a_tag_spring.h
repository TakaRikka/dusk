#ifndef D_A_TAG_SPRING_H
#define D_A_TAG_SPRING_H

#include "d/d_com_inf_game.h"

class daTagSpring_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    u8 checkArea();
    int _delete();

    u32 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }

    /* 0x568 */ u8 mTimer;
};  // Size: 0x56C

#endif /* D_A_TAG_SPRING_H */
