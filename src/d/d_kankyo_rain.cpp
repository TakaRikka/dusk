#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTTexture.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_demo.h"
#include "d/d_kankyo_rain.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"

static void vectle_calc(DOUBLE_POS* i_pos, cXyz* o_out) {
    double s = sqrt(i_pos->x * i_pos->x + i_pos->y * i_pos->y + i_pos->z * i_pos->z);

    if (s != 0.0) {
        o_out->x = i_pos->x / s;
        o_out->y = i_pos->y / s;
        o_out->z = i_pos->z / s;
    } else {
        o_out->x = 0.0;
        o_out->y = 0.0;
        o_out->z = 0.0;
    }
}

static void get_vectle_calc(cXyz* i_vecA, cXyz* i_vecB, cXyz* o_out) {
    DOUBLE_POS pos;
    pos.x = i_vecB->x - i_vecA->x;
    pos.y = i_vecB->y - i_vecA->y;
    pos.z = i_vecB->z - i_vecA->z;

    vectle_calc(&pos, o_out);
}

void dKyr_get_vectle_calc(cXyz* i_vecA, cXyz* i_vecB, cXyz* o_out) {
    get_vectle_calc(i_vecA, i_vecB, o_out);
}

static void dKy_set_eyevect_calc(camera_class* i_camera, Vec* o_out, f32 param_2, f32 param_3) {
    cXyz calc;
    get_vectle_calc(&i_camera->lookat.eye, &i_camera->lookat.center, &calc);
    o_out->x = i_camera->lookat.eye.x + calc.x * param_2;
    o_out->y = (i_camera->lookat.eye.y + calc.y * param_3) - 200.0f;
    o_out->z = i_camera->lookat.eye.z + calc.z * param_2;
}

void dKy_set_eyevect_calc2(camera_class* i_camera, Vec* o_out, f32 param_2, f32 param_3) {
    cXyz calc;
    DOUBLE_POS pos;

    pos.x = i_camera->lookat.center.x - i_camera->lookat.eye.x;
    if (param_3 != 0.0f) {
        pos.y = i_camera->lookat.center.y - i_camera->lookat.eye.y;
    } else {
        pos.y = 0.0f;
    }
    pos.z = i_camera->lookat.center.z - i_camera->lookat.eye.z;

    vectle_calc(&pos, &calc);

    o_out->x = i_camera->lookat.eye.x + calc.x * param_2;
    o_out->y = i_camera->lookat.eye.y + calc.y * param_3;
    o_out->z = i_camera->lookat.eye.z + calc.z * param_2;

    if (param_3 == 0.0f) {
        o_out->y = 0.0f;
    }
}

static void dKyr_set_btitex_common(GXTexObj* i_obj, ResTIMG* i_img, GXTexMapID i_mapID) {
    GXInitTexObj(i_obj, (&i_img->format + i_img->imageOffset), i_img->width, i_img->height,
                 (GXTexFmt)i_img->format, (GXTexWrapMode)i_img->wrapS, (GXTexWrapMode)i_img->wrapT,
                 (GXBool)(i_img->mipmapCount > 1));

    GXInitTexObjLOD(i_obj, (GXTexFilter)i_img->minFilter, (GXTexFilter)i_img->magFilter,
                    i_img->minLOD * 0.125f, i_img->maxLOD * 0.125f, i_img->LODBias * 0.01f,
                    (GXBool)i_img->biasClamp, (GXBool)i_img->doEdgeLOD,
                    (GXAnisotropy)i_img->maxAnisotropy);

    GXLoadTexObj(i_obj, i_mapID);
}

static void dKyr_set_btitex(GXTexObj* i_obj, ResTIMG* i_img) {
    dKyr_set_btitex_common(i_obj, i_img, GX_TEXMAP0);
}

void dKyr_lenzflare_move() {
    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;
    dKankyo_sunlenz_Packet* lenz_packet = g_env_light.mpSunLenzPacket;
    camera_class* camera = dComIfGp_getCamera(0);

    cXyz eyeVect;
    cXyz field_0x3c;
    cXyz sunDirSmth;
    cXyz camFwd;

    if (sun_packet->mVisibility < 0.0001f) {
        return;
    }

    dKy_set_eyevect_calc(camera, &eyeVect, 4000.0f, 4000.0f);
    dKyr_get_vectle_calc(&eyeVect, sun_packet->mPos, &sunDirSmth);
    lenz_packet->mPositions[0] = sun_packet->mPos[0];
    lenz_packet->mPositions[1] = sun_packet->mPos[0];

    cXyz vect;
    cXyz proj;
    cXyz center;
    mDoLib_project(lenz_packet->mPositions, &proj);

    center.x = FB_WIDTH / 2;
    center.y = FB_HEIGHT / 2;
    center.z = 0.0f;
    dKyr_get_vectle_calc(&center, &proj, &vect);

    lenz_packet->field_0x94 = cM_atan2s(vect.x, vect.y);
    lenz_packet->field_0x94 *= 0.0054931640625f;  // Maybe 45 / 8192
    lenz_packet->field_0x94 += 180.0f;

    dKyr_get_vectle_calc(&camera->lookat.eye, &camera->lookat.center, &camFwd);

    for (int i = 2; i < 8; i++) {
        if (i == 2) {
            f32 size = 250.0f + 600.0f * sunDirSmth.abs(camFwd);
            lenz_packet->mPositions[i].x = sun_packet->mPos[0].x - sunDirSmth.x * size * i;
            lenz_packet->mPositions[i].y = sun_packet->mPos[0].y - sunDirSmth.y * size * i;
            lenz_packet->mPositions[i].z = sun_packet->mPos[0].z - sunDirSmth.z * size * i;
        } else {
            f32 size = 250.0f + 110.0f * sunDirSmth.abs(camFwd);
            lenz_packet->mPositions[i].x = sun_packet->mPos[0].x - (4100.0f * sunDirSmth.x + sunDirSmth.x * size * i);
            lenz_packet->mPositions[i].y = sun_packet->mPos[0].y - (4100.0f * sunDirSmth.y + sunDirSmth.y * size * i);
            lenz_packet->mPositions[i].z = sun_packet->mPos[0].z - (4100.0f * sunDirSmth.z + sunDirSmth.z * size * i);
        }
    }
}

static BOOL dKyr_moon_arrival_check() {
    BOOL rv = FALSE;
    if (g_env_light.daytime > 285.0f || g_env_light.daytime < 67.5f) {
        rv = TRUE;
    }
    return rv;
}

void dKyr_sun_move() {
    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;
    dKankyo_sunlenz_Packet* lenz_packet = g_env_light.mpSunLenzPacket;
    camera_class* camera_p2 = dComIfGp_getCamera(0);
    camera_class* camera = dComIfGp_getCamera(0);
    cXyz lightDir;

    f32 pulse_pos;
    f32 sun_parcent = 0.0f;

    static f32 S_parcent_bak = 0.0f;

    u8 numPointsVisible = 0;
    u8 numCenterPointsVisible = 0;
    s32 numPointsCulled = 0;

    u32 stage_type = dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo());

    if (g_env_light.base_light.mColor.r == 0 && stage_type != ST_ROOM) {
        dKyr_get_vectle_calc(&camera_p2->lookat.eye, &g_env_light.base_light.mPosition,
                             &lightDir);
    } else {
        dKyr_get_vectle_calc(&camera_p2->lookat.eye, &g_env_light.sun_light_pos, &lightDir);
    }

    sun_packet->mPos[0].x = camera_p2->lookat.eye.x + 8000.0f * lightDir.x;
    sun_packet->mPos[0].y = camera_p2->lookat.eye.y + 8000.0f * lightDir.y;
    sun_packet->mPos[0].z = camera_p2->lookat.eye.z + 8000.0f * lightDir.z;

    f32 horizon_y = (sun_packet->mPos[0].y - camera_p2->lookat.eye.y) / 8000.0f;
    if (horizon_y < 0.0f) {
        horizon_y = 0.0f;
    }

    if (horizon_y >= 1.0f) {
        horizon_y = 1.0f;
    }

    horizon_y = 1.0f - horizon_y;
    horizon_y *= horizon_y;
    pulse_pos = 1.0f - horizon_y;

    if (dComIfGp_getStage()->getStagInfo() != NULL) {
        dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo());
    }

    if (sun_packet->field_0x28) {
        sun_packet->field_0x28--;
    }

    sun_packet->field_0x29 = 0;

    if (g_env_light.daytime > 77.5f && g_env_light.daytime < 285.0f && dKy_rain_check() < 20 &&
        strcmp(dComIfGp_getStartStageName(), "F_SP200") != 0 &&
        strcmp(dComIfGp_getStartStageName(), "D_MN09B") != 0)
    {
        f32 border_y = 0.0f;
        cLib_addCalc(&sun_packet->mSunAlpha, 1.0f, 0.05f, 0.1f, 0.00001f);

        if (camera != NULL) {
            border_y = camera->mCamera.TrimHeight();
        }

        cXyz proj;
        mDoLib_project(sun_packet->mPos, &proj);

        for (int i = 0; i < 5; i++) {
            cXyz chkpnt = proj;

            static f32 const sun_chkpnt[5][2] = {
                {0.0f, 0.0f},
                {-10.0f, -20.0f},
                {10.0f, 20.0f},
                {-20.0f, 10.0f},
                {20.0f, -10.0f},
            };
            chkpnt.x -= sun_chkpnt[i][0];
            chkpnt.y -= sun_chkpnt[i][1];

            if (chkpnt.x > 0.0f && chkpnt.x < FB_WIDTH && chkpnt.y > border_y && chkpnt.y < 458.0f - border_y) {
                if (sun_packet->field_0x44[i] >= 0xFFFFFF) {
                    numPointsVisible++;
                    if (i == 0) {
                        numCenterPointsVisible++;
                    }
                }
                dComIfGd_peekZ(chkpnt.x, chkpnt.y, sun_packet->field_0x44 + i);
            } else {
                sun_packet->field_0x44[i] = 0;
                numPointsCulled++;
            }
        }

        if (numPointsCulled != 0 && numPointsVisible != 0 && numCenterPointsVisible != 0) {
            numCenterPointsVisible = 1;
            numPointsVisible = 5;
        }

        if (numPointsVisible != 0) {
            if (sun_packet->field_0x28 < 5) {
                sun_packet->field_0x28 += 2;
            }
            sun_packet->field_0x29 = 1;
        }

        lenz_packet->field_0x84 = lenz_packet->field_0x8c;
        lenz_packet->field_0x88 = lenz_packet->field_0x90;
        lenz_packet->field_0x8c = 1000000000.0f; // This is not G_CM3D_F_INF
        lenz_packet->field_0x90 = 0.0f;

        cXyz center;
        center.x = FB_WIDTH / 2;
        center.y = FB_HEIGHT / 2;
        center.z = 0.0f;

        lenz_packet->mDistFalloff = center.abs(proj);
        lenz_packet->mDistFalloff /= 450.0f;
        if (lenz_packet->mDistFalloff > 1.0f) {
            lenz_packet->mDistFalloff = 1.0f;
        }
        lenz_packet->mDistFalloff = 1.0f - lenz_packet->mDistFalloff;
        sun_parcent = lenz_packet->mDistFalloff * lenz_packet->mDistFalloff;
        lenz_packet->mDistFalloff = 1.0f - sun_parcent;
    } else {
        cLib_addCalc(&sun_packet->mSunAlpha, 0.0f, 0.1f, 0.05f, 0.001f);
        numPointsVisible = 0;
        sun_packet->field_0x28 = 0;
        sun_packet->field_0x29 = 0;
    }

    if (g_env_light.mColpatWeather != 0 || g_env_light.mSnowCount > 10) {
        numCenterPointsVisible = 0;
        numPointsVisible = 0;
    }

    if (stage_type == ST_ROOM) {
        numCenterPointsVisible = 0;
        numPointsVisible = 0;
    }

    if (numCenterPointsVisible != 0) {
        if (numPointsVisible == 4) {
            cLib_addCalc(&sun_packet->mVisibility, 1.0f, 0.5f, 0.5f, 0.01f);
        } else {
            cLib_addCalc(&sun_packet->mVisibility, 1.0f, 0.2f, 0.3f, 0.001f);
        }
    } else if (numPointsVisible < 1) {
        cLib_addCalc(&sun_packet->mVisibility, 0.0f, 0.5f, 0.5f, 0.001f);
    } else {
        cLib_addCalc(&sun_packet->mVisibility, 0.0f, 0.2f, 0.3f, 0.001f);
    }

    if (sun_packet->mVisibility <= 0.0f) {
        g_env_light.mpSunLenzPacket->mDrawLenzInSky = TRUE;
    } else {
        g_env_light.mpSunLenzPacket->mDrawLenzInSky = FALSE;
    }

    if (lightDir.y > 0.0f && !g_env_light.mpSunLenzPacket->mDrawLenzInSky) {
        if (dStage_stagInfo_GetArg0(dComIfGp_getStage()->getStagInfo()) != 0) {
            f32 var_f1_3;
            if (S_parcent_bak < sun_parcent) {
                var_f1_3 = sun_parcent - S_parcent_bak;
            } else {
                var_f1_3 = S_parcent_bak - sun_parcent;
            }

            if (var_f1_3 < 0.3f) {
                dKy_set_actcol_ratio(1.0f - sun_parcent * sun_packet->mVisibility);
                dKy_set_bgcol_ratio(1.0f - sun_parcent * sun_packet->mVisibility);
                dKy_set_fogcol_ratio(1.0f - pulse_pos * (sun_parcent * sun_packet->mVisibility));
                dKy_set_vrboxcol_ratio(1.0f - 0.8f * (pulse_pos * (sun_parcent * sun_packet->mVisibility)));
            }
            S_parcent_bak = sun_parcent;
        }
    }

    if (g_env_light.daytime < 255.0f && g_env_light.mColpatWeather == 0) {
        cLib_addCalc(&sun_packet->field_0x6c, 1.0f, 0.1f, 0.01f, 0.0001f);
    } else {
        cLib_addCalc(&sun_packet->field_0x6c, 0.0f, 0.5f, 0.1f, 0.001f);
    }

    static GXColor sun_color[3] = {
        {255, 255, 253, 255},
        {255, 255, 255, 255},
        {255, 255, 253, 255},
    };
    static GXColor sun_color2[3] = {
        {255, 110, 43, 255},
        {156, 121, 92, 255},
        {255, 78, 0, 255},
    };

    if (g_env_light.daytime < 180.0f) {
        f32 parcent;
        if (g_env_light.daytime < 90.0f) {
            parcent = 0.0f;
        } else if (g_env_light.daytime < 105.0f) {
            parcent = dKy_get_parcent(105.0f, 90.0f, g_env_light.daytime);
        } else {
            parcent = 1.0f;
        }

        sun_packet->mColor.r =
            (f32)sun_color[0].r + parcent * ((f32)sun_color[1].r - (f32)sun_color[0].r);
        sun_packet->mColor.g =
            (f32)sun_color[0].g + parcent * ((f32)sun_color[1].g - (f32)sun_color[0].g);
        sun_packet->mColor.b =
            (f32)sun_color[0].b + parcent * ((f32)sun_color[1].b - (f32)sun_color[0].b);

        sun_packet->field_0x74.r =
            (f32)sun_color2[0].r + parcent * ((f32)sun_color2[1].r - (f32)sun_color2[0].r);
        sun_packet->field_0x74.g =
            (f32)sun_color2[0].g + parcent * ((f32)sun_color2[1].g - (f32)sun_color2[0].g);
        sun_packet->field_0x74.b =
            (f32)sun_color2[0].b + parcent * ((f32)sun_color2[1].b - (f32)sun_color2[0].b);
    } else {
        f32 parcent;
        if (g_env_light.daytime < 247.5f) {
            parcent = 1.0f;
        } else if (g_env_light.daytime < 270.0f) {
            parcent = 1.0f - dKy_get_parcent(270.0f, 247.5f, g_env_light.daytime);
        } else {
            parcent = 0.0f;
        }

        sun_packet->mColor.r =
            (f32)sun_color[2].r + parcent * ((f32)sun_color[1].r - (f32)sun_color[2].r);
        sun_packet->mColor.g =
            (f32)sun_color[2].g + parcent * ((f32)sun_color[1].g - (f32)sun_color[2].g);
        sun_packet->mColor.b =
            (f32)sun_color[2].b + parcent * ((f32)sun_color[1].b - (f32)sun_color[2].b);

        sun_packet->field_0x74.r =
            (f32)sun_color2[2].r + parcent * ((f32)sun_color2[1].r - (f32)sun_color2[2].r);
        sun_packet->field_0x74.g =
            (f32)sun_color2[2].g + parcent * ((f32)sun_color2[1].g - (f32)sun_color2[2].g);
        sun_packet->field_0x74.b =
            (f32)sun_color2[2].b + parcent * ((f32)sun_color2[1].b - (f32)sun_color2[2].b);
    }

    if (dKyr_moon_arrival_check()) {
        f32 alpha = (sun_packet->mPos[0].y - camera_p2->lookat.eye.y) / -8000.0f;
        alpha *= alpha;
        alpha *= 6.0f;

        if (alpha > 1.0f) {
            alpha = 1.0f;
        }

        cLib_addCalc(&sun_packet->mMoonAlpha, alpha, 0.2f, 0.01f, 0.001f);
    } else {
        cLib_addCalc(&sun_packet->mMoonAlpha, 0.0f, 0.2f, 0.01f, 0.001f);
    }

    if (g_env_light.camera_water_in_status == 0 && g_env_light.daytime > 255.0f &&
        sun_packet->mSunAlpha >= 0.2f)
    {
        cXyz sp24;
        dKyr_get_vectle_calc(&camera_p2->lookat.eye, &g_env_light.sun_light_pos, &lightDir);
        sp24.x = camera_p2->lookat.eye.x + 30160.0f * lightDir.x;
        sp24.y = camera_p2->lookat.eye.y + 30160.0f * lightDir.y;
        sp24.z = camera_p2->lookat.eye.z + 30160.0f * lightDir.z;
        sp24.y -= 21678.0f;
        sun_packet->field_0x58 = dComIfGp_particle_set(sun_packet->field_0x58, 0x11C, &sp24, 0, 0);
    }
}

void dKyr_rain_init() {
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    g_env_light.mpRainPacket->mpTex = (u8*)dComIfG_getObjectRes("Always", 0x4a);
    g_env_light.mpRainPacket->mpTex2 = (u8*)dComIfG_getObjectRes("Always", 0x57);
    g_env_light.mpRainPacket->mCamEyePos = camera->lookat.eye;
    g_env_light.mpRainPacket->mCamCenterPos = camera->lookat.center;
    g_env_light.mpRainPacket->mCenterDeltaMul = 0.0f;
    g_env_light.mpRainPacket->field_0x3700 = 0.0f;
    g_env_light.mpRainPacket->mSibukiAlpha = 0.0f;
    g_env_light.mpRainPacket->mOverheadFade = 0.0f;
    g_env_light.mpRainPacket->mFwdFade1 = 0.0f;
    g_env_light.mpRainPacket->mFwdFade2 = 0.0f;
    g_env_light.mpRainPacket->mStatus = 0.0f;
    g_env_light.mpRainPacket->mCenterDelta.x = 0.0f;
    g_env_light.mpRainPacket->mCenterDelta.y = 0.0f;
    g_env_light.mpRainPacket->mCenterDelta.z = 0.0f;

    for (int i = 0; i < 250; i++) {
        g_env_light.mpRainPacket->mRainEff[i].mStatus = 0;
    }
    g_env_light.mpRainPacket->raincnt = 0;
}

static void rain_bg_chk(dKankyo_rain_Packet* i_packet, int i_idx) {
    i_packet->mRainEff[i_idx].field_0x30 = dComIfGp_getCamera(0)->lookat.center.y + -800.0f;
}

static BOOL overhead_bg_chk() {
    camera_class* camera = dComIfGp_getCamera(0);
    BOOL chk = false;

    dBgS_ObjGndChk_All gndchk;
    dBgS_RoofChk roofchk;

    cXyz chkPos = camera->lookat.eye;
    chkPos.y += 50.0f;

    roofchk.SetPos(chkPos);
    if (dComIfG_Bgsp().RoofChk(&roofchk) != 1.0e9f) {
        chk = true;
        // Outside East Castle Town
        if (!strcmp(dComIfGp_getStartStageName(), "F_SP122") &&
            dStage_roomControl_c::getStayNo() == 17)
        {
            return false;
        }
    }

    chkPos.y += 10000.0f;
    gndchk.SetPos(&chkPos);
    if (dComIfG_Bgsp().GroundCross(&gndchk) > camera->lookat.eye.y + 50.0f) {
        chk = true;
    }

    return chk;
}

static BOOL forward_overhead_bg_chk(cXyz* ppos, f32 dist) {
    camera_class* camera = dComIfGp_getCamera(0);
    BOOL chk = FALSE;

    dBgS_ObjGndChk_All gndchk;
    dBgS_RoofChk roofchk;

    cXyz chk_pos;
    cXyz vectle;
    dKyr_get_vectle_calc(&camera->lookat.eye, &camera->lookat.center, &vectle);
    chk_pos.x = camera->lookat.eye.x + vectle.x * dist;
    chk_pos.y = camera->lookat.eye.y + vectle.y * dist;
    chk_pos.z = camera->lookat.eye.z + vectle.z * dist;
    chk_pos.y = 50.0f + camera->lookat.eye.y;
    *ppos = chk_pos;

    roofchk.SetPos(chk_pos);
    if (G_CM3D_F_INF != dComIfG_Bgsp().RoofChk(&roofchk)) {
        chk = TRUE;
        if (strcmp(dComIfGp_getStartStageName(), "F_SP122") == 0) {
            if (dStage_roomControl_c::getStayNo() == 17) {
                return FALSE;
            }
        }
    }

    chk_pos.y += 10000.0f;
    gndchk.SetPos(&chk_pos);

    if (dComIfG_Bgsp().GroundCross(&gndchk) > 50.0f + camera->lookat.eye.y) {
        chk = TRUE;
    }
    return chk;
}

void dKyr_rain_move() {
    dKankyo_rain_Packet* rain_packet;
    camera_class* camera;

    rain_packet = g_env_light.mpRainPacket;
    camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz spFC;
    cXyz spF0;
    cXyz spE4;
    cXyz spD8;
    cXyz spCC = dKyw_get_wind_vecpow();

    cXyz spC0;
    cXyz spB4;
    cXyz spA8;
    cXyz sp9C;
    cXyz sp90;

    cXyz* sp3C;
    f32 sp38;
    BOOL sp34;
    BOOL sp30;
    BOOL sp2C;
    u32 sp28;
    BOOL sp24 = 0;
    f32 sp20;
    BOOL sp1C = 0;

    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;

    sp28 = dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo());

    spA8.x = 0.0f;
    spA8.y = -2.5f;
    spA8.z = 0.0f;

    if (rain_packet->raincnt <= g_env_light.raincnt) {
        rain_packet->raincnt = g_env_light.raincnt;
    }

    if (rain_packet->raincnt != 0) {
        dKyr_get_vectle_calc(&camera->lookat.center, &rain_packet->mCamCenterPos, &spB4);

        var_f30 = rain_packet->mCamEyePos.abs(camera->lookat.eye);

        if (var_f30 > 10.0f) {
            var_f30 = (var_f30 - 10.0f) / 50.0f;
            if (var_f30 > 1.0f) {
                var_f30 = 1.0f;
            }
        } else {
            var_f30 = 0.0f;
        }

        rain_packet->mCamEyePos.x = camera->lookat.eye.x;
        rain_packet->mCamEyePos.z = camera->lookat.eye.z;

        var_f28 = rain_packet->mCamEyePos.abs(camera->lookat.eye);
        var_f28 = var_f28 / 20.0f;
        if (var_f28 > 1.0f) {
            var_f28 = 1.0f;
        }

        spA8.y += -(var_f28 * 15.0f);
        rain_packet->mCamEyePos = camera->lookat.eye;

        var_f29 = rain_packet->mCamCenterPos.abs(camera->lookat.center);
        if (var_f29 > 10.0f) {
            var_f29 = (var_f29 - 10.0f) / 50.0f;
            if (var_f29 > 1.0f) {
                var_f29 = 1.0f;
            }
        } else {
            var_f29 = 0.0f;
        }

        cLib_addCalc(&rain_packet->mCenterDeltaMul, var_f30 * var_f29, 0.2f, 0.1f, 0.01f);
        if (rain_packet->mCenterDeltaMul > 0.3f) {
            rain_packet->mCenterDeltaMul = 0.3f;
        }

        cLib_addCalc(&rain_packet->mCenterDelta.x, spB4.x, 0.2f, 0.1f, 0.01f);
        cLib_addCalc(&rain_packet->mCenterDelta.y, spB4.y, 0.2f, 0.1f, 0.01f);
        cLib_addCalc(&rain_packet->mCenterDelta.z, spB4.z, 0.2f, 0.1f, 0.01f);
        rain_packet->mCamCenterPos = camera->lookat.center;

        dKy_set_eyevect_calc2(camera, &spFC, 700.0f, 600.0f);
        spD8.x = spD8.y = spD8.z = 0.0f;

        sp3C = dKyw_get_wind_vec();
        sp38 = dKyw_get_wind_pow();

        rain_packet->field_0x36cc = 0;
        rain_packet->field_0x36c8 = 0;
        rain_packet->mStatus = 0;

        sp34 = 0;
        sp30 = 0;
        sp2C = 0;

        if (strcmp(dComIfGp_getStartStageName(), "R_SP30") == 0) {
            if (dComIfGp_roomControl_getStayNo() == 0 || dComIfGp_roomControl_getStayNo() == 4) {
                sp1C = 1;
            }
        } else if (strcmp(dComIfGp_getStartStageName(), "R_SP107") == 0 &&
                   dComIfGp_roomControl_getStayNo() == 1)
        {
            sp1C = 2;
        } else if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
            sp1C = 3;
        }

        if (sp1C == 0) {
            sp34 = overhead_bg_chk();
            sp30 = forward_overhead_bg_chk(&sp9C, 700.0f);
            sp2C = forward_overhead_bg_chk(&sp90, 1400.0f);

            if (sp34) {
                rain_packet->mStatus |= (u8)1;
            } else if (sp30) {
                rain_packet->mStatus |= (u8)2;
            }
        } else {
            sp24 = 1;
            rain_packet->mStatus |= (u8)1;
            sp20 = 1200.0f;
        }

        if (sp34) {
            cLib_addCalc(&rain_packet->mOverheadFade, 0.0f, 0.5f, 0.2f, 0.01f);
        } else {
            cLib_addCalc(&rain_packet->mOverheadFade, 1.0f, 0.1f, 0.1f, 0.001f);
        }

        if (sp30) {
            cLib_addCalc(&rain_packet->mFwdFade1, 0.0f, 0.5f, 0.2f, 0.01f);
        } else {
            cLib_addCalc(&rain_packet->mFwdFade1, 1.0f, 0.1f, 0.1f, 0.001f);
        }

        if (sp2C) {
            cLib_addCalc(&rain_packet->mFwdFade2, 0.0f, 0.5f, 0.2f, 0.01f);
        } else {
            cLib_addCalc(&rain_packet->mFwdFade2, 1.0f, 0.1f, 0.1f, 0.001f);
        }

        for (int i = rain_packet->raincnt - 1; i >= 0; i--) {
            rain_packet->mRainEff[i].mBasePos.y = spFC.y;

            switch (rain_packet->mRainEff[i].mStatus) {
            case 0:
                rain_packet->mRainEff[i].field_0x24 = -(cM_rndF(10.0f) + 35.5f);
                rain_packet->mRainEff[i].mTimer = 0;
                rain_packet->mRainEff[i].mBasePos.x = spFC.x;
                rain_packet->mRainEff[i].mBasePos.y = spFC.y;
                rain_packet->mRainEff[i].mBasePos.z = spFC.z;
                rain_packet->mRainEff[i].mPosition.x = cM_rndFX(800.0f);
                rain_packet->mRainEff[i].mPosition.y = cM_rndF(600.0f);
                rain_packet->mRainEff[i].mPosition.z = cM_rndFX(800.0f);
                rain_packet->mRainEff[i].mAlpha = 1.0f;
                rain_packet->mRainEff[i].field_0x1c = cM_rndF(360.0f);
                rain_packet->mRainEff[i].field_0x20 = cM_rndF(360.0f);

                rain_bg_chk(rain_packet, i);
                rain_packet->mRainEff[i].mStatus++;
                break;
            case 1:
            case 2:
            case 3:
                cLib_addCalc(&rain_packet->mRainEff[i].field_0x24, rain_packet->mRainEff[i].field_0x24 - cM_rndFX(0.1f), 0.5f, 0.1f, 0.01f);

                rain_packet->mRainEff[i].mPosition.x +=
                    20.0f *
                    ((spCC.x + (10.0f * (rain_packet->mCenterDelta.x * rain_packet->mCenterDeltaMul))) +
                     (spA8.x + (0.08f * (f32)(i & 7))));

                rain_packet->mRainEff[i].mPosition.y +=
                    ((f32)(i & 7) * -2.0f) +
                    (20.0f *
                     (spA8.y +
                      (spCC.y + ((rain_packet->mCenterDelta.y * rain_packet->mCenterDeltaMul) * 10.0f))));

                rain_packet->mRainEff[i].mPosition.z +=
                    20.0f *
                    ((spCC.z + (10.0f * (rain_packet->mCenterDelta.z * rain_packet->mCenterDeltaMul))) +
                     (spA8.z + (0.08f * (f32)(i & 3))));

                spC0.x = rain_packet->mRainEff[i].mBasePos.x + rain_packet->mRainEff[i].mPosition.x;
                spC0.y = spFC.y;
                spC0.z = rain_packet->mRainEff[i].mBasePos.z + rain_packet->mRainEff[i].mPosition.z;

                f32 var_f1 = spC0.abs(spFC);

                if (rain_packet->mRainEff[i].mTimer == 0) {
                    if (var_f1 > 800.0f) {
                        rain_packet->mRainEff[i].mTimer = 10;
                        rain_packet->mRainEff[i].field_0x24 = -(cM_rndF(10.0f) + 35.5f);
                        rain_packet->mRainEff[i].mBasePos = spFC;

                        if (spC0.abs(spFC) > 850.0f) {
                            rain_packet->mRainEff[i].mPosition.x = cM_rndFX(800.0f);
                            rain_packet->mRainEff[i].mPosition.y = cM_rndFX(800.0f);
                            rain_packet->mRainEff[i].mPosition.z = cM_rndFX(800.0f);
                        } else {
                            var_f31 = cM_rndFX(40.0f);
                            get_vectle_calc(&spC0, &spFC, &spB4);

                            rain_packet->mRainEff[i].mPosition.x = spB4.x * (var_f31 + 800.0f);
                            rain_packet->mRainEff[i].mPosition.y = spB4.y * (var_f31 + 800.0f);
                            rain_packet->mRainEff[i].mPosition.z = spB4.z * (var_f31 + 800.0f);
                        }

                        rain_packet->mRainEff[i].mStatus = 1;
                        rain_bg_chk(rain_packet, i);
                    }

                    spC0.y = rain_packet->mRainEff[i].mBasePos.y + rain_packet->mRainEff[i].mPosition.y;
                    if (spC0.y < rain_packet->mRainEff[i].field_0x30 + 20.0f) {
                        rain_packet->mRainEff[i].mBasePos = spFC;
                        rain_packet->mRainEff[i].mPosition.x = cM_rndFX(800.0f);
                        rain_packet->mRainEff[i].mPosition.y = 200.0f;
                        rain_packet->mRainEff[i].mPosition.z = cM_rndFX(800.0f);
                        rain_bg_chk(rain_packet, i);
                        rain_packet->mRainEff[i].mTimer = 10;
                    }
                } else {
                    rain_packet->mRainEff[i].mTimer--;
                }
            }

            spC0.x = rain_packet->mRainEff[i].mBasePos.x + rain_packet->mRainEff[i].mPosition.x;
            spC0.y = rain_packet->mRainEff[i].mBasePos.y + rain_packet->mRainEff[i].mPosition.y;
            spC0.z = rain_packet->mRainEff[i].mBasePos.z + rain_packet->mRainEff[i].mPosition.z;

            if (i > g_env_light.raincnt - 1) {
                if (i == rain_packet->raincnt - 1) {
                    rain_packet->raincnt--;
                }
            }

            var_f31 = 1.0f;
            if (sp34 || rain_packet->mOverheadFade < 1.0f) {
                cXyz sp80;
                f32 sp10 = 800.0f;
                sp80 = spC0;
                sp80.y = camera->lookat.eye.y;

                if (camera->lookat.eye.abs(sp80) < sp10) {
                    var_f31 *= rain_packet->mOverheadFade * 1.0f;
                }
            }

            if (sp30 || rain_packet->mFwdFade1 < 1.0f) {
                cXyz sp74;
                f32 sp0C = 550.0f;
                sp74 = spC0;
                sp74.y = sp9C.y;

                if (sp9C.abs(sp74) < sp0C) {
                    var_f31 *= rain_packet->mFwdFade1;
                }
            }

            if (sp2C || rain_packet->mFwdFade2 < 1.0f) {
                cXyz sp68;
                f32 sp08 = 550.0f;
                sp68 = spC0;
                sp68.y = sp90.y;

                if (sp90.abs(sp68) < sp08) {
                    var_f31 *= rain_packet->mFwdFade2;
                }
            }

            if (sp24) {
                cXyz sp5C;
                if (strcmp(dComIfGp_getStartStageName(), "R_SP30") == 0) {
                    if (dComIfGp_roomControl_getStayNo() == 0) {
                        if (spC0.x > -2680.0f && spC0.z < 2200.0f) {
                            var_f31 = 0.0f;
                        }
                    } else if (dComIfGp_roomControl_getStayNo() == 4 && spC0.z > 1600.0f) {
                        var_f31 = 0.0f;
                    }
                } else if (sp1C == 2) {
                    cXyz sp50;
                    sp50.x = 27453.0f;
                    sp50.y = spC0.y;
                    sp50.z = 8528.0f;

                    if (spC0.y < 1300.0f || player->current.pos.y < 1100.0f) {
                        if (spC0.abs(sp50) > 80.0f) {
                            var_f31 = 0.0f;
                        }
                    } else {
                        if (spC0.abs(sp50) > 500.0f) {
                            var_f31 = 0.0f;
                        }
                    }
                } else if (sp1C == 3) {
                    cXyz sp44;
                    sp44.x = -228.0f;
                    sp44.y = spC0.y;
                    sp44.z = 795.0f;

                    if (spC0.abs(sp44) < 1500.0f) {
                        var_f31 = 0.0f;
                    }

                    if (camera->lookat.eye.y < 0.0f) {
                        var_f31 = 0.0f;
                    }
                } else {
                    sp5C = spC0;
                    sp5C.y = 0.0f;

                    if (sp5C.abs() < sp20) {
                        var_f31 = 0.0f;
                    }
                }
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP113") == 0 &&
                       dComIfGp_roomControl_getStayNo() == 1)
            {
                if (spC0.z < 5100.0f || (spC0.x < -3250.0f && spC0.y < -50.0f) ||
                    (spC0.x < -2700.0f && spC0.z > 15750.0f))
                {
                    var_f31 = 0.0f;
                    rain_packet->mStatus |= (u8)1;
                }
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0 &&
                       dComIfGp_roomControl_getStayNo() == 9)
            {
                if (spC0.x < -3680.0f && spC0.z > -11975.0f && spC0.z < -10530.0f) {
                    var_f31 = 0.0f;
                    rain_packet->mStatus |= (u8)1;
                } else if (spC0.x > 3350.0f && spC0.z > -13027.0f && spC0.z < -11430.0f) {
                    var_f31 = 0.0f;
                    rain_packet->mStatus |= (u8)1;
                }
            } else if (strcmp(dComIfGp_getStartStageName(), "R_SP107") == 0 &&
                       dComIfGp_roomControl_getStayNo() == 2 && spC0.z > 36900.0f)
            {
                var_f31 = 0.0f;
                rain_packet->mStatus |= (u8)1;
            }

            rain_packet->mRainEff[i].mAlpha = var_f31 * (1.0f + cM_rndFX(0.5f));
        }
    }
}

