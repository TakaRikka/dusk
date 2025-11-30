//
// J3DGD
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DGD.h"
#include "JSystem/J3DGraphBase/J3DFifo.h"

void J3DGDSetGenMode(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds,
                     GXCullMode cm) {
    static u8 cm2hw[4] = {0, 2, 1, 3};

    GDOverflowCheck(10);
    J3DGDWriteBPCmd(0xFE07FC3F);
    J3DGDWriteBPCmd(BP_GEN_MODE(nTexGens, nChans, nTevs - 1, cm2hw[cm], nInds));
}

void J3DGDSetGenMode_3Param(u8 nTexGens, u8 nTevs, u8 nInds) {
    GDOverflowCheck(10);
    J3DGDWriteBPCmd(0xFE073C0F);
    J3DGDWriteBPCmd(BP_GEN_MODE(nTexGens, 0, nTevs - 1, 0, nInds));
}

void J3DGDSetLightAttn(GXLightID light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2) {
    J3DGDWriteXFCmdHdr(XF_LIGHT_ATTN_ID + __GDLightID2Offset(light), 6);
    J3DGDWrite_f32(a0);
    J3DGDWrite_f32(a1);
    J3DGDWrite_f32(a2);
    J3DGDWrite_f32(k0);
    J3DGDWrite_f32(k1);
    J3DGDWrite_f32(k2);
}

void J3DGDSetLightColor(GXLightID light, GXColor color) {
    J3DGDWriteXFCmd(XF_LIGHT_COLOR_ID + __GDLightID2Offset(light),
                    (color.r << 24) | (color.g << 16) | (color.b << 8) | (color.a << 0));
}

void J3DGDSetLightPos(GXLightID light, f32 x, f32 y, f32 z) {
    J3DGDWriteXFCmdHdr(XF_LIGHT_POS_ID + __GDLightID2Offset(light), 3);
    J3DGDWrite_f32(x);
    J3DGDWrite_f32(y);
    J3DGDWrite_f32(z);
}

void J3DGDSetLightDir(GXLightID light, f32 nx, f32 ny, f32 nz) {
    J3DGDWriteXFCmdHdr(XF_LIGHT_DIR_ID + __GDLightID2Offset(light), 3);
    J3DGDWrite_f32(nx);
    J3DGDWrite_f32(ny);
    J3DGDWrite_f32(nz);
}

