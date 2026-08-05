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
