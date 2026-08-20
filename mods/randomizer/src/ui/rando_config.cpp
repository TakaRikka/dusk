#include "rando_config.hpp"

#include <mods/svc/log.hpp>
#include <mods/svc/ui.hpp>

#include "../session.hpp"
#include "../tools.h"
#include "../paths.hpp"
#include "../../generator/seedgen/seed.hpp"
#include "../../generator/utility/string.hpp"
#include "../../generator/utility/text.hpp"
#include "../../generator/utility/yaml.hpp"

#include "rando_seed_generation.hpp"
#include "config_store.hpp"

#include <mutex>
#include <thread>
#include <map>
#include <unordered_set>

#include "../randomizer_context.hpp"
#include "d/d_file_select.h"

namespace randomizer::ui {

seedgen::settings::Setting* FindSetting(const std::string& key) {
    if (key.empty()) {
        mods::log::error("Key is empty! Unable to find setting.");
    }

    // TODO: handle multi-world selection
    auto& settings = GetRandomizerConfig().GetSettings();
    try {
        return &settings.GetMap().at(key);
    } catch (std::exception e) {
        mods::log::error("Failed to get Settings Key: {}", key);
    }
}

const std::vector<std::pair<std::string, std::string>>& GetStartingInventoryLayoutOrder() {
    static const std::vector<std::pair<std::string, std::string>> layoutOrder = {
        // { display name , logic item name }
        {"Shadow Crystal", "Shadow Crystal"},
        {"Horse Call", "Horse Call"},
        {"Fishing Rod", "Progressive Fishing Rod"},
        {"Slingshot", "Slingshot"},
        {"Lantern", "Lantern"},
        {"Gale Boomerang", "Gale Boomerang"},
        {"Iron Boots", "Iron Boots"},
        {"Bow", "Progressive Bow"},
        {"Hawkeye", "Hawkeye"},
        {"Bomb Bags", "Bomb Bag"},
        {"Giant Bomb Bags", "Giant Bomb Bag"},
        {"Clawshot", "Progressive Clawshot"},
        {"Spinner", "Spinner"},
        {"Ball and Chain", "Ball and Chain"},
        {"Dominion Rod", "Progressive Dominion Rod"},
        {"Empty Bottle", "Empty Bottle"},
        {"Auru's Memo", "Aurus Memo"},
        {"Ashei's Sketch", "Asheis Sketch"},
        {"Sky Book", "Progressive Sky Book"},
        {"Sword", "Progressive Sword"},
        {"Ordon Shield", "Ordon Shield"},
        {"Hylian Shield", "Hylian Shield"},
        {"Zora Armor", "Zora Armor"},
        {"Magic Armor", "Magic Armor"},
        {"Wallet", "Progressive Wallet"},
        {"Hidden Skills", "Progressive Hidden Skill"},
        {"Poe Souls", "Poe Soul"},
        {"Fused Shadows", "Progressive Fused Shadow"},
        {"Mirror Shards", "Progressive Mirror Shard"},
        {"Gate Keys", "Gate Keys"},
        {"Gerudo Desert Bulblin Camp Key", "Gerudo Desert Bulblin Camp Key"},
        {"Forest Temple Small Keys", "Forest Temple Small Key"},
        {"Goron Mines Small Keys", "Goron Mines Small Key"},
        {"Lakebed Temple Small Keys", "Lakebed Temple Small Key"},
        {"Arbiter's Grounds Small Keys", "Arbiters Grounds Small Key"},
        {"Snowpeak Ruins Small Keys", "Snowpeak Ruins Small Key"},
        {"Ordon Pumpkin", "Ordon Pumpkin"},
        {"Ordon Cheese", "Ordon Cheese"},
        {"Temple of Time Small Keys", "Temple of Time Small Key"},
        {"City in the Sky Small Keys", "City in the Sky Small Key"},
        {"Palace of Twilight Small Keys", "Palace of Twilight Small Key"},
        {"Hyrule Castle Small Keys", "Hyrule Castle Small Key"},
        {"Forest Temple Big Key", "Forest Temple Big Key"},
        {"Goron Mines Key Shards", "Goron Mines Key Shard"},
        {"Lakebed Temple Big Key", "Lakebed Temple Big Key"},
        {"Arbiter's Grounds Big Key", "Arbiters Grounds Big Key"},
        {"Snowpeak Ruins Bedroom Key", "Snowpeak Ruins Bedroom Key"},
        {"Temple of Time Big Key", "Temple of Time Big Key"},
        {"City in the Sky Big Key", "City in the Sky Big Key"},
        {"Palace of Twilight Big Key", "Palace of Twilight Big Key"},
        {"Hyrule Castle Big Key", "Hyrule Castle Big Key"},
        {"Gerudo Desert Portal", "Gerudo Desert Portal"},
        {"Mirror Chamber Portal", "Mirror Chamber Portal"},
        {"Snowpeak Portal", "Snowpeak Portal"},
        {"Sacred Grove Portal", "Sacred Grove Portal"},
        {"Bridge of Eldin Portal", "Bridge of Eldin Portal"},
        {"Upper Zora's River Portal", "Upper Zoras River Portal"}
    };
    return layoutOrder;
}

UiMenuTabHandle g_menu_tab{};

FileSelectGateWindowCtx g_file_select_window_ctx{};

namespace {
// Control Helpers
ModResult add_button(UiElementHandle pane, const char* label, const char* help_rml,
    UiPressedFn on_pressed, void* userdata = nullptr, UiElementHandle* out_handle = nullptr)
{
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_BUTTON;
    desc.label = label;
    desc.help_rml = help_rml;
    desc.on_pressed = on_pressed;
    desc.user_data = userdata;
    return session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

void add_section(UiElementHandle pane, const char* label) {
    session::svc_mng.ui->pane_add_section(session::svc_mng.mod_ctx, pane, label);
}

void add_string_input(UiElementHandle pane, const char* label, const char* help_rml,
    int32_t max_length, UiControlGetFn getFn, UiControlSetFn setFn, UiElementHandle* out_handle = nullptr)
{
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_STRING;
    desc.label = label;
    desc.help_rml = help_rml;
    desc.binding = UI_BINDING_CALLBACKS;
    desc.get = getFn;
    desc.set = setFn;
    desc.max_length = max_length;
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

void add_select(UiElementHandle pane, const char* label, const char* help_rml, const char** options,
    size_t option_count, UiControlGetFn getFn, UiControlSetFn setFn, UiElementHandle* out_handle = nullptr)
{
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_SELECT;
    desc.label = label;
    desc.help_rml = help_rml;
    desc.binding = UI_BINDING_CALLBACKS;
    desc.get = getFn;
    desc.set = setFn;
    desc.options = options;
    desc.option_count = option_count;
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

void add_number_input(UiElementHandle pane, const char* label, const char* help_rml, int64_t min,
    int64_t max, int64_t step, UiControlGetFn getFn, UiControlSetFn setFn,
    UiElementHandle* out_handle = nullptr)
{
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_NUMBER;
    desc.label = label;
    desc.help_rml = help_rml;
    desc.binding = UI_BINDING_CALLBACKS;
    desc.get = getFn;
    desc.set = setFn;
    desc.min = min;
    desc.max = max;
    desc.step = step;
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

void add_select_setting(UiElementHandle pane, const char* key, UiElementHandle* out_handle = nullptr)
{
    auto setting = FindSetting(key);
    auto info = setting->GetInfo();

    std::vector<const char*> optionsList;
    std::string help_rml = "";
    for (size_t i = 0; i < info->GetOptions().size(); ++i) {
        optionsList.push_back(info->GetOptions()[i].c_str());
        help_rml += fmt::format("<br/><span style=\"color: #C2A42D;\">{}</span>: {}", info->GetOptions()[i], info->GetDescriptions()[i]);
    }

    auto getFn = [](ModContext*, void* user_data, UiControlValue* out_value) {
        auto setting = FindSetting(static_cast<const char*>(user_data));
        const auto& options = setting->GetInfo()->GetOptions();

        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == setting->GetCurrentOption()) {
                out_value->int_value = i;
                return;
            }
        }

        // default
        out_value->int_value = 0;
    };

    auto setFn = [](ModContext*, void* user_data, const UiControlValue* value) {
        auto setting = FindSetting(static_cast<const char*>(user_data));
        const auto& options = setting->GetInfo()->GetOptions();

        if (value->int_value >= 0 && value->int_value < options.size()) {
            setting->SetCurrentOption(options[value->int_value]);
            SaveRandomizerConfig();
        }
    };

    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_SELECT;
    desc.label = key;
    desc.help_rml = help_rml.c_str();
    desc.binding = UI_BINDING_CALLBACKS;
    desc.get = getFn;
    desc.set = setFn;
    desc.user_data = (void*)key;
    desc.options = optionsList.data();
    desc.option_count = optionsList.size();
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

void add_select_number_setting(UiElementHandle pane, const char* key, UiElementHandle* out_handle = nullptr)
{
    auto setting = FindSetting(key);
    auto info = setting->GetInfo();

    std::vector<const char*> optionsList;
    for (size_t i = 0; i < info->GetOptions().size(); ++i) {
        optionsList.push_back(info->GetOptions()[i].c_str());
    }

    auto getFn = [](ModContext*, void* user_data, UiControlValue* out_value) {
        auto setting = FindSetting(static_cast<const char*>(user_data));
        const auto& options = setting->GetInfo()->GetOptions();

        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == setting->GetCurrentOption()) {
                out_value->int_value = i;
                return;
            }
        }

        // default
        out_value->int_value = 0;
    };

    auto setFn = [](ModContext*, void* user_data, const UiControlValue* value) {
        auto setting = FindSetting(static_cast<const char*>(user_data));
        const auto& options = setting->GetInfo()->GetOptions();

        if (value->int_value >= 0 && value->int_value < options.size()) {
            setting->SetCurrentOption(options[value->int_value]);
            SaveRandomizerConfig();
        }
    };

    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_SELECT;
    desc.label = key;
    desc.help_rml = "";
    desc.binding = UI_BINDING_CALLBACKS;
    desc.get = getFn;
    desc.set = setFn;
    desc.user_data = (void*)key;
    desc.options = optionsList.data();
    desc.option_count = optionsList.size();
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

void add_number_setting(UiElementHandle pane, const char* label, const char* key,
    UiPredicateFn isDisabledFn, UiElementHandle* out_handle = nullptr)
{
    std::string fullOptionalKey = key;

    // check if setting exists
    auto randoSettings = seedgen::settings::GetAllSettingsInfo();
    if (!randoSettings->contains(fullOptionalKey)) {
        return;
    }
    auto curSetting = FindSetting(fullOptionalKey);
    const auto& options = curSetting->GetInfo()->GetOptions();
    int64_t min = std::stoi(options.front());
    int64_t max = std::stoi(options.back());

    auto getFn = [](ModContext*, void* user_data, UiControlValue* out_value) {
        std::string fullOptionalKey = static_cast<const char*>(user_data);

        // check if setting exists
        auto randoSettings = seedgen::settings::GetAllSettingsInfo();
        if (!randoSettings->contains(fullOptionalKey)) {
            return;
        }

        auto setting = FindSetting(fullOptionalKey);
        out_value->int_value = setting->GetCurrentOptionAsNumber();
    };

    auto setFn = [](ModContext*, void* user_data, const UiControlValue* value) {
        std::string fullOptionalKey = static_cast<const char*>(user_data);

        // check if setting exists
        auto randoSettings = seedgen::settings::GetAllSettingsInfo();
        if (!randoSettings->contains(fullOptionalKey)) {
            return;
        }

        auto setting = FindSetting(fullOptionalKey);
        setting->SetCurrentOption(std::to_string(value->int_value));
    };

    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_NUMBER;
    desc.label = label;
    desc.help_rml = "";
    desc.binding = UI_BINDING_CALLBACKS;
    desc.get = getFn;
    desc.set = setFn;
    desc.is_disabled = isDisabledFn;
    desc.user_data = (void*)key;
    desc.min = min;
    desc.max = max;
    desc.step = 1;
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

// Seed Management Tab
ModResult buildSeedManagementTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    add_button(leftPane,
        "Generate Seed",
        "Generate a Randomizer seed using the current configuration options, and the supplied seed string.",
        [](ModContext*, void*) {
            if (TryCreateRandomSeed()) {
                mods::log::info("Created new Seed for generator.");
            }
            GenerateRandomizerSeed();
        });

    add_string_input(leftPane,
        "Seed String",
        "Current value of the seed used by the randomizer for generation. Leave blank for a random value.",
        31,
        [](ModContext*, void*, UiControlValue* out_value) {
            static char buffer[32];
            strncpy(buffer,GetRandomizerConfig().GetSeed().c_str(),31);
            out_value->string_value = buffer;
        },
        [](ModContext*, void*, const UiControlValue* value) {
            GetRandomizerConfig().SetSeed(value->string_value);
            SaveRandomizerConfig();
        });

    {
        std::filesystem::path seed_dir = paths::GetRandomizerSeedsPath();
        if (!std::filesystem::exists(seed_dir))
            std::filesystem::create_directory(seed_dir);

        std::string help_rml = "Select a seed above to delete it.";

        std::vector<std::string> seedHashes;
        for (const auto& entry : std::filesystem::directory_iterator(seed_dir)) {
            if (entry.is_directory()) {
                seedHashes.push_back(entry.path().filename().string());
            }
        }

        std::vector<const char*> availableSeeds;
        for (const auto& hash : seedHashes) {
            availableSeeds.push_back(hash.c_str());
        }

        add_select(leftPane,
            "Delete Seeds",
            help_rml.c_str(),
            availableSeeds.data(),
            availableSeeds.size(),
            [](ModContext*, void*, UiControlValue* out_value) {
                out_value->int_value = 0;
            },
            [](ModContext*, void*, const UiControlValue* value) {
                int idx = 0;
                for (const auto& entry : std::filesystem::directory_iterator(paths::GetRandomizerSeedsPath())) {
                    if (entry.is_directory()) {
                        if (idx == value->int_value) {
                            std::string hash = entry.path().filename().string();
                            if (randomizer_GetContext().mHash == hash) {
                                randomizer_GetContext() = RandomizerContext{};
                            }
                            std::filesystem::remove_all(entry);
                            break;
                        }
                        idx++;
                    }
                }
            });
    }

    add_section(leftPane, "Permalink");
    {
        std::string help_rml = "Copy your current settings permalink to share with others.<br/>";
        help_rml += fmt::format(
            "<br/>Current Permalink:<br/><span style=\"word-break: break-all;\">{}</span>",
            GetRandomizerConfig().GetPermalink());
        add_button(leftPane,
            "Copy Permalink",
            help_rml.c_str(),
            [](ModContext*, void*) {
                mods::ui::set_clipboard_text(GetRandomizerConfig().GetPermalink().data());

                UiToastDesc desc = UI_TOAST_DESC_INIT;
                desc.title_rml = "Randomizer";
                desc.body_rml = "Permalink Copied";
                desc.duration_ms = 3000;
                session::svc_mng.ui->push_toast(session::svc_mng.mod_ctx, &desc);
            });
    }
    add_button(leftPane,
        "Paste Permalink",
        "Paste in a permalink from your clipboard. This will overwrite your current settings.",
        [](ModContext*, void*) {
            std::string text;
            ModResult rt = mods::ui::get_clipboard_text(text);
            if (rt != MOD_OK) {
                return;
            }

            auto result = GetRandomizerConfig().LoadFromPermalink(text);
            if (result.has_value()) {
                mods::log::error("Failed to load permalink: {}", result.value());
                return;
            }

            SaveRandomizerConfig();

            UiToastDesc desc = UI_TOAST_DESC_INIT;
            desc.title_rml = "Randomizer";
            desc.body_rml = "Applied Permalink";
            desc.duration_ms = 3000;
            session::svc_mng.ui->push_toast(session::svc_mng.mod_ctx, &desc);
        });

    add_section(leftPane, "Presets");

    add_button(leftPane,
        "Save Current Settings as Preset",
        "Save the current settings to your list of presets.",
        [](ModContext*, void*) {
            // TODO
        });

    add_button(leftPane,
        "Load Preset",
        "Choose an existing preset to load from.",
        [](ModContext*, void*) {
            // TODO
        });

    return MOD_OK;
}

ModResult updateSeedManagementTab(ModContext* ctx, void*, ModError*) {
    return MOD_OK;
}

// Seed Options Tab
ModResult buildSeedOptionsTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    add_button(leftPane,
        "Reset Settings to Default",
        "Reset all settings to their default values. This will also clear starting items and excluded locations.",
        [](ModContext*, void*) {
            GetRandomizerConfig().ResetSettingsToDefault();
            SaveRandomizerConfig();
        });

    add_section(leftPane, "Logic Settings");
    add_select_setting(leftPane, "Logic Rules");

    add_section(leftPane, "Access Options");
    add_select_setting(leftPane, "Hyrule Barrier Requirements");

    // pretty ugly way of doing this, but it works for now
    add_number_setting(leftPane,
        "Required Fused Shadows",
        "Hyrule Barrier Fused Shadows",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Barrier Requirements");
            if (setting->GetCurrentOption() != "Fused Shadows") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Mirror Shards",
        "Hyrule Barrier Mirror Shards",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Barrier Requirements");
            if (setting->GetCurrentOption() != "Mirror Shards") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Dungeons",
        "Hyrule Barrier Dungeons",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Barrier Requirements");
            if (setting->GetCurrentOption() != "Dungeons") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Poe Souls",
        "Hyrule Barrier Poe Souls",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Barrier Requirements");
            if (setting->GetCurrentOption() != "Poe Souls") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Hearts",
        "Hyrule Barrier Hearts",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Barrier Requirements");
            if (setting->GetCurrentOption() != "Hearts") {
                return true;
            }

            return false;
        });

    add_select_setting(leftPane, "Palace of Twilight Requirements");
    add_select_setting(leftPane, "Faron Woods Logic");
    add_select_setting(leftPane, "Mirror Chamber Access");

    add_section(leftPane, "Shuffles");
    add_select_setting(leftPane, "Golden Bugs");
    add_select_setting(leftPane, "Sky Characters");
    add_select_setting(leftPane, "Gifts From NPCs");
    add_select_setting(leftPane, "Shop Items");
    add_select_setting(leftPane, "Hidden Skills");
    add_select_setting(leftPane, "Hidden Rupees");
    add_select_setting(leftPane, "Freestanding Rupees");
    add_select_setting(leftPane, "Poe Souls");
    add_select_setting(leftPane, "Ilia Memory Quest");
    add_select_setting(leftPane, "Item Scarcity");
    add_select_setting(leftPane, "Trap Item Frequency");

    add_section(leftPane, "Dungeon Items");
    add_select_setting(leftPane, "Small Keys");
    add_select_setting(leftPane, "Big Keys");
    add_select_setting(leftPane, "Maps and Compasses");

    add_select_setting(leftPane, "Hyrule Castle Big Key Requirements");
    // pretty ugly way of doing this, but it works for now
    add_number_setting(leftPane,
        "Required Fused Shadows",
        "Hyrule Castle Big Key Fused Shadows",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Castle Big Key Requirements");
            if (setting->GetCurrentOption() != "Fused Shadows") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Mirror Shards",
        "Hyrule Castle Big Key Mirror Shards",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Castle Big Key Requirements");
            if (setting->GetCurrentOption() != "Mirror Shards") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Dungeons",
        "Hyrule Castle Big Key Dungeons",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Castle Big Key Requirements");
            if (setting->GetCurrentOption() != "Dungeons") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Poe Souls",
        "Hyrule Castle Big Key Poe Souls",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Castle Big Key Requirements");
            if (setting->GetCurrentOption() != "Poe Souls") {
                return true;
            }

            return false;
        });
    add_number_setting(leftPane,
        "Required Hearts",
        "Hyrule Castle Big Key Hearts",
        [](ModContext*, void*) {
            auto setting = FindSetting("Hyrule Castle Big Key Requirements");
            if (setting->GetCurrentOption() != "Hearts") {
                return true;
            }

            return false;
        });

