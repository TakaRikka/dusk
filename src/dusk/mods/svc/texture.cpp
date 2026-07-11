#include "registry.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/texture.h"

#include <aurora/texture.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>

using namespace std::string_literals;

static_assert(TEXTURE_HASH_WILDCARD == aurora::texture::kWildcardTextureHash);
static_assert(TEXTURE_TLUT_WILDCARD == aurora::texture::kWildcardTlutHash);

namespace dusk::mods::svc {
namespace {

struct TextureRawData {
    std::vector<u8> data;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t mipCount = 1;
    uint32_t gxFormat = 0;
};

aurora::Module Log("dusk::mods::textures");

// Referenced by Aurora's lazy virtual-file reads (from arbitrary threads, under Aurora's registry
// lock) and by raw-entry spans. Immutable after construction; freed only after the corresponding
// unregister_replacement returns, at which point Aurora guarantees no further reads.
struct TextureKeepalive {
    std::shared_ptr<ModBundle> bundle;
    std::string bundlePath;
    std::vector<u8> ownedData;
};

// Called with Aurora's registry lock held: must not take any Dusk lock or re-enter
// aurora::texture. ModBundle reads are documented thread-safe.
bool texture_read_cb(void* userData, const char* path, std::vector<uint8_t>& outBytes) {
    auto* keepalive = static_cast<TextureKeepalive*>(userData);
    try {
        outBytes = keepalive->bundle->readFile(path);
        return true;
    } catch (...) {
        return false;
    }
}

struct RuntimeTextureEntry {
    uint64_t handle = 0;
    aurora::texture::ReplacementRegistration registration;
    std::shared_ptr<TextureKeepalive> keepalive;
    // Original inputs, kept for re-registration when the mod's priority changes.
    bool isVirtual = false;
    aurora::texture::ReplacementKey key;  // raw entries only
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t mipCount = 1;
    uint32_t gxFormat = 0;
    std::string label;
};

struct ModTextureRecord {
    int32_t appliedPriority = 0;
    bool staticRegistered = false;
    aurora::texture::ReplacementGroup staticGroup;
    std::vector<std::shared_ptr<TextureKeepalive>> staticKeepalives;
    std::vector<RuntimeTextureEntry> runtime;
};

// Game thread only: all mutations happen in service calls made from mod code (init/update/hooks
// run inside ModLoader::tick), in the loader's sync/deactivate paths, or at shutdown.
std::unordered_map<const LoadedMod*, ModTextureRecord> s_modTextures;
uint64_t s_nextTextureHandle = 1;

// Position in m_mods (dependency-sorted load order) + 1; later-loaded mods win. The user
// texture_replacements directory uses kUserTextureReplacementPriority, below any mod.
int32_t compute_mod_priority(const LoadedMod& mod) {
    int32_t index = 0;
    for (const auto& other : ModLoader::instance().mods()) {
        ++index;
        if (&other == &mod) {
            return index;
        }
    }
    return index + 1;
}

bool is_sidecar_mip(std::string_view stem) {
    constexpr std::string_view tag = "_mip";
    size_t i = stem.size();
    while (i > 0 && stem[i - 1] >= '0' && stem[i - 1] <= '9') {
        --i;
    }
    if (i == stem.size() || i < tag.size()) {
        return false;
    }
    return stem.substr(i - tag.size(), tag.size()) == tag;
}

bool has_replacement_extension(std::string_view filename) {
    const auto dot = filename.rfind('.');
    if (dot == std::string_view::npos) {
        return false;
    }
    std::string ext{filename.substr(dot)};
    std::ranges::transform(ext, ext.begin(),
        [](char ch) { return ch >= 'A' && ch <= 'Z' ? static_cast<char>(ch + 'a' - 'A') : ch; });
    return ext == ".dds" || ext == ".png";
}

std::string_view final_path_component(std::string_view path) {
    const auto slash = path.rfind('/');
    return slash == std::string_view::npos ? path : path.substr(slash + 1);
}

const LoadedMod* find_static_conflict(
    const aurora::texture::ReplacementKey& key, const LoadedMod* exclude) {
    for (const auto& [mod, record] : s_modTextures) {
        if (mod == exclude) {
            continue;
        }
        for (const auto& registration : record.staticGroup.registrations) {
            if (registration.key == key) {
                return mod;
            }
        }
    }
    return nullptr;
}

void register_static_textures(LoadedMod& mod, ModTextureRecord& record) {
    std::vector<std::string> candidates;
    for (const auto& file : mod.bundle->getFileNames()) {
        if (!file.starts_with("textures/") || !has_replacement_extension(file)) {
            continue;
        }
        auto filename = final_path_component(file);
        if (is_sidecar_mip(filename.substr(0, filename.rfind('.')))) {
            continue;
        }
        candidates.push_back(file);
    }
    // Deterministic order; with the first parse of a key winning, this mirrors Aurora's
    // load_replacement_directory dedupe semantics.
    std::ranges::sort(candidates);

    std::vector<aurora::texture::ReplacementKey> seenKeys;
    for (const auto& path : candidates) {
        const auto parsed = aurora::texture::parse_replacement_filename(final_path_component(path));
        if (!parsed.has_value()) {
            Log.warn(
                "[{}] '{}' does not follow the texture replacement naming convention; skipped.",
                mod.metadata.id, path);
            continue;
        }
        const aurora::texture::ReplacementKey key{*parsed};
        if (std::ranges::find(seenKeys, key) != seenKeys.end()) {
            continue;
        }
        seenKeys.push_back(key);

        if (const auto* other = find_static_conflict(key, &mod); other != nullptr) {
            const auto& winner =
                s_modTextures.find(other)->second.appliedPriority > record.appliedPriority ?
                    *other :
                    mod;
            Log.warn(
                "Texture replacement conflict: '{}' is replaced by both '{}' and '{}'; '{}' wins.",
                path, other->metadata.id, mod.metadata.id, winner.metadata.id);
        }

        auto keepalive = std::make_shared<TextureKeepalive>(mod.bundle, path);
        const auto registration = aurora::texture::register_virtual_replacement(path,
            {.read = texture_read_cb, .userData = keepalive.get()},
            {.priority = record.appliedPriority});
        if (registration.id == 0) {
            continue;
        }
        record.staticGroup.registrations.push_back(registration);
        record.staticKeepalives.push_back(std::move(keepalive));
    }

    record.staticRegistered = true;
    if (!record.staticGroup.registrations.empty()) {
        Log.info("[{}] registered {} texture replacement(s).", mod.metadata.id,
            record.staticGroup.registrations.size());
    }
}

void register_runtime_entry(RuntimeTextureEntry& entry, int32_t priority) {
    if (entry.isVirtual) {
        entry.registration = aurora::texture::register_virtual_replacement(
            entry.keepalive->bundlePath,
            {.read = texture_read_cb, .userData = entry.keepalive.get()}, {.priority = priority});
    } else {
        entry.registration = aurora::texture::register_replacement(entry.key,
            {
                .bytes = {entry.keepalive->ownedData.data(), entry.keepalive->ownedData.size()},
                .width = entry.width,
                .height = entry.height,
                .mipCount = entry.mipCount,
                .gxFormat = entry.gxFormat,
                .label = entry.label,
            },
            {.priority = priority});
    }
}

void unregister_record(ModTextureRecord& record) {
    aurora::texture::unregister_replacements(record.staticGroup);
    record.staticGroup.registrations.clear();
    record.staticKeepalives.clear();
    record.staticRegistered = false;
    for (auto& entry : record.runtime) {
        aurora::texture::unregister_replacement(entry.registration);
        entry.registration = {};
    }
}

void textures_sync_replacements() {
    // Module detach removes records eagerly, but a record whose mod is no
    // longer active must not linger with stale priority.
    std::erase_if(s_modTextures, [&](auto& item) {
        if (item.first->active) {
            return false;
        }
        unregister_record(item.second);
        return true;
    });

    for (auto& mod : ModLoader::instance().active_mods()) {
        const auto priority = compute_mod_priority(mod);
        auto& record = s_modTextures[&mod];

        if (record.staticRegistered && record.appliedPriority == priority) {
            continue;
        }

        if (record.staticRegistered) {
            // A reload re-sorted m_mods and changed this mod's priority: re-register everything
            // at the new priority. Cheap, since file-backed entries decode lazily.
            aurora::texture::unregister_replacements(record.staticGroup);
            record.staticGroup.registrations.clear();
            record.staticKeepalives.clear();
            record.appliedPriority = priority;
            register_static_textures(mod, record);
            for (auto& entry : record.runtime) {
                aurora::texture::unregister_replacement(entry.registration);
                register_runtime_entry(entry, priority);
            }
        } else {
            record.appliedPriority = priority;
            register_static_textures(mod, record);
        }
    }
}

uint64_t texture_register_raw(
    LoadedMod& mod, const aurora::texture::ReplacementKey& key, TextureRawData data) {
    auto& record = s_modTextures[&mod];
    if (record.appliedPriority == 0) {
        record.appliedPriority = compute_mod_priority(mod);
    }

    auto& entry = record.runtime.emplace_back();
    entry.handle = s_nextTextureHandle++;
    entry.keepalive = std::make_shared<TextureKeepalive>();
    entry.keepalive->ownedData = std::move(data.data);
    entry.isVirtual = false;
    entry.key = key;
    entry.width = data.width;
    entry.height = data.height;
    entry.mipCount = data.mipCount;
    entry.gxFormat = data.gxFormat;
    entry.label = fmt::format("mod {} texture {}", mod.metadata.id, entry.handle);
    register_runtime_entry(entry, record.appliedPriority);
    if (entry.registration.id == 0) {
        Log.error("[{}] texture register_data failed: replacement was rejected", mod.metadata.id);
        record.runtime.pop_back();
        return 0;
    }
    return entry.handle;
}

uint64_t texture_register_file(LoadedMod& mod, std::string bundlePath) {
    auto& record = s_modTextures[&mod];
    if (record.appliedPriority == 0) {
        record.appliedPriority = compute_mod_priority(mod);
    }

    auto& entry = record.runtime.emplace_back();
    entry.handle = s_nextTextureHandle++;
    entry.keepalive = std::make_shared<TextureKeepalive>(mod.bundle, std::move(bundlePath));
    entry.isVirtual = true;
    register_runtime_entry(entry, record.appliedPriority);
    if (entry.registration.id == 0) {
        record.runtime.pop_back();
        return 0;
    }
    return entry.handle;
}

bool texture_unregister(LoadedMod& mod, uint64_t handle) {
    const auto it = s_modTextures.find(&mod);
    if (it == s_modTextures.end()) {
        return false;
    }
    auto& runtime = it->second.runtime;
    const auto entry =
        std::ranges::find_if(runtime, [&](const auto& e) { return e.handle == handle; });
    if (entry == runtime.end()) {
        return false;
    }
    aurora::texture::unregister_replacement(entry->registration);
    runtime.erase(entry);
    return true;
}

void textures_remove_mod(LoadedMod& mod) {
    const auto it = s_modTextures.find(&mod);
    if (it == s_modTextures.end()) {
        return;
    }
    unregister_record(it->second);
    s_modTextures.erase(it);
}

std::optional<aurora::texture::ReplacementKey> translate_key(const TextureKey* key) {
    if (key == nullptr || key->struct_size < sizeof(TextureKey)) {
        return std::nullopt;
    }
    switch (key->kind) {
    case TEXTURE_KEY_POINTER:
        if (key->pointer == nullptr) {
            return std::nullopt;
        }
        return aurora::texture::ReplacementKey{aurora::texture::TexturePointerKey{key->pointer}};
    case TEXTURE_KEY_SOURCE:
        if (key->width == 0 || key->height == 0) {
            return std::nullopt;
        }
        return aurora::texture::ReplacementKey{aurora::texture::TextureSourceKey{
            .textureHash = key->texture_hash,
            .tlutHash = key->tlut_hash,
            .width = key->width,
            .height = key->height,
            .format = key->gx_format,
            .hasTlut = key->has_tlut,
        }};
    default:
        return std::nullopt;
    }
}

ModResult texture_register_data(ModContext* context, const TextureKey* key, const TextureData* data,
    TextureReplacementHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    const auto translatedKey = translate_key(key);
    if (mod == nullptr || !translatedKey.has_value() || data == nullptr ||
        data->struct_size < sizeof(TextureData) || data->data == nullptr || data->size == 0 ||
        data->width == 0 || data->height == 0 || data->mip_count == 0)
    {
        return MOD_INVALID_ARGUMENT;
    }

    const auto* bytes = static_cast<const u8*>(data->data);
    const auto handle = texture_register_raw(*mod, *translatedKey,
        {
            .data = std::vector<u8>{bytes, bytes + data->size},
            .width = data->width,
            .height = data->height,
            .mipCount = data->mip_count,
            .gxFormat = data->gx_format,
        });
    if (handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return MOD_OK;
}

ModResult texture_register_file(
    ModContext* context, const char* bundlePath, TextureReplacementHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || bundlePath == nullptr || !is_safe_resource_path(bundlePath)) {
        return MOD_INVALID_ARGUMENT;
    }

    const std::string_view path{bundlePath};
    const auto slash = path.rfind('/');
    const auto filename = slash == std::string_view::npos ? path : path.substr(slash + 1);
    if (!aurora::texture::parse_replacement_filename(filename).has_value()) {
        Log.error("[{}] texture register_file '{}' failed: "
                      "filename does not follow the replacement naming convention",
            mod->metadata.id, bundlePath);
        return MOD_INVALID_ARGUMENT;
    }

    try {
        mod->bundle->getFileSize(bundlePath);
    } catch (const std::exception& e) {
        Log.error(
            "[{}] texture register_file '{}' failed: {}", mod->metadata.id, bundlePath, e.what());
        return MOD_UNAVAILABLE;
    }

    const auto handle = texture_register_file(*mod, bundlePath);
    if (handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return MOD_OK;
}

ModResult texture_unregister(ModContext* context, TextureReplacementHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    if (!texture_unregister(*mod, handle)) {
        Log.error(
            "[{}] texture unregister failed: unknown handle {}", mod->metadata.id, handle);
        return MOD_INVALID_ARGUMENT;
    }
    return MOD_OK;
}

constexpr TextureService s_textureService{
    .header = SERVICE_HEADER(TextureService, TEXTURE_SERVICE_MAJOR, TEXTURE_SERVICE_MINOR),
    .register_data = texture_register_data,
    .register_file = texture_register_file,
    .unregister = texture_unregister,
};

}  // namespace

constinit const ServiceModule g_textureModule{
    .id = TEXTURE_SERVICE_ID,
    .majorVersion = TEXTURE_SERVICE_MAJOR,
    .minorVersion = TEXTURE_SERVICE_MINOR,
    .service = &s_textureService,
    .modDetached = textures_remove_mod,
    .lifecycleApplied = textures_sync_replacements,
};

}  // namespace dusk::mods::svc
