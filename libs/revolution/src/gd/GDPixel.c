#include <revolution/gd.h>
#include <revolution/os.h>

void GDSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
    f32 A;
    f32 B;
    f32 B_mant;
    f32 C;
    f32 A_f;
    u32 b_expn;
    u32 b_m;
    u32 a_hex;
    u32 c_hex;
    u32 fsel;
    u32 proj;

    ASSERTMSGLINE(62, farz >= 0.0f, "GDSetFog: The farz should be positive value");
    ASSERTMSGLINE(63, farz >= nearz, "GDSetFog: The farz should be larger than nearz");

    fsel = type & 7;
    proj = (type >> 3) & 1;

    if (proj != 0) {
        if (farz == nearz || endz == startz) {
            A_f = 0.0f;
            C = 0.0f;
        } else {
            A = 1.0f / (endz - startz);
            A_f = (farz - nearz) * A;
            C = (startz - nearz) * A;
        }

        b_expn = 0;
        b_m = 0;
    } else {
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
    }

    a_hex = *(u32*)&A_f;
    c_hex = *(u32*)&C;

    GDWriteBPCmd(BP_FOG_UNK0(a_hex >> 12, 0xEE));
    GDWriteBPCmd(BP_FOG_UNK1(b_m, 0xEF));
    GDWriteBPCmd(BP_FOG_UNK2(b_expn, 0xF0));
    GDWriteBPCmd(BP_FOG_UNK3(c_hex >> 12, proj, fsel, 0xF1));
    GDWriteBPCmd(BP_FOG_COLOR(color.r, color.g, color.b, 0xF2));
}

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp logic_op) {
    GDWriteBPCmd(0xFE00FFE3);
    GDWriteBPCmd(BP_BLEND_MODE(
        type == GX_BM_BLEND || type == GX_BM_SUBTRACT,
        type == GX_BM_LOGIC,
        0,
        0,
        0,
        dst_factor,
        src_factor,
        type == GX_BM_SUBTRACT,
        logic_op,
        0x41
    ));
}

void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable) {
    GDWriteBPCmd(BP_BLEND_MODE(
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

void GDSetZMode(u8 compare_enable, GXCompare func, u8 update_enable) {
    GDWriteBPCmd(BP_Z_MODE(compare_enable, func, update_enable, 0x40));
}

void GDSetDstAlpha(u8 enable, u8 alpha) {
    GDWriteBPCmd(BP_DST_ALPHA(alpha, enable, 0x42));
}

void GDSetDrawSync(u16 token) {
    GDWriteBPCmd(BP_TOKEN(token, 0x48));
    GDWriteBPCmd(BP_TOKEN(token, 0x47));
}
