/**
 * d_a_obj.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_a_obj.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

namespace daObj {

void eff_break_tsubo(fopAc_ac_c* i_actor, cXyz i_scale, int i_type) {
    J3DModelData* tubo_bmd = (J3DModelData*)dComIfG_getObjectRes("Always", 0x20);
    J3DAnmTexPattern* tubo_btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("Always", 0x42);

    JPABaseEmitter* emitter = dComIfGp_particle_set(
        ID_ZI_J_M_TUBOHAHEN, &i_actor->current.pos, NULL, NULL, 0xFF,
        &dPa_modelEcallBack::mEcallback, fopAcM_GetRoomNo(i_actor), NULL, NULL, &i_scale);

    dPa_modelEcallBack::setModel(emitter, tubo_bmd, i_actor->tevStr, 3, tubo_btp, 0, i_type);

    dComIfGp_particle_set(ID_ZI_J_TUBOHAHEN, &i_actor->current.pos, NULL, NULL, 0xFF,
                          dPa_control_c::getTsuboSelectTexEcallBack(i_type),
                          fopAcM_GetRoomNo(i_actor), NULL, NULL, &i_scale);
}

void make_eff_break_kotubo(fopAc_ac_c* i_actor) {
    static cXyz scale(1.0f, 1.0f, 1.0f);

    eff_break_tsubo(i_actor, scale, 0);
}

void make_eff_break_kotubo2(fopAc_ac_c* i_actor) {
    static cXyz scale(1.0f, 1.0f, 1.0f);

    eff_break_tsubo(i_actor, scale, 1);
}

void make_eff_break_gm_kotubo(fopAc_ac_c* i_actor) {
    static cXyz scale(1.0f, 1.0f, 1.0f);

    eff_break_tsubo(i_actor, scale, 3);
}

void make_eff_break_gm_ootubo(fopAc_ac_c* i_actor) {
    static cXyz scale(2.0f, 2.0f, 2.0f);

    eff_break_tsubo(i_actor, scale, 4);
}

void posMoveF_stream(fopAc_ac_c* i_actor, cXyz const* param_1, cXyz const* param_2, f32 param_3,
                     f32 param_4) {
    posMoveF_grade(i_actor, param_1, param_2, param_3, param_4, NULL, 0.0f, 0.0f, NULL);
}


namespace {
void posMoveF_resist_acc(cXyz* pos, const fopAc_ac_c* i_actor, cXyz const* stream_spd, f32 param_3,
                         f32 param_4) {
    const cXyz sp20 = i_actor->speed - *stream_spd;
    cXyz sp14(sp20.x * param_3, sp20.y * param_3, sp20.z * param_3);

    sp14.x += param_4 * (sp20.x * fabsf(sp20.x));
    sp14.y += param_4 * (sp20.y * fabsf(sp20.y));
    sp14.z += param_4 * (sp20.z * fabsf(sp20.z));

    sp14 *= -1.0f;
    pos->set(sp14);
}

/* 800374EC-80037620 031E2C 0134+00 1/1 0/0 0/0 .text
 * posMoveF_grade_acc__Q25daObj21@unnamed@d_a_obj_cpp@FP4cXyzPC10fopAc_ac_cPC4cXyzffPC4cXyzPC4cXyz
 */
void posMoveF_grade_acc(cXyz* pos, const fopAc_ac_c* i_actor, cXyz const* param_2, f32 friction,
                        f32 no_grade_cos, const cXyz* param_5, const cXyz* param_6) {
    pos->set(cXyz::Zero);

    if (param_2 != NULL) {
        cXyz sp28(*param_5);
        sp28.y += i_actor->gravity;

        if (param_6 != NULL) {
            sp28 += *param_6;
        }

        if (sp28.inprod(*param_2) < 0.0f) {
            if (param_2->y <= no_grade_cos) {
                cXyz sp34;
                cM3d_CrawVec(*param_2, sp28, &sp34);
                *pos += sp34;
            }

            cXyz sp40;
            cM3d_CrawVec(*param_2, i_actor->speed, &sp40);
            *pos -= sp40 * friction;
        }
    }
}
}  // namespace

