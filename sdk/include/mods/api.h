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

#define MOD_ABI_VERSION 1u
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

static_assert(sizeof(ModResult) == 4,
    "mod SDK enums must be int-sized; do not build mods with -fshort-enums");

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

/*
 * Mod metadata records.
 *
 * A mod's manifest is a sequence of records in a dedicated section of the native library ("modmeta"
 * on ELF, "__DATA,__modmeta" on Mach-O, "modmeta$a/$d/$z" on PE).
 *
 * The records are pure data. Every string is inline and NUL-terminated. Fields documented as
 * runtime-only hold relocated pointers that are meaningless on disk; static parsers recover their
 * targets from the file's relocation/bind entries instead.
 *
 * Layout rules:
 * - Little-endian, 8-byte aligned; every record size is a multiple of 8.
 * - Parsers must skip all-zero 8-byte units (linker padding) and records of unknown kind.
 * - Exactly one MOD_META_HEADER record per library.
 *
 * The IMPORT_SERVICE/EXPORT_SERVICE/DEFINE_HOOK macros emit these records; mods do not construct
 * them by hand.
 */

#define MOD_META_SERVICE_ID_SIZE 64u

/* Records are 8-byte aligned so the linker packs them without padding; most are naturally
 * aligned by their pointer fields, the alignment below covers the rest. */
#if defined(__cplusplus)
#define MOD_META_ALIGN alignas(8)
#elif defined(_MSC_VER)
#define MOD_META_ALIGN __declspec(align(8))
#else
#define MOD_META_ALIGN __attribute__((aligned(8)))
#endif

typedef enum ModMetaKind {
    MOD_META_PAD = 0,
    MOD_META_HEADER = 1,
    MOD_META_IMPORT = 2,
    MOD_META_EXPORT = 3,
    MOD_META_HOOK_FN = 4,
    MOD_META_HOOK_MEM = 5,
    MOD_META_HOOK_NAME = 6,
} ModMetaKind;

typedef struct ModMetaRecord {
    uint16_t size; /* total record size in bytes, a multiple of 8 */
    uint8_t kind;  /* ModMetaKind */
    uint8_t flags; /* ServiceImportFlags / ServiceExportFlags for imports/exports */
} ModMetaRecord;

typedef struct ModMetaServiceId {
    char chars[MOD_META_SERVICE_ID_SIZE]; /* NUL-terminated */
} ModMetaServiceId;

typedef struct MOD_META_ALIGN ModMetaHeader {
    ModMetaRecord rec;
    uint32_t abi_version;
} ModMetaHeader;

static_assert(sizeof(ModMetaHeader) == 8);

typedef struct MOD_META_ALIGN ModMetaImport {
    ModMetaRecord rec;
    uint16_t major_version;
    uint16_t min_minor_version;
    void* slot; /* runtime only */
    ModMetaServiceId service_id;
} ModMetaImport;

static_assert(sizeof(ModMetaImport) == 16 + MOD_META_SERVICE_ID_SIZE);

typedef struct MOD_META_ALIGN ModMetaExport {
    ModMetaRecord rec;
    uint16_t major_version;
    uint16_t minor_version;
    const void* service; /* runtime only */
    ModMetaServiceId service_id;
} ModMetaExport;

static_assert(sizeof(ModMetaExport) == 16 + MOD_META_SERVICE_ID_SIZE);

/* Hook on a function named at link time: `target` carries the &fn relocation. */
typedef struct MOD_META_ALIGN ModMetaHookFn {
    ModMetaRecord rec;
    uint32_t reserved;
    void* target;   /* runtime only */
    void* resolved; /* runtime only */
} ModMetaHookFn;

static_assert(sizeof(ModMetaHookFn) == 24);

/*
 * Hook on a member function: `pmf` holds the compiler's pointer-to-member representation
 * (non-virtual: a function address relocation; virtual Itanium/AAPCS: literal slot words). Two
 * NUL-terminated strings follow `resolved`: the class vtable symbol (empty if the class name is not
 * representable), then the stringified target for tooling display.
 */
typedef struct MOD_META_ALIGN ModMetaHookMem {
    ModMetaRecord rec;
    uint32_t reserved;
    unsigned char pmf[16];
    void* resolved; /* runtime only */
} ModMetaHookMem;

static_assert(sizeof(ModMetaHookMem) == 32);

/*
 * Hook on a function by symbol name, for targets that cannot be named in C++ (file-local statics,
 * private members). One NUL-terminated string follows `resolved`; it may be either the platform
 * mangled name or the demangled qualified display name.
 */
typedef struct MOD_META_ALIGN ModMetaHookName {
    ModMetaRecord rec;
    uint32_t reserved;
    void* resolved; /* runtime only */
} ModMetaHookName;

static_assert(sizeof(ModMetaHookName) == 16);

typedef struct ModMeta {
    uint32_t struct_size;
    const void* records_begin;
    const void* records_end;
} ModMeta;

MOD_EXPORT extern const ModMeta mod_meta;

typedef ModResult (*ModInitializeFn)(ModError* out_error);
typedef ModResult (*ModUpdateFn)(ModError* out_error);
typedef ModResult (*ModShutdownFn)(ModError* out_error);

MOD_EXPORT ModResult mod_initialize(ModError* out_error);
MOD_EXPORT ModResult mod_update(ModError* out_error);
MOD_EXPORT ModResult mod_shutdown(ModError* out_error);

#ifdef __cplusplus
}
#endif
