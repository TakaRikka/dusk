//
// Object wflag
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_wflag.h"
#include "d/d_com_inf_game.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_e_wb.h"
#include "d/d_s_play.h"

static int daObj_Wflag_Draw(obj_wflag_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->pillar_model, &actor->tevStr);
    mDoExt_modelUpdateDL(i_this->pillar_model);

    J3DModel* morfModel = i_this->anm_p->getModel();
    g_env_light.setLightTevColorType_MAJI(morfModel, &actor->tevStr);

    i_this->kolin_btk->entry(morfModel->getModelData());
    i_this->kolin_btp->entry(morfModel->getModelData());
    i_this->anm_p->entryDL();

    for (int i = 0; i < 1; i++) {
        wf_tail_s* tail = &i_this->tail_s[i];
        for (int j = 0; j < 19; j++) {
            g_env_light.setLightTevColorType_MAJI(tail->model[j], &actor->tevStr);
            mDoExt_modelUpdateDL(tail->model[j]);
        }
    }

    return 1;
}

static void tail_control(fopAc_ac_c* i_this, wf_tail_s* i_tail) {
    static f32 pd[19] = {
        1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
    };

    cXyz sp4C;
    cXyz sp40;
    cXyz sp34;

    int i;
    cXyz* pPos;
    f32 var_f27;
    f32 var_f28;
    f32 var_f31;
    f32 var_f30;
    f32 speed;
    csXyz* pAngle;

    pPos = &i_tail->pos[1];
    pAngle = &i_tail->rot[1];
    var_f27 = NREG_F(2) + -20.0f;

    cXyz sp28;
    cXyz sp1C;
    cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
    sp4C.x = 0.0f;
    sp4C.y = 0.0f;
    sp4C.z = -(NREG_F(3) + 20.0f + i_this->speedF * 10.0f);
    MtxPosition(&sp4C, &sp28);

    speed = i_this->speedF * (NREG_F(7) + 2.0f);
    sp4C.x = 0.0f;
    sp4C.y = 0.0f;
    sp4C.z = NREG_F(0) + 20.0f;

    for (i = 1; i < 20; i++, pPos++, pAngle++) {
        sp1C.x = sp28.x * pd[i - 1];
        sp1C.z = sp28.z * pd[i - 1];

        sp1C.x += (speed * cM_ssin(i_tail->field_0x0 * (XREG_S(6) + 5400) + i * (XREG_S(7) + -3700))) * 0.3f;
        sp1C.z += (speed * cM_ssin(i_tail->field_0x0 * (XREG_S(6) + 5700) + i * (XREG_S(7) + -4200))) * 0.3f;
        sp1C.y = speed * cM_ssin(i_tail->field_0x0 * (XREG_S(6) + 4200) + i * (XREG_S(7) + -2400));

        var_f28 = sp1C.x + (pPos->x - pPos[-1].x);
        var_f31 = sp1C.y + (var_f27 + (pPos[0].y - pPos[-1].y));
        var_f30 = sp1C.z + (pPos->z - pPos[-1].z);

        s16 xRot = -cM_atan2s(var_f31, var_f30);
        s16 yRot = (s16)cM_atan2s(var_f28, JMAFastSqrt(SQUARE(var_f31) + SQUARE(var_f30)));
        cMtx_XrotS(*calc_mtx, xRot);
        cMtx_YrotM(*calc_mtx, yRot);
        MtxPosition(&sp4C, &sp40);

        pAngle[-1].x = (s16)xRot;
        pAngle[-1].y = (s16)yRot;
        *pPos = pPos[-1] + sp40;
    }
}

static void tail_mtxset(wf_tail_s* i_tail) {
    for (int i = 0; i < 19; i++) {
        MtxTrans(i_tail->pos[i].x, i_tail->pos[i].y, i_tail->pos[i].z, 0);
        cMtx_XrotM(*calc_mtx, i_tail->rot[i].x);
        cMtx_YrotM(*calc_mtx, i_tail->rot[i].y);
        cMtx_XrotM(*calc_mtx, -0x8000);
        cMtx_ZrotM(*calc_mtx, i * 0x2734);
        i_tail->model[i]->setBaseTRMtx(*calc_mtx);
    }
}

