#ifndef D_BG_D_BG_S_SPH_CHK_H
#define D_BG_D_BG_S_SPH_CHK_H

#include "SSystem/SComponent/c_bg_s_poly_info.h"
#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "d/d_bg_s_chk.h"

struct cBgD_Vtx_t;

class dBgS_SphChk;
typedef void (*SphChk_Callback)(dBgS_SphChk*, cBgD_Vtx_t*, int, int, int, cM3dGPla*, void*);

class dBgS_SphChk : public cM3dGSph, public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    dBgS_SphChk();
    virtual ~dBgS_SphChk();

    void SetCallback(SphChk_Callback i_callback) { mCallback = i_callback; }

    /* 0x50 */ SphChk_Callback mCallback;
};

class dBgS_CamSphChk : public dBgS_SphChk {
public:
    dBgS_CamSphChk() { SetCam(); }
    virtual ~dBgS_CamSphChk() {}
};

#endif /* D_BG_D_BG_S_SPH_CHK_H */
