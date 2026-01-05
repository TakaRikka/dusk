/**
 * d_a_obj_ystone.cpp
 * Object - Dungeon Prize
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ystone.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static char const* l_arcName[7] = {
    "N_gD_mskF",
    "N_gD_mskB",
    "N_gD_mskT",
    "MirrorB",
    "MirrorB",
    "MirrorB",
    "MirrorB",
};

static int const l_bmdIndex[7] = {4, 4, 4, 9, 9, 10, 11};

static int const l_brkIndex[3] = {7, 7, 7};

static int const l_btkIndex[7] = {14, 14, 14, 14, 14, 15, 16};

static int getNowLevel() {
    static char const* l_stageName[9] = {
        "D_MN05A",
        "D_MN04A",
        "D_MN01A",
        "D_MN10A",
        "D_MN11A",
        "D_MN06A",
        "D_MN07A",
        "D_MN01A",
        "D_MN01A",
    };

    for (int i = 0; i < 9; i++) {
        if (!strcmp(dComIfGp_getStartStageName(), l_stageName[i])) {
            return i + 1;
        }
    }
    return -1;
}

static int daObj_Ystone_Draw(obj_ystone_class* i_this) {
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    if (i_this->mLevel < 3) {
        if (!i_this->field_0x59b) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
            i_this->mpBrkAnm->entry(i_this->mpModel->getModelData());
            mDoExt_modelUpdateDL(i_this->mpModel);
        }
    } else {
        J3DModel* model = i_this->mpMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
        if (i_this->field_0x59a) {
            i_this->mpBtkAnm->entry(model->getModelData());
        }
        fopAcM_setEffectMtx(i_this, model->getModelData());
        i_this->mpMorf->entryDL();
    }
    return 1;
}

static void setModelBaseMtx(obj_ystone_class* i_this) {
    J3DModel* model;
    if (i_this->mLevel < 3) {
        model = i_this->mpModel;
    } else {
        model = i_this->mpMorf->getModel();
    }
    if (i_this->field_0x598 == 0) {
        mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
        mDoMtx_stack_c::scaleM(i_this->mScaleF, i_this->mScaleF, i_this->mScaleF);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        i_this->field_0x598 = 0;
        mDoMtx_stack_c::copy(model->getBaseTRMtx());
        mDoMtx_stack_c::multVecZero(&i_this->current.pos);
    }
}

static u16 mirror_effect_id[10] = {
    0x89A0, 0x89A1, 0x89A2, 0x89A5, 0x89A6, 0x89A7, 0x89A8, 0x89A9, 0x89AA, 0x89AB
};

static void action(obj_ystone_class* i_this) {
    if (i_this->mLevel < 3) {
        switch (i_this->mShadowMode) {
        case 0:
            i_this->shape_angle.y += 0x100;
            break;
        }

        setModelBaseMtx(i_this);
        i_this->mParticleKey =
            dComIfGp_particle_set(i_this->mParticleKey, 0x8403, &i_this->current.pos,
                                  &i_this->tevStr, &i_this->shape_angle, NULL, 0xff,
                                  NULL, -1, NULL, NULL, NULL);
        i_this->mpBrkAnm->play();
    } else {
        if (i_this->mTimer != 0) {
            i_this->mTimer--;
        }

        J3DAnmTransform* anm;
        J3DAnmTextureSRTKey* btk_anm;
        switch (i_this->mMirrorMode) {
        case 0:
            g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
            static u16 mirror_effect_id[10] = {
                0x89A0, 0x89A1, 0x89A2, 0x89A5, 0x89A6, 0x89A7, 0x89A8, 0x89A9, 0x89AA, 0x89AB
            };
            for (int i = 0; i < 10; i++) {
                dComIfGp_particle_set(mirror_effect_id[i], &i_this->current.pos, &i_this->tevStr,
                                      &i_this->shape_angle, NULL);
            }
            i_this->mMirrorMode = 1;
            i_this->mTimer = 120;
            anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[i_this->mLevel], 5);
            i_this->mpMorf->setAnm(anm, 0, 0.0f, 0.0f, 0.0f, -1.0f);
            i_this->mpMorf->setFrame(1.0f);
            break;

        case 1:
            if (i_this->mTimer == 0) {
                i_this->mMirrorMode = 2;
                i_this->mpMorf->setPlaySpeed(1.0f);
                i_this->mpMorf->setFrame(0.0f);
            }
            // no break

        case 2:
            i_this->mParticleKey =
                dComIfGp_particle_set(i_this->mParticleKey, 0x89a3, &i_this->current.pos,
                                    &i_this->tevStr, &i_this->shape_angle, NULL, 0xff,
                                    NULL, -1, NULL, NULL, NULL);
            i_this->shape_angle.y += 0x100;
            break;

        case 3:
            i_this->mMirrorMode = 4;
            anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[i_this->mLevel], 6);
            i_this->mpMorf->setAnm(anm, 0, 0.0f, 1.0f, 0.0f, -1.0f);
            i_this->field_0x59a = true;
            btk_anm = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName[i_this->mLevel],
                                                                 l_btkIndex[i_this->mLevel]);
            i_this->mpBtkAnm->init(i_this->mpMorf->getModel()->getModelData(),
                                   btk_anm, 1, 0, 1.0f, 0, -1);
            // no break

        case 4: {
            i_this->shape_angle.y += 0x100;
            mDoMtx_stack_c::copy(i_this->mpMorf->getModel()->getAnmMtx(0));
            cXyz particle_pos;
            mDoMtx_stack_c::multVecZero(&particle_pos);
            i_this->mParticleKey =
                dComIfGp_particle_set(i_this->mParticleKey, 0x89a4, &particle_pos, &i_this->tevStr,
                                      &i_this->shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
            break;
        }

        case 5:
            i_this->mMirrorMode = 6;
            anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[i_this->mLevel], 6);
            i_this->mpMorf->setAnm(anm, 0, 0.0f, 0.0f, 0.0f, -1.0f);
            break;
        
        case 6:
            break;
        }

        if (i_this->field_0x59a) {
            i_this->mpBtkAnm->play();
        }
        i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
        setModelBaseMtx(i_this);
        i_this->mpMorf->modelCalc();
    }
}

static int daObj_Ystone_Execute(obj_ystone_class* i_this) {
    if (i_this->mLevel < 3 && i_this->field_0x59b) {
        return 1;
    } else {
        action(i_this);
        i_this->eyePos = i_this->current.pos;
        i_this->attention_info.position = i_this->current.pos;
        return 1;
    }
}

static int daObj_Ystone_IsDelete(obj_ystone_class* i_this) {
    return 1;
}

static int daObj_Ystone_Delete(obj_ystone_class* i_this) {
    dComIfG_resDelete(&i_this->mPhaseReq, l_arcName[i_this->mLevel]);
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_ystone_class* _this = static_cast<obj_ystone_class*>(i_this);
    void* model_data =
        dComIfG_getObjectRes(l_arcName[_this->mLevel], l_bmdIndex[_this->mLevel]);

    if (_this->mLevel < 3) {
        _this->mpModel = mDoExt_J3DModel__create((J3DModelData*)model_data, 0x80000, 0x11000084);
        if (_this->mpModel == NULL) {
            return 0;
        }

        J3DAnmTevRegKey* brk_anm =
            (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName[_this->mLevel],
                                                   l_brkIndex[_this->mLevel]);
        _this->mpBrkAnm = new mDoExt_brkAnm();
        if (_this->mpBrkAnm == NULL ||
            !_this->mpBrkAnm->init((J3DModelData*)model_data, brk_anm, 1, 2, 1.0f, 0, -1))
        {
            return 0;
        }
    } else {
        _this->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)model_data, NULL, NULL,
            (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[_this->mLevel], 5),
            0, 1.0f, 0, -1, NULL, 0, 0x11000284);
        if (_this->mpMorf == NULL || _this->mpMorf->getModel() == NULL) {
            return 0;
        }

        _this->mpBtkAnm = new mDoExt_btkAnm();
        if (_this->mpBtkAnm == NULL) {
            return 0;
        }
        J3DAnmTextureSRTKey* btk_anm =
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName[_this->mLevel],
                                                       l_btkIndex[_this->mLevel]);
        if (!_this->mpBtkAnm->init((J3DModelData*)model_data, btk_anm, 0, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }
    return 1;
}

static cPhs__Step daObj_Ystone_Create(fopAc_ac_c* i_this) {
    obj_ystone_class* _this = static_cast<obj_ystone_class*>(i_this);
    fopAcM_ct(_this, obj_ystone_class);
    _this->mLevel = getNowLevel() - 1;
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhaseReq, l_arcName[_this->mLevel]);

    if (step == cPhs_COMPLEATE_e) {
        if (_this->mLevel < 3) {
            if (!fopAcM_entrySolidHeap(_this, useHeapInit, 0x1000)) {
                return cPhs_ERROR_e;
            }
        } else {
            if (!fopAcM_entrySolidHeap(_this, useHeapInit, 0x33a0)) {
                return cPhs_ERROR_e;
            }
        }

        if (_this->mLevel < 3) {
            fopAcM_SetMtx(_this, _this->mpModel->getBaseTRMtx());
            if (_this->mLevel < 3) {
                _this->field_0x59b = true;
            }
            mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
            _this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        } else {
            fopAcM_SetMtx(_this, _this->mpMorf->getModel()->getBaseTRMtx());
            _this->mMirrorMode = fopAcM_GetParam(_this) & 0xff;
            if (_this->mMirrorMode >= 7) {
                _this->mMirrorMode = 0;
            }
        }

        _this->mScaleF = 1.0f;
        daObj_Ystone_Execute(_this);
    }
    return step;
}


static actor_method_class l_daObj_Ystone_Method = {
    (process_method_func)daObj_Ystone_Create,
    (process_method_func)daObj_Ystone_Delete,
    (process_method_func)daObj_Ystone_Execute,
    (process_method_func)daObj_Ystone_IsDelete,
    (process_method_func)daObj_Ystone_Draw,
};

actor_process_profile_definition g_profile_OBJ_YSTONE = {
    fpcLy_CURRENT_e,
    8,
    fpcPi_CURRENT_e,
    PROC_OBJ_YSTONE,
    &g_fpcLf_Method.base,
    sizeof(obj_ystone_class),
    0,
    0,
    &g_fopAc_Method.base,
    0x259,
    &l_daObj_Ystone_Method,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
