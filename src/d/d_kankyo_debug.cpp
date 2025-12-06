#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_kankyo.h"
#include "d/d_kankyo_rain.h"
#include "d/d_debug_viewer.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"

static void add_update_proc(u8* mode, s16* param_1, s16* param_2, s16* param_3) {
    int value;

    if (*mode != 0) {
        switch (*mode) {
        case 1:
            value = 100;
            break;
        case 2:
            value = 10;
            break;
        case 3:
            value = 1;
            break;
        case 4:
            value = -1;
            break;
        case 5:
            value = -10;
            break;
        case 6:
            value = -100;
            break;
        }

        if (*mode <= 3) {
            if (*param_1 + value > 0xFF) {
                value = 0xFF - *param_1;
            }
            if (*param_2 + value > 0xFF) {
                value = 0xFF - *param_2;
            }
            if (*param_3 + value > 0xFF) {
                value = 0xFF - *param_3;
            }
        } else {
            if (*param_1 + value < 0) {
                value = -*param_1;
            }
            if (*param_2 + value < 0) {
                value = -*param_2;
            }
            if (*param_3 + value < 0) {
                value = -*param_3;
            }
        }

        *param_1 += (s16)value;
        *param_2 += (s16)value;
        *param_3 += (s16)value;
        *mode = 0;

        g_kankyoHIO.light.dKankyo_lightHIOInfoUpDateF();
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }
}

void add_update_proc2(u8* mode, u8* param_1, u8* param_2, u8* param_3) {
    s16 spC;
    s16 spA;
    s16 sp8;
    u8 update_mode;

    update_mode = *mode;
    sp8 = *param_1;
    spA = *param_2;
    spC = *param_3;
    add_update_proc(mode, &sp8, &spA, &spC);

    *param_1 = sp8;
    *param_2 = spA;
    *param_3 = spC;

    if (update_mode != 0) {
        g_kankyoHIO.light.dKankyo_lightHIOInfoUpDateF();
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }
}

void dKydb_color_HIO_update() {
    switch (g_kankyoHIO.light.m_HOSTIO_setting) {
    case 0:
        g_kankyoHIO.light.field_0x52 = 0;
        break;
    case 1:
        if (g_kankyoHIO.light.field_0x52 == 0) {
            g_kankyoHIO.light.dKankyo_lightHIOInfoUpDateF();
        }
        g_kankyoHIO.light.field_0x52 = 1;
        break;
    }

    add_update_proc(&g_kankyoHIO.light.field_0x60, &g_env_light.actor_amb_col.r, &g_env_light.actor_amb_col.g, &g_env_light.actor_amb_col.b);
    add_update_proc(&g_kankyoHIO.light.field_0x61, &g_env_light.bg_amb_col[0].r, &g_env_light.bg_amb_col[0].g, &g_env_light.bg_amb_col[0].b);
    add_update_proc(&g_kankyoHIO.light.field_0x62, &g_env_light.bg_amb_col[1].r, &g_env_light.bg_amb_col[1].g, &g_env_light.bg_amb_col[1].b);
    add_update_proc(&g_kankyoHIO.light.field_0x63, &g_env_light.bg_amb_col[2].r, &g_env_light.bg_amb_col[2].g, &g_env_light.bg_amb_col[2].b);
    add_update_proc(&g_kankyoHIO.light.field_0x64, &g_env_light.bg_amb_col[3].r, &g_env_light.bg_amb_col[3].g, &g_env_light.bg_amb_col[3].b);
    add_update_proc(&g_kankyoHIO.light.field_0x65, &g_env_light.fog_col.r, &g_env_light.fog_col.g, &g_env_light.fog_col.b);
    add_update_proc2(&g_kankyoHIO.light.field_0x66, &g_env_light.dungeonlight[0].mColor.r, &g_env_light.dungeonlight[0].mColor.g, &g_env_light.dungeonlight[0].mColor.b);
    add_update_proc2(&g_kankyoHIO.light.field_0x67, &g_env_light.dungeonlight[1].mColor.r, &g_env_light.dungeonlight[1].mColor.g, &g_env_light.dungeonlight[1].mColor.b);
    add_update_proc2(&g_kankyoHIO.light.field_0x68, &g_env_light.dungeonlight[2].mColor.r, &g_env_light.dungeonlight[2].mColor.g, &g_env_light.dungeonlight[2].mColor.b);
    add_update_proc2(&g_kankyoHIO.light.field_0x69, &g_env_light.dungeonlight[3].mColor.r, &g_env_light.dungeonlight[3].mColor.g, &g_env_light.dungeonlight[3].mColor.b);
    add_update_proc2(&g_kankyoHIO.light.field_0x6a, &g_env_light.dungeonlight[4].mColor.r, &g_env_light.dungeonlight[4].mColor.g, &g_env_light.dungeonlight[4].mColor.b);
    add_update_proc2(&g_kankyoHIO.light.field_0x6b, &g_env_light.dungeonlight[5].mColor.r, &g_env_light.dungeonlight[5].mColor.g, &g_env_light.dungeonlight[5].mColor.b);
}

