/**
 * d_a_npc_ne.cpp
 * NPC - Cat
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_ne.h"
#include "SSystem/SComponent/c_counter.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/actor/d_a_mg_fish.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/JUtility/JUTReport.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "c/c_damagereaction.h"
#include "Z2AudioLib/Z2Instances.h"

static home_path_pnt home_path[38] = {
    {0, {561.0f, 87.0f, -1110.0f}},
    {1, {306.0f, 87.0f, -849.0f}},
    {2, {158.0f, 139.0f, -929.0f}},
    {3, {137.0f, 145.0f, -1055.0f}},
    {4, {332.0f, 481.0f, -1113.0f}},
    {5, {710.0f, 697.0f, -1275.0f}},
    {6, {1005.0f, 766.0f, -1423.0f}},
    {3, {-6.0f, 178.0f, -990.0f}},
    {4, {155.0f, 132.0f, -1606.0f}},
    {5, {482.0f, 93.0f, -1997.0f}},
    {6, {775.0f, 70.0f, -2280.0f}},
    {7, {1185.0f, 75.0f, -2458.0f}},
    {2, {107.0f, 127.0f, -556.0f}},
    {3, {363.0f, 150.0f, 44.0f}},
    {4, {527.0f, 203.0f, 849.0f}},
    {5, {989.0f, 94.0f, 1872.0f}},
    {6, {1346.0f, 267.0f, 2454.0f}},
    {7, {1670.0f, 179.0f, 2923.0f}},
    {8, {2129.0f, 100.0f, 3308.0f}},
    {9, {2967.0f, 120.0f, 3215.0f}},
    {10, {3427.0f, 185.0f, 2506.0f}},
    {11, {3675.0f, 185.0f, 1778.0f}},
    {12, {4129.0f, 197.0f, 988.0f}},
    {13, {4003.0f, 151.0f, 376.0f}},
    {14, {3694.0f, 75.0f, -321.0f}},
    {2, {107.0f, 127.0f, -556.0f}},
    {3, {589.0f, 99.0f, -162.0f}},
    {4, {639.0f, 155.0f, 275.0f}},
    {2, {107.0f, 127.0f, -556.0f}},
    {3, {19.0f, 215.0f, 5.0f}},
    {4, {-172.0f, 227.0f, 728.0f}},
    {5, {-497.0f, 202.0f, 1467.0f}},
    {6, {-992.0f, 115.0f, 2189.0f}},
    {7, {-1479.0f, 101.0f, 2711.0f}},
    {8, {-1911.0f, 88.0f, 3391.0f}},
    {9, {-2173.0f, 75.0f, 4025.0f}},
    {10, {-2526.0f, 80.0f, 4260.0f}},
    {-1, {0.0f, 0.0f, 0.0f}},
};

daNpc_Ne_HIO_c::daNpc_Ne_HIO_c() :
    field_0x04(-1),
    mScale(1.0f),
    mWalkSpeed(2.5f),
    mRunSpeed(12.0f),
    mSwimSpeed(2.0f),
    field_0x18(0)
{
    /* empty function */
}

static void anm_init(npc_ne_class* i_this, int i_resNo, f32 i_morf, u8 i_attr, f32 i_speed) {
    J3DAnmTransform* anm =
        static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(i_this->mResName, i_resNo));
    i_this->mpMorf->setAnm(anm, i_attr, i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->mAnmID = i_resNo;
}


static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        npc_ne_class* _this = (npc_ne_class*)model->getUserArea();
        if (_this != NULL) {
            PSMTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);

            // head
            if (jnt_no == 4) {
                mDoMtx_YrotM(*calc_mtx, _this->mHeadAngleY + _this->mHeadBobAngleY);
                mDoMtx_XrotM(*calc_mtx, _this->mHeadAngleX + _this->mHeadBobAngleX);
                mDoMtx_ZrotM(*calc_mtx, _this->mHeadAngleZ);
            }

            // backbone1 / backbone2 / neck
            if (jnt_no == 1 || jnt_no == 2 || jnt_no == 3) {
                mDoMtx_YrotM(*calc_mtx, _this->mBackboneAngleY);
                if (jnt_no == 3) {
                    mDoMtx_YrotM(*calc_mtx, _this->mNeckAngleY);
                }
            }

            // tail1 / tail2
            if (jnt_no == 20 || jnt_no == 21) {
                if (jnt_no == 20) {
                    mDoMtx_ZrotM(*calc_mtx, _this->mTailAngle);
                } else if (jnt_no == 21) {
                    if (_this->mTailAngle > 0) {
                        mDoMtx_ZrotM(*calc_mtx, _this->mTailAngle * 2);
                    }
                }
                mDoMtx_ZrotM(*calc_mtx, _this->mTailSwayAngle[jnt_no - 20]);
            }

            // center
            if (jnt_no == 0) {
                mDoMtx_YrotM(*calc_mtx, 0);
                mDoMtx_XrotM(*calc_mtx, 0);
                mDoMtx_ZrotM(*calc_mtx, 0);
            }

            model->setAnmMtx(jnt_no, *calc_mtx);
            PSMTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return 1;
}

static int daNpc_Ne_Draw(npc_ne_class* i_this) {
    if (daPy_py_c::linkGrabSubjectNoDraw(i_this)) {
        return 1;
    }
    
    J3DModel* model = i_this->mpMorf->getModel();
    if (i_this->mResName == "Npc_net") {
        if (!dComIfGs_wolfeye_effect_check()) {
            return 1;
        }
        g_env_light.settingTevStruct(4, &i_this->current.pos, &i_this->tevStr);
    } else {
        g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    }
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);

    i_this->mpBtkAnm->entry(model->getModelData());
    i_this->mpBtpAnm->entry(model->getModelData());
    i_this->mpMorf->entryDL();

    if (!fopAcM_checkCarryNow(i_this)) {
        cXyz vec(i_this->current.pos.x, i_this->current.pos.y + 100.0f, i_this->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(
            i_this->mShadowKey, 1, model, &vec, 400.0f, 0.0f, i_this->current.pos.y,
            i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &i_this->tevStr,
            0, 1.0f, dDlst_shadowControl_c::getSimpleTex()
        );
    }

    if (i_this->mBehavior == npc_ne_class::BHV_DISH) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpDishMorf->getModel(), &i_this->tevStr);
        i_this->mpDishMorf->entryDL();
    }

    return 1;
}

static BOOL other_bg_check(fopAc_ac_c* i_this, fopAc_ac_c* i_actor) {
    dBgS_LinChk lin_chk;
    if (i_actor != NULL) {
        cXyz vec1, vec2;
        vec2 = i_actor->current.pos;
        vec2.y += 100.0f;
        vec1 = i_this->current.pos;
        vec1.y += 50.0f;
        lin_chk.Set(&vec1, &vec2, i_this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}


static BOOL ne_carry_check(npc_ne_class* i_this) {
    if (i_this->mAction != npc_ne_class::ACT_CARRY && fopAcM_checkCarryNow(i_this)) {
        i_this->mAction = npc_ne_class::ACT_CARRY;
        i_this->mMode = 0;
        i_this->mNoFollow = false;
        return true;
    } else {
        return false;
    }
}

static int way_bg_check(npc_ne_class* i_this, s16 i_angle) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    dBgS_GndChk gnd_chk;
    cM3dGPla plane;
    cXyz vec1, vec2, vec3;

    vec1 = _this->current.pos;
    vec1.y += 50.0f;
    mDoMtx_YrotS(*calc_mtx, _this->current.angle.y + i_angle);
    if (_this->current.angle.x < 0) {
        mDoMtx_XrotM(*calc_mtx, _this->current.angle.x);
    }
    vec3.set(0.0f, 0.0f, 150.0f);
    MtxPosition(&vec3, &vec2);
    vec2 += vec1;
    lin_chk.Set(&vec1, &vec2, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        if (plane.mNormal.y >= 0.8f) {
            return 1;
        }
    }
    
    mDoMtx_YrotS(*calc_mtx, _this->current.angle.y + i_angle);
    vec3.set(0.0f, 150.0f, 150.0f);
    MtxPosition(&vec3, &vec2);
    vec2 += _this->current.pos;
    vec1 = vec2;
    vec2.y -= 250.0f;
    lin_chk.Set(&vec1, &vec2, _this);
    if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
        return 2;
    }

    dBgS_ObjGndChk_Spl gnd_chk_spl;
    vec2.y += 200.0f;
    gnd_chk.SetPos(&vec2);
    f32 cross1 = dComIfG_Bgsp().GroundCross(&gnd_chk);
    gnd_chk_spl.SetPos(&vec2);
    f32 cross2 = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    if (cross2 > cross1 && fabsf(vec2.y - cross2) <= 200.0f) {
        return 3;
    } else {
        return 0;
    }
}

static int water_check(npc_ne_class* i_this, f32 param_1) {
    dBgS_ObjGndChk_Spl gnd_chk_spl;
    dBgS_GndChk gnd_chk;
    dBgS_LinChk lin_chk;
    cXyz vec;
    vec.x = i_this->current.pos.x;
    vec.y = i_this->current.pos.y + 3000.0f;
    vec.z = i_this->current.pos.z;
    lin_chk.Set(&i_this->current.pos, &vec, i_this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        vec.y = lin_chk.GetCross().y - 10.0f;
    }
    gnd_chk_spl.SetPos(&vec);
    i_this->mWaterY = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    gnd_chk.SetPos(&vec);
    f32 cross = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (i_this->current.pos.y <= i_this->mWaterY
        && i_this->mWaterY - cross > param_1 * 2.0f * i_this->mBaseScale.z)
    {
        return 1;
    } else {
        return 0;
    }
}

static f32 dummy(f32 param_0) {
    return param_0 + 5.0f + 60.0f + 25.0f;
}

static s16 climb_angle_get(npc_ne_class* i_this) {
    dBgS_LinChk lin_chk;
    mDoMtx_YrotS(*calc_mtx, i_this->current.angle.y);
    mDoMtx_XrotM(*calc_mtx, i_this->current.angle.x);
    cXyz vec1, vec2, vec3;
    vec3.set(0.0f, 50.0f, 10.0f);
    MtxPosition(&vec3, &vec1);
    vec1 += i_this->current.pos;
    vec3.y = -100.0f;
    MtxPosition(&vec3, &vec2);
    vec2 += i_this->current.pos;
    lin_chk.Set(&vec1, &vec2, i_this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        i_this->mClimbPos = lin_chk.GetCross();
        vec3.z = 20.0f;
        MtxPosition(&vec3, &vec2);
        vec2 += i_this->current.pos;
        lin_chk.Set(&vec1, &vec2, i_this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            vec2 = lin_chk.GetCross();
            vec3 = vec2 - i_this->mClimbPos;
            return -cM_atan2s(vec3.y, JMAFastSqrt(vec3.x * vec3.x + vec3.z * vec3.z));
        }
        return 0;
    }
    return 0;
}

static bool hio_set;

static daNpc_Ne_HIO_c l_HIO;

static fopAc_ac_c* target_info[5];

static BOOL target_bgc[5];

static int target_info_count;

static void* s_bl_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc)
        && (fopAcM_GetName(i_proc) == PROC_OBJ_FOOD || fopAcM_GetName(i_proc) == PROC_BD)
        && target_info_count < 5)
    {
        target_info[target_info_count] = static_cast<fopAc_ac_c*>(i_proc);
        target_info_count++;
    }
    return NULL;
}

static void* s_ss_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc)
        && (fopAcM_GetName(i_proc) == PROC_NI || fopAcM_GetName(i_proc) == PROC_BD)
        && target_info_count < 5)
    {
        target_info[target_info_count] = static_cast<fopAc_ac_c*>(i_proc);
        target_info_count++;
    }
    return NULL;
}

static fopAc_ac_c* search_bird(npc_ne_class* i_this) {
    target_info_count = 0;
    for (int i = 0; i < 5; i++) {
        target_info[i] = NULL;
        target_bgc[i] = false;
    }

    if (i_this->mSearchBall) {
        fpcEx_Search(s_bl_sub, i_this);
    } else {
        fpcEx_Search(s_ss_sub, i_this);
    }

    f32 f = 200.0f;
    if (target_info_count != 0) {
        fopAc_ac_c* actor;
        int i = 0;
        while (i < target_info_count) {
            actor = target_info[i];
            cXyz eye_delta, delta;
            eye_delta.x = actor->current.pos.x - i_this->eyePos.x;
            eye_delta.y = actor->current.pos.y + 50.0f - i_this->eyePos.y;
            eye_delta.z = actor->current.pos.z - i_this->eyePos.z;
            delta.x = actor->current.pos.x - i_this->current.pos.x;
            delta.z = actor->current.pos.z - i_this->current.pos.z;
            f32 eye_dist = JMAFastSqrt(eye_delta.x * eye_delta.x + eye_delta.z * eye_delta.z);
            f32 dist = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);

            if (eye_dist < f && !(dist > i_this->mDistToTarget + 30.0f)) {
                if (target_bgc[i] || other_bg_check(i_this, actor)) {
                    target_bgc[i] = true;
                } else {
                    if (fabsf(actor->current.pos.y + 50.0f - i_this->eyePos.y) <= 100.0f) {
                        s16 angle = i_this->shape_angle.y - cM_atan2s(eye_delta.x, eye_delta.z);
                        if (angle < 0x4000 && angle > -0x4000) {
                            return actor;
                        }
                    }
                }
            }
            
            i++;
            if (i == target_info_count) {
                i = 0;
                f += 200.0f;
                if (f > 2000.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }
    return NULL;
}

static BOOL way_check(npc_ne_class* i_this, s16 i_angleY) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    f32 fvar1 = 1000.0f;
    cXyz vec1, vec2, vec3, vec4;
    vec2 = _this->current.pos;
    vec2.y += 20.0f;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 0x10; j++) {
            mDoMtx_YrotS(*calc_mtx, i_angleY);
            vec1.set(0.0f, 100.0f, fvar1);
            MtxPosition(&vec1, &vec3);
            vec3 += vec2;
            dBgS_LinChk lin_chk;
            lin_chk.Set(&vec2, &vec3, _this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                i_angleY += 0x1000;
            } else {
                vec4.set(vec3.x, vec3.y - 250.0f, vec3.z);
                lin_chk.Set(&vec3, &vec4, _this);
                if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                    i_angleY += 0x1000;
                } else {
                    i_this->mTargetAngleY = i_angleY;
                    return true;
                }
            }
        }
        fvar1 -= 150.0f;
    }
    return false;
}

