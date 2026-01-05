/**
 * d_a_alink_wolf.inc
 * General Player Wolf action handling
 */

#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_e_pm.h"
#include "d/actor/d_a_e_yc.h"
#include "d/actor/d_a_e_ymb.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_kago.h"
#include "d/actor/d_a_kytag03.h"
#include "d/actor/d_a_npc_gwolf.h"
#include "d/actor/d_a_npc_moi.h"
#include "d/actor/d_a_npc_tk.h"
#include "d/actor/d_a_obj_crope.h"
#include "d/actor/d_a_obj_smw_stone.h"
#include "d/actor/d_a_obj_wchain.h"
#include "d/actor/d_a_obj_wind_stone.h"
#include "d/actor/d_a_obj_yobikusa.h"
#include "d/actor/d_a_tag_howl.h"
#include "d/actor/d_a_tag_wara_howl.h"
#include "d/actor/d_a_tag_wljump.h"
#include "d/actor/d_a_obj_lv4digsand.h"
#include "d/actor/d_a_obj_digplace.h"
#include "d/actor/d_a_obj_digsnow.h"
#include "d/actor/d_a_obj_digholl.h"
#include "d/actor/d_a_obj_sekizoa.h"
#include "d/actor/d_a_midna.h"
#include "d/d_com_inf_game.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_kankyo_mng.h"
#include "m_Do/m_Do_controller_pad.h"

// Forward declarations (for clangd)
int daAlink_kandelaarModelCallBack(J3DJoint* param_0, int param_1);
extern const char l_wArcName[];
extern const char l_bArcName[];
extern const char l_kArcName[];
extern const char l_zArcName[];
extern const char l_mArcName[];
extern const char l_hyShdArcName[];
extern const char l_cWShdArcName[];
extern const char l_sWShdArcName[];
extern const char l_arcName[];
extern dJntColData_c l_jntColData[];
extern dJntColData_c l_wolfJntColData[];
extern f32 l_autoUpHeight;
extern f32 l_autoDownHeight;
extern f32 const l_crawlStartFrontOffset[];
extern Vec const l_crawlFrontOffset;
extern Vec const l_crawlBackOffset;
extern Vec const l_crawlTopOffset;
extern Vec const l_crawlFrontUpOffset;
extern Vec const l_wolfLieFrontUpOffset;
extern Vec const l_crawlBackUpOffset;
extern Vec const l_wolfLieBackUpOffset;
extern Vec const l_crawlStandUpOffset;
extern Vec const l_wolfLieStandUpOffset;
extern Vec const l_crawlTopUpOffset;
extern Vec const l_wolfLieTopUpOffset;
extern Vec const l_crawlSideOffset;
extern Vec const l_wolfLieSideOffset;
extern Vec const l_crawlMinSideOffset;
extern f32 const l_wolfFootOnFrame[][4];
extern cXyz l_wolfBaseAnime;
extern cXyz l_wolfRopeBaseAnime;


void daAlink_c::changeCommon() {
    fopAcM_SetMtx(this, mpLinkModel->getBaseTRMtx());
    model = mpLinkModel;

    field_0x2060->offOldFrameFlg();
    field_0x2f8c = 0;
    resetUpperAnime(UPPER_2, -1.0f);

    offNoResetFlg1(FLG1_UNK_20000000);
    offKandelaarModel();

    int i;
    for (i = 0; i < 2; i++) {
        mFootData1[i].field_0x2 = 0;
        mFootData1[i].field_0x4 = 0;
        mFootData1[i].field_0x6 = 0;
        mFootData2[i].field_0x2 = 0;
        mFootData2[i].field_0x4 = 0;
        mFootData2[i].field_0x6 = 0;
    }

    for (i = 0; i < 10; i++) {
        field_0x302c[i] = 0;
        field_0x3040[i] = 0;
    }

    for (i = 0; i < 3; i++) {
        field_0x3054[i] = 0;
        field_0x305a[i] = 0;
    }

    shape_angle.z = 0;
    field_0x2fec = 0;
    field_0x2fee = 0;

    mBodyAngle.set(0, 0, 0);
    field_0x3080 = 0;
    field_0x3082 = 0;

    offNoResetFlg1(daPy_FLG1(FLG1_DASH_MODE | FLG1_UNK_200000 | FLG1_UNK_80));
    offNoResetFlg2(FLG2_UNK_8000000);

    field_0x2180[0]->init();
    field_0x2180[1]->init();

    mWolfEyeUpTimer = 0;
    resetCombo(1);

    i = 0;
    mItemTrigger = 0;
    mItemButton = 0;
    field_0x2ba4 = 0.0f;
    field_0x33a0 = 0.0f;
    field_0x2fcc = 0;

    field_0x32a0[0].r = 0;
    field_0x32a0[0].g = 0;
    field_0x32a0[0].b = 0;
    field_0x32a0[1].r = 0;
    field_0x32a0[1].g = 0;
    field_0x32a0[1].b = 0;

    field_0x2fc4 = 0;
    mHotspringRecoverTimer = mpHIO->mBasic.m.mHotspringRecoverTime;
    field_0x2fc9 = 0x10;

    for (; i < 4; i++) {
        clearFirePointDamageEffect(i);
    }

    field_0x30d0 = 0;
    field_0x30d2 = 0;
    field_0x2fc3 = 10;
}

void daAlink_c::changeWolf() {
    JKRHeap* current_heap = mAnmHeap3.setAnimeHeap();

    mpLinkModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_wArcName, 14)), 0x20200);

    J3DModelData* chainModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_wArcName, 15));
    for (u16 i = 0; i < 4; i++) {
        mpWlChainModels[i] = initModel(chainModelData, 0);
    }

    mpWlMidnaModel = initModelEnv(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_wArcName, 10)), 0x1020200);
    mpWlMidnaMaskModel =
        initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_wArcName, 7)), 0x1000000);
    mpWlMidnaHandModel =
        initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_wArcName, 6)), 0x1000000);
    mpWlMidnaHairModel =
        initModelEnv(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_wArcName, 11)), 0x1000000);

    mpDMidnaBrk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_wArcName, 18));
    mpDMidnaBrk->searchUpdateMaterialID(mpWlMidnaModel->getModelData());
    mpWlMidnaModel->getModelData()->entryTevRegAnimator(mpDMidnaBrk);
    mpDMidnaBrk->setFrame(1.0f);

    mpDMidnaMaskBrk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_wArcName, 21));
    mpDMidnaMaskBrk->searchUpdateMaterialID(mpWlMidnaMaskModel->getModelData());
    mpWlMidnaMaskModel->getModelData()->entryTevRegAnimator(mpDMidnaMaskBrk);
    mpDMidnaMaskBrk->setFrame(1.0f);

    mpDMidnaHandBrk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_wArcName, 20));
    mpDMidnaHandBrk->searchUpdateMaterialID(mpWlMidnaHandModel->getModelData());
    mpWlMidnaHandModel->getModelData()->entryTevRegAnimator(mpDMidnaHandBrk);
    mpDMidnaHandBrk->setFrame(1.0f);

    mpDMidnaHairHandBrk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_wArcName, 19));
    mpDMidnaHairHandBrk->searchUpdateMaterialID(mpWlMidnaHairModel->getModelData());
    mpWlMidnaHairModel->getModelData()->entryTevRegAnimator(mpDMidnaHairHandBrk);
    mpDMidnaHairHandBrk->setFrame(1.0f);

    mDoExt_setCurrentHeap(current_heap);

    dComIfGs_setTransformStatus(1);
    mpLinkModel->setUserArea((uintptr_t)this);
    changeModelDataDirectWolf(0);
    mEyeHL1.entry(field_0x064C, "wl_eye_Hilight");
    mEyeHL2.entry(mpWlMidnaModel->getModelData(), "midona_eye_Hlight");
    field_0x064C->getMaterialNodePointer(4)->setMaterialAnm(field_0x2180[0]);
    field_0x064C->getMaterialNodePointer(5)->setMaterialAnm(field_0x2180[1]);
    field_0x06e8 = field_0x064C->getMaterialNodePointer(1)->getShape();

    changeCommon();
    onNoResetFlg1(FLG1_IS_WOLF);
    deleteEquipItem(FALSE, FALSE);
    field_0x2e44.init(this, l_wolfJntColData, mpLinkModel, 18);
    offNoResetFlg1(FLG1_UNK_800000);

    field_0x598 = 115.0f;
    mLinkAcch.SetRoofCrrHeight(field_0x598);
    field_0x594 = mpHIO->mWolf.mWlMove.m.mMaxSpeed;

    l_autoUpHeight = mpHIO->mWolf.mWlWallHang.m.mAutoWalkHeight + 0.01f;
    l_autoDownHeight = -l_autoUpHeight;
    field_0x3470 = cM_scos(cM_deg2s(mpHIO->mWolf.mWlSlide.m.mSlidingAngle));
    field_0x3122 = cM_deg2s(mpHIO->mWolf.mWlSlide.m.mClimbingAngle);
    field_0x3458 = -60.0f;
    field_0x345c = -130.0f;

    field_0x2e44.offPassNum(16);
    field_0x2e44.offPassNum(15);

    offNoResetFlg0(FLG0_UNDERWATER);
    setHeavyBoots(0);

    for (int i = 0; i < 3; i++) {
        field_0x3094[i] = 0;
        field_0x309a[i] = 0;
    }

    mZ2Link.setLinkState(1);
    field_0x30d6 = 0;
    field_0x2fa6 = 0;
    field_0x2e54.changeWaterOffset(mpHIO->mBasic.m.mWolfWaterSurfaceEffectHeight);
    field_0x2e54.changeRippleOffset(field_0x598);
    mWoodSwordModel = NULL;
    field_0x3420 = 1.25f;
    field_0x30c6 = 40;
    field_0x30a8 = 3;
    field_0x30aa = 15;
    field_0x30ae = 0x1000;
    field_0x3424 = 8.0f;
    field_0x3428 = 50.0f;
    field_0x2f50 = &l_wolfLieSideOffset;
    field_0x2f54 = &l_wolfLieTopUpOffset;
    m_swordBlur.field_0x14 = 0;
    resetAtCollision(0);
    field_0x30b8 = 19;
    field_0x30ba = 24;
    field_0x30c0 = 19;
    field_0x30c2 = 24;
    field_0x30c4 = 3;
    field_0x30b4 = 4;
    field_0x30bc = 31;
    field_0x30be = 36;
    field_0x32c4[0] = 1;
    field_0x32c4[1] = 2;
    field_0x30b6 = 2;
    attention_info.field_0xa = 50;

    dCcD_Cyl* cyl_p = field_0x850;
    for (int i = 0; i < 3; i++, cyl_p++) {
        cyl_p->OffTgShieldFrontRange();
        cyl_p->OffTgSmallShield();
        cyl_p->OffTgSpShield();
        cyl_p->OffTgShield();
    }

    field_0xFB8.StartCAt(current.pos);
    field_0xFB8.OffAtSetBit();
    field_0xFB8.OnTgSetBit();
    field_0xFB8.OnCoSetBit();
    field_0xFB8.SetR(40.0f);

    mpKanteraModel = NULL;
    mpLinkHatModel = NULL;
    mpLinkFaceModel = NULL;
    mpLinkHandModel = NULL;
    mMagicArmorBodyBrk = NULL;
    mMagicArmorHeadBrk = NULL;

    field_0x2fc7 = (u8)dStage_stagInfo_GetWolfDashType(dComIfGp_getStage()->getStagInfo());
}

void daAlink_c::changeLink(int param_0) {
    JKRHeap* current_heap = mAnmHeap3.setAnimeHeap();
    offNoResetFlg2(daPy_FLG2(FLG2_UNK_100000 | FLG2_UNK_80000));
    mMagicArmorBodyBrk = NULL;
    mMagicArmorHeadBrk = NULL;

    J3DModelData* tempModelData_r28;

    if (checkNoResetFlg2(FLG2_UNK_200000)) {
        offNoResetFlg2(FLG2_UNK_200000);
        onNoResetFlg2(FLG2_UNK_80000);

        tempModelData_r28 = static_cast<J3DModelData*>(dComIfG_getObjectRes("alSumou", 0x31));
        mpLinkModel = mDoExt_J3DModel__create(tempModelData_r28,
                                              0x80000, 0x11000084);
        mpLinkHatModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes("alSumou", 0x33)), 0);
        mpLinkHandModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes("alSumou", 0x32)), 0);
    } else if (checkCasualWearFlg()) {
        onNoResetFlg2(FLG2_UNK_100000);

        mpLinkModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_bArcName, "bl.bmd")), 0);
        mpLinkHatModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_bArcName, "bl_head.bmd")), 0);
        mpLinkHandModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_bArcName, "bl_hands.bmd")), 0);
    } else if (checkZoraWearFlg()) {
        mpLinkModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_zArcName, "zl.bmd")), 0);
        mpLinkHatModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_zArcName, "zl_head.bmd")), 0);
        mpLinkHandModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_zArcName, "al_hands.bmd")), 0);
    } else if (checkMagicArmorWearFlg()) {
        mpLinkModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_mArcName, "ml.bmd")), 0x1000000);
        mpLinkHatModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_mArcName, "ml_head.bmd")), 0x1000000);
        mpLinkHandModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_mArcName, "al_hands.bmd")), 0);

        if (dComIfGs_getRupee() != 0) {
            setMagicArmorBrk(1);
        } else {
            setMagicArmorBrk(0);
        }

        mMagicArmorBodyBrk->setFrame(mMagicArmorBodyBrk->getFrameMax());
        mMagicArmorHeadBrk->setFrame(mMagicArmorHeadBrk->getFrameMax());
    } else {
        mpLinkModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_kArcName, "al.bmd")), 0);
        mpLinkHatModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_kArcName, "al_head.bmd")), 0);
        mpLinkHandModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_kArcName, "al_hands.bmd")), 0);
    }

    if (checkNoResetFlg2(FLG2_UNK_200000) || !checkZoraWearFlg()) {
        mpLinkFaceModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(mArcName, "al_face.bmd")), 0x20200);
    } else {
        mpLinkFaceModel =
            initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(mArcName, "zl_face.bmd")), 0x20200);
    }

    tempModelData_r28 = static_cast<J3DModelData*>(dComIfG_getObjectRes(mArcName, "al_bootsH.bmd"));
    u16 i;
    for (i = 0; i < 2; i++) {
        mpLinkBootModels[i] = initModel(tempModelData_r28, 0);
    }

    mpKanteraModel = initModelEnv(static_cast<J3DModelData*>(dComIfG_getObjectRes(mArcName, "al_kantera.bmd")), 0);

    tempModelData_r28 =
        static_cast<J3DModelData*>(dComIfG_getObjectRes(mArcName, "ef_ktGlow.bmd"));
    mpKanteraGlowModel = initModel(tempModelData_r28, 0x200);

    mpKanteraGlowBtk = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(mArcName, "ef_ktGlow.btk"));
    mpKanteraGlowBtk->setFrame(0.0f);
    mpKanteraGlowBtk->searchUpdateMaterialID(tempModelData_r28);
    tempModelData_r28->entryTexMtxAnimator(mpKanteraGlowBtk);

    mWoodSwordModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(mArcName, "al_SWB.bmd")), 0);

    mDoExt_setCurrentHeap(current_heap);

    dComIfGs_setTransformStatus(0);
    mpLinkModel->setUserArea((uintptr_t)this);
    mpLinkHatModel->setUserArea((uintptr_t)this);
    changeModelDataDirect(param_0);
    field_0x06e4 = NULL;

    u8 var_r27;
    if (checkNoResetFlg2(FLG2_UNK_80000)) {
        field_0x06d8 = field_0x064C->getMaterialNodePointer(4)->getShape();
        field_0x06dc = field_0x064C->getMaterialNodePointer(5)->getShape();
        field_0x06e0 = NULL;
        field_0x06e8 = field_0x064C->getMaterialNodePointer(3)->getShape();
        field_0x06ec = NULL;
        field_0x06f0 = field_0x064C->getMaterialNodePointer(2)->getShape();
        var_r27 = 2;
    } else if (checkCasualWearFlg()) {
        field_0x06d8 = field_0x064C->getMaterialNodePointer(3)->getShape();
        field_0x06dc = field_0x064C->getMaterialNodePointer(4)->getShape();
        field_0x06e0 = NULL;
        field_0x06e8 = field_0x064C->getMaterialNodePointer(2)->getShape();
        field_0x06ec = field_0x064C->getMaterialNodePointer(0)->getShape();
        field_0x06f0 = field_0x064C->getMaterialNodePointer(1)->getShape();
        var_r27 = 2;
    } else if (checkZoraWearFlg()) {
        field_0x06d8 = field_0x064C->getMaterialNodePointer(4)->getShape();
        field_0x06dc = field_0x064C->getMaterialNodePointer(5)->getShape();
        field_0x06e0 = field_0x064C->getMaterialNodePointer(8)->getShape();
        field_0x06e4 = field_0x064C->getMaterialNodePointer(9)->getShape();
        field_0x06e4->hide();
        field_0x06e8 = NULL;
        field_0x06ec = field_0x064C->getMaterialNodePointer(2)->getShape();
        field_0x06f0 = field_0x064C->getMaterialNodePointer(11)->getShape();
        var_r27 = 3;
    } else if (checkMagicArmorWearFlg()) {
        field_0x06d8 = field_0x064C->getMaterialNodePointer(4)->getShape();
        field_0x06dc = field_0x064C->getMaterialNodePointer(5)->getShape();
        field_0x06e0 = field_0x064C->getMaterialNodePointer(10)->getShape();
        field_0x06e8 = field_0x064C->getMaterialNodePointer(3)->getShape();
        field_0x06ec = field_0x064C->getMaterialNodePointer(1)->getShape();
        field_0x06f0 = field_0x064C->getMaterialNodePointer(2)->getShape();

        if (dComIfGs_getRupee() != 0) {
            var_r27 = 4;
        } else {
            var_r27 = 5;
        }
    } else {
        field_0x064C->getMaterialNodePointer(16)->getShape()->hide();
        field_0x06d8 = field_0x064C->getMaterialNodePointer(11)->getShape();
        field_0x06dc = field_0x064C->getMaterialNodePointer(12)->getShape();
        field_0x06e0 = field_0x064C->getMaterialNodePointer(6)->getShape();
        field_0x06e8 = field_0x064C->getMaterialNodePointer(8)->getShape();
        field_0x06ec = field_0x064C->getMaterialNodePointer(4)->getShape();
        field_0x06f0 = field_0x064C->getMaterialNodePointer(7)->getShape();
        var_r27 = 0;
    }

    if (field_0x06e0 != NULL) {
        if (checkEquipHeavyBoots()) {
            field_0x06e0->hide();
        } else {
            field_0x06e0->show();

            if (checkZoraWearMaskDraw()) {
                field_0x06e4->show();
            }
        }
    }

    field_0x06d0 = field_0x06d8;
    field_0x06d4 = field_0x06dc;

    tempModelData_r28 = mpLinkHandModel->getModelData();
    for (i = 0; i < 11; i++) {
        tempModelData_r28->getMaterialNodePointer(i)->getShape()->hide();
    }

    tempModelData_r28 = mpLinkFaceModel->getModelData();
    tempModelData_r28->getMaterialNodePointer(2)->setMaterialAnm(field_0x2180[0]);
    tempModelData_r28->getMaterialNodePointer(3)->setMaterialAnm(field_0x2180[1]);
    mEyeHL1.entry(tempModelData_r28, "highlight02");

    mpKanteraModel->setUserArea((uintptr_t)this);
    mpKanteraModel->getModelData()->getJointNodePointer(1)->setCallBack(
        daAlink_kandelaarModelCallBack);
    mZ2Link.setLinkState(var_r27);

#if VERSION != VERSION_SHIELD_DEBUG
    if (checkHookshotReadyMaterialOffMode()) {
        setHookshotReadyMaterial();
    }
#endif

    if (param_0) {
        fopAcM_SetMtx(this, mpLinkModel->getBaseTRMtx());
        model = mpLinkModel;
        field_0x2060->offOldFrameFlg();
    } else {
        changeCommon();
        offNoResetFlg1(FLG1_IS_WOLF);
        field_0x2e44.init(this, l_jntColData, mpLinkModel, 18);
        field_0x3088 = 0;
        field_0x302c[7] = -0x3800;
        field_0x3060 = 0;
        field_0x3062 = shape_angle.y;

        field_0x598 = 180.0f;
        mLinkAcch.SetRoofCrrHeight(field_0x598);
        field_0x594 = mpHIO->mMove.m.mMaxSpeed;

        l_autoUpHeight = mpHIO->mWallHang.m.auto_walk_height + 0.01f;
        l_autoDownHeight = -l_autoUpHeight;
        field_0x3470 = cM_scos(cM_deg2s(mpHIO->mSlide.m.mSlideAngle));
        field_0x3122 = cM_deg2s(mpHIO->mSlide.m.mClimbAngle);
        field_0x3458 = -120.0f;
        field_0x345c = -200.0f;

        field_0x2e54.changeWaterOffset(mpHIO->mBasic.m.mWaterSurfaceEffectHeight);
        field_0x2e54.changeRippleOffset(field_0x598);
        offWolfEyeUp();

        field_0x3420 = 1.0f;
        field_0x30c6 = 35;
        field_0x30a8 = 1;
        field_0x30aa = 16;
        field_0x30ae = 0x2000;
        field_0x3424 = 4.0f;
        field_0x3428 = 35.0f;
        field_0x2f50 = &l_crawlSideOffset;
        field_0x2f54 = &l_crawlTopUpOffset;

        resetWolfAtCollision();
        resetWolfBallGrab();
        deleteEquipItem(FALSE, FALSE);
        field_0x2fb0 = 0;

        for (int i = 0; i < 10; i++) {
            mWolfLockAcKeep[i].clearData();
        }

        mWolfLockNum = 0;
        field_0x30b8 = 9;
        field_0x30ba = 14;
        field_0x30c0 = 10;
        field_0x30c2 = 15;
        field_0x30c4 = 3;
        field_0x30b4 = 4;
        field_0x30bc = 21;
        field_0x30be = 26;
        field_0x30b6 = 5;
        field_0x32c4[0] = 2;
        field_0x32c4[1] = 16;
        attention_info.field_0xa = 10;

        dCcD_Cyl* cyl_p = field_0x850;
        for (int i = 0; i < 3; i++, cyl_p++) {
            cyl_p->OnTgShieldFrontRange();
            cyl_p->OnTgSmallShield();
        }

        mMagneBootsTopVec = cXyz::BaseY;

        field_0xFB8.OnAtSetBit();
        field_0xFB8.OffTgSetBit();
        field_0xFB8.OffCoSetBit();

        mpWlMidnaModel = NULL;
        mpWlMidnaMaskModel = NULL;
        mpWlMidnaHandModel = NULL;
        mpWlMidnaHairModel = NULL;
    }
}

void daAlink_c::onWolfEyeUp() {
    if (mWolfEyeUp == 0) {
        mZ2Link.setWolfEyeOpen(true);
        mWolfEyeUp = mpHIO->mWolf.m.mSensesLingerTime;
        field_0x2fd2 = 20;
    }
}

void daAlink_c::offWolfEyeUp() {
    if (mWolfEyeUp != 0) {
        mWolfEyeUp = 0;
        field_0x2fd2 = 20;
        mZ2Link.setWolfEyeOpen(false);
    }
}

BOOL daAlink_c::wolfSenseTrigger() {
    return itemTriggerCheck(BTN_X);
}

void daAlink_c::setWolfSenceStatus(u8 status) {
    dComIfGp_setXStatus(status, 0);
}

BOOL daAlink_c::wolfClawTrigger() {
    return itemTriggerCheck(BTN_Y);
}

void daAlink_c::setWolfDigStatus(u8 flag) {
    dComIfGp_setYStatus(13, flag);
}

BOOL daAlink_c::checkWolfShapeReverse() const {
    return ((mProcID == PROC_WOLF_SLIDE_READY || mProcID == PROC_WOLF_LOCK_ATTACK_TURN ||
             mProcID == PROC_WOLF_JUMP_AT_SLIDE_LAND) &&
            mProcVar2.field_0x300c != 0) ||
           mProcID == PROC_WOLF_SLIP_TURN_LAND ||
           (mProcID == PROC_WOLF_LARGE_DAMAGE_UP && field_0x3198 != 0) ||
           mProcID == PROC_WOLF_ROPE_TURN;
}

BOOL daAlink_c::checkWolfSideStep() const {
    return field_0x2f98 != 0 &&
           ((mProcID == PROC_WOLF_ATN_AC_MOVE && !checkModeFlg(1)) ||
            mProcID == PROC_WOLF_SIDESTEP_LAND || mProcID == PROC_WOLF_BACKJUMP_LAND);
}

void daAlink_c::setWolfTailAngle() {
    s16* var_r30 = field_0x3094;
    s16* var_r29 = field_0x309a;

    s16 var_r27 = (s16)(shape_angle.y - field_0x2fe6);

    for (int i = 0; i < 3; i++) {
        if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
            *var_r30 = 0;
            *var_r29 = 0;
        } else {
            *var_r30 -= var_r27;
            s16 sp38 = *var_r30;

            cLib_addCalcAngleS(var_r30, 0, 5, 500, 50);
            *var_r30 = cLib_minMaxLimit<s16>((s16)(*var_r30 + *var_r29), -0x1000, 0x1000);

            var_r27 = (s16)(*var_r30 - sp38);
            *var_r29 = var_r27 * 0.5f;
        }

        var_r30++;
        var_r29++;
    }
}

BOOL daAlink_c::checkWolfSlowDash() {
    return field_0x2fc7 == 1 || field_0x2fc7 == 3 || field_0x3174 == 11;
}

void daAlink_c::setSpeedAndAngleWolf() {
    f32 var_f31 = 0.0f;

    f32 var_f30;
    f32 var_f29;
    s16 var_r30;
    s16 var_r29;
    s16 var_r28;
    if (checkWolfDashMode()) {
        var_r30 = mpHIO->mWolf.mWlMove.m.mTurnAngleRate;
        var_r29 = mpHIO->mWolf.mWlMove.m.mTurnMax;
        var_r28 = mpHIO->mWolf.mWlMove.m.mTurnMin;
        if (checkWolfSlowDash()) {
            var_f30 = mpHIO->mWolf.mWlMove.m.mADashAccelerationSlow;
        } else if (field_0x2fc7 == 2) {
            var_f30 = mpHIO->mWolf.mWlMove.m.mADashAccelerationSlow2;
        } else {
            var_f30 = mpHIO->mWolf.mWlMove.m.mADashAcceleration;
        }
        var_f29 = mpHIO->mWolf.mWlMove.m.mDeceleration;
    } else {
        var_r30 = mpHIO->mWolf.mWlMoveNoP.m.mTurnAngleRate;
        var_r29 = mpHIO->mWolf.mWlMoveNoP.m.mTurnMax;
        var_r28 = mpHIO->mWolf.mWlMoveNoP.m.mTurnMin;
        var_f30 = mpHIO->mWolf.mWlMoveNoP.m.mAcceleration;
        var_f29 = mpHIO->mWolf.mWlMoveNoP.m.mDeceleration;
    }

    BOOL temp_r31 = checkEventRun();
    if (checkInputOnR()) {
        s16 temp_r4 = current.angle.y;
        BOOL var_r26 = 0;
        f32 temp_f28 = field_0x33a8 * field_0x33a8;
        f32 temp_f27 = speedF / field_0x594;

        if (cLib_distanceAngleS(field_0x2fe2, current.angle.y) > 0x7000 &&
            abs(field_0x3180) < 0x5000 &&
            ((!checkModeFlg(1) && !(speedF < 0.001f)) || field_0x33a8 > 0.85f))
        {
            if ((checkModeFlg(1) || speedF < 0.001f) && !temp_r31 &&
                (mProcID == PROC_WOLF_WAIT || mProcID == PROC_WOLF_MOVE))
            {
                cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, var_r30, var_r29, var_r28);
                if (mProcID == PROC_WOLF_MOVE && temp_f27 > 0.6f) {
                    var_r26 = 1;
                }
            } else if (mProcID == PROC_WOLF_MOVE &&
                       ((checkWolfDashMode() && temp_f27 > mpHIO->mWolf.mWlMove.m.mSlipStartRate) ||
                        (!checkWolfDashMode() && temp_f27 > mpHIO->mWolf.mWlMoveNoP.m.mSlideStartRate)))
            {
                return;
            } else {
                cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, var_r30, var_r29, var_r28);
                if (mProcID == PROC_WOLF_MOVE && temp_f27 > 0.6f) {
                    var_r26 = 1;
                }
            }
        } else {
            s16 var_r6;
            s16 var_r7;
            if (temp_r31 != 0) {
                var_r6 = var_r29;
                var_r7 = var_r28;
            } else {
                var_r6 = var_r29 * temp_f28;
                if (var_r6 < 10) {
                    var_r6 = 10;
                }

                var_r7 = var_r28 * temp_f28;
                if (var_r7 < 1) {
                    var_r7 = 1;
                }
            }

            cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, var_r30, var_r6, var_r7);
        }

        if (wolfSideBgCheck(temp_r4)) {
            current.angle.y = temp_r4;
            onEndResetFlg1(ERFLG1_UNK_200000);

            if (cLib_distanceAngleS(field_0x2fe2, current.angle.y) > 0x7000) {
                var_r26 = 1;
            }
        }

        if (!var_r26) {
            f32 temp_f27_2 = getStickAngleDistanceRate();
            if (temp_r31) {
                var_f31 = temp_f27_2 * (var_f30 * field_0x33a8);
            } else {
                f32 var_f26 = (1.0f - fabsf(mNormalSpeed / field_0x594)) * 0.3f;
                if (checkHeavyStateOn(1, 1) != 0) {
                    var_f26 *= mHeavySpeedMultiplier;
                }

                if ((!checkNoResetFlg1(FLG1_UNK_20000000) ||
                     cLib_distanceAngleS(current.angle.y + 0x8000, field_0x3092) >= 0xC00) &&
                    field_0x33a8 > var_f26)
                {
                    var_f31 = temp_f27_2 * (var_f30 * temp_f28);
                }
            }
        }
    }

    if (checkInputOnR() || (mProcID == PROC_WOLF_ROLL_ATTACK_MOVE)) {
        s16 temp_r26 = shape_angle.y;
        s16 var_r4;
        if (checkWolfShapeReverse()) {
            var_r4 = field_0x2fe2 + 0x8000;
        } else {
            var_r4 = field_0x2fe2;
        }

        cLib_addCalcAngleS(&shape_angle.y, var_r4, var_r30, var_r29 * 2, var_r28 * 2);

        int temp_r3 = (s16)(temp_r26 - current.angle.y) * (s16)(shape_angle.y - current.angle.y);
        if (temp_r3 <= 0 && temp_r3 >= -0x1000000) {
            shape_angle.y = current.angle.y;
        }
    } else if (temp_r31 == 0 && !checkInputOnR() && checkAttentionState() &&
               mProcID == PROC_WOLF_WAIT)
    {
        cLib_addCalcAngleS(&shape_angle.y, field_0x2fe4, 2, 0x2000, 0x800);
        current.angle.y = shape_angle.y;
    }

    setNormalSpeedF(var_f31, var_f29);
}

void daAlink_c::setSpeedAndAngleWolfAtn() {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    if (field_0x2f98 == 1) {
        var_f30 = mpHIO->mWolf.mWlAtnMove.m.mRearAcceleration;
        var_f29 = mpHIO->mWolf.mWlAtnMove.m.mRearDeceleration;
    } else {
        var_f30 = mpHIO->mWolf.mWlAtnMove.m.mAcceleration;
        var_f29 = mpHIO->mWolf.mWlAtnMove.m.mDeceleration;
    }

    s16 var_r29;
    if (checkFmChainGrabAnime()) {
        var_r29 = 1000;
    } else {
        var_r29 = mpHIO->mWolf.mWlAtnMove.m.mTurnAngleMax;
    }

    s16 var_r27;
    if (checkUnderMove0BckNoArcWolf(WANM_CHAIN_PULL_END)) {
        var_r27 = getChainStickAngleY(field_0x2fe4);
    } else {
        var_r27 = field_0x2fe2;
    }

    if (checkInputOnR()) {
        if (getDirectionFromCurrentAngle() == DIR_BACKWARD &&
            (field_0x2f98 == 0 || field_0x2f98 == 1))
        {
            current.angle.y += 0x8000;
            mNormalSpeed *= -1.0f;

            if (checkWolfAtnMoveBack((s16)(current.angle.y - shape_angle.y))) {
                field_0x2f98 = 1;
            } else {
                field_0x2f98 = 0;
            }
        }

        s16 temp_r28 = current.angle.y;
        cLib_addCalcAngleS(&current.angle.y, var_r27, mpHIO->mWolf.mWlAtnMove.m.mTurnAngleRate, var_r29,
                           mpHIO->mWolf.mWlAtnMove.m.mTurnAngleMin);

        if (wolfSideBgCheck(temp_r28)) {
            current.angle.y = temp_r28;
            onEndResetFlg1(ERFLG1_UNK_200000);
        }

        var_f31 = var_f30 * field_0x33a8 * cM_scos((s16)(current.angle.y - temp_r28));
    } else {
        if (checkZeroSpeedF()) {
            if (!setShapeAngleToAtnActor(0) && !checkEventRun() && !checkInputOnR() &&
                (mProcID == PROC_WOLF_WAIT || mProcID == PROC_WOLF_ROLL_ATTACK_MOVE ||
                 mProcID == PROC_WOLF_ATN_AC_MOVE))
            {
                cLib_addCalcAngleS(&shape_angle.y, field_0x2fe4, 2, 0x2000, 0x800);
            }

            if (field_0x2f98 == 1) {
                current.angle.y = shape_angle.y + 0x8000;
            } else {
                current.angle.y = shape_angle.y;
            }
        }

        var_f31 = 0.0f;
    }

    if (!checkWolfShapeReverse()) {
        s16 var_r26;
        if (field_0x2f98 == 1 || (checkEndResetFlg1(ERFLG1_UNK_200000) &&
                                  cLib_distanceAngleS(shape_angle.y, current.angle.y) > 0x4000))
        {
            var_r26 = current.angle.y + 0x8000;
        } else {
            var_r26 = current.angle.y;
        }

        cLib_addCalcAngleS(&shape_angle.y, var_r26, mpHIO->mWolf.mWlAtnMove.m.mTurnAngleRate,
                           var_r29 * 2, mpHIO->mWolf.mWlAtnMove.m.mTurnAngleMin * 2);
    }

    setNormalSpeedF(var_f31, var_f29);
}

BOOL daAlink_c::checkWolfAtnWait() {
    if ((checkEnemyGroup(mTargetedActor) || mDemo.getDemoMode() == 0x17) ||
        (mTargetedActor != NULL && fopAcM_GetName(mTargetedActor) == PROC_NPC_MOI &&
         ((daNpc_Moi_c*)mTargetedActor)->chkSFight()))
    {
        return true;
    }

    return false;
}

BOOL daAlink_c::checkUnderMove0BckNoArcWolf(daAlink_c::daAlink_WANM anm_id) const {
    return mUnderAnmHeap[0].checkNoSetArcNo() &&
           m_wlAnmDataTable[anm_id].field_0x0 == mUnderAnmHeap[0].getIdx();
}

void daAlink_c::setBlendWolfMoveAnime(f32 param_0) {
    f32 sp8;
    if (field_0x3174 == 8) {
        sp8 = 1.0f;
    } else {
        sp8 = cM_scos(field_0x2ff0);
    }

    f32 var_f31;
    f32 temp_f30 = fabsf(mNormalSpeed * sp8) / field_0x594;

    daPy_frameCtrl_c* sp38 = &mUnderFrameCtrl[0];
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[1];

    f32 sp34, sp30, sp2C, sp28, sp24, sp20, sp1C, sp18, sp14, sp10;
    if (checkWolfDashMode()) {
        sp20 = mpHIO->mWolf.mWlMove.m.mWalkToBriskWalkRate;
        sp14 = mpHIO->mWolf.mWlMove.m.mWalkRunInterpolation;
        sp1C = mpHIO->mWolf.mWlMove.m.mIdleToWalkRate;
        sp18 = mpHIO->mWolf.mWlMove.m.mStandbyRunToRunRate;
        sp34 = mpHIO->mWolf.mWlMove.m.mIdleAnmSpeed;
        sp30 = mpHIO->mWolf.mWlMove.m.mWalkAnmSpeed;
        sp2C = mpHIO->mWolf.mWlMove.m.mBriskWalkAnmSpeed;
        sp28 = mpHIO->mWolf.mWlMove.m.mRunAnmSpeed;
        sp24 = mpHIO->mWolf.mWlSlide.m.mClimbingAnmSpeed;
        sp10 = mpHIO->mWolf.mWlMove.m.mRunAnmMinBlendRate;
    } else {
        sp20 = mpHIO->mWolf.mWlMoveNoP.m.mWalkToJogRate;
        sp14 = mpHIO->mWolf.mWlMoveNoP.m.mWalkRunInterpolation;
        sp1C = mpHIO->mWolf.mWlMoveNoP.m.mIdleToWalkRate;
        sp18 = mpHIO->mWolf.mWlMoveNoP.m.mJogToRunRate;
        sp34 = mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed;
        sp30 = mpHIO->mWolf.mWlMoveNoP.m.mWalkAnmSpeed;
        sp2C = mpHIO->mWolf.mWlMoveNoP.m.mJogAnmSpeed;
        sp28 = mpHIO->mWolf.mWlMoveNoP.m.mRunAnmSpeed;
        sp24 = mpHIO->mWolf.mWlSlide.m.mClimbingAnmSpeedWeak;
        sp10 = mpHIO->mWolf.mWlMoveNoP.m.mRunAnmMinBlendRate;
    }

    if (temp_f30 < sp20 || checkAttentionState() || checkHeavyStateOn(1, 1) || checkSlope()) {
        field_0x2fc1 = 0;

        if ((temp_f30 < sp20 || checkAttentionState() || checkHeavyStateOn(1, 1)) &&
            field_0x30d0 != 0)
        {
            field_0x30d0 = 0;
            offNoResetFlg1(FLG1_DASH_MODE);
        }

        if (checkNoResetFlg1(FLG1_UNK_800000)) {
            offNoResetFlg1(FLG1_UNK_800000);
            param_0 = sp14;
        }

        int var_r29 = 1;
        daAlink_WANM var_r27 = WANM_WAIT;
        daAlink_WANM var_r26;
        daAlink_WANM var_r25;
        if (checkHeavyStateOn(1, 1)) {
            var_r26 = WANM_ATN_LOCK_WALK_FRONT;
            var_r25 = WANM_ATN_LOCK_WALK_FRONT;
            sp30 = 1.2f;
            sp2C = 1.2f;
        } else {
            var_r25 = WANM_WALK_B;
            var_r26 = WANM_WALK_A;
        }

        if (temp_f30 < sp1C) {
            if (checkModeFlg(1)) {
                var_r29 = 2;

                s16 temp_r0 = shape_angle.y - field_0x2fe6;
                if (abs(temp_r0) > 0 && !checkAttentionState()) {
                    var_f31 = 0.64999998f + (0.001f * abs(temp_r0));
                    if (var_f31 > 1.0f) {
                        var_f31 = 1.0f;
                    }
                    var_f31 = 1.0f - var_f31;

                    setDoubleAnimeWolf(var_f31, sp30, sp34, var_r26, WANM_WAIT, 2, param_0);
                    return;
                }

                if (checkAttentionState()) {
                    var_r27 = WANM_ATN;
                    sp34 = mpHIO->mWolf.mWlMove.m.mGazeIdleAnmSpeed;
                } else if (checkWindSpeedOnXZ() && !checkEventRun()) {
                    var_r27 = WANM_WAIT_WIND;
                    sp34 = 1.0f;
                }
            }

            if (checkSlope()) {
                var_r26 = WANM_WALK_SLOPE;
                var_r27 = WANM_WAIT;
                sp30 = sp24;
                field_0x2fc1 = 2;
            }

            var_f31 = temp_f30 / sp1C;
            setDoubleAnimeWolf(var_f31, sp34, sp30, var_r27, var_r26, var_r29, param_0);
        } else if (checkSlope()) {
            field_0x2fc1 = 2;
            setDoubleAnimeWolf(1.0f, sp24, sp24, WANM_WALK_SLOPE, WANM_WALK_SLOPE, 1, param_0);
            field_0x2fc1 = 2;
        } else {
            var_f31 = (temp_f30 - sp1C) / (sp20 - sp1C);
            if (var_f31 > 1.0f) {
                var_f31 = 1.0f;
            }
            setDoubleAnimeWolf(var_f31, sp30, sp2C, var_r26, var_r25, 1, param_0);
        }

        if (!checkBarkAnime() && var_r29 != 1) {
            checkAttentionState();
        }
        return;
    }

    if (!checkNoResetFlg1(FLG1_UNK_800000)) {
        onNoResetFlg1(FLG1_UNK_800000);
        param_0 = sp14;
    }

    field_0x2fc1 = 1;

    if (temp_f30 < sp18) {
        var_f31 = sp10 + (((1.0f - sp10) * (temp_f30 - sp20)) / (sp18 - sp20));
        setDoubleAnimeWolf(var_f31, sp34, sp28, WANM_WAIT, WANM_DASH_A, 1, param_0);
    } else {
        if (!checkWolfDashMode()) {
            setDoubleAnimeWolf(1.0f, sp28, sp28, WANM_DASH_A, WANM_DASH_A, 1, param_0);
        } else {
            var_f31 = (temp_f30 - sp18) / (mpHIO->mWolf.mWlMove.m.mRunToQuickRunRate - sp18);
            if (var_f31 > 1.0f) {
                var_f31 = 1.0f;
            }

            f32 spC;
            if (checkNoResetFlg1(FLG1_DASH_MODE)) {
                if (checkWolfSlowDash() != 0) {
                    spC = mpHIO->mWolf.mWlMove.m.mADashQuickRunASpeedSlow;
                } else if (field_0x2fc7 == 2) {
                    spC = mpHIO->mWolf.mWlMove.m.mADashQuickRunASpeedSlow2;
                } else {
                    spC = mpHIO->mWolf.mWlMove.m.mADashQuickRunAnmSpeed;
                }
            } else {
                spC = mpHIO->mWolf.mWlMove.m.mQuickRunAnmSpeed;
            }

            setDoubleAnimeWolf(var_f31, sp28, spC, WANM_DASH_A, WANM_DASH_B, 1, param_0);
        }

        if (temp_r28->checkPass(l_wolfFootOnFrame[1][0])) {
            field_0x2f9d = 8;
        } else if (temp_r28->checkPass(l_wolfFootOnFrame[1][1])) {
            field_0x2f9d = 0x10;
        } else if (temp_r28->checkPass(l_wolfFootOnFrame[1][2])) {
            field_0x2f9d = 0x20;
        } else if (temp_r28->checkPass(l_wolfFootOnFrame[1][3])) {
            field_0x2f9d = 0x40;
        }

        if (checkWindSpeedOnAngle()) {
            onEndResetFlg0(ERFLG0_UNK_2000);
            if (checkNoSetUpperAnime()) {
                setUpperAnimeBase(0x2B7);
            }
        }
    }

    if (!checkBarkAnime() && temp_r28->checkPass(11.0f)) {
        voiceStart(Z2SE_WL_V_BREATH_RUN);
    }
}

BOOL daAlink_c::checkWolfAtnMoveBack(s16 param_0) {
    f32 cos = cM_scos(param_0);
    return (field_0x2f98 != 1 && cos < -0.3826834261417389f) ||
           (field_0x2f98 == 1 && cos < -0.1736481785774231f);
}

void daAlink_c::setWolfAtnMoveDirection() {
    if (!checkAttentionState() && mProcID != PROC_WOLF_ROLL_ATTACK_MOVE) {
        field_0x2f98 = 0;
        return;
    }

    s16 var_r29;
    if (checkUnderMove0BckNoArcWolf(WANM_CHAIN_PULL_END)) {
        var_r29 = getChainStickAngleY(field_0x2fe4);
    } else {
        var_r29 = field_0x2fe2;
    }

    if ((mProcID == PROC_WOLF_ATN_AC_MOVE || mProcID == PROC_WOLF_ROLL_ATTACK_MOVE) &&
        checkModeFlg(1) && checkZeroSpeedF())
    {
        if (checkInputOnR()) {
            s16 temp_r28 = current.angle.y;
            current.angle.y = var_r29;

            if (wolfSideBgCheck(temp_r28)) {
                current.angle.y = temp_r28;
                onEndResetFlg1(ERFLG1_UNK_200000);
            }
        } else {
            current.angle.y = shape_angle.y;
        }
    }

    if (mTargetedActor != NULL) {
        field_0x2fe4 = cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos);
    }

    s16 temp_r27 = var_r29 - field_0x2fe4;
    if (mDemo.getDemoMode() == 23) {
        field_0x2f98 = 0;
    } else if (checkInputOnR()) {
        if (checkWolfAtnMoveBack(temp_r27)) {
            field_0x2f98 = 1;
        } else {
            field_0x2f98 = getDirectionFromAngle(temp_r27);
        }
    } else if (mProcID != PROC_WOLF_ROLL_ATTACK_MOVE) {
        if (field_0x2f98 == 1) {
            current.angle.y += 0x8000;
            mNormalSpeed *= -1.0f;
        }

        field_0x2f98 = 0;
    }
}

void daAlink_c::setBlendWolfAtnMoveAnime(f32 param_0) {
    if (checkSetChainPullAnime(field_0x2fe4)) {
        daAlink_WANM var_r29;
        if (checkEndResetFlg1(ERFLG1_UNK_2000000)) {
            var_r29 = WANM_CHAIN_PULL_END;
        } else {
            var_r29 = WANM_CHAIN_PULL;
        }

        if (!checkUnderMove0BckNoArcWolf(var_r29)) {
            f32 var_f29;
            if (checkChainBlockPushPull()) {
                var_f29 = 0.7f;
            } else {
                var_f29 = 1.0f;
            }

            setSingleAnimeWolfBaseSpeed(var_r29, var_f29, 3.0f);
        }
    } else if (field_0x2f98 == 1) {
        setBlendWolfAtnBackMoveAnime(param_0);
    } else {
        field_0x2fc1 = 0;

        s16 var_r28;
        if (field_0x3174 == 8 || !mLinkAcch.ChkGroundHit()) {
            var_r28 = 0;
        } else {
            var_r28 = getGroundAngle(&mLinkAcch.m_gnd, current.angle.y);
        }

        f32 var_f31 = fabsf((mNormalSpeed * cM_scos(var_r28)) / field_0x594);
        f32 var_f30;
        if (var_f31 < mpHIO->mWolf.mWlAtnMove.m.mWalkChangeRate) {
            var_f30 = var_f31 / mpHIO->mWolf.mWlAtnMove.m.mWalkChangeRate;
            int var_r27;
            if (checkModeFlg(1)) {
                var_r27 = 2;
            } else {
                var_r27 = 3;
            }

            daAlink_WANM var_r26;
            f32 var_f28;
            if (checkWolfAtnWait()) {
                var_r26 = WANM_ATN;
                var_f28 = mpHIO->mWolf.mWlMove.m.mGazeIdleAnmSpeed;
            } else {
                var_r26 = WANM_WAIT;
                var_f28 = mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed;
            }

            setDoubleAnimeWolf(var_f30, var_f28, mpHIO->mWolf.mWlAtnMove.m.mWalkAnmSpeed, var_r26,
                               WANM_ATN_LOCK_WALK_FRONT, var_r27, param_0);
        } else if (var_f31 < mpHIO->mWolf.mWlAtnMove.m.mRunChangeRate) {
            var_f30 =
                (var_f31 - mpHIO->mWolf.mWlAtnMove.m.mWalkChangeRate) /
                (mpHIO->mWolf.mWlAtnMove.m.mRunChangeRate - mpHIO->mWolf.mWlAtnMove.m.mWalkChangeRate);
            setDoubleAnimeWolf(var_f30, mpHIO->mWolf.mWlAtnMove.m.mWalkAnmSpeed,
                               mpHIO->mWolf.mWlAtnMove.m.mRunAnmSpeed, WANM_ATN_LOCK_WALK_FRONT,
                               WANM_ATN_LOCK_RUN, 3, param_0);
        } else {
            setDoubleAnimeWolf(1.0f, mpHIO->mWolf.mWlAtnMove.m.mRunAnmSpeed,
                               mpHIO->mWolf.mWlAtnMove.m.mRunAnmSpeed, WANM_ATN_LOCK_RUN,
                               WANM_ATN_LOCK_RUN, 3, param_0);
        }
    }
}

void daAlink_c::setBlendWolfAtnBackMoveAnime(f32 param_0) {
    field_0x594 = mpHIO->mWolf.mWlAtnMove.m.mRearMaxSpeed;

    f32 var_f28;
    if (field_0x3174 == 8) {
        var_f28 = 1.0f;
    } else {
        var_f28 = cM_scos(field_0x2ff0);
    }

    f32 temp_f31 = fabsf(mNormalSpeed * var_f28) / field_0x594;
    f32 var_f30;

    field_0x2fc1 = 3;

    if (temp_f31 < mpHIO->mWolf.mWlAtnMove.m.mRearWalkChangeRate) {
        f32 temp_f29 = temp_f31 / mpHIO->mWolf.mWlAtnMove.m.mRearWalkChangeRate;

        int var_r29;
        if (checkModeFlg(1)) {
            var_r29 = 2;
        } else {
            var_r29 = 3;
        }

        daAlink_WANM var_r28;
        if (checkWolfAtnWait()) {
            var_r28 = WANM_ATN;
            var_f30 = mpHIO->mWolf.mWlMove.m.mGazeIdleAnmSpeed;
        } else {
            var_r28 = WANM_WAIT;
            var_f30 = mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed;
        }

        setDoubleAnimeWolf(temp_f29, var_f30, mpHIO->mWolf.mWlAtnMove.m.mRearWalkAnmSpeed, var_r28,
                           WANM_ATN_LOCK_WALK_BACK, var_r29, param_0);
    } else {
        setDoubleAnimeWolf(1.0f, mpHIO->mWolf.mWlAtnMove.m.mRearWalkAnmSpeed,
                           mpHIO->mWolf.mWlAtnMove.m.mRearWalkAnmSpeed, WANM_ATN_LOCK_WALK_BACK,
                           WANM_ATN_LOCK_WALK_BACK, 3, param_0);
    }
}

int daAlink_c::setDoubleAnimeWolf(f32 param_0, f32 param_1, f32 param_2,
                                   daAlink_c::daAlink_WANM param_3, daAlink_c::daAlink_WANM param_4,
                                   int param_5, f32 param_6) {
    if (!checkWolf()) {
        // "Is Adult Link but setting Wolf animation\n"
        OS_REPORT("リンクなのに狼アニメ設定\n");
        JUT_ASSERT(2091, FALSE);
    }    

    J3DAnmTransform* var_r25 = getNowAnmPackUnder(UNDER_0);
    J3DAnmTransform* var_r29 = getAnimeResource(&mUnderAnmHeap[0], m_wlAnmDataTable[param_3].field_0x0, 0x2C00);
    int var_r30;
    if (var_r29 == NULL) {
        var_r29 = getNowAnmPackUnder(UNDER_0);
        var_r30 = 0;
    } else {
        var_r30 = 1;
    }

    J3DAnmTransform* var_r28 = getAnimeResource(&mUnderAnmHeap[1], m_wlAnmDataTable[param_4].field_0x0, 0x2C00);
    if (!var_r28) {
        var_r28 = getNowAnmPackUnder(UNDER_1);
    } else {
        var_r30 |= 1;
    }

    if (var_r30 != 0 && param_6 < 0.0f) {
        param_6 = mpHIO->mWolf.mWlMove.m.mNormalInterpolation;
    }

    commonDoubleAnime(var_r29, NULL, var_r28, NULL, param_0, param_1, param_2, param_5);

    if (param_6 >= 0.0f) {
        field_0x2060->initOldFrameMorf(param_6, 0, 40);
    }

    setFaceBasicTexture(FTANM_UNK_8B);

    if (checkWolfLieCode()) {
        mMidnaAnm = 2;
    } else {
        mMidnaAnm = m_wlAnmDataTable[param_3].field_0x2;
    }

    daAlink_WANM var_r27;
    if (checkModeFlg(1)) {
        field_0x215c = &mUnderFrameCtrl[0];
        var_r27 = param_3;
    } else {
        field_0x215c = &mUnderFrameCtrl[1];
        var_r27 = param_4;
    }

    field_0x2160 = (u16*)&m_wlAnmDataTable[var_r27].field_0x4;
    field_0x2fd8 = m_wlAnmDataTable[var_r27].field_0x3;
    return 1;
}

void daAlink_c::setSingleAnimeWolfBase(daAlink_c::daAlink_WANM i_anmID) {
    setSingleAnimeWolf(i_anmID, 1.0f, 0.0f, -1, 3.0f);
}

void daAlink_c::setSingleAnimeWolfBaseMorf(daAlink_c::daAlink_WANM i_anmID, f32 i_morf) {
    setSingleAnimeWolf(i_anmID, 1.0f, 0.0f, -1, i_morf);
}

void daAlink_c::setSingleAnimeWolfBaseSpeed(daAlink_c::daAlink_WANM i_anmID, f32 i_speed,
                                            f32 i_morf) {
    setSingleAnimeWolf(i_anmID, i_speed, 0.0f, -1, i_morf);
}

int daAlink_c::setSingleAnimeWolf(daAlink_c::daAlink_WANM i_anmID, f32 i_speed, f32 i_start,
                                   s16 i_endFrame, f32 i_morf) {
    if (!checkWolf()) {
        // "Is Adult Link but setting Wolf animation\n"
        OS_REPORT("リンクなのに狼アニメ設定\n");
        JUT_ASSERT(2091, FALSE);
    }    

    J3DAnmTransform* var_r30 = getAnimeResource(&mUnderAnmHeap[0], m_wlAnmDataTable[i_anmID].field_0x0, 0x10800);
    if (var_r30 == NULL) {
        var_r30 = getNowAnmPackUnder(UNDER_0);
    }

    commonSingleAnime(var_r30, NULL, i_speed, i_start, i_endFrame);

    if (i_morf >= 0.0f) {
        field_0x2060->initOldFrameMorf(i_morf, 0, 40);
    }

    setFaceBasicTexture(FTANM_UNK_8B);

    if (checkWolfLieCode()) {
        mMidnaAnm = 2;
    } else {
        mMidnaAnm = m_wlAnmDataTable[i_anmID].field_0x2;
    }

    field_0x215c = mUnderFrameCtrl;
    field_0x2160 = (u16*)&m_wlAnmDataTable[i_anmID].field_0x4;
    field_0x2fd8 = m_wlAnmDataTable[i_anmID].field_0x3;
    return 1;
}

void daAlink_c::setSingleAnimeWolfParam(daAlink_c::daAlink_WANM i_anmID,
                                        daAlinkHIO_anm_c const* i_anmData) {
    setSingleAnimeWolf(i_anmID, i_anmData->mSpeed, i_anmData->mStartFrame, i_anmData->mEndFrame,
                       i_anmData->mInterpolation);
}

int daAlink_c::checkWolfLandAction(int param_0) {
    f32 temp_f31 = 0.0099999998f * (mFallHeight - current.pos.y);

    if (checkNoLandDamageSlidePolygon() || temp_f31 < mpHIO->mWolf.mWlDamage.mFall.m.mSmallDamageHeight) {
        if (param_0 != 0) {
            procWolfLandDamageInit(0);
        } else if (mProcID == PROC_WOLF_JUMP_ATTACK || mProcID == PROC_WOLF_DASH_REVERSE) {
            return 0;
        } else {
            procWolfLandInit();
        }
    } else {
        int var_r4;
        if (temp_f31 < mpHIO->mWolf.mWlDamage.mFall.m.mLargeDamageHeight) {
            var_r4 = 1;
        } else {
            var_r4 = 2;
        }

        procWolfLandDamageInit(var_r4);
    }

    return 1;
}

BOOL daAlink_c::checkMidnaUseAbility() const {
    return dComIfGp_checkPlayerStatus1(0, 0x800000) || dKy_withwarp_capture_check();
}

void daAlink_c::checkWolfUseAbility() {
    if (mProcID == PROC_WOLF_GIANT_PUZZLE) {
        return;
    }

    if (checkMidnaLockJumpPoint() != NULL && !checkMidnaRide()) {
        offWolfEyeUp();
    } else {
        // event flag 0x4308: senses ability unlocked
        if (dComIfGs_isEventBit(0x4308) && field_0x2fd2 == 0 && !checkEventRun() &&
            mWolfEyeUp < mpHIO->mWolf.m.mSensesLingerTime && wolfSenseTrigger())
        {
            if (mWolfEyeUp == 0) {
                onWolfEyeUp();
            } else {
                offWolfEyeUp();
            }
        }
    }
}

int daAlink_c::checkWolfGroundSpecialMode() {
    if (mLinkAcch.ChkGroundHit() && !checkModeFlg(MODE_PLAYER_FLY) &&
        checkEndResetFlg0(ERFLG0_UNK_1))
    {
        return procCoMetamorphoseInit();
    } else if (mSpecialMode == SMODE_WOLF_PUZZLE || mSpecialMode == 0x28) {
        return procWolfGiantPuzzleInit();
    } else if (checkCargoCarry()) {
        return procWolfCargoCarryInit();
    } else if (checkSlideAction()) {
        return 1;
    } else if (checkEndResetFlg1(ERFLG1_UNK_1)) {
        return procWolfScreamWaitInit();
    }
    return 0;
}

int daAlink_c::checkNextActionWolf(int param_0) {
    if (checkDeadAction(1)) {
        return 1;
    }

    if (checkEndResetFlg1(ERFLG1_MIDNA_RIDE_SHOCK) &&
        (mProcID == PROC_WOLF_WAIT || mProcID == PROC_WOLF_SERVICE_WAIT ||
         mProcID == PROC_WOLF_TIRED_WAIT || (!checkInputOnR() && mProcID == PROC_WOLF_MOVE) ||
         mProcID == PROC_WOLF_LIE_START || mProcID == PROC_WOLF_LIE_MOVE ||
         mProcID == PROC_WOLF_ATN_AC_MOVE))
    {
        return procWolfMidnaRideShockInit();
    }

    if ((!checkAttentionState() ||
         (mProcID != PROC_WOLF_WAIT_ATTACK && mProcID != PROC_WOLF_JUMP_AT_NORMAL_LAND)) &&
        ((mProcID != PROC_WOLF_WAIT_SLIP && !param_0) || checkInputOnR()))
    {
        if (checkAttentionState()) {
            setSpeedAndAngleWolfAtn();
        } else {
            setSpeedAndAngleWolf();
        }
    }

    setFrontWallType();

    if (checkSlope()) {
        if (checkWolfDashMode()) {
            field_0x594 = mpHIO->mWolf.mWlSlide.m.mClimbingMaxSpeed;
        } else {
            field_0x594 = mpHIO->mWolf.mWlSlide.m.mClimbingMaxSpeedWeak;
        }
    } else if (checkFmChainGrabAnime()) {
        field_0x594 = 13.0f;
    } else if (mTargetedActor != NULL) {
        field_0x594 = mpHIO->mWolf.mWlAtnMove.m.mMaxSpeed;
    } else if (checkAttentionState()) {
        field_0x594 = mpHIO->mWolf.mWlMove.m.mGazeMaxSpeed;
    } else if (checkNoResetFlg1(FLG1_DASH_MODE)) {
        if (checkWolfSlowDash()) {
            field_0x594 = mpHIO->mWolf.mWlMove.m.mADashMaxSpeedSlow;
        } else if (field_0x2fc7 == 2) {
            field_0x594 = mpHIO->mWolf.mWlMove.m.mADashMaxSpeedSlow2;
        } else {
            field_0x594 = mpHIO->mWolf.mWlMove.m.mADashMaxSpeed;
        }
    } else if (checkWolfDashMode()) {
        field_0x594 = mpHIO->mWolf.mWlMove.m.mMaxSpeed;
    } else {
        field_0x594 = mpHIO->mWolf.mWlMoveNoP.m.mMaxSpeed;
    }

    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    if (checkWolfLieContinue(0)) {
        return procWolfLieStartInit(0);
    }

    if (checkNextActionFromButton()) {
        return 1;
    }

    fopAc_ac_c* var_r4 = checkWolfRopeHit(&field_0x850[0], field_0x850[0].GetCP(), FALSE);
    if (var_r4 == NULL) {
        var_r4 = checkWolfRopeHit(&field_0x850[1], &current.pos, FALSE);
    }

    if (var_r4 != NULL) {
        field_0x280c.setData(var_r4);
        return procWolfRopeMoveInit(0, 1);
    }

    if (param_0 && !checkInputOnR()) {
        return 0;
    }

    BOOL var_r30;
    if (!checkInputOnR() && mProcID == PROC_WOLF_WAIT_SLIP && checkWolfWaitSlipPolygon()) {
        var_r30 = 0;
    } else if (checkAttentionState()) {
        var_r30 = procWolfAtnActorMoveInit();
    } else {
        field_0x2f98 = 4;

        if (checkZeroSpeedF()) {
            if (!checkEventRun() && field_0x33a8 > 0.85f && abs(field_0x3180) < 0x5000 &&
                (mProcID == PROC_WOLF_WAIT || mProcID == PROC_WOLF_MOVE) &&
                cLib_distanceAngleS(field_0x2fe2, current.angle.y) > 0x7000)
            {
                if (checkEndResetFlg1(ERFLG1_UNK_200000)) {
                    var_r30 = procWolfSlipTurnInit(0);
                } else {
                    var_r30 = 0;
                }

                if (var_r30 == 0 && !checkEventRun()) {
                    var_r30 = checkWaitAction();
                }
            } else {
                var_r30 = checkWaitAction();
            }
        } else if (checkInputOnR() && mGndPolySpecialCode != dBgW_SPCODE_ICE &&
                   abs(field_0x3180) < 0x5000 &&
                   cLib_distanceAngleS(field_0x2fe2, current.angle.y) > 0x7000 &&
                   ((checkWolfDashMode() &&
                     speedF / field_0x594 > mpHIO->mWolf.mWlMove.m.mSlipStartRate) ||
                    (!checkWolfDashMode() &&
                     speedF / field_0x594 > mpHIO->mWolf.mWlMoveNoP.m.mSlideStartRate)))
        {
            var_r30 = procWolfSlipInit();
        } else {
            var_r30 = procWolfMoveInit();
        }
    }

    return var_r30;
}

BOOL daAlink_c::wolfSideBgCheck(s16 param_0) {
    s16 var_r31 = current.angle.y - param_0;
    if (var_r31 == 0) {
        return 0;
    }

    f32 temp_f31 = cM_scos(param_0);
    f32 temp_f30 = cM_ssin(param_0);
    f32 temp_f29 = cM_scos(current.angle.y);
    f32 temp_f28 = cM_ssin(current.angle.y);

    cXyz sp14(current.pos.x + temp_f30 * 90.0f, current.pos.y + mAcchCir[2].GetWallH(),
              current.pos.z + temp_f31 * 90.0f);
    cXyz sp8(current.pos.x + temp_f28 * 90.0f, sp14.y, current.pos.z + temp_f29 * 90.0f);

    if (var_r31 > 0) {
        sp8.x += temp_f29 * 35.0f;
        sp8.z -= temp_f28 * 35.0f;
    } else {
        sp8.x -= temp_f29 * 35.0f;
        sp8.z += temp_f28 * 35.0f;
    }

    if (commonLineCheck(&sp14, &sp8)) {
        sp14.x = current.pos.x - temp_f30 * 50.0f;
        sp14.z = current.pos.z - temp_f31 * 50.0f;
        sp8.x = current.pos.x - temp_f28 * 50.0f;
        sp8.z = current.pos.z - temp_f29 * 50.0f;

        if (var_r31 > 0) {
            sp8.x -= temp_f29 * 35.0f;
            sp8.z += temp_f28 * 35.0f;
        } else {
            sp8.x += temp_f29 * 35.0f;
            sp8.z -= temp_f28 * 35.0f;
        }

        if (commonLineCheck(&sp14, &sp8)) {
            return true;
        }
    }

    return false;
}

BOOL daAlink_c::checkWolfAttackReverse(int param_0) {
    if (mProcID != PROC_WOLF_LOCK_ATTACK && checkAtShieldHit(mAtCyl)) {
        return procWolfAttackReverseInit();
    }

    if ((checkNoResetFlg0(FLG0_UNK_18) && mProcID == PROC_WOLF_DASH) ||
        (param_0 && !checkNoResetFlg3(FLG3_UNK_400) && checkNoResetFlg1(FLG1_UNK_20000000) &&
         cLib_distanceAngleS(field_0x3092, current.angle.y) > 0x6000))
    {
        if (mProcID == PROC_WOLF_DASH) {
            return procWolfDashReverseInit(0);
        } else {
            if (checkNoResetFlg3(FLG3_UNK_800000)) {
                dComIfGp_setHitMark(2, NULL, &field_0x378c, &field_0x3166, NULL, 0);
            }

            return procWolfAttackReverseInit();
        }
    }

    return false;
}

int daAlink_c::checkWolfBarrierHitReverse() {
    if (field_0x3100 != 0 && mProcID != PROC_FRONT_ROLL_SUCCESS && mProcID != PROC_DAMAGE &&
        mProcID != PROC_WOLF_ATTACK_REVERSE && mProcID != PROC_WOLF_DASH_REVERSE)
    {
        if (checkWolf()) {
            return procWolfDashReverseInit(1);
        } else if (mLinkAcch.ChkGroundHit()) {
            if (cLib_distanceAngleS(shape_angle.y, field_0x3102) >= 0x4000) {
                setFrontRollCrashShock(0);
                return procDamageInit(NULL, 1);
            } else {
                return procFrontRollSuccessInit();
            }
        } else {
            setFrontRollCrashShock(0);
            return procCoLargeDamageInit(-6, 1, 0, 0, NULL, 0);
        }
    }

    return 0;
}

BOOL daAlink_c::checkWolfBarrierWallHit(cBgS_PolyInfo& i_poly) {
    if (field_0x3100 != 0 || checkZeroSpeedF()) {
        return false;
    }

    s16 bg_name = getMoveBGActorName(i_poly, TRUE);
    if (bg_name == PROC_OBJ_TP || bg_name == PROC_OBJ_GB || bg_name == PROC_Obj_GanonWall2) {
        return true;
    }

    return false;
}

void daAlink_c::wolfBgCheck() {
    offNoResetFlg3(daPy_FLG3(FLG3_UNK_800000 | FLG3_UNK_400));

    if (checkModeFlg(0x224000) || mProcID == PROC_WOLF_TAG_JUMP || mProcID == PROC_LARGE_DAMAGE) {
        offNoResetFlg1(FLG1_UNK_20000000);
        return;
    }

    cXyz sp38;
    cXyz sp2C;
    cXyz sp20;
    cXyz sp14;
    cXyz sp8;

    int var_r24 = 0;
    int i;
    f32 temp_f27 = cM_ssin(shape_angle.y);
    f32 temp_f26 = cM_scos(shape_angle.y);

    f32 temp_f30;
    f32 var_f31;
    f32 var_f29;
    f32 var_f28;
    if (checkModeFlg(2)) {
        var_f29 = 0.0f;
        var_f28 = 1.0f;
    } else {
        var_f29 = cM_ssin(shape_angle.x);
        var_f28 = cM_scos(shape_angle.x);
    }

    int var_r26;
    if (checkModeFlg(0x40000)) {
        var_r26 = 0;
    } else {
        var_r26 = 2;
    }

    cM3dGPla sp44;

    for (i = 0; i < 2; i++) {
        sp38.set(current.pos.x, current.pos.y + mAcchCir[2].GetWallH(), current.pos.z);

        if (mLinkAcch.ChkGroundHit()) {
            sp38.y -= speed.y;
        }

        if (i == 0) {
            var_f31 = -50.0f;
        } else {
            var_f31 = 90.0f;
        }

        temp_f30 = var_f31 * var_f28;

        sp2C.set(current.pos.x + (temp_f30 * temp_f27), sp38.y - (var_f31 * var_f29), current.pos.z + (temp_f30 * temp_f26));

        int var_r28 = getCrawlMoveVec(&sp38, &sp2C, &sp20, var_r26, 0, &field_0x2fca);
        if (var_r28) {
            if (dComIfG_Bgsp().GetPolyAttackThrough(mLinkLinChk)) {
                onNoResetFlg3(FLG3_UNK_400);
            }

            if (checkWolfBarrierWallHit(mLinkLinChk)) {
                setWolfBarrierHitEffect(mLinkLinChk);
            } else {
                onNoResetFlg3(FLG3_UNK_800000);
                dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp44);
                field_0x378c = mLinkLinChk.GetCross();
            }
        }

        sp38.y -= mAcchCir[2].GetWallH() - mAcchCir[0].GetWallH();
        sp2C.y -= mAcchCir[2].GetWallH() - mAcchCir[0].GetWallH();

        int var_r25 = getCrawlMoveVec(&sp38, &sp2C, &sp14, var_r26, 0, &field_0x2fca);
        if (var_r25) {
            if (dComIfG_Bgsp().GetPolyAttackThrough(mLinkLinChk)) {
                onNoResetFlg3(FLG3_UNK_400);
            }

            if (checkWolfBarrierWallHit(mLinkLinChk)) {
                setWolfBarrierHitEffect(mLinkLinChk);
            } else {
                onNoResetFlg3(FLG3_UNK_800000);
                dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp44);
                field_0x378c = mLinkLinChk.GetCross();
            }
        }

        if (var_r28 != 0 || var_r25 != 0) {
            onNoResetFlg1(FLG1_UNK_20000000);
    
            if (var_r28 != 0 && var_r25 != 0) {
                if (sp20.abs2() > sp14.abs2()) {
                    sp8 = sp20;
                } else {
                    sp8 = sp14;
                }
            } else if (var_r28 != 0) {
                sp8 = sp20;
            } else {
                sp8 = sp14;
            }

            sp38 -= sp8;
            sp2C -= sp8;

            var_r28 = getCrawlMoveVec(&sp38, &sp2C, &sp20, var_r26, 0, NULL);

            sp38.y += mAcchCir[2].GetWallH() - mAcchCir[0].GetWallH();
            sp2C.y += mAcchCir[2].GetWallH() - mAcchCir[0].GetWallH();

            var_r25 = getCrawlMoveVec(&sp38, &sp2C, &sp14, var_r26, 0, NULL);

            if (var_r28 != 0 || var_r25 != 0) {
                if (var_r28 != 0 && var_r25 != 0) {
                    if (sp20.abs2() > sp14.abs2()) {
                        sp8 += sp20;
                    } else {
                        sp8 += sp14;
                    }
                } else if (var_r28 != 0) {
                    sp8 += sp20;
                } else {
                    sp8 += sp14;
                }
            }

            current.pos -= sp8;

            field_0x3092 = cM_atan2s(-sp8.x, -sp8.z);

            if (checkNoResetFlg3(FLG3_UNK_800000)) {
                field_0x3166.set(cM_atan2s(sp44.mNormal.y, sp44.mNormal.absXZ()), cM_atan2s(-sp44.mNormal.x, -sp44.mNormal.z), 0);
            }

            var_r24++;
        }
    }

    if (var_r24 == 0 && checkNoResetFlg1(FLG1_UNK_20000000)) {
        for (i = 0; i < 2; i++) {
            sp38.set(current.pos.x, current.pos.y + mAcchCir[2].GetWallH(), current.pos.z);
            if (mLinkAcch.ChkGroundHit()) {
                sp38.y -= speed.y;
            }

            if (i == 0) {
                var_f31 = -60.0f;
            } else {
                var_f31 = 100.0f;
            }

            temp_f30 = var_f31 * var_f28;
            sp2C.set(current.pos.x + (temp_f30 * temp_f27), sp38.y - (var_f31 * var_f29), current.pos.z + (temp_f30 * temp_f26));

            if (getCrawlMoveVec(&sp38, &sp2C, &sp20, var_r26, 0, NULL)) {
                break;
            }

            sp38.y -= mAcchCir[2].GetWallH() - mAcchCir[0].GetWallH();
            sp2C.y -= mAcchCir[2].GetWallH() - mAcchCir[0].GetWallH();

            if (getCrawlMoveVec(&sp38, &sp2C, &sp14, var_r26, 0, NULL)) {
                break;
            }
        }

        if (i == 2) {
            offNoResetFlg1(FLG1_UNK_20000000);
        }
    }

    if (var_r24 == 2) {
        if (field_0x2fa6 == 0 && !checkEndResetFlg1(ERFLG1_UNK_200000)) {
            s16 var_r27 = field_0x3092 - shape_angle.y;
            if (var_r27 <= 0) {
                var_r27 += 0x4000;
            } else {
                var_r27 -= 0x4000;
            }

            field_0x2fe6 += var_r27;
            current.angle.y += var_r27;
            shape_angle.y += var_r27;
        }

        field_0x2fa6 = 3;
    } else if (field_0x2fa6 != 0) {
        field_0x2fa6--;
    }
}

void daAlink_c::changeWolfBlendRate(int param_0) {
    if (checkWolfGrabAnime()) {
        if (param_0 == 7) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (param_0 == 3) {
            mNowAnmPackUpper[2].setRatio(1.0f);
        }
    } else if (checkBarkAnime()) {
        if (param_0 == 2) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (param_0 == 3) {
            mNowAnmPackUpper[2].setRatio(1.0f);
        }
    }
}

void daAlink_c::setWolfFootMatrix() {
    static const u16 fFootJoint[] = {
        0x0010,
        0x0015,
    };
    static const u16 bFootJoint[] = {
        0x001C,
        0x0021,
    };

    daAlink_footData_c* var_r29 = mFootData1;

    int i, j;
    for (i = 0; i < 2; i++, var_r29++) {
        int var_r31 = fFootJoint[i];
        for (j = 0; j < 3; j++, var_r31++) {
            cMtx_copy(mpLinkModel->getAnmMtx(var_r31), var_r29->field_0x14[j]);
            if (j == 1) {
                var_r31++;
            }
        }
    }

    var_r29 = mFootData2;

    for (int i = 0; i < 2; i++, var_r29++) {
        int var_r31 = bFootJoint[i];
        for (int j = 0; j < 3; j++, var_r31++) {
            cMtx_copy(mpLinkModel->getAnmMtx(var_r31), var_r29->field_0x14[j]);
            if (j == 0) {
                var_r31++;
            }
        }
    }

    if (field_0x2060->getOldFrameFlg()) {
        cXyz sp40[4];
        cXyz sp34(0.0f, 0.0f, 0.0f);
        cXyz sp28;

        var_r29 = mFootData1;

        for (i = 0; i < 2; i++, var_r29++) {
            int var_r31 = fFootJoint[i];
            for (j = 0; j < 4; j++, var_r31++) {
                mDoMtx_multVecZero(mpLinkModel->getAnmMtx(var_r31), &sp40[j]);
            }

            int temp_r31 = fFootJoint[i];
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x6, 0, 0, 0, NULL);
        
            sp34.x = sp40[0].abs(sp40[1]);
            mDoMtx_stack_c::multVec(&sp34, &sp28);
            temp_r31++;
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x4, 0, 0, 0, &sp28);

            sp34.x = sp40[1].abs(sp40[2]);
            mDoMtx_stack_c::multVec(&sp34, &sp28);
            temp_r31++;
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x4, 0, 0, 0, &sp28);

            sp34.x = sp40[2].abs(sp40[3]);
            mDoMtx_stack_c::multVec(&sp34, &sp28);
            temp_r31++;
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x2, 0, 0, 0, &sp28);
        }

        var_r29 = mFootData2;

        for (i = 0; i < 2; i++, var_r29++) {
            int var_r31 = bFootJoint[i];
            for (j = 0; j < 4; j++, var_r31++) {
                mDoMtx_multVecZero(mpLinkModel->getAnmMtx(var_r31), &sp40[j]);
            }

            int temp_r31 = bFootJoint[i];
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x6, 0, 0, 0, NULL);
        
            sp34.x = sp40[0].abs(sp40[1]);
            mDoMtx_stack_c::multVec(&sp34, &sp28);
            temp_r31++;
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x6, 0, 0, 0, &sp28);

            sp34.x = sp40[1].abs(sp40[2]);
            mDoMtx_stack_c::multVec(&sp34, &sp28);
            temp_r31++;
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x4, 0, 0, 0, &sp28);

            sp34.x = sp40[2].abs(sp40[3]);
            mDoMtx_stack_c::multVec(&sp34, &sp28);
            temp_r31++;
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r31), var_r29->field_0x2, 0, 0, 0, &sp28);
        }
    }
}

void daAlink_c::wolfFootBgCheck() {
    int i;
    int j;
    if (field_0x2060->getOldFrameFlg()) {
        cXyz spBC;
        cXyz spB0;

        u32 sp30;
        MtxP sp2C = mpLinkModel->getBaseTRMtx();
        sp30 = checkModeFlg(1);

        cXyz spC8[4];

        f32 spA0[2][2];
        s16 sp58[2][2];
        s16 sp50[2][2];
        s16 sp48[2][2];
        int sp40[2];
        f32 sp38[2];

        static Vec const localFootOffset = {6.0f, -5.0f, 0.0f};
        static Vec const frontOffset = {0.0f, 0.0f, 50.0f};

        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x13), &localFootOffset, &spC8[0]);
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x18), &localFootOffset, &spC8[1]);
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x1F), &localFootOffset, &spC8[2]);
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x24), &localFootOffset, &spC8[3]);

        BOOL sp28 = !mLinkAcch.ChkGroundHit() || checkModeFlg(0x78C52);
        BOOL sp24 = sp28 || (mLinkAcch.ChkGroundHit() && mSinkShapeOffset < 0.0f);

        int j;
        int i;
        daAlink_footData_c* var_r28;

        f32* sp20 = &spA0[0][0];
        cXyz* var_r26 = &spC8[0];

        f32 temp_f31;
        f32 temp_f30;
        f32 temp_f29;
        f32 var_f28;
        f32 temp_f27;
        temp_f29 = 50.0f * fabsf(cM_ssin(field_0x2ff0));
        temp_f27 = 30.0f + temp_f29;
        f32 sp1C = -temp_f27;
        mDoMtx_stack_c::XrotS(-shape_angle.x);
        mDoMtx_stack_c::YrotM(-shape_angle.y);

        for (i = 0; i < 2; i++) {
            if (i == 0) {
                var_r28 = mFootData1;
            } else {
                var_r28 = mFootData2;
            }

            for (j = 0; j < 2; j++, var_r28++, sp20++, var_r26++) {
                if (sp24) {
                    var_r28->field_0x1 = 0xF;
                } else {
                    spB0 = *var_r26 - var_r28->field_0x8;
                    if (spB0.abs2XZ() < 100.0f && sp30 && var_r28->field_0x0 != 0) {
                        if (var_r28->field_0x1 != 0) {
                            var_r28->field_0x1--;
                        } else {
                            *var_r26 = var_r28->field_0x8;
                        }
                    } else {
                        var_r28->field_0x1 = 0xF;
                    }
                }

                var_r28->field_0x8 = *var_r26;

                spBC.set(var_r26->x, temp_f29 + (50.0f + (current.pos.y + l_autoUpHeight)), var_r26->z);
                mLinkGndChk.SetPos(&spBC);

                temp_f31 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
                temp_f30 = var_r26->y - temp_f31;

                if (-G_CM3D_F_INF != temp_f31 && temp_f30 < temp_f27 && temp_f30 > sp1C) {
                    *sp20 = temp_f31;
                    var_r28->field_0x0 = 1;
                    sp48[i][j] = getGroundAngle(&mLinkGndChk, shape_angle.y);

                    spBC.set(var_r26->x - current.pos.x, temp_f31 - current.pos.y, var_r26->z - current.pos.z);
                    mDoMtx_stack_c::multVec(&spBC, &spB0);
                    *sp20 = spB0.y + current.pos.y;
                } else {
                    *sp20 = current.pos.y;
                    var_r28->field_0x0 = 0;
                }
            }
        }

        if (!sp24) {
            for (i = 0; i < 2; i++) {
                if (spA0[i][1] > spA0[i][0]) {
                    sp38[i] = spA0[i][0];
                    sp40[i] = 0;
                } else {
                    sp38[i] = spA0[i][1];
                    sp40[i] = 1;
                }
            }

            if (sp38[0] > sp38[1]) {
                var_f28 = sp38[1];
            } else {
                var_f28 = sp38[0];
            }
        }

        setSandShapeOffset();
        setMatrixOffset(&mSinkShapeOffset, mSinkShapeOffset);
        setSandDownBgCheckWallH();
        var_f28 = 0.0f;
        setMatrixOffset(&field_0x2b94, var_f28);

        for (i = 0; i < 2; i++) {
            if (i == 0) {
                var_r28 = mFootData1;
            } else {
                var_r28 = mFootData2;
            }

            for (j = 0; j < 2; j++, var_r28++) {
                if (sp24 || (i == 0 && checkUnderMove0BckNoArcWolf(WANM_SMELL))) {
                    sp58[i][j] = 0;
                    sp50[i][j] = 0;
                } else {
                    temp_f30 = spA0[i][j] - sp2C[1][3];
                    if (temp_f30 >= 35.0f) {
                        temp_f30 = 35.0f;
                    }

                    if (temp_f30 > 0.0f || sp30) {
                        if (!setLegAngle(temp_f30, var_r28, &sp58[i][j], &sp50[i][j], 1)) {   
                            sp58[i][j] = 0;
                            sp50[i][j] = 0;
                        }
                    } else {
                        sp58[i][j] = 0;
                        sp50[i][j] = 0;
                    }
                }

                cLib_addCalcAngleS(&var_r28->field_0x6, sp58[i][j], 2, 0x1800, 0x10);
                cLib_addCalcAngleS(&var_r28->field_0x4, sp50[i][j], 2, 0x1800, 0x10);
                
                s16 spE;
                if (mProcID == PROC_WOLF_SLIDE_READY) {
                    spE = shape_angle.x;
                } else {
                    spE = -shape_angle.x;
                }

                if (!sp24 && var_r28->field_0x0 != 0 && field_0x3174 != 8 && sp30) {
                    spE += sp48[i][j];
                }

                cLib_addCalcAngleS(&var_r28->field_0x2, spE, 2, 0x1800, 0x10);
            }
        }

        s16 spC;

        if (!sp28) {
            cXyz sp90;
            cXyz sp84;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(shape_angle.y + field_0x308c);
            mDoMtx_stack_c::multVec(&frontOffset, &sp90);

            sp84 = (current.pos * 2.0f) - sp90;

            sp90.y += 165.0f;
            sp84.y += 165.0f;
            mLinkGndChk.SetPos(&sp90);

            temp_f31 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
            if (fabsf(temp_f31 - current.pos.y) < 55.0f + temp_f29) {
                sp90.y = temp_f31;
            } else {
                sp90.y = current.pos.y;
            }

            int sp18;
            if (-G_CM3D_F_INF != temp_f31) {
                sp18 = dComIfG_Bgsp().GetPolyAtt0(mLinkGndChk);
            }

            if (-G_CM3D_F_INF == temp_f31 || sp18 == 3 || sp18 == 0xD) {
                sp90.y += mSinkShapeOffset;
            }

            mLinkGndChk.SetPos(&sp84);

            temp_f31 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
            if (fabsf(temp_f31 - current.pos.y) < 55.0f + temp_f29) {
                sp84.y = temp_f31;
            } else {
                sp84.y = current.pos.y;
            }

            if (-G_CM3D_F_INF != temp_f31) {
                sp18 = dComIfG_Bgsp().GetPolyAtt0(mLinkGndChk);
            }

            if (-G_CM3D_F_INF == temp_f31 || sp18 == 3 || sp18 == 0xD) {
                sp84.y += mSinkShapeOffset;
            }

            sp90 -= sp84;
            spC = sp90.atan2sY_XZ();
        } else if (checkModeFlg(0x20000) || mProcID == PROC_METAMORPHOSE || mProcID == PROC_WOLF_TAG_JUMP) {
            spC = mProcVar2.field_0x300c;
        } else if (mProcID == PROC_WOLF_SLIP_TURN || mProcID == PROC_WOLF_SLIDE_READY || mProcID == PROC_WOLF_DIG || mProcID == PROC_WOLF_SLIP_TURN_LAND) {
            spC = field_0x2ff0;
        } else {
            spC = 0;
        }

        if (mProcID != PROC_WOLF_CARGO_CARRY) {
            cLib_addCalcAngleS(&shape_angle.x, spC, 3, 0x1000, 1000);
        }

        if (checkModeFlg(1) ) {
            spC = -shape_angle.x;
        } else {
            spC = 0;
        }

        cLib_addCalcAngleS(&field_0x30d6, spC, 3, 0x1000, 0x100);
    }
}

BOOL daAlink_c::checkWolfWaitSlipPolygon() {
    if (!dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd) || mGndPolySpecialCode == dBgW_SPCODE_HEAVY_SNOW || field_0x3174 == 8) {
        return 0;
    }

    cM3dGPla sp8;
    if (!dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &sp8) || !cBgW_CheckBGround(sp8.mNormal.y)) {
        return 0;
    }

    if (mGndPolySpecialCode == dBgW_SPCODE_LIGHT_SNOW && sp8.mNormal.y < cM_scos(cM_deg2s(mpHIO->mSlide.m.mClimbAngle))) {
        return 1;
    }

    return cM_atan2s(sp8.mNormal.absXZ(), sp8.mNormal.y) > field_0x3122;
}

f32 daAlink_c::setWolfWaitSlip() {
    cM3dGPla sp8;
    dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &sp8);

    current.angle.y = sp8.mNormal.atan2sX_Z();

    f32 var_f31;
    f32 temp_f30 = JMAFastSqrt(1.0f - sp8.mNormal.y);

    if (abs((s16)(shape_angle.y - current.angle.y)) >= 0x4000) {
        mProcVar2.field_0x300c = 1;
        var_f31 = -1000.0f;
    } else {
        mProcVar2.field_0x300c = 0;
        var_f31 = 1000.0f;
    }

    field_0x37c8.set(current.pos.x + (temp_f30 * (var_f31 * sp8.mNormal.x)), eyePos.y - (var_f31 * sp8.mNormal.y), current.pos.z + (temp_f30 * (var_f31 * sp8.mNormal.z)));
    return temp_f30;
}

void daAlink_c::checkWolfAtnDoCharge() {}

void daAlink_c::setWolfChainPos() {
    static Vec const wolfChainBaseOffset = {22.0f, 0.0f, -12.0f};
    static Vec const wolfChainVec = {0.0f, 0.0f, -9.0f};

    f32 temp_f31;
    f32 temp_f30;
    int i;
    if (mClothesChangeWaitTimer == 0) {
        cXyz* var_r29 = field_0x363c;
        cXyz* var_r26 = field_0x366c;
        csXyz* var_r30 = field_0x3142;

        if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
            for (i = 0; i < 4; i++, var_r26++) {
                *var_r26 = cXyz::Zero;
            }

            var_r26 = field_0x366c;
        }

        cXyz sp78;
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x11), &wolfChainBaseOffset, &sp78);

        cXyz sp6C;
        mDoMtx_multVecSR(mpLinkModel->getAnmMtx(0x11), &cXyz::BaseZ, &sp6C);

        s16 sp12 = sp6C.atan2sX_Z();
        s16 sp10 = sp6C.atan2sY_XZ();
        mDoMtx_multVecSR(mpLinkModel->getAnmMtx(0x11), &cXyz::BaseY, &sp6C);

        s16 spE = (cM_atan2s(sp6C.y, sp6C.absXZ()) + 0x4000);

        if (!checkNoResetFlg1(FLG1_UNK_200000)) {
            onNoResetFlg1(FLG1_UNK_200000);

            mDoMtx_stack_c::transS(sp78);
            mDoMtx_stack_c::ZXYrotM(-0x2000, sp12, 0);
            mDoMtx_stack_c::multVec(&wolfChainVec, var_r29);
            var_r30->set(-0x2000, sp12, spE);
            *var_r26 = cXyz::Zero;

            var_r29++;
            var_r26++;
            var_r30++;
            for (int i = 1; i < 4; i++, var_r29++, var_r26++, var_r30++) {
                *var_r29 = *(var_r29 - 1);
                var_r29->y -= 9.0f;

                *var_r30 = *(var_r30 - 1);
                var_r30->z += 0x4000;

                *var_r26 = cXyz::Zero;
            }
        } else {
            cXyz sp60;
            cXyz sp54;

            for (i = 0; i < 4; i++, var_r29++, var_r26++, var_r30++) {
                sp60 = *var_r29;

                if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
                    sp6C = cXyz::Zero;
                } else {
                    sp6C = (*var_r29 - sp78) + *var_r26;
                }

                sp6C.y += -1.5f;
                temp_f30 = sp6C.absXZ();
                s16 spC = cM_atan2s(sp6C.y, temp_f30);
                s16 spA;

                if (temp_f30 < 1.0f) {
                    spA = var_r30->y;
                } else {
                    spA = cM_atan2s(-sp6C.x, -sp6C.z);
                }

                s16 sp8;
                if (i == 0) {
                    sp8 = (spA - sp12);
                    if (sp8 > 0x2000) {
                        spA = (sp12 + 0x2000);
                    } else if (sp8 < -0x2000) {
                        spA = (sp12 - 0x2000);
                    }

                    sp8 = (spC - sp10);
                    if (sp8 > 0x2000) {
                        spC = (sp10 + 0x2000);
                    } else if (sp8 < -0x2000) {
                        spC = (sp10 - 0x2000);
                    }
                }

                var_r30->set(spC, spA, spE);
                mDoMtx_stack_c::transS(sp78);
                mDoMtx_stack_c::ZXYrotM(*var_r30);
                mDoMtx_stack_c::multVec(&wolfChainVec, var_r29);

                sp54.set(var_r29->x, 10.0f + var_r29->y, var_r29->z);
                mLinkGndChk.SetPos(&sp54);

                temp_f31 = 3.0f + dComIfG_Bgsp().GroundCross(&mLinkGndChk);
                if (temp_f31 > var_r29->y) {
                    var_r30->x = cM_atan2s(sp6C.y + (temp_f31 - var_r29->y), temp_f30 + (temp_f31 - var_r29->y));

                    cM3dGPla sp84;
                    dComIfG_Bgsp().GetTriPla(mLinkGndChk, &sp84);

                    var_r30->z = fopAcM_getPolygonAngle(&sp84, (var_r30->y + 0x4000));

                    sp8 = var_r30[-1].z - var_r30->z;
                    if ((sp8 >= 0 && sp8 <= 0x4000) || sp8 < -0x4000) {
                        var_r30->z -= 0x2000;
                    } else {
                        var_r30->z += 0x2000;
                    }

                    mDoMtx_stack_c::transS(sp78);
                    mDoMtx_stack_c::ZXYrotM(*var_r30);
                    mDoMtx_stack_c::multVec(&wolfChainVec, var_r29);
                }

                *var_r26 = (*var_r29 - sp60) * 0.8f;
                sp78 = *var_r29;
                spE = (var_r30->z + 0x4000);
            }
        }

        for (i = 0; i < 4; i++) {
            mDoMtx_stack_c::transS(field_0x363c[i]);
            mDoMtx_stack_c::ZXYrotM(field_0x3142[i]);
            mpWlChainModels[i]->setBaseTRMtx(mDoMtx_stack_c::get());
            mpWlChainModels[i]->calc();
        }
    }
}

void daAlink_c::setWolfAnmVoice() {
    static u32 const label[12] = {
        Z2SE_WL_V_BREATH_SIT, Z2SE_WL_V_BREATH_WAIT, Z2SE_WL_V_BREATH_WALK, Z2SE_WL_V_BREATH_TIRED,
        Z2SE_WL_V_DASH,       Z2SE_WL_V_BREATH_JUMP, Z2SE_WL_V_SLIP_CLIMB,  Z2SE_WL_V_PUSH_ROCK,
        Z2SE_WL_V_TURN_BACK,  Z2SE_WL_V_HOLD_THROW,  Z2SE_WL_V_FAILED,      Z2SE_WL_V_NOSE,
    };

    s8* var_r29 = (s8*)field_0x2160;

    if (field_0x2fd8 >= 12 || field_0x215c == NULL) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (*var_r29 == -1) {
            return;
        }

        if (field_0x215c->checkPass(*var_r29)) {
            voiceStart(label[field_0x2fd8]);
        }
        var_r29++;
    }
}

int daAlink_c::procWolfServiceWaitInit(int param_0) {
    commonProcInit(PROC_WOLF_SERVICE_WAIT);
    mProcVar5.field_0x3012 = 0;

    if (param_0 == 2) {
        setSingleAnimeWolfBase(WANM_SERVICE_WAIT_D);
        field_0x3198 = 0;
        mProcVar2.field_0x300c = 0;
        mProcVar5.field_0x3012 = 1;
    } else if (param_0 == 1) {
        if (cM_rnd() < 0.5f) {
            setSingleAnimeWolfBase(WANM_SERVICE_WAIT_A);
            setFaceBasicTexture(FTANM_UNK_8D);
            field_0x3198 = 0;
            mProcVar2.field_0x300c = 0;
        } else {
            setSingleAnimeWolf(WANM_SIT, 1.0f, mpHIO->mWolf.mWlLie.m.mSittingAnm.mStartFrame,
                               mpHIO->mWolf.mWlLie.m.mSittingAnm.mEndFrame,
                               mpHIO->mWolf.mWlLie.m.mSittingAnm.mInterpolation);
            field_0x3198 = 0;
            mProcVar2.field_0x300c = 1;
        }
    } else {
        setSingleAnimeWolfBase(WANM_WAIT_A_TO_S);
        field_0x3198 = 0;
        mProcVar2.field_0x300c = 0;
    }

    mProcVar4.field_0x3010 = 0;
    mProcVar0.field_0x3008 = cM_rndF(150.0f) + 150.0f;
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x2f98 = 4;
    return 1;
}

int daAlink_c::procWolfServiceWait() {
    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];

    if (mDemo.getDemoMode() != 0x5E && !checkNextActionWolf(0) && !checkFrontWallTypeAction() && ((checkEventRun() && mProcVar5.field_0x3012 == 0) || shape_angle.y != field_0x2fe6)) {
        temp_r30->setRate(0.0f);
        field_0x3198 = 1;
        procWolfWaitInit();
    } else if (checkAnmEnd(temp_r30)) {
        if (checkUnderMove0BckNoArcWolf(WANM_SERVICE_WAIT_A)) {
            setSingleAnimeWolfBase(WANM_WAIT_A_TO_S);
            field_0x3198 = 0;
            mProcVar2.field_0x300c = 0;
        } else if (mProcVar2.field_0x300c != 0) {
            setSingleAnimeWolfBaseSpeed(WANM_WAIT_SIT, mpHIO->mWolf.mWlLie.m.mSittingIdleAnmSpeed, mpHIO->mWolf.mWlLie.m.mSittingIdleInterp);
        } else if (mProcVar4.field_0x3010 == 0) {
            setSingleAnimeWolfBase(WANM_SERVICE_WAIT_D);
        } else {
            procWolfServiceWaitInit(1);
        }
    } else if (checkUnderMove0BckNoArcWolf(WANM_WAIT_SIT)) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == 0) {
            setSingleAnimeWolfBaseSpeed(WANM_SIT, -1.0f, 2.0f);
            field_0x3198 = 1;
            mProcVar2.field_0x300c = 0;
        }
    } else if (checkUnderMove0BckNoArcWolf(WANM_SERVICE_WAIT_A) && temp_r30->checkPass(25.0f)) {
        voiceStart(Z2SE_WL_V_YAWN);
    } else if (checkUnderMove0BckNoArcWolf(WANM_SERVICE_WAIT_D) && mProcVar5.field_0x3012 == 0) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == 0 && mDemo.getDemoMode() != 0x5E) {
            setSingleAnimeWolfBaseSpeed(WANM_WAIT_A_TO_S, -1.0f, 3.0f);
            mProcVar4.field_0x3010 = 1;
        }
    }

    return 1;
}

int daAlink_c::procWolfTiredWaitInit() {
    if (checkWolfWaitSlipPolygon() && !checkInputOnR()) {
        return procWolfWaitSlipInit();
    }

    if (!commonProcInitNotSameProc(PROC_WOLF_TIRED_WAIT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    setSingleAnimeWolfBase(WANM_WAIT_TO_TIRED);
    return 1;
}

int daAlink_c::procWolfTiredWait() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (!checkNextActionWolf(0) && frameCtrl->checkAnmEnd()) {
        setSingleAnimeWolfBase(WANM_WAIT_TIRED);
    }

    return 1;
}

int daAlink_c::procWolfMidnaRideShockInit() {
    if (!checkWolf()) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
        return procWaitInit();
    }

    if (!commonProcInitNotSameProc(PROC_WOLF_MIDNA_RIDE_SHOCK)) {
        return 0;
    }

    setSingleAnimeWolfBase(WANM_MIDNA_SHOCK);
    setFaceBasicTexture(FTANM_UNK_9F);

    voiceStart(Z2SE_WL_V_MD_SHOCK);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;

    return 1;
}

int daAlink_c::procWolfMidnaRideShock() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (frameCtrl->checkAnmEnd()) {
        if (mDemo.getDemoMode() == 0x32) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextActionWolf(0);
        }
    } else if (frameCtrl->getFrame() > 11.0f && mDemo.getDemoMode() != 0x32) {
        checkNextActionWolf(1);
    }

    return 1;
}

int daAlink_c::procWolfWaitInit() {
    if (checkWolfWaitSlipPolygon() && !checkInputOnR()) {
        return procWolfWaitSlipInit();
    }

    if (mProcID == PROC_WOLF_WAIT) {
        return 0;
    }

    if (mProcID == PROC_WOLF_SERVICE_WAIT && (!checkEventRun() || mProcVar5.field_0x3012 != 0) &&
        (field_0x3198 == 0 || !checkAnmEnd(mUnderFrameCtrl)) && checkNoUpperAnime())
    {
        return 0;
    }

    commonProcInit(PROC_WOLF_WAIT);
    setBlendWolfMoveAnime(mpHIO->mWolf.mWlMove.m.mIdleInterpolation);
    current.angle.y = shape_angle.y;
    field_0x2f98 = 4;
    mNormalSpeed = 0.0f;
    initServiceWaitTime();
    return 1;
}

int daAlink_c::procWolfWait() {
    s16 old_shapeangle = shape_angle.y;

    if (!checkNextActionWolf(0) && !checkFrontWallTypeAction()) {
        field_0x33f0 = (s16)(shape_angle.y - old_shapeangle) * 0.004999999888241291f;
        setBlendWolfMoveAnime(-1.0f);

        if (checkServiceWaitMode()) {
            field_0x30ca--;

            if (field_0x30ca == 0) {
                procWolfServiceWaitInit(0);
            }
        } else {
            initServiceWaitTime();
        }
    }

    return 1;
}

int daAlink_c::procWolfMoveInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_MOVE)) {
        return 0;
    }

    setBlendWolfMoveAnime(mpHIO->mWolf.mWlMove.m.mNormalInterpolation);
    setFootEffectProcType(3);

    return 1;
}

int daAlink_c::procWolfMove() {
    if (!checkNextActionWolf(0) && !checkFrontWallTypeAction()) {
        if (mDemo.getDemoMode() == 2) {
            if (mNormalSpeed > field_0x594 * mpHIO->mWolf.mWlMoveNoP.m.mWalkToJogRate) {
                mNormalSpeed = field_0x594 * mpHIO->mWolf.mWlMoveNoP.m.mWalkToJogRate;
            }
        }

        setBlendWolfMoveAnime(-1.0f);
        field_0x30a2 = field_0x2fec;
    }

    return 1;
}

int daAlink_c::procWolfDashInit() {
    commonProcInit(PROC_WOLF_DASH);
    setSingleAnimeWolfParam(WANM_DASH_START, &mpHIO->mWolf.mWlMove.m.mADashAnm);
    setFootEffectProcType(3);
    current.angle.y = shape_angle.y;

    onNoResetFlg1(FLG1_DASH_MODE);
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;

    if (checkWolfSlowDash()) {
        field_0x30d0 = mpHIO->mWolf.mWlMove.m.mADashDurationSlow;
        field_0x594 = mpHIO->mWolf.mWlMove.m.mADashMaxSpeedSlow;

        if (mNormalSpeed < mpHIO->mWolf.mWlMove.m.mADashInitSpeedSlow) {
            mNormalSpeed = mpHIO->mWolf.mWlMove.m.mADashInitSpeedSlow;
        }
    } else if (field_0x2fc7 == 2) {
        field_0x594 = mpHIO->mWolf.mWlMove.m.mADashMaxSpeedSlow2;
        field_0x594 = mpHIO->mWolf.mWlMove.m.mADashMaxSpeedSlow2;

        if (mNormalSpeed < mpHIO->mWolf.mWlMove.m.mADashInitSpeedSlow2) {
            mNormalSpeed = mpHIO->mWolf.mWlMove.m.mADashInitSpeedSlow2;
        }
    } else {
        field_0x30d0 = mpHIO->mWolf.mWlMove.m.mADashDuration;
        field_0x594 = mpHIO->mWolf.mWlMove.m.mADashMaxSpeed;

        if (mNormalSpeed < mpHIO->mWolf.mWlMove.m.mADashInitSpeed) {
            mNormalSpeed = mpHIO->mWolf.mWlMove.m.mADashInitSpeed;
        }
    }

    return 1;
}

int daAlink_c::procWolfDash() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    cM3dGPla poly;

    if (getSlidePolygon(&poly)) {
        return checkNextActionWolf(0);
    }

    f32 step_amount;
    if (checkWolfSlowDash()) {
        step_amount = mpHIO->mWolf.mWlMove.m.mADashAccelerationSlow;
    } else if (field_0x2fc7 == 2) {
        step_amount = mpHIO->mWolf.mWlMove.m.mADashAccelerationSlow2;
    } else {
        step_amount = mpHIO->mWolf.mWlMove.m.mADashAcceleration;
    }
    cLib_chaseF(&mNormalSpeed, field_0x594, step_amount);

    if (checkInputOnR()) {
        cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, mpHIO->mWolf.mWlMove.m.mTurnAngleRate,
                           mpHIO->mWolf.mWlMove.m.mTurnMax, mpHIO->mWolf.mWlMove.m.mTurnMin);
        shape_angle.y = current.angle.y;
    }

    if (frameCtrl_p->getFrame() > 3.0f && checkWolfAttackReverse(1)) {
        return 1;
    }

    if (swordSwingTrigger()) {
        mProcVar3.field_0x300e = 1;
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar3.field_0x300e != 0) {
            return checkWolfAttackAction();
        }

        if (checkWolfSlowDash()) {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTimeSlow;
        } else if (field_0x2fc7 == 2) {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTimeSlow2;
        } else {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTime;
        }

        return checkNextActionWolf(0);
    } else if (frameCtrl_p->getFrame() > mpHIO->mWolf.mWlMove.m.mADashAnm.mCancelFrame) {
        if (mProcVar3.field_0x300e != 0) {
            return checkWolfAttackAction();
        }

        if (checkWolfSlowDash()) {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTimeSlow;
        } else if (field_0x2fc7 == 2) {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTimeSlow2;
        } else {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTime;
        }

        checkNextActionWolf(1);
        return 1;
    } else if (frameCtrl_p->checkPass(2.5f) || frameCtrl_p->checkPass(14.0f)) {
        field_0x2f9d = 0x60;
        onResetFlg1(RFLG1_UNK_30);
    } else if (frameCtrl_p->checkPass(8.0f) || frameCtrl_p->checkPass(16.5f)) {
        field_0x2f9d = 0x18;
        onResetFlg1(RFLG1_UNK_C);
    }

    field_0x30a2 = field_0x2fec;
    return 1;
}

int daAlink_c::procWolfDashReverseInit(int param_0) {
    commonProcInit(PROC_WOLF_DASH_REVERSE);
    setSingleAnimeWolf(WANM_ATTACK_RECOIL_GROUND, mpHIO->mWolf.mWlMove.m.mDashReboundAnm.mSpeed,
                       mpHIO->mWolf.mWlMove.m.mDashReboundAnm.mStartFrame, 5,
                       mpHIO->mWolf.mWlMove.m.mDashReboundAnm.mInterpolation);

    if (param_0) {
        current.angle.y = field_0x3102 + 0x8000;
        if (cLib_distanceAngleS(current.angle.y, shape_angle.y) > 0x6000) {
            mNormalSpeed = 10.0f;
            speed.y = 10.0f;
        } else {
            mNormalSpeed = 20.0f;
            speed.y = 15.0f;
        }

        mProcVar2.field_0x300c = 1;
    } else {
        mNormalSpeed = mpHIO->mWolf.mWlMove.m.mDashReboundHorizontalSpeed;
        speed.y = mpHIO->mWolf.mWlMove.m.mDashReboundVerticalSpeed;
        current.angle.y += 0x8000;
        mProcVar2.field_0x300c = 0;
    }

    dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
    dKy_Sound_set(current.pos, 100, fopAcM_GetID(this), 5);
    seStartOnlyReverb(Z2SE_WOLF_BODYATTACK);
    field_0x3588 = l_wolfBaseAnime;
    onResetFlg0(RFLG0_FRONT_ROLL_CRASH);
    return 1;
}

int daAlink_c::procWolfDashReverse() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c == 0) {
        field_0x2f99 = 4;
    }

    if (!checkModeFlg(0x2)) {
        if (checkAnmEnd(frameCtrl_p)) {
            checkNextActionWolf(0);
        } else if (frameCtrl_p->getFrame() > mpHIO->mWolf.mWlMove.m.mDashReboundAnm.mCancelFrame) {
            checkNextActionWolf(1);
        }
    } else if (mProcVar2.field_0x300c != 0) {
        mProcVar2.field_0x300c = 0;
    } else if (((mLinkAcch.ChkGroundHit() || checkEndResetFlg2(ERFLG2_UNK_100)) &&
                checkModeFlg(0x2)))
    {
        if (checkWolfLandAction(0)) {
            return 1;
        }

        mNormalSpeed = 0.0f;
        frameCtrl_p->setFrame(mpHIO->mWolf.mWlMove.m.mDashReboundAnm.mSpeed + frameCtrl_p->getFrame());
        getNowAnmPackUnder(UNDER_0)->setFrame(frameCtrl_p->getFrame());
        frameCtrl_p->setRate(mpHIO->mWolf.mWlMove.m.mDashReboundAnm.mSpeed);
        frameCtrl_p->setEnd(mpHIO->mWolf.mWlMove.m.mDashReboundAnm.mEndFrame);

        current.angle.y = shape_angle.y;
        offModeFlg(0x2);
        onModeFlg(0x8000);
    }

    return 1;
}

int daAlink_c::procWolfAtnActorMoveInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_ATN_AC_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    setWolfAtnMoveDirection();
    setBlendWolfAtnMoveAnime(mpHIO->mWolf.mWlMove.m.mNormalInterpolation);

    if (checkWolfAtnWait()) {
        voiceStartLevel(Z2SE_WL_V_ROAR);
    }

    return 1;
}

int daAlink_c::procWolfAtnActorMove() {
    setWolfAtnMoveDirection();

    if (!checkNextActionWolf(0)) {
        if (checkZeroSpeedF()) {
            onModeFlg(1);

            if (checkWolfWaitSlipPolygon() && !checkInputOnR()) {
                return procWolfWaitSlipInit();
            }
        } else {
            offModeFlg(1);
        }

        setBlendWolfAtnMoveAnime(-1.0f);

        if (checkWolfAtnWait()) {
            voiceStartLevel(Z2SE_WL_V_ROAR);
        }
    }

    return 1;
}

int daAlink_c::procWolfWaitTurnInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_WAIT_TURN)) {
        return 0;
    }

    f32 anm_speed;
    if (checkWolfDashMode()) {
        anm_speed = mpHIO->mWolf.mWlMove.m.mWalkAnmSpeed;
    } else {
        anm_speed = mpHIO->mWolf.mWlMoveNoP.m.mWalkAnmSpeed;
    }

    setSingleAnimeWolfBaseSpeed(WANM_WALK_A, anm_speed, mpHIO->mWolf.mWlMove.m.mNormalInterpolation);

    if (checkEventRun()) {
        mNormalSpeed = 0.0f;
    }

    mProcVar3.field_0x300e = field_0x2fe2;
    current.angle.y = shape_angle.y;
    return 1;
}

int daAlink_c::procWolfWaitTurn() {
    f32 step_amount;
    if (checkWolfDashMode()) {
        step_amount = mpHIO->mWolf.mWlMove.m.mDeceleration;
    } else {
        step_amount = mpHIO->mWolf.mWlMoveNoP.m.mDeceleration;
    }
    cLib_chaseF(&mNormalSpeed, 0.0f, step_amount);

    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    s16 var_r31 = cLib_addCalcAngleS(&shape_angle.y, mProcVar3.field_0x300e, 30, 0x3CDF, 8000);
    current.angle.y = shape_angle.y;

    if (checkNextActionFromButton()) {
        return 1;
    }

    if (var_r31 == 0) {
        if (checkEventRun()) {
            if (mDemo.getDemoMode() == 5) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            } else {
                checkNextActionWolf(0);
            }
        } else {
            checkNextActionWolf(0);
        }
    }

    return 1;
}

int daAlink_c::procWolfSideStepInit(int param_0) {
    if (field_0x2f98 == 1 && !param_0 && checkFirstMidnaDemo() && !checkWolfGrabAnime() &&
        field_0x2fcc != 0)
    {
        return procWolfBackJumpInit(0);
    }

    commonProcInit(PROC_WOLF_SIDESTEP);

    if (field_0x2f98 == 0 || param_0) {
        field_0x2f98 = 1;
    }

    if (field_0x2f98 == 1) {
        if (mTargetedActor != NULL) {
            mProcVar2.field_0x300c = cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos);
        } else {
            mProcVar2.field_0x300c = shape_angle.y;
        }

        current.angle.y = shape_angle.y + 0x8000;
        setSingleAnimeWolfParam(WANM_BACK_JUMP_START, &mpHIO->mWolf.mWlSideStep.m.mBackJumpAnm);
        mNormalSpeed = mpHIO->mWolf.mWlSideStep.m.mBackJumpHorizSpeed;
        speed.y = mpHIO->mWolf.mWlSideStep.m.mBackJumpVertSpeed;
        mProcVar1.field_0x300a = 0;
    } else {
        shape_angle.y = field_0x2fe4;

        daAlink_WANM jump_anm;
        if (field_0x2f98 == 2) {
            jump_anm = WANM_SIDE_JUMP_LEFT_START;
            current.angle.y = shape_angle.y + 0x4000;
        } else {
            jump_anm = WANM_SIDE_JUMP_RIGHT_START;
            current.angle.y = shape_angle.y - 0x4000;
        }

        setSingleAnimeWolfParam(jump_anm, &mpHIO->mWolf.mWlSideStep.m.mSideJumpAnm);
        mNormalSpeed = mpHIO->mWolf.mWlSideStep.m.mSideJumpHorizSpeed;
        speed.y = mpHIO->mWolf.mWlSideStep.m.mSideJumpVertSpeed;
        mProcVar1.field_0x300a = 1;
    }

    mProcVar5.field_0x3012 = 0;
    mProcVar3.field_0x300e = 0;
    voiceStart(Z2SE_WL_V_BREATH_JUMP);
    return 1;
}

int daAlink_c::procWolfSideStep() {
    if (mProcVar1.field_0x300a == 0) {
        cLib_addCalcAngleS(
            &shape_angle.y, mProcVar2.field_0x300c, mpHIO->mWolf.mWlAtnMove.m.mTurnAngleRate,
            mpHIO->mWolf.mWlAtnMove.m.mTurnAngleMax, mpHIO->mWolf.mWlAtnMove.m.mTurnAngleMin);
        current.angle.y = shape_angle.y + 0x8000;
    } else if (mTargetedActor != NULL) {
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchActorAngleY(this, mTargetedActor), 5, 0x5E8,
                           0x13C);

        if (field_0x2f98 == 2) {
            current.angle.y = shape_angle.y + 0x4000;
        } else {
            current.angle.y = shape_angle.y - 0x4000;
        }
    }

    checkWolfAtnDoCharge();

    if (mLinkAcch.ChkGroundHit() && mProcVar3.field_0x300e != 0) {
        return procWolfSideStepLandInit(mProcVar5.field_0x3012);
    }

    if (mDemo.getDemoMode() != 0x10 &&
        current.pos.y < mLastJumpPos.y - mpHIO->mWolf.mWlSideStep.m.mFallHeight)
    {
        field_0x2f98 = 0;
        return procWolfFallInit(2, mpHIO->mWolf.mWlSideStep.m.mFallInterp);
    }

    mProcVar3.field_0x300e = 1;
    return 1;
}

int daAlink_c::procWolfSideStepLandInit(s16 param_0) {
    commonProcInit(PROC_WOLF_SIDESTEP_LAND);

    if (field_0x2f98 == 1) {
        setSingleAnimeWolfParam(WANM_BACK_JUMP_END, &mpHIO->mWolf.mWlSideStep.m.mBackLandAnm);
        field_0x3478 = mpHIO->mWolf.mWlSideStep.m.mBackLandAnm.mCancelFrame;
        field_0x2fcc = 10;
    } else {
        field_0x2fcc = 0;
        setSingleAnimeWolfParam(
            field_0x2f98 == 2 ? WANM_SIDE_JUMP_LEFT_END : WANM_SIDE_JUMP_RIGHT_END,
            // Fakematch, should be
            // &mpHIO->mWolf.mWlSideStep.m.mSideLandAnm
            &daAlinkHIO_wlSideStep_c0::m.mSideLandAnm
        );
        field_0x3478 = mpHIO->mWolf.mWlSideStep.m.mSideLandAnm.mCancelFrame;
    }

    mProcVar5.field_0x3012 = param_0;
    field_0x2f9d = 4;
    setFootEffectProcType(2);
    onResetFlg1(RFLG1_UNK_C);
    mNormalSpeed = 0.0f;
    setStepLandVibration();

    return 1;
}

int daAlink_c::procWolfSideStepLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (field_0x2fcc != 0) {
        field_0x2fcc = 10;
    }

    setWolfAtnMoveDirection();
    checkWolfAtnDoCharge();

    if (frameCtrl->checkAnmEnd()) {
        if (mDemo.getDemoMode() == 0x10) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextActionWolf(0);
        }
    } else if (frameCtrl->getFrame() > field_0x3478) {
        checkNextActionWolf(1);
    }

    return 1;
}

int daAlink_c::procWolfBackJumpInit(int param_0) {
    commonProcInit(PROC_WOLF_BACKJUMP);

    if (param_0 == 1) {
        setSingleAnimeWolfParam(WANM_BACKFLIP_START, &mpHIO->mWolf.mWlAttack.mWlAtDown.m.mBackRollAnm);
        mNormalSpeed = mpHIO->mWolf.mWlAttack.mWlAtDown.m.mLandingSideRollMovementWidth;
        speed.y = mpHIO->mWolf.mWlAttack.mWlAtDown.m.mLandingFrontBackMovementWidth;
        voiceStart(Z2SE_WL_V_BREATH_JUMP);
    } else {
        if (param_0 == 2) {
            setSingleAnimeWolf(WANM_BACKFLIP_START, mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mFlipKickPostAnmSpeed,
                               mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mFlipKickPostStartF,
                               mpHIO->mWolf.mWlBackJump.m.mBackJumpAnm.mEndFrame,
                               mpHIO->mWolf.mWlBackJump.m.mBackJumpAnm.mInterpolation);
        } else {
            setSingleAnimeWolfParam(WANM_BACKFLIP_START, &mpHIO->mWolf.mWlBackJump.m.mBackJumpAnm);
            voiceStart(Z2SE_WL_V_BREATH_JUMP);
        }

        mNormalSpeed = mpHIO->mWolf.mWlBackJump.m.mBackJumpHorizSpeed;
        speed.y = mpHIO->mWolf.mWlBackJump.m.mBackJumpVertSpeed;
    }

    if (mTargetedActor != NULL && param_0 == 0) {
        mProcVar2.field_0x300c = cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos);
    } else {
        mProcVar2.field_0x300c = shape_angle.y;
    }

    current.angle.y = shape_angle.y + 0x8000;
    field_0x3478 = mNormalSpeed;
    mProcVar5.field_0x3012 = 0;

    if (param_0 == 0) {
        mProcVar4.field_0x3010 = 1;
    } else {
        mProcVar4.field_0x3010 = 0;
    }

    return 1;
}

int daAlink_c::procWolfBackJump() {
    cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, mpHIO->mWolf.mWlAtnMove.m.mTurnAngleRate,
                       mpHIO->mWolf.mWlAtnMove.m.mTurnAngleMax, mpHIO->mWolf.mWlAtnMove.m.mTurnAngleMin);
    current.angle.y = shape_angle.y - -0x8000;

    if (mProcVar4.field_0x3010 != 0) {
        checkWolfAtnDoCharge();
    }

    backJumpSpeedDec();

    if (mUnderFrameCtrl[0].checkAnmEnd() && mLinkAcch.ChkGroundHit()) {
        procWolfBackJumpLandInit(mProcVar5.field_0x3012, mProcVar4.field_0x3010);
    } else if (mDemo.getDemoMode() != 0x10 &&
               current.pos.y < mLastJumpPos.y - mpHIO->mWolf.mWlBackJump.m.mFallHeight)
    {
        field_0x2f98 = 0;
        procWolfFallInit(2, mpHIO->mWolf.mWlBackJump.m.mFallInterp);
    }

    return 1;
}

int daAlink_c::procWolfBackJumpLandInit(s16 param_0, s16 param_1) {
    commonProcInit(PROC_WOLF_BACKJUMP_LAND);
    setSingleAnimeWolfParam(WANM_BACKFLIP_END, &mpHIO->mWolf.mWlBackJump.m.mBackLandAnm);
    mNormalSpeed = 0.0f;
    field_0x2f9d = 4;
    setFootEffectProcType(2);
    field_0x2fcc = 10;
    mProcVar5.field_0x3012 = param_0;
    mProcVar4.field_0x3010 = param_1;
    setStepLandVibration();

    return 1;
}

int daAlink_c::procWolfBackJumpLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (field_0x2fcc != 0) {
        field_0x2fcc = 10;
    }

    setWolfAtnMoveDirection();

    if (mProcVar4.field_0x3010 != 0) {
        checkWolfAtnDoCharge();
    }

    if (frameCtrl->checkAnmEnd()) {
        if (mDemo.getDemoMode() == 0x10) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextActionWolf(0);
        }
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlBackJump.m.mBackLandAnm.mCancelFrame) {
        checkNextActionWolf(1);
    }

    return 1;
}

int daAlink_c::procWolfHowlInit(int param_0) {
    if (checkEventRun() && mProcID == PROC_WOLF_HOWL) {
        return 0;
    }

    commonProcInit(PROC_WOLF_HOWL);

    if (param_0 != 0) {
        setSingleAnimeWolfParam(WANM_HOWL_SIT, &mpHIO->mWolf.mWlHowl.m.mSittingHowlAnm);
        onModeFlg(0x800000);
        field_0x3478 = mpHIO->mWolf.mWlHowl.m.mSittingHowlAnm.mCancelFrame;
        mProcVar3.field_0x300e = 1;
    } else {
        setSingleAnimeWolfParam(WANM_HOWL, &mpHIO->mWolf.mWlHowl.m.mHowlAnm);
        field_0x3478 = mpHIO->mWolf.mWlHowl.m.mHowlAnm.mCancelFrame;
        mProcVar3.field_0x300e = 0;
    }

    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;

    return 1;
}

int daAlink_c::procWolfHowl() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (checkAnmEnd(frameCtrl_p)) {
        if (doButton()) {
            procWolfHowlInit(mProcVar3.field_0x300e);
        } else {
            checkNextActionWolf(0);
        }
    } else if (frameCtrl_p->getFrame() > field_0x3478) {
        if (doButton()) {
            procWolfHowlInit(mProcVar3.field_0x300e);
        } else {
            checkNextActionWolf(1);
        }
    } else {
        if ((!doButton() || mProcVar2.field_0x300c != 0) && mDemo.getDemoMode() != 0x15 &&
            mDemo.getDemoMode() != 0x16)
        {
            if (!(frameCtrl_p->getFrame() < 38.0f)) {
                mProcVar2.field_0x300c = 1;
            }
        } else if (frameCtrl_p->getFrame() > 56.0f) {
            frameCtrl_p->setFrame((frameCtrl_p->getFrame() - 56.0f) + 38.0f);
        }

        if (frameCtrl_p->getFrame() >= 38.0f && frameCtrl_p->getFrame() <= 56.0f) {
            onResetFlg0(RFLG0_UNK_40000000);
            voiceStartLevel(Z2SE_WL_V_HOWL);
            dKy_Sound_set(current.pos, 100, fopAcM_GetID(this), 1);
        }
    }

    return 1;
}

int daAlink_c::procWolfAutoJumpInit(int param_0) {
    commonProcInit(PROC_WOLF_AUTO_JUMP);

    f32 var_f1;
    f32 var_f31;
    f32 var_f30;
    s16 var_r30;

    if (checkWolfDashMode() && field_0x2fc7 != 3) {
        var_f1 = mpHIO->mWolf.mWlAutoJump.m.mJumpAnm.mSpeed;
        field_0x594 = mpHIO->mWolf.mWlAutoJump.m.mJumpMaxSpeed;
        var_f31 = mpHIO->mWolf.mWlAutoJump.m.mJumpMinSpeed;
        var_f30 = mpHIO->mWolf.mWlAutoJump.m.mJumpSpeedRate;
        var_r30 = mpHIO->mWolf.mWlAutoJump.m.mJumpAngle;
        onNoResetFlg2(FLG2_WOLF_DASH_AUTO_JUMP);
    } else {
        var_f1 = mpHIO->mWolf.mWlAutoJump.m.mWeakJumpAnmSpeed;
        field_0x594 = mpHIO->mWolf.mWlAutoJump.m.mWeakJumpMaxSpeed;
        var_f31 = mpHIO->mWolf.mWlAutoJump.m.mWeakJumpMinSpeed;
        var_f30 = mpHIO->mWolf.mWlAutoJump.m.mWeakJumpSpeedRate;
        var_r30 = mpHIO->mWolf.mWlAutoJump.m.mWeakJumpAngle;
    }

    setSingleAnimeWolf(WANM_JUMP_ATTACK_START, var_f1,
                       mpHIO->mWolf.mWlAutoJump.m.mJumpAnm.mStartFrame,
                       mpHIO->mWolf.mWlAutoJump.m.mJumpAnm.mEndFrame,
                       mpHIO->mWolf.mWlAutoJump.m.mJumpAnm.mInterpolation);
    if (speedF > field_0x594 || param_0 || mpHIO->mWolf.mWlAutoJump.m.mAlwaysMaxSpeedJump == true) {
        speedF = field_0x594;
    } else if (speedF < var_f31) {
        speedF = var_f31;
    }

    mNormalSpeed = speedF * var_f30;
    speed.y = mNormalSpeed * cM_ssin(var_r30);
    mNormalSpeed *= cM_scos(var_r30);

    field_0x3588 = l_wolfBaseAnime;
    field_0x33b0 = l_wolfBaseAnime.y;
    current.angle.y = shape_angle.y;

    offNoResetFlg0(FLG0_UNK_40000);
    onResetFlg0(RFLG0_UNK_100);
    voiceStart(Z2SE_WL_V_BREATH_JUMP);
    mProcVar2.field_0x300c = 1;
    return 1;
}

int daAlink_c::procWolfAutoJump() {
    if (mSpecialMode != 0x27) {
        if (checkInputOnR() && getDirectionFromCurrentAngle() == DIR_BACKWARD) {
            cLib_chaseF(&mNormalSpeed, 0.0f, field_0x33a8 * 0.2f);
        } else if (!checkInputOnR()) {
            cLib_chaseF(&mNormalSpeed, 0.0f, 0.1f);
        }
    }

    field_0x2f99 = 7;

    if (mLinkAcch.ChkGroundHit()) {
        return checkWolfLandAction(0);
    }

    if (checkFrontWallTypeAction()) {
        return 1;
    }

    if (checkWolfRopeJumpHang()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (!checkUnderMove0BckNoArcWolf(WANM_FALL_LAND) &&
        (mFallHeight - current.pos.y) * 0.01f > mpHIO->mWolf.mWlDamage.mFall.m.mAirAnmTransitionHeight)
    {
        field_0x2f99 = 12;
        setSingleAnimeWolfBaseSpeed(WANM_FALL_LAND, 0.0f, mpHIO->mWolf.mWlDamage.mFall.m.mAirAnmInterp);
    } else if (checkUnderMove0BckNoArcWolf(WANM_JUMP_ATTACK_START)) {
        if (speed.y < -gravity) {
            field_0x2f99 = 12;
            setSingleAnimeWolfParam(WANM_JUMP_ATTACK, &mpHIO->mWolf.mWlAutoJump.m.mClimbAnm);
        } else if (checkAnmEnd(frameCtrl_p)) {
            field_0x2f99 = 12;
            setSingleAnimeWolf(WANM_JUMP_ATTACK, 0.0f,
                               mpHIO->mWolf.mWlAutoJump.m.mClimbAnm.mStartFrame,
                               mpHIO->mWolf.mWlAutoJump.m.mClimbAnm.mEndFrame,
                               mpHIO->mWolf.mWlAutoJump.m.mClimbAnm.mInterpolation);
            mProcVar2.field_0x300c = 0;
        }
    } else if (mProcVar2.field_0x300c == 0 && checkUnderMove0BckNoArcWolf(WANM_JUMP_ATTACK) &&
               speed.y < -gravity)
    {
        mProcVar2.field_0x300c = 1;
        frameCtrl_p->setRate(mpHIO->mWolf.mWlAutoJump.m.mClimbAnm.mSpeed);
    }

    return 1;
}

int daAlink_c::procWolfFallInit(int param_0, f32 i_morf) {
    if (!commonProcInitNotSameProc(PROC_WOLF_FALL)) {
        return 0;
    }

    mProcVar5.field_0x3012 = 0;

    if (param_0 == 1) {
        mNormalSpeed = 0.0f;
        speed.y = 0.0f;
        current.angle.y = shape_angle.y;
    } else if (param_0 == 4) {
        current.angle.y = field_0x2ffe;
        dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
        setDamagePointNormal(field_0x318c);
        onNoResetFlg1(FLG1_THROW_DAMAGE);

        if (mDamageTimer != 0) {
            onModeFlg(8);
        }

        mNormalSpeed = field_0x3408;
        speed.y = field_0x340c;
        mProcVar5.field_0x3012 = 1;
    }

    setSingleAnimeWolfBaseSpeed(WANM_JUMP_ATTACK, 0.0f, i_morf);

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    frameCtrl_p->setFrame(frameCtrl_p->getEnd() - 0.001f);
    getNowAnmPackUnder(UNDER_0)->setFrame(frameCtrl_p->getFrame());

    field_0x3588 = l_wolfBaseAnime;
    field_0x2f99 = 12;
    resetBasAnime();

    offNoResetFlg0(FLG0_UNK_40000);
    if (param_0 != 4 && current.angle.y == shape_angle.y) {
        if (mNormalSpeed > 1.0f) {
            mProcVar0.field_0x3008 = 2;
        } else {
            mProcVar0.field_0x3008 = 1;
        }
    } else {
        mProcVar0.field_0x3008 = 0;
    }

    mProcVar1.field_0x300a = 7;
    return 1;
}

int daAlink_c::procWolfFall() {
    if (mProcVar5.field_0x3012 == 0) {
        cLib_chaseF(&mNormalSpeed, 0.0f, 0.1f);
    } else if (mProcVar5.field_0x3012 > 0) {
        mProcVar5.field_0x3012 = -1;
        return 1;
    }

    field_0x2f99 = 4;

    if (mLinkAcch.ChkGroundHit()) {
        return checkWolfLandAction(0);
    }

    if (mProcVar0.field_0x3008 == 1) {
        if (mProcVar1.field_0x300a > 0) {
            mProcVar1.field_0x300a--;
        } else if (checkInputOnR() && getDirectionFromShapeAngle() == DIR_FORWARD &&
                   checkFrontWallTypeAction())
        {
            return 1;
        }
    } else if (mProcVar0.field_0x3008 == 2 && checkFrontWallTypeAction()) {
        return 1;
    }

    if (checkWolfRopeJumpHang()) {
        return 1;
    }

    if (!checkUnderMove0BckNoArcWolf(WANM_FALL_LAND) &&
        (mFallHeight - current.pos.y) * 0.01f > mpHIO->mWolf.mWlDamage.mFall.m.mAirAnmTransitionHeight)
    {
        field_0x2f99 = 12;
        setSingleAnimeWolfBaseSpeed(WANM_FALL_LAND, 0.0f, mpHIO->mWolf.mWlDamage.mFall.m.mAirAnmInterp);
    }

    return 1;
}

int daAlink_c::procWolfLandInit() {
    commonProcInit(PROC_WOLF_LAND);
    mNormalSpeed = 0.0f;
    field_0x2f99 = 12;

    setSingleAnimeWolfParam(WANM_JUMP_ATTACK_END, &mpHIO->mWolf.mWlAutoJump.m.mLandingAnm);
    field_0x3478 = mpHIO->mWolf.mWlAutoJump.m.mLandingAnm.mCancelFrame;
    mProcVar2.field_0x300c = 1;

    field_0x2f9d = 4;
    setFootEffectProcType(4);

    dComIfGp_getVibration().StartShock(1, 15, cXyz(0.0f, 1.0f, 0.0f));
    return 1;
}

int daAlink_c::procWolfLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c != 0) {
        field_0x2f99 = 4;
    }

    if (frameCtrl->checkAnmEnd()) {
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > field_0x3478) {
        checkNextActionWolf(1);
    }

    return 1;
}

int daAlink_c::procWolfSitInit(int param_0) {
    if (!commonProcInitNotSameProc(PROC_WOLF_SIT)) {
        return 0;
    }

    if (param_0 != 0 && !checkUnderMove0BckNoArcWolf(WANM_WAIT_SIT) &&
        !checkUnderMove0BckNoArcWolf(WANM_HOWL_SIT))
    {
        setSingleAnimeWolfParam(WANM_SIT, &mpHIO->mWolf.mWlLie.m.mSittingAnm);
        field_0x3198 = 0;
    } else {
        field_0x3198 = 1;

        if (!checkUnderMove0BckNoArcWolf(WANM_WAIT_SIT)) {
            setSingleAnimeWolfBaseSpeed(WANM_WAIT_SIT, mpHIO->mWolf.mWlLie.m.mSittingIdleAnmSpeed,
                                        mpHIO->mWolf.mWlLie.m.mSittingIdleInterp);
        }
    }

    current.angle.y = shape_angle.y;
    return 1;
}

int daAlink_c::procWolfSit() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    cLib_chaseF(&mNormalSpeed, 0.0f,
                checkWolfDashMode() ? mpHIO->mWolf.mWlMove.m.mDeceleration :
                                      mpHIO->mWolf.mWlMoveNoP.m.mDeceleration);

    if (!spActionButton() && mDemo.getDemoMode() != 0xF) {
        checkNextActionWolfFromLie();
    } else if (frameCtrl->checkAnmEnd()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT_SIT, mpHIO->mWolf.mWlLie.m.mSittingIdleAnmSpeed,
                                    mpHIO->mWolf.mWlLie.m.mSittingIdleInterp);
        field_0x3198 = 1;
    }

    return 1;
}

int daAlink_c::procWolfStepMoveInit() {
    commonProcInit(PROC_WOLF_STEP_MOVE);
    setSingleAnimeWolfParam(WANM_BUMP_START, &mpHIO->mWolf.mWlWallHang.m.mLedgeClimbAnm);
    field_0x3478 = 0.5f * mNormalSpeed;
    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;

    field_0x3588 = l_wolfBaseAnime;
    current.pos.x -= cM_ssin(shape_angle.y) * 10.0f;
    current.pos.z -= cM_scos(shape_angle.y) * 10.0f;

    field_0x2060->getOldFrameTransInfo(0)->mTranslate.z += 10.0f;
    return 1;
}

int daAlink_c::procWolfStepMove() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c != 0) {
        field_0x2f99 = 4;

        if (checkAnmEnd(frameCtrl_p)) {
            return checkNextActionWolf(0);
        }

        if (frameCtrl_p->getFrame() > mpHIO->mWolf.mWlWallHang.m.mLedgeLandAnm.mCancelFrame) {
            mNormalSpeed = field_0x3478;

            if (checkNextActionWolf(1)) {
                return 1;
            }

            mNormalSpeed = 0.0f;
        }
    } else if (checkAnmEnd(frameCtrl_p)) {
        setSingleAnimeWolfParam(WANM_BUMP_END, &mpHIO->mWolf.mWlWallHang.m.mLedgeLandAnm);
        current.pos.set(field_0x34ec.x + cM_ssin(shape_angle.y) * 2.0f, field_0x34ec.y,
                        field_0x34ec.z + cM_scos(shape_angle.y) * 2.0f);
        field_0x2f99 = 0x50;
        mProcVar2.field_0x300c = 1;
    }

    return 1;
}

int daAlink_c::procWolfSlipInit() {
    commonProcInit(PROC_WOLF_SLIP);
    setSingleAnimeWolfParam(WANM_SLIP, &mpHIO->mWolf.mWlMove.m.mSlipAnm);

    if (checkWolfDashMode()) {
        mNormalSpeed = speedF * mpHIO->mWolf.mWlMove.m.mSlipInitSpeedRate;
    } else {
        mNormalSpeed = speedF * mpHIO->mWolf.mWlMoveNoP.m.mSlideInitSpeed;
    }

    current.angle.y = shape_angle.y;
    field_0x3588 = l_wolfBaseAnime;
    setFootEffectProcType(1);
    return 1;
}

int daAlink_c::procWolfSlip() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    field_0x2f99 = 5;

    f32 var_f2;
    if (checkWolfDashMode()) {
        var_f2 = mpHIO->mWolf.mWlMove.m.mSlipDeceleration;
    } else {
        var_f2 = mpHIO->mWolf.mWlMoveNoP.m.mSlideDeceleration;
    }
    cLib_chaseF(&mNormalSpeed, 0.0f, var_f2);

    if (checkAnmEnd(frameCtrl_p)) {
        mNormalSpeed = 0.0f;

        if (checkInputOnR()) {
            procWolfSlipTurnInit(1);
        } else {
            current.pos.x += cM_scos(shape_angle.y) * 30.0f;
            current.pos.z -= cM_ssin(shape_angle.y) * 30.0f;

            field_0x2060->getOldFrameTransInfo(0)->mTranslate.x -= 30.0f;
            checkNextActionWolf(0);
        }
    } else {
        if (mNormalSpeed > 2.5f && frameCtrl_p->getFrame() >= 5.0f) {
            field_0x2f9d = 0x78;
        }

        seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
    }

    return 1;
}

int daAlink_c::procWolfSlipTurnInit(int param_0) {
    if (!commonProcInitNotSameProc(PROC_WOLF_SLIP_TURN)) {
        return 0;
    }

    f32 var_f31;
    if (param_0) {
        var_f31 = mpHIO->mWolf.mWlMove.m.mReverseAnm.mStartFrame;
        current.angle.y = shape_angle.y + 0x8000;

        if (checkWolfDashMode()) {
            field_0x347c = mpHIO->mWolf.mWlMove.m.mReverseSpeed;
        } else {
            field_0x347c = mpHIO->mWolf.mWlMoveNoP.m.mReverseSpeed;
        }
    } else {
        var_f31 = mpHIO->mWolf.mWlMove.m.mIdleReverseStartFrame;
        current.angle.y = field_0x2fe2;
        setOldRootQuaternion(0, (shape_angle.y - current.angle.y) - 0x8000, 0);
        shape_angle.y = current.angle.y + 0x8000;
        field_0x347c = 10.0f;
    }

    setSingleAnimeWolf(WANM_TURN, mpHIO->mWolf.mWlMove.m.mReverseAnm.mSpeed, var_f31, 12,
                       mpHIO->mWolf.mWlMove.m.mReverseAnm.mInterpolation);
    voiceStart(Z2SE_WL_V_BREATH_JUMP);

    mNormalSpeed = 0.0f;
    field_0x2f99 = 5;
    field_0x3588 = l_wolfBaseAnime;
    field_0x3478 = 1.0f / (12.0f - var_f31);
    return 1;
}

int daAlink_c::procWolfSlipTurn() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    field_0x2f99 = 5;

    f32 var_f31 =
        (0.5f - field_0x3478 * (frameCtrl_p->getFrame() - frameCtrl_p->getStart())) * 2.0f;
    field_0x3588.x = l_wolfBaseAnime.x * var_f31;
    field_0x3588.z = l_wolfBaseAnime.z * var_f31;

    if (checkAnmEnd(frameCtrl_p) && mLinkAcch.ChkGroundHit()) {
        return procWolfSlipTurnLandInit();
    }

    if (!checkModeFlg(2) && frameCtrl_p->getFrame() > 4.5f) {
        mNormalSpeed = field_0x347c;
        setJumpMode();
    } else if (checkModeFlg(2) &&
               current.pos.y < mLastJumpPos.y - mpHIO->mWolf.mWlMove.m.mReverseFallHeight)
    {
        setOldRootQuaternion(0, -0x8000, 0);
        shape_angle.y = current.angle.y;
        field_0x2fe6 = shape_angle.y;
        procWolfFallInit(2, mpHIO->mWolf.mWlMove.m.mReverseFallInterpolation);
    }

    return 1;
}

int daAlink_c::procWolfSlipTurnLandInit() {
    commonProcInit(PROC_WOLF_SLIP_TURN_LAND);

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    setFrameCtrl(frameCtrl, 0, 0, mpHIO->mWolf.mWlMove.m.mReverseAnm.mEndFrame,
                 mpHIO->mWolf.mWlMove.m.mReverseAnm.mSpeed,
                 frameCtrl->getFrame() + mpHIO->mWolf.mWlMove.m.mReverseAnm.mSpeed);

    mNormalSpeed = 0.25f * mpHIO->mWolf.mWlMoveNoP.m.mMaxSpeed;
    field_0x2f99 = 5;
    setFootEffectProcType(2);
    field_0x2f9d = 8;
    field_0x2f98 = 0;
    return 1;
}

int daAlink_c::procWolfSlipTurnLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    field_0x2f99 = 5;

    if (frameCtrl->checkAnmEnd()) {
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlMove.m.mReverseAnm.mCancelFrame) {
        checkNextActionWolf(1);
    }

    return 1;
}

int daAlink_c::procWolfSlideReadyInit(s16 i_angle, int param_1) {
    commonProcInit(PROC_WOLF_SLIDE_READY);
    field_0x3090 = 8;
    current.angle.y = i_angle;
    setSingleAnimeWolfParam(WANM_SLIDE_FORWARD_START, &mpHIO->mWolf.mWlSlide.m.mFrontSlideAnm);
    voiceStart(Z2SE_WL_V_SLIP_ROLL);

    mNormalSpeed = 0.0f;
    field_0x594 = mpHIO->mWolf.mWlSlide.m.mMaxSpeed;
    field_0x3588 = l_wolfBaseAnime;
    field_0x3478 = 1.0f / mUnderFrameCtrl[0].getEnd();
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = param_1;
    field_0x814.SetWeight(0xFF);
    return 1;
}

int daAlink_c::procWolfSlideReady() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    field_0x2f99 = 5;

    f32 var_f31 = (0.5f - field_0x3478 * frameCtrl_p->getFrame()) * 2.0f;
    field_0x3588.x = l_wolfBaseAnime.x * var_f31;
    field_0x3588.z = l_wolfBaseAnime.z * var_f31;

    cM3dGPla slide_poly;
    if (getSlidePolygon(&slide_poly)) {
        cLib_addCalcAngleS(&current.angle.y, slide_poly.mNormal.atan2sX_Z(), 4, 0x1000, 0x400);
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 4, 0x1000, 0x400);
    }

    if (frameCtrl_p->checkPass(5.0f)) {
        mProcVar2.field_0x300c = 1;
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar3.field_0x300e != 0) {
            procWolfSlideInit(current.angle.y, 1);
        } else {
            procWolfSlopeStartInit(1);
        }
    } else if (frameCtrl_p->checkPass(18.0f)) {
        offModeFlg(0x8000);
    }

    return 1;
}

int daAlink_c::procWolfSlideInit(s16 i_angle, int param_1) {
    if (cLib_distanceAngleS(i_angle, shape_angle.y) < 0x3800 && !param_1) {
        return procWolfSlideReadyInit(i_angle, 1);
    }

    commonProcInit(PROC_WOLF_SLIDE);
    field_0x814.SetWeight(0xFF);
    field_0x3090 = 8;
    current.angle.y = i_angle;
    setSingleAnimeWolfParam(WANM_SLIDE_BACKWARD, &mpHIO->mWolf.mWlSlide.m.mBackSlideAnm);

    if (param_1) {
        mUnderFrameCtrl[0].setFrame(mpHIO->mWolf.mWlSlide.m.mBackSlideAnm.mEndFrame);
        getNowAnmPackUnder(UNDER_0)->setFrame(mpHIO->mWolf.mWlSlide.m.mBackSlideAnm.mEndFrame);
    }

    dComIfGp_setPlayerStatus1(0, 0x200);
    field_0x2f9d = 0x78;
    setFootEffectProcType(1);
    field_0x594 = mpHIO->mWolf.mWlSlide.m.mMaxSpeed;
    return 1;
}

int daAlink_c::procWolfSlide() {
    cM3dGPla slide_poly;

    if (getSlidePolygon(&slide_poly)) {
        s16 var_r30 = slide_poly.mNormal.atan2sX_Z();
        cLib_addCalcAngleS(&current.angle.y, var_r30, 4, 0x1000, 0x400);
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y + 0x8000, 4, 0x1000, 0x400);

        mNormalSpeed += mpHIO->mWolf.mWlSlide.m.mAcceleration *
                        ((1.0f - slide_poly.mNormal.y) * 0.5f + 1.0f) *
                        cM_scos(current.angle.y - var_r30);
        if (mNormalSpeed > field_0x594) {
            mNormalSpeed = field_0x594;
        }

        field_0x2f9d = 0x78;
        seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
    } else {
        procWolfSlideLandInit();
    }

    return 1;
}

int daAlink_c::procWolfSlideLandInit() {
    commonProcInit(PROC_WOLF_SLIDE_LAND);
    setSingleAnimeWolfParam(WANM_SLIDE_BACKWARD_END, &mpHIO->mWolf.mWlSlide.m.mLandingAnm);
    mNormalSpeed *= 0.5f;
    field_0x814.SetWeight(0xFF);

    return 1;
}

int daAlink_c::procWolfSlideLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    cLib_chaseF(&mNormalSpeed, 0.0f, 4.0f);

    if (frameCtrl->checkAnmEnd()) {
        current.angle.y = shape_angle.y;
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlSlide.m.mLandingAnm.mCancelFrame) {
        s16 old_angle = current.angle.y;
        current.angle.y = shape_angle.y;

        if (!checkNextActionWolf(1)) {
            current.angle.y = old_angle;
        }
    }

    return 1;
}

int daAlink_c::procWolfWaitSlipInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_WAIT_SLIP)) {
        return 0;
    }

    setSingleAnimeWolfBaseSpeed(WANM_ATN, mpHIO->mWolf.mWlMove.m.mGazeIdleAnmSpeed,
                                mpHIO->mWolf.mWlMove.m.mIdleInterpolation);
    mNormalSpeed = 0.0f;
    setWolfWaitSlip();
    field_0x2f9d = 120;
    setFootEffectProcType(1);

    return 1;
}

int daAlink_c::procWolfWaitSlip() {
    f32 var_f31 = mNormalSpeed;
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;

    if (checkInputOnR() && checkWolfWaitSlipPolygon()) {
        cM3dGPla tri;
        dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &tri);
        s16 var_r30 = tri.mNormal.atan2sX_Z();

        if (getGroundAngle(&mLinkAcch.m_gnd, field_0x2fe2) < -field_0x3122) {
            int var_r3 = abs((s16)(var_r30 - shape_angle.y));

            if (var_r3 >= 0x6000) {
                return procWolfSlopeStartInit(0);
            } else if (var_r3 <= 0x2000) {
                return procWolfSlideReadyInit(current.angle.y, 0);
            }
        }
    }

    if (!checkNextActionWolf(0)) {
        f32 temp_f1 = setWolfWaitSlip();
        mNormalSpeed = var_f31 + (mpHIO->mWolf.mWlSlide.m.mSlipDownAcceleration * (temp_f1 + 1.0f));
        if (mNormalSpeed > mpHIO->mWolf.mWlSlide.m.mSlipDownMaxSpeed) {
            mNormalSpeed = mpHIO->mWolf.mWlSlide.m.mSlipDownMaxSpeed;
        }

        field_0x2f9d = 0x78;

        s16 var_r4;
        if (mProcVar2.field_0x300c != 0) {
            var_r4 = current.angle.y + 0x8000;
        } else {
            var_r4 = current.angle.y;
        }

        cLib_addCalcAngleS(&shape_angle.y, var_r4, 4, mpHIO->mWolf.mWlSlide.m.mSlipDownRotationMax,
                           mpHIO->mWolf.mWlSlide.m.mSlipDownRotationMin);
        seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
    }

    return 1;
}

int daAlink_c::procWolfSlopeStartInit(int param_0) {
    commonProcInit(PROC_WOLF_SLOPE_START);
    setSingleAnimeWolfParam(WANM_SLIDE_W, &mpHIO->mWolf.mWlSlide.m.mSlipDownRecoveryAnm);
    field_0x3588 = l_wolfBaseAnime;
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;

    if (param_0) {
        field_0x2f99 = 13;
    }

    return 1;
}

int daAlink_c::procWolfSlopeStart() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (frameCtrl->checkAnmEnd()) {
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlSlide.m.mSlipDownRecoveryAnm.mCancelFrame) {
        if (!checkNextActionWolf(1)) {
            field_0x2f99 = 5;
        }
    } else {
        field_0x2f99 = 5;
    }

    return 1;
}

static fopAc_ac_c* daAlink_searchGoldWolf(fopAc_ac_c* i_actor, void*) {
    if (fopAcM_GetName(i_actor) == PROC_NPC_GWOLF) {
        return i_actor;
    }

    return NULL;
}

static void* daAlink_searchWolfHowl(fopAc_ac_c* i_actor, void* i_data) {
    s16 actor_name = fopAcM_GetName(i_actor);
    daAlink_c* player_p = daAlink_getAlinkActorClass();

    if ((actor_name == PROC_KAGO && dComIfGs_isEventBit(0xA10)) ||
        (actor_name == PROC_E_PM &&
         player_p->current.pos.abs2XZ(i_actor->current.pos) < 9000000.0f) ||
        actor_name == PROC_OBJ_SEKIZOA ||
        (*(fopAc_ac_c**)i_data == NULL && actor_name == player_p->getGrassHowlEventActor()))
    {
        *(fopAc_ac_c**)i_data = i_actor;
    }

    return NULL;
}

void daAlink_c::setWolfHowlNotHappen(int param_0) {
    mMsgClassID = fpcM_ERROR_PROCESS_ID_e;

    if (field_0x3198 == 0xEE) {
        field_0x32cc = 0x532;
    } else {
        field_0x32cc = 0x535;
    }

    mProcVar2.field_0x300c = 0;

    if (!checkUnderMove0BckNoArcWolf(WANM_HOWL_END)) {
        if (param_0 != 0) {
            setSingleAnimeWolfBaseSpeed(WANM_WAIT_SIT, mpHIO->mWolf.mWlLie.m.mSittingIdleAnmSpeed,
                                        mpHIO->mWolf.mWlLie.m.mSittingIdleInterp);
        } else {
            setSingleAnimeWolfBase(WANM_SERVICE_WAIT_D);
        }
    }
}

int daAlink_c::procWolfHowlDemoInit() {
    if (!dComIfGp_event_compulsory(this, 0, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();

    s16 name;
    if (field_0x27f4 != NULL) {
        name = fopAcM_GetName(field_0x27f4);
    } else {
        name = PROC_ALINK;
    }

    if (name == PROC_Tag_WaraHowl) {
        shape_angle.y = field_0x27f4->shape_angle.y;
        current.angle.y = shape_angle.y;
    } else {
        f32 var_f31 = cM_ssin(shape_angle.y) * 200.0f;
        f32 var_f30 = cM_scos(shape_angle.y) * 200.0f;

        cXyz sp14;
        cXyz sp8;
        sp14.set(attention_info.position.x, attention_info.position.y - 50.0f,
                 attention_info.position.z);
        sp8.set(sp14.x + var_f31, sp14.y, sp14.z + var_f30);

        if (commonLineCheck(&sp14, &sp8)) {
            sp8.x = sp14.x - var_f31;
            sp8.z = sp14.z - var_f30;

            if (!commonLineCheck(&sp14, &sp8)) {
                shape_angle.y += 0x8000;
            } else {
                sp8.x = sp14.x + var_f30;
                sp8.z = sp14.z - var_f31;

                if (!commonLineCheck(&sp14, &sp8)) {
                    shape_angle.y += 0x4000;
                } else {
                    sp8.x = sp14.x - var_f30;
                    sp8.z = sp14.z + var_f31;

                    if (!commonLineCheck(&sp14, &sp8)) {
                        shape_angle.y -= 0x4000;
                    }
                }
            }
            current.angle.y = shape_angle.y;
        }
    }

    commonProcInit(PROC_WOLF_HOWL_DEMO);
    field_0x3198 = -1;
    mProcVar5.field_0x3012 = -1;
    mProcVar4.field_0x3010 = 0;
    mProcVar0.field_0x3008 = -1;

    if (name != PROC_ALINK) {
        if (name == PROC_Obj_Yobikusa) {
            daObjYobikusa_c* yobikusa_p = (daObjYobikusa_c*)field_0x27f4;

            mZ2WolfHowlMgr.setCorrectCurve(yobikusa_p->getType());
            mProcVar5.field_0x3012 = yobikusa_p->getPathID();
            shape_angle.y = yobikusa_p->shape_angle.y;
            current.angle.y = shape_angle.y;

            if (yobikusa_p->getType() == 0) {
                field_0x3198 = 0x100;
            } else if (yobikusa_p->getType() == 1) {
                field_0x3198 = 0xEE;
            }
        } else if (name == PROC_TAG_HOWL) {
            mZ2WolfHowlMgr.setCorrectCurve(static_cast<daTag_Howl_c*>(field_0x27f4)->getCurveID());
        } else if (name == PROC_Obj_WindStone) {
            mZ2WolfHowlMgr.setCorrectCurve(static_cast<daWindStone_c*>(field_0x27f4)->getTuneId());
            mProcVar0.field_0x3008 = static_cast<daWindStone_c*>(field_0x27f4)->getNextSceneId();
            mProcVar4.field_0x3010 = 1;
        } else if (name == PROC_Obj_SmWStone) {
            mZ2WolfHowlMgr.setCorrectCurve(static_cast<daSmWStone_c*>(field_0x27f4)->getTuneId());
            mProcVar4.field_0x3010 = 1;
        } else if (name == PROC_Tag_WaraHowl) {
            mZ2WolfHowlMgr.setCorrectCurve(static_cast<daTagWrHowl_c*>(field_0x27f4)->getTuneId());
        } else {
            mZ2WolfHowlMgr.setCorrectCurve(-1);
        }
    } else {
        mZ2WolfHowlMgr.setCorrectCurve(-1);
    }

    mNormalSpeed = 0.0f;

    if (!checkUnderMove0BckNoArcWolf(WANM_WAIT_SIT) && !checkUnderMove0BckNoArcWolf(WANM_LISTEN) &&
        !checkUnderMove0BckNoArcWolf(WANM_HOWL_SIT))
    {
        setSingleAnimeWolfParam(WANM_SIT, &mpHIO->mWolf.mWlLie.m.mSittingAnm);
        field_0x347c = -1.0f;
    } else {
        field_0x347c = 10.0f;

        if (mProcVar4.field_0x3010 == 0) {
            setSingleAnimeWolfBaseSpeed(WANM_WAIT_SIT, mpHIO->mWolf.mWlLie.m.mSittingIdleAnmSpeed,
                                        mpHIO->mWolf.mWlLie.m.mSittingIdleInterp);
        } else {
            setSingleAnimeWolfBase(WANM_LISTEN);
            setFaceBasicTexture(FTANM_UNK_A2);
        }
    }

    dCam_getBody()->StartEventCamera(12, fopAcM_GetID(this), 0);
    mProcVar1.field_0x300a = 0;
    mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
    field_0x32cc = 0x5DD;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = -1;
    mDoAud_taktModeMute();

    if (checkStageName("F_SP200")) {
        fopAc_ac_c* gwolf_p =
            (fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchGoldWolf, NULL);
        field_0x280c.setData(gwolf_p);
    } else {
        field_0x280c.clearData();
    }

    dComIfGp_setPlayerStatus1(0, 0x4000000);
    return 1;
}

int daAlink_c::procWolfHowlDemo() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    daNpc_GWolf_c* gwolf_p = (daNpc_GWolf_c*)field_0x280c.getActor();

    BOOL skip_edge = dComIfGp_getEvent()->checkSkipEdge();
    if (skip_edge) {
        mZ2WolfHowlMgr.skipCorrectDemo();
    }

    if (checkEndMessage(field_0x32cc) ||
        (skip_edge && (field_0x32cc == 0x535 || field_0x32cc == 0x532)))
    {
        if (mProcVar2.field_0x300c == 0) {
            resetSpecialEvent();
            field_0x30b2 = 0;
            field_0x30b0 = 0;
            mDoAud_taktModeMuteOff();
            return 1;
        }
    } else if (field_0x32cc == 0x535 || field_0x32cc == 0x532) {
        if (checkUnderMove0BckNoArcWolf(WANM_HOWL_END) && checkAnmEnd(frameCtrl_p)) {
            setSingleAnimeWolfBase(WANM_SERVICE_WAIT_D);
        }

        return 1;
    }

    if (field_0x347c < 0.0f) {
        if (checkAnmEnd(frameCtrl_p)) {
            if (mProcVar4.field_0x3010 != 0) {
                setSingleAnimeWolfBase(WANM_LISTEN);
                setFaceBasicTexture(FTANM_UNK_A2);
            } else {
                setSingleAnimeWolfBaseSpeed(WANM_WAIT_SIT, mpHIO->mWolf.mWlLie.m.mSittingIdleAnmSpeed,
                                            mpHIO->mWolf.mWlLie.m.mSittingIdleInterp);
            }

            field_0x347c = 1.0f;
        }
    } else if (field_0x347c > 1.5f) {
        field_0x347c -= 1.0f;
    } else {
        bool var_r26 = mDoCPd_c::getHoldA(PAD_1) != false;
        if (mProcVar4.field_0x3010 != 0 && mProcVar2.field_0x300c != 0) {
            var_r26 = false;
        }

        if (checkUnderMove0BckNoArcWolf(WANM_HOWL_SUCCESS)) {
            if (gwolf_p == NULL) {
                dComIfGp_getEvent()->startCheckSkipEdge(this);
            }
        } else if (checkUnderMove0BckNoArcWolf(WANM_HOWL_END)) {
            if (gwolf_p == NULL) {
                dComIfGp_getEvent()->startCheckSkipEdge(this);
            }
        } else if ((var_r26 || mZ2WolfHowlMgr.getTimer() != 0) &&
                   mZ2WolfHowlMgr.getReleaseTimer() != 30)
        {
            if (!checkUnderMove0BckNoArcWolf(WANM_HOWL_SIT) || mProcVar1.field_0x300a == 0) {
                mProcVar1.field_0x300a = 1;
                setSingleAnimeWolfParam(WANM_HOWL_SIT, &mpHIO->mWolf.mWlHowl.m.mSittingHowlAnm);
            } else {
                if (frameCtrl_p->getFrame() > 56.0f) {
                    frameCtrl_p->setFrame((frameCtrl_p->getFrame() - 56.0f) + 38.0f);
                }

                if (frameCtrl_p->getFrame() >= 38.0f && frameCtrl_p->getFrame() <= 56.0f) {
                    onResetFlg0(RFLG0_UNK_40000000);
                }
            }
        } else {
            if (mZ2WolfHowlMgr.getReleaseTimer() == 30) {
                frameCtrl_p->setFrame(56.0f);
                field_0x2060->initOldFrameMorf(3.0f, 0, 40);
            }

            mProcVar1.field_0x300a = 0;

            if (dMsgObject_isHowlHearingMode() &&
                (checkAnmEnd(frameCtrl_p) || checkUnderMove0BckNoArcWolf(WANM_WAIT_SIT)))
            {
                setSingleAnimeWolfBase(WANM_LISTEN);
                setFaceBasicTexture(FTANM_UNK_A2);
            } else if (checkAnmEnd(frameCtrl_p)) {
                setSingleAnimeWolfBaseSpeed(WANM_WAIT_SIT, mpHIO->mWolf.mWlLie.m.mSittingIdleAnmSpeed,
                                            mpHIO->mWolf.mWlLie.m.mSittingIdleInterp);
            }
        }

        s16 var_r25 = mProcVar3.field_0x300e;
        f32 stick_y = mDoCPd_c::getStickY(PAD_1);
        mProcVar3.field_0x300e = mZ2WolfHowlMgr.startWolfHowlSound(0.0f, stick_y, var_r26, 0.0f);
        s16 var_r26_2 = 0;

        if (mProcVar3.field_0x300e == -1 || skip_edge) {
            if (checkUnderMove0BckNoArcWolf(WANM_HOWL_END) || skip_edge) {
                if (checkAnmEnd(frameCtrl_p) || skip_edge) {
                    if (mProcVar0.mHowlExitID >= 0) {
                        dStage_changeScene(mProcVar0.mHowlExitID, 0.0f, 0, fopAcM_GetRoomNo(this),
                                           shape_angle.y, -1);
                    } else {
                        fopAc_ac_c* actor_p = NULL;
                        if (gwolf_p == NULL) {
                            fopAcIt_Executor((fopAcIt_ExecutorFunc)daAlink_searchWolfHowl,
                                             &actor_p);
                        } else {
                            actor_p = gwolf_p;
                        }

                        field_0x30b2 = 0;
                        field_0x30b0 = 0;

                        if (actor_p != NULL) {
                            fopAcM_OnStatus(actor_p, 0x800);

                            if (fopAcM_GetName(actor_p) == PROC_KAGO) {
                                static_cast<daKago_c*>(actor_p)->setEvent();
                                static_cast<daKago_c*>(actor_p)->setKagoPath(
                                    mProcVar5.field_0x3012);
                            } else if (fopAcM_GetName(actor_p) == PROC_OBJ_SEKIZOA) {
                                static_cast<daObj_Sekizoa_c*>(actor_p)->setWolfHowling();
                            } else if (fopAcM_GetName(actor_p) == PROC_E_PM) {
                                static_cast<daE_PM_c*>(actor_p)->AppearSet();
                            } else if (fopAcM_GetName(actor_p) == PROC_HORSE) {
                                int ret = static_cast<daHorse_c*>(actor_p)->callHorse(&current.pos);

                                if (ret == 1 || ret == 2) {
                                    mDoAud_taktModeMuteOff();
                                    return procCoHorseCallWaitInit(ret);
                                }

                                if (ret != 3) {
                                    setWolfHowlNotHappen(skip_edge);
                                    return 1;
                                }
                            } else if (fopAcM_GetName(actor_p) == PROC_NPC_TK) {
                                static_cast<daNPC_TK_c*>(actor_p)->setEventWolf();
                                static_cast<daNPC_TK_c*>(actor_p)->setHawkPath(
                                    mProcVar5.field_0x3012);
                            }
                        } else if (mProcVar4.field_0x3010 == 0) {
                            setWolfHowlNotHappen(skip_edge);
                            return 1;
                        }

                        if (gwolf_p == NULL) {
                            resetSpecialEvent();
                        } else {
                            endDemoMode();
                        }

                        mDoAud_taktModeMuteOff();
                    }

                    procWolfServiceWaitInit(2);
                    return 1;
                }
            } else if (var_r25 != -1) {
                setSingleAnimeWolfBase(WANM_HOWL_END);
                if (gwolf_p != NULL) {
                    gwolf_p->setHowlingEndFlag();
                }
            } else {
                var_r26_2 = (s16)(stick_y * -6144.0f);
            }
        } else if (mProcVar2.field_0x300c == 0) {
            mProcVar2.field_0x300c = 1;
            dMsgObject_onMsgSend();
            setSingleAnimeWolfBase(WANM_HOWL_SUCCESS);
            mUnderFrameCtrl[0].setLoop(27);
            mUnderFrameCtrl[0].setAttribute(2);
            setFaceBasicTexture(FTANM_UNK_A1);

            if (gwolf_p != NULL) {
                fopAcM_OnStatus(gwolf_p, 0x800);
                gwolf_p->setHowlingFlag();
                dComIfGp_event_reset();

                dCam_getBody()->EndEventCamera(fopAcM_GetID(this));
            } else {
                dComIfGp_getEvent()->startCheckSkipEdge(this);
            }
        }

        cLib_addCalcAngleS(&field_0x30b2, var_r26_2, 5, 0x800, 0x100);
    }

    return 1;
}

BOOL daAlink_c::checkWolfRope() {
    return checkModeFlg(0x20000) && field_0x280c.getActor() != NULL &&
           fopAcM_GetName(field_0x280c.getActor()) == PROC_Obj_Crope;
}

fopAc_ac_c* daAlink_c::checkWolfRopeHit(dCcD_GObjInf* i_collider, cXyz const* param_1,
                                        int param_2) const {
    daObjCrope_c* rope_p = (daObjCrope_c*)i_collider->GetCoHitAc();

    if (i_collider->ChkCoHit() && rope_p != NULL && fopAcM_GetName(rope_p) == PROC_Obj_Crope) {
        if (param_2) {
            if (rope_p->getRopeStartPos()->abs2XZ(*param_1) > 10000.0f) {
                if (rope_p->getRopeEndPos()->abs2XZ(*param_1) > 10000.0f) {
                    return rope_p;
                }
            }
        } else if (checkInputOnR()) {
            mDoMtx_stack_c::YrotS(-rope_p->shape_angle.y);
            mDoMtx_stack_c::transM(-rope_p->getRopeStartPos()->x, -rope_p->getRopeStartPos()->y,
                                   -rope_p->getRopeStartPos()->z);

            cXyz sp44;
            cXyz sp38;
            mDoMtx_stack_c::multVec(param_1, &sp44);
            mDoMtx_stack_c::multVec(rope_p->getRopeEndPos(), &sp38);

            if (fabsf(sp44.x) < 40.0f && (sp44.z > -10.0f || sp44.z - sp38.z < 10.0f)) {
                return rope_p;
            }
        }
    }

    return NULL;
}

int daAlink_c::checkWolfRopeJumpHang() {
    fopAc_ac_c* hit_actor = checkWolfRopeHit(&field_0x850[0], &current.pos, TRUE);
    if (hit_actor == NULL) {
        hit_actor = checkWolfRopeHit(&field_0xFB8, &current.pos, TRUE);
    }

    if (hit_actor != NULL) {
        field_0x280c.setData(hit_actor);
        return procWolfRopeHangInit(4);
    }

    return 0;
}

f32 daAlink_c::getWolfRopeMoveSpeed() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    f32 frame = frameCtrl->getFrame();

    if (frame >= 13.5f) {
        frame -= 13.5f;
    }

    return mpHIO->mWolf.mWlRope.m.mMovementRate * frameCtrl->getRate() * cM_fsin(0.23271057f * frame);
}

int daAlink_c::setWolfRopePosY() {
    daObjCrope_c* temp_r3 = (daObjCrope_c*)field_0x280c.getActor();
    cXyz* temp_r28 = temp_r3->getRopeStartPos();
    cXyz* temp_r27 = temp_r3->getRopeEndPos();
    cXyz* temp_r26 = temp_r3->getRopeVec();
    f32 temp_f30 = temp_r3->getStartRate(&current.pos);

    f32 var_f31;
    if (mProcID == PROC_WOLF_ROPE_MOVE &&
        (((var_f31 = temp_r3->getStartRate(&mLeftHandPos)) < 0.0f || var_f31 > 1.0f) ||
         ((var_f31 = temp_r3->getStartRate(&mRightHandPos)) < 0.0f || var_f31 > 1.0f) ||
         ((var_f31 = temp_r3->getStartRate(&mLeftFootPos)) < 0.0f || var_f31 > 1.0f) ||
         ((var_f31 = temp_r3->getStartRate(&mRightFootPos)) < 0.0f || var_f31 > 1.0f)))
    {
        if (temp_r28->abs2XZ(current.pos) > temp_r27->abs2XZ(current.pos)) {
            current.pos.y = temp_r27->y;
        } else {
            current.pos.y = temp_r28->y;
        }

        temp_r3->onOutFlg();
        return 1;
    }

    if (temp_f30 < 0.0f) {
        current.pos.y = temp_r28->y;
    } else if (temp_f30 > 1.0f) {
        current.pos.y = temp_r27->y;
    } else {
        current.pos.y = ((temp_r28->y + (temp_f30 * temp_r26->y)) - 40.0f) -
                        (10.0f * cM_fsin(M_PI * temp_f30));
    }

    return 0;
}

s16 daAlink_c::initWolfRopeShapeAngle() {
    fopAc_ac_c* actor = field_0x280c.getActor();
    int a = abs((s16)(actor->shape_angle.y - shape_angle.y));

    s16 angle;
    if (a < 0x4000) {
        shape_angle.y = actor->shape_angle.y;
        angle = actor->shape_angle.x;
    } else {
        shape_angle.y = actor->shape_angle.y - -0x8000;
        angle = -actor->shape_angle.x;
    }

    return angle;
}

void daAlink_c::wolfRopeSwingInc(f32 param_0) {
    field_0x3478 += param_0;

    u32 var_r6 = (u32)((field_0x3478 * 127.0f) / 100.0f);
    if (var_r6 > 127) {
        var_r6 = 127;
    }

    mDoAud_seStart(Z2SE_FN_ROPE_CREAK_ADD, &current.pos, var_r6, mVoiceReverbIntensity);
}

void daAlink_c::setWolfRopeOffsetY(int param_0) {
    if (param_0) {
        field_0x3478 = 0.0f;
        field_0x2b98 = 0.0f;
        mProcVar0.field_0x3008 = 0;
    } else {
        if (field_0x3478 > 100.0f) {
            field_0x3478 = 100.0f;
        }

        cLib_addCalc(&field_0x3478, 2.0f, 0.05f, 1.0f, 0.05f);
        field_0x2b98 = field_0x3478 * (cM_scos(mProcVar0.field_0x3008) - 1.0f) * 0.5f;

        f32 var_f3 = field_0x3478 - 2.0f;
        if (var_f3 > 2.0f) {
            var_f3 = 2.0f;
        }

        mProcVar0.field_0x3008 += var_f3 * (f32)0x600 + (f32)0xC00;
    }
}

int daAlink_c::getDirectionRopeMove() const {
    s16 angle = field_0x2fe2 - shape_angle.y;

    if (abs(angle) > 0x5000) {
        return DIR_BACKWARD;
    }

    if (angle >= 0x3000) {
        return DIR_LEFT;
    }

    if (angle <= -0x3000) {
        return DIR_RIGHT;
    }

    return DIR_FORWARD;
}

int daAlink_c::procWolfRopeMoveInit(int param_0, int param_1) {
    daObjCrope_c* rope_p = (daObjCrope_c*)field_0x280c.getActor();
    commonProcInit(PROC_WOLF_ROPE_MOVE);
    mProcVar2.field_0x300c = initWolfRopeShapeAngle();
    current.angle.y = shape_angle.y;

    setSingleAnimeWolfBaseSpeed(WANM_ROPE_WALK, 0.0f, mpHIO->mWolf.mWlRope.m.mMovementInterpolation);
    if (param_0) {
        mUnderFrameCtrl[0].setFrame(13.5f);
        getNowAnmPackUnder(UNDER_0)->setFrame(13.5f);
    }

    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;

    if (param_1) {
        current.pos = rope_p->getPlayerNearPos();
        field_0x3478 = 0.0f;
        mProcVar0.field_0x3008 = 0;
    }

    setWolfRopeOffsetY(setWolfRopePosY());
    if (checkWolfGrabAnime()) {
        field_0x30a0 = -0x1800;
    }

    mProcVar3.field_0x300e = 0;
    initServiceWaitTime();
    return 1;
}

int daAlink_c::procWolfRopeMove() {
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
    daObjCrope_c* temp_r29 = (daObjCrope_c*)field_0x280c.getActor();
    int var_r27 = 0;

    f32 temp_f26 = getAnmSpeedStickRate(mpHIO->mWolf.mWlRope.m.mMovementMinAnmSpeed, mpHIO->mWolf.mWlRope.m.mMovementMaxAnmSpeed);
    f32 temp_f25 = -1.0f * getAnmSpeedStickRate(mpHIO->mWolf.mWlRope.m.mMovementMinAnmSpeed, mpHIO->mWolf.mWlRope.m.mBackMovementMaxAnmSpeed);

    if (temp_r28->getRate() > 0.0f) {
        temp_r28->setRate(temp_f26);
    } else if (temp_r28->getRate() < 0.0f) {
        temp_r28->setRate(temp_f25);
    } else {
        var_r27 = 1;
    }

    cXyz* sp10 = temp_r29->getRopeStartPos();
    cXyz* spC = temp_r29->getRopeEndPos();
    cXyz* sp8 = temp_r29->getRopeVec();
    f32 temp_f31 = temp_r29->getStartRate(&current.pos);

    if (setTalkStatus()) {
        if (orderTalk(1)) {
            return 1;
        }
    } else if (mTargetedActor != NULL) {
        setDoStatus(0x8B);

        if (doTrigger()) {
            return procWolfJumpAttackInit(1);
        }
    }

    int temp_r26 = getDirectionRopeMove();
    f32 temp_f29 = sp10->absXZ(current.pos);
    f32 temp_f28 = spC->absXZ(current.pos);

    if (temp_f31 > 0.0f && temp_f31 < 1.0f && temp_f29 > 100.0f && temp_f28 > 100.0f) {
        if (field_0x814.GetCCMoveP()->abs2XZ() > 100.0f) {
            field_0x814.ClrCcMove();

            if (field_0x814.GetCCMoveP()->atan2sX_Z() - shape_angle.y > 0) {
                return procWolfRopeHangInit(2);
            }
            return procWolfRopeHangInit(3);
        }

        if (checkWindSpeedOnXZ()) {
            if (field_0x35b8.atan2sX_Z() - shape_angle.y > 0) {
                return procWolfRopeHangInit(2);
            }
            return procWolfRopeHangInit(3);
        }

        if (field_0x33ac > 0.5f) {
            if (temp_r26 == 2) {
                return procWolfRopeStaggerInit(2);
            } else if (temp_r26 == 3) {
                return procWolfRopeStaggerInit(3);
            }
        }
    }

    field_0x814.ClrCcMove();

    if (temp_f31 > 0.0f && temp_f31 < 1.0f && temp_f29 > 85.0f && temp_f28 > 85.0f) {
        if (!checkInputOnR() && var_r27 != 0 && checkServiceWaitMode()) {
            if (field_0x30ca != 0) {
                field_0x30ca--;
            } else {
                return procWolfRopeStaggerInit(4);
            }
        } else {
            initServiceWaitTime();
        }
    } else {
        initServiceWaitTime();
    }

    if (var_r27 == 1 || temp_r28->checkPass(0.0f) || temp_r28->checkPass(15.0f)) {
        if (checkInputOnR() && temp_r26 != 3 && temp_r26 != 2) {
            if (var_r27 != 0 && field_0x33ac > 0.5f && cLib_distanceAngleS(field_0x2fe2, shape_angle.y) > 0x7000 && ((shape_angle.y == temp_r29->shape_angle.y && temp_f29 > 70.0f) || (shape_angle.y != temp_r29->shape_angle.y && temp_f28 > 70.0f))) {
                return procWolfRopeTurnInit();
            }

            if (temp_r26 != 1) {
                temp_r28->setRate(temp_f26);
                temp_r28->setLoop(0);
            } else {
                temp_r28->setRate(temp_f25);
                temp_r28->setLoop(temp_r28->getEnd());
            }

            initBasAnime();
            wolfRopeSwingInc(10.0f);
            mProcVar0.field_0x3008 = 0;
        } else if (var_r27 == 0) {
            stopHalfMoveAnime(13.5f);
            wolfRopeSwingInc(10.0f);
            mProcVar0.field_0x3008 = 0;
        } else if (checkSubjectAction()) {
            return 1;
        }
    }

    mNormalSpeed = getWolfRopeMoveSpeed();

    if (mNormalSpeed < 0.0f) {
        mNormalSpeed *= -1.0f;
        current.angle.y = shape_angle.y + 0x8000;

        if (checkInputOnR() && !checkAttentionLock()) {
            mProcVar3.field_0x300e++;
            if (mProcVar3.field_0x300e == 60) {
                return procWolfRopeTurnInit();
            }
        } else {
            mProcVar3.field_0x300e = 0;
        }
    } else {
        current.angle.y = shape_angle.y;
        mProcVar3.field_0x300e = 0;
    }

    f32 var_f24 = 45.0f;
    if ((temp_f31 < 0.0f && temp_f29 > var_f24) || (temp_f31 > 1.0f && temp_f28 > var_f24)) {
        temp_r29->offRide();

        f32 var_f27;
        if (current.angle.y != shape_angle.y) {
            var_f27 = 80.0f;
        } else {
            var_f27 = 40.0f;
        }

        current.pos.x += var_f27 * cM_ssin(current.angle.y);
        current.pos.z += var_f27 * cM_scos(current.angle.y);
        current.angle.y = shape_angle.y;
        return checkNextActionWolf(0);
    }

    setWolfRopeOffsetY(setWolfRopePosY());

    f32 temp_f30 = temp_r28->getFrame();
    if (temp_f30 < 1.0f || (temp_f30 > 12.5f && temp_f30 < 14.5f) || temp_f30 > (temp_r28->getEnd() - 1.0f)) {
        if (mLeftHandPos.abs2XZ(current.pos) > mRightHandPos.abs2XZ(current.pos)) {
            temp_r29->setFrontJoint(0x13);
            temp_r29->setBackJoint(0x1F);
        } else {
            temp_r29->setFrontJoint(0x18);
            temp_r29->setBackJoint(0x24);
        }
    } else if (temp_f30 < 13.5f) {
        temp_r29->setFrontJoint(0x13);
        temp_r29->setBackJoint(0x24);
    } else {
        temp_r29->setFrontJoint(0x18);
        temp_r29->setBackJoint(0x1F);
    }

    return 1;
}

int daAlink_c::procWolfRopeHangInit(int param_0) {
    daObjCrope_c* rope_p = (daObjCrope_c*)field_0x280c.getActor();
    commonProcInit(PROC_WOLF_ROPE_HANG);

    if (param_0 != 4) {
        mProcVar3.field_0x300e = 0;
        setSingleAnimeWolf(WANM_HANG, mpHIO->mWolf.mWlRope.m.mTwistClimbAnmSpeed, 0.0f, 16,
                           mpHIO->mWolf.mWlRope.m.mFallInterpolation);
        rope_p->rideKeep();
    } else {
        mProcVar3.field_0x300e = 1;

        daAlink_WANM climb_anm;
        s16 temp_r0 = shape_angle.y - rope_p->shape_angle.y;
        if ((temp_r0 >= 0 && temp_r0 < 0x4000) || temp_r0 <= -0x4000) {
            param_0 = 3;
            climb_anm = WANM_ROPE_CLIMB_RIGHT;
        } else {
            param_0 = 2;
            climb_anm = WANM_ROPE_CLIMB_LEFT;
        }

        setSingleAnimeWolfBaseSpeed(climb_anm, mpHIO->mWolf.mWlRope.m.mTwistClimbAnmSpeed,
                                    mpHIO->mWolf.mWlRope.m.mTwistClimbInterpolation);
        rope_p->setFrontJoint(19);
        rope_p->setBackJoint(23);
        current.pos = rope_p->getPlayerNearPos();

        field_0x2f99 = 0;
        dComIfGp_getVibration().StartShock(2, 15, cXyz(0.0f, 1.0f, 0.0f));
    }

    field_0x3198 = param_0;
    mProcVar4.field_0x3010 = initWolfRopeShapeAngle();
    mProcVar2.field_0x300c = 0;

    if (param_0 == 2) {
        shape_angle.y -= 0x4000;
        mProcVar4.field_0x3010 = -mProcVar4.field_0x3010;
    } else {
        shape_angle.y += 0x4000;
    }

    wolfRopeSwingInc(50.0f);
    mProcVar0.field_0x3008 = 0;
    setWolfRopePosY();
    setWolfRopeOffsetY(0);
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;
    mNormalSpeed = 0.0f;
    shape_angle.x = 0;
    return 1;
}

int daAlink_c::procWolfRopeHang() {
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
    daObjCrope_c* temp_r3 = (daObjCrope_c*)field_0x280c.getActor();

    if (temp_r3 == NULL) {
        return checkNextActionWolf(0);
    }

    if (checkAnmEnd(temp_r28)) {
        if (mProcVar3.field_0x300e != 0) {
            field_0x2060->getOldFrameTransInfo(0)->mTranslate.z -= 15.0f;

            if (field_0x3198 == 2) {
                shape_angle.y += 0x4000;
                setOldRootQuaternion(0, -0x4000, 0);
                shape_angle.x = -shape_angle.z;
                return procWolfRopeMoveInit(0, 0);
            } else {
                shape_angle.y -= 0x4000;
                setOldRootQuaternion(0, 0x4000, 0);
                shape_angle.x = shape_angle.z;
                return procWolfRopeMoveInit(1, 0);
            }
        }

        mProcVar3.field_0x300e = 1;

        daAlink_WANM var_r27;
        if (field_0x3198 == 2) {
            var_r27 = WANM_ROPE_CLIMB_LEFT;
        } else {
            var_r27 = WANM_ROPE_CLIMB_RIGHT;
        }

        setSingleAnimeWolfBaseSpeed(var_r27, mpHIO->mWolf.mWlRope.m.mTwistClimbAnmSpeed, mpHIO->mWolf.mWlRope.m.mTwistClimbInterpolation);
    }

    f32 temp_f31 = temp_r28->getFrame();
    if (mProcVar3.field_0x300e == 0 || temp_f31 < 22.0f) {
        setDoStatus(0x33);

        if (doTrigger()) {
            temp_r3->setCoCancelTimer();

            current.pos.x -= 80.0f * cM_ssin(shape_angle.y);
            current.pos.z -= 80.0f * cM_scos(shape_angle.y);

            field_0x2060->getOldFrameTransInfo(0)->mTranslate.z += 80.0f;
            return procWolfFallInit(2, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
        }
    }

    if (mProcVar3.field_0x300e != 0) {
        if (field_0x3198 == 2) {
            if (temp_r28->checkPass(22.0f)) {
                wolfRopeSwingInc(15.0f);
                mProcVar0.field_0x3008 = 0;
            }

            if (temp_f31 < 1.0f) {
                temp_r3->setFrontJoint(0x13);
                temp_r3->setBackJoint(0x17);
            } else if (temp_f31 < 5.0f) {
                temp_r3->setFrontJoint(-1);
            } else if (temp_f31 < 22.0f) {
                temp_r3->setFrontJoint(0x12);
            } else if (temp_f31 < 41.0f) {
                temp_r3->setBackJoint(0x24);
            } else {
                temp_r3->setFrontJoint(0x13);
                temp_r3->setBackJoint(0x1F);

                if (checkWolfGrabAnime()) {
                    field_0x30a0 = -0x1800;
                }
            }
        } else {
            if (temp_r28->checkPass(27.0f)) {
                wolfRopeSwingInc(15.0f);
                mProcVar0.field_0x3008 = 0;
            }

            if (temp_f31 < 1.0f) {
                temp_r3->setFrontJoint(0x13);
                temp_r3->setBackJoint(0x17);
            } else if (temp_f31 < 12.0f) {
                temp_r3->setFrontJoint(-1);
            } else if (temp_f31 < 27.0f) {
                temp_r3->setFrontJoint(0x12);
            } else if (temp_f31 < 46.0f) {
                temp_r3->setFrontJoint(0x1F);
            } else {
                temp_r3->setBackJoint(0x18);
                temp_r3->setFrontJoint(0x24);

                if (checkWolfGrabAnime()) {
                    field_0x30a0 = -0x1800;
                }
            }
        }
    } else {
        if (temp_r28->checkPass(4.0f)) {
            dComIfGp_getVibration().StartShock(2, 0xF, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (temp_f31 < 7.0f) {
            temp_r3->rideKeep();
        } else if (temp_f31 < 9.0f) {
            temp_r3->setFrontJoint(0x13);
        } else {
            temp_r3->setBackJoint(0x17);
        }
    }

    setWolfRopeOffsetY(0);
    return 1;
}

int daAlink_c::procWolfRopeTurnInit() {
    commonProcInit(PROC_WOLF_ROPE_TURN);
    setSingleAnimeWolfParam(WANM_ROPE_TURN, &mpHIO->mWolf.mWlRope.m.mFlipAnm);
    voiceStart(Z2SE_WL_V_BREATH_JUMP);
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;
    mNormalSpeed = 0.0f;

    mProcVar3.field_0x300e = initWolfRopeShapeAngle();
    current.angle.y = shape_angle.y + 0x8000;
    field_0x3588 = l_wolfBaseAnime;
    shape_angle.x = 0;
    field_0x37c8 = current.pos;
    mProcVar2.field_0x300c = 0;
    field_0x347c = 1.0f / mUnderFrameCtrl[0].getEnd();
    static_cast<daObjCrope_c*>(field_0x280c.getActor())->rideKeep();
    return 1;
}

int daAlink_c::procWolfRopeTurn() {
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
    daObjCrope_c* temp_r29 = (daObjCrope_c*)field_0x280c.getActor();
    field_0x2f99 = 4;

    f32 temp_f30 = field_0x347c * temp_r28->getFrame();
    field_0x3588.x = l_wolfBaseAnime.x - (temp_f30 * (l_wolfBaseAnime.x + l_wolfRopeBaseAnime.x));
    field_0x3588.z = l_wolfBaseAnime.z - (temp_f30 * (l_wolfBaseAnime.z + l_wolfRopeBaseAnime.z));

    setWolfRopePosY();

    if (checkAnmEnd(temp_r28)) {
        return procWolfRopeMoveInit(0, 0);
    }

    f32 temp_f31 = temp_r28->getFrame();
    if (temp_r28->checkPass(9.0f)) {
        wolfRopeSwingInc(30.0f);
        mProcVar0.field_0x3008 = 0;
        dComIfGp_getVibration().StartShock(1, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    } else if (temp_r28->checkPass(13.0f)) {
        wolfRopeSwingInc(10.0f);
        mProcVar0.field_0x3008 = 0;
    }

    if (!(temp_f31 < 7.0f)) {
        if (temp_f31 < 9.0f) {
            mProcVar2.field_0x300c = mProcVar3.field_0x300e;
            temp_r29->setFrontJoint(0x13);
        } else if (temp_f31 < 11.0f) {
            temp_r29->setFrontJoint(0x18);
        } else if (temp_f31 < 13.0f) {
            temp_r29->setFrontJoint(0x18);
            temp_r29->setBackJoint(0x1F);
        } else {
            temp_r29->setFrontJoint(0x13);
            temp_r29->setBackJoint(0x1F);
        }
    }

    setWolfRopeOffsetY(0);
    return 1;
}

int daAlink_c::procWolfRopeStaggerInit(int param_0) {
    commonProcInit(PROC_WOLF_ROPE_STAGGER);

    if (param_0 == 4) {
        setSingleAnimeWolfBase(WANM_ROPE_WAIT);
        mNormalSpeed = 0.0f;
        mProcVar3.field_0x300e = 1;
    } else {
        setSingleAnimeWolfBase(WANM_ROPE_FALTER);
        field_0x3588 = l_wolfRopeBaseAnime;
        mProcVar3.field_0x300e = 0;
    }

    mProcVar4.field_0x3010 = param_0;
    mProcVar2.field_0x300c = initWolfRopeShapeAngle();
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;
    current.angle.y = shape_angle.y;

    if (checkWolfGrabAnime()) {
        field_0x30a0 = -0x1800;
    }

    daObjCrope_c* rope_p = (daObjCrope_c*)field_0x280c.getActor();
    rope_p->setFrontJoint(19);
    rope_p->setBackJoint(31);
    return 1;
}

int daAlink_c::procWolfRopeStagger() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
    daObjCrope_c* temp_r3 = (daObjCrope_c*)field_0x280c.getActor();

    f32 temp_f31, temp_f30, temp_f29;
    temp_f30 = temp_r3->getRopeStartPos()->absXZ(current.pos);
    temp_f29 = temp_r3->getRopeEndPos()->absXZ(current.pos);
    temp_f31 = temp_r3->getStartRate(&current.pos);

    if (field_0x814.GetCCMoveP()->abs2XZ() > 100.0f) {
        field_0x814.ClrCcMove();

        if ((field_0x814.GetCCMoveP()->atan2sX_Z() - shape_angle.y) > 0) {
            return procWolfRopeHangInit(2);
        } else {
            return procWolfRopeHangInit(3);
        }
    }

    if (!(temp_f31 > 0.0f) || !(temp_f31 < 1.0f) || !(temp_f30 > 85.0f) || !(temp_f29 > 85.0f)) {
        return procWolfRopeMoveInit(0, 0);
    }

    if (checkAnmEnd(temp_r29)) {
        if (mProcVar3.field_0x300e != 0) {
            return procWolfRopeMoveInit(0, 0);
        } else {
            return procWolfRopeHangInit(mProcVar4.field_0x3010);
        }
    }

    field_0x814.ClrCcMove();

    int temp_r26 = getDirectionRopeMove();
    if ((mProcVar3.field_0x300e != 0 && checkInputOnR()) || (mProcVar3.field_0x300e == 0 && (!checkInputOnR() || temp_r26 == 0 || temp_r26 == 1))) {
        return procWolfRopeMoveInit(0, 0);
    }

    if (mProcVar3.field_0x300e != 0) {
        if (checkSubjectAction()) {
            return 1;
        }

        if (temp_r29->checkPass(30.0f) || temp_r29->checkPass(61.0f) || temp_r29->checkPass(86.0f)) {
            wolfRopeSwingInc(10.0f);
        } else if (temp_r29->checkPass(65.0f)) {
            voiceStart(Z2SE_WL_V_FOOTMISS);
        }
    } else {
        if (temp_r29->checkPass(16.0f) || temp_r29->checkPass(33.0f) || temp_r29->checkPass(50.0f) || temp_r29->checkPass(71.0f)) {
            wolfRopeSwingInc(10.0f);
        } else if (temp_r29->checkPass(60.0f)) {
            voiceStart(Z2SE_WL_V_BREATH_JUMP);
        }

        field_0x2f99 = 4;

        daObjCrope_c* temp_r28 = (daObjCrope_c*)field_0x280c.getActor();

        if (temp_r29->getFrame() > 46.0f) {
            mNormalSpeed = 0.0f;
        } else {
            mNormalSpeed = (7.0f + (2.0f * (2 - ((int)temp_r29->getFrame() / 15)))) * fabsf(cM_fsin((M_PI * temp_r29->getFrame()) / 15.0f));
        }

        if ((temp_r29->getFrame() >= 5.0f && temp_r29->getFrame() < 19.0f) || (temp_r29->getFrame() >= 39.0f && temp_r29->getFrame() < 51.0f)) {
            temp_r28->setBackJoint(0x24);
        } else {
            temp_r28->setBackJoint(0x1F);
        }

        if ((temp_r29->getFrame() >= 13.0f && temp_r29->getFrame() < 30.0f) || (temp_r29->getFrame() >= 48.0f && temp_r29->getFrame() < 63.0f)) {
            temp_r28->setFrontJoint(0x18);
        } else {
            temp_r28->setFrontJoint(0x13);
        }
    }

    setWolfRopeOffsetY(0);
    return 1;
}

int daAlink_c::procWolfRopeSubjectivityInit() {
    commonProcInit(PROC_WOLF_ROPE_SUBJECTIVITY);

    if (!checkUnderMove0BckNoArcWolf(WANM_ROPE_WALK)) {
        setSingleAnimeWolfBaseSpeed(WANM_ROPE_WALK, 0.0f, mpHIO->mWolf.mWlRope.m.mMovementInterpolation);
    }

    mProcVar2.field_0x300c = initWolfRopeShapeAngle();
    current.angle.y = shape_angle.y;

    setSpecialGravity(0.0f, maxFallSpeed, 0);

    f32 tmp_0 = 0.0f;
    speed.y = tmp_0;
    mNormalSpeed = tmp_0;

    setWolfRopeOffsetY(setWolfRopePosY());

    if (checkWolfGrabAnime()) {
        field_0x30a0 = -0x1800;
    }

    setSubjectMode();
    return 1;
}

int daAlink_c::procWolfRopeSubjectivity() {
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
    daObjCrope_c* temp_r3 = (daObjCrope_c*)field_0x280c.getActor();

    cXyz* temp_r26 = temp_r3->getRopeStartPos();
    cXyz* temp_r25 = temp_r3->getRopeEndPos();
    cXyz* r23 = temp_r3->getRopeVec();

    f32 temp_f31 = temp_r3->getStartRate(&current.pos);
    f32 temp_f30 = temp_r26->absXZ(current.pos);
    f32 temp_f29 = temp_r25->absXZ(current.pos);

    onResetFlg0(RFLG0_UNK_4000000);

    if (temp_f31 > 0.0f && temp_f31 < 1.0f && temp_f30 > 100.0f && temp_f29 > 100.0f && field_0x814.GetCCMoveP()->abs2XZ() > 100.0f) {
        field_0x814.ClrCcMove();

        if ((field_0x814.GetCCMoveP()->atan2sX_Z() - shape_angle.y) > 0) {
            return procWolfRopeHangInit(2);
        } else {
            return procWolfRopeHangInit(3);
        }
    }

    field_0x814.ClrCcMove();
    setWolfRopeOffsetY(setWolfRopePosY());

    if (checkSubjectEnd(1)) {
        int var_r27;
        if (temp_r28->getFrame() >= 12.5f && temp_r28->getFrame() <= 14.5f) {
            var_r27 = 1;
        } else {
            var_r27 = 0;
        }

        return procWolfRopeMoveInit(var_r27, 0);
    }

    shape_angle.y += mBodyAngle.y;
    setBodyAngleToCamera();
    s16 r24 = cLib_minMaxLimit<s16>((s16)(shape_angle.y - current.angle.y), -0x5000, 0x5000);
    mBodyAngle.y = r24;
    shape_angle.y = current.angle.y;
    return 1;
}

int daAlink_c::getWolfTagJumpTime() const {
    int time;
    if (mProcID == PROC_WOLF_TAG_JUMP) {
        time = mProcVar0.field_0x3008 + (int)((mpHIO->mWolf.mWlPoint.m.mLandingAnm.mCancelFrame -
                                               mpHIO->mWolf.mWlPoint.m.mLandingAnm.mStartFrame) /
                                              mpHIO->mWolf.mWlPoint.m.mLandingAnm.mSpeed);
    } else {
        time = (int)(checkMidnaLockJumpPoint()->abs(current.pos) /
                     mpHIO->mWolf.mWlPoint.m.mTagJumpSpeed);
    }

    time -= 2;
    if (time < 1) {
        time = 1;
    }

    return time;
}

const cXyz* daAlink_c::checkMidnaLockJumpPoint() const {
    if ((!mLinkAcch.ChkGroundHit() && mProcID != PROC_WOLF_TAG_JUMP) || checkMidnaUseAbility() ||
        getMidnaActor()->eventInfo.checkCommandTalk())
    {
        return NULL;
    }

    daPy_py_c* player_p = daPy_getLinkPlayerActorClass();
    fopAc_ac_c* partner_p = fopAcM_getTalkEventPartner(player_p);
    fopAc_ac_c* wljump_p = NULL;

    if (checkWolfTagLockJumpReady()) {
        return &field_0x3738;
    }

    if (mProcID == PROC_WOLF_TAG_JUMP && field_0x280c.getActorConst() != NULL) {
        wljump_p = field_0x280c.getActorConst();
    } else if (field_0x27f4 != NULL && fopAcM_GetName(field_0x27f4) == PROC_Tag_Wljump) {
        wljump_p = field_0x27f4;
    } else if (partner_p != NULL && fopAcM_GetName(partner_p) == PROC_Tag_Wljump) {
        wljump_p = partner_p;
    }

    if (wljump_p != NULL) {
        return static_cast<daTagWljump_c*>(wljump_p)->getLockPos();
    }

    return NULL;
}

int daAlink_c::procWolfTagJumpInit(fopAc_ac_c* param_0) {
    commonProcInit(PROC_WOLF_TAG_JUMP);

    field_0x3480 = mpHIO->mWolf.mWlPoint.m.mTagJumpSpeed;
    field_0x3198 = 0;

    if (param_0 == NULL) {
        field_0x37c8.set(current.pos.x + (300.0f * cM_ssin(shape_angle.y)), current.pos.y + 200.0f, current.pos.z + (300.0f * cM_scos(shape_angle.y)));
        field_0x3480 = 30.0f;
        field_0x280c.clearData();
        mProcVar1.field_0x300a = 0;
        field_0x3198 = 1;
    } else {
        field_0x37c8 = *((daTagWljump_c*)param_0)->getLockPos();

        cXyz sp14 = field_0x37c8 - current.pos;
        f32 temp_f31 = ((daTagWljump_c*)param_0)->getLandArea() / sp14.absXZ();

        field_0x37c8.x -= sp14.x * temp_f31;
        field_0x37c8.z -= sp14.z * temp_f31;

        field_0x280c.setData(param_0);
        ((daTagWljump_c*)param_0)->onNextCheckFlg();
        mProcVar1.field_0x300a = ((daTagWljump_c*)param_0)->getNotSlideFlg();
    }

    setSingleAnimeWolf(WANM_JUMP_ATTACK_START, mpHIO->mWolf.mWlAutoJump.m.mWeakJumpAnmSpeed, mpHIO->mWolf.mWlAutoJump.m.mJumpAnm.mStartFrame, mpHIO->mWolf.mWlAutoJump.m.mJumpAnm.mEndFrame, mpHIO->mWolf.mWlAutoJump.m.mJumpAnm.mInterpolation);
    field_0x3588 = l_wolfBaseAnime;

    cXyz sp8 = field_0x37c8 - current.pos;
    f32 temp_f30 = 1.0f / sp8.abs();

    shape_angle.y = sp8.atan2sX_Z();
    current.angle.y = shape_angle.y;
    field_0x2fe4 = shape_angle.y;
    mNormalSpeed = temp_f30 * (field_0x3480 * sp8.absXZ());

    mProcVar0.field_0x3008 = (sp8.absXZ() / mNormalSpeed) - 1.0f;
    if (mProcVar0.field_0x3008 < 1) {
        mProcVar0.field_0x3008 = 1;
    }

    if (fabsf(sp8.y) < 50.0f) {
        setSpecialGravity(-(120.0f + (2.0f * (60.0f - sp8.y)) + (4.0f * JMAFastSqrt(60.0f * (60.0f - sp8.y)))) / (mProcVar0.field_0x3008 * mProcVar0.field_0x3008), maxFallSpeed, 0);
        speed.y = JMAFastSqrt(-120.0f * gravity);
        mProcVar3.field_0x300e = 0;
    } else if (sp8.y > 0.0f) {
        setSpecialGravity((-2.0f * sp8.y) / (mProcVar0.field_0x3008 * mProcVar0.field_0x3008), maxFallSpeed, 0);
        speed.y = -gravity * mProcVar0.field_0x3008;
        mProcVar3.field_0x300e = 1;
    } else {
        speed.y = 0.0f;
        setSpecialGravity((2.0f * sp8.y) / (mProcVar0.field_0x3008 * mProcVar0.field_0x3008), maxFallSpeed, 0);
        mProcVar3.field_0x300e = 1;
    }

    mProcVar2.field_0x300c = cM_atan2s(-speed.y, mNormalSpeed) >> 1;
    field_0x3478 = speed.y;
    mProcVar4.field_0x3010 = 0;
    offNoResetFlg0(FLG0_UNK_40000);
    voiceStart(Z2SE_WL_V_LOCK_JUMP);
    mProcVar5.field_0x3012 = 0;
    return 1;
}

int daAlink_c::procWolfTagJump() {
    daTagWljump_c* temp_r29 = (daTagWljump_c*)field_0x280c.getActor();
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];

    mProcVar0.field_0x3008--;

    if (mProcVar0.field_0x3008 < 0) {
        if (temp_r29 == NULL || temp_r29->getLockPos() == NULL || mProcVar0.field_0x3008 < -mpHIO->mWolf.mWlPoint.m.mLockJumpStopTime) {
            procWolfWaitInit();
        } else {
            if (field_0x3198 == 0) {
                onResetFlg0(RFLG0_UNK_20000);
            }

            field_0x3738 = *temp_r29->getLockPos();
            setDoStatus(0x93);

            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x3738), 2, 0x2000, 0x800);
            current.angle.y = shape_angle.y;

            if (doTrigger()) {
                mProcVar5.field_0x3012 = 1;
            }

            if (mProcVar5.field_0x3012 && (checkModeFlg(0x100) || checkAnmEnd(temp_r28) || temp_r28->getFrame() > mpHIO->mWolf.mWlPoint.m.mLandingAnm.mCancelFrame)) {
                procWolfTagJumpInit(temp_r29);
            } else if (checkAnmEnd(temp_r28)) {
                onModeFlg(0x100);
                setSingleAnimeWolfBaseSpeed(WANM_WAIT_B, mpHIO->mWolf.mWlPoint.m.mIdleAnmSpeed, mpHIO->mWolf.mWlPoint.m.mIdleInterpolation);
            }
        }

        return 1;
    }

    field_0x2f99 = 4;

    if (mProcVar3.field_0x300e != 0) {
        speed.y = field_0x3478 + gravity;
        field_0x3478 = speed.y;
    }

    mProcVar2.field_0x300c = cM_atan2s(-speed.y, mNormalSpeed) >> 1;

    if (checkUnderMove0BckNoArcWolf(WANM_JUMP_ATTACK_START) && checkAnmEnd(temp_r28)) {
        field_0x2f99 = 0xC;
        setSingleAnimeWolfBaseSpeed(WANM_JUMP_ATTACK, 0.0f, mpHIO->mWolf.mWlAutoJump.m.mClimbAnm.mInterpolation);
    }

    if (mProcVar0.field_0x3008 == 0) {
        cXyz sp14 = field_0x37c8 - current.pos;
        if (sp14.absXZ() < (31.0f + field_0x3480)) {
            if (field_0x3198 == 0) {
                current.pos = field_0x37c8;
            }

            if (temp_r29 != NULL) {
                if (temp_r29->getLockPos() != NULL) {
                    onResetFlg0(RFLG0_UNK_20000);
                    field_0x3738 = *temp_r29->getLockPos();

                    if (mProcVar1.field_0x300a != 0) {
                        field_0x2f99 = 0x50;
                        setSingleAnimeWolfParam(WANM_JUMP_B_END, &mpHIO->mWolf.mWlPoint.m.mLandingAnm);

                        mNormalSpeed = 0.0f;
                        speed = cXyz::Zero;
                        setSpecialGravity(0.0f, maxFallSpeed, 0);

                        field_0x2f9d = 4;
                        setFootEffectProcType(4);
                        dComIfGp_getVibration().StartShock(2, 0xF, cXyz(0.0f, 1.0f, 0.0f));
                    } else {
                        procWolfTagJumpLandInit(temp_r29);
                    }
                } else {
                    procWolfTagJumpLandInit(NULL);
                }
            } else if (field_0x3198 != 0) {
                procWolfFallInit(2, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
            } else {
                current.pos.x -= 50.0f * cM_ssin(shape_angle.y);
                current.pos.z -= 50.0f * cM_scos(shape_angle.y);
                procWolfTagJumpLandInit(NULL);
            }
        } else {
            procWolfFallInit(2, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
        }
    }

    return 1;
}

int daAlink_c::procWolfTagJumpLandInit(fopAc_ac_c* param_0) {
    commonProcInit(PROC_WOLF_TAG_JUMP_LAND);
    setSingleAnimeWolfParam(WANM_ATTACK_A_END_FRONT, &mpHIO->mWolf.mWlPoint.m.mTagLandingAnm);

    if (mGndPolySpecialCode == dBgW_SPCODE_ICE) {
        mNormalSpeed = mpHIO->mWolf.mWlPoint.m.mLockLandingIceInitSpeed;
    } else {
        mNormalSpeed = 0.0f;
    }

    speed.y = 0.0f;
    field_0x3588 = l_wolfBaseAnime;

    if (param_0 != NULL) {
        field_0x280c.setData(param_0);
    }

    field_0x2f9d = 0x78;
    setFootEffectProcType(1);
    field_0x2f99 = 12;

    dComIfGp_getVibration().StartShock(2, 15, cXyz(0.0f, 1.0f, 0.0f));
    return 1;
}

int daAlink_c::procWolfTagJumpLand() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    daTagWljump_c* wljump_p = (daTagWljump_c*)field_0x280c.getActor();
    if (wljump_p != NULL && wljump_p->getLockPos() != NULL) {
        onResetFlg0(RFLG0_UNK_20000);
        field_0x3738 = *wljump_p->getLockPos();

        if (frameCtrl_p->getFrame() > mpHIO->mWolf.mWlPoint.m.mTagLandingAnm.mCancelFrame) {
            setDoStatus(0x93);

            if (doTrigger()) {
                return procWolfTagJumpInit(field_0x280c.getActor());
            }
        }
    }

    field_0x2f99 = 4;
    if (cLib_addCalc(&mNormalSpeed, 0.0f, mpHIO->mWolf.mWlPoint.m.mLockLandingIceDecelRate,
                     mpHIO->mWolf.mWlPoint.m.mLockLandingIceMaxDecel,
                     mpHIO->mWolf.mWlPoint.m.mLockLandingIceMinDecel) < 3.0f)
    {
        if (checkAnmEnd(frameCtrl_p)) {
            checkNextActionWolf(0);
        } else if (!(frameCtrl_p->getFrame() > mpHIO->mWolf.mWlPoint.m.mTagLandingAnm.mCancelFrame) ||
                   !checkNextActionWolf(1))
        {
            field_0x2f9d = 0x78;
            seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
        }
    } else {
        field_0x2f9d = 0x78;
        seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
    }

    return 1;
}

static fopAc_ac_c* daAlink_searchGiant(fopAc_ac_c* i_actor, void* i_data) {
    cXyz* gnd_chk_pos = static_cast<cXyz*>(i_data);

    if (fopAcM_GetName(i_actor) == PROC_OBJ_SEKIZOA &&
        i_actor->current.pos.abs2(*gnd_chk_pos) < 40000.0f)
    {
        return i_actor;
    }

    return NULL;
}

static void* daAlink_searchGiantTalk(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetName(i_actor) == PROC_OBJ_SEKIZOA &&
        static_cast<daObj_Sekizoa_c*>(i_actor)->getType() == 0)
    {
        return i_actor;
    }

    return NULL;
}

int daAlink_c::procWolfGiantPuzzleInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_GIANT_PUZZLE)) {
        return 0;
    }

    setBlendWolfMoveAnime(mpHIO->mWolf.mWlMove.m.mIdleInterpolation);
    current.angle.y = shape_angle.y;

    field_0x2f98 = 4;
    mProcVar2.mPuzzleAimAngle = shape_angle.y;
    field_0x3478 = 0.0f;
    field_0x347c = mpHIO->mWolf.mWlMoveNoP.m.mDeceleration;
    field_0x594 = mpHIO->mWolf.mWlMoveNoP.m.mMaxSpeed;
    offWolfEyeUp();

    return 1;
}

int daAlink_c::procWolfGiantPuzzle() {
    if (mSpecialMode == 0) {
        return checkNextActionWolf(0);
    }

    if (!checkNoResetFlg0(FLG0_UNK_4000) && mProcVar2.field_0x300c == shape_angle.y &&
        mSpecialMode != 0x28 && checkZeroSpeedF())
    {
        setDoStatusEmphasys(0x1C);

        if (doTrigger()) {
            fopAc_ac_c* giant_p =
                (fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchGiantTalk, NULL);
            if (giant_p != NULL) {
                fopAcM_orderTalkEvent(this, giant_p, 0, 0);
            }
        } else if (checkInputOnR()) {
            int direction = getDirectionFromShapeAngle();
            if (direction == DIR_FORWARD) {
                cXyz gnd_chk_pos(current.pos.x + cM_ssin(shape_angle.y) * 600.0f,
                                 current.pos.y + 10.0f,
                                 current.pos.z + cM_scos(shape_angle.y) * 600.0f);
                mLinkGndChk.SetPos(&gnd_chk_pos);

                if (fabsf(dComIfG_Bgsp().GroundCross(&mLinkGndChk) - current.pos.y) < 0.1f &&
                    fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchGiant, &gnd_chk_pos) == NULL)
                {
                    field_0x347c = mpHIO->mWolf.mWlMoveNoP.m.mAcceleration;
                    field_0x3478 = field_0x594;
                }
            } else if (direction == DIR_LEFT) {
                mProcVar2.mPuzzleAimAngle += 0x4000;
            } else if (direction == DIR_RIGHT) {
                mProcVar2.mPuzzleAimAngle -= 0x4000;
            } else {
                mProcVar2.mPuzzleAimAngle += 0x8000;
            }
        }
    }

    cLib_chaseF(&mNormalSpeed, field_0x3478, field_0x347c);

    if (mProcVar2.mPuzzleAimAngle != shape_angle.y) {
        cLib_addCalcAngleS(
            &shape_angle.y, mProcVar2.mPuzzleAimAngle, mpHIO->mWolf.mWlMoveNoP.m.mTurnAngleRate,
            mpHIO->mWolf.mWlMoveNoP.m.mTurnMax, mpHIO->mWolf.mWlMoveNoP.m.mTurnMin);
        current.angle.y = shape_angle.y;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    } else {
        offModeFlg(1);
    }

    setBlendWolfMoveAnime(-1.0f);
    return 1;
}

void daAlink_c::setWolfHangGroundY() {
    cXyz pos;
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0), &pos);

    mLinkGndChk.SetPos(&pos);
    field_0x33d8 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
}

int daAlink_c::changeWolfHangEndProc() {
    setWolfHangGroundY();

    f32 var_f1;
    if (checkNoResetFlg0(FLG0_UNK_80) && mWaterY > field_0x33d8) {
        var_f1 = mpHIO->mWolf.mWlSwim.m.mStartHeight + mpHIO->mWolf.mWlSwim.m.mClimbBelowWaterSurface;
    } else {
        var_f1 = mpHIO->mWolf.mWlWallHang.m.mHangingFootHeight;
    }

    if (current.pos.y < (field_0x33d8 + var_f1) - 1.0f) {
        return procWolfLandInit();
    } else {
        setDoStatus(0x33);

        if (doTrigger()) {
            current.pos.x -= cM_ssin(shape_angle.y) * 50.0f;
            current.pos.z -= cM_scos(shape_angle.y) * 50.0f;

            onNoResetFlg2(FLG2_UNK_200);
            procWolfFallInit(2, mpHIO->mWolf.mWlWallHang.m.mHangingFallInterp);
            return 1;
        }
    }

    return 0;
}

int daAlink_c::procWolfHangReadyInit() {
    commonProcInit(PROC_WOLF_HANG_READY);
    setSingleAnimeWolf(WANM_VJMP_START, mpHIO->mWolf.mWlWallHang.m.mPreGrabJumpAnm.mSpeed,
                       mpHIO->mWolf.mWlWallHang.m.mPreGrabJumpAnm.mStartFrame, 12,
                       mpHIO->mWolf.mWlWallHang.m.mPreGrabJumpAnm.mInterpolation);
    field_0x3198 = field_0x2f91;
    mProcVar2.field_0x300c = field_0x306e + 0x8000;

    f32 tmp_0 = 0.0f;
    mNormalSpeed = tmp_0;
    speed.y = tmp_0;

    return 1;
}

int daAlink_c::procWolfHangReady() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, 2, 0x1000, 0x400);
    current.angle.y = shape_angle.y;

    if (frameCtrl->checkAnmEnd()) {
        shape_angle.y = mProcVar2.field_0x300c;
        current.angle.y = shape_angle.y;

        if (!procWolfHangWallCatchInit(field_0x3198)) {
            checkWaitAction();
        }
    }

    return 1;
}

int daAlink_c::procWolfHangWallCatchInit(int param_0) {
    cXyz sp18;

    int var_r29;
    if (mProcID != PROC_WOLF_HANG_FALL_START) {
        cXyz spC = current.pos;

        if (checkModeFlg(2)) {
            current.pos.set(field_0x34ec.x - (1.5f * cM_ssin(field_0x306e)), field_0x34ec.y, field_0x34ec.z - (1.5f * cM_scos(field_0x306e)));
            checkHangStartSideWall(field_0x306e);
        }

        if (field_0x34ec.absXZ(current.pos) > 120.0f) {
            current.pos = spC;
            return 0;
        }

        if (checkModeFlg(2)) {
            sp18.set(current.pos.x, 10.0f + field_0x34ec.y, current.pos.z);
        } else {
            sp18.set(field_0x34ec.x - (1.5f * cM_ssin(field_0x306e)), 10.0f + field_0x34ec.y, field_0x34ec.z - (1.5f * cM_scos(field_0x306e)));
        }

        mLinkGndChk.SetPos(&sp18);
        sp18.y = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
        if (fabsf(sp18.y - field_0x34ec.y) > l_autoUpHeight) {
            return 0;
        }

        var_r29 = dComIfG_Bgsp().GetGroundCode(mLinkGndChk);
        shape_angle.y = field_0x306e + 0x8000;
    } else {
        sp18 = current.pos;
        var_r29 = field_0x3174;
    }

    if (checkModeFlg(0x40000)) {
        mProcVar2.field_0x300c = 1;
    } else {
        mProcVar2.field_0x300c = 0;
    }

    commonProcInit(PROC_WOLF_HANG_WALL_CATCH);

    if (param_0 == 8 && var_r29 != 6) {
        setSingleAnimeWolfParam(WANM_VJMP_CLIMB, &mpHIO->mWolf.mWlWallHang.m.mClimbAnm);
        mProcVar3.field_0x300e = 1;
    } else {
        setSingleAnimeWolfParam(WANM_VJMP_CLIMB_HANG, &mpHIO->mWolf.mWlWallHang.m.mWallGrabAnm);
        mProcVar3.field_0x300e = 0;
    }

    current.pos.x = sp18.x;
    current.pos.y = sp18.y;
    current.pos.z = sp18.z;

    current.angle.y = shape_angle.y;

    field_0x3588.x = 0.33864477f;
    field_0x3588.z = -29.525135f;

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;

    dComIfGp_setPlayerStatus0(0, 0x200);
    setWolfHangGroundY();
    field_0x3478 = mpLinkModel->getAnmMtx(0)[1][3];
    mProcVar4.field_0x3010 = 0;
    return 1;
}

int daAlink_c::procWolfHangWallCatch() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar3.field_0x300e != 0 && frameCtrl_p->getFrame() >= 23.0f) {
        field_0x2f99 = 4;
        offModeFlg(0x4000);
    } else if (changeWolfHangEndProc()) {
        return 1;
    }

    f32 temp_f2 = field_0x3478;
    field_0x3478 = mpLinkModel->getAnmMtx(0)[1][3];
    if (temp_f2 < field_0x33b8 && field_0x3478 >= field_0x33b8) {
        seStartMapInfo(Z2SE_WL_OUTOF_WATER);
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar3.field_0x300e == 0) {
            if (field_0x3174 == 6) {
                if (mProcVar4.field_0x3010 == 0) {
                    setSingleAnimeWolf(WANM_HANG, mpHIO->mWolf.mWlWallHang.m.mFallGrabAnm.mSpeed,
                                       35.0f, mpHIO->mWolf.mWlWallHang.m.mFallGrabAnm.mEndFrame,
                                       5.0f);
                    mProcVar4.field_0x3010 = 1;
                }
            } else {
                setSingleAnimeWolf(WANM_VJMP_CLIMB, mpHIO->mWolf.mWlWallHang.m.mClimbAnm.mSpeed,
                                   17.0f, mpHIO->mWolf.mWlWallHang.m.mClimbAnm.mEndFrame, 2.0f);
                mProcVar3.field_0x300e = 1;
            }
        } else {
            checkNextActionWolf(0);
            int temp_r3 = mProcVar2.field_0x300c;

            if (mProcID == PROC_WOLF_WAIT && temp_r3 != 0) {
                procWolfSwimEndWaitInit(0);
            }

            onNoResetFlg1(FLG1_UNK_20);
        }
    } else if (mProcVar3.field_0x300e != 0 &&
               frameCtrl_p->getFrame() > mpHIO->mWolf.mWlWallHang.m.mClimbAnm.mCancelFrame &&
               checkNextActionWolf(1))
    {
        onNoResetFlg1(FLG1_UNK_20);
    }

    return 1;
}

int daAlink_c::procWolfHangFallStartInit(cM3dGPla* param_0) {
    if (mProcID == PROC_WOLF_HANG_WALL_CATCH || checkNoResetFlg1(FLG1_UNK_20)) {
        return 0;
    }

    s16 var_r30 = param_0->mNormal.atan2sX_Z();
    current.pos.x -= param_0->mNormal.x * 1.5f;
    current.pos.z -= param_0->mNormal.z * 1.5f;

    checkHangStartSideWall(var_r30);

    cXyz gnd_chk_pos(current.pos.x, current.pos.y + 50.0f, current.pos.z);
    mLinkGndChk.SetPos(&gnd_chk_pos);
    gnd_chk_pos.y = dComIfG_Bgsp().GroundCross(&mLinkGndChk);

    if (gnd_chk_pos.y != -G_CM3D_F_INF) {
        if (gnd_chk_pos.y < current.pos.y + l_autoDownHeight) {
            return 0;
        }

        current.pos.y = gnd_chk_pos.y;
    }

    commonProcInit(PROC_WOLF_HANG_FALL_START);
    setOldRootQuaternion(0, shape_angle.y - (var_r30 + 0x8000), 0);
    shape_angle.y = var_r30 + 0x8000;
    current.angle.y = shape_angle.y;

    setSingleAnimeWolfParam(WANM_HANG, &mpHIO->mWolf.mWlWallHang.m.mFallGrabAnm);

    f32 sin = cM_ssin(shape_angle.y);
    f32 cos = cM_scos(shape_angle.y);
    cXyz line_start(current.pos.x - sin * 50.0f, current.pos.y - 120.0f,
                    current.pos.z - cos * 50.0f);
    cXyz line_end(current.pos.x + sin, line_start.y, current.pos.z + cos);

    if (commonLineCheck(&line_start, &line_end)) {
        mProcVar2.field_0x300c = 1;
    } else {
        mProcVar2.field_0x300c = 0;
    }

    mProcVar3.field_0x300e = 0;
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    dComIfGp_setPlayerStatus0(0, 0x100);
    field_0x33d8 = mLinkAcch.GetGroundH();
    return 1;
}

int daAlink_c::procWolfHangFallStart() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (field_0x2060->getOldFrameRate() < 0.1f && changeWolfHangEndProc()) {
        return 1;
    }

    if (frameCtrl_p->checkPass(6.0f)) {
        dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
    }

    if (mProcVar2.field_0x300c == 0 && mProcVar3.field_0x300e == 0) {
        if (frameCtrl_p->getFrame() > 22.0f) {
            frameCtrl_p->setFrame(22.0f);
            getNowAnmPackUnder(UNDER_0)->setFrame(22.0f);

            if (checkInputOnR() && getDirectionFromShapeAngle() == DIR_FORWARD) {
                mProcVar3.field_0x300e = 1;
            }
        }
    } else if (checkAnmEnd(mUnderFrameCtrl) &&
               ((checkInputOnR() && getDirectionFromShapeAngle() == DIR_FORWARD) ||
                mProcVar3.field_0x300e != 0))
    {
        procWolfHangWallCatchInit(8);
    }

    return 1;
}

void daAlink_c::setWolfHeadDamage() {
    setUpperAnimeParam(0x2A7, UPPER_2, &mpHIO->mWolf.mWlDamage.m.mRunningAnm);
    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
    freeGrabItem();
    setFacePriTexture(FTANM_UNK_90);
    voiceStart(Z2SE_WL_V_DAMAGE);
}

int daAlink_c::procWolfDamageInit(dCcD_GObjInf* param_0) {
    commonProcInit(PROC_WOLF_DAMAGE);

    cXyz sp14;
    f32 temp_f31 = cM_ssin(shape_angle.y);
    f32 temp_f30 = cM_scos(shape_angle.y);

    cXyz* var_r30;
    if (param_0 != NULL) {
        var_r30 = getDamageVec(param_0);
    } else {
        sp14.set(cM_ssin(field_0x311e), 0.0f, cM_scos(field_0x311e));
        var_r30 = &sp14;
    }

    cXyz sp8((var_r30->z * -temp_f31) + (var_r30->x * temp_f30), var_r30->y, (var_r30->z * temp_f30) + (var_r30->x * temp_f31));

    int temp_r3 = getDirectionFromAngle(cM_atan2s(-sp8.x, -sp8.z));
    if (temp_r3 == DIR_FORWARD) {
        setSingleAnimeWolfParam(WANM_DMG_FRONT, &mpHIO->mWolf.mWlDamage.mNormal.m.mForwardAnm);
        field_0x3478 = mpHIO->mWolf.mWlDamage.mNormal.m.mForwardAnm.mCancelFrame;
    } else if (temp_r3 == DIR_BACKWARD) {
        setSingleAnimeWolfParam(WANM_DMG_BACK, &mpHIO->mWolf.mWlDamage.mNormal.m.mBackwardAnm);
        field_0x3478 = mpHIO->mWolf.mWlDamage.mNormal.m.mBackwardAnm.mCancelFrame;
    } else if (temp_r3 == DIR_LEFT) {
        setSingleAnimeWolfParam(WANM_DMG_LEFT, &mpHIO->mWolf.mWlDamage.mNormal.m.mLeftAnm);
        field_0x3478 = mpHIO->mWolf.mWlDamage.mNormal.m.mLeftAnm.mCancelFrame;
    } else {
        setSingleAnimeWolfParam(WANM_DMG_RIGHT, &mpHIO->mWolf.mWlDamage.mNormal.m.mRightAnm);
        field_0x3478 = mpHIO->mWolf.mWlDamage.mNormal.m.mRightAnm.mCancelFrame;
    }

    setFaceBasicTexture(FTANM_UNK_90);

    current.angle.y = var_r30->atan2sX_Z();
    mProcVar4.field_0x3010 = current.angle.y;

    mNormalSpeed = mpHIO->mWolf.mWlDamage.mNormal.m.mInitSpeed + (mpHIO->mWolf.mWlDamage.mNormal.m.mAttackSpeedRate * var_r30->absXZ());
    if (mNormalSpeed > mpHIO->mWolf.mWlMove.m.mMaxSpeed) {
        mNormalSpeed = mpHIO->mWolf.mWlMove.m.mMaxSpeed;
    }

    field_0x3588 = l_wolfBaseAnime;

    if (param_0 == NULL || (param_0->GetTgHitGObj() != NULL && param_0->GetTgHitGObj()->GetAtMtrl() == dCcD_MTRL_ICE)) {
        mUnderFrameCtrl[0].setRate(mpHIO->mWolf.mWlDamage.mNormal.m.mIceDamageAnmSpeed);
        mUnderFrameCtrl[0].setEnd(3);
        mNormalSpeed = 0.0f;
        mProcVar0.field_0x3008 = 90;
        onNoResetFlg1(FLG1_ICE_FREEZE);

        seStartOnlyReverb(Z2SE_AL_FREEZE);
        voiceStart(Z2SE_WL_V_DAMAGE_FREEZE);
    } else {
        mProcVar0.field_0x3008 = 0;

        seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
        voiceStart(Z2SE_WL_V_DAMAGE);
    }

    mProcVar1.field_0x300a = 0;
    return 1;
}

int daAlink_c::procWolfDamage() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mWolf.mWlDamage.mNormal.m.mDeceleration);
    field_0x2f99 = 5;

    if (checkFreezeDamage()) {
        freezeTimerDamage();
    }

    if (checkAnmEnd(frameCtrl_p)) {
        current.angle.y = shape_angle.y;

        if (!checkFreezeDamage()) {
            checkNextActionWolf(0);
        } else if (mProcVar0.field_0x3008 == 0) {
            seStartOnlyReverb(Z2SE_AL_FREEZE_RECOVER);
            procWolfRollAttackInit(1, 2);
            onModeFlg(8);
        }
    } else if (frameCtrl_p->getFrame() > field_0x3478) {
        current.angle.y = shape_angle.y;

        if (!checkNextActionWolf(1)) {
            current.angle.y = mProcVar4.field_0x3010;
        }
    }

    return 1;
}

int daAlink_c::procWolfLargeDamageUpInit(int param_0, int param_1, s16 param_2, s16 param_3) {
    if (!commonProcInitNotSameProc(PROC_WOLF_LARGE_DAMAGE_UP)) {
        return 0;
    }

    const daAlinkHIO_wlDamLaHu_c1* var_r30;
    if (param_1 != 0) {
        var_r30 = &mpHIO->mWolf.mWlDamage.mLarge.m;
    } else {
        var_r30 = &mpHIO->mWolf.mWlDamage.mHuge.m;
    }

    field_0x3198 = 0;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    field_0x319c = param_0;
    mProcVar4.field_0x3010 = 0;
    mProcVar5.field_0x3012 = 0;

    f32 var_f31, var_f30, var_f29;

    if (param_0 < 0) {
        daAlink_WANM var_r28;
        s16 var_r27;
        if (param_0 == -3) {
            var_r28 = WANM_DMG_AIR_FRONT_GETUP;
            var_f29 = 0.0f;
            var_r27 = -1;

            var_f30 = 5.0f;
            var_f31 = 1.0f;
            mProcVar0.field_0x3008 = -1;
            field_0x3478 = 36.0f;
            field_0x2f9d = 4;
        } else {
            if (param_0 == -2) {
                var_f30 = 0.0f;
                if (getMidnaActor() != NULL) {
                    getMidnaActor()->onForceMorfCancel();
                }
                if (mDemo.getDemoType() != daPy_demo_c::DEMO_TYPE_TOOL_e) {
                    mProcVar5.field_0x3012 = 1;
                }
            } else {
                var_f30 = mpHIO->mWolf.mWlDamage.mLarge.m.mFrontRiseAnm.mInterpolation;
            }

            var_r28 = WANM_DMG_AIR_FRONT_GETUP;
            var_f29 = 16.0f;
            var_r27 = -1;

            field_0x3478 = mpHIO->mWolf.mWlDamage.mLarge.m.mFrontRiseAnm.mCancelFrame;

            if (param_0 == -4) {
                onNoResetFlg2(FLG2_PRESSED_DAMAGE);
                mProcVar0.field_0x3008 = 0;
                var_f31 = 0.0f;
            } else if (param_0 == -5 && mDemo.getParam1() > 0) {
                mProcVar0.field_0x3008 = mDemo.getParam1();
                var_f31 = 0.0f;
            } else if (param_0 == -2 && mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_TOOL_e) {
                mProcVar0.field_0x3008 = 0x1E;
                var_f31 = 0.0f;
            } else {
                mProcVar0.field_0x3008 = 1;
                var_f31 = mpHIO->mWolf.mWlDamage.m.mReturnWakeupAnmSpeed;
            }

            offModeFlg(0x10008000);
            mProcVar3.field_0x300e = 1;
        }

        mProcVar2.field_0x300c = 1;
        setSingleAnimeWolf(var_r28, var_f31, var_f29, var_r27, var_f30);
        setFaceBasicTexture(FTANM_UNK_92);
        field_0x347c = 9.0f;
        field_0x3484 = 13.0f;
    } else {
        mProcVar0.field_0x3008 = 0;
        dComIfGp_getVibration().StartShock(6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        seStartMapInfo(Z2SE_BODY_FALL_DOWN);
        field_0x2f9d = 4;

        if (param_0 == 0x29) {
            setSingleAnimeWolfParam(WANM_DMG_AIR_BACK_GETUP, &var_r30->mBackRiseAnm);
            setFaceBasicTexture(FTANM_UNK_92);
            field_0x3478 = var_r30->mBackRiseAnm.mCancelFrame;
            field_0x347c = 16.0f;
            field_0x3198 = 1;
            field_0x3484 = 15.0;
        } else if (param_0 == 0x2A) {
            setSingleAnimeWolfParam(WANM_DMG_AIR_LEFT_GETUP, &var_r30->mLeftRiseAnm);
            setFaceBasicTexture(FTANM_UNK_93);
            field_0x3478 = var_r30->mLeftRiseAnm.mCancelFrame;
            field_0x347c = 11.0f;
            field_0x3484 = 13.0f;
        } else if (param_0 == 0x2B) {
            setSingleAnimeWolfParam(WANM_DMG_AIR_RIGHT_GETUP, &var_r30->mRightRiseAnm);
            setFaceBasicTexture(FTANM_UNK_93);
            field_0x3478 = var_r30->mRightRiseAnm.mCancelFrame;
            field_0x347c = 11.0f;
            field_0x3484 = 13.0f;
        } else {
            setSingleAnimeWolfParam(WANM_DMG_AIR_FRONT_GETUP, &var_r30->mFrontRiseAnm);
            setFaceBasicTexture(FTANM_UNK_92);
            field_0x3478 = var_r30->mFrontRiseAnm.mCancelFrame;
            field_0x347c = 9.0f;
            field_0x3484 = 13.0f;
        }

        if (param_1 != 0) {
            mProcVar4.field_0x3010 = 1;
        }
    }

    setOldRootQuaternion(param_2, 0, param_3);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;

    setFootEffectProcType(4);
    field_0x3588 = l_wolfBaseAnime;
    field_0x3480 = 1.0f / mUnderFrameCtrl[0].getEnd();
    return 1;
}

int daAlink_c::procWolfLargeDamageUp() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
    mProcVar4.field_0x3010 = 0;

    if (mProcVar2.field_0x300c == 0) {
        field_0x2f99 = 5;
    } else {
        mProcVar2.field_0x300c = 0;
    }

    if (field_0x3198 != 0) {
        f32 temp_f31 = 2.0f * (0.5f - (field_0x3480 * temp_r29->getFrame()));
        field_0x3588.x = l_wolfBaseAnime.x * temp_f31;
        field_0x3588.z = l_wolfBaseAnime.z * temp_f31;
    }

    if (field_0x319c == -4) {
        if (checkNoResetFlg2(FLG2_PRESSED_DAMAGE)) {
            return 1;
        }
        temp_r29->setRate(mpHIO->mWolf.mWlDamage.mLarge.m.mFrontRiseAnm.mSpeed);
    }

    if (mProcVar0.field_0x3008 > 0) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == 0) {
            mProcVar0.field_0x3008 = -1;
            temp_r29->setRate(mpHIO->mWolf.mWlDamage.m.mReturnWakeupAnmSpeed);
            temp_r29->offEndFlg();
        }
    } else {
        if (mProcVar3.field_0x300e != 0 && temp_r29->checkPass(24.0f)) {
            voiceStart(Z2SE_WL_V_RESTART);
        }

        if (temp_r29->getFrame() > field_0x347c) {
            offModeFlg(0x8000);
        }

        if (checkAnmEnd(temp_r29)) {
            if (mProcVar5.field_0x3012 != 0) {
                daMidna_c* temp_r3_2 = (daMidna_c*)getMidnaActor();
                if (temp_r3_2->checkDemoPortalWarpWait()) {
                    temp_r3_2->changeDemoMode(0xD);
                    return procWolfMidnaRideShockInit();
                }
            }

            if (mDemo.getDemoMode() == 0x1B || mDemo.getDemoMode() == 9) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            } else {
                checkNextActionWolf(0);
            }
        } else if (temp_r29->getFrame() > field_0x3478) {
            checkNextActionWolf(1);
        } else if (checkEndResetFlg1(ERFLG1_LARGE_DAMAGE_UP_STOP) && temp_r29->getFrame() > field_0x3484) {
            temp_r29->setFrame(field_0x3484);
            getNowAnmPackUnder(UNDER_0)->setFrame(field_0x3484);
        }
    }

    return 1;
}

int daAlink_c::procWolfLandDamageInit(int param_0) {
    f32 var_f31;

    if (param_0 == 2) {
        if (!checkEventRun()) {
            setLandDamagePoint(8);
        }

        var_f31 = mpHIO->mWolf.mWlDamage.mFall.m.mLandingAnm.mStartFrame;
        mProcVar0.field_0x3008 = mpHIO->mDamage.mDamFall.m.mBigStopTime;
        dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
    } else {
        var_f31 = mpHIO->mWolf.mWlDamage.mFall.m.mSmallDamageLandingStartFrame;
        mProcVar0.field_0x3008 = mpHIO->mWolf.mWlDamage.mFall.m.mSmallStopTime;

        if (param_0 == 1) {
            if (!checkEventRun()) {
                setLandDamagePoint(4);
            }

            dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
        }
    }

    commonProcInit(PROC_WOLF_LAND_DAMAGE);
    mNormalSpeed = 0.0f;

    if (param_0 == 0) {
        offModeFlg(8);
    } else {
        seStartOnlyReverb(JA_SE_LK_FALL_DAMAGE);
        voiceStart(Z2SE_WL_V_LAND_DAMAGE);
    }

    setSingleAnimeWolf(WANM_FALL_LAND, mpHIO->mWolf.mWlDamage.mFall.m.mLandingAnm.mSpeed, var_f31,
                       mpHIO->mWolf.mWlDamage.mFall.m.mLandingAnm.mEndFrame,
                       mpHIO->mWolf.mWlDamage.mFall.m.mLandingAnm.mInterpolation);
    setFaceBasicTexture(FTANM_UNK_95);
    field_0x3198 = 0;
    field_0x2f9d = 4;
    setFootEffectProcType(4);
    return 1;
}

int daAlink_c::procWolfLandDamage() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (field_0x3198 != 0) {
        if (frameCtrl->checkAnmEnd()) {
            checkNextActionWolf(0);
        } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlDamage.mFall.m.mRightRiseAnm.mCancelFrame) {
            checkNextActionWolf(1);
        }
    } else if (frameCtrl->checkAnmEnd()) {
        if (mProcVar0.field_0x3008 > 0) {
            mProcVar0.field_0x3008--;
        } else {
            setSingleAnimeWolfParam(WANM_FALL_LAND_START, &mpHIO->mWolf.mWlDamage.mFall.m.mRightRiseAnm);
            setFaceBasicTexture(FTANM_UNK_96);
            field_0x3198 = 1;
        }
    }

    return 1;
}

void daAlink_c::setWolfScreamWaitAnime() {
    setSingleAnimeWolfBase(WANM_WAIT_STUNNED);
    setFaceBasicTexture(FTANM_UNK_91);
    voiceStart(Z2SE_WL_V_TERRORED);

    mProcVar2.field_0x300c = 1;
    dComIfGp_getVibration().StartQuake(5, 1, cXyz(0.0f, 1.0f, 0.0f));
}

int daAlink_c::procWolfScreamWaitInit() {
    commonProcInit(PROC_WOLF_SCREAM_WAIT);
    if (checkEndResetFlg1(ERFLG1_UNK_2)) {
        setWolfScreamWaitAnime();
    } else {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed,
                                    mpHIO->mWolf.mWlMove.m.mIdleInterpolation);
        mProcVar2.field_0x300c = 0;
    }

    mNormalSpeed = 0.0f;
    mProcVar3.field_0x300e = 0;
    field_0x3588 = l_wolfBaseAnime;

    return 1;
}

int daAlink_c::procWolfScreamWait() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (!checkEndResetFlg1(ERFLG1_UNK_1)) {
        if (mProcVar3.field_0x300e != 0) {
            field_0x2f99 = 5;

            if (frameCtrl->checkAnmEnd()) {
                checkNextActionWolf(0);
            } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlDamage.mLarge.m.mFrontRiseAnm.mCancelFrame) {
                checkNextActionWolf(1);
            }
        } else {
            mProcVar3.field_0x300e = 1;
            setSingleAnimeWolf(WANM_DMG_AIR_FRONT_GETUP,
                               mpHIO->mWolf.mWlDamage.mLarge.m.mFrontRiseAnm.mSpeed, 20.0f,
                               mpHIO->mWolf.mWlDamage.mLarge.m.mFrontRiseAnm.mEndFrame,
                               mpHIO->mWolf.mWlDamage.mLarge.m.mFrontRiseAnm.mInterpolation);
            field_0x2f99 = 13;
        }
    } else if (mProcVar2.field_0x300c == 0 && checkEndResetFlg1(ERFLG1_UNK_2)) {
        setWolfScreamWaitAnime();
    }

    return 1;
}

f32 daAlink_c::getWolfLieMoveAnmSpeed() {
    if (field_0x3198 != 0) {
        return getAnmSpeedStickRate(0.5f, 2.0f);
    }

    return getAnmSpeedStickRate(mpHIO->mWolf.mWlLie.m.mProneMinAnmSpeed,
                                mpHIO->mWolf.mWlLie.m.mProneMaxAnmSpeed);
}

f32 daAlink_c::getWolfLieMoveSpeed() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    f32 var_f1;
    if (frameCtrl_p->getFrame() >= 15.0f) {
        var_f1 = frameCtrl_p->getFrame() - 15.0f;
    } else {
        var_f1 = frameCtrl_p->getFrame();
    }

    return mpHIO->mWolf.mWlLie.m.mProneMoveRate * frameCtrl_p->getRate() *
           cM_fsin(var_f1 * 0.20943952f);
}

BOOL daAlink_c::checkMidnaDisappearMode() const {
    return mProcID == PROC_WOLF_DIG_THROUGH ||
           (checkWolf() && dComIfGp_checkPlayerStatus0(0, 0x8000000) &&
            !dComIfGp_checkPlayerStatus0(0, 0x10) &&
            (mTargetedActor == NULL || !(mTargetedActor->attention_info.flags & fopAc_AttnFlag_UNK_0x800000)));
}

BOOL daAlink_c::checkWolfLieContinue(int param_0) {
    cM3dGPla slide_poly;

    if ((param_0 ||
         ((mTargetedActor != NULL && (mTargetedActor->attention_info.flags & fopAc_AttnFlag_UNK_0x800000)) &&
          (fopAcM_GetName(mTargetedActor) == PROC_NPC_JAGAR ||
           fopAcM_GetName(mTargetedActor) == PROC_NPC_BOU))) &&
        !getSlidePolygon(&slide_poly) && !checkWolfLieWaterIn())
    {
        return true;
    }

    return false;
}

int daAlink_c::checkNextActionWolfFromLie() {
    if (checkNextActionWolf(0)) {
        if (field_0x2f8c == 1 || field_0x2f8c == 2 || field_0x2f8c == 3) {
            field_0x2060->initOldFrameMorf(mpHIO->mWolf.mWlLie.m.mStandUpInterp, 0, 40);
        }
        return true;
    }

    return false;
}

void daAlink_c::setWolfLieMoveVoice(int param_0) {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (frameCtrl->getRate() == 0.0f && mProcVar5.field_0x3012 != 0) {
        mProcVar5.field_0x3012--;
    }

    if ((param_0 != 0 && (frameCtrl->checkPass(11.0f) || frameCtrl->checkPass(26.0f))) ||
        (frameCtrl->getRate() == 0.0f && mProcVar5.field_0x3012 == 0))
    {
        voiceStart(Z2SE_WL_V_BREATH_WALK);
        mProcVar5.field_0x3012 = 30;
    }
}

bool daAlink_c::checkWolfLieCode() {
    if (mLinkAcch.ChkGroundHit() && mGndPolySpecialCode == 4) {
        return true;
    }

    return false;
}

int daAlink_c::procWolfLieStartInit(int param_0) {
    commonProcInit(PROC_WOLF_LIE_START);
    field_0x3588 = l_wolfBaseAnime;
    setSingleAnimeWolfParam(WANM_CROUCH, &mpHIO->mWolf.mWlLie.m.mProneStartAnm);

    if (param_0) {
        shape_angle.y = field_0x306e + 0x8000;
        current.pos.x = field_0x34ec.x + cM_ssin(field_0x306e) * 90.0f;
        current.pos.z = field_0x34ec.z + cM_scos(field_0x306e) * 90.0f;
        mProcVar4.field_0x3010 = 1;
    } else {
        mProcVar4.field_0x3010 = 0;
    }

    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x2f99 = 0;
    mProcVar5.field_0x3012 = 30;
    dComIfGp_setPlayerStatus0(0, 0x8000000);
    return 1;
}

int daAlink_c::procWolfLieStart() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    decideCrawlDoStatus();

    if (mProcVar4.field_0x3010 != 0) {
        if (checkAnmEnd(frameCtrl_p)) {
            procWolfLieMoveInit(1);
        }
    } else if (!checkWolfLieContinue(0)) {
        checkNextActionWolfFromLie();
    } else if (checkInputOnR() &&
               (checkAnmEnd(frameCtrl_p) ||
                frameCtrl_p->getFrame() > mpHIO->mWolf.mWlLie.m.mProneStartAnm.mCancelFrame))
    {
        procWolfLieMoveInit(0);
    } else {
        field_0x2f99 = 4;
        setWolfLieMoveVoice(0);
        setTalkStatus();
        orderTalk(1);
    }

    return 1;
}

int daAlink_c::procWolfLieMoveInit(int param_0) {
    BOOL var_r30 = mProcID == PROC_TALK;
    BOOL var_r29 = checkUnderMove0BckNoArcWolf(WANM_CROUCH_WALK) == FALSE;
    BOOL var_r28;
    if (dComIfGp_checkPlayerStatus0(0, 0x2000)) {
        var_r28 = TRUE;
    } else {
        var_r28 = FALSE;
    }

    if (!commonProcInitNotSameProc(PROC_WOLF_LIE_MOVE)) {
        return 0;
    }

    mProcVar3.field_0x300e = 0;

    if (!var_r29) {
        if (var_r30) {
            var_r29 = true;
        } else {
            onModeFlg(0x6000);
        }
    } else {
        f32 anm_speed = getWolfLieMoveAnmSpeed();

        if (mDemo.getDemoMode() == 0x2C) {
            anm_speed = 0.0f;
        } else if (param_0) {
            mProcVar3.field_0x300e = 1;
            onModeFlg(0x6000);
        } else if (getDirectionFromShapeAngle() == DIR_BACKWARD) {
            anm_speed *= -1.0f;
        }

        current.angle.y = shape_angle.y;
        setSingleAnimeWolfBaseSpeed(WANM_CROUCH_WALK, anm_speed, mpHIO->mWolf.mWlLie.m.mProneMoveInterp);
    }

    field_0x3198 = var_r29 ^ 1;
    mProcVar4.field_0x3010 = var_r30;
    field_0x2f99 = 0xD;

    dComIfGp_setPlayerStatus0(0, 0x8000000);
    if (var_r28) {
        dComIfGp_setPlayerStatus0(0, 0x2000);
    }

    mProcVar5.field_0x3012 = 30;

    if (checkAttentionLock()) {
        field_0x2fe4 = shape_angle.y;
    }

    return 1;
}

int daAlink_c::procWolfLieMove() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

    cXyz sp64;
    cXyz sp58;
    cXyz sp4C;
    field_0x2f99 = 0xD;

    decideCrawlDoStatus();

    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_wolfLieStandUpOffset, &sp58);
    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_wolfLieFrontUpOffset, &sp64);
    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_wolfLieBackUpOffset, &sp4C);

    cXyz sp40;
    cXyz sp34;
    cMtx_multVecSR(mpLinkModel->getBaseTRMtx(), &l_crawlMinSideOffset, &sp34);

    BOOL sp18 = checkNotCrawlStand(&sp58);
    BOOL sp14 = checkNotCrawlStand(&sp64);
    BOOL sp10 = checkNotCrawlStand(&sp4C);
    int var_r28;
    if (sp18 || sp14 || sp10 || mDemo.getDemoMode() == 0x2C || checkNotCrawlStand(&sp64, &sp34) || checkNotCrawlStand(&sp4C, &sp34) || checkNotCrawlStand(&sp58, &sp34)) {
        var_r28 = 0;
        onModeFlg(0x04000000);
    } else {
        var_r28 = 1;
        offModeFlg(0x04000000);
    }

    if (mDemo.getDemoMode() == 0x2C) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    cM3dGPla sp70;

    if (!checkWolfLieContinue(!var_r28 || mProcVar3.field_0x300e != 0 || mProcVar4.field_0x3010 != 0)) {
        checkNextActionWolfFromLie();
    } else {
        mProcVar4.field_0x3010 = 0;
        int var_r27 = 0;

        f32 temp_f31 = getWolfLieMoveAnmSpeed();
        if (temp_r29->getRate() > 0.0f) {
            temp_r29->setRate(temp_f31);
        } else if (temp_r29->getRate() < 0.0f) {
            temp_r29->setRate(-temp_f31);
        } else {
            var_r27 = 1;
        }

        if (var_r27 == 1 || temp_r29->checkPass(0.0f) || temp_r29->checkPass(15.0f)) {
            if (mProcVar3.field_0x300e != 0) {
                mProcVar3.field_0x300e--;
                temp_r29->setRate(temp_f31);
                temp_r29->setLoop(0);
            } else if (checkInputOnR()) {
                if (getDirectionFromShapeAngle() != 1) {
                    temp_r29->setRate(temp_f31);
                    temp_r29->setLoop(0);
                } else {
                    temp_r29->setRate(-temp_f31);
                    temp_r29->setLoop(temp_r29->getEnd());
                }

                initBasAnime();
            } else if (var_r27 == 0) {
                stopHalfMoveAnime(15.0f);
            }
        }

        setWolfLieMoveVoice(1);
        int spC = field_0x3198;
        s16 spA;

        cXyz sp28;
        field_0x3198 = 0;

        if (checkCrawlInHoll(&sp64, &sp4C, &sp28, var_r28)) {
            setCrawlMoveHoll();
        } else if (spC && sp18 && sp14 && sp10 && var_r28 == 0 && changeCrawlAutoMoveProc(&sp28)) {
            return 1;
        } else if (mProcVar3.field_0x300e == 0 && checkInputOnR()) {
            if (getWolfLieMoveSpeed() >= 0.0f) {
                spA = field_0x2fe2;
            } else {
                spA = field_0x2fe2 + 0x8000;
            }
    
            cLib_addCalcAngleS(&shape_angle.y, spA, mpHIO->mWolf.mWlLie.m.mProneTurnRate, mpHIO->mWolf.mWlLie.m.mProneTurnMax, mpHIO->mWolf.mWlLie.m.mProneTurnMin);
        }

        mNormalSpeed = getWolfLieMoveSpeed();

        cXyz sp1C;
        if (mNormalSpeed < 0.0f) {
            mNormalSpeed *= -1.0f;
            current.angle.y = shape_angle.y + 0x8000;
        } else {
            current.angle.y = shape_angle.y;
        }

        if (field_0x3198 != 0) {
            onModeFlg(0x6000);
        } else {
            offModeFlg(0x6000);
        }

        if (checkAttentionLock()) {
            field_0x2fe4 = shape_angle.y;
        }

        setTalkStatus();
        orderTalk(1);
    }

    return 1;
}

int daAlink_c::procWolfLieAutoMoveInit(int param_0, cXyz* param_1) {
    BOOL var_r30 = dComIfGp_checkPlayerStatus0(0, 0x2000) != 0 ? TRUE : FALSE;
    commonProcInit(PROC_WOLF_LIE_AUTO_MOVE);

    field_0x37c8 = *param_1;
    field_0x3198 = param_0;
    mProcVar0.field_0x3008 = 20;
    mProcVar1.field_0x300a = 300;

    setCrawlMoveDirectionArrow();
    mNormalSpeed = 0.0f;
    field_0x2f99 = 13;

    dComIfGp_setPlayerStatus0(0, 0x8000000);
    if (var_r30) {
        dComIfGp_setPlayerStatus0(0, 0x2000);
    }

    mProcVar5.field_0x3012 = 30;
    return 1;
}

int daAlink_c::procWolfLieAutoMove() {
    field_0x2f99 = 0xD;

    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
    s16 temp_r27 = shape_angle.y;
    s16 temp_r26 = current.angle.y;

    setWolfLieMoveVoice(1);

    if (mProcVar0.field_0x3008 > 0) {
        if (temp_r29->checkPass(0.0f) || temp_r29->checkPass(15.0f)) {
            stopHalfMoveAnime(15.0f);
            mNormalSpeed = 0.0f;
        } else if (checkAnmEnd(temp_r29)) {
            mNormalSpeed = 0.0f;
            mProcVar0.field_0x3008--;
        }

        setCrawlMoveDirectionArrow();
        decideCrawlDoStatus();
        shape_angle.y = temp_r27;
        current.angle.y = temp_r26;
    } else if (mProcVar0.field_0x3008 == 0) {
        setCrawlMoveDirectionArrow();

        if (checkInputOnR()) {
            setCrawlAutoMoveAimPos();

            if (mProcVar0.field_0x3008 == -1) {
                dComIfGp_setAdvanceDirection(0);

                if (shape_angle.y == current.angle.y) {
                    temp_r29->setRate(2.0f);
                    temp_r29->setLoop(0);
                } else {
                    temp_r29->setRate(-2.0f);
                    temp_r29->setLoop(temp_r29->getEnd());
                }

                initBasAnime();
            }
        } else {
            decideCrawlDoStatus();
            shape_angle.y = temp_r27;
            current.angle.y = temp_r26;
        }
    } else {
        int var_r28 = 1;
        if (mProcVar1.field_0x300a > 0) {
            mProcVar1.field_0x300a -= 1;
        }

        if (mProcVar3.field_0x300e != 0) {
            if (cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, 5, 0x480, 0x80)) {
                var_r28 = 0;
            }

            if (mProcVar3.field_0x300e < 0) {
                current.angle.y = shape_angle.y + 0x8000;
            } else {
                current.angle.y = shape_angle.y;
            }

            cLib_addCalc(&current.pos.x, field_0x37c8.x, 0.5f, 3.0f, 1.0f);
            cLib_addCalc(&current.pos.z, field_0x37c8.z, 0.5f, 3.0f, 1.0f);
        }

        cXyz sp8 = field_0x37c8 - current.pos;
        if (cLib_distanceAngleS(sp8.atan2sX_Z(), current.angle.y) < 0x6000) {
            var_r28 = 0;
        }

        mNormalSpeed = getWolfLieMoveSpeed();
        if (mNormalSpeed < 0.0f) {
            mNormalSpeed *= -1.0f;
            current.angle.y = shape_angle.y + 0x8000;
        }

        if (var_r28 != 0 || mProcVar1.field_0x300a == 0) {
            procWolfLieMoveInit(0);
        }
    }

    field_0x310c = shape_angle.y;
    return 1;
}

void daAlink_c::setSpeedAndAngleSwimWolf() {
    if (checkWolfDashMode()) {
        field_0x594 = mpHIO->mWolf.mWlSwim.m.mMaxSpeed;
    } else {
        field_0x594 = mpHIO->mWolf.mWlSwim.m.mMaxSpeedWeak;
    }

    f32 var_f31;
    if (checkInputOnR()) {
        s16 prev_angle = current.angle.y;
        cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, mpHIO->mWolf.mWlSwim.m.mTurnRate,
                           mpHIO->mWolf.mWlSwim.m.mTurnMax, mpHIO->mWolf.mWlSwim.m.mTurnMin);

        var_f31 = field_0x33a8 * 3.0f * cM_scos(current.angle.y - prev_angle);
        shape_angle.y = current.angle.y;
    } else {
        var_f31 = 0.0f;
    }

    if (checkWolfSwimDashAnime()) {
        var_f31 = 3.0f;
    }

    if (checkAttentionState() && mProcID == PROC_WOLF_SWIM_WAIT) {
        if (mTargetedActor != NULL) {
            setShapeAngleToAtnActor(0);
        } else {
            cLib_addCalcAngleS(&shape_angle.y, field_0x2fe4, mpHIO->mWolf.mWlSwim.m.mIdleTurnRate,
                               mpHIO->mWolf.mWlSwim.m.mIdleTurnMax,
                               mpHIO->mWolf.mWlSwim.m.mIdleTurnMin);
        }

        current.angle.y = shape_angle.y;
    }

    setNormalSpeedF(var_f31, mpHIO->mWolf.mWlSwim.m.mDeceleration);

    if (dComIfGp_checkPlayerStatus0(0, 0x10)) {
        mNormalSpeed = 0.0f;
    }
}

f32 daAlink_c::getWolfSwimMoveAnmSpeed() {
    f32 anm_speed = fabsf(mNormalSpeed) / field_0x594;
    if (anm_speed > 1.0f) {
        anm_speed = 1.0f;
    }

    if (checkWolfDashMode()) {
        anm_speed =
            mpHIO->mWolf.mWlSwim.m.mMoveMinAnmSpeed +
            anm_speed * (mpHIO->mWolf.mWlSwim.m.mMoveMaxAnmSpeed - mpHIO->mWolf.mWlSwim.m.mMoveMinAnmSpeed);
    } else {
        anm_speed =
            mpHIO->mWolf.mWlSwim.m.mMoveMinAnmSpeedWeak +
            anm_speed * (mpHIO->mWolf.mWlSwim.m.mMoveMaxAnmSpeedWeak - mpHIO->mWolf.mWlSwim.m.mMoveMinAnmSpeedWeak);
    }

    return anm_speed;
}

int daAlink_c::decideDoStatusSwimWolf() {
    setTalkStatus();

    if (orderTalk(1)) {
        return 1;
    }

    if (field_0x27f4 != NULL && fopAcM_GetName(field_0x27f4) == PROC_Obj_Drop) {
        setDoStatus(0x39);
        if (doTrigger()) {
            fopAcM_setCarryNow(field_0x27f4, 0);
            onResetFlg0(RFLG0_UNK_8000);
        }
    } else if (checkNoResetFlg0(FLG0_SWIM_UP) && field_0x30d2 == 0 && !checkWolfSwimDashAnime() &&
               mProcID == PROC_WOLF_SWIM_MOVE)
    {
        setDoStatus(9);

        if (doTrigger()) {
            onNoResetFlg1(FLG1_DASH_MODE);

            if (field_0x2fc7 == 1 || field_0x2fc7 == 3) {
                field_0x30d0 = mpHIO->mWolf.mWlMove.m.mADashDurationSlow;
            } else if (field_0x2fc7 == 2) {
                field_0x30d0 = mpHIO->mWolf.mWlMove.m.mADashDurationSlow2;
            } else {
                field_0x30d0 = mpHIO->mWolf.mWlMove.m.mADashDuration;
            }

            setSingleAnimeWolfParam(WANM_SWIM_DASH, &mpHIO->mWolf.mWlSwim.m.mDashAnm);
        }
    }

    if (checkWolfSwimDashAnime() && mUnderFrameCtrl[0].checkPass(7.0f)) {
        cXyz pillar_pos(current.pos.x + cM_ssin(shape_angle.y) * 60.0f, current.pos.y,
                        current.pos.z + cM_scos(shape_angle.y) * 60.0f);
        fopKyM_createWpillar(&pillar_pos, 0.75f, 0);
    }

    return 0;
}

int daAlink_c::procWolfSwimUpInit() {
    commonProcInit(PROC_WOLF_SWIM_UP);
    speed.y = 0.0f;
    current.pos.y = mWaterY;
    onNoResetFlg0(FLG0_SWIM_UP);

    mZ2Link.setInWater(false);
    setSingleAnimeWolfParam(WANM_SWIM_RESURFACE, &mpHIO->mWolf.mWlSwim.m.mSurfacingAnm);
    setFaceBasicTexture(FTANM_UNK_8E);
    dComIfGp_setPlayerStatus0(0, 0x100000);
    seStartMapInfo(Z2SE_WL_OUTOF_WATER);
    mProcVar5.field_0x3012 = 1;
    return 1;
}

int daAlink_c::procWolfSwimUp() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    setNormalSpeedF(0.0f, mpHIO->mWolf.mWlSwim.m.mDeceleration);

    if (frameCtrl->checkAnmEnd()) {
        procWolfSwimWaitInit(0);
    } else if (checkInputOnR() &&
               frameCtrl->getFrame() > mpHIO->mWolf.mWlSwim.m.mSurfacingAnm.mCancelFrame)
    {
        procWolfSwimMoveInit();
    } else {
        current.pos.y = mWaterY;
    }

    return 1;
}

int daAlink_c::procWolfSwimWaitInit(int param_0) {
    f32 morf;
    if (mProcID == PROC_WOLF_SWIM_UP) {
        morf = 0.0f;
    } else {
        morf = mpHIO->mWolf.mWlSwim.m.mIdleInterp;
    }

    commonProcInit(PROC_WOLF_SWIM_WAIT);

    if (param_0) {
        seStartMapInfo(Z2SE_WL_INTO_WATER);
        mProcVar4.field_0x3010 = 0;
        mProcVar5.field_0x3012 = 1;
    } else {
        mProcVar5.field_0x3012 = 0;
    }

    f32 anm_speed;
    if (checkWolfDashMode()) {
        anm_speed = mpHIO->mWolf.mWlSwim.m.mIdleAnmSpeed;
    } else {
        anm_speed = mpHIO->mWolf.mWlSwim.m.mIdleAnmSpeedWeak;
    }

    setSingleAnimeWolfBaseSpeed(WANM_SWIM_WAIT, anm_speed, morf);
    dComIfGp_setPlayerStatus0(0, 0x100000);

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        current.pos.y = mWaterY;
        speed.y = 0.0f;
    }

    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procWolfSwimWait() {
    if (mDemo.getDemoMode() == 6 || mDemo.getDemoMode() == 8) {
        setShapeAngleToTalkActor();
        current.angle.y = shape_angle.y;
    }

    setSpeedAndAngleSwimWolf();
    setSwimUpDownOffset();

    if (checkWolfDashMode()) {
        mUnderFrameCtrl[0].setRate(mpHIO->mWolf.mWlSwim.m.mIdleAnmSpeed);
    } else {
        mUnderFrameCtrl[0].setRate(mpHIO->mWolf.mWlSwim.m.mIdleAnmSpeedWeak);
    }

    if (checkSwimUpAction()) {
        return 1;
    }

    if (decideDoStatusSwimWolf()) {
        return 1;
    }

    if (checkInputOnR()) {
        procWolfSwimMoveInit();
    }

    return 1;
}

int daAlink_c::procWolfSwimMoveInit() {
    commonProcInit(PROC_WOLF_SWIM_MOVE);
    setSingleAnimeWolfBaseSpeed(WANM_SWIM, getWolfSwimMoveAnmSpeed(),
                                mpHIO->mWolf.mWlSwim.m.mMoveInterp);

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        current.pos.y = mWaterY;
        speed.y = 0.0f;
    }

    dComIfGp_setPlayerStatus0(0, 0x100000);
    return 1;
}

int daAlink_c::procWolfSwimMove() {
    setSpeedAndAngleSwimWolf();

    if (!checkWolfSwimDashAnime()) {
        mUnderFrameCtrl[0].setRate(getWolfSwimMoveAnmSpeed());
    }

    if (checkSwimUpAction()) {
        return 1;
    }

    if (decideDoStatusSwimWolf()) {
        return 1;
    }

    if (checkAnmEnd(mUnderFrameCtrl) && checkWolfSwimDashAnime()) {
        setSingleAnimeWolfBaseSpeed(WANM_SWIM, getWolfSwimMoveAnmSpeed(),
                                    mpHIO->mWolf.mWlSwim.m.mMoveInterp);

        if (field_0x2fc7 == 1 || field_0x2fc7 == 3) {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTimeSlow;
        } else if (field_0x2fc7 == 2) {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTimeSlow2;
        } else {
            field_0x30d2 = mpHIO->mWolf.mWlMove.m.mADashCooldownTime;
        }
    }

    if (checkFrontWallTypeAction()) {
        swimOutAfter(1);
        return 1;
    }

    if (!checkInputOnR() && !checkWolfSwimDashAnime()) {
        procWolfSwimWaitInit(0);
    }

    return 1;
}

int daAlink_c::procWolfSwimEndWaitInit(int param_0) {
    commonProcInit(PROC_WOLF_SWIM_END_WAIT);
    setSingleAnimeWolfParam(WANM_WAIT_SHAKE, &mpHIO->mWolf.mWlSwim.m.mWaterSplashIdleAnm);
    setFaceBasicTexture(FTANM_UNK_8F);

    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(3), &field_0x37c8);

    if (param_0 == 0) {
        mProcVar3.field_0x300e = 0;
    } else if (field_0x2fbb == 13) {
        mProcVar3.field_0x300e = 2;
    } else {
        mProcVar3.field_0x300e = 1;
    }

    JPABaseEmitter* spC = NULL;
    JPABaseEmitter* sp8 = NULL;

    field_0x32cc = 0;
    if (param_0 != 0) {
        field_0x31bc = 0;
    } else {
        for (int i = 0; i < 2; i++) {
            field_0x32c0[i] = 0;
        }
    }

    setWolfSwimEndEffect(&spC, &sp8);
    mProcVar2.field_0x300c = param_0;
    return 1;
}

int daAlink_c::procWolfSwimEndWait() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(3), &field_0x37c8);

    if (frameCtrl_p->checkPass(6.0f)) {
        if (mProcVar2.field_0x300c == 0) {
            seStartOnlyReverb(Z2SE_WOLF_WIPE_OFF_WATER);
        } else {
            seStartOnlyReverb(Z2SE_WOLF_WIPE_OFF_DUST);
        }
    }

    JPABaseEmitter* spC = NULL;
    JPABaseEmitter* sp8 = NULL;
    setWolfSwimEndEffect(&spC, &sp8);

    if (checkAnmEnd(frameCtrl_p)) {
        checkNextActionWolf(0);
    } else if (frameCtrl_p->getFrame() > mpHIO->mWolf.mWlSwim.m.mWaterSplashIdleAnm.mCancelFrame) {
        checkNextActionWolf(1);
    }

    if (mProcID != PROC_WOLF_SWIM_END_WAIT) {
        if (spC != NULL) {
            spC->stopDrawParticle();
        }

        if (sp8 != NULL) {
            sp8->stopDrawParticle();
        }
    }

    return 1;
}

bool daAlink_c::onWolfEnemyBiteAll(fopAc_ac_c* param_0, daPy_py_c::daPy_FLG2 param_1) {
    if (field_0x281c.getActor() != NULL) {
        return 0;
    }

    if (param_1 == 8) {
        if (mCutType != 0x2C && mCutType != 0x2D && mCutType != 0x31 && mCutType != 0x39 &&
            mCutType != 0x32)
        {
            return 0;
        }
    } else if (mCutType != 0x39) {
        return 0;
    }

    onNoResetFlg2(param_1);
    field_0x281c.setData(param_0);

    return 1;
}

void daAlink_c::resetWolfEnemyBiteAll() {
    field_0x281c.clearData();
    offNoResetFlg2(FLG2_UNK_58);
}

void daAlink_c::checkWolfEnemyThrowAction() {
    daPy_frameCtrl_c* frameCtrl = &mUpperFrameCtrl[2];

    if (frameCtrl->checkAnmEnd()) {
        resetUpperAnime(UPPER_2, 3.0f);
    } else if (frameCtrl->checkPass(17.0f)) {
        resetWolfEnemyBiteAll();
    }
}

void daAlink_c::setWolfLockDomeModel() {
    JKRHeap* prev_heap = setItemHeap();

    mHeldItemModel = initModel(loadAramBmd(799, 0x1C00), 0x200);
    field_0x0718 = loadAramItemBtk(0x3A6, mHeldItemModel);
    field_0x0724 = loadAramItemBrk(0x324, mHeldItemModel);
    mDoExt_setCurrentHeap(prev_heap);

    mEquipItem = 0x109;
    mItemVar0.field_0x3018 = 0;
    mSearchBallScale = mpHIO->mWolf.mWlAttack.mWlAtLock.m.mStartRadius;

    dComIfGp_setPlayerStatus1(0, 0x800000);
    seStartOnlyReverb(Z2SE_MIDNA_BIND_AREA_OPEN);

    if (dKy_darkworld_check()) {
        field_0x0724->setFrame(1.0f);
    } else {
        field_0x0724->setFrame(0.0f);
    }
}

void daAlink_c::setWolfBallModel() {
    JKRHeap* prev_heap = setItemHeap();

    mHeldItemModel = initModel(static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, 0x25)), 0x200);
    field_0x0718 = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(l_arcName, 0x49));
    field_0x0718->searchUpdateMaterialID(mHeldItemModel->getModelData());

    mpHookSound = new Z2SoundObjSimple();
    mDoExt_setCurrentHeap(prev_heap);

    mEquipItem = 0x10A;
    field_0x33dc = 0.0f;
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(6), &mHeldItemRootPos);
    mIronBallCenterPos = mHeldItemRootPos;
    mHookshotTopPos = mHeldItemRootPos;

    field_0x2f38.setOldPosP(&mHookshotTopPos, &mIronBallCenterPos);
    field_0x32d4 = 0;
    mpHookSound->init(&mHeldItemRootPos, 1);
}

void daAlink_c::resetWolfBallGrab() {
    if (mEquipItem == 0x10A) {
        seStartOnlyReverb(Z2SE_OBJ_POU_SOUL_BREAK);
        deleteEquipItem(FALSE, FALSE);

        if (checkWolfGrabAnimeObj()) {
            resetUpperAnime(UPPER_2, 3.0f);
        }

        dComIfGp_particle_setColor(0x86CE, &mHeldItemRootPos, &tevStr, NULL, NULL, 0.0f, 0xFF);
        dComIfGp_particle_setColor(0x86CF, &mHeldItemRootPos, &tevStr, NULL, NULL, 0.0f, 0xFF);
        onEndResetFlg0(ERFLG0_UNK_20000000);
        offNoResetFlg3(FLG3_UNK_200000);
    }
}

void daAlink_c::checkWolfLockData() {
    for (int i = 0; i < 10; i++) {
        mWolfLockAcKeep[i].setActor();
    }

    for (int i = 0; i < mWolfLockNum; i++) {
        if (mWolfLockAcKeep[i].getActor() == NULL) {
            int var_r29;
            for (var_r29 = i; var_r29 < mWolfLockNum - 1; var_r29++) {
                mWolfLockAcKeep[var_r29].setData(mWolfLockAcKeep[var_r29 + 1].getActor());
            }

            mWolfLockAcKeep[var_r29].clearData();
            i--;
            mWolfLockNum--;
        }
    }
}

BOOL daAlink_c::checkWolfLock(fopAc_ac_c* i_actor) const {
    fpc_ProcID actor_id = fopAcM_GetID(i_actor);

    for (int i = 0; i < mWolfLockNum; i++) {
        if (actor_id == mWolfLockAcKeep[i].getID()) {
            return true;
        }
    }
    return false;
}

bool daAlink_c::cancelWolfLock(fopAc_ac_c* i_actor) {
    fpc_ProcID actor_id = fopAcM_GetID(i_actor);

    for (int i = 0; i < mWolfLockNum; i++) {
        if (actor_id == mWolfLockAcKeep[i].getID()) {
            mWolfLockAcKeep[i].clearData();
            return true;
        }
    }
    return false;
}

fopAc_ac_c* daAlink_c::getWolfLockActorEnd() {
    return mWolfLockAcKeep[0].getActor();
}

void daAlink_c::searchWolfLockEnemy(fopAc_ac_c* i_actor, void* i_data) {
    fopAc_ac_c** lock_actor_pp = (fopAc_ac_c**)i_data;

    for (int i = 0; i < mWolfLockNum; i++) {
        if (mWolfLockAcKeep[i].getActor() == i_actor) {
            return;
        }
    }

    fopEn_enemy_c* enemy_p = static_cast<fopEn_enemy_c*>(i_actor);
    if (fopAcM_GetGroup(enemy_p) == 2 && enemy_p->attention_info.flags & (fopAc_AttnFlag_BATTLE_e | fopAc_AttnFlag_LOCK_e) &&
        !enemy_p->checkWolfNoLock())
    {
        f32 dist_to_enemy = current.pos.abs2(enemy_p->eyePos);

        if (enemy_p->eyePos.y >= current.pos.y - 50.0f && dist_to_enemy < field_0x3478) {
            field_0x3478 = dist_to_enemy;
            *lock_actor_pp = i_actor;
        }
    }
}

static int daAlink_searchWolfLockEnemy(fopAc_ac_c* i_actor, void* i_data) {
    daAlink_getAlinkActorClass()->searchWolfLockEnemy(i_actor, i_data);
    return 0;
}

void daAlink_c::checkWolfComboCnt() {
    if (!dComIfGp_checkPlayerStatus1(0, 0x1000000)) {
        if (mComboCutCount == 2 && checkAttentionLock() && mAtCyl.ChkAtHit()) {
            onNoResetFlg0(FLG0_UNK_8000);
        }

        if (field_0x307e > 0) {
            field_0x307e--;
        } else {
            resetCombo(1);
        }
    }
}

BOOL daAlink_c::checkWolfAttackAction() {
    static int const normalType0[] = {3, 3, 3, 0, 0};
    static int const normalType1[] = {0, 0, 0, 3, 3};

    if (mComboCutCount == 4) {
        resetCombo(1);
    }

    mComboCutCount++;
    int atk_direction = getCutDirection();

    if (checkWolfDashMode()) {
        mComboCutCount = 4;
        return procWolfJumpAttackInit(0);
    } else if (checkNoResetFlg0(FLG0_UNK_8000)) {
        if (mComboCutCount == 4) {
            if (atk_direction == DIR_NONE) {
                return procWolfWaitAttackInit(2);
            } else if (atk_direction == DIR_FORWARD || atk_direction == DIR_LEFT) {
                return procWolfJumpAttackInit(0);
            } else {
                return procWolfRollAttackInit(1, 0);
            }
        } else if (atk_direction == DIR_RIGHT || atk_direction == DIR_BACKWARD) {
            return procWolfWaitAttackInit(2);
        } else {
            return procWolfJumpAttackInit(2);
        }
    } else if (mComboCutCount == 4) {
        if (mTargetedActor == NULL) {
            if (atk_direction == DIR_LEFT || atk_direction == DIR_NONE) {
                return procWolfWaitAttackInit(2);
            } else {
                return procWolfJumpAttackInit(0);
            }
        } else if (atk_direction == DIR_LEFT) {
            return procWolfRollAttackInit(0, 0);
        } else if (atk_direction == DIR_RIGHT) {
            return procWolfRollAttackInit(1, 0);
        } else if (atk_direction == DIR_FORWARD) {
            return procWolfJumpAttackInit(0);
        } else {
            return procWolfJumpAttackInit(2);
        }
    } else if (mComboCutCount == 2) {
        return procWolfWaitAttackInit(normalType0[atk_direction]);
    } else if (mComboCutCount == 1 && atk_direction == DIR_FORWARD) {
        return procWolfJumpAttackInit(0);
    } else {
        return procWolfWaitAttackInit(normalType1[atk_direction]);
    }
}

void daAlink_c::setWolfEnemyThrowUpperAnime(daAlink_c::daAlink_WANM i_anmID, f32 param_1) {
    setUpperAnime(
        i_anmID == WANM_FLING_LEFT ? (u16)0x2BD : (u16)0x2BE,
        UPPER_2,
        // Fakematch, should be
        // mpHIO->mWolf.mWlAttack.mWlAtBite.m.mEnemyBiteThrowAnmSpeed,
        daAlinkHIO_wlAtBite_c0::m.mEnemyBiteThrowAnmSpeed,
        param_1,
        -1,
        -1.0f
    );
}

BOOL daAlink_c::setWolfEnemyHangBitePos(fopEn_enemy_c* i_enemy) {
    if (i_enemy != NULL) {
        cXyz old_pos = current.pos = i_enemy->getDownPos();

        current.pos.x -= 60.0f * cM_ssin(shape_angle.y);
        current.pos.y -= 135.0f;
        current.pos.z -= 60.0f * cM_scos(shape_angle.y);

        if (commonLineCheck(&field_0x3540, &current.pos)) {
            current.pos = old_pos;
            return 0;
        } else {
            return 1;
        }
    }

    return 0;
}

static cXyz l_wolfBiteHitMarkScale(0.5f, 0.5f, 0.5f);

void daAlink_c::setWolfBiteDamage(fopEn_enemy_c* i_enemy) {
    i_enemy->onWolfBiteDamage();

    cXyz pos;
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(13), &pos);
    dComIfGp_setHitMark(1, i_enemy, &pos, &shape_angle, (cXyz*)&l_wolfBiteHitMarkScale, 0);
    dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
}

BOOL daAlink_c::checkWolfLockAttackChargeState() {
    return checkMidnaRide() && checkMidnaChargeAttack() && !daMidna_c::checkMidnaTired() &&
           daMidna_c::checkMidnaRealBody();
}

int daAlink_c::procWolfRollAttackChargeInit() {
    commonProcInit(PROC_WOLF_ROLL_ATTACK_CHARGE);
    setSingleAnimeWolfBaseSpeed(WANM_CUT_TURN_CHARGE, 0.0f,
                                mpHIO->mWolf.mWlAttack.m.mReadyInterpolation);
    voiceStartLevel(Z2SE_WL_V_ROAR);
    return 1;
}

int daAlink_c::procWolfRollAttackCharge() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    if (!itemButtonCheck(BTN_B)) {
        checkWolfAttackAction();
    } else if (field_0x2060->getOldFrameRate() < 0.1f) {
        procWolfRollAttackMoveInit();
    } else {
        cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mWolf.mWlMoveNoP.m.mDeceleration);
        setShapeAngleToAtnActor(0);
        current.angle.y = shape_angle.y;
        voiceStartLevel(Z2SE_WL_V_ROAR);
    }

    return 1;
}

int daAlink_c::procWolfRollAttackMoveInit() {
    commonProcInit(PROC_WOLF_ROLL_ATTACK_MOVE);
    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    mProcVar0.field_0x3008 = mpHIO->mWolf.mWlAttack.m.mChargeTime;
    mProcVar1.field_0x300a = mpHIO->mWolf.mWlAttack.mWlAtLock.m.mAutoActivationTime;
    mProcVar5.field_0x3012 = 30;
    field_0x3198 = 0;
    field_0x2fe4 = shape_angle.y;
    voiceStartLevel(Z2SE_WL_V_ROAR);
    dComIfGp_setPlayerStatus0(0, 0x40000000);
    return 1;
}

int daAlink_c::procWolfRollAttackMove() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    if (!swordButton() || mProcVar1.field_0x300a == 0) {
        if (mTargetedActor != NULL) {
            shape_angle.y = cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos);
            current.angle.y = shape_angle.y;
        }

        if (mProcVar0.field_0x3008 == 0) {
            if (mWolfLockNum != 0) {
                resetCombo(1);
                return procWolfLockAttackInit(0);
            } else {
                return procWolfRollAttackInit(1, 0);
            }
        }

        return checkWolfAttackAction();
    }

    if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == 0 && checkWolfLockAttackChargeState()) {
            setWolfLockDomeModel();
        }
    } else {
        onResetFlg0(RFLG0_UNK_10);

        if (mEquipItem == 0x109) {
            seStartOnlyReverbLevel(Z2SE_MIDNA_BIND_AREA_SUS);
            mProcVar1.field_0x300a--;

            if (mSearchBallScale < mpHIO->mWolf.mWlAttack.mWlAtLock.m.mMaxRadius &&
                cLib_chaseF(&mSearchBallScale, mpHIO->mWolf.mWlAttack.mWlAtLock.m.mMaxRadius,
                            mpHIO->mWolf.mWlAttack.mWlAtLock.m.mRadiusAcceleration))
            {
                mItemVar0.field_0x3018 = 1;
            }

            if (mWolfLockNum < 10) {
                field_0x3478 = mSearchBallScale * mSearchBallScale;

                fopAc_ac_c* lock_actor_p = NULL;
                fopAcIt_Executor((fopAcIt_ExecutorFunc)daAlink_searchWolfLockEnemy, &lock_actor_p);

                if (lock_actor_p != NULL) {
                    for (int i = mWolfLockNum - 1; i >= 0; i--) {
                        mWolfLockAcKeep[i + 1].setData(mWolfLockAcKeep[i].getActor());
                    }

                    mWolfLockAcKeep[0].setData(lock_actor_p);
                    mWolfLockNum++;
                }
            }
        }
    }

    voiceStartLevel(Z2SE_WL_V_ROAR);

    BOOL var_r28 = 0;
    f32 anm_speed = getWolfLieMoveAnmSpeed();

    if (frameCtrl_p->getRate() > 0.0f) {
        frameCtrl_p->setRate(anm_speed);
    } else if (frameCtrl_p->getRate() < 0.0f) {
        frameCtrl_p->setRate(-anm_speed);
    } else {
        var_r28 = 1;
    }

    setWolfAtnMoveDirection();

    if (var_r28 == true || frameCtrl_p->checkPass(0.0f) || frameCtrl_p->checkPass(15.0f)) {
        if (checkInputOnR()) {
            if (field_0x2f98 != 1) {
                frameCtrl_p->setRate(anm_speed);
                frameCtrl_p->setLoop(0);
            } else {
                frameCtrl_p->setRate(-anm_speed);
                frameCtrl_p->setLoop(frameCtrl_p->getEnd());
            }

            initBasAnime();
        } else if (!var_r28) {
            stopHalfMoveAnime(15.0f);
        }
    }

    setSpeedAndAngleWolfAtn();
    mNormalSpeed = getWolfLieMoveSpeed();
    if (mNormalSpeed < 0.0f) {
        mNormalSpeed *= -1.0f;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    } else {
        offModeFlg(1);
    }

    return 1;
}

int daAlink_c::procWolfJumpAttackInit(int param_0) {
    commonProcInit(PROC_WOLF_JUMP_ATTACK);
    mProcVar0.field_0x3008 = 0;

    f32 var_f26;
    f32 var_f31;
    f32 var_f27;
    f32 var_f28;
    f32 var_f30;
    f32 var_f29;
    
    if (param_0 == 1) {
        setCutType(0x39);
        setCylAtParam(0x80000000, dCcG_At_Spl_UNK_1, 3, 4, 3, mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mAttackRadius, mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mAttackHeight);
        field_0x594 = mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mMaxHorizontalSpeed;
        var_f26 = mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mMaxVerticalSpeed;

        if (mTargetedActor != NULL) {
            var_f31 = mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mMinVerticalSpeed;
            var_f28 = mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mAttackInitSpeed;
        } else {
            var_f31 = 25.0f;
            var_f28 = 26.0f;
        }

        var_f27 = mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mMinHorizontalSpeed;
        field_0x3438 = mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mAttackRadiusOffset;
        voiceStart(Z2SE_WL_V_ATTACK_L);
        seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_SCREW);

        if (!dComIfGs_isEventBit(0x520) && checkStageName("R_SP30")) {
            mProcVar0.field_0x3008 = 1;
        }

        if (mTargetedActor != NULL) {
            mProcVar5.field_0x3012 = 1;
        } else {
            mProcVar5.field_0x3012 = 0;
        }

        mProcVar4.field_0x3010 = 1;
    } else {
        if (mComboCutCount == 4) {
            setCutType(0x32);
            setCylAtParam(0x80000000, dCcG_At_Spl_UNK_1, 3, 4, 3, mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAttackRadius, mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAttackHeight);
            voiceStart(Z2SE_WL_V_ATTACK_L);
            seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_SCREW);
            mProcVar5.field_0x3012 = 1;
        } else {
            setCutType(0x31);
            setCylAtParam(0x80000000, dCcG_At_Spl_UNK_0, 1, 4, 2, mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAttackRadius, mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAttackHeight);
            voiceStart(Z2SE_WL_V_ATTACK_S);
            seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_S);
            mProcVar5.field_0x3012 = 0;
        }

        field_0x594 = mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mMaxHorizontalSpeed;
        var_f27 = mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mMinHorizontalSpeed;
        var_f26 = mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mMaxVerticalSpeed;
        var_f31 = mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mMinVerticalSpeed;
        field_0x3438 = mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAttackRadiusOffset;
        var_f28 = mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAttackInitSpeed;
        mProcVar4.field_0x3010 = 0;
    }

    setSingleAnimeWolfParam(WANM_ATTACK_A_START, &mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAerialAttackAnm);

    if (mTargetedActor != NULL) {
        shape_angle.y = cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos);

        if (mComboCutCount == 1 && param_0 != 1) {
            speed.y = var_f31;
            mNormalSpeed = (-gravity * eyePos.absXZ(mTargetedActor->eyePos)) / (2.0f * speed.y);
        } else {
            var_f30 = (mTargetedActor->eyePos.y - eyePos.y) - 10.0f;

            if (mProcVar0.field_0x3008 != 0) {
                var_f30 -= 10.0f;
            }
    
            if (var_f30 > 0.0f) {
                var_f29 = JMAFastSqrt((2.0f * var_f30) / -gravity);
            } else {
                var_f29 = 0.0f;
            }

            if (var_f29 >= 1.0f) {
                f32 temp_f25 = 1.0f / var_f29;
                mNormalSpeed = temp_f25 * current.pos.absXZ(mTargetedActor->eyePos);
                speed.y = cLib_minMaxLimit<f32>(temp_f25 * (var_f30 - (var_f29 * (0.5f * gravity * var_f29))), var_f31, var_f26);
            } else {
                mNormalSpeed = var_f28;
                speed.y = var_f31;
            }
        }
    } else {
        mNormalSpeed = var_f28;
        speed.y = var_f31;
    }

    if (mNormalSpeed > field_0x594) {
        mNormalSpeed = field_0x594;
    } else if (mNormalSpeed < var_f27) {
        mNormalSpeed = var_f27;
    }

    mProcVar1.field_0x300a = param_0;
    field_0x3588 = l_wolfBaseAnime;
    field_0x33b0 = l_wolfBaseAnime.y;
    field_0x3198 = 0;
    field_0x32cc = 0;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;

    onResetFlg0(RFLG0_UNK_2);
    dComIfGp_setPlayerStatus0(0, 2);

    if (param_0 != 0) {
        dComIfGp_setPlayerStatus1(0, 0x200000);
    }

    current.angle.y = shape_angle.y;

    if (param_0 != 1) {
        field_0x307e = mpHIO->mWolf.mWlAttack.m.mComboDuration;
    }

    return 1;
}

int daAlink_c::procWolfJumpAttack() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar3.field_0x300e != 0 && mNormalSpeed > mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mHitMinSpeed) {
        cLib_chaseF(&mNormalSpeed, mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mHitMinSpeed,
                    mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mHitDecel);
    }

    if (mAtCyl.ChkAtHit()) {
        field_0x3198 = 1;

        if (mProcVar4.field_0x3010 != 0 && mProcVar3.field_0x300e == 0) {
            mNormalSpeed *= mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mHitDecelRate;
            mProcVar3.field_0x300e = 1;
        }
    }

    if (checkWolfAttackReverse(field_0x32cc)) {
        return 1;
    }

    if (mLinkAcch.ChkGroundHit() && checkWolfLandAction(0)) {
        return 1;
    }

    if (mLinkAcch.ChkGroundHit() && checkSlideAction()) {
        dComIfGp_getVibration().StartShock(1, 15, cXyz(0.0f, 1.0f, 0.0f));
        return 1;
    }

    field_0x32cc = 1;

    if (mProcVar1.field_0x300a != 1) {
        field_0x307e = mpHIO->mWolf.mWlAttack.m.mComboDuration;
    }

    if (mProcVar0.field_0x3008 != 0 && checkFrontWallTypeAction()) {
        return 1;
    }

    field_0x2f99 = 7;

    if (mLinkAcch.ChkGroundHit()) {
        dComIfGp_getVibration().StartShock(1, 15, cXyz(0.0f, 1.0f, 0.0f));

        if (checkNoResetFlg2(FLG2_UNK_8)) {
            procWolfEnemyThrowInit(4);
        } else if (mProcVar5.field_0x3012 != 0 || mProcVar1.field_0x300a == 2 ||
                   mProcVar1.field_0x300a == 3)
        {
            procWolfJumpAttackSlideLandInit(field_0x3198, 1, mProcVar1.field_0x300a == 2);
        } else {
            procWolfJumpAttackNormalLandInit(0);
        }
        return 1;
    }

    if (checkNoResetFlg2(FLG2_WOLF_ENEMY_HANG_BITE)) {
        return procWolfEnemyHangBiteInit();
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar2.field_0x300c == 0) {
            mProcVar2.field_0x300c = 1;
            setSingleAnimeWolfBaseSpeed(WANM_ATTACK_A, mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAerialAnmSpeed,
                                        mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAerialInterpolation);
            onResetFlg0(RFLG0_UNK_2);
        } else {
            onResetFlg0(RFLG0_UNK_2);
        }
    } else {
        onResetFlg0(RFLG0_UNK_2);

        if (current.pos.y < mLastJumpPos.y - mpHIO->mWolf.mWlAttack.m.mFallHeight) {
            procWolfFallInit(2, mpHIO->mWolf.mWlAttack.m.mFallInterpolation);
        }
    }

    return 1;
}

int daAlink_c::procWolfJumpAttackKickInit() {
    commonProcInit(PROC_WOLF_JUMP_AT_KICK);
    setSingleAnimeWolfParam(WANM_JUMP_KICK, &mpHIO->mWolf.mWlAttack.mWlAtCjump.m.mFlipKickAnm);

    f32 tmp_0 = 0.0f;
    speed.y = tmp_0;
    mNormalSpeed = tmp_0;
    setSpecialGravity(tmp_0, maxFallSpeed, 0);

    voiceStart(Z2SE_WL_V_ATTACK_S);
    seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_S);
    dComIfGp_setPlayerStatus1(0, 0x200000);
    return 1;
}

int daAlink_c::procWolfJumpAttackKick() {
    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        procWolfBackJumpInit(2);
        dComIfGp_setPlayerStatus1(0, 0x200000);
    }

    return 1;
}

int daAlink_c::procWolfJumpAttackSlideLandInit(int param_0, int param_1, int param_2) {
    commonProcInit(PROC_WOLF_JUMP_AT_SLIDE_LAND);

    if (mTargetedActor == NULL || !param_0 ||
        (fopAcM_GetGroup(mTargetedActor) == 2 &&
         static_cast<fopEn_enemy_c*>(mTargetedActor)->checkDeadFlg()))
    {
        mProcVar4.field_0x3010 = 0;
    } else {
        mProcVar4.field_0x3010 = 1;
    }

    if ((mTargetedActor != NULL && fopAcM_GetGroup(mTargetedActor) == 2 && param_0 &&
         !static_cast<fopEn_enemy_c*>(mTargetedActor)->checkDeadFlg() &&
         fopAcM_seenActorAngleY(this, mTargetedActor) > 0x5000) ||
        param_2)
    {
        setSingleAnimeWolfParam(WANM_ATTACK_A_END_BACK, &mpHIO->mWolf.mWlAttack.mWlAtLand.m.mBackSlideAnm);
        mProcVar2.field_0x300c = 1;
        field_0x3588.set(-l_wolfBaseAnime.x, l_wolfBaseAnime.y, -l_wolfBaseAnime.z);
        field_0x3478 = mpHIO->mWolf.mWlAttack.mWlAtLand.m.mBackSlideAnm.mCancelFrame;
    } else {
        setSingleAnimeWolfParam(WANM_ATTACK_A_END_FRONT, &mpHIO->mWolf.mWlAttack.mWlAtLand.m.mFrontSlideAnm);
        mProcVar2.field_0x300c = 0;
        field_0x3588 = l_wolfBaseAnime;
        field_0x3478 = mpHIO->mWolf.mWlAttack.mWlAtLand.m.mFrontSlideAnm.mCancelFrame;
    }

    field_0x2f9d = 4;
    setFootEffectProcType(1);
    mProcVar0.field_0x3008 = param_2;

    mNormalSpeed *= 0.5f;
    if (param_1) {
        field_0x2f99 = 4;
    }

    mProcVar5.field_0x3012 = 0;
    field_0x307e = mpHIO->mWolf.mWlAttack.m.mComboDuration;
    field_0x2f98 = 0;
    return 1;
}

int daAlink_c::procWolfJumpAttackSlideLand() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    field_0x2f99 = 4;
    cLib_addCalc(&mNormalSpeed, 0.0f, 0.3f, mpHIO->mWolf.mWlAttack.mWlAtLand.m.mSlideDeceleration, 0.5f);

    if (mProcVar4.field_0x3010 != 0) {
        setShapeAngleToAtnActor(0);
    }

    setComboReserb();
    checkCutTurnCharge();

    if (checkAnmEnd(frameCtrl_p)) {
        if (checkZeroSpeedF()) {
            current.angle.y = shape_angle.y;

            if (mProcVar0.field_0x3008 != 0) {
                procWolfJumpAttackInit(3);
            } else {
                checkNextActionWolf(0);
            }
        }
    } else if (frameCtrl_p->getFrame() > field_0x3478 && mNormalSpeed <= 5.0f) {
        s16 prev_angle = current.angle.y;
        current.angle.y = shape_angle.y;

        if (mProcVar0.field_0x3008 != 0) {
            procWolfJumpAttackInit(3);
        } else if (!checkNextActionWolf(1)) {
            current.angle.y = prev_angle;
        }
    } else if (mNormalSpeed > 5.0f) {
        field_0x2f9d = 4;
        seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
    }

    return 1;
}

int daAlink_c::procWolfJumpAttackNormalLandInit(int param_0) {
    commonProcInit(PROC_WOLF_JUMP_AT_NORMAL_LAND);
    setSingleAnimeWolfParam(WANM_ATTACK_A_END, &mpHIO->mWolf.mWlAttack.mWlAtLand.m.mNormalLandingAnm);

    setFootEffectProcType(2);
    field_0x2f9d = 0x18;
    mNormalSpeed = 0.0f;
    mProcVar5.field_0x3012 = 0;
    mProcVar2.field_0x300c = param_0;
    field_0x2f99 = 12;
    field_0x307e = mpHIO->mWolf.mWlAttack.m.mComboDuration;

    if (param_0 != 0) {
        dComIfGp_setPlayerStatus1(0, 0x200000);
    }

    return 1;
}

int daAlink_c::procWolfJumpAttackNormalLand() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    field_0x2f99 = 4;
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mWolf.mWlMoveNoP.m.mDeceleration);
    setComboReserb();
    checkCutTurnCharge();

    if (frameCtrl->checkAnmEnd()) {
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlAttack.mWlAtLand.m.mNormalLandingAnm.mCancelFrame) {
        if (mProcVar2.field_0x300c != 0) {
            shape_angle.y -= -0x8000;
            return procWolfJumpAttackInit(3);
        }

        if (checkNextActionWolf(1)) {
            return 1;
        }
    }

    return 1;
}

int daAlink_c::procWolfWaitAttackInit(int i_attackType) {
    struct daAlink_WCutParamTbl {
        /* 0x0 */ daAlink_c::daAlink_WANM m_anmID;
        /* 0x4 */ u8 m_cutType;
    };  // Size: 0x8
    
    static const daAlink_WCutParamTbl dataTabl[] = {
        {
            daAlink_c::WANM_ATTACK_B_LEFT,
            daAlink_c::CUT_TYPE_WOLF_B_LEFT,
        },
        {
            daAlink_c::WANM_ATTACK_B_FRONT,
            daAlink_c::CUT_TYPE_WOLF_B_FRONT,
        },
        {
            daAlink_c::WANM_ATTACK_B_TAIL,
            daAlink_c::CUT_TYPE_WOLF_B_BACK,
        },
        {
            daAlink_c::WANM_ATTACK_B_RIGHT,
            daAlink_c::CUT_TYPE_WOLF_B_RIGHT,
        },
    };

    const daAlinkHIO_wlAtWait_c1* atk_hio;
    const daAlink_WCutParamTbl* atk_params = &dataTabl[i_attackType];

    commonProcInit(PROC_WOLF_WAIT_ATTACK);
    setCutType(atk_params->m_cutType);
    mProcVar2.field_0x300c = 0;

    if (i_attackType == 2) {
        atk_hio = &mpHIO->mWolf.mWlAttack.mTailAttack.m;
        mProcVar3.field_0x300e = 0;
    } else if (i_attackType == 1) {
        atk_hio = &mpHIO->mWolf.mWlAttack.mScratchAttack.m;
        mProcVar3.field_0x300e = 1;
    } else {
        atk_hio = &mpHIO->mWolf.mWlAttack.mHorizontalAttack.m;
        mProcVar3.field_0x300e = 1;
    }

    f32 var_f2;
    if (mComboCutCount == 4) {
        setCylAtParam(0x80000000, dCcG_At_Spl_UNK_1, 3, 4, 3, atk_hio->mAttackRadius,
                      atk_hio->mAttackHeight);
        field_0x3478 = atk_hio->mAttackAnm.mCancelFrame;
        mProcVar0.field_0x3008 = atk_hio->mStopTime;
        var_f2 = atk_hio->mAttackAnm.mStartFrame;
    } else {
        setCylAtParam(0x80000000, dCcG_At_Spl_UNK_0, 1, 4, 2, atk_hio->mAttackRadius,
                      atk_hio->mAttackHeight);
        field_0x3478 = atk_hio->mComboMidCF;
        mProcVar0.field_0x3008 = atk_hio->mComboMidStopTime;
        var_f2 = atk_hio->mComboMidStartF;
    }

    setSingleAnimeWolf(atk_params->m_anmID, atk_hio->mAttackAnm.mSpeed, var_f2,
                       atk_hio->mAttackAnm.mEndFrame, atk_hio->mAttackAnm.mInterpolation);

    if (mTargetedActor != NULL) {
        shape_angle.y = cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos);
    }

    current.angle.y = shape_angle.y;
    mProcVar5.field_0x3012 = 0;
    mProcVar1.field_0x300a = 2;
    field_0x347c = atk_hio->mAttackJudgmentStartForward;
    field_0x3480 = atk_hio->mAttackJudgmentEndForward;
    field_0x3484 = atk_hio->mSpeedAddForward;
    field_0x3488 = atk_hio->mSpeed;
    field_0x3438 = atk_hio->mAttackRadiusOffset;
    field_0x307e = mpHIO->mWolf.mWlAttack.m.mComboDuration;
    field_0x3588 = l_wolfBaseAnime;

    if (mComboCutCount != 1) {
        field_0x2f99 = 13;
    }

    return 1;
}

int daAlink_c::procWolfWaitAttack() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    field_0x2f99 = 5;
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mWolf.mWlMoveNoP.m.mDeceleration);
    setComboReserb();
    checkCutTurnCharge();

    if (mCutType != CUT_TYPE_WOLF_B_BACK && checkWolfAttackReverse(mProcVar1.field_0x300a == 0)) {
        return 1;
    }

    if (checkNoResetFlg2(FLG2_UNK_8) && frameCtrl_p->getFrame() >= 14.0f) {
        return procWolfEnemyThrowInit(mCutType == CUT_TYPE_WOLF_B_LEFT ? 2 : 3);
    }

    if (checkAnmEnd(frameCtrl_p)) {
        resetCombo(1);

        if (mDemo.getDemoMode() == 0x2B) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else if (mProcVar0.field_0x3008 > 0) {
            if (!(frameCtrl_p->getFrame() > field_0x3478) || !checkNextActionWolf(1)) {
                mProcVar0.field_0x3008--;
            }
        } else {
            mNormalSpeed = 0.0f;
            checkNextActionWolf(0);
        }
    } else if (frameCtrl_p->getFrame() > field_0x3478) {
        if (!checkNextActionWolf(1)) {
            resetCombo(1);
        }
    } else {
        setShapeAngleToAtnActor(1);
        current.angle.y = shape_angle.y;

        if (frameCtrl_p->checkPass(field_0x3484)) {
            mNormalSpeed = field_0x3488;
        }

        if (mProcVar2.field_0x300c == 0 && frameCtrl_p->getFrame() >= 3.0f) {
            if (mComboCutCount == 4) {
                voiceStart(Z2SE_WL_V_ATTACK_L);
            } else {
                voiceStart(Z2SE_WL_V_ATTACK_S);
            }

            if (mProcVar3.field_0x300e == 0) {
                seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_TAIL);
            } else {
                seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_S);
            }

            mProcVar2.field_0x300c = 1;
        }

        if (frameCtrl_p->getFrame() >= field_0x347c && frameCtrl_p->getFrame() < field_0x3480) {
            onResetFlg0(RFLG0_UNK_2);

            if (mProcVar1.field_0x300a != 0) {
                mProcVar1.field_0x300a--;
            }
        }
    }

    return 1;
}

int daAlink_c::procWolfRollAttackInit(int param_0, int param_1) {
    commonProcInit(PROC_WOLF_ROLL_ATTACK);

    if (param_0 == 2) {
        param_0 = getCutTurnDirection();
    }

    daAlink_WANM anm;
    if (param_0 == 1) {
        anm = WANM_CUT_TURN_RIGHT;
        mProcVar2.field_0x300c = 1;
        setCutType(CUT_TYPE_WOLF_TURN_RIGHT);
    } else {
        anm = WANM_CUT_TURN_LEFT;
        mProcVar2.field_0x300c = 0;
        setCutType(CUT_TYPE_WOLF_TURN_LEFT);
    }

    setSingleAnimeWolfParam(anm, &mpHIO->mWolf.mWlAttack.mAtRoll.m.mAttackAnm);
    field_0x3478 = mpHIO->mWolf.mWlAttack.mAtRoll.m.mAttackRadius;
    setCylAtParam(0x40000000, dCcG_At_Spl_UNK_1, 3, 4, 3, field_0x3478 * 0.5f, 155.0f);
    mNormalSpeed = 0.0f;
    mProcVar3.field_0x300e = param_1;

    if (param_1 != 0) {
        mUnderFrameCtrl[0].setStart(mpHIO->mWolf.mWlAttack.mAtRoll.m.mSpiderThreadEscapeStartF);
    } else {
        seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_L);
        voiceStart(Z2SE_WL_V_ATTACK_SPIN);
    }

    current.angle.y = shape_angle.y;
    field_0x3180 = 0;

    if (getMidnaActor() != NULL) {
        // NOTE: this call does nothing
        getMidnaActor()->resetRatBody();
    }

    return 1;
}

int daAlink_c::procWolfRollAttack() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mWolf.mWlMoveNoP.m.mDeceleration);

    if (checkAnmEnd(frameCtrl_p)) {
        checkNextActionWolf(0);
    } else if (frameCtrl_p->getFrame() > mpHIO->mWolf.mWlAttack.mAtRoll.m.mAttackAnm.mCancelFrame) {
        checkNextActionWolf(1);
    } else if (mProcVar3.field_0x300e == 0 && frameCtrl_p->getFrame() >= 4.0f &&
               frameCtrl_p->getFrame() < 13.0f)
    {
        if (!checkNoResetFlg0(FLG0_UNK_40)) {
            mNormalSpeed = mpHIO->mWolf.mWlAttack.mAtRoll.m.mAttackSpeed;
        }

        onResetFlg0(RFLG0_UNK_2);
        cLib_chaseF(mAtCyl.GetRP(), field_0x3478, 20.0f);
    }

    return 1;
}

int daAlink_c::procWolfDownAttackInit() {
    commonProcInit(PROC_WOLF_DOWN_ATTACK);
    setSingleAnimeWolfParam(WANM_ATTACK_POE_START, &mpHIO->mWolf.mWlAttack.mWlAtDown.m.mLowStanceAnm);

    cXyz sp24(static_cast<fopEn_enemy_c*>(mTargetedActor)->getDownPos());
    sp24.y += 50.0f;

    if (fopAcM_gc_c::gndCheck(&sp24)) {
        sp24.y = fopAcM_gc_c::getGroundY();
    } else {
        sp24.y -= 50.0f;
    }

    current.angle.y = cLib_targetAngleY(&current.pos, &sp24);

    sp24.x -= cM_ssin(current.angle.y) * 80.0f;
    sp24.z -= cM_scos(current.angle.y) * 80.0f;

    f32 var_f1 = current.pos.absXZ(sp24);
    if (var_f1 > 800.0f) {
        var_f1 = 800.0f;
    }

    speed.y = mpHIO->mWolf.mWlAttack.mWlAtDown.m.mSideRollMovementWidth;

    f32 temp_f3 = speed.y + gravity;
    f32 var_f0 = temp_f3 * temp_f3 + gravity * 2.0f * (sp24.y - current.pos.y);
    if (var_f0 < 0.0f) {
        mNormalSpeed = (var_f1 * gravity) / -temp_f3;
    } else {
        mNormalSpeed = (var_f1 * gravity) / (-JMAFastSqrt(var_f0) - temp_f3);
    }

    mProcVar2.field_0x300c = 0;
    field_0x280c.setData(mTargetedActor);
    voiceStart(Z2SE_WL_V_ATTACK_POU_ST);
    seStartOnlyReverb(Z2SE_WOLFATTACK_WIND_S);
    return 1;
}

int daAlink_c::procWolfDownAttack() {
    if (mLinkAcch.ChkGroundHit() && speed.y <= 0.0f) {
        fopEn_enemy_c* enemy_p = (fopEn_enemy_c*)field_0x280c.getActor();

        if (enemy_p != NULL && enemy_p->checkDownFlg() &&
            eyePos.abs2XZ(enemy_p->getDownPos()) < 10000.0f &&
            fabsf(enemy_p->current.pos.y - current.pos.y) < 50.0f)
        {
            procWolfDownAtLandInit(enemy_p);
        } else {
            procWolfDownAtMissLandInit();
        }
    } else if (checkAnmEnd(mUnderFrameCtrl)) {
        if (checkUnderMove0BckNoArcWolf(WANM_ATTACK_POE_START)) {
            setSingleAnimeWolfParam(WANM_ATTACK_POE, &mpHIO->mWolf.mWlAttack.mWlAtDown.m.mPushDownAnm);
            shape_angle.y = current.angle.y;
        }
    } else if (checkUnderMove0BckNoArcWolf(WANM_ATTACK_POE_START)) {
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 2, 0x2000, 0x800);
    }

    return 1;
}

int daAlink_c::procWolfDownAtLandInit(fopEn_enemy_c* i_enemy) {
    commonProcInit(PROC_WOLF_DOWN_AT_LAND);
    field_0x280c.setData(i_enemy);
    setSingleAnimeWolfParam(WANM_ATTACK_POE_END, &mpHIO->mWolf.mWlAttack.mWlAtDown.m.mFrontRollAnm);

    field_0x2f9d = 4;
    setFootEffectProcType(2);
    i_enemy->onWolfDownStartFlg();
    mNormalSpeed = 0.0f;
    setWolfBallModel();

    voiceStart(Z2SE_WL_V_ATTACK_POU_BITE);
    dComIfGp_getVibration().StartShock(2, 15, cXyz(0.0f, 1.0f, 0.0f));
    return 1;
}

int daAlink_c::procWolfDownAtLand() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    fopEn_enemy_c* enemy_p = (fopEn_enemy_c*)field_0x280c.getActor();
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (checkAnmEnd(frameCtrl_p)) {
        if (checkUnderMove0BckNoArcWolf(WANM_PULL_OUT)) {
            setUpperAnime(0x2DA, UPPER_2, 0.0f, 20.0f, -1, -1.0f);

            if (enemy_p != NULL) {
                enemy_p->onWolfDownPullEndFlg();

                dComIfGp_particle_setColor(0x86CA, &enemy_p->getDownPos(), &tevStr, NULL, NULL,
                                           0.0f, 0xFF);
                dComIfGp_particle_setColor(0x86CB, &enemy_p->getDownPos(), &tevStr, NULL, NULL,
                                           0.0f, 0xFF);
                dComIfGp_particle_setColor(0x86CC, &enemy_p->getDownPos(), &tevStr, NULL, NULL,
                                           0.0f, 0xFF);
            }

            return procWolfBackJumpInit(1);
        }

        setSingleAnimeWolfParam(WANM_PULL_OUT, &mpHIO->mWolf.mWlAttack.mWlAtDown.m.mProneGetUpAnm);
        dComIfGp_getVibration().StartShock(2, 15, cXyz(0.0f, 1.0f, 0.0f));
    } else if (checkUnderMove0BckNoArcWolf(WANM_ATTACK_POE_END) &&
               (frameCtrl_p->checkPass(9.0f) || frameCtrl_p->checkPass(16.0f)))
    {
        dComIfGp_getVibration().StartShock(2, 15, cXyz(0.0f, 1.0f, 0.0f));
    }

    return 1;
}

int daAlink_c::procWolfDownAtMissLandInit() {
    commonProcInit(PROC_WOLF_DOWN_AT_MISS_LAND);
    setSingleAnimeWolfParam(WANM_ATTACK_POE_MISS, &mpHIO->mWolf.mWlAttack.mWlAtDown.m.mRollOverAnm);
    setFaceBasicTexture(FTANM_UNK_97);

    mNormalSpeed = 0.0f;
    setFootEffectProcType(1);
    field_0x2f9d = 0x18;
    field_0x3588 = l_wolfBaseAnime;

    dComIfGp_getVibration().StartShock(1, 15, cXyz(0.0f, 1.0f, 0.0f));
    return 1;
}

int daAlink_c::procWolfDownAtMissLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    field_0x2f99 = 4;

    if (frameCtrl->checkAnmEnd()) {
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlAttack.mWlAtDown.m.mRollOverAnm.mCancelFrame) {
        checkNextActionWolf(1);
    } else if (frameCtrl->getFrame() >= 3.0f && frameCtrl->getFrame() < 8.0f) {
        seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
        field_0x2f9d = 0x18;
    }

    return 1;
}

int daAlink_c::procWolfLockAttackInit(int param_0) {
    commonProcInit(PROC_WOLF_LOCK_ATTACK);
    setSingleAnimeWolfParam(WANM_ATTACK_A_START, &mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAerialAttackAnm);

    if (fopAcM_GetName(mWolfLockAcKeep[0].getActor()) == PROC_E_YMB) {
        mProcVar5.field_0x3012 = 1;

        if (param_0 == 0) {
            mProcVar1.field_0x300a =
                static_cast<daE_YMB_c*>(mWolfLockAcKeep[0].getActor())->getMaxLockAttack();
        }

        mProcVar1.field_0x300a--;
        field_0x37c8 = mWolfLockAcKeep[0].getActor()->eyePos;

        if (mProcVar1.field_0x300a == 0) {
            mWolfLockAcKeep[0].clearData();
        }
    } else {
        field_0x37c8 = mWolfLockAcKeep[0].getActor()->eyePos;
        mWolfLockAcKeep[0].clearData();
        mProcVar5.field_0x3012 = 0;
    }

    shape_angle.y = cLib_targetAngleY(&current.pos, &field_0x37c8);
    current.angle.y = shape_angle.y;

    cXyz l_eyePosDelta = field_0x37c8 - eyePos;

    if (l_eyePosDelta.y < 10.0f) {
        l_eyePosDelta.y = 10.0f;
    } else if (l_eyePosDelta.y > mpHIO->mWolf.mWlAttack.mWlAtLock.m.mMaxJumpHeight) {
        l_eyePosDelta.y = mpHIO->mWolf.mWlAttack.mWlAtLock.m.mMaxJumpHeight;
    }

    f32 var_f30 = l_eyePosDelta.absXZ();
    if (var_f30 > mpHIO->mWolf.mWlAttack.mWlAtLock.m.mMaxJumpDistance) {
        var_f30 = mpHIO->mWolf.mWlAttack.mWlAtLock.m.mMaxJumpDistance / var_f30;
        l_eyePosDelta.x *= var_f30;
        l_eyePosDelta.z *= var_f30;
    }

    f32 var_f29 = l_eyePosDelta.abs();
    f32 var_f31 = var_f29 / mpHIO->mWolf.mWlAttack.mWlAtLock.m.mAttackSpeed;
    if (var_f31 < 1.0f)
        var_f31 = 1.0f;

    f32 var_f28 = mpHIO->mWolf.mWlAttack.mWlAtLock.m.mAttackSpeed / var_f29;
    mNormalSpeed = var_f28 * l_eyePosDelta.absXZ();
    setSpecialGravity((l_eyePosDelta.y * -2.0f) / (var_f31 * var_f31), maxFallSpeed, 0);
    speed.y = -gravity * var_f31;
    mProcVar0.field_0x3008 = var_f31;

    setCylAtParam(0x8000000, dCcG_At_Spl_UNK_0, 1, 4, 6, mpHIO->mWolf.mWlAttack.mWlAtLock.m.mAttackRadius,
                  mpHIO->mWolf.mWlAttack.mWlAtLock.m.mAttackHeight);
    field_0x3438 = mpHIO->mWolf.mWlAttack.mWlAtLock.m.mAttackRadiusOffset;

    setCutType(CUT_TYPE_WOLF_LOCK);
    voiceStart(Z2SE_WL_V_ATTACK_THRUST);

    dComIfGp_setPlayerStatus1(0, 0x1000000);
    dComIfGp_particle_set(0x247, &current.pos, &current.angle, 0);
    return 1;
}

int daAlink_c::procWolfLockAttack() {
    BOOL var_r29 = false;

    // check if any field_0x850 clylinders collided with at shields
    for (int i = 0; i < 3; i++) {
        if (checkAtShieldHit(field_0x850[i])) {
            var_r29 = true;
            break;
        }
    }

    // early return checks for colliding with at shields or if the actor hit is not freezard
    if (var_r29 || (checkAtShieldHit(mAtCyl) && (mAtCyl.GetAtHitAc() == NULL ||
                                                 fopAcM_GetName(mAtCyl.GetAtHitAc()) != PROC_E_FB)))
    {
        return procCoLargeDamageInit(-4, 1, 0, 0, NULL, 2);
    }

    if (checkWolfAttackReverse(1)) {
        return 1;
    }

    if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == 0) {
            setSpecialGravity(mpHIO->mWolf.mWlAttack.mWlAtLock.m.mGravity, maxFallSpeed, 0);
        }
    } else if (mNormalSpeed > 30.0f) {
        cLib_addCalc(&mNormalSpeed, 30.0f, 0.3f, 5.0f, 1.0f);
    }

    if (checkAnmEnd(mUnderFrameCtrl) && checkUnderMove0BckNoArcWolf(WANM_ATTACK_A_START)) {
        setSingleAnimeWolfBaseSpeed(WANM_ATTACK_A, mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAerialAnmSpeed,
                                    mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAerialInterpolation);
    }

    if (mLinkAcch.ChkGroundHit() || (mProcVar5.field_0x3012 != 0 && current.pos.y < mWaterY)) {
        if (!mLinkAcch.ChkGroundHit()) {
            current.pos.y = mWaterY;
        }

        if (mWolfLockNum != 0) {
            procWolfLockAttackTurnInit(mProcVar1.field_0x300a);
        } else {
            if (mNormalSpeed > mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mMaxHorizontalSpeed) {
                mNormalSpeed = mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mMaxHorizontalSpeed;
            }

            procWolfJumpAttackSlideLandInit(0, 0, 0);
        }
    } else {
        mZ2Link.startLinkSoundLevel(Z2SE_WOLFATTACK_WIND_RUSH, mComboCutCount,
                                    mVoiceReverbIntensity);
        onResetFlg0(RFLG0_UNK_2);
    }

    return 1;
}

int daAlink_c::procWolfLockAttackTurnInit(int param_0) {
    commonProcInit(PROC_WOLF_LOCK_ATTACK_TURN);

    if (abs((s16)(cLib_targetAngleY(&current.pos, &mWolfLockAcKeep[0].getActor()->eyePos) -
                  shape_angle.y)) > 0x4000)
    {
        setSingleAnimeWolfParam(WANM_ATTACK_A_END_BACK, &mpHIO->mWolf.mWlAttack.mWlAtLock.m.mRearLandingAnm);
        mProcVar2.field_0x300c = 1;
        field_0x3588.set(-l_wolfBaseAnime.x, l_wolfBaseAnime.y, -l_wolfBaseAnime.z);
    } else {
        setSingleAnimeWolfParam(WANM_ATTACK_A_END_FRONT, &mpHIO->mWolf.mWlAttack.mWlAtLock.m.mFrontLandingAnm);
        mProcVar2.field_0x300c = 0;
        field_0x3588 = l_wolfBaseAnime;
    }

    if (mWolfLockAcKeep[0].getActor() != NULL &&
        fopAcM_GetName(mWolfLockAcKeep[0].getActor()) == PROC_E_YMB)
    {
        mProcVar5.field_0x3012 = 1;
    } else {
        mProcVar5.field_0x3012 = 0;
    }

    field_0x2f9d = 4;
    setFootEffectProcType(1);
    mNormalSpeed *= 0.5f;
    field_0x2f99 = 4;
    dComIfGp_setPlayerStatus1(0, 0x1000000);
    offResetFlg0(RFLG0_UNK_8000000);
    mComboCutCount++;
    mProcVar1.field_0x300a = param_0;
    return 1;
}

int daAlink_c::procWolfLockAttackTurn() {
    field_0x2f99 = 4;

    if (mProcVar5.field_0x3012 != 0 && current.pos.y < mWaterY) {
        current.pos.y = mWaterY;
    }

    if (mWolfLockNum != 0) {
        s16 target_angle = cLib_targetAngleY(&current.pos, &mWolfLockAcKeep[0].getActor()->eyePos);
        if (mProcVar2.field_0x300c != 0) {
            target_angle += 0x8000;
        }

        cLib_addCalcAngleS(&shape_angle.y, target_angle, 2, 0x2000, 0x800);
    }

    cLib_addCalc(&mNormalSpeed, 0.0f, 0.3f, 5.0f, 1.0f);

    if (checkAnmEnd(mUnderFrameCtrl)) {
        if (mWolfLockNum != 0) {
            procWolfLockAttackInit(mProcVar1.field_0x300a);
        } else if (checkZeroSpeedF()) {
            current.angle.y = shape_angle.y;
            checkNextActionWolf(0);
        } else {
            field_0x2f9d = 4;
            seStartMapInfoLevel(Z2SE_FN_WOLF_SLIP);
        }
    }

    return 1;
}

int daAlink_c::procWolfAttackReverseInit() {
    commonProcInit(PROC_WOLF_ATTACK_REVERSE);
    setSingleAnimeWolfBase(WANM_ATTACK_RECOIL_START);
    setFaceBasicTexture(FTANM_UNK_9A);

    mNormalSpeed = mpHIO->mWolf.mWlAttack.m.mJumpBackSpeedH;
    speed.y = mpHIO->mWolf.mWlAttack.m.mJumpBackSpeedV;
    current.angle.y += 0x8000;

    dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
    dKy_Sound_set(current.pos, 100, fopAcM_GetID(this), 5);
    seStartOnlyReverb(Z2SE_WOLF_BODYATTACK);

    onResetFlg1(RFLG1_WOLF_ATTACK_REVERSE);
    field_0x3588 = l_wolfBaseAnime;
    return 1;
}

int daAlink_c::procWolfAttackReverse() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    field_0x2f99 = 4;

    if (!checkModeFlg(2)) {
        if (frameCtrl_p->checkPass(30.0f)) {
            voiceStart(Z2SE_WL_V_NOSE);
        }

        if (checkAnmEnd(frameCtrl_p)) {
            checkNextActionWolf(0);
        } else if (frameCtrl_p->getFrame() > mpHIO->mWolf.mWlAttack.m.mJumpBackLandAnm.mCancelFrame) {
            checkNextActionWolf(1);
        }
    } else if ((mLinkAcch.ChkGroundHit() || checkEndResetFlg2(ERFLG2_UNK_100)) && checkModeFlg(2)) {
        if (checkWolfLandAction(0)) {
            return 1;
        }

        mNormalSpeed = 0.0f;
        setSingleAnimeWolfParam(WANM_ATTACK_RECOIL_END, &mpHIO->mWolf.mWlAttack.m.mJumpBackLandAnm);
        setFaceBasicTexture(FTANM_UNK_9B);
        current.angle.y = shape_angle.y;
        offModeFlg(2);
        onModeFlg(1);
    }

    return 1;
}

int daAlink_c::procWolfEnemyThrowInit(int param_0) {
    commonProcInit(PROC_WOLF_ENEMY_THROW);

    daAlink_WANM anm;
    if (param_0 == 2 || (param_0 == 4 && cM_rnd() < 0.5f)) {
        anm = WANM_FLING_LEFT;
        onNoResetFlg2(FLG2_WOLF_ENEMY_LEFT_THROW);
    } else {
        anm = WANM_FLING_RIGHT;
        offNoResetFlg2(FLG2_WOLF_ENEMY_LEFT_THROW);
    }

    f32 var_f31;
    if (param_0 == 4) {
        var_f31 = 0.0f;
    } else {
        var_f31 = 3.0f;
    }

    setSingleAnimeWolf(anm, mpHIO->mWolf.mWlAttack.mWlAtBite.m.mEnemyBiteThrowAnmSpeed, var_f31, -1,
                       mpHIO->mWolf.mWlAttack.mWlAtBite.m.mEnemyBiteThrowInterpolation);
    setWolfEnemyThrowUpperAnime(anm, var_f31);
    field_0x3588 = l_wolfBaseAnime;
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    field_0x2f99 = 13;
    current.angle.y = shape_angle.y;
    return 1;
}

int daAlink_c::procWolfEnemyThrow() {
    if (checkWolfGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    field_0x2f99 = 5;

    if (frameCtrl->checkAnmEnd()) {
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlAttack.mWlAtBite.m.mEnemyBiteThrowCF) {
        checkNextActionWolf(1);
    }

    return 1;
}

int daAlink_c::procWolfEnemyHangBiteInit() {
    commonProcInit(PROC_WOLF_ENEMY_HANG_BITE);
    setSingleAnimeWolfParam(WANM_CATCH_START, &mpHIO->mWolf.mWlAttack.mWlAtBite.m.mHangingBitePreparationAnm);

    f32 tmp_0 = 0.0f;
    mNormalSpeed = tmp_0;
    speed.y = tmp_0;
    setSpecialGravity(tmp_0, maxFallSpeed, 0);

    if (!setWolfEnemyHangBitePos((fopEn_enemy_c*)field_0x281c.getActor())) {
        return procWolfJumpAttackKickInit();
    } else {
        current.angle.y = shape_angle.y;
        field_0x2f99 = 0x50;
        mProcVar0.field_0x3008 = mpHIO->mWolf.mWlAttack.mWlAtBite.m.mBiteAttachTime;
        mProcVar1.field_0x300a = 0;
        mProcVar2.field_0x300c = 0;
        mProcVar3.field_0x300e = 0;
        field_0x3478 = mpHIO->mWolf.mWlAttack.mWlAtBite.m.mHangingBiteMidAnmSpeedMin;
        mProcVar4.field_0x3010 = shape_angle.y;
        voiceStart(Z2SE_WL_V_BITE_ATTACK);
        dComIfGp_setPlayerStatus1(0, 0x200000);
    }

    return 1;
}

int daAlink_c::procWolfEnemyHangBite() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    fopEn_enemy_c* enemy_p = (fopEn_enemy_c*)field_0x281c.getActor();
    if (!setWolfEnemyHangBitePos(enemy_p)) {
        return procWolfJumpAttackKickInit();
    }

    cLib_addCalcAngleS(&shape_angle.y, mProcVar4.field_0x3010, 2, 0x2000, 0x800);
    current.angle.y = shape_angle.y;
    setDoStatusContinuation(3);

    if (doTrigger()) {
        mProcVar3.field_0x300e = 1;
        mProcVar0.field_0x3008 = mpHIO->mWolf.mWlAttack.mWlAtBite.m.mBiteAttachTime;
    } else if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;
    }

    if (mProcVar2.field_0x300c == 0) {
        if (checkAnmEnd(frameCtrl_p)) {
            if (mProcVar3.field_0x300e != 0) {
                setSingleAnimeWolfBaseSpeed(WANM_CATCH, field_0x3478, 3.0f);
                mProcVar3.field_0x300e = 0;
                mProcVar2.field_0x300c = 1;
            } else if (mProcVar0.field_0x3008 == 0) {
                return procWolfJumpAttackKickInit();
            }
        }
    } else {
        if (mProcVar0.field_0x3008 == 0) {
            return procWolfJumpAttackKickInit();
        }

        if (doTrigger()) {
            cLib_chaseF(&field_0x3478, mpHIO->mWolf.mWlAttack.mWlAtBite.m.mHangingBiteMidAnmSpeedMax,
                        mpHIO->mWolf.mWlAttack.mWlAtBite.m.mHangingBiteMidAnmSpeedAcceleration);
        } else {
            cLib_chaseF(&field_0x3478, mpHIO->mWolf.mWlAttack.mWlAtBite.m.mHangingBiteMidAnmSpeedMin,
                        mpHIO->mWolf.mWlAttack.mWlAtBite.m.mHangingBiteMidAnmSpeedDeceleration);
        }

        if (frameCtrl_p->getRate() < 0.1f) {
            if (mProcVar3.field_0x300e != 0) {
                mProcVar3.field_0x300e = 0;
                frameCtrl_p->setRate(field_0x3478);
            }
        } else {
            frameCtrl_p->setRate(field_0x3478);

            if (frameCtrl_p->checkPass(0.0f) || frameCtrl_p->checkPass(17.0f)) {
                if (mProcVar3.field_0x300e == 0) {
                    if (frameCtrl_p->checkPass(0.0f)) {
                        frameCtrl_p->setFrame(0.0f);
                    } else {
                        frameCtrl_p->setFrame(17.0f);
                    }

                    getNowAnmPackUnder(UNDER_0)->setFrame(frameCtrl_p->getFrame());
                    frameCtrl_p->setRate(0.0f);
                } else {
                    mProcVar3.field_0x300e = 0;
                }
            } else if (frameCtrl_p->checkPass(14.0f) || frameCtrl_p->checkPass(36.0f)) {
                setWolfBiteDamage(enemy_p);
            } else if (frameCtrl_p->checkPass(7.5f) || frameCtrl_p->checkPass(30.0f)) {
                voiceStart(Z2SE_WL_V_BITE_ATTACK);
            } else if (frameCtrl_p->checkPass(20.0f)) {
                voiceStart(Z2SE_WL_V_BITE_THROAT);
            }
        }
    }

    return 1;
}

int daAlink_c::procWolfGrabUpInit() {
    if (fopAcM_checkCarryNow(field_0x27f4)) {
        return checkNextActionWolf(0);
    }

    if (fopAcM_GetName(field_0x27f4) == PROC_Obj_MasterSword) {
        setGrabItemActor(field_0x27f4);
        return 1;
    }

    commonProcInit(PROC_WOLF_GRAB_UP);

    f32 var_f31;
    if (!fopAcM_CheckCarryType(field_0x27f4, fopAcM_CARRY_ITEM)) {
        setSingleAnimeWolfParam(WANM_PICKUP_A, &mpHIO->mWolf.mWlGrab.m.mPotGrabAnm);
        var_f31 = 120.0f;
        field_0x3478 = 10.0f;
        field_0x347c = mpHIO->mWolf.mWlGrab.m.mPotGrabAnm.mCancelFrame;
    } else {
        setSingleAnimeWolfParam(WANM_PICKUP_B, &mpHIO->mWolf.mWlGrab.m.mBoneGrabAnm);
        var_f31 = 80.0f;
        field_0x3478 = 7.0f;
        field_0x347c = mpHIO->mWolf.mWlGrab.m.mBoneGrabAnm.mCancelFrame;
    }

    setGrabItemActor(field_0x27f4);
    shape_angle.y = fopAcM_searchActorAngleY(this, field_0x27f4);
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = shape_angle.y;
    field_0x37c8 = field_0x27f4->current.pos - field_0x3510;
    mProcVar3.field_0x300e = 0;

    if (fopAcM_GetName(field_0x27f4) != PROC_Obj_Drop &&
        fopAcM_GetName(field_0x27f4) != PROC_Obj_Sword)
    {
        f32 prev_x = current.pos.x;
        f32 prev_z = current.pos.z;

        current.pos.x = field_0x27f4->current.pos.x - var_f31 * cM_ssin(shape_angle.y);
        current.pos.z = field_0x27f4->current.pos.z - var_f31 * cM_scos(shape_angle.y);
        field_0x37c8.x -= current.pos.x - prev_x;
        field_0x37c8.z -= current.pos.z - prev_z;
    }

    field_0x33e4 = field_0x27f4->current.pos.absXZ(current.pos) - 100.0f;
    if (field_0x33e4 < 0.0f) {
        field_0x33e4 = 0.0f;
    }

    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procWolfGrabUp() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    fopAc_ac_c* item_p = mGrabItemAcKeep.getActor();

    if (item_p == NULL) {
        return checkNextActionWolf(0);
    }

    if (frameCtrl_p->checkPass(field_0x3478)) {
        onResetFlg0(RFLG0_UNK_8000);
        fopAcM_setStageLayer(item_p);
        voiceStart(Z2SE_WL_V_HOLD_MOUTH);
    }

    if (frameCtrl_p->getFrame() >= field_0x3478) {
        mProcVar3.field_0x300e = 1;
    }

    if (checkAnmEnd(frameCtrl_p)) {
        setUpperAnime(mUnderAnmHeap[0].getIdx(), UPPER_2, 0.0f, frameCtrl_p->getEnd() - 0.001f, -1,
                      -1.0f);
        checkNextActionWolf(0);
    } else if (frameCtrl_p->getFrame() > field_0x347c) {
        setUpperAnime(mUnderAnmHeap[0].getIdx(), UPPER_2, 0.0f, frameCtrl_p->getEnd() - 0.001f, -1,
                      -1.0f);

        if (!checkNextActionWolf(1)) {
            resetUpperAnime(UPPER_2, -1.0f);
        }
    }

    return 1;
}

int daAlink_c::procWolfGrabPutInit() {
    if (mProcID == PROC_WOLF_GRAB_PUT) {
        return 0;
    }

    BOOL grab_obj = checkWolfGrabAnimeObj();
    commonProcInit(PROC_WOLF_GRAB_PUT);

    if (grab_obj) {
        setSingleAnimeWolfParam(WANM_PICKUP_A, &mpHIO->mWolf.mWlGrab.m.mPotReleaseAnm);
        field_0x3478 = 10.0f;
        field_0x347c = mpHIO->mWolf.mWlGrab.m.mPotReleaseAnm.mCancelFrame;
    } else {
        setSingleAnimeWolfParam(WANM_PICKUP_B, &mpHIO->mWolf.mWlGrab.m.mBoneReleaseAnm);
        field_0x3478 = 7.0f;
        field_0x347c = mpHIO->mWolf.mWlGrab.m.mBoneReleaseAnm.mCancelFrame;
    }

    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procWolfGrabPut() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    fopAc_ac_c* actor_p = mGrabItemAcKeep.getActor();

    if (frameCtrl_p->getFrame() < field_0x3478) {
        if (actor_p != NULL) {
            actor_p->speedF = 0.0f;
            actor_p->current.pos.x += field_0x33e4 * cM_ssin(shape_angle.y);
            actor_p->current.pos.z += field_0x33e4 * cM_scos(shape_angle.y);
            freeGrabItem();
        }

        if (checkAnmEnd(frameCtrl_p)) {
            if (mDemo.getDemoMode() == 13) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            } else {
                checkNextActionWolf(0);
            }
        } else if (frameCtrl_p->getFrame() < field_0x347c) {
            checkNextActionWolf(1);
        }
    } else {
        if (actor_p == NULL) {
            if (mDemo.getDemoMode() == 13) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
                return 1;
            } else {
                return checkNextActionWolf(0);
            }
        }

        if (grabLineCheck(&field_0x34e0, &actor_p->current.pos) && dBgS_CheckBWallPoly(mObjLinChk))
        {
            cXyz sp8 = actor_p->current.pos - mObjLinChk.GetCross();
            current.pos.x -= sp8.x * 1.5f;
            current.pos.z -= sp8.z * 1.5f;
        }
    }

    return 1;
}

int daAlink_c::procWolfGrabThrowInit() {
    commonProcInit(PROC_WOLF_GRAB_THROW);
    setSingleAnimeWolfParam(WANM_PICKUP_THROW, &mpHIO->mWolf.mWlGrab.m.mThrowAnm);
    current.angle.y = shape_angle.y;

    return 1;
}

int daAlink_c::procWolfGrabThrow() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mWolf.mWlMoveNoP.m.mDeceleration);
    setShapeAngleToAtnActor(0);
    current.angle.y = shape_angle.y;

    if (frameCtrl->checkPass(10.0f)) {
        setGrabItemThrow();
    }

    if (frameCtrl->checkAnmEnd()) {
        checkNextActionWolf(0);
    } else if (frameCtrl->getFrame() > mpHIO->mWolf.mWlGrab.m.mThrowAnm.mCancelFrame) {
        checkNextActionWolf(1);
    }

    return 1;
}

int daAlink_c::procWolfPushInit() {
    commonProcInit(PROC_WOLF_PUSH);
    mProcVar3.field_0x300e = 18;

    s16 var_r5;
    if (fopAcM_CheckCarryType(field_0x27f4, fopAcM_CARRY_TYPE_1)) {
        mProcVar2.field_0x300c = 1;
        var_r5 = 24;
    } else {
        mProcVar2.field_0x300c = 0;
        var_r5 = mProcVar3.field_0x300e;
    }
    setSingleAnimeWolf(WANM_PUSH_LIGHT, 1.0f, 0.0f, var_r5, 3.0f);
    mUnderFrameCtrl[0].setAttribute(0);

    mNormalSpeed = 0.0f;
    shape_angle.y = fopAcM_searchActorAngleY(this, field_0x27f4);
    current.angle.y = shape_angle.y;
    field_0x3588 = l_wolfBaseAnime;
    field_0x2f99 = 12;
    field_0x34d4 = l_wolfBaseAnime;

    current.pos.x += cM_ssin(shape_angle.y) * 50.0f;
    current.pos.z += cM_scos(shape_angle.y) * 50.0f;

    if (fopAcM_CheckCarryType(field_0x27f4, fopAcM_CARRY_TYPE_1)) {
        mProcVar2.field_0x300c = 1;
    } else {
        mProcVar2.field_0x300c = 0;
    }

    field_0x280c.setData(field_0x27f4);
    return 1;
}

int daAlink_c::procWolfPush() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    field_0x2f99 = 4;

    if (frameCtrl->checkAnmEnd()) {
        if (mProcVar2.field_0x300c != 0) {
            setSingleAnimeWolf(WANM_PUSH_LIGHT, 1.0f, 0.0f, mProcVar3.field_0x300e, 3.0f);
            mUnderFrameCtrl[0].setAttribute(0);
            mProcVar2.field_0x300c = 0;
        } else {
            checkNextActionWolf(0);
        }
    } else if (mProcVar2.field_0x300c == 0 && frameCtrl->checkPass(17.5f)) {
        if (field_0x280c.getActor() != NULL) {
            fopAcM_setCarryNow(field_0x280c.getActor(), 0);
        }
    }

    return 1;
}

static fopAc_ac_c* daAlink_searchEnemyCargo(fopAc_ac_c* i_actor, void*) {
    if (fopAcM_GetName(i_actor) == PROC_E_YC) {
        return i_actor;
    }

    return NULL;
}

int daAlink_c::procWolfCargoCarryInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_CARGO_CARRY)) {
        return 1;
    }

    f32 tmp_0 = 0.0f;
    mNormalSpeed = tmp_0;
    speed.y = tmp_0;
    setSpecialGravity(tmp_0, maxFallSpeed, 0);

    daAlink_WANM anm;
    if (checkCargoCarry()) {
        mProcVar2.field_0x300c = 1;
        anm = WANM_WAIT_KARGOROK;
    } else {
        mProcVar2.field_0x300c = 0;

        fopAc_ac_c* cargo_actor =
            (fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchEnemyCargo, NULL);
        if (cargo_actor != NULL) {
            field_0x280c.setData(cargo_actor);
        }

        anm = WANM_CATCH_KARGOROK;
    }

    if (!dComIfGp_evmng_checkStartDemo()) {
        voiceStart(Z2SE_WL_V_CATCHED);
    }

    setSingleAnimeWolfBase(anm);

    return 1;
}

int daAlink_c::procWolfCargoCarry() {
    setJumpMode();

    if (mProcVar2.field_0x300c != 0) {
        mCargoCarryAcKeep.setActor();
        if (mCargoCarryAcKeep.getActor() == NULL || !checkCargoCarry()) {
            offCargoCarry();
            return checkNextActionWolf(0);
        }

        daKago_c* kago_p = (daKago_c*)mCargoCarryAcKeep.getActor();
        mDoMtx_stack_c::copy(kago_p->getLegR3Mtx());
        mDoMtx_stack_c::transM(-9.0f, -17.0f, -30.0f);
        mDoMtx_stack_c::multVecZero(&current.pos);
        mDoMtx_stack_c::ZXYrotM(-0x1928, 0x3FFF, 0);
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &shape_angle);
        current.angle.y = shape_angle.y;

        checkSubjectAction();
    } else {
        if (checkEndResetFlg1(ERFLG1_UNK_10000)) {
            return checkNextActionWolf(0);
        }

        e_yc_class* yc_p = (e_yc_class*)field_0x280c.getActor();
        mDoMtx_stack_c::copy(yc_p->getLegR3Mtx());
        mDoMtx_stack_c::transM(-9.0f, -7.0f, -30.0f);
        mDoMtx_stack_c::multVecZero(&current.pos);
        mDoMtx_stack_c::ZXYrotM(0x770C, 0, -0x4C94);
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &shape_angle);
        current.angle.y = shape_angle.y;

        voiceStartLevel(Z2SE_WL_V_CATCHED_LOOP);
    }

    return 1;
}

int daAlink_c::procWolfChainUpInit() {
    field_0x2fa3 = searchFmChainPos();

    if (field_0x2fa3 == 0) {
        return procWolfWaitInit();
    }

    commonProcInit(PROC_WOLF_CHAIN_UP);
    setSingleAnimeWolfParam(WANM_PICKUP_B, &mpHIO->mWolf.mWlGrab.m.mBoneGrabAnm);
    mNormalSpeed = 0.0f;

    return 1;
}

int daAlink_c::procWolfChainUp() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    setWallGrabStatus(0x96, 2);

    if (frameCtrl->checkAnmEnd() ||
        (checkInputOnR() && frameCtrl->getFrame() > mpHIO->mWolf.mWlGrab.m.mBoneGrabAnm.mCancelFrame))
    {
        u8 old_2fa3 = field_0x2fa3;
        fopAc_ac_c* old_actor = field_0x2844.getActor();

        procWolfWaitInit();
        setUpperAnimeBaseSpeed(m_wlAnmDataTable[WANM_CHAIN_PULL].field_0x0, 0.0f, 3.0f);

        if (old_actor != NULL) {
            field_0x2fa3 = old_2fa3;
            field_0x2844.setData(old_actor);
        }
    }

    return 1;
}

int daAlink_c::procWolfGanonCatchInit() {
    if (!dComIfGp_event_compulsory(this, 0, 0xFFEF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    field_0x27f4->actor_status |= 0x800;

    commonProcInit(PROC_WOLF_GANON_CATCH);
    field_0x280c.setData(field_0x27f4);
    static_cast<fopEn_enemy_c*>(field_0x27f4)->setThrowModeCatch();
    setSingleAnimeWolfBase(WANM_PIG_GANON_CATCH_ST);

    shape_angle.y = cLib_targetAngleY(&current.pos, &field_0x27f4->current.pos);
    current.angle.y = shape_angle.y + 0x8000;

    mNormalSpeed = 50.0f;
    mProcVar5.field_0x3012 = 30;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    setFootEffectProcType(0);
    field_0x2f9d = 0x60;
    mProcVar4.field_0x3010 = 4;
    return 1;
}

int daAlink_c::procWolfGanonCatch() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c == 0) {
        fopEn_enemy_c* ganon_p = (fopEn_enemy_c*)field_0x280c.getActor();
        if (ganon_p == NULL) {
            resetSpecialEvent();
            procWolfWaitInit();
            return 1;
        }

        if (!doButton() || !mLinkAcch.ChkGroundHit()) {
            resetSpecialEvent();
            ganon_p->setThrowModeDash();
            procWolfWaitInit();
            return 1;
        }

        if (checkAnmEnd(frameCtrl_p)) {
            setSingleAnimeWolfBase(WANM_PIG_GANON_CATCH_END);
        }

        if (checkInputOnR()) {
            int direction = getDirectionFromAngle(mStickAngle);
            if (direction == DIR_LEFT || direction == DIR_RIGHT) {
                mProcVar4.field_0x3010 = direction;
            }
        }

        if (cLib_chaseF(&mNormalSpeed, 0.0f, 1.0f)) {
            if (mProcVar5.field_0x3012 != 0) {
                mProcVar5.field_0x3012--;
            } else {
                if (mProcVar4.field_0x3010 == DIR_LEFT) {
                    setSingleAnimeWolfBase(WANM_PIG_GANON_THROW_LEFT_START);
                    ganon_p->setThrowModeThrowLeft();
                } else if (mProcVar4.field_0x3010 == DIR_RIGHT) {
                    setSingleAnimeWolfBase(WANM_PIG_GANON_THROW_RIGHT_START);
                    ganon_p->setThrowModeThrowRight();
                } else {
                    resetSpecialEvent();
                    ganon_p->setThrowModeDash();
                    procWolfWaitInit();
                    return 1;
                }

                mProcVar2.field_0x300c = 1;
                current.angle.y = shape_angle.y;
            }
        } else if (mNormalSpeed > 5.0f) {
            field_0x2f9d = 0x60;
        }
    } else {
        if (checkAnmEnd(frameCtrl_p)) {
            if (mProcVar4.field_0x3010 == DIR_LEFT) {
                setSingleAnimeWolfBase(WANM_PIG_GANON_THROW_LEFT_END);
            } else {
                setSingleAnimeWolfBase(WANM_PIG_GANON_THROW_RIGHT_END);
            }
        }

        if (mProcVar3.field_0x300e != 0) {
            resetSpecialEvent();
            procWolfWaitInit();
        }
    }

    return 1;
}

int daAlink_c::procWolfChainReadyInit() {
    commonProcInit(PROC_WOLF_CHAIN_READY);
    field_0x280c.setData(field_0x27f4);
    setSingleAnimeWolfParam(WANM_ATTACK_A_START, &mpHIO->mWolf.mWlAttack.mWlAtNjump.m.mAerialAttackAnm);
    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;

    return 1;
}

int daAlink_c::procWolfChainReady() {
    daObjWchain_c* chain = static_cast<daObjWchain_c*>(field_0x280c.getActor());
    if (chain == NULL || mLinkAcch.ChkWallHit()) {
        return procWolfFallInit(2, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
    } else if (mUnderFrameCtrl[0].getFrame() < 3.0f) {
        s16 target_angle_y = cLib_targetAngleY(&current.pos, &chain->getJumpAimPos());
        cLib_addCalcAngleS(&shape_angle.y, target_angle_y, 2, 0x2000, 0x800);
        current.angle.y = shape_angle.y;
    } else if (mProcVar3.field_0x300e == 0) {
        shape_angle.y = cLib_targetAngleY(&current.pos, &chain->getJumpAimPos());
        current.angle.y = shape_angle.y;
        mProcVar3.field_0x300e = 1;
        cXyz vec1 = chain->getJumpAimPos() - field_0x34e0;
        f32 dist_xz = vec1.absXZ();
        f32 dist = vec1.abs();
        s16 angle = vec1.atan2sY_XZ();
        mProcVar2.field_0x300c = angle >> 1;
        mNormalSpeed = dist_xz * 40.0f / dist + 1.0f;
        mProcVar0.field_0x3008 = dist_xz / mNormalSpeed + 1.0f;
        speed.y = (vec1.y - gravity * 0.5f * mProcVar0.field_0x3008 * mProcVar0.field_0x3008) /
                  mProcVar0.field_0x3008;
    } else {
        mProcVar0.field_0x3008--;
        if (chain->getJumpAimPos().abs2(field_0x34e0) < 10000.0f) {
            procWolfChainWaitInit();
        } else if (mProcVar0.field_0x3008 == 0) {
            procWolfFallInit(2, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
        }
    }
    return 1;
}

int daAlink_c::procWolfChainWaitInit() {
    daObjWchain_c* chain = static_cast<daObjWchain_c*>(field_0x280c.getActor());
    commonProcInit(PROC_WOLF_CHAIN_WAIT);
    setSingleAnimeWolfParam(WANM_HANDLE_HANG, &mpHIO->mWolf.mWlChain.m.mLoweringStartAnm);
    mProcVar1.field_0x300a = 0;
    chain->onRide();
    field_0x2f99 = 0x50;
    current.pos = chain->getTopPos();
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;
    mProcVar4.field_0x3010 = 0;
    mProcVar0.field_0x3008 = mpHIO->mWolf.mWlChain.m.mTensionWaitTime + 20;
    field_0x3478 = chain->getPullLength();
    current.angle.y = shape_angle.y;
    if (mNormalSpeed > 30.0f) {
        mNormalSpeed = 15.0f;
    } else {
        mNormalSpeed *= 0.5f;
    }
    field_0x37c8.set(mNormalSpeed * cM_ssin(shape_angle.y), 0.0f,
                     mNormalSpeed * cM_scos(shape_angle.y));
    mNormalSpeed = 0.0f;
    voiceStart(Z2SE_WL_V_BITE_HANG);
    return 1;
}

int daAlink_c::procWolfChainWait() {
    daObjWchain_c* chain = static_cast<daObjWchain_c*>(field_0x280c.getActor());
    if (chain == NULL || mProcVar0.field_0x3008 <= -mpHIO->mWolf.mWlChain.m.mReleaseWaitTime) {
        mNormalSpeed = field_0x37c8.absXZ();
        current.angle.y = field_0x37c8.atan2sX_Z();
        onNoResetFlg2(FLG2_UNK_200);
        return procWolfFallInit(2, mpHIO->mWolf.mWlWallHang.m.mHangingFallInterp);
    }

    if (mProcVar4.field_0x3010 == 0) {
        if (mProcVar0.field_0x3008 > 20) {
            mProcVar0.field_0x3008--;
        } else {
            field_0x3478 += mpHIO->mWolf.mWlChain.m.mFallSpeed;
            if (field_0x3478 > chain->getSwitchOffset()) {
                chain->onNowSwitchFlg();
                mProcVar4.field_0x3010 = 1;
                dComIfGp_getVibration().StartShock(7, 3, cXyz(0.0f, 1.0f, 0.0f));
            }
        }
    } else {
        mProcVar0.field_0x3008--;
        if (mProcVar0.field_0x3008 < 0) {
            field_0x3478 = chain->getDownOffset();
        } else {
            field_0x3478 = (chain->getSwitchOffset() + chain->getLastOffset() * 0.5f) +
                           (mProcVar0.field_0x3008 * 0.5f * 0.05f + 0.5f) * chain->getLastOffset() *
                               cM_scos(mProcVar0.field_0x3008 * 0x3000);
            if (mProcVar0.field_0x3008 == 0) {
                chain->onEndFlg();
            }
        }
    }

    cXyz vec1 = current.pos - chain->getRoofPos();
    if (checkInputOnR()) {
        vec1.x += field_0x33a8 * cM_ssin(field_0x2fe2);
        vec1.z += field_0x33a8 * cM_scos(field_0x2fe2);
    }
    vec1.y -= 15.0f;
    vec1 += field_0x37c8;
    vec1.normalizeZP();
    current.pos = chain->getRoofPos() + vec1 * (field_0x3478 + chain->getInitOutLength());
    field_0x37c8 = (current.pos - field_0x3798) * 0.95f;
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::multVec(&vec1, &vec1);
    shape_angle.x = cM_atan2s(-vec1.z, -vec1.y);
    shape_angle.z = cM_atan2s(vec1.x, JMAFastSqrt(vec1.y * vec1.y + vec1.z * vec1.z));
    mProcVar2.field_0x300c = shape_angle.x;

    daPy_frameCtrl_c* frame_ctrl = &mUnderFrameCtrl[0];
    if (mProcVar1.field_0x300a != 0 || frame_ctrl->checkAnmEnd()) {
        daAlink_WANM anm;
        f32 f1, f2, f3;
        if (checkInputOnR()) {
            anm = WANM_HANDLE_HANG_SW;
            f1 = mpHIO->mWolf.mWlChain.m.mWaitAnmSpeed;
            f2 = mpHIO->mWolf.mWlChain.m.mWaitInterp;
        } else {
            anm = WANM_HANDLE_HANG_WAIT;
            f1 = mpHIO->mWolf.mWlChain.m.mSwingAnmSpeed;
            f2 = mpHIO->mWolf.mWlChain.m.mSwingInterp;
        }
        if (anm != mUnderAnmHeap[0].getIdx()) {
            if (mProcVar1.field_0x300a == 0) {
                mProcVar1.field_0x300a = 1;
                f3 = 0.0f;
            } else {
                f3 = frame_ctrl->getFrame() / frame_ctrl->getEnd();
            }
            setSingleAnimeWolfBaseSpeed(anm, f1, f2);
            frame_ctrl->setFrame(f3 * frame_ctrl->getEnd());
        }
    }

    chain->setPullLength(field_0x3478);
    return 1;
}

int daAlink_c::procWolfDigInit() {
    BOOL var_r28 = 1;

    if (checkEndResetFlg1(ERFLG1_UNK_100000) && field_0x27f4 != NULL && fopAcM_GetName(field_0x27f4) == PROC_Obj_Digpl) {
        int type = ((daObjDigpl_c*)field_0x27f4)->getType();
        if (type == 0 || type == 2 || type == 3) {
            var_r28 = 0;
        }
    }

    if (!checkEndResetFlg1(ERFLG1_UNK_100000)) {
        var_r28 = 0;
    }

    if (var_r28) {
        if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
            return 0;
        }
        mDemo.setSpecialDemoType();
    }

    commonProcInit(PROC_WOLF_DIG);

    mFallVoiceInit = var_r28;

    if (field_0x27f4 != NULL) {
        mProcVar1.mDigActorName = fopAcM_GetName(field_0x27f4);
    } else {
        mProcVar1.mDigActorName = PROC_ALINK;
    }

    fopAc_ac_c* var_r29;
    if (checkEndResetFlg1(ERFLG1_UNK_100000) && mProcVar1.mDigActorName != PROC_Obj_Lv4DigSand && mProcVar1.mDigActorName != PROC_Obj_DigSnow) {
        var_r29 = field_0x27f4;
    } else {
        var_r29 = NULL;
    }

    mProcVar5.field_0x3012 = 0;
    mProcVar4.mDigExitID = 0;

    if (var_r29 != NULL) {
        if (fopAcM_GetName(var_r29) == PROC_Obj_Digpl) {
            mProcVar2.mDigType = ((daObjDigpl_c*)var_r29)->getType();
            mProcVar4.mDigExitID = ((daObjDigpl_c*)var_r29)->getSceneNum();
            field_0x280c.setData(var_r29);
            mProcVar5.field_0x3012 = 1;
        } else {
            field_0x280c.clearData();
            mProcVar2.mDigType = 3;
        }

        s16 var_r27 = fopAcM_searchActorAngleY(this, var_r29);
        if (abs((s16)(var_r27 - shape_angle.y)) > 0x4000) {
            var_r27 += 0x8000;
        }

        shape_angle.y = var_r27;
        current.angle.y = shape_angle.y;
        current.pos.x = var_r29->current.pos.x - (50.0f * cM_ssin(shape_angle.y));
        current.pos.z = var_r29->current.pos.z - (50.0f * cM_scos(shape_angle.y));
        field_0x37c8 = var_r29->current.pos;
    } else {
        mProcVar2.mDigType = 4;

        if (checkEndResetFlg1(ERFLG1_UNK_100000)) {
            if (mProcVar1.mDigActorName == PROC_Obj_DigSnow) {
                ((daObjDigSnow_c* )field_0x27f4)->startDig();
            } else if (mProcVar1.mDigActorName == PROC_Obj_Lv4DigSand) {
                ((daObjL4DigSand_c*)field_0x27f4)->startDig();
            }
            field_0x280c.setData(field_0x27f4);
        } else {
            field_0x280c.clearData();
        }

        field_0x37c8.set(current.pos.x + (50.0f * cM_ssin(shape_angle.y)), current.pos.y, current.pos.z + (50.0f * cM_scos(shape_angle.y)));
    }

    s16 var_r26;
    if (mProcVar2.mDigType == 0 || mProcVar2.mDigType == 2 || mProcVar2.mDigType == 3) {
        var_r26 = 59;
        field_0x3478 = 51.0f;
    } else if (mProcVar2.mDigType == 1) {
        var_r26 = 33;
        field_0x3478 = 100.0f;
        dCam_getBody()->SetTrimTypeForce(3);
    } else {
        var_r26 = mpHIO->mWolf.mWlChain.m.mDigAnm.mEndFrame;
        field_0x3478 = 66.0f;
    }

    f32 var_f31;
    if (checkEndResetFlg1(ERFLG1_UNK_100000)) {
        var_f31 = 0.0f;
    } else {
        var_f31 = 29.0f;
        var_r26 = 59;
    }

    setSingleAnimeWolf(WANM_DIG, mpHIO->mWolf.mWlChain.m.mDigAnm.mSpeed, var_f31, var_r26, mpHIO->mWolf.mWlChain.m.mDigAnm.mInterpolation);
    mNormalSpeed = 0.0f;
    field_0x3588 = l_wolfBaseAnime;
    mProcVar3.mDigChangeArea = false;

    cXyz sp8(field_0x37c8.x, 10.0f + field_0x37c8.y, field_0x37c8.z);
    mLinkGndChk.SetPos(&sp8);

    f32 temp_f1 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
    if (field_0x37c8.y - temp_f1 < l_autoUpHeight) {
        mPolyInfo2.SetPolyInfo(mLinkGndChk);
        field_0x37c8.y = temp_f1;
    } else {
        mPolyInfo2.SetPolyInfo(mLinkAcch.m_gnd);
    }

    field_0x37d4 = field_0x37c8;
    if (fopAcM_wt_c::waterCheck(&field_0x37c8) && fopAcM_wt_c::getPolyAtt0() != 6 && fopAcM_wt_c::getWaterY() > field_0x37c8.y) {
        field_0x37d4.y = fopAcM_wt_c::getWaterY();
        field_0x3198 = 7;
        mPolyInfo2.SetPolyInfo(*fopAcM_wt_c::getWaterCheck());
    } else {
        field_0x3198 = dComIfG_Bgsp().GetPolyAtt0(mPolyInfo2);
    }

    field_0x32cc = 0;
    mProcVar0.field_0x3008 = 0;
    return 1;
}

int daAlink_c::procWolfDig() {
    daPy_frameCtrl_c* frame_ctrl = &mUnderFrameCtrl[0];

    fopAc_ac_c* dig_actor;
    if (mProcVar5.field_0x3012 != 0) {
        dig_actor = field_0x280c.getActor();
    } else {
        dig_actor = NULL;
    }

    if (mProcVar2.mDigType == 1) {
        dCam_getBody()->SetTrimTypeForce(3);
    }

    field_0x2f99 = 4;
    onEndResetFlg1(ERFLG1_WOLF_EYE_KEEP);

    cXyz sp14(field_0x37c8);
    dComIfG_Bgsp().MoveBgCrrPos(mLinkAcch.m_gnd, mLinkAcch.ChkGroundHit(), &field_0x37c8, NULL, NULL, false, false);
    field_0x37d4 += field_0x37c8 - sp14;

    if (mProcVar3.mDigChangeArea) {
        if (frame_ctrl->getFrame() >= 25.0f) {
            onSceneChangeArea(mProcVar4.mDigExitID, 0xFF, NULL);

            if (dig_actor != NULL) {
                ((daObjDigpl_c*)dig_actor)->onDigFlg();
                field_0x280c.clearData();
            }
        }

        setWolfDigEffect();
    } else if (checkAnmEnd(frame_ctrl)) {
        if (mProcVar2.mDigType == 1) {
            setSingleAnimeWolfParam(WANM_DIG_IN, &mpHIO->mWolf.mWlChain.m.mDigFallAnm);
            mProcVar3.mDigChangeArea = true;
            setWolfDigEffect();
        } else {
            if (field_0x280c.getActor() != NULL) {
                if (mProcVar1.mDigActorName == PROC_Obj_DigSnow) {
                    ((daObjDigSnow_c*)field_0x280c.getActor())->endDig();
                } else if (mProcVar1.mDigActorName == PROC_Obj_Lv4DigSand) {
                    ((daObjL4DigSand_c*)field_0x280c.getActor())->endDig();
                }
            }

            if (checkEventRun() && mFallVoiceInit != 0) {
                resetSpecialEvent();
            } else {
                procWolfWaitInit();
            }
        }
    } else {
        if (frame_ctrl->checkPass(field_0x3478) && dig_actor != NULL) {
            ((daObjDigpl_c*)dig_actor)->onDigFlg();
            field_0x280c.clearData();
        }
 
        if (frame_ctrl->getFrame() >= 11.0f) {
            if (frame_ctrl->getFrame() <= 41.0f) {
                if (mProcVar0.field_0x3008 == 0) {
                    mProcVar0.field_0x3008 = 1;
                    seStartOnlyReverb(Z2SE_WOLF_DIG_HOLE);
                }

                setWolfDigEffect();

                if (checkMidnaRide()) {
                    getMidnaActor()->setForceNeckAimPos(field_0x37c8);
                }
            }
        }
    }

    return 1;
}

int daAlink_c::procWolfDigThroughInit(int param_0) {
    static const u16 snowID[] = {
        0x884D, 0x884E, 0x8851, 0x8852,
    };
    static const u16 smokeID[] = {
        0x83A0, 0x83A1, 0x83A4, 0x83A5,
    };

    if (param_0 != 0) {
        mDemo.setSpecialDemoType();
    } else if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
        return 0;
    } else {
        mDemo.setSpecialDemoType();
    }

    commonProcInit(PROC_WOLF_DIG_THROUGH);

    int var_r27 = field_0x2fbb;

    if (param_0 == 0) {
        if (fopAcM_seenActorAngleY(field_0x27f4, this) <= 0x4000) {
            shape_angle.y = field_0x27f4->shape_angle.y + 0x8000;
        } else {
            shape_angle.y = field_0x27f4->shape_angle.y;
        }

        current.angle.y = shape_angle.y;
        current.pos.x = field_0x27f4->current.pos.x;
        current.pos.z = field_0x27f4->current.pos.z;

        ((daObjDigholl_c*)field_0x27f4)->onDigStart();

        mProcVar4.field_0x3010 = ((daObjDigholl_c*)field_0x27f4)->getSceneNum();
        if (mProcVar4.field_0x3010 >= 0) {
            field_0x3198 = 1;
        } else {
            field_0x3198 = 3;
        }

        dCam_getBody()->StartEventCamera(0x1E, fopAcM_GetID(this), "Type", 1, &field_0x3198, 0);

        cXyz sp8(current.pos.x, 30.0f + field_0x27f4->current.pos.y, current.pos.z);
        if (fopAcM_gc_c::gndCheck(&sp8)) {
            var_r27 = fopAcM_gc_c::getPolyAtt0();
        }
    } else {
        field_0x3198 = -1;
        mProcVar4.field_0x3010 = -1;
        current.pos.x -= 30.0f * cM_ssin(shape_angle.y);
        current.pos.z -= 30.0f * cM_scos(shape_angle.y);
    }

    mProcVar5.field_0x3012 = param_0;

    const u16* var_r29 = var_r27 == 13 ? snowID : smokeID;
    for (int i = 0; i < 4; i++) {
        dComIfGp_particle_set(*var_r29, &current.pos, &shape_angle, NULL);
        var_r29++;
    }

    seStartOnlyReverb(Z2SE_WOLF_DIG_HOLE_THROUGH);
    setSingleAnimeWolfBase(WANM_DIG_UNDER);
    setFaceBasicTexture(FTANM_UNK_99);

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);

    field_0x3588 = l_wolfBaseAnime;
    field_0x2f99 = 4;
    field_0x34d4 = l_wolfBaseAnime;
    mProcVar3.field_0x300e = 0;
    return 1;
}

int daAlink_c::procWolfDigThrough() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    field_0x2f99 = 4;

    if (mProcVar3.field_0x300e != 0) {
        onSceneChangeArea(mProcVar4.field_0x3010, 0xFF, NULL);
    } else {
        if (checkAnmEnd(frameCtrl_p)) {
            if (mProcVar5.field_0x3012 != 0) {
                dComIfGs_setRestartRoom(current.pos, shape_angle.y, getStartRoomNo());
            }

            resetSpecialEvent();

            if (checkInputOnR()) {
                checkNextActionWolf(0);
            } else {
                field_0x2f99 = 4;
                procWolfSwimEndWaitInit(1);
            }

            return 1;
        }

        if (mProcVar4.field_0x3010 >= 0 && frameCtrl_p->checkPass(21.0f)) {
            mProcVar3.field_0x300e = 1;
        }
    }

    if (frameCtrl_p->checkPass(100.0f)) {
        voiceStart(Z2SE_WL_V_BREATH_RUN);
    }

    return 1;
}

void daAlink_c::setSmellSave() {
    if (dComIfGs_getCollectSmell() == fpcNm_ITEM_SMELL_MEDICINE) {
        dComIfGs_offEventBit(0x2F04);  // OFF: Got Medicine Scent
    }

    dMsgObject_setSmellType(mProcVar5.field_0x3012);

    if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_MEDICINE) {
        dComIfGs_onEventBit(0x2F04);  // ON: Got Medicine Scent
    } else if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_CHILDREN) {
        fopAcM_onSwitch(this, 102);
    } else if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_YELIA_POUCH) {
        fopAcM_onSwitch(this, 103);
    } else if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_FISH) {
        dComIfGs_onEventBit(0x6120);  // ON: Got Reekfish scent
    } else if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_POH) {
        dComIfGs_onEventBit(0x6210);  // ON: Got Poe scent
    }

    setGetSubBgm(mProcVar5.field_0x3012);
}

int daAlink_c::procWolfGetSmellInit() {
    if (!dComIfGp_event_compulsory(this, 0, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_WOLF_GET_SMELL);
    setSingleAnimeWolfBaseSpeed(WANM_SMELL, mpHIO->mWolf.mWlChain.m.mSniffAnmSpeed,
                                mpHIO->mWolf.mWlChain.m.mSniffInterp);
    voiceStartLevel(Z2SE_WL_V_SNIFF);

    shape_angle.y = fopAcM_searchActorAngleY(this, field_0x27f4);
    current.angle.y = shape_angle.y;

    current.pos.x = field_0x27f4->current.pos.x - cM_ssin(shape_angle.y) * 100.0f;
    current.pos.z = field_0x27f4->current.pos.z - cM_scos(shape_angle.y) * 100.0f;

    field_0x280c.setData(field_0x27f4);
    static_cast<kytag03_class*>(field_0x27f4)->field_0x588 = 1;
    static_cast<kytag03_class*>(field_0x27f4)->field_0x587 = 0;
    mProcVar5.field_0x3012 = static_cast<kytag03_class*>(field_0x27f4)->field_0x589;
    fopAcM_OnStatus(field_0x27f4, 0x800);

    dComIfGp_setPlayerStatus1(0, 0x4000000);
    mProcVar2.field_0x300c = 0;

    if (dComIfGs_getCollectSmell() == fpcNm_ITEM_NONE) {
        field_0x3198 = 0xBBC;
    } else {
        field_0x3198 = 0xBBB;
    }

    field_0x319c = -1;

    if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_YELIA_POUCH) {
        field_0x319c = 0x2220;
    } else if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_CHILDREN) {
        field_0x319c = 0x2240;
    }

    if (field_0x319c < 0 || dComIfGs_isEventBit(field_0x319c)) {
        field_0x319c = -1;
    }

    mProcVar4.field_0x3010 = 0;
    mNormalSpeed = 0.0f;
    field_0x3588 = l_wolfBaseAnime;
    onWolfEyeUp();
    return 1;
}

int daAlink_c::procWolfGetSmell() {
    kytag03_class* kytag3_p = (kytag03_class*)field_0x280c.getActor();

    if (field_0x319c >= 0) {
        if (checkSpecialDemoMode()) {
            dComIfGp_getEvent()->reset(this);
            dComIfGp_clearPlayerStatus1(0, 0x4000000);

            if (mProcVar5.field_0x3012 == fpcNm_ITEM_SMELL_YELIA_POUCH) {
                fopAcM_orderOtherEvent(this, "SMELL_1st", 0xFFFF, 0x400, 1);
            } else {
                fopAcM_orderOtherEvent(this, "SMELL_Child", 0xFFFF, 0x400, 1);
            }

            mDemo.setSystemDemoType();
        } else if (mDemo.getParam0() == 0) {
            dComIfGs_onEventBit(field_0x319c);
            dComIfGp_evmng_cutEnd(mAlinkStaffId);

            if (kytag3_p != NULL) {
                kytag3_p->field_0x588 = 0;
            }
        }
    }

    if (checkUnderMove0BckNoArcWolf(WANM_SMELL)) {
        voiceStartLevel(Z2SE_WL_V_SNIFF);
    }

    if (mProcVar4.field_0x3010 != 0) {
        field_0x2f99 = 4;
    }

    if (mProcVar2.field_0x300c != 0) {
        if (mMsgFlow.doFlow(this, NULL, 0)) {
            if (kytag3_p != NULL) {
                kytag3_p->field_0x588 = 0;
            }
            resetSpecialEvent();
        } else if (mMsgFlow.getChoiceNo() == 0 &&
                   mProcVar5.field_0x3012 != dComIfGs_getCollectSmell())
        {
            setSmellSave();
        }
    } else if (mProcVar4.field_0x3010 == 0) {
        if ((kytag3_p == NULL || (field_0x319c < 0 && kytag3_p->field_0x588 == 2)) ||
            (mDemo.getDemoMode() == 0x37 && mDemo.getParam0() == 1))
        {
            setSingleAnimeWolf(WANM_GET_A, 1.0f, 10.0f, -1, 3.0f);
            dComIfGp_setPlayerStatus1(0, 0x4000000);

            if (kytag3_p != NULL) {
                fopAcM_OnStatus(kytag3_p, 0x800);
            }

            mProcVar4.field_0x3010 = 1;

            dCam_getBody()->StartEventCamera(8, fopAcM_GetID(this), 0);
            field_0x2f99 = 12;

            if (field_0x3198 == 0xBBC) {
                setSmellSave();
            }
        } else if (mProcVar0.field_0x3008 != 0) {
            mProcVar0.field_0x3008--;
        }
    } else if (checkAnmEnd(mUnderFrameCtrl)) {
        setSingleAnimeWolfBase(WANM_GET);
        field_0x2f99 = 12;
        mProcVar2.field_0x300c = 1;
        mMsgFlow.init(this, field_0x3198, 0, NULL);
    }

    return 1;
}
