/**
 * d_a_kytag04.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag04.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

static int daKytag04_Draw(kytag04_class* i_this) {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    camera_class* camera = dComIfGp_getCamera(0);
    Mtx sp40;
    cXyz sp50;
    cXyz sp5C;

    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp68;
    BOOL var_r30 = false;

    sp5C.x = 20.0f;
    sp5C.y = 20.0f;
    sp5C.z = 20.0f;

    sp68 = a_this->current.pos;
    sp68.y += 6000.0f;

    a_this->attention_info.position = sp68;
    a_this->eyePos = a_this->attention_info.position;

    if (i_this->field_0x5b4 == 4 || i_this->field_0x5b4 == 5) {
        if (i_this->field_0x5b4 == 4) {
            if (i_this->field_0x5b5 != 0xFF && i_this->field_0x5d1 == 1) {
                if (dComIfGs_isSwitch(i_this->field_0x5b5, dComIfGp_roomControl_getStayNo())) {
                    var_r30 = 1;
                }
            }
        } else {
            if (i_this->field_0x5b5 != 0xFF && i_this->field_0x5d1 == 1 &&
                dComIfGs_isSaveSwitch(i_this->mStageNo, i_this->field_0x5b5))
            {
                var_r30 = 1;
            }
        }

        if (var_r30 != 0) {
            if (i_this->field_0x5c8 <= 0.0f || i_this->field_0x5c8 >= 999.0f) {
                if (i_this->field_0x5c8 <= 0.0f) {
                    i_this->field_0x5a4 = dComIfGp_particle_set(0x84A5, &sp68, NULL, &sp5C);
                }

                if (i_this->field_0x5c8 >= 999.0f) {
                    i_this->field_0x5a8 = dComIfGp_particle_set(0x84A6, &sp68, NULL, &sp5C);
                    i_this->field_0x5ac = dComIfGp_particle_set(0x84A7, &sp68, NULL, &sp5C);
                }
            }

            if (i_this->field_0x5cc != 0 && i_this->field_0x5cc < 20) {
                i_this->field_0x5cc++;

                if (i_this->field_0x5cc == 15) {
                    if (i_this->field_0x5a8 == NULL) {
                        i_this->field_0x5a8 = dComIfGp_particle_set(0x84A6, &sp68, NULL, &sp5C);
                    }

                    if (i_this->field_0x5ac == NULL) {
                        i_this->field_0x5ac = dComIfGp_particle_set(0x84A7, &sp68, NULL, &sp5C);
                    }
                }
            }

            cLib_addCalc(&i_this->field_0x5c8, 1.0f, 0.25f, 0.0125f, 0.0001f);
        } else {
            cLib_addCalc(&i_this->field_0x5c8, 0.0f, 0.5f, 0.02f, 0.0001f);
        }

        if (i_this->field_0x5c8 > 0.0f) {
            mDoMtx_stack_c::transS(sp68.x, sp68.y, sp68.z);
            cMtx_copy(mDoMtx_stack_c::get(), sp40);
            sp50.x = 20.0f;
            sp50.y = 20.0f;
            sp50.z = 20.0f;

            i_this->mpModel->setBaseScale(sp50);
            i_this->mpModel->setBaseTRMtx(sp40);
            i_this->mBtk.entry(i_this->mpModel->getModelData());
            i_this->mBrk.entry(i_this->mpModel->getModelData());

            if (i_this->field_0x5cc >= 15) {
                i_this->mBtk.play();
                i_this->mBrk.play();
            }

            dComIfGd_setListDark();
            mDoExt_modelUpdateDL(i_this->mpModel);
            dComIfGd_setList();
        }
    }

    return 1;
}

static int daKytag04_Execute(kytag04_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp20;
    csXyz sp28;

    switch (i_this->field_0x5b4) {
    case 4:
        if (i_this->field_0x5d1 == 0 && i_this->field_0x5b5 != 0xFF) {
            s32 room_no = dComIfGp_roomControl_getStayNo();

            if (dComIfGs_isSwitch(i_this->field_0x5b5, room_no) && i_this->field_0x5d0 == 0) {
                if (dComIfGp_getEvent().isOrderOK() &&
                    fopAcM_orderOtherEvent(i_this, "WITHWARP_OPEN", 0xFFFF, 1, 0))
                {
                    i_this->field_0x5d1 = 1;
                    i_this->field_0x5d0 = 1;
                    i_this->field_0x5cc = 1;

                    mDoAud_seStart(Z2SE_WARP_HOLE_OPEN, NULL, 0, 0);
                }
            }
        }

        if (i_this->field_0x5d0 != 0) {
            if (i_this->field_0x5d0 == 10) {
                sp20 = a_this->current.pos;
                sp20.y = a_this->current.pos.y;

                if (i_this->mNeedDropNum == 0xFF) {
                    daNpcF_getPlayerInfoFromPlayerList(
                        i_this->mStageNo, dComIfGp_roomControl_getStayNo(), sp20, sp28);
                } else {
                    daNpcF_getPlayerInfoFromPlayerList(
                        i_this->mNeedDropNum, dComIfGp_roomControl_getStayNo(), sp20, sp28);
                }

                player->setPlayerPosAndAngle(&sp20, a_this->current.angle.y, 0);
                player->changeOriginalDemo();
                player->changeDemoMode(4, 0, 0, 0);
            }

            if (i_this->field_0x5d0 < 80) {
                i_this->field_0x5d0++;
                break;
            }

            if (strcmp(dComIfGp_getStartStageName(), "F_SP104") == 0) {
                dStage_changeScene(3, 0.0f, 0, dComIfGp_roomControl_getStayNo(), 0, -1);
            }

            i_this->field_0x5d0 = 0;
        }
        break;
    case 1:
        if (i_this->field_0x5b5 != 0xFF) {
            s32 room_no = dComIfGp_roomControl_getStayNo();

            if (dComIfGs_isSwitch(i_this->field_0x5b5, room_no) && i_this->field_0x5b7 != 0 &&
                i_this->field_0x5b7 != 1)
            {
                switch (i_this->field_0x5c4) {
                case 0:
                    if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                        dComIfGp_getEvent().reset(a_this);
                        fopAcM_orderPotentialEvent(a_this, 0x400, 0xFFFF, 0);
                    } else {
                        i_this->field_0x5c5 = 1;
                        i_this->field_0x5c0 = 10;
                        i_this->field_0x5c4++;
                    }
                    break;
                case 1:
                    if (i_this->field_0x5c0 <= 0) {
                        dStage_changeScene(i_this->mExitID, 0.0f, 0, i_this->current.roomNo, 0, -1);
                        i_this->field_0x5b7 = 1;
                    } else {
                        i_this->field_0x5c0--;
                    }
                    break;
                }
            }
        }
        break;
    }

    return 1;
}

static int daKytag04_IsDelete(kytag04_class* i_this) {
    return 1;
}

static int daKytag04_Delete(kytag04_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Kytag04");

    if (i_this->field_0x5a4 != NULL) {
        i_this->field_0x5a4->deleteAllParticle();
        i_this->field_0x5a4->becomeInvalidEmitter();
        i_this->field_0x5a4->quitImmortalEmitter();
        i_this->field_0x5a4->setEmitterCallBackPtr(NULL);
        i_this->field_0x5a4 = NULL;
    }

    if (i_this->field_0x5a8 != NULL) {
        i_this->field_0x5a8->deleteAllParticle();
        i_this->field_0x5a8->becomeInvalidEmitter();
        i_this->field_0x5a8->quitImmortalEmitter();
        i_this->field_0x5a8->setEmitterCallBackPtr(NULL);
        i_this->field_0x5a8 = NULL;
    }

    if (i_this->field_0x5ac != NULL) {
        i_this->field_0x5ac->deleteAllParticle();
        i_this->field_0x5ac->becomeInvalidEmitter();
        i_this->field_0x5ac->quitImmortalEmitter();
        i_this->field_0x5ac->setEmitterCallBackPtr(NULL);
        i_this->field_0x5ac = NULL;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    kytag04_class* a_this = (kytag04_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kytag04", 5);
    JUT_ASSERT(0, modelData != NULL);

    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11020202);
    if (a_this->mpModel != NULL) {
        J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kytag04", 14);
        if (!a_this->mBtk.init(modelData, pbtk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            OS_REPORT("\nkytag04 btk FALSE!!\n");
            return 0;
        }

        J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Kytag04", 8);
        if (!a_this->mBrk.init(modelData, pbrk, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1)) {
            OS_REPORT("\nkytag04 brk FALSE!!\n");
            return 0;
        }
    } else {
        return 0;
    }

    return 1;
}

static int daKytag04_Create(fopAc_ac_c* i_this) {
    kytag04_class* a_this = (kytag04_class*)i_this;
    fopAcM_ct(i_this, kytag04_class);

    a_this->mNeedDropNum = i_this->current.angle.z & 0xFF;

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "Kytag04");
    if (phase_state == cPhs_COMPLEATE_e) {
        a_this->field_0x5b4 = (fopAcM_GetParam(i_this) >> 8) & 7;
        a_this->mStageNo = (i_this->current.angle.z >> 8) & 0xFF;
        a_this->mExitID = fopAcM_GetParam(i_this) & 0xFF;
        a_this->mNeedDropNum = i_this->current.angle.z & 0xFF;
        a_this->field_0x5b5 = fopAcM_GetParam(i_this) >> 0x10;
        a_this->field_0x5b6 = fopAcM_GetParam(i_this) >> 0x18;

        if (strcmp(dComIfGp_getStartStageName(), "F_SP110") == 0 &&
            dComIfGp_getStartStageRoomNo() == 0 && dComIfG_play_c::getLayerNo(0) == 8)
        {
            a_this->field_0x5b5 = 0xFF;
        }

        a_this->field_0x5b7 = 0xFF;
        a_this->field_0x5b8 = 0;
        a_this->field_0x5b9 = 0;
        a_this->field_0x5c0 = 0;
        a_this->field_0x5c4 = 0;
        a_this->field_0x5c5 = 0;
        a_this->field_0x5c8 = 0.0f;
        a_this->field_0x5cc = 0;
        a_this->field_0x5d0 = 0;
        a_this->field_0x5d1 = 0;

        a_this->field_0x5a4 = NULL;
        a_this->field_0x5a8 = NULL;
        a_this->field_0x5ac = NULL;

        if (a_this->field_0x5b4 == 1) {
            // "\nArg2 is [%d]"
            OS_REPORT("\n引数２は[%d]", a_this->mNeedDropNum);
            dComIfGp_setNeedLightDropNum(a_this->mNeedDropNum);

            if (a_this->field_0x5b5 != 0xFF) {
                if (dComIfGs_isSwitch(a_this->field_0x5b5, dComIfGp_roomControl_getStayNo())) {
                    a_this->field_0x5b7 = 1;
                }
            }

            if (dKy_darkworld_check()) {
                if (dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea()) >=
                        dComIfGp_getNeedLightDropNum() &&
                    (dComIfG_play_c::getLayerNo(0) == 13 || dComIfG_play_c::getLayerNo(0) == 14))
                {
                    // "\nEmergency!! All drops collected but still in dark world, so force
                    // advance]"
                    OS_WARNING("\n緊急事態！！　雫がそろってるのにまだ闇世界なので強引に進めます]");
                    a_this->field_0x5b7 = -1;

                    dComIfGs_onSwitch(a_this->field_0x5b5, dComIfGp_roomControl_getStayNo());
                }
            }
        } else if (a_this->field_0x5b4 == 4) {
            if (a_this->field_0x5b5 != 0xFF) {
                if (dComIfGs_isSwitch(a_this->field_0x5b5, dComIfGp_roomControl_getStayNo())) {
                    a_this->field_0x5d1 = 1;
                    a_this->field_0x5c8 = 999.0f;
                    a_this->field_0x5cc = 20;
                }
            }
        } else if (a_this->field_0x5b4 == 5) {
            if (a_this->field_0x5b5 != 0xFF) {
                if (dComIfGs_isSaveSwitch(a_this->mStageNo, a_this->field_0x5b5)) {
                    a_this->field_0x5d1 = 1;
                    a_this->field_0x5c8 = 999.0f;
                    a_this->field_0x5cc = 20;
                }
            }
        }

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x1150)) {
            return cPhs_ERROR_e;
        }

        if (a_this->field_0x5b4 == 4 && a_this->field_0x5b5 != 0xFF) {
            if (dComIfGs_isSwitch(a_this->field_0x5b5, dComIfGp_roomControl_getStayNo())) {
                a_this->mBtk.setFrame(a_this->mBtk.getEndFrame());
                a_this->mBrk.setFrame(a_this->mBrk.getEndFrame());
            }
        }

        if (a_this->field_0x5b4 == 5 && a_this->field_0x5b5 != 0xFF) {
            if (dComIfGs_isSaveSwitch(a_this->mStageNo, a_this->field_0x5b5)) {
                a_this->mBtk.setFrame(a_this->mBtk.getEndFrame());
                a_this->mBrk.setFrame(a_this->mBrk.getEndFrame());
            }
        }
    }

    return phase_state;
}

static actor_method_class l_daKytag04_Method = {
    (process_method_func)daKytag04_Create,  (process_method_func)daKytag04_Delete,
    (process_method_func)daKytag04_Execute, (process_method_func)daKytag04_IsDelete,
    (process_method_func)daKytag04_Draw,
};

extern actor_process_profile_definition g_profile_KYTAG04 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG04,
    &g_fpcLf_Method.base,
    sizeof(kytag04_class),
    0,
    0,
    &g_fopAc_Method.base,
    98,
    &l_daKytag04_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
