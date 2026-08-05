#pragma once

#include <string>

#include "../../generator/seedgen/config.hpp"
#include "../../generator/seedgen/settings.hpp"

namespace randomizer::ui {

seedgen::config::Config& GetRandomizerConfig();
void SaveRandomizerConfig();
seedgen::settings::Setting* FindSetting(const std::string& key);
bool TryCreateRandomSeed();

}  // namespace randomizer::ui