void J3DGDSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list, bool param_2) {
    u32 posCnt = GX_POS_XYZ;
    u32 posType = GX_F32;
    u32 posFrac = 0;

    u32 nrmCnt = GX_NRM_XYZ;
    u32 nrmType = GX_F32;
    u32 nrmIdx3 = 0;

    u32 c0Cnt = GX_CLR_RGBA;
    u32 c0Type = GX_RGBA8;

    u32 c1Cnt = GX_CLR_RGBA;
    u32 c1Type = GX_RGBA8;

    u32 tx0Cnt = GX_TEX_ST;
    u32 tx0Type = GX_F32;
    u32 tx0Frac = 0;

    u32 tx1Cnt = GX_TEX_ST;
    u32 tx1Type = GX_F32;
    u32 tx1Frac = 0;

    u32 tx2Cnt = GX_TEX_ST;
    u32 tx2Type = GX_F32;
    u32 tx2Frac = 0;

    u32 tx3Cnt = GX_TEX_ST;
    u32 tx3Type = GX_F32;
    u32 tx3Frac = 0;

    u32 tx4Cnt = GX_TEX_ST;
    u32 tx4Type = GX_F32;
    u32 tx4Frac = 0;

    u32 tx5Cnt = GX_TEX_ST;
    u32 tx5Type = GX_F32;
    u32 tx5Frac = 0;

    u32 tx6Cnt = GX_TEX_ST;
    u32 tx6Type = GX_F32;
    u32 tx6Frac = 0;

    u32 tx7Cnt = GX_TEX_ST;
    u32 tx7Type = GX_F32;
    u32 tx7Frac = 0;

    ASSERTMSGLINE(240, vtxfmt < GX_MAX_VTXFMT, "GDSetVtxAttrFmtv: invalid vtx fmt");

    for (; list->attr != GX_VA_NULL; ++list) {
        ASSERTMSGLINE(245, list->attr >= GX_VA_POS && list->attr <= GX_VA_TEX7, "GDSetVtxAttrFmtv: invalid attribute");
        ASSERTMSGLINE(246, list->frac < 32, "GDSetVtxAttrFmtv: invalid frac value");

        switch (list->attr) {
        case GX_VA_POS:
            posCnt = list->cnt;
            posType = list->type;
            posFrac = list->frac;
            break;
        case GX_VA_NRM:
        case GX_VA_NBT:
            nrmType = list->type;
            if (list->cnt == GX_NRM_NBT3) {
                nrmCnt = GX_NRM_NBT;
                nrmIdx3 = 1;
            } else {
                if (param_2) {
                    nrmCnt = GX_NRM_NBT;
                } else {
                    nrmCnt = list->cnt;
                }
                nrmIdx3 = 0;
            }
            break;
        case GX_VA_CLR0:
            c0Cnt = list->cnt;
            c0Type = list->type;
            break;
        case GX_VA_CLR1:
            c1Cnt = list->cnt;
            c1Type = list->type;
            break;
        case GX_VA_TEX0:
            tx0Cnt = list->cnt;
            tx0Type = list->type;
            tx0Frac = list->frac;
            break;
        case GX_VA_TEX1:
            tx1Cnt = list->cnt;
            tx1Type = list->type;
            tx1Frac = list->frac;
            break;
        case GX_VA_TEX2:
            tx2Cnt = list->cnt;
            tx2Type = list->type;
            tx2Frac = list->frac;
            break;
        case GX_VA_TEX3:
            tx3Cnt = list->cnt;
            tx3Type = list->type;
            tx3Frac = list->frac;
            break;
        case GX_VA_TEX4:
            tx4Cnt = list->cnt;
            tx4Type = list->type;
            tx4Frac = list->frac;
            break;
        case GX_VA_TEX5:
            tx5Cnt = list->cnt;
            tx5Type = list->type;
            tx5Frac = list->frac;
            break;
        case GX_VA_TEX6:
            tx6Cnt = list->cnt;
            tx6Type = list->type;
            tx6Frac = list->frac;
            break;
        case GX_VA_TEX7:
            tx7Cnt = list->cnt;
            tx7Type = list->type;
            tx7Frac = list->frac;
        }
    }

    GDOverflowCheck(18);
    J3DGDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP0_ID, CP_REG_VAT_GRP0(posCnt, posType, posFrac, nrmCnt, nrmType, c0Cnt, c0Type, c1Cnt, c1Type, tx0Cnt, tx0Type, tx0Frac, 1, nrmIdx3));
    J3DGDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP1_ID, CP_REG_VAT_GRP1(tx1Cnt, tx1Type, tx1Frac, tx2Cnt, tx2Type, tx2Frac, tx3Cnt, tx3Type, tx3Frac, tx4Cnt, tx4Type, 1));
    J3DGDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP2_ID, CP_REG_VAT_GRP2(tx4Frac, tx5Cnt, tx5Type, tx5Frac, tx6Cnt, tx6Type, tx6Frac, tx7Cnt, tx7Type, tx7Frac));
}