static void npc_ne_wait(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    cLib_addCalc0(&_this->speedF, 1.0f, 1.3f);

    if (i_this->mResName == "Npc_net") {
        switch (i_this->mMode) {
        case 0:
            i_this->mTargetAngleY = cM_rndF(0x10000);
            // fallthrough

        case 1:
            if (cM_rndF(1.0f) < 0.5f) {
                if (i_this->mAnmID != npc_ne_class::ANM_WAIT) {
                    anm_init(i_this, npc_ne_class::ANM_WAIT, 20.0f, 2, 1.0f);
                }
            } else {
                if (i_this->mAnmID != npc_ne_class::ANM_SIT_B) {
                    anm_init(i_this, npc_ne_class::ANM_SIT_B, 20.0f, 0, 1.0f);
                }
            }
            i_this->mTimers[0] = cM_rndF(200.0f) + 500.0f;
            i_this->mMode = 2;
            break;

        case 2:
            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 1;
            }
            break;
        }

        cLib_addCalcAngleS2(&_this->current.angle.y, i_this->mTargetAngleY, 4, 0x400);
        i_this->mMessageState = 2;
    } else {
        i_this->mLookMode = npc_ne_class::LOOK_PLAYER;
        s16 angle_diff;

        switch (i_this->mMode) {
        case 0:
            i_this->mAnmSpeed = 1.5f;
            anm_init(i_this, npc_ne_class::ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);
            i_this->mMode++;
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->mTargetAngleY = i_this->mAngleToPlayer + 0xB000;
            } else {
                i_this->mTargetAngleY = i_this->mAngleToPlayer + 0x5000;
            }
            // fallthrough

        case 1:
            cLib_addCalcAngleS2(&_this->current.angle.y, i_this->mTargetAngleY, 4, 0x400);
            angle_diff = _this->current.angle.y - i_this->mTargetAngleY;
            if (angle_diff < 0x200 && angle_diff > -0x200) {
                i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
                anm_init(i_this, npc_ne_class::ANM_WAIT, 3.0f, 2, i_this->mAnmSpeed);
                i_this->mMode++;
                i_this->mTimers[0] = cM_rndF(100.0f) + 200.0f;
            }
            break;

        case 2:
            angle_diff = _this->current.angle.y - i_this->mAngleToPlayer + 0x8000;
            if (angle_diff < 0x800 && angle_diff > -0x800) {
                i_this->mMode = 0;
            }
            if (i_this->mTimers[0] == 1) {
                i_this->mAnmSpeed = 1.0f;
                anm_init(i_this, npc_ne_class::ANM_SIT_B, 20.0f, 0, i_this->mAnmSpeed);
            }
            break;
        }

        if (!(i_this->mCounter & 0xf) && !other_bg_check(_this, player) &&
            (!i_this->mNoFollow || i_this->mDistToTarget < 350.0f))
        {
            i_this->mAction = npc_ne_class::ACT_AWAY;
            i_this->mMode = 0;
            i_this->mTimers[3] = 0;
            i_this->mTimers[2] = 0;
            i_this->mTimers[1] = 0;
            i_this->mTimers[0] = 0;
        }
    }
}

static void npc_ne_away(npc_ne_class* i_this) {
    if (!i_this->mNoFollow) {
        i_this->mAction = npc_ne_class::ACT_TAME;
        i_this->mMode = 0;
        return;
    }

    int ivar4;
    s16 max_angle_step = 0xa00;
    ivar4 = 0;
    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_WALK, 5.0f, 2, i_this->mAnmSpeed);
        i_this->mTargetAngleY = i_this->mAngleToPlayer + 0x8000 + (s16)cM_rndFX(4000.0f);
        way_check(i_this, i_this->mTargetAngleY);
        i_this->mMode++;
        i_this->mDistScale = cM_rndFX(0.2f) + 1.0f;
        // fallthrough

    case 1:
        max_angle_step = 0x200;
        if (i_this->mTimers[3] == 0) {
            i_this->mTimers[3] = cM_rndF(40.0f) + 40.0f;
            i_this->mLookNoMoveTimer = cM_rndF(20.0f) + 20.0f;
        }
        cLib_addCalc2(&i_this->mAnmSpeed, 2.0f, 1.0f, 0.05f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed,
                      1.0f, l_HIO.mWalkSpeed * 0.2f);
        if (i_this->mDistToTarget > 500.0f) {
            i_this->mMode = 2;
            if (i_this->mTargetAngleY > (s16)(i_this->mAngleToPlayer + 0x8000)) {
                i_this->mTargetAngleY = i_this->mAngleToPlayer + 0xB000;
            } else {
                i_this->mTargetAngleY = i_this->mAngleToPlayer + 0x5000;
            }
            i_this->mTimers[0] = 20;
            i_this->mTimers[1] = 0;
        } else if (i_this->mDistToTarget < 200.0f) {
            i_this->mMode = 5;
            i_this->mTargetAngleY = i_this->mAngleToPlayer + 0x8000 + (s16)cM_rndFX(4000.0f);
            way_check(i_this, i_this->mTargetAngleY);
            i_this->mSound.startSound(Z2SE_CAT_CRY_ANNOY, 0, -1);
        } else {
            if (i_this->mTimers[1] == 0) {
                if (i_this->mCounter & 1) {
                    ivar4 = way_bg_check(i_this, 0);
                }
                if (ivar4 || i_this->mAcch.ChkWallHit() || !(i_this->mCounter & 0x1f)) {
                    i_this->mTargetAngleY =
                        i_this->mAngleToPlayer + 0x8000 + (s16)cM_rndFX(3000.0f);
                    way_check(i_this, i_this->mTargetAngleY);
                }
                if (i_this->mTimers[4] != 0) {
                    max_angle_step = 0x600;
                } else {
                    max_angle_step = 0x200;
                }
            } else {
                max_angle_step = 0x400;
            }
        }
        break;

    case 2:
        cLib_addCalc2(&i_this->mAnmSpeed, 2.0f, 1.0f, 0.05f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed,
                      1.0f, l_HIO.mWalkSpeed * 0.2f);
        if (i_this->mTimers[1] == 0) {
            if (way_bg_check(i_this, 0) >= 2) {
                i_this->mTimers[1] = cM_rndF(20.0f) + 30.0f;
                i_this->mTargetAngleY = i_this->shape_angle.y + 0x8000 + (s16)cM_rndFX(5000.0f);
            }
            max_angle_step = 0x200;
        } else {
            max_angle_step = 0x400;
        }
        i_this->mLookMode = npc_ne_class::LOOK_PLAYER;
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
            anm_init(i_this, npc_ne_class::ANM_WAIT, 3.0f, 2, i_this->mAnmSpeed);
            i_this->mMode = 2;
            i_this->mTimers[0] = cM_rndF(100.0f) + 200.0f;
        }
        break;

    case 5:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        // fallthrough

    case 6:
        if (i_this->mTimers[3] == 0) {
            i_this->mTimers[3] = cM_rndF(20.0f) + 20.0f;
            i_this->mLookNoMoveTimer = cM_rndF(10.0f) + 10.0f;
        }
        cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.1f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed,
                      1.0f, l_HIO.mRunSpeed * 0.5f);
        if (i_this->mCounter & 1) {
            ivar4 = way_bg_check(i_this, 0);
        }
        if (ivar4 || i_this->mAcch.ChkWallHit() || !(i_this->mCounter & 0xf)) {
            i_this->mTargetAngleY = i_this->mAngleToPlayer + 0x8000 + (s16)cM_rndFX(4000.0f);
            way_check(i_this, i_this->mTargetAngleY);
        }
        if (i_this->mTimers[0] == 0) {
            i_this->mTargetAngleY += (s16)cM_rndFX(4000.0f);
            i_this->mTimers[0] = cM_rndF(25.0f) + 20.0f;
        }
        if (i_this->mDistToTarget > 400.0f && i_this->mTimers[2] == 0) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
        }
        break;
    }

    s16 prev_angle = i_this->current.angle.y;
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, max_angle_step);
    f32 ang_z = i_this->speedF * (i_this->current.angle.y - prev_angle) * -0.5f;
    if (ang_z > 4000.0f) {
        ang_z = 4000.0f;
    } else if (ang_z < -4000.0f) {
        ang_z = -4000.0f;
    }
    i_this->current.angle.z = ang_z;

    if (max_angle_step >= 0x400) {
        f32 ang_y = i_this->speedF * (i_this->current.angle.y - prev_angle) * 0.1f;
        if (ang_y > 5000.0f) {
            ang_y = 5000.0f;
        } else if (ang_y < -5000.0f) {
            ang_y = -5000.0f;
        }
        i_this->mBackboneTargetAngleY = ang_y;
    }
    
    i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);
}

static cXyz ground_search(npc_ne_class* i_this) {
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    dBgS_ObjGndChk_Spl gnd_chk_spl;
    dBgS_GndChk gnd_chk;
    cXyz vec1, vec2;
    for (int i = 0; i <= 4; i++) {
        mDoMtx_YrotS(*calc_mtx, player->shape_angle.y);
        vec1.y = 100.0f;
        if (i == 0) {
            vec1.x = 0.0f;
            vec1.z = -300.0f;
        } else if (i == 1) {
            vec1.x = 300.0f;
            vec1.z = -100.0f;
        } else if (i == 2) {
            vec1.x = -300.0f;
            vec1.z = -100.0f;
        } else if (i == 3) {
            vec1.x = -300.0f;
            vec1.z = 100.0f;
        } else {
            vec1.x = -300.0f;
            vec1.z = 100.0f;
        }
        MtxPosition(&vec1, &vec2);
        vec2 += player->current.pos;
        gnd_chk.SetPos(&vec2);
        vec2.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (fabsf(vec2.y - player->current.pos.y) < 100.0f) {
            gnd_chk_spl.SetPos(&vec2);
            if (dComIfG_Bgsp().GroundCross(&gnd_chk_spl) < vec2.y) {
                return vec2;
            }
        }
    }
    return i_this->current.pos;
}


static void* s_fish_sub(void* i_proc, void* i_this) {
    npc_ne_class* _this = static_cast<npc_ne_class*>(i_this);
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)i_proc;
        if (fish->mCurAction == 0x35 && fish->mActionPhase >= 10) {
            _this->mFishID = fopAcM_GetID(fish);
            return i_proc;
        }
    }
    return NULL;
}

