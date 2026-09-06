#ifndef D_A_TAG_TELOP_H
#define D_A_TAG_TELOP_H

#include "d/d_com_inf_game.h"

class daTag_Telop_c : public fopAc_ac_c {
public:
    daTag_Telop_c() {}
    ~daTag_Telop_c() {}

    int create();
    void execute();

    u16 getMessageNo() { return home.angle.z; }

    /* 0x568 */ u16 mMessageNo;
    /* 0x56C */ int mMessageStatus;
};

#endif /* D_A_TAG_TELOP_H */
