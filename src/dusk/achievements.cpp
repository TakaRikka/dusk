#include "dusk/achievements.h"
#include "dusk/io.hpp"
#include "dusk/main.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_map_path_fmap.h"
#include "d/d_stage.h"
#include "d/d_menu_fmap.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_ni.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_b_ob.h"
#include "d/actor/d_a_player.h"
#include "d/d_demo.h"
#include "dusk/ui/ui.hpp"
#include "f_pc/f_pc_name.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_name.h"

#include <filesystem>
#include <algorithm>

namespace dusk {

using json = nlohmann::json;

static void* s_cucco_play_search(void* i_actor, void*) {
    if (!fopAcM_IsActor(i_actor) || fopAcM_GetName((fopAc_ac_c*)i_actor) != fpcNm_NI_e) {
        return nullptr;
    }
    auto* ni = static_cast<ni_class*>(i_actor);
    return ni->mAction == ACTION_PLAY_e ? i_actor : nullptr;
}

static void checkGoatHerding(Achievement& a, int32_t threshMs) {
    if (dMeter2Info_getMaxCount() != 20 || dMeter2Info_getNowCount() != 20) {
        return;
    }
    const int32_t elapsed = dMeter2Info_getTimeMs();
    if (elapsed > 0 && elapsed <= threshMs) {
        a.progress = 1;
    }
}

static constexpr auto ACHIEVEMENTS_FILENAME = "achievements.json";

std::vector<AchievementSystem::Entry> AchievementSystem::makeEntries() {
    return {
        // Challenge
        {
            {
                "hero_of_twilight",
                "Hero of Twilight",
                "Deliver the finishing blow to Ganondorf.",
                AchievementCategory::Challenge,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link != nullptr && link->mProcID == daAlink_c::PROC_GANON_FINISH) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "completionist",
                "Completionist",
                "Complete the game after collecting all equipment, heart containers, portals, bugs, poes, and hidden skills.",
                AchievementCategory::Challenge,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr || link->mProcID != daAlink_c::PROC_GANON_FINISH) {
                    return;
                }
                if (dComIfGs_getMaxLife() < 100) {
                    return;
                }
                for (int i = 0; i < 4; ++i) {
                    if (!dComIfGs_isCollectMirror(i)) {
                        return;
                    }
                }
                for (int i = 0; i < 3; ++i) {
                    if (!dComIfGs_isCollectCrystal(i)) {
                        return;
                    }
                }
                static const u16 skillBits[] = {
                    dSv_event_flag_c::F_0338, dSv_event_flag_c::F_0339,
                    dSv_event_flag_c::F_0340, dSv_event_flag_c::F_0341,
                    dSv_event_flag_c::F_0342, dSv_event_flag_c::F_0343,
                    dSv_event_flag_c::F_0344
                };
                for (u16 bit : skillBits) {
                    if (!dComIfGs_isEventBit(bit)) {
                        return;
                    }
                }
                if (dComIfGs_checkGetInsectNum() < 24) {
                    return;
                }
                if (dComIfGs_getPohSpiritNum() < 60) {
                    return;
                }
                if (dComIfGs_getWalletSize() < 2) {
                    return;
                }
                if (dComIfGs_getArrowMax() < 100) {
                    return;
                }
                if (!dComIfGs_isCollectSword(COLLECT_MASTER_SWORD)) {
                    return;
                }
                if (!dComIfGs_isCollectShield(COLLECT_HYLIAN_SHIELD)) {
                    return;
                }
                if (!dComIfGs_isCollectClothes(KOKIRI_CLOTHES_FLAG)) {
                    return;
                }
                if (!dComIfGs_isItemFirstBit(dItemNo_WEAR_ZORA_e)) {
                    return;
                }
                if (!dComIfGs_isItemFirstBit(dItemNo_ARMOR_e)) {
                    return;
                }
                static const struct { int stage; int sw; } warpPortals[] = {
                    { dStage_SaveTbl_ORDON,     52 },  // Ordon Spring
                    { dStage_SaveTbl_FARON,     71 },  // South Faron Woods
                    { dStage_SaveTbl_FARON,      2 },  // North Faron Woods
                    { dStage_SaveTbl_GROVE,    100 },  // Sacred Grove
                    { dStage_SaveTbl_FIELD,     21 },  // Gorge
                    { dStage_SaveTbl_ELDIN,     31 },  // Kakariko Village
                    { dStage_SaveTbl_ELDIN,     21 },  // Death Mountain
                    { dStage_SaveTbl_FIELD,     99 },  // Bridge of Eldin
                    { dStage_SaveTbl_FIELD,      3 },  // Castle Town
                    { dStage_SaveTbl_LANAYRU,   10 },  // Lake Hylia
                    { dStage_SaveTbl_LANAYRU,   2 },  // Zora's Domain
                    { dStage_SaveTbl_LANAYRU,  21 },  // Upper Zora's River
                    { dStage_SaveTbl_SNOWPEAK, 21 },  // Snowpeak
                    { dStage_SaveTbl_DESERT,   21 },  // Gerudo Mesa
                    { dStage_SaveTbl_DESERT,   40 },  // Mirror Chamber
                };
                for (const auto& p : warpPortals) {
                    if (!g_dComIfG_gameInfo.info.getSavedata().getSave(p.stage).getBit().isSwitch(p.sw)) {
                        return;
                    }
                }
                if (dComIfGs_getCollectSmell() == dItemNo_NONE_e) {
                    return;
                }

                if (dMeter2Info_getRecieveLetterNum() == 0) {
                    return;
                }

                bool hasJournal = false;
                for (int fi = 0; fi < 6; ++fi) {
                    if (dComIfGs_getFishNum(fi) != 0) {
                        hasJournal = true;
                        break;
                    }
                }
                if (!hasJournal) {
                    return;
                }

                int bottleCount = 0;
                for (int i = 0; i < dSv_player_item_c::BOTTLE_MAX; ++i) {
                    if (dComIfGs_getItem(SLOT_11 + i, false) != dItemNo_NONE_e) {
                        bottleCount++;
                    }
                }
                if (bottleCount < 4) {
                    return;
                }

                int bombBagCount = 0;
                for (int i = 0; i < dSv_player_item_c::BOMB_BAG_MAX; ++i) {
                    if (dComIfGs_getItem(SLOT_15 + i, false) != dItemNo_NONE_e) {
                        bombBagCount++;
                    }
                }
                if (bombBagCount < 3) {
                    return;
                }

                bool hasJewelRod = false;
                for (int slot = 0; slot < 24 && !hasJewelRod; ++slot) {
                    const u8 item = dComIfGs_getItem(slot, false);
                    if (item == dItemNo_JEWEL_ROD_e || item == dItemNo_JEWEL_BEE_ROD_e || item == dItemNo_JEWEL_WORM_ROD_e) {
                        hasJewelRod = true;
                    }
                }
                if (!hasJewelRod) {
                    return;
                }

                static const u8 requiredWheelItems[] = {
                    dItemNo_BOOMERANG_e,
                    dItemNo_BOW_e,
                    dItemNo_W_HOOKSHOT_e,
                    dItemNo_SPINNER_e,
                    dItemNo_IRONBALL_e,
                    dItemNo_COPY_ROD_e,
                    dItemNo_HVY_BOOTS_e,
                    dItemNo_KANTERA_e,
                    dItemNo_PACHINKO_e,
                    dItemNo_HAWK_EYE_e,
                    dItemNo_ANCIENT_DOCUMENT_e,
                    dItemNo_HORSE_FLUTE_e,
                };
                for (u8 required : requiredWheelItems) {
                    bool found = false;
                    for (int slot = 0; slot < 24; ++slot) {
                        if (dComIfGs_getItem(slot, false) == required) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        return;
                    }
                }
                a.progress = 1;
            },
            {}
        },
        // Collection
        {
            {
                "princess_of_bugs",
                "The Princess of Bugs",
                "Deliver all 24 golden bugs to Agitha.",
                AchievementCategory::Collection,
                true, 24, 0, false
            },
            [](Achievement& a, json&) {
                a.progress = dComIfGs_checkGetInsectNum();
            },
            {}
        },
        {
            {
                "all_poes",
                "Poe Collector",
                "Collect all 60 Poe Souls.",
                AchievementCategory::Collection,
                true, 60, 0, false
            },
            [](Achievement& a, json&) {
                a.progress = dComIfGs_getPohSpiritNum();
            },
            {}
        },
        {
            {
                "hylian_loach",
                "Legendary Catch",
                "Catch a Hylian Loach.",
                AchievementCategory::Collection,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_getFishNum(1) > 0) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "all_fish",
                "Gone Fishin'",
                "Catch all 6 species of fish.",
                AchievementCategory::Collection,
                true, 6, 0, false
            },
            [](Achievement& a, json&) {
                int nUniqueFish = 0;
                for (int i = 0; i < 6; ++i) {
                    if (dComIfGs_getFishNum(i) != 0) {
                        nUniqueFish++;
                    }
                }
                a.progress = nUniqueFish;
            },
            {}
        },
        {
            {
                "a_big_heart",
                "A Big Heart",
                "Reach maximum health with all 20 heart containers.",
                AchievementCategory::Collection,
                true, 20, 0, false
            },
            [](Achievement& a, json&) {
                a.progress = dComIfGs_getMaxLife() / 5;
            },
            {}
        },
        {
            {
                "all_bottles",
                "Glassware Guardian",
                "Obtain all 4 bottles.",
                AchievementCategory::Collection,
                true, 4, 0, false
            },
            [](Achievement& a, json&) {
                if (daPy_getPlayerActorClass() == nullptr) {
                    return;
                }
                int count = 0;
                for (int i = 0; i < dSv_player_item_c::BOTTLE_MAX; ++i) {
                    if (dComIfGs_getItem(SLOT_11 + i, false) != dItemNo_NONE_e) {
                        count++;
                    }
                }
                a.progress = count;
            },
            {}
        },
        {
            {
                "all_hidden_skills",
                "Master of Secrets",
                "Learn all 7 Hidden Skills.",
                AchievementCategory::Collection,
                true, 7, 0, false
            },
            [](Achievement& a, json&) {
                static const u16 skillBits[] = {
                    dSv_event_flag_c::F_0338, dSv_event_flag_c::F_0339,
                    dSv_event_flag_c::F_0340, dSv_event_flag_c::F_0341,
                    dSv_event_flag_c::F_0342, dSv_event_flag_c::F_0343,
                    dSv_event_flag_c::F_0344
                };
                int count = 0;
                for (u16 bit : skillBits) {
                    if (dComIfGs_isEventBit(bit)) {
                        count++;
                    }
                }
                a.progress = count;
            },
            {}
        },
        {
            {
                "all_letters",
                "We Deliver!",
                "Collect all 16 postman letters.",
                AchievementCategory::Collection,
                true, 16, 0, false
            },
            [](Achievement& a, json&) {
                a.progress = dMeter2Info_getRecieveLetterNum();
            },
            {}
        },
        {
            {
                "cave_of_ordeals",
                "Conqueror of Ordeals",
                "Clear all 50 floors of the Cave of Ordeals.",
                AchievementCategory::Challenge,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (daNpcF_chkEvtBit(0x1F9)) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "cave_of_ordeals_heartless",
                "Indomitable",
                "Clear all 50 floors of the Cave of Ordeals with only 3 heart containers.",
                AchievementCategory::Challenge,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (daNpcF_chkEvtBit(0x1F9) && dComIfGs_getMaxLife() <= 15) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "speedrun_12h",
                "Been There Done That",
                "Defeat Ganondorf with a total save file play time under 12 hours.",
                AchievementCategory::Challenge,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr || link->mProcID != daAlink_c::PROC_GANON_FINISH) {
                    return;
                }
                const int64_t ticks = (static_cast<int64_t>(OSGetTime()) - dComIfGs_getSaveStartTime()) + dComIfGs_getSaveTotalTime();
                if (ticks / OS_TIMER_CLOCK < 12 * 3600) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "speedrun_8h",
                "Swift Blade",
                "Defeat Ganondorf with a total save file play time under 6 hours.",
                AchievementCategory::Challenge,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr || link->mProcID != daAlink_c::PROC_GANON_FINISH) {
                    return;
                }
                const int64_t ticks = (static_cast<int64_t>(OSGetTime()) - dComIfGs_getSaveStartTime()) + dComIfGs_getSaveTotalTime();
                if (ticks / OS_TIMER_CLOCK < 8 * 3600) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "dark_hammer_one_hit",
                "Mortal Edge",
                "Defeat Dark Hammer in a single hit.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (AchievementSystem::get().hasSignal("dark_hammer_one_hit")) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "no_deaths_clear",
                "Deathless",
                "Defeat Ganondorf with 0 deaths on your save file.",
                AchievementCategory::Challenge,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr || link->mProcID != daAlink_c::PROC_GANON_FINISH) {
                    return;
                }
                if (dComIfGs_getDeathCount() == 0) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "untouchable",
                "Untouchable",
                "Kill 25 enemies in a row without taking damage.",
                AchievementCategory::Challenge,
                true, 25, 0, false
            },
            [](Achievement& a, json&) {
                auto& sys = AchievementSystem::get();
                if (sys.hasSignal("player_damaged")) {
                    a.progress = 0;
                }
                if (sys.hasSignal("enemy_killed")) {
                    a.progress++;
                }
            },
            {}
        },
        {
            {
                "bow_100m_hit",
                "Long Shot",
                "Hit an enemy from over 100 meters away with the bow.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (AchievementSystem::get().hasSignal("arrow_hit_100m")) {
                    a.progress = 1;
                }
            },
            {}
        },
        // Minigame
        {
            {
                "plumm_max",
                "Thank You Berry Much",
                "Score 61,454 points in the Plumm minigame.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_getBalloonScore() >= 61454) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "rollgoal_8",
                "Rollgoal Novice",
                "Complete the first 8 rollgoal stages.",
                AchievementCategory::Minigame,
                true, 8, 0, false
            },
            [](Achievement& a, json&) {
                a.progress = std::min((int)dComIfGs_getEventReg(0xf63f), 8);
            },
            {}
        },
        {
            {
                "rollgoal_all",
                "Lost Your Marbles",
                "Complete all rollgoal stages.",
                AchievementCategory::Minigame,
                true, 64, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_isEventBit(dSv_event_flag_c::KORO2_ALLCLEAR)) {
                    a.progress = 64;
                } else {
                    a.progress = dComIfGs_getEventReg(0xf63f);
                }
            },
            {}
        },
        {
            {
                "goat_30s",
                "Ranch Hand",
                "Herd all 20 goats into the pen in under 30 seconds.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                checkGoatHerding(a, 30000);
            },
            {}
        },
        {
            {
                "goat_20s",
                "Bane of Howard",
                "Herd all 20 goats into the pen in under 20 seconds.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                checkGoatHerding(a, 20000);
            },
            {}
        },
        {
            {
                "goat_18s",
                "King of the Ranch",
                "Herd all 20 goats into the pen in under 18 seconds.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                checkGoatHerding(a, 18000);
            },
            {}
        },
        {
            {
                "snowboard_70s",
                "Downhill Dash",
                "Finish the snowboarding minigame in under 70 seconds.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const int32_t bestMs = dComIfGs_getRaceGameTime();
                if (dComIfGs_isEventBit(dSv_event_flag_c::F_0481) &&
                    bestMs > 0 && bestMs <= 70000) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "canoe_perfect",
                "River Raider",
                "Achieve a perfect score in the canoe minigame.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr) {
                    return;
                }
                static bool wasInCanoe = false;
                bool inCanoe = link->mProcID >= daAlink_c::PROC_CANOE_RIDE &&
                               link->mProcID <= daAlink_c::PROC_CANOE_KANDELAAR_POUR;
                if (wasInCanoe && !inCanoe && dMeter2Info_getNowCount() >= 30) {
                    a.progress = 1;
                }
                wasInCanoe = inCanoe;
            },
            {}
        },
        {
            {
                "star_2_under_40s",
                "Rising Star",
                "Complete the STAR Prize 2 minigame in under 40 seconds.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if(dComIfGs_getHookGameTime() > 0 && dComIfGs_getHookGameTime() <= 40000) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "star_2_under_30s",
                "Shooting Star",
                "Complete the STAR Prize 2 minigame in under 30 seconds.",
                AchievementCategory::Minigame,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if(dComIfGs_getHookGameTime() > 0 && dComIfGs_getHookGameTime() <= 30000) {
                    a.progress = 1;
                }
            },
            {}
        },
        // Misc
        {
            {
                "friendly_fire",
                "Friendly Fire",
                "Get hit by your own cannonball.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (AchievementSystem::get().hasSignal("iron_ball_hit_player")) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "long_jump_attack",
                "Long Jump Attack",
                "Travel more than 20 meters in a single jump attack before landing.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                static bool inJump = false;
                static float startX = 0.0f, startZ = 0.0f;

                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr) {
                    inJump = false;
                    return;
                }

                if (!inJump) {
                    if (link->mProcID == daAlink_c::PROC_CUT_JUMP) {
                        inJump = true;
                        startX = link->current.pos.x;
                        startZ = link->current.pos.z;
                    }
                } else if (link->mProcID == daAlink_c::PROC_CUT_JUMP_LAND) {
                    inJump = false;
                    const float dx = link->current.pos.x - startX;
                    const float dz = link->current.pos.z - startZ;
                    if (dx * dx + dz * dz >= 2000.0f * 2000.0f) {
                        a.progress = 1;
                    }
                } else if (link->mProcID != daAlink_c::PROC_CUT_JUMP) {
                    inJump = false;
                }
            },
            {}
        },
        {
            {
                "email_me",
                "Email Me",
                "Read a letter during the Dark Beast Ganon fight.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                void* dbgExists = fopAcM_SearchByName(fpcNm_B_MGN_e);
                if (dbgExists && AchievementSystem::get().hasSignal("open_letter")) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "heavy_hitter",
                "Heavy Hitter",
                "Wear the Iron Boots during the end credits.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr || link->mProcID != daAlink_c::PROC_GANON_FINISH) {
                    return;
                }
                if (daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "fishing_rod_ganondorf",
                "Here Fishy Fishy",
                "Confuse Ganondorf with the fishing rod.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (AchievementSystem::get().hasSignal("ganondorf_fishing_rod")) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "steal_from_trill",
                "Petty Theft",
                "Steal from Trill.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_isEventBit(dSv_event_flag_c::F_0758)) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "cucco_control",
                "Cucco Whisperer",
                "Take control of a cucco.",
                AchievementCategory::Misc,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (fopAcM_Search(s_cucco_play_search, nullptr) != nullptr) {
                    a.progress = 1;
                }
            },
            {}
        },
        // Glitched
        {
            {
                "back_in_time",
                "Back in Time",
                "Perform the Back in Time glitch to play on the title screen.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (fopAcM_SearchByName(fpcNm_TITLE_e) == nullptr) {
                    return;
                }
                const auto* player = static_cast<const daPy_py_c*>(daPy_getPlayerActorClass());

                if (player != nullptr && player->mDemo.getDemoMode() == 1) {
                        a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "early_master_sword",
                "Early Master Sword",
                "Obtain the Master Sword before completing Midna's Desperate Hour.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_isCollectSword(COLLECT_MASTER_SWORD) && !dComIfGs_isEventBit(0x1E08)) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "earliest_master_sword",
                "Earliest Master Sword",
                "Obtain the Master Sword before meeting Midna.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_isCollectSword(COLLECT_MASTER_SWORD) && !dComIfGs_isTransformLV(0)) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "speedrun_4h",
                "Hero of Time",
                "Defeat Ganondorf with a total save file play time under 4 hours.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link == nullptr || link->mProcID != daAlink_c::PROC_GANON_FINISH) {
                    return;
                }
                const int64_t ticks = (static_cast<int64_t>(OSGetTime()) - dComIfGs_getSaveStartTime()) + dComIfGs_getSaveTotalTime();
                if (ticks / OS_TIMER_CLOCK < 4 * 3600) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "no_fish_suit",
                "No Fish Suit No Problem",
                "Defeat Morpheel without equipping Zora Armor.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                static bool prevMorpheelAlive = false;
                static bool inArena = false;
                static bool zoraWorn = false;
                const auto* morpheel = static_cast<const b_ob_class*>(fopAcM_SearchByName(fpcNm_B_OB_e));
                const bool morpheelAlive = morpheel != nullptr && morpheel->mAnmID != 0x14;
                const bool morpheelDead = morpheel != nullptr && morpheel->mAnmID == 0x14;
                const bool lakebedCleared = dComIfGs_isEventBit(dSv_event_flag_c::M_045);

                if (inArena && morpheel == nullptr) {
                    zoraWorn = false;
                }

                if (morpheelAlive && !lakebedCleared) {
                    inArena = true;
                    if (daPy_py_c::checkZoraWearFlg()) {
                        zoraWorn = true;
                    }
                }

                if (prevMorpheelAlive && morpheelDead && inArena && !zoraWorn) {
                    a.progress = 1;
                }

                prevMorpheelAlive = morpheelAlive;
            },
            {}
        },
        {
            {
                "null_item",
                "Null Item",
                "Obtain the mysterious black rupee in the item wheel.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (daPy_getPlayerActorClass() == nullptr) {
                    return;
                }
                for (int i = 0; i < 24; ++i) {
                    if (dComIfGs_getItem(i, false) == 0x00) {
                        a.progress = 1;
                        break;
                    }
                }
            },
            {}
        },
        {
            {
                "stallord_skip",
                "Stallord Skip",
                "Leave Stallord's arena through the exit without defeating Stallord.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                static bool seenStallord = false;
                if (strcmp(dComIfGp_getStartStageName(), "D_MN10A") != 0) {
                    seenStallord = false;
                    return;
                }
                if (dComIfGs_isEventBit(dSv_event_flag_c::F_0265)) {
                    seenStallord = false;
                    return;
                }
                if (fopAcM_SearchByName(fpcNm_B_DS_e) != nullptr) {
                    seenStallord = true;
                }
                if (seenStallord &&
                    dComIfGp_isEnableNextStage() &&
                    strcmp(dComIfGp_getNextStageName(), "F_SP125") == 0) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "lakebed_before_lanayru",
                "White Midna Glitch",
                "Clear the Lakebed Temple before clearing Lanayru's Twilight.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_isEventBit(dSv_event_flag_c::M_045) &&
                    !dComIfGs_isDarkClearLV(2)) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "early_hidden_village",
                "Quick Detour",
                "Rescue the Hidden Village before clearing Goron Mines.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (dComIfGs_isEventBit(dSv_event_flag_c::F_0278) &&
                    !dComIfGs_isEventBit(dSv_event_flag_c::M_031)) {
                    a.progress = 1;
                }
            },
            {}
        },
        {
            {
                "forest_temple_no_boomerang",
                "Must Have Been The Wind",
                "Complete the Forest Temple without obtaining the Gale Boomerang.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                if (!dComIfGs_isEventBit(dSv_event_flag_c::M_022)) {
                    return;
                }
                if (daPy_getPlayerActorClass() == nullptr) {
                    return;
                }
                for (int i = 0; i < 24; ++i) {
                    if (dComIfGs_getItem(i, false) == dItemNo_BOOMERANG_e) {
                        return;
                    }
                }
                a.progress = 1;
            },
            {}
        }
    };
}

