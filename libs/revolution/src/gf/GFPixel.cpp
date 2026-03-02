#include <revolution/gf.h>
#include <revolution/gd.h>

void GFSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
    f32 A;
    f32 B;
    f32 B_mant;
    f32 C;
    f32 A_f;
    u32 b_expn;
    u32 b_m;
    u32 a_hex;
    u32 c_hex;

    ASSERTMSGLINE(62, farz >= 0.0f, "GFSetFog: The farz should be positive value");
    ASSERTMSGLINE(63, farz >= nearz, "GFSetFog: The farz should be larger than nearz");

    if (farz == nearz || endz == startz) {
        A = 0.0f;
        B = 0.5f;
        C = 0.0f;
    } else {
        A = (farz * nearz) / ((farz - nearz) * (endz - startz));
        B = farz / (farz - nearz);
        C = startz / (endz - startz);
    }

    B_mant = B;
    b_expn = 1;

    while (B_mant > 1.0) {
        B_mant *= 0.5f;
        b_expn++;
    }

    while (B_mant > 0.0f && B_mant < 0.5) {
        B_mant *= 2.0f;
        b_expn--;
    }

    A_f = A / (1 << b_expn);
    b_m = (u32) (8388638.0f * B_mant);

    a_hex = *(u32*)&A_f;
    c_hex = *(u32*)&C;

    GFWriteBPCmd(BP_FOG_UNK0(a_hex >> 12, 0xEE));
    GFWriteBPCmd(BP_FOG_UNK1(b_m, 0xEF));
    GFWriteBPCmd(BP_FOG_UNK2(b_expn, 0xF0));
    GFWriteBPCmd(BP_FOG_UNK3(c_hex >> 12, 0, type, 0xF1));
    GFWriteBPCmd(BP_FOG_COLOR(color.r, color.g, color.b, 0xF2));
}

void GFSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable) {
    GFWriteBPCmd(BP_BLEND_MODE(
        type == GX_BM_BLEND || type == GX_BM_SUBTRACT,
        type == GX_BM_LOGIC,
        dither_enable,
        color_update_enable,
        alpha_update_enable,
        dst_factor,
        src_factor,
        type == GX_BM_SUBTRACT,
        logic_op,
        0x41
    ));
}

void GFSetZMode(u8 compare_enable, GXCompare func, u8 update_enable) {
    GFWriteBPCmd(BP_Z_MODE(compare_enable, func, update_enable, 0x40));
}
