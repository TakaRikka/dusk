#include "registry.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/resource.h"

#include <fmt/format.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

namespace dusk::mods::svc {
namespace {

aurora::Module Log("dusk::mods::resource");

// Allocations by owning mod, so buffers still live when a mod detaches can be freed.
std::unordered_map<void*, const LoadedMod*> s_buffers;

void resource_remove_mod(LoadedMod& mod) {
    size_t reclaimed = 0;
    std::erase_if(s_buffers, [&](const auto& entry) {
        if (entry.second != &mod) {
            return false;
        }
        std::free(entry.first);
        ++reclaimed;
        return true;
    });
    if (reclaimed != 0) {
        Log.warn("[{}] reclaimed {} resource buffer(s) that were never freed", mod.metadata.id,
            reclaimed);
    }
}

ModResult resource_load(ModContext* context, const char* relativePath, ResourceBuffer* outBuffer) {
    if (outBuffer == nullptr || outBuffer->struct_size < sizeof(ResourceBuffer)) {
        return MOD_INVALID_ARGUMENT;
    }
    outBuffer->data = nullptr;
    outBuffer->size = 0;
    auto* mod = mod_from_context(context);
    if (mod == nullptr || relativePath == nullptr || !is_safe_resource_path(relativePath)) {
        return MOD_INVALID_ARGUMENT;
    }

    const auto entry = fmt::format("res/{}", relativePath);
    std::vector<u8> data;
    try {
        data = mod->bundle->readFile(entry);
    } catch (const std::runtime_error& e) {
        Log.error("[{}] resource load '{}' failed: {}", mod->metadata.id, entry, e.what());
        return MOD_UNAVAILABLE;
    }

    if (!data.empty()) {
        void* copy = std::malloc(data.size());
        if (copy == nullptr) {
            return MOD_ERROR;
        }
        std::memcpy(copy, data.data(), data.size());
        s_buffers.emplace(copy, mod);
        outBuffer->data = copy;
        outBuffer->size = data.size();
    }
    return MOD_OK;
}

void resource_free(ModContext* context, ResourceBuffer* buffer) {
    if (buffer == nullptr || buffer->struct_size < sizeof(ResourceBuffer) ||
        buffer->data == nullptr)
    {
        return;
    }
    auto* mod = mod_from_context(context);
    const auto it = s_buffers.find(buffer->data);
    if (it == s_buffers.end()) {
        Log.error("[{}] resource free: not a live loaded buffer", mod_id_from_context(context));
        return;
    }
    if (mod == nullptr || it->second != mod) {
        Log.error("[{}] resource free: buffer is owned by '{}'", mod_id_from_context(context),
            it->second != nullptr ? it->second->metadata.id : "unknown");
        return;
    }
    s_buffers.erase(it);
    std::free(buffer->data);
    buffer->data = nullptr;
    buffer->size = 0;
}

constexpr ResourceService s_resourceService{
    .header = SERVICE_HEADER(ResourceService, RESOURCE_SERVICE_MAJOR, RESOURCE_SERVICE_MINOR),
    .load = resource_load,
    .free = resource_free,
};

}  // namespace

constinit const ServiceModule g_resourceModule{
    .id = RESOURCE_SERVICE_ID,
    .majorVersion = RESOURCE_SERVICE_MAJOR,
    .minorVersion = RESOURCE_SERVICE_MINOR,
    .service = &s_resourceService,
    .modDetached = resource_remove_mod,
};

}  // namespace dusk::mods::svc
