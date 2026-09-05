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


#if TARGET_PC
#include "dusk/settings.h"
#include "helpers/string.hpp"

#include "m_Do/m_Do_MemCard.h"
#endif

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

    if (dComIfGs_checkGetItem(dItemNo_HAWK_EYE_e)) {
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
    case dItemNo_NORMAL_BOMB_e:
        return mItemInfo.mItemMaxBombNumCount1;
    case dItemNo_WATER_BOMB_e:
        return mItemInfo.mItemMaxBombNumCount2;
    case dItemNo_POKE_BOMB_e:
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
    mParticle = JKR_NEW dPa_control_c();
    JUT_ASSERT(1095, mParticle != NULL);
}

dSmplMdl_draw_c* dComIfG_play_c::createSimpleModel() {
    if (mSimpleModel == NULL) {
        mSimpleModel = JKR_NEW dSmplMdl_draw_c();
    }
    return mSimpleModel;
}

void dComIfG_play_c::deleteSimpleModel() {
    if (mSimpleModel != NULL) {
        JKR_DELETE(mSimpleModel);
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
    cXyz spdc(param_1 - FB_WIDTH_BASE / 2, param_2 - FB_HEIGHT_BASE / 2, 0.0f);

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
    m_navi = JKR_NEW navi_c();
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

#if PLATFORM_WII || PLATFORM_SHIELD
void dComIfG_inf_c::createBaseCsr() {
    JUT_ASSERT(1622, m_baseCsr == NULL);
    m_baseCsr = JKR_NEW baseCsr_c(1);
    JUT_ASSERT(1624, m_baseCsr != NULL);
    m_baseCsr->create();
    mDoGph_gInf_c::entryBaseCsr(m_baseCsr);
}
#endif

DUSK_GAME_DATA GXColor g_clearColor = {0, 0, 0, 0};

DUSK_GAME_DATA GXColor g_blackColor = {0, 0, 0, 255};

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

DUSK_GAME_DATA dComIfG_inf_c g_dComIfG_gameInfo;

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

#if TARGET_PC
    if (!mDoMemCd_isCardCommNone()) {
        return 0;
    }
    g_mDoMemCd_control.SaveSync();
#endif

    dComIfG_changeOpeningScene(i_scene, fpcNm_OPENING_SCENE_e);
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
                             s16 i_angle, f32 param_5, TGXTexObj* i_tex) {
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
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
        return dComIfGs_isTbox(i_no);
    } else {
        return dComIfGs_isSaveTbox(i_stageNo, i_no);
    }
}

void dComIfGs_onStageSwitch(int i_stageNo, int i_no) {
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
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
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
        return dComIfGs_isSwitch(i_no, -1);
    } else {
        return dComIfGs_isSaveSwitch(i_stageNo, i_no);
    }
}

void dComIfGs_onDungeonItemMap(int i_stageNo) {
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
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
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
        return dComIfGs_isDungeonItemMap();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isDungeonItemMap();
}

void dComIfGs_onDungeonItemCompass(int i_stageNo) {
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
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
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
        return dComIfGs_isDungeonItemCompass();
    }

    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isDungeonItemCompass();
}

void dComIfGs_onDungeonItemBossKey(int i_stageNo) {
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
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
#if TARGET_PC
    if (dComIfGp_getStageStagInfo() && i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#else
    if (i_stageNo == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
#endif
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

    if (mix_item_slotNo != dItemNo_NONE_e && mix_item_slotNo >= SLOT_15 && mix_item_slotNo < SLOT_18) {
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
            g_dComIfG_gameInfo.play.setSelectItem(i_selItemIdx, dItemNo_NONE_e);
        }
    } else if (dComIfGs_getSelectItemIndex(i_selItemIdx) != 0xFF) {
        u8 item = dComIfGs_getItem(dComIfGs_getSelectItemIndex(i_selItemIdx), false);
        g_dComIfG_gameInfo.play.setSelectItem(i_selItemIdx, item);

        if (item == dItemNo_NONE_e) {
            dComIfGs_setSelectItemIndex(i_selItemIdx, 0xFF);
        }
    } else {
        g_dComIfG_gameInfo.play.setSelectItem(i_selItemIdx, dItemNo_NONE_e);
    }
}

u8 dComIfGp_getSelectItem(int i_selItemIdx) {
    u8 playItem = g_dComIfG_gameInfo.play.getSelectItem(i_selItemIdx);

    if ((i_selItemIdx == SELECT_ITEM_X || i_selItemIdx == SELECT_ITEM_Y) &&
        dComIfGs_getMixItemIndex(i_selItemIdx) != 0xFF)
    {
        u8 saveItem = dComIfGs_getItem(dComIfGs_getMixItemIndex(i_selItemIdx), false);

        if (saveItem == dItemNo_BOW_e) {
            saveItem = playItem;
            playItem = dItemNo_BOW_e;
        } else if (saveItem == dItemNo_FISHING_ROD_1_e) {
            saveItem = playItem;
            playItem = dItemNo_FISHING_ROD_1_e;
        }

        if (playItem == dItemNo_BOW_e) {
            switch (saveItem) {
            case dItemNo_NORMAL_BOMB_e:
            case dItemNo_WATER_BOMB_e:
            case dItemNo_POKE_BOMB_e:
                playItem = dItemNo_BOMB_ARROW_e;
                break;
            case dItemNo_HAWK_EYE_e:
                playItem = dItemNo_HAWK_ARROW_e;
                break;
            }
        } else if (playItem == dItemNo_FISHING_ROD_1_e) {
            switch (saveItem) {
            case dItemNo_BEE_CHILD_e:
                playItem = dItemNo_BEE_ROD_e;
                break;
            case dItemNo_WORM_e:
                playItem = dItemNo_WORM_ROD_e;
                break;
            case dItemNo_ZORAS_JEWEL_e:
                playItem = dItemNo_JEWEL_ROD_e;
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

            #if TARGET_PC
            if (i_heap != NULL) {
                JKRHEAP_NAMEF(i_heap, "SubExpHeap2D %d", i);
            }
            #endif
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
        dItemNo_M_BEETLE_e,      dItemNo_F_BEETLE_e,      dItemNo_M_BUTTERFLY_e, dItemNo_F_BUTTERFLY_e, dItemNo_M_STAG_BEETLE_e, dItemNo_F_STAG_BEETLE_e,
        dItemNo_M_GRASSHOPPER_e, dItemNo_F_GRASSHOPPER_e, dItemNo_M_NANAFUSHI_e, dItemNo_F_NANAFUSHI_e, dItemNo_M_DANGOMUSHI_e,  dItemNo_F_DANGOMUSHI_e,
        dItemNo_M_MANTIS_e,      dItemNo_F_MANTIS_e,      dItemNo_M_LADYBUG_e,   dItemNo_F_LADYBUG_e,   dItemNo_M_SNAIL_e,       dItemNo_F_SNAIL_e,
        dItemNo_M_DRAGONFLY_e,   dItemNo_F_DRAGONFLY_e,   dItemNo_M_ANT_e,       dItemNo_F_ANT_e,       dItemNo_M_MAYFLY_e,      dItemNo_F_MAYFLY_e,
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

    if (selectItem == dItemNo_NORMAL_BOMB_e || selectItem == dItemNo_WATER_BOMB_e || selectItem == dItemNo_POKE_BOMB_e ||
        selectItem == dItemNo_BOMB_ARROW_e)
    {
        u8 slot_no = dComIfGs_getSelectMixItemNoArrowIndex(i_selItemIdx) - SLOT_15;
        itemNum = dComIfGs_getBombNum(slot_no);
    } else if (selectItem == dItemNo_PACHINKO_e) {
        itemNum = dComIfGs_getPachinkoNum();
    } else if (selectItem == dItemNo_BEE_CHILD_e) {
        u8 slot_no = dComIfGs_getSelectItemIndex(i_selItemIdx) - SLOT_11;
        itemNum = dComIfGs_getBottleNum(slot_no);
    }

    return itemNum;
}

int dComIfGp_getSelectItemMaxNum(int i_selItemIdx) {
    u8 selectItem = dComIfGp_getSelectItem(i_selItemIdx);
    int itemNum = 0;

    if (selectItem == dItemNo_BOMB_BAG_LV1_e) {
        itemNum = 1;
    } else if (selectItem == dItemNo_NORMAL_BOMB_e || selectItem == dItemNo_WATER_BOMB_e || selectItem == dItemNo_POKE_BOMB_e ||
               selectItem == dItemNo_BOMB_ARROW_e)
    {
        itemNum = dComIfGs_getBombMax(selectItem);
    } else if (selectItem == dItemNo_PACHINKO_e) {
        itemNum = dComIfGs_getPachinkoMax();
    } else if (selectItem == dItemNo_BEE_CHILD_e) {
        itemNum = dComIfGs_getBottleMax();
    }

    return itemNum;
}

void dComIfGp_setSelectItemNum(int i_selItemIdx, s16 i_num) {
    u8 selectItem = dComIfGp_getSelectItem(i_selItemIdx);

    if (selectItem == dItemNo_NORMAL_BOMB_e || selectItem == dItemNo_WATER_BOMB_e || selectItem == dItemNo_POKE_BOMB_e ||
        selectItem == dItemNo_BOMB_ARROW_e)
    {
        u8 mix_slotNo = dComIfGs_getSelectMixItemNoArrowIndex(i_selItemIdx) - SLOT_15;

        if (i_num > dComIfGs_getBombMax(selectItem)) {
            i_num = dComIfGs_getBombMax(selectItem);
        }
        dComIfGs_setBombNum(mix_slotNo, i_num);
    } else if (selectItem == dItemNo_PACHINKO_e) {
        dComIfGs_setPachinkoNum(i_num);
    } else if (selectItem == dItemNo_BEE_CHILD_e) {
        u8 bottle_slot_no = dComIfGs_getSelectItemIndex(i_selItemIdx) - SLOT_11;

        if (i_num > dComIfGs_getBottleMax()) {
            i_num = dComIfGs_getBottleMax();
        }
        dComIfGs_setBottleNum(bottle_slot_no, i_num);
    }
}

void dComIfGp_addSelectItemNum(int i_selItemIdx, s16 i_num) {
    u8 selectItem = dComIfGp_getSelectItem(i_selItemIdx);

    if (selectItem == dItemNo_NORMAL_BOMB_e || selectItem == dItemNo_WATER_BOMB_e || selectItem == dItemNo_POKE_BOMB_e ||
        selectItem == dItemNo_BOMB_ARROW_e)
    {
        u8 slot_no = dComIfGs_getSelectMixItemNoArrowIndex(i_selItemIdx) - SLOT_15;
        dComIfGp_setItemBombNumCount(slot_no, i_num);
    } else if (selectItem == dItemNo_PACHINKO_e) {
        dComIfGp_setItemPachinkoNumCount(i_num);
    } else if (selectItem == dItemNo_BEE_CHILD_e) {
        u8 slot_no = dComIfGs_getSelectItemIndex(i_selItemIdx) - SLOT_11;
        dComIfGs_addBottleNum(slot_no, i_num);
    }
}

int dComIfGd_setShadow(u32 param_0, s8 param_1, J3DModel* param_2, cXyz* param_3, f32 param_4,
                       f32 param_5, f32 param_6, f32 param_7, cBgS_PolyInfo& param_8,
                       dKy_tevstr_c* param_9, s16 param_10, f32 param_11, TGXTexObj* param_12) {
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

    dComIfGs_offItemFirstBit(dItemNo_L2_KEY_PIECES1_e);
    dComIfGs_offItemFirstBit(dItemNo_L2_KEY_PIECES2_e);
    dComIfGs_offItemFirstBit(dItemNo_L2_KEY_PIECES3_e);
    dComIfGs_offItemFirstBit(dItemNo_LV2_BOSS_KEY_e);
    dComIfGs_offItemFirstBit(dItemNo_BOMB_BAG_LV2_e);
    dComIfGs_offItemFirstBit(dItemNo_TOMATO_PUREE_e);
    dComIfGs_offItemFirstBit(dItemNo_TASTE_e);
    dComIfGs_offItemFirstBit(dItemNo_POU_FIRE1_e);
    dComIfGs_offItemFirstBit(dItemNo_POU_FIRE2_e);
    dComIfGs_offItemFirstBit(dItemNo_POU_FIRE3_e);
    dComIfGs_offItemFirstBit(dItemNo_POU_FIRE4_e);

    for (int i = 0; i < 24; i++) {
        dComIfGs_offItemFirstBit(i + dItemNo_M_BEETLE_e);
    }

    dComIfGs_offItemFirstBit(dItemNo_LIGHT_SWORD_e);
    dComIfGs_offItemFirstBit(dItemNo_SHIELD_e);
    dComIfGs_offItemFirstBit(dItemNo_ZORAS_JEWEL_e);

    for (int i = 0; i < 19; i++) {
        dComIfGs_offItemFirstBit(i);
    }

    dComIfGs_setCollectSmell(dItemNo_SMELL_PUMPKIN_e);

    if (!mDoCPd_c::isConnect(PAD_3)) {
        dComIfGs_offItemFirstBit(dItemNo_SMELL_POH_e);
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
    dComIfGs_setBombMax(dItemNo_NORMAL_BOMB_e, 30);
    dComIfGs_setBombMax(dItemNo_WATER_BOMB_e, 15);
    dComIfGs_setBombMax(dItemNo_POKE_BOMB_e, 10);

    dMeter2Info_setCloth(dItemNo_WEAR_KOKIRI_e, false);
    dMeter2Info_setSword(dItemNo_SWORD_e, false);
    dMeter2Info_setShield(dItemNo_HYLIA_SHIELD_e, false);
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

    dComIfGs_setSelectEquipClothes(dItemNo_WEAR_CASUAL_e);
    dComIfGp_setSelectEquipClothes(dItemNo_WEAR_CASUAL_e);
    dComIfGs_setSelectEquipSword(dItemNo_NONE_e);
    dComIfGp_setSelectEquipSword(dItemNo_NONE_e);
    dComIfGs_setSelectEquipShield(dItemNo_NONE_e);
    dComIfGp_setSelectEquipShield(dItemNo_NONE_e);

    for (int i = 0; i < 24; i++) {
        dComIfGs_setItem(i, dItemNo_NONE_e);
    }

    for (int i = 0; i < 0x100; i++) {
        dComIfGs_offItemFirstBit(i);
    }

    dComIfGs_setArrowNum(0);
    dComIfGs_setBombNum(0);
    dComIfGs_setArrowMax(0);
    dComIfGs_setBombMax(0);
    dComIfGs_setSelectEquipShield(dItemNo_SHIELD_e);
    dComIfGp_setSelectEquipShield(dItemNo_SHIELD_e);
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
    ATTRIBUTE_ALIGN(32) u8 buffer[0x800];

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
    ATTRIBUTE_ALIGN(32) u8 buffer[0x800];

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
    case dItemNo_SWORD_e:
        dComIfGs_setCollectSword(COLLECT_ORDON_SWORD);
        break;
    case dItemNo_MASTER_SWORD_e:
        dComIfGs_setCollectSword(COLLECT_MASTER_SWORD);
        break;
    case dItemNo_WOOD_STICK_e:
        dComIfGs_setCollectSword(COLLECT_WOODEN_SWORD);
        break;
    case dItemNo_LIGHT_SWORD_e:
        dComIfGs_setCollectSword(COLLECT_LIGHT_SWORD);
        break;
    }

    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(COLLECT_SWORD, i_itemNo);
}

void dComIfGs_setSelectEquipShield(u8 i_itemNo) {
    switch (i_itemNo) {
    case dItemNo_WOOD_SHIELD_e:
        dComIfGs_setCollectShield(COLLECT_WOODEN_SHIELD);
        break;
    case dItemNo_SHIELD_e:
        dComIfGs_setCollectShield(COLLECT_ORDON_SHIELD);
        break;
    case dItemNo_HYLIA_SHIELD_e:
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
    SAFE_STRCPY(mItemInfo.mWarpItemData.mWarpItemStage, i_stage);
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

#if TARGET_PC
cXyz dComIfGs_getWarpPlayerPos() {
#else
cXyz& dComIfGs_getWarpPlayerPos() {
#endif
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
    SAFE_SPRINTF(buf, "R%02d_00", i_roomNo);
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

DUSK_GAME_DATA GXColor g_whiteColor = {255, 255, 255, 255};

DUSK_GAME_DATA GXColor g_saftyWhiteColor = {160, 160, 160, 255};

#if TARGET_PC
dSv_info_c* dComIfGs_getSaveInfo() {
    return &g_dComIfG_gameInfo.info;
}

dSv_save_c* dComIfGs_getSaveData() {
    return &g_dComIfG_gameInfo.info.getSavedata();
}

u16 dComIfGs_getMaxLife() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMaxLife();
}

void dComIfGs_setMaxLife(u8 i_maxLife) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMaxLife(i_maxLife);
}

u16 dComIfGs_getLife() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getLife();
}

void dComIfGs_setLife(u16 i_life) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setLife(i_life);
}

u16 dComIfGs_getRupee() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getRupee();
}

void dComIfGs_setRupee(u16 i_rupees) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setRupee(i_rupees);
}

u16 dComIfGs_getMaxOil() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMaxOil();
}

void dComIfGs_setMaxOil(u16 i_maxOil) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMaxOil(i_maxOil);
}

