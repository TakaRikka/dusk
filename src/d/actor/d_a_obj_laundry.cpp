/**
* @file d_a_obj_laundry.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_laundry.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_wether.h"

daObjLdy_Attr_c const daObjLdy_c::mAttr = {5.0f, 30.0f, 130.0f, -50.0f, 0.15f, 0.45f, 0.3f, 1000.0f,
                                           0.0f, 0.0f,  0.0f,   0.0f,   0,     10,    0};

void daObjLdy_c::create_init() {
    fopAcM_setCullSizeBox(this, -60.0f, -200.0f, -60.0f, 60.0f, 50.0f, 60.0f);
    fopAcM_setCullSizeFar(this, 1.5f);

    LaundJoint_c* joint = &mJoints[0];
    int index = 0;
    for (int i = 0; i < 3; i++) {
        joint->mPos1 = current.pos;
        joint->mPos1.y += (index + 1) * mAttr.field_0xc;
        joint->mPos2 = joint->mPos1;
        index++;
        joint++;
    }

    gravity = mAttr.field_0x0;
    initBaseMtx();
}

void daObjLdy_c::initBaseMtx() {
    setBaseMtx();

    LaundJoint_c* joint = &mJoints[0];
    for (int i = 0; i < 3; i++) {
        f32 cosAngleY = cM_scos(shape_angle.y);
        f32 sinAngleY = cM_ssin(shape_angle.y);
        joint->mPos4.set(sinAngleY, 0.0f, cosAngleY);
        joint++;
    }
}

void daObjLdy_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZrotM(shape_angle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    PSMTXCopy(mDoMtx_stack_c::get(), mMtx);
}

void daObjLdy_c::getJointAngle(csXyz* i_jointAngle, int i_index) {
    LaundJoint_c* joint = &mJoints[i_index];
    *i_jointAngle = joint->mAngle;
}

const static dCcD_SrcCyl ccCylSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x16048, 0x11}, 0x149}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},                // mGObjTg
        {0x0},                                             // mGObjCo
    },                                                     // mObjInf
    {
        {
            {0.0f, -80.0f, 0.0f},  // mCenter
            25.0f,                 // mRadius
            25.0f                  // mHeight
        }  // mCyl
    }
};

static f32 dummy(cXyz v) {
    return 40.0f + v.abs();
}

void daObjLdy_c::setNormalClothPos() {
    cXyz adjustedPosition;
    cXyz windVector = dKyw_get_AllWind_vecpow(&current.pos);
    windVector *= mAttr.field_0xc * mAttr.field_0x10;
    f32 windPower = windVector.abs();
    LaundJoint_c* joint = &mJoints[0];

    if (mCyl.ChkTgHit() != 0) {
        cCcD_Obj* tgHitObj = mCyl.GetTgHitObj();
        if (tgHitObj->ChkAtType(AT_TYPE_40) != 0 || tgHitObj->ChkAtType(AT_TYPE_ARROW) != 0) {
            cXyz position = fopAcM_GetPosition(dComIfGp_getPlayer(0)) - mJoints[1].mPos1;
            position.normalizeZP();
            position *= 100.0f;
            for (int i = 2; i >= 0; i--) {
                mJoints[i].mPos3 = position;
                position *= mAttr.field_0x18;
            }
            divorceParent();
        } else {
            if (tgHitObj->ChkAtType(AT_TYPE_BOOMERANG) != 0) {
                divorceParent();
            }
        }
    } else {
        if (mCyl.ChkCoHit() != 0) {
            if (fopAcM_GetName(mCyl.GetCoHitAc()) == PROC_NPC_TK) {
                cXyz position = fopAcM_GetPosition(dComIfGp_getPlayer(0)) - mJoints[1].mPos1;
                position.normalizeZP();
                position *= 100.0f;
                for (int i = 2; i >= 0; i--) {
                    mJoints[i].mPos3 = position;
                    position *= mAttr.field_0x18;
                }
                divorceParent();
            }
        } else {
            if (!windVector.isZero()) {
                for (int i = 0; i < 3; i++) {
                    if (cM_rnd() < 0.6f && cM_rnd() < 0.1f) {
                        joint->mPos3 += joint->mPos4 * windPower;
                    }
                    joint++;
                }
            }
        }
    }

    int i;
    LaundJoint_c* mJoint = &mJoints[0];
    cXyz* currentPosition = &fopAcM_GetPosition(this);
    for (i = 0; i < 3; i++) {
        adjustedPosition = *currentPosition - mJoint->mPos1;
        adjustedPosition.y += gravity;
        adjustedPosition += mJoint->mPos3;
        adjustedPosition.normalizeZP();
        mJoint->mPos1 = *currentPosition + (adjustedPosition * mAttr.field_0xc);
        mJoint->mPos3 = (mJoint->mPos3 + (mJoint->mPos2 - mJoint->mPos1)) * mAttr.field_0x14;
        mJoint->mPos2 = mJoint->mPos1;
        currentPosition = &mJoint->mPos1;
        mJoint++;
    }
}

void daObjLdy_c::calcJointAngle() {
    cXyz position;
    LaundJoint_c* joint = &mJoints[0];
    mDoMtx_stack_c::copy(mMtx);
    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::inverse();
        mDoMtx_stack_c::multVec(&joint->mPos1, &position);
        mDoMtx_stack_c::pop();
        position *= -1.0f;
        joint->mAngle.x = cM_atan2s(position.z, position.y);
        joint->mAngle.z = cM_atan2s(-position.y, position.absXZ());
        mDoMtx_stack_c::XrotM(joint->mAngle.x);
        mDoMtx_stack_c::transM(0.0f, mAttr.field_0xc, 0.0f);
        joint++;
    }
}

bool daObjLdy_c::divorceParent() {
    return true;
}

static int nodeCallBack(J3DJoint* i_joint, int i_callbackCondition) {
    J3DModel* jointModel;
    u16 jointNo;
    csXyz jointAngle[2];

    if (i_callbackCondition != 0) {
        return 1;
    }

    jointNo = i_joint->getJntNo();
    jointModel = j3dSys.getModel();
    ((daObjLdy_c*)jointModel->getUserArea())->getJointAngle(jointAngle, jointNo);
    cMtx_copy(jointModel->getAnmMtx(jointNo), mDoMtx_stack_c::get());
    mDoMtx_stack_c::XrotM(jointAngle->x);
    jointModel->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

static char* l_arcName = "J_Sentaku";

int daObjLdy_c::getObjType() {
    return fopAcM_GetParam(this) & 0xff;
}

int daObjLdy_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "J_Sentaku.bmd");
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    J3DAnmTextureSRTKey* key =
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, "J_Sentaku.btk");

    mpBtkAnm = new mDoExt_btkAnm();
    int initResult = mpBtkAnm->init(modelData, key, 1, 2, 1.0f, 0, -1);
    if (initResult == 0) {
        return 0;
    }

    mpBtkAnm->setFrame(getObjType());
    for (u16 i = 0; i < 3; i++) {
        J3DJoint* joint = mpModel->getModelData()->getJointNodePointer(i);
        if (joint != NULL) {
            joint->setCallBack(nodeCallBack);
            mpModel->setUserArea((uintptr_t)this);
        }
    }

    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjLdy_c*>(i_this)->createHeap();
}

int daObjLdy_c::daObjLdy_Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mpBtkAnm->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

static int daObjLdy_Draw(daObjLdy_c* i_this) {
    return i_this->daObjLdy_Draw();
}

int daObjLdy_c::daObjLdy_Execute() {
    setNormalClothPos();
    setBaseMtx();
    calcJointAngle();
    return 1;
}

static int daObjLdy_Execute(daObjLdy_c* i_this) {
    return i_this->daObjLdy_Execute();
}

static int daObjLdy_IsDelete(daObjLdy_c* i_this) {
    return 1;
}

daObjLdy_c::~daObjLdy_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjLdy_Delete(daObjLdy_c* i_this) {
    i_this->~daObjLdy_c();
    return 1;
}

int daObjLdy_c::create() {
    fopAcM_ct(this, daObjLdy_c);
    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x9a0)) {
            phase = cPhs_ERROR_e;
        } else {
            create_init();
            fopAcM_SetMtx(this, mMtx);
        }
    }

    return phase;
}

static int daObjLdy_Create(fopAc_ac_c* i_this) {
    return ((daObjLdy_c*)i_this)->create();
}

static actor_method_class l_daObjLdy_Method = {
    (process_method_func)daObjLdy_Create,  (process_method_func)daObjLdy_Delete,
    (process_method_func)daObjLdy_Execute, (process_method_func)daObjLdy_IsDelete,
    (process_method_func)daObjLdy_Draw,
};

actor_process_profile_definition g_profile_Obj_Laundry = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Laundry,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjLdy_c),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    37,                      // mPriority
    &l_daObjLdy_Method,      // sub_method
    0x00040180,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
