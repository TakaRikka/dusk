#include "registry.hpp"

#include "dusk/app_info.hpp"
#include "dusk/logging.h"
#include "dusk/mods/loader/loader.hpp"

#include <ranges>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace dusk::mods::svc {
namespace {

std::unordered_map<std::string, ServiceRecord> s_services;
std::vector<const ServiceModule*> s_modules;

std::string service_key(std::string_view id, const uint16_t majorVersion) {
    std::string key{id};
    key.push_back('\x1f');
    key += std::to_string(majorVersion);
    return key;
}

const char* mod_id(const LoadedMod* mod) {
    return mod != nullptr ? mod->metadata.id.c_str() : AppName;
}

bool validate_service_header(const ServiceHeader* header, const char* serviceId,
    const uint16_t majorVersion, const uint16_t minorVersion, LoadedMod* provider) {
    if (header == nullptr) {
        DuskLog.error("[{}] service '{}' has null header", mod_id(provider), serviceId);
        return false;
    }
    if (header->struct_size < sizeof(ServiceHeader)) {
        DuskLog.error("[{}] service '{}' has invalid header size {}", mod_id(provider), serviceId,
            header->struct_size);
        return false;
    }
    if (header->major_version != majorVersion || header->minor_version != minorVersion) {
        DuskLog.error("[{}] service '{}' header version {}.{} does not match export {}.{}",
            mod_id(provider), serviceId, header->major_version, header->minor_version, majorVersion,
            minorVersion);
        return false;
    }
    return true;
}

void clear_services() {
    s_services.clear();
    s_modules.clear();
}

}  // namespace

bool valid_service_id(const char* serviceId) {
    return serviceId != nullptr && serviceId[0] != '\0';
}

ModResult register_service(const char* serviceId, const uint16_t majorVersion,
    const uint16_t minorVersion, const void* service, LoadedMod* provider, const bool deferred) {
    if (!valid_service_id(serviceId)) {
        DuskLog.error("[{}] attempted to register a service with no id", mod_id(provider));
        return MOD_INVALID_ARGUMENT;
    }

    if (!deferred && !validate_service_header(static_cast<const ServiceHeader*>(service), serviceId,
                         majorVersion, minorVersion, provider))
    {
        return MOD_INVALID_ARGUMENT;
    }

    const auto key = service_key(serviceId, majorVersion);
    if (s_services.contains(key)) {
        DuskLog.error("[{}] duplicate service '{}@{}'", mod_id(provider), serviceId, majorVersion);
        return MOD_CONFLICT;
    }

    s_services.emplace(key, ServiceRecord{
                                serviceId,
                                majorVersion,
                                minorVersion,
                                service,
                                provider,
                                deferred,
                            });
    return MOD_OK;
}

ModResult publish_deferred_service(
    LoadedMod& provider, const char* serviceId, const uint16_t majorVersion, const void* service) {
    if (!valid_service_id(serviceId) || service == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    const auto it = s_services.find(service_key(serviceId, majorVersion));
    if (it == s_services.end() || !it->second.deferred || it->second.provider != &provider) {
        DuskLog.error("[{}] tried to publish undeclared service '{}@{}'", provider.metadata.id,
            serviceId, majorVersion);
        return MOD_UNSUPPORTED;
    }
    auto& record = it->second;
    if (record.service != nullptr) {
        return MOD_CONFLICT;
    }

    const auto* header = static_cast<const ServiceHeader*>(service);
    if (!validate_service_header(header, serviceId, majorVersion, record.minorVersion, &provider)) {
        return MOD_INVALID_ARGUMENT;
    }

    record.service = service;
    record.minorVersion = header->minor_version;
    return MOD_OK;
}

void remove_services_for_provider(const LoadedMod& provider) {
    std::erase_if(
        s_services, [&](const auto& entry) { return entry.second.provider == &provider; });
}

const ServiceRecord* find_service(
    const char* serviceId, const uint16_t majorVersion, const uint16_t minMinorVersion) {
    const auto* record = find_service_record(serviceId, majorVersion);
    if (record == nullptr || record->service == nullptr || record->minorVersion < minMinorVersion) {
        return nullptr;
    }
    return record;
}

const ServiceRecord* find_service_record(const char* serviceId, const uint16_t majorVersion) {
    if (!valid_service_id(serviceId)) {
        return nullptr;
    }

    const auto it = s_services.find(service_key(serviceId, majorVersion));
    return it != s_services.end() ? &it->second : nullptr;
}

ModResult register_module(const ServiceModule& module) {
    const auto result = register_service(
        module.id, module.majorVersion, module.minorVersion, module.service, nullptr, false);
    if (result != MOD_OK) {
        return result;
    }
    s_modules.push_back(&module);
    if (module.initialize != nullptr) {
        module.initialize();
    }
    return MOD_OK;
}

void modules_mod_detached(LoadedMod& mod) {
    for (const auto* module : s_modules | std::views::reverse) {
        if (module->modDetached != nullptr) {
            module->modDetached(mod);
        }
    }
}

void modules_lifecycle_applied() {
    for (const auto* module : s_modules) {
        if (module->lifecycleApplied != nullptr) {
            module->lifecycleApplied();
        }
    }
}

void modules_frame_begin() {
    for (const auto* module : s_modules) {
        if (module->frameBegin != nullptr) {
            module->frameBegin();
        }
    }
}

void modules_frame_end() {
    for (const auto* module : s_modules) {
        if (module->frameEnd != nullptr) {
            module->frameEnd();
        }
    }
}

void modules_shutdown() {
    for (const auto* module : s_modules | std::views::reverse) {
        if (module->shutdown != nullptr) {
            module->shutdown();
        }
    }
    clear_services();
}

}  // namespace dusk::mods::svc

