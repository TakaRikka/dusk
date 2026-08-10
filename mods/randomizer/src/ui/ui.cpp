#include "ui.hpp"

#include <mods/svc/log.hpp>

#include "rando_seed_generation.hpp"
#include "rando_config.hpp"

namespace randomizer::ui {
ModResult initialize() {
    ModResult res = buildMenuTab();
    if (res != MOD_OK) {
        mods::log::error("failed to initialize randomizer menu tab!");
        return res;
    }

    return MOD_OK;
}

void update() {
    UpdateSeedGenerationDialog();
}

}