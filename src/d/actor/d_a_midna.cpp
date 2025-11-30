/**
 * @file d_a_midna.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_midna.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_kago.h"
#include "d/d_demo.h"
#include "d/d_msg_object.h"
#include "d/d_s_play.h"
#include "d/d_debug_viewer.h"

static f32 dummy_lit_3777(int idx, u8 foo) {
    Vec dummy_vec = {0.0f, 0.0f, 0.0f};
    switch (idx) {
        case 0: return dummy_vec.x;
        case 1: return dummy_vec.y;
        default: return dummy_vec.z;
    }
}

static char const l_arcName[6] = "Midna";

static Vec const l_eyeOffset = { 16.0f, -10.400001f, 0.0f };

static Vec const l_headCenterOffset = { 0.0f, -10.400001f, 0.0f };

static GXColorS10 const l_normalColor = { 0x50, 0x00, 0x00, 0x00 };

static GXColor const l_normalKColor = { 0xB4, 0x87, 0x00, 0x00 };

static GXColor const l_normalKColor2 = { 0x00, 0xC3, 0xC3, 0x00 };

static GXColorS10 const l_bigColor = { 0xFF, 0x64, 0x78, 0x00 };

static GXColor const l_bigKColor = { 0x1E, 0x00, 0x00, 0x00 };

static GXColor const l_lNormalKColor = { 0xFF, 0xDC, 0x00, 0x00 };

static GXColor const l_lNormalKColor2 = { 0x00, 0xC3, 0xEB, 0x00 };

static GXColorS10 const l_lBigColor = { 0xFF, 0x78, 0x00, 0x00 };

static GXColor const l_lBigKColor2 = { 0xAA, 0xFF, 0xC3, 0x00 };

static Vec const l_hairScale[5] = {
    {0.3f, 0.8f, 0.7f},
    {0.2f, 0.8f, 0.4f},
    {0.15f, 0.75f, 0.5f},
    {0.1f, 0.7f, 0.7f},
    {1.0f, 1.0f, 1.0f},
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_HAIR_1,
    /* 0x07 */ JNT_HAIR_2,
    /* 0x08 */ JNT_HAIR_3,
    /* 0x09 */ JNT_HAIR_4,
    /* 0x0A */ JNT_HAIR_5,
    /* 0x0B */ JNT_MOUTH,
    /* 0x0C */ JNT_SHOULDER_L,
    /* 0x0D */ JNT_ARM_L1,
    /* 0x0E */ JNT_ARM_L2,
    /* 0x0F */ JNT_HAND_L,
    /* 0x10 */ JNT_SHOULDER_R,
    /* 0x11 */ JNT_ARM_R1,
    /* 0x12 */ JNT_ARM_R2,
    /* 0x13 */ JNT_HAND_R,
    /* 0x14 */ JNT_WAIST,
    /* 0x15 */ JNT_CLOTCHL,
    /* 0x16 */ JNT_LEG_L1,
    /* 0x17 */ JNT_LEG_L2,
    /* 0x18 */ JNT_FOOT_L,
    /* 0x19 */ JNT_CLOTCHR,
    /* 0x1A */ JNT_LEG_R1,
    /* 0x1B */ JNT_LEG_R2,
    /* 0x1C */ JNT_FOOT_R,
    /* 0x1D */ JNT_MAX,
};

#if DEBUG
void daMidna_hio_c::genMessage(JORMContext* ctx) {
    ctx->genSlider("スケール", &m.scale, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genCheckBox("強制表示", &m.forced_display, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("Ｙ位置", &m.y_pos, -1000.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("Ｚ位置", &m.z_pos, -1000.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genCheckBox("ＨＩＯ優先", &m.hio_priority, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪カラー１Ｒ", &m.hair_color_1r, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪カラー１Ｇ", &m.hair_color_1g, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪カラー１Ｂ", &m.hair_color_1b, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪Ｋカラー１Ｒ", &m.hair_k_color_1r, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪Ｋカラー１Ｇ", &m.hair_k_color_1g, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪Ｋカラー１Ｂ", &m.hair_k_color_1b, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪Ｋカラー２Ｒ", &m.hair_k_color_2r, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪Ｋカラー２Ｇ", &m.hair_k_color_2g, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("髪Ｋカラー２Ｂ", &m.hair_k_color_2b, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

daMidna_hio_c1 const daMidna_hio_c0::m = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1.0f,
    -87.0f,
    25.0f,
};

#if VERSION != VERSION_GCN_PAL
bool daMidna_matAnm_c::sEyeMoveFlg;
u8 daMidna_matAnm_c::sMorfFrame;
#endif

void daMidna_matAnm_c::init() {
    mOldTransX = 0.0f;
    mOldTransY = 0.0f;
    mNowOffsetX = 0.0f;
    mNowOffsetY = 0.0f;
    sEyeMoveFlg = false;
    sMorfFrame = 0;
}

void daMidna_matAnm_c::calc(J3DMaterial* i_material) const {
    J3DMaterialAnm::calc(i_material);
    for (u32 i = 0; i < 8; i++) {
        if (getTexMtxAnm(i).getAnmFlag()) {
            J3DTexMtxInfo& tex_mtx = i_material->getTexGenBlock()->getTexMtx(i)->getTexMtxInfo();
            if (sMorfFrame != 0) {
                f32 t = 1.0f / (sMorfFrame + 1);
                tex_mtx.mSRT.mTranslationX =
                    mOldTransX * (1.0f - t) + tex_mtx.mSRT.mTranslationX * t;
                tex_mtx.mSRT.mTranslationY =
                    mOldTransY * (1.0f - t) + tex_mtx.mSRT.mTranslationY * t;
            } else if (sEyeMoveFlg != 0) {
                tex_mtx.mSRT.mTranslationX = mNowOffsetX;
                tex_mtx.mSRT.mTranslationY = mNowOffsetY;
            }

            const_cast<daMidna_matAnm_c*>(this)->mOldTransX = tex_mtx.mSRT.mTranslationX;
            const_cast<daMidna_matAnm_c*>(this)->mOldTransY = tex_mtx.mSRT.mTranslationY;
        }
    }
}

int daMidna_McaMorfCB1_c::execute(u16 i_jointNo, J3DTransformInfo* transform) {
    if (!daPy_py_c::getMidnaActor()->checkNoHairScale()) {
        if (i_jointNo >= JNT_HAIR_1 && i_jointNo <= JNT_HAIR_5) {
            cXyz* scale = &mpScale[i_jointNo - JNT_HAIR_1];
            transform->mScale.x *= scale->x;
            transform->mScale.y *= scale->y;
            transform->mScale.z *= scale->z;
        }
    }

    return 1;
}

daMidna_texData_s const daMidna_c::m_texDataTable[21] = {
    0x0405, 0x03A4,
    0x03F5, 0x0399,
    0x03F6, 0x0399,
    0x0407, 0x0399,
    0x0406, 0x03A5,
    0x0405, 0x03A0,
    0x03FA, 0x0399,
    0x0401, 0x039E,
    0x03F7, 0x039F,
    0x0403, 0x03A2,
    0x0404, 0x03A3,
    0x03F7, 0x03A1,
    0x03FC, 0x039A,
    0x0402, 0x0399,
    0x03FE, 0x039B,
    0x03FF, 0x039C,
    0x03F8, 0x0399,
    0x03F9, 0x0399,
    0x0400, 0x039D,
    0x03FB, 0x0399,
    0x03FD, 0x0399,
};

daMidna_anmData_s const daMidna_c::m_anmDataTable[53] = {
    0x01DC, 0x00, -1.0f, 0,
    0x01B7, 0x01, -1.0f, 0,
    0x01B8, 0x02, -1.0f, 0,
    0x01DD, 0x03, -1.0f, 0,
    0x01D3, 0x04, 1.0f, Z2SE_MDN_V_LV_PANIC,
    0x01DB, 0x00, -1.0f, 0,
    0x01C8, 0x05, 1.0f, Z2SE_MDN_V_FLY_OUT,
    0x01C7, 0x00, -1.0f, 0,
    0x01CB, 0x10, -1.0f, 0,
    0x01CA, 0x00, 2.0f, Z2SE_MDN_V_HIP,
    0x01C9, 0x00, 0.1f, Z2SE_MDN_V_LEADSWAIT,
    0x01BA, 0x06, 3.0f, Z2SE_MDN_V_CLINGST,
    0x01D8, 0x07, 3.0f, Z2SE_MDN_V_SWAITB_1,
    0x01D9, 0x08, 1.0f, Z2SE_MDN_V_SWAITC,
    0x01E1, 0x09, 1.0f, Z2SE_MDN_V_WLDIEA,
    0x01E2, 0x0A, 1.0f, Z2SE_MDN_V_WLSWIMDIE,
    0x0217, 0x00, -1.0f, 0,
    0x0218, 0x00, -1.0f, 0,
    0x021C, 0x00, -1.0f, 0,
    0x0219, 0x00, -1.0f, 0,
    0x01E0, 0x0B, -1.0f, 0,
    0x01DF, 0x10, 7.0f, Z2SE_MDN_V_WAITTP,
    0x01BE, 0x0C, 1.0f, Z2SE_MDN_V_ENTRANCE,
    0x01DA, 0x0D, 28.0f, Z2SE_MDN_V_SWAITD,
    0x01C6, 0x0E, 10.0f, Z2SE_MDN_V_HIT,
    0x01CC, 0x0F, 11.0f, Z2SE_MDN_V_LOOKABOUT_1,
    0x01D7, 0x12, 8.0f, Z2SE_MDN_V_SWAITA,
    0x021A, 0x00, -1.0f, 0,
    0x01D4, 0x00, -1.0f, 0,
    0x01DE, 0x11, -1.0f, 0,
    0x01B9, 0x11, -1.0f, 0,
    0x01D6, 0x00, 27.0f, Z2SE_MDN_V_ROPEWAIT_1,
    0x01D5, 0x00, 48.0f, Z2SE_MDN_V_ROPEFALTER,
    0x01BB, 0x00, -1.0f, 0,
    0x01BC, 0x00, 17.0f, Z2SE_MDN_V_DOOROP,
    0x01BD, 0x00, 20.0f, Z2SE_MDN_V_DOOROP,
    0x01C2, 0x13, -1.0f, 0,
    0x01C3, 0x14, 0.1f, Z2SE_MDN_V_WARP_LIFTUP,
    0x01CE, 0x10, 1.0f, Z2SE_MDN_V_MGN_CATCH,
    0x01CD, 0x10, -1.0f, 0,
    0x01D0, 0x10, 1.0f, Z2SE_MDN_V_MGN_TAME,
    0x01CF, 0x10, 1.0f, Z2SE_MDN_V_MGN_THROW,
    0x01D2, 0x10, 1.0f, Z2SE_MDN_V_MGN_TAME,
    0x01D1, 0x10, 1.0f, Z2SE_MDN_V_MGN_THROW,
    0x01BF, 0x00, -1.0f, 0,
    0x01C0, 0x10, -1.0f, 0,
    0x01C1, 0x00, -1.0f, 0,
    0x01C1, 0x11, -1.0f, 0,
    0x01C5, 0x00, -1.0f, 0,
    0x021D, 0x00, -1.0f, 0,
    0x021E, 0x00, -1.0f, 0,
    0x021B, 0x00, -1.0f, 0,
    0x01C4, 0x00, -1.0f, 0,
};

int daMidna_c::modelCallBack(int i_jointNo) {
    mDoMtx_stack_c::copy(J3DSys::mCurrentMtx);
    if ((i_jointNo == JNT_BACKBONE1 || i_jointNo == JNT_WAIST) && mDemoMode == 0x200) {
        mDoMtx_stack_c::get()[0][3] = current.pos.x;
        mDoMtx_stack_c::get()[2][3] = current.pos.z;
    }

    if (i_jointNo == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(-mNeckAngle.y);
        mDoMtx_stack_c::ZrotM(mNeckAngle.x);
    } else if (i_jointNo == JNT_BACKBONE1 || i_jointNo == JNT_BACKBONE2) {
        mDoMtx_stack_c::ZrotM(mBackboneAngleZ);
    } else {
        int hair_joint_no = i_jointNo - JNT_HAIR_1;
        if (checkStateFlg0(FLG0_UNK_10000000)) {
            if (i_jointNo != JNT_HAIR_1) {
                mDoMtx_stack_c::YrotM(-mHairAngleY[hair_joint_no - 1]);
                mDoMtx_stack_c::ZrotM(-mHairAngleZ[hair_joint_no - 1]);
            } else {
                mDoMtx_stack_c::transS(J3DSys::mCurrentMtx[0][3], J3DSys::mCurrentMtx[1][3],
                                       J3DSys::mCurrentMtx[2][3]);
                mDoMtx_stack_c::YrotM(shape_angle.y + 0x4000);
                mDoMtx_stack_c::XrotM(0x8000);
                mDoMtx_stack_c::scaleM(mpHIO->m.scale, mpHIO->m.scale,
                                       mpHIO->m.scale);
            }

            mDoMtx_stack_c::XYZrotM(0, mHairAngleY[hair_joint_no], mHairAngleZ[hair_joint_no]);
        }
    }

    mpShadowModel->setAnmMtx(i_jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daMidna_c::changeUpperBck() {
    J3DAnmTransform* anm = mUpperBck.getBckAnm();
    mUpperBck.changeBckOnly(mpMorf->getAnm());
    mpMorf->changeAnm(anm);
    return 1;
}

int daMidna_c::changeFaceBck() {
    if (checkStateFlg0(FLG0_UNK_800000)) {
        J3DAnmTransform* anm = mFaceBck.getBckAnm();
        mFaceBck.changeBckOnly(mpMorf->getAnm());
        mpMorf->changeAnm(anm);
    }

    return 1;
}

int daMidna_c::baseModelCallBack(int i_jointNo) {
    if (i_jointNo == mJntNo) {
        changeFaceBck();
    }

    if (i_jointNo == JNT_WORLD_ROOT || i_jointNo == JNT_HAND_R) {
        changeUpperBck();
    } else if (i_jointNo == JNT_MOUTH) {
        changeFaceBck();
    } else if (i_jointNo != JNT_CHIN) {
        modelCallBack(i_jointNo);
    }

    return 1;
}

static int daMidna_modelCallBack(J3DJoint* i_joint, int param_2) {
    int jnt_no = i_joint->getJntNo();
    daMidna_c* _this = (daMidna_c*)j3dSys.getModel()->getUserArea();
    if (param_2 == 0) {
        _this->baseModelCallBack(jnt_no);
    }
    return 1;
}

int daMidna_c::initInvModel(u16 i_resNo, J3DModel** i_modelP, mDoExt_invisibleModel* i_invModel, u32 flag) {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, i_resNo);
    JUT_ASSERT(664, modelData);
    *i_modelP = mDoExt_J3DModel__create(modelData, 0, flag);
    if (*i_modelP == NULL) {
        return 0;
    }

    if (!i_invModel->create(*i_modelP, 1)) {
        return 0;
    }

    return 1;
}

int daMidna_c::initDemoModel(J3DModel** i_modelP, char const* i_bmdName, u32 flag) {
    *i_modelP = NULL;
    J3DModelData* model_data =
        (J3DModelData*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), i_bmdName);
    if (model_data != NULL) {
        *i_modelP = mDoExt_J3DModel__create(model_data, 0x80000, flag | 0x11000084);
        if (*i_modelP == NULL) {
            return 0;
        }
    }
    return 1;
}

int daMidna_c::createHeap() {
    J3DModelData* modelData;
    
    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            mBckHeap[i].setBufferSize(0x1000);
        } else {
            mBckHeap[i].setBufferSize(0x3800);
        }

        if (!mBckHeap[i].mallocBuffer()) {
            return 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        mpEyeMatAnm[i] = new daMidna_matAnm_c();
        if (mpEyeMatAnm[i] == NULL) {
            return 0;
        }
    }

    if (dStage_roomControl_c::getDemoArcName()[0] != '\0') {
        if (!initDemoModel(&mpDemoHLTmpBmd, "demo00_Midna_cut00_HL_tmp.bmd", 0)) {
            return 0;
        }

        if (mpDemoHLTmpBmd != NULL && !daAlink_c::initDemoBck(&mpDemoHLTmpBck, "demo00_Midna_cut00_HL_tmp.bck")) {
            return 0;
        }

        if (!initDemoModel(&mpDemoHRTmpBmd, "demo00_Midna_cut00_HR_tmp.bmd", 0)) {
            return 0;
        }

        if (mpDemoHRTmpBmd != NULL && !daAlink_c::initDemoBck(&mpDemoHRTmpBck, "demo00_Midna_cut00_HR_tmp.bck")) {
            return 0;
        }

        modelData =
            (J3DModelData*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), "demo00_Midna_cut00_HD_tmp.bmd");
        if (modelData != NULL) {
            mpDemoHDTmpBmd = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
            if (mpDemoHDTmpBmd == NULL) {
                return 0;
            }

            if (!daAlink_c::initDemoBck(&mpDemoHDTmpBck, "demo00_Midna_cut00_HD_tmp.bck")) {
                return 0;
            }
        }

        if (!initDemoModel(&mpDemoFCBlendBmd, "demo00_Midna_cut00_FC_blend.bmd", 0x1000000)) {
            return 0;
        }

        if (!initDemoModel(&mpDemoFCTongueBmd, "demo00_Midna_cut00_FC_tongue.bmd", 0x200)) {
            return 0;
        }

        if (mpDemoFCTongueBmd != NULL && !daAlink_c::initDemoBck(&mpDemoFCTmpBck, "demo00_Midna_cut00_FC_tmp.bck")) {
            return 0;
        }

        modelData =
            (J3DModelData*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), "demo00_Midna_cut00_BD_tmp.bmd");
        if (modelData != NULL) {
            mpDemoBDTmpBmd = mDoExt_J3DModel__create(modelData, 0, 0x11020284);
            if (mpDemoBDTmpBmd == NULL) {
                return 0;
            }

            modelData->getMaterialNodePointer(2)->setMaterialAnm(mpEyeMatAnm[0]);
            modelData->getMaterialNodePointer(3)->setMaterialAnm(mpEyeMatAnm[1]);
        }

        if (!initDemoModel(&mpDemoBDMaskBmd, "demo00_Midna_cut00_BD_mask.bmd", 0)) {
            return 0;
        }

        if (!initDemoModel(&mpDemoBDHandsBmd, "demo00_Midna_cut00_BD_hands.bmd", 0)) {
            return 0;
        }

        if (mpDemoBDHandsBmd != NULL) {
            modelData = mpDemoBDHandsBmd->getModelData();
            for (u16 i = 0; i < 4; i++) {
                modelData->getMaterialNodePointer(i)->getShape()->hide();
            }
        }

        modelData =
            (J3DModelData*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), "demo00_Midna_cut00_BD_hairhand.bmd");
        if (modelData != NULL) {
            mpDemoBDHairhandBmd = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
            if (mpDemoBDHairhandBmd == NULL) {
                return 0;
            }
        }
    }

    JKRReadIdxResource(mBckHeap[0].getBuffer(), mBckHeap[0].getBufferSize(), 0x1DC, dComIfGp_getAnmArchive());
    J3DAnmTransform* md_anm = (J3DAnmTransform*)J3DAnmLoaderDataBase::load(mBckHeap[0].getBuffer());
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 14);
    JUT_ASSERT(852, modelData != NULL);
    mpMorf = new mDoExt_McaMorfSO(modelData, &mMorfCB, NULL, md_anm, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, NULL, 0, 0x11000284);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }
    
    mMorfCB.setScale(mHairScale);
    if (!mInvModel.create(mpMorf->getModel(), 1)) {
        return 0;
    }

    if (!mUpperBck.init(mpMorf->getAnm(), TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
        return 0;
    }

    if (!mFaceBck.init(mpMorf->getAnm(), TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
        return 0;
    }

    if (!mBtpHeap.mallocBuffer()) {
        return 0;
    }

    if (!mBtkHeap.mallocBuffer()) {
        return 0;
    }

    if (!initInvModel(8, &mpShadowMaskBmd, &mMaskInvModel, 0x11000284)) {
        return 0;
    }

    if (!initInvModel(7, &mpShadowHandsBmd, &mHandsInvModel, 0x11000284)) {
        return 0;
    }

    if (!initInvModel(15, &mpShadowHairhandBmd, &mHairhandInvModel, 0x11000284)) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 11);
    JUT_ASSERT(911, modelData);
    mpGokouBmd = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpGokouBmd == NULL) {
        return 0;
    }

    mpHIO = new NPC_MIDNA_HIO_CLASS();
    if (mpHIO == NULL) {
        return 0;
    }

    if (mpDemoFCBlendBmd != NULL) {
        mpDemoFCTmpBlk = new mDoExt_blkAnm();
        if (mpDemoFCTmpBlk == NULL) {
            return 0;
        }

        mpDemoFCTmpBls = (J3DDeformData*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), "demo00_Midna_cut00_FC_tmp.bls");
        if (mpDemoFCTmpBls != NULL) {
            s32 deformData = mpDemoFCBlendBmd->setDeformData(mpDemoFCTmpBls, 1);
            if (deformData) {
                return 0;
            }
        }
    }

    return 1;
}

