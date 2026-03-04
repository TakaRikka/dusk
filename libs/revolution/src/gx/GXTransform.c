#include <revolution/gx.h>
#include <revolution/mtx.h>
#include <revolution/os.h>

#include "__gx.h"

void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz) {
    Vec peye;
    f32 xc;
    f32 yc;
    f32 zc;
    f32 wc;

    ASSERTMSGLINE(181, pm && vp && sx && sy && sz, "GXGet*: invalid null pointer");

    peye.x = mtx[0][3] + ((mtx[0][2] * z) + ((mtx[0][0] * x) + (mtx[0][1] * y)));
    peye.y = mtx[1][3] + ((mtx[1][2] * z) + ((mtx[1][0] * x) + (mtx[1][1] * y)));
    peye.z = mtx[2][3] + ((mtx[2][2] * z) + ((mtx[2][0] * x) + (mtx[2][1] * y)));
    if (pm[0] == 0.0f) {
        xc = (peye.x * pm[1]) + (peye.z * pm[2]);
        yc = (peye.y * pm[3]) + (peye.z * pm[4]);
        zc = pm[6] + (peye.z * pm[5]);
        wc = 1.0f / -peye.z;
    } else {
        xc = pm[2] + (peye.x * pm[1]);
        yc = pm[4] + (peye.y * pm[3]);
        zc = pm[6] + (peye.z * pm[5]);
        wc = 1.0f;
    }
    *sx = (vp[2] / 2.0f) + (vp[0] + (wc * (xc * vp[2] / 2.0f)));
    *sy = (vp[3] / 2.0f) + (vp[1] + (wc * (-yc * vp[3] / 2.0f)));
    *sz = vp[5] + (wc * (zc * (vp[5] - vp[4])));
}

static void WriteProjPS(const __REGISTER f32 proj[6], __REGISTER volatile void* dest) {
    __REGISTER f32 p01, p23, p45;

#ifdef __MWERKS__
    asm {
        psq_l  p01,  0(proj), 0, 0
        psq_l  p23,  8(proj), 0, 0
        psq_l  p45, 16(proj), 0, 0
        psq_st p01,  0(dest), 0, 0
        psq_st p23,  0(dest), 0, 0
        psq_st p45,  0(dest), 0, 0
    }
#endif
}

static void Copy6Floats(const __REGISTER f32 src[6], __REGISTER volatile f32* dest) {
    __REGISTER f32 ps01, ps23, ps45;

    asm {
        psq_l  ps01,  0(src), 0, 0
        psq_l  ps23,  8(src), 0, 0
        psq_l  ps45, 16(src), 0, 0
        psq_st ps01,  0(dest), 0, 0
        psq_st ps23,  8(dest), 0, 0
        psq_st ps45, 16(dest), 0, 0
    }
}

void __GXSetProjection(void) {
    u32 reg = 0x00061020;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
#if DEBUG
    GX_WRITE_XF_REG_F(32, __GXData->projMtx[0]);
    GX_WRITE_XF_REG_F(33, __GXData->projMtx[1]);
    GX_WRITE_XF_REG_F(34, __GXData->projMtx[2]);
    GX_WRITE_XF_REG_F(35, __GXData->projMtx[3]);
    GX_WRITE_XF_REG_F(36, __GXData->projMtx[4]);
    GX_WRITE_XF_REG_F(37, __GXData->projMtx[5]);
    GX_WRITE_XF_REG_2(38, __GXData->projType);
#else
    WriteProjPS(__GXData->projMtx, (volatile void*)GXFIFO_ADDR);
    GX_WRITE_U32(__GXData->projType);
#endif
}

void GXSetProjection(const Mtx44 mtx, GXProjectionType type) {
    CHECK_GXBEGIN(307, "GXSetProjection");

    __GXData->projType = type;
    __GXData->projMtx[0] = mtx[0][0];
    __GXData->projMtx[2] = mtx[1][1];
    __GXData->projMtx[4] = mtx[2][2];
    __GXData->projMtx[5] = mtx[2][3];
    if (type == GX_ORTHOGRAPHIC) {
        __GXData->projMtx[1] = mtx[0][3];
        __GXData->projMtx[3] = mtx[1][3];
    } else {
        __GXData->projMtx[1] = mtx[0][2];
        __GXData->projMtx[3] = mtx[1][2];
    }

    __GXData->dirtyState |= 0x8000000;
}

