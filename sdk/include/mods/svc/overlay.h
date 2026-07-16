#pragma once

#include <mods/api.h>

#define OVERLAY_SERVICE_ID "dev.twilitrealm.dusklight.overlay"
#define OVERLAY_SERVICE_MAJOR 1u
#define OVERLAY_SERVICE_MINOR 0u

/* Handle for a runtime overlay registration. 0 is never a valid handle. */
typedef uint64_t OverlayHandle;

/*
 * Runtime DVD file overlays.
 *
 * Registrations are owned by the calling mod and removed automatically when it is disabled,
 * reloaded, or fails. Changes are applied at the next frame boundary; data the game has already
 * read stays in memory until it re-reads the file (sometimes on scene reload, sometimes on
 * restart).
 *
 * disc_path names the file to overlay: absolute with a leading '/', matched against the disc
 * case-insensitively (e.g. "/res/Stage/R04_00.arc"). Paths that do not exist on the disc are added
 * as new files.
 *
 * If multiple sources overlay the same path, the last one wins: a mod's runtime registrations beat
 * its static overlay/ files, and later-loaded mods beat earlier ones.
 */
typedef struct OverlayService {
    ServiceHeader header;

    /*
     * Overlay disc_path with a file from the calling mod's bundle (bundle-relative path, e.g.
     * "res/replacement.arc"). The file's contents are read lazily on each open, so the bundle
     * file must not change size while registered.
     */
    ModResult (*add_file)(
        ModContext* ctx, const char* disc_path, const char* bundle_path, OverlayHandle* out_handle);

    /*
     * Overlay disc_path with a caller-owned buffer. The data is copied; the caller may free it
     * as soon as this returns.
     */
    ModResult (*add_buffer)(ModContext* ctx, const char* disc_path, const void* data, size_t size,
        OverlayHandle* out_handle);

    /* Remove a runtime overlay previously added by the calling mod. */
    ModResult (*remove)(ModContext* ctx, OverlayHandle handle);
} OverlayService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct mods::ServiceTraits<OverlayService> {
    static constexpr const char* id = OVERLAY_SERVICE_ID;
    static constexpr uint16_t major_version = OVERLAY_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = OVERLAY_SERVICE_MINOR;
};
#endif
