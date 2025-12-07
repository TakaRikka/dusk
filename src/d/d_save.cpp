/**
 * d_save.cpp
 * Save File Manager
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_save.h"
#include "d/d_save_init.h"
#include "f_op/f_op_scene_mng.h"
#include "stdio.h"

#if VERSION == VERSION_SHIELD
#include "lingcod/lingcod.h"
#endif

static u8 dSv_item_rename(u8 i_itemNo) {
    switch (i_itemNo) {
    case fpcNm_ITEM_OIL_BOTTLE_2:
        return fpcNm_ITEM_OIL_BOTTLE;
    case fpcNm_ITEM_RED_BOTTLE_2:
        return fpcNm_ITEM_RED_BOTTLE;
    case fpcNm_ITEM_OIL2:
        return fpcNm_ITEM_OIL;
    default:
        return i_itemNo;
    }
}

void dSv_player_status_a_c::init() {
    mMaxLife = 15;
    mLife = 12;
    mRupee = 0;
    mMaxOil = 0;
    mOil = 0;
    unk10 = 0;

    for (int i = 0; i < MAX_SELECT_ITEM; i++) {
        mSelectItem[i] = 0xFF;
        mMixItem[i] = 0xFF;
        dComIfGp_setSelectItem(i);
    }

    for (int i = 0; i < MAX_EQUIPMENT; i++) {
        mSelectEquip[i] = 0;
    }

    mSelectEquip[COLLECT_CLOTHING] = fpcNm_ITEM_WEAR_CASUAL;
    mSelectEquip[COLLECT_SWORD] = fpcNm_ITEM_NONE;
    mSelectEquip[COLLECT_SHIELD] = fpcNm_ITEM_NONE;
    mSelectEquip[COLLECT_SMELL] = fpcNm_ITEM_NONE;
    mSelectEquip[B_BUTTON_ITEM] = fpcNm_ITEM_NONE;
    mWalletSize = WALLET;
    mMaxMagic = 0;
    mMagic = 0;
    mMagicFlag = 0;
    unk29 = 0;
    mTransformStatus = TF_STATUS_HUMAN;

    for (int i = 0; i < 3; i++) {
        unk31[i] = 0;
    }
}

void dSv_player_status_a_c::setSelectItemIndex(int i_no, u8 i_slotNo) {
    JUT_ASSERT(107, (i_no >= 0) && (i_no < ITEM_XY_MAX_DUMMY/2));

    if (i_no < MAX_SELECT_ITEM) {
        mSelectItem[i_no] = i_slotNo;
    }
}

u8 dSv_player_status_a_c::getSelectItemIndex(int i_no) const {
    JUT_ASSERT(124, (i_no >= 0) && (i_no < ITEM_XY_MAX_DUMMY/2));

    if (i_no < MAX_SELECT_ITEM) {
        return mSelectItem[i_no];
    }
    return 0;
}

void dSv_player_status_a_c::setMixItemIndex(int i_no, u8 i_slotNo) {
    JUT_ASSERT(143, (i_no >= 0) && (i_no < ITEM_XY_MAX_DUMMY/2));

    if (i_no < MAX_SELECT_ITEM) {
        mMixItem[i_no] = i_slotNo;
    }
}

u8 dSv_player_status_a_c::getMixItemIndex(int i_no) const {
    JUT_ASSERT(160, (i_no >= 0) && (i_no < ITEM_XY_MAX_DUMMY/2));

    if (i_no < MAX_SELECT_ITEM) {
        return mMixItem[i_no];
    }
    return 0;
}

u16 dSv_player_status_a_c::getRupeeMax() const {
    if (mWalletSize < 3) {  // if you make this a default, it wont match. Compiler, pls.
        switch (mWalletSize) {
        case WALLET:
            return 300;
        case BIG_WALLET:
            return 600;
        case GIANT_WALLET:
            return 1000;
        }
    }

    return 0;
}

void dSv_player_status_a_c::onMagicFlag(u8 i_magic) {
    JUT_ASSERT(203, 0 <= i_magic && i_magic < 8);
    if (i_magic == 0) {
        /* Main Event - Can use magic */
        dComIfGs_onEventBit(dSv_event_flag_c::M_097);
    }

    mMagicFlag |= (u8)(1 << i_magic);
}

void dSv_player_status_a_c::offMagicFlag(u8 i_magic) {
    JUT_ASSERT(222, 0 <= i_magic && i_magic < 8);
    if (i_magic == 0) {
        /* Main Event - Can use magic */
        dComIfGs_offEventBit(dSv_event_flag_c::M_097);
    }

    mMagicFlag &= (u8)~(u8)(1 << i_magic);
}

BOOL dSv_player_status_a_c::isMagicFlag(u8 i_magic) const {
    JUT_ASSERT(241, 0 <= i_magic && i_magic < 8);
    if (i_magic == 0) {
        /* Main Event - Can use magic */
        return dComIfGs_isEventBit(dSv_event_flag_c::M_097);
    }

    return (mMagicFlag & (u8)(1 << i_magic)) ? TRUE : FALSE;
}

void dSv_player_status_b_c::init() {
    mDateIpl = 0;
    mTransformLevelFlag = 0;
    mDarkClearLevelFlag = 0;
    unk10 = 0;
    mTime = 255.0f;
    mDate = 0;

    for (int i = 0; i < 3; i++) {
        unk18[i] = 0;
    }
}

void dSv_player_status_b_c::onDarkClearLV(int i_no) {
    JUT_ASSERT(281, (i_no >= 0) && (i_no < 8));
    mDarkClearLevelFlag |= (u8)(1 << i_no);
}

void dSv_player_status_b_c::offDarkClearLV(int i_no) {
    JUT_ASSERT(296, (i_no >= 0) && (i_no < 8));
    mDarkClearLevelFlag &= (u8)~(u8)(1 << i_no);
}

BOOL dSv_player_status_b_c::isDarkClearLV(int i_no) const {
    JUT_ASSERT(311, (i_no >= 0) && (i_no < 8));
    return mDarkClearLevelFlag & (u8)(1 << i_no) ? TRUE : FALSE;
}

void dSv_player_status_b_c::onTransformLV(int i_no) {
    JUT_ASSERT(326, (i_no >= 0) && (i_no < 8));
    mTransformLevelFlag |= (u8)(1 << i_no);
}

void dSv_player_status_b_c::offTransformLV(int i_no) {
    JUT_ASSERT(341, (i_no >= 0) && (i_no < 8));
    mTransformLevelFlag &= (u8)~(u8)(1 << i_no);
}

BOOL dSv_player_status_b_c::isTransformLV(int i_no) const {
    JUT_ASSERT(356, (i_no >= 0) && (i_no < 8));
    return mTransformLevelFlag & (u8)(1 << i_no) ? TRUE : FALSE;
}

void dSv_horse_place_c::init() {
    strcpy(mName, "");
    mPos.set(0.0f, 0.0f, 0.0f);
    mAngleY = 0;
    mSpawnId = 0;
    mRoomNo = 0;
}

void dSv_horse_place_c::set(const char* i_name, const cXyz& i_pos, s16 i_angle, s8 i_roomNo) {
    JUT_ASSERT(391, strlen(i_name) <= 7);
    strcpy(mName, i_name);
    mPos = i_pos;
    mAngleY = i_angle;
    mRoomNo = i_roomNo;
}

void dSv_player_return_place_c::init() {
    strcpy(mName, "F_SP108");
    mRoomNo = 1;
    mPlayerStatus = 0;
    unk10 = 21;
    unk11 = 0;
}

void dSv_player_return_place_c::set(const char* i_name, s8 i_roomNo, u8 i_status) {
    JUT_ASSERT(426, strlen(i_name) <= 7);
    strcpy(mName, i_name);
    mRoomNo = i_roomNo;
    mPlayerStatus = i_status;
}

void dSv_player_field_last_stay_info_c::init() {
    strcpy(mName, "");
    mPos.set(0.0f, 0.0f, 0.0f);
    mAngleY = 0;
    mLastSpawnId = 0;
    mRegionNo = 1;
    mFieldDataExistFlag = false;
    mRegion = 0;

    for (int i = 0; i < 2; i++) {
        unk26[i] = 0;
    }
}

void dSv_player_field_last_stay_info_c::set(const char* i_name, const cXyz& i_pos, s16 i_angle,
                                            s8 i_spawn, u8 i_regionNo) {
    JUT_ASSERT(468, strlen(i_name) <= 7);
    strcpy(mName, i_name);
    mPos.set(i_pos);
    mAngleY = i_angle;
    mLastSpawnId = i_spawn;
    mRegionNo = i_regionNo;
}

void dSv_player_field_last_stay_info_c::onRegionBit(int i_region) {
    if (i_region >= 0 && i_region < 8) {
        mRegion |= (u8)(1 << i_region);
    } else {
        JUT_WARN(490, "region error!! %d", i_region);
    }
}

BOOL dSv_player_field_last_stay_info_c::isRegionBit(int i_region) const {
    if (i_region >= 0 && i_region < 8) {
        return (mRegion & (u8)(1 << i_region)) ? TRUE : FALSE;
    } else {
        JUT_WARN(526, "region error!! %d", i_region);
        return FALSE;
    }
}

void dSv_player_last_mark_info_c::init() {
    strcpy(mName, "");
    mPos.set(0.0f, 0.0f, 0.0f);
    mAngleY = 0;
    mRoomNo = 0;
    mSpawnId = 0;
    resetWarpAcceptStage();

    for (int i = 0; i < 3; i++) {
        unk25[i] = 0;
    }
}

void dSv_player_last_mark_info_c::setWarpItemData(const char* i_name, const cXyz& i_pos,
                                                  s16 i_angle, s8 i_roomNo, u8 unk_4, u8 unk_5) {
    strcpy(mName, i_name);
    mPos.set(i_pos);
    mAngleY = i_angle;
    mRoomNo = i_roomNo;

    (void)unk_4;
    (void)unk_5;
}

void dSv_player_item_c::init() {
    for (int i = 0; i < MAX_ITEM_SLOTS; i++) {
        mItems[i] = fpcNm_ITEM_NONE;
        mItemSlots[i] = fpcNm_ITEM_NONE;
    }
}

void dSv_player_item_c::setItem(int i_slotNo, u8 i_itemNo) {
    if (i_slotNo < MAX_ITEM_SLOTS) {
        mItems[i_slotNo] = i_itemNo;
        setLineUpItem();
    }

    for (int i = DEFAULT_SELECT_ITEM_INDEX; i < MAX_SELECT_ITEM - 1; i++) {
        if (i_slotNo == dComIfGs_getSelectItemIndex(i)) {
            dComIfGp_setSelectItem(i);
        }
    }
}

