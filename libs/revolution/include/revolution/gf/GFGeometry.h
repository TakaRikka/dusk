#ifndef _REVOLUTION_GF_GFGEOMETRY_H
#define _REVOLUTION_GF_GFGEOMETRY_H

#include <revolution/gx.h>

#define GF_GEN_MODE(nTexGens, nChans, nTevs, cm, nInds) \
    ( \
        (u32)(nTexGens) << 0 | \
        (u32)(nChans) << 4 | \
        (u32)(nTevs) << 10 | \
        (u32)(cm) << 14 | \
        (u32)(nInds) << 16 \
    )

static inline void GFWrite_u8(u8 data) {
    GXCmd1u8(data);
}

static inline void GFWrite_u32(u32 data) {
    GXCmd1u32(data);
}

static inline void GFWrite_u16(u16 data) {
    GXCmd1u16(data);
}

static inline void GFWriteBPCmd(u32 regval) {
    GFWrite_u8(GX_LOAD_BP_REG);
    GFWrite_u32(regval);
}

static inline void GFWriteXFCmd(u16 addr, u32 val) {
    GFWrite_u8(GX_LOAD_XF_REG);
    GFWrite_u16(0);
    GFWrite_u16(addr);
    GFWrite_u32(val);
}

void GFSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm);

#endif /* _REVOLUTION_GF_GFGEOMETRY_H */