AchievementSystem::AchievementSystem() : m_entries(makeEntries()) {}

AchievementSystem& AchievementSystem::get() {
    static AchievementSystem instance;
    return instance;
}

std::vector<Achievement> AchievementSystem::getAchievements() const {
    std::vector<Achievement> result;
    result.reserve(m_entries.size());
    for (const auto& e : m_entries) {
        result.push_back(e.achievement);
    }
    return result;
}

void AchievementSystem::load() {
    m_loaded = true;
    const auto filePath = dusk::ConfigPath / ACHIEVEMENTS_FILENAME;
    if (!std::filesystem::exists(filePath)) {
        return;
    }
    try {
        auto data = io::FileStream::ReadAllBytes(filePath);
        auto j = json::parse(data);
        if (!j.is_object()) {
            return;
        }
        for (auto& e : m_entries) {
            if (!j.contains(e.achievement.key)) {
                continue;
            }
            const auto& entry = j[e.achievement.key];
            if (entry.contains("progress")) {
                e.achievement.progress = entry["progress"].get<int32_t>();
            }
            if (entry.contains("unlocked")) {
                e.achievement.unlocked = entry["unlocked"].get<bool>();
            }
            if (entry.contains("extra")) {
                e.extra = entry["extra"];
            }
        }
    } catch (const std::exception&) {}
}

