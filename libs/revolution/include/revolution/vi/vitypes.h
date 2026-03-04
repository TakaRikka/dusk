#ifndef _REVOLUTION_VITYPES_H_
#define _REVOLUTION_VITYPES_H_

#include <revolution/types.h>

#define VI_TVMODE(format, interlace)  (((format) << 2) + (interlace))

#define VI_INTERLACE     0
#define VI_NON_INTERLACE 1
#define VI_PROGRESSIVE   2

#define VI_NTSC      0
#define VI_PAL       1
#define VI_MPAL      2
#define VI_DEBUG     3
#define VI_3D        3
#define VI_DEBUG_PAL 4
#define VI_EURGB60   5
#define VI_GCA       6
#define VI_EXTRA     7
#define VI_HD720     8

typedef u8 VIBool;
#define VI_FALSE ((VIBool)0)
#define VI_TRUE ((VIBool)1)
#define VI_DISABLE ((VIBool)0)
#define VI_ENABLE ((VIBool)1)

typedef enum {
    VI_TVMODE_NTSC_INT      = VI_TVMODE(VI_NTSC,        VI_INTERLACE),
    VI_TVMODE_NTSC_DS       = VI_TVMODE(VI_NTSC,        VI_NON_INTERLACE),
    VI_TVMODE_NTSC_PROG     = VI_TVMODE(VI_NTSC,        VI_PROGRESSIVE),
    VI_TVMODE_PAL_INT       = VI_TVMODE(VI_PAL,         VI_INTERLACE),
    VI_TVMODE_PAL_DS        = VI_TVMODE(VI_PAL,         VI_NON_INTERLACE),
    VI_TVMODE_EURGB60_INT   = VI_TVMODE(VI_EURGB60,     VI_INTERLACE),
    VI_TVMODE_EURGB60_DS    = VI_TVMODE(VI_EURGB60,     VI_NON_INTERLACE),
    VI_TVMODE_EURGB60_PROG  = VI_TVMODE(VI_EURGB60,     VI_PROGRESSIVE),
    VI_TVMODE_MPAL_INT      = VI_TVMODE(VI_MPAL,        VI_INTERLACE),
    VI_TVMODE_MPAL_DS       = VI_TVMODE(VI_MPAL,        VI_NON_INTERLACE),
    VI_TVMODE_MPAL_PROG     = VI_TVMODE(VI_MPAL,        VI_PROGRESSIVE),
    VI_TVMODE_DEBUG_INT     = VI_TVMODE(VI_DEBUG,       VI_INTERLACE),
    VI_TVMODE_DEBUG_PAL_INT = VI_TVMODE(VI_DEBUG_PAL,   VI_INTERLACE),
    VI_TVMODE_DEBUG_PAL_DS  = VI_TVMODE(VI_DEBUG_PAL,   VI_NON_INTERLACE),
    VI_TVMODE_NTSC_3D       = VI_TVMODE(VI_NTSC,        VI_3D),
    VI_TVMODE_GCA_INT       = VI_TVMODE(VI_GCA,         VI_INTERLACE),
    VI_TVMODE_GCA_PROG      = VI_TVMODE(VI_GCA,         VI_PROGRESSIVE),
    VI_TVMODE_PAL_PROG      = 6,
    VI_TVMODE_EXTRA_INT     = VI_TVMODE(VI_EXTRA,       VI_INTERLACE),
    VI_TVMODE_EXTRA_DS      = VI_TVMODE(VI_EXTRA,       VI_NON_INTERLACE),
    VI_TVMODE_EXTRA_PROG    = VI_TVMODE(VI_EXTRA,       VI_PROGRESSIVE),
    VI_TVMODE_HD720_PROG    = VI_TVMODE(VI_HD720,       VI_PROGRESSIVE)
} VITVMode;

typedef enum {
    VI_XFBMODE_SF = 0,
    VI_XFBMODE_DF
} VIXFBMode;

typedef enum _VITimeToDIM {
    VI_DM_DEFAULT = 0,
    VI_DM_10M,
    VI_DM_15M
} VITimeToDIM;

typedef enum _VIVideo {
    VI_VMODE_NTSC = 0,
    VI_VMODE_MPAL = 1,
    VI_VMODE_PAL = 2,
    VI_VMODE_RGB = 3
} VIVideo;

typedef enum _VIACPType {
    VI_ACP_OFF = 1,
    VI_ACP_TYPE1 = 2,
    VI_ACP_TYPE2 = 3,
    VI_ACP_TYPE3 = 4
} VIACPType;

typedef enum _VIGamma {
    VI_GM_0_1 = 1,
    VI_GM_0_2,
    VI_GM_0_3,
    VI_GM_0_4,
    VI_GM_0_5,
    VI_GM_0_6,
    VI_GM_0_7,
    VI_GM_0_8,
    VI_GM_0_9,
    VI_GM_1_0,
    VI_GM_1_1,
    VI_GM_1_2,
    VI_GM_1_3,
    VI_GM_1_4,
    VI_GM_1_5,
    VI_GM_1_6,
    VI_GM_1_7,
    VI_GM_1_8,
    VI_GM_1_9,
    VI_GM_2_0,
    VI_GM_2_1,
    VI_GM_2_2,
    VI_GM_2_3,
    VI_GM_2_4,
    VI_GM_2_5,
    VI_GM_2_6,
    VI_GM_2_7,
    VI_GM_2_8,
    VI_GM_2_9,
    VI_GM_3_0
} VIGamma;

typedef enum _VITiming {
    VI_TMG_GAME = 0,
    VI_TMG_DVD  = 1
} VITiming;

typedef enum _VIOverDrive {
	VI_ODV_L1 = 0,
    VI_ODV_L2 = 1,
    VI_ODV_L3 = 2,
    VI_ODV_L4 = 3,
    VI_ODV_L5 = 4,
    VI_ODV_L6 = 5
} VIOverDrive;

typedef void (*VIRetraceCallback)(u32 retraceCount);

typedef struct VIGammaObj {
    u16 a[6];     
    u8 yin[7];
    u16 yout[7];
} VIGammaObj;

typedef struct VIMacroVisionObj {
    u8 m[26];
} VIMacroVisionObj;

#endif
