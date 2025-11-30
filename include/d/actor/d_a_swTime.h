#ifndef D_A_SWTIME_H
#define D_A_SWTIME_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-unsorted
 * @class daSwTime_c
 * @brief Switch Time
 *
 * @details
 *
 */
class daSwTime_c : public fopAc_ac_c {
public:
    u8 getTime() {
        return fopAcM_GetParamBit(this,0,8);
    }

    u8 getSwbit() {
        return fopAcM_GetParamBit(this,8,8);
    }

    u8 getSwbit2() {
        return fopAcM_GetParamBit(this,16,8);
    }

    int Create();
    int create();
    int execute();
    int _delete();

private:
    /* 0x568 */ u8 mTime;
    /* 0x569 */ u8 mSwbit;
    /* 0x56A */ u8 mSwbit2;
};

STATIC_ASSERT(sizeof(daSwTime_c) == 0x56C);

#endif /* D_A_SWTIME_H */