u8 dSv_player_item_c::getItem(int i_slotNo, bool i_checkCombo) const {
    if (i_slotNo < MAX_ITEM_SLOTS) {
        if (i_checkCombo) {
            for (int i = 0; i < SELECT_ITEM_NUM; i++) {
                if ((i_slotNo == dComIfGs_getSelectItemIndex(i) || i_slotNo == dComIfGs_getMixItemIndex(i)) &&
                    dComIfGs_getMixItemIndex(i) != fpcNm_ITEM_NONE)
                {
                    u8 select_item = mItems[dComIfGs_getSelectItemIndex(i)];
                    u8 mix_item = mItems[dComIfGs_getMixItemIndex(i)];

                    // Get Bomb arrow check: Bow + Normal Bombs
                    if ((select_item == fpcNm_ITEM_BOW && mix_item == fpcNm_ITEM_NORMAL_BOMB) ||
                        (mix_item == fpcNm_ITEM_BOW && select_item == fpcNm_ITEM_NORMAL_BOMB)) {
                        return fpcNm_ITEM_BOMB_ARROW;
                    }

                    // Get Bomb arrow check: Bow + Water Bombs
                    if ((select_item == fpcNm_ITEM_BOW && mix_item == fpcNm_ITEM_WATER_BOMB) ||
                        (mix_item == fpcNm_ITEM_BOW && select_item == fpcNm_ITEM_WATER_BOMB)) {
                        return fpcNm_ITEM_BOMB_ARROW;
                    }

                    // Get Bomb arrow check: Bow + Bomblings
                    if ((select_item == fpcNm_ITEM_BOW && mix_item == fpcNm_ITEM_POKE_BOMB) ||
                        (mix_item == fpcNm_ITEM_BOW && select_item == fpcNm_ITEM_POKE_BOMB)) {
                        return fpcNm_ITEM_BOMB_ARROW;
                    }

                    // Get Hawkeye check
                    if ((select_item == fpcNm_ITEM_BOW && mix_item == fpcNm_ITEM_HAWK_EYE) ||
                        (mix_item == fpcNm_ITEM_BOW && select_item == fpcNm_ITEM_HAWK_EYE)) {
                        return fpcNm_ITEM_HAWK_ARROW;
                    }

                    // Get Rod w/ bee larva
                    if ((select_item == fpcNm_ITEM_FISHING_ROD_1 && mix_item == fpcNm_ITEM_BEE_CHILD) ||
                        (mix_item == fpcNm_ITEM_FISHING_ROD_1 && select_item == fpcNm_ITEM_BEE_CHILD)) {
                        return fpcNm_ITEM_BEE_ROD;
                    }

                    // Get Rod w/ coral earring
                    if ((select_item == fpcNm_ITEM_FISHING_ROD_1 && mix_item == fpcNm_ITEM_ZORAS_JEWEL) ||
                        (mix_item == fpcNm_ITEM_FISHING_ROD_1 && select_item == fpcNm_ITEM_ZORAS_JEWEL)) {
                        return fpcNm_ITEM_JEWEL_ROD;
                    }

                    // Get Rod w/ worm
                    if ((select_item == fpcNm_ITEM_FISHING_ROD_1 && mix_item == fpcNm_ITEM_WORM) ||
                        (mix_item == fpcNm_ITEM_FISHING_ROD_1 && select_item == fpcNm_ITEM_WORM)) {
                        return fpcNm_ITEM_WORM_ROD;
                    }

                    // This block will only run on the Wii version
                    if (i == SELECT_ITEM_B) {
                        OS_REPORT("合成アイテム不定２＝＝＝＞%d, %d\n", dComIfGs_getSelectItemIndex(i), dComIfGs_getMixItemIndex(i));

                        if (dComIfGs_getSelectItemIndex(i) == 0 &&
                            dComIfGs_getMixItemIndex(i) == 0)
                        {
                            dComIfGs_setSelectItemIndex(i, fpcNm_ITEM_NONE);
                            dComIfGs_setMixItemIndex(i, fpcNm_ITEM_NONE);
                            return fpcNm_ITEM_NONE;
                        }
                    }
                    // Uncertain combination item＝＝＝＞%d, %d\n
                    OSReport_Error("合成アイテム不定＝＝＝＞%d, %d\n", select_item, mix_item);
                    JUT_ASSERT(731, FALSE);
                }
            }
        }
        return mItems[i_slotNo];
    }

    return fpcNm_ITEM_NONE;
}

void dSv_player_item_c::setLineUpItem() {
    static u8 i_item_lst[23] = {
        10, 8,  6,  2,  9,  4,  3,
        0,  1,  23, 20, 5,  15, 16,
        17, 11, 12, 13, 14, 19, 18,
        22, 21
    };

    int slot_idx = 0;

    for (int i = 0; i < 24; i++) {
        mItemSlots[i] = fpcNm_ITEM_NONE;
    }

    for (int i = 0; i < 23; i++) {
        u8 current = i_item_lst[i];
        if (mItems[current] != fpcNm_ITEM_NONE) {
            mItemSlots[slot_idx] = current;
            slot_idx++;
        }
    }
}

u8 dSv_player_item_c::getLineUpItem(int i_slotNo) const {
    if (i_slotNo < MAX_ITEM_SLOTS) {
        return mItemSlots[i_slotNo];
    }

    return fpcNm_ITEM_NONE;
}

void dSv_player_item_c::setBottleItemIn(u8 curItemIn, u8 newItemIn) {
    curItemIn = dSv_item_rename(curItemIn);
    newItemIn = dSv_item_rename(newItemIn);

    for (int i = 0; i < 4; i++) {
        if (curItemIn == mItems[i + SLOT_11]) {
            setItem(i + SLOT_11, newItemIn);
            if (newItemIn == fpcNm_ITEM_HOT_SPRING) {
                dMeter2Info_setHotSpringTimer(i + SLOT_11);
            }

            for (int j = 0; j < 3; j++) {
                if (i + SLOT_11 == dComIfGs_getSelectItemIndex(j)) {
                    dComIfGp_setSelectItem(j);
                }
            }
            return;
        }
    }
}

void dSv_player_item_c::setEmptyBottleItemIn(u8 i_itemNo) {
    i_itemNo = dSv_item_rename(i_itemNo);
    setBottleItemIn(fpcNm_ITEM_EMPTY_BOTTLE, i_itemNo);
}

void dSv_player_item_c::setEmptyBottle() {
    for (int i = 0; i < 4; i++) {
        if (dComIfGs_getItem((u8)(i + SLOT_11), true) == fpcNm_ITEM_NONE) {
            dComIfGs_setItem((u8)(i + SLOT_11), fpcNm_ITEM_EMPTY_BOTTLE);
            return;
        }
    }
}

void dSv_player_item_c::setEmptyBottle(u8 i_itemNo) {
    for (int i = 0; i < 4; i++) {
        if (dComIfGs_getItem((u8)(i + SLOT_11), true) == fpcNm_ITEM_NONE) {
            dComIfGs_setItem((u8)(i + SLOT_11), i_itemNo);
            return;
        }
    }
}

void dSv_player_item_c::setEquipBottleItemIn(u8 curItemIn, u8 newItemIn) {
    newItemIn = dSv_item_rename(newItemIn);

    if (dComIfGs_getSelectItemIndex(curItemIn) >= SLOT_11 &&
        dComIfGs_getSelectItemIndex(curItemIn) <= SLOT_14) {
        if (newItemIn == fpcNm_ITEM_HOT_SPRING) {
            dMeter2Info_setHotSpringTimer(dComIfGs_getSelectItemIndex(curItemIn));
        }

        setItem(dComIfGs_getSelectItemIndex(curItemIn), newItemIn);
        dComIfGs_setItem(dComIfGs_getSelectItemIndex(curItemIn), newItemIn);
        dComIfGp_setItem(dComIfGs_getSelectItemIndex(curItemIn), newItemIn);
        dComIfGp_setSelectItem(curItemIn);
    }
}

void dSv_player_item_c::setEquipBottleItemEmpty(u8 curItemIn) {
    setEquipBottleItemIn(curItemIn, fpcNm_ITEM_EMPTY_BOTTLE);
}

u8 dSv_player_item_c::checkBottle(u8 i_itemNo) {
    u8 num_bottles = 0;
    i_itemNo = dSv_item_rename(i_itemNo);

    for (int i = 0; i < BOTTLE_MAX; i++) {
        if (i_itemNo == mItems[i + SLOT_11]) {
            num_bottles++;
        }
    }
    return num_bottles;
}

BOOL dSv_player_item_c::checkInsectBottle() {
    for (int i = 0; i < 24; i++) {
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_M_BEETLE + i) &&
            !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x191 + i])) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 dSv_player_item_c::checkEmptyBottle() {
    u8 bottleNum = 0;

    for (int i = 0; i < BOTTLE_MAX; i++) {
        if (mItems[i + SLOT_11] == fpcNm_ITEM_EMPTY_BOTTLE) {
            bottleNum++;
        }
    }
    return bottleNum;
}

void dSv_player_item_c::setBombBagItemIn(u8 i_curBomb, u8 i_newBomb, bool i_setNum) {
    for (int i = 0; i < 3; i++) {
        if (i_curBomb == mItems[i + SLOT_15]) {
            setItem(i + SLOT_15, i_newBomb);

            if (i_setNum == true && i_newBomb != fpcNm_ITEM_BOMB_BAG_LV1) {
                dComIfGs_setBombNum(i, dComIfGs_getBombMax(i_newBomb));
            }

            for (int j = 0; j < 3; j++) {
                if (i + SLOT_15 == dComIfGs_getSelectItemIndex(j)) {
                    dComIfGp_setSelectItem(j);
                }
            }
            break;
        }
    }
}

void dSv_player_item_c::setBombBagItemIn(u8 i_curBomb, u8 i_newBomb, u8 i_bombNum, bool i_setNum) {
    for (int i = 0; i < 3; i++) {
        if (i_curBomb == mItems[i + SLOT_15]) {
            setItem(i + SLOT_15, i_newBomb);

            if (i_setNum == 1 && i_newBomb != fpcNm_ITEM_BOMB_BAG_LV1) {
                if (i_bombNum > dComIfGs_getBombMax(i_newBomb)) {
                    i_bombNum = dComIfGs_getBombMax(i_newBomb);
                }
                dComIfGs_setBombNum(i, i_bombNum);
            }

            for (int j = 0; j < 3; j++) {
                if (i + SLOT_15 == dComIfGs_getSelectItemIndex(j)) {
                    dComIfGp_setSelectItem(j);
                }
            }
            break;
        }
    }
}

void dSv_player_item_c::setEmptyBombBagItemIn(u8 i_newBomb, bool i_setNum) {
    setBombBagItemIn(fpcNm_ITEM_BOMB_BAG_LV1, i_newBomb, i_setNum);
}

void dSv_player_item_c::setEmptyBombBagItemIn(u8 i_newBomb, u8 i_bombNum, bool i_setNum) {
    setBombBagItemIn(fpcNm_ITEM_BOMB_BAG_LV1, i_newBomb, i_bombNum, i_setNum);
}

void dSv_player_item_c::setEmptyBombBag() {
    for (int i = 0; i < 3; i++) {
        if (dComIfGs_getItem((u8)(i + SLOT_15), true) == fpcNm_ITEM_NONE) {
            dComIfGs_setItem((u8)(i + SLOT_15), fpcNm_ITEM_BOMB_BAG_LV1);
            return;
        }
    }
}

void dSv_player_item_c::setEmptyBombBag(u8 i_newBomb, u8 i_bombNum) {
    for (int i = 0; i < 3; i++) {
        if (dComIfGs_getItem((u8)(i + SLOT_15), true) == fpcNm_ITEM_NONE) {
            dComIfGs_setItem((u8)(i + SLOT_15), i_newBomb);

            if (i_newBomb != fpcNm_ITEM_BOMB_BAG_LV1) {
                if (i_bombNum > dComIfGs_getBombMax(i_newBomb)) {
                    i_bombNum = dComIfGs_getBombMax(i_newBomb);
                }
    
                dComIfGs_setBombNum(i, i_bombNum);
            }
            return;
        }
    }
}

u8 dSv_player_item_c::checkBombBag(u8 i_itemNo) {
    u8 bombBags = 0;

    for (int i = 0; i < dSv_player_item_c::BOMB_BAG_MAX; i++) {
        if (i_itemNo == mItems[i + SLOT_15]) {
            bombBags++;
        }
    }
    return bombBags;
}

void dSv_player_item_c::setWarashibeItem(u8 i_itemNo) {
    dComIfGs_setItem(SLOT_21, i_itemNo);
    dComIfGp_setItem(SLOT_21, i_itemNo);

    for (int i = 0; i < 4; i++) {
        if (dComIfGs_getSelectItemIndex((u8)i) == SLOT_21) {
            dComIfGp_setSelectItem((u8)i);
        }
    }
}

