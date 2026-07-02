#include "icon_provider.hpp"

#include "d/dolzel.h"  // IWYU pragma: keep

#ifdef AURORA_ENABLE_RMLUI

#include <SDL3/SDL_surface.h>
#include <aurora/lib/gfx/texture_convert.hpp>
#include <aurora/rmlui.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_meter2_info.h"
#include "d/d_pane_class.h"

namespace dusk::ui {
namespace {

constexpr std::string_view kScheme = "item";
constexpr std::string_view kSourcePrefix = "item://";
constexpr std::string_view kMeterScheme = "meter";
constexpr std::string_view kMeterSourcePrefix = "meter://";
constexpr size_t kItemTextureBufferSize = 0xC00;
constexpr size_t kMaxCachedIcons = 128;
constexpr uint64_t kMeterTextureSourceSlots = 8;
constexpr uint32_t kMinRenderedPaneIconSize = 128;
constexpr uint32_t kMaxRenderedPaneIconSize = 1024;

struct alignas(32) ItemTextureBuffer {
    std::array<std::byte, kItemTextureBufferSize> bytes{};

    std::byte* data() noexcept { return bytes.data(); }
    const std::byte* data() const noexcept { return bytes.data(); }
};

struct CachedIcon {
    std::vector<uint8_t> pixels;
    uint32_t width = 0;
    uint32_t height = 0;
};

struct RuntimeIconState {
    CachedIcon icon;
    uint64_t revision = 0;
    bool valid = false;
};

struct LayerColors {
    JUtility::TColor black;
    JUtility::TColor white;
    std::array<JUtility::TColor, 4> corner;
};

struct RectF {
    float left = std::numeric_limits<float>::max();
    float top = std::numeric_limits<float>::max();
    float right = std::numeric_limits<float>::lowest();
    float bottom = std::numeric_limits<float>::lowest();

    bool valid() const noexcept { return left < right && top < bottom; }
    float width() const noexcept { return right - left; }
    float height() const noexcept { return bottom - top; }

    void include(float x, float y) noexcept {
        if (!std::isfinite(x) || !std::isfinite(y)) {
            return;
        }
        left = std::min(left, x);
        top = std::min(top, y);
        right = std::max(right, x);
        bottom = std::max(bottom, y);
    }