void J3DGDSetTexCoordGen(GXTexGenType func, GXTexGenSrc src_param) {
    u32 tgType;
    u32 form;
    u32 proj;
    u32 row;
    u32 embossRow;
    u32 embossLit;
    
    form = 0;
    proj = 0;
    row = 5;
    embossRow = 5;
    embossLit = 0;

    switch(src_param) {
    case GX_TG_POS:
        row = 0;
        form = 1;
        break;
    case GX_TG_NRM:
        row = 1;
        form = 1;
        break;
    case GX_TG_BINRM:
        row = 3;
        form = 1;
        break;
    case GX_TG_TANGENT:
        row = 4;
        form = 1;
        break;
    case GX_TG_COLOR0:
        row = 2;
        break;
    case GX_TG_COLOR1:
        row = 2;
        break;
    case GX_TG_TEX0:
        row = 5;
        break;
    case GX_TG_TEX1:
        row = 6;
        break;
    case GX_TG_TEX2:
        row = 7;
        break;
    case GX_TG_TEX3:
        row = 8;
        break;
    case GX_TG_TEX4:
        row = 9;
        break;
    case GX_TG_TEX5:
        row = 10;
        break;
    case GX_TG_TEX6:
        row = 11;
        break;
    case GX_TG_TEX7:
        row = 12;
        break;
    case GX_TG_TEXCOORD0:
        embossRow = 0;
        break;
    case GX_TG_TEXCOORD1:
        embossRow = 1;
        break;
    case GX_TG_TEXCOORD2:
        embossRow = 2;
        break;
    case GX_TG_TEXCOORD3:
        embossRow = 3;
        break;
    case GX_TG_TEXCOORD4:
        embossRow = 4;
        break;
    case GX_TG_TEXCOORD5:
        embossRow = 5;
        break;
    case GX_TG_TEXCOORD6:
        embossRow = 6;
        break;
    default:
        ASSERTMSGLINE(433, 0, "GDSetTexCoordGen: invalid texgen source");
        break;
    }

    switch (func) {
    case GX_TG_MTX2x4:
        tgType = 0;
        break;
    case GX_TG_MTX3x4:
        tgType = 0;
        proj = 1;
        break;
    case GX_TG_BUMP0:
    case GX_TG_BUMP1:
    case GX_TG_BUMP2:
    case GX_TG_BUMP3:
    case GX_TG_BUMP4:
    case GX_TG_BUMP5:
    case GX_TG_BUMP6:
    case GX_TG_BUMP7:
        ASSERTMSGLINE(457, src_param >= GX_TG_TEXCOORD0 && src_param <= GX_TG_TEXCOORD6, "GDSetTexCoordGen: invalid emboss source");
        tgType = 1;
        embossLit = func - GX_TG_BUMP0;
        break;
    case GX_TG_SRTG:
        if (src_param == GX_TG_COLOR0) {
            tgType = 2;
        } else {
            tgType = 3;
        }
        break;
    default:
        ASSERTMSGLINE(473, 0, "GDSetTexCoordGen: invalid texgen function");
        break;
    }

    J3DGDWrite_u32(XF_REG_TEX(proj, form, tgType, row, embossRow, embossLit));
}

void J3DGDSetTexCoordScale2(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                            u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap) {
    GDOverflowCheck(15);
    J3DGDWriteBPCmd(0xFE03FFFF);
    J3DGDWriteBPCmd(BP_TEXCOORD_S_SCALE(s_scale - 1, s_bias, s_wrap, 0, 0, coord * 2 + 0x30));
    J3DGDWriteBPCmd(BP_TEXCOORD_T_SCALE(t_scale - 1, t_bias, t_wrap, coord * 2 + 0x31));
}

static u8 J3DGDTexMode0Ids[8] = {
    0x80, 0x81, 0x82, 0x83, 0xA0, 0xA1, 0xA2, 0xA3,
};

static u8 J3DGDTexMode1Ids[8] = {
    0x84, 0x85, 0x86, 0x87, 0xA4, 0xA5, 0xA6, 0xA7,
};

static u8 J3DGDTexImage0Ids[8] = {
    0x88, 0x89, 0x8A, 0x8B, 0xA8, 0xA9, 0xAA, 0xAB,
};

static u8 J3DGDTexImage3Ids[8] = {
    0x94, 0x95, 0x96, 0x97, 0xB4, 0xB5, 0xB6, 0xB7,
};

static u8 J3DGDTexTlutIds[8] = {
    0x98, 0x99, 0x9A, 0x9B, 0xB8, 0xB9, 0xBA, 0xBB,
};

static u8 GX2HWFiltConv[6] = {
    0x00, 0x04, 0x01, 0x05, 0x02, 0x06,
};

/* 8030E438-8030E5D4 308D78 019C+00 0/0 1/1 0/0 .text
 * J3DGDSetTexLookupMode__F11_GXTexMapID14_GXTexWrapMode14_GXTexWrapMode12_GXTexFilter12_GXTexFilterfffUcUc13_GXAnisotropy
 */
