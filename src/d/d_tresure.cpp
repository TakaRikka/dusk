#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_tresure.h"
#include "d/d_com_inf_game.h"
#include "d/d_map_path_dmap.h"
#include "d/d_path.h"
#include "SSystem/SComponent/c_math.h"

static bool data_80450680 = true;

dTres_c::typeGroupData_c* dTres_c::mTypeGroupData;

int dTres_c::createWork() {
    mTypeGroupData = new dTres_c::typeGroupData_c[0x40];
    return 1;
}

void dTres_c::create() {
    if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) != ST_BOSS_ROOM) {
        reset();
    }
}

void dTres_c::remove() {
    data_80450680 = true;
}

dTres_c::type_group_list dTres_c::mTypeGroupListAll[17];

u16 dTres_c::mNum;

void dTres_c::reset() {
    if (data_80450680) {
        mNum = 0;
        data_80450680 = false;

        for (int i = 0; i < 17; i++) {
            mTypeGroupListAll[i].field_0x0 = NULL;
            mTypeGroupListAll[i].field_0x4 = NULL;
            mTypeGroupListAll[i].mNumber = 0;
        }
    }
}

void dTres_c::addData(dTres_c::list_class* p_list, s8 roomNo) {
    if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) != ST_BOSS_ROOM) {
        reset();

        data_s* listData = p_list->field_0x4->getDataPointer();
        typeGroupData_c* groupData = &mTypeGroupData[mNum];
        for (int i = 0; i < p_list->field_0x0; i++, listData++, groupData++) {
            *(data_s*)groupData = *listData;
            groupData->mRoomNo = roomNo;
            groupData->mStatus = 0;

            u32 typeGroupNo = getTypeToTypeGroupNo(groupData->mType);
            groupData->setNextDataPointer(NULL);
            groupData->setTypeGroupNo(typeGroupNo);

            type_group_list* typeGroupList = mTypeGroupListAll + typeGroupNo;
            if (typeGroupList->field_0x0 == NULL) {
                typeGroupList->field_0x0 = groupData;
            }

            if (typeGroupList->field_0x4 != NULL) {
                typeGroupList->field_0x4->setNextDataPointer(groupData);
            }
            typeGroupList->field_0x4 = groupData;
            typeGroupList->mNumber++;

            switch (groupData->mType) {
                case 0xFF:
                    checkTreasureBox(groupData);
                    break;
            }

            mNum++;
        }
    }
}

void dTres_c::checkTreasureBox(dTres_c::data_s* param_0) {
    if (param_0->mSwBit != 0xff && param_0->mArg1 != 0xff){
        dPath* path = dPath_GetRoomPath(param_0->mArg1, -1);

        if (path == NULL) return;

        if (param_0->mArg2 == 0) {
            s16 sVar5 = 0;
            for (int i = 0; i < 4; i++) {
                int sStack_62;
                if (dComIfGs_isSwitch(param_0->mSwBit + i, param_0->mRoomNo) != 0) {
                    sStack_62 = (1 << i);
                } else {
                    sStack_62 = 0;
                }
                sVar5 += sStack_62;
            }
            if (sVar5 != 0) {
                s16 uStack_34 = cM_deg2s((sVar5 - 1) * 0x18 + 12);
                cXyz cStack_48(path->m_points[0].m_position);
                cXyz cStack_54(path->m_points[path->m_num - 1].m_position);
                f32 dVar7 = cStack_48.absXZ(cStack_54);
                cXyz local_60(0.0f, 0.0f, dVar7);
                mDoMtx_stack_c::YrotS(uStack_34);
                mDoMtx_stack_c::multVec(&local_60, &local_60);
                local_60 += cStack_48;
                local_60.y = cStack_54.y;
                param_0->mPos = local_60;
            } else {
                param_0->mPos = path->m_points[0].m_position;
            }
            dMapInfo_n::correctionOriginPos(param_0->mRoomNo, &param_0->mPos);
        } else {
            dPnt* point;
            if (dComIfGs_isSwitch(param_0->mSwBit, param_0->mRoomNo)) {
                point = &path->m_points[path->m_num - 1];
            } else {
                point = &path->m_points[0];
            }

            param_0->mPos = point->m_position;
            dMapInfo_n::correctionOriginPos(param_0->mRoomNo, &param_0->mPos);
        }
    }
}