void dSv_player_item_c::setRodTypeLevelUp() {
    switch (mItems[SLOT_20]) {
    case fpcNm_ITEM_BEE_ROD: {
        mItems[SLOT_20] = fpcNm_ITEM_JEWEL_BEE_ROD;
        break;
    }
    case fpcNm_ITEM_WORM_ROD: {
        mItems[SLOT_20] = fpcNm_ITEM_JEWEL_WORM_ROD;
        break;
    }
    case fpcNm_ITEM_FISHING_ROD_1: {
        mItems[SLOT_20] = fpcNm_ITEM_JEWEL_ROD;
        break;
    }
    }

    for (int i = 0; i < 4; i++) {
        dComIfGp_setSelectItem(i);
    }
}

void dSv_player_item_c::setBaitItem(u8 i_itemNo) {
    switch (i_itemNo) {
    case fpcNm_ITEM_BEE_CHILD: {
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_ZORAS_JEWEL)) {
            mItems[SLOT_20] = fpcNm_ITEM_JEWEL_BEE_ROD;
        } else {
            mItems[SLOT_20] = fpcNm_ITEM_BEE_ROD;
        }
        break;
    }
    case fpcNm_ITEM_WORM: {
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_ZORAS_JEWEL)) {
            mItems[SLOT_20] = fpcNm_ITEM_JEWEL_WORM_ROD;
        } else {
            mItems[SLOT_20] = fpcNm_ITEM_WORM_ROD;
        }
        break;
    }
    case fpcNm_ITEM_NONE: {
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_ZORAS_JEWEL)) {
            mItems[SLOT_20] = fpcNm_ITEM_JEWEL_ROD;
        } else {
            mItems[SLOT_20] = fpcNm_ITEM_FISHING_ROD_1;
        }
        break;
    }
    }

    for (int i = 0; i < 4; i++) {
        dComIfGp_setSelectItem(i);
    }
}

void dSv_player_get_item_c::init() {
    for (int i = 0; i < 8; i++) {
        mItemFlags[i] = 0;
    }
}

void dSv_player_get_item_c::onFirstBit(u8 i_itemno) {
    JUT_ASSERT(1563, 0 <= i_itemno && i_itemno < ITEM_BIT_MAX);

    int index = i_itemno / 32;
    int bit = i_itemno % 32;
    mItemFlags[index] |= (1 << bit);
}

void dSv_player_get_item_c::offFirstBit(u8 i_itemno) {
    JUT_ASSERT(1581, 0 <= i_itemno && i_itemno < ITEM_BIT_MAX);

    int index = i_itemno / 32;
    int bit = i_itemno % 32;
    mItemFlags[index] &= ~(1 << bit);
}

BOOL dSv_player_get_item_c::isFirstBit(u8 i_itemno) const {
    JUT_ASSERT(1599, 0 <= i_itemno && i_itemno < ITEM_BIT_MAX);

    int index = i_itemno / 32;
    int bit = i_itemno % 32;
    return mItemFlags[index] & (1 << bit) ? TRUE : FALSE;
}

void dSv_player_item_record_c::init() {
    mArrowNum = 0;

    for (int i = 0; i < 3; i++) {
        mBombNum[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        mBottleNum[i] = 0;
    }

    mPachinkoNum = 0;

    for (int i = 0; i < 3; i++) {
        unk5[i] = 0;
    }
}

void dSv_player_item_record_c::setBombNum(u8 i_bagIdx, u8 i_bombNum) {
#if VERSION == VERSION_SHIELD_DEBUG
    if (i_bagIdx == 8) {
        return;
    }
#endif

    JUT_ASSERT(1651, 0 <= i_bagIdx && i_bagIdx < dSv_player_item_c::BOMB_BAG_MAX);
    mBombNum[i_bagIdx] = i_bombNum;
}

u8 dSv_player_item_record_c::getBombNum(u8 i_bagIdx) const {
    JUT_ASSERT(1718, 0 <= i_bagIdx && i_bagIdx < dSv_player_item_c::BOMB_BAG_MAX);
    return mBombNum[i_bagIdx];
}

void dSv_player_item_record_c::setBottleNum(u8 i_bottleIdx, u8 i_bottleNum) {
    JUT_ASSERT(1733, 0 <= i_bottleIdx && i_bottleIdx < dSv_player_item_c::BOTTLE_MAX);
    mBottleNum[i_bottleIdx] = i_bottleNum;
}

u8 dSv_player_item_record_c::addBottleNum(u8 i_bottleIdx, s16 i_no) {
    JUT_ASSERT(1748, 0 <= i_bottleIdx && i_bottleIdx < dSv_player_item_c::BOTTLE_MAX);
    int bottleNum = mBottleNum[i_bottleIdx] + i_no;

    u8 var_r28 = dComIfGs_getItem((u8)(i_bottleIdx + SLOT_11), true);

    if (bottleNum < 0) {
        mBottleNum[i_bottleIdx] = 0;
    } else if (bottleNum > dComIfGs_getBottleMax()) {
        mBottleNum[i_bottleIdx] = dComIfGs_getBottleMax();
    } else {
        mBottleNum[i_bottleIdx] = bottleNum;
    }

    return mBottleNum[i_bottleIdx];
}

u8 dSv_player_item_record_c::getBottleNum(u8 i_bottleIdx) const {
    JUT_ASSERT(1776, 0 <= i_bottleIdx && i_bottleIdx < dSv_player_item_c::BOTTLE_MAX);
    return mBottleNum[i_bottleIdx];
}

void dSv_player_item_max_c::init() {
    for (int i = 0; i < 7; i++) {
        mItemMax[i] = 30;
    }
    setBombNum(fpcNm_ITEM_NORMAL_BOMB, 30);
    setBombNum(fpcNm_ITEM_WATER_BOMB, 15);
    setBombNum(fpcNm_ITEM_POKE_BOMB, 10);
    mItemMax[7] = 0;
}

void dSv_player_item_max_c::setBombNum(u8 i_bombType, u8 i_maxNum) {
    switch (i_bombType) {
    case fpcNm_ITEM_NORMAL_BOMB:
        mItemMax[NORMAL_BOMB_MAX] = i_maxNum;
        return;
    case fpcNm_ITEM_WATER_BOMB:
        mItemMax[WATER_BOMB_MAX] = i_maxNum;
        return;
    case fpcNm_ITEM_POKE_BOMB:
        mItemMax[POKE_BOMB_MAX] = i_maxNum;
    }
}

u8 dSv_player_item_max_c::getBombNum(u8 i_bombType) const {
    u8 lv_multiplier = 1;
    if (dComIfGs_isItemFirstBit(fpcNm_ITEM_BOMB_BAG_LV2)) {
        lv_multiplier = 2;
    }

    switch (i_bombType) {
    case fpcNm_ITEM_NORMAL_BOMB:
        return (u8)(mItemMax[NORMAL_BOMB_MAX] * lv_multiplier);
    case fpcNm_ITEM_WATER_BOMB:
        return (u8)(mItemMax[WATER_BOMB_MAX] * lv_multiplier);
    case fpcNm_ITEM_POKE_BOMB:
        return (u8)(mItemMax[POKE_BOMB_MAX] * lv_multiplier);
    default:
        return 0;
    }
}

void dSv_player_collect_c::init() {
    for (int i = 0; i < 8; i++) {
        mItem[i] = 0;
    }

    unk8 = 0;
    mCrystal = 0;
    mMirror = 0;
    unk11 = 0xFF;
    mPohNum = 0;
}

void dSv_player_collect_c::setCollect(int i_item_type, u8 i_item) {
    JUT_ASSERT(1894, 0 <= i_item && i_item < 8);
    mItem[i_item_type] |= (u8)(1 << i_item);
}

void dSv_player_collect_c::offCollect(int i_item_type, u8 i_item) {
    JUT_ASSERT(1909, 0 <= i_item && i_item < 8);
    mItem[i_item_type] &= (u8)~(u8)(1 << i_item);
}

BOOL dSv_player_collect_c::isCollect(int i_item_type, u8 i_item) const {
    JUT_ASSERT(1924, 0 <= i_item && i_item < 8);
    return mItem[i_item_type] & (u8)(1 << i_item) ? TRUE : FALSE;
}

void dSv_player_collect_c::onCollectCrystal(u8 i_item) {
    JUT_ASSERT(1956, 0 <= i_item && i_item < 8);
    mCrystal |= (u8)(1 << i_item);
}

void dSv_player_collect_c::offCollectCrystal(u8 i_item) {
    JUT_ASSERT(1971, 0 <= i_item && i_item < 8);
    mCrystal &= (u8)~(u8)(1 << i_item);
}

BOOL dSv_player_collect_c::isCollectCrystal(u8 i_item) const {
    JUT_ASSERT(1986, 0 <= i_item && i_item < 8);
    return mCrystal & (u8)(1 << i_item) ? TRUE : FALSE;
}

void dSv_player_collect_c::onCollectMirror(u8 i_item) {
    JUT_ASSERT(2001, 0 <= i_item && i_item < 8);
    mMirror |= (u8)(1 << i_item);
}

void dSv_player_collect_c::offCollectMirror(u8 i_item) {
    JUT_ASSERT(2016, 0 <= i_item && i_item < 8);
    mMirror &= (u8)~(u8)(1 << i_item);
}

BOOL dSv_player_collect_c::isCollectMirror(u8 i_item) const {
    JUT_ASSERT(2031, 0 <= i_item && i_item < 8);
    return mMirror & (u8)(1 << i_item) ? TRUE : FALSE;
}

void dSv_player_wolf_c::init() {
    for (int i = 0; i < 3; i++) {
        unk0[i] = 0;
    }

    unk3 = 0;
}

void dSv_light_drop_c::init() {
    for (int i = 0; i < 4; i++) {
        mLightDropNum[i] = 0;
    }

    mLightDropGetFlag = 0;

    for (int i = 0; i < 3; i++) {
        unk5[i] = 0;
    }
}

void dSv_light_drop_c::setLightDropNum(u8 i_nowLevel, u8 i_dropNum) {
    if (i_nowLevel < LIGHT_DROP_STAGE || i_nowLevel > 6) {
        JUT_ASSERT(2089, (i_nowLevel >= 0) && (i_nowLevel < LIGHT_DROP_STAGE));
        mLightDropNum[i_nowLevel] = i_dropNum;
    }
}

u8 dSv_light_drop_c::getLightDropNum(u8 i_nowLevel) const {
    if (i_nowLevel >= LIGHT_DROP_STAGE && i_nowLevel <= 6) {
        return 0;
    }

    JUT_ASSERT(2107, (i_nowLevel >= 0) && (i_nowLevel < LIGHT_DROP_STAGE));
    return mLightDropNum[i_nowLevel];
}

void dSv_light_drop_c::onLightDropGetFlag(u8 i_nowLevel) {
    if (i_nowLevel < LIGHT_DROP_STAGE || i_nowLevel > 6) {
        JUT_ASSERT(2208, (i_nowLevel >= 0) && (i_nowLevel < LIGHT_DROP_STAGE));
        mLightDropGetFlag |= (u8)(1 << i_nowLevel);
    }
}

void dSv_light_drop_c::offLightDropGetFlag(u8 i_nowLevel) {
    if (i_nowLevel < LIGHT_DROP_STAGE || i_nowLevel > 6) {
        JUT_ASSERT(2227, (i_nowLevel >= 0) && (i_nowLevel < LIGHT_DROP_STAGE));
        mLightDropGetFlag &= (u8)~(u8)(1 << i_nowLevel);
    }
}

BOOL dSv_light_drop_c::isLightDropGetFlag(u8 i_nowLevel) const {
    if (i_nowLevel >= LIGHT_DROP_STAGE && i_nowLevel <= 6) {
        return 0;
    }

    JUT_ASSERT(2246, (i_nowLevel >= 0) && (i_nowLevel < LIGHT_DROP_STAGE));
    return mLightDropGetFlag & (u8)(1 << i_nowLevel) ? TRUE : FALSE;
}

void dSv_letter_info_c::init() {
    for (int i = 0; i < 2; i++) {
        mLetterGetFlags[i] = 0;
        mLetterReadFlags[i] = 0;
    }

    for (int i = 0; i < LETTER_INFO_BIT; i++) {
        mGetNumber[i] = 0;
    }
}

void dSv_letter_info_c::onLetterGetFlag(int i_no) {
    JUT_ASSERT(2281, (i_no >= 0) && (i_no < LETTER_INFO_BIT));
    mLetterGetFlags[i_no >> 5] |= 1 << (i_no & 0x1F);
}

BOOL dSv_letter_info_c::isLetterGetFlag(int i_no) const {
    JUT_ASSERT(2311, (i_no >= 0) && (i_no < LETTER_INFO_BIT));
    return mLetterGetFlags[i_no >> 5] & (1 << (i_no & 0x1F)) ? TRUE : FALSE;
}

void dSv_letter_info_c::onLetterReadFlag(int i_no) {
    JUT_ASSERT(2326, (i_no >= 0) && (i_no < LETTER_INFO_BIT));
    mLetterReadFlags[i_no >> 5] |= 1 << (i_no & 0x1F);
}

BOOL dSv_letter_info_c::isLetterReadFlag(int i_no) const {
    JUT_ASSERT(2356, (i_no >= 0) && (i_no < LETTER_INFO_BIT));
    return mLetterReadFlags[i_no >> 5] & 1 << (i_no & 0x1F) ? TRUE : FALSE;
}

void dSv_fishing_info_c::init() {
    for (int i = 0; i < 16; i++) {
        mFishCount[i] = 0;
        mMaxSize[i] = 0;
    }
}

void dSv_fishing_info_c::addFishCount(u8 i_fishIdx) {
    if (mFishCount[i_fishIdx] < 999) {
        mFishCount[i_fishIdx]++;
    }
}

void dSv_player_info_c::init() {
    dMeter2Info_getString(0x382, mPlayerName, NULL);  // Link
    dMeter2Info_getString(0x383, mHorseName, NULL);   // Epona

    unk0 = 0;
    mTotalTime = 0;
    unk16 = 0;
    mDeathCount = 0;
    mClearCount = 0;

    for (int i = 0; i < 5; i++) {
        unk55[i] = 0;
    }
}

extern "C" u8 SCGetLanguage();

void dSv_player_config_c::init() {
#if VERSION == VERSION_GCN_JPN
    unk0 = 0;
#else
    unk0 = 1;
#endif

#if PLATFORM_GCN
    if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
        mSoundMode = OS_SOUND_MODE_MONO;
        Z2AudioMgr::mAudioMgrPtr->setOutputMode(OS_SOUND_MODE_MONO);
    } else {
        mSoundMode = OS_SOUND_MODE_STEREO;
        Z2AudioMgr::mAudioMgrPtr->setOutputMode(OS_SOUND_MODE_STEREO);
    }
#endif

    mAttentionType = 0;
    mVibration = 1;

#if VERSION == VERSION_GCN_PAL
    mLanguage = OSGetLanguage();
#elif VERSION == VERSION_SHIELD_DEBUG
    mLanguage = SCGetLanguage();
#else
    mLanguage = 0;
#endif

    unk5 = 0;
    mShortCut = 0;
    mCalibrateDist = 350;
    mCalValue = 0;
    mCameraControl = 0;
    mPointer = 1;
}

