#include <revolution/gf.h>
#include <revolution/gd.h>

void GFSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm) {
    static u8 cm2hw[4] = {0, 2, 1, 3};
    GFWriteBPCmd(0xFE07FC3F);
    GFWriteBPCmd(GF_GEN_MODE(nTexGens, nChans, nTevs - 1, cm2hw[cm], nInds));
    GFWriteXFCmd(XF_REG_NUMCOLORS_ID, nChans);
    GFWriteXFCmd(XF_REG_NUMTEX_ID, nTexGens);
}