void dKydb_vrbox_HIO_update() {
    switch (g_kankyoHIO.vrbox.m_VrboxSetting) {
    case 0:
        g_kankyoHIO.vrbox.field_0x5 = 0;
        break;
    case 1:
        if (g_kankyoHIO.vrbox.field_0x5 == 0) {
            g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
        }
        g_kankyoHIO.vrbox.field_0x5 = 1;
        break;
    }
}

void dKydb_HIO_kcolor_debug(u8* param_0, u8* param_1, u8* param_2, u8* param_3) {
    if (g_kankyoHIO.light.m_forcedPalette != 0) {
        *param_0 = g_kankyoHIO.light.m_forcedPalette - 1;
        *param_1 = g_kankyoHIO.light.m_forcedPalette - 1;
        *param_2 = g_kankyoHIO.light.m_forcedPalette - 1;
        *param_3 = g_kankyoHIO.light.m_forcedPalette - 1;
    }

    if (dComIfGp_getStagePaletteNumInfo() - 1 < *param_0) {
        JUT_WARN(244, "db color err 1:[%d]", *param_0);
        *param_0 = 0;
    }

    if (dComIfGp_getStagePaletteNumInfo() - 1 < *param_1) {
        JUT_WARN(260, "db color err 2:[%d]", *param_1);
        *param_1 = 0;
    }

    if (dComIfGp_getStagePaletteNumInfo() - 1 < *param_2) {
        JUT_WARN(274, "db color err 3[%d]", *param_2);
        *param_2 = 0;
    }

    if (dComIfGp_getStagePaletteNumInfo() - 1 < *param_3) {
        JUT_WARN(288, "db color err 4[%d]", *param_3);
        *param_3 = 0;
    }
}

void dKydb_HIO_vrbox_debug(u8* param_0, u8* param_1, u8* param_2, u8* param_3) {
    if (g_kankyoHIO.vrbox.field_0x7 != 0) {
        *param_0 = g_kankyoHIO.vrbox.field_0x7 - 1;
        *param_1 = g_kankyoHIO.vrbox.field_0x7 - 1;
        *param_2 = g_kankyoHIO.vrbox.field_0x7 - 1;
        *param_3 = g_kankyoHIO.vrbox.field_0x7 - 1;
    }

    if (dComIfGp_getStageVrboxcolNumInfo() - 1 < *param_0) {
        JUT_WARN(327, "db Vrbox err 1[%d]", *param_0);
        *param_0 = 0;
    }

    if (dComIfGp_getStageVrboxcolNumInfo() - 1 < *param_1) {
        JUT_WARN(342, "db Vrbox err 1[%d]", *param_1);
        *param_1 = 0;
    }

    if (dComIfGp_getStageVrboxcolNumInfo() - 1 < *param_2) {
        JUT_WARN(357, "db Vrbox err 1[%d]", *param_2);
        *param_2 = 0;
    }

    if (dComIfGp_getStageVrboxcolNumInfo() - 1 < *param_3) {
        JUT_WARN(372, "db Vrbox err 1[%d]", *param_2);
        *param_3 = 0;
    }
}

