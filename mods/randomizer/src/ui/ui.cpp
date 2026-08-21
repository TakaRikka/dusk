#include "ui.hpp"

#include <mods/svc/log.hpp>

#include "../session.hpp"

#include "rando_seed_generation.hpp"
#include "rando_config.hpp"

namespace randomizer::ui {
UiStyleHandle styleHandle{};

ModResult initialize() {
    ModResult res;

    res = session::svc_mng.ui->register_styles_file(
        session::svc_mng.mod_ctx,
        UI_SCOPE_WINDOW,
        "ui.rcss",
        &styleHandle);
    if (res != MOD_OK) {
        mods::log::error("failed to register rcss!");
        return res;
    }

    res = buildMenuTab();
    if (res != MOD_OK) {
        mods::log::error("failed to initialize randomizer menu tab!");
        return res;
    }

    return MOD_OK;
}

void update() {
    UpdateSeedGenerationDialog();
}

ModResult shutdown() {
    removeMenuTab();
    session::svc_mng.ui->unregister_styles(mod_ctx, styleHandle);
    return MOD_OK;
}

}