static BOOL d_krain_cut_turn_check() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    BOOL ret = FALSE;

    if (player != NULL && (player->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
                              player->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT ||
                              player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
                              player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT)) {
        ret = TRUE;
    }
    return ret;
}

void dKyr_housi_move() {
    dKankyo_housi_Packet* housi_packet = g_env_light.mpHousiPacket;
    HOUSI_EFF* effect;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz sp84;
    cXyz sp78 = dKyw_get_wind_vecpow();
    cXyz sp6C;
    cXyz sp60;

    dBgS_GndChk gndchk;
    bool var_r27 = 0;
    f32 var_f31 = 1.0f;

    dBgS_CamGndChk_Wtr cam_gndchk;
    f32 var_f30 = -100000000.0f;
    bool var_r24 = 0;

    cXyz sp54;

    if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0) {
        var_r24 = 1;
    }

    if (g_env_light.field_0xea9 == 2) {
        sp54 = camera->lookat.eye;
        sp54.y += 100000.0f;

        cam_gndchk.SetPos(&sp54);
        var_f30 = dComIfG_Bgsp().GroundCross(&cam_gndchk);
    }

    if (dKy_darkworld_check() == true || var_r24 == 1) {
        sp78.x = 0.0f;
        sp78.y = 2.8f;
        sp78.z = 0.0f;
    }

    if (g_env_light.field_0xea9 == 1) {
        sp78.x = 0.0f;
        sp78.y = -0.55f;
        sp78.z = 0.0f;
    }

    if (g_env_light.mHousiCount != 0 ||
        (g_env_light.mHousiCount == 0 && housi_packet->field_0x5de8 <= 0.0f))
    {
        housi_packet->field_0x5dec = g_env_light.mHousiCount;
    }

    if (g_env_light.mHousiCount != 0) {
        cLib_addCalc(&housi_packet->field_0x5de8, 1.0f, 0.2f, 0.05f, 0.01f);
    } else {
        cLib_addCalc(&housi_packet->field_0x5de8, 0.0f, 0.2f, 0.05f, 0.01f);
    }

    if (housi_packet->field_0x5dec == 0) {
        return;
    }

    dKy_set_eyevect_calc2(camera, &sp84, 800.0f, 800.0f);

    if (sp84.abs(housi_packet->field_0x10) > 500.0f) {
        var_r27 = 1;
    }

    housi_packet->field_0x10 = sp84;
    dKyw_get_wind_pow();

    if (g_env_light.field_0xea9 == 1) {
        var_f31 = 0.0f;

        if (g_env_light.camera_water_in_status) {
            dBgS_CamGndChk_Wtr sp90;

            cXyz sp48;
            camera_class* cam_p = dComIfGp_getCamera(0);
            sp48 = cam_p->lookat.eye;
            sp48.y += 100000.0f;

            sp90.SetPos(&sp48);
            f32 gnd_cross = dComIfG_Bgsp().GroundCross(&sp90);
            if (gnd_cross > cam_p->lookat.eye.y) {
                var_f31 = (gnd_cross - cam_p->lookat.eye.y) / 700.0f;
                if (var_f31 < 0.0f) {
                    var_f31 = 0.0f;
                }

                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }
            }
        }
    }

    for (int i = housi_packet->field_0x5dec - 1; i >= 0; i--) {
        f32 var_f26 = 0.4f * housi_packet->field_0x5de8;
        effect = &housi_packet->mHousiEff[i];

        switch (housi_packet->mHousiEff[i].mStatus) {
        case 0:
            if (g_env_light.field_0xea9 == 1) {
                effect->field_0x34 = cM_rndF(0.5f) + 0.1f;
            } else {
                effect->field_0x34 = cM_rndF(1.5f) + 0.2f;
            }

            effect->field_0x3c = 0;
            effect->field_0x4c = cM_rndFX(65536.0f);
            effect->mBasePos.x = sp84.x;
            effect->mBasePos.y = sp84.y;
            effect->mBasePos.z = sp84.z;
            effect->mPosition.x = cM_rndFX(1000.0f);
            effect->mPosition.y = cM_rndFX(1000.0f);
            effect->mPosition.z = cM_rndFX(1000.0f);
            effect->mAlpha = 0.0f;
            effect->field_0x48 = 0.0f;
            effect->mScale.x = cM_rndF(360.0f);
            effect->mScale.y = cM_rndF(360.0f);
            effect->mScale.z = cM_rndF(360.0f);
            effect->mSpeed.x = 0.0f;
            effect->mSpeed.y = 0.0f;
            effect->mSpeed.z = 0.0f;

            if (effect->mBasePos.y + effect->mPosition.y < -100149.9f) {
                effect->mPosition.y = (-99999.9f - effect->mBasePos.y) + 10.0f;
            }

            effect->field_0x38 = 0.0f;
            effect->field_0x44 = 0.0f;
            effect->mStatus++;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            f32 var_f24 = 2.5f;

            if (effect->mStatus != 4) {
                f32 var_f23 = effect->field_0x34;
                if (effect->mStatus == 2) {
                    var_f23 *= 0.25f;
                }

                f32 temp_f0_5 = cM_fsin(effect->mScale.x);
                if (g_env_light.field_0xea9 == 2) {
                    var_f24 = 5.0f;
                }

                if (effect->mStatus != 3) {
                    effect->mPosition.y += var_f23 * (sp78.y * var_f24);
                    effect->mPosition.x += var_f23 * (sp78.x * var_f24);
                    effect->mPosition.y -= var_f23 * 0.6f;

                    if (g_env_light.field_0xea9 == 2) {
                        if (g_env_light.fishing_hole_season == 3) {
                            effect->mPosition.y -= var_f23 * 3.0f;
                        } else {
                            effect->mPosition.y -= var_f23 * 1.5f;
                        }
                    }

                    effect->mPosition.z += var_f23 * (sp78.z * var_f24);
                } else {
                    var_f23 *= 4.5f;

                    effect->mPosition.x += var_f23 * (sp78.x * var_f24);
                    effect->mPosition.y += (var_f23 * (sp78.y * var_f24)) * 0.75f;
                    effect->mPosition.y += var_f23 * 0.3f;
                    effect->mPosition.z += var_f23 * (sp78.z * var_f24);
                }

                effect->mPosition.x += temp_f0_5 * var_f23;
                effect->mPosition.y += var_f23 * 0.5f * cM_fsin(effect->mScale.y);
                effect->mPosition.z += cM_fsin(effect->mScale.z) * var_f23;
            } else if (d_krain_cut_turn_check()) {
                effect->mStatus = 3;
            }

            effect->mScale.x += 0.03f;
            effect->mScale.y += 0.02f;
            effect->mScale.z += 0.01f;

            sp6C.x = effect->mBasePos.x + effect->mPosition.x;
            sp6C.y = effect->mBasePos.y + effect->mPosition.y;
            sp6C.z = effect->mBasePos.z + effect->mPosition.z;

            if (g_env_light.field_0xea9 == 2) {
                cXyz sp3C(sp6C);

                if (sp6C.y <= var_f30) {
                    effect->mStatus = 2;
                }

                if (effect->mStatus == 2) {
                    effect->mPosition.y = var_f30 - effect->mBasePos.y;
                } else if (effect->mStatus != 3 && effect->mStatus != 4) {
                    sp3C.y = player->current.pos.y;

                    if (sp3C.abs(player->current.pos) < 300.0f) {
                        if (sp3C.z > 5600.0f && player->current.pos.y < 130.0f) {
                            if (sp6C.y < player->current.pos.y + 2.0f) {
                                effect->mPosition.y =
                                    (player->current.pos.y + 2.0f) - effect->mBasePos.y;
                                effect->mStatus = 4;
                            }
                        } else {
                            effect->mStatus = 3;
                        }
                    }
                } else {
                    if (effect->mStatus == 4) {
                        effect->mPosition.y =
                            (player->current.pos.y + 2.0f) - effect->mBasePos.y;
                    }

                    if (sp3C.abs(player->current.pos) > 400.0f) {
                        effect->mStatus = 1;
                    }
                }
            }

            sp60 = dKyw_pntwind_get_vecpow(&sp6C);

            if (effect->mSpeed.x < 30.0f) {
                effect->mSpeed.x += sp60.x * 9.0f;
            }

            if (effect->mSpeed.y < 30.0f) {
                effect->mSpeed.y += sp60.y * 9.0f;
            }

            if (effect->mSpeed.z < 30.0f) {
                effect->mSpeed.z += sp60.z * 9.0f;
            }

            cLib_addCalc(&effect->mSpeed.x, 0.0f, 0.2f, 0.1f, 0.00001f);
            cLib_addCalc(&effect->mSpeed.y, 0.0f, 0.2f, 0.1f, 0.00001f);
            cLib_addCalc(&effect->mSpeed.z, 0.0f, 0.2f, 0.1f, 0.00001f);

            effect->mPosition.x += effect->mSpeed.x;
            effect->mPosition.y += effect->mSpeed.y;
            effect->mPosition.z += effect->mSpeed.z;

            sp6C.x = effect->mBasePos.x + effect->mPosition.x;
            sp6C.y = effect->mBasePos.y + effect->mPosition.y;
            sp6C.z = effect->mBasePos.z + effect->mPosition.z;

            f32 var_f1_4 = sp6C.abs(sp84);

            if (effect->field_0x3c == 0) {
                if (var_f1_4 > 1000.0f || sp6C.y < -99979.9f) {
                    effect->field_0x3c = 10;
                    effect->mBasePos = sp84;

                    if (sp6C.abs(sp84) > 1050.0f) {
                        effect->mPosition.x = cM_rndFX(1000.0f);
                        effect->mPosition.y = cM_rndFX(1000.0f);
                        effect->mPosition.z = cM_rndFX(1000.0f);
                    } else {
                        f32 temp_f23 = cM_rndFX(50.0f);
                        get_vectle_calc(&sp6C, &sp84, &sp60);

                        effect->mPosition.x = sp60.x * (temp_f23 + 1000.0f);
                        effect->mPosition.y = sp60.y * (temp_f23 + 1000.0f);
                        effect->mPosition.z = sp60.z * (temp_f23 + 1000.0f);
                    }

                    sp6C.x = effect->mBasePos.x + effect->mPosition.x;
                    sp6C.y = effect->mBasePos.y + effect->mPosition.y;
                    sp6C.z = effect->mBasePos.z + effect->mPosition.z;

                    if (sp6C.y <= var_f30) {
                        effect->mPosition.y += 1000.0f;
                    }

                    effect->mSpeed.x = 0.0f;
                    effect->mSpeed.y = 0.0f;
                    effect->mSpeed.z = 0.0f;

                    if (g_env_light.field_0xea9 == 2) {
                        effect->mPosition.y += cM_rndF(3200.0f);
                        if (sp6C.y > 3200.0f) {
                            effect->mPosition.y = 3200.0f - effect->mBasePos.y;
                        }

                        if (g_env_light.fishing_hole_season == 1) {
                            if (sp6C.x > 600.0f || sp6C.z > 1600.0f) {
                                effect->mStatus = 1;
                            } else {
                                effect->mStatus = 2;
                            }
                        } else if (sp6C.x > 1700.0f || sp6C.z > 2800.0f) {
                            effect->mStatus = 1;
                        } else {
                            effect->mStatus = 2;
                        }
                    }
                }
            } else {
                effect->field_0x3c--;
            }
            break;
        }

        sp6C.x = effect->mBasePos.x + effect->mPosition.x;
        sp6C.y = effect->mBasePos.y + effect->mPosition.y;
        sp6C.z = effect->mBasePos.z + effect->mPosition.z;

        if (g_env_light.field_0xea9 != 2) {
            effect->field_0x4c += 600;
            if (effect->field_0x4c > 30000) {
                var_f26 = 0.0f;
            }
        } else {
            var_f26 = 1.0f;
        }

        cLib_addCalc(&effect->mAlpha, var_f26, 0.5f, 0.02f, 0.00001f);
        effect->mAlpha *= var_f31;

        if (var_r27 != 0) {
            effect->mAlpha = 0.0f;
        }

        if (dKy_darkworld_check() == true || var_r24 == 1) {
            f32 var_f1_6 = sp6C.abs(camera->lookat.eye);
            effect->field_0x48 = var_f1_6;

            f32 var_f1_7;
            if (var_f1_6 >= 800.0f) {
                var_f1_7 = (var_f1_6 - 800.0f) / 825.0f;
                if (var_f1_7 > 1.0f) {
                    var_f1_7 = 1.0f;
                }
            } else {
                var_f1_7 = 0.0f;
            }

            effect->mAlpha = var_f1_7;
        }

        f32 var_f1_8 = sp6C.abs(camera->lookat.eye);
        f32 temp_f25 = var_f1_8 / 2000.0f;
        effect->field_0x48 = 1.0f - (temp_f25 * temp_f25);
    }
}

void dKyr_snow_init() {
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    g_env_light.mpSnowPacket = new (32) dKankyo_snow_Packet();

    if (g_env_light.mpSnowPacket == NULL) {
        return;
    }

    g_env_light.mpSnowPacket->mpTex = (u8*)dComIfG_getObjectRes("Always", 0x56);

    for (int i = 0; i < 500; i++) {
        g_env_light.mpSnowPacket->mSnowEff[i].mStatus = 0;
    }

    g_env_light.mpSnowPacket->field_0x6d88 = 0;
    g_env_light.mpSnowPacket->field_0x6d74 = camera->lookat.eye;
    g_env_light.mpSnowPacket->field_0x6d80 = 0.0f;
    g_env_light.mpSnowPacket->field_0x6d84 = 0.0f;
    g_env_light.mpSnowPacket->field_0x6d8a = 0;
    g_env_light.mpSnowPacket->field_0x6d8c = 0;
    g_env_light.mpSnowPacket->field_0x6d90 = 0;
    g_env_light.mpSnowPacket->field_0x6d91 = 0;
    g_env_light.mpSnowPacket->field_0x6d92 = 0;
    g_env_light.mpSnowPacket->field_0x6d93 = 0;
}

void dKyr_snow_move() {
    dKankyo_snow_Packet* snow_packet = g_env_light.mpSnowPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    DOUBLE_POS spC8;
    cXyz spB8;
    cXyz spAC;
    cXyz spA0;
    cXyz sp94 = dKyw_get_wind_vecpow();
    cXyz sp88 = dKyw_get_wind_vecpow();
    cXyz sp7C;
    cXyz sp70;

    dBgS_ObjGndChk_All spE0;
    cXyz sp64;

    if (snow_packet->field_0x6d88 <= g_env_light.mSnowCount) {
        snow_packet->field_0x6d88 = g_env_light.mSnowCount;
    }

    if (snow_packet->field_0x6d88 == 0) {
        return;
    }

    sp64 = player->current.pos;
    sp64.y += 100.0f;
    spE0.SetPos(&sp64);
    f32 temp_f19 = dComIfG_Bgsp().GroundCross(&spE0);

    dKy_set_eyevect_calc2(camera, &spB8, 500.0f, 500.0f);
    dKy_set_eyevect_calc2(camera, &spAC, 1000.0f, 1000.0f);

    if (snow_packet->field_0x6d74.abs(camera->lookat.eye) > 1500.0f) {
        snow_packet->field_0x6d88 = 0;
    }

    snow_packet->field_0x6d74 = camera->lookat.eye;
    spA0.x = spA0.y = spA0.z = 0.0f;

    cXyz* temp_r21 = dKyw_get_wind_vec();
    f32 var_f20 = dKyw_get_wind_pow();
    if (g_env_light.field_0xe92 == 1) {
        var_f20 *= 1.2f;
    }

    if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
        sp94.x = 0.3f;
        sp94.y = 0.0f;
        sp94.z = 0.0f;
        var_f20 = 0.3f;
    }

    cXyz sp58;
    spC8.x = camera->lookat.center.x - camera->lookat.eye.x;
    spC8.y = 0.0f;
    spC8.z = camera->lookat.center.z - camera->lookat.eye.z;
    vectle_calc(&spC8, &sp58);

    snow_packet->field_0x6d84 =
        cM3d_VectorProduct2d(0.0f, 0.0f, -temp_r21->x, -temp_r21->z, sp58.x, sp58.z);

    f32 temp_f20_1 = fabsf((temp_r21->x * sp58.x) + (temp_r21->z * sp58.z));
    snow_packet->field_0x6d80 = ((1.0f - temp_f20_1) * var_f20) * (1.0f - fabsf(temp_r21->y + 0.3f));
    snow_packet->field_0x6d80 *= fabsf(snow_packet->field_0x6d84);

    for (int i = snow_packet->field_0x6d88 - 1; i >= 0; i--) {
        f32 gravity = -(2.0f + cM_rndF(6.5f));
        f32 speed = 2.0f * (5.0f + (f32)(i & 15));

        switch (snow_packet->mSnowEff[i].mStatus) {
        case 0:
            snow_packet->mSnowEff[i].mWindSpeed = speed;
            snow_packet->mSnowEff[i].mGravity = gravity;
            snow_packet->mSnowEff[i].mTimer = 0;
            snow_packet->mSnowEff[i].mBasePos.x = spAC.x + cM_rndFX(1100.0f);
            snow_packet->mSnowEff[i].mBasePos.y = spAC.y + 1100.0f;
            snow_packet->mSnowEff[i].mBasePos.z = spAC.z + cM_rndFX(1100.0f);
            snow_packet->mSnowEff[i].mPosition.x = spB8.x + cM_rndFX(550.0f);
            snow_packet->mSnowEff[i].mPosition.y = spB8.y + 550.0f;
            snow_packet->mSnowEff[i].mPosition.z = spB8.z + cM_rndFX(550.0f);
            snow_packet->mSnowEff[i].mScale = 0.0f;
            snow_packet->mSnowEff[i].mPosWaveX = cM_rndF(65536.0f);
            snow_packet->mSnowEff[i].mPosWaveZ = cM_rndF(65536.0f);
            snow_packet->mSnowEff[i].mStatus++;
            break;
        case 1:
            cLib_addCalc(&snow_packet->mSnowEff[i].mWindSpeed, snow_packet->mSnowEff[i].mWindSpeed - cM_rndFX(0.08f), 0.5f, 0.1f, 0.01f);

            cXyz sp4C;
            sp88 = sp94;
            if (camera->lookat.eye.abs(snow_packet->mSnowEff[i].mPosition) < 500.0f &&
                snow_packet->mSnowEff[i].mPosition.y < temp_f19 + 250.0f)
            {
                f32 var_f1_3 = ((temp_f19 + 250.0f) - snow_packet->mSnowEff[i].mPosition.y) / 250.0f;
                if (var_f1_3 > 1.0f) {
                    var_f1_3 = 1.0f;
                }

                sp88.y = var_f1_3 * 0.45f;
            }

            s16 wave_x = snow_packet->mSnowEff[i].mPosWaveX;
            s16 wave_z = snow_packet->mSnowEff[i].mPosWaveZ;

            sp4C.x = cM_scos(wave_x) * cM_ssin(wave_z);
            sp4C.y = cM_ssin(wave_x);
            sp4C.z = cM_scos(wave_x) * cM_scos(wave_z);

            snow_packet->mSnowEff[i].mPosition.x += sp88.x * snow_packet->mSnowEff[i].mWindSpeed;
            snow_packet->mSnowEff[i].mPosition.z += sp88.z * snow_packet->mSnowEff[i].mWindSpeed;
            snow_packet->mSnowEff[i].mPosition.y += snow_packet->mSnowEff[i].mGravity + (sp88.y * snow_packet->mSnowEff[i].mWindSpeed);

            snow_packet->mSnowEff[i].mPosition.x += sp4C.x * 5.3f;
            snow_packet->mSnowEff[i].mPosition.y += sp4C.y * 5.3f;
            snow_packet->mSnowEff[i].mPosition.z += sp4C.z * 5.3f;

            sp88 = sp94;
            if (camera->lookat.eye.abs(snow_packet->mSnowEff[i].mPosition) < 500.0f &&
                snow_packet->mSnowEff[i].mBasePos.y < temp_f19 + 250.0f)
            {
                f32 var_f1_5 = ((temp_f19 + 250.0f) - snow_packet->mSnowEff[i].mBasePos.y) / 250.0f;
                if (var_f1_5 > 1.0f) {
                    var_f1_5 = 1.0f;
                }

                sp88.y = var_f1_5 * 0.35f;
            }

            snow_packet->mSnowEff[i].mBasePos.x += sp88.x * snow_packet->mSnowEff[i].mWindSpeed;
            snow_packet->mSnowEff[i].mBasePos.z += sp88.z * snow_packet->mSnowEff[i].mWindSpeed;
            snow_packet->mSnowEff[i].mBasePos.y += snow_packet->mSnowEff[i].mGravity + (sp88.y * snow_packet->mSnowEff[i].mWindSpeed);

            snow_packet->mSnowEff[i].mBasePos.x += sp4C.x * 5.3f;
            snow_packet->mSnowEff[i].mBasePos.y += sp4C.y * 5.3f;
            snow_packet->mSnowEff[i].mBasePos.z += sp4C.z * 5.3f;

            cLib_addCalc(&snow_packet->mSnowEff[i].mPosWaveX, snow_packet->mSnowEff[i].mPosWaveX + cM_rndF(3000.0f), 0.25f,
                         1500.0f, 0.001f);
            cLib_addCalc(&snow_packet->mSnowEff[i].mPosWaveZ, snow_packet->mSnowEff[i].mPosWaveZ + cM_rndF(3000.0f), 0.25f,
                         1500.0f, 0.001f);

            sp7C = snow_packet->mSnowEff[i].mPosition;
            f32 var_f1_6 = sp7C.abs(spB8);

            if (snow_packet->mSnowEff[i].mTimer == 0) {
                if (var_f1_6 > 550.0f) {
                    snow_packet->mSnowEff[i].mTimer = 10;
                    snow_packet->mSnowEff[i].mWindSpeed = speed;
                    snow_packet->mSnowEff[i].mGravity = gravity;

                    if (sp7C.abs(spB8) > 600.0f) {
                        snow_packet->mSnowEff[i].mPosition.x = spB8.x + cM_rndFX(550.0f);
                        snow_packet->mSnowEff[i].mPosition.y = spB8.y + cM_rndFX(550.0f);
                        snow_packet->mSnowEff[i].mPosition.z = spB8.z + cM_rndFX(550.0f);
                    } else {
                        f32 temp_f26_2 = cM_rndFX(27.5f);
                        get_vectle_calc(&sp7C, &spB8, &sp70);

                        snow_packet->mSnowEff[i].mPosition.x = spB8.x + sp70.x * (temp_f26_2 + 550.0f);
                        snow_packet->mSnowEff[i].mPosition.y = spB8.y + sp70.y * (temp_f26_2 + 550.0f);
                        snow_packet->mSnowEff[i].mPosition.z = spB8.z + sp70.z * (temp_f26_2 + 550.0f);
                    }
                }
            } else {
                snow_packet->mSnowEff[i].mTimer--;
            }

            sp7C = snow_packet->mSnowEff[i].mBasePos;
            f32 sp18 = sp7C.abs(spAC);
            if (sp18 > 1100.0f) {
                if (sp7C.abs(spAC) > 1150.0f) {
                    snow_packet->mSnowEff[i].mBasePos.x = spAC.x + cM_rndFX(1100.0f);
                    snow_packet->mSnowEff[i].mBasePos.y = spAC.y + cM_rndFX(1100.0f);
                    snow_packet->mSnowEff[i].mBasePos.z = spAC.z + cM_rndFX(1100.0f);
                } else {
                    f32 temp_f26_3 = cM_rndFX(55.0f);
                    get_vectle_calc(&sp7C, &spAC, &sp70);

                    snow_packet->mSnowEff[i].mBasePos.x = spAC.x + sp70.x * (temp_f26_3 + 1100.0f);
                    snow_packet->mSnowEff[i].mBasePos.y = spAC.y + sp70.y * (temp_f26_3 + 1100.0f);
                    snow_packet->mSnowEff[i].mBasePos.z = spAC.z + sp70.z * (temp_f26_3 + 1100.0f);
                }
            }
        }

        sp7C = snow_packet->mSnowEff[i].mPosition;

        f32 var_f1_11 = sp7C.abs(camera->lookat.eye);
        f32 var_f26 = var_f1_11 / 100.0f;
        if (var_f26 > 1.0) {
            var_f26 = 1.0;
        }

        var_f26 *= 0.4f;

        var_f1_11 = sp7C.abs(spB8);
        if (var_f1_11 > 300.0f) {
            f32 var_f1_12 = (550.0f - var_f1_11) / 250.0f;
            if (var_f1_12 < 0.0f) {
                var_f1_12 = 0.0f;
            }

            var_f26 *= var_f1_12;
        }

        if (i > g_env_light.mSnowCount - 1) {
            var_f26 = 0.0f;
            cLib_addCalc(&snow_packet->mSnowEff[i].mScale, var_f26, 0.2f, 0.1f, 0.01f);
        } else {
            snow_packet->mSnowEff[i].mScale = var_f26;
        }

        if (i > g_env_light.mSnowCount - 1 && snow_packet->mSnowEff[i].mScale < 0.01f) {
            if (i == snow_packet->field_0x6d88 - 1) {
                snow_packet->field_0x6d88--;
            }
        }

        if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
            if (sp7C.z > -340.0f) {
                snow_packet->mSnowEff[i].mScale = 0.0f;
            }
        } else if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
            if (sp7C.z > 9800.0f) {
                snow_packet->mSnowEff[i].mScale = 0.0f;
            }
        }

        sp7C = snow_packet->mSnowEff[i].mBasePos;

        var_f1_11 = sp7C.abs(camera->lookat.eye);
        f32 temp_f29 = var_f1_11 / 100.0f;
        if (temp_f29 > 1.0) {
            temp_f29 = 1.0;
        }

        temp_f29 *= 0.38f;

        var_f1_11 = sp7C.abs(spAC);
        if (var_f1_11 > 850.0f) {
            f32 var_f1_15 = (1100.0f - var_f1_11) / 250.0f;
            if (var_f1_15 < 0.0f) {
                var_f1_15 = 0.0f;
            }

            temp_f29 *= var_f1_15;
        }

        if (i > g_env_light.mSnowCount - 1) {
            temp_f29 = 0.0f;
            cLib_addCalc(&snow_packet->mSnowEff[i].field_0x30, temp_f29, 0.2f, 0.1f, 0.01f);
        } else {
            snow_packet->mSnowEff[i].field_0x30 = temp_f29;
        }

        if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
            if (sp7C.z > -340.0f) {
                snow_packet->mSnowEff[i].field_0x30 = 0.0f;
            }
        } else if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
            if (sp7C.z > 9800.0f) {
                snow_packet->mSnowEff[i].field_0x30 = 0.0f;
            }
        }
    }
}