// NONMATCHING
void dKydb_HIO_debug_TVdsp(f32 param_0, f32 param_1, int param_2, int param_3, u16 param_4) {
    u16 sp30[] = {
        0x01,
        0x02,
        0x04,
        0x08,
        0x10,
        0x20,
        0x40,
        0x80,
    };

    if (g_kankyoHIO.light.m_displayTVColorSettings == TRUE) {
        int sp28 = 20;
        int sp24 = 100;
        int sp20 = 11;
        int sp1C = 15;

        dDbVw_Report(sp28, sp24, "ACTOR Amb R %03d", g_env_light.actor_amb_col.r);
        dDbVw_Report(sp28, sp24 + sp1C, "          G %03d", g_env_light.actor_amb_col.g);
        dDbVw_Report(sp28, sp24 + (sp1C * 2), "          B %03d", g_env_light.actor_amb_col.b);

        for (int i = 0; i < 4; i++) {
            dDbVw_Report(sp28, (i * ((sp1C * 4) - sp1C)) + (sp24 + ((sp1C * 4) - sp1C)), "BG%01d   Amb R %03d", i, g_env_light.bg_amb_col[i].r);
            dDbVw_Report(sp28, (i * ((sp1C * 4) - sp1C)) + (sp24 + (sp1C * 4)), "          G %03d", g_env_light.bg_amb_col[i].g);
            dDbVw_Report(sp28, (i * ((sp1C * 4) - sp1C)) + (sp24 + (sp1C * 5)), "          B %03d", g_env_light.bg_amb_col[i].b);
        }

        for (int i = 0; i < 6; i++) {
            if (param_4 & sp30[i + 2]) {
                dDbVw_Report(sp28 + (sp20 * 20), (i * sp1C) + (sp24 + ((sp1C * 8) - sp1C)), "LIGHT%01d   R %03d G %03d B %03d", i, g_env_light.dungeonlight[i].mColor.r, g_env_light.dungeonlight[i].mColor.g, g_env_light.dungeonlight[i].mColor.b);
            }
        }

        dDbVw_Report(sp28, sp24 + (sp1C * 14), "Fog       R %03d", g_env_light.fog_col.r);
        dDbVw_Report(sp28, sp24 + ((sp1C * 16) - sp1C), "          G %03d", g_env_light.fog_col.g);
        dDbVw_Report(sp28, sp24 + (sp1C * 16), "          B %03d", g_env_light.fog_col.b);
        dDbVw_Report(sp28, sp24 + (sp1C * 17), "     StartZ %06f", g_env_light.mFogNear);
        dDbVw_Report(sp28, sp24 + (sp1C * 18), "       EndZ %06f", g_env_light.mFogFar);

        dDbVw_Report(sp28, sp24 + (sp1C * 21), "Time  Change :");

        switch (param_2) {
        case 0:
            dDbVw_Report(sp28 + (sp20 * 16), sp24 + (sp1C * 21), "ASA");
            break;
        case 1:
            dDbVw_Report(sp28 + (sp20 * 16), sp24 + (sp1C * 21), "HIRU");
            break;
        case 2:
            dDbVw_Report(sp28 + (sp20 * 16), sp24 + (sp1C * 21), "YUU");
            break;
        case 3:
            dDbVw_Report(sp28 + (sp20 * 16), sp24 + (sp1C * 21), "YORU");
            break;
        }

        dDbVw_Report(sp28 + (sp20 * 21), sp24 + (sp1C * 21), "to");

        switch (param_3) {
        case 0:
            dDbVw_Report(sp28 + (sp20 * 24), sp24 + (sp1C * 21), "ASA");
            break;
        case 1:
            dDbVw_Report(sp28 + (sp20 * 24), sp24 + (sp1C * 21), "HIRU");
            break;
        case 2:
            dDbVw_Report(sp28 + (sp20 * 24), sp24 + (sp1C * 21), "YUU");
            break;
        case 3:
            dDbVw_Report(sp28 + (sp20 * 24), sp24 + (sp1C * 21), "YORU");
            break;
        }

        dDbVw_Report(sp28 + (sp20 * 29), sp24 + (sp1C * 21), "(%f%%)", param_0);

        if (param_1 < 0.0001f) {
            dDbVw_Report(sp28, sp24 + (sp1C * 20), "Event Change : NO CHANGE");
        } else {
            dDbVw_Report(sp28, sp24 + (sp1C * 20), "Event Change : CHANGE %03d%%", (int)(100.0f * param_1));
        }
    }

    if (g_kankyoHIO.vrbox.m_displayVrboxTVColorSettings == 1) {
        int var_r29 = 30;
        int var_r28 = 80;
        int sp10 = 11;
        int var_r27 = 15;

        if (g_kankyoHIO.light.m_displayTVColorSettings == 1) {
            var_r29 = 350;
        }

        dDbVw_Report(var_r29, var_r28 + (var_r27 * 2), "Sky    R %03d", g_env_light.vrbox_sky_col.r);
        dDbVw_Report(var_r29, var_r28 + ((var_r27 * 4) - var_r27), "       G %03d", g_env_light.vrbox_sky_col.g);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 4), "       B %03d", g_env_light.vrbox_sky_col.b);

        dDbVw_Report(var_r29, var_r28 + (var_r27 + (var_r27 * 4)), "CloudU R %03d", g_env_light.vrbox_kumo_top_col.r);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 6), "       G %03d", g_env_light.vrbox_kumo_top_col.g);
        dDbVw_Report(var_r29, var_r28 + ((var_r27 * 8) - var_r27), "       B %03d", g_env_light.vrbox_kumo_top_col.b);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 8), "Cloud  A %03d", g_env_light.vrbox_kumo_top_col.a);

        dDbVw_Report(var_r29, var_r28 + (var_r27 + (var_r27 * 8)), "CloudD R %03d", g_env_light.vrbox_kumo_bottom_col.r);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 10), "       G %03d", g_env_light.vrbox_kumo_bottom_col.g);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 11), "       B %03d", g_env_light.vrbox_kumo_bottom_col.b);

        dDbVw_Report(var_r29, var_r28 + (var_r27 * 12), "CloudDDR %03d", g_env_light.vrbox_kumo_shadow_col.r);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 13), "       G %03d", g_env_light.vrbox_kumo_shadow_col.g);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 14), "       B %03d", g_env_light.vrbox_kumo_shadow_col.b);

        dDbVw_Report(var_r29, var_r28 + ((var_r27 * 16) - var_r27), "kasumiFR %03d", g_env_light.vrbox_kasumi_outer_col.r);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 16), "       G %03d", g_env_light.vrbox_kasumi_outer_col.g);
        dDbVw_Report(var_r29, var_r28 + (var_r27 + (var_r27 * 16)), "       B %03d", g_env_light.vrbox_kasumi_outer_col.b);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 18), "       B %03d", g_env_light.vrbox_kasumi_outer_col.a);

        dDbVw_Report(var_r29, var_r28 + (var_r27 * 19), "kasumiBR %03d", g_env_light.vrbox_kasumi_inner_col.r);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 20), "       G %03d", g_env_light.vrbox_kasumi_inner_col.g);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 21), "       B %03d", g_env_light.vrbox_kasumi_inner_col.b);
        dDbVw_Report(var_r29, var_r28 + (var_r27 * 22), "       A %03d", g_env_light.vrbox_kasumi_inner_col.a);

        if (g_kankyoHIO.light.m_displayTVColorSettings != 1) {
            dDbVw_Report(var_r29, var_r28 + (var_r27 * 23), "Time  Change :");

            switch (param_2) {
            case 0:
                dDbVw_Report(var_r29 + (sp10 * 0x10), var_r28 + (var_r27 * 23), "ASA");
                break;
            case 1:
                dDbVw_Report(var_r29 + (sp10 * 0x10), var_r28 + (var_r27 * 23), "HIRU");
                break;
            case 2:
                dDbVw_Report(var_r29 + (sp10 * 0x10), var_r28 + (var_r27 * 23), "YUU");
                break;
            case 3:
                dDbVw_Report(var_r29 + (sp10 * 0x10), var_r28 + (var_r27 * 23), "YORU");
                break;
            }

            dDbVw_Report(var_r29 + (sp10 * 0x15), var_r28 + (var_r27 * 23), "to");

            switch (param_3) {
            case 0:
                dDbVw_Report(var_r29 + (sp10 * 0x18), var_r28 + (var_r27 * 23), "ASA");
                break;
            case 1:
                dDbVw_Report(var_r29 + (sp10 * 0x18), var_r28 + (var_r27 * 23), "HIRU");
                break;
            case 2:
                dDbVw_Report(var_r29 + (sp10 * 0x18), var_r28 + (var_r27 * 23), "YUU");
                break;
            case 3:
                dDbVw_Report(var_r29 + (sp10 * 0x18), var_r28 + (var_r27 * 23), "YORU");
                break;
            }

            dDbVw_Report(var_r29 + (sp10 * 0x1D), var_r28 + (var_r27 * 23), "(%f%%)", param_0);

            if (param_1 < 0.0001f) {
                dDbVw_Report(var_r29, var_r28 + (var_r27 * 22), "Event Change : NO CHANGE");
            } else {
                dDbVw_Report(var_r29, var_r28 + (var_r27 * 22), "Event Change : CHANGE %03d%%", (int)(100.0f * param_1));
            }
        }
    }

    if (g_kankyoHIO.vrbox.field_0xe >= 0) {
        switch (g_kankyoHIO.vrbox.field_0xe) {
        case 0:
            g_env_light.vrbox_sky_col.r = g_env_light.fog_col.r;
            g_env_light.vrbox_sky_col.g = g_env_light.fog_col.g;
            g_env_light.vrbox_sky_col.b = g_env_light.fog_col.b;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            add_update_proc((u8*)&g_kankyoHIO.vrbox.field_0xe, &g_env_light.vrbox_sky_col.r, &g_env_light.vrbox_sky_col.g, &g_env_light.vrbox_sky_col.b);
        }

        g_kankyoHIO.vrbox.field_0xe = -1;
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }

    if (g_kankyoHIO.vrbox.field_0xf >= 0) {
        switch (g_kankyoHIO.vrbox.field_0xf) {
        case 0:
            g_env_light.vrbox_kumo_top_col.r = g_env_light.fog_col.r;
            g_env_light.vrbox_kumo_top_col.g = g_env_light.fog_col.g;
            g_env_light.vrbox_kumo_top_col.b = g_env_light.fog_col.b;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            add_update_proc((u8*)&g_kankyoHIO.vrbox.field_0xf, &g_env_light.vrbox_kumo_top_col.r, &g_env_light.vrbox_kumo_top_col.g, &g_env_light.vrbox_kumo_top_col.b);
        }

        g_kankyoHIO.vrbox.field_0xf = -1;
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }

    if (g_kankyoHIO.vrbox.field_0x10 >= 0) {
        switch (g_kankyoHIO.vrbox.field_0x10) {
        case 0:
            g_env_light.vrbox_kumo_bottom_col.r = g_env_light.fog_col.r;
            g_env_light.vrbox_kumo_bottom_col.g = g_env_light.fog_col.g;
            g_env_light.vrbox_kumo_bottom_col.b = g_env_light.fog_col.b;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            add_update_proc((u8*)&g_kankyoHIO.vrbox.field_0x10, &g_env_light.vrbox_kumo_bottom_col.r, &g_env_light.vrbox_kumo_bottom_col.g, &g_env_light.vrbox_kumo_bottom_col.b);
        }

        g_kankyoHIO.vrbox.field_0x10 = -1;
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }

    if (g_kankyoHIO.vrbox.field_0x11 >= 0) {
        switch (g_kankyoHIO.vrbox.field_0x11) {
        case 0:
            g_env_light.vrbox_kumo_shadow_col.r = g_env_light.fog_col.r;
            g_env_light.vrbox_kumo_shadow_col.g = g_env_light.fog_col.g;
            g_env_light.vrbox_kumo_shadow_col.b = g_env_light.fog_col.b;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            add_update_proc((u8*)&g_kankyoHIO.vrbox.field_0x11, &g_env_light.vrbox_kumo_shadow_col.r, &g_env_light.vrbox_kumo_shadow_col.g, &g_env_light.vrbox_kumo_shadow_col.b);
        }

        g_kankyoHIO.vrbox.field_0x11 = -1;
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }

    if (g_kankyoHIO.vrbox.field_0x12 >= 0) {
        switch (g_kankyoHIO.vrbox.field_0x12) {
        case 0:
            g_env_light.vrbox_kasumi_outer_col.r = g_env_light.fog_col.r;
            g_env_light.vrbox_kasumi_outer_col.g = g_env_light.fog_col.g;
            g_env_light.vrbox_kasumi_outer_col.b = g_env_light.fog_col.b;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            add_update_proc((u8*)&g_kankyoHIO.vrbox.field_0x12, &g_env_light.vrbox_kasumi_outer_col.r, &g_env_light.vrbox_kasumi_outer_col.g, &g_env_light.vrbox_kasumi_outer_col.b);
        }

        g_kankyoHIO.vrbox.field_0x12 = -1;
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }

    if (g_kankyoHIO.vrbox.field_0x13 >= 0) {
        switch (g_kankyoHIO.vrbox.field_0x13) {
        case 0:
            g_env_light.vrbox_kasumi_inner_col.r = g_env_light.fog_col.r;
            g_env_light.vrbox_kasumi_inner_col.g = g_env_light.fog_col.g;
            g_env_light.vrbox_kasumi_inner_col.b = g_env_light.fog_col.b;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            add_update_proc((u8*)&g_kankyoHIO.vrbox.field_0x13, &g_env_light.vrbox_kasumi_inner_col.r, &g_env_light.vrbox_kasumi_inner_col.g, &g_env_light.vrbox_kasumi_inner_col.b);
        }

        g_kankyoHIO.vrbox.field_0x13 = -1;
        g_kankyoHIO.vrbox.dKankyo_vrboxHIOInfoUpDateF();
    }

    if (g_kankyoHIO.bloom.field_0x5 != 0) {
        dDbVw_Report(20, 100, "sikii   %d", mDoGph_gInf_c::getBloom()->getPoint());
        dDbVw_Report(20, 116, "b_haba  %d", mDoGph_gInf_c::getBloom()->getBlureSize());
        dDbVw_Report(20, 132, "b_kosa  %d", mDoGph_gInf_c::getBloom()->getBlureRatio());

        GXColor spC;
        spC = *mDoGph_gInf_c::getBloom()->getBlendColor();
        dDbVw_Report(20, 148, "kosa r  %d", spC.r);
        dDbVw_Report(20, 164, "     g  %d", spC.g);
        dDbVw_Report(20, 180, "     b  %d", spC.b);
        dDbVw_Report(20, 196, "moto A  %d", spC.a);

        spC = *mDoGph_gInf_c::getBloom()->getMonoColor();
        dDbVw_Report(20, 212, "mono r  %d", spC.r);
        dDbVw_Report(20, 228, "     g  %d", spC.g);
        dDbVw_Report(20, 244, "     b  %d", spC.b);
        dDbVw_Report(20, 260, "     A  %d", spC.a);
    }
}