static void npc_ne_tame(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    if (i_this->mNoFollow) {
        i_this->mAction = npc_ne_class::ACT_AWAY;
        i_this->mMode = 0;
        i_this->mTimers[3] = 0;
        i_this->mTimers[2] = 0;
        i_this->mTimers[1] = 0;
        i_this->mTimers[0] = 0;
    } else {
        daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
        s16 angle_max_step = 0;
        i_this->mLookMode = npc_ne_class::LOOK_PLAYER;

        f32 dist1, dist2, dist3;
        if (i_this->mWantsFish) {
            dist1 = 250.0f;
            dist2 = 350.0f;
            dist3 = 150.0f;
        } else {
            dist1 = 200.0f;
            dist2 = 500.0f;
            dist3 = 0.0f;
        }

        int ivar7 = 0;
        s16 angle_diff;
        switch (i_this->mMode) {
            case 0:
                i_this->mAnmSpeed = 1.0f;
                anm_init(i_this, npc_ne_class::ANM_WALK, 5.0f, 2, i_this->mAnmSpeed);
                i_this->mMode++;
                i_this->mDistScale = cM_rndFX(0.2f) + 1.0f;
                // fallthrough

            case 1:
                i_this->mTargetAngleY = i_this->mAngleToPlayer;
                angle_max_step = 0x400;
                i_this->mTailTargetAngle = -15000;
                i_this->mTailSwayTarget = 3000.0f;
                if (i_this->mDistToTarget < dist1) {
                    cLib_addCalc2(&i_this->mAnmSpeed, 1.0f, 1.0f, 0.05f);
                    cLib_addCalc0(&_this->speedF, 1.0f, 2.5f);
                    if (_this->speedF < 4.0f) {
                        i_this->mTimers[0] = cM_rndF(100.0f) + 200.0f;
                        i_this->mMode = 2;
                        i_this->mTimers[1] = 30;
                    }
                } else if (i_this->mDistToTarget > dist2) {
                    i_this->mMode = 5;
                } else {
                    f32 target_anm_speed = (i_this->mDistToTarget - dist1) * 0.1f;
                    if (target_anm_speed > 3.5f) {
                        target_anm_speed = 3.5f;
                    }
                    if (target_anm_speed < 2.0f) {
                        target_anm_speed = 2.0f;
                    }
                    cLib_addCalc2(&i_this->mAnmSpeed, target_anm_speed, 1.0f, 0.1f);
                    cLib_addCalc2(&_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed,
                                  1.0f, 0.2f * l_HIO.mWalkSpeed);
                }
                break;

            case 2:
                i_this->mTargetAngleY = i_this->mAngleToPlayer;
                angle_diff = _this->current.angle.y - i_this->mTargetAngleY;
                if (i_this->mTimers[1] != 0) {
                    if (angle_diff > 0x4000 || angle_diff < -0x4000) {
                        angle_max_step = 0x800;
                        if (i_this->mAnmID != npc_ne_class::ANM_WALK) {
                            anm_init(i_this, npc_ne_class::ANM_WALK, 3.0f, 2, 2.0f);
                        }
                    } else {
                        if (i_this->mAnmID != npc_ne_class::ANM_WAIT) {
                            i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
                            anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, i_this->mAnmSpeed);
                        }
                    }
                }

                if (i_this->mTimers[0] != 0) {
                    i_this->mTailTargetAngle = -15000;
                    i_this->mTailSwayTarget = 3000.0f;
                }

                if (i_this->mDistToTarget < dist3 ||
                                    (i_this->mTimers[3] == 0 && i_this->mAcch.ChkWallHit())) {
                    i_this->mMode = 7;
                    i_this->mTimers[3] = 30;
                } else if (i_this->mDistToTarget < dist3 + 30.0f) {
                    if (i_this->mTimers[2] == 0 && i_this->mTimers[1] == 0
                                    && angle_diff < 0x4000 && angle_diff > -0x4000
                                    && way_bg_check(i_this, 0x8000) == 0) {
                        i_this->mTimers[2] = cM_rndF(7.0f) + 15.0f;
                        anm_init(i_this, npc_ne_class::ANM_WALK, 3.0f, 2, -3.0f);
                        i_this->mAnmSpeed = -3.0f;
                    }
                } else if (i_this->mDistToTarget > dist1 + 50.0f) {
                    i_this->mMode = 0;
                }

                if (i_this->mTimers[2] != 0) {
                    if (i_this->mTimers[2] == 1) {
                        i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
                        anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, i_this->mAnmSpeed);
                    }
                    cLib_addCalc2(&_this->speedF, -3.5f, 1.0f, 1.0f);
                    angle_max_step = 0x400;
                } else {
                    cLib_addCalc0(&_this->speedF, 1.0f, 1.0f);
                }

                if (i_this->mTimers[0] == 1) {
                    i_this->mAnmSpeed = 1.0f;
                    anm_init(i_this, npc_ne_class::ANM_SIT_A, 20.0f, 0, i_this->mAnmSpeed);
                }
                break;

            case 5:
                i_this->mAnmSpeed = 1.0f;
                anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, i_this->mAnmSpeed);
                i_this->mMode++;
                i_this->mSound.startSound(Z2SE_CAT_CRY_FAMILIER, 0, -1);
                // fallthrough
            
            case 6:
                i_this->mTargetAngleY = i_this->mAngleToPlayer;
                angle_max_step = 0xa00;
                cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.1f);
                cLib_addCalc2(&_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed,
                              1.0f, 0.5f * l_HIO.mRunSpeed);

                if (i_this->mDistToTarget < dist1 + 100.0f) {
                    i_this->mAnmSpeed = 3.5f;
                    anm_init(i_this, npc_ne_class::ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);
                    i_this->mMode = 1;
                }
                break;
            
            case 7:
                i_this->mAnmSpeed = 1.0f;
                anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, i_this->mAnmSpeed);
                i_this->mMode++;
                i_this->mSound.startSound(Z2SE_CAT_CRY_FAMILIER, 0, -1);
                i_this->mTargetAngleY = i_this->mAngleToPlayer + 0x8000;
                // fallthrough

            case 8:
                angle_max_step = 0xa00;
                cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.1f);
                cLib_addCalc2(&_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed,
                              1.0f, 0.5f * l_HIO.mRunSpeed);
                
                if (i_this->mCounter & 1) {
                    ivar7 = way_bg_check(i_this, 0);
                }
                if (ivar7 != 0 || i_this->mAcch.ChkWallHit() || !(i_this->mCounter & 0xf))
                {
                    i_this->mTargetAngleY =
                        i_this->mAngleToPlayer + 0x8000 + (s16)cM_rndFX(4000.0f);
                    way_check(i_this, i_this->mTargetAngleY);    
                }

                if (i_this->mDistToTarget > dist3 + 40.0f) {
                    i_this->mAnmSpeed = 3.5f;
                    anm_init(i_this, npc_ne_class::ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);
                    i_this->mMode = 1;
                }
                break;
            
            case 10:
            case 11:
                i_this->mTargetAngleY = i_this->mAngleToPlayer;
                cLib_addCalc0(&_this->speedF, 1.0f, 3.0f);
                angle_diff = _this->current.angle.y - i_this->mTargetAngleY;
                
                if (i_this->mMode == 10) {
                    if (angle_diff > 0x400 || angle_diff < -0x400) {
                        i_this->mAnmSpeed = 1.0f;
                        anm_init(i_this, npc_ne_class::ANM_WALK, 5.0f, 2, i_this->mAnmSpeed);
                        i_this->mMode = 11;
                    }
                } else {
                    angle_max_step = 0x600;
                    i_this->mLookDownTimer = 0;
                    if (angle_diff <= 0x400 && angle_diff >= -0x400) {
                        i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
                        anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, i_this->mAnmSpeed);
                        i_this->mMode = 10;
                    }
                }

                if (i_this->mDistToTarget < dist3) {
                    i_this->mMode = 7;
                    i_this->mTimers[2] = 20;
                }
                break;
        }

        if (i_this->mMode < 7) {
            if (i_this->mTimers[2] == 0) {
                ivar7 = way_bg_check(i_this, 0);
                if (ivar7 != 0) {
                    if (ivar7 == -1) {
                        i_this->mAction = npc_ne_class::ACT_CLIMB;
                        i_this->mMode = 0;
                    } else {
                        i_this->mMode = 10;
                        i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
                        anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, i_this->mAnmSpeed);
                        if (ivar7 >= 2) {
                            i_this->mLookDownTimer = 30;
                        }
                    }
                }
            }
        } else if (i_this->mMode >= 10 && way_bg_check(i_this, 0) == 0) {
            i_this->mMode = 0;
        }

        s16 prev_angle_y = _this->current.angle.y;
        cLib_addCalcAngleS2(&_this->current.angle.y, i_this->mTargetAngleY, 4, angle_max_step);
        f32 angle = _this->speedF * (_this->current.angle.y - prev_angle_y) * -0.5f;
        if (angle > 4000.0f) {
            angle = 4000.0f;
        } else if (angle < -4000.0f) {
            angle = -4000.0f;
        }
        _this->current.angle.z = angle;

        if (angle_max_step > 0x600) {
            angle = _this->speedF * (_this->current.angle.y - prev_angle_y) * 0.1f;
            if (angle > 5000.0f) {
                angle = 5000.0f;
            } else if (angle < -5000.0f) {
                angle = -5000.0f;
            }
            i_this->mBackboneTargetAngleY = angle;
        }

        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);

            /* dSv_event_flag_c::F_0470 - Fishing Pond - Reserved for fishing */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[470])) {
            if (fpcEx_Search(s_fish_sub, _this) != NULL) {
                i_this->mAction = npc_ne_class::ACT_HOME;
                i_this->mMode = 10;
            }
        }

        if ((i_this->mCounter & 0x1f) == 0xf && other_bg_check(_this, player)) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
        }
    }
}

static void npc_ne_sanbasi(npc_ne_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, 1.0f);
        i_this->mMode = 1;
        break;
    case 1:
        i_this->mLookDownTimer = 30;
    }

    if (i_this->mDistToTarget < 500.0f || !i_this->mNoFollow) {
        i_this->mAction = npc_ne_class::ACT_TAME;
        i_this->mMode = 0;
        i_this->mLookDownTimer = 0;
        if (!i_this->mNoFollow) {
            i_this->mSound.startSound(Z2SE_CAT_CRY_FAMILIER, 0, -1);
        }
        i_this->mTimers[4] = 100;
    }
}

static void npc_ne_bird(npc_ne_class* i_this) {
    fopAc_ac_c* bird = search_bird(i_this);
    if (bird == NULL) {
        i_this->mAction = npc_ne_class::ACT_WAIT;
        i_this->mMode = 0;
        return;
    }

    i_this->mTargetPos = bird->current.pos;
    cXyz delta = i_this->mTargetPos - i_this->current.pos;
    i_this->mTargetAngleY = cM_atan2s(delta.x, delta.z);
    i_this->mDistToTarget = delta.abs();

    s16 max_angle_step = 0;
    i_this->mLookMode = npc_ne_class::LOOK_TARGET;
    i_this->mLookTarget = i_this->mTargetPos;

    switch(i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_WALK, 5.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        i_this->mDistScale = cM_rndFX(0.2f) + 1.0f;
        // fallthrough
    
    case 1:
        max_angle_step = 0x400;
        i_this->mTailTargetAngle = -15000;
        i_this->mTailSwayTarget = 3000.0f;
        if (i_this->mDistToTarget < 200.0f) {
            cLib_addCalc2(&i_this->mAnmSpeed, 1.0f, 1.0f, 0.05f);
            cLib_addCalc0(&i_this->speedF, 1.0f, 2.5f);
            if (i_this->speedF < 1.0f) {
                i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
                anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, i_this->mAnmSpeed);
                i_this->mTimers[0] = cM_rndF(100.0f) + 200.0f;
                i_this->mTimers[1] = cM_rndF(20.0f) + 3.0f;
                i_this->mMode = 2;
            }
        } else if (i_this->mDistToTarget > 500.0f) {
            i_this->mMode = 5;
        } else {
            f32 target_anm_speed = (i_this->mDistToTarget - 200.0f) * 0.1f;
            if (target_anm_speed > 3.5f) {
                target_anm_speed = 3.5f;
            }
            if (target_anm_speed < 2.0f) {
                target_anm_speed = 2.0f;
            }
            cLib_addCalc2(&i_this->mAnmSpeed, target_anm_speed, 1.0f, 0.1f);
            cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed,
                          1.0f, 0.2f * l_HIO.mWalkSpeed);
        }
        break;

    case 2:
        if (i_this->mTimers[0] != 0) {
            i_this->mTailTargetAngle = -15000;
            i_this->mTailSwayTarget = 3000.0f;
        }
        cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
        if (i_this->mDistToTarget > 250.0f) {
            i_this->mMode = 0;
        }
        if (i_this->mTimers[1] == 1 && cM_rndF(1.0f) < 0.75f) {
            i_this->mAction = npc_ne_class::ACT_JUMP;
            i_this->mMode = 0;
        }
        if (i_this->mTimers[0] == 1) {
            i_this->mAnmSpeed = 1.0f;
            anm_init(i_this, npc_ne_class::ANM_SIT_A, 20.0f, 0, i_this->mAnmSpeed);
        }
        break;

    case 5:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        // fallthrough

    case 6:
        max_angle_step = 0xa00;
        cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.1f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed,
                      1.0f, 0.5f * l_HIO.mRunSpeed);
        if (i_this->mDistToTarget < 350.0f) {
            i_this->mAnmSpeed = 3.5f;
            anm_init(i_this, npc_ne_class::ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);
            i_this->mMode = 1;
        }
        break;
    }

    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, max_angle_step);
    i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);
}

static void npc_ne_ball(npc_ne_class* i_this) {
    fopAc_ac_c* ball = search_bird(i_this);
    if (ball == NULL) {
        i_this->mAction = npc_ne_class::ACT_WAIT;
        i_this->mMode = 0;
        return;
    }

    i_this->mTargetPos = ball->current.pos;
    cXyz delta = i_this->mTargetPos - i_this->current.pos;
    i_this->mTargetAngleY = cM_atan2s(delta.x, delta.z);
    i_this->mDistToTarget = delta.abs();

    s16 max_angle_step = 0;
    i_this->mLookMode = npc_ne_class::LOOK_TARGET;
    i_this->mLookTarget = i_this->mTargetPos;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_WALK, 5.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        i_this->mDistScale = cM_rndFX(0.2f) + 1.0f;
        if (cM_rndF(1.0f) < 0.35f) {
            i_this->field_0x6d4 = 70.0f;
        } else {
            i_this->field_0x6d4 = 40.0f;
        }
        // fallthrough

    case 1:
        max_angle_step = 0x400;
        i_this->mTailTargetAngle = -15000;
        i_this->mTailSwayTarget = 3000.0f;
        if (i_this->mDistToTarget < i_this->field_0x6d4) {
            cLib_addCalc2(&i_this->mAnmSpeed, 1.0f, 1.0f, 0.05f);
            cLib_addCalc0(&i_this->speedF, 1.0f, 2.5f);
            if (i_this->speedF < 1.0f) {
                if (i_this->field_0x6d4 < 50.0f) {
                    i_this->mMode = 3;
                    if (cM_rndF(1.0f) < 0.5f) {
                        anm_init(i_this, npc_ne_class::ANM_FAWN_L, 10.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, npc_ne_class::ANM_FAWN_R, 10.0f, 0, 1.0f);
                    }
                } else {
                    i_this->mAnmSpeed = cM_rndF(0.1f) + 0.5f;
                    anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, i_this->mAnmSpeed);
                    i_this->mTimers[0] = cM_rndF(100.0f) + 200.0f;
                    i_this->mTimers[1] = cM_rndF(20.0f) + 3.0f;
                    i_this->mMode = 2;
                }
            }
        } else if (i_this->mDistToTarget > 200.0f) {
            i_this->mMode = 5;
        } else {
            f32 target_anm_speed = (i_this->mDistToTarget - 70.0f) * 0.1f;
            if (target_anm_speed > 3.5f) {
                target_anm_speed = 3.5f;
            }
            if (target_anm_speed < 2.0f) {
                target_anm_speed = 2.0f;
            }
            cLib_addCalc2(&i_this->mAnmSpeed, target_anm_speed, 1.0f, 0.1f);
            cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed,
                          1.0f, 0.2f * l_HIO.mWalkSpeed);
        }
        break;

    case 2:
        i_this->mTailTargetAngle = -15000;
        i_this->mTailSwayTarget = 3000.0f;
        cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
        if (i_this->mDistToTarget > 100.0f) {
            i_this->mMode = 0;
        }
        if (i_this->mTimers[1] == 1) {
            i_this->mAction = npc_ne_class::ACT_S_JUMP;
            i_this->mMode = 0;
        }
        break;

    case 3:
        i_this->mTailTargetAngle = -15000;
        i_this->mTailSwayTarget = 3000.0f;
        cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
        }
        if (i_this->mpMorf->getFrame() >= 16.0f) {
            cLib_addCalc2(&i_this->field_0xbf4, 80.0f, 1.0f, 20.0f);
        }
        break;

    case 5:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        // fallthrough

    case 6:
        max_angle_step = 0xa00;
        cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.1f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed,
                      1.0f, 0.5f * l_HIO.mRunSpeed);
        if (i_this->mDistToTarget < 150.0f) {
            i_this->mAnmSpeed = 3.5f;
            anm_init(i_this, npc_ne_class::ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);
            i_this->mMode = 1;
        }
    }

    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, max_angle_step);
    i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);
}

static void npc_ne_pathwalk(npc_ne_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.5f;
        anm_init(i_this, npc_ne_class::ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);
        i_this->mMode = 1;
        // fallthrough

    case 1: {
        i_this->mPathPointNo += i_this->mPathDir;
        if (i_this->mPathPointNo >= (i_this->mpPath->m_num & 0xff)) {
            if (dPath_ChkClose(i_this->mpPath)) {
                i_this->mPathPointNo = 0;
            } else {
                i_this->mPathDir = -1;
                i_this->mPathPointNo = i_this->mpPath->m_num - 2;
            }
        } else if (i_this->mPathPointNo < 0) {
            i_this->mPathDir = 1;
            i_this->mPathPointNo = 1;
        }
        dPnt* point = i_this->mpPath->m_points;
        point += i_this->mPathPointNo;
        i_this->mTargetPos.x = point->m_position.x + cM_rndFX(50.0f);
        i_this->mTargetPos.y = point->m_position.y;
        i_this->mTargetPos.z = point->m_position.z + cM_rndFX(50.0f);
        i_this->mMode = 2;
        i_this->mAngleYStep = 0.0f;
        break;
    }
    case 2:
        i_this->mLookTarget = i_this->mTargetPos;
        cXyz delta = i_this->mTargetPos - i_this->current.pos;
        if (delta.abs() < i_this->speedF * 10.0f) {
            i_this->mMode = 1;
        }
        i_this->mTargetAngleY = cM_atan2s(delta.x, delta.z);
        break;
    }

    if (i_this->mAnmID == npc_ne_class::ANM_WALK) {
        f32 target_speed;
        if (i_this->mDistToTarget < 300.0f) {
            target_speed = 3.0f;
            if (i_this->mTimers[3] == 0) {
                i_this->mTimers[3] = cM_rndF(40.0f) + 40.0f;
                i_this->mLookNoMoveTimer = cM_rndF(20.0f) + 20.0f;
            }
        } else {
            target_speed = 1.5f;
            i_this->mLookMode = npc_ne_class::LOOK_TARGET;
        }
        cLib_addCalc2(&i_this->mAnmSpeed, target_speed, 1.0f, 0.05f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed * 0.9f,
                      1.0f, 0.2f * l_HIO.mWalkSpeed);
        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);
        if (i_this->mDistToTarget < 100.0f && i_this->speedF > 7.4f) {
            i_this->mAnmSpeed = 1.0f;
            anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, i_this->mAnmSpeed);
        }
    } else {
        cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.1f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed,
                      1.0f, 0.5f * l_HIO.mRunSpeed);
        if (i_this->mDistToTarget > 300.0f) {
            i_this->mAnmSpeed = 3.0f;
            anm_init(i_this, npc_ne_class::ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);
        }
    }
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY,
                        8, i_this->mAngleYStep * i_this->speedF);
    cLib_addCalc2(&i_this->mAngleYStep, 150.0f, 1.0f, 30.0f);
}

