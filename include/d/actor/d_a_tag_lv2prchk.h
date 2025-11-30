#ifndef D_A_TAG_LV2PRCHK_H
#define D_A_TAG_LV2PRCHK_H

#include "f_op/f_op_actor_mng.h"

class daTagLv2PrChk_c : public fopAc_ac_c {
public:
    enum Action_e {
        ACTION_CHECK_e,
        ACTION_END_e,
    };

    enum Mode_e {
        MODE_CHECK_FIRST_SW,
        MODE_CHECK_SECOND_SW,
        MODE_CHECK_THIRD_SW,
        MODE_CHECK_FOURTH_SW,
        MODE_PUZZLE_CLEAR,
        MODE_CHECK_RESET = 200,
        MODE_CHECK_WRONG_ORDER = 100,
    };

    int Create();
    int create();
    int execute();
    void action();
    void actionCheck();
    void actionEnd();
    void seStartWrong();
    int _delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwbit2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwbit3() { return fopAcM_GetParamBit(this, 16, 8); }

    /* 0x568 */ u8 mMode;
    /* 0x569 */ u8 mAction;
    /* 0x56A */ s8 mDirection;  // Signifies starting at either the first (1) or last (-1) switch
    /* 0x56B */ u8 mSwbit2;
    /* 0x56C */ u8 mPlayedSound;
    /* 0x56D */ u8 field_0x56d;
};

#endif /* D_A_TAG_LV2PRCHK_H */
