#pragma once

#include "mods/svc/secret_storage.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace dusk::mods::svc {

class SecretStorageBackend {
public:
    virtual ~SecretStorageBackend() = default;
    virtual SecretStorageResult get(
        std::string_view modId, std::string_view key, std::vector<uint8_t>& value) = 0;
    virtual SecretStorageResult put(
        std::string_view modId, std::string_view key, const uint8_t* data, size_t size) = 0;
    virtual SecretStorageResult remove(std::string_view modId, std::string_view key) = 0;
};

class SecretStorageCore {
public:
    using ModIdResolver = std::function<std::string_view(ModContext*)>;

    SecretStorageCore(ModIdResolver modId, std::unique_ptr<SecretStorageBackend> backend);
    ~SecretStorageCore();

    SecretStorageCore(const SecretStorageCore&) = delete;
    SecretStorageCore& operator=(const SecretStorageCore&) = delete;

    SecretStorageResult get(ModContext* context, const char* key, ResourceBuffer* outBuffer);
    void free(ModContext* context, ResourceBuffer* buffer);
    SecretStorageResult put(ModContext* context, const char* key, const void* data, size_t size);
    SecretStorageResult remove(ModContext* context, const char* key);
    void modDetached(ModContext* context);

private:
    struct BufferOwner {
        const ModContext* context = nullptr;
        size_t size = 0;
    };

    ModIdResolver m_modId;
    std::unique_ptr<SecretStorageBackend> m_backend;
    std::unordered_map<void*, BufferOwner> m_buffers;
};

bool valid_secret_storage_key(std::string_view key);
bool valid_secret_storage_mod_id(std::string_view modId);
void wipe_secret_bytes(void* data, size_t size);

}  // namespace dusk::mods::svc
