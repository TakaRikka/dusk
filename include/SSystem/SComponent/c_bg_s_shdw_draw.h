#ifndef C_BG_S_SHDW_DRAW_H
#define C_BG_S_SHDW_DRAW_H

#include "SSystem/SComponent/c_bg_s_chk.h"
#include "SSystem/SComponent/c_m3d_g_aab.h"

struct cBgD_Vtx_t;
class cM3dGPla;

typedef int (*cBgS_ShdwDraw_Callback)(class cBgS_ShdwDraw*, cBgD_Vtx_t*, int, int, int, cM3dGPla*);

class cBgS_ShdwDraw : public cBgS_Chk {
public:
    cBgS_ShdwDraw(void);
    virtual ~cBgS_ShdwDraw(void);
    void Set(cXyz&, cXyz&);
    void SetCallback(cBgS_ShdwDraw_Callback);

    cM3dGAab* GetBndP() { return &mM3dGAab; }

    /* 0x14 */ cM3dGAab mM3dGAab;
    /* 0x30 */ cBgS_ShdwDraw_Callback mCallbackFun;
    #if DEBUG
    /* 0x34 */ int field_0x34;
    #endif
};

#endif /* C_BG_S_SHDW_DRAW_H */