    add_select_setting(leftPane, "Dungeon Rewards Can Be Anywhere");
    add_select_setting(leftPane, "No Small Keys on Bosses");
    add_select_setting(leftPane, "Unrequired Dungeons Are Barren");

    add_section(leftPane, "Timesavers");
    add_select_setting(leftPane, "Skip Prologue");
    add_select_setting(leftPane, "Faron Twilight Cleared");
    add_select_setting(leftPane, "Eldin Twilight Cleared");
    add_select_setting(leftPane, "Lanayru Twilight Cleared");
    add_select_setting(leftPane, "Skip Midna's Desparate Hour");
    add_select_setting(leftPane, "Skip Minor Cutscenes");
    add_select_setting(leftPane, "Skip Major Cutscenes");
    add_select_setting(leftPane, "Unlock Map Regions");
    add_select_setting(leftPane, "Open Door of Time");
    add_select_setting(leftPane, "Active Goron Mines Magnets");
    add_select_setting(leftPane, "Lower Hyrule Castle Chandelier");
    add_select_setting(leftPane, "Skip Bridge Donation");

    add_section(leftPane, "Entrance Randomizer Settings");
    add_select_setting(leftPane, "Randomize Starting Spawn");
    add_select_setting(leftPane, "Randomize Dungeon Entrances");
    add_select_setting(leftPane, "Randomize Boss Entrances");
    add_select_setting(leftPane, "Randomize Grotto Entrances");
    add_select_setting(leftPane, "Randomize Cave Entrances");
    add_select_setting(leftPane, "Randomize Interior Entrances");
    add_select_setting(leftPane, "Randomize Overworld Entrances");
    add_select_setting(leftPane, "Decouple Double Door Entrances");
    add_select_setting(leftPane, "Decouple Entrances");

