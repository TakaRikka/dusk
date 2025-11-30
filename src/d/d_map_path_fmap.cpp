#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_com_inf_game.h"
#include "d/d_map_path_fmap.h"

void fmpTresTypeGroupDataList_c::addTypeGroupData(u8 i_typeGroupNo,
                                                  dTres_c::data_s const* i_tresData) {
    fmpTresTypeGroupData_c* next = mpNextData;
    fmpTresTypeGroupData_c* fmpTresTypeGroupData_p = new fmpTresTypeGroupData_c();

    if (mpTypeGroupDataHead == NULL) {
        mpTypeGroupDataHead = fmpTresTypeGroupData_p;
    }

    fmpTresTypeGroupData_p->setTresData(i_tresData);
    fmpTresTypeGroupData_p->setNextData(NULL);
    fmpTresTypeGroupData_p->setTypeGroupNo(i_typeGroupNo);

    if (next != NULL) {
        next->setNextData(fmpTresTypeGroupData_p);
    }

    mpNextData = fmpTresTypeGroupData_p;
}

bool dMenu_Fmap_room_data_c::isArrival() {
    u8 table_no = mp_parentStage->getStageArc()->mVisitedRoomSaveTableNo;
    return dComIfGs_isSaveVisitedRoom(table_no, m_roomNo) != false;
}

void dMenu_Fmap_room_data_c::buildTresTypeGroup(int i_stageNo, int i_roomIndex, int i_roomNo) {
    dTres_c::list_class* tresure_p = mp_fmapData->getTresure();
    if (tresure_p != NULL) {
        int num = tresure_p->field_0x0;
        dTres_c::data_s* data_p = tresure_p->field_0x4->getDataPointer();

        if (mp_fmpTresTypeGroupDataListAll == NULL) {
            mp_fmpTresTypeGroupDataListAll = new fmpTresTypeGroupDataListAll_c();
        }

        for (int i = 0; i < num; i++) {
            data_p->mRoomNo = i_roomNo;
            u8 typeGroupNo = dTres_c::getTypeToTypeGroupNo(data_p->mType);
            mp_fmpTresTypeGroupDataListAll->addTypeGroupData(typeGroupNo, data_p);
            data_p++;
        }
    }
}

void dMenu_Fmap_room_data_c::buildFmapRoomData(int i_stageNo, int i_index, f32 i_regionOffsetX,
                                               f32 i_regionOffsetZ, f32 i_stageOffsetX,
                                               f32 i_stageOffsetZ) {
    int roomNo = m_roomNo;

    if (mp_fmapData != NULL) {
        buildTresTypeGroup(i_stageNo, i_index, roomNo);
    }
}

bool dMenu_Fmap_stage_data_c::isArrival() {
    bool is_arrival = false;
    dMenu_Fmap_room_data_c* room = mp_roomTop;
    int table_no = mpStageArc->getVisitedRoomSaveTableNo();
    for (; !is_arrival && room != NULL; room = room->getNextData()) {
        // fakematch: should be room->getRoomNo()
        is_arrival = dComIfGs_isSaveVisitedRoom(table_no, room->m_roomNo) != false;
    }

    return is_arrival;
}

int dMenu_Fmap_stage_data_c::buildFmapStageData(int i_stageNo, f32 i_regionOffsetX,
                                                f32 i_regionOffsetZ) {
    dMenu_Fmap_room_data_c* room = mp_roomTop;
    f32 stage_min_x = FLT_MAX;
    f32 stage_min_z = FLT_MAX;
    f32 stage_max_x = -FLT_MAX;
    f32 stage_max_z = -FLT_MAX;
    int room_no = 0;

    for (; room != NULL; room_no++, room = room->getNextData()) {
        room->buildFmapRoomData(i_stageNo, room_no, i_regionOffsetX, i_regionOffsetZ,
                                m_offsetX, m_offsetZ);
        f32 room_min_x = room->getFileList2MinX();
        f32 room_min_z = room->getFileList2MinZ();
        f32 room_max_x = room->getFileList2MaxX();
        f32 room_max_z = room->getFileList2MaxZ();

        if (room_min_x < stage_min_x) {
            stage_min_x = room_min_x;
        }

        if (room_min_z < stage_min_z) {
            stage_min_z = room_min_z;
        }

        if (room_max_x > stage_max_x) {
            stage_max_x = room_max_x;
        }

        if (room_max_z > stage_max_z) {
            stage_max_z = room_max_z;
        }
    }

    if (room_no > 0) {
        m_stageMinX = stage_min_x;
        m_stageMinZ = stage_min_z;
        m_stageMaxX = stage_max_x;
        m_stageMaxZ = stage_max_z;
    } else {
        m_stageMinX = 0.0f;
        m_stageMinZ = 0.0f;
        m_stageMaxX = 0.0f;
        m_stageMaxZ = 0.0f;
    }

    return room_no;
}