u16 dComIfGs_getOil() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getOil();
}

void dComIfGs_setOil(u16 i_oil) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setOil(i_oil);
}

u8 dComIfGs_getSelectEquipClothes() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_CLOTHING);
}

u8 dComIfGs_getSelectEquipSword() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_SWORD);
}

u8 dComIfGs_getSelectEquipShield() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_SHIELD);
}

u8 dComIfGs_getCollectSmell() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_SMELL);
}

void dComIfGs_setCollectSmell(u8 smell) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(COLLECT_SMELL, smell);
}

u8 dComIfGs_getBButtonItemKey() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(B_BUTTON_ITEM);
}

void dComIfGs_setBButtonItemKey(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(B_BUTTON_ITEM, i_itemNo);
}

u8 dComIfGs_getWalletSize() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getWalletSize();
}

void dComIfGs_setWalletSize(u8 i_size) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setWalletSize(i_size);
}

u8 dComIfGs_getMaxMagic() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMaxMagic();
}

void dComIfGs_setMaxMagic(u8 i_maxMagic) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMaxMagic(i_maxMagic);
}

u8 dComIfGs_getMagic() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMagic();
}

void dComIfGs_setMagic(u8 i_magic) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMagic(i_magic);
}

u8 dComIfGs_getTransformStatus() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getTransformStatus();
}

void dComIfGs_setTransformStatus(u8 i_status) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setTransformStatus(i_status);
}

u8 dComIfGs_getSelectItemIndex(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectItemIndex(i_no);
}

u16 dComIfGs_getRupeeMax() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getRupeeMax();
}

void dComIfGs_offGetMagicUseFlag() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().offMagicFlag(0);
}

s32 dComIfGs_isGetMagicUseFlag() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().isMagicFlag(0);
}

f32 dComIfGs_getTime() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().getTime();
}

void dComIfGs_setTime(f32 i_time) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().setTime(i_time);
}

u16 dComIfGs_getDate() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().getDate();
}

void dComIfGs_setDate(u16 i_date) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().setDate(i_date);
}

void dComIfGs_onDarkClearLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().onDarkClearLV(i_no);
}

void dComIfGs_offDarkClearLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().offDarkClearLV(i_no);
}

BOOL dComIfGs_isDarkClearLV(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().isDarkClearLV(i_no);
}

void dComIfGs_onTransformLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().onTransformLV(i_no);
}

void dComIfGs_offTransformLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().offTransformLV(i_no);
}

BOOL dComIfGs_isTransformLV(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().isTransformLV(i_no);
}

cXyz dComIfGs_getHorseRestartPos() {
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getPos();
}

s16 dComIfGs_getHorseRestartAngleY() {
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getAngleY();
}

const char* dComIfGs_getHorseRestartStageName() {
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getStageName();
}

s8 dComIfGs_getHorseRestartRoomNo() {
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getRoomNo();
}

void dComIfGs_setHorseRestart(const char* i_stageName, cXyz& i_pos, s16 i_angle, s8 i_roomNo) {
    g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().set(i_stageName, i_pos, i_angle, i_roomNo);
}

cXyz dComIfGs_getPlayerFieldLastStayPos() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getPos();
}

s16 dComIfGs_getPlayerFieldLastStayAngleY() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getAngleY();
}

char* dComIfGs_getPlayerFieldLastStayName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getName();
}

u8 dComIfGs_getPlayerFieldLastStayRegionNo() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getRegionNo();
}

bool dComIfGs_isPlayerFieldLastStayFieldDataExistFlag() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().isFieldDataExistFlag();
}

void dComIfGs_offPlayerFieldLastStayFieldDataExistFlag() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().offFieldDataExistFlag();
}

void dComIfGs_onPlayerFieldLastStayFieldDataExistFlag() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().onFieldDataExistFlag();
}

BOOL dComIfGs_isRegionBit(int i_region) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().isRegionBit(i_region);
}

void dComIfGs_onRegionBit(int i_region) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().onRegionBit(i_region);
}

void dComIfGs_setPlayerFieldLastStayInfo(
    const char* i_stage, cXyz& i_pos, s16 i_angle, s8 i_point, u8 i_region) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().set(
        i_stage, i_pos, i_angle, i_point, i_region);
}

cXyz dComIfGs_getLastWarpMarkPlayerPos() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getPos();
}

s16 dComIfGs_getLastWarpMarkPlayerAngleY() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getAngleY();
}

const char* dComIfGs_getLastWarpMarkStageName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getName();
}

u8 dComIfGs_getLastWarpMarkRoomNo() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getRoomNo();
}

s8 dComIfGs_getLastWarpAcceptStage() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getWarpAcceptStage();
}

void dComIfGs_setLastWarpAcceptStage(s8 param_0) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().setWarpAcceptStage(param_0);
}

void dComIfGs_resetLastWarpAcceptStage() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().resetWarpAcceptStage();
}

void dComIfGs_setItem(int i_slotNo, u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setItem(i_slotNo, i_itemNo);
}

u8 dComIfGs_getItem(int i_slotNo, bool i_checkCombo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().getItem(i_slotNo, i_checkCombo);
}

void dComIfGs_setLineUpItem() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setLineUpItem();
}

u8 dComIfGs_getLineUpItem(int i_slotNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().getLineUpItem(i_slotNo);
}

void dComIfGs_setBottleItemIn(u8 curItem, u8 newItem) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setBottleItemIn(curItem, newItem);
}

void dComIfGs_setEmptyBottleItemIn(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBottleItemIn(i_itemNo);
}

void dComIfGs_setEmptyBottle() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBottle();
}

void dComIfGs_setEmptyBottle(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBottle(i_itemNo);
}

void dComIfGs_setEquipBottleItemIn(u8 i_curItem, u8 i_newItem) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEquipBottleItemIn(i_curItem, i_newItem);
}

void dComIfGs_setEquipBottleItemEmpty(u8 i_curItem) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEquipBottleItemEmpty(i_curItem);
}

u8 dComIfGs_checkBottle(u8 i_itemNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkBottle(i_itemNo);
}

u8 dComIfGs_checkInsectBottle() {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkInsectBottle();
}

u8 dComIfGs_checkEmptyBottle() {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkEmptyBottle();
}

void dComIfGs_setEmptyBombBagItemIn(u8 i_newBomb, bool i_setNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBagItemIn(i_newBomb, i_setNum);
}

void dComIfGs_setEmptyBombBagItemIn(u8 i_newBomb, u8 i_bombNum, bool i_setNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBagItemIn(
        i_newBomb, i_bombNum, i_setNum);
}

void dComIfGs_setEmptyBombBag() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBag();
}

void dComIfGs_setEmptyBombBag(u8 i_newBomb, u8 i_bombNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBag(i_newBomb, i_bombNum);
}

u8 dComIfGs_checkBombBag(u8 i_itemNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkBombBag(i_itemNo);
}

void dComIfGs_setWarashibeItem(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setWarashibeItem(i_itemNo);
}

void dComIfGs_setRodTypeLevelUp() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setRodTypeLevelUp();
}

void dComIfGs_setBaitItem(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setBaitItem(i_item);
}

void dComIfGs_onItemFirstBit(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getGetItem().onFirstBit(i_itemNo);
}

void dComIfGs_offItemFirstBit(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getGetItem().offFirstBit(i_itemNo);
}

int dComIfGs_isItemFirstBit(u8 i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getGetItem().isFirstBit(i_no);
}

u8 dComIfGs_getArrowNum() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getArrowNum();
}

void dComIfGs_setArrowNum(u8 i_arrowNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setArrowNum(i_arrowNum);
}

u8 dComIfGs_getPachinkoNum() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getPachinkoNum();
}

void dComIfGs_setPachinkoNum(u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setPachinkoNum(i_num);
}

u8 dComIfGs_getPachinkoMax() {
    return 50;
}

void dComIfGs_setBombNum(u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setBombNum(0, i_num);
}

void dComIfGs_setBombNum(u8 i_bagIdx, u8 i_bombNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setBombNum(i_bagIdx, i_bombNum);
}

u8 dComIfGs_getBombNum(u8 i_bagIdx) {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getBombNum(i_bagIdx);
}

void dComIfGs_setBottleNum(u8 i_bottleIdx, u8 i_bottleNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setBottleNum(i_bottleIdx, i_bottleNum);
}

void dComIfGs_addBottleNum(u8 i_bottleIdx, s16 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().addBottleNum(i_bottleIdx, i_num);
}

u8 dComIfGs_getBottleNum(u8 i_bottleIdx) {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getBottleNum(i_bottleIdx);
}

u8 dComIfGs_getArrowMax() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemMax().getArrowNum();
}

void dComIfGs_setArrowMax(u8 i_arrowMax) {
    g_dComIfG_gameInfo.info.getPlayer().getItemMax().setArrowNum(i_arrowMax);
}

void dComIfGs_setBombMax(u8 i_max) {
    g_dComIfG_gameInfo.info.getPlayer().getItemMax().setBombNum(0, i_max);
}

void dComIfGs_setBombMax(u8 i_type, u8 i_max) {
    g_dComIfG_gameInfo.info.getPlayer().getItemMax().setBombNum(i_type, i_max);
}

u8 dComIfGs_getBombMax() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemMax().getBombNum(0);
}

u8 dComIfGs_getBombMax(u8 i_bombType) {
    return g_dComIfG_gameInfo.info.getPlayer().getItemMax().getBombNum(i_bombType);
}

void dComIfGs_setPohSpiritNum(u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setPohNum(i_num);
}

u8 dComIfGs_getPohSpiritNum() {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().getPohNum();
}

void dComIfGs_addPohSpiritNum() {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().addPohNum();
}

BOOL dComIfGs_isCollectClothes(u8 i_clothesNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(
        COLLECT_CLOTHING, i_clothesNo);
}

void dComIfGs_setCollectClothes(u8 i_clothesNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setCollect(COLLECT_CLOTHING, i_clothesNo);
}

void dComIfGs_setCollectSword(u8 i_swordNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setCollect(COLLECT_SWORD, i_swordNo);
}

void dComIfGs_setCollectShield(u8 i_shieldNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setCollect(COLLECT_SHIELD, i_shieldNo);
}

BOOL dComIfGs_isCollectClothing(u8 i_clothesNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(
        COLLECT_CLOTHING, i_clothesNo);
}

void dComIfGs_offCollectClothes(u8 i_clothesNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollect(COLLECT_CLOTHING, i_clothesNo);
}

BOOL dComIfGs_isCollectSword(u8 i_swordNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(COLLECT_SWORD, i_swordNo);
}

void dComIfGs_offCollectSword(u8 i_swordNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollect(COLLECT_SWORD, i_swordNo);
}

BOOL dComIfGs_isCollectShield(u8 i_item) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(COLLECT_SHIELD, i_item);
}

void dComIfGs_offCollectShield(u8 i_shieldNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollect(COLLECT_SHIELD, i_shieldNo);
}

void dComIfGs_onCollectCrystal(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().onCollectCrystal(i_item);
}

void dComIfGs_offCollectCrystal(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollectCrystal(i_item);
}

bool dComIfGs_isCollectCrystal(u8 i_item) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollectCrystal(i_item);
}

void dComIfGs_onCollectMirror(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().onCollectMirror(i_item);
}

void dComIfGs_offCollectMirror(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollectMirror(i_item);
}

bool dComIfGs_isCollectMirror(u8 i_item) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollectMirror(i_item);
}

void dComIfGs_setLightDropNum(u8 i_level, u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getLightDrop().setLightDropNum(i_level, i_num);
}

u8 dComIfGs_getLightDropNum(u8 i_nowLevel) {
    return g_dComIfG_gameInfo.info.getPlayer().getLightDrop().getLightDropNum(i_nowLevel);
}

void dComIfGs_onLightDropGetFlag(u8 i_nowLevel) {
    g_dComIfG_gameInfo.info.getPlayer().getLightDrop().onLightDropGetFlag(i_nowLevel);
}

void dComIfGs_offLightDropGetFlag(u8 i_level) {
    g_dComIfG_gameInfo.info.getPlayer().getLightDrop().offLightDropGetFlag(i_level);
}

BOOL dComIfGs_isLightDropGetFlag(u8 i_nowLevel) {
    return g_dComIfG_gameInfo.info.getPlayer().getLightDrop().isLightDropGetFlag(i_nowLevel);
}

void dComIfGs_onLetterGetFlag(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().onLetterGetFlag(i_no);
}

BOOL dComIfGs_isLetterGetFlag(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().isLetterGetFlag(i_no);
}

void dComIfGs_onLetterReadFlag(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().onLetterReadFlag(i_no);
}

int dComIfGs_isLetterReadFlag(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().isLetterReadFlag(i_no);
}

u8 dComIfGs_getGetNumber(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().getGetNumber(i_no);
}

void dComIfGs_setGetNumber(int i_no, u8 i_value) {
    g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().setGetNumber(i_no, i_value);
}

void dComIfGs_addFishNum(u8 param_0) {
    g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().addFishCount(param_0);
}

u16 dComIfGs_getFishNum(u8 param_0) {
    return g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().getFishCount(param_0);
}

u8 dComIfGs_getFishSize(u8 param_0) {
    return g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().getMaxSize(param_0);
}

void dComIfGs_setFishSize(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().setMaxSize(param_0, param_1);
}

OSTime dComIfGs_getTotalTime() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getTotalTime();
}

void dComIfGs_addDeathCount() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().addDeathCount();
}

u16 dComIfGs_getDeathCount() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getDeathCount();
}

TEXT_SPAN dComIfGs_getPlayerName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getPlayerName();
}

void dComIfGs_setPlayerName(const char* i_name) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().setPlayerName(i_name);
}

TEXT_SPAN dComIfGs_getHorseName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getHorseName();
}

void dComIfGs_setHorseName(const char* i_name) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().setHorseName(i_name);
}

u8 dComIfGs_getClearCount() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getClearCount();
}

// Ruby inline names are from TWW debug.
u8 dComIfGs_getOptRuby() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getRuby();
}

void dComIfGs_setOptRuby(u8 i_ruby) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setRuby(i_ruby);
}

u8 dComIfGs_getOptAttentionType() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getAttentionType();
}

void dComIfGs_setOptAttentionType(u8 i_attentionType) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setAttentionType(i_attentionType);
}

void dComIfGs_setOptCalibrateDist(u16 i_calibrateDist) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setCalibrateDist(i_calibrateDist);
}

void dComIfGs_setOptCalValue(s8 i_calValue) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setCalValue(i_calValue);
}

u8 dComIfGs_getOptCameraControl() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getCameraControl();
}

void dComIfGs_setOptCameraControl(u8 i_cameraControl) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setCameraControl(i_cameraControl);
}