void dKydb_HIO_debug_Wind() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = dComIfGp_getCamera(0);

    f32 substick_x = mDoCPd_c::getSubStickX(PAD_3);
    f32 substick_y = mDoCPd_c::getSubStickY(PAD_3);
    f32 stick_x = mDoCPd_c::getStickX3D(PAD_3);
    f32 stick_y = mDoCPd_c::getStickY(PAD_3);
    cXyz wind_vec;

    g_env_light.global_wind_influence.pow += 0.01f * substick_y;
    if (g_env_light.global_wind_influence.pow > 1.0f) {
        g_env_light.global_wind_influence.pow = 1.0f;
    } else if (g_env_light.global_wind_influence.pow < 0.0f) {
        g_env_light.global_wind_influence.pow = 0.0f;
    }

    g_kankyoHIO.light.field_0x5e -= 400.0f * stick_x;
    g_kankyoHIO.light.field_0x5c += 400.0f * stick_y;

    s16 temp_r31 = g_kankyoHIO.light.field_0x5c;
    s16 temp_r30 = g_kankyoHIO.light.field_0x5e;
    wind_vec.x = cM_scos(temp_r31) * cM_scos(temp_r30);
    wind_vec.y = cM_ssin(temp_r31);
    wind_vec.z = cM_scos(temp_r31) * cM_ssin(temp_r30);

    g_env_light.global_wind_influence.vec = wind_vec;

    if (mDoCPd_c::getHoldA(PAD_3)) {
        JUTReport(40, 200, "MAIN STICK    : POSITION MOVE LR-Yangle UD-Xangle");
        JUTReport(40, 240, "SUB  STICK UD : POWER");
        JUTReport(40, 280, "B BUTTON      : TEST DISP OFF");

        dKy_actor_addcol_amb_set(-0xFF, -0xFF, -0xFF, 1.0f);
        dKy_bg_addcol_amb_set(-0xFF, -0xFF, -0xFF, 1.0f);
    } else {
        dKy_actor_addcol_amb_set(-0xFF, -0xFF, -0xFF, 0.0f);
        dKy_bg_addcol_amb_set(-0xFF, -0xFF, -0xFF, 0.0f);
    }
}

