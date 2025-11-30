#ifndef D_A_TAG_YAMI_H
#define D_A_TAG_YAMI_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

class daTagYami_c : public fopAc_ac_c {
public:
    int create();
    int Execute();
    void calcCoC(cXyz&);
    void col_set();
    u32 chk_CoHit();
    f32 getPlDist();
    void onMidnaTagSw();
    void offMidnaTagSw2();
    void setCoC(cXyz i_coC) { mCoC = i_coC; }

    u8 get_MTagSw() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 get_MTagSw2() { return fopAcM_GetParamBit(this, 16, 8); }
    s32 get_prm() { return fopAcM_GetParam(this) & 0xff; }

    int is_CoHit() { return mIsCoHit; }

    void on_CoHit() {
        mCyl.OnCoSetBit();
        mCoHitValid = 1;
    }

    void off_CoHit() {
        mCyl.OffCoSetBit();
        mCoHitValid = 0;
    }

    f32 getCoR() { return mCoR; }
    void setCoR(f32 i_value) { mCoR = i_value; }
    void setCoH(f32 i_value) { mCoH = i_value; }

private:
    /* 0x568 */ dCcD_Cyl mCyl;
    /* 0x6A4 */ dCcD_Stts mStts;
    /* 0x6E0 */ u8 mCoHitValid;
    /* 0x6E4 */ u32 mMTagSw;
    /* 0x6E8 */ u32 mMTagSw2;
    /* 0x6EC */ cXyz mCoC;
    /* 0x6F8 */ f32 mCoH;
    /* 0x6FC */ f32 mCoR;
    /* 0x700 */ u32 mIsCoHit;
};

#endif /* D_A_TAG_YAMI_H */