    add_section(leftPane, "Additional Settings");
    add_select_setting(leftPane, "Starting Time of Day");
    add_select_setting(leftPane, "Logic Transform Anywhere");
    add_select_setting(leftPane, "Logic Increase Wallet Capacity");
    add_select_setting(leftPane, "Logic Damage Multiplier");

    add_section(leftPane, "Dungeon Entrance Settings");
    add_select_setting(leftPane, "Lakebed Does Not Require Water Bombs");
    add_select_setting(leftPane, "Arbiters Does Not Require Bulblin Camp");
    add_select_setting(leftPane, "Snowpeak Does Not Require Reekfish Scent");
    add_select_setting(leftPane, "Sacred Grove Does Not Require Skull Kid");
    add_select_setting(leftPane, "City Does Not Require Filled Skybook");
    add_select_setting(leftPane, "Goron Mines Entrance");
    add_select_setting(leftPane, "Temple of Time Sword Requirement");

    add_section(leftPane, "Tricks");
    add_select_setting(leftPane, "Back Slice as Sword");
    add_select_setting(leftPane, "Ball and Chain Webs");

    return MOD_OK;
}

// Hints Tab
ModResult buildHintsTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    add_section(leftPane, "Path Hints");
    add_select_number_setting(leftPane, "Number of Path Hints");
    add_select_setting(leftPane, "Path Hints on Midna");
    add_select_setting(leftPane, "Path Hints on Hint Signs");