void dKydb_HIO_debug_draw() {
    csXyz rot;
    cXyz pos;
    cXyz size;

    camera_class* camera = dComIfGp_getCamera(0);
    dKy_set_eyevect_calc2(camera, &pos, 200.0f, 200.0f);

    s16 var_r30 = g_kankyoHIO.light.field_0x5c;
    s16 var_r29 = g_kankyoHIO.light.field_0x5e;

    size.x = 2.0f;
    size.y = 2.0f;
    size.z = 150.0f;

    rot.x = -var_r30;
    rot.y = -var_r29 + 0x4000;
    rot.z = 0;

    static const GXColor c2 = {0xFF, 0xFF, 0x00, 0x40};
    dDbVw_drawCubeXlu(pos, size, rot, c2);
}

// NONMATCHING - stack register
void dKydb_HIO_winddebug_draw() {
    cXyz size;
    csXyz rot;
    camera_class* temp_r31 = dComIfGp_getCamera(0);
    cXyz pos;

    if (!mDoCPd_c::getHoldB(PAD_3)) {
        JUTReport(120, 400, "power=[ %f ]", g_env_light.global_wind_influence.pow);
        JUTReport(100, 420, "HELP - A BUTTON    TESTDISP OFF - B BUTTON");
        dKy_set_eyevect_calc2(temp_r31, &pos, 500.0f, 500.0f);

        s16 var_r30 = g_kankyoHIO.light.field_0x5c;
        s16 var_r29 = g_kankyoHIO.light.field_0x5e;

        rot.x = -var_r30;
        rot.y = -var_r29 + 0x4000;
        rot.z = 0;

        size.x = 20.0f * g_env_light.global_wind_influence.pow;
        size.y = 10.0f * g_env_light.global_wind_influence.pow;
        size.z = 1000.0f * g_env_light.global_wind_influence.pow;

        static const GXColor c2 = {0x00, 0x00, 0xFF, 0x50};
        dDbVw_drawCubeXlu(pos, size, rot, c2);
    }
}