void J3DGDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrap_s,
                           GXTexWrapMode wrap_t, GXTexFilter min_filt,
                           GXTexFilter mag_filt, f32 min_lod, f32 max_lod,
                           f32 lod_bias, u8 bias_clamp, u8 do_edge_lod,
                           GXAnisotropy max_aniso) {
    J3DGDWriteBPCmd(BP_TEX_MODE0(wrap_s, wrap_t, mag_filt == TRUE, GX2HWFiltConv[min_filt], !do_edge_lod, (u8)(32.0f * lod_bias), max_aniso, bias_clamp, J3DGDTexMode0Ids[id]));
    J3DGDWriteBPCmd(BP_TEX_MODE1((u8)(16.0f * min_lod), (u8)(16.0f * max_lod), J3DGDTexMode1Ids[id]));
}

void J3DGDSetTexImgAttr(GXTexMapID id, u16 width, u16 height, GXTexFmt format) {
    J3DGDWriteBPCmd(BP_IMAGE_ATTR(width - 1, height - 1, format, J3DGDTexImage0Ids[id]));
}

void J3DGDSetTexImgPtr(GXTexMapID id, void* image_ptr) {
    J3DGDWriteBPCmd(BP_IMAGE_PTR(OSCachedToPhysical(image_ptr) >> 5, J3DGDTexImage3Ids[id]));
}

void J3DGDSetTexImgPtrRaw(GXTexMapID id, u32 image_ptr_raw) {
    GDOverflowCheck(5);
    J3DGDWriteBPCmd(BP_IMAGE_PTR(image_ptr_raw, J3DGDTexImage3Ids[id]));
}

void J3DGDSetTexTlut(GXTexMapID id, u32 tmem_addr, GXTlutFmt format) {
    J3DGDWriteBPCmd(BP_TEX_TLUT((tmem_addr - 0x80000) >> 9, format, J3DGDTexTlutIds[id]));
}

void J3DGDLoadTlut(void* tlut_ptr, u32 tmem_addr, GXTlutSize size) {
    ASSERTMSGLINE(735, !(tmem_addr & 0x1ff), "GDLoadTlut: invalid TMEM pointer");
    ASSERTMSGLINE(736, size <= 0x400, "GDLoadTlut: invalid TLUT size");

    J3DGDWriteBPCmd(0xFEFFFF00);
    J3DGDWriteBPCmd(0xF000000);
    J3DGDWriteBPCmd(BP_LOAD_TLUT0(OSCachedToPhysical(tlut_ptr) >> 5, 0x64));
    J3DGDWriteBPCmd(BP_LOAD_TLUT1((tmem_addr - 0x80000) >> 9, size, 0x65));
    J3DGDWriteBPCmd(0xFEFFFF00);
    J3DGDWriteBPCmd(0xF000000);
}

void J3DGDSetIndTexMtx(GXIndTexMtxID mtx_id, f32 offset[2][3], s8 scale_exp) {
    s32 offsetS32[6];
    u32 id_offset;

    switch (mtx_id) {
    case GX_ITM_0:
    case GX_ITM_1:
    case GX_ITM_2:
        id_offset = (u32)(mtx_id - GX_ITM_0);
        break;
    case GX_ITM_S0:
    case GX_ITM_S1:
    case GX_ITM_S2:
        id_offset = (u32)(mtx_id - GX_ITM_S0);
        break;
    case GX_ITM_T0:
    case GX_ITM_T1:
    case GX_ITM_T2:
        id_offset = (u32)(mtx_id - GX_ITM_T0);
        break;
    default:
        id_offset = 0;
        break;
    }

    offsetS32[0] = (s32)(offset[0][0] * 0x400) & 0x7FF;
    offsetS32[1] = (s32)(offset[1][0] * 0x400) & 0x7FF;

    offsetS32[2] = (s32)(offset[0][1] * 0x400) & 0x7FF;
    offsetS32[3] = (s32)(offset[1][1] * 0x400) & 0x7FF;

    offsetS32[4] = (s32)(offset[0][2] * 0x400) & 0x7FF;
    offsetS32[5] = (s32)(offset[1][2] * 0x400) & 0x7FF;

    scale_exp += (s8)17;
    GDOverflowCheck(15);

    J3DGDWriteBPCmd(BP_IND_MTX(
        offsetS32[0],
        offsetS32[1],
        scale_exp & 3,
        6 + id_offset * 3
    ));

    J3DGDWriteBPCmd(BP_IND_MTX(
        offsetS32[2],
        offsetS32[3],
        (scale_exp >> 2) & 3,
        7 + id_offset * 3
    ));

    J3DGDWriteBPCmd(BP_IND_MTX(
        offsetS32[4],
        offsetS32[5],
        (scale_exp >> 4) & 3,
        8 + id_offset * 3
    ));
}

