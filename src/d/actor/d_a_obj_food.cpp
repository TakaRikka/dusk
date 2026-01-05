/**
 * d_a_obj_food.cpp
 * Object - Bone / Ball
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_food.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"

static bool hio_set;

static daObj_Food_HIO_c l_HIO;

daObj_Food_HIO_c::daObj_Food_HIO_c() {
    field_0x4 = -1;
    mBoneScale = 1.0f;
}

static int daObj_Food_Draw(obj_food_class* i_this) {
    if (daPy_py_c::wolfGrabSubjectNoDraw(i_this)) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);

    if (!fopAcM_checkCarryNow(i_this)) {
        if (i_this->mType != obj_food_class::TYPE_BALL) {
            cXyz pos;
            pos.set(i_this->current.pos.x, i_this->current.pos.y + 50.0f, i_this->current.pos.z);
            i_this->mShadowKey =
                dComIfGd_setShadow(i_this->mShadowKey, 1, i_this->mpModel, &pos, 200.0f, 0.0f,
                                   i_this->current.pos.y, i_this->mAcch.GetGroundH(),
                                   i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f,
                                   dDlst_shadowControl_c::getSimpleTex());
        } else {
            dComIfGd_setSimpleShadow(&i_this->current.pos, i_this->mAcch.GetGroundH(),
                                     19.0f, i_this->mAcch.m_gnd, 0, 1.0f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }

    return 1;
}

static void ground_ang_set(obj_food_class* i_this) {
    dBgS_GndChk gnd_chk;
    Vec vec1;
    cXyz vec2 = i_this->current.pos;
    vec2.y = i_this->mAcch.GetGroundH();

    vec1.x = vec2.x;
    vec1.y = vec2.y + 50.0f;
    vec1.z = vec2.z + 10.0f;
    gnd_chk.SetPos(&vec1);
    vec1.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (vec1.y != -G_CM3D_F_INF) {
        i_this->mGroundAngleX = -cM_atan2s(vec1.y - vec2.y, vec1.z - vec2.z);
    }

    vec1.x = vec2.x + 10.0f;
    vec1.y = vec2.y + 50.0f;
    vec1.z = vec2.z;
    gnd_chk.SetPos(&vec1);
    vec1.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (vec1.y != -G_CM3D_F_INF) {
        i_this->mGroundAngleZ = cM_atan2s(vec1.y - vec2.y, vec1.x - vec2.x);
    }
}


static void food_carry(obj_food_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    i_this->speed.y = 0.0f;

    if (!fopAcM_checkCarryNow(i_this)) {
        i_this->mCcSph.OnCoSetBit();
        if (i_this->speedF > 0.0f) {
            i_this->speedF = 70.0f;
            i_this->speed.y = 50.0f;
            fopAcM_SetFoodStatus(i_this, fopAcM_FOOD_1);
            i_this->mRotSpeed.z = -0x1200;
            i_this->mRotSpeed.y = 0;
            i_this->current.angle.y = player->shape_angle.y;
            i_this->shape_angle.y = player->shape_angle.y + 0x4000;
            i_this->mBounces = 0;
        } else {
            fopAcM_SetFoodStatus(i_this, fopAcM_FOOD_0);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_TOY_BONE_PUT, &i_this->current.pos,
                                     0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            i_this->mBounces = 3;
        }
        fopAcM_carryOffRevise(i_this);
    }

    cLib_addCalcAngleS2(&i_this->mGroundAngleX, 0, 1, 0x800);
    cLib_addCalcAngleS2(&i_this->mGroundAngleZ, 0, 1, 0x800);
}

static s16 wall_angle_get(obj_food_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    cXyz vec1[2], vec2, vec3;
    cMtx_YrotS(*calc_mtx, _this->current.angle.y);
    
    vec2.x = 0.0f;
    vec2.y = 2.0f;
    vec2.z = -20.0f;
    MtxPosition(&vec2, &vec3);
    vec3 += _this->current.pos;
    
    vec2.x = 15.0f;
    vec2.y = 2.0f;
    vec2.z = 100.0f;

    for (int i = 0; i < 2; i++) {
        MtxPosition(&vec2, &vec1[i]);
        vec2.x *= -1.0f;
        vec1[i] += vec3;

        lin_chk.Set(&vec3, &vec1[i], _this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            vec1[i] = lin_chk.GetCross();
        } else {
            return 0x23;
        }
    }

    vec2 = vec1[1] - vec1[0];
    return cM_atan2s(vec2.x, vec2.z) + 0x4000;
}

static void food_normal(obj_food_class* i_this) {
    if (i_this->mAcch.ChkGroundHit()) {
        fopAcM_SetFoodStatus(i_this, fopAcM_FOOD_0);
        if (i_this->mBounces >= 3) {
            cLib_onBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            i_this->speedF = 0.0f;

            if (fopAcM_checkCarryNow(i_this)) {
                fopAcM_SetFoodStatus(i_this, fopAcM_FOOD_2);
                i_this->mBounces = 0;
                i_this->mCcSph.OffCoSetBit();
            }

            s16 target_angle;
            if (i_this->shape_angle.z < 0x4000 && i_this->shape_angle.z > -0x4000) {
                target_angle = 0;
            } else {
                target_angle = -0x8000;
            }
            cLib_addCalcAngleS2(&i_this->shape_angle.z, target_angle, 1, 0x1000);
        } else {
            i_this->mBounces++;

            if (i_this->speedF > 1.0f) {
                i_this->speedF *= 0.6f;
            } else if (i_this->mOldSpeedY < -40.0f) {
                i_this->speedF = cM_rndF(15.0f);
            }
            i_this->speed.y = i_this->mOldSpeedY * -0.5f;

            Z2GetAudioMgr()->seStart(Z2SE_OBJ_TOY_BONE_BOUND, &i_this->current.pos,
                                     fabsf(i_this->mOldSpeedY), 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            
            i_this->current.angle.y += (s16)cM_rndFX(8000.0f);

            if (i_this->mBounces == 3) {
                i_this->mRotSpeed.z = 0;
                i_this->mRotSpeed.y = 0;
            } else {
                i_this->mRotSpeed.y = cM_rndFX(8000.0f);
                i_this->mRotSpeed.z = cM_rndFX(8000.0f);
            }
        }
    } else {
        if (i_this->mTimer[1] == 0 && i_this->mAcch.ChkWallHit()) {
            s16 wall_angle = wall_angle_get(i_this);
            if (wall_angle != 0x23) {
                s16 refl_angle = 0x8000 - (s16)(i_this->current.angle.y - wall_angle) * 2;
                i_this->current.angle.y += refl_angle;
                i_this->speedF *= (f32)fabs(cM_scos(refl_angle / 2) * 0.5f) + 0.2f;
                i_this->speed.y = 0.0f;
                i_this->mTimer[1] = 10;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_TOY_BONE_BOUND, &i_this->current.pos,
                                         i_this->speedF, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }

        i_this->shape_angle.y += i_this->mRotSpeed.y;
        i_this->shape_angle.z += i_this->mRotSpeed.z;
        
        cXyz pos = i_this->current.pos;
        if (i_this->mBounces != 0 || i_this->speedF < 20.0f) {
            pos.z += 20000.0f;
        }
        i_this->mAtSph.SetC(pos);
        i_this->mAtSph.SetR(30.0f);
        dComIfG_Ccsp()->Set(&i_this->mAtSph);
    }
}

static void ball_move(obj_food_class* i_this) {
    if (i_this->mTimer[0] == 0 && (i_this->mCcSph.ChkTgHit() || i_this->mCcSph.ChkCoHit())) {
        cCcD_Obj* hit_obj;
        if (i_this->mCcSph.ChkTgHit()) {
            hit_obj = i_this->mCcSph.GetTgHitObj();
        } else {
            hit_obj = i_this->mCcSph.GetCoHitObj();
        }
        fopAc_ac_c* actor = dCc_GetAc(hit_obj->GetAc());

        cXyz vec = i_this->current.pos - actor->current.pos;
        i_this->current.angle.y = cM_atan2s(vec.x, vec.z) + (s16)cM_rndFX(8000.0f);
        i_this->mTimer[0] = 10;
        if (actor->field_0x560 == 0x23) {
            i_this->speedF = cM_rndF(1.0f) + 3.0f;
        } else {
            i_this->speedF = cM_rndF(2.5f) + 6.0f;
        }
    }

    if (i_this->mAcch.ChkWallHit() && i_this->mTimer[1] == 0) {
        i_this->mTimer[1] = 10;
        s16 wall_angle = wall_angle_get(i_this);
        if (wall_angle != 0x23) {
            s16 refl_angle = 0x8000 - (s16)(i_this->current.angle.y - wall_angle) * 2;
            i_this->current.angle.y += refl_angle;
            i_this->speedF *= 0.5f;
        }
    }

    if (i_this->mAcch.ChkGroundHit()) {
        cLib_addCalc0(&i_this->speedF, 1.0f, 0.5f);
    }
}

static void action(obj_food_class* i_this) {
    cXyz vec1, vec2;

    switch (i_this->field_0x567) {
    case fopAcM_FOOD_0:
        food_normal(i_this);
        break;
    case fopAcM_FOOD_1:
        food_normal(i_this);
        cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        break;
    case fopAcM_FOOD_2:
        food_carry(i_this);
        cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        break;
    case fopAcM_FOOD_3:
        cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        break;
    case fopAcM_FOOD_4:
        fopAcM_SetFoodStatus(i_this, fopAcM_FOOD_0);
        i_this->mBounces = 0;
        break;
    case fopAcM_FOOD_5:
        ball_move(i_this);
        break;
    }

    if (i_this->mType == obj_food_class::TYPE_BALL) {
        i_this->current.angle.x += (s16)(i_this->speedF * 700.0f);
        cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = i_this->speedF;
        MtxPosition(&vec1, &vec2);
        i_this->speed.x = vec2.x;
        i_this->speed.z = vec2.z;
        i_this->current.pos += i_this->speed;
        i_this->speed.y -= 3.0f;

        i_this->current.pos.y -= 12.0f;
        i_this->old.pos.y -= 12.0f;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        i_this->current.pos.y += 12.0f;
        i_this->old.pos.y += 12.0f;

        mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
        mDoMtx_stack_c::YrotM(i_this->current.angle.y);
        mDoMtx_stack_c::XrotM(i_this->current.angle.x);
        mDoMtx_stack_c::scaleM(0.6f, 0.6f, 0.6f);
        mDoMtx_stack_c::transM(0.0f, -20.0f, 0.0f);
        i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

        cXyz vec3(0.0f, 12.0f, 0.0f);
        i_this->mCcSph.SetC(i_this->current.pos + vec3);
        i_this->mCcSph.SetR(6.0f);
        dComIfG_Ccsp()->Set(&i_this->mCcSph);
    } else if (!fopAcM_CheckFoodStatus(i_this, fopAcM_FOOD_3)) {
        cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = i_this->speedF;
        MtxPosition(&vec1, &vec2);
        i_this->speed.x = vec2.x;
        i_this->speed.z = vec2.z;
        i_this->speed.y += i_this->gravity;
        i_this->current.pos += i_this->speed;
        if (i_this->speed.y < i_this->maxFallSpeed) {
            i_this->speed.y = i_this->maxFallSpeed;
        }
        i_this->mOldSpeedY = i_this->speed.y;

        cXyz vec3 = i_this->current.pos;
        vec3.y += 100.0f;
        f32 water_y;
        fopAcM_getWaterY(&vec3, &water_y);

        if (i_this->speed.y <= -20.0f
            && i_this->current.pos.y < water_y && i_this->old.pos.y >= water_y)
        {
            cXyz scale(0.7f, 0.7f, 0.7f);
            cXyz pos = i_this->current.pos;
            pos.y = water_y;
            static u16 w_eff_id[4] = {0x1B8, 0x1B9, 0x1BA, 0x1BB};
            for (int i = 0; i < 4; i++) {
                i_this->mParticleKey[i] =
                    dComIfGp_particle_set(i_this->mParticleKey[i], w_eff_id[i], &pos,
                                          &i_this->tevStr, NULL, &scale, 0xff, NULL, -1,
                                          NULL, NULL, NULL);
            }
            i_this->mSound.startSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
            i_this->speed.y = -1.0f;
            i_this->mRotSpeed.z = 0;
            i_this->mRotSpeed.y = 0;
            i_this->speedF = 0.0f;
        }

        if (i_this->current.pos.y <= water_y) {
            i_this->gravity = -0.5f;
            i_this->maxFallSpeed = -8.0f;
        } else {
            i_this->gravity = -5.0f;
            i_this->maxFallSpeed = -90.0f;
        }

        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        if (i_this->mAcch.ChkGroundHit()) {
            ground_ang_set(i_this);
        }

        mDoMtx_stack_c::transS(i_this->current.pos.x,
                               i_this->current.pos.y + 5.0f,
                               i_this->current.pos.z);
        mDoMtx_stack_c::XrotM(i_this->mGroundAngleX);
        mDoMtx_stack_c::ZrotM(i_this->mGroundAngleZ);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
        mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
        mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
        mDoMtx_stack_c::transM(0.0f, 0.0f, -5.0f);
        mDoMtx_stack_c::scaleM(l_HIO.mBoneScale, l_HIO.mBoneScale, l_HIO.mBoneScale);
        i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        fopAcM_SetFoodStatus(i_this, fopAcM_FOOD_4);
    }

    i_this->attention_info.position = i_this->eyePos = i_this->current.pos;
}

static int daObj_Food_Execute(obj_food_class* i_this) {
    i_this->mCounter++;
    for (int i = 0; i < 4; i++) {
        if (i_this->mTimer[i] != 0) {
            i_this->mTimer[i]--;
        }
    }
    action(i_this);
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    return 1;
}

static int daObj_Food_IsDelete(obj_food_class* i_this) {
    return 1;
}

static int daObj_Food_Delete(obj_food_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Obj_fd");
    if (i_this->mHIOInit) {
        hio_set = false;
    }
    if (i_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_food_class* _this = static_cast<obj_food_class*>(i_this);

    J3DModelData* model_data;
    if (_this->mType == obj_food_class::TYPE_BALL) {
        model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("Obj_fd", 3));
    } else {
        model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("Obj_fd", 4));
    }

    _this->mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (_this->mpModel == NULL) {
        return 0;
    }

    _this->mSound.init(&_this->current.pos, 1);
    return 1;
}

static cPhs__Step daObj_Food_Create(fopAc_ac_c* i_this) {
    obj_food_class* _this = static_cast<obj_food_class*>(i_this);
    fopAcM_ct(i_this, obj_food_class);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhase, "Obj_fd");
    if (step == cPhs_COMPLEATE_e) {
        _this->mType = (u8)fopAcM_GetParam(_this);
        _this->mParam1 = (u8)(fopAcM_GetParam(_this) >> 8);
        _this->mParam2 = (u8)(fopAcM_GetParam(_this) >> 0x10);
        if (_this->mType == 0xff) {
            _this->mType = obj_food_class::TYPE_BONE;
        }

        if (!fopAcM_entrySolidHeap(_this, useHeapInit, 0x810)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            _this->mHIOInit = true;
            hio_set = true;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(_this, _this->mpModel->getBaseTRMtx());
        _this->mCcStts.Init(10, 0, _this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };
        _this->mCcSph.Set(cc_sph_src);
        _this->mCcSph.SetStts(&_this->mCcStts);

        _this->mAcch.Set(fopAcM_GetPosition_p(_this), fopAcM_GetOldPosition_p(_this), _this, 1,
                         &_this->mAcchCir, fopAcM_GetSpeed_p(_this), NULL, NULL);
        
        if (_this->mType == obj_food_class::TYPE_BALL) {
            _this->mAcchCir.SetWall(20.0f, 20.0f);
            fopAcM_SetFoodStatus(_this, fopAcM_FOOD_5);
        } else {
            _this->mAcchCir.SetWall(30.0f, 30.0f);
            _this->attention_info.distances[fopAc_attn_CARRY_e] = 7;
            _this->gravity = -5.0f;
            fopAcM_SetFoodStatus(_this, fopAcM_FOOD_0);
            fopAcM_OnCarryType(_this, fopAcM_CARRY_UNK_30);

            static dCcD_SrcSph at_sph_src = {
                {
                    {0x0, {{AT_TYPE_THROW_OBJ, 0x0, 0x1f}, {0x0, 0x0}, 0x0}}, // mObj
                    {dCcD_SE_NONE, 0x1, 0x0, 0x0, 0x0}, // mGObjAt
                    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                    {0x0}, // mGObjCo
                }, // mObjInf
                {
                    {{0.0f, 0.0f, 0.0f}, 15.0f} // mSph
                } // mSphAttr
            };
            _this->mAtSph.Set(at_sph_src);
            _this->mAtSph.SetStts(&_this->mCcStts);
            _this->mAtSph.OffAtVsPlayerBit();
        }

        _this->gravity = -5.0f;
        _this->maxFallSpeed = -90.0f;

        daObj_Food_Execute(_this);
    }

    return step;
}

static actor_method_class l_daObj_Food_Method = {
    (process_method_func)daObj_Food_Create,
    (process_method_func)daObj_Food_Delete,
    (process_method_func)daObj_Food_Execute,
    (process_method_func)daObj_Food_IsDelete,
    (process_method_func)daObj_Food_Draw,
};

actor_process_profile_definition g_profile_OBJ_FOOD = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_OBJ_FOOD,
    &g_fpcLf_Method.base,
    sizeof(obj_food_class),
    0,
    0,
    &g_fopAc_Method.base,
    0x2C8,
    &l_daObj_Food_Method,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