u8 dComIfGs_getOptPointer() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getPointer();
}

void dComIfGs_setOptPointer(u8 i_pointer) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setPointer(i_pointer);
}

u8 dComIfGs_checkOptVibration() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().checkVibration();
}

u8 dComIfGs_getOptSound() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getSound();
}

void dComIfGs_setOptSound(u8 i_mode) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setSound(i_mode);
}

u8 dComIfGs_getOptVibration() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getVibration();
}

void dComIfGs_setOptVibration(u8 i_status) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setVibration(i_status);
}

u8 dComIfGs_getPalLanguage() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getPalLanguage();
}

BOOL dComIfGs_isSaveTbox(int i_stageNo, int i_no) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isTbox(i_no);
}

void dComIfGs_onSaveSwitch(int i_stageNo, int i_no) {
    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onSwitch(i_no);
}

void dComIfGs_offSaveSwitch(int i_stageNo, int i_no) {
    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offSwitch(i_no);
}

BOOL dComIfGs_isSaveSwitch(int i_stageNo, int i_no) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isSwitch(i_no);
}

void dComIfGs_onSaveVisitedRoom(int param_0, int i_roomNo) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave2(param_0)->onVisitedRoom(i_roomNo);
}

void dComIfGs_offSaveVisitedRoom(int param_0, int i_roomNo) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave2(param_0)->offVisitedRoom(i_roomNo);
}

BOOL dComIfGs_isSaveVisitedRoom(int param_0, int i_roomNo) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave2(param_0)->isVisitedRoom(i_roomNo);
}

void* dComIfGs_getPEventBit() {
    return g_dComIfG_gameInfo.info.getEvent().getPEventBit();
}

void dComIfGs_onEventBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getEvent().onEventBit(i_flag);
}

void dComIfGs_offEventBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getEvent().offEventBit(i_flag);
}

BOOL dComIfGs_isEventBit(const u16 i_flag) {
    return g_dComIfG_gameInfo.info.getEvent().isEventBit(i_flag);
}

void dComIfGs_setEventReg(u16 reg, u8 value) {
    g_dComIfG_gameInfo.info.getEvent().setEventReg(reg, value);
}

u8 dComIfGs_getEventReg(u16 reg) {
    return g_dComIfG_gameInfo.info.getEvent().getEventReg(reg);
}

int dComIfGs_getHookGameTime() {
    return g_dComIfG_gameInfo.info.getMiniGame().getHookGameTime();
}

void dComIfGs_setHookGameTime(u32 i_time) {
    g_dComIfG_gameInfo.info.getMiniGame().setHookGameTime(i_time);
}

u32 dComIfGs_getBalloonScore() {
    return g_dComIfG_gameInfo.info.getMiniGame().getBalloonScore();
}

void dComIfGs_setBalloonScore(u32 i_score) {
    g_dComIfG_gameInfo.info.getMiniGame().setBalloonScore(i_score);
}

int dComIfGs_getRaceGameTime() {
    return g_dComIfG_gameInfo.info.getMiniGame().getRaceGameTime();
}

void dComIfGs_setRaceGameTime(int i_time) {
    g_dComIfG_gameInfo.info.getMiniGame().setRaceGameTime(i_time);
}

u8 dComIfGs_getKeyNum() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().getKeyNum();
}

void dComIfGs_setKeyNum(u8 i_keyNum) {
    g_dComIfG_gameInfo.info.getMemory().getBit().setKeyNum(i_keyNum);
}

void dComIfGs_onDungeonItemMap() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemMap();
}

void dComIfGs_offDungeonItemMap() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemMap();
}

s32 dComIfGs_isDungeonItemMap() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemMap();
}

void dComIfGs_onDungeonItemCompass() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemCompass();
}

void dComIfGs_offDungeonItemCompass() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemCompass();
}

s32 dComIfGs_isDungeonItemCompass() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemCompass();
}

void dComIfGs_onDungeonItemBossKey() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemBossKey();
}

void dComIfGs_offDungeonItemBossKey() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemBossKey();
}

s32 dComIfGs_isDungeonItemBossKey() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemBossKey();
}

void dComIfGs_onStageBossEnemy() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageBossEnemy();
}

void dComIfGs_offStageBossEnemy() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageBossEnemy();
}

s32 dComIfGs_isStageBossEnemy() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageBossEnemy();
}

void dComIfGs_onStageLife() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageLife();
}

void dComIfGs_offStageLife() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageLife();
}

s32 dComIfGs_isStageLife() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageLife();
}

void dComIfGs_onStageBossDemo() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageBossDemo();
}

void dComIfGs_offStageBossDemo() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageBossDemo();
}

s32 dComIfGs_isStageBossDemo() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageBossDemo();
}

void dComIfGs_onDungeonItemWarp() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemWarp();
}

void dComIfGs_offDungeonItemWarp() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemWarp();
}

s32 dComIfGs_isDungeonItemWarp() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemWarp();
}

void dComIfGs_onStageMiddleBoss() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageBossEnemy2();
}

void dComIfGs_offStageMiddleBoss() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageBossEnemy2();
}

BOOL dComIfGs_isStageMiddleBoss() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageBossEnemy2();
}

void dComIfGs_onTbox(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().onTbox(i_no);
}

void dComIfGs_offTbox(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().offTbox(i_no);
}

BOOL dComIfGs_isTbox(int i_no) {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isTbox(i_no);
}

void dComIfGs_onSaveSwitch(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().onSwitch(i_no);
}

void dComIfGs_offSaveSwitch(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().offSwitch(i_no);
}

BOOL dComIfGs_isSaveSwitch(int i_no) {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isSwitch(i_no);
}

BOOL dComIfGs_isSaveItem(int i_no) {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isItem(i_no);
}

void dComIfGs_onSaveDunSwitch(int i_swNo) {
    g_dComIfG_gameInfo.info.getDan().onSwitch(i_swNo);
}

void dComIfGs_offSaveDunSwitch(int i_swNo) {
    g_dComIfG_gameInfo.info.getDan().offSwitch(i_swNo);
}

BOOL dComIfGs_isSaveDunSwitch(int i_no) {
    return g_dComIfG_gameInfo.info.getDan().isSwitch(i_no);
}

BOOL dComIfGs_isSaveDunItem(int i_no) {
    return g_dComIfG_gameInfo.info.getDan().isItem(i_no);
}

void dComIfGs_resetDan() {
    g_dComIfG_gameInfo.info.resetDan();
}

void dComIfGs_initDan(s8 i_stageNo) {
    g_dComIfG_gameInfo.info.initDan(i_stageNo);
}

void dComIfGs_clearRoomSwitch(int i_zoneNo) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().clearRoomSwitch();
}

void dComIfGs_clearRoomItem(int i_zoneNo) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().clearRoomItem();
}

void dComIfGs_onSvZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onSwitch(i_swBit);
}

void dComIfGs_offSvZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offSwitch(i_swBit);
}

BOOL dComIfGs_isSvZoneSwitch(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isSwitch(i_swBit);
}

void dComIfGs_onSvOneZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onOneSwitch(i_swBit);
}

void dComIfGs_offSvOneZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offOneSwitch(i_swBit);
}

BOOL dComIfGs_isSvOneZoneSwitch(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isOneSwitch(i_swBit);
}

void dComIfGs_onSvZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onItem(i_swBit);
}

void dComIfGs_offSvZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offItem(i_swBit);
}

BOOL dComIfGs_isSvZoneItem(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isItem(i_swBit);
}

void dComIfGs_onSvOneZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onOneItem(i_swBit);
}

void dComIfGs_offSvOneZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offOneItem(i_swBit);
}

BOOL dComIfGs_isSvOneZoneItem(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isOneItem(i_swBit);
}

void dComIfGs_removeZone(int i_zoneNo) {
    g_dComIfG_gameInfo.info.removeZone(i_zoneNo);
}

s8 dComIfGs_getRestartRoomNo() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomNo();
}

s16 dComIfGs_getStartPoint() {
    return g_dComIfG_gameInfo.info.getRestart().getStartPoint();
}

void dComIfGs_setStartPoint(s16 i_point) {
    g_dComIfG_gameInfo.info.getRestart().setStartPoint(i_point);
}

s16 dComIfGs_getRestartRoomAngleY() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomAngleY();
}

const cXyz& dComIfGs_getRestartRoomPos() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomPos();
}

u32 dComIfGs_getRestartRoomParam() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomParam();
}

void dComIfGs_setRestartRoomParam(u32 i_param) {
    g_dComIfG_gameInfo.info.getRestart().setRoomParam(i_param);
}

f32 dComIfGs_getLastSceneSpeedF() {
    return g_dComIfG_gameInfo.info.getRestart().getLastSpeedF();
}

u32 dComIfGs_getLastSceneMode() {
    return g_dComIfG_gameInfo.info.getRestart().getLastMode();
}

s16 dComIfGs_getLastSceneAngleY() {
    return g_dComIfG_gameInfo.info.getRestart().getLastAngleY();
}

void dComIfGs_setRestartRoom(const cXyz& i_position, s16 i_angle, s8 i_roomNo) {
    g_dComIfG_gameInfo.info.getRestart().setRoom(i_position, i_angle, i_roomNo);
}

void dComIfGs_onTmpBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getTmp().onEventBit(i_flag);
}

void dComIfGs_offTmpBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getTmp().offEventBit(i_flag);
}

BOOL dComIfGs_isTmpBit(const u16 i_flag) {
    return g_dComIfG_gameInfo.info.getTmp().isEventBit(i_flag);
}

void dComIfGs_setTmpReg(u16 i_reg, u8 i_no) {
    g_dComIfG_gameInfo.info.getTmp().setEventReg(i_reg, i_no);
}

u8 dComIfGs_getTmpReg(const u16 i_reg) {
    return g_dComIfG_gameInfo.info.getTmp().getEventReg(i_reg);
}

dSv_turnRestart_c& dComIfGs_getTurnRestart() {
    return g_dComIfG_gameInfo.info.getTurnRestart();
}

const cXyz& dComIfGs_getTurnRestartPos() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getPos();
}

u32 dComIfGs_getTurnRestartParam() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getParam();
}

s16 dComIfGs_getTurnRestartAngleY() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getAngleY();
}

s8 dComIfGs_getTurnRestartRoomNo() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getRoomNo();
}

void dComIfGs_setTurnRestart(const cXyz& i_position, s16 i_angle, s8 param_2, u32 i_param) {
    g_dComIfG_gameInfo.info.getTurnRestart().set(i_position, i_angle, param_2, i_param);
}

u8 dComIfGs_getDataNum() {
    return g_dComIfG_gameInfo.info.getDataNum();
}

void dComIfGs_setDataNum(u8 i_num) {
    return g_dComIfG_gameInfo.info.setDataNum(i_num);
}

u8 dComIfGs_getNewFile() {
    return g_dComIfG_gameInfo.info.getNewFile();
}

void dComIfGs_setNewFile(u8 i_fileNo) {
    return g_dComIfG_gameInfo.info.setNewFile(i_fileNo);
}

u8 dComIfGs_getNoFile() {
    return g_dComIfG_gameInfo.info.getNoFile();
}

void dComIfGs_setNoFile(u8 i_file) {
    g_dComIfG_gameInfo.info.setNoFile(i_file);
}

s64 dComIfGs_getSaveStartTime() {
    return g_dComIfG_gameInfo.info.getStartTime();
}

void dComIfGs_setSaveStartTime(s64 i_time) {
    g_dComIfG_gameInfo.info.setStartTime(i_time);
}

s64 dComIfGs_getSaveTotalTime() {
    return g_dComIfG_gameInfo.info.getSaveTotalTime();
}

void dComIfGs_setSaveTotalTime(s64 i_time) {
    g_dComIfG_gameInfo.info.setSaveTotalTime(i_time);
}

void dComIfGs_init() {
    g_dComIfG_gameInfo.info.init();
}

void dComIfGs_getSave(int i_stageNo) {
    g_dComIfG_gameInfo.info.getSave(i_stageNo);
}

void dComIfGs_putSave(int i_stageNo) {
    g_dComIfG_gameInfo.info.putSave(i_stageNo);
}

void dComIfGs_initZone() {
    g_dComIfG_gameInfo.info.initZone();
}

int dComIfGs_createZone(int roomNo) {
    return g_dComIfG_gameInfo.info.createZone(roomNo);
}

void dComIfGs_onSwitch(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.onSwitch(i_no, i_roomNo);
}

void dComIfGs_offSwitch(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.offSwitch(i_no, i_roomNo);
}

BOOL dComIfGs_isSwitch(int i_no, int i_roomNo) {
    return g_dComIfG_gameInfo.info.isSwitch(i_no, i_roomNo);
}

void dComIfGs_revSwitch(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.revSwitch(i_no, i_roomNo);
}

void dComIfGs_onItem(int i_bitNo, int i_roomNo) {
    g_dComIfG_gameInfo.info.onItem(i_bitNo, i_roomNo);
}

bool dComIfGs_isItem(int i_bitNo, int i_roomNo) {
    return g_dComIfG_gameInfo.info.isItem(i_bitNo, i_roomNo);
}

void dComIfGs_onActor(int i_bitNo, int i_roomNo) {
    g_dComIfG_gameInfo.info.onActor(i_bitNo, i_roomNo);
}

void dComIfGs_offActor(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.offActor(i_no, i_roomNo);
}

BOOL dComIfGs_isActor(int i_no, int i_roomNo) {
    return g_dComIfG_gameInfo.info.isActor(i_no, i_roomNo);
}

void dComIfGs_setMemoryToCard(u8* i_saveData, int i_dataNum) {
    g_dComIfG_gameInfo.info.memory_to_card((char*)i_saveData, i_dataNum);
}

void dComIfGs_setCardToMemory(u8* card_ptr, int dataNum) {
    g_dComIfG_gameInfo.info.card_to_memory((char*)card_ptr, dataNum);
}

void dComIfGs_setInitDataToCard(u8* i_saveData, int i_dataNum) {
    g_dComIfG_gameInfo.info.initdata_to_card((char*)i_saveData, i_dataNum);
}

u8 dComIfGs_getWolfAbility(int i_idx) {
    return g_dComIfG_gameInfo.play.getWolfAbility(i_idx);
}

s8 dComIfGs_Grass_hide_Check() {
    return g_dComIfG_gameInfo.field_0x1ddfc;
}

dStage_startStage_c* dComIfGp_getStartStage() {
    return g_dComIfG_gameInfo.play.getStartStage();
}

void dComIfGp_setStartStage(dStage_startStage_c* p_startStage) {
    g_dComIfG_gameInfo.play.setStartStage(p_startStage);
}

const char* dComIfGp_getStartStageName() {
    return g_dComIfG_gameInfo.play.getStartStageName();
}

s16 dComIfGp_getStartStagePoint() {
    return g_dComIfG_gameInfo.play.getStartStagePoint();
}

s8 dComIfGp_getStartStageRoomNo() {
    return g_dComIfG_gameInfo.play.getStartStageRoomNo();
}

s8 dComIfGp_getStartStageLayer() {
    return g_dComIfG_gameInfo.play.getStartStageLayer();
}

void dComIfGp_setStartStageLayer(s8 layer) {
    g_dComIfG_gameInfo.play.setStartStageLayer(layer);
}

s8 dComIfGp_getStartStageDarkArea() {
    return g_dComIfG_gameInfo.play.getStartStageDarkArea();
}

void dComIfGp_setStartStageDarkArea(s8 darkArea) {
    g_dComIfG_gameInfo.play.setStartStageDarkArea(darkArea);
}

dStage_startStage_c* dComIfGp_getNextStartStage() {
    return g_dComIfG_gameInfo.play.getNextStartStage();
}

const char* dComIfGp_getNextStageName() {
    return g_dComIfG_gameInfo.play.getNextStageName();
}

s16 dComIfGp_getNextStagePoint() {
    return g_dComIfG_gameInfo.play.getNextStagePoint();
}

