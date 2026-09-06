#include "dusk/texture_replacements.hpp"

#include <aurora/texture.hpp>
#include <fmt/format.h>

#include <array>
#include <cassert>
#include <span>
#include <numbers>

namespace {

constexpr u16 kMapIconResolutionMultiplier = 4;
constexpr u16 kMapImageSide = 16 * kMapIconResolutionMultiplier;
constexpr u32 kMapImageTotalPixels = kMapImageSide * kMapImageSide;

// give higher priority to user and mod replacements
constexpr auto kInternalTextureReplacementPriority = dusk::texture_replacements::kUserTextureReplacementPriority - 1;

typedef std::function<u8(size_t, size_t)> PaintI8Fn;

enum class ArcIndex : int {
    Circle16 = 82,  // map_icon_circle16x16_4i.bti - simple circle
    Circle   = 76,  // im_map_icon_circle_4i.bti - outlined circle
    Nijumaru = 78,  // im_map_icon_nijumaru_4i.bti - concentric rings
    Enter    = 77,  // im_map_icon_enter_4i.bti - outlined octagram
    TryForce = 81,  // im_map_icon_try_force_4i.bti - outlined circle with triangle
};

struct Replacement {
    ArcIndex index;
    PaintI8Fn painter;
};

struct Icon {
    u8* origData = nullptr;
    std::unique_ptr<u8[]> newData;
    std::string label;
    std::optional<aurora::texture::ReplacementRegistration> reg;
};

bool s_initialized = false;
bool s_active = false;
std::unordered_map<int, Icon> s_icons;

void paint_i8(std::span<u8> dst, size_t width, PaintI8Fn paint) {
    assert(width % 8 == 0 && dst.size() % 32 == 0);

    const auto blocksAcross = width >> 3;

    for (size_t i = 0; i < dst.size(); i++) {
        // 8x4 block swizzling for I8
        const auto blockIdx = i >> 5;
        const auto localIdx = i & 31;

        const auto blockY = blockIdx / blocksAcross;
        const auto blockX = blockIdx % blocksAcross;

        const auto localY = localIdx >> 3;
        const auto localX = localIdx & 7;

        const auto x = (blockX << 3) + localX;
        const auto y = (blockY << 2) + localY;

        dst[i] = paint(x, y);
    }
}

void draw_all_replacements() {
    constexpr auto center = kMapImageSide / 2.0f;
    constexpr auto radiusSq = center * center;

    // clang-format off
    const auto replacements = std::to_array<Replacement>({
        {
            ArcIndex::Circle16,
            [=](auto x, auto y) {
                const auto dx = (x + 0.5f) - center;
                const auto dy = (y + 0.5f) - center;
                return (dx * dx + dy * dy < radiusSq) ? 0x11 : 0;
            }
        },
        {
            ArcIndex::Circle,
            [=](auto x, auto y) {
                constexpr auto innerRadius = kMapImageSide * 3.0f / 8.0f;
                constexpr auto innerRadiusSq = innerRadius * innerRadius;

                const auto dx = (x + 0.5f) - center;
                const auto dy = (y + 0.5f) - center;
                const auto dSq = dx * dx + dy * dy;

                return dSq < radiusSq ? (dSq < innerRadiusSq ? 0x22 : 0x11) : 0;
            }
        },
        {
            ArcIndex::Nijumaru,
            [=](auto x, auto y) {
                constexpr u8 nijumaruRings[] = {0x11, 0x22, 0x11, 0x11, 0x22, 0x22};

                const auto dx = (x + 0.5f) - center;
                const auto dy = (y + 0.5f) - center;
                const auto dSq = dx * dx + dy * dy;

                if (dSq < radiusSq) {
                    auto ringIndex = static_cast<size_t>(std::trunc(std::sqrt(dSq) / kMapImageSide * 12));
                    ringIndex = std::min(ringIndex, sizeof(nijumaruRings) - 1);
                    return nijumaruRings[ringIndex];
                }
                return u8{0};
            }
        },
        {
            ArcIndex::Enter,
            [=](auto x, auto y) {
                constexpr auto outlineWidth = kMapImageSide / 6.0f;

                const auto adx = std::abs((x + 0.5f) - center);
                const auto ady = std::abs((y + 0.5f) - center);
                const auto dist =
                    std::min(adx + ady, std::max(adx, ady) * std::numbers::sqrt2_v<float>) -
                    kMapImageSide / 2.0f;

                return dist > 0.0f ? 0 : (dist > -outlineWidth ? 0x22 : 0x33);
            }
        },
        {
            ArcIndex::TryForce,
            [=](auto x, auto y) {
                constexpr auto innerRadiusNorm = 5.0f / 12.0f;
                constexpr auto innerRadius = kMapImageSide * innerRadiusNorm;
                constexpr auto innerRadiusSq = innerRadius * innerRadius;
                constexpr auto triRadius = kMapImageSide * innerRadiusNorm / 2.0f;

                const auto dx = (x + 0.5f) - center;
                const auto dy = (y + 0.5f) - center;
                const auto dSq = dx * dx + dy * dy;
                const auto triSideDist = (std::numbers::sqrt3_v<float> * std::abs(dx) - dy) * 0.5f;
                const auto insideTri = std::max(dy, triSideDist) < triRadius;

                return insideTri ? 0x22 : (dSq < radiusSq ? (dSq < innerRadiusSq ? 0x33 : 0x22) : 0);
            }
        }
    });
    // clang-format on

    for (const auto r : replacements) {
        auto pixels = std::make_unique_for_overwrite<u8[]>(kMapImageTotalPixels);
        paint_i8(std::span{pixels.get(), kMapImageTotalPixels}, kMapImageSide, r.painter);
        
        auto& icon = s_icons[static_cast<int>(r.index)];
        icon.newData = std::move(pixels);
        icon.label = fmt::format("hq minimap icon {}", static_cast<int>(r.index));
    }
}

}  // namespace

namespace dusk::hq_minimap {

void register_pointer(int idx, u8* ptr) {
    if (s_initialized) {
        return;
    }

    s_icons[idx].origData = ptr;
}

void set_active(bool active) {
    s_active = active;
}

void update() {
    if (!s_initialized) {
        return;
    }

    for (auto& [idx, icon] : s_icons) {
        const bool shouldBeRegistered = s_active && icon.origData && icon.newData;

        if (shouldBeRegistered && !icon.reg) {
            aurora::texture::ReplacementKey key{aurora::texture::TexturePointerKey{icon.origData}};
            aurora::texture::RawTextureReplacement repl{
                .bytes = std::span{icon.newData.get(), kMapImageTotalPixels},
                .width = kMapImageSide,
                .height = kMapImageSide,
                .mipCount = 1,
                .gxFormat = GX_TF_I8,
                .label = icon.label,
            };
            icon.reg = aurora::texture::register_replacement(
                key, repl, {.priority = kInternalTextureReplacementPriority});
        } else if (!shouldBeRegistered && icon.reg) {
            aurora::texture::unregister_replacement(*icon.reg);
            icon.reg.reset();
        }
    }
}

void initialize_if_needed() {
    if (s_initialized) {
        return;
    }

    draw_all_replacements();
    s_initialized = true;

    update();
}

}  // namespace dusk::hq_minimap