u32 dSv_player_config_c::checkVibration() const {
#if PLATFORM_GCN
    return JUTGamePad::sRumbleSupported & 0x80000000 ? dComIfGp_getNowVibration() : 0;
#else
    return dComIfGp_getNowVibration();
#endif
}

u8 dSv_player_config_c::getSound() {
    return mSoundMode;
}

void dSv_player_config_c::setSound(u8 i_mode) {
    mSoundMode = i_mode;
}

u8 dSv_player_config_c::getVibration() {
    return mVibration;
}

void dSv_player_config_c::setVibration(u8 i_status) {
    mVibration = i_status;
}

u8 dSv_player_config_c::getPalLanguage() const {
#if VERSION == VERSION_GCN_PAL
    switch (OSGetLanguage()) {
    case 0:
        return LANGAUGE_ENGLISH;
    case 1:
        return LANGAUGE_GERMAN;
    case 2:
        return LANGAUGE_FRENCH;
    case 3:
        return LANGAUGE_SPANISH;
    case 4:
        return LANGAUGE_ITALIAN;
    }
#elif VERSION == VERSION_SHIELD_DEBUG
    switch (SCGetLanguage()) {
    case 1:
        return 0;
    case 3:
        return 2;
    case 4:
        return 3;
    }
#endif

    return 0;
}

void dSv_player_c::init() {
    mPlayerStatusA.init();
    mPlayerStatusB.init();
    mHorsePlace.init();
    mPlayerReturnPlace.init();
    mPlayerFieldLastStayInfo.init();
    mPlayerLastMarkInfo.init();
    mItem.init();
    mGetItem.init();
    mItemRecord.init();
    mItemMax.init();
    mCollect.init();
    mWolf.init();
    mLightDrop.init();
    mLetterInfo.init();
    mFishingInfo.init();
    mPlayerInfo.init();
    mConfig.init();
}

