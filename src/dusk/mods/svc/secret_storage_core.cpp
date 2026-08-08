#include "secret_storage_core.hpp"

#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>

namespace dusk::mods::svc {
namespace {

std::mutex s_secretStorageMutex;

bool valid_label(const std::string_view label) {
    if (label.empty() || label.size() > SECRET_STORAGE_MOD_ID_LABEL_MAX_SIZE) {
        return false;
    }
    for (const unsigned char character : label) {
        if (!((character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') ||
                character == '_'))
        {
            return false;
        }
    }
    return true;
}

bool bounded_c_string(const char* text, const size_t maximum, std::string_view& result) {
    if (text == nullptr) {
        return false;
    }
    const void* terminator = std::memchr(text, '\0', maximum + 1);
    if (terminator == nullptr) {
        return false;
    }
    result = std::string_view(text, static_cast<const char*>(terminator) - text);
    return true;
}

SecretStorageResult validate(
    ModContext* context, const SecretStorageCore::ModIdResolver& modIdResolver, const char* key,
    const void* data, const size_t size, std::string_view& modId, std::string_view& keyText) {
    if (size > SECRET_STORAGE_VALUE_MAX_SIZE) {
        return SECRET_STORAGE_TOO_LARGE;
    }
    if (context == nullptr || (data == nullptr && size != 0) ||
        !bounded_c_string(key, SECRET_STORAGE_KEY_MAX_SIZE, keyText) ||
        !valid_secret_storage_key(keyText))
    {
        return SECRET_STORAGE_INVALID_ARGUMENT;
    }
    modId = modIdResolver(context);
    return valid_secret_storage_mod_id(modId) ? SECRET_STORAGE_OK : SECRET_STORAGE_INVALID_ARGUMENT;
}

}  // namespace

void wipe_secret_bytes(void* data, size_t size) {
    volatile auto* bytes = static_cast<volatile uint8_t*>(data);
    while (bytes != nullptr && size != 0) {
        *bytes++ = 0;
        --size;
    }
}

bool valid_secret_storage_key(const std::string_view key) {
    if (key.empty() || key.size() > SECRET_STORAGE_KEY_MAX_SIZE || key.front() == '.') {
        return false;
    }
    for (const unsigned char character : key) {
        if (!((character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') ||
                character == '_' || character == '.' || character == '-'))
        {
            return false;
        }
    }
    return true;
}

bool valid_secret_storage_mod_id(const std::string_view modId) {
    if (modId.empty() || modId.size() > SECRET_STORAGE_MOD_ID_MAX_SIZE || modId.front() == '.' ||
        modId.back() == '.')
    {
        return false;
    }
    size_t start = 0;
    while (start < modId.size()) {
        const size_t end = modId.find('.', start);
        if (!valid_label(modId.substr(start, end == std::string_view::npos ? end : end - start))) {
            return false;
        }
        if (end == std::string_view::npos) {
            return true;
        }
        start = end + 1;
    }
    return false;
}

SecretStorageCore::SecretStorageCore(
    ModIdResolver modId, std::unique_ptr<SecretStorageBackend> backend)
    : m_modId(std::move(modId)), m_backend(std::move(backend)) {}

SecretStorageCore::~SecretStorageCore() {
    std::lock_guard lock(s_secretStorageMutex);
    for (const auto& [data, owner] : m_buffers) {
        wipe_secret_bytes(data, owner.size);
        std::free(data);
    }
    m_buffers.clear();
}

SecretStorageResult SecretStorageCore::get(
    ModContext* context, const char* key, ResourceBuffer* outBuffer) {
    if (outBuffer == nullptr || outBuffer->struct_size < sizeof(ResourceBuffer)) {
        return SECRET_STORAGE_INVALID_ARGUMENT;
    }
    outBuffer->data = nullptr;
    outBuffer->size = 0;

    std::string_view modId;
    std::string_view keyText;
    if (const auto result = validate(context, m_modId, key, nullptr, 0, modId, keyText);
        result != SECRET_STORAGE_OK)
    {
        return result;
    }

    std::vector<uint8_t> value;
    std::lock_guard lock(s_secretStorageMutex);
    if (m_backend == nullptr) {
        return SECRET_STORAGE_UNAVAILABLE;
    }
    const SecretStorageResult result = m_backend->get(modId, keyText, value);
    if (result != SECRET_STORAGE_OK) {
        wipe_secret_bytes(value.data(), value.size());
        return result;
    }
    if (value.size() > SECRET_STORAGE_VALUE_MAX_SIZE) {
        wipe_secret_bytes(value.data(), value.size());
        return SECRET_STORAGE_CORRUPT;
    }
    if (!value.empty()) {
        void* allocation = std::malloc(value.size());
        if (allocation == nullptr) {
            wipe_secret_bytes(value.data(), value.size());
            return SECRET_STORAGE_IO_ERROR;
        }
        std::memcpy(allocation, value.data(), value.size());
        m_buffers.emplace(allocation, BufferOwner{.context = context, .size = value.size()});
        outBuffer->data = allocation;
        outBuffer->size = value.size();
    }
    wipe_secret_bytes(value.data(), value.size());
    return SECRET_STORAGE_OK;
}

void SecretStorageCore::free(ModContext* context, ResourceBuffer* buffer) {
    if (buffer == nullptr || buffer->struct_size < sizeof(ResourceBuffer) || buffer->data == nullptr) {
        return;
    }
    std::lock_guard lock(s_secretStorageMutex);
    const auto found = m_buffers.find(buffer->data);
    if (found == m_buffers.end() || found->second.context != context) {
        return;
    }
    wipe_secret_bytes(buffer->data, found->second.size);
    std::free(buffer->data);
    m_buffers.erase(found);
    buffer->data = nullptr;
    buffer->size = 0;
}

SecretStorageResult SecretStorageCore::put(
    ModContext* context, const char* key, const void* data, const size_t size) {
    std::string_view modId;
    std::string_view keyText;
    if (const auto result = validate(context, m_modId, key, data, size, modId, keyText);
        result != SECRET_STORAGE_OK)
    {
        return result;
    }
    std::vector<uint8_t> copy(size);
    if (size != 0) {
        std::memcpy(copy.data(), data, size);
    }
    std::lock_guard lock(s_secretStorageMutex);
    const SecretStorageResult result =
        m_backend == nullptr ? SECRET_STORAGE_UNAVAILABLE : m_backend->put(modId, keyText, copy.data(), size);
    wipe_secret_bytes(copy.data(), copy.size());
    return result;
}

SecretStorageResult SecretStorageCore::remove(ModContext* context, const char* key) {
    std::string_view modId;
    std::string_view keyText;
    if (const auto result = validate(context, m_modId, key, nullptr, 0, modId, keyText);
        result != SECRET_STORAGE_OK)
    {
        return result;
    }
    std::lock_guard lock(s_secretStorageMutex);
    return m_backend == nullptr ? SECRET_STORAGE_UNAVAILABLE : m_backend->remove(modId, keyText);
}

void SecretStorageCore::modDetached(ModContext* context) {
    std::lock_guard lock(s_secretStorageMutex);
    for (auto it = m_buffers.begin(); it != m_buffers.end();) {
        if (it->second.context == context) {
            wipe_secret_bytes(it->first, it->second.size);
            std::free(it->first);
            it = m_buffers.erase(it);
        } else {
            ++it;
        }
    }
}

}  // namespace dusk::mods::svc
