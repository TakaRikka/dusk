#pragma once

#include <mods/api.h>
#include <mods/svc/resource.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define SECRET_STORAGE_SERVICE_ID "dev.twilitrealm.dusklight.secret_storage"
#define SECRET_STORAGE_SERVICE_MAJOR 1u
#define SECRET_STORAGE_SERVICE_MINOR 0u

#define SECRET_STORAGE_KEY_MAX_SIZE 128u
#define SECRET_STORAGE_MOD_ID_MAX_SIZE 240u
#define SECRET_STORAGE_MOD_ID_LABEL_MAX_SIZE 64u
#define SECRET_STORAGE_VALUE_MAX_SIZE (64u * 1024u)

typedef enum SecretStorageResult {
    SECRET_STORAGE_OK = 0,
    SECRET_STORAGE_NOT_FOUND = 1,
    SECRET_STORAGE_INVALID_ARGUMENT = 2,
    SECRET_STORAGE_TOO_LARGE = 3,
    SECRET_STORAGE_UNAVAILABLE = 4,
    SECRET_STORAGE_CORRUPT = 5,
    SECRET_STORAGE_KEY_INVALIDATED = 6,
    SECRET_STORAGE_IO_ERROR = 7,
} SecretStorageResult;

#if defined(__cplusplus)
static_assert(sizeof(SecretStorageResult) == sizeof(int));
#else
_Static_assert(sizeof(SecretStorageResult) == sizeof(int), "SecretStorageResult ABI size");
#endif

/*
 * Private secret storage for the calling mod. Keys are 1..128 lowercase ASCII
 * [a-z0-9_.-] with no leading dot; values are at most 65536 bytes.
 *
 * get overwrites out_buffer with an empty buffer on every entry/failure without
 * freeing previous contents: callers must free a prior successful buffer first.
 * Empty values return data == NULL and size == 0. free only releases buffers
 * returned to the same ModContext; a wrong-owner free leaves the buffer intact.
 */
typedef struct SecretStorageService {
    ServiceHeader header;

    SecretStorageResult (*get)(ModContext* ctx, const char* key, ResourceBuffer* out_buffer);
    void (*free)(ModContext* ctx, ResourceBuffer* buffer);
    SecretStorageResult (*put)(ModContext* ctx, const char* key, const void* data, size_t size);
    SecretStorageResult (*remove)(ModContext* ctx, const char* key);
} SecretStorageService;

MOD_DECLARE_SERVICE(SecretStorageService, svc_secret_storage, SECRET_STORAGE_SERVICE_ID,
    SECRET_STORAGE_SERVICE_MAJOR, SECRET_STORAGE_SERVICE_MINOR);