void GXSetProjectionv(const f32* ptr) {
    CHECK_GXBEGIN(351, "GXSetProjectionv");

    __GXData->projType = ptr[0] == 0.0f ? GX_PERSPECTIVE : GX_ORTHOGRAPHIC;

#if DEBUG
    __GXData->projMtx[0] = ptr[1];
    __GXData->projMtx[1] = ptr[2];
    __GXData->projMtx[2] = ptr[3];
    __GXData->projMtx[3] = ptr[4];
    __GXData->projMtx[4] = ptr[5];
    __GXData->projMtx[5] = ptr[6];
#else
    Copy6Floats(&ptr[1], __GXData->projMtx);
#endif

    __GXData->dirtyState |= 0x8000000;
}

#define qr0 0

void GXGetProjectionv(f32* ptr) {
    ASSERTMSGLINE(382, ptr, "GXGet*: invalid null pointer");

    ptr[0] = (u32)__GXData->projType != GX_PERSPECTIVE ? 1.0f : 0.0f;

#if DEBUG
    ptr[1] = __GXData->projMtx[0];
    ptr[2] = __GXData->projMtx[1];
    ptr[3] = __GXData->projMtx[2];
    ptr[4] = __GXData->projMtx[3];
    ptr[5] = __GXData->projMtx[4];
    ptr[6] = __GXData->projMtx[5];
#else
    Copy6Floats(__GXData->projMtx, &ptr[1]);
#endif
}

static void WriteMTXPS4x3(const __REGISTER f32 mtx[3][4], __REGISTER volatile f32* dest) {
    __REGISTER f32 a00_a01;
    __REGISTER f32 a02_a03;
    __REGISTER f32 a10_a11;
    __REGISTER f32 a12_a13;
    __REGISTER f32 a20_a21;
    __REGISTER f32 a22_a23;

    asm {
        psq_l a00_a01, 0x00(mtx), 0, qr0
        psq_l a02_a03, 0x08(mtx), 0, qr0
        psq_l a10_a11, 0x10(mtx), 0, qr0
        psq_l a12_a13, 0x18(mtx), 0, qr0
        psq_l a20_a21, 0x20(mtx), 0, qr0
        psq_l a22_a23, 0x28(mtx), 0, qr0
        psq_st a00_a01, 0(dest), 0, qr0
        psq_st a02_a03, 0(dest), 0, qr0
        psq_st a10_a11, 0(dest), 0, qr0
        psq_st a12_a13, 0(dest), 0, qr0
        psq_st a20_a21, 0(dest), 0, qr0
        psq_st a22_a23, 0(dest), 0, qr0
    }
}

static void WriteMTXPS3x3from3x4(__REGISTER f32 mtx[3][4], __REGISTER volatile f32* dest) {
    __REGISTER f32 a00_a01;
    __REGISTER f32 a02_a03;
    __REGISTER f32 a10_a11;
    __REGISTER f32 a12_a13;
    __REGISTER f32 a20_a21;
    __REGISTER f32 a22_a23;

    asm {
        psq_l  a00_a01, 0x00(mtx), 0, qr0
        lfs    a02_a03, 0x08(mtx)
        psq_l  a10_a11, 0x10(mtx), 0, qr0
        lfs    a12_a13, 0x18(mtx)
        psq_l  a20_a21, 0x20(mtx), 0, qr0
        lfs    a22_a23, 0x28(mtx)
        psq_st a00_a01, 0(dest), 0, qr0
        stfs   a02_a03, 0(dest)
        psq_st a10_a11, 0(dest), 0, qr0
        stfs   a12_a13, 0(dest)
        psq_st a20_a21, 0(dest), 0, qr0
        stfs   a22_a23, 0(dest)
    }
}

