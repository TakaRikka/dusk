#ifndef _D_BG_S_CAPT_POLY_H
#define _D_BG_S_CAPT_POLY_H

#include "d/d_bg_s_chk.h"
#include "SSystem/SComponent/c_m3d_g_aab.h"
#include "SSystem/SComponent/c_xyz.h"

class dBgS_CaptPoly : public cBgS_Chk, public dBgS_Chk {
public:
    dBgS_CaptPoly();
    ~dBgS_CaptPoly() {}
    void Set(cXyz&, cXyz&);

    /* 0x24 */ cM3dGAab aab;
    /* 0x48 */ int field_0x48;
};

#endif /* _D_BG_S_CAPT_POLY_H */
