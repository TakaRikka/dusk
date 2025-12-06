/**
 * d_a_alink_whistle.inc
 * Player Horse / Hawk Grass Whistle action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_npc_tk.h"
#include "d/actor/d_a_obj_yobikusa.h"

static fopAc_ac_c* daAlink_searchWhistle(fopAc_ac_c* i_actor, void* i_data) {
    if (daAlink_getAlinkActorClass()->getGrassHowlEventActor() == fopAcM_GetName(i_actor)) {
        return i_actor;
    }

    return NULL;
}

static fopAc_ac_c* daAlink_searchHawk(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetName(i_actor) == PROC_NPC_TK) {
        return i_actor;
    }

    return NULL;
}

void daAlink_c::setGrassWhistleModel(int modelType) {
    static const char* grassWhistleArcName[] = {
        "J_Tobi",
        "J_Umak",
    };
    static const u16 grassWhistleIdx[2] = {
        3,
        3,
    };

    JKRHeap* prev_heap = setItemHeap();

    J3DModelData* modeldata = (J3DModelData*)dComIfG_getObjectRes(grassWhistleArcName[modelType],
                                                                  grassWhistleIdx[modelType]);
    mHeldItemModel = mDoExt_J3DModel__create(modeldata, 0x80000, 0x11000084);

    mDoExt_setCurrentHeap(prev_heap);
    mEquipItem = 0x104;
    field_0x2f94 = 2;
}

void daAlink_c::setHorseWhistleModel() {
    JKRHeap* prev_heap = setItemHeap();

    mHeldItemModel = initModel(loadAramBmd(0x31A, 0x3000), 0);

    mDoExt_setCurrentHeap(prev_heap);
    mEquipItem = fpcNm_ITEM_HORSE_FLUTE;
    field_0x2f94 = 2;
}

int daAlink_c::procGrassWhistleGetInit() {
    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_GRASS_WHISTLE_GET, field_0x27f4);
    }

    commonProcInit(PROC_GRASS_WHISTLE_GET);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x2f98 = 4;

    setSingleAnimeParam(ANM_BOTTLE_SWING_DOWN, &mpHIO->mItem.mBottle.m.mSwingDownAnm);
    field_0x2f92 = 0xFE;
    field_0x280c.setData(field_0x27f4);

    daObjYobikusa_c* yobikusa_p = (daObjYobikusa_c*)field_0x27f4;
    mProcVar2.field_0x300c = yobikusa_p->getType();
    mProcVar3.field_0x300e = yobikusa_p->getPathID();
    mProcVar4.field_0x3010 = yobikusa_p->isPlayerCorrect();
    mProcVar5.field_0x3012 = yobikusa_p->shape_angle.y + 0x8000;
    field_0x37c8.set(yobikusa_p->current.pos.x + cM_ssin(yobikusa_p->shape_angle.y) * 50.0f,
                     yobikusa_p->current.pos.y + 50.0f,
                     yobikusa_p->current.pos.z + cM_scos(yobikusa_p->shape_angle.y) * 50.0f);
    voiceStart(Z2SE_AL_V_SWING_BOTTLE);
    return 1;
}


int daAlink_c::procGrassWhistleGet() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (frameCtrl_p->checkAnmEnd()) {
        procGrassWhistleWaitInit(mProcVar2.field_0x300c, mProcVar3.field_0x300e,
                                 mProcVar4.field_0x3010, mProcVar5.field_0x3012, &field_0x37c8);
    } else if (mEquipItem == fpcNm_ITEM_NONE && frameCtrl_p->getFrame() >= 9.0f) {
        setGrassWhistleModel(mProcVar2.field_0x300c);

        if (field_0x280c.getActor() != NULL) {
            fopAcM_setCarryNow(field_0x280c.getActor(), 0);
            seStartOnlyReverb(Z2SE_AL_PICKUP_GRASS);
        }
    }

    return 1;
}

int daAlink_c::procGrassWhistleWaitInit(int param_0, int param_1, int param_2, s16 param_3,
                                        cXyz* param_4) {
    u16 var_r6;
    if (param_0 == 3) {
        var_r6 = 0xFFFF;
    } else {
        var_r6 = 0xFFF9;
    }

    if (!dComIfGp_event_compulsory(this, NULL, var_r6)) {
        return checkNextAction(0);
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_GRASS_WHISTLE_WAIT);

    if (param_0 == 3) {
        keepItemData();
        setHorseWhistleModel();
        setSingleAnimeBase(ANM_GRASS_WHISTLE_START);
        field_0x3198 = -1;
        field_0x32cc = 0x532;
    } else {
        setSingleAnimeBase(ANM_WAIT);
        mKeepItem = fpcNm_ITEM_NONE;

        if (param_0 == 0) {
            field_0x3198 = 0x100;
            field_0x32cc = 0x535;
        } else {
            field_0x3198 = -1;
            field_0x32cc = 0x532;
        }
    }

    setUpperAnimeBaseMorf(getMainBckData(ANM_GRASS_WHISTLE_START)->m_underID, -1.0f);
    setFacePriAnime(ANM_GRASS_WHISTLE_START);
    mProcVar2.field_0x300c = param_0;
    mProcVar3.field_0x300e = param_1;

    dCam_getBody()->StartEventCamera(12, fopAcM_GetID(this), 0);
    mProcVar0.field_0x3008 = 0;
    mProcVar4.field_0x3010 = 0;
    mProcVar5.field_0x3012 = 0;
    mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x2f98 = 4;

    if (param_2) {
        mLinkGndChk.SetPos(param_4);

        f32 gnd_cross = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
        if (gnd_cross != -G_CM3D_F_INF) {
            current.pos.set(param_4->x, gnd_cross, param_4->z);
        }

        shape_angle.y = param_3;
        current.angle.y = shape_angle.y;
    }

    dComIfGp_setPlayerStatus1(0, 0x4000000);
    return 1;
}


int daAlink_c::procGrassWhistleWait() {
    if (mProcVar5.field_0x3012 != 0) {
        if (checkEndMessage(field_0x32cc)) {
            resetSpecialEvent();
            return procWaitInit();
        }

        return 1;
    }

    if (mProcVar0.field_0x3008 < 0) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == -3) {
            return procWaitInit();
        }
    } else if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == 0) {
            daHorse_c* horse_p = (daHorse_c*)dComIfGp_getHorseActor();
            daNPC_TK_c* tk_p = (daNPC_TK_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchWhistle, NULL);

            if (tk_p != NULL) {
                fopAcM_OnStatus(tk_p, 0x800);

                if (field_0x3198 == 0x100) {
                    tk_p->setEvent();
                    tk_p->setHawkPath(mProcVar3.field_0x300e);
                }

                mProcVar0.field_0x3008--;
                resetSpecialEvent();
            } else {
                int temp_r4;
                if ((mProcVar2.field_0x300c == 1 || mProcVar2.field_0x300c == 3) && horse_p != NULL) {
                    temp_r4 = horse_p->callHorse(&current.pos);
                } else {
                    temp_r4 = 0;
                }

                if (temp_r4 == 1 || temp_r4 == 4 || temp_r4 == 2) {
                    return procCoHorseCallWaitInit(temp_r4);
                } else if (temp_r4 == 3) {
                    resetSpecialEvent();
                    return procWaitInit();
                }

                setFaceBasicTexture(FTANM_UNK_0);
                mProcVar5.field_0x3012 = 1;
                return 1;
            }
        }

        setGrassCancelStatus(0);
    } else if (checkAnmEnd(&mUpperFrameCtrl[2])) {
        setGrassCancelStatus(0x12);
        setDoStatus(0x2C);

        if (mProcVar2.field_0x300c == 3 || doTrigger()) {
            setSingleAnimeBaseMorf(ANM_GRASS_WHISTLE_PLAY, 0.0f);
            resetUpperAnime(UPPER_2, -1.0f);
            dComIfGp_event_onHindFlag(6);

            if (mProcVar2.field_0x300c == 0) {
                voiceStart(Z2SE_PLAY_REED_TOBIKUSA);
                mProcVar0.field_0x3008 = 100;

                if (dComIfGs_isEventBit(dSv_event_flag_c::F_0700) &&
                    dComIfGs_isEventBit(dSv_event_flag_c::F_0037))
                {
                    dComIfGs_onEventBit(dSv_event_flag_c::F_0581);
                }
            } else {
                voiceStart(Z2SE_PLAY_REED_UMAKUSA);
                mProcVar0.field_0x3008 = 140;

                if (mProcVar2.field_0x300c == 1) {
                    dComIfGs_onEventBit(dSv_event_flag_c::F_0580);
                }
            }
        } else if (grassCancelTrigger()) {
            resetUpperAnime(UPPER_2, -1.0f);
            resetSpecialEvent();
            procWaitInit();
        }
    } else {
        #if VERSION != VERSION_SHIELD_DEBUG
        setGrassCancelStatus(0);
        #endif
    }

    return 1;
}

int daAlink_c::procCoHorseCallWaitInit(int param_0) {
    commonProcInit(PROC_HORSE_CALL_WAIT);

    if (param_0 == 4) {
        mProcVar4.field_0x3010 = 2;
    } else if (param_0 == 1) {
        mProcVar4.field_0x3010 = 0x3D;
    } else {
        mProcVar4.field_0x3010 = 0x10;
    }

    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMove.m.mIdleAnmSpeed, 3.0f);
    } else {
        setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
    }

    setGrassCancelStatus(0);
    dComIfGp_setPlayerStatus1(0, 0x4000000);
    return 1;
}

int daAlink_c::procCoHorseCallWait() {
    setGrassCancelStatus(0);

    mProcVar4.field_0x3010--;
    if (mProcVar4.field_0x3010 > 2 && dComIfGp_getHorseActor() != NULL) {
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchActorAngleY(this, dComIfGp_getHorseActor()), 2, 0x2000, 0x800);
        current.angle.y = shape_angle.y;
    }

    if (mProcVar4.field_0x3010 == 60) {
        dCam_getBody()->EventFlag(2);
    } else if (mProcVar4.field_0x3010 == 15) {
        dCam_getBody()->EventFlag(3);
    } else if (mProcVar4.field_0x3010 == 0) {
        resetSpecialEvent();

        if (checkWolf()) {
            procWolfWaitInit();
        } else {
            procWaitInit();
        }
    }

    return 1;
}

int daAlink_c::procHawkCatchInit() {
    if (mProcID == PROC_HAWK_SUBJECT || !commonProcInitNotSameProc(PROC_HAWK_CATCH)) {
        return 0;
    }

    setSingleAnimeBase(ANM_HAWK_CATCH);
    deleteEquipItem(FALSE, TRUE);
    return 1;
}

int daAlink_c::procHawkCatch() {
    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
        procHawkSubjectInit();
    }

    return 1;
}

int daAlink_c::procHawkSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_HAWK_SUBJECT)) {
        return 0;
    }

    setSingleAnimeBase(ANM_HAWK_WAIT);

    fopAc_ac_c* hawk_p = (fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchHawk, NULL);
    if (hawk_p != NULL) {
        field_0x280c.setData(hawk_p);
    }

    mProcVar2.field_0x300c = 0;
    mBodyAngle.y = 0;
    deleteEquipItem(FALSE, TRUE);
    return 1;
}

int daAlink_c::procHawkSubject() {
    if (mProcVar2.field_0x300c != 0) {
        if (mProcVar2.field_0x300c != 0) {
            mProcVar2.field_0x300c--;
            if (mProcVar2.field_0x300c == 0) {
                checkWaitAction();
            }
        }
    } else if (checkEventRun()) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else {
        fopAc_ac_c* temp_r29 = field_0x280c.getActor();
        setGrassCancelStatus(0x12);
        setDoStatus(0x31);

        if (doTrigger()) {
            mProcVar2.field_0x300c = 2;
            if (temp_r29 != NULL) {
                ((daNPC_TK_c*)temp_r29)->setAttack();
                field_0x280c.clearData();
            }
        } else if (grassCancelTrigger()) {
            return checkWaitAction();
        } else if (setBodyAngleToCamera()) {
            cXyz sp8;
            checkSightLine(10000.0f, &sp8);
            mSight.setPos(&sp8);
            mSight.onDrawFlg();
        }
    }

    return 1;
}
