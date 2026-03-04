#ifndef _DOLPHIN_GX_GXTEXTURE_H_
#define _DOLPHIN_GX_GXTEXTURE_H_

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef GXTexRegion *(*GXTexRegionCallback)(GXTexObj* t_obj, GXTexMapID id);
typedef GXTlutRegion *(*GXTlutRegionCallback)(u32 idx);

u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u8 mipmap, u8 max_lod);
void GXInitTexObj(GXTexObj* obj, void* image_ptr, u16 width, u16 height, GXTexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, u8 mipmap);
void GXInitTexObjCI(GXTexObj* obj, void* image_ptr, u16 width, u16 height, GXCITexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, u8 mipmap, u32 tlut_name);
void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt,
    f32 min_lod, f32 max_lod, f32 lod_bias, GXBool bias_clamp,
    GXBool do_edge_lod, GXAnisotropy max_aniso);
void GXInitTexObjData(GXTexObj* obj, void* image_ptr);
void GXInitTexObjWrapMode(GXTexObj* obj, GXTexWrapMode s, GXTexWrapMode t);
void GXInitTexObjTlut(GXTexObj* obj, u32 tlut_name);
void GXInitTexObjUserData(GXTexObj* obj, void* user_data);
void* GXGetTexObjUserData(const GXTexObj* obj);
void GXLoadTexObjPreLoaded(GXTexObj* obj, GXTexRegion* region, GXTexMapID id);
void GXLoadTexObj(GXTexObj* obj, GXTexMapID id);
void GXInitTlutObj(GXTlutObj* tlut_obj, void* lut, GXTlutFmt fmt, u16 n_entries);
void GXLoadTlut(GXTlutObj* tlut_obj, u32 tlut_name);
void GXInitTexCacheRegion(GXTexRegion* region, u8 is_32b_mipmap, u32 tmem_even, GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd);
void GXInitTexPreLoadRegion(GXTexRegion* region, u32 tmem_even, u32 size_even, u32 tmem_odd, u32 size_odd);
void GXInitTlutRegion(GXTlutRegion* region, u32 tmem_addr, GXTlutSize tlut_size);
void GXInvalidateTexRegion(GXTexRegion* region);
void GXInvalidateTexAll(void);
GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f);
GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f);
void GXPreLoadEntireTexture(GXTexObj* tex_obj, GXTexRegion* region);
void GXSetTexCoordScaleManually(GXTexCoordID coord, u8 enable, u16 ss, u16 ts);
void GXSetTexCoordCylWrap(GXTexCoordID coord, u8 s_enable, u8 t_enable);
void GXSetTexCoordBias(GXTexCoordID coord, u8 s_enable, u8 t_enable);
void GXInitTexObjFilter(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt);
void GXInitTexObjMaxLOD(GXTexObj* obj, f32 max_lod);
void GXInitTexObjMinLOD(GXTexObj* obj, f32 min_lod);
void GXInitTexObjLODBias(GXTexObj* obj, f32 lod_bias);
void GXInitTexObjBiasClamp(GXTexObj* obj, u8 bias_clamp);
void GXInitTexObjEdgeLOD(GXTexObj* obj, u8 do_edge_lod);
void GXInitTexObjMaxAniso(GXTexObj* obj, GXAnisotropy max_aniso);

#ifdef __cplusplus
}
#endif

#endif
