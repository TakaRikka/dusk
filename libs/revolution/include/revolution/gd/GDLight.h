#ifndef _REVOLUTION_GD_LIGHT_H_
#define _REVOLUTION_GD_LIGHT_H_

#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>
#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XF_LIGHT_ID          0x0600
#define XF_LIGHT_COLOR_ID    0x0603
#define XF_LIGHT_ATTN_ID     0x0604
#define XF_LIGHT_DISTATTN_ID 0x0607
#define XF_LIGHT_POS_ID      0x060A
#define XF_LIGHT_SPEC_DIR_ID 0x060A
#define XF_LIGHT_DIR_ID      0x060D

#define XF_REG_AMBIENT0_ID     0x100A
#define XF_REG_AMBIENT1_ID     0x100B
#define XF_REG_MATERIAL0_ID    0x100C
#define XF_REG_MATERIAL1_ID    0x100D
#define XF_REG_COLOR0CNTRL_ID  0x100E
#define XF_REG_COLOR1CNTRL_ID  0x100F
#define XF_REG_ALPHA0CNTRL_ID  0x1010
#define XF_REG_ALPHA1CNTRL_ID  0x1011

#define XF_REG_CHAN_CTRL(matsrc, enable, lightmask0, ambsrc, diff_fn, p5, p6, lightmask4) \
	( \
		(u32)(matsrc) << 0 | \
		(u32)(enable) << 1 | \
		(u32)(lightmask0) << 2 | \
		(u32)(ambsrc) << 6 | \
		(u32)(diff_fn) << 7 | \
		(u32)(p5) << 9 | \
		(u32)(p6) << 10 | \
		(u32)(lightmask4) << 11 \
	)

void GDSetLightAttn(GXLightID light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
void GDSetLightSpot(GXLightID light, f32 cutoff, GXSpotFn spot_func);
void GDSetLightDistAttn(GXLightID light, f32 ref_dist, f32 ref_br, GXDistAttnFn dist_func);
void GDSetLightColor(GXLightID light, GXColor color);
void GDSetLightPos(GXLightID light, f32 x, f32 y, f32 z);
void GDSetLightDir(GXLightID light, f32 nx, f32 ny, f32 nz);
void GDSetSpecularDirHA(GXLightID light, f32 nx, f32 ny, f32 nz, f32 hx, f32 hy, f32 hz);
void GDSetSpecularDir(GXLightID light, f32 nx, f32 ny, f32 nz);
void GDLoadLightObjIndx(u32 lt_obj_indx, GXLightID light);
void GDSetChanAmbColor(GXChannelID chan, GXColor color);
void GDSetChanMatColor(GXChannelID chan, GXColor color);
void GDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                   GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                   GXAttnFn attn_fn);

inline static u16 __GDLightID2Index(GXLightID id) {
	u16 idx;

	idx = 0x1F - __cntlzw(id);
	if (idx > 7) {
		idx = 0;
	}
	return idx;
}

inline static u16 __GDLightID2Offset(GXLightID id) {
	return __GDLightID2Index(id) * 16;
}

#ifdef __cplusplus
}
#endif

#endif
