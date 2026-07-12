#pragma once

#include "dusk/mod_loader.hpp"
#include "mods/svc/host.h"
#include "mods/svc/log.h"

#include <cstdint>
#include <string>

namespace dusk::mods::svc {

struct ServiceRecord {
    std::string id;
    uint16_t majorVersion = 0;
    uint16_t minorVersion = 0;
    const void* service = nullptr;
    LoadedMod* provider = nullptr;
    bool deferred = false;
};

// A host service and its lifecycle hooks. Every hook is optional. Frame and lifecycle hooks run in
// registration order, modDetached in reverse registration order.
struct ServiceModule {
    const char* id = nullptr;
    uint16_t majorVersion = 0;
    uint16_t minorVersion = 0;
    const void* service = nullptr;

    // One-time setup, at registration (ModLoader::init_services).
    void (*initialize)() = nullptr;
    // A mod is going away (deactivation or failed activation): drop all state held for it.
    // Runs after the mod's mod_shutdown and before its library unloads, so pointers into
    // the mod are still valid but must not be called.
    void (*modDetached)(LoadedMod& mod) = nullptr;
    // A batch of (de)activations finished applying: startup, and runtime enable/disable/
    // reload requests. The set of active mods is stable when this runs.
    void (*lifecycleApplied)() = nullptr;
    // Top of ModLoader::tick, before pending lifecycle requests apply.
    void (*frameBegin)() = nullptr;
    // End of ModLoader::tick, after every mod_update.
    void (*frameEnd)() = nullptr;
    // ModLoader::shutdown, after every mod has deactivated.
    void (*shutdown)() = nullptr;
};

bool valid_service_id(const char* serviceId);
ModResult register_service(const char* serviceId, uint16_t majorVersion, uint16_t minorVersion,
    const void* service, LoadedMod* provider, bool deferred);
ModResult publish_deferred_service(
    LoadedMod& provider, const char* serviceId, uint16_t majorVersion, const void* service);
void remove_services_for_provider(const LoadedMod& provider);
const ServiceRecord* find_service(
    const char* serviceId, uint16_t majorVersion, uint16_t minMinorVersion);
// Unlike find_service, also returns deferred records that have not been published yet.
const ServiceRecord* find_service_record(const char* serviceId, uint16_t majorVersion);

ModResult register_module(const ServiceModule& module);
void modules_mod_detached(LoadedMod& mod);
void modules_lifecycle_applied();
void modules_frame_begin();
void modules_frame_end();
void modules_shutdown();

extern const ServiceModule g_hostModule;
extern const ServiceModule g_logModule;
extern const ServiceModule g_resourceModule;
extern const ServiceModule g_hookModule;
extern const ServiceModule g_overlayModule;
extern const ServiceModule g_textureModule;
extern const ServiceModule g_configModule;
extern const ServiceModule g_uiModule;
extern const ServiceModule g_gameModule;
extern const ServiceModule g_cameraModule;
extern const ServiceModule g_gfxModule;

}  // namespace dusk::mods::svc
