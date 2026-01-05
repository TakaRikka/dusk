/**
 * @file d_a_obj_ornament_cloth.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ornament_cloth.h"
#include "d/actor/d_a_player.h"
#include "d/d_a_obj.h"

enum J_Necktie_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_J_NECKTIE = 0x4,

    /* BTK */
    /* 0x7 */ BTK_J_NECKTIE = 0x7,
};

#if DEBUG
class daObjOnCloth_Hio_c : public JORReflexible {
public:

    void default_set() { mAttr = daObjOnCloth_c::M_attr; }

    void genMessage(JORMContext* ctx) {
        ctx->genLabel("§ 飾り布パラメータ設定  §\n", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18); // § Ornamental Fabric Parameter Strings §
        ctx->genSlider("重力", &mAttr.gravity, 0.0f, 30.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("オフセット", &mAttr.offset, -100.0f, 100.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("windRate", &mAttr.windRate, 0.0f, 5.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("speedRate", &mAttr.speedRate, 0.0f, 5.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("revPowRate", &mAttr.revPowRate, 0.0f, 1.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("回転限界[0]", &mAttr.rotationLimit[0], -0x4000, 0x3FFF, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("回転限界[1]", &mAttr.rotationLimit[1], -0x4000, 0x3FFF, 0, 
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("回転限界[2]", &mAttr.rotationLimit[2], -0x4000, 0x3FFF, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("プレイヤ反応距離", &mAttr.playerReactionDist, 0.0f, 500.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("プレイヤ反応速度", &mAttr.playerReactionSpeed, 0.0f, 50.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("プレイヤ反応係数", &mAttr.playerReactionCoeff, 0.0f, 5.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("反射還元力係数", &mAttr.reflectReducePowCoeff, 0.0f, 1.0f, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genSlider("揺れ遅延フレーム", &mAttr.delayedVibrationFrame, 1, 30, 0,
                       NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        ctx->genCheckBox("ジョイント表示", &mAttr.jointDisplay, 1, 0,
                         NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    }

    void ct() {
        if (mCount++ == 0) {
            daObj::HioVarious_c::init(this, "飾り布"); // Ornamental Cloth
        }
    }

    void dt() {
        if (--mCount == 0) {
            daObj::HioVarious_c::clean(this);
        }
    }

    /* 0x4 */ int mCount;
    /* 0x8 */ daObjOnCloth_Attr_c mAttr;
};
#endif

daObjOnCloth_Attr_c const daObjOnCloth_c::M_attr = {
    8.0f,
    -40.0f,
    0.3f,
    0.47f,
    0.3f,
    150.0f,
    14.0f,
    0.08f,
    0.8f,
    0x040,
    0x431,
    0x431,
    15,
    0
};

void daObjOnCloth_c::create_init() {
    fopAcM_setCullSizeBox(this, -50.0f, -170.0f, -50.0f, 50.0f, 20.0f, 50.0f);
    ClothJoint_c* clothJoints_p = mClothJoints;

    for (int i = 0; i < 3; i++, clothJoints_p++) {
        clothJoints_p->field_0x0 = current.pos;
        clothJoints_p->field_0x0.y += (i + 1) * attr().offset;
        clothJoints_p[0].field_0xc = clothJoints_p[0].field_0x0;
        clothJoints_p->field_0x24.set(cM_ssin(shape_angle.y) * -1.0f, 0.0f, cM_scos(shape_angle.y) * -1.0f);
    }

    gravity = attr().gravity;
    initBaseMtx();
}

void daObjOnCloth_c::initBaseMtx() {
    setBaseMtx();
}

void daObjOnCloth_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
}

void daObjOnCloth_c::setNormalClothPos() {
    cXyz sp4c = dKyw_get_AllWind_vecpow(&current.pos);
    sp4c *= attr().offset * attr().windRate;
    f32 fVar1 = sp4c.abs();
    cXyz sp58;
    ClothJoint_c* clothJoints_p = mClothJoints;
    cXyz* pos = &current.pos;

    for (int i = 0; i < 3; i++, clothJoints_p++) {
        sp58 = *pos - clothJoints_p->field_0x0;
        sp58.y += gravity;

        if (cM_rnd() < 0.6f && cM_rnd() < 0.1f) {
            clothJoints_p->field_0x18 += clothJoints_p->field_0x24 * fVar1;
        }

        sp58 += clothJoints_p->field_0x18;
        sp58.normalizeZP();
        clothJoints_p->field_0x0 = *pos + (sp58 * attr().offset);
        clothJoints_p->field_0x18 = (clothJoints_p->field_0x18 + (clothJoints_p->field_0xc - clothJoints_p->field_0x0)) * attr().speedRate;
        clothJoints_p->field_0xc = clothJoints_p->field_0x0;
        pos = &clothJoints_p->field_0x0;
    }
}

void daObjOnCloth_c::calcJointAngle() {
    cXyz sp24;
    ClothJoint_c* clothJoints_p = mClothJoints;
    
    mDoMtx_stack_c::copy(mMtx);

    for (int i = 0; i < 3; i++, clothJoints_p++) {
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::inverse();
        mDoMtx_stack_c::multVec(&clothJoints_p->field_0x0, &sp24);
        mDoMtx_stack_c::pop();
        sp24 *= -1.0f;
        clothJoints_p->field_0x30.x = cM_atan2s(sp24.z, sp24.y);

        if (getColorType() != 4.0f) {
            if (attr().rotationLimit[i] > 0) {
                if (clothJoints_p->field_0x30.x > attr().rotationLimit[i]) {
                    clothJoints_p->field_0x30.x = attr().rotationLimit[i];
                    clothJoints_p->field_0x18.zero();
                }
            } else if (attr().rotationLimit[i] < 0 && clothJoints_p->field_0x30.x < attr().rotationLimit[i]) {
                clothJoints_p->field_0x30.x = attr().rotationLimit[i];
                clothJoints_p->field_0x18.zero();
            }
        }

        mDoMtx_stack_c::XrotM(clothJoints_p->field_0x30.x);
        mDoMtx_stack_c::transM(0.0f, attr().offset, 0.0f);
    }
}

void daObjOnCloth_c::checkPlayerMove() {
    f32 fVar1 = field_0x650[0];
    ClothJoint_c* clothJoints_p = mClothJoints + 1;

    for (int i = 2; i > 0; i--, clothJoints_p--) {
        clothJoints_p->field_0x18 += clothJoints_p->field_0x24 * (-fVar1 * attr().playerReactionCoeff);
        fVar1 = fVar1 * attr().reflectReducePowCoeff;
    }

    f32* pfVar1 = &field_0x650[0];
    f32* pfVar2 = &field_0x650[1];

    for (int i = 0; i < attr().delayedVibrationFrame - 1; i++) {
        *pfVar1 = *pfVar2;
        pfVar1++;
        pfVar2++;
    }

    fVar1 = 0.0f;
    field_0x650[attr().delayedVibrationFrame - 1] = 0.0f;

    if (!(fopAcM_searchPlayerDistanceXZ2(this) > attr().playerReactionDist * attr().playerReactionDist)) {
        f32 fVar2 = fopAcM_GetSpeedF(daPy_getPlayerActorClass());
        if (fVar2 > attr().playerReactionSpeed) {
            field_0x650[attr().delayedVibrationFrame - 1] = fVar2;
        }
    }
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 != 0) {
        return 1;
    }

    int jntNo = i_joint->getJntNo();
    J3DModel* model = j3dSys.getModel();
    daObjOnCloth_c* i_this = (daObjOnCloth_c*)model->getUserArea();
    csXyz angle;
    i_this->getJointAngle(angle, jntNo);

    MTXCopy(model->getAnmMtx(jntNo), mDoMtx_stack_c::get());
    mDoMtx_stack_c::XrotM(angle.x);
    model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

static char* l_arcName = "J_Necktie";

int daObjOnCloth_c::createHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, "J_Necktie.bmd"));

    JUT_ASSERT(413, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, "J_Necktie.btk");

    JUT_ASSERT(420, pbtk != NULL);

    mBtkAnm = new mDoExt_btkAnm();

    JUT_ASSERT(424, mBtkAnm);

    if (mBtkAnm->init(modelData, pbtk, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }
    
    mBtkAnm->setFrame(getColorType());

    for (u16 i = 0; i < 3; i++) {
        J3DJoint* jointNode_p = mpModel->getModelData()->getJointNodePointer(i);
        if (jointNode_p != NULL) {
            jointNode_p->setCallBack(nodeCallBack);
            mpModel->setUserArea((u32)this);
        }
    }

    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static int createSolidHeap(fopAc_ac_c* a_this) {
    return static_cast<daObjOnCloth_c*>(a_this)->createHeap();
}

int daObjOnCloth_c::draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mBtkAnm->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

static int daObjOnCloth_Draw(daObjOnCloth_c* i_this) {
    return i_this->draw();
}

int daObjOnCloth_c::execute() {
    checkPlayerMove();
    setNormalClothPos();
    calcJointAngle();
    return 1;
}

static int daObjOnCloth_Execute(daObjOnCloth_c* i_this) {
    return i_this->execute();
}

static int daObjOnCloth_IsDelete(daObjOnCloth_c* i_this) {
    return 1;
}

daObjOnCloth_c::~daObjOnCloth_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjOnCloth_Delete(daObjOnCloth_c* i_this) {
    i_this->~daObjOnCloth_c();
    return 1;
}

ClothJoint_c::~ClothJoint_c() {}

cPhs__Step daObjOnCloth_c::create() {
    fopAcM_ct(this, daObjOnCloth_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x9A0)) {
            return cPhs_ERROR_e;
        }

        create_init();
        fopAcM_SetMtx(this, mMtx);
    }

    return phase;
}

static cPhs__Step daObjOnCloth_Create(fopAc_ac_c* a_this) {
    return static_cast<daObjOnCloth_c*>(a_this)->create();
}

ClothJoint_c::ClothJoint_c() {
    /* empty function */
}

static actor_method_class l_daObjOnCloth_Method = {
    (process_method_func)daObjOnCloth_Create,
    (process_method_func)daObjOnCloth_Delete,
    (process_method_func)daObjOnCloth_Execute,
    (process_method_func)daObjOnCloth_IsDelete,
    (process_method_func)daObjOnCloth_Draw,
};

actor_process_profile_definition g_profile_Obj_OnCloth = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_OnCloth,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjOnCloth_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  33,                     // mPriority
  &l_daObjOnCloth_Method, // sub_method
  0x00040180,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
