#include "d/d_com_static.h"
#include "d/d_demo.h"
#include "d/actor/d_a_arrow.h"
#include "d/actor/d_a_dshutter.h"
#include "d/actor/d_a_grass.h"
#include "d/actor/d_a_mirror.h"
#include "d/actor/d_a_movie_player.h"
#include "d/actor/d_a_set_bgobj.h"
#include "d/actor/d_a_ykgr.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/actor/d_a_obj_movebox.h"
#include "d/actor/d_a_tag_magne.h"
#include "d/actor/d_a_tag_mist.h"
#include "d/actor/d_a_tag_stream.h"
#include "stdio.h"

void daSus_c::data_c::set(s8 i_roomNo, cXyz const& param_1, cXyz const& param_2, u8 param_3,
                          u8 param_4, u8 i_type) {
    mRoomNo = i_roomNo;
    field_0x8.set(param_1.x - param_2.x, param_1.y, param_1.z - param_2.z);
    field_0x14.set(param_1.x + param_2.x, param_1.y + param_2.y, param_1.z + param_2.z);
    field_0x2 = param_3;
    field_0x1 = param_4 != 0;
    field_0x3 = isSwitch() == false;
    mType = i_type;
}

daSus_c::data_c* daSus_c::data_c::reset() {
    mRoomNo = -1;
    return mpNext;
}

u8 daSus_c::data_c::isSwitch() {
    if (field_0x1 & 1) {
        return dComIfGs_isSwitch(field_0x2, mRoomNo);
    } else {
        return dComIfGs_isSwitch(field_0x2, mRoomNo) == false;
    }
}

bool daSus_c::data_c::check(cXyz const& i_pos) {
    if (field_0x3 != 0) {
        return false;
    }

    if ((field_0x8.x <= i_pos.x && i_pos.x <= field_0x14.x) &&
        (field_0x8.y <= i_pos.y && i_pos.y <= field_0x14.y) &&
        (field_0x8.z <= i_pos.z && i_pos.z <= field_0x14.z))
    {
        return true;
    }

    return false;
}

u8 daSus_c::data_c::check(fopAc_ac_c* i_actor) {
    if (field_0x3) {
        return 0x80;
    }

    u8 tmp = 0x80;

    if (check(i_actor->home.pos)) {
        tmp |= 1;
    }

    if (check(i_actor->current.pos)) {
        tmp |= 2;
        return tmp;
    }

    return tmp;
}

void daSus_c::data_c::execute() {
    u8 sw = isSwitch();
    if (sw != field_0x3) {
        field_0x3 = sw;
    }
}

void daSus_c::room_c::add(daSus_c::data_c* i_data) {
    if (mpData == NULL) {
        mpData = i_data;
        i_data->setNext(NULL);
        return;
    }

    if (i_data->getType() == 0) {
        i_data->setNext(mpData);
        mpData = i_data;
        return;
    }

    daSus_c::data_c* data1 = mpData;
    daSus_c::data_c* data2 = data1->getNext();
    while (data2 != NULL) {
        if (data1->getType() != 0)
            break;
        data1 = data2;
        data2 = data2->getNext();
    }

    i_data->setNext(data1->getNext());
    data1->setNext(i_data);
}

void daSus_c::room_c::reset() {
    while (mpData != NULL && mpData->getType() == 0) {
        mpData = mpData->reset();
    }
}

daSus_c::data_c daSus_c::mData[0x20];

daSus_c::room_c daSus_c::mRoom[0x40];

/* 80450D88 0002+00 data_80450D88 m_count__9daArrow_c */
s16 daArrow_c::m_count;

/* 80450D8A 0002+00 data_80450D8A mSetTop__7daSus_c */
s16 daSus_c::mSetTop;

int daSus_c::newData(s8 i_roomNo, cXyz const& param_1, cXyz const& param_2, u8 param_3, u8 param_4,
                     u8 i_type) {
    s16 setTop = mSetTop;
    daSus_c::data_c* pData = ((daSus_c::data_c*)mData) + setTop;
    for (s16 i = setTop; i < 32; pData++, i++) {
        if (pData->isUsed()) {
            pData->set(i_roomNo, param_1, param_2, param_3, param_4, i_type);
            daSus_c::room_c* pRoom = ((daSus_c::room_c*)mRoom) + i_roomNo;
            pRoom->add(pData);
            mSetTop = (i + 1) % 32;
            return 1;
        }
    }

    pData = ((daSus_c::data_c*)mData);
    for (s16 i = 0; i < setTop; pData++, i++) {
        if (pData->isUsed()) {
            pData->set(i_roomNo, param_1, param_2, param_3, param_4, i_type);
            ((daSus_c::room_c*)mRoom)[i_roomNo].add(pData);
            mSetTop = (i + 1) % 32;
            return 1;
        }
    }

    return 0;
}

void daSus_c::reset() {
    daSus_c::data_c* pData = (daSus_c::data_c*)mData;
    for (int i = 0; i < 0x20; i++, pData++) {
        pData->reset();
    }

    daSus_c::room_c* pRoom = (daSus_c::room_c*)mRoom;
    for (int i = 0; i < 0x40; i++, pRoom++) {
        pRoom->init();
    }
}