    add_section(leftPane, "Barren Hints");
    add_select_number_setting(leftPane, "Number of Barren Hints");
    add_select_setting(leftPane, "Barren Hints on Midna");
    add_select_setting(leftPane, "Barren Hints on Hint Signs");

    add_section(leftPane, "Item Hints");
    add_select_number_setting(leftPane, "Number of Item Hints");
    add_select_setting(leftPane, "Item Hints on Midna");
    add_select_setting(leftPane, "Item Hints on Hint Signs");

    add_section(leftPane, "Location Hints");
    add_select_number_setting(leftPane, "Number of Location Hints");
    add_select_setting(leftPane, "Location Hints on Midna");
    add_select_setting(leftPane, "Location Hints on Hint Signs");
    add_select_setting(leftPane, "Prioritize Remote Location Hints");

    return MOD_OK;
}

// Starting Inventory Tab
UiElementHandle startingInventoryRmlElem = 0;

template <int Max = 1>
void on_pressed_starting_inventory_item(ModContext*, void* userdata) {
    const char* itemName = static_cast<const char*>(userdata);
    auto& inventory = GetRandomizerConfig().GetSettings().GetModifiableStartingInventory();
    int newCount = inventory[itemName] + 1;
    if (newCount > Max) {
        inventory.erase(itemName);
    } else {
        inventory.at(itemName) = newCount;
    }
    SaveRandomizerConfig();

    mDoAud_seStartMenu(Z2SE_SY_NAME_CURSOR);
}