static void WriteMTXPS3x3(__REGISTER f32 mtx[3][3], __REGISTER volatile f32* dest) {
    __REGISTER f32 a00_a01;
    __REGISTER f32 a02_a10;
    __REGISTER f32 a11_a12;
    __REGISTER f32 a20_a21;
    __REGISTER f32 a22_nnn;

    asm {
        psq_l a00_a01, 0x00(mtx), 0, qr0
        psq_l a02_a10, 0x08(mtx), 0, qr0
        psq_l a11_a12, 0x10(mtx), 0, qr0
        psq_l a20_a21, 0x18(mtx), 0, qr0
        lfs   a22_nnn, 0x20(mtx)
        psq_st a00_a01, 0(dest), 0, qr0
        psq_st a02_a10, 0(dest), 0, qr0
        psq_st a11_a12, 0(dest), 0, qr0
        psq_st a20_a21, 0(dest), 0, qr0
        stfs   a22_nnn, 0(dest)
    }
}

static void WriteMTXPS4x2(const __REGISTER f32 mtx[2][4], __REGISTER volatile f32* dest) {
    __REGISTER f32 a00_a01;
    __REGISTER f32 a02_a03;
    __REGISTER f32 a10_a11;
    __REGISTER f32 a12_a13;

    asm {
        psq_l a00_a01, 0x00(mtx), 0, qr0
        psq_l a02_a03, 0x08(mtx), 0, qr0
        psq_l a10_a11, 0x10(mtx), 0, qr0
        psq_l a12_a13, 0x18(mtx), 0, qr0
        psq_st a00_a01, 0(dest), 0, qr0
        psq_st a02_a03, 0(dest), 0, qr0
        psq_st a10_a11, 0(dest), 0, qr0
        psq_st a12_a13, 0(dest), 0, qr0
    }
}

#define GX_WRITE_MTX_ELEM(addr, value) \
do { \
    f32 xfData = (value); \
    GX_WRITE_F32(value); \
    VERIF_MTXLIGHT((addr), *(u32 *)&xfData); \
} while (0)

void GXLoadPosMtxImm(const Mtx mtx, u32 id) {
    u32 reg;
    u32 addr;

    CHECK_GXBEGIN(516, "GXLoadPosMtxImm");

    addr = id * 4;
    reg = addr | 0xB0000;

    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
#if DEBUG
    GX_WRITE_MTX_ELEM(addr + 0, mtx[0][0]);
    GX_WRITE_MTX_ELEM(addr + 1, mtx[0][1]);
    GX_WRITE_MTX_ELEM(addr + 2, mtx[0][2]);
    GX_WRITE_MTX_ELEM(addr + 3, mtx[0][3]);
    GX_WRITE_MTX_ELEM(addr + 4, mtx[1][0]);
    GX_WRITE_MTX_ELEM(addr + 5, mtx[1][1]);
    GX_WRITE_MTX_ELEM(addr + 6, mtx[1][2]);
    GX_WRITE_MTX_ELEM(addr + 7, mtx[1][3]);
    GX_WRITE_MTX_ELEM(addr + 8, mtx[2][0]);
    GX_WRITE_MTX_ELEM(addr + 9, mtx[2][1]);
    GX_WRITE_MTX_ELEM(addr + 10, mtx[2][2]);
    GX_WRITE_MTX_ELEM(addr + 11, mtx[2][3]);
#else
    WriteMTXPS4x3(mtx, &GXWGFifo.f32);
#endif
}

void GXLoadPosMtxIndx(u16 mtx_indx, u32 id) {
    u32 offset;
    u32 reg;

    CHECK_GXBEGIN(555, "GXLoadPosMtxIndx");
    offset = id * 4;
    reg = 0;
    SET_REG_FIELD(561, reg, 12, 0, offset);
    SET_REG_FIELD(563, reg, 4, 12, 11);
    SET_REG_FIELD(563, reg, 16, 16, mtx_indx);
    GX_WRITE_U8(0x20);
    GX_WRITE_U32(reg);
#if DEBUG
    __GXShadowIndexState(4, reg);
#endif
}

