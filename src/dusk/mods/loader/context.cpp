#include "loader.hpp"

#include "dusk/mods/log_buffer.hpp"
#include "dusk/mods/svc/registry.hpp"

namespace dusk::mods {

LoadedMod* mod_from_context(ModContext* context) {
    return context != nullptr ? context->mod : nullptr;
}

const LoadedMod* mod_from_context(const ModContext* context) {
    return context != nullptr ? context->mod : nullptr;
}

const char* mod_id_from_context(ModContext* context) {
    const auto* mod = mod_from_context(context);
    return mod != nullptr ? mod->metadata.id.c_str() : "mod";
}

bool is_safe_resource_path(std::string_view path) {
    if (path.empty() || path.starts_with('/') || path.starts_with('\\') ||
        path.find(':') != std::string_view::npos)
    {
        return false;
    }

    while (!path.empty()) {
        const auto slash = path.find_first_of("/\\");
        const auto segment = path.substr(0, slash);
        if (segment.empty() || segment == "." || segment == "..") {
            return false;
        }
        if (slash == std::string_view::npos) {
            break;
        }
        path.remove_prefix(slash + 1);
    }

    return true;
}

void fail_mod(LoadedMod& mod, ModResult code, std::string_view message) {
    const bool firstFailure = !mod.loadFailed;
    mod.active = false;
    mod.loadFailed = true;
    if (firstFailure || mod.failureReason.empty()) {
        mod.failureReason = message;
    }
    // Stop the failed mod's services from resolving; mods that required them fail in turn.
    // Pointers already handed to other mods stay callable since the library remains loaded.
    // Nothing else is torn down here: fail_mod can run mid-frame (e.g. from a failing mod
    // callback), and full teardown happens via deactivate_mod at a safe point.
    svc::remove_services_for_provider(mod);
    mod.servicesRegistered = false;
    ModLoader::instance().notify_mod_failure(mod, firstFailure);
    log::write(
        mod.metadata.id, LOG_LEVEL_ERROR, "failed: {} ({})", message, static_cast<int>(code));
}

}  // namespace dusk::mods
