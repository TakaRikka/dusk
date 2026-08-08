#include "mods/svc/secret_storage.h"

SecretStorageResult secret_storage_c_testmod_roundtrip(
    const SecretStorageService* service, ModContext* context, const char* key) {
    static const char value[] = "c-table";
    ResourceBuffer buffer = RESOURCE_BUFFER_INIT;
    if (service == 0 || service->header.major_version != SECRET_STORAGE_SERVICE_MAJOR ||
        service->put(context, key, value, sizeof(value) - 1) != SECRET_STORAGE_OK ||
        service->get(context, key, &buffer) != SECRET_STORAGE_OK ||
        buffer.size != sizeof(value) - 1)
    {
        return SECRET_STORAGE_IO_ERROR;
    }
    service->free(context, &buffer);
    return service->remove(context, key);
}
