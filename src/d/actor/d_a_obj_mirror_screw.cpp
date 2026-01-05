/**
 * d_a_obj_mirror_screw.cpp
 * Mirror Chamber Statue
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mirror_screw.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"

static char const* l_arcName = "MR-Screw";

daObjMirrorScrew_c::attr const daObjMirrorScrew_c::M_attr = {
    -2710.0f,  // mDownDist
    0.6f,      // mRotSpeedThreshold
    45,        // mSpinCount
    0,         // mResetSpinCount
    5.0f,      // mSpedStep
    -3.375f,   // mDownSpeed
};

daObjMirrorScrew_actionFunc daObjMirrorScrew_c::ActionTable[2][2] = {
    {
        &daObjMirrorScrew_c::initWait,
        &daObjMirrorScrew_c::executeWait,
    },
    {
        &daObjMirrorScrew_c::initDown,
        &daObjMirrorScrew_c::executeDown,
    },
};

void daObjMirrorScrew_c::initWait() {
    mSpinCount = 0;
}

void daObjMirrorScrew_c::executeWait() {
    if (mpSwSpinner != NULL) {
        if (mpSwSpinner->GetRotSpeedY() / (f32)0x1000 > M_attr.mRotSpeedThreshold) {
            mSpinCount++;
            dComIfGp_getVibration().StartQuake(4, 0xf, cXyz(0.0f, 1.0f, 0.0f));

            if (mSpinCount > M_attr.mSpinCount) {
                setAction(ACT_DOWN);
            }
        } else {
            mSpinCount = 0;
            dComIfGp_getVibration().StopQuake(0x1f);
        }
    }
}

void daObjMirrorScrew_c::initDown() {
    speedF = 0.0f;
    mSpinCount = M_attr.mResetSpinCount;

    /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[361]);

    dComIfGp_getVibration().StartShock(8, 0xf, cXyz(0.0f, 1.0f, 0.0f));
    dComIfGp_getVibration().StartQuake(6, 0xf, cXyz(0.0f, 1.0f, 0.0f));
    Z2GetAudioMgr()->seStart(Z2SE_OBJ_MR_SCRW_ON, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

    fopAcM_orderMapToolEvent(this, getEventID(), 0xFF, 0xFFFF, 1, 0);

    cXyz pos(0.0f, 0.0f, 0.0f);
    csXyz angle(0, 0, 0);
    dComIfGp_particle_set(0x8ACA, &pos, &angle, NULL);
    dComIfGp_particle_set(0x8ACB, &pos, &angle, NULL);
    dComIfGp_particle_set(0x8ACE, &pos, &angle, NULL);
    dComIfGp_particle_set(0x8ACF, &pos, &angle, NULL);
    dComIfGp_particle_set(0x8AD0, &pos, &angle, NULL);
    dComIfGp_particle_set(0x8AD1, &pos, &angle, NULL);
}

void daObjMirrorScrew_c::executeDown() {
    if (dComIfGp_event_runCheck()) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        player->changeOriginalDemo();
        player->changeDemoMode(14, 1, 0, 0);
    }

    if (mSpinCount >= 0) {
        mSpinCount--;
        if (mSpinCount <= 0) {
            if (mpChain != NULL) {
                mpChain->setAnmSpeed(1.0f);
            }

            if (mpTable != NULL) {
                mpTable->setAnmSpeed(1.0f);
            }

            if (mp6Pole != NULL) {
                mp6Pole->setAnmSpeed(1.0f);
            }

            if (mpSand != NULL) {
                mpSand->setAnmSpeed(1.0f);
            }
        }
    }

    cLib_chaseF(&speedF, M_attr.mDownSpeed, M_attr.mSpeedStep);
    current.pos.y += speedF;
    mpSwSpinner->current.pos = cXyz(current.pos.x, current.pos.y + 2636.121f, current.pos.z);
    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_MR_SCRW_MV, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                  0);

    if (current.pos.y <= home.pos.y + M_attr.mDownDist) {
        current.pos.y = home.pos.y + M_attr.mDownDist;

        dComIfGp_getVibration().StartShock(8, 0xf, cXyz(0.0f, 1.0f, 0.0f));
        dComIfGp_getVibration().StopQuake(0x1f);
        Z2GetAudioMgr()->seStart(Z2SE_OBJ_MR_SCRW_OFF, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                 0);

        u8 sw = getSwitchNo();
        if (sw != 0xFF) {
            fopAcM_onSwitch(this, sw);
        }

        fopAcM_delete(this);
    }
}

static int daObjMirrorScrew_Draw(daObjMirrorScrew_c* i_this) {
    return i_this->Draw();
}

static int daObjMirrorScrew_Execute(daObjMirrorScrew_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjMirrorScrew_IsDelete(daObjMirrorScrew_c* i_this) {
    return 1;
}

static int daObjMirrorScrew_Delete(daObjMirrorScrew_c* i_this) {
    i_this->MoveBGDelete();
    return 1;
}

cPhs__Step daObjMirrorScrew_c::create() {
    fopAcM_ct(this, daObjMirrorScrew_c);
    if (fopAcM_isSwitch(this, getSwitchNo()) ||
        /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])
           /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
        || dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354]))
    {
        return cPhs_ERROR_e;
    }

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        step = (cPhs__Step)MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0xc760, NULL);
        if (step == cPhs_ERROR_e) {
            return step;
        }
    }

    return step;
}

static cPhs__Step daObjMirrorScrew_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjMirrorScrew_c*>(i_this)->create();
}

int daObjMirrorScrew_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    return mpModel == NULL ? 0 : 1;
}

void daObjMirrorScrew_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjMirrorScrew_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

int daObjMirrorScrew_c::Create() {
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    initBaseMtx();

    cXyz sw_pos(current.pos.x, current.pos.y + 2636.121f, current.pos.z);
    fopAcM_createChild(PROC_Obj_SwSpinner, fopAcM_GetID(this), getSwitchNo() << 8, &sw_pos,
                       fopAcM_GetRoomNo(this), NULL, NULL, -1, NULL);

    mpSwSpinner = NULL;
    mpSpPath = NULL;
    mpChain = NULL;
    mpTable = NULL;
    mp6Pole = NULL;
    mpSand = NULL;

    setAction(ACT_WAIT);
    return cPhs_COMPLEATE_e;
}

int daObjMirrorScrew_c::Execute(Mtx** i_mtxP) {
    if (mpSwSpinner == NULL) {
        mpSwSpinner = static_cast<daObjSwSpinner_c*>(fopAcM_SearchByName(PROC_Obj_SwSpinner));
    }

    if (mpSpPath == NULL) {
        mpSpPath = static_cast<daTagSppath_c*>(fopAcM_SearchByName(PROC_Tag_Sppath));
    }

    if (mpChain == NULL) {
        mpChain = static_cast<daObjMirrorChain_c*>(fopAcM_SearchByName(PROC_Obj_MirrorChain));
    }

    if (mpTable == NULL) {
        mpTable = static_cast<daObjMirrorTable_c*>(fopAcM_SearchByName(PROC_Obj_MirrorTable));
    }

    if (mp6Pole == NULL) {
        mp6Pole = static_cast<daObjMirror6Pole_c*>(fopAcM_SearchByName(PROC_Obj_Mirror6Pole));
    }

    if (mpSand == NULL) {
        mpSand = static_cast<daObjMirrorSand_c*>(fopAcM_SearchByName(PROC_Obj_MirrorSand));
    }

    callExecute();
    setBaseMtx();
    *i_mtxP = &mBgMtx;
    return 1;
}

int daObjMirrorScrew_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjMirrorScrew_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static actor_method_class l_daObjMirrorScrew_Method = {
    (process_method_func)daObjMirrorScrew_Create,  (process_method_func)daObjMirrorScrew_Delete,
    (process_method_func)daObjMirrorScrew_Execute, (process_method_func)daObjMirrorScrew_IsDelete,
    (process_method_func)daObjMirrorScrew_Draw,
};

actor_process_profile_definition g_profile_Obj_MirrorScrew = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_MirrorScrew,
    &g_fpcLf_Method.base,
    sizeof(daObjMirrorScrew_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x2A6,
    &l_daObjMirrorScrew_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