void dSv_memBit_c::init() {
    for (int i = 0; i < 2; i++) {
        mTbox[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        mSwitch[i] = 0;
    }

    for (int i = 0; i < 1; i++) {
        mItem[i] = 0;
    }

    mKeyNum = 0;
    mDungeonItem = 0;
}

void dSv_memBit_c::onTbox(int i_no) {
    JUT_ASSERT(2726, 0 <= i_no && i_no < TBOX_MAX);
    mTbox[i_no >> 5] |= 1 << (i_no & 0x1F);
}

void dSv_memBit_c::offTbox(int i_no) {
    JUT_ASSERT(2740, 0 <= i_no && i_no < TBOX_MAX);
    mTbox[i_no >> 5] &= ~(1 << (i_no & 0x1F));
}

BOOL dSv_memBit_c::isTbox(int i_no) const {
    JUT_ASSERT(2754, 0 <= i_no && i_no < TBOX_MAX);
    return 1 << (i_no & 0x1f) & mTbox[i_no >> 5] ? TRUE : FALSE;
}

void dSv_memBit_c::onSwitch(int i_no) {
    JUT_ASSERT(2786, 0 <= i_no && i_no < 128);
    mSwitch[i_no >> 5] |= 1 << (i_no & 0x1F);
}

void dSv_memBit_c::offSwitch(int i_no) {
    JUT_ASSERT(2800, 0 <= i_no && i_no < 128);
    mSwitch[i_no >> 5] &= ~(1 << (i_no & 0x1F));
}

BOOL dSv_memBit_c::isSwitch(int i_no) const {
    JUT_ASSERT(2814, 0 <= i_no && i_no < 128);
    return (mSwitch[i_no >> 5] & 1 << (i_no & 0x1F)) ? TRUE : FALSE;
}

BOOL dSv_memBit_c::revSwitch(int i_no) {
    JUT_ASSERT(2828, 0 <= i_no && i_no < 128);

    u32 switchInd = i_no >> 5;
    u32 tmp = 1 << (i_no & 0x1F);
    mSwitch[switchInd] ^= tmp;
    return mSwitch[switchInd] & tmp ? TRUE : FALSE;
}

void dSv_memBit_c::onItem(int i_no) {
    JUT_ASSERT(2847, 0 <= i_no && i_no < 64);
    mItem[i_no >> 5] |= 1 << (i_no & 0x1F);
}

void dSv_memBit_c::offItem(int i_no) {
    JUT_ASSERT(2861, 0 <= i_no && i_no < 64);
    mItem[i_no >> 5] |= ~(1 << (i_no & 0x1F));
}

BOOL dSv_memBit_c::isItem(int i_no) const {
    JUT_ASSERT(2875, 0 <= i_no && i_no < 64);
    return (mItem[i_no >> 5] & 1 << (i_no & 0x1F)) ? TRUE : FALSE;
}

void dSv_memBit_c::onDungeonItem(int i_no) {
    JUT_ASSERT(2969, 0 <= i_no && i_no < DSV_MEMBIT_ENUM_MAX);
    mDungeonItem |= (u8)(1 << i_no);
}

void dSv_memBit_c::offDungeonItem(int i_no) {
    JUT_ASSERT(2983, 0 <= i_no && i_no < DSV_MEMBIT_ENUM_MAX);
    mDungeonItem &= (u8)~(u8)(1 << i_no);
}

s32 dSv_memBit_c::isDungeonItem(int i_no) const {
    JUT_ASSERT(2998, 0 <= i_no && i_no < DSV_MEMBIT_ENUM_MAX);
    return mDungeonItem & (u8)(1 << i_no) ? TRUE : FALSE;
}

void dSv_event_c::init() {
    int i;
    for (i = 0; i < MAX_EVENTS; i++) {
        mEvent[i] = 0;
    }

    setInitEventBit();
}

void dSv_event_c::onEventBit(u16 i_no) {
    mEvent[i_no >> 8] |= (u8)i_no;
}

void dSv_event_c::offEventBit(u16 i_no) {
    mEvent[i_no >> 8] &= ~(u8)i_no;
}

BOOL dSv_event_c::isEventBit(const u16 i_no) const {
    return mEvent[i_no >> 8] & (i_no & 0xFF) ? TRUE : FALSE;
}

void dSv_event_c::setEventReg(u16 i_reg, u8 i_no) {
    mEvent[i_reg >> 8] &= ~(u8)i_reg;
    mEvent[i_reg >> 8] |= i_no;
}

u8 dSv_event_c::getEventReg(u16 i_reg) const {
    return (u8)i_reg & mEvent[i_reg >> 8];
}

void dSv_MiniGame_c::init() {
    for (int i = 0; i < 1; i++) {
        unk0[i][0] = 0;
        for (int j = 0; j < 3; j++) {
            unk0[i][1 + j] = 0;
        }
    }

    mHookGameTime = 120000;
    mBalloonScore = 0;
    mRaceGameTime = 0;
    unk16 = 0;
    unk20 = 0;
}

void dSv_memory_c::init() {
    mBit.init();
}

void dSv_memory2_c::init() {
    for (int i = 0; i < 2; i++) {
        mVisitedRoom[i] = 0;
    }
}

void dSv_memory2_c::onVisitedRoom(int i_no) {
    JUT_ASSERT(3279, 0 <= i_no && i_no < 64);
    mVisitedRoom[i_no >> 5] |= 1 << (i_no & 0x1F);
}

void dSv_memory2_c::offVisitedRoom(int i_no) {
    JUT_ASSERT(3293, 0 <= i_no && i_no < 64);
    mVisitedRoom[i_no >> 5] &= ~(1 << (i_no & 0x1F));
}

BOOL dSv_memory2_c::isVisitedRoom(int i_no) {
    JUT_ASSERT(3307, 0 <= i_no && i_no < 64);
    return (1 << (i_no & 0x1F) & mVisitedRoom[i_no >> 5]) ? TRUE : FALSE;
}

bool dSv_danBit_c::init(s8 i_stageNo) {
    OS_REPORT("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    OS_REPORT("\n!!! dSv_danBit_c::init i_stage=[%d] mStageNo=[%d]", i_stageNo, mStageNo);
    OS_REPORT("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    if (i_stageNo != mStageNo) {
        mSwitch[0] = 0;
        mSwitch[1] = 0;
        mItem[0] = 0;
        mItem[1] = 0;
        mItem[2] = 0;
        mItem[3] = 0;
        mStageNo = i_stageNo;
        unk1 = 0;

        for (int i = 0; i < 16; i++) {
            unk28[i] = 0xFFFF;
        }

        daObjCarry_c::clrSaveFlag();
        return true;
    } else {
        daObjCarry_c::setSaveFlag();
        return false;
    }
}

void dSv_danBit_c::onSwitch(int i_no) {
    JUT_ASSERT(3384, 0 <= i_no && i_no < 64);
    mSwitch[i_no >> 5] |= 1 << (i_no & 0x1F);
}

void dSv_danBit_c::offSwitch(int i_no) {
    JUT_ASSERT(3398, 0 <= i_no && i_no < 64);
    mSwitch[i_no >> 5] &= ~(1 << (i_no & 0x1F));
}

BOOL dSv_danBit_c::isSwitch(int i_no) const {
    JUT_ASSERT(3412, 0 <= i_no && i_no < 64);
    return mSwitch[i_no >> 5] & (1 << (i_no & 0x1F)) ? TRUE : FALSE;
}

BOOL dSv_danBit_c::revSwitch(int i_no) {
    JUT_ASSERT(3426, 0 <= i_no && i_no < 64);

    int uVar1 = 1 << (i_no & 0x1F);
    mSwitch[i_no >> 5] ^= uVar1;
    return mSwitch[i_no >> 5] & uVar1 ? TRUE : FALSE;
}

void dSv_danBit_c::onItem(int i_no) {
    JUT_ASSERT(3444, 0 <= i_no && i_no < ITEM_MAX_DAN);
    mItem[i_no >> 5] |= 1 << (i_no & 0x1F);
}

void dSv_danBit_c::offItem(int i_no) {
    JUT_ASSERT(3458, 0 <= i_no && i_no < ITEM_MAX_DAN);
    mItem[i_no >> 5] &= ~(1 << (i_no & 0x1F));
}

BOOL dSv_danBit_c::isItem(int i_no) const {
    JUT_ASSERT(3472, 0 <= i_no && i_no < ITEM_MAX_DAN);
    return mItem[i_no >> 5] & 1 << (i_no & 0x1F) ? TRUE : FALSE;
}

#if VERSION == VERSION_SHIELD_DEBUG
static void dummyString() {
    DEAD_STRING("i_no < 6");
}
#endif

void dSv_zoneBit_c::init() {
    for (int i = 0; i < 2; i++) {
        mSwitch[i] = 0;
    }

    for (int i = 0; i < 2; i++) {
        mItem[i] = 0;
    }

    mRoomSwitch = 0;
    mRoomItem = 0;
}

void dSv_zoneBit_c::clearRoomSwitch() {
    mRoomSwitch = 0;
}

void dSv_zoneBit_c::clearRoomItem() {
    mRoomItem = 0;
}

void dSv_zoneBit_c::onSwitch(int i_no) {
    JUT_ASSERT(3587, 0 <= i_no && i_no < SWITCH_ZONE_MAX);
    mSwitch[i_no >> 4] |= (u16)(1 << (i_no & 0xF));
}

void dSv_zoneBit_c::offSwitch(int i_no) {
    JUT_ASSERT(3601, 0 <= i_no && i_no < SWITCH_ZONE_MAX);
    mSwitch[i_no >> 4] &= ~(1 << (i_no & 0xF));
}

BOOL dSv_zoneBit_c::isSwitch(int i_no) const {
    JUT_ASSERT(3615, 0 <= i_no && i_no < SWITCH_ZONE_MAX);
    return mSwitch[i_no >> 4] & 1 << (i_no & 0xF) ? TRUE : FALSE;
}

BOOL dSv_zoneBit_c::revSwitch(int i_no) {
    JUT_ASSERT(3629, 0 <= i_no && i_no < SWITCH_ZONE_MAX);

    u32 switchInd = i_no >> 4;
    int uVar1 = 1 << (i_no & 0xF);
    mSwitch[switchInd] ^= uVar1;
    return mSwitch[switchInd] & uVar1 ? TRUE : FALSE;
}

void dSv_zoneBit_c::onOneSwitch(int i_no) {
    JUT_ASSERT(3649, 0 <= i_no && i_no < SWITCH_ONE_ZONE_MAX);
    mRoomSwitch |= (u16)(1 << i_no);
}

void dSv_zoneBit_c::offOneSwitch(int i_no) {
    JUT_ASSERT(3663, 0 <= i_no && i_no < SWITCH_ONE_ZONE_MAX);
    mRoomSwitch &= ~(1 << i_no);
}

BOOL dSv_zoneBit_c::isOneSwitch(int i_no) const {
    JUT_ASSERT(3677, 0 <= i_no && i_no < SWITCH_ONE_ZONE_MAX);
    return mRoomSwitch & 1 << i_no ? TRUE : FALSE;
}

BOOL dSv_zoneBit_c::revOneSwitch(int i_no) {
    JUT_ASSERT(3691, 0 <= i_no && i_no < SWITCH_ONE_ZONE_MAX);

    int iVar1 = 1 << i_no;
    mRoomSwitch ^= iVar1;
    return mRoomSwitch & iVar1 ? TRUE : FALSE;
}

void dSv_zoneBit_c::onItem(int i_no) {
    JUT_ASSERT(3711, 0 <= i_no && i_no < ITEM_ZONE_MAX);
    mItem[i_no >> 4] |= (u16)(1 << (i_no & 0xF));
}

void dSv_zoneBit_c::offItem(int i_no) {
    JUT_ASSERT(3729, 0 <= i_no && i_no < ITEM_ZONE_MAX);
    mItem[i_no >> 4] &= (u16)~(1 << (i_no & 0xF));
}

BOOL dSv_zoneBit_c::isItem(int i_no) const {
    JUT_ASSERT(3745, 0 <= i_no && i_no < ITEM_ZONE_MAX);
    return mItem[i_no >> 4] & 1 << (i_no & 0xF) ? TRUE : FALSE;
}

void dSv_zoneBit_c::onOneItem(int i_no) {
    JUT_ASSERT(3783, 0 <= i_no && i_no < ITEM_ONE_ZONE_MAX);
    mRoomItem |= (u16)(1 << i_no);
}

void dSv_zoneBit_c::offOneItem(int i_no) {
    JUT_ASSERT(3798, 0 <= i_no && i_no < ITEM_ONE_ZONE_MAX);
    mRoomItem &= (u16)~(1 << i_no);
}

BOOL dSv_zoneBit_c::isOneItem(int i_no) const {
    JUT_ASSERT(3811, 0 <= i_no && i_no < ITEM_ONE_ZONE_MAX);
    return mRoomItem & 1 << i_no ? TRUE : FALSE;
}

void dSv_zoneActor_c::init() {
    for (int i = 0; i < 4; i++) {
        mActorFlags[i] = 0;
    }
}

void dSv_zoneActor_c::on(int i_id) {
    JUT_ASSERT(3856, 0 <= i_id && i_id < ACTOR_MAX);
    mActorFlags[i_id >> 5] |= 1 << (i_id & 0x1F);
}

void dSv_zoneActor_c::off(int i_id) {
    JUT_ASSERT(3870, 0 <= i_id && i_id < ACTOR_MAX);
    mActorFlags[i_id >> 5] &= ~(1 << (i_id & 0x1F));
}

BOOL dSv_zoneActor_c::is(int i_id) const {
    JUT_ASSERT(3884, 0 <= i_id && i_id < ACTOR_MAX);
    return mActorFlags[i_id >> 5] & 1 << (i_id & 0x1F) ? TRUE : FALSE;
}

void dSv_zone_c::init(int i_roomNo) {
    mRoomNo = i_roomNo;
    mBit.init();
    mActor.init();
}

void dSv_restart_c::setRoom(const cXyz& i_position, s16 i_angleY, s8 i_roomNo) {
    mRoomNo = i_roomNo;
    mRoomPos = i_position;
    mRoomAngleY = i_angleY;
}

void dSv_turnRestart_c::set(const cXyz& i_position, s16 i_angleY, s8 param_3, u32 i_param) {
    mPosition = i_position;
    mAngleY = i_angleY;
    unk18 = param_3;
    mParam = i_param;
}

void dSv_info_c::init() {
    mSavedata.init();
    mMemory.init();
    initDan(-1);
    initZone();
    mTmp.init();

#if VERSION == VERSION_SHIELD_DEBUG
    unk_0x0 = 0;
    unk_0x1 = 0;
#endif
}

void dSv_save_c::init() {
    mPlayer.init();
    for (int i = 0; i < STAGE_MAX; i++) {
        mSave[i].init();
    }

    for (int i = 0; i < STAGE2_MAX; i++) {
        mSave2[i].init();
    }

    mEvent.init();
    mMiniGame.init();
}

dSv_memory2_c* dSv_save_c::getSave2(int i_stage2No) {
    JUT_ASSERT(4117, 0 <= i_stage2No && i_stage2No < STAGE2_MAX);
    return &mSave2[i_stage2No];
}

void dSv_info_c::getSave(int i_stageNo) {
    JUT_ASSERT(4133, 0 <= i_stageNo && i_stageNo < dSv_save_c::STAGE_MAX);
    mMemory = mSavedata.getSave(i_stageNo);
}

void dSv_info_c::putSave(int i_stageNo) {
    JUT_ASSERT(4149, 0 <= i_stageNo && i_stageNo < dSv_save_c::STAGE_MAX);
    mSavedata.putSave(i_stageNo, mMemory);
}

void dSv_info_c::initZone() {
    for (int i = 0; i < 0x20; i++) {
        mZone[i].init(-1);
    }
}

u32 dSv_info_c::createZone(int i_roomNo) {
    dSv_zone_c* zone = mZone;
    for (int i = 0; i < 0x20; zone++, i++) {
        if (zone->getRoomNo() < 0) {
            zone->init(i_roomNo);
            return i;
        }
    }
    return -1;
}

void dSv_info_c::onSwitch(int i_no, int i_roomNo) {
    JUT_ASSERT(4210, (0 <= i_no && i_no < (MEMORY_SWITCH+ DAN_SWITCH+ ZONE_SWITCH+ ONEZONE_SWITCH)) || i_no == -1 || i_no == 255);

    if (i_no == -1 || i_no == 255) {
        return;
    }

    if (i_no < MEMORY_SWITCH) {
        mMemory.getBit().onSwitch(i_no);
    } else if (i_no < (MEMORY_SWITCH + DAN_SWITCH)) {
        mDan.onSwitch(i_no - MEMORY_SWITCH);
    } else {
        JUT_ASSERT(4226, 0 <= i_roomNo && i_roomNo < 64);
        int zoneId = dComIfGp_roomControl_getZoneNo(i_roomNo);
        JUT_ASSERT(4228, 0 <= zoneId && zoneId < ZONE_MAX);

        if (i_no < (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH)) {
            mZone[zoneId].getBit().onSwitch(i_no - (MEMORY_SWITCH + DAN_SWITCH));
        } else {
            mZone[zoneId].getBit().onOneSwitch(i_no - (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH));
        }
    }
}

void dSv_info_c::offSwitch(int i_no, int i_roomNo) {
    JUT_ASSERT(4256, (0 <= i_no && i_no < (MEMORY_SWITCH+ DAN_SWITCH+ ZONE_SWITCH+ ONEZONE_SWITCH)) || i_no == -1 || i_no == 255);

    if (i_no == -1 || i_no == 255) {
        return;
    }

    if (i_no < MEMORY_SWITCH) {
        mMemory.getBit().offSwitch(i_no);
    } else if (i_no < (MEMORY_SWITCH + DAN_SWITCH)) {
        mDan.offSwitch(i_no - MEMORY_SWITCH);
    } else {
        JUT_ASSERT(4269, 0 <= i_roomNo && i_roomNo < 64);
        int zoneNo = dComIfGp_roomControl_getZoneNo(i_roomNo);
        JUT_ASSERT(4271, 0 <= zoneNo && zoneNo < ZONE_MAX);

        if (i_no < (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH)) {
            mZone[zoneNo].getBit().offSwitch(i_no - (MEMORY_SWITCH + DAN_SWITCH));
        } else {
            mZone[zoneNo].getBit().offOneSwitch(i_no - (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH));
        }
    }
}

BOOL dSv_info_c::isSwitch(int i_no, int i_roomNo) const {
    if (!((0 <= i_no && i_no < (MEMORY_SWITCH+ DAN_SWITCH+ ZONE_SWITCH+ ONEZONE_SWITCH)) || i_no == -1 || i_no == 255)) {
        OS_REPORT("i_no = %d\n", i_no);
        JUT_ASSERT(4302, FALSE);
    }

    if (i_no == -1 || i_no == 255) {
        return FALSE;
    }

    if (i_no < MEMORY_SWITCH) {
        return mMemory.getBit().isSwitch(i_no);
    } else if (i_no < (MEMORY_SWITCH + DAN_SWITCH)) {
        return mDan.isSwitch(i_no - MEMORY_SWITCH);
    } else {
        JUT_ASSERT(4269, 0 <= i_roomNo && i_roomNo < 64);
        int zoneId = dComIfGp_roomControl_getZoneNo(i_roomNo);

        if (zoneId < 0 || zoneId >= ZONE_MAX) {
            return FALSE;
        } else {
            if (i_no < (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH)) {
                return mZone[zoneId].getBit().isSwitch(i_no - (MEMORY_SWITCH + DAN_SWITCH));
            } else {
                return mZone[zoneId].getBit().isOneSwitch(
                    i_no - (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH));
            }
        }
    }
}

BOOL dSv_info_c::revSwitch(int i_no, int i_roomNo) {
    JUT_ASSERT(4356, (0 <= i_no && i_no < (MEMORY_SWITCH+ DAN_SWITCH+ ZONE_SWITCH+ ONEZONE_SWITCH)) || i_no == -1 || i_no == 255);

    if (i_no == -1 || i_no == 255) {
        return FALSE;
    }

    if (i_no < MEMORY_SWITCH) {
        return mMemory.getBit().revSwitch(i_no);
    } else if (i_no < (MEMORY_SWITCH + DAN_SWITCH)) {
        return mDan.revSwitch(i_no - MEMORY_SWITCH);
    } else {
        JUT_ASSERT(4368, 0 <= i_roomNo && i_roomNo < 64);
        int zoneNo = dComIfGp_roomControl_getZoneNo(i_roomNo);
        JUT_ASSERT(4370, 0 <= zoneNo && zoneNo < ZONE_MAX);

        if (i_no < (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH)) {
            return mZone[zoneNo].getBit().revSwitch(i_no - (MEMORY_SWITCH + DAN_SWITCH));
        } else {
            return mZone[zoneNo].getBit().revOneSwitch(
                i_no - (MEMORY_SWITCH + DAN_SWITCH + ZONE_SWITCH));
        }
    }
}

void dSv_info_c::onItem(int i_no, int i_roomNo) {
    JUT_ASSERT(4398, (0 <= i_no && i_no < (MEMORY_ITEM+ DAN_ITEM+ ZONE_ITEM+ ONEZONE_ITEM)) || i_no == -1 || i_no == 255);

    if (i_no == -1 || i_no == 255) {
        return;
    }

    if (i_no < MEMORY_ITEM) {
        mDan.onItem(i_no);
    } else if (i_no < (MEMORY_ITEM + DAN_ITEM)) {
        mMemory.getBit().onItem(i_no - MEMORY_ITEM);
    } else {
        JUT_ASSERT(4414, 0 <= i_roomNo && i_roomNo < 64);
        int zoneNo = dComIfGp_roomControl_getZoneNo(i_roomNo);
        JUT_ASSERT(4416, 0 <= zoneNo && zoneNo < ZONE_MAX);

        if (i_no < (MEMORY_ITEM + DAN_ITEM + ZONE_ITEM)) {
            mZone[zoneNo].getBit().onItem(i_no - (MEMORY_ITEM + DAN_ITEM));
        } else {
            mZone[zoneNo].getBit().onOneItem(i_no - (MEMORY_ITEM + DAN_ITEM + ZONE_ITEM));
        }
    }
}

BOOL dSv_info_c::isItem(int i_no, int i_roomNo) const {
    JUT_ASSERT(4488, (0 <= i_no && i_no < (MEMORY_ITEM+ DAN_ITEM+ ZONE_ITEM+ ONEZONE_ITEM)) || i_no == -1 || i_no == 255);

    if (i_no == -1 || i_no == 255) {
        return FALSE;
    }

    if (i_no < MEMORY_ITEM) {
        return mDan.isItem(i_no);
    } else if (i_no < (MEMORY_ITEM + DAN_ITEM)) {
        return mMemory.getBit().isItem(i_no - MEMORY_ITEM);
    } else {
        JUT_ASSERT(4501, 0 <= i_roomNo && i_roomNo < 64);
        int zoneNo = dComIfGp_roomControl_getZoneNo(i_roomNo);
        JUT_ASSERT(4503, 0 <= zoneNo && zoneNo < ZONE_MAX);

        if (i_no < (MEMORY_ITEM + DAN_ITEM + ZONE_ITEM)) {
            return mZone[zoneNo].getBit().isItem(i_no - (MEMORY_ITEM + DAN_ITEM));
        } else {
            return mZone[zoneNo].getBit().isOneItem(i_no - (MEMORY_ITEM + DAN_ITEM + ZONE_ITEM));
        }
    }
}

void dSv_info_c::onActor(int i_id, int i_roomNo) {
    if (i_id == -1 || i_id == 0xFFFF || i_roomNo == -1) {
        return;
    }

    JUT_ASSERT(4573, (0 <= i_id && i_id < dSv_zoneActor_c::ACTOR_MAX) && (0 <= i_roomNo && i_roomNo < 64));
    int zoneNo = dComIfGp_roomControl_getZoneNo(i_roomNo);
    JUT_ASSERT(4575, 0 <= zoneNo && zoneNo < ZONE_MAX);
    mZone[zoneNo].getActor().on(i_id);
}

void dSv_info_c::offActor(int i_id, int i_roomNo) {
    if (i_id == -1 || i_id == 0xFFFF || i_roomNo == -1) {
        return;
    }

    JUT_ASSERT(4598, (0 <= i_id && i_id < dSv_zoneActor_c::ACTOR_MAX) && (0 <= i_roomNo && i_roomNo < 64));
    int zoneNo = dComIfGp_roomControl_getZoneNo(i_roomNo);
    JUT_ASSERT(4600, 0 <= zoneNo && zoneNo < ZONE_MAX);
    mZone[zoneNo].getActor().off(i_id);
}

BOOL dSv_info_c::isActor(int i_id, int i_roomNo) const {
    if (i_id == -1 || i_id == 0xFFFF || i_roomNo == -1) {
        return FALSE;
    }

    if (!(0 <= i_id && i_id < dSv_zoneActor_c::ACTOR_MAX)) {
        OS_REPORT("#######isActor=%d\n", i_id);
        JUT_ASSERT(4624, 0 <= i_id && i_id < dSv_zoneActor_c::ACTOR_MAX);
    }

    JUT_ASSERT(4626, 0 <= i_roomNo && i_roomNo < 64);
    int zoneNo = dComIfGp_roomControl_getZoneNo(i_roomNo);
    JUT_ASSERT(4628, 0 <= zoneNo && zoneNo < ZONE_MAX);
    return mZone[zoneNo].getActor().is(i_id);
}

int dSv_info_c::memory_to_card(char* card_ptr, int dataNum) {
    BOOL lantern_stolen;
    BOOL lantern_dropped;

    OSTime start;
    OSTime time;
    OSTime play_time;
    
    bool lantern_not_recovered = false;
    bool tmp_lantern_check = false;
    u16 current_lantern_oil = 0;

    #if VERSION == VERSION_SHIELD
    lingcod_revalidateNVSI();
    #endif
    
    // If haven't gotten then lantern back from the monkey
    if (!dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[226])) {
        // Store whether or not it's stolen and dropped
        lantern_dropped = dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[224]);
        lantern_stolen = dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[225]);

        // Then turn those events off
        dComIfGs_offEventBit((u16)dSv_event_flag_c::saveBitLabels[224]);
        dComIfGs_offEventBit((u16)dSv_event_flag_c::saveBitLabels[225]);

        // Used to turn events back on later
        lantern_not_recovered = true;
    }

    // If have the lantern and Slot 1 in the item wheel is blank
    if (dComIfGs_isItemFirstBit(fpcNm_ITEM_KANTERA) && dComIfGs_getItem(SLOT_1, 1) == fpcNm_ITEM_NONE) {
        // Set Slot 1 to Lantern and fetch current oil
        dComIfGs_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        current_lantern_oil = dComIfGs_getOil();

        // Set oil to oil gauge backup
        dComIfGs_setOil(dMeter2Info_getOilGaugeBackUp());

        // Used to set slot 1 back later
        tmp_lantern_check = true;
    }

    card_ptr += dataNum * QUEST_LOG_SIZE;
    char* var_r29 = card_ptr;

    dSv_save_c* savedata = dComIfGs_getSaveData();

    start = dComIfGs_getSaveStartTime();
    time = OSGetTime();

    play_time = (time - start) + dComIfGs_getSaveTotalTime();

    // 3599940 = 999:59 in seconds
    if (dComIfGs_getSaveTotalTime() / OS_TIMER_CLOCK < 3599940) {
        savedata->getPlayer().getPlayerInfo().setTotalTime(play_time);
    }

    savedata->getPlayer().getPlayerStatusB().setDateIpl(OSGetTime());

    memcpy(card_ptr, savedata, sizeof(dSv_save_c));
    card_ptr += 0x958;
    OS_REPORT("(dSv_player_c) size %d\n", sizeof(dSv_player_c));
    OS_REPORT("(dSv_memory_c) size %d(%d)\n", sizeof(dSv_memory_c));
    OS_REPORT("(dSv_memory2_c) size %d(%d)\n", sizeof(dSv_memory2_c));
    OS_REPORT("(dSv_event_c) size %d\n", sizeof(dSv_event_c));
    OS_REPORT("(dSv_reserve_c) size %d\n", sizeof(dSv_reserve_c));
    OS_REPORT("(dSv_MiniGame_c) size %d\n", sizeof(dSv_MiniGame_c));
    OS_REPORT("セーブ全体情報(dSv_save_c) size %d\n", sizeof(dSv_save_c));
    printf("Write size:%d\n", card_ptr - var_r29);

    // Now that we've saved, reset events if needed
    if (lantern_not_recovered == true) {
        if (lantern_dropped) {
            dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[224]);
        }
        if (lantern_stolen) {
            dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[225]);
        }
    }

    // Now that we've saved, reset slot if needed
    if (tmp_lantern_check == true) {
        dComIfGs_setItem(SLOT_1, fpcNm_ITEM_NONE);
        dComIfGs_setOil(current_lantern_oil);
    }
    
    if (card_ptr - var_r29 > (QUEST_LOG_SIZE - 8)) {
        printf("SAVE size over(%d/%d)\n", (QUEST_LOG_SIZE - 8), card_ptr - var_r29);
        return -1;
    }

    printf("SAVE size:%d\n", card_ptr - var_r29);
    return 0;
}

