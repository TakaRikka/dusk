// Translation Unit: d_a_npc_fish

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_fish.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"

static int daNpc_Fish_Draw(npc_fish_class* i_this) {
    return 1;
}

static int daNpc_Fish_Execute(npc_fish_class* i_this) {
    return 1;
}

static int daNpc_Fish_IsDelete(npc_fish_class* i_this) {
    return 1;
}

static int daNpc_Fish_Delete(npc_fish_class* i_this) {
    return 1;
}

static void fish_set(fopAc_ac_c* i_this, fish_data_s* data) {
    dScnKy_env_light_c* envLight = dKy_getEnvlight();
    cXyz pos;
    for (int i = 0; i < 100; i++, data++) {
        if (data->param < 0) {
            return;
        }
        u32 param = data->param;
        pos = data->pos;
        if ((param == 1u) && envLight->fishing_hole_season != 2) {
            param = 2;
        }
        param |= data->field_0x10 << 8;
        fopAcM_create(PROC_MG_FISH, param, &pos, fopAcM_GetRoomNo(i_this), NULL, NULL, -1);
    }
}

static fish_data_s fish_d127_S[49] = {
    {20, {0.f, 0.f, 0.f}, 0},         {22, {0.f, 0.f, 0.f}, 0},
    {21, {0.f, 0.f, 0.f}, 0},         {24, {0.f, 0.f, 0.f}, 0},
    {25, {0.f, 0.f, 0.f}, 0},         {26, {0.f, 0.f, 0.f}, 0},
    {27, {0.f, 0.f, 0.f}, 0},         {6, {6064.f, -80.0f, 1138.f}, 0},
    {6, {6163.f, -80.f, 911.f}, 0},   {6, {6254.f, -80.f, 769.f}, 0},
    {9, {6369.f, -80.f, 611.f}, 0},   {9, {6460.f, -80.f, 372.f}, 0},
    {9, {6316.f, -80.f, 152.f}, 0},   {8, {6051.f, -80.f, 166.f}, 0},
    {5, {5853.f, -80.f, 277.f}, 0},   {5, {5759.f, -80.f, 506.f}, 0},
    {5, {5522.f, -80.f, 533.f}, 0},   {6, {-3931.f, -80.f, 3020.f}, 0},
    {5, {-3381.f, -80.f, 3473.f}, 0}, {5, {-2999.f, -80.f, 3608.f}, 0},
    {6, {-2668.f, -80.f, 4088.f}, 0}, {5, {-2528.f, -80.f, 4649.f}, 0},
    {5, {-2363.f, -80.f, 4953.f}, 0}, {6, {-1881.f, -80.f, 5180.f}, 0},
    {5, {-1344.f, -80.f, 5162.f}, 0}, {5, {-760.f, -80.f, 5062.f}, 0},
    {6, {-55.f, -80.f, 5064.f}, 0},   {5, {697.f, -80.f, 4958.f}, 0},
    {5, {4694.f, -80.f, 1852.f}, 0},  {5, {4276.f, -80.f, 1496.f}, 0},
    {5, {4552.f, -80.f, 1848.f}, 0},  {6, {4979.f, -80.f, 367.f}, 0},
    {5, {1096.f, -80.f, 4773.f}, 0},  {6, {1681.f, -80.f, 4705.f}, 0},
    {8, {2498.f, -80.f, 4834.f}, 0},  {5, {4139.f, -80.f, 5178.f}, 0},
    {5, {4481.f, -80.f, 2090.f}, 0},  {6, {4105.f, -80.f, 1628.f}, 0},
    {5, {3615.f, -80.f, 1058.f}, 0},  {8, {3173.f, -80.f, -215.f}, 0},
    {6, {2376.f, -80.f, -684.f}, 0},  {5, {1911.f, -80.f, -927.f}, 0},
    {6, {1489.f, -80.f, -1940.f}, 0}, {9, {1569.f, -80.f, -2745.f}, 0},
    {8, {1954.f, -80.f, -3690.f}, 0}, {9, {3011.f, -80.f, -4520.f}, 0},
    {8, {-4600.f, -80.f, 3100.f}, 0}, {7, {3616.f, -80.f, -4739.f}, 0},
    {-1, {0.f, 0.f, 0.f}, 0},
};