void dKyr_star_init() {
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    g_env_light.mpStarPacket = new (32) dKankyo_star_Packet();

    if (g_env_light.mpStarPacket == NULL) {
        return;
    }

    g_env_light.mpStarPacket->mpTex = (u8*)dComIfG_getObjectRes("Always", 0x4a);
    g_env_light.mpStarPacket->mEffect[0].field_0x28 = 1.0f;
    g_env_light.mpStarPacket->mEffectNum = 0;
}


void dKyr_star_move() {
    dKankyo_star_Packet* packet = g_env_light.mpStarPacket;
    packet->mEffectNum = g_env_light.mStarCount;

    if (packet->mEffectNum != 0) {
        packet->mEffect[0].field_0x28 = 1.0f;
    }
}

void cloud_shadow_move() {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_cloud_Packet* packet = g_env_light.mpCloudPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    Mtx camMtx;
    cXyz wind_vecpow = dKyw_get_wind_vecpow();
    cXyz* wind_vec = dKyw_get_wind_vec();
    cXyz center;
    cXyz spA0;
    cXyz sp94;
    cXyz sp88;
    cXyz sp7C;
    cXyz pos;
    cXyz sp64;
    cXyz sp58;

    f32 sp44 = 400.0f;
    f32 sp40;
    f32 sp3C = 1.0f;
    f32 wind_pow = dKyw_get_wind_pow();

    if (dComIfGd_getView() != NULL) {
        MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
    } else {
        return;
    }

    if (packet->mCount <= g_env_light.mMoyaCount) {
        packet->mCount = (s16)g_env_light.mMoyaCount;
    }

    if (packet->mCount != 0) {
        if (g_env_light.mMoyaMode == 8 || g_env_light.mMoyaMode == 10) {
            if ((g_Counter.mCounter0 & 128) == 0) {
                cLib_addCalc(&g_env_light.field_0xebc, 1.0f, 0.1f, 0.01f + cM_rndFX(0.005f), 0.000001f);
            } else {
                cLib_addCalc(&g_env_light.field_0xebc, 0.3f, 0.1f, 0.01f, 0.000001f);
            }

            if (g_env_light.wether >= 3) {
                cLib_addCalc(&g_env_light.field_0xebc, 1.0f, 0.5f, 0.1f, 0.000001f);
            }
        }

        if (g_env_light.mMoyaMode == 4 || g_env_light.mMoyaMode == 11) {
            wind_vecpow.x = wind_vec->x * (0.6f + (0.4f * dKyw_get_wind_pow()));
            wind_vecpow.y = wind_vec->y * (0.6f + (0.4f * dKyw_get_wind_pow()));
            wind_vecpow.z = wind_vec->z * (0.6f + (0.4f * dKyw_get_wind_pow()));
        }

        f32 sp28;
        if (dComIfGd_getView() != NULL) {
            sp28 = dComIfGd_getView()->fovy / 40.0f;
            if (sp28 >= 1.0f) {
                sp28 = 1.0f;
            }
        } else {
            sp28 = 1.0f;
        }

        sp40 = 1200.0f;
        f32 rnd_pos = 1400.0f;
        dKy_set_eyevect_calc2(camera, &center, sp40, sp40);

        for (int i = 0; i < packet->mCount; i++) {
            switch (packet->mCloudEff[i].mStatus) {
            case 0:
                packet->mCloudEff[i].mBasePos.x = center.x;
                packet->mCloudEff[i].mBasePos.y = center.y;
                if (g_env_light.mMoyaMode == 8) {
                    packet->mCloudEff[i].mBasePos.y = -300.0f;
                }
                packet->mCloudEff[i].mBasePos.z = center.z;

                packet->mCloudEff[i].mPosition.x = cM_rndFX(rnd_pos);
                packet->mCloudEff[i].mPosition.y = cM_rndFX(rnd_pos);
                packet->mCloudEff[i].mPosition.z = cM_rndFX(rnd_pos);

                packet->mCloudEff[i].field_0x28 = 0.5f + cM_rndF(0.5f);
                packet->mCloudEff[i].mAlpha = 0.0f;
                packet->mCloudEff[i].field_0x2c = cM_rndF(65535.0f);
                packet->mCloudEff[i].mPntWindSpeed.x = 0.0f;
                packet->mCloudEff[i].mPntWindSpeed.y = 0.0f;
                packet->mCloudEff[i].mPntWindSpeed.z = 0.0f;

                packet->mCloudEff[i].mStatus++;
                break;
            case 1:
            case 2: {
                pos.x = packet->mCloudEff[i].mBasePos.x + packet->mCloudEff[i].mPosition.x;
                pos.y = packet->mCloudEff[i].mBasePos.y + packet->mCloudEff[i].mPosition.y;
                pos.z = packet->mCloudEff[i].mBasePos.z + packet->mCloudEff[i].mPosition.z;

                f32 sp34 = (0.5f * (1.0f + cM_ssin(packet->mCloudEff[i].field_0x2c)));
                f32 sp30 = cM_scos(packet->mCloudEff[i].field_0x2c);
                packet->mCloudEff[i].field_0x2c += 100;
                f32 sp2C = (sp44 * packet->mCloudEff[i].field_0x28);

                if (g_env_light.mMoyaMode != 5) {
                    packet->mCloudEff[i].mPosition.x += (0.6f + (0.4f * sp34)) * (sp2C * (wind_vecpow.x * (wind_vecpow.x * wind_vecpow.x)));
                    packet->mCloudEff[i].mPosition.y += (0.6f * (sp2C * (wind_vecpow.y * (wind_vecpow.y * wind_vecpow.y)))) + (0.08f * sp2C * (sp30 * dKyw_get_wind_pow()));
                    packet->mCloudEff[i].mPosition.z += (0.6f + (0.4f * sp34)) * (sp2C * (wind_vecpow.z * (wind_vecpow.z * wind_vecpow.z)));
                } else {
                    packet->mCloudEff[i].mPosition.x += sp30;
                    packet->mCloudEff[i].mPosition.y += (0.6f * (0.027000003f * sp2C)) + (0.3f * sp30 * (0.08f * sp2C));
                    packet->mCloudEff[i].mPosition.z += sp30;
                }

                if (g_env_light.mMoyaMode == 6) {
                    packet->mCloudEff[i].mPosition.y += 23.0f * packet->mCloudEff[i].field_0x28;
                } else if (g_env_light.mMoyaMode == 8) {
                    if ((i & 1) == 0) {
                        packet->mCloudEff[i].mPosition.y += 20.0f * packet->mCloudEff[i].field_0x28;
                    }
                } else if (g_env_light.mMoyaMode == 10) {
                    packet->mCloudEff[i].mPosition.y += 20.0f * packet->mCloudEff[i].field_0x28 * dKyw_get_wind_pow();
                } else if (g_env_light.mMoyaMode == 11) {
                    packet->mCloudEff[i].mPosition.y += 120.0f * packet->mCloudEff[i].field_0x28 * dKyw_get_wind_pow();
                }

                pos.x = packet->mCloudEff[i].mBasePos.x + packet->mCloudEff[i].mPosition.x;
                pos.y = packet->mCloudEff[i].mBasePos.y + packet->mCloudEff[i].mPosition.y;
                pos.z = packet->mCloudEff[i].mBasePos.z + packet->mCloudEff[i].mPosition.z;

                if (pos.abs(center) > rnd_pos) {
                    packet->mCloudEff[i].mBasePos.x = center.x;
                    packet->mCloudEff[i].mBasePos.y = center.y;
                    if (g_env_light.mMoyaMode == 8) {
                        packet->mCloudEff[i].mBasePos.y = -300.0f;
                    }
                    packet->mCloudEff[i].mBasePos.z = center.z;

                    if (pos.abs(center) > rnd_pos + (0.1f * rnd_pos)) {
                        packet->mCloudEff[i].mPosition.x = cM_rndFX(rnd_pos);
                        packet->mCloudEff[i].mPosition.y = cM_rndFX(rnd_pos);
                        packet->mCloudEff[i].mPosition.z = cM_rndFX(rnd_pos);
                    } else {
                        cLib_addCalc(&packet->mCloudEff[i].mAlpha, 0.0f, 0.5f, 0.1f, 0.01f);
                        if (packet->mCloudEff[i].mAlpha < 0.01f) {
                            get_vectle_calc(&pos, &center, &sp64);
                            sp64.x += cM_rndF(0.5f);
                            sp64.y += cM_rndF(0.5f);
                            sp64.z += cM_rndF(0.5f);

                            packet->mCloudEff[i].mPosition.x = sp64.x * rnd_pos;
                            packet->mCloudEff[i].mPosition.y = sp64.y * rnd_pos;
                            packet->mCloudEff[i].mPosition.z = sp64.z * rnd_pos;
                        }
                    }

                    packet->mCloudEff[i].mAlpha = 0.0f;
                }
                break;
            }
            case 3:
                packet->mCloudEff[i].mStatus = 0;
                break;
            }

            pos.x = packet->mCloudEff[i].mBasePos.x + packet->mCloudEff[i].mPosition.x;
            pos.y = packet->mCloudEff[i].mBasePos.y + packet->mCloudEff[i].mPosition.y;
            pos.z = packet->mCloudEff[i].mBasePos.z + packet->mCloudEff[i].mPosition.z;
            
            f32 sp20 = 0.0f;
            f32 sp1C = 700.0f;
            f32 sp24 = pos.abs(camera->lookat.eye);

            f32 sp18 = (sp24 / rnd_pos);
            if (sp18 > 1.0f) {
                sp18 = 1.0f;
            }
            sp18 = sp18;

            f32 alpha_target;
            if (sp24 < sp1C) {
                if (sp24 < sp1C - 400.0f) {
                    alpha_target = 0.0f;
                } else {
                    alpha_target = (sp24 - (sp1C - 400.0f)) / (sp1C - (sp1C - 400.0f));
                    if (alpha_target < 0.0f) {
                        alpha_target = 0.0f;
                    } else if (alpha_target > 1.0f) {
                        alpha_target = 1.0f;
                    }
                }
            } else {
                f32 sp14 = 0.8f * (sp40 + rnd_pos);
                if (sp24 < 0.5f * sp14) {
                    alpha_target = 1.0f;
                } else {
                    alpha_target = (sp24 - (0.5f * sp14)) / (sp14 - (0.5f * sp14));
                    if (alpha_target < 0.0f) {
                        alpha_target = 0.0f;
                    } else if (alpha_target > 1.0f) {
                        alpha_target = 1.0f;
                    }
                    alpha_target = 1.0f - alpha_target;
                }
            }

            packet->mCloudEff[i].mSize = (44.0f + (408.0f * sp18)) * sp28;

            f32 alpha_max = 0.12f;
            if (g_env_light.mMoyaMode == 1 || g_env_light.mMoyaMode == 8) {
                alpha_max = 0.24f;
            } else if (g_env_light.mMoyaMode == 10) {
                alpha_max = 0.2f * dKyw_get_wind_pow();
            } else if (g_env_light.mMoyaMode == 11) {
                alpha_max = dKyw_get_wind_pow() * dKyw_get_wind_pow();
            } else if (g_env_light.mMoyaMode == 2) {
                alpha_max = 0.36f;
            } else if (g_env_light.mMoyaMode == 4) {
                alpha_max = 0.38f * dKyw_get_wind_pow();
            } else if (g_env_light.mMoyaMode == 50) {
                alpha_max = 0.25f * (envlight->senses_effect_strength * envlight->senses_effect_strength * envlight->senses_effect_strength);
                packet->mCloudEff[i].mAlpha = alpha_max * alpha_target;
            } else if (g_env_light.mMoyaMode == 5) {
                alpha_max = 0.09f;
            } else if (g_env_light.mMoyaMode == 6) {
                alpha_max = 0.13f;
            }

            if (g_env_light.field_0xe92 == 1) {
                alpha_max = 0.3f;
            }

            if (g_env_light.mMoyaMode == 9) {
                if (dKyw_get_wind_pow() > 0.7f) {
                    alpha_max = 0.36f;
                } else {
                    alpha_max = 0.1f;
                }
            }

            f32 sp10 = 1.0f;
            s8 sp8;
            sp10 = dKy_move_room_ratio(&player->tevStr, &sp8);
            packet->mCloudEff[i].mAlpha *= sp10;

            if (i > g_env_light.mMoyaCount - 1) {
                alpha_max = 0.0f;
                if (packet->mCloudEff[i].mAlpha < 0.001f && i == packet->mCount - 1) {
                    packet->mCount--;
                }
            }

            if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0) {
                dKankyo_mud_Packet* mud_packet = g_env_light.mpMudPacket;
                if (mud_packet != NULL) {
                    alpha_max *= mud_packet->field_0x1c3c;
                }
            }

            cLib_addCalc(&packet->mCloudEff[i].mAlpha, alpha_max * alpha_target, 0.1f, 0.1f, 0.001f);
        }
    }
}

void vrkumo_move() {
    cXyz wind_vecpow = dKyw_get_wind_vecpow();
    dKankyo_vrkumo_Packet* vrkumo_packet = g_env_light.mpVrkumoPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    cXyz sp80;

    f32 sp3C = 0.0f;
    cXyz* wind_vec = dKyw_get_wind_vec();

    f32 sp70, sp6C, sp68, sp64, sp60;
    f32 sp5C, sp58, sp54, sp50, sp4C;
    if (dKy_darkworld_check()) {
        sp70 = 80.0f;
        sp6C = 0.0f;
        sp68 = 0.0f;
        sp64 = 0.98f;
        sp60 = 0.75f;
        sp5C = sp70;
        sp58 = sp6C;
        sp54 = sp68;
        sp50 = sp64;
        sp4C = sp60;
    } else {
        sp70 = 10.0f;
        sp6C = 2500.0f;
        sp68 = 2500.0f;
        sp64 = 0.99f;
        sp60 = 1.0f;
        sp5C = 25.0f;
        sp58 = 1200.0f;
        sp54 = 1200.0f;
        sp50 = 0.95f;
        sp4C = 1.0f;
    }

    if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0) {
        sp58 = 4000.0f;
        sp54 = 2000.0f;
    }

    f32 strength = g_env_light.mVrkumoStrength;
    sp70 += strength * (sp5C - sp70);
    sp6C += strength * (sp58 - sp6C);
    sp68 += strength * (sp54 - sp68);
    sp64 += strength * (sp50 - sp64);
    sp60 += strength * (sp4C - sp60);

    int stage_type = dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo());

    if (dComIfGd_getView() != NULL) {
        f32 sp2C = 0.0f;

        dStage_FileList_dt_c* filelist = NULL;
        if (dComIfGp_roomControl_getStayNo() >= 0) {
            filelist = dComIfGp_roomControl_getStatusRoomDt(dComIfGp_roomControl_getStayNo())->getFileListInfo();
        }

        if (filelist != NULL) {
            sp2C = dStage_FileList_dt_SeaLevel(filelist);
            #if DEBUG
            if (g_kankyoHIO.vrbox.field_0x14) {
                sp2C = g_kankyoHIO.vrbox.m_horizonHeight;
            } else {
                g_kankyoHIO.vrbox.m_horizonHeight = sp2C;
            }
            #endif
        }

        sp6C -= 0.09f * (camera->lookat.eye.y - sp2C);
    }

    for (int i = 0; i < 100; i++) {
        s16 sp8;
        f32 sp34;
        f32 var_f31;
        switch (vrkumo_packet->mVrkumoEff[i].mStatus) {
        case 0:
            sp8 = cM_rndF(65535.0f);
            sp34 = cM_rndF(18000.0f);
            if (sp34 > 15000.0f) {
                sp34 = 14000.0 + cM_rndF(1000.0f);
            }

            var_f31 = sp34 * cM_ssin(sp8);
            // @bug - parenthesis should not be on the condition
            if ((f32)fabs(var_f31 < 5000.0f)) {
                if (var_f31 > 0.0f) {
                    var_f31 += 5000.0f;
                } else {
                    var_f31 -= 5000.0f;
                }
            }
            vrkumo_packet->mVrkumoEff[i].mPosition.x = var_f31;

            vrkumo_packet->mVrkumoEff[i].mPosition.y = 0.0f;

            var_f31 = sp34 * cM_scos(sp8);
            // @bug - parenthesis should not be on the condition
            if ((f32)fabs(var_f31 < 5000.0f)) {
                if (var_f31 > 0.0f) {
                    var_f31 += 5000.0f;
                } else {
                    var_f31 -= 5000.0f;
                }
            }
            vrkumo_packet->mVrkumoEff[i].mPosition.z = var_f31;

            vrkumo_packet->mVrkumoEff[i].mHeight = 0.3f * cM_rndFX(0.3f);
            vrkumo_packet->mVrkumoEff[i].mAlpha = 0.0f;
            vrkumo_packet->mVrkumoEff[i].mSpeed = 0.4f + cM_rndF(2.0f);

            vrkumo_packet->mVrkumoEff[i].mStatus++;
            break;
        case 1:
            sp80 = vrkumo_packet->mVrkumoEff[i].mPosition;
            sp80.y = 0.0f;

            if (sp80.abs() > 15000.0f) {
                if (sp80.abs() > 15100.0f) {
                    sp8 = cM_rndF(65535.0f);
                    sp34 = cM_rndF(18000.0f);
                    if (sp34 > 15000.0f) {
                        sp34 = 14000.0 + cM_rndF(1000.0f);
                    }
                    var_f31 = sp34 * cM_ssin(sp8);

                    // @bug - parenthesis should not be on the condition
                    if ((f32)fabs(var_f31 < 5000.0f)) {
                        if (var_f31 > 0.0f) {
                            var_f31 += 5000.0f;
                        } else {
                            var_f31 -= 5000.0f;
                        }
                    }
                    vrkumo_packet->mVrkumoEff[i].mPosition.x = var_f31;

                    vrkumo_packet->mVrkumoEff[i].mPosition.y = 0.0f;

                    var_f31 = sp34 * cM_scos(sp8);

                    // @bug - parenthesis should not be on the condition
                    if ((f32)fabs(var_f31 < 5000.0f)) {
                        if (var_f31 > 0.0f) {
                            var_f31 += 5000.0f;
                        } else {
                            var_f31 -= 5000.0f;
                        }
                    }
                    vrkumo_packet->mVrkumoEff[i].mPosition.z = var_f31;
                } else {
                    vrkumo_packet->mVrkumoEff[i].mPosition.x = -vrkumo_packet->mVrkumoEff[i].mPosition.x;
                    vrkumo_packet->mVrkumoEff[i].mPosition.z = -vrkumo_packet->mVrkumoEff[i].mPosition.z;
                }
                vrkumo_packet->mVrkumoEff[i].mAlpha = 0.0f;
            }

            if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0) {
                wind_vecpow.x = -0.7f;
                wind_vecpow.z = 0.0f;
            } else if (strcmp(dComIfGp_getStartStageName(), "R_SP30") == 0) {
                wind_vecpow.x = (0.3f * wind_vec->x) + (0.5f * wind_vecpow.x);
                wind_vecpow.z = (0.3f * wind_vec->z) + (0.5f * wind_vecpow.z);
            }

            if (vrkumo_packet->mVrkumoEff[i].mAlpha <= 0.0f) {
                vrkumo_packet->mVrkumoEff[i].mPosition.x += wind_vecpow.x * (sp70 + ((i / 100.0f) * sp70));
                vrkumo_packet->mVrkumoEff[i].mPosition.z += wind_vecpow.z * (sp70 + ((i / 100.0f) * sp70));
            } else {
                vrkumo_packet->mVrkumoEff[i].mPosition.x += wind_vecpow.x * (sp70 * vrkumo_packet->mVrkumoEff[i].mSpeed) * vrkumo_packet->mVrkumoEff[i].mDistFalloff;
                vrkumo_packet->mVrkumoEff[i].mPosition.z += wind_vecpow.z * (sp70 * vrkumo_packet->mVrkumoEff[i].mSpeed) * vrkumo_packet->mVrkumoEff[i].mDistFalloff;
            }
            break;
        case 3:
            break;
        }

        cXyz sp74;
        sp74 = vrkumo_packet->mVrkumoEff[i].mPosition;
        sp74.y = 0.0f;

        f32 sp24 = sp74.abs();
        f32 sp20 = 1.0f - (sp24 / 15000.0f);
        if (sp20 < 0.0f) {
            sp20 = 0.0f;
        }

        f32 spC = 1.0f;

        f32 sp1C = i / 100.0f;
        f32 sp44 = 1.0f - sp20;
        sp44 = 1.0f - (sp44 * sp44 * sp44);
        vrkumo_packet->mVrkumoEff[i].mPosition.y = (500.0f * sp1C) + (sp6C + (sp68 * sp44));

        sp20 = sp24 / 15000.0f;
        if (sp20 > 1.0f) {
            sp20 = 1.0f;
        }
        sp20 = 1.0f - (0.6f * (sp20 * sp20 * sp20 * sp20));
        vrkumo_packet->mVrkumoEff[i].mDistFalloff = sp20;

        f32 max_alpha;
        f32 alpha_step = 0.025f;
        f32 sp10 = 1.0f;
        if (i >= g_env_light.mVrkumoCount) {
            alpha_step = 0.025f;
            max_alpha = 0.0f;
        } else if (vrkumo_packet->mVrkumoEff[i].mDistFalloff < 0.5f) {
            alpha_step = 0.025f;
            max_alpha = 0.0f;
        } else {
            max_alpha = sp10;
        }

        max_alpha *= spC;
        cLib_addCalc(&vrkumo_packet->mVrkumoEff[i].mAlpha, max_alpha, 0.5f, alpha_step, 0.001f);
    }
}

