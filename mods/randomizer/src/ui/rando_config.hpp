#pragma once

#include <filesystem>
#include <vector>
#include <mods/api.h>
#include "mods/svc/ui.h"

// Forward declaration
namespace randomizer::seedgen::config {
class Config;
}
class dFile_select_c;

namespace randomizer::ui {

enum dialogSelectModeState : uint8_t {
    SelectReady,
    SelectWait,
    SelectVanilla,
    SelectRandomizer
};
extern dialogSelectModeState g_dialogSelectModeState;

struct FileSelectGateWindowCtx {
    UiWindowHandle window_handle{};
    bool is_proceed{false};
};
extern FileSelectGateWindowCtx g_file_select_window_ctx;

void SaveNewRandomizerPreset(const std::string& presetName, bool overwriteExisting = false);
void ApplyExistingRandomizerPreset(const std::filesystem::path& presetFilePath);
void CopyPermalinkToClipboard();
void PastePermalinkFromClipboard();

ModResult buildMenuTab();
ModResult buildFileSelectGateMenu(dFile_select_c*);

std::filesystem::path GetRandomizerPath();
std::filesystem::path GetRandomizerSettingsPath();
std::filesystem::path GetRandomizerPreferencesPath();
std::filesystem::path GetRandomizerSeedsPath();
std::filesystem::path GetRandomizerPresetsPath();

}