int dSv_info_c::card_to_memory(char* i_cardPtr, int i_dataNum) {
    i_cardPtr = i_cardPtr + i_dataNum * QUEST_LOG_SIZE;
    char* var_r30 = i_cardPtr;

    memcpy(dComIfGs_getSaveData(), i_cardPtr, sizeof(dSv_save_c));
    i_cardPtr += sizeof(dSv_save_c);

#if PLATFORM_GCN
    if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
        g_dComIfG_gameInfo.info.getPlayer().getConfig().setSound(OS_SOUND_MODE_MONO);
        Z2AudioMgr::mAudioMgrPtr->setOutputMode(OS_SOUND_MODE_MONO);
    } else if (g_dComIfG_gameInfo.info.getPlayer().getConfig().getSound() == 2) {
        Z2AudioMgr::mAudioMgrPtr->setOutputMode(2);
    } else {
        g_dComIfG_gameInfo.info.getPlayer().getConfig().setSound(OS_SOUND_MODE_STEREO);
        Z2AudioMgr::mAudioMgrPtr->setOutputMode(OS_SOUND_MODE_STEREO);
    }
#endif

    dSv_save_c* savedata = dComIfGs_getSaveData();
    if (savedata->getPlayer().getPlayerStatusA().getLife() < 12) {
        savedata->getPlayer().getPlayerStatusA().setLife(12);
    }

    dComIfGs_setKeyNum(6, 0);

    if (dComIfGs_getItem(SLOT_9, true) == fpcNm_ITEM_W_HOOKSHOT) {
        dComIfGs_setItem(SLOT_10, fpcNm_ITEM_W_HOOKSHOT);
        dComIfGs_setItem(SLOT_9, fpcNm_ITEM_NONE);
    }

    if (dComIfGs_getItem(SLOT_9, true) == fpcNm_ITEM_HOOKSHOT && dComIfGs_getItem(SLOT_10, true) == fpcNm_ITEM_W_HOOKSHOT) {
        dComIfGs_setItem(SLOT_9, fpcNm_ITEM_NONE);
    }

    dComIfGs_setLineUpItem();
    dComIfGp_setNowVibration(savedata->getPlayer().getConfig().getVibration());
    dMeter2Info_setSaveStageName(g_dComIfG_gameInfo.info.getPlayer().getPlayerReturnPlace().getName());

    OS_REPORT("########### save stage ====> %s\n", dMeter2Info_getSaveStageName());
    if (i_cardPtr - var_r30 > (QUEST_LOG_SIZE - 8)) {
        printf("LOAD size over(%d/%d)\n", (QUEST_LOG_SIZE - 8), i_cardPtr - var_r30);
        return -1;
    }

    printf("LOAD size:%d\n", i_cardPtr - var_r30);
    return 0;
}

