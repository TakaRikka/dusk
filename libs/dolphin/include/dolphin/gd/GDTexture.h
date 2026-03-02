#ifndef _DOLPHIN_GD_TEXTURE_H
#define _DOLPHIN_GD_TEXTURE_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BP_TEX_MODE0(wrap_s, wrap_t, mag_filt, min_filt, edge_lod, lod_bias, max_aniso, bias_clamp, id) \
    ( \
        (u32)(wrap_s) << 0 | \
        (u32)(wrap_t) << 2 | \
        (u32)(mag_filt) << 4 | \
        (u32)(min_filt) << 5 | \
        (u32)(edge_lod) << 8 | \
        (u32)(lod_bias) << 9 | \
        (u32)(max_aniso) << 19 | \
        (u32)(bias_clamp) << 21 | \
        (u32)(id) << 24 \
    )

#define BP_TEX_MODE1(min_lod, max_lod, id) \
    ( \
        (u32)(min_lod) << 0 | \
        (u32)(max_lod) << 8 | \
        (u32)(id) << 24 \
    )

#define BP_IMAGE_ATTR(width, height, format, id) \
    ( \
        (u32)(width) << 0 | \
        (u32)(height) << 10 | \
        (u32)(format) << 20 | \
        (u32)(id) << 24 \
    )

#define BP_IMAGE_PTR(ptr, id) \
    ( \
        (u32)(ptr) << 0 | \
        (u32)(id) << 24 \
    )

#define BP_TEX_CACHE_EVEN(tmem, size0, size1, type, id) \
    ( \
        (u32)(tmem) << 0 | \
        (u32)(size0) << 15 | \
        (u32)(size1) << 18 | \
        (u32)(type) << 21 | \
        (u32)(id) << 24 \
    )

#define BP_TEX_CACHE_ODD(tmem, size0, size1, id) \
    ( \
        (u32)(tmem) << 0 | \
        (u32)(size0) << 15 | \
        (u32)(size1) << 18 | \
        (u32)(id) << 24 \
    )

#define BP_TEX_TLUT(tmem, format, id) \
    ( \
        (u32)(tmem) << 0 | \
        (u32)(format) << 10 | \
        (u32)(id) << 24 \
    )

#define BP_TEXCOORD_S_SCALE(s_scale, s_bias, s_wrap, line_offset, point_offset, id) \
    ( \
        (u32)(s_scale) << 0 | \
        (u32)(s_bias) << 16 | \
        (u32)(s_wrap) << 17 | \
        (u32)(line_offset) << 18 | \
        (u32)(point_offset) << 19 | \
        (u32)(id) << 24 \
    )

#define BP_TEXCOORD_T_SCALE(t_scale, t_bias, t_wrap, id) \
    ( \
        (u32)(t_scale) << 0 | \
        (u32)(t_bias) << 16 | \
        (u32)(t_wrap) << 17 | \
        (u32)(id) << 24 \
    )

#define BP_LOAD_TLUT0(ptr, id) \
    ( \
        (u32)(ptr) << 0 | \
        (u32)(id) << 24 \
    )

#define BP_LOAD_TLUT1(ptr, size, id) \
    ( \
        (u32)(ptr) << 0 | \
        (u32)(size) << 10 | \
        (u32)(id) << 24 \
    )

void GDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrap_s,
                        GXTexWrapMode wrap_t, GXTexFilter min_filt,
                        GXTexFilter mag_filt, f32 min_lod, f32 max_lod,
                        f32 lod_bias, u8 bias_clamp, u8 do_edge_lod,
                        GXAnisotropy max_aniso);

void GDSetTexImgAttr(GXTexMapID id, u16 width, u16 height, GXTexFmt format);
void GDSetTexImgPtr(GXTexMapID id, void* image_ptr);
void GDSetTexImgPtrRaw(GXTexMapID id, u32 image_ptr_raw);
void GDPatchTexImgPtr(void* image_ptr);
void GDSetTexCached(GXTexMapID id, u32 tmem_even, GXTexCacheSize size_even,
                    u32 tmem_odd, GXTexCacheSize size_odd);
void GDSetTexPreLoaded(GXTexMapID id, u32 tmem_even, u32 tmem_odd);
void GDSetTexTlut(GXTexMapID id, u32 tmem_addr, GXTlutFmt format);
void GDSetTexCoordScale(GXTexCoordID coord, u16 s_scale, u16 t_scale);
void GDSetTexCoordScale2(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                         u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap);
void GDSetTexCoordScaleAndTOEs(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                               u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap,
                               u8 line_offset, u8 point_offset);
void GDLoadTlut(void* tlut_ptr, u32 tmem_addr, GXTlutSize size);
void GDLoadTlutRaw(u32 tlut_ptr_raw, u32 tmem_addr, GXTlutSize size);

#ifdef __cplusplus
}
#endif

#endif
