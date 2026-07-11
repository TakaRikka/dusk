#pragma once

#include "window.hpp"

#include <vector>

#include "dusk/mod_loader.hpp"

namespace dusk::ui {

class Pane;

class ModsWindow : public Window {
public:
    ModsWindow();
    void update() override;

private:
    struct ModSnapshot {
        mods::LoadedMod* mod = nullptr;
        bool active = false;
        bool loadFailed = false;
        bool enabled = false;
        bool suspended = false;
        u32 cacheGeneration = 0;
    };

    void build_content(Rml::Element* content);
    void build_detail(Pane& pane, mods::LoadedMod& mod);
    void mark_current_entry();

    std::vector<ModSnapshot> mSnapshot;
    std::vector<Component*> mEntries;
    std::vector<mods::LoadedMod*> mEntryMods;
    mods::LoadedMod* mSelectedMod = nullptr;
};

}  // namespace dusk::ui
