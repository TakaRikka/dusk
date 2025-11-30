#ifndef D_A_TAG_PACHI_H
#define D_A_TAG_PACHI_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

class daTagPati_c : public fopAc_ac_c {
public:
    ~daTagPati_c();
    int create();
    int Execute();
    void col_set();
    u32 chk_CoHit();

    u32 get_prm() {
        u32 retVal = fopAcM_GetParam(this) & 0xFF;
        if (retVal == 0xFF) {
            retVal = 0;
        }
        return retVal;
    }

    BOOL isDistChkTag1() { return field_0x6E0 == 0; }
    BOOL isDistChkTag2() { return field_0x6E0 == 1; }
    BOOL isEscapeChkTag1() { return field_0x6E0 == 2; }
    BOOL isEscapeChkTag2() { return field_0x6E0 == 3; }
    BOOL is_CoHit() const { return field_0x6E4; }
    void clr_CoHit() { 
        mCyl.ClrCoHit();
        field_0x6E4 = 0;
    }

    /* 0x568 */ dCcD_Cyl mCyl;
    /* 0x6A4 */ dCcD_Stts mStts;
    /* 0x6E0 */ u32 field_0x6E0;
    /* 0x6E4 */ u32 field_0x6E4;
};

STATIC_ASSERT(sizeof(daTagPati_c) == 0x6E8);

#endif /* D_A_TAG_PACHI_H */
