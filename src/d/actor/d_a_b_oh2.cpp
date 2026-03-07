/**
 * d_a_b_oh2.cpp
 * Morpheel Tentacle
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_oh2.h"
#include "d/actor/d_a_b_ob.h"
#include "SSystem/SComponent/c_math.h"
#include "c/c_damagereaction.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "f_op/f_op_actor_mng.h"

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        b_oh2_class* area = (b_oh2_class*)model->getUserArea();

        if (area != NULL) {
            MtxTrans(area->field_0x660[jntNo].x, area->field_0x660[jntNo].y,
                     area->field_0x660[jntNo].z, 0);
            mDoMtx_YrotM((MtxP)calc_mtx, area->field_0x7d4[jntNo].y);
            mDoMtx_XrotM((MtxP)calc_mtx, area->field_0x7d4[jntNo].x);
            mDoMtx_YrotM((MtxP)calc_mtx, -0x4000);

            MtxScale(1.0f, area->field_0x5e8[jntNo], area->field_0x5e8[jntNo], 1);
            model->setAnmMtx(jntNo, (MtxP)calc_mtx);
        }
    }

    return 1;
}

static int daB_OH2_Draw(b_oh2_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();

    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);

    i_this->mpBtk->entry(model->getModelData());
    i_this->mpBrk->entry(model->getModelData());
    i_this->mInvisModel.entryDL(NULL);

    return 1;
}

// pointer to main morpheel boss actor
static b_ob_class* boss;

static void dmcalc(b_oh2_class* i_this) {
    cXyz sp20;
    cXyz sp14;

    i_this->field_0x660[0] = i_this->current.pos;
    cXyz* var_r30 = &i_this->field_0x660[1];
    csXyz* var_r29 = &i_this->field_0x7d4[1];

    cXyz sp8;
    mDoMtx_YrotS((MtxP)calc_mtx, i_this->shape_angle.y);
    mDoMtx_XrotM((MtxP)calc_mtx, i_this->shape_angle.x);

    sp20.x = 0.0f;
    sp20.y = 0.0f;
    if (i_this->field_0x5d2[3] != 0) {
        sp20.z = 200.0f;
    } else if (boss->mAction == OB_ACTION_FISH_VACUME) {
        sp20.z = 100.0f;
    } else {
        sp20.z = boss->field_0x5d04 * 123.0f;
    }

    MtxPosition(&sp20, &sp8);
    sp20.x = 0.0f;
    sp20.y = 0.0f;
    sp20.z = i_this->field_0x5e4;

    f32 var_f31 = 1.0f;
    f32 var_f30 = 0.0f;
    f32 var_f29 = -50000.0f;

    if (boss->speedF < var_f31) {
        var_f29 = boss->field_0x47a0 + 100.0f;
        var_f30 = -20.0f;
    }

    for (int i = 1; i < 31; i++) {
        f32 temp_f26 = (var_r30->x - var_r30[-1].x) + sp8.x * var_f31;
        f32 var_f1 = var_f30 + (var_r30->y + (sp8.y * var_f31));
        if (var_f1 < var_f29) {
            var_f1 = var_f29;
        }

        f32 temp_f25 = var_f1 - var_r30[-1].y;
        f32 temp_f24 = (var_r30->z - var_r30[-1].z) + sp8.z * var_f31;
        var_f31 *= 0.85f;

        s16 temp_r25;
        int temp_r24 = cM_atan2s(temp_f26, temp_f24);
        temp_r25 = -cM_atan2s(temp_f25, JMAFastSqrt((temp_f26 * temp_f26) + (temp_f24 * temp_f24)));

        mDoMtx_YrotS((MtxP)calc_mtx, temp_r24);
        mDoMtx_XrotM((MtxP)calc_mtx, temp_r25);
        MtxPosition(&sp20, &sp14);

        var_r30->x = var_r30[-1].x + sp14.x;
        var_r30->y = var_r30[-1].y + sp14.y;
        var_r30->z = var_r30[-1].z + sp14.z;
        var_r29[-1].x = temp_r25;
        var_r29[-1].y = temp_r24;

        var_r30++;
        var_r29++;
    }

    i_this->field_0x5e0 = 0.2f;
    i_this->field_0x5dc += 2000;

    int temp_r6 = 0;
    for (int i = 0; i < 30; i++, temp_r6 += -10000) {
        if (boss->speedF > 1.0f) {
            i_this->field_0x5e8[i] = i_this->field_0x5e0 + 1.0f +
                                     (i_this->field_0x5e0 * cM_ssin(i_this->field_0x5dc + temp_r6));
        } else {
            i_this->field_0x5e8[i] = 1.0f;
        }
    }
}

static void action(b_oh2_class* i_this) {
    cXyz local_20;
    cXyz vstack_2C;

    switch (i_this->field_0x5ce) {
    case 0:
        break;
    }

    MTXCopy(boss->mBodyParts[0].mpMorf->getModel()->getAnmMtx(i_this->field_0x5c8 + 8),
              mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&i_this->current.pos);
    MTXCopy(boss->mBodyParts[0].mpMorf->getModel()->getAnmMtx(0), mDoMtx_stack_c::get());

    local_20.set(0.0f, 0.0f, i_this->field_0x5c8 * 20.0f + 650.0f);
    mDoMtx_stack_c::multVec(&local_20, &vstack_2C);
    local_20 = i_this->current.pos - vstack_2C;

    i_this->shape_angle.y = cM_atan2s(local_20.x, local_20.z);
    i_this->shape_angle.x =
        -cM_atan2s(local_20.y, JMAFastSqrt((local_20.x * local_20.x) + (local_20.z * local_20.z)));

    dmcalc(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    i_this->mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));

    if (boss->speedF <= 1.0f) {
        i_this->mpBrk->setFrame(0.0f);
        i_this->mpBtk->setFrame(0.0f);
    } else {
        i_this->mpBtk->play();
        i_this->mpBrk->play();
    }

    i_this->mpMorf->modelCalc();
}

static int daB_OH2_Execute(b_oh2_class* i_this) {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    if (i_this->field_0x5c8 == 0) {
        boss = (b_ob_class*)fopAcM_SearchByID(i_this->parentActorID);
    }

    if (boss == NULL) {
        return 1;
    }

    i_this->field_0x5cc++;

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x5d2[i] != 0) {
            i_this->field_0x5d2[i]--;
        }
    }

    if (i_this->field_0x5da != 0) {
        i_this->field_0x5da--;
    }

    action(i_this);
    return 1;
}

static int daB_OH2_IsDelete(b_oh2_class* i_this) {
    return 1;
}

static int daB_OH2_Delete(b_oh2_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "B_oh");
    if (i_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    b_oh2_class* _this = static_cast<b_oh2_class*>(i_this);

    _this->mpMorf =
        new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("B_oh", 0x29), NULL, NULL, NULL, 2,
                             1.0f, 0, -1, &_this->mZ2Enemy, 0, 0x11000284);

    if (_this->mpMorf == NULL || _this->mpMorf->getModel() == NULL) {
        return 0;
    }

    if (!_this->mInvisModel.create(_this->mpMorf->getModel(), 1)) {
        return 0;
    }

    _this->mpMorf->getModel()->setUserArea((uintptr_t)i_this);

    for (u16 i = 0; i < _this->mpMorf->getModel()->getModelData()->getJointNum(); i++) {
        _this->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(
            nodeCallBack);
    }

    _this->mpBtk = new mDoExt_btkAnm();
    if (_this->mpBtk == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_oh", 0x36);
    if (!_this->mpBtk->init(_this->mpMorf->getModel()->getModelData(), btk, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    _this->mpBtk->setFrame(cM_rndF(39.0f));
    _this->mpBtk->setPlaySpeed(cM_rndFX(0.1f) + 1.0f);

    _this->mpBrk = new mDoExt_brkAnm();
    if (_this->mpBrk == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_oh", 0x2F);
    if (!_this->mpBrk->init(_this->mpMorf->getModel()->getModelData(), brk, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    _this->mpBrk->setFrame(cM_rndF(39.0f));
    _this->mpBrk->setPlaySpeed(cM_rndFX(0.1f) + 1.0f);

    return 1;
}

static int daB_OH2_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, b_oh2_class);
    b_oh2_class* _this = static_cast<b_oh2_class*>(i_this);

    int phase = dComIfG_resLoad(&_this->mPhase, "B_oh");
    if (phase == cPhs_COMPLEATE_e) {
        _this->field_0x5c8 = fopAcM_GetParam(_this) & 0xFF;

        if (!fopAcM_entrySolidHeap(_this, (heapCallbackFunc)useHeapInit, 0x23E0)) {
            return cPhs_ERROR_e;
        }

        _this->mZ2Enemy.init(&_this->current.pos, &_this->current.pos, 3, 1);
        _this->field_0x5cc = cM_rndF(65536.0f);
        _this->field_0x5dc = cM_rndF(65536.0f);

        for (int i = 0; i < 31; i++) {
            _this->field_0x660[i].y = -50000.0f;
        }

        _this->field_0x5d2[3] = 10;
        _this->field_0x5e4 = cM_rndFX(5.0f) + 50.0f;

        daB_OH2_Execute(_this);
    }

    return phase;
}

static actor_method_class l_daB_OH2_Method = {
    (process_method_func)daB_OH2_Create,  (process_method_func)daB_OH2_Delete,
    (process_method_func)daB_OH2_Execute, (process_method_func)daB_OH2_IsDelete,
    (process_method_func)daB_OH2_Draw,
};

actor_process_profile_definition g_profile_B_OH2 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_B_OH2,
    &g_fpcLf_Method.base,
    sizeof(b_oh2_class),
    0,
    0,
    &g_fopAc_Method.base,
    219,
    &l_daB_OH2_Method,
    0x44000,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};
