/**
 * d_com_inf_game.cpp
 * Game Information
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_map_path_dmap.h"
#include "d/d_menu_fmap.h"
#include "d/d_menu_window_HIO.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_simple_model.h"
#include "d/d_timer.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_scene_mng.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include <cstdio>
#include <cstring>

void dComIfG_play_c::ct() {
    mWindowNum = 0;
    mParticle = NULL;
    mLayerOld = 0;

    memset(mLastPlayStageName, 0, 8);
    init();
}

static __d_timer_info_c dComIfG_mTimerInfo;

void dComIfG_play_c::init() {
    for (int i = 0; i < ARRAY_SIZE(mPlayerInfo); i++) {
        mPlayerInfo[i].mpPlayer = NULL;
        mPlayerInfo[i].mCameraID = -1;
    }
    for (int i = 0; i < ARRAY_SIZE(mCameraInfo); i++) {
        mCameraInfo[i].mCamera = NULL;
    }

    for (int i = 0; i < ARRAY_SIZE(mPlayerPtr); i++) {
        mPlayerPtr[i] = NULL;
    }

    if (mItemInfo.mGameoverStatus == 2) {
        dComIfGp_roomControl_initZone();
    }
    mItemInfo.mGameoverStatus = 0;
}

BOOL dComIfGp_checkItemGet(u8 i_itemNo, int param_1) {
    return checkItemGet(i_itemNo, param_1);
}

void dComIfG_play_c::itemInit() {
    dMeter2Info_Initialize();

    JKRExpHeap* heap = mItemInfo.mExpHeap2D;
    memset(&mItemInfo.mMsgObjectClass, 0, 300);
    mItemInfo.mExpHeap2D = heap;
    mItemInfo.mOxygen = 600;
    mItemInfo.mNowOxygen = 600;
    mItemInfo.mMaxOxygen = 600;

    if (dComIfGs_checkGetItem(fpcNm_ITEM_HAWK_EYE)) {
        mItemInfo.field_0x4f4b = 0;
    } else {
        mItemInfo.field_0x4f4b = 21;
    }
    mItemInfo.field_0x4f4c = 7;

    mItemInfo.mNowVibration = dComIfGs_getOptVibration();
#if DEBUG
    g_mwHIO.init();
    g_mwHIO.setArrowFlag(true);
    g_mwHIO.setPachinkoFlag(true);
    g_mwHIO.setBombFlag(true);
    g_mwHIO.update();
#endif
}

void dComIfG_play_c::setItemBombNumCount(u8 i_item, s16 count) {
#if DEBUG
    if (i_item == 8) {
        mItemInfo.field_0x4ec8 += count;
        return;
    }
#endif
    JUT_ASSERT(176, 0 <= i_item && i_item < dSv_player_item_c::BOMB_BAG_MAX);
    mItemInfo.mItemBombNumCount[i_item] += count;
}

s16 dComIfG_play_c::getItemBombNumCount(u8 i_item) {
#if DEBUG
    if (i_item == 8) {
        return mItemInfo.field_0x4ec8;
    }
#endif
    JUT_ASSERT(197, 0 <= i_item && i_item < dSv_player_item_c::BOMB_BAG_MAX);
    return mItemInfo.mItemBombNumCount[i_item];
}

void dComIfG_play_c::clearItemBombNumCount(u8 i_item) {
#if DEBUG
    if (i_item == 8) {
        mItemInfo.field_0x4ec8 = 0;
        return;
    }
#endif
    JUT_ASSERT(220, 0 <= i_item && i_item < dSv_player_item_c::BOMB_BAG_MAX);
    mItemInfo.mItemBombNumCount[i_item] = 0;
}

s16 dComIfG_play_c::getItemMaxBombNumCount(u8 i_bombType) {
    switch (i_bombType) {
    case fpcNm_ITEM_NORMAL_BOMB:
        return mItemInfo.mItemMaxBombNumCount1;
    case fpcNm_ITEM_WATER_BOMB:
        return mItemInfo.mItemMaxBombNumCount2;
    case fpcNm_ITEM_POKE_BOMB:
        return mItemInfo.field_0x4ed8;
    }
    return 0;
}

void dComIfG_play_c::setNowVibration(u8 i_vibration) {
    mItemInfo.mNowVibration = i_vibration;
}

u32 dComIfG_play_c::getNowVibration() {
    return mItemInfo.mNowVibration;
}

void dComIfG_play_c::setStartStage(dStage_startStage_c* i_startStage) {
    mLayerOld = mStartStage.getLayer();
    mStartStage = *i_startStage;
}

void dComIfG_get_timelayer(int* o_layer) {
    if (dKy_daynight_check()) {
        *o_layer += 1;
    }
}

int dComIfG_play_c::getLayerNo_common_common(const char* i_stageName, int i_roomNo, int o_layer) {
    int layer = o_layer;
    if (layer < 0) {
        layer = -1;

        // Stage is in a Twilight state
        if (dKy_darkworld_stage_check(i_stageName, i_roomNo) == TRUE) {
            layer = 14;
        }

        if (layer < 13) {
            // Stage is Snowpeak Ruins or Snowpeak
            if (!strcmp(i_stageName, "D_MN11") || !strcmp(i_stageName, "F_SP114")) {
                // Cleared Snowpeak Ruins
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[266])) {
                    layer = 3;
                }

                // Talked with Yeta after giving Cheese
                else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[163]))
                {
                    layer = 2;
                }

                // Talked with Yeta after giving Pumpkin
                else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[162]))
                {
                    layer = 1;
                }
            }

            // Stage is Faron Woods
            else if (!strcmp(i_stageName, "F_SP108"))
            {
                // Cleared Snowpeak Ruins
                if (dComIfGs_isEventBit(0x2008)) {
                    layer = 5;
                }

                // Completed Midna's Desperate Hour
                else if (dComIfGs_isEventBit(0x1E08))
                {
                    layer = 3;
                }

                // Cleared Forest Temple
                else if (dComIfGs_isEventBit(0x0602))
                {
                    layer = 2;
                }

                // Haven't finished Ordon Day 2
                else if (!dComIfGs_isEventBit(0x4510))
                {
                    layer = 1;
                }
            }

            // Stage is Faron Woods Interiors
            else if (!strcmp(i_stageName, "R_SP108"))
            {
                // Cleared Forest Temple
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[55])) {
                    layer = 2;
                }

                // Haven't finished Ordon Day 2
                else if (!dComIfGs_isEventBit(0x4510))
                {
                    layer = 1;
                }
            }

            // Stage is Kakariko Village or Kakariko Graveyard
            else if (!strcmp(i_stageName, "F_SP109") || !strcmp(i_stageName, "F_SP111"))
            {
                // Obtained Zora's Armor
                if (dComIfGs_isEventBit(0x0804)) {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }

                // Finished Telma Wagon Escort
                else if (dComIfGs_isEventBit(0x0810))
                {
                    layer = 4;
                }

                // Watched cutscene after leaving Goron Mines
                else if (dComIfGs_isEventBit(0x1320))
                {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }

                // Cleared Goron Mines
                else if (dComIfGs_isEventBit(0x0701))
                {
                    layer = 12;
                }

                // Defeated King Bulblin 1
                else if (dComIfGs_isEventBit(0x0A08))
                {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }

                // King Bulblin 1 trigger activated
                else if (dComIfGs_isEventBit(0x0608))
                {
                    layer = 1;
                }
            }

            // Stage is Kakariko Village Interiors or Graveyard Interiors
            else if (!strcmp(i_stageName, "R_SP109") || !strcmp(i_stageName, "R_SP209"))
            {
                // Stage is Kakariko Interiors and room is Barnes shop and Cleared Lakebed Temple
                if (!strcmp(i_stageName, "R_SP109") && i_roomNo == 1 &&
                    dComIfGs_isEventBit(0x0904))
                {
                    layer = 4;
                    dComIfG_get_timelayer(&layer);
                } else {
                    // Defeated King Bulblin 1
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[85])) {
                        layer = 2;
                        dComIfG_get_timelayer(&layer);
                    }

                    // King Bulblin 1 trigger activated
                    else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[53]))
                    {
                        layer = 1;
                    }
                }
            }

            // Stage is Death Mountain
            else if (!strcmp(i_stageName, "F_SP110"))
            {
                // Cleared Goron Mines
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[64])) {
                    layer = 2;
                }
            }

            // Stage is Death Mountain Interiors
            else if (!strcmp(i_stageName, "R_SP110"))
            {
                // Returned Wood Statue to Ilia
                if (dComIfGs_isEventBit(0x2320)) {
                    layer = 3;
                }

                // Cleared Temple of Time
                else if (dComIfGs_isEventBit(0x2004))
                {
                    layer = 4;
                }

                // Obtained Master Sword
                else if (dComIfGs_isEventBit(0x2020))
                {
                    layer = 2;
                }

                // Cleared Goron Mines
                else if (dComIfGs_isEventBit(0x0701))
                {
                    layer = 1;
                }
            }

            // Stage is Lake Hylia, Castle Town, Telma's Bar, or R_SP115 (removed)
            else if (!strcmp(i_stageName, "F_SP115") || !strcmp(i_stageName, "F_SP116") ||
                     (!strcmp(i_stageName, "R_SP116") && i_roomNo == 5) ||
                     !strcmp(i_stageName, "R_SP115"))
            {
                // Stage is Lake Hylia and room is Lake
                if (!strcmp(i_stageName, "F_SP115") && i_roomNo == 0) {
                    // Repaired Sky Cannon
                    if (dComIfGs_isEventBit(0x3B08)) {
                        layer = 3;
                    }

                    // Warped Sky Cannon to Lake Hylia
                    else if (dComIfGs_isEventBit(0x3120))
                    {
                        layer = 1;
                    }

                    // Cleared Lakebed Temple
                    else if (dComIfGs_isEventBit(0x0904))
                    {
                        layer = 2;
                    }
                }

                // Stage is Telma's Bar and room is Bar and Obtained Master Sword
                else if (!strcmp(i_stageName, "R_SP116") && i_roomNo == 5 &&
                         dComIfGs_isEventBit(0x2020))
                {
                    layer = 4;
                }

                // Completed Midna's Desperate Hour and Stage is Castle Town
                else if (dComIfGs_isEventBit(0x1E08) && !strcmp(i_stageName, "F_SP116"))
                {
                    // Room is not East, South, or North Castle Town
                    if (i_roomNo != 4 && i_roomNo != 3 && i_roomNo != 1) {
                        layer = 0;
                    } else {
                        layer = 1;
                    }
                } else {
                    // Cleared Lakebed Temple
                    if (dComIfGs_isEventBit(0x0904)) {
                        // Stage is Lake Hylia and room is Fountain and haven't started Midna's
                        // Desperate Hour
                        if ((!strcmp(i_stageName, "F_SP115") && i_roomNo == 1) &&
                            !dComIfGs_isEventBit(0x0C01))
                        {
                            layer = 9;
                        } else {
                            layer = 2;
                        }
                    } else {
                        // Stage is Castle Town and room is South Castle Town and Finished Telma
                        // Wagon Escort
                        if ((!strcmp(i_stageName, "F_SP116") && i_roomNo == 3) &&
                            dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[68]))
                        {
                            layer = 1;
                        }
                    }
                }
            }

            // Stage is Zora's Domain
            else if (!strcmp(i_stageName, "F_SP113"))
            {
                // Cleared Snowpeak Ruins
                if (dComIfGs_isEventBit(0x2008)) {
                    layer = 2;
                }
            }

            // Stage is Upper Zora's River
            else if (!strcmp(i_stageName, "F_SP126"))
            {
                // Unlocked Iza's River Ride (1)
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[95])) {
                    layer = 1;
                }
            }

            // Stage is Gerudo Desert and room is Desert
            else if (!strcmp(i_stageName, "F_SP124") && i_roomNo == 0)
            {
                layer = 8;

                // Used Sky Cannon to go to Desert
                if (dComIfGs_isEventBit(0x4008)) {
                    layer = 0;
                }
            }

            // Stage is Zora's River
            else if (!strcmp(i_stageName, "F_SP112"))
            {
                // Unlocked Iza's River Ride (1)
                if (dComIfGs_isEventBit(0x0B01)) {
                    layer = 1;
                }

                // Started Iza's River Ride (1)
                else if (dComIfGs_isEventBit(0x0902))
                {
                    layer = 2;
                }
            }

            // Stage is Ordon Village
            else if (!strcmp(i_stageName, "F_SP103"))
            {
                // Room is Main Village
                if (i_roomNo == 0) {
                    // Tamed Epona
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[56])) {
                        layer = 4;
                        dComIfG_get_timelayer(&layer);
                    }

                    // Cleared Faron Twilight
                    else if (dComIfGs_isDarkClearLV(0))
                    {
                        layer = 2;
                        dComIfG_get_timelayer(&layer);
                    }

                    // Escaped Hyrule Castle Sewers (1st Time)
                    else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[47]))
                    {
                        layer = 1;
                    }

                    // Finished Ordon Day 2
                    else if (dComIfGs_isEventBit(0x4510))
                    {
                        layer = 7;
                    }

                    // Finished Ordon Day 1
                    else if (dComIfGs_isEventBit(0x4A40))
                    {
                        layer = 0;
                    } else {
                        layer = 6;
                    }
                }

                // Room is Outside Link's House
                else if (i_roomNo == 1)
                {
                    // Cleared Faron Twilight
                    if (dComIfGs_isDarkClearLV(0)) {
                        layer = 2;
                    }

                    // Escaped Hyrule Castle Sewers (1st Time)
                    else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[47]))
                    {
                        layer = 1;
                    }

                    // Finished Ordon Day 2
                    else if (dComIfGs_isEventBit(0x4510))
                    {
                        layer = 0;
                    }

                    // Finished Ordon Day 1
                    else if (dComIfGs_isEventBit(0x4A40))
                    {
                        layer = 4;
                    } else {
                        layer = 3;
                    }
                }
            }

            // Stage is Ordon Village Interiors
            else if (!strcmp(i_stageName, "R_SP01"))
            {
                // Room is Sera's Shop
                if (i_roomNo == 1) {
                    // Cleared Faron Twilight
                    if (dComIfGs_isDarkClearLV(0)) {
                        layer = 2;
                    }
                }

                // Room is Shield house
                else if (i_roomNo == 2)
                {
                    // Watched cutscene after defeating King Bulblin 1
                    if (dComIfGs_isEventBit(0x0780)) {
                        layer = 3;
                    }

                    // Cleared Faron Twilight
                    else if (dComIfGs_isDarkClearLV(0))
                    {
                        layer = 2;
                    }

                    // Escaped Hyrule Castle Sewers (1st Time)
                    else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[47]))
                    {
                        layer = 1;
                    }
                }

                // Room is Rusl and Uli's house
                else if (i_roomNo == 5)
                {
                    // Tamed Epona
                    if (dComIfGs_isEventBit(0x0601)) {
                        layer = 4;
                    }

                    // Cleared Faron Twilight
                    else if (dComIfGs_isDarkClearLV(0))
                    {
                        layer = 2;
                    }
                }
            }

            // Stage is Ordon Spring
            else if (!strcmp(i_stageName, "F_SP104"))
            {
                // Room is Ordon Spring
                if (i_roomNo == 1) {
                    // Cleared Faron Twilight
                    if (dComIfGs_isDarkClearLV(0)) {
                        layer = 2;
                    }

                    // Escaped Hyrule Castle Sewers (1st Time)
                    else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[47]))
                    {
                        layer = 4;
                    }

                    // Finished Ordon Day 2
                    else if (dComIfGs_isEventBit(0x4510))
                    {
                        layer = 0;
                    }

                    // Finished Ordon Day 1
                    else if (dComIfGs_isEventBit(0x4A20))
                    {
                        layer = 3;
                    } else {
                        layer = 1;
                    }
                }

                // Cleared Faron Twilight
                else if (dComIfGs_isDarkClearLV(0))
                {
                    layer = 2;
                }

                // Escaped Hyrule Castle Sewers (1st Time)
                else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[47]))
                {
                    layer = 4;
                }
            }

            // Stage is Ordon Ranch
            else if (!strcmp(i_stageName, "F_SP00"))
            {
                // Cleared Faron Twilight
                if (dComIfGs_isDarkClearLV(0)) {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }

                // Escaped Hyrule Castle Sewers (1st Time)
                else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[47]))
                {
                    layer = 1;
                }

                // Watched cutscene after herding goats on Ordon Day 3
                else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[169]))
                {
                    layer = 10;
                }

                // Finished Ordon Day 2
                else if (dComIfGs_isEventBit(0x4510))
                {
                    layer = 9;
                }

                // Finished Ordon Day 1
                else if (dComIfGs_isEventBit(0x4A40))
                {
                    layer = 11;
                } else {
                    layer = 12;
                }
            }

            // Stage is Hyrule Field
            else if (!strcmp(i_stageName, "F_SP121"))
            {
                // Completed Midna's Desperate Hour
                if (dComIfGs_isEventBit(0x1E08)) {
                    layer = 6;
                }

                // Started Midna's Desperate Hour
                else if (dComIfGs_isEventBit(0x0C01))
                {
                    layer = 4;
                }

                // Finished Telma Wagon Escort
                else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[68]))
                {
                    layer = 0;
                }

                else if (dComIfGs_isTmpBit(0x0601))
                {
                    if (dComIfGs_isTmpBit(0x0602)) {
                        layer = 2;
                    } else {
                        layer = 3;
                    }
                }
            }

            // Stage is Outside Castle Town
            else if (!strcmp(i_stageName, "F_SP122"))
            {
                // Room is Outside Castle Town - West
                if (i_roomNo == 8) {
                    // Completed Midna's Desperate Hour
                    if (dComIfGs_isEventBit(0x1E08)) {
                        layer = 6;
                    }

                    // Started Midna's Desperate Hour
                    else if (dComIfGs_isEventBit(0x0C01))
                    {
                        layer = 4;
                    }
                }

                // Room is Outside Castle Town - South
                else if (i_roomNo == 16)
                {
                    // Obtained Wood Statue
                    if (dComIfGs_isEventBit(0x2204)) {
                        layer = 6;
                    }

                    // Talked to Louise after getting Medicine Scent
                    else if (dComIfGs_isEventBit(0x2102))
                    {
                        layer = 1;
                    }

                    // Completed Midna's Desperate Hour
                    else if (dComIfGs_isEventBit(0x1E08))
                    {
                        layer = 6;
                    }

                    // Started Midna's Desperate Hour
                    else if (dComIfGs_isEventBit(0x0C01))
                    {
                        layer = 4;
                    }
                }

                // Room is Outside Castle Town - East
                else if (i_roomNo == 17)
                {
                    // Completed Midna's Desperate Hour
                    if (dComIfGs_isEventBit(0x1E08)) {
                        layer = 0;
                    }

                    // Started Midna's Desperate Hour
                    else if (dComIfGs_isEventBit(0x0C01))
                    {
                        layer = 4;
                    }
                }
            }

            // Stage is Hidden Village
            else if (!strcmp(i_stageName, "F_SP128"))
            {
                if (dComIfGs_isEventBit(0x2320)) {
                    layer = 1;
                }
            }

            // Stage is Castle Town Interiors
            else if (!strcmp(i_stageName, "R_SP160"))
            {
                // Room is Jovani's house
                if (i_roomNo == 5) {
                    // Completed Midna's Desperate Hour
                    if (dComIfGs_isEventBit(0x1E08)) {
                        layer = 0;
                    } else {
                        layer = 1;
                    }
                }

                // Fundraised Malo Mart Castle Town branch
                else if (dComIfGs_isEventBit(0x2210))
                {
                    layer = 1;
                }
            }

            // Stage is Sacred Grove
            else if (!strcmp(i_stageName, "F_SP117"))
            {
                // Cleared Snowpeak Ruins
                if (dComIfGs_isEventBit(0x2008)) {
                    layer = 2;
                }
            }

            // Stage is Bulblin Camp
            else if (!strcmp(i_stageName, "F_SP118"))
            {
                // Fixed the Mirror of Twilight
                if (dComIfGs_isEventBit(0x2B08)) {
                    layer = 3;
                }

                // Cleared Arbiter's Grounds
                else if (dComIfGs_isEventBit(0x2010))
                {
                    layer = 2;
                }

                // Escaped the burning tent
                else if (dComIfGs_isEventBit(0x0B40))
                {
                    layer = 1;
                }
            }

            // Stage is Faron Woods Cave
            else if (!strcmp(i_stageName, "D_SB10"))
            {
                // Finished Ordon Day 2
                if (dComIfGs_isEventBit(0x4510)) {
                    layer = 1;
                }
            }

            // Stage is Hyrule Castle Sewers
            else if (!strcmp(i_stageName, "R_SP107"))
            {
                if (dComIfGs_isTransformLV(3)) {
                    layer = 13;
                }
            }

            // Stage is Hyrule Castle
            else if (!strcmp(i_stageName, "D_MN09"))
            {
                // Room is not Entrance, Outside Left Wing, or Outside Right Wing
                if (i_roomNo != 11 && i_roomNo != 13 && i_roomNo != 14) {
                    layer = 1;
                }
            }

            // Stage is Fishing Pond or Hena's Hut
            else if (!strcmp(i_stageName, "F_SP127") || !strcmp(i_stageName, "R_SP127"))
            {
                switch (g_env_light.fishing_hole_season) {
                case 1:
                    layer = 0;
                    break;
                case 2:
                    layer = 1;
                    break;
                case 3:
                    layer = 2;
                    break;
                case 4:
                    layer = 3;
                    break;
                }
            }
        }
    }

    if (layer == 14) {
        // Warped meteor to Zora's Domain
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[65])) {
            // Stage is Zora's River, Zora's Domain, Lake Hylia, Castle Town, Telma's Bar, R_SP115,
            // Hyrule Field, Upper Zora's River, or Outside Castle Town
            if (!strcmp(i_stageName, "F_SP112") || !strcmp(i_stageName, "F_SP113") ||
                !strcmp(i_stageName, "F_SP115") || !strcmp(i_stageName, "F_SP116") ||
                (!strcmp(i_stageName, "R_SP116") && i_roomNo == 5) ||
                !strcmp(i_stageName, "R_SP115") || !strcmp(i_stageName, "F_SP121") ||
                !strcmp(i_stageName, "F_SP126") || !strcmp(i_stageName, "F_SP122"))
            {
                // Stage is Hyrule Field
                if (!strcmp(i_stageName, "F_SP121")) {
                    if (i_roomNo >= 9 && i_roomNo <= 14) {
                        layer = 13;
                    }
                } else {
                    layer = 13;
                }
            }
        }

        // Stage is Hyrule Castle Sewers and room is Prison Cell
        if (!strcmp(i_stageName, "R_SP107") && i_roomNo == 0) {
            // Haven't been to Hyrule Castle Sewers
            if (!dComIfGs_isEventBit(0x4D08)) {
                layer = 11;
            }
        }
        // Stage and room is Zant Throne Room
        else if (!strcmp(i_stageName, "D_MN08A") && i_roomNo == 10)
        {
            // Defeated Zant
            if (dComIfGs_isEventBit(0x5410)) {
                layer = 1;
            } else {
                layer = 0;
            }
        }
    }

    return layer;
}

int dComIfG_play_c::getLayerNo_common(char const* i_stageName, int i_roomID, int i_layerOverride) {
    int layer = getLayerNo_common_common(i_stageName, i_roomID, i_layerOverride);

    if (layer < 0) {
        layer = 0;
    }

    return layer;
}

int dComIfG_play_c::getLayerNo(int param_1) {
    UNUSED(param_1);
    int layerNo = 0;
    int roomNo = dComIfGp_roomControl_getStayNo();

    if (roomNo <= -1) {
        roomNo = dComIfGp_getStartStageRoomNo();
    }

    layerNo = getLayerNo_common(dComIfGp_getStartStageName(), roomNo, dComIfGp_getStartStageLayer());
    return layerNo;
}

void dComIfG_play_c::createParticle() {
    mParticle = new dPa_control_c();
    JUT_ASSERT(1095, mParticle != NULL);
}

dSmplMdl_draw_c* dComIfG_play_c::createSimpleModel() {
    if (mSimpleModel == NULL) {
        mSimpleModel = new dSmplMdl_draw_c();
    }
    return mSimpleModel;
}

void dComIfG_play_c::deleteSimpleModel() {
    if (mSimpleModel != NULL) {
        delete mSimpleModel;
        mSimpleModel = NULL;
    }
}

void dComIfG_play_c::drawSimpleModel() {
    if (mSimpleModel != NULL) {
        mSimpleModel->draw();
    }
}

int dComIfG_play_c::addSimpleModel(J3DModelData* i_modelData, int i_roomNo, u8 i_drawBG) {
    if (mSimpleModel != NULL && mSimpleModel->addModel(i_modelData, i_roomNo, i_drawBG)) {
        return 1;
    }

    return -1;
}

int dComIfG_play_c::removeSimpleModel(J3DModelData* i_modelData, int i_roomNo) {
    if (mSimpleModel != NULL) {
        mSimpleModel->removeModel(i_modelData, i_roomNo);
        return 1;
    }

    return 0;
}

void dComIfG_play_c::entrySimpleModel(J3DModel* i_model, int i_roomNo) {
    if (mSimpleModel != NULL) {
        mSimpleModel->entry(i_model, i_roomNo);
    }
}

void dComIfG_play_c::setTimerNowTimeMs(int i_time) {
    mTimerInfo.mTimerNowTimeMs = i_time;
}

int dComIfG_play_c::getTimerNowTimeMs() {
    return mTimerInfo.mTimerNowTimeMs;
}

void dComIfG_play_c::setTimerLimitTimeMs(int i_time) {
    mTimerInfo.mTimerLimitTimeMs = i_time;
}

int dComIfG_play_c::getTimerLimitTimeMs() {
    return mTimerInfo.mTimerLimitTimeMs;
}

void dComIfG_play_c::setTimerMode(int i_mode) {
    mTimerInfo.mTimerMode = i_mode;
}

int dComIfG_play_c::getTimerMode() {
    return mTimerInfo.mTimerMode;
}

void dComIfG_play_c::setTimerType(u8 i_type) {
    mTimerInfo.mTimerType = i_type;
}

u8 dComIfG_play_c::getTimerType() {
    return mTimerInfo.mTimerType;
}

void dComIfG_play_c::setTimerPtr(dTimer_c* i_ptr) {
    mTimerInfo.mTimerPtr = i_ptr;
}

dTimer_c* dComIfG_play_c::getTimerPtr() {
    return mTimerInfo.mTimerPtr;
}

#if PLATFORM_WII || VERSION == VERSION_SHIELD_DEBUG
dComIfG_inf_c::baseCsr_c* dComIfG_inf_c::m_baseCsr;
dComIfG_inf_c::baseCsr_c::navi_c* dComIfG_inf_c::baseCsr_c::m_navi;
dPa_hermiteEcallBack_c dComIfG_inf_c::baseCsr_c::m_blurCB;

void dComIfG_inf_c::baseCsr_c::navi_c::create() {
    { int unused; }
    m_heap = mDoExt_createSolidHeapFromGameToCurrent(0, 32);
    JUT_ASSERT(1323, m_heap != NULL);

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("NNGC", "nv.bmd");

    JUT_ASSERT(1327, modelData != NULL);

    m_model = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    JUT_ASSERT(1331, m_model != NULL);

    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes("NNGC", "waitA.bck");
    JUT_ASSERT(1334, bck != NULL);

    int rt = m_bck.init(bck, 1, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false);
    JUT_ASSERT(1336, rt);

    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("NNGC", "nv_color.brk");
    rt = m_brk.init(modelData, brk, 0, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
    JUT_ASSERT(1340, rt);

    mDoExt_adjustSolidHeap(m_heap);
    mDoExt_restoreCurrentHeap();
}

bool dComIfG_inf_c::baseCsr_c::navi_c::draw(f32 param_1, f32 param_2, u8 param_3) {
    f32 f29 = (param_1 - mDoGph_gInf_c::getMinXF()) / mDoGph_gInf_c::getWidthF();
    f32 f31 = (param_2 - mDoGph_gInf_c::getMinYF()) / mDoGph_gInf_c::getHeightF();
    f32 f28 = f29 - field_0x58;
    f32 f27 = f31 - field_0x5c;
    field_0x58 = f29;
    field_0x5c = f31;
    cXyz spdc(param_1 - 304.0f, param_2 - 224.0f, 0.0f);

    f32 target = param_3 != 0 ? 1.5f : 0.0f;

    cLib_chaseF(&field_0x54, target, 0.15f);

    if (field_0x54 == 0.0f) {
        return true;
    }

    s16 r27 = cLib_targetAngleY(&field_0x40, &spdc);
    cLib_addCalcAngleS2(&field_0x4c.y, r27, 6, 4000);
    field_0x40 = spdc;
    mDoMtx_stack_c::transS(field_0x40);
    mDoMtx_stack_c::YrotM(field_0x4c.y);
    mDoMtx_stack_c::scaleM(field_0x54, -field_0x54, field_0x54);
    m_model->setBaseTRMtx(mDoMtx_stack_c::get());
    m_bck.play();
    J3DModelData* modelData = m_model->getModelData();
    m_bck.entry(modelData);
    m_brk.entry(modelData);
    dComIfGd_setListCursor();
    mDoExt_modelUpdateDL(m_model);
    dComIfGd_setList();

    if (0.0f <= field_0x58 && field_0x58 <= 1.0f && 0.0f <= f31 && f31 <= 1.0f) {
        f32 sqrt = JMAFastSqrt(SQUARE(f28) + SQUARE(f27));
        Z2GetAudioMgr()->playNaviFlySound(field_0x58, cLib_maxLimit(sqrt, 1.0f));
    }

    mParticleId = dComIfGp_particle_set(mParticleId, ID_ZR_J_2DNV_TAIL_A, &field_0x40, &field_0x4c, NULL);
    dComIfGp_particle_levelEmitterOnEventMove(mParticleId);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleId);
    if (emitter != NULL) {
        f32 f30 = field_0x54 * 0.5f;
        JGeometry::TVec3<f32> scale(f30, f30, f30);
        emitter->setGlobalScale(scale);
    }

    return param_3 == 0;
}

dComIfG_inf_c::baseCsr_c::baseCsr_c(u8 param_1) {
    field_0x13c = 1;
    field_0x13d = param_1;
    field_0x13e = 1;
    m_blurCB.setOldPosP(&mDoGph_gInf_c::csr_c::m_oldEffPos, &mDoGph_gInf_c::csr_c::m_oldOldEffPos);
}

void dComIfG_inf_c::baseCsr_c::draw(f32 param_1, f32 param_2) {
    static cXyz effScale(0.53f, 0.53f, 0.53f);
    u32 r28 = 0;
    bool local_6a = true;
    bool local_6b = true;
    bool local_6c = true;
    bool local_6d = true;
    bool local_6e = true;

    if (field_0x13e && dComIfGp_event_runCheck()) {
        local_6e = false;
    }

    if (!local_6e && !dComIfGp_isPauseFlag()) {
        local_6d = false;
    }

    if (!local_6d) {
        bool local_6f = false;
        if (dMsgObject_getMsgObjectClass() && dMsgObject_isSelectTalkNowCheck()) {
            local_6f = true;
        }

        if (!local_6f) {
            local_6c = false;
        }
    }

    if (!local_6c && !dMeter2Info_isShopTalkFlag()) {
        local_6b = false;
    }

    if (!local_6b && dComIfGp_isHeapLockFlag() != 6) {
        local_6a = false;
    }

    int r27 = local_6a;
    bool local_70 = false;
    if (r27 && field_0x13d) {
        local_70 = true;
    }

    if (m_navi->draw(param_1, param_2, local_70) && r27 && field_0x13c) {
        r28 = 0xFF;
    }

    J2DPicture* picture = field_0x8.getPicture(MULTI_CHAR('cursor00'));
    JUT_ASSERT(1450, picture != NULL);
    picture->scale(1.3f, 1.3f);
    JUtility::TColor color = picture->getWhite();
    cLib_chaseUC(&color.a, r28, 0x20);
    picture->setWhite(color);

    if (color.a != 0) {
        picture->translate(param_1, param_2);
        dComIfGd_set2DXlu(&field_0x8);

        if (color.a == 0xFF) {
            f32 absVal = mDoGph_gInf_c::csr_c::m_nowEffPos.abs2(mDoGph_gInf_c::csr_c::m_oldEffPos);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mDoGph_gInf_c::csr_c::m_blurID);
            if (absVal > 289.0f || (emitter != NULL && absVal > 9.0f)) {
                mDoGph_gInf_c::csr_c::m_blurID = g_dComIfG_gameInfo.play.getParticle()->set(mDoGph_gInf_c::csr_c::m_blurID, 17, ID_ZR_J_POINTINGCURSOR_TAIL_B,
                                                                                            &mDoGph_gInf_c::csr_c::m_nowEffPos, NULL, NULL, &effScale, 0xFF, &m_blurCB,
                                                                                            -1, NULL, NULL, NULL, 1.0f);
                dComIfGp_particle_levelEmitterOnEventMove(mDoGph_gInf_c::csr_c::m_blurID);
            }

            m_blurCB.setRate(3.5f);
            m_blurCB.setMaxCnt(40);
        }
    }
}

void dComIfG_inf_c::baseCsr_c::create() {
    dRes_info_c* resInfo = dComIfG_getObjectResInfo("NNGC");
    JUT_ASSERT(1495, resInfo != NULL);

    int rt = field_0x8.create(resInfo->getArchive(), "zelda_pointing_cursor_navi.blo");
    JUT_ASSERT(1498, rt);

    J2DPicture* picture = field_0x8.getPicture(MULTI_CHAR('cursor00'));
    JUT_ASSERT(1500, picture != NULL);
    JUtility::TColor color = picture->getWhite();
    color.a = 0;
    picture->setWhite(color);

    J2DScreen* screen = field_0x8.getScreen();
    screen->setUserInfo('n_43');

    if (m_navi) {
        return;
    }
    m_navi = new navi_c();
    JUT_ASSERT(1517, m_navi != NULL);
    m_navi->create();
}

void dComIfG_inf_c::baseCsr_c::particleExecute() {
    if (m_navi != NULL) {
        dComIfGp_particle_levelExecute(m_navi->getParticleId());
    }
}

void dComIfG_inf_c::anmCsr_c::draw(f32 param_1, f32 param_2) {
    field_0x8.setPos(MULTI_CHAR('lock_n'), param_1, param_2);
    dComIfGd_set2DXlu(&field_0x8);
}
#endif

void dComIfG_inf_c::ct() {
    mFadeBrightness = 255;
    #if DEBUG
    mIsDebugMode = 0;
    #endif
    play.ct();
    mWorldDark = 0;
    field_0x1ddfa = -1;
    mPolyDamageOff = false;
    field_0x1de00 = 0;
    field_0x1de04 = 0;
    field_0x1de08 = 0;
    field_0x1ddfc = 0;
    field_0x1de09 = 0xFF;
    field_0x1de0a = 0xFF;
}

#if PLATFORM_WII || VERSION == VERSION_SHIELD_DEBUG
void dComIfG_inf_c::createBaseCsr() {
    JUT_ASSERT(1622, m_baseCsr == NULL);
    m_baseCsr = new baseCsr_c(1);
    JUT_ASSERT(1624, m_baseCsr != NULL);
    m_baseCsr->create();
    mDoGph_gInf_c::entryBaseCsr(m_baseCsr);
}
#endif

GXColor g_clearColor = {0, 0, 0, 0};

GXColor g_blackColor = {0, 0, 0, 255};

int dComIfG_changeOpeningScene(scene_class* i_scene, s16 i_procName) {
    dComIfGp_offEnableNextStage();
    dComIfGp_setNextStage("F_SP102", 100, 0, 10);
    mDoAud_setSceneName(dComIfGp_getNextStageName(), dComIfGp_getNextStageRoomNo(),
                        dComIfGp_getNextStageLayer());
    dComIfGs_setRestartRoomParam(0);

    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
    }

    fopScnM_ChangeReq(i_scene, i_procName, 0, 30);
    fopScnM_ReRequest(i_procName, 0);

    return 1;
}

dComIfG_inf_c g_dComIfG_gameInfo;

BOOL dComIfG_resetToOpening(scene_class* i_scene) {
    #if PLATFORM_WII || VERSION == VERSION_SHIELD_DEBUG
    if (mDoRst::isShutdown() || mDoRst::isReturnToMenu() || !mDoRst::isReset() || mDoGph_gInf_c::getFader()->getStatus() == 2) {
        return 0;
    }
    #else
    if (mDoRst::isReturnToMenu() || !mDoRst::isReset() || mDoGph_gInf_c::getFader()->getStatus() == 2) {
        return 0;
    }
    #endif

    dComIfG_changeOpeningScene(i_scene, PROC_OPENING_SCENE);
    mDoAud_bgmStop(30);
    mDoAud_resetProcess();
    #if PLATFORM_WII || VERSION == VERSION_SHIELD_DEBUG
    mDoGph_gInf_c::resetDimming();
    #endif
    return 1;
}

static int phase_1(char* i_arcName) {
    if (!dComIfG_setObjectRes(i_arcName, (u8)0, NULL)) {
        OSReport_Error("%s.arc Read Error !!\n", i_arcName);
        return cPhs_ERROR_e;
    }

    return cPhs_NEXT_e;
}

static int phase_2(char* i_arcName) {
    int syncStatus = dComIfG_syncObjectRes(i_arcName);

    if (syncStatus < 0) {
        OSReport_Error("%s.arc Sync Read Error !!\n", i_arcName);
        return cPhs_ERROR_e;
    }

    if (syncStatus > 0) {
        return cPhs_INIT_e;
    } else {
        return cPhs_NEXT_e;
    }
}

static int phase_3(char*) {
    return cPhs_COMPLEATE_e;
}

/**
 * Attempts to load a Resource Archive (*.arc) into the Resource Control.
 * The archive must be successfully loaded into the Resource Control before
 * calling dComIfG_getObjectRes / dComIfG_getStageRes
 * @param i_phase Pointer to phase request for handling resource loading process
 * @param i_arcName Name of archive to be loaded
 * @return Loading phase state. cPhs_COMPLEATE_e if loaded successfully
 */
