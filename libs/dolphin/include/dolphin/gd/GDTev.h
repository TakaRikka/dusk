#ifndef _DOLPHIN_GD_TEV_H
#define _DOLPHIN_GD_TEV_H

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BP_TEV_COLOR(d, c, b, a, bias, op, clamp, scale, out, id) \
    ( \
        (u32)(d) << 0 | \
        (u32)(c) << 4 | \
        (u32)(b) << 8 | \
        (u32)(a) << 12 | \
        (u32)(bias) << 16 | \
        (u32)(op) << 18 | \
        (u32)(clamp) << 19 | \
        (u32)(scale) << 20 | \
        (u32)(out) << 22 | \
        (u32)(id) << 24 \
    )

#define BP_TEV_ALPHA(ras_sel, tex_sel, d, c, b, a, bias, op, clamp, scale, out, id) \
    ( \
        (u32)(ras_sel) << 0 | \
        (u32)(tex_sel) << 2 | \
        (u32)(d) << 4 | \
        (u32)(c) << 7 | \
        (u32)(b) << 10 | \
        (u32)(a) << 13 | \
        (u32)(bias) << 16 | \
        (u32)(op) << 18 | \
        (u32)(clamp) << 19 | \
        (u32)(scale) << 20 | \
        (u32)(out) << 22 | \
        (u32)(id) << 24 \
    )

#define BP_TEV_COLOR_REG_RA(r, a, reg, id) \
    ( \
        (u32)(r) << 0 | \
        (u32)(a) << 12 | \
        (u32)(reg) << 23 | \
        (u32)(id) << 24 \
    )

#define BP_TEV_COLOR_REG_BG(b, g, reg, id) \
    ( \
        (u32)(b) << 0 | \
        (u32)(g) << 12 | \
        (u32)(reg) << 23 | \
        (u32)(id) << 24 \
    )

#define BP_TEV_KSEL(rb, ga, kcsel0, kasel0, kcsel1, kasel1, id) \
    ( \
        (u32)(rb) << 0 | \
        (u32)(ga) << 2 | \
        (u32)(kcsel0) << 4 | \
        (u32)(kasel0) << 9 | \
        (u32)(kcsel1) << 14 | \
        (u32)(kasel1) << 19 | \
        (u32)(id) << 24 \
    )

#define BP_ALPHA_COMPARE(ref0, ref1, comp0, comp1, op, id) \
    ( \
        (u32)(ref0) << 0 | \
        (u32)(ref1) << 8 | \
        (u32)(comp0) << 16 | \
        (u32)(comp1) << 19 | \
        (u32)(op) << 22 | \
        (u32)(id) << 24 \
    )

#define BP_ZTEX_PARAMS_0(bias, id) \
    ( \
        (u32)(bias) << 0 | \
        (u32)(id) << 24 \
    )

#define BP_ZTEX_PARAMS_1(zfmt, op, id) \
    ( \
        (u32)(zfmt) << 0 | \
        (u32)(op) << 2 | \
        (u32)(id) << 24 \
    )

#define BP_TEV_ORDER(map0, coord0, enable0, color0, map1, coord1, enable1, color1, id) \
    ( \
        (u32)(map0) << 0 | \
        (u32)(coord0) << 3 | \
        (u32)(enable0) << 6 | \
        (u32)(color0) << 7 | \
        (u32)(map1) << 12 | \
        (u32)(coord1) << 15 | \
        (u32)(enable1) << 18 | \
        (u32)(color1) << 19 | \
        (u32)(id) << 24 \
    )

void GDSetTevOp(GXTevStageID stage, GXTevMode mode);
void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                       GXTevColorArg c, GXTevColorArg d, GXTevOp op,
                       GXTevBias bias, GXTevScale scale, u8 clamp,
                       GXTevRegID out_reg);
void GDSetTevAlphaCalcAndSwap(GXTevStageID stage, GXTevAlphaArg a,
                              GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d,
                              GXTevOp op, GXTevBias bias, GXTevScale scale,
                              u8 clamp, GXTevRegID out_reg,
                              GXTevSwapSel ras_sel, GXTevSwapSel tex_sel);
void GDSetTevColor(GXTevRegID reg, GXColor color);
void GDSetTevColorS10(GXTevRegID reg, GXColorS10 color);
void GDSetTevKColor(GXTevKColorID reg, GXColor color);
void GDSetTevKonstantSel(GXTevStageID evenStage, GXTevKColorSel kcsel0,
                         GXTevKAlphaSel kasel0, GXTevKColorSel kcsel1,
                         GXTevKAlphaSel kasel1);
void GDSetTevSwapModeTable(GXTevSwapSel table, GXTevColorChan red,
                           GXTevColorChan green, GXTevColorChan blue,
                           GXTevColorChan alpha);
void GDSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1);
void GDSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias);
void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0,
                   GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1);

#ifdef __cplusplus
}
#endif

#endif
