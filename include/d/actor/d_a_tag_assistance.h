#ifndef D_A_TAG_ASSISTANCE_H
#define D_A_TAG_ASSISTANCE_H

#include "d/d_com_inf_game.h"

class daTagAssist_c : public fopAc_ac_c {
public:
    inline void create_init();
    inline int create();
    inline int execute();

    u8 field_0x568[8];
    s16 field_0x570;
};

#endif /* D_A_TAG_ASSISTANCE_H */
