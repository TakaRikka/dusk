/**
 * d_a_tag_hjump.cpp
 * Epona fence jump trigger / object
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_hjump.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"

static char const l_arcName[] = "Hfence";

int daTagHjump_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daTagHjump_c::create() {
    fopAcM_ct(this, daTagHjump_c);

    mType = (fopAcM_GetParam(this) >> 4) & 0xF;
    field_0x5ad = fopAcM_GetParam(this) & 0xF;
    mOnFlagID = (fopAcM_GetParam(this) >> 8) & 0xFFF;
    mOffFlagID = fopAcM_GetParam(this) >> 0x14;

    if (field_0x5ad == 15) {
        field_0x5ad = 0;
    }

    int phase;
    if (mType != TYPE_TRIGGER_e) {
        phase = dComIfG_resLoad(&mPhase, l_arcName);
        if (phase == cPhs_COMPLEATE_e) {
            phase = MoveBGCreate(l_arcName, 7, NULL, 0xCA0, NULL);

            if (phase == cPhs_COMPLEATE_e) {
                fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
                tevStr.room_no = fopAcM_GetRoomNo(this);

                mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
                mDoMtx_stack_c::YrotM(shape_angle.y);
                mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

                mDoMtx_stack_c::get()[1][3] -= 100.0f;
                current.pos.y -= 100.0f;

                mpBgW->SetLock();
                scale.x = 250.0f;
                scale.y = 350.0f;
                scale.z = 400.0f;
                field_0x5b8 = 200.0f;

                fopAcM_SetMin(this, -scale.x, 0.0f, -50.0f);
                fopAcM_SetMax(this, scale.x, 200.0f, 50.0f);

                field_0x5ae = 0x2000;
                fopAcM_setCullSizeFar(this, 5.0f);
                field_0x5ad = 4;
            }
        }
    } else {
        phase = cPhs_COMPLEATE_e;
        scale.x *= 100.0f;
        scale.z *= 100.0f;
        scale.y *= 100.0f;

        if (field_0x5ad == 0 || field_0x5ad == 1) {
            field_0x5b8 = 50.0f;
            field_0x5ae = 0x1000;
        } else {
            field_0x5b8 = 200.0f;
            field_0x5ae = 0x2000;
        }
    }

    field_0x5b4 = current.pos.y + scale.y;
    return phase;
}

static int daTagHjump_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagHjump_c*>(i_this)->create();
}

daTagHjump_c::~daTagHjump_c() {
    if (mType != TYPE_TRIGGER_e) {
        dComIfG_resDelete(&mPhase, l_arcName);
    }
}

static int daTagHjump_Delete(daTagHjump_c* i_this) {
    i_this->MoveBGDelete();
    i_this->~daTagHjump_c();
    return 1;
}

int daTagHjump_c::execute() {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (horse_p != NULL &&
        (mType != TYPE_TRIGGER_e ||
         ((mOnFlagID == 0xFFF ||
           dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[mOnFlagID])) &&
          (mOffFlagID == 0xFFF ||
           !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[mOffFlagID])))))
    {
        s16 angle_diff = horse_p->shape_angle.y - shape_angle.y;
        int abs_angle = abs(angle_diff);
        f32 min_jump_speed;

        if (field_0x5ad == 0 || field_0x5ad == 2) {
            min_jump_speed = horse_p->getLashMaxSpeedF() * 0.8f;
        } else if (field_0x5ad == 1 || field_0x5ad == 3) {
            min_jump_speed = horse_p->getNormalMaxSpeedF() * 0.8f;
        } else {
            min_jump_speed = horse_p->getNormalMaxSpeedF() +
                             (horse_p->getLashMaxSpeedF() - horse_p->getNormalMaxSpeedF()) * 0.5f;
        }

        if (min_jump_speed < horse_p->speedF &&
            (abs_angle < field_0x5ae || abs_angle > 0x8000 - field_0x5ae))
        {
            cXyz rel_pos;
            fpoAcM_relativePos(this, &horse_p->current.pos, &rel_pos);

            f32 temp_f3 = fabsf(rel_pos.z);
            if (rel_pos.y >= 0.0f && rel_pos.y <= scale.y && fabsf(rel_pos.x) <= scale.x) {
                if (temp_f3 >= (scale.z - field_0x5b8) && temp_f3 <= (scale.z + field_0x5b8) &&
                    ((rel_pos.z > 0.0f && abs_angle > 0x8000 - field_0x5ae) ||
                     (rel_pos.z < 0.0f && abs_angle < field_0x5ae)))
                {
                    f32 temp_f3_2 = fabsf(rel_pos.z / cM_scos(angle_diff));
                    f32 var_f4;

                    if (temp_f3_2 < field_0x5b8) {
                        var_f4 = field_0x5b8;
                    } else {
                        var_f4 = temp_f3_2;
                    }

                    rel_pos.x += (temp_f3_2 * cM_ssin(angle_diff)) + (var_f4 * cM_ssin(angle_diff));
                    if (fabsf(rel_pos.x) <= scale.x ||
                        (mType == TYPE_TRIGGER_e && field_0x5ad == 4))
                    {
                        horse_p->onTagJump(temp_f3_2, field_0x5b4, var_f4);
                    }
                }
            }
        }
    }

    return 1;
}

static int daTagHjump_Execute(daTagHjump_c* i_this) {
    return i_this->execute();
}

int daTagHjump_c::Draw() {
    if (mType != TYPE_TRIGGER_e) {
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
    }

    return 1;
}

static int daTagHjump_Draw(daTagHjump_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class l_daTagHjump_Method = {
    (process_method_func)daTagHjump_Create,  (process_method_func)daTagHjump_Delete,
    (process_method_func)daTagHjump_Execute, (process_method_func)NULL,
    (process_method_func)daTagHjump_Draw,
};

actor_process_profile_definition g_profile_Tag_Hjump = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Tag_Hjump,
    &g_fpcLf_Method.base,
    sizeof(daTagHjump_c),
    0,
    0,
    &g_fopAc_Method.base,
    434,
    &l_daTagHjump_Method,
    0x44100,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