static fish_data_s fish_d127[56] = {
    {0, {-4605.0f, -80.f, 3526.f}, 45},     {0, {-5300.0f, -80.0f, 2875.0f}, 46},
    {0, {-7163.0f, -80.0f, 4140.0f}, 47},   {0, {-8161.0f, -80.0f, 4235.0f}, 44},
    {0, {-9338.0f, -80.0f, 2573.0f}, 43},   {0, {-8949.0f, -80.0f, -1096.0f}, 50},
    {0, {-7153.0f, -80.0f, -979.0f}, 52},   {0, {-7406.0f, -80.0f, -881.0f}, 51},
    {0, {-6138.0f, -80.0f, -305.0f}, 45},   {0, {-6100.0f, -80.0f, -2880.0f}, 53},
    {0, {-5541.0f, -80.0f, -655.0f}, 51},   {0, {-5421.0f, -80.0f, -1326.0f}, 46},
    {0, {-3982.0f, -80.0f, -2826.0f}, 43},  {2, {-3567.0f, -80.0f, -4828.0f}, 64},
    {0, {-4286.0f, -80.0f, -6508.0f}, 47},  {1, {-4126.0f, -80.0f, -8243.0f}, 71},
    {0, {-2717.0f, -80.0f, -8005.0f}, 49},  {0, {-1084.0f, -80.0f, -7971.0f}, 52},
    {2, {-2134.0f, -80.0f, -8430.0f}, 63},  {2, {-1064.0f, -80.0f, -6086.0f}, 62},
    {2, {329.0f, -80.0f, -5500.0f}, 66},    {0, {1698.0f, -80.0f, -6166.0f}, 46},
    {2, {2534.0f, -80.0f, -4105.0f}, 65},   {0, {1836.0f, -80.0f, -3195.0f}, 46},
    {0, {1668.0f, -80.0f, -2061.0f}, 48},   {0, {2038.0f, -80.0f, -989.0f}, 45},
    {0, {3469.0f, -80.0f, -152.0f}, 49},    {0, {-885.0f, -80.0f, 1402.0f}, 43},
    {0, {-233.0f, -80.0f, 1504.0f}, 45},    {0, {564.0f, -80.0f, 1415.0f}, 44},
    {0, {-85.0f, -80.0f, 0.0f}, 46},        {2, {-500.0f, -80.0f, -200.0f}, 63},
    {0, {-950.0f, -80.0f, 450.0f}, 48},     {0, {-1898.0f, -80.0f, 470.0f}, 46},
    {0, {-1285.0f, -80.0f, -211.0f}, 46},   {2, {-1146.0f, -80.0f, -916.0f}, 59},
    {0, {-2612.0f, -80.0f, -1243.0f}, 47},  {0, {-2684.0f, -80.0f, -116.0f}, 46},
    {0, {-2479.0f, -80.0f, 5304.0f}, 39},   {0, {-1744.0f, -80.0f, 5509.0f}, 38},
    {0, {-859.0f, -80.0f, 5470.0f}, 41},    {0, {971.0f, -80.0f, 5421.0f}, 37},
    {0, {1877.0f, -80.0f, 4878.0f}, 42},    {0, {3784.0f, -80.0f, 5578.0f}, 39},
    {0, {5395.0f, -80.0f, 2247.0f}, 37},    {0, {4209.0f, -80.0f, 994.0f}, 41},
    {3, {-3129.0f, -80.0f, 1548.0f}, 61},   {3, {-4282.0f, -80.0f, -1381.0f}, 65},
    {3, {-1317.0f, -80.0f, 2515.0f}, 63},   {0, {-9391.0f, -637.0f, 386.0f}, 51},
    {0, {-9057.0f, -675.0f, -770.0f}, 52},  {0, {-5562.0f, -675.0f, -2115.0f}, 53},
    {0, {-3837.0f, -546.0f, -2871.0f}, 51}, {0, {-328.0f, -955.0f, 1600.0f}, 52},
    {0, {-2689.0f, -901.0f, -126.0f}, 53},  {-1, {0.0f, 0.0f, 0.0f}, 0}};


static fish_pos lf_pos[12] = {
    {10, -2602.f, 5000.f}, {10, -863.f, 5095.f},   {10, 566.f, 5015.f},   {10, 3594.f, 4732.f},
    {10, 6020.f, 426.f},   {10, 2825.f, 77.f},     {10, 3396.f, 5024.f},  {10, 1962.f, -5789.f},
    {16, -1500.f, 665.f},  {16, -5952.f, -2760.f}, {16, -7265.f, -700.f}, {16, -8929.f, -630.f},
};

static int daNpc_Fish_Create(fopAc_ac_c* i_this) {
    cXyz pos;
    fopAcM_ct(i_this, npc_fish_class);
    if (strcmp(dComIfGp_getStartStageName(), "F_SP127")) {
        return 5;
    } else {
        dScnKy_env_light_c* envLight = dKy_getEnvlight();
        s32 param = 0;
        if (envLight->fishing_hole_season == 2) {
            param = 160;
        } else if (envLight->fishing_hole_season == 3) {
            param = 64;
        }
        if (param != 0) {
            pos.set(-3382.0f, 0.0f, -7473.0f);
            fopAcM_create(PROC_OBJ_LP, 0xFF010E00 | param, &pos, fopAcM_GetRoomNo(i_this), NULL,
                          NULL, -1);
        }
    }
    if (g_env_light.fishing_hole_season == 2) {
        for (int i = 0; i < 4; i++) {
            pos.set(cM_rndFX(700.0f) + -2936.0f, 0.0f, cM_rndFX(700.0f) + -7865.0f);
            csXyz angle(0, 0, -1);
            fopAcM_create(PROC_FR, 0, &pos, fopAcM_GetRoomNo(i_this), &angle, NULL, -1);
        }
    }
    
        /* dSv_event_flag_c::F_0463 - Fishing Pond - Reserved for fishing */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[463])) {
        fish_set(i_this, fish_d127);
    } else {
        fish_set(i_this, fish_d127_S);
        for (int i = 0; i < 12; i++) {
            pos.set(lf_pos[i].pos_x, -180.0f, lf_pos[i].pos_z);
            fopAcM_create(PROC_NPC_LF, lf_pos[i].param, &pos, fopAcM_GetRoomNo(i_this), NULL, NULL,
                          -1);
        }
    }
    return 5;
}

static actor_method_class l_daNpc_Fish_Method = {
    (process_method_func)daNpc_Fish_Create,  (process_method_func)daNpc_Fish_Delete,
    (process_method_func)daNpc_Fish_Execute, (process_method_func)daNpc_Fish_IsDelete,
    (process_method_func)daNpc_Fish_Draw,
};

extern actor_process_profile_definition g_profile_NPC_FISH = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_NPC_FISH,
    &g_fpcLf_Method.base,
    sizeof(npc_fish_class),
    0,
    0,
    &g_fopAc_Method.base,
    719,
    &l_daNpc_Fish_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_6_e,
};