static void dKr_cullVtx_Set() {
    GXSetCullMode(GX_CULL_NONE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

static void dKyr_draw_rev_moon(Mtx drawMtx, u8** tex) {
    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;
    dKankyo_sunlenz_Packet* lenz_packet = g_env_light.mpSunLenzPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    cXyz pos[4];

    u16 date = dComIfGs_getDate();
    cXyz sp60 = camera->lookat.eye + g_env_light.moon_pos;
    sp60.y = camera->lookat.eye.y - g_env_light.moon_pos.y;

    cXyz moon_pos;
    moon_pos.x = sp60.x - camera->lookat.eye.x;
    moon_pos.y = sp60.y - camera->lookat.eye.y;
    moon_pos.z = sp60.z - camera->lookat.eye.z;

    Vec vp, lp;

    int weekday = date % 8;
    if (g_env_light.getDaytime() < 180.0f) {
        if (weekday != 0) {
            weekday--;
        } else {
            weekday = 7;
        }
    }

    if (weekday != 4) {
        int texidx;
        f32 flipX;
        switch (weekday) {
        case 0:
            texidx = 0;
            flipX = 1.0f;
            break;
        case 1:
            texidx = 1;
            flipX = 1.0f;
            break;
        case 2:
            texidx = 2;
            flipX = 1.0f;
            break;
        case 3:
            texidx = 3;
            flipX = 1.0f;
            break;
        case 4:
            texidx = 0;
            flipX = 1.0f;
            break;
        case 5:
            texidx = 3;
            flipX = -1.0f;
            break;
        case 6:
            texidx = 2;
            flipX = -1.0f;
            break;
        case 7:
        default:
            texidx = 1;
            flipX = -1.0f;
            break;
        }

        GXColor color_reg0;
        color_reg0.r = g_env_light.fog_col.r;
        color_reg0.g = g_env_light.fog_col.g;
        color_reg0.b = g_env_light.fog_col.b;
        color_reg0.a = 0xFF;

        GXColor color_reg1;
        color_reg1.r = 0;
        color_reg1.g = 0;
        color_reg1.b = 0;
        color_reg1.a = 0xFF;

        Mtx camMtx;
        if (dComIfGd_getView() != NULL) {
            MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
        } else {
            if (sun_packet->field_0x28 < 5) {
                sun_packet->field_0x28 += 2;
            }
            sun_packet->field_0x29 = 1;
            return;
        }

        GXTexObj texobj;
        dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[0], GX_TEXMAP0);
        dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[1], GX_TEXMAP1);
        dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[texidx + 2], GX_TEXMAP2);

        GXSetNumChans(0);
        GXSetTevColor(GX_TEVREG0, color_reg0);
        GXSetTevColor(GX_TEVREG1, color_reg1);
        GXSetNumTexGens(3);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
        GXSetNumIndStages(0);
        GXSetCullMode(GX_CULL_NONE);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_CLR_RGBA, GX_RGBA4, 8);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);

        cXyz camfwd;
        f32 dayscale[8] = {
            1.0f, 0.83f, 0.6f, 0.78f,
            1.0f, 0.78f, 0.6f, 0.83f,
        };
        
        dKyr_get_vectle_calc(&camera->lookat.eye, &camera->lookat.center, &camfwd);

        f32 cam_distXZ = JMAFastSqrt((camfwd.x * camfwd.x) + (camfwd.z * camfwd.z));
        f32 cam_theta = atan2f(camfwd.x, camfwd.z);
        f32 cam_phi = atan2f(camfwd.y, cam_distXZ);

        f32 moon_distXZ = JMAFastSqrt((moon_pos.x * moon_pos.x) + (moon_pos.z * moon_pos.z));
        f32 moon_theta = atan2f(moon_pos.x, moon_pos.z);
        f32 moon_phi = atan2f(moon_pos.y, moon_distXZ);

        f32 sp3C = (moon_theta - cam_theta) / -(M_PI * 2);
        f32 rot = -(45.0f + (360.0f * sp3C));

        Mtx rotMtx;
        MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
        MTXConcat(camMtx, rotMtx, camMtx);
        GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);

        color_reg0.r = 0;
        color_reg0.g = 0;
        color_reg0.b = 0;

        f32 size = 8000.0f;
        if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
            size = 11000.0f;
        }

        color_reg0.a = 255.0f * sun_packet->mMoonAlpha;
        GXSetTevColor(GX_TEVREG0, color_reg0);

        for (int i = 0; i < 2; i++) {
            if (i == 1) {
                dKyr_set_btitex(&texobj, (ResTIMG*)lenz_packet->mpResBall);
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);

                size *= 2.3f;
                color_reg0.a = 40.0f * sun_packet->mMoonAlpha;

                color_reg0.r = 0xFF;
                color_reg0.g = 0xFF;
                color_reg0.b = 0xCF;

                color_reg1.r = 0xC5;
                color_reg1.g = 0x69;
                color_reg1.b = 0x23;

                rot = (50.0f * flipX);
                MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
                MTXConcat(camMtx, rotMtx, camMtx);
                GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
            }

            GXSetTevColor(GX_TEVREG0, color_reg0);
            GXSetTevColor(GX_TEVREG1, color_reg1);

            vp.x = -size * flipX;
            vp.y = size;
            vp.z = 0.0f;
            cMtx_multVec(camMtx, &vp, &lp);
            pos[0].x = sp60.x + lp.x;
            pos[0].y = sp60.y + lp.y;
            pos[0].z = sp60.z + lp.z;

            vp.x = size * flipX;
            vp.y = size;
            vp.z = 0.0f;
            cMtx_multVec(camMtx, &vp, &lp);
            pos[1].x = sp60.x + lp.x;
            pos[1].y = sp60.y + lp.y;
            pos[1].z = sp60.z + lp.z;

            if (i == 0) {
                vp.x = size * flipX;
                vp.y = -size;
            } else {
                vp.x = (size * flipX) * dayscale[weekday];
                vp.y = -size * dayscale[weekday];
            }

            vp.z = 0.0f;
            cMtx_multVec(camMtx, &vp, &lp);
            pos[2].x = sp60.x + lp.x;
            pos[2].y = sp60.y + lp.y;
            pos[2].z = sp60.z + lp.z;

            vp.x = -size * flipX;
            vp.y = -size;
            vp.z = 0.0f;
            cMtx_multVec(camMtx, &vp, &lp);
            pos[3].x = sp60.x + lp.x;
            pos[3].y = sp60.y + lp.y;
            pos[3].z = sp60.z + lp.z;

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
            GXTexCoord2s16(0, 0);
            if (i == 0) {
                GXTexCoord2s16(0, 0);
                GXTexCoord2s16(0, 0);
            }

            GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
            GXTexCoord2s16(0xFF, 0);
            if (i == 0) {
                GXTexCoord2s16(0xFF, 0);
                GXTexCoord2s16(0xFF, 0);
            }

            GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
            GXTexCoord2s16(0xFF, 0xFF);
            if (i == 0) {
                GXTexCoord2s16(0xFF, 0xFF);
                GXTexCoord2s16(0xFF, 0xFF);
            }

            GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
            GXTexCoord2s16(0, 0xFF);
            if (i == 0) {
                GXTexCoord2s16(0, 0xFF);
                GXTexCoord2s16(0, 0xFF);
            }

            GXEnd();
        }

        J3DShape::resetVcdVatCache();
    }
}

void dKyr_drawSun(Mtx drawMtx, cXyz* ppos, GXColor& unused, u8** tex) {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    cXyz pos[4];
    cXyz sunpos;
    cXyz spB4;
    cXyz moon_pos;
    cXyz sp9C;

    Vec sp90, sp84;

    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;
    dKankyo_sunlenz_Packet* lenz_packet = g_env_light.mpSunLenzPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    f32 rot = 0.0f;

    u8 draw_moon = false;
    u8 draw_sun = false;
    u16 date = dComIfGs_getDate();

#if VERSION == VERSION_GCN_JPN
    if (g_env_light.hide_vrbox) {
        return;
    }
#endif

    if (strcmp(dComIfGp_getStartStageName(), "F_SP200") == 0) {
        sun_packet->mMoonAlpha = 1.0f;
        sun_packet->mSunAlpha = 0.0f;
    }

    if (sun_packet->mSunAlpha > 0.0f) {
        draw_sun = true;
    }

    if (sun_packet->mMoonAlpha > 0.0f) {
        draw_moon = true;
    }

    if ((draw_sun | draw_moon) != 0) {
        sunpos.x = ppos->x;
        sunpos.y = ppos->y;
        sunpos.z = ppos->z;

        u32 stage_type = dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo());
        if (g_env_light.base_light.mColor.r == 0 && stage_type != ST_ROOM) {
            if (g_env_light.daytime > 285.0f || g_env_light.daytime < 105.0f) {
                draw_moon = false;
            }

            spB4.x = ppos->x;
            spB4.y = ppos->y;
            spB4.z = ppos->z;
        } else {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP200") == 0 && dComIfG_play_c::getLayerNo(0) == 0) {
                spB4 = envlight->moon_pos;
                moon_pos = spB4;
            } else {
                spB4 = camera->lookat.eye + envlight->moon_pos;
                moon_pos.x = spB4.x - camera->lookat.eye.x;
                moon_pos.y = spB4.y - camera->lookat.eye.y;
                moon_pos.z = spB4.z - camera->lookat.eye.z;
            }
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0 && dKy_daynight_check()) {
            spB4.x = 3900.0f + camera->lookat.eye.x;
            spB4.y = 8052.0f + camera->lookat.eye.y;
            spB4.z = -9072.0f + camera->lookat.eye.z;

            moon_pos.x = spB4.x - camera->lookat.eye.x;
            moon_pos.y = spB4.y - camera->lookat.eye.y;
            moon_pos.z = spB4.z - camera->lookat.eye.z;
        }

        int weekday = date % 8;
        if (g_env_light.getDaytime() < 180.0f) {
            if (weekday != 0) {
                weekday--;
            } else {
                weekday = 7;
            }
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP200") == 0) {
            if (g_env_light.getDaytime() < 180.0f) {
                weekday++;
                if (weekday >= 8) {
                    weekday = 0;
                }
            }

            if (weekday == 4) {
                weekday = 3;
            }
        }

        if (weekday != 4) {
            int texidx;
            f32 flipX;
            switch (weekday) {
            case 0:
                texidx = 0;
                flipX = 1.0f;
                break;
            case 1:
                texidx = 1;
                flipX = 1.0f;
                break;
            case 2:
                texidx = 2;
                flipX = 1.0f;
                break;
            case 3:
                texidx = 3;
                flipX = 1.0f;
                break;
            case 4:
                texidx = 0;
                flipX = 1.0f;
                break;
            case 5:
                texidx = 3;
                flipX = -1.0f;
                break;
            case 6:
                texidx = 2;
                flipX = -1.0f;
                break;
            case 7:
            default:
                texidx = 1;
                flipX = -1.0f;
                break;
            }

            GXColor color_reg0;
            color_reg0.r = g_env_light.fog_col.r;
            color_reg0.g = g_env_light.fog_col.g;
            color_reg0.b = g_env_light.fog_col.b;
            color_reg0.a = 0xFF;

            GXColor color_reg1;
            color_reg1.r = 0;
            color_reg1.g = 0;
            color_reg1.b = 0;
            color_reg1.a = 0xFF;

            Mtx camMtx;
            if (dComIfGd_getView() != NULL) {
                MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
            } else {
                if (sun_packet->field_0x28 < 5) {
                    sun_packet->field_0x28 += 2;
                }
                sun_packet->field_0x29 = 1;
                return;
            }

            GXTexObj texobj;
            dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[0], GX_TEXMAP0);
            dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[1], GX_TEXMAP1);
            dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[texidx + 2], GX_TEXMAP2);

            GXSetNumChans(0);
            GXSetTevColor(GX_TEVREG0, color_reg0);
            GXSetTevColor(GX_TEVREG1, color_reg1);
            GXSetNumTexGens(3);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
            GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
            GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
            GXSetNumTevStages(3);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
            GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
            GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
            GXSetNumIndStages(0);
            GXSetCullMode(GX_CULL_NONE);

            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_CLR_RGBA, GX_RGBA4, 8);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);

            if (draw_moon == true) {
                cXyz camfwd;
                f32 dayscale[8] = {
                    1.0f, 0.83f, 0.6f, 0.78f,
                    1.0f, 0.78f, 0.6f, 0.83f,
                };

                if (strcmp(dComIfGp_getStartStageName(), "F_SP200") != 0) {
                    dKyr_get_vectle_calc(&camera->lookat.eye, &camera->lookat.center, &camfwd);
                    f32 cam_distXZ = JMAFastSqrt((camfwd.x * camfwd.x) + (camfwd.z * camfwd.z));
                    f32 cam_theta = atan2f(camfwd.x, camfwd.z);
                    f32 cam_phi = atan2f(camfwd.y, cam_distXZ);

                    f32 moon_distXZ = JMAFastSqrt((moon_pos.x * moon_pos.x) + (moon_pos.z * moon_pos.z));
                    f32 moon_theta = atan2f(moon_pos.x, moon_pos.z);
                    f32 moon_phi = atan2f(moon_pos.y, moon_distXZ);

                    f32 sp3C = (moon_theta - cam_theta) / -(M_PI * 2);
                    rot = 45.0f + (360.0f * sp3C);
                }

                Mtx rotMtx;
                MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
                MTXConcat(camMtx, rotMtx, camMtx);
                GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);

                color_reg0.r = 0;
                color_reg0.g = 0;
                color_reg0.b = 0;

                f32 size = 8000.0f;
                if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == NULL) {
                    size = 11000.0f;
                } else if (strcmp(dComIfGp_getStartStageName(), "F_SP200") == NULL) {
                    size = 10000.0f;
                }

                if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0 && dKy_daynight_check()) {
                    size = 1200.0f;
                    dEvent_manager_c* eventMng = &dComIfGp_getEventManager();
                    if (eventMng != NULL) {
                        if (strcmp(eventMng->getRunEventName(), "APPEAR_HAWKER") == 0) {
                            size = 2400.0f;
                        }
                    }
                }

                color_reg0.a = 255.0f * sun_packet->mMoonAlpha;
                GXSetTevColor(GX_TEVREG0, color_reg0);

                for (int i = 0; i < 2; i++) {
                    if (i == 1) {
                        dKyr_set_btitex(&texobj, (ResTIMG*)lenz_packet->mpResBall);
                        GXClearVtxDesc();
                        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
                        GXSetNumTevStages(1);
                        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
                        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);

                        size *= 2.3f;
                        color_reg0.a = 40.0f * sun_packet->mMoonAlpha;

                        color_reg0.r = 0xFF;
                        color_reg0.g = 0xFF;
                        color_reg0.b = 0xCF;

                        color_reg1.r = 0xC5;
                        color_reg1.g = 0x69;
                        color_reg1.b = 0x23;

                        rot = (50.0f * flipX);
                        MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
                        MTXConcat(camMtx, rotMtx, camMtx);
                        GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
                        GXSetCurrentMtx(GX_PNMTX0);
                    }

                    GXSetTevColor(GX_TEVREG0, color_reg0);
                    GXSetTevColor(GX_TEVREG1, color_reg1);

                    sp90.x = -size * flipX;
                    sp90.y = size;
                    sp90.z = 0.0f;
                    cMtx_multVec(camMtx, &sp90, &sp84);
                    pos[0].x = spB4.x + sp84.x;
                    pos[0].y = spB4.y + sp84.y;
                    pos[0].z = spB4.z + sp84.z;

                    sp90.x = size * flipX;
                    sp90.y = size;
                    sp90.z = 0.0f;
                    cMtx_multVec(camMtx, &sp90, &sp84);
                    pos[1].x = spB4.x + sp84.x;
                    pos[1].y = spB4.y + sp84.y;
                    pos[1].z = spB4.z + sp84.z;

                    if (i == 0) {
                        sp90.x = size * flipX;
                        sp90.y = -size;
                    } else {
                        sp90.x = (size * flipX) * dayscale[weekday];
                        sp90.y = -size * dayscale[weekday];
                    }
                    sp90.z = 0.0f;
                    cMtx_multVec(camMtx, &sp90, &sp84);
                    pos[2].x = spB4.x + sp84.x;
                    pos[2].y = spB4.y + sp84.y;
                    pos[2].z = spB4.z + sp84.z;

                    sp90.x = -size * flipX;
                    sp90.y = -size;
                    sp90.z = 0.0f;
                    cMtx_multVec(camMtx, &sp90, &sp84);
                    pos[3].x = spB4.x + sp84.x;
                    pos[3].y = spB4.y + sp84.y;
                    pos[3].z = spB4.z + sp84.z;

                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                    GXTexCoord2s16(0, 0);
                    if (i == 0) {
                        GXTexCoord2s16(0, 0);
                        GXTexCoord2s16(0, 0);
                    }

                    GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                    GXTexCoord2s16(0xFF, 0);
                    if (i == 0) {
                        GXTexCoord2s16(0xFF, 0);
                        GXTexCoord2s16(0xFF, 0);
                    }

                    GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                    GXTexCoord2s16(0xFF, 0xFF);
                    if (i == 0) {
                        GXTexCoord2s16(0xFF, 0xFF);
                        GXTexCoord2s16(0xFF, 0xFF);
                    }

                    GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                    GXTexCoord2s16(0, 0xFF);
                    if (i == 0) {
                        GXTexCoord2s16(0, 0xFF);
                        GXTexCoord2s16(0, 0xFF);
                    }

                    GXEnd();

                    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
                        dKyr_draw_rev_moon(drawMtx, tex);
                    }
                }
            }

            J3DShape::resetVcdVatCache();
        }
    }
}

void dKyr_drawLenzflare(Mtx drawMtx, cXyz* ppos, GXColor& param_2, u8** tex) {
    dKankyo_sunlenz_Packet* lenz_packet = g_env_light.mpSunLenzPacket;
    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    static s16 S_rot_work1 = 0;
    static s16 S_rot_work2 = 0;

#if VERSION == VERSION_GCN_JPN
    if (g_env_light.hide_vrbox) {
        return;
    }
#endif

    Mtx camMtx;
    Mtx rotMtx;

    cXyz pos[4];
    cXyz spFC;
    cXyz spF0;

    Vec spE4, spD8;

    s16 spC = 0;
    s16 spA = 0;

    f32 sun_visibility = sun_packet->mVisibility;
    f32 spAC = 1.0f - lenz_packet->mDistFalloff;
    f32 spA8 = sun_packet->mVisibility * sun_packet->mVisibility;

    if (!(sun_visibility < 0.1f)) {
        dKy_set_eyevect_calc2(camera, &spFC, 8000.0f, 8000.0f);

        GXColor color_reg0;
        color_reg0.r = sun_packet->mColor.r;
        color_reg0.g = sun_packet->mColor.g;
        color_reg0.b = sun_packet->mColor.b;

        GXColor color_reg1;
        color_reg1.r = sun_packet->field_0x74.r;
        color_reg1.g = sun_packet->field_0x74.g;
        color_reg1.b = sun_packet->field_0x74.b;

        if (dComIfGd_getView() != NULL) {
            MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
        } else {
            return;
        }

        j3dSys.reinitGX();

        GXTexObj texobj;
        dKyr_set_btitex(&texobj, (ResTIMG*)tex[0]);
        GXSetNumChans(0);
        GXSetTevColor(GX_TEVREG0, color_reg0);
        GXSetTevColor(GX_TEVREG1, color_reg1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1.0f, color_reg0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZCompLoc(GX_TRUE);
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
        GXSetNumIndStages(0);
        dKr_cullVtx_Set();

        MTXRotRad(rotMtx, 'Z', 0.0f);
        MTXConcat(camMtx, rotMtx, camMtx);
        GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);

        if (sun_packet->field_0x6c > 0.0f) {
            spC = S_rot_work1 - 0x7F6;
            spA = S_rot_work2 + 0x416B;
            S_rot_work1 += 8;
            S_rot_work2 -= 14;

            if (dComIfGd_getView() != NULL) {
                MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
            }

            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);

            color_reg0.a = sun_packet->field_0x6c * (15.0f * (spA8 * spA8 * spA8));
            GXSetTevColor(GX_TEVREG0, color_reg0);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetClipMode(GX_CLIP_ENABLE);
            GXSetNumIndStages(0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);

            for (int i = 0; i < 16; i++) {
                f32 spA0;
                f32 sp9C;
                f32 sp98;
                f32 sp94;
                f32 sp90;
                f32 sp8C;
                f32 sp88;
                f32 sp84;
                f32 sp80;
                f32 sp7C;
                f32 sp78;
                f32 sp74;
                f32 sp70;
                f32 sp6C;
                f32 sp68;
                f32 sp64;

                if (i & 1) {
                    sp78 = spC;
                } else {
                    sp78 = spA;
                }

                sp6C = 0.4f + (0.6f * (sun_visibility * (spAC * spAC)));
                sp64 = 300.0f;
                sp68 = cM_ssin(34.0f * sp78);
                if (sp68 < 0.0f) {
                    sp68 = -sp68;
                }

                sp74 = 1000.0f * (0.5f + sp68);

                spA0 = sp64 * sp6C * cM_ssin(sp78 + sp74);
                sp9C = sp64 * sp6C * cM_scos(sp78 + sp74);
                sp90 = sp64 * sp6C * cM_ssin(sp78 - sp74);
                sp8C = sp64 * sp6C * cM_scos(sp78 - sp74);

                sp70 = (0.6f + (0.4f * sp68)) * (160.0f * sp6C * (2.9f + spAC));
                sp70 *= 1.5f * sun_visibility;

                if (i & 3) {
                    sp70 *= 0.86f;
                    if (i & 2) {
                        sp70 *= 2.46f;
                    }
                }

                sp98 = sp70 * cM_ssin(sp78);
                sp94 = sp70 * cM_scos(sp78);
                spC += 0x1000;
                spA += 0x1C71;

                spE4.x = sp98;
                spE4.y = sp94;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                spF0.x = ppos->x + spD8.x;
                spF0.y = ppos->y + spD8.y;
                spF0.z = ppos->z + spD8.z;

                sp7C = spFC.abs(spF0);
                if (sp7C < lenz_packet->field_0x8c) {
                    lenz_packet->field_0x8c = sp7C;
                } else if (sp7C > lenz_packet->field_0x90) {
                    lenz_packet->field_0x90 = sp7C;
                }

                sp88 = lenz_packet->field_0x88 - lenz_packet->field_0x84;
                if (sp88 > 0.0f) {
                    sp84 = 1.0f - ((sp7C - lenz_packet->field_0x84) / sp88);
                } else {
                    sp84 = 1.0f;
                }

                switch (i & 3) {
                case 0:
                    sp80 = 0.1f;
                    break;
                case 1:
                    sp80 = 1.1f;
                    break;
                case 2:
                    sp80 = 0.2f;
                    break;
                case 3:
                    sp80 = 0.4f;
                    break;
                }

                f32 spC4 = spA8;
                sp98 *= sun_visibility * (spC4 + sp80);
                sp94 *= sun_visibility * (spC4 + sp80);

                spE4.x = spA0;
                spE4.y = sp9C;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                pos[0].x = sun_packet->mPos[0].x + spD8.x;
                pos[0].y = sun_packet->mPos[0].y + spD8.y;
                pos[0].z = sun_packet->mPos[0].z + spD8.z;

                spE4.x = sp98;
                spE4.y = sp94;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                pos[1].x = sun_packet->mPos[0].x + spD8.x;
                pos[1].y = sun_packet->mPos[0].y + spD8.y;
                pos[1].z = sun_packet->mPos[0].z + spD8.z;

                spE4.x = sp90;
                spE4.y = sp8C;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                pos[2].x = sun_packet->mPos[0].x + spD8.x;
                pos[2].y = sun_packet->mPos[0].y + spD8.y;
                pos[2].z = sun_packet->mPos[0].z + spD8.z;

                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
                GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                GXEnd();
            }
        }

        GXSetNumChans(0);
        GXSetTevColor(GX_TEVREG0, color_reg0);
        GXSetTevColor(GX_TEVREG1, color_reg1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1.0f, color_reg0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZCompLoc(GX_TRUE);
        GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
        GXSetNumIndStages(0);
        dKr_cullVtx_Set();

        static const f32 scale_dat[] = {
            8000.0f, 10500.0f, 3.2f, 1.8f, 1.8f, 6.2f, 1.2f, 4.0f
        };

        static const GXColor col_dat[] = {
            {0xB4, 0xC8, 0xFF, 0xFF},
            {0xC8, 0xC8, 0xFF, 0x50},
            {0xFF, 0x8C, 0x78, 0x5A},
            {0xFF, 0xB4, 0x78, 0x64},
            {0xB4, 0xB4, 0x78, 0x55},
            {0xB4, 0xC8, 0x64, 0x5A},
            {0xDC, 0xFF, 0xB4, 0x6E},
            {0xC8, 0xDC, 0xFF, 0x5A},
        };

        for (int i = 0; i < 8; i++) {
            if (!g_env_light.mpSunLenzPacket->mDrawLenzInSky || i == 0) {
                f32 spC4 = spA8;
                if (i < 2) {
                    if (i == 0) {
                        color_reg0.a = sun_packet->field_0x6c * (0.8f * (spC4 * col_dat[i].a));
                    } else {
                        color_reg0.a = sun_packet->field_0x6c * (0.2f * (spC4 * col_dat[i].a));
                    }
                } else if (i == 2) {
                    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
                    GXColorS10 spD0;
                    spD0.r = 0.5f * sun_packet->mColor.r;
                    spD0.g = 0.5f * sun_packet->mColor.g;
                    spD0.b = 0.5f * sun_packet->mColor.b;

                    if (spD0.r > 0xFF) {
                        spD0.r = 0xFF;
                    }
                    if (spD0.g > 0xFF) {
                        spD0.g = 0xFF;
                    }
                    if (spD0.b > 0xFF) {
                        spD0.b = 0xFF;
                    }

                    GXColor sp60;
                    sp60.r = 0xFF;
                    sp60.g = 0xFF;
                    sp60.b = 0xFF;

                    f32 sp5C = 0.12f;
                    color_reg0.r = ((f32)spD0.r * (1.0f - sp5C)) + ((f32)sp60.r * sp5C);
                    color_reg0.g = ((f32)spD0.g * (1.0f - sp5C)) + ((f32)sp60.g * sp5C);
                    color_reg0.b = ((f32)spD0.b * (1.0f - sp5C)) + ((f32)sp60.b * sp5C);

                    spD0.r = 0.5f * sun_packet->field_0x74.r;
                    spD0.g = 0.5f * sun_packet->field_0x74.g;
                    spD0.b = 0.5f * sun_packet->field_0x74.b;

                    if (spD0.r > 0xFF) {
                        spD0.r = 0xFF;
                    }
                    if (spD0.g > 0xFF) {
                        spD0.g = 0xFF;
                    }
                    if (spD0.b > 0xFF) {
                        spD0.b = 0xFF;
                    }

                    color_reg1.r = spD0.r;
                    color_reg1.g = spD0.g;
                    color_reg1.b = spD0.b;

                    f32 sp58 = 1.0f - (spC4 * spAC);
                    sp58 = 1.0f - (sp58 * sp58 * sp58);
                    color_reg0.a = 140.0f * sp58;
                    color_reg1.a = 140.0f * sp58;
                } else {
                    color_reg0.r = col_dat[i].r;
                    color_reg0.g = col_dat[i].g;
                    color_reg0.b = col_dat[i].b;

                    color_reg1.r = col_dat[i].r;
                    color_reg1.g = col_dat[i].g;
                    color_reg1.b = col_dat[i].b;

                    color_reg0.a = spAC * (0.5f * (250.0f * spC4) * lenz_packet->mDistFalloff);
                    color_reg1.a = spAC * (0.5f * (250.0f * spC4) * lenz_packet->mDistFalloff);
                }

                GXSetTevColor(GX_TEVREG0, color_reg0);
                GXSetTevColor(GX_TEVREG1, color_reg1);

                f32 var_f31;
                if (i < 2) {
                    var_f31 = (0.04f + (0.075f * sun_visibility)) * scale_dat[i];
                    if (i == 0) {
                        var_f31 += 0.2f * (sun_visibility * (scale_dat[i] - 5000.0f)) * (spAC * spAC);
                    } else {
                        var_f31 += 0.2f * (sun_visibility * scale_dat[i]) * (spAC * spAC);
                    }
                    var_f31 = 0.85f * var_f31;
                } else if (i == 2) {
                    var_f31 = 4000.0f;
                } else {
                    var_f31 = 60.0f * (sun_visibility * scale_dat[i]) * (1.0f - (lenz_packet->mDistFalloff * lenz_packet->mDistFalloff * lenz_packet->mDistFalloff));
                }

                if (i == 1) {
                    dKyr_set_btitex(&texobj, (ResTIMG*)tex[2]);
                } else if (i == 2) {
                    dKyr_set_btitex(&texobj, (ResTIMG*)tex[3]);
                } else {
                    dKyr_set_btitex(&texobj, (ResTIMG*)tex[0]);
                }

                spE4.x = -var_f31;
                spE4.y = var_f31;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                pos[0].x = spD8.x + ppos[i].x;
                pos[0].y = spD8.y + ppos[i].y;
                pos[0].z = spD8.z + ppos[i].z;

                spE4.x = var_f31;
                spE4.y = var_f31;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                pos[1].x = spD8.x + ppos[i].x;
                pos[1].y = spD8.y + ppos[i].y;
                pos[1].z = spD8.z + ppos[i].z;

                spE4.x = var_f31;
                spE4.y = -var_f31;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                pos[2].x = spD8.x + ppos[i].x;
                pos[2].y = spD8.y + ppos[i].y;
                pos[2].z = spD8.z + ppos[i].z;

                spE4.x = -var_f31;
                spE4.y = -var_f31;
                spE4.z = 0.0f;
                cMtx_multVec(camMtx, &spE4, &spD8);
                pos[3].x = spD8.x + ppos[i].x;
                pos[3].y = spD8.y + ppos[i].y;
                pos[3].z = spD8.z + ppos[i].z;

                s16 sp8;
                if (i == 1) {
                    sp8 = 0x1FF;
                } else {
                    sp8 = 0xFF;
                }

                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                GXTexCoord2s16(0, 0);
                GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                GXTexCoord2s16(sp8, 0);
                GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                GXTexCoord2s16(sp8, sp8);
                GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                GXTexCoord2s16(0, sp8);
                GXEnd();
            }
        }

        dKy_GxFog_set();
        J3DShape::resetVcdVatCache();
    }
}