bool daSus_c::check(s8 i_roomNo, cXyz const& i_pos) {
    daSus_c::data_c* pData = (daSus_c::data_c*)mData;
    for (s16 i = 0; i < 0x20; pData++, i++) {
        if (!pData->isUsed() && i_roomNo == pData->getRoomNo() && pData->getType()) {
            if (pData->check(i_pos)) {
                return true;
            }
        }
    }
    return false;
}

void daSus_c::check(fopAc_ac_c* i_actor) {
    if (fopAcM_GetGroup(i_actor) != 1 && fopAcM_GetGroup(i_actor) != 5) {
        daSus_c::data_c* pData = (daSus_c::data_c*)mData;
        u8 res = 0;
        for (s16 i = 0; i < 0x20; pData++, i++) {
            if (!pData->isUsed() && pData->getType() == 0) {
                res |= pData->check(i_actor);
            }
        }

        if ((res & 0x80) != 0) {
            if ((res & 3) == 3) {
                fopAcM_OnStatus(i_actor, 0x20000000);
            } else {
                fopAcM_OffStatus(i_actor, 0x20000000);
            }
        }
    }
}

void daSus_c::execute() {
    daSus_c::data_c* pData = (daSus_c::data_c*)mData;
    for (s16 i = 0; i < 0x20; pData++, i++) {
        if (!pData->isUsed()) {
            pData->execute();
        }
    }
}

int daNpcMsg_setEvtNum(u8 iEvtNum) {
    dComIfGs_setTmpReg(0xFDFF, iEvtNum);
    return 1;
}

u8 daNpcKakashi_chkSwdTutorialStage() {
    return strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0 && dComIfG_play_c::getLayerNo(0) == 8;
}

void daNpcKakashi_setSwdTutorialStep(u8 iEvtNum) {
    dComIfGs_setTmpReg(0xFF0F, iEvtNum);
}

u8 daNpcKakashi_getSwdTutorialStep() {
    return dComIfGs_getTmpReg(0xFF0F) & 0xff;
}

bool daNpcKakashi_getSwdTutorialResult() {
           /* dSv_event_tmp_flag_c::T_0004 - Ordon Village - sword tutorial - outcome */
    return dComIfGs_isTmpBit(0x40);
}

void daNpcKakashi_setSwdTutorialResult(bool param_0) {
    if (param_0) {
        /* dSv_event_tmp_flag_c::T_0004 - Ordon Village - sword tutorial - outcome */
        dComIfGs_onTmpBit(0x40);
    } else {
        /* dSv_event_tmp_flag_c::T_0004 - Ordon Village - sword tutorial - outcome */
        dComIfGs_offTmpBit(0x40);
    }
}

u8 daNpcKakashi_getSuccessCount() {
    return dComIfGs_getTmpReg(0xFEFF) & 0xFF;
}

void daNpcKakashi_incSuccessCount() {
    int tmpReg = dComIfGs_getTmpReg(0xFEFF);
    tmpReg++;
    dComIfGs_setTmpReg(0xFEFF, tmpReg);
}

void daNpcKakashi_clrSuccessCount() {
    dComIfGs_setTmpReg(0xFEFF, 0);
}

/* 80450D8C 0001+00 data_80450D8C None */
bool daYkgr_c::m_flag;

/* 80450D8D 0001+00 data_80450D8D None */
bool daYkgr_c::m_alpha_flag;

/* 80450D8E 0002+00 data_80450D8E None */
u8 daYkgr_c::m_alpha;

f32 daYkgr_c::m_aim_rate;

dPath* daYkgr_c::m_path;

JPABaseEmitter* daYkgr_c::m_emitter;

const char* daSetBgObj_c::getArcName(fopAc_ac_c* i_this) {
    static char arcName[8];

    sprintf(arcName, "@bg%04x", (u16)fopAcM_GetParam(i_this));
    return arcName;
}

s16 const daObjMovebox::Act_c::M_dir_base[4] = {
    0x0000,
    0x4000,
    -0x8000,
    -0x4000,
};

f32 const daDsh_c::OPEN_SIZE = 270.1f;

f32 const daDsh_c::OPEN_ACCEL = 10.0f;

f32 const daDsh_c::OPEN_SPEED = -40.0f;

f32 const daDsh_c::OPEN_BOUND_SPEED = -30.0f;

f32 const daDsh_c::OPEN_BOUND_RATIO = -0.4f;

f32 const daDsh_c::CLOSE_ACCEL = 10.0f;

f32 const daDsh_c::CLOSE_SPEED = 40.0f;

f32 const daDsh_c::CLOSE_BOUND_SPEED = 30.0f;

f32 const daDsh_c::CLOSE_BOUND_RATIO = -0.4f;

int daTagStream_c::checkArea(cXyz const* param_0) {
    cXyz relativePos;
    fpoAcM_relativePos(this, param_0, &relativePos);
    if (relativePos.y >= 0.0f && relativePos.y <= scale.y && fabsf(relativePos.x) <= scale.x &&
        fabsf(relativePos.z) <= scale.z)
    {
        return 1;
    }
    return 0;
}

