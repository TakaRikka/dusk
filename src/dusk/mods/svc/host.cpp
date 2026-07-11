#include "registry.hpp"

#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/manifest.hpp"
#include "fmt/format.h"

#include <algorithm>
#include <vector>
#include <version.h>

namespace dusk::mods::svc {
namespace {

ModResult host_get_service(ModContext*, const char* serviceId, const uint16_t majorVersion,
    const uint16_t minMinorVersion, const void** outService) {
    if (outService == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    *outService = nullptr;
    const auto* service = find_service(serviceId, majorVersion, minMinorVersion);
    if (service == nullptr) {
        return MOD_UNAVAILABLE;
    }
    *outService = service->service;
    return MOD_OK;
}

ModResult host_publish_service(
    ModContext* context, const char* serviceId, const uint16_t majorVersion, const void* service) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !valid_service_id(serviceId) || service == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    return publish_deferred_service(*mod, serviceId, majorVersion, service);
}

void host_fail(ModContext* context, const ModResult code, const char* message) {
    auto* mod = mod_from_context(context);
    if (mod != nullptr) {
        fail_mod(*mod, code, message != nullptr ? message : "Mod reported an unknown failure");
    }
}

const char* host_mod_id(ModContext* context) {
    const auto* mod = mod_from_context(context);
    return mod != nullptr ? mod->metadata.id.c_str() : "";
}

const char* host_mod_name(ModContext* context) {
    const auto* mod = mod_from_context(context);
    return mod != nullptr ? mod->metadata.name.c_str() : "";
}

const char* host_mod_version(ModContext* context) {
    const auto* mod = mod_from_context(context);
    return mod != nullptr ? mod->metadata.version.c_str() : "";
}

const char* host_mod_dir(ModContext* context) {
    const auto* mod = mod_from_context(context);
    return mod != nullptr ? mod->dir.c_str() : "";
}

struct LifecycleWatcher {
    uint64_t handle = 0;
    LoadedMod* owner = nullptr;
    ModLifecycleFn fn = nullptr;
    void* userData = nullptr;
};

std::vector<LifecycleWatcher> s_watchers;
uint64_t s_nextWatchHandle = 1;

ModResult host_watch_mod_lifecycle(
    ModContext* context, ModLifecycleFn fn, void* userData, uint64_t* outHandle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || fn == nullptr || outHandle == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto handle = s_nextWatchHandle++;
    s_watchers.push_back({handle, mod, fn, userData});
    *outHandle = handle;
    return MOD_OK;
}

ModResult host_unwatch_mod_lifecycle(ModContext* context, const uint64_t handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto erased = std::erase_if(s_watchers,
        [&](const LifecycleWatcher& w) { return w.handle == handle && w.owner == mod; });
    return erased != 0 ? MOD_OK : MOD_INVALID_ARGUMENT;
}

void host_mod_detached(LoadedMod& mod) {
    // The subject's own watches go first: a mod is never notified about its own teardown.
    std::erase_if(s_watchers, [&](const LifecycleWatcher& w) { return w.owner == &mod; });

    // Iterate a snapshot: callbacks may watch/unwatch, and a failing callback erases the
    // failing mod's services.
    const auto snapshot = s_watchers;
    for (const auto& watcher : snapshot) {
        const bool alive = std::ranges::any_of(
            s_watchers, [&](const LifecycleWatcher& w) { return w.handle == watcher.handle; });
        if (!alive) {
            continue;
        }
        try {
            watcher.fn(watcher.owner->context.get(), mod.context.get(), mod.metadata.id.c_str(),
                MOD_LIFECYCLE_DETACHED, watcher.userData);
        } catch (const std::exception& e) {
            fail_mod(*watcher.owner, MOD_ERROR,
                fmt::format("Exception in mod lifecycle callback: {}", e.what()));
        } catch (...) {
            fail_mod(*watcher.owner, MOD_ERROR, "Unknown exception in mod lifecycle callback");
        }
    }
}

constinit HostService s_hostService{
    .header = SERVICE_HEADER(HostService, HOST_SERVICE_MAJOR, HOST_SERVICE_MINOR),
    .version = DUSK_VERSION_STRING,
    .build_id = nullptr,
    .build_id_len = 0,
    .get_service = host_get_service,
    .publish_service = host_publish_service,
    .fail = host_fail,
    .mod_id = host_mod_id,
    .mod_name = host_mod_name,
    .mod_version = host_mod_version,
    .mod_dir = host_mod_dir,
    .watch_mod_lifecycle = host_watch_mod_lifecycle,
    .unwatch_mod_lifecycle = host_unwatch_mod_lifecycle,
};

}  // namespace

constinit const ServiceModule g_hostModule{
    .id = HOST_SERVICE_ID,
    .majorVersion = HOST_SERVICE_MAJOR,
    .minorVersion = HOST_SERVICE_MINOR,
    .service = &s_hostService,
    .initialize =
        [] {
            const auto& buildId = manifest::image_build_id();
            s_hostService.build_id = buildId.empty() ? nullptr : buildId.data();
            s_hostService.build_id_len = static_cast<uint32_t>(buildId.size());
        },
    .modDetached = host_mod_detached,
};

}  // namespace dusk::mods::svc
