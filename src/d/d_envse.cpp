/**
 * d_envse.cpp
 * Environment Sound Effects handler
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_envse.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "f_op/f_op_camera_mng.h"
#include "string.h"

static int dEnvSe_Draw(dEnvSe_c* i_this) {
    return 1;
}

static void dEnvSe_getNearPathPos(cXyz* param_0, cXyz* param_1, dPath* i_path) {
    int var_r31 = 0;
    BOOL sp10[2] = {0, 0};
    f32 sp8;

    f32 var_f31 = FLT_MAX;

    int i;
    dPnt* point_p = i_path->m_points;

    cXyz sp3C[2];
    cM3dGLin sp14;

    for (i = 0; i < i_path->m_num; i++) {
        sp8 = cM3d_LenSq(param_1, &point_p->m_position);
        if (var_f31 > sp8) {
            var_f31 = sp8;
            var_r31 = i;
        }

        point_p++;
    }

    point_p = &i_path->m_points[var_r31];
    if (var_r31 != 0) {
        sp14.set(point_p[-1].m_position, point_p[0].m_position);
        sp10[0] = cM3d_Len3dSqPntAndSegLine(&sp14, param_1, &sp3C[0], &sp8);
    }

    if (var_r31 != i_path->m_num - 1) {
        sp14.set(point_p[0].m_position, point_p[1].m_position);
        sp10[1] = cM3d_Len3dSqPntAndSegLine(&sp14, param_1, &sp3C[1], &sp8);
    }

    if (sp10[0]) {
        if (sp10[1]) {
            if (cM3d_LenSq(&sp3C[1], param_1) > sp8) {
                *param_0 = sp3C[0];
            } else {
                *param_0 = sp3C[1];
            }
        } else {
            *param_0 = sp3C[0];
        }
    } else if (sp10[1]) {
        *param_0 = sp3C[1];
    } else {
        param_0->set(point_p[0].m_position);
    }
}

int dEnvSe_c::execute_common(dStage_SoundInfo_c* i_soundInf, s8* param_1, u8 param_2) {
    int roomNo = dComIfGp_roomControl_getStayNo();
    int path_roomNo = dComIfGp_roomControl_getStayNo();
    BOOL var_r25 = false;

    if (i_soundInf == NULL) {
        return 1;
    }

    int i = i_soundInf->num;
    stage_sound_data* data_p = i_soundInf->entries;

    cXyz cam_eye = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0))->mCamera.Eye();

    if (param_2 == 0) {
        path_roomNo = -1;
    }

    for (; i != 0; i--) {
        if (memcmp(data_p->field_0x0, "sndtag", 6) == 0) {
            if (!(*param_1 & 1)) {
                if (data_p->field_0x17 != 0) {
                    mDoAud_mEnvse_initStaticEnvSe(data_p->field_0x17, data_p->field_0x14,
                                                  data_p->field_0x19, data_p->field_0x1a,
                                                  &data_p->field_0x8);
                }
                *param_1 |= 1;
            }

            if (!var_r25) {
                mDoAud_mEnvse_startStaticEnvSe(dComIfGp_getReverb(roomNo));
            }
            var_r25 = true;
        } else if (data_p->field_0x17 < 8) {
            if (!(*param_1 & 2)) {
                mDoAud_mEnvse_initRiverSe(data_p->field_0x17, data_p->field_0x14,
                                          data_p->field_0x19, data_p->field_0x1a);
                *param_1 |= 2;
            }

            for (dPath* path_p = dPath_GetRoomPath(data_p->field_0x18, path_roomNo); path_p != NULL;
                 path_p = dPath_GetNextRoomPath(path_p, path_roomNo))
            {
                dEnvSe_getNearPathPos(&pos, &cam_eye, path_p);
                mDoAud_mEnvse_registRiverSePos(&pos);
            }

            mDoAud_mEnvse_startRiverSe(dComIfGp_getReverb(roomNo));
        } else if (data_p->field_0x17 < 16) {
            cXyz sp14;
            f32 fvar12 = FLT_MAX;

            if (!(*param_1 & 4)) {
                mDoAud_mEnvse_initFallSe(data_p->field_0x17, data_p->field_0x14, data_p->field_0x19,
                                         data_p->field_0x1a);
                *param_1 |= 4;
            }

            pos = cam_eye;

            for (dPath* path_p = dPath_GetRoomPath(data_p->field_0x18, path_roomNo); path_p != NULL;
                 path_p = dPath_GetNextRoomPath(path_p, path_roomNo))
            {
                dEnvSe_getNearPathPos(&sp14, &cam_eye, path_p);

                f32 fvar11 = cM3d_LenSq(&sp14, &cam_eye);
                if (fvar11 < fvar12) {
                    fvar12 = fvar11;
                    pos = sp14;
                }

                mDoAud_mEnvse_registFallSePos(&sp14);
            }

            mDoAud_mEnvse_startFallSe(dComIfGp_getReverb(roomNo));
        } else if (data_p->field_0x17 < 32) {
            if (!(*param_1 & 8)) {
                mDoAud_mEnvse_initSmellSe(data_p->field_0x17, data_p->field_0x14,
                                          data_p->field_0x19, data_p->field_0x1a);
                *param_1 |= 8;
            }

            for (dPath* path_p = dPath_GetRoomPath(data_p->field_0x18, path_roomNo); path_p != NULL;
                 path_p = dPath_GetNextRoomPath(path_p, path_roomNo))
            {
                dEnvSe_getNearPathPos(&pos, &cam_eye, path_p);
                mDoAud_mEnvse_registSmellSePos(&pos);
            }

            mDoAud_mEnvse_startSmellSe(dComIfGp_getReverb(roomNo));
        } else if (data_p->field_0x17 < 64) {
            for (dPath* path_p = dPath_GetRoomPath(data_p->field_0x18, path_roomNo); path_p != NULL;
                 path_p = dPath_GetNextRoomPath(path_p, path_roomNo))
            {
                dEnvSe_getNearPathPos(&pos, &cam_eye, path_p);
                mDoAud_mEnvse_registWindowPos(&pos);
            }
        } else if (data_p->field_0x17 >= 100) {
            if (!((field_0x108 >> i) & 1)) {
                mDoAud_initLv3WaterSe(data_p->field_0x17, data_p->field_0x14, data_p->field_0x19,
                                      data_p->field_0x1a);
                field_0x108 |= (1 << i);
            }

            for (dPath* path_p = dPath_GetRoomPath(data_p->field_0x18, path_roomNo); path_p != NULL;
                 path_p = dPath_GetNextRoomPath(path_p, path_roomNo))
            {
                dEnvSe_getNearPathPos(&pos, &cam_eye, path_p);
                mDoAud_registLv3WaterSePos(data_p->field_0x17, &pos);
            }

            mDoAud_startLv3WaterSe(dComIfGp_getReverb(roomNo));
        }
        data_p++;
    }

    return 1;
}

int dEnvSe_c::execute() {
    int roomNo = dComIfGp_roomControl_getStayNo();
    dStage_roomDt_c* roomDt_p = dComIfGp_roomControl_getStatusRoomDt(roomNo);

    if (roomDt_p == NULL) {
        return 1;
    }

    if (mRoomNo != roomNo) {
        field_0xfc = 0;
        field_0xfd = 0;
        field_0x108 = 0;
        mRoomNo = roomNo;
    }

    execute_common(roomDt_p->getSoundInfCL(), &field_0xfc, 1);
    execute_common(roomDt_p->getSoundInf(), &field_0xfd, 1);
    execute_common(dComIfGp_getStage()->getSoundInfCL(), &field_0xfe, 0);

    return execute_common(dComIfGp_getStage()->getSoundInf(), &field_0xff, 0);
}

static int dEnvSe_Execute(dEnvSe_c* i_this) {
    return i_this->execute();
}

static int dEnvSe_IsDelete(dEnvSe_c* i_this) {
    return 1;
}

static int dEnvSe_Delete(dEnvSe_c* i_this) {
    return 1;
}

static int dEnvSe_Create(dEnvSe_c* i_this) {
    i_this->field_0xfc = 0;
    i_this->field_0xfd = 0;
    i_this->field_0xfe = 0;
    i_this->field_0xff = 0;
    i_this->mRoomNo = 0xFF;
    i_this->field_0x108 = 0;

    return cPhs_COMPLEATE_e;
}

static leafdraw_method_class l_dEnvSe_Method = {
    (process_method_func)dEnvSe_Create,  (process_method_func)dEnvSe_Delete,
    (process_method_func)dEnvSe_Execute, (process_method_func)dEnvSe_IsDelete,
    (process_method_func)dEnvSe_Draw,
};

extern kankyo_process_profile_definition g_profile_ENVSE = {
    fpcLy_CURRENT_e,
    2,
    fpcPi_CURRENT_e,
    PROC_ENVSE,
    &g_fpcLf_Method.base,
    sizeof(dEnvSe_c),
    0,
    0,
    &g_fopKy_Method,
    689,
    &l_dEnvSe_Method,
};
