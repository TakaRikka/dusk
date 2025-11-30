/**
 * d_a_do.cpp
 * Dog
 */

// Every function matches, but a Z2SoundObjSimple dtor is generated that should not exist

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "c/c_damagereaction.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_do.h"
#include "d/actor/d_a_obj_food.h"
#include "d/actor/d_a_player.h"
#include "d/d_a_item_static.h"
#include "m_Do/m_Do_controller_pad.h"
#include "Z2AudioLib/Z2Instances.h"

/* 8066F2B6 0002+00 data_8066F2B6 None */
static bool noFallCheck;
static bool hioInit;

static daDo_HIO_c l_HIO;

daDo_HIO_c::daDo_HIO_c() {
    field_0x04 = -1;
    mBaseSize = 1.0f;
    mWalkSpeed = 2.5f;
    mRunSpeed = 12.0f;
    mSwimSpeed = 2.0f;
    mPlayerRecognitionDist = 600.0f;
    mWaitType = 0;
    mSwimming = 1;
    mWaterHuntAnimType = 0;
}

static void anm_init(do_class* i_this, int i_resIdx, f32 i_morf, u8 i_mode, f32 i_speed) {
    if (!(i_this->field_0x608 > 1.0f)) {
        i_this->mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Do", i_resIdx)),
                               i_mode, i_morf, i_speed, 0.0f, -1.0f, 0);
        i_this->mAnmID = i_resIdx;
    }
}

static int nodeCallBack(J3DJoint* i_jntP, int param_1) {
    if (param_1 == 0) {
        int joint_num = i_jntP->getJntNo();
        J3DModel* model = j3dSys.getModel();
        do_class* _this = (do_class*)model->getUserArea();

        if (_this != NULL) {
            MTXCopy(model->getAnmMtx(joint_num), *calc_mtx);

            if (joint_num == 9 || joint_num == 10) {
                cMtx_YrotM(*calc_mtx, _this->mHeadAngle.y + _this->mHeadBob.y);
                cMtx_XrotM(*calc_mtx, _this->mHeadAngle.z + _this->mHeadBob.z);
                cMtx_ZrotM(*calc_mtx, _this->mHeadAngle.x);
            } else if (joint_num == 22) {
                cMtx_YrotM(*calc_mtx, _this->mTailAngle.y << 1);
                cMtx_ZrotM(*calc_mtx, _this->mTailAngle.x << 1);
            } else {
                cMtx_YrotM(*calc_mtx, _this->mTailAngle.y);
                cMtx_ZrotM(*calc_mtx, _this->mTailAngle.x);
            }

            model->setAnmMtx(joint_num, *calc_mtx);
            MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
        }
    }
    return 1;
}

static int daDo_Draw(do_class* i_this) {
    if (i_this->mNoDraw != 0) {
        i_this->mNoDraw = 0;
        return 1;
    }

    if (daPy_py_c::linkGrabSubjectNoDraw(i_this)) {
        return 1;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
    i_this->mpBtk->entry(model->getModelData());
    i_this->mpBtp->entry(model->getModelData());
    i_this->mpMorf->entryDL();

    if (!fopAcM_checkCarryNow(i_this) && i_this->mAction != ACT_BOAT) {
        cXyz pos;
        pos.set(i_this->current.pos.x, i_this->current.pos.y + 100.0f, i_this->current.pos.z);
        i_this->mShadowKey =
            dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, 400.0f, 0.0f,
                               i_this->current.pos.y, i_this->mBgS_Acch.GetGroundH(),
                               i_this->mBgS_Acch.m_gnd, &i_this->tevStr, 0, 1.0f,
                               dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static BOOL daDo_other_bg_check(do_class* i_this, fopAc_ac_c* i_actor) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    cXyz dog_pos;
    cXyz actor_pos;
    dBgS_LinChk lin_chk;

    if (i_actor != NULL) {
        actor_pos = i_actor->current.pos;
        actor_pos.y += 50.0f;

        dog_pos = _this->current.pos;
        dog_pos.y = _this->eyePos.y;

        lin_chk.Set(&dog_pos, &actor_pos, _this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

static BOOL daDo_other_bg_check2(do_class* i_this, cXyz* i_pos) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    cXyz dog_pos;
    cXyz pos;
    dBgS_LinChk lin_chk;

    pos = *i_pos;
    pos.y += 30.0f;

    dog_pos = _this->current.pos;
    dog_pos.y += 30.0f;

    lin_chk.Set(&dog_pos, &pos, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return true;
    } else {
        return false;
    }
}

static fopAc_ac_c* target_info[5];

static int target_info_count;

static BOOL target_bgc[5];

static void* s_w_sub(void* i_proc, void* i_this) {
    if (fopAcM_IsActor(i_proc) &&
        (fopAcM_GetName(i_proc) == PROC_OBJ_FOOD || fopAcM_GetName(i_proc) == PROC_OBJ_KANBAN2) &&
        fopAcM_checkCarryNow(static_cast<fopAc_ac_c*>(i_proc)) && target_info_count < 5)
    {
        target_info[target_info_count] = static_cast<fopAc_ac_c*>(i_proc);
        target_info_count++;
    }
    return NULL;
}

static u32 search_food(do_class* i_this) {
    target_info_count = 0;

    for (int i = 0; i < 5; i++) {
        target_info[i] = NULL;
        target_bgc[i] = false;
    }

    fpcM_Search(s_w_sub, i_this);
    f32 pos_check = 200.0f;

    if (target_info_count != 0) {
        fopAc_ac_c* actor;
        int i = 0;
        while (i < target_info_count) {
            actor = target_info[i];
            cXyz delta;
            delta.x = actor->current.pos.x - i_this->eyePos.x;
            delta.y = actor->current.pos.y + 50.0f - i_this->eyePos.y;
            delta.z = actor->current.pos.z - i_this->eyePos.z;
            f32 dist_xz = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);

            if (dist_xz < pos_check) {
                if (target_bgc[i] || daDo_other_bg_check(i_this, actor)) {
                    target_bgc[i] = true;
                } else {
                    return fopAcM_GetID(actor);
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                pos_check += 100.0f;
                if (pos_check > i_this->mScale.z * 240.0f) {
                    return -1;
                }
            }
        }
    } else {
        return -1;
    }
    return -1;
}

static void food_check(do_class* i_this) {
    i_this->mFoodActorID = search_food(i_this);

    if (fopAcM_SearchByID(i_this->mFoodActorID)) {
        i_this->mAction = ACT_FOOD;
        i_this->mMode = 0;
    }
}

static BOOL do_carry_check(do_class* i_this) {
    if (i_this->mAction != ACT_CARRY && fopAcM_checkCarryNow(i_this)) {
        i_this->mAction = ACT_CARRY;
        i_this->mMode = 0;
        return true;
    }

    return false;
}

static BOOL depth_check(do_class* i_this, cXyz i_pos, f32 param_2) {
    dBgS_GndChk gnd_chk;
    Vec pos;
    f32 f_gnd_chk;
    f32 f_gnd_chk_spl;

    pos.x = i_pos.x;
    pos.y = i_pos.y + 200.0f;
    pos.z = i_pos.z;

    gnd_chk.SetPos(&pos);
    f_gnd_chk = dComIfG_Bgsp().GroundCross(&gnd_chk);

    dBgS_ObjGndChk_Spl gnd_chk_spl;
    gnd_chk_spl.SetPos(&pos);
    f_gnd_chk_spl = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

    if (f_gnd_chk_spl - f_gnd_chk > 2.0f * param_2 * 0.8f * i_this->mScale.z) {
        return 1;
    } else {
        return 0;
    }
}

static bool water_check(do_class* i_this) {
    dBgS_GndChk gnd_chk;
    Vec pos;

    pos = i_this->current.pos;
    pos.y += 200.0f;

    gnd_chk.SetPos(&pos);
    f32 f_gnd_chk = dComIfG_Bgsp().GroundCross(&gnd_chk);

    dBgS_ObjGndChk_Spl gnd_chk_spl;
    gnd_chk_spl.SetPos(&pos);
    i_this->mWaterY = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

    if (i_this->mWaterY - f_gnd_chk > 8.0f * i_this->mScale.z) {
        return true;
    } else {
        return false;
    }
}

static int dansa_check(do_class* i_this, cXyz i_pos, f32 param_2) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    i_pos.y += 25.0f;
    dBgS_GndChk gnd_chk;

    Vec pos;
    pos.x = i_pos.x;
    pos.y = i_pos.y + 25.0f;
    pos.z = i_pos.z;

    gnd_chk.SetPos(&pos);
    f32 f_gnd_chk = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if (i_this->current.pos.y - f_gnd_chk > param_2) {
        return 1;
    } else {
        dBgS_LinChk lin_chk;
        cXyz pos2;

        pos2 = _this->current.pos;
        pos2.y += 60.0f;

        lin_chk.Set(&pos2, &i_pos, _this);

        if (dComIfG_Bgsp().LineCross(&lin_chk) != 0) {
            return -1;
        } else if (depth_check(i_this, i_pos, 5.0f) && l_HIO.mSwimming == 0) {
            return -100;
        } else {
            return 0;
        }
    }
}

static BOOL dansa_check2(do_class* i_this, f32 param_1) {
    cXyz pos, pos2;

    cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
    pos.x = 0.0f;
    pos.y = 70.0f;
    pos.z = 0.8f * param_1 * i_this->mScale.z +
            0.8f * i_this->speedF * i_this->mScale.z * 9.0f;

    MtxPosition(&pos, &pos2);
    pos2 += i_this->current.pos;
    return dansa_check(i_this, pos2, 100.0f) != 0 ? TRUE : FALSE;
}

static BOOL move_dansa_check(do_class* i_this, f32 i_speed) {
    if (dansa_check2(i_this, 130.0f)) {
        i_this->mAction = ACT_WAIT_1;

        if (i_speed > 15.0f) {
            i_this->mMode = 10;
        } else {
            i_this->mMode = 0;
        }

        return true;
    } else {
        return false;
    }
}

static void area_check(do_class* i_this) {
    cXyz pos_delta = i_this->home.pos - i_this->current.pos;

    if (i_this->mParam2 != 0xff) {
        if (pos_delta.abs() > i_this->mParam2 * 100.0f * 2.0f) {
            i_this->mAction = ACT_WALK;
            i_this->mMode = -1;
            i_this->mTimer[2] = cM_rndF(100.0f) + 100.0f;
        }
    }
}

static void do_stay(do_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        if (i_this->mParam0 == 0) {
            anm_init(i_this, ANM_DOWN_WT, 15.0f, 2, 1.0f);
            i_this->mMode++;
            i_this->mTimer[0] = 10;
        } else {
            i_this->mAction = ACT_WALK;
            i_this->mMode = 0;
            return;
        }
        // fallthrough

    case 1:
        if (i_this->mTimer[0] == 0 && i_this->mDistFromPlayer < l_HIO.mPlayerRecognitionDist) {
            i_this->mMode++;
            i_this->mTimer[0] = cM_rndF(50.0f) + 20.0f;
        }
        break;

    case 2:
        i_this->field_0x616 = 1;
        i_this->mHeadTilt = -6300;
        i_this->mTailWagTarget = 1000.0f;
        if (i_this->mTimer[0] == 0) {
            anm_init(i_this, ANM_DOWN, 5.0f, 0, 1.0f);
            i_this->mMode++;
        }
        break;

    case 3:
        i_this->field_0x616 = 1;
        i_this->mTailWagTarget = 2000.0f;

        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WALK_RUN;
            i_this->mMode = 0;
        }
        break;

    case 10:
        anm_init(i_this, ANM_DOWN_WT, 15.0f, 2, 1.0f);
        i_this->mMode++;
        // fallthrough

    case 11:
        i_this->field_0x616 = 1;
        i_this->mHeadTilt = -6300;
        i_this->mTailWagTarget = 1000.0f;

        if (i_this->field_0x658 > 30.0f) {
            i_this->mMode++;
            i_this->mTimer[0] = cM_rndF(10.0f) + 10.0f;
        }

        if (i_this->mDistFromPlayer > 100.0f + l_HIO.mPlayerRecognitionDist) {
            i_this->field_0x5f4 = 0;
            i_this->mMode = 0;
        }
        break;

    case 12:
        i_this->field_0x616 = 1;
        i_this->mHeadTilt = -6300;
        i_this->mTailWagTarget = 1500.0f;

        if (i_this->field_0x658 > 30.0f) {
            if (i_this->mTimer[0] == 0) {
                anm_init(i_this, ANM_DOWN, 3.0f, 0, 1.0f);
                i_this->mMode = 13;
            }
        } else {
            i_this->mMode = 11;
        }
        break;

    case 13:
        i_this->field_0x616 = 1;
        i_this->mTailWagTarget = 1500.0f;

        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT_1;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
}

static void do_walk(do_class* i_this) {
    cXyz local_5c;
    s16 angle_diff;
    i_this->mTailWagTarget = 1000.0f;

    switch (i_this->mMode) {
    case -1:
        i_this->mTargetPos = i_this->home.pos;
        if (i_this->speedF >= l_HIO.mRunSpeed) {
            i_this->mAnmSpeed = 4.0f;
        } else {
            i_this->mAnmSpeed = 1.7f;
        }

        i_this->mTargetAnmSpeed = 1.7f;
        anm_init(i_this, ANM_WALK, 3.0f, 2, i_this->mAnmSpeed);
        i_this->mMode = 3;
        break;

    case 0:
        if (i_this->field_0x5f4 == 8) {
            anm_init(i_this, ANM_SIT, 5.0f, 0, 1.0f);
        } else {
            anm_init(i_this, ANM_DOWN, 5.0f, 0, 1.0f);
        }

        i_this->mMode++;
        break;

    case 1:
        if (i_this->mpMorf->isStop()) {
            i_this->mMode++;
        }
        break;

    case 2:
        local_5c.x = 0.0f;
        local_5c.y = 20.0f;

        for (int i = 0; i < 100; i++) {
            cMtx_YrotS(*calc_mtx, cM_rndF(65536.0f));

            local_5c.z = cM_rndF(100.0f * i_this->mParam2);
            MtxPosition(&local_5c, &i_this->mTargetPos);

            i_this->mTargetPos += i_this->home.pos;

            if (dansa_check(i_this, i_this->mTargetPos, 100.0f) == 0) {
                local_5c = i_this->mTargetPos - i_this->current.pos;

                if (local_5c.abs() > 300.0f)
                    break;
            }
        }

        i_this->mAnmSpeed = i_this->mTargetAnmSpeed = 1.3f + cM_rndF(0.6f);
        anm_init(i_this, ANM_WALK, 5.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;

    case 3:
        local_5c = i_this->mTargetPos - i_this->current.pos;

        cLib_addCalcAngleS2(&i_this->current.angle.y, (s16)cM_atan2s(local_5c.x, local_5c.z), 0x10,
                            0x100);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed, 1.0f,
                      l_HIO.mWalkSpeed * 0.2f);

        if (local_5c.abs() < 150.0f) {
            if (i_this->mParam0 == 0) {
                i_this->mAction = ACT_STAY;
                i_this->mMode = 0;
            } else {
                anm_init(i_this, ANM_WAIT, 5.0f, 2, 1.0f);
                i_this->mTimer[0] = cM_rndF(50.0f) + 20.0f;
                i_this->mTargetAnmSpeed = 1.0f;
                i_this->mMode++;
            }
        }

        break;

    case 4:
        cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);

        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 2;
        }

        break;

    case 10:
        cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, 0x400);

        i_this->field_0x616 = 1;
        i_this->mTailWagTarget = 2000.0f;

        angle_diff = i_this->current.angle.y - i_this->mTargetAngleY;

        if (angle_diff < 0) {
            angle_diff *= -1;
        }

        if (angle_diff < 0x800) {
            i_this->mMode++;
            anm_init(i_this, ANM_WAIT, 5.0f, 2, 1.0f);
            i_this->mTargetAnmSpeed = 1.0f;
            i_this->mTimer[0] = cM_rndF(10.0f) + 10.0f;
        }

        break;

    case 11:
        i_this->field_0x616 = 1;
        i_this->mTailWagTarget = 3000.0f;
        if (i_this->mTimer[0] == 0) {
            i_this->mAction = ACT_WALK_RUN;
            i_this->mMode = 0;
            return;
        }
        break;
    }

    cLib_addCalc2(&i_this->mAnmSpeed, i_this->mTargetAnmSpeed, 1.0f, 0.05f);
    i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);

    if (i_this->mTimer[2] == 0 && i_this->mMode < 10 &&
        i_this->mDistFromPlayer < l_HIO.mPlayerRecognitionDist)
    {
        anm_init(i_this, ANM_STEP_2, 2.0f, 2, 3.0f);
        i_this->mMode = 10;
    }

    move_dansa_check(i_this, i_this->speedF);
}

