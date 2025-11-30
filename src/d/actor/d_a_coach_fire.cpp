/**
* @file d_a_coach_fire.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_coach_fire.h"

CoachFireAttr const daCoachFire_c::M_attr = {
    50.0f,  // radius
    4.0f,   // field_0x04
    36,     // field_0x08
    255,    // colorR
    100,    // colorG
    0,      // colorB
    700.0f, // pow
};

void daCoachFire_c::create_init() {
    fopAcM_setStageLayer(this);
    fopAcM_setCullSizeBox(this, -100.0, -10.0, -100.0, 100.0, 200.0, 100.0);
    attention_info.flags = fopAc_AttnFlag_CHECK_e | fopAc_AttnFlag_LOCK_e;
    attention_info.distances[fopAc_attn_LOCK_e] = 0x24;
    attention_info.distances[fopAc_attn_CHECK_e] = 0x22;
    mLightInfluence.mPosition = current.pos;
    mLightInfluence.mColor.r = attr().colorR;
    mLightInfluence.mColor.g = attr().colorG;
    mLightInfluence.mColor.b = attr().colorB;
    mLightInfluence.mPow = attr().pow;
    dKy_plight_set(&mLightInfluence);
    mpMtx = NULL;
    initBaseMtx();
    initCcSphere();
}

void daCoachFire_c::initBaseMtx() {
    daNpcCoach_c* coach = (daNpcCoach_c*)fpcM_SearchByID(parentActorID);
    if (coach != NULL) {
        mPos = current.pos;
        mpMtx = coach->getCoachMtx();
    }
    setBaseMtx();
}

void daCoachFire_c::setBaseMtx() {
    if (mpMtx != NULL) {
        mDoMtx_stack_c::copy(*mpMtx);
        mDoMtx_stack_c::multVec(&mPos, &current.pos);
        attention_info.position = current.pos;
        eyePos = current.pos;
    }
}


void daCoachFire_c::initCcSphere() {
    static const dCcD_SrcSph ccSphSrc = {
        {
            {0, {{AT_TYPE_0, 0, 0}, {0x00010000, 0x11}, 0}},  // mObj
            {dCcD_SE_NONE, 0, 0, 0, 0},                       // mGObjAt
            {dCcD_SE_NONE, 0, 0, 0, 0x4},                     // mGObjTg
            {0},                                              // mGObjCo
        },                                                    // mObjInf
        {{{0.0f, 0.0f, 0.0f}, 50.0f}}                         // mSph
    };  // mSphAttr

    mStts.Init(0xFF, 0xFF, this);
    mSph.Set(ccSphSrc);
    mSph.SetStts(&mStts);
    mSph.SetC(current.pos);
    mSph.SetR(attr().radius);
}

int daCoachFire_c::draw() {
    mParticleKey = dComIfGp_particle_set(mParticleKey, 0x8113, &current.pos, NULL, NULL, NULL, 0xFF,
                                         NULL, -1, NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey);
    if (emitter != NULL) {
        speed = current.pos - old.pos;
        speed.y = 0.0f;
        speed *= 0.9f;
        emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
        emitter->setUserWork((uintptr_t)(&speed));
    }
    return 1;
}

static int daCoachFire_Draw(daCoachFire_c* i_this) {
    return i_this->draw();
}

void daCoachFire_c::setCcSphere() {
    if (noHitTimer != 0) {
        mSph.ClrTgHit();
    } else {
        mSph.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mSph);
    }
}

int daCoachFire_c::execute() {
    daNpcCoach_c* coach = (daNpcCoach_c*)fpcM_SearchByID(parentActorID);
    if (noHitTimer > 0) {
        noHitTimer = noHitTimer - 1;
    }
    if (mSph.ChkTgHit() && noHitTimer == 0) {
        if (coach != NULL) {
            coach->deleteFireArrow(fopAcM_GetID(this));
        }
        setNoHitTimer();
    }
    if (coach != NULL && coach->checkCoachBlazing()) {
        dAttention_c* attn = dComIfGp_getAttention();
        if (attn->LockonTruth()) {
            if (attn->LockonTarget(0) != this) {
                if (coach->deleteFireArrowFromList(fopAcM_GetID(this)) != NULL) {
                    fopAcM_delete(this);
                }
            }
        } else if (coach->deleteFireArrowFromList(fopAcM_GetID(this)) != NULL) {
            fopAcM_delete(this);
        }
    }
    if (mpMtx != NULL) {
        mDoMtx_stack_c::copy(*mpMtx);
        mDoMtx_stack_c::multVec(&mPos, &current.pos);
        attention_info.position = current.pos;
        eyePos = current.pos;
    }
    setCcSphere();
    mLightInfluence.mPosition = current.pos;
    return 1;
}

static int daCoachFire_Execute(daCoachFire_c* i_this) {
    return i_this->execute();
}

static bool daCoachFire_IsDelete(daCoachFire_c* i_this) {
    return true;
}

daCoachFire_c::~daCoachFire_c() {
    dKy_plight_cut(&mLightInfluence);
}

static int daCoachFire_Delete(daCoachFire_c* i_this) {
    i_this->~daCoachFire_c();
    return 1;
}

static int daCoachFire_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, daCoachFire_c);
    static_cast<daCoachFire_c*>(i_this)->create_init();
    return cPhs_COMPLEATE_e;
}

static char* stringBase_80658454 = "Coach";

static actor_method_class l_daCoachFire_Method = {
    (process_method_func)daCoachFire_Create,  (process_method_func)daCoachFire_Delete,
    (process_method_func)daCoachFire_Execute, (process_method_func)daCoachFire_IsDelete,
    (process_method_func)daCoachFire_Draw,
};

extern actor_process_profile_definition g_profile_COACH_FIRE = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_COACH_FIRE,         // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daCoachFire_c),   // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    332,                     // mPriority
    &l_daCoachFire_Method,   // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