// NONMATCHING - stack register
void dKydb_winddisp_draw() {
    cXyz size;
    csXyz rot;
    cXyz pos;

    camera_class* camera = dComIfGp_getCamera(0);
    dKy_set_eyevect_calc2(camera, &pos, 200.0f, 200.0f);

    if (g_kankyoHIO.wind.display_wind_dir == TRUE) {
        f32 temp_f31 = g_env_light.global_wind_influence.vec.x;
        f32 temp_f29 = g_env_light.global_wind_influence.vec.y;
        f32 temp_f30 = g_env_light.global_wind_influence.vec.z;

        f32 temp_f28 = JMAFastSqrt(SQUARE(temp_f31) + SQUARE(temp_f30));
        s16 temp_r30 = cM_atan2s(temp_f31, temp_f30);
        s16 var_r31 = cM_atan2s(temp_f28, temp_f29);

        rot.x = 16384.0f + var_r31;
        rot.y = (s16)temp_r30;
        rot.z = 0;

        size.x = 20.0f * g_env_light.global_wind_influence.pow;
        size.y = 10.0f * g_env_light.global_wind_influence.pow;
        size.z = 1000.0f * g_env_light.global_wind_influence.pow;

        static const GXColor c2 = {0xFF, 0x80, 0x80, 0x80};
        dDbVw_drawCubeXlu(pos, size, rot, c2);

        dDbVw_Report(10, 410, "WIND x[%f]y[%f]z[%f]pow[%f]",
            g_env_light.global_wind_influence.vec.x, g_env_light.global_wind_influence.vec.y, g_env_light.global_wind_influence.vec.z, g_env_light.global_wind_influence.pow);
    }
}