static void do_walk_run(do_class* i_this) {
    i_this->field_0x616 = 1;
    i_this->mTailWagTarget = 3000.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 0.25f;
        anm_init(i_this, 22, 10.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        // fallthrough

    case 1:
        cLib_addCalc2(&i_this->mAnmSpeed, 2.0f, 1.0f, 0.05f);
        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);

        if (i_this->mAnmSpeed >= 2.0f) {
            i_this->mAction = ACT_RUN;
            i_this->mMode = 0;
            i_this->mSound.startSound(Z2SE_DOG_BARK, 0, -1);
        }
        // fallthrough

    default:
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed,
                      1.0f, 0.2f * l_HIO.mWalkSpeed);
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 8, 0x400);

        if (i_this->mDistFromPlayer < 160.0f * i_this->mScale.z) {
            if (l_HIO.mWaitType != 0) {
                i_this->mAction = ACT_WAIT_2;
            } else {
                i_this->mAction = ACT_WAIT_1;
            }
            i_this->mMode = 0;
        }

        area_check(i_this);
        move_dansa_check(i_this, i_this->speedF);
        break;
    }
}

static void do_run(do_class* i_this) {
    i_this->field_0x616 = 1;
    i_this->mTailWagTarget = 4000.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 1.0f;
        i_this->mTargetAnmSpeed = cM_rndF(0.1f) + 1.5f;
        anm_init(i_this, 14, 2.0f, 2, 1.2f * i_this->mAnmSpeed);
        i_this->mMode++;
        // fallthrough

    case 1:
        cLib_addCalc2(&i_this->mAnmSpeed, i_this->mTargetAnmSpeed, 1.0f, 0.02f);
        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);

        if (i_this->mDistFromPlayer < 320.0f * i_this->mScale.z) {
            i_this->mAction = ACT_RUN_WALK;
            i_this->mMode = 0;
        }
        // fallthrough

    default:
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed * 1.4f,
                      1.0f, 0.2f * l_HIO.mRunSpeed);
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 8, 0x800);
        area_check(i_this);
        move_dansa_check(i_this, i_this->speedF);
        break;
    }
}

static void do_run_walk(do_class* i_this) {
    i_this->field_0x616 = 1;
    i_this->mTailWagTarget = 3000.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mAnmSpeed = 4.0f;
        anm_init(i_this, 22, 3.0f, 2, i_this->mAnmSpeed);
        i_this->mMode++;
        // fallthrough

    case 1:
        cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.05f);
        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);
        // fallthrough

    default:
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed, 1.0f, 5.0f);
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 8, 0x400);

        if (i_this->mDistFromPlayer < 160.0f * i_this->mScale.z) {
            if (l_HIO.mWaitType != 0) {
                i_this->mAction = ACT_WAIT_2;
            } else {
                i_this->mAction = ACT_WAIT_1;
            }
            i_this->mMode = 0;
        } else {
            if (i_this->mDistFromPlayer > 360.0f * i_this->mScale.z) {
                i_this->mAction = ACT_RUN;
                i_this->mMode = 0;
            }
        }

        area_check(i_this);
        move_dansa_check(i_this, i_this->speedF);
    }
}