dMenu_Fmap_stage_data_c* dMenu_Fmap_region_data_c::getMenuFmapStageData(int param_0) {
    dMenu_Fmap_stage_data_c* stage = mpMenuFmapStageDataTop;
    for (int i = 0; stage != NULL && i < param_0; i++) {
        stage = stage->getNextData();
    }

    return stage;
}

int dMenu_Fmap_region_data_c::getPointStagePathInnerNo(f32 i_pointX, f32 i_pointZ, int i_stageNo,
                                                       int* o_stageNo, int* o_roomNo) {
    int line_num, group_num, floor_num;
    f32* point;
    u16* point_index;
    dDrawPath_c::line_class* line;
    dDrawPath_c::group_class* group;
    dDrawPath_c::floor_class* floor;
    f32* points;
    dMenu_Fmap_room_data_c* room;
    dMenu_Fmap_stage_data_c* stage;
    int point_num, point_no;
    int stage_no = 0;
    bool outside, found;
    int floor_no, group_no, line_no;
    int found_stage_no = -1;
    int dir, prev_dir;
    int found_room_no = -1;
    cXyz vec1, vec2, cross;
    vec1.y = 0.0f;
    vec2.y = 0.0f;

    stage = mpMenuFmapStageDataTop;
    for(; stage != NULL; stage = stage->getNextData(), stage_no++) {
        if (stage->getStageArc() == NULL) continue;
        int save_table_no = stage->getStageArc()->getVisitedRoomSaveTableNo();

        room = stage->getFmapRoomDataTop();
        for (; room != NULL; room = room->getNextData()) {
            if (!(i_stageNo == stage_no || stage->isArrival())) continue;
            // fakematch: should be room->getRoomNo()
            if (!dComIfGs_isSaveVisitedRoom(save_table_no, room->m_roomNo)) continue;
            
            f32 offsetX = mRegionOffsetX + stage->getOffsetX();
            f32 offsetZ = mRegionOffsetZ + stage->getOffsetZ();

            dMenu_Fmap_data_c* map_data = room->getFmapData();
            if (map_data == NULL) continue;

            dDrawPath_c::room_class* path = map_data->getMapPath();
            if (path == NULL) continue;

            floor = path->mpFloor;
            points = path->mpFloatData;
            if (floor == NULL) continue;

            found = false;
            floor_num = path->mFloorNum;
            for (floor_no = 0; !found && floor_no < floor_num; floor_no++, floor++) {
                group_num = floor->mGroupNum;
                group = floor->mpGroup;
                if (group == NULL) continue;
                for (group_no = 0; !found && group_no < group_num; group_no++, group++) {
                    line_num = group->mLineNum;
                    if (line_num <= 0) continue;
                    line = group->mpLine;
                    if (line == NULL) continue;
                    for (line_no = 0; !found && line_no < line_num; line_no++, line++) {
                        if (line->field_0x1 != 3) continue;
                        point_num = line->mDataNum;
                        if (!(point_num >= 3)) continue;

                        outside = false;
                        point_index = line->mpData;
                        f32* prev_point = &points[point_index[0] * 2];
                        point = &points[point_index[1] * 2];
                        point_no = 2;
                        point_index += 2;

                        do {
                            vec1.x = prev_point[0] + offsetX - i_pointX;
                            vec1.z = prev_point[1] + offsetZ - i_pointZ;
                            vec2.x = point[0] + offsetX - i_pointX;
                            vec2.z = point[1] + offsetZ - i_pointZ;

                            PSVECCrossProduct(&vec1, &vec2, &cross);
                            if (cross.y >= 0.0f) {
                                dir = 0;
                            } else {
                                dir = -1;
                            }

                            if (point_no > 2 && dir != prev_dir) {
                                outside = true;
                                break;
                            }

                            prev_dir = dir;
                            prev_point = point;
                            point = &points[*point_index * 2];
                            point_no++;
                            point_index++;
                        } while (point_no <= point_num);

                        if (!outside) {
                            found = true;
                            found_stage_no = stage_no;
                            found_room_no = room->getRoomNo();
                        }
                    }
                }
            }
        }
    }

    if (o_stageNo != NULL) {
        *o_stageNo = found_stage_no;
    }
    if (o_roomNo != NULL) {
        *o_roomNo = found_room_no;
    }

    return 1;
}

