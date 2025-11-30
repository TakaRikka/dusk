#ifndef D_A_TAG_SMK_EMT_H
#define D_A_TAG_SMK_EMT_H

#include "d/d_com_inf_game.h"

class daTagSmkEmt_c : public fopAc_ac_c {
public:
    daTagSmkEmt_c();
    virtual ~daTagSmkEmt_c();
    int create();
    int Delete();
    int draw();
    int execute();
    void createSmkEmt();
    void deleteSmkEmt();

    /* 0x56C */ JPABaseEmitter* mParticles[3];
};

#endif /* D_A_TAG_SMK_EMT_H */
