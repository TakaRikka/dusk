#pragma once

#include <filesystem>
#include <vector>
#include <mods/api.h>

// Forward declaration
namespace randomizer::seedgen::config {
class Config;
}
class dFile_select_c;

namespace randomizer::ui {

void SaveNewRandomizerPreset(const std::string& presetName, bool overwriteExisting = false);
void ApplyExistingRandomizerPreset(const std::filesystem::path& presetFilePath);
void CopyPermalinkToClipboard();
void PastePermalinkFromClipboard();

ModResult buildMenuTab();

std::filesystem::path GetRandomizerPath();
std::filesystem::path GetRandomizerSettingsPath();
std::filesystem::path GetRandomizerPreferencesPath();
std::filesystem::path GetRandomizerSeedsPath();
std::filesystem::path GetRandomizerPresetsPath();

}
