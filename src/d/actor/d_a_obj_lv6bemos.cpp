/**
 * @file d_a_obj_lv6bemos.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv6bemos.h"
#include "SSystem/SComponent/c_lib.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"

enum Obj_lv6bm_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_EF_BIMOBEAM = 0x07,

    /* BMDE */
    /* 0x0A */ BMDE_BM6 = 0xA,

    /* BMDR */
    /* 0x0D */ BMDR_EF_BIMOBEAM = 0xD,
    /* 0x0E */ BMDR_K_BIMO00,

    /* BTK */
    /* 0x11 */ BTK_EF_BIMOBEAM = 0x11,
    /* 0x12 */ BTK_EF_BIMOBEAM_OFF,
    /* 0x13 */ BTK_EF_BIMOBEAM_ON,

    /* DZB */
    /* 0x16 */ DZB_K_BIMO00 = 0x16,
    /* 0x17 */ DZB_LV6_OBJ_BEMOS,
};

const static dCcD_SrcSph l_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0xd8fbfdff, 0x11}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x8}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 50.0f} // mSph
    } // mSphAttr
};

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        daObjL6Bm_c* i_this = (daObjL6Bm_c*)model->getUserArea();

        mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));

        if (jntNo == 1) {
            mDoMtx_stack_c::XrotM(i_this->field_0x892);
        } else if (jntNo == 2) {
            mDoMtx_stack_c::push();
            mDoMtx_stack_c::multVecZero(&i_this->field_0x894);
            daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

            if (i_this->field_0x891 != 0) {
                cLib_addCalc0(&i_this->field_0x914, 0.1f, 20.0f);
            } else {
                i_this->field_0x914 = 200.0f;
            }

            f32 fVar1 = player->current.pos.absXZ(i_this->field_0x894);
            f32 yDiff = i_this->field_0x894.y - player->current.pos.y;
            if (fVar1 > i_this->field_0x914) {
                fVar1 -= i_this->field_0x914;
            }

            s16 sVar1 = cM_atan2s(yDiff, fVar1);
            if (sVar1 > 8000) {
                sVar1 = 8000;
            }

            mDoMtx_stack_c::transS(i_this->field_0x894);
            mDoMtx_stack_c::YrotM(i_this->field_0x892);
            mDoMtx_stack_c::XrotM(sVar1);
            i_this->mBeamEffectModel->setBaseTRMtx(mDoMtx_stack_c::get());
            cXyz sp40(0.0f, 0.0f, 1200.0f);
            mDoMtx_stack_c::multVec(&sp40, &i_this->field_0x72c.mEnd);
            i_this->field_0x72c.mStart = i_this->field_0x894;
            i_this->field_0x72c.mRadius = 20.0f;
            
            i_this->field_0x5e8.cM3dGCps::Set(i_this->field_0x72c);
            i_this->field_0x5e8.CalcAtVec();
            mDoMtx_stack_c::pop();
        }

        model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
        mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    }

    return 1;
}

void daObjL6Bm_c::initBaseMtx() {
    mBeamosModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjL6Bm_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mBeamosModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static char* l_arcName = "Obj_lv6bm";

static cull_box l_cull_box = {
    {-200.0f, -100.0f, -200.0f},
    {200.0f, 100.0f, 200.0f},
};

static dCcD_SrcCps l_cps_src = {
    {
        {0x0, {{0x100, 0x1, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_SWORD, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 50.0f}, // mCps
    } // mCpsAttr
};

int daObjL6Bm_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mBeamosModel->getBaseTRMtx());
    mStts.Init(0xFF, 0xFF, this);
    field_0x5e8.Set(l_cps_src);
    field_0x5e8.SetStts(&mStts);
    field_0x748.Set(l_sph_src);
    field_0x748.SetStts(&mStts);

    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);

    J3DJoint* neckJnt = mBeamosModel->getModelData()->getJointNodePointer(1);
    if (neckJnt != NULL) {
        neckJnt->setCallBack(nodeCallBack);
    }

    J3DJoint* eyeJnt = mBeamosModel->getModelData()->getJointNodePointer(2);
    if (eyeJnt != NULL) {
        eyeJnt->setCallBack(nodeCallBack);
    }

    mBeamosModel->setUserArea((uintptr_t)this);

    if (neckJnt == NULL) {
        OS_REPORT_ERROR("表ビーモス：首ジョイントが見つかりませんでした！\n"); // Table Beamos : Neck joint not found!
    }

    if (eyeJnt == NULL) {
        OS_REPORT_ERROR("表ビーモス：目ジョイントが見つかりませんでした！\n"); // Table Beamos: Eye joint not found!
    }

    setAction(0);
    return 1;
}