int dComIfG_resLoad(request_of_phase_process_class* i_phase, char const* i_arcName) {
    static int (*l_method[3])(void*) = {(int (*)(void*))phase_1, (int (*)(void*))phase_2,
                                        (int (*)(void*))phase_3};

    if (i_phase->id == 2) {
        return cPhs_COMPLEATE_e;
    }

    return dComLbG_PhaseHandler(i_phase, l_method, (void*)i_arcName);
}

static int phase_01(phaseParam_c* i_phasePrm) {
    JKRHeap* r30;
    if (dComIfG_setObjectRes(i_phasePrm->mResName, (u8)0, i_phasePrm->mpHeap) == 0) {
        r30 = i_phasePrm->mpHeap;
        return cPhs_ERROR_e;
    }

    return cPhs_NEXT_e;
}

static int phase_02(phaseParam_c* i_phasePrm) {
    int syncStatus = dComIfG_syncObjectRes(i_phasePrm->mResName);
    if (syncStatus < 0) {
        return cPhs_ERROR_e;
    }

    if (syncStatus > 0) {
        return cPhs_INIT_e;
    } else {
        return cPhs_NEXT_e;
    }
}

static int phase_03(phaseParam_c*) {
    return cPhs_INIT_e;
}

/**
 * Attempts to load a Resource Archive (*.arc) into the Resource Control using a specified heap.
 * The archive must be successfully loaded into the Resource Control before
 * calling dComIfG_getObjectRes / dComIfG_getStageRes
 * @param i_phase Pointer to phase request for handling resource loading process
 * @param i_arcName Name of archive to be loaded
 * @param i_heap Pointer to heap to load resources into
 * @return Loading phase state. cPhs_COMPLEATE_e if loaded successfully
 */