/* 8030EE10-8030EF08 309750 00F8+00 0/0 2/2 0/0 .text
 * J3DGDSetIndTexCoordScale__F16_GXIndTexStageID14_GXIndTexScale14_GXIndTexScale14_GXIndTexScale14_GXIndTexScale
 */
void J3DGDSetIndTexCoordScale(GXIndTexStageID indStageEven, GXIndTexScale scaleS0,
                              GXIndTexScale scaleT0, GXIndTexScale scaleS1,
                              GXIndTexScale scaleT1) {
    GDOverflowCheck(5);
    J3DGDWriteBPCmd(BP_IND_TEXCOORD_SCALE(
        scaleS0,
        scaleT0,
        scaleS1,
        scaleT1,
        0x25 + (indStageEven >> 1)
    ));
}

/* 8030EF08-8030F108 309848 0200+00 0/0 2/2 0/0 .text
 * J3DGDSetIndTexOrder__FUl13_GXTexCoordID11_GXTexMapID13_GXTexCoordID11_GXTexMapID13_GXTexCoordID11_GXTexMapID13_GXTexCoordID11_GXTexMapID
 */
void J3DGDSetIndTexOrder(u32 count, GXTexCoordID texCoord0, GXTexMapID texMap0,
                         GXTexCoordID texCoord1, GXTexMapID texMap1,
                         GXTexCoordID texCoord2, GXTexMapID texMap2,
                         GXTexCoordID texCoord3, GXTexMapID texMap3) {
    GDOverflowCheck(10);
    J3DGDWriteBPCmd(BP_IND_TEX_ORDER(
        texMap0 & 7,
        texCoord0 & 7,
        texMap1 & 7,
        texCoord1 & 7,
        texMap2 & 7,
        texCoord2 & 7,
        texMap3 & 7,
        texCoord3 & 7,
        0x27
    ));

    u32 mask = 0;
    for (u32 i = 0; i < count; i++) {
        switch (i) {
        case 0:
            mask |= 1 << (texMap0 & 7);
            break;
        case 1:
            mask |= 1 << (texMap1 & 7);
            break;
        case 2:
            mask |= 1 << (texMap2 & 7);
            break;
        case 3:
            mask |= 1 << (texMap3 & 7);
            break;
        }
    }

    J3DGDWriteBPCmd(BP_IND_MASK(mask, 0xF));
}

/* 8030F108-8030F294 309A48 018C+00 0/0 9/9 0/0 .text
 * J3DGDSetTevOrder__F13_GXTevStageID13_GXTexCoordID11_GXTexMapID12_GXChannelID13_GXTexCoordID11_GXTexMapID12_GXChannelID
 */
void J3DGDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0,
                      GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                      GXChannelID color1) {
    static u8 c2r[] = {0, 1, 0, 1, 0, 1, 7, 5, 6, 0, 0, 0, 0, 0, 0, 7};

    GXTexCoordID coord0_ = coord0 >= GX_MAX_TEXCOORD ? GX_TEXCOORD0 : coord0;
    GXTexCoordID coord1_ = coord1 >= GX_MAX_TEXCOORD ? GX_TEXCOORD0 : coord1;
    GDOverflowCheck(5);
    J3DGDWriteBPCmd(BP_TEV_ORDER(
        map0 & 7,
        coord0_,
        map0 != GX_TEXMAP_NULL && !(map0 & GX_TEX_DISABLE),
        c2r[color0 & 0xF],
        map1 & 7,
        coord1_,
        map1 != GX_TEXMAP_NULL && !(map1 & GX_TEX_DISABLE),
        c2r[color1 & 0xF],
        evenStage / 2 + 0x28
    ));
}