s16 dComIfGp_getNextStageRoomNo() {
    return g_dComIfG_gameInfo.play.getNextStageRoomNo();
}

s16 dComIfGp_getNextStageLayer() {
    return g_dComIfG_gameInfo.play.getNextStageLayer();
}

BOOL dComIfGp_isEnableNextStage() {
    return g_dComIfG_gameInfo.play.isEnableNextStage();
}

void dComIfGp_offEnableNextStage() {
    g_dComIfG_gameInfo.play.offEnableNextStage();
}

s8 dComIfGp_getNextStageWipe() {
    return g_dComIfG_gameInfo.play.getNextStageWipe();
}

u8 dComIfGp_getNextStageWipeSpeed() {
    return g_dComIfG_gameInfo.play.getNextStageWipeSpeed();
}

dStage_stageDt_c* dComIfGp_getStage() {
    return &g_dComIfG_gameInfo.play.getStage();
}

roomRead_class* dComIfGp_getStageRoom() {
    return g_dComIfG_gameInfo.play.getStage().getRoom();
}

stage_palette_info_class* dComIfGp_getStagePaletteInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPaletteInfo();
}

stage_pselect_info_class* dComIfGp_getStagePselectInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPselectInfo();
}

stage_envr_info_class* dComIfGp_getStageEnvrInfo() {
    return g_dComIfG_gameInfo.play.getStage().getEnvrInfo();
}

stage_vrboxcol_info_class* dComIfGp_getStageVrboxcolInfo() {
    return g_dComIfG_gameInfo.play.getStage().getVrboxcolInfo();
}

stage_plight_info_class* dComIfGp_getStagePlightInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPlightInfo();
}

int dComIfGp_getStagePaletteNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPaletteNumInfo();
}

int dComIfGp_getStagePselectNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPselectNumInfo();
}

int dComIfGp_getStageEnvrNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getEnvrNumInfo();
}

int dComIfGp_getStageVrboxcolNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getVrboxcolNumInfo();
}

int dComIfGp_getStagePlightNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPlightNumInfo();
}

stage_stag_info_class* dComIfGp_getStageStagInfo() {
    return g_dComIfG_gameInfo.play.getStage().getStagInfo();
}

stage_scls_info_dummy_class* dComIfGp_getStageSclsInfo() {
    return g_dComIfG_gameInfo.play.getStage().getSclsInfo();
}

dStage_Multi_c* dComIfGp_getMulti() {
    return g_dComIfG_gameInfo.play.getStage().getMulti();
}

void dComIfGp_setOldMulti() {
    g_dComIfG_gameInfo.play.getStage().setOldMulti();
}

void dComIfGp_resetOldMulti() {
    g_dComIfG_gameInfo.play.getStage().resetOldMulti();
}

s16 dComIfGp_getStageWorldRollAngleX() {
    return g_dComIfG_gameInfo.play.getStage().getWorldRollAngleX();
}

s16 dComIfGp_getStageWorldRollDirAngleY() {
    return g_dComIfG_gameInfo.play.getStage().getWorldRollDirAngleY();
}

int dComIfGp_roomControl_getStayNo() {
    return dStage_roomControl_c::getStayNo();
}

BOOL dComIfGp_roomControl_getTimePass() {
    return g_dComIfG_gameInfo.play.getRoomControl()->GetTimePass();
}

void dComIfGp_roomControl_setTimePass(int isPassing) {
    g_dComIfG_gameInfo.play.getRoomControl()->SetTimePass(isPassing);
}

dKy_tevstr_c* dComIfGp_roomControl_getTevStr(int i_roomNo) {
    return g_dComIfG_gameInfo.play.getRoomControl()->getTevStr(i_roomNo);
}

void dComIfGp_roomControl_setStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->setStatusFlag(i_roomNo, flag);
}

void dComIfGp_roomControl_onStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->onStatusFlag(i_roomNo, flag);
}

void dComIfGp_roomControl_offStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->offStatusFlag(i_roomNo, flag);
}

u8 dComIfGp_roomControl_checkStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->checkStatusFlag(i_roomNo, flag);
}

s32 dComIfGp_roomControl_getZoneNo(int i_roomNo) {
    return g_dComIfG_gameInfo.play.getRoomControl()->getZoneNo(i_roomNo);
}

void dComIfGp_roomControl_setZoneNo(int roomNo, int zoneNo) {
    g_dComIfG_gameInfo.play.getRoomControl()->setZoneNo(roomNo, zoneNo);
}

void dComIfGp_roomControl_init() {
    g_dComIfG_gameInfo.play.getRoomControl()->init();
}

void dComIfGp_roomControl_initZone() {
    g_dComIfG_gameInfo.play.getRoomControl()->initZone();
}

dStage_roomDt_c* dComIfGp_roomControl_getStatusRoomDt(int room_no) {
    return g_dComIfG_gameInfo.play.getRoomControl()->getStatusRoomDt(room_no);
}

void dComIfGp_roomControl_setStayNo(int stayNo) {
    g_dComIfG_gameInfo.play.mRoomControl.setStayNo(stayNo);
}

BOOL dComIfGp_roomControl_checkRoomDisp(int i_roomNo) {
    return g_dComIfG_gameInfo.play.getRoomControl()->checkRoomDisp(i_roomNo);
}

int dComIfGp_roomControl_loadRoom(int param_0, u8* param_1, bool param_2) {
    return g_dComIfG_gameInfo.play.getRoomControl()->loadRoom(param_0, param_1, param_2);
}

void dComIfGp_roomControl_zoneCountCheck(int i_roomNo) {
    g_dComIfG_gameInfo.play.getRoomControl()->zoneCountCheck(i_roomNo);
}

dEvt_control_c* dComIfGp_getEvent() {
    return g_dComIfG_gameInfo.play.getEvent();
}

u16 dComIfGp_event_chkEventFlag(u16 i_flag) {
    return g_dComIfG_gameInfo.play.getEvent()->chkEventFlag(i_flag);
}

void dComIfGp_event_onEventFlag(u16 i_flag) {
    g_dComIfG_gameInfo.play.getEvent()->onEventFlag(i_flag);
}

void dComIfGp_event_onHindFlag(u16 i_flag) {
    g_dComIfG_gameInfo.play.getEvent()->onHindFlag(i_flag);
}

void dComIfGp_event_offHindFlag(u16 i_flag) {
    g_dComIfG_gameInfo.play.getEvent()->offHindFlag(i_flag);
}

u8 dComIfGp_event_getMode() {
    return g_dComIfG_gameInfo.play.getEvent()->getMode();
}

BOOL dComIfGp_event_runCheck() {
    return g_dComIfG_gameInfo.play.getEvent()->runCheck();
}

u16 dComIfGp_event_checkHind(u16 i_hindFlag) {
    if (!dComIfGp_event_runCheck()) {
        return false;
    }
    return g_dComIfG_gameInfo.play.getEvent()->checkHind(i_hindFlag);
}

BOOL dComIfGp_event_chkTalkXY() {
    return g_dComIfG_gameInfo.play.getEvent()->chkTalkXY();
}

u8 dComIfGp_event_getPreItemNo() {
    return g_dComIfG_gameInfo.play.getEvent()->getPreItemNo();
}

f32 dComIfGp_event_getCullRate() {
    return g_dComIfG_gameInfo.play.getEvent()->getCullRate();
}

void dComIfGp_event_setCullRate(f32 i_rate) {
    g_dComIfG_gameInfo.play.getEvent()->setCullRate(i_rate);
}

int dComIfGp_event_order(u16 i_type, u16 i_prio, u16 i_flags, u16 i_hindFlags, void* i_requestActor,
    void* i_targetActor, s16 i_eventID, u8 i_mapToolId) {
    return g_dComIfG_gameInfo.play.getEvent()->order(i_type, i_prio, i_flags, i_hindFlags,
        i_requestActor, i_targetActor, i_eventID, i_mapToolId);
}

void dComIfGp_event_reset() {
    g_dComIfG_gameInfo.play.getEvent()->reset();
}

int dComIfGp_event_moveApproval(void* i_actor) {
    return g_dComIfG_gameInfo.play.getEvent()->moveApproval(i_actor);
}

BOOL dComIfGp_event_compulsory(void* i_actor, const char* i_eventname, u16 i_hindFlag) {
    return g_dComIfG_gameInfo.play.getEvent()->compulsory(i_actor, i_eventname, i_hindFlag);
}

void dComIfGp_event_remove() {
    g_dComIfG_gameInfo.play.getEvent()->remove();
}

fopAc_ac_c* dComIfGp_event_getPt1() {
    return g_dComIfG_gameInfo.play.getEvent()->getPt1();
}

fopAc_ac_c* dComIfGp_event_getPt2() {
    return g_dComIfG_gameInfo.play.getEvent()->getPt2();
}

fopAc_ac_c* dComIfGp_event_getTalkPartner() {
    return g_dComIfG_gameInfo.play.getEvent()->getPtT();
}

void dComIfGp_event_setTalkPartner(void* i_actor) {
    g_dComIfG_gameInfo.play.getEvent()->setPtT(i_actor);
}

fopAc_ac_c* dComIfGp_event_getItemPartner() {
    return g_dComIfG_gameInfo.play.getEvent()->getPtI();
}

void dComIfGp_event_setItemPartner(void* i_actor) {
    g_dComIfG_gameInfo.play.getEvent()->setPtI(i_actor);
}

void dComIfGp_event_setItemPartnerId(fpc_ProcID i_id) {
    g_dComIfG_gameInfo.play.getEvent()->setPtI_Id(i_id);
}

fopAc_ac_c* dComIfGp_event_getDoorPartner() {
    return g_dComIfG_gameInfo.play.getEvent()->getPtD();
}

void dComIfGp_event_setDoorPartner(void* i_partner) {
    g_dComIfG_gameInfo.play.getEvent()->setPtD(i_partner);
}

u8 dComIfGp_event_getGtItm() {
    return g_dComIfG_gameInfo.play.getEvent()->getGtItm();
}

void dComIfGp_event_setGtItm(u8 i_itemNo) {
    g_dComIfG_gameInfo.play.getEvent()->setGtItm(i_itemNo);
}

dEvent_manager_c& dComIfGp_getEventManager() {
    return g_dComIfG_gameInfo.play.getEvtManager();
}

dEvent_manager_c* dComIfGp_getPEvtManager() {
    return &g_dComIfG_gameInfo.play.getEvtManager();
}

int dComIfGp_evmng_cameraPlay() {
    return dComIfGp_getPEvtManager()->cameraPlay();
}

int dComIfGp_evmng_startDemo(int i_mapToolId) {
    return dComIfGp_getPEvtManager()->setStartDemo(i_mapToolId);
}

void dComIfGp_evmng_create() {
    g_dComIfG_gameInfo.play.getEvtManager().create();
}

void dComIfGp_evmng_remove() {
    g_dComIfG_gameInfo.play.getEvtManager().remove();
}

bool dComIfGp_evmng_existence(s16 eventId) {
    return g_dComIfG_gameInfo.play.getEvtManager().getEventData(eventId) != NULL;
}

s16 dComIfGp_evmng_getEventIdx(const char* eventName, u8 mapToolID) {
    return g_dComIfG_gameInfo.play.getEvtManager().getEventIdx(eventName, mapToolID, -1);
}

bool dComIfGp_evmng_existence(const char* eventname) {
    return g_dComIfG_gameInfo.play.getEvtManager().getEventData(
               dComIfGp_evmng_getEventIdx(eventname, 0xFF)) != NULL;
}

BOOL dComIfGp_evmng_isMapToolCamera(u8 mapToolID) {
    return g_dComIfG_gameInfo.play.getEvtManager().isMapToolCamera(mapToolID, -1);
}

BOOL dComIfGp_evmng_startCheck(s16 i_eventId) {
    return g_dComIfG_gameInfo.play.getEvtManager().startCheck(i_eventId);
}

BOOL dComIfGp_evmng_startCheck(char const* i_eventname) {
    return g_dComIfG_gameInfo.play.getEvtManager().startCheckOld(i_eventname);
}

BOOL dComIfGp_evmng_endCheck(s16 i_eventID) {
    return g_dComIfG_gameInfo.play.getEvtManager().endCheck(i_eventID);
}

BOOL dComIfGp_evmng_endCheck(const char* i_eventname) {
    return g_dComIfG_gameInfo.play.getEvtManager().endCheckOld(i_eventname);
}

int dComIfGp_evmng_getMyStaffId(const char* i_staffname, fopAc_ac_c* i_actor, int i_tagId) {
    return dComIfGp_getPEvtManager()->getMyStaffId(i_staffname, i_actor, i_tagId);
}

int dComIfGp_evmng_getIsAddvance(int i_staffId) {
    return dComIfGp_getPEvtManager()->getIsAddvance(i_staffId);
}

int dComIfGp_evmng_getMyActIdx(int i_staffId, DUSK_CONST char* DUSK_CONST* i_actions,
    int i_actionNum, BOOL param_3, BOOL param_4) {
    return dComIfGp_getPEvtManager()->getMyActIdx(
        i_staffId, i_actions, i_actionNum, param_3, param_4);
}

f32* dComIfGp_evmng_getMyFloatP(int i_staffId, DUSK_CONST char* i_dataname) {
    return (f32*)dComIfGp_getPEvtManager()->getMySubstanceP(
        i_staffId, i_dataname, dEvDtData_c::TYPE_FLOAT);
}

cXyz* dComIfGp_evmng_getMyXyzP(int i_staffId, DUSK_CONST char* i_dataname) {
    return (cXyz*)dComIfGp_getPEvtManager()->getMySubstanceP(
        i_staffId, i_dataname, dEvDtData_c::TYPE_VEC);
}

int* dComIfGp_evmng_getMyIntegerP(int i_staffId, DUSK_CONST char* i_dataname) {
    return (int*)dComIfGp_getPEvtManager()->getMySubstanceP(
        i_staffId, i_dataname, dEvDtData_c::TYPE_INT);
}

char* dComIfGp_evmng_getMyStringP(int i_staffId, DUSK_CONST char* i_dataname) {
    return (char*)dComIfGp_getPEvtManager()->getMySubstanceP(
        i_staffId, i_dataname, dEvDtData_c::TYPE_STRING);
}

int dComIfGp_evmng_getMySubstanceNum(int i_staffId, DUSK_CONST char* i_dataname) {
    return dComIfGp_getPEvtManager()->getMySubstanceNum(i_staffId, i_dataname);
}

void dComIfGp_evmng_cutEnd(int i_staffId) {
    dComIfGp_getPEvtManager()->cutEnd(i_staffId);
}

void dComIfGp_evmng_setGoal(cXyz* i_pos) {
    dComIfGp_getPEvtManager()->setGoal(i_pos);
}

cXyz* dComIfGp_evmng_getGoal() {
    return dComIfGp_getPEvtManager()->getGoal();
}

BOOL dComIfGp_evmng_ChkPresentEnd() {
    return dComIfGp_getPEvtManager()->ChkPresentEnd();
}

int dComIfGp_evmng_checkStartDemo() {
    return dComIfGp_getPEvtManager()->checkStartDemo();
}

dAttention_c* dComIfGp_getAttention() {
    return g_dComIfG_gameInfo.play.getAttention();
}

fopAc_ac_c* dComIfGp_att_getZHint() {
    return dComIfGp_getAttention()->getZHintTarget();
}

int dComIfGp_att_ZHintRequest(fopAc_ac_c* param_1, int param_2) {
    return dComIfGp_getAttention()->ZHintRequest(param_1, param_2);
}

fopAc_ac_c* dComIfGp_att_getCatghTarget() {
    return dComIfGp_getAttention()->getCatghTarget();
}

u8 dComIfGp_att_getCatchChgItem() {
    return dComIfGp_getAttention()->getCatchChgItem();
}

int dComIfGp_att_CatchRequest(fopAc_ac_c* param_0, u8 param_1, f32 i_horizontalDist, f32 i_upDist,
    f32 i_downDist, s16 i_angle, int param_5) {
    return dComIfGp_getAttention()->CatchRequest(
        param_0, param_1, i_horizontalDist, i_upDist, i_downDist, i_angle, param_5);
}

