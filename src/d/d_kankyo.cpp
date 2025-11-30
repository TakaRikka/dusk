#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_kankyo.h"
#include <dolphin.h>
#include <dolphin/gf/GFPixel.h>

#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_kytag08.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_debug_viewer.h"
#include "d/d_debug_pad.h"
#include "d/d_kankyo_rain.h"
#include "d/d_kankyo_static.h"
#include "d/d_kankyo_debug.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_kankyo.h"
#include "f_ap/f_ap_game.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"
#include "c/c_damagereaction.h"

static void GxXFog_set();

struct sub_kankyo__class : public kankyo_class {};

// setSunpos calls these functions as inlines somehow
// these need to be moved to MSL_C once an appropriate solution is found
inline float sinf(float x) {
    return sin(x);
}

inline float cosf(float x) {
    return cos(x);
}

static LightStatus lightStatusBase = {
    {-36384.5f, 29096.699f, 17422.199f},
    {377.0f, 5207.3999f, 1220.4f},
    {0xFF, 0xFF, 0xFF, 0xFF},
    0,
    1.0f,
    0.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    {0.0f, -1.0f, -1.0f},
    90.0f,
    GX_SP_OFF,
    500.0f,
    1.0f,
    GX_DA_OFF,
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    },
    320.0f,
    -320.0f,
    1000.0f,
    160000.0f,
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    },
    0.536479f,
    2.726476f,
    0.0f,
    0.5f,
};

static u16 lightMaskData[8] = {
    GX_LIGHT0, GX_LIGHT1, GX_LIGHT2, GX_LIGHT3, GX_LIGHT4, GX_LIGHT5, GX_LIGHT6, GX_LIGHT7,
};

dScnKy_env_light_c g_env_light;

Z2EnvSeMgr g_mEnvSeMgr;

#if DEBUG
dKankyo_HIO_c g_kankyoHIO;
#endif

static LightStatus lightStatusData[8];

static u16 lightMask = 0x0001;

static LightStatus* lightStatusPt = lightStatusData;

void dKy_WolfPowerup_AmbCol(GXColorS10* in_col_p) {
    JUT_ASSERT(185, in_col_p != NULL);

    in_col_p->r = 40;
    in_col_p->g = 52;
    in_col_p->b = 49;
}

int dKy_sense_pat_get() {
    int pat = 0;
    if (dKy_Outdoor_check()) {
        pat = 1;
    } else {
        pat = 5;
    }

    // Stage is Lake Hylia
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP115")) {
        pat = 9;

        // Room is Lake
        if (dComIfGp_roomControl_getStayNo() == 0 && dComIfGs_isSaveSwitch(17) &&
            !dComIfGs_isSaveSwitch(0x75))
        {
            pat = 2;
        }

        if (dComIfGs_sense_type_change_Get() > 0) {
            pat = 13;
        }
    }
    // Stage is Zora's Domain
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP113"))
    {
        if (dComIfGs_sense_type_change_Get() > 0) {
            pat = 13;
        } else {
            pat = 1;
        }
    }
    // Stage is Faron Woods Cave
    else if (!strcmp(dComIfGp_getStartStageName(), "D_SB10"))
    {
        pat = 1;
    }
    // Stage is Zora's River
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP112"))
    {
        pat = 3;
    }
    // Stage is Upper Zora's River
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP126"))
    {
        pat = 6;
    }
    // Stage is Snowpeak
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP114"))
    {
        pat = 4;
    }
    // Stage is Forest Temple or Goron Mines or Goron Stockcave or Grotto
    else if (!memcmp(dComIfGp_getStartStageName(), "D_MN05", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_MN04", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB04", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB05", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB06", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB07", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB08", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB09", 6))
    {
        pat = 5;
    }
    // Stage is Stallord arena or Argorok arena
    else if (!strcmp(dComIfGp_getStartStageName(), "D_MN10A") ||
             !strcmp(dComIfGp_getStartStageName(), "D_MN07A"))
    {
        pat = 14;
    }
    // Stage is Death Sword arena
    else if (!strcmp(dComIfGp_getStartStageName(), "D_MN10B"))
    {
        pat = 15;
    }
    // Stage is Arbiter's Grounds
    else if (!strcmp(dComIfGp_getStartStageName(), "D_MN10"))
    {
        pat = 8;
        // Room is Central Poe room
        if (dComIfGp_roomControl_getStayNo() == 2) {
            pat = 14;
            // Room is 2nd Poe room or 4th Poe room
        } else if (dComIfGp_roomControl_getStayNo() == 3 || dComIfGp_roomControl_getStayNo() == 8) {
            pat = 6;
        }
    }
    // Stage is Blizzeta arena
    else if (!strcmp(dComIfGp_getStartStageName(), "D_MN11A"))
    {
        pat = 4;
    }
    // Stage is Snowpeak Ruins or Darkhammer or Ice Puzzle cave
    else if (!strcmp(dComIfGp_getStartStageName(), "D_MN11") ||
             !strcmp(dComIfGp_getStartStageName(), "D_MN11B") ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB00", 6))
    {
        pat = 7;

        // Stage is Snowpeak Ruins and Room is Courtyard
        if (!strcmp(dComIfGp_getStartStageName(), "D_MN11") &&
            dComIfGp_roomControl_getStayNo() == 4)
        {
            pat = 4;
        }
    }
    // Stage is Ganondorf Field or Ganondorf Defeated or Castle Town
    else if (!strcmp(dComIfGp_getStartStageName(), "D_MN09B") ||
             !strcmp(dComIfGp_getStartStageName(), "D_MN09C") ||
             !strcmp(dComIfGp_getStartStageName(), "F_SP116"))
    {
        pat = 1;
    }
    // Stage is Temple of Time or City in the Sky or Hyrule Castle Throne Room or
    // Cave of Ordeals or Cloud Sea or Hyrule Castle cutscenes or Lakebed Temple or Deku Toad
    else if (!memcmp(dComIfGp_getStartStageName(), "D_MN06", 6) ||
             !strcmp(dComIfGp_getStartStageName(), "D_MN07") ||
             !strcmp(dComIfGp_getStartStageName(), "D_MN09A") ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB01", 6) ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP300") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP301") ||
             !strcmp(dComIfGp_getStartStageName(), "D_MN01") ||
             !strcmp(dComIfGp_getStartStageName(), "D_MN01B"))
    {
        pat = 8;
    }
    // Stage is Morpheel arena
    else if (!strcmp(dComIfGp_getStartStageName(), "D_MN01A"))
    {
        pat = 14;
    }
    // Stage is Palace of Twilight
    else if (!memcmp(dComIfGp_getStartStageName(), "D_MN08", 6))
    {
        pat = 9;
    }
    // Stage is Gorge Cave or Lake Hylia Cave
    else if (!memcmp(dComIfGp_getStartStageName(), "D_SB02", 6) ||
             !memcmp(dComIfGp_getStartStageName(), "D_SB03", 6))
    {
        pat = 10;
    }
    // Stage is Ordon Village Interiors or Faron Woods Interiors or
    // Kakariko Graveyard or Telma's Bar or STAR game or Death Mountain or
    // Hyrule Castle sewers
    else if (!strcmp(dComIfGp_getStartStageName(), "R_SP01") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP108") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP209") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP116") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP161") ||
             !strcmp(dComIfGp_getStartStageName(), "F_SP110") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP107"))
    {
        pat = 11;

        // Stage is Ordon Village and Room is Sera's Shop
        if (!strcmp(dComIfGp_getStartStageName(), "R_SP01") &&
            dComIfGp_roomControl_getStayNo() == 1)
        {
            pat = 12;
        }
    }
    // Stage is Hidden Village interiors or Castle Town interiors or Fishing Pond interiors
    else if (!strcmp(dComIfGp_getStartStageName(), "R_SP128") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP160") ||
             !strcmp(dComIfGp_getStartStageName(), "R_SP127"))
    {
        pat = 12;
    }
    // Stage is Kakariko Village interiors
    else if (!strcmp(dComIfGp_getStartStageName(), "R_SP109"))
    {
        pat = 12;

        // Room is Sanctuary or Inn
        if (dComIfGp_roomControl_getStayNo() == 0 || dComIfGp_roomControl_getStayNo() == 2) {
            pat = 11;
        }
    }

    return pat;
}

void dKy_WolfPowerup_BgAmbCol(GXColorS10* in_col_p) {
    int pattern = 0;
    JUT_ASSERT(374, in_col_p != NULL);

    pattern = dKy_sense_pat_get();
    #if DEBUG
    if (g_kankyoHIO.navy.twilight_sense_pat != 0) {
        pattern = g_kankyoHIO.navy.twilight_sense_pat;
    }

    if (g_kankyoHIO.navy.twilight_sense_pat_tv_display_ON) {
        dDbVw_Report(20, 100, "sense pat[%d]", pattern);
    }
    #endif

    switch (pattern) {
    case 1:
        in_col_p->r = 33;
        in_col_p->g = 48;
        in_col_p->b = 44;
        break;
    case 2:
        in_col_p->r = 33;
        in_col_p->g = 48;
        in_col_p->b = 44;
        break;
    case 3:
        in_col_p->r = 40;
        in_col_p->g = 53;
        in_col_p->b = 49;
        break;
    case 4:
        in_col_p->r = 32;
        in_col_p->g = 46;
        in_col_p->b = 40;
        break;
    case 5:
        in_col_p->r = 45;
        in_col_p->g = 64;
        in_col_p->b = 58;
        break;
    case 6:
        in_col_p->r = 33;
        in_col_p->g = 48;
        in_col_p->b = 44;
        break;
    case 7:
        in_col_p->r = 38;
        in_col_p->g = 53;
        in_col_p->b = 49;
        break;
    case 8:
        in_col_p->r = 33;
        in_col_p->g = 48;
        in_col_p->b = 44;
        break;
    case 9:
        in_col_p->r = 40;
        in_col_p->g = 59;
        in_col_p->b = 53;
        break;
    case 11:
    case 12:
        in_col_p->r = 45;
        in_col_p->g = 64;
        in_col_p->b = 58;
        break;
    case 10:
        in_col_p->r = 40;
        in_col_p->g = 60;
        in_col_p->b = 58;
        break;
    case 13:
        in_col_p->r = 33;
        in_col_p->g = 48;
        in_col_p->b = 44;
        break;
    case 14:
        in_col_p->r = 33;
        in_col_p->g = 48;
        in_col_p->b = 44;
        break;
    case 15:
        in_col_p->r = 30;
        in_col_p->g = 48;
        in_col_p->b = 59;
        break;
    }

    #if DEBUG
    if (g_kankyoHIO.navy.twilight_sense_pat_tv_display_ON) {
        dDbVw_Report(20, 120, "R[%d]", in_col_p->r);
        dDbVw_Report(20, 136, "G[%d]", in_col_p->g);
        dDbVw_Report(20, 152, "B[%d]", in_col_p->b);
    }
    #endif
}

void dKy_WolfPowerup_FogNearFar(f32* near_p, f32* far_p) {
    int pattern = 0;
    JUT_ASSERT(499, near_p != NULL);
    JUT_ASSERT(500, far_p != NULL);

    pattern = dKy_sense_pat_get();
    #if DEBUG
    if (g_kankyoHIO.navy.twilight_sense_pat != 0) {
        pattern = g_kankyoHIO.navy.twilight_sense_pat;
    }
    #endif

    switch (pattern) {
    case 1:
        *near_p = 750.0f;
        *far_p = 1750.0f;
        break;
    case 2:
        *near_p = 5000.0f;
        *far_p = 40000.0f;
        break;
    case 3:
        *near_p = 4000.0f;
        *far_p = 12000.0f;
        break;
    case 4:
        *near_p = 500.0f;
        *far_p = 2250.0f;
        break;
    case 5:
        *near_p = 1000.0f;
        *far_p = 1800.0f;
        break;
    case 6:
        *near_p = 1000.0f;
        *far_p = 2750.0f;
        break;
    case 7:
        *near_p = 750.0f;
        *far_p = 1750.0f;
        break;
    case 8:
        *near_p = 750.0f;
        *far_p = 1750.0f;
        break;
    case 9:
        *near_p = 2000.0f;
        *far_p = 3000.0f;
        break;
    case 10:
        *near_p = 300.0f;
        *far_p = 700.0f;
        break;
    case 11:
        *near_p = 750.0f;
        *far_p = 1500.0f;
        break;
    case 12:
        *near_p = 500.0f;
        *far_p = 1250.0f;
        break;
    case 13:
        *near_p = 1000.0f;
        *far_p = 80000.0f;
        break;
    case 14:
        *near_p = 1250.0f;
        *far_p = 3000.0f;
        break;
    case 15:
        *near_p = 1000.0f;
        *far_p = 4750.0f;
        break;
    }

    #if DEBUG
    if (g_kankyoHIO.navy.twilight_sense_pat_tv_display_ON) {
        dDbVw_Report(100, 120, "near[%f]", *near_p);
        dDbVw_Report(100, 136, "far [%f]", *far_p);
    }
    #endif
}

void dKy_pos2_get_angle(cXyz* pos1_p, cXyz* pos2_p, s16* pitch_p, s16* yaw_p) {
    cXyz vec;
    vec = *pos1_p - *pos2_p;

    *pitch_p = cM_atan2s(-vec.y, vec.absXZ());
    *yaw_p = cM_atan2s(vec.x, vec.z);
}

void dKy_twi_wolflight_set(int light_id) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera_p = dComIfGp_getCamera(0);
    cXyz vectle;

    if (camera_p == NULL) {
        return;
    }

    s16 angle_x;
    s16 angle_y;
    dKy_pos2_get_angle(&camera_p->lookat.center, &camera_p->lookat.eye, &angle_x, &angle_y);
    dKyr_get_vectle_calc(&camera_p->lookat.center, &camera_p->lookat.eye, &vectle);

    kankyo->field_0x0c18[light_id].mPos.x = camera_p->lookat.eye.x + vectle.x * 300.0f;
    kankyo->field_0x0c18[light_id].mPos.y = camera_p->lookat.eye.y + vectle.y * 300.0f;
    kankyo->field_0x0c18[light_id].mPos.z = camera_p->lookat.eye.z + vectle.z * 300.0f;

    int size = g_env_light.light_size;
    #if DEBUG
    if (g_kankyoHIO.navy.room_light_type != 0) {
        size = g_kankyoHIO.navy.room_light_type - 1;
    }

    if (!g_kankyoHIO.navy.camera_light_adjust_ON) {
    #endif

    switch (size) {
    case LIGHT_SIZE_S:
        kankyo->field_0x0c18[light_id].mPos.y += 1500.0f;
        break;
    case LIGHT_SIZE_M:
        kankyo->field_0x0c18[light_id].mPos.y += 500.0f;
        break;
    case LIGHT_SIZE_L:
        kankyo->field_0x0c18[light_id].mPos.y += 1000.0f;
        break;
    case LIGHT_SIZE_LL:
        kankyo->field_0x0c18[light_id].mPos.y += 1500.0f;
        break;
    default:
        kankyo->field_0x0c18[light_id].mPos.y += 500.0f;
        break;
    }

    #if DEBUG
    } else {
        kankyo->field_0x0c18[light_id].mPos.y += g_kankyoHIO.navy.camera_light_y_shift;
    }
    #endif

    angle_x += (s16)6000;
    kankyo->field_0x0c18[light_id].mAngleX = cM_sht2d(-angle_x);
    kankyo->field_0x0c18[light_id].mAngleY = cM_sht2d(-angle_y) + 90.0f;
}

void dKy_lightdir_set(f32 angle_x, f32 angle_y, Vec* out_dir_p) {
    MtxP view_mtx = j3dSys.getViewMtx();
    Mtx inv;
    Vec calc_dir;
    cXyz dir;

    f32 x_deg = angle_x / RAD_TO_DEG(1);
    f32 y_deg = angle_y / RAD_TO_DEG(1);
    dir.x = cM_fcos(x_deg) * cM_fcos(y_deg);
    dir.y = cM_fsin(x_deg);
    dir.z = cM_fcos(x_deg) * cM_fsin(y_deg);

    cMtx_inverseTranspose(view_mtx, inv);
    cMtx_multVec(inv, &dir, &calc_dir);

    out_dir_p->x = calc_dir.x;
    out_dir_p->y = calc_dir.y;
    out_dir_p->z = calc_dir.z;
}

void dKy_GXInitLightSpot(J3DLightInfo* light_p, f32 cutoff, u8 spot_type) {
    if (cutoff <= 0.0f || cutoff > 90.0f) {
        spot_type = GX_SP_OFF;
    }

    f32 var_f30;

    f32 sp8 = (cutoff * M_PI) / 180.0f;
    f32 var_f4 = cosf(sp8);
    f32 x, y, z;

    switch (spot_type) {
    case GX_SP_FLAT:
        x = var_f4 * -1000.0f;
        y = 1000.0f;
        z = 0.0f;
        break;
    case GX_SP_COS:
        x = -var_f4 / (1.0f - var_f4);
        y = 1.0f / (1.0f - var_f4);
        z = 0.0f;
        break;
    case GX_SP_COS2:
        x = 0.0f;
        y = -var_f4 / (1.0f - var_f4);
        z = 1.0f / (1.0f - var_f4);
        break;
    case GX_SP_SHARP:
        var_f30 = ((1.0f - var_f4) * (1.0f - var_f4));
        x = (var_f4 * (var_f4 - 2.0f)) / var_f30;
        y = 2.0f / var_f30;
        z = -1.0f / var_f30;
        break;
    case GX_SP_RING1:
        var_f30 = ((1.0f - var_f4) * (1.0f - var_f4));
        x = (var_f4 * -4.0f) / var_f30;
        y = ((var_f4 + 1.0f) * 4.0f) / var_f30;
        z = -4.0f / var_f30;
        break;
    case GX_SP_RING2:
        var_f30 = ((1.0f - var_f4) * (1.0f - var_f4));
        x = 1.0f - (var_f4 * 2.0f * var_f4) / var_f30;
        y = (var_f4 * 4.0f) / var_f30;
        z = -2.0f / var_f30;
        break;
    case GX_SP_OFF:
    default:
        x = 1.0f;
        y = 0.0f;
        z = 0.0f;
        break;
    }

    light_p->mCosAtten.x = x;
    light_p->mCosAtten.y = y;
    light_p->mCosAtten.z = z;
}

void dKy_GXInitLightDistAttn(J3DLightInfo* light_p, f32 param_1, f32 param_2, u8 distattn_type) {
    f32 x;
    f32 y;
    f32 z;

    if (param_1 < 0.0f) {
        distattn_type = GX_DA_OFF;
    }

    if (param_2 <= 0.0f || param_2 >= 1.0f) {
        distattn_type = GX_DA_OFF;
    }

    switch (distattn_type) {
    case GX_DA_GENTLE:
        x = 1.0f;
        y = (1.0f - param_2) / (param_2 * param_1);
        z = 0.0f;
        break;
    case GX_DA_MEDIUM:
        x = 1.0f;
        y = ((1.0f - param_2) * 0.5f) / (param_2 * param_1);
        z = ((1.0f - param_2) * 0.5f) / (param_2 * param_1 * param_1);
        break;
    case GX_DA_STEEP:
        x = 1.0f;
        y = 0.0f;
        z = (1.0f - param_2) / (param_1 * (param_2 * param_1));
        break;
    default:
    case GX_DA_OFF:
        x = 1.0f;
        y = 0.0f;
        z = 0.0f;
        break;
    }

    light_p->mDistAtten.x = x;
    light_p->mDistAtten.y = y;
    light_p->mDistAtten.z = z;
}

static s16 u8_data_ratio_set(u8 start, u8 end, f32 ratio) {
    return start + (int)(ratio * ((s16)end - (s16)start));
}

static s16 s16_data_ratio_set(s16 start, s16 end, f32 ratio) {
    return start + (s16)(ratio * (end - start));
}

static u8 kankyo_color_ratio_calc_common(s16 color, f32 ratio) {
    s16 calc = color * ratio;

    if (calc < 0) {
        calc = 0;
    } else if (calc > 255) {
        calc = 255;
    }

    return calc;
}

static void kankyo_color_ratio_calc(GXColor* out_col_p, GXColorS10 color, f32 ratio) {
    GXColorS10 work_color;
    work_color = color;

    out_col_p->r = kankyo_color_ratio_calc_common(work_color.r, ratio);
    out_col_p->g = kankyo_color_ratio_calc_common(work_color.g, ratio);
    out_col_p->b = kankyo_color_ratio_calc_common(work_color.b, ratio);
}

static s16 kankyo_color_ratio_set(u8 color_a_start, u8 color_a_end, f32 color_ratio,
                                  u8 color_b_start, u8 color_b_end, f32 blend_ratio, s16 add_color,
                                  f32 scale) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();

    s16 a = s16_data_ratio_set(color_a_start, color_a_end, color_ratio);
    s16 b = s16_data_ratio_set(color_b_start, color_b_end, color_ratio);
    s16 blend_color = s16_data_ratio_set(a, b, blend_ratio);
    blend_color += add_color;

    s16 calc_color = kankyo->now_allcol_ratio * scale * blend_color;

    if (calc_color < 0) {
        calc_color = 0;
    }

    if (calc_color > 255) {
        calc_color = 255;
    }

    return calc_color;
}

static f32 fl_data_ratio_set(f32 start, f32 end, f32 ratio) {
    return start + ratio * (end - start);
}

static f32 float_kankyo_color_ratio_set(f32 color_a_start, f32 color_a_end, f32 color_ratio,
                                        f32 color_b_start, f32 color_b_end, f32 blend_ratio,
                                        f32 param_6, f32 param_7) {
    f32 a = fl_data_ratio_set(color_a_start, color_a_end, color_ratio);
    f32 b = fl_data_ratio_set(color_b_start, color_b_end, color_ratio);
    f32 calc_color = fl_data_ratio_set(a, b, blend_ratio);
    f32 var_f28 = param_7 * (param_6 - calc_color);
    calc_color += var_f28;

    return calc_color;
}

static f32 get_parcent(f32 max, f32 min, f32 value) {
    f32 range = max - min;

    if (0.0f != range) {
        f32 diff = max - value;
        f32 parcent = 1.0f - (diff / range);
        if (!(parcent >= 1.0f)) {
            return parcent;
        }
    }

    return 1.0f;
}

f32 dKy_get_parcent(f32 max, f32 min, f32 value) {
    return get_parcent(max, min, value);
}

static void dKy_FiveSenses_fullthrottle_dark_static1() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    BOOL init_mode_change = FALSE;

    cXyz particle_pos;
    cXyz particle_size;

    particle_pos.x = 0.0f;
    particle_pos.y = 0.0f;
    particle_pos.z = 0.0f;

    particle_size.x = 1.0f;
    particle_size.y = 1.0f;
    particle_size.z = 1.0f;

    #if !PLATFORM_GCN
    particle_size.x *= mDoGph_gInf_c::getScale();
    #endif

    if (daPy_py_c::checkNowWolfPowerUp()) {
        kankyo->now_senses_effect = 1;
        init_mode_change = TRUE;

        if (kankyo->senses_effect_strength <= 0.0f) {
            dComIfGp_particle_set(0x1E3, &particle_pos, NULL, NULL, &particle_size);
        }

        cLib_addCalc(&kankyo->senses_effect_strength, 1.0f, 0.5f, 0.1f, 0.01f);
    } else if (kankyo->senses_effect_strength > 0.0f) {
        if (kankyo->senses_effect_strength >= 1.0f) {
            dComIfGp_particle_set(0x46A, &particle_pos, NULL, NULL, &particle_size);
        }

        cLib_addCalc(&kankyo->senses_effect_strength, 0.0f, 0.5f, 0.1f, 0.01f);
        init_mode_change = TRUE;
    }

    switch (kankyo->senses_mode) {
    case 0:
        if (init_mode_change != TRUE) {
            break;
        }

        kankyo->senses_ef_emitter0 = NULL;
        kankyo->senses_ef_emitter1 = NULL;
        kankyo->senses_ef_emitter2 = NULL;

        kankyo->senses_ef_emitter0 = dComIfGp_particle_set(0x1F2, &particle_pos, NULL, NULL);
        kankyo->senses_ef_emitter1 = dComIfGp_particle_set(0x1F3, &particle_pos, NULL, NULL, &particle_size);
        kankyo->senses_ef_emitter2 = dComIfGp_particle_set(0x1F4, &particle_pos, NULL, NULL, &particle_size);
        kankyo->senses_mode++;
    case 1:
        if (init_mode_change == TRUE) {
            if (kankyo->senses_ef_emitter0 != NULL) {
                kankyo->senses_ef_emitter0->setGlobalTranslation(particle_pos.x, particle_pos.y,
                                                                 particle_pos.z);
                kankyo->senses_ef_emitter0->setGlobalAlpha(kankyo->senses_effect_strength * 255.0f);
            }

            if (kankyo->senses_ef_emitter1 != NULL) {
                kankyo->senses_ef_emitter1->setGlobalTranslation(particle_pos.x, particle_pos.y,
                                                                 particle_pos.z);
                kankyo->senses_ef_emitter1->setGlobalAlpha(kankyo->senses_effect_strength * 255.0f);
            }

            if (kankyo->senses_ef_emitter2 != NULL) {
                kankyo->senses_ef_emitter2->setGlobalTranslation(particle_pos.x, particle_pos.y,
                                                                 particle_pos.z);

                kankyo->senses_ef_emitter2->setGlobalEnvColor(
                    255.0f * (1.0f - kankyo->senses_effect_strength),
                    255.0f * (1.0f - kankyo->senses_effect_strength),
                    255.0f * (1.0f - kankyo->senses_effect_strength));
            }
        } else {
            kankyo->senses_mode++;
        }
        break;
    case 2:
        if (kankyo->senses_ef_emitter0 != NULL) {
            kankyo->senses_ef_emitter0->deleteAllParticle();
            kankyo->senses_ef_emitter0->becomeInvalidEmitter();
            kankyo->senses_ef_emitter0->quitImmortalEmitter();
            kankyo->senses_ef_emitter0->setEmitterCallBackPtr(NULL);
            kankyo->senses_ef_emitter0 = NULL;
        }

        if (kankyo->senses_ef_emitter1 != NULL) {
            kankyo->senses_ef_emitter1->deleteAllParticle();
            kankyo->senses_ef_emitter1->becomeInvalidEmitter();
            kankyo->senses_ef_emitter1->quitImmortalEmitter();
            kankyo->senses_ef_emitter1->setEmitterCallBackPtr(NULL);
            kankyo->senses_ef_emitter1 = NULL;
        }

        if (kankyo->senses_ef_emitter2 != NULL) {
            kankyo->senses_ef_emitter2->deleteAllParticle();
            kankyo->senses_ef_emitter2->becomeInvalidEmitter();
            kankyo->senses_ef_emitter2->quitImmortalEmitter();
            kankyo->senses_ef_emitter2->setEmitterCallBackPtr(NULL);
            kankyo->senses_ef_emitter2 = NULL;
        }

        kankyo->senses_mode = 0;
        break;
    }
}

void dKy_FiveSenses_fullthrottle_dark() {
    dKy_FiveSenses_fullthrottle_dark_static1();
}

int dKy_light_influence_id(cXyz position, int param_1) {
    f32 closest_plight_dist = 1000000.0f;

    int closest_plight_no = -1;
    int var_r27 = -1;
    int spC;
    int sp8 = 0;
    int j;
    int var_r25 = -1;
    int i;

    f32 var_f30 = 800.0f;
    if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
        var_f30 = 250.0f;
    }

    for (i = 0; i <= param_1; i++) {
        for (j = 0; j < 100; j++) {
            if (g_env_light.pointlight[j] != NULL && (i == 0 || j != closest_plight_no) &&
                g_env_light.pointlight[j]->mPow > 0.01f)
            {
                if (closest_plight_dist > position.abs(g_env_light.pointlight[j]->mPosition)) {
                    if (g_env_light.pointlight[j]->mIndex & 0x8000) {
                        if (closest_plight_dist > var_f30) {
                            if (i == 0) {
                                closest_plight_no = j;
                            } else {
                                var_r27 = j;
                            }

                            closest_plight_dist = var_f30;
                        }
                    } else {
                        closest_plight_dist = position.abs(g_env_light.pointlight[j]->mPosition);
                        if (closest_plight_dist < g_env_light.pointlight[j]->mPow) {
                            #if PLATFORM_GCN
                            if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0 &&
                                dComIfGp_roomControl_getStayNo() == 0)
                            {
                                if (closest_plight_dist < g_env_light.pointlight[j]->mPow * 0.5f) {
                                    var_r25 = 99;
                                }
                            } else {
                                var_r25 = 99;
                            }
                            #else
                            var_r25 = 99;
                            #endif
                        }

                        if (var_r25 != -2) {
                            if (i == 0) {
                                closest_plight_no = j;
                            } else {
                                var_r27 = j;
                            }
                        }
                    }
                }

                if (g_env_light.pointlight[j]->mIndex < 0 && var_r25 != 99) {
                    if (i == 0) {
                        closest_plight_no = j;
                    }

                    var_r25 = -2;
                }
            }
        }
        closest_plight_dist = 1000000.0f;
    }

    
    if (param_1 == 0) {
        spC = closest_plight_no;
    } else {
        spC = var_r27;
    }

    return spC;
}

int dKy_eflight_influence_id(cXyz position, int param_1) {
    f32 var_f31 = 1000000.0f;

    int var_r28 = -1;
    int var_r27 = -1;
    int var_r26;
    int sp8 = 0;
    int j, i;

    for (i = 0; i <= param_1; i++) {
        for (j = 0; j < 5; j++) {
            if (g_env_light.efplight[j] != NULL && (i == 0 || j != var_r28)) {
                if (var_f31 > position.abs(g_env_light.efplight[j]->mPosition) &&
                    g_env_light.efplight[j]->mPow > 0.01f)
                {
                    var_f31 = position.abs(g_env_light.efplight[j]->mPosition);

                    if (i == 0) {
                        var_r28 = j;
                    } else {
                        var_r27 = j;
                    }
                }
            }
        }
        var_f31 = 1000000.0f;
    }

    if (param_1 == 0) {
        var_r26 = var_r28;
    } else {
        var_r26 = var_r27;
    }

    return var_r26;
}

GXColorS10 dKy_light_influence_col(int light_id) {
    return g_env_light.pointlight[light_id >= 0 ? light_id : 0]->mColor;
}

GXColor dKy_light_influence_col(GXColor* in_col_p, f32 ratio) {
    GXColor out_col;

    f32 color = in_col_p->r * ratio;
    if (color <= 255.0f) {
        out_col.r = color;
    } else {
        out_col.r = 255;
    }

    color = in_col_p->g * ratio;
    if (color <= 255.0f) {
        out_col.g = color;
    } else {
        out_col.g = 255;
    }

    color = in_col_p->b * ratio;
    if (color <= 255.0f) {
        out_col.b = color;
    } else {
        out_col.b = 255;
    }

    return out_col;
}

f32 dKy_light_influence_power(int light_id) {
    return g_env_light.pointlight[light_id >= 0 ? light_id : 0]->mPow;
}

f32 dKy_light_influence_yuragi(int light_id) {
    return g_env_light.pointlight[light_id >= 0 ? light_id : 0]->mFluctuation;
}

f32 dKy_light_influence_distance(cXyz position, int light_id) {
    return position.abs(g_env_light.pointlight[light_id >= 0 ? light_id : 0]->mPosition);
}

static void plight_init() {
    g_env_light.mLightInfluence[0].mPow = 99999.9f;

    for (int i = 0; i < 100; i++) {
        g_env_light.pointlight[i] = NULL;
    }

    for (int i = 0; i < 5; i++) {
        g_env_light.efplight[i] = NULL;
        g_env_light.unk_0x72c[i] = NULL;
    }

    g_env_light.mPlayerPLightIdx = -1;
    g_env_light.mPlayerEfLightIdx = -1;
}

static void darkmist_init() {
    for (int i = 0; i < 10; i++) {
        g_env_light.dalkmist_influence[i] = NULL;
    }
}

static void plight_set() {
    int plight_no = 0;

    stage_plight_info_class* plight_info = dComIfGp_getStagePlightInfo();
    if (plight_info != NULL) {
        g_env_light.stage_plight_info = plight_info;

        for (int i = 0; i < dComIfGp_getStagePlightNumInfo(); i++) {
            if (plight_no < 30) {
                g_env_light.mLightInfluence[plight_no].mPosition.x = plight_info[i].position.x;
                g_env_light.mLightInfluence[plight_no].mPosition.y = plight_info[i].position.y;
                g_env_light.mLightInfluence[plight_no].mPosition.z = plight_info[i].position.z;
                g_env_light.mLightInfluence[plight_no].mColor.r = plight_info[i].color.r;
                g_env_light.mLightInfluence[plight_no].mColor.g = plight_info[i].color.g;
                g_env_light.mLightInfluence[plight_no].mColor.b = plight_info[i].color.b;
                g_env_light.mLightInfluence[plight_no].mPow = plight_info[i].power * 200.0f;
                g_env_light.mLightInfluence[plight_no].mFluctuation = plight_info[i].fluctuation;
                dKy_plight_set(&g_env_light.mLightInfluence[plight_no]);
                plight_no++;
            } else {
#ifdef DEBUG
                // "\nToo many Point Lights set!!!"
                OSReport_Warning("\nポイントライトマップ配置が多すぎます！！！");
#endif
            }
        }
    }
}

static void bgparts_activelight_init() {
    int i;
    for (i = 0; i < 2; i++) {
        g_env_light.bgparts_active_light[0].mIndex = 0;
    }
}

#if DEBUG
static void hostio_init() {
    g_kankyoHIO.time_change = 0;
    g_kankyoHIO.navy.field_0x6 = 0;
    g_kankyoHIO.light.field_0x5 = 0;
    g_kankyoHIO.light.field_0x8 = 320;
    g_kankyoHIO.light.m_fogtype = 0;
    g_kankyoHIO.wether_effect = 0;
    g_kankyoHIO.wether_palette = 0;
    g_kankyoHIO.display_env_tag_debug = 0;
    g_kankyoHIO.display_wether_debug = 0;
    g_kankyoHIO.field_0x9 = 0;
    g_kankyoHIO.field_0xa = 1;
    g_kankyoHIO.display_schedule_bit = 0;
    g_kankyoHIO.field_0xc = 1.0f;
    g_kankyoHIO.field_0x10 = 0;
    g_kankyoHIO.start_rain = 0;
    g_kankyoHIO.effect_light_monitor = 0;
    g_kankyoHIO.point_light_monitor = 0;
    g_kankyoHIO.field_0x14 = 0;
    g_kankyoHIO.field_0x15 = 0;
    g_kankyoHIO.field_0x16 = 0;
    g_kankyoHIO.field_0x17 = 1;
    g_kankyoHIO.no_setting_tevstr = 0;
    g_kankyoHIO.no_color_type = 0;
}
#endif

static void dungeonlight_init() {
    static Vec test_pos_tbl[] = {
        {0.0f, -99999.0f, 0.0f}, {0.0f, -99999.0f, 0.0f}, {0.0f, -99999.0f, 0.0f},
        {0.0f, -99999.0f, 0.0f}, {0.0f, -99999.0f, 0.0f}, {0.0f, -99999.0f, 0.0f},
        {0.0f, -99999.0f, 0.0f}, {0.0f, -99999.0f, 0.0f},
    };
    static color_RGB_class test_color_tbl[] = {
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    };

    dScnKy_env_light_c* kankyo = dKy_getEnvlight();

    for (int i = 0; i < 8; i++) {
        kankyo->dungeonlight[i].mPosition = cXyz(test_pos_tbl[i]);
        kankyo->dungeonlight[i].mRefDistance = 1.0f;
        kankyo->dungeonlight[i].field_0x14 = 0.99f;
        kankyo->dungeonlight[i].mColor.r = test_color_tbl[i].r;
        kankyo->dungeonlight[i].mColor.g = test_color_tbl[i].g;
        kankyo->dungeonlight[i].mColor.b = test_color_tbl[i].b;
        kankyo->dungeonlight[i].mColor.a = 255;

        kankyo->dungeonlight[i].mInfluence.mPosition = kankyo->dungeonlight[i].mPosition;
        kankyo->dungeonlight[i].mInfluence.mColor.r = kankyo->dungeonlight[i].mColor.r;
        kankyo->dungeonlight[i].mInfluence.mColor.g = kankyo->dungeonlight[i].mColor.g;
        kankyo->dungeonlight[i].mInfluence.mColor.b = kankyo->dungeonlight[i].mColor.b;
        kankyo->dungeonlight[i].mInfluence.mPow = kankyo->dungeonlight[i].mRefDistance * 100.0f;
        kankyo->dungeonlight[i].mInfluence.mFluctuation = 0.0f;

        kankyo->dungeonlight[i].mCutoffAngle = 90.0f;
        kankyo->dungeonlight[i].mAngleX = -90.0f;
        kankyo->dungeonlight[i].mAngleY = 0.0f;
        kankyo->dungeonlight[i].mAngleAttenuation = GX_SP_OFF;
        kankyo->dungeonlight[i].mDistAttenuation = GX_DA_MEDIUM;

        kankyo->field_0x0c18[i].mPos = cXyz(test_pos_tbl[i]);
        kankyo->field_0x0c18[i].mColor.r = test_color_tbl[i].r;
        kankyo->field_0x0c18[i].mColor.g = test_color_tbl[i].g;
        kankyo->field_0x0c18[i].mColor.b = test_color_tbl[i].b;
        kankyo->field_0x0c18[i].mColor.a = 255;
        kankyo->field_0x0c18[i].mRefDistance = 0.001f;
        kankyo->field_0x0c18[i].field_0x14 = 0.99f;
        kankyo->field_0x0c18[i].field_0x26 = 0;
        kankyo->field_0x0c18[i].mCutoffAngle = 90.0f;
        kankyo->field_0x0c18[i].mAngleX = -90.0f;
        kankyo->field_0x0c18[i].mAngleY = 0.0f;
        kankyo->field_0x0c18[i].mAngleAttenuation = GX_SP_OFF;
        kankyo->field_0x0c18[i].mDistAttenuation = GX_DA_MEDIUM;

        if (i < 6) {
            kankyo->field_0x0d58[i] = kankyo->field_0x0c18[i];
        }
    }
}

static void undwater_init() {
    J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes("Always", 0x1D);
    JUT_ASSERT(1867, modelData2 != NULL);

    g_env_light.undwater_ef_heap = mDoExt_createSolidHeapFromGameToCurrent(0x600, 0x20);

    if (g_env_light.undwater_ef_heap != NULL) {
        g_env_light.undwater_ef_model = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11020202);

        if (g_env_light.undwater_ef_model != NULL) {
            g_env_light.undwater_btk = new mDoExt_btkAnm();

            if (g_env_light.undwater_btk != NULL) {
                if (!g_env_light.undwater_btk->init(modelData2, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Always", 0x3C), TRUE,
                                                    J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1))
                {
                    JUT_ASSERT(1898, FALSE);
                }
            } else {
                JUT_ASSERT(1902, FALSE);
            }
        }
        mDoExt_restoreCurrentHeap();
        u32 var_r28 = mDoExt_adjustSolidHeap(g_env_light.undwater_ef_heap);
    } else {
        JUT_ASSERT(1917, FALSE);
    }
}

void dKy_light_size_get(char const* stageName) {
    dKydata_lightsizeInfo_c* size_tbl = dKyd_light_size_tbl_getp();
    dKydata_lightsizeInfo_c* tw_size_tbl = dKyd_light_tw_size_tbl_getp();

    if (!dKy_darkworld_check()) {
        for (int i = 0; i < 36; i++) {
            if (!strcmp(stageName, size_tbl->stageName)) {
                g_env_light.light_size = size_tbl->size;
                break;
            }
            size_tbl++;
        }
    } else {
        for (int i = 0; i < 9; i++) {
            if (!strcmp(stageName, tw_size_tbl->stageName)) {
                g_env_light.light_size = tw_size_tbl->size;
                return;
            }
            tw_size_tbl++;
        }
    }
}

static void envcolor_init() {
    stage_palette_info_class* stage_palette_p = dComIfGp_getStagePaletteInfo();
    stage_pselect_info_class* stage_psel_p = dComIfGp_getStagePselectInfo();
    stage_envr_info_class* stage_envr_p = dComIfGp_getStageEnvrInfo();
    stage_vrboxcol_info_class* stage_vrboxcol_p = dComIfGp_getStageVrboxcolInfo();

    dKy_actor_addcol_set(0, 0, 0, 0.0f);
    dKy_fog_startendz_set(0.0f, 0.0f, 0.0f);
    dKy_vrbox_addcol_set(0, 0, 0, 0.0f);

    g_env_light.field_0x1280 = 0;
    g_env_light.light_init_timer = 1;
    g_env_light.schbit = 0;
    g_env_light.schbit_timer = 0;

    g_env_light.hide_vrbox = false;
    g_env_light.mContrastFlag = 0;
    g_env_light.mFogAdjEnable = true;
    g_env_light.mFogAdjTableType = 0;
    g_env_light.mFogAdjCenter = 0x140;
    dKyd_xfog_table_set(g_env_light.mFogAdjTableType);

    g_env_light.now_allcol_ratio = 1.0f;
    g_env_light.now_actcol_ratio = 1.0f;
    g_env_light.now_bgcol_ratio = 1.0f;
    g_env_light.now_fogcol_ratio = 1.0f;
    g_env_light.now_vrboxsoracol_ratio = 1.0f;
    g_env_light.now_vrboxkumocol_ratio = 1.0f;
    g_env_light.field_0x1210 = 1.0f;

    g_env_light.allcol_ratio = 1.0f;
    g_env_light.actcol_ratio = 1.0f;
    g_env_light.bgcol_ratio = 1.0f;
    g_env_light.fogcol_ratio = 1.0f;
    g_env_light.vrboxsoracol_ratio = 1.0f;
    g_env_light.vrboxkumocol_ratio = 1.0f;
    g_env_light.field_0x122c = 1.0f;

    g_env_light.Itemgetcol_chg = 0;
    g_env_light.field_0x1240 = 0.0f;
    g_env_light.field_0x1230 = 0.0f;
    g_env_light.field_0x1234 = 0.0f;

    g_env_light.mThunderEff.mState = 0;
    g_env_light.field_0x1238 = 0.0f;
    g_env_light.field_0x123c = 0.0f;
    g_env_light.field_0x12d6 = 0;
    g_env_light.shadow_mode = 0;
    g_env_light.camera_water_in_status = 0;

    g_env_light.field_0x1258 = 0.0f;
    g_env_light.mActorLightEffect = 100;
    g_env_light.bg_light_influence = 1.0f;
    g_env_light.field_0x1064.x = 0.0f;
    g_env_light.field_0x1064.y = -1.0f;
    g_env_light.field_0x1064.z = -1.0f;
    g_env_light.mDemoAttentionPoint = 0.0f;

    if (!strcmp(dComIfGp_getStartStageName(), "R_SP127")) {
        g_env_light.mDemoAttentionPoint = 0.11f;
    }

    #if DEBUG
    if (g_env_light.light_mask_type & 0xF0) {
        g_env_light.light_mask_type = g_env_light.light_mask_type & 0xF;
    } else {
        g_env_light.light_mask_type = 0;
    }
    #else
    g_env_light.light_mask_type = 0;
    #endif

    g_env_light.field_0x130b = 0;
    g_env_light.light_size = LIGHT_SIZE_M;
    dKy_light_size_get(dComIfGp_getStartStageName());
    g_env_light.grass_light_inf_rate = 1.0f;
    g_env_light.field_0x12fc = -1;
    g_env_light.field_0x1278 = 0.0f;
    g_env_light.field_0x1264 = -255.0f;
    g_env_light.field_0x1268 = 999999.9f;
    g_env_light.field_0x126c = 999999.9f;
    g_env_light.field_0x127c = 200.0f;

    stage_stag_info_class* stageinfo = dComIfGp_getStage()->getStagInfo();
    if (dStage_stagInfo_GetSTType(stageinfo) == ST_BOSS_ROOM) {
        g_env_light.light_schedule = dKyd_schejule_boss_getp();
    } else {
        g_env_light.light_schedule = dKyd_schejule_getp();
    }

    g_env_light.PrevCol = dComIfGp_roomControl_getStayNo();
    g_env_light.UseCol = dComIfGp_roomControl_getStayNo();
    g_env_light.pat_ratio = 1.0f;
    g_env_light.field_0x12b0 = 0;
    g_env_light.mColpatWeather = 0;
    g_env_light.mVrkumoStrength = 0.0f;

    if (!strcmp(dComIfGp_getStartStageName(), "F_SP127") ||
        !strcmp(dComIfGp_getStartStageName(), "R_SP127"))
    {
        if (g_env_light.wether >= 7) {
            g_env_light.mColpatWeather = 2;
        } else if (g_env_light.wether != 0) {
            g_env_light.mColpatWeather = 1;
        }
    } else if (!strcmp(dComIfGp_getStartStageName(), "D_MN07A")) {
        if (g_env_light.wether == 1) {
            g_env_light.mColpatWeather = 1;
        } else if (g_env_light.wether == 2) {
            g_env_light.mColpatWeather = 2;
        }
    } else {
        g_env_light.wether = 0;
    }

    g_env_light.base_raincnt = 0;
    g_env_light.field_0x12a0 = 0;
    g_env_light.dice_wether_mode = 0;
    g_env_light.dice_wether_change_time = dComIfGs_getTime() + (cM_rndF(g_Counter.mCounter0 & 31) * 15.0f);
    if (g_env_light.dice_wether_change_time >= 360.0f) {
        g_env_light.dice_wether_change_time -= 360.0f;
    }

    g_env_light.dice_wether_state = 0;
    g_env_light.dice_wether_pat = 0;
    g_env_light.dice_wether_counter = 0;
    g_env_light.dice_wether_time = 0.0f;

    g_env_light.wether_pat0 = g_env_light.mColpatWeather;
    g_env_light.wether_pat1 = g_env_light.mColpatWeather;
    g_env_light.mColpatPrevGather = 0xFF;
    g_env_light.mColpatCurrGather = 0xFF;
    g_env_light.mColPatBlendGather = -1.0f;
    g_env_light.mColPatMode = 0;
    g_env_light.mColPatModeGather = 0;

    if (stage_envr_p != NULL) {
        g_env_light.stage_envr_info = stage_envr_p;
    } else {
        g_env_light.stage_envr_info = (stage_envr_info_class*)dKyd_dmenvr_getp();
    }

    if (stage_palette_p != NULL) {
        g_env_light.stage_palette_info = stage_palette_p;
    } else {
        g_env_light.stage_palette_info = (stage_palette_info_class*)dKyd_dmpalet_getp();
    }

    if (stage_psel_p != NULL) {
        g_env_light.stage_pselect_info = stage_psel_p;
    } else {
        g_env_light.stage_pselect_info = (stage_pselect_info_class*)dKyd_dmpselect_getp();
    }

    if (stage_vrboxcol_p != NULL) {
        g_env_light.stage_vrboxcol_info = stage_vrboxcol_p;
    } else {
        g_env_light.stage_vrboxcol_info = (stage_vrboxcol_info_class*)dKyd_dmvrbox_getp();
    }

    plight_init();
    plight_set();
    darkmist_init();
    bgparts_activelight_init();

    g_env_light.unk_0x11c8 = 1.0f;
    g_env_light.unk_0x11cc = 1.0f;
    g_env_light.unk_0x11d0 = 1.0f;
    g_env_light.field_0xf2c = 0;
    g_env_light.undwater_ef_model = NULL;
    g_env_light.field_0xf34 = 0.0f;
    g_env_light.undwater_ef_heap = NULL;

    undwater_init();
    dComIfGs_PolyDamageOff_Set(0);

    g_env_light.player_room_no = dComIfGp_roomControl_getStayNo();
    g_env_light.is_blure = 0;
    g_env_light.using_time_control_tag = 0;
    g_env_light.field_0x1300 = -1;
    g_env_light.field_0x1302 = -1;
    g_env_light.mWaterSurfaceShineRate = 1.0f;

    #if DEBUG
    g_kankyoHIO.navy.camera_light_col.r = 25;
    g_kankyoHIO.navy.camera_light_col.g = 90;
    g_kankyoHIO.navy.camera_light_col.b = 183;
    g_kankyoHIO.navy.field_0x2ae = 0xFF;
    g_kankyoHIO.navy.camera_light_y_shift = 1500.0f;
    g_kankyoHIO.navy.camera_light_power = 1.25f;
    g_kankyoHIO.navy.camera_light_cutoff = 90.0f;
    g_kankyoHIO.navy.camera_light_sp = 2;
    g_kankyoHIO.navy.camera_light_da = 3;
    g_kankyoHIO.navy.room_light_type = 0;
    g_kankyoHIO.navy.field_0x311 = 0;
    g_kankyoHIO.navy.field_0x312 = 0;
    g_kankyoHIO.navy.field_0x314 = 0.0f;
    g_kankyoHIO.navy.terrain_height_crr = 200.0f;
    g_kankyoHIO.navy.influence_multiplier = 1.0f;
    g_kankyoHIO.navy.cutoff_multiplier = 1.0f;

    if (dKy_darkworld_check()) {
        g_kankyoHIO.navy.cloud_sunny_wind_influence_rate = 80.0f;
        g_kankyoHIO.navy.cloud_sunny_bottom_height = 0.0f;
        g_kankyoHIO.navy.cloud_sunny_top_height = 0.0f;
        g_kankyoHIO.navy.cloud_sunny_size = 0.8f;
        g_kankyoHIO.navy.cloud_sunny_height_shrink_rate = 0.98f;
        g_kankyoHIO.navy.cloud_sunny_alpha = 0.75f;

        g_kankyoHIO.navy.cloud_cloudy_wind_influence_rate = g_kankyoHIO.navy.cloud_sunny_wind_influence_rate;
        g_kankyoHIO.navy.cloud_cloudy_bottom_height = g_kankyoHIO.navy.cloud_sunny_bottom_height;
        g_kankyoHIO.navy.cloud_cloudy_top_height = g_kankyoHIO.navy.cloud_sunny_top_height;
        g_kankyoHIO.navy.cloud_cloudy_size = g_kankyoHIO.navy.cloud_sunny_size;
        g_kankyoHIO.navy.cloud_cloudy_height_shrink_rate = g_kankyoHIO.navy.cloud_sunny_height_shrink_rate;
        g_kankyoHIO.navy.cloud_cloudy_alpha = g_kankyoHIO.navy.cloud_sunny_alpha;
    } else {
        g_kankyoHIO.navy.cloud_sunny_wind_influence_rate = 10.0f;
        g_kankyoHIO.navy.cloud_sunny_bottom_height = 2500.0f;
        g_kankyoHIO.navy.cloud_sunny_top_height = 2500.0f;
        g_kankyoHIO.navy.cloud_sunny_size = 0.6f;
        g_kankyoHIO.navy.cloud_sunny_height_shrink_rate = 0.9999f;
        g_kankyoHIO.navy.cloud_sunny_alpha = 1.0f;

        g_kankyoHIO.navy.cloud_cloudy_wind_influence_rate = 25.0f;
        g_kankyoHIO.navy.cloud_cloudy_bottom_height = 1200.0f;
        g_kankyoHIO.navy.cloud_cloudy_top_height = 1200.0f;
        g_kankyoHIO.navy.cloud_cloudy_size = 0.84f;
        g_kankyoHIO.navy.cloud_cloudy_height_shrink_rate = 0.96f;
        g_kankyoHIO.navy.cloud_cloudy_alpha = 1.0f;

        g_kankyoHIO.navy.cloud_cloudy_bottom_height = 1200.0f;
        g_kankyoHIO.navy.cloud_cloudy_top_height = 1200.0f;
        g_kankyoHIO.navy.cloud_cloudy_size = 0.84f;
        g_kankyoHIO.navy.cloud_cloudy_height_shrink_rate = 0.96f;
        g_kankyoHIO.navy.cloud_cloudy_alpha = 1.0f;
    }
    #endif

    #if DEBUG
    if (g_env_light.time_change_rate < 1000.0f) 
    #endif
    {
        stage_stag_info_class* stageinfo = dComIfGp_getStage()->getStagInfo();
        int stage_time = dStage_stagInfo_GetTimeH(stageinfo);
        if ((s8)stage_time >= 0) {
            dComIfGs_setTime((s8)stage_time * 15.0f);
        }
    }

    #if DEBUG
    if (g_env_light.time_change_rate >= 2000.0f) {
        g_env_light.time_change_rate = 0.03f;
    } else if (g_env_light.time_change_rate >= 1000.0f) {
        g_env_light.time_change_rate = 0.0f;
    } else
    #endif
    {
        g_env_light.time_change_rate = 0.012f;
    }

    memset(&g_env_light.moya_se, 0, 0x10);

    #if DEBUG
    hostio_init();
    #endif
}

void dKy_clear_game_init() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();

    dKy_actor_addcol_set(0, 0, 0, 0.0f);

    kankyo->sun_pos.x = 1.0f;
    kankyo->sun_pos.y = 0.0f;
    kankyo->sun_pos.z = 0.0f;

    kankyo->moon_pos.x = -1.0f;
    kankyo->moon_pos.y = 0.0f;
    kankyo->moon_pos.z = 0.0f;

    kankyo->nexttime = -1.0f;
    kankyo->old_time = -1.0f;
    kankyo->dark_daytime = 120.0f;

    kankyo->darktime_week = 0;
    kankyo->fishing_hole_season = 0;
    kankyo->field_0x130a = 0;
    kankyo->wether = 0;
    kankyo->staffroll_next_timer = 0;
}

dScnKy_env_light_c::dScnKy_env_light_c() {
    dKy_clear_game_init();
}

void dScnKy_env_light_c::setDaytime() {
    mDate = dComIfGs_getDate();
    daytime = dComIfGs_getTime();

    #if DEBUG
    switch (g_kankyoHIO.time_change) {
    case 0:
    #endif
        if (using_time_control_tag == 0) {
            if (!dKy_darkworld_check()) {
                if (dComIfGp_event_runCheck() == FALSE) {
                    msg_class* msg = dMsgObject_c::getActor();
                    u8 temp_r29 = true;
                    if (msg != NULL && msg->mode >= 2) {
                        temp_r29 = false;
                    }

                    if (dComIfGp_roomControl_getTimePass() && !field_0x130a && temp_r29) {
                        daytime += time_change_rate;

                        // Stage is Fishing Pond or Hena's Hut
                        if (!strcmp(dComIfGp_getStartStageName(), "F_SP127") ||
                            !strcmp(dComIfGp_getStartStageName(), "R_SP127"))
                        {
                            if (daytime >= 300.0f || daytime <= 60.0f) {
                                daytime += time_change_rate;
                                daytime += time_change_rate;
                            } else if (daytime >= 150.0f && daytime <= 195.0f) {
                                daytime = daytime + time_change_rate;
                            }
                        }

                        if ((u32)daytime >= 360.0f) {
                            daytime = 0.0f;
                            mDate++;
                            dKankyo_DayProc();
                        }
                    } else {
                        #if DEBUG
                        if (fapGmHIO_get2Ddraw()) {
                            dDbVw_Report(190, 65, "TS");
                        }
                        #endif
                    }
                } else {
                    #if DEBUG
                    if (fapGmHIO_get2Ddraw()) {
                        dDbVw_Report(190, 65, "EVENT_PROC!");
                    }
                    #endif
                }
            } else {
                dark_daytime += time_change_rate;
                if ((u32)dark_daytime >= 360.0f) {
                    darktime_week++;
                    dark_daytime = 0.0f;
                }
                daytime = 0.0f;
            }
        }
    #if DEBUG
        break;
    case 1:
        daytime = 90.0f;
        break;
    case 2:
        daytime = 105.0f;
        break;
    case 3:
        daytime = 165.0f;
        break;
    case 4:
        daytime = 255.0f;
        break;
    case 5:
        daytime = 285.0f;
        break;
    case 6:
        daytime = 345.0f;
        break;
    case 7:
        daytime += time_change_rate;
        if ((u32)daytime >= 360.0f) {
            daytime = 0.0f;
            mDate++;
            dKankyo_DayProc();
        }
        break;
    }
    #endif

    #if DEBUG
    static u8 data_8074c978;
    static u8 data_80745860 = 99;

    if (g_kankyoHIO.time_change != data_80745860) {
        if (data_8074c978 >= 2) {
            data_8074c978 = 0;
            data_80745860 = g_kankyoHIO.time_change;
            g_kankyoHIO.light.dKankyo_lightHIOInfoUpDateF();
        } else {
            data_8074c978++;
        }
    }
    #endif

    if (daytime >= 360.0f) {
        daytime = 0.0f;
    }

    dComIfGs_setTime(daytime);
    #if DEBUG
    if (fapGmHIO_get2Ddraw()) {
        dKydb_timedisp();
    }
    #endif

    mDoAud_setHour(dKy_getdaytime_hour());
    mDoAud_setMinute(dKy_getdaytime_minute());
    mDoAud_setWeekday(dKy_get_dayofweek());
    dComIfGs_setDate(mDate);
    g_env_light.using_time_control_tag = 0;
}

void dScnKy_env_light_c::setSunpos() {
    camera_class* camera_p = dComIfGp_getCamera(0);
    cXyz pos;
    f32 parcent;
    f32 sun_angle;
    f32 moon_angle;
    f32 moon_time;

    if (camera_p != NULL && strcmp(dComIfGp_getStartStageName(), "F_SP200") != 0) {
        if (g_env_light.daytime >= 90.0f && g_env_light.daytime <= 270.0f) {
            parcent = get_parcent(270.0f, 90.0f, g_env_light.daytime);
            sun_angle = (parcent * 150.0f) + 105.0f;
        } else {
            sun_angle = g_env_light.daytime;
            if (sun_angle < 90.0f) {
                sun_angle += 360.0f;
            }

            parcent = get_parcent(450.0f, 270.0f, sun_angle);
            sun_angle = (parcent * 210.0f) + 255.0f;
            if (sun_angle > 360.0f) {
                sun_angle -= 360.0f;
            }
        }

        moon_time = 180.0f + g_env_light.daytime;
        if (moon_time >= 360.0f) {
            moon_time -= 360.0f;
        }

        if (moon_time >= 90.0f && moon_time <= 270.0f) {
            parcent = get_parcent(270.0f, 90.0f, moon_time);
            moon_angle = (parcent * 150.0f) + 105.0f;
        } else {
            moon_angle = moon_time;
            if (moon_angle < 90.0f) {
                moon_angle += 360.0f;
            }

            parcent = get_parcent(450.0f, 270.0f, moon_angle);
            moon_angle = (parcent * 210.0f) + 255.0f;
            if (moon_angle > 360.0f) {
                moon_angle -= 360.0f;
            }
        }

        pos.x = sinf(DEG_TO_RAD(sun_angle)) * 80000.0f;
        pos.y = cosf(DEG_TO_RAD(sun_angle)) * 80000.0f;
        pos.z = cosf(DEG_TO_RAD(sun_angle)) * -48000.0f;

        sun_pos.x = camera_p->lookat.eye.x + pos.x;
        sun_pos.y = camera_p->lookat.eye.y - pos.y;
        sun_pos.z = camera_p->lookat.eye.z + pos.z;

        pos.x = sinf(DEG_TO_RAD(moon_angle)) * 80000.0f;
        pos.y = cosf(DEG_TO_RAD(moon_angle)) * 80000.0f;
        pos.z = cosf(DEG_TO_RAD(moon_angle)) * -48000.0f;

        moon_pos.x = pos.x;
        moon_pos.y = -pos.y;
        moon_pos.z = pos.z;
    }
}

f32 dScnKy_env_light_c::getDaytime() {
    return daytime;
}

int dKy_getdaytime_hour() {
    f32 time = dComIfGs_getTime();
    return time / 15.0f;
}

int dKy_getdaytime_minute() {
    f32 time = dComIfGs_getTime();
    f32 var_f30 = ((int)(time * 1000000.0f) % 15000000) / 1000000.0f;
    return var_f30 / 15.0f * 60.0f;
}

BOOL dKy_daynight_check() {
    int hour = dKy_getdaytime_hour();
    if (hour >= 6 && hour < 19) {
        return FALSE;  // day time
    } else {
        return TRUE;  // night time
    }
}

int dKy_getDarktime_hour() {
    f32 time = g_env_light.getDarkDaytime();
    return time / 15.0f;
}

int dKy_getDarktime_minute() {
    f32 time = g_env_light.getDarkDaytime();
    f32 temp_f30 = ((int)(time * 1000000.0f) % 15000000) / 1000000.0f;
    return 60.0f * (temp_f30 / 15.0f);
}

u8 dKy_getDarktime_week() {
    return g_env_light.darktime_week;
}

f32 dScnKy_env_light_c::getDarkDaytime() {
    return dark_daytime;
}

/**
 * Gets stage environment color palette information based on given IDs.
 * This gets both previous and next palette information so that blending between
 * palettes can occur. Both previous and next palette's also get a start and ending palette
 * used to blend color based on time of day.
 *
 * @param prev_envr_id_p Index into stage ENVR info for previous palette
 * @param next_envr_id_p Index into stage ENVR info for next (target) palette
 * @param prev_pat_p Index of stage Pselect (Palette Select) to use for previous palette
 * @param next_pat_p Index of stage Pselect (Palette Select) to use for next (target) palette
 * @param prev_pal_start_id_p Start Palette ID for previous palette color
 * @param prev_pal_end_id_p Ending Palette ID for previous palette color
 * @param next_pal_start_id_p Start Palette ID for next (target) palette color
 * @param next_pal_end_id_p Ending Palette ID for next (target) palette color
 * @param color_ratio_p Ratio in-between start and ending palette to use
 * @param start_pat_pal_id_p Index into stage Pselect for starting palette
 * @param end_pat_pal_id_p Index into stage Pselect for ending palette
 * @param pattern_ratio_p Ratio in-between previous pattern to the next
 * @param init_timer_p
 */
void dScnKy_env_light_c::setLight_palno_get(u8* prev_envr_id_p, u8* next_envr_id_p, u8* prev_pat_p,
                                            u8* next_pat_p, u8* prev_pal_start_id_p,
                                            u8* prev_pal_end_id_p, u8* next_pal_start_id_p,
                                            u8* next_pal_end_id_p, f32* color_ratio_p,
                                            int* start_pat_pal_id_p, int* end_pat_pal_id_p,
                                            f32* pattern_ratio_p, u8* init_timer_p) {
    stage_envr_info_class* envr_p;
    u8 psel_idx = 0;
    int i;
    int sp14 = 0;

    if (*init_timer_p != 0) {
        (*init_timer_p)++;

        if (*init_timer_p > 20) {
            *init_timer_p = 0;
        }

        if (g_env_light.mColPatMode == 0 && g_env_light.mColPatModeGather == 0) {
            *pattern_ratio_p = g_env_light.pat_ratio;

            // previous has reached next, so set previous to next
            if (*pattern_ratio_p >= 1.0f) {
                *prev_envr_id_p = *next_envr_id_p;
                *prev_pat_p = *next_pat_p;
            }
        }
    }

    for (i = 0; i < 11; i++) {
        // if time of day is not within light schedule range, then don't calculate light color
        if (!(daytime >= light_schedule[i].startTime && daytime <= light_schedule[i].endTime)) {
            continue;
        }

        *start_pat_pal_id_p = light_schedule[i].startTimeLight;
        *end_pat_pal_id_p = light_schedule[i].endTimeLight;
        *color_ratio_p = get_parcent(light_schedule[i].endTime, light_schedule[i].startTime, daytime);

        #if DEBUG
        if (dComIfGp_getStageEnvrNumInfo() - 1 < g_env_light.PrevCol) {
            sp14 = TRUE;
            JUT_WARN(2920, "color error 1:[%d]", g_env_light.PrevCol);
        }
        #endif

        envr_p = &g_env_light.stage_envr_info[*prev_envr_id_p];
        switch (*prev_pat_p) {
        case 0:
            psel_idx = envr_p->pselect_id[0];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[0]) {
                sp14 = TRUE;
                JUT_WARN(2936, "color error 2:pat0:[%d]", envr_p->pselect_id[0]);
            }
            #endif
            break;
        case 1:
            psel_idx = envr_p->pselect_id[1];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[1]) {
                sp14 = TRUE;
                JUT_WARN(2947, "color error 3:pat1:[%d]", envr_p->pselect_id[1]);
            }
            #endif
            break;
        case 2:
            psel_idx = envr_p->pselect_id[2];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[2]) {
                sp14 = TRUE;
                JUT_WARN(2958, "color error 4:pat2:[%d]", envr_p->pselect_id[2]);
            }
            #endif
            break;
        case 3:
            psel_idx = envr_p->pselect_id[3];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[3]) {
                sp14 = TRUE;
                JUT_WARN(2969, "color error 5:pat3:[%d]", envr_p->pselect_id[3]);
            }
            #endif
            break;
        case 4:
            psel_idx = envr_p->pselect_id[4];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[4]) {
                sp14 = TRUE;
                JUT_WARN(2980, "color error 6:pat4:[%d]", envr_p->pselect_id[4]);
            }
            #endif
            break;
        case 5:
            psel_idx = envr_p->pselect_id[5];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[5]) {
                sp14 = TRUE;
                JUT_WARN(2991, "color error 7:pat5:[%d]", envr_p->pselect_id[5]);
            }
            #endif
            break;
        case 6:
            psel_idx = envr_p->pselect_id[6];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[6]) {
                sp14 = TRUE;
                JUT_WARN(3002, "color error 8:pat6:[%d]", envr_p->pselect_id[6]);
            }
            #endif
            break;
        case 7:
            psel_idx = envr_p->pselect_id[7];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[7]) {
                sp14 = TRUE;
                JUT_WARN(3013, "color error 9:pat7:[%d]", envr_p->pselect_id[7]);
            }
            #endif
            break;
        default:
            if (*prev_pat_p > 7 && *prev_pat_p < 64) {
                psel_idx = envr_p->pselect_id[*prev_pat_p];

                #if DEBUG
                if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[*prev_pat_p] &&
                    dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[*prev_pat_p] &&
                    g_kankyoHIO.light.m_displayColorPaletteCheckInfo == TRUE)
                {
                    sp14 = TRUE;
                    JUT_WARN(3030, "color error 10:pat8-:[%d]", envr_p->pselect_id[*prev_pat_p]);
                }
                #endif
            }
            break;
        }

        // use a specific palette for underwater (except during morpheel and zant fight)
        if (g_env_light.camera_water_in_status &&
            strcmp(dComIfGp_getStartStageName(), "D_MN08D") != 0 &&
            strcmp(dComIfGp_getStartStageName(), "D_MN01A") != 0)
        {
            if (g_env_light.wether_pat1 == 0) {
                psel_idx = envr_p->pselect_id[8];

                #if DEBUG
                if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[8]) {
                    sp14 = TRUE;
                }
                #endif
            } else {
                psel_idx = envr_p->pselect_id[9];

                #if DEBUG
                if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[9]) {
                    sp14 = TRUE;
                }
                #endif
            }
        }
        // use a specific palette for rollgoal (camera eye gets set below 0.0)
        else if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0)
        {
            camera_class* camera_p = dComIfGp_getCamera(0);

            if (camera_p != NULL && camera_p->lookat.eye.y < 0.0f) {
                psel_idx = envr_p->pselect_id[10];
            }
        }

        if (psel_idx > 250) {
            psel_idx = 0;
        }

        stage_pselect_info_class* pselect_p = &g_env_light.stage_pselect_info[psel_idx];
        switch (*start_pat_pal_id_p) {
        case 0:
            *prev_pal_start_id_p = pselect_p->palette_id[0];
            break;
        case 1:
            *prev_pal_start_id_p = pselect_p->palette_id[1];
            break;
        case 2:
            *prev_pal_start_id_p = pselect_p->palette_id[2];
            break;
        case 3:
            *prev_pal_start_id_p = pselect_p->palette_id[3];
            break;
        case 4:
            *prev_pal_start_id_p = pselect_p->palette_id[4];
            break;
        case 5:
            *prev_pal_start_id_p = pselect_p->palette_id[5];
            break;
        }

        switch (*end_pat_pal_id_p) {
        case 0:
            *prev_pal_end_id_p = pselect_p->palette_id[0];
            break;
        case 1:
            *prev_pal_end_id_p = pselect_p->palette_id[1];
            break;
        case 2:
            *prev_pal_end_id_p = pselect_p->palette_id[2];
            break;
        case 3:
            *prev_pal_end_id_p = pselect_p->palette_id[3];
            break;
        case 4:
            *prev_pal_end_id_p = pselect_p->palette_id[4];
            break;
        case 5:
            *prev_pal_end_id_p = pselect_p->palette_id[5];
            break;
        }

        #if DEBUG
        if (dComIfGp_getStageEnvrNumInfo() - 1 < g_env_light.UseCol) {
            sp14 = TRUE;
            JUT_WARN(3107, "color error 11:[%d]", g_env_light.UseCol);
        }
        #endif

        envr_p = &g_env_light.stage_envr_info[*next_envr_id_p];
        switch (*next_pat_p) {
        case 0:
            psel_idx = envr_p->pselect_id[0];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[0]) {
                sp14 = TRUE;
                JUT_WARN(3122, "color error 12:pat0:[%d]", envr_p->pselect_id[0]);
            }
            #endif
            break;
        case 1:
            psel_idx = envr_p->pselect_id[1];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[1]) {
                sp14 = TRUE;
                JUT_WARN(3133, "color error 13:pat1:[%d]", envr_p->pselect_id[1]);
            }
            #endif
            break;
        case 2:
            psel_idx = envr_p->pselect_id[2];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[2]) {
                sp14 = TRUE;
                JUT_WARN(3144, "color error 14:pat2:[%d]", envr_p->pselect_id[2]);
            }
            #endif
            break;
        case 3:
            psel_idx = envr_p->pselect_id[3];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[3]) {
                sp14 = TRUE;
                JUT_WARN(3155, "color error 15:pat3:[%d]", envr_p->pselect_id[3]);
            }
            #endif
            break;
        case 4:
            psel_idx = envr_p->pselect_id[4];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[4]) {
                sp14 = TRUE;
                JUT_WARN(3166, "color error 16:pat4:[%d]", envr_p->pselect_id[4]);
            }
            #endif
            break;
        case 5:
            psel_idx = envr_p->pselect_id[5];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[5]) {
                sp14 = TRUE;
                JUT_WARN(3177, "color error 17:pat5:[%d]", envr_p->pselect_id[5]);
            }
            #endif
            break;
        case 6:
            psel_idx = envr_p->pselect_id[6];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[6]) {
                sp14 = TRUE;
                JUT_WARN(3188, "color error 18:pat6:[%d]", envr_p->pselect_id[6]);
            }
            #endif
            break;
        case 7:
            psel_idx = envr_p->pselect_id[7];

            #if DEBUG
            if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[7]) {
                sp14 = TRUE;
                JUT_WARN(3199, "color error 19:pat7:[%d]", envr_p->pselect_id[7]);
            }
            #endif
            break;
        default:
            if (*next_pat_p > 7 && *next_pat_p < 64) {
                psel_idx = envr_p->pselect_id[*next_pat_p];

                #if DEBUG
                if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[*next_pat_p] &&
                    dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[*next_pat_p] &&
                    g_kankyoHIO.light.m_displayColorPaletteCheckInfo == TRUE)
                {
                    sp14 = TRUE;
                    JUT_WARN(3218, "color error 20:pat8-:[%d]", envr_p->pselect_id[*next_pat_p]);
                }
                #endif
            }
            break;
        }

        if (psel_idx > 250) {
            psel_idx = 0;
        }

        pselect_p = &g_env_light.stage_pselect_info[psel_idx];
        if (*prev_envr_id_p != *next_envr_id_p || *prev_pat_p != *next_pat_p) {
            if (pselect_p->change_rate < (1.0f / 30)) {
                pselect_p->change_rate = (1.0f / 30);
            }

            if (g_env_light.mColPatMode == 0) {
                if (pselect_p->change_rate > 0.0f) {
                    *pattern_ratio_p += (1.0f / 30) / pselect_p->change_rate;
                }

                // pattern change rate is faster in hyrule field
                if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 &&
                    *prev_pat_p == *next_pat_p)
                {
                    *pattern_ratio_p += (1.0f / 15);
                }

                if (*pattern_ratio_p >= 1.0f) {
                    *prev_envr_id_p = *next_envr_id_p;
                    *prev_pat_p = *next_pat_p;
                    *pattern_ratio_p = 1.0f;
                }
            }
        }

        // use a specific palette for underwater (except during morpheel and zant fight)
        if (g_env_light.camera_water_in_status &&
            strcmp(dComIfGp_getStartStageName(), "D_MN08D") != 0 &&
            strcmp(dComIfGp_getStartStageName(), "D_MN01A") != 0)
        {
            if (g_env_light.wether_pat1 == 0) {
                pselect_p = &g_env_light.stage_pselect_info[envr_p->pselect_id[8]];

                #if DEBUG
                if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[8]) {
                    sp14 = TRUE;
                }
                #endif
            } else {
                pselect_p = &g_env_light.stage_pselect_info[envr_p->pselect_id[9]];

                #if DEBUG
                if (dComIfGp_getStagePselectNumInfo() - 1 < envr_p->pselect_id[9]) {
                    sp14 = TRUE;
                }
                #endif
            }
        }
        // use a specific palette for rollgoal (camera eye gets set below 0.0)
        else if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0)
        {
            camera_class* camera_p = dComIfGp_getCamera(0);

            if (camera_p != NULL && camera_p->lookat.eye.y < 0.0f) {
                pselect_p = &g_env_light.stage_pselect_info[envr_p->pselect_id[10]];
            }
        }

        switch (*start_pat_pal_id_p) {
        case 0:
            *next_pal_start_id_p = pselect_p->palette_id[0];
            break;
        case 1:
            *next_pal_start_id_p = pselect_p->palette_id[1];
            break;
        case 2:
            *next_pal_start_id_p = pselect_p->palette_id[2];
            break;
        case 3:
            *next_pal_start_id_p = pselect_p->palette_id[3];
            break;
        case 4:
            *next_pal_start_id_p = pselect_p->palette_id[4];
            break;
        case 5:
            *next_pal_start_id_p = pselect_p->palette_id[5];
            break;
        }

        switch (*end_pat_pal_id_p) {
        case 0:
            *next_pal_end_id_p = pselect_p->palette_id[0];
            break;
        case 1:
            *next_pal_end_id_p = pselect_p->palette_id[1];
            break;
        case 2:
            *next_pal_end_id_p = pselect_p->palette_id[2];
            break;
        case 3:
            *next_pal_end_id_p = pselect_p->palette_id[3];
            break;
        case 4:
            *next_pal_end_id_p = pselect_p->palette_id[4];
            break;
        case 5:
            *next_pal_end_id_p = pselect_p->palette_id[5];
            break;
        }
        break;
    }

    #if DEBUG
    dKydb_HIO_kcolor_debug(prev_pal_start_id_p, next_pal_start_id_p, prev_pal_end_id_p, next_pal_end_id_p);
    if (sp14) {
        *prev_pal_start_id_p = 255;
        *next_pal_start_id_p = 255;
        *prev_pal_end_id_p = 255;
        *next_pal_end_id_p = 255;
    }
    #else
    if (*prev_pal_start_id_p > 250) {
        *prev_pal_start_id_p = 0;
    }

    if (*next_pal_start_id_p > 250) {
        *next_pal_start_id_p = 0;
    }

    if (*prev_pal_end_id_p > 250) {
        *prev_pal_end_id_p = 0;
    }

    if (*next_pal_end_id_p > 250) {
        *next_pal_end_id_p = 0;
    }
    #endif
}

/* 801A0340-801A040C 19AC80 00CC+00 3/3 0/0 0/0 .text
 * dKy_calc_color_set__FP11_GXColorS10P15color_RGB_classP15color_RGB_classP15color_RGB_classP15color_RGB_classff11_GXColorS10f
 */
void dKy_calc_color_set(GXColorS10* out_color_p, color_RGB_class* color_a_start_p,
                        color_RGB_class* color_b_start_p, color_RGB_class* color_a_end_p,
                        color_RGB_class* color_b_end_p, f32 color_ratio, f32 blend_ratio,
                        GXColorS10 add_col, f32 scale) {
    out_color_p->r =
        kankyo_color_ratio_set(color_a_start_p->r, color_a_end_p->r, color_ratio,
                               color_b_start_p->r, color_b_end_p->r, blend_ratio, add_col.r, scale);
    out_color_p->g =
        kankyo_color_ratio_set(color_a_start_p->g, color_a_end_p->g, color_ratio,
                               color_b_start_p->g, color_b_end_p->g, blend_ratio, add_col.g, scale);
    out_color_p->b =
        kankyo_color_ratio_set(color_a_start_p->b, color_a_end_p->b, color_ratio,
                               color_b_start_p->b, color_b_end_p->b, blend_ratio, add_col.b, scale);
}

void dScnKy_env_light_c::setLight() {
    u8* init_timer = &g_env_light.light_init_timer;
    camera_class* camera_p = dComIfGp_getCamera(0);

    GXColorS10 add_col;
    add_col.r = 0;
    add_col.g = 0;
    add_col.b = 0;
    add_col.a = 0;

    if (camera_p != NULL) {
        if (g_env_light.camera_water_in_status) {
            dBgS_CamGndChk_Wtr camchk;
            cXyz chkpos;
            f32 sp84;
            f32 sp80 = 1.0f;

            if (daPy_py_c::checkZoraWearFlg() && !daPy_py_c::checkNowWolf()) {
                dKy_WaterIn_Light_set();
            }

            chkpos = camera_p->lookat.eye;
            chkpos.y += 100000.0f;

            camchk.SetPos(&chkpos);
            sp84 = dComIfG_Bgsp().GroundCross(&camchk);

            if (sp84 > camera_p->lookat.eye.y) {
                sp84 -= camera_p->lookat.eye.y;
                sp80 = 1.0f - sp84 / 2250.0f;
                if (sp80 < 0.0f) {
                    sp80 = 0.0f;
                }

                sp80 = ((sp80 * 0.7f) * 0.5f) + 0.5f;
            }

            cLib_addCalc(&g_env_light.field_0x1258, 1.0f, 0.25f, 0.01f, 0.0000000000001f);
        } else {
            g_env_light.field_0x1258 = 0.0f;
        }

        u8 prev_pal_start_id;
        u8 next_pal_start_id;
        u8 prev_pal_end_id;
        u8 next_pal_end_id;
        f32 color_ratio;
        int start_pat_pal_id;
        int end_pat_pal_id;
        setLight_palno_get(&g_env_light.PrevCol, &g_env_light.UseCol, &g_env_light.wether_pat0,
                           &g_env_light.wether_pat1, &prev_pal_start_id, &prev_pal_end_id,
                           &next_pal_start_id, &next_pal_end_id, &color_ratio, &start_pat_pal_id,
                           &end_pat_pal_id, &g_env_light.pat_ratio, init_timer);

        if (prev_pal_start_id == 0xFF) {
            actor_amb_col.r = 255;
            actor_amb_col.g = 0;
            actor_amb_col.b = 0;

            for (int i = 0; i < 4; i++) {
                bg_amb_col[i].r = 255;
                bg_amb_col[i].g = 0;
                bg_amb_col[i].b = 0;
                bg_amb_col[i].a = 0;
            }
        } else {
            stage_palette_info_class* prev_pal_start_p = &g_env_light.stage_palette_info[prev_pal_start_id];
            stage_palette_info_class* prev_pal_end_p = &g_env_light.stage_palette_info[prev_pal_end_id];
            stage_palette_info_class* next_pal_start_p = &g_env_light.stage_palette_info[next_pal_start_id];
            stage_palette_info_class* next_pal_end_p = &g_env_light.stage_palette_info[next_pal_end_id];

            #if DEBUG
            if (!g_kankyoHIO.light.m_HOSTIO_setting && !g_kankyoHIO.vrbox.m_VrboxSetting)
            #endif
            {

            dKy_calc_color_set(&actor_amb_col, &prev_pal_start_p->actor_amb_col,
                               &next_pal_start_p->actor_amb_col, &prev_pal_end_p->actor_amb_col,
                               &next_pal_end_p->actor_amb_col, color_ratio, g_env_light.pat_ratio,
                               add_col,
                               g_env_light.field_0x1210 * (g_env_light.now_actcol_ratio * g_env_light.now_actcol_ratio));

            for (int i = 0; i < 4; i++) {
                if (i != 3 || (strcmp(dComIfGp_getStartStageName(), "R_SP127") != 0 &&
                               strcmp(dComIfGp_getStartStageName(), "F_SP127") != 0))
                {
                    dKy_calc_color_set(&bg_amb_col[i], &prev_pal_start_p->bg_amb_col[i],
                                       &next_pal_start_p->bg_amb_col[i],
                                       &prev_pal_end_p->bg_amb_col[i],
                                       &next_pal_end_p->bg_amb_col[i], color_ratio,
                                       g_env_light.pat_ratio, add_col, g_env_light.now_bgcol_ratio);
                }
            }

            if (daPy_py_c::checkNowWolfPowerUp()) {
                dKy_WolfPowerup_AmbCol(&actor_amb_col);
                dKy_WolfPowerup_BgAmbCol(&bg_amb_col[0]);
            }

            bg_amb_col[1].a = (u8)kankyo_color_ratio_set(
                prev_pal_start_p->BG1_amb_alpha, prev_pal_end_p->BG1_amb_alpha, color_ratio,
                next_pal_start_p->BG1_amb_alpha, next_pal_end_p->BG1_amb_alpha,
                g_env_light.pat_ratio, 0, 1.0f);
            bg_amb_col[2].a = (u8)kankyo_color_ratio_set(
                prev_pal_start_p->BG2_amb_alpha, prev_pal_end_p->BG2_amb_alpha, color_ratio,
                next_pal_start_p->BG2_amb_alpha, next_pal_end_p->BG2_amb_alpha,
                g_env_light.pat_ratio, 0, 1.0f);
            bg_amb_col[3].a = (u8)kankyo_color_ratio_set(
                prev_pal_start_p->BG3_amb_alpha, prev_pal_end_p->BG3_amb_alpha, color_ratio,
                next_pal_start_p->BG3_amb_alpha, next_pal_end_p->BG3_amb_alpha,
                g_env_light.pat_ratio, 0, 1.0f);

            s16 bg_light_inf = kankyo_color_ratio_set(
                prev_pal_start_p->bg_light_influence, prev_pal_end_p->bg_light_influence,
                color_ratio, next_pal_start_p->bg_light_influence,
                next_pal_end_p->bg_light_influence, g_env_light.pat_ratio, 0, 1.0f);
            bg_light_influence = bg_light_inf * 0.01f;
            if (bg_light_influence > 2.0f) {
                bg_light_influence = 1.0f;
            }

            mFogDensity = kankyo_color_ratio_set(
                prev_pal_start_p->cloud_shadow_density, prev_pal_end_p->cloud_shadow_density,
                color_ratio, next_pal_start_p->cloud_shadow_density,
                next_pal_end_p->cloud_shadow_density, g_env_light.pat_ratio, 0, 1.0f);
            if (daPy_py_c::checkNowWolfPowerUp()) {
                mFogDensity = -1;
            }

            for (int i = 0; i < 6; i++) {
                dKy_calc_color_set(&dungeonlight_col[i], &prev_pal_start_p->plight_col[i],
                                   &next_pal_start_p->plight_col[i], &prev_pal_end_p->plight_col[i],
                                   &next_pal_end_p->plight_col[i], color_ratio,
                                   g_env_light.pat_ratio, add_col, g_env_light.now_bgcol_ratio);

                if (daPy_py_c::checkNowWolfPowerUp()) {
                    dungeonlight_col[i].r = 0;
                    dungeonlight_col[i].g = 0;
                    dungeonlight_col[i].b = 0;
                }

                g_env_light.dungeonlight[i].mColor.r = dungeonlight_col[i].r;
                g_env_light.dungeonlight[i].mColor.g = dungeonlight_col[i].g;
                g_env_light.dungeonlight[i].mColor.b = dungeonlight_col[i].b;
            }

            dKy_calc_color_set(&fog_col, &prev_pal_start_p->fog_col, &next_pal_start_p->fog_col,
                               &prev_pal_end_p->fog_col, &next_pal_end_p->fog_col, color_ratio,
                               g_env_light.pat_ratio, addcol_fog, g_env_light.now_fogcol_ratio);
            mFogNear = float_kankyo_color_ratio_set(
                prev_pal_start_p->fog_start_z, prev_pal_end_p->fog_start_z, color_ratio,
                next_pal_start_p->fog_start_z, next_pal_end_p->fog_start_z, g_env_light.pat_ratio,
                g_env_light.field_0x11ec, g_env_light.field_0x11f4);
            mFogFar = float_kankyo_color_ratio_set(
                prev_pal_start_p->fog_end_z, prev_pal_end_p->fog_end_z, color_ratio,
                next_pal_start_p->fog_end_z, next_pal_end_p->fog_end_z, g_env_light.pat_ratio,
                g_env_light.field_0x11f0, g_env_light.field_0x11f4);

            if (daPy_py_c::checkNowWolfPowerUp()) {
                fog_col.r = 0;
                fog_col.g = 0;
                fog_col.b = 0;
                dKy_WolfPowerup_FogNearFar(&mFogNear, &mFogFar);
            }
            }

            u8 prev_bloom_start_id = prev_pal_start_p->bloom_tbl_id;
            u8 next_bloom_start_id = next_pal_start_p->bloom_tbl_id;
            u8 prev_bloom_end_id = prev_pal_end_p->bloom_tbl_id;
            u8 next_bloom_end_id = next_pal_end_p->bloom_tbl_id;

            #if DEBUG
            if (g_kankyoHIO.light.m_HOSTIO_setting) {
                prev_bloom_start_id = next_bloom_start_id = prev_bloom_end_id = next_bloom_end_id = g_kankyoHIO.bloom.m_saturationPattern;
            } else {
                g_kankyoHIO.bloom.m_saturationPattern = prev_pal_end_p->bloom_tbl_id;
            }

            if (g_kankyoHIO.navy.twilight_sense_saturation_mode && daPy_py_c::checkNowWolfPowerUp()) {
                prev_bloom_start_id = next_bloom_start_id = prev_bloom_end_id = next_bloom_end_id = g_kankyoHIO.navy.twilight_sense_saturation_mode;
            }
            #endif

            if (daPy_py_c::checkNowWolfPowerUp()) {
                prev_bloom_start_id = next_bloom_start_id = prev_bloom_end_id = next_bloom_end_id = 3;
            }

            f32 temp_f31;
            if (g_env_light.field_0x12fc >= 0) {
                prev_bloom_end_id = g_env_light.field_0x12fc;
                next_bloom_end_id = g_env_light.field_0x12fc;
                temp_f31 = color_ratio;
                color_ratio = g_env_light.field_0x1278;
            }

            dKydata_BloomInfo_c* bloomInf0_p;
            dKydata_BloomInfo_c* bloomInf1_p;
            dKydata_BloomInfo_c* bloomInf2_p;
            dKydata_BloomInfo_c* bloomInf3_p;

            bloomInf0_p = dKyd_BloomInf_tbl_getp(prev_bloom_start_id);
            bloomInf2_p = dKyd_BloomInf_tbl_getp(next_bloom_start_id);
            bloomInf1_p = dKyd_BloomInf_tbl_getp(prev_bloom_end_id);
            bloomInf3_p = dKyd_BloomInf_tbl_getp(next_bloom_end_id);

            #if DEBUG
            if (bloomInf0_p == NULL || bloomInf2_p == NULL || bloomInf1_p == NULL || bloomInf3_p == NULL) {
                JUT_ASSERT(3839, FALSE);
            }
            #endif

            u8 bloom_point = kankyo_color_ratio_set(
                bloomInf0_p->info.mThreshold, bloomInf1_p->info.mThreshold, color_ratio,
                bloomInf2_p->info.mThreshold, bloomInf3_p->info.mThreshold, g_env_light.pat_ratio, 0, 1.0f);
            mDoGph_gInf_c::getBloom()->setPoint(bloom_point);

            u8 blure_size = (u8)kankyo_color_ratio_set(
                bloomInf0_p->info.mBlurAmount, bloomInf1_p->info.mBlurAmount, color_ratio,
                bloomInf2_p->info.mBlurAmount, bloomInf3_p->info.mBlurAmount, g_env_light.pat_ratio, 0, 1.0f);

            if (dKy_darkworld_check()) {
                static s16 S_fuwan_sin;

                f32 sin = cM_ssin(S_fuwan_sin);
                S_fuwan_sin += (s16)cM_rndF(2000.0f) + 500;

                blure_size += (u8)(sin * (0.2f * blure_size));
            }

            mDoGph_gInf_c::getBloom()->setBlureSize(blure_size);
            u8 sp21 = kankyo_color_ratio_set(
                bloomInf0_p->info.mDensity, bloomInf1_p->info.mDensity, color_ratio, bloomInf2_p->info.mDensity,
                bloomInf3_p->info.mDensity, g_env_light.pat_ratio, 0, 1.0f);
            mDoGph_gInf_c::getBloom()->setBlureRatio(sp21);

            GXColor bloom_blend_col;
            bloom_blend_col.r = kankyo_color_ratio_set(
                bloomInf0_p->info.mColorR, bloomInf1_p->info.mColorR, color_ratio, bloomInf2_p->info.mColorR,
                bloomInf3_p->info.mColorR, g_env_light.pat_ratio, 0, 1.0f);
            bloom_blend_col.g = kankyo_color_ratio_set(
                bloomInf0_p->info.mColorG, bloomInf1_p->info.mColorG, color_ratio, bloomInf2_p->info.mColorG,
                bloomInf3_p->info.mColorG, g_env_light.pat_ratio, 0, 1.0f);
            bloom_blend_col.b = kankyo_color_ratio_set(
                bloomInf0_p->info.mColorB, bloomInf1_p->info.mColorB, color_ratio, bloomInf2_p->info.mColorB,
                bloomInf3_p->info.mColorB, g_env_light.pat_ratio, 0, 1.0f);
            bloom_blend_col.a =
                kankyo_color_ratio_set(bloomInf0_p->info.mOrigDensity, bloomInf1_p->info.mOrigDensity,
                                       color_ratio, bloomInf2_p->info.mOrigDensity,
                                       bloomInf3_p->info.mOrigDensity, g_env_light.pat_ratio, 0, 1.0f);
            mDoGph_gInf_c::getBloom()->setBlendColor(bloom_blend_col);

            if (g_env_light.field_0x12fc >= 0) {
                color_ratio = temp_f31;
            }

            GXColor bloom_mono_col;
            bloom_mono_col.r = kankyo_color_ratio_set(
                bloomInf0_p->info.mSaturateSubtractR, bloomInf1_p->info.mSaturateSubtractR, color_ratio,
                bloomInf2_p->info.mSaturateSubtractR, bloomInf3_p->info.mSaturateSubtractR,
                g_env_light.pat_ratio, 0, 1.0f);
            bloom_mono_col.g = kankyo_color_ratio_set(
                bloomInf0_p->info.mSaturateSubtractG, bloomInf1_p->info.mSaturateSubtractG, color_ratio,
                bloomInf2_p->info.mSaturateSubtractG, bloomInf3_p->info.mSaturateSubtractG,
                g_env_light.pat_ratio, 0, 1.0f);
            bloom_mono_col.b = kankyo_color_ratio_set(
                bloomInf0_p->info.mSaturateSubtractB, bloomInf1_p->info.mSaturateSubtractB, color_ratio,
                bloomInf2_p->info.mSaturateSubtractB, bloomInf3_p->info.mSaturateSubtractB,
                g_env_light.pat_ratio, 0, 1.0f);
            bloom_mono_col.a = kankyo_color_ratio_set(
                bloomInf0_p->info.mSaturateSubtractA, bloomInf1_p->info.mSaturateSubtractA, color_ratio,
                bloomInf2_p->info.mSaturateSubtractA, bloomInf3_p->info.mSaturateSubtractA,
                g_env_light.pat_ratio, 0, 1.0f);
            mDoGph_gInf_c::getBloom()->setMonoColor(bloom_mono_col);

            if (bloom_point >= 0xFF) {
                mDoGph_gInf_c::getBloom()->setEnable(0);
            } else {
                u8 mode = 0;
                mDoGph_gInf_c::getBloom()->setEnable(1);

                if (prev_bloom_start_id != 0 && bloomInf0_p->info.mType != 0) {
                    mode = 1;
                } else if (next_bloom_start_id != 0 && bloomInf2_p->info.mType != 0) {
                    mode = 1;
                } else if (prev_bloom_end_id != 0 && bloomInf1_p->info.mType != 0) {
                    mode = 1;
                } else if (next_bloom_end_id != 0 && bloomInf3_p->info.mType != 0) {
                    mode = 1;
                }

                mDoGph_gInf_c::getBloom()->setMode(mode);
            }

            f32 var_f30;
            if (dKy_Outdoor_check() == true) {
                static f32 now_shadow_alpha[] = {0.25f, 0.35f, 0.6f, 0.6f, 0.25f, 0.35f};

                f32 var_f1;
                f32 var_f2;
                f32 var_f4;
                f32 var_f5;

                if (start_pat_pal_id < 6) {
                    var_f1 = now_shadow_alpha[start_pat_pal_id];
                } else {
                    var_f1 = 0.25f;
                }

                if (end_pat_pal_id < 6) {
                    var_f2 = now_shadow_alpha[end_pat_pal_id];
                } else {
                    var_f2 = 0.25f;
                }

                var_f4 = 0.25f;
                var_f5 = 0.25f;

                if (prev_pal_start_p == next_pal_start_p) {
                    var_f4 = var_f1;
                    var_f5 = var_f2;
                }

                var_f30 = float_kankyo_color_ratio_set(var_f1, var_f2, color_ratio, var_f4, var_f5,
                                                       g_env_light.pat_ratio, 0.0f, 0.0f);

                field_0x123c = 0.8f;
                if (mColpatWeather != 0) {
                    var_f30 *= 0.4f;
                }
            } else {
                var_f30 = 0.4f;
                field_0x123c = 0.65f;
            }

            if (dKy_darkworld_check()) {
                var_f30 = 0.55f;
                field_0x123c = 0.55f;
            }

            #if DEBUG
            if (g_kankyoHIO.navy.shadow_adjust_ON) {
                var_f30 = g_kankyoHIO.navy.shadow_normal_alpha;
                field_0x123c = g_kankyoHIO.navy.shadow_max_alpha;
            }
            #endif

            if (g_env_light.light_init_timer != 0) {
                field_0x1238 = var_f30;
            } else {
                cLib_addCalc(&field_0x1238, var_f30, 0.05f, 0.005f, 0.000001f);
            }

            #if DEBUG
            if (!g_kankyoHIO.light.m_HOSTIO_setting && !g_kankyoHIO.vrbox.m_VrboxSetting)
            #endif
            {
            
            u8 sp2B = prev_pal_start_p->vrboxcol_id;
            u8 sp2A = prev_pal_end_p->vrboxcol_id;
            u8 sp29 = next_pal_start_p->vrboxcol_id;
            u8 sp28 = next_pal_end_p->vrboxcol_id;

            #if DEBUG
            dKydb_HIO_vrbox_debug(&sp2B, &sp2A, &sp29, &sp28);
            #endif

            stage_vrboxcol_info_class* prev_vrboxcol_start_p = &g_env_light.stage_vrboxcol_info[sp2B];
            stage_vrboxcol_info_class* prev_vrboxcol_end_p = &g_env_light.stage_vrboxcol_info[sp2A];
            stage_vrboxcol_info_class* next_vrboxcol_start_p = &g_env_light.stage_vrboxcol_info[sp29];
            stage_vrboxcol_info_class* next_vrboxcol_end_p = &g_env_light.stage_vrboxcol_info[sp28];

            vrbox_sky_col.r = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->sky_col.r, prev_vrboxcol_end_p->sky_col.r, color_ratio,
                next_vrboxcol_start_p->sky_col.r, next_vrboxcol_end_p->sky_col.r,
                g_env_light.pat_ratio, vrbox_addcol_sky0.r,
                g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11c8);
            vrbox_sky_col.g = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->sky_col.g, prev_vrboxcol_end_p->sky_col.g, color_ratio,
                next_vrboxcol_start_p->sky_col.g, next_vrboxcol_end_p->sky_col.g,
                g_env_light.pat_ratio, vrbox_addcol_sky0.g,
                g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11cc);
            vrbox_sky_col.b = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->sky_col.b, prev_vrboxcol_end_p->sky_col.b, color_ratio,
                next_vrboxcol_start_p->sky_col.b, next_vrboxcol_end_p->sky_col.b,
                g_env_light.pat_ratio, vrbox_addcol_sky0.b,
                g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11d0);
            vrbox_sky_col.a = 255;

            vrbox_kumo_top_col.r = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_top_col.r, prev_vrboxcol_end_p->kumo_top_col.r,
                color_ratio, next_vrboxcol_start_p->kumo_top_col.r,
                next_vrboxcol_end_p->kumo_top_col.r, g_env_light.pat_ratio, vrbox_addcol_sky0.r,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11c8);
            vrbox_kumo_top_col.g = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_top_col.g, prev_vrboxcol_end_p->kumo_top_col.g,
                color_ratio, next_vrboxcol_start_p->kumo_top_col.g,
                next_vrboxcol_end_p->kumo_top_col.g, g_env_light.pat_ratio, vrbox_addcol_sky0.g,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11cc);
            vrbox_kumo_top_col.b = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_top_col.b, prev_vrboxcol_end_p->kumo_top_col.b,
                color_ratio, next_vrboxcol_start_p->kumo_top_col.b,
                next_vrboxcol_end_p->kumo_top_col.b, g_env_light.pat_ratio, vrbox_addcol_sky0.b,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11d0);
            vrbox_kumo_top_col.a = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_shadow_col.a, prev_vrboxcol_end_p->kumo_shadow_col.a,
                color_ratio, next_vrboxcol_start_p->kumo_shadow_col.a,
                next_vrboxcol_end_p->kumo_shadow_col.a, g_env_light.pat_ratio, 0, 1.0f);

            vrbox_kumo_bottom_col.r = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_bottom_col.r, prev_vrboxcol_end_p->kumo_bottom_col.r,
                color_ratio, next_vrboxcol_start_p->kumo_bottom_col.r,
                next_vrboxcol_end_p->kumo_bottom_col.r, g_env_light.pat_ratio, vrbox_addcol_sky0.r,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11c8);
            vrbox_kumo_bottom_col.g = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_bottom_col.g, prev_vrboxcol_end_p->kumo_bottom_col.g,
                color_ratio, next_vrboxcol_start_p->kumo_bottom_col.g,
                next_vrboxcol_end_p->kumo_bottom_col.g, g_env_light.pat_ratio, vrbox_addcol_sky0.g,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11cc);
            vrbox_kumo_bottom_col.b = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_bottom_col.b, prev_vrboxcol_end_p->kumo_bottom_col.b,
                color_ratio, next_vrboxcol_start_p->kumo_bottom_col.b,
                next_vrboxcol_end_p->kumo_bottom_col.b, g_env_light.pat_ratio, vrbox_addcol_sky0.b,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11d0);

            vrbox_kumo_shadow_col.r = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_shadow_col.r, prev_vrboxcol_end_p->kumo_shadow_col.r,
                color_ratio, next_vrboxcol_start_p->kumo_shadow_col.r,
                next_vrboxcol_end_p->kumo_shadow_col.r, g_env_light.pat_ratio, vrbox_addcol_sky0.r,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11c8);
            vrbox_kumo_shadow_col.g = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_shadow_col.g, prev_vrboxcol_end_p->kumo_shadow_col.g,
                color_ratio, next_vrboxcol_start_p->kumo_shadow_col.g,
                next_vrboxcol_end_p->kumo_shadow_col.g, g_env_light.pat_ratio, vrbox_addcol_sky0.g,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11cc);
            vrbox_kumo_shadow_col.b = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kumo_shadow_col.b, prev_vrboxcol_end_p->kumo_shadow_col.b,
                color_ratio, next_vrboxcol_start_p->kumo_shadow_col.b,
                next_vrboxcol_end_p->kumo_shadow_col.b, g_env_light.pat_ratio, vrbox_addcol_sky0.b,
                g_env_light.now_vrboxkumocol_ratio * g_env_light.unk_0x11d0);

            vrbox_kasumi_outer_col.r = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_outer_col.r, prev_vrboxcol_end_p->kasumi_outer_col.r,
                color_ratio, next_vrboxcol_start_p->kasumi_outer_col.r,
                next_vrboxcol_end_p->kasumi_outer_col.r, g_env_light.pat_ratio,
                vrbox_addcol_kasumi.r, g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11c8);
            vrbox_kasumi_outer_col.g = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_outer_col.g, prev_vrboxcol_end_p->kasumi_outer_col.g,
                color_ratio, next_vrboxcol_start_p->kasumi_outer_col.g,
                next_vrboxcol_end_p->kasumi_outer_col.g, g_env_light.pat_ratio,
                vrbox_addcol_kasumi.g, g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11cc);
            vrbox_kasumi_outer_col.b = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_outer_col.b, prev_vrboxcol_end_p->kasumi_outer_col.b,
                color_ratio, next_vrboxcol_start_p->kasumi_outer_col.b,
                next_vrboxcol_end_p->kasumi_outer_col.b, g_env_light.pat_ratio,
                vrbox_addcol_kasumi.b, g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11d0);
            vrbox_kasumi_outer_col.a = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_outer_col.a, prev_vrboxcol_end_p->kasumi_outer_col.a,
                color_ratio, next_vrboxcol_start_p->kasumi_outer_col.a,
                next_vrboxcol_end_p->kasumi_outer_col.a, g_env_light.pat_ratio, 0, 1.0f);

            vrbox_kasumi_inner_col.r = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_inner_col.r, prev_vrboxcol_end_p->kasumi_inner_col.r,
                color_ratio, next_vrboxcol_start_p->kasumi_inner_col.r,
                next_vrboxcol_end_p->kasumi_inner_col.r, g_env_light.pat_ratio,
                vrbox_addcol_kasumi.r, g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11c8);
            vrbox_kasumi_inner_col.g = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_inner_col.g, prev_vrboxcol_end_p->kasumi_inner_col.g,
                color_ratio, next_vrboxcol_start_p->kasumi_inner_col.g,
                next_vrboxcol_end_p->kasumi_inner_col.g, g_env_light.pat_ratio,
                vrbox_addcol_kasumi.g, g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11cc);
            vrbox_kasumi_inner_col.b = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_inner_col.b, prev_vrboxcol_end_p->kasumi_inner_col.b,
                color_ratio, next_vrboxcol_start_p->kasumi_inner_col.b,
                next_vrboxcol_end_p->kasumi_inner_col.b, g_env_light.pat_ratio,
                vrbox_addcol_kasumi.b, g_env_light.now_vrboxsoracol_ratio * g_env_light.unk_0x11d0);
            vrbox_kasumi_inner_col.a = kankyo_color_ratio_set(
                prev_vrboxcol_start_p->kasumi_inner_col.a, prev_vrboxcol_end_p->kasumi_inner_col.a,
                color_ratio, next_vrboxcol_start_p->kasumi_inner_col.a,
                next_vrboxcol_end_p->kasumi_inner_col.a, g_env_light.pat_ratio, 0, 1.0f);

            if (daPy_py_c::checkNowWolfPowerUp()) {
                vrbox_sky_col.r = 0;
                vrbox_sky_col.g = 0;
                vrbox_sky_col.b = 0;

                vrbox_kumo_top_col.r = 0;
                vrbox_kumo_top_col.g = 0;
                vrbox_kumo_top_col.b = 0;

                vrbox_kumo_bottom_col.r = 0;
                vrbox_kumo_bottom_col.g = 0;
                vrbox_kumo_bottom_col.b = 0;

                vrbox_kumo_shadow_col.r = 0;
                vrbox_kumo_shadow_col.g = 0;
                vrbox_kumo_shadow_col.b = 0;

                vrbox_kasumi_outer_col.r = 0;
                vrbox_kasumi_outer_col.g = 0;
                vrbox_kasumi_outer_col.b = 0;

                vrbox_kasumi_inner_col.r = 0;
                vrbox_kasumi_inner_col.g = 0;
                vrbox_kasumi_inner_col.b = 0;
            }
            }

            #if DEBUG
            dKydb_HIO_debug_TVdsp(color_ratio, g_env_light.pat_ratio, start_pat_pal_id, end_pat_pal_id, lightMask);
            #endif
        }
    }
}

void dScnKy_env_light_c::setLight_bg(dKy_tevstr_c* tevstr_p, GXColorS10* bg_col_p,
                                     GXColorS10* fog_col_p, f32* fog_near_p, f32* fog_far_p) {
    tevstr_p->wether_pat0 = g_env_light.wether_pat0;
    tevstr_p->wether_pat1 = g_env_light.wether_pat1;

    if (tevstr_p->wether_pat0 != tevstr_p->wether_pat1) {
        tevstr_p->pat_ratio = g_env_light.pat_ratio;
    }

    u8 prev_pal_start_id;
    u8 next_pal_start_id;
    u8 prev_pal_end_id;
    u8 next_pal_end_id;
    f32 color_ratio;
    int start_pat_pal_id;
    int end_pat_pal_id;
    int i;
    setLight_palno_get(&tevstr_p->PrevCol, &tevstr_p->UseCol, &tevstr_p->wether_pat0,
                       &tevstr_p->wether_pat1, &prev_pal_start_id, &prev_pal_end_id,
                       &next_pal_start_id, &next_pal_end_id, &color_ratio, &start_pat_pal_id,
                       &end_pat_pal_id, &tevstr_p->pat_ratio, &tevstr_p->mInitTimer);

    if (prev_pal_start_id == 0xFF) {
        for (i = 0; i < 4; i++) {
            bg_col_p[i].r = 255;
            bg_col_p[i].g = 0;
            bg_col_p[i].b = 0;
        }
    } else {
        stage_palette_info_class* prev_pal_start_p = &g_env_light.stage_palette_info[prev_pal_start_id];
        stage_palette_info_class* prev_pal_end_p = &g_env_light.stage_palette_info[prev_pal_end_id];
        stage_palette_info_class* next_pal_start_p = &g_env_light.stage_palette_info[next_pal_start_id];
        stage_palette_info_class* next_pal_end_p = &g_env_light.stage_palette_info[next_pal_end_id];

        for (i = 0; i < 4; i++) {
            dKy_calc_color_set(&bg_col_p[i], &prev_pal_start_p->bg_amb_col[i],
                               &next_pal_start_p->bg_amb_col[i], &prev_pal_end_p->bg_amb_col[i],
                               &next_pal_end_p->bg_amb_col[i], color_ratio, tevstr_p->pat_ratio,
                               bg_addcol_amb, g_env_light.now_bgcol_ratio);
        }

        if (daPy_py_c::checkNowWolfPowerUp()) {
            dKy_WolfPowerup_BgAmbCol(bg_col_p);
        }

        bg_col_p[0].a =
        bg_col_p[1].a =
        bg_col_p[2].a =
        bg_col_p[3].a = 255;

        GXColorS10 sp68;
        sp68.r = 0;
        sp68.g = 0;
        sp68.b = 0;
        sp68.a = 0;

        GXColorS10 plight_colors[6];
        for (i = 0; i < 6; i++) {
            dKy_calc_color_set(&plight_colors[i], &prev_pal_start_p->plight_col[i],
                               &next_pal_start_p->plight_col[i], &prev_pal_end_p->plight_col[i],
                               &next_pal_end_p->plight_col[i], color_ratio, tevstr_p->pat_ratio,
                               bg_addcol_amb, g_env_light.now_bgcol_ratio);

            if (daPy_py_c::checkNowWolfPowerUp()) {
                plight_colors[i].r = 0;
                plight_colors[i].g = 0;
                plight_colors[i].b = 0;
            }

            J3DLightInfo& light_info = tevstr_p->mLights[i].getLightInfo();

            GXColor light_color;
            light_color.r = plight_colors[i].r;
            light_color.g = plight_colors[i].g;
            light_color.b = plight_colors[i].b;
            light_color.a = plight_colors[i].a;
            light_info.mColor = dKy_light_influence_col(&light_color, tevstr_p->field_0x374);
        }

        dKy_calc_color_set(fog_col_p, &prev_pal_start_p->fog_col, &next_pal_start_p->fog_col,
                           &prev_pal_end_p->fog_col, &next_pal_end_p->fog_col, color_ratio,
                           tevstr_p->pat_ratio, addcol_fog, g_env_light.now_fogcol_ratio);

        *fog_near_p = float_kankyo_color_ratio_set(
            prev_pal_start_p->fog_start_z, prev_pal_end_p->fog_start_z, color_ratio,
            next_pal_start_p->fog_start_z, next_pal_end_p->fog_start_z, tevstr_p->pat_ratio,
            g_env_light.field_0x11ec, g_env_light.field_0x11f4);
        *fog_far_p = float_kankyo_color_ratio_set(
            prev_pal_start_p->fog_end_z, prev_pal_end_p->fog_end_z, color_ratio,
            next_pal_start_p->fog_end_z, next_pal_end_p->fog_end_z, tevstr_p->pat_ratio,
            g_env_light.field_0x11f0, g_env_light.field_0x11f4);

        if (daPy_py_c::checkNowWolfPowerUp()) {
            fog_col_p->r = 0;
            fog_col_p->g = 0;
            fog_col_p->b = 0;
            dKy_WolfPowerup_FogNearFar(fog_near_p, fog_far_p);
        }
    }
}

void dScnKy_env_light_c::setLight_actor(dKy_tevstr_c* tevstr_p, GXColorS10* fog_col_p,
                                        f32* fog_near_p, f32* fog_far_p) {
    u8 prev_pal_start_id;
    u8 next_pal_start_id;
    u8 prev_pal_end_id;
    u8 next_pal_end_id;
    f32 color_ratio;
    int start_pat_pal_id;
    int end_pat_pal_id;
    int i;

    tevstr_p->wether_pat0 = g_env_light.wether_pat0;
    tevstr_p->wether_pat1 = g_env_light.wether_pat1;
    if (tevstr_p->wether_pat0 != tevstr_p->wether_pat1) {
        tevstr_p->pat_ratio = g_env_light.pat_ratio;
    } else if (tevstr_p->PrevCol == tevstr_p->UseCol) {
        tevstr_p->pat_ratio = 0.0f;
    }

    setLight_palno_get(&tevstr_p->PrevCol, &tevstr_p->UseCol, &tevstr_p->wether_pat0,
                       &tevstr_p->wether_pat1, &prev_pal_start_id, &prev_pal_end_id,
                       &next_pal_start_id, &next_pal_end_id, &color_ratio, &start_pat_pal_id,
                       &end_pat_pal_id, &tevstr_p->pat_ratio, &tevstr_p->mInitTimer);

    if (prev_pal_start_id == 0xFF) {
        for (i = 0; i < 4; i++) {
            tevstr_p->AmbCol.r = 255;
            tevstr_p->AmbCol.g = 0;
            tevstr_p->AmbCol.b = 0;
        }
    } else {
        stage_palette_info_class* prev_pal_start_p = &g_env_light.stage_palette_info[prev_pal_start_id];
        stage_palette_info_class* prev_pal_end_p = &g_env_light.stage_palette_info[prev_pal_end_id];
        stage_palette_info_class* next_pal_start_p = &g_env_light.stage_palette_info[next_pal_start_id];
        stage_palette_info_class* next_pal_end_p = &g_env_light.stage_palette_info[next_pal_end_id];

        if (tevstr_p->Type == 10 || tevstr_p->Type == 9 || tevstr_p->Material_id != 0) {
            dKy_calc_color_set(&tevstr_p->AmbCol, &prev_pal_start_p->actor_amb_col,
                               &next_pal_start_p->actor_amb_col, &prev_pal_end_p->actor_amb_col,
                               &next_pal_end_p->actor_amb_col, color_ratio, tevstr_p->pat_ratio,
                               actor_addcol_amb,
                               tevstr_p->field_0x374 *
                                   (g_env_light.now_actcol_ratio * g_env_light.now_actcol_ratio));
        } else {
            dKy_calc_color_set(&tevstr_p->AmbCol, &prev_pal_start_p->actor_amb_col,
                               &next_pal_start_p->actor_amb_col, &prev_pal_end_p->actor_amb_col,
                               &next_pal_end_p->actor_amb_col, color_ratio, tevstr_p->pat_ratio,
                               actor_addcol_amb,
                               tevstr_p->field_0x374 *
                                   (g_env_light.field_0x1210 * (g_env_light.now_actcol_ratio * g_env_light.now_actcol_ratio)));
        }

        if (daPy_py_c::checkNowWolfPowerUp()) {
            dKy_WolfPowerup_AmbCol(&tevstr_p->AmbCol);
        }

        GXColorS10 spC0;
        spC0.r = 0;
        spC0.g = 0;
        spC0.b = 0;
        spC0.a = 0;

        GXColorS10 plight_col[6];
        for (i = 0; i < 6; i++) {
            J3DLightInfo* light_info = &tevstr_p->mLights[i].getLightInfo();

            if (i == 0) {
                if (tevstr_p->Type == 10 || tevstr_p->Type == 9 || tevstr_p->Material_id != 0) {
                    dKy_calc_color_set(&plight_col[i], &prev_pal_start_p->plight_col[i],
                                       &next_pal_start_p->plight_col[i],
                                       &prev_pal_end_p->plight_col[i],
                                       &next_pal_end_p->plight_col[i], color_ratio,
                                       tevstr_p->pat_ratio, actor_addcol_amb, 1.0f);

                    if (daPy_py_c::checkNowWolfPowerUp()) {
                        plight_col[i].r = 0;
                        plight_col[i].g = 0;
                        plight_col[i].b = 0;
                    }

                    light_info = &tevstr_p->mLights[i].getLightInfo();

                    GXColor light_col;
                    light_col.r = plight_col[i].r;
                    light_col.g = plight_col[i].g;
                    light_col.b = plight_col[i].b;
                    light_col.a = plight_col[i].a;
                    light_info->mColor = dKy_light_influence_col(&light_col, tevstr_p->field_0x374);
                } else {
                    dKy_calc_color_set(
                        &plight_col[i], &prev_pal_start_p->plight_col[i],
                        &next_pal_start_p->plight_col[i], &prev_pal_end_p->plight_col[i],
                        &next_pal_end_p->plight_col[i], color_ratio, tevstr_p->pat_ratio,
                        actor_addcol_amb, g_env_light.field_0x1210);

                    if (daPy_py_c::checkNowWolfPowerUp()) {
                        plight_col[i].r = 0;
                        plight_col[i].g = 0;
                        plight_col[i].b = 0;
                    }

                    kankyo_color_ratio_calc(&light_info->mColor, plight_col[i],
                                            field_0x1210 * tevstr_p->field_0x374);
                }
            } else if (tevstr_p->Type == 10 || tevstr_p->Type == 9 || tevstr_p->Material_id != 0) {
                dKy_calc_color_set(&plight_col[i], &prev_pal_start_p->plight_col[i],
                                   &next_pal_start_p->plight_col[i], &prev_pal_end_p->plight_col[i],
                                   &next_pal_end_p->plight_col[i], color_ratio, tevstr_p->pat_ratio,
                                   actor_addcol_amb,
                                   g_env_light.now_actcol_ratio * g_env_light.now_actcol_ratio);

                if (daPy_py_c::checkNowWolfPowerUp()) {
                    plight_col[i].r = 0;
                    plight_col[i].g = 0;
                    plight_col[i].b = 0;
                }

                GXColor light_col;
                light_col.r = plight_col[i].r;
                light_col.g = plight_col[i].g;
                light_col.b = plight_col[i].b;
                light_col.a = plight_col[i].a;
                light_info->mColor = dKy_light_influence_col(&light_col, tevstr_p->field_0x374);
            } else {
                dKy_calc_color_set(&plight_col[i], &prev_pal_start_p->plight_col[i],
                                   &next_pal_start_p->plight_col[i], &prev_pal_end_p->plight_col[i],
                                   &next_pal_end_p->plight_col[i], color_ratio, tevstr_p->pat_ratio,
                                   actor_addcol_amb,
                                   g_env_light.field_0x1210 * (g_env_light.now_actcol_ratio *
                                                               g_env_light.now_actcol_ratio));

                if (daPy_py_c::checkNowWolfPowerUp()) {
                    plight_col[i].r = 0;
                    plight_col[i].g = 0;
                    plight_col[i].b = 0;
                }

                kankyo_color_ratio_calc(&light_info->mColor, plight_col[i],
                                        field_0x1210 * tevstr_p->field_0x374);
            }
        }

        dKy_calc_color_set(fog_col_p, &prev_pal_start_p->fog_col, &next_pal_start_p->fog_col,
                           &prev_pal_end_p->fog_col, &next_pal_end_p->fog_col, color_ratio,
                           tevstr_p->pat_ratio, addcol_fog, g_env_light.now_fogcol_ratio);
        if ((tevstr_p->Type != 10 && tevstr_p->Type != 9) || g_env_light.field_0x1210 == 1.0f) {
            *fog_near_p = float_kankyo_color_ratio_set(
                prev_pal_start_p->fog_start_z, prev_pal_end_p->fog_start_z, color_ratio,
                next_pal_start_p->fog_start_z, next_pal_end_p->fog_start_z, tevstr_p->pat_ratio,
                g_env_light.field_0x11ec, g_env_light.field_0x11f4);
            *fog_far_p = float_kankyo_color_ratio_set(
                prev_pal_start_p->fog_end_z, prev_pal_end_p->fog_end_z, color_ratio,
                next_pal_start_p->fog_end_z, next_pal_end_p->fog_end_z, tevstr_p->pat_ratio,
                g_env_light.field_0x11f0, g_env_light.field_0x11f4);
        } else {
            *fog_near_p = float_kankyo_color_ratio_set(
                prev_pal_start_p->fog_start_z, prev_pal_end_p->fog_start_z, color_ratio,
                next_pal_start_p->fog_start_z, next_pal_end_p->fog_start_z, tevstr_p->pat_ratio,
                g_env_light.field_0x11ec, 0.0f);
            *fog_far_p = float_kankyo_color_ratio_set(
                prev_pal_start_p->fog_end_z, prev_pal_end_p->fog_end_z, color_ratio,
                next_pal_start_p->fog_end_z, next_pal_end_p->fog_end_z, tevstr_p->pat_ratio,
                g_env_light.field_0x11f0, 0.0f);
        }

        if (daPy_py_c::checkNowWolfPowerUp()) {
            int sp58 = 0;
            fog_col_p->r = 0;
            fog_col_p->g = 0;
            fog_col_p->b = 0;
            dKy_WolfPowerup_FogNearFar(fog_near_p, fog_far_p);
        }
    }
}

/* 801A1D64-801A1F58 19C6A4 01F4+00 1/1 0/0 0/0 .text
 * settingTevStruct_colget_actor__18dScnKy_env_light_cFP4cXyzP12dKy_tevstr_cP11_GXColorS10P11_GXColorS10PfPf
 */
void dScnKy_env_light_c::settingTevStruct_colget_actor(cXyz* unused, dKy_tevstr_c* tevstr_p,
                                                       GXColorS10* out_color_p,
                                                       GXColorS10* fog_col_p, f32* fog_near_p,
                                                       f32* fog_far_p) {
    #if DEBUG
    if (tevstr_p->room_no >= 0x80) {
        dDbVw_Report(20, 80, "ObjActor RoomNo ga futei.jyoutyuu?[%d]", tevstr_p->room_no);
        dDbVw_Report(20, 100, "Tevstr wo jimae de kitte init ha arimasuka?[%d]", tevstr_p->room_no);
    }
    #endif

    if (tevstr_p->YukaCol != 0xFF) {
        f32 target = tevstr_p->YukaCol / 100.0f;

        if (g_env_light.mActorLightEffect == 100) {
            cLib_addCalc(&tevstr_p->field_0x374, target, 0.25f, 0.05f, 0.000001f);
        }
    } else if (tevstr_p->room_no >= 0) {
        tevstr_p->UseCol = tevstr_p->room_no;
        if (g_env_light.mActorLightEffect == 100) {
            cLib_addCalc(&tevstr_p->field_0x374, 1.0f, 0.25f, 0.05f, 0.000001f);
        }
    } else {
        #if DEBUG
        dDbVw_Report(10, 40, "Non tevstr->room_no x[%d]y[%d]z[%d]", (int)unused->x, (int)unused->y, (int)unused->z);
        #endif

        tevstr_p->UseCol = 0;
    }

    if ((tevstr_p->Type >= 1 && tevstr_p->Type <= 7) ||
        (tevstr_p->Type == 9 && dKy_darkworld_check()))
    {
        if ((tevstr_p->Type != 2 && tevstr_p->Type != 3) || dKy_darkworld_check()) {
            tevstr_p->field_0x374 = 0.0f;
        }
    }

    if (tevstr_p->PrevCol != tevstr_p->UseCol &&
        (tevstr_p->pat_ratio >= 1.0f || tevstr_p->pat_ratio <= 0.0f))
    {
        tevstr_p->pat_ratio = 0.0f;
    }

    if (tevstr_p->Type != 8) {
        setLight_actor(tevstr_p, fog_col_p, fog_near_p, fog_far_p);
        *out_color_p = tevstr_p->AmbCol;
    } else {
        GXColorS10 BG_col[4];
        setLight_bg(tevstr_p, BG_col, fog_col_p, fog_near_p, fog_far_p);
        *out_color_p = BG_col[0];
    }

    #if DEBUG
    if (g_kankyoHIO.light.m_HOSTIO_setting) {
        if (tevstr_p->UseCol == g_env_light.UseCol && tevstr_p->PrevCol == g_env_light.PrevCol) {
            if (tevstr_p->Type != 8) {
                *out_color_p = actor_amb_col;
            } else {
                *out_color_p = g_env_light.bg_amb_col[0];
            }
        }
    }
    #endif
}

void dScnKy_env_light_c::settingTevStruct_colget_player(dKy_tevstr_c* tevstr_p) {
    if (tevstr_p->YukaCol != 0xFF) {
        f32 target = tevstr_p->YukaCol / 100.0f;

        if (g_env_light.mActorLightEffect == 100) {
            cLib_addCalc(&tevstr_p->field_0x374, target, 0.25f, 0.05f, 0.000001f);
        }
    } else if (tevstr_p->room_no >= 0) {
        tevstr_p->UseCol = tevstr_p->room_no;
        if (g_env_light.mActorLightEffect == 100) {
            cLib_addCalc(&tevstr_p->field_0x374, 1.0f, 0.25f, 0.05f, 0.000001f);
        }
    }

    #if DEBUG
    if (g_kankyoHIO.light.m_displayTVColorSettings == TRUE) {
        dDbVw_Report(228, 80, "RoomColSel");
        dDbVw_Report(228, 95, "YukaCol[%d]", tevstr_p->YukaCol);
        dDbVw_Report(228, 110, "HeyaCol[%d]", tevstr_p->room_no);
        dDbVw_Report(228, 125, "UseCol [%d]", tevstr_p->UseCol);
        dDbVw_Report(228, 140, "wether_pat0 [%d]", g_env_light.wether_pat0);
        dDbVw_Report(228, 155, "wether_pat1 [%d]", g_env_light.wether_pat1);
        dDbVw_Report(228, 170, "pat_ratio   [%f]", g_env_light.pat_ratio);
    }
    #endif

    if (g_env_light.UseCol != tevstr_p->UseCol) {
        if (tevstr_p->UseCol == g_env_light.PrevCol) {
            g_env_light.PrevCol = g_env_light.UseCol;
            g_env_light.UseCol = tevstr_p->UseCol;
            g_env_light.pat_ratio = 1.0f - g_env_light.pat_ratio;

            tevstr_p->pat_ratio = 1.0f - g_env_light.pat_ratio;
        } else if (g_env_light.pat_ratio >= 1.0f || g_env_light.pat_ratio <= 0.0f) {
            g_env_light.UseCol = tevstr_p->UseCol;
            g_env_light.pat_ratio = 0.0f;
            tevstr_p->pat_ratio = 0.0f;
        }
    }
}

static void cLib_addCalcU8(u8* i_value, u8 i_target, s16 i_scale, s16 i_maxStep) {
    s16 step, value;
    value = *i_value;

    step = i_target - value;
    if (abs(step) >= i_scale) {
        step /= i_scale;
    }

    if (step > i_maxStep) {
        value += i_maxStep;
    } else if (step < -i_maxStep) {
        value -= i_maxStep;
    } else {
        value += step;
    }

    *i_value = value;
}

/* 801A2128-801A37C4 19CA68 169C+00 2/1 0/0 0/0 .text
 * settingTevStruct_plightcol_plus__18dScnKy_env_light_cFP4cXyzP12dKy_tevstr_c11_GXColorS1011_GXColorS10Uc
 */
void dScnKy_env_light_c::settingTevStruct_plightcol_plus(cXyz* pos_p, dKy_tevstr_c* tevstr_p,
                                                         GXColorS10 param_2, GXColorS10 param_3,
                                                         u8 init_timer) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    cXyz light_pos;
    Vec spDC;
    MtxP view_mtx = j3dSys.getViewMtx();
    f32 light_dist;
    f32 light_power;
    f32 light_yuragi;
    int sp40 = 0;
    int sp3C = 0;
    int light_inf_id;
    u8 sp9 = 0;
    f32 sp34 = 1.0f;
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (pos_p != NULL) {
        J3DLightInfo* light_info = &tevstr_p->mLightObj.getLightInfo();
        light_info->mColor.a = 0xFF;

        light_inf_id = dKy_light_influence_id(*pos_p, 0);
        BOOL sp44 = 0;

        if (tevstr_p->Type == 7 || tevstr_p->Type == 1 ||
            ((tevstr_p->Type == 2 || tevstr_p->Type == 6 || tevstr_p->Type == 3) &&
             dKy_darkworld_check()) ||
            tevstr_p->Type == 4 || tevstr_p->Type == 5)
        {
            light_inf_id = -2;
        } else if (tevstr_p->Type == 9 && dKy_darkworld_check()) {
            light_inf_id = -2;
        }

        if (light_inf_id >= 0) {
            light_dist = dKy_light_influence_distance(*pos_p, light_inf_id);
            light_power = dKy_light_influence_power(light_inf_id);

            if (light_power < 0.001f) {
                light_power = 0.001f;
            }

            if (light_dist < 1000.0f + light_power) {
                sp44 = 1;
            }
        }

        if (sp44 == 0) {
            light_pos = kankyo->base_light.mPosition;
            light_dist = kankyo->base_light.mPosition.abs(*pos_p);
            light_power = kankyo->base_light.mPow;
            light_yuragi = kankyo->base_light.mFluctuation;

            field_0x10f8.r = 0;
            field_0x10f8.g = 0;
            field_0x10f8.b = 0;

            #if DEBUG
            if (g_kankyoHIO.navy.adjust_light_mode != 0)
            #endif
            {

            if ((light_inf_id <= -2 || (light_inf_id == -1 && dKy_Outdoor_check() == TRUE) ||
                 strcmp(dComIfGp_getStartStageName(), "D_MN09A") == 0))
            {
                cXyz camfwd;
                camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
                J3DLightInfo* light0_info = &tevstr_p->mLights[0].getLightInfo();
                sp9 = 1;

                dKyr_get_vectle_calc(&camera->lookat.center, &camera->lookat.eye, &camfwd);
                light_pos = *pos_p + (camfwd * 500.0f);
                light_pos.y += 40.0f;

                #if DEBUG
                light_pos.y += g_kankyoHIO.navy.adjust_height;
                #endif

                if (tevstr_p->Type >= 1 && tevstr_p->Type <= 9) {
                    dKyr_get_vectle_calc(&camera->lookat.center, &camera->lookat.eye, &camfwd);
                    light_pos = camera->lookat.eye + (camfwd * 180.0f);
                }

                field_0x10f8.r = light0_info->mColor.r;
                field_0x10f8.g = light0_info->mColor.g;
                field_0x10f8.b = light0_info->mColor.b;

                light_yuragi = 0.0f;
                light_power = 120.0f;
                light_dist = 100.0f;
                sp40 = 1;
                sp3C = 1;

                if (strcmp(dComIfGp_getStartStageName(), "D_MN09A") == 0) {
                    if (dComIfG_play_c::getLayerNo(0) == 0) {
                        if (tevstr_p->Type == 0) {
                            field_0x10f8.r = 0x1E;
                            field_0x10f8.g = 0x37;
                            field_0x10f8.b = 0x6E;

                            light_pos.y += -800.0f;
                            light_power = 250.0f;

                            #if DEBUG
                            field_0x10f8.r = g_kankyoHIO.navy.possessed_zelda_light_col.r;
                            field_0x10f8.g = g_kankyoHIO.navy.possessed_zelda_light_col.g;
                            field_0x10f8.b = g_kankyoHIO.navy.possessed_zelda_light_col.b;

                            light_pos.y += g_kankyoHIO.navy.possessed_zelda_light_height;
                            light_power = g_kankyoHIO.navy.possessed_zelda_light_power;
                            #endif
                        }
                    } else if (dComIfG_play_c::getLayerNo(0) == 1 && tevstr_p->Type == 0) {
                        field_0x10f8.r = 0x3C;
                        field_0x10f8.g = 0x5F;
                        field_0x10f8.b = 0x64;
                        light_pos.y += -800.0f;
                        light_power = 150.0f;

                        #if DEBUG
                        field_0x10f8.r = g_kankyoHIO.navy.beast_ganon_light_col.r;
                        field_0x10f8.g = g_kankyoHIO.navy.beast_ganon_light_col.g;
                        field_0x10f8.b = g_kankyoHIO.navy.beast_ganon_light_col.b;

                        light_pos.y += g_kankyoHIO.navy.beast_ganon_light_height;
                        light_power = g_kankyoHIO.navy.beast_ganon_light_power;
                        #endif
                    }
                }

                #if DEBUG
                if (g_kankyoHIO.navy.adjust_light_mode == 2) {
                    field_0x10f8.r = g_kankyoHIO.navy.adjust_custom_R;
                    field_0x10f8.g = g_kankyoHIO.navy.adjust_custom_G;
                    field_0x10f8.b = g_kankyoHIO.navy.adjust_custom_B;
                }
                #endif

                if (tevstr_p != NULL && tevstr_p->mLightInf.a != 0) {
                    field_0x10f8.r = tevstr_p->mLightInf.r * 4;
                    field_0x10f8.g = tevstr_p->mLightInf.g * 4;
                    field_0x10f8.b = tevstr_p->mLightInf.b * 4;
                }

                switch (tevstr_p->Type) {
                case 1:
                    field_0x10f8.r = 500;
                    field_0x10f8.g = 500;
                    field_0x10f8.b = 500;
                    light_dist = 90.0f;
                    light_power = 5000.0f;
                    break;
                case 4:
                    field_0x10f8.r = 500;
                    field_0x10f8.g = 500;
                    field_0x10f8.b = 500;
                    light_dist = 90.0f;
                    light_power = 5000.0f;
                    break;
                case 5:
                    field_0x10f8.r = 170;
                    field_0x10f8.g = 170;
                    field_0x10f8.b = 170;
                    light_dist = 90.0f;
                    light_power = 5000.0f;
                    break;
                case 2:
                    if (dKy_darkworld_check()) {
                        field_0x10f8.r = 103;
                        field_0x10f8.g = 129;
                        field_0x10f8.b = 199;
                        light_dist = 90.0f;
                        light_power = 5000.0f;
                    }
                    break;
                case 3:
                    if (dKy_darkworld_check()) {
                        field_0x10f8.r = 53;
                        field_0x10f8.g = 53;
                        field_0x10f8.b = 80;
                        light_dist = 90.0f;
                        light_power = 5000.0f;
                    }
                    break;
                case 9:
                    if (dKy_darkworld_check()) {
                        field_0x10f8.r = 110;
                        field_0x10f8.g = 110;
                        field_0x10f8.b = 140;
                        light_dist = 90.0f;
                        light_power = 5000.0f;
                    }
                    break;
                case 7: {
                    f32 sp34 = tevstr_p->TevColor.a / 255.0f;
                    field_0x10f8.r = 180;
                    field_0x10f8.g = 180;
                    field_0x10f8.b = 180;
                    light_dist = 90.0f;
                    light_pos.y += -120.0f;

                    f32 sp24 = cM_ssin(g_Counter.mCounter0 * 325);
                    f32 sp20 = cM_scos(g_Counter.mCounter0 * 285);
                    cLib_addCalcU8(&light_info->mColor.a, fabsf(sp24 * 255.0f), 2, 255);
                    light_info->mColor.a *= sp34;

                    light_pos.y += sp20 * 500.0f;
                    light_pos.y -= 80.0f;
                    light_power = 300.0f;

                    #if DEBUG
                    light_pos.y += g_kankyoHIO.navy.adjust_height;
                    #endif
                    break;
                }
                case 6:
                    if (dKy_darkworld_check()) {
                        field_0x10f8.r = 255;
                        field_0x10f8.g = 255;
                        field_0x10f8.b = 255;
                        light_dist = 90.0f;
                        light_power = 5000.0f;
                    }
                    break;
                }
            }

            #if DEBUG
            if (tevstr_p->Type >= 1 && tevstr_p->Type <= 9 && g_kankyoHIO.navy.adjust_light_mode == 2) {
                field_0x10f8.r = (s16)g_kankyoHIO.navy.adjust_light_dif0_col_R;
                field_0x10f8.g = (s16)g_kankyoHIO.navy.adjust_light_dif0_col_G;
                field_0x10f8.b = (s16)g_kankyoHIO.navy.adjust_light_dif0_col_B;
            }
            #endif
            }
        } else {
            light_pos = dKy_light_influence_pos(light_inf_id);
            field_0x10f8 = dKy_light_influence_col(light_inf_id);
            light_yuragi = dKy_light_influence_yuragi(light_inf_id);
        }

        f32 var_f31;
        if (light_power <= 0.0f && sp3C == 0) {
            var_f31 = 1.0f;
        } else {
            var_f31 = light_dist / light_power;
        }

        if (var_f31 > 1.0f) {
            var_f31 = 1.0f;
        }

        var_f31 = (1.0f - var_f31);
        var_f31 *= g_env_light.now_actcol_ratio;

        if ((tevstr_p->Type != 10 && tevstr_p->Type != 9) || tevstr_p->Material_id != 0) {
            field_0x10f0.r = field_0x10f8.r * (var_f31 * kankyo->field_0x1210);
            field_0x10f0.g = field_0x10f8.g * (var_f31 * kankyo->field_0x1210);
            field_0x10f0.b = field_0x10f8.b * (var_f31 * kankyo->field_0x1210);
        } else {
            field_0x10f0.r = field_0x10f8.r * var_f31;
            field_0x10f0.g = field_0x10f8.g * var_f31;
            field_0x10f0.b = field_0x10f8.b * var_f31;
        }

        if (field_0x10f0.r > 0xFF) {
            field_0x10f0.r = 0xFF;
        }

        if (field_0x10f0.g > 0xFF) {
            field_0x10f0.g = 0xFF;
        }

        if (field_0x10f0.b > 0xFF) {
            field_0x10f0.b = 0xFF;
        }

        if (init_timer != 0 || var_f31 <= 0.0f || sp9 == 0) {
            light_info->mColor.r = field_0x10f0.r;
            light_info->mColor.g = field_0x10f0.g;
            light_info->mColor.b = field_0x10f0.b;
        } else {
            cLib_addCalcU8(&light_info->mColor.r, field_0x10f0.r, 10, 0x80);
            cLib_addCalcU8(&light_info->mColor.g, field_0x10f0.g, 10, 0x80);
            cLib_addCalcU8(&light_info->mColor.b, field_0x10f0.b, 10, 0x80);
        }

        field_0x10f0.r = param_2.r + (int)(light_info->mColor.r * (0.1f * var_f31));
        field_0x10f0.g = param_2.g + (int)(light_info->mColor.g * (0.1f * var_f31));
        field_0x10f0.b = param_2.b + (int)(light_info->mColor.b * (0.1f * var_f31));

        if (field_0x10f0.r > 0xFF) {
            field_0x10f0.r = 0xFF;
        }

        if (field_0x10f0.g > 0xFF) {
            field_0x10f0.g = 0xFF;
        }

        if (field_0x10f0.b > 0xFF) {
            field_0x10f0.b = 0xFF;
        }

        if (init_timer != 0 || sp40 != 0 || tevstr_p->field_0x384 != 0) {
            tevstr_p->field_0x32c = light_pos;
        } else {
            f32 sp18 = pos_p->abs(tevstr_p->field_0x32c);
            f32 sp14 = sp18 / 10000.0f;
            if (sp14 > 1.0f) {
                sp14 = 1.0f;
            }
            sp14 *= sp14;

            sp18 = pos_p->abs(light_pos);
            f32 var_f28 = sp18 / 5000.0f;
            if (var_f28 > 1.0f) {
                var_f28 = 1.0f;
            }

            var_f28 = 1.0f - var_f28;
            var_f28 *= var_f28 * var_f28;
            f32 sp1C = 10.0f + (10000.0f * sp14) + (100.0f * var_f28);
            cLib_addCalc(&tevstr_p->field_0x32c.x, light_pos.x, 0.5f, sp1C, 0.001f);
            cLib_addCalc(&tevstr_p->field_0x32c.y, light_pos.y, 0.5f, sp1C, 0.001f);
            cLib_addCalc(&tevstr_p->field_0x32c.z, light_pos.z, 0.5f, sp1C, 0.001f);
        }

        cXyz spC4;
        cXyz spB8;
        spB8 = tevstr_p->field_0x32c;
        cMtx_multVec(view_mtx, &spB8, &spDC);

        if (dKy_shadow_mode_check(4) == 0) {
            if ((tevstr_p->Type == 9 || tevstr_p->Type == 10) &&
                (player->getSinkShapeOffset() < -20.0f || player->checkSinkDead() || dKy_camera_water_in_status_check() || !player->checkSwimUp()))
            {
                cLib_addCalc(&tevstr_p->mLightPosWorld.x, pos_p->x, 0.5f, 5000.0f, 0.1f);
                cLib_addCalc(&tevstr_p->mLightPosWorld.y, 1000.0f + pos_p->y, 0.5f, 5000.0f, 0.1f);
                cLib_addCalc(&tevstr_p->mLightPosWorld.z, pos_p->z, 0.5f, 5000.0f, 0.1f);
            } else if ((kankyo->shadow_mode & 1) || (kankyo->shadow_mode & 2)) {
                cLib_addCalc(&tevstr_p->mLightPosWorld.x, g_env_light.field_0x10a0.x, 0.5f, 5000.0f,
                             0.1f);
                cLib_addCalc(&tevstr_p->mLightPosWorld.y, g_env_light.field_0x10a0.y, 0.5f, 5000.0f,
                             0.1f);
                cLib_addCalc(&tevstr_p->mLightPosWorld.z, g_env_light.field_0x10a0.z, 0.5f, 5000.0f,
                             0.1f);
            } else if (sp9 != 0) {
                if (init_timer != 0 || tevstr_p->field_0x384 != 0) {
                    tevstr_p->mLightPosWorld = kankyo->base_light.mPosition;
                } else {
                    cLib_addCalc(&tevstr_p->mLightPosWorld.x, kankyo->base_light.mPosition.x, 0.25f,
                                 1000.0f, 0.0001f);
                    cLib_addCalc(&tevstr_p->mLightPosWorld.y, kankyo->base_light.mPosition.y, 0.25f,
                                 1000.0f, 0.0001f);
                    cLib_addCalc(&tevstr_p->mLightPosWorld.z, kankyo->base_light.mPosition.z, 0.25f,
                                 1000.0f, 0.0001f);
                }
            } else {
                cXyz spAC;
                u8 sp8 = 0;
                spAC = tevstr_p->field_0x32c;

                for (int i = 0; i < 6; i++) {
                    if (kankyo->field_0x0c18[i].field_0x26 == 1) {
                        if (kankyo->field_0x0c18[i].mPos.abs(*pos_p) < spAC.abs(*pos_p) &&
                            kankyo->field_0x0c18[i].mPos.abs(*pos_p) < 1000.0f)
                        {
                            spAC = kankyo->field_0x0c18[i].mPos;
                            sp8 = i + 1;
                        }
                    }
                }

                if (sp8 == 0) {
                    if (init_timer != 0 || tevstr_p->field_0x384 != 0) {
                        tevstr_p->mLightPosWorld = tevstr_p->field_0x32c;
                    } else {
                        cLib_addCalc(&tevstr_p->mLightPosWorld.x, tevstr_p->field_0x32c.x, 0.25f,
                                     1000.0f, 0.0001f);
                        cLib_addCalc(&tevstr_p->mLightPosWorld.y, tevstr_p->field_0x32c.y, 0.25f,
                                     1000.0f, 0.0001f);
                        cLib_addCalc(&tevstr_p->mLightPosWorld.z, tevstr_p->field_0x32c.z, 0.25f,
                                     1000.0f, 0.0001f);
                    }
                } else {
                    if (init_timer != 0 || tevstr_p->field_0x384 != 0) {
                        tevstr_p->mLightPosWorld = kankyo->field_0x0c18[sp8 - 1].mPos;
                    } else {
                        cLib_addCalc(&tevstr_p->mLightPosWorld.x,
                                     kankyo->field_0x0c18[sp8 - 1].mPos.x, 0.25f, 1000.0f, 0.0001f);
                        cLib_addCalc(&tevstr_p->mLightPosWorld.y,
                                     kankyo->field_0x0c18[sp8 - 1].mPos.y, 0.25f, 1000.0f, 0.0001f);
                        cLib_addCalc(&tevstr_p->mLightPosWorld.z,
                                     kankyo->field_0x0c18[sp8 - 1].mPos.z, 0.25f, 1000.0f, 0.0001f);
                    }

                    var_f31 = kankyo->field_0x0c18[sp8 - 1].mPos.abs(*pos_p) / 500.0f;
                    if (var_f31 > 1.0f) {
                        var_f31 = 1.0f;
                    }
                    var_f31 = 1.0f - var_f31;
                }
            }

            if ((tevstr_p->Type == 9 || tevstr_p->Type == 10) &&
                (player->checkSinkDead() || dComIfGp_checkPlayerStatus0(0, 0x02000308) ||
                 dComIfGp_checkPlayerStatus1(0, 0x02000000) || player->checkMagneBootsOn()))
            {
                cLib_addCalc(&tevstr_p->field_0x344, 0.0f, 0.75f, 0.21f, 0.0001f);
            } else if (tevstr_p->Type == 9 && player->getSinkShapeOffset() < -35.0f) {
                cLib_addCalc(&tevstr_p->field_0x344, 0.0f, 0.1f, 0.01f, 0.00001f);
            } else if (tevstr_p->Type == 10 && player->getSinkShapeOffset() < -100.0f) {
                cLib_addCalc(&tevstr_p->field_0x344, 0.0f, 0.1f, 0.01f, 0.00001f);
            } else if (init_timer != 0) {
                tevstr_p->field_0x344 = kankyo->field_0x1238 +
                                        (var_f31 * (kankyo->field_0x123c - kankyo->field_0x1238));
            } else {
                cLib_addCalc(&tevstr_p->field_0x344,
                             kankyo->field_0x1238 +
                                 (var_f31 * (kankyo->field_0x123c - kankyo->field_0x1238)),
                             0.75f, 0.04f, 0.000001f);
            }

            if (tevstr_p->field_0x344 > 1.0f) {
                tevstr_p->field_0x344 = 1.0f;
            }
        }

        light_info->mLightPosition = spDC;
        light_info->mLightDirection = g_env_light.field_0x1064;

        light_info->mCosAtten.x = 1.0f;
        light_info->mCosAtten.y = 0.0f;
        light_info->mCosAtten.z = 0.0f;

        light_info->mDistAtten.x = 1.0f;
        light_info->mDistAtten.y = 0.0f;
        light_info->mDistAtten.z = 0.0f;
    }
}

void dScnKy_env_light_c::settingTevStruct(int tevstrType, cXyz* pos_p, dKy_tevstr_c* tevstr_p) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    int sp58 = 0;
    int sp54 = -1;

    u8 init_timer = tevstr_p->mInitTimer;
    u8 spA = 0;

    GXColorS10 amb_col;
    GXColorS10 fog_tev_col;

    f32 fog_near;
    f32 fog_far;

    cXyz pos;
    if (tevstr_p->room_no < 0) {
        tevstr_p->room_no = dComIfGp_roomControl_getStayNo();
    }

    if (pos_p != NULL) {
        pos = *pos_p;
    } else {
        pos.set(0.0f, 0.0f, 0.0f);
    }

    #if DEBUG
    if (g_kankyoHIO.no_setting_tevstr) {
        return;
    }
    #endif

    if (tevstr_p != NULL && g_env_light.mActorLightEffect != 100) {
        tevstr_p->field_0x374 = g_env_light.mActorLightEffect / 100.0f;
    }

    tevstr_p->Type = tevstrType;

    if (tevstr_p->Material_use_fg != 123 && tevstr_p->Material_use_fg != 124) {
        dKy_tevstr_init(tevstr_p, dComIfGp_roomControl_getStayNo(), 0xFF);
        OS_REPORT_ERROR("\n\ntevstr init Non. Material_use_fg[%d] setroom=[%d]\n",
                        tevstr_p->Material_use_fg, (s8)dComIfGp_roomControl_getStayNo());
    }

    tevstr_p->Material_use_fg = 124;
    actor_amb_col.a = 255;

    if (tevstrType == 14) {
        MtxPtr sp50 = j3dSys.getViewMtx();
        camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
        cXyz calc_pos;
        tevstr_p->mLightMode = 0;

        // @bug room_no is a signed char, so this will always evaluate to false
        if (tevstr_p->room_no >= 128) {
            tevstr_p->UseCol = 0;
        } else {
            tevstr_p->UseCol = tevstr_p->room_no;
        }

        if (!dKy_darkworld_check()) {
            field_0x10f0.r = 24;
            field_0x10f0.g = 24;
            field_0x10f0.b = 24;
            field_0x10f0.a = 255;
        } else {
            field_0x10f0.r = 55;
            field_0x10f0.g = 55;
            field_0x10f0.b = 77;
        }

        #if DEBUG
        if (g_kankyoHIO.navy.adjust_light_mode == 2) {
            field_0x10f0.r = g_kankyoHIO.navy.adjust_light_ambcol.r;
            field_0x10f0.g = g_kankyoHIO.navy.adjust_light_ambcol.g;
            field_0x10f0.b = g_kankyoHIO.navy.adjust_light_ambcol.b;
            field_0x10f0.a = 255;
        }
        #endif

        fog_tev_col.r = 255;
        fog_tev_col.g = 255;
        fog_tev_col.b = 255;

        fog_near = 30000.0f;
        fog_far = 30000.0f;
        dKyr_get_vectle_calc(&pos, &camera_p->lookat.eye, &calc_pos);

        for (int i = 0; i < 6; i++) {
            J3DLightInfo& light_info = tevstr_p->mLights[i].getLightInfo();

            if (i == 0) {
                if (!dKy_darkworld_check()) {
                    light_info.mColor.r = 126;
                    light_info.mColor.g = 110;
                    light_info.mColor.b = 89;
                } else {
                    light_info.mColor.r = 0;
                    light_info.mColor.g = 0;
                    light_info.mColor.b = 0;
                }

                #if DEBUG
                if (g_kankyoHIO.navy.adjust_light_mode == 2) {
                    light_info.mColor.r = g_kankyoHIO.navy.adjust_light_dif0_col_R;
                    light_info.mColor.g = g_kankyoHIO.navy.adjust_light_dif0_col_G;
                    light_info.mColor.b = g_kankyoHIO.navy.adjust_light_dif0_col_B;
                }
                #endif
            } else if (i == 1) {
                if (!dKy_darkworld_check()) {
                    light_info.mColor.r = 24;
                    light_info.mColor.g = 41;
                    light_info.mColor.b = 50;
                } else {
                    light_info.mColor.r = 0;
                    light_info.mColor.g = 0;
                    light_info.mColor.b = 0;
                }

                #if DEBUG
                if (g_kankyoHIO.navy.adjust_light_mode == 2) {
                    light_info.mColor.r = g_kankyoHIO.navy.adjust_light_dif1_col.r;
                    light_info.mColor.g = g_kankyoHIO.navy.adjust_light_dif1_col.g;
                    light_info.mColor.b = g_kankyoHIO.navy.adjust_light_dif1_col.b;
                }
                #endif
            } else {
                light_info.mColor.r = 0;
                light_info.mColor.g = 0;
                light_info.mColor.b = 0;
            }

            dKy_GXInitLightSpot(&light_info, 90.0f, GX_SP_OFF);
            f32 var_f31 = 100000.0f;
            dKy_GXInitLightDistAttn(&light_info, var_f31, 0.99999f, GX_DA_STEEP);

            light_info.mLightPosition.x = 0.0f;
            light_info.mLightPosition.y = 0.0f;
            light_info.mLightPosition.z = 0.0f;

            if (i == 0) {
                light_info.mLightPosition.x = 500.0f;
                light_info.mLightPosition.y = 500.0f;
                light_info.mLightPosition.z = 500.0f;

                #if DEBUG
                light_info.mLightPosition.x = g_kankyoHIO.navy.adjust_light_main_pos.x;
                light_info.mLightPosition.y = g_kankyoHIO.navy.adjust_light_main_pos.y;
                light_info.mLightPosition.z = g_kankyoHIO.navy.adjust_light_main_pos.z;
                #endif
            } else {
                light_info.mLightPosition.x = -500.0f;
                light_info.mLightPosition.y = -500.0f;
                light_info.mLightPosition.z = -500.0f;

                #if DEBUG
                light_info.mLightPosition.x = -g_kankyoHIO.navy.adjust_light_main_pos.x;
                light_info.mLightPosition.y = -g_kankyoHIO.navy.adjust_light_main_pos.y;
                light_info.mLightPosition.z = -g_kankyoHIO.navy.adjust_light_main_pos.z;
                #endif
            }

            dKy_lightdir_set(0.0f, 0.0f, &light_info.mLightDirection);

            light_info.mLightDirection.x = -light_info.mLightDirection.x;
            light_info.mLightDirection.y = -light_info.mLightDirection.y;
            light_info.mLightDirection.z = -light_info.mLightDirection.z;
        }

        MtxP view_mtx = j3dSys.getViewMtx();
        Vec sp8C;
        Vec sp80;

        sp80.x = camera_p->lookat.eye.x;
        sp80.y = camera_p->lookat.eye.y;
        sp80.z = camera_p->lookat.eye.z;

        J3DLightInfo& light_info = tevstr_p->mLightObj.getLightInfo();
        cMtx_multVec(view_mtx, &sp80, &sp8C);

        light_info.mLightPosition = sp8C;
        tevstr_p->field_0x32c = sp80;
        tevstr_p->mLightPosWorld = sp80;
        light_info.mLightDirection = g_env_light.field_0x1064;

        light_info.mColor.r = 0;
        light_info.mColor.g = 0;
        light_info.mColor.b = 0;

        light_info.mCosAtten.x = 1.0f;
        light_info.mCosAtten.y = 0.0f;
        light_info.mCosAtten.z = 0.0f;

        light_info.mDistAtten.x = 1.0f;
        light_info.mDistAtten.y = 0.0f;
        light_info.mDistAtten.z = 0.0f;
    } else if (tevstrType == 12 || tevstrType == 13) {
        MtxP sp3C = j3dSys.getViewMtx();
        camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
        cXyz spD8;

        tevstr_p->TevColor.r = 0;
        tevstr_p->TevColor.g = 0;
        tevstr_p->TevColor.b = 0;
        tevstr_p->TevColor.a = 0;

        tevstr_p->TevKColor.r = 0;
        tevstr_p->TevKColor.g = 0;
        tevstr_p->TevKColor.b = 0;
        tevstr_p->TevKColor.a = 0;
        tevstr_p->mLightMode = 0;

        if (tevstr_p->room_no >= 128) {
            tevstr_p->UseCol = 0;
        } else {
            tevstr_p->UseCol = tevstr_p->room_no;
        }

        if (tevstrType == 12) {
            field_0x10f0.r = 25;
            field_0x10f0.g = 20;
            field_0x10f0.b = 25;
        } else {
            field_0x10f0.r = 40;
            field_0x10f0.g = 35;
            field_0x10f0.b = 30;
        }

        field_0x10f0.a = 0xFF;

        #if DEBUG
        if (g_kankyoHIO.navy.light_adjust_ON) {
            field_0x10f0.r = g_kankyoHIO.navy.adjust_light_ambcol.r;
            field_0x10f0.g = g_kankyoHIO.navy.adjust_light_ambcol.g;
            field_0x10f0.b = g_kankyoHIO.navy.adjust_light_ambcol.b;
            field_0x10f0.a = 255;
        }
        #endif

        fog_tev_col.r = 255;
        fog_tev_col.g = 255;
        fog_tev_col.b = 255;

        fog_near = 30000.0f;
        fog_far = 30000.0f;

        for (int i = 0; i < 6; i++) {
            camera_class* sp30 = dComIfGp_getCamera(0);
            J3DLightInfo& temp_r31 = tevstr_p->mLights[i].getLightInfo();

            if (i == 0) {
                if (tevstrType == 12) {
                    temp_r31.mLightPosition.x = -30000.0f;
                    temp_r31.mLightPosition.y = 18800.0f;
                    temp_r31.mLightPosition.z = 29000.0f;
                    temp_r31.mColor.r = 120;
                    temp_r31.mColor.g = 110;
                    temp_r31.mColor.b = 100;
                } else {
                    temp_r31.mLightPosition.x = -37000.0f;
                    temp_r31.mLightPosition.y = 18800.0f;
                    temp_r31.mLightPosition.z = 500.0f;
                    temp_r31.mColor.r = 85;
                    temp_r31.mColor.g = 90;
                    temp_r31.mColor.b = 100;
                }

                #if DEBUG
                if (g_kankyoHIO.navy.light_adjust_ON) {
                    temp_r31.mColor.r = g_kankyoHIO.navy.adjust_light_dif0_col_R;
                    temp_r31.mColor.g = g_kankyoHIO.navy.adjust_light_dif0_col_G;
                    temp_r31.mColor.b = g_kankyoHIO.navy.adjust_light_dif0_col_B;
                    temp_r31.mLightPosition.x = g_kankyoHIO.navy.adjust_light_main_pos.x;
                    temp_r31.mLightPosition.y = g_kankyoHIO.navy.adjust_light_main_pos.y;
                    temp_r31.mLightPosition.z = g_kankyoHIO.navy.adjust_light_main_pos.z;
                }
                #endif
            } else if (i == 1) {
                if (tevstrType == 12) {
                    temp_r31.mLightPosition.x = 14400.0f;
                    temp_r31.mLightPosition.y = 7500.0f;
                    temp_r31.mLightPosition.z = 3900.0f;
                    temp_r31.mColor.r = 30;
                    temp_r31.mColor.g = 45;
                    temp_r31.mColor.b = 30;
                } else {
                    temp_r31.mLightPosition.x = 18000.0f;
                    temp_r31.mLightPosition.y = -6500.0f;
                    temp_r31.mLightPosition.z = -10000.0f;
                    temp_r31.mColor.r = 100;
                    temp_r31.mColor.g = 65;
                    temp_r31.mColor.b = 40;
                }

                #if DEBUG
                if (g_kankyoHIO.navy.light_adjust_ON) {
                    temp_r31.mColor.r = g_kankyoHIO.navy.adjust_light_dif1_col.r & 0xFF;
                    temp_r31.mColor.g = g_kankyoHIO.navy.adjust_light_dif1_col.g & 0xFF;
                    temp_r31.mColor.b = g_kankyoHIO.navy.adjust_light_dif1_col.b & 0xFF;
                    temp_r31.mLightPosition.x = g_kankyoHIO.navy.collect_light_reflect_pos.x;
                    temp_r31.mLightPosition.y = g_kankyoHIO.navy.collect_light_reflect_pos.y;
                    temp_r31.mLightPosition.z = g_kankyoHIO.navy.collect_light_reflect_pos.z;
                }
                #endif
            } else {
                temp_r31.mColor.r = 0;
                temp_r31.mColor.g = 0;
                temp_r31.mColor.b = 0;
            }

            dKy_GXInitLightSpot(&temp_r31, 90.0f, 0);
            f32 var_f30 = 100000.0f;
            dKy_GXInitLightDistAttn(&temp_r31, var_f30, 0.99999f, 3);
            dKy_lightdir_set(0.0f, 0.0f, &temp_r31.mLightDirection);

            temp_r31.mLightDirection.x = -temp_r31.mLightDirection.x;
            temp_r31.mLightDirection.y = -temp_r31.mLightDirection.y;
            temp_r31.mLightDirection.z = -temp_r31.mLightDirection.z;
        }

        MtxP view_mtx = j3dSys.getViewMtx();
        Vec sp74;
        Vec sp68;

        sp68.x = camera_p->lookat.eye.x;
        sp68.y = camera_p->lookat.eye.y;
        sp68.z = camera_p->lookat.eye.z;

        J3DLightInfo& light_info = tevstr_p->mLightObj.getLightInfo();
        cMtx_multVec(view_mtx, &sp68, &sp74);

        light_info.mLightPosition = sp74;
        if (tevstrType == 13) {
            tevstr_p->field_0x32c = sp68;
            tevstr_p->mLightPosWorld = sp68;
        }

        light_info.mLightDirection = g_env_light.field_0x1064;

        light_info.mColor.r = 0;
        light_info.mColor.g = 0;
        light_info.mColor.b = 0;

        light_info.mCosAtten.x = 1.0f;
        light_info.mCosAtten.y = 0.0f;
        light_info.mCosAtten.z = 0.0f;

        light_info.mDistAtten.x = 1.0f;
        light_info.mDistAtten.y = 0.0f;
        light_info.mDistAtten.z = 0.0f;
    } else if (!(tevstrType & 0xF0)) {
        tevstr_p->mLightMode = 1;

        amb_col = actor_amb_col;
        fog_tev_col = fog_col;

        fog_near = mFogNear;
        fog_far = mFogFar;

        if (tevstrType == 0 || tevstrType == 0 || tevstrType == 8 || tevstrType == 7 ||
            tevstrType == 1 || tevstrType == 2 || tevstrType == 3 || tevstrType == 5 ||
            tevstrType == 4 || tevstrType == 11)
        {
            settingTevStruct_colget_actor(pos_p, tevstr_p, &amb_col, &fog_tev_col, &fog_near,
                                          &fog_far);
        } else if (tevstrType == 10 || tevstrType == 9) {
            if (g_env_light.player_room_no != (u8)dComIfGp_roomControl_getStayNo()) {
                g_env_light.player_room_no = dComIfGp_roomControl_getStayNo();
                mDoAud_setSceneName(
                    dComIfGp_getStartStageName(), dComIfGp_roomControl_getStayNo(),
                    dComIfG_play_c::getLayerNo_common(dComIfGp_getStartStageName(),
                                                      dComIfGp_roomControl_getStayNo(),
                                                      dComIfGp_getStartStageLayer()));

                if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0) {
                    mDoAud_load2ndDynamicWave();
                }
            }
            init_timer = g_env_light.light_init_timer;
            settingTevStruct_colget_player(tevstr_p);
            settingTevStruct_colget_actor(pos_p, tevstr_p, &amb_col, &fog_tev_col, &fog_near,
                                          &fog_far);
        }

        field_0x10f0.r = (s16)amb_col.r;
        field_0x10f0.g = (s16)amb_col.g;
        field_0x10f0.b = (s16)amb_col.b;
        field_0x10f0.a = 255;

        if (tevstrType != 11) {
            settingTevStruct_plightcol_plus(pos_p, tevstr_p, amb_col, amb_col, init_timer);
        }

        if (tevstrType == 10 || tevstrType == 9) {
            g_env_light.plight_near_pos = tevstr_p->mLightPosWorld;
        }
    } else {
        tevstr_p->mLightMode = 0;

        #if DEBUG
        if (tevstr_p->room_no >= 128) {
            dDbVw_Report(20, 90, "BgActor RoomNo ga futei.jyoutyuu?[%d]", tevstr_p->room_no);
            dDbVw_Report(20, 100, "Tevstr wo jimae de kitte init ha arimasuka?[%d]", tevstr_p->room_no);
        }
        #endif

        if (tevstr_p->Type != 20) {
            tevstr_p->field_0x374 = g_env_light.bg_light_influence;
        } else {
            u8 sp9 = tevstr_p->mLightInf.r;

            #if DEBUG
            if (g_kankyoHIO.navy.door_light_influence_ratio != 0xFF) {
                sp9 = g_kankyoHIO.navy.door_light_influence_ratio;
            }
            #endif

            switch (sp9) {
            case 0:
                tevstr_p->field_0x374 = 0.2f;
                break;
            case 1:
                tevstr_p->field_0x374 = 0.3f;
                break;
            case 2:
                tevstr_p->field_0x374 = 0.4f;
                break;
            case 3:
                tevstr_p->field_0x374 = 0.6f;
                break;
            case 4:
                tevstr_p->field_0x374 = 0.8f;
                break;
            case 5:
                tevstr_p->field_0x374 = 0.9f;
                break;
            case 6:
                tevstr_p->field_0x374 = 1.0f;
                break;
            case 7:
                tevstr_p->field_0x374 = 1.2f;
                break;
            #if DEBUG
            case 0x99:
                tevstr_p->field_0x374 = g_kankyoHIO.navy.unk_light_influence_ratio / 100.0f;
                break;
            #endif
            default:
                tevstr_p->field_0x374 = 1.0f;
                break;
            }
        }

        // @bug room_no is a signed char, so this will always evaluate to false
        if (tevstr_p->room_no >= 128) {
            tevstr_p->UseCol = 0;
        } else {
            tevstr_p->UseCol = tevstr_p->room_no;
        }

        GXColorS10 BG_col[4];
        setLight_bg(tevstr_p, BG_col, &fog_tev_col, &fog_near, &fog_far);

        sp54 = tevstrType & 3;
        field_0x10f0 = BG_col[sp54];

        MtxP view_mtx = j3dSys.getViewMtx();
        Vec sp5C;
        cXyz pos;
        bool var_r25_3 = 0;
        pos.x = pos.y = pos.z = 0.0f;

        int light_inf_id = dKy_light_influence_id(pos, 0);
        if (light_inf_id >= 0 && g_env_light.pointlight[light_inf_id]->mIndex < 0) {
            var_r25_3 = 1;
        }

        if (var_r25_3 == 1) {
            pos = g_env_light.pointlight[light_inf_id]->mPosition;
            J3DLightInfo& light_info = tevstr_p->mLightObj.getLightInfo();

            cMtx_multVec(view_mtx, &pos, &sp5C);

            light_info.mLightPosition = sp5C;
            tevstr_p->field_0x32c = pos;
            light_info.mLightDirection = g_env_light.field_0x1064;

            light_info.mColor.a = g_env_light.pointlight[light_inf_id]->mColor.a;
            light_info.mColor.r = g_env_light.pointlight[light_inf_id]->mColor.r;
            light_info.mColor.g = g_env_light.pointlight[light_inf_id]->mColor.g;
            light_info.mColor.b = g_env_light.pointlight[light_inf_id]->mColor.b;
            dKy_GXInitLightSpot(&light_info, 90.0f, GX_SP_OFF);
            dKy_GXInitLightDistAttn(&light_info,
                                    g_env_light.pointlight[light_inf_id]->mPow * 0.001f, 0.99999f,
                                    GX_DA_STEEP);
        } else {
            MtxP view_mtx = j3dSys.getViewMtx();
            J3DLightInfo& light_info = tevstr_p->mLightObj.getLightInfo();
            Vec sp44;
            cMtx_multVec(view_mtx, &lightStatusData[0].position, &sp44);

            light_info.mLightPosition = sp44;
            tevstr_p->field_0x32c = lightStatusData[0].position;
            light_info.mLightDirection = g_env_light.field_0x1064;

            light_info.mColor.r = 0;
            light_info.mColor.g = 0;
            light_info.mColor.b = 0;

            light_info.mCosAtten.x = 0.0f;
            light_info.mCosAtten.y = 0.0f;
            light_info.mCosAtten.z = 0.0f;

            light_info.mDistAtten.x = 0.0f;
            light_info.mDistAtten.y = 0.0f;
            light_info.mDistAtten.z = 0.0f;
        }

        tevstr_p->mLightPosWorld = kankyo->base_light.mPosition;

        #if DEBUG
        if (g_kankyoHIO.light.m_HOSTIO_setting) {
            if (tevstr_p->UseCol == g_env_light.UseCol && tevstr_p->PrevCol == g_env_light.PrevCol) {
                field_0x10f0 = bg_amb_col[sp54];
            }
        }
        #endif
    }

    #if DEBUG
    if (g_kankyoHIO.light.m_HOSTIO_setting) {
        if (tevstr_p->UseCol == g_env_light.UseCol && tevstr_p->PrevCol == g_env_light.PrevCol) {
            fog_tev_col = fog_col;
            fog_near = mFogNear;
            fog_far = mFogFar;
        }
    }
    #endif

    field_0x10f0.a = 255;
    tevstr_p->AmbCol = field_0x10f0;
    tevstr_p->FogCol = fog_tev_col;
    tevstr_p->mFogStartZ = fog_near;
    tevstr_p->mFogEndZ = fog_far;
}

void dScnKy_env_light_c::setLightTevColorType(J3DModelData*, dKy_tevstr_c*) {}

static void setLightTevColorType_MAJI_sub(J3DMaterial* material_p, dKy_tevstr_c* tevstr_p,
                                          int lightType) {
    int i;
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    MtxP view_mtx = j3dSys.getViewMtx();
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    if (tevstr_p != NULL) {
        GXColor amb_col;
        amb_col.r = tevstr_p->AmbCol.r;
        amb_col.g = tevstr_p->AmbCol.g;
        amb_col.b = tevstr_p->AmbCol.b;
        amb_col.a = tevstr_p->AmbCol.a;

        if (((tevstr_p->Type >= 1 && tevstr_p->Type <= 7) || tevstr_p->Type == 5 ||
             tevstr_p->Type == 15 || (tevstr_p->Type == 9 && dKy_darkworld_check())) &&
            ((tevstr_p->Type != 2 && tevstr_p->Type != 3) || dKy_darkworld_check()))
        {
            amb_col.r = 0;
            amb_col.g = 0;
            amb_col.b = 0;
            amb_col.a = 0;

            if (tevstr_p->Type == 7) {
                amb_col.a = 4;
            }

            if (tevstr_p->Type == 3) {
                amb_col.r = 12;
                amb_col.g = 12;
                amb_col.b = 12;
            } else if (tevstr_p->Type == 6) {
                f32 var_r30 = fabsf(cM_ssin(g_Counter.mCounter0 * 662));
                amb_col.r = 255.0f - (185.0f * var_r30);
                amb_col.g = 255.0f - (185.0f * var_r30);
                amb_col.b = 255.0f - (185.0f * var_r30);
            } else if (tevstr_p->Type == 2) {
                amb_col.r = 0x12;
                amb_col.g = 0x12;
                amb_col.b = 0x12;
            } else if (tevstr_p->Type == 4) {
                amb_col.r = 0xFF;
                amb_col.g = 0x33;
                amb_col.b = 0xB;
            } else if (tevstr_p->Type == 5) {
                amb_col.r = 0xA;
                amb_col.g = 0xA;
                amb_col.b = 8;
            } else if (tevstr_p->Type == 15) {
                amb_col.r = 0x19;
                amb_col.g = 0x1E;
                amb_col.b = 0x23;
            }

            #if DEBUG
            if ((tevstr_p->Type == 4 || tevstr_p->Type == 2 || tevstr_p->Type == 1 || tevstr_p->Type == 7 ||
                tevstr_p->Type == 6 || tevstr_p->Type == 5 || tevstr_p->Type == 15 || (tevstr_p->Type == 9 && dKy_darkworld_check())))
            {
                if (g_kankyoHIO.navy.adjust_light_mode == 2) {
                    amb_col.r = g_kankyoHIO.navy.adjust_light_ambcol.r & 0xFF;
                    amb_col.g = g_kankyoHIO.navy.adjust_light_ambcol.g & 0xFF;
                    amb_col.b = g_kankyoHIO.navy.adjust_light_ambcol.b & 0xFF;
                    amb_col.a = 0;
                }
            }
            #endif
        }

        material_p->setAmbColor(0, (J3DGXColor*)&amb_col);

        if (lightType != 0) {
            material_p->setTevColor(0, (J3DGXColorS10*)&tevstr_p->TevColor);
            material_p->setTevKColor(0, (J3DGXColor*)&tevstr_p->TevKColor);
        }

        MtxP sp10 = j3dSys.getViewMtx();
        if (sp10 != NULL) {
            Vec sp28;
            J3DLightInfo* sp20 = &tevstr_p->mLightObj.getLightInfo();
            cMtx_multVec(sp10, &tevstr_p->field_0x32c, &sp28);
            sp20->mLightPosition = sp28;
        }

        material_p->setLight(0, &tevstr_p->mLightObj);

        if (view_mtx != NULL) {
            if (lightType != 2) {
                for (i = 0; i < 6; i++) {
                    material_p->setLight(i + 2, &tevstr_p->mLights[i]);
                }
            } else if (g_env_light.fishing_hole_season >= 1 && g_env_light.fishing_hole_season <= 4)
            {
                color_RGB_class maple_color;
                maple_color.r = dKyd_maple_col_getp()[g_env_light.fishing_hole_season + 7].r;
                maple_color.g = dKyd_maple_col_getp()[g_env_light.fishing_hole_season + 7].g;
                maple_color.b = dKyd_maple_col_getp()[g_env_light.fishing_hole_season + 7].b;

                #if DEBUG
                if (g_kankyoHIO.navy.fish_pond_tree_adjust_ON) {
                    maple_color.r = g_kankyoHIO.navy.fish_pond_tree_ambcol.r;
                    maple_color.g = g_kankyoHIO.navy.fish_pond_tree_ambcol.g;
                    maple_color.b = g_kankyoHIO.navy.fish_pond_tree_ambcol.b;
                }
                #endif

                amb_col.r = 0xA;
                amb_col.g = 0xA;
                amb_col.b = 0xA;

                f32 var_f31;
                var_f31 = (amb_col.r * g_env_light.now_bgcol_ratio) / 10.0f;
                var_f31 *= var_f31;
                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }
                amb_col.r = g_env_light.now_bgcol_ratio * (maple_color.r * var_f31);

                var_f31 = (amb_col.g * g_env_light.now_bgcol_ratio) / 10.0f;
                var_f31 *= var_f31;
                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }
                amb_col.g = g_env_light.now_bgcol_ratio * (maple_color.g * var_f31);

                var_f31 = (amb_col.b * g_env_light.now_bgcol_ratio) / 10.0f;
                var_f31 *= var_f31;
                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }
                amb_col.b = g_env_light.now_bgcol_ratio * (maple_color.b * var_f31);

                material_p->setAmbColor(0, (J3DGXColor*)&amb_col);

                for (i = 0; i < 6; i++) {
                    if (i <= 1) {
                        J3DLightInfo* var_r28;
                        if (i == 0) {
                            kankyo->global_maple_col_change[i].light_obj = tevstr_p->mLights[i];

                            J3DLightInfo* sp8 =
                                &kankyo->global_maple_col_change[1].light_obj.getLightInfo();
                            var_r28 = &kankyo->global_maple_col_change[i].light_obj.getLightInfo();
                            sp8->mColor = var_r28->mColor;
                        } else {
                            var_r28 = &kankyo->global_maple_col_change[i].light_obj.getLightInfo();
                        }

                        maple_color.r =
                            dKyd_maple_col_getp()[(g_env_light.fishing_hole_season - 1 + (i * 4))].r;
                        maple_color.g =
                            dKyd_maple_col_getp()[(g_env_light.fishing_hole_season - 1 + (i * 4))].g;
                        maple_color.b =
                            dKyd_maple_col_getp()[(g_env_light.fishing_hole_season - 1 + (i * 4))].b;

                        #if DEBUG
                        if (g_kankyoHIO.navy.fish_pond_tree_adjust_ON) {
                            if (i == 0) {
                                maple_color.r = g_kankyoHIO.navy.fish_pond_tree_dif0_col.r;
                                maple_color.g = g_kankyoHIO.navy.fish_pond_tree_dif0_col.g;
                                maple_color.b = g_kankyoHIO.navy.fish_pond_tree_dif0_col.b;
                            } else {
                                maple_color.r = g_kankyoHIO.navy.fish_pond_tree_dif1_col.r;
                                maple_color.g = g_kankyoHIO.navy.fish_pond_tree_dif1_col.g;
                                maple_color.b = g_kankyoHIO.navy.fish_pond_tree_dif1_col.b;
                            }
                        }
                        #endif

                        var_f31 = var_r28->mColor.r / 95.0f;
                        var_f31 *= var_f31;
                        if (var_f31 > 1.0f) {
                            var_f31 = 1.0f;
                        }
                        var_r28->mColor.r = maple_color.r;
                        var_r28->mColor.r *= var_f31;

                        var_f31 = var_r28->mColor.g / 95.0f;
                        var_f31 *= var_f31;
                        if (var_f31 > 1.0f) {
                            var_f31 = 1.0f;
                        }
                        var_r28->mColor.g = maple_color.g;
                        var_r28->mColor.g *= var_f31;

                        var_f31 = var_r28->mColor.b / 95.0f;
                        var_f31 *= var_f31;
                        if (var_f31 > 1.0f) {
                            var_f31 = 1.0f;
                        }
                        var_r28->mColor.b = maple_color.b;
                        var_r28->mColor.b *= var_f31;

                        material_p->setLight(i + 2, &kankyo->global_maple_col_change[i].light_obj);
                    } else {
                        material_p->setLight(i + 2, &tevstr_p->mLights[i]);
                    }
                }
            }
        }

        if (material_p->getFog() != NULL) {
            J3DFogInfo* fog_info = material_p->getFog()->getFogInfo();
            if (fog_info->mType != 0) {
                #if DEBUG
                switch (g_kankyoHIO.light.m_fogtype) {
                case 1:
                    fog_info->mType = 0;
                    break;
                case 2:
                    fog_info->mType = 2;
                    break;
                case 3:
                    fog_info->mType = 4;
                    break;
                case 4:
                    fog_info->mType = 5;
                    break;
                case 5:
                    fog_info->mType = 6;
                    break;
                case 6:
                    fog_info->mType = 7;
                    break;
                }
                #endif

                fog_info->mStartZ = tevstr_p->mFogStartZ;
                fog_info->mEndZ = tevstr_p->mFogEndZ;

                if (dComIfGd_getView() != NULL) {
                    fog_info->mNearZ = dComIfGd_getView()->near;
                    fog_info->mFarZ = dComIfGd_getView()->far;
                }

                if (fog_info->mType == 7) {
                    fog_info->mType = 2;
                    fog_info->mColor.r = 0;
                    fog_info->mColor.g = 0;
                    fog_info->mColor.b = 0;
                } else if (fog_info->mType == 6) {
                    fog_info->mColor.r = 0xFF;
                    fog_info->mColor.g = 0xFF;
                    fog_info->mColor.b = 0xFF;
                } else {
                    fog_info->mColor.r = tevstr_p->FogCol.r;
                    fog_info->mColor.g = tevstr_p->FogCol.g;
                    fog_info->mColor.b = tevstr_p->FogCol.b;
                }

                fog_info->mAdjEnable = g_env_light.mFogAdjEnable;
                if (fog_info->mAdjEnable == 1) {
                    fog_info->mCenter = g_env_light.mFogAdjCenter;
                    memcpy(&fog_info->mFogAdjTable, &g_env_light.mXFogTbl, sizeof(GXFogAdjTable));
                }
            }
        }
    }
}

void dKy_cloudshadow_scroll(J3DModelData* modelData_p, dKy_tevstr_c* tevstr_p, int param_2) {
    JUTNameTab* mat_nametbl = modelData_p->getMaterialName();

    for (int i = modelData_p->getMaterialNum() - 1; i >= 0; i--) {
        J3DMaterial* mat_p = modelData_p->getMaterialNodePointer(i);

        int lightType = param_2;
        if (tevstr_p->Material_id != 0 && tevstr_p->Material_id == i) {
            lightType = 2;
        }

        setLightTevColorType_MAJI_sub(mat_p, tevstr_p, lightType);

        const char* mat_name = mat_nametbl->getName(i);
        if ((mat_name[3] == 'M' && mat_name[4] == 'A' && mat_name[5] == '0' &&
             (mat_name[6] == '0' || mat_name[6] == '1')) ||
            (mat_name[5] == '1' && mat_name[6] == '6'))
        {
            if (mat_name[6] == '0') {
                GXColor k_color;
                k_color.r = g_env_light.mFogDensity & 0xFF;
                k_color.g = 0;
                k_color.b = 0;
                k_color.a = 0;
                mat_p->setTevKColor(1, (J3DGXColor*)&k_color);
            }

            if (mat_p->getTexGenBlock()->getTexMtx(1) != NULL) {
                J3DTexMtxInfo* tex_mtx_inf =
                    &mat_p->getTexGenBlock()->getTexMtx(1)->getTexMtxInfo();

                if (tex_mtx_inf != NULL && g_env_light.mpVrkumoPacket != NULL) {
                    tex_mtx_inf->mSRT.mTranslationX = g_env_light.mpVrkumoPacket->field_0x1150;
                    tex_mtx_inf->mSRT.mTranslationY = g_env_light.mpVrkumoPacket->field_0x1154;
                }
            }
        }
    }
}

void dScnKy_env_light_c::setLightTevColorType_MAJI(J3DModelData* modelData_p,
                                                   dKy_tevstr_c* tevstr_p) {
    #if DEBUG
    if (g_kankyoHIO.no_color_type) {
        return;
    }
    #endif

    if (tevstr_p->Material_use_fg != 124) {
        #if DEBUG
        if (!g_kankyoHIO.no_setting_tevstr) {
            OS_REPORT_ERROR("\ncoltype tevstr init Non. Material_use_fg[%d]\n",
                            tevstr_p->Material_use_fg);
        }
        #endif
        
        if (tevstr_p->Material_use_fg != 123) {
            dKy_tevstr_init(tevstr_p, dComIfGp_roomControl_getStayNo(), 0xFF);
        }

        settingTevStruct(11, NULL, tevstr_p);
    }

    dKy_setLight_nowroom_actor(tevstr_p);

    int lightType;
    if (tevstr_p->Type > 10 && tevstr_p->Type != 12 && tevstr_p->Type != 13) {
        lightType = 0;
    } else {
        lightType = 1;
    }

    if (tevstr_p->Type & 0x20) {
        dKy_cloudshadow_scroll(modelData_p, tevstr_p, lightType);
    } else {
        for (int i = modelData_p->getMaterialNum() - 1; i >= 0; i--) {
            J3DMaterial* material = modelData_p->getMaterialNodePointer(i);
            setLightTevColorType_MAJI_sub(material, tevstr_p, lightType);
        }
    }
}

void dScnKy_env_light_c::CalcTevColor() {
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);

    if (player_p != NULL) {
        mPlayerEfLightIdx = dKy_eflight_influence_id(player_p->current.pos, 0);
        mPlayerPLightIdx = dKy_light_influence_id(player_p->current.pos, 0);
    }
}

void dScnKy_env_light_c::Sndpos() {
    if (sound_influence.timer != 0) {
        sound_influence.timer--;
        if (sound_influence.timer == 0) {
            dKy_Sound_init();
        }
    }
}

void dScnKy_env_light_c::Eflight_flush_proc() {
    f32 power;
    f32 fluctuation;
    dKy_flush_info* info;

    if (eflight.mLightType == 0) {
        static dKy_flush_info flush_col[] = {
            {1, 0x00, 0x00, 0x00},
            {4, 0x43, 0x73, 0x58},
            {10, 0x25, 0x3E, 0x2D},
            {20, 0x00, 0x00, 0x00},
        };

        info = flush_col;
        fluctuation = 100.0f;
        power = 120.0f;
    } else if (eflight.mLightType == 1) {
        static dKy_flush_info flush_col2[] = {
            {1, 0x00, 0x00, 0x00},
            {4, 0x6E, 0x5A, 0x00},
            {8, 0x4B, 0x22, 0x00},
            {15, 0x00, 0x00, 0x00},
        };

        info = flush_col2;
        fluctuation = 0.0f;
        power = 120.0f;
    } else {
        static dKy_flush_info flush_col3[] = {
            {1, 0x00, 0x00, 0x00},
            {4, 0x46, 0x4D, 0x43},
            {15, 0x22, 0x1E, 0x00},
            {20, 0x00, 0x00, 0x00},
        };

        info = flush_col3;
        fluctuation = 100.0f;
        power = 80.0f;
    }

    #if DEBUG
    if (g_kankyoHIO.effLight.adjust_ON) {
        info = &g_kankyoHIO.effLight.step1;
        power = g_kankyoHIO.effLight.power;
        fluctuation = g_kankyoHIO.effLight.fluctuation;
    }
    #endif

    switch (eflight.mState) {
    case 0:
        break;
    case 1:
        eflight.mFrame = 0;
        eflight.field_0x8.mColor.r = info[0].r;
        eflight.field_0x8.mColor.g = info[0].g;
        eflight.field_0x8.mColor.b = info[0].b;
        eflight.field_0x8.mPow = power;
        eflight.field_0x8.mFluctuation = fluctuation;

        dKy_efplight_set(&eflight.field_0x8);
        eflight.mState++;
        break;
    case 2:
        for (int i = 0; i < 3; i++) {
            if (eflight.mFrame >= info[i].start_frame && eflight.mFrame <= info[i + 1].start_frame)
            {
                f32 ratio = 1.0f - (f32)(info[i + 1].start_frame - eflight.mFrame) /
                                       (f32)(info[i + 1].start_frame - info[i].start_frame);

                eflight.field_0x8.mColor.r = u8_data_ratio_set(info[i].r, info[i + 1].r, ratio);
                eflight.field_0x8.mColor.g = u8_data_ratio_set(info[i].g, info[i + 1].g, ratio);
                eflight.field_0x8.mColor.b = u8_data_ratio_set(info[i].b, info[i + 1].b, ratio);
                break;
            }
        }

        if (eflight.mFrame > info[3].start_frame) {
            eflight.mState++;
        }

        eflight.mFrame++;
        break;
    case 3:
        dKy_efplight_cut(&eflight.field_0x8);
        eflight.mState = 0;
        break;
    case 4:
        dKy_efplight_cut(&eflight.field_0x8);
        eflight.mState = 1;
        break;
    }
}

void dScnKy_env_light_c::SetBaseLight() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    f32 daytime;

    if (dKy_SunMoon_Light_Check() == TRUE) {
        daytime = g_env_light.getDaytime();
        if (daytime > 67.5f && daytime < 292.5f) {
            base_light.mPosition = kankyo->sun_light_pos;
        } else if (camera != NULL) {
            base_light.mPosition = camera->lookat.eye + kankyo->moon_pos;
        } else {
            base_light.mPosition = kankyo->moon_pos;
        }
    } else {
        base_light.mPosition = lightStatusPt[2].position;
    }

    base_light.mColor.r = 0xFF;
    base_light.mColor.g = 0xFF;
    base_light.mColor.b = 0xFF;
    base_light.mColor.a = 0xFF;
    base_light.mPow = 0.0f;
    base_light.mFluctuation = 0.0f;
}

void dScnKy_env_light_c::exeKankyo() {
    int sp18 = 0;

    for (int i = 0; i < 6; i++) {
        field_0x0c18[i].field_0x26 = 0;

        if (field_0x0d58[i].field_0x26 != 0) {
            field_0x0d58[i].field_0x26 = 0;
            dKy_BossLight_set(&field_0x0d58[i].mPos, &field_0x0d58[i].mColor,
                              field_0x0d58[i].mRefDistance, 0);
        }
    }

    g_env_light.mColPatMode = g_env_light.mColPatModeGather;

    if (dComIfGp_event_runCheck() == false && g_env_light.mColPatModeGather != 0) {
        if (g_env_light.mColPatModeGather >= 3) {
            g_env_light.mColPatModeGather = 0;
        } else {
            g_env_light.mColPatModeGather++;
        }
    }

    if (g_env_light.mColPatMode != 0) {
        if (g_env_light.mColpatPrevGather != 0xFF) {
            g_env_light.wether_pat0 = g_env_light.mColpatPrevGather;

            if (g_env_light.mColPatModeGather == 0) {
                g_env_light.mColpatPrevGather = 0xFF;
            }
        }

        if (g_env_light.mColpatCurrGather != 0xFF) {
            g_env_light.wether_pat1 = g_env_light.mColpatCurrGather;

            if (g_env_light.mColPatModeGather == 0) {
                g_env_light.mColpatCurrGather = 0xFF;
            }
        }

        if (g_env_light.mColPatBlendGather >= 0.0f) {
            g_env_light.pat_ratio = g_env_light.mColPatBlendGather;

            if (g_env_light.mColPatModeGather == 0) {
                g_env_light.mColPatBlendGather = -1.0f;
            }
        }
    } else if (g_env_light.wether_pat0 == g_env_light.wether_pat1) {
        if (g_env_light.mColpatPrevGather != 0xFF) {
            g_env_light.wether_pat0 = g_env_light.mColpatPrevGather;
            g_env_light.mColpatPrevGather = 0xFF;
        }

        if (g_env_light.mColpatCurrGather != 0xFF) {
            g_env_light.wether_pat1 = g_env_light.mColpatCurrGather;
            g_env_light.mColpatCurrGather = 0xFF;
            g_env_light.mColpatWeather = g_env_light.wether_pat1;
        }

        if (g_env_light.mColPatBlendGather >= 0.0f) {
            g_env_light.pat_ratio = g_env_light.mColPatBlendGather;
            g_env_light.mColPatBlendGather = -1.0f;
        }
    }

    g_env_light.field_0x1210 = g_env_light.field_0x122c;

    cLib_addCalc(&g_env_light.now_allcol_ratio, g_env_light.allcol_ratio, 0.5f, 0.25f, 0.01f);
    cLib_addCalc(&g_env_light.now_actcol_ratio, g_env_light.actcol_ratio, 0.5f, 0.25f, 0.01f);
    cLib_addCalc(&g_env_light.now_bgcol_ratio, g_env_light.bgcol_ratio * g_env_light.field_0x1210,
                 0.5f, 0.25f, 0.01f);
    cLib_addCalc(&g_env_light.now_fogcol_ratio, g_env_light.fogcol_ratio * g_env_light.field_0x1210,
                 0.5f, 0.25f, 0.01f);
    cLib_addCalc(&g_env_light.now_vrboxsoracol_ratio,
                 g_env_light.vrboxsoracol_ratio * g_env_light.field_0x1210, 0.5f, 0.25f, 0.01f);
    cLib_addCalc(&g_env_light.now_vrboxkumocol_ratio,
                 g_env_light.vrboxkumocol_ratio * g_env_light.field_0x1210, 0.5f, 0.25f, 0.01f);

    g_env_light.allcol_ratio = 1.0f;
    g_env_light.actcol_ratio = 1.0f;
    g_env_light.bgcol_ratio = 1.0f;
    g_env_light.fogcol_ratio = 1.0f;
    g_env_light.vrboxsoracol_ratio = 1.0f;
    g_env_light.vrboxkumocol_ratio = 1.0f;
    g_env_light.field_0x122c = 1.0f;
    g_env_light.field_0x126c = g_env_light.field_0x1268;
    g_env_light.field_0x1268 = 999999.9f;

    for (int i = 50; i < 100; i++) {
        g_env_light.pointlight[i] = NULL;
    }

    g_env_light.field_0x130b = 0;

    setDaytime();
    dKyw_wether_proc();
    CalcTevColor();
    Sndpos();
    Eflight_flush_proc();

    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (player_p != NULL) {
        cXyz* flame_pos_p = player_p->getKandelaarFlamePos();

        if (flame_pos_p != NULL) {
            g_env_light.field_0x10a0 = *flame_pos_p;
            dKy_shadow_mode_set(2);
        } else if (dKy_shadow_mode_check(2)) {
            dKy_shadow_mode_reset(2);
        }
    }

    if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
        if ((dCam_getBody()->Mode() == 4 || dCam_getBody()->Mode() == 7) && dComIfGp_event_runCheck())
        {
            cLib_addCalc(&g_env_light.mDemoAttentionPoint, 0.0f, 0.5f, 0.1f, 1E-05f);
        } else {
            cLib_addCalc(&g_env_light.mDemoAttentionPoint, 0.11f, 0.5f, 0.1f, 1E-05f);
        }
    }

    #if DEBUG
    if (!g_kankyoHIO.light.field_0x5) {
        g_kankyoHIO.light.field_0x8 = g_env_light.mFogAdjCenter;
        for (int i = 0; i < 10; i++) {
            g_kankyoHIO.light.field_0xc[i] = g_env_light.mXFogTbl.r[i];
        }
    } else {
        g_env_light.mFogAdjCenter = g_kankyoHIO.light.field_0x8;
        for (int i = 0; i < 10; i++) {
            g_env_light.mXFogTbl.r[i] = g_kankyoHIO.light.field_0xc[i];
        }
    }

    dKydb_color_HIO_update();
    dKydb_vrbox_HIO_update();

    if (mDoCPd_c::getHoldR(PAD_2) && mDoCPd_c::getHoldL(PAD_2) && mDoCPd_c::getTrigL(PAD_2)) {
        static u8 data_8074c97c;
        if (!data_8074c97c) {
            fapGmHIO_set2Ddraw(0);
            fapGmHIO_offPrint();
            g_HIO.mDisplayMeter = 0;
            data_8074c97c = TRUE;
        } else {
            fapGmHIO_set2Ddraw(1);
            fapGmHIO_onPrint();
            g_HIO.mDisplayMeter = 1;
            data_8074c97c = FALSE;
        }
    }

    cXyz particle_pos;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (player != NULL) {
        particle_pos = player->current.pos;
        particle_pos.y += 10.0f;

        GXColor prim, env;
        prim = g_kankyoHIO.particleLight.prim_col;
        env = g_kankyoHIO.particleLight.env_col;

        if (g_kankyoHIO.particleLight.field_0x14) {
            g_kankyoHIO.particleLight.field_0x14 = 0;

            switch (g_kankyoHIO.particleLight.type) {
            case 0:
                dComIfGp_particle_setColor(ID_ZI_J_TESTSMOKE_A01, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 1:
                dComIfGp_particle_setColor(ID_ZI_J_DASHSMOKE_A, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 2:
                dComIfGp_particle_setColor(ID_ZI_J_DOWNSMOKE_A, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 3:
                dComIfGp_particle_setColor(ID_ZI_J_TESTSMOKE_A02, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 4:
                dComIfGp_particle_setColor(dPa_RM(ID_ZI_S_SCREENKAGEROU01), &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
            case 5:
                dComIfGp_particle_setColor(ID_ZI_J_ZENKAISCREEN2_A, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 6:
                dComIfGp_particle_setColor(ID_ZI_J_ZENKAISCREEN2_B, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 7:
                dComIfGp_particle_setColor(ID_ZI_J_TESTSMOKE_B01, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 8:
                dComIfGp_particle_setColor(ID_ZI_J_TESTSMOKE_B02, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 9:
                dComIfGp_particle_setColor(ID_ZI_J_TESTSMOKE_c01, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            case 10:
                dComIfGp_particle_setColor(ID_ZI_J_TESTSMOKE_c02, &particle_pos, &player->tevStr, &prim, &env, g_kankyoHIO.particleLight.blend_ratio, g_kankyoHIO.particleLight.prim_col.a);
                break;
            }
        }
    }
    #endif
}

#if DEBUG
dKankyo_lightHIO_c::dKankyo_lightHIO_c() {
    m_HOSTIO_setting = FALSE;
    field_0x52 = 0;
    m_forcedPalette = FALSE;
    m_displayColorPaletteCheckInfo = TRUE;

    field_0x58 = 0.0f;
    field_0x60 = 0;
    field_0x61 = 0;
    field_0x62 = 0;
    field_0x63 = 0;
    field_0x64 = 0;
    field_0x65 = 0;
    field_0x66 = 0;
    field_0x67 = 0;
    field_0x68 = 0;
    field_0x69 = 0;
    field_0x6a = 0;
    field_0x6b = 0;

    m_BG_fakelight_R = 0;
    m_BG_fakelight_G = 0;
    m_BG_fakelight_B = 0;
    m_BG_fakelight_power = 0.0f;

    field_0x80 = 0;
}

void dKankyo_lightHIO_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("「カラーパレット＆ライト位置情報の設定」", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->startComboBox("■ HOSTIO設定", &m_HOSTIO_setting, 0, NULL, 0xFFFF, 0xFFFF, 0x100,
                        0x1a);
    mctx->genComboBoxItem("未使用", 0);
    mctx->genComboBoxItem("ＨＯＳＴＩＯ設定！", 1);
    mctx->endComboBox();
    mctx->genLabel("ＨＯＳＴＩＯ設定にすると、ゲーム内の現在データが入ります！", 0x80000001, 0,
                   NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("これをしないとカラー変更が利きません！", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);

    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    // ● Adjustment of kantera, wolf lights, and underwater lights.
    mctx->genLabel("●カンテラ・狼ライト・水中ライトの調整", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genCheckBox("調整開始", &g_kankyoHIO.navy.field_0x311, 1, 0, NULL, 0xFFFF,
                      0xFFFF, 0x200, 0x18);
    mctx->genSlider("地形のみ高さ補正", &g_kankyoHIO.navy.terrain_height_crr, -5000.0,
                    5000.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("影響力　倍率", &g_kankyoHIO.navy.influence_multiplier, 0.0,
                    100.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("cutoff　倍率", &g_kankyoHIO.navy.cutoff_multiplier, 0.01,
                    10.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("● 部屋ライトタイプのＳｍＬLL切り替え（調整用）", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->startComboBox("選択", &g_kankyoHIO.navy.room_light_type, 0, NULL, 0xFFFF,
                        0xFFFF, 0x100, 0x1a);
    mctx->genComboBoxItem("ゲームまかせ", 0);
    mctx->genComboBoxItem("（ Ｓ ）", 1);
    mctx->genComboBoxItem("（ Ｍ ）", 2);
    mctx->genComboBoxItem("（ Ｌ ）", 3);
    mctx->genComboBoxItem("（ＬＬ）", 4);
    mctx->endComboBox();

    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("● ポリゴンコードによる設定(ファイル出力無し)", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("ここで取り決めた数値を該当のポリゴンコードのカラーに設定してください",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("「アクタへの生ライトの影響率」", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                   0x18);
    mctx->genSlider("影響率(0%-200%)", &g_env_light.mActorLightEffect, 0, 200, 0,
                    NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    //● TEV_TYPE_DOOR attribute set to the actor's argument (no file output)
    mctx->genLabel("● TEV_TYPE_DOOR属性アクタの引数へ設定する(ファイル出力無し)", 0x80000001, 0,
                   NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("「ドアなど引数へ設定する物への生ライトの影響率」", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->startComboBox("選択", &g_kankyoHIO.navy.door_light_influence_ratio, 0, NULL,
                        0xFFFF, 0xFFFF, 0x100, 0x1a);
    mctx->genComboBoxItem("ゲームまかせ", 0xff);
    mctx->genComboBoxItem("0-20%", 0);
    mctx->genComboBoxItem("1-30%", 1);
    mctx->genComboBoxItem("2-40%", 2);
    mctx->genComboBoxItem("3-60%", 3);
    mctx->genComboBoxItem("4-80%", 4);
    mctx->genComboBoxItem("5-90%", 5);
    mctx->genComboBoxItem("6-100%", 6);
    mctx->genComboBoxItem("7-120%", 7);
    mctx->genComboBoxItem("↓で新設", 0x99);
    mctx->endComboBox();
    mctx->genLabel("新たな％を試したい場合はコチラ、プログラム対応必要です。ささきまで", 0x80000001,
                   0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("影響率(0%-200%)", &g_kankyoHIO.navy.unk_light_influence_ratio, 0,
                    200, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("● カラーパレットごとに入っている設定値", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genSlider("地形ライト影響率 ", &g_env_light.bg_light_influence, 0.0,
                    2.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("雲影の濃さ ", &g_env_light.mFogDensity, 0, 0xff, 0, NULL,
                    0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("●草ライト影響率 「部屋情報」にてセット", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genCheckBox("調整開始", &g_kankyoHIO.navy.grass_light_debug, 1, 0, NULL,
                      0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("影響率 ", &g_kankyoHIO.navy.grass_light_influence_ratio, 0, 200,
                    0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("● 草Amb 草を配置時に引数にセット", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                   0x18);
    mctx->genCheckBox("調整開始", &g_kankyoHIO.navy.grass_adjust_ON, 1, 0, NULL,
                      0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("引数１ r", &g_kankyoHIO.navy.grass_ambcol.r, 0, 30, 0, NULL,
                    0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("引数２ g", &g_kankyoHIO.navy.grass_ambcol.g, 0, 30, 0, NULL,
                    0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("引数３ b", &g_kankyoHIO.navy.grass_ambcol.b, 0, 30, 0, NULL,
                    0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->startComboBox("■飽和パターン", &g_kankyoHIO.bloom.m_saturationPattern, 0,
                        NULL, 0xFFFF, 0xFFFF, 0x100, 0x1a);
    mctx->genComboBoxItem("使用しない", 0);
    mctx->genComboBoxItem("1 トワイライト(く)", 1);
    mctx->genComboBoxItem("2 トワイライト_弱(く)", 2);
    mctx->genComboBoxItem("3 センス", 3);
    mctx->genComboBoxItem("4 フィールド基準(朝0)", 4);
    mctx->genComboBoxItem("5 フィールド基準(朝1)", 5);
    mctx->genComboBoxItem("6 フィールド基準(昼)", 6);
    mctx->genComboBoxItem("7 フィールド基準(夕0)", 7);
    mctx->genComboBoxItem("8 フィールド基準(夕1)", 8);
    mctx->genComboBoxItem("9 フィールド基準(夜)", 9);
    mctx->genComboBoxItem("10 キャラバン火事", 10);
    mctx->genComboBoxItem("11 LV8闇の宮殿", 11);
    mctx->genComboBoxItem("12 demo28_03", 12);
    mctx->genComboBoxItem("13 虫が光る", 13);
    mctx->genComboBoxItem("14 雲海(勇者の間)」", 14);
    mctx->genComboBoxItem("15 ゾーラの里 滝TW", 15);
    mctx->genComboBoxItem("16 ゾーラの里 玉座TW", 16);
    mctx->genComboBoxItem("17 センスもどき", 17);
    mctx->genComboBoxItem("18 現在過去切替", 18);
    mctx->genComboBoxItem("19 宮城さん下水道", 19);
    mctx->genComboBoxItem("20 回想シーン", 20);
    mctx->genComboBoxItem("21 トワイライトゲート", 21);
    mctx->genComboBoxItem("22 水中（基本）", 22);
    mctx->genComboBoxItem("25 ダンジョン基準_弱", 25);
    mctx->genComboBoxItem("26 ダンジョン基準_強", 26);
    mctx->genComboBoxItem("27 溶岩(く)", 27);
    mctx->genComboBoxItem("28 溶岩以外_Lv2(く)", 28);
    mctx->genComboBoxItem("29 過去", 29);
    mctx->genComboBoxItem("30 城壁", 30);
    mctx->genComboBoxItem("40 デモ：回想シーン", 40);
    mctx->genComboBoxItem("41 デモ：にせＴＷ", 41);
    mctx->genComboBoxItem("42 デモ：demo32_03用", 42);
    mctx->genComboBoxItem("43 デモ：ＴＷ回想シーン", 43);
    mctx->genComboBoxItem("44 デモ：demo31_20用", 44);
    mctx->genComboBoxItem("45 デモ：溶岩Ｗ", 45);
    mctx->endComboBox();

    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("● CSV情報のファイル書き出し", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("(カラーパレット情報のファイル書き出し)", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genButton("パレットWRITE", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("(生ライト位置情報リストファイル書き出し)", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genButton("位置情報WRITE", 0x40000003, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("● CSV情報のファイル読み込み（確認用）", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genLabel("(カラーパレット情報のファイル読み込み)", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genButton("FILEREAD", 0x40000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("(生ライト位置情報リストファイル読み込み)", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    mctx->genButton("位置情報READ", 0x41000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("● アンビエント", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-----------------------------------------------------------------------------------------",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("(アクタ)", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("■ ACTOR_Amb Ｒ", &g_env_light.actor_amb_col.r, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("             Ｇ", &g_env_light.actor_amb_col.g, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("             Ｂ", &g_env_light.actor_amb_col.b, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +100", 0x40000010, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     + 10", 0x40000011, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +  1", 0x40000012, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -  1", 0x40000013, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     - 10", 0x40000014, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -100", 0x40000015, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("(地形)", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("■ ＢＧ０_Amb Ｒ", &g_env_light.bg_amb_col[0].r, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("              Ｇ", &g_env_light.bg_amb_col[0].g, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("        　　　Ｂ", &g_env_light.bg_amb_col[0].b, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +100", 0x40000016, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     + 10", 0x40000017, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +  1", 0x40000018, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -  1", 0x40000019, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     - 10", 0x4000001a, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -100", 0x4000001b, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("■ ＢＧ１_Amb Ｒ", &g_env_light.bg_amb_col[1].r, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("        　　　Ｇ", &g_env_light.bg_amb_col[1].g, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("        　　　Ｂ", &g_env_light.bg_amb_col[1].b, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +100", 0x4000001c, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     + 10", 0x4000001d, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +  1", 0x4000001e, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -  1", 0x4000001f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     - 10", 0x40000020, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -100", 0x40000021, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("水面α  Ａ", &g_env_light.bg_amb_col[1].a, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("補佐α  Ａ２", &g_env_light.bg_amb_col[2].a, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("■ ＢＧ２_Amb Ｒ", &g_env_light.bg_amb_col[2].r, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("        　　　Ｇ", &g_env_light.bg_amb_col[2].g, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("        　　　Ｂ", &g_env_light.bg_amb_col[2].b, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +100", 0x40000022, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     + 10", 0x40000023, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +  1", 0x40000024, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -  1", 0x40000025, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     - 10", 0x40000026, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -100", 0x40000027, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("■ ＢＧ３_Amb Ｒ", &g_env_light.bg_amb_col[3].r, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("        　　　Ｇ", &g_env_light.bg_amb_col[3].g, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("        　　　Ｂ", &g_env_light.bg_amb_col[3].b, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +100", 0x40000028, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     + 10", 0x40000029, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +  1", 0x4000002a, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -  1", 0x4000002b, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     - 10", 0x4000002c, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -100", 0x4000002d, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("ウソFog　Ａ", &g_env_light.bg_amb_col[3].a, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("● フォグ", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-----------------------------------------------------------------------------------------",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->startComboBox("fog type", &m_fogtype, 0, NULL, 0xFFFF, 0xFFFF, 0x100, 0x1a);
    mctx->genComboBoxItem("STAN-BY", 2);
    mctx->genComboBoxItem("GX_FOG_NONE", 0);
    mctx->genComboBoxItem("GX_FOG_LIN", 2);
    mctx->genComboBoxItem("GX_FOG_EXP", 4);
    mctx->genComboBoxItem("GX_FOG_EXP2", 5);
    mctx->genComboBoxItem("GX_FOG_REVEXP", 6);
    mctx->genComboBoxItem("GX_FOG_REVEXP2", 7);
    mctx->endComboBox();
    mctx->genSlider("■ ＦＯＧ   Ｒ", &g_env_light.fog_col.r, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("            Ｇ", &g_env_light.fog_col.g, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("            Ｂ", &g_env_light.fog_col.b, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +100", 0x4000002e, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     + 10", 0x4000002f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     +  1", 0x40000030, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -  1", 0x40000031, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     - 10", 0x40000032, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton("RGB同時加算     -100", 0x40000033, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("          near", &g_env_light.mFogNear, -2500000.0, 2500000.0, 0,
                    NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("           far", &g_env_light.mFogFar, -2500000.0, 2500000.0, 0,
                    NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("――――――――――――――――――――――――――――――――――――――――――", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("● 生ライト", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-----------------------------------------------------------------------------------------",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("デバッグ球の表示",
                      &g_kankyoHIO.dungeonLight.displayDebugSphere, 1, 0, NULL,
                      0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->startComboBox("■ 使用ライト", &g_kankyoHIO.dungeonLight.usedLights, 0, NULL,
                        0xFFFF, 0xFFFF, 0x100, 0x1a);
    mctx->genComboBoxItem("なし", 0);
    mctx->genComboBoxItem("２", 1);
    mctx->genComboBoxItem("２ ３", 2);
    mctx->genComboBoxItem("２ ３ ４", 3);
    mctx->genComboBoxItem("２ ３ ４ ５", 4);
    mctx->genComboBoxItem("２ ３ ４ ５ ６", 5);
    mctx->genComboBoxItem("２ ３ ４ ５ ６ ７", 6);
    mctx->endComboBox();

    for (int i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            mctx->genLabel("-----------------------------------------------------------------------------------------",
                            0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("（ライト０）―えせポイントライト", 0x80000001, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("（ライト１）―エフェクトライト", 0x80000001, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("-----------------------------------------------------------------------------------------",
                            0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("● ● ●  ラ イ ト ２  ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●",
                            0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("※太陽が存在する場合、設定は上書きされます", 0x80000001, 0, NULL,
                            0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("↓↓↓ 誤押注意 ↓↓↓", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("プレイヤー位置にライト２を移動", 0x40000004, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genButton("カメラCTR 位置にライト２を移動", 0x400000a0, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("(((ライト２位置へワープ！！)))", 0x4000000a, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            break;
        case 1:
            mctx->genLabel("-----------------------------------------------------------------------------------------",
                            0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("● ● ●  ラ イ ト ３  ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●",
                            0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("※月が存在する場合、設定が上書きされます", 0x80000001, 0, NULL,
                            0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("↓↓↓ 誤押注意 ↓↓↓", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("プレイヤー位置にライト３を移動！", 0x40000005, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genButton("カメラCTR 位置にライト３を移動", 0x400000a1, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("(((ライト３位置へワープ！！)))", 0x4000000b, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            break;
        case 2:
            mctx->genLabel("-----------------------------------------------------------------------------------------",
                            0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("● ● ●  ラ イ ト ４  ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●",
                            0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("↓↓↓ 誤押注意 ↓↓↓", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("プレイヤー位置にライト４を移動！", 0x40000006, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genButton("カメラCTR 位置にライト４を移動", 0x400000a2, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("(((ライト４位置へワープ！！)))", 0x4000000c, 0, NULL, 0xFFFF,
                            0xFFFF, 0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            break;
        case 3:
            mctx->genLabel("-----------------------------------------------------------------------------------------",
                           0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("● ● ●  ラ イ ト ５  ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●",
                           0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("↓↓↓ 誤押注意 ↓↓↓", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("プレイヤー位置にライト５を移動！", 0x40000007, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("カメラCTR 位置にライト５を移動", 0x400000a3, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("(((ライト５位置へワープ！！)))", 0x4000000d, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            break;
        case 4:
            mctx->genLabel("-----------------------------------------------------------------------------------------",
                           0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("● ● ●  ラ イ ト ６  ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●",
                           0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("↓↓↓ 誤押注意 ↓↓↓", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("プレイヤー位置にライト６を移動！", 0x40000008, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("カメラCTR 位置にライト６を移動", 0x400000a4, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("(((ライト６位置へワープ！！)))", 0x4000000e, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            break;
        case 5:
            mctx->genLabel("-----------------------------------------------------------------------------------------",
                           0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("● ● ●  ラ イ ト ７  ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●",
                           0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genLabel("↓↓↓ 誤押注意 ↓↓↓", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("プレイヤー位置にライト７を移動！", 0x40000009, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("カメラCTR 位置にライト７を移動", 0x400000a5, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            mctx->genButton("(((ライト７位置へワープ！！)))", 0x4000000f, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
            break;
        }

        mctx->genLabel("※色を変える場合はカラーパレットのファイル作成も忘れずに！", 0x80000001, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("色  Ｒ", &g_env_light.dungeonlight[i].mColor.r, 0,
                        0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("    Ｇ", &g_env_light.dungeonlight[i].mColor.g, 0,
                        0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("    Ｂ", &g_env_light.dungeonlight[i].mColor.b, 0,
                        0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

        switch (i) {
        case 0:
            mctx->genButton("RGB同時加算     +100", 0x40000034, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("RGB同時加算     + 10", 0x40000035, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("RGB同時加算     +  1", 0x40000036, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("RGB同時加算     -  1", 0x40000037, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("RGB同時加算     - 10", 0x40000038, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            mctx->genButton("RGB同時加算     -100", 0x40000039, 0, NULL, 0xFFFF, 0xFFFF,
                            0x200, 0x18);
            break;
        case 1:
            mctx->genButton("RGB同時加算     +100", 0x4000003a, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     + 10", 0x4000003b, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     +  1", 0x4000003c, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -  1", 0x4000003d, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     - 10", 0x4000003e, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -100", 0x4000003f, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            break;
        case 2:
            mctx->genButton("RGB同時加算     +100", 0x40000040, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     + 10", 0x40000041, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     +  1", 0x40000042, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -  1", 0x40000043, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     - 10", 0x40000044, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -100", 0x40000045, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            break;
        case 3:
            mctx->genButton("RGB同時加算     +100", 0x40000046, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     + 10", 0x40000047, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     +  1", 0x40000048, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -  1", 0x40000049, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     - 10", 0x4000004a, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -100", 0x4000004b, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            break;
        case 4:
            mctx->genButton("RGB同時加算     +100", 0x4000004c, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     + 10", 0x4000004d, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     +  1", 0x4000004e, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -  1", 0x4000004f, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     - 10", 0x40000050, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -100", 0x40000051, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            break;
        case 5:
            mctx->genButton("RGB同時加算     +100", 0x40000052, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     + 10", 0x40000053, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     +  1", 0x40000054, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -  1", 0x40000055, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     - 10", 0x40000056, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            mctx->genButton("RGB同時加算     -100", 0x40000057, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                            0x18);
            break;
        }

        mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("位置Ｘ", &g_env_light.dungeonlight[i].mPosition.x,
                        -300000.0, 300000.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("    Ｙ", &g_env_light.dungeonlight[i].mPosition.y,
                        -300000.0, 300000.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("    Ｚ", &g_env_light.dungeonlight[i].mPosition.z,
                        -300000.0, 300000.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("ref_distance",
                        &g_env_light.dungeonlight[i].mRefDistance, 0.01, 10000.0,
                        0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->startComboBox("距離減衰　○推奨",
                            &g_env_light.dungeonlight[i].mDistAttenuation, 0, NULL,
                            0xFFFF, 0xFFFF, 0x100, 0x1a);
        mctx->genComboBoxItem("※減衰なし [GX_DA_OFF]", 0);
        mctx->genComboBoxItem("おだやか [GX_DA_GENTLE]", 1);
        mctx->genComboBoxItem("中間     [GX_DA_MEDIUM]", 2);
        mctx->genComboBoxItem("○険しい   [GX_DA_STEEP]", 3);
        mctx->endComboBox();
        mctx->startComboBox("角度減衰　○推奨",
                            &g_env_light.dungeonlight[i].mAngleAttenuation, 0, NULL,
                            0xFFFF, 0xFFFF, 0x100, 0x1a);
        mctx->genComboBoxItem("○ポイント [GX_SP_OFF]", 0);
        mctx->genComboBoxItem("※直角     [GX_SP_FLAT]", 1);
        mctx->genComboBoxItem("○COS曲線  [GX_SP_COS]", 2);
        mctx->genComboBoxItem("COS2曲線 [GX_SP_COS2]", 3);
        mctx->genComboBoxItem("シャープ [GX_SP_SHARP]", 4);
        mctx->genComboBoxItem("リング状 [GX_SP_RING1]", 5);
        mctx->genComboBoxItem("リング状2[GX_SP_RING2]", 6);
        mctx->endComboBox();
        mctx->genLabel("角度減衰をポイント以外に設定したらスポットライトなので以下も設定を！",
                       0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("カットオフ角度",
                        &g_env_light.dungeonlight[i].mCutoffAngle, 0.001, 90.0,
                        0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("Ｘ角度(紫軸)", &g_env_light.dungeonlight[i].mAngleX,
                        -360.0, 360.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genSlider("Ｙ角度(緑軸)", &g_env_light.dungeonlight[i].mAngleY,
                        -360.0, 360.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    }

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("● ● ●  えせライト地形反映特別版　 ● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("「地形反映えせライト」を配置した部屋で有効です。", 0x80000001, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    mctx->genLabel("調整した値をマップツールで「地形反映えせライト」のパラメータへ指定して下さい",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("位置Ｘ", &g_kankyoHIO.light.m_BG_fakelight_pos.x, -300000.0,
                    300000.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("    Ｙ", &g_kankyoHIO.light.m_BG_fakelight_pos.y, -300000.0,
                    300000.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("    Ｚ", &g_kankyoHIO.light.m_BG_fakelight_pos.z, -300000.0,
                    300000.0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("Color    R", &g_kankyoHIO.light.m_BG_fakelight_R, 0, 0xff, 0,
                    NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("         G", &g_kankyoHIO.light.m_BG_fakelight_G, 0, 0xff, 0,
                    NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("         B", &g_kankyoHIO.light.m_BG_fakelight_B, 0, 0xff, 0,
                    NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("Power    ", &g_kankyoHIO.light.m_BG_fakelight_power, 0.0, 1000.0,
                    0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("=========================================================================================",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("カラー設定ＴＶ画面確認表示", &m_displayTVColorSettings, 1, 0,
                      NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("カラーパレットチェック画面表示", &m_displayColorPaletteCheckInfo,
                      1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox("カラーパレットチェック画面表示詳細", &m_displayColorPaletteCheckInfo,
                      2, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("=========================================================================================",
                   0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->startComboBox("強制パレット指定", &m_forcedPalette, 0, NULL, 0xFFFF, 0xFFFF, 0x100,
                        0x1a);
    mctx->genComboBoxItem("しない", 0);
    mctx->genComboBoxItem("パレット０使用", 1);
    mctx->genComboBoxItem("パレット１使用", 2);
    mctx->genComboBoxItem("パレット２使用", 3);
    mctx->genComboBoxItem("パレット３使用", 4);
    mctx->genComboBoxItem("パレット４使用", 5);
    mctx->genComboBoxItem("パレット５使用", 6);
    mctx->genComboBoxItem("パレット６使用", 7);
    mctx->genComboBoxItem("パレット７使用", 8);
    mctx->genComboBoxItem("パレット８使用", 9);
    mctx->genComboBoxItem("パレット９使用", 10);
    mctx->genComboBoxItem("パレット１０使用", 11);
    mctx->genComboBoxItem("パレット１１使用", 12);
    mctx->genComboBoxItem("パレット１２使用", 13);
    mctx->genComboBoxItem("パレット１３使用", 14);
    mctx->genComboBoxItem("パレット１４使用", 15);
    mctx->genComboBoxItem("パレット１５使用", 16);
    mctx->endComboBox();

    mctx->genLabel("=========================================================================================", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}

void dKankyo_lightHIO_c::dKankyo_lightHIOInfoUpDateF() {
    JORMContext* mctx = attachJORMContext(8);
    mctx->startUpdateNode(&g_kankyoHIO.light);

    mctx->updateSlider(2, &g_env_light.actor_amb_col.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.actor_amb_col.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.actor_amb_col.b, 0, 0, 0);

    mctx->updateSlider(2, &g_env_light.bg_amb_col[0].r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[0].g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[0].b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[1].r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[1].g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[1].b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[2].r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[2].g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[2].b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[3].r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[3].g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.bg_amb_col[3].b, 0, 0, 0);

    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mColor.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mColor.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mColor.b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mColor.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mColor.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mColor.b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mColor.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mColor.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mColor.b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mColor.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mColor.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mColor.b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mColor.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mColor.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mColor.b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mColor.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mColor.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mColor.b, 0, 0, 0);

    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mPosition.x, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mPosition.y, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mPosition.z, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mPosition.x, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mPosition.y, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mPosition.z, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mPosition.x, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mPosition.y, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mPosition.z, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mPosition.x, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mPosition.y, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mPosition.z, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mPosition.x, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mPosition.y, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mPosition.z, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mPosition.x, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mPosition.y, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mPosition.z, 0.0f, 0.0f, 0);

    mctx->updateSlider(2, &g_env_light.fog_col.r, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.fog_col.g, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.fog_col.b, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.mFogNear, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.mFogFar, 0.0f, 0.0f, 0);

    mctx->updateSlider(2, &g_env_light.bg_light_influence, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.mFogDensity, 0, 0, 0);
    mctx->updateSlider(2, &g_env_light.field_0x12f9, 0, 0, 0);
    mctx->updateComboBox(2, &g_kankyoHIO.bloom.m_saturationPattern, 0);

    mctx->updateComboBox(2, &g_env_light.dungeonlight[0].mAngleAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[1].mAngleAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[2].mAngleAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[3].mAngleAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[4].mAngleAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[5].mAngleAttenuation, 0);

    mctx->updateComboBox(2, &g_env_light.dungeonlight[0].mDistAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[1].mDistAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[2].mDistAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[3].mDistAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[4].mDistAttenuation, 0);
    mctx->updateComboBox(2, &g_env_light.dungeonlight[5].mDistAttenuation, 0);

    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mCutoffAngle, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mCutoffAngle, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mCutoffAngle, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mCutoffAngle, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mCutoffAngle, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mCutoffAngle, 0.0f, 0.0f, 0);

    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mAngleX, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mAngleX, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mAngleX, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mAngleX, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mAngleX, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mAngleX, 0.0f, 0.0f, 0);

    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mAngleY, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mAngleY, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mAngleY, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mAngleY, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mAngleY, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mAngleY, 0.0f, 0.0f, 0);

    mctx->updateSlider(2, &g_env_light.dungeonlight[0].mRefDistance, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[1].mRefDistance, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[2].mRefDistance, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[3].mRefDistance, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[4].mRefDistance, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_env_light.dungeonlight[5].mRefDistance, 0.0f, 0.0f, 0);

    mctx->updateSlider(2, &g_kankyoHIO.navy.terrain_height_crr, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_kankyoHIO.navy.influence_multiplier, 0.0f, 0.0f, 0);
    mctx->updateSlider(2, &g_kankyoHIO.navy.cutoff_multiplier, 0.0f, 0.0f, 0);

    releaseJORMContext(mctx);
}

void dKankyo_lightHIO_c::listenPropertyEvent(const JORPropertyEvent* property) {
    // DEBUG NONMATCHING
}

dKankyo_vrboxHIO_c::dKankyo_vrboxHIO_c() {
    m_VrboxSetting = 0;
    field_0x5 = 0;
    field_0x7 = 0;
    field_0x8 = 0;
    field_0x9 = 0;
    field_0xa = 0;
    field_0xb = 0;
    field_0xc = 0;
    field_0xd = 0;
    field_0xe = -1;
    field_0xf = -1;
    field_0x10 = -1;
    field_0x11 = -1;
    field_0x12 = -1;
    field_0x13 = -1;
    field_0x14 = 0;
    m_horizonHeight = 0.0f;
}

void dKankyo_vrboxHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

void dKankyo_vrboxHIO_c::dKankyo_vrboxHIOInfoUpDateF() {
    // DEBUG NONMATCHING
}

void dKankyo_vrboxHIO_c::listenPropertyEvent(const JORPropertyEvent* property) {
    // DEBUG NONMATCHING
}

dKankyo_bloomHIO_c::dKankyo_bloomHIO_c() {
    field_0x4 = 0;
    m_saturationPattern = 0;
    field_0x5 = 0;

    for (int i = 0; i < 64; i++) {
        dKydata_BloomInfo_c* bloominf = dKyd_BloomInf_tbl_getp(i);
        bloom_info[i] = bloominf->info;
    }
}

void dKankyo_bloomHIO_c::listenPropertyEvent(const JORPropertyEvent* property) {
    // DEBUG NONMATCHING
}

void dKankyo_bloomHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

void dKankyo_dungeonlightHIO_c::listenPropertyEvent(const JORPropertyEvent* property) {}

dKankyo_navyHIO_c::dKankyo_navyHIO_c() {
    // DEBUG NONMATCHING
}

void dKankyo_navyHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

dKankyo_efflightHIO_c::dKankyo_efflightHIO_c() {
    // DEBUG NONMATCHING
}

void dKankyo_efflightHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

dKankyo_windHIO_c::dKankyo_windHIO_c() {
    // DEBUG NONMATCHING
}

dKankyo_demolightHIO_c::dKankyo_demolightHIO_c() {
    // DEBUG NONMATCHING
}

void dKankyo_demolightHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

void dKankyo_windHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

dKankyo_dungeonlightHIO_c::dKankyo_dungeonlightHIO_c() {
    // DEBUG NONMATCHING
}

void dKankyo_dungeonlightHIO_c::genMessage(JORMContext* mctx) {}

dKankyo_ParticlelightHIO_c::dKankyo_ParticlelightHIO_c() {
    // DEBUG NONMATCHING
}

void dKankyo_ParticlelightHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

void dKankyo_ParticlelightHIO_c::listenPropertyEvent(const JORPropertyEvent* property) {
    // DEBUG NONMATCHING
}

dKankyo_HIO_c::dKankyo_HIO_c() {
    light.m_displayTVColorSettings = FALSE;
    vrbox.m_displayVrboxTVColorSettings = FALSE;
}

void dKankyo_HIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

#endif

void dScnKy_env_light_c::drawKankyo() {
    setSunpos();
    SetBaseLight();
    setLight();
    dKy_setLight_nowroom(g_env_light.PrevCol);
}

void dKy_undwater_filter_draw() {
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    cXyz size;
    f32 var_f30 = 60.0f;

    static cXyz S_old_cameye(0.0f, 0.0f, 0.0f);
    static cXyz S_old_camctr(0.0f, 0.0f, 0.0f);

    if (dKy_camera_water_in_status_check()) {
        f32 dist_to_old = camera->lookat.eye.abs(S_old_cameye);
        if (dist_to_old > 100.0f) {
            dist_to_old = 100.0f;
        }

        if (g_env_light.field_0x1258 > 1.0f - (dist_to_old / 100.0f)) {
            cLib_addCalc(&g_env_light.field_0x1258, 1.0f - (dist_to_old / 100.0f), 0.5f, 0.05f,
                         0.0001f);
        }

        dist_to_old = camera->lookat.center.abs(S_old_camctr);
        if (dist_to_old > 100.0f) {
            dist_to_old = 100.0f;
        }

        if (g_env_light.field_0x1258 > 1.0f - (dist_to_old / 100.0f)) {
            cLib_addCalc(&g_env_light.field_0x1258, 1.0f - (dist_to_old / 100.0f), 0.5f, 0.05f,
                         0.0001f);
        }

        S_old_cameye = camera->lookat.eye;
        S_old_camctr = camera->lookat.center;

        mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);

        size.x = 0.00524f;
        size.y = 0.00524f;
        size.z = 0.00524f;

        #if !PLATFORM_GCN
        size.x *= mDoGph_gInf_c::getScale();
        #endif

        if (g_env_light.undwater_ef_model != NULL) {
            J3DGXColor sp8;
            g_env_light.undwater_ef_model->setBaseScale(size);
            g_env_light.undwater_ef_model->setBaseTRMtx(mDoMtx_stack_c::get());

            if (g_env_light.undwater_btk != NULL) {
                g_env_light.undwater_btk->setPlaySpeed(2.0f + (35.0f * (1.0f - g_env_light.field_0x1258)));
                g_env_light.undwater_btk->entry(g_env_light.undwater_ef_model->getModelData());
                g_env_light.undwater_btk->play();

                dComIfGd_setXluList2DScreen();
                mDoExt_modelUpdateDL(g_env_light.undwater_ef_model);
                dComIfGd_setList();
            }
        }
    }
}

static int dKy_Draw(sub_kankyo__class* i_this) {
    (void)i_this;
    g_env_light.drawKankyo();

    #if DEBUG
    if ((dDebugPad.Enable(1) || g_kankyoHIO.field_0x15) && g_kankyoHIO.field_0x17) {
        dKydb_HIO_debug_draw();
    }

    if (dDebugPad.Enable(2)) {
        dKydb_HIO_winddebug_draw();
    }

    dKydb_winddisp_draw();
    dKydb_dungeonlight_draw();
    #endif

    return 1;
}

static int dKy_Execute(sub_kankyo__class* i_this) {
    (void)i_this;
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    g_env_light.exeKankyo();
    dKyw_wind_set();
    dKy_twilight_camelight_set();

    #if DEBUG
    if (g_kankyoHIO.display_schedule_bit) {
        dDbVw_Report(40, 80, "schbit[0x%02x] timer[%d]", dKy_get_schbit(), dKy_get_schbit_timer());
    }

    if (!dDebugPad.Enable(1)) {
        // NOTE: a g_kankyoHIO load is generated here but nothing is done with it
        g_kankyoHIO.display_schedule_bit;
    }

    if (dDebugPad.Enable(2)) {
        dKydb_HIO_debug_Wind();
    }

    if (g_kankyoHIO.effect_light_monitor) {
        dKydb_efplight_monitor();
    }

    if (g_kankyoHIO.point_light_monitor) {
        dKydb_plight_monitor();
    }

    debug_actor_create();
    #endif

    return 1;
}

static int dKy_IsDelete(sub_kankyo__class* i_this) {
    return 1;
}

static int dKy_Delete(sub_kankyo__class* i_this) {
    (void)i_this;
    plight_init();
    mDoAud_mEnvse_resetScene();

    if (g_env_light.undwater_ef_heap != NULL) {
        mDoExt_destroySolidHeap(g_env_light.undwater_ef_heap);
    }

    #if DEBUG
    mDoHIO_DELETE_CHILD(g_kankyoHIO.field_0x1a);
    #endif

    return 1;
}

static int dKy_Create(void* i_this) {
    (void)i_this;
    BOOL next_time_set = false;

    stage_envr_info_class* stage_envr_p = dComIfGp_getStageEnvrInfo();
    if (stage_envr_p != NULL && dComIfGp_getStartStageRoomNo() != -1) {
        stage_envr_p += dComIfGp_getStartStageRoomNo();
        if (stage_envr_p->pselect_id[64] != 0) {
            mDoGph_gInf_c::getBloom()->create();

            #if DEBUG
            OS_REPORT("\n-----------------------------------");
            OS_REPORT("\n飽和加算用にＲＡＭを確保しました！ ");
            OS_REPORT("\n-----------------------------------");
            #endif
        }
    }

    envcolor_init();

    #if DEBUG
    g_kankyoHIO.field_0x1a = mDoHIO_CREATE_CHILD("環境", &g_kankyoHIO);
    #endif

    g_env_light.field_0x12b0 = 0;
    g_env_light.field_0x1254 = 1.0f;
    g_env_light.plight_near_pos.x = 0.0f;
    g_env_light.plight_near_pos.y = 0.0f;
    g_env_light.plight_near_pos.z = 0.0f;
    g_env_light.senses_mode = 0;
    g_env_light.senses_effect_strength = 0.0f;
    g_env_light.now_senses_effect = 0;

    dKy_setLight_init();
    dKy_Sound_init();
    dKyw_wind_set();
    dungeonlight_init();
    dKy_setLight_nowroom(dComIfGp_roomControl_getStayNo());

    #if DEBUG
    g_kankyoHIO.navy.field_0x22a = 0;
    #endif

    g_env_light.pol_efftbl = (dKy_pol_efftbl_struct*)dComIfG_getStageRes("pol_efftbl.dat");
    g_env_light.pol_effcol = (dKy_pol_effcol_struct*)dComIfG_getStageRes("pol_effcol.dat");
    g_env_light.pol_efftbl2 = (dKy_pol_efftbl_struct*)dComIfG_getStageRes("pol_efftbl2.dat");
    g_env_light.pol_effcol2 = (dKy_pol_effcol_struct*)dComIfG_getStageRes("pol_effcol2.dat");
    g_env_light.pol_sound = (dKy_pol_sound_struct*)dComIfG_getStageRes("pol_sound.dat");
    g_env_light.pol_arg = (dKy_pol_arg_struct*)dComIfG_getStageRes("pol_arg.dat");
    g_env_light.colordata_tbl = (dKy_color_data_struct*)dComIfG_getStageRes("colordata_tbl.dat");

    if (g_env_light.nexttime != -1.0f) {
        next_time_set = true;
        dComIfGs_setTime(g_env_light.nexttime);
    }

    if (dKy_darkworld_check()) {
        if (g_env_light.old_time == -1.0f) {
            g_env_light.old_time = dComIfGs_getTime();
        }
    } else if (g_env_light.old_time != -1.0f) {
        if (!next_time_set) {
            dComIfGs_setTime(g_env_light.old_time);
        }
        g_env_light.old_time = -1.0f;
    }

    g_env_light.nexttime = -1.0f;

    #if DEBUG
    if (strcmp(dComIfGp_getStartStageName(), "T_SASA0") == 0) {
        dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[142]);
        dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[167]);
    }
    #endif

    return cPhs_COMPLEATE_e;
}

void dKy_setLight_init() {
    for (int i = 0; i < 8; i++) {
        lightStatusData[i] = lightStatusBase;
    }
}

void dKy_setLight() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    kankyo->sun_light_pos = kankyo->sun_pos;
}

void dKy_GlobalLight_set() {
    MtxP view_mtx = j3dSys.getViewMtx();

    for (int i = 0; i < 8; i++) {
        LightStatus* light_status = &lightStatusData[i];

        Vec light_pos;
        cMtx_multVec(view_mtx, &light_status->position, &light_pos);

        GXLightObj light_obj;
        GXInitLightPos(&light_obj, light_pos.x, light_pos.y, light_pos.z);
        GXInitLightDir(&light_obj, light_status->mLightDir.x, light_status->mLightDir.y,
                       light_status->mLightDir.z);
        GXInitLightColor(&light_obj, light_status->color);

        if (light_status->mRefDist <= 0.0f) {
            light_status->mRefDist = 0.000001f;
        }

        GXInitLightDistAttn(&light_obj, light_status->mRefDist, light_status->mRefBrightness,
                            light_status->mDistFn);
        GXInitLightSpot(&light_obj, light_status->mCutoff, light_status->mSpotFn);
        GXLoadLightObjImm(&light_obj, (GXLightID)lightMaskData[i]);
    }
}

inline int dLVI_getSWLight(stage_pure_lightvec_info_class* i_info) {
    return i_info->flags & 0x80;
}

BOOL dKy_lightswitch_check(stage_pure_lightvec_info_class* stage_light_info_p, char room_no) {
    BOOL is_switch = TRUE;

    if (stage_light_info_p->switch_no != 0xFF) {
        if (!dLVI_getSWLight(stage_light_info_p)) {
            if (dComIfGs_isSwitch(stage_light_info_p->switch_no, room_no)) {
                is_switch = FALSE;
            }
        } else if (!dComIfGs_isSwitch(stage_light_info_p->switch_no, room_no)) {
            is_switch = FALSE;
        }
    }

    return is_switch;
}

void dKy_setLight_nowroom_common(char room_no, f32 light_ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    MtxP view_mtx = j3dSys.getViewMtx();
    dKy_tevstr_c* room_tevstr = dComIfGp_roomControl_getTevStr(room_no);

    if (dComIfGp_roomControl_getStatusRoomDt(room_no) != NULL) {
        stage_pure_lightvec_info_class* room_light_info = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLightVecInfo();
        int room_light_info_num = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLightVecInfoNum();
        if (room_light_info_num > 6) {
            room_light_info_num = 6;
        }

        #if DEBUG
        if (g_kankyoHIO.light.m_HOSTIO_setting) {
            room_light_info_num = g_kankyoHIO.dungeonLight.usedLights;
        }
        #endif

        lightMask = 0;

        for (int i = 0; i < room_light_info_num; i++) {
            if (room_light_info != NULL) {
                lightMask |= lightMaskData[i + 2];
            }
            room_light_info++;
        }

        room_light_info = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLightVecInfo();
        if (dKy_SunMoon_Light_Check() == TRUE && room_light_info == NULL) {
            lightMask |= lightMaskData[2];
            lightMask |= lightMaskData[3];
            room_light_info_num = 2;
        }

        if (g_env_light.bgparts_active_light[0].mIndex != 0) {
            lightMask |= 1;
        }

        if (g_env_light.bgparts_active_light[1].mIndex != 0) {
            lightMask |= 2;
        }

        if (camera != 0) {
            int eflight_id = dKy_eflight_influence_id(camera->lookat.eye, 0);
            if (eflight_id >= 0) {
                dKy_bgparts_activelight_set(g_env_light.efplight[eflight_id], 1);
                if (dKy_Indoor_check() == TRUE) {
                    g_env_light.field_0x10a0 = g_env_light.efplight[eflight_id]->mPosition;
                    dKy_shadow_mode_set(1);
                }
            } else {
                dKy_bgparts_activelight_cut(1);
                if (dKy_shadow_mode_check(1)) {
                    dKy_shadow_mode_reset(1);
                }
            }
        }

        for (int i = 0; i < 2; i++) {
            if (g_env_light.bgparts_active_light[i].mIndex != 0 &&
                0.0f != g_env_light.bgparts_active_light[i].mPow &&
                (i != 1 || dMeter2Info_getWindowStatus() != 3))
            {
                lightStatusPt[i].position = g_env_light.bgparts_active_light[i].mPosition;

                if (i == 0) {
                    lightStatusPt[i].mRefDist = g_env_light.bgparts_active_light[i].mPow * 0.01f;
                } else {
                    lightStatusPt[i].mRefDist = g_env_light.bgparts_active_light[i].mPow * 0.01f;
                }

                lightStatusPt[i].mRefBrightness = 0.99999f;
                lightStatusPt[i].field_0x1c = 1;
                lightStatusPt[i].mDistFn = GX_DA_STEEP;
                lightStatusPt[i].mSpotFn = GX_SP_OFF;
                lightStatusPt[i].color.r = g_env_light.bgparts_active_light[i].mColor.r;
                lightStatusPt[i].color.g = g_env_light.bgparts_active_light[i].mColor.g;
                lightStatusPt[i].color.b = g_env_light.bgparts_active_light[i].mColor.b;
            } else {
                lightStatusPt[i].color.r = 0;
                lightStatusPt[i].color.g = 0;
                lightStatusPt[i].color.b = 0;
            }
        }

        for (int i = 0; i < 6; i++) {
            if (room_light_info != 0) {
                if (i < room_light_info_num) {
                    lightStatusPt[i + 2].position.x = room_light_info[i].position.x;
                    lightStatusPt[i + 2].position.y = room_light_info[i].position.y;
                    lightStatusPt[i + 2].position.z = room_light_info[i].position.z;

                    if (dKy_lightswitch_check(&room_light_info[i], room_no) == TRUE) {
                        lightStatusPt[i + 2].mRefDist = room_light_info[i].radius;
                    } else {
                        lightStatusPt[i + 2].mRefDist = 0.000001f;
                    }

                    lightStatusPt[i + 2].mRefBrightness = 0.99999f;
                    lightStatusPt[i + 2].field_0x1c = 0;
                    lightStatusPt[i + 2].mSpotFn = (GXSpotFn)room_light_info[i].spot_type;
                    lightStatusPt[i + 2].mDistFn = (GXDistAttnFn)room_light_info[i].dist_atten_type;
                    lightStatusPt[i + 2].mCutoff = room_light_info[i].spotCutoff;
                    dKy_lightdir_set(room_light_info[i].directionX, room_light_info[i].directionY,
                                     &((&lightStatusPt[i] + 2))->mLightDir);
                } else {
                    lightStatusPt[i + 2].mRefDist = 0.0f;
                    lightStatusPt[i + 2].mRefBrightness = 0.99999f;
                    lightStatusPt[i + 2].field_0x1c = 0;
                    lightStatusPt[i + 2].mDistFn = GX_DA_OFF;
                    lightStatusPt[i + 2].mSpotFn = GX_SP_OFF;
                    lightStatusPt[i + 2].mCutoff = 90.0f;
                }
            }

            if (dKy_SunMoon_Light_Check() == TRUE && i <= 1) {
                lightMask |= lightMaskData[2];
                lightMask |= lightMaskData[3];

                if (i == 0) {
                    ((&lightStatusPt[i] + 2))->position = kankyo->sun_pos;
                } else if (camera != 0) {
                    ((&lightStatusPt[i] + 2))->position = camera->lookat.eye + kankyo->moon_pos;
                } else {
                    ((&lightStatusPt[i] + 2))->position = kankyo->moon_pos;
                }

                lightStatusPt[i + 2].mRefDist = 10000.0f;
                lightStatusPt[i + 2].mRefBrightness = 0.99999f;
                lightStatusPt[i + 2].field_0x1c = 1;
                lightStatusPt[i + 2].mDistFn = GX_DA_STEEP;
                lightStatusPt[i + 2].mSpotFn = GX_SP_OFF;
            }

            if (lightMask & lightMaskData[i + 2]) {
                if (room_tevstr == NULL) {
                    JUT_ASSERT(12633, FALSE);
                }

                J3DLightInfo* room_light = &room_tevstr->mLights[i].getLightInfo();
                if (room_light != NULL) {
                    ((&lightStatusPt[i] + 2))->color =
                        dKy_light_influence_col(&room_light->mColor, light_ratio);
                } else {
                    ((&lightStatusPt[i] + 2))->color =
                        dKy_light_influence_col(&g_env_light.dungeonlight[i].mColor, light_ratio);
                }

                if (room_no == dComIfGp_roomControl_getStayNo() && room_light_info != NULL &&
                    i < room_light_info_num)
                {
                    g_env_light.dungeonlight[i].mPosition = ((&lightStatusPt[i] + 2))->position;
                    g_env_light.dungeonlight[i].mRefDistance = lightStatusPt[i + 2].mRefDist;
                    g_env_light.dungeonlight[i].mCutoffAngle = lightStatusPt[i + 2].mCutoff;
                    g_env_light.dungeonlight[i].mAngleAttenuation = lightStatusPt[i + 2].mSpotFn;
                    g_env_light.dungeonlight[i].mDistAttenuation = lightStatusPt[i + 2].mDistFn;
                    g_env_light.dungeonlight[i].mAngleX = room_light_info[i].directionX;
                    g_env_light.dungeonlight[i].mAngleY = room_light_info[i].directionY;
                }
            } else {
                lightStatusPt[i + 2].color.r = 0;
                lightStatusPt[i + 2].color.g = 0;
                lightStatusPt[i + 2].color.b = 0;
            }
        }

        for (int i = 0; i < 6; i++) {
            if (kankyo->field_0x0c18[i].field_0x26 == 1) {
                for (int j = 0; j < 6; j++) {
                    if (!(lightMask & lightMaskData[j + 2])) {
                        lightMask |= lightMaskData[j + 2];

                        if (kankyo->field_0x0c18[i].mColor.a == 254 ||
                            kankyo->field_0x0c18[i].mColor.a == 253)
                        {
                            dKy_twi_wolflight_set(i);
                        }

                        if (room_tevstr->Type < 16) {
                            lightStatusPt[j + 2].position.x = kankyo->field_0x0c18[i].mPos.x;
                            lightStatusPt[j + 2].position.y = kankyo->field_0x0c18[i].mPos.y;
                            lightStatusPt[j + 2].position.z = kankyo->field_0x0c18[i].mPos.z;
                        } else {
                            lightStatusPt[j + 2].position.x = kankyo->field_0x0c18[i].mPos.x;
                            if (i == 0) {
                                lightStatusPt[j + 2].position.y =
                                    kankyo->field_0x0c18[i].mPos.y + g_env_light.field_0x127c;
                            } else {
                                lightStatusPt[j + 2].position.y =
                                    200.0f + kankyo->field_0x0c18[i].mPos.y;
                            }
                            lightStatusPt[j + 2].position.z = kankyo->field_0x0c18[i].mPos.z;
                        }

                        ((&lightStatusPt[j] + 2))->color = dKy_light_influence_col(&kankyo->field_0x0c18[i].mColor, light_ratio);
                        lightStatusPt[j + 2].mRefDist = kankyo->field_0x0c18[i].mRefDistance;
                        lightStatusPt[j + 2].mRefBrightness = 0.99999f;
                        lightStatusPt[j + 2].field_0x1c = 1;
                        lightStatusPt[j + 2].mSpotFn = (GXSpotFn)kankyo->field_0x0c18[i].mAngleAttenuation;
                        lightStatusPt[j + 2].mDistFn = (GXDistAttnFn)kankyo->field_0x0c18[i].mDistAttenuation;
                        lightStatusPt[j + 2].mCutoff = kankyo->field_0x0c18[i].mCutoffAngle;

                        dKy_lightdir_set(kankyo->field_0x0c18[i].mAngleX,
                                         kankyo->field_0x0c18[i].mAngleY,
                                         &((&lightStatusPt[j] + 2))->mLightDir);
                        break;
                    }
                }
            }
        }

        dKy_GlobalLight_set();
    }
}

void dKy_setLight_nowroom(char room_no) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    MtxPtr var_r28 = j3dSys.getViewMtx();

    if (camera != NULL) {
        dKy_setLight_nowroom_common(room_no, 1.0f);
    }
}

void dKy_setLight_nowroom_grass(char room_no, f32 light_ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    MtxP view_mtx = j3dSys.getViewMtx();

    dStage_FileList_dt_c* filelist = NULL;
    if (dComIfGp_roomControl_getStatusRoomDt(room_no) != NULL) {
        filelist = dComIfGp_roomControl_getStatusRoomDt(room_no)->getFileListInfo();
        if (filelist == NULL) {
            return;
        }
    } else {
        return;
    }

    int grass_light = dStage_FileList_dt_GRASSLIGHT(filelist);
    #if DEBUG
    if (g_kankyoHIO.navy.grass_light_debug) {
        grass_light = g_kankyoHIO.navy.grass_light_influence_ratio;
    } else if (room_no == dComIfGp_roomControl_getStayNo()) {
        g_kankyoHIO.navy.grass_light_influence_ratio = grass_light;
    }
    #endif

    f32 grass_light_ratio;
    if (grass_light >= 0xFF) {
        grass_light_ratio = 1.0f;
    } else {
        grass_light_ratio = grass_light / 100.0f;
    }

    if (light_ratio == 0.0f) {
        dKy_setLight_nowroom_common(room_no, grass_light_ratio);
    } else {
        dKy_setLight_nowroom_common(room_no, light_ratio);
    }
}

f32 dKy_move_room_ratio(dKy_tevstr_c* tevstr_p, s8* room_no_p) {
    f32 ratio = 1.0f;

    if (memcmp(dComIfGp_getStartStageName(), "D_MN", 4) == 0 && g_env_light.mColPatMode == 0 &&
        g_env_light.mColPatModeGather == 0 && tevstr_p->pat_ratio > 0.0f &&
        tevstr_p->pat_ratio < 1.0f)
    {
        if (tevstr_p->pat_ratio < 0.5f) {
            if (tevstr_p->YukaCol == 0xFF) {
                *room_no_p = tevstr_p->PrevCol;
                ratio = fabsf(2.0f * (0.5f - tevstr_p->pat_ratio));
            } else {
                *room_no_p = tevstr_p->room_no;
                ratio = fabsf(2.0f * (tevstr_p->pat_ratio - 0.5f));
            }
        } else {
            *room_no_p = tevstr_p->room_no;
            ratio = fabsf(2.0f * (tevstr_p->pat_ratio - 0.5f));
        }

        if (ratio <= 0.0f) {
            ratio = 0.000001f;
        }
    }

    return ratio;
}

void dKy_setLight_nowroom_actor(dKy_tevstr_c* tevstr_p) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    Vec light_pos;
    Vec sp3C;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    MtxP view_mtx = j3dSys.getViewMtx();

    s8 room_no = tevstr_p->room_no;
    f32 light_ratio = 1.0f;

    if (tevstr_p->Type == 9 || tevstr_p->Type == 10) {
        light_ratio = dKy_move_room_ratio(tevstr_p, &room_no);
    }

    if (tevstr_p->Type == 14 || tevstr_p->Type == 12 || tevstr_p->Type == 13) {
        return;
    }

    if (dComIfGp_roomControl_getStatusRoomDt(room_no) != NULL) {
        J3DLightInfo* light;
        stage_pure_lightvec_info_class* room_lights = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLightVecInfo();
        int room_light_num = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLightVecInfoNum();
        if (room_light_num > 6) {
            room_light_num = 6;
        }

        #if DEBUG
        if (g_kankyoHIO.light.m_HOSTIO_setting) {
            room_light_num = g_kankyoHIO.dungeonLight.usedLights;
        }
        #endif

        lightMask = 0;

        for (int i = 0; i < room_light_num; i++) {
            if (room_lights != NULL) {
                lightMask |= lightMaskData[i + 2];
            }
            room_lights++;
        }

        room_lights = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLightVecInfo();

        for (int i = 0; i < 6; i++) {
            light = &tevstr_p->mLights[i].getLightInfo();
            if (view_mtx != NULL) {
                if (room_lights != 0 && i < room_light_num) {
                    sp3C.x = room_lights[i].position.x;
                    sp3C.y = room_lights[i].position.y;
                    sp3C.z = room_lights[i].position.z;

                    dKy_GXInitLightSpot(light, room_lights[i].spotCutoff, room_lights[i].spot_type);

                    f32 var_f29;
                    if (dKy_lightswitch_check(&room_lights[i], room_no) == TRUE) {
                        var_f29 = light_ratio * room_lights[i].radius;
                    } else {
                        var_f29 = 0.000001f;
                    }

                    dKy_GXInitLightDistAttn(light, var_f29, 0.99999f,
                                            room_lights[i].dist_atten_type);
                    dKy_lightdir_set(room_lights[i].directionX, room_lights[i].directionY,
                                     &light->mLightDirection);
                    light->mLightDirection.x = -light->mLightDirection.x;
                    light->mLightDirection.y = -light->mLightDirection.y;
                    light->mLightDirection.z = -light->mLightDirection.z;
                }

                if (dKy_SunMoon_Light_Check() == TRUE && i <= 1) {
                    lightMask |= lightMaskData[2];
                    lightMask |= lightMaskData[3];

                    if (i == 0) {
                        sp3C.x = kankyo->sun_pos.x;
                        sp3C.y = kankyo->sun_pos.y;
                        sp3C.z = kankyo->sun_pos.z;
                    } else if (camera != NULL) {
                        sp3C.x = camera->lookat.eye.x + kankyo->moon_pos.x;
                        sp3C.y = camera->lookat.eye.y + kankyo->moon_pos.y;
                        sp3C.z = camera->lookat.eye.z + kankyo->moon_pos.z;
                    } else {
                        sp3C.x = kankyo->moon_pos.x;
                        sp3C.y = kankyo->moon_pos.y;
                        sp3C.z = kankyo->moon_pos.z;
                    }

                    dKy_GXInitLightSpot(light, 90.0f, GX_SP_OFF);
                    dKy_GXInitLightDistAttn(light, 10000.0f, 0.99999f, GX_DA_STEEP);
                }

                cMtx_multVec(view_mtx, &sp3C, &light_pos);
                light->mLightPosition = light_pos;

                if (1.0f != light_ratio) {
                    light->mColor.r *= light_ratio;
                    light->mColor.g *= light_ratio;
                    light->mColor.b *= light_ratio;
                }
            }

            if (!(lightMask & lightMaskData[i + 2])) {
                light->mColor.r = 0;
                light->mColor.g = 0;
                light->mColor.b = 0;
            }

            light = &tevstr_p->mLightObj.getLightInfo();
            if (1.0f != light_ratio) {
                light->mColor.r *= light_ratio;
                light->mColor.g *= light_ratio;
                light->mColor.b *= light_ratio;
            }
        }

        for (int i = 0; i < 6; i++) {
            if (kankyo->field_0x0c18[i].field_0x26 == 1 && view_mtx != NULL) {
                for (int j = 0; j < 6; j++) {
                    if (!(lightMask & lightMaskData[j + 2])) {
                        lightMask |= lightMaskData[j + 2];

                        light = &tevstr_p->mLights[j].getLightInfo();
                        if (kankyo->field_0x0c18[i].mColor.a == 254 ||
                            kankyo->field_0x0c18[i].mColor.a == 253)
                        {
                            dKy_twi_wolflight_set(i);
                        }

                        light->mColor = dKy_light_influence_col(&kankyo->field_0x0c18[i].mColor,
                                                                tevstr_p->field_0x374);

                        if (tevstr_p->Type < 16) {
                            sp3C.x = kankyo->field_0x0c18[i].mPos.x;
                            sp3C.y = kankyo->field_0x0c18[i].mPos.y;
                            sp3C.z = kankyo->field_0x0c18[i].mPos.z;

                            if (kankyo->field_0x0c18[i].mColor.a == 253) {
                                light->mColor.r = light->mColor.r * 0.65f;
                                light->mColor.g = light->mColor.g * 0.65f;
                                light->mColor.b = light->mColor.b * 0.65f;
                            }
                        } else {
                            sp3C.x = kankyo->field_0x0c18[i].mPos.x;
                            if (i == 0) {
                                sp3C.y = kankyo->field_0x0c18[i].mPos.y + g_env_light.field_0x127c;
                            } else {
                                sp3C.y = 200.0f + kankyo->field_0x0c18[i].mPos.y;
                            }
                            sp3C.z = kankyo->field_0x0c18[i].mPos.z;
                        }

                        dKy_GXInitLightSpot(light, kankyo->field_0x0c18[i].mCutoffAngle,
                                            kankyo->field_0x0c18[i].mAngleAttenuation);
                        f32 var_f30 = kankyo->field_0x0c18[i].mRefDistance * light_ratio;
                        if (var_f30 <= 0.0f) {
                            var_f30 = 0.000001f;
                        }

                        dKy_GXInitLightDistAttn(light, var_f30, 0.99999f,
                                                kankyo->field_0x0c18[i].mDistAttenuation);
                        cMtx_multVec(view_mtx, &sp3C, &light_pos);
                        light->mLightPosition = light_pos;

                        dKy_lightdir_set(kankyo->field_0x0c18[i].mAngleX,
                                         kankyo->field_0x0c18[i].mAngleY, &light->mLightDirection);
                        light->mLightDirection.x = -light->mLightDirection.x;
                        light->mLightDirection.y = -light->mLightDirection.y;
                        light->mLightDirection.z = -light->mLightDirection.z;
                        break;
                    }
                }
            }
        }
    }
}

void dKy_setLight_again() {
    dKy_GlobalLight_set();
}

void dKy_Global_amb_set(dKy_tevstr_c* tevstr_p) {
    GXColor color;
    color.r = tevstr_p->AmbCol.r;
    color.g = tevstr_p->AmbCol.g;
    color.b = tevstr_p->AmbCol.b;
    color.a = tevstr_p->AmbCol.a;

    if (tevstr_p->Type == 2 || tevstr_p->Type == 3) {
        if (dKy_darkworld_check()) {
            if (tevstr_p->Type == 2) {
                color.r = 18;
                color.g = 18;
                color.b = 18;
                color.a = 0;
            } else {
                color.r = 12;
                color.g = 12;
                color.b = 12;
                color.a = 0;
            }
        }
    } else if ((tevstr_p->Type >= 1 && tevstr_p->Type <= 7) ||
               (tevstr_p->Type == 9 && dKy_darkworld_check()))
    {
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 0;
    }

    GXSetChanAmbColor(GX_COLOR0A0, color);
}

cXyz dKy_light_influence_pos(int light_id) {
    if (light_id < 0) {
        light_id = 0;
    }

    return g_env_light.pointlight[light_id]->mPosition;
}

cXyz dKy_plight_near_pos() {
    return g_env_light.plight_near_pos;
}

void dKy_mock_light_every_set(LIGHT_INFLUENCE* light_inf_p) {
    int i;
    BOOL var_r30 = 0;

    for (i = 50; i < 100; i++) {
        if (g_env_light.pointlight[i] == NULL) {
            var_r30 = 1;
            g_env_light.pointlight[i] = light_inf_p;
            g_env_light.pointlight[i]->mIndex = i + 1;
            break;
        }
    }

    #if DEBUG
    if (i >= 100 && !var_r30) {
        OS_WARNING("\nPOINTLIGHT2 COUNT OVER!!!\n");
    }
    #endif
}

void dKy_plight_set(LIGHT_INFLUENCE* light_inf_p) {
    int i;
    BOOL var_r30 = 0;

    for (i = 0; i < 100; i++) {
        if (g_env_light.pointlight[i] == light_inf_p) {
            return;
        }
    }

    for (i = 0; i < 50; i++) {
        if (g_env_light.pointlight[i] == NULL && !var_r30) {
            var_r30 = 1;
            g_env_light.pointlight[i] = light_inf_p;
            g_env_light.pointlight[i]->mIndex = i + 1;
            break;
        }
    }

    #if DEBUG
    if (i >= 100 && !var_r30) {
        OS_WARNING("\nPOINTLIGHT COUNT OVER!!!\n");
    }
    #endif
}

void dKy_dalkmist_inf_set(DALKMIST_INFLUENCE* dalkmist_inf_p) {
    int i;
    for (i = 0; i < 10; i++) {
        if (g_env_light.dalkmist_influence[i] == NULL) {
            g_env_light.dalkmist_influence[i] = dalkmist_inf_p;
            g_env_light.dalkmist_influence[i]->mIndex = i;
            break;
        }
    }
}

void dKy_dalkmist_inf_cut(DALKMIST_INFLUENCE* dalkmist_inf_p) {
    if (dalkmist_inf_p != NULL) {
        if (dalkmist_inf_p->mIndex < 10) {
            g_env_light.dalkmist_influence[dalkmist_inf_p->mIndex] = NULL;
        }
    }
}

void dKy_plight_priority_set(LIGHT_INFLUENCE* light_inf_p) {
    int i = 0;

    for (; i < 50; i++) {
        if (g_env_light.pointlight[i] == NULL) {
            g_env_light.pointlight[i] = light_inf_p;
            g_env_light.pointlight[i]->mIndex = -(i + 1);
            break;
        }
    }

    if (i >= 50) {
        OSReport_Warning("\nPOINTLIGHT COUNT OVER!!!\n");
    }
}

void dKy_plight_cut(LIGHT_INFLUENCE* light_inf_p) {
    int idx;
    if (light_inf_p == NULL) {
        OS_REPORT("\nd_kankyo.cpp dKy_plight_cut NULL!!");
    } else if (light_inf_p->mIndex == 0) {
        OS_REPORT("\x1b[46m\nポイントライト空削除!\n\x1b[m");
    } else {
        if (light_inf_p->mIndex < 0) {
            light_inf_p->mIndex *= -1;
        }

        idx = (light_inf_p->mIndex & 0xFFF) - 1;
        if (idx < 50) {
            g_env_light.pointlight[idx] = NULL;
        }
    }
}

void dKy_efplight_set(LIGHT_INFLUENCE* light_inf_p) {
    int i;
    for (i = 0; i < 5; i++) {
        if (g_env_light.efplight[i] == light_inf_p) {
            return;
        }
    }

    for (i = 0; i < 5; i++) {
        if (g_env_light.efplight[i] == NULL) {
            g_env_light.efplight[i] = light_inf_p;
            g_env_light.efplight[i]->mIndex = i + 1;
            break;
        }
    }

    #if DEBUG
    if (i >= 5) {
        OS_REPORT("\nEffectLIGHT COUNT OVER!!!");
    }
    #endif
}

void dKy_efplight_cut(LIGHT_INFLUENCE* light_inf_p) {
    int idx;
    if (light_inf_p == NULL) {
        OS_REPORT("\nd_kankyo.cpp dKy_plight_cut NULL!!");
    } else if (light_inf_p->mIndex == 0) {
        OS_WARNING("\nEFLIGHT CUT nomi!\n");
    } else {
        idx = light_inf_p->mIndex - 1;
        if (idx >= 0 && idx < 5) {
            g_env_light.efplight[idx] = NULL;
        }
    }
}

void dKy_bgparts_activelight_set(LIGHT_INFLUENCE* light_inf_p, int bgpart_id) {
    if (light_inf_p != NULL) {
        memcpy(&g_env_light.bgparts_active_light[bgpart_id], light_inf_p, sizeof(LIGHT_INFLUENCE));
        g_env_light.bgparts_active_light[bgpart_id].mIndex = bgpart_id + 1;
    } else {
        JUT_ASSERT(13758, FALSE);
    }
}

void dKy_bgparts_activelight_cut(int bgpart_id) {
    g_env_light.bgparts_active_light[bgpart_id].mIndex = 0;
}

void dKy_actor_addcol_amb_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.actor_addcol_amb.r = r * ratio;
    g_env_light.actor_addcol_amb.g = g * ratio;
    g_env_light.actor_addcol_amb.b = b * ratio;
}

void dKy_bg_addcol_amb_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.bg_addcol_amb.r = r * ratio;
    g_env_light.bg_addcol_amb.g = g * ratio;
    g_env_light.bg_addcol_amb.b = b * ratio;
}

void dKy_bg1_addcol_amb_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.bg1_addcol_amb.r = r * ratio;
    g_env_light.bg1_addcol_amb.g = g * ratio;
    g_env_light.bg1_addcol_amb.b = b * ratio;
}

void dKy_bg2_addcol_amb_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.bg2_addcol_amb.r = r * ratio;
    g_env_light.bg2_addcol_amb.g = g * ratio;
    g_env_light.bg2_addcol_amb.b = b * ratio;
}

void dKy_bg3_addcol_amb_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.bg3_addcol_amb.r = r * ratio;
    g_env_light.bg3_addcol_amb.g = g * ratio;
    g_env_light.bg3_addcol_amb.b = b * ratio;
}

void dKy_addcol_fog_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.addcol_fog.r = r * ratio;
    g_env_light.addcol_fog.g = g * ratio;
    g_env_light.addcol_fog.b = b * ratio;
}

void dKy_actor_addcol_set(s16 r, s16 g, s16 b, f32 ratio) {
    dKy_actor_addcol_amb_set(r, g, b, ratio);
    dKy_bg_addcol_amb_set(r, g, b, ratio);
    dKy_bg1_addcol_amb_set(r, g, b, ratio);
    dKy_bg2_addcol_amb_set(r, g, b, ratio);
    dKy_bg3_addcol_amb_set(r, g, b, ratio);
}

void dKy_vrbox_addcol_sky0_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.vrbox_addcol_sky0.r = r * ratio;
    g_env_light.vrbox_addcol_sky0.g = g * ratio;
    g_env_light.vrbox_addcol_sky0.b = b * ratio;
}

void dKy_vrbox_addcol_kasumi_set(s16 r, s16 g, s16 b, f32 ratio) {
    g_env_light.vrbox_addcol_kasumi.r = r * ratio;
    g_env_light.vrbox_addcol_kasumi.g = g * ratio;
    g_env_light.vrbox_addcol_kasumi.b = b * ratio;
}

void dKy_vrbox_addcol_set(s16 r, s16 g, s16 b, f32 ratio) {
    dKy_vrbox_addcol_sky0_set(r, g, b, ratio);
    dKy_vrbox_addcol_kasumi_set(r, g, b, ratio);
    dKy_addcol_fog_set(r, g, b, ratio);
}

void dKy_fog_startendz_set(f32 param_0, f32 param_1, f32 ratio) {
    if (ratio < 0.0f || ratio > 1.0f) {
        OSReport_Warning("\ndKy_fog_startendz_set ratio error!\n");
        ratio = 0.0f;
    }

    if (ratio < 0.0001f) {
        ratio = 0.0f;
    }

    g_env_light.field_0x11ec = param_0;
    g_env_light.field_0x11f0 = param_1;
    g_env_light.field_0x11f4 = ratio;
}

void dKy_Itemgetcol_chg_on() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    OS_REPORT("\ndKy_Itemgetcol_chg_on[%d]", kankyo->Itemgetcol_chg);

    if (kankyo->Itemgetcol_chg == 0 || kankyo->Itemgetcol_chg == 6) {
        kankyo->Itemgetcol_chg = 1;
    }
}

void dKy_Sound_init() {
    g_env_light.sound_influence.position.x = 999999.9f;
    g_env_light.sound_influence.position.y = 999999.9f;
    g_env_light.sound_influence.position.z = 999999.9f;
    g_env_light.sound_influence.field_0xc = 0;
    g_env_light.sound_influence.actor_id = fpcM_ERROR_PROCESS_ID_e;
    g_env_light.sound_influence.timer = 0;
}

void dKy_Sound_set(cXyz pos, int param_1, fpc_ProcID actor_id, int timer) {
    camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
    BOOL set_sound = FALSE;
    f32 dist_pos_to_eye = pos.abs(camera_p->lookat.eye);
    f32 dist_sndpos_to_eye = g_env_light.sound_influence.position.abs(camera_p->lookat.eye);

    if (dist_pos_to_eye < dist_sndpos_to_eye) {
        if (dist_sndpos_to_eye < 1500.0f) {
            set_sound = TRUE;
        } else if (g_env_light.sound_influence.field_0xc < param_1) {
            set_sound = TRUE;
        }
    }

    if (set_sound) {
        g_env_light.sound_influence.position = pos;
        g_env_light.sound_influence.field_0xc = param_1;
        g_env_light.sound_influence.actor_id = actor_id;
        g_env_light.sound_influence.timer = timer;
    }
}

SND_INFLUENCE* dKy_Sound_get() {
    return &g_env_light.sound_influence;
}

void dKy_SordFlush_set(cXyz light_pos, int light_type) {
    dScnKy_env_light_c* light = dKy_getEnvlight();
    EF_THUNDER* thunder = &light->mThunderEff;

    if (!dKy_darkworld_check() && (thunder->mState >= 10 || thunder->mFlashTimer <= 0.0f)) {
        if (g_env_light.eflight.mState == 0) {
            g_env_light.eflight.mState = 1;
            g_env_light.eflight.mLightType = light_type;
            g_env_light.eflight.field_0x8.mPosition = light_pos;
        } else if (g_env_light.eflight.mState == 2) {
            g_env_light.eflight.mState = 4;
        }
    }
}

static void GxFogSet_Sub(GXColor* fog_col_p) {
    f32 near_z = 1.0f;
    f32 far_z = 160000.0f;
    GXColor color;
    color = *fog_col_p;

    if (dComIfGd_getView() != NULL) {
        if (dComIfGd_getView()->near >= 0.0f && dComIfGd_getView()->far >= 0.0f) {
            if (dComIfGd_getView()->near < dComIfGd_getView()->far) {
                near_z = dComIfGd_getView()->near;
                far_z = dComIfGd_getView()->far;
            }
        }
    }

    GXSetFog(GX_FOG_PERSP_LIN, g_env_light.mFogNear, g_env_light.mFogFar, near_z, far_z, color);
}

static void GxFog_set() {
    GXColor color;
    color.r = g_env_light.fog_col.r;
    color.g = g_env_light.fog_col.g;
    color.b = g_env_light.fog_col.b;

    GxFogSet_Sub(&color);
}

void dKy_GxFog_set() {
    GxFog_set();
    GxXFog_set();
}

void dKy_GxFog_tevstr_set(dKy_tevstr_c* tevstr_p) {
    f32 near_z = 1.0f;
    f32 far_z = 160000.0f;

    GXColor color;
    color.r = tevstr_p->FogCol.r;
    color.g = tevstr_p->FogCol.g;
    color.b = tevstr_p->FogCol.b;

    if (dComIfGd_getView() != NULL) {
        if (dComIfGd_getView()->near >= 0.0f && dComIfGd_getView()->far >= 0.0f) {
            if (dComIfGd_getView()->near < dComIfGd_getView()->far) {
                near_z = dComIfGd_getView()->near;
                far_z = dComIfGd_getView()->far;
            }
        }
    }

    GXSetFog(GX_FOG_PERSP_LIN, tevstr_p->mFogStartZ, tevstr_p->mFogEndZ, near_z, far_z, color);
    GxXFog_set();
}

void dKy_GfFog_tevstr_set(dKy_tevstr_c* tevstr_p) {
    f32 near_z = 1.0f;
    f32 far_z = 160000.0f;

    GXColor color;
    color.r = tevstr_p->FogCol.r;
    color.g = tevstr_p->FogCol.g;
    color.b = tevstr_p->FogCol.b;

    if (dComIfGd_getView() != NULL) {
        if (dComIfGd_getView()->near >= 0.0f && dComIfGd_getView()->far >= 0.0f) {
            if (dComIfGd_getView()->near < dComIfGd_getView()->far) {
                near_z = dComIfGd_getView()->near;
                far_z = dComIfGd_getView()->far;
            }
        }
    }

    GFSetFog(GX_FOG_PERSP_LIN, tevstr_p->mFogStartZ, tevstr_p->mFogEndZ, near_z, far_z, color);
    GxXFog_set();
}

static void GxXFog_set() {
    dKyd_xfog_table_set(g_env_light.mFogAdjTableType);
    GXSetFogRangeAdj(g_env_light.mFogAdjEnable, g_env_light.mFogAdjCenter, &g_env_light.mXFogTbl);
}

void dKy_change_colpat(u8 colpat) {
    g_env_light.mColpatCurrGather = colpat;
    if (g_env_light.wether_pat1 != g_env_light.mColpatCurrGather) {
        g_env_light.mColPatBlendGather = 0.0f;
    }
}

void dKy_custom_colset(u8 prevGather, u8 curGather, f32 blend) {
    #if DEBUG
    if (g_kankyoHIO.light.m_HOSTIO_setting) {
        return;
    }
    #endif

    if (g_env_light.light_init_timer != 1) {
        if (blend < 1.0f) {
            g_env_light.mColpatPrevGather = prevGather;
        } else {
            g_env_light.mColpatPrevGather = curGather;
        }

        g_env_light.mColpatCurrGather = curGather;
        g_env_light.mColPatBlendGather = blend;
        g_env_light.mColPatModeGather = 1;
    }
}

void dKy_setLight_mine(dKy_tevstr_c* tevstr_p) {
    GXLightObj light;
    J3DLightInfo& info = tevstr_p->mLightObj.getLightInfo();

    GXInitLightPos(&light, info.mLightPosition.x, info.mLightPosition.y, info.mLightPosition.z);
    GXInitLightDir(&light, info.mLightDirection.x, info.mLightDirection.y, info.mLightDirection.z);
    GXInitLightColor(&light, info.mColor);
    GXInitLightAttn(&light, info.mCosAtten.x,
                    info.mCosAtten.y, info.mCosAtten.z,
                    info.mDistAtten.x, info.mDistAtten.y,
                    info.mDistAtten.z);
    GXLoadLightObjImm(&light, (GXLightID)*lightMaskData);
}

void dKy_tevstr_init(dKy_tevstr_c* tevstr_p, s8 room_no, u8 floorCol) {
    J3DLightInfo* light_info;
    memset(tevstr_p, 0, sizeof(dKy_tevstr_c));

    if (room_no == -1) {
        tevstr_p->room_no = dComIfGp_roomControl_getStayNo();
    } else {
        tevstr_p->room_no = room_no;
    }

    tevstr_p->UseCol = tevstr_p->room_no;
    tevstr_p->PrevCol = tevstr_p->room_no;
    tevstr_p->YukaCol = floorCol;
    tevstr_p->mInitTimer = 1;
    tevstr_p->Material_use_fg = 0x7B;
    tevstr_p->AmbCol.a = 0;
    tevstr_p->FogCol.a = 255;
    tevstr_p->Material_id = 0;
    tevstr_p->field_0x374 = 1.0f;
    tevstr_p->field_0x384 = 0;

    light_info = &tevstr_p->mLightObj.getLightInfo();
    light_info->mColor.g = 0;
    light_info->mColor.b = 0;
    light_info->mColor.a = 255;
    light_info->mCosAtten.x = 1.0f;
    light_info->mCosAtten.y = 0.0f;
    light_info->mCosAtten.z = 0.0f;
    light_info->mDistAtten.x = 1.0f;
    light_info->mDistAtten.y = 0.0f;
    light_info->mDistAtten.z = 0.0f;

    int i;
    for (i = 0; i < 6; i++) {
        light_info = &tevstr_p->mLights[i].getLightInfo();

        light_info->mLightPosition = lightStatusData[i].position;
        light_info->mColor = lightStatusData[i].color;
        light_info->mCosAtten.x = 1.0f;
        light_info->mCosAtten.y = 0.0f;
        light_info->mCosAtten.z = 0.0f;
        light_info->mDistAtten.x = 1.0f;
        light_info->mDistAtten.y = 0.0f;
        light_info->mDistAtten.z = 0.0f;
    }
}

u8 dKy_get_schbit() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    return kankyo->schbit;
}

int dKy_get_schbit_timer() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    return kankyo->schbit_timer;
}

int dKy_rain_check() {
    return g_env_light.raincnt;
}

void dKy_set_allcol_ratio(f32 ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    kankyo->allcol_ratio = ratio;
}

void dKy_set_actcol_ratio(f32 ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    kankyo->actcol_ratio = ratio;
}

void dKy_set_bgcol_ratio(f32 ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    kankyo->bgcol_ratio = ratio;
}

void dKy_set_fogcol_ratio(f32 ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    g_env_light.fogcol_ratio = ratio;
}

void dKy_set_vrboxcol_ratio(f32 ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    dKy_set_vrboxsoracol_ratio(ratio);
    dKy_set_vrboxkumocol_ratio(ratio);
}

void dKy_set_vrboxsoracol_ratio(f32 ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    kankyo->vrboxsoracol_ratio = ratio;
}

void dKy_set_vrboxkumocol_ratio(f32 ratio) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    kankyo->vrboxkumocol_ratio = ratio;
}

void dKy_itudemo_se() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    int room_no = dComIfGp_roomControl_getStayNo();

    if (kankyo->moya_se != 0) {
        mDoAud_seStart(kankyo->moya_se, NULL, 0, 0);
    }
}

int dKy_get_dayofweek() {
    u16 date = dComIfGs_getDate();
    int day_of_week = date % 7;
    return day_of_week;
}

void dKy_set_nexttime(f32 next_time) {
    OS_REPORT("\n\n\nnext_time=[%f]\n\n\n", next_time);
    #if DEBUG
    if (next_time > 360.0f) {
        JUT_ASSERT(14615, FALSE);
    }
    #endif

    g_env_light.nexttime = next_time;
}

void dKy_instant_timechg(f32 timechg) {
    f32 time = g_env_light.getDaytime();
    if (timechg < time) {
        dComIfGs_setDate(dComIfGs_getDate() + 1);
        dKankyo_DayProc();
    }

    dComIfGs_setTime(timechg);
}

void dKy_instant_rainchg() {
    dKyw_rain_set(250);
    g_env_light.mColpatWeather = 1;
    g_env_light.wether_pat0 = 1;
    g_env_light.wether_pat1 = 1;
}

static GXColor NewAmbColGet(GXColorS10* in_col_p) {
    GXColorS10 sp18, sp10;
    f32 temp_f31;
    sp18 = *in_col_p;

    sp10.r = sp18.r * 4;
    sp10.g = sp18.g * 4;
    sp10.b = sp18.b * 4;

    if (sp10.r > 0xFF) {
        temp_f31 = 255.0f / sp10.r;
        sp10.r *= temp_f31;
        sp10.g *= temp_f31;
        sp10.b *= temp_f31;
    }

    if (sp10.g > 0xFF) {
        temp_f31 = 255.0f / sp10.g;
        sp10.r *= temp_f31;
        sp10.g *= temp_f31;
        sp10.b *= temp_f31;
    }

    if (sp10.b > 0xFF) {
        temp_f31 = 255.0f / sp10.b;
        sp10.r *= temp_f31;
        sp10.g *= temp_f31;
        sp10.b *= temp_f31;
    }

    GXColor amb_col;
    amb_col.r = sp10.r;
    amb_col.g = sp10.g;
    amb_col.b = sp10.b;
    amb_col.a = 0xFF;

    return amb_col;
}

static leafdraw_method_class l_dKy_Method = {
    (process_method_func)dKy_Create,  (process_method_func)dKy_Delete,
    (process_method_func)dKy_Execute, (process_method_func)dKy_IsDelete,
    (process_method_func)dKy_Draw,
};

extern kankyo_process_profile_definition g_profile_KANKYO = {
    fpcLy_CURRENT_e,
    1,
    fpcPi_CURRENT_e,
    PROC_KANKYO,
    &g_fpcLf_Method.base,
    sizeof(sub_kankyo__class),
    0,
    0,
    &g_fopKy_Method,
    2,
    &l_dKy_Method,
};

// fixes float literal order
static f32 dummy() {
    return 50.0f;
}

void dKy_ParticleColor_get_base(cXyz* param_0, dKy_tevstr_c* param_1, GXColor* param_2,
                                GXColor* param_3, GXColor* param_4, GXColor* param_5, f32 param_6) {
    GXColor sp70[3];
    cXyz sp64;
    f32 sp58[] = {100000000.0f, 100000000.0f, 100000000.0f};
    int i;
    u8 spD = 0;
    s8 room_no;

    GXColorS10 sp48;
    J3DLightInfo* sp44;
    GXColor sp40;
    int sp3C;
    int sp38;
    stage_pure_lightvec_info_class* stage_light_info_p;
    int sp30;

    for (i = 0; i < 6; i++) {
        if (param_1 != NULL) {
            sp44 = &param_1->mLights[i].getLightInfo();
            sp40 = sp44->mColor;
        } else {
            sp38 = dComIfGp_roomControl_getStayNo();
            if (dKy_SunMoon_Light_Check() == TRUE && i <= 1) {
                sp40 = g_env_light.dungeonlight[i].mColor;
            } else if (dComIfGp_roomControl_getStatusRoomDt(sp38) != NULL) {
                stage_light_info_p =
                    dComIfGp_roomControl_getStatusRoomDt(sp38)->getLightVecInfo();
                sp30 = dComIfGp_roomControl_getStatusRoomDt(sp38)->getLightVecInfoNum();
                if (i < sp30) {
                    sp40 = g_env_light.dungeonlight[i].mColor;
                } else {
                    sp40.r = 0;
                    sp40.g = 0;
                    sp40.b = 0;
                }
            }
        }

        f32 var_f31;
        if (dKy_SunMoon_Light_Check() == TRUE && i <= 1) {
            if (g_env_light.daytime >= 90.0f && g_env_light.daytime <= 285.0f) {
                if (i == 0) {
                    var_f31 = 1.0f;
                } else {
                    var_f31 = 2.0f;
                }
            } else if (i == 0) {
                var_f31 = 2.0f;
            } else {
                var_f31 = 1.0f;
            }

            if (strcmp(dComIfGp_getStartStageName(), "F_SP114") == 0) {
                if (sp40.r < 15.0f) {
                    sp40.r = 15;
                }

                if (sp40.g < 17.0f) {
                    sp40.g = 17;
                }

                if (sp40.b < 20.0f) {
                    sp40.b = 20;
                }
            }
        } else if (sp40.r != 0 || sp40.g != 0 || sp40.b != 0) {
            stage_pure_lightvec_info_class* stage_light_info_p;
            if (param_1 != NULL && param_1->room_no >= 0) {
                stage_light_info_p = dComIfGp_roomControl_getStatusRoomDt(param_1->room_no)
                                         ->getLightVecInfo();
                sp3C = dComIfGp_roomControl_getStatusRoomDt(param_1->room_no)
                           ->getLightVecInfoNum();
            } else {
                stage_light_info_p = dComIfGp_roomControl_getStatusRoomDt(dComIfGp_roomControl_getStayNo())->getLightVecInfo();
                sp3C = dComIfGp_roomControl_getStatusRoomDt(dComIfGp_roomControl_getStayNo())->getLightVecInfoNum();
            }

            f32 var_f27;
            f32 var_f26;
            if (stage_light_info_p != 0) {
                if (i < sp3C) {
                    sp64.x = stage_light_info_p[i].position.x;
                    sp64.y = stage_light_info_p[i].position.y;
                    sp64.z = stage_light_info_p[i].position.z;

                    if (param_1 != NULL && param_1->room_no >= 0) {
                        room_no = param_1->room_no;
                    } else {
                        room_no = dComIfGp_roomControl_getStayNo();
                    }

                    if (dKy_lightswitch_check(&stage_light_info_p[i], room_no) == TRUE) {
                        var_f27 = 190.0f * stage_light_info_p[i].radius;
                    } else {
                        var_f27 = 0.0f;
                    }
                } else {
                    sp64.x = lightStatusPt[i + 2].position.x;
                    sp64.y = lightStatusPt[i + 2].position.y;
                    sp64.z = lightStatusPt[i + 2].position.z;
                    var_f27 = 190.0f * lightStatusPt[i + 2].mRefDist;
                }
            } else {
                sp64.x = 100000000.0f;
                sp64.y = 100000000.0f;
                sp64.z = 100000000.0f;
                var_f27 = 0.0f;
            }

            var_f31 = param_0->abs(sp64);
            if (var_f31 <= 0.0f) {
                var_f31 = 0.0000000000001f;
            }

            if (var_f27 > 0.0f) {
                var_f26 = var_f27 / var_f31;
            } else {
                var_f26 = 0.0f;
            }

            if (var_f26 < 1.0f) {
                sp40.r *= var_f26;
                sp40.g *= var_f26;
                sp40.b *= var_f26;
            } else {
                var_f31 = 5.0f;
            }
        }

        if (sp58[0] > var_f31) {
            sp58[2] = sp58[1];
            sp58[1] = sp58[0];
            sp58[0] = var_f31;
            sp70[2] = sp70[1];
            sp70[1] = sp70[0];
            sp70[0] = sp40;
        } else if (sp58[1] > var_f31) {
            sp58[2] = sp58[1];
            sp58[1] = var_f31;
            sp70[2] = sp70[1];
            sp70[1] = sp40;
        } else if (sp58[2] > var_f31) {
            sp58[2] = var_f31;
            sp70[2] = sp40;
        }
    }

    sp48.r = 0;
    sp48.g = 0;
    sp48.b = 0;

    for (int i = 0; i < 3; i++) {
        if (sp58[i] < 100000000.0f) {
            spD++;
        }
    }

    static f32 parcent_tabel[][3] = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.7f, 0.3f, 0.0f},
        {0.6f, 0.3f, 0.1f},
    };

    if (sp58[0] < 100000000.0f) {
        f32 temp_f28 = parcent_tabel[spD][0];
        sp48.r = sp70[0].r * temp_f28;
        sp48.g = sp70[0].g * temp_f28;
        sp48.b = sp70[0].b * temp_f28;
    }

    for (int i = 1; i < 3; i++) {
        if (sp58[i] < 100000000.0f) {
            f32 temp_f28 = parcent_tabel[spD][i];
            sp48.r += (s16)(sp70[i].r * temp_f28);
            sp48.g += (s16)(sp70[i].g * temp_f28);
            sp48.b += (s16)(sp70[i].b * temp_f28);
        }
    }

    *param_3 = NewAmbColGet(&sp48);

    sp48.r = (0.8f * param_2->r) + (1.2f * param_3->r);
    sp48.g = (0.8f * param_2->g) + (1.2f * param_3->g);
    sp48.b = (0.8f * param_2->b) + (1.2f * param_3->b);

    if (sp48.r > 0xFF) {
        f32 temp_f30 = 255.0f / sp48.r;
        sp48.r *= temp_f30;
        sp48.g *= temp_f30;
        sp48.b *= temp_f30;
    }

    if (sp48.g > 0xFF) {
        f32 temp_f30 = 255.0f / sp48.g;
        sp48.r *= temp_f30;
        sp48.g *= temp_f30;
        sp48.b *= temp_f30;
    }

    if (sp48.b > 0xFF) {
        f32 temp_f30 = 255.0f / sp48.b;
        sp48.r *= temp_f30;
        sp48.g *= temp_f30;
        sp48.b *= temp_f30;
    }

    GXColorS10 sp50;
    sp50.r = sp48.r & 0xFF;
    sp50.g = sp48.g & 0xFF;
    sp50.b = sp48.b & 0xFF;

    f32 sp20 = param_4->r / 255.0f;
    f32 sp1C = param_4->g / 255.0f;
    f32 sp18 = param_4->b / 255.0f;

    param_2->r = (sp20 * (sp50.r * (1.0f - param_6))) + (param_4->r * param_6);
    param_2->g = (sp1C * (sp50.g * (1.0f - param_6))) + (param_4->g * param_6);
    param_2->b = (sp18 * (sp50.b * (1.0f - param_6))) + (param_4->b * param_6);

    sp20 = (param_5->r / 255.0f);
    sp1C = (param_5->g / 255.0f);
    sp18 = (param_5->b / 255.0f);

    param_3->r = (sp20 * (sp50.r * (1.0f - param_6))) + (param_5->r * param_6);
    param_3->g = (sp1C * (sp50.g * (1.0f - param_6))) + (param_5->g * param_6);
    param_3->b = (sp18 * (sp50.b * (1.0f - param_6))) + (param_5->b * param_6);
}

// Dummy data to force 0x30 bytes of padding between parcent_table and dScnKy_env_light's vtable.
// This extra space is likely allocated for stripped vtables - debug indicates that dBgS_ObjGndChk
// is used somewhere which has a 0x30 byte vtable, but I can't find a way to generate it without
// also reserving space for cBgS_GrpPassChk and cBgS_PolyPassChk which ends up making .data too
// large.
static u8 dummy_padding_data_0x354[0x30] = {};

void dKy_ParticleColor_get_actor(cXyz* param_0, dKy_tevstr_c* tevstr_p, GXColor* param_2,
                                 GXColor* param_3, GXColor* param_4, GXColor* param_5,
                                 f32 param_6) {
    if (tevstr_p != NULL) {
        *param_2 = NewAmbColGet(&tevstr_p->AmbCol);
    } else {
        *param_2 = NewAmbColGet(&g_env_light.actor_amb_col);
    }

    dKy_ParticleColor_get_base(param_0, tevstr_p, param_2, param_3, param_4, param_5, param_6);
}

void dKy_ParticleColor_get_bg(cXyz* param_0, dKy_tevstr_c* tevstr_p, GXColor* param_2,
                              GXColor* param_3, GXColor* param_4, GXColor* param_5, f32 param_6) {
    *param_2 = NewAmbColGet(&g_env_light.bg_amb_col[0]);
    dKy_ParticleColor_get_base(param_0, tevstr_p, param_2, param_3, param_4, param_5, param_6);
}

int dKy_BossLight_set(cXyz* pos_p, GXColor* color_p, f32 ref_dist, u8 param_3) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    int sp10 = 0;
    f32 var_f31 = 0.0f;
    u8 spA = 0xFF;
    int stage_light_info_num = 0;
    int i;

    if (strcmp(dComIfGp_getStartStageName(), "F_SP118") != 0) {
        u8 room_no = dComIfGp_roomControl_getStayNo();
        if (dComIfGp_roomControl_getStatusRoomDt(room_no) == NULL) {
            return 0;
        }

        stage_light_info_num = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLightVecInfoNum();
        if (stage_light_info_num < 0 || stage_light_info_num > 6) {
            stage_light_info_num = 6;
        }
    }

    BOSS_LIGHT* boss_light_p;
    if (param_3 == 0) {
        boss_light_p = kankyo->field_0x0c18;
    } else {
        boss_light_p = kankyo->field_0x0d58;
    }

    for (i = 1; i < 6 - stage_light_info_num; i++) {
        if (boss_light_p[i].field_0x26 == 1 && pos_p->x == boss_light_p[i].mPos.x &&
            pos_p->y == boss_light_p[i].mPos.y && pos_p->z == boss_light_p[i].mPos.z)
        {
            return 0;
        }
    }

    for (i = 1; i < 6 - stage_light_info_num; i++) {
        if (boss_light_p[i].field_0x26 == 0) {
            boss_light_p[i].mPos = *pos_p;
            boss_light_p[i].mColor.r = color_p->r;
            boss_light_p[i].mColor.g = color_p->g;
            boss_light_p[i].mColor.b = color_p->b;
            boss_light_p[i].mColor.a = 0xFF;
            boss_light_p[i].mRefDistance = ref_dist;
            boss_light_p[i].field_0x14 = 0.99f;
            boss_light_p[i].mCutoffAngle = 90.0f;
            boss_light_p[i].mAngleX = 0.0f;
            boss_light_p[i].mAngleY = 0.0f;
            boss_light_p[i].mAngleAttenuation = GX_SP_OFF;
            boss_light_p[i].mDistAttenuation = GX_DA_STEEP;
            boss_light_p[i].field_0x26 = 1;
            sp10 = 1;
            break;
        }
    }

    for (i = 1; i < 6 - stage_light_info_num; i++) {
        if (boss_light_p[i].field_0x26 != 0) {
            cXyz sp1C(boss_light_p[i].mPos);

            if (strcmp(dComIfGp_getStartStageName(), "D_SB03") == 0) {
                sp1C.x = camera->lookat.eye.x;
                sp1C.z = camera->lookat.eye.z;

                if (sp1C.abs(camera->lookat.eye) > 2000.0f) {
                    var_f31 = 99999.9f;
                    spA = i;
                    break;
                } else {
                    if (boss_light_p[i].mPos.abs(camera->lookat.eye) > var_f31) {
                        var_f31 = boss_light_p[i].mPos.abs(camera->lookat.eye);
                        spA = i;
                    }
                }
            } else {
                if (boss_light_p[i].mPos.abs(camera->lookat.eye) > var_f31) {
                    var_f31 = boss_light_p[i].mPos.abs(camera->lookat.eye);
                    spA = i;
                }
            }
        }
    }

    if (sp10 == 0 && spA != 0xFF) {
        i = spA;
        if (i < 6 && pos_p->abs(camera->lookat.eye) < var_f31) {
            boss_light_p[i].mPos = *pos_p;
            boss_light_p[i].mColor.r = color_p->r;
            boss_light_p[i].mColor.g = color_p->g;
            boss_light_p[i].mColor.b = color_p->b;
            boss_light_p[i].mColor.a = 0xFF;
            boss_light_p[i].mRefDistance = ref_dist;
            boss_light_p[i].field_0x14 = 0.99f;
            boss_light_p[i].mCutoffAngle = 90.0f;
            boss_light_p[i].mAngleX = 0.0f;
            boss_light_p[i].mAngleY = 0.0f;
            boss_light_p[i].mAngleAttenuation = GX_SP_OFF;
            boss_light_p[i].mDistAttenuation = GX_DA_STEEP;
            boss_light_p[i].field_0x26 = 1;
            sp10 = 1;
        }
    }

    return sp10;
}

int dKy_BossSpotLight_set(cXyz* pos_p, f32 angle_x, f32 angle_y, f32 cutoff, GXColor* color_p,
                          f32 ref_dist, u8 spot_type, u8 distattn_type) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    int spC = 0;
    f32 var_f31 = 0.0f;
    u8 sp8 = 0xFF;
    int i;

    for (i = 1; i < 6; i++) {
        if (kankyo->field_0x0c18[i].field_0x26 == 0) {
            kankyo->field_0x0c18[i].mPos = *pos_p;
            kankyo->field_0x0c18[i].mColor.r = color_p->r;
            kankyo->field_0x0c18[i].mColor.g = color_p->g;
            kankyo->field_0x0c18[i].mColor.b = color_p->b;
            kankyo->field_0x0c18[i].mColor.a = 0xFF;
            kankyo->field_0x0c18[i].mRefDistance = ref_dist;
            kankyo->field_0x0c18[i].field_0x14 = 0.99f;
            kankyo->field_0x0c18[i].mCutoffAngle = cutoff;
            kankyo->field_0x0c18[i].mAngleX = angle_x;
            kankyo->field_0x0c18[i].mAngleY = angle_y + 90.0f;
            kankyo->field_0x0c18[i].mAngleAttenuation = spot_type;
            kankyo->field_0x0c18[i].mDistAttenuation = distattn_type;
            kankyo->field_0x0c18[i].field_0x26 = 1;
            spC = 1;
            break;
        }
    }

    if (spC == 0) {
        for (i = 1; i < 6; i++) {
            if (kankyo->field_0x0c18[i].field_0x26 != 0 &&
                kankyo->field_0x0c18[i].mPos.abs(camera->lookat.eye) > var_f31)
            {
                var_f31 = kankyo->field_0x0c18[i].mPos.abs(camera->lookat.eye);
                sp8 = i;
            }
        }

        if (sp8 != 0xFF) {
            i = sp8;
            if (i < 6 && pos_p->abs(camera->lookat.eye) <
                             kankyo->field_0x0c18[i].mPos.abs(camera->lookat.eye))
            {
                kankyo->field_0x0c18[i].mPos = *pos_p;
                kankyo->field_0x0c18[i].mColor.r = color_p->r;
                kankyo->field_0x0c18[i].mColor.g = color_p->g;
                kankyo->field_0x0c18[i].mColor.b = color_p->b;
                kankyo->field_0x0c18[i].mColor.a = 0xFF;
                kankyo->field_0x0c18[i].mRefDistance = ref_dist;
                kankyo->field_0x0c18[i].field_0x14 = 0.99f;
                kankyo->field_0x0c18[i].mCutoffAngle = cutoff;
                kankyo->field_0x0c18[i].mAngleX = angle_x;
                kankyo->field_0x0c18[i].mAngleY = angle_y + 90.0f;
                kankyo->field_0x0c18[i].mAngleAttenuation = spot_type;
                kankyo->field_0x0c18[i].mDistAttenuation = distattn_type;
                kankyo->field_0x0c18[i].field_0x26 = 1;
                spC = 1;
            }
        }
    }

    return spC;
}

int dKy_WolfEyeLight_set(cXyz* pos_p, f32 angle_x, f32 angle_y, f32 cutoff, GXColor* color_p,
                         f32 ref_dist, u8 spot_type, u8 distattn_type) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();

    kankyo->field_0x0c18[0].mPos = *pos_p;

    kankyo->field_0x0c18[0].mColor.r = color_p->r;
    kankyo->field_0x0c18[0].mColor.g = color_p->g;
    kankyo->field_0x0c18[0].mColor.b = color_p->b;
    kankyo->field_0x0c18[0].mColor.a = 255;

    kankyo->field_0x0c18[0].mRefDistance = ref_dist;
    kankyo->field_0x0c18[0].field_0x14 = 0.99f;
    kankyo->field_0x0c18[0].mCutoffAngle = cutoff;
    kankyo->field_0x0c18[0].mAngleX = angle_x;
    kankyo->field_0x0c18[0].mAngleY = angle_y + 90.0f;
    kankyo->field_0x0c18[0].mAngleAttenuation = spot_type;
    kankyo->field_0x0c18[0].mDistAttenuation = distattn_type;

    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    switch (kankyo->light_size) {
    case LIGHT_SIZE_S:
        var_f31 = 50.0f;
        var_f30 = 1.0f;
        var_f29 = 0.9f;
        break;
    case LIGHT_SIZE_M:
        var_f31 = 200.0f;
        var_f30 = 1.0f;
        var_f29 = 1.0f;
        break;
    case LIGHT_SIZE_L:
        var_f31 = 350.0f;
        var_f30 = 1.5f;
        var_f29 = 1.5f;
        break;
    case LIGHT_SIZE_LL:
    default:
        var_f31 = 1500.0f;
        var_f30 = 4.0f;
        var_f29 = 1.5f;
    }

    kankyo->field_0x127c = var_f31;
    kankyo->field_0x0c18[0].mRefDistance *= var_f30;
    kankyo->field_0x0c18[0].mCutoffAngle *= var_f29;
    kankyo->field_0x0c18[0].field_0x26 = 1;
    return 1;
}

void dKy_twilight_camelight_set() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
    u8 sp8 = 0;

    if (strcmp(dComIfGp_getStartStageName(), "R_SP107") != 0 ||
        dComIfGp_roomControl_getStayNo() != 3 || dComIfGp_getStartStageLayer() != 12)
    {
        if (!dKy_darkworld_check() || memcmp(dComIfGp_getStartStageName(), "D_MN08", 6) == 0) {
            return;
        }

        if (!daPy_py_c::checkNowWolfPowerUp()) {
            for (int i = 0; i < 6; i++) {
                if (kankyo->field_0x0c18[i].field_0x26 != 1) {
                    dKy_twi_wolflight_set(i);
                    kankyo->field_0x0c18[i].field_0x14 = 0.99f;
                    kankyo->field_0x0c18[i].mColor.a = 254;

                    f32 var_f31 = 0.0f;
                    if (player_p != NULL) {
                        f32 var_f30 = camera_p->lookat.eye.y - player_p->current.pos.y;
                        if (var_f30 < 0.0f) {
                            var_f30 = 0.0f;
                        }
                        if (var_f30 > 600.0f) {
                            var_f30 = 600.0f;
                        }
                        var_f31 = var_f30 / 450.0f;
                    }

                    switch (g_env_light.light_size) {
                    case LIGHT_SIZE_S:
                        kankyo->field_0x0c18[i].mColor.r = 0x19;
                        kankyo->field_0x0c18[i].mColor.g = 0x5A;
                        kankyo->field_0x0c18[i].mColor.b = 0xB7;
                        cLib_addCalc(&kankyo->field_0x0c18[i].mRefDistance,
                                     1.0625f + (0.75f * var_f31), 0.2f, 10.0f, 0.0001f);
                        kankyo->field_0x0c18[i].mCutoffAngle = 90.0f;
                        break;
                    case LIGHT_SIZE_M:
                        kankyo->field_0x0c18[i].mColor.r = 0x48;
                        kankyo->field_0x0c18[i].mColor.g = 0x87;
                        kankyo->field_0x0c18[i].mColor.b = 0xCE;
                        cLib_addCalc(&kankyo->field_0x0c18[i].mRefDistance,
                                     1.7f + (0.75f * var_f31), 0.2f, 10.0f, 0.0001f);
                        kankyo->field_0x0c18[i].mCutoffAngle = 70.0f;
                        break;
                    case LIGHT_SIZE_L:
                        kankyo->field_0x0c18[i].mColor.r = 0x48;
                        kankyo->field_0x0c18[i].mColor.g = 0x87;
                        kankyo->field_0x0c18[i].mColor.b = 0xCE;
                        cLib_addCalc(&kankyo->field_0x0c18[i].mRefDistance,
                                     2.5500002f + (0.75f * var_f31), 0.2f, 10.0f, 0.0001f);
                        kankyo->field_0x0c18[i].mCutoffAngle = 70.0f;
                        break;
                    case LIGHT_SIZE_LL:
                        kankyo->field_0x0c18[i].mColor.r = 0x50;
                        kankyo->field_0x0c18[i].mColor.g = 0x87;
                        kankyo->field_0x0c18[i].mColor.b = 0xCE;
                        cLib_addCalc(&kankyo->field_0x0c18[i].mRefDistance,
                                     3.4f + (0.75f * var_f31), 0.2f, 10.0f, 0.0001f);
                        kankyo->field_0x0c18[i].mCutoffAngle = 65.0f;
                        break;
                    }

                    kankyo->field_0x0c18[i].mAngleAttenuation = GX_SP_COS;
                    kankyo->field_0x0c18[i].mDistAttenuation = GX_DA_STEEP;
                    kankyo->field_0x0c18[i].field_0x26 = 1;
                    break;
                }
            }
        }
    }
}

// NONMATCHING - direct g_env_light member loads instead of storing to temps. matches debug, not retail
void dKy_WaterIn_Light_set() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    int sp10 = 0;
    int spC = 0;
    f32 var_f31, var_f30, var_f29, var_f28, var_f27, var_f26;
    f32 var_f25 = -100000000.0f;
    var_f29 = 200.0f;

    int sp8 = 1;
    if (strcmp(dComIfGp_getStartStageName(), "F_SP109") == 0 ||
        strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0 ||
        strcmp(dComIfGp_getStartStageName(), "F_SP122") == 0)
    {
        sp8 = 3;
    }

#if PLATFORM_GCN
#define WATERIN_LIGHT_SET_LOOP_MAX 1
#else
#define WATERIN_LIGHT_SET_LOOP_MAX 6
#endif
    for (int i = 0; i < WATERIN_LIGHT_SET_LOOP_MAX; i++) {
        if (kankyo->field_0x0c18[i].field_0x26 != 1) {
            dKy_twi_wolflight_set(i);
            kankyo->field_0x0c18[i].mColor.r = 0x8A;
            kankyo->field_0x0c18[i].mColor.g = 0xC0;
            kankyo->field_0x0c18[i].mColor.b = 0xBC;
            kankyo->field_0x0c18[i].mColor.a = 0xFD;

            var_f26 = 0.0f;
            if (player_p != NULL) {
                var_f31 = camera->lookat.eye.y - player_p->current.pos.y;
                if (var_f31 < 0.0f) {
                    var_f31 = 0.0f;
                }
                if (var_f31 > 600.0f) {
                    var_f31 = 600.0f;
                }
                var_f26 = var_f31 / 450.0f;
            }

            kankyo->field_0x0c18[i].mRefDistance = 7.2250004f + (0.75f * var_f26);
            kankyo->field_0x0c18[i].mCutoffAngle = 28.0f;
            kankyo->field_0x0c18[i].mAngleAttenuation = 2;
            kankyo->field_0x0c18[i].mDistAttenuation = 3;

            if (strcmp(dComIfGp_getStartStageName(), "D_MN01A") == 0) {
                dBgS_CamGndChk_Wtr sp28;
                cXyz sp1C;
                sp8 = 1;
                kankyo->field_0x0c18[i].mCutoffAngle = 60.0f;

                sp1C = camera->lookat.eye;
                sp1C.y += 100000.0f;
                sp28.SetPos(&sp1C);

                var_f25 = dComIfG_Bgsp().GroundCross(&sp28);
                var_f30 = (var_f25 - camera->lookat.eye.y) / 3000.0f;
                if (var_f30 < 0.0f) {
                    var_f30 = 0.0f;
                }
                if (var_f30 > 1.0f) {
                    var_f30 = 1.0f;
                }
                kankyo->field_0x0c18[i].mRefDistance *= var_f30;
            }

#if DEBUG
            if (g_kankyoHIO.navy.room_light_type != 0) {
                sp8 = g_kankyoHIO.navy.room_light_type - 1;
            }

            switch (sp8) {
            case 0:
                dDbVw_Report(0x176, 0x1c, "W-LS[S]");
                break;
            case 1:
                dDbVw_Report(0x176, 0x1c, "W-LS[M]");
                break;
            case 2:
                dDbVw_Report(0x176, 0x1c, "W-LS[L]");
                break;
            case 3:
                dDbVw_Report(0x176, 0x1c, "W-LS[LL]");
                break;
            }
#endif

            switch (sp8) {
            case 0:
            case 1:
                var_f29 = 200.0f;
                var_f28 = 1.0f;
                var_f27 = 1.0f;
                break;
            case 2:
            case 3:
            default:
                var_f29 = 200.0f;
                var_f28 = 2.0f;
                var_f27 = 1.25f;
                break;
            }

#if DEBUG
            if (g_kankyoHIO.navy.field_0x311 == 0) {
#endif
                kankyo->field_0x127c = var_f29;
                kankyo->field_0x0c18[i].mRefDistance *= var_f28;
                kankyo->field_0x0c18[i].mCutoffAngle *= var_f27;
#if DEBUG
            }

            static int S_mLighSize_pat_old = 0x63;
            if (S_mLighSize_pat_old != g_kankyoHIO.navy.room_light_type) {
                S_mLighSize_pat_old = g_kankyoHIO.navy.room_light_type;

                g_kankyoHIO.navy.terrain_height_crr = var_f29;
                g_kankyoHIO.navy.influence_multiplier = var_f28;
                g_kankyoHIO.navy.cutoff_multiplier = var_f27;
                g_kankyoHIO.light.dKankyo_lightHIOInfoUpDateF();
            }

            if (g_kankyoHIO.navy.field_0x311 != 0) {
                kankyo->field_0x127c = g_kankyoHIO.navy.terrain_height_crr;
                kankyo->field_0x0c18[i].mRefDistance *= g_kankyoHIO.navy.influence_multiplier;
                kankyo->field_0x0c18[i].mCutoffAngle *= g_kankyoHIO.navy.cutoff_multiplier;
            }
#endif

            kankyo->field_0x0c18[i].field_0x26 = 1;
            sp10 = 1;

            break;
        }
    }
}

void dKy_camera_water_in_status_set(u8 status) {
    g_env_light.camera_water_in_status = status;
}

BOOL dKy_camera_water_in_status_check() {
    return g_env_light.camera_water_in_status;
}

u8 dKy_pol_efftype_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    u8 efftype;
    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    if (kankyo->pol_efftbl != 0) {
        efftype = kankyo->pol_efftbl[z0].data[z1];
    } else {
        efftype = 0xFF;
    }

    return efftype;
}

u8 dKy_pol_efftype2_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    u8 efftype;
    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    if (kankyo->pol_efftbl2 != 0) {
        efftype = kankyo->pol_efftbl2[z0].data[z1];
    } else {
        efftype = 0xFF;
    }

    return efftype;
}

u8 dKy_pol_sound_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP127") && g_env_light.fishing_hole_season == 4 &&
        z0 == 4)
    {
        z0 = 13;
    }

    u8 sound;
    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    if (kankyo->pol_sound != NULL) {
        sound = kankyo->pol_sound[z0].data[z1];
    } else {
        sound = 0;
    }

    if (sound == 0xFF) {
        sound = 0;
    }

    return sound;
}

u8 dKy_pol_argument_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP127") && g_env_light.fishing_hole_season == 4 &&
        z0 == 4)
    {
        z0 = 13;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    if (kankyo->pol_arg != NULL) {
        return kankyo->pol_arg[z0].data[z1];
    }

    return 0;
}

u8 dKy_pol_eff_prim_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    if (polyinfo_p == NULL || out_color_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 eff_id;
    if (kankyo->pol_effcol != NULL) {
        eff_id = kankyo->pol_effcol[z0].data[z1];
    } else {
        return 0xFF;
    }

    if (eff_id >= 100) {
        return 0xFF;
    }

    out_color_p->r = kankyo->colordata_tbl[eff_id].data[0];
    out_color_p->g = kankyo->colordata_tbl[eff_id].data[1];
    out_color_p->b = kankyo->colordata_tbl[eff_id].data[2];
    return eff_id;
}

u8 dKy_pol_eff_env_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    if (polyinfo_p == NULL || out_color_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 eff_id;
    if (kankyo->pol_effcol != NULL) {
        eff_id = kankyo->pol_effcol[z0].data[z1];
    } else {
        return 0xFF;
    }

    if (eff_id >= 100) {
        return 0xFF;
    }

    out_color_p->r = kankyo->colordata_tbl[eff_id].data[3];
    out_color_p->g = kankyo->colordata_tbl[eff_id].data[4];
    out_color_p->b = kankyo->colordata_tbl[eff_id].data[5];
    return eff_id;
}

u8 dKy_pol_eff2_prim_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    if (polyinfo_p == NULL || out_color_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 eff_id;
    if (kankyo->pol_effcol2 != NULL) {
        eff_id = kankyo->pol_effcol2[z0].data[z1];
    } else {
        return 0xFF;
    }

    if (eff_id >= 100) {
        return 0xFF;
    }

    out_color_p->r = kankyo->colordata_tbl[eff_id].data[0];
    out_color_p->g = kankyo->colordata_tbl[eff_id].data[1];
    out_color_p->b = kankyo->colordata_tbl[eff_id].data[2];
    return eff_id;
}

u8 dKy_pol_eff2_env_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    if (polyinfo_p == NULL || out_color_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 eff_id;
    if (kankyo->pol_effcol2 != NULL) {
        eff_id = kankyo->pol_effcol2[z0].data[z1];
    } else {
        return 0xFF;
    }

    if (eff_id >= 100) {
        return 0xFF;
    }

    out_color_p->r = kankyo->colordata_tbl[eff_id].data[3];
    out_color_p->g = kankyo->colordata_tbl[eff_id].data[4];
    out_color_p->b = kankyo->colordata_tbl[eff_id].data[5];
    return eff_id;
}

u8 dKy_pol_eff_alpha_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 eff_id;
    if (kankyo->pol_effcol != NULL) {
        eff_id = kankyo->pol_effcol[z0].data[z1];
    } else {
        return 0xFF;
    }

    if (eff_id >= 100) {
        return 0xFF;
    }

    eff_id = kankyo->colordata_tbl[eff_id].data[6];
    return eff_id;
}

f32 dKy_pol_eff_ratio_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    f32 ratio = 0.0f;

    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0.0f;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 eff_id;
    if (kankyo->pol_effcol != NULL) {
        eff_id = kankyo->pol_effcol[z0].data[z1];
    } else {
        return ratio;
    }

    if (eff_id >= 100) {
        return ratio;
    }

    ratio = (f32)kankyo->colordata_tbl[eff_id].data[7];
    ratio = 0.01f * ratio;
    return ratio;
}

u8 dKy_pol_eff2_alpha_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0xFF;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 var_r30;
    if (kankyo->pol_effcol2 != NULL) {
        var_r30 = kankyo->pol_effcol2[z0].data[z1];
    } else {
        return 0xFF;
    }

    if (var_r30 >= 100) {
        return 0xFF;
    }

    var_r30 = kankyo->colordata_tbl[var_r30].data[6];
    return var_r30;
}

f32 dKy_pol_eff2_ratio_get(const cBgS_PolyInfo* polyinfo_p) {
    dScnKy_env_light_c* kankyo = &g_env_light;
    f32 ratio = 0.0f;

    if (polyinfo_p == NULL || &dComIfG_Bgsp() == NULL) {
        return 0.0f;
    }

    int z0 = dComIfG_Bgsp().GetPolyAtt0(*polyinfo_p);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 &&
        g_env_light.fishing_hole_season == 4)
    {
        if (z0 == 4) {
            z0 = 13;
        }
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 &&
               dComIfGs_isStageBossEnemy() && z0 == 11)
    {
        z0 = 7;
    }

    int z1 = dComIfG_Bgsp().GetPolyAtt1(*polyinfo_p);
    u8 eff_id;
    if (kankyo->pol_effcol2 != NULL) {
        eff_id = kankyo->pol_effcol2[z0].data[z1];
    } else {
        return ratio;
    }

    if (eff_id >= 100) {
        return ratio;
    }

    ratio = (f32)kankyo->colordata_tbl[eff_id].data[7];
    ratio = 0.01f * ratio;
    return ratio;
}

BOOL dKy_TeachWind_existence_chk() {
    if (g_env_light.TeachWind_existence == 0xFF) {
        return -1;
    }

    return g_env_light.TeachWind_existence ? TRUE : FALSE;
}

BOOL dKy_SunMoon_Light_Check() {
    BOOL check = false;
    if (g_env_light.mSunInitialized && !dKy_darkworld_check()) {
        // stage is not City in the Sky, Hyrule Castle, or Hidden Skill area
        if (memcmp(dComIfGp_getStartStageName(), "D_MN07", 6) &&
            memcmp(dComIfGp_getStartStageName(), "D_MN09", 6) &&
            strcmp(dComIfGp_getStartStageName(), "F_SP200"))
        {
            check = true;
        }
    }

    return check;
}

BOOL dKy_Outdoor_check() {
    BOOL outdoors = false;
    u32 stage_type = ST_FIELD;

    if (dComIfGp_getStage() != NULL) {
        stage_type = dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo());
    }

    if (stage_type == ST_FIELD) {
        outdoors = true;
    }

    return outdoors;
}

BOOL dKy_Indoor_check() {
    BOOL indoors = false;

    int sttype = dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo());
    if (sttype != ST_FIELD) {
        indoors = true;
    }

    return indoors;
}

BOOL dKy_withwarp_capture_check() {
    return false;
}

void dKy_depth_dist_set(void* process_p) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
    cXyz sp30;
    cXyz sp24;

    fopAc_ac_c* actor_p = (fopAc_ac_c*)process_p;
    sp24 = actor_p->eyePos;

    if (dComIfGp_event_runCheck() && actor_p->tevStr.Type <= 10) {
        if (fabsf(sp24.y - camera_p->lookat.eye.y) < 400.0f) {
            sp24.y = camera_p->lookat.eye.y;
        }

        f32 var_f31 = sp24.abs(camera_p->lookat.eye);
        if (var_f31 < 2000.0f && var_f31 < kankyo->field_0x1268) {
            mDoLib_project(&actor_p->eyePos, &sp30);

            if ((sp30.x >= 0.0f && sp30.x < FB_WIDTH) && (sp30.y >= 0.0f && sp30.y < 600.0f)) {
                cXyz sp18;
                cXyz spC;
                dKyr_get_vectle_calc(&camera_p->lookat.eye, &camera_p->lookat.center, &sp18);
                dKyr_get_vectle_calc(&camera_p->lookat.eye, &actor_p->eyePos, &spC);

                s16 temp_r27 = cM_atan2s(sp18.x, sp18.z);
                s16 temp_r26 = cM_atan2s(spC.x, spC.z);

                if ((s16)fabsf((f32)temp_r26 - (f32)temp_r27) > 0) {
                    kankyo->field_0x1268 = var_f31;
                }
            }
        }
    }
}

u8 dKy_darkworld_check() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    u8 check = FALSE;
    u8 var_r29 = 0;

    if (dComIfGp_world_dark_get() == TRUE) {
        check = TRUE;
    }

    return check;
}

/**
 * @brief Returns the following info about a room: (1) if the room must not be in twilight and (2)
 * which darkLv the room belongs to (Faron, Eldin, etc.).
 *
 * @param stageName stage name
 * @param roomNo room number
 * @param out_darkLv byte pointer to write darkLv to, or NULL
 * @param tblIndex index in darkworld table for the stageName
 * @return int Returns -1 if the given room must not be loaded in twilight, else returns 0 or 1. A
 * return of 1 means darkLv should be read from out_darkLv and 0 means it should be read from the
 * darkworld table.
 */
int dKy_F_SP121Check(char const* stageName, int roomNo, u8* out_darkLv, int tblIndex) {
    dKydata_darkworldInfo_c* darkworldTbl = dKyd_darkworld_tbl_getp();
    int result = 0;

    if (out_darkLv != NULL) {
        *out_darkLv = KY_DARKLV_UNCLEARABLE;
    }

    // Stage is Hyrule Field
    if (!strcmp(stageName, "F_SP121")) {
        // Room is one of:
        // - Eldin Field (0)
        // - Kakariko Gorge (3)
        // - Eldin Field / Kakariko Gorge Path North (5) and South (4)
        // - Faron Field / Kakariko Gorge Path North (2)
        // - Outside Hidden Village (7)
        if (roomNo == 0 || (2 <= roomNo && roomNo <= 5) || roomNo == 7) {
            if (out_darkLv != NULL) {
                *out_darkLv = KY_DARKLV_ELDIN;
            }
            result = 1;
        }

        // Room is one of:
        // - Lanayru Field (10)
        // - Great Bridge of Hylia (13)
        // - Lanayru Field / Great Bridge of Hylia Path North (11) and South (12)
        // - Faron Field / Great Bridge of Hylia Path North (14)
        // - Lanayru Field / Outside Hidden Village Path (9)
        else if (roomNo >= 9 && roomNo <= 14)
        {
            if (out_darkLv != NULL) {
                *out_darkLv = KY_DARKLV_LANAYRU;
            }
            result = 1;
        }

        // Room is one of:
        // - Faron Field (6)
        // - Faron Field / Kakariko Gorge Path South (1)
        // - Faron Field / Great Bridge of Hylia Path South (15)
        else
        {
            result = -1;
        }
    }

    // Faron Spring; No twilight during Rusl cutscene at very beginning of game.
    else if (!strcmp(stageName, "F_SP108") && roomNo == 1 && dComIfGp_getStartStageLayer() == 13)
    {
        result = -1;
    }

    // Prevent twilight if stage depends on Faron Twilight cleared status (Faron Woods, Coro's
    // Lantern Shop, Faron Woods Cave) but haven't finished Ordon Day 2.
    if (darkworldTbl[tblIndex].darkLv == KY_DARKLV_FARON && !dComIfGs_isEventBit(0x4510)) {
        result = -1;
    }

    return result;
}

/**
 * @brief Returns TRUE if (1) the room is one which can be loaded in twilight, (2) there is nothing
 * currently preventing it from being loaded in twilight, and (3) the player has not cleared the
 * relevant dark level (Faron Twilight, etc.). Otherwise returns FALSE.
 *
 * @param stageName stage name
 * @param roomNo room number
 * @return BOOL Returns TRUE if the room can be loaded as twilight and the player has not already
 * cleared it, else FALSE.
 */
u8 dKy_darkworld_stage_check(char const* stageName, int roomNo) {
    dKydata_darkworldInfo_c* darkworldTbl = dKyd_darkworld_tbl_getp();
    u8 result = FALSE;
    u8 darkLv[1];

    for (int i = 0; i < 34; i++) {
        if (!strcmp(stageName, darkworldTbl[i].stageName)) {
            if (darkworldTbl[i].darkLv != KY_DARKLV_ALWAYS) {
                int fsp121CheckResult = dKy_F_SP121Check(stageName, roomNo, darkLv, i);
                if (fsp121CheckResult >= 0) {
                    if (fsp121CheckResult == 0) {
                        *darkLv = darkworldTbl[i].darkLv;
                    }
                    if (!dComIfGs_isDarkClearLV(*darkLv)) {
                        result = TRUE;
                    }
                    break;
                }
            } else {
                // KY_DARKLV_ALWAYS is used to force twilight (likely for testing). This will
                // never normally run since it is not present in l_darkworld_tbl.
                result = TRUE;
                break;
            }
        }
    }

    return result;
}

/**
 * @brief Returns TRUE if a given room would be loaded in twilight. This function always behaves as
 * if the player has not cleared any twilights.
 *
 * For example, Eldin Field will always return TRUE. Faron Woods on the other hand might return TRUE
 * or FALSE depending on whether or not the player has completed Ordon Day 2.
 *
 * @param stageName stage name
 * @param roomNo room number
 * @return BOOL Returns TRUE if a given room would be loaded in twilight. This function always
 * behaves as if the player has not cleared any twilights.
 */
BOOL dKy_darkworld_spot_check(char const* stageName, int roomNo) {
    dKydata_darkworldInfo_c* darkworldTblPtr = dKyd_darkworld_tbl_getp();
    BOOL result = FALSE;

    for (int i = 0; i < 34; i++) {
        if (!strcmp(stageName, darkworldTblPtr->stageName) &&
            dKy_F_SP121Check(stageName, roomNo, NULL, i) >= 0)
        {
            result = TRUE;
            break;
        }
        darkworldTblPtr++;
    }

    return result;
}

void dKy_darkworld_Area_set(char const* stageName, int roomNo) {
    dKydata_darkworldInfo_c* darkworldTblPtr = dKyd_darkworld_tbl_getp();
    u8 darkLv[1];

    for (int i = 0; i < 34; i++) {
        if (!strcmp(stageName, darkworldTblPtr[i].stageName)) {
            int fsp121CheckResult = dKy_F_SP121Check(stageName, roomNo, darkLv, i);
            if (fsp121CheckResult >= 0) {
                if (fsp121CheckResult == 0) {
                    *darkLv = darkworldTblPtr[i].darkLv;
                }
                dComIfGp_setStartStageDarkArea(*darkLv);
                break;
            }
        }
    }
}

void dKy_murky_set(J3DMaterial* material_p) {
    dKankyo_sunlenz_Packet* lenz_packet = g_env_light.mpSunLenzPacket;
    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;
    camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
    cXyz sp18;
    J3DGXColorS10 tev_col;
    J3DGXColor tev_kcol;

    JUT_ASSERT(0x43DB, material_p != NULL);

    tev_col.r = g_env_light.bg_amb_col[1].r;
    tev_col.g = g_env_light.bg_amb_col[1].g;
    tev_col.b = g_env_light.bg_amb_col[1].b;
    tev_col.a = g_env_light.bg_amb_col[2].a;
    tev_kcol.a = g_env_light.bg_amb_col[1].a;

    f32 var_f29 = 1.0f;

    if (g_env_light.field_0x1300 != -1) {
        tev_kcol.a = g_env_light.field_0x1300 * var_f29;
    }

    if (g_env_light.field_0x1302 != -1) {
        tev_col.a = (u8)(g_env_light.field_0x1302 * var_f29);
    }

    if (lenz_packet != NULL && sun_packet->mVisibility > 0.000001f) {
        tev_col.a = (u8)(tev_col.a *
                         (1.0f - (sun_packet->mVisibility *
                                  (1.0f - (lenz_packet->mDistFalloff * lenz_packet->mDistFalloff * lenz_packet->mDistFalloff)))));
    }

    // Diababa room handling
    if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0) {
        dScnKy_env_light_c* var_r26 = dKy_getEnvlight();
        dKankyo_mud_Packet* var_r27 = var_r26->mpMudPacket;

        f32 var_f31;
        if (var_r27 != NULL) {
            var_f31 = 1.0f - var_r27->field_0x1c3c;
        }

        tev_col.r = 20;
        tev_col.g = 6;
        tev_col.b = 15;

        tev_col.r = (u8)(tev_col.r + (s16)(-(f32)tev_col.r * var_f31));
        tev_col.g = (u8)(tev_col.g + (s16)((36.0f - tev_col.g) * var_f31));
        tev_col.b = (u8)(tev_col.b + (s16)((29.0f - tev_col.b) * var_f31));

        if (camera_p != NULL && camera_p->lookat.eye.z < 1800.0f) {
            f32 var_f30;
            if (camera_p->lookat.eye.z < 1550.0f) {
                var_f30 = 1.0f;
            } else {
                var_f30 = 1.0f - ((camera_p->lookat.eye.z - 1550.0f) / 250.0f);
            }
            tev_col.a = (u8)(255.0f * var_f30);
        } else {
            tev_col.a = 0;
        }

        tev_col.a = (u8)(tev_col.a + (s16)(-(f32)tev_col.a * var_f31));

        tev_kcol.a = 0xFF;
        tev_kcol.a = (u8)(tev_kcol.a + (s16)((140.0f - tev_kcol.a) * var_f31));
    }

    tev_kcol.r = 0;
    tev_kcol.g = 0;
    tev_kcol.b = 0;

    material_p->setTevColor(1, &tev_col);
    material_p->setTevKColor(3, &tev_kcol);
}

void dKy_shadow_mode_set(u8 mode) {
    g_env_light.shadow_mode |= mode;
}

void dKy_shadow_mode_reset(u8 mode) {
    g_env_light.shadow_mode &= ~mode;
}

u8 dKy_shadow_mode_check(u8 mode) {
    return g_env_light.shadow_mode & mode;
}

static J3DZModeInfo l_zmodeUpEnable = {1, 3, 1};

static J3DZModeInfo l_zmodeUpDisable = {1, 3, 0};

static J3DAlphaCompInfo l_alphaCompInfoOPA = {
    0x07, 0x00, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00,
};

static J3DAlphaCompInfo l_alphaCompInfo = {
    0x04, 0x80, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x00,
};

void dKy_bg_MAxx_proc(void* bg_model_p) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    J3DModel* model_p = (J3DModel*)bg_model_p;
    dDlst_window_c* window = dComIfGp_getWindow(0);
    camera_class* camera_p = (camera_class*)dComIfGp_getCamera(window->getCameraID());
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);

    if (model_p != NULL && player_p != NULL) {
        J3DModelData* modelData = model_p->getModelData();

        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DGXColor sp5C;

            J3DMaterial* mat_p = modelData->getMaterialNodePointer(i);
            JUTNameTab* mat_nametbl = modelData->getMaterialName();
            const char* mat_name = mat_nametbl->getName(i);

            if (mat_name[3] == 'M' && mat_name[4] == 'A') {
                if (memcmp(&mat_name[3], "MA06", 4) == 0) {
                    dKy_murky_set(mat_p);
                }

                if (memcmp(&mat_name[3], "MA03", 4) == 0 || memcmp(&mat_name[3], "MA09", 4) == 0 ||
                    memcmp(&mat_name[3], "MA17", 4) == 0 || memcmp(&mat_name[3], "MA19", 4) == 0)
                {
                    if (mat_name[5] != '1') {
                        dComIfGd_setListDarkBG();
                    } else if (mat_name[6] == '9') {
                        dComIfGd_setListInvisisble();
                    }

                    if (mat_p->getFog() != NULL) {
                        J3DFogInfo* fog_info_p = mat_p->getFog()->getFogInfo();

                        if (fog_info_p != NULL) {
                            if (mat_name[5] == '0' && mat_name[6] == '9') {
                                fog_info_p->mType = 6;

                                J3DGXColor sp44;
                                sp44.r = 245.0f - g_env_light.mWaterSurfaceShineRate * 45.0f;
                                sp44.g = 245.0f - g_env_light.mWaterSurfaceShineRate * 45.0f;
                                sp44.b = 245.0f - g_env_light.mWaterSurfaceShineRate * 35.0f;
                                sp44.a = 255;
                                mat_p->setTevKColor(1, &sp44);
                            } else {
                                fog_info_p->mType = 7;

                                J3DGXColorS10 spA0;
                                J3DGXColor sp40;
                                spA0.r = g_env_light.bg_amb_col[2].r;
                                spA0.g = g_env_light.bg_amb_col[2].g;
                                spA0.b = g_env_light.bg_amb_col[2].b;
                                spA0.a = g_env_light.bg_amb_col[2].a;

                                sp40.a = g_env_light.bg_amb_col[1].a;
                                sp40.r = 0;
                                sp40.g = 0;
                                sp40.b = 0;

                                mat_p->setTevColor(1, &spA0);
                                mat_p->setTevKColor(3, &sp40);
                            }
                        }
                    }
                }

                if (memcmp(&mat_name[3], "MA07", 4) == 0) {
                    J3DGXColorS10 sp98;
                    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
                    EF_THUNDER* thunder_ef = &kankyo->mThunderEff;

                    sp98.r = (u8)(thunder_ef->mFlashTimer * 100.0f);
                    sp98.g = (u8)(thunder_ef->mFlashTimer * 100.0f);
                    sp98.b = (u8)(thunder_ef->mFlashTimer * 100.0f);
                    sp98.a = 255;

                    mat_p->setTevColor(0, &sp98);
                }

                if (memcmp(&mat_name[3], "MA10", 4) == 0 || memcmp(&mat_name[3], "MA02", 4) == 0) {
                    dComIfGd_setListInvisisble();

                    if (mat_p->getTexGenBlock()->getTexMtx(0) != NULL) {
                        J3DTexMtxInfo* tex_mtx_inf =
                            &mat_p->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();
                        if (tex_mtx_inf != NULL) {
                            cXyz* sp34 = dKyw_get_wind_vec();
                            cXyz sp108;

                            Mtx sp1D8;
                            if (mat_name[6] == '2') {
                                C_MTXLightPerspective(sp1D8, dComIfGd_getView()->fovy,
                                                      camera_p->aspect, 1.0f, 1.0f, -0.01f, 0.0f);
                            } else {
                                C_MTXLightPerspective(sp1D8, dComIfGd_getView()->fovy,
                                                      camera_p->aspect, 0.49f, -0.49f, 0.5f, 0.5f);
                            }

                            tex_mtx_inf->setEffectMtx(sp1D8);
                            modelData->simpleCalcMaterial(0, (MtxP)j3dDefaultMtx);
                        }
                    }
                }

                if (memcmp(&mat_name[3], "MA00", 4) == 0 || memcmp(&mat_name[3], "MA01", 4) == 0 ||
                    memcmp(&mat_name[3], "MA04", 4) == 0 || memcmp(&mat_name[3], "MA16", 4) == 0)
                {
                    sp5C.r = g_env_light.mFogDensity;
                    sp5C.g = 0;
                    sp5C.b = 0;
                    if (mat_name[6] == '1') {
                        J3DPEBlock* peblock_p = mat_p->getPEBlock();
                        mat_p->change();

                        if (dKy_camera_water_in_status_check()) {
                            sp5C.a = 255;
                            peblock_p->getAlphaComp()->setAlphaCompInfo(l_alphaCompInfo);
                            peblock_p->getZMode()->setZModeInfo(l_zmodeUpEnable);
                        } else {
                            sp5C.a = 0;
                            peblock_p->getAlphaComp()->setAlphaCompInfo(l_alphaCompInfoOPA);
                            peblock_p->getZMode()->setZModeInfo(l_zmodeUpDisable);
                        }
                    } else {
                        sp5C.a = 0;
                    }

                    mat_p->setTevKColor(1, &sp5C);
                }

                if (memcmp(&mat_name[3], "MA11", 4) == 0) {
                    J3DGXColorS10 sp90;
                    if (dKy_darkworld_check()) {
                        dComIfGd_setListDarkBG();

                        sp90.r = 170;
                        sp90.g = 160;
                        sp90.b = 255;
                        sp90.a = 255;
                        mat_p->setTevColor(1, &sp90);

                        sp90.r = 50;
                        sp90.g = 20;
                        sp90.b = 90;
                        sp90.a = 255;
                        mat_p->setTevColor(2, &sp90);
                    } else {
                        cXyz spFC;
                        cXyz spF0;

                        sp90.r = 120;
                        sp90.g = 90;
                        sp90.b = 180;
                        sp90.a = 255;

                        if (dComIfG_play_c::getLayerNo(0) == 1) {
                            sp90.a = 0;
                        }

                        mat_p->setTevColor(1, &sp90);

                        sp90.r = 40;
                        sp90.g = 30;
                        sp90.b = 65;
                        sp90.a = 255;
                        mat_p->setTevColor(2, &sp90);

                        if (kankyo->fog_avoid_tag != NULL) {
                            spF0 = kankyo->fog_avoid_tag->mAvoidPos;
                            f32 var_f28 =
                                kankyo->fog_avoid_tag->mSize.x * kankyo->fog_avoid_tag->mSizeScale;
                            if (var_f28 < 0.1f) {
                                var_f28 = 0.1f;
                            }

                            if (mat_p->getTexGenBlock()->getTexMtx(0) != NULL) {
                                J3DTexMtxInfo* tex_mtx_inf =
                                    &mat_p->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();

                                if (tex_mtx_inf != NULL) {
                                    Mtx sp1A8;
                                    Mtx sp178;
                                    C_MTXLightPerspective(sp1A8, var_f28 * 2.8f, 1.0f, 0.5f, 0.5f,
                                                          0.0f, 0.0f);
                                    spFC.x = spF0.x;
                                    spFC.y = 100.0f;
                                    spFC.z = spF0.z;

                                    cXyz spE4(spF0.x, -1000.0f, spF0.z);
                                    mDoMtx_lookAt(sp178, &spE4, &spFC, 0);
                                    cMtx_concat(sp1A8, sp178, sp178);
                                    tex_mtx_inf->setEffectMtx(sp178);
                                }
                            }
                        }
                    }
                } else if (memcmp(&mat_name[3], "MA20", 4) == 0) {
                    cXyz spD8;
                    cXyz spCC;
                    if (mat_p->getFog() != NULL) {
                        J3DFogInfo* fog_inf = mat_p->getFog()->getFogInfo();
                        if (fog_inf != NULL) {
                            fog_inf->mType = 7;
                        }

                        J3DGXColorS10 sp88;
                        sp88.r = g_env_light.bg_amb_col[3].r;
                        sp88.g = g_env_light.bg_amb_col[3].g;
                        sp88.b = g_env_light.bg_amb_col[3].b;
                        sp88.a = 255;

                        mat_p->setTevColor(1, &sp88);

                        if (mat_p->getTexGenBlock()->getTexMtx(2) != NULL) {
                            J3DTexMtxInfo* tex_mtx_inf =
                                &mat_p->getTexGenBlock()->getTexMtx(2)->getTexMtxInfo();

                            if (tex_mtx_inf != NULL) {
                                Mtx sp148;
                                Mtx sp118;
                                C_MTXLightPerspective(sp148, 170.0f, 1.0f, 1.5f, 1.5f, 0.0f, 0.0f);
                                spD8.x = player_p->current.pos.x;
                                spD8.y = -14770.0f;
                                spD8.z = player_p->current.pos.z;

                                cXyz spC0(player_p->current.pos.x, -14570.0f,
                                          player_p->current.pos.z);
                                mDoMtx_lookAt(sp118, &spC0, &spD8, 0);
                                cMtx_concat(sp148, sp118, sp118);
                                tex_mtx_inf->setEffectMtx(sp118);
                            }
                        }
                    }
                } else if (memcmp(&mat_name[3], "MA13", 4) == 0) {
                    J3DGXColorS10 sp80;
                    sp80.r = g_env_light.bg_amb_col[3].r;
                    sp80.g = g_env_light.bg_amb_col[3].g;
                    sp80.b = g_env_light.bg_amb_col[3].b;
                    sp80.a = g_env_light.bg_amb_col[3].a;
                    mat_p->setTevColor(1, &sp80);
                } else if (memcmp(&mat_name[3], "MA14", 4) == 0) {
                    J3DGXColorS10 sp78;
                    J3DGXColor sp1C;
                    sp78.r = g_env_light.fog_col.r;
                    sp78.g = g_env_light.fog_col.g;
                    sp78.b = g_env_light.fog_col.b;
                    sp78.a = g_env_light.fog_col.a;
                    mat_p->setTevColor(1, &sp78);

                    sp1C.r = sp1C.g = sp1C.b = 0;
                    sp1C.a = g_env_light.bg_amb_col[3].a;
                    mat_p->setTevKColor(3, &sp1C);
                } else if (memcmp(&mat_name[3], "MA16", 4) == 0) {
                    J3DGXColorS10 sp70;
                    J3DGXColor sp18;
                    sp70.r = g_env_light.bg_amb_col[1].r;
                    sp70.g = g_env_light.bg_amb_col[1].g;
                    sp70.b = g_env_light.bg_amb_col[1].b;
                    sp70.a = g_env_light.bg_amb_col[1].a;
                    mat_p->setTevColor(1, &sp70);

                    sp18.r = sp18.g = sp18.b = 0;
                    sp18.a = g_env_light.bg_amb_col[3].a;
                    mat_p->setTevKColor(3, &sp18);
                }
            }

            if (memcmp(&mat_name[3], "Rainbow", 7) == 0) {
                f32 temp_f29;
                f32 temp_f31;

                f32 time = g_env_light.getDaytime();
                cXyz spB4;
                camera_class* camera_p = dComIfGp_getCamera(0);
                J3DGXColor sp10;

                cXyz spA8;
                spA8.x = -5853.0f;
                spA8.y = camera_p->lookat.eye.y;
                spA8.z = -879.0f;

                temp_f31 = spA8.abs(camera_p->lookat.eye);
                temp_f31 = -0.2f + (temp_f31 / 4500.0f);
                if (temp_f31 > 1.0f) {
                    temp_f31 = 1.0f;
                } else if (temp_f31 < 0.0f) {
                    temp_f31 = 0.0f;
                }

                sp10.b = sp10.g = sp10.r = 255;
                sp10.a = 255;

                temp_f29 = 0.0f;
                if (time >= 270.0f) {
                    if (time < 285.0f) {
                        temp_f29 = (285.0f - time) / 15.0f;
                    }
                } else if (time < 75.0f) {
                    if (time >= 60.0f) {
                        temp_f29 = 1.0f - ((75.0f - time) / 15.0f);
                    }
                } else {
                    temp_f29 = 1.0f;
                }

                if (g_env_light.mColpatWeather != 0) {
                    temp_f29 = 0.0f;
                }

                sp10.b = 255;
                sp10.g = 255;
                sp10.r = 255;

                f32 temp_f25 = 1.0f;

                spA8 = camera_p->lookat.center;
                spA8.y = camera_p->lookat.eye.y;
                dKyr_get_vectle_calc(&camera_p->lookat.eye, &spA8, &spB4);

                f32 temp_f27 = cM3d_VectorProduct2d(0.0f, 0.0f, -0.81f, 0.583f, spB4.x, spB4.z);
                f32 temp_f26;
                if (temp_f27 < 0.2f) {
                    sp10.a = 0;
                } else {
                    if (temp_f27 > 0.6f) {
                        temp_f26 = 1.0f;
                    } else {
                        temp_f26 = (temp_f27 - 0.2f) / 0.4f;
                    }
                    sp10.a = temp_f25 * (temp_f31 * (115.0f * temp_f26 * temp_f29));
                }

                mat_p->setTevKColor(3, &sp10);
            }
        }
    }
}
