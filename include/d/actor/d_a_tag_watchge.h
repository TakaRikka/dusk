#ifndef D_A_TAG_WATCHGE_H
#define D_A_TAG_WATCHGE_H

#include "d/actor/d_a_e_ge.h"
#include "d/d_com_inf_game.h"

class daTagWatchGe_c : public fopAc_ac_c {
public:
    u8 getGroupNo() {
        return mGroupNo;
    }

    int draw();
    int execute();
    int create();

    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 mGroupNo;
    /* 0x56A */ u8 field_0x56a;
};

#endif /* D_A_TAG_WATCHGE_H */
