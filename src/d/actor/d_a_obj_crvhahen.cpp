/**
* @file d_a_obj_crvhahen.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_crvhahen.h"
#include "d/actor/d_a_player.h"

static const char* l_arcName = "CrvFence";

inline int daObjCRVHAHEN_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "CaravanPiece.bmd");

    for (int i = 0; i < 10; i++) {
        mpModel[i] = mDoExt_J3DModel__create(model_data, 0x20000, 0x11000084);
        if (mpModel[i] == NULL) {
            return 0;
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daObjCRVHAHEN_c*)i_this)->CreateHeap();
}

static int daObjCRVHAHEN_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjCRVHAHEN_c*>(i_this)->create();
}

static int daObjCRVHAHEN_Delete(daObjCRVHAHEN_c* i_this) {
    i_this->Delete();
    return 1;
}

int daObjCRVHAHEN_c::HahenSet(cXyz i_param_1, cXyz i_param_2, cXyz i_param_3, cXyz i_param_4,
                              f32 duration) {
    mDrawHahen = true;
    mTimer = duration;

    for (int i = 0; i < 10; i++) {
        mPos[i].set(i_param_2.x + cM_rndFX(i_param_1.x), i_param_2.y + cM_rndF(i_param_1.y),
                    i_param_2.z + cM_rndFX(i_param_1.z));
        mInitialPos[i].set(cM_rndF(1.0f) + 0.5f, cM_rndF(1.0f) + 0.5f, cM_rndF(1.0f) + 0.5f);
        mVelocity[i].set(i_param_4.x + cM_rndFX(i_param_3.x), i_param_4.y + cM_rndF(i_param_3.y),
                         i_param_4.z + cM_rndF(i_param_3.z));

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);

        mDoMtx_stack_c::multVec(&mPos[i], &mPos[i]);
        mDoMtx_stack_c::ZXYrotS(shape_angle);
        mDoMtx_stack_c::multVec(&mVelocity[i], &mVelocity[i]);

        mRotSpeed[i].x = cM_rndFX(5000.0f);
        mRotSpeed[i].y = cM_rndFX(5000.0f);
        mRotation[i].x = cM_rndFX(65536.0f);
        mRotation[i].y = cM_rndFX(65536.0f);
    }
    return 1;
}

bool daObjCRVHAHEN_c::Wall_Check(cXyz i_origin, cXyz i_target) {
    dBgS_LinChk line_check;

    cXyz linePos(i_origin.x + i_target.x, i_origin.y, i_origin.z + i_target.z);

    line_check.Set(&i_origin, &linePos, NULL);

    bool did_line_cross = dComIfG_Bgsp().LineCross(&line_check);

    if (did_line_cross == true) {
        return true;
    } else {
        return false;
    }
    return did_line_cross;
}

void daObjCRVHAHEN_c::Hahen_Hakai(int i_start, int i_end) {
    for (int i = i_start; i < i_end; ++i) {
        if (mStatus[i] > 3) {
            mPos[i].y = mGroundHeight;
            mRotation[i].x = 0x4000;
        } else {
            mVelocity[i].y += +-9.0f;
            Wall_Check(mPos[i], mVelocity[i]);

            // mPos[i].x = mPos[i].x + mVelocity[i].x;
            // mPos[i].y = mPos[i].y + mVelocity[i].y;
            // mPos[i].z = mPos[i].z + mVelocity[i].z;

            mPos[i].x += mVelocity[i].x;
            mPos[i].y += mVelocity[i].y;
            mPos[i].z += mVelocity[i].z;

            mRotation[i].x += mRotSpeed[i].x;
            mRotation[i].y += mRotSpeed[i].y;
            mRotation[i].z += mRotSpeed[i].z;

            if ((mGroundHeight + 13.0f) >= mPos[i].y) {
                ++mStatus[i];
                mPos[i].y = mGroundHeight + 13.0f;

                mVelocity[i].y = mVelocity[i].y * -0.6f;
                mVelocity[i].x = mVelocity[i].x * 0.8f;
                mVelocity[i].z = mVelocity[i].z * 0.8f;

                // mVelocity[i].y *= -0.6f;
                // mVelocity[i].x *= 0.8f;
                // mVelocity[i].z *= 0.8f;

                mRotation[i].x = 0x4000;
                mRotation[i].y = mRotation[i].y + mRotSpeed[i].y;
            }
        }
    }
}

void daObjCRVHAHEN_c::CheckCull() {
    daPy_py_c* player_actor = daPy_getPlayerActorClass();
    cXyz& player_pos = fopAcM_GetPosition((fopAc_ac_c*)player_actor);

    int culled_num = 1;
    for (int i = 0; i < 10; i++) {
        if ((mPos[i].absXZ(player_pos) > mTimer) && !checkViewArea(&mPos[i])) {
            culled_num++;
            if (culled_num == 10) {
                fopAcM_delete(this);
            }
        }
    }
}

bool daObjCRVHAHEN_c::checkViewArea(cXyz* i_this) {
    Vec proj;
    mDoLib_project(i_this, &proj);

    bool ret = false;

    if (proj.x >= 0.0f && proj.x <= FB_WIDTH && proj.y >= 0.0f && proj.y <= FB_HEIGHT) {
        ret = true;
    }

    return ret;
}

int daObjCRVHAHEN_c::Execute() {
    if (mDrawHahen) {
        daObjCRVHAHEN_c::Hahen_Hakai(0, 10);
    }

    daObjCRVHAHEN_c::setBaseMtx();
    daObjCRVHAHEN_c::CheckCull();

    return 1;
}

int daObjCRVHAHEN_c::Delete() {
    if (mInitialized) {
        J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "CaravanPiece.bmd");
        dComIfGp_removeSimpleModel(model_data, fopAcM_GetRoomNo(this));
    }

    dComIfG_resDelete(&mPhase, l_arcName);

    return 1;
}

void daObjCRVHAHEN_c::setBaseMtx() {
    for (int i = 0; i < 10; i++) {
        mDoMtx_stack_c::transS(mPos[i]);
        mDoMtx_stack_c::ZXYrotM(mRotation[i]);
        mDoMtx_stack_c::scaleM(mInitialPos[i]);
        mpModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

inline int daObjCRVHAHEN_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);

    for (int i = 0, j = i; i < 10; i = i + 1) {
        g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);
    }

    dComIfGd_setListBG();

    if (mDrawHahen) {
        for (int i = 0; i < 10; i = i + 1) {
            s8 roomno = fopAcM_GetRoomNo(this);
            dComIfGp_entrySimpleModel(mpModel[i], roomno);
        }
    }

    dComIfGd_setList();

    return 1;
}

static int daObjCRVHAHEN_Draw(daObjCRVHAHEN_c* i_this) {
    return i_this->Draw();
}

static int daObjCRVHAHEN_Execute(daObjCRVHAHEN_c* i_this) {
    return i_this->Execute();
}

int daObjCRVHAHEN_c::create() {
    fopAcM_ct(this, daObjCRVHAHEN_c);

    

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);

    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1320)) {
            return cPhs_ERROR_e;
        }
        mDrawHahen = 0;
        gravity = -9.0f;
        J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "CaravanPiece.bmd");
        dComIfGp_addSimpleModel(model_data, fopAcM_GetRoomNo(this), '\0');
        mInitialized = true;
        mGroundHeight = current.pos.y;
        fopAcM_setCullSizeBox(this, -1000.0, -500.0, -1000.0, 1000.0, 500.0, 1000.0);
        daObjCRVHAHEN_Execute(this);
    }
    return phase_state;
}

static int daObjCRVHAHEN_IsDelete(daObjCRVHAHEN_c* i_this) {
    return true;
}

static actor_method_class l_daObjCRVHAHEN_Method = {
    (process_method_func)daObjCRVHAHEN_Create,  (process_method_func)daObjCRVHAHEN_Delete,
    (process_method_func)daObjCRVHAHEN_Execute, (process_method_func)daObjCRVHAHEN_IsDelete,
    (process_method_func)daObjCRVHAHEN_Draw,
};

actor_process_profile_definition g_profile_Obj_CRVHAHEN = {
    fpcLy_CURRENT_e,          // mLayerID
    3,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_Obj_CRVHAHEN,        // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daObjCRVHAHEN_c),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    468,                      // mPriority
    &l_daObjCRVHAHEN_Method,  // sub_method
    0x00040000,               // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