void GXLoadNrmMtxImm(const Mtx mtx, u32 id) {
    u32 reg;
    u32 addr;

    CHECK_GXBEGIN(597, "GXLoadNrmMtxImm");

    addr = id * 3 + 0x400;
    reg = addr | 0x80000;

    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
#if DEBUG
    GX_WRITE_MTX_ELEM(addr + 0, mtx[0][0]);
    GX_WRITE_MTX_ELEM(addr + 1, mtx[0][1]);
    GX_WRITE_MTX_ELEM(addr + 2, mtx[0][2]);
    GX_WRITE_MTX_ELEM(addr + 3, mtx[1][0]);
    GX_WRITE_MTX_ELEM(addr + 4, mtx[1][1]);
    GX_WRITE_MTX_ELEM(addr + 5, mtx[1][2]);
    GX_WRITE_MTX_ELEM(addr + 6, mtx[2][0]);
    GX_WRITE_MTX_ELEM(addr + 7, mtx[2][1]);
    GX_WRITE_MTX_ELEM(addr + 8, mtx[2][2]);
#else
    WriteMTXPS3x3from3x4((void*)mtx, &GXWGFifo.f32);
#endif
}

void GXLoadNrmMtxImm3x3(const f32 mtx[3][3], u32 id) {
    u32 reg;
    u32 addr;

    CHECK_GXBEGIN(633, "GXLoadNrmMtxImm3x3");

    addr = id * 3 + 0x400;
    reg = addr | 0x80000;

    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
#if DEBUG
    GX_WRITE_MTX_ELEM(addr + 0, mtx[0][0]);
    GX_WRITE_MTX_ELEM(addr + 1, mtx[0][1]);
    GX_WRITE_MTX_ELEM(addr + 2, mtx[0][2]);
    GX_WRITE_MTX_ELEM(addr + 3, mtx[1][0]);
    GX_WRITE_MTX_ELEM(addr + 4, mtx[1][1]);
    GX_WRITE_MTX_ELEM(addr + 5, mtx[1][2]);
    GX_WRITE_MTX_ELEM(addr + 6, mtx[2][0]);
    GX_WRITE_MTX_ELEM(addr + 7, mtx[2][1]);
    GX_WRITE_MTX_ELEM(addr + 8, mtx[2][2]);
#else
    WriteMTXPS3x3((void*)mtx, &GXWGFifo.f32);
#endif
}

void GXLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id) {
    u32 offset;
    u32 reg;

    CHECK_GXBEGIN(679, "GXLoadNrmMtxIndx3x3");
    offset = id * 3 + 0x400;
    reg = 0;
    SET_REG_FIELD(685, reg, 12, 0, offset);
    SET_REG_FIELD(687, reg, 4, 12, 8);
    SET_REG_FIELD(687, reg, 16, 16, mtx_indx);
    GX_WRITE_U8(0x28);
    GX_WRITE_U32(reg);
#if DEBUG
    __GXShadowIndexState(5, reg);
#endif
}

void GXSetCurrentMtx(u32 id) {
    CHECK_GXBEGIN(717, "GXSetCurrentMtx");
    SET_REG_FIELD(721, __GXData->matIdxA, 6, 0, id);
    __GXData->dirtyState |= 0x4000000;
}

void GXLoadTexMtxImm(const f32 mtx[][4], u32 id, GXTexMtxType type) {
    u32 reg;
    u32 addr;
    u32 count;

    CHECK_GXBEGIN(750, "GXLoadTexMtxImm");

    if (id >= GX_PTTEXMTX0) {
        addr = (id - GX_PTTEXMTX0) * 4 + 0x500;
        ASSERTMSGLINE(760, type == GX_MTX3x4, "GXLoadTexMtx: Invalid matrix type");
    } else {
        addr = id * 4;
    }
    count = (type == GX_MTX2x4) ? 8 : 12;
    reg = addr | ((count - 1) << 16);

    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
#if DEBUG
    GX_WRITE_MTX_ELEM(addr + 0, mtx[0][0]);
    GX_WRITE_MTX_ELEM(addr + 1, mtx[0][1]);
    GX_WRITE_MTX_ELEM(addr + 2, mtx[0][2]);
    GX_WRITE_MTX_ELEM(addr + 3, mtx[0][3]);
    GX_WRITE_MTX_ELEM(addr + 4, mtx[1][0]);
    GX_WRITE_MTX_ELEM(addr + 5, mtx[1][1]);
    GX_WRITE_MTX_ELEM(addr + 6, mtx[1][2]);
    GX_WRITE_MTX_ELEM(addr + 7, mtx[1][3]);
    if (type == GX_MTX3x4) {
        GX_WRITE_MTX_ELEM(addr + 8, mtx[2][0]);
        GX_WRITE_MTX_ELEM(addr + 9, mtx[2][1]);
        GX_WRITE_MTX_ELEM(addr + 10, mtx[2][2]);
        GX_WRITE_MTX_ELEM(addr + 11, mtx[2][3]);
    }
#else
    if (type == GX_MTX3x4) {
        WriteMTXPS4x3(mtx, &GXWGFifo.f32);
    } else {
        WriteMTXPS4x2(mtx, &GXWGFifo.f32);
    }
#endif
}

