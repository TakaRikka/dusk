#pragma once

#include <filesystem>

namespace randomizer::paths {

// Root of the randomizer's writable data (settings, preferences, generated seeds),
// under the host's configured data path: <data>/randomizer/.
std::filesystem::path GetRandomizerPath();
std::filesystem::path GetRandomizerSettingsPath();
std::filesystem::path GetRandomizerPreferencesPath();
std::filesystem::path GetRandomizerSeedsPath();

}  // namespace randomizer::paths