static int daMidna_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daMidna_c*>(i_this)->createHeap();
}

#if DEBUG
daMidna_hio_c::daMidna_hio_c() {
    m = daMidna_hio_c0::m;
}
#endif

cPhs__Step daMidna_c::create() {
    fopAcM_ct(this, daMidna_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);

    if (step == cPhs_COMPLEATE_e) {
        if (dStage_roomControl_c::getDemoArcName()[0] != '\0' &&
                    dComIfG_syncObjectRes(dStage_roomControl_c::getDemoArcName()) > 0) {
            return cPhs_INIT_e;
        }

        u32 heap_size = 0x15b60;
        if (dStage_roomControl_c::getDemoArcName()[0] != '\0') {
            heap_size = 0x4d740;
            heap_size |= 0x80000000;
            OS_REPORT("MIDNA DEMO [%s]\n", dStage_roomControl_c::getDemoArcName());
        }

        if (!fopAcM_entrySolidHeap(this, daMidna_createHeap, heap_size)) {
            return cPhs_ERROR_e;
        }

        mpFunc = &daMidna_c::checkMetamorphoseEnableBase;
        
        for (u16 i = 0; i < 3; i++) {
            mBckHeap[i].createHeap(daPy_anmHeap_c::HEAP_TYPE_3);
        }
        
        mBtpHeap.createHeap(daPy_anmHeap_c::HEAP_TYPE_1);
        mBtkHeap.createHeap(daPy_anmHeap_c::HEAP_TYPE_2);
        
        mSound.init(&current.pos, &eyePos, 3, 1);
        mpShadowModel = mpMorf->getModel();
        mpShadowModel->setUserArea((uintptr_t)this);

        J3DModelData* modelData = mpShadowModel->getModelData();
        modelData->getJointNodePointer(JNT_WORLD_ROOT)->setCallBack(daMidna_modelCallBack);
        modelData->getJointNodePointer(JNT_HAND_R)->setCallBack(daMidna_modelCallBack);
        modelData->getJointNodePointer(JNT_HEAD)->setCallBack(daMidna_modelCallBack);
        modelData->getJointNodePointer(JNT_BACKBONE1)->setCallBack(daMidna_modelCallBack);
        modelData->getJointNodePointer(JNT_BACKBONE2)->setCallBack(daMidna_modelCallBack);
        modelData->getJointNodePointer(JNT_MOUTH)->setCallBack(daMidna_modelCallBack);
        modelData->getJointNodePointer(JNT_CHIN)->setCallBack(daMidna_modelCallBack);
        modelData->getJointNodePointer(JNT_WAIST)->setCallBack(daMidna_modelCallBack);
        for (u16 i = JNT_HAIR_1; i < JNT_HAIR_5 + 1; i++) {
            modelData->getJointNodePointer(i)->setCallBack(daMidna_modelCallBack);
        }

        mpShadowLeftHandShape = modelData->getMaterialNodePointer(1)->getShape();
        mpShadowRightHandShape = modelData->getMaterialNodePointer(2)->getShape();
        
        mpMoyaBtk = setSimpleBtk(modelData, 27);
        mpAppearBrk = setSimpleBrk(modelData, 18);
        modelData = mpShadowMaskBmd->getModelData();
        mpMaskMoyaBtk = setSimpleBtk(modelData, 26);
        mpMaskAppearBrk = setSimpleBrk(modelData, 21);
        modelData = mpShadowHandsBmd->getModelData();
        mpHandsMoyaBtk = setSimpleBtk(modelData, 25);
        for (u16 i = 0; i < 4; i++) {
            modelData->getMaterialNodePointer(i)->getShape()->hide();
        }
        modelData = mpShadowHairhandBmd->getModelData();
        mpHairhandMoyaBtk = setSimpleBtk(modelData, 24);
        mpHairhandAppearBrk = setSimpleBrk(modelData, 20);
        mpGokouAppearBrk = setSimpleBrk(mpGokouBmd->getModelData(), 19);
        mpGokouAppearBrk->setFrame(0.0f);

        #if DEBUG
        mpHIO->id = mDoHIO_CREATE_CHILD("ミドナ", mpHIO);
        #endif

        fopAcM_setStageLayer(this);
        fopAcM_SetMtx(this, mpShadowModel->getBaseTRMtx());
        mVoiceFrame = -1.0f;
        setMatrix();
        setRoomInfo();
        daPy_py_c::setMidnaActor(this);
        mpMorf->setMorf(1.0f);
        
        if (fopAcM_GetParam(this) == 1 && checkMidnaRealBody() && !checkMidnaTired()) {
            changeOriginalDemo();
            changeDemoMode(11);
        }
        
        setHairAngle();
        offStateFlg0(FLG0_UNK_8);
        setAnm();
        mpMorf->setMorf(0.0f);
        allAnimePlay();
        mpMorf->modelCalc();
        setBodyPartMatrix();
        
        field_0x6e0.r = l_normalColor.r;
        field_0x6e0.g = l_normalColor.g;
        field_0x6e0.b = l_normalColor.b;
        field_0x6e0.a = l_normalColor.a;
        if (dKy_darkworld_check()) {
            field_0x6e8.r = l_normalKColor.r;
            field_0x6e8.g = l_normalKColor.g;
            field_0x6e8.b = l_normalKColor.b;
            field_0x6e8.a = l_normalKColor.a;
            field_0x6ec.r = l_normalKColor2.r;
            field_0x6ec.g = l_normalKColor2.g;
            field_0x6ec.b = l_normalKColor2.b;
            field_0x6ec.a = l_normalKColor2.a;
        } else {
            field_0x6e8.r = l_lNormalKColor.r;
            field_0x6e8.g = l_lNormalKColor.g;
            field_0x6e8.b = l_lNormalKColor.b;
            field_0x6e8.a = l_lNormalKColor.a;
            field_0x6ec.r = l_lNormalKColor2.r;
            field_0x6ec.g = l_lNormalKColor2.g;
            field_0x6ec.b = l_lNormalKColor2.b;
            field_0x6ec.a = l_lNormalKColor2.a;
        }

        model = mpShadowModel;
        setMidnaNoDrawFlg();
    }

    return step;
}

static cPhs__Step daMidna_Create(fopAc_ac_c* i_this) {
    daMidna_c* actor = (daMidna_c*)i_this;
    fpc_ProcID id = fopAcM_GetID(i_this);
    return actor->create();
}

void daMidna_c::allAnimePlay() {
    daAlink_c::simpleAnmPlay(mpMoyaBtk);
    daAlink_c::simpleAnmPlay(mpHandsMoyaBtk);
    daAlink_c::simpleAnmPlay(mpMaskMoyaBtk);
    daAlink_c::simpleAnmPlay(mpHairhandMoyaBtk);
    daAlink_c::simpleAnmPlay(field_0x668);
    daAlink_c::simpleAnmPlay(mpDemoFCBlendBrk);

    mUpperBck.play();
    if (mDemoMode == 0xc) {
        mUpperBck.setFrame(0.0f);
    }
    mUpperBck.getBckAnm()->setFrame(mUpperBck.getFrame());

    if (checkStateFlg0(FLG0_UNK_800000)) {
        if (dMsgObject_isMouthCheck()) {
            mFaceBckTimer = 20;
        } else {
            if (mFaceBckTimer != 0) {
                mFaceBckTimer--;
            }
        }
        if (mFaceBckTimer == 0 && mFaceBck.checkFrame(0.0f)) {
            mFaceBck.setFrame(0.0f);
            mFaceBck.getBckAnm()->setFrame(0.0f);
        } else {
            mFaceBck.play();
            mFaceBck.getBckAnm()->setFrame(mFaceBck.getFrame());
        }
    }

    if (mpModel != NULL && mpFaceBtp != NULL && mpFaceBtk != NULL) {
        s16 uvar6, uvar7;
        if (checkStateFlg0(FLG0_UNK_40)) {
            if (mFrameCounter != 0) {
                mFrameCounter++;
                if (mFrameCounter > mpFaceBtp->getFrameMax()) {
                    mFrameCounter = 0;
                }
            } else if (cM_rnd() < 0.012f) {
                mFrameCounter++;
            }
            uvar7 = uvar6 = mFrameCounter;
        } else if (checkStateFlg0(FLG0_UNK_800000)) {
            uvar7 = uvar6 = mFaceBck.getFrame();
        } else {
            uvar7 = uvar6 = mUpperBck.getFrame();
        }

        if (uvar6 > mpFaceBtp->getFrameMax()) {
            uvar6 = mpFaceBtp->getFrameMax();
        }

        if (uvar7 > mpFaceBtk->getFrameMax()) {
            uvar7 = mpFaceBtk->getFrameMax();
        }
        
        mpFaceBtp->setFrame(uvar6);
        mpFaceBtk->setFrame(uvar7);
    }

    mpMorf->play(0, mReverb);
    
    if (mDemoMode == 0xc) {
        mpMorf->setFrame(0.0f);
    }

    if (checkSetAnime(0, ANM_S_APPEAR) || checkSetAnime(0, ANM_S_APPEARBL)) {
        mpAppearBrk->setFrame(mpMorf->getFrame());
        mpMaskAppearBrk->setFrame(mpMorf->getFrame());
        mpHairhandAppearBrk->setFrame(mpMorf->getFrame());
        mpGokouAppearBrk->setFrame(mpMorf->getFrame());
    } else if (field_0x84e == 1 || field_0x84e == 2) {
        mpAppearBrk->setFrame(0.0f);
        mpMaskAppearBrk->setFrame(0.0f);
        mpHairhandAppearBrk->setFrame(0.0f);
        mpGokouAppearBrk->setFrame(0.0f);
    } else {
        mpAppearBrk->setFrame(mpAppearBrk->getFrameMax() - 0.001f);
        mpMaskAppearBrk->setFrame(mpMaskAppearBrk->getFrameMax() - 0.001f);
        mpHairhandAppearBrk->setFrame(mpHairhandAppearBrk->getFrameMax() - 0.001f);

        if (checkSetAnime(0, ANM_S_RETURN)) {
            mpGokouAppearBrk->setFrame(mpMorf->getEndFrame() - mpMorf->getFrame());
        } else if (checkSetAnime(0, ANM_RETURN)) {
            f32 fvar8 = mpGokouAppearBrk->getFrame() - 2.0f;
            if (fvar8 < 0.0f) {
                fvar8 = 0.0f;
            }

            mpGokouAppearBrk->setFrame(fvar8);
        } else {
            mpGokouAppearBrk->setFrame(mpGokouAppearBrk->getFrameMax() - 0.001f);
        }
    }

    if (mLeftHandShapeIdx == 0xfd && mpDemoHLTmpBck != NULL) {
        mpDemoHLTmpBck->play();
    }

    if (mRightHandShapeIdx == 0xfd && mpDemoHRTmpBck != NULL) {
        mpDemoHRTmpBck->play();
    }

    if (checkStateFlg1(FLG1_UNK_10) && mpDemoHDTmpBck != NULL) {
        mpDemoHDTmpBck->play();
    }

    if (checkStateFlg0(FLG0_UNK_10)) {
        if (mpDemoFCTmpBlk != NULL && mpDemoFCTmpBlk->getBlkAnm() != NULL) {
            daAlink_c::simpleAnmPlay(mpDemoFCTmpBlk->getBlkAnm());
        }

        if (mpDemoFCTongueBtk != NULL) {
            daAlink_c::simpleAnmPlay(mpDemoFCTongueBtk);
        }

        if (mpDemoFCTmpBck != NULL) {
            mpDemoFCTmpBck->play();
        }
    }
}