static void do_wait_1(do_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    i_this->field_0x616 = 1;
    i_this->mTailWagTarget = 2000.0f;

    s16 angle_diff = i_this->current.angle.y - i_this->mTargetAngleY;

    if (angle_diff < 0) {
        angle_diff *= -1;
    }

    s16 angle_check = 0x3000;
    if ((i_this->mCounter & 0x7fU) == 0) {
        angle_check = 0x1000;
    }

    switch (i_this->mMode) {
    case 0:
        if (i_this->mDistFromPlayer > i_this->mScale.z * 320.0f) {
            anm_init(i_this, ANM_JOYFUL, 5.0f, 2, cM_rndFX(0.1f) + 0.6f);
            i_this->mMode = -1;
        } else {
            anm_init(i_this, ANM_WAIT, 10.0f, 2, 1.0f);
            i_this->mMode++;
        }
        // fallthrough

    case -1:
    case 1:
case1:
        if (angle_diff > angle_check) {
            anm_init(i_this, ANM_STEP_2, 2.0f, 2, 3.0f);
            i_this->mMode = 2;
        } else if (i_this->field_0x658 < 75.0f) {
            i_this->mMode = 3;
            i_this->mTimer[0] = cM_rndF(10.0f) + 10.0f;
        } else {
            if (i_this->mDistFromPlayer < i_this->mScale.z * 96.0f) {
                cXyz local_68;
                local_68.x = 0.0f;
                local_68.y = 20.0f;

                for (int i = 0; i < 20; i++) {
                    if (i < 10) {
                        cMtx_YrotS(*calc_mtx, i_this->mTargetAngleY + (s16)cM_rndFX(5000.0f));
                    } else {
                        cMtx_YrotS(*calc_mtx, cM_rndF(65536.0f));
                    }

                    local_68.z = -176.0f * i_this->mScale.z;

                    MtxPosition(&local_68, &i_this->mTargetPos);
                    i_this->mTargetPos += player->current.pos;

                    if (cM_rndF(1.0f) < 0.2f)
                        break;

                    if (dansa_check(i_this, i_this->mTargetPos, 100.0f) == 0)
                        break;

                    i_this->mLookPos = i_this->mTargetPos;
                    i_this->mLookTimer = 20.0f + cM_rndF(10.0f);
                }

                i_this->mMode = 5;
                i_this->mRandomTargetAngleY = cM_rndFX(10000.0f);

                anm_init(i_this, ANM_WALK, 2.0f, 2, -3.0f);
                i_this->mTimer[0] = cM_rndF(10.0f) + 15.0f;
            } else {
                if (i_this->mMode != 6 && i_this->mDistFromPlayer < i_this->mScale.z * 120.0f)
                {
                    anm_init(i_this, ANM_JOYFUL, 3.0f, 2, cM_rndFX(0.1f) + 1.0f);
                    i_this->mMode = 6;
                }
            }
        }
        break;

    case 2:
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, 0x400);

        if (angle_diff < 0x800) {
            i_this->mMode = 0;
        }

        break;

    case 3:
        if (i_this->field_0x658 < 75.0f) {
            if (i_this->mTimer[0] == 0) {
                if (i_this->field_0x658 < 30.0f) {
                    i_this->mAction = ACT_STAY;
                    i_this->mMode = 10;
                } else {
                    i_this->mAction = ACT_SIT;
                    i_this->mMode = 0;
                }
            }
        } else {
            i_this->mMode = 1;
        }
        break;

    case 5:
        cLib_addCalcAngleS2(&i_this->current.angle.y,
                            i_this->mTargetAngleY + i_this->mRandomAngleY, 4, 0x800);
        cLib_addCalcAngleS2(&i_this->mRandomAngleY, i_this->mRandomTargetAngleY, 4, 0x1000);
        cLib_addCalc2(&i_this->current.pos.x, i_this->mTargetPos.x, 0.2f, 3.5f);
        cLib_addCalc2(&i_this->current.pos.z, i_this->mTargetPos.z, 0.2f, 3.5f);

        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 0;
        }
        break;

    case 6:
        i_this->mTailWagTarget = 4000.0f;

        if (i_this->mDistFromPlayer > i_this->mScale.z * 176.0f) {
            i_this->mMode = 0;
        }
        goto case1;

    case 10:
        i_this->mAnmSpeed = 5.0f;
        anm_init(i_this, ANM_WALK, 2.0f, 2, i_this->mAnmSpeed);

        i_this->mMode++;
        // fallthrough

    case 11:
        cLib_addCalc2(&i_this->mAnmSpeed, 2.0f, 1.0f, 0.2f);
        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);

        if (i_this->mAnmSpeed <= 2.2f) {
            i_this->mMode = 0;
        }

        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);

    if (i_this->mDistFromPlayer > i_this->mScale.z * 240.0f && !dansa_check2(i_this, 130.0f) &&
        i_this->mMode < 10)
    {
        i_this->mAction = ACT_WALK_RUN;
        i_this->mAnmSpeed = 1.5f;
        anm_init(i_this, ANM_WALK, 5.0f, 2, i_this->mAnmSpeed);
        i_this->mMode = 1;
    }

    if (daPy_getLinkPlayerActorClass()->checkCanoeRide() && i_this->mDistFromPlayer < 1000.0f) {
        i_this->mAction = ACT_HELP;
        i_this->mMode = 0;
    }

    if (i_this->mParam0 == 2) {
        f32 distance = i_this->mDistFromPlayer * 0.0215f;

        if (distance > 127.0f) {
            distance = 127.0f;
        }

        i_this->mSound.startLevelSound(Z2SE_DOG_V_CALL_HELP, (u8)distance, -1);
    }
}

static void do_wait_2(do_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    i_this->field_0x616 = 1;
    i_this->mTailWagTarget = 4000.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode++;
        // fallthrough

    case 1:
        if (i_this->mTimer[0] == 0) {
            i_this->mTimer[0] = cM_rndF(20.0f) + 10.0f;
            i_this->mRandomTargetAngleY = cM_rndFX(15000.0f);

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->mAnmSpeed = cM_rndF(0.2f) + 2.0f;
            } else {
                i_this->mAnmSpeed = -(cM_rndF(0.2f) + 2.0f);
            }

            anm_init(i_this, ANM_STEP, 3.0f, 2, i_this->mAnmSpeed);
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);

            cXyz local_38;

            local_38.x = cM_rndFX(100.0f);
            local_38.y = 0.0f;
            local_38.z = cM_rndF(50.0f) + 120.0f;

            MtxPosition(&local_38, &i_this->mTargetPos);
            i_this->mTargetPos += player->current.pos;
        }

        break;
    }

    cLib_addCalc2(&i_this->current.pos.x, i_this->mTargetPos.x, 0.2f, 2.0f);
    cLib_addCalc2(&i_this->current.pos.z, i_this->mTargetPos.z, 0.2f, 2.0f);
    cLib_addCalc0(&i_this->speedF, 1.0f, 2.0f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY + i_this->mRandomAngleY,
                        4, 0x800);
    cLib_addCalcAngleS2(&i_this->mRandomAngleY, i_this->mRandomTargetAngleY, 4, 0x1000);

    if (i_this->mDistFromPlayer > 240.0f * i_this->mScale.z) {
        i_this->mAction = ACT_RUN;
        i_this->mMode = 0;
    }

    area_check(i_this);
}

static void do_sit(do_class* i_this) {
    i_this->field_0x616 = 1;
    i_this->mTailWagTarget = 1000.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_SIT_WAIT, 10.0f, 2, 1.0f);
        i_this->mMode++;
        // fallthrough

    case 1:
        if (i_this->field_0x658 > 75.0f) {
            i_this->mMode = 3;
            i_this->mTimer[0] = cM_rndF(10.0f) + 10.0f;
        } else {
            if (i_this->field_0x658 < 30.0f) {
                i_this->mMode = 2;
                i_this->mTimer[0] = cM_rndF(10.0f) + 10.0f;
            }
        }
        break;

    case 2:
        if (i_this->field_0x658 < 30.0f) {
            if (i_this->mTimer[0] == 0) {
                i_this->mAction = ACT_STAY;
                i_this->mMode = 10;
            }
        } else {
            i_this->mMode = 1;
        }
        break;

    case 3:
        if (i_this->field_0x658 > 75.0f) {
            if (i_this->mTimer[0] == 0) {
                anm_init(i_this, ANM_SIT, 3.0f, 0, 1.0f);
                i_this->mMode++;
            }
        } else {
            i_this->mMode = 1;
        }

        break;

    case 4:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT_1;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 2.0f);

    if (i_this->mDistFromPlayer > 400.0f * i_this->mScale.z) {
        i_this->mAction = ACT_STAY;
        i_this->field_0x5f4 = 8;
        i_this->mMode = 0;
    }

    area_check(i_this);
}

static s16 hang_set(do_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    cXyz vec1, vec2, vec3[2];
    vec1 = _this->old.pos - _this->current.pos;
    cMtx_YrotS(*calc_mtx, cM_atan2s(vec1.x, vec1.z));
    
    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = -100.0f;
    MtxPosition(&vec1, &vec2);
    vec2 += _this->current.pos;
    vec2.y = _this->old.pos.y - 5.0f;

    vec1.x = 10.0f;
    vec1.y = 0.0f;
    vec1.z = 250.0f;

    for (int i = 0; i < 2; i++) {
        MtxPosition(&vec1, &vec3[i]);
        vec1.x *= -1.0f;
        vec3[i] += vec2;
        lin_chk.Set(&vec2, &vec3[i], _this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            vec3[i] = lin_chk.GetCross();
        } else {
            return 0xDCF;
        }
    }

    i_this->mHangPos = vec3[0] + (vec3[1] - vec3[0]) * 0.5f;
    i_this->mHangPos.y = _this->old.pos.y;

    vec1 = vec3[1] - vec3[0];
    return cM_atan2s(vec1.x, vec1.z) + 0x4000;
}

