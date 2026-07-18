#pragma once

#include <mods/api.h>

#define TEXTURE_SERVICE_ID "dev.twilitrealm.dusklight.texture"
#define TEXTURE_SERVICE_MAJOR 1u
#define TEXTURE_SERVICE_MINOR 0u

/* Handle for a runtime texture replacement registration. 0 is never a valid handle. */
typedef uint64_t TextureReplacementHandle;

typedef enum TextureKeyKind {
    /* Match a texture by the address of its in-memory GX texel data. */
    TEXTURE_KEY_POINTER = 0,
    /* Match by content: XXH64 of the base mip level (and of the referenced TLUT range for
     * palette formats), as encoded in replacement filenames / texture dumps. */
    TEXTURE_KEY_SOURCE = 1,
} TextureKeyKind;

/* Wildcard values for TEXTURE_KEY_SOURCE hashes ("$" in the filename convention). */
#define TEXTURE_HASH_WILDCARD UINT64_C(0xFFFFFFFFFFFFFFFF)
#define TEXTURE_TLUT_WILDCARD UINT64_C(0xFFFFFFFFFFFFFFFE)

typedef struct TextureKey {
    uint32_t struct_size;
    TextureKeyKind kind;
    const void* pointer;   /* TEXTURE_KEY_POINTER only */
    uint64_t texture_hash; /* TEXTURE_KEY_SOURCE */
    uint64_t tlut_hash;    /* TEXTURE_KEY_SOURCE, palette formats only */
    uint32_t width;
    uint32_t height;
    uint32_t gx_format;
    bool has_tlut;
} TextureKey;

#define TEXTURE_KEY_INIT {sizeof(TextureKey), TEXTURE_KEY_POINTER, NULL, 0u, 0u, 0u, 0u, 0u, false}

typedef struct TextureData {
    uint32_t struct_size;
    const void* data; /* texel data laid out in gx_format; copied by the service */
    size_t size;
    uint32_t width;
    uint32_t height;
    uint32_t mip_count;
    uint32_t gx_format; /* any GX texture format supported by Aurora's converter */
} TextureData;

#define TEXTURE_DATA_INIT {sizeof(TextureData), NULL, 0u, 0u, 0u, 1u, 0u}

/*
 * Runtime texture replacements.
 *
 * Registrations are owned by the calling mod and removed automatically when it is disabled,
 * reloaded, or fails. When multiple sources replace the same texture, the highest priority wins:
 * later-loaded mods beat earlier ones, and any mod beats the user's texture_replacements config
 * directory. Files shipped in a mod's textures/ directory register automatically with the same
 * ownership and priority; this service is for replacements decided at runtime.
 */
typedef struct TextureService {
    ServiceHeader header;

    /* Register a replacement from raw texel data. The data is copied; the caller may free it as
     * soon as this returns. */
    ModResult (*register_data)(ModContext* ctx, const TextureKey* key, const TextureData* data,
        TextureReplacementHandle* out_handle);

    /*
     * Register a replacement from an encoded .dds/.png inside the calling mod's bundle. The
     * filename encodes the key (same convention as the texture_replacements directory, e.g.
     * "tex1_{w}x{h}_{hash}_{fmt}.dds"); "_mipN" sidecars next to it are picked up automatically.
     * The file is decoded lazily on first use by the renderer.
     */
    ModResult (*register_file)(ModContext* ctx, const char* bundle_path,
        TextureReplacementHandle* out_handle);

    /* Remove a replacement previously registered by the calling mod. */
    ModResult (*unregister)(ModContext* ctx, TextureReplacementHandle handle);
} TextureService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct mods::ServiceTraits<TextureService> {
    static constexpr const char* id = TEXTURE_SERVICE_ID;
    static constexpr uint16_t major_version = TEXTURE_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = TEXTURE_SERVICE_MINOR;
};
#endif