int dComIfG_resLoad(request_of_phase_process_class* i_phase, char const* i_resName,
                    JKRHeap* i_heap) {
    static int (*l_method[3])(void*) = {(int (*)(void*))phase_01, (int (*)(void*))phase_02,
                                        (int (*)(void*))phase_03};

    if (i_phase->id == 2) {
        return cPhs_COMPLEATE_e;
    }

    phaseParam_c param(i_resName, i_heap);
    return dComLbG_PhaseHandler(i_phase, l_method, &param);
}

/**
 * Attempts to unload a Resource Archive (*.arc) from the Resource Control.
 * This should be called from a process' delete method to prevent a memory leak.
 * @param i_phase Pointer to phase request for handling resource unloading process
 * @param i_arcName Name of archive to be unloaded
 * @return TRUE if successful, FALSE otherwise
 */
int dComIfG_resDelete(request_of_phase_process_class* i_phase, char const* i_resName) {
    JUT_ASSERT(1889, i_phase->id != 1);
    if (i_phase->id != 2) {
        return 0;
    }

    int r30 = dComIfG_deleteObjectResMain(i_resName);
    i_phase->id = 0;
    return 1;
}

s8 dComIfGp_getReverb(int i_roomNo) {
    return dStage_roomRead_dt_c_GetReverbStage(*dComIfGp_getStageRoom(), i_roomNo);
}