int daObjL6Bm_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMDR_K_BIMO00);
    JUT_ASSERT(620, modelData != NULL);
    mBeamosModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mBeamosModel == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMDR_EF_BIMOBEAM);
    JUT_ASSERT(634, modelData != NULL);
    mBeamEffectModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mBeamEffectModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, BTK_EF_BIMOBEAM_ON);
    JUT_ASSERT(648, pbtk != NULL);
    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL || mpBtkAnm->init(modelData, pbtk, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    #if DEBUG
    pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, BTK_EF_BIMOBEAM_OFF);
    JUT_ASSERT(660, pbtk != NULL);
    #endif

    pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, BTK_EF_BIMOBEAM);
    JUT_ASSERT(666, pbtk != NULL);
    mpBtkAnm2 = new mDoExt_btkAnm();
    if (mpBtkAnm2 == NULL || mpBtkAnm2->init(modelData, pbtk, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, BCK_EF_BIMOBEAM);
    JUT_ASSERT(678, pbck != NULL);
    mBeamBck = new mDoExt_bckAnm();
    if (mBeamBck == NULL || mBeamBck->init(pbck, 1, 2, 1.0f, 0, -1, false) == 0) {
        return 0;
    }

    if (field_0x891 != 0) {
        mpBtkAnm->setFrame(mpBtkAnm->getEndFrame());
        mpBtkAnm2->setPlaySpeed(1.0f);
        field_0x8b2 = 0x1000;
        field_0x8b4 = 4;
        field_0x890 = 5;
    }

    return 1;
}

cPhs__Step daObjL6Bm_c::create1st() {
    if (fopAcM_isSwitch(this, getSwBit())) {
        OS_REPORT("ビーモス破壊済み！<%d>\n", getSwBit() & 0xFF); // Beamos has been destroyed! <%d>\n
        return cPhs_ERROR_e;
    }

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = (cPhs__Step)MoveBGCreate(l_arcName, DZB_K_BIMO00, NULL, 0x4000, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    return phase;
}

int daObjL6Bm_c::Execute(f32 (**param_1)[3][4]) {
    static u16 const l_particle_id[6] = {
        0x828F, 0x8290, 0x8291, 0x8292, 0x8293, 0x8294,
    };

    if (field_0x891 != 0) {
        mBeamBck->play();
        dComIfG_Ccsp()->Set(&field_0x5e8);
    }

    if (field_0x748.ChkTgHit() && field_0x748.GetTgHitObj() != NULL) {
        for (int i = 0; i < 5; i++) {
            dComIfGp_particle_set(l_particle_id[i], &current.pos, NULL, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        }

        fopAcM_delete(this);
        fopAcM_onSwitch(this, getSwBit());
    }

    field_0x748.SetC(field_0x894);
    dComIfG_Ccsp()->Set(&field_0x748);
    action();
    *param_1 = &mBgMtx;
    setBaseMtx();
    return 1;
}

void daObjL6Bm_c::action() {
    static daObjL6Bm_c::actionFunc l_func[3] = {
        &daObjL6Bm_c::actionWait,
        &daObjL6Bm_c::actionFindPlayer,
        &daObjL6Bm_c::actionAttack,
    };

    (this->*l_func[mAction])();
}

void daObjL6Bm_c::actionWait() {
    bool var_r28 = false;
    bool var_r27 = false;
    f32 fVar1;
    f32 playerDistance = fopAcM_searchPlayerDistanceXZ(this);

    if (getArg0() != 0xFF) {
        fVar1 = getArg0() * 50.0f;
    } else {
        fVar1 = 1000.0f;
    }

    if (playerDistance < fVar1) {
        var_r28 = true;
    }

    s16 targetAngle = cLib_targetAngleY(fopAcM_GetPosition_p(this), fopAcM_GetPosition_p(dComIfGp_getPlayer(0)));
    s16 angleDiff = (s16)abs((s16)(targetAngle - current.angle.y));
    s16 unkBound = 0x2000;
    if (angleDiff < unkBound / 2) {
        var_r27 = true;
    }

    if (var_r28 && var_r27) {
        setAction(1);
        field_0x911 = 20;
    } else {
        s16 sVar1;
        if (getArg1() != 0xFF) {
            sVar1 = (u16)getArg1() * 100;
        } else {
            sVar1 = 0x4B0;
        }
        current.angle.y += sVar1;
    }

    field_0x892 = current.angle.y;
    if (mpBtkAnm->play() != 0) {
        mpBtkAnm->setPlaySpeed(0.0f);
    }
}

void daObjL6Bm_c::actionFindPlayer() {
    f32 fVar1;
    bool var_r28 = false;
    f32 playerDistance = fopAcM_searchPlayerDistanceXZ(this);

    if (getArg0() != 0xFF) {
        fVar1 = getArg0() * 50.0f;
    } else {
        fVar1 = 1000.0f;
    }

    if (playerDistance < fVar1) {
        var_r28 = true;
    }

    if (!var_r28) {
        OS_REPORT("プレイヤー離れました！\n"); // Player has left!\n
        setAction(0);

        if (field_0x891 != 0) {
            mpBtkAnm->init(mBeamEffectModel->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, BTK_EF_BIMOBEAM_OFF),
                           1, 0, 1.0f, 0, -1);
            field_0x891 = 0;
        }
    } else {
        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 10, 0x400);
        if (cLib_calcTimer(&field_0x911) == 0) {
            if (field_0x891 == 0) {
                mpBtkAnm->init(mBeamEffectModel->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, BTK_EF_BIMOBEAM_ON),
                               1, 0, 0.0f, 0, -1);
                field_0x891 = 1;
                mpBtkAnm->setPlaySpeed(1.0f);
            }

            mpBtkAnm->play();
            mpBtkAnm2->play();
        }
    }

    field_0x892 = current.angle.y;
}