fopAc_ac_c* dComIfGp_att_getLookTarget() {
    return dComIfGp_getAttention()->getLookTarget();
}

void dComIfGp_att_LookRequest(fopAc_ac_c* param_0, f32 i_horizontalDist, f32 i_upDist,
    f32 i_downDist, s16 i_angle, int param_5) {
    dComIfGp_getAttention()->LookRequest(
        param_0, i_horizontalDist, i_upDist, i_downDist, i_angle, param_5);
}

dVibration_c& dComIfGp_getVibration() {
    return g_dComIfG_gameInfo.play.getVibration();
}

JKRAramArchive* dComIfGp_getFieldMapArchive2() {
    return g_dComIfG_gameInfo.play.getFieldMapArchive2();
}

void dComIfGp_setFieldMapArchive2(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setFieldMapArchive2(arc);
}

JKRArchive* dComIfGp_getMsgArchive(int idx) {
    return g_dComIfG_gameInfo.play.getMsgArchive(idx);
}

void dComIfGp_setMsgArchive(int i, JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMsgArchive(i, arc);
}

JKRArchive* dComIfGp_getDemoMsgArchive() {
    return g_dComIfG_gameInfo.play.getDemoMsgArchive();
}

void dComIfGp_setDemoMsgArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setDemoMsgArchive(arc);
}

JKRArchive* dComIfGp_getMeterButtonArchive() {
    return g_dComIfG_gameInfo.play.getMeterButtonArchive();
}

void dComIfGp_setMeterButtonArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMeterButtonArchive(arc);
}

JKRArchive* dComIfGp_getFontArchive() {
    return g_dComIfG_gameInfo.play.getFontArchive();
}

void dComIfGp_setFontArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setFontArchive(arc);
}

JKRArchive* dComIfGp_getRubyArchive() {
    return g_dComIfG_gameInfo.play.getRubyArchive();
}

void dComIfGp_setRubyArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setRubyArchive(arc);
}

JKRArchive* dComIfGp_getAnmArchive() {
    return g_dComIfG_gameInfo.play.getAnmArchive();
}

void dComIfGp_setAnmArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setAnmArchive(arc);
}

JKRArchive* dComIfGp_getCollectResArchive() {
    return g_dComIfG_gameInfo.play.getCollectResArchive();
}

void dComIfGp_setCollectResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setCollectResArchive(arc);
}

JKRArchive* dComIfGp_getFmapResArchive() {
    return g_dComIfG_gameInfo.play.getFmapResArchive();
}

void dComIfGp_setFmapResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setFmapResArchive(arc);
}

JKRArchive* dComIfGp_getDmapResArchive() {
    return g_dComIfG_gameInfo.play.getDmapResArchive();
}

void dComIfGp_setDmapResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setDmapResArchive(arc);
}

JKRArchive* dComIfGp_getOptionResArchive() {
    return g_dComIfG_gameInfo.play.getOptionResArchive();
}

void dComIfGp_setOptionResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setOptionResArchive(arc);
}

JKRArchive* dComIfGp_getItemIconArchive() {
    return g_dComIfG_gameInfo.play.getItemIconArchive();
}

void dComIfGp_setItemIconArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setItemIconArchive(arc);
}

JKRArchive* dComIfGp_getNameResArchive() {
    return g_dComIfG_gameInfo.play.getNameResArchive();
}

void dComIfGp_setNameResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setNameResArchive(arc);
}

void dComIfGp_setErrorResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setErrorResArchive(arc);
}

JKRArchive* dComIfGp_getAllMapArchive() {
    return g_dComIfG_gameInfo.play.getAllMapArchive();
}

void dComIfGp_setAllMapArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setAllMapArchive(arc);
}

JKRArchive* dComIfGp_getMsgCommonArchive() {
    return g_dComIfG_gameInfo.play.getMsgCommonArchive();
}

void dComIfGp_setMsgCommonArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMsgCommonArchive(arc);
}

JKRArchive* dComIfGp_getRingResArchive() {
    return g_dComIfG_gameInfo.play.getRingResArchive();
}

void dComIfGp_setRingResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setRingResArchive(arc);
}

JKRArchive* dComIfGp_getCardIconResArchive() {
    return g_dComIfG_gameInfo.play.getCardIconResArchive();
}

void dComIfGp_setCardIconResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setCardIconResArchive(arc);
}

JKRArchive* dComIfGp_getMsgDtArchive() {
    return g_dComIfG_gameInfo.play.getMsgDtArchive();
}

JKRArchive* dComIfGp_getMsgDtArchive(int idx) {
    return g_dComIfG_gameInfo.play.getMsgDtArchive(idx);
}

void dComIfGp_setMsgDtArchive(int i, JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMsgDtArchive(i, arc);
}

JKRArchive* dComIfGp_getMain2DArchive() {
    return g_dComIfG_gameInfo.play.getMain2DArchive();
}

void dComIfGp_setMain2DArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMain2DArchive(arc);
}

JKRExpHeap* dComIfGp_particle_getResHeap() {
    return g_dComIfG_gameInfo.play.getParticle()->getResHeap();
}

void dComIfGp_particle_levelExecute(u32 param_0) {
    g_dComIfG_gameInfo.play.getParticle()->levelExecute(param_0);
}

void dComIfGp_particle_createCommon(const void* data) {
    g_dComIfG_gameInfo.play.getParticle()->createCommon(data);
}

void dComIfGp_particle_readScene(u8 particle_no, mDoDvdThd_toMainRam_c** param_1) {
    g_dComIfG_gameInfo.play.getParticle()->readScene(particle_no, param_1);
}

void dComIfGp_particle_createScene(const void* param_0) {
    g_dComIfG_gameInfo.play.getParticle()->createScene(param_0);
}

void dComIfGp_particle_removeScene(bool param_0) {
    g_dComIfG_gameInfo.play.getParticle()->removeScene(param_0);
}

void dComIfGp_particle_cleanup() {
    g_dComIfG_gameInfo.play.getParticle()->cleanup();
}

void dComIfGp_particle_calc3D() {
    g_dComIfG_gameInfo.play.getParticle()->calc3D();
}

void dComIfGp_particle_calc2D() {
    g_dComIfG_gameInfo.play.getParticle()->calc2D();
}

void dComIfGp_particle_calcMenu() {
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->calcMenu();
    }
}

void dComIfGp_particle_draw(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormal(i_drawInfo);
    }
}

void dComIfGp_particle_drawFog(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalFog(i_drawInfo);
    }
}

void dComIfGp_particle_drawP1(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalP1(i_drawInfo);
    }
}

void dComIfGp_particle_drawProjection(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawProjection(i_drawInfo);
    }
}

void dComIfGp_particle_drawNormalPri0_A(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalPri0_A(i_drawInfo);
    }
}

void dComIfGp_particle_drawNormalPri0_B(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalPri0_B(i_drawInfo);
    }
}

void dComIfGp_particle_drawFogPri0_A(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri0_A(i_drawInfo);
    }
}

void dComIfGp_particle_drawFogPri0_B(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri0_B(i_drawInfo);
    }
}

void dComIfGp_particle_drawFogPri1(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri1(i_drawInfo);
    }
}

void dComIfGp_particle_drawFogPri2(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri2(i_drawInfo);
    }
}

void dComIfGp_particle_drawFogPri3(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri3(i_drawInfo);
    }
}

void dComIfGp_particle_drawFogPri4(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri4(i_drawInfo);
    }
}

void dComIfGp_particle_drawDarkworld(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawDarkworld(i_drawInfo);
    }
}

void dComIfGp_particle_drawScreen(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogScreen(i_drawInfo);
    }
}

void dComIfGp_particle_draw2Dgame(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2Dgame(i_drawInfo);
    }
}

void dComIfGp_particle_draw2Dfore(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2Dfore(i_drawInfo);
    }
}

void dComIfGp_particle_draw2Dback(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2Dback(i_drawInfo);
    }
}

void dComIfGp_particle_draw2DmenuFore(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2DmenuFore(i_drawInfo);
    }
}

void dComIfGp_particle_draw2DmenuBack(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2DmenuBack(i_drawInfo);
    }
}

void dComIfGp_setHitMark(u16 i_hitmark, fopAc_ac_c* param_1, const cXyz* param_2,
    const csXyz* param_3, const cXyz* param_4, u32 i_atType) {
    g_dComIfG_gameInfo.play.getParticle()->setHitMark(
        i_hitmark, param_1, param_2, param_3, param_4, i_atType);
}

void dComIfGp_particle_setWaterRipple(u32* param_0, cBgS_PolyInfo& param_1, const cXyz* param_2,
    f32 param_3, const dKy_tevstr_c* param_4, const cXyz* param_5, s8 param_6) {
    g_dComIfG_gameInfo.play.getParticle()->setWaterRipple(
        param_0, param_1, param_2, param_3, param_4, param_5, param_6);
}

u32 dComIfGp_particle_setPolyColor(u32 param_0, u16 param_1, cBgS_PolyInfo& param_2,
    const cXyz* param_3, const dKy_tevstr_c* param_4, const csXyz* param_5, const cXyz* param_6,
    int param_7, dPa_levelEcallBack* param_8, s8 param_9, const cXyz* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setPoly(param_0, param_1, param_2, param_3,
        param_4, param_5, param_6, param_7, param_8, param_9, param_10);
}

JPABaseEmitter* dComIfGp_particle_setPolyColor(u16 param_1, cBgS_PolyInfo& param_2,
    const cXyz* param_3, const dKy_tevstr_c* param_4, const csXyz* param_5, const cXyz* param_6,
    int param_7, dPa_levelEcallBack* param_8, s8 param_9, const cXyz* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setPoly(
        param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10);
}

void dComIfGp_particle_setSimple(u16 param_0, cXyz* i_pos, u8 param_2, GXColor& param_3,
    GXColor& param_4, int param_5, float param_6) {
    g_dComIfG_gameInfo.play.getParticle()->setSimple(
        param_0, i_pos, 0, param_2, param_3, param_4, param_5, param_6);
}

u32 dComIfGp_particle_setStopContinue(u32 param_0) {
    return g_dComIfG_gameInfo.play.getParticle()->setStopContinue(param_0);
}

u32 dComIfGp_particle_setSimpleFoot(u32 param_0, u32* param_1, cBgS_PolyInfo& param_2,
    cXyz const* param_3, dKy_tevstr_c const* param_4, int param_5, csXyz const* param_6,
    cXyz const* param_7, dPa_levelEcallBack* param_8, s8 param_9, cXyz const* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setSimpleFoot(param_0, param_1, param_2, param_3,
        param_4, param_5, param_6, param_7, param_8, param_9, param_10);
}

u16 dComIfGp_particle_setCommonPoly(u32* param_0, cBgS_PolyInfo* param_1, const cXyz* param_2,
    const cXyz* param_3, const dKy_tevstr_c* param_4, u32 param_5, u32 param_6,
    const csXyz* param_7, const cXyz* param_8, s8 param_9) {
    return g_dComIfG_gameInfo.play.getParticle()->setCommonPoly(
        param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9);
}

void dComIfGp_particle_levelEmitterOnEventMove(u32 param_0) {
    g_dComIfG_gameInfo.play.getParticle()->forceOnEventMove(param_0);
}

JPABaseEmitter* dComIfGp_particle_getEmitter(u32 param_0) {
    return g_dComIfG_gameInfo.play.getParticle()->getEmitter(param_0);
}

u32 dComIfGp_particle_set(u32 param_0, u16 param_1, const cXyz* i_pos, const dKy_tevstr_c* param_3,
    const csXyz* i_rotation, const cXyz* i_scale, u8 i_alpha, dPa_levelEcallBack* param_7,
    s8 param_8, const GXColor* param_9, const GXColor* param_10, const cXyz* param_11) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(param_0, param_1, i_pos, param_3,
        i_rotation, i_scale, i_alpha, param_7, param_8, param_9, param_10, param_11, 1.0f);
}

u32 dComIfGp_particle_set(u32 param_0, u16 param_1, const cXyz* i_pos, const csXyz* i_rotation,
    const cXyz* i_scale, u8 param_5, dPa_levelEcallBack* param_6, s8 param_7,
    const GXColor* param_8, const GXColor* param_9, const cXyz* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(param_0, param_1, i_pos, NULL,
        i_rotation, i_scale, param_5, param_6, param_7, param_8, param_9, param_10, 1.0f);
}

JPABaseEmitter* dComIfGp_particle_set(u16 i_resID, const cXyz* i_pos, const dKy_tevstr_c* param_3,
    const csXyz* i_rotation, const cXyz* i_scale, u8 i_alpha, dPa_levelEcallBack* i_callback,
    s8 param_8, const GXColor* i_prmColor, const GXColor* i_envColor, const cXyz* i_particleScale) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(i_resID, i_pos, param_3, i_rotation,
        i_scale, i_alpha, i_callback, param_8, i_prmColor, i_envColor, i_particleScale, 1.0f);
}

JPABaseEmitter* dComIfGp_particle_set(u16 i_resID, const cXyz* i_pos, const csXyz* i_rotation,
    const cXyz* i_scale, u8 i_alpha, dPa_levelEcallBack* i_callback, s8 param_7,
    const GXColor* i_prmColor, const GXColor* i_envColor, const cXyz* i_particleScale) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(i_resID, i_pos, NULL, i_rotation,
        i_scale, i_alpha, i_callback, param_7, i_prmColor, i_envColor, i_particleScale, 1.0f);
}

u32 dComIfGp_particle_set(
    u32 param_0, u16 param_1, const cXyz* i_pos, const dKy_tevstr_c* param_3) {
    return dComIfGp_particle_set(
        param_0, param_1, i_pos, param_3, NULL, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
}

JPABaseEmitter* dComIfGp_particle_set(
    u16 i_resID, const cXyz* i_pos, const csXyz* i_rotation, const cXyz* i_scale) {
    return dComIfGp_particle_set(
        i_resID, i_pos, NULL, i_rotation, i_scale, 255, NULL, -1, NULL, NULL, NULL);
}

JPABaseEmitter* dComIfGp_particle_set(u16 i_resID, const cXyz* i_pos, const dKy_tevstr_c* param_2,
    const csXyz* i_rotation, const cXyz* i_scale) {
    return dComIfGp_particle_set(
        i_resID, i_pos, param_2, i_rotation, i_scale, 255, NULL, -1, NULL, NULL, NULL);
}

u32 dComIfGp_particle_set(
    u32 param_0, u16 param_1, const cXyz* i_pos, const csXyz* param_3, const cXyz* param_4) {
    return dComIfGp_particle_set(
        param_0, param_1, i_pos, param_3, param_4, 0xFF, NULL, -1, NULL, NULL, NULL);
}

u32 dComIfGp_particle_setColor(u32 param_0, u16 param_1, const cXyz* i_pos,
    const dKy_tevstr_c* param_3, const GXColor* param_4, const GXColor* param_5, f32 param_6,
    u8 param_7, const csXyz* param_8, const cXyz* param_9, dPa_levelEcallBack* param_10,
    s8 param_11, const cXyz* param_12) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(param_0, param_1, i_pos, param_3,
        param_8, param_9, param_7, param_10, param_11, param_4, param_5, param_12, param_6);
}

JPABaseEmitter* dComIfGp_particle_setColor(u16 param_0, const cXyz* i_pos,
    const dKy_tevstr_c* param_2, const GXColor* param_3, const GXColor* param_4, f32 param_5,
    u8 param_6, const csXyz* param_7, const cXyz* param_8, dPa_levelEcallBack* param_9, s8 param_10,
    const cXyz* param_11) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(param_0, i_pos, param_2, param_7,
        param_8, param_6, param_9, param_10, param_3, param_4, param_11, param_5);
}