void AchievementSystem::save() {
    json j = json::object();
    for (const auto& e : m_entries) {
        json entry = {
            {"progress", e.achievement.progress},
            {"unlocked", e.achievement.unlocked},
        };
        if (!e.extra.is_null()) {
            entry["extra"] = e.extra;
        }
        j[e.achievement.key] = std::move(entry);
    }
    try {
        io::FileStream::WriteAllText(
            dusk::ConfigPath / ACHIEVEMENTS_FILENAME,
            j.dump(2)
        );
    } catch (const std::exception&) {}
}

void AchievementSystem::clearAll() {
    m_entries = makeEntries();
    save();
}

void AchievementSystem::signal(const char* key) {
    m_signals.insert(key);
}

bool AchievementSystem::hasSignal(const char* key) const {
    return m_signals.count(key) > 0;
}

void AchievementSystem::clearOne(const char* key) {
    for (auto& e : m_entries) {
        if (std::string(e.achievement.key) == key) {
            e.achievement.progress = 0;
            e.achievement.unlocked = false;
            e.extra = {};
            break;
        }
    }
    save();
}

void AchievementSystem::processEntry(Entry& e) {
    if (e.achievement.unlocked) {
        return;
    }
    const int32_t prevProgress = e.achievement.progress;
    e.check(e.achievement, e.extra);

    const bool progressChanged = e.achievement.progress != prevProgress;
    const bool nowUnlocked = e.achievement.isCounter ?
        e.achievement.progress >= e.achievement.goal :
        e.achievement.progress > 0;

    if (nowUnlocked) {
        e.achievement.progress = e.achievement.isCounter ? e.achievement.goal : 1;
        e.achievement.unlocked = true;
        if (getSettings().game.enableAchievementToasts) {
            ui::push_toast({
                .type = "achievement",
                .title = "Achievement Unlocked!",
                .content = e.achievement.name,
                .duration = std::chrono::seconds(5),
            });
        }
        m_dirty = true;
    } else if (progressChanged) {
        m_dirty = true;
    }
}

void AchievementSystem::tick() {
    if (!m_loaded) {
        load();
    }
    if (!dusk::IsGameLaunched) {
        return;
    }
    for (auto& e : m_entries) {
        processEntry(e);
    }
    m_signals.clear();
    if (m_dirty) {
        save();
        m_dirty = false;
    }
}

} // namespace dusk