template <int Max = 1>
void add_starting_inventory_item(UiElementHandle leftPane, const char* displayName, const char* itemName = "") {
    if (std::strcmp(itemName, "") == 0) {
        itemName = displayName;
    }

    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_BUTTON;
    desc.label = displayName;
    desc.help_rml = "";
    desc.binding = UI_BINDING_CALLBACKS;
    desc.on_pressed = on_pressed_starting_inventory_item<Max>;
    desc.user_data = (void*)itemName;
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, leftPane, &desc, nullptr);
};

ModResult buildStartingInventoryTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    add_button(leftPane,
        "Clear Selected Starting Items",
        "",
        [](ModContext*, void*) {
            auto& inventory = GetRandomizerConfig().GetSettings().GetModifiableStartingInventory();
            inventory.clear();
            SaveRandomizerConfig();
        });

    add_section(leftPane, "Main Items");
    add_starting_inventory_item(leftPane, "Shadow Crystal");
    add_starting_inventory_item(leftPane, "Horse Call");
    add_starting_inventory_item<2>(leftPane, "Fishing Rod", "Progressive Fishing Rod");
    add_starting_inventory_item(leftPane, "Slingshot");
    add_starting_inventory_item(leftPane, "Lantern");
    add_starting_inventory_item(leftPane, "Gale Boomerang");
    add_starting_inventory_item(leftPane, "Iron Boots");
    add_starting_inventory_item<3>(leftPane, "Bow", "Progressive Bow");
    add_starting_inventory_item(leftPane, "Hawkeye");
    add_starting_inventory_item<3>(leftPane, "Bomb Bags", "Bomb Bag");
    add_starting_inventory_item(leftPane, "Giant Bomb Bags", "Giant Bomb Bag");
    add_starting_inventory_item<2>(leftPane, "Clawshot", "Progressive Clawshot");
    add_starting_inventory_item(leftPane, "Spinner");
    add_starting_inventory_item(leftPane, "Ball and Chain");
    add_starting_inventory_item<2>(leftPane, "Dominion Rod", "Progressive Dominion Rod");
    add_starting_inventory_item(leftPane, "Empty Bottle");
    add_starting_inventory_item(leftPane, "Auru's Memo", "Aurus Memo");
    add_starting_inventory_item(leftPane, "Ashei's Sketch", "Asheis Sketch");
    add_starting_inventory_item<7>(leftPane, "Sky Book", "Progressive Sky Book");

    add_section(leftPane, "Gear Screen");
    add_starting_inventory_item<4>(leftPane, "Sword", "Progressive Sword");
    add_starting_inventory_item(leftPane, "Ordon Shield");
    add_starting_inventory_item(leftPane, "Hylian Shield");
    add_starting_inventory_item(leftPane, "Zora Armor");
    add_starting_inventory_item(leftPane, "Magic Armor");
    add_starting_inventory_item<2>(leftPane, "Wallet", "Progressive Wallet");
    add_starting_inventory_item<7>(leftPane, "Hidden Skills", "Progressive Hidden Skill");
    add_starting_inventory_item<60>(leftPane, "Poe Souls", "Poe Soul");
    add_starting_inventory_item<3>(leftPane, "Fused Shadows", "Progressive Fused Shadow");
    add_starting_inventory_item<4>(leftPane, "Mirror Shards", "Progressive Mirror Shard");

    add_section(leftPane, "Overworld Keys");
    add_starting_inventory_item(leftPane, "Gate Keys");
    add_starting_inventory_item(leftPane, "Gerudo Desert Bulblin Camp Key");

    add_section(leftPane, "Dungeon Items");
    add_starting_inventory_item<4>(leftPane, "Forest Temple Small Keys", "Forest Temple Small Key");
    add_starting_inventory_item<3>(leftPane, "Goron Mines Small Keys", "Goron Mines Small Key");
    add_starting_inventory_item<3>(leftPane, "Lakebed Temple Small Keys", "Lakebed Temple Small Key");
    add_starting_inventory_item<5>(leftPane, "Arbiter's Grounds Small Keys", "Arbiters Grounds Small Key");
    add_starting_inventory_item<4>(leftPane, "Snowpeak Ruins Small Keys", "Snowpeak Ruins Small Key");
    add_starting_inventory_item(leftPane, "Ordon Pumpkin");
    add_starting_inventory_item(leftPane, "Ordon Cheese");
    add_starting_inventory_item<4>(leftPane, "Temple of Time Small Keys", "Temple of Time Small Key");
    add_starting_inventory_item<1>(leftPane, "City in the Sky Small Keys", "City in the Sky Small Key");
    add_starting_inventory_item<7>(leftPane, "Palace of Twilight Small Keys", "Palace of Twilight Small Key");
    add_starting_inventory_item<3>(leftPane, "Hyrule Castle Small Keys", "Hyrule Castle Small Key");

    add_starting_inventory_item(leftPane, "Forest Temple Big Key");
    add_starting_inventory_item<3>(leftPane, "Goron Mines Key Shards", "Goron Mines Key Shard");
    add_starting_inventory_item(leftPane, "Lakebed Temple Big Key");
    add_starting_inventory_item(leftPane, "Arbiter's Grounds Big Key", "Arbiters Grounds Big Key");
    add_starting_inventory_item(leftPane, "Snowpeak Ruins Bedroom Key");
    add_starting_inventory_item(leftPane, "Temple of Time Big Key");
    add_starting_inventory_item(leftPane, "City in the Sky Big Key");
    add_starting_inventory_item(leftPane, "Palace of Twilight Big Key");
    add_starting_inventory_item(leftPane, "Hyrule Castle Big Key");

    add_section(leftPane, "Warp Portals");
    add_starting_inventory_item(leftPane, "Gerudo Desert Portal");
    add_starting_inventory_item(leftPane, "Mirror Chamber Portal");
    add_starting_inventory_item(leftPane, "Snowpeak Portal");
    add_starting_inventory_item(leftPane, "Sacred Grove Portal");
    add_starting_inventory_item(leftPane, "Bridge of Eldin Portal");
    add_starting_inventory_item(leftPane, "Upper Zora's River Portal", "Upper Zoras River Portal");

    add_section(rightPane, "Selected Starting Items");
    session::svc_mng.ui->pane_add_rml(session::svc_mng.mod_ctx, rightPane, "", &startingInventoryRmlElem);

    return MOD_OK;
}