JPABaseEmitter* dComIfGp_particle_setColor(u16 param_0, const cXyz* i_pos,
    const dKy_tevstr_c* param_2, const GXColor* param_3, const GXColor* param_4, f32 param_5,
    u8 param_6) {
    return dComIfGp_particle_setColor(
        param_0, i_pos, param_2, param_3, param_4, param_5, param_6, NULL, NULL, NULL, -1, NULL);
}

u32 dComIfGp_particle_getHeapSize() {
    return g_dComIfG_gameInfo.play.getParticle()->getHeapSize();
}

u32 dComIfGp_particle_getSceneHeapSize() {
    return g_dComIfG_gameInfo.play.getParticle()->getSceneHeapSize();
}

int dComIfGp_particle_getEmitterNum() {
    return g_dComIfG_gameInfo.play.getParticle()->getEmitterNum();
}

int dComIfGp_particle_getParticleNum() {
    return g_dComIfG_gameInfo.play.getParticle()->getParticleNum();
}

dSmplMdl_draw_c* dComIfGp_getSimpleModel() {
    return g_dComIfG_gameInfo.play.getSimpleModel();
}

int dComIfGp_getWindowNum() {
    return g_dComIfG_gameInfo.play.getWindowNum();
}

void dComIfGp_setWindowNum(int num) {
    g_dComIfG_gameInfo.play.setWindowNum(num);
}

s8 dComIfGp_getLayerOld() {
    return g_dComIfG_gameInfo.play.getLayerOld();
}

s32 dComIfGp_checkStatus(u16 flags) {
    return g_dComIfG_gameInfo.play.checkStatus(flags);
}

void dComIfGp_setStatus(u16 status) {
    g_dComIfG_gameInfo.play.setStatus(status);
}

void dComIfGp_onStatus(u16 i_status) {
    g_dComIfG_gameInfo.play.onStatus(i_status);
}

dDlst_window_c* dComIfGp_getWindow(int i) {
    return g_dComIfG_gameInfo.play.getWindow(i);
}

void dComIfGp_setWindow(u8 i, f32 param_1, f32 param_2, f32 param_3, f32 param_4, f32 param_5,
    f32 param_6, int camID, int mode) {
    g_dComIfG_gameInfo.play.setWindow(
        i, param_1, param_2, param_3, param_4, param_5, param_6, camID, mode);
}

camera_process_class* dComIfGp_getCamera(int idx) {
    return (camera_process_class*)g_dComIfG_gameInfo.play.getCamera(idx);
}

void dComIfGp_setCamera(int i, camera_class* cam) {
    g_dComIfG_gameInfo.play.setCamera(i, cam);
}

int dComIfGp_getCameraWinID(int idx) {
    return g_dComIfG_gameInfo.play.getCameraWinID(idx);
}

int dComIfGp_getCameraPlayer1ID(int idx) {
    return g_dComIfG_gameInfo.play.getCameraPlayer1ID(idx);
}

int dComIfGp_getCameraPlayer2ID(int idx) {
    return g_dComIfG_gameInfo.play.getCameraPlayer2ID(idx);
}

u32 dComIfGp_getCameraAttentionStatus(int i_no) {
    return g_dComIfG_gameInfo.play.getCameraAttentionStatus(i_no);
}

BOOL dComIfGp_checkCameraAttentionStatus(int i, u32 flag) {
    return g_dComIfG_gameInfo.play.checkCameraAttentionStatus(i, flag);
}

void dComIfGp_onCameraAttentionStatus(int i, u32 flag) {
    g_dComIfG_gameInfo.play.onCameraAttentionStatus(i, flag);
}

void dComIfGp_offCameraAttentionStatus(int i, u32 flag) {
    g_dComIfG_gameInfo.play.offCameraAttentionStatus(i, flag);
}

void dComIfGp_setCameraInfo(
    int camIdx, camera_class* p_cam, int param_2, int param_3, int param_4) {
    g_dComIfG_gameInfo.play.setCameraInfo(camIdx, p_cam, param_2, param_3, param_4);
}

f32 dComIfGp_getCameraZoomScale(int i_no) {
    return g_dComIfG_gameInfo.play.getCameraZoomScale(i_no);
}

void dComIfGp_setCameraZoomScale(int i_no, f32 i_scale) {
    g_dComIfG_gameInfo.play.setCameraZoomScale(i_no, i_scale);
}

f32 dComIfGp_getCameraZoomForcus(int i_no) {
    return g_dComIfG_gameInfo.play.getCameraZoomForcus(i_no);
}

void dComIfGp_setCameraZoomForcus(int i_no, f32 i_focus) {
    g_dComIfG_gameInfo.play.setCameraZoomForcus(i_no, i_focus);
}

const char* dComIfGp_getCameraParamFileName(int i) {
    return g_dComIfG_gameInfo.play.getCameraParamFileName(i);
}

void dComIfGp_setCameraParamFileName(int i, char* name) {
    g_dComIfG_gameInfo.play.setCameraParamFileName(i, name);
}

void dComIfGp_saveCameraPosition(int i, cXyz* i_pos, cXyz* i_target, f32 i_fovy, s16 i_bank) {
    g_dComIfG_gameInfo.play.saveCameraPosition(i, i_pos, i_target, i_fovy, i_bank);
}

void dComIfGp_loadCameraPosition(int i, cXyz* o_pos, cXyz* o_target, f32* o_fovy, s16* o_bank) {
    g_dComIfG_gameInfo.play.loadCameraPosition(i, o_pos, o_target, o_fovy, o_bank);
}

fopAc_ac_c* dComIfGp_getPlayer(int idx) {
    return g_dComIfG_gameInfo.play.getPlayer(idx);
}

void dComIfGp_setPlayer(int i, fopAc_ac_c* player) {
    g_dComIfG_gameInfo.play.setPlayer(i, player);
}

int dComIfGp_getPlayerCameraID(int idx) {
    return g_dComIfG_gameInfo.play.getPlayerCameraID(idx);
}

void dComIfGp_setPlayerInfo(int plyrIdx, fopAc_ac_c* ptr, int camIdx) {
    g_dComIfG_gameInfo.play.setPlayerInfo(plyrIdx, ptr, camIdx);
}

daPy_py_c* dComIfGp_getLinkPlayer() {
    return (daPy_py_c*)g_dComIfG_gameInfo.play.getPlayerPtr(LINK_PTR);
}

daHorse_c* dComIfGp_getHorseActor() {
    return (daHorse_c*)g_dComIfG_gameInfo.play.getPlayerPtr(HORSE_PTR);
}

void dComIfGp_setLinkPlayer(fopAc_ac_c* ptr) {
    g_dComIfG_gameInfo.play.setPlayerPtr(0, ptr);
}

void dComIfGp_setHorseActor(fopAc_ac_c* i_horse) {
    g_dComIfG_gameInfo.play.setPlayerPtr(1, i_horse);
}

void dComIfGp_setPlayerPtr(int i, fopAc_ac_c* ptr) {
    g_dComIfG_gameInfo.play.setPlayerPtr(i, ptr);
}

dMsgObject_c* dComIfGp_getMsgObjectClass() {
    return g_dComIfG_gameInfo.play.getMsgObjectClass();
}

void dComIfGp_setMsgObjectClass(dMsgObject_c* obj) {
    return g_dComIfG_gameInfo.play.setMsgObjectClass(obj);
}

f32 dComIfGp_getItemLifeCount() {
    return g_dComIfG_gameInfo.play.getItemLifeCount();
}

u8 dComIfGp_getItemLifeCountType() {
    return g_dComIfG_gameInfo.play.getItemLifeCountType();
}

void dComIfGp_setItemLifeCount(f32 amount, u8 type) {
    g_dComIfG_gameInfo.play.setItemLifeCount(amount, type);
}

void dComIfGp_clearItemLifeCount() {
    g_dComIfG_gameInfo.play.clearItemLifeCount();
}

s32 dComIfGp_getItemRupeeCount() {
    return g_dComIfG_gameInfo.play.getItemRupeeCount();
}

void dComIfGp_setItemRupeeCount(s32 amount) {
    g_dComIfG_gameInfo.play.setItemRupeeCount(amount);
}

void dComIfGp_clearItemRupeeCount() {
    g_dComIfG_gameInfo.play.clearItemRupeeCount();
}

s16 dComIfGp_getItemKeyNumCount() {
    return g_dComIfG_gameInfo.play.getItemKeyNumCount();
}

void dComIfGp_setItemKeyNumCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemKeyNumCount(count);
}

void dComIfGp_clearItemKeyNumCount() {
    g_dComIfG_gameInfo.play.clearItemKeyNumCount();
}

s16 dComIfGp_getItemMaxLifeCount() {
    return g_dComIfG_gameInfo.play.getItemMaxLifeCount();
}

void dComIfGp_setItemMaxLifeCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemMaxLifeCount(count);
}

void dComIfGp_clearItemMaxLifeCount() {
    g_dComIfG_gameInfo.play.clearItemMaxLifeCount();
}

void dComIfGp_setItemMagicCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemMagicCount(count);
}

void dComIfGp_setItemNowMagic(s16 magic) {
    g_dComIfG_gameInfo.play.setItemNowMagic(magic);
}

void dComIfGp_setItemMaxMagicCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemMaxMagicCount(count);
}

s32 dComIfGp_getItemOilCount() {
    return g_dComIfG_gameInfo.play.getItemOilCount();
}

void dComIfGp_setItemOilCount(s32 oil) {
    g_dComIfG_gameInfo.play.setItemOilCount(oil);
}

void dComIfGp_setItemMaxOilCount(s32 oil) {
    g_dComIfG_gameInfo.play.setItemMaxOilCount(oil);
}

void dComIfGp_clearItemOilCount() {
    g_dComIfG_gameInfo.play.clearItemOilCount();
}

s32 dComIfGp_getItemNowOil() {
    return g_dComIfG_gameInfo.play.getItemNowOil();
}

void dComIfGp_setItemNowOil(s32 oil) {
    g_dComIfG_gameInfo.play.setItemNowOil(oil);
}

s32 dComIfGp_getItemMaxOilCount() {
    return g_dComIfG_gameInfo.play.getItemMaxOilCount();
}

void dComIfGp_clearItemMaxOilCount() {
    g_dComIfG_gameInfo.play.clearItemMaxOilCount();
}

int dComIfGp_getOxygen() {
    return g_dComIfG_gameInfo.play.getOxygen();
}

void dComIfGp_setOxygen(s32 oxygen) {
    g_dComIfG_gameInfo.play.setOxygen(oxygen);
}

int dComIfGp_getNowOxygen() {
    return g_dComIfG_gameInfo.play.getNowOxygen();
}

void dComIfGp_setNowOxygen(s32 oxygen) {
    g_dComIfG_gameInfo.play.setNowOxygen(oxygen);
}

s32 dComIfGp_getMaxOxygen() {
    return g_dComIfG_gameInfo.play.getMaxOxygen();
}

void dComIfGp_setMaxOxygen(s32 i_oxygen) {
    g_dComIfG_gameInfo.play.setMaxOxygen(i_oxygen);
}

s32 dComIfGp_getOxygenCount() {
    return g_dComIfG_gameInfo.play.getOxygenCount();
}

void dComIfGp_setOxygenCount(s32 oxygen) {
    g_dComIfG_gameInfo.play.setOxygenCount(oxygen);
}

void dComIfGp_clearOxygenCount() {
    g_dComIfG_gameInfo.play.clearOxygenCount();
}

void dComIfGp_setMaxOxygenCount(s32 oxygen) {
    g_dComIfG_gameInfo.play.setMaxOxygenCount(oxygen);
}

s32 dComIfGp_getMaxOxygenCount() {
    return g_dComIfG_gameInfo.play.getMaxOxygenCount();
}

void dComIfGp_clearMaxOxygenCount() {
    g_dComIfG_gameInfo.play.clearMaxOxygenCount();
}

s16 dComIfGp_getItemArrowNumCount() {
    return g_dComIfG_gameInfo.play.getItemArrowNumCount();
}

void dComIfGp_setItemArrowNumCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemArrowNumCount(count);
}

void dComIfGp_clearItemArrowNumCount() {
    g_dComIfG_gameInfo.play.clearItemArrowNumCount();
}

s16 dComIfGp_getItemPachinkoNumCount() {
    return g_dComIfG_gameInfo.play.getItemPachinkoNumCount();
}

void dComIfGp_setItemPachinkoNumCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemPachinkoNumCount(count);
}

void dComIfGp_clearItemPachinkoNumCount() {
    g_dComIfG_gameInfo.play.clearItemPachinkoNumCount();
}

s16 dComIfGp_getItemMaxArrowNumCount() {
    return g_dComIfG_gameInfo.play.getItemMaxArrowNumCount();
}

int dComIfGp_getMessageCountNumber() {
    return g_dComIfG_gameInfo.play.getMessageCountNumber();
}

void dComIfGp_setMessageCountNumber(s32 number) {
    g_dComIfG_gameInfo.play.setMessageCountNumber(number);
}

u16 dComIfGp_getItemNowLife() {
    return g_dComIfG_gameInfo.play.getItemNowLife();
}

void dComIfGp_setItemNowLife(u16 life) {
    g_dComIfG_gameInfo.play.setItemNowLife(life);
}

u8 dComIfGp_getMesgStatus() {
    return g_dComIfG_gameInfo.play.getMesgStatus();
}

u8 dComIfGp_getRStatus() {
    return g_dComIfG_gameInfo.play.getRStatus();
}

bool dComIfGp_isRSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isRSetFlag(flag);
}

void dComIfGp_setRStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setRStatus(status, flag);
}

u8 dComIfGp_getRStatusForce() {
    return g_dComIfG_gameInfo.play.getRStatusForce();
}

u8 dComIfGp_getRSetFlagForce() {
    return g_dComIfG_gameInfo.play.getRSetFlagForce();
}

void dComIfGp_setRStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setRStatusForce(status, flag);
}

u8 dComIfGp_getAStatus() {
    return g_dComIfG_gameInfo.play.getAStatus();
}

bool dComIfGp_isASetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isASetFlag(flag);
}

void dComIfGp_setAStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setAStatus(status, flag);
}

u8 dComIfGp_getAStatusForce() {
    return g_dComIfG_gameInfo.play.getAStatusForce();
}

u8 dComIfGp_getASetFlagForce() {
    return g_dComIfG_gameInfo.play.getASetFlagForce();
}

void dComIfGp_setAStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setAStatusForce(status, flag);
}

u8 dComIfGp_getNunStatus() {
    return g_dComIfG_gameInfo.play.getNunStatus();
}

bool dComIfGp_isNunSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isNunSetFlag(flag);
}

void dComIfGp_setNunStatus(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setNunStatus(status, param_1, flag);
}

u8 dComIfGp_getBottleStatus() {
    return g_dComIfG_gameInfo.play.getBottleStatus();
}

bool dComIfGp_isBottleSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isBottleSetFlag(flag);
}

void dComIfGp_setBottleStatus(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setBottleStatus(param_0, param_1);
}

u8 dComIfGp_getBottleStatusForce() {
    return g_dComIfG_gameInfo.play.getBottleStatusForce();
}

u8 dComIfGp_getBottleSetFlagForce() {
    return g_dComIfG_gameInfo.play.getBottleSetFlagForce();
}

void dComIfGp_setBottleStatusForce(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setBottleStatusForce(param_0, param_1);
}

u8 dComIfGp_getRemoConStatus() {
    return g_dComIfG_gameInfo.play.getRemoConStatus();
}

bool dComIfGp_isRemoConSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isRemoConSetFlag(flag);
}

void dComIfGp_setRemoConStatus(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setRemoConStatus(status, param_1, flag);
}

u8 dComIfGp_getDoStatus() {
    return g_dComIfG_gameInfo.play.getDoStatus();
}

bool dComIfGp_isDoSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isDoSetFlag(flag);
}

void dComIfGp_setDoStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setDoStatus(status, flag);
}

u8 dComIfGp_getDoStatusForce() {
    return g_dComIfG_gameInfo.play.getDoStatusForce();
}

u8 dComIfGp_getDoSetFlagForce() {
    return g_dComIfG_gameInfo.play.getDoSetFlagForce();
}

