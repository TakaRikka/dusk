#ifndef D_D_KANKYO_TEV_STR
#define D_D_KANKYO_TEV_STR

#include "JSystem/J3DGraphBase/J3DTevs.h"
#include "SSystem/SComponent/c_xyz.h"
#include "dolphin/types.h"

class dKy_tevstr_c {
public:
    // Supposedly exists but didn't work. Might be too big to inline.
    // See dPa_modelEcallBack::model_c::set.
    // inline dKy_tevstr_c& operator=(const dKy_tevstr_c& b);

    /* 0x000 */ J3DLightObj mLightObj;
    /* 0x074 */ J3DLightObj mLights[6];
    /* 0x32C */ cXyz field_0x32c;  // some light pos, unsure how it differs
    /* 0x338 */ cXyz mLightPosWorld;
    /* 0x344 */ f32 field_0x344;
    /* 0x348 */ GXColorS10 AmbCol;
    /* 0x350 */ GXColorS10 FogCol;
    /* 0x358 */ GXColorS10 TevColor;
    /* 0x360 */ GXColor TevKColor;
    /* 0x364 */ GXColor mLightInf;
    /* 0x368 */ f32 mFogStartZ;
    /* 0x36C */ f32 mFogEndZ;
    /* 0x370 */ f32 pat_ratio;
    /* 0x374 */ f32 field_0x374;
    /* 0x378 */ u16 Material_id;  // Used for some sort of special material handling when non-0
    /* 0x37A */ u8 Type;
    /* 0x37B */ u8 mInitTimer;
    /* 0x37C */ u8 UseCol;
    /* 0x37D */ u8 PrevCol;
    /* 0x37E */ u8 wether_pat1;
    /* 0x37F */ u8 wether_pat0;
    /* 0x380 */ s8 room_no;  // Room Color
    /* 0x381 */ u8 YukaCol;  // Floor (Poly) Color
    /* 0x382 */ u8 mLightMode;
    /* 0x383 */ u8 Material_use_fg;
    /* 0x384 */ u8 field_0x384;
    /* 0x385 */ u8 field_0x385;
};  // Size = 0x388

STATIC_ASSERT(sizeof(dKy_tevstr_c) == 0x388);

#endif /* D_D_KANKYO_TEV_STR */