ModResult updateStartingInventoryTab(ModContext* ctx, void*, ModError*) {
    const auto& inventory = GetRandomizerConfig().GetSettings().GetStartingInventory();
    const auto& layoutOrder = GetStartingInventoryLayoutOrder();

    std::string rightPaneRml = "";
    for (const auto& [itemText, itemName] : layoutOrder) {
        if (!inventory.contains(itemName)) {
            continue;
        }

        int count = inventory.at(itemName);
        if (count <= 0) {
            continue;
        }

        // If we have a prettier name for the item, prioritize that
        std::string prettyItemName = fmt::format("{} x{}", itemName, count);
        if (randomizer::textObjectExists(prettyItemName)) {
            rightPaneRml += fmt::format("• {}<br/>", randomizer::getTextStr(prettyItemName));
        }
        // Display the count before the itemname for these items
        else if (itemName.find("Small Key") != std::string::npos ||
            itemName.find("Shard") != std::string::npos ||
            itemName.find("Fused Shadow") != std::string::npos ||
            itemName.find("Hidden Skill") != std::string::npos ||
            itemName == "Poe Soul" ||
            itemName == "Bomb Bag")
        {
            rightPaneRml += fmt::format("• {} {}<br/>", count, itemText);
        } else {
            rightPaneRml += fmt::format("• {}<br/>", itemName);
        }
    }

    session::svc_mng.ui->elem_set_rml(session::svc_mng.mod_ctx, startingInventoryRmlElem, rightPaneRml.c_str());
    return MOD_OK;
}

// Excluded Locations Tab
UiElementHandle exlocHeaderElem = 0;
UiElementHandle exlocSubHeaderElem = 0;
UiElementHandle exlocRightPaneElem = 0;
UiElementHandle exlocLeftPaneElem = 0;
UiElementHandle exlocClearBtnElem = 0;
// UiElementHandle exlocFilterInputElem = 0;
std::string exlocFilter{};

struct ExcludedTabLocData {
    std::string name {};
    std::string lowercaseName{};
    std::unordered_set<std::string> categories{};
};

auto& getExcludedLocationsList() {
    static std::list<ExcludedTabLocData> locationsForExcludedTab;

    // If we haven't loaded the locations to display for the excluded locations tab, load them up
    if (locationsForExcludedTab.empty()) {
        auto locationDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "locations.yaml");
        for (const auto& locationNode : locationDataTree) {
            ExcludedTabLocData excludedTabLocData{};
            auto& name = excludedTabLocData.name;
            auto& lowercaseName = excludedTabLocData.lowercaseName;
            name = locationNode["Name"].as<std::string>();
            lowercaseName = name;
            std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(),
           [](unsigned char c) { return std::tolower(c); });

            for (const auto& category : locationNode["Categories"]) {
                excludedTabLocData.categories.insert(category.as<std::string>());
            }

            if (locationNode["Metadata"].IsMap()) {
                for (const auto& data : locationNode["Metadata"]) {
                    excludedTabLocData.categories.insert(data.first.as<std::string>());
                }
            }

            // Don't include warp portals
            if (excludedTabLocData.categories.contains("Warp Portal")) {
                continue;
            }

            // Certain locations we don't include for now
            if (randomizer::utility::str::Contains(excludedTabLocData.name,
                "Renados Letter", "Telma Invoice", "Wooden Statue", "Ilia Charm",
                "Defeat Ganondorf", "Twilit Insect", "Twilit Bloat", "Hint"))
            {
                continue;
            }

            locationsForExcludedTab.push_back(excludedTabLocData);
        }

        locationsForExcludedTab.sort([](const auto& a, const auto& b) {
            return a.name < b.name;
        });
    }

    // Create the vector we're going to return
    static std::vector<const std::string*> locationNames{};
    locationNames.clear();

    // Get settings values
    auto& randoSettings = GetRandomizerConfig().GetSettings().GetMap();
    bool goldenBugs = randoSettings.at("Golden Bugs") == "On";
    bool skyCharacters = randoSettings.at("Sky Characters") == "On";
    bool npcs = randoSettings.at("Gifts From NPCs") == "On";
    bool shops = randoSettings.at("Shop Items") == "On";
    bool goldenWolves = randoSettings.at("Hidden Skills") == "On";
    bool hiddenRupees = randoSettings.at("Hidden Rupees") == "On";
    bool freestandingRupees = randoSettings.at("Freestanding Rupees") == "On";
    bool overworldPoes = randoSettings.at("Poe Souls").IsAnyOf("Overworld", "All");
    bool dungeonPoes = randoSettings.at("Poe Souls").IsAnyOf("Dungeon", "All");

    // Create lowercase filter
    std::string lowercaseFilter = exlocFilter;
    std::transform(lowercaseFilter.begin(), lowercaseFilter.end(), lowercaseFilter.begin(),
               [](unsigned char c) { return std::tolower(c); });

    // Add relevant location names
    for (const auto& locData : locationsForExcludedTab) {
        // Skip categories that aren't shuffled
        auto& cats = locData.categories;
        if ((!goldenBugs && cats.contains("Golden Bug")) ||
            (!skyCharacters && cats.contains("Sky Character")) ||
            (!npcs && cats.contains("Npc")) ||
            (!shops && cats.contains("Shop")) ||
            (!goldenWolves && cats.contains("Golden Wolf")) ||
            (!hiddenRupees && cats.contains("Rupee - Hidden")) ||
            (!freestandingRupees && cats.contains("Rupee - Freestanding")) ||
            (!overworldPoes && cats.contains("Poe") && cats.contains("Overworld")) ||
            (!dungeonPoes && cats.contains("Poe") && cats.contains("Dungeon")))
        {
            continue;
        }

        // Don't add this location if it doesn't match the current filter
        if (locData.lowercaseName.find(lowercaseFilter) == std::string::npos) {
            continue;
        }

        locationNames.push_back(&locData.name);
    }

    return locationNames;
}