static void npc_ne_jump(npc_ne_class* i_this) {
    int frame = i_this->mpMorf->getFrame();
    f32 target_speed = 0.0f;
    f32 max_speed_step = 3.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_TO_JUMP, 5.0f, 0, 1.0f);
        i_this->mMode++;
        break;

    case 1:
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, 0x1000);
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, npc_ne_class::ANM_JUMP_LOOP, 2.0f, 0, 1.0f);
            i_this->mMode++;
            i_this->mSound.startSound(Z2SE_CAT_CRY_ATTACK, 0, -1);
        }
        break;

    case 2:
        target_speed = 2.0f;
        max_speed_step = 2.0f;
        if (frame >= 1) {
            if (frame < 10) {
                target_speed = 25.0f;
                max_speed_step = 25.0f;
                i_this->mAtEnable = 1;
                i_this->mAtSph.OffAtVsPlayerBit();
            } else {
                target_speed = 0.0f;
                max_speed_step = 4.0f;
            }
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, npc_ne_class::ANM_TO_WAIT, 2.0f, 0, 1.0f);
            i_this->mMode++;
        }
        break;

    case 3:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, max_speed_step);
}

static void npc_ne_s_jump(npc_ne_class* i_this) {
    int frame = i_this->mpMorf->getFrame();
    f32 target_speed = 0.0f;
    f32 max_speed_step = 3.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_TO_JUMP, 3.0f, 0, 1.5f);
        i_this->mMode++;
        break;

    case 1:
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 2, 0x2000);
        if (frame >= 3) {
            anm_init(i_this, npc_ne_class::ANM_JUMP_START, 2.0f, 0, 1.0f);
            i_this->mMode++;
        }
        break;

    case 2:
        if (frame >= 1) {
            target_speed = 20.0f;
            max_speed_step = 20.0f;
        } else {
            target_speed = 2.0f;
            max_speed_step = 2.0f;
        }
        if (frame >= 3) {
            anm_init(i_this, npc_ne_class::ANM_JUMP_END, 3.0f, 0, 1.0f);
            i_this->mpMorf->setFrame(4.0f);
            i_this->mMode++;
        }
        break;

    case 3:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, max_speed_step);
}

static void npc_ne_roof(npc_ne_class* i_this) {
    i_this->mCcDisableTimer = 5;
    i_this->mLookMode = npc_ne_class::LOOK_PLAYER;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_WAIT, 5.0f, 2, 1.0f);
        i_this->mMode = 1;
        i_this->mTimers[0] = cM_rndF(100.0f) + 200.0f;
        break;

    case 1:
        if (i_this->mTimers[0] == 1) {
            anm_init(i_this, npc_ne_class::ANM_SIT_B, 20.0f, 0, 1.0f);
        }
        if (i_this->mDistToTarget < 500.0f) {
            i_this->mMode = 0;
        }
        break;
    }

    if (i_this->mDistToTarget < 200.0f) {
        i_this->mAction = npc_ne_class::ACT_HOME;
        i_this->mMode = 0;
        i_this->mSound.startSound(Z2SE_CAT_CRY_ANNOY, 0, -1);
    }
}

static int home_path_search(npc_ne_class* i_this, int param_1) {
    f32 max_dist = 100.0f;
    BOOL local_1c0[100];
    for (int i = 0; i < 100; i++) {
        local_1c0[i] = false;
    }

    for (int i = 0; i < 100; i++) {
        if (home_path[i].mNo >= 0) {
            if (home_path[i].mNo >= param_1
                && (i_this->mHomePathIdx == 1000
                    || home_path[i_this->mHomePathIdx].mNo == home_path[i].mNo + 1)
                && !local_1c0[i])
            {
                cXyz delta, pos;
                pos = home_path[i].mPosition;
                delta = i_this->current.pos - pos;
                if (delta.abs() < max_dist) {
                    return i;
                }
            }
        } else {
            i = -1;
            max_dist += 100.0f;
            if (max_dist > 2000.0f) {
                return -1;
            }
        }
    }

    return -1;
}

static BOOL npc_ne_home(npc_ne_class* i_this) {
    BOOL ret = true;
    i_this->mCcDisableTimer = 10;
    int frame = i_this->mpMorf->getFrame();
    f32 target_speed = 0.0f;
    f32 max_speed_step = 4.0f;
    cXyz vec;
    f32 dist_xz;
    s16 angle;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, 1.5f);
        i_this->mMode = 1;
        i_this->mHomePathIdx = 1000;
        i_this->mAngleYStep = 10000.0f;
        i_this->mTimers[0] = 0;
        // fallthrough

    case 1:
        i_this->mHomePathIdx = home_path_search(i_this, 0);
        if (i_this->mHomePathIdx < 0) {
            i_this->mAction = npc_ne_class::ACT_TAME;
            i_this->mMode = 0;
            return true;
        }
        if (i_this->mDemoMode == 0) {
            i_this->mDemoMode = 1;
        }
        i_this->mMode = 2;
        // fallthrough

    case 2:
        target_speed = l_HIO.mRunSpeed * 1.5f;
        vec.x = home_path[i_this->mHomePathIdx].mPosition.x;
        vec.y = home_path[i_this->mHomePathIdx].mPosition.y;
        vec.z = home_path[i_this->mHomePathIdx].mPosition.z;
        vec -= i_this->current.pos;
        i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
        dist_xz = JMAFastSqrt(vec.x * vec.x + vec.z * vec.z);
        if (dist_xz <= target_speed * 1.2f) {
            if (i_this->mHomePathIdx == 1) {
                i_this->mMode = 20;
                i_this->mDemoMode = 3;
                i_this->mDemoCounter = 0;
                i_this->current.pos.set(236.0f, 93.3f, -777.5f);
                i_this->current.angle.y = 0x5f94;
                i_this->mSound.startSound(Z2SE_CAT_CRY_CARRY, 0, -1);
            } else if (i_this->mHomePathIdx == 5 || i_this->mHomePathIdx == 4) {
                i_this->mMode = 3;
                i_this->speedF = 0.0f;
                if (i_this->mHomePathIdx == 5) {
                    i_this->mTimers[1] = 8;
                } else {
                    i_this->mTimers[1] = 12;
                }
                anm_init(i_this, npc_ne_class::ANM_TO_JUMP, 2.0f, 0, 1.0f);
            } else {
                i_this->mMode = 1;
                i_this->mAngleYStep = 5000.0f;
            }
        }
        if (i_this->mAcch.ChkWallHit()) {
            i_this->mTimers[0] += 2;
        }
        if (i_this->mTimers[0] > 30) {
            i_this->mAction = npc_ne_class::ACT_TAME;
            i_this->mMode = 0;
        }
        break;

    case 3:
        i_this->speedF = 0.0f;
        if (i_this->mTimers[1] == 0) {
            i_this->mMode = 4;
            anm_init(i_this, npc_ne_class::ANM_JUMP_LOOP, 2.0f, 0, 1.0f);
            i_this->mTimers[1] = 6;
        }
        break;

    case 4:
        i_this->current.angle.x = 0x3000;
        target_speed = l_HIO.mRunSpeed;
        if (i_this->mTimers[1] == 0 && i_this->mAcch.ChkGroundHit()) {
            i_this->mMode = 1;
            anm_init(i_this, npc_ne_class::ANM_RUN, 2.0f, 2, 1.5f);
        }
        break;

    case 10:
        i_this->mHomePathIdx = 1000;
        i_this->mAngleYStep = 5000.0f;
        i_this->mHomePathIdx = home_path_search(i_this, 3);
        i_this->current.pos.x = home_path[i_this->mHomePathIdx].mPosition.x;
        i_this->current.pos.y = home_path[i_this->mHomePathIdx].mPosition.y;
        i_this->current.pos.z = home_path[i_this->mHomePathIdx].mPosition.z;
        i_this->old.pos = i_this->current.pos;
        anm_init(i_this, npc_ne_class::ANM_TO_JUMP, 5.0f, 0, 1.0f);
        i_this->mMode = 11;
        i_this->mHomePathIdx = home_path_search(i_this, 0);
        vec.x = home_path[i_this->mHomePathIdx].mPosition.x;
        vec.y = home_path[i_this->mHomePathIdx].mPosition.y;
        vec.z = home_path[i_this->mHomePathIdx].mPosition.z;
        vec -= i_this->current.pos;
        angle = cM_atan2s(vec.x, vec.z);
        i_this->shape_angle.y = angle;
        i_this->current.angle.y = angle;
        i_this->mTargetAngleY = angle;
        i_this->mDemoMode = 10;
        i_this->mTimers[0] = 30;
        i_this->mSound.startSound(Z2SE_CAT_CRY_ANNOY_D, 0, -1);
        // fallthrough

    case 11:
        i_this->mNeckAngleY = cM_ssin(g_Counter.mTimer * 5000) * 500.0f;
        if (i_this->mTimers[0] == 0 && i_this->mpMorf->isStop()) {
            anm_init(i_this, npc_ne_class::ANM_JUMP_LOOP, 2.0f, 0, 1.0f);
            i_this->mSound.startSound(Z2SE_CAT_CRY_ATTACK_D, 0, -1);
            i_this->mMode = 12;
        }
        break;

    case 12:
        target_speed = 2.0f;
        max_speed_step = 2.0f;
        if (frame >= 1) {
            if (frame < 10) {
                target_speed = 20.0f;
                max_speed_step = 20.0f;
            } else {
                target_speed = 0.0f;
                max_speed_step = 4.0f;
            }
        }
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, 1.5f);
            i_this->mMode = 13;
        }
        break;

    case 13:
        i_this->mHomePathIdx = home_path_search(i_this, 0);
        if (i_this->mHomePathIdx < 0) {
            i_this->mAction = npc_ne_class::ACT_TAME;
            i_this->mMode = 0;
            i_this->mDemoMode = 99;
            return true;
        }
        i_this->mMode = 14;
        // fallthrough

    case 14:
        target_speed = l_HIO.mRunSpeed * 1.5f;
        vec.x = home_path[i_this->mHomePathIdx].mPosition.x;
        vec.y = home_path[i_this->mHomePathIdx].mPosition.y;
        vec.z = home_path[i_this->mHomePathIdx].mPosition.z;
        vec -= i_this->current.pos;
        i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
        dist_xz = JMAFastSqrt(vec.x * vec.x + vec.z * vec.z);
        if (dist_xz <= 70.0f) {
            if (i_this->mHomePathIdx == 1) {
                i_this->mMode = 20;
                i_this->mDemoMode = 3;
                i_this->mDemoCounter = 0;
                i_this->current.pos.set(236.0f, 93.3f, -777.5f);
                i_this->current.angle.y = 0x5f94;
                i_this->mAngleYStep = 5000.0f;
                /* dSv_event_flag_c::F_0038 - Ordon Village - Opening (2nd day) cat returns home */
                dComIfGs_onEventBit(0x1001);
            } else {
                i_this->mMode = 13;
                i_this->mAngleYStep = 0.0f;
                JUTReport(200, 150, " PN %d", home_path[i_this->mHomePathIdx].mNo);
                if (home_path[i_this->mHomePathIdx].mNo >= 4 && i_this->mDemoCounter > 0xa0) {
                    i_this->mHomePathIdx = home_path_search(i_this, 0);
                    i_this->current.pos.x = home_path[i_this->mHomePathIdx].mPosition.x;
                    i_this->current.pos.y = home_path[i_this->mHomePathIdx].mPosition.y;
                    i_this->current.pos.z = home_path[i_this->mHomePathIdx].mPosition.z;
                    i_this->old.pos = i_this->current.pos;
                    i_this->field_0xcb4 = true;
                    i_this->mDemoCounter = 122;
                }
            }
        }
        if (i_this->mAcch.ChkWallHit()) {
            i_this->mTimers[0] += 2;
        }
        if (i_this->mTimers[0] > 30) {
            i_this->mDemoMode = 99;
        }
        break;

    case 20:
        target_speed = l_HIO.mRunSpeed * 1.5f;
        ret = false;
        i_this->speed.y = 0.0f;
        i_this->gravity = 0.0f;
        vec.x = home_path[0].mPosition.x;
        vec.z = home_path[0].mPosition.z;
        vec -= i_this->current.pos;
        i_this->mTargetAngleY = cM_atan2s(vec.x, vec.z);
        i_this->mAngleYStep = 5000.0f;
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, max_speed_step);
    s16 prev_ang_y = i_this->current.angle.y;
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 2, i_this->mAngleYStep);
    cLib_addCalc2(&i_this->mAngleYStep, 5000.0f, 1.0f, 200.0f);
    
    f32 ang_z = i_this->speedF * (i_this->current.angle.y - prev_ang_y) * -0.5f;
    if (ang_z > 4000.0f) {
        ang_z = 4000.0f;
    } else if (ang_z < -4000.0f) {
        ang_z = -4000.0f;
    }
    i_this->current.angle.z = ang_z;
    
    if (i_this->mAngleYStep > 1000.0f) {
        f32 ang_y = i_this->speedF * (i_this->current.angle.y - prev_ang_y) * 0.1f;
        if (ang_y > 5000.0f) {
            ang_y = 5000.0f;
        } else if (ang_y < -5000.0f) {
            ang_y = -5000.0f;
        }
        i_this->mBackboneTargetAngleY = ang_y;
    }

    return ret;
}