void dComIfGp_setDoStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setDoStatusForce(status, flag);
}

u8 dComIfGp_get3DStatus() {
    return g_dComIfG_gameInfo.play.get3DStatus();
}

u8 dComIfGp_get3DDirection() {
    return g_dComIfG_gameInfo.play.get3DDirection();
}

bool dComIfGp_is3DSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.is3DSetFlag(flag);
}

void dComIfGp_set3DStatus(u8 status, u8 direction, u8 flag) {
    g_dComIfG_gameInfo.play.set3DStatus(status, direction, flag);
}

u8 dComIfGp_get3DStatusForce() {
    return g_dComIfG_gameInfo.play.get3DStatusForce();
}

u8 dComIfGp_get3DDirectionForce() {
    return g_dComIfG_gameInfo.play.get3DDirectionForce();
}

u8 dComIfGp_get3DSetFlagForce() {
    return g_dComIfG_gameInfo.play.get3DSetFlagForce();
}

void dComIfGp_set3DStatusForce(u8 status, u8 direction, u8 flag) {
    g_dComIfG_gameInfo.play.set3DStatusForce(status, direction, flag);
}

u8 dComIfGp_getCStickStatus() {
    return g_dComIfG_gameInfo.play.getCStickStatus();
}

u8 dComIfGp_getCStickDirection() {
    return g_dComIfG_gameInfo.play.getCStickDirection();
}

bool dComIfGp_isCStickSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isCStickSetFlag(flag);
}

void dComIfGp_setCStickStatus(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setCStickStatus(status, param_1, flag);
}

u8 dComIfGp_getCStickStatusForce() {
    return g_dComIfG_gameInfo.play.getCStickStatusForce();
}

u8 dComIfGp_getCStickDirectionForce() {
    return g_dComIfG_gameInfo.play.getCStickDirectionForce();
}

u8 dComIfGp_getCStickSetFlagForce() {
    return g_dComIfG_gameInfo.play.getCStickSetFlagForce();
}

void dComIfGp_setCStickStatusForce(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setCStickStatusForce(status, param_1, flag);
}

u8 dComIfGp_getSButtonStatus() {
    return g_dComIfG_gameInfo.play.getSButtonStatus();
}

bool dComIfGp_isSButtonSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isSButtonSetFlag(flag);
}

void dComIfGp_setSButtonStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setSButtonStatus(status, flag);
}

u8 dComIfGp_getSButtonStatusForce() {
    return g_dComIfG_gameInfo.play.getSButtonStatusForce();
}

u8 dComIfGp_getSButtonSetFlagForce() {
    return g_dComIfG_gameInfo.play.getSButtonSetFlagForce();
}

void dComIfGp_setSButtonStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setSButtonStatusForce(status, flag);
}

u8 dComIfGp_getZStatus() {
    return g_dComIfG_gameInfo.play.getZStatus();
}

bool dComIfGp_isZSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isZSetFlag(flag);
}

void dComIfGp_setZStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setZStatus(status, flag);
}

u8 dComIfGp_getZStatusForce() {
    return g_dComIfG_gameInfo.play.getZStatusForce();
}

u8 dComIfGp_getZSetFlagForce() {
    return g_dComIfG_gameInfo.play.getZSetFlagForce();
}

void dComIfGp_setZStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setZStatusForce(status, flag);
}

u8 dComIfGp_getXStatus() {
    return g_dComIfG_gameInfo.play.getXStatus();
}

bool dComIfGp_isXSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isXSetFlag(flag);
}

void dComIfGp_setXStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setXStatus(status, flag);
}

u8 dComIfGp_getXStatusForce() {
    return g_dComIfG_gameInfo.play.getXStatusForce();
}

u8 dComIfGp_getXSetFlagForce() {
    return g_dComIfG_gameInfo.play.getXSetFlagForce();
}

void dComIfGp_setXStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setXStatusForce(status, flag);
}

u8 dComIfGp_getYStatus() {
    return g_dComIfG_gameInfo.play.getYStatus();
}

bool dComIfGp_isYSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isYSetFlag(flag);
}

void dComIfGp_setYStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setYStatus(status, flag);
}

u8 dComIfGp_getYStatusForce() {
    return g_dComIfG_gameInfo.play.getYStatusForce();
}

u8 dComIfGp_getYSetFlagForce() {
    return g_dComIfG_gameInfo.play.getYSetFlagForce();
}

void dComIfGp_setYStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setYStatusForce(status, flag);
}

u8 dComIfGp_getNunZStatus() {
    return g_dComIfG_gameInfo.play.getNunZStatus();
}

bool dComIfGp_isNunZSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isNunZSetFlag(flag);
}

void dComIfGp_setNunZStatus(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setNunZStatus(param_0, param_1);
}

u8 dComIfGp_getNunCStatus() {
    return g_dComIfG_gameInfo.play.getNunCStatus();
}

bool dComIfGp_isNunCSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isNunCSetFlag(flag);
}

void dComIfGp_setNunCStatus(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setNunCStatus(param_0, param_1);
}

void dComIfGp_setSelectEquipClothes(u8 i_clothNo) {
    g_dComIfG_gameInfo.play.setSelectEquip(COLLECT_CLOTHING, i_clothNo);
}

void dComIfGp_setSelectEquipSword(u8 i_swordNo) {
    g_dComIfG_gameInfo.play.setSelectEquip(COLLECT_SWORD, i_swordNo);
}

void dComIfGp_setSelectEquipShield(u8 i_shieldNo) {
    g_dComIfG_gameInfo.play.setSelectEquip(COLLECT_SHIELD, i_shieldNo);
}

u8 dComIfGp_getMesgAnimeAttrInfo() {
    return g_dComIfG_gameInfo.play.getBaseAnimeID();
}

void dComIfGp_setMesgAnimeAttrInfo(u8 param_1) {
    g_dComIfG_gameInfo.play.setBaseAnimeID(param_1);
}

void dComIfGp_clearMesgAnimeAttrInfo() {
    g_dComIfG_gameInfo.play.clearBaseAnimeID();
}

u8 dComIfGp_getMesgFaceAnimeAttrInfo() {
    return g_dComIfG_gameInfo.play.getFaceAnimeID();
}

void dComIfGp_setMesgFaceAnimeAttrInfo(u8 id) {
    g_dComIfG_gameInfo.play.setFaceAnimeID(id);
}

void dComIfGp_clearMesgFaceAnimeAttrInfo() {
    g_dComIfG_gameInfo.play.clearFaceAnimeID();
}

void dComIfGp_clearMesgAnimeTagInfo() {
    g_dComIfG_gameInfo.play.clearNowAnimeID();
}

void dComIfGp_setItem(u8 slot, u8 i_no) {
    g_dComIfG_gameInfo.play.setItem(slot, i_no);
}

u8 dComIfGp_getAdvanceDirection() {
    return g_dComIfG_gameInfo.play.getDirection();
}

void dComIfGp_setAdvanceDirection(u8 i_dir) {
    g_dComIfG_gameInfo.play.setDirection(i_dir);
}

u8 dComIfGp_checkMesgCancelButton() {
    return g_dComIfG_gameInfo.play.checkMesgCancelButton();
}

void dComIfGp_setMesgCancelButton(u8 button) {
    g_dComIfG_gameInfo.play.setMesgCancelButton(button);
}

u8 dComIfGp_getGameoverStatus() {
    return g_dComIfG_gameInfo.play.getGameoverStatus();
}

void dComIfGp_setGameoverStatus(u8 i_status) {
    return g_dComIfG_gameInfo.play.setGameoverStatus(i_status);
}

u8 dComIfGp_isHeapLockFlag() {
    return g_dComIfG_gameInfo.play.isHeapLockFlag();
}

u8 dComIfGp_getSubHeapLockFlag(int idx) {
    return g_dComIfG_gameInfo.play.getSubHeapLockFlag(idx);
}

void dComIfGp_setSubHeapLockFlag(int idx, u8 status) {
    g_dComIfG_gameInfo.play.setSubHeapLockFlag(idx, status);
}

u8 dComIfGp_getNeedLightDropNum() {
    return g_dComIfG_gameInfo.play.getNeedLightDropNum();
}

void dComIfGp_setNeedLightDropNum(u8 i_num) {
    g_dComIfG_gameInfo.play.setNeedLightDropNum(i_num);
}

u8 dComIfGp_checkMesgBgm() {
    return g_dComIfG_gameInfo.play.checkMesgBgm();
}

void dComIfGp_setMesgBgmOff() {
    g_dComIfG_gameInfo.play.setMesgBgm(0);
}

void dComIfGp_setMesgBgmOn() {
    g_dComIfG_gameInfo.play.setMesgBgm(1);
}

u8 dComIfGp_isPauseFlag() {
    return g_dComIfG_gameInfo.play.isPauseFlag();
}

void dComIfGp_offPauseFlag() {
    g_dComIfG_gameInfo.play.offPauseFlag();
}

void dComIfGp_onPauseFlag() {
    g_dComIfG_gameInfo.play.onPauseFlag();
}

u8 dComIfGp_getOxygenShowFlag() {
    return g_dComIfG_gameInfo.play.getOxygenShowFlag();
}

void dComIfGp_offOxygenShowFlag() {
    g_dComIfG_gameInfo.play.setOxygenShowFlag(0);
}

void dComIfGp_onOxygenShowFlag() {
    g_dComIfG_gameInfo.play.setOxygenShowFlag(1);
}

u8 dComIfGp_2dShowCheck() {
    return g_dComIfG_gameInfo.play.show2dCheck();
}

void dComIfGp_2dShowOn() {
    g_dComIfG_gameInfo.play.show2dOn();
}

void dComIfGp_2dShowOff() {
    g_dComIfG_gameInfo.play.show2dOff();
}

JKRExpHeap* dComIfGp_getExpHeap2D() {
    return g_dComIfG_gameInfo.play.getExpHeap2D();
}

void dComIfGp_setExpHeap2D(void* heap) {
    g_dComIfG_gameInfo.play.setExpHeap2D(heap);
}

JKRExpHeap* dComIfGp_getSubExpHeap2D(int idx) {
    return g_dComIfG_gameInfo.play.getSubExpHeap2D(idx);
}

void dComIfGp_setSubExpHeap2D(int idx, void* heap) {
    g_dComIfG_gameInfo.play.setSubExpHeap2D(idx, heap);
}

JKRExpHeap* dComIfGp_getMsgExpHeap() {
    return g_dComIfG_gameInfo.play.getMsgExpHeap();
}

void dComIfGp_setMsgExpHeap(void* heap) {
    g_dComIfG_gameInfo.play.setMsgExpHeap(heap);
}

dComIfG_MesgCamInfo_c* dComIfGp_getMesgCameraInfo() {
    return g_dComIfG_gameInfo.play.getMesgCamInfo();
}

void dComIfGp_setMesgCameraTagInfo(int id) {
    g_dComIfG_gameInfo.play.setMesgCamInfoID(id);
}

void dComIfGp_clearMesgCameraTagInfo() {
    g_dComIfG_gameInfo.play.clearMesgCamInfoID();
}

void dComIfGp_setMesgCameraAttrInfo(int param_1) {
    g_dComIfG_gameInfo.play.setMesgCamInfoBasicID(param_1);
}

void dComIfGp_clearMesgCameraAttrInfo() {
    g_dComIfG_gameInfo.play.clearMesgCamInfoBasicID();
}

void dComIfGp_setMesgCameraInfoActor(fopAc_ac_c* param_1, fopAc_ac_c* param_2, fopAc_ac_c* param_3,
    fopAc_ac_c* param_4, fopAc_ac_c* param_5, fopAc_ac_c* param_6, fopAc_ac_c* param_7,
    fopAc_ac_c* param_8, fopAc_ac_c* param_9, fopAc_ac_c* param_10)

{
    g_dComIfG_gameInfo.play.setMesgCamInfoActor(
        param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10);
}

u32 dComIfGp_checkPlayerStatus0(int param_0, u32 flag) {
    return g_dComIfG_gameInfo.play.checkPlayerStatus(param_0, 0, flag);
}

u32 dComIfGp_checkPlayerStatus1(int param_0, u32 flag) {
    return g_dComIfG_gameInfo.play.checkPlayerStatus(param_0, 1, flag);
}

void dComIfGp_setPlayerStatus0(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.setPlayerStatus(param_0, 0, flag);
}

void dComIfGp_setPlayerStatus1(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.setPlayerStatus(param_0, 1, flag);
}

void dComIfGp_clearPlayerStatus0(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.clearPlayerStatus(param_0, 0, flag);
}

void dComIfGp_clearPlayerStatus1(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.clearPlayerStatus(param_0, 1, flag);
}

void dComIfGp_setCurrentWindow(dDlst_window_c* i_window) {
    g_dComIfG_gameInfo.play.setCurrentWindow(i_window);
}

void dComIfGp_setCurrentView(view_class* i_view) {
    g_dComIfG_gameInfo.play.setCurrentView(i_view);
}

void dComIfGp_setCurrentViewport(view_port_class* i_viewport) {
    g_dComIfG_gameInfo.play.setCurrentViewport(i_viewport);
}

J2DGrafContext* dComIfGp_getCurrentGrafPort() {
    return g_dComIfG_gameInfo.play.getCurrentGrafPort();
}

void dComIfGp_setCurrentGrafPort(J2DOrthoGraph* i_graf) {
    g_dComIfG_gameInfo.play.setCurrentGrafPort(i_graf);
}

void* dComIfGp_getItemTable() {
    return g_dComIfG_gameInfo.play.getItemTable();
}

void dComIfGp_setItemTable(void* data) {
    g_dComIfG_gameInfo.play.setItemTable(data);
}

char* dComIfGp_getLastPlayStageName() {
    return g_dComIfG_gameInfo.play.getLastPlayStageName();
}

void dComIfGp_setLastPlayStageName(char* name) {
    g_dComIfG_gameInfo.play.setLastPlayStageName(name);
}

void dComIfGp_init() {
    g_dComIfG_gameInfo.play.init();
}

void dComIfGp_itemDataInit() {
    g_dComIfG_gameInfo.play.itemInit();
}

void dComIfGp_setItemBombNumCount(u8 i_item, s16 count) {
    g_dComIfG_gameInfo.play.setItemBombNumCount(i_item, count);
}

s16 dComIfGp_getItemBombNumCount(u8 i_no) {
    return g_dComIfG_gameInfo.play.getItemBombNumCount(i_no);
}

void dComIfGp_clearItemBombNumCount(u8 i_no) {
    g_dComIfG_gameInfo.play.clearItemBombNumCount(i_no);
}

s16 dComIfGp_getItemMaxBombNumCount() {
    return g_dComIfG_gameInfo.play.getItemMaxBombNumCount(dItemNo_NORMAL_BOMB_e);
}

void dComIfGp_setNowVibration(u8 status) {
    g_dComIfG_gameInfo.play.setNowVibration(status);
}

u32 dComIfGp_getNowVibration() {
    return g_dComIfG_gameInfo.play.getNowVibration();
}

void dComIfGp_particle_create() {
    g_dComIfG_gameInfo.play.createParticle();
}

void dComIfGp_createSimpleModel() {
    g_dComIfG_gameInfo.play.createSimpleModel();
}

void dComIfGp_deleteSimpleModel() {
    g_dComIfG_gameInfo.play.deleteSimpleModel();
}

void dComIfGp_drawSimpleModel() {
    g_dComIfG_gameInfo.play.drawSimpleModel();
}

int dComIfGp_addSimpleModel(J3DModelData* i_modelData, int roomNo, u8 i_drawBG) {
    return g_dComIfG_gameInfo.play.addSimpleModel(i_modelData, roomNo, i_drawBG);
}

void dComIfGp_removeSimpleModel(J3DModelData* i_modelData, int roomNo) {
    g_dComIfG_gameInfo.play.removeSimpleModel(i_modelData, roomNo);
}

void dComIfGp_entrySimpleModel(J3DModel* model, int roomNo) {
    g_dComIfG_gameInfo.play.entrySimpleModel(model, roomNo);
}