ModResult buildExcludedLocationsTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    using namespace session;
    auto mod_ctx = svc_mng.mod_ctx;

    svc_mng.ui->elem_set_class(mod_ctx, leftPane, "excluded-locations-pane-left", true);
    svc_mng.ui->elem_set_class(mod_ctx, rightPane, "excluded-locations-pane-right", true);

    add_button(leftPane,
        "Clear All",
        "",
        [](ModContext*, void*) {
            GetRandomizerConfig().GetSettings().GetModifiableExcludedLocations().clear();
        },
        nullptr,
        &exlocClearBtnElem);
    svc_mng.ui->elem_set_class(mod_ctx, exlocClearBtnElem, "clear-all-button", true);

    // TODO: leaving out filter for now, since we're building all the buttons at tab init.
    // try to reimplement later when UiService is able to support something closer to the original impl
    /* add_string_input(leftPane,
        "Filter",
        "",
        256,
        [](ModContext*, void*, UiControlValue* out_value) {
            out_value->string_value = exlocFilter.c_str();
        },
        [](ModContext*, void*, const UiControlValue* value) {
            exlocFilter = value->string_value;
        },
        &exlocFilterInputElem);
    svc_mng.ui->elem_set_class(mod_ctx, exlocFilterInputElem, "filter-input", true); */

    svc_mng.ui->pane_add_rml(mod_ctx, leftPane, "", &exlocLeftPaneElem);
    svc_mng.ui->elem_set_class(mod_ctx, exlocLeftPaneElem, "excluded-locations-pane", true);

    auto exlocList = getExcludedLocationsList();
    for (auto e : exlocList) {
        UiElementHandle handle{};
        add_button(leftPane,
            e->c_str(),
            "",
            [](ModContext*, void* userdata) {
                std::string locationName = static_cast<const char*>(userdata);
                auto& excludedLocations = GetRandomizerConfig().GetSettings().GetModifiableExcludedLocations();

                if (excludedLocations.contains(locationName)) {
                    excludedLocations.erase(locationName);
                } else {
                    excludedLocations.insert(locationName);
                }

                SaveRandomizerConfig();
            },
            (void*)e->data(),
            &handle);
        svc_mng.ui->elem_set_class(mod_ctx, handle, "excluded-location-button", true);
    }

    svc_mng.ui->pane_add_text(mod_ctx, rightPane, "Current Excluded Locations", &exlocHeaderElem);
    svc_mng.ui->elem_set_class(mod_ctx, exlocHeaderElem, "excluded-locations-header", true);

    svc_mng.ui->pane_add_text(mod_ctx, rightPane, "Re-select a location to remove it", &exlocSubHeaderElem);
    svc_mng.ui->elem_set_class(mod_ctx, exlocSubHeaderElem, "excluded-locations-subheader", true);

    svc_mng.ui->pane_add_rml(mod_ctx, rightPane, "", &exlocRightPaneElem);
    svc_mng.ui->elem_set_class(mod_ctx, exlocRightPaneElem, "excluded-locations-inner-pane-right", true);
    return MOD_OK;
}

ModResult updateExcludedLocationsTab(ModContext* ctx, void*, ModError*) {
    using namespace session;
    std::string rml = "";

    for (const auto& location : GetRandomizerConfig().GetSettings().GetExcludedLocations()) {
        rml += fmt::format("<span>• {}</span><br/>", location);
    }

    svc_mng.ui->elem_set_rml(svc_mng.mod_ctx, exlocRightPaneElem, rml.c_str());
    return MOD_OK;
}

