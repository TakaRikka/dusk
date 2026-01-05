//
// d_a_npc_tr.cpp
// NPC - Trout
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_tr.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_mtx.h"

daNPC_TR_HIO_c::daNPC_TR_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 0.8f;
    field_0xc = 15.0f;
    field_0x10 = 0.3f;
    field_0x14 = 0.6f;
    field_0x18 = 250.0f;
}

static int nodeCallBack(J3DJoint* p_joint, int param_1) {
    if (param_1 == 0) {
        int jointNo = p_joint->getJntNo();
        J3DModel* sysModel = j3dSys.getModel();
        npc_tr_class* npc_tr = (npc_tr_class*)sysModel->getUserArea();

        mDoMtx_copy(sysModel->getAnmMtx(jointNo), *calc_mtx);

        if (jointNo == 1) {
            mDoMtx_YrotM(*calc_mtx, npc_tr->field_0x5f2[0] + (s16)(npc_tr->field_0x5f8 * 0.3f));
        } else if (jointNo >= 1 && jointNo <= 3) {
            mDoMtx_YrotM(*calc_mtx, npc_tr->field_0x5f2[jointNo - 1] + (s16)(npc_tr->field_0x5f8));
        }
        sysModel->setAnmMtx(jointNo, *calc_mtx);
        mDoMtx_copy(*calc_mtx, j3dSys.mCurrentMtx);
    }

    return 1;
}

static int daNPC_TR_Draw(npc_tr_class* npc_tr) {
    g_env_light.settingTevStruct(0, &npc_tr->current.pos, &npc_tr->tevStr);
    g_env_light.setLightTevColorType_MAJI(npc_tr->field_0x5b8, &npc_tr->tevStr);
    mDoExt_modelUpdateDL(npc_tr->field_0x5b8);
    return 1;
}

// placeholder for daNPC_TR_HIO_c::genMessage to generate literals in the right place
static f32 dummyLiterals() {
    f32 dummy = 50.0f;
    dummy *= 0.0f;
    dummy *= 1000.0f;
    dummy *= 200.0f;
    return dummy;
}

static u8 hio_set;

static daNPC_TR_HIO_c l_HIO;

static void npc_tr_move(npc_tr_class* npc_tr) {
    cXyz distance;
    f32 var_f31;
    s16 var_r29;

    switch (npc_tr->field_0x5c0) {
    case 0:
        if (npc_tr->field_0x5dc[0] == 0) {
            for (int i = 0; i < 100; i++) {
                npc_tr->field_0x5c4.x = npc_tr->home.pos.x + cM_rndFX(1000.0f);
                npc_tr->field_0x5c4.y = npc_tr->home.pos.y + cM_rndFX(200.0f);
                npc_tr->field_0x5c4.z = npc_tr->home.pos.z + cM_rndFX(1000.0f);

                distance = npc_tr->field_0x5c4 - npc_tr->current.pos;
                distance.y = 0.0f;

                if (distance.abs() > 500.0f) {
                    npc_tr->field_0x5c0 = 1;
                    npc_tr->field_0x5dc[0] = (s16)(cM_rndF(30.0f) + 30.0f);
                    break;
                }
            }
        }
        npc_tr->field_0x5e8 = 0.1f;
        var_r29 = 0;
        var_f31 = 0.05f;
        break;
    case 1:
        if (npc_tr->field_0x5dc[0] == 0) {
            npc_tr->field_0x5c0 = 0;
            npc_tr->field_0x5dc[0] = (s16)(cM_rndF(30.0f) + 30.0f);
        }
        npc_tr->field_0x5e8 = l_HIO.field_0x10;
        var_r29 = 0x400;
        var_f31 = 0.5f;
        break;
    }

    if (npc_tr->field_0x5dc[1] != 0) {
        var_f31 = 0.5f;
        npc_tr->field_0x5e8 = l_HIO.field_0x14;
        var_r29 = 0x600;
    } else if (npc_tr->field_0x5d8 < l_HIO.field_0x18) {
        npc_tr->field_0x5dc[1] = (s16)(cM_rndF(20.0f) + 20.0f);

        if (npc_tr->field_0x5c0 == 1) {
            npc_tr->field_0x5c0 = 0;
        }
    }

    distance = npc_tr->field_0x5c4 - npc_tr->current.pos;
    s16 angle = npc_tr->current.angle.y;
    cLib_addCalcAngleS2(&npc_tr->current.angle.y, cM_atan2s(distance.x, distance.z), 4, var_r29);

    f32 var_f2 = JMAFastSqrt((distance.x * distance.x) + (distance.z * distance.z));
    cLib_addCalcAngleS2(&npc_tr->current.angle.x, -cM_atan2s(distance.y, var_f2), 4, var_r29);

    angle -= npc_tr->current.angle.y;
    f32 var_f1_2 = angle * 5.0f;
    if (var_f1_2 > 4000.0f) {
        var_f1_2 = 4000.0f;
    } else if (var_f1_2 < -4000.0f) {
        var_f1_2 = -4000.0f;
    }

    cLib_addCalc2(&npc_tr->field_0x5f8, var_f1_2, 0.5f, 1000.0f);
    cLib_addCalc2(&npc_tr->speedF, npc_tr->field_0x5e4 * l_HIO.field_0xc, 1.0f, var_f31);
}