static void do_hang(do_class* i_this) {
    i_this->mTailWagTarget = 3000.0f;
    i_this->field_0x616 = 2;
    cXyz vec1, vec2;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_CLIFF_WT, 3.0f, 2, 1.0f);
        i_this->mMode++;
        i_this->speedF = 0.0f;
        i_this->mHangSpeed = 0.0f;
        i_this->mTimer[0] = cM_rndF(30.0f) + 30.0f;
        // fallthrough

    case 1:
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mHangAngleY, 1, 0x1000);

        if (i_this->mTimer[0] == 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->speed.y = 0.0f;
                i_this->mMode = 5;
                i_this->mTimer[0] = 3;
                anm_init(i_this, ANM_RUN, 5.0f, 2, 3.0f);
            } else {
                anm_init(i_this, ANM_CLIFF, 2.0f, 0, 1.0f);
                i_this->mMode = 2;
            }
        }

        cMtx_YrotS(*calc_mtx, i_this->mHangAngleY);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = i_this->mScale.z * 28.0f;
        MtxPosition(&vec1, &vec2);
        cLib_addCalc2(&i_this->current.pos.x, i_this->mHangPos.x + vec2.x, 1.0f,
                      i_this->mHangSpeed);
        cLib_addCalc2(&i_this->current.pos.y, i_this->mHangPos.y + vec2.y, 1.0f,
                      i_this->mHangSpeed);
        cLib_addCalc2(&i_this->current.pos.z, i_this->mHangPos.z + vec2.z, 1.0f,
                      i_this->mHangSpeed);
        cLib_addCalc2(&i_this->mHangSpeed, 100.0f, 1.0f, 20.0f);
        break;

    case 2:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_STAY;
            i_this->mMode = 13;
        }
        break;

    case 5:
        cMtx_YrotS(*calc_mtx, i_this->mHangAngleY);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = i_this->mScale.z * -60.0f;
        MtxPosition(&vec1, &vec2);
        cLib_addCalc2(&i_this->current.pos.x, i_this->mHangPos.x + vec2.x, 1.0f, 15.0f);
        cLib_addCalc2(&i_this->current.pos.z, i_this->mHangPos.z + vec2.z, 1.0f, 15.0f);
        i_this->current.pos.y += i_this->speed.y;
        i_this->speed.y += i_this->gravity;

        if (i_this->mTimer[0] == 0) {
            i_this->mBgS_Acch.CrrPos(dComIfG_Bgsp());
            if (i_this->mBgS_Acch.ChkGroundHit()) {
                i_this->current.angle.x = 0;
                anm_init(i_this, ANM_DOWN, 2.0f, 0, 2.0f);
                i_this->mMode = 6;
            }
        }
        break;

    case 6:
        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACT_WAIT_1;
            i_this->mMode = 0;
        }
        break;
    }
}

static void do_food(do_class* i_this) {
    fopAc_ac_c* food = fopAcM_SearchByID(i_this->mFoodActorID);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz vec;
    s16 angle_step;

    BOOL end = false;
    if (food == NULL) {
        end = true;
    }

    if (i_this->mBgS_Acch.ChkWallHit()) {
        i_this->mFoodEndTimer += 2;
        if (i_this->mFoodEndTimer >= 60) {
            end = true;
        }
    } else if (i_this->mFoodEndTimer != 0) {
        i_this->mFoodEndTimer--;
    }

    if (end) {
        i_this->mAction = ACT_WAIT_1;
        i_this->mMode = 0;
        return;
    }

    i_this->mTailWagTarget = 4000.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode++;
        i_this->mTimer[1] = cM_rndF(10.0f) + 10.0f;
        i_this->mTimer[0] = 0;
        // fallthrough

    case 1:
        i_this->speedF = 0.0f;

        if (i_this->mTimer[1] == 0) {
            if (i_this->mTimer[0] == 0) {
                if (cM_rndF(1.0f) < 0.5f) {
                    anm_init(i_this, ANM_JOYFUL, 3.0f, 2, 1.0f + cM_rndFX(0.1f));
                    i_this->mMode = 2;
                    i_this->mTimer[0] = cM_rndF(20.0f) + 10.0f;
                } else {
                    i_this->mTimer[0] = cM_rndF(20.0f) + 10.0f;
                    i_this->mRandomTargetAngleY = cM_rndFX(15000.0f);
                    i_this->mAnmSpeed = cM_rndF(0.2f) + 2.0f;
                    anm_init(i_this, ANM_STEP, 3.0f, 2, i_this->mAnmSpeed);

                    cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                    vec.x = cM_rndFX(100.0f);
                    vec.y = 0.0f;
                    vec.z = cM_rndF(50.0f) + 120.0f;
                    MtxPosition(&vec, &i_this->mTargetPos);
                    i_this->mTargetPos += player->current.pos;
                }
            }

            cLib_addCalc2(&i_this->current.pos.x, i_this->mTargetPos.x, 0.2f, 1.5f);
            cLib_addCalc2(&i_this->current.pos.z, i_this->mTargetPos.z, 0.2f, 1.5f);
            cLib_addCalcAngleS2(&i_this->current.angle.y,
                                i_this->mTargetAngleY + i_this->mRandomAngleY, 4, 0x800);
            cLib_addCalcAngleS2(&i_this->mRandomAngleY, i_this->mRandomTargetAngleY, 4, 0x1000);
        }

case1and2:
        i_this->field_0x616 = 1;

        if (fopAcM_CheckFoodStatus(food, fopAcM_FOOD_1)) {
            i_this->mMode = 10;
            i_this->mTimer[0] = cM_rndF(20.0f);
            i_this->mTimer[2] = cM_rndF(50.0f) + 70.0f;
        } else if (fopAcM_CheckFoodStatus(food, fopAcM_FOOD_0)) {
            i_this->mAction = ACT_WAIT_1;
            i_this->mMode = 0;
        } else if (i_this->mDistFromPlayer > 240.0f * i_this->mScale.z) {
            i_this->mAction = ACT_RUN;
            i_this->mMode = 0;
        }
        break;

    case 2:
        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 1;
        }
        goto case1and2;

    case 10:
        if (i_this->mTimer[0] == 0) {
            i_this->mAnmSpeed = 1.0f;
            i_this->mTargetAnmSpeed = cM_rndF(0.1f) + 1.5f;
            anm_init(i_this, ANM_RUN, 2.0f, 2, 1.2f * i_this->mAnmSpeed);
            i_this->mMode++;
        } else {
            i_this->mLookPos = food->current.pos;
            i_this->mLookTimer = 10;
            break;
        }
        // fallthrough

    case 11:
        i_this->mLookPos = food->current.pos;
        i_this->mLookTimer = 10;

        cLib_addCalc2(&i_this->mAnmSpeed, i_this->mTargetAnmSpeed, 1.0f, 0.02f);
        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed, 1.0f,
                      0.2f * l_HIO.mRunSpeed);
        
        vec = food->current.pos - i_this->current.pos;
        angle_step = 0x1000;
        if (vec.abs() < 120.0f * i_this->mScale.z) {
            angle_step = 0x2000;
            cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed * 0.75f, 1.0f,
                          0.2f * l_HIO.mRunSpeed * 1.5f);
        }
        cLib_addCalcAngleS2(&i_this->current.angle.y, cM_atan2s(vec.x, vec.z), 8, angle_step);

        if (fopAcM_CheckFoodStatus(food, fopAcM_FOOD_2)
            || fopAcM_CheckFoodStatus(food, fopAcM_FOOD_3)
            || fopAcM_CheckFoodStatus(food, fopAcM_FOOD_4))
        {
            i_this->mMode = 25;
            i_this->mTimer[0] = cM_rndF(20.0f) + 5.0f;
        } else if (vec.abs() < 50.0f * i_this->mScale.z) {
            bool get_item = false;
            if (i_this->mDistFromPlayer > 400.0f
                && !strcmp(dComIfGp_getStartStageName(), "F_SP116"))
            {
                if (fopAcM_GetRoomNo(i_this) == 4) {
                    if (i_this->current.pos.z < -1050.f) {
                        get_item = true;
                    }
                } else if (fopAcM_GetRoomNo(i_this) == 2) {
                    vec.x = -4314.0f - i_this->current.pos.x;
                    vec.z = 728.0f - i_this->current.pos.z;
                    if (JMAFastSqrt(vec.x * vec.x + vec.z * vec.z) < 300.0f) {
                        get_item = true;
                    }
                }
            }

            if (get_item) {
                int item_no;
                if (dComIfGs_getLife() <= 4) {
                    item_no = fpcNm_ITEM_HEART;
                } else {
                    int rnd = cM_rndF(11.0f);
                    if (rnd <= 5) {
                        item_no = fpcNm_ITEM_HEART;
                    } else if (rnd <= 9) {
                        item_no = fpcNm_ITEM_GREEN_RUPEE;
                    } else {
                        item_no = fpcNm_ITEM_BLUE_RUPEE;
                    }
                }

                i_this->mItemActorID =
                    fopAcM_createItem(&i_this->eyePos, item_no, -1, -1, NULL, NULL, 0);
                i_this->mMode = 25;
                i_this->mTimer[0] = cM_rndF(10.0f) + 5.0f;
            } else {
                i_this->mMode = 20;
                i_this->mFoodInMouth = 1;
                i_this->mAnmSpeed = 4.0f;
                anm_init(i_this, ANM_JOYFUL, 3.0f, 2, 0.3f);
            }
        }
        break;

    case 20:
        i_this->mLookPos = player->current.pos;
        i_this->mLookTimer = 10;
        cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.2f);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed, 1.0f, 5.0f);

        if (i_this->mAnmSpeed <= 1.6f) {
            i_this->mTargetAnmSpeed = cM_rndF(0.1f) + 1.5f;
            anm_init(i_this, ANM_RUN, 2.0f, 2, 1.2f * i_this->mAnmSpeed);
            i_this->mMode++;
        }

        i_this->mFoodInMouth = 1;
        break;

    case 21:
        i_this->field_0x616 = 1;
        cLib_addCalc2(&i_this->mAnmSpeed, i_this->mTargetAnmSpeed, 1.0f, 0.02f);
        i_this->mpMorf->setPlaySpeed(i_this->mAnmSpeed);
        cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mRunSpeed, 1.0f,
                      0.2f * l_HIO.mRunSpeed);
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 8, 0x1000);
        i_this->mFoodInMouth = 1;

        if (i_this->mDistFromPlayer < 120.0f * i_this->mScale.z) {
            i_this->mAction = ACT_RUN_WALK;
            i_this->mMode = 0;
            food->speedF = 0.0f;
        }
        break;

    case 25:
        if (i_this->mTimer[0] != 0) {
            if (i_this->mTimer[0] == 1) {
                i_this->mAnmSpeed = cM_rndFX(1.0f) + 3.0f;
                anm_init(i_this, ANM_JOYFUL, 3.0f, 2, 0.3f);
            }
        } else {
            i_this->mLookPos = player->current.pos;
            i_this->mLookTimer = 10;
            cLib_addCalc2(&i_this->mAnmSpeed, 1.5f, 1.0f, 0.1f);
            cLib_addCalc2(&i_this->speedF, i_this->mAnmSpeed * l_HIO.mWalkSpeed, 1.0f, 5.0f);

            if (i_this->mAnmSpeed <= 1.6f) {
                i_this->mAction = ACT_RUN;
                i_this->mMode = 0;
            }
        }
        break;
    }
}