void dKyr_drawRain(Mtx drawMtx, u8** tex) {
    dKankyo_rain_Packet* rain_packet = g_env_light.mpRainPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    cXyz wind_vecpow = dKyw_get_wind_vecpow();

    static u32 rot = 0;

    Mtx camMtx;
    cXyz pos[4];

    Vec sp54, sp48;
    cXyz sp3C;
    cXyz sp30;

    if (g_env_light.mSnowCount == 0 && !g_env_light.camera_water_in_status) {
        sp30.x = 0.0f;
        sp30.y = -2.0f;
        sp30.z = 0.0f;

        if (rain_packet->raincnt != 0) {
            GXColor color_reg0;
            color_reg0.r = 0xFF;
            color_reg0.g = 0xFF;
            color_reg0.b = 0xFF;

            if (dComIfGd_getView() != NULL) {
                MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
            } else {
                return;
            }

            GXTexObj texobj;
            dKyr_set_btitex(&texobj, (ResTIMG*)tex[0]);
            GXSetNumChans(0);
            GXSetTevColor(GX_TEVREG0, color_reg0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
            GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
            GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
            GXSetClipMode(GX_CLIP_DISABLE);
            GXSetNumIndStages(0);
            dKr_cullVtx_Set();

            Mtx rotMtx;
            MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
            MTXConcat(camMtx, rotMtx, camMtx);
            GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);

            for (int i = 0; i < rain_packet->raincnt; i++) {
                f32 temp_f30 = -1.0f;

                if (!(rain_packet->mRainEff[i].mAlpha <= 0.0f)) {
                    if (dKy_darkworld_check()) {
                        color_reg0.a = 34.0f * rain_packet->mRainEff[i].mAlpha;
                    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0) {
                        color_reg0.a = 9.0f * rain_packet->mRainEff[i].mAlpha;
                    } else {
                        color_reg0.a = 20.0f * rain_packet->mRainEff[i].mAlpha;
                    }

                    GXSetTevColor(GX_TEVREG0, color_reg0);
                    sp3C.x = rain_packet->mRainEff[i].mBasePos.x + rain_packet->mRainEff[i].mPosition.x;
                    sp3C.y = rain_packet->mRainEff[i].mBasePos.y + rain_packet->mRainEff[i].mPosition.y;
                    sp3C.z = rain_packet->mRainEff[i].mBasePos.z + rain_packet->mRainEff[i].mPosition.z;

                    f32 dist = 0.1f + (sp3C.abs(camera->lookat.eye) / 1500.0f);
                    if (dist > 1.0f) {
                        dist = 1.0f;
                    }

                    f32 temp_f31 = 1.5f;
                    cXyz tilt;
                    f32 speed = 5.0f + (70.0f * dist);

                    tilt.x = speed * (sp30.x + (0.08f * (i & 7)) + ((10.0f * (rain_packet->mCenterDelta.x * rain_packet->mCenterDeltaMul)) + (wind_vecpow.x * (1.0f + cM_rndF(0.25f)))));
                    tilt.y = speed * (sp30.y + ((rain_packet->mCenterDelta.y * rain_packet->mCenterDeltaMul) + (wind_vecpow.y * (1.0f + cM_rndF(0.25f)))));
                    tilt.z = speed * (sp30.z + (0.08f * (i & 3)) + ((10.0f * (rain_packet->mCenterDelta.z * rain_packet->mCenterDeltaMul)) + (wind_vecpow.z * (1.0f + cM_rndF(0.25f)))));

                    sp54.x = -temp_f31 * temp_f30;
                    sp54.y = 0.0f;
                    sp54.z = 0.0f;
                    cMtx_multVec(camMtx, &sp54, &sp48);
                    pos[0].x = (sp3C.x + sp48.x) - tilt.x;
                    pos[0].y = (sp3C.y + sp48.y) - tilt.y;
                    pos[0].z = (sp3C.z + sp48.z) - tilt.z;

                    sp54.x = temp_f31 * temp_f30;
                    sp54.y = 0.0f;
                    sp54.z = 0.0f;
                    cMtx_multVec(camMtx, &sp54, &sp48);
                    pos[1].x = (sp3C.x + sp48.x) - tilt.x;
                    pos[1].y = (sp3C.y + sp48.y) - tilt.y;
                    pos[1].z = (sp3C.z + sp48.z) - tilt.z;

                    sp54.x = temp_f31 * temp_f30;
                    sp54.y = 0.0f;
                    sp54.z = 0.0f;
                    cMtx_multVec(camMtx, &sp54, &sp48);
                    pos[2].x = sp3C.x + sp48.x;
                    pos[2].y = sp3C.y + sp48.y;
                    pos[2].z = sp3C.z + sp48.z;

                    sp54.x = -temp_f31 * temp_f30;
                    sp54.y = 0.0f;
                    sp54.z = 0.0f;
                    cMtx_multVec(camMtx, &sp54, &sp48);
                    pos[3].x = sp3C.x + sp48.x;
                    pos[3].y = sp3C.y + sp48.y;
                    pos[3].z = sp3C.z + sp48.z;

                    for (int j = 0; j < 4; j++) {
                        static cXyz add_table[] = {
                            cXyz(150.0f, 0.0f, 0.0f),
                            cXyz(0.0f, 150.0f, 150.0f),
                            cXyz(150.0f, 320.0f, 150.0f),
                            cXyz(45.0f, 480.0f, 45.0f),
                        };

                        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                        GXPosition3f32(pos[0].x + add_table[j].x, pos[0].y + add_table[j].y, pos[0].z + add_table[j].z);
                        GXTexCoord2s16(0, 0);
                        GXPosition3f32(pos[1].x + add_table[j].x, pos[1].y + add_table[j].y, pos[1].z + add_table[j].z);
                        GXTexCoord2s16(0xFF, 0);
                        GXPosition3f32(pos[2].x + add_table[j].x, pos[2].y + add_table[j].y, pos[2].z + add_table[j].z);
                        GXTexCoord2s16(0xFF, 0xFF);
                        GXPosition3f32(pos[3].x + add_table[j].x, pos[3].y + add_table[j].y, pos[3].z + add_table[j].z);
                        GXTexCoord2s16(0, 0xFF);
                        GXEnd();
                    }
                }
            }

            GXSetClipMode(GX_CLIP_ENABLE);
            J3DShape::resetVcdVatCache();
        }
    }
}

void dKyr_drawSibuki(Mtx drawMtx, u8** tex) {
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    dKankyo_rain_Packet* rain_packet = g_env_light.mpRainPacket;
    cXyz eyevect;
    cXyz sp38;    

    if (g_env_light.mSnowCount == 0 && g_env_light.camera_water_in_status == 0 && dComIfGd_getView() != NULL) {
        Mtx camMtx;
        MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);   
    } else {
        return;
    }

    if (strcmp(dComIfGp_getStartStageName(), "F_SP113") == 0 && dComIfGp_roomControl_getStayNo() == 1) {
        if ((camera->lookat.eye.z < 5100.0f || (camera->lookat.eye.x < -3250.0f && camera->lookat.eye.y < -50.0f)) ||
            (camera->lookat.eye.x < -2700.0f && camera->lookat.eye.z > 15750.0f))
        {
            return;
        }
    }

    f32 alpha = 255.0f;
    if (rain_packet->mStatus & 1) {
        alpha = 0.0f;
    } else if (rain_packet->mStatus & 2) {
        alpha = 200.0f;
    }

    cLib_addCalc(&rain_packet->mSibukiAlpha, alpha, 0.2f, 30.0f, 0.001f);
    dKy_set_eyevect_calc(camera, &eyevect, 7000.0f, 4000.0f);
    cXyz camdir;
    dKyr_get_vectle_calc(&camera->lookat.eye, &camera->lookat.center, &camdir);

    f32 alphaFade = 0.0f;
    if (camdir.y > alphaFade) {
        if (camdir.y < 0.5f) {
            alphaFade = 1.0f - (camdir.y / 0.5f);
        } else {
            alphaFade = 0.0f;
        }
    } else {
        alphaFade = 1.0f;
    }

    GXColor color;
    color.r = 0xB4;
    color.g = 0xC8;
    color.b = 0xC8;
    color.a = rain_packet->mSibukiAlpha * alphaFade;

    GXTexObj texobj;
    dKyr_set_btitex(&texobj, (ResTIMG*)tex[1]);
    GXSetNumChans(0);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColor(GX_TEVREG1, color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
    GXSetClipMode(GX_CLIP_DISABLE);
    GXSetNumIndStages(0);
    dKr_cullVtx_Set();
    GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    f32 scale;
    if (dComIfGd_getView() != NULL) {
        scale = dComIfGd_getView()->fovy / 45.0f;
        scale *= scale;
        if (scale >= 1.0f) {
            scale = 1.0f;
        }
    } else {
        scale = 0.2f;
    }

    for (int i = 0; i < g_env_light.raincnt >> 1; i++) {
        cXyz pos[4];
        f32 size = scale * (15.0f + cM_rndF(10.0f));
        cXyz sp20;

        f32 local_x = cM_rndFX(3600.0f);
        f32 local_y = cM_rndFX(1500.0f);
        f32 local_z = cM_rndFX(3600.0f);

        sp20.x = eyevect.x + local_x;
        sp20.y = eyevect.y + local_y;
        sp20.z = eyevect.z + local_z;

        pos[0].x = sp20.x - size;
        pos[0].y = sp20.y;
        pos[0].z = sp20.z - size;

        pos[1].x = sp20.x + size;
        pos[1].y = sp20.y;
        pos[1].z = sp20.z - size;

        pos[2].x = sp20.x + size;
        pos[2].y = sp20.y;
        pos[2].z = sp20.z + size;

        pos[3].x = sp20.x - size;
        pos[3].y = sp20.y;
        pos[3].z = sp20.z + size;

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
        GXTexCoord2s16(0, 0);

        GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
        GXTexCoord2s16(0x1FF, 0);

        GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
        GXTexCoord2s16(0x1FF, 0x1FF);

        GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
        GXTexCoord2s16(0, 0x1FF);
        GXEnd();
    }

    GXSetClipMode(GX_CLIP_ENABLE);
    J3DShape::resetVcdVatCache();
}

void dKyr_drawHousi(Mtx drawMtx, u8** tex) {
    dKankyo_housi_Packet* housi_packet = g_env_light.mpHousiPacket;
    static f32 rot = 0.0f;

    Mtx camMtx;
    Mtx rotMtx;
    cXyz pos[4];
    GXTexObj spDC;
    cXyz spD0;
    Vec spC4;
    Vec spB8;

    bool var_r28 = 0;
    if (housi_packet->field_0x5dec != 0) {
        if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0) {
            var_r28 = 1;
        }

        if (strcmp(dComIfGp_getStartStageName(), "D_MN08") != 0 ||
            dComIfGp_roomControl_getStayNo() == 0 || dComIfGp_roomControl_getStayNo() == 11)
        {
            j3dSys.reinitGX();
            f32 var_f25 = 120.0f;

            if (g_env_light.field_0xea9 == 1) {
                var_f25 = 140.0f;
            } else if (g_env_light.camera_water_in_status != 0) {
                return;
            }

            GXColor color_reg0;
            color_reg0.r = 0xE5;
            color_reg0.g = 0xFF;
            color_reg0.b = 0xC8;
            color_reg0.a = var_f25;

            GXColor color_reg1;
            color_reg1.r = 0x43;
            color_reg1.g = 0xD2;
            color_reg1.b = 0xCA;
            color_reg1.a = 0xFF;

            if (dKy_darkworld_check() == 1 || var_r28 == 1) {
                color_reg0.r = 0;
                color_reg0.g = 0;
                color_reg0.b = 0;
                color_reg0.a = var_f25;

                color_reg1.r = 0;
                color_reg1.g = 0;
                color_reg1.b = 0;
                color_reg1.a = 0xFF;

                var_f25 = 255.0f;
            } else if (g_env_light.field_0xea9 == 1) {
                color_reg0.r = 0xFF;
                color_reg0.g = 0xFF;
                color_reg0.b = 0xFF;

                color_reg1.r = 0;
                color_reg1.g = 0x50;
                color_reg1.b = 0x50;
            } else if (g_env_light.field_0xea9 == 2 &&
                       (g_env_light.fishing_hole_season == 1 || g_env_light.fishing_hole_season == 3))
            {
                GXColor sp1C = {0x32, 0x32, 0x32, 0xFF};
                GXColor sp18 = {0xFF, 0xD7, 0xF0, 0xFF};

                camera_class* cam_p = dComIfGp_getCamera(0);
                if (g_env_light.fishing_hole_season == 3) {
                    sp1C.r = 0x78;
                    sp1C.g = 0x0A;
                    sp1C.b = 0x14;

                    sp18.r = 0x14;
                    sp18.g = 0x3C;
                    sp18.b = 0x00;
                }

                dKy_ParticleColor_get_bg(&cam_p->lookat.eye, NULL, &color_reg1, &color_reg0, &sp1C, &sp18,
                                         0.0f);
                var_f25 = 255.0f;
            }

            if (dComIfGd_getView() != NULL) {
                MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
            } else {
                return;
            }

            f32 temp_f26 = 1.2f;
            f32 temp_f24 = 6.5f;

            for (int i = 0; i < 1; i++) {
                dKyr_set_btitex(&spDC, (ResTIMG*)*tex);
                GXSetNumChans(0);
                GXSetTevColor(GX_TEVREG0, color_reg0);
                GXSetTevColor(GX_TEVREG1, color_reg1);
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                                GX_TEVPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                                GX_TEVPREV);

                if (strcmp(dComIfGp_getStartStageName(), "F_NW01") == 0 ||
                    g_env_light.field_0xea9 == 1)
                {
                    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
                } else {
                    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
                                    GX_LO_SET);
                }

                GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

                if (i == 1) {
                    GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
                } else {
                    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
                }

                GXSetClipMode(GX_CLIP_DISABLE);
                GXSetNumIndStages(0);
                dKr_cullVtx_Set();

                rot += 1.2f;
                MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
                MTXConcat(camMtx, rotMtx, camMtx);

                GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);

                for (int j = 0; j < housi_packet->field_0x5dec; j++) {
                    fopAc_ac_c* player = dComIfGp_getPlayer(0);

                    spD0.x =
                        housi_packet->mHousiEff[j].mBasePos.x + housi_packet->mHousiEff[j].mPosition.x;
                    spD0.y =
                        housi_packet->mHousiEff[j].mBasePos.y + housi_packet->mHousiEff[j].mPosition.y;
                    spD0.z =
                        housi_packet->mHousiEff[j].mBasePos.z + housi_packet->mHousiEff[j].mPosition.z;

                    if (i == 1 && j == 0) {
                        color_reg0.r = 0;
                        color_reg0.g = 0;
                        color_reg0.b = 0;

                        color_reg1.r = 0;
                        color_reg1.g = 0;
                        color_reg1.b = 0;

                        GXSetTevColor(GX_TEVREG1, color_reg1);
                    }

                    if (i == 1) {
                        f32 temp_f4 = 100.0f;
                        if (!(spD0.y > player->current.pos.y + temp_f4)) {
                            if (!(spD0.y < player->current.pos.y - 20.0f)) {
                                if (!(housi_packet->mHousiEff[j].mAlpha <= 0.0f)) {
                                    color_reg0.a =
                                        housi_packet->mHousiEff[j].mAlpha * 40.0f *
                                        (1.0f - ((spD0.y - player->current.pos.y) / 100.0f));
                                    spD0.y = player->current.pos.y - 20.0f;
                                    goto block_14;  // probably fake match
                                }
                            }
                        }
                    } else {
                        color_reg0.a = housi_packet->mHousiEff[j].mAlpha * var_f25;

                    block_14:
                        GXLoadTexObj(&spDC, GX_TEXMAP0);
                        GXSetTevColor(GX_TEVREG0, color_reg0);

                        f32 var_f27 = housi_packet->mHousiEff[j].field_0x48 * 9.0f;
                        if (g_env_light.field_0xea9 == 1) {
                            var_f27 = housi_packet->mHousiEff[j].field_0x48 * 18.0f;
                        }

                        f32 temp_f28 =
                            (var_f27 * 0.2f) * cM_fsin(housi_packet->mHousiEff[j].mScale.x * 5.0f);
                        f32 temp_f30 =
                            (var_f27 * 0.2f) * cM_fcos(housi_packet->mHousiEff[j].mScale.y * 6.0f);

                        if (dKy_darkworld_check() == 1 || var_r28 == 1) {
                            cXyz sp7C[] = {
                                cXyz(-1.0f, -0.5f, 0.0f),
                                cXyz(-1.0f, 1.5f, 0.0f),
                                cXyz(1.0f, 1.5f, 0.0f),
                                cXyz(1.0f, -0.5f, 0.0f),
                            };

                            for (int k = 0; k < 4; k++) {
                                cXyz spAC;
                                cXyz spA0;

                                f32 temp_f26_2 = cM_ssin((f32)j * 123.0f + (f32)(g_Counter.mCounter0 * 600));

                                cXyz* temp_r3 = &sp7C[k];
                                spAC.x = temp_r3->x * (8.0f * (1.0f + (temp_f26_2 * 0.3f)));
                                spAC.y = temp_r3->y * (8.0f * (1.0f + (temp_f26_2 * 0.3f)));
                                spAC.z = temp_r3->z * (8.0f * (1.0f + (temp_f26_2 * 0.3f)));

                                mDoMtx_stack_c::transS(spD0.x, spD0.y, spD0.z);
                                mDoMtx_stack_c::YrotM(temp_f26_2 * 65536.0f);
                                mDoMtx_stack_c::multVec(&spAC, &spA0);
                                pos[k] = spA0;
                            }
                        } else if (g_env_light.field_0xea9 == 2) {
                            cXyz sp4C[] = {
                                cXyz(-1.0f, -0.9f, 0.0f),
                                cXyz(-1.0f, 1.1f, 0.0f),
                                cXyz(1.0f, 1.1f, 0.0f),
                                cXyz(1.0f, -0.9f, 0.0f),
                            };

                            for (int k = 0; k < 4; k++) {
                                cXyz sp94;
                                cXyz sp88;

                                f32 var_f24;
                                if (housi_packet->mHousiEff[j].mStatus == 1 ||
                                    housi_packet->mHousiEff[j].mStatus == 3)
                                {
                                    var_f24 =
                                        0.2f +
                                        (housi_packet->mHousiEff[j].field_0x34 *
                                            (fabsf(cM_ssin((f32)j * 213.0f +
                                                        (f32)(g_Counter.mCounter0 * 330))) *
                                            0.8f));
                                } else {
                                    var_f24 = cM_ssin((f32)j * 123.0f +
                                                        (f32)(g_Counter.mCounter0 * 80));
                                }

                                f32 var_f2;
                                if (g_env_light.fishing_hole_season == 3) {
                                    var_f2 = 15.0f;

                                    if (housi_packet->mHousiEff[j].mStatus == 1) {
                                        var_f24 =
                                            housi_packet->mHousiEff[j].field_0x34 *
                                            fabsf(cM_ssin((f32)j * 250.0f +
                                                            (f32)(g_Counter.mCounter0 * 88)));
                                    } else {
                                        var_f24 = cM_ssin((f32)j * 685.0f +
                                                            (f32)(g_Counter.mCounter0 * 20));
                                    }
                                } else {
                                    var_f2 = 6.0f;
                                }

                                cXyz* temp_r3_2 = &sp4C[k];
                                sp94.x = temp_r3_2->x * (var_f2 * (1.0f + (var_f24 * 0.3f)));
                                sp94.y = temp_r3_2->y * (var_f2 * (1.0f + (var_f24 * 0.3f)));
                                sp94.z = temp_r3_2->z * (var_f2 * (1.0f + (var_f24 * 0.3f)));
                                mDoMtx_stack_c::transS(spD0.x, spD0.y, spD0.z);

                                if (housi_packet->mHousiEff[j].mStatus == 1 ||
                                    housi_packet->mHousiEff[j].mStatus == 3)
                                {
                                    housi_packet->mHousiEff[j].field_0x38 +=
                                        483.0f * (0.5f + (var_f24 * 0.5f));

                                    housi_packet->mHousiEff[j].field_0x44 =
                                        (s16)housi_packet->mHousiEff[j].field_0x38;
                                    mDoMtx_stack_c::YrotM(housi_packet->mHousiEff[j].field_0x38);
                                    mDoMtx_stack_c::XrotM(housi_packet->mHousiEff[j].field_0x38);
                                    mDoMtx_stack_c::ZrotM(housi_packet->mHousiEff[j].field_0x38);
                                } else {
                                    if (housi_packet->mHousiEff[j].mStatus == 2) {
                                        if (g_env_light.fishing_hole_season == 3) {
                                            housi_packet->mHousiEff[j].field_0x38 += var_f24 * 30.0f;
                                        } else {
                                            housi_packet->mHousiEff[j].field_0x38 +=
                                                var_f24 * 100.0f;
                                        }
                                    }

                                    if (housi_packet->mHousiEff[j].field_0x38 > 32765.0f) {
                                        cLib_addCalc(&housi_packet->mHousiEff[j].field_0x44,
                                                        -16384.0f, 0.1f, 500.0f, 0.0001f);
                                    } else {
                                        cLib_addCalc(&housi_packet->mHousiEff[j].field_0x44,
                                                        16384.0f, 0.1f, 500.0f, 0.0001f);
                                    }

                                    mDoMtx_stack_c::YrotM(housi_packet->mHousiEff[j].field_0x38);
                                    mDoMtx_stack_c::XrotM(housi_packet->mHousiEff[j].field_0x44);
                                    mDoMtx_stack_c::ZrotM(housi_packet->mHousiEff[j].field_0x38);
                                }

                                mDoMtx_stack_c::multVec(&sp94, &sp88);
                                pos[k] = sp88;
                            }
                        } else {
                            spC4.x = var_f27 - temp_f30;
                            spC4.y = var_f27 - temp_f28;
                            spC4.z = 0.0f;
                            MTXMultVec(camMtx, &spC4, &spB8);
                            pos[0].x = spD0.x + spB8.x;
                            pos[0].y = spD0.y + spB8.y;
                            pos[0].z = spD0.z + spB8.z;

                            spC4.x = -var_f27 + temp_f30;
                            spC4.y = var_f27 - temp_f28;
                            spC4.z = 0.0f;
                            MTXMultVec(camMtx, &spC4, &spB8);
                            pos[1].x = spD0.x + spB8.x;
                            pos[1].y = spD0.y + spB8.y;
                            pos[1].z = spD0.z + spB8.z;

                            spC4.x = -var_f27 + temp_f30;
                            spC4.y = -var_f27 + temp_f28;
                            spC4.z = 0.0f;
                            MTXMultVec(camMtx, &spC4, &spB8);
                            pos[2].x = spD0.x + spB8.x;
                            pos[2].y = spD0.y + spB8.y;
                            pos[2].z = spD0.z + spB8.z;

                            spC4.x = var_f27 - temp_f30;
                            spC4.y = -var_f27 + temp_f28;
                            spC4.z = 0.0f;
                            MTXMultVec(camMtx, &spC4, &spB8);
                            pos[3].x = spD0.x + spB8.x;
                            pos[3].y = spD0.y + spB8.y;
                            pos[3].z = spD0.z + spB8.z;
                        }

                        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

                        s16 var_r17 = 0x1FF;
                        if (dKy_darkworld_check() == true || var_r28 == 1) {
                            var_r17 = 0xFA;
                        }

                        GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                        GXTexCoord2s16(0, 0);
                        GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                        GXTexCoord2s16(var_r17, 0);
                        GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                        GXTexCoord2s16(var_r17, var_r17);
                        GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                        GXTexCoord2s16(0, var_r17);
                        GXEnd();
                    }
                }
            }

            GXSetClipMode(GX_CLIP_ENABLE);
            J3DShape::resetVcdVatCache();
        }
    }
}

void dKyr_drawSnow(Mtx drawMtx, u8** tex) {
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    dKankyo_snow_Packet* snow_packet = g_env_light.mpSnowPacket;

    static f32 rot = 0.0f;

    Mtx camMtx;
    cXyz pos[4];

    Vec sp94, sp88;
    cXyz sp7C;

    u8 spC = 1;
    GXColor color_reg0;
    GXColor color_reg1;

    GXColor sp64;
    GXColor sp60;
    GXColor sp5C = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor sp58 = {0xFF, 0xFF, 0xFF, 0xFF};
    int sp54;
    int sp4C = 0;

    static cXyz add_table[] = {
        cXyz(0.0f, 0.0f, 0.0f),
        cXyz(50.0f, 50.0f, 50.0f),
        cXyz(-50.0f, 20.0f, 50.0f),
        cXyz(-50.0f, -20.0f, -50.0f),
    };

    static f32 S_fubuki_ratio = 0.0f;

    if (camera != NULL) {
        j3dSys.reinitGX();
        if (snow_packet->field_0x6d88 != 0 || g_env_light.field_0xe90 != 0) {
            if (snow_packet->field_0x6d88 == 0) {
                sp54 = g_env_light.field_0xe90 * 10;
                sp4C = 1;
            } else {
                sp54 = snow_packet->field_0x6d88;
            }
    
            if (!g_env_light.camera_water_in_status) {
                dKy_ParticleColor_get_bg(&camera->lookat.eye, NULL, &sp64, &sp60, &sp5C, &sp58, 0.0f);
                color_reg0.r = 178.5f + (0.3f * sp60.r);
                color_reg0.g = 178.5f + (0.3f * sp60.g);
                color_reg0.b = 178.5f + (0.3f * sp60.b);

                color_reg1.r = 127.5f + (0.5f * sp60.r);
                color_reg1.g = 127.5f + (0.5f * sp60.g);
                color_reg1.b = 127.5f + (0.5f * sp60.b);

                color_reg0.a = 120;
                color_reg1.a = 120;

                f32 sp6C = 4.0f;

                if (dComIfGd_getView() != NULL) {
                    MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
                } else {
                    return;
                }

                if (tex[0] != NULL) {
                    GXTexObj spA0;
                    dKyr_set_btitex(&spA0, (ResTIMG*)tex[0]);
                    GXSetNumChans(0);
                    GXSetTevColor(GX_TEVREG0, color_reg0);
                    GXSetTevColor(GX_TEVREG1, color_reg1);
                    GXSetNumTexGens(1);
                    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
                    GXSetNumTevStages(1);
                    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
                    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
                    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
                    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
                    GXSetClipMode(GX_CLIP_DISABLE);
                    GXSetNumIndStages(0);
                    dKr_cullVtx_Set();

                    Mtx rotMtx;
                    MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));

                    rot += 5.0f + cM_rndFX(2.0f);
                    if (rot > 719.0f) {
                        rot = 0.0f;
                    }

                    MTXConcat(camMtx, rotMtx, camMtx);
                    GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
                    GXSetCurrentMtx(GX_PNMTX0);

                    if (g_env_light.field_0xe92 == 0 && sp4C == 0) {
                        cLib_addCalc(&S_fubuki_ratio, 0.0f, 0.25f, 0.1f, 0.001f);
                        if (S_fubuki_ratio > 0.0f) {
                            spC = 2;
                        }
                    } else {
                        if (g_env_light.field_0xe92) {
                            spC = 2;
                        }
                        cLib_addCalc(&S_fubuki_ratio, 1.0f, 0.25f, 0.1f, 0.001f);
                    }

                    f32 sp50;
                    if (dComIfGd_getView() != NULL) {
                        sp50 = dComIfGd_getView()->fovy / 45.0f;
                        if (sp50 >= 1.0f) {
                            sp50 = 1.0f;
                        }
                    } else {
                        sp50 = 1.0f;
                    }

                    for (int i = 0; i < sp54; i++) {
                        for (int j = 0; j < 2; j++) {
                            f32 sp44 = -1.0f;
                            if (g_env_light.mpSnowPacket->mSnowEff[i].mStatus != 0) {
                                f32 var_f30;
                                f32 temp_f29;
                                f32 temp_f31 = 1.5f + (((i & 15) / 15.0f) * cM_ssin(g_Counter.mCounter0));
                                temp_f29 = 0.3f * ((i & 15) / 15.0f);

                                color_reg0.r = (255.0f * temp_f29) + (sp60.r * (1.0f - temp_f29));
                                color_reg0.g = (255.0f * temp_f29) + (sp60.g * (1.0f - temp_f29));
                                color_reg0.b = (255.0f * temp_f29) + (sp60.b * (1.0f - temp_f29));

                                f32 sp40, sp3C;
                                if (j == 0) {
                                    sp7C = snow_packet->mSnowEff[i].mPosition;
                                    sp40 = snow_packet->mSnowEff[i].mScale * snow_packet->mSnowEff[i].mScale;
                                    sp3C = (snow_packet->mSnowEff[i].mScale - sp40) + snow_packet->mSnowEff[i].mScale;

                                    color_reg0.a = 180.0f * ((snow_packet->mSnowEff[i].mScale * 0.8f) + temp_f29);
                                } else {
                                    sp7C = snow_packet->mSnowEff[i].mBasePos;
                                    sp40 = snow_packet->mSnowEff[i].field_0x30 * snow_packet->mSnowEff[i].field_0x30;
                                    sp3C = (snow_packet->mSnowEff[i].field_0x30 - sp40) + snow_packet->mSnowEff[i].field_0x30;

                                    color_reg0.a = S_fubuki_ratio * (220.0f * ((0.8f * (snow_packet->mSnowEff[i].field_0x30)) + temp_f29));
                                }

                                if (strcmp(dComIfGp_getStartStageName(), "D_MN11") == 0) {
                                    if (dComIfGp_roomControl_getStayNo() == 4) {
                                        if ((sp7C.x > 2079.0f && sp7C.x < 3013.0f && sp7C.y < 864.0f && sp7C.z > -6000.0f && sp7C.z < -4145.0f) ||
                                            sp7C.x < -2960.0f || sp7C.z > -880.0f || sp7C.z < -6000.0f ||
                                            (sp7C.z < -4920.0f && sp7C.y < 864.0f && sp7C.x < -2000.0f))
                                        {
                                            continue;
                                        }
                                    } else if (snow_packet->field_0x6d88 != 0) {
                                        continue;
                                    }
                                }

                                GXSetTevColor(GX_TEVREG0, color_reg0);
                                f32 sp38 = 2.0f * (i / 500.0f) * snow_packet->field_0x6d80;
                                f32 sp68 = sp50 * (camera->lookat.eye.abs(sp7C) / 1000.0f);
                                if (sp68 > 1.0f) {
                                    sp68 = 1.0f;
                                }

                                if (g_env_light.field_0xe92 == 0) {
                                    var_f30 = sp68 * (sp3C * (sp6C + (int)(10.0f * ((i & 15) / 15.0f))));
                                } else {
                                    var_f30 = sp68 * (sp3C * (sp6C + (int)(14.0f * ((i & 15) / 15.0f))));
                                }

                                sp94.x = -var_f30 * sp44;
                                sp94.y = var_f30 - sp38;
                                sp94.z = 0.0f;
                                cMtx_multVec(camMtx, &sp94, &sp88);
                                pos[0].x = sp7C.x + sp88.x;
                                pos[0].y = sp7C.y + sp88.y;
                                pos[0].z = sp7C.z + sp88.z;

                                sp94.x = var_f30 * sp44;
                                sp94.y = var_f30;
                                sp94.z = 0.0f;
                                cMtx_multVec(camMtx, &sp94, &sp88);
                                pos[1].x = sp7C.x + sp88.x;
                                pos[1].y = sp7C.y + sp88.y;
                                pos[1].z = sp7C.z + sp88.z;

                                sp94.x = var_f30 * sp44;
                                sp94.y = -var_f30;
                                sp94.z = 0.0f;
                                cMtx_multVec(camMtx, &sp94, &sp88);
                                pos[2].x = sp7C.x + sp88.x;
                                pos[2].y = sp7C.y + sp88.y;
                                pos[2].z = sp7C.z + sp88.z;

                                sp94.x = -var_f30 * sp44;
                                sp94.y = -var_f30 - sp38;
                                sp94.z = 0.0f;
                                cMtx_multVec(camMtx, &sp94, &sp88);
                                pos[3].x = sp7C.x + sp88.x;
                                pos[3].y = sp7C.y + sp88.y;
                                pos[3].z = sp7C.z + sp88.z;

                                for (int k = 0; k < spC; k++) {
                                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                                    GXPosition3f32(pos[0].x + (temp_f31 * add_table[k].x), pos[0].y + (temp_f31 * add_table[k].y), pos[0].z + (temp_f31 * add_table[k].z));
                                    GXTexCoord2s16(0, 0);
                                    GXPosition3f32(pos[1].x + (temp_f31 * add_table[k].x), pos[1].y + (temp_f31 * add_table[k].y), pos[1].z + (temp_f31 * add_table[k].z));
                                    GXTexCoord2s16(0xFF, 0);
                                    GXPosition3f32(pos[2].x + (temp_f31 * add_table[k].x), pos[2].y + (temp_f31 * add_table[k].y), pos[2].z + (temp_f31 * add_table[k].z));
                                    GXTexCoord2s16(0xFF, 0xFF);
                                    GXPosition3f32(pos[3].x + (temp_f31 * add_table[k].x), pos[3].y + (temp_f31 * add_table[k].y), pos[3].z + (temp_f31 * add_table[k].z));
                                    GXTexCoord2s16(0, 0xFF);
                                    GXEnd();
                                }

                                if ((g_env_light.field_0xe90 != 0 && dComIfGp_roomControl_getStayNo() == 0 && sp7C.z < 3000.0f) || dComIfGp_roomControl_getStayNo() == 3 || dComIfGp_roomControl_getStayNo() == 6 || dComIfGp_roomControl_getStayNo() == 9 || dComIfGp_roomControl_getStayNo() == 13) {
                                    color_reg0.a = 255.0f * ((0.4f * snow_packet->mSnowEff[i].field_0x30) + temp_f29);
                                    GXSetTevColor(GX_TEVREG0, color_reg0);

                                    f32 sp34;
                                    f32 sp30;
                                    if (dComIfGp_roomControl_getStayNo() == 0) {
                                        sp30 = -24.0f;
                                    } else if (dComIfGp_roomControl_getStayNo() == 6) {
                                        sp30 = 952.0f;
                                    } else if (dComIfGp_roomControl_getStayNo() == 3) {
                                        sp30 = -24.0f;
                                    } else if (dComIfGp_roomControl_getStayNo() == 9) {
                                        sp30 = -52.0f;
                                    } else if (dComIfGp_roomControl_getStayNo() == 13) {
                                        sp30 = 2.0f;
                                    } else {
                                        sp30 = 0.0f;
                                    }

                                    sp34 = sp7C.y - sp30;
                                    sp34 = sp30 - sp34;

                                    sp94.x = -var_f30 * sp44;
                                    sp94.y = var_f30 - sp38;
                                    sp94.z = 0.0f;
                                    cMtx_multVec(camMtx, &sp94, &sp88);
                                    pos[0].x = sp7C.x + sp88.x;
                                    pos[0].y = sp34 + sp88.y;
                                    pos[0].z = sp7C.z + sp88.z;

                                    sp94.x = (var_f30 * sp44);
                                    sp94.y = var_f30;
                                    sp94.z = 0.0f;
                                    cMtx_multVec(camMtx, &sp94, &sp88);
                                    pos[1].x = sp7C.x + sp88.x;
                                    pos[1].y = sp34 + sp88.y;
                                    pos[1].z = sp7C.z + sp88.z;

                                    sp94.x = (var_f30 * sp44);
                                    sp94.y = -var_f30;
                                    sp94.z = 0.0f;
                                    cMtx_multVec(camMtx, &sp94, &sp88);
                                    pos[2].x = sp7C.x + sp88.x;
                                    pos[2].y = sp34 + sp88.y;
                                    pos[2].z = sp7C.z + sp88.z;

                                    sp94.x = (-var_f30 * sp44);
                                    sp94.y = (-var_f30 - sp38);
                                    sp94.z = 0.0f;
                                    cMtx_multVec(camMtx, &sp94, &sp88);
                                    pos[3].x = sp7C.x + sp88.x;
                                    pos[3].y = sp34 + sp88.y;
                                    pos[3].z = sp7C.z + sp88.z;

                                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                                    int var_r27 = 0;
                                    GXPosition3f32(pos[0].x + (temp_f31 * add_table[var_r27].x), pos[0].y + (temp_f31 * add_table[var_r27].y), pos[0].z + (temp_f31 * add_table[var_r27].z));
                                    GXTexCoord2s16(0, 0);
                                    GXPosition3f32(pos[1].x + (temp_f31 * add_table[var_r27].x), pos[1].y + (temp_f31 * add_table[var_r27].y), pos[1].z + (temp_f31 * add_table[var_r27].z));
                                    GXTexCoord2s16(0xFF, 0);
                                    GXPosition3f32(pos[2].x + (temp_f31 * add_table[var_r27].x), pos[2].y + (temp_f31 * add_table[var_r27].y), pos[2].z + (temp_f31 * add_table[var_r27].z));
                                    GXTexCoord2s16(0xFF, 0xFF);
                                    GXPosition3f32(pos[3].x + (temp_f31 * add_table[var_r27].x), pos[3].y + (temp_f31 * add_table[var_r27].y), pos[3].z + (temp_f31 * add_table[var_r27].z));
                                    GXTexCoord2s16(0, 0xFF);
                                    GXEnd();
                                }
                            }
                        }
                    }

                    GXSetClipMode(GX_CLIP_ENABLE);
                    J3DShape::resetVcdVatCache();
                }
            }
        }
    }
}