void dComIfG_ct() {
    g_dComIfG_gameInfo.ct();
}

dBgS& dComIfG_Bgsp() {
    return g_dComIfG_gameInfo.play.mBgs;
}

dCcS* dComIfG_Ccsp() {
    return &g_dComIfG_gameInfo.play.mCcs;
}

dCcS& dComIfG_Ccsp2() {
    return g_dComIfG_gameInfo.play.mCcs;
}

void dComIfG_setTimerNowTimeMs(int time) {
    g_dComIfG_gameInfo.play.setTimerNowTimeMs(time);
}

int dComIfG_getTimerNowTimeMs() {
    return g_dComIfG_gameInfo.play.getTimerNowTimeMs();
}

void dComIfG_setTimerLimitTimeMs(int i_time) {
    g_dComIfG_gameInfo.play.setTimerLimitTimeMs(i_time);
}

int dComIfG_getTimerLimitTimeMs() {
    return g_dComIfG_gameInfo.play.getTimerLimitTimeMs();
}

void dComIfG_setTimerMode(int mode) {
    return g_dComIfG_gameInfo.play.setTimerMode(mode);
}

int dComIfG_getTimerMode() {
    return g_dComIfG_gameInfo.play.getTimerMode();
}

void dComIfG_setTimerType(u8 i_type) {
    g_dComIfG_gameInfo.play.setTimerType(i_type);
}

u8 dComIfG_getTimerType() {
    return g_dComIfG_gameInfo.play.getTimerType();
}

void dComIfG_setTimerPtr(dTimer_c* i_ptr) {
    g_dComIfG_gameInfo.play.setTimerPtr(i_ptr);
}

dTimer_c* dComIfG_getTimerPtr() {
    return g_dComIfG_gameInfo.play.getTimerPtr();
}

/**
 * Attempts to add a new Object Resource Archive (*.arc) into the Resource Control.
 * @param i_arcName Name of archive to be added
 * @param i_mountDirection The direction to mount the archive. mDoDvd_MOUNT_DIRECTION_HEAD or
 * mDoDvd_MOUNT_DIRECTION_TAIL
 * @param i_heap Pointer to heap to load resources into
 * @return TRUE if successful, FALSE otherwise
 */
int dComIfG_setObjectRes(const char* i_arcName, u8 i_mountDirection, JKRHeap* i_heap) {
    return g_dComIfG_gameInfo.mResControl.setObjectRes(i_arcName, i_mountDirection, i_heap);
}

int dComIfG_setObjectRes(
    const char* i_arcName, void* i_archiveRes, u32 i_bufferSize, JKRHeap* i_heap) {
    UNUSED(i_heap);
    return g_dComIfG_gameInfo.mResControl.setObjectRes(i_arcName, i_archiveRes, i_bufferSize, NULL);
}

/**
 * Attempts to add a new Stage Resource Archive (*.arc) into the Resource Control.
 * @param i_arcName Name of archive to be added
 * @param i_heap Pointer to heap to load resources into
 * @return TRUE if successful, FALSE otherwise
 */
int dComIfG_setStageRes(const char* i_arcName, JKRHeap* i_heap) {
    return g_dComIfG_gameInfo.mResControl.setStageRes(i_arcName, i_heap);
}

int dComIfG_syncObjectRes(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.syncObjectRes(i_arcName);
}

int dComIfG_syncStageRes(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.syncStageRes(i_arcName);
}

int dComIfG_deleteObjectResMain(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.deleteObjectRes(i_arcName);
}

int dComIfG_deleteStageRes(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.deleteStageRes(i_arcName);
}

void* dComIfG_getStageRes(const char* i_arcName, const char* i_resName) {
    return g_dComIfG_gameInfo.mResControl.getStageRes(i_arcName, i_resName);
}

void* dComIfG_getObjectRes(const char* i_arcName, const char* i_resName) {
    return g_dComIfG_gameInfo.mResControl.getObjectRes(i_arcName, i_resName);
}

void* dComIfG_getObjectRes(const char* i_arcName, int i_index) {
    return g_dComIfG_gameInfo.mResControl.getObjectRes(i_arcName, i_index);
}

void dComIfG_dumpResControl() {
    g_dComIfG_gameInfo.mResControl.dump();
}

dRes_info_c* dComIfG_getObjectResInfo(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getObjectResInfo(i_arcName);
}

dRes_info_c* dComIfG_getStageResInfo(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getStageResInfo(i_arcName);
}

int dComIfG_syncAllObjectRes() {
    return g_dComIfG_gameInfo.mResControl.syncAllObjectRes();
}

void* dComIfG_getObjectIDRes(const char* i_arcName, u16 i_resID) {
    return g_dComIfG_gameInfo.mResControl.getObjectIDRes(i_arcName, i_resID);
}

int dComIfG_getObjctResName2Index(const char* i_arcName, const char* i_resName) {
    return g_dComIfG_gameInfo.mResControl.getObjectResName2Index(i_arcName, i_resName);
}

u8 dComIfG_getBrightness() {
    return g_dComIfG_gameInfo.mFadeBrightness;
}

void dComIfG_setBrightness(u8 brightness) {
    g_dComIfG_gameInfo.mFadeBrightness = brightness;
}

BOOL dComIfG_isDebugMode() {
    return g_dComIfG_gameInfo.mIsDebugMode;
}

u32 dComIfG_getTrigB(u32 i_padNo) {
    return mDoCPd_c::getTrig(i_padNo) & PAD_BUTTON_B;
}

#if DEBUG
u32 dComIfG_getObjectAllSize() {
    return g_dComIfG_gameInfo.mResControl.getObjectAllSize();
}

u32 dComIfG_getStageAllSize() {
    return g_dComIfG_gameInfo.mResControl.getStageAllSize();
}

u32 dComIfG_getObjectSize(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getObjectSize(i_arcName);
}

u32 dComIfG_getStageSize(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getStageSize(i_arcName);
}

void dComIfG_initStopwatch() {
    OSInitStopwatch(&g_dComIfG_gameInfo.mStopwatch, "dComIfG");
}
#endif

int dComIfGd_setRealShadow(u32 param_0, s8 param_1, J3DModel* param_2, cXyz* param_3, f32 param_4,
    f32 param_5, dKy_tevstr_c* param_6) {
    return g_dComIfG_gameInfo.drawlist.setRealShadow(
        param_0, param_1, param_2, param_3, param_4, param_5, param_6);
}

int dComIfGd_setSimpleShadow(
    cXyz* pos, f32 param_1, f32 param_2, cXyz* param_3, s16 angle, f32 param_5, TGXTexObj* tex) {
    return g_dComIfG_gameInfo.drawlist.setSimpleShadow(
        pos, param_1, param_2, param_3, angle, param_5, tex);
}

bool dComIfGd_addRealShadow(u32 key, J3DModel* model) {
    return g_dComIfG_gameInfo.drawlist.addRealShadow(key, model);
}

void dComIfGd_drawListItem3d() {
    g_dComIfG_gameInfo.drawlist.drawOpaListItem3d();
    g_dComIfG_gameInfo.drawlist.drawXluListItem3d();
}

#if VERSION > VERSION_GCN_JPN
void dComIfGd_drawListCursor() {
    g_dComIfG_gameInfo.drawlist.drawOpaListCursor();
    g_dComIfG_gameInfo.drawlist.drawXluListCursor();
}
#endif

void dComIfGd_reset() {
    g_dComIfG_gameInfo.drawlist.reset();
}

void dComIfGd_set2DOpa(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.set2DOpa(dlst);
}

void dComIfGd_set2DXlu(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.set2DXlu(dlst);
}

void dComIfGd_set2DOpaTop(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.set2DOpaTop(dlst);
}

void dComIfGd_setCopy2D(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.setCopy2D(dlst);
}

view_class* dComIfGd_getView() {
    return g_dComIfG_gameInfo.drawlist.getView();
}

Mtx44* dComIfGd_getProjViewMtx() {
    return &(g_dComIfG_gameInfo.drawlist.getView()->projViewMtx);
}

MtxP dComIfGd_getInvViewMtx() {
    return g_dComIfG_gameInfo.drawlist.getView()->invViewMtx;
}

view_port_class* dComIfGd_getViewport() {
    return g_dComIfG_gameInfo.drawlist.getViewport();
}

MtxP dComIfGd_getViewRotMtx() {
    return g_dComIfG_gameInfo.drawlist.getView()->viewMtxNoTrans;
}
MtxP dComIfGd_getViewMtx() {
    return g_dComIfG_gameInfo.drawlist.getView()->viewMtx;
}

J3DDrawBuffer* dComIfGd_getListFilter() {
    return g_dComIfG_gameInfo.drawlist.getOpaListFilter();
}

J3DDrawBuffer* dComIfGd_getOpaListIndScreen() {
    return g_dComIfG_gameInfo.drawlist.getOpaListP0();
}

J3DDrawBuffer* dComIfGd_getListPacket() {
    return g_dComIfG_gameInfo.drawlist.getOpaListPacket();
}

void dComIfGd_setListSky() {
    g_dComIfG_gameInfo.drawlist.setOpaListSky();
    g_dComIfG_gameInfo.drawlist.setXluListSky();
}

void dComIfGd_setListDark() {
    g_dComIfG_gameInfo.drawlist.setOpaListDark();
    g_dComIfG_gameInfo.drawlist.setXluListDark();
}

void dComIfGd_setListInvisisble() {
    g_dComIfG_gameInfo.drawlist.setOpaListInvisible();
    g_dComIfG_gameInfo.drawlist.setXluListInvisible();
}

void dComIfGd_setListDarkBG() {
    g_dComIfG_gameInfo.drawlist.setOpaListDarkBG();
    g_dComIfG_gameInfo.drawlist.setXluListDarkBG();
}

void dComIfGd_setXluListDarkBG() {
    g_dComIfG_gameInfo.drawlist.setXluListDarkBG();
}

void dComIfGd_setList() {
    g_dComIfG_gameInfo.drawlist.setOpaList();
    g_dComIfG_gameInfo.drawlist.setXluList();
}

void dComIfGd_setListItem3D() {
    g_dComIfG_gameInfo.drawlist.setOpaListItem3D();
    g_dComIfG_gameInfo.drawlist.setXluListItem3D();
}

void dComIfGd_setList3Dlast() {
    g_dComIfG_gameInfo.drawlist.setOpaList3Dlast();
    g_dComIfG_gameInfo.drawlist.setXluList3Dlast();
}

void dComIfGd_setXluList2DScreen() {
    g_dComIfG_gameInfo.drawlist.setXluList2DScreen();
}

void dComIfGd_setXluListBG() {
    g_dComIfG_gameInfo.drawlist.setXluListBG();
}

void dComIfGd_setListBG() {
    g_dComIfG_gameInfo.drawlist.setOpaListBG();
    g_dComIfG_gameInfo.drawlist.setXluListBG();
}

void dComIfGd_setListIndScreen() {
    g_dComIfG_gameInfo.drawlist.setOpaListP0();
    g_dComIfG_gameInfo.drawlist.setXluListP0();
}

void dComIfGd_setListMiddle() {
    g_dComIfG_gameInfo.drawlist.setOpaListMiddle();
    g_dComIfG_gameInfo.drawlist.setXluListMiddle();
}

void dComIfGd_setListZxlu() {
    g_dComIfG_gameInfo.drawlist.setOpaListZxlu();
    g_dComIfG_gameInfo.drawlist.setXluListZxlu();
}

J3DDrawBuffer* dComIfGd_getOpaList() {
    return g_dComIfG_gameInfo.drawlist.getOpaList();
}

J3DDrawBuffer* dComIfGd_getOpaListBG() {
    return g_dComIfG_gameInfo.drawlist.getOpaListBG();
}

J3DDrawBuffer* dComIfGd_getOpaListDark() {
    return g_dComIfG_gameInfo.drawlist.getOpaListDark();
}

J3DDrawBuffer* dComIfGd_getXluListBG() {
    return g_dComIfG_gameInfo.drawlist.getXluListBG();
}

void dComIfGd_setListFilter() {
    g_dComIfG_gameInfo.drawlist.setOpaListFilter();
    g_dComIfG_gameInfo.drawlist.setXluListFilter();
}

void dComIfGd_init() {
    g_dComIfG_gameInfo.drawlist.init();
}

void dComIfGd_peekZ(s16 param_0, s16 param_1, u32* param_2) {
    g_dComIfG_gameInfo.drawlist.newPeekZdata(param_0, param_1, param_2);
}

void dComIfGd_peekZdata() {
    g_dComIfG_gameInfo.drawlist.peekZdata();
}

void dComIfGd_setView(view_class* view) {
    g_dComIfG_gameInfo.drawlist.setView(view);
}

void dComIfGd_setWindow(dDlst_window_c* window) {
    g_dComIfG_gameInfo.drawlist.setWindow(window);
}

void dComIfGd_setViewport(view_port_class* port) {
    g_dComIfG_gameInfo.drawlist.setViewport(port);
}

void dComIfGd_entryZSortListZxlu(J3DPacket* i_packet, cXyz& param_1) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.entryZSortListZxlu(i_packet, param_1);
}

void dComIfGd_entryZSortXluList(J3DPacket* i_packet, cXyz& param_1) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.entryZSortXluList(i_packet, param_1);
}

void dComIfGd_drawCopy2D() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawCopy2D();
}

void dComIfGd_drawOpaListSky() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListSky();
}

void dComIfGd_drawXluListSky() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListSky();
}

void dComIfGd_drawOpaListBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListBG();
}

void dComIfGd_drawOpaListDarkBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListDarkBG();
}

void dComIfGd_drawOpaListMiddle() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListMiddle();
}

void dComIfGd_drawOpaList() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaList();
}

void dComIfGd_drawOpaListDark() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListDark();
}

void dComIfGd_drawOpaListPacket() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListPacket();
}

void dComIfGd_drawXluListBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListBG();
}

void dComIfGd_drawXluListDarkBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListDarkBG();
}

void dComIfGd_drawXluList() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluList();
}

void dComIfGd_drawXluListDark() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListDark();
}

void dComIfGd_drawXluListZxlu() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListZxlu();
}

void dComIfGd_drawXluList2DScreen() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluList2DScreen();
}

void dComIfGd_drawOpaList3Dlast() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaList3Dlast();
}

void dComIfGd_draw2DOpa() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.draw2DOpa();
}

void dComIfGd_draw2DOpaTop() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.draw2DOpaTop();
}

void dComIfGd_draw2DXlu() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.draw2DXlu();
}

void dComIfGd_drawOpaListFilter() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListFilter();
}

void dComIfGd_drawIndScreen() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListP0();
}

void dComIfGd_drawListZxlu() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListZxlu();
}

void dComIfGd_drawShadow(Mtx param_0) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawShadow(param_0);
}

void dComIfGd_imageDrawShadow(Mtx param_0) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.imageDrawShadow(param_0);
}

void dComIfGd_set3DlineMat(mDoExt_3DlineMat_c* param_0) {
    g_dComIfG_gameInfo.drawlist.set3DlineMat(param_0);
}

void dComIfGd_set3DlineMatDark(mDoExt_3DlineMat_c* param_0) {
    g_dComIfG_gameInfo.drawlist.set3DlineMatDark(param_0);
}

#if PLATFORM_WII || VERSION == VERSION_SHIELD_DEBUG
void dComIfGd_setListCursor() {
    g_dComIfG_gameInfo.drawlist.setOpaListCursor();
    g_dComIfG_gameInfo.drawlist.setXluListCursor();
}
#endif

void dComIfGd_drawXluListInvisible() {
    ZoneScoped;
    if (!dusk::getSettings().game.disableWaterRefraction) {
        g_dComIfG_gameInfo.drawlist.drawXluListInvisible();
    }
}

void dComIfGd_drawOpaListInvisible() {
    ZoneScoped;
    if (!dusk::getSettings().game.disableWaterRefraction) {
        g_dComIfG_gameInfo.drawlist.drawOpaListInvisible();
    }
}
#endif