void GXLoadTexMtxIndx(u16 mtx_indx, u32 id, GXTexMtxType type) {
    u32 offset;
    u32 reg;
    u32 count;

    CHECK_GXBEGIN(813, "GXLoadTexMtxIndx");

    if (id >= GX_PTTEXMTX0) {
        offset = (id - GX_PTTEXMTX0) * 4 + 0x500;
        ASSERTMSGLINE(0x337, type == GX_MTX3x4, "GXLoadTexMtx: Invalid matrix type");
    } else {
        offset = id * 4;
    }
    count = (type == GX_MTX2x4) ? 8 : 12;

    reg = 0;
    SET_REG_FIELD(830, reg, 12, 0, offset);
    SET_REG_FIELD(831, reg, 4, 12, (count - 1));
    SET_REG_FIELD(832, reg, 16, 16, mtx_indx);
    GX_WRITE_U8(0x30);
    GX_WRITE_U32(reg);
#if DEBUG
    __GXShadowIndexState(6, reg);
#endif
}

void __GXSetViewport(void) {
    f32 sx;
    f32 sy;
    f32 sz;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 zmin;
    f32 zmax;
    u32 reg;

    sx = __GXData->vpWd / 2.0f;
    sy = -__GXData->vpHt / 2.0f;
    ox = 342.0f + (__GXData->vpLeft + (__GXData->vpWd / 2.0f));
    oy = 342.0f + (__GXData->vpTop + (__GXData->vpHt / 2.0f));

    zmin = __GXData->vpNearz * __GXData->zScale;
    zmax = __GXData->vpFarz * __GXData->zScale;

    sz = zmax - zmin;
    oz = zmax + __GXData->zOffset;

    reg = 0x5101A;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(reg);
    GX_WRITE_XF_REG_F(26, sx);
    GX_WRITE_XF_REG_F(27, sy);
    GX_WRITE_XF_REG_F(28, sz);
    GX_WRITE_XF_REG_F(29, ox);
    GX_WRITE_XF_REG_F(30, oy);
    GX_WRITE_XF_REG_F(31, oz);
}

void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz, u32 field) {
    CHECK_GXBEGIN(912, "GXSetViewport");  // not the correct function name

    if (field == 0) {
        top -= 0.5f;
    }

    __GXData->vpLeft = left;
    __GXData->vpTop = top;
    __GXData->vpWd = wd;
    __GXData->vpHt = ht;
    __GXData->vpNearz = nearz;
    __GXData->vpFarz = farz;
    
    __GXData->dirtyState |= 0x10000000;
}

void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz) {
    GXSetViewportJitter(left, top, wd, ht, nearz, farz, 1);
}

void GXGetViewportv(f32* vp) {
    ASSERTMSGLINE(977, vp, "GXGet*: invalid null pointer");

#if DEBUG
    vp[0] = __GXData->vpLeft;
    vp[1] = __GXData->vpTop;
    vp[2] = __GXData->vpWd;
    vp[3] = __GXData->vpHt;
    vp[4] = __GXData->vpNearz;
    vp[5] = __GXData->vpFarz;
#else
    Copy6Floats(&__GXData->vpLeft, vp);
#endif
}

#define GX_WRITE_XF_REG_F_(addr, value) \
do { \
    GX_WRITE_U8(0x10); \
    GX_WRITE_U32(0x1000 + (addr)); \
    { \
        f32 xfData = (value); \
        GX_WRITE_F32(value); \
        VERIF_XF_REG_alt(addr, *(u32 *)&xfData); \
    } \
} while (0)

