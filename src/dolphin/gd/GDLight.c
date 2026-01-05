#include <dolphin/gd.h>
#include <sdk_math.h>

void GDSetLightAttn(GXLightID light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2) {
    GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_ATTN_ID, 6);
    GDWrite_f32(a0);
    GDWrite_f32(a1);
    GDWrite_f32(a2);
    GDWrite_f32(k0);
    GDWrite_f32(k1);
    GDWrite_f32(k2);
}

void GDSetLightSpot(GXLightID light, f32 cutoff, GXSpotFn spot_func) {
    f32 a0;
    f32 a1;
    f32 a2;
    f32 r;
    f32 d;
    f32 cr;

    if (cutoff <= 0.0f || cutoff > 90.0f) {
        spot_func = 0;
    }

    r = M_PI * cutoff / 180.0f;
    cr = cosf(r);

    switch(spot_func) {
    case GX_SP_FLAT:
        a0 = -1000.0f * cr;
        a1 = 1000.0f;
        a2 = 0.0f;
        break;
    case GX_SP_COS:
        a0 = -cr / (1.0f - cr);
        a1 = 1.0f / (1.0f - cr);
        a2 = 0.0f;
        break;
    case GX_SP_COS2:
        a0 = 0.0f;
        a1 = -cr / (1.0f - cr);
        a2 = 1.0f / (1.0f - cr);
        break;
    case GX_SP_SHARP:
        d = (1.0f - cr) * (1.0f - cr);
        a0 = (cr * (cr - 2.0f)) / d;
        a1 = 2.0f / d;
        a2 = -1.0f / d;
        break;
    case GX_SP_RING1:
        d = (1.0f - cr) * (1.0f - cr);
        a0 = (-4.0f * cr) / d;
        a1 = (4.0f * (1.0f + cr)) / d;
        a2 = -4.0f / d;
        break;
    case GX_SP_RING2:
        d = (1.0f - cr) * (1.0f - cr);
        a0 = 1.0f - (2.0f * cr * cr) / d;
        a1 = (4.0f * cr) / d;
        a2 = -2.0f / d;
        break;
    case GX_SP_OFF:
    default:
        a0 = 1.0f;
        a1 = 0.0f;
        a2 = 0.0f;
        break;
    }

    GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_ATTN_ID, 3);
    GDWrite_f32(a0);
    GDWrite_f32(a1);
    GDWrite_f32(a2);
}

void GDSetLightDistAttn(GXLightID light, f32 ref_dist, f32 ref_br, GXDistAttnFn dist_func) {
    f32 k0;
    f32 k1;
    f32 k2;

    if (ref_dist < 0.0f || ref_br <= 0.0f || ref_br >= 1.0f) {
        dist_func = 0;
    }

    switch (dist_func) {
    case GX_DA_GENTLE:
        k0 = 1.0f;
        k1 = (1.0f - ref_br) / (ref_br * ref_dist);
        k2 = 0.0f;
        break;
    case GX_DA_MEDIUM:
        k0 = 1.0f;
        k1 = (0.5f * (1.0f - ref_br)) / (ref_br * ref_dist);
        k2 = (0.5f * (1.0f - ref_br)) / (ref_dist * (ref_br * ref_dist));
        break;
    case GX_DA_STEEP:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = (1.0f - ref_br) / (ref_dist * (ref_br * ref_dist));
        break;
    case GX_DA_OFF:
    default:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = 0.0f;
        break;
    }

    GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_DISTATTN_ID, 3);
    GDWrite_f32(k0);
    GDWrite_f32(k1);
    GDWrite_f32(k2);
}

void GDSetLightColor(GXLightID light, GXColor color) {
    GDWriteXFCmd(__GDLightID2Offset(light) + XF_LIGHT_COLOR_ID, color.r << 24 | color.g << 16 | color.b << 8 | color.a);
}

void GDSetLightPos(GXLightID light, f32 x, f32 y, f32 z) {
    GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_POS_ID, 3);
    GDWrite_f32(x);
    GDWrite_f32(y);
    GDWrite_f32(z);
}

void GDSetLightDir(GXLightID light, f32 nx, f32 ny, f32 nz) {
    GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_DIR_ID, 3);
    GDWrite_f32(nx);
    GDWrite_f32(ny);
    GDWrite_f32(nz);
}

void GDSetSpecularDirHA(GXLightID light, f32 nx, f32 ny, f32 nz, f32 hx, f32 hy, f32 hz) {
    f32 px;
    f32 py;
    f32 pz;
    px = 1000000000000000000.0f * -nx;
    py = 1000000000000000000.0f * -ny;
    pz = 1000000000000000000.0f * -nz;

    GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_SPEC_DIR_ID, 6);
    GDWrite_f32(px);
    GDWrite_f32(py);
    GDWrite_f32(pz);
    GDWrite_f32(hx);
    GDWrite_f32(hy);
    GDWrite_f32(hz);
}

void GDSetSpecularDir(GXLightID light, f32 nx, f32 ny, f32 nz) {
    f32 px;
    f32 py;
    f32 pz;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 mag;

    hx = -nx;
    hy = -ny;
    hz = 1.0f + -nz;

    mag = hx * hx + hy * hy + hz * hz;
    if (mag != 0.0f) {
        mag = 1.0f / sqrtf(mag);
    }
    
    hx = hx * mag;
    hy = hy * mag;
    hz = hz * mag;
    px = 1000000000000000000.0f * -nx;
    py = 1000000000000000000.0f * -ny;
    pz = 1000000000000000000.0f * -nz;

    GDWriteXFCmdHdr(__GDLightID2Offset(light) + XF_LIGHT_SPEC_DIR_ID, 6);
    GDWrite_f32(px);
    GDWrite_f32(py);
    GDWrite_f32(pz);
    GDWrite_f32(hx);
    GDWrite_f32(hy);
    GDWrite_f32(hz);
}

void GDLoadLightObjIndx(u32 lt_obj_indx, GXLightID light) {
    GDWriteXFIndxDCmd(__GDLightID2Offset(light) + XF_LIGHT_ID, 0x10, lt_obj_indx);
}

void GDSetChanAmbColor(GXChannelID chan, GXColor color) {
    GDWriteXFCmd((chan & 1) + XF_REG_AMBIENT0_ID, color.r << 24 | color.g << 16 | color.b << 8 | color.a);
}

void GDSetChanMatColor(GXChannelID chan, GXColor color) {
    GDWriteXFCmd((chan & 1) + XF_REG_MATERIAL0_ID, color.r << 24 | color.g << 16 | color.b << 8 | color.a);
}

void GDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src, GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn) {
    u32 reg;

    reg = XF_REG_CHAN_CTRL(mat_src, enable, light_mask & 0xF, amb_src, attn_fn == GX_AF_SPEC ? GX_DF_NONE : diff_fn, attn_fn != GX_AF_NONE, attn_fn != GX_AF_SPEC, (light_mask >> 4) & 0xF);
    GDWriteXFCmd((chan & 3) + XF_REG_COLOR0CNTRL_ID, reg);

    if (chan == 4 || chan == 5) {
        GDWriteXFCmd(chan + XF_REG_MATERIAL0_ID, reg);
    }
}
