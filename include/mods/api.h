#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
extern "C" {
#else
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif

#if defined(_WIN32)
#define MOD_EXPORT __declspec(dllexport)
#else
#define MOD_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
#define MOD_EXTERN_C extern "C"
#else
#define MOD_EXTERN_C
#endif

#define MOD_ABI_VERSION 5u
#define MOD_ERROR_MESSAGE_SIZE 512u

typedef struct ModContext ModContext;

typedef enum ModResult {
    MOD_OK = 0,
    MOD_ERROR = 1,
    MOD_UNAVAILABLE = 2,
    MOD_UNSUPPORTED = 3,
    MOD_CONFLICT = 4,
    MOD_INVALID_ARGUMENT = 5,
} ModResult;

static_assert(sizeof(ModResult) == 4, "mod SDK enums must be int-sized; do not build mods with -fshort-enums");

typedef struct ModError {
    uint32_t struct_size;
    ModResult code;
    char message[MOD_ERROR_MESSAGE_SIZE];
} ModError;

#define MOD_ERROR_INIT {sizeof(ModError), MOD_OK, {0}}

/*
 * Opaque per-mod context, populated by the host before mod_initialize is called.
 * Pass it as the first argument to every service call; it identifies the calling
 * mod for attribution (logging, resource lookup, hook ownership, etc.).
 */
MOD_EXPORT extern ModContext* mod_ctx;

/*
 * Service versioning contract:
 *
 * A service is a struct of function pointers, beginning with a ServiceHeader.
 * Compatibility is tracked with a major/minor version pair:
 *
 * - A major version bump is a breaking change. Different majors are distinct
 *   services; the registry never matches an import against a different major.
 * - A minor version bump may only append fields to the end of the struct.
 *   Existing fields must keep their offsets and semantics.
 *
 * Providers: exporting minor N means every function pointer introduced at or
 * below N is populated (non-NULL). struct_size reflects the compiled struct.
 *
 * Importers: importing with min_minor_version N guarantees (enforced at load
 * time) that the resolved service is at least minor N, so any field introduced
 * at or below N may be used unconditionally, with no availability checks.
 * Fields newer than the declared min_minor_version must be gated behind
 * SERVICE_HAS plus a NULL check on the pointer itself.
 *
 * Load ordering: a manifest import of another mod's service (required or
 * optional) guarantees that the provider's mod_initialize completed before the
 * importer's runs, and deferred services published during the provider's
 * initialization resolve into import slots just like static exports. If a
 * provider fails to load, mods that required its services fail in turn. Mods
 * whose required imports form a cycle all fail to load; a cycle involving an
 * optional import is broken by dropping the ordering guarantee (not the
 * resolution) of that optional import. Dynamic lookups via
 * HostService::get_service carry no ordering guarantee: they see whatever has
 * been published at call time.
 */
typedef struct ServiceHeader {
    uint32_t struct_size;
    uint16_t major_version;
    uint16_t minor_version;
} ServiceHeader;

#define SERVICE_HEADER(service_type, major, minor) {sizeof(service_type), (major), (minor)}

#define SERVICE_HAS(service, service_type, field)                                                  \
    ((service) != NULL &&                                                                          \
        (service)->header.struct_size >=                                                           \
            (uint32_t)(offsetof(service_type, field) + sizeof(((service_type*)0)->field)))

typedef enum ServiceImportFlags {
    SERVICE_IMPORT_REQUIRED = 0u,
    SERVICE_IMPORT_OPTIONAL = 1u << 0u,
} ServiceImportFlags;

typedef enum ServiceExportFlags {
    SERVICE_EXPORT_STATIC = 0u,
    SERVICE_EXPORT_DEFERRED = 1u << 0u,
} ServiceExportFlags;

typedef struct ServiceImport {
    uint32_t struct_size;
    const char* service_id;
    uint16_t major_version;
    uint16_t min_minor_version;
    uint32_t flags;
    void* slot;
} ServiceImport;

typedef struct ServiceExport {
    uint32_t struct_size;
    const char* service_id;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t flags;
    const void* service;
} ServiceExport;

typedef struct ModManifest {
    uint32_t struct_size;
    uint32_t abi_version;
    const ServiceImport* imports;
    size_t import_count;
    const ServiceExport* exports;
    size_t export_count;
} ModManifest;

typedef const ModManifest* (*ModGetManifestFn)(void);

typedef ModResult (*ModInitializeFn)(ModError* out_error);
typedef ModResult (*ModUpdateFn)(ModError* out_error);
typedef ModResult (*ModShutdownFn)(ModError* out_error);

MOD_EXPORT const ModManifest* mod_get_manifest(void);

MOD_EXPORT ModResult mod_initialize(ModError* out_error);
MOD_EXPORT ModResult mod_update(ModError* out_error);
MOD_EXPORT ModResult mod_shutdown(ModError* out_error);

#ifdef __cplusplus
}
#endif
