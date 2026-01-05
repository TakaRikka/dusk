/**
 * d_a_obj_hbombkoya.cpp
 * Kakariko Exploding Bomb Warehouse
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_hbombkoya.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"

static char* l_arcName = "H_Bombkoy";

int daObjHBombkoya_c::create1st() {
    if (fopAcM_isSwitch(this, getSw2No())) {
        return cPhs_ERROR_e;
    }

    request_of_phase_process_class* actor_phase = (request_of_phase_process_class*)this;

    int phase = dComIfG_resLoad(actor_phase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        MTXIdentity(mBgMtx);

        phase = MoveBGCreate(l_arcName, 7, NULL, 0x2860, &mBgMtx);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    return phase;
}

int daObjHBombkoya_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    Mtx m;
    MTXIdentity(m);
    mpModel->setBaseTRMtx(m);
    return 1;
}

static dCcD_SrcCyl l_cc_cyl_src = {
    {
        {0x0, {{AT_TYPE_BOMB, 0x4, 0x13}, {0x0, 0x0}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                   // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                   // mGObjTg
        {0x0},                                                // mGObjCo
    },                                                        // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            1000.0f,             // mRadius
            1000.0f              // mHeight
        }                        // mCyl
    }                            // mCylAttr
};

int daObjHBombkoya_c::Create() {
    mpModel->setBaseScale(scale);
    fopAcM_SetMtx(this, NULL);

    cM3dGAab aab(*mpBgW->GetBnd());
    aab.GetMaxP()->y += 200.0f;
    fopAcM_setCullSizeBox(this, aab.GetMinP()->x, aab.GetMinP()->y, aab.GetMinP()->z,
                          aab.GetMaxP()->x, aab.GetMaxP()->y, aab.GetMaxP()->z);
    mTimer = 0;

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
    MTXCopy(mDoMtx_stack_c::get(), mMtx);

    for (int i = 0; i < 9; i++) {
        mEmitterKeys[i] = 0;
    }

    mActive = true;
    mStts.Init(0xFF, 0, this);
    mCyl.Set(l_cc_cyl_src);
    mCyl.SetStts(&mStts);
    return 1;
}

bool daObjHBombkoya_c::setParticle(u16* i_particleIDs, int i_particleMax,
                                   daObjHBombkoya_c::PSetTbl* i_setTbl, int i_tableMax,
                                   int i_timer) {
    bool particle_set = false;

    for (int i = 0; i < i_tableMax; i++) {
        if (i_timer == i_setTbl[i].m_startTime) {
            cXyz position;
            cXyz base_pos;

            base_pos = i_setTbl[i].m_position;
            MTXMultVec(mMtx, &base_pos, &position);

            cXyz scale;
            scale.z = i_setTbl[i].m_scale;
            scale.y = i_setTbl[i].m_scale;
            scale.x = i_setTbl[i].m_scale;

            for (int j = 0; j < i_particleMax; j++) {
                dComIfGp_particle_set(i_particleIDs[j], &position, NULL, &scale);
                particle_set = true;
            }
        }
    }

    return particle_set;
}

int daObjHBombkoya_c::Execute(Mtx** param_0) {
    *param_0 = &mBgMtx;

    if (fopAcM_isSwitch(this, getSwNo())) {
        if (fopAcM_isSwitch(this, getSw2No())) {
            mActive = false;
            fopAcM_delete(this);
        } else {
            if (mTimer == 180) {
                mActive = false;
            }

            u16 particle_ids[] = {0x858D, 0x858E, 0x858F, 0x8590, 0x8591, 0x8592, 0x8593, 0x8594};
            PSetTbl ptcl_set_table[8] = {
                {{0.0f, 235.0f, 0.0f}, 90, 0.8f},          {{-823.0f, 125.0f, -300.0f}, 110, 1.0f},
                {{-1000.0f, 1000.0f, -590.0f}, 115, 1.0f}, {{-525.0f, 863.0f, -261.0f}, 120, 0.8f},
                {{-1000.0f, 486.0f, -903.0f}, 140, 1.0f},  {{-485.0f, 12.0f, 0.0f}, 150, 0.8f},
                {{-480.0f, 614.0f, -462.0f}, 155, 1.0f},   {{-1000.0f, 116.0f, -469.0f}, 160, 1.0f},
            };

            if (setParticle(particle_ids, 8, ptcl_set_table, 8, mTimer)) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOMB_EXPLODE, &current.pos, 0, 0, 1.0f,
                                                1.0f, -1.0f, -1.0f, 0);
            }

            u16 particle_ids2[] = {0x8580, 0x8581, 0x8582};
            PSetTbl ptcl_set_table2[5] = {
                {{-165.0f, 500.0f, 106.0f}, 0, 1.0f},    {{-715.0f, 160.0f, -158.0f}, 0, 1.2f},
                {{-318.0f, 1120.0f, -514.0f}, 0, 1.1f},  {{-1000.0f, 140.0f, -792.0f}, 0, 1.2f},
                {{-921.0f, 1000.0f, -708.0f}, 0, 1.25f},
            };

            setParticle(particle_ids2, 3, ptcl_set_table2, 5, mTimer);

            static u16 id[] = {0x8577, 0x8578, 0x8579, 0x857A, 0x857B,
                               0x857C, 0x857D, 0x857E, 0x857F};

            if (mTimer < 170) {
                for (int i = 0; i < 9; i++) {
                    mEmitterKeys[i] = dComIfGp_particle_set(mEmitterKeys[i], id[i], &current.pos,
                                                            &shape_angle, NULL);
                }

                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_BOMB_HOUSE_BURN, &current.pos, 900, 0,
                                                     1.0f, 1.0f, -1.0f, -1.0f, 0);
            } else {
                fopAcM_onSwitch(this, getSw2No());

                if (mTimer == 170) {
                    Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOMB_HOUSE_EXPLD, &current.pos, 0, 0,
                                                    1.0f, 1.0f, -1.0f, -1.0f, 0);
                }

                cXyz base_pos;
                cXyz cyl_pos;

                base_pos.x = -500.0f;
                base_pos.y = 0.0f;
                base_pos.z = -500.0f;

                MTXMultVec(mMtx, &base_pos, &cyl_pos);
                mCyl.SetC(cyl_pos);
                dComIfG_Ccsp()->Set(&mCyl);
            }

            struct ptbl {
                /* 0x00 */ u16 particle_id;
                /* 0x04 */ PSetTbl set_info;
            };

            static ptbl ptable[] = {
                {0x8583, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x8584, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x8585, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x8586, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x8587, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x8588, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x8589, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x858A, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x858B, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x858C, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
                {0x8595, {{-655.0f, 0.0f, -580.0f}, 0, 1.0f}},
                {0x8595, {{-100.0f, 0.0f, -640.0f}, 0, 1.25f}},
                {0x8595, {{-325.0f, 0.0f, -760.0f}, 0, 1.8f}},
                {0x8595, {{-710.0f, 0.0f, -205.0f}, 0, 1.35f}},
                {0x8595, {{36.0f, 0.0f, -28.0f}, 0, 0.8f}},
                {0x8596, {{0.0f, 0.0f, 0.0f}, 0, 1.0f}},
            };

            for (u32 i = 0; i < 16; i++) {
                if (mTimer == ptable[i].set_info.m_startTime) {
                    cXyz pos;
                    cXyz base_pos;

                    base_pos = ptable[i].set_info.m_position;
                    MTXMultVec(mMtx, &base_pos, &pos);

                    cXyz scale;
                    scale.z = ptable[i].set_info.m_scale;
                    scale.y = ptable[i].set_info.m_scale;
                    scale.x = ptable[i].set_info.m_scale;

                    dComIfGp_particle_set(ptable[i].particle_id, &pos, &current.angle, &scale);
                }
            }

            mTimer++;
        }
    }

    return 1;
}