// Menu Tab
void OnMenuTabSelected(ModContext* ctx, void*) {
    UiTabDesc tabs[5]{};

    tabs[0].struct_size = sizeof(UiTabDesc);
    tabs[0].title = "Seed Management";
    tabs[0].build = buildSeedManagementTab;
    tabs[0].update = updateSeedManagementTab;

    tabs[1].struct_size = sizeof(UiTabDesc);
    tabs[1].title = "Seed Options";
    tabs[1].build = buildSeedOptionsTab;

    tabs[2].struct_size = sizeof(UiTabDesc);
    tabs[2].title = "Hints";
    tabs[2].build = buildHintsTab;

    tabs[3].struct_size = sizeof(UiTabDesc);
    tabs[3].title = "Starting Inventory";
    tabs[3].build = buildStartingInventoryTab;
    tabs[3].update = updateStartingInventoryTab;

    tabs[4].struct_size = sizeof(UiTabDesc);
    tabs[4].title = "Excluded Locations";
    tabs[4].build = buildExcludedLocationsTab;
    tabs[4].update = updateExcludedLocationsTab;

    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 5;
    UiWindowHandle window{};
    session::svc_mng.ui->window_push(ctx, &desc, &window);
}

// Play Tab
ModResult buildPlayTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    std::filesystem::path seed_dir = paths::GetRandomizerSeedsPath();
    if (!std::filesystem::exists(seed_dir))
        std::filesystem::create_directory(seed_dir);

    std::string help_rml = "";
    if (std::filesystem::is_empty(seed_dir)) {
        help_rml = "No seeds generated! You can generate a seed from the Seed Management Tab.";
    } else {
        help_rml = "Choose which seed you want to play.";
    }

    std::vector<std::string> seedHashes;
    for (const auto& entry : std::filesystem::directory_iterator(seed_dir)) {
        if (entry.is_directory()) {
            seedHashes.push_back(entry.path().filename().string());
        }
    }
    
    std::vector<const char*> availableSeeds;
    for (const auto& hash : seedHashes) {
        availableSeeds.push_back(hash.c_str());
    }

    add_select(leftPane,
        "Selected Seed",
        help_rml.c_str(),
        availableSeeds.data(),
        availableSeeds.size(),
        [](ModContext*, void*, UiControlValue* out_value) {
            int idx = 0;
            for (const auto& entry : std::filesystem::directory_iterator(paths::GetRandomizerSeedsPath())) {
                if (entry.is_directory()) {
                    std::string hash = entry.path().filename().string();
                    if (session::g_pending_seed_hash == hash) {
                        break;
                    }
                    idx++;
                }
            }

            out_value->int_value = idx;
        },
        [](ModContext*, void*, const UiControlValue* value) {
            int idx = 0;
            for (const auto& entry : std::filesystem::directory_iterator(paths::GetRandomizerSeedsPath())) {
                if (entry.is_directory()) {
                    if (idx == value->int_value) {
                        session::g_pending_seed_hash = entry.path().filename().string();
                        break;
                    }
                    idx++;
                }
            }
        });

    {
        UiControlDesc desc = UI_CONTROL_DESC_INIT;
        desc.kind = UI_CONTROL_BUTTON;
        desc.label = "Start Randomizer";
        desc.help_rml = "";
        desc.on_pressed = [](ModContext*, void* userdata) {
            // set flag to move to name screen after window close
            g_file_select_window_ctx.is_proceed = true;
            session::svc_mng.ui->window_close(session::svc_mng.mod_ctx, *static_cast<UiWindowHandle*>(userdata));
            mDoAud_seStartMenu(Z2SE_SY_NEW_FILE);
        };
        desc.user_data = &g_file_select_window_ctx.window_handle;
        desc.is_disabled = [](ModContext*, void*) {
            return session::g_pending_seed_hash.empty();
        };
        session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, leftPane, &desc, nullptr);
    }

    return MOD_OK;
}
}

ModResult buildMenuTab() {
    UiMenuTabDesc desc = UI_MENU_TAB_DESC_INIT;
    desc.label = "Randomizer";
    desc.on_selected = OnMenuTabSelected;

    return session::svc_mng.ui->register_menu_tab(session::svc_mng.mod_ctx, &desc, &g_menu_tab);
}

ModResult buildFileSelectGateMenu(dFile_select_c* fileSelect) {
    UiTabDesc tabs[6]{};

    tabs[0].struct_size = sizeof(UiTabDesc);
    tabs[0].title = "Play";
    tabs[0].build = buildPlayTab;

    tabs[1].struct_size = sizeof(UiTabDesc);
    tabs[1].title = "Seed Management";
    tabs[1].build = buildSeedManagementTab;
    tabs[1].update = updateSeedManagementTab;

    tabs[2].struct_size = sizeof(UiTabDesc);
    tabs[2].title = "Seed Options";
    tabs[2].build = buildSeedOptionsTab;

    tabs[3].struct_size = sizeof(UiTabDesc);
    tabs[3].title = "Hints";
    tabs[3].build = buildHintsTab;

    tabs[4].struct_size = sizeof(UiTabDesc);
    tabs[4].title = "Starting Inventory";
    tabs[4].build = buildStartingInventoryTab;
    tabs[4].update = updateStartingInventoryTab;

    tabs[5].struct_size = sizeof(UiTabDesc);
    tabs[5].title = "Excluded Locations";
    tabs[5].build = buildExcludedLocationsTab;
    tabs[5].update = updateExcludedLocationsTab;

    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 6;
    desc.user_data = fileSelect;
    desc.on_closed = [](ModContext*, UiWindowHandle, void* userdata) {
        dFile_select_c* i_this = static_cast<dFile_select_c*>(userdata);

        // if closing the window through backing out, return to file select
        if (!g_file_select_window_ctx.is_proceed)  {
            i_this->headerTxtSet(0x43, 1, 0);
            i_this->fileRecScaleAnmInitSet2(0.0f, 1.0f);
            i_this->nameMoveAnmInitSet(0xd29, 0xd1f);
            i_this->modoruTxtDispAnmInit(0);
            i_this->mDataSelProc = dFile_select_c::DATASELPROC_NAME_TO_DATA_SELECT_MOVE;
        }

        g_dialogSelectModeState = SelectReady;
    };

    return session::svc_mng.ui->window_push(session::svc_mng.mod_ctx, &desc, &g_file_select_window_ctx.window_handle);
}

} // namespace dusk::ui