static void action(npc_tr_class* i_this) {
    switch (i_this->field_0x5be) {
    case 0:
        npc_tr_move(i_this);
        break;
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.GetY(), 4, 0x2000);
    cLib_addCalcAngleS2(&i_this->shape_angle.x, i_this->current.angle.GetX(), 4, 0x2000);

    mDoMtx_YrotS(*calc_mtx, i_this->current.angle.GetY());
    mDoMtx_XrotM(*calc_mtx, i_this->current.angle.GetX());

    cXyz v;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = i_this->speedF * l_HIO.field_0x8;
    MtxPosition(&v, &i_this->speed);

    i_this->current.pos += i_this->speed;

    cLib_addCalc2(&i_this->field_0x5e4, i_this->field_0x5e8, 1.0f, 0.2f);
    cLib_addCalc2(&i_this->field_0x5fc, i_this->field_0x5e4 * 2000.0f + 2000.0f, 0.5f, 200.0f);

    i_this->field_0x5ee = i_this->field_0x5e4 * 13000.0f + 2000.0f;
    i_this->field_0x5ec += i_this->field_0x5ee;

    f32 local_28[3] = {0.5f, 1.0f, 2.5f};
    for (int i = 0; i < 3; ++i) {
        i_this->field_0x5f2[i] =
            local_28[i] * (i_this->field_0x5fc * cM_ssin(i_this->field_0x5ec + (i * -15000)));
    }

    i_this->field_0x5f0 = cM_ssin(i_this->field_0x5ec + -7000) * i_this->field_0x5fc * -0.3f;
}

static int daNPC_TR_Execute(npc_tr_class* npc_tr) {
    npc_tr->field_0x5bc++;
    npc_tr->field_0x5d8 = fopAcM_searchActorDistance(npc_tr, (fopAc_ac_c*)dComIfGp_getPlayer(0));

    for (int i = 0; i < 4; i++) {
        if (npc_tr->field_0x5dc[i] != 0) {
            npc_tr->field_0x5dc[i]--;
        }
    }
    action(npc_tr);

    mDoMtx_stack_c::transS(npc_tr->current.pos.x, npc_tr->current.pos.y, npc_tr->current.pos.z);
    mDoMtx_stack_c::YrotM(npc_tr->shape_angle.y + npc_tr->field_0x5f0);
    mDoMtx_stack_c::XrotM(npc_tr->shape_angle.x);

    f32 scale = npc_tr->scale.x * l_HIO.field_0x8;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    MTXCopy(mDoMtx_stack_c::get(), npc_tr->field_0x5b8->mBaseTransformMtx);

    return 1;
}

static bool daNPC_TR_IsDelete(npc_tr_class* param_0) {
    return true;
}

static int daNPC_TR_Delete(npc_tr_class* npc_tr) {
    dComIfG_resDelete(&npc_tr->mPhaseReq, "NPC_TR");

    if (npc_tr->field_0x600) {
        hio_set = 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    npc_tr_class* npc_tr = (npc_tr_class*)actor;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("NPC_TR", 3);
    npc_tr->field_0x5b8 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (npc_tr->field_0x5b8 == NULL) {
        return 0;
    }

    npc_tr->field_0x5b8->setUserArea((uintptr_t)actor);

    for (u16 i = 0; i < npc_tr->field_0x5b8->getModelData()->getJointNum(); i++) {
        npc_tr->field_0x5b8->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    return 1;
}

static int daNPC_TR_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, npc_tr_class);
    npc_tr_class* npc_tr = (npc_tr_class*)i_this;

    int phase_state = dComIfG_resLoad(&npc_tr->mPhaseReq, "NPC_TR");
    if (phase_state == cPhs_COMPLEATE_e) {
        npc_tr->field_0x5b4 = fopAcM_GetParam(npc_tr);

        if (!fopAcM_entrySolidHeap(npc_tr, useHeapInit, 0x4B000)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            npc_tr->field_0x600 = 1;
            hio_set = 1;
            l_HIO.field_0x4 = -1;
        }

        npc_tr->cullMtx = npc_tr->field_0x5b8->getBaseTRMtx();
        npc_tr->scale.x = cM_rndFX(0.1f) + 0.8f;
        daNPC_TR_Execute(npc_tr);
    }

    return phase_state;
}

daNPC_TR_HIO_c::~daNPC_TR_HIO_c() {}

static actor_method_class l_daNPC_TR_Method = {
    (process_method_func)daNPC_TR_Create,  (process_method_func)daNPC_TR_Delete,
    (process_method_func)daNPC_TR_Execute, (process_method_func)daNPC_TR_IsDelete,
    (process_method_func)daNPC_TR_Draw,
};

actor_process_profile_definition g_profile_NPC_TR = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_NPC_TR,            // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(npc_tr_class),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    723,                    // mPriority
    &l_daNPC_TR_Method,     // sub_method
    0x40100,                // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_0_e,      // cullType
};
