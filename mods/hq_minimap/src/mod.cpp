#include "mods/service.hpp"
#include "mods/svc/log.hpp"
#include "mods/svc/texture.h"

#include "d/d_com_inf_game.h"

#include <array>
#include <span>
#include <numbers>

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(TextureService, svc_texture);

namespace {
bool g_alwaysArcLoaded = false;

constexpr u16 kMapIconResolutionMultiplier = 4;
constexpr u16 kMapImageSide = 16 * kMapIconResolutionMultiplier;
constexpr u32 kMapImageTotalPixels = kMapImageSide * kMapImageSide;

typedef std::function<u8(size_t, size_t)> PaintI8Fn;

struct Replacement {
    int index;
    PaintI8Fn painter;
};

void paint_i8(std::span<u8> dst, size_t width, PaintI8Fn paint) {
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

void apply_all_replacements() {
    constexpr auto center = kMapImageSide / 2.0f;
    constexpr auto radiusSq = center * center;

    // clang-format off
    const auto replacements = std::to_array<Replacement>({
        {
            82,  // map_icon_circle16x16_4i.bti - simple circle
            [=](auto x, auto y) {
                const auto dx = (x + 0.5f) - center;
                const auto dy = (y + 0.5f) - center;
                return (dx * dx + dy * dy < radiusSq) ? 0x11 : 0;
            }
        },
        {
            76,  // im_map_icon_circle_4i.bti - outlined circle
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
            78,  // im_map_icon_nijumaru_4i.bti - concentric rings
            [=](auto x, auto y) {
                constexpr u8 nijumaruRings[] = {0x11, 0x22, 0x11, 0x11, 0x22, 0x22};

                const auto dx = (x + 0.5f) - center;
                const auto dy = (y + 0.5f) - center;
                const auto dSq = dx * dx + dy * dy;

                if (dSq < radiusSq) {
                    const auto ringIndex =
                        static_cast<size_t>(std::trunc(std::sqrt(dSq) / kMapImageSide * 12));
                    return nijumaruRings[ringIndex];
                }
                return u8{0};
            }
        },
        {
            77,  // im_map_icon_enter_4i.bti - outlined octagram
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
            81,  // im_map_icon_try_force_4i.bti - outlined circle with triangle
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
        const auto img = static_cast<ResTIMG*>(dComIfG_getObjectRes("Always", r.index));

        if (img) {
            std::array<u8, kMapImageTotalPixels> pixels;
            paint_i8(std::span{pixels}, kMapImageSide, r.painter);

            TextureKey key = TEXTURE_KEY_INIT;
            key.kind = TEXTURE_KEY_POINTER;
            key.pointer = reinterpret_cast<u8*>(img) + img->imageOffset;

            TextureData data = TEXTURE_DATA_INIT;
            data.data = pixels.data();
            data.size = pixels.size();
            data.width = data.height = kMapImageSide;
            data.gx_format = GX_TF_I8;

            svc_texture->register_data(mod_ctx, &key, &data, nullptr);
        } else {
            mods::log::error("resource at index {} not found in Always archive", r.index);
        }
    }
}
}  // namespace

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    // wait until Always.arc is loaded and apply texture replacements
    if (!g_alwaysArcLoaded) {
        if (dComIfG_syncObjectRes("Always") == 0) {
            g_alwaysArcLoaded = true;
            apply_all_replacements();
            svc_log->info(mod_ctx, "texture replacements applied");
        }
    }

    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    // TextureService will automatically unregister our replacements
    g_alwaysArcLoaded = false;
    return MOD_OK;
}
}