int dSv_info_c::initdata_to_card(char* i_cardPtr, int i_dataNum) {
    i_cardPtr = i_cardPtr + (i_dataNum * QUEST_LOG_SIZE);
    char* var_r30 = i_cardPtr;

    dSv_save_c save;
    save.init();
    save.getPlayer().getPlayerInfo().setPlayerName("");
    save.getPlayer().getPlayerInfo().setHorseName("");
    memcpy(i_cardPtr, &save, sizeof(dSv_save_c));
    i_cardPtr += sizeof(dSv_save_c);

    if (i_cardPtr - var_r30 > (QUEST_LOG_SIZE - 8)) {
        printf("INIT size over %d/%d\n", (QUEST_LOG_SIZE - 8), i_cardPtr - var_r30);
        return -1;
    }

    printf("INIT size:%d\n", i_cardPtr - var_r30);
    return 0;
}

#if DEBUG
flagFile_c::flagFile_c() {
    m_no = mDoHIO_CREATE_CHILD("フラグファイル", this);
    m_flags = FLAG_ALL_e;
}

flagFile_c::~flagFile_c() {
    mDoHIO_DELETE_CHILD(m_no);
}

void flagFile_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genLabel("- フラグデータのデバッグ用一時セーブ", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genButton(" 読込 ", 101, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);

    if (fopScnM_SearchByID(dStage_roomControl_c::getProcID()) != NULL) {
        mctx->genLabel("- セーブ要素", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genCheckBox(" ステージ名", &m_flags, FLAG_SCENE_e, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genCheckBox(" セーブ領域", &m_flags, FLAG_SAVE_e, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genCheckBox(" セーブ作業領域", &m_flags, FLAG_MEM_e, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genCheckBox(" ダンジョンセーブ", &m_flags, FLAG_DAN_e, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genButton(" 書出 ", 102, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    }
}

void flagFile_c::listenPropertyEvent(const JORPropertyEvent* i_event) {
    JORReflexible::listenPropertyEvent(i_event);

    const char ext_mask[] = "フラグファイル(*.zff)\0*.zff\0その他のファイル(*.*)\0*.*\0";
    char buffer[sizeof(dSv_save_c)];
    u32 head[2];

    switch ((u32)i_event->id) {
    case 101: {
        JORFile file;
        if (file.open(1, ext_mask, NULL, NULL, NULL)) {
            OS_REPORT("read from %s\n", file.getFilename());
            char* scene_name = (char*)dComIfGs_getSaveInfo() + 1;
            *scene_name = 0;

            BOOL read_end = FALSE;
            while (!read_end) {
                file.readData(head, sizeof(head));
                switch (head[0]) {
                case '_SCN': {
                    file.readData(scene_name, head[1]);
                    OS_REPORT("scene name [%s]\n", scene_name);

                    const char* start_stage_name = dComIfGp_getStartStageName();
                    if (fopScnM_SearchByID(dStage_roomControl_c::getProcID()) != NULL &&
                        strcmp(start_stage_name, scene_name) != 0)
                    {
                        OS_REPORT("error: bad scene: %s now!!\n", start_stage_name);
                        read_end = TRUE;
                    }
                    break;
                }
                case '+SAV': {
                    OS_REPORT("save data...\n");
                    file.readData(&buffer, head[1]);
                    dSv_save_c* save = (dSv_save_c*)buffer;
                    dComIfGs_getSaveInfo()->setSavedata(*save);
                    break;
                }
                case '+MEM': {
                    OS_REPORT("temporary data...\n");
                    file.readData(&buffer, head[1]);
                    dSv_memory_c* mem = (dSv_memory_c*)buffer;
                    dComIfGs_getSaveInfo()->setMemory(*mem);
                    break;
                }
                case '+DAN': {
                    OS_REPORT("dungeon save data...\n");
                    file.readData(&buffer, head[1]);
                    dSv_danBit_c* dan = (dSv_danBit_c*)buffer;
                    dComIfGs_getSaveInfo()->setDan(*dan);
                    break;
                }
                default:
                    OS_REPORT("error: illegal flag data\n");
                case 'END_':
                    read_end = TRUE;
                }
            }

            file.close();
            dComIfGs_getSaveInfo()->unk_0x0 = 1;
        }
        break;
    }
    case 102: {
        OSCalendarTime time;
        OSTicksToCalendarTime(OSGetTime(), &time);

        const char* start_stage_name = dComIfGp_getStartStageName();
        char filename[64];
        sprintf(filename, "_%02d%02d%02d%02d%02d-%s.zff", time.mon + 1, time.mday, time.hour, time.min, time.sec, start_stage_name);
        OS_REPORT("write to %s\n", filename);

        JORFile file;
        if (file.open(filename, 2, "", NULL, NULL, NULL)) {
            if (check_flag(FLAG_SCENE_e)) {
                head[0] = '_SCN';
                head[1] = strlen(start_stage_name) + 1;

                file.writeData(head, sizeof(head));
                file.writeData(start_stage_name, head[1]);
            }

            if (check_flag(FLAG_SAVE_e)) {
                head[0] = '+SAV';
                head[1] = sizeof(dSv_save_c);

                file.writeData(head, sizeof(head));
                file.writeData(&dComIfGs_getSaveInfo()->getSavedata(), head[1]);
            }

            if (check_flag(FLAG_MEM_e)) {
                head[0] = '+MEM';
                head[1] = sizeof(dSv_memory_c);

                file.writeData(head, sizeof(head));
                file.writeData(&dComIfGs_getSaveInfo()->getMemory(), head[1]);
            }

            if (check_flag(FLAG_DAN_e)) {
                head[0] = '+DAN';
                head[1] = sizeof(dSv_danBit_c);

                file.writeData(head, sizeof(head));
                file.writeData(&dComIfGs_getSaveInfo()->getDan(), head[1]);
            }

            head[0] = 'END_';
            head[1] = 0;

            file.writeData(head, sizeof(head));
            file.close();
        }
        break;
    }
    }
}

BOOL flagFile_c::check_flag(u16 i_flag) {
    return (m_flags & i_flag) != 0;
}
#endif

#if VERSION > VERSION_GCN_JPN
const
#endif
u16 dSv_event_flag_c::saveBitLabels[822] = {
    UNUSED, TEST_001, TEST_002, TEST_003, TEST_004, F_0001, F_0002, F_0003, F_0004, F_0005, F_0006, 
    F_0007, F_0008, F_0009, F_0010, F_0011, F_0012, F_0013, F_0014, F_0015, F_0016, F_0017, F_0018, 
    F_0019, F_0020, D_0001, F_0021, F_0022, F_0023, F_0024, F_0025, F_0026, F_0027, F_0028, F_0029, 
    F_0030, F_0031, F_0032, F_0033, M_006,  M_007,  M_008,  M_009,  M_010,  M_011,  M_012,  M_013,  
    M_014,  M_015,  M_016,  M_017,  M_018,  M_019,  M_020,  M_021,  M_022,  M_023,  M_024,  M_025,  
    M_026,  M_027,  M_028,  M_029,  M_030,  M_031,  M_032,  M_033,  M_034,  M_035,  M_036,  M_037,  
    M_038,  M_039,  M_040,  M_041,  M_042,  M_043,  M_044,  M_045,  M_046,  M_047,  M_048,  M_049,  
    M_050,  M_051,  M_052,  M_053,  M_054,  M_055,  M_056,  M_057,  M_058,  M_059,  M_060,  M_061,  
    M_062,  M_063,  M_064,  M_065,  M_066,  M_067,  M_068,  M_069,  M_070,  M_071,  M_072,  M_073,  
    M_074,  M_075,  M_076,  M_077,  M_078,  M_079,  M_080,  M_081,  M_082,  M_083,  M_084,  M_085,  
    M_086,  M_087,  M_088,  M_089,  M_090,  M_091,  M_092,  M_093,  M_094,  M_095,  M_096,  F_0034, 
    F_0035, M_001,  M_003,  F_0036, F_0037, F_0038, F_0039, F_0040, F_0041, F_0042, F_0043, F_0044, 
    F_0045, F_0046, F_0047, F_0048, F_0049, F_0050, F_0051, F_0052, F_0053, F_0054, F_0055, F_0056, 
    F_0057, F_0058, F_0059, F_0060, F_0061, F_0062, F_0063, F_0064, F_0065, F_0066, F_0067, F_0068, 
    F_0069, F_0070, M_002,  F_0071, F_0072, F_0073, F_0074, F_0075, F_0076, F_0077, F_0078, F_0079, 
    F_0080, F_0081, F_0082, F_0083, F_0084, F_0085, EREG_000,       F_0086, F_0087, F_0088, F_0089, 
    F_0090, F_0091, F_0092, F_0093, F_0094, F_0095, F_0096, F_0097, TOTAL_PRICE_HIGH,       TOTAL_PRICE_LOW, 
    TOTAL_PAYMENT_HIGH,     TOTAL_PAYMENT_LOW,      F_0202, F_0203, F_0204, F_0205, F_0206, F_0207, 
    F_0208, F_0209, F_0210, F_0211, F_0212, F_0213, F_0214, F_0215, F_0216, F_0217, F_0218, F_0219, 
    F_0220, F_0221, F_0222, F_0223, F_0224, F_0225, F_0226, F_0227, F_0228, F_0229, F_0230, F_0231,
    F_0232, F_0233, F_234,  F_0235, F_0236, F_0237, F_0238, T_0239, F_0240, F_0241, F_0242, F_0243, 
    F_0244, MAP_VISIBLE,    F_0246, F_0247, F_0248, F_0249, F_0250, F_0251, F_0252, F_0253, F_0254, 
    F_0255, F_0256, F_0257, F_0258, F_0259, F_0260, F_0261, F_0262, F_0263, F_0264, F_0265, F_0266, 
    F_0267, F_0268, F_0269, F_0270, F_0271, F_0272, F_0273, F_0274, F_0275, F_0276, F_0277, F_0278,
    F_0279, F_0280, F_0281, F_282,  F_283,  F_284,  F_285,  F_286,  F_287,  F_288,  F_289,  F_290, 
    M_097,  F_0292, F_0293, F_0294, F_0295, F_0296, F_0297, F_0298, F_0299, F_0300, F_0301, F_0302, 
    F_0303, F_0304, F_0305, F_0306, F_0307, F_0308, F_0309, F_0310, F_0311, F_0312, F_0313, F_0314, 
    F_0315, F_0316, F_0317, F_0318, F_0319, F_0320, F_0321, F_0322, F_0323, F_0324, F_0325, F_0326, 
    F_0327, F_0328, F_0329, F_0330, F_0331, F_0332, F_0333, F_0334, F_0335, F_0336, F_0337, F_0338, 
    F_0339, F_0340, F_0341, F_0342, F_0343, F_0344, F_0345, F_0346, F_0347, F_0348, F_0349, F_0350, 
    F_0351, F_0352, F_0353, F_0354, F_0355, F_0356, F_0357, F_0358, F_0359, F_0360, F_0361, F_0362, 
    F_0363, F_0364, F_0365, F_0366, F_0367, F_0368, F_0369, F_0370, F_0371, F_0372, F_0373, F_0374, 
    F_0375, F_0376, F_0377, F_0378, F_0379, F_0380, F_0381, F_0382, F_0383, F_0384, F_0385, F_0386, 
    F_0387, F_0388, F_0389, F_0390, F_0391, F_0392, F_0393, F_0394, F_0395, F_0396, F_0397, F_0398, 
    F_0399, F_0400, F_0401, F_0402, F_0403, F_0404, F_0405, F_0406, F_0407, F_0408, F_0409, F_0410, 
    F_0411, F_0412, F_0413, F_0414, F_0415, F_0416, F_0417, F_0418, F_0419, F_0420, F_0421, F_0422, 
    F_0423, F_0424, F_0425, F_0426, F_0427, F_0428, F_0429, F_0430, F_0431, F_0432, F_0433, F_0434, 
    F_0435, F_0436, F_0437, F_0438, F_0439, F_0440, F_0441, F_0442, F_0443, F_0444, F_0445, F_0446, 
    F_0447, F_0448, F_0449, F_0450, FUND_RAISING_LOW,       FUND_RAISING_HIGH,      F_0453, F_0454, 
    F_0455, F_0456, F_0457, F_0458, F_0459, F_0460, F_0461, F_0462, F_0463, F_0464, F_0465, F_0466, 
    F_0467, F_0468, F_0469, F_0470, F_0471, F_0472, F_0473, F_0474, F_0475, F_0476, F_0477, F_0478, 
    F_0479, F_0480, F_0481, F_0482, F_0483, F_0484, F_0485, F_0486, F_0487, F_0488, F_0489, F_0490, 
    F_0491, F_0492, F_0493, F_0494, F_0495, F_0496, F_0497, F_0498, F_0499, F_0500, F_0501, F_0502, 
    F_0503, F_0504, F_0505, F_0506, F_0507, F_0508, F_0509, F_0510, FUND_RAISING_LOW2,      FUND_RAISING_HIGH2, 
    F_0513, F_0514, F_0515, F_0516, F_0517, F_0518, F_0519, F_0520, F_0521, F_0522, F_0523, F_0524, 
    F_0525, F_0526, F_0527, F_0528, F_0529, F_0530, F_0531, F_0532, F_0533, F_0534, F_0535, F_0536, 
    F_0537, F_0538, F_0539, F_0540, F_0541, F_0542, F_0543, F_0544, F_0545, F_0546, F_0547, F_0548, 
    F_0549, F_0550, F_0551, F_0552, F_0553, F_0554, F_0555, F_0556, F_0557, F_0558, F_0559, F_0560, 
    F_0561, F_0562, F_0563, F_0564, F_0565, F_0566, F_0567, F_0568, F_0569, F_0570, F_0571, F_0572,
    F_0573, F_0574, F_0575, F_0576, F_0577, F_0578, F_0579, F_0580, F_0581, F_0582, F_0583, F_0584, 
    F_0585, F_0586, F_0587, F_0588, F_0589, F_0590, F_0591, F_0592, F_0593, F_0594, F_0595, F_0596, 
    F_0597, F_0598, F_0599, F_0600, F_0601, UNUSED, F_0700, F_0701, F_0702, F_0606, F_0607, F_0608, 
    F_0609, F_0610, F_0611, F_0612, F_0613, F_0614, F_0615, F_0616, F_0617, F_0618, F_0619, F_0620, 
    F_0621, F_0622, F_0623, F_0624, F_0625, F_0626, F_0627, F_0628, F_0629, F_0630, F_0631, F_0632, 
    F_0633, F_0634, F_0635, F_0636, F_0637, F_0638, F_0639, F_0640, F_0641, F_0642, F_0643, F_0644, 
    F_0645, F_0646, F_0647, F_0648, F_0649, F_0650, F_0651, F_0652, F_0653, F_0654, F_0655, F_0656, 
    F_0657, F_0658, F_0659, F_0660, F_0661, F_0662, F_0663, F_0664, F_0665, F_0666, F_0667, F_0668, 
    F_0669, F_0670, F_0671, F_0672, F_0673, F_0674, F_0675, F_0676, F_0677, F_0678, F_0679, F_0680, 
    F_0681, F_0682, F_0683, F_0684, F_0685, F_0686, F_0687, F_0688, F_0689, F_0690, F_0691, F_0692, 
    F_0693, F_0694, F_0695, F_0696, F_0697, F_0698, F_0699, F_0700B,        F_0701B,        F_0702B, 
    F_0703, F_0704, F_0705, F_0706, F_0707, F_0708, F_0709, KORO2_LEVEL,    LM_RECORD_0,    RI_RECORD_0,
    NP_RECORD_0,    CF_RECORD_0,    F_0715, F_0716, F_0717, F_0718, F_0719, F_0720, F_0721, F_0722,
    F_0723, F_0724, F_0725, F_0726, F_0727, F_0728, F_0729, F_0730, SP_LURE__USE_COUNT,     F_0732, 
    F_0733, F_0734, F_0735, F_0736, F_0737, F_0738, F_0739, F_0740, F_0741, F_0742, F_0743, F_0744, 
    F_0745, F_0746, F_0747, F_0748, F_0749, F_0750, F_0751, F_0752, F_0753, F_0754, F_0755, F_0756, 
    F_0757, F_0758, F_0759, KAKERA_GET_INFO_00,     KAKERA_GET_INFO_01,     KAKERA_GET_INFO_02,
    KAKERA_GET_INFO_03,     KAKERA_GET_INFO_04,     KAKERA_GET_INFO_05,     B_BTN_GUIDE,    J_BTN_GUIDE,
    F_0768, F_0769, F_0770, F_0771, F_0772, F_0773, F_0774, F_0775, F_0776, F_0777, F_0778, F_0779,
    F_0780, F_0781, F_0782, F_0783, F_0784, F_0785, F_0786, F_0787, F_0788, F_0789, F_0790, F_0791,
    F_0792, F_0793, F_0794, F_0795, F_0796, F_0797, F_0798, F_0799, F_0800, F_0801, F_0802, F_0803,
    F_0804, F_0805, F_0806, F_0807, F_0808, F_0809, F_0810, F_0811, F_0812, F_0813, F_0814, F_0815,
    F_0816, F_0817, F_0818, F_0819, F_0820, KORO2_ALLCLEAR,
};

u16 const dSv_event_tmp_flag_c::tempBitLabels[185] = {
    UNUSED, UNUSED, T_0002, T_0003, T_0004, T_0005, T_0006, T_0007, T_0001, T_0008, T_0009, T_0010,
    T_0011, T_0012, T_0013, T_0014, T_0015, T_0016, T_0017, T_0018, T_0019, T_0020, T_0021, T_0022,
    T_0023, T_0024, T_0025, T_0026, T_0027, T_0028, T_0029, T_0030, T_0031, T_0032, TREG_000, 
    T_0033, T_0034, T_0035, T_0036, TEMP_0038,      TEMP_0040,      T_0041, T_0042, T_0043, T_0044,
    T_0045, T_0046, T_0047, T_0048, T_0049, T_0050, T_0051, T_0052, T_0053, T_0054, T_0055, T_0056, 
    T_0057, T_058, T_0059,  T_0060, T_0061, T_0062, T_0063, T_0064, T_0065, T_0066, T_0067, T_0068,
    T_0069, T_0070, T_0071, T_0072, T_0073, T_0074, T_0075, T_0076, T_0077, T_0078, T_0079, T_0080,
    T_0081, T_0082, T_0083, T_0084, T_0085, T_0086, T_0087, T_0088, T_0089, T_0090, T_0091, T_0092, 
    T_0093, T_0094, T_0095, T_0096, T_0097, T_0098, T_0099, T_0100, SHA_LOVEFORTUNE_00,     SHA_LOVEFORTUNE_01, 
    SHA_LOVEFORTUNE_02,     SHA_LOVEFORTUNE_03,     SHA_LOVEFORTUNE_04,     SHA_LOVEFORTUNE_05, 
    T_0107, T_0108, T_0109, T_0110, T_0111, T_0112, T_0113, PACHITUTORIAL_CNT,      PACHITUTORIAL_INF, 
    WXXTO,  MN09_02,        MN09_03,        T_0119, T_0120, T_0121, T_0122, T_0123, T_0124, YKM_ATTN, 
    YKM_TALK,       YKW_TALK,       YKW_ATTN,       T_0125,         DUNGEON_130,    DUNGEON_131,    
    DUNGEON_132,    DUNGEON_133,    DUNGEON_134,    DUNGEON_135,    DUNGEON_136,    DUNGEON_137,    
    DUNGEON_138,    DUNGEON_139,    DUNGEON_140,    DUNGEON_141,    DUNGEON_142,    DUNGEON_143,
    DUNGEON_144,    DUNGEON_145,    DUNGEON_146,    DUNGEON_147,    DUNGEON_148,    DUNGEON_149,
    DUNGEON_150,    CAT_1,   CAT_2, CAT_3,  CAT_4,  CAT_5,  CAT_6,  CAT_7,  CAT_8,  CAT_9,  CAT_10,
    CAT_11, CAT_12, CAT_13, CAT_14, CAT_15, CAT_16, CAT_17, CAT_18, CAT_19, CAT_20, GUARD_URI, 
    Hanch_CLIMBOUT_POND,    NODRAW_STAGE_NAME,      NO_TELOP,       T_0175, T_0176, T_0177, T_0178, 
    T_0179, T_0180, T_0181, T_0182, T_0183, SHA_DBG
};
