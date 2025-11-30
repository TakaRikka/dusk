#ifndef D_A_TAG_MAGNE_H
#define D_A_TAG_MAGNE_H

#include "f_op/f_op_actor_mng.h"

class cBgS_PolyInfo;

class daTagMagne_c : public fopAc_ac_c {
public:
    static BOOL checkMagnetCode(cBgS_PolyInfo&);
    int checkMagneA();
    int checkMagneB();
    int checkMagneC();
    int Create();
    int create();
    int _delete();

    u8 getSwNo1() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwNo2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwNo3() { return fopAcM_GetParamBit(this, 16, 8); }

    static daTagMagne_c* mTagMagne;

    /* 0x568 */ u8 mSwNo1;
    /* 0x569 */ u8 mSwNo2;
    /* 0x56A */ u8 mSwNo3;
};

#endif /* D_A_TAG_MAGNE_H */
