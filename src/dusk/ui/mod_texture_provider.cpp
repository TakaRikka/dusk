#include "mod_texture_provider.hpp"

#include "dusk/mod_loader.hpp"

#include <fmt/format.h>

namespace dusk::ui {

std::string mod_image_source(const mods::LoadedMod& mod, std::string_view bundlePath) {
    return fmt::format("mod://{}/{}?rev={}", mod.metadata.id, bundlePath, mod.cacheGeneration);
}

}  // namespace dusk::ui

#ifdef AURORA_ENABLE_RMLUI

#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_surface.h>
#include <aurora/lib/logging.hpp>
#include <aurora/rmlui.hpp>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "dusk/mods/loader/loader.hpp"

namespace dusk::ui {
namespace {

aurora::Module Log{"dusk::ui::modTexture"};

constexpr std::string_view kScheme = "mod";
constexpr std::string_view kSourcePrefix = "mod://";
constexpr size_t kMaxCachedImages = 64;
constexpr size_t kMaxImageFileSize = 16 * 1024 * 1024;
constexpr uint32_t kMaxImageDimension = 4096;

struct CachedImage {
    std::vector<uint8_t> pixels;
    uint32_t width = 0;
    uint32_t height = 0;
};

std::unordered_map<std::string, CachedImage>& image_cache() {
    static auto* cache = new std::unordered_map<std::string, CachedImage>();
    return *cache;
}

std::string_view strip_query(std::string_view path) noexcept {
    const auto queryPos = path.find_first_of("?#");
    if (queryPos != std::string_view::npos) {
        path = path.substr(0, queryPos);
    }
    return path;
}

std::optional<CachedImage> decode_png(std::span<const uint8_t> data, std::string_view source) {
    SDL_IOStream* stream = SDL_IOFromConstMem(data.data(), data.size());
    if (stream == nullptr) {
        Log.warn("Failed to open image stream for '{}': {}", source, SDL_GetError());
        return std::nullopt;
    }

    SDL_Surface* loadedSurface = SDL_LoadPNG_IO(stream, true);
    if (loadedSurface == nullptr) {
        Log.warn("Failed to decode image '{}': {}", source, SDL_GetError());
        return std::nullopt;
    }

    SDL_Surface* rgbaSurface = SDL_ConvertSurface(loadedSurface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(loadedSurface);
    if (rgbaSurface == nullptr) {
        Log.warn("Failed to convert image '{}': {}", source, SDL_GetError());
        return std::nullopt;
    }

    const auto width = static_cast<uint32_t>(rgbaSurface->w);
    const auto height = static_cast<uint32_t>(rgbaSurface->h);
    if (width == 0 || height == 0 || width > kMaxImageDimension || height > kMaxImageDimension) {
        Log.warn("Image '{}' has unsupported dimensions {}x{}", source, width, height);
        SDL_DestroySurface(rgbaSurface);
        return std::nullopt;
    }

    const size_t rowSize = static_cast<size_t>(width) * 4;
    CachedImage image{
        .pixels = std::vector<uint8_t>(rowSize * height),
        .width = width,
        .height = height,
    };
    for (uint32_t row = 0; row < height; ++row) {
        const auto* src = static_cast<const uint8_t*>(rgbaSurface->pixels) +
                          static_cast<size_t>(row) * static_cast<size_t>(rgbaSurface->pitch);
        auto* dst = image.pixels.data() + static_cast<size_t>(row) * rowSize;
        std::memcpy(dst, src, rowSize);

        // Convert to premultiplied alpha for correct compositing.
        for (size_t col = 0; col < rowSize; col += 4) {
            const uint8_t alpha = dst[col + 3];
            for (size_t channel = 0; channel < 3; ++channel) {
                dst[col + channel] = static_cast<uint8_t>(
                    (static_cast<uint32_t>(dst[col + channel]) * static_cast<uint32_t>(alpha)) /
                    255);
            }
        }
    }

    SDL_DestroySurface(rgbaSurface);
    return image;
}

std::optional<CachedImage> load_mod_image(std::string_view idAndPath, std::string_view source) {
    const auto slash = idAndPath.find('/');
    if (slash == std::string_view::npos || slash == 0 || slash + 1 >= idAndPath.size()) {
        Log.warn("Malformed mod image source '{}'", source);
        return std::nullopt;
    }
    const std::string modId{idAndPath.substr(0, slash)};
    const std::string path{idAndPath.substr(slash + 1)};
    if (!mods::is_safe_resource_path(path)) {
        Log.warn("Unsafe path in mod image source '{}'", source);
        return std::nullopt;
    }

    std::shared_ptr<mods::ModBundle> bundle;
    for (const auto& mod : mods::ModLoader::instance().mods()) {
        if (mod.metadata.id == modId) {
            bundle = mod.bundle;
            break;
        }
    }
    if (bundle == nullptr) {
        Log.warn("Unknown mod in image source '{}'", source);
        return std::nullopt;
    }

    std::vector<u8> data;
    try {
        if (bundle->getFileSize(path) > kMaxImageFileSize) {
            Log.warn("Image '{}' exceeds the {} MiB limit", source, kMaxImageFileSize >> 20);
            return std::nullopt;
        }
        data = bundle->readFile(path);
    } catch (const std::runtime_error& e) {
        Log.warn("Failed to read image '{}': {}", source, e.what());
        return std::nullopt;
    }

    return decode_png(std::span{data.data(), data.size()}, source);
}

std::optional<aurora::rmlui::RuntimeTexture> mod_texture_provider(std::string_view source) {
    if (!source.starts_with(kSourcePrefix)) {
        return std::nullopt;
    }

    auto& cache = image_cache();
    const std::string key{source};
    auto it = cache.find(key);
    if (it == cache.end()) {
        auto image = load_mod_image(strip_query(source.substr(kSourcePrefix.size())), source);
        if (!image) {
            return std::nullopt;
        }
        if (cache.size() >= kMaxCachedImages) {
            cache.erase(cache.begin());
        }
        it = cache.emplace(key, std::move(*image)).first;
    }

    const auto& image = it->second;
    return aurora::rmlui::RuntimeTexture{
        .width = image.width,
        .height = image.height,
        .rgba8 =
            std::span{reinterpret_cast<const std::byte*>(image.pixels.data()), image.pixels.size()},
        .premultipliedAlpha = true,
    };
}

}  // namespace

void register_mod_texture_provider() noexcept {
    aurora::rmlui::register_texture_provider(std::string{kScheme}, mod_texture_provider);
}

void unregister_mod_texture_provider() noexcept {
    aurora::rmlui::unregister_texture_provider(kScheme);
    image_cache().clear();
}

}  // namespace dusk::ui

#else

namespace dusk::ui {

void register_mod_texture_provider() noexcept {}
void unregister_mod_texture_provider() noexcept {}

}  // namespace dusk::ui

#endif
