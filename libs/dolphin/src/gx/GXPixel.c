#include <sdk_math.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>

#include "__gx.h"

void GXSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
    u32 fogclr;
    u32 fog0;
    u32 fog1;
    u32 fog2;
    u32 fog3;
    f32 A;
    f32 B;
    f32 B_mant;
    f32 C;
    f32 a;
    f32 c;
    u32 B_expn;
    u32 b_m;
    u32 b_s;
    u32 a_hex;
    u32 c_hex;
    u32 fsel;
    u32 proj;
    u32 rgba;

    fogclr = 0;
    fog0 = 0;
    fog1 = 0;
    fog2 = 0;
    fog3 = 0;

    CHECK_GXBEGIN(138, "GXSetFog");

    ASSERTMSGLINE(140, farz >= 0.0f, "GXSetFog: The farz should be positive value");
    ASSERTMSGLINE(141, farz >= nearz, "GXSetFog: The farz should be larger than nearz");

    fsel = type & 7;
    proj = (type >> 3) & 1;
    
    if (proj) {
        if (farz == nearz || endz == startz) {
            a = 0.0f;
            c = 0.0f;
        } else {
            A = (1.0f / (endz - startz));
            a = A * (farz - nearz);
            c = A * (startz - nearz);
        }
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
        B_expn = 0;
        while (B_mant > 1.0) {
            B_mant /= 2.0f;
            B_expn++;
        }
        while (B_mant > 0.0f && B_mant < 0.5) {
            B_mant *= 2.0f;
            B_expn--;
        }

        a = A / (f32) (1 << (B_expn + 1));
        b_m = 8.388638e6f * B_mant;
        b_s = B_expn + 1;
        c = C;

        SET_REG_FIELD(198, fog1, 24, 0, b_m);
        SET_REG_FIELD(198, fog1, 8, 24, 0xEF);

        SET_REG_FIELD(201, fog2, 5, 0, b_s);
        SET_REG_FIELD(201, fog2, 8, 24, 0xF0);
    }

    a_hex = *(u32 *)&a;
    c_hex = *(u32 *)&c;

    SET_REG_FIELD(209, fog0, 11, 0, (a_hex >> 12) & 0x7FF);
    SET_REG_FIELD(210, fog0, 8, 11, (a_hex >> 23) & 0xFF);
    SET_REG_FIELD(211, fog0, 1, 19, (a_hex >> 31));
    SET_REG_FIELD(211, fog0, 8, 24, 0xEE);

    SET_REG_FIELD(214, fog3, 11, 0, (c_hex >> 12) & 0x7FF);
    SET_REG_FIELD(215, fog3, 8, 11, (c_hex >> 23) & 0xFF);
    SET_REG_FIELD(216, fog3, 1, 19, (c_hex >> 31));

    SET_REG_FIELD(217, fog3, 1, 20, proj);
    SET_REG_FIELD(218, fog3, 3, 21, fsel);
    SET_REG_FIELD(218, fog3, 8, 24, 0xF1);

    rgba = *(u32*)&color;
    SET_REG_FIELD(222, fogclr, 24, 0, rgba >> 8);
    SET_REG_FIELD(222, fogclr, 8, 24, 0xF2);

    GX_WRITE_RAS_REG(fog0);
    GX_WRITE_RAS_REG(fog1);
    GX_WRITE_RAS_REG(fog2);
    GX_WRITE_RAS_REG(fog3);
    GX_WRITE_RAS_REG(fogclr);

    __GXData->bpSentNot = 0;
}

void GXSetFogColor(GXColor color) {
    u32 rgba;
    u32 fogclr = 0xF2000000;

    rgba = *(u32*)&color;
    SET_REG_FIELD(250, fogclr, 24, 0, rgba >> 8);
    GX_WRITE_RAS_REG(fogclr);
    __GXData->bpSentNot = 0;
}

