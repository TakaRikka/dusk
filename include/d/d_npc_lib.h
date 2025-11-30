#ifndef D_D_NPC_LIB_H
#define D_D_NPC_LIB_H

#include "d/d_com_inf_game.h"

class dNpcLib_lookat_c {
public:
    dNpcLib_lookat_c();
    void init(J3DModel*, int*, csXyz*, csXyz*);
    void action(cXyz, cXyz, fopAc_ac_c*, f32 (*)[4], int);
    void dbView();
    void setPrm();
    void update();
    int limitter(s16*, s16, s16, s16);
    virtual ~dNpcLib_lookat_c() {}

    /* 0x04 */ cXyz field_0x04[4];
    /* 0x34 */ csXyz field_0x34[4];
    /* 0x4C */ csXyz field_0x4c[4];
    /* 0x64 */ csXyz field_0x64[4];
    /* 0x7C */ csXyz field_0x7c[4];
    /* 0x94 */ csXyz field_0x94[4];
    /* 0xAC */ f32 field_0xac[4];
    /* 0xBC */ s32 field_0xbc[4];
    /* 0xCC */ s32 mCount;
};

#endif /* D_D_NPC_LIB_H */