namespace dusk::mods {

void ModLoader::init_services() {
    svc::clear_services();
    for (const auto* module :
        {
            &svc::g_hostModule,
            &svc::g_logModule,
            &svc::g_resourceModule,
            &svc::g_hookModule,
            &svc::g_overlayModule,
            &svc::g_textureModule,
            &svc::g_configModule,
            &svc::g_uiModule,
            &svc::g_gameModule,
            &svc::g_cameraModule,
            &svc::g_gfxModule,
        })
    {
        svc::register_module(*module);
    }
}

bool ModLoader::register_static_service_exports(LoadedMod& mod) {
    if (!mod.native || mod.native->manifest == nullptr) {
        return true;
    }

    const auto& manifest = *mod.native->manifest;
    for (size_t i = 0; i < manifest.export_count; ++i) {
        const auto& serviceExport = manifest.exports[i];
        if (serviceExport.struct_size != sizeof(ServiceExport) ||
            !svc::valid_service_id(serviceExport.service_id))
        {
            fail_mod(mod, MOD_INVALID_ARGUMENT, "Invalid service export descriptor");
            return false;
        }

        const bool deferred = (serviceExport.flags & SERVICE_EXPORT_DEFERRED) != 0;
        if (!deferred && serviceExport.service == nullptr) {
            fail_mod(mod, MOD_INVALID_ARGUMENT, "Static service export has null service pointer");
            return false;
        }

        const auto result =
            svc::register_service(serviceExport.service_id, serviceExport.major_version,
                serviceExport.minor_version, serviceExport.service, &mod, deferred);
        if (result != MOD_OK) {
            fail_mod(mod, result, "Service export registration failed");
            return false;
        }
    }

    return true;
}

std::string ModLoader::describe_missing_import(
    const char* serviceId, const uint16_t majorVersion, const uint16_t minMinorVersion) const {
    if (const auto* record = svc::find_service_record(serviceId, majorVersion)) {
        if (record->service == nullptr) {
            return fmt::format("Required service {}@{} was never published by provider '{}'",
                serviceId, majorVersion, svc::mod_id(record->provider));
        }
        return fmt::format("Required service {}@{} only provides minor version {} (need >= {})",
            serviceId, majorVersion, record->minorVersion, minMinorVersion);
    }

    // No record can also mean the provider failed or is disabled and its services were removed.
    for (const auto& other : mods()) {
        if ((other.active && !other.loadFailed) || !other.native ||
            other.native->manifest == nullptr)
        {
            continue;
        }
        const auto& manifest = *other.native->manifest;
        for (size_t i = 0; i < manifest.export_count; ++i) {
            const auto& serviceExport = manifest.exports[i];
            if (serviceExport.struct_size == sizeof(ServiceExport) &&
                svc::valid_service_id(serviceExport.service_id) &&
                std::string_view{serviceExport.service_id} == serviceId &&
                serviceExport.major_version == majorVersion)
            {
                return fmt::format("Required service {}@{} unavailable: provider '{}' {}",
                    serviceId, majorVersion, other.metadata.id,
                    other.loadFailed ? "failed to load" : "is disabled");
            }
        }
    }

    return fmt::format("Required service unavailable: {}@{}", serviceId, majorVersion);
}

bool ModLoader::resolve_service_imports(LoadedMod& mod) {
    if (!mod.native || mod.native->manifest == nullptr) {
        return true;
    }

    const auto& manifest = *mod.native->manifest;
    for (size_t i = 0; i < manifest.import_count; ++i) {
        const auto& serviceImport = manifest.imports[i];
        if (serviceImport.struct_size != sizeof(ServiceImport) ||
            !svc::valid_service_id(serviceImport.service_id) || serviceImport.slot == nullptr)
        {
            fail_mod(mod, MOD_INVALID_ARGUMENT, "Invalid service import descriptor");
            return false;
        }

        const auto* service = svc::find_service(
            serviceImport.service_id, serviceImport.major_version, serviceImport.min_minor_version);
        if (service == nullptr) {
            *static_cast<const void**>(serviceImport.slot) = nullptr;
            if ((serviceImport.flags & SERVICE_IMPORT_OPTIONAL) != 0) {
                continue;
            }

            fail_mod(mod, MOD_UNAVAILABLE,
                describe_missing_import(serviceImport.service_id, serviceImport.major_version,
                    serviceImport.min_minor_version));
            return false;
        }

        *static_cast<const void**>(serviceImport.slot) = service->service;
    }

    return true;
}

}  // namespace dusk::mods