void GXInitFogAdjTable(GXFogAdjTable *table, u16 width, const f32 projmtx[4][4]) {
    f32 xi;
    f32 iw;
    f32 rangeVal;
    f32 nearZ;
    f32 sideX;
    u32 i;

    CHECK_GXBEGIN(275, "GXInitFogAdjTable");
    ASSERTMSGLINE(276, table != NULL, "GXInitFogAdjTable: table pointer is null");
    ASSERTMSGLINE(277, width <= 640, "GXInitFogAdjTable: invalid width value");

    if (0.0 == projmtx[3][3]) {
        nearZ = projmtx[2][3] / (projmtx[2][2] - 1.0f);
        sideX = nearZ / projmtx[0][0];
    } else {
        sideX = 1.0f / projmtx[0][0];
        nearZ = 1.73205f * sideX;
    }

    iw = 2.0f / width;
    for (i = 0; i < 10; i++) {
        xi = (i + 1) << 5;
        xi *= iw;
        xi *= sideX;
        rangeVal = sqrtf(1.0f + ((xi * xi) / (nearZ * nearZ)));
        table->r[i] = (u32)(256.0f * rangeVal) & 0xFFF;
    }
}

void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable *table) {
    u32 i;
    u32 range_adj;
    u32 range_c;

    CHECK_GXBEGIN(331, "GXSetFogRangeAdj");

    if (enable) {
        ASSERTMSGLINE(334, table != NULL, "GXSetFogRangeAdj: table pointer is null");
        for (i = 0; i < 10; i += 2) {
            range_adj = 0;
            SET_REG_FIELD(338, range_adj, 12, 0, table->r[i]);
            SET_REG_FIELD(339, range_adj, 12, 12, table->r[i + 1]);
            SET_REG_FIELD(340, range_adj, 8, 24, (i >> 1) + 0xE9);
            GX_WRITE_RAS_REG(range_adj);
        }
    }
    range_c = 0;
    SET_REG_FIELD(346, range_c, 10, 0, center + 342);
    SET_REG_FIELD(347, range_c, 1, 10, enable);
    SET_REG_FIELD(348, range_c, 8, 24, 0xE8);
    GX_WRITE_RAS_REG(range_c);
    __GXData->bpSentNot = 0;
}

void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op) {
    u32 reg;
    u32 blend_en;

    CHECK_GXBEGIN(375, "GXSetBlendMode");

    reg = __GXData->cmode0;

#if DEBUG
    blend_en = type == GX_BM_BLEND || type == GX_BM_SUBTRACT;
#endif

    SET_REG_FIELD(389, reg, 1, 11, (type == GX_BM_SUBTRACT));
#if DEBUG
    SET_REG_FIELD(392, reg, 1, 0, blend_en);
#else
    SET_REG_FIELD(392, reg, 1, 0, type);
#endif
    SET_REG_FIELD(393, reg, 1, 1, (type == GX_BM_LOGIC));
    SET_REG_FIELD(394, reg, 4, 12, op);
    SET_REG_FIELD(395, reg, 3, 8, src_factor);
    SET_REG_FIELD(396, reg, 3, 5, dst_factor);
    GX_WRITE_RAS_REG(reg);

    __GXData->cmode0 = reg;
    __GXData->bpSentNot = 0;
}

void GXSetColorUpdate(GXBool update_enable) {
    u32 reg;
    CHECK_GXBEGIN(419, "GXSetColorUpdate");

    reg = __GXData->cmode0;

    SET_REG_FIELD(421, reg, 1, 3, update_enable);
    GX_WRITE_RAS_REG(reg);

    __GXData->cmode0 = reg;
    __GXData->bpSentNot = 0;
}

void GXSetAlphaUpdate(GXBool update_enable) {
    u32 reg;
    CHECK_GXBEGIN(432, "GXSetAlphaUpdate");

    reg = __GXData->cmode0;

    SET_REG_FIELD(434, reg, 1, 4, update_enable);
    GX_WRITE_RAS_REG(reg);

    __GXData->cmode0 = reg;
    __GXData->bpSentNot = 0;
}

void GXSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable) {
    u32 reg;
    CHECK_GXBEGIN(459, "GXSetZMode");

    reg = __GXData->zmode;

    SET_REG_FIELD(462, reg, 1, 0, compare_enable);
    SET_REG_FIELD(463, reg, 3, 1, func);
    SET_REG_FIELD(464, reg, 1, 4, update_enable);
    GX_WRITE_RAS_REG(reg);

    __GXData->zmode = reg;
    __GXData->bpSentNot = 0;
}

void GXSetZCompLoc(GXBool before_tex) {
    CHECK_GXBEGIN(474, "GXSetZCompLoc");
    SET_REG_FIELD(475, __GXData->peCtrl, 1, 6, before_tex);
    GX_WRITE_RAS_REG(__GXData->peCtrl);
    __GXData->bpSentNot = 0;
}

void GXSetPixelFmt(GXPixelFmt pix_fmt, GXZFmt16 z_fmt) {
    u32 oldPeCtrl;
    u8 aa;
    static u32 p2f[8] = { 0, 1, 2, 3, 4, 4, 4, 5 };

    CHECK_GXBEGIN(511, "GXSetPixelFmt");
    oldPeCtrl = __GXData->peCtrl;
    ASSERTMSGLINE(515, pix_fmt >= GX_PF_RGB8_Z24 && pix_fmt <= GX_PF_YUV420, "Invalid Pixel format");
    SET_REG_FIELD(517, __GXData->peCtrl, 3, 0, p2f[pix_fmt]);
    SET_REG_FIELD(518, __GXData->peCtrl, 3, 3, z_fmt);

    if (oldPeCtrl != __GXData->peCtrl) {
        GX_WRITE_RAS_REG(__GXData->peCtrl);
        if (pix_fmt == GX_PF_RGB565_Z16)
            aa = 1;
        else
            aa = 0;
        SET_REG_FIELD(527, __GXData->genMode, 1, 9, aa);
        __GXData->dirtyState |= 4;
    }

    if (p2f[pix_fmt] == 4) {
        SET_REG_FIELD(534, __GXData->cmode1, 2, 9, (pix_fmt - 4) & 0x3);
        SET_REG_FIELD(534, __GXData->cmode1, 8, 24, 0x42);
        GX_WRITE_RAS_REG(__GXData->cmode1);
    }

    __GXData->bpSentNot = 0;
}

void GXSetDither(GXBool dither) {
    u32 reg;
    CHECK_GXBEGIN(556, "GXSetDither");

    reg = __GXData->cmode0;

    SET_REG_FIELD(559, reg, 1, 2, dither);
    GX_WRITE_RAS_REG(reg);

    __GXData->cmode0 = reg;
    __GXData->bpSentNot = 0;
}

void GXSetDstAlpha(GXBool enable, u8 alpha) {
    u32 reg;
    CHECK_GXBEGIN(581, "GXSetDstAlpha");

    reg = __GXData->cmode1;

    SET_REG_FIELD(584, reg, 8, 0, alpha);
    SET_REG_FIELD(585, reg, 1, 8, enable);
    GX_WRITE_RAS_REG(reg);

    __GXData->cmode1 = reg;
    __GXData->bpSentNot = 0;
}

void GXSetFieldMask(GXBool odd_mask, GXBool even_mask) {
    u32 reg;

    CHECK_GXBEGIN(608, "GXSetFieldMask");
    reg = 0;
    SET_REG_FIELD(610, reg, 1, 0, even_mask);
    SET_REG_FIELD(611, reg, 1, 1, odd_mask);
    SET_REG_FIELD(611, reg, 8, 24, 0x44);
    GX_WRITE_RAS_REG(reg);
    __GXData->bpSentNot = 0;
}

void GXSetFieldMode(GXBool field_mode, GXBool half_aspect_ratio) {
    u32 reg;

    CHECK_GXBEGIN(637, "GXSetFieldMode");
    SET_REG_FIELD(641, __GXData->lpSize, 1, 22, half_aspect_ratio);
    GX_WRITE_RAS_REG(__GXData->lpSize);
    __GXFlushTextureState();
    reg = field_mode | 0x68000000;
    GX_WRITE_RAS_REG(reg);
    __GXFlushTextureState();
}
