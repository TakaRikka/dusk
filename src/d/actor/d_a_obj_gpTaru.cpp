/**
 * @file d_a_obj_gpTaru.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_gpTaru.h"
#include "d/actor/d_a_obj_eff.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_kankyo_mng.h"

static daGpTaru_HIO_c l_HIO;

daGpTaru_HIO_c::daGpTaru_HIO_c() {
    weight = 200.0f;
    explosion_wait_time = 15;
    shake = 200.0f;
    explosion_range = 200.0f;
    effect_scale.x = 3.0f;
    effect_scale.y = 3.0f;
    effect_scale.z = 3.0f;
    pre_smoke_scale.x = 6.0f;
    pre_smoke_scale.y = 6.0f;
    pre_smoke_scale.z = 6.0f;
    barrel_scale.x = 1.6f;
    barrel_scale.y = 1.6f;
    barrel_scale.z = 1.6f;
    vibration = 5;
    smoke_scale = 1.0f;
}

void daGpTaru_c::setBaseMtx() {
    scale.x = l_HIO.barrel_scale.x;
    scale.y = l_HIO.barrel_scale.y;
    scale.z = l_HIO.barrel_scale.z;

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::ZXYrotM(field_0x93c.x, field_0x93c.y, field_0x93c.z);
    mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);

    cXyz bmd_scale(1.0f, 1.0f, 1.0f);
    mpModel->setBaseScale(bmd_scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daGpTaru_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("K_ktar00", 3);
    JUT_ASSERT(295, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

dCcD_SrcCyl daGpTaru_c::mCcDCyl = {
    daGpTaru_c::mCcDObjInfo,
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        0.0f,                // mRadius
        0.0f                 // mHeight
    }  // mCyl
};

const dCcD_SrcGObjInf daGpTaru_c::mCcDObjInfo = {
    {0, {{0, 0, 0x1F}, {0xD8FAFDBF, 0x11}, {0x79}}},
    {dCcD_SE_NONE, 0, 1, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 2},
    {0},
};

int daGpTaru_c::create() {
    fopAcM_ct(this, daGpTaru_c);

    if (fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xFF)) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, "K_ktar00");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x820)) {
            return cPhs_ERROR_e;
        }

        setBaseMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mAcchCir.SetWall(160.0f, 80.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);

        fopAcM_SetGravity(this, -7.0f);
        fopAcM_SetMaxFallSpeed(this, -100.0f);
        fopAcM_SetModel(this, mpModel);

        mCcStts.Init(0xFE, 0xFF, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);

        field_0x954 = fopAcM_GetParam(this);
        fopAcM_OnCarryType(this, fopAcM_CARRY_TYPE_1);

        if (field_0x954 == 0) {
            cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            attention_info.distances[fopAc_attn_CARRY_e] = 42;
        } else {
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        }

        mIsPrevCarry = fopAcM_checkCarryNow(this) != 0;
        mode_init_wait();

        field_0x93c.x = 0;
        field_0x93c.y = 0;
        field_0x93c.z = 0;

        field_0x944 = 0;
        field_0x938 = 0.0f;
        mExplosionRange = 80.0f;
        field_0x955 = 1;

        #if DEBUG
        l_HIO.entryHIO("火薬樽");
        #endif
    }

    return phase_state;
}

int daGpTaru_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daGpTaru_c*)i_this)->CreateHeap();
}

static f32 bound(cXyz* param_0, const cBgS_PolyInfo& param_1, f32 param_2) {
    cM3dGPla plane;

    bool var_r29 = dComIfG_Bgsp().GetTriPla(param_1, &plane);
    if (var_r29) {
        cXyz pos;
        f32 abs = param_0->absXZ();

        VECReflect(param_0, &plane.mNormal, &pos);
        *param_0 = (pos * abs) * param_2;
        return param_0->absXZ();
    } else {
        return 0.0f;
    }
}

BOOL daGpTaru_c::chkSinkAll() {
    return mAcch.m_wtr.GetHeight() > 100.0f + current.pos.y;
}

BOOL daGpTaru_c::chkWaterLineIn() {
    return mAcch.m_wtr.GetHeight() > 40.0f + current.pos.y;
}

void daGpTaru_c::bgCheck() {
    bool is_roof_hit = mAcch.ChkRoofHit();
    bool is_wall_hit = mAcch.ChkWallHit() != 0;
    bool is_gnd_land = mAcch.ChkGroundLanding();
    bool is_gnd_hit = mAcch.ChkGroundHit();
    bool is_water_hit = mAcch.ChkWaterHit();
    bool is_water_in = mAcch.ChkWaterIn();

    if (is_wall_hit && speed.abs() > 8.0f) {
        cXyz sp2C(speed);
        cXyz sp20(speed);
        bound(&sp2C, mAcchCir, 0.6f);

        speed.set(sp2C);
        speed.y = sp20.y;
        speedF = sp2C.absXZ();
        current.angle.y = cM_atan2s(sp2C.x, sp2C.z);
    }

    if (is_gnd_land && speed.abs() > 8.0f) {
        f32 temp_f30 = field_0x594.y;
        bound(&speed, mAcch.m_gnd, 0.6f);

        speed.y = 0.6f * -temp_f30;
        if (std::fabs(speed.y) < 5.0f) {
            speed.y = 0.0f;
        }
    }

    if (is_roof_hit) {
        f32 temp_f31 = field_0x594.y;
        bound(&speed, mAcch.m_gnd, 0.6f);

        speed.y -= 0.6f * -temp_f31;
        OS_REPORT("HIT ROOF!!!!spdy<%f>spdy<%f>\n", temp_f31, speed.y);
    }

    if (is_gnd_land && mMode == 2) {
        mode_init_roll();
    }

    if (mMode == 0 || mMode == 2 || mMode == 1) {
        f32 wtr_height = mAcch.m_wtr.GetHeight();
        if (is_water_hit) {
            f32 spd_y = speed.y;

            if (chkWaterLineIn() && mMode != 1) {
                mode_init_sink();
            }

            if (is_water_in && !mIsPrevInWater) {
                cXyz pos(current.pos);
                pos.y = wtr_height;

                if (spd_y < -15.0f) {
                    fopKyM_createWpillar(&pos, 2.0f, 0);
                    fopAcM_seStart(this, Z2SE_OBJ_FALL_WATER_M, 0);
                }
            }
        }
    }

    if (is_gnd_hit && (mMode == 2 || mMode == 3) && !is_water_hit) {
        daObjEff::Act_c::make_land_smoke(&current.pos, l_HIO.smoke_scale);
        fopAcM_seStart(this, JA_SE_OBJ_PUT_WBOX_S, 0);
    }

    cXyz hamon_pos(current.pos);
    hamon_pos.y = mAcch.m_wtr.GetHeight();
    if (hamon_pos.y > current.pos.y && ((mMode == 6 && !chkSinkAll()) || (mMode == 0 && is_gnd_hit) || (mMode == 1 && is_water_in && !chkSinkAll()))) {
        fopAcM_effHamonSet(&field_0x950, &hamon_pos, 1.0f, 0.1f);
    }
}

int daGpTaru_c::Execute() {
    field_0x594 = speed;

    mode_proc_call();

    field_0x588 = current.pos;
    mAcch.CrrPos(dComIfG_Bgsp());

    field_0x57c = current.pos - field_0x588;

    if (mMode == 1) {
        current.pos = field_0x588;
    }

    bgCheck();

    attention_info.position.x = current.pos.x;
    attention_info.position.y = 50.0f + current.pos.y;
    attention_info.position.z = current.pos.z;
    eyePos = attention_info.position;

    mIsPrevInWater = mAcch.ChkWaterIn() != 0;

    setBaseMtx();

    if (mCcCyl.ChkTgHit()) {
        if (mCcCyl.GetTgHitGObj()->GetAtType() & 0xD8FAFDBF) {
            if (field_0x955 != 0) {
                mDoAud_seStart(Z2SE_OBJ_BOMBBARREL_HIT, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                mode_init_exploInit();
                field_0x955 = 0;
            }
        } else {
            fopAc_ac_c* tg_hit_actor = dCc_GetAc(mCcCyl.GetTgHitObj()->GetAc());
            if (tg_hit_actor != NULL && fopAcM_GetName(tg_hit_actor) == PROC_E_WB) {
                mode_init_explosion();
            }
        }
    }

    if (mCcCyl.ChkCoHit()) {
        fopAc_ac_c* co_hit_actor = dCc_GetAc(mCcCyl.GetCoHitObj()->GetAc());
        if (co_hit_actor != NULL && fopAcM_GetName(co_hit_actor) == PROC_E_WB) {
            if (co_hit_actor->speedF >= 7.0f) {
                mode_init_explosion();
            }
        }
    }

    mCcCyl.SetR(mExplosionRange + oREG_F(2));
    mCcCyl.SetH(170.0f + oREG_F(3));
    mCcCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCyl);
    mCcStts.Move();
    return 1;
}

void daGpTaru_c::mode_proc_call() {
    static void (daGpTaru_c::*l_mode_func[])() = {
        &daGpTaru_c::mode_proc_wait,
        &daGpTaru_c::mode_proc_carry,
        &daGpTaru_c::mode_proc_drop,
        &daGpTaru_c::mode_proc_roll,
        &daGpTaru_c::mode_proc_exploInit,
        &daGpTaru_c::mode_proc_explosion,
        &daGpTaru_c::mode_proc_sink,
    };

    if (fopAcM_checkCarryNow(this) && !mIsPrevCarry) {
        mode_init_carry();
    }

    (this->*l_mode_func[mMode])();
    
    mIsPrevCarry = fopAcM_checkCarryNow(this) != 0;
}

void daGpTaru_c::mode_init_wait() {
    if (field_0x954 == 0) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    speedF = 0.0f;
    gravity = -7.0f;
    mMode = 0;
}

void daGpTaru_c::mode_proc_wait() {
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
}

void daGpTaru_c::mode_init_carry() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    speedF = 0.0f;
    mMode = 1;
}

void daGpTaru_c::mode_proc_carry() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    player->setGrabCollisionOffset(field_0x57c.x, field_0x57c.z, NULL);

    if (!fopAcM_checkCarryNow(this)) {
        if (chkWaterLineIn()) {
            mode_init_sink();
        } else if (fopAcM_GetSpeedF(this) != 0.0f) {
            mode_init_drop();
        } else {
            mode_init_roll();
        }
    }
}

void daGpTaru_c::mode_init_drop() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    gravity = -l_HIO.weight;
    mMode = 2;
}

void daGpTaru_c::mode_proc_drop() {
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
}

void daGpTaru_c::mode_init_roll() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mMode = 3;
}

void daGpTaru_c::mode_proc_roll() {
    f32 temp_f30 = 70.0f;
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    
    dBgS_ObjGndChk gnd_chk;
    cXyz sp38(current.pos);
    sp38.y += 4.0f;

    gnd_chk.SetPos(&sp38);
    f32 gnd_height = dComIfG_Bgsp().GroundCross(&gnd_chk);

    cXyz sp2C;
    f32 temp_f27 = 0.1f;

    cM3dGPla tripla;
    bool var_r26 = fopAcM_gc_c::gndCheck(&sp38);
    bool found_tripla = fopAcM_gc_c::getTriPla(&tripla);

    if (-G_CM3D_F_INF != gnd_height && found_tripla) {
        f32 temp_f28 = cM_scos(cM_deg2s(temp_f30 - 0.5f));
        sp2C = tripla.mNormal;
        if (sp2C.y < temp_f28) {
            cXyz sp20(sp2C);
            s16 temp_r28 = cM_atan2s(sp20.x, sp20.z);

            f32 var_f31 = 10.0f;
            var_f31 *= 1.0f - sp2C.y;
            if (var_f31 < 0.1f) {
                var_f31 = 0.1f;
            } else if (var_f31 > 1.0f) {
                var_f31 = 1.0f;
            }

            cXyz sp14(speedF * cM_ssin(current.angle.y), 0.0f, speedF * cM_scos(current.angle.y));
            cXyz sp8(var_f31 * cM_ssin(temp_r28), 0.0f, var_f31 * cM_scos(temp_r28));
            sp14 += sp8;
            speedF = std::sqrt(SQUARE(sp14.x) + SQUARE(sp14.z));
            current.angle.y = cM_atan2s(sp14.x, sp14.z);
        }
    }

    speedF *= temp_f27;
    if (mCcStts.GetCCMoveP()->abs2XZ() > SQUARE(0.1f)) {
        speedF *= 0.9f;
    }

    if (mAcch.ChkGroundHit()) {
        if (speedF < 0.3f) {
            mode_init_wait();
        }
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }
}

void daGpTaru_c::mode_init_exploInit() {
    mExploTimer = l_HIO.explosion_wait_time;
    field_0x938 = l_HIO.shake;
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);

    mWind.position = current.pos;
    mWind.mDirection.x = 0.0f;
    mWind.mDirection.y = 1.0f;
    mWind.mDirection.z = 0.0f;
    mWind.mRadius = 500.0f;
    mWind.field_0x20 = 0.0f;
    mWind.mStrength = 0.5f;
    dKyw_pntwind_set(&mWind);

    field_0x9a8 = 0;
    mExploWindStrength = 0.0f;
    mMode = 4;
}

void daGpTaru_c::mode_proc_exploInit() {
    if (mExploTimer != 0) {
        mDoAud_seStartLevel(Z2SE_OBJ_BOMBBARREL_IGNITION, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        mExploTimer--;
    } else {
        mDoAud_seStop(Z2SE_OBJ_BOMBBARREL_IGNITION, 0);
        mode_init_explosion();
    }

    cXyz sp18(current.pos);
    sp18.x -= 20.0f;
    sp18.y += 100.0f * l_HIO.barrel_scale.y;

    mSmokeEmtID = dComIfGp_particle_set(mSmokeEmtID, ID_ZI_J_ARWB_SMOKE_A, &sp18, &tevStr, NULL, &l_HIO.pre_smoke_scale, 0xFF, NULL, -1, NULL, NULL, NULL);

    field_0x93c.x = field_0x938 * cM_scos(field_0x944 * 0x3000);
    field_0x93c.z = field_0x938 * cM_ssin(field_0x944 * 0x3000);
    field_0x944++;
}

void daGpTaru_c::mode_init_explosion() {
    mWind.mStrength = mExploWindStrength;

    switch (field_0x9a8) {
    case 0:
        cLib_addCalc(&mExploWindStrength, 1.0f, 0.5f, 0.4f, 0.01f);
        if (mExploWindStrength >= 0.99f) {
            field_0x9a8++;
        }
        break;
    case 1:
        cLib_addCalc(&mExploWindStrength, 0.0f, 0.05f, 0.04f, 0.001f);
        if (mExploWindStrength <= 0.01f) {
            field_0x9a8++;
        }
        break;
    }

    mDoAud_seStart(Z2SE_OBJ_BOMBBARREL_BREAK, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    dComIfGp_getVibration().StartShock(l_HIO.vibration, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    breakEffSet();

    mCcCyl.SetAtType(AT_TYPE_BOMB);
    mCcCyl.SetAtAtp(4);
    mExplosionRange = l_HIO.explosion_range;
    mMode = 5;
}

void daGpTaru_c::mode_proc_explosion() {
    fopAcM_onSwitch(this, fopAcM_GetParam(this) & 0xFF);
    fopAcM_delete(this);
}

void daGpTaru_c::mode_init_sink() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mMode = 6;
}

void daGpTaru_c::mode_proc_sink() {
    cLib_addCalc(&speedF, 0.0f, 0.05f, 0.1f, 0.01f);
    speed.y = cLib_minMaxLimit<f32>(speed.y, -15.0f, 13.0f);

    if (mAcch.ChkGroundHit()) {
        speedF *= 0.9f;
    }

    if (chkWaterLineIn()) {
        gravity = -10.0f;
    } else {
        gravity = -20.0f;
    }

    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());

    if (mAcch.ChkGroundHit()) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }
}

void daGpTaru_c::breakEffSet() {
    cXyz pos(current.pos);
    cXyz size(0.75f, 0.75f, 0.75f);

    J3DModelData* kibako_bmd = (J3DModelData*)dComIfG_getObjectRes("Always", "BreakWoodBox.bmd");
    JUT_ASSERT(1259, kibako_bmd != NULL);

    JPABaseEmitter* emt_p = dComIfGp_particle_set(dPa_RM(ID_ZF_S_M_EXBARREL00BOARD), &pos, NULL, NULL, 0xFF, &dPa_modelEcallBack::getEcallback(), fopAcM_GetRoomNo(this), NULL, NULL, &size);
    dPa_modelEcallBack::setModel(emt_p, kibako_bmd, tevStr, 3, NULL, 0, 0);

    for (int i = 0; i < 8; i++) {
        static u16 effName[] = {
            dPa_RM(ID_ZF_S_EXBARREL01BOARD),
            dPa_RM(ID_ZF_S_EXBARREL_B),
            dPa_RM(ID_ZF_S_EXBARREL_C),
            dPa_RM(ID_ZF_S_EXBARREL_E),
            dPa_RM(ID_ZF_S_EXBARREL_F),
            dPa_RM(ID_ZF_S_EXBARREL_G),
            dPa_RM(ID_ZF_S_EXBARREL_H),
            dPa_RM(ID_ZF_S_EXBARREL_I),
        };

        dComIfGp_particle_set(effName[i], &pos, &tevStr, NULL, &l_HIO.barrel_scale);
    }
}

int daGpTaru_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daGpTaru_c::Delete() {
    dKyw_pntwind_cut(&mWind);
    dComIfG_resDelete(&mPhase, "K_ktar00");

    #if DEBUG
    l_HIO.removeHIO(this);
    #endif
    return 1;
}

static int daGpTaru_Draw(daGpTaru_c* i_this) {
    return i_this->Draw();
}

static int daGpTaru_Execute(daGpTaru_c* i_this) {
    return i_this->Execute();
}

static int daGpTaru_Delete(daGpTaru_c* i_this) {
    return i_this->Delete();
}

static int daGpTaru_Create(fopAc_ac_c* i_this) {
    return ((daGpTaru_c*)i_this)->create();
}

static actor_method_class l_daGpTaru_Method = {
    (process_method_func)daGpTaru_Create,
    (process_method_func)daGpTaru_Delete,
    (process_method_func)daGpTaru_Execute,
    0,
    (process_method_func)daGpTaru_Draw,
};

extern actor_process_profile_definition g_profile_Obj_GpTaru = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_GpTaru,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daGpTaru_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  509,                    // mPriority
  &l_daGpTaru_Method,     // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