static void do_swim(do_class* i_this) {
    cXyz pos;
    i_this->field_0x616 = 1;
    i_this->mTailWagTarget = 2000.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 19, 5.0f, 2, cM_rndF(0.15f) + 1.3f);
        i_this->mMode++;
        break;

    case 1:
        break;
    }

    cLib_addCalc2(&i_this->speedF, l_HIO.mSwimSpeed, 1.0f, 0.5f * l_HIO.mSwimSpeed);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 16, 0x100);

    i_this->speed.y = 0.0f;
    i_this->gravity = 0.0f;

    cLib_addCalc2(&i_this->current.pos.y, i_this->mWaterY - 45.0f, 1.0f, 5.0f);

    pos = i_this->eyePos;
    pos.y = i_this->mWaterY;

    fopAcM_effHamonSet(&i_this->mRippleKey, &pos, 0.7f, 0.1f);
}

static void* s_c_sub(void* param_0, void* param_1) {
    if (fopAcM_IsActor(param_0) && fopAcM_GetName(param_0) == PROC_CANOE) {
        return param_0;
    }
    return 0;
}

static void do_help(do_class* i_this) {
    f32 target_speed = 0.0f;
    i_this->field_0x616 = 1;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_WALK, 3.0f, 2, 2.0f);
        i_this->mMode++;
        break;

    case 1:
        target_speed = 5.0f;
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 2, 0x800);
        if (dansa_check2(i_this, 50.0f)) {
            anm_init(i_this, ANM_WAIT, 5.0f, 2, 1.0f);
            i_this->mMode++;
        }
        break;

    case 2:
        if (i_this->mDistFromPlayer < 800.0f) {
            i_this->mTailWagTarget = 1000.0f;
        }
        if (i_this->mDistFromPlayer < 500.0f) {
            i_this->mMode++;
            anm_init(i_this, ANM_JOYFUL, 3.0f, 2, 1.0f);
        }
        break;

    case 3:
        i_this->mTailWagTarget = 2000.0f;
        if (i_this->mDistFromPlayer > 550.0f) {
            anm_init(i_this, ANM_WAIT, 5.0f, 2, 1.0f);
            i_this->mMode = 2;
        }

        if (i_this->mTimer[2] == 0 && daPy_getLinkPlayerActorClass()->checkCanoeRide()) {
            cXyz vec1, vec2, vec3;
            daCanoe_c* canoe = (daCanoe_c*)fpcM_Search(s_c_sub, i_this);
            if (canoe != NULL) {
                vec3 = i_this->field_0x68c;
                MTXCopy(canoe->getModelMtx(), *calc_mtx);
                vec1.set(0.0f, 52.0f, 150.0f);
                MtxPosition(&vec1, &i_this->field_0x68c);
                i_this->mLookPos = i_this->field_0x68c;
                i_this->mLookTimer = 20;
                vec2 = i_this->field_0x68c - i_this->current.pos;
                vec2.y = 0.0f;
                if ((i_this->field_0x68c - vec3).abs() <= 5.0f && vec2.abs() < 230.0f) {
                    i_this->mParam0 = 0;
                    i_this->mAction = ACT_BOAT;
                    i_this->mMode = 0;
                    anm_init(i_this, ANM_DEFEND, 10.0f, 0, 1.0f);
                    i_this->mTimer[0] = 20;
                }
            }
            i_this->mTargetAngleY = cM_atan2s(vec2.x, vec2.z);
        }

        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, 0x400);
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, 2.0f);
    
    if (!daPy_getLinkPlayerActorClass()->checkCanoeRide() && i_this->mDistFromPlayer < 200.0f) {
        i_this->mAction = ACT_WAIT_1;
        i_this->mMode = 0;
    }

    if (i_this->mParam0 == 2) {
        f32 fvar2 = i_this->mDistFromPlayer * 0.0215f;
        if (fvar2 > 127.0f) {
            fvar2 = 127.0f;
        }
        i_this->mSound.startLevelSound(Z2SE_DOG_V_CALL_HELP, (u8)fvar2, -1);
    }
}

static s8 do_boat(do_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    s8 ret = 1;
    i_this->mCcDisableTimer = 10;
    f32 fvar3;

    daCanoe_c* canoe = (daCanoe_c*)fpcM_Search(s_c_sub, i_this);
    if (canoe == NULL) {
        return 1;
    }

    cXyz vec1, vec2;
    s16 canoe_angle = canoe->shape_angle.y;
    vec2 = i_this->field_0x68c;
    MTXCopy(canoe->getModelMtx(), *calc_mtx);
    vec1.set(0.0f, 52.0f, 150.0f);
    MtxPosition(&vec1, &i_this->field_0x68c);
    f32 fvar4 = (i_this->field_0x68c - vec2).abs();
    if (!daPy_getLinkPlayerActorClass()->checkCanoeRide() && (i_this->mCounter & 0x200) != 0) {
        fvar4 = 5.0f;
    }
    s8 bvar2 = false;

    switch (i_this->mMode) {
    case 0:
        if (daDo_other_bg_check2(i_this, &i_this->field_0x68c)) {
            i_this->mAction = ACT_HELP;
            i_this->mMode = 0;
            _this->speedF = 0.0f;
            i_this->mTimer[2] = 50;
            break;
        }

        if (i_this->mTimer[0] == 2) {
            anm_init(i_this, ANM_RUN, 2.0f, 0, 2.0f);
        }

        if (i_this->mTimer[0] != 0) {
            break;
        }

        i_this->mMode++;
        _this->speed.y = 30.0f;
        _this->speedF = 0.0f;
        i_this->mSound.startSound(Z2SE_DOG_V_JUMP, 0, -1);
        i_this->mTimer[1] = 7;
        // fallthrough

    case 1:
        if (i_this->mTimer[1] != 0) {
            ret = 0;
        }

        vec1 = i_this->field_0x68c - _this->current.pos;
        fvar3 = vec1.abs();
        cMtx_YrotS(*calc_mtx, cM_atan2s(vec1.x, vec1.z));
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = fvar3 * 0.075f + 10.0f;
        MtxPosition(&vec1, &i_this->field_0x698);

        cLib_addCalc2(&i_this->current.pos.x, i_this->field_0x68c.x, 1.0f,
                      fabsf(i_this->field_0x698.x) + 2.0f);
        cLib_addCalc2(&i_this->current.pos.z, i_this->field_0x68c.z, 1.0f,
                      fabsf(i_this->field_0x698.z) + 2.0f);
        
        if (_this->speed.y <= -80.0f) {
            _this->speed.y = -80.0f;
        }

        if (_this->current.pos.y <= i_this->field_0x68c.y) {
            _this->current.pos.y = i_this->field_0x68c.y;
            _this->speed.y = 0.0f;
        }

        if ((i_this->field_0x68c - _this->current.pos).abs() <= 5.0f) {
            i_this->mMode = 2;
            anm_init(i_this, ANM_STEP_2, 3.0f, 2, 3.0f);
        } else {
            break;
        }
        // fallthrough

    case 2:
        ret = 0;

        i_this->field_0x616 = 1;
        _this->speed.y = 0.0f;
        _this->gravity = 0.0f;
        _this->current.pos = i_this->field_0x68c;
        cLib_addCalcAngleS2(&_this->current.angle.y, i_this->mTargetAngleY, 2, 0x800);
        _this->current.angle.x = 2700;

        if (i_this->mAnmID == ANM_STEP_2) {
            s16 angle = i_this->mTargetAngleY - _this->current.angle.y;
            if (angle < 0x800 && angle > -0x800) {
                anm_init(i_this, ANM_JOYFUL, 3.0f, 2, 1.0f);
            }
        } else if (i_this->mAnmID == ANM_JOYFUL && fvar4 > 3.0f) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_STEP_2, 3.0f, 2, 4.0f);
        }
        break;

    case 3:
        ret = 0;

        if (i_this->mTimer[0] == 0) {
            i_this->mTimer[0] = cM_rndF(70.0f) + 50.0f;
            cMtx_YrotS(*calc_mtx, _this->shape_angle.y);
            vec1.x = cM_rndFX(2000.0f);
            vec1.y = cM_rndF(50.0f) + 50.0f;
            vec1.z = 2000.0f;
            MtxPosition(&vec1, &i_this->mLookPos);
            i_this->mLookPos += _this->current.pos;
        }

        i_this->mLookTimer = 2;
        _this->speed.y = 0.0f;
        _this->gravity = 0.0f;
        _this->current.pos = i_this->field_0x68c;
        cLib_addCalcAngleS2(&_this->current.angle.y, canoe_angle, 2, 0x800);
        _this->current.angle.x = -2700;

        if (i_this->mAnmID == ANM_STEP_2) {
            s16 angle = canoe_angle - _this->current.angle.y;
            if (angle < 0x800 && angle > -0x800) {
                anm_init(i_this, ANM_WAIT, 3.0f, 2, 1.0f);
            }
        } else if (i_this->mAnmID == ANM_WAIT && fvar4 < 10.0f) {
            bvar2 = 1;
            if (fvar4 < 1.0f) {
                i_this->mMode = 2;
                anm_init(i_this, ANM_STEP_2, 3.0f, 2, 4.0f);
            }
        }
        break;

    case 5:
        anm_init(i_this, ANM_DEFEND, 10.0f, 0, 1.0f);
        i_this->mTimer[0] = 20;
        i_this->mMode++;
        // fallthrough

    case 6:
        ret = 0;
        i_this->mLookPos = i_this->mTargetPos;
        i_this->mLookTimer = 2;
        _this->speed.y = 0.0f;
        _this->gravity = 0.0f;
        _this->current.pos = i_this->field_0x68c;
        vec1 = i_this->mTargetPos - _this->current.pos;
        cLib_addCalcAngleS2(&_this->current.angle.y, cM_atan2s(vec1.x, vec1.z), 2, 0x800);

        if (i_this->mTimer[0] == 2) {
            anm_init(i_this, ANM_RUN, 2.0f, 0, 2.0f);
        }

        if (i_this->mTimer[0] == 0) {
            i_this->mMode++;
            _this->speed.y = 30.0f;
            _this->speedF = 30.0f;
            _this->current.angle.x = 0;
            i_this->mSound.startSound(Z2SE_DOG_V_JUMP, 0, -1);
        }
        break;

    case 7:
        if (i_this->mBgS_Acch.ChkGroundHit()) {
            i_this->mAction = ACT_HELP;
            i_this->mMode = 0;
            _this->speedF = 0.0f;
            i_this->mTimer[2] = 100;
        }
        break;
    }

    if (bvar2) {
        i_this->field_0xc84 = _this->current.pos;
        i_this->field_0xc84.y += 40.0f;
        cMtx_YrotS(*calc_mtx, canoe->shape_angle.y);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 250.0f;
        MtxPosition(&vec1, &i_this->field_0xc90);
        i_this->field_0xc90 += i_this->field_0xc84;
        i_this->field_0xc9c = i_this->field_0xc90;
        i_this->field_0xc9c.y += -100.0f;
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 180.0f;
        MtxPosition(&vec1, &i_this->field_0xca8);
        i_this->field_0xca8 += i_this->field_0xc84;
        i_this->field_0xcb4 = i_this->field_0xca8;
        i_this->field_0xcb4.y += -100.0f;

        dBgS_LinChk lin_chk;
        lin_chk.Set(&i_this->field_0xc84, &i_this->field_0xc90, _this);
        i_this->field_0xc80 = true;
        if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
            lin_chk.Set(&i_this->field_0xca8, &i_this->field_0xcb4, _this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                lin_chk.Set(&i_this->field_0xc90, &i_this->field_0xc9c, _this);
                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    i_this->field_0xc80 = false;
                    i_this->mTargetPos = lin_chk.GetCross();
                    i_this->mMode = 5;
                }
            }
        }
    }

    return ret;
}