int dComIfGd_setSimpleShadow(cXyz* i_pos, f32 param_1, f32 param_2, cBgS_PolyInfo& param_3,
                             s16 i_angle, f32 param_5, GXTexObj* i_tex) {
    if (param_3.ChkSetInfo() && -G_CM3D_F_INF != param_1) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(param_3, &plane);

        return dComIfGd_setSimpleShadow(i_pos, param_1, param_2, &plane.mNormal, i_angle, param_5,
                                        i_tex);
    } else {
        return 0;
    }
}

bool dComIfGp_getMapTrans(int i_roomNo, f32* o_transX, f32* o_transY, s16* o_angle) {
    dStage_Multi_c* multi = dComIfGp_getMulti();
    if (multi == NULL) {
        return false;
    }

    dStage_Mult_info* info = multi->m_entries;
    for (int i = 0; i < multi->num; i++) {
        if (i_roomNo == info->mRoomNo) {
            *o_transX = info->mTransX;
            *o_transY = info->mTransY;
            *o_angle = info->mAngle;
            return true;
        }
        info++;
    }

    return false;
}

stage_camera_class* dComIfGp_getRoomCamera(int i_roomNo) {
    dStage_roomDt_c* room_dt = dComIfGp_roomControl_getStatusRoomDt(i_roomNo);

    if (room_dt == NULL) {
        return NULL;
    }

    return room_dt->getCamera();
}

stage_arrow_class* dComIfGp_getRoomArrow(int i_roomNo) {
    dStage_roomDt_c* room_dt = dComIfGp_roomControl_getStatusRoomDt(i_roomNo);

    if (room_dt == NULL) {
        return NULL;
    }

    return room_dt->getArrow();
}

void dComIfGp_setNextStage(char const* i_stage, s16 i_point, s8 i_roomNo, s8 i_layer,
                           f32 i_lastSpeed, u32 i_lastMode, int i_setPoint, s8 i_wipe,
                           s16 i_lastAngle, int param_9, int i_wipeSpeedT) {
    if (i_layer >= 15) {
        i_layer = -1;
    }

    if (dComIfGs_isPlayerFieldLastStayFieldDataExistFlag() &&
        daPy_getLinkPlayerActorClass() != NULL)
    {
        s8 curPoint = fopAcM_GetRoomNo(daPy_getLinkPlayerActorClass());
        cXyz pos = dMapInfo_n::getMapPlayerPos();
        s16 angle = daPy_getLinkPlayerActorClass()->shape_angle.y;
        dComIfGs_setPlayerFieldLastStayInfo(dComIfGp_getStartStageName(), pos, angle, curPoint,
                                            dComIfGp_getNowLevel());
    }

    // Set Key Num to 2 if loading Wagon Escort after King Bulblin 2
    if (!strcmp(i_stage, "F_SP121") && i_roomNo == 13 && (i_point == 99 || i_point == 98) &&
        i_layer == 2)
    {
        dComIfGs_setKeyNum(6, 2);
    } else {
        dComIfGs_setKeyNum(6, 0);
    }

    if (daAlink_getAlinkActorClass() != NULL) {
        daAlink_getAlinkActorClass()->setLastSceneMode(&i_lastMode);
    }

    if (strcmp(dMeter2Info_getSaveStageName(), "") && strcmp(i_stage, dMeter2Info_getSaveStageName())) {
        dMeter2Info_setSaveStageName("");
    }

    u8 wipe_speed;
    switch (i_wipeSpeedT) {
    case 0:
        wipe_speed = 26;
        break;
    case 1:
        wipe_speed = 13;
        break;
    case 2:
        wipe_speed = 6;
        break;
    case 3:
        wipe_speed = 1;
        break;
    default:
        wipe_speed = 26;
        break;
    }

    g_dComIfG_gameInfo.play.setNextStage(i_stage, i_roomNo, i_point, i_layer, i_wipe, wipe_speed);
    g_dComIfG_gameInfo.info.getRestart().setLastSceneInfo(i_lastSpeed, i_lastMode, i_lastAngle);
    if (i_setPoint) {
        dComIfGs_setStartPoint(i_point);
    }
}

void dComIfGp_setNextStage(char const* i_stage, s16 i_point, s8 i_roomNo, s8 i_layer) {
    dComIfGp_setNextStage(i_stage, i_point, i_roomNo, i_layer, 0.0f, 0, 1, 0, 0, 1, 0);
}

static void dummy1() {
    dComIfGs_offTbox(0);
    dComIfGs_onTbox(0);
    g_dComIfG_gameInfo.info.getSavedata().getSave(0);
}

BOOL dComIfGs_isStageTbox(int i_stageNo, int i_no) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isTbox(i_no);
    } else {
        return dComIfGs_isSaveTbox(i_stageNo, i_no);
    }
}

void dComIfGs_onStageSwitch(int i_stageNo, int i_no) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onSwitch(i_no, -1);
    }

    dComIfGs_onSaveSwitch(i_stageNo, i_no);
}

