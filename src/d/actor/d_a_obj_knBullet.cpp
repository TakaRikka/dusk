/**
 * @file d_a_obj_knBullet.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_knBullet.h"
#include "d/actor/d_a_npc_kn.h"

static const daObjKnBullet_Hio_Param_c l_DATA = {
    30.0f,
    20.0f,
    150
};

const static dCcD_SrcSph l_sph_src = {
    {
        {0x0, {{AT_TYPE_THROW_OBJ, 0x0, 0xd}, {0x10, 0x1f}, 0x0}}, // mObj
        {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x8}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f} // mSph
    } // mSphAttr
};

int daObjKnBullet_c::Create() {
    fopAcM_ct(this, daObjKnBullet_c);

    setBaseMtx();
    fopAcM_SetMtx(this, mMtx);
    mTimer = l_DATA.lifetime + 30;
    fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);

    col_init();
    mActionMode = 0;

    for (int i = 0; i < 3; i++) {
        mEmtIds[i] = -1;
    }

    return cPhs_COMPLEATE_e;
}

int daObjKnBullet_c::Execute() {
    if (!col_chk()) {
        return 1;
    }

    fopAcM_posMove(this, NULL);
    setBaseMtx();

    if (cLib_calcTimer<s16>(&mTimer) == 0) {
        fopAcM_delete(this);
        return 1;
    }

    for (int i = 0; i < 3; i++) {
        static const u16 l_prticles_id[] = {
            dPa_RM(ID_ZI_S_KN_MAGICBALL_A),
            dPa_RM(ID_ZI_S_KN_MAGICBALL_B),
            dPa_RM(ID_ZI_S_KN_MAGICBALL_C),
        };

        mEmtIds[i] = dComIfGp_particle_set(mEmtIds[i], l_prticles_id[i], &current.pos, &current.angle, NULL);
        JPABaseEmitter* emt = dComIfGp_particle_getEmitter(mEmtIds[i]);
        if (emt != NULL) {
            emt->setGlobalTranslation(current.pos.x, current.pos.y, current.pos.z);
        }
    }

    Z2GetAudioMgr()->seStartLevel(Z2SE_KN_BALL_LEVEL, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)), 1.0f, 1.0f, -1.0f, -1.0f, 0);
    return 1;
}

int daObjKnBullet_c::Draw() {
    return 1;
}

int daObjKnBullet_c::Delete() {
    return 1;
}

void daObjKnBullet_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::XYZrotM(shape_angle);
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
}

void daObjKnBullet_c::col_init() {
    mCcStts.Init(0xFF, 0xFF, this);
    mCcSph.Set(l_sph_src);
    mCcSph.SetStts(&mCcStts);
    mCcSph.SetC(current.pos);
    mCcSph.SetR(l_DATA.radius);
}

BOOL daObjKnBullet_c::col_chk() {
    if (mActionMode == 0) {
        return TRUE;
    }

    if (mActionMode == 1) {
        cXyz move_vec(0.0f, 0.0f, l_DATA.move_speed);
        mDoMtx_stack_c::YrotS(shape_angle.y);
        mDoMtx_stack_c::multVec(&move_vec, &speed);

        if ((mCcSph.ChkTgShieldHit() || mCcSph.ChkAtShieldHit()) && daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
            daNpc_Kn_c* parent = (daNpc_Kn_c*)fpcM_SearchByID(parentActorID);
            if (parent != NULL) {
                cXyz sp20(parent->current.pos);
                sp20.y += 150.0f;
                speed = sp20 - current.pos;
                speed.normalizeZP();
                speed *= l_DATA.move_speed;

                mCcSph.SetAtSPrm(0x13);
                mActionMode = 2;
                mTimer = l_DATA.lifetime;
            } else {
                hitPrtclSet();
                fopAcM_delete(this);
                return FALSE;
            }
        } else if (mCcSph.ChkAtShieldHit()) {
            daNpc_Kn_c* parent = (daNpc_Kn_c*)fpcM_SearchByID(parentActorID);
            if (parent != NULL) {
                parent->setTalkFlag(2);
                hitPrtclSet();
                fopAcM_delete(this);
                return FALSE;
            }
        } else if (mCcSph.ChkAtHit()) {
            daNpc_Kn_c* parent = (daNpc_Kn_c*)fpcM_SearchByID(parentActorID);
            if (parent != NULL) {
                parent->setTalkFlag(1);
                hitPrtclSet();
                fopAcM_delete(this);
                return FALSE;
            }
        }
    } else if (mActionMode == 2 && mCcSph.ChkAtHit()) {
        hitPrtclSet();
        fopAcM_delete(this);
        speed.set(0.0f, 0.0f, 0.0f);
        return FALSE;
    }

    mCcSph.SetC(current.pos);
    mCcSph.SetR(l_DATA.radius);
    dComIfG_Ccsp()->Set(&mCcSph);
    return TRUE;
}

void daObjKnBullet_c::hitPrtclSet() {
    for (int i = 0; i < 3; i++) {
        JPABaseEmitter* emt = dComIfGp_particle_getEmitter(mEmtIds[i]);
        if (emt != NULL) {
            emt->becomeInvalidEmitter();
            emt->deleteAllParticle();
        }
    }

    for (int i = 0; i < 3; i++) {
        static const u16 l_prticles_id[] = {
            dPa_RM(ID_ZI_S_KN_MBHIT_A),
            dPa_RM(ID_ZI_S_KN_MBHIT_B),
            dPa_RM(ID_ZI_S_KN_MBHIT_C),
        };

        dComIfGp_particle_set(l_prticles_id[i], &current.pos, &current.angle, NULL);
    }
}

static int daObjKnBullet_Execute(daObjKnBullet_c* i_this) {
    return i_this->Execute();
}

static int daObjKnBullet_Draw(daObjKnBullet_c* i_this) {
    return i_this->Draw();
}

static int daObjKnBullet_IsDelete(daObjKnBullet_c* i_this) {
    return 1;
}

static int daObjKnBullet_Delete(daObjKnBullet_c* i_this) {
    return i_this->Delete();
}

static int daObjKnBullet_create(fopAc_ac_c* i_this) {
    return ((daObjKnBullet_c*)i_this)->Create();
}

static actor_method_class l_daObjKnBullet_Method = {
    (process_method_func)daObjKnBullet_create,
    (process_method_func)daObjKnBullet_Delete,
    (process_method_func)daObjKnBullet_Execute,
    (process_method_func)daObjKnBullet_IsDelete,
    (process_method_func)daObjKnBullet_Draw,
};

actor_process_profile_definition g_profile_KN_BULLET = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_KN_BULLET,          // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjKnBullet_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  348,                     // mPriority
  &l_daObjKnBullet_Method, // sub_method
  0x00044100,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
