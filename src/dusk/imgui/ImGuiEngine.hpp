#pragma once

#include <memory>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace dusk {
class ImGuiEngine {
public:
    static ImFont* fontNormal;
    static ImFont* fontLarge;
    static ImFont* fontExtraLarge;
    static ImFont* fontMono;
    static ImTextureID orgIcon;
    static ImTextureID duskLogo;
    // DPI-derived font scale baked at init time by ImGuiEngine_Initialize(), before the user's
    // Debug UI Scale preference is applied. Multiply this by that preference every frame to get
    // io.FontGlobalScale, rather than overwriting FontGlobalScale directly (which would require a
    // full re-init/font atlas rebake any time the preference changes).
    static float baseFontScale;
};

void ImGuiEngine_Initialize(float scale);
void ImGuiEngine_AddTextures();

struct Image {
    std::unique_ptr<uint8_t[]> data;
    size_t size;
    uint32_t width;
    uint32_t height;
};
Image GetImage(const std::string& path);

#if (defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_MACCATALYST) || defined(__ANDROID__)
inline constexpr bool IsMobile = true;
#else
inline constexpr bool IsMobile = false;
#endif
}  // namespace dusk