void GXSetZScaleOffset(f32 scale, f32 offset) {
    f32 sz;
    f32 oz;
    f32 zmin;
    f32 zmax;

    CHECK_GXBEGIN(996, "GXSetZScaleOffset");

    oz = offset * 1.6777215e7f;
    __GXData->zOffset = oz;

    sz = (scale * 1.6777215e7f) + 1.0f;
    __GXData->zScale = sz;

    zmin = __GXData->vpNearz * sz;
    zmax = __GXData->vpFarz * sz;
    sz = zmax - zmin;
    oz = oz + zmax;

    GX_WRITE_XF_REG_F_(0x1C, sz);
    GX_WRITE_XF_REG_F_(0x1F, oz);

    __GXData->bpSentNot = 1;
}

void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht) {
    u32 tp;
    u32 lf;
    u32 bm;
    u32 rt;

    CHECK_GXBEGIN(1057, "GXSetScissor");
    ASSERTMSGLINE(1058, left < 1706, "GXSetScissor: Left origin > 1708");
    ASSERTMSGLINE(1059, top < 1706, "GXSetScissor: top origin > 1708");
    ASSERTMSGLINE(1060, left + wd < 1706, "GXSetScissor: right edge > 1708");
    ASSERTMSGLINE(1061, top + ht < 1706, "GXSetScissor: bottom edge > 1708");

    tp = top + 342;
    lf = left + 342;
    bm = tp + ht - 1;
    rt = lf + wd - 1;

    SET_REG_FIELD(1068, __GXData->suScis0, 11, 0, tp);
    SET_REG_FIELD(1069, __GXData->suScis0, 11, 12, lf);
    SET_REG_FIELD(1071, __GXData->suScis1, 11, 0, bm);
    SET_REG_FIELD(1072, __GXData->suScis1, 11, 12, rt);

    GX_WRITE_RAS_REG(__GXData->suScis0);
    GX_WRITE_RAS_REG(__GXData->suScis1);
    __GXData->bpSentNot = 0;
}

void GXGetScissor(u32* left, u32* top, u32* wd, u32* ht) {
    u32 tp;
    u32 lf;
    u32 bm;
    u32 rt;

    ASSERTMSGLINE(1098, left && top && wd && ht, "GXGet*: invalid null pointer");

    tp = __GXData->suScis0 & 0x7FF;
    lf = (__GXData->suScis0 & 0x7FF000) >> 12;
    bm = __GXData->suScis1 & 0x7FF;
    rt = (__GXData->suScis1 & 0x7FF000) >> 12;

    *left = lf - 342;
    *top = tp - 342;
    *wd = rt - lf + 1;
    *ht = bm - tp + 1;
}

void GXSetScissorBoxOffset(s32 x_off, s32 y_off) {
    u32 reg = 0;
    u32 hx;
    u32 hy;

    CHECK_GXBEGIN(1128, "GXSetScissorBoxOffset");

    ASSERTMSGLINE(1131, !!((u32)(x_off + 342) < 2048), "GXSetScissorBoxOffset: Invalid X offset");
    ASSERTMSGLINE(1133, !!((u32)(y_off + 342) < 2048), "GXSetScissorBoxOffset: Invalid Y offset");

    hx = (u32)(x_off + 342) >> 1;
    hy = (u32)(y_off + 342) >> 1;

    SET_REG_FIELD(1138, reg, 10, 0, hx);
    SET_REG_FIELD(1139, reg, 10, 10, hy);
    SET_REG_FIELD(1131, reg, 8, 24, 0x59);
    GX_WRITE_RAS_REG(reg);
    __GXData->bpSentNot = 0;
}

void GXSetClipMode(GXClipMode mode) {
    CHECK_GXBEGIN(1160, "GXSetClipMode");
    GX_WRITE_XF_REG(0x1005, mode, 0);
    __GXData->bpSentNot = 1;
}

void __GXSetMatrixIndex(GXAttr matIdxAttr) {
    if (matIdxAttr < GX_VA_TEX4MTXIDX) {
        GX_WRITE_SOME_REG4(8, 0x30, __GXData->matIdxA, -12);
        GX_WRITE_XF_REG(0x1018, __GXData->matIdxA, 0);
    } else {
        GX_WRITE_SOME_REG4(8, 0x40, __GXData->matIdxB, -12);
        GX_WRITE_XF_REG(0x1019, __GXData->matIdxB, 0);
    }
    __GXData->bpSentNot = 1;
}
