#include "ui.hpp"

#include <atomic>
#include <cstring>
#include <filesystem>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <mods/svc/log.hpp>

#include "../session.hpp"
#include "../randomizer_context.hpp"
#include "../paths.hpp"

#include "config_store.hpp"
#include "rando_seed_generation.hpp"

namespace randomizer::ui {
namespace {
// Seed Tab
ModResult buildSeedTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    return MOD_OK;
}

ModResult updateSeedTab(ModContext* ctx, void*, ModError*) {
    return MOD_OK;
}

// Settings Tab
ModResult buildSettingsTab(ModContext* ctx, UiWindowHandle, UiElementHandle leftPane,
    UiElementHandle rightPane, void*, ModError*)
{
    return MOD_OK;
}

// Menu Tab
void OnMenuTabSelected(ModContext* ctx, void*) {
    UiTabDesc tabs[2]{};

    tabs[0].struct_size = sizeof(UiTabDesc);
    tabs[0].title = "Seed";
    tabs[0].build = buildSeedTab;
    tabs[0].update = updateSeedTab;

    tabs[1].struct_size = sizeof(UiTabDesc);
    tabs[1].title = "Settings";
    tabs[1].build = buildSettingsTab;

    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 2;
    UiWindowHandle window{};
    session::svc_mng.ui->window_push(ctx, &desc, &window);
}
}

UiMenuTabHandle g_menu_tab{};

ModResult initialize() {
    UiMenuTabDesc desc = UI_MENU_TAB_DESC_INIT;
    desc.label = "Randomizer";
    desc.on_selected = OnMenuTabSelected;

    ModResult res =
        session::svc_mng.ui->register_menu_tab(session::svc_mng.mod_ctx, &desc, &g_menu_tab);
    if (res != MOD_OK) {
        return res;
    }

    return MOD_OK;
}

void update() {
    UpdateSeedGenerationDialog();
}

}