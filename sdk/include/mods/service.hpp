#pragma once

#include <mods/api.h>
#include <mods/meta.hpp>

#include <cstdint>
#include <cstdio>
#include <type_traits>

namespace mods {

template <class Service>
struct ServiceTraits;

inline ModResult set_error(ModError* outError, ModResult code, const char* message) {
    if (outError != nullptr && outError->struct_size >= sizeof(ModError)) {
        outError->code = code;
        outError->message[0] = '\0';
        if (message != nullptr) {
            std::snprintf(outError->message, sizeof(outError->message), "%s", message);
        }
    }
    return code;
}

}  // namespace mods

#define DEFINE_MOD()                                                                               \
    extern "C" {                                                                                   \
    MOD_EXPORT ModContext* mod_ctx = nullptr;                                                      \
    }                                                                                              \
    MOD_META_RECORD static constinit ModMetaHeader mod_meta_header_record =                        \
        ::mods::detail::make_header();                                                             \
    MOD_META_BOUNDS_DEFN                                                                           \
    extern "C" {                                                                                   \
    MOD_EXPORT constinit const ModMeta mod_meta = {                                                \
        sizeof(ModMeta),                                                                           \
        MOD_META_BOUNDS_BEGIN,                                                                     \
        MOD_META_BOUNDS_END,                                                                       \
    };                                                                                             \
    }

// Declares `static const service_type* variable`, filled in by the host before mod_initialize.
// Required imports are guaranteed non-null (the mod fails to load otherwise); optional imports
// must be checked against nullptr before use. The unversioned macros use the latest minor version;
// set an explicit version to target an older minor version for backwards compatibility.
#define IMPORT_SERVICE_EX(                                                                         \
    service_type, variable, service_id_value, major_value, min_minor_value, flags_value)           \
    static const service_type* variable = nullptr;                                                 \
    MOD_META_RECORD static constinit ModMetaImport mod_meta_import_##variable = {                  \
        {sizeof(ModMetaImport), MOD_META_IMPORT, static_cast<uint8_t>(flags_value)},               \
        static_cast<uint16_t>(major_value),                                                        \
        static_cast<uint16_t>(min_minor_value),                                                    \
        &(variable),                                                                               \
        ::mods::detail::make_service_id(service_id_value),                                         \
    }

#define IMPORT_SERVICE_VERSION(service_type, variable, min_minor_value)                            \
    IMPORT_SERVICE_EX(service_type, variable, ::mods::ServiceTraits<service_type>::id,             \
        ::mods::ServiceTraits<service_type>::major_version, min_minor_value,                       \
        SERVICE_IMPORT_REQUIRED)

#define IMPORT_SERVICE(service_type, variable)                                                     \
    IMPORT_SERVICE_VERSION(                                                                        \
        service_type, variable, ::mods::ServiceTraits<service_type>::minor_version)

#define IMPORT_OPTIONAL_SERVICE_VERSION(service_type, variable, min_minor_value)                   \
    IMPORT_SERVICE_EX(service_type, variable, ::mods::ServiceTraits<service_type>::id,             \
        ::mods::ServiceTraits<service_type>::major_version, min_minor_value,                       \
        SERVICE_IMPORT_OPTIONAL)

#define IMPORT_OPTIONAL_SERVICE(service_type, variable)                                            \
    IMPORT_OPTIONAL_SERVICE_VERSION(                                                               \
        service_type, variable, ::mods::ServiceTraits<service_type>::minor_version)

#define EXPORT_SERVICE_AS(instance, service_id_value)                                              \
    MOD_META_RECORD static constinit ModMetaExport mod_meta_export_##instance = {                  \
        {sizeof(ModMetaExport), MOD_META_EXPORT, SERVICE_EXPORT_STATIC},                           \
        (instance).header.major_version,                                                           \
        (instance).header.minor_version,                                                           \
        &(instance),                                                                               \
        ::mods::detail::make_service_id(service_id_value),                                         \
    }

#define EXPORT_SERVICE(instance)                                                                   \
    EXPORT_SERVICE_AS(instance, ::mods::ServiceTraits<std::remove_cv_t<decltype(instance)>>::id)

#define EXPORT_DEFERRED_SERVICE(token, service_id_value, major_value, minor_value)                 \
    MOD_META_RECORD static constinit ModMetaExport mod_meta_export_##token = {                     \
        {sizeof(ModMetaExport), MOD_META_EXPORT, SERVICE_EXPORT_DEFERRED},                         \
        static_cast<uint16_t>(major_value),                                                        \
        static_cast<uint16_t>(minor_value),                                                        \
        nullptr,                                                                                   \
        ::mods::detail::make_service_id(service_id_value),                                         \
    }
