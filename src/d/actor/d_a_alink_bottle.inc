/**
 * d_a_alink_bottle.inc
 * Player Bottle action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_insect.h"
#include "SSystem/SComponent/c_math.h"

static int daAlink_bottleModelCallBack(J3DJoint* i_joint, int param_1);

int daAlink_c::bottleModelCallBack() {
    if (mItemMode != 0) {
        mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(mRightItemJntNo));
        mDoMtx_stack_c::transM(-3.0f, -1.0f, 0.3f);
        mDoMtx_stack_c::XYZrotM(cM_deg2s(102.0f), cM_deg2s(-0.5f), cM_deg2s(-122.0f));
        mDoMtx_copy(mDoMtx_stack_c::get(), mHeldItemModel->getAnmMtx(1));
    }

    return 1;
}

static int daAlink_bottleModelCallBack(J3DJoint* i_joint, int param_1) {
    UNUSED(i_joint);
    UNUSED(param_1);

    daAlink_c* player_p = (daAlink_c*)j3dSys.getModel()->getUserArea();

    if (param_1 == 0) {
        player_p->bottleModelCallBack();
    }

    return 1;
}

fpc_ProcID daAlink_c::getBottleOpenAppearItem() const {
    if (mProcID == PROC_BOTTLE_OPEN) {
        return mEquipItem;
    }

    return fpcM_ERROR_PROCESS_ID_e;
}

BOOL daAlink_c::checkOilBottleItemNotGet(u16 i_itemNo) {
    return mProcID != PROC_BOTTLE_GET && mProcID != PROC_BOTTLE_SWING &&
           checkOilBottleItem(i_itemNo);
}

void daAlink_c::setBottleModel(u16 i_itemNo) {
    JKRHeap* prev_heap = setItemHeap();
    mHeldItemModel = initModelEnv(loadAramBmd(dRes_ID_ALANM_BMD_AL_BOTTLE_e, 0x5C00), 0x1020200);
    field_0x072c = loadAramItemBtp(dRes_ID_ALANM_BTP_AL_BOTTLE_e, mHeldItemModel);
    field_0x0724 = loadAramItemBrk(dRes_ID_ALANM_BRK_AL_BOTTLE_e, mHeldItemModel);

    if (checkDrinkBottleItem(i_itemNo) || checkOilBottleItemNotGet(i_itemNo)) {
        field_0x071c = loadAramItemBtk(dRes_ID_ALANM_BTK_BOTTLE_D_YURE_e, mHeldItemModel);
        field_0x0720 = loadAramItemBtk(dRes_ID_ALANM_BTK_BOTTLE_D_NOMU_e, mHeldItemModel);
    } else {
        field_0x071c = loadAramItemBtk(dRes_ID_ALANM_BTK_BOTTLE_NUKU_e, mHeldItemModel);
        field_0x0720 = loadAramItemBtk(dRes_ID_ALANM_BTK_BOTTLE_DASU_e, mHeldItemModel);
    }

    field_0x0718 = loadAramItemBtk(dRes_ID_ALANM_BTK_BOTTLE_YURE_e, mHeldItemModel);

    if (i_itemNo == dItemNo_FAIRY_e) {
        J3DAnmTransform* bck = (J3DAnmTransform*)loadAram(dRes_ID_ALANM_BCK_O_GD_NV_e, 0xC00);
        mpHookTipModel = initModel(loadAramBmd(dRes_ID_ALANM_BMD_O_GD_NV_e, 0x1400), 0);
        if (!mHookTipBck.init(bck, FALSE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
            JUT_ASSERT(142, FALSE);
        }
        mHookTipBck.entry(mpHookTipModel->getModelData(), 0.0f);
    } else if (i_itemNo == dItemNo_WORM_e) {
        J3DAnmTransform* bck = (J3DAnmTransform*)loadAram(dRes_ID_ALANM_BCK_O_GD_WORM_e, 0xC00);
        mpHookTipModel = initModel(loadAramBmd(dRes_ID_ALANM_BMD_O_GD_WORM_e, 0x2400), 0);
        if (!mHookTipBck.init(bck, FALSE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
            JUT_ASSERT(151, FALSE);
        }
        mHookTipBck.entry(mpHookTipModel->getModelData(), 0.0f);
    } else if (i_itemNo == dItemNo_BEE_CHILD_e) {
        mpHookTipModel = initModel(loadAramBmd(dRes_ID_ALANM_BMD_O_GD_HK_e, 0x1000), 0);
    }

    mDoExt_setCurrentHeap(prev_heap);

    J3DModelData* item_model_data = mHeldItemModel->getModelData();
    mEquipItem = i_itemNo;

    if (checkOilBottleItemNotGet(i_itemNo)) {
        item_model_data->getJointNodePointer(1)->setCallBack(NULL);
        field_0x2f95 = 9;
    } else {
        item_model_data->getJointNodePointer(1)->setCallBack(daAlink_bottleModelCallBack);
        mHeldItemModel->setUserArea((uintptr_t)this);
        field_0x2f94 = 3;
        mItemMode = 0;
    }

    if ((checkDrinkBottleItem(i_itemNo) && i_itemNo != dItemNo_BEE_CHILD_e) || checkOilBottleItem(i_itemNo)) {
        item_model_data->getMaterialNodePointer(0)->getShape()->hide();

        if (i_itemNo == dItemNo_RED_BOTTLE_e || i_itemNo == dItemNo_CHUCHU_RED_e || i_itemNo == dItemNo_RED_BOTTLE_2_e) {
            field_0x0724->setFrame(0.0f);
        } else if (i_itemNo == dItemNo_GREEN_BOTTLE_e || i_itemNo == dItemNo_CHUCHU_GREEN_e) {
            field_0x0724->setFrame(1.0f);
        } else if (i_itemNo == dItemNo_BLUE_BOTTLE_e || i_itemNo == dItemNo_CHUCHU_BLUE_e) {
            field_0x0724->setFrame(2.0f);
        } else if (checkOilBottleItem(i_itemNo)) {
            field_0x0724->setFrame(4.0f);
        } else if (i_itemNo == dItemNo_HOT_SPRING_e) {
            field_0x0724->setFrame(5.0f);
        } else if (i_itemNo == dItemNo_CHUCHU_PURPLE_e) {
            field_0x0724->setFrame(6.0f);
        } else if (i_itemNo == dItemNo_CHUCHU_RARE_e) {
            field_0x0724->setFrame(7.0f);
        } else if (i_itemNo == dItemNo_UGLY_SOUP_e || i_itemNo == dItemNo_CHUCHU_BLACK_e) {
            field_0x0724->setFrame(8.0f);
        } else if (i_itemNo == dItemNo_LV1_SOUP_e) {
            field_0x0724->setFrame(9.0f);
        } else if (i_itemNo == dItemNo_LV2_SOUP_e || i_itemNo == dItemNo_LV3_SOUP_e) {
            field_0x0724->setFrame(10.0f);
        } else if (i_itemNo == dItemNo_FAIRY_DROP_e) {
            field_0x0724->setFrame(11.0f);
        } else {
            field_0x0724->setFrame(3.0f);

            if (i_itemNo == dItemNo_HALF_MILK_BOTTLE_e) {
                field_0x072c->setFrame(1.0f);
            }
        }
    } else if (checkOpenBottleItem(i_itemNo)) {
        if (i_itemNo == dItemNo_WATER_BOTTLE_e) {
            field_0x0724->setFrame(5.0f);
        } else {
            field_0x072c->setFrame(2.0f);
        }

        item_model_data->getMaterialNodePointer(0)->getShape()->show();
    } else {
        field_0x072c->setFrame(2.0f);
        item_model_data->getMaterialNodePointer(0)->getShape()->hide();
    }
}

int daAlink_c::commonBottleDrink(BOOL param_0) {
    simpleAnmPlay(field_0x0718);
    simpleAnmPlay(field_0x071c);

    daPy_frameCtrl_c* frameCtrl_p;
    u16 current_anm;
    if (param_0 || mUpperAnmHeap[2].getIdx() == dRes_ID_ALANM_BCK_TAKEL_e) {
        frameCtrl_p = &mUpperFrameCtrl[2];
        current_anm = mUpperAnmHeap[2].getIdx();
    } else {
        frameCtrl_p = &mUnderFrameCtrl[0];
        current_anm = mUnderAnmHeap[0].getIdx();
    }

    if (current_anm == getMainBckData(ANM_BOTTLE_DRINK_START)->m_underID) {
        if (checkAnmEnd(frameCtrl_p)) {
            if (param_0) {
                setUpperAnimeBaseMorf(getMainBckData(ANM_BOTTLE_DRINK)->m_upperID, 0.0f);
                setFacePriAnime(ANM_BOTTLE_DRINK);
            } else {
                setSingleAnimeBaseMorf(ANM_BOTTLE_DRINK, 0.0f);
            }

            mItemVar0.mBottleItemHealAmount = 0;

            if (mEquipItem != dItemNo_UGLY_SOUP_e && mEquipItem != dItemNo_CHUCHU_BLACK_e &&
                mEquipItem != dItemNo_CHUCHU_PURPLE_e)
            {
                if (mEquipItem == dItemNo_LV1_SOUP_e) {
                    dComIfGp_setItemLifeCount(8, 0);
                } else if (mEquipItem == dItemNo_LV2_SOUP_e) {
                    dComIfGp_setItemLifeCount(16, 0);
                } else if (mEquipItem == dItemNo_LV3_SOUP_e) {
                    dComIfGp_setItemLifeCount(32, 0);
                } else if (mEquipItem == dItemNo_BEE_CHILD_e) {
                    field_0x2fd3 = 3;
                    dComIfGp_setItemLifeCount(dComIfGp_getSelectItemNum(mSelectItemId), 0);
                } else if (mEquipItem == dItemNo_RED_BOTTLE_e || mEquipItem == dItemNo_CHUCHU_RED_e ||
                           mEquipItem == dItemNo_RED_BOTTLE_2_e)
                {
                    dComIfGp_setItemLifeCount(32, 0);
                } else if (mEquipItem == dItemNo_MILK_BOTTLE_e || mEquipItem == dItemNo_HALF_MILK_BOTTLE_e) {
                    dComIfGp_setItemLifeCount(12, 0);
                } else if (mEquipItem == dItemNo_BLUE_BOTTLE_e || mEquipItem == dItemNo_CHUCHU_BLUE_e ||
                           mEquipItem == dItemNo_HOT_SPRING_e)
                {
                    dComIfGp_setItemLifeCount(dComIfGs_getMaxLifeGauge(), 0);
                } else if (mEquipItem == dItemNo_CHUCHU_RARE_e || mEquipItem == dItemNo_FAIRY_DROP_e) {
                    dComIfGp_setItemLifeCount(dComIfGs_getMaxLifeGauge(), 0);
                }
            }
            voiceStart(Z2SE_AL_V_DRINK);
        } else if (frameCtrl_p->checkPass(50.0f)) {
            field_0x071c->setFrame(0.0f);
            mHeldItemModel->getModelData()->entryTexMtxAnimator(field_0x071c);
        }
    } else if (current_anm == getMainBckData(ANM_BOTTLE_DRINK)->m_underID) {
        if (dComIfGp_getItemNowLife() == dComIfGs_getLife()) {
            mpHookTipModel = NULL;

            if (mEquipItem == dItemNo_MILK_BOTTLE_e) {
                if (param_0) {
                    setUpperAnimeParam(getMainBckData(ANM_BOTTLE_DRINK_END)->m_upperID, UPPER_2,
                                       &mpHIO->mItem.mBottle.m.mEndDrinkAnm);
                    setFacePriAnime(ANM_BOTTLE_DRINK_END);
                } else {
                    setSingleAnimeParam(ANM_BOTTLE_DRINK_END,
                                        &mpHIO->mItem.mBottle.m.mEndDrinkAnm);
                }

                field_0x072c->setFrame(1.0f);
                if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_SPECIAL_e) {
                    dComIfGs_setEquipBottleItemIn(mSelectItemId, dItemNo_HALF_MILK_BOTTLE_e);
                }
            } else if (mProcVar2.field_0x300c == 0) {
                mProcVar2.field_0x300c = 1;
                mHeldItemModel->getModelData()->entryTexMtxAnimator(field_0x0720);
            } else if (simpleAnmPlay(field_0x0720)) {
                if (mEquipItem == dItemNo_UGLY_SOUP_e) {
                    if (dComIfGs_getLife() <= 2) {
                        mItemVar0.mBottleItemHealAmount = dComIfGs_getMaxLifeGauge();
                    } else if (dComIfGs_getLife() <= 4) {
                        mItemVar0.mBottleItemHealAmount = (dComIfGs_getMaxLifeGauge() / 2) - dComIfGs_getLife();
                    } else if (dComIfGs_getLife() == dComIfGs_getMaxLifeGauge() || cM_rnd() < 0.5f) {
                        mItemVar0.mBottleItemHealAmount = -4;
                    } else {
                        mItemVar0.mBottleItemHealAmount = 4;
                    }
                } else if (mEquipItem == dItemNo_CHUCHU_PURPLE_e) {
                    f32 rnd = cM_rndF(6.0f);
                    if (rnd < 1.0f) {
                        mItemVar0.mBottleItemHealAmount = dComIfGs_getMaxLifeGauge();
                    } else if (rnd < 2.0f) {
                        mItemVar0.mBottleItemHealAmount = -(dComIfGs_getLife() - 1);
                        if (mItemVar0.mBottleItemHealAmount == 0) {
                            mItemVar0.mBottleItemHealAmount = -1;
                        }
                    } else if (rnd < 4.0f) {
                        mItemVar0.mBottleItemHealAmount = -4;
                    } else {
                        mItemVar0.mBottleItemHealAmount = 4;
                    }
                } else if (mEquipItem == dItemNo_CHUCHU_BLACK_e) {
                    mItemVar0.mBottleItemHealAmount = -4;
                }

                if (mItemVar0.mBottleItemHealAmount < 0 || mEquipItem == dItemNo_BEE_CHILD_e) {
                    if (param_0) {
                        setUpperAnimeParam(getMainBckData(ANM_BOTTLE_DRINK_NASTY)->m_upperID,
                                           UPPER_2, &mpHIO->mItem.mBottle.m.mDrinkNastyAnm);
                        setFacePriAnime(ANM_BOTTLE_DRINK_NASTY);
                    } else {
                        setSingleAnimeParam(ANM_BOTTLE_DRINK_NASTY,
                                            &mpHIO->mItem.mBottle.m.mDrinkNastyAnm);
                    }
                } else if (param_0) {
                    setUpperAnimeParam(getMainBckData(ANM_BOTTLE_DRINK_END)->m_upperID, UPPER_2,
                                       &mpHIO->mItem.mBottle.m.mEndDrinkAnm);
                    setFacePriAnime(ANM_BOTTLE_DRINK_END);
                } else {
                    setSingleAnimeParam(ANM_BOTTLE_DRINK_END,
                                        &mpHIO->mItem.mBottle.m.mEndDrinkAnm);
                }

                field_0x072c->setFrame(2.0f);
                if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_SPECIAL_e) {
                    dComIfGs_setEquipBottleItemEmpty(mSelectItemId);
                }

                if (mEquipItem == dItemNo_CHUCHU_RARE_e || mEquipItem == dItemNo_FAIRY_DROP_e) {
                    mSwordUpTimer = 510;
                }
            }
        }
    } else if (current_anm == getMainBckData(ANM_BOTTLE_DRINK_END)->m_underID ||
               current_anm == getMainBckData(ANM_BOTTLE_DRINK_NASTY)->m_underID)
    {
        if (checkAnmEnd(frameCtrl_p)) {
            if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_42_e) {
                setSingleAnimeParam(ANM_PICK_UP, &mpHIO->mItem.mPickUp.m.mPlaceAnm);
            } else {
                allUnequip(mKeepItem == dItemNo_KANTERA_e);
            }
        } else if (frameCtrl_p->checkPass(11.0f)) {
            mHeldItemModel->getModelData()->entryTexMtxAnimator(field_0x0718);
        } else if (mItemVar0.mBottleItemHealAmount >= 0 && frameCtrl_p->checkPass(21.0f)) {
            if (mItemVar0.mBottleItemHealAmount > 0) {
                dComIfGp_setItemLifeCount(mItemVar0.mBottleItemHealAmount, 0);
            }

            voiceStart(Z2SE_AL_V_DRINK_2);
        } else if (mItemVar0.mBottleItemHealAmount < 0) {
            if (frameCtrl_p->checkPass(25.0f)) {
                voiceStart(Z2SE_AL_V_DRINK_DAMAGE);
            } else if (frameCtrl_p->checkPass(17.0f)) {
                mDoAud_seStart(Z2SE_AL_DAMAGE_SMALL, NULL, 0, 0);

                if (dComIfGs_getLife() <= -mItemVar0.mBottleItemHealAmount) {
                    dComIfGp_setItemLifeCount(-(dComIfGs_getLife() - 1), 0);
                } else {
                    dComIfGp_setItemLifeCount(mItemVar0.mBottleItemHealAmount, 0);
                }
            }
        }
    } else if (current_anm == getMainBckData(ANM_PICK_UP)->m_underID) {
        if (checkAnmEnd(frameCtrl_p)) {
            resetUpperAnime(UPPER_2, 3.0f);
            dCam_getBody()->EndEventCamera(fopAcM_GetID(this));
            return 1;
        } else if (frameCtrl_p->checkPass(9.0f)) {
            returnKeepItemData();
        }
    } else if (mEquipItem == dItemNo_NONE_e || mEquipItem == dItemNo_KANTERA_e) {
        resetUpperAnime(UPPER_2, 3.0f);
        if (mEquipItem == dItemNo_NONE_e) {
            returnKeepItemData();
        }

        resetSpecialEvent();
        return 1;
    }

    return 0;
}

fopAc_ac_c* daAlink_c::makeFairy(cXyz* i_pos, u32 i_params) {
    cXyz fairy_pos;

    if (checkModeFlg(0x40000)) {
        fairy_pos = *i_pos;

        if (checkWolf()) {
            fairy_pos.y -= 100.0f;
        } else {
            fairy_pos.y -= 130.0f;
        }

        i_pos = &fairy_pos;
    }

    field_0x285c.setData(fopAcM_fastCreate(fpcNm_Obj_Yousei_e, i_params, i_pos, fopAcM_GetRoomNo(this),
                         &shape_angle, NULL, -1, NULL, NULL));
    onResetFlg0(RFLG0_UNK_400);

    if (checkEventRun() && field_0x285c.getActor() != NULL) {
        fopAcM_OnStatus(field_0x285c.getActor(), 0x800);
    }

    return field_0x285c.getActor();
}

int daAlink_c::procBottleDrinkInit(u16 i_itemNo) {
    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_42_e) {
        if (mProcID == PROC_BOTTLE_DRINK) {
            return 1;
        }
    } else {
        if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
            return 0;
        }

        mDemo.setSpecialDemoType();
    }

    commonProcInit(PROC_BOTTLE_DRINK);
    current.angle.y = shape_angle.y;
    mNormalSpeed = 0.0f;
    field_0x2f98 = 4;

    setSingleAnimeParam(ANM_BOTTLE_DRINK_START, &mpHIO->mItem.mBottle.m.mStartDrinkAnm);
    keepItemData();
    setBottleModel(i_itemNo);
    dComIfGp_setPlayerStatus1(0, 0x2000);
    field_0x3194 = 1;
    mProcVar0.field_0x3008 = 0;

    u32 id = fopAcM_GetID(this);
    dCam_getBody()->StartEventCamera(0x12, id, "Type", 1, &field_0x3194, nullptr);
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    return 1;
}

int daAlink_c::procBottleDrink() {
    if (mProcVar3.field_0x300e != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else {
        mProcVar3.field_0x300e = commonBottleDrink(0);
    }

    return 1;
}

int daAlink_c::procBottleOpenInit(u16 i_itemNo) {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_BOTTLE_OPEN);
    current.angle.y = shape_angle.y;
    mNormalSpeed = 0.0f;
    field_0x2f98 = 4;

    setSingleAnimeParam(ANM_BOTTLE_OPEN, &mpHIO->mItem.mBottle.m.mOpenBottleAnm);
    keepItemData();
    setBottleModel(i_itemNo);
    dComIfGp_setPlayerStatus1(0, 0x4000);
    mProcVar1.field_0x300a = 0;
    field_0x32cc = -1;
    mProcVar0.field_0x3008 = 1;

    if (mEquipItem == dItemNo_FAIRY_e) {
        field_0x3194 = 3;
    } else {
        field_0x3194 = 2;

        if (mEquipItem == dItemNo_WORM_e) {
            field_0x32cc = fopAcM_create(fpcNm_NPC_WORM_e, 2, &current.pos, fopAcM_GetRoomNo(this),
                                         NULL, NULL, -1);
            mProcVar0.field_0x3008 = 0;
            mProcVar1.field_0x300a = 1;
        }
    }

    mProcVar3.field_0x300e = 0;

    dCam_getBody()->StartEventCamera(0x12, fopAcM_GetID(this), "Type", 1, &field_0x3194, nullptr);
    mProcVar2.field_0x300c = 0;

    field_0x27c8.initialize(current.pos.y + mHeight);

    if (i_itemNo == dItemNo_WATER_BOTTLE_e) {
        setCylAtParam(AT_TYPE_LANTERN_SWING, dCcG_At_Spl_UNK_0, 0, dCcD_SE_NONE, 0, 30.0f, 30.0f);
    }

    mProcVar4.field_0x3010 = 0;
    return 1;
}

int daAlink_c::procBottleOpen() {
    static Vec const bottleTop = {0.0f, 25.0f, 0.0f};
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c == 0) {
        simpleAnmPlay(field_0x0718);
    } else if (mProcVar2.field_0x300c == 2) {
        if (simpleAnmPlay(field_0x0720)) {
            field_0x072c->setFrame(2.0f);
        }
    } else if (simpleAnmPlay(field_0x071c)) {
        mHeldItemModel->getModelData()->entryTexMtxAnimator(field_0x0718);
        mProcVar2.field_0x300c = 0;
    }

    if (checkUnderMove0BckNoArc(ANM_BOTTLE_OPEN)) {
        if (checkAnmEnd(frameCtrl_p)) {
            if (mEquipItem == dItemNo_FAIRY_e) {
                setSingleAnimeBaseMorf(ANM_BOTTLE_FAIRY, -1.0f);
            } else {
                setSingleAnimeParam(ANM_BOTTLE_POUR, &mpHIO->mItem.mBottle.m.mPourAnm);

                if (mEquipItem == dItemNo_WATER_BOTTLE_e) {
                    if (mWaterY > mLeftHandPos.y) {
                        mProcVar3.field_0x300e = 0;
                    } else {
                        mProcVar3.field_0x300e = 1;
                    }
                }
            }
        } else if (frameCtrl_p->checkPass(30.0f)) {
            mItemMode = 1;
            mProcVar2.field_0x300c = 1;
            mHeldItemModel->getModelData()->entryTexMtxAnimator(field_0x071c);

            if (mEquipItem == dItemNo_FAIRY_e) {
                cXyz fairyPos;
                mDoMtx_multVec(mHeldItemModel->getBaseTRMtx(), &bottleTop, &fairyPos);

                if (makeFairy(&fairyPos, 0)) {
                    onModeFlg(0x8000000);
                    mpHookTipModel = NULL;
                    resetFairyEffect();
                }
            }
        } else if (frameCtrl_p->getFrame() >= 18.0f) {
            mRightHandIndex = 5;
        }
    } else {
        if (checkAnmEnd(frameCtrl_p) && mProcVar0.field_0x3008 != 0) {
            returnKeepItemData();
            resetSpecialEvent();
        } else if (frameCtrl_p->checkPass(11.0f)) {
            mHeldItemModel->getModelData()->entryTexMtxAnimator(field_0x0720);
            mProcVar2.field_0x300c = 2;
            dComIfGs_setEquipBottleItemEmpty(mSelectItemId);

            if (mEquipItem == dItemNo_WATER_BOTTLE_e && mProcVar3.field_0x300e != 0) {
                seStartOnlyReverb(Z2SE_LK_SPRINKLE_WATER);
            }
        } else if (frameCtrl_p->getFrame() > 16.0f && frameCtrl_p->getFrame() < 31.0f) {
            mProcVar4.field_0x3010 = 1;
        } else {
            mProcVar4.field_0x3010 = 0;
        }

        if (mProcVar0.field_0x3008 == 0 && frameCtrl_p->getFrame() >= 25.0f) {
            dInsect_c* insect_p = (dInsect_c*)fopAcM_SearchByID(field_0x32cc);
            if (insect_p != NULL) {
                mpHookTipModel = NULL;
                mProcVar0.field_0x3008 = 1;
                mDoMtx_multVec(mHeldItemModel->getBaseTRMtx(), &bottleTop, &insect_p->current.pos);
                fopAcM_OnStatus(insect_p, 0x800);

                if (mProcVar1.field_0x300a != 0) {
                    insect_p->Insect_Release();
                }
            }
        }
    }

    return 1;
}

int daAlink_c::procBottleSwingInit(fopAc_ac_c* i_catchActor, int param_1) {
    if (i_catchActor != NULL && mEquipItem != dItemNo_KANTERA_e) {
        return procPreActionUnequipInit(PROC_BOTTLE_SWING, NULL);
    }

    if (i_catchActor == NULL && getReadyItem() != dItemNo_EMPTY_BOTTLE_e) {
        return checkWaitAction();
    }

    commonProcInit(PROC_BOTTLE_SWING);
    current.angle.y = shape_angle.y;
    mNormalSpeed = 0.0f;
    field_0x2f98 = 4;
    mProcVar4.field_0x3010 = param_1;

    fopAc_ac_c* catchActor;
    if (i_catchActor != NULL) {
        catchActor = i_catchActor;
        field_0x280c.setData(i_catchActor);
    } else {
        catchActor = dComIfGp_att_getCatghTarget();
    }

    BOOL temp_r3;
    s16 catchActorName;
    if (catchActor != NULL) {
        if (catchActor->attention_info.position.y > field_0x3834.y - 30.0f) {
            temp_r3 = false;
        } else {
            temp_r3 = true;
        }

        catchActorName = fopAcM_GetName(catchActor);
    } else {
        if (field_0x2fbc != 11 && field_0x2fbc != 6 && mWaterY > current.pos.y + 10.0f) {
            temp_r3 = true;
        } else {
            temp_r3 = false;
        }

        catchActorName = fpcNm_ALINK_e;
    }

    if (catchActorName == fpcNm_TAG_SSDRINK_e || catchActorName == fpcNm_OBJ_OILTUBO_e ||
        catchActorName == fpcNm_TAG_BTLITM_e || mEquipItem == dItemNo_KANTERA_e)
    {
        setSingleAnimeParam(ANM_BOTTLE_SCOOP, &mpHIO->mItem.mBottle.m.mScoopAnm);

        if (mEquipItem == dItemNo_KANTERA_e) {
            onNoResetFlg2(FLG2_KANDELAAR_LIGHT_OFF);

            JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(field_0x31c4);
            if (emitter_p != NULL) {
                emitter_p->stopDrawParticle();
            }
        }

        field_0x3478 = mpHIO->mItem.mBottle.m.mScoopAnm.mCancelFrame;
        field_0x347c = 14.0f;

        if (catchActor != NULL) {
            shape_angle.y = cLib_targetAngleY(&current.pos, &catchActor->current.pos);
            current.angle.y = shape_angle.y;

            current.pos.x = catchActor->current.pos.x - cM_ssin(shape_angle.y) * 70.0f;
            current.pos.z = catchActor->current.pos.z - cM_scos(shape_angle.y) * 70.0f;
        }

        if (catchActorName == fpcNm_OBJ_OILTUBO_e) {
            onModeFlg(0x2000);
        }
    } else if (temp_r3) {
        setSingleAnimeParam(ANM_BOTTLE_SWING_DOWN, &mpHIO->mItem.mBottle.m.mSwingDownAnm);
        field_0x3478 = mpHIO->mItem.mBottle.m.mSwingDownAnm.mCancelFrame;
        field_0x347c = 8.0f;
    } else {
        setSingleAnimeParam(ANM_BOTTLE_SWING, &mpHIO->mItem.mBottle.m.mSwingSideAnm);
        field_0x3478 = mpHIO->mItem.mBottle.m.mSwingSideAnm.mCancelFrame;
        field_0x347c = 4.0f;
    }

    mProcVar2.field_0x300c = 0;
    voiceStart(Z2SE_AL_V_SWING_BOTTLE);
    seStartOnlyReverb(Z2SE_AL_SWING_BOTTLE);
    mProcVar3.field_0x300e = 0;
    return 1;
}

int daAlink_c::procBottleSwing() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c == 0) {
        if (eventInfo.checkCommandCatch()) {
            if (mProcVar3.field_0x300e == 0) {
                dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                mProcVar3.field_0x300e = 1;
            }

            dComIfGp_setPlayerStatus1(0, 0x4000000);

            if (dComIfGp_att_getCatghTarget() != NULL) {
                fopAcM_OnStatus(dComIfGp_att_getCatghTarget(), 0x800);
            } else if (field_0x280c.getActor() != NULL) {
                fopAcM_OnStatus(field_0x280c.getActor(), 0x800);
            }

            if (frameCtrl_p->getFrame() > field_0x347c) {
                mProcVar2.field_0x300c = 1;
                offModeFlg(4);

                if (mEquipItem != dItemNo_KANTERA_e) {
                    u16 bottle_item = dComIfGp_event_getPreItemNo();

                    if (bottle_item == dItemNo_WATER_BOTTLE_e &&
                        getMoveBGActorName(mLinkAcch.m_wtr, FALSE) == fpcNm_Obj_Onsen_e)
                    {
                        bottle_item = dItemNo_HOT_SPRING_e;
                    }

                    dComIfGs_setEquipBottleItemIn(mSelectItemId, bottle_item);

                    if (bottle_item == dItemNo_BEE_CHILD_e) {
                        dComIfGp_setSelectItemNum(mSelectItemId, dComIfGs_getBottleMax());
                    } else if (bottle_item == dItemNo_WATER_BOTTLE_e) {
                        seStartOnlyReverb(Z2SE_LK_SCOOP_WATER);
                    } else if (checkOilBottleItem(bottle_item)) {
                        seStartOnlyReverb(Z2SE_AL_SCOOP_OIL);
                    } else if (bottle_item == dItemNo_LV1_SOUP_e || bottle_item == dItemNo_LV2_SOUP_e ||
                               bottle_item == dItemNo_LV3_SOUP_e)
                    {
                        seStartOnlyReverb(Z2SE_AL_SCOOP_SOUP);
                    }

                    setBottleModel(bottle_item);
                    mHeldItemModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
                }

                mDemo.setSpecialDemoType();
            }
        } else if (frameCtrl_p->getFrame() <= field_0x347c) {
            if (getReadyItem() == dItemNo_EMPTY_BOTTLE_e &&
                (dComIfGp_att_getCatghTarget() != NULL ||
                 (field_0x2fbc != 11 && field_0x2fbc != 6 && mWaterY > current.pos.y + 10.0f)))
            {
                fopAcM_orderCatchEvent(this, dComIfGp_att_getCatghTarget(), 0, 0);
            } else if (mEquipItem == dItemNo_KANTERA_e) {
                if (field_0x280c.getActor() != NULL) {
                    fopAcM_orderCatchEvent(this, field_0x280c.getActor(), 0, 0);
                }
            }
        }
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar2.field_0x300c != 0) {
            procBottleGetInit(mProcVar4.field_0x3010);
        } else {
            checkNextAction(0);
        }
    } else if (frameCtrl_p->getFrame() > field_0x3478 && mProcVar2.field_0x300c == 0) {
        onModeFlg(4);
        checkNextAction(1);
    }

    return 1;
}

int daAlink_c::procBottleGetInit(BOOL param_0) {
    BOOL isPrevPickUp = mProcID == PROC_PICK_UP;
    commonProcInit(PROC_BOTTLE_GET);

    int var_r30;
    if (mEquipItem == dItemNo_KANTERA_e) {
        setSingleAnime(ANM_BOTTLE_GET, mpHIO->mItem.mBottle.m.mGetAnm.mSpeed, 40.0f,
                       mpHIO->mItem.mBottle.m.mGetAnm.mEndFrame,
                       mpHIO->mItem.mBottle.m.mGetAnm.mInterpolation);
        mRightHandIndex = 0xFE;

        if (isPrevPickUp) {
            execItemGet(dItemNo_KANTERA_e);
            var_r30 = dItemNo_KANTERA2_e;
            dComIfGp_setItemOilCount(-dComIfGs_getMaxOil());
        } else {
            dComIfGp_setItemOilCount(dComIfGs_getMaxOil());

            if (param_0) {
                var_r30 = dItemNo_CHUCHU_YELLOW2_e;
            } else {
                var_r30 = dItemNo_OIL2_e;
            }
        }

        setGetSubBgm(var_r30);
    } else {
        setSingleAnimeParam(ANM_BOTTLE_GET, &mpHIO->mItem.mBottle.m.mGetAnm);
        var_r30 = mEquipItem;
    }

    dComIfGp_setPlayerStatus1(0, 0x4001000);
    mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
    field_0x3194 = 0;
    field_0x32cc = var_r30 + 0x65;

    if (mEquipItem == dItemNo_KANTERA_e) {
        field_0x3194 = 12;
    }

    if (mEquipItem == dItemNo_KANTERA_e) {
        resetBasAnime();
    }

    u32 id = fopAcM_GetID(this);
    dCam_getBody()->StartEventCamera(0x12, id, "Type", 1, &field_0x3194, nullptr);

    if (mEquipItem == dItemNo_FAIRY_DROP_e || mEquipItem == dItemNo_CHUCHU_RARE_e || mEquipItem == dItemNo_WORM_e) {
        setFaceBasicTexture(FTANM_I_BINGET);
        setFaceBasicBck(dRes_ID_ALANM_BCK_FI_e);
    } else if (mEquipItem == dItemNo_UGLY_SOUP_e) {
        setFaceBasicBck(dRes_ID_ALANM_BCK_FJ_e);
    }

    if (mEquipItem == dItemNo_BEE_CHILD_e && dComIfGs_isEventBit(dSv_event_flag_c::F_0700)) {
        dComIfGs_onEventBit(dSv_event_flag_c::F_0618);
    }

    return 1;
}

int daAlink_c::procBottleGet() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    simpleAnmPlay(field_0x0718);

    if (checkAnmEnd(frameCtrl) && checkEndMessage(field_0x32cc)) {
        resetFairyEffect();
        if (mEquipItem != dItemNo_KANTERA_e) {
            deleteEquipItem(FALSE, FALSE);
        }

        resetSpecialEvent();
    } else if (frameCtrl->getFrame() >= 37.0f) {
        mRightHandIndex = 0xFE;
    } else if (frameCtrl->getFrame() >= 15.0f) {
        mItemMode = 0;
    } else if (frameCtrl->getFrame() >= 5.0f) {
        mHeldItemModel->getModelData()->getMaterialNodePointer(0)->getShape()->show();
        mRightHandIndex = 5;
        mItemMode = 1;
    }

    if (frameCtrl->checkPass(39.0f)) {
        setGetSubBgm(mEquipItem);
    }

    return 1;
}