    void include(const RectF& rect) noexcept {
        if (!rect.valid()) {
            return;
        }
        include(rect.left, rect.top);
        include(rect.right, rect.bottom);
    }
};

struct PictureLayer {
    J2DPicture* picture = nullptr;
    RectF rect;
    uint8_t alpha = 0;
};

struct SurfaceDeleter {
    void operator()(SDL_Surface* surface) const noexcept { SDL_DestroySurface(surface); }
};

using SurfacePtr = std::unique_ptr<SDL_Surface, SurfaceDeleter>;

std::unordered_map<std::string, CachedIcon>& icon_cache() {
    static auto* cache = new std::unordered_map<std::string, CachedIcon>();
    return *cache;
}

RuntimeIconState& midna_icon_state() {
    static auto* state = new RuntimeIconState();
    return *state;
}

std::string_view strip_query(std::string_view path) noexcept {
    const auto queryPos = path.find_first_of("?#");
    if (queryPos != std::string_view::npos) {
        path = path.substr(0, queryPos);
    }
    return path;
}

std::optional<u8> parse_item_no(std::string_view text) noexcept {
    if (text.starts_with("0x") || text.starts_with("0X")) {
        text.remove_prefix(2);
    }
    unsigned value = 0;
    const auto* first = text.data();
    const auto* last = text.data() + text.size();
    const auto [ptr, ec] = std::from_chars(first, last, value, 16);
    if (ec != std::errc() || ptr != last || value > 0xFF) {
        return std::nullopt;
    }
    return static_cast<u8>(value);
}

bool is_valid_icon_item(u8 itemNo) noexcept {
    return itemNo != 0 && itemNo != dItemNo_NONE_e;
}

u8 item_icon_texture_item(u8 itemNo) noexcept {
    if (itemNo == dItemNo_LIGHT_ARROW_e) {
        return dItemNo_BOW_e;
    }
    return itemNo;
}

std::optional<u8> selected_slot_item(int slot) noexcept {
    const u8 itemNo = dComIfGp_getSelectItem(slot);
    if (!is_valid_icon_item(itemNo)) {
        return std::nullopt;
    }
    return item_icon_texture_item(itemNo);
}

bool is_sword_item(u8 itemNo) noexcept {
    switch (itemNo) {
    case dItemNo_WOOD_STICK_e:
    case dItemNo_SWORD_e:
    case dItemNo_MASTER_SWORD_e:
    case dItemNo_LIGHT_SWORD_e:
        return true;
    default:
        return false;
    }
}

std::optional<u8> b_button_item() noexcept {
    const u8 action = dComIfGp_getAStatus();
    if (action == 0x26 || action == 0x2E) {
        const u8 sword = dComIfGs_getSelectEquipSword();
        if (is_sword_item(sword)) {
            return sword;
        }
        return std::nullopt;
    }
    if (action == 0x4F) {
        return dItemNo_LURE_ROD_e;
    }
    return std::nullopt;
}

std::optional<u8> item_for_source(std::string_view source) noexcept {
    if (!source.starts_with(kSourcePrefix)) {
        return std::nullopt;
    }

    std::string_view path = strip_query(source.substr(kSourcePrefix.size()));
    if (path.starts_with("item/")) {
        path.remove_prefix(5);
        const auto itemNo = parse_item_no(path);
        if (itemNo && is_valid_icon_item(*itemNo)) {
            return item_icon_texture_item(*itemNo);
        }
        return std::nullopt;
    }
    if (path == "slot/x") {
        return selected_slot_item(0);
    }
    if (path == "slot/y") {
        return selected_slot_item(1);
    }
    if (path == "button/b") {
        return b_button_item();
    }
    return std::nullopt;
}

uint32_t item_revision(u8 itemNo) noexcept {
    uint32_t revision = itemNo;
    revision = revision * 131u + g_meter2_info.getItemType(itemNo);

    if (itemNo == dItemNo_KANTERA_e || itemNo == dItemNo_KANTERA2_e) {
        revision = revision * 131u + (dComIfGs_getOil() == 0 ? 0u : 1u);
    }
    if (itemNo == dItemNo_COPY_ROD_e) {
        auto* player = daPy_getPlayerActorClass();
        revision = revision * 131u + (player != nullptr && player->checkCopyRodTopUse() ? 1u : 0u);
    }
    return revision;
}

std::string item_source_for_item(u8 itemNo) {
    itemNo = item_icon_texture_item(itemNo);
    return fmt::format("{}://item/{:02x}?rev={:08x}", kScheme, itemNo, item_revision(itemNo));
}

std::optional<int> selected_slot_count(int slot) noexcept {
    const u8 itemNo = dComIfGp_getSelectItem(slot);
    if (!is_valid_icon_item(itemNo)) {
        return std::nullopt;
    }
    if (item_icon_texture_item(itemNo) == dItemNo_KANTERA_e ||
        item_icon_texture_item(itemNo) == dItemNo_KANTERA2_e)
    {
        return std::nullopt;
    }

    int count = 0;
    int max = 0;
    switch (itemNo) {
    case dItemNo_BOW_e:
    case dItemNo_LIGHT_ARROW_e:
    case dItemNo_ARROW_LV1_e:
    case dItemNo_ARROW_LV2_e:
    case dItemNo_ARROW_LV3_e:
    case dItemNo_HAWK_ARROW_e:
        count = dComIfGs_getArrowNum();
        max = dComIfGs_getArrowMax();
        break;
    case dItemNo_BOMB_ARROW_e:
        count = std::min<int>(dComIfGp_getSelectItemNum(slot), dComIfGs_getArrowNum());
        max = std::max<int>(dComIfGp_getSelectItemMaxNum(slot), dComIfGs_getArrowMax());
        break;
    default:
        count = dComIfGp_getSelectItemNum(slot);
        max = dComIfGp_getSelectItemMaxNum(slot);
        break;
    }
    if (max <= 0) {
        return std::nullopt;
    }
    return std::clamp(count, 0, max);
}

aurora::gfx::ConvertedTexture decode_timg(const ResTIMG* image) {
    if (image == nullptr || image->width.host() == 0 || image->height.host() == 0) {
        return {};
    }

    const auto* base = reinterpret_cast<const uint8_t*>(image);
    const auto width = image->width.host();
    const auto height = image->height.host();
    const uint32_t textureSize = GXGetTexBufferSize(width, height, image->format, GX_FALSE, 0);
    const auto* textureData = base + static_cast<int32_t>(image->imageOffset);

    if (image->indexTexture != 0) {
        const auto* paletteData = base + static_cast<int32_t>(image->paletteOffset);
        return aurora::gfx::convert_texture_palette(image->format, width, height, 1,
            aurora::ArrayRef{textureData, textureSize}, static_cast<GXTlutFmt>(image->colorFormat),
            image->numColors,
            aurora::ArrayRef{paletteData, static_cast<size_t>(image->numColors) * 2});
    }

    return aurora::gfx::convert_texture(
        image->format, width, height, 1, aurora::ArrayRef{textureData, textureSize});
}

uint8_t lerp_u8(uint8_t a, uint8_t b, uint32_t t) noexcept {
    return static_cast<uint8_t>(
        (static_cast<uint32_t>(a) * (255u - t) + static_cast<uint32_t>(b) * t) / 255u);
}

JUtility::TColor lerp_color(
    const JUtility::TColor& a, const JUtility::TColor& b, uint32_t t) noexcept {
    return {
        lerp_u8(a.r, b.r, t),
        lerp_u8(a.g, b.g, t),
        lerp_u8(a.b, b.b, t),
        lerp_u8(a.a, b.a, t),
    };
}

JUtility::TColor bilerp_corner(
    const LayerColors& colors, uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
    const uint32_t u = width > 1 ? (x * 255u) / (width - 1u) : 0u;
    const uint32_t v = height > 1 ? (y * 255u) / (height - 1u) : 0u;
    const JUtility::TColor top = lerp_color(colors.corner[0], colors.corner[1], u);
    const JUtility::TColor bottom = lerp_color(colors.corner[2], colors.corner[3], u);
    return lerp_color(top, bottom, v);
}

std::array<uint8_t, 4> apply_layer_colors(std::span<const uint8_t, 4> src,
    const LayerColors& colors, uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
    std::array out{
        lerp_u8(colors.black.r, colors.white.r, src[0]),
        lerp_u8(colors.black.g, colors.white.g, src[1]),
        lerp_u8(colors.black.b, colors.white.b, src[2]),
        src[3],
    };

    const auto corner = bilerp_corner(colors, x, y, width, height);
    out[0] = static_cast<uint8_t>((static_cast<uint32_t>(out[0]) * corner.r) / 255u);
    out[1] = static_cast<uint8_t>((static_cast<uint32_t>(out[1]) * corner.g) / 255u);
    out[2] = static_cast<uint8_t>((static_cast<uint32_t>(out[2]) * corner.b) / 255u);
    out[3] = static_cast<uint8_t>((static_cast<uint32_t>(out[3]) * corner.a) / 255u);
    return out;
}

void blend_premultiplied(uint8_t* dst, const std::array<uint8_t, 4>& src) noexcept {
    const uint32_t srcAlpha = src[3];
    const uint32_t invAlpha = 255u - srcAlpha;
    const uint32_t srcR = (static_cast<uint32_t>(src[0]) * srcAlpha) / 255u;
    const uint32_t srcG = (static_cast<uint32_t>(src[1]) * srcAlpha) / 255u;
    const uint32_t srcB = (static_cast<uint32_t>(src[2]) * srcAlpha) / 255u;

    dst[0] = static_cast<uint8_t>(
        std::min(255u, srcR + (static_cast<uint32_t>(dst[0]) * invAlpha) / 255u));
    dst[1] = static_cast<uint8_t>(
        std::min(255u, srcG + (static_cast<uint32_t>(dst[1]) * invAlpha) / 255u));
    dst[2] = static_cast<uint8_t>(
        std::min(255u, srcB + (static_cast<uint32_t>(dst[2]) * invAlpha) / 255u));
    dst[3] = static_cast<uint8_t>(
        std::min(255u, srcAlpha + (static_cast<uint32_t>(dst[3]) * invAlpha) / 255u));
}

LayerColors layer_colors(const J2DPicture& picture) noexcept {
    return {
        .black = picture.getBlack(),
        .white = picture.getWhite(),
        .corner = {picture.corner(0), picture.corner(1), picture.corner(2), picture.corner(3)},
    };
}

LayerColors layer_colors(J2DPicture& picture, uint8_t alpha) noexcept {
    std::array<JUtility::TColor, 4> corners{};
    picture.getNewColor(corners.data());
    for (auto& corner : corners) {
        corner.a = static_cast<uint8_t>((static_cast<uint32_t>(corner.a) * alpha) / 255u);
    }
    return {
        .black = picture.getBlack(),
        .white = picture.getWhite(),
        .corner = corners,
    };
}

std::optional<CachedIcon> render_item_icon(u8 itemNo) {
    std::array<ItemTextureBuffer, 4> buffers{};
    std::array<J2DPicture, 4> pictures{};

    const int textureCount =
        dMeter2Info_readItemTexture(itemNo, buffers[0].data(), &pictures[0], buffers[1].data(),
            &pictures[1], buffers[2].data(), &pictures[2], buffers[3].data(), &pictures[3], -1);
    if (textureCount <= 0) {
        return std::nullopt;
    }

    std::array<aurora::gfx::ConvertedTexture, 4> decodedLayers{};
    std::array<LayerColors, 4> colors{};
    int decodedCount = 0;
    for (int i = 0; i < textureCount && i < static_cast<int>(decodedLayers.size()); ++i) {
        auto decoded = decode_timg(reinterpret_cast<const ResTIMG*>(buffers[i].data()));
        if (decoded.data.empty()) {
            continue;
        }
        colors[decodedCount] = layer_colors(pictures[i]);
        decodedLayers[decodedCount] = std::move(decoded);
        ++decodedCount;
    }
    if (decodedCount == 0) {
        return std::nullopt;
    }

    CachedIcon icon{
        .width = decodedLayers[0].width,
        .height = decodedLayers[0].height,
    };
    icon.pixels.assign(static_cast<size_t>(icon.width) * static_cast<size_t>(icon.height) * 4, 0);

    for (int layer = 0; layer < decodedCount; ++layer) {
        const auto& decoded = decodedLayers[layer];
        for (uint32_t y = 0; y < icon.height; ++y) {
            const uint32_t sourceY = decoded.height > 0 ? (y * decoded.height) / icon.height : 0;
            for (uint32_t x = 0; x < icon.width; ++x) {
                const uint32_t sourceX = decoded.width > 0 ? (x * decoded.width) / icon.width : 0;
                const size_t sourceOffset =
                    (static_cast<size_t>(sourceY) * decoded.width + static_cast<size_t>(sourceX)) *
                    4;
                if (sourceOffset + 3 >= decoded.data.size()) {
                    continue;
                }

                const std::span<const uint8_t, 4> sourcePixel(
                    decoded.data.data() + sourceOffset, 4);
                const auto pixel =
                    apply_layer_colors(sourcePixel, colors[layer], x, y, icon.width, icon.height);
                uint8_t* destination =
                    icon.pixels.data() +
                    (static_cast<size_t>(y) * icon.width + static_cast<size_t>(x)) * 4;
                blend_premultiplied(destination, pixel);
            }
        }
    }

    return icon;
}

SurfacePtr create_rgba_surface(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0 ||
        width > static_cast<uint32_t>(std::numeric_limits<int>::max()) ||
        height > static_cast<uint32_t>(std::numeric_limits<int>::max()))
    {
        return {};
    }