void posMoveF_grade(fopAc_ac_c* i_actor, cXyz const* param_1, cXyz const* stream_spd, f32 param_3,
                    f32 param_4, cXyz const* param_5, f32 friction, f32 no_grade_cos,
                    cXyz const* param_8) {
    JUT_ASSERT(0, stream_spd != NULL);

    cXyz spAC;
    posMoveF_resist_acc(&spAC, i_actor, stream_spd, param_3, param_4);

    JUT_ASSERT(0, (friction >= 0.0f) && (friction < 1.0f) && (no_grade_cos >= 0.0f) &&
               (no_grade_cos <= 1.0f));

    cXyz spB8;
    posMoveF_grade_acc(&spB8, i_actor, param_5, friction, no_grade_cos, &spAC, param_8);

    f32 speedF = fopAcM_GetSpeedF(i_actor);
    f32 gravity = fopAcM_GetGravity(i_actor);
    cXyz* speed_p = fopAcM_GetSpeed_p(i_actor);

    f32 speed_x = spB8.x + (spAC.x + (speedF * cM_ssin(i_actor->current.angle.y)));
    f32 speed_y = spB8.y + (spAC.y + (speed_p->y + gravity));
    f32 speed_z = spB8.z + (spAC.z + (speedF * cM_scos(i_actor->current.angle.y)));

    if (param_8 != NULL) {
        speed_x += param_8->x;
        speed_y += param_8->y;
        speed_z += param_8->z;
    }

    if (speed_y < fopAcM_GetMaxFallSpeed(i_actor)) {
        speed_y = fopAcM_GetMaxFallSpeed(i_actor);
    }

    fopAcM_SetSpeed(i_actor, speed_x, speed_y, speed_z);
    fopAcM_SetSpeedF(i_actor, JMAFastSqrt((speed_x * speed_x) + (speed_z * speed_z)));
    i_actor->current.angle.y = cM_atan2s(speed_x, speed_z);
    fopAcM_posMove(i_actor, param_1);
}

void quat_rotBaseY(Quaternion* quat, cXyz const& param_1) {
    static const Quaternion zero_quat = {0.0f, 0.0f, 0.0f, 1.0f};

    f32 var_f31 = param_1.abs2();
    if (var_f31 > 9.999999E-9f) {
        cXyz sp4C = cXyz::BaseY.outprod(param_1);

        f32 var_f30 = sp4C.abs2();
        if (var_f30 > 9.999999E-9f) {
            cXyz sp58 = param_1 / var_f31;
            f32 var_f1 = acos(cXyz::BaseY.inprod(sp58));
            sp4C /= JMAFastSqrt(var_f30);
            mDoMtx_quatRotAxisRad(quat, &sp4C, var_f1);
        } else {
            *quat = zero_quat;
        }
    } else {
        *quat = zero_quat;
    }
}

void HitSeStart(cXyz const* i_sePos, int i_roomNo, dCcD_GObjInf const* i_CcObj, u32 param_3) {
    int hit_se = const_cast<dCcD_GObjInf*>(i_CcObj)->GetTgHitObjSe();
    fopAc_ac_c* hit_actor = const_cast<dCcD_GObjInf*>(i_CcObj)->GetTgHitAc();
    cCcD_Obj* hit_obj = const_cast<dCcD_GObjInf*>(i_CcObj)->GetTgHitObj();
    bool master_sword_hit = false;
    u32 sfx;

    if (hit_se != dCcD_SE_NONE) {
        if (hit_actor != NULL && hit_obj != NULL) {
            if (hit_obj->ChkAtType(AT_TYPE_8000)) {
                sfx = Z2SE_HIT_HAMMER;
            } else if (hit_obj->ChkAtType(AT_TYPE_ARROW)) {
                sfx = Z2SE_HIT_ARROW_REBOUND;
            } else if (hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
                sfx = Z2SE_HIT_HOOKSHOT_REBOUND;
            } else {
                if (fopAcM_GetProfName(hit_actor) == PROC_ALINK &&
                    daPy_py_c::checkMasterSwordEquip())
                {
                    master_sword_hit = true;
                }

                if (hit_se == dCcD_SE_WOOD) {
                    sfx = Z2SE_HIT_WOOD_WEAPON;
                } else if (hit_se == dCcD_SE_METAL) {
                    sfx = Z2SE_HIT_HOOKSHOT_REBOUND;
                } else if (master_sword_hit) {
                    sfx = Z2SE_HIT_SWORD;
                } else {
                    sfx = Z2SE_HIT_SWORD;
                }
            }
        }

        s8 reverb = dComIfGp_getReverb(i_roomNo);
        mDoAud_seStart(sfx, i_sePos, param_3 & 0xFF, (s8)reverb);
    }
}

static cXyz dummy1(cCcD_CylAttr* cyl) {
    return cyl->GetCoCP();
}

static cXyz dummy2(cCcD_SphAttr* sph) {
    return sph->GetCoCP();
}

}  // namespace daObj