void J3DGDSetTevKColor(GXTevKColorID reg, GXColor color) {
    u32 regRA;
    u32 regBG;

    regRA = BP_TEV_COLOR_REG_RA(color.r, color.a, 1, 0xE0 + reg * 2);
    regBG = BP_TEV_COLOR_REG_BG(color.b, color.g, 1, 0xE1 + reg * 2);

    GDOverflowCheck(0xa);
    J3DGDWriteBPCmd(regRA);
    J3DGDWriteBPCmd(regBG);
}

void J3DGDSetTevColorS10(GXTevRegID reg, GXColorS10 color) {
    u32 regRA;
    u32 regBG;

    regRA = BP_TEV_COLOR_REG_RA(color.r & 0x7FF, color.a & 0x7FF, 0, 0xE0 + reg * 2);
    regBG = BP_TEV_COLOR_REG_BG(color.b & 0x7FF, color.g & 0x7FF, 0, 0xE1 + reg * 2);

    GDOverflowCheck(0x14);
    J3DGDWriteBPCmd(regRA);
    J3DGDWriteBPCmd(regBG);
    J3DGDWriteBPCmd(regBG);
    J3DGDWriteBPCmd(regBG);
}

void J3DGDSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
    f32 A;
    f32 B;
    f32 B_mant;
    f32 C;
    f32 A_f;
    u32 b_expn;
    u32 b_m;
    u32 a_hex;
    u32 c_hex;

    ASSERTMSGLINE(1036, farz >= 0.0f, "GDSetFog: The farz should be positive value");
    ASSERTMSGLINE(1037, farz >= nearz, "GDSetFog: The farz should be larger than nearz");


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

    J3DGDWriteBPCmd(BP_FOG_UNK0(a_hex >> 12, 0xEE));
    J3DGDWriteBPCmd(BP_FOG_UNK1(b_m, 0xEF));
    J3DGDWriteBPCmd(BP_FOG_UNK2(b_expn, 0xF0));
    J3DGDWriteBPCmd(BP_FOG_UNK3(c_hex >> 12, 0, type, 0xF1));
    J3DGDWriteBPCmd(BP_FOG_COLOR(color.r, color.g, color.b, 0xF2));
}

#define BP_FOG_RANGE_ADJ_K0(arg0, arg1, arg2) \
    ( \
        (u32)(arg2) << 24 | \
        (u32)(arg1) << 12 | \
        (u32)(arg0) << 0 \
    )

#define BP_FOG_RANGE_ADJ(arg0, arg1, arg2) \
    ( \
        (u32)(arg2) << 24 | \
        (u32)(arg0) << 0 | \
        (u32)(arg1) << 10 \
    )

void J3DGDSetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable* table) {
    if (enable) {
        for (int i = 0; i < 10; i += 2) {
            u32 range_adj = BP_FOG_RANGE_ADJ_K0(table->r[i], table->r[i + 1], i / 2 + 0xE9);
            J3DGDWriteBPCmd(range_adj);
        }
    }

    u32 range_c = BP_FOG_RANGE_ADJ(center + 342, enable, GX_BP_REG_FOGRANGE);
    J3DGDWriteBPCmd(range_c);
}

void J3DFifoLoadPosMtxImm(MtxP mtx, u32 id) {
    J3DFifoWriteXFCmdHdr(4 * id, 12);
    J3DGXCmd1f32ptr(&mtx[0][0]);
    J3DGXCmd1f32ptr(&mtx[0][1]);
    J3DGXCmd1f32ptr(&mtx[0][2]);
    J3DGXCmd1f32ptr(&mtx[0][3]);
    J3DGXCmd1f32ptr(&mtx[1][0]);
    J3DGXCmd1f32ptr(&mtx[1][1]);
    J3DGXCmd1f32ptr(&mtx[1][2]);
    J3DGXCmd1f32ptr(&mtx[1][3]);
    J3DGXCmd1f32ptr(&mtx[2][0]);
    J3DGXCmd1f32ptr(&mtx[2][1]);
    J3DGXCmd1f32ptr(&mtx[2][2]);
    J3DGXCmd1f32ptr(&mtx[2][3]);
}