int dMenu_Fmap_region_data_c::buildFmapRegionData(int param_0) {
    f32 region_min_x = FLT_MAX;
    f32 region_min_z = FLT_MAX;
    f32 region_max_x = -FLT_MAX;
    f32 region_max_z = -FLT_MAX;

    dMenu_Fmap_stage_data_c* stage = mpMenuFmapStageDataTop;
    int stage_no = 0;
    
    for (; stage != NULL; stage_no++, stage = stage->getNextData()) {
        stage->buildFmapStageData(stage_no, mRegionOffsetX, mRegionOffsetZ);
        f32 stage_min_x = stage->getStageMinX() + stage->getOffsetX();
        f32 stage_min_z = stage->getStageMinZ() + stage->getOffsetZ();
        f32 stage_max_x = stage->getStageMaxX() + stage->getOffsetX();
        f32 stage_max_z = stage->getStageMaxZ() + stage->getOffsetZ();

        if (stage_min_x < region_min_x) {
            region_min_x = stage_min_x;
        }
        if (stage_min_z < region_min_z) {
            region_min_z = stage_min_z;
        }
        if (stage_max_x > region_max_x) {
            region_max_x = stage_max_x;
        }
        if (stage_max_z > region_max_z) {
            region_max_z = stage_max_z;
        }
    }

    if (stage_no > 0) {
        mRegionMinX = region_min_x;
        mRegionMinZ = region_min_z;
        mRegionMaxX = region_max_x;
        mRegionMaxZ = region_max_z;
    } else {
        mRegionMinX = 0.0f;
        mRegionMaxX = 0.0f;
        mRegionMinZ = 0.0f;
        mRegionMaxZ = 0.0f;
    }

    return stage_no;
}

void dMenu_Fmap_world_data_c::create(dMenu_Fmap_region_data_c* i_regionData) {
    mp_fmapRegionData = i_regionData;
    buildFmapWorldData();
}

int dMenu_Fmap_world_data_c::buildFmapWorldData() {
    f32 min_x = FLT_MAX;
    f32 min_z = FLT_MAX;
    f32 max_x = -FLT_MAX;
    f32 max_z = -FLT_MAX;

    dMenu_Fmap_region_data_c* region = mp_fmapRegionData;
    int region_no = 0;
    
    for (; region != NULL; region_no++, region = region->getNextData()) {
        f32 region_min_x = region->getRegionMinX() + region->getRegionOffsetX();
        f32 region_min_z = region->getRegionMinZ() + region->getRegionOffsetZ();
        f32 region_max_x = region->getRegionMaxX() + region->getRegionOffsetX();
        f32 region_max_z = region->getRegionMaxZ() + region->getRegionOffsetZ();

        if (region_min_x < min_x) {
            min_x = region_min_x;
        }
        if (region_min_z < min_z) {
            min_z = region_min_z;
        }
        if (region_max_x > max_x) {
            max_x = region_max_x;
        }
        if (region_max_z > max_z) {
            max_z = region_max_z;
        }
    }

    if (region_no > 0) {
        m_worldMinX = min_x;
        m_worldMinZ = min_z;
        m_worldMaxX = max_x;
        m_worldMaxZ = max_z;
    } else {
        m_worldMinX = 0.0f;
        m_worldMaxX = 0.0f;
        m_worldMinZ = 0.0f;
        m_worldMaxZ = 0.0f;
    }

    return region_no;
}

bool dMenuFmapIconPointer_c::init(dMenu_Fmap_region_data_c* i_regionData,
                                 dMenu_Fmap_stage_data_c* i_stageData, u8 i_typeGroupNo,
                                 int i_stayStageNo, int i_stayRoomNo) {
    stage_stag_info_class* stag_info = dComIfGp_getStage()->getStagInfo();
    if (stag_info == NULL) {
        return false;
    }

    mSaveTbl = dStage_stagInfo_GetSaveTbl(stag_info);
    mpRegionData = i_regionData;
    mpStageData = i_stageData;
    mType = dTres_c::getTypeGroupNoToType(i_typeGroupNo);
    mTypeGroupNo = i_typeGroupNo;
    mStayStageNo = i_stayStageNo;
    mStayRoomNo = i_stayRoomNo;
    mStageNo = 0;
    getFirstRoomData();
    return true;
}