static s16 wall_angle_get(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    mDoMtx_YrotS(*calc_mtx, _this->shape_angle.y);
    cXyz vec1, vec2, vec3[2];
    vec1.set(0.0f, 50.0f, -50.0f);
    MtxPosition(&vec1, &vec2);
    vec2 += _this->current.pos;
    vec1.set(5.0f, 0.0f, 200.0f);
    for (int i = 0; i < 2; i++) {
        MtxPosition(&vec1, &vec3[i]);
        vec1.x *= -1.0f;
        vec3[i] += vec2;
        lin_chk.Set(&vec2, &vec3[i], _this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            i_this->mClimbPos = lin_chk.GetCross();
            vec3[i] = lin_chk.GetCross();
        } else {
            return 1;
        }
    }
    vec1 = vec3[1] - vec3[0];
    return cM_atan2s(vec1.x, vec1.z) + 0x4000;
}

static void search_ground_1(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    cXyz pos[16];
    cXyz vec1, vec2, vec3;
    s16 ang_y = _this->shape_angle.y;
    f32 var_y, var_z;
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP103")) {
        var_y = 200.0f;
        var_z = 100.0f;
    } else {
        var_y = 30.0f;
        var_z = 30.0f;
    }

    for (int i = 0; i < 16; i++) {
        vec1 = _this->current.pos;
        vec1.y = i_this->mWaterY;
        mDoMtx_YrotS(*calc_mtx, ang_y);
        vec3.set(0.0f, 0.0f, 5000.0f);
        MtxPosition(&vec3, &vec2);
        vec2 += vec1;
        lin_chk.Set(&vec1, &vec2, _this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            vec2 = lin_chk.GetCross();
            vec3.set(0.0f, var_y, var_z);
            MtxPosition(&vec3, &vec1);
            vec1 += vec2;
            vec2 = vec1;
            vec2.y -= var_y + 20.0f;
            lin_chk.Set(&vec1, &vec2, _this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                pos[i] = lin_chk.GetCross();
                vec1 = lin_chk.GetCross();
                vec1.y += 5.0f;
                vec3.set(0.0f, 0.0f, 200.0f);
                MtxPosition(&vec3, &vec2);
                vec2 += vec1;
                lin_chk.Set(&vec1, &vec2, _this);
                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    pos[i] = _this->current.pos;
                    pos[i].x += 10000.0f;
                }
            } else {
                pos[i] = _this->current.pos;
                pos[i].x += 10000.0f;
            }
        }
        ang_y += 0x1000;
    }

    int i = 0;
    for (int j = 1; j < 16; j++) {
        if ((pos[i] - _this->current.pos).abs() > (pos[j] - _this->current.pos).abs()) {
            i = j;
        }
    }
    i_this->mTargetPos = pos[i];
}

static BOOL search_ground_2(npc_ne_class* i_this, s16 i_wallAngle) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    cXyz vec1, vec2, vec3;
    vec2 = _this->current.pos;
    vec2.y = i_this->mWaterY;
    f32 var_y, var_z;
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP103")) {
        var_y = 120.0f;
        var_z = 100.0f;
    } else {
        var_y = 50.0f;
        var_z = 40.0f;
    }
    mDoMtx_YrotS(*calc_mtx, i_wallAngle);
    vec3.set(0.0f, var_y, var_z);
    MtxPosition(&vec3, &vec1);
    vec1 += vec2;
    vec2 = vec1;
    vec2.y -= 200.0f;
    lin_chk.Set(&vec1, &vec2, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        i_this->mTargetPos = lin_chk.GetCross();
        return true;
    } else {
        return false;
    }
}

static void npc_ne_swim(npc_ne_class* i_this) {
    s16 max_angle_step = 0;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_SWIM, 5.0f, 2, 1.0f);
        i_this->mMode++;
        i_this->mDistScale = cM_rndFX(0.2f) + 1.0f;
        i_this->speedF = 0.0f;
        i_this->mTimers[1] = 50;
        search_ground_1(i_this);
        // fallthrough

    case 1:
        if (i_this->mpMorf->checkFrame(4.0f) || i_this->mpMorf->checkFrame(12.0f)) {
            i_this->mSound.startSound(Z2SE_CAT_SWIM, 0, -1);
        }
        max_angle_step = 0x100;
        if ((i_this->mCounter & 0x3f) == 0) {
            search_ground_1(i_this);
        }
        cXyz delta = i_this->mTargetPos - i_this->current.pos;
        i_this->mTargetAngleY = cM_atan2s(delta.x, delta.z);
        break;
    }

    cLib_addCalc2(&i_this->speedF, l_HIO.mSwimSpeed, 1.0f, 0.1f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 0x20, max_angle_step);
    i_this->speed.y = 0.0f;
    i_this->gravity = 0.0f;
    cLib_addCalc2(&i_this->current.pos.y,
                  i_this->mWaterY - i_this->mBaseScale.z * 45.0f, 1.0f, 5.0f);
    if (i_this->mTimers[1] == 0 && i_this->mAcch.ChkWallHit()) {
        s16 wall_angle = wall_angle_get(i_this);
        if (wall_angle != 1 && search_ground_2(i_this, wall_angle)) {
            i_this->mAction = npc_ne_class::ACT_OUT_SWIM;
            i_this->mMode = 0;
            i_this->mTargetAngleY = wall_angle_get(i_this);
        }
    }
}

static void npc_ne_outswim(npc_ne_class* i_this) {
    cXyz delta;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_RUN, 5.0f, 2, 2.0f);
        i_this->mMode++;
        i_this->speedF = 0.0f;
        // fallthrough

    case 1:
        i_this->current.angle.x = -0x4000;
        cLib_addCalc2(&i_this->current.pos.y, i_this->mTargetPos.y - 10.0f, 1.0f, 5.0f);
        cLib_addCalc2(&i_this->current.pos.x, i_this->mClimbPos.x, 1.0f, 5.0f);
        cLib_addCalc2(&i_this->current.pos.z, i_this->mClimbPos.z, 1.0f, 5.0f);
        if (fabsf(i_this->current.pos.y - (i_this->mTargetPos.y - 10.0f)) < 1.0f) {
            i_this->mMode++;
            i_this->mTimers[0] = 5;
        }
        break;

    case 2:
        i_this->current.angle.x = 0;
        cLib_addCalc2(&i_this->current.pos.x, i_this->mTargetPos.x, 1.0f, 5.0f);
        cLib_addCalc2(&i_this->current.pos.y, i_this->mTargetPos.y, 1.0f, 5.0f);
        cLib_addCalc2(&i_this->current.pos.z, i_this->mTargetPos.z, 1.0f, 5.0f);
        delta = i_this->current.pos - i_this->mTargetPos;
        if (i_this->mTimers[0] == 1) {
            anm_init(i_this, npc_ne_class::ANM_JUMP_END, 3.0f, 0, 1.0f);
        }
        if (delta.abs() < 1.0f) {
            i_this->mMode++;
            if (i_this->mAnmID != npc_ne_class::ANM_JUMP_END) {
                anm_init(i_this, npc_ne_class::ANM_JUMP_END, 2.0f, 0, 2.0f);
            }
        }
        break;

    case 3:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = npc_ne_class::ACT_AWAY;
            i_this->mMode = 0;
            i_this->mTimers[3] = 0;
            i_this->mTimers[2] = 0;
            i_this->mTimers[1] = 0;
            i_this->mTimers[0] = 0;
        }
        break;
    }

    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, 0x800);
    i_this->speed.y = 0.0f;
    i_this->gravity = 0.0f;
}

static BOOL npc_ne_climb(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    BOOL ret = false;
    i_this->mCcDisableTimer = 10;
    cXyz vec1, vec2, delta;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_RUN, 3.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        // fallthrough

    case 1:
        ret = true;
        cLib_addCalc2(&_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed,
                      1.0f, 0.5f * l_HIO.mRunSpeed);
        delta = i_this->mClimbPos - _this->current.pos;
        if (delta.abs() < 40.0f || i_this->mAcch.ChkWallHit() ||
            fopAcM_wayBgCheck(_this, 35.0f * i_this->mBaseScale.z, 30.0f * i_this->mBaseScale.y))
        {
            s16 wall_angle = wall_angle_get(i_this);
            if (wall_angle != 1) {
                anm_init(i_this, npc_ne_class::ANM_TO_JUMP, 5.0f, 0, 1.0f);
                _this->current.angle.y = wall_angle;
                _this->current.pos = _this->old.pos;
                _this->speedF = 0.0f;
                i_this->mMode++;
            } else {
                i_this->mAction = npc_ne_class::ACT_WAIT;
                i_this->mMode = 0;
            }
        }
        break;

    case 2:
        i_this->mLookUpTimer = 2;
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, npc_ne_class::ANM_RUN, 5.0f, 2, 2.0f);
            i_this->mMode++;
            i_this->mTimers[0] = 50;
        }
        break;

    case 3:
        _this->current.angle.x = climb_angle_get(i_this);
        if (i_this->mTimers[0] >= 47) {
            _this->current.angle.x = -0x3800;
        }
        cLib_addCalc2(&_this->current.pos.y, i_this->mTargetPos.y, 1.0f, 15.0f);
        if (_this->current.angle.x <= -0x2000) {
            cLib_addCalc2(&_this->current.pos.x, i_this->mClimbPos.x, 1.0f, 15.0f);
            cLib_addCalc2(&_this->current.pos.z, i_this->mClimbPos.z, 1.0f, 15.0f);
            break;
        } else {
            mDoMtx_YrotS(*calc_mtx, _this->current.angle.y);
            vec1.x = 0.0f;
            vec1.y = 100.0f;
            vec1.z = 40.0f * i_this->mBaseScale.z;
            MtxPosition(&vec1, &vec2);
            vec2 += _this->current.pos;
            dBgS_GndChk gnd_chk;
            gnd_chk.SetPos(&vec2);
            i_this->mTargetPos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            i_this->mTargetPos.x = vec2.x;
            i_this->mTargetPos.z = vec2.z;
            i_this->mMode = 4;
        }
        // fallthrough

    case 4:
        i_this->current.angle.x = climb_angle_get(i_this);
        cLib_addCalc2(&_this->current.pos.x, i_this->mTargetPos.x, 1.0f, 15.0f);
        cLib_addCalc2(&_this->current.pos.y, i_this->mTargetPos.y, 1.0f, 15.0f);
        cLib_addCalc2(&_this->current.pos.z, i_this->mTargetPos.z, 1.0f, 15.0f);
        delta = i_this->mTargetPos - _this->current.pos;
        if (delta.abs() < 30.0f || i_this->mTimers[0] == 0) {
            anm_init(i_this, npc_ne_class::ANM_JUMP_END, 2.0f, 0, 1.0f);
            i_this->mMode = 5;
        }
        break;

    case 5:
        cLib_addCalc2(&_this->current.pos.x, i_this->mTargetPos.x, 0.2f, 15.0f);
        cLib_addCalc2(&_this->current.pos.y, i_this->mTargetPos.y, 1.0f, 15.0f);
        cLib_addCalc2(&_this->current.pos.z, i_this->mTargetPos.z, 0.2f, 15.0f);
        if (i_this->mpMorf->isStop()) {
            if (i_this->mBehavior == npc_ne_class::BHV_PIER) {
                i_this->mAction = npc_ne_class::ACT_HOME;
            } else {
                i_this->mAction = npc_ne_class::ACT_WAIT;
            }
            i_this->mMode = 0;
        }
        break;
    }

    if (!ret) {
        _this->speed.y = 0.0f;
        _this->speedF = 0.0f;
        _this->gravity = 0.0f;
    }
    return ret;
}

static void npc_ne_drop(npc_ne_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        i_this->mMode++;
        i_this->speedF = 0.0f;
        i_this->speed.y = 0.0f;
        anm_init(i_this, npc_ne_class::ANM_JUMP_END, 3.0f, 0, 0.0f);
        // fallthrough

    case 1:
        i_this->current.angle.x = 0x3000;
        if (i_this->mAcch.ChkGroundHit()) {
            i_this->shape_angle.x = 0;
            i_this->current.angle.x = 0;
            i_this->mpMorf->setPlaySpeed(2.0f);
            i_this->mTimers[0] = 10;
            i_this->mMode++;
        }
        break;

    case 2:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
            i_this->mNoFollow = true;
        }
        break;
    }
}

static void npc_ne_s_drop(npc_ne_class* i_this) {
    int frame = i_this->mpMorf->getFrame();
    f32 target_speed = 0.0f;
    f32 max_speed_step = 3.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        anm_init(i_this, npc_ne_class::ANM_TO_JUMP, 3.0f, 0, 1.0f);
        i_this->mMode++;
        break;

    case 1:
        i_this->mLookDownTimer = 3;
        i_this->speed.y = 0.0f;
        i_this->speedF = 0.0f;
        i_this->gravity = 0.0f;
        if (!i_this->mpMorf->isStop()) {
            break;
        }
        anm_init(i_this, npc_ne_class::ANM_JUMP_START, 2.0f, 0, 1.0f);
        i_this->mMode++;
        i_this->mTimers[0] = 3;
        i_this->shape_angle.x = 0x1c00;
        // fallthrough

    case 2:
        i_this->current.angle.x = 0x3800;
        if (frame >= 1) {
            max_speed_step = target_speed = 10.0f;
        } else {
            max_speed_step = target_speed = 2.0f;
        }
        if (i_this->mTimers[0] == 0 && i_this->mAcch.ChkGroundHit()) {
            anm_init(i_this, npc_ne_class::ANM_JUMP_END, 3.0f, 0, 1.0f);
            i_this->mpMorf->setFrame(4.0f);
            i_this->mMode++;
        }
        break;

    case 3:
        max_speed_step = 1.0f;
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, max_speed_step);
}

