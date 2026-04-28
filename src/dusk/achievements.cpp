#include "dusk/achievements.h"
#include "dusk/io.hpp"
#include "dusk/main.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_player.h"
#include "d/d_demo.h"
#include "f_pc/f_pc_name.h"
#include "f_op/f_op_actor_mng.h"

#include <filesystem>
#include <algorithm>

namespace dusk {

using json = nlohmann::json;

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
        {
            {
                "hero_of_twilight",
                "Hero of Twilight",
                "Deliver the finishing blow to Ganondorf.",
                AchievementCategory::Story,
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
                "back_in_time",
                "Back in Time",
                "Perform the Back in Time glitch to play on the title screen.",
                AchievementCategory::Glitched,
                false, 0, 0, false
            },
            [](Achievement& a, json&) {
                static int titleNoDemoFrames = 0;
                if (fopAcM_SearchByName(fpcNm_TITLE_e) == nullptr) {
                    titleNoDemoFrames = 0;
                    return;
                }
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                if (link != nullptr && dDemo_c::getMode() == 0) {
                    if (++titleNoDemoFrames >= 60) {
                        a.progress = 1;
                    }
                } else {
                    titleNoDemoFrames = 0;
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
                "ultimate_delivery",
                "The Ultimate Delivery",
                "Have all 16 postman letters at the same time.",
                AchievementCategory::Glitched,
                true, 16, 0, false
            },
            [](Achievement& a, json&) {
                a.progress = dMeter2Info_getRecieveLetterNum();
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
        }
    };
}

AchievementSystem::AchievementSystem() : m_entries(makeEntries()) {}

AchievementSystem& AchievementSystem::get() {
    static AchievementSystem instance;
    return instance;
}

std::string AchievementSystem::consumePendingUnlock() {
    std::string msg = std::move(m_pendingUnlocks.front());
    m_pendingUnlocks.pop();
    return msg;
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
        auto data = io::FileStream::ReadAllBytes(filePath.string().c_str());
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
            (dusk::ConfigPath / ACHIEVEMENTS_FILENAME).string().c_str(),
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
        m_pendingUnlocks.push(e.achievement.name);
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
