/**
 * @file d_a_npc_lf.cpp
 * 
 * @brief NPC - Little Fish
 *
 * @details Group of several small fish spawned in a group. Used in Fishing Pond.
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_lf.h"
#include "f_op/f_op_camera_mng.h"

static f32 wp[3] = {
    0.5f,
    1.0f,
    2.5f
};

daNPC_LF_HIO_c::daNPC_LF_HIO_c() {
    field_0x04 = -1;
    field_0x08 = 1.0f;
    field_0x10 = 7.0f;
    field_0x0c = 0.3f;
    field_0x14 = 0.6f;
    field_0x18 = 250.0f;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        lf_s* _this = (lf_s*)model->getUserArea();
        PSMTXCopy(model->getAnmMtx(jnt_no),*calc_mtx);

        if (jnt_no == 1) {
            cMtx_YrotM(*calc_mtx, _this->field_0x50[1] + (s16)(_this->field_0x58 * 0.3f));
        } else if (jnt_no >= 1 && jnt_no <= 3) {
            cMtx_YrotM(*calc_mtx, (_this->field_0x50[jnt_no]) + (s16)(_this->field_0x58));
        }

        model->setAnmMtx(jnt_no,*calc_mtx);
        PSMTXCopy(*calc_mtx,J3DSys::mCurrentMtx);
    }
    

    return 1;
}

static int daNPC_LF_Draw(npc_lf_class* i_this) {
    lf_s* fish = i_this->mFish;
    s8 j = 0;

    for (int i = 0; i < i_this->mIter; i++, fish++) {
        if (fish->field_0x17 == 0) {
            if (j == 0) {
                g_env_light.settingTevStruct(0, &fish->mPos, &i_this->tevStr);
            }
            j++;

            if (j == 5) {
                j = 0;
            }

            g_env_light.setLightTevColorType_MAJI(fish->mpModel, &i_this->tevStr);
            mDoExt_modelUpdateDL(fish->mpModel);
        }
    }

    return 1;
}

static u8 data_80A6AA90;

static daNPC_LF_HIO_c l_HIO;

f32 dummyLiteral1() { return 50.0f; }
f32 dummyLiteral2() { return 0.0f; }

static void npc_lf_move(fopAc_ac_c* param_0, lf_s* pFish) {
    cXyz pos;
    s16 maxStepAngle;
    f32 maxStep;
    if ((pFish->field_0x18 & 7) == 0) {
        pFish->mDistFromPlayer = fopAcM_searchPlayerDistance(param_0);
    }

    switch (pFish->field_0x1c) {        
    case 0:
        if (pFish->field_0x38[0] == 0) {
            for (int i = 0; i < 100; i++) {
                pFish->field_0x20.x = param_0->home.pos.x + cM_rndFX(500.0f);
                pFish->field_0x20.y = param_0->home.pos.y + cM_rndFX(100.0f);
                pFish->field_0x20.z = param_0->home.pos.z + cM_rndFX(500.0f);

                pos = pFish->field_0x20 - pFish->mPos;
                pos.y = 0.0f;

                if (pos.abs() > 250.0f) {
                    pFish->field_0x1c = 1;
                    pFish->field_0x38[0] = cM_rndF(30.0f) + 30.0f;
                    break;
                }
            }
        }
        pFish->field_0x40 = 0.1f;
        maxStepAngle = 0;
        maxStep = 0.05f;
        break;
    case 1:
        if (pFish->field_0x38[0] == 0) {
            pFish->field_0x1c = 0;
            pFish->field_0x38[0] = 10.0f + cM_rndF(10.0f);
        }

        pFish->field_0x40 = l_HIO.field_0x0c;
        maxStepAngle = 0x400;
        maxStep = 0.5f;
        break;
    }

    if (pFish->field_0x38[1] != 0) {
        maxStep = 0.5f;
        pFish->field_0x40 = l_HIO.field_0x14;
        maxStepAngle = 0x600;
        
    } else {
        if (pFish->mDistFromPlayer < l_HIO.field_0x18) {
            pFish->field_0x38[1] = cM_rndF(20.0f) + 20.0f;

            if (pFish->field_0x1c == 1)
                pFish->field_0x1c = 0;
        }
    }

    pos = pFish->field_0x20 - pFish->mPos;
    s16 old_pos_y = pFish->mAngle.y;

    cLib_addCalcAngleS2(&pFish->mAngle.y, cM_atan2s(pos.x,pos.z),4,maxStepAngle);
    cLib_addCalcAngleS2(&pFish->mAngle.x, -cM_atan2s(pos.y,JMAFastSqrt((pos.x * pos.x) + (pos.z * pos.z))),4,maxStepAngle);
    
    old_pos_y -= pFish->mAngle.y;
    f32 float1 = old_pos_y * 5.0f;

    if (float1 > 4000.0f) {
        float1 = 4000.0f;
    } else {
        if (float1 < -4000.0f) {
            float1 = -4000.0f;
        }
    }

    cLib_addCalc2(&pFish->field_0x58, float1, 0.5f, 1000.0f);
    cLib_addCalc2(&pFish->field_0x44, (pFish->field_0x3c * l_HIO.field_0x10), 1.0f, maxStep);
}

static void action(fopAc_ac_c* param_0, lf_s* pFish) {
    cXyz pos;
    pFish->field_0x18++;

    for (int i  = 0; i < 2; i++) {
        if (pFish->field_0x38[i] != 0) {
            pFish->field_0x38[i]--;
        }
    }

    switch (pFish->mActionMode) {
    case npc_lf_class::ACT_MOVE:
        npc_lf_move(param_0,pFish);
    }

    cMtx_YrotS(*calc_mtx,pFish->mAngle.y);
    cMtx_XrotM(*calc_mtx,pFish->mAngle.x);

    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = pFish->field_0x44 * l_HIO.field_0x08;

    MtxPosition(&pos,&pos);

    pFish->mPos += pos;

    cLib_addCalc2(&pFish->field_0x3c,pFish->field_0x40,1.0f,0.2f);
    cLib_addCalc2(&pFish->field_0x5c,(pFish->field_0x3c * 2000.0f + 2000.0f),0.5f,200.0f);

    pFish->field_0x4c[1] = pFish->field_0x3c * 13000.0f + 2000.0f;
    pFish->field_0x4c[0] += pFish->field_0x4c[1];

    for (int i = 0; i < 3; i++) {
        pFish->field_0x50[i + 1] = cM_ssin(-15000 * i + pFish->field_0x4c[0]) * pFish->field_0x5c * wp[i];
    }

    pFish->field_0x50[0] = pFish->field_0x5c * cM_ssin((pFish->field_0x4c[0] - 7000)) * -0.3f;

    mDoMtx_stack_c::transS(pFish->mPos.x,pFish->mPos.y,pFish->mPos.z);
    mDoMtx_stack_c::YrotM(pFish->mAngle.y + pFish->field_0x50[0]);
    mDoMtx_stack_c::XrotM(pFish->mAngle.x);
    f32 value = pFish->field_0x60 * l_HIO.field_0x08;
    mDoMtx_stack_c::scaleM(value,value,value);
    pFish->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daNPC_LF_Execute(npc_lf_class* i_this2) {
    npc_lf_class* i_this = (npc_lf_class*) i_this2;
    view_class* camera;
    lf_s* fish;
    cXyz pos;
    cXyz pos2;
    camera = dComIfGp_getCamera(0);
    fish = i_this->mFish;
    for (int i = 0; i < i_this->mIter; i++, fish++) {
        pos.x = fish->mPos.x - camera->lookat.eye.x;
        pos.z = fish->mPos.z - camera->lookat.eye.z;

        if (JMAFastSqrt((pos.x * pos.x) + (pos.z * pos.z)) < 1500.0f) {
            fish->field_0x17 = 0;
            action(i_this, fish);
        } else {
            fish->field_0x17 = 1;
        }
    }

    return 1;
}

static bool daNPC_LF_IsDelete(npc_lf_class* param_0) {
    return true;
}

static int daNPC_LF_Delete(npc_lf_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "NPC_LF");

    if (i_this->field_0x1fc0 != 0) {
        data_80A6AA90 = 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    u16 jnt_no;
    npc_lf_class* a_this = static_cast<npc_lf_class*>(i_this);
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("NPC_LF",3);

    for (u16 i = 0; modelData->getJointNum() > i; i++) {
        modelData->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    lf_s* fish = a_this->mFish;

    for (int i = 0; i < a_this->mIter; i++, fish++) {        
        fish->mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer,0x11000084);

        if (!fish->mpModel) {
            return 0;
        }

        fish->mpModel->setUserArea((uintptr_t)fish);
    }

    return 1;
}

static cPhs__Step daNPC_LF_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, npc_lf_class);
    npc_lf_class* a_this = static_cast<npc_lf_class*>(i_this);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&a_this->mPhase, "NPC_LF");

    if (phase == cPhs_COMPLEATE_e) {
        a_this->mParam1 = fopAcM_GetParam(a_this);

        if (a_this->mParam1 == 0xff) {
            a_this->mIter = 32;
        } else {
            a_this->mIter = a_this->mParam1;

            if (a_this->mIter > 64)
                a_this->mIter = 64;
        }

        a_this->mParam2 = fopAcM_GetParam(a_this) >> 8;

        if (a_this->mParam2 == 1) {
            a_this->home.pos.set(6457.0f, 50.0f, -5188.0f);
            a_this->current.pos = a_this->home.pos;
            a_this->current.angle = a_this->home.angle;
            a_this->current.roomNo = a_this->home.roomNo;
            a_this->current.field_0x13 = a_this->home.field_0x13;
        }

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, a_this->mIter * 0x920)) {
            return cPhs_ERROR_e;
        }

        if (data_80A6AA90 == 0) {
            a_this->field_0x1fc0 = 1;
            data_80A6AA90 = 1;
            l_HIO.field_0x04 = -1;
        }

        lf_s* fish = a_this->mFish;

        for (int i = 0; i < a_this->mIter; i++, fish++) {
            fish->mActionMode = npc_lf_class::ACT_MOVE;
            fish->mPos.x = a_this->current.pos.x + cM_rndFX(300.0f);
            fish->mPos.y = a_this->current.pos.y + cM_rndFX(50.0f);
            fish->mPos.z = a_this->current.pos.z + cM_rndFX(300.0f);
            fish->mAngle.y = cM_rndF(65536.0f);
            fish->field_0x48 = cM_rndFX(0.1f) + 0.6f;
            fish->field_0x60 = cM_rndFX(0.1f) + 0.6f;
            // fish++;
        }

        daNPC_LF_Execute(a_this);
    }

    return phase;
}

lf_s::~lf_s() {}

lf_s::lf_s() {}

daNPC_LF_HIO_c::~daNPC_LF_HIO_c() {}

static actor_method_class l_daNPC_LF_Method = {
    (process_method_func)daNPC_LF_Create,
    (process_method_func)daNPC_LF_Delete,
    (process_method_func)daNPC_LF_Execute,
    (process_method_func)daNPC_LF_IsDelete,
    (process_method_func)daNPC_LF_Draw,
};

extern actor_process_profile_definition g_profile_NPC_LF = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_NPC_LF,           // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(npc_lf_class),  // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  702,                   // mPriority
  &l_daNPC_LF_Method,    // sub_method
  0x00040000,            // mStatus
  fopAc_ENEMY_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