daMirror_c::entryModelFunc daMirror_c::m_entryModel = NULL;

daTagStream_c* daTagStream_c::m_top;

daMirror_c* daMirror_c::m_myObj;

int daMirror_c::entry(J3DModel* param_0) {
    if (m_myObj == NULL) {
        return 0;
    }
    return (m_myObj->*(daMirror_c::m_entryModel))(param_0);
}

bool daMirror_c::remove() {
    if (m_myObj == NULL) {
        return false;
    }

    fopAcM_delete(m_myObj);
    return true;
}

dGrass_packet_c::deleteFunc dGrass_packet_c::m_deleteRoom = NULL;

daGrass_c* daGrass_c::m_myObj;

dGrass_packet_c* daGrass_c::m_grass;

void daGrass_c::deleteRoomGrass(int param_0) {
    if (m_grass != NULL) {
        dGrass_packet_c::deleteFunc delete_func = dGrass_packet_c::m_deleteRoom;
        (m_grass->*(delete_func))(param_0);
    }
}

dFlower_packet_c::deleteFunc dFlower_packet_c::m_deleteRoom = NULL;

dFlower_packet_c* daGrass_c::m_flower;

void daGrass_c::deleteRoomFlower(int param_0) {
    if (m_flower != NULL) {
        dFlower_packet_c::deleteFunc delete_func = dFlower_packet_c::m_deleteRoom;
        (m_flower->*(delete_func))(param_0);
    }
}

daMP_c* daMP_c::m_myObj;

u32 daMP_c::daMP_c_Get_MovieRestFrame() {
    return m_myObj->mpGetMovieRestFrame();
}

void daMP_c::daMP_c_Set_PercentMovieVolume(f32 i_volume) {
    m_myObj->mpSetPercentMovieVol(i_volume);
}

int daMP_c::daMP_c_THPPlayerPlay() {
    if (m_myObj == NULL) {
        dDemo_c::onStatus(1);
        return 0;
    } else {
        dDemo_c::offStatus(1);
        return m_myObj->mpTHPPlay();
    }
}

void daMP_c::daMP_c_THPPlayerPause() {
    m_myObj->mpTHPPause();
}

daTagMagne_c* daTagMagne_c::mTagMagne;

int daTagMagne_c::checkMagnetCode(cBgS_PolyInfo& poly) {
    if (mTagMagne == NULL) {
        return 0;
    }

    int magCode = dComIfG_Bgsp().GetMagnetCode(poly);
    if ((magCode == 1 && mTagMagne->checkMagneA()) || (magCode == 2 && mTagMagne->checkMagneB()) ||
        (magCode == 3 && mTagMagne->checkMagneC()))
    {
        return 1;
    }

    return 0;
}

int daTagMagne_c::checkMagneA() {
    if (mSwNo1 == 0xFF || fopAcM_isSwitch(this, mSwNo1)) {
        return 1;
    }

    return 0;
}

int daTagMagne_c::checkMagneB() {
    if (mSwNo2 == 0xFF || fopAcM_isSwitch(this, mSwNo2)) {
        return 1;
    }

    return 0;
}

int daTagMagne_c::checkMagneC() {
    if (mSwNo3 == 0xFF || fopAcM_isSwitch(this, mSwNo3)) {
        return 1;
    }

    return 0;
}

/* 80450DC0 0001+00 data_80450DC0 None */
u8 daTagMist_c::mPlayerNo;

/* 80450DC1 0003+00 data_80450DC1 None */
bool daObjCarry_c::mSaveFlag;

u8 daTagMist_c::getPlayerNo() {
    return mPlayerNo;
}

void daObjCarry_c::clrSaveFlag() {
    mSaveFlag = false;
}

void daObjCarry_c::setSaveFlag() {
    mSaveFlag = true;
}

bool daObjCarry_c::chkSaveFlag() {
    return mSaveFlag == true;
}

cXyz daObjCarry_c::mPos[5];

const cXyz& daObjCarry_c::getPos(int param_0) {
    return mPos[param_0];
}

void daObjCarry_c::savePos(int param_0, cXyz param_1) {
    mPos[param_0].x = param_1.x;
    mPos[param_0].y = param_1.y;
    mPos[param_0].z = param_1.z;
}

u8 daObjCarry_c::mSttsFlag[5];

void daObjCarry_c::onSttsFlag(int idx, u8 flag) {
    mSttsFlag[idx] |= flag;
}

void daObjCarry_c::offSttsFlag(int idx, u8 flag) {
    mSttsFlag[idx] &= ~flag;
}

u8 daObjCarry_c::chkSttsFlag(int idx, u8 flag) {
    return (u8)(mSttsFlag[idx] & flag) ? 1 : 0;
}

s8 daObjCarry_c::mRoomNo[5];

void daObjCarry_c::setRoomNo(int idx, s8 roomNo) {
    mRoomNo[idx] = roomNo;
}

s8 daObjCarry_c::getRoomNo(int idx) {
    return mRoomNo[idx];
}
