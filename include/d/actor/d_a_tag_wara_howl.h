#ifndef D_A_TAG_WARA_HOWL_H
#define D_A_TAG_WARA_HOWL_H

#include "f_op/f_op_actor_mng.h"

class daTagWrHowl_c : public fopAc_ac_c {
public:
    daTagWrHowl_c();
    int create();
    int Delete();
    int draw();
    int execute();
    void init();
    bool chkWlfInTag();

    virtual ~daTagWrHowl_c();

    s8 getTuneId() { return fopAcM_GetParamBit(this, 4, 4); }

    /* 0x56C */ f32 field_0x56c;
    /* 0x570 */ f32 field_0x570;
};

#endif /* D_A_TAG_WARA_HOWL_H */
