/**
 * @file d_a_obj_kazeneko.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kazeneko.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_npc_tk.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

KazoNekoAttr const daObjKazeNeko_c::M_attr = {
    30.0f, 120.0f, 3000.0f, 7000.0f, 90.0f, 
    1800.0f, 180.0f, 27.0f, 15.0f,
    1, 0x70, 0x6400, 0x190000,
};

void daObjKazeNeko_c::create_init() {
    fopAcM_setCullSizeBox(this, -100.0f, -10.0f, -100.0f, 100.0f, 200.0f, 100.0f);
    field_0x72c.set(current.pos.x, current.pos.y + attr().field_0x1c, current.pos.z);
    field_0x752 = 0;
    initBaseMtx();
    initCcCylinder();
}

void daObjKazeNeko_c::initBaseMtx() {
    setBaseMtx();
}

void daObjKazeNeko_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::ZrotM(shape_angle.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mMtx);
    if (field_0x738 == 0) {
        mDoMtx_stack_c::transM(0.0f, attr().field_0x1c, 0.0f);
        mDoMtx_stack_c::YrotM(field_0x748);
        for (int i = 0; i < 4; i++) {
            mDoMtx_stack_c::YrotM(0x4000);
            mArmModels[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

const static dCcD_SrcCyl ccCylSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x2048, 0x11}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        10.0f, // mRadius
        30.0f // mHeight
    } // mCyl
};

void daObjKazeNeko_c::initCcCylinder() {
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(ccCylSrc);
    mCyl.SetStts(&mStts);
    mCyl.SetC(current.pos);
    mCyl.SetR(attr().mCylRadius);
    mCyl.SetH(attr().mCylHeight);
}

void daObjKazeNeko_c::setCcCylinder() {
    mCyl.SetC(current.pos);
    mCyl.SetR(attr().mCylRadius);
    mCyl.SetH(attr().mCylHeight);
    dComIfG_Ccsp()->Set(&mCyl);
}

void daObjKazeNeko_c::swingHead() {
    if (field_0x744 != 0.0f) {
        f32 dVar5 = field_0x744 * cM_ssin(field_0x74c);
        shape_angle.x = dVar5 * cM_ssin(field_0x74e);
        shape_angle.z = dVar5 * cM_scos(field_0x74e);
        cLib_chaseF(&field_0x744, 0.0f, attr().field_0x18);
        field_0x74c += attr().field_0x26;
    }
}

static f32 dummyFloat1() {
    return 1.0f;
}


static f32 dummyFloat2() {
    return -1.0f;
}

void daObjKazeNeko_c::getFirstVec(cXyz* param_1, int param_2) {
    s16 sVar4 = field_0x748 + (param_2 << 14);
    param_1->set((attr().field_0x20 * cM_ssin(sVar4)), 30.0f, attr().field_0x20 * cM_scos(sVar4));
}

static char* l_arcName = "J_Kazami";

inline int daObjKazeNeko_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "pole.bmd");
    JUT_ASSERT(458, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return 0;
    }
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "arm.bmd");
    JUT_ASSERT(465, modelData != NULL);
    for (int i = 0; i < 4; i++) {
        mArmModels[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mArmModels[i] == NULL) {
            return 0;
        }
    }
    return 1;
}


static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjKazeNeko_c*>(i_this)->createHeap();
}

inline int daObjKazeNeko_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    if (field_0x738 == 1) {
        return 1;
    }
    for (int i = 0; i < 4; i++) {
        g_env_light.setLightTevColorType_MAJI(

            mArmModels[i], &tevStr);
        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mArmModels[i]);
        dComIfGd_setList();
    }
    return 1;
}

static int daObjKazeNeko_Draw(daObjKazeNeko_c* i_this) {
    return i_this->draw();
}

inline int daObjKazeNeko_c::execute() {
    if (field_0x738 == 0) {
        field_0x73c = attr().field_0x08 * dKyw_get_wind_pow();
        field_0x748 = field_0x748 + field_0x73c + field_0x740;
        cLib_chaseF(&field_0x740, 0.0f, attr().field_0x10);
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_CAT_VANE_ROLL, &field_0x72c, 0, 0, 1.0f, 1.0f, -1.0f,
                                      -1.0f, 0);
    }
    if (mCyl.ChkTgHit() != 0) {
        daNPC_TK_c* tk = (daNPC_TK_c*)mCyl.GetTgHitAc();
        if (fopAcM_GetName(tk) == PROC_NPC_TK) {
            field_0x740 = attr().field_0x0c;
            tk->setBump();
        }
    }
    swingHead();
    if (field_0x750 > 0) {
        if (--field_0x750 <= 0) {
            daNPC_TK_c* tk = (daNPC_TK_c*)fopAcM_SearchByName(PROC_NPC_TK);
            if (tk != NULL) {
                tk->endHawkCamera();
                field_0x752 = 1;
            }
        }
    }
    setBaseMtx();
    setCcCylinder();
    return 1;
}

static int daObjKazeNeko_Execute(daObjKazeNeko_c* i_this) {
    return i_this->execute();
}

static int daObjKazeNeko_IsDelete(daObjKazeNeko_c* i_this) {
    return 1;
}

inline daObjKazeNeko_c::~daObjKazeNeko_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjKazeNeko_Delete(daObjKazeNeko_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->~daObjKazeNeko_c();
    return 1;
}

inline int daObjKazeNeko_c::create() {
    fopAcM_ct(this, daObjKazeNeko_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, createSolidHeap, 0x2800) == 0) {
            return cPhs_ERROR_e;
        } 
        create_init();
        fopAcM_SetMtx(this, mMtx);
    }
    return rv;
}

static int daObjKazeNeko_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjKazeNeko_c*>(i_this)->create();
}


static actor_method_class l_daObjKazeNeko_Method = {
    (process_method_func)daObjKazeNeko_Create,
    (process_method_func)daObjKazeNeko_Delete,
    (process_method_func)daObjKazeNeko_Execute,
    (process_method_func)daObjKazeNeko_IsDelete,
    (process_method_func)daObjKazeNeko_Draw,
};

extern actor_process_profile_definition g_profile_Obj_KazeNeko = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_KazeNeko,       // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjKazeNeko_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  30,                      // mPriority
  &l_daObjKazeNeko_Method, // sub_method
  0x00044180,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