void dKyr_drawStar(Mtx drawMtx, u8** tex) {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_star_Packet* star_packet = g_env_light.mpStarPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    static f32 rot = 0.0f;

    cXyz pos[4];

    Mtx camMtx;
    cXyz spBC;
    cXyz moon_proj;
    cXyz star_proj;

    Vec sp98, sp8C;
    cXyz moon_pos;

    u8 gwolf_howl_stage = false;
    u8 draw_mirrored = false;
    BOOL sp38 = false;

    // Cassiopeia and Orion constellation positions
    static csXyz hokuto_position[] = {
        csXyz(15283, 31005, -17919),
        csXyz(13525, 28369, -22265),
        csXyz(8300, 31884, -20507),
        csXyz(3906, 31005, -23144),
        csXyz(-439, 30127, -17919),
        csXyz(-7421, 31005, 18798),
        csXyz(-10937, 28000, 15000),
        csXyz(-10000, 24902, 18400),
        csXyz(-9400, 22500, 15900),
        csXyz(-9179, 21300, 14300),
        csXyz(-10300, 22000, 21000),
        csXyz(-16000, 25500, 20000),
        csXyz(0, 30000, 19000),
    };

#if VERSION == VERSION_GCN_JPN
    if (g_env_light.hide_vrbox) {
        return;
    }
#endif

    if (star_packet->mEffectNum != 0) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP200") == 0 && dComIfG_play_c::getLayerNo(0) == 0) {
            gwolf_howl_stage = true;
        } else if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 ||
                   ((strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0) && dComIfGp_roomControl_getStayNo() == 0))
        {
            // draw stars on the opposite skybox hemisphere as well for water reflections
            draw_mirrored = true;
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0 && dKy_daynight_check()) {
            sp38 = true;
        }

        GXColor color_reg0;
        color_reg0.r = 0xDC;
        color_reg0.g = 0xE6;
        color_reg0.b = 0xFF;
        color_reg0.a = 0xFF;

        if (dComIfGd_getView() != NULL) {
            MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
        } else {
            return;
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP200") == 0 && dComIfG_play_c::getLayerNo(0) == 0) {
            moon_pos = envlight->moon_pos;
        } else {
            moon_pos = camera->lookat.eye + envlight->moon_pos;
            if (sp38) {
                moon_pos.x = 3900.0f + camera->lookat.eye.x;
                moon_pos.y = 8052.0f + camera->lookat.eye.y;
                moon_pos.z = -9072.0f + camera->lookat.eye.z;
            }
        }

        mDoLib_project(&moon_pos, &moon_proj);

        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevColor(GX_TEVREG0, color_reg0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetNumIndStages(0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

        Mtx rotMtx;
        MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
        MTXConcat(camMtx, rotMtx, camMtx);

        GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);

        rot += 0.65f;
        if (rot > 719.0f) {
            rot = 0.0f;
        }

        spBC.x = camera->lookat.eye.x;
        spBC.y = camera->lookat.eye.y;
        spBC.z = camera->lookat.eye.z;

        f32 sp34 = -1.0f;
        int sp30 = 0;
        f32 var_f30 = 0.0f;

        if (dComIfGd_getView() != NULL) {
            var_f30 = dComIfGd_getView()->fovy / 45.0f;
            if (var_f30 >= 1.0f) {
                var_f30 = 1.0f;
            }
            var_f30 = 1.0f - var_f30;
        }

        f32 temp_f27 = 0.28f * (1.0f - var_f30);

        sp98.x = 0.0f;
        sp98.y = temp_f27;
        sp98.z = 0.0f;
        cMtx_multVec(camMtx, &sp98, &sp8C);
        pos[0].x = spBC.x + sp8C.x;
        pos[0].y = spBC.y + sp8C.y;
        pos[0].z = spBC.z + sp8C.z;

        sp98.x = temp_f27;
        sp98.y = -(0.5f * temp_f27);
        sp98.z = 0.0f;
        cMtx_multVec(camMtx, &sp98, &sp8C);
        pos[1].x = spBC.x + sp8C.x;
        pos[1].y = spBC.y + sp8C.y;
        pos[1].z = spBC.z + sp8C.z;

        sp98.x = -temp_f27;
        sp98.y = -(0.5f * temp_f27);
        sp98.z = 0.0f;
        cMtx_multVec(camMtx, &sp98, &sp8C);
        pos[2].x = spBC.x + sp8C.x;
        pos[2].y = spBC.y + sp8C.y;
        pos[2].z = spBC.z + sp8C.z;

        int sp48 = 0;
        int sp44 = 0;
        f32 var_f28 = 0.0f;

        for (int i = 0; i < star_packet->mEffectNum; i++) {
            f32 var_f31;
            cXyz star_pos;
            f32 sp2C = 300.0f;

            if (i < 13) {
                star_pos.x = hokuto_position[i].x;
                star_pos.y = hokuto_position[i].y;
                star_pos.z = hokuto_position[i].z;

                if (i <= 4) {
                    var_f31 = 540.0f + star_packet->mEffect[0].field_0x28;
                } else {
                    var_f31 = 400.0f + star_packet->mEffect[0].field_0x28;
                }
                var_f31 -= var_f30 * (0.5f * var_f31);

                color_reg0.r = 0xA0;
                color_reg0.g = 0x8C;
                color_reg0.b = 0x96;
                color_reg0.a = 0xC3;
            } else {
                var_f31 = 0.8f + (star_packet->mEffect[0].field_0x28 * (0.03125f * (i & 31)));
                f32 temp_f29 = 1.0f - (0.004950495f * var_f28);

                star_pos.x = temp_f29 * (sp2C * -cM_ssin((sp48 - 0x8000)));
                star_pos.y = 45.0f + var_f28;
                star_pos.z = temp_f29 * (sp2C * cM_scos((sp48 - 0x8000)));

                sp48 += sp44;
                sp44 += 2250;

                temp_f29 = var_f28 / 200.0f;
                temp_f29 *= temp_f29 * temp_f29;
                var_f28 += 1.0f + (3.0f * temp_f29);
                if (var_f28 > 200.0f) {
                    var_f28 = 20.0f * (i / 1200.0f);
                }

                static const GXColor star_col[] = {
                    /* red   */ {0xFF, 0xBE, 0xC8, 0xA0},
                    /* green */ {0xC8, 0xFF, 0xBE, 0x78},
                    /* blue  */ {0xC8, 0xBE, 0xFF, 0x50},
                    /* white */ {0xFF, 0xFF, 0xFF, 0xC8},
                };

                color_reg0 = star_col[i & 3];
                color_reg0.a = 70.0f + ((i & 63) * 2);
            }

            GXSetTevColor(GX_TEVREG0, color_reg0);

            cXyz sp68;
            cXyz sp5C;
            u16 date = dComIfGs_getDate();
            sp68.x = spBC.x + star_pos.x;
            sp68.y = spBC.y + star_pos.y;
            sp68.z = spBC.z + star_pos.z;

            mDoLib_project(&sp68, &star_proj);

            moon_proj.z = 0.0f;
            star_proj.z = 0.0f;
            f32 moon_dist_to_star = moon_proj.abs(star_proj);

            f32 moon_threshold;
            if (gwolf_howl_stage) {
                moon_threshold = 180.0f + (700.0f * (var_f30 * var_f30));
            } else {
                moon_threshold = 80.0f + (700.0f * (var_f30 * var_f30));
            }

            int weekday = date % 8;
            if (g_env_light.getDaytime() < 180.0f) {
                if (weekday != 0) {
                    weekday--;
                } else {
                    weekday = 7;
                }
            }

            if (weekday == 4 && !gwolf_howl_stage) {
                // it's a New Moon, so no threshold needed
                moon_threshold = 0.0f;
            }

            // if a star is too close to the moon then avoid drawing
            if (moon_dist_to_star > moon_threshold) {
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
                GXPosition3f32(sp68.x + (var_f31 * (pos[0].x - spBC.x)), sp68.y + (var_f31 * (pos[0].y - spBC.y)), sp68.z + (var_f31 * (pos[0].z - spBC.z)));
                GXPosition3f32(sp68.x + (var_f31 * (pos[1].x - spBC.x)), sp68.y + (var_f31 * (pos[1].y - spBC.y)), sp68.z + (var_f31 * (pos[1].z - spBC.z)));
                GXPosition3f32(sp68.x + (var_f31 * (pos[2].x - spBC.x)), sp68.y + (var_f31 * (pos[2].y - spBC.y)), sp68.z + (var_f31 * (pos[2].z - spBC.z)));
                GXEnd();

                if (draw_mirrored) {
                    sp5C.x = spBC.x + star_pos.x;
                    sp5C.y = spBC.y - star_pos.y;
                    sp5C.z = spBC.z + star_pos.z;

                    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
                    GXPosition3f32(sp5C.x + (var_f31 * (pos[0].x - spBC.x)), sp5C.y + (var_f31 * (pos[0].y - spBC.y)), sp5C.z + (var_f31 * (pos[0].z - spBC.z)));
                    GXPosition3f32(sp5C.x + (var_f31 * (pos[1].x - spBC.x)), sp5C.y + (var_f31 * (pos[1].y - spBC.y)), sp5C.z + (var_f31 * (pos[1].z - spBC.z)));
                    GXPosition3f32(sp5C.x + (var_f31 * (pos[2].x - spBC.x)), sp5C.y + (var_f31 * (pos[2].y - spBC.y)), sp5C.z + (var_f31 * (pos[2].z - spBC.z)));
                    GXEnd();
                }
            }
        }

        J3DShape::resetVcdVatCache();
    }
}

void drawCloudShadow(Mtx drawMtx, u8** tex) {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_cloud_Packet* cloud_packet = g_env_light.mpCloudPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    static f32 rot = 0.0f;

    Mtx camMtx;
    Mtx rotMtx;
    Mtx sp120;
    Mtx spF0;

    Vec sp74, sp68;
    cXyz sp5C;
    GXColor color_reg0;
    GXColor color_reg1;
    GXColor sp48;
    GXColor sp44;
    GXColor sp40 = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor sp3C = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor sp38 = {0xFF, 0xFF, 0xFF, 0xFF};

    f32 var_f29 = 1.0f;

    if (cloud_packet->mCount > 0) {
        j3dSys.reinitGX();
        
        if (dComIfGd_getView() != NULL) {
            MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
        } else {
            OS_REPORT("\ndrawCloud ikinasi return!!");
            return;
        }

        GXSetClipMode(GX_CLIP_DISABLE);

        GXTexObj texobj, fb_texobj;
        if (g_env_light.mMoyaMode < 50) {
            dKy_ParticleColor_get_bg(&camera->lookat.eye, NULL, &sp48, &sp44, &sp40, &sp3C, 0.0f);
            f32 temp_f30 = 0.4f;
            
            color_reg0.r = (sp38.r * temp_f30) + (sp44.r * (1.0f - temp_f30));
            color_reg0.g = (sp38.g * temp_f30) + (sp44.g * (1.0f - temp_f30));
            color_reg0.b = (sp38.b * temp_f30) + (sp44.b * (1.0f - temp_f30));

            color_reg1.r = (0.45f * sp38.r) + (0.55f * sp44.r);
            color_reg1.g = (0.45f * sp38.g) + (0.55f * sp44.g);
            color_reg1.b = (0.45f * sp38.b) + (0.55f * sp44.b);

            dKyr_set_btitex(&texobj, (ResTIMG*)tex[0]);
            GXSetNumChans(0);
            GXSetTevColor(GX_TEVREG0, color_reg0);
            GXSetTevColor(GX_TEVREG1, color_reg1);
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            dKy_GxFog_set();

            if (g_env_light.mMoyaMode == 3 || g_env_light.mMoyaMode == 4 || g_env_light.mMoyaMode == 6 || g_env_light.mMoyaMode == 10 || g_env_light.mMoyaMode == 11) {
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_COPY);
            } else {
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
            }

            GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
            GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
            GXSetNumIndStages(0);
            dKr_cullVtx_Set();

            MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
            MTXConcat(camMtx, rotMtx, camMtx);
            GXLoadPosMtxImm(drawMtx, GX_PNMTX0);

            rot -= 0.45f;
            if (rot < 0.0f) {
                rot = 719.0f;
            }

            GXSetCurrentMtx(GX_PNMTX0);
        } else {
            color_reg0.r = 0xFF;
            color_reg0.g = 0xFF;
            color_reg0.b = 0x73;

            color_reg1.r = 0x50;
            color_reg1.g = 0x32;
            color_reg1.b = 0;
            color_reg1.a = 0xFF;

            dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[0], GX_TEXMAP1);

            ResTIMG* fb_timg = mDoGph_gInf_c::getFrameBufferTimg();
            dDlst_window_c* window = dComIfGp_getWindow(0);
            camera_class* window_cam = dComIfGp_getCamera(window->getCameraID());
            dKyr_set_btitex_common(&fb_texobj, fb_timg, GX_TEXMAP0);

            f32 scale = 0.49f;
            C_MTXLightPerspective(sp120, window_cam->fovy, window_cam->aspect, scale, -scale, 0.5f, 0.5f);
            #if WIDESCREEN_SUPPORT
            mDoGph_gInf_c::setWideZoomLightProjection(sp120);
            #endif
            cMtx_concat(sp120, j3dSys.getViewMtx(), spF0);

            rot += 2.0f;
            MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
            MTXConcat(camMtx, rotMtx, camMtx);

            GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXLoadTexMtxImm(spF0, GX_TEXMTX0, GX_MTX3x4);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 8);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
            GXSetTevColor(GX_TEVREG0, color_reg0);
            GXSetTevColor(GX_TEVREG1, color_reg1);
            GXSetNumTexGens(2);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
            GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
            GXSetNumTevStages(2);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
            GXSetZCompLoc(GX_TRUE);
            GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
            GXSetCullMode(GX_CULL_NONE);
            GXSetNumIndStages(0);
        }

        if (g_env_light.mMoyaMode == 7 || g_env_light.mMoyaMode == 8 || g_env_light.mMoyaMode == 10 || g_env_light.mMoyaMode == 11) {
            var_f29 = g_env_light.field_0xebc;
        }

        for (int i = 0; i < cloud_packet->mCount; i++) {
            cXyz pos[4];
            f32 size = cloud_packet->mCloudEff[i].mSize;

            if (!(cloud_packet->mCloudEff[i].mAlpha <= 0.01f)) {
                color_reg0.a = 255.0f * (cloud_packet->mCloudEff[i].mAlpha * var_f29);
                GXSetTevColor(GX_TEVREG0, color_reg0);

                sp5C.x = cloud_packet->mCloudEff[i].mBasePos.x + cloud_packet->mCloudEff[i].mPosition.x;
                sp5C.y = cloud_packet->mCloudEff[i].mBasePos.y + cloud_packet->mCloudEff[i].mPosition.y;
                sp5C.z = cloud_packet->mCloudEff[i].mBasePos.z + cloud_packet->mCloudEff[i].mPosition.z;

                sp74.x = -size;
                sp74.y = size;
                sp74.z = 0.0f;
                cMtx_multVec(camMtx, &sp74, &sp68);
                pos[0].x = sp5C.x + sp68.x;
                pos[0].y = sp5C.y + sp68.y;
                pos[0].z = sp5C.z + sp68.z;

                sp74.x = size;
                sp74.y = size;
                sp74.z = 0.0f;
                cMtx_multVec(camMtx, &sp74, &sp68);
                pos[1].x = sp5C.x + sp68.x;
                pos[1].y = sp5C.y + sp68.y;
                pos[1].z = sp5C.z + sp68.z;

                sp74.x = size;
                sp74.y = -size;
                sp74.z = 0.0f;
                cMtx_multVec(camMtx, &sp74, &sp68);
                pos[2].x = sp5C.x + sp68.x;
                pos[2].y = sp5C.y + sp68.y;
                pos[2].z = sp5C.z + sp68.z;

                sp74.x = -size;
                sp74.y = -size;
                sp74.z = 0.0f;
                cMtx_multVec(camMtx, &sp74, &sp68);
                pos[3].x = sp5C.x + sp68.x;
                pos[3].y = sp5C.y + sp68.y;
                pos[3].z = sp5C.z + sp68.z;

                if (g_env_light.mMoyaMode < 50) {
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                    GXTexCoord2s16(0, 0);
                    GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                    GXTexCoord2s16(0xFF, 0);
                    GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                    GXTexCoord2s16(0xFF, 0xFF);
                    GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                    GXTexCoord2s16(0, 0xFF);
                    GXEnd();
                } else {
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                    GXTexCoord2s16(0, 0);
                    GXTexCoord2s16(0, 0);
                    GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                    GXTexCoord2s16(0xFF, 0);
                    GXTexCoord2s16(0xFF, 0);
                    GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                    GXTexCoord2s16(0xFF, 0xFF);
                    GXTexCoord2s16(0xFF, 0xFF);
                    GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                    GXTexCoord2s16(0, 0xFF);
                    GXTexCoord2s16(0, 0xFF);
                    GXEnd();
                }
            }
        }

        GXSetClipMode(GX_CLIP_ENABLE);
        J3DShape::resetVcdVatCache();
    }
}

