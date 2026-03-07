/**
 * d_s_room.cpp
 * Gameplay Room Scene
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_s_room.h"
#include "d/d_bg_parts.h"
#include "m_Do/m_Do_Reset.h"
#include "f_ap/f_ap_game.h"

#include <cstdio>
#include <cstring>
#ifndef __MWERKS__
#include "dusk/extras.h"
#endif

static int dScnRoom_Draw(room_of_scene_class* i_this) {
    fpc_ProcID id = fpcM_GetID(i_this);
    return 1;
}

static int getResetArchiveBank(int i_roomNo, const u8** o_bankIDList) {
    dStage_roomControl_c::dStage_bankName* arcBankName = dStage_roomControl_c::getArcBankName();
    int bankNum = 0;

    *o_bankIDList = NULL;

    if (arcBankName != NULL) {
        dStage_roomControl_c::dStage_bankData* arcBankData = dStage_roomControl_c::getArcBankData();
        dStage_roomControl_c::dStage_bankDataEntry* entries = arcBankData->m_entries;
        for (int i = 0; i < arcBankData->m_num; entries++, i++) {
            if (entries->roomNo == i_roomNo) {
                if (entries->layerNo == dComIfG_play_c::getLayerNo(0)) {
                    bankNum = 32;
                    *o_bankIDList = entries->nameIDs;
                    break;
                }
            }
        }
    }

    const u8* pbank = *o_bankIDList;
    for (int i = 0; i < bankNum; pbank++, i++) {
        char* arcBank = dStage_roomControl_c::getArcBank(i);
        char* name = "";

        int id = *pbank;
        if (id != 0xFF) {
            JUT_ASSERT(160, 0 <= id && id < arcBankName->m_num);
            name = arcBankName->m_names[id];
        }

        if (strcmp(arcBank, name) != 0) {
            #if DEBUG
            if (strcmp(arcBank, "") != 0) {
                // "archive has been deleted from bank! <%d:%s!=%s>"
                OSReport_Warning("バンクから削除するアーカイブが発生しました！ <%d:%s!=%s>\n", i, arcBank, name);
            }
            #endif

            return i;
        }
    }
    
    return bankNum;
}

static bool resetArchiveBank(int i_roomNo) {
    const u8* bankIDList;
    return dStage_roomControl_c::resetArchiveBank(getResetArchiveBank(i_roomNo, &bankIDList));
}

static bool setArchiveBank(int i_roomNo) {
    u8* bankIDList;
    int bankNo = getResetArchiveBank(i_roomNo, (const u8**)&bankIDList);
    if (bankIDList == NULL) {
        return true;
    }

    dStage_roomControl_c::dStage_bankName* arcBankName = dStage_roomControl_c::getArcBankName();
    for (; bankNo < 32; bankNo++) {
        const char* name = "";
        int id = bankIDList[bankNo];
        if (id != 0xFF) {
            JUT_ASSERT(216, 0 <= id && id < arcBankName->m_num);
            name = arcBankName->m_names[id];
        }

        if (strcmp(name, "") != 0) {
            if (strnicmp(name, "pack", 4) == 0) {
                #if DEBUG
                if (fapGm_HIO_c::mPackArchiveMode)
                #endif
                {
                    int rt = dComIfG_syncObjectRes(name);
                    if (rt < 0) {
                        if (!dComIfG_setObjectRes(name, 0, mDoExt_getArchiveHeap())) {
                            OSReport_Error("Bank[%d] : %s.arc Read Error !!\n", bankNo, name);
                        } else {
                            return false;
                        }
                    } else if (rt != 0) {
                        return false;
                    }
                }
            } else {
                if (!dComIfG_setObjectRes(name, 0, mDoExt_getArchiveHeap())) {
                    OSReport_Error("Bank[%d] : %s.arc Read Error !!\n", bankNo, name);
                }
            }
        }

        OS_REPORT(">>>>>>>>>>> create Bank[%d] : %s\n", bankNo, name);
        dStage_roomControl_c::setArcBank(bankNo, name);
    }

    return true;
}

static int objectDeleteJugge(void* i_process, void* i_data) {
    if (fpcM_GetProfName(i_process) != PROC_BG) {
        if (fopAcM_IsActor(i_process)) {
            #if DEBUG
            char namebuf[16];
            fopAcM_getNameString((fopAc_ac_c*)i_process, namebuf);
            #endif

            if (!fopAcM_CheckCondition((fopAc_ac_c*)i_process, fopAcCnd_NODRAW_e)) {
                return 0;
            }
            
            OS_REPORT("削除！<%s>\n", namebuf);
        }

        fpcM_Delete(i_process);
    }

    return 0;
}

static int deleteJugge(void* i_process, void* i_data) {
    fpcM_Delete(i_process);
    return 0;
}

static void* isCreatingCallback(create_tag* i_createTag, fpc_ProcID* i_procId) {
    create_request* create_req = (create_request*)i_createTag->base.mpTagData;
    if (create_req->layer->layer_id == *i_procId) {
        return create_req;
    }

    return NULL;
}

static u8 isCreating(fpc_ProcID i_id) {
    if (fpcCtIt_Judge((fpcCtIt_JudgeFunc)isCreatingCallback, &i_id)) {
        return TRUE;
    }

    return FALSE;
}

static int loadDemoArchive(int i_roomNo) {
    if (*dStage_roomControl_c::getDemoArcName() == 0) {
        dStage_Lbnk_c* lbnk = dComIfGp_roomControl_getStatusRoomDt(i_roomNo)->getLbnk();

        if (lbnk != NULL) {
            dStage_Lbnk_dt_c* entries = lbnk->entries;
            if (entries != NULL) {
                int bank = entries[dComIfG_play_c::getLayerNo(i_roomNo)].bank;
                if (bank != 0xFF) {
                    JUT_ASSERT(350, 0 <= bank && bank < 100);
                    int bank2 = entries[dComIfG_play_c::getLayerNo(i_roomNo)].bank2;
                    JUT_ASSERT(353, 0 <= bank2 && bank2 < 100);

                    sprintf(dStage_roomControl_c::getDemoArcName(), "Demo%02d_%02d", bank, bank2);
                    if (!dComIfG_setObjectRes(dStage_roomControl_c::getDemoArcName(), 0, (JKRHeap*)NULL)) {
                        const char* name = dStage_roomControl_c::getDemoArcName();
                        *dStage_roomControl_c::getDemoArcName() = 0;
                    } else {
                        // "Loading demo archive <%s>"
                        OS_REPORT("デモアーカイブ読み込み<%s>\n", dStage_roomControl_c::getDemoArcName());
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

                if (i_this->roomInfo != NULL) {
                    loadDemoArchive(roomNo);
                }
                break;
            default:
                JUT_ASSERT(417, 0);
            }

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
            dStage_dt_c_roomReLoader(i_this->roomInfo, i_this->roomDt, roomNo);
            dComIfGp_ret_wp_set(roomNo);
            i_this->field_0x1d4 = -1;
            i_this->field_0x1d5 = 1;
        }
    } else if (status_flag_8) {
        if (isCreating(fpcM_LayerID(i_this))) {
            return 0;
        }

        fpcM_LyJudge(&i_this->base, (fpcLyIt_JudgeFunc)deleteJugge, NULL);
        g_dComIfG_gameInfo.play.getParticle()->levelAllForceOnEventMove();

        // "Object deleted! <%d>"
        OS_REPORT("\x1b[32mオブジェクト削除！<%d>\n\x1b[m", roomNo);

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

    if (dComIfGp_roomControl_checkStatusFlag(roomNo, 2)) {
        dComIfGp_roomControl_offStatusFlag(roomNo, 2);
        dComIfGp_roomControl_onStatusFlag(roomNo, 1);
    } else if (objectSetCheck(i_this)) {
        if (dComIfGp_roomControl_checkStatusFlag(roomNo, 4)) {
            fopScnM_DeleteReq(i_this);
        } else if (i_this->field_0x1d4 < 0 && i_this->field_0x1d5 != 0 && !dComIfGp_event_runCheck()) {
            int param = fopScnM_GetParam(i_this);
            if (param == dComIfGp_roomControl_getStayNo()) {
                if (isCreating(fpcM_LayerID(i_this))) {
                    dScnPly_c::setPauseTimer(2);
                } else {
                    i_this->field_0x1d5 = 0;
                } 
            }
        }
    }

    return 1;
}

static int dScnRoom_IsDelete(room_of_scene_class*) {
    return 1;
}

static bool isReadRoom(int i_roomNo) {
    roomRead_class* room = dComIfGp_getStageRoom();
    if (room == NULL) {
        return false;
    }
    if (room->num <= i_roomNo) {
        return false;
    }

    JUT_ASSERT(589, 0 <= dComIfGp_getNextStageRoomNo() && dComIfGp_getNextStageRoomNo() < 64);

    roomRead_data_class* roomData = room->m_entries[dComIfGp_getNextStageRoomNo()];
    u8* room_data = roomData->m_rooms;

    for (int i = 0; i < roomData->num; i++) {
        if (dStage_roomRead_dt_c_ChkBg(*room_data) &&
            i_roomNo == dStage_roomRead_dt_c_GetLoadRoomIndex(*room_data))
        {
            return true;
        }
        room_data++;
    }

    return false;
}

inline const char* setArcName(room_of_scene_class* i_room) {
    return dComIfG_getRoomArcName(fopAcM_GetParam(i_room));
}

static int dScnRoom_Delete(room_of_scene_class* i_this) {
    OS_REPORT("dScnRoom_Delete():room%d\n", fopScnM_GetParam(i_this));

    int roomNo = fopScnM_GetParam(i_this);

    #if DEBUG
    dBgp_c* bgp = dStage_roomControl_c::getBgp(roomNo);
    if (bgp != NULL) {
        if (!bgp->remove()) {
            return 0;
        }

        delete bgp;
        dStage_roomControl_c::setBgp(roomNo, NULL);
    }
    #endif

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
    } else {
        // "Room(%d) data saved!"
        OS_REPORT(">>> 部屋(%d)データ保持！\n", roomNo);
    }

    dComIfGp_roomControl_getStatusRoomDt(roomNo)->init();
    dComIfGp_getPEvtManager()->roomFinish(roomNo);
    return 1;
}

static int phase_0(room_of_scene_class* i_this) {
    int param = fopScnM_GetParam(i_this);
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
            OS_REPORT("#######<%d>\n", heap->getTotalUsedSize());

            if (heap->getTotalUsedSize() != 0) {
                return cPhs_INIT_e;
            }
        } else {
            if (dStage_staginfo_GetArchiveHeap(dComIfGp_getStage()->getStagInfo()) != FALSE) {
                heap = mDoExt_getArchiveHeap();
            }
        }

        int rt = dComIfG_setStageRes(arcName, heap);
        if (!rt) {
            //! In Wii USA Revision 0, if a stage's resources fail to load, the stage will restart as a failsafe.
            //! In later versions this failsafe was removed, and the room will simply not load.
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

    int rt = dComIfG_syncStageRes(arcName);
    if (rt < 0) {
        //! In Wii USA Revision 0, if a stage's resources fail to load, the stage will restart as a failsafe.
        //! In later versions this failsafe was removed, and the room will simply not load.
        #if VERSION == VERSION_WII_USA_R0
        dStage_escapeRestart();
        #endif
        return cPhs_ERROR_e;
    }

    JUT_ASSERT(750, rt >= 0);
    if (rt != 0) {
        return cPhs_INIT_e;
    }

    int roomNo = fopScnM_GetParam(i_this);

    #if DEBUG
    JKRExpHeap* block = dStage_roomControl_c::getMemoryBlock(roomNo);
    OS_REPORT("*************************************** room%d <%x> <%x : %x>\n",
                roomNo,
                block,
                block != NULL ? block->getTotalFreeSize() : 0,
                block != NULL ? block->getFreeSize() : 0
                );
    #endif

    if (dComIfGp_roomControl_getZoneNo(roomNo) < 0) {
        dComIfGp_roomControl_setZoneNo(roomNo, dComIfGs_createZone(roomNo));
    }

    i_this->roomDt = dComIfGp_roomControl_getStatusRoomDt(roomNo);
    i_this->roomDt->setRoomNo(roomNo);
    i_this->roomInfo = dComIfG_getStageRes(arcName, "room.dzr");

    if (i_this->roomInfo != NULL) {
        dStage_dt_c_roomLoader(i_this->roomInfo, i_this->roomDt, roomNo);
    }

    JKRHeap* old_heap = NULL;
    JKRExpHeap* heap = dStage_roomControl_c::getMemoryBlock(roomNo);

    if (heap != NULL) {
        old_heap = mDoExt_setCurrentHeap(heap);
    }

    #if DEBUG
    void* unit = dComIfGp_roomControl_getStatusRoomDt(roomNo)->getUnit();
    if (unit != NULL) {
        dBgp_c* bgp = new dBgp_c();
        JUT_ASSERT(786, bgp != NULL);
        
        bgp->create(roomNo, unit);
        dStage_roomControl_c::setBgp(roomNo, bgp);
    }
    #endif

    if (old_heap != NULL) {
        mDoExt_setCurrentHeap(old_heap);
    }

    return cPhs_NEXT_e;
}

static int phase_3(room_of_scene_class* i_this) {
    if (!objectSetCheck(i_this)) {
        return cPhs_INIT_e;
    } else {
        return cPhs_NEXT_e;
    }
}

static int phase_4(room_of_scene_class* i_this) {
    if (dComIfGp_getPlayer(0) == NULL) {
        return cPhs_INIT_e;
    }

    if (!objectSetCheck(i_this)) {
        return cPhs_INIT_e;
    }

    OS_REPORT("dScnRoom_Create(): End !! room%d\n", fopScnM_GetParam(i_this));
    return cPhs_COMPLEATE_e;
}

static int dScnRoom_Create(scene_class* i_this) {
    static request_of_phase_process_fn l_method[5] = {
        (request_of_phase_process_fn)phase_0,
        (request_of_phase_process_fn)phase_1,
        (request_of_phase_process_fn)phase_2,
        (request_of_phase_process_fn)phase_3,
        (request_of_phase_process_fn)phase_4,
    };

    room_of_scene_class* room = static_cast<room_of_scene_class*>(i_this);
    return dComLbG_PhaseHandler(&room->phase, l_method, i_this);
}

static scene_method_class l_dScnRoom_Method = {
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
    &l_dScnRoom_Method,  // mpMtd
};