static void do_a_swim(do_class* i_this) {
    i_this->mTailWagTarget = 2000.0f;
    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
    JPABaseEmitter* emitter;

    switch (i_this->mMode) {
    case 0:
        if (l_HIO.mWaterHuntAnimType == 1) {
            anm_init(i_this, ANM_JOYFUL, 5.0f, 2, 1.0f);
        } else {
            anm_init(i_this, ANM_BULBUL, 5.0f, 2, 1.0f);
        }

        i_this->mMode = 1;
        i_this->mTimer[0] = 20;

        emitter = dComIfGp_particle_set(0x2a3, &i_this->current.pos, 0, 0);

        if (emitter) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(2));
            cXyz _pos(0.6f, 0.6f, 0.6f);
            JGeometry::TVec3<f32> pos;
            JGeometry::setTVec3f(&_pos.x, &pos.x);
            emitter->setGlobalScale(pos);
        }
        // fallthrough

    case 1:
        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 2;
            i_this->mShakeSpeed = 0.1f;
            i_this->mShakeTargetSpeed = 1.0f;
        }
        break;

    case 2:
        if (l_HIO.mWaterHuntAnimType == 1) {
            i_this->mpMorf->setPlaySpeed(i_this->mShakeSpeed * 5.0f);
        } else {
            i_this->mpMorf->setPlaySpeed(i_this->mShakeSpeed * 5.0f);
        }

        if (i_this->mShakeSpeed <= 0.025f) {
            i_this->mShakeSpeed = 0.0f;
            i_this->mAction = ACT_WAIT_1;
            i_this->mMode = 0;
        }
        break;
    }
}

static s8 do_carry(do_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    s8 ret = 0;
    i_this->mCcDisableTimer = 5;
    _this->speed.y = 0.0f;
    _this->speedF = 0.0f;
    
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_TO_CARRY_B, 1.0f, 0,
                 daPy_getLinkPlayerActorClass()->getBaseAnimeFrameRate());
        i_this->mpMorf->setFrame(daPy_getLinkPlayerActorClass()->getBaseAnimeFrame());
        i_this->mTimer[0] = 20;
        i_this->mMode++;
        break;

    case 1:
        if (i_this->mpMorf->checkFrame(10.0f)) {
            i_this->mSound.startSound(Z2SE_DOG_CARRIED, 0, -1);
        }

        if (daPy_getLinkPlayerActorClass()->getGrabPutStart()) {
            anm_init(i_this, ANM_TO_CARRY_B, 1.0f, 0,
                     daPy_getLinkPlayerActorClass()->getBaseAnimeFrameRate());
            i_this->mpMorf->setFrame(daPy_getLinkPlayerActorClass()->getBaseAnimeFrame());
            i_this->mMode++;
        }
        break;
    }

    if (!fopAcM_checkCarryNow(_this)) {
        i_this->mAction = ACT_WAIT_1;
        i_this->mMode = 0;
        
        dBgS_GndChk gnd_chk;
        cXyz vec = _this->current.pos;
        vec.y += 50.0f;
        gnd_chk.SetPos(&vec);
        if (fabsf(_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk)) < 30.0f) {
            _this->current.pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            _this->gravity = 0.0f;
        }
    }

    i_this->mTailWagTarget = 1000.0f;
    _this->speedF = 0.0f;

    if (i_this->mTimer[0] != 0) {
        ret = 1;
        _this->speed.y = -0.1f;
    } else {
        _this->speed.y = 0.0f;
    }

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    dBgS_LinChk lin_chk;
    cXyz vec = _this->current.pos;
    vec.y += 2.0f;
    lin_chk.Set(&player->eyePos, &vec, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        _this->current.pos = lin_chk.GetCross();
    }

    return ret;
}

static void do_message(do_class* i_this) {
    i_this->mTailWagTarget = 2000.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 21, 5.0f, 2, 1.0f);
        i_this->mMode++;
        break;

    case 1:
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 2.0f);
    i_this->field_0x616 = 1;
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 2, 0x1000);

    if (!dComIfGp_event_runCheck() && i_this->mDistFromPlayer > 400.0f) {
        i_this->mAction = ACT_STAY;
        i_this->mMode = 0;
    }
}

