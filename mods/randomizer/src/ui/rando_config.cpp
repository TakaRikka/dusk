#include "rando_config.hpp"

#include <mods/svc/log.hpp>

#include "../session.hpp"
#include "../tools.h"
#include "../paths.hpp"
#include "../../generator/seedgen/seed.hpp"
#include "../../generator/utility/string.hpp"

#include "rando_seed_generation.hpp"
#include "config_store.hpp"

#include <mutex>
#include <thread>
#include <map>

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

namespace {
// Control Helpers
void add_button(UiElementHandle pane, const char* label, const char* help_rml,
    UiPressedFn on_pressed, UiElementHandle* out_handle = nullptr)
{
    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_BUTTON;
    desc.label = label;
    desc.help_rml = help_rml;
    desc.on_pressed = on_pressed;
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
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

void add_select_setting(UiElementHandle pane, const char* key, const char* help_rml,
    UiControlGetFn getFn, UiControlSetFn setFn, UiElementHandle* out_handle = nullptr)
{
    auto setting = FindSetting(key);
    auto info = setting->GetInfo();

    std::vector<const char*> optionsList;
    for (size_t i = 0; i < info->GetOptions().size(); ++i) {
        optionsList.push_back(info->GetOptions()[i].c_str());
    }

    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    desc.kind = UI_CONTROL_SELECT;
    desc.label = key;
    desc.help_rml = help_rml;
    desc.binding = UI_BINDING_CALLBACKS;
    desc.get = getFn;
    desc.set = setFn;
    desc.options = optionsList.data();
    desc.option_count = optionsList.size();
    session::svc_mng.ui->pane_add_control(session::svc_mng.mod_ctx, pane, &desc, out_handle);
}

// Seed Management Tab
ModResult buildSeedManagementTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    add_button(leftPane,
        "Generate Seed",
        "Generate a Randomizer seed using the current configuration options, and the supplied seed string.",
        [](ModContext*, void*) {});

    add_string_input(leftPane,
        "Seed String",
        "Current value of the seed used by the randomizer for generation. Leave blank for a random value.",
        32,
        [](ModContext*, void*, UiControlValue*) {},
        [](ModContext*, void*, const UiControlValue*) {});

    add_button(leftPane,
        "Delete Seeds",
        " ",
        [](ModContext*, void*) {});

    add_section(leftPane, "Permalink");

    add_button(leftPane,
        "Copy Permalink",
        "Copy your current settings permalink to share with others.",
        [](ModContext*, void*) {});

    add_button(leftPane,
        "Paste Permalink",
        "Paste in a permalink from your clipboard. This will overwrite your current settings.",
        [](ModContext*, void*) {});

    add_section(leftPane, "Presets");

    add_button(leftPane,
        "Save Current Settings as Preset",
        "Save the current settings to your list of presets.",
        [](ModContext*, void*) {});

    add_button(leftPane,
        "Load Preset",
        "Choose an existing preset to load from.",
        [](ModContext*, void*) {});

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
        [](ModContext*, void*) {});

    add_section(leftPane, "Logic Settings");

    add_select_setting(leftPane,
        "Logic Rules",
        " ",
        [](ModContext*, void*, UiControlValue*) {},
        [](ModContext*, void*, const UiControlValue*) {});

    add_section(leftPane, "Access Options");

    add_section(leftPane, "Shuffles");

    add_section(leftPane, "Dungeon Items");

    add_section(leftPane, "Timesavers");

    add_section(leftPane, "Additional Settings");

    add_section(leftPane, "Dungeon Entrance Settings");

    add_section(leftPane, "Tricks");

    return MOD_OK;
}

// Hints Tab
ModResult buildHintsTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    return MOD_OK;
}

// Starting Inventory Tab
ModResult buildStartingInventoryTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    return MOD_OK;
}

// Excluded Locations Tab
ModResult buildExcludedLocationsTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
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

    tabs[4].struct_size = sizeof(UiTabDesc);
    tabs[4].title = "Excluded Locations";
    tabs[4].build = buildExcludedLocationsTab;

    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 5;
    UiWindowHandle window{};
    session::svc_mng.ui->window_push(ctx, &desc, &window);
}
}

UiMenuTabHandle g_menu_tab{};

ModResult buildMenuTab() {
    UiMenuTabDesc desc = UI_MENU_TAB_DESC_INIT;
    desc.label = "Randomizer";
    desc.on_selected = OnMenuTabSelected;

    return session::svc_mng.ui->register_menu_tab(session::svc_mng.mod_ctx, &desc, &g_menu_tab);
}

std::filesystem::path GetRandomizerPath() {
    return paths::GetRandomizerPath() / "randomizer";
}

std::filesystem::path GetRandomizerSettingsPath() {
    return GetRandomizerPath() / "settings.yaml";
}

std::filesystem::path GetRandomizerPreferencesPath() {
    return GetRandomizerPath() / "preferences.yaml";
}

std::filesystem::path GetRandomizerPresetsPath() {
    return GetRandomizerPath() / "presets";
}

std::filesystem::path GetRandomizerSeedsPath() {
    return GetRandomizerPath() / "seeds";
}

} // namespace dusk::ui
