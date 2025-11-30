#ifndef D_A_MANT_H
#define D_A_MANT_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class mant_class
 * @brief Ganondorf's Cloak
 *
 */

class daMant_packet_c : public J3DPacket {
public:
    void draw();
    ~daMant_packet_c();

    /* 0x0010 */ Mtx mMtx;
    /* 0x0040 */ Mtx mMtx2;
    /* 0x0070 */ dKy_tevstr_c* mTevStr;
    /* 0x0074 */ u8 field_0x74;
    /* 0x0078 */ cXyz mPos[2][169];
    /* 0x1050 */ cXyz mNrm[2][169];
    /* 0x2028 */ u8 field_0x2028;
    /* 0x2029 */ u8 mArg0;

    daMant_packet_c() {
        field_0x74 = 0;
    }

    cXyz* getPos() {
        return &mPos[field_0x74][0];
    }

    cXyz* getNrm() {
        return &mNrm[field_0x74][0];
    }

    void setTevStr(dKy_tevstr_c* tevStr) {
        mTevStr = tevStr;
    }

    MtxP getMtx2() {
        return mMtx2;
    }

    MtxP getMtx() {
        return mMtx;
    }
};
struct mant_j_s {
    public:
        ~mant_j_s();
        mant_j_s();
    
        /* 0x0000 */ cXyz field_0x0[13];
        /* 0x009C */ cXyz field_0x9c[13];
        /* 0x0138 */ s16 field_0x0138;
        /* 0x013A */ s16 field_0x013a;
        /* 0x013C */ u8 field_0x013C[0x174 - 0x13C];
        /* 0x0174 */ f32 field_0x0174;
        /* 0x0178 */ f32 field_0x0178;
        /* 0x017C */ f32 field_0x017c;
};
class mant_class : public fopAc_ac_c {
public:
    /* 0x0568 */ u8 field_0x568[0x570 - 0x568];
    /* 0x0570 */ daMant_packet_c field_0x0570;
    /* 0x259C */ u8 field_0x259c;
    /* 0x259D */ u8 field_0x259D[0x25A0 - 0x259D];
    /* 0x25A0 */ u32 field_0x25a0;
    /* 0x2595 */ u8 field_0x25A5[0x25A8 - 0x25A5];
    /* 0x25A8 */ mant_j_s field_0x25a8[13];
    /* 0x3928 */ cXyz field_0x3928[2];
    /* 0x3940 */ cXyz field_0x3940;
    /* 0x394C */ f32 field_0x394c;
    /* 0x3950 */ f32 field_0x3950;
    /* 0x3954 */ f32 field_0x3954;
    /* 0x3958 */ f32 field_0x3958;
    /* 0x395C */ f32 field_0x395c;
    /* 0x3960 */ f32 field_0x3960;
    /* 0x3964 */ u8 field_0x3964;
    /* 0x3965 */ u8 field_0x3965;
    /* 0x3966 */ u8 field_0x3966;
    /* 0x3967 */ u8 field_0x3967;
    /* 0x3968 */ u8 field_0x3968;
    /* 0x3969 */ u8 field_0x3969;
    /* 0x396A */ u8 field_0x396A[0x399E - 0x396A];
    /* 0x399E */ s16 field_0x399e;
    /* 0x39A0 */ u8 field_0x39A0[0x39A4 - 0x39A0];
    
};

STATIC_ASSERT(sizeof(mant_class) == 0x39a4);

#endif /* D_A_MANT_H */