static void action(do_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    cXyz vec1, vec2;
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));

    _this->gravity = -7.0f;

    i_this->mDistFromPlayer = fopAcM_searchPlayerDistance(_this);
    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        i_this->mDistFromPlayer -= 100.0f;
    }

    i_this->mTargetAngleY = fopAcM_searchPlayerAngleY(_this);

    i_this->field_0x658 = fabsf(player->eyePos.y - player->current.pos.y);
    if (!mDoCPd_c::getHoldR(PAD_1) || fabsf(_this->current.pos.y - player->current.pos.y) > 50.0f) {
        i_this->field_0x658 = 300.0f;
    }
    i_this->field_0x658 *= 0.6f;

    s8 ground_check = 1;
    s8 carry_check = 1;
    s8 swim_check = 1;

    i_this->mCcSph.OnCoSetBit();

    if (i_this->field_0x608 < 1.0f) {
        i_this->mMessageState = 0;

        switch (i_this->mAction) {
        case ACT_STAY:
            do_stay(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_WALK:
            do_walk(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_WALK_RUN:
            do_walk_run(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_RUN:
            do_run(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_RUN_WALK:
            do_run_walk(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_FOOD:
            do_food(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_WAIT_1:
            do_wait_1(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_WAIT_2:
            do_wait_2(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_SIT:
            do_sit(i_this);
            food_check(i_this);
            i_this->mMessageState = 1;
            break;
        case ACT_A_SWIM:
            do_a_swim(i_this);
            break;
        case ACT_SWIM:
            do_swim(i_this);
            swim_check = 0;
            fopAcM_riverStream(&_this->current.pos, &i_this->field_0x6b0, &i_this->field_0x6b4,
                               0.5f);
            break;
        case ACT_HELP:
            do_help(i_this);
            break;
        case ACT_BOAT:
            ground_check = do_boat(i_this);
            break;
        case ACT_HANG:
            do_hang(i_this);
            break;
        case ACT_CARRY:
            ground_check = do_carry(i_this);
            i_this->mCcSph.OffCoSetBit();
            carry_check = 0;
            break;
        case ACT_MESSAGE:
            do_message(i_this);
            i_this->mMessageState = 2;
        }
    }

    if (i_this->mItemActorID != -1 && i_this->mDistFromPlayer < 160.0f * i_this->mScale.z) {
        daItem_c* item = static_cast<daItem_c*>(fopAcM_SearchByID(i_this->mItemActorID));

        if (item) {
            item->endControl();
        }

        i_this->mItemControlTimer = 0;
        i_this->mItemActorID = -1;
    }

    if (i_this->mRunningSoundTimer != 0) {
        i_this->mRunningSoundTimer--;
        if (i_this->mRunningSoundTimer == 0) {
            i_this->mSound.startSound(Z2SE_M007_DOG_COME_RUNNING, 0, -1);
        }
    }

    if (i_this->mMessageState == 1 && daPy_py_c::checkNowWolf()
        && i_this->mDistFromPlayer < 300.0f)
    {
        i_this->mAction = ACT_MESSAGE;
        i_this->mMode = 0;
    }

    if (carry_check && player->speedF < 2.0f) {
        cLib_onBit<u32>(_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);

        if (do_carry_check(i_this)) {
            return;
        }
    } else {
        cLib_offBit<u32>(_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    cLib_addCalcAngleS2(&_this->current.angle.x, 0, 1, 0x400);

    if (i_this->mAction != ACT_HANG) {
        cMtx_YrotS(*calc_mtx, _this->current.angle.y);

        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = _this->speedF * 0.8f * i_this->mScale.z * l_HIO.mBaseSize;

        MtxPosition(&vec1, &vec2);

        _this->speed.x = vec2.x;
        _this->speed.z = vec2.z;
        _this->speed.y += _this->gravity;

        _this->current.pos += _this->speed;

        if (_this->speed.y < -80.0f) {
            _this->speed.y = -80.0f;
        }

        if (!fopAcM_checkCarryNow(_this)) {
            cXyz* cc_move = i_this->mStts.GetCCMoveP();
            if (cc_move != NULL) {
                _this->current.pos.x += 0.3f * cc_move->x;
                _this->current.pos.z += 0.3f * cc_move->z;
            }
        }

        if (i_this->field_0x608 > 1.0f) {
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = -i_this->field_0x608;

            cMtx_YrotS(*calc_mtx, i_this->field_0x60c);
            MtxPosition(&vec1, &vec2);

            _this->current.pos += vec2;
            cLib_addCalc0(&i_this->field_0x608, 1.0f, 6.0f);

            _this->speedF = 0.0f;
            ground_check = 1;
        }

        if (ground_check) {
            i_this->mBgS_AcchCir.SetWall(30.0f,
                                         fabsf(_this->speedF) + fabsf(i_this->field_0x608) + 30.0f);
            i_this->mBgS_Acch.CrrPos(dComIfG_Bgsp());

            if (i_this->mBgS_Acch.ChkGroundHit() && !noFallCheck && !fopAcM_checkCarryNow(_this))
            {
                dBgS_GndChk gnd_chk;

                cMtx_YrotS(*calc_mtx, _this->shape_angle.y);

                vec1.x = 0.0f;
                vec1.y = 0.0f;
                vec1.z = -25.0f * i_this->mScale.z;

                MtxPosition(&vec1, &vec2);
                vec2 += _this->current.pos;

                Vec pos;
                pos.x = vec2.x;
                pos.y = vec2.y + 100.0f;
                pos.z = vec2.z;

                gnd_chk.SetPos(&pos);
                vec2.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                vec1 = vec2 - _this->current.pos;

                if (fabsf(vec1.y) < 50.0f) {
                    _this->current.angle.x =
                        cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z));
                }
            }
        }
    } else {
        if (i_this->field_0x608 < 1.0f && dansa_check(i_this, _this->current.pos, 100.0f) > 0) {
            i_this->mHangAngleY = hang_set(i_this);

            if (i_this->mHangAngleY != 0xdcf) {
                i_this->mAction = ACT_HANG;
                i_this->mMode = 0;
                i_this->mASwimTimer = 0;
            }
        }
    }

    water_check(i_this);

    if (swim_check) {
        if (i_this->mAction != ACT_CARRY) {
            if (i_this->mASwimTimer != 0) {
                i_this->mASwimTimer--;
                if (i_this->mASwimTimer == 0) {
                    i_this->mAction = ACT_A_SWIM;
                    i_this->mMode = 0;
                }
            }

            if (i_this->mWaterY - _this->current.pos.y > 45.0f) {
                i_this->mAction = ACT_SWIM;
                i_this->mMode = 0;
                i_this->field_0x608 = 0.0f;

                if (i_this->mSwimEffectTimer == 0) {
                    i_this->mSwimEffectTimer = 20;
                    cXyz pos = _this->current.pos;
                    pos.y = i_this->mWaterY;

                    static cXyz scc(0.7f, 0.7f, 0.7f);
                    static u16 w_eff_id[4] = {0x1B8, 0x1B9, 0x1BA, 0x1BB};

                    for (int i = 0; i < 4; i++) {
                        i_this->mParticleKey[i] =
                            dComIfGp_particle_set(i_this->mParticleKey[i], w_eff_id[i], &pos,
                                                  &_this->tevStr, NULL, &scc, 0xff, NULL, -1,
                                                  NULL, NULL, NULL);
                    }

                    i_this->mSound.startSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
                }
            }
        }
    } else {
        i_this->mASwimTimer = cM_rndF(25.0f) + 30.0f;

        if (i_this->mWaterY - _this->current.pos.y < 35.0f) {
            i_this->mAction = ACT_WAIT_1;
            i_this->mMode = 0;
        }
    }

    cLib_addCalcAngleS2(&_this->shape_angle.y, _this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&_this->shape_angle.x, _this->current.angle.x, 4, 0x1000);
    _this->shape_angle.z = _this->current.angle.z;

    s16 head_angle_y = 0;
    s16 head_angle_x = 0;

    if (i_this->field_0x616 != 0 || i_this->mLookTimer != 0) {
        if (i_this->field_0x616 == 2) {
            if (i_this->mCounter & 8) {
                head_angle_y = 10000;
            } else {
                head_angle_y = -10000;
            }
        } else {
            if (i_this->mLookTimer != 0) {
                i_this->mLookTimer--;
                vec1 = i_this->mLookPos - _this->eyePos;
            } else {
                vec1 = player->eyePos - _this->eyePos;
            }

            vec1.y += i_this->mScale.z * -16.0f;

            head_angle_y = cM_atan2s(vec1.x, vec1.z) - _this->shape_angle.y;
            head_angle_x = _this->shape_angle.x +
                cM_atan2s(vec1.y, JMAFastSqrt(vec1.x * vec1.x + vec1.z * vec1.z));

            if (i_this->mLookTimer == 0 && (head_angle_y > 24000 || head_angle_y < -24000)) {
                head_angle_y = 0;
            }

            if (head_angle_y > 12000) {
                head_angle_y = 12000;
            } else if (head_angle_y < -12000) {
                head_angle_y = -12000;
            }

            if (head_angle_x > 12000) {
                head_angle_x = 12000;
            } else if (head_angle_x < -12000) {
                head_angle_x = -12000;
            }

            head_angle_y += i_this->mHeadTilt;
        }

        i_this->field_0x616 = 0;
    }

    cLib_addCalcAngleS2(&i_this->mHeadAngle.y, head_angle_y / 2, 4, 0x2000);
    cLib_addCalcAngleS2(&i_this->mHeadAngle.x, head_angle_x / 2, 4, 0x2000);
    cLib_addCalcAngleS2(&i_this->mHeadTilt, 0, 2, 0x300);
    cLib_addCalcAngleS2(&i_this->mHeadBob.y, i_this->mHeadBobTarget.y, 4, 0x400);
    cLib_addCalcAngleS2(&i_this->mHeadBob.z, i_this->mHeadBobTarget.z, 4, 0x400);

    if (fabsf(player->speedF) < 2.0f && fabsf(_this->speedF) < 2.0f) {
        if (i_this->mHeadBobTimer != 0) {
            i_this->mHeadBobTimer--;
            if (i_this->mHeadBobTimer == 0) {
                i_this->mHeadBobTimer = cM_rndF(100.0f) + 20.0f;

                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mHeadBobTarget.y = cM_rndFX(2000.0f);
                }

                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mHeadBobTarget.z = cM_rndFX(2000.0f);
                }
            }
        }
    } else {
        i_this->mHeadBobTarget.z = 0;
        i_this->mHeadBobTarget.y = 0;
        i_this->mHeadBobTimer = cM_rndF(60.0f) + 80.0f;
    }

    if (i_this->mShakeSpeed >= 0.025f) {
        if (l_HIO.mWaterHuntAnimType != 0) {
            f32 fvar17 = i_this->mShakeSpeed * 3000.0f;
            i_this->mHeadAngle.y = fvar17 * cM_ssin(i_this->mCounter * 22000) * 0.5f;
            i_this->mHeadAngle.z = fvar17 * cM_ssin(i_this->mCounter * 25000);
        } else {
            i_this->mHeadAngle.z = 0;
            i_this->mHeadAngle.y = 0;
        }
    }

    cLib_addCalc2(&i_this->mShakeSpeed, i_this->mShakeTargetSpeed, 1.0f, 0.15f);
    cLib_addCalc0(&i_this->mShakeTargetSpeed, 1.0f, 0.04f);

    i_this->mTailAngle.y = i_this->mTailWag * cM_ssin(i_this->mTailPhaseY);
    i_this->mTailAngle.x = i_this->mTailWag * cM_ssin(i_this->mTailPhaseX) * 0.5f;

    i_this->mTailPhaseY += i_this->mTailWag * 2.0f;
    if (i_this->mTailPhaseY > 65536.0f) {
        i_this->mTailPhaseY -= 65536.0f;
    }

    i_this->mTailPhaseX += i_this->mTailWag * 1.5f;
    if (i_this->mTailPhaseX > 65536.0f) {
        i_this->mTailPhaseX -= 65536.0f;
    }

    cLib_addCalc2(&i_this->mTailWag, i_this->mTailWagTarget, 1.0f, 100.0f);
    i_this->mTailWagTarget = 0.0f;

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

static void message(do_class* i_this) {
    if (i_this->mIsTalking) {
        i_this->mCcDisableTimer = 10;

        if (i_this->mMsg.doFlow(i_this, 0, 0)) {
            dComIfGp_event_reset();
            i_this->mIsTalking = 0;
        }

    } else {
        if (dComIfGp_event_runCheck() && i_this->eventInfo.checkCommandTalk()) {
            i_this->mMsg.init(i_this, i_this->mFlowID, 0, 0);
            i_this->mIsTalking = 1;
        }

        if (i_this->mMessageState == 2 && i_this->mFlowID != -1 && daPy_py_c::checkNowWolf()) {
            fopAcM_OnStatus(i_this, 0);
            cLib_onBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
            i_this->eventInfo.onCondition(dEvtCnd_CANTALK_e);
        } else {
            fopAcM_OffStatus(i_this, 0);
            cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        }
    }
}

static int daDo_Execute(do_class* i_this) {
    if (cDmrNowMidnaTalk() && !fopAcM_checkCarryNow(i_this)) {
        return 1;
    }

    i_this->mCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimer[i] != 0) {
            i_this->mTimer[i]--;
        }
    }

    if (i_this->mCcDisableTimer != 0) {
        i_this->mCcDisableTimer--;
    }

    if (i_this->mSwimEffectTimer != 0) {
        i_this->mSwimEffectTimer--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    i_this->scale.set(i_this->mScale.x * l_HIO.mBaseSize,
                     i_this->mScale.y * l_HIO.mBaseSize,
                     i_this->mScale.z * l_HIO.mBaseSize);
    mDoMtx_stack_c::scaleM(i_this->scale.x, i_this->scale.y, i_this->scale.z);
    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->play(&i_this->eyePos, 0, 0);

    if (i_this->mAnmID == ANM_JOYFUL) {
        if (i_this->mpMorf->checkFrame(16.0f)) {
            i_this->mpMorf->setFrame(0.0f);
        }
    } else if (i_this->mAnmID == ANM_RUN) {
        if (i_this->mpMorf->checkFrame(15.0f)) {
            i_this->mpMorf->setFrame(0.0f);
        }
    }

    if ((i_this->mAnmID == ANM_JOYFUL || i_this->mAnmID == ANM_STEP) && i_this->mTimer[3] == 0) {
        i_this->mTimer[3] = cM_rndF(4.0f) + 13.0f;
        i_this->mSound.startSound(Z2SE_DOG_BREATH, 0, -1);
    }

    i_this->mpBtk->setFrame(i_this->mBtkFrame);
    i_this->mpBtp->setFrame(i_this->mBtpFrame);
    i_this->mpMorf->modelCalc();

    cXyz vec, center;
    MTXCopy(model->getAnmMtx(10), *calc_mtx);
    vec.set(5.0f, 5.0f, 0.0f);
    MtxPosition(&vec, &i_this->eyePos);
    i_this->attention_info.position = i_this->eyePos;
    i_this->attention_info.position.y += i_this->mScale.y * 16.0f;
    
    vec.set(-15.0f, -15.0f, 0.0f);
    MtxPosition(&vec, &center);
    if (i_this->mCcDisableTimer != 0) {
        center.y += 13000.0f;
    }
    i_this->mCcSph.SetC(center);

    if (i_this->mAction == ACT_A_SWIM) {
        i_this->mCcSph.SetR(i_this->mScale.z * 45.0f);
    } else {
        i_this->mCcSph.SetR(i_this->mScale.z * 30.0f);
    }

    dComIfG_Ccsp()->Set(&i_this->mCcSph);

    if (i_this->mItemActorID != -1) {
        daItem_c* item = static_cast<daItem_c*>(fopAcM_SearchByID(i_this->mItemActorID));
        if (item != NULL) {
            if (i_this->mItemControlTimer < 10) {
                i_this->mItemControlTimer++;
                if (i_this->mItemControlTimer == 10) {
                    item->startControl();
                    i_this->mItemControlTimer = 20;
                }
            }

            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(10), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 0);
            cMtx_XrotM(*calc_mtx, 0);
            cMtx_ZrotM(*calc_mtx, 0);
            MtxTrans(20.0f, -24.0f, 0.0f, 1);
            vec.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&vec, &item->current.pos);
            
            item->shape_angle = i_this->shape_angle;
            item->current.angle = item->shape_angle;
            item->speed.zero();
            item->speedF = 0.0f;
        }
    }

    if (i_this->mFoodInMouth) {
        obj_food_class* food =
            static_cast<obj_food_class*>(fopAcM_SearchByID(i_this->mFoodActorID));
        if (food != NULL) {
            if (fopAcM_GetName(food) == PROC_OBJ_FOOD) {
                MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(10), *calc_mtx);
                cMtx_YrotM(*calc_mtx, 0x4000);
                MtxTrans(0.0f, -7.0f, 25.0f, 1);
                food->mpModel->setBaseTRMtx(*calc_mtx);
                vec.set(0.0f, 0.0f, 0.0f);
                MtxPosition(&vec, &food->current.pos);
                fopAcM_SetFoodStatus(food, fopAcM_FOOD_3);
            }
        } else {
            //!@bug: the last two lines will fail because this path only executes if food is null
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(10), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 0x4000);
            MtxTrans(0.0f, -7.0f, 25.0f, 1);
            vec.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&vec, &food->current.pos);
            fopAcM_SetFoodStatus(food, fopAcM_FOOD_3);
        }
        i_this->mFoodInMouth = 0;
    }

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    message(i_this);
    return 1;
}

