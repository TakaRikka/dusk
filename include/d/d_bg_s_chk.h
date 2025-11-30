#ifndef D_BG_D_BG_S_CHK_H
#define D_BG_D_BG_S_CHK_H

#include "d/d_bg_s_poly_pass_chk.h"
#include "d/d_bg_s_grp_pass_chk.h"

class dBgS_Chk : public dBgS_PolyPassChk, public dBgS_GrpPassChk {
public:
    dBgS_Chk();
    cBgS_PolyPassChk* GetPolyPassChkInfo();
    cBgS_GrpPassChk* GetGrpPassChkInfo();

    virtual ~dBgS_Chk();
};  // Size: 0x18

#endif /* D_BG_D_BG_S_CHK_H */
