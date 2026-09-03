#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define FILE_SERVICE_ID "dev.twilitrealm.dusklight.file"
#define FILE_SERVICE_MAJOR 1u
#define FILE_SERVICE_MINOR 0u

typedef uint64_t FileStreamHandle;

typedef enum FileOpenMode {
    FILE_OPEN_READ = 0,
    FILE_OPEN_TRUNCATE = 1,
    FILE_OPEN_APPEND = 2,
} FileOpenMode;

typedef struct FileFilter {
    const char* name;
    /* Semicolon-separated extensions or "*". */
    const char* pattern;
} FileFilter;

typedef struct FilePickOptions {
    uint32_t struct_size;
    const FileFilter* filters;
    uint32_t filter_count;
    /* Optional previously returned location. */
    const char* default_location;
} FilePickOptions;

#define FILE_PICK_OPTIONS_INIT {sizeof(FilePickOptions), NULL, 0u, NULL}

/* Locations and error are valid only for the duration of the callback. Canceled picks report
 * MOD_UNAVAILABLE. The callback runs on the game thread. */
typedef void (*FilePickFn)(ModContext* ctx, ModResult status, const char* const* locations,
    uint32_t location_count, const char* error, void* user_data);

typedef struct FileBuffer {
    uint32_t struct_size;
    void* data;
    size_t size;
} FileBuffer;

#define FILE_BUFFER_INIT {sizeof(FileBuffer), NULL, 0u}

typedef struct FileEntry {
    const char* name;
    const char* location;
    bool is_directory;
} FileEntry;

/* Called once per entry, then once with entry == NULL. Entries are valid only for the duration of
 * the callback. */
typedef void (*FileListFn)(ModContext* ctx, const FileEntry* entry, void* user_data);

/*
 * Access to user-selected files and folders.
 *
 * A location is an opaque UTF-8 string returned by `pick_*`, `export_file`, `join`, `create_child`.
 * Save it and pass it back to the service. Never parse it or manually append path segments.
 *
 * Android: Security grants are restored across launches. Only 512 (or 128 before API 30) grants are
 * allowed at a time. If a grant cannot be retained or was revoked, `check`/`open` returns
 * MOD_UNAVAILABLE so the user can select the location again.
 *
 * Calls other than picker completion are synchronous and must run on the game thread. Avoid file
 * reads and folder traversal in per-frame callbacks.
 */
typedef struct FileService {
    ServiceHeader header;

    /* One native dialog is allowed at a time. Returns MOD_CONFLICT while one is outstanding. */
    ModResult (*pick_file)(
        ModContext* ctx, const FilePickOptions* options, FilePickFn fn, void* user_data);
    ModResult (*pick_folder)(
        ModContext* ctx, const FilePickOptions* options, FilePickFn fn, void* user_data);
    /* Exports an existing file to a user-chosen destination. fn receives its final location. */
    ModResult (*export_file)(ModContext* ctx, const char* source_location,
        const char* suggested_name, FilePickFn fn, void* user_data);

    ModResult (*display_name)(
        ModContext* ctx, const char* location, char* buffer, uint32_t buffer_size);
    /* MOD_OK, MOD_UNAVAILABLE when gone or inaccessible, or MOD_UNSUPPORTED. */
    ModResult (*check)(ModContext* ctx, const char* location);

    /* Write modes require an existing writable location and return MOD_UNSUPPORTED otherwise. */
    ModResult (*open)(
        ModContext* ctx, const char* location, FileOpenMode mode, FileStreamHandle* out_handle);
    ModResult (*size)(ModContext* ctx, FileStreamHandle handle, uint64_t* out_size);
    ModResult (*read)(ModContext* ctx, FileStreamHandle handle, void* buffer, uint64_t length,
        uint64_t* out_read);
    ModResult (*write)(
        ModContext* ctx, FileStreamHandle handle, const void* buffer, uint64_t length);
    ModResult (*seek)(ModContext* ctx, FileStreamHandle handle, uint64_t offset);
    ModResult (*flush)(ModContext* ctx, FileStreamHandle handle);
    /* Reports flush and close failures; writers must check the result. */
    ModResult (*close)(ModContext* ctx, FileStreamHandle handle);

    ModResult (*read_all)(ModContext* ctx, const char* location, FileBuffer* out_buffer);
    /* Truncates and writes an existing location. This operation is not atomic. */
    ModResult (*write_all)(ModContext* ctx, const char* location, const void* data, size_t size);
    void (*free)(ModContext* ctx, FileBuffer* buffer);

    ModResult (*list)(ModContext* ctx, const char* folder_location, FileListFn fn, void* user_data);
    /* out_location remains valid until this mod's next `join` or `create_child` call. */
    ModResult (*join)(ModContext* ctx, const char* folder_location, const char* relative_path,
        const char** out_location);

    /* Creates one file without replacing an existing child. */
    ModResult (*create_child)(
        ModContext* ctx, const char* folder_location, const char* name, const char** out_location);
} FileService;

MOD_DECLARE_SERVICE(FileService, svc_file, FILE_SERVICE_ID, FILE_SERVICE_MAJOR, FILE_SERVICE_MINOR);
