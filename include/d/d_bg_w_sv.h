#ifndef D_BG_D_BG_W_SV_H
#define D_BG_D_BG_W_SV_H

#include "d/d_bg_w.h"

class dBgWSv : public dBgW {
public:
    dBgWSv() {
        field_0xc0 = NULL;
    }

    u8 Set(cBgD_t*, u32);
    void CopyBackVtx();
    bool CrrPosWork(cXyz*, int, int, int);
    bool TransPosWork(cXyz*, int, int, int);

    virtual ~dBgWSv() {}
    virtual void CrrPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*, csXyz*);
    virtual void TransPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*, csXyz*);
    virtual void MatrixCrrPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*, csXyz*);

    /* 0xC0 */ Vec* field_0xc0;
    /* 0xC4 */ u8 field_0xc4;
};

#endif /* D_BG_D_BG_W_SV_H */
