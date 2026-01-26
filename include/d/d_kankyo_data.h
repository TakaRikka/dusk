#ifndef D_KANKYO_D_KANKYO_DATA_H
#define D_KANKYO_D_KANKYO_DATA_H

#include <dolphin/types.h>

struct color_RGB_class {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
};  // Size: 0x3

// Invented name; Used as u8 `darkLv` in dKydata_darkworldInfo_c.
enum dKyd_DARKLV {
    KY_DARKLV_FARON = 0,
    KY_DARKLV_ELDIN = 1,
    KY_DARKLV_LANAYRU = 2,
    KY_DARKLV_TEST = 5,         // Only used with nonexistent test stages
    KY_DARKLV_UNCLEARABLE = 6,  // Default. Palace of Twilight stages use this
    KY_DARKLV_ALWAYS = 8,  // Unused. Forces Twilight
};

class dKydata_darkworldInfo_c {
public:
    /* 0x0 */ const char* stageName;
    /* 0x4 */ u8 darkLv;
};  // Size: 0x8

class dKydata_lightsizeInfo_c {
public:
    /* 0x0 */ const char* stageName;
    /* 0x4 */ u8 size;
};  // Size: 0x8

// Invented name; adjust as needed in the future.
// Notes (needs more investigation):
// startTime and endTime are in the range 0 to 360, which is the same as the current time of day.
// l_time_attribute and l_time_attribute_boss are arrays of `dKyd_lightSchedjule` with their times
// set up such that the current time of day will fall in the range of one dKyd_lightSchedjule. For
// example, if the current time is 110.0f (7:20 AM), then the dKyd_lightSchedjule {105.0f, 135.0f,
// 1, 2} would be selected. If the time was 105.0f, we would use light 1. If the time was 135.0f, we
// would use light 2. Since 110.0f is closer to 105.0f than 135.0f, we get a blend of lights 1 and 2
// which is mostly 1. Blending the lights over time is how we get a smooth day-night transition.
struct dKyd_lightSchejule {
    float startTime;
    float endTime;
    u8 startTimeLight;
    u8 endTimeLight;
};  // Size: 0xC

enum BloomType {
    /* 0x0 */ BLOOM_CLEAR,
    /* 0x1 */ BLOOM_SOFT
};

struct dkydata_bloomInfo_info_class {
    /* 0x0 */ u8 mType;
    /* 0x1 */ u8 mThreshold;
    /* 0x2 */ u8 mBlurAmount;
    /* 0x3 */ u8 mDensity;
    /* 0x4 */ u8 mColorR;
    /* 0x5 */ u8 mColorG;
    /* 0x6 */ u8 mColorB;
    /* 0x7 */ u8 mOrigDensity;  // ?
    /* 0x8 */ u8 mSaturateSubtractR;
    /* 0x9 */ u8 mSaturateSubtractG;
    /* 0xA */ u8 mSaturateSubtractB;
    /* 0xB */ u8 mSaturateSubtractA; 
};

#define BLOOMINFO_MAX 64
class dKydata_BloomInfo_c {
public:
    dkydata_bloomInfo_info_class info;
};  // Size: 0xC

void* dKyd_dmpalet_getp();
void* dKyd_dmpselect_getp();
void* dKyd_dmenvr_getp();
void* dKyd_dmvrbox_getp();
dKyd_lightSchejule* dKyd_schejule_getp();
dKyd_lightSchejule* dKyd_schejule_boss_getp();
void dKyd_xfog_table_set(u8);
color_RGB_class* dKyd_maple_col_getp();
dKydata_darkworldInfo_c* dKyd_darkworld_tbl_getp();
dKydata_lightsizeInfo_c* dKyd_light_size_tbl_getp();
dKydata_lightsizeInfo_c* dKyd_light_tw_size_tbl_getp();
dKydata_BloomInfo_c* dKyd_BloomInf_tbl_getp(int);

#endif /* D_KANKYO_D_KANKYO_DATA_H */