void J3DFifoLoadNrmMtxImm(MtxP mtx, u32 id) {
    J3DFifoWriteXFCmdHdr(id * 3 + 0x400, 9);
    J3DGXCmd1f32ptr(&mtx[0][0]);
    J3DGXCmd1f32ptr(&mtx[0][1]);
    J3DGXCmd1f32ptr(&mtx[0][2]);
    J3DGXCmd1f32ptr(&mtx[1][0]);
    J3DGXCmd1f32ptr(&mtx[1][1]);
    J3DGXCmd1f32ptr(&mtx[1][2]);
    J3DGXCmd1f32ptr(&mtx[2][0]);
    J3DGXCmd1f32ptr(&mtx[2][1]);
    J3DGXCmd1f32ptr(&mtx[2][2]);
}

void J3DFifoLoadNrmMtxImm3x3(Mtx3P mtx, u32 id) {
    J3DFifoWriteXFCmdHdr(id * 3 + 0x400, 9);
    J3DGXCmd1f32ptr(&mtx[0][0]);
    J3DGXCmd1f32ptr(&mtx[0][1]);
    J3DGXCmd1f32ptr(&mtx[0][2]);
    J3DGXCmd1f32ptr(&mtx[1][0]);
    J3DGXCmd1f32ptr(&mtx[1][1]);
    J3DGXCmd1f32ptr(&mtx[1][2]);
    J3DGXCmd1f32ptr(&mtx[2][0]);
    J3DGXCmd1f32ptr(&mtx[2][1]);
    J3DGXCmd1f32ptr(&mtx[2][2]);
}

void J3DFifoLoadNrmMtxToTexMtx(MtxP mtx, u32 id) {
    J3DFifoWriteXFCmdHdr(4 * id, 12);
    J3DGXCmd1f32ptr(&mtx[0][0]);
    J3DGXCmd1f32ptr(&mtx[0][1]);
    J3DGXCmd1f32ptr(&mtx[0][2]);
    J3DGXCmd1f32(0.0f);
    J3DGXCmd1f32ptr(&mtx[1][0]);
    J3DGXCmd1f32ptr(&mtx[1][1]);
    J3DGXCmd1f32ptr(&mtx[1][2]);
    J3DGXCmd1f32(0.0f);
    J3DGXCmd1f32ptr(&mtx[2][0]);
    J3DGXCmd1f32ptr(&mtx[2][1]);
    J3DGXCmd1f32ptr(&mtx[2][2]);
    J3DGXCmd1f32(0.0f);
}

void J3DFifoLoadNrmMtxToTexMtx3x3(Mtx3P mtx, u32 id) {
    J3DFifoWriteXFCmdHdr(4 * id, 0xc);
    J3DGXCmd1f32ptr(&mtx[0][0]);
    J3DGXCmd1f32ptr(&mtx[0][1]);
    J3DGXCmd1f32ptr(&mtx[0][2]);
    J3DGXCmd1f32(0.0f);
    J3DGXCmd1f32ptr(&mtx[1][0]);
    J3DGXCmd1f32ptr(&mtx[1][1]);
    J3DGXCmd1f32ptr(&mtx[1][2]);
    J3DGXCmd1f32(0.0f);
    J3DGXCmd1f32ptr(&mtx[2][0]);
    J3DGXCmd1f32ptr(&mtx[2][1]);
    J3DGXCmd1f32ptr(&mtx[2][2]);
    J3DGXCmd1f32(0.0f);
}

static u8 J3DTexImage1Ids[8] = {
    0x8C, 0x8D, 0x8E, 0x8F, 0xAC, 0xAD, 0xAE, 0xAF,
};

static u8 J3DTexImage2Ids[8] = {
    0x90, 0x91, 0x92, 0x93, 0xB0, 0xB1, 0xB2, 0xB3,
};

void J3DFifoLoadTexCached(GXTexMapID id, u32 tmem_even, GXTexCacheSize size_even,
                          u32 tmem_odd, GXTexCacheSize size_odd) {
    J3DFifoLoadBPCmd(BP_TEX_CACHE_EVEN(tmem_even >> 5, size_even + 3, size_even + 3, 0, J3DTexImage1Ids[id]));

    if (size_odd != 3 && tmem_odd < 0x100000) {
        J3DFifoLoadBPCmd(BP_TEX_CACHE_ODD(tmem_odd >> 5, size_odd + 3, size_odd + 3, J3DTexImage2Ids[id]));
    }
}
