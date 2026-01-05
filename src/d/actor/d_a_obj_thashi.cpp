/**
 * @file d_a_obj_thashi.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_thashi.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"

char* l_arcName = "S_thashi";

static void dataStripping() {
    const static u32 ccCylSrc[] = {0, 0x100000, 0,   0x13, 0x100000, 0x1f, 0x75, 0,
                                   0, 0x10000,  0x2, 0,    0l,       0,    0,    0x42f00000};
    extern void stripFloat(f32);
    extern void stripint(int);
    extern int getStripInt();
    stripFloat(0.0f);
    stripFloat(200.0f);
    stripFloat(300.0f);
    stripFloat(getStripInt());
}

void daObjTHASHI_c::initCcCylinder() {
    #if !DEBUG
    const
    #endif
    static dCcD_SrcCyl ccCylSrc = {
        {
            {0x0, {{AT_TYPE_0, 0x0, 0x0}, {0x00100000, 0x1f}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x1, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                110.f + nREG_F(17), // mRadius
                130.f + nREG_F(16) // mHeight
            } // mCyl
        } // mCylAttr
    };
    unk13C8.Init(0xff, 0xff, this);
    unk1A58.Set(ccCylSrc);
    unk1A58.SetStts(&unk13C8);
    unk1A58.SetC(current.pos);
}

static void dataStripping2() {
    extern void stripFloat(f32);
    extern void stripDouble(f64);
    stripFloat(15.0f);
    stripFloat(60.0f);
    stripFloat(30.0f);
    stripFloat(150.0f);
    stripFloat(10.0f);
    stripFloat(70.0f);
    stripFloat(0.65f);
    stripFloat(0.25f);
    stripFloat(1.5f);
    stripFloat(0.6f);
    stripFloat(1.0f);
    stripFloat(8192.0f);
    stripFloat(32000.0f);
    stripFloat(15000.0f);
    stripFloat(65536.0f);
    stripFloat(-0.05f);
    stripFloat(0.2f);
    stripFloat(4.0f);
    stripFloat(0.7f);
    stripFloat(0.3f);
    stripFloat(5000.0f);
    stripFloat(5.0f);
    stripFloat(-9.0f);
    stripFloat(16384.0f);
    stripFloat(-1.0f);
    stripFloat(450.0f);

    stripDouble(0.5f);
    stripDouble(3.0f);
    stripDouble(0.0f);

    stripFloat(3.0f);
    stripFloat(2000.0f);
    stripFloat(FB_WIDTH);
    stripFloat(FB_HEIGHT);
}

static int daObjTHASHI_Create(fopAc_ac_c* param_0) {
    daObjTHASHI_c* i_this = (daObjTHASHI_c*)param_0;
    fpc_ProcID id = fopAcM_GetID(param_0);
    return i_this->create();
}

static int daObjTHASHI_Delete(daObjTHASHI_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    i_this->MoveBGDelete();
    return TRUE;
}

// stripped function to move dtor weak ordering
static void strippedFunc3(cXyz* a, csXyz* b) {
    delete[] b;
    delete[] a;
}

void daObjTHASHI_c::setBaseMtx() {
    switch (unk5A4) {
    case 0:
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mThashi00Model->setBaseTRMtx(mDoMtx_stack_c::get());
        break;
    case 1:
    case 2:
    case 7: {
        for (s16 i = 0; i < 10; i++) {
            mDoMtx_stack_c::transS(mCollectiveTrans[i]);
            mDoMtx_stack_c::ZXYrotM(mCollectiveRot[i]);
            mDoMtx_stack_c::scaleM(mCollectiveScale[i]);
            mThashi03Models[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
        f32 t = fabsf(cM_ssin(mRot.x) * (70.f + KREG_F(0)));
        mDoMtx_stack_c::transS(mTrans.x, mTrans.y + t + 10.f + KREG_F(11), mTrans.z);
        mDoMtx_stack_c::YrotM(mRot.y);
        mDoMtx_stack_c::ZrotM(mRot.z);
        mDoMtx_stack_c::XrotM(mRot.x);
        mDoMtx_stack_c::YrotM(-mRot.y);
        mDoMtx_stack_c::transM(0.f, 60.f + KREG_F(12), 0.f);
        mThashi02Model->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mThashi01Model->setBaseTRMtx(mDoMtx_stack_c::get());
        break;
    }
    case 3:
        for (s16 i = 0; i < 50; i++) {
            mDoMtx_stack_c::transS(mCollectiveTrans[i]);
            mDoMtx_stack_c::ZXYrotM(mCollectiveRot[i]);
            mDoMtx_stack_c::scaleM(mCollectiveScale[i]);
            mThashi03Models[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
        break;
    case 4:
    case 8:
        for (s16 i = 0; i < 40; i++) {
            mDoMtx_stack_c::transS(mCollectiveTrans[i]);
            mDoMtx_stack_c::ZXYrotM(mCollectiveRot[i]);
            mDoMtx_stack_c::scaleM(mCollectiveScale[i]);
            mThashi03Models[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mThashi01Model->setBaseTRMtx(mDoMtx_stack_c::get());
        return;
    }
}

static int daObjTHASHI_Draw(daObjTHASHI_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjTHASHI_Execute(daObjTHASHI_c* i_this) {
    return i_this->MoveBGExecute();
}

int daObjTHASHI_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "S_thashi00.bmd");
    JUT_ASSERT(92, modelData != NULL);
    J3DModelData* modelData1 = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "S_thashi01.bmd");
    JUT_ASSERT(95, modelData1 != NULL);
    mThashi00Model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mThashi00Model == NULL) {
        return FALSE;
    }
    mThashi01Model = mDoExt_J3DModel__create(modelData1, 0x80000, 0x11000084);
    if (mThashi01Model == NULL) {
        return FALSE;
    }
    J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "S_thashi03.bmd");
    JUT_ASSERT(113, modelData2 != NULL);
    J3DModelData* modelData3 = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "S_thashi02.bmd");
    JUT_ASSERT(116, modelData3 != NULL);
    for (int i = 0; i < 50; i++) {
        mThashi03Models[i] = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000084);
        if (mThashi03Models[i] == NULL) {
            return FALSE;
        }
    }
    mThashi02Model = mDoExt_J3DModel__create(modelData3, 0x80000, 0x11000084);
    if (mThashi02Model == NULL) {
        return FALSE;
    }
    u32 ind = dComIfG_getObjctResName2Index(l_arcName, "S_thashi01.dzb");
    unk5A0 = new dBgW();

    if (unk5A0 != NULL && !unk5A0->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, ind), 1, &mBgMtx)) {
        if (m_set_func != NULL) {
            unk5A0->SetCrrFunc(m_set_func);
        }
    } else {
        unk5A0 = NULL;
        return FALSE;
    }
    return TRUE;
}

int daObjTHASHI_c::create() {
    fopAcM_ct(this, daObjTHASHI_c);
    int phase_state = dComIfG_resLoad(&unk11A8, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        unk10A0 = -9.0f;
        gravity = -0.0f;
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "S_thashi00.dzb");

        unk5A4 = 0;
        if (dzb_id == -1) {
            // "dzb data not found!<%s>"
            OS_REPORT("dzbデータが見つかりませんでした!<%s>\n\n", l_arcName);
        }
        JUT_ASSERT(1797, dzb_id != -1);
        phase_state = MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x4000, 0);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }

        fopAcM_SetMtx(this, mThashi00Model->getBaseTRMtx());
        initCcCylinder();
        unk636 = 0;
        mTrans.set(current.pos.x, current.pos.y, current.pos.z);
        daObjTHASHI_Execute(this);
    }
    return phase_state;
}

static int daObjTHASHI_IsDelete(daObjTHASHI_c* param_0) {
    return TRUE;
}

static actor_method_class l_daObjTHASHI_Method = {
    (process_method_func)daObjTHASHI_Create,
    (process_method_func)daObjTHASHI_Delete,
    (process_method_func)daObjTHASHI_Execute,
    (process_method_func)daObjTHASHI_IsDelete,
    (process_method_func)daObjTHASHI_Draw,
};

actor_process_profile_definition g_profile_Obj_THASHI = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_THASHI,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjTHASHI_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  461,                    // mPriority
  &l_daObjTHASHI_Method,  // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

int daObjTHASHI_c::Create() {
    fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0f, 1000.0f);
    return cPhs_COMPLEATE_e;
}

int daObjTHASHI_c::Execute(Mtx** i_pMtx) {
    *i_pMtx = &mBgMtx;
    setBaseMtx();
    return TRUE;
}

// needs to be inline for jump tables to be at end of data
inline int daObjTHASHI_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    switch (unk5A4) {
    case 0:
        g_env_light.setLightTevColorType_MAJI(mThashi00Model, &tevStr);
        break;
    case 1:
    case 2:
    case 7:
        for (int i = 0; i < 10; i++) {
            g_env_light.setLightTevColorType_MAJI(mThashi03Models[i], &tevStr);
        }
        g_env_light.setLightTevColorType_MAJI(mThashi01Model, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mThashi02Model, &tevStr);
        break;
    case 3:
        for (int i = 0; i < 50; i++) {
            g_env_light.setLightTevColorType_MAJI(mThashi03Models[i], &tevStr);
        }
        break;

    case 4:
    case 8:
        for (int i = 0; i < 40; i++) {
            g_env_light.setLightTevColorType_MAJI(mThashi03Models[i], &tevStr);
        }
        g_env_light.setLightTevColorType_MAJI(mThashi01Model, &tevStr);
        break;
    }
    dComIfGd_setListBG();
    switch (unk5A4) {
    case 0:
        mDoExt_modelUpdateDL(mThashi00Model);
        break;
    case 1:
    case 2:
    case 7:
        for (int i = 0; i < 10; i++) {
            mDoExt_modelUpdateDL(mThashi03Models[i]);
        }
        mDoExt_modelUpdateDL(mThashi01Model);
        mDoExt_modelUpdateDL(mThashi02Model);
        break;
    case 3:
        for (int i = 0; i < 50; i++) {
            mDoExt_modelUpdateDL(mThashi03Models[i]);
        }
        break;

    case 4:
    case 8:
        for (int i = 0; i < 40; i++) {
            mDoExt_modelUpdateDL(mThashi03Models[i]);
        }
        mDoExt_modelUpdateDL(mThashi01Model);
        break;
    }
    return TRUE;
}

// has to be inline to maintain function order with Draw as inline
inline int daObjTHASHI_c::Delete() {
    dComIfG_resDelete(&unk11A8, l_arcName);
    return TRUE;
}
