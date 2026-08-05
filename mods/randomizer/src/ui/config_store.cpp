#include "config_store.hpp"

#include <mods/svc/log.hpp>

#include "../../generator/seedgen/seed.hpp"
#include "../paths.hpp"

namespace randomizer::ui {

seedgen::config::Config& GetRandomizerConfig() {
    static seedgen::config::Config s_config{paths::GetRandomizerSettingsPath(),
                                            paths::GetRandomizerPreferencesPath()};
    return s_config;
}

void SaveRandomizerConfig() {
    GetRandomizerConfig().WriteToFile(paths::GetRandomizerSettingsPath(),
                                      paths::GetRandomizerPreferencesPath());
}

seedgen::settings::Setting* FindSetting(const std::string& key) {
    if (key.empty()) {
        return nullptr;
    }
    auto& settings = GetRandomizerConfig().GetSettings();
    auto& map = settings.GetMap();
    auto it = map.find(key);
    if (it == map.end()) {
        mods::log::error("randomizer: failed to get settings key: {}", key);
        return nullptr;
    }
    return &it->second;
}

bool TryCreateRandomSeed() {
    auto& config = GetRandomizerConfig();
    if (config.GetSeed().empty()) {
        config.SetSeed(seedgen::seed::GenerateSeed());
        SaveRandomizerConfig();
        return true;
    }
    return false;
}

}  // namespace randomizer::ui