static BOOL npc_ne_carry(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    BOOL ret = false;
    i_this->mCcDisableTimer = 5;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_TO_CARRY_B, 1.0f, 0,
                 daPy_getLinkPlayerActorClass()->getBaseAnimeFrameRate());
        i_this->mpMorf->setFrame(daPy_getLinkPlayerActorClass()->getBaseAnimeFrame());
        i_this->mTimers[0] = 20;
        i_this->mMode++;
        break;

    case 1:
        if (i_this->mpMorf->checkFrame(10.0f)) {
            i_this->mSound.startSound(Z2SE_CAT_CRY_CARRY, 0, -1);
        }
        if (daPy_getLinkPlayerActorClass()->getGrabPutStart()) {
            anm_init(i_this, npc_ne_class::ANM_TO_CARRY_B, 1.0f, 0,
                    daPy_getLinkPlayerActorClass()->getBaseAnimeFrameRate());
            i_this->mpMorf->setFrame(daPy_getLinkPlayerActorClass()->getBaseAnimeFrame());
            i_this->mMode++;
        }
        break;
    }

    i_this->mTailSwayTarget = 1300.0f;

    if (!fopAcM_checkCarryNow(_this)) {
        i_this->mAction = npc_ne_class::ACT_TAME;
        i_this->mMode = 0;
        dBgS_GndChk gnd_chk;
        cXyz pos = _this->current.pos;
        pos.y += 50.0f;
        gnd_chk.SetPos(&pos);
        if (fabsf(_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk)) < 30.0f) {
            _this->current.pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            i_this->mGroundY = _this->current.pos.y;
            _this->gravity = 0.0f;
        } else {
            i_this->mAction = npc_ne_class::ACT_DROP;
            i_this->mMode = 0;
            i_this->mSound.startSound(Z2SE_CAT_CRY_ANNOY, 0, -1);
            _this->speed.y = -10.0f;
            return false;
        }
    }

    _this->speedF = 0.0f;
    if (i_this->mTimers[0] != 0) {
        ret = true;
        _this->speed.y = -0.1f;
    } else {
        _this->speed.y = 0.0f;
    }
    i_this->mCarryTimer = 10;

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    dBgS_LinChk lin_chk;
    cXyz pos = _this->current.pos;
    pos.y += 2.0f;
    lin_chk.Set(&player->eyePos, &pos, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        _this->current.pos = lin_chk.GetCross();
    }

    return ret;
}

static void npc_ne_dish(npc_ne_class* i_this) {
    i_this->mCcDisableTimer = 10;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_MILK, 1.0f, 2, 1.0f);
        i_this->mMode++;
        // fallthrough

    case 1:
    default:
             /* dSv_event_flag_c::F_0038 - Ordon Village - Opening (2nd day) cat returns home */
        if (!dComIfGs_isEventBit(0x1001)) {
            i_this->current.pos.y = 20000.0f;
            i_this->speed.y = 0.0f;
            fopAcM_OffStatus(i_this, fopAcStts_CULL_e);
        }
        break;
    }
}

static void npc_ne_message(npc_ne_class* i_this) {
    i_this->mLookMode = npc_ne_class::LOOK_PLAYER;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, npc_ne_class::ANM_WAIT, 3.0f, 2, 1.0f);
        i_this->mMode++;
        // fallthrough

    case 1:
    default:
        cLib_addCalc0(&i_this->speedF, 1.0f, 2.0f);
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 2, 0x1000);
        if (!i_this->mIsTalking && i_this->mDistToTarget > 400.0f) {
            i_this->mAction = npc_ne_class::ACT_WAIT;
            i_this->mMode = 0;
        }
        break;
    }
}

static void action(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));

    _this->gravity = -7.0f;

    if (i_this->mResName == "Npc_net") {
        i_this->mDistToTarget = 10000.0f;
    } else {
        i_this->mDistToTarget = i_this->mDistScale * fopAcM_searchPlayerDistance(_this);
    }
    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        i_this->mDistToTarget -= 100.0f;
    }
    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(_this);

    i_this->field_0x6b8 = fabsf(player->eyePos.y - player->current.pos.y);
    if (fabsf(i_this->current.pos.y - player->current.pos.y) > 50.0f) {
        i_this->field_0x6b8 = 100.0f;
    }
    i_this->field_0x6b8 *= 0.7f;

    BOOL ground_check = true;
    int water_check_state = 1;
    BOOL drop_check = true;
    int water_state = 0;
    BOOL carry_check = true;
    i_this->mCcSph.OnCoSetBit();

    if (i_this->field_0x65c < 1.0f) {
        BOOL bird_check = false;
        i_this->field_0x560 = 1;
        i_this->mMessageState = 0;

        switch (i_this->mAction) {
        case npc_ne_class::ACT_WAIT:
            i_this->mMessageState = 1;
            npc_ne_wait(i_this);
            bird_check = true;
            break;
        case npc_ne_class::ACT_AWAY:
            npc_ne_away(i_this);
            bird_check = true;
            i_this->mMessageState = 1;
            break;
        case npc_ne_class::ACT_TAME:
            npc_ne_tame(i_this);
            bird_check = true;
            i_this->mMessageState = 1;
            break;
        case npc_ne_class::ACT_HOME:
            ground_check = npc_ne_home(i_this);
            drop_check = false;
            break;
        case npc_ne_class::ACT_ROOF:
            npc_ne_roof(i_this);
            drop_check = true;
            break;
        case npc_ne_class::ACT_BIRD:
            npc_ne_bird(i_this);
            break;
        case npc_ne_class::ACT_BALL:
            i_this->field_0x560 = 0x23;
            npc_ne_ball(i_this);
            break;
        case npc_ne_class::ACT_SANBASI:
            npc_ne_sanbasi(i_this);
            drop_check = false;
            carry_check = false;
            break;
        case npc_ne_class::ACT_JUMP:
            npc_ne_jump(i_this);
            carry_check = false;
            break;
        case npc_ne_class::ACT_S_JUMP:
            npc_ne_s_jump(i_this);
            carry_check = false;
            break;
        case npc_ne_class::ACT_PATH_WALK:
            npc_ne_pathwalk(i_this);
            bird_check = true;
            i_this->mMessageState = 1;
            break;
        case npc_ne_class::ACT_SWIM:
            npc_ne_swim(i_this);
            water_check_state = 2;
            water_state = 2;
            drop_check = false;
            fopAcM_riverStream(&_this->current.pos, &i_this->field_0xcb6,
                               &i_this->field_0xcb8, 0.5f);
            break;
        case npc_ne_class::ACT_OUT_SWIM:
            npc_ne_outswim(i_this);
            water_check_state = 0;
            ground_check = false;
            drop_check = false;
            break;
        case npc_ne_class::ACT_CLIMB:
            ground_check = npc_ne_climb(i_this);
            water_check_state = 0;
            drop_check = false;
            break;
        case npc_ne_class::ACT_DROP:
            npc_ne_drop(i_this);
            drop_check = false;
            carry_check = false;
            break;
        case npc_ne_class::ACT_S_DROP:
            npc_ne_s_drop(i_this);
            drop_check = false;
            carry_check = false;
            break;
        case npc_ne_class::ACT_MESSAGE:
            npc_ne_message(i_this);
            i_this->mMessageState = 2;
            break;
        case npc_ne_class::ACT_DISH:
            npc_ne_dish(i_this);
            water_check_state = 0;
            drop_check = false;
            carry_check = false;
            break;
        case npc_ne_class::ACT_CARRY:
            ground_check = npc_ne_carry(i_this);
            water_check_state = 0;
            drop_check = false;
            i_this->mCcSph.OffCoSetBit();
            carry_check = false;
            break;
        }

        if (i_this->mResName != "Npc_net") {
            if (i_this->mMessageState == 1 && daPy_py_c::checkNowWolf()
                                         && i_this->mDistToTarget < 300.0f) {
                i_this->mAction = npc_ne_class::ACT_MESSAGE;
                i_this->mMode = 0;
                bird_check = false;
            }

            if (bird_check && !i_this->mWantsFish) {
                fopAc_ac_c* bird_or_ball = search_bird(i_this);
                if (bird_or_ball != NULL) {
                    if (fopAcM_GetName(bird_or_ball) == PROC_OBJ_FOOD) {
                        i_this->mAction = npc_ne_class::ACT_BALL;
                        i_this->mMode = 0;
                        carry_check = true;
                    } else {
                        i_this->mAction = npc_ne_class::ACT_BIRD;
                        i_this->mMode = 0;
                        carry_check = true;
                    }
                }
            }
        }
    }

    if (carry_check && !i_this->mWantsFish) {
        cLib_onBit<u32>(_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        if (ne_carry_check(i_this)) {
            return;
        }
    } else {
        cLib_offBit<u32>(_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    cXyz vec1, vec2;
    if (i_this->mWantsFish && i_this->mAction != npc_ne_class::ACT_SANBASI
                            && player->current.pos.z > -2800.0f) {
        dmg_rod_class* rod = ((dmg_rod_class*)fopAcM_SearchByName(PROC_MG_ROD));
        if (rod != NULL && rod->kind == 1 && rod->action != 5
                        && rod->play_cam_mode != 0 && !i_this->mNoFollow) {
            if (i_this->mDistToTarget > 500.0f) {
                _this->current.pos = ground_search(i_this);
                _this->old = _this->current;
            }
        } else if (!i_this->mNoFollow) {
            if (i_this->mDistToTarget > 700.0f && fopAcM_CheckCondition(_this, 4)
                                             && player->speedF > 2.0f) {
                camera_class* camera = dComIfGp_getCamera(0);
                vec1.x = camera->lookat.eye.x - camera->lookat.center.x;
                vec1.z = camera->lookat.eye.z - camera->lookat.center.z;
                mDoMtx_YrotS(*calc_mtx, cM_atan2s(vec1.x, vec1.z));
                vec1.set(0.0f, -50.0f, 100.0f);
                MtxPosition(&vec1, &vec2);
                vec2 += camera->lookat.eye;
                dBgS_ObjGndChk_Spl gnd_chk_spl;
                gnd_chk_spl.SetPos(&vec2);
                f32 cross = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
                dBgS_GndChk gnd_chk;
                gnd_chk.SetPos(&vec2);
                vec2.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
                if (vec2.y >= cross && fabsf(vec2.y - player->current.pos.y) < 100.0f) {
                    if (i_this->mAction != npc_ne_class::ACT_TAME) {
                        i_this->mAction = npc_ne_class::ACT_TAME;
                        i_this->mMode = 0;
                    }
                    _this->current.pos = vec2;
                    _this->old = _this->current;
                }
            }
        } else {
            if (player->current.pos.y > 500.0f) {
                i_this->mAction = npc_ne_class::ACT_SANBASI;
                i_this->mMode = 0;
                _this->current.pos.set(3400.0f, 84.13f, 64.0f);
                _this->current.angle.y = -0x6987;
                _this->shape_angle.y = _this->current.angle.y;
                _this->old = _this->home = _this->current;
                _this->speedF = 0.0f;
            }
        }
    }

    cLib_addCalcAngleS2(&_this->current.angle.x, 0, 1, 0x400);
    mDoMtx_YrotS(*calc_mtx, _this->current.angle.y);
    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = _this->speedF * i_this->mBaseScale.z;
    MtxPosition(&vec1, &vec2);
    _this->speed.x = vec2.x;
    _this->speed.z = vec2.z;
    _this->speed.y += _this->gravity;
    _this->current.pos += _this->speed;
    if (_this->speed.y < -120.0f) {
        _this->speed.y = -120.0f;
    }
    if (!fopAcM_checkCarryNow(_this) && i_this->mAction != npc_ne_class::ACT_PATH_WALK) {
        cXyz* cc_move = i_this->mStts.GetCCMoveP();
        if (cc_move != NULL) {
            _this->current.pos.x += cc_move->x * 0.1f;
            _this->current.pos.y += cc_move->y * 0.1f;
            _this->current.pos.z += cc_move->z * 0.1f;
        }
    }

    if (i_this->field_0x65c > 1.0f) {
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = -i_this->field_0x65c;
        mDoMtx_YrotS(*calc_mtx, i_this->field_0x660);
        MtxPosition(&vec1, &vec2);
        _this->current.pos += vec2;
        cLib_addCalc0(&i_this->field_0x65c, 1.0f, 6.0f);
        _this->speedF = 0.0f;
    }

    if (ground_check) {
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        i_this->mGroundY = i_this->mAcch.GetGroundH();
    } else {
        i_this->mGroundY = _this->current.pos.y;
    }
    if (ground_check && i_this->mAcch.ChkGroundHit()) {
        dBgS_GndChk gnd_chk;
        mDoMtx_YrotS(*calc_mtx, _this->shape_angle.y);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = i_this->mBaseScale.z * -25.0f;
        MtxPosition(&vec1, &vec2);
        vec2 += _this->current.pos;
        Vec gnd_chk_pos;
        gnd_chk_pos.x = vec2.x;
        gnd_chk_pos.y = vec2.y + 100.0f;
        gnd_chk_pos.z = vec2.z;
        gnd_chk.SetPos(&gnd_chk_pos);
        vec2.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        vec1 = vec2 - _this->current.pos;
        if (fabsf(vec1.y) < 50.0f) {
            f32 vec1_xz = JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z);
            _this->current.angle.x = cM_atan2s(vec1.y, vec1_xz);
        }
    }

    cLib_addCalcAngleS2(&_this->shape_angle.y, _this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&_this->shape_angle.x, _this->current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&_this->shape_angle.z, _this->current.angle.z, 4, 0x1000);
    _this->current.angle.z = 0;
    cLib_addCalcAngleS2(&i_this->mBackboneAngleY, i_this->mBackboneTargetAngleY, 2, 0x400);

    if (drop_check) {
        f32 height = _this->current.pos.y - i_this->mGroundY;
        if (height > 300.0f) {
            i_this->mAction = npc_ne_class::ACT_DROP;
            i_this->mMode = 0;
            i_this->mSound.startSound(Z2SE_CAT_CRY_ANNOY, 0, -1);
        } else if (height > 30.0f && i_this->mAction != npc_ne_class::ACT_ROOF) {
            _this->current.pos = _this->old.pos;
            i_this->mAction = npc_ne_class::ACT_S_DROP;
            i_this->mMode = 0;
        }
    }

    if (water_check_state == 1) {
        if (water_check(i_this, 5.0f) > 0) {
            i_this->mAction = npc_ne_class::ACT_SWIM;
            i_this->mMode = 0;
            i_this->mNoFollow = true;
            water_state = 1;
            if (i_this->mBehavior == npc_ne_class::BHV_PIER) {
                i_this->mBehavior = npc_ne_class::BHV_NONE;
            }
            i_this->field_0x65c = 0.0f;
        }
    } else if (water_check_state == 2) {
        if (water_check(i_this, 5.0f) != 1) {
            i_this->mAction = npc_ne_class::ACT_AWAY;
            i_this->mMode = 5;
            i_this->mTimers[2] = cM_rndF(5.0f) + 10.0f;
            i_this->mTimers[3] = 30;
            i_this->mTargetAngleY = _this->shape_angle.y;
            i_this->mTimers[0] = cM_rndF(5.0f) + 10.0f;
            i_this->mTimers[1] = 20;
        }
    }

    if (i_this->mLookNoMoveTimer != 0) {
        i_this->mLookNoMoveTimer--;
        i_this->mLookMode = npc_ne_class::LOOK_NONE;
    }
    if (i_this->mLookDownTimer != 0) {
        i_this->mLookDownTimer--;
        i_this->mLookMode = npc_ne_class::LOOK_TARGET;
        mDoMtx_YrotS(*calc_mtx, _this->current.angle.y);
        vec1.set(0.0f, -30.0f, 50.0f);
        MtxPosition(&vec1, &i_this->mLookTarget);
        i_this->mLookTarget += _this->current.pos;
    }
    if (i_this->mLookUpTimer != 0) {
        i_this->mLookUpTimer--;
        i_this->mLookMode = npc_ne_class::LOOK_TARGET;
        mDoMtx_YrotS(*calc_mtx, _this->current.angle.y);
        vec1.set(0.0f, 30.0f, 50.0f);
        MtxPosition(&vec1, &i_this->mLookTarget);
        i_this->mLookTarget += _this->current.pos;
    }

    if (i_this->mBackboneAngleY < 0x800 && i_this->mBackboneAngleY > -0x800
                                    && i_this->mLookMode != npc_ne_class::LOOK_FORWARD) {
        if ((i_this->mCounter & 0xf) == 0 || i_this->mLookMode <= npc_ne_class::LOOK_TARGET) {
            if (i_this->mLookMode == npc_ne_class::LOOK_TARGET) {
                vec1 = i_this->mLookTarget - i_this->current.pos;
            } else {
                vec1 = player->eyePos - i_this->current.pos;
                if (!daPy_py_c::checkNowWolf()) {
                    vec1.y += i_this->mBaseScale.z * -40.0f;
                }
            }
            i_this->mHeadTargetAngleY = cM_atan2s(vec1.x, vec1.z) - _this->shape_angle.y;
            f32 vec1_xz = JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z);
            i_this->mHeadTargetAngleZ = cM_atan2s(vec1.y, vec1_xz) + _this->shape_angle.x;
            if (i_this->mHeadTargetAngleY > 15000) {
                i_this->mHeadTargetAngleY = 15000;
            } else if (i_this->mHeadTargetAngleY < -15000) {
                i_this->mHeadTargetAngleY = -15000;
            }
            if (i_this->mHeadTargetAngleZ > 15000) {
                i_this->mHeadTargetAngleZ = 15000;
            } else if (i_this->mHeadTargetAngleZ < -15000) {
                i_this->mHeadTargetAngleZ = -15000;
            }
        }
        i_this->mLookMode = npc_ne_class::LOOK_FORWARD;
    } else {
        i_this->mHeadTargetAngleZ = 0;
        i_this->mHeadTargetAngleY = 0;
    }

    i_this->mBackboneTargetAngleY = 0;
    cLib_addCalcAngleS2(&i_this->mHeadAngleY, i_this->mHeadTargetAngleY, 4, 0x1000);
    cLib_addCalcAngleS2(&i_this->mHeadAngleZ, i_this->mHeadTargetAngleZ, 4, 0x1000);
    cLib_addCalcAngleS2(&i_this->mHeadBobAngleY, i_this->mHeadBobTargetAngleY, 4, 0x400);
    cLib_addCalcAngleS2(&i_this->mHeadBobAngleX, i_this->mHeadBobTargetAngleX, 4, 0x400);

    if (fabsf(player->speedF) < 2.0f) {
        if (i_this->mHeadBobTimer != 0) {
            i_this->mHeadBobTimer--;
            if (i_this->mHeadBobTimer == 0) {
                i_this->mHeadBobTimer = cM_rndF(20.0f) + 20.0f;
                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mHeadBobTargetAngleY = cM_rndFX(1500.0f);
                }
                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mHeadBobTargetAngleX = cM_rndFX(1500.0f);
                }
            }
        }
    } else {
        i_this->mHeadBobTargetAngleX = 0;
        i_this->mHeadBobTargetAngleY = 0;
        i_this->mHeadBobTimer = cM_rndF(40.0f) + 40.0f;
    }

    for (int i = 0; i < 2; i++) {
        i_this->mTailSwayAngle[i] =
            i_this->mTailSway * cM_ssin(i_this->mCounter * 2000 + i * -22000);
    }
    cLib_addCalc2(&i_this->mTailSway, i_this->mTailSwayTarget, 0.1f, 100.0f);
    i_this->mTailSwayTarget = 0.0f;
    cLib_addCalcAngleS2(&i_this->mTailAngle, i_this->mTailTargetAngle, 8, 1000);
    i_this->mTailTargetAngle = 0;

    if (water_state != 0) {
        cXyz splash_pos = _this->eyePos;
        splash_pos.y = i_this->mWaterY;
        if (water_state == 1) {
            if (i_this->mSplashTimer == 0) {
                i_this->mSplashTimer = 20;
                fopKyM_createWpillar(&splash_pos, 0.7f, 0);
                i_this->mSound.startSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
            }
        } else {
            fopAcM_effHamonSet(i_this->mParticle, &splash_pos, 0.7f, 0.1f);
        }
    }

    if (i_this->mBtpReset != 0) {
        cLib_addCalcAngleS2(&i_this->mBtpFrame, 3, 1, 1);
        i_this->mBtpReset = 0;
        i_this->mBtpTimer = 60;
    } else {
        if (i_this->mBtpTimer == 0) {
            i_this->mBtpTimer = cM_rndF(100.0f) + 30.0f;
        } else {
            i_this->mBtpTimer--;
            if (i_this->mBtpTimer <= 5) {
                i_this->mBtpFrame = 5 - i_this->mBtpTimer;
            } else {
                i_this->mBtpFrame = 0;
            }
        }
    }
}