void daMidna_c::setMatrix() {
    Vec vec1 = {0.0f, mpHIO->m.y_pos, mpHIO->m.z_pos};
    Vec scale = {mpHIO->m.scale, mpHIO->m.scale, mpHIO->m.scale};

    if (mpModel != NULL) {
        mpModel->setBaseScale(scale);
    }
    mpShadowModel->setBaseScale(scale);

    daAlink_c* link = daAlink_getAlinkActorClass();

    if (mDemoMode == 0x200) {
        J3DTransformInfo transform;
        mpMorf->getTransform(1, &transform);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        Vec vec2 = {transform.mTranslate.x, 0.0f, transform.mTranslate.z};
        mDoMtx_stack_c::multVec(&vec2, &current.pos);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mpShadowModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else if (mpKago != NULL) {
        mDoMtx_multVecZero(mpKago->getMidnaLocaterMtx(), &current.pos);
        shape_angle.y = mpKago->shape_angle.y;
        mDoMtx_MtxToRot(mpKago->getMidnaLocaterMtx(), &shape_angle);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(0.0f, -98.0f, 17.0f);
        mpShadowModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else if (!checkStateFlg0(FLG0_WOLF_NO_POS) && (!daPy_py_c::checkNowWolf() || checkShadowModelDrawSmode())) {
        if (field_0x84e != 4) {
            f32 sin_link_y = cM_ssin(link->shape_angle.y);
            f32 cos_link_y = cM_scos(link->shape_angle.y);
            if (field_0x84e == 2) {
                current.pos = link->current.pos;
                shape_angle.y = link->shape_angle.y;
            } else if (checkStateFlg0(FLG0_UNK_80000)) {
                // FIXME: x-val is a fakematch. Subtraction + negation fixes instruction order,
                //  but debug no longer matches.
                current.pos.set(
                    link->current.pos.x + -30.0f * sin_link_y - -(65.0f * cos_link_y),
                    link->current.pos.y,
                    link->current.pos.z + -30.0f * cos_link_y - 65.0f * sin_link_y
                );
                shape_angle.y = link->shape_angle.y - cM_deg2s(50.0f);
                mPrevShapeAngleY = shape_angle.y;
            } else {
                current.pos.set(
                    link->current.pos.x + 90.0f * sin_link_y,
                    link->current.pos.y,
                    link->current.pos.z + 90.0f * cos_link_y
                );
                shape_angle.y = link->shape_angle.y + 0x8000;
                mPrevShapeAngleY = shape_angle.y;
            }

            if (!checkStateFlg0(FLG0_UNK_80000) && checkStateFlg0(FLG0_UNK_40000000)) {
                current.pos.x += 80.0f * sin_link_y;
                current.pos.z += 80.0f * cos_link_y;
            }

            if (link->checkHorseRide() || link->checkBoarRide()) {
                current.pos.y -= 100.0f;
            } else if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
                current.pos.y -= 150.0f;
            }

            if (mDemoMode == 2) {
                current.pos.x += field_0x8dc * sin_link_y;
                current.pos.z += field_0x8dc * cos_link_y;
            }
        }
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mpShadowModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        if (!checkStateFlg0(FLG0_WOLF_NO_POS) && mDemoMode != 9) {
            mDoMtx_stack_c::copy(link->getWolfMidnaMatrix());
            mDoMtx_stack_c::ZXYrotM(-0x4000, -0x4000, 0);
            mDoMtx_stack_c::transM(-1.6f, -1.56f, -6.6f);

            if (checkSetAnime(0, ANM_WLSWIMDIE) && !checkStateFlg0(FLG0_NO_DRAW)) {
                mDoMtx_stack_c::multVecZero(&current.pos);
            } else {
                mDoMtx_stack_c::multVec(&vec1, &current.pos);
            }

            shape_angle = link->shape_angle;
            mDoMtx_stack_c::get()[0][3] = current.pos.x;
            mDoMtx_stack_c::get()[1][3] = current.pos.y;
            mDoMtx_stack_c::get()[2][3] = current.pos.z;
        } else {
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(shape_angle.y);
            if (checkSetAnime(0, ANM_WARPIN) && checkStateFlg1(FLG1_SIDE_WARP)) {
                f32 fvar7 = (mpMorf->getFrame() - 24.0f) * 0.05f;
                if (fvar7 < 0.0f) {
                    fvar7 = 0.0f;
                } else if (fvar7 > 1.0f) {
                    fvar7 = 1.0f;
                }

                mDoMtx_stack_c::transM(0.0f, 150.0f, 0.0f);
                mDoMtx_stack_c::XrotM(fvar7 * 16384.0f);
                mDoMtx_stack_c::transM(0.0f, -150.0f, 0.0f);
            }
        }

        mpShadowModel->setBaseTRMtx(mDoMtx_stack_c::get());
        speedF = link->speedF;
        current.angle = shape_angle;
        s16 target_backbone_angle;

        if (checkStateFlg0(FLG0_UNK_4)) {
            target_backbone_angle = -(shape_angle.x >> 1);
        } else {
            target_backbone_angle = 0;
        }

        cLib_addCalcAngleS(&mBackboneAngleZ, target_backbone_angle, 3, 0x800, 0x100);
    }
}

void daMidna_c::setBodyPartMatrix() {
    if (mpHandsBmd != NULL) {
        mpHandsBmd->setBaseTRMtx(mpShadowModel->getBaseTRMtx());
        mpHandsBmd->calc();
        mpHandsBmd->setAnmMtx(1, mpShadowModel->getAnmMtx(JNT_HAND_L));
        mpHandsBmd->setAnmMtx(2, mpShadowModel->getAnmMtx(JNT_HAND_R));
    }

    mpShadowHandsBmd->setBaseTRMtx(mpShadowModel->getBaseTRMtx());
    mpShadowHandsBmd->calc();
    mpShadowHandsBmd->setAnmMtx(1, mpShadowModel->getAnmMtx(JNT_HAND_L));
    mpShadowHandsBmd->setAnmMtx(2, mpShadowModel->getAnmMtx(JNT_HAND_R));

    if (mpMaskBmd != NULL) {
        mpMaskBmd->setBaseTRMtx(mpShadowModel->getAnmMtx(JNT_HEAD));
        mpMaskBmd->calc();
    }

    mpShadowMaskBmd->setBaseTRMtx(mpShadowModel->getAnmMtx(JNT_HEAD));
    mpShadowMaskBmd->calc();

    mDoMtx_stack_c::copy(mpShadowModel->getAnmMtx(JNT_HAIR_5));
    mDoMtx_stack_c::transM(6.5f, 0.0f, 0.0f);

    if (checkStateFlg1(FLG1_UNK_10)) {
        mpDemoHDTmpBmd->setBaseTRMtx(mDoMtx_stack_c::get());
        if (mpDemoHDTmpBck != NULL) {
            mpDemoHDTmpBck->entry(mpDemoHDTmpBmd->getModelData());
        }

        mpDemoHDTmpBmd->calc();
    }

    BOOL bvar8 = false;

    if (!checkStateFlg0((daMidna_FLG0)(FLG0_UNK_10000000 | FLG0_UNK_200000))) {
        cXyz vec1, vec2;
        mDoMtx_multVecZero(mpShadowModel->getAnmMtx(JNT_HAIR_1), &vec1);
        mDoMtx_multVecZero(mpShadowModel->getAnmMtx(JNT_HAIR_5), &vec2);
        vec2 -= vec1;
        mDoMtx_stack_c::XYZrotM(0, mHairAngleY[4], mHairAngleZ[4] - vec2.atan2sY_XZ());
    } else if (daAlink_getAlinkActorClass()->getWolfLockActorEnd() != NULL) {
        cXyz vec;
        mDoMtx_stack_c::multVecZero(&vec);
        vec = daAlink_getAlinkActorClass()->getWolfLockActorEnd()->eyePos - vec;
        mDoMtx_stack_c::XrotM(vec.atan2sX_Z() - shape_angle.y);
        bvar8 = true;
    }

    if (mDemoMode == 0x13 || mDemoMode == 0x14 || mDemoMode == 0x15) {
        bvar8 = true;
    }

    mpShadowHairhandBmd->setBaseTRMtx(mDoMtx_stack_c::get());
    mpShadowHairhandBmd->calc();
    J3DModelData* modelData = mpShadowHairhandBmd->getModelData();
    for (u16 i = 0; i < 3; i++) {
        modelData->getMaterialNodePointer(i)->getShape()->hide();
    }

    BOOL bvar2;
    if (
        checkStateFlg0(FLG0_UNK_10000000) || mBckHeap[2].getIdx() == m_anmDataTable[ANM_HAIR].mResID ||
        mBckHeap[2].getIdx() == m_anmDataTable[ANM_S_TAKES].mResID || mBckHeap[2].getIdx() == m_anmDataTable[ANM_S_WAITS].mResID ||
        mBckHeap[2].getIdx() == m_anmDataTable[ANM_S_PACKAWAY].mResID || mBckHeap[2].getIdx() == m_anmDataTable[ANM_GRABST].mResID ||
        checkEndResetStateFlg0(ERFLG0_UNK_40) || dComIfGp_checkPlayerStatus1(0, 0x800000)
    ) {
        if (bvar8) {
            modelData->getMaterialNodePointer(2)->getShape()->show();
        } else {
            modelData->getMaterialNodePointer(1)->getShape()->show();
        }

        bvar2 = true;
    } else {
        modelData->getMaterialNodePointer(0)->getShape()->show();
        bvar2 = false;
    }

    if (mpHairhandBmd != NULL) {
        mpHairhandBmd->setBaseTRMtx(mDoMtx_stack_c::get());
        mpHairhandBmd->calc();

        modelData = mpHairhandBmd->getModelData();
        for (u16 i = 0; i < 3; i++) {
            modelData->getMaterialNodePointer(i)->getShape()->hide();
        }

        const GXColorS10* color;
        const GXColor* kcolor1;
        const GXColor* kcolor2;
        
        if (bvar2) {
            if (bvar8) {
                modelData->getMaterialNodePointer(2)->getShape()->show();
            } else {
                modelData->getMaterialNodePointer(1)->getShape()->show();
            }

            kcolor1 = &l_bigKColor;
            if (dKy_darkworld_check()) {
                color = &l_bigColor;
                kcolor2 = &l_normalKColor2;
            } else {
                color = &l_lBigColor;
                kcolor2 = &l_lBigKColor2;
            }
        } else {
            modelData->getMaterialNodePointer(0)->getShape()->show();

            color = &l_normalColor;
            if (dKy_darkworld_check()) {
                kcolor1 = &l_normalKColor;
                kcolor2 = &l_normalKColor2;
            } else {
                kcolor1 = &l_lNormalKColor;
                kcolor2 = &l_lNormalKColor2;
            }
        }

        cLib_chaseS(&field_0x6e0.r, color->r, 10);
        cLib_chaseS(&field_0x6e0.g, color->g, 10);
        cLib_chaseS(&field_0x6e0.b, color->b, 10);
        cLib_chaseUC(&field_0x6e8.r, kcolor1->r, 10);
        cLib_chaseUC(&field_0x6e8.g, kcolor1->g, 10);
        cLib_chaseUC(&field_0x6e8.b, kcolor1->b, 10);
        cLib_chaseUC(&field_0x6ec.r, kcolor2->r, 10);
        cLib_chaseUC(&field_0x6ec.g, kcolor2->g, 10);
        cLib_chaseUC(&field_0x6ec.b, kcolor2->b, 10);
    }

    if (mpModel != NULL) {
        mpModel->setBaseTRMtx(mpShadowModel->getBaseTRMtx());
        for (int i = 0; i < JNT_MAX; i++) {
            mpModel->setAnmMtx(i, mpShadowModel->getAnmMtx(i));
        }
        mpModel->calcWeightEnvelopeMtx();
    }

    mDoMtx_stack_c::transS(mpShadowModel->getAnmMtx(JNT_BACKBONE1)[0][3],
                           mpShadowModel->getAnmMtx(JNT_BACKBONE1)[1][3] + 20.0f,
                           mpShadowModel->getAnmMtx(JNT_BACKBONE1)[2][3]);
    mpGokouBmd->setBaseTRMtx(mDoMtx_stack_c::get());
    mpGokouBmd->calc();

    if (mLeftHandShapeIdx == 0xfd) {
        mpDemoHLTmpBmd->setBaseTRMtx(mpShadowModel->getAnmMtx(JNT_HAND_L));
        if (mpDemoHLTmpBck != NULL) {
            mpDemoHLTmpBck->entry(mpDemoHLTmpBmd->getModelData());
        }
        mpDemoHLTmpBmd->calc();

        mpShadowLeftHandShape->hide();
        if (mpLeftHandShape != NULL) {
            mpLeftHandShape->hide();
        }
    } else {
        mpShadowLeftHandShape->show();
        if (mpLeftHandShape != NULL) {
            mpLeftHandShape->show();
        }
    }

    if (mRightHandShapeIdx == 0xfd) {
        mpDemoHRTmpBmd->setBaseTRMtx(mpShadowModel->getAnmMtx(JNT_HAND_R));
        if (mpDemoHRTmpBck != NULL) {
            mpDemoHRTmpBck->entry(mpDemoHRTmpBmd->getModelData());
        }
        
        mpDemoHRTmpBmd->calc();
        mpShadowRightHandShape->hide();
        if (mpRightHandShape != NULL) {
            mpRightHandShape->hide();
        }
    } else {
        mpShadowRightHandShape->show();
        if (mpRightHandShape != NULL) {
            mpRightHandShape->show();
        }
    }

    if (checkStateFlg0(FLG0_UNK_10)) {
        mpDemoFCBlendBmd->setBaseTRMtx(mpShadowModel->getAnmMtx(JNT_HEAD));
        mpDemoFCBlendBmd->calc();

        mpDemoFCTongueBmd->setBaseTRMtx(mpShadowModel->getAnmMtx(JNT_HEAD));
        if (mpDemoFCTmpBck != NULL) {
            mpDemoFCTmpBck->entry(mpDemoFCTongueBmd->getModelData());
        }
        mpDemoFCTongueBmd->calc();
    }
}

void daMidna_c::setRoomInfo() {
    int room_no;
    if (fopAcM_gc_c::gndCheck(&current.pos)) {
        room_no = fopAcM_gc_c::getRoomId();
        tevStr.YukaCol = fopAcM_gc_c::getPolyColor();
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
    }
    tevStr.room_no = room_no;
    mReverb = dComIfGp_getReverb(room_no);
    fopAcM_SetRoomNo(this, room_no);
}

void daMidna_c::setBodyPartPos() {
    if (!(checkSetAnime(0, ANM_S_RETURN) && mpMorf->getFrame() > 14.0f)) {
        mDoMtx_multVec(mpShadowModel->getAnmMtx(JNT_HEAD), &l_eyeOffset, &eyePos);
    }

    if (field_0x84e == 1 || (checkSetAnime(0, ANM_S_APPEAR) && mpMorf->getFrame() < 3.0f)) {
        daAlink_c* link = daAlink_getAlinkActorClass();
        f32 fvar1 = daPy_py_c::checkNowWolf() ? 250.0f : 50.0f;
        attention_info.position.set(
            link->current.pos.x + fvar1 * cM_ssin(link->shape_angle.y),
            link->current.pos.y - 30.0f,
            link->current.pos.z + fvar1 * cM_scos(link->shape_angle.y)
        );
        eyePos = attention_info.position;
    } else if (field_0x84e == 2) {
        attention_info.position = eyePos;
    } else {
        attention_info.position.set(current.pos.x, current.pos.y + 120.0f, current.pos.z);
    }

    mDoMtx_multVec(mpShadowModel->getAnmMtx(JNT_HEAD), &l_headCenterOffset, &mHeadCenterPos);
    if (!checkStateFlg0(FLG0_NO_DRAW)) {
        cXyz item_pos;
        static Vec const localItemPos = { 0.0f, -68.0f, 0.0f };
        mDoMtx_multVec(mpShadowModel->getAnmMtx(JNT_HEAD), &localItemPos, &item_pos);
        daAlink_getAlinkActorClass()->setItemPos(&item_pos);
    }
}

BOOL daMidna_c::checkAppear() {
    if (
        daPy_py_c::checkNowWolf() && daPy_py_c::checkFirstMidnaDemo() &&
        (
            !daAlink_getAlinkActorClass()->checkMidnaDisappearMode() ||
            checkStateFlg0(FLG0_WOLF_NO_POS) ||
            (checkSetAnime(0, ANM_RETURN) && !mpMorf->isStop())
        )
    ) {
        return TRUE;
    }
    return FALSE;
}

void daMidna_c::checkMidnaPosState() {
    daAlink_c* link = daAlink_getAlinkActorClass();

    const cXyz* jump_point = link->checkMidnaLockJumpPoint();
    if (checkStateFlg1(FLG1_UNK_200) || checkSetAnime(0, ANM_LEADTOWAITA)) {
        jump_point = NULL;
    }

    offStateFlg1(FLG1_UNK_200);
    const cXyz* pos = NULL;
    if (jump_point == NULL) {
        offStateFlg1(FLG1_UNK_400);
        field_0x850 = 0;
    }

    if (mDemoMode == 0x200) {
        dDemo_actor_c* demo_actor = dDemo_c::getActor(demoActorID);
        if (demo_actor != NULL) {
            int tmp1, tmp2, tmp3;
            u16 tmp4;
            while (demo_actor->getDemoIDData(&tmp1, &tmp2, &tmp3, &tmp4, NULL)) {
                if (tmp1 == 0 && tmp2 == 0 && tmp3 == 1) {
                    if (tmp4 != 0) {
                        offStateFlg0(FLG0_WOLF_NO_POS);
                    } else {
                        onStateFlg0(FLG0_WOLF_NO_POS);
                    }
                }
            }
        }

        offStateFlg0(FLG0_UNK_20);
        return;
    }
    
    if (checkStateFlg0(FLG0_TAG_WAIT)) {
        if (!checkEndResetStateFlg0(ERFLG0_UNK_2)) {
            onStateFlg0(FLG0_NO_INPUT);
        }
        onStateFlg0(FLG0_WOLF_NO_POS);
        pos = &mTagWaitPos;
        field_0x8d4 = 30.0f;
        offStateFlg0(FLG0_UNK_20);
    } else if (link->checkMidnaWolfDeadAnime()) {
        offStateFlg0((daMidna_FLG0)(FLG0_UNK_1000 | FLG0_UNK_200 | FLG0_UNK_20));
        if (checkSetAnime(0, ANM_WLDIEA) && !checkStateFlg0(FLG0_NO_DRAW)
                                         && mpMorf->getFrame() >= 72.0f) {
            onStateFlg0(FLG0_WOLF_NO_POS);
        } else {
            offStateFlg0(FLG0_WOLF_NO_POS);
        }
    } else if (link->checkMidnaWolfSwimDeadAnime()) {
        offStateFlg0((daMidna_FLG0)(FLG0_UNK_1000 | FLG0_UNK_200 | FLG0_UNK_20));
        if (checkSetAnime(0, ANM_WLSWIMDIE) && !checkStateFlg0(FLG0_NO_DRAW)
                                         && mpMorf->getFrame() >= 2.0f) {
            onStateFlg0(FLG0_WOLF_NO_POS);
        } else {
            offStateFlg0(FLG0_WOLF_NO_POS);
        }
    } else if (jump_point != NULL) {
        if (
            checkStateFlg0(FLG0_UNK_100) && !link->checkWolfTagLockJump() && field_0x9a0.abs2(*jump_point) > 100.0f
        ) {
            offStateFlg0((daMidna_FLG0)(FLG0_WOLF_NO_POS | FLG0_UNK_200 | FLG0_UNK_100));
            field_0x850 = 0;
            field_0x851 = 0;
            jump_point = pos;
        } else {
            onStateFlg0(FLG0_WOLF_NO_POS);
            if (
                !checkStateFlg0(FLG0_UNK_100) || (field_0x9a0.abs2(*jump_point) > 100.0f &&
                link->current.pos.abs2(field_0x9a0) < 40000.0f)
            ) {
                field_0x9a0 = *jump_point;
                field_0x8d4 = jump_point->abs(current.pos) / link->getWolfTagJumpTime();
                if (field_0x8d4 < 10.0f) {
                    field_0x8d4 = 10.0f;
                }
                onStateFlg0(FLG0_UNK_100);
                offStateFlg0(FLG0_UNK_200);
                pos = jump_point;
                mSound.startCreatureSound(Z2SE_MIDNA_JUMP, field_0x850++, -1);
                field_0x851 = field_0x850;
            } else {
                pos = &field_0x9a0;
            }

            if (!link->checkWolfTagLockJump() && !checkStateFlg1(FLG1_UNK_400)
                            && (!checkStateFlg0(FLG0_UNK_200) || checkSetAnime(0, ANM_LEADED))) {
                onStateFlg0(FLG0_NO_INPUT);    
            } else {
                onStateFlg1(FLG1_UNK_400);
            }
        }
        offStateFlg0(FLG0_UNK_20);
    } else if (link->checkWolfTagLockJump() || checkStateFlg0(FLG0_UNK_100)) {
        onStateFlg0(FLG0_WOLF_NO_POS);
        if (!link->checkWolfTagLockJump()) {
            onStateFlg1(FLG1_UNK_200);
        } else {
            onStateFlg0(FLG0_UNK_20);
        }
        offStateFlg0(FLG0_UNK_100);
    } else if (checkStateFlg0(FLG0_WOLF_NO_POS)) {
        offStateFlg0((daMidna_FLG0)(FLG0_WOLF_NO_POS | FLG0_UNK_200));
        onStateFlg0(FLG0_UNK_1000);
        if (checkStateFlg0(FLG0_UNK_20)) {
            offStateFlg0(FLG0_UNK_20);
            mSound.startCreatureSound(Z2SE_MIDNA_JUMP_FINISH, field_0x851, -1);
        }
    } else {
        offStateFlg0(FLG0_UNK_20);
    }

    if (mDemoMode == 12) {
        if (daPy_py_c::checkNowWolf()) {
            onStateFlg0(FLG0_WOLF_NO_POS);
            Vec vec1 = {0.0f, mpHIO->m.y_pos, mpHIO->m.z_pos};
            cXyz vec2;
            mDoMtx_stack_c::copy(link->getWolfMidnaMatrix());
            mDoMtx_stack_c::ZXYrotM(-0x4000, -0x4000, 0);
            mDoMtx_stack_c::multVec(&vec1, &vec2);
            if (cLib_chasePos(&current.pos, vec2, 50.0f)) {
                dComIfGp_evmng_cutEnd(mStaffID);
            }
        } else {
            offStateFlg0(FLG0_WOLF_NO_POS);
            dComIfGp_evmng_cutEnd(mStaffID);
        }

        offStateFlg0(FLG0_UNK_400000);
    } else if (mDemoMode == 13) {
        if (mpMorf->isStop()) {
            dComIfGp_evmng_cutEnd(mStaffID);
        }

        offStateFlg0(FLG0_UNK_400000);
    } else if (mDemoMode == 17 || mDemoMode == 18) {
        if (mpMorf->isStop()) {
            dComIfGp_evmng_cutEnd(mStaffID);
        }
    } else if (
        mDemoMode == 3 || mDemoMode == 6 || mDemoMode == 4 || mDemoMode == 19 ||
        mDemoMode == 20 || mDemoMode == 21 || mDemoMode == 7 || mDemoMode == 14 ||
        mDemoMode == 5 || mDemoMode == 16 || mDemoMode == 11 || mDemoMode == 8
    ) {
        onStateFlg0(FLG0_WOLF_NO_POS);
        offStateFlg0((daMidna_FLG0)(FLG0_UNK_400000 | FLG0_UNK_1000));
        if (mDemoMode == 16) {
            current.pos = mDemoPos;
            shape_angle.y = mDemoAngle;
            current.angle.y = shape_angle.y;
            dComIfGp_evmng_cutEnd(mStaffID);
        } else if (mDemoMode == 3 || mDemoMode == 6) {
            if (cLib_addCalcPos(&current.pos, mDemoPos, 0.5f, mDemoSpeed, 1.0f) < 1.0f) {
                dComIfGp_evmng_cutEnd(mStaffID);
            } else {
                cXyz vec = mDemoPos - current.pos;
                cLib_addCalcAngleS(&shape_angle.y, vec.atan2sX_Z(), 2, 0x1000, 0x400);
                current.angle.y = shape_angle.y;
                onEndResetStateFlg0(ERFLG0_UNK_1);
            }
        } else if (mDemoMode == 4) {
            if (cLib_addCalcAngleS(&shape_angle.y, mDemoAngle, 2, 0x1000, 0x400) == 0) {
                dComIfGp_evmng_cutEnd(mStaffID);
            }
            current.angle.y = shape_angle.y;
        } else if (mDemoMode == 8) {
            if (checkSetAnime(0, ANM_S_HOLE)) {
                cLib_addCalcAngleS(&shape_angle.y, mDemoAngle, 2, 0x2000, 0x800);
                current.angle.y = shape_angle.y;
                if (mpMorf->getFrame() >= 26.0f) {
                    setRightHandShape(3);
                } else if (mpMorf->getFrame() >= 13.0f) {
                    setRightHandShape(2);
                }

                if (mpMorf->isStop()) {
                    dComIfGp_evmng_cutEnd(mStaffID);
                }
            }
        } else if (mDemoMode == 7) {
            if (checkSetAnime(2, ANM_S_TAKES) && mFaceBck.isStop()) {
                dComIfGp_evmng_cutEnd(mStaffID);
            }
        } else if (mDemoMode == 14) {
            if (checkSetAnime(2, ANM_S_PACKAWAY) && mFaceBck.isStop()) {
                dComIfGp_evmng_cutEnd(mStaffID);
            }
        } else if (mDemoMode == 5) {
            if ((checkSetAnime(0, ANM_S_RETURN) || checkSetAnime(0, ANM_RETURN)) && mpMorf->isStop())
            {
                offStateFlg0((daMidna_FLG0)(FLG0_WOLF_NO_POS | FLG0_TAG_WAIT));
                dComIfGp_evmng_cutEnd(mStaffID);
            }
        } else if (mDemoMode == 19 || mDemoMode == 21 || mDemoMode == 20) {
            if (dComIfGp_event_getPt2()) {
                s16 angle = fopAcM_searchActorAngleY(this, dComIfGp_event_getPt2());
                cLib_addCalcAngleS(&shape_angle.y, angle, 2, 0x2000, 0x800);
                current.angle.y = shape_angle.y;
            }

            if (mDemoMode == 20 || (mDemoMode == 21 && mpMorf->isStop())
                                || (mDemoMode == 19 && mFaceBck.isStop())) {
                dComIfGp_evmng_cutEnd(mStaffID);
            }
        }
    } else if (pos != NULL) {
        if (checkEndResetStateFlg0(ERFLG0_UNK_2)) {
            onStateFlg0(FLG0_UNK_200);
            current.pos = *pos;
            shape_angle.y = fopAcM_searchPlayerAngleY(this);
            current.angle.y = shape_angle.y;
            mPrevShapeAngleY = shape_angle.y;
            onEndResetStateFlg0(ERFLG0_UNK_100);
        } else if (cLib_chasePos(&current.pos, *pos, field_0x8d4)) {
            if (!checkStateFlg0(FLG0_UNK_200)) {
                mPlayerAngleY = fopAcM_searchPlayerAngleY(this);
            }
            
            onStateFlg0(FLG0_UNK_200);
        } else {
            cXyz start(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            cXyz end = current.pos * 2.0f - old.pos;
            end.y += 100.0f;
            mLinChk.Set(&start, &end, this);
            if (dComIfG_Bgsp().LineCross(&mLinChk)) {
                onStateFlg0(FLG0_UNK_400000);
            }
        }

        s16 angle = shape_angle.y;
        if (checkSetAnime(0, ANM_LEADED)) {
            if (mpMorf->getFrame() >= 4.0f && mpMorf->getFrame() < 17.0f) {
                if ((s16)(mPlayerAngleY - shape_angle.y) < 0) {
                    angle = shape_angle.y + 0x7FFF;
                } else {
                    angle = mPlayerAngleY;
                }
            }
        } else if (checkSetAnime(0, ANM_LEADST)) {
            if (!checkStateFlg0(FLG0_UNK_200)) {
                angle = cM_atan2s(pos->x - current.pos.x, pos->z - current.pos.z);
            }
        } else {
            angle = fopAcM_searchPlayerAngleY(this);
            onEndResetStateFlg0(ERFLG0_UNK_100);
        }
        cLib_addCalcAngleS(&shape_angle.y, angle, 2, 0x2000, 0x800);
    } else {
        offStateFlg0(FLG0_UNK_400000);
    }
}

BOOL daMidna_c::setUpperAnime(u16 i_resID, u16 param_1) {
    J3DAnmTransform* bck;
    if (param_1 == 0xffff) {
        bck = (J3DAnmTransform*)mBckHeap[1].loadDataIdx(i_resID);
    } else {
        bck = (J3DAnmTransform*)mBckHeap[1].loadDataDemoRID(i_resID, 0);
    }

    if (bck == NULL) {
        return FALSE;
    }

    mUpperBck.init(bck, TRUE, -1, 1.0f, 0, -1, true);
    mSoundID = 0;
    mVoiceFrame = -1.0f;
    return TRUE;
}

BOOL daMidna_c::setUpperAnimeAndSe(daMidna_c::daMidna_ANM i_anm) {
    const daMidna_anmData_s* anm_data = &m_anmDataTable[i_anm];
    if (setUpperAnime(anm_data->mResID, 0xffff)) {
        mSoundID = anm_data->mSoundID;
        mVoiceFrame = anm_data->mSoundFrame;
        return TRUE;
    }

    return FALSE;
}

BOOL daMidna_c::setFaceAnime(u16 i_resID, u16 param_1) {
    J3DAnmTransform* bck;
    if (param_1 == 0xFFFF) {
        bck = (J3DAnmTransform*)mBckHeap[2].loadDataIdx(i_resID);
    } else {
        bck = (J3DAnmTransform*)mBckHeap[2].loadDataDemoRID(i_resID, 0);
    }

    onStateFlg0(FLG0_UNK_800000);

    if (bck == NULL) {
        return FALSE;
    }

    mFaceBck.init(bck, TRUE, -1, 1.0f, 0, -1, true);
    mFaceBckTimer = 20;
    return TRUE;
}

void daMidna_c::endHighModel() {
    if (mpDemoFCTongueBtk != NULL) {
        mpDemoFCTongueBmd->getModelData()->removeTexMtxAnimator(mpDemoFCTongueBtk);
    }

    offStateFlg0(FLG0_UNK_10);
    mpDemoFCTmpCluster = NULL;
    mpDemoFCTongueBtk = NULL;

    if (mpModel != NULL) {
        mpModel->getModelData()->getMaterialNodePointer(9)->getShape()->show();
        mpModel->getModelData()->getMaterialNodePointer(1)->getShape()->show();
        mpModel->getModelData()->getMaterialNodePointer(2)->getShape()->show();
        mpModel->getModelData()->getMaterialNodePointer(3)->getShape()->show();
        mpModel->getModelData()->getMaterialNodePointer(8)->getShape()->show();
    }
}

BOOL daMidna_c::setDemoAnm() {
    dDemo_actor_c* demo_actor = dDemo_c::getActor(demoActorID);
    if (demo_actor == NULL) {
        return FALSE;
    }

    BOOL ret = FALSE;
    BOOL bVar1 = FALSE;
    int var1, var2, var3;
    u16 var4;

    while (demo_actor->getDemoIDData(&var1, &var2, &var3, &var4, NULL)) {
        if (var1 == 0) {
            if (var2 == 0) {
                if (var3 == 2) {
                    offStateFlg1((daMidna_FLG1)(FLG1_UNK_20 | FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE | FLG1_UNK_1));
                    if (var4 == 3) {
                        onStateFlg1(FLG1_UNK_20);
                    } else if (var4 == 2) {
                        onStateFlg1(FLG1_UNK_1);
                    } else if (var4 == 1) {
                        field_0x84e = 3;
                        onStateFlg1(FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE);
                    } else {
                        field_0x84e = 0;
                    }
                } else if (var3 == 3) {
                    if (var4 == 2) {
                        onStateFlg1(FLG1_FORCE_NORMAL_COL);
                        offStateFlg1(FLG1_FORCE_TIRED_COL);
                    } else if (var4 == 1) {
                        offStateFlg1(FLG1_FORCE_NORMAL_COL);
                        onStateFlg1(FLG1_FORCE_TIRED_COL);
                    } else {
                        offStateFlg1((daMidna_FLG1)(FLG1_FORCE_NORMAL_COL | FLG1_FORCE_TIRED_COL));
                    }
                } else if (var3 == 4) {
                    if (var4 == 1) {
                        onEndResetStateFlg0(ERFLG0_UNK_20);
                    }
                } else if (var3 == 5) {
                    if (var4 == 1) {
                        onStateFlg1(FLG1_NO_MASK_DRAW);
                    } else {
                        offStateFlg1(FLG1_NO_MASK_DRAW);
                    }
                } else if (var3 == 6) {
                    if (var4 == 1) {
                        onStateFlg1(FLG1_SHADOW_NO_DRAW);
                    } else {
                        offStateFlg1(FLG1_SHADOW_NO_DRAW);
                    }
                }
            } else if (var2 == 5) {
                if (var4 < 2) {
                    var4 += 2;
                } else if (var4 == 0xfd && mpDemoHRTmpBmd == NULL) {
                    var4 = 0xfe;
                }
                setRightHandShape(var4);
            } else if (var2 == 6) {
                if (var4 == 0xfd && mpDemoHLTmpBmd == NULL) {
                    var4 = 0xfe;
                }
                setLeftHandShape(var4);
            } else if (var2 == 9) {
                if (var4 == 1 && mpDemoFCBlendBmd != NULL && mpDemoFCTongueBmd != NULL) {
                    onStateFlg0(FLG0_UNK_10);
                    bVar1 = TRUE;
                    if (mpModel != NULL) {
                        mpModel->getModelData()->getMaterialNodePointer(9)->getShape()->hide();
                        mpModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
                        mpModel->getModelData()->getMaterialNodePointer(2)->getShape()->hide();
                        mpModel->getModelData()->getMaterialNodePointer(3)->getShape()->hide();
                        mpModel->getModelData()->getMaterialNodePointer(8)->getShape()->hide();
                    }
                } else {
                    endHighModel();
                }
            } else if (var2 == 4) {
                if (var3 == 1) {
                    if (var4 == 1) {
                        onStateFlg0((daMidna_FLG0)(FLG0_NO_HAIR_SCALE | FLG0_UNK_200000));
                    }
                } else if (var4 == 2) {
                    onStateFlg1(FLG1_UNK_40);
                } else if (var4 != 0 && mpDemoHDTmpBmd != NULL) {
                    onStateFlg1(FLG1_UNK_10);
                } else {
                    offStateFlg1((daMidna_FLG1)(FLG1_UNK_40 | FLG1_UNK_10));
                }
            }
        } else if (var1 == 2) {
            if (var2 == 1) {
                if (var3 == 2) {
                    J3DAnmTransform* bck = static_cast<J3DAnmTransform*>(mBckHeap[0].loadDataDemoRID(var4, 0));
                    if (bck != NULL) {
                        f32 morf;
                        if (demo_actor->checkEnable(0x80)) {
                            morf = demo_actor->getPrm_Morf();
                        } else {
                            morf = 0.0f;
                        }
                        
                        setBckAnime(bck, -1, morf);
                        setUpperAnime(var4, 0);
                        offStateFlg0((daMidna_FLG0)(FLG0_UNK_1000 | FLG0_UNK_1));
                        demo_actor->setAnmFrameMax(mpMorf->getEndFrame());
                    }
                    ret = TRUE;
                }
            } else if (var2 == 6) {
                if (var3 == 2) {
                    if (mpDemoHLTmpBck != NULL) {
                        J3DAnmTransform* bck = static_cast<J3DAnmTransform*>(
                            dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                        if (bck != NULL && mpDemoHLTmpBck->getBckAnm() != bck) {
                            mpDemoHLTmpBck->init(bck, TRUE, -1, 1.0f, 0, -1, true);
                        }
                    }
                    ret = TRUE;
                }
            } else if (var2 == 5) {
                if (var3 == 2) {
                    if (mpDemoHRTmpBck != NULL) {
                        J3DAnmTransform* bck = static_cast<J3DAnmTransform*>(
                            dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                        if (bck != NULL && mpDemoHRTmpBck->getBckAnm() != bck) {
                            mpDemoHRTmpBck->init(bck, TRUE, -1, 1.0f, 0, -1, true);
                        }
                    }
                    ret = TRUE;
                }
            } else if (var2 == 9) {
                if (var3 == 5) {
                    setFaceBtp(var4, TRUE);
                    ret = TRUE;
                } else if (var3 == 3) {
                    if (checkStateFlg0(FLG0_UNK_10)) {
                        J3DAnmTextureSRTKey* btk = static_cast<J3DAnmTextureSRTKey*>(
                            dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                        if (btk != NULL && mpDemoFCTongueBtk != btk) {
                            btk->searchUpdateMaterialID(mpDemoFCTongueBmd->getModelData());
                            mpDemoFCTongueBmd->getModelData()->entryTexMtxAnimator(btk);
                            btk->setFrame(0.0f);
                            mpDemoFCTongueBtk = btk;
                        }
                    } else {
                        setFaceBtk(var4, TRUE);
                    }
                    ret = TRUE;
                } else if (var3 == 7) {
                    J3DAnmCluster* cluster = static_cast<J3DAnmCluster*>(
                        dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                    if (mpDemoFCTmpCluster != cluster) {
                        mpDemoFCTmpCluster = cluster;
                        mpDemoFCTmpBlk->init(mpDemoFCTmpBls, mpDemoFCTmpCluster, FALSE,
                                             J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
                        mpDemoFCTmpBlk->getBlkAnm()->setFrame(0.0f);
                    }
                    ret = TRUE;
                } else if (var3 == 2) {
                    if (mpDemoFCTmpBck != NULL) {
                        J3DAnmTransform* bck = static_cast<J3DAnmTransform*>(
                            dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                        if (bck != NULL && mpDemoFCTmpBck->getBckAnm() != bck) {
                            mpDemoFCTmpBck->init(bck, TRUE, -1, 1.0f, 0, -1, true);
                        }
                    }
                    ret = TRUE;
                } else if (var3 == 4) {
                    J3DAnmTevRegKey* brk = static_cast<J3DAnmTevRegKey*>(
                        dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                    if (brk != NULL && mpDemoFCBlendBrk != brk) {
                        brk->setFrame(0.0f);
                        mpDemoFCBlendBrk = brk;
                        if (mpDemoFCBlendBmd != NULL) {
                            mpDemoFCBlendBrk->searchUpdateMaterialID(mpDemoFCBlendBmd->getModelData());
                        }
                    }
                }
            } else if (var2 == 4) {
                if (var3 == 2) {
                    if (mpDemoHDTmpBck != NULL) {
                        J3DAnmTransform* bck = static_cast<J3DAnmTransform*>(
                            dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                        if (bck != NULL && mpDemoHDTmpBck->getBckAnm() != bck) {
                            mpDemoHDTmpBck->init(bck, TRUE, -1, 1.0f, 0, -1, true);
                        }
                    }
                    ret = TRUE;
                } else if (var3 == 4) {
                    J3DAnmTevRegKey* brk = static_cast<J3DAnmTevRegKey*>(
                        dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), var4));
                    if (brk != NULL && field_0x668 != brk) {
                        brk->setFrame(0.0f);
                        field_0x668 = brk;
                    }
                }
            }
        }
    }

    if (demo_actor->checkEnable(0x40)) {
        mpMorf->setFrameF(demo_actor->getAnmFrame());
        mUpperBck.setFrame(demo_actor->getAnmFrame());
        mFaceBck.setFrame(demo_actor->getAnmFrame());
        if (mpFaceBtp != NULL && !mBtpHeap.checkNoSetArcNo()) {
            mpFaceBtp->setFrame(demo_actor->getAnmFrame());
        }

        if (mpFaceBtk != NULL && !mBtkHeap.checkNoSetArcNo()) {
            mpFaceBtk->setFrame(demo_actor->getAnmFrame());
        }

        if (mpDemoFCTmpBck != NULL) {
            mpDemoFCTmpBck->setFrame(demo_actor->getAnmFrame());
        }

        if (mpDemoHLTmpBck != NULL) {
            mpDemoHLTmpBck->setFrame(demo_actor->getAnmFrame());
        }

        if (mpDemoHRTmpBck != NULL) {
            mpDemoHRTmpBck->setFrame(demo_actor->getAnmFrame());
        }

        if (mpDemoHDTmpBck != NULL) {
            mpDemoHDTmpBck->setFrame(demo_actor->getAnmFrame());
        }

        if (mpDemoFCTongueBtk != NULL) {
            mpDemoFCTongueBtk->setFrame(demo_actor->getAnmFrame());
        }

        if (mpDemoFCTmpBlk != NULL && mpDemoFCTmpBlk->getBlkAnm() != NULL) {
            mpDemoFCTmpBlk->getBlkAnm()->setFrame(demo_actor->getAnmFrame());
        }
    }

    if (!bVar1) {
        endHighModel();
    }

    return ret;
}

void daMidna_c::setFaceBtp(u16 i_index, BOOL i_isDemo) {
    J3DAnmTexPattern* btp;
    if (i_isDemo) {
        btp = static_cast<J3DAnmTexPattern*>(mBtpHeap.loadDataDemoRID(i_index, 0));
    } else {
        btp = static_cast<J3DAnmTexPattern*>(mBtpHeap.loadDataIdx(i_index));
    }

    if (btp != NULL && mpModel != NULL) {
        mpFaceBtp = btp;
        mpFaceBtp->searchUpdateMaterialID(mpModel->getModelData());
        mpModel->getModelData()->entryTexNoAnimator(mpFaceBtp);
        mFrameCounter = 0;
        mpFaceBtp->setFrame(mFrameCounter);
        offStateFlg0(FLG0_UNK_40);
    }
}

void daMidna_c::setFaceBtk(u16 i_index, BOOL i_isDemo) {
    J3DAnmTextureSRTKey* btk;
    if (i_isDemo) {
        btk = static_cast<J3DAnmTextureSRTKey*>(mBtkHeap.loadDataDemoRID(i_index, 0));
    } else {
        btk = static_cast<J3DAnmTextureSRTKey*>(mBtkHeap.loadDataIdx(i_index));
    }

    if (btk != NULL && mpModel != NULL) {
        mpFaceBtk = btk;
        mpFaceBtk->searchUpdateMaterialID(mpModel->getModelData());
        mpModel->getModelData()->entryTexMtxAnimator(mpFaceBtk);
        mFrameCounter = 0;
        mpFaceBtk->setFrame(mFrameCounter);
        offStateFlg0(FLG0_UNK_40);
    }
}

void daMidna_c::setLeftHandShape(u16 i_idx) {
    mLeftHandShapeIdx = i_idx;
    if (mpLeftHandShape != NULL) {
        mpLeftHandShape->hide();
    }
    mpShadowLeftHandShape->hide();

    if (i_idx == 0xfe || i_idx == 0xfd) {
        mpShadowLeftHandShape = mpShadowModel->getModelData()->getMaterialNodePointer(1)->getShape();
        if (mpLeftHandShape != NULL) {
            mpLeftHandShape = mpModel->getModelData()->getMaterialNodePointer(6)->getShape();
        }
    } else {
        mpShadowLeftHandShape = mpShadowHandsBmd->getModelData()->getMaterialNodePointer(i_idx)->getShape();
        if (mpLeftHandShape != NULL) {
            mpLeftHandShape = mpHandsBmd->getModelData()->getMaterialNodePointer(i_idx)->getShape();
        }
    }

    mpShadowLeftHandShape->show();
    if (mpLeftHandShape != NULL) {
        mpLeftHandShape->show();
    }
}

void daMidna_c::setRightHandShape(u16 i_idx) {
    mRightHandShapeIdx = i_idx;
    if (mpRightHandShape != NULL) {
        mpRightHandShape->hide();
    }
    mpShadowRightHandShape->hide();

    if (i_idx == 0xfe || i_idx == 0xfd) {
        mpShadowRightHandShape = mpShadowModel->getModelData()->getMaterialNodePointer(2)->getShape();
        if (mpRightHandShape != NULL) {
            mpRightHandShape = mpModel->getModelData()->getMaterialNodePointer(7)->getShape();
        }
    } else {
        mpShadowRightHandShape = mpShadowHandsBmd->getModelData()->getMaterialNodePointer(i_idx)->getShape();
        if (mpRightHandShape != NULL) {
            mpRightHandShape = mpHandsBmd->getModelData()->getMaterialNodePointer(i_idx)->getShape();
        }
    }

    mpShadowRightHandShape->show();
    if (mpRightHandShape != NULL) {
        mpRightHandShape->show();
    }
}

BOOL daMidna_c::checkHairOnlyAnime(int i_anm) const {
    return i_anm >= ANM_HAIR && i_anm < ANM_MAX;
}

void daMidna_c::setBckAnime(J3DAnmTransform* i_bck, int i_attr, f32 i_morf) {
    mpMorf->setAnm(i_bck, i_attr, i_morf, 1.0f, 0.0f, -1.0f);
    void* bas;
    
    if (!mBckHeap[0].checkNoSetArcNo()) {
        bas = static_cast<mDoExt_transAnmBas*>(i_bck)->getBas();
        if (bas == NULL) {
            offStateFlg1(FLG1_UNK_800);
        } else {
            onStateFlg1(FLG1_UNK_800);
        }
    } else {
        u8* buf = mBckHeap[0].getBuffer();
        if (*(u32*)(buf + 0x1c) == 0xffffffff) {
            offStateFlg1(FLG1_UNK_800);
            bas = NULL;
        } else {
            onStateFlg1(FLG1_UNK_800);
            bas = mBckHeap[0].getBuffer() + *(u32*)(buf + 0x1c);
        }
    }

    if (bas != NULL) {
        mSound.initAnime(bas, mpMorf->getPlaySpeed() >= 0.0f, mpMorf->getLoopFrame(), 0.0f);
    }
}

void daMidna_c::setAnm() {
    u16 sVar4, res_id;
    offStateFlg0((daMidna_FLG0)(FLG0_NO_HAIR_SCALE | FLG0_UNK_200000));

    if (setDemoAnm()) {
        return;
    }

    daAlink_c* link = daAlink_getAlinkActorClass();
    BOOL bVar1;
    if (dComIfGp_event_runCheck() || checkEndResetStateFlg0(ERFLG0_NO_SERVICE_WAIT)) {
        bVar1 = TRUE;
    } else {
        bVar1 = FALSE;
    }
    BOOL bVar2 = FALSE;
    BOOL bVar3 = TRUE;
    bool tired = checkMidnaTired();
    daMidna_ANM anm;

    if (link->checkMidnaWolfDeadAnime() && !checkStateFlg0(FLG0_TAG_WAIT)) {
        if (tired) {
            anm = ANM_CLINGD;
        } else {
            anm = ANM_WLDIEA;
        }
        offStateFlg0(FLG0_UNK_1000);
    } else if (link->checkMidnaWolfSwimDeadAnime() && !checkStateFlg0(FLG0_TAG_WAIT)) {
        if (tired) {
            anm = ANM_CLINGD;
        } else {
            anm = ANM_WLSWIMDIE;
            bVar2 = TRUE;
        }
        offStateFlg0(FLG0_UNK_1000);
    } else if (link->checkMidnaGanonCatchAnm()) {
        bVar2 = TRUE;
        if (checkSetAnime(0, ANM_MGNCATCHED)
            || (mpMorf->isStop() && checkSetAnime(0, ANM_MGNCATCHST)))
        {
            anm = ANM_MGNCATCHED;
        } else {
            anm = ANM_MGNCATCHST;
        }
    } else if (link->checkMidnaGanonThrowLeftAnm()) {
        if (
            checkSetAnime(0, ANM_MGNTHROWLED) ||
            (mpMorf->isStop() && checkSetAnime(0, ANM_MGNTHROWLST))
        ) {
            bVar2 = TRUE;
            anm = ANM_MGNTHROWLED;
            if (mpMorf->isStop() && checkSetAnime(0, ANM_MGNTHROWLED)) {
                link->setEndGanonThrow();
            }
        } else {
            anm = ANM_MGNTHROWLST;
        }
    } else if (link->checkMidnaGanonThrowRightAnm()) {
        if (
            checkSetAnime(0, ANM_MGNTHROWRED) ||
            (mpMorf->isStop() && checkSetAnime(0, ANM_MGNTHROWRST))
        ) {
            bVar2 = TRUE;
            anm = ANM_MGNTHROWRED;
            if (mpMorf->isStop() && checkSetAnime(0, ANM_MGNTHROWRED)) {
                link->setEndGanonThrow();
            }
        } else {
            anm = ANM_MGNTHROWRST;
        }
    } else if (mDemoMode == 9) {
        anm = ANM_WARPIN;
        offStateFlg0(FLG0_UNK_1000);
    } else if (mDemoMode == 0x15) {
        anm = ANM_GRABED;
        offStateFlg0(FLG0_UNK_1000);
    } else if (mDemoMode == 0xd || mDemoMode == 0xc) {
        anm = ANM_LEADTOWAITA;
        bVar2 = TRUE;
        offStateFlg0(FLG0_UNK_1000);
    } else if (mDemoMode == 0x11) {
        anm = ANM_DOOROPC;
        offStateFlg0(FLG0_UNK_1000);
    } else if (mDemoMode == 0x12) {
        anm = ANM_DOOROPD;
        offStateFlg0(FLG0_UNK_1000);
    } else if (mDemoMode == 8) {
        anm = ANM_S_HOLE;
    } else if (
        mDemoMode == 7 || mDemoMode == 6 || mDemoMode == 0x13 || mDemoMode == 0x14 || mDemoMode == 0xe
    ) {
        anm = ANM_LEADWAIT;
        bVar1 = TRUE;
        offStateFlg0(FLG0_UNK_1);
    } else if (mDemoMode == 5) {
        if (field_0x8dc > 0.5f) {
            anm = ANM_RETURN;
        } else {
            anm = ANM_S_RETURN;
        }
    } else if (mDemoMode == 0xf) {
        anm = ANM_RETURN;
        bVar2 = TRUE;
    } else if (
        (checkShadowModelDrawSmode() && !checkStateFlg0(FLG0_WOLF_NO_POS)) ||
        mDemoMode == 6 || mDemoMode == 3 || mDemoMode == 0x10 || mDemoMode == 4
    ) {
        if (field_0x84e == 4) {
            anm = ANM_S_RETURN;
        } else if (field_0x84e == 2) {
            if (checkStateFlg0(FLG0_UNK_80000)) {
                anm = ANM_S_APPEARBL;
            } else {
                anm = ANM_S_APPEAR;
            }
        } else if (checkEndResetStateFlg0(ERFLG0_UNK_1)) {
            anm = ANM_S_MOVE;
        } else {
            anm = ANM_LEADWAIT;
        }

        offStateFlg0((daMidna_FLG0)(FLG0_UNK_1000 | FLG0_UNK_1));
        bVar1 = TRUE;
        bVar2 = TRUE;
    } else if (checkStateFlg0(FLG0_UNK_1000)) {
        bVar2 = TRUE;
        anm = ANM_LEADTOWAITA;
        if (mpMorf->isStop() && checkSetAnime(0, ANM_LEADTOWAITA)) {
            offStateFlg0(FLG0_UNK_1000);
        }
    } else if (checkStateFlg0(FLG0_UNK_200)) {
        if (
            (mpMorf->isStop() && checkSetAnime(0, ANM_LEADED)) ||
            checkSetAnime(0, ANM_LEADSWAIT) || checkSetAnime(0, ANM_LEADWAIT)
        ) {
            anm = ANM_LEADWAIT;
            offStateFlg0(FLG0_NO_INPUT);
        } else {
            anm = ANM_LEADED;
        }
    } else if (checkStateFlg0((daMidna_FLG0)(FLG0_TAG_WAIT | FLG0_UNK_100))) {
        anm = ANM_LEADST;
    } else if (mpKago != NULL) {
        if (dComIfGp_evmng_checkStartDemo()) {
            anm = ANM_CLING;
            onStateFlg0(FLG0_UNK_4000000);
        } else if (checkStateFlg0(FLG0_UNK_4000000)) {
            if (mpMorf->isStop() || checkSetAnime(0, ANM_CLING)) {
                anm = ANM_CLING;
            } else {
                anm = ANM_LEADTOWAITA;
            }
        } else {
            anm = ANM_LEADTOWAITA;
            onStateFlg0(FLG0_UNK_4000000);
        }
    } else {
        bVar3 = FALSE;
        offStateFlg0(FLG0_UNK_4000000);
        if (!tired && mMotionNum >= 31 && mMotionNum < 40 && !checkStateFlg0(FLG0_NO_DRAW)) {
            static daMidna_ANM const motionTable[9] = {
                ANM_WAITA,
                ANM_CLING,
                ANM_CLINGB,
                ANM_WAITB,
                ANM_TALKA,
                ANM_HIT,
                ANM_LOOKABOUT,
                ANM_SWAITA,
                ANM_SWAITD,
            };
            anm = motionTable[mMotionNum - 31];
            offStateFlg0(FLG0_UNK_1000);
        } else if (link->checkMidnaWolfDashAnime() && !tired) {
            if (
                (mpMorf->isStop() && checkSetAnime(0, ANM_CLINGST)) ||
                checkSetAnime(0, ANM_CLING)
            ) {
                anm = ANM_CLING;
            } else {
                anm = ANM_CLINGST;
            }
        } else if ((link->checkMidnaDisappearMode() || (link->checkMidnaDigInAnime() && !tired))
                        && checkMidnaRealBody()) {
            anm = ANM_RETURN;
            if (!checkSetAnime(0, ANM_RETURN)) {
                offStateFlg0(FLG0_NO_DRAW);
            }
            bVar2 = TRUE;
        } else if (dComIfGp_checkPlayerStatus1(0, 0x800000)) {
            anm = ANM_WAITA;
            offStateFlg0(FLG0_UNK_1);
        } else if (tired) {
            if (link->checkMidnaDigInAnime()) {
                anm = ANM_DIGIN;
            } else if (link->checkMidnaClingAnime() || link->checkMidnaLowClingAnime()) {
                anm = ANM_CLINGD;
            } else {
                anm = ANM_WAITD;
            }
        } else if (link->checkMidnaLookAroundAnime()) {
            if (
                (mpMorf->isStop() && checkSetAnime(0, ANM_ENTRANCE)) || checkSetAnime(0, ANM_SWAITB)
            ) {
                anm = ANM_SWAITB;
                bVar2 = TRUE;
            } else {
                anm = ANM_ENTRANCE;
            }
        } else if (link->checkMidnaRopeWaitStaggerAnime()) {
            anm = ANM_ROPEWAIT;
        } else if (link->checkMidnaRopeMoveStaggerAnime()) {
            anm = ANM_ROPEFALTER;
        } else if (link->checkMidnaClingAnime()) {
            anm = ANM_CLING;
        } else if (link->checkMidnaLowClingAnime()) {
            anm = ANM_CLINGB;
        } else if (link->checkMidnaPanicAnime() || checkEndResetStateFlg0(ERFLG0_FORCE_PANIC)) {
            anm = ANM_PANIC;
            offStateFlg0(FLG0_UNK_1);
        } else if (
            !checkStateFlg0(FLG0_UNK_1) && checkSetAnime(0, ANM_SWAITB) && !mpMorf->isStop()
        ) {
            anm = ANM_SWAITB;
        } else {
            anm = ANM_WAITA;
        }
    }

    if (anm != ANM_WAITA && anm != ANM_LEADWAIT && anm != ANM_PANIC) {
        offStateFlg0(FLG0_UNK_1);
    }

    if (anm == ANM_WAITA || anm == ANM_WAITB || anm == ANM_TALKA) {
        onStateFlg0(FLG0_UNK_4);
    } else {
        offStateFlg0(FLG0_UNK_4);
    }

    sVar4 = mBckHeap[1].getIdx();
    res_id = m_anmDataTable[anm].mResID;
    J3DAnmTransform* bck;

    if (
        !mBckHeap[0].checkNoSetArcNo() || (!checkSetAnime(0, anm) &&
        (mMotionNum != 0 || !checkStateFlg0(FLG0_UNK_1) ||
        ((anm != ANM_WAITA || dComIfGp_event_runCheck()) && anm != ANM_LEADWAIT)))
    ) {
        if (!mBckHeap[0].checkNoSetArcNo()) {
            mBckHeap[0].resetArcNo();
            mBckHeap[0].resetIdx();
            mBckHeap[1].resetArcNo();
            mBckHeap[1].resetIdx();
        }

        if (anm == ANM_S_APPEAR || anm == ANM_S_APPEARBL) {
            mSound.startCreatureSound(Z2SE_MIDNA_APPEAR, 0, -1);
        } else if (anm == ANM_S_RETURN || anm == ANM_RETURN) {
            mSound.startCreatureSound(Z2SE_MIDNA_DISAPPEAR, 0, -1);
        }

        if (checkStateFlg0(FLG0_UNK_2)) {
            offStateFlg0(FLG0_UNK_2);
        }

        f32 morf;
        if (bVar2 || checkEndResetStateFlg0(ERFLG0_FORCE_MORF_CANCEL)) {
            morf = 0.0f;
        } else {
            morf = 5.0f;
        }
        bck = (J3DAnmTransform*)mBckHeap[0].loadDataIdx(res_id);
        setBckAnime(bck, -1, morf);

        offStateFlg0(FLG0_UNK_1);

        if (checkStateFlg0(FLG0_UNK_2)) {
            setUpperAnime(mBckHeap[0].getIdx(), 0xffff);
            offStateFlg0(FLG0_UNK_2);
        }

        if (anm == ANM_WARPIN) {
            if (daPy_py_c::checkNowWolf()) {
                u32 sound_id;
                if (checkStateFlg1(FLG1_SIDE_WARP)) {
                    sound_id = Z2SE_MDN_WARP_IN_YOKO;
                } else {
                    sound_id = Z2SE_MDN_WARP_IN_TATE;
                }
                mSound.startCreatureSound(sound_id, 0, -1);
            }

            if (!checkStateFlg0(FLG0_PORTAL_OBJ_CALL)) {
                cXyz pos;
                mDoMtx_multVecZero(mpShadowModel->getAnmMtx(JNT_BACKBONE1), &pos);
                f32 dist = pos.abs(current.pos);
                f32 tmp = 19.0f * cM_scos(shape_angle.x);
                current.pos.set(pos.x + tmp * cM_ssin(shape_angle.y),
                                pos.y - dist,
                                pos.z + tmp * cM_scos(shape_angle.y));
            }
        } else if (anm == ANM_S_RETURN) {
            current.pos.x += 90.0f * cM_ssin(shape_angle.y);
            current.pos.z += 90.0f * cM_scos(shape_angle.y);
            shape_angle.y += 0x8000;
            mPrevShapeAngleY = shape_angle.y;
            current.angle.y = shape_angle.y;
            mpMorf->getOldTransInfo()[JNT_BACKBONE1].mTranslate.z += 90.0f;
            mpMorf->getOldTransInfo()[JNT_WAIST].mTranslate.z += 90.0f;
            Quaternion* quat_backbone_ptr = &mpMorf->getOldQuaternion()[JNT_BACKBONE1];
            Quaternion quat1, quat2;
            JMAEulerToQuat(0, 0x8000, 0, &quat1);
            quat2 = *quat_backbone_ptr;
            mDoMtx_QuatConcat(&quat1, &quat2, quat_backbone_ptr);
            quat_backbone_ptr = &mpMorf->getOldQuaternion()[JNT_WAIST];
            quat2 = *quat_backbone_ptr;
            mDoMtx_QuatConcat(&quat1, &quat2, quat_backbone_ptr);
        } else if (anm == ANM_WLSWIMDIE) {
            J3DTransformInfo* trans_backbone = &mpMorf->getOldTransInfo()[JNT_BACKBONE1];
            J3DTransformInfo* trans_waist = &mpMorf->getOldTransInfo()[JNT_WAIST];
            trans_backbone->mTranslate.x = trans_waist->mTranslate.x = 0.9f;
            trans_backbone->mTranslate.y = trans_waist->mTranslate.y = 36.060127f;
            trans_backbone->mTranslate.z = trans_waist->mTranslate.z = -14.515757f;
        } else if (anm == ANM_LEADTOWAITA && mDemoMode != 0xd && mDemoMode != 0xc) {
            if (link->checkMidnaRide()) {
                link->onMidnaRideShock();
            }
        } else if (mDemoMode == 0xc) {
            current.pos.y += 1000.0f;
        }

        setUpperAnimeAndSe(anm);

    } else if (
        mMotionNum == 0 &&
        ((anm == ANM_WAITA && !dComIfGp_event_runCheck()) || anm == ANM_LEADWAIT)
    ) {
        if (dComIfGp_checkPlayerStatus1(0, 0x800000)) {
            onStateFlg0(FLG0_UNK_2);
            offStateFlg0(FLG0_UNK_1);
            if (setUpperAnimeAndSe(ANM_WAITTP)) {
                mUpperBck.setLoopFrame(15.0);
                mpMorf->setMorf(5.0f);
                offStateFlg0(FLG0_UNK_80);
                if (mpModel != NULL) {
                    setFaceBtp(0x3F8, FALSE);
                }
            } else if (mUpperBck.checkFrame(9.0f)) {
                setRightHandShape(2);
            } else if (mUpperBck.checkFrame(12.0f)) {
                setLeftHandShape(0);
            }
        } else if (mDemoMode == 0x14) {
            if (setUpperAnimeAndSe(ANM_GRAB)) {
                mpMorf->setMorf(5.0f);
            }
        } else if (checkStateFlg0(FLG0_UNK_2)) {
            offStateFlg0(FLG0_UNK_2);
            setUpperAnime(mBckHeap[0].getIdx(), 0xffff);
            mpMorf->setMorf(5.0f);
        } else if (checkStateFlg0(FLG0_UNK_1)) {
            if (
                mUpperBck.isStop() || mUpperBck.checkFrame(0.0f) ||
                mNeckAngle.x != 0 || mNeckAngle.y != 0 || bVar1 ||
                (checkSetAnime(0, ANM_SWAITC) && fabsf(speedF) > 0.1f)
            ) {
                offStateFlg0(FLG0_UNK_1);
                bck = (J3DAnmTransform*)mBckHeap[0].loadDataIdx(m_anmDataTable[anm].mResID);
                setBckAnime(bck, J3DFrameCtrl::EMode_LOOP, 5.0f);
                setUpperAnime(mBckHeap[0].getIdx(), 0xffff);
            } else if (checkSetAnime(0, ANM_SWAITB) && mUpperBck.checkFrame(95.0f)
                && fabsf(speedF) < 0.1f && cM_rnd() < 0.5f)
            {
                anm = ANM_SWAITC;
                setUpperAnimeAndSe(ANM_SWAITC);
                bck = (J3DAnmTransform*)mBckHeap[0].loadDataIdx(m_anmDataTable[anm].mResID);
                setBckAnime(bck, J3DFrameCtrl::EMode_NONE, 0.0f);
            }
        } else if (
            daPy_py_c::checkNowWolf() && !bVar1 &&
            ((mNeckAngle.y == 0 && mNeckAngle.x == 0 && anm != ANM_LEADWAIT && cM_rnd() < 0.01f) ||
            (anm == ANM_LEADWAIT && !checkStateFlg0(FLG0_NO_HAIR_LEAD) && cM_rnd() < 0.0125f))
        ) {
            onStateFlg0(FLG0_UNK_1);
            if (anm == ANM_LEADWAIT) {
                anm = ANM_LEADSWAIT;
            } else {
                f32 rnd;
                if (fabsf(link->speedF) < 0.1f) {
                    rnd = cM_rndF(5.0f);
                } else {
                    rnd = cM_rndF(4.0f);
                }

                if (rnd < 1.0f) {
                    anm = ANM_WAITB;
                } else if (rnd < 2.0f) {
                    anm = ANM_SWAITB;
                } else if (rnd < 3.0f) {
                    anm = ANM_SWAITD;
                } else if (rnd < 4.0f) {
                    anm = ANM_LOOKABOUT;
                } else {
                    anm = ANM_HIT;
                }
            }

            res_id = m_anmDataTable[anm].mResID;
            setUpperAnimeAndSe(anm);
            bck = (J3DAnmTransform*)mBckHeap[0].loadDataIdx(res_id);
            setBckAnime(bck, J3DFrameCtrl::EMode_NONE, 5.0f);
        }

    } else if (checkStateFlg0(FLG0_UNK_2)) {
        offStateFlg0(FLG0_UNK_2);
        setUpperAnime(mBckHeap[0].getIdx(), 0xffff);
        mpMorf->setMorf(5.0f);
    }

    if (sVar4 != mBckHeap[1].getIdx()) {
        if (
            checkSetAnime(1, ANM_CLINGB) || checkSetAnime(1, ANM_CLING) ||
            checkSetAnime(1, ANM_GRAB) || checkSetAnime(1, ANM_GRABED) ||
            checkSetAnime(1, ANM_WAITB)
        ) {
            setLeftHandShape(0);
            setRightHandShape(2);
        } else {
            setLeftHandShape(0xFE);
            setRightHandShape(0xFE);
        }
    }

    if (checkSetAnime(1, ANM_SWAITD)) {
        if (mUpperBck.checkFrame(110.0f)) {
            setLeftHandShape(0xFE);
            setRightHandShape(0xFE);
        } else if (mUpperBck.checkFrame(15.0f)) {
            setLeftHandShape(0);
            setRightHandShape(2);
        }
    } else if (checkSetAnime(1, ANM_GRABED) && mUpperBck.checkFrame(53.0f)) {
        setLeftHandShape(0xFE);
        setRightHandShape(0xFE);
    }

    daMidna_ANM face_anm;
    if (mDemoMode == 0x13) {
        face_anm = ANM_GRABST;
    } else if (mDemoMode == 0x14) {
        face_anm = ANM_NONE;
    } else if (
        (anm == ANM_LEADWAIT || anm == ANM_LEADSWAIT) &&
        !checkStateFlg0(FLG0_NO_HAIR_LEAD) && checkStateFlg0(FLG0_UNK_200)
    ) {
        face_anm = ANM_HAIR;
    } else if (mDemoMode == 6) {
        face_anm = ANM_S_WAITS;
    } else if (mDemoMode == 7) {
        face_anm = ANM_S_TAKES;
    } else if (mDemoMode == 0xe) {
        face_anm = ANM_S_PACKAWAY;
    } else if (bVar3 || mFaceNum == 100) {
        face_anm = ANM_NONE;
    } else if (mFaceNum == 0x1f) {
        face_anm = ANM_FTALKA;
    } else if (mFaceNum == 0x20) {
        face_anm = ANM_FTALKB;
    } else if (mFaceNum == 0x21) {
        face_anm = ANM_FTALKC;
    } else if (mFaceNum == 0x22) {
        face_anm = ANM_FTALKC2;
    } else {
        face_anm = ANM_NONE;
    }

    if (
        checkHairOnlyAnime(face_anm) ||
        (mpModel != NULL && !checkStateFlg0(FLG0_NO_DRAW) &&
        (sVar4 != mBckHeap[1].getIdx() || face_anm != ANM_NONE ||
        (face_anm == ANM_NONE && checkStateFlg0(FLG0_UNK_800000))))
    ) {
        if (face_anm == ANM_NONE) {
            offStateFlg0(FLG0_UNK_800000);
            mBckHeap[2].resetIdx();
            mJntNo = JNT_HEAD;
        } else {
            setFaceAnime(m_anmDataTable[face_anm].mResID, 0xffff);
            if (checkHairOnlyAnime(face_anm)) {
                mJntNo = JNT_CHIN;
                mFaceBckTimer = 20;
            } else {
                mJntNo = JNT_HEAD;
            }
        }
    } else {
        offStateFlg0(FLG0_UNK_800000);
        mBckHeap[2].resetIdx();
        mJntNo = JNT_HEAD;
    }

    if (mpModel != NULL && !checkStateFlg0(FLG0_NO_DRAW)
                            && (!checkStateFlg0(FLG0_UNK_1) || anm != ANM_WAITA)) {
        if (!mBtpHeap.checkNoSetArcNo()) {
            mBtpHeap.resetArcNo();
            mBtpHeap.resetIdx();
        }

        u16 tex_id;
        if (face_anm != ANM_NONE) {
            tex_id = m_anmDataTable[face_anm].mTexID;
        } else if (dComIfGp_checkPlayerStatus1(0, 0x800000)) {
            tex_id = 0x10;
        } else if (checkSetAnime(1, ANM_GRAB)) {
            tex_id = 0x13;
        } else {
            tex_id = m_anmDataTable[anm].mTexID;
        }

        setFaceBtp(m_texDataTable[tex_id].mBtpID, FALSE);

        if (!mBtkHeap.checkNoSetArcNo()) {
            mBtkHeap.resetArcNo();
            mBtkHeap.resetIdx();
        }

        setFaceBtk(m_texDataTable[tex_id].mBtkID, FALSE);

        if (m_texDataTable[tex_id].mBtpID == 0x405) {
            onStateFlg0(FLG0_UNK_40);
        } else {
            offStateFlg0(FLG0_UNK_40);
        }
    }

    BOOL local_c0 = mDemoMode == 0x14 || (anm >= ANM_DOOROPC && anm < ANM_FTALKA);
    if (checkStateFlg0(FLG0_UNK_2) || local_c0 || mJntNo == JNT_CHIN) {
        onStateFlg0((daMidna_FLG0)(FLG0_NO_HAIR_SCALE | FLG0_UNK_200000));
        if (local_c0) {
            onEndResetStateFlg0(ERFLG0_UNK_40);
        }
    } else if (
        anm == ANM_S_APPEAR || anm == ANM_S_APPEARBL || anm == ANM_RETURN ||
        (anm == ANM_WARPIN && mpMorf->getFrame() > 2.0f) || anm == ANM_S_RETURN
    ) {
        onStateFlg0(FLG0_NO_HAIR_SCALE);
    }
}

s16 daMidna_c::getNeckAimAngle(cXyz const* i_atnPos, s16* o_neckX, s16* o_neckY, s16* o_eyeX, s16* o_eyeY) {
    cXyz eye_vec;
    s16 offset_y = mPrevShapeAngleY;
    eye_vec = eyePos - mHeadCenterPos;
    s16 eye_angle_x = eye_vec.atan2sY_XZ();
    s16 eye_angle_y = eye_vec.atan2sX_Z();
    s16 sVar1 = eye_angle_x - mNeckAngle.x;
    s16 sVar2 = eye_angle_y - offset_y - mNeckAngle.y;

    if (i_atnPos != NULL) {
        cXyz atn_vec = *i_atnPos - mHeadCenterPos;
        s16 atn_angle_x = atn_vec.atan2sY_XZ();
        s16 atn_angle_y = atn_vec.atan2sX_Z() - offset_y;
        if ((atn_angle_y > 0x7000 && mNeckAngle.y < 0) || (atn_angle_y < -0x7000 && mNeckAngle.y > 0)) {
            atn_angle_y *= -1;
        }

        s16 sVar7 = cLib_minMaxLimit<s16>((s16)atn_angle_x, -10000, 8000);
        s16 sVar8 = cLib_minMaxLimit<s16>((s16)atn_angle_y, -20000, 20000);
        *o_eyeX = (s16)(sVar7 - sVar1) >> 1;
        *o_eyeY = (s16)(sVar8 - sVar2) >> 1;
        *o_neckX = *o_eyeX;
        *o_neckY = *o_eyeY;
        *o_eyeX += (s16)(atn_angle_x - sVar7);
        *o_eyeY += (s16)(atn_angle_y - sVar8);
    } else {
        *o_neckX = daAlink_getAlinkActorClass()->getProcNeckX();
        *o_neckY = daAlink_getAlinkActorClass()->getMidnaProcNeckY();
    }
    return sVar2;
}

void daMidna_c::clearEyeMove() {
    mpEyeMatAnm[0]->setNowOffsetX(0.0f);
    mpEyeMatAnm[1]->setNowOffsetX(0.0f);
    mpEyeMatAnm[0]->setNowOffsetY(0.0f);
    mpEyeMatAnm[1]->setNowOffsetY(0.0f);
    
    if (daMidna_matAnm_c::getEyeMoveFlg()) {
        daMidna_matAnm_c::offEyeMoveFlg();
        daMidna_matAnm_c::setMorfFrame(3);
    }
}

void daMidna_c::setEyeMove(cXyz const* i_atnPos, s16 i_angleX, s16 i_angleY) {
    daAlink_c* link = daAlink_getAlinkActorClass();
    u8 timer = mEyeMoveTimer;
    f32 move_x = mEyeMoveX;
    f32 move_y = mEyeMoveY;
    mEyeMoveTimer = cM_rndF(30.0f) + 75.0f;
    mEyeMoveX = 0.0f;
    mEyeMoveY = 0.0f;

    f32 tmp_y, tmp_x;
    if (i_atnPos != NULL) {
        tmp_y = i_angleX * (1.0f / 0x2000);
        tmp_x = i_angleY * (1.0f / 0x2000);
    } else if (link->getEyeMoveRateY() != 0.0f || link->getMidnaEyeMoveRateX() != 0.0f) {
        tmp_y = link->getEyeMoveRateY();
        tmp_x = link->getMidnaEyeMoveRateX();
    } else if (
        !dComIfGp_event_runCheck() && checkStateFlg0(FLG0_UNK_40) &&
        !dComIfGp_checkPlayerStatus1(0, 0x800000) && fabsf(speedF) < 0.01f
    ) {
        if (timer != 0) {
            mEyeMoveTimer = timer - 1;
            mEyeMoveX = move_x;
            mEyeMoveY = move_y;
        } else if (move_x != 0 || move_y != 0) {
            if (cM_rnd() < 0.5f) {
                mEyeMoveX = 0.0f;
                mEyeMoveY = 0.0f;
            } else {
                s16 angle = cM_atan2s(mEyeMoveX, mEyeMoveY);
                s16 angle2 = angle + (s16)((int)cM_rndF(3.0f) * 0x2000 + 0x6000);
                mEyeMoveX = cM_ssin(angle2);
                mEyeMoveY = cM_scos(angle2);
            }
        } else {
            s16 angle = (s16)cM_rndF(8.0f) * 0x2000;
            mEyeMoveX = cM_ssin(angle);
            mEyeMoveY = cM_scos(angle);
        }
        
        tmp_x = mEyeMoveX;
        tmp_y = mEyeMoveY;
    } else {
        clearEyeMove();
        return;
    }

    if (daMidna_matAnm_c::getMorfFrame() == 0) {
        if (tmp_x > 1.0f) {
            tmp_x = 1.0f;
        } else if (tmp_x < -1.0f) {
            tmp_x = -1.0f;
        }

        if (tmp_y > 1.0f) {
            tmp_y = 1.0f;
        } else if (tmp_y < -1.0f) {
            tmp_y = -1.0f;
        }
        
        f32 target_lx, target_rx, target_ly, target_ry;
        if (tmp_x > 0.0f) {
            target_lx = tmp_x * -0.2f;
            target_rx = tmp_x * 0.2f;
        } else {
            target_lx = tmp_x * -0.2f;
            target_rx = tmp_x * 0.2f;
        }

        if (tmp_y > 0.0f) {
            target_ly = tmp_y * 0.2f;
        } else {
            target_ly = tmp_y * 0.2f;
        }
        target_ry = target_ly;
        
        f32 fVar7 = JMAFastSqrt(tmp_x * tmp_x + tmp_y * tmp_y);
        if (fVar7 > 1.0f) {
            if (tmp_x * tmp_y < 0.0f) {
                target_rx *= fabsf(tmp_x) / fVar7;
                target_ry *= fabsf(tmp_y) / fVar7;
            } else {
                target_lx *= fabsf(tmp_x) / fVar7;
                target_ly *= fabsf(tmp_y) / fVar7;
            }
        }

        daMidna_matAnm_c::onEyeMoveFlg();
        cLib_addCalc(mpEyeMatAnm[0]->getNowOffsetXP(), target_lx, 0.5f, 0.1f, 0.03f);
        cLib_addCalc(mpEyeMatAnm[1]->getNowOffsetXP(), target_rx, 0.5f, 0.1f, 0.03f);
        cLib_addCalc(mpEyeMatAnm[0]->getNowOffsetYP(), target_ly, 0.5f, 0.1f, 0.03f);
        cLib_addCalc(mpEyeMatAnm[1]->getNowOffsetYP(), target_ry, 0.5f, 0.1f, 0.03f);
    }
}

void daMidna_c::setNeckAngle() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    BOOL clear_eye_move = mBtkHeap.getIdx() != 0x3A4 && !mBtkHeap.checkNoSetIdx() && mBtkHeap.getIdx() != 0x399;
    s16 neck_x = 0;
    s16 neck_y = 0;
    s16 eye_x = 0;
    s16 eye_y = 0;
    cXyz* pos;

    if (
        field_0x84e == 2 || checkMidnaTired() || checkSetAnime(0, ANM_CLINGB) ||
        (checkStateFlg0((daMidna_FLG0)(FLG0_NO_HAIR_SCALE | FLG0_UNK_200000)) && !checkEndResetStateFlg0(ERFLG0_UNK_100)) ||
        (clear_eye_move && !checkStateFlg0(FLG0_NO_DRAW)) ||
        (!checkStateFlg0(FLG0_UNK_1) && checkSetAnime(0, ANM_SWAITB))
    ) {
        pos = NULL;
        offStateFlg0(FLG0_FORCE_NECK_AIM);
    } else if (checkStateFlg0(FLG0_FORCE_NECK_AIM)) {
        pos = &mForceNeckAimPos;
        offStateFlg0(FLG0_FORCE_NECK_AIM);
    } else if (checkEndResetStateFlg0(ERFLG0_UNK_100)) {
        pos = &link->eyePos;
    } else if (
        checkShadowModelDrawSmode() && mDemoMode != 4 && mDemoMode != 7 && mDemoMode != 8 &&
        mDemoMode != 0x10 && mDemoMode != 3 &&!checkStateFlg0(FLG0_WOLF_NO_POS)
    ) {
        pos = &link->eyePos;
    } else if (
        link->checkMidnaAtnPos() && !checkSetAnime(0, ANM_LEADST) && !checkSetAnime(0, ANM_LEADED)
    ) {
        if (checkStateFlg0((daMidna_FLG0)(FLG0_TAG_WAIT | FLG0_UNK_100))) {
            pos = &link->eyePos;
        } else {
            pos = link->getMidnaAtnPos();
        }
    } else {
        pos = NULL;
    }

    s16 neckAimAngle = getNeckAimAngle(pos, &neck_x, &neck_y, &eye_x, &eye_y);
    daPy_addCalcShort(&mNeckAngle.x, neck_x, 3, 0x1000, 0x100);
    daPy_addCalcShort(&mNeckAngle.y, neck_y, 3, 0x1000, 0x100);

    if (clear_eye_move) {
        clearEyeMove();
    } else {
        setEyeMove(pos, eye_x, eye_y);
    }
}

void daMidna_c::initHairAngle() {
    static Vec const localOffset = {0.0f, 136.0f, -2.4f};
    MTXMultVec(mpShadowModel->getBaseTRMtx(), &localOffset, &mHairPos[0]);
    mHairDir[0] = cXyz::Zero;
    mHairAngleZ[0] = 0;
    mHairAngleY[0] = 0;
    mHairScale[0] = l_hairScale[0];
    for (int i = 1; i < 5; i++) {
        mHairPos[i] = mHairPos[i - 1];
        mHairPos[i].y -= 28.0f;
        mHairDir[i] = cXyz::Zero;
        mHairAngleZ[i] = 0;
        mHairAngleY[i] = 0;
        mHairScale[i] = l_hairScale[i];
    }

    onStateFlg0(FLG0_UNK_8);
}

void daMidna_c::setHairAngle() {
    if (!checkStateFlg0(FLG0_UNK_8)) {
        initHairAngle();
        return;
    }

    cXyz prev_pos, head_dir;
    mDoMtx_multVecSR(mpShadowModel->getAnmMtx(JNT_HEAD), &cXyz::BaseX, &head_dir);
    mDoMtx_multVecZero(mpShadowModel->getAnmMtx(JNT_HAIR_1), &prev_pos);
    s16 head_angle = head_dir.atan2sX_Z();
    s16 inv_head_angle = head_angle + 0x8000;
    cXyz vec, old_pos;

    int i;
    cXyz* pos = mHairPos;
    cXyz* dir = mHairDir;
    cXyz* scale = mHairScale;
    s16* angle_z = mHairAngleZ;
    s16* angle_y = mHairAngleY;

    f32 fVar4 = 0.75f;
    f32 fVar1 = 0.05f;
    s16 target_angle_y;
    BOOL bVar5 = false;
    f32 fVar2 = fabsf(speedF) * 0.04f;
    if (fVar2 > 1.0f) {
        fVar2 = 1.0f;
    }
    s16 target_angle_z, iVar16;
    iVar16 = field_0x872;
    BOOL bVar4 = false;
    field_0x872 += fVar2 * 0x1000 + 0x800;

    cXyz* atn_pos = NULL;
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (
        link->checkMidnaHairAtnPos() && !checkMidnaTired() &&
        !checkStateFlg0((daMidna_FLG0)(FLG0_NO_HAIR_SCALE | FLG0_UNK_200000 | FLG0_TAG_WAIT | FLG0_UNK_100))
    ) {
        atn_pos = link->getMidnaHairAtnPos();
        onStateFlg0(FLG0_UNK_10000000);
    } else {
        offStateFlg0(FLG0_UNK_10000000);
    }

    for (i = 0; i < 5; i++, pos++, dir++, angle_z++, angle_y++, scale++) {
        if (checkStateFlg0(FLG0_UNK_10000000)) {
            cLib_chasePos(scale, l_hairScale[4], 0.1f);
        } else {
            cLib_chasePos(scale, l_hairScale[i], 0.1f);
        }

        old_pos = *pos;

        if (checkStateFlg0(FLG0_UNK_200000)) {
            if (i == 4) {
                mDoMtx_multVec(mpShadowModel->getAnmMtx(JNT_HAIR_5), &cXyz::BaseX, pos);
            } else {
                mDoMtx_multVecZero(mpShadowModel->getAnmMtx(JNT_HAIR_2 + i), pos);
            }

            old_pos = *pos;
            vec = *pos - prev_pos;
            *angle_z = cM_atan2s(-vec.y, -vec.z);
            *angle_y = cM_atan2s(vec.x, JMAFastSqrt(vec.y * vec.y + vec.z * vec.z));
            prev_pos = *pos;
        } else if (atn_pos != NULL) {
            vec = *atn_pos - prev_pos;
            mDoMtx_stack_c::YrotS(-shape_angle.y);
            mDoMtx_stack_c::multVec(&vec, &vec);
            target_angle_y = fVar1 * cM_atan2s(vec.x, JMAFastSqrt(vec.y * vec.y + vec.z * vec.z));
            if (i == 0 && (vec.z < 0.0f || vec.y >= 0.0f)) {
                bVar4 = true;
            }

            if (bVar4) {
                if (vec.y < 1.0f && i < 4) {
                    vec.y = 1.0f;
                }
                target_angle_z = fVar1 * cM_atan2s(-vec.y, -vec.z) - (1.0f - fVar1) * 0x4000;
            } else {
                target_angle_z = fVar1 * (cM_atan2s(-vec.y, -vec.z) - 0x10000) - (1.0f - fVar1) * 0x4000;
            }

            cLib_addCalcAngleS(angle_z, target_angle_z, 5, 0x1800, 0x100);
            cLib_addCalcAngleS(angle_y, target_angle_y, 5, 0x1800, 0x100);
            fVar1 += 0.3f;
            if (fVar1 > 1.0f) {
                fVar1 = 1.0f;
            }
        } else {
            vec = *pos - prev_pos + *dir;
            vec += daAlink_getAlinkActorClass()->getWindSpeed();
            if (checkEndResetStateFlg0(ERFLG0_UNK_20)) {
                vec = cXyz::Zero;
            }

            vec.y -= 2.0f;
            vec.y += fVar2 * (cM_rndFX(3.0f) + 3.0f) * cM_ssin(iVar16);
            mDoMtx_stack_c::YrotS(-head_angle);
            mDoMtx_stack_c::multVec(&vec, &vec);
            if (vec.abs() < 1.0f) {
                if (i == 0) {
                    cLib_addCalcAngleS(angle_z, 0, 5, 0x1800, 0x100);
                    cLib_addCalcAngleS(angle_y, 0, 5, 0x1800, 0x100);
                } else {
                    cLib_addCalcAngleS(angle_z, 0, 5, 0x1800, 0x100);
                    cLib_addCalcAngleS(angle_y, 0, 5, 0x1800, 0x100);
                }
            } else {
                if (i == 0) {
                    if (vec.z > 0.0f) {
                        vec.z *= -1.0f;
                    }
                    f32 fVar21 = vec.absXZ();
                    if (fVar21 < vec.y * -1.732f) {
                        if (fVar21 < 1.0f) {
                            vec.x = 0.0f;
                            vec.z = -1.0f;
                            vec.y = -0.577367f;
                        } else {
                            if (fabsf(speedF) < 0.1f) {
                                vec.x *= 0.5f;
                                fVar21 = vec.absXZ();
                            }
                            vec.y = fVar21 * -0.577367f;
                        }
                        bVar5 = true;
                    }
                }

                *angle_y = cM_atan2s(vec.x, JMAFastSqrt(vec.y * vec.y + vec.z * vec.z));
                if (i == 4) {
                    *angle_z = cM_atan2s(-vec.y, -vec.z);
                } else {
                    *angle_z = 0;
                }
            }
            
            iVar16 -= (fVar2 * 0x1000 + 0x800);
        }

        static Vec const hairOffset = {0.0f, 0.0f, 28.0f};
        if (!checkStateFlg0(FLG0_UNK_200000)) {
            mDoMtx_stack_c::transS(prev_pos);
            mDoMtx_stack_c::YrotM(inv_head_angle);
            mDoMtx_stack_c::XrotM(*angle_z);
            mDoMtx_stack_c::YrotM(-*angle_y);
            mDoMtx_stack_c::scaleM(*scale);
            mDoMtx_stack_c::multVec(&hairOffset, pos);
        }

        if (checkEndResetStateFlg0(ERFLG0_UNK_20)) {
            *dir = cXyz::Zero;
        } else {
            *dir = (*pos - old_pos) * fVar4;
            if (bVar5) {
                *dir *= 0.5f;
            }
        }

        prev_pos = *pos;
        fVar4 -= 0.1f;
    }
}

void daMidna_c::setDemoData() {
    if (!dComIfGp_event_runCheck()) {
        mStaffID = -1;
    } else {
        mStaffID = dComIfGp_evmng_getMyStaffId("Midna", this, 0);
    }

    if (dComIfGp_getEvent().isOrderOK()) {
        if (mDemoType != 0) {
            mDemoType = 0;
            mDemoMode = 0;
            field_0x8dc = 0.0f;
            endHighModel();
            if (mLeftHandShapeIdx == 0xfd) {
                mLeftHandShapeIdx = 0xfe;
            }
            if (mRightHandShapeIdx == 0xfd) {
                mRightHandShapeIdx = 0xfe;
            }

            offStateFlg1((daMidna_FLG1)(FLG1_SHADOW_NO_DRAW | FLG1_NO_MASK_DRAW | FLG1_UNK_40
                                        | FLG1_UNK_20 | FLG1_UNK_10 | FLG1_FORCE_NORMAL_COL
                                        | FLG1_FORCE_TIRED_COL | FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE
                                        | FLG1_UNK_1));
        }

        offStateFlg0((daMidna_FLG0)(FLG0_UNK_2000000 | FLG0_UNK_1000000));

        if (mpDemoFCBlendBrk != NULL) {
            if (mpDemoFCBlendBmd != NULL) {
                mpDemoFCBlendBmd->getModelData()->removeTevRegAnimator(mpDemoFCBlendBrk);
            }
            mpDemoFCBlendBrk = NULL;
        }
        field_0x668 = NULL;

    } else {
        dDemo_actor_c* demo_actor = dDemo_c::getActor(demoActorID);
        if (demo_actor != NULL) {
            demo_actor->setModel(mpShadowModel);

            if (mDemoType != 1) {
                mDemoType = 1;
                onStateFlg0(FLG0_WOLF_NO_POS);
            }

            if (demo_actor->checkEnable(0x20)) {
                mDemoMode = demo_actor->getAnmId();
            }

            if (demo_actor->checkEnable(2)) {
                current.pos = demo_actor->getTrans();
            }

            if (demo_actor->checkEnable(8)) {
                shape_angle.x = 0;
                shape_angle.y = demo_actor->getRatate().y;
                shape_angle.z = 0;
            }

        } else if (mStaffID != -1) {
            if (mDemoType != 2) {
                mDemoType = 2;
            }

            char* cut_name = dComIfGp_getPEvtManager()->getMyNowCutName(mStaffID);
            if (cut_name != NULL) {
                mDemoMode = (cut_name[0] - 0x30) * 10 + (cut_name[1] - 0x30);

                cXyz* pos = dComIfGp_evmng_getMyXyzP(mStaffID, "pos");
                if (pos != NULL) {
                    mDemoPos = *pos;
                } else {
                    mDemoPos = current.pos;
                }

                f32* speed = dComIfGp_evmng_getMyFloatP(mStaffID, "speed");
                if (speed != NULL) {
                    mDemoSpeed = *speed;
                } else {
                    mDemoSpeed = 5.0f;
                }

                int* angle = dComIfGp_evmng_getMyIntegerP(mStaffID, "angle");
                if (angle != NULL) {
                    mDemoAngle = *angle;
                } else {
                    mDemoAngle = shape_angle.y;
                }

                cXyz* npos = dComIfGp_evmng_getMyXyzP(mStaffID, "npos");
                if (npos != NULL) {
                    setForceNeckAimPos(*npos);
                }

                f32* fprm0 = dComIfGp_evmng_getMyFloatP(mStaffID, "fprm0");
                if (fprm0 != NULL) {
                    field_0x8dc = *fprm0;
                } else {
                    field_0x8dc = 0.0f;
                }

            } else {
                mDemoMode = 1;
            }

            if (!checkMidnaRealBody() && !checkShadowModelDrawSmode()
                && (mDemoMode == 9 || mDemoMode == 0xb || mDemoMode == 0xd || mDemoMode == 0xc))
            {
                mDemoMode = 1;
            } else if (mDemoMode == 0xd) {
                offStateFlg0(FLG0_WOLF_NO_POS);
            }

            if ((mDemoMode == 1 || mDemoMode == 0xb || (mDemoMode == 2 && field_0x84e == 3))) {
                dComIfGp_evmng_cutEnd(mStaffID);
                if (mDemoMode == 1) {
                    offStateFlg0(FLG0_WOLF_NO_POS);
                }
            }
        }
    }
}

J3DAnmTevRegKey* daMidna_c::setSimpleBrk(J3DModelData* i_modelData, u16 i_idx) {
    J3DAnmTevRegKey* brk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_arcName, i_idx));
    if (brk == NULL) {
        return NULL;
    }

    brk->searchUpdateMaterialID(i_modelData);
    i_modelData->entryTevRegAnimator(brk);
    brk->setFrame(brk->getFrameMax() - 0.001f);
    return brk;
}

J3DAnmTextureSRTKey* daMidna_c::setSimpleBtk(J3DModelData* i_modelData, u16 i_idx) {
    J3DAnmTextureSRTKey* btk = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(l_arcName, i_idx));
    if (btk == NULL) {
        return NULL;
    }

    btk->searchUpdateMaterialID(i_modelData);
    i_modelData->entryTexMtxAnimator(btk);
    btk->setFrame(0.0f);
    return btk;
}

void daMidna_c::initMidnaModel() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (checkStateFlg1(FLG1_UNK_1)) {
        if (mpDemoBDTmpBmd != NULL && mpModel != mpDemoBDTmpBmd) {
            mpModel = mpDemoBDTmpBmd;
            mpMaskBmd = mpDemoBDMaskBmd;
            mpHandsBmd = mpDemoBDHandsBmd;
            mpHairhandBmd = mpDemoBDHairhandBmd;
            if (mpLeftHandShape != NULL) {
                mpLeftHandShape->hide();
            }
            mpLeftHandShape = mpModel->getModelData()->getMaterialNodePointer(6)->getShape();

            if (mpRightHandShape != NULL) {
                mpRightHandShape->hide();
            }
            mpRightHandShape = mpModel->getModelData()->getMaterialNodePointer(7)->getShape();
            mBtpHeap.initData();
            mBtkHeap.initData();
        }
    } else {
        if (mpDemoBDTmpBmd != NULL && mpModel == mpDemoBDTmpBmd) {
            if (mpLeftHandShape != NULL) {
                mpLeftHandShape->hide();
            }

            if (mpRightHandShape != NULL) {
                mpRightHandShape->hide();
            }

            mpModel = NULL;
            mpMaskBmd = NULL;
            mpHandsBmd = NULL;
            mpHairhandBmd = NULL;
            mpLeftHandShape = NULL;
            mpRightHandShape = NULL;
            mBtpHeap.initData();
            mBtkHeap.initData();
        }
        
        J3DModel* midna_model = link->getMidnaModel();
        if (mpModel != NULL) {
            if (midna_model == NULL && !checkStateFlg1(FLG1_UNK_1)) {
                mpModel = NULL;
                mpMaskBmd = NULL;
                mpHandsBmd = NULL;
                mpHairhandBmd = NULL;
                mpLeftHandShape = NULL;
                mpRightHandShape = NULL;
            }
        } else if (midna_model != NULL) {
            mpModel = midna_model;
            mpMaskBmd = link->getMidnaMaskModel();
            mpHandsBmd = link->getMidnaHandModel();
            mpHairhandBmd = link->getMidnaHairHandModel();
            J3DModelData* modelData = mpModel->getModelData();
            mpLeftHandShape = modelData->getMaterialNodePointer(6)->getShape();
            mpRightHandShape = modelData->getMaterialNodePointer(7)->getShape();
            modelData->getMaterialNodePointer(2)->setMaterialAnm(mpEyeMatAnm[0]);
            modelData->getMaterialNodePointer(3)->setMaterialAnm(mpEyeMatAnm[1]);
            mpEyeMatAnm[0]->init();
            mpEyeMatAnm[1]->init();
            modelData = mpHandsBmd->getModelData();
            for (u16 i = 0; i < 4; i++) {
                modelData->getMaterialNodePointer(i)->getShape()->hide();
            }
            mBtpHeap.initData();
            mBtkHeap.initData();
        }
    }
}

static void* daMidna_searchNpc(fopAc_ac_c* i_actor, void* o_far) {
    daAlink_c* link = daAlink_getAlinkActorClass();
    cXyz link_pos = link->current.pos;
    link_pos.y += 100.0f;

    if (
        fopAcM_GetGroup(i_actor) == fopAc_NPC_e && !fopAcM_CheckStatus(i_actor, 0x8000000) &&
        fabsf(link_pos.y - i_actor->eyePos.y) <= 700.0f
    ) {
        f32 dist = i_actor->eyePos.absXZ(link_pos);
        if (dist <= link->getMetamorphoseNearDis()) {
            *(BOOL*)o_far = FALSE;
        } else if (
            dist <= link->getMetamorphoseFarDis()
            && fopAcM_seenPlayerAngleY(i_actor) <= link->getMetamorphoseFarAngle()
        ) {
            *(BOOL*)o_far = TRUE;
        } else {
            return NULL;
        }

        if (!fopAcM_lc_c::lineCheck(&i_actor->eyePos, &link_pos, i_actor)) {
            return i_actor;
        }
    }
    return NULL;
}

void daMidna_c::setMidnaNoDrawFlg() {
    if (
        (!checkStateFlg1((daMidna_FLG1)(FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE | FLG1_UNK_1)) &&
        (!checkAppear() || !checkMidnaRealBody())) || daAlink_c::checkCloudSea()
    ) {
        onStateFlg0(FLG0_NO_DRAW);
    } else if (
        !checkStateFlg1((daMidna_FLG1)(FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE | FLG1_UNK_1)) &&
        daAlink_getAlinkActorClass()->checkPlayerNoDraw() &&
        !checkStateFlg0((daMidna_FLG0)(FLG0_TAG_WAIT | FLG0_UNK_100))
    ) {
        onStateFlg0((daMidna_FLG0)(FLG0_UNK_10000 | FLG0_NO_DRAW));
    } else {
        if (
            checkStateFlg0(FLG0_NO_DRAW) &&
            !checkStateFlg1((daMidna_FLG1)(FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE | FLG1_UNK_1)) &&
            !checkStateFlg0(FLG0_UNK_10000)
        ) {
            onStateFlg0(FLG0_UNK_1000);
        }

        offStateFlg0((daMidna_FLG0)(FLG0_UNK_10000 | FLG0_NO_DRAW));
    }
}

BOOL daMidna_c::checkMetamorphoseEnableBase() {
    BOOL tmp;
    if (
        !daAlink_getAlinkActorClass()->checkMidnaRide() || (g_env_light.mEvilInitialized & 0x80) ||
        /* dSv_event_flag_c::M_077 - Main Event - Get shadow crystal (can now transform) */
        !dComIfGs_isEventBit(0xD04) ||
        fopAcIt_Judge((fopAcIt_JudgeFunc)daMidna_searchNpc, &tmp)
    ) {
        return FALSE;
    }
    return TRUE;
}

BOOL daMidna_c::checkNoDrawState() {
    return mDemoMode == 0xB || checkStateFlg0(FLG0_UNK_400000) ||
           checkStateFlg1(FLG1_UNK_20) || (checkStateFlg0(FLG0_NO_DRAW) && !checkShadowModelDrawSmode());
}

void daMidna_c::setSound() {
    static u32 const anmSoundLabel[4][2] = {
        Z2SE_MDN_HIP, Z2SE_MDN_HIP_WTR,
        Z2SE_MDN_HAND, Z2SE_MDN_HAND_WTR,
        Z2SE_MDN_LEG, Z2SE_MDN_LEG_WTR,
        Z2SE_MDN_LAND, Z2SE_MDN_LAND_WTR,
    };

    int idx = 4;
    if (mDemoMode == 0xB || checkStateFlg1(FLG1_UNK_20)
        || (checkStateFlg0(FLG0_NO_DRAW) && !checkStateFlg0(FLG0_UNK_10000)
            && !checkShadowModelDrawSmode()))
    {
        return;
    }

    if (mUpperBck.checkFrame(mVoiceFrame)) {
        mSound.startCreatureVoice(mSoundID, -1);
    }

    if (checkSetAnime(1, ANM_SWAITB)) {
        if (mUpperBck.checkFrame(83.0f)) {
            mSound.startCreatureVoice(Z2SE_MDN_V_SWAITB_3, -1);
        } else if (mUpperBck.checkFrame(24.0f) || mUpperBck.checkFrame(99.0f)) {
            idx = 0;
        }
    } else if (checkSetAnime(1, ANM_ROPEWAIT)) {
        if (mUpperBck.checkFrame(66.0f)) {
            mSound.startCreatureVoice(Z2SE_MDN_V_ROPEWAIT_2, -1);
        }
    } else if (checkSetAnime(1, ANM_CLINGST)) {
        if (mUpperBck.checkFrame(11.0f)) {
            idx = 0;
        }
    } else if (checkSetAnime(1, ANM_LEADTOWAITA)) {
        if (mUpperBck.checkFrame(2.0f)) {
            idx = 0;
        }
    } else if (checkSetAnime(1, ANM_HIT)) {
        if (mUpperBck.checkFrame(10.0f) || mUpperBck.checkFrame(11.0f)) {
            idx = 1;
        } else if (mUpperBck.checkFrame(28.0f) || mUpperBck.checkFrame(35.0f)) {
            idx = 2;
        }
    } else if (checkSetAnime(1, ANM_SWAITC)) {
        if (mUpperBck.checkFrame(2.0f)) {
            idx = 3;
        } else if (mUpperBck.checkFrame(67.0f)) {
            idx = 0;
        }
    } else if (checkSetAnime(1, ANM_DIGIN)) {
        if (mUpperBck.checkFrame(19.0f) || mUpperBck.checkFrame(24.0f)
                                        || mUpperBck.checkFrame(34.0f)) {
            idx = 1;
        }
    } else if (checkSetAnime(1, ANM_WAITTP)) {
        if (mUpperBck.checkFrame(9.0f) || mUpperBck.checkFrame(12.0f)) {
            idx = 1;
        }
    }

    if (idx != 4) {
        u32 sound_id = anmSoundLabel[idx][dComIfGp_checkPlayerStatus0(0, 0x100000) ? 1 : 0];
        mSound.startCreatureSound(sound_id, 0, -1);
    }

    if (checkStateFlg1(FLG1_UNK_800)) {
        mSound.updateAnime(mpMorf->getFrame(), mpMorf->getPlaySpeed());
    }

    if (checkMidnaTired() && !dComIfGp_checkPlayerStatus0(0, 0x20000000)) {
        mSound.startCreatureVoiceLevel(Z2SE_MDN_V_WAITD, -1);
    }
}

int daMidna_c::execute() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (!link->checkMetamorphose()) {
        if (daPy_py_c::checkNowWolf()) {
            onStateFlg0(FLG0_UNK_40000000);
        } else {
            offStateFlg0(FLG0_UNK_40000000);
        }
    }

    initMidnaModel();
    mPrevShapeAngleY = shape_angle.y;
    setDemoData();
    offStateFlg0(FLG0_NO_INPUT);
    if (checkStateFlg0(FLG0_TAG_WAIT) && (link->checkMidnaUseAbility()
                                         || checkSetAnime(0, ANM_LEADTOWAITA)
                                         || eventInfo.checkCommandTalk())) {
        offStateFlg0((daMidna_FLG0)(FLG0_PORTAL_OBJ_CALL | FLG0_TAG_WAIT | FLG0_UNK_200));
    }

    daMidna_matAnm_c::decMorfFrame();
    checkMidnaPosState();
    if (daPy_py_c::checkFirstMidnaDemo() && !checkStateFlg0(FLG0_WOLF_NO_POS)) {
        link->onMidnaRide();
    } else {
        link->offMidnaRide();
    }
    setMidnaNoDrawFlg();

    fopAc_ac_c* talk_partner = fopAcM_getTalkEventPartner(link);
    if (
        checkStateFlg0(FLG0_NO_DRAW) && (eventInfo.checkCommandTalk() || checkStateFlg0(FLG0_WOLF_NO_POS) ||
        mDemoMode == 2 || (dComIfGp_event_runCheck() && talk_partner != NULL && (checkStateFlg0(FLG0_UNK_2000000) ||
        fopAcM_GetName(talk_partner) == PROC_Tag_Mhint || fopAcM_GetName(talk_partner) == PROC_Tag_Mstop ||
        fopAcM_GetName(talk_partner) == PROC_Tag_Mwait)))
    ) {
        if (
            mDemoMode == 3 || mDemoMode == 0x10 || mDemoMode == 4 || mDemoMode == 6 ||
            mDemoMode == 7 || mDemoMode == 0xE || mDemoMode == 0x13 || mDemoMode == 8
        ) {
            field_0x84e = 3;
        } else if (field_0x84e == 0) {
            if (checkStateFlg0(FLG0_WOLF_NO_POS)) {
                field_0x84e = 3;
            } else {
                field_0x84e = 1;
                if (link->initShadowScaleLight()) {
                    onStateFlg0(FLG0_UNK_80000);
                } else {
                    offStateFlg0(FLG0_UNK_80000);
                }
            }
        } else if (field_0x84e == 1) {
            if (link->moveShadowScaleLight()) {
                field_0x84e = 2;
            }
        } else if (field_0x84e == 2) {
            link->moveShadowScaleLight();
            if (mpMorf->isStop()) {
                field_0x84e = 3;
            }
        } else if (field_0x84e == 3 && checkStateFlg0(FLG0_UNK_1000000)) {
            offStateFlg0(FLG0_UNK_1000000);
            field_0x84e = 4;
        } else if (field_0x84e == 4) {
            if (mpMorf->isStop()) {
                field_0x84e = 5;
            }
        } else if (field_0x84e == 5 && checkStateFlg0(FLG0_WOLF_NO_POS)) {
            field_0x84e = 3;
        }
    } else {
        field_0x84e = 0;
    }

    if (checkSetAnime(0, ANM_LEADTOWAITA) && mpMorf->checkFrame(2.0f) && daPy_py_c::checkNowWolf()
                                     && mpKago == NULL) {
        dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
    }
    allAnimePlay();
    setAnm();
    setHairAngle();
    setMatrix();
    mpMorf->modelCalc();
    setRoomInfo();
    setNeckAngle();
    if (checkEndResetStateFlg0(ERFLG0_UNK_20)) {
        setHairAngle();
    }
    setBodyPartMatrix();
    setBodyPartPos();
    mSound.framework(0, mReverb);

    if (eventInfo.checkCommandTalk()) {
        if (!checkShadowModeTalkWait() || fopAcM_getTalkEventPartner(link) == this) {
            if (!checkStateFlg0(FLG0_UNK_8000)) {
                offStateFlg0((daMidna_FLG0)(FLG0_NPC_NEAR | FLG0_NPC_FAR));
                BOOL far;
                if (fopAcIt_Judge((fopAcIt_JudgeFunc)daMidna_searchNpc, &far)) {
                    if (!far) {
                        onStateFlg0(FLG0_NPC_NEAR);
                    } else {
                        onStateFlg0(FLG0_NPC_FAR);
                    }
                }
                onStateFlg0(FLG0_UNK_8000);
                mMsgFlow.init(this, 0xbb9, 0, NULL);
            } else if (mMsgFlow.doFlow(this, NULL, 0)) {
                int item_id;
                u16 event_id = mMsgFlow.getEventId(&item_id);
                if (checkStateFlg0(FLG0_NO_DRAW)) {
                    if (field_0x84e == 3) {
                        field_0x84e = 4;
                    } else {
                        onStateFlg0(FLG0_UNK_1000000);
                    }
                }

                if (event_id == 4 || event_id == 5) {
                    dComIfGp_getEvent().reset(this);
                    offStateFlg0(FLG0_UNK_8000);
                    fopAcM_orderPotentialEvent(this, 0x400, 0xffff, 1);
                    link->changeOriginalDemo();
                    if (event_id == 4) {
                        link->changeDemoMode(0x3a, 0, 0, 0);
                        if (mpModel != NULL && !checkStateFlg0(FLG0_NO_DRAW)) {
                            changeOriginalDemo();
                            changeDemoMode(0xf);
                        }
                    } else {
                        link->changeDemoMode(0x39, 0, 0, 0);
                    }

                    if (checkStateFlg0(FLG0_NO_DRAW)) {
                        onStateFlg0(FLG0_UNK_2000000);
                    }
                } else if (
                    !checkStateFlg0(FLG0_NO_DRAW) || (checkSetAnime(0, ANM_S_RETURN) && mpMorf->isStop())
                ) {
                    if (event_id == 0xB) {
                        dMeter2Info_setPauseStatus(6);
                        link->onPortalWarpMidnaAtnKeep();
                    } else {
                        link->onMidnaTalkPolySpeed();
                    }

                    dComIfGp_getEvent().reset(this);
                    offStateFlg0(FLG0_UNK_8000);
                }
            }
        }
    } else if (eventInfo.checkCommandDemoAccrpt() && !link->checkMetamorphose()) {
        dComIfGp_getEvent().reset(this);
    }

    if (link->checkMidnaRide()) {
        eventInfo.onCondition(1);
    }

    if (mpModel != NULL && !checkStateFlg0(FLG0_NO_DRAW)) {
        cXyz pos;
        if (checkSetAnime(0, ANM_LEADST)) {
            mDoMtx_multVecZero(mpModel->getAnmMtx(JNT_BACKBONE1), &pos);
            mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0x8397, &pos, &tevStr, &shape_angle,
                                                NULL, 0xff, NULL, -1, NULL, NULL, 0);
            mDoMtx_multVecZero(mpModel->getAnmMtx(JNT_HAIR_1), &pos);
            mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0x8398, &pos, &tevStr, &shape_angle,
                                                NULL, 0xff, NULL, -1, NULL, NULL, 0);
        }
        
        if (checkStateFlg0(FLG0_UNK_400000)) {
            static s16 const throughEffectJoint[6] =
                { JNT_HEAD, JNT_BACKBONE2, JNT_WAIST, JNT_HEAD, JNT_BACKBONE2, JNT_WAIST };
            static u16 const throughEffectName[6] =
                { 0x84da, 0x84d9, 0x84db, 0x84dd, 0x84dc, 0x84de };
            int i;
            u32* particle_key = mThroughEffectKey;
            for (i = 0; i < 6; i++, particle_key++) {
                mDoMtx_multVecZero(mpModel->getAnmMtx(throughEffectJoint[i]), &pos);
                *particle_key = dComIfGp_particle_set(*particle_key, throughEffectName[i], &pos,
                                                      &tevStr, &shape_angle, NULL, 0xff, NULL, -1,
                                                      NULL, NULL, 0);
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(*particle_key);
                if (emitter != NULL && emitter->isEnableDeleteEmitter()) {
                    offStateFlg0(FLG0_UNK_400000);
                }
            }
        } else {
            for (int i = 0; i < 6; i++) {
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mThroughEffectKey[i]);
                if (emitter != NULL) {
                    emitter->stopDrawParticle();
                }
            }
        }
    } else {
        offStateFlg0(FLG0_UNK_400000);
    }

    mEndResetStateFlg0 = 0;
    link->clearMidnaMsgNum();
    setSound();
    return 1;
}

