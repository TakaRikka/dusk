/**
 * @file d_a_obj_wdStick.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_wdStick.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_kankyo_mng.h"
#include <cmath.h>

static daWdStick_HIO_c l_HIO;

dCcD_SrcSph daWdStick_c::mCcDSph = {daWdStick_c::mCcDObjInfo, {{{0.0f, 0.0f, 0.0f}, 4.0f}}};

daWdStick_HIO_c::daWdStick_HIO_c() {
    field_0x04 = 60;
    field_0x05 = 1;
    field_0x06 = 0xf;
    field_0x08 = 0;
}

void daWdStick_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mModel->setBaseScale(scale);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daWdStick_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("H_Kinobou", 4);
    JUT_ASSERT(245, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mModel != 0 ? 1 : 0;
}

dCcD_SrcGObjInf const daWdStick_c::mCcDObjInfo = {
    {0, {{0x200, 0, 0x1f}, {0xd8fbffff, 0x1f}, {0x0}}},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 4},
    {0},
};

int daWdStick_c::create() {
    fopAcM_ct(this, daWdStick_c);
    int rv = dComIfG_resLoad(&mPhase, "H_Kinobou");
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, createHeapCallBack, 0x820) == 0) {
            return cPhs_ERROR_e;
        }
        setBaseMtx();
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mModel->getModelData());
        lightInit();
        field_0xaac = 0;
        field_0xaad = 0;
        field_0xaae = 0;
        field_0xab0 = 0;
        setLight();
        mAcchCir.SetWall(10.0f, 35.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                            &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        fopAcM_SetGravity(this, -10.0f);
        fopAcM_SetMaxFallSpeed(this, -100.0f);
        fopAcM_SetModel(this, mModel);
        mStts.Init(10, 0xff, this);
        mSph1.Set(mCcDSph);
        mSph1.SetStts(&mStts);
        mSph2.Set(mCcDSph);
        mSph2.SetStts(&mStts);
        fopAcM_OnCarryType(this, fopAcM_CARRY_UNK_30);
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        attention_info.distances[4] = 59;
        misCarrying = fopAcM_checkCarryNow(this);
        mode_init_wait();
    }
    return rv;
}

int daWdStick_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daWdStick_c*>(i_this)->CreateHeap();
}

void daWdStick_c::lightInit() {
}

void daWdStick_c::setLight() {
}

void daWdStick_c::setFire() {
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    cXyz local_24(75.0f, 4.0f, 0.0f);
    cXyz cStack_30;
    mDoMtx_stack_c::multVec(&local_24, &cStack_30);
    field_0xa64 = current.pos + cStack_30;
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    local_24.x = -75.0f;
    mDoMtx_stack_c::multVec(&local_24, &cStack_30);
    field_0xa70 = current.pos + cStack_30;
    if (field_0xab0 != 0) {
        field_0xab0--;
    } else if (field_0xab0 == 0 && field_0xaad == 1) {
        field_0xaad = 0;
        mSph2.SetAtMtrl(dCcD_MTRL_NONE);
        mDoAud_seStart(Z2SE_OBJ_FIRE_OFF, &field_0xa70, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
    if (field_0xaae != 0) {
        field_0xaae--;
    } else if (field_0xaae == 0 && field_0xaac == 1) {
        field_0xaac = 0;
        mSph1.SetAtMtrl(dCcD_MTRL_NONE);
        mDoAud_seStart(Z2SE_OBJ_FIRE_OFF, &field_0xa64, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
    int i;
    if (field_0xaac != 0) {
        field_0xa94 = (field_0xa64 - field_0xa7c) * 0.7f;
        field_0xae4[0] = dComIfGp_particle_set(field_0xae4[0], 0x3ad, &field_0xa64, 0, 0);
        field_0xae4[1] = dComIfGp_particle_set(field_0xae4[1], 0x3af, &field_0xa64, 0, 0);
        mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &field_0xa64, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        for (i = 0; i < 2; i++) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0xae4[i]);
            if (emitter != NULL) {
                emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                emitter->setUserWork((uintptr_t)&field_0xa94);
            }
        }
    }
    if (field_0xaad != 0) {
        field_0xaa0 = (field_0xa70 - field_0xa88) * 0.7f;
        field_0xaec[0] = dComIfGp_particle_set(field_0xaec[0], 0x3ad, &field_0xa70, 0, 0);
        field_0xaec[1] = dComIfGp_particle_set(field_0xaec[1], 0x3af, &field_0xa70, 0, 0);
        mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &field_0xa70, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        for (i = 0; i < 2; i++) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0xaec[i]);
            if (emitter != NULL) {
                emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                emitter->setUserWork((uintptr_t)&field_0xaa0);
            }
        }
    }
}

int daWdStick_c::Execute() {
    field_0x594 = speed;
    field_0xa7c = field_0xa64;
    field_0xa88 = field_0xa70;
    setFire();
    field_0xac0 = field_0xab4;
    field_0xad8 = field_0xacc;
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    cXyz local_24(90.0f, 4.0f, 0.0f);
    cXyz cStack_30;
    mDoMtx_stack_c::multVec(&local_24, &cStack_30);
    field_0xab4 = current.pos + cStack_30;
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    local_24.x = -90.0;
    mDoMtx_stack_c::multVec(&local_24, &cStack_30);
    field_0xacc = current.pos + cStack_30;
    mode_proc_call();
    attention_info.position.x = current.pos.x;
    attention_info.position.y = current.pos.y;
    attention_info.position.z = current.pos.z;
    eyePos = current.pos;
    setBaseMtx();
    if (mSph1.ChkTgHit()) {
        dCcD_GObjInf* tgHitObj = mSph1.GetTgHitGObj();
        if (tgHitObj->GetAtType() == AT_TYPE_LANTERN_SWING && tgHitObj->GetAtMtrl() == dCcD_MTRL_FIRE &&
            field_0xaac == 0)
        {
            field_0xaac = 1;
            field_0xaae = l_HIO.field_0x06 * 30;
            mSph1.SetAtMtrl(dCcD_MTRL_FIRE);
            mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &field_0xa64, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            setLight();
        }
    }
    if (mSph2.ChkTgHit()) {
        dCcD_GObjInf* tgHitObj = mSph2.GetTgHitGObj();
        if (tgHitObj->GetAtType() == AT_TYPE_LANTERN_SWING &&
            tgHitObj->GetAtMtrl() == dCcD_MTRL_FIRE && field_0xaad == 0)
        {
            field_0xaad = 1;
            field_0xab0 = l_HIO.field_0x06 * 30;
            mSph2.SetAtMtrl(dCcD_MTRL_FIRE);
            mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &field_0xa70, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            setLight();
        }
    }
    field_0xaf4 = mAcch.ChkWaterIn();
    mSph1.SetR(30.0f);
    mSph1.SetC(field_0xa64);
    mSph2.SetR(30.0f);
    mSph2.SetC(field_0xa70);
    dComIfG_Ccsp()->Set(&mSph1);
    dComIfG_Ccsp()->Set(&mSph2);
    if (current.pos.y - home.pos.y <= -770.0f) {
        OS_REPORT("=== 木の棒再セット (%f) ===\n", current.pos.y - home.pos.y);
        field_0xaac = 0;
        field_0xaad = 0;
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        current.pos = home.pos;
        mode_init_wait();
    }
    return 1;
}

static f32 bound(cXyz* param_1, cBgS_PolyInfo const& param_2, f32 param_3) {
    cM3dGPla acStack_3c;
    if (dComIfG_Bgsp().GetTriPla(param_2, &acStack_3c) != 0) {
        cXyz cStack_48;
        f32 dVar5 = param_1->absXZ();
        C_VECReflect(param_1, &acStack_3c.mNormal, &cStack_48);
        *param_1 = (cStack_48 * dVar5) * param_3;
        return param_1->absXZ();
    }
    return 0.0f;
}

void daWdStick_c::bgCheck() {
    bool isRoofHit = mAcch.ChkRoofHit() != 0;
    bool isGroundLanding = mAcch.ChkGroundLanding() != 0;
    bool isWaterHit = mAcch.ChkWaterHit() != 0;
    bool isWaterIn = mAcch.ChkWaterIn() != 0;
    if (mAcch.ChkWallHit() && speed.abs() > 8.0f) {
        cXyz local_7c(speed);
        cXyz cStack_88(speed);
        bound(&local_7c, mAcchCir, 0.6f);
        speed.set(local_7c);
        speed.y = cStack_88.y;
        speedF = local_7c.absXZ();
        current.angle.y = cM_atan2s(local_7c.x, local_7c.z);
        if (current.pos.y - home.pos.y > -50.0f) {
            setBoundSe();
        }
    }
    if (mMode == MODE_WAIT || mMode == MODE_DROP || mMode == MODE_CARRY) {
        f32 dVar12 = mAcch.m_wtr.GetHeight();
        if (isWaterHit) {
            f32 dVar13 = speed.y;
            f32 dVar14 = dVar12;
            if (chkWaterLineIn() && mMode != 1) {
                mode_init_sink();
            }
            if (isWaterIn && field_0xaf4 == 0) {
                cXyz cStack_94(current.pos);
                cStack_94.y = dVar14;
                if (dVar13 < -15.0f) {
                    fopKyM_createWpillar(&cStack_94, 2.0f, 0);
                    fopAcM_seStart(this, Z2SE_OBJ_FALL_WATER_M, 0);
                }
            }
        }
    }
    if (isGroundLanding && speed.abs() > 8.0f) {
        f32 dVar12 = field_0x594.y;
        bound(&speed, mAcch.m_gnd,
                               0.6f);
        speed.y = (-dVar12 * 0.6f);
        if (fabsf(speed.y) < 5.0f) {
            speed.y = 0.0f;
        }
        if (current.pos.y - home.pos.y > -50.0f) {
            setBoundSe();
        }
    }
    if (isRoofHit) {
        f32 dVar12 = field_0x594.y;
        bound(&speed, mAcch.m_gnd, 0.6f);
        speed.y -= -dVar12 * 0.6f;
        OS_REPORT("HIT ROOF!!!!spdy<%f>spdy<%f>\n", dVar12, speed.y);
    }
    if (isGroundLanding && mMode == MODE_DROP) {
        mode_init_roll();
    }
}

void daWdStick_c::setBoundSe() {
    u32 uVar1 = fabsf(field_0x594.y) * 2.0f;
    if (uVar1 > 100) {
        uVar1 = 100;
    }
    mDoAud_seStart(Z2SE_OBJ_WOODSTICK_BOUND, &current.pos, uVar1, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daWdStick_c::mode_proc_call() {
    static daWdStick_c::modeFunc l_mode_func[5] = {
        &daWdStick_c::mode_proc_wait,
        &daWdStick_c::mode_proc_carry,
        &daWdStick_c::mode_proc_drop,
        &daWdStick_c::mode_proc_roll,
        &daWdStick_c::mode_proc_sink,
    };

    if (fopAcM_checkCarryNow(this) && !misCarrying) {
        mode_init_carry();
    }
    (this->*(l_mode_func[mMode]))();
    misCarrying = fopAcM_checkCarryNow(this);
}

void daWdStick_c::mode_init_wait() {
    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    speedF = 0.0f;
    gravity = -10.0f;
    mMode = MODE_WAIT;
}

void daWdStick_c::mode_proc_wait() {
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bgCheck();
}

void daWdStick_c::mode_init_carry() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    speedF = 0.0f;
    mMode = MODE_CARRY;
}

void daWdStick_c::mode_proc_carry() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();
    cXyz cStack_154(current.pos);
    dBgS_ObjLinChk adStack_b0;
    dBgS_ObjLinChk adStack_120;
    cXyz local_160;
    cXyz local_16c;
    dBgS_ObjLinChk* pdVar9 = NULL;
    local_160.x = 0.0f;
    local_160.y = 0.0f;
    local_160.z = 0.0f;
    local_16c.x = 0.0f;
    local_16c.y = 0.0f;
    local_16c.z = 0.0f;
    adStack_b0.Set(&current.pos, &field_0xab4, this);
    if (dComIfG_Bgsp().LineCross(&adStack_b0)) {
        if (dComIfG_Bgsp().GetWallCode(adStack_b0) != 7) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(adStack_b0, &plane);
            f32 dVar11 = plane.getPlaneFunc(&field_0xab4);
            VECScale(&plane.mNormal, &local_16c, dVar11 * -1.0f);
            pdVar9 = &adStack_b0;
        }
    }

    adStack_120.Set(&current.pos, &field_0xacc, this);
    if (dComIfG_Bgsp().LineCross(&adStack_120)) {
        if (dComIfG_Bgsp().GetWallCode(adStack_120) != 7) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(adStack_120, &plane);
            f32 dVar11 = plane.getPlaneFunc(&field_0xacc);
            VECScale(&plane.mNormal, &local_160, dVar11 * -1.0f);
            pdVar9 = &adStack_120;
        }
    }
    
    field_0x57c = local_160 + local_16c;
    player->setGrabCollisionOffset(field_0x57c.x, field_0x57c.z, pdVar9);
    current.pos = cStack_154;
    if (!fopAcM_checkCarryNow(this)) {
        if (fopAcM_GetSpeedF(this) != 0.0f) {
            mode_init_drop();
        } else {
            mode_init_roll();
            mDoAud_seStart(Z2SE_OBJ_WOODSTICK_PUT, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
    }
}

void daWdStick_c::mode_init_drop() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    gravity = -5.0f;
    mMode = MODE_DROP;
}

void daWdStick_c::mode_proc_drop() {
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bgCheck();
}

void daWdStick_c::mode_init_roll() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mMode = MODE_ROLL;
}

void daWdStick_c::mode_proc_roll() {
    f32 dVar14 = 70.0f;
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    dBgS_ObjGndChk adStack_d8;
    cXyz cStack_f8(current.pos);
    cStack_f8.y += 4.0f;
    adStack_d8.SetPos(&cStack_f8);
    f32 groundCross = dComIfG_Bgsp().GroundCross(&adStack_d8);
    cXyz cStack_104;
    f32 dVar13 = 0.75f;
    cM3dGPla cStack_ec;
    fopAcM_gc_c::gndCheck(&cStack_f8);
    bool iVar7 = fopAcM_gc_c::getTriPla(&cStack_ec);
    if ((groundCross != -G_CM3D_F_INF) && iVar7 != 0) {
        f32 dVar11 = cM_scos(cM_deg2s(dVar14 - 0.5f));
        cStack_104 = cStack_ec.mNormal;
        if (cStack_104.y < dVar11) {
            cXyz local_110(cStack_104);
            s16 sVar8 = cM_atan2s(local_110.x, local_110.z);
            f32 dVar11 = 10.0f;
            dVar11 *= (1.0f - cStack_104.y);
            if (dVar11 < 0.1f) {
                dVar11 = 0.1f;
            } else if (dVar11 > 1.0f) {
                dVar11 = 1.0f;
            }
            cXyz local_11c((speedF * cM_ssin(current.angle.y)), 0.0f,
                         speedF * cM_scos(current.angle.y));
            cXyz cStack_128(dVar11 * cM_ssin(sVar8), 0.0f, dVar11 * cM_scos(sVar8));
            local_11c += cStack_128;
            speedF = std::sqrt(local_11c.x * local_11c.x + local_11c.z * local_11c.z);
            current.angle.y = cM_atan2s(local_11c.x, local_11c.z);
        }
    }
    speedF *= dVar13;
    if (mStts.GetCCMoveP()->abs2XZ() > 0.01000000070780516f) {
        speedF *= 0.9f;
    }
    if (mAcch.ChkGroundHit()) {
        if (speedF < 0.3f) {
            mode_init_wait();
        }
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }
    mAcch.CrrPos(dComIfG_Bgsp());
    bgCheck();
}

f32 dummyLiteral() { return 100.0f; }

BOOL daWdStick_c::chkWaterLineIn() {
    return mAcch.m_wtr.GetHeight() > current.pos.y + 40.0f;
}

void daWdStick_c::mode_init_sink() {
    field_0xab0 = 0;
    field_0xaae = 0;
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mMode = MODE_SINK;
}

void daWdStick_c::mode_proc_sink() {
    cLib_addCalc(&speedF, 0, 0.05f, 0.1f, 0.01f);
    speed.y = cLib_minMaxLimit(speed.y, -15.0f, 13.0f);
    if (mAcch.ChkGroundHit()) {
        speedF *= 0.9f;
    }
    if (chkWaterLineIn()) {
        gravity = -2.0f;
    } else {
        gravity = -5.0f;
    }
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    f32 waterHeight = mAcch.m_wtr.GetHeight();
    if (current.pos.y < waterHeight + 100.0f) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        current.pos = home.pos;
        mode_init_wait();
    }
    if (mAcch.ChkGroundHit()) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }
}

int daWdStick_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    mDoExt_modelUpdateDL(mModel);
    return 1;
}

int daWdStick_c::Delete() {
    dComIfG_resDelete(&mPhase, "H_Kinobou");
    return 1;
}

static int daWdStick_Draw(daWdStick_c* i_this) {
    return i_this->Draw();
}

static int daWdStick_Execute(daWdStick_c* i_this) {
    return i_this->Execute();
}

static int daWdStick_Delete(daWdStick_c* i_this) {
    return i_this->Delete();
}

static int daWdStick_Create(fopAc_ac_c* i_this) {
    return static_cast<daWdStick_c*>(i_this)->create();
}

static actor_method_class l_daWdStick_Method = {
    (process_method_func)daWdStick_Create,
    (process_method_func)daWdStick_Delete,
    (process_method_func)daWdStick_Execute,
    NULL,
    (process_method_func)daWdStick_Draw,
};

extern actor_process_profile_definition g_profile_Obj_WdStick = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_WdStick,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daWdStick_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  497,                    // mPriority
  &l_daWdStick_Method,    // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