void daObjL6Bm_c::actionAttack() {
    /* empty function */
}

int daObjL6Bm_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mBeamosModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mBeamosModel);
    dComIfGd_setList();

    if (field_0x891 != 0) {
        g_env_light.setLightTevColorType_MAJI(mBeamEffectModel, &tevStr);
        mpBtkAnm->entry(mBeamEffectModel->getModelData());
        mpBtkAnm2->entry(mBeamEffectModel->getModelData());
        mBeamBck->entry(mBeamEffectModel->getModelData());
        mDoExt_modelUpdateDL(mBeamEffectModel);
        mBeamBck->remove(mBeamEffectModel->getModelData());
        mpBtkAnm->remove(mBeamEffectModel->getModelData());
        mpBtkAnm2->remove(mBeamEffectModel->getModelData());
    }

    if (field_0x90c != -G_CM3D_F_INF) {
        cM3dGPla plane;
        if (dComIfG_Bgsp().GetTriPla(mObjGndChk, &plane)) {
            dComIfGd_setSimpleShadow(&current.pos, field_0x90c, 120.0f, &plane.mNormal, 0, 
                                     1.0f, dDlst_shadowControl_c::getSimpleTex());
        }
    }

    return 1;
}

int daObjL6Bm_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjL6Bm_create1st(daObjL6Bm_c* i_this) {
    fopAcM_ct(i_this, daObjL6Bm_c);
    return i_this->create1st();
}

static int daObjL6Bm_MoveBGDelete(daObjL6Bm_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjL6Bm_MoveBGExecute(daObjL6Bm_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjL6Bm_MoveBGDraw(daObjL6Bm_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjL6Bm_METHODS = {
    (process_method_func)daObjL6Bm_create1st,
    (process_method_func)daObjL6Bm_MoveBGDelete,
    (process_method_func)daObjL6Bm_MoveBGExecute,
    0,
    (process_method_func)daObjL6Bm_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Lv6bemos = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv6bemos,      // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjL6Bm_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  630,                    // mPriority
  &daObjL6Bm_METHODS,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
