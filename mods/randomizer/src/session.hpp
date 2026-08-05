#pragma once

#include "mods/svc/host.h"
#include "mods/svc/log.h"
#include "mods/svc/config.h"
#include "mods/svc/hook.h"
#include "mods/svc/ui.h"
#include "mods/svc/resource.h"
#include "mods/svc/save.h"
#include "mods/svc/stage.h"

namespace randomizer::session {
struct ServiceManager {
    ModContext* mod_ctx;
    const HostService* host;
    const LogService* log;
    const HookService* hook;
    const UiService* ui;
    const ResourceService* resource;
    const ConfigService* config;
    const SaveService* save;
    const StageService* stage;
};

extern ServiceManager svc_mng;

ModResult initialize(const ServiceManager& services);

inline void LogError(const char* msg) {
    svc_mng.log->error(svc_mng.mod_ctx, msg);
}

inline void LogDebug(const char* msg) {
    svc_mng.log->debug(svc_mng.mod_ctx, msg);
}

inline void LogWarn(const char* msg) {
    svc_mng.log->warn(svc_mng.mod_ctx, msg);
}
}