static int daMidna_Execute(daMidna_c* i_this) {
    return i_this->execute();
}

int daMidna_c::draw() {
    daAlink_c* link = daAlink_getAlinkActorClass();

    if (checkNoDrawState() ||
        (!checkStateFlg1((daMidna_FLG1)(FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE | FLG1_UNK_1))
            && daAlink_getAlinkActorClass()->checkPlayerNoDraw()
            && !checkStateFlg0((daMidna_FLG0)(FLG0_TAG_WAIT | FLG0_UNK_100))))
    {
        return 1;
    }

    int bvar1 = false;
    dComIfGd_setListDark();

    if (!checkStateFlg0(FLG0_NO_DRAW) && !checkStateFlg1(FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE) && mpModel != NULL) {
        g_env_light.settingTevStruct(3, &current.pos, &tevStr);
        if (checkSetAnime(0, ANM_RETURN)) {
            tevStr.TevColor.r = (mpMorf->getFrame() / mpMorf->getEndFrame()) * -32.0f;
            tevStr.TevColor.g = tevStr.TevColor.r;
            tevStr.TevColor.b = tevStr.TevColor.r;
        } else if (link->getIceDamageWaitTimer() != 0 || link->checkFreezeDamage() != 0) {
            tevStr.TevColor = link->tevStr.TevColor;
            field_0x6e0 = link->tevStr.TevColor;
            if (dKy_darkworld_check()) {
                field_0x6e8 = l_normalKColor;
                field_0x6ec = l_normalKColor2;
            } else {
                field_0x6e8 = l_lNormalKColor;
                field_0x6ec = l_lNormalKColor2;
            }
        }

        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

        #if DEBUG
        J3DGXColorS10 hairColor1;
        J3DGXColor hairKColor1;
        J3DGXColor hairKColor2;
        if (mpHIO->m.hio_priority) {
            hairColor1.r = mpHIO->m.hair_color_1r;
            hairColor1.g = mpHIO->m.hair_color_1g;
            hairColor1.g = mpHIO->m.hair_color_1g;
            hairColor1.a = 0;

            hairKColor1.r = mpHIO->m.hair_k_color_1r;
            hairKColor1.g = mpHIO->m.hair_k_color_1g;
            hairKColor1.b = mpHIO->m.hair_k_color_1b;
            hairKColor1.a = 0;

            hairKColor2.r = mpHIO->m.hair_k_color_2r;
            hairKColor2.g = mpHIO->m.hair_k_color_2g;
            hairKColor2.b = mpHIO->m.hair_k_color_2b;
            hairKColor2.a = 0;
        }
        #endif
        
        J3DMaterial* material = mpModel->getModelData()->getMaterialNodePointer(4);
        if (field_0x668 != NULL) {
            field_0x668->getTevColorReg(0, &field_0x6e0);
            field_0x668->getTevKonstReg(0, &field_0x6e8);
        }

        #if DEBUG
        if (mpHIO->m.hio_priority) {
            material->setTevColor(1, &hairColor1);
            material->setTevKColor(1, &hairKColor1);
        } else {
            material->setTevColor(1, &field_0x6e0);
            material->setTevKColor(1, &field_0x6e8);
        }
        #else
        material->setTevColor(1, &field_0x6e0);
        material->setTevKColor(1, &field_0x6e8);
        #endif

        mDoExt_modelEntryDL(mpModel);
        g_env_light.setLightTevColorType_MAJI(mpHandsBmd, &tevStr);
        mDoExt_modelEntryDL(mpHandsBmd);
        if (mpHairhandBmd != NULL && !checkStateFlg1((daMidna_FLG1)(FLG1_UNK_40 | FLG1_UNK_10))) {
            for (u16 i = 0; i < 3; i++) {
                J3DMaterial* material = mpHairhandBmd->getModelData()->getMaterialNodePointer(i);

                #if DEBUG
                if (mpHIO->m.hio_priority) {
                    material->setTevColor(1, &hairColor1);
                    material->setTevKColor(1, &hairKColor1);
                    material->setTevKColor(2, &hairKColor2);
                } else {
                    material->setTevColor(1, &field_0x6e0);
                    material->setTevKColor(1, &field_0x6e8);
                    material->setTevKColor(2, &field_0x6ec);
                }
                #else
                material->setTevColor(1, &field_0x6e0);
                material->setTevKColor(1, &field_0x6e8);
                material->setTevKColor(2, &field_0x6ec);
                #endif
            }
            g_env_light.setLightTevColorType_MAJI(mpHairhandBmd, &tevStr);
            mDoExt_modelEntryDL(mpHairhandBmd);
        }

        if (!checkStateFlg1(FLG1_NO_MASK_DRAW)) {
            g_env_light.setLightTevColorType_MAJI(mpMaskBmd, &tevStr);
            mDoExt_modelEntryDL(mpMaskBmd);
        }

        if (!link->checkMidnaRide()) {
            bvar1 = true;
        }
    } else {
        g_env_light.settingTevStruct(1, &current.pos, &tevStr);
        if (field_0x668 != NULL) {
            field_0x668->getTevColorReg(0, &tevStr.TevColor);
        }

        g_env_light.setLightTevColorType_MAJI(mpGokouBmd, &tevStr);
        mDoExt_modelEntryDL(mpGokouBmd);
        MtxP mtx = mpShadowModel->getAnmMtx(JNT_WORLD_ROOT);
        cXyz vec(mtx[0][3], mtx[1][3], mtx[2][3]);
        MTXMultVec(dComIfGd_getViewMtx(), &vec, &vec);
        vec.z -= 200.0f;
        MTXMultVec(dComIfGd_getInvViewMtx(), &vec, &vec);
        g_env_light.setLightTevColorType_MAJI(mpShadowModel, &tevStr);
        mInvModel.entryDL(&vec);
        if (!checkStateFlg1(FLG1_NO_MASK_DRAW)) {
            g_env_light.setLightTevColorType_MAJI(mpShadowMaskBmd, &tevStr);
            mMaskInvModel.entryDL(&vec);
        }

        g_env_light.setLightTevColorType_MAJI(mpShadowHandsBmd, &tevStr);
        mHandsInvModel.entryDL(&vec);
        if (!checkStateFlg1(FLG1_UNK_40)) {
            g_env_light.setLightTevColorType_MAJI(mpShadowHairhandBmd, &tevStr);
            mHairhandInvModel.entryDL(&vec);
        }
    }

    if (mLeftHandShapeIdx == 0xfd) {
        g_env_light.setLightTevColorType_MAJI(mpDemoHLTmpBmd, &tevStr);
        mDoExt_modelEntryDL(mpDemoHLTmpBmd);
    }

    if (mRightHandShapeIdx == 0xfd) {
        g_env_light.setLightTevColorType_MAJI(mpDemoHRTmpBmd, &tevStr);
        mDoExt_modelEntryDL(mpDemoHRTmpBmd);
    }

    if (checkStateFlg0(FLG0_UNK_10)) {
        g_env_light.setLightTevColorType_MAJI(mpDemoFCBlendBmd, &tevStr);
        if (mpDemoFCBlendBrk != NULL) {
            mpDemoFCBlendBmd->getModelData()->entryTevRegAnimator(mpDemoFCBlendBrk);
        }
        mDoExt_modelEntryDL(mpDemoFCBlendBmd);
        g_env_light.setLightTevColorType_MAJI(mpDemoFCTongueBmd, &tevStr);
        mDoExt_modelEntryDL(mpDemoFCTongueBmd);
    }

    if (!checkStateFlg1(FLG1_UNK_40) && checkStateFlg1(FLG1_UNK_10)) {
        g_env_light.setLightTevColorType_MAJI(mpDemoHDTmpBmd, &tevStr);
        mDoExt_modelEntryDL(mpDemoHDTmpBmd);
    }

    tevStr.TevColor.r = 0;
    tevStr.TevColor.g = 0;
    tevStr.TevColor.b = 0;
    dComIfGd_setList();

    #if DEBUG
    if (UREG_S(2) != 0) {
        const Vec src = {UREG_F(1), UREG_F(2), UREG_F(3)};
        cXyz work;

        if (UREG_S(2) == 100) {
            cMtx_multVecSR(mpShadowModel->getBaseTRMtx(), &src, &work);
            work += current.pos;
        } else if (UREG_S(2) == 101) {
            work = attention_info.position;
        } else if (UREG_S(2) == 102) {
            work = eyePos;
        } else if (UREG_S(2) >= 200 && UREG_S(2) - 200 < 29) {
            cMtx_multVec(mpShadowModel->getAnmMtx(UREG_S(2) - 200), &src, &work);
        } else {
            work = current.pos;
        }

        static GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
        dDbVw_drawSphereOpa(work, UREG_F(0) + 5.0f, color, 1);
    }
    #endif

    if (bvar1 && !checkStateFlg1(FLG1_SHADOW_NO_DRAW)) {
        cXyz vec;
        mDoMtx_multVecZero(mpModel->getAnmMtx(JNT_BACKBONE1), &vec);
        if (fopAcM_gc_c::gndCheck(&vec)) {
            mShadowKey = dComIfGd_setShadow(mShadowKey, 0, mpModel, &vec, 600.0f, 0.0f, vec.y - 40.0f,
                                    fopAcM_gc_c::getGroundY(), *fopAcM_gc_c::getGroundCheck(),
                                    &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
            if (!checkStateFlg1(FLG1_NO_MASK_DRAW)) {
                dComIfGd_addRealShadow(mShadowKey, mpMaskBmd);
            }

            if (!checkStateFlg1(FLG1_UNK_40)) {
                if (checkStateFlg1(FLG1_UNK_10)) {
                    dComIfGd_addRealShadow(mShadowKey, mpDemoHDTmpBmd);
                } else {
                    dComIfGd_addRealShadow(mShadowKey, mpHairhandBmd);
                }
            }
        }
    }

    return 1;
}

static int daMidna_Draw(daMidna_c* i_this) {
    return i_this->draw();
}

daMidna_c::~daMidna_c() {
    #if DEBUG
    if (mpHIO != NULL) {
        mDoHIO_DELETE_CHILD(mpHIO->id);
    }
    #endif

    dComIfG_resDelete(&mPhase, l_arcName);
    daPy_py_c::setMidnaActor(NULL);
    mSound.deleteObject();
}

static int daMidna_Delete(daMidna_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    i_this->~daMidna_c();
    return 1;
}

// FAKEMATCH to get function ordering matching in retail
static void dummy() {
    ((J3DTexNoAnm*)NULL)->calc(NULL);
}
template unsigned char cLib_calcTimer<unsigned char>(unsigned char*);

static actor_method_class l_daMidna_Method = {
    (process_method_func)daMidna_Create,
    (process_method_func)daMidna_Delete,
    (process_method_func)daMidna_Execute,
    (process_method_func)NULL,
    (process_method_func)daMidna_Draw,
};

extern actor_process_profile_definition g_profile_MIDNA = {
    fpcLy_CURRENT_e,
    6,
    fpcPi_CURRENT_e,
    PROC_MIDNA,
    &g_fpcLf_Method.base,
    sizeof(daMidna_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x5C,
    &l_daMidna_Method,
    0x60000,
    fopAc_ENV_e,
    fopAc_CULLBOX_0_e,
};