void dComIfGs_offStageSwitch(int i_stageNo, int i_no) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offSwitch(i_no, -1);
    }

    dComIfGs_offSaveSwitch(i_stageNo, i_no);
}

BOOL dComIfGs_isStageSwitch(int i_stageNo, int i_no) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isSwitch(i_no, -1);
    } else {
        return dComIfGs_isSaveSwitch(i_stageNo, i_no);
    }
}

void dComIfGs_onDungeonItemMap(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onDungeonItemMap();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onDungeonItemMap();
}

void dComIfGs_offDungeonItemMap(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offDungeonItemMap();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offDungeonItemMap();
}

s32 dComIfGs_isDungeonItemMap(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isDungeonItemMap();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isDungeonItemMap();
}

void dComIfGs_onDungeonItemCompass(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onDungeonItemCompass();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onDungeonItemCompass();
}

void dComIfGs_offDungeonItemCompass(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offDungeonItemCompass();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offDungeonItemCompass();
}

s32 dComIfGs_isDungeonItemCompass(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isDungeonItemCompass();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isDungeonItemCompass();
}

void dComIfGs_onDungeonItemBossKey(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onDungeonItemBossKey();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onDungeonItemBossKey();
}

void dComIfGs_offDungeonItemBossKey(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offDungeonItemBossKey();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offDungeonItemBossKey();
}

s32 dComIfGs_isDungeonItemBossKey(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isDungeonItemBossKey();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isDungeonItemBossKey();
}

void dComIfGs_onStageBossEnemy(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onStageBossEnemy();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onStageBossEnemy();
}

void dComIfGs_offStageBossEnemy(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offStageBossEnemy();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offStageBossEnemy();
}

s32 dComIfGs_isStageBossEnemy(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isStageBossEnemy();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isStageBossEnemy();
}

void dComIfGs_onStageMiddleBoss(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onStageMiddleBoss();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onStageBossEnemy2();
}

void dComIfGs_offStageMiddleBoss(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offStageMiddleBoss();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offStageBossEnemy2();
}

s32 dComIfGs_isStageMiddleBoss(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isStageMiddleBoss();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isStageBossEnemy2();
}

void dComIfGs_onStageLife(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onStageLife();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onStageLife();
}

void dComIfGs_offStageLife(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offStageLife();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offStageLife();
}

s32 dComIfGs_isStageLife(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isStageLife();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isStageLife();
}

void dComIfGs_onStageBossDemo(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onStageBossDemo();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onStageBossDemo();
}

void dComIfGs_offStageBossDemo(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offStageBossDemo();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offStageBossDemo();
}

s32 dComIfGs_isStageBossDemo(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isStageBossDemo();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isStageBossDemo();
}

void dComIfGs_onDungeonItemWarp(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_onDungeonItemWarp();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onDungeonItemWarp();
}

void dComIfGs_offDungeonItemWarp(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        dComIfGs_offDungeonItemWarp();
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offDungeonItemWarp();
}

s32 dComIfGs_isDungeonItemWarp(int i_stageNo) {
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isDungeonItemWarp();
    } else {
        return g_dComIfG_gameInfo.info.getSavedata()
            .getSave(i_stageNo)
            .getBit()
            .isDungeonItemWarp();
    }
}

void dComIfGs_BossLife_public_Set(s8 param_0) {
    g_dComIfG_gameInfo.field_0x1ddfa = param_0;
}

s8 dComIfGs_BossLife_public_Get() {
    return g_dComIfG_gameInfo.field_0x1ddfa;
}

void dComIfGs_sense_type_change_Set(s8 param_0) {
    g_dComIfG_gameInfo.field_0x1ddfa = param_0;
}

s8 dComIfGs_sense_type_change_Get() {
    return g_dComIfG_gameInfo.field_0x1ddfa;
}

void dComIfGs_onZoneSwitch(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_onSvZoneSwitch(zone_no, i_swBit);
}

void dComIfGs_offZoneSwitch(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_offSvZoneSwitch(zone_no, i_swBit);
}

BOOL dComIfGs_isZoneSwitch(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return false;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    return dComIfGs_isSvZoneSwitch(zone_no, i_swBit);
}

void dComIfGs_onOneZoneSwitch(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_onSvOneZoneSwitch(zone_no, i_swBit);
}

void dComIfGs_offOneZoneSwitch(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_offSvOneZoneSwitch(zone_no, i_swBit);
}

BOOL dComIfGs_isOneZoneSwitch(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return false;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    return dComIfGs_isSvOneZoneSwitch(zone_no, i_swBit);
}

void dComIfGs_onZoneItem(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_onSvZoneItem(zone_no, i_swBit);
}

void dComIfGs_offZoneItem(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_offSvZoneItem(zone_no, i_swBit);
}

BOOL dComIfGs_isZoneItem(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return false;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    return dComIfGs_isSvZoneItem(zone_no, i_swBit);
}

void dComIfGs_onOneZoneItem(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_onSvOneZoneItem(zone_no, i_swBit);
}

void dComIfGs_offOneZoneItem(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    dComIfGs_offSvOneZoneItem(zone_no, i_swBit);
}

BOOL dComIfGs_isOneZoneItem(int i_swBit, int i_roomNo) {
    int room_no;
    if (i_roomNo >= 0) {
        room_no = i_roomNo;
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
        if (room_no < 0) {
            return false;
        }
    }

    int zone_no = dComIfGp_roomControl_getZoneNo(room_no);
    return dComIfGs_isSvOneZoneItem(zone_no, i_swBit);
}

u16 dComIfGs_getMaxLifeGauge() {
    return (dComIfGs_getMaxLife() / 5) * 4;
}

void dComIfGs_onGetMagicUseFlag() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().onMagicFlag(0);
    if (dComIfGs_getMaxMagic() == 0) {
        dComIfGp_setItemMaxMagicCount(16);
        dComIfGp_setItemMagicCount(16);
    }
}

void dComIfGs_setSelectItemIndex(int i_no, u8 i_slotNo) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectItemIndex(i_no, i_slotNo);
    dComIfGp_setSelectItem(i_no);
}

void dComIfGs_setMixItemIndex(int i_no, u8 i_slotNo) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMixItemIndex(i_no, i_slotNo);
}

u8 dComIfGs_getSelectMixItemNoArrowIndex(int i_selmixItemIdx) {
    u8 sel_item_slotNo = dComIfGs_getSelectItemIndex(i_selmixItemIdx);
    u8 mix_item_slotNo = dComIfGs_getMixItemIndex(i_selmixItemIdx);

    if (sel_item_slotNo >= SLOT_15 && sel_item_slotNo < SLOT_18) {
        return sel_item_slotNo;
    }

    if (mix_item_slotNo != fpcNm_ITEM_NONE && mix_item_slotNo >= SLOT_15 && mix_item_slotNo < SLOT_18) {
        return mix_item_slotNo;
    }

    return 0xFF;
}

u8 dComIfGs_getMixItemIndex(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMixItemIndex(i_no);
}

void dComIfGp_setSelectItem(int i_selItemIdx) {
    if (i_selItemIdx == SELECT_ITEM_DOWN) {
        if (dComIfGs_getSelectItemIndex(i_selItemIdx) != 0xFF) {
            u8 selItem_slotNo = dComIfGs_getSelectItemIndex(i_selItemIdx);
            g_dComIfG_gameInfo.play.setSelectItem(i_selItemIdx, selItem_slotNo);

            if (selItem_slotNo == 0xFF) {
                dComIfGs_setSelectItemIndex(i_selItemIdx, 0xFF);
            }
        } else {
            g_dComIfG_gameInfo.play.setSelectItem(i_selItemIdx, fpcNm_ITEM_NONE);
        }
    } else if (dComIfGs_getSelectItemIndex(i_selItemIdx) != 0xFF) {
        u8 item = dComIfGs_getItem(dComIfGs_getSelectItemIndex(i_selItemIdx), false);
        g_dComIfG_gameInfo.play.setSelectItem(i_selItemIdx, item);

        if (item == fpcNm_ITEM_NONE) {
            dComIfGs_setSelectItemIndex(i_selItemIdx, 0xFF);
        }
    } else {
        g_dComIfG_gameInfo.play.setSelectItem(i_selItemIdx, fpcNm_ITEM_NONE);
    }
}

u8 dComIfGp_getSelectItem(int i_selItemIdx) {
    u8 playItem = g_dComIfG_gameInfo.play.getSelectItem(i_selItemIdx);

    if ((i_selItemIdx == SELECT_ITEM_X || i_selItemIdx == SELECT_ITEM_Y) &&
        dComIfGs_getMixItemIndex(i_selItemIdx) != 0xFF)
    {
        u8 saveItem = dComIfGs_getItem(dComIfGs_getMixItemIndex(i_selItemIdx), false);

        if (saveItem == fpcNm_ITEM_BOW) {
            saveItem = playItem;
            playItem = fpcNm_ITEM_BOW;
        } else if (saveItem == fpcNm_ITEM_FISHING_ROD_1) {
            saveItem = playItem;
            playItem = fpcNm_ITEM_FISHING_ROD_1;
        }

        if (playItem == fpcNm_ITEM_BOW) {
            switch (saveItem) {
            case fpcNm_ITEM_NORMAL_BOMB:
            case fpcNm_ITEM_WATER_BOMB:
            case fpcNm_ITEM_POKE_BOMB:
                playItem = fpcNm_ITEM_BOMB_ARROW;
                break;
            case fpcNm_ITEM_HAWK_EYE:
                playItem = fpcNm_ITEM_HAWK_ARROW;
                break;
            }
        } else if (playItem == fpcNm_ITEM_FISHING_ROD_1) {
            switch (saveItem) {
            case fpcNm_ITEM_BEE_CHILD:
                playItem = fpcNm_ITEM_BEE_ROD;
                break;
            case fpcNm_ITEM_WORM:
                playItem = fpcNm_ITEM_WORM_ROD;
                break;
            case fpcNm_ITEM_ZORAS_JEWEL:
                playItem = fpcNm_ITEM_JEWEL_ROD;
                break;
            }
        }
    }

    return playItem;
}

void dComIfGp_mapShow() {
    dComIfGs_offEventBit(dSv_event_flag_c::MAP_VISIBLE);
}

void dComIfGp_mapHide() {
    dComIfGs_onEventBit(dSv_event_flag_c::MAP_VISIBLE);
}

bool dComIfGp_checkMapShow() {
    return !dComIfGs_isEventBit(dSv_event_flag_c::MAP_VISIBLE);
}

s32 dComIfGp_setHeapLockFlag(u8 flag) {
    if (flag == 7 || flag == 8) {
        if (!dComIfGp_isHeapLockFlag()) {
            dComIfGp_createSubExpHeap2D();
        }
        g_dComIfG_gameInfo.play.setHeapLockFlag(5);

        int heapIndex = dComIfGp_checkEmptySubHeap2D();
        if (heapIndex >= 0) {
            dComIfGp_setSubHeapLockFlag(heapIndex, flag);
        }
    } else if (flag == 0) {
        dComIfGp_offHeapLockFlag(0);
    } else {
        g_dComIfG_gameInfo.play.setHeapLockFlag(flag);
    }

    return 1;
}

s32 dComIfGp_offHeapLockFlag(int flag) {
    if (dComIfGp_isHeapLockFlag() == 5) {
        if (flag == 7 || flag == 8) {
            int heapIndex = dComIfGp_searchUseSubHeap2D(flag);
            if (heapIndex >= 0) {
                dComIfGp_setSubHeapLockFlag(heapIndex, 0);
            }

            if (!dComIfGp_getSubHeapLockFlag(0) && !dComIfGp_getSubHeapLockFlag(1)) {
                dComIfGp_destroySubExpHeap2D();
                g_dComIfG_gameInfo.play.offHeapLockFlag();
            }
        }
    } else {
        g_dComIfG_gameInfo.play.offHeapLockFlag();
    }

    return 1;
}

