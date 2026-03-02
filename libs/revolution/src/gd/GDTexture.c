#include <revolution/gd.h>
#include <revolution/os.h>

u8 GD2HWFiltConv[] = {0, 4, 1, 5, 2, 6};

u8 GDTexMode0Ids[8] = {128, 129, 130, 131, 160, 161, 162, 163};
u8 GDTexMode1Ids[8] = {132, 133, 134, 135, 164, 165, 166, 167};
u8 GDTexImage0Ids[8] = {136, 137, 138, 139, 168, 169, 170, 171};
u8 GDTexImage1Ids[8] = {140, 141, 142, 143, 172, 173, 174, 175};
u8 GDTexImage2Ids[8] = {144, 145, 146, 147, 176, 177, 178, 179};
u8 GDTexImage3Ids[8] = {148, 149, 150, 151, 180, 181, 182, 183};
u8 GDTexTlutIds[8] = {152, 153, 154, 155, 184, 185, 186, 187};

void GDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrap_s,
                        GXTexWrapMode wrap_t, GXTexFilter min_filt,
                        GXTexFilter mag_filt, f32 min_lod, f32 max_lod,
                        f32 lod_bias, u8 bias_clamp, u8 do_edge_lod,
                        GXAnisotropy max_aniso) {
    GDWriteBPCmd(BP_TEX_MODE0(wrap_s, wrap_t, mag_filt == TRUE, GD2HWFiltConv[min_filt], !do_edge_lod, (u8)(32.0f * lod_bias), max_aniso, bias_clamp, GDTexMode0Ids[id]));
    GDWriteBPCmd(BP_TEX_MODE1((u8)(16.0f * min_lod), (u8)(16.0f * max_lod), GDTexMode1Ids[id]));
}

void GDSetTexImgAttr(GXTexMapID id, u16 width, u16 height, GXTexFmt format) {
    GDWriteBPCmd(BP_IMAGE_ATTR(width - 1, height - 1, format, GDTexImage0Ids[id]));
}

void GDSetTexImgPtr(GXTexMapID id, void* image_ptr) {
    GDWriteBPCmd(BP_IMAGE_PTR(OSCachedToPhysical(image_ptr) >> 5, GDTexImage3Ids[id]));
}

void GDSetTexImgPtrRaw(GXTexMapID id, u32 image_ptr_raw) {
    GDWriteBPCmd(BP_IMAGE_PTR(image_ptr_raw, GDTexImage3Ids[id]));
}

void GDPatchTexImgPtr(void* image_ptr) {
    GDWrite_u24(OSCachedToPhysical(image_ptr) >> 5);
}

void GDSetTexCached(GXTexMapID id, u32 tmem_even, GXTexCacheSize size_even,
                    u32 tmem_odd, GXTexCacheSize size_odd) {
    GDWriteBPCmd(BP_TEX_CACHE_EVEN(tmem_even >> 5, size_even + 3, size_even + 3, 0, GDTexImage1Ids[id]));

    if (size_odd != 3 && tmem_odd < 0x100000) {
        GDWriteBPCmd(BP_TEX_CACHE_ODD(tmem_odd >> 5, size_odd + 3, size_odd + 3, GDTexImage2Ids[id]));
    }
}


void GDSetTexPreLoaded(GXTexMapID id, u32 tmem_even, u32 tmem_odd) {
    GDWriteBPCmd(BP_TEX_CACHE_EVEN(tmem_even >> 5, 0, 0, 1, GDTexImage1Ids[id]));

    if (tmem_odd < 0x100000) {
        GDWriteBPCmd(BP_TEX_CACHE_ODD(tmem_odd >> 5, 0, 0, GDTexImage2Ids[id]));
    }
}

void GDSetTexTlut(GXTexMapID id, u32 tmem_addr, GXTlutFmt format) {
    GDWriteBPCmd(BP_TEX_TLUT((tmem_addr - 0x80000) >> 9, format, GDTexTlutIds[id]));
}

void GDSetTexCoordScale(GXTexCoordID coord, u16 s_scale, u16 t_scale) {
    GDWriteBPCmd(0xFE00FFFF);
    GDWriteBPCmd(BP_TEXCOORD_S_SCALE(s_scale - 1, 0, 0, 0, 0, coord * 2 + 0x30));

    GDWriteBPCmd(0xFE00FFFF);
    GDWriteBPCmd(BP_TEXCOORD_T_SCALE(t_scale - 1, 0, 0, coord * 2 + 0x31));
}

void GDSetTexCoordScale2(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                         u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap) {
    GDWriteBPCmd(0xFE03FFFF);
    GDWriteBPCmd(BP_TEXCOORD_S_SCALE(s_scale - 1, s_bias, s_wrap, 0, 0, coord * 2 + 0x30));
    GDWriteBPCmd(BP_TEXCOORD_T_SCALE(t_scale - 1, t_bias, t_wrap, coord * 2 + 0x31));
}

void GDSetTexCoordScaleAndTOEs(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                               u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap,
                               u8 line_offset, u8 point_offset) {
    GDWriteBPCmd(BP_TEXCOORD_S_SCALE(s_scale - 1, s_bias, s_wrap, line_offset, point_offset, coord * 2 + 0x30));
    GDWriteBPCmd(BP_TEXCOORD_T_SCALE(t_scale - 1, t_bias, t_wrap, coord * 2 + 0x31));
}

void GDLoadTlut(void* tlut_ptr, u32 tmem_addr, GXTlutSize size) {
    ASSERTMSGLINE(488, !(tmem_addr & 0x1ff), "GDLoadTlut: invalid TMEM pointer");
    ASSERTMSGLINE(489, size <= 0x400, "GDLoadTlut: invalid TLUT size");

    GDWriteBPCmd(0xFEFFFF00);
    GDWriteBPCmd(0xF000000);
    GDWriteBPCmd(BP_LOAD_TLUT0(OSCachedToPhysical(tlut_ptr) >> 5, 0x64));
    GDWriteBPCmd(BP_LOAD_TLUT1((tmem_addr - 0x80000) >> 9, size, 0x65));
    GDWriteBPCmd(0xFEFFFF00);
    GDWriteBPCmd(0xF000000);
}

void GDLoadTlutRaw(u32 tlut_ptr_raw, u32 tmem_addr, GXTlutSize size) {
    ASSERTMSGLINE(527, size <= 0x400, "GDLoadTlut: invalid TLUT size");

    GDWriteBPCmd(0xFEFFFF00);
    GDWriteBPCmd(0xF000000);
    GDWriteBPCmd(BP_LOAD_TLUT0(tlut_ptr_raw, 0x64));
    GDWriteBPCmd(BP_LOAD_TLUT1((tmem_addr - 0x80000) >> 9, size, 0x65));
    GDWriteBPCmd(0xFEFFFF00);
    GDWriteBPCmd(0xF000000);
}
