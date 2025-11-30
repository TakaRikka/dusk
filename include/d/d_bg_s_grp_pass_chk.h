#ifndef D_BG_D_BG_S_GRP_PASS_CHK_H
#define D_BG_D_BG_S_GRP_PASS_CHK_H

#include "SSystem/SComponent/c_bg_s_chk.h"

class dBgS_GrpPassChk : public cBgS_GrpPassChk {
public:
    enum {
        /* 0x1 */ NORMAL_GRP = 1,
        /* 0x2 */ WATER_GRP,
        /* 0x3 */ FULL_GRP
    };

    dBgS_GrpPassChk();

    virtual ~dBgS_GrpPassChk();

    void OnWaterGrp() { mGrp |= WATER_GRP; }
    void OnSpl() { mGrp |= WATER_GRP; }
    void OffWaterGrp() { mGrp &= ~WATER_GRP; }
    void OnNormalGrp() { mGrp |= NORMAL_GRP; }
    void OffNormalGrp() { mGrp &= ~NORMAL_GRP; }
    void OffFullGrp() { mGrp &= ~FULL_GRP; }
    void OnAll() { mGrp |= FULL_GRP; }
    u32 MaskNormalGrp() { return mGrp & NORMAL_GRP; }
    u32 MaskWaterGrp() { return mGrp & WATER_GRP; }
private:
    /* 0x4 */ u32 mGrp;
};

#endif /* D_BG_D_BG_S_GRP_PASS_CHK_H */