void dComIfGp_createSubExpHeap2D() {
    u32 size = dComIfGp_getExpHeap2D()->getTotalFreeSize() * (2.0f / 5.0f);

    for (int i = 0; i < 2; i++) {
        JUT_ASSERT(3573, dComIfGp_getSubExpHeap2D(i) == NULL);
        if (dComIfGp_getSubExpHeap2D(i) == NULL) {
            JKRExpHeap* i_heap = JKRCreateExpHeap(size, dComIfGp_getExpHeap2D(), false);
            JUT_ASSERT(3576, i_heap != NULL);
            dComIfGp_setSubExpHeap2D(i, i_heap);
        }
    }
}

void dComIfGp_destroySubExpHeap2D() {
    for (int i = 0; i < 2; i++) {
        if (dComIfGp_getSubExpHeap2D(i) != NULL) {
            mDoExt_destroyExpHeap(dComIfGp_getSubExpHeap2D(i));
            dComIfGp_setSubExpHeap2D(i, NULL);
        }
    }
}

int dComIfGp_checkEmptySubHeap2D() {
    if (dComIfGp_isHeapLockFlag() == 5) {
        for (int i = 0; i < 2; i++) {
            if (dComIfGp_getSubHeapLockFlag(i) == 0) {
                return i;
            }
        }
    }
    return -1;
}

int dComIfGp_searchUseSubHeap2D(int flag) {
    if (dComIfGp_isHeapLockFlag() == 5) {
        for (int i = 0; i < 2; i++) {
            if (flag == dComIfGp_getSubHeapLockFlag(i)) {
                return i;
            }
        }
    }
    return -1;
}

JKRExpHeap* dComIfGp_getSubHeap2D(int flag) {
    if (dComIfGp_isHeapLockFlag() == 5) {
        for (int i = 0; i < 2; i++) {
            if (flag == dComIfGp_getSubHeapLockFlag(i)) {
                return dComIfGp_getSubExpHeap2D(i);
            }
        }
    }
    return NULL;
}

u8 dComIfGs_checkGetInsectNum() {
    static u8 l_itemno[24] = {
        fpcNm_ITEM_M_BEETLE,      fpcNm_ITEM_F_BEETLE,      fpcNm_ITEM_M_BUTTERFLY, fpcNm_ITEM_F_BUTTERFLY, fpcNm_ITEM_M_STAG_BEETLE, fpcNm_ITEM_F_STAG_BEETLE,
        fpcNm_ITEM_M_GRASSHOPPER, fpcNm_ITEM_F_GRASSHOPPER, fpcNm_ITEM_M_NANAFUSHI, fpcNm_ITEM_F_NANAFUSHI, fpcNm_ITEM_M_DANGOMUSHI,  fpcNm_ITEM_F_DANGOMUSHI,
        fpcNm_ITEM_M_MANTIS,      fpcNm_ITEM_F_MANTIS,      fpcNm_ITEM_M_LADYBUG,   fpcNm_ITEM_F_LADYBUG,   fpcNm_ITEM_M_SNAIL,       fpcNm_ITEM_F_SNAIL,
        fpcNm_ITEM_M_DRAGONFLY,   fpcNm_ITEM_F_DRAGONFLY,   fpcNm_ITEM_M_ANT,       fpcNm_ITEM_F_ANT,       fpcNm_ITEM_M_MAYFLY,      fpcNm_ITEM_F_MAYFLY,
    };

    u8 insectCount = 0;
    u8* insectList = l_itemno;

    for (int i = 0; i < ARRAY_SIZEU(l_itemno); i++) {
        if (dComIfGs_isItemFirstBit(*insectList++) &&
            dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x191 + i]))
        {
            insectCount++;
        }
    }
    return insectCount;
}

u8 dComIfGs_checkGetItem(u8 i_itemNo) {
    u8 count = 0;

    for (int i = 0; i < 60; i++) {
        if (i_itemNo == dComIfGs_getItem(i, true)) {
            count++;
        }
    }

    if (dComIfGs_getSelectEquipClothes() == i_itemNo) {
        count++;
    }

    if (dComIfGs_getSelectEquipSword() == i_itemNo) {
        count++;
    }

    if (dComIfGs_getSelectEquipShield() == i_itemNo) {
        count++;
    }

    return count;
}

u8 dComIfGs_getBottleMax() {
    return 10;
}

s16 dComIfGp_getSelectItemNum(int i_selItemIdx) {
    u8 selectItem = dComIfGp_getSelectItem(i_selItemIdx);
    s16 itemNum = 0;

    if (selectItem == fpcNm_ITEM_NORMAL_BOMB || selectItem == fpcNm_ITEM_WATER_BOMB || selectItem == fpcNm_ITEM_POKE_BOMB ||
        selectItem == fpcNm_ITEM_BOMB_ARROW)
    {
        u8 slot_no = dComIfGs_getSelectMixItemNoArrowIndex(i_selItemIdx) - SLOT_15;
        itemNum = dComIfGs_getBombNum(slot_no);
    } else if (selectItem == fpcNm_ITEM_PACHINKO) {
        itemNum = dComIfGs_getPachinkoNum();
    } else if (selectItem == fpcNm_ITEM_BEE_CHILD) {
        u8 slot_no = dComIfGs_getSelectItemIndex(i_selItemIdx) - SLOT_11;
        itemNum = dComIfGs_getBottleNum(slot_no);
    }

    return itemNum;
}

int dComIfGp_getSelectItemMaxNum(int i_selItemIdx) {
    u8 selectItem = dComIfGp_getSelectItem(i_selItemIdx);
    int itemNum = 0;

    if (selectItem == fpcNm_ITEM_BOMB_BAG_LV1) {
        itemNum = 1;
    } else if (selectItem == fpcNm_ITEM_NORMAL_BOMB || selectItem == fpcNm_ITEM_WATER_BOMB || selectItem == fpcNm_ITEM_POKE_BOMB ||
               selectItem == fpcNm_ITEM_BOMB_ARROW)
    {
        itemNum = dComIfGs_getBombMax(selectItem);
    } else if (selectItem == fpcNm_ITEM_PACHINKO) {
        itemNum = dComIfGs_getPachinkoMax();
    } else if (selectItem == fpcNm_ITEM_BEE_CHILD) {
        itemNum = dComIfGs_getBottleMax();
    }

    return itemNum;
}

void dComIfGp_setSelectItemNum(int i_selItemIdx, s16 i_num) {
    u8 selectItem = dComIfGp_getSelectItem(i_selItemIdx);

    if (selectItem == fpcNm_ITEM_NORMAL_BOMB || selectItem == fpcNm_ITEM_WATER_BOMB || selectItem == fpcNm_ITEM_POKE_BOMB ||
        selectItem == fpcNm_ITEM_BOMB_ARROW)
    {
        u8 mix_slotNo = dComIfGs_getSelectMixItemNoArrowIndex(i_selItemIdx) - SLOT_15;

        if (i_num > dComIfGs_getBombMax(selectItem)) {
            i_num = dComIfGs_getBombMax(selectItem);
        }
        dComIfGs_setBombNum(mix_slotNo, i_num);
    } else if (selectItem == fpcNm_ITEM_PACHINKO) {
        dComIfGs_setPachinkoNum(i_num);
    } else if (selectItem == fpcNm_ITEM_BEE_CHILD) {
        u8 bottle_slot_no = dComIfGs_getSelectItemIndex(i_selItemIdx) - SLOT_11;

        if (i_num > dComIfGs_getBottleMax()) {
            i_num = dComIfGs_getBottleMax();
        }
        dComIfGs_setBottleNum(bottle_slot_no, i_num);
    }
}

void dComIfGp_addSelectItemNum(int i_selItemIdx, s16 i_num) {
    u8 selectItem = dComIfGp_getSelectItem(i_selItemIdx);

    if (selectItem == fpcNm_ITEM_NORMAL_BOMB || selectItem == fpcNm_ITEM_WATER_BOMB || selectItem == fpcNm_ITEM_POKE_BOMB ||
        selectItem == fpcNm_ITEM_BOMB_ARROW)
    {
        u8 slot_no = dComIfGs_getSelectMixItemNoArrowIndex(i_selItemIdx) - SLOT_15;
        dComIfGp_setItemBombNumCount(slot_no, i_num);
    } else if (selectItem == fpcNm_ITEM_PACHINKO) {
        dComIfGp_setItemPachinkoNumCount(i_num);
    } else if (selectItem == fpcNm_ITEM_BEE_CHILD) {
        u8 slot_no = dComIfGs_getSelectItemIndex(i_selItemIdx) - SLOT_11;
        dComIfGs_addBottleNum(slot_no, i_num);
    }
}

int dComIfGd_setShadow(u32 param_0, s8 param_1, J3DModel* param_2, cXyz* param_3, f32 param_4,
                       f32 param_5, f32 param_6, f32 param_7, cBgS_PolyInfo& param_8,
                       dKy_tevstr_c* param_9, s16 param_10, f32 param_11, GXTexObj* param_12) {
    if (param_7 <= -G_CM3D_F_INF) {
        return 0;
    } else {
        
        param_0 = dComIfGd_setRealShadow(param_0, param_1, param_2, param_3, param_4,
                                         param_6 - param_7, param_9);
        return param_0;
    }
}

void dComIfGs_gameStart() {
    dComIfGp_offEnableNextStage();

    dComIfGp_setNextStage(
        g_dComIfG_gameInfo.info.getPlayer().getPlayerReturnPlace().getName(),
        g_dComIfG_gameInfo.info.getPlayer().getPlayerReturnPlace().getPlayerStatus(),
        g_dComIfG_gameInfo.info.getPlayer().getPlayerReturnPlace().getRoomNo(),
        -1, 0.0f, 0, 1, 0, 0, 0, 0
    );
}

