#pragma once

#include "mods/svc/gfx.h"

namespace dusk::mods {

void gfx_run_stage(GfxStage stage, const view_class* gameView = nullptr,
    const view_port_class* gameViewport = nullptr);

}  // namespace dusk::mods