int daObjHBombkoya_c::Draw() {
    if (mActive && !fopAcM_isSwitch(this, getSw2No())) {
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
    }

    return 1;
}

int daObjHBombkoya_c::Delete() {
    request_of_phase_process_class* actor_phase = (request_of_phase_process_class*)this;
    dComIfG_resDelete(actor_phase, l_arcName);
    return 1;
}

static int daObjHBombkoya_create1st(daObjHBombkoya_c* i_this) {
    fopAcM_ct(i_this, daObjHBombkoya_c);

    return i_this->create1st();
}

static int daObjHBombkoya_MoveBGDelete(daObjHBombkoya_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjHBombkoya_MoveBGExecute(daObjHBombkoya_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjHBombkoya_MoveBGDraw(daObjHBombkoya_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjHBombkoya_METHODS = {
    (process_method_func)daObjHBombkoya_create1st,
    (process_method_func)daObjHBombkoya_MoveBGDelete,
    (process_method_func)daObjHBombkoya_MoveBGExecute,
    NULL,
    (process_method_func)daObjHBombkoya_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_HBombkoya = {
  fpcLy_CURRENT_e,         // mLayerID
  3,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_HBombkoya,      // mProcName
  &g_fpcLf_Method.base,   // sub_method
  0x000007C0,              // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  675,                     // mPriority
  &daObjHBombkoya_METHODS, // sub_method
  0x00040100,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
