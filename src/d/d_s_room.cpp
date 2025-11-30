/**
 * d_s_room.cpp
 * Gameplay Room Scene
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_s_room.h"
#include "m_Do/m_Do_Reset.h"
#include "stdio.h"

static int dScnRoom_Draw(room_of_scene_class* i_this) {
    return 1;
}

static int getResetArchiveBank(int param_1, u8 const** param_2) {
    dStage_roomControl_c::nameData* arcBankName = dStage_roomControl_c::getArcBankName();
    int local_30 = 0;

    *param_2 = NULL;
    if (arcBankName != NULL) {
        dStage_roomControl_c::bankData* arcBankData = dStage_roomControl_c::getArcBankData();
        dStage_roomControl_c::bankDataEntry* entries = arcBankData->m_entries;
        for (int i = 0; i < arcBankData->m_num; entries++, i++) {
            if (entries->field_0x0 == param_1) {
                if (entries->mLayerNo == dComIfG_play_c::getLayerNo(0)) {
                    local_30 = 0x20;
                    *param_2 = entries->field_0x2;
                    break;
                }
            }
        }
    }

    u8 const* pbVar1 = *param_2;
    for (int i = 0; i < local_30; pbVar1++, i++) {
        char* arcBank = dStage_roomControl_c::getArcBank(i);
        char* name = "";

        int id = *pbVar1;
        if (id != 0xff) {
            JUT_ASSERT(160, 0 <= id && id < arcBankName->m_num);
            name = arcBankName->m_names[id];
        }

        if (strcmp(arcBank, name) != 0) {
            return i;
        }
    }

    return local_30;
}

static bool resetArchiveBank(int param_0) {
    const u8* tmp;
    return dStage_roomControl_c::resetArchiveBank(getResetArchiveBank(param_0, &tmp));
}

static bool setArchiveBank(int param_0) {
    u8* arr[4];
    int bank_no = getResetArchiveBank(param_0, (u8 const**)&arr);
    if (arr[0] == NULL) {
        return true;
    }

    dStage_roomControl_c::nameData* arcBankName = dStage_roomControl_c::getArcBankName();
    for (; (int)bank_no < 0x20; bank_no++) {
        const char* name = "";
        int id = arr[0][bank_no];
        if (id != 0xff) {
            JUT_ASSERT(216, 0 <= id && id < arcBankName->m_num);
            name = arcBankName->m_names[id];
        }

        if (strcmp(name, "") != 0) {
            if (strnicmp(name, "pack", 4) == 0) {
                int syncres = dComIfG_syncObjectRes(name);
                if (syncres < 0) {
                    if (!dComIfG_setObjectRes(name, 0, mDoExt_getArchiveHeap())) {
                        OSReport_Error("Bank[%d] : %s.arc Read Error !!\n", bank_no, name);
                    } else {
                        return false;
                    }
                } else if (syncres != 0) {
                    return false;
                }
            } else {
                if (!dComIfG_setObjectRes(name, 0, mDoExt_getArchiveHeap())) {
                    OSReport_Error("Bank[%d] : %s.arc Read Error !!\n", bank_no, name);
                }
            }
        }

        dStage_roomControl_c::setArcBank(bank_no, name);
    }

    return true;
}

static int objectDeleteJugge(void* i_obj, void*) {
    if (fpcM_GetProfName(i_obj) != PROC_BG) {
        if (fopAcM_IsActor(i_obj) && !fopAcM_CheckCondition((fopAc_ac_c*)i_obj, 4)) {
            return 0;
        }
        fpcM_Delete(i_obj);
    }

    return 0;
}

static int deleteJugge(void* i_obj, void*) {
    fpcM_Delete(i_obj);
    return 0;
}

static void* isCreatingCallback(create_tag* param_1, fpc_ProcID* param_2) {
    create_request* create_req = (create_request*)param_1->base.mpTagData;
    if (create_req->layer->layer_id == *param_2) {
        return create_req;
    }

    return NULL;
}

static bool isCreating(fpc_ProcID param_0) {
    return fpcCtIt_Judge((fpcCtIt_JudgeFunc)isCreatingCallback, &param_0);
}

static int loadDemoArchive(int room_no) {
    if (*dStage_roomControl_c::getDemoArcName() == 0) {
        dStage_Lbnk_c* lbnk = dComIfGp_roomControl_getStatusRoomDt(room_no)->getLbnk();

        if (lbnk != NULL) {
            dStage_Lbnk_dt_c* entries = lbnk->entries;

            if (entries != NULL) {
                int bank = entries[dComIfG_play_c::getLayerNo(room_no)].bank;

                if (bank != 0xff) {
                    JUT_ASSERT(350, 0 <= bank && bank < 100);
                    u8 bank2 = entries[dComIfG_play_c::getLayerNo(room_no)].bank2;
                    JUT_ASSERT(353, 0 <= bank2 && bank2 < 100);

                    sprintf(dStage_roomControl_c::getDemoArcName(), "Demo%02d_%02d", bank, bank2);
                    if (!dComIfG_setObjectRes(dStage_roomControl_c::getDemoArcName(), 0,
                                              (JKRHeap*)NULL))
                    {
                        *dStage_roomControl_c::getDemoArcName() = 0;
                    } else {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

static bool objectSetCheck(room_of_scene_class* i_this) {
    int roomNo = fopScnM_GetParam(i_this);
    BOOL status_flag_8 = dComIfGp_roomControl_checkStatusFlag(roomNo, 8);
    BOOL status_flag_20 = dComIfGp_roomControl_checkStatusFlag(roomNo, 0x20);

    if (i_this->field_0x1d4 == 0 || (i_this->field_0x1d4 > 0 && !status_flag_8)) {
        if (!status_flag_8) {
            switch (i_this->field_0x1d4) {
            case 0:
                if (!resetArchiveBank(roomNo)) {
                    return 0;
                }
                i_this->field_0x1d4++;
            case 1:
                if (!setArchiveBank(roomNo)) {
                    return 0;
                }

                if (i_this->mpDzrRes != NULL) {
                    loadDemoArchive(roomNo);
                }
            default:
                if (*dStage_roomControl_c::getDemoArcName() != '\0') {
                    int phase = dComIfG_syncObjectRes(dStage_roomControl_c::getDemoArcName());

                    if (phase < 0) {
                        #if VERSION == VERSION_WII_USA_R0
                        dStage_escapeRestart();
                        #endif
                    } else if (phase > 0) {
                        return 0;
                    }
                }

                fopAcM_create(PROC_BG, roomNo, NULL, -1, NULL, NULL, -1);
                dComIfGp_getPEvtManager()->demoInit();
                dComIfGp_getPEvtManager()->roomInit(roomNo);
                dStage_dt_c_roomReLoader(i_this->mpDzrRes, i_this->mpRoomDt, roomNo);
                dComIfGp_ret_wp_set(roomNo);
                i_this->field_0x1d4 = -1;
                i_this->field_0x1d5 = 1;
            }
        }
    } else if (status_flag_8) {
        if (isCreating(fpcM_LayerID(i_this))) {
            return 0;
        }

        fpcM_LyJudge(&i_this->base, (fpcLyIt_JudgeFunc)deleteJugge, NULL);
        g_dComIfG_gameInfo.play.getParticle()->levelAllForceOnEventMove();
        dComIfGs_clearRoomSwitch(dComIfGp_roomControl_getZoneNo(roomNo));
        dComIfGs_clearRoomItem(dComIfGp_roomControl_getZoneNo(roomNo));
        dComIfGp_roomControl_offStatusFlag(roomNo, 0x20);
        i_this->field_0x1d4 = 0;
    } else if (status_flag_20) {
        fpcM_LyJudge(&i_this->base, (fpcLyIt_JudgeFunc)objectDeleteJugge, NULL);
        g_dComIfG_gameInfo.play.getParticle()->levelAllForceOnEventMove();
    }

    return 1;
}

static int dScnRoom_Execute(room_of_scene_class* i_this) {
    int roomNo = fopScnM_GetParam(i_this);

    u32 flag = g_dComIfG_gameInfo.play.getRoomControl()->mStatus[roomNo].mFlag;
    if (flag & 2) {
        dComIfGp_roomControl_setStatusFlag(roomNo, flag & ~2);
        dComIfGp_roomControl_onStatusFlag(roomNo, 1);
    } else if (objectSetCheck(i_this)) {
        if (dComIfGp_roomControl_checkStatusFlag(roomNo, 4)) {
            fopScnM_DeleteReq(i_this);
        } else if (i_this->field_0x1d4 < 0 && i_this->field_0x1d5 != 0 &&
                   !dComIfGp_event_runCheck() &&
                   (int)fopScnM_GetParam(i_this) == dComIfGp_roomControl_getStayNo())
        {
            if (isCreating(fpcM_LayerID(i_this))) {
                dScnPly_c::nextPauseTimer = 2;
            } else {
                i_this->field_0x1d5 = 0;
            }
        }
    }

    return 1;
}

static int dScnRoom_IsDelete(room_of_scene_class*) {
    return 1;
}

static bool isReadRoom(int param_0) {
    roomRead_class* room = dComIfGp_getStageRoom();

    if (room == NULL) {
        return false;
    }

    if (room->num <= param_0) {
        return false;
    }

    roomRead_data_class* roomData = room->m_entries[dComIfGp_getNextStageRoomNo()];
    u8* tmp = roomData->m_rooms;

    for (int i = 0; i < roomData->num; i++) {
        if (dStage_roomRead_dt_c_ChkBg(*tmp) &&
            param_0 == dStage_roomRead_dt_c_GetLoadRoomIndex(*tmp))
        {
            return true;
        }
        tmp++;
    }

    return false;
}

inline const char* setArcName(room_of_scene_class* i_room) {
    return dComIfG_getRoomArcName(fopAcM_GetParam(i_room));
}

static int dScnRoom_Delete(room_of_scene_class* i_this) {
    int roomNo = fopScnM_GetParam(i_this);
    dComIfGp_roomControl_setStatusFlag(roomNo, 0);
    dComIfGp_roomControl_getStatusRoomDt(roomNo)->init();

    if (mDoRst::isReset() || !dComIfGp_isEnableNextStage() ||
        strcmp(dComIfGp_getNextStageName(), dComIfGp_getStartStageName()) ||
        (roomNo != dComIfGp_getNextStageRoomNo() && !isReadRoom(roomNo)))
    {
        dComIfG_deleteStageRes(setArcName(i_this));

        JKRExpHeap* heap = dStage_roomControl_c::getMemoryBlock(roomNo);
        if (heap != NULL) {
            heap->freeAll();
        }
    }

    dComIfGp_roomControl_getStatusRoomDt(roomNo)->init();
    dComIfGp_getPEvtManager()->roomFinish(roomNo);
    return 1;
}

static int phase_0(room_of_scene_class* i_this) {
    int roomNo = fopScnM_GetParam(i_this);
    dStage_roomControl_c::setStatusProcID(roomNo, fopScnM_GetID(i_this));
    return cPhs_NEXT_e;
}

static int phase_1(room_of_scene_class* i_this) {
    i_this->field_0x1d8 = 1;

    int roomNo = fopScnM_GetParam(i_this);
    const char* arcName = setArcName(i_this);

    if (dComIfG_syncStageRes(arcName) < 0) {
        JKRExpHeap* heap = dStage_roomControl_c::getMemoryBlock(roomNo);

        if (heap != NULL) {
            if (heap->getTotalUsedSize() != 0) {
                return cPhs_INIT_e;
            }
        } else {
            stage_stag_info_class* stagInfo = dComIfGp_getStage()->getStagInfo();

            if (dStage_staginfo_GetArchiveHeap(stagInfo) != NULL) {
                heap = mDoExt_getArchiveHeap();
            }
        }

        if (!dComIfG_setStageRes(arcName, heap)) {
            #if VERSION == VERSION_WII_USA_R0
            dStage_escapeRestart();
            #endif
            return cPhs_ERROR_e;
        }
    }

    return cPhs_NEXT_e;
}

static int phase_2(room_of_scene_class* i_this) {
    const char* arcName = setArcName(i_this);
    int phase = dComIfG_syncStageRes(arcName);

    if (phase < 0) {
        #if VERSION == VERSION_WII_USA_R0
        dStage_escapeRestart();
        #endif
        return cPhs_ERROR_e;
    }

    if (phase != 0) {
        return cPhs_INIT_e;
    }

    int roomNo = fopScnM_GetParam(i_this);
    if (dComIfGp_roomControl_getZoneNo(roomNo) < 0) {
        dComIfGp_roomControl_setZoneNo(roomNo, dComIfGs_createZone(roomNo));
    }

    i_this->mpRoomDt = dComIfGp_roomControl_getStatusRoomDt(roomNo);
    i_this->mpRoomDt->setRoomNo(roomNo);
    i_this->mpDzrRes = dComIfG_getStageRes(arcName, "room.dzr");

    if (i_this->mpDzrRes != NULL) {
        dStage_dt_c_roomLoader(i_this->mpDzrRes, i_this->mpRoomDt, roomNo);
    }

    JKRHeap* old_heap = NULL;
    JKRExpHeap* heap = dStage_roomControl_c::getMemoryBlock(roomNo);

    if (heap != NULL) {
        old_heap = mDoExt_setCurrentHeap(heap);
    }

    if (old_heap != NULL) {
        mDoExt_setCurrentHeap(old_heap);
    }

    return cPhs_NEXT_e;
}

void dStage_roomControl_c::setZoneNo(int i_roomNo, int i_zoneNo) {
    mStatus[i_roomNo].mZoneNo = i_zoneNo;
}

static int phase_3(room_of_scene_class* i_this) {
    if (objectSetCheck(i_this)) {
        return cPhs_NEXT_e;
    } else {
        return cPhs_INIT_e;
    }
}

static int phase_4(room_of_scene_class* i_this) {
    if (dComIfGp_getPlayer(0) == NULL) {
        return cPhs_INIT_e;
    }

    if (objectSetCheck(i_this)) {
        return cPhs_COMPLEATE_e;
    } else {
        return cPhs_INIT_e;
    }
}

static int dScnRoom_Create(scene_class* i_this) {
    static int (*l_method[5])(void*) = {
        (int (*)(void*))phase_0, (int (*)(void*))phase_1, (int (*)(void*))phase_2,
        (int (*)(void*))phase_3, (int (*)(void*))phase_4,
    };

    room_of_scene_class* room = static_cast<room_of_scene_class*>(i_this);
    return dComLbG_PhaseHandler(&room->field_0x1c4, l_method, i_this);
}

static leafdraw_method_class l_dScnRoom_Method = {
    (process_method_func)dScnRoom_Create,  (process_method_func)dScnRoom_Delete,
    (process_method_func)dScnRoom_Execute, (process_method_func)dScnRoom_IsDelete,
    (process_method_func)dScnRoom_Draw,
};

scene_process_profile_definition g_profile_ROOM_SCENE = {
    fpcLy_CURRENT_e,                            // mLayerID
    0,                                          // mListID
    fpcPi_CURRENT_e,                            // mListPrio
    PROC_ROOM_SCENE,                            // mProcName
    &g_fpcNd_Method.base,                      // sub_method
    sizeof(room_of_scene_class),                // mSize
    0,                                          // mSizeOther
    0,                                          // mParameters
    &g_fopScn_Method.base,                     // sub_method
    (process_method_class*)&l_dScnRoom_Method,  // mpMtd
};