void drawVrkumo(Mtx drawMtx, GXColor& color, u8** tex) {
    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_vrkumo_Packet* vrkumo_packet = g_env_light.mpVrkumoPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    camera_class* camera2 = (camera_class*)dComIfGp_getCamera(0);

    Mtx camMtx;
    Mtx rotMtx;

    GXTexObj texobj;
    cXyz proj;

    f32 rot;
    f32 spCC;
    int pass = 1;
    f32 spC4 = 0.0f;

#if VERSION == VERSION_GCN_JPN
    if (g_env_light.hide_vrbox) {
        return;
    }
#endif

    cXyz sp15C;
    cXyz sp150;
    cXyz sp144;
    cXyz sp138;
    cXyz sp12C;
    cXyz sp120;
    cXyz sp114;
    cXyz sp108;
    cXyz spFC;

    f32 spC0;
    f32 spBC;
    f32 spB8;
    f32 spB4;
    f32 spB0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;

    int sp88;
    int sp84;
    int sp80 = 0;
    GXColor color_reg1;

    int j, k;

    if (camera2 != NULL) {
        spC4 = camera2->mCamera.TrimHeight();
    }

    if (dComIfGd_getView() != NULL) {
        MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
    } else {
        OS_REPORT("\ndrawCloud ikinasi return!!");
        return;
    }

    rot = cAngle::s2d(fopCamM_GetBank(camera));

    sp88 = 0;
    spCC = 100000.0f;

    f32 unused;
    f32 y_pos;

    if (dComIfGd_getView() != NULL) {
        f32 sp70 = 0.0f;
        dStage_FileList_dt_c* filelist = NULL;
        if (dComIfGp_roomControl_getStayNo() >= 0) {
            filelist = dComIfGp_roomControl_getStatusRoomDt(dComIfGp_roomControl_getStayNo())->getFileListInfo();
        }

        if (filelist != NULL) {
            sp70 = dStage_FileList_dt_SeaLevel(filelist);
        }

#if DEBUG
        if (g_kankyoHIO.vrbox.field_0x14) {
            sp70 = g_kankyoHIO.vrbox.m_horizonHeight;
        } else {
            g_kankyoHIO.vrbox.m_horizonHeight = sp70;
        }
#endif

        unused = 1.0f - (0.09f * (camera->lookat.eye.y - sp70));
    }

    if (g_env_light.daytime > 105.0f && g_env_light.daytime < 240.0f && !dComIfGp_event_runCheck() && sun_packet != NULL && sun_packet->mSunAlpha > 0.0f) {
        mDoLib_project(&sun_packet->mPos[0], &proj);
        if (proj.x > 0.0f && proj.x < FB_WIDTH && proj.y > spC4 && proj.y < (458.0f - spC4)) {
            pass = 0;
        }
    }

    for (; pass < 2; pass++) {
        if (pass == 0) {
            GXSetColorUpdate(GX_DISABLE);
            GXSetClipMode(GX_CLIP_DISABLE);
        } else {
            GXSetClipMode(GX_CLIP_ENABLE);
        }

        for (j = 0; j < 3; j++) {
            color.r = g_env_light.vrbox_kumo_bottom_col.r;
            color.g = g_env_light.vrbox_kumo_bottom_col.g;
            color.b = g_env_light.vrbox_kumo_bottom_col.b;

            color_reg1.r = 0;
            color_reg1.g = 0;
            color_reg1.b = 0;
            dKyr_set_btitex(&texobj, (ResTIMG*)tex[j]);

            GXSetNumChans(0);
            GXSetTevColor(GX_TEVREG0, color);
            GXSetTevColor(GX_TEVREG1, color_reg1);
            GXSetNumTexGens(1);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1.0f, color);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);

            if (pass == 0) {
                GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
                GXSetZCompLoc(GX_FALSE);
                GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
            } else {
                GXSetAlphaCompare(GX_GREATER, 4, GX_AOP_OR, GX_GREATER, 4);
                GXSetZCompLoc(GX_TRUE);
                GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
            }

            GXSetNumIndStages(0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

            if (pass == 1 && j == 0) {
                MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
                MTXConcat(camMtx, rotMtx, camMtx);
            }

            GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);

            for (k = 0; k < 100; k++) {
                cXyz pos[4];

                if (!(vrkumo_packet->mVrkumoEff[k].mAlpha <= 0.0000000001f) && (pass != 0 || !(vrkumo_packet->mVrkumoEff[k].mAlpha < 0.45f))) {
                    f32 sp68;
                    f32 sp64;
                    f32 sp60;
                    f32 sp5C;

                    f32 sp58 = ((j + k) & 15) / 16.0f;
                    f32 sp54 = 1.0f - (sp58 * sp58 * sp58);
                    f32 sp50;

                    static f32 howa_loop_cnt = 0.0f;

#if DEBUG
                    spAC = g_kankyoHIO.navy.cloud_sunny_size;
                    spA8 = g_kankyoHIO.navy.cloud_cloudy_size;
#else
                    spAC = 0.6f;
                    spA8 = 0.84f;
#endif

                    if (dKy_darkworld_check()) {
                        spAC = 0.8f;
                        spA8 = 0.8f;
                    }

                    if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0) {
                        spA8 = 0.65f;
                    }

                    spAC += g_env_light.mVrkumoStrength * (spA8 - spAC);
                    sp68 = sp54 * spAC * vrkumo_packet->mVrkumoEff[k].mDistFalloff;

                    sp50 = cM_fsin(j + (0.0001f * howa_loop_cnt));
                    sp50 *= vrkumo_packet->mVrkumoEff[k].mDistFalloff;
                    howa_loop_cnt += 1.5f * sp58;

                    sp68 += (0.05f * sp68 * sp50);
                    sp64 = sp68 + (sp68 * vrkumo_packet->mVrkumoEff[k].mHeight);

                    if (pass == 0) {
                        color.a = 1;
                    } else {
                        f32 sp4C = 1.0f - vrkumo_packet->mVrkumoEff[k].mDistFalloff;

                        color.r = (f32)g_env_light.vrbox_kumo_top_col.r + (sp4C * ((f32)g_env_light.vrbox_kumo_bottom_col.r - (f32)g_env_light.vrbox_kumo_top_col.r));
                        color.g = (f32)g_env_light.vrbox_kumo_top_col.g + (sp4C * ((f32)g_env_light.vrbox_kumo_bottom_col.g - (f32)g_env_light.vrbox_kumo_top_col.g));
                        color.b = (f32)g_env_light.vrbox_kumo_top_col.b + (sp4C * ((f32)g_env_light.vrbox_kumo_bottom_col.b - (f32)g_env_light.vrbox_kumo_top_col.b));
                        color.a = 255.0f * vrkumo_packet->mVrkumoEff[k].mAlpha;

                        if (j == 1) {
                            color.r *= 0.8f;
                            color.g *= 0.8f;
                            color.b *= 0.8f;
                        } else if (j == 2) {
                            color.r *= 0.92f;
                            color.g *= 0.92f;
                            color.b *= 0.92f;
                        }
                    }

                    if (!(vrkumo_packet->mVrkumoEff[k].mAlpha <= 0.000001f)) {
                        GXLoadTexObj(&texobj, GX_TEXMAP0);
                        GXSetTevColor(GX_TEVREG0, color);

                        sp60 = sp68 * (0.2f + (0.2f * (k / 100.0f)));
                        sp5C = sp68 * (0.55f + (0.3f * (k / 100.0f)));
                        spFC = vrkumo_packet->mVrkumoEff[k].mPosition;

                        spA4 = 0.0f;
                        spA0 = 0.0f;

                        if (j != 0) {
                            switch (k & 3) {
                            case 0:
                                if (j == 1) {
                                    spA4 = sp60;
                                    spA0 = sp5C;
                                } else if (j == 2) {
                                    spA4 = sp5C;
                                    spA0 = sp60;
                                }
                                break;
                            case 1:
                                if (j == 1) {
                                    spA4 = -sp60;
                                    spA0 = sp60;
                                } else if (j == 2) {
                                    spA4 = -sp5C;
                                    spA0 = sp5C;
                                }
                                break;
                            case 2:
                                if (j == 1) {
                                    spA4 = sp5C;
                                    spA0 = -sp60;
                                } else if (j == 2) {
                                    spA4 = sp60;
                                    spA0 = -sp5C;
                                }
                                break;
                            default:
                                if (j == 1) {
                                    spA4 = -sp5C;
                                    spA0 = sp5C;
                                } else if (j == 2) {
                                    spA4 = -sp60;
                                    spA0 = sp60;
                                }
                                break;
                            }
                        }

                        spC0 = JMAFastSqrt((spFC.x * spFC.x) + (spFC.z * spFC.z));
                        spB0 = atan2f(spFC.x, spFC.z);
                        spB4 = atan2f(spFC.y, spC0);
                        spB0 += spA0;
                        spB4 += spA4;

                        sp94 = spB4 / 1.9f;
                        if (sp94 > 1.0f) {
                            sp94 = 1.0f;
                        }
                        sp94 *= sp94 * sp94;

                        sp98 = 0.6f * sp68 * (1.0f + (16.0f * sp94));
                        sp9C = 0.9f * sp64 * (1.0f + (-4.0f * sp94));
                        sp8C = 0.6f * sp68 * (1.0f + (2.0f * sp94));
                        sp90 = 0.0f;
                        spB8 = spB0 + sp98;
                        spBC = spB4 + sp9C;
                        if (spBC > 1.21f) {
                            spBC = 1.21f;
                        }

                        sp144.x = cosf(spBC) * sinf(spB8);
                        sp144.y = sinf(spBC);
                        sp144.z = cosf(spBC) * cosf(spB8);
                        sp12C = sp144;

                        spB8 = spB0 - sp98;
                        spBC = spB4 + sp9C;
                        if (spBC > 1.21f) {
                            spBC = 1.21f;
                        }

                        sp144.x = cosf(spBC) * sinf(spB8);
                        sp144.y = sinf(spBC);
                        sp144.z = cosf(spBC) * cosf(spB8);
                        sp120 = sp144;

                        spB8 = spB0 - sp8C;
                        spBC = spB4 - sp90;

                        sp144.x = cosf(spBC) * sinf(spB8);
                        sp144.y = sinf(spBC);
                        sp144.z = cosf(spBC) * cosf(spB8);
                        sp114 = sp144;

                        spB8 = spB0 + sp8C;
                        spBC = spB4 - sp90;

                        sp144.x = cosf(spBC) * sinf(spB8);
                        sp144.y = sinf(spBC);
                        sp144.z = cosf(spBC) * cosf(spB8);
                        sp108 = sp144;

                        pos[0].x = sp12C.x * spCC;
                        pos[0].y = sp12C.y * spCC;
                        pos[0].z = sp12C.z * spCC;

                        pos[1].x = sp120.x * spCC;
                        pos[1].y = sp120.y * spCC;
                        pos[1].z = sp120.z * spCC;

                        pos[2].x = sp114.x * spCC;
                        pos[2].y = sp114.y * spCC;
                        pos[2].z = sp114.z * spCC;

                        pos[3].x = sp108.x * spCC;
                        pos[3].y = sp108.y * spCC;
                        pos[3].z = sp108.z * spCC;

                        pos[0] += camera->lookat.eye;
                        pos[1] += camera->lookat.eye;
                        pos[2] += camera->lookat.eye;
                        pos[3] += camera->lookat.eye;

                        sp84 = 0;
                        if (dComIfGd_getView()->fovy > 40.0f) {
                            cXyz spF0;

                            f32 x;
                            f32 y;
                            f32 z;
                            for (int sp3C = 0; sp3C < 4; sp3C++) {
                                spF0 = pos[sp3C];
                                x = 100.0f;
                                y = 100.0f;
                                z = 100.0f;
                                mDoLib_project(&spF0, &proj);

                                if (proj.x > -x && proj.x < (FB_WIDTH + x) && proj.y > -y && proj.y < (458.0f + z)) {
                                    break;
                                }

                                if (sp3C == 3) {
                                    sp84 = 1;
                                }
                            }
                        }

                        if (sp84 == 0) {
                            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                            GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                            GXTexCoord2s16(0, 0);
                            GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                            GXTexCoord2s16(0xFF, 0);
                            GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                            GXTexCoord2s16(0xFF, 0xFF);
                            GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                            GXTexCoord2s16(0, 0xFF);
                            GXEnd();
                        }

                        if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 ||
                            strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0 ||
                            strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 ||
                            strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0 ||
                            strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0 ||
#if DEBUG
                            strcmp(dComIfGp_getStartStageName(), "MIYAGI") == 0 ||
                            strcmp(dComIfGp_getStartStageName(), "T_SASA0") == 0 ||
#endif
                            (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0 && dComIfGp_roomControl_getStayNo() == 0))
                        {
                            sp84 = 0;
                            if (dComIfGd_getView()->fovy > 40.0f) {
                                cXyz spE4;

                                f32 x;
                                f32 y;
                                f32 z;
                                for (int sp2C = 0; sp2C < 4; sp2C++) {
                                    spE4 = pos[sp2C];
                                    spE4.y = -pos[sp2C].y;

                                    x = 100.0f;
                                    y = 100.0f;
                                    z = 100.0f;
                                    mDoLib_project(&spE4, &proj);

                                    if (proj.x > -x && proj.x < (FB_WIDTH + x) && proj.y > -y && proj.y < (458.0f + z)) {
                                        break;
                                    }

                                    if (sp2C == 3) {
                                        sp84 = 1;
                                    }
                                }
                            }

                            if (sp84 == 0) {
                                GXBegin(GX_QUADS, GX_VTXFMT0, 4);

                                y_pos = -pos[0].y;
                                GXPosition3f32(pos[0].x, y_pos, pos[0].z);
                                GXTexCoord2s16(0, 0);

                                y_pos = -pos[1].y;
                                GXPosition3f32(pos[1].x, y_pos, pos[1].z);
                                GXTexCoord2s16(0xFF, 0);

                                y_pos = -pos[2].y;
                                GXPosition3f32(pos[2].x, y_pos, pos[2].z);
                                GXTexCoord2s16(0xFF, 0xFF);

                                y_pos = -pos[3].y;
                                GXPosition3f32(pos[3].x, y_pos, pos[3].z);
                                GXTexCoord2s16(0, 0xFF);
                                GXEnd();
                            }
                        }
                    }
                }
            }

            sp88 = 1;
        }

        if (pass == 0) {
            GXSetColorUpdate(GX_ENABLE);
        }
    }

    GXSetClipMode(GX_CLIP_ENABLE);
    dKy_GxFog_set();
    J3DShape::resetVcdVatCache();
}

void dKyr_thunder_init() {
    g_env_light.mThunderEff.mState = 0;
    g_env_light.mThunderEff.field_0x2 = 0;
}

void dKyr_thunder_move() {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    EF_THUNDER* effect = &envlight->mThunderEff;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    cXyz sp28;
    int sp10 = 0;

    if (memcmp(dComIfGp_getStartStageName(), "R_SP", 4) == 0) {
        sp10 = 1;
        if (strcmp(dComIfGp_getStartStageName(), "R_SP30") == 0 && dComIfGp_roomControl_getStayNo() == 4) {
            sp10 = 2;
        }
    }

    switch (effect->mState) {
    case 0:
        effect->mFlashTimer = 0.0f;
        effect->field_0xc = 0.0f;
        effect->field_0x10 = 0.0f;
        effect->field_0x14 = cM_rndFX(10000.0f);
        effect->field_0x18 = cM_rndFX(10000.0f);

        if (dDemo_c::getMode() != 0) {
            u8 sp8 = 0;
            u32 demo_frame = dDemo_c::getFrame();
            if (g_env_light.mThunderEff.field_0x2 != 0) {
                sp8 = 1;
                g_env_light.mThunderEff.field_0x2 = 0;
            }

            if (sp8 == 1) {
                effect->mLightInfluence.mPosition = camera->lookat.eye;
                effect->mLightInfluence.mColor.r = 0;
                effect->mLightInfluence.mColor.g = 0;
                effect->mLightInfluence.mColor.b = 0;
                effect->mLightInfluence.mPow = 90000.0f;
                effect->mLightInfluence.mFluctuation = 1.0f;
                dKy_efplight_set(&effect->mLightInfluence);
                effect->field_0x1c = 0.6f + cM_rndF(0.4f);
                fopKyM_create(PROC_KY_THUNDER, -1, NULL, NULL, NULL);
                effect->mState++;
            }
        } else if (strcmp(dComIfGp_getStartStageName(), "D_MN09B") == 0 && dComIfGp_event_runCheck()) {
            if (g_env_light.mThunderEff.field_0x2 != 0) {
                g_env_light.mThunderEff.field_0x2 = 0;

                effect->mLightInfluence.mPosition = camera->lookat.eye;
                effect->mLightInfluence.mColor.r = 0;
                effect->mLightInfluence.mColor.g = 0;
                effect->mLightInfluence.mColor.b = 0;
                effect->mLightInfluence.mPow = 90000.0f;
                effect->mLightInfluence.mFluctuation = 1.0f;
                dKy_efplight_set(&effect->mLightInfluence);
                effect->field_0x1c = 0.6f + cM_rndF(0.4f);
                fopKyM_create(PROC_KY_THUNDER, -1, NULL, NULL, NULL);
                effect->mState++;
            }
        } else if (cM_rndF(1.0f) < 0.007f) {
            effect->field_0x1c = 0.6f + cM_rndF(0.4f);
            effect->mState = 11;
            if (cM_rndF(1.0f) < 0.2f) {
                fopKyM_create(PROC_KY_THUNDER, -1, NULL, NULL, NULL);
            }
        } else if (cM_rndF(1.0f) < 0.005f && g_env_light.mThunderEff.mMode < 10 && sp10 != 1) {
            effect->mLightInfluence.mPosition = camera->lookat.eye;
            effect->mLightInfluence.mColor.r = 0;
            effect->mLightInfluence.mColor.g = 0;
            effect->mLightInfluence.mColor.b = 0;
            effect->mLightInfluence.mPow = 90000.0f;
            effect->mLightInfluence.mFluctuation = 1.0f;
            dKy_efplight_set(&effect->mLightInfluence);
            effect->field_0x1c = 0.6f + cM_rndF(0.4f);
            fopKyM_create(PROC_KY_THUNDER, -1, NULL, NULL, NULL);
            effect->mState++;
        }
        break;
    case 1:
    case 11:
        cLib_addCalc(&effect->mFlashTimer, 1.0f, 0.3f, 0.2f, 0.001f);
        if (effect->mFlashTimer >= effect->field_0x1c) {
            if (effect->mState < 10) {
                mDoAud_mEnvSe_startNearThunderSe();
            }
            effect->mState++;
        }
        break;
    case 2:
    case 12:
        cLib_addCalc(&effect->mFlashTimer, 0.0f, 0.1f, 0.05f, 0.001f);
        if (effect->mFlashTimer <= 0.0f) {
            if (effect->mState < 10) {
                dKy_efplight_cut(&effect->mLightInfluence);
            }

            effect->mState = 0;
            if (g_env_light.mThunderEff.mMode == 0) {
                g_env_light.mThunderEff.mStatus = 0;
            }
        } else if (effect->mFlashTimer <= 0.5f && effect->mFlashTimer > 0.4f && cM_rndF(1.0f) < 0.3f) {
            effect->field_0x1c = 0.6f + cM_rndF(0.4f);
            if (cM_rndF(1.0f) < 0.05f) {
                fopKyM_create(PROC_KY_THUNDER, -1, NULL, NULL, NULL);
            }
            effect->mState--;
        }
        break;
    }

    if (effect->mState != 0) {
        if (effect->mState < 10) {
            dKyr_get_vectle_calc(&camera->lookat.eye, &camera->lookat.center, &sp28);
            effect->mLightInfluence.mPosition.x = camera->lookat.eye.x - (sp28.x * effect->field_0x14);
            effect->mLightInfluence.mPosition.y = camera->lookat.eye.y + 2000.0f;
            effect->mLightInfluence.mPosition.z = camera->lookat.eye.z - (sp28.z * effect->field_0x18);

            if (sp10 == 2) {
                effect->mLightInfluence.mPosition.x = 195520.0f;
                effect->mLightInfluence.mPosition.y = 4818.0f;
                effect->mLightInfluence.mPosition.z = 1385.0f;
            }

            effect->mLightInfluence.mColor.r = (u8)(effect->mFlashTimer * 75.0f);
            effect->mLightInfluence.mColor.g = (u8)(effect->mFlashTimer * 130.0f);
            effect->mLightInfluence.mColor.b = (u8)(effect->mFlashTimer * 150.0f);

            if (g_env_light.field_0x12d6 == 0) {
                if (sp10 == 0) {
                    dKyr_get_vectle_calc(&camera->lookat.eye, &camera->lookat.center, &sp28);

                    f32 temp_f31;
                    if (sp28.y < 0.2f) {
                        if (sp28.y > 0.0f) {
                            temp_f31 = 1.0f - (sp28.y / 0.2f);
                        } else {
                            temp_f31 = 0.0f;
                        }
                    } else {
                        temp_f31 = 1.0f;
                    }
                    temp_f31 = 0.75f;

                    f32 temp_f29 = 0.25f * (1.0f - temp_f31);
                    f32 temp_f30 = effect->mFlashTimer * effect->mFlashTimer * effect->mFlashTimer;

                    s16 sp60;
                    s16 sp58;
                    s16 sp50;
                    if (sp10 == 0) {
                        sp60 = -64.0f * temp_f31;
                        sp58 = -64.0f * temp_f31;
                        sp50 = -64.0f * temp_f31;
                        dKy_actor_addcol_amb_set(sp60, sp58, sp50, temp_f30);
                    }

                    sp60 = -64.0f * temp_f31;
                    sp58 = -64.0f * temp_f31;
                    sp50 = -64.0f * temp_f31;
                    dKy_bg_addcol_amb_set(sp60, sp58, sp50, temp_f30);

                    
                    s16 sp48 = -64.0f * temp_f31;
                    s16 sp40 = -64.0f * temp_f31;
                    s16 sp38 = -64.0f * temp_f31;
                    dKy_bg1_addcol_amb_set(sp48, sp40, sp38, 0.4f * temp_f30);
                }

                dKy_vrbox_addcol_sky0_set(75, 130, 150, 0.6f * effect->mFlashTimer);
                dKy_vrbox_addcol_kasumi_set(75, 130, 150, 0.5f * effect->mFlashTimer);

                if (1.0f == g_env_light.field_0x1210) {
                    dKy_addcol_fog_set(75, 130, 150, 0.4f * effect->mFlashTimer);
                }
            }
        } else {
            dKy_vrbox_addcol_sky0_set(75, 130, 150, 0.15f * effect->mFlashTimer);
            dKy_vrbox_addcol_kasumi_set(75, 130, 150, 0.35f * effect->mFlashTimer);

            if (1.0f == g_env_light.field_0x1210) {
                dKy_addcol_fog_set(75, 130, 150, 0.12f * effect->mFlashTimer);
            }
        }
    }
}

void dKyr_shstar_init() {}

void dKyr_shstar_move() {}

void dKyr_odour_init() {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_odour_Packet* odour_packet = envlight->mOdourData.mpOdourPacket;

    for (int i = 0; i < 2000; i++) {
        odour_packet->mOdourEff[i].mStatus = 0;
        odour_packet->mOdourEff[i].field_0x28 = 0.0f;
        odour_packet->mOdourEff[i].field_0x2c = 0.0f;
        odour_packet->mOdourEff[i].field_0x24 = 0.0f;
    }
}

void dKyr_odour_move() {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKy_Odour_Data* pOdourData = &envlight->mOdourData;
    dKankyo_odour_Packet* odour_packet = pOdourData->mpOdourPacket;
    
    Mtx camMtx;
    cXyz sp20(0.0f, 1.0f, 0.0f);
    cXyz sp14;

    if (dComIfGd_getView() != NULL) {
        MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
    } else {
        return;
    }

    odour_packet->field_0x17724 -= ((int)cM_rndF(150.0f) + 430);
    odour_packet->field_0x17726 -= ((int)cM_rndF(50.0f) + 200);

    for (int i = 0; i < 2000; i++) {
        EF_ODOUR_EFF* effect = &odour_packet->mOdourEff[i];
        switch (effect->mStatus) {
        case 0:
            break;
        case 1:
        case 11:
            if (cM_rndF(1.0f) < 0.1f) {
                effect->field_0x24 = 0.0f;
                effect->mStatus++;
            }
            break;
        case 2:
        case 12:
        case 3:
        case 13:
            f32 temp_f29 = odour_packet->field_0x17714.abs(effect->mBasePos);
            f32 temp_f31 = temp_f29 / 4000.0f;
            if (temp_f31 < 0.0f) {
                temp_f31 = 0.0f;
            } else if (temp_f31 > 1.0f) {
                temp_f31 = 1.0f;
            }
            temp_f31 = 0.3f;

            f32 temp_f30 = cM_scos(odour_packet->field_0x17724 + 38 * (s16)temp_f29);
            effect->mPosition.x = 60.0f * (temp_f30 * temp_f31);
            effect->mPosition.z = 60.0f * (temp_f30 * temp_f31);

            if (effect->mStatus < 10) {
                temp_f30 = cM_scos(odour_packet->field_0x17724 + ((s16)temp_f29 * 160));
                effect->mPosition.y = 90.0f * ((0.8f + temp_f30) * temp_f31);
                temp_f30 = cM_ssin(odour_packet->field_0x17726 + ((s16)temp_f29 * 45));
                effect->mPosition.y += 110.0f * ((0.8f + temp_f30) * temp_f31);
            } else {
                temp_f30 = cM_scos((int)(78.0f * temp_f29) + (int)(f32)odour_packet->field_0x17724);

                effect->mPosition.y = 0.0f;
                effect->mPosition.x = 150.0f * (temp_f30 * temp_f31);
                effect->mPosition.z = 140.0f * (temp_f30 * temp_f31);

                temp_f30 = cM_ssin(((s16)temp_f29 * 45) + (int)(f32)odour_packet->field_0x17726);
                effect->mPosition.x += 100.0f * (temp_f30 * temp_f31);
                effect->mPosition.z += 100.0f * (temp_f30 * temp_f31);
            }

            mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::ZXYrotM(effect->mRotX, effect->mRotY, 0);
            mDoMtx_stack_c::multVec(&sp20, &sp14);

            effect->mBasePos.x += sp14.x * effect->field_0x20;
            effect->mBasePos.y += sp14.y * effect->field_0x20;
            effect->mBasePos.z += sp14.z * effect->field_0x20;

            if (effect->mStatus == 2 || effect->mStatus == 12) {
                cLib_addCalc(&effect->field_0x24, 1.0f, 0.1f, 0.2f, 0.001f);
                if (effect->field_0x24 >= 1.0f) {
                    effect->mStatus++;
                }
            } else if (effect->mStatus == 3 || effect->mStatus == 13) {
                cLib_addCalc(&effect->field_0x24, 0.0f, 0.1f, 0.06f, 0.001f);
                if (effect->field_0x24 <= 0.1f) {
                    effect->field_0x24 = 0.0f;
                    effect->mStatus = 0;
                }
            }
        }
    }
}

void dKyr_odour_draw(Mtx drawMtx, u8** tex) {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_odour_Packet* odour_packet = envlight->mOdourData.mpOdourPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    static f32 rot = 0.0f;

    Mtx camMtx;
    Mtx rotMtx;
    Mtx sp120;
    Mtx spF0;
    cXyz sp70;

    j3dSys.reinitGX();

    if (dComIfGd_getView() != NULL) {
        MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
    } else {
        OS_REPORT("\nodour_draw return!!");
        return;
    }

    if (envlight->senses_effect_strength <= 0.0f || envlight->now_senses_effect != 1) {
        return;
    }

    GXSetClipMode(GX_CLIP_DISABLE);

    GXColor color_reg0;
    GXColor color_reg1;

    switch (dComIfGs_getCollectSmell()) {
    case fpcNm_ITEM_SMELL_YELIA_POUCH:
        color_reg0.r = 0xFF;
        color_reg0.g = 0xFF;
        color_reg0.b = 0xFF;

        color_reg1.r = 0xFF;
        color_reg1.g = 0x78;
        color_reg1.b = 0xA8;
        color_reg1.a = 0xFF;
        break;
    case fpcNm_ITEM_SMELL_FISH:
        color_reg0.r = 0xFF;
        color_reg0.g = 0xA1;
        color_reg0.b = 0x3B;

        color_reg1.r = 0xD6;
        color_reg1.g = 0x60;
        color_reg1.b = 0x32;
        color_reg1.a = 0xFF;
        break;
    case fpcNm_ITEM_SMELL_CHILDREN:
        color_reg0.r = 0xFF;
        color_reg0.g = 0xDB;
        color_reg0.b = 0xFF;

        color_reg1.r = 0xE4;
        color_reg1.g = 0xAC;
        color_reg1.b = 0x52;
        color_reg1.a = 0xFF;
        break;
    case fpcNm_ITEM_SMELL_MEDICINE:
        color_reg0.r = 0xD5;
        color_reg0.g = 0xCE;
        color_reg0.b = 0xA7;

        color_reg1.r = 0x55;
        color_reg1.g = 0x78;
        color_reg1.b = 0x27;
        color_reg1.a = 0xFF;
        break;
    case fpcNm_ITEM_SMELL_POH:
        color_reg0.r = 0xFF;
        color_reg0.g = 0xC8;
        color_reg0.b = 0xFF;

        color_reg1.r = 0x44;
        color_reg1.g = 0x1A;
        color_reg1.b = 0x6B;
        color_reg1.a = 0xFF;
        break;
    }

    GXTexObj texobj, fb_texobj;
    dKyr_set_btitex_common(&texobj, (ResTIMG*)tex[0], GX_TEXMAP1);

    ResTIMG* fb_timg = mDoGph_gInf_c::getFrameBufferTimg();
    dDlst_window_c* window = dComIfGp_getWindow(0);
    camera_class* window_cam = dComIfGp_getCamera(window->getCameraID());
    dKyr_set_btitex_common(&fb_texobj, fb_timg, GX_TEXMAP0);

    f32 scale = 0.49f;
    C_MTXLightPerspective(sp120, window_cam->fovy, window_cam->aspect, scale, -scale, 0.5f, 0.5f);
    cMtx_concat(sp120, j3dSys.getViewMtx(), spF0);

    rot += 2.0f;
    MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
    MTXConcat(camMtx, rotMtx, camMtx);

    GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadTexMtxImm(spF0, GX_TEXMTX0, GX_MTX3x4);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 8);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetTevColor(GX_TEVREG0, color_reg0);
    GXSetTevColor(GX_TEVREG1, color_reg1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_DISABLE);
    GXSetNumIndStages(0);

    for (int i = 0; i < 2000; i++) {
        EF_ODOUR_EFF* effect = &odour_packet->mOdourEff[i];
        camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
        cXyz pos[4];
        Vec sp64, sp58;
        cXyz sp4C;

        f32 size = effect->field_0x2c;
        if (effect->mStatus != 0 && effect->mStatus != 1 && effect->mStatus != 11) {
            sp4C = effect->mBasePos + effect->mPosition;

            f32 var_f31 = camera->lookat.eye.abs(sp4C);
            if (var_f31 < 250.0f) {
                if (var_f31 < 150.0f) {
                    var_f31 = 0.0f;
                } else {
                    var_f31 = (var_f31 - 150.0f) / 150.0f;
                    if (var_f31 < 0.0f) {
                        var_f31 = 0.0f;
                    } else if (var_f31 > 1.0f) {
                        var_f31 = 1.0f;
                    }
                }
            } else {
                var_f31 = 1.0f;
            }

            f32 temp_f29 = var_f31 * (effect->field_0x28 * (effect->field_0x24 * (envlight->senses_effect_strength * envlight->senses_effect_strength * envlight->senses_effect_strength)));

            if (effect->mStatus != 0) {
                if (!(temp_f29 <= 0.000001f)) {
                    color_reg0.a = 255.0f * temp_f29;
                    GXSetTevColor(GX_TEVREG0, color_reg0);

                    sp70 = sp4C;

                    sp64.x = -size;
                    sp64.y = size;
                    sp64.z = 0.0f;
                    cMtx_multVec(camMtx, &sp64, &sp58);
                    pos[0].x = sp70.x + sp58.x;
                    pos[0].y = sp70.y + sp58.y;
                    pos[0].z = sp70.z + sp58.z;

                    sp64.x = size;
                    sp64.y = size;
                    sp64.z = 0.0f;
                    cMtx_multVec(camMtx, &sp64, &sp58);
                    pos[1].x = sp70.x + sp58.x;
                    pos[1].y = sp70.y + sp58.y;
                    pos[1].z = sp70.z + sp58.z;

                    sp64.x = size;
                    sp64.y = -size;
                    sp64.z = 0.0f;
                    cMtx_multVec(camMtx, &sp64, &sp58);
                    pos[2].x = sp70.x + sp58.x;
                    pos[2].y = sp70.y + sp58.y;
                    pos[2].z = sp70.z + sp58.z;

                    sp64.x = -size;
                    sp64.y = -size;
                    sp64.z = 0.0f;
                    cMtx_multVec(camMtx, &sp64, &sp58);
                    pos[3].x = sp70.x + sp58.x;
                    pos[3].y = sp70.y + sp58.y;
                    pos[3].z = sp70.z + sp58.z;

                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                    GXTexCoord2s16(0, 0);
                    GXTexCoord2s16(0, 0);
                    GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                    GXTexCoord2s16(0xFF, 0);
                    GXTexCoord2s16(0xFF, 0);
                    GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                    GXTexCoord2s16(0xFF, 0xFF);
                    GXTexCoord2s16(0xFF, 0xFF);
                    GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                    GXTexCoord2s16(0, 0xFF);
                    GXTexCoord2s16(0, 0xFF);
                    GXEnd();
                }
            }
        }
    }

    GXSetClipMode(GX_CLIP_ENABLE);
    J3DShape::resetVcdVatCache();
}

void dKyr_mud_init() {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    envlight->mpMudPacket->mpMoyaRes = (u8*)dComIfG_getObjectRes("Always", 0x53);

    for (int i = 0; i < 100; i++) {
        envlight->mpMudPacket->mEffect[i].mStatus = 0;
    }

    envlight->mpMudPacket->mEffectNum = 0;

    if (!dComIfGs_isStageBossEnemy()) {
        envlight->mpMudPacket->field_0x1c3c = 1.0f;
    } else {
        envlight->mpMudPacket->field_0x1c3c = 0.0f;
    }
}

