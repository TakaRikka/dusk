/**
 * @file d_a_obj_bkdoor.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bkdoor.h"

#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "f_op/f_op_actor_mng.h"

static void* search_door(void* i_actor, void* i_this) {
    if (i_actor != NULL && fopAcM_GetProfName(i_actor) == PROC_Obj_BkDoor &&
        fopAcM_GetParamBit(i_this, 0, 1) != fopAcM_GetParamBit(i_actor, 0, 1))
    {
        return i_actor;
    }
    return NULL;
}

void daObjBkDoor_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjBkDoor_c::setBaseMtx() {
    mDoMtx_trans(mDoMtx_stack_c::get(), current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_YrotM(mDoMtx_stack_c::get(), shape_angle.y);
    mDoMtx_ZXYrotM(mDoMtx_stack_c::get(), mRotation.x, mRotation.y, mRotation.z);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

static char* l_arcName = "A_BkDoor";

static int const l_bmd[] = {0x04, 0x05};
static int const l_dzb[] = {0x08, 0x09};

// Unused
static f32 const l_col_offsetX[] = {
    80.0f, 160.0f, 240.0f, 320.0f, 400.0f,
};

static Vec const l_roll_crash_check_areaL[] = {
    {0.0f, 0.0f, -140.0f},
    {1100.0f, 0.0f, -140.0f},  // Unused
    {1100.0f, 0.0f, 140.0f},
    {0.0f, 0.0f, 140.0f},  // Unused
};

static Vec const l_roll_crash_check_areaR[] = {
    {0.0f, 0.0f, -140.0f},  // Unused
    {-1100.0f, 0.0f, -140.0f},
    {-1100.0f, 0.0f, 140.0f},  // Unused
    {0.0f, 0.0f, 140.0f},
};

int daObjBkDoor_c::Create() {
        /* dSv_event_flag_c::F_0011 - Ordon Ranch - Fence jumping complete */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[15])) {
        shape_angle.y -= 0x8000;
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    fopAcM_setCullSizeFar(this, 2.0f);

    return TRUE;
}

int daObjBkDoor_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmd[mOrientation]);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return false;
    }
    return true;
}

int daObjBkDoor_c::create1st() {
    mOrientation = fopAcM_GetParamBit(this, 0, 1);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        step = (cPhs__Step)MoveBGCreate(l_arcName, l_dzb[mOrientation], dBgS_MoveBGProc_TypicalRotY,
                                        0xBD0, NULL);
        if (step == cPhs_ERROR_e) {
            return step;
        }
    }

    return step;
}

void daObjBkDoor_c::openCheck() {
    if (!mOpenCheckDone) {
        fopAc_ac_c* actor = fopAcM_Search((fopAcIt_JudgeFunc)search_door, this);
        if (actor == NULL || (home.angle.GetY() - actor->home.angle.GetY()) != 0) {
            mOpening = true;
        }
        mOpenCheckDone = true;
    }
}

int daObjBkDoor_c::Execute(Mtx** i_mtxP) {
    daPy_py_c* player = dComIfGp_getLinkPlayer();
                                  /* dSv_event_flag_c::F_0011 - Ordon Ranch - Fence jumping complete */
    BOOL fence_jumping_complete = dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[15]);

    mFrameCounter++;
    openCheck();

    if (player->checkFrontRollCrash() && !fence_jumping_complete) {
        cXyz check_area_posA;
        cXyz check_area_posB;

        if (mOrientation == 1) {
            check_area_posA = l_roll_crash_check_areaR[1];
            check_area_posB = l_roll_crash_check_areaR[3];

            if (mOpening) {
                check_area_posA.x = -600.0f;
            }
        } else {
            check_area_posA = l_roll_crash_check_areaL[0];
            check_area_posB = l_roll_crash_check_areaL[2];

            if (mOpening) {
                check_area_posB.x = 600.0f;
            }
        }

        if (dLib_checkActorInRectangle(player, this, &check_area_posA, &check_area_posB)) {
            if (mOrientation == 1) {
                mSwingSpeed = 700.0f;

                if (mOpening) {
                    fopAcM_seStart(this, Z2SE_OBJ_BK_DOOR_ATK_OP, 0);
                } else {
                    fopAcM_seStart(this, Z2SE_OBJ_BK_DOOR_ATK_CL, 0);
                }
            } else {
                mSwingSpeed = -700.0f;

                if (mOpening) {
                    fopAcM_seStart(this, Z2SE_OBJ_BK_DOOR_ATK_OP, 0);
                }
            }
        }
    }

    *i_mtxP = &mBgMtx;
    setBaseMtx();

    mRotation.y = mSwingSpeed * cM_ssin(mFrameCounter * 4000);
    cLib_addCalc0(&this->mSwingSpeed, 0.1f, 20.0f);

    return TRUE;
}

int daObjBkDoor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return TRUE;
}

int daObjBkDoor_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return TRUE;
}

static int daObjBkDoor_create1st(daObjBkDoor_c* i_this) {
    fopAcM_ct(i_this, daObjBkDoor_c);
    return i_this->create1st();
}

static int daObjBkDoor_MoveBGDelete(daObjBkDoor_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjBkDoor_MoveBGExecute(daObjBkDoor_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjBkDoor_MoveBGDraw(daObjBkDoor_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjBkDoor_METHODS = {
    (process_method_func)daObjBkDoor_create1st,     (process_method_func)daObjBkDoor_MoveBGDelete,
    (process_method_func)daObjBkDoor_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjBkDoor_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_BkDoor = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_BkDoor,         // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjBkDoor_c),   // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    451,                     // mPriority
    &daObjBkDoor_METHODS,    // sub_method
    0x40100,                 // mStatus
    0,                       // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