void dKydb_timedisp() {
    if (g_kankyoHIO.navy.display_save_location) {
        dDbVw_Report(20, 16, "ALWAYS SavMem STAGE[%s] Room[%d] Lp[%d]",
            g_dComIfG_gameInfo.info.getPlayer().getPlayerReturnPlace().getName(),
            g_dComIfG_gameInfo.info.getPlayer().getPlayerReturnPlace().getRoomNo(),
            g_dComIfG_gameInfo.info.getPlayer().getPlayerReturnPlace().getPlayerStatus());
    }

    f32 time = g_env_light.getDaytime();
    f32 hour = time / 15.0f;

    f32 var_f30 = ((int)(time * 1000000.0f) % 15000000) / 1000000.0f;
    f32 min = var_f30 / 15.0f * 60.0f;

    var_f30 = ((int)(time * 1000000.0f) % 250000) / 1000000.0f;
    f32 sec = var_f30 / 0.25f * 60.0f;

    dDbVw_Report(0x1E, 0x41, "TIME %02d:%02d:%02d", (int)hour, (int)min, (int)sec);

    if (strcmp(dComIfGp_getStartStageName(), "F_SP116") == 0) {
        time = g_env_light.getDarkDaytime();
        hour = time / 15.0f;

        var_f30 = ((int)(time * 1000000.0f) % 15000000) / 1000000.0f;
        min = var_f30 / 15.0f * 60.0f;

        var_f30 = ((int)(time * 1000000.0f) % 250000) / 1000000.0f;
        sec = var_f30 / 0.25f * 60.0f;

        dDbVw_Report(0x1E, 0x54, "S_TM %02d:%02d:%02d", (int)hour, (int)min, (int)sec);
    }

    static const char* weekString[] = {
        "Sun",
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sut",
    };

    dDbVw_Report(0xDC, 0x41, "%s", weekString[g_env_light.mDate % 7]);
    dDbVw_Report(0x12C, 0x2D, "%s", dComIfGp_getStartStageName());
    dDbVw_Report(0x12C, 0x41, "Rm %d Lr %d Tw %d", dComIfGp_roomControl_getStayNo(), dComIfG_play_c::getLayerNo(0), dComIfGp_getStartStageDarkArea());
  
    if (dComIfGs_Grass_hide_Check()) {
        dDbVw_Report(0x12C, 0x55, "orz");
    }

    if (g_env_light.light_mask_type != 0) {
        dDbVw_Report(0x12C, 0x1C, "LM[%d]", g_env_light.light_mask_type);
    }

    if (!dKy_camera_water_in_status_check()) {
        switch (g_env_light.light_size) {
        case 0:
            dDbVw_Report(0x190, 0x1C, "LS[S]");
            break;
        case 1:
            dDbVw_Report(0x190, 0x1C, "LS[M]");
            break;
        case 2:
            dDbVw_Report(0x190, 0x1C, "LS[L]");
            break;
        case 3:
            dDbVw_Report(0x190, 0x1C, "LS[LL]");
            break;
        }
    }

#ifdef DEBUG
    if (g_presetHIO.field_0x2716 != 0) {
        dDbVw_Report(0x1E, 0x55, "JYOKYO FILE YOMIKOMI SIPPAI!");
    }
#endif

    int var_r31 = 0x55;
    if ((g_kankyoHIO.navy.field_0x22a & 1)) {
        dDbVw_Report(0x1E, var_r31, "BMD Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 2)) {
        dDbVw_Report(0x1E, var_r31, "BMDM Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 4)) {
        dDbVw_Report(0x1E, var_r31, "BMDC Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 8)) {
        dDbVw_Report(0x1E, var_r31, "BMDS Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x10)) {
        dDbVw_Report(0x1E, var_r31, "BSMD Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x20)) {
        dDbVw_Report(0x1E, var_r31, "BDL  Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x40)) {
        dDbVw_Report(0x1E, var_r31, "BDLL Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x80)) {
        dDbVw_Report(0x1E, var_r31, "BDLM Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x100)) {
        dDbVw_Report(0x1E, var_r31, "BMDP Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x200)) {
        dDbVw_Report(0x1E, var_r31, "BDLI Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x400)) {
        dDbVw_Report(0x1E, var_r31, "BDLC Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x800)) {
        dDbVw_Report(0x1E, var_r31, "BMDA Hakken!! Mada Irimasuka-?");
        var_r31 += 0x14;
    }

    if ((g_kankyoHIO.navy.field_0x22a & 0x1000)) {
        // unused    
    }
}

void dKydb_efplight_monitor() {
    int i;
    for (i = 0; i < 5; i++) {
        if (g_env_light.efplight[i] != NULL) {
            OSReport("\n[%d]pos[%f][%f][%f]", i, g_env_light.efplight[i]->mPosition.x, g_env_light.efplight[i]->mPosition.y, g_env_light.efplight[i]->mPosition.z);
        }
    }
}

void dKydb_plight_monitor() {
    OSReport("\nNow PLIGHT\n");

    int i;
    for (i = 0; i < 100; i++) {
        if (g_env_light.pointlight[i] != NULL) {
            OSReport("[%d]", i);
            if (g_kankyoHIO.point_light_monitor == 2) {
                OSReport("pos x[%f]y[%f]z[%f]pow[%f]\n", g_env_light.pointlight[i]->mPosition.x, g_env_light.pointlight[i]->mPosition.y, g_env_light.pointlight[i]->mPosition.z, g_env_light.pointlight[i]->mPow);
            }
        }
    }
}

void dKydb_dungeonlight_draw() {
    cXyz proj;
    camera_class* camera = dComIfGp_getCamera(0);
    int i;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (g_kankyoHIO.dungeonLight.displayDebugSphere == TRUE) {
        for (i = 0; i < g_kankyoHIO.dungeonLight.usedLights; i++) {
            cXyz size;
            csXyz rot;
            cXyz sp34;
            cXyz pos;
            cXyz sp1C;

            f32 var_f31 = g_env_light.dungeonlight[i].mAngleX / RAD_TO_DEG(1);
            f32 var_f30 = -(g_env_light.dungeonlight[i].mAngleY / RAD_TO_DEG(1));

            rot.x = 65536.0f * (var_f31 / (M_PI * 2));
            rot.y = (65536.0f * (var_f30 / (M_PI * 2))) - 16384.0f;
            rot.z = 0;

            size.x = 30.0f;
            size.y = 20.0f;
            size.z = 50.0f;

            GXColor color;
            color = g_env_light.dungeonlight[i].mColor;
            color.a = 0xC8;

            if (g_env_light.dungeonlight[i].mAngleAttenuation == 0) {
                dDbVw_drawSphereXlu(g_env_light.dungeonlight[i].mPosition, 60.0f, color, 1);
            } else {
                color.r = 0xFF;
                color.g = 0xFF;
                color.b = 0xFF;
                color.a = 0xFF;
                dDbVw_drawCubeXlu(g_env_light.dungeonlight[i].mPosition, size, rot, color);

                pos.x = -(cM_scos(rot.x) * cM_ssin(rot.y));
                pos.y = cM_ssin(rot.x);
                pos.z = -(cM_scos(rot.x) * cM_scos(rot.y));

                pos.x = (8000.0f * pos.x) + g_env_light.dungeonlight[i].mPosition.x;
                pos.y = (8000.0f * pos.y) + g_env_light.dungeonlight[i].mPosition.y;
                pos.z = (8000.0f * pos.z) + g_env_light.dungeonlight[i].mPosition.z;

                size.x = 3.0f;
                size.y = 3.0f;
                size.z = 8000.0f;
                dDbVw_drawCubeXlu(pos, size, rot, color);

                color.r = 0xFF;
                color.g = 0;
                color.b = 0xFF;
                color.a = 0xFF;

                size.x = 280.0f;
                size.y = 2.0f;
                size.z = 2.0f;
                dDbVw_drawCubeXlu(g_env_light.dungeonlight[i].mPosition, size, rot, color);
            }

            color.r = 0;
            color.g = 0xFF;
            color.b = 0;
            color.a = 0xFF;

            size.x = 2.0f;
            size.y = 2.0f;
            size.z = 280.0f;

            rot.x = 0x4000;
            rot.y = 0;
            rot.z = 0;
            dDbVw_drawCubeXlu(g_env_light.dungeonlight[i].mPosition, size, rot, color);

            color.r = 0xFF;
            color.g = 0;
            color.b = 0;
            color.a = 0xFF;

            size.x = 1.0f;
            size.y = 1.0f;
            size.z = 140.0f;

            rot.x = 0;
            rot.y = 0x4000;
            dDbVw_drawCubeXlu(g_env_light.dungeonlight[i].mPosition, size, rot, color);

            color.r = 0;
            color.g = 0;
            color.b = 0xFF;
            color.a = 0xFF;

            rot.x = 0;
            rot.y = 0;
            dDbVw_drawCubeXlu(g_env_light.dungeonlight[i].mPosition, size, rot, color);

            color.r = 0;
            color.g = 0xFF;
            color.b = 0;
            color.a = 0xFF;

            size.x = 1.0f;
            size.y = 1.0f;
            size.z = 280.0f;

            rot.x = 0x4000;
            rot.y = 0;
            rot.z = 0;
            dDbVw_drawCubeXlu(player->current.pos, size, rot, color);

            color.r = 0xFF;
            color.g = 0;
            color.b = 0;
            color.a = 0xFF;

            rot.x = 0;
            rot.y = 0x4000;
            dDbVw_drawCubeXlu(player->current.pos, size, rot, color);

            color.r = 0;
            color.g = 0;
            color.b = 0xFF;
            color.a = 0xFF;

            rot.x = 0;
            rot.y = 0;
            dDbVw_drawCubeXlu(player->current.pos, size, rot, color);

            mDoLib_project(&g_env_light.dungeonlight[i].mPosition, &proj);
            if (proj.x > 30.0f) {
                proj.x -= 30.0f;
            }

            if (proj.x < 0.0f || proj.x > FB_WIDTH || proj.y < 0.0f || proj.y > FB_HEIGHT) {
                if (proj.x < 0.0f) {
                    proj.x = 10.0f;
                }
                if (proj.x > FB_WIDTH) {
                    proj.x = 560.0f;
                }
                if (proj.y < 0.0f) {
                    proj.y = 40.0f;
                }
                if (proj.y > FB_HEIGHT) {
                    proj.y = 404.0f;
                }

                dDbVw_Report((int)proj.x, (int)proj.y, "OUT %d", i + 2);
            } else {
                dDbVw_Report((int)proj.x, (int)proj.y, "LIGHT %d", i + 2);
            }
        }
    }

    for (i = 0; i < 6; i++) {
        if (g_kankyoHIO.dungeonLight.field_0x8 != 0) {
            if ((g_kankyoHIO.dungeonLight.field_0x8 - 1) == i) {
                g_env_light.dungeonlight[i].mPosition = player->current.pos;
            } else if (g_kankyoHIO.dungeonLight.field_0x8 == i + 160) {
                g_env_light.dungeonlight[i].mPosition = camera->lookat.center;
            }
        }
    }

    if (g_kankyoHIO.dungeonLight.field_0x8 >= 10) {
        fopAc_ac_c* sp8 = dComIfGp_getPlayer(0);
        if (g_kankyoHIO.dungeonLight.usedLights > g_kankyoHIO.dungeonLight.field_0x8 - 10) {
            sp8->current.pos = g_env_light.dungeonlight[g_kankyoHIO.dungeonLight.field_0x8 - 10].mPosition;
        }
    }

    if (g_kankyoHIO.dungeonLight.field_0x8 != 0) {
        g_kankyoHIO.light.dKankyo_lightHIOInfoUpDateF();
        g_kankyoHIO.dungeonLight.field_0x8 = 0;
    }
}
