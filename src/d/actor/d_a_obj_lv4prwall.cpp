/**
 * @file d_a_obj_lv4prwall.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4prwall.h"
#include "d/actor/d_a_obj_swturn.h"

static const int l_dzbidx[] = {
    7,
    7,
    7,
};

static const int l_bmdidx[] = {
    4,
    4,
    4,
};

static char* l_arcName[3] = {
    "PRWallR09",
    "P_Rwall",
    "PRWallR04",
};

int daObjLv4PRwall_c::create1st() {
    mType = getType();

    int phase_state = dComIfG_resLoad(this, l_arcName[mType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        field_0x60e = 0;

        if (mType == 0 && getSw2No() != 0xFF && fopAcM_isSwitch(this, getSw2No())) {
            field_0x629 = 1;
            field_0x60e = -0x3FFF;
        } else if (getSwNo() != 0xFF && fopAcM_isSwitch(this, getSwNo())) {
            field_0x60e = 0x3FFF;
        }

        setMtx();

        phase_state = MoveBGCreate(l_arcName[mType], l_dzbidx[mType], dBgS_MoveBGProc_TypicalRotY, 0x8440, &mBgMtx2);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

static const Vec l_cull_box[] = { // cull_box struct not used due to mult later
    {-1600.0f, -100.0f, -1600.0f},
    {1600.0f, 1437.7f, 1600.0f},
    {-1100.04, -60.0f, -1100.5f},
    {1100.0f, 1400.0f, 1100.5f},
    {-1100.23f, -200.2f, -1100.23f},
    {1100.23f, 1000.7f, 1100.23f},
};

void daObjLv4PRwall_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x620, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y + field_0x60e);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx2);

    if (field_0x60c != 0) {
        s16 var_r29 = field_0x60c;
        if (var_r29 < 0) {
            var_r29 *= -1;
        }
        if (var_r29 > 64) {
            var_r29 = 64;
        }

        f32 temp_f31 = (f32)var_r29 / (f32)64;
        mDoMtx_stack_c::transM(cM_rndFX(2.0f * temp_f31), cM_rndFX(2.0f * temp_f31), cM_rndFX(2.0f * temp_f31));
    }

    MTXCopy(mDoMtx_stack_c::get(), mModelMtx);
}

int daObjLv4PRwall_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mType], l_bmdidx[mType]);
    JUT_ASSERT(226, model_data != NULL);

    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObjLv4PRwall_c::Create() {
    fopAcM_SetMtx(this, mModelMtx);
    mpModel->setBaseTRMtx(mModelMtx);

    int cull_idx = mType * 2;
    fopAcM_setCullSizeBox(this, l_cull_box[cull_idx].x, l_cull_box[cull_idx].y, l_cull_box[cull_idx].z,
                          l_cull_box[cull_idx + 1].x, l_cull_box[cull_idx + 1].y, l_cull_box[cull_idx + 1].z);

    field_0x60c = 0;
    field_0x610 = 0;
    field_0x618 = 1;
    field_0x628 = 0;
    field_0x624 = 0;
    field_0x619 = 0;
    field_0x61c = 0.0f;
    field_0x620 = 0.0f;
    field_0x629 = 0;

    setMtx();
    mpModel->setBaseTRMtx(mModelMtx);
    return 1;
}

static void* searchObjSwTurn(void* i_actor, void* i_data) {
    daObjSwTurn_c* swturn = (daObjSwTurn_c*)i_actor;
    daObjLv4PRwall_c* prwall = (daObjLv4PRwall_c*)i_data;
    if (swturn != NULL && fopAcM_IsActor(swturn) && fopAcM_GetProfName(swturn) == PROC_Obj_SwTurn) {
        if (swturn->getSwNo() == prwall->getSwNo()) {
            return i_actor;
        }
    }

    return NULL;
}

int daObjLv4PRwall_c::Execute(Mtx** param_0) {
    daObjSwTurn_c* swturn = (daObjSwTurn_c*)fopAcM_Search(searchObjSwTurn, this);
    if (swturn != NULL) {
        field_0x60e = swturn->getRotateAngle();
        if (field_0x60e != field_0x610) {
            field_0x60c = 1;
            field_0x618 = 0;
        } else {
            if (field_0x618 == 0) {
                field_0x618 = 1;
                field_0x61c = -2.0f;
                field_0x619 = 1;
            }
            field_0x60c = 0;
        }
        field_0x610 = field_0x60e;
    } else if (field_0x629 == 0) {
        if (fopAcM_isSwitch(this, getSwNo()) || (getSw3No() != 0xFF && fopAcM_isSwitch(this, getSw3No()))) {
            if (field_0x60e > -0x3FFF) {
                field_0x60e += field_0x60c;
                if (field_0x60c <= -64) {
                    field_0x60c = -64;
                } else {
                    field_0x60c -= 1;
                }
                field_0x618 = 0;
            } else {
                if (field_0x618 == 0) {
                    field_0x618 = 1;
                    dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                    field_0x61c = -2.0f;
                    field_0x619 = 1;

                    if (mType == 0) {
                        field_0x629 = 1;
                        if (getSw2No() != 0xFF) {
                            fopAcM_onSwitch(this, getSw2No());
                        }
                    }

                    if (mType == 0) {
                        Z2GetAudioMgr()->seStart(Z2SE_OBJ_STN_WL_RL_ST, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                    }
                }

                field_0x60e = -0x3FFF;
                field_0x60c = 0;
            }
        } else if (field_0x60e < 0) {
            field_0x60e += field_0x60c;
            if (field_0x60c >= 64) {
                field_0x60c = 64;
            } else {
                field_0x60c += 1;
            }
            field_0x618 = 0;
        } else {
            if (field_0x618 == 0) {
                field_0x618 = 1;
                dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                field_0x61c = -2.0f;
                field_0x619 = 1;

                if (mType == 0) {
                    Z2GetAudioMgr()->seStart(Z2SE_OBJ_STN_WL_RL_ST, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                }
            }

            field_0x60e = 0;
            field_0x60c = 0;
        }
    }

    if (fopAcM_isSwitch(this, getSwNo()) && getSw3No() != 0xFF && !fopAcM_isSwitch(this, getSw3No()) && field_0x60e < -6735) {
        fopAcM_onSwitch(this, getSw3No());
    }

    if (field_0x619 != 0) {
        field_0x620 += field_0x61c;
        field_0x61c += 0.5f;

        if (field_0x620 >= 0.0f) {
            field_0x620 = 0.0f;
            field_0x61c *= -0.2f;

            if (fabsf(field_0x61c) < 0.5f) {
                field_0x61c = 0.0f;
                field_0x619 = 0;
            }
        }
    }

    if (field_0x60c != 0) {
        int var_r28 = 20.0f * cM_ssin(field_0x60e * 2);
        if (var_r28 < 0) {
            var_r28 *= -1;
        }

        var_r28++;
        if (var_r28 < 1) {
            var_r28 = 1;
        }
        if (var_r28 > 5) {
            var_r28 = 5;
        }

        if (field_0x624 != var_r28) {
            field_0x624 = var_r28;
            dComIfGp_getVibration().StartQuake(field_0x624, 2, cXyz(0.0f, 1.0f, 0.0f));
            field_0x628 = 1;
        }

        if (mType == 0) {
            if (field_0x60c < 0) {
                int sp18 = field_0x60c * -1;
                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_STN_WL_RL, NULL, sp18, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            } else {
                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_STN_WL_RL_OP, NULL, field_0x60c, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }
    } else {
        if (field_0x628 != 0) {
            dComIfGp_getVibration().StopQuake(2);
            field_0x628 = 0;
        }
        field_0x624 = 0;
    }

    setMtx();
    mpModel->setBaseTRMtx(mModelMtx);
    *param_0 = &mBgMtx2;
    return 1;
}

int daObjLv4PRwall_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjLv4PRwall_c::Delete() {
    dComIfG_resDelete(this, l_arcName[mType]);
    return 1;
}

static int daObjLv4PRwall_create1st(daObjLv4PRwall_c* i_this) {
    fopAcM_ct(i_this, daObjLv4PRwall_c);
    return i_this->create1st();
}

static int daObjLv4PRwall_MoveBGDelete(daObjLv4PRwall_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv4PRwall_MoveBGExecute(daObjLv4PRwall_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv4PRwall_MoveBGDraw(daObjLv4PRwall_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv4PRwall_METHODS = {
    (process_method_func)daObjLv4PRwall_create1st,
    (process_method_func)daObjLv4PRwall_MoveBGDelete,
    (process_method_func)daObjLv4PRwall_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjLv4PRwall_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Lv4PRwall = {
  fpcLy_CURRENT_e,          // mLayerID
  3,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_Obj_Lv4PRwall,       // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daObjLv4PRwall_c), // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  672,                      // mPriority
  &daObjLv4PRwall_METHODS,  // sub_method
  0x00040100,               // mStatus
  fopAc_ACTOR_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
