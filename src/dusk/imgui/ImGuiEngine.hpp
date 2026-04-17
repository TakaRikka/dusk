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
