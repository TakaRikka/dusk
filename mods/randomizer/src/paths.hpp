#pragma once

#include <filesystem>

namespace randomizer::paths {

std::filesystem::path GetRandomizerPath();
std::filesystem::path GetRandomizerSettingsPath();
std::filesystem::path GetRandomizerPreferencesPath();
std::filesystem::path GetRandomizerPresetsPath();
std::filesystem::path GetRandomizerSeedsPath();

}  // namespace randomizer::paths