static void demo_camera(npc_ne_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    if (i_this->mDemoTimer != 0) {
        i_this->mDemoTimer--;
    }

    dBgS_LinChk lin_chk;
    dBgS_GndChk gnd_chk;
    fopAc_ac_c* fish;
    cXyz vec, eye, center;
    BOOL bvar1 = false;
    BOOL bird_check = false;

    switch (i_this->mDemoMode) {
    case 0:
        break;

    case 1:
        if (!_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(_this, 2, 0xffff, 0);
            _this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 2;
        i_this->mDemoCounter = 0;
        i_this->mCameraFovY = 55.0f;
        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        // fallthrough

    case 2:
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&player->current.pos,
                                                         i_this->mAngleToPlayer + 0x8000, 0);
        
        center = _this->current.pos;
        eye = _this->current.pos;
        eye.y += 200.0f;
        eye.z += 500.0f;

        if (i_this->mDemoTimer == 0) {
            i_this->mCameraEye1 = eye;
            i_this->mCameraCenter1 = center;
        } else {
            cLib_addCalc2(&i_this->mCameraEye1.x, eye.x, 0.1f, 30.0f);
            cLib_addCalc2(&i_this->mCameraEye1.y, eye.y, 0.1f, 30.0f);
            cLib_addCalc2(&i_this->mCameraEye1.z, eye.z, 0.1f, 30.0f);
            cLib_addCalc2(&i_this->mCameraCenter1.x, center.x, 0.2f, 30.0f);
            cLib_addCalc2(&i_this->mCameraCenter1.y, center.y, 0.2f, 30.0f);
            cLib_addCalc2(&i_this->mCameraCenter1.z, center.z, 0.2f, 30.0f);
        }
        break;

    case 3:
        if (i_this->mDemoCounter == 0) {
            i_this->mCameraCenter1.set(387.0f, 133.0f, -866.0f);
            i_this->mCameraEye1.set(284.0f, 208.0f, -585.0f);
        }

        if (i_this->mDemoCounter == 12) {
            fopAc_ac_c* door = fopAcM_SearchByName(PROC_OBJ_NDOOR);
            if (door != NULL) {
                door->field_0x567 = 10;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CAT_DOOR, &door->current.pos,
                                         0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
            Z2GetAudioMgr()->changeBgmStatus(0);
        }

        cLib_addCalc2(&i_this->mCameraFovY, 35.0f, 0.1f, 0.33f);

        if (i_this->field_0xcc0 && i_this->mDemoCounter == 52) {
            Z2GetAudioMgr()->seStart(Z2SE_READ_RIDDLE_B, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }

        if (i_this->mDemoCounter > 110) {
            i_this->mDemoMode = 99;
        }
        break;

    case 10:
        if (!_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(_this, 2, 0xffff, 0);
            _this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 11;
        i_this->mDemoCounter = 0;
        i_this->mCameraFovY = 45.0f;
        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        // fallthrough

    case 11:
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&player->current.pos,
                                                         i_this->mAngleToPlayer + 0x8000, 0);
        
        mDoMtx_YrotS(*calc_mtx, _this->current.angle.y);

        if (i_this->mMode >= 13) {
            center = _this->current.pos;
            center.y += 20.0f;
        } else {
            vec.x = 0.0f;
            vec.y = 0.0f;
            vec.z = 100.0f;
            MtxPosition(&vec, &center);
            center += _this->current.pos;
        }

        if (i_this->mDemoCounter <= 38) {
            mg_fish_class* fish = (mg_fish_class*)fopAcM_SearchByID(i_this->mFishID);
            if (i_this->mDemoCounter == 0) {
                vec.x = 0.0f;
                vec.y = 0.0f;
                vec.z = 200.0f;
                MtxPosition(&vec, &i_this->mDemoFishPos);
                i_this->mDemoFishPos += i_this->current.pos;
            }
            fish->actor.current.pos.x = i_this->mDemoFishPos.x;
            fish->actor.current.pos.z = i_this->mDemoFishPos.z;
            if (i_this->mDemoCounter >= 25) {
                cLib_addCalc2(&fish->actor.current.pos.y, i_this->mAcch.GetGroundH() + 10.0f,
                              0.2f, 30.0f);
            }
            if (i_this->mDemoCounter == 38) {
                fish->field_0xc3c = 1;
                i_this->field_0xcc0 = true;
                Z2GetAudioMgr()->changeBgmStatus(1);
            }
        }

        if (i_this->mDemoCounter == 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                vec.x = 50.0f;
            } else {
                vec.x = -50.0f;
            }
            vec.y = 150.0f;
            vec.z = 350.0f;
            MtxPosition(&vec, &i_this->mCameraEye1);
            i_this->mCameraEye1 += _this->current.pos;
            i_this->mCameraCenter1 = center;
        } else {
            cLib_addCalc2(&i_this->mCameraCenter1.x, center.x, 0.2f, 30.0f);
            cLib_addCalc2(&i_this->mCameraCenter1.y, center.y, 0.2f, 30.0f);
            cLib_addCalc2(&i_this->mCameraCenter1.z, center.z, 0.2f, 30.0f);
            if (i_this->mMode >= 13) {
                cLib_addCalc2(&i_this->mCameraFovY, 45.0f, 0.03f, 0.2f);
                i_this->mCameraEye1.x += 0.2f * _this->speed.x;
                i_this->mCameraEye1.z += 0.2f * _this->speed.z;
            } else {
                cLib_addCalc2(&i_this->mCameraFovY, 55.0f, 0.05f, 0.2f);
            }

            if (home_path[i_this->mHomePathIdx].mNo > 3) {
                bvar1 = true;
            }

            if (i_this->mDemoCounter >= 80 && i_this->mDemoCounter <= 120) {
                bird_check = true;
                if (i_this->mDemoCounter == 80) {
                    i_this->mCameraCenter2 = player->eyePos;
                    i_this->mCameraCenter2.y += -10.0f;
                    mDoMtx_YrotS(*calc_mtx, player->shape_angle.y);
                    vec.x = 30.0f;
                    vec.y = 130.0f;
                    vec.z = 120.0f;
                    MtxPosition(&vec, &i_this->mCameraEye2);
                    i_this->mCameraEye2 += player->current.pos;
                    player->changeDemoParam2(2);
                } else if (i_this->mDemoCounter == 120) {
                    player->changeDemoParam2(0);
                }
            }
        }
        break;

    case 99:
        fopAcM_delete(_this);
        fish = fopAcM_SearchByID(i_this->mFishID);
        if (fish != NULL) {
            fopAcM_delete(fish);
        }
        // fallthrough

    case 100:
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        i_this->mDemoMode = 0;
        break;
    }

    if (i_this->mDemoMode != 0) {
        if (bird_check) {
            camera->mCamera.Set(i_this->mCameraCenter2, i_this->mCameraEye2,
                                i_this->mCameraFovY, 0);
        } else {
            if (bvar1) {
                lin_chk.Set(&i_this->mCameraCenter1, &i_this->mCameraEye1, _this);
                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    i_this->field_0xcb4 = true;
                    i_this->mDemoCounter = 122;
                }
            }

            if (i_this->field_0xcb4) {
                i_this->field_0xcb4 = false;
                mDoMtx_YrotS(*calc_mtx, _this->current.angle.y);
                vec.x = cM_rndF(50.0f) + 100.0f;
                if (cM_rndF(1.0f) < 0.5f) {
                    vec.x *= -1.0f;
                }
                vec.y = cM_rndF(100.0f) + 170.0f;
                if (home_path[i_this->mHomePathIdx].mNo > 3) {
                    vec.z = cM_rndFX(100.0f) + 200.0f;
                } else {
                    vec.z = cM_rndF(100.0f) + -300.0f;
                }
                MtxPosition(&vec, &i_this->mCameraEye1);
                i_this->mCameraEye1 += _this->current.pos;
                i_this->mCameraCenter1 = _this->current.pos;
                i_this->mCameraCenter1.y += 20.0f;
                i_this->mCameraFovY = 55.0f;
            }

            camera->mCamera.Set(i_this->mCameraCenter1, i_this->mCameraEye1,
                                i_this->mCameraFovY, 0);
        }

        i_this->mDemoCounter++;
        
        if (i_this->mDemoMode < 10 && i_this->mAction != npc_ne_class::ACT_HOME) {
            i_this->mDemoMode = 100;
        }
    }
}

static int message(npc_ne_class* i_this) {
    if (i_this->mResName == "Npc_net" && !dComIfGs_wolfeye_effect_check()) {
        i_this->mMessageState = 0;
    }

    if (i_this->mIsTalking) {
        i_this->mCcDisableTimer = 10;
        if (i_this->mMsgFlow.doFlow(i_this, NULL, 0)) {
            dComIfGp_event_reset();
            i_this->mIsTalking = 0;
            if (i_this->mIsGengle == 1 && (i_this->mMsgFlow.getNowMsgNo() == 0x18a1 ||
                                           i_this->mMsgFlow.getNowMsgNo() == 0x18a2)) {
                fopAcM_createItem(&dComIfGp_getPlayer(0)->eyePos, fpcNm_ITEM_SILVER_RUPEE, -1,
                                  fopAcM_GetRoomNo(i_this), NULL, NULL, 3);
            }
        }
        return 1;
    } else {
        if (dComIfGp_event_runCheck() && i_this->eventInfo.checkCommandTalk()) {
            i_this->mMsgFlow.init(i_this, i_this->mFlowID, 0, NULL);
            i_this->mIsTalking = 1;
        }

        if (i_this->mMessageState == 2 && i_this->mFlowID != -1 && daPy_py_c::checkNowWolf() &&
                                !fopAcM_otherBgCheck(daPy_getLinkPlayerActorClass(), i_this)) {
            fopAcM_OnStatus(i_this, 0);
            cLib_onBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
            if (i_this->mResName == "Npc_net") {
                cLib_onBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_UNK_0x800000 | fopAc_AttnFlag_UNK_0x400000);
            }
            i_this->eventInfo.onCondition(dEvtCnd_CANTALK_e);
        } else {
            fopAcM_OffStatus(i_this, 0);
            cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_UNK_0x800000 | fopAc_AttnFlag_UNK_0x400000 | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        }

        return 0;
    }
}

