#include "secret_storage_core.hpp"
#include "registry.hpp"

#include "dusk/mods/loader/loader.hpp"

#include <memory>

#if defined(__ANDROID__)
#include "secret_storage_android.hpp"
#endif

namespace dusk::mods::svc {
namespace {

class UnavailableSecretStorageBackend final : public SecretStorageBackend {
public:
    SecretStorageResult get(std::string_view, std::string_view, std::vector<uint8_t>& value) override {
        wipe_secret_bytes(value.data(), value.size());
        value.clear();
        return SECRET_STORAGE_UNAVAILABLE;
    }
    SecretStorageResult put(std::string_view, std::string_view, const uint8_t*, size_t) override {
        return SECRET_STORAGE_UNAVAILABLE;
    }
    SecretStorageResult remove(std::string_view, std::string_view) override {
        return SECRET_STORAGE_UNAVAILABLE;
    }
};

std::unique_ptr<SecretStorageCore> s_core;

SecretStorageResult secret_get(ModContext* context, const char* key, ResourceBuffer* outBuffer) {
    return s_core != nullptr ? s_core->get(context, key, outBuffer) : SECRET_STORAGE_UNAVAILABLE;
}

void secret_free(ModContext* context, ResourceBuffer* buffer) {
    if (s_core != nullptr) {
        s_core->free(context, buffer);
    }
}

SecretStorageResult secret_put(
    ModContext* context, const char* key, const void* data, const size_t size) {
    return s_core != nullptr ? s_core->put(context, key, data, size) : SECRET_STORAGE_UNAVAILABLE;
}

SecretStorageResult secret_remove(ModContext* context, const char* key) {
    return s_core != nullptr ? s_core->remove(context, key) : SECRET_STORAGE_UNAVAILABLE;
}

constexpr SecretStorageService s_secretStorageService{
    .header = SERVICE_HEADER(
        SecretStorageService, SECRET_STORAGE_SERVICE_MAJOR, SECRET_STORAGE_SERVICE_MINOR),
    .get = secret_get,
    .free = secret_free,
    .put = secret_put,
    .remove = secret_remove,
};

std::unique_ptr<SecretStorageBackend> make_backend() {
#if defined(__ANDROID__)
    return make_android_secret_storage_backend();
#else
    return std::make_unique<UnavailableSecretStorageBackend>();
#endif
}

}  // namespace

constinit const ServiceModule g_secretStorageModule{
    .id = SECRET_STORAGE_SERVICE_ID,
    .majorVersion = SECRET_STORAGE_SERVICE_MAJOR,
    .minorVersion = SECRET_STORAGE_SERVICE_MINOR,
    .service = &s_secretStorageService,
    .initialize = [] {
        s_core = std::make_unique<SecretStorageCore>(
            [](ModContext* context) -> std::string_view {
                const auto* mod = mod_from_context(context);
                return mod != nullptr ? std::string_view(mod->metadata.id) : std::string_view{};
            },
            make_backend());
    },
    .modDetached = [](LoadedMod& mod) {
        if (s_core != nullptr) {
            s_core->modDetached(mod.context.get());
        }
    },
    .shutdown = [] { s_core.reset(); },
};

}  // namespace dusk::mods::svc
