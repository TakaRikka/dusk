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
    static ImTextureID duskIcon;
};

void ImGuiEngine_Initialize(float scale);
void ImGuiEngine_AddTextures();

struct Image {
    std::unique_ptr<uint8_t[]> data;
    size_t size;
    uint32_t width;
    uint32_t height;
};
Image GetImage(std::string_view path);
}  // namespace dusk