static int daNpc_Ne_Execute(npc_ne_class* i_this) {
    if (cDmrNowMidnaTalk() && !fopAcM_checkCarryNow(i_this)) {
        return 1;
    }

    cXyz vec1(0.0f, 0.0f, 0.0f);
    i_this->mCounter++;

    if (i_this->mWantsFish && (i_this->mCounter & 0xf) == 0) {
            /* dSv_event_flag_c::F_0470 - Fishing Pond - Reserved for fishing */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[470])
                                    && i_this->mDistToTarget < 1500.0f) {
            if (fopAcM_SearchByName(PROC_MG_ROD) != NULL) {
                i_this->mNoFollow = false;
            } else {
                i_this->mNoFollow = true;
            }
        } else {
            i_this->mNoFollow = true;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }
    if (i_this->mCcDisableTimer != 0) {
        i_this->mCcDisableTimer--;
    }
    if (i_this->mCarryTimer != 0) {
        i_this->mCarryTimer--;
    }
    if (i_this->mSplashTimer != 0) {
        i_this->mSplashTimer--;
    }
    i_this->mNeckAngleY = 0;

    action(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    i_this->scale.x = i_this->mBaseScale.x * l_HIO.mScale;
    mDoMtx_stack_c::scaleM(i_this->scale.x, i_this->scale.x, i_this->scale.x);
    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->play(&i_this->eyePos, 0, 0);
    i_this->mpBtkAnm->setFrame(i_this->mBtkFrame);
    i_this->mpBtpAnm->setFrame(i_this->mBtpFrame);
    i_this->mpMorf->modelCalc();

    if (i_this->mBehavior == npc_ne_class::BHV_DISH) {
        mDoMtx_stack_c::transS(i_this->mDishPos.x, i_this->mDishPos.y, i_this->mDishPos.z);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
        i_this->mpDishMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpDishMorf->play(&i_this->eyePos, 0, 0);
        i_this->mpDishMorf->modelCalc();
    }

    PSMTXCopy(model->getAnmMtx(4), *calc_mtx);
    MtxPosition(&vec1, &i_this->eyePos);
    i_this->attention_info.position = i_this->eyePos;
    i_this->attention_info.position.y += i_this->mBaseScale.y * 20.0f;

    PSMTXCopy(model->getAnmMtx(3), *calc_mtx);
    vec1.set(i_this->field_0xbf4 + -30.0f, 0.0f, 0.0f);
    cLib_addCalc0(&i_this->field_0xbf4, 1.0f, 5.0f);
    cXyz center;
    MtxPosition(&vec1, &center);
    if (i_this->mCcDisableTimer != 0) {
        center.y += 100000.0f;
    }
    i_this->mCcSph.SetC(center);
    i_this->mCcSph.SetR(i_this->mBaseScale.z * 30.0f);
    dComIfG_Ccsp()->Set(&i_this->mCcSph);

    cXyz vec3(-20000.0f, 300000.0f, -10000.0f);
    center = i_this->eyePos;
    if (i_this->mAtEnable == 0) {
        center += vec3;
    } else {
        i_this->mAtEnable = 0;
    }
    i_this->mAtSph.SetC(center);
    i_this->mAtSph.SetR(25.0f);
    dComIfG_Ccsp()->Set(&i_this->mAtSph);

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    message(i_this);
    demo_camera(i_this);

    if (i_this->mResName == "Npc_net" && !dComIfGs_wolfeye_effect_check()) {
        static u16 e_name[2] = {0x8497, 0x8498};
        for (int i = 0; i < 2; i++) {
            i_this->mParticle[i] = dComIfGp_particle_set(i_this->mParticle[i], e_name[i],
                                                        &i_this->eyePos, NULL, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mParticle[i]);
            if (emitter != NULL) {
                emitter->setGlobalAlpha(!dComIfGs_wolfeye_effect_check() ? 0xff : 0);
            }
        }
    }

    if (i_this->mFishID != -1 && i_this->field_0xcc0) {
        mg_fish_class* fish = (mg_fish_class*)fopAcM_SearchByID(i_this->mFishID);
        PSMTXCopy(model->getAnmMtx(4), mDoMtx_stack_c::get());
        mDoMtx_stack_c::ZrotM(-19000);
        mDoMtx_stack_c::scaleM(0.5f, 0.5f, 0.5f);
        mDoMtx_stack_c::transM(5.0f, 35.0f, 15.0f);
        fish->mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        int ivar3 = cM_ssin(g_Counter.mTimer * 15000) * 1500.0f;
        for (int i = 0; i <= fish->mNumJoints; i++) {
            fish->jointYaws1[i] = ivar3;
        }
        for (u16 i = 1; i < fish->mpMorf->getModel()->getModelData()->getJointNum(); i++) {
            fish->mpMorf->getModel()->getModelData()
                ->getJointNodePointer(i)->setCallBack(fish->mNodeCallBack);
        }
        fish->mpMorf->play(NULL, 0, 0);
        fish->mpMorf->modelCalc();
        fish->actor.current.pos = i_this->eyePos;
        fish->actor.eyePos = i_this->eyePos;
    }

    if (i_this->current.pos.y - i_this->home.pos.y < -5000.0f && fopAcM_CheckCondition(i_this, 4)) {
        i_this->old.pos = i_this->current.pos = i_this->home.pos;
    }

    return 1;
}

static int daNpc_Ne_IsDelete(npc_ne_class* i_this) {
    return 1;
}

static int daNpc_Ne_Delete(npc_ne_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, i_this->mResName);
    if (i_this->mHIOInit != 0) {
        hio_set = false;
    }
    if (i_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    npc_ne_class* _this = static_cast<npc_ne_class*>(i_this);
    _this->mpMorf =
        new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes(_this->mResName, 28)),
                           NULL, NULL,
                           static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(_this->mResName, 24)),
                           2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11020284);
    if (_this->mpMorf == NULL || _this->mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = _this->mpMorf->getModel();
    i_this->model = model;
    model->setUserArea((uintptr_t)_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i == 1 || i == 2 || i == 3 || i == 4 || i == 20 || i == 21) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    _this->mpBtkAnm = new mDoExt_btkAnm();
    if (_this->mpBtkAnm == NULL) {
        return 0;
    }
    if (FALSE == _this->mpBtkAnm->init(_this->mpMorf->getModel()->getModelData(),
        static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(_this->mResName, 32)), 1, 0, 1.0f, 0, -1)) {
        return 0;
    }

    _this->mpBtpAnm = new mDoExt_btpAnm();
    if (_this->mpBtpAnm == NULL) {
        return 0;
    }
    if (FALSE == _this->mpBtpAnm->init(_this->mpMorf->getModel()->getModelData(),
        static_cast<J3DAnmTexPattern*>(dComIfG_getObjectRes(_this->mResName, 35)), 1, 0, 1.0f, 0, -1)) {
        return 0;
    }

    _this->mSound.init(&i_this->current.pos, 1);

    if (_this->mBehavior == npc_ne_class::BHV_DISH) {
        static int dish_bck[2] = {npc_ne_class::ANM_EMPTY_DISH, npc_ne_class::ANM_DRINK_DISH};
                        /* dSv_event_flag_c::F_0038 - Ordon Village - Opening (2nd day) cat returns home */
        s32 dish_flag;
        if (dComIfGs_isEventBit(0x1001)) {
            dish_flag = 1;
        } else {
            dish_flag = 0;
        }
        _this->mpDishMorf = new mDoExt_McaMorf(
            static_cast<J3DModelData*>(dComIfG_getObjectRes(_this->mResName, 29)), NULL, NULL,
            static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(_this->mResName,
                                                               dish_bck[dish_flag])),
            2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000084);
        if (_this->mpDishMorf == NULL || _this->mpDishMorf->getModel() == NULL) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daNpc_Ne_Create(fopAc_ac_c* i_this) {
    npc_ne_class* _this = static_cast<npc_ne_class*>(i_this);
    fopAcM_ct(i_this, npc_ne_class);

    if (dKy_darkworld_check()) {
        _this->mResName = "Npc_net";
    } else {
        _this->mResName = "Npc_ne";
    }
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhase, _this->mResName);

    if (step == cPhs_COMPLEATE_e) {
        _this->mBehavior = (u8)fopAcM_GetParam(_this);
        _this->mTexture = (u8)(fopAcM_GetParam(_this) >> 8) & 0xf;
        _this->mSize = (u8)(fopAcM_GetParam(_this) >> 0xc) & 0xf;
        _this->mPathIndex = (u8)(fopAcM_GetParam(_this) >> 0x10);
        _this->mIsGengle = (u8)(fopAcM_GetParam(_this) >> 0x18);

        if (_this->mIsGengle == 1 
                /* dSv_event_flag_c::F_0457 - Castle Town - Revived cat */
            && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[457])) {
            return cPhs_ERROR_e;
        }

        _this->mFlowID = i_this->current.angle.x;
        i_this->shape_angle.x = 0;
        i_this->current.angle.x = 0;

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x2a50)) {
            return cPhs_ERROR_e;
        }

        if (_this->mPathIndex != 0xff) {
            _this->mpPath = dPath_GetRoomPath(_this->mPathIndex, fopAcM_GetRoomNo(i_this));
            if (_this->mpPath == NULL) {
                return cPhs_ERROR_e;
            }
            _this->field_0x5b9 = _this->mPathIndex + 1;
            _this->mPathDir = 1;
            _this->mAction = npc_ne_class::ACT_PATH_WALK;
        } else {
            if (_this->mBehavior == npc_ne_class::BHV_TAME) {
                _this->mAction = npc_ne_class::ACT_TAME;
            } else if (_this->mBehavior == npc_ne_class::BHV_PIER) {
                    /* dSv_event_flag_c::F_0038 - Ordon Village - Opening (2nd day) cat returns home */
                if (dComIfGs_isEventBit(0x1001)) {
                    _this->mAction = npc_ne_class::ACT_ROOF;
                    i_this->current.pos.set(1005.0f, 766.0f, -1423.0f);
                    i_this->old = i_this->home = i_this->current;
                    i_this->current.angle.y = 0;
                    _this->mNoFollow = true;
                } else {
                    _this->mAction = npc_ne_class::ACT_SANBASI;
                    i_this->current.pos.set(3400.0f, 84.13f, 64.0f);
                    i_this->current.angle.y = -0x6987;
                    i_this->shape_angle.y = i_this->current.angle.y;
                    i_this->old = _this->home = i_this->current;
                    _this->mNoFollow = true;
                    _this->mWantsFish = 1;
                }
            } else if (_this->mBehavior == npc_ne_class::BHV_DISH) {
                _this->mAction = npc_ne_class::ACT_DISH;
                _this->mDishPos = i_this->current.pos;
                fopAcM_OnStatus(i_this, fopAcStts_NOPAUSE_e);
            }
        }

        if (!hio_set) {
            _this->mHIOInit = 1;
            hio_set = true;
            l_HIO.field_0x04 = -1;
        }

        fopAcM_OnStatus(i_this, fopAcStts_CULL_e);
        fopAcM_OnCarryType(i_this, fopAcM_CARRY_TYPE_8);
        i_this->attention_info.flags = 0;
        i_this->attention_info.distances[fopAc_attn_CARRY_e] = 7;
        fopAcM_SetMtx(i_this, _this->mpMorf->getModel()->getBaseTRMtx());
        i_this->health = 1;
        _this->field_0x560 = 1;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x1f}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 15.0f} // mSph
            } // mSphAttr
        };

        _this->mStts.Init(0x1e, 0, i_this);
        _this->mCcSph.Set(cc_sph_src);
        _this->mCcSph.SetStts(&_this->mStts);
        _this->mAtSph.Set(at_sph_src);
        _this->mAtSph.SetStts(&_this->mStts);
        _this->mCcSph.OnTgNoHitMark();

        if (_this->mTexture == 0xf) {
            _this->mBtkFrame = cM_rndF(3.999f);
        } else {
            _this->mBtkFrame = _this->mTexture;
            if (_this->mBtkFrame > 3) {
                _this->mBtkFrame = 3;
            }
        }
        _this->mCounter = cM_rndF(0x10000);
        if (_this->mSize == 1) {
            _this->mBaseScale.set(0.75f, 0.75f, 0.75f);
            _this->mSearchBall = 1;
        } else if (_this->mSize == 2) {
            _this->mBaseScale.set(0.625f, 0.625f, 0.625f);
            _this->mSearchBall = 1;
        } else {
            _this->mBaseScale.set(1.0f, 1.0f, 1.0f);
        }
        _this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                         &_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        _this->mAcchCir.SetWall(_this->mBaseScale.y * 30.0f, _this->mBaseScale.z * 35.0f);
        _this->mDistScale = cM_rndFX(0.2f) + 1.0f;
        _this->mGroundY = i_this->current.pos.y;
        if (_this->mResName == "Npc_net") {
            _this->mAction = npc_ne_class::ACT_WAIT;
            _this->mMode = 0;
        }
        _this->mFishID = -1;

        daNpc_Ne_Execute(_this);
    }
    return step;
}


npc_ne_class::npc_ne_class() {
    /* empty function */
}

static actor_method_class l_daNpc_Ne_Method = {
    (process_method_func)daNpc_Ne_Create,
    (process_method_func)daNpc_Ne_Delete,
    (process_method_func)daNpc_Ne_Execute,
    (process_method_func)daNpc_Ne_IsDelete,
    (process_method_func)daNpc_Ne_Draw,
};

actor_process_profile_definition g_profile_NPC_NE = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_NPC_NE,
    &g_fpcLf_Method.base,
    sizeof(npc_ne_class),
    0,
    0,
    &g_fopAc_Method.base,
    0x2BC,
    &l_daNpc_Ne_Method,
    0x8044000,
    fopAc_NPC_e,
    fopAc_CULLBOX_0_e,
};

AUDIO_INSTANCES;