#if DEBUG
void dComIfG_playerStatusD() {
    dComIfGs_setDataNum(0);
    dComIfGs_setMaxLife(50);
    dComIfGs_setLife(20);
    dComIfGs_setRupee(64);
    dComIfGs_setMaxMagic(32);
    dComIfGs_setMagic(16);
    dComIfGs_setWalletSize(1);
    dComIfGs_setMaxOil(21600);
    dComIfGs_setOil(21600);
    dComIfGp_setMaxOxygen(600);
    dComIfGp_setOxygen(600);

    for (int i = 0; i < 4; i++) {
        dComIfGs_setMixItemIndex(i, 0xFF);
    }

    dComIfGs_setSelectItemIndex(0, SLOT_0);
    dComIfGs_setSelectItemIndex(1, SLOT_4);
    dComIfGs_setSelectItemIndex(2, 0xFF);
    dComIfGs_setSelectItemIndex(3, 0xFF);

    for (int i = 23; i >= 0; i--) {
        dComIfGs_setItem(i, fopMsgM_itemNumIdx(i));
    }

    for (int i = 0; i < 0x100; i++) {
        dComIfGs_onItemFirstBit(i);
    }

    dComIfGs_offItemFirstBit(fpcNm_ITEM_L2_KEY_PIECES1);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_L2_KEY_PIECES2);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_L2_KEY_PIECES3);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_LV2_BOSS_KEY);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_BOMB_BAG_LV2);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_TOMATO_PUREE);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_TASTE);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_POU_FIRE1);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_POU_FIRE2);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_POU_FIRE3);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_POU_FIRE4);

    for (int i = 0; i < 24; i++) {
        dComIfGs_offItemFirstBit(i + fpcNm_ITEM_M_BEETLE);
    }

    dComIfGs_offItemFirstBit(fpcNm_ITEM_LIGHT_SWORD);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_SHIELD);
    dComIfGs_offItemFirstBit(fpcNm_ITEM_ZORAS_JEWEL);

    for (int i = 0; i < 19; i++) {
        dComIfGs_offItemFirstBit(i);
    }

    dComIfGs_setCollectSmell(fpcNm_ITEM_SMELL_PUMPKIN);

    if (!mDoCPd_c::isConnect(PAD_3)) {
        dComIfGs_offItemFirstBit(fpcNm_ITEM_SMELL_POH);
    }

    dComIfGs_setArrowNum(30);
    dComIfGs_setArrowMax(30);
    dComIfGs_setPachinkoNum(dComIfGs_getPachinkoMax());
    dComIfGs_setBombNum(0, 30);
    dComIfGs_setBombNum(1, 15);
    dComIfGs_setBombNum(2, 10);

    for (int i = 0; i < 4; i++) {
        dComIfGs_setBottleNum(i, dComIfGs_getBottleMax());
    }

    dComIfGs_setSaveTotalTime(dComIfGs_getTotalTime());
    dComIfGs_setSaveStartTime(OSGetTime());

    dComIfGs_setBombNum(8, 30);
    dComIfGs_setBombMax(fpcNm_ITEM_NORMAL_BOMB, 30);
    dComIfGs_setBombMax(fpcNm_ITEM_WATER_BOMB, 15);
    dComIfGs_setBombMax(fpcNm_ITEM_POKE_BOMB, 10);

    dMeter2Info_setCloth(fpcNm_ITEM_WEAR_KOKIRI, false);
    dMeter2Info_setSword(fpcNm_ITEM_SWORD, false);
    dMeter2Info_setShield(fpcNm_ITEM_HYLIA_SHIELD, false);
    dComIfGs_onGetMagicUseFlag();

    dComIfGs_onEventBit(0x540);
    dComIfGs_onEventBit(0xc10);
    dComIfGs_onEventBit(0x510);
    dMeter2Info_offTempBit(0);
    dComIfGs_onEventBit(0x5c01);
    dComIfGs_onEventBit(0x5d80);

    if (!mDoCPd_c::isConnect(PAD_3)) {
        g_fmapHIO.mAllRegionsUnlocked = 0;
    } else {
        g_fmapHIO.mAllRegionsUnlocked = 1;
    }

    g_fmapHIO.mPortalWarpON = 1;
    g_fmapHIO.update();

    g_mwHIO.setArrowFlag(1);
    g_mwHIO.setPachinkoFlag(1);
    g_mwHIO.setBombFlag(1);
    g_mwHIO.update();
    g_mwHIO.setBombFlag(1);
}

void dComIfG_playerStatusD_pre_clear() {
    dComIfGs_setDataNum(0);
    dComIfGs_setMaxLife(15);
    dComIfGs_setLife(12);
    dComIfGs_setRupee(0);
    dComIfGs_setMaxMagic(0);
    dComIfGs_setMagic(0);
    dComIfGs_setWalletSize(0);
    dComIfGs_setMaxOil(21600);
    dComIfGs_setOil(21600);
    dComIfGp_setMaxOxygen(600);
    dComIfGp_setOxygen(600);

    for (int i = 0; i < 4; i++) {
        dComIfGs_setMixItemIndex(i, 0xFF);
        dComIfGs_setSelectItemIndex(i, 0xFF);
    }

    dComIfGs_setSelectEquipClothes(fpcNm_ITEM_WEAR_CASUAL);
    dComIfGp_setSelectEquipClothes(fpcNm_ITEM_WEAR_CASUAL);
    dComIfGs_setSelectEquipSword(fpcNm_ITEM_NONE);
    dComIfGp_setSelectEquipSword(fpcNm_ITEM_NONE);
    dComIfGs_setSelectEquipShield(fpcNm_ITEM_NONE);
    dComIfGp_setSelectEquipShield(fpcNm_ITEM_NONE);

    for (int i = 0; i < 24; i++) {
        dComIfGs_setItem(i, fpcNm_ITEM_NONE);
    }

    for (int i = 0; i < 0x100; i++) {
        dComIfGs_offItemFirstBit(i);
    }

    dComIfGs_setArrowNum(0);
    dComIfGs_setBombNum(0);
    dComIfGs_setArrowMax(0);
    dComIfGs_setBombMax(0);
    dComIfGs_setSelectEquipShield(fpcNm_ITEM_SHIELD);
    dComIfGp_setSelectEquipShield(fpcNm_ITEM_SHIELD);
}
#endif

u32 dComIfG_getTrigA(u32 i_padNo) {
    return mDoCPd_c::getTrigA(i_padNo);
}

struct field_data_header {
    /* 0x00 */ BE(u32) field_0x0;
    /* 0x04 */ BE(u32) field_0x4;
    /* 0x08 */ BE(u32) field_0x8;
    /* 0x0C */ BE(u32) field_0xc;
    /* 0x10 */ BE(u32) field_0x10;
    /* 0x14 */ BE(u32) field_0x14;
};

struct field_data {
    /* 0x00 */ char stage_name[8];
    /* 0x08 */ u8 room_no;
    /* 0x09 */ u8 region_no;
    /* 0x0A */ u8 field_0xa[0x14 - 0xA];
};

void dComIfGp_calcNowRegion() {
    u8 buffer[0x800] ATTRIBUTE_ALIGN(32);

    dComIfGp_getFieldMapArchive2()->readResource(buffer, 0x800, "dat/field.dat");
    u8* entry_num_p = &buffer[((field_data_header*)buffer)->field_0x4];
    u8* entries_p = entry_num_p + 4;

    for (int i = 0; i < 64; i++) {
        dStage_roomControl_c::setRegionNo(i, 0xFF);
    }

    dComIfGs_offPlayerFieldLastStayFieldDataExistFlag();

    if (!dComIfGs_isTmpBit(dSv_event_tmp_flag_c::NO_TELOP)) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP104") != 0 ||
            dComIfG_play_c::getLayerNo(0) != 12)
        {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0 &&
                dComIfGp_getStartStageRoomNo() == 1)
            {
                cXyz pos(dComIfGs_getPlayerFieldLastStayPos());
                pos.x = -90300.0f;
                pos.z = 51800.0f;

                dComIfGs_setPlayerFieldLastStayInfo(dComIfGp_getStartStageName(), pos, 0, 0, 4);
            } else {
                for (int i = 0; i < *entry_num_p; i++) {
                    field_data* data = (field_data*)entries_p;

                    if (strcmp(dMenuFmap_getStartStageName(buffer), data[i].stage_name) == 0) {
                        if (data[i].room_no == 0xFF) {
                            for (int j = 0; j < 64; j++) {
                                if (dStage_roomControl_c::getRegionNo(j) == 0xFF) {
                                    dStage_roomControl_c::setRegionNo(j, data[i].region_no);
                                }
                            }
                        } else {
                            dStage_roomControl_c::setRegionNo(data[i].room_no, data[i].region_no);
                        }

                        dComIfGs_onPlayerFieldLastStayFieldDataExistFlag();
                    }
                }
            }
        }
    }

    if (!dComIfGs_isPlayerFieldLastStayFieldDataExistFlag()) {
        for (int i = 0; i < 64; i++) {
            dStage_roomControl_c::setRegionNo(i, dComIfGs_getPlayerFieldLastStayRegionNo());
        }
    }

    dComIfGp_getFieldMapArchive2()->removeResourceAll();
}

u8 dComIfG_getNowCalcRegion() {
    u8 buffer[0x800] ATTRIBUTE_ALIGN(32);

    dComIfGp_getFieldMapArchive2()->readResource(buffer, 0x800, "dat/field.dat");
    u8* entry_num_p = &buffer[((field_data_header*)buffer)->field_0x4];
    u8* entries_p = entry_num_p + 4;

    bool var_r26 = false;

    u8 sp20[64];
    for (int i = 0; i < 64; i++) {
        sp20[i] = 0xFF;
    }

    for (int i = 0; i < *entry_num_p; i++) {
        field_data* data = (field_data*)entries_p;

        if (strcmp(dMenuFmap_getStartStageName(buffer), data[i].stage_name) == 0) {
            if (data[i].room_no == 0xFF) {
                for (int j = 0; j < 64; j++) {
                    if (sp20[j] == 0xFF) {
                        sp20[j] = data[i].region_no;
                    }
                }
            } else {
                sp20[data[i].room_no] = data[i].region_no;
            }

            var_r26 = true;
        }
    }

    dComIfGp_getFieldMapArchive2()->removeResourceAll();

    if (var_r26) {
        int room_no = -1;

        if (daPy_getLinkPlayerActorClass() != NULL) {
            room_no = fopAcM_GetRoomNo(daPy_getLinkPlayerActorClass());
        }

        if (room_no < 0) {
            room_no = dComIfGp_getStartStageRoomNo();
        }

        return sp20[room_no];
    }

    return 0xFF;
}

bool dComIfGp_isLightDropMapVisible() {
    for (int i = 0; i < 3; i++) {
        if (dComIfGs_isLightDropGetFlag(i) != FALSE && dComIfGs_getLightDropNum(i) < 16) {
            return true;
        }
    }
    return false;
}

u8 dComIfGp_getNowLevel() {
    int roomNo = -1;
    if (daPy_getLinkPlayerActorClass() != NULL) {
        roomNo = fopAcM_GetRoomNo(daPy_getLinkPlayerActorClass());
    }

    if (roomNo < 0) {
        roomNo = dComIfGp_getStartStageRoomNo();
    }

    return dStage_roomControl_c::getRegionNo(roomNo);
}

void dComIfGs_setSelectEquipClothes(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(COLLECT_CLOTHING,
                                                                          i_itemNo);
}

void dComIfGs_setSelectEquipSword(u8 i_itemNo) {
    switch (i_itemNo) {
    case fpcNm_ITEM_SWORD:
        dComIfGs_setCollectSword(COLLECT_ORDON_SWORD);
        break;
    case fpcNm_ITEM_MASTER_SWORD:
        dComIfGs_setCollectSword(COLLECT_MASTER_SWORD);
        break;
    case fpcNm_ITEM_WOOD_STICK:
        dComIfGs_setCollectSword(COLLECT_WOODEN_SWORD);
        break;
    case fpcNm_ITEM_LIGHT_SWORD:
        dComIfGs_setCollectSword(COLLECT_LIGHT_SWORD);
        break;
    }

    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(COLLECT_SWORD, i_itemNo);
}

void dComIfGs_setSelectEquipShield(u8 i_itemNo) {
    switch (i_itemNo) {
    case fpcNm_ITEM_WOOD_SHIELD:
        dComIfGs_setCollectShield(COLLECT_WOODEN_SHIELD);
        break;
    case fpcNm_ITEM_SHIELD:
        dComIfGs_setCollectShield(COLLECT_ORDON_SHIELD);
        break;
    case fpcNm_ITEM_HYLIA_SHIELD:
        dComIfGs_setCollectShield(COLLECT_HYLIAN_SHIELD);
        break;
    }

    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(COLLECT_SHIELD, i_itemNo);
}

void dComIfGs_setKeyNum(int i_stageNo, u8 i_keyNum) {
    if (dComIfGp_getStageStagInfo()) {
        if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
            dComIfGs_setKeyNum(i_keyNum);
        }
    }

    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().setKeyNum(i_keyNum);
}

static void dComIfGs_setWarpItemData(int param_0, char const* i_stage, cXyz i_pos, s16 i_angle, s8 i_roomNo,
                                     u8 param_5, u8 param_6) {
    UNUSED(param_0);
    g_dComIfG_gameInfo.play.setWarpItemData(i_stage, i_pos, i_angle, i_roomNo, param_5, param_6);
}