bool dMenuFmapIconPointer_c::getFirstData() {
    bool ret = false;
    if (!strcmp(mpStageData->getStageName(), dComIfGp_getStartStageName())) {
        mpTresTypeGroupData = dTres_c::getFirstData(mTypeGroupNo);
        mpTresData = mpTresTypeGroupData->getConstDataPointer();
    } else {
        mpFmpTresTypeGroupDataList = NULL;
        mpFmpTresTypeGroupData = NULL;
        mpTresData = NULL;
        mpFmpTresTypeGroupDataListAll = mpRoomData->getTypeGroupDataListAll();
        if (mpFmpTresTypeGroupDataListAll != NULL) {
            mpFmpTresTypeGroupDataList =
                mpFmpTresTypeGroupDataListAll->getTypeGroupDataList(mTypeGroupNo);
            if (mpFmpTresTypeGroupDataList != NULL) {
                mpFmpTresTypeGroupData = mpFmpTresTypeGroupDataList->getTypeGroupDataHead();
                if (mpFmpTresTypeGroupData != NULL) {
                    do {
                        mpTresData = mpFmpTresTypeGroupData->getTresData();
                        if (mpTresData != NULL) {
                            ret = true;
                            break;
                        }
                        mpFmpTresTypeGroupData = mpFmpTresTypeGroupData->getNextData();
                    } while (mpTresData != NULL);  // !@bug presumably supposed to be mpFmpTresTypeGroupData != NULL
                }
            }
        }
    }
    return ret;
}

bool dMenuFmapIconPointer_c::getData() {
    bool ret = false;
    if (!strcmp(mpStageData->getStageName(), dComIfGp_getStartStageName())) {
        if (mpTresTypeGroupData != NULL
            && mpRoomData->getRoomNo() == mpTresTypeGroupData->getRoomNo())
        {
            mpTresData = mpTresTypeGroupData->getConstDataPointer();
            ret = true;
        }
    } else {
        if (mpTresData != NULL) {
            ret = true;
        }
    }
    return ret;
}

void dMenuFmapIconPointer_c::getFirstRoomData() {
    mpRoomData = mpStageData->getFmapRoomDataTop();
    getFirstData();
}

bool dMenuFmapIconPointer_c::getNextRoomData() {
    bool ret = false;
    mpRoomData = mpRoomData->getNextData();
    if (mpRoomData == NULL) {
        ret = true;
    }
    return ret;
}

bool dMenuFmapIconPointer_c::getNextStageData() {
    bool ret = false;
    mStageNo++;
    mpStageData = mpStageData->getNextData();
    if (mpStageData == NULL) {
        ret = true;
    }
    return ret;
}

bool dMenuFmapIconPointer_c::getNextData() {
    bool ret = true;
    if (!strcmp(mpStageData->getStageName(), dComIfGp_getStartStageName())) {
        if (mpTresTypeGroupData != NULL) {
            mpTresTypeGroupData = dTres_c::getNextData(mpTresTypeGroupData);
            if (mpTresTypeGroupData != NULL) {
                mpTresData = mpTresTypeGroupData->getConstDataPointer();
                ret = false;
            }
        }
    } else {
        if (mpFmpTresTypeGroupData != NULL) {
            mpFmpTresTypeGroupData = mpFmpTresTypeGroupData->getNextData();
            if (mpFmpTresTypeGroupData != NULL) {
                mpTresData = mpFmpTresTypeGroupData->getTresData();
                if (mpTresData != NULL) {
                    ret = false;
                }
            }
        }
    }
    return ret;
}

bool dMenuFmapIconPointer_c::nextData() {
    bool ret = false;
    if (getNextData()) {
        if (getNextRoomData()) {
            if (getNextStageData()) {
                ret = true;
            } else {
                getFirstRoomData();
            }
        } else {
            getFirstData();
        }
    }
    return ret;
}

bool dMenuFmapIconPointer_c::getValidData() {
    bool iVar1, iVar2 = false;
    do {
        iVar1 = getData();
        if (!iVar1) {
            iVar2 = nextData();
        }
    } while (!iVar2 && !iVar1);
    return iVar2;
}

bool dMenuFmapIconDisp_c::getPosition(int* o_stageNo, int* o_roomNo, f32* o_posX, f32* o_posZ,
                                      dTres_c::data_s const** o_tresData) {
    f32 offset_x = mpRegionData->getRegionOffsetX() + mpStageData->getOffsetX();
    f32 offset_z = mpRegionData->getRegionOffsetZ() + mpStageData->getOffsetZ();

    if (o_posX != NULL) {
        *o_posX = 0.0f;
    }
    if (o_posZ != NULL) {
        *o_posZ = 0.0f;
    }
    if (o_stageNo != NULL) {
        *o_stageNo = mStageNo;
    }
    if (o_roomNo != NULL) {
        *o_roomNo = mpRoomData->getRoomNo();
    }

    if (!strcmp(mpStageData->getStageName(), dComIfGp_getStartStageName())) {
        if (mpTresTypeGroupData != NULL) {
            if (o_tresData != NULL) {
                *o_tresData = mpTresTypeGroupData->getConstDataPointer();
            }
            if (o_posX != NULL) {
                *o_posX = offset_x + mpTresTypeGroupData->getPos()->x;
            }
            if (o_posZ != NULL) {
                *o_posZ = offset_z + mpTresTypeGroupData->getPos()->z;
            }
        }
    } else {
        if (mpTresData != NULL) {
            if (o_tresData != NULL) {
                *o_tresData = mpTresData;
            }
            if (o_posX != NULL) {
                *o_posX = offset_x + mpTresData->mPos.x;
            }
            if (o_posZ != NULL) {
                *o_posZ = offset_z + mpTresData->mPos.z;
            }
        }
    }

    return false;
}