    return SurfacePtr{SDL_CreateSurface(
        static_cast<int>(width), static_cast<int>(height), SDL_PIXELFORMAT_RGBA32)};
}

bool lock_surface(SDL_Surface* surface) noexcept {
    return surface != nullptr && (!SDL_MUSTLOCK(surface) || SDL_LockSurface(surface));
}

void unlock_surface(SDL_Surface* surface) noexcept {
    if (surface != nullptr && SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
}

SurfacePtr create_layer_surface(
    const aurora::gfx::ConvertedTexture& decoded, const LayerColors& colors) {
    if (decoded.width == 0 || decoded.height == 0 || decoded.data.empty()) {
        return {};
    }

    auto surface = create_rgba_surface(decoded.width, decoded.height);
    if (!surface || !lock_surface(surface.get())) {
        return {};
    }

    for (uint32_t y = 0; y < decoded.height; ++y) {
        auto* destination = static_cast<uint8_t*>(surface->pixels) +
                            static_cast<size_t>(y) * static_cast<size_t>(surface->pitch);
        for (uint32_t x = 0; x < decoded.width; ++x) {
            const size_t sourceOffset =
                (static_cast<size_t>(y) * decoded.width + static_cast<size_t>(x)) * 4;
            if (sourceOffset + 3 >= decoded.data.size()) {
                continue;
            }

            const std::span<const uint8_t, 4> sourcePixel(decoded.data.data() + sourceOffset, 4);
            const auto pixel =
                apply_layer_colors(sourcePixel, colors, x, y, decoded.width, decoded.height);
            std::memcpy(destination + static_cast<size_t>(x) * 4, pixel.data(), pixel.size());
        }
    }

    unlock_surface(surface.get());
    SDL_SetSurfaceBlendMode(surface.get(), SDL_BLENDMODE_BLEND);
    return surface;
}

std::optional<CachedIcon> icon_from_surface(SDL_Surface* surface) {
    if (surface == nullptr || surface->w <= 0 || surface->h <= 0) {
        return std::nullopt;
    }

    CachedIcon icon{
        .width = static_cast<uint32_t>(surface->w),
        .height = static_cast<uint32_t>(surface->h),
    };
    const size_t rowSize = static_cast<size_t>(icon.width) * 4u;
    icon.pixels.resize(rowSize * static_cast<size_t>(icon.height));

    if (!lock_surface(surface)) {
        return std::nullopt;
    }

    for (uint32_t y = 0; y < icon.height; ++y) {
        const auto* source = static_cast<const uint8_t*>(surface->pixels) +
                             static_cast<size_t>(y) * static_cast<size_t>(surface->pitch);
        auto* destination = icon.pixels.data() + static_cast<size_t>(y) * rowSize;
        std::memcpy(destination, source, rowSize);
    }

    unlock_surface(surface);
    return icon;
}

RectF pane_global_rect(J2DPane* pane) noexcept {
    RectF rect;
    CPaneMgr paneMgr;
    Mtx m;
    for (u8 i = 0; i < 4; ++i) {
        const Vec vertex = paneMgr.getGlobalVtx(pane, &m, i, false, 0);
        rect.include(vertex.x, vertex.y);
    }
    return rect;
}

uint8_t effective_pane_alpha(J2DPane& pane, uint8_t parentAlpha) noexcept {
    uint32_t alpha = pane.getAlpha();
    if (pane.isInfluencedAlpha()) {
        alpha = alpha * parentAlpha / 255u;
    }
    return static_cast<uint8_t>(alpha);
}

void collect_picture_layers(
    J2DPane* pane, std::vector<PictureLayer>& layers, uint8_t parentAlpha = 255) noexcept {
    if (pane == nullptr || !pane->isVisible()) {
        return;
    }

    const uint8_t paneAlpha = effective_pane_alpha(*pane, parentAlpha);
    if (paneAlpha == 0) {
        return;
    }

    if (pane->getKind() == MULTI_CHAR('PIC1') || pane->getKind() == MULTI_CHAR('PIC2')) {
        auto* picture = static_cast<J2DPicture*>(pane);
        if (picture->getTexture(0) != nullptr) {
            RectF rect = pane_global_rect(pane);
            if (rect.valid()) {
                layers.push_back({
                    .picture = picture,
                    .rect = rect,
                    .alpha = paneAlpha,
                });
            }
        }
    }

    for (J2DPane* child = pane->getFirstChildPane(); child != nullptr;
        child = child->getNextChildPane())
    {
        collect_picture_layers(child, layers, paneAlpha);
    }
}

std::optional<uint32_t> icon_dimension(float value) noexcept {
    if (!std::isfinite(value) || value <= 0.0f) {
        return std::nullopt;
    }

    const auto dimension = static_cast<uint32_t>(std::ceil(value));
    if (dimension == 0 || dimension > kMaxRenderedPaneIconSize) {
        return std::nullopt;
    }
    return dimension;
}

float pane_icon_render_scale(const std::vector<PictureLayer>& layers, const RectF& canvas) {
    float scale = 1.0f;
    for (const auto& layer : layers) {
        if (layer.picture == nullptr || !layer.rect.valid() || layer.rect.width() <= 0.0f ||
            layer.rect.height() <= 0.0f)
        {
            continue;
        }

        auto* texture = layer.picture->getTexture(0);
        const ResTIMG* image = texture != nullptr ? texture->getTexInfo() : nullptr;
        if (image == nullptr || image->width.host() == 0 || image->height.host() == 0) {
            continue;
        }

        scale = std::max(scale, static_cast<float>(image->width) / layer.rect.width());
        scale = std::max(scale, static_cast<float>(image->height) / layer.rect.height());
    }

    const float canvasMax = std::max(canvas.width(), canvas.height());
    if (canvasMax <= 0.0f) {
        return scale;
    }

    const float minScale = static_cast<float>(kMinRenderedPaneIconSize) / canvasMax;
    const float maxScale = static_cast<float>(kMaxRenderedPaneIconSize) / canvasMax;
    return std::clamp(std::max(scale, minScale), 1.0f, maxScale);
}

void composite_picture_layer(
    SDL_Surface& icon, const RectF& canvas, const PictureLayer& layer, float renderScale) {
    if (layer.picture == nullptr || !layer.rect.valid()) {
        return;
    }

    auto* texture = layer.picture->getTexture(0);
    if (texture == nullptr) {
        return;
    }

    auto decoded = decode_timg(texture->getTexInfo());
    if (decoded.data.empty() || decoded.width == 0 || decoded.height == 0) {
        return;
    }

    const auto colors = layer_colors(*layer.picture, layer.alpha);
    auto layerSurface = create_layer_surface(decoded, colors);
    if (!layerSurface) {
        return;
    }

    const float dstLeft = (layer.rect.left - canvas.left) * renderScale;
    const float dstTop = (layer.rect.top - canvas.top) * renderScale;
    const float dstRight = (layer.rect.right - canvas.left) * renderScale;
    const float dstBottom = (layer.rect.bottom - canvas.top) * renderScale;
    const float dstWidth = dstRight - dstLeft;
    const float dstHeight = dstBottom - dstTop;
    if (dstWidth <= 0.0f || dstHeight <= 0.0f) {
        return;
    }

    const int x0 = std::clamp(static_cast<int>(std::floor(dstLeft)), 0, icon.w);
    const int y0 = std::clamp(static_cast<int>(std::floor(dstTop)), 0, icon.h);
    const int x1 = std::clamp(static_cast<int>(std::ceil(dstRight)), 0, icon.w);
    const int y1 = std::clamp(static_cast<int>(std::ceil(dstBottom)), 0, icon.h);
    if (x0 >= x1 || y0 >= y1) {
        return;
    }

    SDL_Rect destinationRect{
        .x = x0,
        .y = y0,
        .w = x1 - x0,
        .h = y1 - y0,
    };
    SDL_BlitSurfaceScaled(
        layerSurface.get(), nullptr, &icon, &destinationRect, SDL_SCALEMODE_LINEAR);
}

std::optional<CachedIcon> render_j2d_pane_icon(J2DPane* pane) {
    std::vector<PictureLayer> layers;
    collect_picture_layers(pane, layers);
    if (layers.empty()) {
        return std::nullopt;
    }

    RectF canvas;
    for (const auto& layer : layers) {
        canvas.include(layer.rect);
    }
    if (!canvas.valid()) {
        return std::nullopt;
    }

    const float renderScale = pane_icon_render_scale(layers, canvas);
    auto width = icon_dimension(canvas.width() * renderScale);
    auto height = icon_dimension(canvas.height() * renderScale);
    if (!width || !height) {
        return std::nullopt;
    }

    auto surface = create_rgba_surface(*width, *height);
    if (!surface) {
        return std::nullopt;
    }

    for (const auto& layer : layers) {
        composite_picture_layer(*surface, canvas, layer, renderScale);
    }

    return icon_from_surface(surface.get());
}

std::optional<aurora::rmlui::RuntimeTexture> icon_provider(std::string_view source) {
    const auto itemNo = item_for_source(source);
    if (!itemNo) {
        return std::nullopt;
    }

    auto& cache = icon_cache();
    const std::string key(source);
    auto it = cache.find(key);
    if (it == cache.end()) {
        auto icon = render_item_icon(*itemNo);
        if (!icon) {
            return std::nullopt;
        }
        if (cache.size() >= kMaxCachedIcons) {
            cache.erase(cache.begin());
        }
        it = cache.emplace(key, std::move(*icon)).first;
    }

    const auto& icon = it->second;
    return aurora::rmlui::RuntimeTexture{
        .width = icon.width,
        .height = icon.height,
        .rgba8 =
            std::span(reinterpret_cast<const std::byte*>(icon.pixels.data()), icon.pixels.size()),
        .premultipliedAlpha = true,
    };
}

std::optional<aurora::rmlui::RuntimeTexture> meter_texture_provider(std::string_view source) {
    if (!source.starts_with(kMeterSourcePrefix)) {
        return std::nullopt;
    }

    const std::string name(strip_query(source.substr(kMeterSourcePrefix.size())));
    if (name != "midna") {
        return std::nullopt;
    }

    const auto& state = midna_icon_state();
    if (!state.valid) {
        return std::nullopt;
    }

    return aurora::rmlui::RuntimeTexture{
        .width = state.icon.width,
        .height = state.icon.height,
        .rgba8 = std::span(
            reinterpret_cast<const std::byte*>(state.icon.pixels.data()), state.icon.pixels.size()),
        .premultipliedAlpha = true,
    };
}

}  // namespace

void register_icon_texture_provider() noexcept {
    aurora::rmlui::register_texture_provider(std::string(kScheme), icon_provider);
    aurora::rmlui::register_texture_provider(std::string(kMeterScheme), meter_texture_provider);
}

void unregister_icon_texture_provider() noexcept {
    aurora::rmlui::unregister_texture_provider(kScheme);
    aurora::rmlui::unregister_texture_provider(kMeterScheme);
    icon_cache().clear();
    midna_icon_state() = {};
}

void update_midna_icon_texture(J2DPane* pane) noexcept {
    auto& state = midna_icon_state();
    if (pane == nullptr || !pane->isVisible()) {
        if (state.valid) {
            state.valid = false;
            state.icon = {};
            state.revision++;
        }
        return;
    }

    auto icon = render_j2d_pane_icon(pane);
    if (!icon) {
        if (state.valid) {
            state.valid = false;
            state.icon = {};
            state.revision++;
        }
        return;
    }

    if (!state.valid || state.icon.width != icon->width || state.icon.height != icon->height ||
        state.icon.pixels != icon->pixels)
    {
        state.icon = std::move(*icon);
        state.valid = true;
        state.revision++;
    }
}

std::string midna_icon_source() {
    const auto& state = midna_icon_state();
    if (!state.valid) {
        return "";
    }
    return fmt::format(
        "{}://midna?slot={}", kMeterScheme, state.revision % kMeterTextureSourceSlots);
}

uint64_t midna_icon_revision() noexcept {
    const auto& state = midna_icon_state();
    return state.valid ? state.revision : 0;
}

std::string item_icon_source_for_button(Control control) {
    std::optional<u8> itemNo;
    switch (control) {
    case Control::X:
        itemNo = selected_slot_item(0);
        break;
    case Control::Y:
        itemNo = selected_slot_item(1);
        break;
    case Control::B:
        itemNo = b_button_item();
        break;
    default:
        break;
    }
    if (!itemNo) {
        return {};
    }
    return item_source_for_item(*itemNo);
}

std::string item_count_label_for_button(Control control) {
    std::optional<int> count;
    switch (control) {
    case Control::X:
        count = selected_slot_count(0);
        break;
    case Control::Y:
        count = selected_slot_count(1);
        break;
    default:
        break;
    }
    if (!count) {
        return {};
    }
    return fmt::format("{}", *count);
}

std::optional<float> item_oil_fill_for_button(Control control) noexcept {
    std::optional<u8> itemNo;
    switch (control) {
    case Control::X:
        itemNo = selected_slot_item(0);
        break;
    case Control::Y:
        itemNo = selected_slot_item(1);
        break;
    default:
        break;
    }
    if (!itemNo || (*itemNo != dItemNo_KANTERA_e && *itemNo != dItemNo_KANTERA2_e)) {
        return std::nullopt;
    }

    const int maxOil = dComIfGs_getMaxOil();
    if (maxOil <= 0) {
        return std::nullopt;
    }
    return std::clamp(
        static_cast<float>(dComIfGs_getOil()) / static_cast<float>(maxOil), 0.0f, 1.0f);
}

}  // namespace dusk::ui

#else

namespace dusk::ui {

void register_icon_texture_provider() noexcept {}
void unregister_icon_texture_provider() noexcept {}
void update_midna_icon_texture(J2DPane*) noexcept {}
std::string midna_icon_source() {
    return {};
}
uint64_t midna_icon_revision() noexcept {
    return 0;
}
std::string item_icon_source_for_button(Control) {
    return {};
}
std::string item_count_label_for_button(Control) {
    return {};
}
std::optional<float> item_oil_fill_for_button(Control) noexcept {
    return std::nullopt;
}

}  // namespace dusk::ui

#endif