static int daDo_IsDelete(do_class* i_this) {
    return 1;
}

static int daDo_Delete(do_class* i_this) {
    u32 actor_id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "Do");
    if (i_this->mHIOInit) {
        hioInit = false;
    }
    if (i_this->heap) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    do_class* _this = (do_class*)i_this;

    _this->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Do", 25), NULL, NULL,
                                       (J3DAnmTransform*)dComIfG_getObjectRes("Do", 14), 2,
                                       1.0f, 0, -1, 1, NULL, 0x80000, 0x11020284);

    if (_this->mpMorf == NULL || _this->mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = _this->mpMorf->getModel();
    _this->model = model;
    model->setUserArea((uintptr_t)i_this);

    for (u16 jnt_no = 0; jnt_no < model->getModelData()->getJointNum(); jnt_no++) {
        if (jnt_no == 9 || jnt_no == 10 || jnt_no == 22 || jnt_no == 23) {
            model->getModelData()->getJointNodePointer(jnt_no)->setCallBack(nodeCallBack);
        }
    }

    _this->mpBtk = new mDoExt_btkAnm();
    if (_this->mpBtk == NULL) {
        return 0;
    }

    if (!_this->mpBtk->init(_this->mpMorf->getModel()->getModelData(),
                            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Do", 29), TRUE, 0,
                            1.0f, 0, -1))
    {
        return 0;
    }

    _this->mpBtp = new mDoExt_btpAnm();
    if (_this->mpBtp == NULL) {
        return 0;
    }

    if (!_this->mpBtp->init(_this->mpMorf->getModel()->getModelData(),
                            (J3DAnmTexPattern*)dComIfG_getObjectRes("Do", 32), TRUE, 0,
                            1.0f, 0, -1))
    {
        return 0;
    }

    _this->mSound.init(&_this->current.pos, 1);
    return 1;
}

static cPhs__Step daDo_Create(fopAc_ac_c* i_this) {
    do_class* _this = static_cast<do_class*>(i_this);
    fopAcM_ct(i_this, do_class);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhase, "Do");

    if (step == cPhs_COMPLEATE_e) {
        _this->mParam0 = fopAcM_GetParam(i_this) & 7;
        _this->mParam1 = fopAcM_GetParam(i_this) >> 4 & 0xf;
        _this->mParam2 = (u8)(fopAcM_GetParam(i_this) >> 8);
        if (_this->mParam0 != 2) {
            _this->mParam0 = 0;
        }

        _this->mFlowID = i_this->current.angle.x;
        i_this->shape_angle.x = 0;
        i_this->current.angle.x = 0;

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x2120)) {
            return cPhs_ERROR_e;
        }

        if (!hioInit) {
            _this->mHIOInit = true;
            hioInit = 1;
            l_HIO.field_0x04 = -1;
        }

        fopAcM_OnStatus(i_this, fopAcStts_CULL_e);
        fopAcM_OnCarryType(i_this, fopAcM_CARRY_TYPE_8);
        i_this->attention_info.flags = 0;
        i_this->attention_info.distances[fopAc_attn_CARRY_e] = 7;
        fopAcM_SetMtx(i_this, _this->mpMorf->getModel()->getBaseTRMtx());
        _this->mBgS_Acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this,
                             1, &_this->mBgS_AcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        _this->mBgS_AcchCir.SetWall(30.0f, 30.0f);
        i_this->health = 1;
        _this->field_0x560 = 1;
        _this->mStts.Init(100, 0, i_this);

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
        _this->mCcSph.Set(cc_sph_src);
        _this->mCcSph.SetStts(&_this->mStts);
        _this->mCcSph.OnTgNoHitMark();

        if (_this->mParam1 == 0xff) {
            _this->mBtkFrame = cM_rndF(3.999f);
        } else {
            _this->mBtkFrame = _this->mParam1;
            if (_this->mBtkFrame > 3) {
                _this->mBtkFrame = 3;
            }
        }

        _this->mTailPhaseY = cM_rndF(0x10000);
        _this->mTailPhaseX = cM_rndF(0x10000);
        _this->mCounter = cM_rndF(0x10000);
        _this->mScale.set(1.0f, 1.0f, 1.0f);

        if (_this->mParam0 == 2) {
            _this->mAction = ACT_HELP;
            i_this->current.pos.set(7216.0f, 220.0f, -4808.0f);
        }

        noFallCheck = false;
        if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) != ST_FIELD) {
            noFallCheck = true;
            _this->mParam2 = 200;
        }
        
        _this->mItemActorID = -1;
        daDo_Execute(_this);
    }

    return step;
}

AUDIO_INSTANCES;

static actor_method_class l_daDo_Method = {
    (process_method_func)daDo_Create,
    (process_method_func)daDo_Delete,
    (process_method_func)daDo_Execute,
    (process_method_func)daDo_IsDelete,
    (process_method_func)daDo_Draw,
};

extern actor_process_profile_definition g_profile_DO = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_DO,
    &g_fpcLf_Method.base,
    sizeof(do_class),
    0,
    0,
    &g_fopAc_Method.base,
    0x2BB,
    &l_daDo_Method,
    0x8044000,
    fopAc_NPC_e,
    fopAc_CULLBOX_0_e,
};
