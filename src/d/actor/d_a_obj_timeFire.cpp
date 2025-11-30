/**
 * @file d_a_obj_timeFire.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_timeFire.h"
#include "d/d_com_inf_game.h"

daTimeFire_HIO_c::daTimeFire_HIO_c() {
    field_0x4 = 20;
}

void daTimeFire_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daTimeFire_c::create() {
    fopAcM_ct(this, daTimeFire_c);
    setBaseMtx();
    field_0x570 = current.pos;
    lightInit();
    mStartTime = getStartTime();
    mEndTime = getEndTime();
    int scaleParam = getScale();
    f32 fVar1 = scaleParam / 10.0f;
    mParticleScale.set(fVar1, fVar1, fVar1);
    field_0x56c = fireCheck(1);
    if (!field_0x56c) {
        cutLight();
        field_0x56d = false;
    } else {
        setLight();
        field_0x56d = true;
    }
    field_0x568 = 0;
    field_0x569 = 0;
    return cPhs_COMPLEATE_e;
}

void daTimeFire_c::lightInit() {
    field_0x594 = field_0x570;
    field_0x594.y += 10.0f;
    mLightInfluence.mPosition = field_0x594;
    mLightInfluence.mColor.r = 0xbc;
    mLightInfluence.mColor.g = 0x66;
    mLightInfluence.mColor.b = 0x42;
    mLightInfluence.mPow = 500.0f;
    mLightInfluence.mFluctuation = 1.0f;
}

void daTimeFire_c::setLight() {
    dKy_plight_set(&mLightInfluence);
}

void daTimeFire_c::cutLight() {
    dKy_plight_cut(&mLightInfluence);
}

int daTimeFire_c::Execute() {
    bool prevFireCheck = field_0x56c;
    field_0x56c = fireCheck(0);
    if (prevFireCheck != field_0x56c) {
        if (field_0x56c == 0) {
            cutLight();
            field_0x56d = false;
        } else {
            setLight();
            field_0x56d = true;
        }
    }
    if (field_0x56d) {
        mParticleId1 = dComIfGp_particle_set(mParticleId1, 0x3ad, &field_0x570, NULL, &mParticleScale);
        mParticleId2 = dComIfGp_particle_set(mParticleId2, 0x3af, &field_0x570, NULL, &mParticleScale);
        mParticleId3 = dComIfGp_particle_set(mParticleId3, 0x3ae, &field_0x570, NULL, &mParticleScale);
        s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &field_0x570, 0, reverb);
    }
    return 1;
}

bool daTimeFire_c::fireCheck(u8 param_1) {
    bool inTime = false;
    BOOL isSw;
    if (getSw() == 0xff) {
        isSw = 1;
    } else {
        isSw = fopAcM_isSwitch(this, getSw()) & 0xff;
    }
    if (mStartTime > mEndTime) {
        if (dKy_getdaytime_hour() >= mStartTime || dKy_getdaytime_hour() <= mEndTime) {
            inTime = true;
        }
    } else {
        if (dKy_getdaytime_hour() >= mStartTime && dKy_getdaytime_hour() <= mEndTime) {
            inTime = true;
        }
    }
    if (inTime && (isSw & 0xff) && dKy_getdaytime_hour() == mStartTime && dKy_getdaytime_minute() == 0 &&
        field_0x569 == 0 && param_1 == 0)
    {
        s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &field_0x570, 0, reverb);
        field_0x569 = 1;
    }
    if (inTime && (isSw & 0xff) && dKy_getdaytime_hour() == mEndTime && dKy_getdaytime_minute() == 0 &&
        field_0x569 == 1 && param_1 == 0)
    {
        s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mDoAud_seStart(Z2SE_OBJ_FIRE_OFF, &field_0x570, 0, reverb);
        field_0x569 = 2;
    }
    if (inTime && (isSw & 0xff)) {
        return 1;
    }
    field_0x569 = 0;
    return 0;
}

int daTimeFire_c::Draw() {
    return 1;
}

int daTimeFire_c::Delete() {
    dKy_plight_cut(&mLightInfluence);
    return 1;
}

static int daTimeFire_Draw(daTimeFire_c* i_this) {
    return i_this->Draw();
}

static int daTimeFire_Execute(daTimeFire_c* i_this) {
    return i_this->Execute();
}

static int daTimeFire_Delete(daTimeFire_c* i_this) {
    return i_this->Delete();
}

static int daTimeFire_Create(fopAc_ac_c* i_this) {
    return static_cast<daTimeFire_c*>(i_this)->create();
}

static daTimeFire_HIO_c l_HIO;

static actor_method_class l_daTimeFire_Method = {
    (process_method_func)daTimeFire_Create,
    (process_method_func)daTimeFire_Delete,
    (process_method_func)daTimeFire_Execute,
    NULL,
    (process_method_func)daTimeFire_Draw,
};

extern actor_process_profile_definition g_profile_Obj_TimeFire = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_TimeFire,      // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daTimeFire_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  661,                    // mPriority
  &l_daTimeFire_Method,   // sub_method
  0x00060000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
