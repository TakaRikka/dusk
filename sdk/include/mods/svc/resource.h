#pragma once

#include <mods/api.h>

/*
 * Read-only access to the res/ tree of the calling mod's own bundle. Reload serves the new
 * bundle's contents. For writable storage, use HostService::mod_dir.
 */

#define RESOURCE_SERVICE_ID "dev.twilitrealm.dusklight.resource"
#define RESOURCE_SERVICE_MAJOR 1u
#define RESOURCE_SERVICE_MINOR 0u

/*
 * A loaded resource, allocated by the service. Return every successful load with free;
 * buffers still live when the mod is disabled or reloaded are reclaimed with a warning.
 */
typedef struct ResourceBuffer {
    uint32_t struct_size;
    void* data;
    size_t size;
} ResourceBuffer;

#define RESOURCE_BUFFER_INIT {sizeof(ResourceBuffer), NULL, 0u}

typedef struct ResourceService {
    ServiceHeader header;

    /*
     * Load a file into a fresh allocation. `relative_path` is resolved against the bundle's
     * res/ directory. Absolute paths and ".." are rejected. MOD_UNAVAILABLE if the file does not
     * exist. An empty file loads as data == NULL with size 0. Previous contents of `out_buffer` are
     * overwritten, not freed.
     */
    ModResult (*load)(ModContext* ctx, const char* relative_path, ResourceBuffer* out_buffer);

    /*
     * Release a loaded buffer and reset it to the empty state. Safe to call on an empty or
     * already-freed buffer.
     */
    void (*free)(ModContext* ctx, ResourceBuffer* buffer);
} ResourceService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct dusk::mods::ServiceTraits<ResourceService> {
    static constexpr const char* id = RESOURCE_SERVICE_ID;
    static constexpr uint16_t major_version = RESOURCE_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = RESOURCE_SERVICE_MINOR;
};
#endif
