/**
 * d_a_itembase.cpp
 * Item Actor base
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/actor/d_a_itembase.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "f_op/f_op_actor_mng.h"

int daItemBase_c::DeleteBase(const char* i_resName) {
    dComIfG_resDelete(&mPhase, i_resName);
    return 1;
}

int daItemBase_c::clothCreate() {
    return 1;
}

int daItemBase_c::__CreateHeap() {
    return 1;
}

int daItemBase_c::CreateItemHeap(char const* i_arcName, s16 i_bmdName, s16 i_btkName, s16 i_bpkName,
                                 s16 i_bckName, s16 i_bxaName, s16 i_brkName, s16 i_btpName) {
    JUT_ASSERT(0, 0 <= m_itemNo && m_itemNo <= 255);

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_arcName, i_bmdName);
    JUT_ASSERT(0, modelData != NULL);

    u32 flags = 0x11000084;
    u32 modelflags = 0x80000;

    if (i_btkName > 0) {
        flags |= 0x200;
    }

    if (i_bpkName > 0) {
        flags |= 0x1;
    }

    if (i_btpName > 0) {
        flags |= 0x20000;
    }

    if (chkFlag(4)) {
        modelflags = 0;
    }

    mpModel = mDoExt_J3DModel__create(modelData, modelflags, flags);
    if (mpModel == NULL) {
        return 0;
    }

    mpBtkAnm = NULL;
    if (i_btkName > 0) {
        J3DAnmTextureSRTKey* pbtk =
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(i_arcName, i_btkName);
        JUT_ASSERT(0, pbtk != NULL);

        mpBtkAnm = new mDoExt_btkAnm();
        if (mpBtkAnm == NULL || !mpBtkAnm->init(modelData, pbtk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }

    mpBpkAnm = NULL;
    if (i_bpkName > 0) {
        J3DAnmColor* pbpk = (J3DAnmColor*)dComIfG_getObjectRes(i_arcName, i_bpkName);
        JUT_ASSERT(0, pbpk != NULL);

        mpBpkAnm = new mDoExt_bpkAnm();
        if (mpBpkAnm == NULL || !mpBpkAnm->init(modelData, pbpk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }

    mpBckAnm = NULL;
    if (i_bckName > 0) {
        J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(i_arcName, i_bckName);
        JUT_ASSERT(0, pbck != NULL);

        mpBckAnm = new mDoExt_bckAnm();
        if (mpBckAnm == NULL || !mpBckAnm->init(pbck, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
            return 0;
        }
    }

    mpBrkAnm = NULL;
    if (i_brkName > 0) {
        J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(i_arcName, i_brkName);
        JUT_ASSERT(0, pbrk != NULL);

        s8 tevFrm = getTevFrm();
        int anmPlay = TRUE;
        if (tevFrm != -1) {
            anmPlay = FALSE;
        }

        mpBrkAnm = new mDoExt_brkAnm();
        if (mpBrkAnm == NULL || !mpBrkAnm->init(modelData, pbrk, anmPlay, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }

    mpBtpAnm = NULL;
    if (i_btpName > 0) {
        J3DAnmTexPattern* pbtp = (J3DAnmTexPattern*)dComIfG_getObjectRes(i_arcName, i_btpName);
        JUT_ASSERT(0, pbtp != NULL);

        mpBtpAnm = new mDoExt_btpAnm();
        if (mpBtpAnm == NULL || !mpBtpAnm->init(modelData, pbtp, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }

    if (!clothCreate()) {
        return 0;
    }

    return __CreateHeap() ? TRUE : FALSE;
}

int daItemBase_c::DrawBase() {
    setTevStr();
    animEntry();
    setListStart();
    settingBeforeDraw();
    mDoExt_modelUpdateDL(mpModel);
    setListEnd();
    setShadow();
    return 1;
}

void daItemBase_c::RotateYBase() {
    shape_angle.y += (s16)(0xFFFF / getData().mRotateYSpeed);
}

void daItemBase_c::setListStart() {
    dComIfGd_setListDark();
}

void daItemBase_c::setListEnd() {
    dComIfGd_setList();
}

void daItemBase_c::settingBeforeDraw() {
    if (chkFlag(4)) {
        fopAcM_setEffectMtx(this, mpModel->getModelData());
    }
}

void daItemBase_c::setTevStr() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
}

void daItemBase_c::setShadow() {
    f32 size = scale.x * getShadowSize();

    if (!chkFlag(0x10)) {
        if (getShadowSize() != 0.0f) {
            dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), size, mAcch.m_gnd, 0, 1.0f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    } else {
        f32 shadowSize = getShadowSize();
        if (shadowSize == 0.0f) {
            shadowSize = 150.0f;
        }

        mShadowKey = dComIfGd_setShadow(mShadowKey, 3, mpModel, &current.pos, shadowSize, 0.0f,
                                        current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0,
                                        1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
}

void daItemBase_c::animEntry() {
    if (mpBrkAnm != NULL) {
        s8 tevFrm = getTevFrm();
        if (tevFrm != -1) {
            mpBrkAnm->entry(mpModel->getModelData(), tevFrm);
        } else {
            mpBrkAnm->entry(mpModel->getModelData());
        }
    }

    if (mpBtpAnm != NULL) {
        s8 btpFrm = getBtpFrm();
        if (btpFrm != -1) {
            mpBtpAnm->entry(mpModel->getModelData(), btpFrm);
        } else {
            mpBtpAnm->entry(mpModel->getModelData());
        }
    }

    if (mpBtkAnm != NULL) {
        mpBtkAnm->entry(mpModel->getModelData());
    }

    if (mpBckAnm != NULL) {
        mpBckAnm->entry(mpModel->getModelData());
    }

    if (mpBpkAnm != NULL) {
        mpBpkAnm->entry(mpModel->getModelData());
    }
}

void daItemBase_c::animPlay(f32 i_btkSpeed, f32 i_bpkSpeed, f32 i_bckSpeed, f32 param_3, f32 i_brkSpeed,
                            f32 i_btpSpeed) {
    if (mpBrkAnm != NULL && getTevFrm() == -1) {
        mpBrkAnm->setPlaySpeed(i_brkSpeed);
        mpBrkAnm->play();
    }

    if (mpBtpAnm != NULL && getBtpFrm() == -1) {
        mpBtpAnm->setPlaySpeed(i_btpSpeed);
        mpBtpAnm->play();
    }

    if (mpBtkAnm != NULL) {
        mpBtkAnm->setPlaySpeed(i_btkSpeed);
        mpBtkAnm->play();
    }

    if (mpBpkAnm != NULL) {
        mpBpkAnm->setPlaySpeed(i_bpkSpeed);
        mpBpkAnm->play();
    }

    if (mpBckAnm != NULL) {
        mpBckAnm->setPlaySpeed(i_bckSpeed);
        mpBckAnm->play();
    }
}

BOOL daItemBase_c::chkFlag(int i_flag) {
    return dItem_data::chkFlag(m_itemNo, i_flag);
}

s8 daItemBase_c::getTevFrm() {
    return dItem_data::getTevFrm(m_itemNo);
}

s8 daItemBase_c::getBtpFrm() {
    return dItem_data::getBtpFrm(m_itemNo);
}

u8 daItemBase_c::getShadowSize() {
    return dItem_data::getShadowSize(m_itemNo);
}

u8 daItemBase_c::getCollisionH() {
    return dItem_data::getH(m_itemNo);
}

u8 daItemBase_c::getCollisionR() {
    return dItem_data::getR(m_itemNo);
}