static int daObj_Wflag_Execute(obj_wflag_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp14;

    i_this->counter++;

    for (int i = 0; i < 2; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->unk_timer != 0) {
        i_this->unk_timer--;
    }

    fopAc_ac_c* parent = fopAcM_SearchByID(actor->parentActorID);
    if (parent != NULL) {
        actor->current.angle.y = parent->shape_angle.y;
        actor->speedF = parent->speedF;
        
        J3DModel* model = ((e_wb_class*)parent)->anm_p->getModel();
        MTXCopy(model->getAnmMtx(YREG_S(3) + 16), *calc_mtx);
        s16 rotX = (actor->speedF * cM_ssin(i_this->counter * 5400)) * 5.0f;
        cMtx_XrotM(*calc_mtx, rotX);
        i_this->pillar_model->setBaseTRMtx(*calc_mtx);
    } else {
        mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
        mDoMtx_stack_c::YrotM((s16)actor->shape_angle.y);
        i_this->pillar_model->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    MTXCopy(i_this->pillar_model->getBaseTRMtx(), mDoMtx_stack_c::get());
    mDoMtx_stack_c::transM(VREG_F(0) + 133.0f, VREG_F(1) + 248.0f, VREG_F(2) + 0.0f);
    mDoMtx_stack_c::YrotM(VREG_S(0) + 0x4000);
    mDoMtx_stack_c::XrotM(VREG_S(1) + 2100);
    mDoMtx_stack_c::ZrotM((s16)VREG_S(2));
    
    J3DModel* model = i_this->anm_p->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->kolin_btk->play();
    i_this->kolin_btp->play();
    i_this->anm_p->play(NULL, 0, 0);
    i_this->anm_p->modelCalc();
    MTXCopy(i_this->pillar_model->getBaseTRMtx(), mDoMtx_stack_c::get());

    for (int i = 0; i < 1; i++) {
        wf_tail_s* tail = &i_this->tail_s[i];
        mDoMtx_stack_c::transM(VREG_F(3) + 143.0f, VREG_F(4) + 418.0f, VREG_F(5) + 0.0f);
        mDoMtx_stack_c::multVecZero(tail->pos);
        tail->field_0x0++;
        tail_control(actor, tail);
        tail_mtxset(tail);
    }

    return 1;
}

static int daObj_Wflag_IsDelete(obj_wflag_class* i_this) {
    return 1;
}

static int daObj_Wflag_Delete(obj_wflag_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fpc_ProcID id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, "Obj_wflag");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_actor) {
    obj_wflag_class* i_this = (obj_wflag_class*)i_actor;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_wflag", 10);
    JUT_ASSERT(409, modelData != NULL);

    i_this->pillar_model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->pillar_model == NULL) {
        return 0;
    } 

    i_this->anm_p = JKR_NEW mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Obj_wflag", 11), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("Obj_wflag", 6), 2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11020284);
    if (i_this->anm_p == NULL || i_this->anm_p->getModel() == NULL) {
        return 0;
    } 

    i_this->kolin_btk = JKR_NEW mDoExt_btkAnm();
    if (i_this->kolin_btk == NULL) {
        return 0;
    }

    if (i_this->kolin_btk->init(i_this->anm_p->getModel()->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Obj_wflag", 14), 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->kolin_btp = JKR_NEW mDoExt_btpAnm();
    if (i_this->kolin_btp == NULL) {
        return cPhs_ERROR_e;
    } 

    if (i_this->kolin_btp->init(i_this->anm_p->getModel()->getModelData(), (J3DAnmTexPattern*)dComIfG_getObjectRes("Obj_wflag", 18), 1, 2, 1.0f, 0, -1) == 0) {
        return cPhs_ERROR_e;
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("Obj_wflag", 9));
    JUT_ASSERT(479, modelData != NULL);

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 19; j++) {
            i_this->tail_s[i].model[j] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->tail_s[i].model[j] == NULL) {
                return 0;
            }
        }
    }

    return 1;
}

static int daObj_Wflag_Create(fopAc_ac_c* actor) {
    obj_wflag_class* i_this = (obj_wflag_class*)actor;
    fopAcM_ct(actor, obj_wflag_class);

    int rv = dComIfG_resLoad(&i_this->phase, "Obj_wflag");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_WFLAG PARAM %x\n", fopAcM_GetParam(actor));
        i_this->unk_0x82c = fopAcM_GetParam(actor);

        OS_REPORT("OBJ_WFLAG//////////////OBJ_WFLAG SET 1 !!\n");
    
        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0xd700)) {
            OS_REPORT("//////////////OBJ_WFLAG SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_WFLAG SET 2 !!\n");
        i_this->counter = cM_rndF(65536.0f);
        daObj_Wflag_Execute(i_this);
    }

    return rv;
}

static actor_method_class l_daObj_Wflag_Method = {
    (process_method_func)daObj_Wflag_Create,
    (process_method_func)daObj_Wflag_Delete,
    (process_method_func)daObj_Wflag_Execute,
    (process_method_func)daObj_Wflag_IsDelete,
    (process_method_func)daObj_Wflag_Draw,
};

actor_process_profile_definition g_profile_OBJ_WFLAG = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_OBJ_WFLAG_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(obj_wflag_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_OBJ_WFLAG_e,
    /* Actor SubMtd */ &l_daObj_Wflag_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