void dTres_c::onStatus(u8 listIdx, int param_1, int flag) {
    typeGroupData_c* groupData = getFirstData(listIdx);
    int typeGroupNo = getTypeGroupNumber(listIdx);

    for (int i = 0; i < typeGroupNo; i++) {
        if (param_1 == groupData->getNo()) {
            u8 status = groupData->getStatus();

            cLib_onBit<u8>(status, flag);
            groupData->setStatus(status);
        }
        groupData = getNextData(groupData);
    }
}

void dTres_c::offStatus(u8 listIdx, int param_1, int flag) {
    typeGroupData_c* groupData = getFirstData(listIdx);
    s32 typeGroupNo = getTypeGroupNumber(listIdx);

    for (int i = 0; i < typeGroupNo; i++) {
        if (param_1 == groupData->getNo()) {
            u8 status = groupData->getStatus();

            cLib_offBit<u8>(status, flag);
            groupData->setStatus(status);
        }
        groupData = getNextData(groupData);
    }
}

bool dTres_c::getBossIconFloorNo(int* o_floorNo) {
    bool ret;

    if (o_floorNo == NULL) {
        return 0;
    }

    ret = 0;
    *o_floorNo = 0;
    typeGroupData_c* groupData = getFirstData(3);

    if (groupData != NULL && dComIfGs_isDungeonItemCompass()) {
        if ((groupData->getSwBit() == 0xFF ||
             (groupData->getSwBit() != 0xFF &&
              dComIfGs_isSwitch(groupData->getSwBit(), groupData->getRoomNo()))) &&
            !dComIfGs_isStageBossEnemy()) {
            *o_floorNo =
                dMapInfo_c::calcFloorNo(groupData->getPos()->y, true, groupData->getRoomNo());
            ret = 1;
        }
    }

    return ret;
}

dTres_c::typeGroupData_c* dTres_c::getFirstData(u8 listIdx) {
    if (mTypeGroupListAll + listIdx == NULL) {
        return NULL;
    }

    if ((s32)mTypeGroupListAll[listIdx].mNumber == 0) {
        return NULL;
    }

    return mTypeGroupListAll[listIdx].field_0x0;
}

dTres_c::typeGroupData_c* dTres_c::getNextData(dTres_c::typeGroupData_c* p_data) {
    if (p_data == NULL) {
        return NULL;
    }

    return p_data->getNextDataPointer();
}

dTres_c::typeGroupData_c* dTres_c::getNextData(dTres_c::typeGroupData_c const* p_data) {
    if (p_data == NULL) {
        return NULL;
    }

    return p_data->getNextDataPointer();
}

void dTres_c::setPosition(int dataNo, u8 listIdx, Vec const* i_pos, int i_roomNo) {
    typeGroupData_c* groupData = getFirstData(listIdx);

    for (int i = getTypeGroupNumber(listIdx); i > 0; i--) {
        if (dataNo == groupData->getNo()) {
            Vec pos;
            pos.x = i_pos->x;
            pos.y = i_pos->y;
            pos.z = i_pos->z;

            if (i_roomNo >= 0) {
                groupData->setRoomNo(i_roomNo);
            }
            dMapInfo_n::correctionOriginPos(groupData->getRoomNo(), &pos);
            groupData->setPos(pos);
            return;
        }
        groupData = groupData->getNextDataPointer();
    }
}

u8 const dTres_c::typeToTypeGroup[17][2] = {
    {0xFF, 0x00}, {0x00, 0x01}, {0x01, 0x02}, {0x02, 0x03}, {0x03, 0x04}, {0x04, 0x05},
    {0x05, 0x06}, {0x06, 0x07}, {0x07, 0x08}, {0x80, 0x09}, {0x81, 0x0A}, {0x82, 0x0B},
    {0x83, 0x0C}, {0x84, 0x0D}, {0x85, 0x0E}, {0x87, 0x0F}, {0x88, 0x10},
};

int dTres_c::getTypeGroupNoToType(u8 i_typeGroupNo) {
    return typeToTypeGroup[i_typeGroupNo][0];
}

u8 dTres_c::getTypeToTypeGroupNo(u8 i_type) {
    int groupNo = 17;
    for (int i = 0; i < 17; i++) {
        if (i_type == typeToTypeGroup[i][0]) {
            groupNo = typeToTypeGroup[i][1];
            break;
        }
    }

    return groupNo;
}