void dComIfG_play_c::setWarpItemData(char const* i_stage, cXyz i_pos, s16 i_angle, s8 i_roomNo,
                                     u8 param_4, u8 param_5) {
    strcpy(mItemInfo.mWarpItemData.mWarpItemStage, i_stage);
    mItemInfo.mWarpItemData.mWarpItemPos.set(i_pos);
    mItemInfo.mWarpItemData.mWarpItemAngle = i_angle;
    mItemInfo.mWarpItemData.mWarpItemRoom = i_roomNo;
    mItemInfo.mWarpItemData.field_0x4fac = param_5;
    mItemInfo.mWarpItemData.field_0x4fab = param_4;
}

void dComIfGs_setWarpItemData(char const* i_stage, cXyz i_pos, s16 i_angle, s8 i_roomNo, u8 param_4,
                              u8 param_5) {
    int r31 = 0;
    dComIfGs_setWarpItemData(r31, i_stage, i_pos, i_angle, i_roomNo, param_4, param_5);
    dComIfGs_setLastWarpMarkItemData(i_stage, i_pos, i_angle, i_roomNo, param_4, param_5);
}

void dComIfGs_setLastWarpMarkItemData(const char* i_stage, cXyz i_pos, s16 i_angle, s8 i_roomNo,
                                      u8 param_4, u8 param_5) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().setWarpItemData(
        i_stage, i_pos, i_angle, i_roomNo, param_4, param_5);
}

const char* dComIfGs_getWarpStageName() {
    return dComIfGs_getLastWarpMarkStageName();
}

cXyz& dComIfGs_getWarpPlayerPos() {
    return dComIfGs_getLastWarpMarkPlayerPos();
}

s16 dComIfGs_getWarpPlayerAngleY() {
    return dComIfGs_getLastWarpMarkPlayerAngleY();
}

s8 dComIfGs_getWarpRoomNo() {
    return dComIfGs_getLastWarpMarkRoomNo();
}

u8 dComIfGs_getWarpMarkFlag() {
    return dComIfGs_getLastWarpAcceptStage() >= 0 ? 1 : 0;
}

void dComIfGs_setWarpMarkFlag(u8) {}

dComIfG_resLoader_c::dComIfG_resLoader_c() {
    mResNameTable = NULL;
    mPhase.id = cPhs_INIT_e;
    mLoadIndex = 0;
}

dComIfG_resLoader_c::~dComIfG_resLoader_c() {
    if (mResNameTable != NULL) {
        for (int i = mLoadIndex; i >= 0; i--) {
            dComIfG_resDelete(&mPhase, mResNameTable[i]);
            mPhase.id = cPhs_NEXT_e;
        }
    }
}

int dComIfG_resLoader_c::load(char const** i_resNameTbl, JKRHeap* i_heap) {
    mResNameTable = i_resNameTbl;

    int phase_state = dComIfG_resLoad(&mPhase, mResNameTable[mLoadIndex], i_heap);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (mResNameTable[mLoadIndex + 1][0] != 0) {
            mLoadIndex++;
            mPhase.id = cPhs_INIT_e;
            return cPhs_INIT_e;
        }
    }

    return phase_state;
}

void* dComIfG_getStageRes(char const* i_resName) {
    return dComIfG_getStageRes("Stg_00", i_resName);
}

void* dComIfG_getOldStageRes(char const* i_resName) {
    return dComIfG_getStageRes("Xtg_00", i_resName);
}

char* dComIfG_getRoomArcName(int i_roomNo) {
    static char buf[32];
    sprintf(buf, "R%02d_00", i_roomNo);
    return buf;
}

void dComIfGp_ret_wp_set(s8) {}

void dComIfGp_world_dark_set(u8 param_0) {
    g_dComIfG_gameInfo.mWorldDark = param_0;
}

u8 dComIfGp_world_dark_get() {
    return g_dComIfG_gameInfo.mWorldDark;
}

int dComIfG_TimerStart(int i_mode, s16 i_time) {
    if (i_mode == dComIfG_getTimerMode()) {
        dTimer_c* timer = dComIfG_getTimerPtr();
        if (timer != NULL) {
            if (i_time == 0) {
                timer->start(i_mode);
            } else {
                timer->start(i_mode, i_time);
            }
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

int dComIfG_TimerStop(int i_mode) {
    if (i_mode == dComIfG_getTimerMode()) {
        dTimer_c* timer = dComIfG_getTimerPtr();
        if (timer != NULL) {
            timer->stop(4);
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

int dComIfG_TimerReStart(int i_mode) {
    if (i_mode == dComIfG_getTimerMode()) {
        dTimer_c* timer = dComIfG_getTimerPtr();
        if (timer != NULL) {
            timer->restart(4);
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

int dComIfG_TimerEnd(int i_mode, int param_1) {
    if (i_mode == dComIfG_getTimerMode()) {
        dTimer_c* timer = dComIfG_getTimerPtr();
        if (timer != NULL) {
            timer->end(param_1);
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

int dComIfG_TimerDeleteCheck(int i_mode) {
    if (i_mode == dComIfG_getTimerMode()) {
        dTimer_c* timer = dComIfG_getTimerPtr();
        if (timer != NULL) {
            return timer->deleteCheck();
        } else {
            return 0;
        }
    }

    return 0;
}

int dComIfG_TimerDeleteRequest(int i_mode) {
    if (i_mode == dComIfG_getTimerMode()) {
        dTimer_c* timer = dComIfG_getTimerPtr();
        if (timer != NULL) {
            timer->deleteRequest();
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

BOOL dComIfGs_Wolf_Change_Check() {
    BOOL is_wolf = false;

    // Transforming Unlocked
    if (dComIfGs_isEventBit(0x0D04)) {
        is_wolf = dComIfGs_getTransformStatus();
    } else if (dComIfGs_isTransformLV(0) && !dComIfGs_isDarkClearLV(0)) {
        is_wolf = true;
    } else if (dComIfGs_isTransformLV(1) && !dComIfGs_isDarkClearLV(1)) {
        is_wolf = true;
    } else if (dComIfGs_isTransformLV(2) && !dComIfGs_isDarkClearLV(2)) {
        is_wolf = true;
    } else if (dComIfGs_isTransformLV(3) && !dComIfGs_isDarkClearLV(3)) {
        is_wolf = true;
    }

    OS_REPORT("dComIfGs_isSaveSwitch 12[%x] 13[%x]\n", dComIfGs_isSaveSwitch(12), dComIfGs_isSaveSwitch(13));

    // Stage is Hyrule Field and Room is Eldin Gorge Entrance
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") && dComIfGp_getStartStageRoomNo() == 2 &&
        dComIfGp_getStartStagePoint() == 10 && !dComIfGs_isSaveSwitch(12))
    {
        is_wolf = false;
    }
    // Stage is Hyrule Field and Room is North Lanayru - Eldin Entrance
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") &&
             dComIfGp_getStartStageRoomNo() == 9 && dComIfGp_getStartStagePoint() == 10 &&
             !dComIfGs_isSaveSwitch(13))
    {
        is_wolf = false;
    }

    return is_wolf;
}

void dComIfGs_PolyDamageOff_Set(s8 i_setting) {
    g_dComIfG_gameInfo.mPolyDamageOff = i_setting;
}

s8 dComIfGs_PolyDamageOff_Check() {
    return g_dComIfG_gameInfo.mPolyDamageOff;
}

void dComIfGs_shake_kandelaar() {
    g_dComIfG_gameInfo.field_0x1ddfa = 1;
}

int dComIfGs_shake_kandelaar_check() {
    int check = 0;
    if (g_dComIfG_gameInfo.field_0x1ddfa == 2) {
        check = 1;
    }
    return check;
}

BOOL dComIfGs_wolfeye_effect_check() {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    BOOL ret = false;

    if (env_light->now_senses_effect == 1 && env_light->senses_effect_strength > 0.6) {
        ret = true;
    }
    return ret;
}

void dComIfGs_Grass_hide_Set(s8 param_0) {
    g_dComIfG_gameInfo.field_0x1ddfc = param_0;
}

void dComIfGp_TargetWarpPt_set(u8 param_0) {
    g_dComIfG_gameInfo.field_0x1de09 = param_0;
}

void dComIfGp_SelectWarpPt_set(u8 param_0) {
    g_dComIfG_gameInfo.field_0x1de0a = param_0;
}

u8 dComIfGp_TargetWarpPt_get() {
    return g_dComIfG_gameInfo.field_0x1de09;
}

u8 dComIfGp_SelectWarpPt_get() {
    return g_dComIfG_gameInfo.field_0x1de0a;
}

BOOL dComIfGp_TransportWarp_check() {
    BOOL check = false;
    if (g_dComIfG_gameInfo.field_0x1de09 != 0xFF && g_dComIfG_gameInfo.field_0x1de0a != 0xFF &&
        g_dComIfG_gameInfo.field_0x1de09 == g_dComIfG_gameInfo.field_0x1de0a)
    {
        check = true;
    }
    return check;
}

int dComLbG_PhaseHandler(request_of_phase_process_class* i_phaseReq, cPhs__Handler* i_handler,
                         void* i_data) {
    int phase = cPhs_Handler(i_phaseReq, i_handler, i_data);

    switch (phase) {
    case cPhs_NEXT_e:
        phase = dComLbG_PhaseHandler(i_phaseReq, i_handler, i_data);
        break;
    case cPhs_LOADING_e:
    case cPhs_UNK3_e:
        break;
    }

    return phase;
}

void dComIfGs_onVisitedRoom(int i_roomNo) {
    dStage_FileList2_dt_c* fileList = dStage_roomControl_c::getFileList2(i_roomNo);

    if (fileList != NULL && fileList->field_0x13 < 0x40) {
        u8 region = dComIfG_getNowCalcRegion();
        if (region != 0xFF && !dComIfGs_isRegionBit(region)) {
            dComIfGs_onRegionBit(region);
            OS_REPORT("●●●●リージョン%d　到達フラグ立ちました！！！！！●●●●●\n", region);
        }

        JUT_ASSERT(6169, 0 <= dComIfGp_roomControl_getStayNo() && dComIfGp_roomControl_getStayNo() < 64);
        dComIfGs_onSaveVisitedRoom(dStage_roomControl_c::getFileList2(dComIfGp_roomControl_getStayNo())->field_0x13, i_roomNo);
    }
}

void dComIfGs_offVisitedRoom(int i_roomNo) {
    JUT_ASSERT(6181, 0 <= dComIfGp_roomControl_getStayNo() && dComIfGp_roomControl_getStayNo() < 64);

    dComIfGs_offSaveVisitedRoom(dStage_roomControl_c::getFileList2(dComIfGp_roomControl_getStayNo())->field_0x13, i_roomNo);
}

BOOL dComIfGs_isVisitedRoom(int param_0) {
    JUT_ASSERT(6191, 0 <= dComIfGp_roomControl_getStayNo() && dComIfGp_roomControl_getStayNo() < 64);

    return dComIfGs_isSaveVisitedRoom(dStage_roomControl_c::getFileList2(dComIfGp_roomControl_getStayNo())->field_0x13, param_0);
}

void dComIfGs_staffroll_next_go() {
    dScnKy_env_light_c* envLight = dKy_getEnvlight();
    envLight->staffroll_next_timer = true;
}

u8 dComIfGs_staffroll_next_go_check() {
    dScnKy_env_light_c* envLight = dKy_getEnvlight();
    return envLight->staffroll_next_timer;
}

GXColor g_whiteColor = {255, 255, 255, 255};

GXColor g_saftyWhiteColor = {160, 160, 160, 255};
