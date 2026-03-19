#ifndef D_BG_D_BG_S_CAPT_POLY_H
#define D_BG_D_BG_S_CAPT_POLY_H

#include "d/d_bg_s_chk.h"
#include "SSystem/SComponent/c_m3d_g_aab.h"

struct dBgS_CaptPoly;

typedef int (*CaptPolyCallback)(dBgS_CaptPoly*, cBgD_Vtx_t*, int, int, int, cM3dGPla*);

struct dBgS_CaptPoly : public cBgS_Chk, public dBgS_Chk {
public:
    dBgS_CaptPoly();
    ~dBgS_CaptPoly() {}
    void Set(cXyz&, cXyz&);

    void SetCallback(CaptPolyCallback callback) { m_callback = callback; }
    cM3dGAab* GetBndP() { return &m_bnd;}

    /* 0x2C */ cM3dGAab m_bnd;
    /* 0x48 */ CaptPolyCallback m_callback;
};

#endif
