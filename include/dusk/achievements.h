#pragma once

#include <cstdint>
#include <functional>
#include <queue>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

namespace dusk {

enum class AchievementCategory : uint8_t {
    Story,
    Collection,
    Challenge,
    Minigame,
    Glitched
};

struct Achievement {
    const char* key;
    const char* name;
    const char* description;
    AchievementCategory category;
    bool isCounter;
    int32_t goal;
    int32_t progress;
    bool unlocked;
};

// Responsible for updating a.progress.
// Use extra for any per-achievement state that must survive across frames or sessions, extra is saved
using AchievementCheckFn = std::function<void(Achievement& a, nlohmann::json& extra)>;

class AchievementSystem {
public:
    static AchievementSystem& get();

    void load();
    void save();
    void tick();
    void clearAll();

    std::vector<Achievement> getAchievements() const;
    bool hasPendingUnlock() const { return !m_pendingUnlocks.empty(); }
    std::string consumePendingUnlock();

private:
    struct Entry {
        Achievement achievement;
        AchievementCheckFn check;
        nlohmann::json extra;
    };

    AchievementSystem();
    static std::vector<Entry> makeEntries();
    void processEntry(Entry& e);

    std::vector<Entry> m_entries;
    bool m_loaded = false;
    bool m_dirty = false;
    std::queue<std::string> m_pendingUnlocks;
};

} // namespace dusk
