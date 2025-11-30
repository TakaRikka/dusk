#ifndef D_A_ANDSW_H
#define D_A_ANDSW_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-unsorted
 * @class daAndsw_c
 * @brief
 *
 * @details
 *
 */
class daAndsw_c : public fopAc_ac_c {
public:
    u8 getSwNo() {
        return fopAcM_GetParamBit(this,0,8);
    }

    u8 getSwNo2() {
        return fopAcM_GetParamBit(this,8,8);
    }

    u8 getTimer() {
        return fopAcM_GetParamBit(this,16,8);
    }

    u8 getType() {
        return fopAcM_GetParamBit(this,24,8);
    }

    int Create();
    int create();
    int execute();
    int _delete();

    /* 0x568 */ u8 mSwNo;
    /* 0x568 */ u8 mSwNo2;
    /* 0x56A */ s16 mTimer;
};

class daAndsw_HIO_c : public mDoHIO_entry_c {
public:
    daAndsw_HIO_c();

    void genMessage(JORMContext*);

    /* 0x06 */ u8 field_0x6;
};


#endif /* D_A_ANDSW_H */
