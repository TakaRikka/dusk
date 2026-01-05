/**
 * d_a_kytag10.cpp
 * Sparks Particle Emitter on a Path
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag10.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "d/d_kankyo_rain.h"
#include "f_op/f_op_camera_mng.h"

static cXyz get_rail_ratio_pos(dPath* i_path, int i_pointIdx, f32 param_2, s16* param_3,
                               s16* param_4) {
    cXyz point_a;
    cXyz point_b;
    cXyz ret;
    cXyz calc_vec;

    point_a.x = i_path->m_points[i_pointIdx].m_position.x;
    point_a.y = i_path->m_points[i_pointIdx].m_position.y;
    point_a.z = i_path->m_points[i_pointIdx].m_position.z;

    point_b.x = i_path->m_points[i_pointIdx + 1].m_position.x;
    point_b.y = i_path->m_points[i_pointIdx + 1].m_position.y;
    point_b.z = i_path->m_points[i_pointIdx + 1].m_position.z;

    ret.x = point_a.x + param_2 * (point_b.x - point_a.x);
    ret.y = point_a.y + param_2 * (point_b.y - point_a.y);
    ret.z = point_a.z + param_2 * (point_b.z - point_a.z);

    dKyr_get_vectle_calc(&point_a, &point_b, &calc_vec);

    *param_3 =
        cM_atan2s(JMAFastSqrt(calc_vec.x * calc_vec.x + calc_vec.z * calc_vec.z), calc_vec.y);
    *param_4 = cM_atan2s(calc_vec.x, calc_vec.z);

    return ret;
}

static dPath* set_path_info(fopAc_ac_c* i_actor) {
    dPath* path_p = NULL;
    u8 path_id = fopAcM_GetParam(i_actor) >> 0x10;

    if (path_id != 0xFF) {
        path_p = dPath_GetRoomPath(path_id, fopAcM_GetRoomNo(i_actor));
    }

    return path_p;
}

static dPath* get_Extent_pos_start_get(kytag10_class* i_this, dPath* i_path, cXyz*, f32,
                                       int* param_4) {
    *param_4 = 0;
    return i_path;
}

static dPath* get_Extent_pos_end_get(kytag10_class* i_this, dPath* i_path, cXyz*, f32,
                                     int* param_4) {
    *param_4 = i_path->m_num - 1;
    return i_path;
}

static void sparks_move(kytag10_class* i_this) {
    camera_class* camera_p = dComIfGp_getCamera(0);

    cXyz ratio_pos_1;
    cXyz ratio_pos_2;
    cXyz ratio_pos_5;
    cXyz ratio_pos_3;
    cXyz ratio_pos_4;

    dPath* path_p = i_this->mpPath;

    if (path_p != NULL) {
        int start_point_idx;
        dPath* path_start = get_Extent_pos_start_get(i_this, path_p, &camera_p->lookat.eye,
                                                     1000.0f, &start_point_idx);

        int end_point_idx;
        dPath* path_end = get_Extent_pos_end_get(i_this, path_p, &camera_p->lookat.eye, 1000.0f,
                                                 &end_point_idx);

        s16 spD6;
        s16 spD8;
        ratio_pos_1 = get_rail_ratio_pos(path_start, 0, 0.0f, &spD6, &spD8);
        ratio_pos_2 = get_rail_ratio_pos(path_end, end_point_idx - 1, 1.0f, &spD6, &spD8);
        ratio_pos_3 = get_rail_ratio_pos(path_start, i_this->mPathPoint, 0.0f, &spD6, &spD8);
        ratio_pos_4 = get_rail_ratio_pos(path_start, i_this->mPathPoint, 1.0f, &spD6, &spD8);

        f32 temp_f = 250.0f / ratio_pos_3.abs(ratio_pos_4);

        ratio_pos_5 =
            get_rail_ratio_pos(path_start, i_this->mPathPoint, i_this->field_0x594, &spD6, &spD8);
        i_this->field_0x588 = ratio_pos_5;

        i_this->mpEmitter1->setGlobalTranslation(ratio_pos_5.x, ratio_pos_5.y, ratio_pos_5.z);
        i_this->mpEmitter2->setGlobalTranslation(ratio_pos_5.x, ratio_pos_5.y, ratio_pos_5.z);

        i_this->mpEmitter1->setVolumeSize(i_this->mEmitterVolSize);
        i_this->mpEmitter1->setLifeTime(i_this->mEmitterLifeTime);

        i_this->mpEmitter2->setVolumeSize(i_this->mEmitterVolSize);
        i_this->mpEmitter2->setLifeTime(i_this->mEmitterLifeTime);

        i_this->mpEmitter1->setGlobalParticleScale(i_this->mEmitterPtclScale);
        i_this->mpEmitter2->setGlobalParticleScale(i_this->mEmitterPtclScale);

        if (i_this->field_0x584 != 0xFF) {
            f32 rate = i_this->field_0x584 / 100.0f;
            i_this->mpEmitter1->setRate(rate);
            i_this->mpEmitter2->setRate(rate);
        }

        if (i_this->field_0x594 <= 1.0f - temp_f * 1.5f) {
            i_this->field_0x594 += temp_f;
            return;
        } else if (i_this->mPathPoint >= end_point_idx - 1) {
            i_this->mPathPoint = start_point_idx;
        } else {
            i_this->mPathPoint++;
        }

        i_this->field_0x594 = 0.0f;
    }
}

static int daKytag10_Draw(kytag10_class* i_this) {
    GXColor sphere_color = {0xFF, 0xFF, 0xFF, 0xFF};
    return 1;
}

static int daKytag10_Execute(kytag10_class* i_this) {
    sparks_move(i_this);
    return 1;
}

static int daKytag10_IsDelete(kytag10_class* i_this) {
    return 1;
}

static int daKytag10_Delete(kytag10_class* i_this) {
    g_env_light.mOdourData.field_0xf21 = 0;

    if (i_this->mpEmitter1 != NULL) {
        i_this->mpEmitter1->deleteAllParticle();
        i_this->mpEmitter1->becomeInvalidEmitter();
        i_this->mpEmitter1->quitImmortalEmitter();
        i_this->mpEmitter1->setEmitterCallBackPtr(NULL);
        i_this->mpEmitter1 = NULL;
    }

    if (i_this->mpEmitter2 != NULL) {
        i_this->mpEmitter2->deleteAllParticle();
        i_this->mpEmitter2->becomeInvalidEmitter();
        i_this->mpEmitter2->quitImmortalEmitter();
        i_this->mpEmitter2->setEmitterCallBackPtr(NULL);
        i_this->mpEmitter2 = NULL;
    }

    return 1;
}

static int daKytag10_Create(fopAc_ac_c* i_this) {
    kytag10_class* a_this = (kytag10_class*)i_this;
    fopAcM_ct(i_this, kytag10_class);

    u8 prm0 = fopAcM_GetParam(i_this);
    if (prm0 == 0xFF) {
        prm0 = 10;
    }

    a_this->mEmitterPtclScale.z = prm0 * 0.1f;
    a_this->mEmitterPtclScale.y = prm0 * 0.1f;
    a_this->mEmitterPtclScale.x = prm0 * 0.1f;

    s16 prm1 = (fopAcM_GetParam(i_this) >> 8) & 0xFF;
    if (prm1 == 0xFF) {
        prm1 = 0;
    }

    a_this->mEmitterVolSize = prm1 * 10;

    s16 prm3 = (fopAcM_GetParam(i_this) >> 0x18) & 0xFF;
    if (prm3 == 0xFF) {
        prm3 = 15;
    }

    a_this->mEmitterLifeTime = prm3 * 10;
    a_this->field_0x584 = i_this->current.angle.x & 0xFF;
    a_this->mpEmitter1 = NULL;
    a_this->mpEmitter2 = NULL;
    a_this->field_0x594 = 0.0f;
    a_this->mPathPoint = 0;
    a_this->mpPath = set_path_info(i_this);
    a_this->mpEmitter1 = dComIfGp_particle_set(0x852B, &i_this->current.pos, NULL, NULL);
    a_this->mpEmitter2 = dComIfGp_particle_set(0x852C, &i_this->current.pos, NULL, NULL);
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag10_Method = {
    (process_method_func)daKytag10_Create,  (process_method_func)daKytag10_Delete,
    (process_method_func)daKytag10_Execute, (process_method_func)daKytag10_IsDelete,
    (process_method_func)daKytag10_Draw,
};

actor_process_profile_definition g_profile_KYTAG10 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG10,
    &g_fpcLf_Method.base,
    sizeof(kytag10_class),
    0,
    0,
    &g_fopAc_Method.base,
    104,
    &l_daKytag10_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