void dKyr_mud_move() {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_mud_Packet* mud_packet = g_env_light.mpMudPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz spA4;
    cXyz sp98;
    cXyz sp8C;
    cXyz sp80;
    cXyz wind_vecpow = dKyw_get_wind_vecpow();
    cXyz sp68;
    cXyz sp5C;

    dBgS_GndChk gndchk;
    f32 sp34 = 800.0f;
    f32 temp_f31 = 700.0f;
    f32 sp30 = 0.0f;
    f32 sp2C = 0.0f;

    if (g_env_light.field_0x1048 != 0 || (g_env_light.field_0x1048 == 0 && mud_packet->field_0x1c3c <= 0.0f)) {
        mud_packet->mEffectNum = (s16)g_env_light.field_0x1048;
    }

    if (g_env_light.field_0x1048 != 0) {
        if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 && g_env_light.wether_pat1 == 6) {
            dComIfGs_PolyDamageOff_Set(1);
            cLib_addCalc(&mud_packet->field_0x1c3c, 0.0f, 0.1f, 0.001f, 0.000001f);
        } else {
            cLib_addCalc(&mud_packet->field_0x1c3c, 1.0f, 0.2f, 0.05f, 0.01f);
        }
    } else {
        cLib_addCalc(&mud_packet->field_0x1c3c, 0.0f, 0.2f, 0.05f, 0.01f);
    }

    if (mud_packet->mEffectNum == 0) {
        return;
    }

    dKy_set_eyevect_calc2(camera, &spA4, sp34, sp30);
    f32 sp3C = dKyw_get_wind_pow();
    f32 sp38 = -99999.898f;

    u8 sp8;
    for (int i = mud_packet->mEffectNum - 1; i >= 0; i--) {
        sp8 = 0;

        switch (mud_packet->mEffect[i].mStatus) {
        case 0:
            mud_packet->mEffect[i].field_0x34 = 2.5f * (0.5f + cM_rndF(0.5f));
            mud_packet->mEffect[i].field_0x40 = 0;
            mud_packet->mEffect[i].field_0x42 = cM_rndFX(65536.0f);
            mud_packet->mEffect[i].mBasePos.x = spA4.x;
            mud_packet->mEffect[i].mBasePos.y = 0.0f;
            mud_packet->mEffect[i].mBasePos.z = spA4.z;
            mud_packet->mEffect[i].mPosition.x = cM_rndFX(temp_f31);
            mud_packet->mEffect[i].mPosition.y = 0.0f;
            mud_packet->mEffect[i].mPosition.z = cM_rndFX(temp_f31);
            mud_packet->mEffect[i].field_0x38 = 0.0f;
            mud_packet->mEffect[i].field_0x3c = 0.0f;
            mud_packet->mEffect[i].field_0x28 = cM_rndF(360.0f);
            mud_packet->mEffect[i].field_0x2c = cM_rndF(360.0f);
            mud_packet->mEffect[i].field_0x30 = cM_rndF(360.0f);

            mud_packet->mEffect[i].mStatus++;
            break;
        case 1:
        case 2:
        case 3:
            if (mud_packet->mEffect[i].mStatus == 1) {
                f32 sp24 = 4.0f;
                f32 sp28 = cM_fsin(mud_packet->mEffect[i].field_0x28);

                mud_packet->mEffect[i].mPosition.x += sp28 * mud_packet->mEffect[i].field_0x34;
                mud_packet->mEffect[i].mPosition.z += mud_packet->mEffect[i].field_0x34 * cM_fsin(mud_packet->mEffect[i].field_0x30);
                mud_packet->mEffect[i].field_0x28 += 0.03f;
                mud_packet->mEffect[i].field_0x2c += 0.02f;
                mud_packet->mEffect[i].field_0x30 += 0.01f;
            }

            cXyz sp50;
            sp68.x = mud_packet->mEffect[i].mBasePos.x + mud_packet->mEffect[i].mPosition.x;
            sp68.y = 0.0f;
            sp68.z = mud_packet->mEffect[i].mBasePos.z + mud_packet->mEffect[i].mPosition.z;

            sp50 = spA4;
            sp50.y = sp68.y;

            f32 sp20 = sp68.abs(sp50);
            if (sp20 > temp_f31) {
                sp8 = 1;
                if (mud_packet->mEffect[i].field_0x38 < 0.01f) {
                    mud_packet->mEffect[i].mBasePos = sp50;

                    if (sp68.abs(sp50) > temp_f31 + (0.1f * temp_f31)) {
                        mud_packet->mEffect[i].mPosition.x = cM_rndFX(temp_f31);
                        mud_packet->mEffect[i].mPosition.y = 0.0f;
                        mud_packet->mEffect[i].mPosition.z = cM_rndFX(temp_f31);
                    } else {
                        get_vectle_calc(&sp68, &sp50, &sp5C);
                        sp5C.x += cM_rndFX(0.2f);
                        sp5C.y = 0.0f;
                        sp5C.z += cM_rndFX(0.2f);

                        mud_packet->mEffect[i].mPosition.x = sp5C.x * temp_f31;
                        mud_packet->mEffect[i].mPosition.y = 0.0f;
                        mud_packet->mEffect[i].mPosition.z = sp5C.z * temp_f31;
                    }
                }
            }
        }

        f32 sp1C = 0.05f;
        f32 sp18 = 1.0f;
        f32 sp14 = 0.0f;

        mud_packet->mEffect[i].mBasePos.y = 0.0f;
        mud_packet->mEffect[i].mPosition.y = sp14 - (1.0f + (f32)i);

        f32 sp10 = 1.0f;
        if (sp8 != 0) {
            sp10 = 0.0f;
        }

        cLib_addCalc(&mud_packet->mEffect[i].field_0x38, sp10 * mud_packet->field_0x1c3c, 0.25f, sp1C, 0.000001f);

        sp68.x = mud_packet->mEffect[i].mBasePos.x + mud_packet->mEffect[i].mPosition.x;
        sp68.y = camera->lookat.eye.y;
        sp68.z = mud_packet->mEffect[i].mBasePos.z + mud_packet->mEffect[i].mPosition.z;

        f32 spC = sp68.abs(camera->lookat.eye);
        f32 var_f30 = spC / 400.0f;
        if (var_f30 > 1.0f) {
            var_f30 = 1.0f;
        }

        mud_packet->mEffect[i].field_0x3c = var_f30;
    }
}

void dKyr_mud_draw(Mtx drawMtx, u8** tex) {
    dKankyo_mud_Packet* mud_packet = g_env_light.mpMudPacket;
    dKankyo_sun_Packet* sun_packet = g_env_light.mpSunPacket;

    static f32 rot = 0.0f;

    Mtx camMtx;
    Mtx rotMtx;
    cXyz pos[4];
    cXyz sp70;
    Vec sp64;

    GXColorS10 sp50;
    GXColor color_reg0;
    GXColor color_reg1;

    camera_class* sp34 = (camera_class*)dComIfGp_getCamera(0);
    s16 spC = g_env_light.bg_amb_col[1].a;
    dKankyo_sunlenz_Packet* lenz_packet = g_env_light.mpSunLenzPacket;

    if (mud_packet->mEffectNum != 0) {
        j3dSys.reinitGX();
        f32 var_f31 = 255.0f;

        if (g_env_light.field_0x1300 != -1) {
            spC = g_env_light.field_0x1300;
        }
    
        sp50.r = 1.3f * g_env_light.bg_amb_col[1].r;
        sp50.g = 1.3f * g_env_light.bg_amb_col[1].g;
        sp50.b = 1.3f * g_env_light.bg_amb_col[1].b;

        if (sp50.r >= 0xFF) {
            sp50.r = 0xFF;
        }
        if (sp50.g >= 0xFF) {
            sp50.g = 0xFF;
        }
        if (sp50.b >= 0xFF) {
            sp50.b = 0xFF;
        }

        color_reg0.r = sp50.r;
        color_reg0.g = sp50.g;
        color_reg0.b = sp50.b;
        color_reg0.a = 0xFF;

        var_f31 = spC;
        var_f31 *= spC / 255.0f;
        var_f31 *= 0.25f + (0.75f * (g_env_light.field_0x1302 / 255.0f));

        if (lenz_packet != NULL && sun_packet->mVisibility > 0.000001f) {
            var_f31 = (u8)(var_f31 * (1.0f - (sun_packet->mVisibility * (1.0f - (lenz_packet->mDistFalloff * lenz_packet->mDistFalloff * lenz_packet->mDistFalloff)))));
        }

        color_reg1.r = 0.5f * g_env_light.bg_amb_col[1].r;
        color_reg1.g = 0.5f * g_env_light.bg_amb_col[1].g;
        color_reg1.b = 0.5f * g_env_light.bg_amb_col[1].b;
        color_reg1.a = 0xFF;

        if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0) {
            f32 temp_f29 = 1.0f - mud_packet->field_0x1c3c;

            color_reg0.r = 0x60;
            color_reg0.g = 0x41;
            color_reg0.b = 0x45;
            color_reg0.a = 0x64;

            var_f31 = 100.0f;

            color_reg1.r = 0xD1;
            color_reg1.g = 0xEF;
            color_reg1.b = 0;

            color_reg0.r = (u8)(color_reg0.r + (s16)((20.0f - color_reg0.r) * temp_f29));
            color_reg0.g = (u8)(color_reg0.g + (s16)((100.0f - color_reg0.g) * temp_f29));
            color_reg0.b = (u8)(color_reg0.b + (s16)((120.0f - color_reg0.b) * temp_f29));
            color_reg0.a = (u8)(color_reg0.a + (s16)(-(f32)color_reg0.b * temp_f29));

            var_f31 = color_reg0.a;

            color_reg1.r = (u8)(color_reg1.r + (s16)((20.0f - color_reg1.r) * temp_f29));
            color_reg1.g = (u8)(color_reg1.g + (s16)((100.0f - color_reg1.g) * temp_f29));
            color_reg1.b = (u8)(color_reg1.b + (s16)((120.0f - color_reg1.b) * temp_f29));
        }

        if (dComIfGd_getView() != NULL) {
            MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
        } else {
            return;
        }
    
        if (g_env_light.camera_water_in_status == 0) {
            for (int i = 0; i < 1; i++) {
                GXTexObj texobj;
                dKyr_set_btitex(&texobj, (ResTIMG*)tex[0]);

                GXSetNumChans(0);
                GXSetTevColor(GX_TEVREG0, color_reg0);
                GXSetTevColor(GX_TEVREG1, color_reg1);
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
                GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

                if (g_env_light.camera_water_in_status != 0) {
                    GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_ENABLE);
                } else {
                    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
                }

                GXSetClipMode(GX_CLIP_ENABLE);
                GXSetNumIndStages(0);
                dKr_cullVtx_Set();

                MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
                MTXConcat(camMtx, rotMtx, camMtx);
                GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);

                for (int j = 0; j < mud_packet->mEffectNum; j++) {
                    fopAc_ac_c* sp28 = dComIfGp_getPlayer(0);
                    cXyz sp58;
                    sp70.x = mud_packet->mEffect[j].mBasePos.x + mud_packet->mEffect[j].mPosition.x;
                    sp70.y = mud_packet->mEffect[j].mBasePos.y + mud_packet->mEffect[j].mPosition.y;
                    sp70.z = mud_packet->mEffect[j].mBasePos.z + mud_packet->mEffect[j].mPosition.z;

                    color_reg0.a = mud_packet->mEffect[j].field_0x38 * var_f31;

                    GXLoadTexObj(&texobj, GX_TEXMAP0);
                    GXSetTevColor(GX_TEVREG0, color_reg0);

                    f32 sp30 = 1.0f;

                    f32 temp_f26 = 45.0f * (12.0f * sp30 * mud_packet->mEffect[j].field_0x3c);
                    temp_f26 *= 0.25f;
                    f32 temp_f30 = temp_f26;

                    f32 temp_f28 = mud_packet->mEffect[j].field_0x3c * cM_fsin(mud_packet->mEffect[j].field_0x28 * (5.0f * mud_packet->mEffect[j].field_0x3c));
                    f32 temp_f27 = mud_packet->mEffect[j].field_0x3c * cM_fcos(mud_packet->mEffect[j].field_0x2c * (5.0f * mud_packet->mEffect[j].field_0x3c));

                    sp64.x = temp_f30 - temp_f27;
                    sp64.y = 0.0f;
                    sp64.z = temp_f30 - temp_f28;
                    pos[0].x = sp70.x + sp64.x;
                    pos[0].y = sp70.y + sp64.y;
                    pos[0].z = sp70.z + sp64.z;

                    sp64.x = -temp_f30 - temp_f27;
                    sp64.y = 0.0f;
                    sp64.z = temp_f30 - temp_f28;
                    pos[1].x = sp70.x + sp64.x;
                    pos[1].y = sp70.y + sp64.y;
                    pos[1].z = sp70.z + sp64.z;

                    sp64.x = -temp_f30 - temp_f27;
                    sp64.y = 0.0f;
                    sp64.z = -temp_f30 - temp_f28;
                    pos[2].x = sp70.x + sp64.x;
                    pos[2].y = sp70.y + sp64.y;
                    pos[2].z = sp70.z + sp64.z;

                    sp64.x = temp_f30 - temp_f27;
                    sp64.y = 0.0f;
                    sp64.z = -temp_f30 - temp_f28;
                    pos[3].x = sp70.x + sp64.x;
                    pos[3].y = sp70.y + sp64.y;
                    pos[3].z = sp70.z + sp64.z;

                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                    GXTexCoord2s16(0, 0);
                    GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                    GXTexCoord2s16(0xFF, 0);
                    GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                    GXTexCoord2s16(0xFF, 0xFF);
                    GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                    GXTexCoord2s16(0, 0xFF);
                    GXEnd();
                }
            }

            GXSetClipMode(GX_CLIP_ENABLE);
            J3DShape::resetVcdVatCache();
        }
    }
}

void dKyr_evil_init() {
    if (g_env_light.mpEvilPacket != NULL) {
        g_env_light.mpEvilPacket->mpMoyaRes = (u8*)dComIfG_getObjectRes("Always", 0x53);
        g_env_light.mpEvilPacket->mpKumoLightRes = (u8*)dComIfG_getStageRes("F_kumolight_01.bti");

        if (g_env_light.mpEvilPacket->mpKumoLightRes == NULL) {
            g_env_light.mpEvilPacket->mpKumoLightRes = (u8*)dComIfG_getObjectRes("Always", 0x53);
        }

        for (int i = 0; i < 2000; i++) {
            g_env_light.mpEvilPacket->mEffect[i].mStatus = 0;
        }

        g_env_light.mpEvilPacket->mEffectNum = 0;
    }
}

void dKyr_evil_move() {
    dKankyo_evil_Packet* evil_packet = g_env_light.mpEvilPacket;
    cXyz vec = dKyw_get_wind_vecpow();

    dBgS_GndChk gndchk;
    for (int i = 0; i < evil_packet->mEffectNum; i++) {}
}

static void dKyr_evil_draw2(Mtx drawMtx, u8** tex) {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_evil_Packet* evil_packet = envlight->mpEvilPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    static f32 rot = 0.0f;

    Mtx camMtx;
    Mtx rotMtx;
    cXyz spA0;
    f32 sp48 = fabsf(cM_ssin(g_Counter.mCounter0 * 40));
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (evil_packet != NULL) {
        j3dSys.reinitGX();
        if (dComIfGd_getView() != NULL) {
            MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
        } else {
            OS_REPORT("\nevil_draw return!!");
            return;
        }
    
        GXColor color_reg0;
        GXColor color_reg1;

        color_reg1.r = 255.0f * sp48;
        color_reg1.g = 120.0f * sp48;
        color_reg1.b = 0;
        color_reg1.a = 0xFF;
        
        color_reg0.r = 0xFF;
        color_reg0.g = 0x9F;
        color_reg0.b = 0x87;
        color_reg0.a = 0xFF;

        GXTexObj texobj;
        dKyr_set_btitex(&texobj, (ResTIMG*)tex[1]);

        rot += 0.7f;
        MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
        MTXConcat(camMtx, rotMtx, camMtx);

        GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetNumChans(0);
        GXSetTevColor(GX_TEVREG0, color_reg0);
        GXSetTevColor(GX_TEVREG1, color_reg1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZCompLoc(GX_TRUE);
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
        GXSetCullMode(GX_CULL_NONE);
        GXSetClipMode(GX_CLIP_DISABLE);
        GXSetNumIndStages(0);

        for (int i = 0; i < g_env_light.field_0x1054; i++) {
            EF_EVIL_EFF* effect = &evil_packet->mEffect[i];
            camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

            cXyz pos[4];
            Vec sp94, sp88;
            cXyz sp7C;
            f32 var_f31 = effect->field_0x38;

            if (effect->mStatus != 0 && (i & 1)) {
                f32 temp_f30 = 0.2f + (0.8f * fabsf(cM_ssin(effect->field_0x3c)));
                sp7C = effect->mBasePos + effect->mPosition;

                if ((strcmp(dComIfGp_getStartStageName(), "D_MN08") != 0 || dComIfGp_roomControl_getStayNo() != 1 || i < 1600 || !(camera->lookat.eye.x >= -5000.0f)) && !(var_f31 > 9000.0f)) {
                    if (dComIfGd_getView()->fovy > 40.0f) {
                        cXyz proj;
                        Vec sp34;
                        sp34.x = 80.0f;
                        sp34.y = 80.0f;
                        sp34.z = 80.0f;

                        mDoLib_project(&sp7C, &proj);

                        if (!(proj.x > -sp34.x) || !(proj.x < (FB_WIDTH + sp34.x)) ||
                            !(proj.y > -sp34.y) || !(proj.y < (458.0f + sp34.z)))
                        {
                            continue;
                        }
                    }

                    f32 sp40;
                    f32 sp3C = 150.0f;
                    f32 sp38 = 250.0f;

                    f32 var_f29 = camera->lookat.eye.abs(sp7C);
                    if (var_f29 < sp38) {
                        if (var_f29 < sp3C) {
                            var_f29 = 0.0f;
                        } else {
                            var_f29 = (var_f29 - sp3C) / (sp38 - sp3C);
                            if (var_f29 < 0.0f) {
                                var_f29 = 0.0f;
                            } else if (var_f29 > 1.0f) {
                                var_f29 = 1.0f;
                            }
                        }
                    } else {
                        var_f29 = 1.0f;
                    }

                    sp40 = effect->field_0x24 * var_f29;
                    var_f31 *= 0.5f + (0.5f * var_f29);
                    var_f31 *= 0.2f + (0.4f * temp_f30);
                    if (dComIfGp_roomControl_getStayNo() == 0) {
                        var_f31 = 100.0f;
                    }

                    if (!(sp40 <= 0.000001f)) {
                        color_reg0.a = 255.0f * sp40;

                        if (daPy_py_c::checkNowWolfPowerUp()) {
                            color_reg0.r = 80.0f * temp_f30;
                            color_reg0.g = 0;
                            color_reg0.b = 0;
                            color_reg0.a = 0x28;

                            color_reg1.r = 0x1E;
                            color_reg1.g = 0;
                            color_reg1.b = 0;
                            color_reg1.a = 0x28;
                        } else {
                            color_reg1.r = 255.0f * temp_f30;
                            color_reg1.g = 120.0f * (temp_f30 * temp_f30);
                            color_reg1.b = 20.0f * (temp_f30 * temp_f30 * temp_f30);

                            color_reg0.r = 255.0f * temp_f30;
                            color_reg0.g = 159.0f * (temp_f30 * temp_f30);
                            color_reg0.b = 135.0f * (temp_f30 * temp_f30);
                        }

                        color_reg0.a = 0xFF;
                        GXSetTevColor(GX_TEVREG0, color_reg0);
                        GXSetTevColor(GX_TEVREG1, color_reg1);

                        spA0 = sp7C;

                        sp94.x = -var_f31;
                        sp94.y = var_f31;
                        sp94.z = 0.0f;
                        cMtx_multVec(camMtx, &sp94, &sp88);
                        pos[0].x = spA0.x + sp88.x;
                        pos[0].y = spA0.y + sp88.y;
                        pos[0].z = spA0.z + sp88.z;

                        sp94.x = var_f31;
                        sp94.y = var_f31;
                        sp94.z = 0.0f;
                        cMtx_multVec(camMtx, &sp94, &sp88);
                        pos[1].x = spA0.x + sp88.x;
                        pos[1].y = spA0.y + sp88.y;
                        pos[1].z = spA0.z + sp88.z;

                        sp94.x = var_f31;
                        sp94.y = -var_f31;
                        sp94.z = 0.0f;
                        cMtx_multVec(camMtx, &sp94, &sp88);
                        pos[2].x = spA0.x + sp88.x;
                        pos[2].y = spA0.y + sp88.y;
                        pos[2].z = spA0.z + sp88.z;

                        sp94.x = -var_f31;
                        sp94.y = -var_f31;
                        sp94.z = 0.0f;
                        cMtx_multVec(camMtx, &sp94, &sp88);
                        pos[3].x = spA0.x + sp88.x;
                        pos[3].y = spA0.y + sp88.y;
                        pos[3].z = spA0.z + sp88.z;

                        for (int j = 0; j < 1; j++) {
                            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                            GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                            GXTexCoord2s16(0, 0);
                            GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                            GXTexCoord2s16(0xFF, 0);
                            GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                            GXTexCoord2s16(0xFF, 0xFF);
                            GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                            GXTexCoord2s16(0, 0xFF);
                            GXEnd();
                        }
                    }
                }
            }
        }

        GXSetClipMode(GX_CLIP_ENABLE);
        J3DShape::resetVcdVatCache();
    }
}

static f32 dKyr_near_bosslight_check(cXyz pos) {
    f32 nearest_dist = 10000000.0f;
    dScnKy_env_light_c* envlight = dKy_getEnvlight();

    for (int i = 0; i < 6; i++) {
        if (envlight->field_0x0c18[i].field_0x26 == 1) {
            if (envlight->field_0x0c18[i].mPos.abs(pos) < nearest_dist) {
                nearest_dist = envlight->field_0x0c18[i].mPos.abs(pos);
            }
        }
    }

    return nearest_dist;
}

void dKyr_evil_draw(Mtx drawMtx, u8** tex) {
    dScnKy_env_light_c* envlight = dKy_getEnvlight();
    dKankyo_evil_Packet* evil_packet = envlight->mpEvilPacket;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    static f32 rot = 0.0f;

    Mtx camMtx;
    Mtx rotMtx;
    cXyz spC8;

    GXColor color_reg0;
    GXColor color_reg1;
    f32 sp64 = fabsf(cM_ssin(g_Counter.mCounter0 * 40));
    f32 sp60 = fabsf(cM_ssin(g_Counter.mCounter0 * 215));

    if (evil_packet != NULL) {
        j3dSys.reinitGX();
        if (dComIfGd_getView() != NULL) {
            MTXInverse(dComIfGd_getView()->viewMtxNoTrans, camMtx);
        } else {
            OS_REPORT("\nevil_draw return!!");
            return;
        }

        color_reg0.r = 69.0f * sp64;
        color_reg0.g = 45.0f * sp64;
        color_reg0.b = 137.0f * sp64;
        color_reg0.a = 0;

        color_reg1.r = 10;
        color_reg1.g = 0;
        color_reg1.b = 10;
        color_reg1.a = 255;

        GXTexObj texobj;
        dKyr_set_btitex(&texobj, (ResTIMG*)tex[0]);

        rot += 1.0f;
        MTXRotRad(rotMtx, 'Z', DEG_TO_RAD(rot));
        MTXConcat(camMtx, rotMtx, camMtx);

        GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetNumChans(0);
        GXSetTevColor(GX_TEVREG0, color_reg0);
        GXSetTevColor(GX_TEVREG1, color_reg1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZCompLoc(GX_TRUE);
        GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
        GXSetCullMode(GX_CULL_NONE);
        GXSetClipMode(GX_CLIP_DISABLE);
        GXSetNumIndStages(0);

        dComIfG_Ccsp()->PrepareMass();

        for (int i = 0; i < g_env_light.field_0x1054; i++) {
            EF_EVIL_EFF* effect = &evil_packet->mEffect[i];
            camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

            cXyz pos[4];
            Vec spBC, spB0;
            cXyz spA4;

            f32 temp_f30 = effect->field_0x38;

            if (effect->mStatus != 0) {
                spA4 = effect->mBasePos + effect->mPosition;

                if (!(temp_f30 > 9000.0f)) {
                    if (dComIfGd_getView()->fovy > 40.0f) {
                        cXyz proj;
                        Vec sp44;
                        sp44.x = 80.0f;
                        sp44.y = 80.0f;
                        sp44.z = 80.0f;

                        mDoLib_project(&spA4, &proj);

                        if (!(proj.x > -sp44.x) || !(proj.x < (FB_WIDTH + sp44.x)) ||
                            !(proj.y > -sp44.y) || !(proj.y < (458.0f + sp44.z)))
                        {
                            continue;
                        }
                    }

                    f32 sp5C;
                    f32 sp50 = 50.0f;
                    f32 sp4C = 800.0f;

                    f32 var_f31 = camera->lookat.eye.abs(spA4);
                    if (var_f31 < sp4C) {
                        if (var_f31 < sp50) {
                            var_f31 = 0.0f;
                        } else {
                            var_f31 = (var_f31 - sp50) / (sp4C - sp50);
                            if (var_f31 < 0.0f) {
                                var_f31 = 0.0f;
                            } else if (var_f31 > 1.0f) {
                                var_f31 = 1.0f;
                            }
                            var_f31 = 1.0f - var_f31;
                            var_f31 *= var_f31;
                            var_f31 = 1.0f - var_f31;
                        }
                    } else {
                        var_f31 = 1.0f;
                    }

                    f32 sp54 = effect->field_0x24 * var_f31;
                    temp_f30 *= var_f31;
                    temp_f30 = 1.8f * temp_f30;

                    if (!(sp54 <= 0.000001f)) {
                        color_reg0.a = 255.0f * sp54;

                        f32 bosslight_dist = dKyr_near_bosslight_check(spA4);
                        if (bosslight_dist < 5000.0f) {
                            f32 sp34 = (1.0f - (bosslight_dist / 5000.0f));
                            sp5C = 0.1f + (0.9f * sp34 * fabsf(cM_ssin(g_Counter.mCounter0 * ((i & 7) + 601))));
                        } else {
                            sp5C = 0.0f;
                        }
                        cLib_addCalc(&effect->field_0x2c, sp5C, 0.5f, 0.1f, 0.01f);

                        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
                        if (player != NULL && player->getKandelaarFlamePos() != NULL) {
                            color_reg1.r = 120.0f * effect->field_0x2c;
                            color_reg1.g = 140.0f * effect->field_0x2c;
                            color_reg1.b = 165.0f * effect->field_0x2c;
                        } else {
                            color_reg1.r = 80.0f * effect->field_0x2c;
                            color_reg1.g = 130.0f * effect->field_0x2c;
                            color_reg1.b = 155.0f * effect->field_0x2c;
                        }

                        if (dComIfGp_roomControl_getStayNo() == 0 && effect->mBasePos.y < 0.0f) {
                            f32 sp48 = (1.0f - effect->mBasePos.y / -3000.0f);
                            if (sp48 > 1.0f) {
                                sp48 = 1.0f;
                            } else if (sp48 < 0.0f) {
                                sp48 = 0.0f;
                            }

                            color_reg0.a = 255.0f * sp48;
                        }

                        f32 sp2C = (f32)i / (f32)g_env_light.field_0x1054;
                        sp2C = (i & 15) / 15.0f;

                        if (daPy_py_c::checkNowWolfPowerUp()) {
                            color_reg0.r = (int)(127.0f * fabsf(sp2C - sp64)) + 0x80;
                            color_reg0.g = 0x80;
                            color_reg0.b = (int)(127.0f * fabsf(sp2C - sp64)) + 0x80;
                            color_reg0.a = 0x23;
                        } else {
                            f32 sp28 = 0.0f;
                            if (effect->mStatus == 2) {
                                sp28 = 1.0f - effect->field_0x24;
                            }

                            color_reg0.r = (50.0f * sp28) + (15.0f * fabsf(sp2C - sp64));
                            color_reg0.g = 100.0f * sp28;
                            color_reg0.b = (115.0f * sp28) + (15.0f * fabsf(sp2C - sp64));
                        }

                        GXSetTevColor(GX_TEVREG0, color_reg0);
                        GXSetTevColor(GX_TEVREG1, color_reg1);

                        spC8 = spA4;

                        spBC.x = -temp_f30;
                        spBC.y = temp_f30;
                        spBC.z = 0.0f;
                        cMtx_multVec(camMtx, &spBC, &spB0);
                        pos[0].x = spC8.x + spB0.x;
                        pos[0].y = spC8.y + spB0.y;
                        pos[0].z = spC8.z + spB0.z;

                        spBC.x = temp_f30;
                        spBC.y = temp_f30;
                        spBC.z = 0.0f;
                        cMtx_multVec(camMtx, &spBC, &spB0);
                        pos[1].x = spC8.x + spB0.x;
                        pos[1].y = spC8.y + spB0.y;
                        pos[1].z = spC8.z + spB0.z;

                        spBC.x = temp_f30;
                        spBC.y = -temp_f30;
                        spBC.z = 0.0f;
                        cMtx_multVec(camMtx, &spBC, &spB0);
                        pos[2].x = spC8.x + spB0.x;
                        pos[2].y = spC8.y + spB0.y;
                        pos[2].z = spC8.z + spB0.z;

                        spBC.x = -temp_f30;
                        spBC.y = -temp_f30;
                        spBC.z = 0.0f;
                        cMtx_multVec(camMtx, &spBC, &spB0);
                        pos[3].x = spC8.x + spB0.x;
                        pos[3].y = spC8.y + spB0.y;
                        pos[3].z = spC8.z + spB0.z;

                        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                        GXPosition3f32(pos[0].x, pos[0].y, pos[0].z);
                        GXTexCoord2s16(0, 0);
                        GXPosition3f32(pos[1].x, pos[1].y, pos[1].z);
                        GXTexCoord2s16(0xFF, 0);
                        GXPosition3f32(pos[2].x, pos[2].y, pos[2].z);
                        GXTexCoord2s16(0xFF, 0xFF);
                        GXPosition3f32(pos[3].x, pos[3].y, pos[3].z);
                        GXTexCoord2s16(0, 0xFF);
                        GXEnd();
                    }
                }
            }
        }

        J3DShape::resetVcdVatCache();
        GXSetClipMode(GX_CLIP_ENABLE);

        if (!daPy_py_c::checkNowWolfPowerUp()) {
            dKyr_evil_draw2(drawMtx, tex);
        }
    }
}