bool dMenuFmapIconDisp_c::isDrawDisp() {
    dMenu_Fmap_stage_arc_data_c* stage_arc = mpStageData->getStageArc();
    if (stage_arc == NULL) {
        return false;
    }

    u8 save_table_no = stage_arc->getSaveTableNo();
    s32 room_no = mpRoomData->getRoomNo();
    bool bVar2 = (mStayStageNo == mStageNo && mStayRoomNo == room_no) || mpRoomData->isArrival();
    bool ret = false;
    bool bVar1;

    switch (mTypeGroupNo) {
    case 1:
    case 8:
        if (save_table_no == mSaveTbl) {
            ret = bVar2 && (mpTresData->mSwBit == 0xff ||
                (mpTresData->mSwBit != 0xff && dComIfGs_isSwitch(mpTresData->mSwBit, room_no)));
        } else {
            ret = bVar2 && (mpTresData->mSwBit == 0xff ||
                    (mpTresData->mSwBit != 0xff && mpTresData->mSwBit < 0x80
                        && dComIfGs_isStageSwitch(save_table_no, mpTresData->mSwBit)));
        }
        break;

    case 4:
        bVar1 = false;
        if (dComIfGp_isLightDropMapVisible()) {
            if (dComIfGp_getStartStageDarkArea() == 2) {
                int tres_no = mpTresData->mNo;
                if (tres_no == 0x33 || tres_no == 0x34 || tres_no == 0x35) {
                        /* dSv_event_flag_c::M_086 - Twilight Hyrule Field - Show Boss Bug's Tear of Light on the map */
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[119])) {
                        bVar1 = true;
                    }
                } else {
                    bVar1 = true;
                }
            } else {
                bVar1 = true;
            }
        }
        if (bVar1 && mpTresData->mNo != 0xff && !dComIfGs_isStageTbox(save_table_no, mpTresData->mNo)) {
            ret = true;
        }
        break;

    case 5:
        if (save_table_no == mSaveTbl) {
            ret = (mpTresData->mNo == 0xff ||
                    (mpTresData->mNo != 0xff && !dComIfGs_isTbox(mpTresData->mNo)))
                && (mpTresData->mSwBit == 0xff || (mpTresData->mSwBit != 0xff
                    && dComIfGs_isSwitch(mpTresData->mSwBit, room_no)));
        } else {
            ret = (mpTresData->mNo == 0xff || (mpTresData->mNo != 0xff
                    && !dComIfGs_isStageTbox(save_table_no, mpTresData->mNo)))
                && (mpTresData->mSwBit == 0xff ||
                    (mpTresData->mSwBit != 0xff && mpTresData->mSwBit < 0x80 &&
                    dComIfGs_isStageSwitch(save_table_no, mpTresData->mSwBit)));
        }
        break;

    case 6:
        if (save_table_no == mSaveTbl) {
            ret = dComIfGs_isSwitch(mpTresData->mSwBit, room_no) != FALSE;
        } else if (mpTresData->mSwBit < 0x80) {
            ret = dComIfGs_isStageSwitch(save_table_no, mpTresData->mSwBit) != FALSE;
        }
        break;

    case 10:
        ret = mpTresData->mNo != 0xff && dComIfGs_isStageTbox(save_table_no, mpTresData->mNo);
        break;

    case 13:
    case 14:
        if (save_table_no == mSaveTbl) {
            ret = mpTresData->mSwBit == 0xff ||
                (mpTresData->mSwBit != 0xff && dComIfGs_isSwitch(mpTresData->mSwBit, room_no));
        } else {
            ret = mpTresData->mSwBit == 0xff || (mpTresData->mSwBit < 0x80
                && dComIfGs_isStageSwitch(save_table_no, mpTresData->mSwBit));
        }
        break;
    }

    return ret;
}
